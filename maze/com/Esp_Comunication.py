#!/usr/bin/env python3
import os
import time
import sys, traceback
from serial.serialutil import SerialException
from serial import Serial
import Constants
import time
import struct
import _thread
import binascii
from model_class import Model

""""
#the protocol use 8 bytes
0-HEADER0
1-HEADER1
2-packet_size
3-comamnd
4-4 bytes int like aditional parameter
"""
class Esp32(): 
    def __init__(self, port=Constants.serial_port, baudrate=Constants.baud_rate, timeout=1):
        self.model=Model()
        self.port = port
        self.port_name = port
        self.baudrate = baudrate
        self.timeout = timeout
        self.encoder_count = 0
        self.writeTimeout = timeout
        self.interCharTimeout = timeout / 30.

        self.WAITING_FF = 0
        self.WAITING_AA = 1
        self.RECEIVE_LEN = 2
        self.RECEIVE_PACKAGE = 3
        self.RECEIVE_CHECK = 4
        self.HEADER0 = 0xff
        self.HEADER1 = 0xaa
        
        self.SUCCESS = 0
        self.FAIL = -1

        self.receive_state_ = self.WAITING_FF
        self.receive_check_sum_ = 0
        self.payload_command = b''
        self.payload_ack = b''
        self.payload_args = b''
        self.payload_len = 0
        self.byte_count_ = 0
        self.receive_message_length_ = 0
    
        # Keep things thread safe
        self.mutex = _thread.allocate_lock()
    def connect(self):
        while True:
            try:
                print("Connecting to Microcontroller on port", self.port, "...")
                self.port = Serial(port=self.port, baudrate=self.baudrate, timeout=self.timeout, writeTimeout=self.writeTimeout)
                # The next line is necessary to give the firmware time to wake up.
                time.sleep(1)
                print("conectado")
                # state_, val = self.get_baud()
                # if val != self.baudrate:
                #     time.sleep(1)
                #     state_, val  = self.get_baud()
                #     if val != self.baudrate:
                #         raise SerialException
                # print("Connected at", self.baudrate)
                # print("Microcontroller is ready.")
                break
            except SerialException:
                print("Serial Exception:")
    def reconnect(self):
        self.port.close()
        try:
            print("Reconnecting to Microcontroller on port", self.port, "...")
            self.port = Serial(port=self.port_name, baudrate=self.baudrate, timeout=self.timeout, writeTimeout=self.writeTimeout)
            state_, val = self.get_baud()
            if val != self.baudrate:
                state_, val  = self.get_baud()
                if val != self.baudrate:
                    raise SerialException
            
            print("Connected at", self.baudrate)
            print("Microcontroller is ready.")
        
        except SerialException:
            print("Serial Exception:")
            print(sys.exc_info())
            print("Traceback follows:")
            traceback.print_exc(file=sys.stdout)
            print("Cannot connect to Microcontroller!")


    def open(self): 
        ''' Open the serial port.
        '''
        self.port.open()

    def close(self): 
        ''' Close the serial port.
        '''
        self.port.close() 
    
    def send(self, cmd):
        ''' This command should not be used on its own: it is called by the execute commands
            below in a thread safe manner.
        '''
        self.port.write(cmd)
    
    def recv(self, timeout=0.5):
        timeout = min(timeout, self.timeout)
        ''' This command should not be used on its own: it is called by the execute commands   
            below in a thread safe manner.  Note: we use read() instead of readline() since
            readline() tends to return garbage characters from the Microcontroller
        '''
        c = ''
        value = ''
        attempts = 0
        c = self.port.read(1)
        while self.receiveFiniteStates(c) != 1:
            c = self.port.read(1)
            attempts += 1
            if attempts * self.interCharTimeout > timeout:
                return 0
        return 1

    def receiveFiniteStates(self, rx_data):
        if self.receive_state_ == self.WAITING_FF:
            #print str(binascii.b2a_hex(rx_data))
            if rx_data == b'\xff':
                self.receive_state_ = self.WAITING_AA
                self.receive_check_sum_ =0
                self.receive_message_length_ = 0
                self.byte_count_=0
                self.payload_ack = b''
                self.payload_args = b''
                self.payload_len = 0


        elif self.receive_state_ == self.WAITING_AA :
             if rx_data == b'\xaa':
                 self.receive_state_ = self.RECEIVE_LEN
                 self.receive_check_sum_ = 0
             else:
                 self.receive_state_ = self.WAITING_FF

        elif self.receive_state_ == self.RECEIVE_LEN:
             self.receive_message_length_, = struct.unpack("B",rx_data)
             self.receive_state_ = self.RECEIVE_PACKAGE
             self.receive_check_sum_ = self.receive_message_length_
        elif self.receive_state_ == self.RECEIVE_PACKAGE:
             if self.byte_count_==0:
                 self.payload_ack = rx_data
             else:
                 self.payload_args += rx_data
             uc_tmp_, = struct.unpack("B",rx_data)
             self.receive_check_sum_ = self.receive_check_sum_ + uc_tmp_
             self.byte_count_ +=1
             if self.byte_count_ >= self.receive_message_length_:
                 self.receive_state_ = self.RECEIVE_CHECK

        elif self.receive_state_ == self.RECEIVE_CHECK:
            self.receive_state_ = self.WAITING_FF
            return 1 
        else:
            self.receive_state_ = self.WAITING_FF
        return 0

    def recv(self, timeout=0.5):
        timeout = min(timeout, self.timeout)
        ''' This command should not be used on its own: it is called by the execute commands   
            below in a thread safe manner.  Note: we use read() instead of readline() since
            readline() tends to return garbage characters from the Microcontroller
        '''
        c = ''
        value = ''
        attempts = 0
        c = self.port.read(1)
        while self.receiveFiniteStates(c) != 1:
            c = self.port.read(1)
            attempts += 1
            if attempts * self.interCharTimeout > timeout:
                return 0
        return 1
            
    def recv_ack(self):
        ''' This command should not be used on its own: it is called by the execute commands
            below in a thread safe manner.
        '''
        ack = self.recv(self.timeout)
        return ack == 'OK'

    def execute(self, cmd):
        ''' Thread safe execution of "cmd" on the Microcontroller returning a single integer value.
        '''
        self.mutex.acquire()
        
        try:
            self.port.flushInput()
        except:
            pass
        
        ntries = 1
        attempts = 0
        
        try:
            self.port.write(cmd)
            res = self.recv(self.timeout)
            while attempts < ntries and res !=1 :
                try:
                    self.port.flushInput()
                    self.port.write(cmd)
                    res = self.recv(self.timeout)
                    #print "response : " + str(binascii.b2a_hex(res))
                except:
                    print("Exception executing command: " + str(binascii.b2a_hex(cmd)))
                attempts += 1
        except:
            self.mutex.release()
            print("Exception executing command: " + str(binascii.b2a_hex(cmd)))
            return 0
        
        self.mutex.release()
        return 1
#Main funtion
    def sentDetection(self):
        self.recv()
        if self.payload_ack==b'\x02':
            detection=self.model.getDetectionRight()
            cmd_str=struct.pack("4B", self.HEADER0, self.HEADER1, 0x01, detection) + struct.pack("B", detection+0x01)
            if (self.execute(cmd_str))==1 and self.payload_ack == b'\x00':
                return  self.SUCCESS
            else:
                # print("ACK", self.payload_ack, self.payload_ack == b'\x00', self.execute(cmd_str)==1)
                return self.FAIL, 0
        if self.payload_ack==b'\x03':
            detection=self.model.getDetectionLeft()
            cmd_str=struct.pack("4B", self.HEADER0, self.HEADER1, 0x01, detection) + struct.pack("B", detection+0x01)
            if (self.execute(cmd_str))==1 and self.payload_ack == b'\x00':
                return  self.SUCCESS
            else:
                # print("ACK", self.payload_ack, self.payload_ack == b'\x00', self.execute(cmd_str)==1)
                return self.FAIL, 0

    def get_baud(self):
        ''' Get the current baud rate on the serial port.
        '''
        cmd_str=struct.pack("4B", self.HEADER0, self.HEADER1, 0x01, 0x00) + struct.pack("i", 0x01)
        if (self.execute(cmd_str))==1 and self.payload_ack == b'\x00':
            val, = struct.unpack('I', self.payload_args)
            return  self.SUCCESS, val 
        else:
            # print("ACK", self.payload_ack, self.payload_ack == b'\x00', self.execute(cmd_str)==1)
            return self.FAIL, 0

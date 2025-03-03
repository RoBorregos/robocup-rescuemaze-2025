#ifndef JETSON_H
#define JETSON_H

#include "Encoder.h"
#include "Arduino.h"
class Jetson{
  public: 
    Jetson();
    void executeCommand(uint8_t packet_size, uint8_t command, uint8_t* buffer);
    void writeSerial(bool success, uint8_t* payload, int elements);
    void readSerial();
    String flag;
    uint8_t cube_offset;
    String state();
  
  private:

};

#endif
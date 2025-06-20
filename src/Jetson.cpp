#include "Jetson.h"

Jetson::Jetson(){
  // Serial.begin(baud);
}
void Jetson::getDetection(){
    Serial.flush();
    float current=millis();
    if(robot.vlx[vlxID::right].isWall()){
        getDetectionRight();
        if(robot.victim!=0) robot.kitState=kitID::kRight;
    }
    if(robot.vlx[vlxID::left].isWall()){
        getDetectionLeft();
        if(robot.victim!=0) robot.kitState=kitID::kLeft;

    }
}
void Jetson::getDetectionRight(){
    writeSerial(0x02, (uint8_t*)t, sizeof(t));
    int currentTime=millis();
    while((millis()-currentTime)<waitingTime){
        if(robot.buttonPressed==true) break;
        if(readSerial()){
            break;
        }
    }
}
void Jetson::getDetectionLeft(){
    writeSerial(0x03, (uint8_t*)t, sizeof(t));
    int currentTime=millis();
    while((millis()-currentTime)<waitingTime){
        if(robot.buttonPressed==true) break;
        if(readSerial()){
            break;
        }
    }
}

void Jetson::connect(){
    while(true){
        // writeSerial(0x00, (uint8_t*)t, sizeof(t));
        if(readSerial()){
            break;
        }
    }
}
void Jetson::writeSerial(uint8_t ack, uint8_t* payload, int elements) {
    //uint8_t ack = success ? 0x00 : 0x01;
    Serial.write(0xFF);
    Serial.write(0xAA);
    Serial.write(sizeof(uint8_t) * elements + 1); // Packet size
    Serial.write(ack); // ACK
    
    // Send payload bytes
    for (size_t i = 0; i < elements; i++) {
      Serial.write(payload[i]);
    }
  
    Serial.write(0x00); // Footer
    Serial.flush();
}

bool Jetson::readSerial() {
    static uint8_t buffer[18];
    static uint8_t index = 0;
    static uint8_t packet_size = 0;
    static uint8_t command = 0;
    static uint8_t check_sum = 0;
    
    while (Serial.available()) {
        if(robot.buttonPressed==true) break;
        buffer[index++] = Serial.read();

        // Check packet header
        if (index == 1 && buffer[0] != 0xFF) {
            index = 0;
            packet_size = 0;
            command = 0;
        }
        if (index == 2 && buffer[1] != 0xAA) {
            packet_size = 0;
            command = 0;
            index = 0;
        }
        
        // Read packet size and command
        if (index == 4) {
            packet_size = buffer[2];
            command = buffer[3];
        }
        
        // Check if the entire packet has been received
        if (index == 3 + (packet_size) + 1) {
            check_sum = buffer[index - 1];
            if (check_sum != command + 1) {
                // Checksum error
                index = 0;
                packet_size = 0;
                command = 0;
                continue;
            }
            // Execute the command
            executeCommand(packet_size, command, &buffer[4]);
            while (Serial.available() > 0) {
                if(robot.buttonPressed==true) break;
                Serial.read();  // Lee y descarta cada byte
              }              
            // Reset index and packet_size
            index = 0;
            packet_size = 0;
            return true;
        }
        
    }
    return false;
    // if serial is not available, start a counter to stop the robot if nothing is received in a time frame
    // unactive_time_ = millis();
}

void Jetson::executeCommand(uint8_t packet_size, uint8_t command, uint8_t* buffer) {
    switch (command) {
        case 0x13: // Hardware Version 
            if (packet_size == 1) { // Check packet size
                uint32_t version[] = {1};
                writeSerial(0x00, (uint8_t*)version, sizeof(version));
            }
        break;
        case 0x00: // Baud
            if (packet_size == 1) { // Check packet size
                uint32_t baud[] = {115200};
                writeSerial(0x00, (uint8_t*)baud, sizeof(baud));
            }
            break;
        case 0x0A: // harmed victim color
            if (packet_size == 1) { // Check packet size
                // robot.victim=true;
                robot.screenPrint("harmed Model");
                robot.victim = 1;
                // robot.harmedVictim();
                // memcpy(&t, buffer, sizeof(t));
                writeSerial(0x00, (uint8_t*)t, sizeof(t));
            }
            break;
        case 0x0B: // stable victim color
            if (packet_size == 1) { // Check packet size
                // robot.victim=true;
                robot.screenPrint("stable Model");
                robot.victim = 2;
                // robot.stableVictim();
                // memcpy(&t, buffer, sizeof(t));
                writeSerial(0x00, (uint8_t*)t, sizeof(t));
            }
        break;
        case 0x0C: // unharmed victim color
            if (packet_size == 1) { // Check packet size
                // robot.victim=true;
                robot.screenPrint("unharm Model");
                robot.victim = 3;
                // robot.unharmedVictim();
                // memcpy(&t, buffer, sizeof(t));
                writeSerial(0x00, (uint8_t*)t, sizeof(t));
            }
        break;
        case 0x0D: // none
            if (packet_size == 1) { // Check packet size
                // robot.screenPrint("none");
                // memcpy(&t, buffer, sizeof(t));
                writeSerial(0x00, (uint8_t*)t, sizeof(t));
            }

        break;
        case 0x07: // harm MODEL
        if (packet_size == 1) { // Check packet size
            // robot.victim=true;
            robot.screenPrint("harmed Color");
            robot.victim = 1;
            // robot.harmedVictim();
            // memcpy(&t, buffer, sizeof(t));
            writeSerial(0x00, (uint8_t*)t, sizeof(t));
        }
        break;
        case 0x08: // stable Model
        if (packet_size == 1) { // Check packet size
            // robot.victim=true;
            robot.screenPrint("stable Color");
            robot.victim = 2;
            // robot.stableVictim();
            // memcpy(&t, buffer, sizeof(t));
            writeSerial(0x00, (uint8_t*)t, sizeof(t));
        }
        break;
        case 0x09: // unharmed victim Model
        if (packet_size == 1) { // Check packet size
            // robot.victim=true;
            robot.screenPrint("unharm Color");
            robot.victim = 3;
            // robot.unharmedVictim();
            // memcpy(&t, buffer, sizeof(t));
            writeSerial(0x00, (uint8_t*)t, sizeof(t));
        }
    }
}
#ifndef JETSON_H
#define JETSON_H

#include "Encoder.h"
#include "Arduino.h"
class Jetson{
  public: 
    Jetson();
    void executeCommand(uint8_t packet_size, uint8_t command, uint8_t* buffer);
    void writeSerial(uint8_t code, uint8_t* payload, int elements);
    bool readSerial();
    void getDetectionRight();
    void getDetectionLeft();
    void getDetection();
    void getWall();
    void connect();
    uint8_t detection;
    String flag;
    uint8_t cube_offset;
    String state();

  private:
  uint32_t t[1] = {200};
  int waitingTime=2000;
};

#endif
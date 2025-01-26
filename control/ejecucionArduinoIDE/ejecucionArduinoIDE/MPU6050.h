#ifndef MPU6050_H
#define MPU6050_H
#include <Arduino.h>
#include "Simple_MPU6050.h"
class MPU6050{
private:
    
public:
    float z_rotation;//angulo de rotacion
    float angulo;
    float ypr[3] = { 0, 0, 0 };
    float xyz[3] = { 0, 0, 0 };
    MPU6050();
    void loop_mpu();
    void getAngulo();
    void PrintValues(int32_t*, uint16_t);
    void ChartValues(int32_t*, uint16_t);
    void PrintAllValues(int16_t*, int16_t*, int32_t*, uint16_t);
    void ChartAllValues(int16_t*, int16_t*, int32_t*, uint16_t);
    void PrintQuaternion(int32_t*, uint16_t);
    void PrintEuler(int32_t*, uint16_t);
    void PrintRealAccel(int16_t*, int32_t*, uint16_t);
    void PrintWorldAccel(int16_t*, int32_t*, uint16_t);
    void print_Values (int16_t *, int16_t *, int32_t *);
    void inicializarMPU6050();
};
#endif



#include "VLX.h"

VLX::VLX(){

}
VLX::VLX(const uint8_t posMux){
    mux_.setNewChannel(posMux);
}

void VLX::begin(){
    mux_.selectChannel();
    VLX_.begin();
    if (!VLX_.begin()) {
        Serial.println("¡Error al iniciar el sensor VL53L0X!");
        while (1);
    }
    Serial.println("VL53L0X iniciado correctamente.");  
}

void VLX::setMux(const uint8_t posMux) {
    mux_.setNewChannel(posMux);
}
void VLX::updateDistance() {
    mux_.selectChannel();
    VLX_.rangingTest(&measure, false);
}
float VLX::getDistance(){
    updateDistance();
    if (measure.RangeStatus != 4) {
    distance=measure.RangeMilliMeter;
    distance=distance/10;
    return distance;
    }else{
        return 180;
    }
}

void VLX::printDistance(){
    updateDistance();
    if (measure.RangeStatus != 4) {
        Serial.print("Distance: ");
        Serial.print(measure.RangeMilliMeter);
        Serial.println(" mm");
    } else {
        Serial.println("Fuera de rango.");
    }
    delay(500); 
    }
bool VLX::isWall(){
    // if(distance<kDistanceToWall){
    if(getDistance()<kDistanceToWall){
        return true;
    }else{
        return false;
    }
}




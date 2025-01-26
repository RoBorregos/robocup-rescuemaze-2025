#include "motors.h"
#include "Pins.h"
#include "Encoder.h"
motors::motors(){
}
void motors::setupMotors(){
    for(int i=0;i<4;i++){
        motor[i].initialize(Pins::digitalOne[i],Pins::digitalTwo[i],Pins::pwmPin[i],i);
        Serial.println(Pins::pwmPin[i]);
        // myPID[i].changeConstants(5,0.1,0.005,50);
    }
    setupVlx(vlxID::frontLeft);
    // for(int i=0;i<kNumVlx;i++){
        // setupVlx(0);
    // }
    delay(500);
    attachInterrupt(digitalPinToInterrupt(Pins::encoder[0]),Encoder::backRightEncoder,RISING);
    attachInterrupt(digitalPinToInterrupt(Pins::encoder[1]),Encoder::backLeftEncoder,RISING);
    attachInterrupt(digitalPinToInterrupt(Pins::encoder[2]),Encoder::frontRightEncoder,RISING);
    attachInterrupt(digitalPinToInterrupt(Pins::encoder[3]),Encoder::frontLeftEncoder,RISING);
    bno.setupBNO();
    targetAngle=0;
}
void motors::printAngle(){
    double angulo=bno.getOrientationX();
    Serial.println(angulo);
}
void motors::PID_speed(double setpoint,double angle, int reference_speed){
    PID PID;
    double output=PID.calculate_PID(setpoint,angle);
    int right_speed=reference_speed-output;
    int left_speed=reference_speed+output;
    right_speed=constrain(right_speed,30,255);
    left_speed=constrain(left_speed,30,255);
    for(int i=0;i<4;i++){
        motor[i].setSpeed((i%2==0) ? left_speed:right_speed);
    }
    delay(20);
}
void motors::PID_encoders(double targetSpeed){
    double speed_setpoint=targetSpeed;
    int reference_pwm[4];
    double lastTics[4];
    for(int i=0;i<4;i++){
        reference_pwm[i]=motor[i].getSpeed();
        lastTics[i]=motor[i].tics;
    }
    delay(100);
    for(int i=0;i<4;i++){
        double speedTics=motor[i].tics-lastTics[i];
        double error=myPID[i].calculate_PID(speed_setpoint,speedTics);
        int speed=reference_pwm[i]+error;
        speed=constrain(speed,0,255);
        motor[i].setSpeed(speed);
    }
}
void motors::ahead(){//prueba
    resetTics();
    setahead();
    while(getAvergeTics()<kTicsPerTile){
        angle=bno.getOrientationX();
        Serial.println(z_rotation);
        float speed=changeSpeedMove(true,false,kTicsPerTile);
        PID_speed(targetAngle,(targetAngle==0 ? z_rotation:angle),speed);
        // showSpeeds();
    }
    stop();
    resetTics();
}
// void motors::ahead_ultra(){
//     double distance=vlx[vlxID::frontLeft].getDistance();
//     int targetDistances[]={10,40,70,100};
//     int targetDistance=findNearest(distance,targetDistances,4);
//     setahead();
//     while(distance>targetDistance){
//         angle=bno.getOrientationX();
//         distance=vlx[vlxID::frontLeft].getDistance();
//         float speed=changeSpeedMove(false,false,targetDistance);
//         PID_speed(targetAngle,(targetAngle==0 ? z_rotation:angle),speed);
//         showSpeeds();
//     }
//     stop();resetTics();
// }
void motors::ahead_ultra(){//como  quisiera
    resetTics();
    double distance=vlx[vlxID::frontLeft].getDistance();
    // setahead();
    if(distance<maxVlxDistance){
        int targetDistances[]={edgeTileDistance,30+edgeTileDistance,60+edgeTileDistance,90+edgeTileDistance};
        int targetDistance=findNearest(distance,targetDistances,4);
        while(distance>targetDistance){
            // float changeAngle=nearWall();
            distance=vlx[vlxID::frontLeft].getDistance();
            Serial.println(distance);
            bno.getOrientationX();
            float speed=changeSpeedMove(false,false,targetDistance);
            PID_speed(targetAngle/*+changeAngle*/,(targetAngle==0 ? z_rotation:angle),speed);
            // showSpeeds();
        }
    }else{
        while(getAvergeTics()<kTicsPerTile){
            bno.getOrientationX();
            // float changeAngle=nearWall();
            float speed=changeSpeedMove(0.5,false,kTicsPerTile);
            PID_speed((targetAngle/*+changeAngle*/),(targetAngle==0 ? z_rotation:angle),speed);
            showSpeeds();
        }
    }
    stop();resetTics();
}
float motors::nearWall(){
    float changeAngle=0;
    vlx[vlxID::left].getDistance();
    vlx[vlxID::Right].getDistance();
    if(vlx[vlxID::left].distance<minDisToLateralWall){
        changeAngle=+(maxChangeAngle-(maxChangeAngle/minDisToLateralWall*vlx[vlxID::left].distance));//ecuacion de la recta
    }
    if(vlx[vlxID::Right].distance<minDisToLateralWall){
        changeAngle=-(maxChangeAngle-(maxChangeAngle/minDisToLateralWall*vlx[vlxID::Right].distance));//ecuacion de la recta
    }
    return changeAngle;
}
int motors::findNearest(int number,int numbers[],int size){
    int nearest=numbers[0];
    float minDifference=abs(number-numbers[0]);
    for(int i=1;i<size;i++){
        float currentDifference=abs(number-numbers[i]);
        if(currentDifference<minDifference){
            nearest=numbers[i];
            minDifference=currentDifference;
        }
    }
    nearest=nearest-30;
    nearest=constrain(nearest,5,95);
    return nearest;
}
void motors::back(){
    setback();
}
void motors::right(){
    targetAngle=targetAngle+90;
    rotate(targetAngle);
    if(targetAngle==360){
        targetAngle=0;
    }
}
void motors::left(){
    if(targetAngle==0){
        targetAngle=360;
    }
    targetAngle=targetAngle-90;
    rotate(targetAngle);
}
float motors::calculateAngularDistance(){
    float rightAngularDistance, leftAngularDistance;
    if(targetAngle>=angle){
        rightAngularDistance=targetAngle-angle;
        leftAngularDistance=angle+(360-targetAngle);
    }else{
        rightAngularDistance=(360-angle)+targetAngle;
        leftAngularDistance=angle-targetAngle;
    }
    return (rightAngularDistance<=leftAngularDistance) ? rightAngularDistance:-leftAngularDistance;
}
void motors::rotate(float deltaAngle){
    targetAngle=deltaAngle;
    bno.getOrientationX();
    float currentAngle,rightAngularDistance, leftAngularDistance,minInterval,maxInterval,tolerance=1;
    bool hexadecimal;
    //calculate angular distance in both directions
    if(targetAngle>=angle){
        rightAngularDistance=targetAngle-angle;
        leftAngularDistance=angle+(360-targetAngle);
    }else{
        rightAngularDistance=(360-angle)+targetAngle;
        leftAngularDistance=angle-targetAngle;
    }
    //define target interval and use angle or z_rotation
    if(targetAngle!=360 && targetAngle!=0){ 
        minInterval=(targetAngle-tolerance),maxInterval=(targetAngle+tolerance);
        hexadecimal=true;
    }else{
        minInterval=-tolerance,maxInterval=tolerance;
        hexadecimal=false;
    }
    //decide shortest route and rotate("?" es un operador ternario para remplazar if-else)
    (rightAngularDistance<=leftAngularDistance) ? setright():setleft();
    currentAngle=hexadecimal ? angle:z_rotation;
    while (currentAngle<minInterval||currentAngle>maxInterval){
        changeSpeedMove(false,true,0);
        bno.getOrientationX();
        currentAngle= hexadecimal ? angle:z_rotation;
        Serial.println(angle);
    }
    stop();
}
float motors::changeSpeedMove(bool encoders,bool rotate,int targetDistance){
    float speed;
    if(rotate==true){
        double missingAngle=abs(targetAngle-angle);
        speed=map(missingAngle,90,0,kMaxSpeedRotate,kMinSpeedRotate);
        speed=constrain(speed,kMinSpeedRotate,kMaxSpeedRotate);
        setSpeed(speed);
        return 0;
    }else{
        if(encoders==true){
            speed=map(getAvergeTics(),0,kTicsPerTile,kMaxSpeedFormard,kMinSpeedFormard);
            return speed;
        }else{
            double missingDistance=abs(vlx[vlxID::frontLeft].getDistance()-targetDistance);
            speed=map(missingDistance,30,0,kMaxSpeedFormard,kMinSpeedFormard);
            speed=constrain(speed,kMinSpeedFormard,kMaxSpeedFormard);
            return speed;
        }
    }
}
void motors::setSpeed(int speed){
    for(int i=0;i<4;i++){ 
        motor[i].setSpeed(speed);}
}
void motors::setahead(){
    for(int i=0;i<4;i++){ 
        motor[i].ahead();}
}
void motors::setback(){
    for(int i=0;i<4;i++){ 
        motor[i].back();}
}
void motors::setright(){
    motor[0].ahead();
    motor[1].back();
    motor[2].ahead();
    motor[3].back();

}
void motors::setleft(){
    motor[0].back();
    motor[1].ahead();
    motor[2].back();
    motor[3].ahead();
}
void motors::stop(){
    for(int i=0;i<4;i++){ 
        motor[i].stop();}
        setSpeed(0);
}
void motors::showSpeeds(){
    double speedM1=motor[0].getSpeed();
    double speedM2=motor[1].getSpeed();
    double speedM3=motor[2].getSpeed();
    double speedM4=motor[3].getSpeed();
    Serial.println("Velocidades:");
    Serial.print("Motor1:"); Serial.print(speedM1);
    Serial.print("Motor2:"); Serial.print(speedM2);
    Serial.print("Motor3:"); Serial.print(speedM3);
    Serial.print("Motor4:"); Serial.println(speedM4);
}
void motors::resetTics(){
    for(int i=0;i<4;i++){ 
        motor[i].resetTics();}
}
double motors::getAvergeTics(){
    double totalTics;
    for(int i=0;i<4;i++){
        totalTics+=motor[i].tics;
    }
    return totalTics/4;
}

void motors::setupVlx(const int index) {
    vlx[index].setMux(Pins::vlxPins[index]);
    vlx[index].begin();
}
void motors::printTicsSpeed(){
     // double current_time=millis()-last_time;
  // Serial.println(robot.motor[MotorID::kFrontLeft].tics);
  // if(current_time>=calculate_time){
  //   Serial.print("front left: ");
  //   Serial.println(Encoder::deltaTics[MotorID::kFrontLeft]);
  //   Serial.print("front right: ");
  //   Serial.println(Encoder::deltaTics[MotorID::kFrontRight]);
  //   Serial.print("back left: ");
  //   Serial.println(Encoder::deltaTics[MotorID::kBackLeft]);
  //   Serial.print("back right: ");
  //   Serial.println(Encoder::deltaTics[MotorID::kBackRight]);
  //   for(int i=0;i<4;i++){
  //     Encoder::deltaTics[i]=0;
  //   }
  //   last_time=millis();
  // }
}

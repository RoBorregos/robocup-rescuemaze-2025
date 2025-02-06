#include "motors.h"
#include "Pins.h"
#include "Encoder.h"
#include <WiFi.h>
WiFiServer server(1234);
WiFiClient client;
const char* ssid="RoBorregos2";
const char* password="RoBorregos2024";
motors::motors(){
}
void motors::setupMotors(){
    ////wifi////////
    // WiFi.begin(ssid,password);
    // Serial.print("Conectando");
    // while (WiFi.status() != WL_CONNECTED){
    //     delay(500);
    //     Serial.print(".");
    // }
    // Serial.println("conectado a WIFI");
    // Serial.print("Direccion IP: ");
    // Serial.println(WiFi.localIP());
    // server.begin();
    // while(!client) {
    //     client = server.available();
    //     delay(100);
    // }
    for(int i=0;i<4;i++){
        motor[i].initialize(Pins::digitalOne[i],Pins::digitalTwo[i],Pins::pwmPin[i],i);
        Serial.println(Pins::pwmPin[i]);
        // myPID[i].changeConstants(5,0.1,0.005,50);
    }
    // client.print("inicializado correctamente");
    Wire.begin();  // Inicializa el bus I2C
    setupVlx(vlxID::frontLeft);
    // setupVlx(vlxID::frontRight);
    
    setupVlx(vlxID::back);
    setupVlx(vlxID::right);
    setupVlx(vlxID::left);
    bno.setupBNO();
    double distance=vlx[vlxID::frontLeft].getDistance();
    delay(500);
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
// void motors::ahead_ultra(){
// }
void motors::ahead(){
    // double obstacleDistance=passObstacle();
    Serial.println("2");
    resetTics();
    Serial.println("3");
    double distance=vlx[vlxID::frontLeft].getDistance();
    Serial.println("4");
    setahead();
    Serial.println("5");
    if(distance<maxVlxDistance){
        int targetDistances[]={edgeTileDistance,30+edgeTileDistance,60+edgeTileDistance,90+edgeTileDistance};
        int targetDistance=findNearest(distance,targetDistances,4,true);//agregar variable de adelante atras
        while(distance>targetDistance){//poner rango
            // float changeAngle=nearWall();
            distance=vlx[vlxID::frontLeft].getDistance();
            Serial.println(distance);
            Serial.println("ahead");
            bno.getOrientationX();
            float speed=changeSpeedMove(false,false,targetDistance,true);
            PID_speed(targetAngle/*+changeAngle*/,(targetAngle==0 ? z_rotation:angle),speed);
            // showSpeeds();
        }
    }else{
        while(getAvergeTics()<kTicsPerTile/*+obstacleDistance*/){
            bno.getOrientationX();
            // float changeAngle=nearWall();
            float speed=changeSpeedMove(true,false,kTicsPerTile,false);
            PID_speed((targetAngle/*+changeAngle*/),(targetAngle==0 ? z_rotation:angle),speed);
            // printSpeeds();
        }
    }
    stop();resetTics();
}
float motors::nearWall(){
    float changeAngle=0;
    vlx[vlxID::left].getDistance();
    vlx[vlxID::right].getDistance();
    if(vlx[vlxID::left].distance<minDisToLateralWall){
        changeAngle=+(maxChangeAngle-(maxChangeAngle/minDisToLateralWall*vlx[vlxID::left].distance));//ecuacion de la recta
    }
    if(vlx[vlxID::right].distance<minDisToLateralWall){
        changeAngle=-(maxChangeAngle-(maxChangeAngle/minDisToLateralWall*vlx[vlxID::right].distance));//ecuacion de la recta
    }
    return changeAngle;
}
double motors::passObstacle(){
    double targetAngle_=targetAngle;
    if(!vlx[vlxID::frontRight].isWall() && !vlx[vlxID::frontLeft].isWall()){
        return 0;
    }
    if(vlx[vlxID::frontRight].isWall()){
        if(targetAngle=360){
        targetAngle=0;
        }
        rotate(targetAngle+45);
    }else if(vlx[vlxID::frontLeft].isWall()){
        if(targetAngle=0){
        targetAngle=360;
        }
        rotate(targetAngle-45);
    }
    int backDistanceCm=7;
    double distanceTics=backDistanceCm*kTicsPerRev/distancePerRev;
    wait(delayTime);
    resetTics();
    while(getAvergeTics()<distanceTics){
        setback();
        setSpeed(kMinPwmFormard);
    }
    stop();
    wait(delayTime);
    targetAngle=targetAngle_;
    rotate(targetAngle);
    double linearDistanceTics=distanceTics*cos((45*PI/180));
    return linearDistanceTics;
}
int motors::findNearest(int number,int numbers[],int size,bool frontVlx){
    int nearest=numbers[0];
    float minDifference=abs(number-numbers[0]);
    for(int i=1;i<size;i++){
        float currentDifference=abs(number-numbers[i]);
        if(currentDifference<minDifference){
            nearest=numbers[i];
            minDifference=currentDifference;
        }
    }
    nearest=nearest+(frontVlx ? -kTileLength:kTileLength);
    nearest=constrain(nearest,5,95);
    return nearest;
}
void motors::back(){
    setback();
}
void motors::right(){
    Serial.println("right");
    targetAngle=targetAngle+90;
    rotate(targetAngle);
    if(targetAngle==360){
        targetAngle=0;
    }
}
void motors::left(){
    Serial.println("left");
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
    Serial.println("rotate");
    targetAngle=deltaAngle;
    Serial.println("rotate1");
    delayMicroseconds(1);
    bno.getOrientationX();
    Serial.println("rotate2");
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
    Serial.println("rotate3");
    while (currentAngle<minInterval||currentAngle>maxInterval){
        changeSpeedMove(false,true,0,false);
        bno.getOrientationX();
        currentAngle= hexadecimal ? angle:z_rotation;
        Serial.println(angle);
    }
    stop();
}
float motors::changeSpeedMove(bool encoders,bool rotate,int targetDistance,bool frontVlx){
    float speed;
    double missingDistance,missingAngle;
    if(rotate==true){
        missingAngle=abs(targetAngle-angle);
        speed=map(missingAngle,90,0,kMaxPwmRotate,kMinPwmRotate);
        speed=constrain(speed,kMinPwmRotate,kMaxPwmRotate);
        setSpeed(speed);
        return 0;
    }else{
        if(encoders==true){
            speed=map(getAvergeTics(),0,kTicsPerTile,kMaxPwmFormard,kMinPwmFormard);
            missingDistance=kTileLength-(getAvergeTics()*kTileLength/kTicsPerTile);
            return speed;
        }else{
            missingDistance=abs((frontVlx ? vlx[vlxID::frontLeft].getDistance():vlx[vlxID::back].getDistance())-targetDistance);//intercambiar vlx
            // client.println(motor[0].getTicsSpeed());
            // client.println(motor[1].getTicsSpeed());
            // client.println(motor[2].getTicsSpeed());
            // client.println(motor[3].getTicsSpeed());
            // client.println(getAvergeTics());
            // client.println("/////////////////////");
            speed=frontVlx ? map(missingDistance,kTileLength,0,kMaxPwmFormard,kMinPwmFormard):map(missingDistance,0,kTileLength,kMaxPwmFormard,kMinPwmFormard);
            speed=constrain(speed,kMinPwmFormard,kMaxPwmFormard);
            return speed;
        }
    }
    // changePwm(rotate,(rotate ? missingAngle:missingDistance));
}
void motors::changePwm(bool rotate,double missing){
    double tolerance=10;
    int kp_=10;
    float kMaxSpeed, kMinSpeed;
    uint8_t *kMinPwm, *kMaxPwm;
    if (rotate) {
        kMaxSpeed = kMaxSpeedRotate;
        kMinSpeed = kMinSpeedRotate;
        kMinPwm = &kMinPwmRotate;
        kMaxPwm = &kMaxPwmRotate;
    } else {
        kMaxSpeed = kMaxSpeedFormard;
        kMinSpeed = kMinSpeedFormard;
        kMinPwm = &kMinPwmFormard;
        kMaxPwm = &kMaxPwmFormard;
    }
    float targetTicsSpeed = map(missing, rotate ? 90 : kTileLength, 0, kMaxSpeed, kMinSpeed);
    float currentSpeed = getTicsSpeed();
    if(targetTicsSpeed<(currentSpeed-tolerance)){
        *kMinPwm+=kp_;
        *kMaxPwm+=kp_;
    }else if(targetTicsSpeed>(currentSpeed+tolerance)){
        *kMinPwm-=kp_;
        *kMaxPwm-=kp_;
    } 
    kMinPwmRotate=constrain(kMinPwmRotate,0,255);
    kMaxPwmRotate=constrain(kMaxPwmRotate,0,255);
    kMinPwmFormard=constrain(kMinPwmFormard,0,255);
    kMaxPwmFormard=constrain(kMaxPwmFormard,0,255);
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
void motors::printSpeeds(){
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
double motors::getTicsSpeed(){
    double ticsSpeed;
    for(int i=0;i<4;i++){
        ticsSpeed+=motor[i].getTicsSpeed();
    }
    return ticsSpeed;
}
void motors::setupVlx(const uint8_t index) {
    vlx[index].setMux(index);
    vlx[index].begin();
}
bool motors::isWall(uint8_t direction){
    int rulet[4][4]={{0,1,2,3},{3,0,1,2},{2,3,0,1},{1,2,3,0}};
    int relativeDir;
    int deltaTargetAngle=static_cast<int>(targetAngle);
    switch(deltaTargetAngle) {
        case 0:
            relativeDir=0; 
        case 360:
            relativeDir=0;
            break;
        case 90:
            relativeDir=1; 
            break;
        case 180:
            relativeDir=2; 
            break;
        case 270:
            relativeDir=3; 
            break;
        default: 
          break;
    }
    int realPos=rulet[relativeDir][direction];
    switch(realPos) {
        case 0:
            return vlx[vlxID::frontLeft].isWall() /*&& vlx[vlxID::frontRight].isWall()*/;
        case 1:
            return vlx[vlxID::right].isWall();
        case 2:
            return vlx[vlxID::back].isWall();
        case 3:
            return vlx[vlxID::left].isWall();
        default: 
          return false;
    }
}
bool motors::rampInFront(){
    if((vlx[vlxID::frontLeft].getDistance()-vlx[vlxID::frontLeft/*down*/].getDistance())>=2){
        return true;
    }else{
        return false;
    }
}
bool motors::isRamp() {
    const double currentOrientationY = bno.getOrientationY();
    if (currentOrientationY >= kMinRampOrientation || currentOrientationY <= -kMinRampOrientation) {
        // screenPrint("Ramp detected");
        // if (currentOrientationY <= -kMinRampOrientation) {
        //     downRamp_ = true;
        //     pidWallAlignment_.setBaseSpeed(kBaseSpeedDownRamp_);
        //     pidWallAlignment_.setKp(kPDownRamp);
        // }
        // #if DEBUG_MOVEMENT
        // customPrintln("TRUE");
        // #endif
        return true;
    }
    // #ifndef DEBUG_MOVEMENT
    // customPrintln("FALSE");
    // #endif
    return false;
}
void motors::ramp(){
    PID rampPID;
    rampPID.changeConstants(7,1,1,20);
    float speedRamp;
    while(abs(bno.getOrientationY())>kMinAngleRamp){

    }
}
void motors::wait(unsigned long targetTime){
    unsigned long initialTime=millis();
    while((millis()-initialTime)<targetTime){

    }
}
void motors::wifiPrint(String message, float i){
    if (!client || !client.connected()) { 
        Serial.println("Nuevo cliente conectado.");
        return;
    }
    client.print(message);
    client.println(i);
    // Serial.println("Enviado: ");
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

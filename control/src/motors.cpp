#include "motors.h"
#include "Pins_ID.h"
#include <WiFi.h>
WiFiServer server(1234);
WiFiClient client;
const char* ssid="Roborregos";
const char* password="RoBorregos2025";
motors::motors(){
}
void motors::setupMotors(){
    // ////wifi////////
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
    for(uint8_t i=0;i<4;i++){
        motor[i].initialize(Pins::digitalOne[i],Pins::digitalTwo[i],Pins::pwmPin[i],i);
        Serial.println(Pins::pwmPin[i]);
        myPID[i].changeConstants(6.5,0.1,0.005,50);
        // myPID[i].changeConstants(7.5,0.1,0.005,50);
    }
    Wire.begin();  // Inicializa el bus I2C
    bno.setupBNO();
    
    setupVlx(vlxID::frontLeft);
    setupVlx(vlxID::left);
    
    setupVlx(vlxID::frontRight);
    // setupVlx(vlxID::front);
    setupVlx(vlxID::right);
    setupVlx(vlxID::back);
    // setupTCS();
    // tcs_.getColor();
    limitSwitch_[LimitSwitchID::kLeft].initLimitSwitch(Pins::limitSwitchPins[LimitSwitchID::kLeft]);
    limitSwitch_[LimitSwitchID::kRight].initLimitSwitch(Pins::limitSwitchPins[LimitSwitchID::kRight]);
    pinMode(Pins::checkpointPin, INPUT_PULLDOWN);
    servo.attach(Pins::servoPin);
    delay(500);
    targetAngle=0;
}
void motors::printAngle(){
    double angulo=bno.getOrientationX();
    Serial.println(angulo);
}
void motors::PID_speed(float setpoint,float angle, uint16_t reference_speed){
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
void motors::PID_Wheel(int targetSpeed,int i){
    int speed_setpoint=targetSpeed;
    int reference_pwm;
    reference_pwm=motor[i].getSpeed();
    int speedTics=motor[i].getTicsSpeed();
    float error=myPID[i].calculate_PID(speed_setpoint,speedTics);
    uint16_t speed=reference_pwm+error;
    speed=constrain(speed,0,255);
    motor[i].setSpeed(speed);
}
void motors::PID_AllWheels(int targetSpeed){
    for(uint8_t i=0;i<4;i++){
        PID_Wheel(targetSpeed,i);
    }
}
void motors::pidEncoders(int speedReference,bool ahead){
    bno.getOrientationX();
    speedReference;
    PID pidBno(0.5,0.1,0.01,1);
    float changeAngle=nearWall();
    float error=pidBno.calculate_PID(targetAngle+changeAngle,(targetAngle==0 ? z_rotation:angle));
    error=constrain(error,-8,8);//aumentar
    Serial.println(error);
    if(!ahead) error=-error;
    PID_Wheel(speedReference+error,MotorID::kFrontLeft);
    PID_Wheel(speedReference+error,MotorID::kBackLeft);
    PID_Wheel(speedReference-error,MotorID::kFrontRight);
    PID_Wheel(speedReference-error,MotorID::kBackRight);
}
void motors::ahead(){
    float obstacleDistance=passObstacle();
    resetTics();
    float distance;
    float frontDistance=vlx[vlxID::frontLeft].getDistance();
    float backDistance=vlx[vlxID::back].getDistance();
    bool encoder,frontVlx;
    if(frontDistance<maxVlxDistance){
        distance=frontDistance;
        encoder=false;
        frontVlx=true;
        // wifiPrint("vlx adelante",1);
    }else if(backDistance<maxVlxDistance-kTileLength){
        distance=backDistance;
        encoder=false;
        frontVlx=false;
        // wifiPrint("vlx atras",1);
    }else{
        encoder=true;
        // wifiPrint("encoders ",1);
    }
    if(!encoder){
        float targetDistance=findNearest(distance,targetDistances,4,frontVlx);//agregar variable de adelante atras
        while(frontVlx ? (distance>targetDistance):(distance<targetDistance)){//poner rango
            setahead();
            // float changeAngle=nearWall();
            // limitCrash();
            // checkTileColor();
            // if(blackTile) break;
            distance=(frontVlx ? vlx[vlxID::frontLeft].getDistance():vlx[vlxID::back].getDistance());
            Serial.println(distance);
            float missingDistance=abs(distance-targetDistance);
            float speed;
            speed=map(missingDistance,kTileLength,0,kMaxSpeedFormard,kMinSpeedFormard);
            // speed=(kMaxSpeedFormard+kMinSpeedFormard)/2;
            speed=constrain(speed,kMinSpeedFormard,kMaxSpeedFormard);
            pidEncoders(speed,true);
            // if(isRamp()) break;
        }
    }else if(encoder){
        Serial.println(getAvergeTics());
        while(getAvergeTics()<kTicsPerTile+obstacleDistance){
            setahead();
            // limitCrash();
            // checkTileColor();
            // if(blackTile) return;
            bno.getOrientationX();
            // float changeAngle=nearWall();
            float missingDistance=kTileLength-(getAvergeTics()*kTileLength/kTicsPerTile);
            float speed=map(missingDistance,kTileLength,0,kMaxSpeedFormard,kMinSpeedFormard);
            speed=constrain(speed,kMinSpeedFormard,kMaxSpeedFormard);
            pidEncoders(speed,true);
            // if(isRamp()) break;
        }
    }
    resetTics();
    Serial.println("aheadHecho");
    stop();resetTics();/*checkTileColor();*/
}
void motors::checkTileColor(){
    tileColor=tcs_.getColor();
    Serial.println(tileColor);
    if(tileColor==kBlackColor){
        resetTics();
        setback();
        blackTile=true;
        while(getAvergeTics()<kTicsPerTile/2){
            int speed=map(getAvergeTics(),0,kTicsPerTile/2,kMaxSpeedFormard,kMinSpeedFormard);
            pidEncoders(speed,false);
        }
        stop();resetTics();
    }else if(tileColor==kBlueColor&&inMotion==false){
        blueTile=true;
        wait(5100);
        wifiPrint("BLUE",88);
    }else if(tileColor==kCheckpointColor&&inMotion==false){
        checkpoint=true;
        wifiPrint("CHECKPOINT",99);
    }
}

void motors::ahead_ultra(){
    // double obstacleDistance=passObstacle();
    resetTics();
    float distance;
    float frontDistance=vlx[vlxID::frontLeft].getDistance();
    float backDistance=vlx[vlxID::back].getDistance();
    bool encoder,frontVlx;
    if(frontDistance<maxVlxDistance){
        distance=frontDistance;
        encoder=false;
        frontVlx=true;
        wifiPrint("vlx adelante",1);
    }else if(backDistance<maxVlxDistance-kTileLength){////////////////////////////////
        distance=backDistance;
        encoder=false;
        frontVlx=false;
        wifiPrint("vlx atras",1);
    }else{
        encoder=true;
        wifiPrint("encoders ",1);
    }
    setahead();
    if(!encoder){
        float targetDistance=findNearest(distance,targetDistances,4,frontVlx);//agregar variable de adelante atras
        while(frontVlx ? (distance>targetDistance):(distance<targetDistance)){//poner rango
            float changeAngle=nearWall();
            wifiPrint("changeAngle",changeAngle);
            // wifiPrint("targetdistance",targetDistance);
            distance=(frontVlx ? vlx[vlxID::frontLeft].getDistance():vlx[vlxID::back].getDistance());
            Serial.println(distance);
            bno.getOrientationX();
            float speed=changeSpeedMove(false,false,targetDistance,frontVlx);
            PID_speed(targetAngle+changeAngle,(targetAngle==0 ? z_rotation:angle),speed);
        }
    }else if(encoder){
        while(getAvergeTics()<kTicsPerTile/*+obstacleDistance*/){
            // wifiPrint("encodersssssss",1);
            bno.getOrientationX();
            float changeAngle=nearWall();
            float speed=changeSpeedMove(true,false,kTicsPerTile,frontVlx);
            PID_speed((targetAngle+changeAngle),(targetAngle==0 ? z_rotation:angle),speed);
        }
    }
    stop();resetTics();
}
float motors::nearWall(){
    float changeAngle=0;
    vlx[vlxID::left].getDistance();
    vlx[vlxID::right].getDistance();
    if(vlx[vlxID::left].distance<minDisToLateralWall ){
        changeAngle=maxChangeAngle;
    }else if(vlx[vlxID::right].distance<minDisToLateralWall){
        changeAngle=-maxChangeAngle;
    }
    return changeAngle;
}
float motors::passObstacle(){
    uint16_t targetAngle_=targetAngle;
    if(!vlx[vlxID::frontRight].isWall() && !vlx[vlxID::frontLeft].isWall()){
        return 0;
    }
    if(vlx[vlxID::frontRight].isWall()){
        if(targetAngle==360){
        targetAngle=0;
        }
        rotate(targetAngle+45);
    }else if(vlx[vlxID::frontLeft].isWall()){
        if(targetAngle==0){
        targetAngle=360;
        }
        rotate(targetAngle-45);
    }
    int backDistanceCm=7;
    float distanceTics=backDistanceCm*kTicsPerRev/distancePerRev;
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
    float linearDistanceTics=distanceTics*cos((45*PI/180));
    return linearDistanceTics;
}
float motors::limitCrash(){
    float targetAngle_=targetAngle;
    bool leftState=limitSwitch_[LimitSwitchID::kLeft].getState();
    bool rightState=limitSwitch_[LimitSwitchID::kRight].getState();
    if((leftState && rightState) || (!leftState && !rightState)){
        return 0;
    }
    if(rightState){
        if(targetAngle=360){
        targetAngle=0;
        }
        rotate(targetAngle+45);
    }else if(leftState){
        if(targetAngle=0){
        targetAngle=360;
        }
        rotate(targetAngle-45);
    }
    int backDistanceCm=13;
    float distanceTics=backDistanceCm*kTicsPerTile/kTileLength;
    wait(delayTime);
    resetTics();
    while(getAvergeTics()<distanceTics){
        setback();
        PID_AllWheels(kMinSpeedFormard);
    }
    stop();
    wait(delayTime);
    targetAngle=targetAngle_;
    rotate(targetAngle);
    float linearDistanceTics=distanceTics*cos((45*PI/180));
    return linearDistanceTics;
}

uint8_t motors::findNearest(float number,const uint8_t numbers[],uint8_t size,bool frontVlx){
    uint8_t nearest=numbers[0];
    float minDifference=abs(number-numbers[0]);
    for(uint8_t i=1;i<size;i++){
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
    targetAngle=deltaAngle;
    delayMicroseconds(1);
    bno.getOrientationX();
    float currentAngle,rightAngularDistance, leftAngularDistance,minInterval,maxInterval,tolerance=2;
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
        targetAngle=0;
        minInterval=-tolerance,maxInterval=tolerance;
        hexadecimal=false;
    }
    //decide shortest route and rotate("?" es un operador ternario para remplazar if-else)
    (rightAngularDistance<=leftAngularDistance) ? setright():setleft();
    currentAngle=hexadecimal ? angle:z_rotation;
    while (currentAngle<minInterval||currentAngle>maxInterval){
        changeSpeedMove(false,true,0,false);
        bno.getOrientationX();
        currentAngle= hexadecimal ? angle:z_rotation;
        Serial.println(angle);
    }
    stop();wait(50);
}
float motors::changeSpeedMove(bool encoders,bool rotate,int targetDistance,bool frontVlx){
    float speed;
    float missingDistance,missingAngle;
    if(rotate==true){
        missingAngle=abs(targetAngle-(targetAngle==0 ? z_rotation:angle));
        speed=map(missingAngle,90,0,kMaxSpeedRotate,kMinSpeedRotate);
        speed=constrain(speed,kMinSpeedRotate,kMaxSpeedRotate);
        PID_AllWheels(speed);
        return 0;
    }else{
        if(encoders==true){
            speed=map(getAvergeTics(),0,kTicsPerTile,kMaxPwmFormard,kMinPwmFormard);
            missingDistance=kTileLength-(getAvergeTics()*kTileLength/kTicsPerTile);
            return speed;
        }else{
            missingDistance=abs((frontVlx ? vlx[vlxID::frontLeft].getDistance():vlx[vlxID::back].getDistance())-targetDistance);//intercambiar vlx
            speed=frontVlx ? map(missingDistance,kTileLength,0,kMaxPwmFormard,kMinPwmFormard):map(missingDistance,kTileLength,0,kMaxPwmFormard,kMinPwmFormard);
            speed=constrain(speed,kMinPwmFormard,kMaxPwmFormard);
            return speed;
        }
    }
    // changePwm(rotate,(rotate ? missingAngle:missingDistance));
}
void motors::changePwm(bool rotate,double missing){
    uint8_t tolerance=10;
    int kp_=10;
    float kMaxSpeed, kMinSpeed;
    uint16_t *kMinPwm, *kMaxPwm;
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
void motors::setSpeed(uint16_t speed){
    for(uint8_t i=0;i<4;i++){ 
        motor[i].setSpeed(speed);}
}
void motors::setahead(){
    for(int i=0;i<4;i++){ 
        motor[i].ahead();}
    inMotion=true;
}
void motors::setback(){
    for(int i=0;i<4;i++){ 
        motor[i].back();}
    inMotion=true;
}
void motors::setright(){
    motor[MotorID::kBackLeft].ahead();
    motor[MotorID::kBackRight].back();
    motor[MotorID::kFrontLeft].ahead();
    motor[MotorID::kFrontRight].back();
    inMotion=true;
}
void motors::setleft(){
    motor[MotorID::kBackLeft].back();
    motor[MotorID::kBackRight].ahead();
    motor[MotorID::kFrontLeft].back();
    motor[MotorID::kFrontRight].ahead();
    inMotion=true;
}
void motors::stop(){
    for(uint8_t i=0;i<4;i++){ 
        motor[i].stop();}
    setSpeed(0);
    inMotion=false;
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
    for(uint8_t i=0;i<4;i++){ 
        motor[i].resetTics();}
}
double motors::getAvergeTics(){
    float totalTics=0;
    for(int i=0;i<4;i++){
        totalTics+=motor[i].tics;
    }
    return totalTics/4;
}
double motors::getTicsSpeed(){
    float ticsSpeed;
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
    uint8_t relativeDir;
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
    uint8_t realPos=rulet[relativeDir][direction];
    switch(realPos) {
        bool wall1,wall2,wall3,wall4;
        case 0:
            wall1=vlx[vlxID::frontLeft].isWall() /*&& vlx[vlxID::frontRight].isWall()*/;
            wifiPrint("wallFRONT",wall1);
            return wall1;
        case 1:
            wall2=vlx[vlxID::right].isWall();
            wifiPrint("wallRIGHT",vlx[vlxID::right].distance   );
            return wall2;
        case 2:
            wall3=vlx[vlxID::back].isWall();
            wifiPrint("wallBACK",wall3);
            return wall3;
        case 3:
            wall4=vlx[vlxID::left].isWall();
            wifiPrint("wallLEFT",wall4);
            return wall4;
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
    float currentOrientationY = bno.getOrientationY();
    if (currentOrientationY >= kMinRampOrientation || currentOrientationY <= -kMinRampOrientation) {
        // screenPrint("Ramp detected");
        ramp();
        if (currentOrientationY <= -kMinRampOrientation) {
            rampState = 2;
            return true;
        }else if (currentOrientationY > kMinRampOrientation) {
            rampState = 1;
            return true;
        }
    }
    return false;
}
void motors::ramp(){
    PID rampPID;
    setahead();
    rampPID.changeConstants(1,0.1,0.01,20);
    while(bno.getOrientationY()>kMinAngleRamp){
        float error=rampPID.calculate_PID(0,(vlx[vlxID::right].getDistance()-vlx[vlxID::left].getDistance()));
        wifiPrint("error",error);
        error=constrain(error,-15,15);
        PID_Wheel(kSpeedRampUp+error,MotorID::kFrontLeft);
        PID_Wheel(kSpeedRampUp+error,MotorID::kBackLeft);
        PID_Wheel(kSpeedRampUp-error,MotorID::kFrontRight);
        PID_Wheel(kSpeedRampUp-error,MotorID::kBackRight);
    }
    while(bno.getOrientationY() < -kMinAngleRamp){
        pidEncoders(kSpeedRampDown,true);
    }
    moveDistance(kTileLength/3);
    stop();
}
void motors::moveDistance(uint8_t targetDistance){
    setahead();
    resetTics();
    while(getCurrentDistanceCm()<targetDistance){
        pidEncoders((kMinSpeedFormard+kMaxSpeedFormard)/2,true);
    }
    stop();
}
float motors::getCurrentDistanceCm(){
    return getAvergeTics()*kTileLength/kTicsPerTile;
}
uint16_t motors::getAngleOrientation(){
    float currentAngle=bno.getOrientationX();
    if((currentAngle>315&&currentAngle<=360) || (currentAngle>=0&&currentAngle<=45)) return 0;
    else if(currentAngle>45&&currentAngle<=135) return 90;
    else if(currentAngle>135&&currentAngle<=225) return 180;
    else if(currentAngle>225&&currentAngle<=315) return 270;
}
Advanced motors::checkpointElection(){
    float angleOrientation=getAngleOrientation();
    targetAngle=angleOrientation;
    uint8_t angleThreshold=7;
    float currentAngle = (angleOrientation == 0) ? z_rotation : angle;
    Advanced advanced;
    int turn;
    if((currentAngle-angleOrientation) < -angleThreshold){
        turn=-1; 
        ahead();
        left();
        ahead();
    } 
    else if((currentAngle-angleOrientation)<angleThreshold){
        turn=1; 
        ahead();
        right();
        ahead(); 
    } 
    else turn=0;
    if(angleOrientation==0 && turn==-1) advanced={-1,1};
    else if(angleOrientation==0 && turn==1) advanced={1,1};
    else if(angleOrientation==90 && turn==-1) advanced={1,1};
    else if(angleOrientation==90 && turn==1) advanced={1,-1};
    else if(angleOrientation==180 && turn==-1) advanced={1,-1};
    else if(angleOrientation==180 && turn==1) advanced={-1,-1};
    else if(angleOrientation==270 && turn==-1) advanced={-1,-1};
    else if(angleOrientation==270 && turn==1) advanced={-1,1};
    else advanced={0,0};
    return advanced;
}

void motors::setupTCS() {
    tcs_.setMux(Pins::tcsPins[0]);
    tcs_.setPrecision(kPrecision);
    tcs_.init(kColors, kColorAmount, kColorList, kColorThresholds);
}
void motors::wait(unsigned long targetTime){
    unsigned long initialTime=millis();
    while((millis()-initialTime)<targetTime){

    }
}
void motors::wifiPrint(String message, float i){
    client.print(message);
    client.println(i);
    // Serial.println("Enviado: ");
}


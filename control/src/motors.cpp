#include "motors.h"
#include "Pins_ID.h"
#include <WiFi.h>
#include "Jetson.h"
Jetson jettson;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
motors::motors(){
}
void motors::setupMotors(){
    for(uint8_t i=0;i<4;i++){
        motor[i].initialize(Pins::digitalOne[i],Pins::digitalTwo[i],Pins::pwmPin[i],i);
        Serial.println(Pins::pwmPin[i]);
        myPID[i].changeConstants(kP_mov,kI_mov,kD_mov,movTime);
    }
    rampUpPID.changeConstants(kP_RampUp,kI_RampUp,kD_RampUp,rampTime);
    rampDownPID.changeConstants(kP_RampDown,kI_RampDown,kD_RampDown,rampTime);
    Wire.begin();
    screenBegin();
    screenPrint("r");
    bno.setupBNO();
    setupVlx(vlxID::frontLeft);
    setupVlx(vlxID::left);
    setupVlx(vlxID::front);
    setupVlx(vlxID::frontRight);
    setupVlx(vlxID::right);
    setupVlx(vlxID::back);
    setupTCS();
    leds.setupLeds();
    limitSwitch_[LimitSwitchID::kLeft].initLimitSwitch(Pins::limitSwitchPins[LimitSwitchID::kLeft]);
    limitSwitch_[LimitSwitchID::kRight].initLimitSwitch(Pins::limitSwitchPins[LimitSwitchID::kRight]);
    pinMode(Pins::checkpointPin, INPUT_PULLDOWN);
    servo.attach(Pins::servoPin);
    servo.write(servoPos);
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
    Serial.println("reference_pwm");
    Serial.println(reference_pwm);
    int speedTics=motor[i].getTicsSpeed();
    Serial.println("speedTics");
    Serial.println(speedTics);
    float error=myPID[i].calculate_PID(speed_setpoint,speedTics);
    int speed=reference_pwm+error;
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
    if(rampState!=0) changeAngle=0;
    float error=pidBno.calculate_PID(targetAngle+changeAngle,(targetAngle==0 ? z_rotation:angle));
    error=constrain(error,-8,8);//aumentar
    // Serial.println(error);
    Serial.println(angle);
    if(!ahead) error=-error;
    PID_Wheel(speedReference+error,MotorID::kFrontLeft);
    PID_Wheel(speedReference+error,MotorID::kBackLeft);
    PID_Wheel(speedReference-error,MotorID::kFrontRight);
    PID_Wheel(speedReference-error,MotorID::kBackRight);
}
void motors::ahead(){
    passObstacle();
    nearWall();
    resetTics();
    int offset=0;;
    float distance;
    bool encoder,frontVlx;
    bool rampCaution=false;
    float frontDistance=vlx[vlxID::front].getDistance();
    float backDistance;
    if(frontDistance<maxVlxDistance && frontDistance>=1){
        distance=frontDistance;
        encoder=false;
        frontVlx=true;
    }else{ 
        backDistance=vlx[vlxID::back].getDistance();
        if((backDistance<maxVlxDistance-kTileLength)&& frontDistance>=1){
            distance=backDistance;
            encoder=false;
            frontVlx=false;
        }else encoder=true;
    }

    String print=static_cast<String>(frontDistance);
    robot.screenPrint(print);
    if(frontDistance>300) rampCaution=true;
    if(abs(bno.getOrientationY())>4 ){
        encoder=true;offset=kTicsPerTile/6;
    } 
    if(!encoder){
        float targetDistance=findNearest(distance,frontVlx ? targetDistances:targetDistancesB,2,frontVlx);
        targetDistance=targetDistance;
        while(frontVlx ? (distance>=targetDistance):(distance<=targetDistance)){//poner rango
            setahead();
            checkTileColor();
            if(blackTile) break;
            if(buttonPressed) break;
            if(isRamp()) break;
            limitCrash();
            distance=(frontVlx ? vlx[vlxID::front].getDistance():vlx[vlxID::back].getDistance());
            float missingDistance=abs(distance-targetDistance);
            float speed;
            speed=map(missingDistance,kTileLength,0,kMaxSpeedFormard,kMinSpeedFormard);
            if(slope==true){
                missingDistance=kTileLength-(getAvergeTics()*kTileLength/kTicsPerTile);
                speed=map(missingDistance,kTileLength,0,kMaxSpeedFormard,kMinSpeedFormard);
                if(getAvergeTics()>kTicsPerTile) break;
            }
            if(rampCaution) speed=map(missingDistance,kTileLength,0,(kMaxSpeedFormard/3),kMinSpeedFormard);
            speed=constrain(speed,kMinSpeedFormard,kMaxSpeedFormard);
            pidEncoders(speed,true);
        }
    }else if(encoder){
        while(getAvergeTics()<kTicsPerTile-offset){
            setahead();
            // limitCrash();
            checkTileColor();
            if(blackTile) return;
            if(buttonPressed) break;
            if(isRamp()) break;
            float missingDistance=kTileLength-(getAvergeTics()*kTileLength/kTicsPerTile);
            float speed=map(missingDistance,kTileLength,0,kMaxSpeedFormard,kMinSpeedFormard);
            speed=constrain(speed,kMinSpeedFormard,kMaxSpeedFormard);
            if(rampCaution) speed=map(missingDistance,kTileLength,0,(kMaxSpeedFormard/3),kMinSpeedFormard);
            pidEncoders(speed,true);
        }
    }
    slope=false;
    stop();resetTics();checkTileColor();resetTics();
   
}
void motors::checkTileColor(){
    tileColor=tcs_.getColor();
    Serial.println(tileColor);
    if(tileColor==kBlackColor && inMotion==true){
        inMotion=false;
        resetTics();
        setback();
        blackTile=true;
        while(getAvergeTics()<kTicsPerTile/2){
            setback();
            if(buttonPressed) break;
            int speed=map(getAvergeTics(),0,kTicsPerTile/2,kMaxSpeedFormard,kMinSpeedFormard);
            pidEncoders(speed,false);
            screenPrint("Black Tile");
        }
        stop();resetTics();
    }else if(tileColor==kBlueColor&&inMotion==false){
        screenPrint("Blue Tile");
        blueTile=true;
        wait(5500);
    }else if(tileColor==kCheckpointColor&&inMotion==false){
        checkpoint=true;
        // screenPrint("Checkpoint Tile");
    }
}

float motors::nearWall(){
    vlx[vlxID::left].getDistance();
    vlx[vlxID::right].getDistance();
    if(vlx[vlxID::left].distance<minDisToLateralWall ){
        changeAngle=maxChangeAngle;
    }else if(vlx[vlxID::right].distance<minDisToLateralWall){
        changeAngle=-maxChangeAngle;
    }else{
        changeAngle=0;
    }
    return changeAngle;
}
void motors::passObstacle(){
    float targetAngle_=targetAngle;
    float frontLeftDistance=vlx[vlxID::frontLeft].getDistance();
    float frontRightDistance=vlx[vlxID::frontRight].getDistance();
    if((frontLeftDistance>kDistanceToWall) && (frontRightDistance>kDistanceToWall) ){
        return;
    }
    moveDistance(kTileLength/6,true);
    if(frontRightDistance<kDistanceToWall){
        // limitColition=true;
        if(targetAngle==360){
            targetAngle=0;
        }
        rotate(targetAngle+25);
    }else if(frontLeftDistance<kDistanceToWall){
        // limitColition=true;
        if(targetAngle==0){
            targetAngle=360;
        }
        rotate(targetAngle-25);
    }
    delay(300);
    moveDistance(kTileLength/5,false);
    delay(300);
    targetAngle=targetAngle_;
    rotate(targetAngle);
    limitColition=false;
}
void motors::limitCrash(){
    float targetAngle_=targetAngle;
    bool leftState=limitSwitch_[LimitSwitchID::kLeft].getState();
    bool rightState=limitSwitch_[LimitSwitchID::kRight].getState();
    if(slope) return;
    if(rampState!=0 ){
        if(leftState || rightState) limitColition=true;
        return;
    } 
    if((leftState && rightState) || (!leftState && !rightState)){
        return;
    }
    else if(rightState){
        screenPrint("RightLimit");
        Serial.println("rightlimit");
        if(targetAngle==360){
            targetAngle=0;
        }
        rotate(targetAngle+25);
    }else if(leftState){
        screenPrint("leftLimit");
        Serial.println("rightlimit");
        if(targetAngle==0){
            targetAngle=360;
        }
        rotate(targetAngle-25);
    }
    delay(300);
    moveDistance(kTileLength/6,false);
    delay(300);
    targetAngle=targetAngle_;
    rotate(targetAngle);
    limitColition=false;
}

uint8_t motors::findNearest(float number,const uint8_t numbers[],uint8_t size,bool frontVlx){
    if(frontVlx) number-=kTileLength;
    else if(!frontVlx) number+=kTileLength;
    uint8_t nearest=numbers[0];
    float minDifference=abs(number-numbers[0]);
    for(uint8_t i=1;i<size;i++){
        float currentDifference=abs(number-numbers[i]);
        if(currentDifference<minDifference){
            nearest=numbers[i];
            minDifference=currentDifference;
        }
    }
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
    String print="Turn "+ static_cast<String>(deltaAngle);
    // screenPrint(print);
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
    //decide shortest route and rotate
    (rightAngularDistance<=leftAngularDistance) ? (limitColition ? setrightTraslation():setright()):(limitColition ? setleftTraslation():setleft());

    currentAngle=hexadecimal ? angle:z_rotation;
    while (currentAngle<minInterval||currentAngle>maxInterval){
        changeSpeedMove(false,true,0,false);
        bno.getOrientationX();
        currentAngle= hexadecimal ? angle:z_rotation;
        if(buttonPressed) break;
        // Serial.println(angle);
    }
    stop();
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
    // inMotion=true;
}
void motors::setright(){
    motor[MotorID::kBackLeft].ahead();
    motor[MotorID::kBackRight].back();
    motor[MotorID::kFrontLeft].ahead();
    motor[MotorID::kFrontRight].back();
    // inMotion=true;
}
void motors::setleft(){
    motor[MotorID::kBackLeft].back();
    motor[MotorID::kBackRight].ahead();
    motor[MotorID::kFrontLeft].back();
    motor[MotorID::kFrontRight].ahead();
    // inMotion=true;
}
void motors::setrightTraslation(){
    motor[MotorID::kBackLeft].stop();
    motor[MotorID::kBackRight].back();
    motor[MotorID::kFrontLeft].stop();
    motor[MotorID::kFrontRight].back();
    // inMotion=true;
}
void motors::setleftTraslation(){
    motor[MotorID::kBackLeft].back();
    motor[MotorID::kBackRight].stop();
    motor[MotorID::kFrontLeft].back();
    motor[MotorID::kFrontRight].stop();
    // inMotion=true;
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
void motors::resetVlx() {
    screenBegin();
    setupVlx(vlxID::frontLeft);
    setupVlx(vlxID::left);
    setupVlx(vlxID::front);
    setupVlx(vlxID::frontRight);
    setupVlx(vlxID::right);
    setupVlx(vlxID::back);
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
            wall1=vlx[vlxID::front].isWall();
            return wall1;
        case 1:
            wall2=vlx[vlxID::right].isWall();
            return wall2;
        case 2:
            wall3=vlx[vlxID::back].isWall();
            return wall3;
        case 3:
            wall4=vlx[vlxID::left].isWall();
            return wall4;
        default: 
          return false;
    }
}

bool motors::rampInFront(){
    if((vlx[vlxID::frontLeft].getDistance()-vlx[vlxID::front].getDistance())>=2){
        return true;
    }else{
        return false;
    }
}bool motors::isRamp() {
    float currentOrientationY = bno.getOrientationY();
    // screenPrint("isRamp");
    if(abs(currentOrientationY)>7) slope=true;
    if (currentOrientationY >= kMinRampOrientation || currentOrientationY <= -kMinRampOrientation) {
        
        if (currentOrientationY <= -kMinRampOrientation) {
            // screenPrint("Ramp detected");
            ramp();
            return true;
        }else if (currentOrientationY > kMinRampOrientation) {
            // screenPrint("Ramp detected");
            ramp();
            return true;
        }
        
    }
    return false;
}
void motors::ramp(){
    resetTics();
    setahead();
    while(bno.getOrientationY()>7){
        if(buttonPressed==true)break;
        // limitCrash();
        if(limitColition==true){
            stop();break;
        }
        float error;
        vlx[vlxID::right].getDistance();
        vlx[vlxID::left].getDistance();
        if((vlx[vlxID::right].distance<vlx[vlxID::right].kDistanceToWall && vlx[vlxID::left].distance<vlx[vlxID::left].kDistanceToWall) &&
        (vlx[vlxID::right].distance<6 || vlx[vlxID::left].distance<6)){
            error=rampUpPID.calculate_PID(0,(vlx[vlxID::right].distance-vlx[vlxID::left].distance));
            error=constrain(error,-15,15);
            PID_Wheel(kSpeedRampUp-error,MotorID::kFrontLeft);
            PID_Wheel(kSpeedRampUp-error,MotorID::kBackLeft);
            PID_Wheel(kSpeedRampUp+error,MotorID::kFrontRight);
            PID_Wheel(kSpeedRampUp+error,MotorID::kBackRight);
        }else{
            pidEncoders(kSpeedRampUp,true);
        }
        rampState = 1; 
        screenPrint("rampUp");
    }
    while(bno.getOrientationY() < -7){
        if(buttonPressed==true)break;
        // limitCrash();
        if(limitColition==true) break;
        float error;
        vlx[vlxID::right].getDistance();
        vlx[vlxID::left].getDistance();
        if((vlx[vlxID::right].distance<vlx[vlxID::right].kDistanceToWall && vlx[vlxID::left].distance<vlx[vlxID::left].kDistanceToWall) &&
        (vlx[vlxID::right].distance<6 || vlx[vlxID::left].distance<6)){
            error=rampDownPID.calculate_PID(0,(vlx[vlxID::right].distance-vlx[vlxID::left].distance));
            error=constrain(error,-6,6);
            PID_Wheel(kSpeedRampDown-error,MotorID::kFrontLeft);
            PID_Wheel(kSpeedRampDown-error,MotorID::kBackLeft);
            PID_Wheel(kSpeedRampDown+error,MotorID::kFrontRight);
            PID_Wheel(kSpeedRampDown+error,MotorID::kBackRight);
        }else{
            pidEncoders(kSpeedRampDown,true);
        }
        rampState = 2;
        screenPrint("rampDown");
    }
    if(getAvergeTics()>1*kTicsPerTile && rampState==1){
        // stop();
        // bno.resetOrientationX();
        // setahead();
        moveDistance(kTileLength/3,true);
        rotate(targetAngle);
    }else if(getAvergeTics()>0.8*kTicsPerTile && rampState==2){
        // stop();
        // bno.resetOrientationX();
        // setahead();
        moveDistance(kTileLength/2,true);
        rotate(targetAngle);
    }else{
        rampState=0;
    }
    limitColition=false;
    resetTics();stop();wait(100);
}
void motors::moveDistance(uint8_t targetDistance,bool ahead){
    screenPrint("leaving");
    ahead ? setahead():setback();
    resetTics();
    while(getCurrentDistanceCm()<targetDistance){
        if(buttonPressed) break;
        pidEncoders((kMinSpeedFormard+kMaxSpeedFormard)/2,ahead);
    }
    stop(); 
}
float motors::getCurrentDistanceCm(){
    return getAvergeTics()*kTileLength/kTicsPerTile;
}
float motors::getAngleOrientation(){
    float currentAngle=bno.getOrientationX();
    if((currentAngle>315&&currentAngle<=360) || (currentAngle>=0&&currentAngle<=45)) return 0;
    else if(currentAngle>45&&currentAngle<=135) return 90;
    else if(currentAngle>135&&currentAngle<=225) return 180;
    else if(currentAngle>225&&currentAngle<=315) return 270;

}
void motors::resetOrientation(){
    bno.resetOrientation();
    targetAngle=0;
}
void motors::checkpointElection(){
    float angleOrientation=getAngleOrientation();
    Serial.println("angule");
    String print;
    print=static_cast<String>(angleOrientation);
    screenPrint(print);
    Serial.println(angleOrientation);
    uint8_t angleThreshold=10;
    float currentAngle = (angleOrientation == 0) ? z_rotation : angle;
    if(abs(currentAngle-angleOrientation) < angleThreshold){
        return;
    } 
    rotate(angleOrientation);
    int turn;
    if((currentAngle-angleOrientation) < -angleThreshold){
        turn=-1; 
        ahead();
        left();
        ahead();
    } 
    else if((currentAngle-angleOrientation)>angleThreshold){
        turn=1; 
        ahead();
        right();
        ahead(); 
    } 
    return;
}
void motors::victimSequency(){
    float current=millis();
    while((millis()-current)<5100){
        screenPrint("Victim");
        leds.setBlue();
        delay(500);
        screenPrint(" ");
        leds.turnOff();
        delay(500);
    }
    leds.setWhite(); 
}
void motors::harmedVictim(){
    victimSequency();
    if(kitState==kitID::kRight){
        // screenPrint("Right");
        kitRight(2);     } 
    else if(kitState==kitID::kLeft){
        // screenPrint("Left");
        kitLeft(2); 
    } 
    // screenPrint("");

}
void motors::stableVictim(){
    // screenPrint("Stable");
    victimSequency();
    if(kitState==kitID::kRight){
        kitRight(1); 
    } 
    else if(kitState==kitID::kLeft){
        kitLeft(1); 
    } 
    // screenPrint("");
}
void motors::unharmedVictim(){
    // screenPrint("Unharmed");
    victimSequency();
    // screenPrint("");
}
void motors::kitLeft(uint8_t n){
    uint16_t dt=0;
    for(uint8_t i=0;i<n;i++){ 
        writeServo(servoPosRight);
        writeServo(servoPosLeft);
        writeServo(10);
        writeServo(servoPosLeft);
        writeServo(90);
    }
}
void motors::writeServo(uint16_t servoAngle){
    servo.write(servoAngle);
    delay(200);
    if((servoAngle!=10 && servoAngle!=90) && servoAngle!=170){
        for(uint8_t i;i<4;i++){
        servo.write(servoAngle-6);
        delay(40);
        servo.write(servoAngle+6);
        delay(40);
        }
    }  
}
void motors::kitRight(uint8_t n){
    uint16_t dt=600;
    for(uint8_t i=0;i<n;i++){ 
        writeServo(servoPosLeft);
        writeServo(servoPosRight);
        writeServo(170);
        writeServo(servoPosRight);
        writeServo(90);
    }
}
void motors::reloadKits(){
    writeServo(servoPosLeft);
    writeServo(servoPosRight);
    writeServo(90);
}

void motors::setupTCS() {
    tcs_.setMux(Pins::tcsPins[0]);
    tcs_.setPrecision(kPrecision);
    tcs_.init(kColors, kColorAmount, kColorList, kColorThresholds);
    bno.setPhaseCorrection(bno.getOrientationX());
    bno.setPhaseCorrectionY(bno.getOrientationY());
}
void motors::wait(unsigned long targetTime){
    unsigned long initialTime=millis();
    while((millis()-initialTime)<targetTime){
        if(buttonPressed) break;
    }
}
void motors::wifiPrint(String message, float i){
    // client.print(message);
    // client.println(i);
    // Serial.println("Enviado: ");
}
void motors::screenBegin(){
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    Serial.println("Pantalla inicializada");
}
void motors::screenPrint(String output){
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println(output);
    display.display();
}
void motors::calibrateColors(){
    uint16_t dt=5000;
    screenPrint("Blue Tile");
    delay(2000);
    tcs_.updateRGBC();
    tcs_.printRGB();
    float redInBlue=tcs_.red_;
    float greenInBlue=tcs_.green_;
    float blueInBlue=tcs_.blue_;

    screenPrint("Black Tile");
    delay(dt);
    tcs_.updateRGBC();
    tcs_.printRGB();
    float redInBlack=tcs_.red_;
    float greenInBlack=tcs_.green_;
    float blueInBlack=tcs_.blue_;

    screenPrint("Checkpoint Tile");
    delay(dt);
    tcs_.updateRGBC();
    tcs_.printRGB();
    float redInCheck=tcs_.red_;
    float greenInCheck=tcs_.green_;
    float blueInCheck=tcs_.blue_;

    screenPrint("white Tile");
    delay(dt);
    tcs_.updateRGBC();
    tcs_.printRGB();
    float redInWhite=tcs_.red_;
    float greenInWhite=tcs_.green_;
    float blueInWhite=tcs_.blue_;

    while(true){
        if(buttonPressed) break;
        String print;
        dt=2000;
        screenPrint("RGB Blue");
        delay(800);
        print=static_cast<String>(redInBlue)+","+static_cast<String>(greenInBlue)+","+static_cast<String>(blueInBlue);
        screenPrint(print);
        delay(dt);

        screenPrint("RGB Black");
        delay(800);
        print=static_cast<String>(redInBlack)+","+static_cast<String>(greenInBlack)+","+static_cast<String>(blueInBlack);
        screenPrint(print);
        delay(dt);

        screenPrint("RGB Check");
        delay(800);
        print=static_cast<String>(redInCheck)+","+static_cast<String>(greenInCheck)+","+static_cast<String>(blueInCheck);
        screenPrint(print);
        delay(dt);

        screenPrint("RGB white");
        delay(800);
        print=static_cast<String>(redInWhite)+","+static_cast<String>(greenInWhite)+","+static_cast<String>(blueInWhite);
        screenPrint(print);
        delay(dt);
    }
}

#include "motors.h"
#include "Pins_ID.h"
#include <WiFi.h>
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
motors::motors(){
}
void motors::setupMotors(){
    for(uint8_t i=0;i<4;i++){
        motor[i].initialize(Pins::digitalOne[i],Pins::digitalTwo[i],Pins::pwmPin[i],i);
        Serial.println(Pins::pwmPin[i]);
        myPID[i].changeConstants(5.5,0.1,0.005,50);
    }
    rampUpPID.changeConstants(kP_RampUp,kI_RampUp,kD_RampUp,rampTime);
    rampDownPID.changeConstants(kP_RampDown,kI_RampDown,kD_RampDown,rampTime);
    Wire.begin();
    screenBegin();
    screenPrint("vlx");
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
    servo.write(0);
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
    // float changeAngle=nearWall();
    float error=pidBno.calculate_PID(targetAngle/*+changeAngle*/,(targetAngle==0 ? z_rotation:angle));
    error=constrain(error,-8,8);//aumentar
    Serial.println(error);
    if(!ahead) error=-error;
    PID_Wheel(speedReference+error,MotorID::kFrontLeft);
    PID_Wheel(speedReference+error,MotorID::kBackLeft);
    PID_Wheel(speedReference-error,MotorID::kFrontRight);
    PID_Wheel(speedReference-error,MotorID::kBackRight);
}
void motors::ahead(){
    // float obstacleDistance=passObstacle();
    // screenPrint("Ahead");
    resetTics();
    int edgeVlx;
    float distance;
    float frontLeftDistance=vlx[vlxID::frontLeft].getDistance();
    float frontRightDistance=vlx[vlxID::frontRight].getDistance();
    float frontDistance=(frontLeftDistance>frontRightDistance) ? frontLeftDistance:frontRightDistance;
    float backDistance=vlx[vlxID::back].getDistance();
    bool encoder,frontVlx;
    bool rampCaution=false;
    if(frontDistance<maxVlxDistance){
        distance=frontDistance;
        encoder=false;
        frontVlx=true;
        // edgeVlx=5;
        // screenPrint("FRONT");
    }else if(backDistance<maxVlxDistance-kTileLength){
        distance=backDistance;
        encoder=false;
        frontVlx=false;
        // edgeVlx=5;
        if(frontDistance>300) rampCaution=true;
        // screenPrint("BACK");
    }else{
        encoder=true;
        // screenPrint("ENCODER");
    }
    if(!encoder){
        float targetDistance=findNearest(distance,targetDistances,4,frontVlx);
        targetDistance=targetDistance+edgeVlx;
        while(frontVlx ? (distance>targetDistance):(distance<targetDistance)){//poner rango
            // String print=static_cast<String>(targetDistance);
            // screenPrint(print);
            setahead();
            limitCrash();
            checkTileColor();
            if(blackTile) break;
            if(buttonPressed) break;
            distance=(frontVlx ? vlx[vlxID::front].getDistance():vlx[vlxID::back].getDistance());
            Serial.println(distance);
            float missingDistance=abs(distance-targetDistance);
            float speed;
            speed=map(missingDistance,kTileLength,0,kMaxSpeedFormard,kMinSpeedFormard);
            speed=constrain(speed,kMinSpeedFormard,kMaxSpeedFormard);
            Serial.println(rampCaution);
            if(rampCaution){
                speed=map(missingDistance,kTileLength,0,(kMaxSpeedFormard/2),kMinSpeedFormard);
            }
            pidEncoders(speed,true);
            if(isRamp()) break;
        }
    }else if(encoder){
        Serial.println(getAvergeTics());
        while(getAvergeTics()<kTicsPerTile/*+obstacleDistance*/){
            setahead();
            limitCrash();
            checkTileColor();
            if(blackTile) return;
            if(buttonPressed) break;
            bno.getOrientationX();
            float missingDistance=kTileLength-(getAvergeTics()*kTileLength/kTicsPerTile);
            float speed=map(missingDistance,kTileLength,0,kMaxSpeedFormard,kMinSpeedFormard);
            speed=constrain(speed,kMinSpeedFormard,kMaxSpeedFormard);
            pidEncoders(speed,true);
            if(isRamp()) break;
        }
    }
    resetTics();
    stop();resetTics();checkTileColor();
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
            screenPrint("Black Tile");
        }
        stop();resetTics();
    }else if(tileColor==kBlueColor&&inMotion==false){
        screenPrint("Blue Tile");
        blueTile=true;
        wait(5500);
    }else if(tileColor==kCheckpointColor&&inMotion==false){
        checkpoint=true;
        screenPrint("Checkpoint Tile");
    }
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
        targetAngle=0;
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
        limitColition=true;
        if(targetAngle==360){
            targetAngle=0;
        }
        rotate(targetAngle+45);
    }else if(leftState){
        limitColition=true;
        if(targetAngle==0){
            targetAngle=360;
        }
        rotate(targetAngle-45);
    }
    int backDistanceCm=13;
    float distanceTics=backDistanceCm*kTicsPerTile/kTileLength;
    // wait(delayTime);
    // resetTics();
    // while(getAvergeTics()<distanceTics){
    //     setback();
    //     PID_AllWheels(kMinSpeedFormard);
    // }
    // stop();
    // wait(delayTime);
    targetAngle=targetAngle_;
    rotate(targetAngle);
    float linearDistanceTics=distanceTics*cos((45*PI/180));
    limitColition=false;
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
        Serial.println(angle);
    }
    stop();wait(200);
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
void motors::setrightTraslation(){
    motor[MotorID::kBackLeft].stop();
    motor[MotorID::kBackRight].back();
    motor[MotorID::kFrontLeft].stop();
    motor[MotorID::kFrontRight].back();
    inMotion=true;
}
void motors::setleftTraslation(){
    motor[MotorID::kBackLeft].back();
    motor[MotorID::kBackRight].stop();
    motor[MotorID::kFrontLeft].back();
    motor[MotorID::kFrontRight].stop();
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
    setahead();
    while(bno.getOrientationY()>kMinAngleRamp){
        float error;
        vlx[vlxID::right].getDistance();
        vlx[vlxID::left].getDistance();
        if(vlx[vlxID::right].distance<vlx[vlxID::right].kDistanceToWall && vlx[vlxID::left].distance<vlx[vlxID::left].kDistanceToWall){
            error=rampUpPID.calculate_PID(0,(vlx[vlxID::right].distance-vlx[vlxID::left].distance));
            error=constrain(error,-15,15);
            PID_Wheel(kSpeedRampUp-error,MotorID::kFrontLeft);
            PID_Wheel(kSpeedRampUp-error,MotorID::kBackLeft);
            PID_Wheel(kSpeedRampUp+error,MotorID::kFrontRight);
            PID_Wheel(kSpeedRampUp+error,MotorID::kBackRight);
        }else{
            pidEncoders(kSpeedRampUp,true);
        }
        
    }
    while(bno.getOrientationY() < -kMinAngleRamp){
        float error;
        vlx[vlxID::right].getDistance();
        vlx[vlxID::left].getDistance();
        if(vlx[vlxID::right].distance<vlx[vlxID::right].kDistanceToWall || vlx[vlxID::left].distance<vlx[vlxID::left].kDistanceToWall){
            error=rampDownPID.calculate_PID(0,(vlx[vlxID::right].distance-vlx[vlxID::left].distance));
            error=constrain(error,-6,6);
            PID_Wheel(kSpeedRampDown-error,MotorID::kFrontLeft);
            PID_Wheel(kSpeedRampDown-error,MotorID::kBackLeft);
            PID_Wheel(kSpeedRampDown+error,MotorID::kFrontRight);
            PID_Wheel(kSpeedRampDown+error,MotorID::kBackRight);
        }else{
            pidEncoders(kSpeedRampUp,true);
        }
        
    }
    moveDistance(kTileLength/2);
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
float motors::getAngleOrientation(){
    float currentAngle=bno.getOrientationX();
    if((currentAngle>315&&currentAngle<=360) || (currentAngle>=0&&currentAngle<=45)) return 0;
    else if(currentAngle>45&&currentAngle<=135) return 90;
    else if(currentAngle>135&&currentAngle<=225) return 180;
    else if(currentAngle>225&&currentAngle<=315) return 270;

}
void motors::resetOrientation(){
    bno.resetOrientation();
}
Advanced motors::checkpointElection(){
    float angleOrientation=getAngleOrientation();
    Serial.println("angule");
    Serial.println(angleOrientation);
    uint8_t angleThreshold=10;
    float currentAngle = (angleOrientation == 0) ? z_rotation : angle;
    Advanced advanced;
    rotate(angleOrientation);
    
    int turn;
    if((currentAngle-angleOrientation) < -angleThreshold){
        turn=-1; 
        ahead();
        bno.resetOrientation();
        left();
        ahead();
    } 
    else if((currentAngle-angleOrientation)<angleThreshold){
        turn=1; 
        ahead();
        bno.resetOrientation();
        right();
        ahead(); 
    } 
    // else turn=0;
    // if(angleOrientation==0 && turn==-1) advanced={-1,1};
    // else if(angleOrientation==0 && turn==1) advanced={1,1};
    // else if(angleOrientation==90 && turn==-1) advanced={1,1};
    // else if(angleOrientation==90 && turn==1) advanced={1,-1};
    // else if(angleOrientation==180 && turn==-1) advanced={1,-1};
    // else if(angleOrientation==180 && turn==1) advanced={-1,-1};
    // else if(angleOrientation==270 && turn==-1) advanced={-1,-1};
    // else if(angleOrientation==270 && turn==1) advanced={-1,1};
    advanced={0,0};
    return advanced;
}
void motors::harmedVictim(){
    float current=millis();
    Serial.print("harmed");
    while((millis()-current)<5500){
        screenPrint("Harmed");
        digitalWrite(2,1);
        delay(500);
        screenPrint(".");
        digitalWrite(2,0);
    }
    for(uint8_t i=0;i<2;i++){
        servoPos+=27.692;
        servoPos=constrain(servoPos,0,180);
        servo.write(servoPos);
        wait(300);
    }   
}
void motors::stableVictim(){
    float current=millis();
    Serial.print("stable");
    while((millis()-current)<5500){
        screenPrint("Stable");
        delay(500);
        screenPrint(".");
        // digitalWrite(2,1);
    }
    servoPos+=27.692;
    servoPos=constrain(servoPos,0,180);
    servo.write(servoPos);
    wait(300);
}
void motors::unharmedVictim(){
    float current=millis();
    Serial.print("unharmed");
    while((millis()-current)<5500){
        screenPrint("Unharmed");
        delay(500);
        screenPrint(".");
    }
    

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

    }
}
void motors::wifiPrint(String message, float i){
    // client.print(message);
    // client.println(i);
    // Serial.println("Enviado: ");
}
void motors::screenBegin(){
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}
void motors::screenPrint(String output){
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println(output);
    display.display();
    #if USING_SCREEN
    // delay(1500);
    #endif
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

    while(true){
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
    }
}



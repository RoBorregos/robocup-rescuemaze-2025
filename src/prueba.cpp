// //actual
// void motors::ahead(){
//     double obstacleDistance=passObstacle();
//     resetTics();
//     double distance=vlx[vlxID::frontRight].getDistance();
//     setahead();
//     if(distance<maxVlxDistance){
//         int targetDistances[]={edgeTileDistance,30+edgeTileDistance,60+edgeTileDistance,90+edgeTileDistance};
//         int targetDistance=findNearest(distance,targetDistances,4);//agregar variable de adelante atras
//         while(distance>targetDistance){//poner rango
//             // float changeAngle=nearWall();
//             distance=vlx[vlxID::frontRight].getDistance();
//             Serial.println(distance);
//             bno.getOrientationX();
//             float speed=changeSpeedMove(false,false,targetDistance);
//             PID_speed(targetAngle/*+changeAngle*/,(targetAngle==0 ? z_rotation:angle),speed);
//             // showSpeeds();
//         }
//     }else{
//         while(getAvergeTics()<kTicsPerTile+obstacleDistance){
//             bno.getOrientationX();
//             // float changeAngle=nearWall();
//             float speed=changeSpeedMove(0.5,false,kTicsPerTile);
//             PID_speed((targetAngle/*+changeAngle*/),(targetAngle==0 ? z_rotation:angle),speed);
//             printSpeeds();
//         }
//     }
//     stop();resetTics();
// }


// //ideal
// void motors::ahead(){
//     double obstacleDistance=passObstacle();
//     resetTics();
//     double distance=vlx[vlxID::frontRight].getDistance();
//     double frontDistance=vlx[vlxID::frontRight].getDistance();
//     double backDistance=vlx[vlxID::back].getDistance();
//     bool encoder,frontVlx;
//     if(frontDistance<maxVlxDistance){
//         distance=frontDistance;
//         encoder=false;
//         frontVlx=true;
//     }else if(backDistance<maxVlxDistance){
//         distance=backDistance;
//         encoder=false;
//         frontVlx=false;
//     }else{
//         encoder=false;
//     }
//     setahead();
//     if(!encoder){
//         int targetDistances[]={edgeTileDistance,30+edgeTileDistance,60+edgeTileDistance,90+edgeTileDistance};
//         int targetDistance=findNearest(distance,targetDistances,4,frontVlx);//agregar variable de adelante atras
//         while(abs(distance-targetDistance)>1){//poner rango
//             // float changeAngle=nearWall();
//             distance=(frontVlx ? vlx[vlxID::frontRight].getDistance():vlx[vlxID::back].getDistance());
//             Serial.println(distance);
//             bno.getOrientationX();
//             float speed=changeSpeedMove(false,false,targetDistance,frontVlx);
//             PID_speed(targetAngle/*+changeAngle*/,(targetAngle==0 ? z_rotation:angle),speed);
//             // showSpeeds();
//         }
//     }else{
//         while(getAvergeTics()<kTicsPerTile+obstacleDistance){
//             bno.getOrientationX();
//             // float changeAngle=nearWall();
//             float speed=changeSpeedMove(true,false,kTicsPerTile,frontVlx);
//             PID_speed((targetAngle/*+changeAngle*/),(targetAngle==0 ? z_rotation:angle),speed);
//             printSpeeds();
//         }
//     }
//     stop();resetTics();
// }


// #include <WiFi.h>
// WiFiServer server(1234);
// WiFiClient client;
// const char* ssid = "Tec";
// const char* identity = "A00841856";  // Usuario
// const char* password = "";        // Contraseña

// void setup() {
//     Serial.begin(115200);
    
//     WiFi.disconnect(true);
//     delay(1000);

//     WiFi.begin(ssid, WPA2_AUTH_PEAP, identity, password);

//     Serial.print("Conectando a WiFi");
//     while (WiFi.status() != WL_CONNECTED) {
//         Serial.print(".");
//         delay(1000);
//     }

//     Serial.println("\nConectado a WiFi!");
//     Serial.print("IP: ");
//     Serial.println(WiFi.localIP());
//     server.begin();
//     while(!client) {
//         client = server.available();
//         delay(100);
//     }
// }

// void loop() {
//     client.print("hola desde red del tec");
// }

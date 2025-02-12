#define RX_PIN 16  // Adjust based on ESP module
#define TX_PIN 17  

void setup() {
    Serial.begin(115200);  // Debugging in Serial Monitor
    Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);  // UART communication with Jetson Nano

    pinMode(LED_BUILTIN, OUTPUT);  // Example: Using built-in LED to react
}

void loop() {
    if (Serial2.available()) {
        String received = Serial2.readStringUntil('\n');  // Read number sent from Jetson
        int number = received.toInt();  // Convert to integer

        Serial.print("Received Number: ");
        Serial.println(number);

        // Example action: Blink LED based on received number
        if (number % 2 == 0) {
            digitalWrite(LED_BUILTIN, HIGH);  
        } else {
            digitalWrite(LED_BUILTIN, LOW);  
        }
    }
}
/*
Open the Serial Monitor in Arduino IDE to see received numbers.
The ESP will toggle its LED based on the received number.

Debugging options:
    Check Serial Ports:
        ls /dev/tty*
    Ensure correct baud rate in both devices (9600 in this case).
    Use a USB-TTL converter to debug the ESP’s serial output.
*/

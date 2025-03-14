#include <iostream>
#include <fstream>
#include <unistd.h> // For sleep function

using namespace std;

int main() {
    string port = "/dev/ttyTHS1";  // Adjust this if needed (use /dev/ttyS0 if THS1 is unavailable)
    ofstream serial(port);

    if (!serial.is_open()) {
        cerr << "Failed to open serial port!" << endl;
        return 1;
    }

    while (true) {
        int number;
        cout << "Enter a number to send to ESP: ";
        cin >> number;

        serial << number << endl;
        serial.flush();

        cout << "Sent: " << number << endl;
        sleep(1);
    }

    serial.close();
    return 0;
}

/*
running: 
g++ -o serial_send serial_send.cpp
./serial_send
*/

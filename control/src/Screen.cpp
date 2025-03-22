#include "Screen.h"

void screenPrint(const String& output){
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
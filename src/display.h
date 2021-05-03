#ifndef DISPLAY
#define DISPLAY
#pragma once

#include <Arduino.h>
#include <LedDisplay.h>
#include "globals.h"
#include "midi.h"

extern bool DEBUG;

enum class DisplayType {
    LED,
    OLED,
    TFT
    // ...
};

namespace hjkl {
/*
* @brief Display control for HJKL
* @author Niklas Adam
* @date 23-07-2020
*/

// led display pins:
constexpr int dataPin = 13;         // the display's data in
constexpr int registerSelect = 25;  // the display's register select pin
constexpr int clockPin = 26;        // the display's clock pin
constexpr int enable = 32;          // the display's chip enable pin
constexpr int reset = 33;           // the display's reset pin

// led display settings
constexpr int displayLength = 8;    // number of characters
int displayBrightness;              // screen brightness

// start an instance of the LED display library:
LedDisplay display = LedDisplay(dataPin, registerSelect, clockPin,
enable, reset, displayLength);

/*
* @brief Display Class
* @author Niklas Adam
* @date 23-07-2020
*/

class Display {
public:
/* int pin; */
/* DisplayType type; */

void setup() {
    display.begin();
};

void version() {
    display.clear();
    display.print("HJKLv1.0");
    delay(1000);
    display.clear();
    display.print(display.version(), DEC);
    delay(1000);
    display.clear();
    delay(100);
}

void brightness() {
}

void welcome(MIDIMODE mode) {
    switch(mode) {
        case MIDIMODE::_7BIT:
            display.clear();
            display.setCursor(2);
            display.print("midi");
            delay(700);
            display.clear();
            delay(100);
            break;
        case MIDIMODE::_14BIT:
            display.clear();
            display.setCursor(1);
            display.print("14 bit");
            delay(700);
            display.print("s");
            delay(700);
            display.clear();
            delay(100);
            break;
        default:
            display.clear();
            display.setCursor(0);
            display.print("MODE_ERR"); // MISSING MIDI MODE
            break;
    }
};

void update() {
};
} // namespace hjkl
#endif

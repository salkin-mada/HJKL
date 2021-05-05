#ifndef SATANDISPLAY
#define SATANDISPLAY
#pragma once

#include "Arduino.h"
#include "LedDisplay.h"
#include "globals.h"
#include "midi.h"

extern const bool DEBUG;

namespace hjkl {

/* enum class DisplayType { */
/*     LED, */
/*     OLED, */
/*     TFT */
/*     // ... */
/* }; */

// led display pins:
/* constexpr int dataPin = 13;         // the display's data in */
/* constexpr int registerSelect = 25;  // the display's register select pin */
/* constexpr int clockPin = 26;        // the display's clock pin */
/* constexpr int enable = 32;          // the display's chip enable pin */
/* constexpr int reset = 33;           // the display's reset pin */

// led display settings
/* constexpr int displayLength = 8;    // number of characters */
/* int displayBrightness;              // screen brightness */

/* LedDisplay display(int dataPin, int registerSelect, int clockPin, int enable, int reset, int displayLength); */
/* LedDisplay display; */

/*
* @brief Display Class
* @author Niklas Adam
* @date 23-07-2020
*/

/* template <int dataPin, int registerSelect, int clockPin, int enable, int reset, int displayLength> */
class Display {
public:
int displayBrightness;
/* int dataPin; */
/* int registerSelect; */
/* int clockPin; */
/* int enable; */
/* int reset; */
/* int displayLength; */
/* DisplayType type; */

LedDisplay display;
// instance und contructorrzz hmmm..
/* LedDisplay display = LedDisplay(dataPin, registerSelect, clockPin, enable, reset, displayLength); */

Display(int dataPin, int registerSelect, int clockPin, int enable, int reset, int displayLength)
: display(dataPin, registerSelect, clockPin, enable, reset, displayLength) {};

void setup() {
    display.begin();
    display.clear();
    display.setBrightness(10);
    for (auto i=0; i<20; i++) {
        display.setCursor(0);
        display.print(gen_random(8));
        delay(100);
    }
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
            delay(500);
            display.clear();
            delay(100);
            break;
        case MIDIMODE::_14BIT:
            display.clear();
            display.setCursor(1);
            display.print("14 bit");
            delay(500);
            display.print("s");
            delay(300);
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

void after_boot(int brightness) {
    display.setBrightness(brightness);
    display.setCursor(4);
    display.print(gen_random(4));
}

void update(String func, int brightness) {
    display.setBrightness(brightness);
    display.setCursor(0);
    display.print(func);
    display.setCursor(4);
    display.print(gen_random(4));
};

};

} // namespace hjkl

#endif

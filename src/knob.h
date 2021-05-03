#ifndef KNOB
#define KNOB
#pragma once

#include <Arduino.h>
#include <ResponsiveAnalogRead.h>
#include "midi.h"

namespace hjkl {
/*
* @brief Knob for HJKL
* @author Niklas Adam
* @date 23-07-2020
*/
class Knob {
    /* Knob * self; */
public:
int pin;
/* int number; */
/* }; */

/* ResponsiveAnalogRead knob = ResponsiveAnalogRead(); */
ResponsiveAnalogRead knob;
Midi midi;

int number;
int value;

Knob(int knob_pin, int knob_num, int RESOLUTION)
: knob(knob_pin, knob_num, true), midi(MIDIMODE::_14BIT),
number(knob_num) {
    pin = knob_pin;
    setup(RESOLUTION, pin);
}

/* 13 bit RESOLUTION = 8192 */
/* void setup(int RESOLUTION, int pin, Knob &self) { */
void setup(int RESOLUTION, int pin) {
    /* ResponsiveAnalogRead knob(pin, true); */
    /* self->setAnalogResolution(RESOLUTION); */
    /* knob.setAnalogResolution(8192); */
    knob.setAnalogResolution(RESOLUTION);
}

void read() {
    knob.update();
    if (knob.hasChanged()) {
        // Shift up from ADC's 13 bits to Midi 14 bit
        /* analogVal_14_bit[0] = knob.getValue() << 1; */
        /* Serial.println(knob.getValue() << 1); */
        /* value = (knob.getValue() << 1); */
        value = knob.getValue();
        midi.sendCC(value, number, channel);
        /* Serial.println(knob.getValue()); */
        /* Serial.println(value); */
    }
}

void print() {
    Serial.print("Knob num ");
    Serial.print(number);
    Serial.print(" on pin ");
    Serial.print(pin);
    Serial.print(": ");
    Serial.println(value);
}
};
} // namespace hjkl

#endif

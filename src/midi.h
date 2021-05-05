#ifndef SATANMIDI
#define SATANMIDI
#pragma once

#include "globals.h"
/* #include "usb_midi.h" */

namespace hjkl {

/* extern usb_midi_class usbMIDI; */

/**
* @brief Midi sender class for HJKL
* @author Niklas Adam
* @date 01-05-2021
*/
class Midi {
/* Midi * self; */ // self pointer

public:
MIDIMODE mode;
int maxMIDIvalue;
/* int channel; */
int ccNum;

Midi(MIDIMODE initmode) : mode(initmode){
    init();
    /* setup(); */
};

void init() { changeMidiMode(mode); }

/* void setup() { usbMIDI.begin(); } */

void sendCC(int value, int knob_num, int channel) {
    ccNum = knob_num;
    switch (mode) {
        case MIDIMODE::_7BIT:
            send7BitMidi(ccNum, value, channel);
            break;
        case MIDIMODE::_14BIT:
            send14BitMidi(ccNum, value, channel);
            break;
    }
}

void noteOn(int note, int velocity, int channel) {
    usbMIDI.sendNoteOn(note, velocity, channel);
}
void noteOff(int note, int velocity, int channel) {
    usbMIDI.sendNoteOff(note, velocity, channel);
}

/* auto calculateNoteValues(int button_num, int current_pitch_page) { */
/*     auto val = 36+button_num; */
/*     return val; */
/* } */

void changeMidiMode(MIDIMODE newmode) {
    switch (newmode) {
        case MIDIMODE::_7BIT:
            mode = newmode;
            maxMIDIvalue = 127;
            break;
        case MIDIMODE::_14BIT:
            mode = newmode;
            maxMIDIvalue = 16383;
            break;
    }
}

void discardIncomingMIDI() {
    while (usbMIDI.read()); // read and discard any incoming MIDI messages
}

private:
// Expects 14 bit values in range 0 - 16383
void send14BitMidi(int lowerccnum, int value, int channel) {
    // Throw away everything above 14 bits;
    /* value = value << 14; */

    // Clip values
    if (value > maxMIDIvalue) {
        value = maxMIDIvalue;
    } else if (value < 0) {
        value = 0;
    }

    // Lower 7 bits of signal
    auto lowBitVal = value & 0x7F;

    // Upper 7 bits of signal
    auto highBitVal = (value >> 7) & 0x7F;

    usbMIDI.sendControlChange(lowerccnum + 32, lowBitVal, channel);
    usbMIDI.sendControlChange(lowerccnum, highBitVal, channel);
}
// Expects 7 bit values in range 0 - 127
void send7BitMidi(int cnum, int value, int channel) {
    // Throw away everything above 14 bits;
    /* value = value << 14; */

    // Clip values
    if (value > maxMIDIvalue) {
        value = maxMIDIvalue;
    } else if (value < 0) {
        value = 0;
    }

    usbMIDI.sendControlChange(cnum, value, channel);
}
};
} // namespace hjkl

#endif

#ifndef BUTTON
#define BUTTON
#pragma once

#include <Arduino.h>
#include <Bounce2.h>
#include "globals.h"
/* #include "usb_midi.h" */

namespace hjkl {

enum class ButtonAction {
    Nothing,
    FuncSwitch,
    H,
    J,
    K,
    L,
    Eval,
    PitchPageUp,
    PitchPageDown,
    SendNoteOn,
    SendNoteOff,
    Print
};

/*
* @brief Button for HJKL
* @author Niklas Adam
* @date 23-07-2020
*/
class Button {
public:
int pin;
ButtonAction onAction, offAction;
int buttonFunc;
/* hjkl_state &settings; */

Bounce button = Bounce();
/* Bounce debouncer[ numDigitalPinsForPageSellection + numDigitalPins ] = {Bounce()}; //deboucing */
int buttonNumber;

Button(int initPin, int debounce_time, int button_num)
: button(initPin, debounce_time),
buttonNumber(button_num) {
    pin = initPin;
};

void setup(ButtonAction buttonOnAction, ButtonAction buttonOffAction) {
    onAction = buttonOnAction;
    offAction = buttonOffAction;
    state = HIGH; // not pressed

    pinMode(pin, INPUT_PULLUP);
};

void read() {
    button.update();
    auto reading = button.read();
    /* int reading = button.read(); */

    // If value is new, perform actions
    if (reading != state) {

        if (reading == HIGH) {
            performOffAction();
        } else if (reading == LOW) {
            performOnAction();
        }

        // Update state
        state = reading;
    }
};

void setButtonOnAction(ButtonAction buttonAction) { onAction = buttonAction; }
void setButtonOffAction(ButtonAction buttonAction) { offAction = buttonAction; }

void print() {
    Serial.print("Button num ");
    Serial.print(buttonNumber);
    Serial.print(" on pin ");
    Serial.print(pin);
    Serial.print(": ");
    Serial.println(state);
}

private:
int state;
void performAction(ButtonAction doit) {
    switch (doit) {
        case ButtonAction::Nothing:
            break;
        case ButtonAction::FuncSwitch:
            /* buttonFunc = 1; */
            break;
        case ButtonAction::H:
            //Keyboard.press(KEY_H);
            //Keyboard.release(KEY_H);
            break;

        case ButtonAction::J:
            //Keyboard.press(KEY_J);
            //Keyboard.release(KEY_J);
            break;

        case ButtonAction::K:
            //Keyboard.press(KEY_K);
            //Keyboard.release(KEY_K);
            break;

        case ButtonAction::L:
            //Keyboard.press(KEY_L);
            //Keyboard.release(KEY_L);
            break;

        case ButtonAction::Eval:
            /* //Keyboard.press(); */
            /* //Keyboard.release(); */
            break;

        case ButtonAction::PitchPageUp:
            pitchPage++;
            /* //Keyboard.press(); */
            /* //Keyboard.release(); */
            break;

        case ButtonAction::PitchPageDown:
            pitchPage--;
            /* //Keyboard.press(); */
            /* //Keyboard.release(); */
            break;

        case ButtonAction::SendNoteOn:
            usbMIDI.sendNoteOn(calculate_midi_note(buttonNumber, pitchPage, num_buttons-num_special_buttons), 127, channel);
            /* usbMIDI.sendNoteOn(36, 127, channel); */
            /* midi.noteOn(36, 127, channel); */
            break;

        case ButtonAction::SendNoteOff:
            usbMIDI.sendNoteOff(calculate_midi_note(buttonNumber, pitchPage, num_buttons-num_special_buttons), 127, channel);
            /* midi.noteOff(36, 127, channel); */
            break;

        case ButtonAction::Print:
            print();
            break;
    }
}

void performOnAction() { performAction(onAction); }

void performOffAction() { performAction(offAction); }
};

} // namespace hjkl

#endif

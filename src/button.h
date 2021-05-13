#ifndef SATANBUTTON
#define SATANBUTTON
#pragma once

#include <Arduino.h>
#include <Bounce2.h>
#include "globals.h"
/* #include "usb_midi.h" */
/* #include <usb_keyboard.h> */
/* #include "usb_keyboard.h" */

namespace hjkl {

enum class ButtonAction {
    Nothing,
    FuncSwitch,
    H,
    J,
    K,
    L,
    PAGE_UP,
    PAGE_DOWN,
    EVAL,
    PitchUp,
    PitchDown,
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
: button(),
buttonNumber(button_num) {
    pin = initPin;
};

void setup(ButtonAction buttonOnAction, ButtonAction buttonOffAction) {
    onAction = buttonOnAction;
    offAction = buttonOffAction;
    state = HIGH; // not pressed

    pinMode(pin, INPUT_PULLUP);
    /* button.attach(pin, INPUT_PULLUP); */
    button.attach(pin);
    button.interval(5);
};


elapsedMillis retrig_msec = 0;
elapsedMillis hold_msec = 0;
bool holded = false;
bool first_press = false;
void read() {
    button.update();
    auto reading = button.read();
    if(keyboard_func) {
        if (reading != state || reading == LOW) {
            /* if (!holded) {hold_msec=0;} */
            if (reading == HIGH) {
                performOffAction();
                holded = false;
                first_press = false;
            } else if (reading == LOW) {
                if (hold_msec >= 600) { holded = true; retrig_msec=0; hold_msec = 0; }
                if (holded) {
                    if(retrig_msec >= 38) {
                        performOnAction();
                        retrig_msec = 0;
                    }
                } else {
                    if (!first_press) {
                        hold_msec = 0;
                        performOnAction();
                        first_press = true;
                    }
                }
            }
            state = reading;
        } else {
            hold_msec = 0;
        }

    } else {
        if (reading != state) {
            if (reading == HIGH) {
                performOffAction();
            } else if (reading == LOW) {
                performOnAction();
            }
            state = reading;
        }
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
            if (keyboard_func) {
                keyboard_func = false;
            } else {
                keyboard_func = true;
            }
            break;
        case ButtonAction::H:
            Keyboard.press(KEY_H);
            Keyboard.release(KEY_H);
            break;
        case ButtonAction::J:
            Keyboard.press(KEY_J);
            Keyboard.release(KEY_J);
            break;
        case ButtonAction::K:
            Keyboard.press(KEY_K);
            Keyboard.release(KEY_K);
            break;
        case ButtonAction::L:
            Keyboard.press(KEY_L);
            Keyboard.release(KEY_L);
            break;
        case ButtonAction::PAGE_UP:
            Keyboard.press(KEY_PAGE_UP);
            Keyboard.release(KEY_PAGE_UP);
            break;
        case ButtonAction::PAGE_DOWN:
            Keyboard.press(KEY_PAGE_DOWN);
            Keyboard.release(KEY_PAGE_DOWN);
            break;
        case ButtonAction::EVAL:
            Keyboard.set_modifier(MODIFIERKEY_CTRL);
            /* Keyboard.set_key1(KEY_E); */
            Keyboard.send_now();
            Keyboard.press(KEY_E);
            Keyboard.set_modifier(0);
            /* Keyboard.set_key1(0); */
            Keyboard.send_now();
            Keyboard.release(KEY_E);
            /* //Keyboard.press(); */
            /* //Keyboard.release(); */
            break;
        case ButtonAction::PitchUp:
            pitchPage++;
            /* //Keyboard.press(); */
            /* //Keyboard.release(); */
            break;
        case ButtonAction::PitchDown:
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

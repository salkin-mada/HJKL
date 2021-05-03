#include <Arduino.h>
#include "button.h"
#include "knob.h"
/* #include "data.h" */
/* #include "display.h" */
#include <LedDisplay.h>
#include "settings.h"
#include <array>
/* #include "usb_midi.h" */
#include "midi.h"
/* extern usb_midi_class usbMIDI; */
/* Midi midi; */

constexpr bool DEBUG = false;

using namespace hjkl;

constexpr int num_knobs = 20;

/* hjkl_state settings; */

constexpr int min_val = 0;
constexpr int _7bit_max = 127;
constexpr int _14bit_max = 16383;
int midi_mode_max;

/* Data<num_pages, num_knobs> data; */
/* Display<num_pages, num_knobs> display(settings, data); */
/* Display display; */
constexpr int dataPin = 13;         // the display's data in
constexpr int registerSelect = 25;  // the display's register select pin
constexpr int clockPin = 26;        // the display's clock pin
constexpr int enable = 32;          // the display's chip enable pin
constexpr int reset = 33;           // the display's reset pin
constexpr int displayLength = 8;    // number of characters
LedDisplay display = LedDisplay(dataPin, registerSelect, clockPin,
enable, reset, displayLength);

// Initialize knobs
/* typedef std::array<int, 1> KnobPin; */
/* std::array<std::array<int, 1>, num_knobs> knob_pins = { */
/* KnobPin{A0}, KnobPin{A1}, KnobPin{A2}, KnobPin{A3}, KnobPin{A4}, */
/* KnobPin{A5}, KnobPin{A6}, KnobPin{A7}, KnobPin{A8}, KnobPin{A9}, */
/* KnobPin{A10}, KnobPin{A11}, KnobPin{A13}, KnobPin{A12}, KnobPin{A14}, */
/* KnobPin{A16}, KnobPin{A17}, KnobPin{A18}, KnobPin{A19}, KnobPin{A20} */
/* }; */
std::array<int, num_knobs> knob_pins = {
A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10,
A11, A13, A12, A14, A16, A17, A18, A19, A20
};
/* auto RESOLUTION = 8192; */
auto RESOLUTION = 16384;
/* typedef Knob<num_knobs> knob; */
std::array<Knob, num_knobs> knobs{
Knob(knob_pins[0],1, RESOLUTION),
Knob(knob_pins[1],2, RESOLUTION),
Knob(knob_pins[2],3, RESOLUTION),
Knob(knob_pins[3],4, RESOLUTION),
Knob(knob_pins[4],5, RESOLUTION),
Knob(knob_pins[5],6, RESOLUTION),
Knob(knob_pins[6],7, RESOLUTION),
Knob(knob_pins[7],8, RESOLUTION),
Knob(knob_pins[8],9, RESOLUTION),
Knob(knob_pins[9],10, RESOLUTION),
Knob(knob_pins[10],11, RESOLUTION),
Knob(knob_pins[11],12, RESOLUTION),
Knob(knob_pins[12],13, RESOLUTION),
Knob(knob_pins[13],14, RESOLUTION),
Knob(knob_pins[14],15, RESOLUTION),
Knob(knob_pins[15],16, RESOLUTION),
Knob(knob_pins[16],17, RESOLUTION),
Knob(knob_pins[17],18, RESOLUTION),
Knob(knob_pins[18],19, RESOLUTION),
Knob(knob_pins[19],20, RESOLUTION)
};

// Initialize buttons
constexpr int debounce = 10; // ms
std::array<Button, num_buttons> buttons{
Button(4, debounce, 0),
Button(3, debounce, 1),
Button(2, debounce, 2), // K
Button(1, debounce, 3),
Button(0, debounce, 4),
Button(9, debounce, 5),
Button(8, debounce, 6), // H
Button(7, debounce, 7), // J
Button(6, debounce, 8), // L
Button(5, debounce, 9),
Button(10, debounce, 10), // midi pitch up
Button(11, debounce, 11), // midi pitch down
Button(12, debounce, 12), // switch functionality on 10 lower buttons
Button(24, debounce, 13)  // nvim <C-e> aka evaluate
};

void setup() {
    /* start usb */
    /* usbMIDI.begin(); */

    /* resolution */
    analogReadResolution(14); // 13 max recommend baby

    /* smooth input values */
    analogReadAveraging(32); // 16 virkede

    display.begin();
    display.clear();
    display.setBrightness(10);
    for (auto i=0; i<20; i++) {
        display.setCursor(0);
        display.print(gen_random(8));
        delay(100);
    }

        // Lower buttons
        for (auto b = 0; b < (buttons.size()-num_special_buttons); b++) {
            buttons[b].setup(ButtonAction::SendNoteOn, ButtonAction::SendNoteOff);
        }
        // Upper buttons
        buttons[10].setup(ButtonAction::PitchPageDown, ButtonAction::Nothing);
        buttons[11].setup(ButtonAction::PitchPageUp, ButtonAction::Nothing);
        buttons[12].setup(ButtonAction::Nothing, ButtonAction::FuncSwitch);
        buttons[13].setup(ButtonAction::Eval, ButtonAction::Nothing);
    if (DEBUG) {
        // set baudrate?
        while (!Serial) {}
        Serial.println("DEBUG MODE");
        for (auto b = 0; b < (buttons.size()); b++) {
            buttons[b].setup(ButtonAction::Print, ButtonAction::Nothing);
        }

    }
    /* display.setup(); */
}

void readButtons() {
    for (auto btn = 0; btn < buttons.size(); btn++) {
        buttons[btn].read();
    }
    if (DEBUG) {
        for (auto btn = 0; btn < buttons.size(); btn++) {
            /* buttons[btn].print(); */
        }
    }
}

void readKnobs() {
    for (auto knb = 0; knb < knobs.size(); knb++) {
        knobs[knb].read();
    }
    if (DEBUG) {
        for (auto knb = 0; knb < knobs.size(); knb++) {
        /* knobs[knb].print(); */
        }
    }
}

void loop() {
    readKnobs();
    readButtons();
    if (DEBUG) {
        delay(50);
        /* display.setCursor(0); */
        /* display.print(gen_random(8)); */
    }
    /* midi.discardIncomingMIDI(); */
    /* while (usbMIDI.read()); // read and discard any incoming MIDI messages */
}

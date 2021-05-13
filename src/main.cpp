#include <Arduino.h>
#include "button.h"
#include "knob.h"
/* #include "data.h" */
#include <LedDisplay.h>
#include "settings.h"
#include <array>
/* #include "usb_midi.h" */
#include "midi.h"
#include "display.h"
/* extern usb_midi_class usbMIDI; */
/* #include "usb_desc.h" */

constexpr bool DEBUG = false;

using namespace hjkl;

Midi midi(MIDIMODE::_14BIT);

constexpr int num_knobs = 20;

/* hjkl_state settings; */

constexpr int min_val = 0;
constexpr int _7bit_max = 127;
constexpr int _14bit_max = 16383;
int midi_mode_max;

/* Data<num_pages, num_knobs> data; */
/* Display<num_pages, num_knobs> display(settings, data); */
/* Display display = Display(13,25,26,32,33,8); */
/* Display display = LedDisplay(13,25,26,32,33,8); */
Display display(13,25,26,32,33,8);
/* std::array<Display, 1> displays{Display(13,25,26,32,33,8)}; */
/* Display<13,25,26,32,33,8> display; */
/* LedDisplay Display(13,25,26,32,33,8); */
/* constexpr int dataPin = 13;         // the display's data in */
/* constexpr int registerSelect = 25;  // the display's register select pin */
/* constexpr int clockPin = 26;        // the display's clock pin */
/* constexpr int enable = 32;          // the display's chip enable pin */
/* constexpr int reset = 33;           // the display's reset pin */
/* constexpr int displayLength = 8;    // number of characters */
/* LedDisplay display = LedDisplay(dataPin, registerSelect, clockPin, */
/* enable, reset, displayLength); */

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

    /* resolution */
    analogReadResolution(14); // 13 max recommend baby

    /* smooth input values */
    analogReadAveraging(32); // 16 virker fint med 13BIT ADC

    display.setup();
    display.welcome(MIDIMODE::_14BIT);
    /* display.begin(); */
    /* display.clear(); */
    /* display.setBrightness(10); */
    /* for (auto i=0; i<20; i++) { */
    /*     display.setCursor(0); */
    /*     display.print(gen_random(8)); */
    /*     delay(100); */
    /* } */

    if (DEBUG) {
        while (!Serial);
        Serial.println("DEBUG MODE");
        for (size_t b = 0; b < buttons.size(); b++) {
            Serial.print(b);
            buttons[b].setup(ButtonAction::Print, ButtonAction::Nothing);
        }
        Serial.println();
    } else {
        // INIT BUTTON SETUP
        // Lower buttons
        for (size_t b = 0; b < (buttons.size()-num_special_buttons); b++) {
            buttons[b].setup(ButtonAction::SendNoteOn, ButtonAction::SendNoteOff);
        }
        // Upper buttons
        buttons[10].setup(ButtonAction::PitchDown, ButtonAction::Nothing);
        buttons[11].setup(ButtonAction::PitchUp, ButtonAction::Nothing);
        buttons[12].setup(ButtonAction::Nothing, ButtonAction::FuncSwitch);
        buttons[13].setup(ButtonAction::EVAL, ButtonAction::Nothing);

        keyboard_state = keyboard_func;
    }

    display.after_boot(5);
}

void readButtons() {
    for (size_t btn = 0; btn < buttons.size(); btn++) {
        buttons[btn].read();
    }
    /* if (DEBUG) { */
    /*     for (size_t btn = 0; btn < buttons.size(); btn++) { */
    /*         buttons[btn].print(); */
    /*     } */
    /* } */
}

void readKnobs() {
    for (size_t knb = 0; knb < knobs.size(); knb++) {
        knobs[knb].read();
    }
    if (DEBUG) {
        for (size_t knb = 0; knb < knobs.size(); knb++) {
            /* knobs[knb].print(); */
        }
    }
}

void midi_btns() {
    for (size_t b = 0; b < (buttons.size()-num_special_buttons); b++) {
        buttons[b].setup(ButtonAction::SendNoteOn, ButtonAction::SendNoteOff);
    }
    buttons[10].setup(ButtonAction::PitchDown, ButtonAction::Nothing);
    buttons[11].setup(ButtonAction::PitchUp, ButtonAction::Nothing);
}
void keyboard_btns() {
    buttons[0].setup(ButtonAction::Nothing, ButtonAction::Nothing);
    buttons[1].setup(ButtonAction::Nothing, ButtonAction::Nothing);
    buttons[2].setup(ButtonAction::K, ButtonAction::Nothing);
    buttons[3].setup(ButtonAction::Nothing, ButtonAction::Nothing);
    buttons[4].setup(ButtonAction::Nothing, ButtonAction::Nothing);
    buttons[5].setup(ButtonAction::Nothing, ButtonAction::Nothing);
    buttons[6].setup(ButtonAction::H, ButtonAction::Nothing);
    buttons[7].setup(ButtonAction::J, ButtonAction::Nothing);
    buttons[8].setup(ButtonAction::L, ButtonAction::Nothing);
    buttons[9].setup(ButtonAction::Nothing, ButtonAction::Nothing);
    buttons[10].setup(ButtonAction::PAGE_DOWN, ButtonAction::Nothing);
    buttons[11].setup(ButtonAction::PAGE_UP, ButtonAction::Nothing);
}

elapsedMillis msec = 0;
void loop() {
    if(keyboard_state != keyboard_func) {
        if(keyboard_func) {
            keyboard_btns();
        } else {
            midi_btns();
        }
        keyboard_state = keyboard_func;

        String info = "HJKL";
        if(keyboard_func) {info=".|\\~";};
        display.update(info, 10);
    };
    /* if(msec >= 50) {String info = "HJKL"; if(keyboard_func){info=".|\\~";}; display.update(info, 10); msec = 0; }; */
    readKnobs();
    readButtons();
    midi.discardIncomingMIDI();

    if (DEBUG) {
        delay(50);
        /* display.setCursor(0); */
        /* display.print(gen_random(8)); */
    }
}

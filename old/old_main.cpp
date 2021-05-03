/*___               __      _                __            ___              ___//
|                                                                               |
|                                                   
                            HJKL 14 BIT MIDI
|                                                                               |
//___          _     _             __                ___        __          ___*/

/*							or really just 13  								   */

/*
antal 5V: 4 (to display)
antal 3.3V 14+20 = 34
antal gnd: 14 (to digital buttons) + 3 (to display) = 17
antal agnd: 20 (caps to analog pins) + 20 (to analog knobs) = 40
*/

#include <Arduino.h>
#include <ResponsiveAnalogRead.h>
#include <LedDisplay.h>
#include <Bounce2.h>
#include "Utilities.h"
#include "ValueHolder.h"

// led display pins:
#define dataPin 13            // the display's data in
#define registerSelect 25     // the display's register select pin
#define clockPin 26           // the display's clock pin
#define enable 32             // the display's chip enable pin
#define reset 33              // the display's reset pin
// led display settings
#define displayLength 8       // number of characters

#define TRUE 1
#define FALSE 0

// for 14bit serial
/* Set analog resolution (13 bit adc)*/
#define RESOLUTION 8192
/* lo and hi bits */ 
#define LOWCC1  1
#define LOWCC2  2
#define LOWCC3  3
#define LOWCC4  4
#define LOWCC5  5
#define LOWCC6  6
#define LOWCC7  7
#define LOWCC8  8
#define LOWCC9  9
#define LOWCC10 10
#define LOWCC11 11
#define LOWCC12 12
#define LOWCC13 13
#define LOWCC14 14
#define LOWCC15 15
#define LOWCC16 16
#define LOWCC17 17
#define LOWCC18 18
#define LOWCC19 19
#define LOWCC20 20
#define HIGHCC1  LOWCC1  + 32
#define HIGHCC2  LOWCC2  + 32
#define HIGHCC3  LOWCC3  + 32
#define HIGHCC4  LOWCC4  + 32
#define HIGHCC5  LOWCC5  + 32
#define HIGHCC6  LOWCC6  + 32
#define HIGHCC7  LOWCC7  + 32
#define HIGHCC8  LOWCC8  + 32
#define HIGHCC9  LOWCC9  + 32
#define HIGHCC10 LOWCC10 + 32
#define HIGHCC11 LOWCC11 + 32
#define HIGHCC12 LOWCC12 + 32
#define HIGHCC13 LOWCC13 + 32
#define HIGHCC14 LOWCC14 + 32
#define HIGHCC15 LOWCC15 + 32
#define HIGHCC16 LOWCC16 + 32
#define HIGHCC17 LOWCC17 + 32
#define HIGHCC18 LOWCC18 + 32
#define HIGHCC19 LOWCC19 + 32
#define HIGHCC20 LOWCC20 + 32

// modes
#define MODE_DEFAULT 0
#define MODE_MIDI 1
#define MODE_THIRD 2

// mode holder
unsigned int mode;

int displayBrightness;        // screen brightness
// start an instance of the LED display library:
LedDisplay myDisplay = LedDisplay(dataPin, registerSelect, clockPin,
enable, reset, displayLength);

/* int num_pages = 4; */
// page translation map for display
char pageNumberToCharTranslation[] = {
'A','B','C','D'
};
int currentMidiValForDisplay;
int controlChangeNumberForDisplay;
int neededNumberOfPlaceholdersForControlChangeVal = 0; // no needed number of midi-cc-value-placeholders yet .
int myCursorPositionForControlChangeVal;
int neededNumberOfPlaceholdersForControlChangeNumber = 0; // no needed number of midi-cc-number-placeholders yet .
int myCursorPositionForControlChangeNumber;
int displayUpdateType;
int displayClearDone;
char CcValueDisplayBuffer[3];
char CcNumberDisplayBuffer[2];
// char holder for 14 bit
char CcValueDisplayBuffer_14bit[5];


//CC analog pins:
int const num_analog_pins = 20;
int analogPins[] = { // A12 og A13 er vent om på hardware =?!!????
A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A13,A12,A14,A16,A17,A18,A19,A20
};
int currentAnalogVal[num_analog_pins];
int newAnalogVal[num_analog_pins];
int analogThreshold = 3; // filtrering (1 = non thresh)

/* 14 bit settings */
/* storage */
int analogVal_14_bit[num_analog_pins];
/* Set up read pins */
ResponsiveAnalogRead pot1(analogPins[0], true);
ResponsiveAnalogRead pot2(analogPins[1], true);
ResponsiveAnalogRead pot3(analogPins[2], true);
ResponsiveAnalogRead pot4(analogPins[3], true);
ResponsiveAnalogRead pot5(analogPins[4], true);
ResponsiveAnalogRead pot6(analogPins[5], true);
ResponsiveAnalogRead pot7(analogPins[6], true);
ResponsiveAnalogRead pot8(analogPins[7], true);
ResponsiveAnalogRead pot9(analogPins[8], true);
ResponsiveAnalogRead pot10(analogPins[9], true);
ResponsiveAnalogRead pot11(analogPins[10], true);
ResponsiveAnalogRead pot12(analogPins[11], true);
ResponsiveAnalogRead pot13(analogPins[12], true);
ResponsiveAnalogRead pot14(analogPins[13], true);
ResponsiveAnalogRead pot15(analogPins[14], true);
ResponsiveAnalogRead pot16(analogPins[15], true);
ResponsiveAnalogRead pot17(analogPins[16], true);
ResponsiveAnalogRead pot18(analogPins[17], true);
ResponsiveAnalogRead pot19(analogPins[18], true);
ResponsiveAnalogRead pot20(analogPins[19], true);

// notes digital pins:
int const numDigitalPins =  10;
int currentDigitalVal[numDigitalPins];
int newDigitalVal[numDigitalPins];
int digitalPins[] = { // lidt underlig rækkefølge fordi hardware er koblet wack
4,3,2,1,0,9,8,7,6,5
};

// page buttons -> digital pins:
int const numDigitalPinsForPageSellection = 4;
int digitalPinsForPageSellection[] = {
10,11,12,24
};

Bounce debouncer[ numDigitalPinsForPageSellection + numDigitalPins ] = {Bounce()}; //deboucing

// timer
elapsedMillis msec = 0;

// midi channel
const int channel = 1;

// boot from pageA(0)
int currentSellectedPage = 0;
int pageDependentMidiPitchOffset = 0;
int pageDependentControlChangeNumberOffset = 0;
// pitch for buttons
int midiPitch[] = {
36,37,38,39,40,41,42,43,44,45
};
// page selectors
bool p_sel[numDigitalPinsForPageSellection] = {FALSE};

void setup() {
/*  ~~~ ~~~ ~~~ init config BEGIN ~~~ ~~~ ~~~
    ~~~ ~~~ ~~~ ~~~ ~~~   ~~~ ~~~ ~~~ ~~~ ~~~   */
    /* Serial.begin(19200); */

    // Pullup for button pins
    for (int i = 0; i < numDigitalPins; i++) {
        pinMode (digitalPins[i], INPUT_PULLUP);
    }
    // Pullup for page sellect pins
    for (int i = 0; i < numDigitalPinsForPageSellection; i++) {
        pinMode (digitalPinsForPageSellection[i], INPUT_PULLUP);
    }
    // debounce setup
    for (int i = 0; i < numDigitalPinsForPageSellection; ++i) {
        debouncer[i].attach(digitalPinsForPageSellection[i]);
        debouncer[i].interval(5);
    }

    for (int i = 0; i < numDigitalPins; ++i) {
        debouncer[i+numDigitalPinsForPageSellection].attach(digitalPins[i]);
        debouncer[i+numDigitalPinsForPageSellection].interval(5);
    }

    // initialize the display library:
    myDisplay.begin();
    // check the display version
    // Serial.println(myDisplay.version(), DEC);

    // check if page select buttons are pressed during boot up
    for (int i = 0; i < numDigitalPinsForPageSellection; i++) {
        if(digitalRead(digitalPinsForPageSellection[i]) == 0) {
            p_sel[i] = TRUE;
        }
    }
/*  ~~~ ~~~ ~~~ init config DONE ~~~ ~~~ ~~~
    ~~~ ~~~ ~~~ ~~~ ~~~   ~~~ ~~~ ~~~ ~~~ ~~~   */

    /* if no page select buttons are pressed load default - 14 bit serial mode */
    if (p_sel[0] == FALSE && p_sel[1] == FALSE && p_sel[2] == FALSE && p_sel[3] == FALSE) {

        mode = MODE_DEFAULT;

        /* resolution */
        analogReadResolution(13);

        /* smooth input values */
        analogReadAveraging(16);

        // setup potentiometers
        pot1.setAnalogResolution(RESOLUTION);
        pot2.setAnalogResolution(RESOLUTION);
        pot3.setAnalogResolution(RESOLUTION);
        pot4.setAnalogResolution(RESOLUTION);
        pot5.setAnalogResolution(RESOLUTION);
        pot6.setAnalogResolution(RESOLUTION);
        pot7.setAnalogResolution(RESOLUTION);
        pot8.setAnalogResolution(RESOLUTION);
        pot9.setAnalogResolution(RESOLUTION);
        pot10.setAnalogResolution(RESOLUTION);
        pot11.setAnalogResolution(RESOLUTION);
        pot12.setAnalogResolution(RESOLUTION);
        pot13.setAnalogResolution(RESOLUTION);
        pot14.setAnalogResolution(RESOLUTION);
        pot15.setAnalogResolution(RESOLUTION);
        pot16.setAnalogResolution(RESOLUTION);
        pot17.setAnalogResolution(RESOLUTION);
        pot18.setAnalogResolution(RESOLUTION);
        pot19.setAnalogResolution(RESOLUTION);
        pot20.setAnalogResolution(RESOLUTION);

        // display setup feedback
        myDisplay.clear();
        myDisplay.setCursor(1);
        myDisplay.print("14 bit");
        delay(700);
        myDisplay.print("s");
        delay(700);
        // clear display
        myDisplay.clear();
        delay(500);
        /* if page A select button is pressed load classic midi mode */    
    } else if (p_sel[0] == TRUE && p_sel[1] == FALSE && p_sel[2] == FALSE && p_sel[3] == FALSE) {
        mode = MODE_MIDI;
        myDisplay.clear();
        myDisplay.setCursor(1);
        myDisplay.print("midi");
        delay(1000);
        // clear display
        myDisplay.clear();
        delay(500);
        /* if all is pressed, load/goto third modus */
    } else if (p_sel[0] == TRUE && p_sel[1] == TRUE && p_sel[2] == TRUE && p_sel[3] == TRUE) {
        mode = MODE_THIRD;
        myDisplay.clear();
        myDisplay.setCursor(1);
        myDisplay.print("third");
        for (int i = 0; i < 1; i++) {
            //fade up
            for (displayBrightness = 0; displayBrightness < 16; displayBrightness++) {
                myDisplay.setBrightness(displayBrightness);
                delay(100);
            }
            // zigzag top
            for (displayBrightness = 15; displayBrightness > 8; displayBrightness--) {
                myDisplay.setBrightness(displayBrightness);
                delay(22);
            }
            for (displayBrightness = 8; displayBrightness < 16; displayBrightness++) {
                myDisplay.setBrightness(displayBrightness);
                delay(22);
            }
            for (displayBrightness = 15; displayBrightness > 8; displayBrightness--) {
                myDisplay.setBrightness(displayBrightness);
                delay(22);
            }
            for (displayBrightness = 8; displayBrightness < 16; displayBrightness++) {
                myDisplay.setBrightness(displayBrightness);
                delay(22);
            }
            // fade down
            for (displayBrightness = 15; displayBrightness >= 0; displayBrightness--) {
                myDisplay.setBrightness(displayBrightness);
                delay(100);
            }
        }
        myDisplay.print("empty");
        delay(500);
        myDisplay.print(" mode ");
        delay(500);
        // clear display
        myDisplay.clear();
        delay(20);
    }

    /* greeting - all modes */
    // setup led display start_up
    myDisplay.home();
    myDisplay.print("/\\HJKL/\\");
    for (int i = 0; i < 4; i++) {
        // fade up
        for (displayBrightness = 0; displayBrightness < 16; displayBrightness++) {
            myDisplay.setBrightness(displayBrightness);
            delay(15);
        }
        // fade down
        for (displayBrightness = 15; displayBrightness >= 0; displayBrightness--) {
            myDisplay.setBrightness(displayBrightness);
            delay(15);
        }
    }
    myDisplay.clear();
    delay(20);
    myDisplay.setCursor(2);
    myDisplay.print("hello");
    // pre fade up
    for (displayBrightness = 0; displayBrightness <= 10; displayBrightness++) {
        myDisplay.setBrightness(displayBrightness);
        delay(15);
    }
    for (int i = 0; i < 8; i++) {
        // fade down
        for (displayBrightness = 11; displayBrightness >= 8; displayBrightness--) {
            myDisplay.setBrightness(displayBrightness);
            delay(10);
        }
        // fade up
        for (displayBrightness = 7; displayBrightness <= 10; displayBrightness++) {
            myDisplay.setBrightness(displayBrightness);
            delay(10);
        }
    }
    myDisplay.clear();

    // boot process end display brightness
    displayBrightness = 10; // eats values 0-15
    myDisplay.setBrightness(displayBrightness);

    // set curser to 0
    myDisplay.home();
    myDisplay.print("im ready");
    delay(1000);

    // clear display
    myDisplay.clear();
    delay(20);
    // display feedback for initial page number
    myDisplay.home();
    myDisplay.print(pageNumberToCharTranslation[currentSellectedPage]);
}

void clearDisplay() {
    if (displayClearDone == false) {
        myDisplay.clear();
        displayClearDone = true;
    }
}

void makeDisplayBright() {
    myDisplay.setBrightness(15);
}

void updateDisplay() {
    // page update
    if (displayUpdateType == 0) {
        myDisplay.home(); // cursor position 0
        myDisplay.print(pageNumberToCharTranslation[currentSellectedPage]);
    }
    // cc update (0-127 midi)
    if (displayUpdateType == 1) {
        //myDisplay.setCursor(myCursorPositionForControlChangeVal);
        myDisplay.setCursor(2);
        sprintf(CcNumberDisplayBuffer, "%2d", controlChangeNumberForDisplay);
        myDisplay.print(CcNumberDisplayBuffer);
        myDisplay.setCursor(5);
        sprintf(CcValueDisplayBuffer, "%3d", currentMidiValForDisplay);
        myDisplay.print(CcValueDisplayBuffer);
        // with fading response from used knob
        displayBrightness = map(currentMidiValForDisplay,0,127,1,5); // values 0-127 mapped to 1-10 (display eats 0-15)
        myDisplay.setBrightness(displayBrightness);
    }


    // cc update 14 bit
    if (displayUpdateType == 2) {
        char yoyo[2];
        myDisplay.setCursor(1);
        sprintf(yoyo, "%2d", controlChangeNumberForDisplay);
        myDisplay.print(yoyo);
        String lol = gen_random(const int 4);
        myDisplay.setCursor(4);
        myDisplay.print(lol);
        displayBrightness = random(1,5);
        myDisplay.setBrightness(displayBrightness);
    }
}

void fourteen_bit() {
    /* unsigned int jitter = random(50,3000); */
    /* if(msec >= jitter) { */
    /* 	msec = 0; */

    /* 	myDisplay.setCursor(random(2,5)); */
    /* 	myDisplay.print(random(0, 9999)); */
    /* } */
    // handle page sellection
    for (int i = 0; i < numDigitalPinsForPageSellection; i++) {
        debouncer[i].update();
        if (debouncer[i].read() == 0) {
            if (currentSellectedPage != i) {
                currentSellectedPage = i;
                // PAGE for midi pitch value- / control change number offset
                pageDependentMidiPitchOffset = numDigitalPins*i;
                /* pageDependentControlChangeNumberOffset = num_analog_pins*i; */

                // update display
                displayUpdateType = 0;
                updateDisplay();
            }
        }
    }

    if(msec >= 20) { // 20ms = 50 times per second .. thats enough. i.e. not bombarding receiver with midi values...
        msec = 0;

        // digital buttons - midi noteOn / noteOff
        for (int i = 0; i < numDigitalPins; i++) {
            debouncer[i+numDigitalPinsForPageSellection].update();
            if (debouncer[i+numDigitalPinsForPageSellection].read() == 0  && currentDigitalVal[i] == 1) {
                usbMIDI.sendNoteOn(
                midiPitch[i]+pageDependentMidiPitchOffset, 100, channel
                );
                currentDigitalVal[i] = 0; // flag

                myDisplay.setBrightness(6);

            }
            if (debouncer[i+numDigitalPinsForPageSellection].read() == 1 && currentDigitalVal[i] == 0) {
                usbMIDI.sendNoteOff(
                midiPitch[i]+pageDependentMidiPitchOffset, 100, channel
                );
                currentDigitalVal[i] = 1; // flag

                myDisplay.setBrightness(2);

            }
        }


        /* update and read */ 
        pot1.update();
        if (pot1.hasChanged()) {
            // Shift up from ADC's 13 bits to Midi 14 bit
            analogVal_14_bit[0] = pot1.getValue() << 1;

            usbMIDI.sendControlChange(HIGHCC1, analogVal_14_bit[0] & 0x7F, 1);
            usbMIDI.sendControlChange(LOWCC1, (analogVal_14_bit[0] >> 7) & 0x7F, 1);

            /* display */
            /* currentMidiValForDisplay = pot1.getValue() << 1; */
            controlChangeNumberForDisplay = 1;
            displayUpdateType = 2;
            updateDisplay();
        }

        pot2.update();
        if (pot2.hasChanged()) {
            analogVal_14_bit[1] = pot2.getValue() << 1;

            usbMIDI.sendControlChange(HIGHCC2, analogVal_14_bit[1] & 0x7F, 1);
            usbMIDI.sendControlChange(LOWCC2, (analogVal_14_bit[1] >> 7) & 0x7F, 1);

            // display
            // currentMidiValForDisplay = analogVal_14_bit[1];
            // controlChangeNumberForDisplay = 2;
            // displayUpdateType = 1;
            // updateDisplay();
        }

        pot3.update();
        if (pot3.hasChanged()) {
            analogVal_14_bit[2] = pot3.getValue() << 1;

            usbMIDI.sendControlChange(HIGHCC3, analogVal_14_bit[2] & 0x7F, 1);
            usbMIDI.sendControlChange(LOWCC3, (analogVal_14_bit[2] >> 7) & 0x7F, 1);

            // display
            // currentMidiValForDisplay = analogVal_14_bit[2];
            // controlChangeNumberForDisplay = 3;
            // displayUpdateType = 1;
            // updateDisplay();
        }

        pot4.update();
        if (pot4.hasChanged()) {
            analogVal_14_bit[3] = pot4.getValue() << 1;

            usbMIDI.sendControlChange(HIGHCC4, analogVal_14_bit[3] & 0x7F, 1);
            usbMIDI.sendControlChange(LOWCC4, (analogVal_14_bit[3] >> 7) & 0x7F, 1);

            // display
            // currentMidiValForDisplay = analogVal_14_bit[3];
            // controlChangeNumberForDisplay = 4;
            // displayUpdateType = 1;
            // updateDisplay();
        }

        pot5.update();
        if (pot5.hasChanged()) {
            analogVal_14_bit[4] = pot5.getValue() << 1;

            usbMIDI.sendControlChange(HIGHCC5, analogVal_14_bit[4] & 0x7F, 1);
            usbMIDI.sendControlChange(LOWCC5, (analogVal_14_bit[4] >> 7) & 0x7F, 1);

            // display
            // currentMidiValForDisplay = analogVal_14_bit[4];
            // controlChangeNumberForDisplay = 5;
            // displayUpdateType = 1;
            // updateDisplay();
        }

        pot6.update();
        if (pot6.hasChanged()) {
            analogVal_14_bit[5] = pot6.getValue() << 1;

            usbMIDI.sendControlChange(HIGHCC6, analogVal_14_bit[5] & 0x7F, 1);
            usbMIDI.sendControlChange(LOWCC6, (analogVal_14_bit[5] >> 7) & 0x7F, 1);

            // display
            // currentMidiValForDisplay = analogVal_14_bit[5];
            // controlChangeNumberForDisplay = 6;
            // displayUpdateType = 1;
            // updateDisplay();
        }

        pot7.update();
        if (pot7.hasChanged()) {
            analogVal_14_bit[6] = pot7.getValue() << 1;

            usbMIDI.sendControlChange(HIGHCC7, analogVal_14_bit[6] & 0x7F, 1);
            usbMIDI.sendControlChange(LOWCC7, (analogVal_14_bit[6] >> 7) & 0x7F, 1);

            // display
            // currentMidiValForDisplay = analogVal_14_bit[6];
            // controlChangeNumberForDisplay = 7;
            // displayUpdateType = 1;
            // updateDisplay();
        }

        pot8.update();
        if (pot8.hasChanged()) {
            analogVal_14_bit[7] = pot8.getValue() << 1;

            usbMIDI.sendControlChange(HIGHCC8, analogVal_14_bit[7] & 0x7F, 1);
            usbMIDI.sendControlChange(LOWCC8, (analogVal_14_bit[7] >> 7) & 0x7F, 1);

            // display
            // currentMidiValForDisplay = analogVal_14_bit[7];
            // controlChangeNumberForDisplay = 8;
            // displayUpdateType = 1;
            // updateDisplay();
        }

        pot9.update();
        if (pot9.hasChanged()) {
            analogVal_14_bit[8] = pot9.getValue() << 1;

            usbMIDI.sendControlChange(HIGHCC9, analogVal_14_bit[8] & 0x7F, 1);
            usbMIDI.sendControlChange(LOWCC9, (analogVal_14_bit[8] >> 7) & 0x7F, 1);

            // display
            // currentMidiValForDisplay = analogVal_14_bit[8];
            // controlChangeNumberForDisplay = 9;
            // displayUpdateType = 1;
            // updateDisplay();
        }

        pot10.update();
        if (pot10.hasChanged()) {
            analogVal_14_bit[9] = pot10.getValue() << 1;

            usbMIDI.sendControlChange(HIGHCC10, analogVal_14_bit[9] & 0x7F, 1);
            usbMIDI.sendControlChange(LOWCC10, (analogVal_14_bit[9] >> 7) & 0x7F, 1);

            // display
            // currentMidiValForDisplay = analogVal_14_bit[9];
            // controlChangeNumberForDisplay = 10;
            // displayUpdateType = 1;
            // updateDisplay();
        }

        pot11.update();
        if (pot11.hasChanged()) {
            analogVal_14_bit[10] = pot11.getValue() << 1;

            usbMIDI.sendControlChange(HIGHCC11, analogVal_14_bit[10] & 0x7F, 1);
            usbMIDI.sendControlChange(LOWCC11, (analogVal_14_bit[10] >> 7) & 0x7F, 1);

            // display
            // currentMidiValForDisplay = analogVal_14_bit[10];
            // controlChangeNumberForDisplay = 11;
            // displayUpdateType = 1;
            // updateDisplay();
        }

        pot12.update();
        if (pot12.hasChanged()) {
            analogVal_14_bit[11] = pot12.getValue() << 1;

            usbMIDI.sendControlChange(HIGHCC12, analogVal_14_bit[11] & 0x7F, 1);
            usbMIDI.sendControlChange(LOWCC12, (analogVal_14_bit[11] >> 7) & 0x7F, 1);

            // display
            // currentMidiValForDisplay = analogVal_14_bit[11];
            // controlChangeNumberForDisplay = 12;
            // displayUpdateType = 1;
            // updateDisplay();
        }

        pot13.update();
        if (pot13.hasChanged()) {
            analogVal_14_bit[12] = pot13.getValue() << 1;

            usbMIDI.sendControlChange(HIGHCC13, analogVal_14_bit[12] & 0x7F, 1);
            usbMIDI.sendControlChange(LOWCC13, (analogVal_14_bit[12] >> 7) & 0x7F, 1);

            // display
            // currentMidiValForDisplay = analogVal_14_bit[12];
            // controlChangeNumberForDisplay = 13;
            // displayUpdateType = 1;
            // updateDisplay();
        }

        pot14.update();
        if (pot14.hasChanged()) {
            analogVal_14_bit[13] = pot14.getValue() << 1;

            usbMIDI.sendControlChange(HIGHCC14, analogVal_14_bit[13] & 0x7F, 1);
            usbMIDI.sendControlChange(LOWCC14, (analogVal_14_bit[13] >> 7) & 0x7F, 1);

            // display
            // currentMidiValForDisplay = analogVal_14_bit[13];
            // controlChangeNumberForDisplay = 14;
            // displayUpdateType = 1;
            // updateDisplay();
        }

        pot15.update();
        if (pot15.hasChanged()) {
            analogVal_14_bit[14] = pot15.getValue() << 1;

            usbMIDI.sendControlChange(HIGHCC15, analogVal_14_bit[14] & 0x7F, 1);
            usbMIDI.sendControlChange(LOWCC15, (analogVal_14_bit[14] >> 7) & 0x7F, 1);

            // display
            // currentMidiValForDisplay = analogVal_14_bit[14];
            // controlChangeNumberForDisplay = 15;
            // displayUpdateType = 1;
            // updateDisplay();
        }

        pot16.update();
        if (pot16.hasChanged()) {
            analogVal_14_bit[15] = pot16.getValue() << 1;

            usbMIDI.sendControlChange(HIGHCC16, analogVal_14_bit[15] & 0x7F, 1);
            usbMIDI.sendControlChange(LOWCC16, (analogVal_14_bit[15] >> 7) & 0x7F, 1);

            // display
            // currentMidiValForDisplay = analogVal_14_bit[15];
            // controlChangeNumberForDisplay = 16;
            // displayUpdateType = 1;
            // updateDisplay();
        }

        pot17.update();
        if (pot17.hasChanged()) {
            analogVal_14_bit[16] = pot17.getValue() << 1;

            usbMIDI.sendControlChange(HIGHCC17, analogVal_14_bit[16] & 0x7F, 1);
            usbMIDI.sendControlChange(LOWCC17, (analogVal_14_bit[16] >> 7) & 0x7F, 1);

            // display
            // currentMidiValForDisplay = analogVal_14_bit[16];
            // controlChangeNumberForDisplay = 17;
            // displayUpdateType = 1;
            // updateDisplay();
        }

        pot18.update();
        if (pot18.hasChanged()) {
            analogVal_14_bit[17] = pot18.getValue() << 1;

            usbMIDI.sendControlChange(HIGHCC18, analogVal_14_bit[17] & 0x7F, 1);
            usbMIDI.sendControlChange(LOWCC18, (analogVal_14_bit[17] >> 7) & 0x7F, 1);

            // display
            // currentMidiValForDisplay = analogVal_14_bit[17];
            // controlChangeNumberForDisplay = 18;
            // displayUpdateType = 1;
            // updateDisplay();
        }

        pot19.update();
        if (pot19.hasChanged()) {
            analogVal_14_bit[18] = pot19.getValue() << 1;

            usbMIDI.sendControlChange(HIGHCC19, analogVal_14_bit[18] & 0x7F, 1);
            usbMIDI.sendControlChange(LOWCC19, (analogVal_14_bit[18] >> 7) & 0x7F, 1);

            // display
            // currentMidiValForDisplay = analogVal_14_bit[18];
            // controlChangeNumberForDisplay = 19;
            // displayUpdateType = 1;
            // updateDisplay();
        }

        pot20.update();
        if (pot20.hasChanged()) {
            analogVal_14_bit[19] = pot20.getValue() << 1;

            usbMIDI.sendControlChange(HIGHCC20, analogVal_14_bit[19] & 0x7F, 1);
            usbMIDI.sendControlChange(LOWCC20, (analogVal_14_bit[19] >> 7) & 0x7F, 1);

            // display
            // currentMidiValForDisplay = analogVal_14_bit[19];
            // controlChangeNumberForDisplay = 20;
            // displayUpdateType = 1;
            // updateDisplay();
        }


    }
}

void midi() {
    // handle page sellection
    for (int i = 0; i < numDigitalPinsForPageSellection; i++) {
        debouncer[i].update();
        if (debouncer[i].read() == 0) {
            if (currentSellectedPage != i) {
                currentSellectedPage = i;
                // PAGE for midi pitch value- / control change number offset
                pageDependentMidiPitchOffset = numDigitalPins*i;
                pageDependentControlChangeNumberOffset = num_analog_pins*i;

                // print to serial
                // Serial.print("currentSellectedPage: ");
                // Serial.println(currentSellectedPage);
                // update display
                displayUpdateType = 0;
                updateDisplay();
            }
        }
    }

    // handle midi
    if(msec >= 20) { // 20ms = 50 times per second .. thats enough. i.e. not bombarding receiver with midi values...
        msec = 0;

        // digital buttons - midi noteOn / noteOff
        for (int i = 0; i < numDigitalPins; i++) {
            debouncer[i+numDigitalPinsForPageSellection].update();
            if (debouncer[i+numDigitalPinsForPageSellection].read() == 0  && currentDigitalVal[i] == 1) {
                usbMIDI.sendNoteOn(
                midiPitch[i]+pageDependentMidiPitchOffset, 100, channel
                );
                currentDigitalVal[i] = 0; // flag

                myDisplay.setBrightness(2);

            }
            if (debouncer[i+numDigitalPinsForPageSellection].read() == 1 && currentDigitalVal[i] == 0) {
                usbMIDI.sendNoteOff(
                midiPitch[i]+pageDependentMidiPitchOffset, 100, channel
                );
                currentDigitalVal[i] = 1; // flag

                myDisplay.setBrightness(8);

            }
        }

        // analog knobs - midi CC
        for (int i = 0; i < num_analog_pins; i++) {

            newAnalogVal[i] = analogRead(analogPins[i]);

            if (abs(newAnalogVal[i] - currentAnalogVal[i]) > analogThreshold) {
                currentAnalogVal[i] = newAnalogVal[i];
                usbMIDI.sendControlChange(
                i+1+pageDependentControlChangeNumberOffset, currentAnalogVal[i]>>3, channel
                ); // shifted 3 bits

                // display
                currentMidiValForDisplay = currentAnalogVal[i]>>3;
                controlChangeNumberForDisplay = i+1+pageDependentControlChangeNumberOffset;
                displayUpdateType = 1;
                updateDisplay();
            }
        }
    }
}

void third() {
    ; // empty
}

void loop() {
    if (mode == MODE_DEFAULT) {
        fourteen_bit();
    } else if (mode == MODE_MIDI) {
        midi();
    } else if (mode == MODE_THIRD) {
        third();
    }
    // ignore incomming midi
    while (usbMIDI.read()); // read and discard any incoming MIDI messages

}

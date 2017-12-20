/*
antal 5V: 4 (to display)
antal 3.3V 14+20 = 34
antal gnd: 14 (to digital buttons) + 3 (to display) = 17
antal agnd: 20 (caps to analog pins) + 20 (to analog knobs) = 40
*/

#include <LedDisplay.h>
#include <Bounce2.h>

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
int displayBrightness;        // screen brightness
// start an instance of the LED display library:
LedDisplay myDisplay = LedDisplay(dataPin, registerSelect, clockPin,
enable, reset, displayLength);

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

//CC analog pins:
int const numAnalogPins = 20;
int currentAnalogVal[numAnalogPins];
int newAnalogVal[numAnalogPins];
int analogThreshold = 3; // filtrering (if non = 1)
int analogPins[] = { // A12 og A13 er vent om på hardware =?!!????
    A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A13,A12,A14,A16,A17,A18,A19,A20
};

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


/*
// page leds -> digital pins:
int const numDigitalPinsForPageSellectionLedFeedback = 4;
int digitalPinsForPageSellectionLedFeedback[] = {
    24,25,26,32,33
};
*/

// misc
elapsedMillis msec = 0;
//elapsedMillis msecSecondFunc = 0;
//hvis alle fire page knapper har været holdt inde i 2 sekunder så . nam nam
const int channel = 1;

// boot from pageA / page0
int currentSellectedPage = 0;
int pageDependentMidiPitchOffset = 0;
int pageDependentControlChangeNumberOffset = 0;
int midiPitch[] = {
    36,37,38,39,40,41,42,43,44,45
};
bool p_sel[numDigitalPinsForPageSellection] = {FALSE};

void setup() {
   /*  ~~~ ~~~ ~~~ init config BEGIN ~~~ ~~~ ~~~
       ~~~ ~~~ ~~~ ~~~ ~~~   ~~~ ~~~ ~~~ ~~~ ~~~   */
    // Serial.begin(19200);

    // Pullup for button pins
    for (int i = 0; i < numDigitalPins; i++) {
        pinMode (digitalPins[i], INPUT_PULLUP);
    }
    // Pullup for page sellect pins
    for (int i = 0; i < numDigitalPinsForPageSellection; i++) {
        pinMode (digitalPinsForPageSellection[i], INPUT_PULLUP);
    }

    // debounce setup
    for (int i = 0; i < (numDigitalPinsForPageSellection + numDigitalPins); ++i) {
        if (i < numDigitalPinsForPageSellection) {
            debouncer[i].attach(digitalPinsForPageSellection[i]);
            debouncer[i].interval(5);
        }
        if (i >= numDigitalPinsForPageSellection && i < (numDigitalPinsForPageSellection + numDigitalPins))
        debouncer[i].attach(digitalPinsForPageSellection[i]);
        debouncer[i].interval(5);
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

    // if all is pressed, load/goto secondary modus
    if (p_sel[0] == TRUE && p_sel[1] == TRUE && p_sel[2] == TRUE && p_sel[3] == TRUE) {
        myDisplay.clear();
        myDisplay.setCursor(1);
        myDisplay.print("second");
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
        // clear display
        myDisplay.clear();
        delay(500);
    }
    


    // setup led display start_up
    myDisplay.clear();
    myDisplay.setCursor(2);
    myDisplay.print("hello");
    for (int i = 0; i < 5; i++) {
        //fade up
        for (displayBrightness = 0; displayBrightness < 16; displayBrightness++) {
            myDisplay.setBrightness(displayBrightness);
            delay(30);
        }
        delay(50);
        // fade down
        for (displayBrightness = 15; displayBrightness >= 0; displayBrightness--) {
            myDisplay.setBrightness(displayBrightness);
            delay(30);
        }
    }
    // clear display
    myDisplay.clear();
    delay(500);
    displayBrightness = 10; // eats values 0-15
    myDisplay.setBrightness(displayBrightness);

    // set curser to 0
    myDisplay.home();
    myDisplay.print("im ready");
    delay(1000);

    // clear display
    myDisplay.clear();
    delay(100);
    // display feedback for initial page number
    myDisplay.home();
    myDisplay.print(pageNumberToCharTranslation[currentSellectedPage]);
}

void clearDisplay() {
    if (displayClearDone == false) {
        myDisplay.clear();
        displayClearDone = true;
        // Serial.println("Display was CLEARED !!!!!!!");
    }
}

void makeDisplayBright() {
    myDisplay.setBrightness(15);
    // Serial.println("made briiight !!!!!!!!!");
}

void updateDisplay() {
    // page update
    if (displayUpdateType == 0) {
        myDisplay.home(); // cursor position 0
        myDisplay.print(pageNumberToCharTranslation[currentSellectedPage]);
        // Serial.println("page on Display was updated");
    }
    // cc update
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
}

void loop() {

    // handle page sellection
    for (int i = 0; i < numDigitalPinsForPageSellection; i++) {
        debouncer[i].update();
        if (debouncer[i].read() == 0) {
            if (currentSellectedPage != i) {
                currentSellectedPage = i;
                // PAGE for midi pitch value- / control change number offset
                pageDependentMidiPitchOffset = numDigitalPins*i;
                pageDependentControlChangeNumberOffset = numAnalogPins*i;

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
            debouncer[i].update();
            if (debouncer[i+numDigitalPinsForPageSellection].read() == 0  && currentDigitalVal[i] == 1) {
                usbMIDI.sendNoteOn(
                    midiPitch[i]+pageDependentMidiPitchOffset, 100, channel
                );
                currentDigitalVal[i] = 0; // flag

                myDisplay.setBrightness(2);

                // Serial.print("button on digitalpin: ");
                // Serial.print(digitalPins[i]);
                // Serial.print(" pressed - midi noteOn: ");
                // Serial.println(midiPitch[i]+pageDependentMidiPitchOffset);
            }
            if (debouncer[i+numDigitalPinsForPageSellection].read() == 1 && currentDigitalVal[i] == 0) {
                usbMIDI.sendNoteOff(
                    midiPitch[i]+pageDependentMidiPitchOffset, 100, channel
                );
                currentDigitalVal[i] = 1; // flag

                myDisplay.setBrightness(8);

                // Serial.print("button on digitalpin: ");
                // Serial.print(digitalPins[i]);
                // Serial.print(" released - midi noteOff: ");
                // Serial.println(midiPitch[i]+pageDependentMidiPitchOffset);
            }
        }

        // analog knobs - midi CC
        for (int i = 0; i < numAnalogPins; i++) {

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

                // Serial.print("knob on analogpin: ");
                // Serial.print(analogPins[i]);
                // Serial.print(" full range _: ");
                // Serial.print(currentAnalogVal[i]);
                // Serial.print(" midi val _: ");
                // Serial.println(currentAnalogVal[i]>>3);
                // Serial.print(" CCnumber _: ");
                // Serial.println(i+1+pageDependentControlChangeNumberOffset);
            }
        }
    }

// ignore incomming midi
while (usbMIDI.read()); // read and discard any incoming MIDI messages
}

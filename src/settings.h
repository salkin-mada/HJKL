#ifndef HAKKESETTINGS
#define HAKKESETTINGS
#pragma once

#include "globals.h"
/* #include "midi.h" */

namespace hjkl {

/*
 * @TODO:
 * - Should all state be put in here, including oled and data?
 *
 */

constexpr int maxMidiVal14Bit = 16383;
constexpr int maxMidiVal7Bit = 127;

constexpr int large14bitval = maxMidiVal14Bit / 128;
constexpr int huge14bitval = maxMidiVal14Bit / 20;

/**
 * @brief Settings object for HJKL
 * @author Niklas Adam
 * @date 02-05-2021
 * @description ..
 */
struct hjkl_state {

  enum class IncrementType { Small, Large, Huge };
  IncrementType incrementType;

  int maxValue;

  int largeIncrement, smallIncrement, hugeIncrement;
  int incrementSize;

  void setup(int numberLayers, int numberPages, int numberEncoders,
             int numButtons, IncrementType incType, MIDIMODE midimode);

  void setMidiMode(MIDIMODE midimode) {
    mode = midimode;

    if (midimode == MIDIMODE::_7BIT) {
      maxValue = maxMidiVal7Bit;

      smallIncrement = 1;
      largeIncrement = 2;
      hugeIncrement = maxMidiVal7Bit / 20;
    } else if (midimode == MIDIMODE::_14BIT) {

      maxValue = maxMidiVal14Bit;
      smallIncrement = 1;
      largeIncrement = large14bitval;
      hugeIncrement = huge14bitval;
    }

    // Update current increment size
    setIncrement(incrementType);
  }

  // Set current increment size
  void setIncrement(IncrementType incType) {
    incrementType = incType;

    switch (incType) {
    case IncrementType::Small:
      incrementSize = smallIncrement;
      break;
    case IncrementType::Large:
      incrementSize = largeIncrement;
      break;
    case IncrementType::Huge:
      incrementSize = hugeIncrement;
      break;
    }
  };

  int currentLayer = 0;
  int currentPage = 0;

  int numLayers;
  int numPages;
  int numEncoders;
  int numButtons;

  // Midi processing mode
  MIDIMODE mode;

  bool wrapMode = false;

  void print();

  /* void progressTimer() { time = millis(); } */
  /* private: */
  /*   int time; */

private:
};
} // namespace hakke

#endif

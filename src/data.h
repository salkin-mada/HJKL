#ifndef DATA
#define DATA
#pragma once

#include <array>

namespace hjkl {

/**
 * @brief data structure hjkl
 * @author Niklas Adam
 * @date 01-05-2021
 * @decription ...
 */
template <int numLayers, int numPages, int numValuesPerPage> class Data {

public:
  int globalButtonFunctionality;
  int lastTouchedKnob;

private:
  std::array<std::array<std::array<int, numValuesPerPage>, numPages>, numLayers>
      midiLayers;

public:
  Data() { reset(); }

  void reset(int value = 0) {
    for (auto layer : midiLayers) {
      for (auto page : layer) {
        page.fill(value);
      }
    }
  }

  // Sets the value for encoder num in the layer and page put in.
  void setValue(int layer, int page, int encoderNum, int value) {
    // Set value in layer
    midiLayers[layer][page][encoderNum] = value;
  };

  int getValue(int layer, int page, int encoderNum) {
    return midiLayers[layer][page][encoderNum];
  }

  void printValues() {
    for (int layNum = 0; layNum < midiLayers.size(); layNum++) {

      Serial.print("Layer:");
      Serial.println(layNum);

      for (int pageNum = 0; pageNum < midiLayers[layNum].size(); pageNum++) {
        Serial.print("\tPage: ");
        Serial.println(pageNum);

        for (int encNum = 0; encNum < midiLayers[layNum][pageNum].size();
             encNum++) {
          const auto val = getValue(layNum, pageNum, encNum);
          Serial.print("\t\tEnc: ");
          Serial.print(encNum);
          Serial.print(", ");
          Serial.println(val);
        }
      }
    }
  }
};

} // namespace hakke

#endif

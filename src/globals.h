#ifndef GLOBALS
#define GLOBALS
#pragma once

#include <Arduino.h>

namespace hjkl {
enum class MIDIMODE { _7BIT, _14BIT };

int channel = 1;

int pitchPage = 4; // init button midi note pitches 40-50

constexpr int num_buttons = 14;
constexpr int num_special_buttons = 4;

inline int calculate_midi_note(int button_num, int num_buttons, int pitch_page_num) {
    /* auto midi_note = button_num + (pitch_page_num * num_buttons); */
    auto midi_note = button_num + (pitch_page_num * num_buttons);
    return midi_note;
}

inline String gen_random(const int len) {

	String tmp_s;
	static const char alphanum[] =
        "(/\\)";
		/* "0123456789" */
		/* "ABCDEFGHIJKLMNOPQRSTUVWXYZ" */
		/* "abcdefghijklmnopqrstuvwxyz"; */

	for (auto i = 0; i < len; ++i) 
		tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];


	return tmp_s;
}

template <typename T> T clipValue(T value, T minValue, T maxValue) {
    if (value > maxValue) {
        return maxValue;
    } else if (value < minValue) {
        return minValue;
    } else {
        return value;
    }
}

template <typename T> T wrapValue(T value, T minValue, T maxValue) {
    T range_size = maxValue - minValue + 1;
    if (value < minValue) {
        value += range_size * ((minValue - value) / range_size + 1);
    }
    return minValue + (value - minValue) % range_size;
}

} // namespace hjkl
#endif

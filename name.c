#include "usb_names.h"

#define MIDI_NAME   {'S','O','V','J','E','T','I'}
#define MIDI_NAME_LEN  7

struct usb_string_descriptor_struct usb_string_product_name = {
	2 + MIDI_NAME_LEN * 2,
	3,
	MIDI_NAME
};

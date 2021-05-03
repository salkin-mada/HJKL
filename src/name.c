#include "usb_names.h"

#define MIDI_NAME   {'H','J','K','L'}
#define MIDI_NAME_LEN  4

struct usb_string_descriptor_struct usb_string_product_name = {
	2 + MIDI_NAME_LEN * 2,
	3,
	MIDI_NAME
};

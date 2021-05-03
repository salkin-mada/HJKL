#include "ValueHolder.h"
#include <Arduino.h>
#include <EEPROM.h>

const uint16_t ee_bytes = 2048; // cycling through bytes to prolong life
const uint8_t ee_address = 0;

// timeguard for writing
unsigned long previous_write_time = 0;
const long write_time_interval = 1000;

struct Values {
	uint16_t saved_values_page_a[num_analog_pins];
	uint16_t saved_values_page_b[num_analog_pins];
	uint16_t saved_values_page_c[num_analog_pins];
	uint16_t saved_values_page_d[num_analog_pins];
};

void store_values() {
	unsigned long current_write_time = millis();

	ValueHolder list = {
		analogVal_14_bit[0],
		analogVal_14_bit[1],
		analogVal_14_bit[2],
		analogVal_14_bit[3],
		analogVal_14_bit[4],
		analogVal_14_bit[5],
		analogVal_14_bit[6],
		analogVal_14_bit[7],
		analogVal_14_bit[8],
		analogVal_14_bit[9],
		analogVal_14_bit[10],
		analogVal_14_bit[11],
		analogVal_14_bit[12],
		analogVal_14_bit[13],
		analogVal_14_bit[14],
		analogVal_14_bit[15],
		analogVal_14_bit[16],
		analogVal_14_bit[17],
		analogVal_14_bit[18],
		analogVal_14_bit[19],
		selected_page
	};

	if (current_write_time - previous_write_time > write_time_interval)
	{
		//Serial.println("saving UIdata if something changed");

		EEPROM.put(ee_address, list);

		previous_write_time = current_write_time;
	}
};

void recall_values()
{
	ValueHolder list;
	EEPROM.get(ee_address, list);

	/* Serial.println("Read data from EEPROM: "); */
	for (auto i = 0; i < num_pages; ++i) 
		for (auto j = 0; j < num_analog_pins; ++j) 
			; // interate on multi dim array (pages - values [i][j])
};

void clearEEPROM()
{
	for (unsigned int i = 0; i < EEPROM.length(); i++)
		EEPROM.write(i, 0);
}

/* only works with max 120MHz teensy 3.6
   void WRITE2EEPROM(unsigned int i) { //only write if it is different
   if (doStep[i] != EEPROM.read(i)) {
   EEPROM.write(i, doStep[i]);
   }
   } */

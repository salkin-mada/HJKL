#ifndef VALUEHOLDER_H
#define VALUEHOLDER_H

extern int analogVal_14_bit[0];
extern int selected_page;
extern int num_pages;
extern int num_analog_pins;

void store_values();
void recall_values();
void clear_EEPROM();

#endif

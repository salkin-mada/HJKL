* add these lines
```python
    "USB_KEYBOARD_MIDI",
    "USB_KEYBOARD_MIDI_SERIAL",
```
to the BUILTIN_USB_FLAGS list in `/home/$USER/.platformio/platforms/teensy/builder/frameworks/arduino.py`

* and add the following two `elif`s to `/home/$USER/.platformio/packages/framework-arduinoteensy/cores/teensy3/usb_desc.h`
```h
#elif defined(USB_KEYBOARD_MIDI)
  #define VENDOR_ID             0x16C0
  #define PRODUCT_ID            0x0485
  #define MANUFACTURER_NAME     {'T','e','e','n','s','y','d','u','i','n','o'}
  #define MANUFACTURER_NAME_LEN 11
  #define PRODUCT_NAME          {'S','a','t','a','n'}
  #define PRODUCT_NAME_LEN      5
  #define EP0_SIZE             64
  #define NUM_ENDPOINTS         6
  #define NUM_USB_BUFFERS      28
  #define NUM_INTERFACE         4
  #define SEREMU_INTERFACE      1  // Serial emulation
  #define SEREMU_TX_ENDPOINT    1
  #define SEREMU_TX_SIZE       64
  #define SEREMU_TX_INTERVAL    1
  #define SEREMU_RX_ENDPOINT    2
  #define SEREMU_RX_SIZE       32
  #define SEREMU_RX_INTERVAL    2
  #define MIDI_INTERFACE        2  // MIDI
  #define MIDI_NUM_CABLES       1
  #define MIDI_TX_ENDPOINT      5
  #define MIDI_TX_SIZE         64
  #define MIDI_RX_ENDPOINT      6
  #define MIDI_RX_SIZE         64
  #define KEYBOARD_INTERFACE    3  // Keyboard
  #define KEYBOARD_ENDPOINT     3
  #define KEYBOARD_SIZE         8
  #define KEYBOARD_INTERVAL     1
  #define KEYMEDIA_INTERFACE    4  // Keyboard Media Keys
  #define KEYMEDIA_ENDPOINT     4
  #define KEYMEDIA_SIZE         8
  #define KEYMEDIA_INTERVAL     4
  #define ENDPOINT1_CONFIG  ENDPOINT_TRANSMIT_ONLY
  #define ENDPOINT2_CONFIG  ENDPOINT_RECEIVE_ONLY
  #define ENDPOINT3_CONFIG  ENDPOINT_TRANSMIT_ONLY
  #define ENDPOINT4_CONFIG  ENDPOINT_TRANSMIT_ONLY
  #define ENDPOINT5_CONFIG  ENDPOINT_TRANSMIT_ONLY
  #define ENDPOINT6_CONFIG  ENDPOINT_RECEIVE_ONLY

#elif defined(USB_KEYBOARD_MIDI_SERIAL)
  #define VENDOR_ID             0x16C0
  #define PRODUCT_ID            0x0485
  #define MANUFACTURER_NAME     {'T','e','e','n','s','y','d','u','i','n','o'}
  #define MANUFACTURER_NAME_LEN 11
  #define PRODUCT_NAME          {'S','a','t','a','n'}
  #define PRODUCT_NAME_LEN      5
  #define EP0_SIZE             64
  #define NUM_ENDPOINTS         7
  #define NUM_USB_BUFFERS      30
  #define NUM_INTERFACE         4
  #define CDC_IAD_DESCRIPTOR    1
  #define CDC_STATUS_INTERFACE  0
  #define CDC_DATA_INTERFACE    1  // Serial
  #define CDC_ACM_ENDPOINT      1
  #define CDC_RX_ENDPOINT       2
  #define CDC_TX_ENDPOINT       3
  #define CDC_ACM_SIZE         16
  #define CDC_RX_SIZE          64
  #define CDC_TX_SIZE          64
  #define MIDI_INTERFACE        2  // MIDI
  #define MIDI_NUM_CABLES       1
  #define MIDI_TX_ENDPOINT      6
  #define MIDI_TX_SIZE         64
  #define MIDI_RX_ENDPOINT      7
  #define MIDI_RX_SIZE         64
  #define KEYBOARD_INTERFACE    3  // Keyboard
  #define KEYBOARD_ENDPOINT     4
  #define KEYBOARD_SIZE         8
  #define KEYBOARD_INTERVAL     1
  #define KEYMEDIA_INTERFACE    4  // Keyboard Media Keys
  #define KEYMEDIA_ENDPOINT     5
  #define KEYMEDIA_SIZE         8
  #define KEYMEDIA_INTERVAL     4
  #define ENDPOINT1_CONFIG  ENDPOINT_TRANSMIT_ONLY
  #define ENDPOINT2_CONFIG  ENDPOINT_RECEIVE_ONLY
  #define ENDPOINT3_CONFIG  ENDPOINT_TRANSMIT_ONLY
  #define ENDPOINT4_CONFIG  ENDPOINT_TRANSMIT_ONLY
  #define ENDPOINT5_CONFIG  ENDPOINT_TRANSMIT_ONLY
  #define ENDPOINT6_CONFIG  ENDPOINT_TRANSMIT_ONLY
  #define ENDPOINT7_CONFIG  ENDPOINT_RECEIVE_ONLY
```

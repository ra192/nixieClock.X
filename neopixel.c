#include "neopixel.h"
#include "mcc_generated_files/pin_manager.h"

#define send(b) LED_LAT=1; LED_LAT=b; NOP(); LED_LAT=0; NOP(); //3 4 4

// send out a byte b in WS2812 protocol
void sendByte (unsigned char b) {

    if (b & 0b10000000) { send(1); } else { send(0); }
    if (b & 0b01000000) { send(1); } else { send(0); }
    if (b & 0b00100000) { send(1); } else { send(0); }
    if (b & 0b00010000) { send(1); } else { send(0); }
    if (b & 0b00001000) { send(1); } else { send(0); }
    if (b & 0b00000100) { send(1); } else { send(0); }
    if (b & 0b00000010) { send(1); } else { send(0); }
    if (b & 0b00000001) { send(1); } else { send(0); }  
}

// send red, green, and blue values in WS2812 protocol
void sendRGB (unsigned char r, unsigned char g, unsigned char b) {

    sendByte(g);
    sendByte(r);
    sendByte(b);
}

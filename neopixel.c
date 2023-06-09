#include "neopixel.h"
#include "mcc_generated_files/pin_manager.h"

#define send(b) LED_LAT=1; LED_LAT=b; NOP(); LED_LAT=0; NOP(); //3 4 4

typedef struct Colour {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} Colour;

Colour OFF_COLOUR = {0, 0, 0};

const uint8_t lights[360] = {
    0, 0, 0, 0, 0, 1, 1, 2,
    2, 3, 4, 5, 6, 7, 8, 9,
    11, 12, 13, 15, 17, 18, 20, 22,
    24, 26, 28, 30, 32, 35, 37, 39,
    42, 44, 47, 49, 52, 55, 58, 60,
    63, 66, 69, 72, 75, 78, 81, 85,
    88, 91, 94, 97, 101, 104, 107, 111,
    114, 117, 121, 124, 127, 131, 134, 137,
    141, 144, 147, 150, 154, 157, 160, 163,
    167, 170, 173, 176, 179, 182, 185, 188,
    191, 194, 197, 200, 202, 205, 208, 210,
    213, 215, 217, 220, 222, 224, 226, 229,
    231, 232, 234, 236, 238, 239, 241, 242,
    244, 245, 246, 248, 249, 250, 251, 251,
    252, 253, 253, 254, 254, 255, 255, 255,
    255, 255, 255, 255, 254, 254, 253, 253,
    252, 251, 251, 250, 249, 248, 246, 245,
    244, 242, 241, 239, 238, 236, 234, 232,
    231, 229, 226, 224, 222, 220, 217, 215,
    213, 210, 208, 205, 202, 200, 197, 194,
    191, 188, 185, 182, 179, 176, 173, 170,
    167, 163, 160, 157, 154, 150, 147, 144,
    141, 137, 134, 131, 127, 124, 121, 117,
    114, 111, 107, 104, 101, 97, 94, 91,
    88, 85, 81, 78, 75, 72, 69, 66,
    63, 60, 58, 55, 52, 49, 47, 44,
    42, 39, 37, 35, 32, 30, 28, 26,
    24, 22, 20, 18, 17, 15, 13, 12,
    11, 9, 8, 7, 6, 5, 4, 3,
    2, 2, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

// send out a byte b in WS2812 protocol

void sendByte(unsigned char b) {

    if (b & 0b10000000) {
        send(1);
    } else {
        send(0);
    }
    if (b & 0b01000000) {
        send(1);
    } else {
        send(0);
    }
    if (b & 0b00100000) {
        send(1);
    } else {
        send(0);
    }
    if (b & 0b00010000) {
        send(1);
    } else {
        send(0);
    }
    if (b & 0b00001000) {
        send(1);
    } else {
        send(0);
    }
    if (b & 0b00000100) {
        send(1);
    } else {
        send(0);
    }
    if (b & 0b00000010) {
        send(1);
    } else {
        send(0);
    }
    if (b & 0b00000001) {
        send(1);
    } else {
        send(0);
    }
}

// send red, green, and blue values in WS2812 protocol

void sendRGB(Colour* colour) {
    sendByte(colour->green);
    sendByte(colour->red);
    sendByte(colour->blue);
}

void set_leds_colour(Colour* colour) {
    sendRGB(colour);
    sendRGB(colour);
    sendRGB(colour);
    sendRGB(colour);
}

void led_off(void) {
    set_leds_colour(&OFF_COLOUR);
}

// sine wave rainbow. See https://www.instructables.com/How-to-Make-Proper-Rainbow-and-Random-Colors-With-/

void set_leds_colour_by_angle(uint16_t angle) {
    Colour colour = {lights[(angle + 120) % 360], lights[angle], lights[(angle + 240) % 360]};
    set_leds_colour(&colour);
}
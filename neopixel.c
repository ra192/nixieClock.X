#include "neopixel.h"
#include "mcc_generated_files/pin_manager.h"

#define send(b) LED_LAT=1; NOP(); LED_LAT=b; NOP(); NOP(); LED_LAT=0; NOP(); NOP(); //3 4 4

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

// https://www.vagrearg.org/content/hsvrgb

void fast_hsv2rgb_8bit(uint16_t h, uint8_t s, uint8_t v, uint8_t *r, uint8_t *g, uint8_t *b) {
    HSV_MONOCHROMATIC_TEST(s, v, r, g, b); // Exit with grayscale if s == 0

    uint8_t sextant = h >> 8;

    HSV_SEXTANT_TEST(sextant); // Optional: Limit hue sextants to defined space

    HSV_POINTER_SWAP(sextant, r, g, b); // Swap pointers depending which sextant we are in

    *g = v; // Top level

    // Perform actual calculations
    uint8_t bb;
    uint16_t ww;

    /*
     * Bottom level: v * (1.0 - s)
     * --> (v * (255 - s) + error_corr) / 256
     */
    bb = ~s;
    ww = v * bb;
    ww += 1; // Error correction
    ww += ww >> 8; // Error correction
    *b = ww >> 8;

    uint8_t h_fraction = h & 0xff; // 0...255

    if (!(sextant & 1)) {
        // *r = ...slope_up...;
        /*
         * Slope up: v * (1.0 - s * (1.0 - h))
         * --> (v * (255 - (s * (256 - h) + error_corr1) / 256) + error_corr2) / 256
         */
        ww = !h_fraction ? ((uint16_t) s << 8) : (s * (uint8_t) (-h_fraction));
        ww += ww >> 8; // Error correction 1
        bb = ww >> 8;
        bb = ~bb;
        ww = v * bb;
        ww += v >> 1; // Error correction 2
        *r = ww >> 8;
    } else {
        // *r = ...slope_down...;
        /*
         * Slope down: v * (1.0 - s * h)
         * --> (v * (255 - (s * h + error_corr1) / 256) + error_corr2) / 256
         */
        ww = s * h_fraction;
        ww += ww >> 8; // Error correction 1
        bb = ww >> 8;
        bb = ~bb;
        ww = v * bb;
        ww += v >> 1; // Error correction 2
        *r = ww >> 8;

        /*
         * A perfect match for h_fraction == 0 implies:
         *	*r = (ww >> 8) + (h_fraction ? 0 : 1)
         * However, this is an extra calculation that may not be required.
         */
    }
}


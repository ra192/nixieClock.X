/* 
 * File:   neopixel.h
 * Author: yak19
 *
 * Created on May 15, 2023, 3:51 PM
 */

#ifndef NEOPIXEL_H
#define	NEOPIXEL_H

#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct Colour {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    } Colour;

    void set_leds_colour(Colour* colour);

    void set_leds_colour_by_angle_1(uint16_t angle);
    
    void set_leds_colour_by_angle_2(uint16_t angle);

#ifdef	__cplusplus
}
#endif

#endif	/* NEOPIXEL_H */


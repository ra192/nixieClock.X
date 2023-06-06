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
    void led_off(void);
    void set_leds_colour_by_angle(uint16_t angle);

#ifdef	__cplusplus
}
#endif

#endif	/* NEOPIXEL_H */


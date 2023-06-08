/* 
 * File:   nixie.h
 * Author: yak19
 *
 * Created on May 7, 2023, 5:15 AM
 */

#ifndef NIXIE_H
#define	NIXIE_H

#include "mcc_generated_files/pin_manager.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define DIGITS_SIZE 4       

    void set_digits(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3);

    void set_digit_displayed(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3);

    void set_digit_displayed_all(void);

    void toggle_digit_displayed(uint8_t number);

    void refresh_digits(void);

    void off_digits(void);

    void flip_all(uint16_t timer_count);

    void flip_seq(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3, uint8_t dig4, uint8_t dig5, uint16_t timer_count);
    
    void shift(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3, uint8_t dig4, uint8_t dig5, uint16_t timer_count);

#ifdef	__cplusplus
}
#endif

#endif	/* NIXIE_H */


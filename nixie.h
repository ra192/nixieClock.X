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

    void refresh_digits(void);

    void toggle(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3, uint8_t dig0_toggled, uint8_t dig1_toggled, uint8_t dig2_toggled, uint8_t dig3_toggled);
    
    void flip_all(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3);

    void flip_seq(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3, uint8_t dig4, uint8_t dig5, uint8_t dig6, uint8_t dig7);
    
    void shift(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3, uint8_t dig4, uint8_t dig5, uint8_t dig6, uint8_t dig7);

#ifdef	__cplusplus
}
#endif

#endif	/* NIXIE_H */


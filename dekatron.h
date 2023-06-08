/* 
 * File:   dekatron.h
 * Author: yak19
 *
 * Created on May 23, 2023, 6:22 AM
 */

#ifndef DEKATRON_H
#define	DEKATRON_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>

    typedef enum DekMode {
        DISPLAY_VAL,
        SPIN_CW,
        SPIN_CCW,
        FILL        
    } DekMode;

    void dek_set_val(uint8_t disp_val);
    
    void dek_set_mode(DekMode mod);

    void refresh_dek(void);
    
    

#ifdef	__cplusplus
}
#endif

#endif	/* DEKATRON_H */


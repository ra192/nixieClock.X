/* 
 * File:   buzzer.h
 * Author: yak19
 *
 * Created on May 25, 2023, 10:26 PM
 */

#ifndef BUZZER_H
#define	BUZZER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>

    void buzzer_off(void);

    uint8_t buzzer_get_on(void);

    void start_melody(uint8_t melody_index);

    void refresh_buzzer(void);


#ifdef	__cplusplus
}
#endif

#endif	/* BUZZER_H */


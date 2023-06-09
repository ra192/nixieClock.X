/* 
 * File:   button.h
 * Author: yak19
 *
 * Created on May 7, 2023, 4:35 PM
 */

#ifndef BUTTON_H
#define	BUTTON_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include "settings.h"    

#define BTN_PRESS_TIME 0.01
#define BTN_LONG_PRESS_TIME 0.5    

#define BTN_PRESS_COUNT (TICKS_FREQ * BTN_PRESS_TIME)
#define BTN_LONG_PRESS_COUNT (TICKS_FREQ * BTN_LONG_PRESS_TIME)    
    
typedef enum Btn_state {
    RELEASED,
    PRESSED,
    HOLD_PRESSED,        
    LONG_PRESSED,
    HOLD_LONG_PRESSED        
            
} Btn_state;

typedef struct Button {
    Btn_state state;
    uint16_t pressed_tick_count;
} Button;

void read_button(Button* btn, uint8_t pin_val);

#ifdef	__cplusplus
}
#endif

#endif	/* BUTTON_H */


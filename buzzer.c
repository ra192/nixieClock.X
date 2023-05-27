#include <stdio.h>

#include "buzzer.h"
#include "mcc_generated_files/pwm4.h"
#include "mcc_generated_files/tmr4.h"

#define B7_NOTE_TMR_VAL 62
#define A7_NOTE_TMR_VAL 70
#define G7_NOTE_TMR_VAL 79
#define F7_NOTE_TMR_VAL 88
#define E7_NOTE_TMR_VAL 94
#define D7_NOTE_TMR_VAL 105
#define C7_NOTE_TMR_VAL 118
#define B6_NOTE_TMR_VAL 125
#define A6_NOTE_TMR_VAL 141
#define G6_NOTE_TMR_VAL 158
#define F6_NOTE_TMR_VAL 178
#define E6_NOTE_TMR_VAL 188
#define D6_NOTE_TMR_VAL 211
#define C6_NOTE_TMR_VAL 238

#define PAUSE 0x00

#define NOTE_TICKS 100

#define PWM_DUTY_VAL 213

uint8_t buzzer_is_on = 0;
uint8_t buzzer_ticks;

//uint8_t notes_arr[] = {C6_NOTE_TMR_VAL, PAUSE, D6_NOTE_TMR_VAL, PAUSE, E6_NOTE_TMR_VAL,
//    PAUSE, F6_NOTE_TMR_VAL, PAUSE, G6_NOTE_TMR_VAL, PAUSE, A6_NOTE_TMR_VAL,
//    PAUSE, B6_NOTE_TMR_VAL, PAUSE, C7_NOTE_TMR_VAL, PAUSE, D7_NOTE_TMR_VAL, PAUSE,
//    E7_NOTE_TMR_VAL, PAUSE, F7_NOTE_TMR_VAL, PAUSE, G7_NOTE_TMR_VAL, A7_NOTE_TMR_VAL, PAUSE, B7_NOTE_TMR_VAL};
//uint8_t notes_size = 27;

uint8_t notes_arr[] = { A7_NOTE_TMR_VAL, PAUSE, A7_NOTE_TMR_VAL, PAUSE, B7_NOTE_TMR_VAL};
uint8_t notes_size = 5;

uint8_t current_notes_ind;

void buzzer_off(void) {
    PWM4_LoadDutyValue(0);
    buzzer_is_on = 0;
}

void buzzer_on(uint8_t note_tmr_val, uint8_t ticks) {
    if (note_tmr_val == PAUSE) {
        PWM4_LoadDutyValue(0);
    } else {
        TMR4_LoadPeriodRegister(note_tmr_val);
        PWM4_LoadDutyValue(PWM_DUTY_VAL);
    }
    buzzer_ticks = ticks;
    buzzer_is_on = 1;
}

void start_melody(void) {
    buzzer_on(notes_arr[0], NOTE_TICKS);
    current_notes_ind = 0;
}

void refresh_buzzer(void) {
    if (buzzer_is_on) {
        if (buzzer_ticks > 0)
            buzzer_ticks--;
        else if (current_notes_ind < notes_size - 1) {
            current_notes_ind++;
            buzzer_on(notes_arr[current_notes_ind], NOTE_TICKS);
        } else {
            buzzer_off();
        }
    }
}
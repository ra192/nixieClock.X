#include <stdio.h>

#include "buzzer.h"
#include "mcc_generated_files/pwm4.h"
#include "mcc_generated_files/tmr4.h"

#define B7_NOTE_TMR_VAL 62
#define AS7_NOTE_TMR_VAL 66
#define A7_NOTE_TMR_VAL 70
#define GS7_NOTE_TMR_VAL 74
#define G7_NOTE_TMR_VAL 79
#define FS7_NOTE_TMR_VAL 83
#define F7_NOTE_TMR_VAL 88
#define E7_NOTE_TMR_VAL 94
#define DS7_NOTE_TMR_VAL 99
#define D7_NOTE_TMR_VAL 105
#define CS7_NOTE_TMR_VAL 112
#define C7_NOTE_TMR_VAL 118
#define B6_NOTE_TMR_VAL 125
#define AS6_NOTE_TMR_VAL 133
#define A6_NOTE_TMR_VAL 141
#define GS6_NOTE_TMR_VAL 149
#define G6_NOTE_TMR_VAL 158
#define FS6_NOTE_TMR_VAL 168
#define F6_NOTE_TMR_VAL 178
#define E6_NOTE_TMR_VAL 188
#define DS6_NOTE_TMR_VAL 200
#define D6_NOTE_TMR_VAL 211
#define CS6_NOTE_TMR_VAL 224
#define C6_NOTE_TMR_VAL 238

#define PAUSE 0x00

#define NOTE_LENGTH_1_8 50
#define NOTE_LENGTH_1_4 100
#define NOTE_LENGTH_1_2 200
#define NOTE_LENGTH_1 400

#define PWM_DUTY_VAL 213

#define MELODY_REPEATS 3

typedef struct Note {
    uint8_t note;
    uint16_t length;
} Note;

uint8_t buzzer_is_on = 0;
uint16_t buzzer_ticks;


Note notes_arr[] = {
    {E7_NOTE_TMR_VAL, NOTE_LENGTH_1_8},
    {D7_NOTE_TMR_VAL, NOTE_LENGTH_1_8},
    {FS6_NOTE_TMR_VAL, NOTE_LENGTH_1_4},
    {GS6_NOTE_TMR_VAL, NOTE_LENGTH_1_4},

    {CS7_NOTE_TMR_VAL, NOTE_LENGTH_1_8},
    {B6_NOTE_TMR_VAL, NOTE_LENGTH_1_8},
    {D6_NOTE_TMR_VAL, NOTE_LENGTH_1_4},
    {E6_NOTE_TMR_VAL, NOTE_LENGTH_1_4},

    {B6_NOTE_TMR_VAL, NOTE_LENGTH_1_8},
    {A6_NOTE_TMR_VAL, NOTE_LENGTH_1_8},
    {CS6_NOTE_TMR_VAL, NOTE_LENGTH_1_4},
    {E6_NOTE_TMR_VAL, NOTE_LENGTH_1_4},

    {A6_NOTE_TMR_VAL, NOTE_LENGTH_1_2},

    {PAUSE, NOTE_LENGTH_1}
};
uint8_t notes_size = 14;

uint8_t current_notes_ind;
uint8_t melody_repeat_count;

void buzzer_off(void) {
    PWM4_LoadDutyValue(0);
    buzzer_is_on = 0;
}

void buzzer_on(uint8_t note_tmr_val, uint16_t ticks) {
    if (note_tmr_val == PAUSE) {
        PWM4_LoadDutyValue(0);
    } else {
        TMR4_LoadPeriodRegister(note_tmr_val);
        PWM4_LoadDutyValue(PWM_DUTY_VAL);
    }
    buzzer_ticks = ticks;
    buzzer_is_on = 1;
}

uint8_t buzzer_get_on(void) {
    return buzzer_is_on;
}

void start_melody(void) {
    buzzer_on(notes_arr[0].note, notes_arr[0].length);
    current_notes_ind = 0;
    melody_repeat_count = 0;
}

void refresh_buzzer(void) {
    if (buzzer_is_on) {
        if (buzzer_ticks > 0)
            buzzer_ticks--;
        else if (current_notes_ind < notes_size - 1) {
            current_notes_ind++;
            buzzer_on(notes_arr[current_notes_ind].note, notes_arr[current_notes_ind].length);
        } else if (melody_repeat_count < MELODY_REPEATS - 1) {
            melody_repeat_count++;
            current_notes_ind = 0;
            buzzer_on(notes_arr[current_notes_ind].note, notes_arr[current_notes_ind].length);
        } else {
            buzzer_off();
        }
    }
}
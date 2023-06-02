#include <stdio.h>

#include "buzzer.h"
#include "mcc_generated_files/pwm4.h"
#include "mcc_generated_files/tmr4.h"

#define NOTE_B7 62
#define NOTE_AS7 66
#define NOTE_A7 70
#define NOTE_GS7 74
#define NOTE_G7 79
#define NOTE_FS7 83
#define NOTE_F7 88
#define NOTE_E7 94
#define NOTE_DS7 99
#define NOTE_D7 105
#define NOTE_CS7 112
#define NOTE_C7 118
#define NOTE_B6 125
#define NOTE_AS6 133
#define NOTE_A6 141
#define NOTE_GS6 149
#define NOTE_G6 158
#define NOTE_FS6 168
#define NOTE_F6 178
#define NOTE_E6 188
#define NOTE_DS6 200
#define NOTE_D6 211
#define NOTE_CS6 224
#define NOTE_C6 238

#define REST 0x00

#define NOTE_LENGTH_1_8 80
#define NOTE_LENGTH_1_4 160
#define NOTE_LENGTH_1_2 320
#define NOTE_LENGTH_1 640

#define PWM_DUTY_VAL 213

#define MELODY_REPEATS 5

typedef struct Note {
    uint8_t note;
    uint16_t length;
} Note;

uint8_t buzzer_is_on = 0;
uint16_t buzzer_ticks;


Note notes_arr[] = {
    {NOTE_FS7, NOTE_LENGTH_1_8},
    {NOTE_FS7, NOTE_LENGTH_1_8},
    {NOTE_D7, NOTE_LENGTH_1_8},
    {NOTE_B6, NOTE_LENGTH_1_8},
    {REST, NOTE_LENGTH_1_8},
    {NOTE_B6, NOTE_LENGTH_1_8},
    {REST, NOTE_LENGTH_1_8},
    {NOTE_E7, NOTE_LENGTH_1_8},
    {REST, NOTE_LENGTH_1_8},
    {NOTE_E7, NOTE_LENGTH_1_8},
    {REST, NOTE_LENGTH_1_8},
    {NOTE_E7, NOTE_LENGTH_1_8},
    {NOTE_GS7, NOTE_LENGTH_1_8},
    {NOTE_GS7, NOTE_LENGTH_1_8},
    {NOTE_A7, NOTE_LENGTH_1_8},
    {NOTE_B7, NOTE_LENGTH_1_8},
    {NOTE_A7, NOTE_LENGTH_1_8},
    {NOTE_A7, NOTE_LENGTH_1_8},
    {NOTE_A7, NOTE_LENGTH_1_8},
    {NOTE_E7, NOTE_LENGTH_1_8},
    {REST, NOTE_LENGTH_1_8},
    {NOTE_D7, NOTE_LENGTH_1_8},
    {REST, NOTE_LENGTH_1_8},
    {NOTE_FS7, NOTE_LENGTH_1_8},
    {REST, NOTE_LENGTH_1_8},
    {NOTE_FS7, NOTE_LENGTH_1_8},
    {REST, NOTE_LENGTH_1_8},
    {NOTE_FS7, NOTE_LENGTH_1_8},
    {NOTE_E7, NOTE_LENGTH_1_8},
    {NOTE_E7, NOTE_LENGTH_1_8},
    {NOTE_FS7, NOTE_LENGTH_1_8},
    {NOTE_E7, NOTE_LENGTH_1_8},
};
uint8_t notes_size = 32;

uint8_t current_notes_ind;
uint8_t melody_repeat_count;

void buzzer_off(void) {
    PWM4_LoadDutyValue(0);
    buzzer_is_on = 0;
}

void buzzer_on(uint8_t note_tmr_val, uint16_t ticks) {
    if (note_tmr_val == REST) {
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
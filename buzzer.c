#include <stdio.h>

#include "buzzer.h"
#include "mcc_generated_files/pwm4.h"
#include "mcc_generated_files/tmr4.h"

//#define NOTE_B7 62
//#define NOTE_AS7 66
//#define NOTE_A7 70
//#define NOTE_GS7 74
//#define NOTE_G7 79
//#define NOTE_FS7 83
//#define NOTE_F7 88
//#define NOTE_E7 94
//#define NOTE_DS7 99
//#define NOTE_D7 105
//#define NOTE_CS7 112
//#define NOTE_C7 118
//#define NOTE_B6 125
//#define NOTE_AS6 133
//#define NOTE_A6 141
//#define NOTE_GS6 149
//#define NOTE_G6 158
//#define NOTE_FS6 168
//#define NOTE_F6 178
//#define NOTE_E6 188
//#define NOTE_DS6 200
//#define NOTE_D6 211
//#define NOTE_CS6 224
//#define NOTE_C6 238

#define NOTE_B6 63
#define NOTE_AS6 67
#define NOTE_A6 71
#define NOTE_GS6 75
#define NOTE_G6 80
#define NOTE_FS6 84
#define NOTE_F6 89
#define NOTE_E6 95
#define NOTE_DS6 100
#define NOTE_D6 106
#define NOTE_CS6 113
#define NOTE_C6 119

#define NOTE_B5 127
#define NOTE_AS5 134
#define NOTE_A5 142
#define NOTE_GS5 150
#define NOTE_G5 159
#define NOTE_FS5 169
#define NOTE_F5 179
#define NOTE_E5 190
#define NOTE_DS5 201
#define NOTE_D5 213
#define NOTE_CS5 225
#define NOTE_C5 239

#define REST 0x00

#define NOTE_LENGTH_1_16 40
#define NOTE_LENGTH_1_16_D 60
#define NOTE_LENGTH_1_8 80
#define NOTE_LENGTH_1_8_D 120
#define NOTE_LENGTH_1_4 160
#define NOTE_LENGTH_1_4_D 240
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


const Note melody_0[] = {
    // Take on me https://github.com/robsoncouto/arduino-songs/blob/master/takeonme/takeonme.ino
    {NOTE_FS6, NOTE_LENGTH_1_8},
    {NOTE_FS6, NOTE_LENGTH_1_8},
    {NOTE_D6, NOTE_LENGTH_1_8},
    {NOTE_B5, NOTE_LENGTH_1_8},
    {REST, NOTE_LENGTH_1_8},
    {NOTE_B5, NOTE_LENGTH_1_8},
    {REST, NOTE_LENGTH_1_8},
    {NOTE_E6, NOTE_LENGTH_1_8},
    {REST, NOTE_LENGTH_1_8},
    {NOTE_E6, NOTE_LENGTH_1_8},
    {REST, NOTE_LENGTH_1_8},
    {NOTE_E6, NOTE_LENGTH_1_8},
    {NOTE_GS6, NOTE_LENGTH_1_8},
    {NOTE_GS6, NOTE_LENGTH_1_8},
    {NOTE_A6, NOTE_LENGTH_1_8},
    {NOTE_B6, NOTE_LENGTH_1_8},
    {NOTE_A6, NOTE_LENGTH_1_8},
    {NOTE_A6, NOTE_LENGTH_1_8},
    {NOTE_A6, NOTE_LENGTH_1_8},
    {NOTE_E6, NOTE_LENGTH_1_8},
    {REST, NOTE_LENGTH_1_8},
    {NOTE_D6, NOTE_LENGTH_1_8},
    {REST, NOTE_LENGTH_1_8},
    {NOTE_FS6, NOTE_LENGTH_1_8},
    {REST, NOTE_LENGTH_1_8},
    {NOTE_FS6, NOTE_LENGTH_1_8},
    {REST, NOTE_LENGTH_1_8},
    {NOTE_FS6, NOTE_LENGTH_1_8},
    {NOTE_E6, NOTE_LENGTH_1_8},
    {NOTE_E6, NOTE_LENGTH_1_8},
    {NOTE_FS6, NOTE_LENGTH_1_8},
    {NOTE_E6, NOTE_LENGTH_1_8},
};
uint8_t melody_0_size = 32;

const Note melody_1[] = {
    // Imperial march https://github.com/robsoncouto/arduino-songs/blob/master/imperialmarch/imperialmarch.ino
    {NOTE_A5, NOTE_LENGTH_1_4},
    {NOTE_A5, NOTE_LENGTH_1_4},
    {NOTE_A5, NOTE_LENGTH_1_4},
    {NOTE_F5, NOTE_LENGTH_1_8_D},
    {NOTE_C6, NOTE_LENGTH_1_16},
    {NOTE_A5, NOTE_LENGTH_1_4},
    {NOTE_F5, NOTE_LENGTH_1_8_D},
    {NOTE_C6, NOTE_LENGTH_1_16},
    {NOTE_A5, NOTE_LENGTH_1_2}, //4
    {NOTE_E6, NOTE_LENGTH_1_4},
    {NOTE_E6, NOTE_LENGTH_1_4},
    {NOTE_E6, NOTE_LENGTH_1_4},
    {NOTE_F6, NOTE_LENGTH_1_8_D},
    {NOTE_C6, NOTE_LENGTH_1_16},
    {NOTE_A5, NOTE_LENGTH_1_4},
    {NOTE_F6, NOTE_LENGTH_1_8_D},
    {NOTE_C6, NOTE_LENGTH_1_16},
    {NOTE_A5, NOTE_LENGTH_1_2},
};
uint8_t melody_1_size = 18;

const Note melody_2[] = {
    // Nokia ringtone https://github.com/robsoncouto/arduino-songs/blob/master/nokia/nokia.ino
    {NOTE_E6, NOTE_LENGTH_1_8},
    {NOTE_D6, NOTE_LENGTH_1_8},
    {NOTE_FS5, NOTE_LENGTH_1_4},
    {NOTE_GS5, NOTE_LENGTH_1_4},
    {NOTE_CS6, NOTE_LENGTH_1_8},
    {NOTE_B5, NOTE_LENGTH_1_8},
    {NOTE_D5, NOTE_LENGTH_1_4},
    {NOTE_E5, NOTE_LENGTH_1_4},
    {NOTE_B5, NOTE_LENGTH_1_8},
    {NOTE_A5, NOTE_LENGTH_1_8},
    {NOTE_CS5, NOTE_LENGTH_1_4},
    {NOTE_E5, NOTE_LENGTH_1_4},
    {NOTE_A5, NOTE_LENGTH_1_2}
};
uint8_t melody_2_size = 13;

const Note melody_3[] = {
    // Fur Elise https://github.com/robsoncouto/arduino-songs/blob/master/furelise/furelise.ino
    {NOTE_E6, NOTE_LENGTH_1_16},
    {NOTE_DS6, NOTE_LENGTH_1_16}, //1
    {NOTE_E6, NOTE_LENGTH_1_16},
    {NOTE_DS6, NOTE_LENGTH_1_16},
    {NOTE_E6, NOTE_LENGTH_1_16},
    {NOTE_B5, NOTE_LENGTH_1_16},
    {NOTE_D6, NOTE_LENGTH_1_16},
    {NOTE_C6, NOTE_LENGTH_1_16},
    {NOTE_A5, NOTE_LENGTH_1_8_D},
    {NOTE_C5, NOTE_LENGTH_1_16},
    {NOTE_E5, NOTE_LENGTH_1_16},
    {NOTE_A5, NOTE_LENGTH_1_16},
    {NOTE_B5, NOTE_LENGTH_1_8_D},
    {NOTE_E5, NOTE_LENGTH_1_16},
    {NOTE_GS5, NOTE_LENGTH_1_16},
    {NOTE_B5, NOTE_LENGTH_1_16},
    {NOTE_C6, NOTE_LENGTH_1_8},
    {REST, NOTE_LENGTH_1_16},
    {NOTE_E5, NOTE_LENGTH_1_16},
    {NOTE_E6, NOTE_LENGTH_1_16},
    {NOTE_DS6, NOTE_LENGTH_1_16}
};
uint8_t melody_3_size = 21;

const Note* current_melody;
uint8_t current_melody_size;
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

void start_melody(uint8_t melody_index) {
    switch (melody_index) {
        case 1:
            current_melody = melody_1;
            current_melody_size = melody_1_size;
            break;
        case 2:
            current_melody = melody_2;
            current_melody_size = melody_2_size;
            break;
        case 3:
            current_melody = melody_3;
            current_melody_size = melody_3_size;
            break;
        default:
            current_melody = melody_0;
            current_melody_size = melody_0_size;
    }

    buzzer_on(current_melody[0].note, current_melody[0].length);
    current_notes_ind = 0;
    melody_repeat_count = 0;
}

void refresh_buzzer(void) {
    if (buzzer_is_on) {
        if (buzzer_ticks > 0) {
            if (buzzer_ticks == current_melody[current_notes_ind].length >> 3)
                PWM4_LoadDutyValue(0);
            buzzer_ticks--;
        } else if (current_notes_ind < current_melody_size - 1) {
            current_notes_ind++;
            buzzer_on(current_melody[current_notes_ind].note, current_melody[current_notes_ind].length);
        } else if (melody_repeat_count < MELODY_REPEATS - 1) {
            melody_repeat_count++;
            current_notes_ind = 0;
            buzzer_on(current_melody[current_notes_ind].note, current_melody[current_notes_ind].length);
        } else {
            buzzer_off();
        }
    }
}
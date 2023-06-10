#include "nixie.h"
#include "settings.h"

#define DIGITS_TO_VALUES(dig0, dig1, dig2, dig3) copy_digits(dig0, dig1, dig2, dig3, digit_values);
#define DIGITS_TO_SAVED_VALUES(dig0, dig1, dig2, dig3) copy_digits(dig0, dig1, dig2, dig3, saved_digit_values);

#define DISPLAY_ALL() copy_digits(1, 1, 1, 1, digit_displayed)
#define SET_DISPLAYED(disp0, disp1,disp2,disp3) copy_digits(disp0, disp1, disp2, disp3, digit_displayed)

typedef enum DisplayEffect {
    NONE,
    TOGGLE,
    FLIP_ALL,
    FLIP_SEQ,
    SHIFT
} DisplayEffect;

const uint8_t decoder_arr[] = {
    0b00000000, 0b00000010, 0b00000101, 0b00000111, 0b00000011,
    0b00001001, 0b00000110, 0b00001000, 0b00000100, 0b00000001
};

const uint8_t flip_num_arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

uint8_t digit_values[DIGITS_SIZE];
uint8_t digit_displayed[DIGITS_SIZE];

uint8_t current = 0;
uint8_t is_on = 0;

DisplayEffect effect;
uint16_t effect_ticks;
uint8_t digit_toggled[DIGITS_SIZE];
uint8_t saved_digit_values[DIGITS_SIZE];
uint8_t prev_digit_values[DIGITS_SIZE];

void copy_arr(uint8_t* src_arr, uint8_t* dest_arr, uint8_t size) {
    for (int i = 0; i < size; i++) {
        dest_arr[i] = src_arr[i];
    }
}

void copy_digits(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3, uint8_t* dest_arr) {
    dest_arr[0] = dig0;
    dest_arr[1] = dig1;
    dest_arr[2] = dig2;
    dest_arr[3] = dig3;
}

void set_digits(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3) {
    DIGITS_TO_VALUES(dig0, dig1, dig2, dig3);
    DISPLAY_ALL();
    effect = NONE;
}

void toggle_digit_displayed(uint8_t number) {
    digit_displayed[number] ^= 1;
}

void on_digit(uint8_t number) {
    uint8_t dec_pin_vals = decoder_arr[digit_values[number]];

    DEC_A0_LAT = dec_pin_vals & 0x01;
    DEC_A1_LAT = dec_pin_vals >> 1 & 0x01;
    DEC_A2_LAT = dec_pin_vals >> 2 & 0x01;
    DEC_A3_LAT = dec_pin_vals >> 3 & 0x01;

    int l_pin_vals = 1 << number;

    L1_LAT = l_pin_vals & 0x01;
    L2_LAT = l_pin_vals >> 1 & 0x01;
    L3_LAT = l_pin_vals >> 2 & 0x01;
    L4_LAT = l_pin_vals >> 3 & 0x01;
}

void off_digits(void) {
    L1_SetLow();
    L2_SetLow();
    L3_SetLow();
    L4_SetLow();
}

void refresh_digits(void) {
    if (effect == TOGGLE) {
        if (effect_ticks < REFRESH_FREQ / 2 - 1)
            effect_ticks++;
        else {
            for (uint8_t i = 0; i < DIGITS_SIZE; i++)
                if (digit_toggled[i]) toggle_digit_displayed(i);
            effect_ticks = 0;
        }
    }

    if (effect == FLIP_ALL) {
        if (effect_ticks < REFRESH_FREQ) {
            if (effect_ticks % 10 == 0) {
                uint8_t i = (effect_ticks % 100) / 10;
                DIGITS_TO_VALUES(flip_num_arr[i], flip_num_arr[i], flip_num_arr[i], flip_num_arr[i]);
            }
            effect_ticks++;
        } else {
            DIGITS_TO_VALUES(saved_digit_values[0], saved_digit_values[1], saved_digit_values[2], saved_digit_values[3]);
            effect = NONE;
        }
    }

    if (effect == FLIP_SEQ) {
        if (effect_ticks < REFRESH_FREQ) {
            if (effect_ticks % 10 == 0) {
                uint8_t dig_num = (uint8_t) (effect_ticks / 100);
                uint8_t i = (effect_ticks % 100) / 10;
                switch (dig_num) {
                    case 0:
                        DIGITS_TO_VALUES(flip_num_arr[i], prev_digit_values[1], prev_digit_values[2], prev_digit_values[3]);
                        break;

                    case 1:
                        DIGITS_TO_VALUES(saved_digit_values[0], flip_num_arr[i], prev_digit_values[2], prev_digit_values[3]);
                        break;

                    case 2:
                        DIGITS_TO_VALUES(saved_digit_values[0], saved_digit_values[1], flip_num_arr[i], prev_digit_values[3]);
                        break;

                    case 3:
                        DIGITS_TO_VALUES(saved_digit_values[0], saved_digit_values[1], saved_digit_values[2], flip_num_arr[i]);
                }
            }
            effect_ticks++;
        } else {
            DIGITS_TO_VALUES(saved_digit_values[0], saved_digit_values[1], saved_digit_values[2], saved_digit_values[3]);
            effect = NONE;
        }
    }

    if (effect == SHIFT) {
        if (effect_ticks < REFRESH_FREQ) {
            if (effect_ticks % 80 == 0) {
                switch (effect_ticks / 80) {
                    case 0:
                        DIGITS_TO_VALUES(prev_digit_values[1], prev_digit_values[2], prev_digit_values[3], 0);
                        SET_DISPLAYED(1, 1, 1, 0);
                        break;
                    case 1:
                        DIGITS_TO_VALUES(prev_digit_values[2], saved_digit_values[3], 0, 0);
                        SET_DISPLAYED(1, 1, 0, 0);
                        break;
                    case 2:
                        DIGITS_TO_VALUES(prev_digit_values[3], 0, 0, saved_digit_values[0]);
                        SET_DISPLAYED(1, 0, 0, 1);
                        break;
                    case 3:
                        DIGITS_TO_VALUES(0, 0, saved_digit_values[0], saved_digit_values[1]);
                        SET_DISPLAYED(0, 0, 1, 1);
                        break;
                    default:
                        DIGITS_TO_VALUES(0, saved_digit_values[0], saved_digit_values[1], saved_digit_values[2]);
                        SET_DISPLAYED(0, 1, 1, 1);
                }
            }
            effect_ticks++;
        } else {
            DIGITS_TO_VALUES(saved_digit_values[0], saved_digit_values[1], saved_digit_values[2], saved_digit_values[3]);
            DISPLAY_ALL();
            effect = NONE;
        }
    }

    if (is_on) {
        off_digits();
        is_on = 0;
    } else {
        if (digit_displayed[current]) on_digit(current);
        current = (current + 1) % DIGITS_SIZE;
        is_on = 1;
    }
}

void toggle(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3, uint8_t dig0_toggled, uint8_t dig1_toggled, uint8_t dig2_toggled, uint8_t dig3_toggled) {
    effect = TOGGLE;
    DIGITS_TO_VALUES(dig0, dig1, dig2, dig3);
    copy_digits(dig0_toggled, dig1_toggled, dig2_toggled, dig3_toggled, digit_toggled);
    effect_ticks = 0;
    DISPLAY_ALL();
}

void flip_all(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3) {
    effect = FLIP_ALL;
    DIGITS_TO_SAVED_VALUES(dig0, dig1, dig2, dig3);
    effect_ticks = 0;
    DISPLAY_ALL();
}

void flip_seq(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3) {
    effect = FLIP_SEQ;
    copy_arr(digit_values, prev_digit_values, DIGITS_SIZE);
    DIGITS_TO_SAVED_VALUES(dig0, dig1, dig2, dig3);
    effect_ticks = 0;
    DISPLAY_ALL();
}

void shift(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3) {
    effect = SHIFT;
    copy_arr(digit_values, prev_digit_values, DIGITS_SIZE);
    DIGITS_TO_SAVED_VALUES(dig0, dig1, dig2, dig3);
    effect_ticks = 0;
    DISPLAY_ALL();
}
#include "nixie.h"
#include "settings.h"

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
uint8_t saved_digit_values[DIGITS_SIZE * 2];

void copy_digits(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3) {
    digit_values[0] = dig0;
    digit_values[1] = dig1;
    digit_values[2] = dig2;
    digit_values[3] = dig3;
}

void copy_digit_toggled(uint8_t dig0_toggled, uint8_t dig1_toggled, uint8_t dig2_toggled, uint8_t dig3_toggled) {
    digit_toggled[0] = dig0_toggled;
    digit_toggled[1] = dig1_toggled;
    digit_toggled[2] = dig2_toggled;
    digit_toggled[3] = dig3_toggled;
}

void copy_saved_digits(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3, uint8_t dig4,
        uint8_t dig5, uint8_t dig6, uint8_t dig7) {
    saved_digit_values[0] = dig0;
    saved_digit_values[1] = dig1;
    saved_digit_values[2] = dig2;
    saved_digit_values[3] = dig3;
    saved_digit_values[4] = dig4;
    saved_digit_values[5] = dig5;
    saved_digit_values[6] = dig6;
    saved_digit_values[7] = dig7;
}

void set_digit_displayed(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3) {
    digit_displayed[0] = dig0;
    digit_displayed[1] = dig1;
    digit_displayed[2] = dig2;
    digit_displayed[3] = dig3;
}

void set_digit_displayed_all(void) {
    set_digit_displayed(1, 1, 1, 1);
}

void set_digits(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3) {
    copy_digits(dig0, dig1, dig2, dig3);
    set_digit_displayed_all();
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
        if (effect_ticks < TICKS_FREQ / 2 - 1)
            effect_ticks++;
        else {
            for (uint8_t i = 0; i < DIGITS_SIZE; i++)
                if (digit_toggled[i]) toggle_digit_displayed(i);
            effect_ticks = 0;
        }
    }

    if (effect == FLIP_ALL) {
        if (effect_ticks < TICKS_FREQ) {
            if (effect_ticks % 10 == 0) {
                uint8_t i = (effect_ticks % 100) / 10;
                copy_digits(flip_num_arr[i], flip_num_arr[i], flip_num_arr[i], flip_num_arr[i]);
            }
            effect_ticks++;
        } else {
            copy_digits(saved_digit_values[0], saved_digit_values[1], saved_digit_values[2], saved_digit_values[3]);
            effect = NONE;
        }
    }

    if (effect == FLIP_SEQ) {
        if (effect_ticks < TICKS_FREQ) {
            if (effect_ticks % 10 == 0) {
                uint8_t dig_num = (uint8_t) (effect_ticks / 100);
                uint8_t i = (effect_ticks % 100) / 10;
                switch (dig_num) {
                    case 0:
                        copy_digits(flip_num_arr[i], saved_digit_values[1], saved_digit_values[2], saved_digit_values[3]);
                        break;

                    case 1:
                        copy_digits(saved_digit_values[4], flip_num_arr[i], saved_digit_values[2], saved_digit_values[3]);
                        break;

                    case 2:
                        copy_digits(saved_digit_values[4], saved_digit_values[5], flip_num_arr[i], saved_digit_values[3]);
                        break;

                    case 3:
                        copy_digits(saved_digit_values[4], saved_digit_values[5], saved_digit_values[6], flip_num_arr[i]);
                }
            }
            effect_ticks++;
        } else {
            copy_digits(saved_digit_values[4], saved_digit_values[5], saved_digit_values[6], saved_digit_values[7]);
            effect = NONE;
        }
    }

    if (effect == SHIFT) {
        if (effect_ticks < TICKS_FREQ) {
            if (effect_ticks % 80 == 0) {
                switch (effect_ticks / 80) {
                    case 0:
                        copy_digits(saved_digit_values[0], saved_digit_values[1], saved_digit_values[2], 0);
                        set_digit_displayed(1, 1, 1, 0);
                        break;
                    case 1:
                        copy_digits(saved_digit_values[1], saved_digit_values[2], 0, 0);
                        set_digit_displayed(1, 1, 0, 0);
                        break;
                    case 2:
                        copy_digits(saved_digit_values[2], 0, 0, saved_digit_values[4]);
                        set_digit_displayed(1, 0, 0, 1);
                        break;
                    case 3:
                        copy_digits(0, 0, saved_digit_values[4], saved_digit_values[5]);
                        set_digit_displayed(0, 0, 1, 1);
                        break;
                    default:
                        copy_digits(0, saved_digit_values[4], saved_digit_values[5], saved_digit_values[6]);
                        set_digit_displayed(0, 1, 1, 1);
                }
            }
            effect_ticks++;
        } else {
            copy_digits(saved_digit_values[4], saved_digit_values[5], saved_digit_values[6], saved_digit_values[7]);
            set_digit_displayed_all();
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
    copy_digits(dig0, dig1, dig2, dig3);
    copy_digit_toggled(dig0_toggled, dig1_toggled, dig2_toggled, dig3_toggled);
    effect_ticks = 0;
    set_digit_displayed_all();
}

void flip_all(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3) {
    effect = FLIP_ALL;
    copy_saved_digits(dig0, dig1, dig2, dig3, 0, 0, 0, 0);
    effect_ticks = 0;
    set_digit_displayed_all();
}

void flip_seq(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3, uint8_t dig4, uint8_t dig5, uint8_t dig6, uint8_t dig7) {
    effect = FLIP_SEQ;
    copy_saved_digits(dig0, dig1, dig2, dig3, dig4, dig5, dig6, dig7);
    effect_ticks = 0;
    set_digit_displayed_all();
}

void shift(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3, uint8_t dig4, uint8_t dig5, uint8_t dig6, uint8_t dig7) {
    effect = SHIFT;
    copy_saved_digits(dig0, dig1, dig2, dig3, dig4, dig5, dig6, dig7);
    effect_ticks = 0;
    set_digit_displayed_all();
}
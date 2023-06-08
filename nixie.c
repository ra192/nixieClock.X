#include "nixie.h"

const uint8_t decoder_arr[] = {
    0b00000000, 0b00000010, 0b00000101, 0b00000111, 0b00000011,
    0b00001001, 0b00000110, 0b00001000, 0b00000100, 0b00000001
};

const uint8_t flip_num_arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

uint8_t digit_values[DIGITS_SIZE];
uint8_t digit_displayed[DIGITS_SIZE];

uint8_t current = 0;
uint8_t is_on = 0;

void set_digits(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3) {
    digit_values[0] = dig0;
    digit_values[1] = dig1;
    digit_values[2] = dig2;
    digit_values[3] = dig3;

    set_digit_displayed_all();
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
    if (is_on) {
        off_digits();
        is_on = 0;
    } else {
        if (digit_displayed[current]) on_digit(current);
        current = (current + 1) % DIGITS_SIZE;
        is_on = 1;
    }
}

void flip_all(uint16_t timer_count) {
    if (timer_count % 10 == 0) {
        uint8_t i = (timer_count % 100) / 10;
        set_digits(flip_num_arr[i], flip_num_arr[i], flip_num_arr[i], flip_num_arr[i]);
    }
}

void flip_seq(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3, uint8_t dig4, uint8_t dig5, uint16_t timer_count) {
    if (timer_count % 10 == 0) {
        uint8_t dig_num = (uint8_t) (timer_count / 100);
        uint8_t i = (timer_count % 100) / 10;
        switch (dig_num) {
            case 0:
                set_digits(flip_num_arr[i], dig0, dig1, dig2);
                break;

            case 1:
                set_digits(dig3, flip_num_arr[i], dig1, dig2);
                break;

            case 2:
                set_digits(dig3, dig4, flip_num_arr[i], dig2);
                break;

            case 3:
                set_digits(dig3, dig4, dig5, flip_num_arr[i]);
        }
    }
}

void shift(uint8_t dig0, uint8_t dig1, uint8_t dig2, uint8_t dig3, uint8_t dig4, uint8_t dig5, uint16_t timer_count) {
    if (timer_count % 80 == 0) {
        switch (timer_count / 80) {
            case 0:
                set_digits(dig0, dig1, dig2, 0);
                set_digit_displayed(1, 1, 1, 0);
                break;
            case 1:
                set_digits(dig1, dig2, 0, 0);
                set_digit_displayed(1, 1, 0, 0);
                break;
            case 2:
                set_digits(dig2, 0, 0, dig3);
                set_digit_displayed(1, 0, 0, 1);
                break;
            case 3:
                set_digits(0, 0, dig3, dig4);
                set_digit_displayed(0, 0, 1, 1);
                break;
            default:
                set_digits(0, dig3, dig4, dig5);
                set_digit_displayed(0, 1, 1, 1);
        }
    }
}
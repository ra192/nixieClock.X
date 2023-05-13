#include "nixie.h"

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
    digit_displayed[number]^= 1;
}

void on_digit(uint8_t number) {
    switch (digit_values[number]) {
        case 0:
            DEC_A0_SetLow();
            DEC_A1_SetLow();
            DEC_A2_SetLow();
            DEC_A3_SetLow();
            break;
        case 1:
            DEC_A0_SetLow();
            DEC_A1_SetHigh();
            DEC_A2_SetLow();
            DEC_A3_SetLow();
            break;
        case 2:
            DEC_A0_SetHigh();
            DEC_A1_SetLow();
            DEC_A2_SetHigh();
            DEC_A3_SetLow();
            break;
        case 3:
            DEC_A0_SetHigh();
            DEC_A1_SetHigh();
            DEC_A2_SetHigh();
            DEC_A3_SetLow();
            break;
        case 4:
            DEC_A0_SetHigh();
            DEC_A1_SetHigh();
            DEC_A2_SetLow();
            DEC_A3_SetLow();
            break;
        case 5:
            DEC_A0_SetHigh();
            DEC_A1_SetLow();
            DEC_A2_SetLow();
            DEC_A3_SetHigh();
            break;
        case 6:
            DEC_A0_SetLow();
            DEC_A1_SetHigh();
            DEC_A2_SetHigh();
            DEC_A3_SetLow();
            break;
        case 7:
            DEC_A0_SetLow();
            DEC_A1_SetLow();
            DEC_A2_SetLow();
            DEC_A3_SetHigh();
            break;
        case 8:
            DEC_A0_SetLow();
            DEC_A1_SetLow();
            DEC_A2_SetHigh();
            DEC_A3_SetLow();
            break;
        case 9:
            DEC_A0_SetHigh();
            DEC_A1_SetLow();
            DEC_A2_SetLow();
            DEC_A3_SetLow();
    }

    switch (number) {
        case 0:
            L1_SetHigh();
            L2_SetLow();
            L3_SetLow();
            L4_SetLow();
            break;
        case 1:
            L1_SetLow();
            L2_SetHigh();
            L3_SetLow();
            L4_SetLow();
            break;
        case 2:
            L1_SetLow();
            L2_SetLow();
            L3_SetHigh();
            L4_SetLow();
            break;
        case 3:
            L1_SetLow();
            L2_SetLow();
            L3_SetLow();
            L4_SetHigh();
    }
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
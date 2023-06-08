#include "dekatron.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/device_config.h"

#define CATHODES_COUNT 30

uint8_t cat_num = 0;
uint8_t val;

uint8_t ticks = 0;

DekMode mode = DISPLAY_VAL;

void change_cat() {
    if (cat_num == 0) {
        CAT_0_SetHigh();
        CAT_SetLow();
        CAT_1_SetLow();
        CAT_2_SetLow();
    } else {
        uint8_t cat_pins = (uint8_t) (1 << cat_num % 3);
        CAT_0_LAT = 0;
        CAT_LAT = cat_pins & 0x01;
        CAT_1_LAT = cat_pins >> 1 & 0x01;
        CAT_2_LAT = cat_pins >> 2 & 0x01;
    }
}

void move_next(void) {
    cat_num = (cat_num + 1) % CATHODES_COUNT;
    change_cat();
}

void move_prev(void) {
    if (cat_num > 0)
        cat_num--;
    else
        cat_num = CATHODES_COUNT - 1;

    change_cat();
}

void dek_set_val(uint8_t disp_val) {
    val = disp_val % CATHODES_COUNT;
}

void dek_set_mode(DekMode mod) {
    mode = mod;
    if (mode == SPIN_CW || mode == SPIN_CCW)
        ticks = 0;
}

void refresh_dek(void) {
    switch (mode) {
        case SPIN_CW:
            if (ticks == 0) move_next();
            ticks = (ticks + 1) % 4;
            break;
        case SPIN_CCW:
            if (ticks == 0) move_prev();
            ticks = (ticks + 1) % 4;
            break;
        case FILL:
            if (cat_num < val)
                move_next();
            else {
                cat_num = 0;
                change_cat();
            }
            break;
        default:
            if (cat_num != val)
                move_next();
    }
}
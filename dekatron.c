#include "dekatron.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/device_config.h"

uint8_t cat_num = 0;
uint8_t val;

uint8_t ticks = 0;

uint8_t switch_count;
uint8_t direction;

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

void dek_set_mode(DekMode mod) {
    mode = mod;

    switch (mode) {
        case SPIN_CW:
            direction = 0;
            ticks = 0;
            break;
        case SPIN_CCW:
            direction = 1;
            ticks = 0;
            break;
        case DISPLAY_WITH_SPIN_CW:
            direction = 0;
            ticks = 0;
            switch_count = 0;
            break;
        case DISPLAY_WITH_SPIN_CCW:
            direction = 1;
            ticks = 0;
            switch_count = 0;
            break;
        default:
            break;
    }
}

void dek_set_val(uint8_t disp_val) {
    switch (mode) {
        case SPIN_CW:
        case SPIN_CCW:
            val = disp_val % CATHODES_COUNT;
            ticks = 0;
            break;
        case DISPLAY_WITH_SPIN_CW:
        case DISPLAY_WITH_SPIN_CCW:
            val = disp_val % CATHODES_COUNT;
            ticks = 0;
            switch_count = 0;
            break;
        case RING:
            val = disp_val % (CATHODES_COUNT * 2);
            break;
        default:
            val = disp_val % CATHODES_COUNT;
    }
}

void move(uint8_t dir) {
    if (direction)
        move_prev();
    else
        move_next();
}

void refresh_dek(void) {
    switch (mode) {
        case SPIN_CW:
        case SPIN_CCW:
            if (ticks == 0) {
                move(direction);
            };
            ticks = (ticks + 1) % SPIN_PRESC;
            break;
        case DISPLAY_WITH_SPIN_CW:
        case DISPLAY_WITH_SPIN_CCW:
            if (cat_num == val) {
                if (switch_count < CATHODES_COUNT / 2) {
                    if (ticks == 0) {
                        move(direction);
                        switch_count++;
                    }
                    ticks = (ticks + 1) % SPIN_PRESC;
                }
            } else {
                if (ticks == 0) {
                    move(direction);
                    switch_count++;
                }
                ticks = (ticks + 1) % SPIN_PRESC;
            }
            break;
        case RING:
            if (val < CATHODES_COUNT) {
                if (cat_num < val) {
                    move_next();
                } else {
                    cat_num = 0;
                    change_cat();
                }
            } else {
                uint8_t cat_val = val % CATHODES_COUNT;

                if ((cat_num > cat_val + 1) || (cat_num == 0 && cat_val != CATHODES_COUNT - 1)) {
                        move_prev();
                    } else {
                    cat_num = 0;
                            change_cat();
                }
            }
            break;
        default:
            if (cat_num != val)
                    move_next();
            }
}
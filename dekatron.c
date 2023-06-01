#include "dekatron.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/device_config.h"

#define CATHODES_COUNT 30

uint8_t cat_num = 0;

void dek_set_zero(void) {
    CAT_0_SetHigh();
    CAT_SetLow();
    CAT_1_SetLow();
    CAT_2_SetLow();
    cat_num = 0;
}

void dek_move_next(void) {
    cat_num = (cat_num + 1) % CATHODES_COUNT;
    if (cat_num == 0) {
        dek_set_zero();
    } else {
        uint8_t cat_pins = (uint8_t) (1 << cat_num % 3);
        CAT_0_LAT = 0;
        CAT_LAT = cat_pins & 0x01;
        CAT_1_LAT = cat_pins >> 1 & 0x01;
        CAT_2_LAT = cat_pins >> 2 & 0x01;
    }
}

uint8_t dek_get_cat_num(void) {
    return cat_num;
}
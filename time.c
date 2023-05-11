#include "time.h"
#include "i2c.h"

#define DS3231_WRITE_ADDRESS 0xD0
#define DS3231_READ_ADDRESS 0xD1

extern void i2c_start();

void read_time(Time* time) {
    i2c_start();
    i2c_wr(DS3231_WRITE_ADDRESS);
    i2c_wr(0x00);

    i2c_start();
    i2c_wr(DS3231_READ_ADDRESS);

    uint8_t reg_arr[7];

    i2c_rd_bytes(reg_arr, 7);

    i2c_stop();

    time->ss = (reg_arr[0] >> 4)*10 + (reg_arr[0] & 0x0F);

    time->mm = (reg_arr[1] >> 4)*10 + (reg_arr[1] & 0x0F);

    time->is_12 = reg_arr[2] >> 6;

    if (time->is_12) {
        time->pm = reg_arr[2] >> 5;
        time->hh = (reg_arr[2] >> 4 & 0x01) * 10 + (reg_arr[2] & 0x0F);
    } else {
        time->hh = (reg_arr[2] >> 4 & 0x03) * 10 + (reg_arr[2] & 0x0F);
    }

    time->day = reg_arr[3];

    time->dd = (reg_arr[4] >> 4)*10 + (reg_arr[4] & 0x0F);

    time->MM = (reg_arr[5] >> 4)*10 + (reg_arr[5] & 0x0F);

    time->yy = (reg_arr[6] >> 4)*10 + (reg_arr[6] & 0x0F);
}

void update_time(Time* time) {
    uint8_t reg_arr[7];

    reg_arr[0] = (uint8_t) (time->ss / 10 << 4 | (time->ss % 10));
    reg_arr[1] = (uint8_t) (time->mm / 10 << 4 | (time->mm % 10));

    if (time->is_12) {
        reg_arr[2] = (uint8_t) (1 << 6 | time->pm << 5 | time->hh / 10 << 4 | (time->hh % 10));
    } else {
        reg_arr[2] = (uint8_t) (time->hh / 10 << 4 | (time->hh % 10));
    }

    reg_arr[3] = time->day;
    reg_arr[4] = (uint8_t) (time->dd / 10 << 4 | (time->dd % 10));
    reg_arr[5] = (uint8_t) (time->MM / 10 << 4 | (time->MM % 10));
    reg_arr[6] = (uint8_t) (time->yy / 10 << 4 | (time->yy % 10));

    i2c_start();
    i2c_wr(DS3231_WRITE_ADDRESS);
    i2c_wr(0x00);
    i2c_wr_bytes(reg_arr, 7);
    i2c_stop();
}

void increase_hour(Time* time) {
    if (time->is_12) {
        if (time->hh < 12)
            time->hh++;
        else {
            time->hh = 1;
            time->pm = (time->pm) ? 0 : 1;
        }
    } else {
        time->hh = (time->hh + 1) % 24;
    }
}

void decrease_hour(Time* time) {
    if (time->is_12) {
        if (time->hh > 1)
            time->hh--;
        else {
            time->hh = 1;
            time->pm = (time->pm) ? 0 : 1;
        }
    } else {
        if (time->hh > 0)
            time->hh--;
        else {
            time->hh = 23;
        }
    }
}

void increase_minute(Time* time) {
    time->mm = (time->mm + 1) % 60;
}

void decrease_minute(Time* time) {
    if (time->mm > 0)
        time->mm--;
    else
        time->mm = 59;
}

void increase_date(Time* time) {
    if (time->MM == 2) {
        if (time->dd < 28)
            time->dd++;
        else
            time->dd = 1;
    } else if (time->MM == 1 || time->MM == 3 || time->MM == 5 || time->MM == 7
            || time->MM == 8 || time->MM == 10 || time->MM == 12) {
        if (time->dd < 31)
            time->dd++;
        else
            time->dd = 1;
    } else {
        if (time->dd < 30)
            time->dd++;
        else
            time->dd = 1;
    }
}

void decrease_date(Time* time) {
    if (time->MM == 2) {
        if (time->dd > 1)
            time->dd--;
        else
            time->dd = 28;
    } else if (time->MM == 1 || time->MM == 3 || time->MM == 5 || time->MM == 7
            || time->MM == 8 || time->MM == 10 || time->MM == 12) {
        if (time->dd > 1)
            time->dd--;
        else
            time->dd = 31;
    } else {
        if (time->dd > 1)
            time->dd--;
        else
            time->dd = 30;
    }
}

void increase_month(Time* time) {
    if (time->MM < 12)
        time->MM++;
    else
        time->MM = 1;
}

void decrease_month(Time* time) {
    if (time->MM > 1)
        time->MM--;
    else
        time->MM = 12;
}

void copy_time_fields(Time* src_time, Time* dest_time) {
    dest_time->hh = src_time->hh;
    dest_time->is_12 = src_time->is_12;
    dest_time->pm = src_time->pm;

    dest_time->mm = src_time->mm;
    dest_time->ss = src_time->ss;

    dest_time->day = src_time->day;

    dest_time->dd = src_time->dd;

    dest_time->MM = src_time->MM;

    dest_time->yy = src_time->yy;
}
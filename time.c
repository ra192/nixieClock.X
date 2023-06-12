#include <stdlib.h>

#include "time.h"
#include "i2c.h"

#define DS3231_ADDRESS 0xD0

extern void i2c_start();

void read_bytes(uint8_t start_address, uint8_t* reg_arr, uint8_t length) {
    i2c_start();
    i2c_wr(DS3231_ADDRESS);
    i2c_wr(start_address);

    i2c_start();
    i2c_wr(DS3231_ADDRESS | 0x01);

    i2c_rd_bytes(reg_arr, length);

    i2c_stop();
}

void read_time(Time* time) {
    uint8_t reg_arr[3];

    read_bytes(0x00, reg_arr, 3);

    time->ss = (reg_arr[0] >> 4)*10 + (reg_arr[0] & 0x0F);
    time->mm = (reg_arr[1] >> 4)*10 + (reg_arr[1] & 0x0F);

    time->is_12 = reg_arr[2] >> 6;

    if (time->is_12) {
        time->pm = reg_arr[2] >> 5 & 0x01;
        time->hh = (reg_arr[2] >> 4 & 0x01) * 10 + (reg_arr[2] & 0x0F);
    } else {
        time->hh = (reg_arr[2] >> 4 & 0x03) * 10 + (reg_arr[2] & 0x0F);
    }
}

void read_date(Date* date) {
    uint8_t reg_arr[4];

    read_bytes(0x03, reg_arr, 4);

    date->day = reg_arr[0];
    date->dd = (reg_arr[1] >> 4)*10 + (reg_arr[1] & 0x0F);
    date->MM = (reg_arr[2] >> 4)*10 + (reg_arr[2] & 0x0F);
    date->yy = (reg_arr[3] >> 4)*10 + (reg_arr[3] & 0x0F);
}

void read_alarm(Alarm* alarm) {
    uint8_t reg_arr[4];
    read_bytes(0x07, reg_arr, 4);

    alarm->ss = (reg_arr[0] >> 4 & 0x07)*10 + (reg_arr[0] & 0x0F);
    alarm->mm = (reg_arr[1] >> 4 & 0x07)*10 + (reg_arr[1] & 0x0F);
    alarm->hh = (reg_arr[2] >> 4 & 0x03)*10 + (reg_arr[2] & 0x0F);
    alarm->dy_dt = reg_arr[3] >> 6 & 0x01;
    alarm->dd = (reg_arr[3] >> 4 & 0x03)*10 + (reg_arr[3] & 0x0F);
    alarm->on = reg_arr[3] >> 7;
}

void read_temp(Temp* temp) {
    uint8_t reg_arr[2];
    read_bytes(0x11, reg_arr, 2);

    temp->int_part = reg_arr[0];
    temp->fract_part = (reg_arr[1] >> 6) * 25;
}

void write_bytes(uint8_t start_address, uint8_t* reg_arr, uint8_t length) {
    i2c_start();
    i2c_wr(DS3231_ADDRESS);
    i2c_wr(start_address);
    i2c_wr_bytes(reg_arr, length);
    i2c_stop();
}

void update_time(Time* time) {
    uint8_t reg_arr[3];

    reg_arr[0] = (uint8_t) (time->ss / 10 << 4 | (time->ss % 10));
    reg_arr[1] = (uint8_t) (time->mm / 10 << 4 | (time->mm % 10));

    if (time->is_12) {
        reg_arr[2] = (uint8_t) (1 << 6 | time->pm << 5 | time->hh / 10 << 4 | (time->hh % 10));
    } else {
        reg_arr[2] = (uint8_t) (time->hh / 10 << 4 | (time->hh % 10));
    }

    write_bytes(0x00, reg_arr, 3);
}

void update_date(Date* date) {
    uint8_t reg_arr[4];
    
    reg_arr[0] = date->day;
    reg_arr[1] = (uint8_t) (date->dd / 10 << 4 | (date->dd % 10));
    reg_arr[2] = (uint8_t) (date->MM / 10 << 4 | (date->MM % 10));
    reg_arr[3] = (uint8_t) (date->yy / 10 << 4 | (date->yy % 10));
    
    write_bytes(0x03, reg_arr, 4);
}

void update_alarm(Alarm* alarm, uint8_t is_12) {
    uint8_t reg_arr[4];

    reg_arr[0] = (uint8_t) (alarm->ss / 10 << 4 | (alarm->ss % 10));
    reg_arr[1] = (uint8_t) (alarm->mm / 10 << 4 | (alarm->mm % 10));

    if (is_12) {
        reg_arr[2] = (uint8_t) (1 << 6 | alarm->pm << 5 | alarm->hh / 10 << 4 | (alarm->hh % 10));
    } else {
        reg_arr[2] = (uint8_t) (alarm->hh / 10 << 4 | (alarm->hh % 10));
    }

    reg_arr[3] = (uint8_t) (alarm->on << 7 | alarm->dd / 10 << 4 | alarm->dd % 10);

    write_bytes(0x07, reg_arr, 4);
}

void increase_hour(uint8_t* hh, uint8_t *pm, uint8_t is_12) {
    if (is_12) {
        if (*hh < 12)
            (*hh)++;
        else {
            *hh = 1;
            *pm = (*pm) ? 0 : 1;
        }
    } else {
        *hh = (*hh + 1) % 24;
    }
}

void decrease_hour(uint8_t* hh, uint8_t *pm, uint8_t is_12) {
    if (is_12) {
        if (*hh > 1)
            (*hh)--;
        else {
            *hh = 1;
            *pm = (*pm) ? 0 : 1;
        }
    } else {
        if (*hh > 0)
            (*hh)--;
        else {
            *hh = 23;
        }
    }
}

void increase_minute(uint8_t* mm) {
    *mm = (*mm + 1) % 60;
}

void decrease_minute(uint8_t* mm) {
    if (*mm > 0)
        (*mm)--;
    else
        *mm = 59;
}

void increase_date(uint8_t* dd, uint8_t MM) {
    if (MM == 2) {
        if (*dd < 28)
            (*dd)++;
        else
            *dd = 1;
    } else if (MM == 1 || MM == 3 || MM == 5 || MM == 7 || MM == 8 || MM == 10
            || MM == 12) {
        if (*dd < 31)
            (*dd)++;
        else
            *dd = 1;
    } else {
        if (*dd < 30)
            (*dd)++;
        else
            *dd = 1;
    }
}

void decrease_date(uint8_t* dd, uint8_t MM) {
    if (MM == 2) {
        if (*dd > 1)
            (*dd)--;
        else
            *dd = 28;
    } else if (MM == 1 || MM == 3 || MM == 5 || MM == 7 || MM == 8 || MM == 10 || MM == 12) {
        if (*dd > 1)
            (*dd)--;
        else
            *dd = 31;
    } else {
        if (*dd > 1)
            (*dd)--;
        else
            *dd = 30;
    }
}

void increase_month(uint8_t* MM) {
    if (*MM < 12)
        (*MM)++;
    else
        *MM = 1;
}

void decrease_month(uint8_t* MM) {
    if (*MM > 1)
        (*MM)--;
    else
        *MM = 12;
}

void increase_year(uint8_t* yy) {
    *yy = (*yy + 1) % 100;
}

void decrease_year(uint8_t* yy) {
    if (*yy > 0)
        (*yy)--;
    else
        *yy = 99;
}

void convert_12_to_24(uint8_t* hh, uint8_t* pm) {
    if (*hh < 12) {
        if (*hh == 0) *hh = 12;
        *pm = 0;
    } else {
        if (*hh != 12) *hh = *hh - 12;
        *pm = 1;
    }
}

void convert_24_to_12(uint8_t* hh, uint8_t* pm) {
    if (*pm && *hh != 12) *hh = *hh + 12;
    else if (*hh == 12) *hh = 0;
}

void toggle_12_24(uint8_t* hh, uint8_t* pm, uint8_t* alarm_hh, uint8_t* alarm_pm, uint8_t* is_12) {
    if (*is_12) {
        convert_12_to_24(hh, pm);
        convert_12_to_24(alarm_hh, alarm_pm);
        *is_12 = 0;
    } else {
        convert_24_to_12(hh, pm);
        convert_24_to_12(alarm_hh, alarm_pm);
        *is_12 = 1;
    }
}

void toggle_alarm_on_off(uint8_t* on) {
    *on = *on ^ 1;
}

void copy_time_fields(Time* src_time, Time* dest_time) {
    dest_time->hh = src_time->hh;
    dest_time->is_12 = src_time->is_12;
    dest_time->pm = src_time->pm;

    dest_time->mm = src_time->mm;
    dest_time->ss = src_time->ss;
}

void copy_date_fields(Date* src_date, Date* dest_date) {
    dest_date->day = src_date->day;

    dest_date->dd = src_date->dd;

    dest_date->MM = src_date->MM;

    dest_date->yy = src_date->yy;
}
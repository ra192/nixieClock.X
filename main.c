/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC16F1936
        Driver Version    :  2.00
 */

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
 */

#include "mcc_generated_files/mcc.h"
#include "settings.h"
#include "time.h"
#include "nixie.h"
#include "button.h"
#include "neopixel.h"
#include "dekatron.h"
#include "buzzer.h"

#define REFRESH_PRESCALER (TICKS_FREQ / REFRESH_FREQ)

#define DISPLAY_DATE_DURATION 5 * TICKS_FREQ
#define DISPLAY_TEMP_DURATION 5 * TICKS_FREQ

#define DATAEE_DISPLAY_MODE_ADDR 0x08

#define DATAEE_DEK_MODE_ADDR 0x09

#define DATAEE_LED_MODE_ADDR 0x10

#define DATAEE_LED_COLOUR_FIXED_ANGEL_1 0x11
#define DATAEE_LED_COLOUR_FIXED_ANGEL_2 0x12

#define LED_RAINBOW_PRESC (TICKS_FREQ / 50)

#define DATAEE_ALARM_MELODY_ADDR 0x14

#define BUZZER_REFRESH_PRESCALER (TICKS_FREQ / BUZZER_REFRESH_FREQ)

typedef enum State {
    DISPLAY_TIME,
    DISPLAY_DATE,
    DISPLAY_YEAR,
    DISPLAY_TEMP,
    SET_HH,
    SET_MM,
    SET_12_24,
    SET_AM_PM,
    SET_DD,
    SET_MONTH,
    SET_YY,
    SET_DISPLAY_MODE,
    SET_ALARM_HH,
    SET_ALARM_MM,
    SET_ALARM_AM_PM,
    SET_ALARM_ON_OFF,
    SET_ALARM_MELODY
} State;

typedef enum DisplayMode {
    TIME_ONLY,
    TIME_AND_DATE,
    TIME_DATE_AND_TEMP
} DisplayMode;

typedef enum LedState {
    LED_OFF,
    LED_RAINBOW,
    LED_FIXED
} LedState;

volatile uint8_t timer_ticked = 0;
volatile uint16_t timer_count = 0;

Time time;
Time updated_time;

Date date;
Date updated_date;

Alarm alarm;

Temp temp;

Button btn1;
Button btn2;
Button btn3;

State state = DISPLAY_TIME;

uint16_t displayed_ticks;

DisplayMode display_mode;

LedState led_state;

uint16_t rainbow_angle;

uint8_t alarm_melody;

DekMode dek_mode;

void display_temp(void);

void change_rainbow_colour(void);

void tmr1_ISR(void) {
    timer_ticked = 1;
    if (timer_count == TICKS_FREQ - 1) {
        timer_count = 0;
    } else {
        timer_count++;
    }
}

void read_buttons() {
    read_button(&btn1, BTN_1_GetValue());
    read_button(&btn2, BTN_2_GetValue());
    read_button(&btn3, BTN_3_GetValue());
}

void set_time_digits(void) {
    set_digits(time.hh / 10, time.hh % 10, time.mm / 10, time.mm % 10);
}

void set_hh_digits(void) {
    toggle(updated_time.hh / 10, updated_time.hh % 10, updated_time.mm / 10, updated_time.mm % 10, 1, 1, 0, 0);
}

void set_mm_digits(void) {
    toggle(updated_time.hh / 10, updated_time.hh % 10, updated_time.mm / 10, updated_time.mm % 10, 0, 0, 1, 1);
}

void set_dd_digits(void) {
    toggle(updated_date.dd / 10, updated_date.dd % 10, updated_date.MM / 10, updated_date.MM % 10, 1, 1, 0, 0);
}

void set_MM_digits(void) {
    toggle(updated_date.dd / 10, updated_date.dd % 10, updated_date.MM / 10, updated_date.MM % 10, 0, 0, 1, 1);
}

void set_yy_digits(void) {
    toggle(2, 0, updated_date.yy / 10, updated_date.yy % 10, 0, 0, 1, 1);
}

void set_12_24_digits(void) {
    if (updated_time.is_12) {
        toggle(1, 2, 0, updated_time.pm, 1, 1, 0, 0);
    } else {
        toggle(2, 4, 0, 0, 1, 1, 0, 0);
    }
}

void set_am_pm_digits(void) {
    toggle(1, 2, 0, updated_time.pm, 0, 0, 1, 1);
}

void set_display_mode_digits(void) {
    toggle(0, display_mode, 0, 0, 1, 1, 0, 0);
}

void set_alarm_hh_digits(void) {
    toggle(alarm.hh / 10, alarm.hh % 10, alarm.mm / 10, alarm.mm % 10, 1, 1, 0, 0);
}

void set_alarm_mm_digits(void) {
    toggle(alarm.hh / 10, alarm.hh % 10, alarm.mm / 10, alarm.mm % 10, 0, 0, 1, 1);
}

void set_alarm_am_pm_digits(void) {
    toggle(1, 2, 0, alarm.pm, 0, 0, 1, 1);
}

void set_alarm_on_off_digits(void) {
    toggle(0, alarm.on, 0, alarm_melody, 1, 1, 0, 0);
}

void set_alarm_melody_digits(void) {
    toggle(0, alarm.on, 0, alarm_melody, 0, 0, 1, 1);
}

void flip_time(void) {
    flip_all(time.hh / 10, time.hh % 10, time.mm / 10, time.mm % 10);
}

void flip_date(void) {
    flip_seq(date.dd / 10, date.dd % 10, date.MM / 10, date.MM % 10);
}

void flip_year(void) {
    flip_seq(2, 0, date.yy / 10, date.yy % 10);
}

uint8_t celsius_to_farenheit(uint8_t cel_int_part, uint8_t cel_fract_part) {
    uint8_t farenh = (cel_int_part * 9 + 3) / 5 + 32;
    if (cel_fract_part > 25) farenh++;
    return farenh;
}

void shift_temp(void) {
    if (time.is_12) {
        uint8_t temp_in_farenh = celsius_to_farenheit(temp.int_part, temp.fract_part);
        shift(0, temp_in_farenh / 100, temp_in_farenh / 10, temp_in_farenh % 10);
    } else {
        shift(temp.int_part / 10, temp.int_part % 10, temp.fract_part / 10, temp.fract_part % 10);
    }
}

void change_dek_mode(void) {
    switch (dek_mode) {
        case DISPLAY_VAL:
            dek_mode = DISPLAY_WITH_SPIN_CW;
            break;
        case DISPLAY_WITH_SPIN_CW:
            dek_mode = DISPLAY_WITH_SPIN_CCW;
            break;
        case DISPLAY_WITH_SPIN_CCW:
            dek_mode = RING;
            break;
        default:
            dek_mode = DISPLAY_VAL;
    }

    dek_set_mode(dek_mode);
    DATAEE_WriteByte(DATAEE_DEK_MODE_ADDR, dek_mode);
}

void set_led_state(void) {
    switch (led_state) {
        case LED_RAINBOW:
            rainbow_angle = 0;
            set_leds_colour_by_angle(rainbow_angle);
            break;
        case LED_FIXED:
            set_leds_colour_by_angle(rainbow_angle);
            break;
        default:
            led_off();
            led_state = LED_OFF;
    }
}

void change_led_state(void) {
    switch (led_state) {
        case LED_OFF:
            led_state = LED_RAINBOW;
            break;
        case LED_RAINBOW:
            led_state = LED_FIXED;
            DATAEE_WriteByte(DATAEE_LED_COLOUR_FIXED_ANGEL_1, rainbow_angle >> 8);
            DATAEE_WriteByte(DATAEE_LED_COLOUR_FIXED_ANGEL_2, rainbow_angle & 0xFF);
            break;
        case LED_FIXED:
            led_state = LED_OFF;
    }
    set_led_state();
    DATAEE_WriteByte(DATAEE_LED_MODE_ADDR, led_state);
}

void change_rainbow_colour(void) {
    rainbow_angle = (rainbow_angle + 1) % 360;
    set_leds_colour_by_angle(rainbow_angle);
}

void handle_display_time(void) {
    if (btn1.state == PRESSED) {
        change_led_state();
    } else if (btn2.state == PRESSED) {
        change_dek_mode();
    } else if (btn2.state == LONG_PRESSED) {
        copy_time_fields(&time, &updated_time);
        copy_date_fields(&date, &updated_date);
        set_hh_digits();
        state = SET_HH;
    } else if (btn3.state == PRESSED) {
        displayed_ticks = 0;
        state = DISPLAY_DATE;
        flip_date();
        dek_set_mode(SPIN_CW);
    } else if (time.mm % 5 == 0 && time.ss == 0) {
        switch (display_mode) {
            case TIME_AND_DATE:
            case TIME_DATE_AND_TEMP:
                displayed_ticks = 0;
                state = DISPLAY_DATE;
                flip_date();
                dek_set_mode(SPIN_CW);
                break;
            default:
                flip_time();
        }
    } else if (timer_count == 0) {
        set_time_digits();
        dek_set_val(time.ss);
    }
}

void display_time(void) {
    state = DISPLAY_TIME;
    set_time_digits();
    dek_set_mode(dek_mode);
}

void handle_display_date(void) {
    if (btn3.state == PRESSED) {
        display_temp();
    } else if (displayed_ticks == DISPLAY_DATE_DURATION) {
        state = DISPLAY_YEAR;
        flip_year();
        displayed_ticks = 0;
    }
    displayed_ticks++;
}

void handle_display_year(void) {
    if (btn3.state == PRESSED) {
        display_temp();
    } else if (displayed_ticks == DISPLAY_DATE_DURATION) {
        if (display_mode == TIME_DATE_AND_TEMP) {
            display_temp();
        } else {
            display_time();
        }
    }
    displayed_ticks++;
}

void display_temp(void) {
    read_temp(&temp);
    shift_temp();
    displayed_ticks = 0;
    state = DISPLAY_TEMP;
    dek_set_mode(SPIN_CCW);
}

void handle_display_temp(void) {
    if (btn3.state == PRESSED || displayed_ticks == DISPLAY_TEMP_DURATION) {
        display_time();
    }
    displayed_ticks++;
}

void handle_set_hour(void) {
    if (btn2.state == PRESSED) {
        state = SET_MM;
        set_mm_digits();
    } else if (btn1.state == PRESSED
            || (btn1.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
        decrease_hour(&updated_time.hh, &updated_time.pm, updated_time.is_12);
        set_hh_digits();
    } else if (btn3.state == PRESSED
            || (btn3.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
        increase_hour(&updated_time.hh, &updated_time.pm, updated_time.is_12);
        set_hh_digits();
    }
}

void handle_set_minute(void) {
    if (btn2.state == PRESSED) {
        state = SET_12_24;
        set_12_24_digits();
    } else if (btn1.state == PRESSED
            || (btn1.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
        decrease_minute(&updated_time.mm);
        set_mm_digits();
    } else if (btn3.state == PRESSED
            || (btn3.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
        increase_minute(&updated_time.mm);
        set_mm_digits();
    }
}

void save_time(void) {
    update_time(&updated_time);
    update_alarm(&alarm, updated_time.is_12);
    copy_time_fields(&updated_time, &time);
    set_dd_digits();
    state = SET_DD;
}

void handle_set_12_24() {
    if (btn2.state == PRESSED) {
        if (updated_time.is_12) {
            state = SET_AM_PM;
            set_am_pm_digits();
        } else {
            save_time();
        }
    } else if (btn1.state == PRESSED || btn3.state == PRESSED) {
        toggle_12_24(&updated_time.hh, &updated_time.pm, &alarm.hh, &alarm.pm, &updated_time.is_12);
        set_12_24_digits();
    }
}

void handle_set_am_pm() {
    if (btn2.state == PRESSED) {
        save_time();
    } else if (btn1.state == PRESSED || btn3.state == PRESSED) {
        updated_time.pm ^= 1;
        set_am_pm_digits();
    }
}

void handle_set_day(void) {
    if (btn2.state == PRESSED) {
        state = SET_MONTH;
        set_MM_digits();
    } else if (btn1.state == PRESSED
            || (btn1.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
        decrease_date(&updated_date.dd, updated_date.MM);
        set_dd_digits();
    } else if (btn3.state == PRESSED
            || (btn3.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
        increase_date(&updated_date.dd, updated_date.dd);
        set_dd_digits();
    }
}

void handle_set_month(void) {
    if (btn2.state == PRESSED) {
        set_yy_digits();
        state = SET_YY;
    } else if (btn1.state == PRESSED
            || (btn1.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
        decrease_month(&updated_date.MM);
        set_MM_digits();
    } else if (btn3.state == PRESSED
            || (btn3.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
        increase_month(&updated_date.MM);
        set_MM_digits();
    }
}

void handle_set_year(void) {
    if (btn2.state == PRESSED) {
        update_date(&updated_date);
        copy_date_fields(&updated_date, &date);
        state = SET_DISPLAY_MODE;
        set_display_mode_digits();
    } else if (btn1.state == PRESSED
            || (btn1.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
        decrease_year(&updated_date.yy);
        set_yy_digits();
    } else if (btn3.state == PRESSED
            || (btn3.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
        increase_year(&updated_date.yy);
        set_yy_digits();
    }
}

void handle_set_display_mode(void) {
    if (btn2.state == PRESSED) {
        DATAEE_WriteByte(DATAEE_DISPLAY_MODE_ADDR, display_mode);
        state = SET_ALARM_HH;
        set_alarm_hh_digits();
    } else if (btn1.state == PRESSED) {
        if (display_mode > 0)
            display_mode--;
        else
            display_mode = TIME_DATE_AND_TEMP;
        set_display_mode_digits();
    } else if (btn3.state == PRESSED) {
        if (display_mode < TIME_DATE_AND_TEMP)
            display_mode++;
        else
            display_mode = TIME_ONLY;
        set_display_mode_digits();
    }
}

void handle_set_alarm_hour(void) {
    if (btn2.state == PRESSED) {
        state = SET_ALARM_MM;
        set_alarm_mm_digits();
    } else if (btn1.state == PRESSED
            || (btn1.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
        decrease_hour(&alarm.hh, &alarm.pm, time.is_12);
        set_alarm_hh_digits();
    } else if (btn3.state == PRESSED
            || (btn3.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
        increase_hour(&alarm.hh, &alarm.pm, time.is_12);
        set_alarm_hh_digits();
    }
}

void handle_set_alarm_minute(void) {
    if (btn2.state == PRESSED) {
        if (time.is_12) {
            state = SET_ALARM_AM_PM;
            set_alarm_am_pm_digits();
        } else {
            state = SET_ALARM_ON_OFF;
            set_alarm_on_off_digits();
        }
    } else if (btn1.state == PRESSED
            || (btn1.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
        decrease_minute(&alarm.mm);
        set_alarm_mm_digits();
    } else if (btn3.state == PRESSED
            || (btn3.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
        increase_minute(&alarm.mm);
        set_alarm_mm_digits();
    }
}

void handle_set_alarm_am_pm() {
    if (btn2.state == PRESSED) {
        state = SET_ALARM_ON_OFF;
        set_alarm_on_off_digits();
    } else if (btn1.state == PRESSED || btn3.state == PRESSED) {
        alarm.pm ^= 1;
        set_alarm_am_pm_digits();
    }
}

void handle_set_alarm_on_off(void) {
    if (btn2.state == PRESSED) {
        state = SET_ALARM_MELODY;
        set_alarm_melody_digits();
    } else if (btn1.state == PRESSED || btn3.state == PRESSED) {
        toggle_alarm_on_off(&alarm.on);
        set_alarm_on_off_digits();
    }
}

void handle_set_alarm_melody(void) {
    if (btn2.state == PRESSED) {
        update_alarm(&alarm, time.is_12);
        DATAEE_WriteByte(DATAEE_ALARM_MELODY_ADDR, alarm_melody);
        state = DISPLAY_TIME;
        set_time_digits();
        dek_set_mode(dek_mode);
    } else if (btn1.state == PRESSED) {
        if (alarm_melody > 0)
            alarm_melody--;
        else
            alarm_melody = MELODY_COUNT - 1;
        set_alarm_melody_digits();
    } else if (btn3.state == PRESSED) {
        alarm_melody = (alarm_melody + 1) % MELODY_COUNT;
        set_alarm_melody_digits();
    }
}

void handle_alarm(void) {
    if (alarm.on && time.hh == alarm.hh && (time.is_12 == 0 || time.pm == alarm.pm)
            && time.mm == alarm.mm && time.ss == 0 && !buzzer_get_on()) {
        start_melody(alarm_melody);
    } else if (buzzer_get_on() && (btn1.state == PRESSED || btn2.state == PRESSED || btn3.state == PRESSED)) {
        buzzer_off();
    } else if (buzzer_get_on() && timer_count % BUZZER_REFRESH_PRESCALER == 0)
        refresh_buzzer();
}

void handle_state(void) {
    switch (state) {
        case DISPLAY_TIME:
            handle_display_time();
            break;
        case DISPLAY_DATE:
            handle_display_date();
            break;
        case DISPLAY_YEAR:
            handle_display_year();
            break;
        case DISPLAY_TEMP:
            handle_display_temp();
            break;
        case SET_HH:
            handle_set_hour();
            break;
        case SET_MM:
            handle_set_minute();
            break;
        case SET_12_24:
            handle_set_12_24();
            break;
        case SET_AM_PM:
            handle_set_am_pm();
            break;
        case SET_DD:
            handle_set_day();
            break;
        case SET_MONTH:
            handle_set_month();
            break;
        case SET_YY:
            handle_set_year();
            break;
        case SET_DISPLAY_MODE:
            handle_set_display_mode();
            break;
        case SET_ALARM_HH:
            handle_set_alarm_hour();
            break;
        case SET_ALARM_MM:
            handle_set_alarm_minute();
            break;
        case SET_ALARM_AM_PM:
            handle_set_alarm_am_pm();
            break;
        case SET_ALARM_ON_OFF:
            handle_set_alarm_on_off();
            break;
        case SET_ALARM_MELODY:
            handle_set_alarm_melody();
    }
}

/*
                         Main application
 */
void main(void) {
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    TMR1_SetInterruptHandler(tmr1_ISR);

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    read_time(&time);
    read_date(&date);
    read_alarm(&alarm);

    set_time_digits();

    dek_mode = DATAEE_ReadByte(DATAEE_DEK_MODE_ADDR);
    if (dek_mode > 5) dek_mode = DISPLAY_VAL;
    dek_set_mode(dek_mode);
    dek_set_val(time.ss % 30);

    display_mode = DATAEE_ReadByte(DATAEE_DISPLAY_MODE_ADDR);
    if (display_mode > TIME_DATE_AND_TEMP) display_mode = TIME_ONLY;

    led_state = DATAEE_ReadByte(DATAEE_LED_MODE_ADDR);
    if (led_state > LED_FIXED) led_state = 0;

    rainbow_angle = (uint16_t) (DATAEE_ReadByte(DATAEE_LED_COLOUR_FIXED_ANGEL_1) << 8
            | DATAEE_ReadByte(DATAEE_LED_COLOUR_FIXED_ANGEL_2));

    set_led_state();

    alarm_melody = DATAEE_ReadByte(DATAEE_ALARM_MELODY_ADDR);
    if (alarm_melody > MELODY_COUNT - 1) alarm_melody = 0;

    while (1) {
        if (timer_ticked) {
            if (timer_count % REFRESH_PRESCALER == 0) refresh_digits();
            refresh_dek();
            read_buttons();
            handle_state();
            handle_alarm();
            if (timer_count == 0) {
                read_time(&time);
                if (((time.hh == 0 && !time.is_12) || (time.hh == 12 && !time.pm && time.is_12))
                        && time.mm == 0 && time.ss == 0)
                    update_date(&date);
            }
            if (led_state == LED_RAINBOW && timer_count % LED_RAINBOW_PRESC == 0) change_rainbow_colour();
            timer_ticked = 0;
        }
    }
}
/**
 End of File
 */
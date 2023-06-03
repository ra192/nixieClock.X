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
#include "time.h"
#include "nixie.h"
#include "button.h"
#include "neopixel.h"
#include "dekatron.h"
#include "buzzer.h"

#define TICKS_FREQ 400

#define DISPLAY_DATE_DURATION 5 * TICKS_FREQ
#define DISPLAY_TEMP_DURATION 5 * TICKS_FREQ

#define SHIFT_EFFECT_FREQ TICKS_FREQ / 5

#define DATAEE_LED_MODE_ADDR 0x10

#define LED_RAINBOW_FREQ TICKS_FREQ / 50

#define DATAEE_ALARM_MELODY_ADDR 0x11

typedef enum State {
    DISPLAY_TIME,
    DISPLAY_DATE,
    DISPLAY_TEMP,
    SET_HH,
    SET_MM,
    SET_DD,
    SET_MONTH,
    SET_YY,
    SET_12_24,
    SET_ALARM_HH,
    SET_ALARM_MM,
    SET_ALARM_ON_OFF,
    SET_ALARM_MELODY
} State;

typedef enum LedState {
    LED_OFF,
    LED_RED,
    LED_GREEN,
    LED_BLUE,
    LED_RAINBOW_1,
    LED_RAINBOW_2
} LedState;

Colour OFF_COLOUR = {0, 0, 0};
Colour RED = {64, 0, 0};
Colour GREEN = {0, 64, 0};
Colour BLUE = {0, 0, 64};

volatile uint8_t timer_ticked = 0;
volatile uint16_t timer_count = 0;

uint8_t flip_num_arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

Time time;
Time updated_time;

Alarm alarm;

Temp temp;

Button btn1;
Button btn2;
Button btn3;

State state = DISPLAY_TIME;

uint16_t date_displayed_ticks;
uint16_t temp_displayed_ticks;

LedState led_state;

uint8_t rainbow_angle;

uint8_t alarm_melody;

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

void set_time_digits(Time* tm) {
    set_digits(tm->hh / 10, tm->hh % 10, tm->mm / 10, tm->mm % 10);
}

void set_date_digits(Time* tm) {
    set_digits(tm->dd / 10, tm->dd % 10, tm->MM / 10, tm->MM % 10);
}

void set_temp_digits(Temp* temp) {
    set_digits(temp->int_part / 10, temp->int_part % 10, temp->fract_part / 10, temp->fract_part % 10);
}

void set_year_digits(Time* tm) {
    set_digits(tm->yy / 10, tm->yy % 10, 0, 0);
}

void set_12_24_digits(Time* tm) {
    if (tm->is_12) {
        set_digits(1, 2, 0, 0);
    } else {
        set_digits(2, 4, 0, 0);
    }
}

void set_alarm_digits(Alarm* alarm) {
    set_digits(alarm->hh / 10, alarm->hh % 10, alarm->mm / 10, alarm->mm % 10);
}

void set_alarm_on_off_and_melody_digits(Alarm* alarm, uint8_t alarm_melody) {
    set_digits(0, alarm->on, 0, alarm_melody);
}

void flip_time(void) {
    if (timer_count % 10 == 0) {
        uint8_t i = (timer_count % 100) / 10;
        set_digits(flip_num_arr[i], flip_num_arr[i], flip_num_arr[i], flip_num_arr[i]);
    }
}

void flip_date(void) {
    if (date_displayed_ticks % 10 == 0) {
        uint8_t dig_num = (uint8_t) (date_displayed_ticks / 100);
        uint8_t i = (date_displayed_ticks % 100) / 10;
        switch (dig_num) {
            case 0:
                set_digits(flip_num_arr[i], time.hh % 10, time.mm / 10, time.mm % 10);
                break;

            case 1:
                set_digits(time.dd / 10, flip_num_arr[i], time.mm / 10, time.mm % 10);
                break;

            case 2:
                set_digits(time.dd / 10, time.dd % 10, flip_num_arr[i], time.mm % 10);
                break;

            case 3:
                set_digits(time.dd / 10, time.dd % 10, time.MM / 10, flip_num_arr[i]);
        }
    }
}

void shift_temp(void) {
    if (temp_displayed_ticks % (SHIFT_EFFECT_FREQ) == 0) {
        switch (temp_displayed_ticks / (SHIFT_EFFECT_FREQ)) {
            case 0:
                set_digits(time.hh % 10, time.mm / 10, time.mm % 10, 0);
                set_digit_displayed(1, 1, 1, 0);
                break;
            case 1:
                set_digits(time.mm / 10, time.mm % 10, 0, 0);
                set_digit_displayed(1, 1, 0, 0);
                break;
            case 2:
                set_digits(time.mm % 10, 0, 0, temp.int_part / 10);
                set_digit_displayed(1, 0, 0, 1);
                break;
            case 3:
                set_digits(0, 0, temp.int_part / 10, temp.int_part % 10);
                set_digit_displayed(0, 0, 1, 1);
                break;
            default:
                set_digits(0, temp.int_part / 10, temp.int_part % 10, temp.fract_part / 10);
                set_digit_displayed(0, 1, 1, 1);
        }
    }
}

void set_led_state(void) {
    switch (led_state) {
        case LED_RED:
            set_leds_colour(&RED);
            break;
        case LED_GREEN:
            set_leds_colour(&GREEN);
            break;
        case LED_BLUE:
            set_leds_colour(&BLUE);
            break;
        case LED_RAINBOW_1:
            rainbow_angle = 0;
            set_leds_colour_by_angle_1(rainbow_angle);
            break;
        case LED_RAINBOW_2:
            rainbow_angle = 0;
            set_leds_colour_by_angle_2(rainbow_angle);
            break;
        default:
            set_leds_colour(&OFF_COLOUR);
            led_state = LED_OFF;
    }
}

void change_led_state(void) {
    switch (led_state) {
        case LED_OFF:
            led_state = LED_RED;
            break;
        case LED_RED:
            led_state = LED_GREEN;
            break;
        case LED_GREEN:
            led_state = LED_BLUE;
            break;
        case LED_BLUE:
            led_state = LED_RAINBOW_1;
            break;
        case LED_RAINBOW_1:
            led_state = LED_RAINBOW_2;
            break;
        case LED_RAINBOW_2:
            led_state = LED_OFF;
    }
    set_led_state();
    DATAEE_WriteByte(DATAEE_LED_MODE_ADDR, led_state);
}

void change_rainbow_colour(void) {
    if (timer_count % (LED_RAINBOW_FREQ) == 0) {
        rainbow_angle = (rainbow_angle + 1) % 360;
        switch (led_state) {
            case LED_RAINBOW_1:
                set_leds_colour_by_angle_1(rainbow_angle);
                break;
            default:
                set_leds_colour_by_angle_2(rainbow_angle);
        }
    }
}

void refresh_dek(void) {
    switch (state) {
        case DISPLAY_DATE:
            if (timer_count % 8 == 0)
                dek_move_next();
            break;
        default:
            if (time.ss / 2 != dek_get_cat_num()) {
                dek_move_next();
            }
    }
}

void handle_display_time(void) {
    if (btn1.state == PRESSED) {
        date_displayed_ticks = 0;
        state = DISPLAY_DATE;
    } else if (btn2.state == PRESSED) {
        change_led_state();
    } else if (btn2.state == LONG_PRESSED) {
        copy_time_fields(&time, &updated_time);
        set_time_digits(&updated_time);
        state = SET_HH;
    } else if (btn3.state == PRESSED) {
        read_temp(&temp);
        set_temp_digits(&temp);
        temp_displayed_ticks = 0;
        state = DISPLAY_TEMP;
    } else if (time.mm % 10 == 0 && time.ss == 30) {
        flip_time();
    } else if (timer_count == 0) {
        set_time_digits(&time);
    }
}

void handle_display_date(void) {
    if (btn1.state == PRESSED || date_displayed_ticks == DISPLAY_DATE_DURATION) {
        state = DISPLAY_TIME;
        set_time_digits(&time);
    } else if (date_displayed_ticks < TICKS_FREQ) {
        flip_date();
    } else if (date_displayed_ticks == TICKS_FREQ) {
        set_date_digits(&time);
    }
    date_displayed_ticks++;
}

void handle_display_temp(void) {
    if (btn3.state == PRESSED || temp_displayed_ticks == DISPLAY_TEMP_DURATION) {
        state = DISPLAY_TIME;
        set_time_digits(&time);
    } else if (btn3.state == LONG_PRESSED) {
        state = SET_ALARM_HH;
        set_alarm_digits(&alarm);
    } else if (temp_displayed_ticks < TICKS_FREQ) {
        shift_temp();
    } else if (temp_displayed_ticks == TICKS_FREQ) {
        set_temp_digits(&temp);
    }
    temp_displayed_ticks++;
}

void handle_set_hour(void) {
    if (btn2.state == PRESSED) {
        state = SET_MM;
        set_digit_displayed_all();
    } else if (btn1.state == PRESSED
            || (btn1.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
        decrease_hour(&updated_time.hh, &updated_time.pm, updated_time.is_12);
        set_time_digits(&updated_time);
    } else if (btn3.state == PRESSED
            || (btn3.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
        increase_hour(&updated_time.hh, &updated_time.pm, updated_time.is_12);
        set_time_digits(&updated_time);
    } else if (timer_count == 0 || timer_count == TICKS_FREQ / 2) {
        toggle_digit_displayed(0);
        toggle_digit_displayed(1);
    }
}

void handle_set_minute(void) {
    if (btn2.state == PRESSED) {
        state = SET_DD;
        set_date_digits(&updated_time);
    } else if (btn1.state == PRESSED
            || (btn1.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
        decrease_minute(&updated_time.mm);
        set_time_digits(&updated_time);
    } else if (btn3.state == PRESSED
            || (btn3.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
        increase_minute(&updated_time.mm);
        set_time_digits(&updated_time);
    } else if (timer_count == 0 || timer_count == TICKS_FREQ / 2) {
        toggle_digit_displayed(2);
        toggle_digit_displayed(3);
    }
}

void handle_set_day(void) {
    if (btn2.state == PRESSED) {
        state = SET_MONTH;
        set_digit_displayed_all();
    } else {
        if (btn1.state == PRESSED
                || (btn1.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
            decrease_date(&updated_time.dd, updated_time.MM);
            set_date_digits(&updated_time);
        } else if (btn3.state == PRESSED
                || (btn3.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
            increase_date(&updated_time.dd, updated_time.dd);
            set_date_digits(&updated_time);
        }
    }
    if (timer_count == 0 || timer_count == TICKS_FREQ / 2) {
        toggle_digit_displayed(0);
        toggle_digit_displayed(1);
    }
}

void handle_set_month(void) {
    if (btn2.state == PRESSED) {
        set_year_digits(&updated_time);
        state = SET_YY;
    } else if (btn1.state == PRESSED
            || (btn1.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
        decrease_month(&updated_time.MM);
        set_date_digits(&updated_time);
    } else if (btn3.state == PRESSED
            || (btn3.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
        increase_month(&updated_time.MM);
        set_date_digits(&updated_time);
    } else if (timer_count == 0 || timer_count == TICKS_FREQ / 2) {
        toggle_digit_displayed(2);
        toggle_digit_displayed(3);
    }
}

void handle_set_year(void) {
    if (btn2.state == PRESSED) {
        set_12_24_digits(&updated_time);
        state = SET_12_24;
    } else if (btn1.state == PRESSED
            || (btn1.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
        decrease_year(&updated_time.yy);
        set_year_digits(&updated_time);
    } else if (btn3.state == PRESSED
            || (btn3.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
        increase_year(&updated_time.yy);
        set_year_digits(&updated_time);
    } else if (timer_count == 0 || timer_count == TICKS_FREQ / 2) {
        toggle_digit_displayed(0);
        toggle_digit_displayed(1);
    }
}

void handle_set_12_24() {
    if (btn2.state == PRESSED) {
        update_time(&updated_time);
        update_alarm(&alarm, updated_time.is_12);
        copy_time_fields(&updated_time, &time);
        set_time_digits(&time);
        state = DISPLAY_TIME;
    } else if (btn1.state == PRESSED || btn3.state == PRESSED) {

        toggle_12_24(&updated_time.hh, &updated_time.pm, &alarm.hh, &alarm.pm, &updated_time.is_12);
        set_12_24_digits(&updated_time);
    } else if (timer_count == 0 || timer_count == TICKS_FREQ / 2) {
        toggle_digit_displayed(0);
        toggle_digit_displayed(1);
    }
}

void handle_set_alarm_hour(void) {
    if (btn2.state == PRESSED) {
        state = SET_ALARM_MM;
        set_digit_displayed_all();
    } else if (btn1.state == PRESSED
            || (btn1.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
        decrease_hour(&alarm.hh, &alarm.pm, time.is_12);
        set_alarm_digits(&alarm);
    } else if (btn3.state == PRESSED
            || (btn3.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
        increase_hour(&alarm.hh, &alarm.pm, time.is_12);
        set_alarm_digits(&alarm);
    } else if (timer_count == 0 || timer_count == TICKS_FREQ / 2) {
        toggle_digit_displayed(0);
        toggle_digit_displayed(1);
    }
}

void handle_set_alarm_minute(void) {
    if (btn2.state == PRESSED) {
        state = SET_ALARM_ON_OFF;
        set_alarm_on_off_and_melody_digits(&alarm, alarm_melody);
    } else if (btn1.state == PRESSED
            || (btn1.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
        decrease_minute(&alarm.mm);
        set_alarm_digits(&alarm);
    } else if (btn3.state == PRESSED
            || (btn3.state == HOLD_LONG_PRESSED && timer_count % 10 == 0)) {
        increase_minute(&alarm.mm);
        set_alarm_digits(&alarm);
    } else if (timer_count == 0 || timer_count == TICKS_FREQ / 2) {
        toggle_digit_displayed(2);
        toggle_digit_displayed(3);
    }
}

void handle_set_alarm_on_off(void) {
    if (btn2.state == PRESSED) {
        state = SET_ALARM_MELODY;
        set_digit_displayed_all();
    } else if (btn1.state == PRESSED) {
        if (alarm_melody > 0)
            alarm_melody--;
        else
            alarm_melody = MELODY_COUNT - 1;
        set_alarm_on_off_and_melody_digits(&alarm, alarm_melody);
    } else if (btn3.state == PRESSED) {
        alarm_melody = (alarm_melody + 1) % MELODY_COUNT;
    } else if (timer_count == 0 || timer_count == TICKS_FREQ / 2) {
        toggle_digit_displayed(2);
        toggle_digit_displayed(3);
    }
}

void handle_set_alarm_melody(void) {
    if (btn2.state == PRESSED) {
        update_alarm(&alarm, time.is_12);
        DATAEE_WriteByte(DATAEE_ALARM_MELODY_ADDR, alarm_melody);
        state = DISPLAY_TIME;
        set_time_digits(&time);
    } else if (btn1.state == PRESSED || btn3.state == PRESSED) {
        toggle_alarm_on_off(&alarm.on);
        set_alarm_on_off_and_melody_digits(&alarm, alarm_melody);
    } else if (timer_count == 0 || timer_count == TICKS_FREQ / 2) {

        toggle_digit_displayed(0);
        toggle_digit_displayed(1);
    }
}

void handle_alarm(void) {
    if (alarm.on && time.hh == alarm.hh && (time.is_12 == 0 || time.pm == alarm.pm)
            && time.mm == alarm.mm && time.ss == alarm.ss && timer_count == 0) {
        start_melody(alarm_melody);
    } else if (buzzer_get_on() && (btn1.state == PRESSED || btn2.state == PRESSED || btn3.state == PRESSED)) {
        buzzer_off();
    } else if (buzzer_get_on())
        refresh_buzzer();
    //    if (btn1.state == PRESSED) {
    //        start_melody(alarm_melody);
    //    } else {
    //
    //        refresh_buzzer();
    //    }
}

void handle_state(void) {
    switch (state) {
        case DISPLAY_TIME:
            handle_display_time();
            break;
        case DISPLAY_DATE:
            handle_display_date();
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
        case SET_DD:
            handle_set_day();
            break;
        case SET_MONTH:
            handle_set_month();
            break;
        case SET_YY:
            handle_set_year();
            break;
        case SET_12_24:
            handle_set_12_24();
            break;
        case SET_ALARM_HH:
            handle_set_alarm_hour();
            break;
        case SET_ALARM_MM:
            handle_set_alarm_minute();
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
    read_alarm(&alarm);

    set_time_digits(&time);

    led_state = DATAEE_ReadByte(DATAEE_LED_MODE_ADDR);
    set_led_state();

    alarm_melody = DATAEE_ReadByte(DATAEE_ALARM_MELODY_ADDR);

    while (1) {
        if (timer_ticked) {
            refresh_digits();
            refresh_dek();
            read_buttons();
            handle_state();
            if (timer_count == 0) {
                read_time(&time);
            }
            if (led_state == LED_RAINBOW_1 || led_state == LED_RAINBOW_2) change_rainbow_colour();
            handle_alarm();
            timer_ticked = 0;
        }
    }
}
/**
 End of File
 */
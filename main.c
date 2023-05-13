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

#define TICKS_IN_SEC 400
#define DISPLAY_DATE_DURATION 5 * TICKS_IN_SEC

#define SHIFT_FRAME_DURATION TICKS_IN_SEC / 5

typedef enum State {
    DISPLAY_TIME,
    DISPLAY_DATE,
    SET_HH,
    SET_MM,
    SET_DD,
    SET_MONTH,
    SET_YY,
    SET_12_24
} State;

volatile uint8_t timer_ticked = 0;
volatile uint16_t timer_count = 0;

uint8_t flip_num_arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

Time time;
Time updated_time;

Button btn1;
Button btn2;
Button btn3;

State state = DISPLAY_TIME;

uint16_t date_displayed_ticks;

void tmr1_ISR(void) {
    timer_ticked = 1;
    if (timer_count == TICKS_IN_SEC - 1) {
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

void flip_time() {
    if (timer_count < TICKS_IN_SEC && timer_count % 10 == 0) {
        uint8_t i = (timer_count % 100) / 10;
        set_digits(flip_num_arr[i], flip_num_arr[i], flip_num_arr[i], flip_num_arr[i]);
    }
}

void flip_date() {
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

void handle_display_time(void) {
    if (btn2.state == LONG_PRESSED) {
        copy_time_fields(&time, &updated_time);
        set_time_digits(&updated_time);
        state = SET_HH;
    } else if (btn1.state == PRESSED) {
        date_displayed_ticks = 0;
        state = DISPLAY_DATE;
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
    } else if (date_displayed_ticks < TICKS_IN_SEC) {
        flip_date();
    } else if (date_displayed_ticks == TICKS_IN_SEC) {
        set_date_digits(&time);
    }
    date_displayed_ticks++;
}

void handle_set_hour(void) {
    if (btn2.state == PRESSED) {
        state = SET_MM;
        set_digit_displayed_all();
    } else if (btn1.state == PRESSED) {
        decrease_hour(&updated_time);
        set_time_digits(&updated_time);
    } else if (btn3.state == PRESSED) {
        increase_hour(&updated_time);
        set_time_digits(&updated_time);
    } else if (timer_count == 0 || timer_count == TICKS_IN_SEC / 2) {
        toggle_digit_displayed(0);
        toggle_digit_displayed(1);
    }
}

void handle_set_minute(void) {
    if (btn2.state == PRESSED) {
        state = SET_DD;
        set_date_digits(&updated_time);
    } else if (btn1.state == PRESSED) {
        decrease_minute(&updated_time);
        set_time_digits(&updated_time);
    } else if (btn3.state == PRESSED) {
        increase_minute(&updated_time);
        set_time_digits(&updated_time);
    } else if (timer_count == 0 || timer_count == TICKS_IN_SEC / 2) {
        toggle_digit_displayed(2);
        toggle_digit_displayed(3);
    }
}

void handle_set_day(void) {
    if (btn2.state == PRESSED) {
        state = SET_MONTH;
        set_digit_displayed_all();
    } else {
        if (btn1.state == PRESSED) {
            decrease_date(&updated_time);
            set_date_digits(&updated_time);
        } else if (btn3.state == PRESSED) {
            increase_date(&updated_time);
            set_date_digits(&updated_time);
        }
    }
    if (timer_count == 0 || timer_count == TICKS_IN_SEC / 2) {
        toggle_digit_displayed(0);
        toggle_digit_displayed(1);
    }
}

void handle_set_month(void) {
    if (btn2.state == PRESSED) {
        set_year_digits(&updated_time);
        state = SET_YY;
    } else if (btn1.state == PRESSED) {
        decrease_month(&updated_time);
        set_date_digits(&updated_time);
    } else if (btn3.state == PRESSED) {
        increase_month(&updated_time);
        set_date_digits(&updated_time);
    } else if (timer_count == 0 || timer_count == TICKS_IN_SEC / 2) {
        toggle_digit_displayed(2);
        toggle_digit_displayed(3);
    }
}

void handle_set_year(void) {
    if (btn2.state == PRESSED) {
        set_12_24_digits(&updated_time);
        state = SET_12_24;
    } else if (btn1.state == PRESSED) {
        decrease_year(&updated_time);
        set_year_digits(&updated_time);
    } else if (btn3.state == PRESSED) {
        increase_year(&updated_time);
        set_year_digits(&updated_time);
    } else if (timer_count == 0 || timer_count == TICKS_IN_SEC / 2) {
        toggle_digit_displayed(0);
        toggle_digit_displayed(1);
    }
}

void handle_set_12_24() {
    if (btn2.state == PRESSED) {
        update_time(&updated_time);
        copy_time_fields(&updated_time, &time);
        set_time_digits(&time);
        state = DISPLAY_TIME;
    } else if (btn1.state == PRESSED || btn3.state == PRESSED) {
        toggle_12_24(&updated_time);
        set_12_24_digits(&updated_time);
    } else if (timer_count == 0 || timer_count == TICKS_IN_SEC / 2) {
        toggle_digit_displayed(0);
        toggle_digit_displayed(1);
    }
}

void handle_state(void) {
    switch (state) {
        case DISPLAY_TIME:
            handle_display_time();
            break;
        case DISPLAY_DATE:
            handle_display_date();
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
    set_time_digits(&time);

    while (1) {
        if (timer_ticked) {
            refresh_digits();
            read_buttons();
            if (timer_count == 0) read_time(&time);
            handle_state();
            timer_ticked = 0;
        }
    }
}
/**
 End of File
 */
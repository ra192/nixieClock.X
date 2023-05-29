/* 
 * File:   time.h
 * Author: yak19
 *
 * Created on May 7, 2023, 6:40 PM
 */

#ifndef TIME_H
#define	TIME_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>

    typedef struct Time {
        uint8_t hh;

        uint8_t is_12;
        uint8_t pm;

        uint8_t mm;

        uint8_t ss;

        uint8_t day;

        uint8_t dd;

        uint8_t MM;

        uint8_t yy;
    } Time;

    typedef struct Alarm {
        uint8_t hh;
        uint8_t pm;

        uint8_t mm;

        uint8_t ss;

        uint8_t dy_dt;

        uint8_t dd;
        
        uint8_t on;
    } Alarm;

    typedef struct Temp {
        uint8_t int_part;
        uint8_t fract_part;
    } Temp;

    void read_time(Time* time);

    void read_alarm(Alarm* alarm);

    void read_temp(Temp* temp);

    void update_time(Time* time);

    void update_alarm(Alarm* alarm, uint8_t is_12);

    void increase_hour(uint8_t* hh, uint8_t *pm, uint8_t is_12);

    void decrease_hour(uint8_t* hh, uint8_t *pm, uint8_t is_12);

    void increase_minute(uint8_t* mm);

    void decrease_minute(uint8_t* mm);

    void increase_date(uint8_t* dd, uint8_t MM);

    void decrease_date(uint8_t* dd, uint8_t MM);

    void increase_month(uint8_t* MM);

    void decrease_month(uint8_t* MM);

    void increase_year(uint8_t* yy);

    void decrease_year(uint8_t* yy);

    void toggle_12_24(uint8_t* hh, uint8_t* pm, uint8_t* alarm_hh, uint8_t* alarm_pm, uint8_t* is_12);
    
    void toggle_alarm_on_off(uint8_t* on);

    void copy_time_fields(Time* src_time, Time* dest_time);


#ifdef	__cplusplus
}
#endif

#endif	/* TIME_H */


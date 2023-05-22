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

        uint8_t al1_hh;

        uint8_t al1_mm;

        uint8_t al1_ss;

        uint8_t al1_dy_dt;

        uint8_t al1_dd;

    } Time;

    typedef struct Alarm {
        uint8_t hh;

        uint8_t mm;

        uint8_t ss;

        uint8_t dy_dt;

        uint8_t dd;
    } Alarm;
    
    typedef struct Temp {
        uint8_t int_part;
        uint8_t fract_part;
    } Temp;
    
    void read_time(Time* time);
    
    void read_alarm(Alarm* alarm);
    
    void read_temp(Temp* temp);

    void update_time(Time* time);

    void increase_hour(Time* time);

    void decrease_hour(Time* time);

    void increase_minute(Time* time);

    void decrease_minute(Time* time);

    void increase_date(Time* time);

    void decrease_date(Time* time);

    void increase_month(Time* time);

    void decrease_month(Time* time);

    void increase_year(Time* time);

    void decrease_year(Time* time);

    void toggle_12_24(Time* time);

    void copy_time_fields(Time* src_time, Time* dest_time);


#ifdef	__cplusplus
}
#endif

#endif	/* TIME_H */


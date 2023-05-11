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

    void read_time(Time* time);

    void update_time(Time* time);

    void increase_hour(Time* time);

    void decrease_hour(Time* time);

    void increase_minute(Time* time);

    void decrease_minute(Time* time);

    void increase_date(Time* time);

    void decrease_date(Time* time);

    void increase_month(Time* time);

    void decrease_month(Time* time);

    void copy_time_fields(Time* src_time, Time* dest_time);
    

#ifdef	__cplusplus
}
#endif

#endif	/* TIME_H */


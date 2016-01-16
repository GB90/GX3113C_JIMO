#ifndef __GX_RTC_TIMER_H_
#define __GX_RTC_TIMER_H_

struct gx_rtc_registers
{
    unsigned long GX3110_RTC_CONTROL;
    unsigned long GX3110_RTC_IER;
    unsigned long GX3110_RTC_ISR;
    unsigned long GX3110_RTC_DIV_PRESCALE;
    unsigned long GX3110_RTC_TICK_COUNT;
    unsigned long GX3110_RTC_ALARM1_MASK_CONTROL;
    unsigned long GX3110_RTC_ALARM1_US;
    unsigned long GX3110_RTC_ALARM1_MS;
    unsigned long GX3110_RTC_ALARM1_SECOND;
    unsigned long GX3110_RTC_ALARM1_MINUTE;
    unsigned long GX3110_RTC_ALARM1_HOUR;
    unsigned long GX3110_RTC_ALARM1_WEEK;
    unsigned long GX3110_RTC_ALARM1_DATE;
    unsigned long GX3110_RTC_ALARM1_MONTH;
    unsigned long GX3110_RTC_ALARM1_YEAR;
    unsigned long GX3110_RTC_ALARM2_MASK_CONTROL;
    unsigned long GX3110_RTC_ALARM2_US;
    unsigned long GX3110_RTC_ALARM2_MS;
    unsigned long GX3110_RTC_ALARM2_SECOND;
    unsigned long GX3110_RTC_ALARM2_MINUTE;
    unsigned long GX3110_RTC_ALARM2_HOUR;
    unsigned long GX3110_RTC_ALARM2_WEEK;
    unsigned long GX3110_RTC_ALARM2_DATE;
    unsigned long GX3110_RTC_ALARM2_MONTH;
    unsigned long GX3110_RTC_ALARM2_YEAR; /* maybe unavailable */
    unsigned long GX3110_RTC_TIME_US;
    unsigned long GX3110_RTC_TIME_MS;
    unsigned long GX3110_RTC_TIME_SECOND;
    unsigned long GX3110_RTC_TIME_MINUTE;
    unsigned long GX3110_RTC_TIME_HOUR;
    unsigned long GX3110_RTC_TIME_WEEK;
    unsigned long GX3110_RTC_TIME_DATE;
    unsigned long GX3110_RTC_TIME_MONTH;
    unsigned long GX3110_RTC_TIME_YEAR;
};

typedef void (*gx_rtc_timer_callback)(unsigned long data);

struct gx_rtc_timer_list
{
    struct gx_rtc_timer_list* next;
    int id; /* the unique number for this timer */
    gx_rtc_timer_callback callback;
    unsigned long data; /* argument for callback */
    unsigned long trigger;
    unsigned long interval;
};

/* create one timer, trigger and interval is in MS */
/* callback: user timer callback, data: callback argument(maybe pointer) */
/* trigger: the first triggering time */
/* interval: the latter triggering interval, if you define this as 0, your timer is 'one-time' */
int gx_rtc_timer_create(gx_rtc_timer_callback callback, unsigned long data, unsigned long trigger, unsigned long interval);
int gx_rtc_timer_delete(int timer_id);

#endif


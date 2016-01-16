/* Define to prevent recursive inclusion */
#ifndef __RTC_H__
#define __RTC_H__
/* Cooperation with C and C++ */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef FALSE
#define FALSE 0
#define TRUE  1
#endif

typedef enum RTC_ClockFrequency_e
{
	RTC_CLOCK_4MHZ			= 40,
	RTC_CLOCK_8MHZ  		= 60,
	RTC_CLOCK_13_5MHZ  		= 135,
	RTC_CLOCK_16MHZ 		= 160,
	RTC_CLOCK_27MHZ  		= 270
}RTC_ClockFrequency_t;
typedef struct RtcDateTime
{
	unsigned int		TmYear;				// ��,BCD��
	unsigned short		TmMonth;			// ��,BCD��
	unsigned short		TmDay;				// ��,BCD��
	unsigned short		TmWeek;				// ��,BCD��
	unsigned short		TmHour;				// ʱ,BCD��
	unsigned short		TmMin;				// ��,BCD��
	unsigned short		TmSec;				// ��,BCD��
	unsigned short		TmMs;				// ����,DECֵ
	unsigned short		TmUs;				// ΢��,DECֵ
}gx_time_t;

extern void gx_get_rtc_time_ms(unsigned int *cur_time);
extern void gx_rtc_init(
			  unsigned int CtrIntEnable,
			  unsigned int Alarm1IntEnable,
			  unsigned int Alarm2IntEnable);
void gx_rtc_time_write(gx_time_t *DateTime);
void gx_rtc_time_read(gx_time_t *DateTime);

#ifdef __cplusplus
}
#endif

#endif

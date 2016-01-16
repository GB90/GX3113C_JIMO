/**
 *
 * @file        app_common_epg.h
 * @brief
 * @version     1.1.0
 * @date        10/19/2012 15:17:49 PM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_COMMON_EPG__H__
#define __APP_COMMON_EPG__H__
#ifdef __cplusplus
extern "C" {
#endif

#include "gxepg.h"

typedef struct time_s
{
    unsigned int year    : 12;
    unsigned int month   : 4;
    unsigned int day     : 5;
    unsigned int hour    : 5;
    unsigned int minute  : 6;
} com_time_t;



typedef struct
{
	 uint8_t gApp_epg_enable ;
	 int32_t msg_get_epg_id ;
	 int32_t msg_get_epg_num_id ;
	 uint32_t sEpg_day ;
	 uint32_t cur_event_count ;
	 uint32_t sOffSetSel ;
	 uint32_t sOldOffset ;
	 int get_event_count ;
}epg_get_para;

/*
* 以下接口提供menu调用
*/


/*
* 开启epg过滤
*/
void app_epg_open(void);

/*
* 关闭epg过滤
*/
void app_epg_close(void);

/*
* 设置系统时间,
*/
void app_epg_set_local_time(struct tm *pTime,uint32_t timezoneFlag);

/*
* 获取当前天之后day天的年/月/日/星期等信息
*/
void app_epg_get_time_by_day_offset(struct tm *pLocalTime,int day,uint32_t timezoneFlag);

/*
* 获取当前时间信息
*/
void app_epg_get_local_time(struct tm *pLocalTime,uint32_t timezoneFlag);

/*
* 退出显示EPG信息界面 (如信息条、节目列表、EPG)
* 等界面中调用
*/
void app_epg_free(void);

/*
* 获取p/f epg信息
*/
GxEpgInfo * app_epg_get_pf_event(uint32_t pos);


/*
* 更新选中节目某天的EPG信息(返回当前选中行EPG)
*/
GxEpgInfo * app_epg_update_event_by_day(uint32_t value,int32_t nListSel,uint32_t checkflag);

/*
* 获取某天对应索引的EPG信息
*/
GxEpgInfo *  app_epg_get_event_info_by_sel(uint32_t nListSel);

/*
* 读取一天EPG个数
*/
uint32_t app_epg_get_event_count(void);

/*
* 日期轮循切换
*/
uint32_t app_epg_switch_day_offset(void);

/*
* 获取EPG信息参数
*/
epg_get_para* app_epg_get_para(void);

/*
* 重置EPG系统参数
*/
void app_epg_reset_get_para(void);

/*
* 同步更新当前时间
*/
void app_epg_sync_time(void);

/*
* epg信息中获取开始、结束时间
*/
void app_epg_get_event_time(GxEpgInfo *epg_info_temp,struct tm *tm_start,struct tm *tm_end);

/*
* mjd日期转化年月日格式
*/
void  app_epg_convert_mjd_to_ymd( uint16_t   mjd, uint16_t   *year, uint8_t  *month, uint8_t  *day, uint8_t  *weekDay );

/*
* 年月日转化mjd格式
*/
void  app_epg_convert_ymd_to_mjd( uint16_t   *mjd, uint16_t   year, uint8_t  month, uint8_t  day);

/*
* 日期转化UTC
*/
void app_epg_convert_to_utc( unsigned short wDate, com_time_t* pTimeOut );



#ifdef __cplusplus
}
#endif
#endif /*__APP_COMMON_EPG__H__*/


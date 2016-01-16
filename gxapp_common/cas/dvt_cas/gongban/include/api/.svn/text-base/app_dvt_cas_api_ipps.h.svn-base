/**
 *
 * @file        app_dvt_cas_api_ipps.h
 * @brief
 * @version     1.1.0
 * @date        
 * @author      
 *
 */
#ifndef __APP_DVT_CAS_API_IPPS_H__
#define __APP_DVT_CAS_API_IPPS_H__
#ifdef __cplusplus
extern "C" {
#endif

//#include "app_common_play.h"
#include "DVTCAS_STBInf.h"
#include "app_common_porting_stb_api.h"
#include "app_dvt_cas_api_pop.h"

int32_t app_dvt_cas_book_ipps_init(void);

int32_t app_dvt_cas_book_ipps_get_totalcount(void);

char *app_dvt_cas_book_ipps_get_progname(int progindex);//节目名称

BYTE app_dvt_cas_book_ipps_get_bookstatus(int progindex);//订购状态

BYTE app_dvt_cas_book_ipps_get_type(int progindex);//IPP节目类型


DWORD app_dvt_cas_book_ipps_get_starttime(int progindex);//开始时间


DWORD app_dvt_cas_book_ipps_get_duration(int progindex);//持续时长(秒)

char* app_dvt_cas_book_ipps_get_servicename(int progindex);//所在业务名

WORD app_dvt_cas_book_ipps_get_bookedprice(int progindex);//已经订购价格(分)

BYTE app_dvt_cas_book_ipps_get_bookedinterval(int progindex);//收费间隔


BYTE app_dvt_cas_book_ipps_get_bookedpricetype(int progindex);//当前收费间隔


BYTE app_dvt_cas_book_ipps_get_unit(int progindex);//收费间隔时间单位

DWORD app_dvt_cas_book_ipps_get_curprice(int progindex);//当前价格(分)


BYTE app_dvt_cas_book_ipps_get_curinterval(int progindex);//当前收费间隔
SDVTCAIpp app_dvt_cas_book_ipps_get_curIPPs(int progindex);//当前IPP


/////////////////////////////////////////////////////////////////////////
int32_t app_dvt_cas_viewed_ipps_init(void);
int32_t app_dvt_cas_viewed_ipps_get_totalcount(void);
char *app_dvt_cas_viewed_ipps_get_tvs_name(int progindex);//运营商名称

DWORD app_dvt_cas_viewed_ipps_get_starttime(int progindex);//开始时间
DWORD app_dvt_cas_viewed_ipps_get_duration(int progindex);//持续时长

BYTE app_dvt_cas_viewed_ipps_get_interval(int progindex);//收费间隔


BYTE app_dvt_cas_viewed_ipps_get_pricetype(int progindex);//价格类型


BYTE app_dvt_cas_viewed_ipps_get_unit(int progindex);//收费单位

char* app_dvt_cas_viewed_ipps_get_progname(int progindex);//产品名称

char* app_dvt_cas_viewed_ipps_get_otherinfo(int progindex);//其它信息




#ifdef __cplusplus
}
#endif
#endif /*__APP_DVT_CAS_API_IPPS_H__*/


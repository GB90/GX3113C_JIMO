/**
 *
 * @file        app_wf_cas_api_baseinfo.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_WFCAS_API_BASEINFO_H__
#define __APP_WFCAS_API_BASEINFO_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

typedef enum 
{
	WF_BASEINFO_CARDID_ID=0,
	WF_BASEINFO_PURSEINFO_ID,
	WF_BASEINFO_STBNO_ID,
	WF_BASEINFO_CASID_ID,
	WF_BASEINFO_CASVER_ID,	
	WF_BASEINFO_CASPRO_ID,
	WF_BASEINFO_CASDES_ID,
	WF_BASEINFO_CARDVALID_ID,
	WF_BASEINFO_DATATIME_ID
}WFCA_BASEIINFO_ID_E;

typedef 
struct CommonTime_s
{
	uint32_t		m_nYear : 12;
	uint32_t 		m_nMonth : 4;
	uint32_t		m_nDay : 5;
	uint32_t		m_nHour :5;
	uint32_t 		m_nMinute : 6;
	uint32_t		m_nSecond : 6;
	uint32_t		m_nReverse : 26;
} CommonTime_t;

int32_t app_wf_cas_api_init_baseinfo_data(void);
char * app_wf_cas_api_get_baseinfo_data(ca_get_date_t *data);


#ifdef __cplusplus
}
#endif
#endif /*__APP_WFCAS_API_BASEINFO_H__*/


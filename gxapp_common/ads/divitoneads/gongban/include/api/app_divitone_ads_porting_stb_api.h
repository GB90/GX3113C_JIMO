/**
 *
 * @file        app_divitone_ads_porting_stb_api.h
 * @brief
 * @version     1.1.0
 * @date        11/27/2013 
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_DIVITONE_ADS_PORTING_API_H__
#define __APP_DIVTIONE_ADS_PORTING_API_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <gxtype.h>
#include "gxbus.h"

typedef enum AD_DIVITONE_SHOW_TYPE
{
	AD_DIVITONE_TYPE_NONE=0,
	AD_DIVITONE_TYPE_LOGO ,
	AD_DIVITONE_TYPE_BAR,
	AD_DIVITONE_TYPE_VOLUME,
	AD_DIVITONE_TYPE_JIAO,
	AD_DIVITONE_TYPE_MENU,
	AD_DIVITONE_TYPE_LIST,
	AD_DIVITONE_TYPE_PIC_OSD,
	AD_DIVITONE_TYPE_TEXT_OSD,
	AD_DIVITONE_TYPE_SERVICE_OSD,
	AD_DIVITONE_TYPE_MAX
}AD_DIVITONE_SHOW_TYPE_T;
typedef struct
{
	char IconName[64];
	char* file_ddram_path; /*播放广告文件需要创建的内存文件名称，包含路径*/
	event_list* ad_timer;
	uint8_t isShown;
	uint8_t status;
}DIVITONE_AD_PIC_t;

private_parse_status ad_data_got(uint8_t* pSectionData, size_t Size);
int GxAD_LoadData(void) ;
void app_divitone_ads_play(char * IconName ,uint8_t type);
u_int8_t app_divione_ads_play_state(uint8_t type);
void app_divitone_ads_stop(char* IconName,uint8_t type);
void app_divitone_ads_logo_sleep(uint32_t ms);
void app_divitone_ads_show_av_logo(int32_t VideoResolution);

#ifdef __cplusplus
}
#endif
#endif /*__APP_3H_ADS_PORTING_API_H__*/


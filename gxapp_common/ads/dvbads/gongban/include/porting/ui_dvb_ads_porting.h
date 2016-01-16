#ifndef __UI_DVB_ADS_PORTING_H__
#define __UI_DVB_ADS_PORTING_H__

#include "dvb_ads_function_porting.h"


#define DVB_ADS_WIDGET_NAME_LEN			64

#define DVB_ADS_MAIN_MENU_PATH			"/mnt/main_menu.gif"
#define DVB_ADS_MAIN_MENU_CFG_PATH 		"/mnt/main_menu.cfg"

#define DVB_ADS_EPG_PATH			"/mnt/epg.gif"
#define DVB_ADS_EPG_CFG_PATH			"/mnt/epg.cfg"

#define DVB_ADS_VOLUME_PATH			"/mnt/volume.gif"
#define DVB_ADS_VOLUME_CFG_PATH			"/mnt/volume.cfg"

#define DVB_ADS_CHLIST_PATH			"/mnt/chlist.gif"
#define DVB_ADS_CHLIST_CFG_PATH			"/mnt/chlist.cfg"

#define DVB_ADS_CURRENT_FOLLOW_PATH		"/mnt/current_follow.gif"
#define DVB_ADS_CURRENT_FOLLOW_CFG_PATH		"/mnt/current_follow.cfg"

#define DVB_ADS_CHANNEL_INFO_PATH		"/mnt/channel_info.gif"
#define DVB_ADS_CHANNEL_INFO_CFG_PATH		"/mnt/channel_info.cfg"

typedef struct
{
  u32 tp_freq;
  u32 tp_sym;
  u32 nim_modulate;
} ad_lock_t;

// >> operation types of PIC 
typedef enum
{
	LOAD_PIC_DATA,
	SHOW_PIC_DATA,
	DRAW_PIC_DATA,
	HIDE_PIC_DATA,
	FREE_PIC_DATA,
}DVB_AD_PIC_OPT;

typedef struct
{
	dvbad_show_pic_info *pic_show_info;
	u8 IconName[DVB_ADS_WIDGET_NAME_LEN];
	u8* cfgPath;
	u8* filePath;
	event_list * ptrTimer;
	u8 isShown;
}dvb_ads_pic_t;

void dvb_ads_get_flash_bat_data(void);
RET_CODE dvb_ads_save_flash_bat_data(void);

// >> show the logo in file ,arg :the widget name
void dvb_ads_show_logo(int32_t VideoResolution);
// >> hidden the logo in file
// >> save logo data to flash file and the logo config data to flash address
RET_CODE dvb_ads_save_logo_data(void);
RET_CODE save_all_pic_to_dram(void);
// >> the following 12 funcs are for showing or hidding the AD pics by position of them
void dvb_ads_menu_pic_display(char*  IconeName);
void dvb_ads_hide_menu_pic(char*  IconeName);
void dvb_ads_epg_pic_display(char*  IconeName);
void dvb_ads_hide_epg_pic(char*  IconeName);
void dvb_ads_vol_pic_display(char*  IconeName);
void dvb_ads_hide_vol_pic(char*  IconeName);
void dvb_ads_chlist_pic_display(char*  IconeName);
void dvb_ads_hide_chlist_pic(char*  IconeName);
void dvb_ads_current_follow_pic_display(char*  IconeName);
void dvb_ads_hide_current_follow_pic(char*  IconeName);
void dvb_ads_channel_info_display(char*  IconeName);
void dvb_ads_hide_channel_info_pic(char*  IconeName);


#endif //__UI_DVB_ADS_PORTING_H__

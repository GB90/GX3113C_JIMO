#ifndef __SYS_COMMON_CONFIGURE_H__
#define __SYS_COMMON_CONFIGURE_H__

#include <gxtype.h>
#include "gxprogram_manage_berkeley.h"
#include "service/gxplayer.h"
#include "app_common_flash.h"
#include "gxapp_sys_config.h"



/*
* 定义语言xml对应名称
*/
#define LANGUAGE_CHINESE ("Chinese")
#define LANGUAGE_ENGLISH ("English")
#ifdef CUST_YUEHAI
#define LANGUAGE_WEIWEN ("WEIWEN")
#endif
/*
* 此头文件中宏定义值提供common模块调用
* 宏定义值可根据不同方案实际修改
* 宏定义变量名切勿随意修改、删除(可增加)，否则可能导致编译、运行异常
*/
#define VIDEO_DISPLAY_SCREEN_DV       (DISPLAY_SCREEN_4X3)
#ifdef APP_SD
#define VIDEO_HDMI_MODE_DV            (VIDEO_OUTPUT_PAL)//    (VIDEO_OUTPUT_HDMI_720P_50HZ)
#endif
#ifdef APP_HD
#define VIDEO_HDMI_MODE_DV 			 (VIDEO_OUTPUT_HDMI_1080I_50HZ)
#endif
#define VIDEO_ASPECT_DV                       (0)//(ASPECT_NORMAL)
#define VIDEO_QUIET_SWITCH_DV           (VIDEO_SWITCH_STILL)
#define VIDEO_AUTO_ADAPT_DV              (0) //
#define AUDIO_VOLUME_DV                      (48)
#define AUDIO_TRACK_DV                 	  (AUDIO_TRACK_STEREO)
#define AUDIO_AUDIO_TRACK_DV            (AUDIO_TRACK_STEREO)
#define AUDIO_AC3_BYPASS_DV        	  (0)
#define OSD_LANG_DV                 		  (LANGUAGE_CHINESE)
#define TELTEXT_LANG_DV                 	  (LANGUAGE_ENGLISH)
#define SUBTITLE_LANG_DV                 	  (LANGUAGE_ENGLISH)


#define OSD_TRANS_DV                            (10)

#define VIDEOCOLOR_BRIGHTNESS_DV                   (50)
#define VIDEOCOLOR_SATURATION_DV                   (50)
#define VIDEOCOLOR_CONTRAST_DV                  	(50)


#ifdef DVB_CA_TYPE_DS_CAS50_FLAG
#define BAR_TIME_DV                               (5)
#else
#define BAR_TIME_DV                               (2)
#endif
#define LCN_DV                                   (LCN_STATE_OFF)

#ifdef DVB_CA_TYPE_QZ_FLAG
	#define PASSWORD_DV                             ("9527")
#else
	#ifdef CUST_JIMO
		#define PASSWORD_DV                             ("9798")
	#else
		#define PASSWORD_DV                             ("0000")
	#endif
#endif

#define TIMEZONE_DV                              (8)
#define MAIN_FREQ_NITVERSION_DV       (32)
#ifdef CUST_YULIN
#define MANUSAL_SEARCH_FREQ_DV       (730)
#else
#define MANUSAL_SEARCH_FREQ_DV       (706)
#endif
#define MANUSAL_SEARCH_SYMBOL_DV   (6875)
#define MANUSAL_SEARCH_QAM_DV         (2)
#define PASSWORD_FLAG_DV 			   (0)
#define MUTE_FLAG_DV                             (0)
#define DTMB_MODE_DV                GXBUS_PM_SAT_1501_DTMB
#define USER_LIST_DV                              (0)
#define TRACK_GLOBLE_FLAG_DV             (1)
#define VOLUMN_GLOBLE_FLAG_DV          (1)
#define PVR_DURATION_VALUE              (0)
#define PVR_TIMESHIFT_DEFAULT	   			 (0)	
#define SUBTITLE_LANG_VALUE               (0)
#define SUBTITLE_MODE_VALUE               (0)


/*
* 定义搜索节目过程中，列表中同时显示搜索到的节目最大个数
*/
#define SEARCH_PROG_LIST_MAX (7)

/*
* 创建timer切台，设置timer启动时间
*/
#define PLAY_TIMER_DURATION (300)

/*
* 密码最大长度
*/
#define MAX_PASSWD_LEN			(4)

/*
* 前端监测时间
*/
#define FRONT_MONITER_DURATION (1000)

/*
* 序列号最大长度
*/
#define MAX_SEARIAL_LEN (20)

/*
* 全品频段搜索开始、结束频率
*/
#define FRE_BEGIN_LOW (52)
#define FRE_BEGIN_HIGH (866)
#ifdef CUST_NEIMENG
#define FRE_BEGIN_LOW_FULL (450)
#define FRE_BEGIN_HIGH_FULL (860)
#elif defined(CUST_TAIHUI_MG)
#define FRE_BEGIN_LOW_FULL (490)
#define FRE_BEGIN_HIGH_FULL (722)
#elif defined(CUST_JIMO)
#define FRE_BEGIN_LOW_FULL (474)
#define FRE_BEGIN_HIGH_FULL (858)
#else
#define FRE_BEGIN_LOW_FULL (52)
#define FRE_BEGIN_HIGH_FULL (866)

#endif
#define MAIN_MENU_VIDEO_X  192 
#define MAIN_MENU_VIDEO_Y  152
#define MAIN_MENU_VIDEO_W  348 
#define MAIN_MENU_VIDEO_H  240 

/*
* tv list menu video zoom position
*/

#define TV_MENU_VIDEO_X  85 
#define TV_MENU_VIDEO_Y  140 
#define TV_MENU_VIDEO_W  248 
#define TV_MENU_VIDEO_H  186 

/*
* epg menu video zoom position
*/
#define EPG_MENU_VIDEO_X  64 
#define EPG_MENU_VIDEO_Y  116 
#define EPG_MENU_VIDEO_W  220 
#define EPG_MENU_VIDEO_H  150 

/*
* fav menu video zoom position
*/
#define FAV_MENU_VIDEO_X  85 
#define FAV_MENU_VIDEO_Y  140 
#define FAV_MENU_VIDEO_W  248 
#define FAV_MENU_VIDEO_H  186 

/*
* edit menu video zoom position
*/
#define EDIT_MENU_VIDEO_X  430 
#define EDIT_MENU_VIDEO_Y  40 
#define EDIT_MENU_VIDEO_W  240 
#define EDIT_MENU_VIDEO_H  180 
#endif

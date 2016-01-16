/*****************************************************************************
* 						   CONFIDENTIAL								
*        Hangzhou GuoXin Science and Technology Co., Ltd.             
*                      (C)2010, All right reserved
******************************************************************************

******************************************************************************
* File Name :	win_main_menu.h
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :	
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
  VERSION	Date			  AUTHOR         Description
   1.0  	2012.10.30		  zhouhm 	 			creation
*****************************************************************************/
#ifndef __APP_MAIN_MENU1_H__
#define __APP_MAIN_MENU1_H__
#include "gxapp_sys_config.h"

typedef enum  
{
	BUTTON_TV = 0,
	BUTTON_RADIO,
	BUTTON_EPG,
	BUTTON_NVOD,
	BUTTON_MEDIA,
	BUTTON_SYSTEM_SET,
	BUTTON_MAIN_END
}win_main_menu_button;
int win_main_menu_change_button(int oldbuttonindex , int newbuttonindex);

#ifdef APP_SD
#define MAIN_MENU_VIDEO_X 		337
#define MAIN_MENU_VIDEO_Y 		50
#define MAIN_MENU_VIDEO_WIDTH 	326
#define MAIN_MENU_VIDEO_HIGHTH 	246
#endif/*APP_SD*/

#ifdef APP_HD
int win_main_menu_set_img_list(void);
#endif


#endif

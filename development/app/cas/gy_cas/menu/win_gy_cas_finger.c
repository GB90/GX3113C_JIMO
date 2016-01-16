/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_ca_api.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2012.12.02		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_play.h"
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "win_gy_cas_finger.h"
#include "app_gy_cas_api_pop.h"

void gy_cas_finger_show(uint8_t number,uint8_t showFlag,char* message)
{
	printf("gy_cas_finger_show number=%d\n",number);
	char	 sTextBuffer[40]={0};
	
	if (DVB_CA_FINGER_FLAG_HIDE == showFlag)
		{	
			sprintf( sTextBuffer, "win_full_screen_text_finger%d", number);
			GUI_SetProperty(sTextBuffer,"state","hide");
		}
	else
		if (DVB_CA_FINGER_FLAG_TRANS_HIDE == showFlag)
			{
				sprintf( sTextBuffer, "win_full_screen_text_finger%d", number);
				GUI_SetProperty(sTextBuffer,"state","osd_trans_hide");				
			}
	else
		if (DVB_CA_FINGER_FLAG_SHOW == showFlag)
			{
				sprintf( sTextBuffer, "win_full_screen_text_finger%d", number);					
				GUI_SetProperty(sTextBuffer,"string", message); 				
				GUI_SetProperty(sTextBuffer,"state","show");	
			}
	return;
}










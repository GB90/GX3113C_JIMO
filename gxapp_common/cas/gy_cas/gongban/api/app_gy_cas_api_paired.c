/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_ca_api.c
* Author    : 	bingyu
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2012.12.02		  bingyu 	 			creation
*****************************************************************************/
#include <gxtype.h>
#include "gxmsg.h"
#include "gxapp_sys_config.h"
#include "app_common_play.h"
#include "app_common_table_pmt.h"
#include "app_common_porting_stb_api.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_porting_ca_smart.h"
	
#include "gyca_def.h"
#include "gyca_pub.h"
#include "gyca_osp.h"
#include "gyca_interface.h"
#include "gyca_porting.h"
	
#include "app_gy_cas_api_pin.h"
#include "app_gy_cas_api_smart.h"
#include "app_gy_cas_api_demux.h"
#include "app_gy_cas_api_paired.h"


static char    s_text_buffer[100];


int32_t app_gy_cas_api_init_paired_data(void)
{
	return 0;
}

char* app_gy_cas_api_set_paired_data(char *pin)
{
	GYS32 paired;
	
	paired = GYCA_SetPaired((GYU8 *)pin);

    if( paired == GYCA_OK)
    {
			sprintf(s_text_buffer,"%s","Setting Paired Succeed");
	}
	else
	{	
		sprintf(s_text_buffer,"%s","Setting Paired Error, Please Check Pin");
    }

	return s_text_buffer;
}


char * app_gy_cas_api_get_paired_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0;


	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;    
	
	switch(ID)
	{
		case GY_PAIRED_INFO_ID:

			switch(GYCA_GetPaired())
			{
				case GYCA_CARD_MISPAIR:
				{
					sprintf( s_text_buffer,"%s", "当前的智能卡已经与其他的机顶盒配对");
				}break;
				case GYCA_CARD_NOPAIR:
				{
					sprintf( s_text_buffer,"%s", "当前的智能卡尚未与任何机顶盒配对");
				}break;
				case GYCA_OK:
				{
					sprintf( s_text_buffer,"%s", "当前的机顶盒与当前的智能卡配对");
				}break;
				default:
				{
					sprintf( s_text_buffer,"%s", "智能卡的信息无法获得");
				}break;
			}
			
			return s_text_buffer;
			
			break;
		default:
			break;
	}
	return NULL;

}







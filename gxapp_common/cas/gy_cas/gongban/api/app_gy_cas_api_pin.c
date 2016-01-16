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
	
	
#include "app_gy_cas_api_smart.h"
#include "app_gy_cas_api_pin.h"

char* app_gy_cas_api_change_pin(ca_pin_t* data)
{
	uint8_t newPassword1[GYCA_MAXLEN_PINCODE+1]={0};
	uint8_t newPassword2[GYCA_MAXLEN_PINCODE+1]={0};
    uint8_t newPassword3[GYCA_MAXLEN_PINCODE+1]={0};
    unsigned int   wRet            = 0;

	if (NULL == data)
		return NULL;


	memcpy(newPassword1,data->oldpin, GYCA_MAXLEN_PINCODE);
	memcpy(newPassword2, data->newpin, GYCA_MAXLEN_PINCODE);
	memcpy(newPassword3, data->newconfirmpin, GYCA_MAXLEN_PINCODE);
	
    if(strlen((char*)newPassword1)!=GYCA_MAXLEN_PINCODE ||strlen((char*)newPassword2)!=GYCA_MAXLEN_PINCODE ||strlen((char*)newPassword3)!=GYCA_MAXLEN_PINCODE )
	{
		return (char*)"Input new PIN";
	}
	
    if(atoi( (char*)newPassword3 ) != atoi( (char*)newPassword2 ))
    {
		return (char*)"Input new PIN";
    }
	
    wRet = GYCA_ChangePin(newPassword1,newPassword2);
	
    switch( wRet )
    {
        case GYCA_OK:
			return (char*)"Setting Pin Succeed";
        case GYCA_ERR:
			return (char*)"Setting Pin Error";
        case GYCA_PIN_ERR:
            return (char*)"Old PIN Error";
        default:
			break;
    }

	return NULL;
}








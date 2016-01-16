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
#include "DiviGuardCa.h"
#include "app_common_porting_stb_api.h"
#include "app_divitone_cas_api_pin.h"
extern UINT8 Repeattime;

char* app_divitone_cas_api_change_pin(ca_pin_t* data)
{
	uint8_t newPassword1[6+1]={0};
	uint8_t newPassword2[6+1]={0};
    	uint8_t newPassword3[6+1]={0};
    	INT32 wRet            = 0;

	if (NULL == data)
		return NULL;
	
	memcpy(newPassword1,data->oldpin, 6);
	memcpy(newPassword2, data->newpin, 6);
	memcpy(newPassword3, data->newconfirmpin, 6);
    	if(strlen((char*)newPassword1)!=6 ||strlen((char*)newPassword2)!=6 ||strlen((char*)newPassword3)!=6 )
	{
		return (char*)"Input new PIN";
	}
 //   	if(atoi( (char*)newPassword3 ) != atoi( (char*)newPassword2 ))
	if(strcmp((char*)newPassword3,(char*)newPassword2))
    	{
		return (char*)"Input new PIN";
    	}
	Repeattime = 0;
   	wRet = DIVI_Modify_Pin(newPassword1,newPassword2,6,&Repeattime);
	if(wRet == 0)
		return (char*)"Set Successfully!";
	else
		return (char*)"PIN Error!";

	return NULL;
}








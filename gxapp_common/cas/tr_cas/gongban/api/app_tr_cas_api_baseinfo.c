/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_dvt_cas_api_baseinfo.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.12.04		  xxxxx 	 			creation
*****************************************************************************/
#include "app_common_porting_stb_api.h"
#include "app_tr_cas_api_baseinfo.h"
#include "Tr_Cas.h"


int32_t app_tr_cas_api_init_baseinfo_data(void)
{
	return 0;
}

char* app_tr_cas_api_get_baseinfo_data(ca_get_date_t *data)
{
	uint32_t pos = 0;                             
	uint32_t ID = 0;  
   	static char s_text_buffer[40] = {0};
	S16 Ret = 0;
	uint8_t chRating = 0;
	U16 wRegionCode = 0xFFFF;
	char *pBuf = NULL;
	
	if (NULL == data)
	{
		return NULL;
	}

	pos = data->pos;
	ID = data->ID;
	
	memset(s_text_buffer, 0, sizeof(s_text_buffer));

	switch (ID)
	{
		case TR_BASEINFO_SERIAL_ID:
			Ret = MC_GetScSerialNumber(s_text_buffer, 40);
			if (0 == Ret)
			{	
				return s_text_buffer;
			}
			else
			{
				return (char*)"Failed";
			}
			break;
			
		case TR_BASEINFO_COS_VER_ID:
			Ret = MC_GetSmartCardVersion(s_text_buffer, 40);
			if (0 == Ret)
			{
				return s_text_buffer;
			}
			else
			{
				return (char*)"Failed";
			}
			break;
			
		case TR_BASEINFO_CAS_VER_ID:
			pBuf = (char *)MC_GetRevisionString();
			if (NULL != pBuf)
			{
				strncpy(s_text_buffer,pBuf,40);
				return s_text_buffer;
			}
			else
			{
				return (char*)"Failed";
			}
			break;

		case TR_BASEINFO_RATING_ID:
			Ret = MC_SCARD_GetRating(&chRating);
			if (0 == Ret)
			{
				sprintf( (char *)s_text_buffer, "%d", chRating);
				return s_text_buffer;
			}
			else
			{
			
				return (char*)"Failed";//观看级别	
			}	
			break;
		
		case TR_BASEINFO_AREA_ID:	//区域码
			Ret = MC_SCARD_GetRegionCode(&wRegionCode);
			if (Ret == 0)
			{
				memset(s_text_buffer, 0, 40);
				sprintf((char *)s_text_buffer, "0x%04x", wRegionCode);
				return s_text_buffer;
			}
			else
			{
				return (char*)"Failed";
			}
			break;

		case TR_BASEINFO_MARRY_STATE:
			Ret = MC_GetScMarryState(s_text_buffer, 40);
			if (Ret == 0)
			{
				return (char *)"Marryed";
			}
			else if (7 == Ret)
			{
				return s_text_buffer;
			}
			else
			{
				sprintf((char *)s_text_buffer, "Err:%d", Ret);
				return s_text_buffer;
			}
			break;	
			
		default:
			break;
	}
	
	return NULL;
}



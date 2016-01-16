/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_xinshimao_cas_api_baseinfo.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.09.25		  zhouhm 	 			creation
*****************************************************************************/
#include "xinshimao_ca.h"
#include "app_common_porting_stb_api.h"
#include "app_common_porting_ca_smart.h"
#include "app_xinshimao_cas_api_baseinfo.h"

int32_t app_xinshimao_cas_api_init_baseinfo_data(void)
{
	return 0;
}
char * app_xinshimao_cas_api_get_baseinfo_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0;  
	uint16_t region = 0; 	
	uint8_t flag = 0;

       static char                  s_text_buffer[32]={0};

	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	memset(s_text_buffer,0,sizeof(s_text_buffer));

	
	if (FALSE == app_porting_ca_smart_status())
		return " ";

	switch(ID)
		{
			case XINSHIMAO_BASEINFO_CARD_ID:
				XinShiMao_GetSMCNO((UINT8 *)s_text_buffer);
				return (char*)s_text_buffer;
			case XINSHIMAO_BASEINFO_TYPE_ID:
				XinShiMao_GetSMCType(&flag);
				if (0 == flag)
					{
						strcpy(s_text_buffer,"Mother Card");
						return (char*)s_text_buffer;
					}
				else
					{
						strcpy(s_text_buffer,"Child Card");
						return (char*)s_text_buffer;
					}
				break;
			case XINSHIMAO_BASEINFO_MARRIED_ID:
				XinShiMao_GetSMCStbPaired(&flag);
				if (0 == flag)
					{
						strcpy(s_text_buffer,"No Married");
						return (char*)s_text_buffer;
					}
				else
					{
						strcpy(s_text_buffer,"Married");
						return (char*)s_text_buffer;
					}
				break;
			case XINSHIMAO_BASEINFO_FLAG_ID:
				XinShiMao_GetSMCEntitle(&flag);
				if (0 == flag)
					{
						strcpy(s_text_buffer,"NO");
						return (char*)s_text_buffer;
					}
				else
					{
						strcpy(s_text_buffer,"YES");
						return (char*)s_text_buffer;
					}
				break;
			case XINSHIMAO_BASEINFO_REGION_ID:
				region = XinShiMao_GetCardRegionID();
				sprintf(s_text_buffer,"0x%04x",region);
				return (char*)s_text_buffer;
			case XINSHIMAO_BASEINFO_NETWORK_ID:
				region = XinShiMao_GetCardRegionID();
				sprintf(s_text_buffer,"0x%04x",region);
				return (char*)s_text_buffer;
			default:
				break;
		}
	return NULL;

}







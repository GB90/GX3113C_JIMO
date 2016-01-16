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
#include "BYCAca_stb.h"
#include "app_common_porting_stb_api.h"
#include "app_by_cas_api_baseinfo.h"

int32_t app_by_cas_api_init_baseinfo_data(void)
{
	return 0;
}
char * app_by_cas_api_get_baseinfo_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0;  

	BU32 pdwVer = 0;
	BU8 pCaSysSp[64]={0};
	BU8 pCaSysCt[32]={0};
	BU32 pdwCardID = 0;
	uint8_t ret = 0;

       static char                  s_text_buffer[70]={0};

	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	memset(s_text_buffer,0,sizeof(s_text_buffer));

	switch(ID)
		{
			case BY_BASEINFO_VERSION_ID:
				ret = BYCASTB_GetVer(&pdwVer, pCaSysSp, pCaSysCt);
				if (TRUE == ret)
					{
						sprintf(s_text_buffer,"0x%08x",(unsigned int)pdwVer);
						return s_text_buffer;
					}
				else
					{
						return NULL;
					}
				break;
			case BY_BASEINFO_SYS_SP_ID:
				ret = BYCASTB_GetVer(&pdwVer, pCaSysSp, pCaSysCt);
				if (TRUE == ret)
					{
						strcpy(s_text_buffer,(char*)pCaSysSp);
						return s_text_buffer;
					}
				else
					{
						return NULL;
					}
				break;
			case BY_BASEINFO_SYS_CT_ID:
				ret = BYCASTB_GetVer(&pdwVer, pCaSysSp, pCaSysCt);
				if (TRUE == ret)
					{
						strcpy(s_text_buffer,(char*)pCaSysCt);
						return s_text_buffer;
					}
				else
					{
						return NULL;
					}
				break;
			case BY_BASEINFO_CARDID_ID:
				ret = BYCASTB_GetCardID(&pdwCardID);
				if (TRUE == ret)
					{
						sprintf(s_text_buffer,"%d",(unsigned int)pdwCardID);
						return s_text_buffer;
					}
				else
					{
						return NULL;
					}
				break;
			default:
				break;
		}
	return NULL;

}







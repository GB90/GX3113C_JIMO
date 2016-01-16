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
1.0  	2013.12.04		  zhouhm 	 			creation
*****************************************************************************/
//#include "BYCAca_stb.h"
#include "DVTCAS_STBInf.h"
#include "app_common_porting_stb_api.h"
#include "app_dvt_cas_api_baseinfo.h"
SDVTCAManuInfo SCInfo = {0,};
HRESULT h_result = DVTCA_OK;
HRESULT mother_result = DVTCA_OK;
DWORD SCMotherCardID = 1;



int32_t app_dvt_cas_api_init_baseinfo_data(void)
{
	//卡信息
		
	h_result = DVTCASTB_GetStreamGuardManuInfo(&SCInfo);
	mother_result = DVTCASTB_GetMotherInfo(&SCMotherCardID);

	return 0;
}
char * app_dvt_cas_api_get_baseinfo_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0;  

	SDVTCAAreaInfo SCaRreaInfo = {0,};
	bool SCLocked = 0;
//	unsigned char SCLocked = 0;
	BYTE SCyRating = 0;
	BYTE SCStartHour = 0;
	BYTE SCStartMinute = 0; 
	BYTE SCEndHour = 0; 
	BYTE SCEndMinute = 0;
    static char                  s_text_buffer[40]={0};

	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	memset(s_text_buffer,0,sizeof(s_text_buffer));

	switch(ID)
		{
			case DVT_BASEINFO_SERIAL_ID:
				if (DVTCA_OK == h_result)
				{
					memcpy(s_text_buffer,SCInfo.m_byszSerialNO,39);
					//内部卡号
					//sprintf( (char *)s_text_buffer, "%d",  (unsigned int)SCInfo.m_dwCardID );
					
					return s_text_buffer;
				}
				else
				{
					return (char*)"Failed";
				}
				break;
			case DVT_BASEINFO_COS_VER_ID:
				if (DVTCA_OK == h_result)
					{
						sprintf( (char *)s_text_buffer, "%d",  (unsigned int)SCInfo.m_dwSCCOSVer );
						return s_text_buffer;
					}
				else
					{
						return (char*)"Failed";
					}
				break;
			case DVT_BASEINFO_CAS_VER_ID:
				if (DVTCA_OK == h_result)
					{
						DWORD stb_usermask =  SCInfo.m_dwSTBCASVer;
						sprintf( (char *)s_text_buffer,"%u.%u.%u.%u",
							(int)((stb_usermask&0xff000000)>>24),(int)((stb_usermask&0x00ff0000)>>16),
							(int)((stb_usermask&0x0000ff00)>>8),(int)(stb_usermask&0x000000ff));
						return s_text_buffer;
					}
				else
					{
						return (char*)"Failed";
					}
				break;
			case DVT_BASEINFO_MANU_NAME_ID:
				if (DVTCA_OK == h_result)
					{
						sprintf(  (char *)s_text_buffer, "%s", SCInfo.m_szSCCASManuName );
							return s_text_buffer;
					}
				else
					{
						return (char*)"Failed";
					}
				break;
			case DVT_BASEINFO_MOTHER_ID:
				if (DVTCA_OK == mother_result)
				{
					if(SCMotherCardID)
					{
						return (char*)"Daughter Card";// 子母卡属性
					}
					else
					{
						return (char*)"Master Card";// 子母卡属性
					}				
				}
				else
				{
						return (char*)"None";// 子母卡属性
				}
				break;
			case DVT_BASEINFO_MOTHER_NUMBER_ID:
				if (DVTCA_OK == mother_result)
				{
						sprintf( (char *)s_text_buffer, "%d", (unsigned int)SCMotherCardID);
						return s_text_buffer;
				}
				else
				{
					return (char*)"None";// 子母卡属性
				}				
				break;
			case DVT_BASEINFO_LOCK_ID:
				if (DVTCA_OK == DVTCASTB_IsPinLocked(&SCLocked))
				{
					if(SCLocked)
					{
						return (char*)"Lock";//PIN码锁定状态				
					}
					else
					{
						return (char*)"Unlock";//PIN码锁定状态
					}
				}
				else
				{
					return (char*)"Failed";//PIN码锁定状态
				}
				break;
			case DVT_BASEINFO_RATING_ID:
				if (DVTCA_OK == DVTCASTB_GetRating(&SCyRating))
					{
						sprintf( (char *)s_text_buffer, "%d", SCyRating);
						return s_text_buffer;
					}
				else
				{
				
					return (char*)"Failed";//观看级别	
				}				
				break;
			case DVT_BASEINFO_WORK_TIME_ID:
				if (DVTCA_OK == DVTCASTB_GetWorkTime(&SCStartHour, &SCStartMinute, 
											&SCEndHour,&SCEndMinute))
				{
						memset(s_text_buffer, 0, 40);
						sprintf( (char *)s_text_buffer, "%02d:%02d-%02d:%02d", SCStartHour,
										SCStartMinute,SCEndHour,SCEndMinute);
						return s_text_buffer;
				}
				else
				{
					return (char*)"Failed";//工作时段
				}				
				break;
			case DVT_BASEINFO_AREA_ID:	//区域码
				if (DVTCA_OK == DVTCASTB_GetAreaInfo(&SCaRreaInfo))
					{
						memset(s_text_buffer, 0, 40);
						sprintf(  (char *)s_text_buffer, "%d", (unsigned int)SCaRreaInfo.m_dwCardArea );
						return s_text_buffer;
					}
				else
					{
						return (char*)"None";			
					}
				break;
			case DVT_BASEINFO_AREA_LOCK://区域码锁定状态
				if (DVTCA_OK == DVTCASTB_GetAreaInfo(&SCaRreaInfo))
				{
					memset(s_text_buffer, 0, 40);
					if(SCaRreaInfo.m_byStartFlag)//启动
					{
						sprintf( (char *)s_text_buffer, "Started");
					}
					else//未启动
					{
						sprintf( (char *)s_text_buffer, "Do not Started");
					}
					
					return s_text_buffer;
				}
				else
				{
					return (char*)"None";			
				}
				break;
			case DVT_BASEINFO_AREA_TIME://收到区域码流时间
				if (DVTCA_OK == DVTCASTB_GetAreaInfo(&SCaRreaInfo))
				{
					memset(s_text_buffer, 0, 40);
                    extern bool GetTimeStrFromValC1(uint32_t nInputTime,char * szOut);
					GetTimeStrFromValC1(SCaRreaInfo.m_tSetStreamTime,s_text_buffer);
					return s_text_buffer;
				}
				else
				{
					return (char*)"None";			
				}
				break;
			default:
				break;
		}
	return NULL;

}







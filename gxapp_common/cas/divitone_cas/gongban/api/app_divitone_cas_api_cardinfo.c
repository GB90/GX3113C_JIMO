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
#include "app_common_porting_ca_smart.h"
#include "app_divitone_cas_api_cardinfo.h"
int32_t app_divitone_cas_api_init_baseinfo_data(void)
{
	return 0;
}
char * app_divitone_cas_api_get_baseinfo_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0;  

	INT32 i;
	UINT8 smartcard_id[20];
	char tmp1[40];
	UINT8  WorkTimeEnable=1;
	UINT8  BeginTime[3];
	UINT8  EndTime[3];	
	uint8_t ret = 0;

       static char s_text_buffer[70]={0};

	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	memset(s_text_buffer,0,sizeof(s_text_buffer));

	switch(ID)
	{
			case DIVI_BASEINFO_CARDID_ID:
				if(DIVI_OK == DIVI_GetSMCNO(smartcard_id))
				{
				//	CAS_Dbg("SMARTCARD ID: ");
					for(i=0;i<8;i++)
					{
				//		printf("%02x ",smartcard_id[i]);
						tmp1[2*i]=((smartcard_id[i]&0xf0)>>4)+0x30;   
						tmp1[2*i+1]=(smartcard_id[i]&0x0f)+0x30;			
						
					}
					tmp1[16]='\0';

				/*	for(i=0;i<17;i++)
					{
						CAS_Dbg("%x ",tmp1[i]);							
						
					}
				*/
					memcpy(s_text_buffer,tmp1,sizeof(tmp1));

                    //CAS_Dbg("SMARTCARD ID: %s\n",s_text_buffer);
					return s_text_buffer;
				}
				else 
				{
                    CAS_Dbg("SMARTCARD ID: Failed.\n");
					return NULL;
                }
				break;
			case DIVI_BASEINFO_CARDTYPE_ID:
				if(0x08==DIVI_GetSMCUserType())
				{
					snprintf(s_text_buffer,70,"%s","Mother Card");
					return s_text_buffer;
				}
				else if(0x09==DIVI_GetSMCUserType())
				{
					snprintf(s_text_buffer,70,"%s","Child Card");
					return s_text_buffer;
				}
				else
				{
					return NULL;
				}
				break;
			case DIVI_BASEINFO_CARDSTATE_ID:
				ret = app_porting_ca_smart_status();
				if(ret == TRUE)
				{
					snprintf(s_text_buffer,70,"%s","Card Insert");
					return s_text_buffer;
				}
				else
				{
					snprintf(s_text_buffer,70,"%s","Sc No Card");
					return s_text_buffer;
				}
				break;
			case DIVI_BASEINFO_REGION_ID:
				//DIVI_GetCurr_NetRegionID();
				snprintf(s_text_buffer,70,"%s","No Area");
				return s_text_buffer;
				break;
			case DIVI_BASEINFO_WORKTIME_ID:
 				if(0==DIVI_GetSMCWorkTime(&WorkTimeEnable,BeginTime,EndTime))
				{			 	
				// 	CAS_Dbg("########BeginTime=0x%x  0x%x  0x%x \n",BeginTime[0],BeginTime[1],BeginTime[2]);
				//	CAS_Dbg("#########EndTime=0x%x  0x%x  0x%x \n",EndTime[0],EndTime[1],EndTime[2]);

					if(1==WorkTimeEnable)
					{
						tmp1[0]=((BeginTime[0]&0xf0)>>4)+0x30;   
						tmp1[1]=(BeginTime[0]&0x0f)+0x30;	
						tmp1[2]=':';
						tmp1[3]=((BeginTime[1]&0xf0)>>4)+0x30;   
						tmp1[4]=(BeginTime[1]&0x0f)+0x30;					
						tmp1[5]='-';				
						tmp1[6]=((EndTime[0]&0xf0)>>4)+0x30;   
						tmp1[7]=(EndTime[0]&0x0f)+0x30;	
						tmp1[8]=':';
						tmp1[9]=((EndTime[1]&0xf0)>>4)+0x30;   
						tmp1[10]=(EndTime[1]&0x0f)+0x30;				
						tmp1[11]='\0';

						memcpy(s_text_buffer,tmp1,sizeof(tmp1));
						return s_text_buffer;
					}
					else
					{
						snprintf(s_text_buffer,70,"%s","Unkown");
						return s_text_buffer;		
					}					
				 }
			 	else
			 	{			 	
					snprintf(s_text_buffer,70,"%s","Unkown");
					return s_text_buffer;				 	
				}
				
				break;
			case DIVI_BASEINFO_WATCHLEVEL_ID:
				 i= DIVI_GetSMCUserLevel();

				 if(-1==i)
				 {
					snprintf(s_text_buffer,70,"%s","Unkown");
				 }
				 else if(0==i)
				 {
					snprintf(s_text_buffer,70,"%s","四岁以上");
				 }
				 else if(1==i)
				 {
					snprintf(s_text_buffer,70,"%s","八岁以上");
				 }
				 else if(2==i)
				 {
					snprintf(s_text_buffer,70,"%s","十二岁以上");
				 }
				 else if(i>2)
				 {
					snprintf(s_text_buffer,70,"%s","十六岁以上");
				 }
				return s_text_buffer;
				break;
			case DIVI_BASEINFO_PAIR_STATE:
				i = DIVI_GetPaired();
				if(i == 1)
					snprintf(s_text_buffer,70,"%s","Paired");
				else if(i == 2)
					snprintf(s_text_buffer,70,"%s","Not Paired");
				else
					snprintf(s_text_buffer,70,"%s","Sc No Card");
				return s_text_buffer;
				break;
			case DIVI_BASEINFO_CAVERSION_ID:
				snprintf(s_text_buffer,70,"%s","V2.1");
				return s_text_buffer;
				break;				
			default:
				break;
		}
	return NULL;

}







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
#include "wanfacas.h"
#include "app_common_porting_stb_api.h"
#include "app_wf_cas_api_baseinfo.h"

extern void  ConvertMjdToYmd( uint16_t   mjd, uint16_t   *year, uint8_t  *month, uint8_t  *day, uint8_t  *weekDay );
#define BCD2DEC(bcd)	(((bcd)>>4)*10+((bcd)&0xf))
void mjd_to_tm(U16 mjd, CommonTime_t * tm)
{
	unsigned char IK = 0;
	
	tm->m_nYear = (U32)( (mjd -15078.2) /365.25 );
	tm->m_nMonth= (U32)( (mjd - 14956.1 - (U32)(tm->m_nYear* 365.25) ) 
																/30.6001);
	tm->m_nDay= (U32)(mjd -14956 - (U32)(tm->m_nYear * 365.25) 
										- (U32)(tm->m_nMonth * 30.6001));
	
	if(tm->m_nMonth == 14 || tm->m_nMonth == 15)
	{
		IK = 1;
	}
	else 
	{
		IK = 0;
	}

	tm->m_nYear+= (IK+1900);
	tm->m_nMonth = tm->m_nMonth - 1 - IK*12;
	
}

 void utc_to_tm(U32 bcd_time, CommonTime_t *tm)
{
	tm->m_nHour= BCD2DEC(bcd_time&0xff);
	tm->m_nMinute= BCD2DEC((bcd_time>>8)&0xff);
	tm->m_nSecond= BCD2DEC((bcd_time>>16)&0xff);

	if(tm->m_nHour>23)
	{
		tm->m_nHour = 23;
	}
	
	if(tm->m_nMinute>59)
	{
		tm->m_nMinute = 59;
	}
}


int32_t app_wf_cas_api_init_baseinfo_data(void)
{
	return 0;
}
char * app_wf_cas_api_get_baseinfo_data(ca_get_date_t *data)
{                      
	uint32_t ID = 0;  

	uint8_t ret = 0;

	WFCAS_Purse_t  Purse;
	WFCAS_User_t  UserInfo;
	WFCAS_Version_t VerInfo;
	WFCAS_DateTime_t  DateTime;
	U32 StbNo[40]={0};
	uint16_t year1,year2;
	uint8_t month1,month2;
	uint8_t day1,day2;
	uint8_t week1,week2;
	CommonTime_t  tm_time;

//	char smcNo[40];
	char buf0[40] ;
	char buf1[40];
	int k;

       static char s_text_buffer[70]={0};

	if (NULL == data)
		return NULL;

	ID = data->ID;

	memset(s_text_buffer,0,sizeof(s_text_buffer));
	switch(ID)
	{
		case WF_BASEINFO_CARDID_ID:
			if(WFCAS_GetUserInfo(&UserInfo) == 0)
			{
				memset(buf0, 0, 40);		
				for(k=0;k<8;k++)
				{
					sprintf(buf1,"%d",BCD2DEC(UserInfo.Card_No[k]));
					strcat(buf0, buf1);
				}
				memset(s_text_buffer,0,70);
				strncat(s_text_buffer,&buf0[0],1);
				strncat(s_text_buffer," ",1);
				strncat(s_text_buffer,&buf0[1],4);
				strncat(s_text_buffer," ",1);
				strncat(s_text_buffer,&buf0[5],2);
				strncat(s_text_buffer," ",1);
				strncat(s_text_buffer,&buf0[7],8);
				strncat(s_text_buffer," ",1);
				strncat(s_text_buffer,&buf0[15],1);
				if(UserInfo.ChildCard==TRUE)
				{
					strcat(s_text_buffer,"CHILD CARD");					
				}
				else
				{
					strcat(s_text_buffer,"MOTHER CARD");
				}
				CAS_Dbg("LINE:%d---%s\n",__LINE__,s_text_buffer);
				return s_text_buffer;
			}
			else
			{
                printf("WFCAS_GetUserInfo failed.\n");
				return NULL;
			}

			break;
		case WF_BASEINFO_PURSEINFO_ID:
			ret = WFCAS_GetPurseInfo(&Purse);
			if(ret == 0)
			{
				snprintf(s_text_buffer,70,"%ld/%ld",Purse.Balance,Purse.Money);
				CAS_Dbg("LINE:%d---%s\n",__LINE__,s_text_buffer);
				return s_text_buffer;
			}
			else
				return NULL;

			break;
		case WF_BASEINFO_STBNO_ID:
			ret = WFCAS_GetStbNo(StbNo);
			if(0 == ret)
			{
				memcpy(s_text_buffer,(char*)StbNo,40);
				CAS_Dbg("LINE:%d---%s\n",__LINE__,s_text_buffer);
				return s_text_buffer;
			}
			else
			{
				return NULL;
			}
			break;
		case WF_BASEINFO_CASID_ID:
			if(WFCAS_GetVerInfo(&VerInfo) == 0)
			{
				snprintf(s_text_buffer,70,"%x",VerInfo.Cas_Id);
				CAS_Dbg("LINE:%d---%s\n",__LINE__,s_text_buffer);
			//	CAS_Dbg("LINE:%d---VerInfo.Version=%d\n",__LINE__,VerInfo.Version);
				return s_text_buffer;
			}
			else
				return NULL;
			break;
		case WF_BASEINFO_CASVER_ID:
			if(WFCAS_GetVerInfo(&VerInfo) == 0)
			{
			//	snprintf(s_text_buffer,70,"%d",VerInfo.Sub_Id);
				snprintf(s_text_buffer,70,"0x%02x",VerInfo.Version);
				CAS_Dbg("LINE:%d---%s\n",__LINE__,s_text_buffer);
				return s_text_buffer;
			}
			else
				return NULL;
			break;			
		case WF_BASEINFO_CASPRO_ID:
			if(WFCAS_GetUserInfo(&UserInfo) == 0)
			{
				snprintf(s_text_buffer,70,"%s",UserInfo.Provider);
				CAS_Dbg("LINE:%d---%s\n",__LINE__,s_text_buffer);
				return s_text_buffer;
			}
			else
				return NULL;
			break;
		case WF_BASEINFO_CASDES_ID:
			if(WFCAS_GetVerInfo(&VerInfo) == 0)
			{
				snprintf(s_text_buffer,70,"%s",VerInfo.Description);
				CAS_Dbg("LINE:%d---%s\n",__LINE__,s_text_buffer);
				return s_text_buffer;
			}
			else
				return NULL;
			break;
		case WF_BASEINFO_CARDVALID_ID:
			if(WFCAS_GetUserInfo(&UserInfo) == 0)
			{
				ConvertMjdToYmd(UserInfo.Issuance_Date,&year1,&month1,&day1,&week1);
				ConvertMjdToYmd(UserInfo.Issuance_Date,&year2,&month2,&day2,&week2);
				snprintf(s_text_buffer,70,"%d/%02d/%02d-%d/%02d/%02d",year1,month1,day1,year2,month2,day2);
				CAS_Dbg("LINE:%d---%s\n",__LINE__,s_text_buffer);
				return s_text_buffer;
			}
			else
				return NULL;
			break;
		case WF_BASEINFO_DATATIME_ID:
			if(WFCAS_GetDateTime(&DateTime)==0)
			{
				mjd_to_tm(DateTime.Mjd, &tm_time);
				utc_to_tm(DateTime.Utc,&tm_time);
				snprintf(s_text_buffer,70,"%d-%02d-%02d %02d:%02d:%02d",tm_time.m_nYear,tm_time.m_nMonth,tm_time.m_nDay,tm_time.m_nHour,tm_time.m_nMinute,tm_time.m_nSecond);
				CAS_Dbg("LINE:%d---%s\n",__LINE__,s_text_buffer);
				return s_text_buffer;
			}
			else
				return NULL;
			break;
		default:
			break;
	}
	return NULL;

}







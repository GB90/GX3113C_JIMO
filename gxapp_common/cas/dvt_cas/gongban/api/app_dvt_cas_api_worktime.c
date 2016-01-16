/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_dvt_cas_api_worktime.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.12.05		  zhouhm 	 			creation
*****************************************************************************/
#include "DVTCAS_STBInf.h"
#include "app_common_porting_stb_api.h"
#include "app_dvt_cas_api_worktime.h"

BYTE  pbyStartHour = 0;
BYTE  pbyStartMinute = 0;
BYTE  pbyEndHour = 0;
BYTE  pbyEndMinute = 0;



uint8_t app_dvt_cas_api_init_worktime_data(void)
{

	if (DVTCA_OK == DVTCASTB_GetWorkTime(&pbyStartHour, &pbyStartMinute, &pbyEndHour, &pbyEndMinute))
	{
		return 1;		
	}
	
	return 0;
}

char * app_dvt_cas_api_get_worktime_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0;
    static char            s_text_buffer[30];

	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	memset(s_text_buffer,0,sizeof(s_text_buffer));
	
	switch(ID)
		{
			case DVT_WORKTIME_START_ID:
				sprintf(s_text_buffer, "%02d:%02d", pbyStartHour,pbyStartMinute);
				return s_text_buffer;
				break;
			case DVT_WORKTIME_END_ID:
				sprintf(s_text_buffer, "%02d:%02d", pbyEndHour,pbyEndMinute);
				return s_text_buffer;
				break;
			default:
				break;
		}
	return NULL;

}

char* app_dvt_cas_api_change_worktime(ca_work_time_t* data)
{
	char StartTime[20]={0,};
	char EndTime[20]={0,};

	char  StartTimeHour[3];
	char  StartTimeMin[3];
	char  EndTimeHour[3];
	char  EndTimeMin[3];
	HRESULT hresult = 0;
	SDVTCAPin pNewPin = {0};
	uint8_t Password1[DVTCA_MAXLEN_PINCODE+1]={0};

	if (NULL == data)
		return NULL;

	 memcpy(Password1,data->pin,DVTCA_MAXLEN_PINCODE);
	 
	 memcpy(StartTime,data->starttime,5);
	 memcpy(EndTime,data->endtime,5);


	 if(strlen((char*)Password1)!=DVTCA_MAXLEN_PINCODE)
	 {
		 return (char*)"Input 8-bit PIN";
	 }
	 
	 pNewPin.m_byLen = 8;
	 pNewPin.m_byszPin[0] = Password1[0]-0x30;
	 pNewPin.m_byszPin[1] = Password1[1]-0x30;
	 pNewPin.m_byszPin[2] = Password1[2]-0x30;
	 pNewPin.m_byszPin[3] = Password1[3]-0x30;
	 pNewPin.m_byszPin[4] = Password1[4]-0x30;
	 pNewPin.m_byszPin[5] = Password1[5]-0x30;
	 pNewPin.m_byszPin[6] = Password1[6]-0x30;
	 pNewPin.m_byszPin[7] = Password1[7]-0x30;
	 hresult = DVTCASTB_VerifyPin(&pNewPin);
	 switch(hresult)
	 {
		 case DVTCAERR_STB_DATA_LEN_ERROR:
			 {
				 return (char*)"输入PIN码长度错误,如果连续3次错误,PIN码将被锁定";
			 }
			 break;
		 case DVTCAERR_STB_PIN_LOCKED:
			 {
				 return (char*)"The PIN has been locked";
			 }
			 break;
		 case DVTCAERR_STB_PIN_INVALID:
			 {
				 return (char*)"输入PIN错误,如果连续3次错误,PIN码将被锁定";
			 }
			 break;
		 default:
			 break; 					 
	 }
	 
	 
	 StartTimeHour[2] = '\0';
	 memcpy(StartTimeHour, StartTime, 2);
	 StartTimeMin[2] = '\0';
	 memcpy(StartTimeMin, StartTime + 3, 2);
	 EndTimeHour[2] = '\0';
	 memcpy(EndTimeHour, EndTime, 2);
	 EndTimeMin[2] = '\0';
	 memcpy(EndTimeMin, EndTime + 3, 2);
	 
	 pbyStartHour = atoi(StartTimeHour);
	 pbyStartMinute = atoi(StartTimeMin);
	 pbyEndHour = atoi(EndTimeHour);
	 pbyEndMinute = atoi(EndTimeMin);
	 
	 if (DVTCA_OK ==  DVTCASTB_SetWorkTime( pbyStartHour,  pbyStartMinute,	pbyEndHour,  pbyEndMinute))
		 {
		 	return (char*)"Settings success";
		 }
	 else
		 {
			 return (char*)"Setting fail, hour range 0~23, minute range 0~59";
		 }

     return NULL;
}








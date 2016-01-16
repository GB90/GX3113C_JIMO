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
#include "CDCASS.h"
#include "app_common_porting_stb_api.h"
#include "app_cd_cas_3.0_api_worktime.h"

unsigned char  StartHour;
unsigned char  StartMin;
unsigned char  StartSec;
unsigned char  EndHour;
unsigned char  EndMin;
unsigned char  EndSec;


uint8_t app_cd_cas30_api_init_worktime_data(void)
{
	StartHour=0;
	StartMin=0;
	StartSec=0;
	EndHour=0;
	EndMin=0;
	EndSec=0;
	
	CDCASTB_GetWorkTime(&StartHour,&StartMin,&StartSec,&EndHour,&EndMin,&EndSec);
	return 0;
}

char * app_cd_cas30_api_get_worktime_data(ca_get_date_t *data)
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
			case CD_WORKTIME_START_ID:
				sprintf(s_text_buffer, "%02d:%02d:%02d", StartHour,StartMin,StartSec);
				return s_text_buffer;
				break;
			case CD_WORKTIME_END_ID:
				sprintf(s_text_buffer, "%02d:%02d:%02d", EndHour,EndMin,EndSec);
				return s_text_buffer;
				break;
			default:
				break;
		}
	return NULL;

}

char* app_cd_cas30_api_change_worktime(ca_work_time_t* data)
{
	char StartTime[20]={0,};
	char EndTime[20]={0,};
    unsigned char i;
	char  StartTimeHour[3];
	char  StartTimeMin[3];
    char  StartTimeSec[3];
	char  EndTimeHour[3];
	char  EndTimeMin[3];
    char  EndTimeSec[3];
    unsigned char  startHour;
	unsigned char  startMin;
    unsigned char  startSec;
	unsigned char  endHour;
	unsigned char  endMin;
    unsigned char  endSec;
    unsigned short  wRet;
	uint8_t PinValue[CDCA_MAXLEN_PINCODE+1]={0};


	if (NULL == data)
		return NULL;

	 memcpy(PinValue,data->pin,CDCA_MAXLEN_PINCODE);
	 if(strlen((char*)PinValue)!=CDCA_MAXLEN_PINCODE)
	 {
		 return (char*)"Invalid PIN";	 
	 }
	 
	 for( i = 0; i < CDCA_MAXLEN_PINCODE; i++ )
	 {
		 PinValue[i] -= 0x30;
	 }
	 memcpy(StartTime,data->starttime,8);
	 memcpy(EndTime,data->endtime,8);
	 
	 StartTimeHour[2] = '\0';
	 memcpy(StartTimeHour, StartTime, 2);
	 StartTimeMin[2] = '\0';
	 memcpy(StartTimeMin, StartTime + 3, 2);
	 StartTimeSec[2] = '\0';
	 memcpy(StartTimeSec, StartTime + 6, 2);
	 
	 EndTimeHour[2] = '\0';
	 memcpy(EndTimeHour, EndTime, 2);
	 EndTimeMin[2] = '\0';
	 memcpy(EndTimeMin, EndTime + 3, 2);
	 EndTimeSec[2] = '\0';
	 memcpy(EndTimeSec, EndTime + 6, 2);
	 
	 startHour= atoi(StartTimeHour);
	 startMin = atoi(StartTimeMin);
	 startSec= atoi(StartTimeSec);
	 endHour = atoi(EndTimeHour);
	 endMin = atoi(EndTimeMin);
	 endSec= atoi(EndTimeSec);
	 
	 wRet = CDCASTB_SetWorkTime(PinValue,startHour,startMin,startSec,endHour,endMin,endSec);	
	 switch( wRet )
	 {
	 case CDCA_RC_OK:
		 return (char*)"Working Hours changed successfully";	 
	 case CDCA_RC_CARD_INVALID:
		 return (char*)"Invalid Smartcard";
	 case CDCA_RC_WORKTIME_INVALID:
		 return (char*)"Invalid working hours";
	 case CDCA_RC_PIN_INVALID:
		 return (char*)"Invalid PIN";
	 default:
		 return (char*)"Time Setting Fail";
	 }

     return NULL;
}








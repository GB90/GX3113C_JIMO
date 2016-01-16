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
#include "app_divitone_cas_api_worktime.h"

unsigned char  StartHour;
unsigned char  StartMin;
unsigned char  StartSec;
unsigned char  EndHour;
unsigned char  EndMin;
unsigned char  EndSec;

UINT8 Repeattime;

uint8_t app_divitone_cas_api_init_worktime_data(void)
{
	UINT8  WorkTimeEnable=1;
	UINT8 BeginTime[3];
	UINT8 EndTime[3];
	UINT8 tmp1,tmp2;

	StartHour=0;
	StartMin=0;
	StartSec=0;
	EndHour=0;
	EndMin=0;
	EndSec=0;
	
	if(0==DIVI_GetSMCWorkTime(&WorkTimeEnable,BeginTime,EndTime))
	{
		tmp1=(BeginTime[0]&0xf0)>>4;   
		tmp2=BeginTime[0]&0x0f;	
		StartHour=tmp1*10+tmp2;	
		
		tmp1=(BeginTime[1]&0xf0)>>4;   
		tmp2=BeginTime[1]&0x0f;	
		StartMin=tmp1*10+tmp2;	

		tmp1=(EndTime[0]&0xf0)>>4;   
		tmp2=EndTime[0]&0x0f;	
		EndHour=tmp1*10+tmp2;

		tmp1=(EndTime[1]&0xf0)>>4;   
		tmp2=EndTime[1]&0x0f;	
		EndMin=tmp1*10+tmp2;			
	}

	return 0;
}

char * app_divitone_cas_api_get_worktime_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0;
    	static char s_text_buffer[30];

	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	memset(s_text_buffer,0,sizeof(s_text_buffer));
	
	switch(ID)
		{
			case DIVI_WORKTIME_START_ID:
				sprintf(s_text_buffer, "%02d:%02d", StartHour,StartMin);
				return s_text_buffer;
				break;
			case DIVI_WORKTIME_END_ID:
				sprintf(s_text_buffer, "%02d:%02d", EndHour,EndMin);
				return s_text_buffer;
				break;
			default:
				break;
		}
	return NULL;

}

char* app_divitone_cas_api_change_worktime(ca_work_time_t* data)
{
	char StartTime[20]={0,};
	char StopTime[20]={0,};
//    	unsigned char i;
	char  StartTimeHour[3];
	char  StartTimeMin[3];
//    	char  StartTimeSec[3];
	char  EndTimeHour[3];
	char  EndTimeMin[3];
//    	char  EndTimeSec[3];

    	INT32  wRet;
	uint8_t PinValue[6+1]={0};

	UINT8 BeginTime[3];
	UINT8 EndTime[3];
	UINT8 WorkTimeEnable=1;
//	UINT8 Repeattime;

	UINT8 BCD_10,BCD_1;

	if (NULL == data)
		return NULL;

	 memcpy(PinValue,data->pin,6);
	 if(strlen((char*)PinValue)!=6)
	 {
		 return (char*)"Input new PIN";	 
	 }
	 
//	 for( i = 0; i < 6; i++ )
//	 {
//		 PinValue[i] -= 0x30;
//	 }
	 memcpy(StartTime,data->starttime,5);
	 memcpy(StopTime,data->endtime,5);
	 
	 StartTimeHour[2] = '\0';
	 memcpy(StartTimeHour, StartTime, 2);
	 StartTimeMin[2] = '\0';
	 memcpy(StartTimeMin, StartTime + 3, 2);
//	 StartTimeSec[2] = '\0';
//	 memcpy(StartTimeSec, StartTime + 6, 2);
	 
	 EndTimeHour[2] = '\0';
	 memcpy(EndTimeHour, StopTime, 2);
	 EndTimeMin[2] = '\0';
	 memcpy(EndTimeMin, StopTime + 3, 2);
//	 EndTimeSec[2] = '\0';
//	 memcpy(EndTimeSec, EndTime + 6, 2);
	 
	 BeginTime[0] = atoi(StartTimeHour);
	 BeginTime[1] = atoi(StartTimeMin);
	 BeginTime[2] = 0;//atoi(StartTimeSec);
	 EndTime[0] = atoi(EndTimeHour);
	 EndTime[1] = atoi(EndTimeMin);
	 EndTime[2] = 0;//atoi(EndTimeSec);

	 BCD_10=BeginTime[0]/10;
	 BCD_1=BeginTime[0]-BCD_10*10;					 
	 BeginTime[0]=(BCD_10<<4)|BCD_1;

	 BCD_10=BeginTime[1]/10;
	 BCD_1=BeginTime[1]-BCD_10*10;		
	 BeginTime[1]=(BCD_10<<4)|BCD_1;

	 BCD_10=EndTime[0]/10;
	 BCD_1=EndTime[0]-BCD_10*10;					 
	 EndTime[0]=(BCD_10<<4)|BCD_1;

	 BCD_10=EndTime[1]/10;
	 BCD_1=EndTime[1]-BCD_10*10;		
	 EndTime[1]=(BCD_10<<4)|BCD_1;

	Repeattime = 0;
 	wRet = DIVI_SetWorkPeriod(PinValue,BeginTime,EndTime,WorkTimeEnable,&Repeattime);
	 
	if(wRet == 0)
		return (char*)"Set Successfully!";
	else
		return (char*)"PIN Error!";

     return NULL;
}








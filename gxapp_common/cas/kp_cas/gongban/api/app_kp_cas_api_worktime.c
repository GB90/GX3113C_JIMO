#include "KPAPP.h"
#include "app_common_porting_stb_api.h"
#include "app_kp_cas_api_worktime.h"

unsigned char  StartHour;
unsigned char  StartMin;
unsigned char  StartSec;
unsigned char  EndHour;
unsigned char  EndMin;
unsigned char  EndSec;


uint8_t app_kp_cas_api_init_worktime_data(void)
{
	U8 BeginTime[3];
	U8 EndTime[3];

	StartHour=0;
	StartMin=0;
	StartSec=0;
	EndHour=0;
	EndMin=0;
	EndSec=0;
	
	if(KP_RET_SUCCESS == KPCAAPP_GetWorkTime(BeginTime,BeginTime+1,BeginTime +2,EndTime,EndTime+1,EndTime+2))
	{

		StartHour = BeginTime[0];	
		StartMin =BeginTime[1];	
        StartSec = BeginTime[2];	

        EndHour = EndTime[0];	
		EndMin =EndTime[1];	
        EndSec = EndTime[2];		
	}

	return 0;
}

char * app_kp_cas_api_get_worktime_data(ca_get_date_t *data)
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
		case KP_WORKTIME_START_ID:
			sprintf(s_text_buffer, "%02d:%02d", StartHour,StartMin);
			return s_text_buffer;
			break;
		case KP_WORKTIME_END_ID:
			sprintf(s_text_buffer, "%02d:%02d", EndHour,EndMin);
			return s_text_buffer;
			break;
		default:
			break;
	}
	return NULL;

}

char* app_kp_cas_api_change_worktime(ca_work_time_t* data)
{
	char StartTime[20]={0,};
	char StopTime[20]={0,};
	U8  BeginTime[3]={0};
	U8  EndTime[3]={0};
    char temp[8];
    U8  wRet;
	U8 PinValue[6+1]={0};


	if (NULL == data)
		return NULL;

	 memcpy(PinValue,data->pin,6);
	 if(strlen((char*)PinValue)!=6)
	 {
		 return (char*)"Input new PIN";	 
	 }
	 
	 strcpy(StartTime,data->starttime);
	 strcpy(StopTime,data->endtime);

	 temp[2] = '\0';
	 memcpy(temp, StartTime, 2);
     BeginTime[0] = atoi(temp);
     
	 temp[2] = '\0';
	 memcpy(temp, StartTime + 3, 2);
     BeginTime[1] = atoi(temp);

	 //temp[2] = '\0';
	 //memcpy(temp, StartTime + 6, 2);
     //BeginTime[2] = atoi(temp);

     temp[2] = '\0';
	 memcpy(temp, StopTime, 2);
     EndTime[0] = atoi(temp);
     
	 temp[2] = '\0';
	 memcpy(temp, StopTime + 3, 2);
     EndTime[1] = atoi(temp);
     
	 //temp[2] = '\0';
	 //memcpy(temp, StopTime + 6, 2);
     //EndTime[2] = atoi(temp);
     
 	 wRet = KPCAAPP_SetWorkTime(PinValue,BeginTime[0],BeginTime[1],BeginTime[2],
 	                EndTime[0],EndTime[1],EndTime[2]);
	 
	if(wRet == KP_RET_SUCCESS)
		return (char*)"Set Successfully";
	else if(wRet == KP_RET_PIN_ERROR)
		return (char*)"PIN Error";
     return NULL;
}








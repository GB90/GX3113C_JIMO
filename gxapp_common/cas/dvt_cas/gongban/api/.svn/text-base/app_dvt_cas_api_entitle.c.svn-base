/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_dvt_cas_api_entitle.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.12.03		  zhouhm 	 			creation
*****************************************************************************/
//#include "BYCAca_stb.h"
#include "app_common_porting_stb_api.h"
#include "app_dvt_cas_api_entitle.h"
#include "app_dvt_cas_api_operator.h"
#include "app_common_epg.h"
#include "DVTCAS_STBInf.h"

SDVTCAServiceEntitle  gServiceEntitles[250] ;
BYTE pEntitleCount = 250;
extern uint32_t operator_sel;

union chartoint
{
	int utc;
	char str[4];
};
extern 	SDVTCATvsInfo pOperatorInfo[5];
bool GetDateStrFromValC(int wInputDate,char * szOut)
{/*szOut memory is provided by outside,and its out format is like 2002-5-22.*/
	int nYear,nMon,nDay,nTemp,nDate = wInputDate;
	char sz[30];
	nYear = 1970;
	do{/*year.*/
		if((nYear%4 == 0 && nYear%100 != 0) || nYear%400 == 0)
			nTemp = 366;
		else
			nTemp = 365;
		if(nDate - nTemp < 0)
		break;
		nDate -= nTemp;
		nYear ++;
	}while(true);
	nMon = 1;
	do{/*month.*/
	if(2 == nMon)
		{/* if is Feb.*/
			if((nYear%4 == 0 && nYear%100 != 0) || nYear%400 == 0) /*if leap.*/
				nTemp = 29;
			else
				nTemp = 28;
		}
	else if(4 == nMon || 6 == nMon || 9 == nMon || 11 == nMon)
		nTemp = 30;
	else
		nTemp = 31;
	if(nDate - nTemp < 0)
	break;
	nDate -= nTemp;
	nMon ++;
	}while(true);
	if(1 > nMon || 12 < nMon)
	return false;
	nDay = 1;
	nDay += nDate; /*day.*/
	if(1 > nDay || 31 < nDay)
	return false;
	memset(sz,0,sizeof(sz));
	sprintf(sz,"%d-%02d-%02d",nYear,nMon,nDay);
	strcpy(szOut,sz);
	return true;
}

bool GetTimeStrFromValC(uint32_t nInputTime,char * szOut)
{/*szOut memory is provided by outside,and its out format is like 2002-5-22 12:3:5.*/
	int nDate,nHour,nMin,nSec,nOff,nTime;
	char szTime[40];
	nDate = nInputTime / (24*3600);
	nTime = nInputTime % (24*3600);
	nHour = nTime / 3600;
	if(24 <= (nHour+8)) //to local hour.
		nDate ++;
	if(!GetDateStrFromValC(nDate,szTime))
		return false;
		nOff = strlen(szTime);
	if(8 > nOff)
		return false;
		nTime = nInputTime % 3600;
		nMin = nTime / 60;
	if(0 > nMin || 59 < nMin)
		return false;
		nSec = nTime % 60;
	if(0 > nSec || 59 < nSec)
		return false;
		sprintf(szTime + nOff," %02d:%02d:%02d",(nHour+8)%24,nMin,nSec);
		strcpy(szOut,szTime);
return true;
}

bool GetDateStrFromValC1(int wInputDate,char * szOut)
{/*szOut memory is provided by outside,and its out format is like 2002-5-22.*/
	int nYear,nMon,nDay,nTemp,nDate = wInputDate;
	char sz[30];
	nYear = 1970;
	do{/*year.*/
		if((nYear%4 == 0 && nYear%100 != 0) || nYear%400 == 0)
			nTemp = 366;
		else
			nTemp = 365;
		if(nDate - nTemp < 0)
		break;
		nDate -= nTemp;
		nYear ++;
	}while(true);
	nMon = 1;
	do{/*month.*/
	if(2 == nMon)
		{/* if is Feb.*/
			if((nYear%4 == 0 && nYear%100 != 0) || nYear%400 == 0) /*if leap.*/
				nTemp = 29;
			else
				nTemp = 28;
		}
	else if(4 == nMon || 6 == nMon || 9 == nMon || 11 == nMon)
		nTemp = 30;
	else
		nTemp = 31;
	if(nDate - nTemp < 0)
	break;
	nDate -= nTemp;
	nMon ++;
	}while(true);
	if(1 > nMon || 12 < nMon)
	return false;
	nDay = 1;
	nDay += nDate; /*day.*/
	if(1 > nDay || 31 < nDay)
	return false;
	
	memset(sz,0,sizeof(sz));
	sprintf(sz,"%02d-%02d-%02d",nYear,nMon,nDay);
	strcpy(szOut,sz);
	return true;
}

bool GetTimeStrFromValC1(uint32_t nInputTime,char * szOut)
{/*szOut memory is provided by outside,and its out format is like 2002-5-22 12:3:5.*/
	int nDate,nHour,nMin,nSec,nOff,nTime;
	char szTime[40];
	nDate = nInputTime / (24*3600);
	nTime = nInputTime % (24*3600);
	nHour = nTime / 3600;
	if(24 <= (nHour+8)) //to local hour.
		nDate ++;
	if(!GetDateStrFromValC1(nDate,szTime))
		return false;
		nOff = strlen(szTime);
		nTime = nInputTime % 3600;
		nMin = nTime / 60;
	if(0 > nMin || 59 < nMin)
		return false;
		nSec = nTime % 60;
	if(0 > nSec || 59 < nSec)
		return false;

	sprintf(szTime + nOff," %02d:%02d:%02d",(nHour+8)%24,nMin,nSec);
	strcpy(szOut,szTime);
	
	return true;
}



int32_t app_dvt_cas_api_init_entitle_data(void)
{
	memset(&gServiceEntitles[0],0,250*sizeof(SDVTCAServiceEntitle));
	pEntitleCount = 250;
	if (DVTCA_OK == DVTCASTB_GetServiceEntitles(pOperatorInfo[operator_sel].m_wTVSID, &pEntitleCount, &gServiceEntitles[0]))
	{
		return 1;		
	}
	return 0;	
}

char * app_dvt_cas_api_get_entitle_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0; 

	static char cTextBuffer_Time[40] = {0};
	static char cTextBuffer_EndTime[40] = {0};
	static char cTextBuffer_StartTime[40] = {0};
	static char cTextBuffer_Name[40] = {0};
	static char cTextBuffer_ID[10] = {0};

	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	switch(ID)
		{
			case DVT_ENTITLE_NAME_ID:
				memset(cTextBuffer_Name,0,sizeof(cTextBuffer_Name));
				printf("m_szProductName = %s\n",gServiceEntitles[pos].m_szProductName);
				strcpy(cTextBuffer_Name,gServiceEntitles[pos].m_szProductName);	
				return cTextBuffer_Name;
			case DVT_ENTITLE_ID_ID:
				memset(cTextBuffer_ID,0,sizeof(cTextBuffer_ID));
				sprintf(cTextBuffer_ID,"%d",gServiceEntitles[pos].m_wProductID);
				return cTextBuffer_ID;
			case DVT_ENTITLE_AUTHORIZE_TIME_ID:
				memset(cTextBuffer_Time,0,sizeof(cTextBuffer_Time));
				GetTimeStrFromValC1(gServiceEntitles[pos].m_tEntitleTime,cTextBuffer_Time);
				return cTextBuffer_Time;
			case DVT_ENTITLE_START_TIME_ID:
				memset(cTextBuffer_StartTime,0,sizeof(cTextBuffer_StartTime));
				GetTimeStrFromValC1(gServiceEntitles[pos].m_tStartTime,cTextBuffer_StartTime);
				return cTextBuffer_StartTime;
			case DVT_ENTITLE_END_TIME_ID:
				memset(cTextBuffer_EndTime,0,sizeof(cTextBuffer_EndTime));
				GetTimeStrFromValC1(gServiceEntitles[pos].m_tEndTime,cTextBuffer_EndTime);
				return cTextBuffer_EndTime;			
			default:
				break;
		}
	return NULL;

}

uint8_t app_dvt_cas_api_get_entitle_count(ca_get_count_t *data)
{
	printf("%s CaMenu_number=%d\n",__func__,pEntitleCount);	
	data->totalnum = pEntitleCount;
    return pEntitleCount;
}






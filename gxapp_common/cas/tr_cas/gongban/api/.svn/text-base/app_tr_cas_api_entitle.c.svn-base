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
#include "Tr_Cas.h"
#include "app_common_porting_stb_api.h"
#include "app_tr_cas_api_entitle.h"


#define TRCA_MAXNUM_SERVICE 	 	(320)

typedef struct _TRCAEntitle
{
	unsigned short 		    m_dwProductID;		/*普通授权的节目ID*/
	CAS_TIMESTAMP           m_tBeginDate;		/*授权的起始时间*/
	CAS_TIMESTAMP		    m_tExpireDate;		/*授权的过期时间*/
    unsigned int            m_nRemainTime;      
	unsigned char 		    m_chEntitleStatus;		/*授权状态*/
	unsigned char 		    m_chReaded   :4;		/*授权状态*/
	unsigned char 		    m_chRetValue :4;		/*授权状态*/
}STRCAEntitle;

static STRCAEntitle s_entitleProg[TRCA_MAXNUM_SERVICE] = 
{
    {0}
};

static int s_entitleCount = TRCA_MAXNUM_SERVICE;


unsigned short gx_ca_entitle_read_data( unsigned short wReadTimes )
{
	CAS_TIMESTAMP chStartTime;
	CAS_TIMESTAMP chEndTime;
	uint32_t chRemainTime = 0;
	unsigned short wErr = 1;
    
	memset(&chStartTime, 0, sizeof(CAS_TIMESTAMP));
	memset(&chEndTime, 0, sizeof(CAS_TIMESTAMP));
	chRemainTime = 0;
	
	wErr = MC_TIME_GetEntitleInfo((U8*)&chStartTime,(U8*)&chEndTime,
							(U32*)&chRemainTime,wReadTimes);
    s_entitleProg[wReadTimes].m_dwProductID = wReadTimes;
    s_entitleProg[wReadTimes].m_tExpireDate = chEndTime;
    s_entitleProg[wReadTimes].m_nRemainTime = chRemainTime;
    s_entitleProg[wReadTimes].m_tBeginDate = chStartTime;
	s_entitleProg[wReadTimes].m_chEntitleStatus = wErr;
	s_entitleProg[wReadTimes].m_chReaded = 1;
	s_entitleProg[wReadTimes].m_chRetValue = wErr;

	return 0;
}

int32_t app_tr_cas_api_init_entitle_data(void)
{
	memset(s_entitleProg, 0, sizeof(STRCAEntitle)*TRCA_MAXNUM_SERVICE);
	
	return 1;	
}

char* app_tr_cas_api_get_entitle_data(ca_get_date_t *data)
{
	uint32_t pos = 0;                             
	uint32_t ID = 0; 
	char *pStr = NULL;
	unsigned short chReaded = 0;
	unsigned short chEntitleStatus;
	
	static char cTextBuffer_ID[10] = {0};
	static char cTextBuffer_State[40] = {0};
	static char cTextBuffer_Dates[60] = {0};
	static char cTextBuffer_LeftDay[30] = {0};

	if (NULL == data)
	{
		return NULL;
	}

	pos = data->pos;
	ID = data->ID;
	chReaded = s_entitleProg[pos].m_chReaded;
	
	if (0 == chReaded)
	{
		gx_ca_entitle_read_data(pos);
	}
	
	chEntitleStatus = s_entitleProg[pos].m_chEntitleStatus;
	
	switch (ID)
	{
		case TR_ENTITLE_INDEX_ID:
			memset(cTextBuffer_ID,0,sizeof(cTextBuffer_ID));
			sprintf((void*)cTextBuffer_ID, "%d", s_entitleProg[pos].m_dwProductID + 1);
			return cTextBuffer_ID;
			
		case TR_ENTITLE_STATE_ID:
			memset(cTextBuffer_State, 0, sizeof(cTextBuffer_State));
			if (chEntitleStatus == 0)
			{
				pStr = "Valid Entitle";
			}
			else if(chEntitleStatus == 1)
			{
				pStr = "Para Error";
			}
			else if(chEntitleStatus == 2)
			{
				pStr = "No Entitle";
			}
			else if(chEntitleStatus == 3)
			{
				pStr = "Limited Entitle";
			}
			else if(chEntitleStatus == 4)
			{
				pStr = "Entitle Closed";
			}
			else if(chEntitleStatus == 5)
			{
				pStr = "Entitle Paused";
			}
			else
			{
				pStr = "Unkown";
			}
			
			if (pStr != NULL)
			{
				strcpy(cTextBuffer_State, pStr);
			}
			
			return cTextBuffer_State;
			
		case TR_ENTITLE_DATES_ID:
			memset(cTextBuffer_Dates, 0, sizeof(cTextBuffer_Dates));
			if (2 != chEntitleStatus)
			{			
				sprintf((void *)cTextBuffer_Dates, "%02d%02d/%02d/%02d-%02d%02d/%02d/%02d",
					     s_entitleProg[pos].m_tBeginDate.bYear[0], 
			             s_entitleProg[pos].m_tBeginDate.bYear[1],
			             s_entitleProg[pos].m_tBeginDate.bMonth, 
			             s_entitleProg[pos].m_tBeginDate.bDay, 
			             s_entitleProg[pos].m_tExpireDate.bYear[0], 
			             s_entitleProg[pos].m_tExpireDate.bYear[1],
			             s_entitleProg[pos].m_tExpireDate.bMonth, 
			             s_entitleProg[pos].m_tExpireDate.bDay);
			}
			else
			{
				sprintf((void*)cTextBuffer_Dates, "%s","Unkown");
			}
			return cTextBuffer_Dates;
			
		case TR_ENTITLE_LEFTDAY_ID:
			memset(cTextBuffer_LeftDay, 0, sizeof(cTextBuffer_LeftDay));
			if (2 != chEntitleStatus)
			{
				sprintf((void*)cTextBuffer_LeftDay, "%d", s_entitleProg[pos].m_nRemainTime);
			}
			else
			{
				sprintf((void*)cTextBuffer_LeftDay, "%s","Unkown");
			}
			return cTextBuffer_LeftDay;	
			
		default:
			break;
	}
	
	return NULL;
}

uint8_t app_tr_cas_api_get_entitle_count(ca_get_count_t *data)
{
	data->totalnum = s_entitleCount;
    return s_entitleCount;
}



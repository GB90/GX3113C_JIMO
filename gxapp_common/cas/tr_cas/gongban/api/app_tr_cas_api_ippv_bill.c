/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_tr_cas_api_ippv_bill.c
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
#include "Tr_Cas.h"
#include "app_common_porting_stb_api.h"
#include "app_tr_cas_api_ippv_bill.h"

#define TRCA_MAXLEN_IPPVINDEX   (49)

static CAS_IPPRECORD_STRUCT  s_ippRecordData[TRCA_MAXLEN_IPPVINDEX] = {{0,},};

unsigned int s_nIppvRecordCount = 0;

unsigned int gx_ca_ippv_record_read_data( void )
{
	unsigned short chErr = 1;
	int i = 0;
	int j = 0;
	CAS_IPPRECORD_STRUCT IppvRecord_t[TRCA_MAXLEN_IPPVINDEX] =
    {
        {0}
    };

	s_nIppvRecordCount = 0;
	
	for (i = 0; i < TRCA_MAXLEN_IPPVINDEX; i++)
	{
		chErr = MC_GetIppRecord(&IppvRecord_t[i], i);
		if (chErr != MC_OK)
		{
			break;
		}
		else
		{
			memcpy(&s_ippRecordData[j], &IppvRecord_t[i], sizeof(CAS_IPPRECORD_STRUCT));
			j++;
		}
	}
	
	s_nIppvRecordCount = j;
    return s_nIppvRecordCount;
}

int32_t app_tr_cas_api_init_ippv_bill_data(void)
{
	gx_ca_ippv_record_read_data();
	return 1;
}                      

char* app_tr_cas_api_get_ippv_bill_data(ca_get_date_t *data)
{
	uint32_t pos = 0;                             
	uint32_t ID = 0;  
	char bChinese = 0;
	char* osd_language = app_flash_get_config_osd_language();
	
	if (strcmp(osd_language, LANGUAGE_CHINESE) == 0)
	{
		bChinese = 1;
	}
	
    static char cText_BillID[40] =
    {
        0
    };
	
    static char cText_billTIME[40] =
    {
        0
    };
    static char cText_billStatus[40] =
    {
        0
    };
    
    static char cText_billFLOW[40] =
    {
        0
    };
    
    static char cText_billprice[40] =
    {
        0
    };

	if (NULL == data)
	{
		return NULL;
	}

	pos = data->pos;
	ID = data->ID;

	switch(ID)
	{
		case TR_IPP_BILL_ID:
			memset(cText_BillID, 0x00, 40);
			sprintf( cText_BillID, "%d", pos+1);
			return cText_BillID;
			
		case TR_IPP_TIME_ID:
			memset(cText_billTIME, 0x00, 40);
			sprintf(cText_billTIME, "%02d%02d.%02d.%02d-%02d:%02d:%02d", 
			         s_ippRecordData[pos].ulExchTime.bYear[0], 
			         s_ippRecordData[pos].ulExchTime.bYear[1],
			         s_ippRecordData[pos].ulExchTime.bMonth, 
			         s_ippRecordData[pos].ulExchTime.bDay,
			         s_ippRecordData[pos].ulExchTime.bHour,
			         s_ippRecordData[pos].ulExchTime.bMinute,
			         s_ippRecordData[pos].ulExchTime.bSecond
			         );
			return cText_billTIME;
			
		case TR_IPP_STATUS_ID:
			memset(cText_billStatus, 0x00, 40);
			if (0 == s_ippRecordData[pos].bStateFlag)
			{
				if (bChinese)
				{
					strcpy(cText_billStatus,"支出");
				}
				else
				{
					strcpy(cText_billStatus,"Consume");
				}
			}
			else if (1 == s_ippRecordData[pos].bStateFlag)
			{
				if (bChinese)
				{
					strcpy(cText_billStatus,"存入");
				}
				else
				{
					strcpy(cText_billStatus,"Charge");
				}
			}
			else
			{
				if (bChinese)
				{
					strcpy(cText_billStatus,"未知");
				}
				else
				{
					strcpy(cText_billStatus,"Unkown");
				}
			}
			return cText_billStatus;
			
		case TR_IPP_FLOW_ID:
			memset(cText_billFLOW, 0x00, 40);
			sprintf(cText_billFLOW, "%010d", (unsigned int)s_ippRecordData[pos].ulExchRunningNum);
			return cText_billFLOW;
			
		case TR_IPP_PRICE_ID:
			memset(cText_billprice, 0x00, 40);
			sprintf(cText_billprice, "%d", (unsigned int)s_ippRecordData[pos].ulExchValue);
			return cText_billprice;
			
		default:
			break;
	}
	
	return NULL;
}

uint8_t app_tr_cas_api_get_ippv_bill_count(ca_get_count_t *data)
{
	if (NULL == data)
	{
		return 0;
	}
	
	data->totalnum = s_nIppvRecordCount;
    return s_nIppvRecordCount;
}



/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_tr_cas_api_ippv_slot.c
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
#include "app_tr_cas_api_ippv_slot.h"


#define TRCA_MAXLEN_PURSE   (1) 

static CAS_EPURSEINFO_STRUCT s_ePurseInfo[TRCA_MAXLEN_PURSE] = {{0,},};
static unsigned char s_dIppvSlotCount = 7;


unsigned int gx_ca_ippv_slot_read_data(void)
{
    unsigned char chIndex = 0;
	unsigned short wErr   = 1;
	unsigned char chPurseNum = 0;

	s_dIppvSlotCount = 0;
	memset(s_ePurseInfo, 0, TRCA_MAXLEN_PURSE * sizeof(CAS_EPURSEINFO_STRUCT));
	
	for (chIndex = 0; chIndex < TRCA_MAXLEN_PURSE; chIndex++)
	{
		wErr = MC_GetEPurseState(s_ePurseInfo, chIndex);
		if (wErr == 0)
		{
			++chPurseNum;
		}
	}
	
	s_dIppvSlotCount = 1;
	return 0;
}

int32_t app_tr_cas_api_init_ippv_slot_data(void)
{
	gx_ca_ippv_slot_read_data();
	return 1;
}                      

char* app_tr_cas_api_get_ippv_slot_data(ca_get_date_t *data)
{
	uint32_t pos = 0;                             
	uint32_t ID = 0;  
	
    static char cText_SlotID[40] = {0};
    static char   cText_Balance[40] = {0};
    static char   cText_LineCredit[40] = {0};

	if (NULL == data)
	{
		return NULL;
	}

	pos = data->pos;
	ID = data->ID;

	switch (ID)
	{
		case TR_SLOT_ID:
			memset(cText_SlotID,0x00,40);
			sprintf( cText_SlotID, "%d", pos+1);
			return cText_SlotID;
			
		case TR_BALANCE_ID:
			memset(cText_Balance,0x00,40);
			sprintf(cText_Balance, "%d", (unsigned int)s_ePurseInfo[pos].ulCashValue);
			return cText_Balance;
			
		case TR_CREDIT_LINE_ID:
			memset(cText_LineCredit,0x00,40);
			sprintf(cText_LineCredit, "%d", (unsigned int)s_ePurseInfo[pos].ulCreditValue);
			return cText_LineCredit;
			
		default:
			break;
	}
	
	return NULL;
}

uint8_t app_tr_cas_api_get_ippv_slot_count(ca_get_count_t *data)
{
	if (NULL == data)
	{
		return 0;
	}
	
	data->totalnum = s_dIppvSlotCount;
    return s_dIppvSlotCount;
}



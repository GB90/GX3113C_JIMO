/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_tr_cas_api_ippv_info.c
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
#include "app_tr_cas_api_ippv_info.h"


#define TRCA_MAXLEN_IPPVINDEX   (49)

static CAS_IPPPRODUCT_STRUCT s_ippProductData[TRCA_MAXLEN_IPPVINDEX] = {{0,},};
static unsigned int s_nIppvOrderCount1 = 0;


unsigned int gx_ca_ippv_order_info_read_data(void)
{	
    unsigned short chErr = 1;
	int i = 0;
	int j = 0;
	CAS_IPPPRODUCT_STRUCT Ippv_t[TRCA_MAXLEN_IPPVINDEX] = {{0,}};

	s_nIppvOrderCount1 = 0;
	
	for( i = 0; i < TRCA_MAXLEN_IPPVINDEX; i++ )
	{
		chErr = MC_GetIppProduct(&Ippv_t[i], i);
		if (chErr != MC_OK)
		{
			break;
		}
		else
		{
			memcpy(&s_ippProductData[j], &Ippv_t[i], sizeof(CAS_IPPPRODUCT_STRUCT));
			j++;
		}
	}
	
	s_nIppvOrderCount1 = j;
    return s_nIppvOrderCount1;
}

int32_t app_tr_cas_api_init_ippv_info_data(void)
{
	memset(s_ippProductData, 0, sizeof(CAS_IPPPRODUCT_STRUCT)*TRCA_MAXLEN_IPPVINDEX);   
	gx_ca_ippv_order_info_read_data();
	
    return 1;
}                      

char* app_tr_cas_api_get_ippv_info_data(ca_get_date_t *data)
{
	uint32_t pos = 0;                             
	uint32_t ID = 0;  
	
    static char cText_order_oper[40] = {0};
    static char cText_order_product[40] = {0};
    static char cText_order_slot[40] = {0};
    static char cText_order_booked[40] = {0};
    static char cText_order_price[40] = {0};
    static char cText_order_tapeable[40] = {0};
    static char cText_order_expired[40] = {0};

	if (NULL == data)
	{
		return NULL;
	}

	pos = data->pos;
	ID = data->ID;

	switch (ID)
	{
		case TR_IPPV_INFO_TVS_ID:
			memset(cText_order_oper, 0x00, 40);
			sprintf(cText_order_oper, "%d", pos+1);
			return cText_order_oper;
			
		case TR_IPPV_INFO_PROGRAM_ID:
			memset(cText_order_product,0x00,40);
			if (0 == s_ippProductData[pos].bProductType)
		    {
		        strcpy(cText_order_product,"IPPV");
		    }
		    else if (1 == s_ippProductData[pos].bProductType)
		    {
		         strcpy(cText_order_product,"IPPT");
		    }
		    else
		    {
		        strcpy(cText_order_product,"Unkown");
		    }
			return cText_order_product;
			
		case TR_IPPV_INFO_SLOT_ID:
			return cText_order_slot;
			
		case TR_IPPV_INFO_STATUS_ID:
			return cText_order_booked;
			break;
			
		case TR_IPPV_INFO_PRICE_ID:
			return cText_order_price;
			
		case TR_IPPV_INFO_TAPEABLE_ID:
			memset(cText_order_tapeable, 0x00, 40);
			sprintf(cText_order_tapeable, "%02d%02d.%d.%d-%02d:%02d:%02d", 
		             s_ippProductData[pos].ulStartTime.bYear[0], 
		             s_ippProductData[pos].ulStartTime.bYear[1],
		             s_ippProductData[pos].ulStartTime.bMonth, 
		             s_ippProductData[pos].ulStartTime.bDay,
		             s_ippProductData[pos].ulStartTime.bHour,
		             s_ippProductData[pos].ulStartTime.bMinute,
		             s_ippProductData[pos].ulStartTime.bSecond
		             );
			return cText_order_tapeable;
			break;
			
		case TR_IPPV_INFO_ENDTIME_ID:
			memset(cText_order_expired, 0x00, 40);
			sprintf(cText_order_expired, "%02d%02d.%d.%d-%02d:%02d:%02d",
		             s_ippProductData[pos].ulEndTime.bYear[0], 
		             s_ippProductData[pos].ulEndTime.bYear[1],
		             s_ippProductData[pos].ulEndTime.bMonth, 
		             s_ippProductData[pos].ulEndTime.bDay,
		             s_ippProductData[pos].ulEndTime.bHour,
		             s_ippProductData[pos].ulEndTime.bMinute,
		             s_ippProductData[pos].ulEndTime.bSecond
		             );
			return cText_order_expired;
			
		default:
			break;
	}
	
	return NULL;
}

uint8_t app_tr_cas_api_get_ippv_info_count(ca_get_count_t *data)
{
	if (NULL == data)
	{
		return 0;
	}
	
	//printf("[app_tr_cas_api_get_ippv_info_count]s_nIppvOrderCount1(%d).\n", s_nIppvOrderCount1);
	data->totalnum = s_nIppvOrderCount1;
    return s_nIppvOrderCount1;
}



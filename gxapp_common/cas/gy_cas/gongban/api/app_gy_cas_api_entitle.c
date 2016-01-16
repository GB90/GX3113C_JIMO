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
#include <gxtype.h>
#include "gxmsg.h"
#include "gxapp_sys_config.h"
#include "app_common_play.h"
#include "app_common_epg.h"
#include "app_common_table_pmt.h"
#include "app_common_porting_stb_api.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_porting_ca_smart.h"
	
#include "gyca_def.h"
#include "gyca_pub.h"
#include "gyca_osp.h"
#include "gyca_interface.h"
#include "gyca_porting.h"

#include "app_gy_cas_api_entitle.h"	
#include "app_gy_cas_api_pin.h"
#include "app_gy_cas_api_smart.h"
#include "app_gy_cas_api_demux.h"
#include "app_gy_cas_api_baseinfo.h"


static int ProductNum=0;
static T_CAProductEntitleInfo pProductEntitles;


int32_t app_gy_cas_api_init_entitle_data(void)
{   
	memset(&pProductEntitles,0x00, sizeof(T_CAProductEntitleInfo));
	
    ProductNum = GYCA_GetProductEntitles(0,&pProductEntitles);

    return ProductNum;
}

 void MjdToUtc(uint32_t mjd,com_time_t * m_utc)
{

	int k, mp, yp;
	if(m_utc == NULL)
		return;
	yp = ((double)mjd - 15078.2)/365.25;
	mp = ((double)mjd - 14956.1 - yp * 365.25)/30.6001;
	m_utc->day = mjd - 14956 - (yp * 365.25) - (mp * 30.6001);

	if(mp == 14 || mp == 15)
		k = 1;
	else
		k = 0;

	m_utc->year = yp + k + 1900;
	m_utc->month = mp - 1 - k * 12;
}


/***********************************************************************
  Convert MJD format to Date (tm) Function
***********************************************************************/
void _Mjd2Date( uint8_t u8D1, uint8_t u8D2, com_time_t * pstTM )
{
	//s32 mjd, YY, MM, K, W, Y, M, D, WY, WD;
	int32_t mjd, YY, MM, K, Y, M, D;
	/* date */
	mjd = (u8D1 * 256) + u8D2;
	/* yr, mn, dt, hur, min */
	YY = (int)((mjd - 15078.2) / 365.25);
	MM = (int)((mjd - 14956.1 - (YY * 365.25)) / 30.6001);
	if ((MM == 14) || (MM == 15))
	{
		K = 1;
	}
	else
	{
		K = 0;
	}
	//ignore W and WY
	/*W = (int) ( (mjd / 7.0) - 2144.64 );
	WY = (int) ( (W * 28.0 / 1461) - 0.0079 );*/
	//SYS_DBG(( "mjd=%ld, YY=%ld, MM=%ld, K=%ldn", mjd, YY, MM, K));
	Y = YY + K;
	M = MM - 1 - (K * 12);
	D = mjd - 14956 - (int)(YY * 365.25) - (int)(MM * 30.6001);
	//ignore WY and WD
	/*WY = W - (int) ( (WY * 1461.0 / 28) + 0.41 );
	WD = ( (mjd + 2) % 7 ) + 1;*/
	/*SYS_DBG(("Mjd2Data: X%x X%x => %02d/%02d/%02d %d,%d!n",
	u8D1, u8D2, (int)Y, (int)M, (int)D, WY, WD));*/
	/*SYS_DBG(("Mjd2Data: X%hx X%hx => %02d/%02d/%02d !n",
	u8D1, u8D2, (int)Y, (int)M, (int)D));*/
	pstTM->year= (int)Y+1900;
	pstTM->month= (int)M;
	pstTM->day= (int)D;
}

char * app_gy_cas_api_get_entitle_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0;  
	
    static char   cTextBuffer_pro[40] =
    {
        0
    };
    static char   cTextBuffer_exp[40] =
    {
        0
    };
    static char   cTextBuffer_can_type[40] =
    {
        0
    };

	
	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	switch(ID)
	{
		case GY_ENTITLE_OPERATE_ID:
			sprintf( cTextBuffer_pro, "%d",1);/*Porting is unsuported*/
			return cTextBuffer_pro;
		case GY_ENTITLE_PRODUCT_ID:
		{
			sprintf( cTextBuffer_pro, "%d",pProductEntitles.m_entitleInfo[pos].m_byProductID);
			return cTextBuffer_pro;
		}break;
		case GY_ENTITLE_CAN_TYPE_ID:
		{
			switch(pProductEntitles.m_entitleInfo[pos].m_byComeFrom)
		    {
		    	case 0:
					sprintf(cTextBuffer_can_type,"%s","智能卡授权");/* 0-智能卡授权*/
					break;
				case 1: 
					sprintf(cTextBuffer_can_type,"%s","集团授权");/* 1-集团授权*/
					break;
				case 2:
					sprintf(cTextBuffer_can_type,"%s","主卡授权");/* 2-主卡授权. 备用*/
					break;
		    }
		    return cTextBuffer_can_type;
		}break;
		case GY_ENTITLE_EXPIRT_DATE_ID:
		{
			com_time_t	Time_t;
			printf("m_byDate=0x%x\n",pProductEntitles.m_entitleInfo[pos].m_expireDate.m_byDate);
			/*MjdToUtc(pProductEntitles.m_entitleInfo[pos].m_expireDate.m_byDate,&Time_t);*/
			_Mjd2Date(pProductEntitles.m_entitleInfo[pos].m_expireDate.m_byDate>>8, 
						pProductEntitles.m_entitleInfo[pos].m_expireDate.m_byDate&0xff, 
						&Time_t );
			sprintf( cTextBuffer_exp, "%04d-%02d-%02d", Time_t.year,Time_t.month,Time_t.day);
			
			return cTextBuffer_exp;
		}break;
		default:
			break;
	}
	
	return NULL;

}

uint8_t app_gy_cas_api_get_entitle_count(ca_get_count_t *data)
{
	data->totalnum = ProductNum;
    return ProductNum;
}






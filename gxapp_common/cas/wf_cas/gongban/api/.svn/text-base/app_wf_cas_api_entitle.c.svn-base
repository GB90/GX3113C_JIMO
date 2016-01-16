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
#include "wanfacas.h"
#include "app_common_porting_stb_api.h"
#include "app_wf_cas_api_entitle.h"
#include "app_wf_cas_api_baseinfo.h"
#include "app_common_epg.h"

extern void mjd_to_tm(U16 mjd, CommonTime_t * tm);
extern void utc_to_tm(U32 bcd_time, CommonTime_t * tm);

WFCAS_Entitlement_t  wfca_entitle[350];
U08	CaMenu_number = 0;

int32_t app_wf_cas_api_init_entitle_data(void)
{
	CAS_Dbg("%s\n",__FUNCTION__);
    	U32 wRet = WFERR_COMMON_NO;
	U08 iTmp = 0;

	memset(wfca_entitle,0,350*sizeof(WFCAS_Entitlement_t));
//	memset(&wfca_entitle[iTmp],0,sizeof(wfca_entitle[iTmp]));
	wRet = WFCAS_GetEntitlement(&wfca_entitle[iTmp], FIRST);
	if (wRet != WFERR_COMMON_NO)
	{
		CAS_Dbg("WFCAS_GetEntitlement %d, wRet=%x\n", iTmp, (unsigned int)wRet);
		CaMenu_number = 0;
		return 0;
	}
	else
	{
		iTmp++;
		while(wRet == WFERR_COMMON_NO)
		{
			wRet = WFCAS_GetEntitlement(&wfca_entitle[iTmp], NEXT);
			if (wRet != WFERR_COMMON_NO)
			{
				CAS_Dbg("WFCAS_GetEntitlement %d wRet=%x\n", iTmp, (unsigned int)wRet);
				CaMenu_number = iTmp;
				return 0;
			}
			CAS_Dbg("---开始---iTmp %d wRet=%x\n", iTmp, wfca_entitle[iTmp].Ori_Date);
			CAS_Dbg("---结束---iTmp %d wRet=%x\n", iTmp, wfca_entitle[iTmp].Exp_Date);
			iTmp++;
		}
	}
	return 0;
}


char * app_wf_cas_api_get_entitle_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0; 

	CommonTime_t time_start;
	CommonTime_t time_end;

	static char   cTextBuffer_no[30] ={0};
	static char   cTextBuffer_id[30] ={0};
	static char   cTextBuffer_starttime[30] ={0};
	static char   cTextBuffer_endtime[30] ={0};


	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	switch(ID)
	{
		case WF_ENTITLE_PRODUCT_NO_ID:
			memset(cTextBuffer_no,0,sizeof(cTextBuffer_id));
		     	sprintf( cTextBuffer_no, "%02d",pos);
			return cTextBuffer_no;	
		case WF_ENTITLE_PRODUCT_ID:
			memset(cTextBuffer_id,0,sizeof(cTextBuffer_id));
		     	sprintf( cTextBuffer_id, "%08x",(unsigned int)wfca_entitle[pos].Product_Id);
			return cTextBuffer_id;
		case WF_ENTITLE_PRODUCT_STARTTIME_ID:
			memset(cTextBuffer_starttime,0,sizeof(cTextBuffer_starttime));
			mjd_to_tm(wfca_entitle[pos].Ori_Date, &time_start);
			utc_to_tm(wfca_entitle[pos].Ori_Time, &time_start);
			sprintf(cTextBuffer_starttime, "%d-%02d-%02d %02d:%02d:%02d", 
				time_start.m_nYear, time_start.m_nMonth, time_start.m_nDay, time_start.m_nHour, time_start.m_nMinute, time_start.m_nSecond);
			return cTextBuffer_starttime;
		case WF_ENTITLE_PRODUCT_ENDTIME_ID:
			memset(cTextBuffer_endtime,0,sizeof(cTextBuffer_endtime));
			mjd_to_tm(wfca_entitle[pos].Exp_Date, &time_end);
			utc_to_tm(wfca_entitle[pos].Exp_Time, &time_end);
			sprintf(cTextBuffer_endtime, "%d-%02d-%02d %02d:%02d:%02d", 
				time_end.m_nYear, time_end.m_nMonth, time_end.m_nDay, time_end.m_nHour, time_end.m_nMinute, time_end.m_nSecond);
			return cTextBuffer_endtime;
		default:
			break;
	}
	return NULL;

}

uint8_t app_wf_cas_api_get_entitle_count(ca_get_count_t *data)
{
	printf("app_wf_cas_api_get_entitle_count CaMenu_number=%d\n",CaMenu_number);	
	data->totalnum = CaMenu_number;
    	return CaMenu_number;
}






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
#include "app_common_porting_stb_api.h"
//#include "app_win_interface.h"
#include "app.h"
#include "wanfacas.h"
#include "app_common_epg.h"
#include "app_wf_cas_api_pop.h"


static wf_cas_pptv_buy_st ca_pptv_buy = {0};
/*
* 刷新IPPV购买显示框
*/

int32_t app_wf_cas_gxmsg_ca_on_event_pptv_buy(GxMsgProperty0_OnEvent* event)
{
	wf_cas_pptv_buy_st * pptv_buy = NULL;
	popmsg_ret ret;
	char buf[100]={0};
	char* osd_language=NULL;

	if (NULL == event)
		return 0;

	pptv_buy = (wf_cas_pptv_buy_st*)(event->buf);

	if (NULL == pptv_buy)
		return 0;

	if (TRUE == pptv_buy->wStatus)
	{
		/*需要显示IPPV购买框*/
		memcpy(&ca_pptv_buy,pptv_buy,sizeof(wf_cas_pptv_buy_st));

		if(ca_pptv_buy.wShowStatus == FALSE)
		{
			ca_pptv_buy.wShowStatus = TRUE;
			osd_language = app_flash_get_config_osd_language();
			if(ca_pptv_buy.IsPPV == 1)
			{
				if(0 == strcmp(osd_language,LANGUAGE_CHINESE))
					sprintf((void*)buf,"本节目为:%s节目:\n计价单位:%d 小时/次\n价格:%d\n确认收看吗?","IPPV",ca_pptv_buy.Interval,ca_pptv_buy.UnitPrice);
				else if(0 == strcmp(osd_language,LANGUAGE_ENGLISH))
					sprintf((void*)buf,"PROG:%s:\nUnit of account:%d Hours / time\nPrice:%d\nWatch?","IPPV",ca_pptv_buy.Interval,ca_pptv_buy.UnitPrice);
			}
			else if(ca_pptv_buy.IsPPV == 2)
			{
				if(0 == strcmp(osd_language,LANGUAGE_CHINESE))
					sprintf((void*)buf,"本节目为:%s节目:\n计价单位:%d 秒/次\n价格:%d\n确认收看吗?","IPPT",ca_pptv_buy.Interval,ca_pptv_buy.UnitPrice);
				else if(0 == strcmp(osd_language,LANGUAGE_ENGLISH))
					sprintf((void*)buf,"PROG:%s:\nUnit of account:%d S / time\nPrice:%d\nWatch?","IPPT",ca_pptv_buy.Interval,ca_pptv_buy.UnitPrice);
					
			}
			CAS_Dbg("%s\n",buf);
			ret = app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,buf,POPMSG_TYPE_YESNO);
			if(ret == POPMSG_RET_YES)
			{
				if(ca_pptv_buy.IsPPV == 1)
					WFCAS_SendPPTOkFlag(TRUE);
				else if(ca_pptv_buy.IsPPV == 2)
					WFCAS_SendPPTOkFlag(FALSE);
			}
		}
	
	}
	
	return 1;
}






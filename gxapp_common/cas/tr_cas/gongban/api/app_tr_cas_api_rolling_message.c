/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_tr_cas_api_rolling_message.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.04.16		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_porting_stb_api.h"
#include "app_tr_cas_api_rolling_message.h"


extern int32_t tr_cas_rolling_message_get_times(uint8_t byStyle);
extern 	void tr_cas_rolling_message_show(uint8_t byStyle, uint8_t showFlag, char* message);
extern int32_t tr_cas_rolling_message_get_times(uint8_t byStyle);
extern char* GUI_GetFocusWindow(void);
extern uint8_t app_win_is_full_window(void);
extern void app_tr_cas_osd_roll_timer_check(void);

static TR_CAS_ROLLING_MSG_t s_caRollingMsg[TR_CA_OSD_MAX];


static void trcas_rolling_message_update_data(TR_CAS_ROLLING_MSG_t *pDest, 
	   												TR_CAS_ROLLING_MSG_t *pSource)
{
	if ((NULL == pSource) || (NULL == pDest))
	{
		return;
	}

	pDest->dataCrc32 = pSource->dataCrc32;
    pDest->wStatus = pSource->wStatus; 
    pDest->roll_status = pSource->roll_status;
    pDest->byStyle = pSource->byStyle; 
    pDest->wTimes = pSource->wTimes; 
    pDest->period = pSource->period; 
	pDest->timer_handle = pSource->timer_handle; 
    pDest->len = pSource->len;

	memset(pDest->content, 0, sizeof(pDest->content));
    strncpy((char *)pDest->content, (char *)pSource->content, pSource->len);

	return;
}


int8_t app_tr_cas_rolling_message_init(void)
{
    int i = 0;
	
	memset(&s_caRollingMsg[0], 0, TR_CA_OSD_MAX*sizeof(TR_CAS_ROLLING_MSG_t));
	
    for (i = 0; i < TR_CA_OSD_MAX; i++)
    {
        if (GXCORE_SUCCESS != GxCore_MutexCreate(&s_caRollingMsg[i].mutex))
        {
            printf("[app_tr_cas_rolling_message_init]s_caRollingMsg mutex create failed!!!\n");
        }
    }
  
    return 0;
}

int32_t app_tr_cas_gxmsg_ca_on_event_rolling(GxMsgProperty0_OnEvent* event)
{
	TR_CAS_ROLLING_MSG_t *pRollingMsg = NULL;
	
	if (NULL == event)
	{
		printf("[app_tr_cas_gxmsg_ca_on_event_rolling]param event is NULL!!!\n");
		return 1;
	}

	pRollingMsg = (TR_CAS_ROLLING_MSG_t *)(event->buf);
	if (NULL == pRollingMsg)
	{
		printf("[app_tr_cas_gxmsg_ca_on_event_rolling]pRollingMsg is NULL!!!\n");
		return 2;
	}

	if (TR_CA_OSD_TOP == pRollingMsg->byStyle)	 
	{
	    GxCore_MutexLock(s_caRollingMsg[TR_CA_OSD_TOP].mutex);
        trcas_rolling_message_update_data(&s_caRollingMsg[TR_CA_OSD_TOP], pRollingMsg);
		//printf("++TR_CA_OSD_TOP-String(%s)++\n", s_caRollingMsg[TR_CA_OSD_TOP].content);
		s_caRollingMsg[TR_CA_OSD_TOP].roll_status = FALSE;
		s_caRollingMsg[TR_CA_OSD_TOP].wStatus = TRUE;
        printf("##TR_CA_OSD_TOP times:%d##\n", pRollingMsg->wTimes);
        GxCore_MutexUnlock(s_caRollingMsg[TR_CA_OSD_TOP].mutex);
	}
    else if (TR_CA_OSD_BOTTOM == pRollingMsg->byStyle)	 
	{
        GxCore_MutexLock(s_caRollingMsg[TR_CA_OSD_BOTTOM].mutex);
        trcas_rolling_message_update_data(&s_caRollingMsg[TR_CA_OSD_BOTTOM], pRollingMsg);
        s_caRollingMsg[TR_CA_OSD_BOTTOM].roll_status = FALSE;
		s_caRollingMsg[TR_CA_OSD_BOTTOM].wStatus = TRUE;
        printf("@@TR_CA_OSD_BOTTOM times:%d@@\n", pRollingMsg->wTimes);
        GxCore_MutexUnlock(s_caRollingMsg[TR_CA_OSD_BOTTOM].mutex);
	}    
    
	return 0;
}

int32_t app_tr_cas_rolling_message_exec(void)
{
	int32_t topTimes = 0;
	int32_t bottomTimes = 0;
	char* curFocusWin = (char *)GUI_GetFocusWindow();
	uint8_t bOsdRollStop[2] = {FALSE, FALSE};

	if (0 == app_win_is_full_window())
	{
		return TRUE;
	}

	bOsdRollStop[TR_CA_OSD_TOP] = FALSE;
	bOsdRollStop[TR_CA_OSD_BOTTOM] = FALSE;
	
    GxCore_MutexLock(s_caRollingMsg[TR_CA_OSD_TOP].mutex);
	if (TRUE == s_caRollingMsg[TR_CA_OSD_TOP].wStatus)
	{
		if (FALSE == s_caRollingMsg[TR_CA_OSD_TOP].roll_status)
		{
			if ((NULL != curFocusWin) && (0 == strcasecmp(curFocusWin, "win_full_screen")))
			{
				tr_cas_rolling_message_show(TR_CA_OSD_TOP, DVB_CA_ROLLING_FLAG_RESET,\
				   					 	(char *)s_caRollingMsg[TR_CA_OSD_TOP].content);
 				tr_cas_rolling_message_show(TR_CA_OSD_TOP, DVB_CA_ROLLING_FLAG_SHOW,\
					  					(char *)s_caRollingMsg[TR_CA_OSD_TOP].content);
				s_caRollingMsg[TR_CA_OSD_TOP].roll_status = TRUE;		
			}	
		}
		
		topTimes = tr_cas_rolling_message_get_times(TR_CA_OSD_TOP); 
		if (topTimes >= s_caRollingMsg[TR_CA_OSD_TOP].wTimes)
		{
			s_caRollingMsg[TR_CA_OSD_TOP].wStatus = FALSE;
			s_caRollingMsg[TR_CA_OSD_TOP].roll_status = FALSE;	
			tr_cas_rolling_message_show(TR_CA_OSD_TOP, DVB_CA_ROLLING_FLAG_CLEAR, NULL);
			s_caRollingMsg[TR_CA_OSD_TOP].wTimes = 0;
			memset(&s_caRollingMsg[TR_CA_OSD_TOP].create_time, 0, sizeof(CAS_TIMESTAMP));
			bOsdRollStop[TR_CA_OSD_TOP] = TRUE;
            printf("|+|+|+|+|+|+|-TOP OSD->stop.+.\n");
		}
	}
	else
	{
		if (TRUE == s_caRollingMsg[TR_CA_OSD_TOP].roll_status)
		{
			tr_cas_rolling_message_show(TR_CA_OSD_TOP, DVB_CA_ROLLING_FLAG_HIDE, NULL);
			s_caRollingMsg[TR_CA_OSD_TOP].roll_status = FALSE;
			s_caRollingMsg[TR_CA_OSD_TOP].wTimes = 0;
			memset(&s_caRollingMsg[TR_CA_OSD_TOP].create_time, 0, sizeof(CAS_TIMESTAMP));
		}
	}
    GxCore_MutexUnlock(s_caRollingMsg[TR_CA_OSD_TOP].mutex);

    GxCore_MutexLock(s_caRollingMsg[TR_CA_OSD_BOTTOM].mutex);
	if (TRUE == s_caRollingMsg[TR_CA_OSD_BOTTOM].wStatus)
	{
		if (FALSE == s_caRollingMsg[TR_CA_OSD_BOTTOM].roll_status)
		{		
			if ((NULL != curFocusWin) && (0 == strcasecmp(curFocusWin, "win_full_screen")))
			{
				tr_cas_rolling_message_show(TR_CA_OSD_BOTTOM, DVB_CA_ROLLING_FLAG_RESET,\
				   						(char *)s_caRollingMsg[TR_CA_OSD_BOTTOM].content);
				tr_cas_rolling_message_show(TR_CA_OSD_BOTTOM, DVB_CA_ROLLING_FLAG_SHOW,\
					  					(char *)s_caRollingMsg[TR_CA_OSD_BOTTOM].content);
				s_caRollingMsg[TR_CA_OSD_BOTTOM].roll_status = TRUE;		
			}
		}
		
		bottomTimes = tr_cas_rolling_message_get_times(TR_CA_OSD_BOTTOM);
		if (bottomTimes >= s_caRollingMsg[TR_CA_OSD_BOTTOM].wTimes)
		{
			s_caRollingMsg[TR_CA_OSD_BOTTOM].wStatus = FALSE;
			s_caRollingMsg[TR_CA_OSD_BOTTOM].roll_status = FALSE; 
			tr_cas_rolling_message_show(TR_CA_OSD_BOTTOM, DVB_CA_ROLLING_FLAG_CLEAR, NULL);
			s_caRollingMsg[TR_CA_OSD_BOTTOM].wTimes = 0;
			memset(&s_caRollingMsg[TR_CA_OSD_BOTTOM].create_time, 0, sizeof(CAS_TIMESTAMP));
			bOsdRollStop[TR_CA_OSD_BOTTOM] = TRUE;
			printf("|+|+|+|+|+|+|-BOTTOM OSD->stop.+.\n");
		}
	}
	else
	{
		if (TRUE == s_caRollingMsg[TR_CA_OSD_BOTTOM].roll_status)
		{
			tr_cas_rolling_message_show(TR_CA_OSD_BOTTOM, DVB_CA_ROLLING_FLAG_HIDE, NULL);
			s_caRollingMsg[TR_CA_OSD_BOTTOM].roll_status = FALSE;
			s_caRollingMsg[TR_CA_OSD_BOTTOM].wTimes = 0;
			memset(&s_caRollingMsg[TR_CA_OSD_BOTTOM].create_time, 0, sizeof(CAS_TIMESTAMP));
		}			
	}
    GxCore_MutexUnlock(s_caRollingMsg[TR_CA_OSD_BOTTOM].mutex);

	if (bOsdRollStop[TR_CA_OSD_TOP])
	{
		app_tr_cas_osd_roll_timer_check();
	}
    
	return TRUE;
}

void app_tr_cas_rolling_message_hide(void)
{
	GxCore_MutexLock(s_caRollingMsg[TR_CA_OSD_TOP].mutex);
	if (TRUE == s_caRollingMsg[TR_CA_OSD_TOP].roll_status)
	{
		tr_cas_rolling_message_show(TR_CA_OSD_TOP, DVB_CA_ROLLING_FLAG_RESET,\
				   				(char *)s_caRollingMsg[TR_CA_OSD_TOP].content);
		s_caRollingMsg[TR_CA_OSD_TOP].roll_status = FALSE;
	}
	GxCore_MutexUnlock(s_caRollingMsg[TR_CA_OSD_TOP].mutex);

	GxCore_MutexLock(s_caRollingMsg[TR_CA_OSD_BOTTOM].mutex);
	if (TRUE == s_caRollingMsg[TR_CA_OSD_BOTTOM].roll_status)
	{
		tr_cas_rolling_message_show(TR_CA_OSD_BOTTOM, DVB_CA_ROLLING_FLAG_RESET,\
				   				(char *)s_caRollingMsg[TR_CA_OSD_BOTTOM].content);
		s_caRollingMsg[TR_CA_OSD_BOTTOM].roll_status = FALSE;
	}
	GxCore_MutexUnlock(s_caRollingMsg[TR_CA_OSD_BOTTOM].mutex);

	return;
}

uint8_t app_tr_cas_get_cur_rolling_info(uint8_t osd_type, TR_CAS_ROLLING_MSG_t *rolling_infoPtr)
{
	if ((osd_type >= TR_CA_OSD_MAX) || (NULL == rolling_infoPtr))
	{
		printf("[app_tr_cas_get_cur_osd_info]param is NULL!!!\n");
		return 1;
	}

	memcpy(rolling_infoPtr, &s_caRollingMsg[osd_type], sizeof(TR_CAS_ROLLING_MSG_t));
	return 0;
}



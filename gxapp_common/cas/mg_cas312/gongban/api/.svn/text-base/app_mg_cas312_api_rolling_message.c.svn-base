/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_mg_cas_api_rolling_message.c
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
#include "mg312def.h"
#include "app_common_porting_stb_api.h"
#include "app_mg_cas312_api_pop.h"
#include "app_mg_cas312_api_rolling_message.h"
#include "app_mg_cas312_api_event.h"


extern int32_t mg_cas312_rolling_message_get_times(uint8_t byStyle);
extern 	void mg_cas312_rolling_message_show(uint8_t byStyle, uint8_t showFlag, char* message);
extern int32_t mg_cas_rolling_message_get_times(uint8_t byStyle);
extern char* GUI_GetFocusWindow(void);

mg_cas312_rolling_message_st g_rollingMsg[MG312_MSG_MAX];

static mg_cas312_rolling_message_st s_caRollingMsg[MG312_CA_OSD_BOTTOM];

#define ROLLING_MESSAGE_CPY(pdst,psrc) \
        (pdst)->wStatus = (psrc)->wStatus; \
        (pdst)->roll_status = (psrc)->roll_status; \
        (pdst)->byStyle = (psrc)->byStyle; \
        (pdst)->message_type = (psrc)->message_type; \
        (pdst)->wTimes = (psrc)->wTimes; \
        memcpy((pdst)->content,(psrc)->content,MG312_CA_MAX_LEN_OSD);


/*-----------------------------------------------------------------------------------*/
int8_t app_mg_cas312_rolling_message_init(void)
{
    int i;
	
	memset(&s_caRollingMsg[0], 0, MG312_CA_OSD_BOTTOM*sizeof(mg_cas312_rolling_message_st));
	
    for (i = 0; i < MG312_CA_OSD_BOTTOM; i++)
    {
        if (GXCORE_SUCCESS != GxCore_MutexCreate(&s_caRollingMsg[i].mutex))
        {
            printf("[app_mg_cas312_rolling_message_init]s_caRollingMsg.mutex create failed!!!\n");
        }
    }
    
    memset(&g_rollingMsg[0], 0, MG312_MSG_MAX*sizeof(mg_cas312_rolling_message_st));
    for (i = 0; i < MG312_MSG_MAX; i++)
    {
        if (GXCORE_SUCCESS != GxCore_MutexCreate(&g_rollingMsg[i].mutex))
        {
            printf("[app_mg_cas312_rolling_message_init]g_rollingMsg.mutex create failed.\n");
        }
    }
	
    return 0;
}

int32_t app_mg_cas312_gxmsg_ca_on_event_msg(GxMsgProperty0_OnEvent* event)
{
	mg_cas312_rolling_message_st* rolling_msg = NULL;
	if (NULL == event)
	{
		return 0;
	}

	rolling_msg = (mg_cas312_rolling_message_st *)(event->buf);
	if (NULL == rolling_msg)
	{
		return 0;
	}
    
    GxCore_MutexLock(rolling_msg->mutex);
	if (MG312_MSG_OSD == rolling_msg->message_type)
	{
		app_mg_cas312_gxmsg_ca_on_event_rolling(event);
	}
	else if (MG312_MSG_MSG == rolling_msg->message_type)
	{
		app_mg_cas312_msg_show(event);
	}
	GxCore_MutexUnlock(rolling_msg->mutex);
    
	return 1;
}

int32_t app_mg_cas312_gxmsg_ca_on_event_rolling(GxMsgProperty0_OnEvent* event)
{
	mg_cas312_rolling_message_st* rolling_msg = NULL;
	if (NULL == event)
	{
		return 0;
	}

	rolling_msg = (mg_cas312_rolling_message_st*)(event->buf);
	if (NULL == rolling_msg)
	{
		return 0;
	}

    if (rolling_msg->message_type == MG312_MSG_OSD)
    {
    	if (MG312_CA_OSD_TOP == rolling_msg->byStyle)	 
    	{//复位滚动显示，实际还未显示
    	    GxCore_MutexLock(s_caRollingMsg[MG312_CA_OSD_TOP-1].mutex);
            ROLLING_MESSAGE_CPY(&s_caRollingMsg[MG312_CA_OSD_TOP-1], rolling_msg);
    		s_caRollingMsg[MG312_CA_OSD_TOP-1].roll_status = FALSE;
    		s_caRollingMsg[MG312_CA_OSD_TOP-1].wStatus = TRUE;
            printf("##MG312_CA_OSD_TOP times(%d).##\n", rolling_msg->wTimes);
            GxCore_MutexUnlock(s_caRollingMsg[MG312_CA_OSD_TOP-1].mutex);
    	}
        
    	if (MG312_CA_OSD_BOTTOM == rolling_msg->byStyle)	 
    	{
            GxCore_MutexLock(s_caRollingMsg[MG312_CA_OSD_BOTTOM-1].mutex);
            ROLLING_MESSAGE_CPY(&s_caRollingMsg[MG312_CA_OSD_BOTTOM-1], rolling_msg);
            s_caRollingMsg[MG312_CA_OSD_BOTTOM-1].roll_status = FALSE;
    		s_caRollingMsg[MG312_CA_OSD_BOTTOM-1].wStatus = TRUE;
            printf("@@MG312_CA_OSD_BOTTOM times(%d)@@\n", rolling_msg->wTimes);
            GxCore_MutexUnlock(s_caRollingMsg[MG312_CA_OSD_BOTTOM-1].mutex);
    	}
    }
    else
    {
        return 0;
    }
    
	return 1;
}

int32_t app_mg_cas312_rolling_message_exec(void)
{
	int32_t top_times = 0;
	int32_t bottom_times = 0;
	/*wangjian add on 20141218*/
	char* curFocusWin = (char *)GUI_GetFocusWindow();
	
	if ((NULL != curFocusWin) && (strcasecmp(curFocusWin, "win_full_screen") != 0))
	{
		return TRUE;
	}

    GxCore_MutexLock(s_caRollingMsg[MG312_CA_OSD_TOP-1].mutex);
	if (TRUE == s_caRollingMsg[MG312_CA_OSD_TOP-1].wStatus)
	{
		/* 显示在屏幕上方 */
		if (FALSE == s_caRollingMsg[MG312_CA_OSD_TOP-1].roll_status)
		{
            mg_cas312_rolling_message_show(MG312_CA_OSD_TOP, DVB_CA_ROLLING_FLAG_RESET,\
				   					 (char *)s_caRollingMsg[MG312_CA_OSD_TOP-1].content);
			mg_cas312_rolling_message_show(MG312_CA_OSD_TOP, DVB_CA_ROLLING_FLAG_SHOW,\
				   					(char *)s_caRollingMsg[MG312_CA_OSD_TOP-1].content);
			s_caRollingMsg[MG312_CA_OSD_TOP-1].roll_status = TRUE;				
		}

		top_times = mg_cas312_rolling_message_get_times(MG312_CA_OSD_TOP); 
        //printf("top osd->%d,%d\n", top_times, s_caRollingMsg[MG312_CA_OSD_TOP-1].wTimes);
		if (top_times >= s_caRollingMsg[MG312_CA_OSD_TOP-1].wTimes)
		{//超过滚动次数，停止滚动
			s_caRollingMsg[MG312_CA_OSD_TOP-1].wStatus = FALSE;
			s_caRollingMsg[MG312_CA_OSD_TOP-1].roll_status = FALSE;	
			mg_cas312_rolling_message_show(MG312_CA_OSD_TOP, DVB_CA_ROLLING_FLAG_HIDE, NULL);
			s_caRollingMsg[MG312_CA_OSD_TOP-1].wTimes = 0;
            printf("@@@top osd->stop###\n");
		}
	}
	else
	{
		if (TRUE == s_caRollingMsg[MG312_CA_OSD_TOP-1].roll_status)
		{
			mg_cas312_rolling_message_show(MG312_CA_OSD_TOP, DVB_CA_ROLLING_FLAG_HIDE, NULL);
			s_caRollingMsg[MG312_CA_OSD_TOP-1].roll_status = FALSE;
			s_caRollingMsg[MG312_CA_OSD_TOP-1].wTimes = 0;
		}
	}
    GxCore_MutexUnlock(s_caRollingMsg[MG312_CA_OSD_TOP-1].mutex);

    GxCore_MutexLock(s_caRollingMsg[MG312_CA_OSD_BOTTOM-1].mutex);
	if (TRUE == s_caRollingMsg[MG312_CA_OSD_BOTTOM-1].wStatus)
	{
		/* 显示在屏幕下方 */
		if (FALSE == s_caRollingMsg[MG312_CA_OSD_BOTTOM-1].roll_status)
		{		
            mg_cas312_rolling_message_show(MG312_CA_OSD_BOTTOM, DVB_CA_ROLLING_FLAG_RESET,\
				   					 (char *)s_caRollingMsg[MG312_CA_OSD_BOTTOM-1].content);
			mg_cas312_rolling_message_show(MG312_CA_OSD_BOTTOM, DVB_CA_ROLLING_FLAG_SHOW,\
				   					(char *)s_caRollingMsg[MG312_CA_OSD_BOTTOM-1].content);
			s_caRollingMsg[MG312_CA_OSD_BOTTOM-1].roll_status = TRUE;				
		}

		bottom_times = mg_cas312_rolling_message_get_times(MG312_CA_OSD_BOTTOM);
        //printf("bottom osd->%d,%d\n", bottom_times, s_caRollingMsg[MG312_CA_OSD_BOTTOM-1].wTimes);
		if (bottom_times >= s_caRollingMsg[MG312_CA_OSD_BOTTOM-1].wTimes)
		{
			s_caRollingMsg[MG312_CA_OSD_BOTTOM-1].wStatus = FALSE;
			s_caRollingMsg[MG312_CA_OSD_BOTTOM-1].roll_status = FALSE; 
			mg_cas312_rolling_message_show(MG312_CA_OSD_BOTTOM, DVB_CA_ROLLING_FLAG_HIDE, NULL);
			s_caRollingMsg[MG312_CA_OSD_BOTTOM-1].wTimes = 0;
            printf("###bottom osd->stop@@@\n");
		}
	}
	else
	{
		if (TRUE == s_caRollingMsg[MG312_CA_OSD_BOTTOM-1].roll_status)
		{
			mg_cas312_rolling_message_show(MG312_CA_OSD_BOTTOM, DVB_CA_ROLLING_FLAG_HIDE, NULL);
			s_caRollingMsg[MG312_CA_OSD_BOTTOM-1].roll_status = FALSE;
			s_caRollingMsg[MG312_CA_OSD_BOTTOM-1].wTimes = 0;
		}			
	}
    GxCore_MutexUnlock(s_caRollingMsg[MG312_CA_OSD_BOTTOM-1].mutex);
    
	return TRUE;
}

/*wangjian add on 20141218*/
void app_mg_cas312_rolling_message_hide(void)
{
	printf("[app_mg_cas312_rolling_message_hide]----------\n");
	GxCore_MutexLock(s_caRollingMsg[MG312_CA_OSD_TOP-1].mutex);
	if (TRUE == s_caRollingMsg[MG312_CA_OSD_TOP-1].roll_status)
	{
		mg_cas312_rolling_message_show(MG312_CA_OSD_TOP, DVB_CA_ROLLING_FLAG_RESET,\
				   				(char *)s_caRollingMsg[MG312_CA_OSD_TOP-1].content);
		s_caRollingMsg[MG312_CA_OSD_TOP-1].roll_status = FALSE;
	}
	GxCore_MutexUnlock(s_caRollingMsg[MG312_CA_OSD_TOP-1].mutex);

	GxCore_MutexLock(s_caRollingMsg[MG312_CA_OSD_BOTTOM-1].mutex);
	if (TRUE == s_caRollingMsg[MG312_CA_OSD_BOTTOM-1].roll_status)
	{
		mg_cas312_rolling_message_show(MG312_CA_OSD_BOTTOM, DVB_CA_ROLLING_FLAG_RESET,\
				   				(char *)s_caRollingMsg[MG312_CA_OSD_BOTTOM-1].content);
		s_caRollingMsg[MG312_CA_OSD_BOTTOM-1].roll_status = FALSE;
	}
	GxCore_MutexUnlock(s_caRollingMsg[MG312_CA_OSD_BOTTOM-1].mutex);

	return;
}



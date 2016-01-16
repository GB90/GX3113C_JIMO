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
#include "mgdef.h"
#include "app_common_porting_stb_api.h"
#include "app_mg_cas_api_pop.h"
#include "app_mg_cas_api_rolling_message.h"
extern int32_t app_mg_cas_msg_show(GxMsgProperty0_OnEvent* event);
extern int32_t mg_cas_rolling_message_get_times(uint8_t byStyle);
extern 	void mg_cas_rolling_message_show(uint8_t byStyle,uint8_t showFlag,char* message);
extern int32_t mg_cas_rolling_message_get_times(uint8_t byStyle);
extern char* GUI_GetFocusWindow(void);

static mg_cas_rolling_message_st ca_rolling_msg[MG_CA_OSD_BOTTOM];
mg_cas_rolling_message_st  rollingmsg[MG_MSG_MAX];

#define rolling_message_cpy(pdst,psrc) \
        (pdst)->wStatus = (psrc)->wStatus; \
        (pdst)->roll_status = (psrc)->roll_status; \
        (pdst)->byStyle = (psrc)->byStyle; \
        (pdst)->message_type = (psrc)->message_type; \
        (pdst)->wTimes = (psrc)->wTimes; \
        memcpy((pdst)->content,(psrc)->content,MG_CA_MAX_LEN_OSD);


int8_t app_mg_cas_rolling_message_init(void)
{
    int i;
	memset(&ca_rolling_msg[0],0,MG_CA_OSD_BOTTOM*sizeof(mg_cas_rolling_message_st));
    for(i=0;i<MG_CA_OSD_BOTTOM;i++)
    {
        if(GXCORE_SUCCESS != GxCore_MutexCreate(&ca_rolling_msg[i].mutex))
        {
            printf("GxCore_MutexCreate failed.\n");
        }
    }
    
    memset(&rollingmsg[0],0,MG_MSG_MAX*sizeof(mg_cas_rolling_message_st));
    for(i=0;i<MG_MSG_MAX;i++)
    {
        if(GXCORE_SUCCESS != GxCore_MutexCreate(&rollingmsg[i].mutex))
        {
            printf("GxCore_MutexCreate failed.\n");
        }
    }
    return 0;
}

int32_t app_mg_cas_gxmsg_ca_on_event_msg(GxMsgProperty0_OnEvent* event)
{
	mg_cas_rolling_message_st* rolling_msg = NULL;
	if (NULL == event)
		return 0;

	rolling_msg = (mg_cas_rolling_message_st*)(event->buf);

	if (NULL == rolling_msg)
		return 0;
    
    GxCore_MutexLock (rolling_msg->mutex);
	if (MG_MSG_OSD == rolling_msg->message_type)
	{
		app_mg_cas_gxmsg_ca_on_event_rolling(event);
	}
	else if (MG_MSG_MSG == rolling_msg->message_type)
	{
		app_mg_cas_msg_show(event);
	}
	GxCore_MutexUnlock (rolling_msg->mutex);
    
	return 1;
}


int32_t app_mg_cas_gxmsg_ca_on_event_rolling(GxMsgProperty0_OnEvent* event)
{
	mg_cas_rolling_message_st* rolling_msg = NULL;
	if (NULL == event)
		return 0;

	rolling_msg = (mg_cas_rolling_message_st*)(event->buf);

	if (NULL == rolling_msg)
		return 0;

    if(rolling_msg->message_type == MG_MSG_OSD)
    {
    	if (MG_CA_OSD_TOP == rolling_msg->byStyle)	 
    	{//复位滚动显示，实际还未显示
    	    GxCore_MutexLock (ca_rolling_msg[MG_CA_OSD_TOP-1].mutex);
            rolling_message_cpy(&ca_rolling_msg[MG_CA_OSD_TOP-1],rolling_msg);
    		ca_rolling_msg[MG_CA_OSD_TOP-1].roll_status=FALSE;
    		ca_rolling_msg[MG_CA_OSD_TOP-1].wStatus = TRUE;
            //mg_cas_rolling_message_show(MG_CA_OSD_TOP,DVB_CA_ROLLING_FLAG_RESET,(char*)ca_rolling_msg[MG_CA_OSD_TOP-1].content);
            printf("##MG_CA_OSD_TOP times:%d\n",rolling_msg->wTimes);
            GxCore_MutexUnlock (ca_rolling_msg[MG_CA_OSD_TOP-1].mutex);
    	}
        
    	if (MG_CA_OSD_BOTTOM == rolling_msg->byStyle)	 
    	{
            GxCore_MutexLock (ca_rolling_msg[MG_CA_OSD_BOTTOM-1].mutex);
            rolling_message_cpy(&ca_rolling_msg[MG_CA_OSD_BOTTOM-1],rolling_msg);
            ca_rolling_msg[MG_CA_OSD_BOTTOM-1].roll_status=FALSE;
    		ca_rolling_msg[MG_CA_OSD_BOTTOM-1].wStatus=TRUE;
            //mg_cas_rolling_message_show(MG_CA_OSD_BOTTOM,DVB_CA_ROLLING_FLAG_RESET,(char*)ca_rolling_msg[MG_CA_OSD_BOTTOM-1].content);
            printf("##MG_CA_OSD_BOTTOM times:%d\n",rolling_msg->wTimes);
            GxCore_MutexUnlock (ca_rolling_msg[MG_CA_OSD_BOTTOM-1].mutex);
    	}
        
    }
    else
    {
        return 0;
    }
    
	return 1;
}

int32_t app_mg_cas_rolling_message_exec(void)
{
	int32_t top_times = 0;
	int32_t bottom_times = 0;

	/*wangjian add on 20141224*/
	char* curFocusWin = (char *)GUI_GetFocusWindow();
	
	if ((NULL != curFocusWin) && (strcasecmp(curFocusWin, "win_full_screen") != 0))
	{
		return TRUE;
	}

    GxCore_MutexLock (ca_rolling_msg[MG_CA_OSD_TOP-1].mutex);
	if (TRUE == ca_rolling_msg[MG_CA_OSD_TOP-1].wStatus)
	{
		/* CDCA_OSD_TOP: OSD风格：显示在屏幕上方 */
		if (FALSE == ca_rolling_msg[MG_CA_OSD_TOP-1].roll_status)
		{
            mg_cas_rolling_message_show(MG_CA_OSD_TOP,DVB_CA_ROLLING_FLAG_RESET,(char*)ca_rolling_msg[MG_CA_OSD_TOP-1].content);
			mg_cas_rolling_message_show(MG_CA_OSD_TOP,DVB_CA_ROLLING_FLAG_SHOW,(char*)ca_rolling_msg[MG_CA_OSD_TOP-1].content);
			ca_rolling_msg[MG_CA_OSD_TOP-1].roll_status = TRUE;				
		}

		top_times = mg_cas_rolling_message_get_times(MG_CA_OSD_TOP); 
        //printf("top osd->%d,%d\n",top_times,ca_rolling_msg[MG_CA_OSD_TOP-1].wTimes);
		if ( top_times >= ca_rolling_msg[MG_CA_OSD_TOP-1].wTimes )
		{//超过滚动次数，停止滚动
			ca_rolling_msg[MG_CA_OSD_TOP-1].wStatus = FALSE;
			ca_rolling_msg[MG_CA_OSD_TOP-1].roll_status= FALSE;	
			mg_cas_rolling_message_show(MG_CA_OSD_TOP,DVB_CA_ROLLING_FLAG_HIDE,NULL);
			ca_rolling_msg[MG_CA_OSD_TOP-1].wTimes= 0;
            printf("top osd->stop\n");
		}
	}
	else
	{
		if (TRUE == ca_rolling_msg[MG_CA_OSD_TOP-1].roll_status)
		{
			mg_cas_rolling_message_show(MG_CA_OSD_TOP,DVB_CA_ROLLING_FLAG_HIDE,NULL);
			ca_rolling_msg[MG_CA_OSD_TOP-1].roll_status = FALSE;
			ca_rolling_msg[MG_CA_OSD_TOP-1].wTimes= 0;
		}
	}
    GxCore_MutexUnlock (ca_rolling_msg[MG_CA_OSD_TOP-1].mutex);

    GxCore_MutexLock (ca_rolling_msg[MG_CA_OSD_BOTTOM-1].mutex);
	if (TRUE == ca_rolling_msg[MG_CA_OSD_BOTTOM-1].wStatus)
	{
		/* CDCA_OSD_BOTTOM: OSD风格：显示在屏幕下方 */

		if (FALSE == ca_rolling_msg[MG_CA_OSD_BOTTOM-1].roll_status)
		{		
            mg_cas_rolling_message_show(MG_CA_OSD_BOTTOM,DVB_CA_ROLLING_FLAG_RESET,(char*)ca_rolling_msg[MG_CA_OSD_BOTTOM-1].content);
			mg_cas_rolling_message_show(MG_CA_OSD_BOTTOM,DVB_CA_ROLLING_FLAG_SHOW,(char*)ca_rolling_msg[MG_CA_OSD_BOTTOM-1].content);
			ca_rolling_msg[MG_CA_OSD_BOTTOM-1].roll_status = TRUE;				
		}

		bottom_times = mg_cas_rolling_message_get_times(MG_CA_OSD_BOTTOM);
        //printf("bottom osd->%d,%d\n",bottom_times,ca_rolling_msg[MG_CA_OSD_BOTTOM-1].wTimes);
		if ( bottom_times >= ca_rolling_msg[MG_CA_OSD_BOTTOM-1].wTimes)
		{
			ca_rolling_msg[MG_CA_OSD_BOTTOM-1].wStatus = FALSE;
			ca_rolling_msg[MG_CA_OSD_BOTTOM-1].roll_status= FALSE; 
			mg_cas_rolling_message_show(MG_CA_OSD_BOTTOM,DVB_CA_ROLLING_FLAG_HIDE,NULL);
			ca_rolling_msg[MG_CA_OSD_BOTTOM-1].wTimes= 0;
            printf("bottom osd->stop\n");
		}

	}
	else
	{
		if (TRUE == ca_rolling_msg[MG_CA_OSD_BOTTOM-1].roll_status)
		{
			mg_cas_rolling_message_show(MG_CA_OSD_BOTTOM,DVB_CA_ROLLING_FLAG_HIDE,NULL);
			ca_rolling_msg[MG_CA_OSD_BOTTOM-1].roll_status= FALSE;
			ca_rolling_msg[MG_CA_OSD_BOTTOM-1].wTimes= 0;
		}			
	}
    GxCore_MutexUnlock (ca_rolling_msg[MG_CA_OSD_BOTTOM-1].mutex);
    
	return TRUE;

}

/*wangjian add on 20141223*/
void app_mg_cas_rolling_message_hide(void)
{
	printf("[app_mg_cas_rolling_message_hide]----------\n");
	GxCore_MutexLock(ca_rolling_msg[MG_CA_OSD_TOP-1].mutex);
	if (TRUE == ca_rolling_msg[MG_CA_OSD_TOP-1].roll_status)
	{
		mg_cas_rolling_message_show(MG_CA_OSD_TOP, DVB_CA_ROLLING_FLAG_RESET,\
				   				(char *)ca_rolling_msg[MG_CA_OSD_TOP-1].content);
		ca_rolling_msg[MG_CA_OSD_TOP-1].roll_status = FALSE;
	}
	GxCore_MutexUnlock(ca_rolling_msg[MG_CA_OSD_TOP-1].mutex);

	GxCore_MutexLock(ca_rolling_msg[MG_CA_OSD_BOTTOM-1].mutex);
	if (TRUE == ca_rolling_msg[MG_CA_OSD_BOTTOM-1].roll_status)
	{
		mg_cas_rolling_message_show(MG_CA_OSD_BOTTOM, DVB_CA_ROLLING_FLAG_RESET,\
				   				(char *)ca_rolling_msg[MG_CA_OSD_BOTTOM-1].content);
		ca_rolling_msg[MG_CA_OSD_BOTTOM-1].roll_status = FALSE;
	}
	GxCore_MutexUnlock(ca_rolling_msg[MG_CA_OSD_BOTTOM-1].mutex);

	return;
}



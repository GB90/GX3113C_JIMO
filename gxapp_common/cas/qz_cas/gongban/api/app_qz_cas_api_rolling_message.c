/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_qz_cas_api_rolling_message.c
* Author    : 	chenth
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.05.6		  chenth 	 			creation
*****************************************************************************/
#include "app_common_porting_stb_api.h"
#include "app_qz_cas_api_pop.h"
#include "app_qz_cas_api_rolling_message.h"

extern int32_t qz_cas_rolling_message_get_times(uint8_t byStyle);
extern 	void qz_cas_rolling_message_show(uint8_t byStyle,uint8_t showFlag,char* message);
static qz_cas_rolling_message_st ca_rolling_msg[QZ_CA_OSD_NUM];
qz_cas_rolling_message_st rollingmsg[QZ_MSG_MAX];


int8_t app_qz_cas_rolling_message_init(void)
{
    int i;
	memset(&ca_rolling_msg[0],0,QZ_CA_OSD_NUM*sizeof(qz_cas_rolling_message_st));

    for(i=0;i<QZ_CA_OSD_NUM;i++)
    {
        if(GXCORE_SUCCESS != GxCore_MutexCreate(&ca_rolling_msg[i].mutex))
        {
            printf("GxCore_MutexCreate failed.\n");
        }
    }
    
    memset(&rollingmsg[0],0,QZ_MSG_MAX*sizeof(qz_cas_rolling_message_st));
    for(i=0;i<QZ_MSG_MAX;i++)
    {
        if(GXCORE_SUCCESS != GxCore_MutexCreate(&rollingmsg[i].mutex))
        {
            printf("GxCore_MutexCreate failed.\n");
        }
    }
	return 0;
}

int32_t app_qz_cas_gxmsg_ca_on_event_msg(GxMsgProperty0_OnEvent* event)
{
	qz_cas_rolling_message_st* rolling_msg = NULL;
	if (NULL == event)
		return 0;
	rolling_msg = (qz_cas_rolling_message_st*)(event->buf);
	if (NULL == rolling_msg)
		return 0;
    printf("XXXXXXXXXX\n");
    GxCore_MutexLock (rolling_msg->mutex);
	if (QZ_MSG_OSD == rolling_msg->message_type)
	{
		app_qz_cas_gxmsg_ca_on_event_rolling(event);
	}
	GxCore_MutexUnlock (rolling_msg->mutex);	
	printf("XXXXXXXXXXXXXXXXXXXX\n");
	return 1;
}


int32_t app_qz_cas_gxmsg_ca_on_event_rolling(GxMsgProperty0_OnEvent* event)
{
	qz_cas_rolling_message_st* rolling_msg = NULL;
	if (NULL == event)
		return 0;

	rolling_msg = (qz_cas_rolling_message_st*)(event->buf);

	if (NULL == rolling_msg)
		return 0;

    if(rolling_msg->message_type == QZ_MSG_OSD)
    {
    	if (QZ_CA_OSD_TOP == rolling_msg->byStyle)	 
    	{//复位滚动显示，实际还未显示
    	    GxCore_MutexLock (ca_rolling_msg[QZ_CA_OSD_TOP-1].mutex);
            rolling_message_cpy(&ca_rolling_msg[QZ_CA_OSD_TOP-1],rolling_msg);
            ca_rolling_msg[QZ_CA_OSD_TOP-1].roll_status = FALSE;
    		ca_rolling_msg[QZ_CA_OSD_TOP-1].wStatus = TRUE;
            GxCore_MutexUnlock (ca_rolling_msg[QZ_CA_OSD_TOP-1].mutex);	
    	}
    }
     
	return 1;
}

int32_t app_qz_cas_rolling_message_exec(void)
{
	int32_t top_times = 0;
    ROLLING_STATE_E step = ROLLING_STATE_INIT;
    uint8_t contentBuf[QZ_CA_MAX_LEN_OSD];
	char* curFocusWin = (char *)GUI_GetFocusWindow();
	
	if ((NULL != curFocusWin) && (strcasecmp(curFocusWin, "win_full_screen") != 0))
	{
		return TRUE;
	}

    GxCore_MutexLock (ca_rolling_msg[QZ_CA_OSD_TOP-1].mutex);
	if (TRUE == ca_rolling_msg[QZ_CA_OSD_TOP-1].wStatus)
	{
		/* CDCA_OSD_TOP: OSD风格：显示在屏幕上方 */
		if (FALSE == ca_rolling_msg[QZ_CA_OSD_TOP-1].roll_status)
		{
			ca_rolling_msg[QZ_CA_OSD_TOP-1].roll_status = TRUE;	
            memcpy(contentBuf,ca_rolling_msg[QZ_CA_OSD_TOP-1].content,QZ_CA_MAX_LEN_OSD);
            step = ROLLING_STATE_SHOWSTART;
            printf("top osd->start\n");
		}

		top_times = qz_cas_rolling_message_get_times(QZ_CA_OSD_TOP); 
		if ( top_times >= ca_rolling_msg[QZ_CA_OSD_TOP-1].wTimes )
		{//超过滚动次数，停止滚动
			ca_rolling_msg[QZ_CA_OSD_TOP-1].wStatus = FALSE;
			ca_rolling_msg[QZ_CA_OSD_TOP-1].roll_status= FALSE;	
            ca_rolling_msg[QZ_CA_OSD_TOP-1].wTimes= 0;
			step = ROLLING_STATE_HIDESTART;
            printf("top osd->stop\n");
		}
	}
	else
	{
		if (TRUE == ca_rolling_msg[QZ_CA_OSD_TOP-1].roll_status)
		{
			ca_rolling_msg[QZ_CA_OSD_TOP-1].roll_status = FALSE;
			ca_rolling_msg[QZ_CA_OSD_TOP-1].wTimes= 0;
            step = ROLLING_STATE_HIDESTART;
		}
	}
    GxCore_MutexUnlock (ca_rolling_msg[QZ_CA_OSD_TOP-1].mutex);	

    if(ROLLING_STATE_SHOWSTART == step)
    {
        qz_cas_rolling_message_show(QZ_CA_OSD_TOP,DVB_CA_ROLLING_FLAG_RESET,(char*)contentBuf);
        qz_cas_rolling_message_show(QZ_CA_OSD_TOP,DVB_CA_ROLLING_FLAG_SHOW,(char*)contentBuf);
    }
    else if(ROLLING_STATE_HIDESTART == step)
    {
        qz_cas_rolling_message_show(QZ_CA_OSD_TOP,DVB_CA_ROLLING_FLAG_HIDE,NULL);
    }
    else
    {
        //printf("nothing todo\n");
    }
    
	return TRUE;

}
void app_qz_cas_rolling_message_hide(void)
{
	printf("[app_qz_cas_rolling_message_hide]----------\n");
	GxCore_MutexLock(ca_rolling_msg[QZ_CA_OSD_TOP-1].mutex);
	if (TRUE == ca_rolling_msg[QZ_CA_OSD_TOP-1].roll_status)
	{
		qz_cas_rolling_message_show(QZ_CA_OSD_TOP, DVB_CA_ROLLING_FLAG_RESET,\
				   				(char *)ca_rolling_msg[QZ_CA_OSD_TOP-1].content);
		ca_rolling_msg[QZ_CA_OSD_TOP-1].roll_status = FALSE;
	}
	GxCore_MutexUnlock(ca_rolling_msg[QZ_CA_OSD_TOP-1].mutex);
	return;
}




#include "app_common_porting_stb_api.h"
#include "app_xinshimao_cas_api_rolling_message.h"

extern void win_xinshimao_cas_entitle_showDialog(void *data,int size);
extern int32_t xinshimao_cas_rolling_message_get_times(uint8_t byStyle);
extern 	void xinshimao_cas_rolling_message_show(uint8_t byStyle,uint8_t showFlag,char* message);
static xinshimao_cas_rolling_message_st ca_rolling_msg[CA_OSD_TOP];
xinshimao_cas_rolling_message_st  rollingmsg[XINSHIMAO_MSG_MAX];

#define rolling_message_cpy(pdst,psrc) \
        (pdst)->wStatus = (psrc)->wStatus; \
        (pdst)->roll_status = (psrc)->roll_status; \
        (pdst)->byStyle = (psrc)->byStyle; \
        (pdst)->message_type = (psrc)->message_type; \
        (pdst)->wTimes = (psrc)->wTimes; \
        memcpy((pdst)->content,(psrc)->content,CA_MAX_LEN_OSD);


int8_t app_xinshimao_cas_rolling_message_init(void)
{
    int i;
	memset(ca_rolling_msg,0,CA_OSD_TOP*sizeof(xinshimao_cas_rolling_message_st));
    for(i=0;i<CA_OSD_TOP;i++)
    {
        if(GXCORE_SUCCESS != GxCore_MutexCreate(&ca_rolling_msg[i].mutex))
        {
            printf("GxCore_MutexCreate failed.\n");
        }
    }
    
    memset(rollingmsg,0,XINSHIMAO_MSG_MAX*sizeof(xinshimao_cas_rolling_message_st));
    for(i=0;i<XINSHIMAO_MSG_MAX;i++)
    {
        if(GXCORE_SUCCESS != GxCore_MutexCreate(&rollingmsg[i].mutex))
        {
            printf("GxCore_MutexCreate failed.\n");
        }
    }
    return 0;
}

void xinshimao_cas_sent_osdmsg(uint8_t style,char *msg)
{
    GxCore_MutexLock (rollingmsg[XINSHIMAO_MSG_OSD].mutex);
    rollingmsg[XINSHIMAO_MSG_OSD].wStatus= TRUE;
    rollingmsg[XINSHIMAO_MSG_OSD].roll_status= FALSE;
    rollingmsg[XINSHIMAO_MSG_OSD].byStyle= style;
    rollingmsg[XINSHIMAO_MSG_OSD].wTimes= 3;
    strcpy((char *)rollingmsg[XINSHIMAO_MSG_OSD].content,msg);
    GxCore_MutexUnlock (rollingmsg[XINSHIMAO_MSG_OSD].mutex);
    app_cas_api_on_event(DVB_CA_ROLLING_INFO,CAS_NAME, &rollingmsg[XINSHIMAO_MSG_OSD], sizeof(xinshimao_cas_rolling_message_st));	
}

void xinshimao_cas_sent_msg(char *msg)
{
    GxCore_MutexLock (rollingmsg[XINSHIMAO_MSG_MSG].mutex);
    rollingmsg[XINSHIMAO_MSG_MSG].message_type = XINSHIMAO_MSG_MSG;
    strcpy((char *)rollingmsg[XINSHIMAO_MSG_MSG].content,msg);
    GxCore_MutexUnlock (rollingmsg[XINSHIMAO_MSG_MSG].mutex);
    app_cas_api_on_event(DVB_CA_ROLLING_INFO,CAS_NAME, &rollingmsg[XINSHIMAO_MSG_MSG], sizeof(xinshimao_cas_rolling_message_st));	
}


int32_t app_xinshimao_cas_msg_show(GxMsgProperty0_OnEvent* event)
{
    xinshimao_cas_rolling_message_st *data = NULL;
	if (NULL == event)
		return 0;

	data = (xinshimao_cas_rolling_message_st *)(event->buf);

	if (NULL == data)
		return 0;

    win_xinshimao_cas_entitle_showDialog((void *)data,sizeof(xinshimao_cas_rolling_message_st));

    return 1;
}

int32_t app_xinshimao_cas_gxmsg_ca_on_event_msg(GxMsgProperty0_OnEvent* event)
{
	xinshimao_cas_rolling_message_st* rolling_msg = NULL;
	if (NULL == event)
		return 0;

	rolling_msg = (xinshimao_cas_rolling_message_st *)(event->buf);

	if (NULL == rolling_msg)
		return 0;
    
    GxCore_MutexLock (rolling_msg->mutex);
	if (XINSHIMAO_MSG_OSD == rolling_msg->message_type)
	{
		app_xinshimao_cas_gxmsg_ca_on_event_rolling(event);
	}
	else if (XINSHIMAO_MSG_MSG == rolling_msg->message_type)
	{
		app_xinshimao_cas_msg_show(event);
	}
	GxCore_MutexUnlock (rolling_msg->mutex);
    
	return 1;
}


int32_t app_xinshimao_cas_gxmsg_ca_on_event_rolling(GxMsgProperty0_OnEvent* event)
{
	xinshimao_cas_rolling_message_st* rolling_msg = NULL;
	if (NULL == event)
		return 0;

	rolling_msg = (xinshimao_cas_rolling_message_st *)(event->buf);

	if (NULL == rolling_msg)
		return 0;

    if(rolling_msg->message_type == XINSHIMAO_MSG_OSD)
    {
    	if (CA_OSD_TOP == rolling_msg->byStyle)	 
    	{//复位滚动显示，实际还未显示
    	    GxCore_MutexLock (ca_rolling_msg[CA_OSD_TOP-1].mutex);
            rolling_message_cpy(&ca_rolling_msg[CA_OSD_TOP-1],rolling_msg);
    		ca_rolling_msg[CA_OSD_TOP-1].roll_status=FALSE;
    		ca_rolling_msg[CA_OSD_TOP-1].wStatus = TRUE;
            printf("##XINSHIMAO_CA_OSD_TOP times:%d\n",rolling_msg->wTimes);
            GxCore_MutexUnlock (ca_rolling_msg[CA_OSD_TOP-1].mutex);
    	}
    }
    else
    {
        return 0;
    }
    
	return 1;
}

int32_t app_xinshimao_cas_rolling_message_exec(void)
{
	int32_t top_times = 0;

    GxCore_MutexLock (ca_rolling_msg[CA_OSD_TOP-1].mutex);
	if (TRUE == ca_rolling_msg[CA_OSD_TOP-1].wStatus)
	{
		/* CDCA_OSD_TOP: OSD风格：显示在屏幕上方 */
		if (FALSE == ca_rolling_msg[CA_OSD_TOP-1].roll_status)
		{
            xinshimao_cas_rolling_message_show(CA_OSD_TOP,DVB_CA_ROLLING_FLAG_RESET,(char*)ca_rolling_msg[CA_OSD_TOP-1].content);
			xinshimao_cas_rolling_message_show(CA_OSD_TOP,DVB_CA_ROLLING_FLAG_SHOW,(char*)ca_rolling_msg[CA_OSD_TOP-1].content);
			ca_rolling_msg[CA_OSD_TOP-1].roll_status = TRUE;				
		}

		top_times = xinshimao_cas_rolling_message_get_times(CA_OSD_TOP); 
		if ( top_times >= ca_rolling_msg[CA_OSD_TOP-1].wTimes )
		{//超过滚动次数，停止滚动
			ca_rolling_msg[CA_OSD_TOP-1].wStatus = FALSE;
			ca_rolling_msg[CA_OSD_TOP-1].roll_status= FALSE;	
			xinshimao_cas_rolling_message_show(CA_OSD_TOP,DVB_CA_ROLLING_FLAG_HIDE,NULL);
			ca_rolling_msg[CA_OSD_TOP-1].wTimes= 0;
            printf("top osd->stop\n");
		}
	}
	else
	{
		if (TRUE == ca_rolling_msg[CA_OSD_TOP-1].roll_status)
		{
			xinshimao_cas_rolling_message_show(CA_OSD_TOP,DVB_CA_ROLLING_FLAG_HIDE,NULL);
			ca_rolling_msg[CA_OSD_TOP-1].roll_status = FALSE;
			ca_rolling_msg[CA_OSD_TOP-1].wTimes= 0;
		}
	}
    GxCore_MutexUnlock (ca_rolling_msg[CA_OSD_TOP-1].mutex);
    
	return TRUE;

}



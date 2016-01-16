/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_mg_cas_api_rolling_message.c
* Author    : 	chenth
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.12.19		  chenth 	 			creation
*****************************************************************************/
#include "app_common_porting_stb_api.h"
#include "app_kp_cas_api_rolling_message.h"

extern char * GUI_GetFocusWindow(void);
extern int32_t kp_cas_rolling_message_get_times(uint8_t byStyle);
extern void kp_cas_rolling_message_show(uint8_t byStyle,uint8_t showFlag,char* message);

static kp_cas_rolling_message_st *prolling_msg_tmp;
static kp_cas_rolling_message_st *pca_rolling_msg[CA_OSD_BOTTOM];


#define rolling_message_cpy(pdst,psrc) \
        (pdst)->wStatus = (psrc)->wStatus; \
        (pdst)->roll_status = (psrc)->roll_status; \
        (pdst)->byStyle = (psrc)->byStyle; \
        (pdst)->message_type = (psrc)->message_type; \
        (pdst)->wTimes = (psrc)->wTimes; \
        memcpy((pdst)->content,(psrc)->content,CA_MAX_LEN_OSD);

int Osd_Analyze(char *text,int len,uint8_t *style,uint8_t *times_min,uint8_t *time_num,char *cMsg)
{
    int idx  = 0,clen = 0;
    if(!text || len<=0 || !style || !times_min || !time_num || !cMsg)
    {
        return -1;
    }

    idx = 1;
    
    if(text[idx ++] == 'P') 
        *style = text[idx ++];
    if(text[idx] == 'D') //滚动时间
        *times_min = text[idx +1];
    else if(text[idx] == 'N') //滚动次数
        *time_num  = text[idx +1];
        
    idx   += 2;
        
    if(text[idx ++] == 'C')
    {
        clen = text[idx ++];
        if(clen > 0) 
        {
            strncpy(cMsg, (char *)&text[idx], clen);
        }
        idx  += clen;
    }
    
    if(idx != len) 
    {
        return -1;
    }

    return 1;

}



void kp_cas_sent_osdmsg(uint8_t style,uint8_t times_min,uint8_t time_num,char *msg)
{
    GxCore_MutexLock (prolling_msg_tmp->mutex);
    prolling_msg_tmp->wStatus= TRUE;
    prolling_msg_tmp->roll_status= FALSE;
    prolling_msg_tmp->byStyle= style;
    prolling_msg_tmp->wTimes= (times_min<<8)|time_num;
    strcpy((char *)prolling_msg_tmp->content,msg);
    printf("----------osd msg:%s\n",(char *)prolling_msg_tmp->content);
    GxCore_MutexUnlock (prolling_msg_tmp->mutex);
    app_cas_api_on_event(DVB_CA_ROLLING_INFO,CAS_NAME, prolling_msg_tmp, sizeof(kp_cas_rolling_message_st));	
}


void kp_cas_rolling_test()
{
    char* strtemp = "KP CAS短消息测试";
    kp_cas_sent_osdmsg(1,1,0,strtemp);// 1min
    kp_cas_sent_osdmsg(2,0,3,strtemp);// 3次
}


int8_t app_kp_cas_rolling_message_init(void)
{
    int i;
    prolling_msg_tmp = (kp_cas_rolling_message_st *)GxCore_Malloc(sizeof(kp_cas_rolling_message_st));
    memset(prolling_msg_tmp,0,sizeof(kp_cas_rolling_message_st));
    if(GXCORE_SUCCESS != GxCore_MutexCreate(&prolling_msg_tmp->mutex))
    {
        printf("GxCore_MutexCreate failed.\n");
    }

    for(i=0;i<CA_OSD_BOTTOM;i++)
    {
        pca_rolling_msg[i] = (kp_cas_rolling_message_st *)GxCore_Malloc(sizeof(kp_cas_rolling_message_st));
        memset(pca_rolling_msg[i],0,sizeof(kp_cas_rolling_message_st));
        if(GXCORE_SUCCESS != GxCore_MutexCreate(&pca_rolling_msg[i]->mutex))
        {
            printf("GxCore_MutexCreate failed.\n");
        }
    }
    
    return 0;
}

static int32_t app_kp_cas_gxmsg_ca_on_event_rolling(GxMsgProperty0_OnEvent* event)
{
	kp_cas_rolling_message_st* rolling_msg = NULL;
	if (NULL == event)
		return 0;

	rolling_msg = (kp_cas_rolling_message_st *)(event->buf);

	if (NULL == rolling_msg)
		return 0;
    
	if (rolling_msg->byStyle==1)// 1,3
	{//复位滚动显示，实际还未显示
	    GxCore_MutexLock (pca_rolling_msg[CA_OSD_TOP-1]->mutex);
        rolling_message_cpy(pca_rolling_msg[CA_OSD_TOP-1],rolling_msg);
		pca_rolling_msg[CA_OSD_TOP-1]->roll_status=FALSE;
		pca_rolling_msg[CA_OSD_TOP-1]->wStatus = TRUE;
        printf("##KP_CA_OSD_TOP times:%d,%d min.\n",rolling_msg->wTimes&0xff,(rolling_msg->wTimes&0xff00)>8);
        GxCore_MutexUnlock (pca_rolling_msg[CA_OSD_TOP-1]->mutex);
	}
	else if(rolling_msg->byStyle==2)
	{
        GxCore_MutexLock (pca_rolling_msg[CA_OSD_BOTTOM-1]->mutex);
        rolling_message_cpy(pca_rolling_msg[CA_OSD_BOTTOM-1],rolling_msg);
        pca_rolling_msg[CA_OSD_BOTTOM-1]->roll_status = FALSE;
		pca_rolling_msg[CA_OSD_BOTTOM-1]->wStatus = TRUE;
        printf("##KP_CA_OSD_BOTTOM times:%d,%d min.[osd]%s\n",rolling_msg->wTimes&0xff,(rolling_msg->wTimes&0xff00)>8,(char*)pca_rolling_msg[CA_OSD_BOTTOM-1]->content);
        GxCore_MutexUnlock (pca_rolling_msg[CA_OSD_BOTTOM-1]->mutex);
	}
	return 1;
}
int32_t app_kp_cas_gxmsg_ca_on_event_msg(GxMsgProperty0_OnEvent* event)
{
	kp_cas_rolling_message_st* rolling_msg = NULL;
	if (NULL == event)
		return 0;

	rolling_msg = (kp_cas_rolling_message_st*)(event->buf);

	if (NULL == rolling_msg)
		return 0;
    
    GxCore_MutexLock (rolling_msg->mutex);
	app_kp_cas_gxmsg_ca_on_event_rolling(event);
	GxCore_MutexUnlock (rolling_msg->mutex);
    
	return 1;
}
int32_t app_kp_cas_rolling_message_exec(void)
{
	int32_t top_times = 0,top_times_tmp;
	int32_t bottom_times = 0,bottom_times_tmp;
    GxTime localtime;
    int32_t top_time_sec_tmp = 0,bottom_time_sec_tmp;
    static int32_t top_sec_time = 0,bottom_sec_time = 0;

    char* focus_Window = (char*)GUI_GetFocusWindow();
	if ((NULL != focus_Window)&&(strcasecmp("win_full_screen", focus_Window) != 0))
	{
		return 1;
	}


    GxCore_MutexLock (pca_rolling_msg[CA_OSD_TOP-1]->mutex);
    
	if (TRUE == pca_rolling_msg[CA_OSD_TOP-1]->wStatus)
	{
        
		/* CDCA_OSD_TOP: OSD风格：显示在屏幕上方 */
		if (FALSE == pca_rolling_msg[CA_OSD_TOP-1]->roll_status)
		{
            if(((pca_rolling_msg[CA_OSD_TOP-1]->wTimes&0xff00)>>8))
            {
                GxCore_GetLocalTime(&localtime);
                top_sec_time = localtime.seconds;
            }
            kp_cas_rolling_message_show(CA_OSD_TOP,DVB_CA_ROLLING_FLAG_RESET,(char *)pca_rolling_msg[CA_OSD_TOP-1]->content);
			kp_cas_rolling_message_show(CA_OSD_TOP,DVB_CA_ROLLING_FLAG_SHOW,(char *)pca_rolling_msg[CA_OSD_TOP-1]->content);
			pca_rolling_msg[CA_OSD_TOP-1]->roll_status = TRUE;	
		}

        
        if((top_times_tmp = pca_rolling_msg[CA_OSD_TOP-1]->wTimes&0xff)>0)
        {
            //printf("CA_OSD_TOP-------1\n");
    		top_times = kp_cas_rolling_message_get_times(CA_OSD_TOP); 
    		if ( top_times >= top_times_tmp)
    		{//超过滚动次数，停止滚动
    			pca_rolling_msg[CA_OSD_TOP-1]->wStatus = FALSE;
    			pca_rolling_msg[CA_OSD_TOP-1]->roll_status= FALSE;	
    			kp_cas_rolling_message_show(CA_OSD_TOP,DVB_CA_ROLLING_FLAG_HIDE,NULL);
    			pca_rolling_msg[CA_OSD_TOP-1]->wTimes &= ~(0xff);
                printf("top osd->stop\n");
    		}
            //printf("CA_OSD_TOP-------2\n");
        }
        else//按照时间计算
        {
            //printf("CA_OSD_TOP-------11\n");
            top_time_sec_tmp = 60*((pca_rolling_msg[CA_OSD_TOP-1]->wTimes&0xff00)>>8);
            GxCore_GetLocalTime(&localtime);
            if((localtime.seconds-top_sec_time)>top_time_sec_tmp)
            {
                pca_rolling_msg[CA_OSD_TOP-1]->wStatus = FALSE;
    			pca_rolling_msg[CA_OSD_TOP-1]->roll_status= FALSE;	
    			kp_cas_rolling_message_show(CA_OSD_TOP,DVB_CA_ROLLING_FLAG_HIDE,NULL);
    			pca_rolling_msg[CA_OSD_TOP-1]->wTimes &= ~(0xff00);
                printf("top osd->stop\n");
            }
             //printf("CA_OSD_TOP-------22\n");
        }
        
	}
	else
	{
		if (TRUE == pca_rolling_msg[CA_OSD_TOP-1]->roll_status)
		{
			kp_cas_rolling_message_show(CA_OSD_TOP,DVB_CA_ROLLING_FLAG_HIDE,NULL);
			pca_rolling_msg[CA_OSD_TOP-1]->roll_status = FALSE;
			pca_rolling_msg[CA_OSD_TOP-1]->wTimes= 0;
		}
	}
    
    GxCore_MutexUnlock (pca_rolling_msg[CA_OSD_TOP-1]->mutex);

    GxCore_MutexLock (pca_rolling_msg[CA_OSD_BOTTOM-1]->mutex);
	if (TRUE == pca_rolling_msg[CA_OSD_BOTTOM-1]->wStatus)
	{
		/* CDCA_OSD_BOTTOM: OSD风格：显示在屏幕下方 */
        //printf("CA_OSD_BOTTOM-------1\n");
		if (FALSE == pca_rolling_msg[CA_OSD_BOTTOM-1]->roll_status)
		{		
            if(((pca_rolling_msg[CA_OSD_BOTTOM-1]->wTimes&0xff00)>>8))
            {
                GxCore_GetLocalTime(&localtime);
                bottom_sec_time = localtime.seconds;
            }
            
            kp_cas_rolling_message_show(CA_OSD_BOTTOM,DVB_CA_ROLLING_FLAG_RESET,(char*)pca_rolling_msg[CA_OSD_BOTTOM-1]->content);
			kp_cas_rolling_message_show(CA_OSD_BOTTOM,DVB_CA_ROLLING_FLAG_SHOW,(char*)pca_rolling_msg[CA_OSD_BOTTOM-1]->content);
			pca_rolling_msg[CA_OSD_BOTTOM-1]->roll_status = TRUE;	
            
		}
        if((bottom_times_tmp = pca_rolling_msg[CA_OSD_BOTTOM-1]->wTimes&0xff)>0)
        {
    		bottom_times = kp_cas_rolling_message_get_times(CA_OSD_BOTTOM);
    		if ( bottom_times >= bottom_times_tmp)
    		{
    			pca_rolling_msg[CA_OSD_BOTTOM-1]->wStatus = FALSE;
    			pca_rolling_msg[CA_OSD_BOTTOM-1]->roll_status= FALSE; 
    			kp_cas_rolling_message_show(CA_OSD_BOTTOM,DVB_CA_ROLLING_FLAG_HIDE,NULL);
                pca_rolling_msg[CA_OSD_BOTTOM-1]->wTimes &= ~(0xff);
                printf("bottom osd->stop\n");
    		}
        }
        else//按照时间计算
        {
            bottom_time_sec_tmp = 60*((pca_rolling_msg[CA_OSD_BOTTOM-1]->wTimes&0xff00)>>8);
            GxCore_GetLocalTime(&localtime);
            if((localtime.seconds-bottom_sec_time)>bottom_time_sec_tmp)
            {
                pca_rolling_msg[CA_OSD_BOTTOM-1]->wStatus = FALSE;
    			pca_rolling_msg[CA_OSD_BOTTOM-1]->roll_status= FALSE;	
    			kp_cas_rolling_message_show(CA_OSD_BOTTOM,DVB_CA_ROLLING_FLAG_HIDE,NULL);
    			pca_rolling_msg[CA_OSD_BOTTOM-1]->wTimes &= ~(0xff00);
                printf("top osd->stop\n");
            }
        }
        //printf("CA_OSD_BOTTOM-------2\n");
	}
	else
	{
		if (TRUE == pca_rolling_msg[CA_OSD_BOTTOM-1]->roll_status)
		{
			kp_cas_rolling_message_show(CA_OSD_BOTTOM,DVB_CA_ROLLING_FLAG_HIDE,NULL);
			pca_rolling_msg[CA_OSD_BOTTOM-1]->roll_status= FALSE;
			pca_rolling_msg[CA_OSD_BOTTOM-1]->wTimes= 0;
		}			
	}
    
    GxCore_MutexUnlock (pca_rolling_msg[CA_OSD_BOTTOM-1]->mutex);
	return TRUE;
}



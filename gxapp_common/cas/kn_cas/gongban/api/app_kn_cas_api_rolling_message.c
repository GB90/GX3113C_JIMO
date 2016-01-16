/******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2012.12.02		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_porting_stb_api.h"
#include "app_kn_cas_api_pop.h"
#include "app_kn_cas_api_rolling_message.h"
#include "gxmsg.h"
#include "cas_hcn_Public.h"
#include "gui_core.h"



extern 	void kn_cas_rolling_message_show(uint8_t byStyle,uint8_t showFlag,char* message);
extern 	int32_t kn_cas_rolling_message_get_times(void);

static int rolling_status = 0;
static int rolling_flag = 0;

static char g_rolling_osd_buf[1024]={0};
static handle_t    g_rolling_osd_buf_mutex;
void reset_rolling_status(void)
{
	rolling_status = 0;
}

void app_kn_cas_rolling_message_init()
{
    rolling_flag = 0;
    rolling_status = 0;
    memset(g_rolling_osd_buf,0,sizeof(g_rolling_osd_buf));
    if(GXCORE_SUCCESS != GxCore_MutexCreate(&g_rolling_osd_buf_mutex))
    {
        printf("GxCore_MutexCreate failed.\n");
    }
}

#if (1==LINUX_OTT_SUPPORT)
extern void transform_entry(char * utf8 , int utf8_len,unsigned char * gb);
#endif
void kn_cas_rolling_test()
{
    char* strtemp = "DIVI CAS¶ÌÏûÏ¢²âÊÔ£¬¿¨ºÅ 900001";
    memset(g_rolling_osd_buf,0,sizeof(g_rolling_osd_buf));
#if (1==LINUX_OTT_SUPPORT)
     
     char *osd_language = app_flash_get_config_osd_language();
	 if(0 == strcmp(osd_language,LANGUAGE_CHINESE))
	 {
		char utf8_str_buffer[512]={0};
		unsigned char  gb_str[256]={0};
        strcpy((char *)gb_str,strtemp);
        transform_entry(utf8_str_buffer,sizeof(utf8_str_buffer),gb_str);
		memcpy(g_rolling_osd_buf,utf8_str_buffer,strlen(utf8_str_buffer));//66);
	 }
 	 else 
#endif
       memcpy(g_rolling_osd_buf,strtemp,strlen(strtemp));

    kn_cas_rolling_message_show(0,DVB_CA_ROLLING_FLAG_RESET,g_rolling_osd_buf);
	rolling_flag = 1;
	rolling_status=0;
}


int32_t app_kn_cas_gxmsg_ca_on_event_rolling(GxMsgProperty0_OnEvent* event)
{
    char osd_buf[1024] = {0};
	hcncas_osd* rolling_msg = NULL;
    
	if (NULL == event)
		return 0;
	
	rolling_msg = (hcncas_osd*)(event->buf);

	if (NULL == rolling_msg)
	    return 0;
    
	memset(osd_buf,0,sizeof(osd_buf));
    
    printf("+++++++++++recv osd.\n");
    
    //printf("\napp_kn_cas_gxmsg_ca_on_event_rolling****%s*****\n",rolling_msg->subdata);
	//printf("\napp_kn_cas_gxmsg_ca_on_event_rolling****%d*****\n",rolling_msg->len);

    
#if (1==LINUX_OTT_SUPPORT)
     char *osd_language = app_flash_get_config_osd_language();
	 if(0 == strcmp(osd_language,LANGUAGE_CHINESE))
	 {
		char utf8_str_buffer[1024]={0};
		unsigned char  gb_str[512]={0};
        int len;
        memset(gb_str,0,sizeof(gb_str));
        len = (rolling_msg->len <= sizeof(gb_str))?rolling_msg->len:sizeof(gb_str);
        memcpy(gb_str,rolling_msg->subdata,len);
        transform_entry(utf8_str_buffer,sizeof(utf8_str_buffer),gb_str);
		memcpy(osd_buf,utf8_str_buffer,strlen(utf8_str_buffer));
        
	 }
     else
#endif
        memcpy(osd_buf,rolling_msg->subdata,sizeof(rolling_msg->subdata));

    GxCore_MutexLock (g_rolling_osd_buf_mutex);
	rolling_flag = 1;
	rolling_status = 0;
    memcpy(g_rolling_osd_buf,osd_buf,1024);
    GxCore_MutexUnlock (g_rolling_osd_buf_mutex);
	return 1;
}


int32_t app_kn_cas_rolling_message_exec(void)
{

	int wTimes;
	char* focus_Window = (char*)GUI_GetFocusWindow();
	if ((NULL != focus_Window)&&(strcasecmp("win_full_screen", focus_Window) != 0))
	{
		return 1;
	}

    GxCore_MutexLock (g_rolling_osd_buf_mutex);
	if(rolling_flag)
	{
		if(rolling_status == 0)
		{
            kn_cas_rolling_message_show(0,DVB_CA_ROLLING_FLAG_RESET,g_rolling_osd_buf);
			kn_cas_rolling_message_show(0,DVB_CA_ROLLING_FLAG_SHOW,g_rolling_osd_buf);
			rolling_status = 1;
		}
        
		wTimes = kn_cas_rolling_message_get_times();
		if (wTimes >=  1)
		{	
			kn_cas_rolling_message_show(0,DVB_CA_ROLLING_FLAG_HIDE,NULL);
			rolling_flag = 0;
			rolling_status = 0;
		}
	}
    GxCore_MutexUnlock (g_rolling_osd_buf_mutex);
	return 0;
}



#include "app_common_porting_stb_api.h"
#include "app_kp_cas_api_event.h"


static kp_ca_finger_data_st ca_finger_data = {0};
extern 	void win_kp_cas_finger_show(uint8_t showFlag,char* message);



void KPCA_toShowFinger(int OnOff,char *msg)
{
    kp_ca_finger_data_st fingerdata;
    memset(&fingerdata,0,sizeof(kp_ca_finger_data_st));
    
    if(TRUE == OnOff)
    {
        fingerdata.wStatus = TRUE;
        if(msg)
        {
            snprintf(fingerdata.msg,sizeof(fingerdata.msg)-1,"%s",msg);
        }
        
    }
    else
    {
        fingerdata.wStatus = FALSE;
    }
    app_cas_api_on_event(DVB_CA_FINGER_INFO,CAS_NAME, &fingerdata, sizeof(kp_ca_finger_data_st));
}


//消息处理
int32_t app_kp_cas_gxmsg_ca_on_event_finger(GxMsgProperty0_OnEvent* event)
{
    kp_ca_finger_data_st* finger_data = NULL;
	if (NULL == event)
		return 0;

	finger_data = (kp_ca_finger_data_st*)(event->buf);

	if (NULL == finger_data)
		return 0;

    if ((FALSE != ca_finger_data.wStatus)&&(TRUE == ca_finger_data.wShowStatus))
	{
		win_kp_cas_finger_show(DVB_CA_FINGER_FLAG_HIDE,NULL); 				
	}

    memcpy(&ca_finger_data,finger_data,sizeof(kp_ca_finger_data_st));
	ca_finger_data.wShowStatus = FALSE;//初始化未显示

	return 1;
}

//200ms定时器
int32_t app_kp_cas_finger_exec(void)
{
	if (FALSE == ca_finger_data.wStatus)
	{
		/*清除指纹信息*/
		if (TRUE == ca_finger_data.wShowStatus)
		{
			ca_finger_data.wShowStatus = FALSE;
			win_kp_cas_finger_show(DVB_CA_FINGER_FLAG_HIDE,NULL); 				
		}
	}
	else if (TRUE == ca_finger_data.wStatus)
	{
		/*显示指纹信息*/
		if (FALSE == ca_finger_data.wShowStatus)
		{
			win_kp_cas_finger_show(DVB_CA_FINGER_FLAG_SHOW,ca_finger_data.msg); 				
			ca_finger_data.wShowStatus = TRUE;
		}
	}
    return 1;
}




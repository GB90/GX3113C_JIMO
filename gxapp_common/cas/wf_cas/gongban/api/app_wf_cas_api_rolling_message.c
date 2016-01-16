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
#include "app_wf_cas_api_pop.h"
#include "app_wf_cas_api_rolling_message.h"
#include "gxmsg.h"

#include "wanfacas.h"
#include "interfac.h"

static wf_ca_rolling_message_st ca_rolling_msg = {0};
extern char * GUI_GetFocusWindow(void);

static U08 EmailInfo_Osd[3][1124] = {{0,},{0,},{0,}};
static U08 Identfier_Osd[3] = {0};
static U32 Current_show_Identfier = 0;

static U08 Have_New_Osd = 0;
static U08 OSD_Save_Num = 0;

static U08 OSD_Full_Flag = 0;
static U08 OSD_Empty_Flag = 1;

static U08 *pucRollString = NULL;

extern 	void wf_cas_rolling_message_show(uint8_t byStyle,uint8_t showFlag,char* message);
extern 	int32_t wf_cas_rolling_message_get_times(void);

void WFSTB_Set_roll_status(uint8_t flag)
{
	ca_rolling_msg.roll_status= flag;
}

U08 WFSTB_Get_Have_Osd_Flag(VOID)
{
	return Have_New_Osd;
}

VOID WFSTB_Set_Have_Osd_Flag(U08 Flag)
{
	Have_New_Osd = Flag;
	return;
}

U08* WFSTB_Get_Current_Str(VOID)
{	
	return &EmailInfo_Osd[OSD_Save_Num][0];
}

int32_t app_wf_cas_gxmsg_ca_on_event_rolling(GxMsgProperty0_OnEvent* event)
{
	wf_ca_rolling_message_st* rolling_msg = NULL;
	WFSTB_Email_t *email_rolling;

	U32 nEmailContextLen = 0xff;
	
	if (NULL == event)
		return 0;

	rolling_msg = (wf_ca_rolling_message_st*)(event->buf);

	if (NULL == rolling_msg)
		return 0;

	email_rolling = (WFSTB_Email_t *)(rolling_msg->buffer);

	if(Current_show_Identfier == email_rolling->Identfier)
	{
		printf("\n##################Current_show_Identfier == email_rolling->Identfier = %ld##################\n",Current_show_Identfier);
		return 0;
	}
	if(OSD_Full_Flag == 1)
	{
		printf("\n##################OSD_Full_Flag == 1----return##################\n");
		return 0;
	}

	nEmailContextLen = strlen(email_rolling->Text);
	if(nEmailContextLen > 1123)
	{
		nEmailContextLen = 1122;
	}

	Have_New_Osd++;
	printf("\n##################Have_New_Osd == %d##################\n",Have_New_Osd);
	if(Have_New_Osd >= 2)
	{
		OSD_Full_Flag = 1;
		printf("\n##################OSD_Full_Flag == 1----continue##################\n");
	}
	else
		OSD_Full_Flag = 0;

	if (OSD_Save_Num == 0)
	{
		OSD_Save_Num = 1;
	}
	else if (OSD_Save_Num == 1)
	{
		OSD_Save_Num = 2;
	}
	else if (OSD_Save_Num == 2)
	{
		OSD_Save_Num = 0;
	}
	memset(&EmailInfo_Osd[OSD_Save_Num][0], 0, 1124);

	memcpy(&EmailInfo_Osd[OSD_Save_Num][0],email_rolling->Text, nEmailContextLen);
	EmailInfo_Osd[OSD_Save_Num][nEmailContextLen] = '\0';

	Identfier_Osd[OSD_Save_Num] = email_rolling->Identfier;
	
	if(OSD_Empty_Flag == 1)
	{
		OSD_Empty_Flag = 0;
		ca_rolling_msg.wTimes= 0;
		ca_rolling_msg.roll_status=FALSE;
		ca_rolling_msg.wStatus = TRUE;

		pucRollString  = WFSTB_Get_Current_Str();
		wf_cas_rolling_message_show(0,DVB_CA_ROLLING_FLAG_RESET,(char*)pucRollString);		
	}
	
	return 1;
}

int32_t app_wf_cas_rolling_message_exec(void)
{
	U08 	Osd_Have;
	
	char* focus_Window = (char*)GUI_GetFocusWindow();
	if ((NULL != focus_Window)&&(strcasecmp("win_full_screen", focus_Window) != 0))
	{
		return 1;
	}
	
	if (TRUE == ca_rolling_msg.wStatus )
	{
		if (FALSE == ca_rolling_msg.roll_status)
		{
			pucRollString  = WFSTB_Get_Current_Str();
			Current_show_Identfier = Identfier_Osd[OSD_Save_Num];
			wf_cas_rolling_message_show(0,DVB_CA_ROLLING_FLAG_SHOW,(char*)pucRollString);
			printf("\nwf_cas_rolling_message_show:%s	Current_show_Identfier=%ld\n",pucRollString,Current_show_Identfier);
			ca_rolling_msg.roll_status= TRUE;	

		}

		ca_rolling_msg.wTimes = wf_cas_rolling_message_get_times();
		if (ca_rolling_msg.wTimes >=  1)
		{
			Osd_Have = WFSTB_Get_Have_Osd_Flag();
			if ((Osd_Have == 2) || (Osd_Have == 1))
			{
				Osd_Have--;
			}
			WFSTB_Set_Have_Osd_Flag(Osd_Have);
			printf("\n##################Set Have_New_Osd == %d##################\n",Have_New_Osd);
			if (Osd_Have < 2)
				OSD_Full_Flag = 0;
			if (Osd_Have > 0)
			{
				pucRollString  = WFSTB_Get_Current_Str();
				Current_show_Identfier = Identfier_Osd[OSD_Save_Num];
				wf_cas_rolling_message_show(0,DVB_CA_ROLLING_FLAG_RESET,(char*)pucRollString);		
				wf_cas_rolling_message_show(0,DVB_CA_ROLLING_FLAG_SHOW,(char*)pucRollString);
				printf("\nwf_cas_rolling_message_show:%s	Current_show_Identfier=%ld\n",pucRollString,Current_show_Identfier);
			}
			else
			{
			       // 查询滚动结束，清除OSD滚动标志
				ca_rolling_msg.wStatus = FALSE;
				ca_rolling_msg.roll_status= FALSE;	
				wf_cas_rolling_message_show(0,DVB_CA_ROLLING_FLAG_HIDE,NULL);
				ca_rolling_msg.wTimes= 0;
				Current_show_Identfier = 0;
				OSD_Empty_Flag = 1;
			}
		}
	}
	else
	{
		if (TRUE == ca_rolling_msg.roll_status)
		{
			wf_cas_rolling_message_show(0,DVB_CA_ROLLING_FLAG_HIDE,NULL);
			ca_rolling_msg.roll_status= FALSE;
//			ca_rolling_msg.wTimes= 0;
//			ca_rolling_msg.wDuration=0;
//			ca_rolling_msg.wStartTime=0; 
		}
	}
	return TRUE;
}



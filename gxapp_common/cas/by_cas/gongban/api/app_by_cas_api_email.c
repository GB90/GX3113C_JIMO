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
#include "app_by_cas_api_email.h"
#include "app_common_epg.h"
#include "BYCAca_stb.h"


BU16	p_byEmailCount = 0;
BU16	p_byNewEmailCount = 0;

MAIL_DATA P_byEmailData[EMail_MAXNUM];


static by_cas_email_st ca_email_flag = {0};
extern 	void app_by_cas_email_flag_img_show(uint8_t showFlag);
int32_t app_by_cas_gxmsg_ca_on_event_email(GxMsgProperty0_OnEvent* event)
{
	by_cas_email_st* email_flag = NULL;
	if (NULL == event)
		return 0;

	email_flag = (by_cas_email_st*)(event->buf);

	if (NULL == email_flag)
		return 0;

	printf("email_flag->wStatus=%d\n",email_flag->wStatus);


//	if ((TRUE == ca_email_flag.wShowStatus)&&(ca_email_flag.wStatus == email_flag->wStatus))
//		return 0;

	if ((email_flag->wStatus != ca_email_flag.wStatus)&&(TRUE == ca_email_flag.wShowStatus))
	{
		/*
		* 关闭之前显示
		*/
		app_by_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_HIDE);
		ca_email_flag.wStatus = 0;
		ca_email_flag.wShowStatus = 0;
		ca_email_flag.uCount = 0;
	}

	ca_email_flag.wStatus = email_flag->wStatus;
//	memcpy(&ca_email_flag,email_flag,sizeof(by_cas_email_st));


	
	return 1;
}

/*
* 反授权图标显示、隐藏
*/
int32_t app_by_cas_email_flag_exec()
{
	uint8_t detitle_status = ca_email_flag.wStatus;
	if ( DVB_CA_EMAIL_NEW == detitle_status)
		{	
			/* 收到新的反授权码，显示反授权码图标 */
			if (FALSE == ca_email_flag.wShowStatus)
				{
					app_by_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_SHOW);
				}
			ca_email_flag.wShowStatus = TRUE;
		}
	else
		if ( DVB_CA_EMAIL_EXHAUST == detitle_status)
			{
				ca_email_flag.uCount++;
				ca_email_flag.uCount = ca_email_flag.uCount%3;
				
				/* 反授权码空间不足，改变图标状态提示用户 */

						if (0 != ca_email_flag.uCount)
						{						
							app_by_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_SHOW);
							ca_email_flag.wShowStatus = TRUE;
						}
						else
						{
							app_by_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_TRANS_HIDE);
							ca_email_flag.wShowStatus = FALSE;
						}					
			}
		else
			if (DVB_CA_EMAIL_FLAG_HIDE == detitle_status)
			{	
				/* 要求隐藏邮件图标 */
				if (TRUE == ca_email_flag.wShowStatus)
					{
						app_by_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_HIDE);
					}
				ca_email_flag.wShowStatus = FALSE;
			}
		return 1;
	
}



int32_t app_by_cas_api_init_email_data(void)
{  
	BU16      i = 0;  
	BU16	pEmailCount = 0;
	BU16	pNewEmailCount = 0;
	memset(&P_byEmailData[0],0,EMail_MAXNUM*sizeof(MAIL_DATA));

	if (TRUE ==  BYCASTB_GetEmailCount(&pEmailCount,&pNewEmailCount))
		{
			printf("app_by_cas_api_init_email_data pEmailCount=%d  pNewEmailCount=%d\n",pEmailCount,pNewEmailCount);
			p_byEmailCount = pEmailCount;
			p_byNewEmailCount = pNewEmailCount;
		}

	for (i =0; i< p_byEmailCount; i++)
		{
			BYCASTB_GetEmailContent(i,&P_byEmailData[i]);			
		}
	return 0;

}


char * app_by_cas_api_get_email_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0; 
	uint16_t timetmp = 0;
//	com_time_t Time_t = {0};
	uint16_t	 year = 0;
	uint8_t  month=0;
	uint8_t  day=0;
	uint8_t  weekDay=0;
	uint8_t  hour=0;
	uint8_t  minute=0;


    static char   cTextBuffer_flag[10] =
    {
        0
    };
    static char   cTextBuffer_sendtime[20] =
    {
        0
    };
    static char   cTextBuffer_endtime[20] =
    {
        0
    };


	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	switch(ID)
		{
			case BY_EMAIL_FLAG_ID:
				memset(cTextBuffer_flag,0,sizeof(cTextBuffer_flag));
				if (2 == P_byEmailData[pos].MessageFlag)
					strcpy(cTextBuffer_flag,"New");
				if (3 == P_byEmailData[pos].MessageFlag)
					strcpy(cTextBuffer_flag,"Readed");
				return cTextBuffer_flag;
			case BY_EMAIL_TITLE_ID:
				return (char*)P_byEmailData[pos].Title;
			case BY_EMAIL_SENDTIME_ID:
				memset(cTextBuffer_sendtime,0,sizeof(cTextBuffer_sendtime));
				timetmp = (uint16_t)(( P_byEmailData[pos].SendTime[0]<<8 )|P_byEmailData[pos].SendTime[1]);				
				app_epg_convert_mjd_to_ymd( timetmp, &year,  &month, &day, &weekDay );
				hour = ((P_byEmailData[pos].SendTime[2] &0xF0) >>4)*10 + (P_byEmailData[pos].SendTime[2] &0x0F);
				minute = ((P_byEmailData[pos].SendTime[3] &0xF0) >>4)*10 + (P_byEmailData[pos].SendTime[3] &0x0F);	
				sprintf( cTextBuffer_sendtime, "%04d-%02d-%02d  %02d:%02d", year, month,
            			 day,hour,minute);
				return cTextBuffer_sendtime;
			case BY_EMAIL_VALIDTIME_ID:
				memset(cTextBuffer_endtime,0,sizeof(cTextBuffer_endtime));
				timetmp = (uint16_t)(( P_byEmailData[pos].VaildTime[0]<<8 )|P_byEmailData[pos].VaildTime[1]);
				app_epg_convert_mjd_to_ymd( timetmp, &year,  &month, &day, &weekDay );
				hour = ((P_byEmailData[pos].VaildTime[2] &0xF0) >>4)*10 + (P_byEmailData[pos].VaildTime[2] &0x0F);
				minute = ((P_byEmailData[pos].VaildTime[3] &0xF0) >>4)*10 + (P_byEmailData[pos].VaildTime[3] &0x0F);	
				sprintf( cTextBuffer_endtime, "%04d-%02d-%02d %02d:%02d", year, month,
            			 day, hour,minute);
				return cTextBuffer_endtime;
			case BY_EMAIL_CONTENT_ID:
				BYCASTB_EmailRead(pos);
				return (char*)P_byEmailData[pos].Content;
			default:
				break;
		}
	return NULL;

}

uint8_t app_by_cas_api_get_email_count(ca_get_count_t *data)
{
	app_by_cas_api_init_email_data();
	data->totalnum = p_byEmailCount;
	data->newnum = p_byNewEmailCount;
	printf("data->totalnum=%d, data->newnum=%d\n",data->totalnum,data->newnum);
    return p_byEmailCount;
}

bool app_by_cas_api_delete_email(ca_delete_data_t *data)
{
	uint32_t pos = 0 ;                             
	if (NULL == data)
		return FALSE;

	pos = data->pos;
	pos +=1;
	BYCASTB_DelEmail(pos) ;    
	return 0;
}

bool app_by_cas_api_delete_all_email(ca_delete_data_t *data)
{
	if (NULL == data)
		return FALSE;

	BYCASTB_DelEmail(0);	

	return 0;
}



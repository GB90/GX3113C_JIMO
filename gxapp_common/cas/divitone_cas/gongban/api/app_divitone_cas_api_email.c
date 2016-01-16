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
#include "app_divitone_cas_api_email.h"
#include "app_common_epg.h"
#include "app_divitone_cas_api_pop.h"
#include "DiviGuardCa.h"


UINT16	p_divitoneEmailCount = 0;
UINT16	p_divitoneNewEmailCount = 0;

TDIVI_EmailHead EmailHeads[10]; 
TDIVI_EmailContent EmailContent;

static divitone_cas_email_st ca_email_flag = {0};

extern void app_divitone_cas_email_flag_img_show(uint8_t showFlag);

int32_t app_divitone_cas_gxmsg_ca_on_event_email(GxMsgProperty0_OnEvent* event)
{
	divitone_cas_email_st* email_flag = NULL;
	if (NULL == event)
		return 0;

	email_flag = (divitone_cas_email_st*)(event->buf);

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
		app_divitone_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_HIDE);
		ca_email_flag.wStatus = 0;
		ca_email_flag.wShowStatus = 0;
		ca_email_flag.uCount = 0;
	}

	ca_email_flag.wStatus = email_flag->wStatus;
//	memcpy(&ca_email_flag,email_flag,sizeof(divitone_cas_email_st));
	
	return 1;
}


int32_t app_divitone_cas_email_flag_exec(void)
{
	uint8_t detitle_status = 0;
	if((p_divitoneNewEmailCount !=0 ) && (ca_email_flag.wStatus == DVB_CA_EMAIL_FLAG_HIDE))
	{
		ca_email_flag.wStatus = DVB_CA_EMAIL_NEW;
		//ca_email_flag.wShowStatus = TRUE;
	}
	else if((ca_email_flag.wShowStatus == TRUE) && ((p_divitoneEmailCount == 0) || (p_divitoneNewEmailCount == 0)))
	{
		//app_divitone_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_HIDE);
		//ca_email_flag.wShowStatus = FALSE;
		app_divitone_cas_api_init_email_data();
		if(p_divitoneNewEmailCount == 0)
			ca_email_flag.wStatus = DVB_CA_EMAIL_FLAG_HIDE;
	}
	detitle_status = ca_email_flag.wStatus;
	
	if ( DVB_CA_EMAIL_NEW == detitle_status)
	{	
		if (FALSE == ca_email_flag.wShowStatus)
		{
			app_divitone_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_SHOW);
		}
		ca_email_flag.wShowStatus = TRUE;
	}
	else if ( DVB_CA_EMAIL_EXHAUST == detitle_status)
	{
		ca_email_flag.uCount++;
		ca_email_flag.uCount = ca_email_flag.uCount%3;
		
		if (0 != ca_email_flag.uCount)
		{						
			app_divitone_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_SHOW);
			ca_email_flag.wShowStatus = TRUE;
		}
		else
		{
			app_divitone_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_TRANS_HIDE);
			ca_email_flag.wShowStatus = FALSE;
		}					
	}
	else if (DVB_CA_EMAIL_FLAG_HIDE == detitle_status)
	{	
		/* 要求隐藏邮件图标 */
		if (TRUE == ca_email_flag.wShowStatus)
		{
			app_divitone_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_HIDE);
		}
		ca_email_flag.wShowStatus = FALSE;
	}
	//app_divitone_cas_api_init_email_data();
	return 1;
	
}


//extern int32_t app_divitone_cas_gxmsg_ca_on_event_rolling_test(void);

int32_t app_divitone_cas_api_init_email_data(void)
{  
	p_divitoneEmailCount = 0;
	p_divitoneNewEmailCount = 0;

	p_divitoneEmailCount = DIVI_GetEmailHeads(&EmailHeads[0], 10);
	p_divitoneNewEmailCount = DIVI_GetNewMailCount();
	//app_divitone_cas_gxmsg_ca_on_event_rolling_test();	
	return 0;
}


char * app_divitone_cas_api_get_email_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0; 
	
	unsigned char tmp,tmp1,tmp2,tmp3;
	static char s_text_buffer[30]={0};
	static char s_text_flag_id[10]={0};

	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	memset(s_text_buffer,0,sizeof(s_text_buffer));
	

	switch(ID)
		{
			case DIVI_EMAIL_FLAG_ID:
				{
					sprintf(s_text_flag_id, "%02d", EmailHeads[pos].m_bEmailID);
				//	sprintf(s_text_buffer, "%02d", pos);
					return s_text_flag_id;
				}
				break;
			case DIVI_EMAIL_LEVEL_ID:
				if(EmailHeads[pos].m_bEmail_Level)
				{
					return "important";
				}
				else
				{
					return "normal";
				}
				break;
			case DIVI_EMAIL_IS_NEW_ID:
				if(EmailHeads[pos].m_bNewEmail == 1)
					return "new";
				else
					return "readed";
				break;
			case DIVI_EMAIL_TIME_ID:
				tmp1=EmailHeads[pos].m_tCreateTime.M;
				tmp2=EmailHeads[pos].m_tCreateTime.D;

				tmp=tmp1/10;
				s_text_buffer[0]=tmp+0x30;
				s_text_buffer[1]=(tmp1-tmp*10)+0x30;
				s_text_buffer[2]='-';

				tmp=tmp2/10;
				s_text_buffer[3]=tmp+0x30;
				s_text_buffer[4]=(tmp2-tmp*10)+0x30;
				s_text_buffer[5]=' ';
				 

				tmp1=EmailHeads[pos].m_tCreateTime.H;
				tmp2=EmailHeads[pos].m_tCreateTime.MI;
				tmp3=EmailHeads[pos].m_tCreateTime.S;
			
				tmp=tmp1/10;
				s_text_buffer[6]=tmp+0x30;
				s_text_buffer[7]=(tmp1-tmp*10)+0x30;
				s_text_buffer[8]=':';

				tmp=tmp2/10;
				s_text_buffer[9]=tmp+0x30;
				s_text_buffer[10]=(tmp2-tmp*10)+0x30;
				s_text_buffer[11]=':';

				tmp=tmp3/10;
				s_text_buffer[12]=tmp+0x30;
				s_text_buffer[13]=(tmp3-tmp*10)+0x30;
				s_text_buffer[14]='\0';

				return s_text_buffer;

			case DIVI_EMAIL_TITLE_ID:
				return EmailHeads[pos].m_szEmailTitle;
				break;
			case DIVI_EMAIL_CONTENT_ID:
				DIVI_GetEmailContent(EmailHeads[pos].m_bEmailID,&EmailContent);					
				return EmailContent.m_szEmail;
				break;
			default:
				break;
		}
	return NULL;

}

uint8_t app_divitone_cas_api_get_email_count(ca_get_count_t *data)
{
	app_divitone_cas_api_init_email_data();
	data->totalnum = p_divitoneEmailCount;
	data->newnum = p_divitoneNewEmailCount;
    	return p_divitoneEmailCount;
}

bool app_divitone_cas_api_delete_email(ca_delete_data_t *data)
{
	uint32_t pos = 0 ;                             
	if (NULL == data)
		return FALSE;

	pos = data->pos;
//	pos +=1;
	DIVI_DelEmail(EmailHeads[pos].m_bEmailID) ;    
	return 0;
}

bool app_divitone_cas_api_delete_all_email(ca_delete_data_t *data)
{
	if (NULL == data)
		return FALSE;
	uint32_t i = 0 ;
	for(i=0;i<p_divitoneEmailCount;i++)
		DIVI_DelEmail(EmailHeads[i].m_bEmailID);

	return 0;
}



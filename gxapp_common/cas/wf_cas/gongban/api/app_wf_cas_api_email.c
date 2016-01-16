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
#include "wanfacas.h"
#include "interfac.h"
#include "app_common_porting_stb_api.h"
#include "app_wf_cas_api_email.h"
#include "app_common_epg.h"
#include "app_wf_cas_api_pop.h"


#include	"gx_email.h"

#define MAX_EMAIL_NUM  			60

typedef struct EmailInfo_s
{
	handle_t mail;
	int32_t email_count;
	int32_t email_unread_count;
	int32_t mail_id[MAX_EMAIL_NUM];
	char title[MAX_EMAIL_NUM][64];
	time_t create_time[MAX_EMAIL_NUM];
	int8_t mail_state[MAX_EMAIL_NUM];
	char mail_context[1024];
}EmailInfo_t;
EmailInfo_t EmailData;

uint32_t Identfier_email[60] = {0,};


static wf_cas_email_st ca_email_flag = {0};
extern 	void app_wf_cas_email_flag_img_show(uint8_t showFlag);
extern char * GUI_GetFocusWindow(void);

void app_open_mail_area(void)
{
	EmailData.mail = GxEmail_Open("/home/gx/wfca.dat",90*1024);//每封邮件1520字节的大小
	if(EmailData.mail == E_INVALID_HANDLE )
    	{
           printf("open cas.mail file err!\n");
    	}
}
int app_email_add(WFSTB_Email_t *data)
{
       int32_t ret;
	struct tm LocalTime;
       Email_t  *mail=NULL;

	int32_t maillen = 0;
	uint32_t Identfier_bak = 0;
						
	mail = malloc(sizeof(Email_t));
	if(NULL==mail)
	{		
		printf("email alloc memory err!\n");
		return -1;
	}

	maillen = strlen(data->Text);
	if(maillen > 1020)
	{
		maillen = 1019;
		mail->mail_body.context[1023] = '\0';
	}
	printf("maillen = %d !!!!\n",maillen);
	
	memset(mail,0,sizeof(Email_t));

	if(maillen > 64)
	{
		memcpy(mail->mail_head.title,data->Text,63);
		mail->mail_head.title[63] = '\0';
	}
	else
		memcpy(mail->mail_head.title,data->Text,maillen);
	printf("mail->mail_head.title = %s !!!!\n",mail->mail_head.title);
	
	mail->mail_head.mail_id = data->Identfier;
	printf("mail->mail_head.mail_id = %d !!!!\n",mail->mail_head.mail_id);
	Identfier_bak = data->Identfier;
	mail->mail_body.context[0] = (char)((Identfier_bak&0xff000000)>>24);
	mail->mail_body.context[1] = (char)((Identfier_bak&0x00ff0000)>>16);
	mail->mail_body.context[2] = (char)((Identfier_bak&0x0000ff00)>>8);
	mail->mail_body.context[3] = (char)(Identfier_bak&0x000000ff);
	printf("%d	%d	%d	%d\n",mail->mail_body.context[0],mail->mail_body.context[1],mail->mail_body.context[2],mail->mail_body.context[3]);
	
 	LocalTime.tm_sec = data->Property.DateTime.Second;
 	LocalTime.tm_min = data->Property.DateTime.Minute;
	LocalTime.tm_hour = data->Property.DateTime.Hour;
	LocalTime.tm_mday = data->Property.DateTime.Day;
	LocalTime.tm_mon = data->Property.DateTime.Month;
	LocalTime.tm_year = data->Property.DateTime.Year;
	
	mail->mail_head.creat_time = mktime(&LocalTime);


       memcpy(mail->mail_body.context + 4,data->Text,maillen);
	printf("mail->mail_body.context = %s !!!!\n",mail->mail_body.context+4);
	ret=GxEmail_Add(EmailData.mail,mail);
	if(ret==0)
	{
		 printf("email added success !!!!\n");
	}
       free(mail);
	mail=NULL;

	return 0;
}

void app_cas_set_mail_init(void)
{
	  EmailData.email_count=0;
	  EmailData.email_unread_count=0;
	 
	  memset(EmailData.mail_id,0,sizeof(EmailData.mail_id));
	  memset(EmailData.mail_state,0,sizeof(EmailData.mail_state));
	  memset(EmailData.create_time,0,sizeof(EmailData.create_time));
	  memset(EmailData.title,0,sizeof(EmailData.title));
	  memset(EmailData.mail_context,0,sizeof(EmailData.mail_context));
}

int app_cas_get_mail_headlist(void)
{
	int ret=-1;
	int8_t mail_count = 0,i=0;
	
	EmailBox_t boxinfo;
	EmailHead_t* pEmailHead=NULL;
	
	ret =  GxEmail_ReadBoxInfo(EmailData.mail,&boxinfo);

	if(ret<0)
	{
		printf("\n get mail count[0]\n");
		return -1;
	}
	EmailData.email_count=boxinfo.total_num;
	EmailData.email_unread_count=boxinfo.unreaded_num;

	printf("\napp_cas_get_mail_count..[%d]...\n",boxinfo.total_num);
	
	if(EmailData.email_count==0)
		return -1;

	if(EmailData.email_count>MAX_EMAIL_NUM)
		EmailData.email_count=MAX_EMAIL_NUM;
		
	pEmailHead = malloc(EmailData.email_count * sizeof(EmailHead_t));
	if(NULL==pEmailHead)
	{
		printf("\n malloc email head err!!!\n");
		return -1;
	}
	mail_count = GxEmail_ReadHeads(EmailData.mail, 0, EmailData.email_count, pEmailHead);
	if(mail_count!=EmailData.email_count)
	{
		printf("\n Error !!!Not Equal ReadHeads:[%d]  EmailData.email_count[%d]",mail_count,EmailData.email_count);
		free(pEmailHead);
		pEmailHead=NULL;
		return -1;
	}
	for(i = 0; i < EmailData.email_count; i++)
	{
		EmailData.mail_id[i]=pEmailHead[i].mail_id;
	   	EmailData.mail_state[i]=pEmailHead[i].mail_state; // 1:是新邮件,0是已读邮件
		EmailData.create_time[i]=pEmailHead[i].creat_time;
		
		memcpy(&(EmailData.title[i]), pEmailHead[i].title, strlen(pEmailHead[i].title));
	}
	
	if(NULL!=pEmailHead)
	{
		free(pEmailHead);
		pEmailHead=NULL;
	}
	
	return EmailData.email_count;
}

int app_cas_check_mail_onflash(int32_t Identfier)
{
	uint8_t i = 0;
	int32_t ret;
	uint32_t Identfier_temp = 0;
	EmailBody_t pEmailBody;
	app_cas_set_mail_init();
	app_cas_get_mail_headlist();
//	if(EmailData.email_count > MAX_EMAIL_NUM)		//邮件满
//		return 1;
	memset(&pEmailBody,0,sizeof(EmailBody_t));


	for(i = 0; i<EmailData.email_count; i++)
	{
		ret=GxEmail_ReadBody(EmailData.mail, EmailData.mail_id[i], &pEmailBody);
	       if( ret >= 0 )
	       {
	           	;
	       }
	       else
	       {
	       	printf("get mail body failure!\n");
	       }
		Identfier_temp = (uint32_t)(pEmailBody.context[0]<<24)+(uint32_t)(pEmailBody.context[1]<<16)+(uint32_t)(pEmailBody.context[2]<<8)+(uint32_t)(pEmailBody.context[3]);
		Identfier_email[i] = Identfier_temp;
		printf("Identfier_email[%d] = %d\n",i,Identfier_email[i]);
/*		if(EmailData.mail_id[i] == Identfier)
		{
			return 1;
		}		*/
	}
	for(i = 0; i<EmailData.email_count; i++)
		if(Identfier_email[i] == Identfier)
			return 1;
		
	return 0;
}

int app_cas_check_mail_new(void)
{
	uint8_t i = 0;
	
	for(i = 0; i<EmailData.email_count; i++)
	{
		if(EmailData.mail_state[i])
		{
			return 1;
		}
	}
	return 0;
}

void app_cas_set_new_mail(uint8_t flag)
{
	ca_email_flag.wStatus = flag;
}

int app_cas_get_mail_count(void)
{
	 return  EmailData.email_count;
}
int app_cas_get_email_unread_count(void)
{
	 return  EmailData.email_unread_count;
}

void app_cas_get_mail_body(int32_t  mail_sel)
{
	int32_t ret;
	int mail_id = 0;
	EmailBody_t *pEmailBody = NULL;
	
	memset(EmailData.mail_context, 0, 1024);
	if((mail_sel >= EmailData.email_count)||(mail_sel < 0))
	{
		printf("[mail] select failure!\n");
		return;
	}
	mail_id = EmailData.mail_id[mail_sel];
	pEmailBody = malloc(sizeof(EmailBody_t));
	if(NULL==pEmailBody)
	{
		printf("\n malloc email body err!!!\n");
		return;
	}
	
	ret=GxEmail_ReadBody(EmailData.mail, mail_id, pEmailBody);
       if( ret >= 0 )
       {
           	printf("get a mail body!\n");
       }
       else
       {
       	printf("get mail body failure!\n");
       }
	memcpy(EmailData.mail_context, pEmailBody->context+4, strlen(pEmailBody->context+4));
	free(pEmailBody);
	pEmailBody=NULL;
}
void app_cas_set_mail_readed(int32_t mail_sel)
{
	int mail_id = 0;
	if((mail_sel >= EmailData.email_count)||(mail_sel < 0))
	{
		printf("[mail] select failure!\n");
		return;
	}
	mail_id = EmailData.mail_id[mail_sel];
	GxEmail_SetReaded(EmailData.mail, mail_id);
}

void app_cas_delete_one_mail(int32_t mail_sel)
{
	int mail_id = 0;
	if(mail_sel >= EmailData.email_count&&mail_sel<0)
	{
		printf("[mail] select failure!\n");
		return;
	}

	mail_id = EmailData.mail_id[mail_sel];
	GxEmail_Del(EmailData.mail, mail_id);
}
void app_cas_delete_all_mail(void)
{
	uint8_t i = 0;

	for(i = 0; i < EmailData.email_count; i++)
	{
		app_cas_delete_one_mail(i);
	}
}

uint8_t app_wf_cas_api_init_email_data(void)
{
	app_cas_set_mail_init();
	app_cas_get_mail_headlist();
	return 1;
}

char * app_wf_cas_api_get_email_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0;                               

    	static char   email_no[20] =
    	{
        	0
    	};

    	static char   email_date[40] =
    	{
        	0
    	};

	int32_t  nTime = 0;
	struct tm *local;



	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	switch(ID)
		{
			case WF_EMAIL_NO_ID:
				{
				//	sprintf(email_no, "%02d", EmailData.mail_id[pos]);
					sprintf(email_no, "%02d", pos);
					return email_no;
				}
				break;
			case WF_EMAIL_IS_NEW_ID:
				if (1 == EmailData.mail_state[pos])
				{
					return (char*)"New";		
				}
				else
				{
					return (char*)"Readed";				
				}				
				break;
			case WF_EMAIL_DATE_ID:
				nTime = EmailData.create_time[pos]+8*60*60;
				local = localtime((const time_t*)&nTime);
				sprintf( email_date, "%04d-%02d-%02d %02d:%02d", 1900 + local->tm_year, 
						local->tm_mon+1, 
						local->tm_mday,
						local->tm_hour,
						local->tm_min
						);
				return email_date;				
			case WF_EMAIL_HEAD_ID:
				return EmailData.title[pos];//pEmail[pos].m_szEmailHead;
				break;
			case WF_EMAIL_CONTENT:
				app_cas_get_mail_body(pos);
				printf("EmailData.mail_context:%s\n",EmailData.mail_context);
				return EmailData.mail_context;//pEmail[pos].m_szEmailHead;
				break;				
			default:
				break;
		}
	return NULL;
}


uint8_t app_wf_cas_api_get_email_count(ca_get_count_t *data)
{	
	if (NULL == data)
		return 0;

	data->totalnum = app_cas_get_mail_count();
	data->newnum = app_cas_get_email_unread_count();
	app_wf_cas_api_init_email_data();

    return app_cas_get_mail_count();
}

bool app_wf_cas_api_delete_email(ca_delete_data_t *data)
{
	uint32_t pos = 0 ;                             
	if (NULL == data)
		return FALSE;

	pos = data->pos;

	app_cas_delete_one_mail(pos); 
	return 0;
}

bool app_wf_cas_api_delete_all_email(ca_delete_data_t *data)
{
	if (NULL == data)
		return FALSE;

	app_cas_delete_all_mail();

	return 0;
}


int32_t app_wf_cas_gxmsg_ca_on_event_email(GxMsgProperty0_OnEvent* event)
{
	wf_cas_email_st* email_flag = NULL;
	WFSTB_Email_t *email;

	uint8_t email_status = FALSE;
	
	char* focus_Window = (char*)GUI_GetFocusWindow();
	if ((NULL != focus_Window)&&(strcasecmp("win_full_screen", focus_Window) != 0))
	{
		return 0;
	}
	
	if (NULL == event)
		return 0;

	email_flag = (wf_cas_email_st*)(event->buf);

	if (NULL == email_flag)
		return 0;

	email = (WFSTB_Email_t *)email_flag->buffer;
	printf("++++++++++email->Identfier=%ld",email->Identfier);
	if(app_cas_check_mail_onflash(email->Identfier) == 1)
	{
		printf("++++++++++++the same mail++++++++++++email->Identfier=%ld\n",email->Identfier);
		return 0;
	}

//	if ((TRUE == ca_email_flag.wShowStatus)&&(ca_email_flag.wStatus == email_flag->wStatus))
//		return 0;
	
	app_email_add(email);
//	printf("\nsizeof(WFSTB_Email_t) = %d\n",sizeof(Email_t));

	if ((NULL != focus_Window)&&(strcasecmp("win_full_screen", focus_Window) != 0))
	{
		email_status = ca_email_flag.wShowStatus;
		memcpy(&ca_email_flag,email_flag,sizeof(wf_cas_email_st));
		ca_email_flag.wShowStatus = email_status;
		return 1;
	}

	if ((0 != ca_email_flag.wStatus)&&(TRUE == ca_email_flag.wShowStatus))
	{
		/*
		* 关闭之前显示
		*/
	//	app_wf_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_HIDE);
	//	ca_email_flag.wStatus = 0;
	//	ca_email_flag.wShowStatus = 0;
	//	ca_email_flag.uCount = 0;
	}

	memcpy(&ca_email_flag,email_flag,sizeof(wf_cas_email_st));


	
	return 1;
}


int32_t app_wf_cas_email_flag_exec()
{
	uint8_t detitle_status = ca_email_flag.wStatus;

	char* focus_Window = (char*)GUI_GetFocusWindow();
	if ((NULL != focus_Window)&&(strcasecmp("win_full_screen", focus_Window) != 0))
	{	
		return 1;
	}
	
	if ( DVB_CA_EMAIL_NEW == detitle_status)
		{	
			/* 收到新的反授权码，显示反授权码图标 */
			if (FALSE == ca_email_flag.wShowStatus)
				{
					app_wf_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_SHOW);
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
							app_wf_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_SHOW);
							ca_email_flag.wShowStatus = TRUE;
						}
						else
						{
							app_wf_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_TRANS_HIDE);
							ca_email_flag.wShowStatus = FALSE;
						}					
			}
		else
			if (DVB_CA_EMAIL_FLAG_HIDE == detitle_status)
			{	
				/* 要求隐藏邮件图标 */
				if (TRUE == ca_email_flag.wShowStatus)
					{
						app_wf_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_HIDE);
					}
				ca_email_flag.wShowStatus = FALSE;
			}
		return 1;
	
}



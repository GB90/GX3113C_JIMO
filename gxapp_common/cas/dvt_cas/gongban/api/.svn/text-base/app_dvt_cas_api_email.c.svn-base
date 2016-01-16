/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_dvt_cas_api_email.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.12.05		  zhouhm 	 			creation
*****************************************************************************/
#include "DVTCAS_STBInf.h"
#include "app_common_porting_stb_api.h"
#include "app_dvt_cas_api_email.h"
#include "app_common_epg.h"
#include "app_dvt_cas_api_pop.h"


static dvt_cas_email_st ca_email_flag = {0};

WORD  pEmailCount = 0;
WORD  pNewEmailCount = 0;
WORD EmailCount = 50;
SDVTCAEmailInfo  pEmail[50];
SDVTCAEmailContent  pEmailContent = {0};

extern 	void app_dvt_cas_email_flag_img_show(uint8_t showFlag);
int32_t app_dvt_cas_gxmsg_ca_on_event_email(GxMsgProperty0_OnEvent* event)
{
	dvt_cas_email_st* email_flag = NULL;
	if (NULL == event)
		return 0;

	email_flag = (dvt_cas_email_st*)(event->buf);

	if (NULL == email_flag)
		return 0;

//	if ((TRUE == ca_email_flag.wShowStatus)&&(ca_email_flag.wStatus == email_flag->wStatus))
//		return 0;

	if ((0 != ca_email_flag.wStatus)&&(TRUE == ca_email_flag.wShowStatus))
	{
		/*
		* 关闭之前显示
		*/
		app_dvt_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_HIDE);
		ca_email_flag.wStatus = 0;
		ca_email_flag.wShowStatus = 0;
		ca_email_flag.uCount = 0;
	}

	memcpy(&ca_email_flag,email_flag,sizeof(dvt_cas_email_st));


	
	return 1;
}

/*
* 反授权图标显示、隐藏
*/
int32_t app_dvt_cas_email_flag_exec(void)
{
	uint8_t detitle_status = ca_email_flag.wStatus;
	if ( DVB_CA_EMAIL_NEW == detitle_status)
		{	
			/* 收到新的反授权码，显示反授权码图标 */
			if (FALSE == ca_email_flag.wShowStatus)
				{
					app_dvt_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_SHOW);
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
							app_dvt_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_SHOW);
							ca_email_flag.wShowStatus = TRUE;
						}
						else
						{
							app_dvt_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_TRANS_HIDE);
							ca_email_flag.wShowStatus = FALSE;
						}					
			}
		else
			if (DVB_CA_EMAIL_FLAG_HIDE == detitle_status)
			{	
				/* 要求隐藏邮件图标 */
				if (TRUE == ca_email_flag.wShowStatus)
					{
						app_dvt_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_HIDE);
					}
				ca_email_flag.wShowStatus = FALSE;
			}
		return 1;
	
}

uint8_t app_dvt_cas_api_init_email_data(void)
{
	uint8_t i;
	uint8_t j;
	SDVTCAEmailInfo  pEmailTemp={0};

	pEmailCount = 50;
	pNewEmailCount = 50;	
	if (DVTCA_OK == DVTCASTB_GetEmailCount(&pEmailCount, &pNewEmailCount))
	{
		printf("app_email_GetEmailCount pEmailCount=%d pNewEmailCount=%d\n",
			pEmailCount,pNewEmailCount);
	}
	else
	{
		return 0;
	}


	EmailCount = 50;
	if (DVTCA_OK ==  DVTCASTB_GetEmailHead(&EmailCount, &(pEmail[0])))
	{
		for (i =0; i< EmailCount-1;i++)
		{
			for (j=i+1; j<EmailCount;j++)
			{
				if (pEmail[i].m_dwVersion < pEmail[j].m_dwVersion)
				{
					memcpy(&pEmailTemp,&pEmail[i],sizeof(SDVTCAEmailInfo));
					memcpy(&pEmail[i],&pEmail[j],sizeof(SDVTCAEmailInfo));
					memcpy(&pEmail[j],&pEmailTemp,sizeof(SDVTCAEmailInfo));
				}
			}
		}
		printf("app_email_GetEmailHead EmailCount=%d\n",EmailCount);
//			return 1;
	}
	else
	{
		return 0;
	}
	
	return EmailCount;


}

char * app_dvt_cas_api_get_email_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0;                               

	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	switch(ID)
		{
			case DVT_EMAIL_STATE_ID:
				if ( DVTCAS_EMAIL_STATUS_READ == pEmail[pos].m_Status)
				{
					return (char*)"Readed";
				}
				else
				{
					return (char*)"Unread";
				}				
				break;
			case DVT_EMAIL_TITLE_ID:
				return (char *)pEmail[pos].m_szTitle;
			case DVT_EMAIL_DESPATCHER_ID:
				return (char *)	pEmail[pos].m_szSenderName;
			case DVT_EMAIL_CONTENT_ID:
				if (DVTCA_OK ==  DVTCASTB_GetEmailContent(pEmail[pos].m_dwVersion, &pEmailContent))
					{
				
						DVTCASTB_EmailRead(pEmail[pos].m_dwVersion);
						return (char *)pEmailContent.m_szEmail;
					}
				else
					{
						if ((0 != pEmailContent.m_wEmailLength)&&('\0' != pEmailContent.m_szEmail[0]))
							{	// 不是此用户的邮件，不能浏览
								return (char *)pEmailContent.m_szEmail;											
							}
						else
							{
								return NULL;
							}
					
					}
			default:
				break;
		}
	return NULL;
}


uint8_t app_dvt_cas_api_get_email_count(ca_get_count_t *data)
{	
	if (NULL == data)
		return 0;

	data->totalnum = pEmailCount;
	data->emptynum = pNewEmailCount;
    return pEmailCount;
}

bool app_dvt_cas_api_delete_email(ca_delete_data_t *data)
{
	uint32_t pos = 0 ;                             
	if (NULL == data)
		return FALSE;

	pos = data->pos;

	if (DVTCAERR_STB_NO_EMAIL == DVTCASTB_DelEmail(pEmail[pos].m_dwVersion))
	{
		return 0;
	}

	app_dvt_cas_api_init_email_data();

	return 1;
}

bool app_dvt_cas_api_delete_all_email(ca_delete_data_t *data)
{
	if (NULL == data)
		return FALSE;


	if (DVTCAERR_STB_NO_EMAIL == DVTCASTB_DelEmail((DWORD)-1))
	{
		return 0;
	}

	app_dvt_cas_api_init_email_data();

	return 1;
}










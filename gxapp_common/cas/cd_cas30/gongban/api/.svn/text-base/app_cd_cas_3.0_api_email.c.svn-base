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
#include "CDCASS.h"
#include "app_common_porting_stb_api.h"
#include "app_cd_cas_3.0_api_email.h"
#include "app_common_epg.h"
#include "app_cd_cas_3.0_api_pop.h"
              
CDCA_U8  pEmailCount = CDCA_MAXNUM_EMAIL;
CDCA_U8  pEmptyEmailCount = 0;
SCDCAEmailHead  pEmail[CDCA_MAXNUM_EMAIL];
SCDCAEmailContent  pEmailContent ;


static cd_cas30_email_st ca_email_flag = {0};
extern 	void app_cd_cas30_email_flag_img_show(uint8_t showFlag);
int32_t app_cd_cas30_gxmsg_ca_on_event_email(GxMsgProperty0_OnEvent* event)
{
	cd_cas30_email_st* email_flag = NULL;
	if (NULL == event)
		return 0;

	email_flag = (cd_cas30_email_st*)(event->buf);

	if (NULL == email_flag)
		return 0;

//	if ((TRUE == ca_email_flag.wShowStatus)&&(ca_email_flag.wStatus == email_flag->wStatus))
//		return 0;

	if ((0 != ca_email_flag.wStatus)&&(TRUE == ca_email_flag.wShowStatus))
	{
		/*
		* 关闭之前显示
		*/
		app_cd_cas30_email_flag_img_show(DVB_CA_EMAIL_PROMTP_HIDE);
		ca_email_flag.wStatus = 0;
		ca_email_flag.wShowStatus = 0;
		ca_email_flag.uCount = 0;
	}

	memcpy(&ca_email_flag,email_flag,sizeof(cd_cas30_email_st));


	
	return 1;
}

/*
* 反授权图标显示、隐藏
*/
int32_t app_cd_cas30_email_flag_exec()
{
	uint8_t detitle_status = ca_email_flag.wStatus;
	if ( DVB_CA_EMAIL_NEW == detitle_status)
	{	
		/* 收到新的反授权码，显示反授权码图标 */
		if (FALSE == ca_email_flag.wShowStatus)
		{
			app_cd_cas30_email_flag_img_show(DVB_CA_EMAIL_PROMTP_SHOW);
		}
		ca_email_flag.wShowStatus = TRUE;
	}
	else if ( DVB_CA_EMAIL_EXHAUST == detitle_status)
	{
		ca_email_flag.uCount++;
		ca_email_flag.uCount = ca_email_flag.uCount%3;
		
		/* 反授权码空间不足，改变图标状态提示用户 */

		if (0 != ca_email_flag.uCount)
		{						
			app_cd_cas30_email_flag_img_show(DVB_CA_EMAIL_PROMTP_SHOW);
			ca_email_flag.wShowStatus = TRUE;
		}
		else
		{
			app_cd_cas30_email_flag_img_show(DVB_CA_EMAIL_PROMTP_TRANS_HIDE);
			ca_email_flag.wShowStatus = FALSE;
		}					
	}
	else if (DVB_CA_EMAIL_FLAG_HIDE == detitle_status)
	{	
		/* 要求隐藏邮件图标 */
		if (TRUE == ca_email_flag.wShowStatus)
		{
			app_cd_cas30_email_flag_img_show(DVB_CA_EMAIL_PROMTP_HIDE);
		}
		ca_email_flag.wShowStatus = FALSE;
	}
	return 1;
	
}

uint8_t app_cd_cas30_api_init_email_data(void)
{
	uint8_t i;
	uint8_t j;
	CDCA_U8  EmailCount = CDCA_MAXNUM_EMAIL;
	CDCA_U8        pbyFromIndex =0;
	SCDCAEmailHead  pEmailTemp={0};
	memset(pEmail,0,CDCA_MAXNUM_EMAIL*sizeof(SCDCAEmailHead));
	if (CDCA_RC_OK ==  CDCASTB_GetEmailHeads(pEmail,&EmailCount,&pbyFromIndex))
	{
		for (i =0; i< EmailCount-1;i++)
		{
			for (j=i+1; j<EmailCount;j++)
			{
				if (pEmail[i].m_tCreateTime <= pEmail[j].m_tCreateTime)
				{
					memcpy(&pEmailTemp,&pEmail[i],sizeof(SCDCAEmailHead));
					memcpy(&pEmail[i],&pEmail[j],sizeof(SCDCAEmailHead));
					memcpy(&pEmail[j],&pEmailTemp,sizeof(SCDCAEmailHead));
				}
			}
		}			
		printf("app_email_GetEmailHead EmailCount=%d\n",EmailCount);
		return TRUE;
	}
	else
	{
		printf("app_email_GetEmailHead fail\n");						
		return FALSE;
	}
}

char * app_cd_cas30_api_get_email_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0;                               
	char* osd_language=app_flash_get_config_osd_language();;

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
		case CD_EMAIL_IS_NEW_ID:
			if (1 == pEmail[pos].m_bNewEmail)
			{
				if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
					return (char *)"未读";
				else
					return (char *)"Unread";		
			}
			else
			{
				if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
					return (char *)"已读";
				else
					return (char*)"Readed";				
			}				
			break;
		case CD_EMAIL_HEAD_ID:
			return (char *)pEmail[pos].m_szEmailHead;
		case CD_EMAIL_DATE_ID:
				nTime = pEmail[pos].m_tCreateTime+8*60*60;
				local = localtime((const time_t*)&nTime);
				sprintf( email_date, "%04d-%02d-%02d", 1900 + local->tm_year, 
						local->tm_mon+1, 
						local->tm_mday
						);
			return email_date;
		case CD_EMAIL_IMPORTANCE:
			if (1 == pEmail[pos].m_wImportance)
			{
				if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
					return (char *)"重要";
				else
					return (char*)"Importance";
			}
			else
			{
				if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
					return (char *)"正常";
				else
					return (char*)"Normal";
			}
		case CD_EMAIL_CONTENT:
			memset(&pEmailContent,0,sizeof(SCDCAEmailContent));
			CDCASTB_RequestMaskBuffer();
			if (CDCA_RC_OK ==  CDCASTB_GetEmailContent( pEmail[pos].m_dwActionID,
                                     &pEmailContent ))
			{
				CDCASTB_RequestUpdateBuffer();
				return (char*)pEmailContent.m_szEmail;
			}
			else
			{
				CDCASTB_RequestUpdateBuffer();
				return NULL;
			}
		default:
			break;
	}
	return NULL;
}


uint8_t app_cd_cas30_api_get_email_count(ca_get_count_t *data)
{	
	if (NULL == data)
		return 0;

	pEmailCount = CDCA_MAXNUM_EMAIL;
	pEmptyEmailCount = CDCA_MAXNUM_EMAIL;
	CDCASTB_RequestMaskBuffer();	
	if (CDCA_RC_OK == CDCASTB_GetEmailSpaceInfo(&pEmailCount, &pEmptyEmailCount))
	{	
		printf("app_email_GetEmailCount pEmailCount=%d pEmptyEmailCount=%d\n",
			pEmailCount,pEmptyEmailCount);
//			return TRUE;
	}
	else
	{
		printf("app_email_GetEmailCount fail\n");
//			return FALSE;
	}
	data->totalnum = pEmailCount;
	data->emptynum = pEmptyEmailCount;
	app_cd_cas30_api_init_email_data();
	CDCASTB_RequestUpdateBuffer();
    return pEmailCount;
}

bool app_cd_cas30_api_delete_email(ca_delete_data_t *data)
{
	uint32_t pos = 0 ;                             
	if (NULL == data)
		return FALSE;

	pos = data->pos;

	CDCASTB_RequestMaskBuffer();
	CDCASTB_DelEmail(pEmail[pos].m_dwActionID);	
	CDCASTB_RequestUpdateBuffer();       
	return 0;
}

bool app_cd_cas30_api_delete_all_email(ca_delete_data_t *data)
{
	if (NULL == data)
		return FALSE;

	CDCASTB_RequestMaskBuffer();
	CDCASTB_DelEmail(0);	
	CDCASTB_RequestUpdateBuffer();

	return 0;
}






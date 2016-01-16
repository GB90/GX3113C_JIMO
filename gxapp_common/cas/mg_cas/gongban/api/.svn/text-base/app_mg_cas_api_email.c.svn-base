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
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_stb_api.h"
#include "app_mg_cas_api_email.h"
//#include "app_mg_cas_api_pop.h"
#include "app_common_epg.h"
#include "mgdef.h"


static MG_MAIL_LIST EmailList;

static mg_cas_email_st ca_email_flag = {0};
extern void app_mg_cas_email_flag_img_show(uint8_t showFlag);
extern void app_mg_cas_email_list_update();

extern void MGCA_toEmailNotify(uint8_t byShow);
/*
void MGCA_PopEmailMsg()
{
    mg_cas_rolling_message_st  rollingmsg;
    memset(&rollingmsg,0,sizeof(mg_cas_rolling_message_st));
	rollingmsg.message_type = MG_MSG_MSG;
    sprintf((char *)rollingmsg.content,"%s","Email is full, please clean up in time");
    
	app_cas_api_on_event(DVB_CA_ROLLING_INFO,CAS_NAME, &rollingmsg, sizeof(mg_cas_rolling_message_st));						
}
*/
/*
* 邮件图标显示、隐藏,(20ms)定时器任务
*/
int32_t app_mg_cas_email_flag_exec()
{
	uint8_t detitle_status = ca_email_flag.wStatus;
	if ( DVB_CA_EMAIL_NEW == detitle_status)
	{	
		/* 收到新的反授权码，显示反授权码图标 */
		if (FALSE == ca_email_flag.wShowStatus)
		{
			app_mg_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_SHOW);
		}
		ca_email_flag.wShowStatus = TRUE;
	}
	else if ( DVB_CA_EMAIL_EXHAUST == detitle_status)
	{
		ca_email_flag.uCount++;
		ca_email_flag.uCount = ca_email_flag.uCount%3;
		
		/* 空间不足，闪烁图标状态提示用户 */

		if (0 != ca_email_flag.uCount)
		{						
			app_mg_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_SHOW);
			ca_email_flag.wShowStatus = TRUE;
		}
		else
		{
			app_mg_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_TRANS_HIDE);
			ca_email_flag.wShowStatus = FALSE;
		}					
	}
    else if (DVB_CA_EMAIL_FLAG_HIDE == detitle_status)
	{	
		/* 要求隐藏邮件图标 */
		if (TRUE == ca_email_flag.wShowStatus)
		{
			app_mg_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_HIDE);
		}
		ca_email_flag.wShowStatus = FALSE;
	}

	return 1;
	
}


//邮件图标显示
int32_t app_mg_cas_gxmsg_ca_on_event_email(GxMsgProperty0_OnEvent* event)
{
	mg_cas_email_st* email_flag = NULL;
	if (NULL == event)
		return 0;

	email_flag = (mg_cas_email_st *)(event->buf);

	if (NULL == email_flag)
		return 0;

	printf("email_flag->wStatus=%d\n",email_flag->wStatus);

	if ((email_flag->wStatus != ca_email_flag.wStatus)&&(TRUE == ca_email_flag.wShowStatus))
	{
		/*
		* 关闭之前显示
		*/
		app_mg_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_HIDE);
		ca_email_flag.wStatus = FALSE;
		ca_email_flag.wShowStatus = FALSE;
		ca_email_flag.uCount = 0;
	}

	ca_email_flag.wStatus = email_flag->wStatus;

    if(ca_email_flag.wStatus == DVB_CA_EMAIL_NEW)
    {//刷新邮件显示
        app_mg_cas_email_list_update();
    }
    
	return 1;
}

static MG_MAIL_DATA *EmailNode_CreateInit()
{
    MG_MAIL_DATA *pNode =NULL;
    if((pNode = (MG_MAIL_DATA *)GxCore_Mallocz(sizeof(MG_MAIL_DATA)))==NULL)
        return NULL;

    GX_INIT_LIST_HEAD(&pNode->ltNode);
    
    return pNode;
}
  
static MG_MAIL_DATA * EmailList_Search(int msgid)
{
    MG_MAIL_DATA *pNode = NULL;
    struct      gxlist_head *pHead = NULL;
    struct      gxlist_head *pCur = NULL;

    pHead = &(EmailList.Head);
    gxlist_for_each(pCur,pHead)
    {
        pNode = gxlist_entry(pCur,MG_MAIL_DATA,ltNode);

        if(msgid == pNode->MessageId)
        {
            return pNode;
        }
    }
    
    return NULL;
    
}


void EmailList_Info()
{
    MG_MAIL_DATA *pNode = NULL;
    struct      gxlist_head *pHead = NULL;
    struct      gxlist_head *pCur = NULL;
    
    GxCore_MutexLock (EmailList.mutex);
    pHead = &(EmailList.Head);
    gxlist_for_each(pCur,pHead)
    {
        pNode = gxlist_entry(pCur,MG_MAIL_DATA,ltNode);

        printf("Email,%s,%s\n",(char *)pNode->Title,(char *)pNode->Content);
    }
    printf("EmailCount,EmailCount:%d,NewEmailCount:%d\n",EmailList.EmailCount,EmailList.NewEmailCount);
    GxCore_MutexUnlock (EmailList.mutex);

}

int EmailList_ReadFromFile()
{//从flash初始化数据
    uint8_t *ptemp = NULL;
    MG_FLASH_DATA *pdata = NULL;
    MG_MAIL_DATA *pNode = NULL,*ptempNode = NULL;
    int ret = 1;
    uint32_t len = EMail_SIZE-4;
    int i;
    if(NULL == (ptemp = (uint8_t *)GxCore_Mallocz(len)))
    {
        printf("EmailList_ReadFromFile GxCore_Mallocz failed.\n");
        return -1;
    }
    if(!app_porting_ca_flash_read_data(0,ptemp,&len))
    {
        printf("app_porting_ca_flash_read_data failed.\n");
        ret = -1;
        goto Exit;
    }
    
    pdata = (MG_FLASH_DATA *)ptemp;
    if(EMail_MAGIC != pdata->u16Magic)
    {
        ret = -1;
        goto Exit;
    }

    //printf("sizeof(MG_FLASH_DATA)=%d,%x,%d\n",sizeof(MG_FLASH_DATA),pdata->u16Magic,pdata->u16Size);
    
    pNode = (MG_MAIL_DATA *)pdata->data;
    
    GxCore_MutexLock (EmailList.mutex);
    for(i = 0;i < pdata->u16Size; i++)
    {
        if((ptempNode = EmailNode_CreateInit()) == NULL)
        {
            GxCore_MutexUnlock (EmailList.mutex);
            ret = -1;
            goto Exit;
        }
            
        pNode = (MG_MAIL_DATA *)(pdata->data + i*sizeof(MG_MAIL_DATA));
        memcpy(ptempNode,pNode,sizeof(MG_MAIL_DATA));
        
        ptempNode->MessageId = ++ EmailList.uId;
        if(ptempNode->MessageFlag == 2)
        {
            EmailList.NewEmailCount ++;
        }
        EmailList.EmailCount ++;
        
        GX_INIT_LIST_HEAD(&ptempNode->ltNode);
        gxlist_add_tail(&ptempNode->ltNode,&EmailList.Head);
    }

    GxCore_MutexUnlock (EmailList.mutex);
Exit:
    if(ptemp)
        GxCore_Free(ptemp);
    return ret;
}

#if 0
void EmailList_ClearFile()
{
    MG_FLASH_DATA data,*pdata = NULL;
    memset(&data,0,sizeof(MG_FLASH_DATA));
    pdata = &data;
    
    pdata->u16Magic = (uint16_t)EMail_MAGIC;
    pdata->u16Size = 0;
    app_porting_ca_flash_write_data(0,(uint8_t *)pdata,sizeof(MG_FLASH_DATA));
    
}
#endif

//数据回写
int EmailList_WriteToFile()
{
    uint8_t *ptemp = NULL;
    MG_FLASH_DATA *pdata = NULL;
    MG_MAIL_DATA *pNode =NULL;
    struct      gxlist_head *pHead = NULL;
    struct      gxlist_head *pCur = NULL;

    int ret = 1;
    uint32_t len = 0;

    if(NULL == (ptemp = (uint8_t *)GxCore_Mallocz(EMail_SIZE)))
    {
        printf("EmailList_WriteToFile GxCore_Mallocz failed.\n");
        return -1;
    }
    
    memset(ptemp,0,EMail_SIZE);
    pdata = (MG_FLASH_DATA *)ptemp;
    pdata->u16Magic = (uint16_t)EMail_MAGIC;
    pdata->u16Size = 0;
    len = sizeof(MG_FLASH_DATA);
    
    GxCore_MutexLock(EmailList.mutex);
    EmailList.EmailCount = 0;
    EmailList.NewEmailCount = 0;
    
    pHead = &(EmailList.Head);
    gxlist_for_each(pCur,pHead)
    {
        pNode = gxlist_entry(pCur,MG_MAIL_DATA,ltNode);

        if(len + sizeof(MG_MAIL_DATA)> EMail_SIZE)
        {
            ret = 0;//空间不足
            break;
        }
        
        memcpy((void *)(ptemp + len),(void *)pNode,sizeof(MG_MAIL_DATA));
        
        len += sizeof(MG_MAIL_DATA);
        EmailList.EmailCount ++;
        if(pNode->MessageFlag == 2)
        {
            EmailList.NewEmailCount ++;
        }
    }

    pdata->u16Size = EmailList.EmailCount;
    if(EmailList.NewEmailCount == 0)
    {
        MGCA_toEmailNotify(MGCAS_EMAIL_NONE);
    }

    GxCore_MutexUnlock (EmailList.mutex);
    app_porting_ca_flash_write_data(0,ptemp,len);

    if(ptemp)
        GxCore_Free(ptemp);
    
    return ret;

    
}


int EmailList_Init()
{
    static int init_flag = 0;
    if(init_flag)
    {
        return 0;
    }
    
    memset(&EmailList,0,sizeof(MG_MAIL_LIST));
    if(GXCORE_SUCCESS != GxCore_MutexCreate(&EmailList.mutex))
    {
        printf("GxCore_MutexCreate failed.\n");
    }
    
    EmailList.uId = 0;
    EmailList.EmailCount = 0;
    EmailList.NewEmailCount = 0;
    GX_INIT_LIST_HEAD(&EmailList.Head);
    init_flag = 1;

    EmailList_ReadFromFile();
    
    return 1;
}

int EmailHead_Analyze(uint8_t EmailHead[50],char *cTitle,char *cSender,char *cTime,int *ContentLen)
{
    struct tm LocalTime;
    if(!cTitle || !cSender || !cTime || !ContentLen)
        return 0;
    strcpy(cTitle,(char *)&EmailHead[MG_EMAIL_TITLE_ID]);
    strcpy(cSender,(char *)&EmailHead[MG_EMAIL_SENDER_ID]);

    LocalTime.tm_year = ((EmailHead[MG_EMAIL_SENDTIME_ID]&0xF0)>>4)*1000
                        +(EmailHead[MG_EMAIL_SENDTIME_ID]&0x0F)*100
                        +((EmailHead[MG_EMAIL_SENDTIME_ID+1]&0xF0)>>4)*10
                        +(EmailHead[MG_EMAIL_SENDTIME_ID+1]&0x0F);
    LocalTime.tm_mon = ((EmailHead[MG_EMAIL_SENDTIME_ID+2]&0xF0)>>4)*10
                        +(EmailHead[MG_EMAIL_SENDTIME_ID+2]&0x0F);

    LocalTime.tm_mday = ((EmailHead[MG_EMAIL_SENDTIME_ID+3]&0xF0)>>4)*10
                        +(EmailHead[MG_EMAIL_SENDTIME_ID+3]&0x0F);

    LocalTime.tm_hour = ((EmailHead[MG_EMAIL_SENDTIME_ID+4]&0xF0)>>4)*10
                        +(EmailHead[MG_EMAIL_SENDTIME_ID+4]&0x0F);
    LocalTime.tm_min = ((EmailHead[MG_EMAIL_SENDTIME_ID+5]&0xF0)>>4)*10
                        +(EmailHead[MG_EMAIL_SENDTIME_ID+5]&0x0F);
	sprintf(cTime,"%04d-%02d-%02d %02d:%02d",
				LocalTime.tm_year,
				LocalTime.tm_mon,
				LocalTime.tm_mday,
				LocalTime.tm_hour,
				LocalTime.tm_min);
    *ContentLen = (EmailHead[MG_EMAIL_CONTENT_ID]<<8)|EmailHead[MG_EMAIL_CONTENT_ID+1];
    return 1;
}

int EmailList_Update(char *cTitle,char *cContent,char *cSender,char *cTime,int msgid)
{
    MG_MAIL_DATA *pNode = NULL;
    //struct tm LocalTime;
    GxCore_MutexLock (EmailList.mutex);
    if(msgid != 0)
    {
        if((pNode = EmailList_Search(msgid)) != NULL)
        {
            if(pNode->MessageFlag == 2)
            {
                pNode->MessageFlag = 3;
                EmailList.NewEmailCount --;
            }
            else
            {
                GxCore_MutexUnlock (EmailList.mutex);
                return 1;
            }
        }
        else
        {
            GxCore_MutexUnlock (EmailList.mutex);
            printf("EmailList_Search failed\n");
            return -1;
        }
    }
    else if(msgid == 0 && cTitle && cContent)
    {//新的邮件
        
        if(EmailList.EmailCount + 1 > EMail_MAXNUM)
        {
            GxCore_MutexUnlock (EmailList.mutex);
            printf("Warning:Email num > %d\n",EMail_MAXNUM);
            return 0;
        }

        if((pNode = EmailNode_CreateInit()) == NULL)
        {
            GxCore_MutexUnlock (EmailList.mutex);
            printf("EmailNode_CreateInit failed\n");
            return -1;
        }
        
        pNode->MessageFlag = 2;
        pNode->MessageId = ++ EmailList.uId;
        strcpy((char *)pNode->Title,cTitle);
        strcpy((char *)pNode->Content,cContent);
        strcpy((char *)pNode->Sender,cSender);
        strcpy((char *)pNode->SendTime,cTime);
        /*
        app_epg_get_local_time(&LocalTime,TRUE);
		snprintf((char *)pNode->SendTime,sizeof(pNode->SendTime)-1,
					"%04d-%02d-%02d %02d:%02d",
					LocalTime.tm_year,
					LocalTime.tm_mon,
					LocalTime.tm_mday,
					LocalTime.tm_hour,
					LocalTime.tm_min);
        */
        gxlist_add(&(pNode->ltNode),&(EmailList.Head));
        EmailList.EmailCount ++;
        EmailList.NewEmailCount ++;
    }
    GxCore_MutexUnlock (EmailList.mutex);
    //操作文件,整块读写
    EmailList_WriteToFile();

    return 1;
}

static MG_MAIL_DATA *EmailList_SearchByRow(int row)
{
    MG_MAIL_DATA *pNode = NULL;
    struct      gxlist_head *pHead = NULL;
    struct      gxlist_head *pCur = NULL;
    int n = 0;
    
    GxCore_MutexLock (EmailList.mutex);
    pHead = &(EmailList.Head);
    gxlist_for_each(pCur,pHead)
    {
        pNode = gxlist_entry(pCur,MG_MAIL_DATA,ltNode);

        if(n ++ == row)
        {
            GxCore_MutexUnlock (EmailList.mutex);
            return pNode;
        }
    }
    GxCore_MutexUnlock (EmailList.mutex);
    return NULL;
    
}

void EmailList_deletebyRow(int row)
{
    MG_MAIL_DATA *pNode = NULL;
    struct      gxlist_head *pHead = NULL;
    struct      gxlist_head *pCur = NULL;
    struct      gxlist_head *pTemp = NULL;
    int n = 0;
    
    GxCore_MutexLock (EmailList.mutex);
    pHead = &(EmailList.Head);
   
    gxlist_for_each_safe(pCur,pTemp,pHead)
    {
        if(n ++ == row)
        {
            pNode = gxlist_entry(pCur,MG_MAIL_DATA,ltNode);
            gxlist_del_init(&pNode->ltNode);
            GxCore_Free(pNode);
            break;
        }
    }
    GxCore_MutexUnlock (EmailList.mutex);

    //更新文件数据
    //回写文件数据
    EmailList_WriteToFile();
}

void EmailList_deleteAll()
{
    MG_MAIL_DATA *pNode = NULL;
    struct   gxlist_head *pHead = NULL;
    struct   gxlist_head *pCur = NULL;
    struct   gxlist_head *pTemp = NULL;
    
    GxCore_MutexLock (EmailList.mutex);
    pHead = &(EmailList.Head);
    gxlist_for_each_safe(pCur,pTemp,pHead)
    {
        pNode = gxlist_entry(pCur,MG_MAIL_DATA,ltNode);
        gxlist_del_init(&pNode->ltNode);
        GxCore_Free(pNode);
    }
    GX_INIT_LIST_HEAD(&EmailList.Head);
    EmailList.uId = 0;
    EmailList.EmailCount = 0;
    EmailList.NewEmailCount = 0;
    GxCore_MutexUnlock (EmailList.mutex);

    //删除整个数据区
    EmailList_WriteToFile();
}

//邮件数据初始化
int32_t app_mg_cas_api_init_email_data(void)
{
    //EmailList_ClearFile();
    //从flash文件初始化数据
    if(EmailList_Init())
    {
#if 0
    //for test
	char EmailHead[50];
	char EmailBody[512];
    int i;
    for(i = 0;i<5;i++)
    {
        snprintf(EmailHead,50,"head%d",i);
        snprintf(EmailBody,512,"bodey%d",i);
        EmailList_Update(EmailHead,EmailBody,"AAA","2014-01-01 00:00",0);
    }
    EmailList_Info();
#endif
    }
    
    if(EmailList.NewEmailCount>0)
    {
        MGCA_toEmailNotify(MGCAS_NEW_EAMIL);
    }
    else
    {
        MGCA_toEmailNotify(MGCAS_EMAIL_NONE);
    }
    
    return 0;
}

uint8_t app_mg_cas_api_get_email_count(ca_get_count_t *data)
{
    GxCore_MutexLock (EmailList.mutex);
	data->totalnum = EmailList.EmailCount;
	data->newnum = EmailList.NewEmailCount;
    GxCore_MutexUnlock (EmailList.mutex);
	printf("data->totalnum=%d, data->newnum=%d\n",data->totalnum,data->newnum);
    return data->totalnum;
}


char * app_mg_cas_api_get_email_data(ca_get_date_t *data)
{
    uint32_t pos = 0 ;                             
	uint32_t ID = 0; 

    MG_MAIL_DATA *pNode = NULL;
    static char   cTextBuffer_flag[10] ={ 0};

	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	switch(ID)
	{
		case MG_EMAIL_FLAG_ID:
			memset(cTextBuffer_flag,0,sizeof(cTextBuffer_flag));
            pNode = EmailList_SearchByRow(pos);
            if(pNode)
            {
                if (2 == pNode->MessageFlag)
				    strcpy(cTextBuffer_flag,"New");
			    else if (3 == pNode->MessageFlag)
				    strcpy(cTextBuffer_flag,"Readed");
            }
			
			return cTextBuffer_flag;
		case MG_EMAIL_TITLE_ID:
            pNode = EmailList_SearchByRow(pos);
            if(pNode)
            {
                return (char *)pNode->Title;
            }
            else
            {
                return NULL;
            }
		case MG_EMAIL_SENDER_ID:
            pNode = EmailList_SearchByRow(pos);
            if(pNode)
            {
                return (char *)pNode->Sender;
            }
            else
            {
                return NULL;
            }
		case MG_EMAIL_SENDTIME_ID:
            pNode = EmailList_SearchByRow(pos);
            if(pNode)
            {
                return (char *)pNode->SendTime;
            }
            else
            {
                return NULL;
            }
            
		case MG_EMAIL_CONTENT_ID://获取邮件内容，并设置为已读
            pNode = EmailList_SearchByRow(pos);
            if(pNode)
            {
                EmailList_Update(NULL,NULL,NULL,NULL,pNode->MessageId);
                return (char *)pNode->Content;
            }
            else
            {
                return NULL;
            }
		default:
			break;
	}
	return NULL;
}


bool app_mg_cas_api_delete_email(ca_delete_data_t *data)
{
	uint32_t pos = 0 ;                             
	if (NULL == data)
		return FALSE;


	pos = data->pos;
	EmailList_deletebyRow(pos) ;    
	return TRUE;
}

bool app_mg_cas_api_delete_all_email(ca_delete_data_t *data)
{
	if (NULL == data)
		return FALSE;

	EmailList_deleteAll();
	return TRUE;
}


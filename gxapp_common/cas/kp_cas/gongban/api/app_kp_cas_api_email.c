/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_kp_cas_api_email.c
* Author    : 	chenth
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.5.14		  chenth 	 			creation
*****************************************************************************/
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_stb_api.h"
#include "app_kp_cas_api_email.h"
#include "app_common_epg.h"
#include "KPAPP.h"
#include "kp_cas.h"
static MAIL_LIST EmailList;

static cas_email_st ca_email_flag = {0};

extern void app_kp_cas_email_flag_img_show(uint8_t showFlag);
extern void app_kp_cas_email_list_update();


static uint8_t kp_read_email(uint32_t Offset,uint8_t *pbyData,uint32_t *pdwLen)
{
    return app_porting_ca_flash_read_data(KPCA_FLASH_SIZE+Offset,pbyData,pdwLen);
}
static uint8_t kp_write_email(uint32_t Offset,uint8_t *pbyData,uint32_t dwLen)
{
    return app_porting_ca_flash_write_data(KPCA_FLASH_SIZE+Offset,pbyData,dwLen);
}

/*++
功能：显示新邮件的标志。
参数：
	byShow:					标志类型。含义如下：
							CAS_NEW_EAMIL						新邮件通知。
							CAS_NEW_EMAIL_NO_ROOM		有新邮件，但Email空间不够。
							CAS_EMAIL_NONE					没有新邮件。
*/
void CA_toEmailNotify(uint8_t byShow)
{
    static cas_email_st emailflag= {0};
    
	if ((CAS_EMAIL_NONE == byShow)
        &&(DVB_CA_EMAIL_FLAG_HIDE == emailflag.wStatus))
		return ;
	
	switch(byShow)
	{
		case CAS_EMAIL_NONE:
			emailflag.wStatus = DVB_CA_EMAIL_FLAG_HIDE;
			break;
		case CAS_NEW_EAMIL:
			emailflag.wStatus = DVB_CA_EMAIL_NEW;
			break;
		case CAS_NEW_EMAIL_NO_ROOM:
			emailflag.wStatus = DVB_CA_EMAIL_EXHAUST;				
			break;
		default:
			break;
	}

    app_cas_api_on_event(DVB_CA_EMAIL_INFO,CAS_NAME, &emailflag, sizeof(emailflag));
	return;
}


/*
* 邮件图标显示、隐藏,(20ms)定时器任务
*/
int32_t app_kp_cas_email_flag_exec()
{
	uint8_t detitle_status = ca_email_flag.wStatus;
	if ( DVB_CA_EMAIL_NEW == detitle_status)
	{	
		/* 收到新的反授权码，显示反授权码图标 */
		if (FALSE == ca_email_flag.wShowStatus)
		{
			app_kp_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_SHOW);
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
			app_kp_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_SHOW);
			ca_email_flag.wShowStatus = TRUE;
		}
		else
		{
			app_kp_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_TRANS_HIDE);
			ca_email_flag.wShowStatus = FALSE;
		}					
	}
    else if (DVB_CA_EMAIL_FLAG_HIDE == detitle_status)
	{	
		/* 要求隐藏邮件图标 */
		if (TRUE == ca_email_flag.wShowStatus)
		{
			app_kp_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_HIDE);
		}
		ca_email_flag.wShowStatus = FALSE;
	}

	return 1;
	
}


//邮件图标显示
int32_t app_kp_cas_gxmsg_ca_on_event_email(GxMsgProperty0_OnEvent* event)
{
	cas_email_st* email_flag = NULL;
	if (NULL == event)
		return 0;

	email_flag = (cas_email_st *)(event->buf);

	if (NULL == email_flag)
		return 0;

	printf("email_flag->wStatus=%d\n",email_flag->wStatus);

	if ((email_flag->wStatus != ca_email_flag.wStatus)&&(TRUE == ca_email_flag.wShowStatus))
	{
		/*
		* 关闭之前显示
		*/
		app_kp_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_HIDE);
		ca_email_flag.wStatus = FALSE;
		ca_email_flag.wShowStatus = FALSE;
		ca_email_flag.uCount = 0;
	}

	ca_email_flag.wStatus = email_flag->wStatus;

    if(ca_email_flag.wStatus == DVB_CA_EMAIL_NEW)
    {//刷新邮件显示
        app_kp_cas_email_list_update();
    }
    
	return 1;
}

static KP_MAIL_DATA *EmailNode_CreateInit()
{
    KP_MAIL_DATA *pNode =NULL;
    if((pNode = (KP_MAIL_DATA *)GxCore_Malloc(sizeof(KP_MAIL_DATA))) == NULL)
        return NULL;

    memset(pNode,0,sizeof(KP_MAIL_DATA));
    GX_INIT_LIST_HEAD(&pNode->ltNode);
    
    return pNode;
}
  
static KP_MAIL_DATA * EmailList_Search(int msgid)
{
    KP_MAIL_DATA *pNode = NULL;
    struct      gxlist_head *pHead = NULL;
    struct      gxlist_head *pCur = NULL;

    pHead = &(EmailList.Head);
    gxlist_for_each(pCur,pHead)
    {
        pNode = gxlist_entry(pCur,KP_MAIL_DATA,ltNode);

        if(msgid == pNode->MessageId)
        {
            return pNode;
        }
    }
    
    return NULL;
    
}


void EmailList_Info()
{
    KP_MAIL_DATA *pNode = NULL;
    struct      gxlist_head *pHead = NULL;
    struct      gxlist_head *pCur = NULL;
    
    GxCore_MutexLock (EmailList.mutex);
    pHead = &(EmailList.Head);
    gxlist_for_each(pCur,pHead)
    {
        pNode = gxlist_entry(pCur,KP_MAIL_DATA,ltNode);

        printf("Email,%s,%s\n",(char *)pNode->Title,(char *)pNode->Content);
    }
    printf("EmailCount,EmailCount:%d,NewEmailCount:%d\n",EmailList.EmailCount,EmailList.NewEmailCount);
    GxCore_MutexUnlock (EmailList.mutex);

}

int EmailList_ReadFromFile()
{//从flash初始化数据
    uint8_t *ptemp = NULL;
    MAIL_FLASH_DATA *pdata = NULL;
    KP_MAIL_DATA *pNode = NULL,*ptempNode = NULL;
    int ret = 1;
    uint32_t len = EMail_SIZE-4;
    int i;
    
    if(NULL == (ptemp = (uint8_t *)GxCore_Mallocz(len)))
    {
        printf("EmailList_ReadFromFile GxCore_Mallocz failed.\n");
        return -1;
    }
    if(!kp_read_email(0,ptemp,&len))
    {
        printf("kp_read_email failed.\n");
        ret = -1;
        goto Exit;
    }
    
    pdata = (MAIL_FLASH_DATA *)ptemp;
    if(EMail_MAGIC != pdata->u16Magic)
    {
        ret = -1;
        goto Exit;
    }

    //printf("sizeof(MG_FLASH_DATA)=%d,%x,%d\n",sizeof(MG_FLASH_DATA),pdata->u16Magic,pdata->u16Size);
    
    pNode = (KP_MAIL_DATA *)pdata->data;
    
    GxCore_MutexLock (EmailList.mutex);
    for(i = 0;i < pdata->u16Size; i++)
    {
        if((ptempNode = EmailNode_CreateInit()) == NULL)
        {
            GxCore_MutexUnlock (EmailList.mutex);
            ret = -1;
            goto Exit;
        }
            
        pNode = (KP_MAIL_DATA *)(pdata->data + i*sizeof(KP_MAIL_DATA));
        memcpy(ptempNode,pNode,sizeof(KP_MAIL_DATA));
        
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


//数据回写
int EmailList_WriteToFile()
{
    uint8_t *ptemp = NULL;
    MAIL_FLASH_DATA *pdata = NULL;
    KP_MAIL_DATA *pNode =NULL;
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
    pdata = (MAIL_FLASH_DATA *)ptemp;
    pdata->u16Magic = (uint16_t)EMail_MAGIC;
    pdata->u16Size = 0;
    len = sizeof(MAIL_FLASH_DATA);
    
    GxCore_MutexLock(EmailList.mutex);
    EmailList.EmailCount = 0;
    EmailList.NewEmailCount = 0;
    
    pHead = &(EmailList.Head);
    gxlist_for_each(pCur,pHead)
    {
        pNode = gxlist_entry(pCur,KP_MAIL_DATA,ltNode);

        if(len + sizeof(KP_MAIL_DATA)> EMail_SIZE)
        {
            ret = 0;//空间不足
            break;
        }
        
        memcpy((void *)(ptemp + len),(void *)pNode,sizeof(KP_MAIL_DATA));
        
        len += sizeof(KP_MAIL_DATA);
        EmailList.EmailCount ++;
        if(pNode->MessageFlag == 2)
        {
            EmailList.NewEmailCount ++;
        }
    }

    pdata->u16Size = EmailList.EmailCount;
    if(EmailList.NewEmailCount == 0)
    {
        CA_toEmailNotify(CAS_EMAIL_NONE);
    }

    GxCore_MutexUnlock (EmailList.mutex);
    kp_write_email(0,ptemp,len);

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
    
    memset(&EmailList,0,sizeof(MAIL_LIST));
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


int Email_Analyze(char *text,int len,char *cTitle,char *cSender,char *cContent,char *cTime)
{
    int idx  = 0,clen = 0,tlen = 0;
    struct tm LocalTime;
    if(!text || !cTitle || !cTime || !cContent)
    {
        return -1;
    }

   idx = 1;
   if(text[idx ++] == 'T')//标题
    {
        tlen = text[idx ++];//标题长度
        if(tlen > 0) 
        {
            strncpy((char *)cTitle, (char *)&text[idx], tlen);
        }
        idx  += tlen;
    }
    if(text[idx ++] == 'C')
    {
        clen = text[idx ++];
        if(clen > 0) 
        {
            strncpy((char *)cContent, (char *)&text[idx], clen);
        }
        idx  += clen;
    }
    //KP_Dbg("tlen:%d, clen:%d, len:%d\n", tlen, clen, len);
    if(idx != len || !clen || !tlen) 
    {
        return -1;
    }
    app_epg_get_local_time(&LocalTime,TRUE);
	sprintf(cTime,"%04d-%02d-%02d %02d:%02d",
				LocalTime.tm_year,
				LocalTime.tm_mon,
				LocalTime.tm_mday,
				LocalTime.tm_hour,
				LocalTime.tm_min);
    return 1;
}


int EmailList_Update(char *cTitle,char *cContent,char *cSender,char *cTime,int msgid)
{
    KP_MAIL_DATA *pNode = NULL;
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
        if(cSender)
            strcpy((char *)pNode->Sender,cSender);
        if(cTime)
            strcpy((char *)pNode->SendTime,cTime);
        gxlist_add(&(pNode->ltNode),&(EmailList.Head));
        EmailList.EmailCount ++;
        EmailList.NewEmailCount ++;
    }
    GxCore_MutexUnlock (EmailList.mutex);
    //操作文件,整块读写
    EmailList_WriteToFile();

    return 1;
}

static KP_MAIL_DATA *EmailList_SearchByRow(int row)
{
    KP_MAIL_DATA *pNode = NULL;
    struct      gxlist_head *pHead = NULL;
    struct      gxlist_head *pCur = NULL;
    int n = 0;
    
    GxCore_MutexLock (EmailList.mutex);
    pHead = &(EmailList.Head);
    gxlist_for_each(pCur,pHead)
    {
        pNode = gxlist_entry(pCur,KP_MAIL_DATA,ltNode);

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
    KP_MAIL_DATA *pNode = NULL;
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
            pNode = gxlist_entry(pCur,KP_MAIL_DATA,ltNode);
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
    KP_MAIL_DATA *pNode = NULL;
    struct   gxlist_head *pHead = NULL;
    struct   gxlist_head *pCur = NULL;
    struct   gxlist_head *pTemp = NULL;
    
    GxCore_MutexLock (EmailList.mutex);
    pHead = &(EmailList.Head);
    gxlist_for_each_safe(pCur,pTemp,pHead)
    {
        pNode = gxlist_entry(pCur,KP_MAIL_DATA,ltNode);
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
int32_t app_kp_cas_api_init_email_data(void)
{
    //从flash文件初始化数据
    if(EmailList_Init())
    {
    }
    
    if(EmailList.NewEmailCount>0)
    {
        CA_toEmailNotify(CAS_NEW_EAMIL);
    }
    else
    {
        CA_toEmailNotify(CAS_EMAIL_NONE);
    }
    
    return 0;
}

uint8_t app_kp_cas_api_get_email_count(ca_get_count_t *data)
{
    GxCore_MutexLock (EmailList.mutex);
	data->totalnum = EmailList.EmailCount;
	data->newnum = EmailList.NewEmailCount;
    GxCore_MutexUnlock (EmailList.mutex);
	printf("data->totalnum=%d, data->newnum=%d\n",data->totalnum,data->newnum);
    return data->totalnum;
}


char * app_kp_cas_api_get_email_data(ca_get_date_t *data)
{
    uint32_t pos = 0 ;                             
	uint32_t ID = 0; 

    KP_MAIL_DATA *pNode = NULL;
    static char   cTextBuffer_flag[10] ={ 0};

	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	switch(ID)
	{
		case EMAIL_FLAG_ID:
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
		case EMAIL_TITLE_ID:
            pNode = EmailList_SearchByRow(pos);
            if(pNode)
            {
                return (char *)pNode->Title;
            }
            else
            {
                return NULL;
            }
		case EMAIL_SENDER_ID:
            pNode = EmailList_SearchByRow(pos);
            if(pNode)
            {
                return (char *)pNode->Sender;
            }
            else
            {
                return NULL;
            }
		case EMAIL_SENDTIME_ID:
            pNode = EmailList_SearchByRow(pos);
            if(pNode)
            {
                return (char *)pNode->SendTime;
            }
            else
            {
                return NULL;
            }
            
		case EMAIL_CONTENT_ID://获取邮件内容，并设置为已读
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


bool app_kp_cas_api_delete_email(ca_delete_data_t *data)
{
	uint32_t pos = 0 ;                             
	if (NULL == data)
		return FALSE;


	pos = data->pos;
	EmailList_deletebyRow(pos) ;    
	return TRUE;
}

bool app_kp_cas_api_delete_all_email(ca_delete_data_t *data)
{
	if (NULL == data)
		return FALSE;

	EmailList_deleteAll();
	return TRUE;
}



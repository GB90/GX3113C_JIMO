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
#include "app_mg_cas312_api_email.h"
#include "app_common_epg.h"
#include "mg312def.h"


static MG312_MAIL_LIST s_emailList;
static mg312_cas_email_st s_caEmailFlag = {0};

extern void app_mg_cas312_email_flag_img_show(uint8_t showFlag);
extern void app_mg_cas312_email_list_update(void);
extern void MGCA312_toEmailNotify(uint8_t byShow);

/*
* 邮件图标显示、隐藏,(20ms)定时器任务
*/
int32_t app_mg_cas312_email_flag_exec(void)
{
	uint8_t detitle_status = s_caEmailFlag.wStatus;
	
	if (DVB_CA_EMAIL_NEW == detitle_status)
	{	
		/* 收到新的邮件，显示邮件图标 */
		if (FALSE == s_caEmailFlag.wShowStatus)
		{
			app_mg_cas312_email_flag_img_show(DVB_CA_EMAIL_PROMTP_SHOW);
		}
		
		s_caEmailFlag.wShowStatus = TRUE;
	}
	else if (DVB_CA_EMAIL_EXHAUST == detitle_status)
	{
		s_caEmailFlag.uCount++;
		s_caEmailFlag.uCount = s_caEmailFlag.uCount%3;
		
		/* 空间不足，闪烁图标状态提示用户 */
		if (0 != s_caEmailFlag.uCount)
		{						
			app_mg_cas312_email_flag_img_show(DVB_CA_EMAIL_PROMTP_SHOW);
			s_caEmailFlag.wShowStatus = TRUE;
		}
		else
		{
			app_mg_cas312_email_flag_img_show(DVB_CA_EMAIL_PROMTP_TRANS_HIDE);
			s_caEmailFlag.wShowStatus = FALSE;
		}					
	}
    else if (DVB_CA_EMAIL_FLAG_HIDE == detitle_status)
	{	
		/* 要求隐藏邮件图标 */
		if (TRUE == s_caEmailFlag.wShowStatus)
		{
			app_mg_cas312_email_flag_img_show(DVB_CA_EMAIL_PROMTP_HIDE);
		}
		
		s_caEmailFlag.wShowStatus = FALSE;
	}

	return 1;
}

//邮件图标显示
int32_t app_mg_cas312_gxmsg_ca_on_event_email(GxMsgProperty0_OnEvent* event)
{
	mg312_cas_email_st* email_flag = NULL;
	if (NULL == event)
	{
		return 0;
	}

	email_flag = (mg312_cas_email_st *)(event->buf);
	if (NULL == email_flag)
	{
		return 0;
	}

	//printf("[app_mg_cas312_gxmsg_ca_on_event_email]email_flag->wStatus=%d.\n", email_flag->wStatus);
	if ((email_flag->wStatus != s_caEmailFlag.wStatus) 
		&& (TRUE == s_caEmailFlag.wShowStatus))
	{
		app_mg_cas312_email_flag_img_show(DVB_CA_EMAIL_PROMTP_HIDE);
		s_caEmailFlag.wStatus = FALSE;
		s_caEmailFlag.wShowStatus = FALSE;
		s_caEmailFlag.uCount = 0;
	}

	s_caEmailFlag.wStatus = email_flag->wStatus;

    if (s_caEmailFlag.wStatus == DVB_CA_EMAIL_NEW)
    {//刷新邮件显示
        app_mg_cas312_email_list_update();
    }
    
	return 1;
}

static MG312_MAIL_DATA* emailNode_CreateInit(void)
{
    MG312_MAIL_DATA *pNode = NULL;
	
    if ((pNode = (MG312_MAIL_DATA *)GxCore_Mallocz(sizeof(MG312_MAIL_DATA))) == NULL)
    {
    	return NULL;
	}

    GX_INIT_LIST_HEAD(&pNode->ltNode);
    return pNode;
}
  
static MG312_MAIL_DATA* emailList_Search(int msgid)
{
    MG312_MAIL_DATA *pNode = NULL;
    struct gxlist_head *pHead = NULL;
    struct gxlist_head *pCur = NULL;

    pHead = &(s_emailList.Head);
    gxlist_for_each(pCur, pHead)
    {
        pNode = gxlist_entry(pCur, MG312_MAIL_DATA, ltNode);
        if (msgid == pNode->MessageId)
        {
            return pNode;
        }
    }
    
    return NULL;
}

#if 0
void EmailList_Info(void)
{
    MG312_MAIL_DATA *pNode = NULL;
    struct      gxlist_head *pHead = NULL;
    struct      gxlist_head *pCur = NULL;
    
    GxCore_MutexLock (s_emailList.mutex);
    pHead = &(s_emailList.Head);
    gxlist_for_each(pCur,pHead)
    {
        pNode = gxlist_entry(pCur,MG312_MAIL_DATA,ltNode);

        printf("Email,%s,%s\n",(char *)pNode->Title,(char *)pNode->Content);
    }
    printf("EmailCount,EmailCount:%d,NewEmailCount:%d\n",s_emailList.EmailCount,s_emailList.NewEmailCount);
    GxCore_MutexUnlock(s_emailList.mutex);
}
#endif

static int emailList_ReadFromFile(void)
{
    uint8_t *ptemp = NULL;
    MG312_FLASH_DATA *pdata = NULL;
    MG312_MAIL_DATA *pNode = NULL, *ptempNode = NULL;
    int ret = 1;
    uint32_t len = EMail_SIZE-4;
    int i;
	
    if (NULL == (ptemp = (uint8_t *)GxCore_Mallocz(len)))
    {
        printf("[emailList_ReadFromFile]GxCore_Mallocz failed!!!\n");
        return -1;
    }
	
    if (!app_porting_ca_flash_read_data(0, ptemp, &len))
    {
        printf("[emailList_ReadFromFile]app_porting_ca_flash_read_data failed!!!\n");
        ret = -1;
        goto Exit;
    }
    
    pdata = (MG312_FLASH_DATA *)ptemp;
    if(EMail_MAGIC != pdata->u16Magic)
    {
        ret = -1;
        goto Exit;
    }

    //printf("sizeof(MG_FLASH_DATA)=%d,%x,%d\n",sizeof(MG_FLASH_DATA),pdata->u16Magic,pdata->u16Size);
    
    pNode = (MG312_MAIL_DATA *)pdata->data;
    
    GxCore_MutexLock(s_emailList.mutex);
	
    for (i = 0; i < pdata->u16Size; i++)
    {
        if ((ptempNode = emailNode_CreateInit()) == NULL)
        {
            GxCore_MutexUnlock(s_emailList.mutex);
            ret = -1;
            goto Exit;
        }
            
        pNode = (MG312_MAIL_DATA *)(pdata->data + i*sizeof(MG312_MAIL_DATA));
        memcpy(ptempNode, pNode, sizeof(MG312_MAIL_DATA));
        
        ptempNode->MessageId = ++s_emailList.uId;
        if (ptempNode->MessageFlag == 2)
        {
            s_emailList.NewEmailCount++;
        }
		
        s_emailList.EmailCount++;
        GX_INIT_LIST_HEAD(&ptempNode->ltNode);
        gxlist_add_tail(&ptempNode->ltNode, &s_emailList.Head);
    }

    GxCore_MutexUnlock(s_emailList.mutex);
	
Exit:
    if (ptemp)
    {
    	GxCore_Free(ptemp);
	}
	
    return ret;
}

#if 0
void EmailList_ClearFile()
{
    MG312_FLASH_DATA data, *pdata = NULL;
	
    memset(&data, 0, sizeof(MG312_FLASH_DATA));
    pdata = &data;
    
    pdata->u16Magic = (uint16_t)EMail_MAGIC;
    pdata->u16Size = 0;
    app_porting_ca_flash_write_data(0, (uint8_t *)pdata, sizeof(MG312_FLASH_DATA));
}
#endif

//数据回写
static int emailList_WriteToFile(void)
{
    uint8_t *ptemp = NULL;
    MG312_FLASH_DATA *pdata = NULL;
    MG312_MAIL_DATA *pNode = NULL;
    struct gxlist_head *pHead = NULL;
    struct gxlist_head *pCur = NULL;

    int ret = 1;
    uint32_t len = 0;

    if (NULL == (ptemp = (uint8_t *)GxCore_Mallocz(EMail_SIZE)))
    {
        printf("[emailList_WriteToFile]GxCore_Mallocz failed!!!\n");
        return -1;
    }
    
    memset(ptemp, 0, EMail_SIZE);
    pdata = (MG312_FLASH_DATA *)ptemp;
    pdata->u16Magic = (uint16_t)EMail_MAGIC;
    pdata->u16Size = 0;
    len = sizeof(MG312_FLASH_DATA);
    
    GxCore_MutexLock(s_emailList.mutex);
    s_emailList.EmailCount = 0;
    s_emailList.NewEmailCount = 0;
    
    pHead = &(s_emailList.Head);
    gxlist_for_each(pCur, pHead)
    {
        pNode = gxlist_entry(pCur, MG312_MAIL_DATA, ltNode);

        if (len + sizeof(MG312_MAIL_DATA) > EMail_SIZE)
        {
            ret = 0;//空间不足
            break;
        }
        
        memcpy((void *)(ptemp + len), (void *)pNode, sizeof(MG312_MAIL_DATA));
        
        len += sizeof(MG312_MAIL_DATA);
        s_emailList.EmailCount++;
        if (pNode->MessageFlag == 2)
        {
            s_emailList.NewEmailCount++;
        }
    }

    pdata->u16Size = s_emailList.EmailCount;
    if (s_emailList.NewEmailCount == 0)
    {
        MGCA312_toEmailNotify(MGCAS312_EMAIL_NONE);
    }

    GxCore_MutexUnlock(s_emailList.mutex);
    app_porting_ca_flash_write_data(0, ptemp, len);

    if (ptemp)
    {
    	GxCore_Free(ptemp);
	}
    
    return ret;
}

static int emailList_Init(void)
{
    static int init_flag = 0;
	
    if (1 == init_flag)
    {
    	printf("[emailList_Init]init_flag = 1!!!\n");
        return 0;
    }
    
    memset(&s_emailList, 0, sizeof(MG312_MAIL_LIST));
    if (GXCORE_SUCCESS != GxCore_MutexCreate(&s_emailList.mutex))
    {
        printf("[emailList_Init]GxCore_MutexCreate failed!!!\n");
    }
    
    s_emailList.uId = 0;
    s_emailList.EmailCount = 0;
    s_emailList.NewEmailCount = 0;
    GX_INIT_LIST_HEAD(&s_emailList.Head);
    init_flag = 1;

    emailList_ReadFromFile();
    
    return 1;
}

int MG_CAS312_EmailHead_Analyze(uint8_t EmailHead[50], char *cTitle, 
	    							char *cSender, char *cTime, int *ContentLen)
{
    struct tm LocalTime;
	
    if ((!cTitle) || (!cSender) || (!cTime) || (!ContentLen))
    {
    	printf("[MG_CAS312_EmailHead_Analyze]param is NULL!!!\n");
    	return 0;
	}
	
    strcpy(cTitle, (char *)&EmailHead[MG312_EMAIL_TITLE_ID]);
    strcpy(cSender, (char *)&EmailHead[MG312_EMAIL_SENDER_ID]);

    LocalTime.tm_year = ((EmailHead[MG312_EMAIL_SENDTIME_ID]&0xF0)>>4)*1000
                        +(EmailHead[MG312_EMAIL_SENDTIME_ID]&0x0F)*100
                        +((EmailHead[MG312_EMAIL_SENDTIME_ID+1]&0xF0)>>4)*10
                        +(EmailHead[MG312_EMAIL_SENDTIME_ID+1]&0x0F);
    LocalTime.tm_mon = ((EmailHead[MG312_EMAIL_SENDTIME_ID+2]&0xF0)>>4)*10
                        +(EmailHead[MG312_EMAIL_SENDTIME_ID+2]&0x0F);

    LocalTime.tm_mday = ((EmailHead[MG312_EMAIL_SENDTIME_ID+3]&0xF0)>>4)*10
                        +(EmailHead[MG312_EMAIL_SENDTIME_ID+3]&0x0F);

    LocalTime.tm_hour = ((EmailHead[MG312_EMAIL_SENDTIME_ID+4]&0xF0)>>4)*10
                        +(EmailHead[MG312_EMAIL_SENDTIME_ID+4]&0x0F);
    LocalTime.tm_min = ((EmailHead[MG312_EMAIL_SENDTIME_ID+5]&0xF0)>>4)*10
                        +(EmailHead[MG312_EMAIL_SENDTIME_ID+5]&0x0F);
	sprintf(cTime,"%04d-%02d-%02d %02d:%02d", LocalTime.tm_year, LocalTime.tm_mon,
			 LocalTime.tm_mday, LocalTime.tm_hour, LocalTime.tm_min);
    *ContentLen = (EmailHead[MG312_EMAIL_CONTENT_ID]<<8)|EmailHead[MG312_EMAIL_CONTENT_ID+1];
    return 1;
}

int MG_CAS312_EmailList_Update(char *cTitle, char *cContent, 
	   							char *cSender, char *cTime, int msgid)
{
    MG312_MAIL_DATA *pNode = NULL;

    GxCore_MutexLock(s_emailList.mutex);
    if (msgid != 0)
    {
        if ((pNode = emailList_Search(msgid)) != NULL)
        {
            if (pNode->MessageFlag == 2)
            {
                pNode->MessageFlag = 3;
                s_emailList.NewEmailCount--;
            }
            else
            {
                GxCore_MutexUnlock(s_emailList.mutex);
                return 1;
            }
        }
        else
        {
            GxCore_MutexUnlock(s_emailList.mutex);
            printf("[MG_CAS312_EmailList_Update] failed!!!\n");
            return -1;
        }
    }
    else if ((msgid == 0) && cTitle && cContent)
    {//新的邮件
        if ((s_emailList.EmailCount + 1) > EMail_MAXNUM)
        {
            GxCore_MutexUnlock(s_emailList.mutex);
            printf("Warning:Email num > %d.###\n", EMail_MAXNUM);
            return 0;
        }

        if ((pNode = emailNode_CreateInit()) == NULL)
        {
            GxCore_MutexUnlock(s_emailList.mutex);
            printf("emailNode_CreateInit failed!!!\n");
            return -1;
        }
        
        pNode->MessageFlag = 2;
        pNode->MessageId = ++s_emailList.uId;
        strcpy((char *)pNode->Title, cTitle);
        strcpy((char *)pNode->Content, cContent);
        strcpy((char *)pNode->Sender, cSender);
        strcpy((char *)pNode->SendTime, cTime);
        gxlist_add(&(pNode->ltNode), &(s_emailList.Head));
        s_emailList.EmailCount++;
        s_emailList.NewEmailCount++;
    }
	
    GxCore_MutexUnlock(s_emailList.mutex);
	
    //操作文件,整块读写
    emailList_WriteToFile();
    return 1;
}

static MG312_MAIL_DATA* emailList_SearchByRow(int row)
{
    MG312_MAIL_DATA *pNode = NULL;
    struct gxlist_head *pHead = NULL;
    struct gxlist_head *pCur = NULL;
    int n = 0;
    
    GxCore_MutexLock(s_emailList.mutex);
    pHead = &(s_emailList.Head);
    gxlist_for_each(pCur, pHead)
    {
        pNode = gxlist_entry(pCur, MG312_MAIL_DATA, ltNode);

        if ((n++) == row)
        {
            GxCore_MutexUnlock(s_emailList.mutex);
            return pNode;
        }
    }
	
    GxCore_MutexUnlock(s_emailList.mutex);
    return NULL;
}

static void emailList_deletebyRow(int row)
{
    MG312_MAIL_DATA *pNode = NULL;
    struct gxlist_head *pHead = NULL;
    struct gxlist_head *pCur = NULL;
    struct gxlist_head *pTemp = NULL;
    int n = 0;
    
    GxCore_MutexLock(s_emailList.mutex);
    pHead = &(s_emailList.Head);
    gxlist_for_each_safe(pCur, pTemp, pHead)
    {
        if ((n++) == row)
        {
            pNode = gxlist_entry(pCur, MG312_MAIL_DATA, ltNode);
            gxlist_del_init(&pNode->ltNode);
            GxCore_Free(pNode);
            break;
        }
    }
    GxCore_MutexUnlock(s_emailList.mutex);

    //更新文件数据
    //回写文件数据
    emailList_WriteToFile();
}

static void emailList_deleteAll(void)
{
    MG312_MAIL_DATA *pNode = NULL;
    struct gxlist_head *pHead = NULL;
    struct gxlist_head *pCur = NULL;
    struct gxlist_head *pTemp = NULL;
    
    GxCore_MutexLock(s_emailList.mutex);
    pHead = &(s_emailList.Head);
    gxlist_for_each_safe(pCur, pTemp, pHead)
    {
        pNode = gxlist_entry(pCur, MG312_MAIL_DATA, ltNode);
        gxlist_del_init(&pNode->ltNode);
        GxCore_Free(pNode);
    }
    GX_INIT_LIST_HEAD(&s_emailList.Head);
    s_emailList.uId = 0;
    s_emailList.EmailCount = 0;
    s_emailList.NewEmailCount = 0;
    GxCore_MutexUnlock(s_emailList.mutex);

    //删除整个数据区
    emailList_WriteToFile();
}

//邮件数据初始化
int32_t app_mg_cas312_api_init_email_data(void)
{
    if (emailList_Init())
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
	        emailList_Update(EmailHead,EmailBody,"AAA","2014-01-01 00:00",0);
	    }
		
	    emailList_Info();
#endif
    }
    
    if (s_emailList.NewEmailCount > 0)
    {
        MGCA312_toEmailNotify(MGCAS312_NEW_EAMIL);
    }
    else
    {
        MGCA312_toEmailNotify(MGCAS312_EMAIL_NONE);
    }
    
    return 0;
}

uint8_t app_mg_cas312_api_get_email_count(ca_get_count_t *data)
{
    GxCore_MutexLock(s_emailList.mutex);
	data->totalnum = s_emailList.EmailCount;
	data->newnum = s_emailList.NewEmailCount;
    GxCore_MutexUnlock(s_emailList.mutex);
	
	//printf("[app_mg_cas312_api_get_email_count]data->totalnum(%d), data->newnum(%d).\n",
		 	//data->totalnum, data->newnum);
    return data->totalnum;
}

char* app_mg_cas312_api_get_email_data(ca_get_date_t *data)
{
    uint32_t pos = 0;                             
	uint32_t ID = 0; 
    MG312_MAIL_DATA *pNode = NULL;
    static char cTextBuffer_flag[10] = {0};

	if (NULL == data)
	{
		return NULL;
	}

	pos = data->pos;
	ID = data->ID;
	switch (ID)
	{
		case MG312_EMAIL_FLAG_ID:
			memset(cTextBuffer_flag, 0, sizeof(cTextBuffer_flag));
            pNode = emailList_SearchByRow(pos);
            if (pNode)
            {
                if (2 == pNode->MessageFlag)
				{
					strcpy(cTextBuffer_flag, "New");
				}
			    else if (3 == pNode->MessageFlag)
				{
					strcpy(cTextBuffer_flag, "Readed");
				}
            }
			
			return cTextBuffer_flag;
			
		case MG312_EMAIL_TITLE_ID:
            pNode = emailList_SearchByRow(pos);
            if (pNode)
            {
                return (char *)pNode->Title;
            }
            else
            {
                return NULL;
            }
			
		case MG312_EMAIL_SENDER_ID:
            pNode = emailList_SearchByRow(pos);
            if (pNode)
            {
                return (char *)pNode->Sender;
            }
            else
            {
                return NULL;
            }
			
		case MG312_EMAIL_SENDTIME_ID:
            pNode = emailList_SearchByRow(pos);
            if (pNode)
            {
                return (char *)pNode->SendTime;
            }
            else
            {
                return NULL;
            }
            
		case MG312_EMAIL_CONTENT_ID://获取邮件内容，并设置为已读
            pNode = emailList_SearchByRow(pos);
            if (pNode)
            {
                MG_CAS312_EmailList_Update(NULL, NULL, NULL,\
					   					NULL, pNode->MessageId);
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

bool app_mg_cas312_api_delete_email(ca_delete_data_t *data)
{
	uint32_t pos = 0;    
	
	if (NULL == data)
	{
		return FALSE;
	}

	pos = data->pos;
	emailList_deletebyRow(pos);
	
	return TRUE;
}

bool app_mg_cas312_api_delete_all_email(ca_delete_data_t *data)
{
	if (NULL == data)
	{
		return FALSE;
	}

	emailList_deleteAll();
	return TRUE;
}



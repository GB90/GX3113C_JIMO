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
#include "gyca_porting.h"
#include "app_common_porting_stb_api.h"
#include "app_gy_cas_api_email.h"
#include "app_common_epg.h"
#include "app_gy_cas_api_pop.h"
#include "gyca_interface.h"


/******************************************************
金亚CA邮件操作相关实现 
*******************************************************/
/* ****************************************************
金亚CA的邮件需要由应用程序进行管理
===========================================
******************************************************/

#define GYCA_FLASH_EMAIL_COUNT      (50) // 最大50封邮件
#define GYCA_FLASH_EMAIL_START     (FLASH_GY_CAS_ADDR + FLASH_GY_CAS_SIZE-15*1024)//邮件在NVRAM中起始地址
#define GYCA_FLASH_EMAIL_SIZE     (GYCA_FLASH_EMAIL_COUNT*sizeof(GxEmail_t))//邮件的FLASH空间大小  

static GYU8  pEmailCount = GYCA_FLASH_EMAIL_COUNT;
static GYU8  pNewEmailCount = 0;
static GYU8 pEmptyEmailCount = 0;
GxEmail_t  pEmail[GYCA_FLASH_EMAIL_COUNT];

static gy_cas_email_st ca_email_flag = {0};



/*****************************************************/

// 判断邮件是否有效. 1有效, 0无效
GYS32 GYCA_IsEmailValid(GxEmail_t *pEmail)
{
	//printf("%s\n", __FUNCTION__); 
    if (pEmail->id > GYCA_FLASH_EMAIL_COUNT)
        return 0; 

    // 已读标志即不是0也不是1
    if ((pEmail->readFlag != 0) && (pEmail->readFlag != 1))
        return 0; 

    // 内容无效 
    if ((pEmail->email.m_szEmail_Content[0] == 0x00) && (pEmail->email.m_szEmail_Content[1] == 0x00))
        return 0; 
            
    if ((pEmail->email.m_szEmail_Content[0] == 0xFF) && (pEmail->email.m_szEmail_Content[1] == 0xFF))
        return 0; 

    return 1; 
} 
// 判断邮件是否已经存在. 方法是对标题和内容进行比较. 
GYS32 GYCA_IsEmailExisted(GxEmail_t *pEmail)
{
    GYS32 i = 0; 
    GYS32 addr = 0; 
    GxEmail_t mail = {0}; 
	 GYS32 nLen = 0; 
    
    printf("%s, content: %s\n", __FUNCTION__, pEmail->email.m_szEmail_Content); 
    for (i=0; i<GYCA_FLASH_EMAIL_COUNT; i++)
    {
        addr = GYCA_FLASH_EMAIL_START + i*sizeof(GxEmail_t); 
		  nLen = sizeof(GxEmail_t); 
        if (0 == GYCA_ReadBuffer(addr, (GYU8 *)&mail, &nLen))
        {
            if (1== GYCA_IsEmailValid(&mail))
            {
                if ((strcmp((char *)mail.email.m_szEmail_Content, (char *)pEmail->email.m_szEmail_Content) == 0)
                    && (strcmp((char *)mail.email.m_szEmail_Title, (char *)pEmail->email.m_szEmail_Title) == 0))
                {
                    printf("%s, existed content: %s\n", __FUNCTION__, pEmail->email.m_szEmail_Content); 
                    return 1; 
                }
            }
         }
     }
	  printf("not existed.\n"); 

     return 0; 
} 

GYS32 GYCA_EmailWrite(GxEmail_t *pEmail)
{
    GYS32 i = 0; 
    GYS32 ret = -1; 
    GYU32 addr = 0; 
    GxEmail_t mail; 
    GYS32 nLen = 0; 

    printf("%s, content: %s\n", __FUNCTION__, pEmail->email.m_szEmail_Content); 

    if (0 != GYCA_IsEmailExisted(pEmail))
    {
            printf("%s, email existed.\n", __FUNCTION__); 
            return 0; 
    }
    
    for (i=0; i<GYCA_FLASH_EMAIL_COUNT; i++)
	{
        addr = GYCA_FLASH_EMAIL_START + i*sizeof(GxEmail_t); 
			nLen = sizeof(GxEmail_t); 
        if (0 == GYCA_ReadBuffer(addr, (GYU8 *)&mail, &nLen))
        {
            // 空位置. 判定条件是邮件content为空. 
            if (0== GYCA_IsEmailValid(&mail))
            {
                // 保存的位置写入FLASH , 方便在顺序混乱的情况下进行
                // 查询和删除操作 
                pEmail->id = i; 
                ret = GYCA_WriteBuffer(addr, (GYU8 *)pEmail, sizeof(GxEmail_t)); 
                if (0 == ret)
                    printf("%s, write email ok. pos=%d. \n", __FUNCTION__, i); 
                else
                    printf("%s, write email failed. pos=%d, ret=%d. \n", __FUNCTION__, i, ret); 
                return 1; 
            }
        }
	}

    // 邮箱已满
    printf("%s, write email failed. full. \n", __FUNCTION__); 
    
    return -1; 
}

GYS32 GYCA_EmailDel(GYU8 email_id)
{
    GYS32 ret = -1; 
    GYU32 addr = 0; 
    GxEmail_t mail; 

    printf("%s, content: %d\n", __FUNCTION__, email_id); 
    if (email_id > GYCA_FLASH_EMAIL_COUNT)
    {
        printf("email id error: %d\n", email_id); 
        return -3; 
    }
    
    addr = GYCA_FLASH_EMAIL_START + email_id*sizeof(GxEmail_t); 
    memset(&mail, 0x00, sizeof(GxEmail_t));  
    ret = GYCA_WriteBuffer(addr, (GYU8 *)&mail, sizeof(GxEmail_t)); 

    if (0 == ret)
        printf("%s, write email ok. \n", __FUNCTION__); 
    else
        printf("%s, write email failed. ret=%d. \n", __FUNCTION__, ret);

    return ret; 
}


GYS32 GYCA_EmailRead(GYU8 email_id, GxEmail_t *pEmail)
{
    GYU32 addr = 0; 
    GxEmail_t mail; 
	 GYS32 nLen = 0; 

    //printf("%s, id: %d\n", __FUNCTION__, email_id); 
    if (email_id > GYCA_FLASH_EMAIL_COUNT)
    {
        printf("email id error: %d\n", email_id); 
        return -3; 
    }
    
    addr = GYCA_FLASH_EMAIL_START + email_id*sizeof(GxEmail_t); 
			nLen = sizeof(GxEmail_t); 
    if (0 == GYCA_ReadBuffer(addr, (GYU8 *)&mail, &nLen))
    {
        // 判定邮件ID. 如果匹配就将FLASH内容写为空 
        memcpy(pEmail, &mail, sizeof(GxEmail_t)); 
        return 0; 
    }
    
    return -1; 
}

GYS32 GYCA_EmailSetReadFlag(GYU8 email_id)
{
    GYS32 ret = -1; 
    GYU32 addr = 0; 
    GxEmail_t mail; 
	 GYS32 nLen = 0; 

    printf("%s, content: %d\n", __FUNCTION__, email_id); 
    if (email_id > GYCA_FLASH_EMAIL_COUNT)
    {
        printf("email id error: %d\n", email_id); 
        return -3; 
    }
    
    addr = GYCA_FLASH_EMAIL_START + email_id*sizeof(GxEmail_t); 
			nLen = sizeof(GxEmail_t); 
    if (0 == GYCA_ReadBuffer(addr, (GYU8 *)&mail, &nLen))
    {
        mail.readFlag = 1; 
        ret =  GYCA_WriteBuffer(addr, (GYU8 *)&mail, sizeof(GxEmail_t)); 

        if (0 == ret)
                printf("%s, write email ok.\n", __FUNCTION__); 
        else
                printf("%s, write email failed. ret=%d. \n", __FUNCTION__, ret);

        return ret; 
    }
    
    return -1; 
}

GYS32 mjd_compare(GYU8 *mjd1, GYU8 *mjd2)
{
	GYU32 n1 = (mjd1[0]<<8 | mjd1[1])*10000 + mjd1[2]*24+ mjd1[3]*24+ mjd1[4]*24; 
	GYU32 n2 = (mjd2[0]<<8 | mjd2[1])*10000 + mjd2[2]*24+ mjd2[3]*24+ mjd2[4]*24; 

	if (n1 > n2)
		return 1; 
	else if (n1 < n2)
		return -1; 
	else
		return 0; 
}

extern 	void app_gy_cas_email_flag_img_show(uint8_t showFlag);

void app_gy_cas_api_add_email(T_CAMailInfo *pMailInfo)
{	
	GxEmail_t mail={0}; 

	
	printf("%s\n", __FUNCTION__); 
	mail.id = 0; 
	mail.readFlag = 0; 
	
	memcpy(&mail.email, pMailInfo, sizeof(T_CAMailInfo)); 

	if (1==GYCA_EmailWrite(&mail))
	{
		app_gy_cas_api_init_email_data(); 
	}
}
int32_t app_gy_cas_gxmsg_ca_on_event_email(GxMsgProperty0_OnEvent* event)
{
	gy_cas_email_st* email_data = NULL;
	if (NULL == event)
		return 0;

	email_data = (gy_cas_email_st*)(event->buf);

	if (NULL == email_data)
		return 0;

	if ((0 != ca_email_flag.wStatus)&&(TRUE == ca_email_flag.wShowStatus))
	{
		app_gy_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_HIDE);
		ca_email_flag.wStatus = 0;
		ca_email_flag.wShowStatus = 0;
		ca_email_flag.uCount = 0;
	}

	memcpy(&ca_email_flag,email_data,sizeof(gy_cas_email_st));

	return 1;
}


/*
* 反授权图标显示、隐藏
*/
int32_t app_gy_cas_email_flag_exec()
{
	uint8_t email_status = ca_email_flag.wStatus;
	if ( DVB_CA_EMAIL_NEW == email_status)
	{	
		if (FALSE == ca_email_flag.wShowStatus)
		{
			app_gy_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_SHOW);
		}
		ca_email_flag.wShowStatus = TRUE;
	}
	else if ( DVB_CA_EMAIL_EXHAUST == email_status)
	{
		ca_email_flag.uCount++;
		ca_email_flag.uCount = ca_email_flag.uCount%3;

		if (0 != ca_email_flag.uCount)
		{						
			app_gy_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_SHOW);
			ca_email_flag.wShowStatus = TRUE;
		}
		else
		{
			app_gy_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_HIDE);
			ca_email_flag.wShowStatus = FALSE;
		}					
	}
	else if (DVB_CA_EMAIL_FLAG_HIDE == email_status)
	{	
		/* 要求隐藏邮件图标 */
		if (TRUE == ca_email_flag.wShowStatus)
		{
			app_gy_cas_email_flag_img_show(DVB_CA_EMAIL_PROMTP_HIDE);
		}
		ca_email_flag.wShowStatus = FALSE;
	}
	return 1;
	
}

uint8_t app_gy_cas_api_init_email_data(void)
{
	GxEmail_t email; 
	int i, j; 
    static gy_cas_email_st emailflag= {0};
				
	printf("%s\n", __FUNCTION__); 

	memset(pEmail,0,GYCA_FLASH_EMAIL_COUNT*sizeof(GxEmail_t));
	pEmailCount = 0;
	pNewEmailCount = 0;	
	pEmptyEmailCount = GYCA_FLASH_EMAIL_COUNT; 

    for (i=0; i<GYCA_FLASH_EMAIL_COUNT; i++)
    {
        if (0 != GYCA_EmailRead(i, &email))
            continue; 

        if (0== GYCA_IsEmailValid(&email)) 
            continue; 

        if (email.readFlag == 0)
            pNewEmailCount++; 
                        
        memcpy(&pEmail[pEmailCount], &email, sizeof(GxEmail_t));
        pEmailCount++; 

        printf("%s, %s, %s\n", __FUNCTION__, email.email.m_szEmail_Title, email.email.m_szEmail_Content); 
    } 

	//排序 
	for (i =0; i< pEmailCount-1;i++)
	{
		for (j=i+1; j<pEmailCount;j++)
		{
			// 这里只比较了日期 
			if (mjd_compare(pEmail[i].email.m_byEmail_Create_Time, pEmail[j].email.m_byEmail_Create_Time) < 0)
			{
				memcpy(&email,&pEmail[i],sizeof(GxEmail_t));
				memcpy(&pEmail[i],&pEmail[j],sizeof(GxEmail_t));
				memcpy(&pEmail[j],&email,sizeof(GxEmail_t));
			}
		}
	}			


	pEmptyEmailCount = GYCA_FLASH_EMAIL_COUNT - pEmailCount;
	
    printf("app_email_GetEmailCount pEmailCount=%d pNewEmailCount=%d\n", pEmailCount,pNewEmailCount);


	if(pNewEmailCount == 0)
	{
		emailflag.wStatus = DVB_CA_EMAIL_FLAG_HIDE;
	}
	else if(pNewEmailCount != 0)
	{
		emailflag.wStatus = DVB_CA_EMAIL_NEW;
	}
	if(pEmptyEmailCount <= 0)
	{
		emailflag.wStatus = DVB_CA_EMAIL_EXHAUST;	
	}

    app_cas_api_on_event(DVB_CA_EMAIL_INFO,CAS_NAME, &emailflag, sizeof(gy_cas_email_st));

	return TRUE; 
}

char * app_gy_cas_api_get_email_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0;         
    static gy_cas_email_st emailflag= {0};

	printf("%s,%d\n", __FUNCTION__, data->pos); 

    static char   email_date[40] ={0};

	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	switch(ID)
	{
		case GY_EMAIL_IS_NEW_ID:
			if (0 == pEmail[pos].readFlag)
			{
				return (char*)"New";		
			}
			else
			{
				return (char*)"Readed";				
			}				
			break;
		case GY_EMAIL_HEAD_ID:
			return (char *)pEmail[pos].email.m_szEmail_Title;
		case GY_EMAIL_DATE_ID:
		{
			GYU16 year; 
			GYU8 month, day, week_day; 
			GYU16 mjd; 

			mjd = pEmail[pos].email.m_byEmail_Create_Time[0] << 8 | pEmail[pos].email.m_byEmail_Create_Time[1]; 
			app_epg_convert_mjd_to_ymd(mjd, &year, &month, &day, &week_day); 

			sprintf( email_date, "%04d-%02d-%02d %02X:%02X", year, month, day
				, pEmail[pos].email.m_byEmail_Create_Time[2]
				, pEmail[pos].email.m_byEmail_Create_Time[3]);
		}
			return email_date;
		case GY_EMAIL_IMPORTANCE:
		if (1 == pEmail[pos].email.m_byEmail_Level)
		{
			return (char*)"Importance";
		}
		else
		{
			return (char*)"Normal";
		}
		case GY_EMAIL_CONTENT: 
			pEmail[pos].readFlag = 1;
			pNewEmailCount--;
			GYCA_EmailSetReadFlag(pEmail[pos].id); 
			if(pNewEmailCount == 0 && pEmptyEmailCount > 0)
			{
				emailflag.wStatus = DVB_CA_EMAIL_FLAG_HIDE;
				app_cas_api_on_event(DVB_CA_EMAIL_INFO,CAS_NAME, &emailflag, sizeof(gy_cas_email_st));
			}
			return (char *)pEmail[pos].email.m_szEmail_Content;
		default:
			break;
	}

	return NULL;
}


uint8_t app_gy_cas_api_get_email_count(ca_get_count_t *data)
{	
	if (NULL == data)
		return 0;

	printf("%s\n", __FUNCTION__); 
	
	//app_gy_cas_api_init_email_data(); 

	data->totalnum = pEmailCount;
	data->emptynum = pEmptyEmailCount;

	return pEmailCount;
}

bool app_gy_cas_api_delete_email(ca_delete_data_t *data)
{
	if (NULL == data)
		return FALSE;

	printf("%s, %d\n", __FUNCTION__, data->pos); 

	GYCA_EmailDel(pEmail[data->pos].id); 
						
	app_gy_cas_api_init_email_data(); 

	return 0;
}

bool app_gy_cas_api_delete_all_email(ca_delete_data_t *data)
{
	GYU8 i; 
	
	if (NULL == data)
		return FALSE;

	printf("%s\n", __FUNCTION__); 

	for (i=0; i<GYCA_FLASH_EMAIL_COUNT; i++) 
		GYCA_EmailDel(i); 

	app_gy_cas_api_init_email_data(); 
	
	return 0;
}






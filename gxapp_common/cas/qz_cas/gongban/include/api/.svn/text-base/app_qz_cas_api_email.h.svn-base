/**
 *
 * @file        app_qz_cas_api_email.h
 * @brief
 * @version     1.1.0
 * @date        5/14/2014 10:12:49 AM
 * @author      chenth, chenth@nationalchip.com
 *
 */
#ifndef __APP_QZCAS_API_EMAIL_H__
#define __APP_QZCAS_API_EMAIL_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"
#include <common/list.h>
#include <gxos/gxcore_os_core.h>

#define EMail_SIZE                      (128*1024)
#define EMail_MAGIC                     (0x5A5A)
#define EMail_TIME						20
#define	EMail_Title_MAXLEN				64
#define	EMail_Sender_MAXLEN				32
#define	Email_Content_MAXLEN			512
#define	EMail_MAXNUM					20 //最多20封邮件

#define	CAS_NEW_EAMIL				    0/*新邮件通知。*/
#define CAS_NEW_EMAIL_NO_ROOM		    1/*有新邮件，但Email空间不够。*/
#define	CAS_EMAIL_NONE			        2/*没有新邮件。*/


typedef struct {
	uint8_t wStatus; 
	uint8_t wShowStatus; 
	uint8_t uCount; 
} cas_email_st; 

typedef enum 
{
	EMAIL_TITLE_ID = 0,
    EMAIL_SENDER_ID,
	EMAIL_SENDTIME_ID,
	EMAIL_CONTENT_ID,
	EMAIL_FLAG_ID,
}EMAIL_ID_E;


typedef struct tag_qz_mail_Data_struct{
    struct      gxlist_head ltNode;
    uint32_t	MessageId;		/* 	4 byte*/
	uint8_t		MessageFlag; 	/*	0 -- 不需要存储 1 -- 需要存储 2 -- 未阅读 3-- 已经阅读 4 -- 删除 0xff 没有初始化*/
    uint8_t     mailId;
    uint8_t		reserved[2];			/*  1 byte*/
    uint8_t		SendTime[EMail_TIME]; /* 20 byte*/
	uint8_t 	Title[EMail_Title_MAXLEN];/*21 byte*/
    uint8_t		Sender[EMail_Sender_MAXLEN]; /* 21 byte*/
	uint8_t   	Content[Email_Content_MAXLEN];/*512 byte*/
}QZ_MAIL_DATA;

typedef struct tag_flash_data_st{
    uint16_t u16Magic;
    uint16_t u16Size;
    uint8_t  data[0];
}MAIL_FLASH_DATA;

typedef struct tag_mail_list
{
    uint16_t	EmailCount;
    uint16_t    NewEmailCount;
    uint32_t    uId;
    handle_t    mutex;
    struct      gxlist_head Head;
}MAIL_LIST,*pMAIL_LIST;

int32_t app_qz_cas_api_init_email_data(void);
uint8_t app_qz_cas_api_get_email_count(ca_get_count_t *data);
char * app_qz_cas_api_get_email_data(ca_get_date_t *data);

bool app_qz_cas_api_delete_email(ca_delete_data_t *data);
bool app_qz_cas_api_delete_all_email(ca_delete_data_t *data);

int32_t app_qz_cas_email_flag_exec();
int32_t app_qz_cas_gxmsg_ca_on_event_email(GxMsgProperty0_OnEvent* event);
void CA_toEmailNotify(uint8_t byShow);
int Email_Analyze(uint8_t *buf,int buflen,uint8_t *mailId,char *cTitle,char *cContent,char *cSender,char *cTime);
int EmailList_Update(uint8_t mailId,char *cTitle,char *cContent,char *cSender,char *cTime,int msgid);
QZ_MAIL_DATA *EmailList_SearchByMailId(uint8_t mailId);
void EmailList_deletebyRow(int row);
void EmailList_deleteAll();


#ifdef __cplusplus
}
#endif
#endif /*__APP_QZCAS_API_EMAIL_H__*/


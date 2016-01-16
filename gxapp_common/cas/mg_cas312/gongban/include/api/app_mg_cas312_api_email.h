/**
 *
 * @file        app_mg_cas_api_email.h
 * @brief
 * @version     1.1.0
 * @date        4/21/2014 10:12:49 AM
 * @author      chenth, chenth@nationalchip.com
 *
 */
#ifndef __APP_MGCAS312_API_EMAIL_H__
#define __APP_MGCAS312_API_EMAIL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"
#include <common/list.h>
#include <gxos/gxcore_os_core.h>

#define EMail_SIZE                      (128*1024)
#define EMail_MAGIC                     (0x5A5A)
#define EMail_TIME						(20)
#define	EMail_Title_MAXLEN				(21)
#define	EMail_Sender_MAXLEN				(21)
#define	Email_Content_MAXLEN			(512)
#define	EMail_MAXNUM					(20)

#define	MGCAS312_NEW_EAMIL		  		(0)
#define MGCAS312_NEW_EMAIL_NO_ROOM	  	(1)
#define	MGCAS312_EMAIL_NONE	   		   	(2)


typedef struct 
{
	uint8_t wStatus; 
	uint8_t wShowStatus; 
	uint8_t uCount; 
} mg312_cas_email_st; 

typedef enum 
{
	MG312_EMAIL_TITLE_ID = 0,
    MG312_EMAIL_SENDER_ID = 21,
	MG312_EMAIL_SENDTIME_ID = 42 ,
	MG312_EMAIL_CONTENT_ID  = 48,
	MG312_EMAIL_FLAG_ID,
}MG312_EMAIL_ID_E;


typedef struct tag_mg312_mail_Data_struct
{
    struct      gxlist_head ltNode;
    uint32_t	MessageId;		/* 	4 byte*/
	uint8_t		MessageFlag; 	/*	0 -- 不需要存储 1 -- 需要存储 2 -- 未阅读 3-- 已经阅读 4 -- 删除 0xff 没有初始化*/
    uint8_t		reserved[3];			/*  1 byte*/
    uint8_t		SendTime[EMail_TIME]; /* 20 byte*/
	uint8_t 	Title[EMail_Title_MAXLEN+1];/*21 byte*/
    uint8_t		Sender[EMail_Sender_MAXLEN+1]; /* 21 byte*/
	uint8_t   	Content[Email_Content_MAXLEN];/*512 byte*/
}MG312_MAIL_DATA;

typedef struct tag_mg312_flash_data_st
{
    uint16_t u16Magic;
    uint16_t u16Size;
    uint8_t  data[0];
}MG312_FLASH_DATA;

typedef struct tag_mg312_mail_list
{
    uint16_t	EmailCount;
    uint16_t    NewEmailCount;
    uint32_t    uId;
    handle_t    mutex;
    struct      gxlist_head Head;
}MG312_MAIL_LIST,*pMG312_MAIL_LIST;


int32_t app_mg_cas312_api_init_email_data(void);
uint8_t app_mg_cas312_api_get_email_count(ca_get_count_t *data);
char * app_mg_cas312_api_get_email_data(ca_get_date_t *data);

bool app_mg_cas312_api_delete_email(ca_delete_data_t *data);
bool app_mg_cas312_api_delete_all_email(ca_delete_data_t *data);

int32_t app_mg_cas312_email_flag_exec(void);
int32_t app_mg_cas312_gxmsg_ca_on_event_email(GxMsgProperty0_OnEvent* event);

int MG_CAS312_EmailHead_Analyze(uint8_t EmailHead[50], char *cTitle, 
	   									char *cSender, char *cTime, int *ContentLen);
int MG_CAS312_EmailList_Update(char *cTitle, char *cContent, 
	    						char *cSender, char *cTime, int msgid);

#ifdef __cplusplus
}
#endif

#endif /*__APP_MGCAS312_API_EMAIL_H__*/


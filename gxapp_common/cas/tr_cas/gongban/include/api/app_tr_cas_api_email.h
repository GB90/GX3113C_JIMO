/**
 *
 * @file        app_tr_cas_api_email.h
 * @brief
 * @version     1.1.0
 * @date        12/29/2014 
 * @author      wangjian, wangjian@nationalchip.com
 *
 */
#ifndef __APP_TRCAS_API_EMAIL_H__
#define __APP_TRCAS_API_EMAIL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"
#include <common/list.h>
#include <gxos/gxcore_os_core.h>
#include "Tr_Cas.h"
#include "tr_cas_porting.h"


/*wangjian add on 20150326 for support deleted email can be received.*/
#define SUPPORT_DELETE_EMAIL_STORE

#define EMAIL_FLASH_OFFSET   (FLASH_TRCAS_COMMON_OFFSET+FLASH_TRCAS_COMMON_SIZE)
#define EMAIL_DATA_SIZE                 (64*1024)
#define EMAIL_MAGIC                     (0x5A5A)

#define	EMAIL_MAXNUM					(50)
#define EMAIL_TIME_LEN 					(20)
#define	EMAIL_TITLE_MAXLEN				(TRCAS_TITLE_LEN)
#define	EMAIL_CONTENT_MAXLEN			(TRCAS_CONTENT_LEN)

#define	TRCAS_EMAIL_NONE  (0)
#define	TRCAS_EMAIL_NEW   (1)

typedef enum 
{
	TR_EMAIL_FLAG_ID,
	TR_EMAIL_LEVEL_ID,
	TR_EMAIL_TITLE_ID,
	TR_EMAIL_SENDTIME_ID,
	TR_EMAIL_CONTENT_ID
}TR_EMAIL_ID_e;

typedef enum
{
	EMAIL_FLAG_FREE = 0,
	EMAIL_FLAG_UNREAD,
	EMAIL_FLAG_READED,
	EMAIL_FLAG_DELETE
}TRCAS_EMAIL_FLAG_t;

typedef struct 
{
	uint8_t status; 
	uint8_t show_status; 
}TRCAS_EMAIL_t; 

typedef struct tag_tr_mail_Data_st
{
	uint32_t data_crc32;
	
	TRCAS_EMAIL_FLAG_t flag;
	uint8_t level; /*0:LOW; 1: MED; 2:HIGH; 3:URGENT*/
	uint16_t title_len;
	uint16_t content_len;
    CAS_TIMESTAMP create_time;
	uint8_t title[EMAIL_TITLE_MAXLEN+1];
	uint8_t content[EMAIL_CONTENT_MAXLEN];
}TR_EMAIL_DATA_t;

typedef struct tag_mg_mail_list
{	
	uint16_t head_magic;
	handle_t mutex;
    uint8_t total;
    uint8_t new_count;    
    TR_EMAIL_DATA_t list[EMAIL_MAXNUM];
	uint16_t tail_magic;
}TR_EMAIL_LIST_t;

/*API---------------------------------------------------------------------*/
int32_t app_tr_cas_api_init_email_data(void);
uint8_t app_tr_cas_api_email_isnew(uint32_t email_crc32);

uint8_t app_tr_cas_api_get_email_count(ca_get_count_t *data);
char* app_tr_cas_api_get_email_data(ca_get_date_t *data);

bool app_tr_cas_api_delete_email(ca_delete_data_t *data);
bool app_tr_cas_api_delete_all_email(ca_delete_data_t *data);

int32_t app_tr_cas_email_flag_exec(void);
int32_t app_tr_cas_gxmsg_ca_on_event_email(GxMsgProperty0_OnEvent* event);
uint8_t app_tr_cas_email_list_update(void *pCa_emailInfo, uint32_t email_crc32);
uint8_t* app_tr_cas_api_get_email_title_data(void);
uint8_t* app_tr_cas_api_get_email_content_data(void);


#ifdef __cplusplus
}
#endif

#endif /*__APP_TRCAS_API_EMAIL_H__*/


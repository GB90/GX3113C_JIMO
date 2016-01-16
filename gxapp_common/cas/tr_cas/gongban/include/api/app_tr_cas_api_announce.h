/**
 *
 * @file        app_tr_cas_api_announce.h
 * @brief
 * @version     1.1.0
 * @date        12/28/2014 
 * @author      wangjian (wj), wangjian@nationalchip.com
 */
#ifndef __APP_TR_CAS_API_ANNOUNCE_H__
#define __APP_TR_CAS_API_ANNOUNCE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"
#include "Tr_Cas.h"


/*wangjian add on 20150326 for announce data save into flash.*/
#define ANNOUNCE_DATA_SIZE 		(64*1024) 
#define ANNOUNCE_NUM_MAX       	(50)
#define ANNOUNCE_MAGIC          (0x55AA)

#define ANNOUNCE_TITLE_LEN    	(TRCAS_TITLE_LEN)
#define ANNOUNCE_CONTENT_LEN    (TRCAS_CONTENT_LEN)

typedef enum 
{
	TR_ANNOUNCE_FLAG_ID,
	TR_ANNOUNCE_LEVEL_ID,
	TR_ANNOUNCE_TITLE_ID,
	TR_ANNOUNCE_SENDTIME_ID,
	TR_ANNOUNCE_CONTENT_ID
}TR_ANNOUNCE_ID_e;

typedef enum
{
	ANNOUNCE_FREE = 0,
	ANNOUNCE_UNREAD,
	ANNOUNCE_READED,
	ANNOUNCE_HIDE
}TRCAS_ANNOUNCE_FLAG_t;

typedef struct 
{
	uint8_t status;/*0: hide; 1: unread; 2: readed.*/
	uint32_t data_crc32;
	uint8_t level; /*0:LOW; 1: MED; 2:HIGH; 3:URGENT*/
	CAS_TIMESTAMP create_time;

	uint8_t title[ANNOUNCE_TITLE_LEN+1];
	uint8_t content[ANNOUNCE_CONTENT_LEN];
	uint16_t title_len;
	uint16_t content_len;
}TR_CAS_ANNOUNCE_INFO_t; 

typedef struct 
{
	uint16_t head_magic;
	handle_t mutex;

	uint8_t total;
	uint8_t new_count;
	TR_CAS_ANNOUNCE_INFO_t announce_list[ANNOUNCE_NUM_MAX];
	uint16_t tail_magic;
}TR_CAS_ANNOUNCE_LIST_t; 


/*-----------------------------------------------------------*/
int8_t app_tr_cas_announce_init(void);
int32_t app_tr_cas_gxmsg_ca_on_event_announce(GxMsgProperty0_OnEvent* event);
uint8_t app_tr_cas_announce_exec(void);
uint8_t app_tr_cas_announce_is_new(uint32_t msg_crc32);

uint8_t app_tr_cas_api_get_announce_count(ca_get_count_t *data);
char* app_tr_cas_api_get_announce_data(ca_get_date_t *data);
bool app_tr_cas_api_delete_announce(ca_delete_data_t *data);
bool app_tr_cas_api_delete_all_announce(ca_delete_data_t *data);

#ifdef __cplusplus
}
#endif

#endif /*__APP_TR_CAS_API_ANNOUNCE_H__*/


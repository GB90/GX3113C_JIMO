/**
 *
 * @file        app_mg_cas_api_demux.h
 * @brief
 * @version     1.1.0
 * @date        04/11/2014 
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_MG_CAS312_API_DEMUX_H__
#define __APP_MG_CAS312_API_DEMUX_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"


#define ECM_TABLEID_80                      (0x80)

#define EMM_FILTER_NUM						(3)
#define EMM_TABLEID_U		                (0x84)
#define EMM_TABLEID_G		                (0x85)
#define EMM_TABLEID_A		                (0x87)


uint8_t	app_mg_cas312_api_release_ecm_filter(void);
uint8_t app_mg_cas312_api_start_ecm_filter(play_scramble_para_t *pplayPara);
uint8_t app_mg_cas312_api_release_emm_filter(void);
uint8_t app_mg_cas312_api_start_emm_filter(uint16_t emm_pid);
uint8_t app_mg_cas312_api_reset_emm_filter(void);
bool app_mg_cas312_api_check_cat_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id);
bool app_mg_cas312_api_check_pmt_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id);
 void app_mg_cas312_api_create_filter_time_out_task(void);
 void app_mg_cas312_filter_notify(handle_t Filter, const uint8_t* Section, size_t Size);
 void app_mg_cas312_filter_timeout_notify(handle_t Filter, const uint8_t* Section, size_t Size);

#ifdef __cplusplus
}
#endif

#endif /*__APP_MG_CAS312_API_DEMUX_H__*/


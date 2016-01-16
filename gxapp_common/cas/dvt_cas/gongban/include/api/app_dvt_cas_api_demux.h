/**
 *
 * @file        app_dvt_cas_api_demux.h
 * @brief
 * @version     1.1.0
 * @date        11/29/2013 
 * @author      zhouhm
 *
 */
#ifndef __APP_DVT_CAS_API_DEMUX_H__
#define __APP_DVT_CAS_API_DEMUX_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

uint16_t app_dvt_cas_api_get_emm_pid(void);
uint8_t	app_dvt_cas_api_release_ecm_filter(void);
uint8_t app_dvt_cas_api_start_ecm_filter(play_scramble_para_t *pplayPara);
uint8_t app_dvt_cas_api_release_emm_filter(void);
uint8_t app_dvt_cas_api_start_emm_filter(uint16_t emm_pid);
bool app_dvt_cas_api_check_cat_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id);
bool app_dvt_cas_api_check_pmt_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id);
 void app_dvt_cas_api_create_filter_time_out_task(void);
 void app_dvt_cas_filter_notify(handle_t Filter, const uint8_t* Section, size_t Size);
 void app_dvt_cas_filter_timeout_notify(handle_t Filter, const uint8_t* Section, size_t Size);

#ifdef __cplusplus
}
#endif
#endif /*__APP_DVT_CAS_API_DEMUX_H__*/


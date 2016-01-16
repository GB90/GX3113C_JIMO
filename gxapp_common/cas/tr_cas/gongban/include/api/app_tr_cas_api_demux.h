/**
 *
 * @file        app_gy_cas_api_demux.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      bingyu , yubing@nationalchip.com
 *
 */
#ifndef __APP_TR_CAS_API_DEMUX_H__
#define __APP_TR_CAS_API_DEMUX_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"


uint8_t	app_tr_cas_api_release_ecm_filter(void);
uint8_t app_tr_cas_api_start_ecm_filter(play_scramble_para_t *pplayPara);
uint8_t app_tr_cas_api_release_emm_filter(void);
uint8_t app_tr_cas_api_start_emm_filter(uint16_t emm_pid);

bool app_tr_cas_api_check_cat_ca_descriptor_valid(uint8_t *sectionData, uint32_t CA_system_id);
bool app_tr_cas_api_check_pmt_ca_descriptor_valid(uint8_t *sectionData, uint32_t CA_system_id);
void app_tr_api_smart_init(void);
void app_tr_cas_api_get_app_scramble_para(play_scramble_para_t* Param);
void app_tr_cas_api_nit_zone_des(uint8_t* databuf);
uint8_t app_tr_cas_api_search_regioncode(void);
uint16_t app_tr_cas_api_get_regioncode(void);


#ifdef __cplusplus
}
#endif

#endif /*__APP_TR_CAS_API_DEMUX_H__*/


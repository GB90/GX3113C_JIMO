/**
 *
 * @file        app_cd_cas_3.0_api_demux.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_DESAICAS_50_API_DEMUX_H__
#define __APP_DESAICAS_50_API_DEMUX_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"


uint8_t	app_desai_cas50_api_release_ecm_filter(void);
uint8_t app_desai_cas50_api_start_ecm_filter(play_scramble_para_t *pplayPara);
uint8_t app_desai_cas50_api_release_emm_filter(void);
uint8_t app_desai_cas50_api_start_emm_filter(uint16_t emm_pid);
bool app_desai_cas50_api_check_cat_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id);
bool app_desai_cas50_api_check_pmt_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id);
 void app_desai_cas50_filter_notify(handle_t Filter, const uint8_t* Section, size_t Size);
 void app_desai_cas50_filter_timeout_notify(handle_t Filter, const uint8_t* Section, size_t Size);
 void app_desai_cas50_api_nit_zone_des(uint8_t* databuf);

#ifdef __cplusplus
}
#endif
#endif


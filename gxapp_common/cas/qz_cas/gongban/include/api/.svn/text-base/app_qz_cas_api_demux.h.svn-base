/**
 *
 * @file        app_qz_cas_api_demux.h
 * @brief
 * @version     1.1.0
 * @date        05/6/2014 
 * @author      chenth, chenth@nationalchip.com
 *
 */
#ifndef __APP_QZ_CAS_API_DEMUX_H__
#define __APP_QZ_CAS_API_DEMUX_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

#define EMM_TABLEID	                    (0x80)
#define ECM_TABLEID                     (0x81)



uint8_t	app_qz_cas_api_release_ecm_filter(void);
uint8_t app_qz_cas_api_start_ecm_filter(play_scramble_para_t *pplayPara);
uint8_t app_qz_cas_api_reset_ecm_filter(void);
uint8_t app_qz_cas_api_release_emm_filter(void);
uint8_t app_qz_cas_api_start_emm_filter(uint16_t emm_pid);
uint8_t app_qz_cas_api_reset_emm_filter(void);
bool app_qz_cas_api_check_cat_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id);
bool app_qz_cas_api_check_pmt_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id);
 void app_qz_cas_filter_notify(handle_t Filter, const uint8_t* Section, size_t Size);
 void app_qz_cas_filter_timeout_notify(handle_t Filter, const uint8_t* Section, size_t Size);

#ifdef __cplusplus
}
#endif
#endif /*__APP_QZ_CAS_API_DEMUX_H__*/


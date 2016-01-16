
#ifndef __APP_KP_CAS_API_DEMUX_H__
#define __APP_KP_CAS_API_DEMUX_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"


#define KP_ECM_TABLEID     (0x80)

uint8_t	app_kp_cas_api_release_ecm_filter(void);
uint8_t app_kp_cas_api_start_ecm_filter(play_scramble_para_t *pplayPara);
uint8_t app_kp_cas_api_release_emm_filter(void);
uint8_t app_kp_cas_api_start_emm_filter(uint16_t emm_pid);
uint8_t app_kp_cas_api_reset_emm_filter(void);
bool app_kp_cas_api_check_cat_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id);
bool app_kp_cas_api_check_pmt_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id);

void app_kp_cas_filter_notify(handle_t Filter, const uint8_t* Section, size_t Size);
void app_kp_cas_filter_timeout_notify(handle_t Filter, const uint8_t* Section, size_t Size);

#ifdef __cplusplus
}
#endif
#endif /*__APP_MG_CAS_API_DEMUX_H__*/
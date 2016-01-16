#ifndef __APP_KP_API_WATCHLEVEL_H__
#define __APP_KP_API_WATCHLEVEL_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

typedef enum 
{
	KP_RATING_LEVEL_ID=0,
	KP_RATING_MAX
}KP_WATCHLEVEL_ID_E;


char * app_kp_cas_api_get_watchlevel_data(ca_get_date_t *data);
char* app_kp_cas_api_change_watchlevel(ca_rating_t* data);


#ifdef __cplusplus
}
#endif
#endif


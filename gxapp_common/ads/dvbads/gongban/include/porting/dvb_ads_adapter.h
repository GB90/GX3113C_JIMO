/******************************************************************************/
/* Copyright (c) 2012 Montage Tech - All Rights Reserved                      */
/******************************************************************************/
#ifndef __DVB_ADS_ADAPTER_H__
#define __DVB_ADS_ADAPTER_H__

#include "dvb_ads_type.h"

typedef enum{
	GET_AD_START,
	GET_AD_FAILURE,
	GET_AD_TRUE,
}get_ad_flag;

/*The id of all different ads*/
typedef enum
{
	ADS_ID_ADT_TR = 0,
	ADS_ID_ADT_WFA,
	ADS_ID_ADT_QZ,
	ADS_ID_ADT_SHIDA,
	ADS_ID_ADT_DESAI,  
	ADS_ID_ADT_DVBAD,
	ADS_ID_ADT_MAX_NUM,
}ads_module_id_t;

/*show position of ad*/
typedef enum
{
	ADS_AD_TYPE_WELCOME = 0,
	ADS_AD_TYPE_LOGO,
	ADS_AD_TYPE_RADIO,
	ADS_AD_TYPE_CHBAR,
	ADS_AD_TYPE_CHLIST,
	ADS_AD_TYPE_EPG,
	ADS_AD_TYPE_MAIN_MENU,
	ADS_AD_TYPE_MENU_UP,
	ADS_AD_TYPE_MENU_DOWN,
	ADS_AD_TYPE_VOLBAR,
	ADS_AD_TYPE_SUBTITLE,
	ADS_AD_TYPE_VOD,
	ADS_AD_TYPE_CA,
	ADS_AD_TYPE_BANNER,
	ADS_AD_TYPE_OTHER_GIF,
	ADS_AD_TYPE_CHANGE_CHANNEL,
	ADS_AD_TYPE_SUB_MENU,
	ADS_AD_TYPE_FAVORITEPRO,
	ADS_AD_TYPE_CHANGE_FREQ,
	ADS_AD_TYPE_SEARCHING,
	ADS_AD_TYPE_SIGNAL,
	ADS_AD_TYPE_NOSIGNAL,
	ADS_AD_TYPE_NORIGHT,
	ADS_AD_TYPE_LEFTCHANNEL,
	ADS_AD_TYPE_RIGHTCHANNEL,
	ADS_AD_TYPE_STEREOPHONIC,
	ADS_AD_TYPE_OSD,
	ADS_AD_TYPE_PIC,
	ADS_AD_TYPE_AV,
	ADS_AD_TYPE_UNAUTH_PG,
	ADS_AD_TYPE_NUM,
}ads_ad_type_t;

typedef enum
{
	ADS_PIC_IN_FLASH = 1,
	ADS_PIC_IN_SDRAM,
}ads_pic_in_t;

typedef enum
{
	ADS_IOCMD_AD_PIC_INFO_GET = 0,
	ADS_IOCMD_AD_SEARCH_SET,
	ADS_IOCMD_AD_KEY_GET,
	ADS_IOCMD_AD_GET_FLASH,
	ADS_IOCMD_AD_EXIT_TIMEOUT,
	ADS_IOCMD_AD_NO_PROGRAM,
	ADS_IOCMD_AD_PROGRAM,
	ADS_IOCMD_AD_VERSION_GET,
}ads_ioctrl_cmd_t;

typedef struct
{
	u16 service_id;
	u16 ts_id;
	u16 network_id;
}ads_service_t;

typedef enum
{
	ADS_PLATFORM_SD = 1,
	ADS_PLATFORM_HD = 2,
	ADS_PLATFORM_UNKNOWN
}ads_platform_type_t;

typedef struct
{
	u8  type;
	u8  reserve1;
	u16 reserve2;
	s16 x;
	s16 y;
}ads_pic_position_t;

typedef struct
{
	ads_platform_type_t platform_type;
	ads_ad_type_t ad_type;
	ads_service_t service_info;
	ads_pic_position_t pic_pos;
	ads_pic_in_t pic_in;
	u8 task_priority;
	u32 *p_task_stack;  
	u32 stack_size;   
	u8* flash_start_addr;  
	u32 flash_size;
	u32 channel_frequency;  
	u32 channel_symbolrate;
	u32 channel_qam;
}ads_module_cfg_t;

//---------------------------------------------------
typedef struct
{
	u8 id;
	u8 *version;
	u8 *p_ad_url;
	u32 show_time;
	u8 task_priority;
	u8* flash_start_addr;  
	u32 flash_size;
	u32 channel_frequency;  
	u32 channel_symbolrate;
	u16 channel_qam;
}ads_module_priv_t;

//----------------------------------------------------
typedef struct
{
  	RET_CODE (*init)(void *param);
  	RET_CODE (*deinit)();  
  	RET_CODE (*open)();
  	RET_CODE (*close)();
  	RET_CODE (*io_ctrl)(u32 cmd, void *param);
}ads_func_t;

typedef struct
{
	  u8 attached;
	  u8 inited;
	  ads_func_t func;
	  void *p_priv;
}ads_op_t;

typedef struct
{
	  u32 inited;
	  ads_op_t adm_op[ADS_ID_ADT_MAX_NUM];
	  //os_sem_t evt_mutex;
}ads_adapter_priv_t;

//-----------------------------------------------------

RET_CODE ads_init(ads_module_id_t adm_id, ads_module_cfg_t *p_param);
RET_CODE ads_deinit(ads_module_id_t adm_id);
RET_CODE ads_detach(u32 adm_id);
RET_CODE ads_open(ads_module_id_t adm_id);
RET_CODE ads_close(ads_module_id_t adm_id);

RET_CODE ads_io_ctrl(ads_module_id_t adm_id, u32 cmd, void *p_param);

ads_module_id_t ads_adt_dvbad_attach(ads_module_cfg_t * p_cfg);

#endif //__DVB_ADS_ADAPTER_H__

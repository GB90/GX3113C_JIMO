/******************************************************************************/
/* Copyright (c) 2012 Montage Tech - All Rights Reserved                      */
/******************************************************************************/

/*
 This file for match the right AD.
*/
#include "dvb_ads_function_porting.h"
#include "dvb_ads_type.h"
#include "dvb_ads_drv.h"

#define OPEN_AD_MODULE_MAX_TIMES    5000

get_ad_flag g_ads_get_flag = GET_AD_START;

// * The global var to save the atached module info and operation info of this module!!!
ads_adapter_priv_t g_ads_priv = {0};
ads_module_priv_t *p_ads_priv = NULL;	//the potiner is point to the same addr of g_ads_priv.

ads_module_priv_t *p_dvbad_priv = NULL;

//----------------------------------------
void dvb_set_ad_flag(get_ad_flag flag)
{
    g_ads_get_flag = flag;
}

static get_ad_flag dvb_get_ad_flag()
{
    return g_ads_get_flag;
}
//----------------------------------------

//For using the sys func,must use the sys struct ,make a change and choose the correct para
fe_modulation_t exchange_modulation_from_ad_to_sys(u16 modulation)

{
	if(0 <=  modulation)
	{
		switch(modulation)
		{
			case 0:
				return QPSK;
				break;
			case 1:
				return QAM_16;
				break;
			case 2:
				return QAM_32;
				break;
			case 3:
				return QAM_64;
				break;
			case 4:
				return QAM_128;
				break;
			case 5:
				return QAM_256;
				break;
			case 6:
				return QAM_AUTO;
				break;	
			default :
				return QAM_64;
				break;
		}
	}
	return QAM_64;
}

/*-----------the next func are for the attach and detach the right AD type--------------*/
//-------------operation after choose the right AD type and attached----------------

RET_CODE ads_adt_dvbad_init(void *param)
{
	return SUCCESS;
}

RET_CODE ads_adt_dvbad_deinit()
{
	return SUCCESS;
}

static RET_CODE ads_adt_dvbad_open(void)
{
	//BOOL ret = FALSE;
	u32 index = 0;
	get_ad_flag ad_flag = GET_AD_START;
	RET_CODE r = ERR_FAILURE;
	
	//fe_modulation_t channel_qam = exchange_modulation_from_ad_to_sys(p_dvbad_priv->channel_qam);
	//if (0 != app_search_lock_tp(p_dvbad_priv->channel_frequency,p_dvbad_priv->channel_symbolrate,INVERSION_OFF,channel_qam,2000))
	//{
	//	DVB_ADS_DEBUG(DVBAD,ERR,"Lock tp failed!!!");
	//	search_lock_main_tp_flag = FALSE;
	//	return ERR_FAILURE;
	//}
	//search_lock_main_tp_flag = TRUE;
	dvbad_set_filter_bat();
	DVB_ADS_DEBUG(DVBAD,INFO,"DVB AD OPEN!!!\n");
	dvbad_ads_client_init();
	DVB_ADS_DEBUG(DVBAD,INFO,"The main thread start to monitor the flag of get_ad_flag!!!");
	for(index = 0; index < OPEN_AD_MODULE_MAX_TIMES; index++)
	{
		ad_flag = dvb_get_ad_flag();
		if(GET_AD_TRUE == ad_flag)//filter END no matter success or failed
		{
			
			r = SUCCESS;
			DVB_ADS_DEBUG(DVBAD,INFO,"Get ad success!!!");
			break;
		}
		else if(GET_AD_FAILURE == ad_flag)
		{
			DVB_ADS_DEBUG(DVBAD,INFO,"Get ad failed!!!");
			r = SUCCESS;
			break;
		}
		GxCore_ThreadDelay(10);
	}
	return r;
}

static RET_CODE ads_adt_dvbad_close()
{
	return SUCCESS;
}

static DVBAD_POS_TYPE dvbad_type_get(u32 ads_type)
{
    	DVBAD_POS_TYPE ad_type = DVBAD_STB_POS_UNKNOWN;


	switch(ads_type)
    	{
		case ADS_AD_TYPE_LOGO:
		case ADS_AD_TYPE_WELCOME:
		    	ad_type = DVBAD_STB_LOG;
			break;
		case ADS_AD_TYPE_MAIN_MENU:
			ad_type = DVBAD_STB_MAINMENU;
			break;
		case ADS_AD_TYPE_CHLIST:
			ad_type = DVBAD_STB_CHLIST;
			break;
		case ADS_AD_TYPE_CHBAR:
			ad_type = DVBAD_STB_CURRENT_FOLLOW;
			break;
		case ADS_AD_TYPE_EPG:
			ad_type = DVBAD_STB_EPG;
			break;
		case ADS_AD_TYPE_VOLBAR:
			ad_type = DVBAD_STB_VOLUME;
			break;
		case ADS_AD_TYPE_UNAUTH_PG:
	       		ad_type = DVBAD_STB_POS_UNKNOWN;
			break;
		}
		DVB_ADS_DEBUG(DVBAD, INFO, "get ad_type end.ad_type:%d\n", ad_type);
		return ad_type;
}

static RET_CODE ads_adt_dvbad_io_ctrl(u32 cmd, void *param)
{
	RET_CODE ret = SUCCESS;
	DVBAD_POS_TYPE dvbad_ad_type = DVBAD_STB_POS_UNKNOWN;
	dvbad_show_pic_info program_show;
	dvbad_program_para	  program_para = {0};
//	dvbad_osd_program_show_info osd_show;

	ads_module_cfg_t *p_cfg = (ads_module_cfg_t *)param;
	memset(&program_show, 0, sizeof(dvbad_show_pic_info));

	DVB_ADS_DEBUG(DVBAD, INFO, "cmd:%u,ad_type:%u,network_id:%u,ts_id:%u,service_id:%u\n",
						(unsigned int)cmd, (unsigned int)p_cfg->ad_type, (unsigned int)p_cfg->service_info.network_id,
						(unsigned int)p_cfg->service_info.ts_id, (unsigned int)p_cfg->service_info.service_id);
	dvbad_ad_type = dvbad_type_get(p_cfg->ad_type);
	switch(cmd)
	{
		case ADS_IOCMD_AD_NO_PROGRAM:
			ret = dvbad_get_ad(&program_para, &program_show, dvbad_ad_type);
			if(ret != SUCCESS)
				break;
			//dvbad_info_parser(&program_show);
			break;

		case ADS_IOCMD_AD_PROGRAM:
			program_para.usNetworkId = p_cfg->service_info.network_id;
			program_para.usTsId = p_cfg->service_info.ts_id;
			program_para.usServiceId = p_cfg->service_info.service_id;
			ret = dvbad_get_ad(&program_para, &program_show, dvbad_ad_type);
			if(ret != SUCCESS)
				break;
			DVB_ADS_DEBUG(DVBAD, INFO, "x:%d, y:%d, w:%d, h:%d\n", 
						program_show.x, program_show.y, program_show.w, program_show.h);
			break;
		case ADS_IOCMD_AD_VERSION_GET:
			 ret =	get_AD_version(OUT (u16 *)param);
			 break;
	}
	return ret;
}

ads_module_id_t ads_adt_dvbad_attach(IN ads_module_cfg_t * p_cfg)
{
	DVB_ADS_PRINTF("[adt] %s,%d \n",__func__,__LINE__);

	g_ads_priv.adm_op[ADS_ID_ADT_DVBAD].attached = 1;
	g_ads_priv.adm_op[ADS_ID_ADT_DVBAD].inited = 0;

	g_ads_priv.adm_op[ADS_ID_ADT_DVBAD].func.init
		= ads_adt_dvbad_init;
	g_ads_priv.adm_op[ADS_ID_ADT_DVBAD].func.deinit
		= ads_adt_dvbad_deinit;
	g_ads_priv.adm_op[ADS_ID_ADT_DVBAD].func.open
		= ads_adt_dvbad_open;
	g_ads_priv.adm_op[ADS_ID_ADT_DVBAD].func.close
		= ads_adt_dvbad_close;
	g_ads_priv.adm_op[ADS_ID_ADT_DVBAD].func.io_ctrl
		= ads_adt_dvbad_io_ctrl;

   	p_dvbad_priv = g_ads_priv.adm_op[ADS_ID_ADT_DVBAD].p_priv
	 = app_porting_ca_os_malloc(sizeof(ads_module_priv_t));
   	DVB_ADS_ASSERT((NULL != p_dvbad_priv));
	   memset(p_dvbad_priv, 0x00, sizeof(ads_module_priv_t));

	p_dvbad_priv->id = ADS_ID_ADT_DVBAD;

	p_dvbad_priv->channel_frequency = p_cfg->channel_frequency;
	p_dvbad_priv->channel_symbolrate = p_cfg->channel_symbolrate;
	p_dvbad_priv->channel_qam = p_cfg->channel_qam;
	p_dvbad_priv->task_priority = p_cfg->task_priority;
	DVB_ADS_DEBUG(DVBAD, INFO, "p_cfg channel_frequency %u, channel_symbolrate %u, channel_qam%u",
		(unsigned int)p_cfg->channel_frequency,(unsigned int)p_cfg->channel_symbolrate,
		(unsigned int)p_cfg->channel_qam);
	return ADS_ID_ADT_DVBAD;
}


RET_CODE ads_detach(u32 adm_id)
{
  ads_op_t *p_op = (ads_op_t *)adm_id;

  if(p_op->inited == 1)
  {
    p_op->func.deinit();
  }   
  p_op->inited = 0;
  
  
  if(p_op->p_priv != NULL)
  {
    app_porting_ca_os_free(p_op->p_priv);
    p_op->p_priv = NULL;
  }
  p_op->attached = 0;
  
  return SUCCESS;
}

/*-----------the next are for the other file calls of the ads operation-------*/
RET_CODE ads_init(ads_module_id_t adm_id, ads_module_cfg_t *p_param)
{
  RET_CODE ret = ERR_FAILURE;

  if(g_ads_priv.inited == 1)
  {
    ret = SUCCESS;
    return ret;
  }
  g_ads_priv.inited = 1;
  //DVB_ADS_ASSERT((TRUE == app_dvb_ads_sem_create(&g_ads_priv.evt_mutex,0)));
  //return SUCCESS aways,the next init func do nothing!!!
  return g_ads_priv.adm_op[adm_id].func.init(p_param);
}


RET_CODE ads_deinit(ads_module_id_t adm_id)
{
  if(g_ads_priv.inited == 1)
  {
   // DVB_ADS_ASSERT((TRUE == app_dvb_ads_sem_destroy(&g_ads_priv.evt_mutex)));
  }
  g_ads_priv.inited = 0;
  return g_ads_priv.adm_op[adm_id].func.deinit();
}



RET_CODE ads_open(ads_module_id_t adm_id)
{
  return g_ads_priv.adm_op[adm_id].func.open();
}

RET_CODE ads_close(ads_module_id_t adm_id)
{
  return g_ads_priv.adm_op[adm_id].func.close();
}

RET_CODE ads_io_ctrl(ads_module_id_t adm_id, u32 cmd, void *p_param)
{
  if(g_ads_priv.adm_op[adm_id].attached != 1)
  {
    return ERR_STATUS;
  }
  return g_ads_priv.adm_op[adm_id].func.io_ctrl(cmd, p_param);
}
//-----------------------------------------------------------------------------

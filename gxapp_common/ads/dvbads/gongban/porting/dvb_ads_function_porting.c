#include "dvb_ads_function_porting.h"
#include "ui_dvb_ads_porting.h"

static u8 version[25];

static ad_lock_t ad_maintp = {307000,6875,2};

static ads_module_cfg_t module_config = {0};
ads_module_id_t ads_id = ADS_ID_ADT_MAX_NUM;
extern os_sem_t show_pic_sem;
//NOW just to chek the p_log_data out to make sure if logo has been parsed out!!!
extern u32 dvbad_get_logo(u8 *p_log_data, u32 size);

u8* ads_get_ad_version(void)
{
  return version;
}

static RET_CODE ads_dvbad_module_init(void)
{
  RET_CODE ret = ERR_FAILURE;

  module_config.channel_frequency = ad_maintp.tp_freq;
  module_config.channel_symbolrate = ad_maintp.tp_sym;
  module_config.channel_qam = ad_maintp.nim_modulate;

  module_config.flash_size = (u32)DVB_LOGO_MAX_LEN;
  module_config.flash_start_addr = (u8*)DVB_LOGO_START_ADDR;
  module_config.pic_in = ADS_PIC_IN_SDRAM;
  module_config.task_priority = PRIORITY_LEVEL_DEFAULT;
  module_config.platform_type = ADS_PLATFORM_SD;
  DVB_ADS_PRINTF("ADS:ads_module_config.flash_start_adr= 0x%x, flash_size = 0x%x \n", (unsigned int)module_config.flash_start_addr, (unsigned int)module_config.flash_size);
  //according to the different ad to get the ads_id which will be used in the other func
  ads_id = ads_adt_dvbad_attach(IN &module_config);

  ret=ads_init(ads_id,&module_config);
  if(ret != SUCCESS)
  {
    return ret;
  }
  ret = ads_open(ads_id);
  if (ret != SUCCESS)
	return ret;
  GxCore_ThreadDelay(100);
  memset(version,0,25);
  ads_io_ctrl(ads_id, ADS_IOCMD_AD_VERSION_GET, version);
  DVB_ADS_PRINTF("ad Version:%s\n",version);
  return SUCCESS;
}


void ads_ap_init(void)
{
  ad_lock_t *tmp_maintp = &ad_maintp;
  RET_CODE ret = ERR_FAILURE;
  /*get the config freq info*/
  tmp_maintp->tp_freq = app_flash_get_config_center_freq();
  tmp_maintp->tp_sym = app_flash_get_config_center_freq_symbol_rate();
  tmp_maintp->nim_modulate = app_flash_get_config_center_freq_qam();
  app_dvb_ads_sem_create(&show_pic_sem,1);
  DVB_ADS_PRINTF("[AD_INIT] tmp_maintp.nim_modulate= %u \n",(unsigned int)tmp_maintp->nim_modulate);
      
  ret = ads_dvbad_module_init();
  if(ret == SUCCESS)
  {
     DVB_ADS_DEBUG(DVBAD,INFO,"DVB AD init OK!!!");
  }
  else
  {
    DVB_ADS_DEBUG(DVBAD,INFO,"DVB AD Wrong!!!");
  }
}


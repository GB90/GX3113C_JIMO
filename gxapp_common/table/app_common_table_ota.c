#include <gxtype.h>
#include "service/gxsi.h"
#include "gxapp_sys_config.h"
#include "app_common_table_ota.h"
#include "app_common_flash.h"
#include "app_common_search.h"
#include "app_common_init.h"
#include "gxfrontend.h"
#include "gx_demux.h"
#include "app_common_porting_stb_api.h"
#include "app_common_porting_ca_demux.h"

static handle_t ota_filter =0;
static handle_t ota_channel=0;
uint8_t ota_flag = 0;
uint16_t ota_pid = 0x1fff;
uint8_t ota_table_id = 0x0;
static char* ota_qam[]={"QAM_16","QAM_32","QAM_64","QAM_128","QAM_256"};
extern unsigned int htoi(const char *str);
extern unsigned int convert_version_str(const char *str);


handle_t app_table_ota_get_filter_handle(void)
{
	return ota_filter;
}

handle_t app_table_ota_get_channel_handle(void)
{
	return ota_channel;
}


int app_table_ota_monitor_filter_close(void)
{
	int32_t ret = 0;
	app_porting_psi_demux_lock();

	if (0 != ota_channel)
	{
		printf("!!!!!!!!!!app_table_ota_monitor_filter_close!!!!!!!!!!!!!!!!!\n");

		if(ota_filter != 0)
		{
			//				ret = GxDemux_FilterDisable(ota_filter);
			//				CHECK_RET(DEMUX,ret);
			ret = GxDemux_FilterFree(ota_filter);
			CHECK_RET(DEMUX,ret);				
			ota_filter = 0;
		}

		ret = GxDemux_ChannelFree(ota_channel);
		CHECK_RET(DEMUX,ret);
		ota_channel = 0;
	}
	app_porting_psi_demux_unlock();

	return 0;
}


void app_table_ota_filter_notify(handle_t Filter, const uint8_t* Section, size_t Size)
{

	int16_t            pid;
	uint16_t            section_length;
	uint8_t*            data = (uint8_t*)Section;
	uint8_t version = 0;
	uint8_t match[8] = {0,};
	uint8_t mask[8] = {0,};
	int32_t ret;

	if (NULL == Section)
		return;

	//    ASSERT(Section != NULL);
	app_porting_psi_demux_lock();

	ret = GxDemux_FilterGetPID(Filter,&pid);
	if (ota_pid != pid)
	{
		CAS_Dbg("app_table_ota_filter_notify pid error  pid:0x%x\n", pid);
		app_porting_psi_demux_unlock();
		return ;
	}
	app_porting_psi_demux_unlock();

	CAS_Dbg("app_table_ota_filter_notify total size:%d\n", Size);



	/*应用分析NIT表，先调用应用分析NIT表
	  之后永新CA库中设置NIT过滤分析*/

	section_length = ((data[1] & 0x0F) << 8) + data[2] + 3;
	version = data[5] & 0x3E;
	app_table_ota_monitor_section_parse(data, section_length);
	{
		app_porting_psi_demux_lock();
		ret = GxDemux_FilterDisable(Filter);
		CHECK_RET(DEMUX,ret);
		match[0] = ota_table_id;
		mask[0]  = 0xFF;
		match[5] = version;
		mask[5]  = 0x3E;
		match[6] = 0x00; // section number 0
		mask[6]  = 0xFF;

		ret = GxDemux_FilterDisable(Filter);
		CHECK_RET(DEMUX,ret);
		ret = GxDemux_FilterSetup(Filter, match, mask, FALSE, TRUE,0, 8);
		CHECK_RET(DEMUX,ret);
		ret = GxDemux_ChannelEnable(ota_channel);
		CHECK_RET(DEMUX,ret);
		ret = GxDemux_FilterEnable(Filter);
		CHECK_RET(DEMUX,ret);
		app_porting_psi_demux_unlock();

	}

	return;

}

int app_table_ota_monitor_filter_open(uint16_t pid,uint8_t tabieid)
{
	handle_t filter;
	uint8_t match[8] = {0,};
	uint8_t mask[8] = {0,};

	int32_t ret = 0;
	match[0] = tabieid;
	mask[0]  = 0xFF;
	match[6] = 0x00; // section number 0
	mask[6]  = 0xFF;


	CAS_Dbg("app_table_ota_monitor_filter_open\n");
	app_porting_psi_demux_lock();

	if(ota_channel != 0)
	{
		printf("ota_channel = 0x%x\n",ota_channel);
		app_porting_psi_demux_unlock();

		return 0;
	}
	printf("!!!!!!!!!!app_table_ota_monitor_filter_open!!!!!!!!!!!!!!!!!\n");
	ota_channel = GxDemux_ChannelAllocate(0, pid);
	if ((0 == ota_channel)||(-1 == ota_channel))
	{
		CA_ERROR("  nit_channel=0x%x\n",ota_channel);
		ota_channel = 0;
		app_porting_psi_demux_unlock();

		return 0;
	}

	filter = GxDemux_FilterAllocate(ota_channel);
	if (( 0 == filter)||(-1 == filter))
	{
		ret = GxDemux_ChannelFree(ota_channel);
		CHECK_RET(DEMUX,ret);
		ota_channel = 0;

		CA_ERROR(" filter=0x%x\n",filter);
		filter = 0;
		app_porting_psi_demux_unlock();

		return 0;
	}

	ota_filter = filter;
	ret = GxDemux_FilterSetup(filter, match, mask, TRUE, TRUE,0, 8);
	CHECK_RET(DEMUX,ret);
	//	GxCA_DemuxFilterRigsterNotify(filter, app_table_nit_filter_notify,NULL);
	ret = GxDemux_ChannelEnable(ota_channel);
	CHECK_RET(DEMUX,ret);
	ret = GxDemux_FilterEnable(filter);
	CHECK_RET(DEMUX,ret);
	ota_pid = pid;
	ota_table_id = tabieid;
	app_porting_psi_demux_unlock();



	return 0;
}

int app_table_ota_monitor_filter_restart(void)
{	
	handle_t filter;
	uint8_t match[8] = {0,};
	uint8_t mask[8] = {0,};

	int32_t ret = 0;
	match[0] = ota_table_id;
	mask[0]  = 0xFF;
	match[6] = 0x00; // section number 0
	mask[6]  = 0xFF;
	app_porting_psi_demux_lock();

	if (0x1fff == ota_pid)
	{
		/*
		 * ota pid invalid 
		 */
		app_porting_psi_demux_unlock();
		return 0;
	}


	CAS_Dbg("app_table_ota_monitor_filter_restart\n");


	if (0 != ota_filter)
	{
		//				ret = GxDemux_FilterDisable(ota_filter);
		//				CHECK_RET(DEMUX,ret);
		ret = GxDemux_FilterFree(ota_filter);
		CHECK_RET(DEMUX,ret);
		ota_filter = 0;
	}
	if (0 != ota_channel)
	{
		ret = GxDemux_ChannelFree(ota_channel);
		CHECK_RET(DEMUX,ret);
		ota_channel = 0;
	}

	ota_channel = GxDemux_ChannelAllocate(0, ota_pid);
	if ((0 == ota_channel)||(-1 == ota_channel))
	{
		CA_ERROR(" ota_channel=0x%x\n",ota_channel);
		ota_channel = 0;
		app_porting_psi_demux_unlock();
		return 0;
	}
	filter = GxDemux_FilterAllocate(ota_channel);
	if (( 0 == filter)||(-1 == filter))
	{
		ret = GxDemux_ChannelFree(ota_channel);
		CHECK_RET(DEMUX,ret);
		ota_channel = 0;

		CA_ERROR(" filter=0x%x\n",filter);
		filter = 0;
		app_porting_psi_demux_unlock();
		return 0;
	}


	ret = GxDemux_FilterSetup(filter, match, mask, TRUE, TRUE,0, 8);
	CHECK_RET(DEMUX,ret);
	//	GxCA_DemuxFilterRigsterNotify(filter, app_table_cat_filter_notify,NULL);
	ret = GxDemux_ChannelEnable(ota_channel);
	CHECK_RET(DEMUX,ret);
	ret = GxDemux_FilterEnable(filter);
	CHECK_RET(DEMUX,ret);
	ota_filter = filter;
	app_porting_psi_demux_unlock();

	return 0;
}
//cth
void app_table_ota_monitor_section_parse(uint8_t* pSectionData,size_t Size)
{

	uint16_t wSectionLength = 0;
	uint16_t wSectionNumber = 0;
	uint8_t  chFileHeadLength = 0;
	uint8_t  chTableId = 0;
	uint16_t wLastSectionNumber = 0;
	uint8_t *pData = pSectionData;
	char oemValue[20]={0};
	GxFrontend frontendpara[FRONTEND_MAX];
	char*          softwareVer = NULL;
	char*          hardwareVer = NULL;
	uint32_t       m_Frequency=0;
	uint32_t       m_PlatformID = 0;
	uint8_t        m_Modulation=0;
	uint32_t       m_SymbolRate=0;
	char*          PlatformIDStr = NULL;
	uint32_t	   cmperr=0;




	if (NULL == pSectionData)
		return;

	printf("app_table_ota_monitor_section_parse=%d\n",Size);

	chTableId	   = pData[0];
	wSectionLength = (((uint16_t)pData[1] << 8) | pData[2]) & 0x0fff;
	wSectionNumber = ((uint16_t)pData[6] << 8 )| pData[7];
	wLastSectionNumber = ((uint16_t)pData[8] << 8 )| pData[9];
	pData += 10;
	/*第一个分段解析，第一个分段中包含版本控制信息
	  需要分开解析。具体版本信息参考相关文档*/
	if(0 == wSectionNumber)
	{
		unsigned int manufacture_id = 0;		
		unsigned int soft_ver = 0;
		unsigned int hard_ver = 0;
		unsigned int update_type = 0;

		chFileHeadLength = *pData++;
		CAS_Dbg("section 0  head len = %d \n",chFileHeadLength);
		
		update_type = (*(pData+12));
		CAS_Dbg("update_type = %x\n",update_type);

		manufacture_id = ((*pData)<<24)
			+(*(pData+1)<<16)
			+(*(pData+2)<<8)
			+(*(pData+3));
		pData += 4;
		CAS_Dbg("manufacture_id = %x\n",manufacture_id);
		PlatformIDStr = app_flash_get_oem_manufacture_id();
		m_PlatformID = htoi(PlatformIDStr);

		if (manufacture_id != m_PlatformID)
		{
			CAS_Dbg("manufacture_id=0x%x m_PlatformID=0x%x\n",manufacture_id,m_PlatformID);
			cmperr =1;
		}

		hard_ver = ((*pData)<<24)
			+(*(pData+1)<<16)
			+(*(pData+2)<<8)
			+(*(pData+3));
		memset(oemValue,0,20);
		sprintf(oemValue,"%02x.%02x.%02x.%02x",(*pData),*(pData+1),*(pData+2),*(pData+3));
		CAS_Dbg("hardware_version = %s\n",oemValue);

		//匹配硬件版本号，ps: 版本号00.00.00.00,转成字符串00.00.00.00保存
		hardwareVer = app_flash_get_oem_hardware_version_str();
		if (hard_ver  != convert_version_str(hardwareVer))
		{
			cmperr =2;	
		}

		pData += 4;

		soft_ver = ((*pData)<<24)
			+(*(pData+1)<<16)
			+(*(pData+2)<<8)
			+(*(pData+3));
		//匹配软件版本号

		memset(oemValue,0,20);
		sprintf(oemValue,"%02x.%02x.%02x.%02x",(*pData),*(pData+1),*(pData+2),*(pData+3));
		pData += 4;
		CAS_Dbg("softver = %x\n",soft_ver);		

		softwareVer = app_flash_get_oem_softversion_str();
		if(soft_ver <= convert_version_str(softwareVer))
		{
			cmperr =3;
		}
		if((0 == cmperr)||((5 ==update_type)&&(cmperr !=3)))
		{
				/* promt start */
			char buf[50] = {0};
			char* osd_language = NULL;

			osd_language = app_flash_get_config_osd_language();
			if (0== strcmp(osd_language,LANGUAGE_CHINESE))
			{
				sprintf((void*)buf,"当前软件版本:%s\n升级软件版本:%s\n 超时10秒或任意键进入升级", 
		        							softwareVer, oemValue);
			}
			else
			{
				sprintf((void*)buf,"Current SW:%s\nNew SW:%s\n timeout 10s,enter update.", 
		        							softwareVer, oemValue);
			}

			if ( 2 != app_ota_popmsg(210, 150, buf, 10))
			{
				return;
			}

			printf("\n----> enter update now.\n");
			/*升级类型1=手动下载2=强制升级*/
			/*发送消息至APP，需要进行更新提示*/
			printf("[update]>>需要升级，界面提示\n");
			app_flash_set_oem_soft_updateversion((char*)oemValue);

			memset(oemValue,0,20);
			sprintf((char*)oemValue, "0x%x",ota_pid);
			app_flash_set_oem_dmx_pid((char*)oemValue);
			CAS_Dbg("ota_pid = %s\n",oemValue);	
			memset(oemValue,0,20);
			sprintf((char*)oemValue, "0x%x",ota_table_id);
			app_flash_set_oem_dmx_tableid((char*)oemValue);
			CAS_Dbg("ota_table_id = %s\n",oemValue);	

			/*				memset(oemValue,0,20);
							sprintf((char*)oemValue, "%d",m_UpdateTimes);
							app_flash_set_oem_repeat_times((char*)oemValue);*/
			memset(oemValue,0,20);
			/*
			   升级私有表和升级流在同一个频点中下发，检测到升级描述子记录
			   当前频点信息，OTA锁当前频点过滤升级流*/

			GxFrontend_GetCurFre(0, &frontendpara[0]);
			m_Frequency = frontendpara[0].fre;
			m_Frequency = m_Frequency*1000;
			sprintf((char*)oemValue, "%d",m_Frequency);
			app_flash_set_oem_dmx_frequency((char*)oemValue);
			CAS_Dbg("m_Frequency = %s\n",oemValue);


			memset(oemValue,0,20);
			m_SymbolRate = frontendpara[0].symb;
			CAS_Dbg("m_SymbolRate = %d\n",m_SymbolRate);
			sprintf((char*)oemValue, "%d",m_SymbolRate);
			app_flash_set_oem_fe_symbolrate((char*)oemValue);
			CAS_Dbg("m_SymbolRate = %s\n",oemValue);


			m_Modulation = frontendpara[0].qam;
			app_flash_set_oem_fe_modulation((char*)ota_qam[m_Modulation-1]);
			CAS_Dbg("qam = %s\n",(char*)ota_qam[m_Modulation-1]);


			memset(oemValue,0,20);
			//				GxFrontend_GetCurFre(0, &frontendpara[0]);
#if (DVB_DEMOD_DTMB == DVB_DEMOD_MODE)
			if(frontendpara[0].type_1501 == GXBUS_PM_SAT_1501_DTMB)  //DTMB
			{
				sprintf((char*)oemValue,"%s","DTMB");
			}
			else
			{
				sprintf((char*)oemValue,"%s","DVB_C");
			}
#elif (DVB_DEMOD_DVBC == DVB_DEMOD_MODE)
			sprintf((char*)oemValue,"%s","DVB_C");
#endif
			CAS_Dbg("DVB_MODE = %s\n",oemValue);

			app_flash_set_oem_fe_workmode((char*)oemValue);

			if (0 != strcasecmp("yes",app_flash_get_oem_ota_flag()))
			{
				/*
				 * 无升级标志。检测升级描述子存在，立即触发升级。
				 */

				app_flash_set_oem_ota_flag("yes");
				app_flash_save_oem();

				GxCore_ThreadDelay(1000);
				// 重启
				restart();
			}
			else
			{
				/*
				 * 如果升级标志已保存(之前选择取消)，不触发
				 */
				app_flash_save_oem();
			}
			return ;

		}				
	}

	return ;
}



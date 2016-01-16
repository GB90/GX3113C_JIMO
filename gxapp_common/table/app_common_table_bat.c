#include <gxtype.h>
#include "service/gxsi.h"
#include "gxapp_sys_config.h"
#include "app_common_table_bat.h"
#include "app_common_flash.h"
#include "app_common_search.h"
#include "gxfrontend.h"
#include "gx_demux.h"
#include "app_common_porting_stb_api.h"
#include "app_common_init.h"
#include "app_common_prog.h"


uint8_t pBatSectionFlag[256] = {0};
static bat_parse_descriptor app_table_bat_parse_descriptor_callback = NULL;

static int32_t s_BatSubtId = -1;
static uint32_t s_BatRequestId = 0;

void app_table_bat_filter_close(void)
{
	GxMsgProperty_SiRelease params_release;
	
	if (-1 != s_BatSubtId)
	{
		params_release = s_BatSubtId;
		app_send_msg(GXMSG_SI_SUBTABLE_RELEASE, (void*)&params_release);
	}
	s_BatSubtId = -1;	
}

void app_table_bat_filter_open(void)
{

	static GxSubTableDetail subt_detail = {0};
	GxMsgProperty_SiCreate	params_create;
	GxMsgProperty_SiStart params_start;

	if(s_BatSubtId == -1)
	{
		subt_detail.ts_src = DVB_TS_SRC;
		subt_detail.demux_id = 0;
		subt_detail.time_out = 10000;
		subt_detail.si_filter.pid = BAT_PID;
		subt_detail.si_filter.match_depth = 5/*5*/;
		subt_detail.si_filter.eq_or_neq = EQ_MATCH;
		subt_detail.si_filter.match[0] = BAT_TID;
		subt_detail.si_filter.match[3] = 0x60;
		subt_detail.si_filter.match[4] = 0x65;
		subt_detail.si_filter.mask[0] = 0xff;
		subt_detail.si_filter.mask[3] = 0xff;
		subt_detail.si_filter.mask[4] = 0xff;
		params_create = &subt_detail;

		subt_detail.table_parse_cfg.mode = PARSE_PRIVATE_ONLY;
		subt_detail.table_parse_cfg.table_parse_fun = app_table_bat_search_section_parse;

		app_send_msg(GXMSG_SI_SUBTABLE_CREATE,(void*)&params_create);

		s_BatSubtId = subt_detail.si_subtable_id;
		s_BatRequestId = subt_detail.request_id;
				
		memset(pBatSectionFlag,0,256);
		// start si
		params_start = s_BatSubtId;
		app_send_msg(GXMSG_SI_SUBTABLE_START, (void*)&params_start);

	}



}

void app_table_bat_get_filter_info(int32_t* pBatSubtId,uint32_t* pBatRequestId)
{
	if ((NULL == pBatSubtId)||(NULL == pBatRequestId))
		return;

	*pBatSubtId = s_BatSubtId ;
	*pBatRequestId = s_BatRequestId;
	return;
}



private_parse_status app_table_bat_search_section_parse(uint8_t *p_section_data, size_t len)
{
	uint8_t *date, *date1;
	int16_t len1 = 0;
	int16_t len2 = 0;
	int16_t len3 = 0;
	int16_t len4 = 0;
	uint8_t i = 0;
	uint16_t m_wStreamId = 0;	

	uint32_t ret = PRIVATE_SECTION_OK;
	
	printf("bat_descriptor_parse \n");

	if ((len>4096)||(len<16))
		{
			printf("bat_descriptor_parse len= %d\n",len);
		}
	date = p_section_data;

	if(date[0] == BAT_TID)
	{
		printf("cur section[%d] last section[%d] \n",date[6],date[7]);
		#if 1
		if(pBatSectionFlag[date[6]] == 1)
		{
			ret = PRIVATE_SECTION_OK;
			return ret;
		}
		else
		{
			pBatSectionFlag[date[6]] = 1;
		}
		
		for(i = 0;i<=date[7];i++)
		{
			if(pBatSectionFlag[i]!=1)
			{
				break;
			}
		}
		
		if(i == (date[7]+1))
		{
			memset(pBatSectionFlag,0,256);
			ret = PRIVATE_SUBTABLE_OK;
		}
		#endif
		len1 = ((date[8]&0xf)<<8)+date[9];
		len2 = ((date[9+len1+1]&0xf)<<8)+date[9+len1+2];
		date +=(9+1+len1+2);
		while(len2>0)
		{
			m_wStreamId = ((date[0]<<8)&0xff00)+date[1];
			len4 = len3 = ((date[4]&0xf)<<8)+date[5];
			date1 = &date[6];
			while(len3>0)
			{
				//printf("lcn_descriptor_parse \n");
				if (NULL != app_table_bat_parse_descriptor_callback)
				{
					app_table_bat_parse_descriptor_callback(date1[0],date1,m_wStreamId);
				}
//				lcn_descriptor_parse(date1[0],&(date1[0]),date1[1]);
				len3 -= (date1[1]+2);
				date1 += (date1[1]+2);
			}
			date +=(6+len4);
			len2 -=(6+len4);
		}
		
	}
	printf("bat_descriptor_parse ret= %d\n",ret);
	return ret;
}


void app_table_bat_register_parse_descriptor_callback(bat_parse_descriptor bat_parse_descriptor_call_back)
{
	if (NULL != bat_parse_descriptor_call_back)
		{
			app_table_bat_parse_descriptor_callback = bat_parse_descriptor_call_back;
		}
	return ;
}







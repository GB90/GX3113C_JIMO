#include "dvb_ads_drv.h"
#include "dvb_ads_os_porting.h"

extern BOOL add_list_data( ads_data_node** p_node, ads_pic_attribute data);
extern ads_data_node* create_list_data(ads_pic_attribute data);
extern void delete_node(ads_data_node** pNode);
extern void dvb_set_ad_flag(get_ad_flag flag);
extern RET_CODE save_all_pic_to_dram(void);
extern RET_CODE ads_xml_parse(const xmlChar *p_buf, u32 buf_len, dvbad_ads_pic_data *p_dvbad_ads_pic_attribute_data);

//BOOL dvb_ads_version_need_update_flag = TRUE;
dvbad_priv_t s_dvbad;
ad_save_info_t ad_save_info;
ad_pic_filter_flag_t* ad_pic_filter_flag = NULL;
extern ads_module_priv_t *p_dvbad_priv;

dvb_mempool * dvbad_get_memp(void)
{
	return &s_dvbad.mem;
}  

static void exit_monitor(get_ad_flag flag)
{	
	DVB_ADS_DEBUG(DVBAD, NOTICE, "exit get ad monitor\n");
	dvb_set_ad_flag(flag);
	//Two times of excuting filter control func and then exit!!!
	s_dvbad.monitor_count = 2;
	dvb_ads_thread_destroy();
	DVB_ADS_DEBUG(DVBAD,INFO,"END the thread!!!");
}
#if 0
static RET_CODE add_data_to_list(ads_data_node **p_list, ads_pic_attribute pic_attr)
{
	ads_data_node *p_node;
	ads_data_node *p_data_node = NULL;
	if((NULL==p_list)||(NULL==*p_list))
	{
		DVB_ADS_DEBUG(DVBAD, INFO, "null == p_list\n");
		return ERR_FAILURE;
	}
	p_data_node = create_list_data(pic_attr);
	p_node = *p_list;
	while(NULL!=p_node->next)
	{
		p_node = p_node->next;
	}
	DVB_ADS_DEBUG(DVBAD, INFO, "type:%d\n", p_node->ads_pic_attribute_data.type);
	p_node->next = p_data_node;
	return SUCCESS;
}


static RET_CODE add_first_data_to_list(ads_data_node *p_list, ads_pic_attribute attr_data)
{
	if(NULL == p_list)
	{
		DVB_ADS_DEBUG(DVBAD, INFO, "null == p_list\n");
		return ERR_FAILURE;
	}
	memcpy(&p_list->ads_pic_attribute_data, &attr_data, sizeof(ads_pic_attribute));
	return SUCCESS;
}

static RET_CODE dvbad_set_sum(char *p_data, u32 data_len)
{
	u32 i;
	u8 sum = 0;
	if(NULL==p_data)
	{
		DVB_ADS_DEBUG(DVBAD, ERR, "NULL == p_data\n");
		return ERR_FAILURE;
	}
	for(i = 0;i<data_len-1;i++)
	{
		sum += p_data[i];
	}
	p_data[data_len-1] = sum;
	return SUCCESS;
}

static RET_CODE dvbad_check_sum(char *p_data, u32 data_len)
{
	u32 i;
	u8 sum = 0;
	if(NULL==p_data)
	{
		DVB_ADS_DEBUG(DVBAD, ERR, "null == p_data\n");
		return ERR_FAILURE;
	}
	
	for(i = 0; i<data_len-1; i++)
	{
		sum += p_data[i];
	}
	if(sum == p_data[data_len-1])
	{
		return SUCCESS;
	}
	return ERR_FAILURE;
}

static ads_data_node *dvbad_get_node_by_id(ads_data_node* p_data, u16 id)
{
	ads_data_node *p = p_data;
	while (id != p->ads_pic_attribute_data.file_number)
	{
		if (NULL== p->next)
		{
			DVB_ADS_DEBUG(DVBAD,ERR, "ERROR!(NULL== *p->dvbad_pic_attribute_node)\n");
			return NULL;
		}
		p = p->next;
	}
	return p;
}
#endif

static RET_CODE dvb_ads_get_logo_num(u16 *num)
{
	dvbad_priv_t *ad = &s_dvbad;
	ads_data_node *p_node = &(ad->p_ad_data->dvbad_pic_attribute_node);

	while(DVBAD_STB_LOG != p_node->ads_pic_attribute_data.type)
	{
		if(NULL==p_node->next)
		{
			return ERR_FAILURE;
		}
		p_node = p_node->next;
	}
	*num = p_node->ads_pic_attribute_data.file_number;
	DVB_ADS_DEBUG(DVBAD,INFO,"p_nodei:file_number = %d x=%d y=%d w = %d h=%d duration=%d version=%d,pos_type=%d\n",\
		 p_node->ads_pic_attribute_data.file_number ,p_node->ads_pic_attribute_data.x,p_node->ads_pic_attribute_data.y,\
		 p_node->ads_pic_attribute_data.w,p_node->ads_pic_attribute_data.h,p_node->ads_pic_attribute_data.duration,\
		 p_node->ads_pic_attribute_data.version,p_node->ads_pic_attribute_data.type);
	return SUCCESS;
}

ads_data_node *dvbad_get_data_node_by_serviceid(ads_data_node *p_node, DVBAD_POS_TYPE ad_type, u32 service_id)
{
	ads_data_node *p = p_node;
	int count = 0;
	int chose = 0;

	if(NULL == p_node)
	{
		DVB_ADS_DEBUG(DVBAD, ERR, "null==p_node\n");
		return NULL;
	}
	DVB_ADS_DEBUG(DVBAD, INFO, "service_id:%u\n",(unsigned int)service_id);

	while (p->next != NULL)
	{
		if(ad_type == p->ads_pic_attribute_data.type)
		{
			if(service_id == (u16)atoi((char*)p->ads_pic_attribute_data.type_name))
			{
				count ++;
			}
		}
		p = p->next;
	}
	if (count == 0)
		return NULL;


	chose = rand()%count; /*% means 0~count-1*/
	chose += 1; /*% means 1~count*/
	DVB_ADS_DEBUG(DVBAD, ERR, "count:%d chose:%d\n", count,chose);	
	count = 0;
	p = p_node;
	while (p->next != NULL)
	{
		if(ad_type == p->ads_pic_attribute_data.type)
		{
			if(service_id==(u16)atoi((char*)p->ads_pic_attribute_data.type_name))
				count ++;
		}

		if (count == chose)
			return p;
		p = p->next;
	}
	return NULL;
}
//extern void sys_status_get_utc_time(utc_time_t *p_time);

ads_data_node *dvbad_get_data_node(ads_data_node *p_node, DVBAD_POS_TYPE ad_type)
{
	ads_data_node *p = p_node;
	int count = 0;
	int chose = 0;

	if(NULL==p_node)
	{
		DVB_ADS_DEBUG(DVBAD, ERR, "null==p_node\n");
		return NULL;
	}
	while (p->next != NULL)
	{
		if (p->ads_pic_attribute_data.type == ad_type)
			count ++;
		p = p->next;
	}
	if (count == 0)
		return NULL;

	if (DVBAD_STB_LOG == ad_type)
	{
		//NOTE
		#if 0
		utc_time_t time;
		u32 sec;
		//sys_status_get_utc_time(&time);
		sec = time.day * 24*60*60
			+ time.hour * 60*60
			+ time.minute *60;
			+ time.second;
		chose = mtos_ticks_get()%count;
		#endif
	}
	else
	{
		chose = rand()%count;
	}
	chose += 1; /*% means 0~count-1*/
	DVB_ADS_DEBUG(DVBAD, ERR, "count:%d chose:%d\n", count,chose);	
	DVB_ADS_DEBUG(DVBAD, INFO, "get data node now.ad_type:%d\n", ad_type);
	count = 0;
	p = p_node;
	while (p->next != NULL)
	{
		if (p->ads_pic_attribute_data.type == ad_type)
			count ++;
		if (count == chose)
			return p;
		p = p->next;
	}
	return NULL;

}

RET_CODE dvbad_get_logo_flag(void)
{
	dvbad_priv_t *ad = &s_dvbad;
	ads_data_node *p_node = &(ad->p_ad_data->dvbad_pic_attribute_node);
	
	p_node =  dvbad_get_data_node(p_node, DVBAD_STB_LOG);
	if(NULL!=p_node)
	{
		return SUCCESS;
	}
	return ERR_FAILURE;
}


/*return logo size*/
u32 dvbad_get_logo(u8 *p_log_data, u32 size)
{
	dvbad_priv_t *ad = &s_dvbad;
	u32 len;
	ads_data_node *p_node = &(ad->p_ad_data->dvbad_pic_attribute_node);

	if(NULL==p_log_data)
		return 0;
	p_node = dvbad_get_data_node(p_node, DVBAD_STB_LOG);
	if(NULL == p_node)
	{
		return 0;
	}

	len = p_node->p_pic_info.pic_original_size;
	if (len > size)
		len = size;
	memcpy(p_log_data, p_node->p_ads_pic_data, len);
	DVB_ADS_DEBUG(DVBAD, INFO, "get logo end\n");
	return len;
}

RET_CODE dvbad_get_ad_by_type_name(dvbad_show_pic_info *p_pic,char* type_name)
{
	dvbad_priv_t *ad = &s_dvbad;
	ads_data_node *p_node = &(ad->p_ad_data->dvbad_pic_attribute_node);
	
	if((NULL == type_name) || (p_pic == NULL))
		return ERR_FAILURE;
	
	while (p_node->next != NULL)
	{
		if (0 == strcmp((char*)p_node->ads_pic_attribute_data.type_name,(char*)type_name))
			break;
		p_node = p_node->next;
	}
	if(NULL == p_node)
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"Get the sub menu type name = %s failed!!!\n",type_name);
		return ERR_FAILURE;
	}
	p_pic->data_len = p_node->p_pic_info.pic_original_size;
	p_pic->h = p_node->ads_pic_attribute_data.h;
	p_pic->w = p_node->ads_pic_attribute_data.w;
	p_pic->x = p_node->ads_pic_attribute_data.x;
	p_pic->y = p_node->ads_pic_attribute_data.y;
	p_pic->pic_id = p_node->ads_pic_attribute_data.file_number;
	p_pic->pic_type = p_node->ads_pic_attribute_data.p_format;
	p_pic->pos_type = p_node->ads_pic_attribute_data.type;
	memcpy((char*)p_pic->type_name,(char*)p_node->ads_pic_attribute_data.type_name,sizeof((char*)p_node->ads_pic_attribute_data.type_name));
	p_pic->pic_data = dvb_ads_mempool_alloc(&s_dvbad.mem,p_node->p_pic_info.pic_original_size);
	memcpy((void*)p_pic->pic_data,(void*)p_node->p_ads_pic_data,p_node->p_pic_info.pic_original_size);
	DVB_ADS_DEBUG(DVBAD,INFO,"PIC data get info:x:%d y:%d w:%d h:%d pos_type:%d pic_type:%d pic_id:%d data_len=%u type_name=%s\n",\
		     p_pic->x,p_pic->y,p_pic->w,p_pic->h,p_pic->pos_type,p_pic->pic_type,p_pic->pic_id,(unsigned int)p_pic->data_len,(char *)p_pic->type_name);
	return SUCCESS;

}

RET_CODE dvbad_get_ad(IN dvbad_program_para* para, dvbad_show_pic_info *p_pic, DVBAD_POS_TYPE ad_type)
{
	dvbad_priv_t *ad = &s_dvbad;
	ads_data_node *p_node = &(ad->p_ad_data->dvbad_pic_attribute_node);
	
	if((NULL != para) && (para->usServiceId >= 0))
	{	
		if((DVBAD_STB_CURRENT_FOLLOW == ad_type)||(DVBAD_STB_CHANNEL_INFO == ad_type))
		{
			DVB_ADS_DEBUG(DVBAD, INFO, "The current program usServiceId:%d\n", para->usServiceId);
			p_node = dvbad_get_data_node_by_serviceid(p_node, ad_type, (u32)para->usServiceId);
			if(NULL == p_node)
			{
				//if the service_id of the program is not match then get the service_id = 0 porgram related ads!!!
				//That means no matter what service id of the program is that we should show the ads if program exit in the stream!!!
				//NOTE: The test stream is not containing program ,so no program related ads will be shown!!!
				p_node = &(ad->p_ad_data->dvbad_pic_attribute_node);
				p_node = dvbad_get_data_node_by_serviceid(p_node, ad_type, (u32)0);
				if(NULL == p_node)
				{
					DVB_ADS_DEBUG(DVBAD,INFO,"Get node of service_id = %d failed!!!\n",para->usServiceId);
					DVB_ADS_DEBUG(DVBAD,INFO,"Type of %d in current program not exit!!!\n",ad_type);
					return ERR_FAILURE;
				}
			}
		}
	}
	else
	{
		p_node = dvbad_get_data_node(p_node, ad_type);
	}
	if(NULL == p_node)
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"Type of %d in current program not exit!!!\n",ad_type);
		return ERR_FAILURE;
	}
	DVB_ADS_DEBUG(DVBAD,INFO,"Type of ad : %d in current program exit!!!\n",ad_type);
	if(FINISH == p_node->status)
	{
		p_pic->data_len = p_node->p_pic_info.pic_original_size;
		p_pic->h = p_node->ads_pic_attribute_data.h;
		p_pic->w = p_node->ads_pic_attribute_data.w;
		p_pic->x = p_node->ads_pic_attribute_data.x;
		p_pic->y = p_node->ads_pic_attribute_data.y;
		p_pic->pic_id = p_node->ads_pic_attribute_data.file_number;
		p_pic->pic_type = p_node->ads_pic_attribute_data.p_format;
		p_pic->pos_type = p_node->ads_pic_attribute_data.type;
		p_pic->service_id = (u16)atoi((char*)p_node->ads_pic_attribute_data.type_name);
		p_pic->pic_data = dvb_ads_mempool_alloc(&s_dvbad.mem,p_node->p_pic_info.pic_original_size);
		memcpy((void*)p_pic->pic_data,(void*)p_node->p_ads_pic_data,p_node->p_pic_info.pic_original_size);
		DVB_ADS_DEBUG(DVBAD,INFO,"PIC data get info:x:%d y:%d w:%d h:%d pos_type:%d pic_type:%d pic_id:%d data_len:%u service_id=%d\n",\
			p_pic->x,p_pic->y,p_pic->w,p_pic->h,p_pic->pos_type,p_pic->pic_type,p_pic->pic_id,(unsigned int)p_pic->data_len,p_pic->service_id);
	/*
	p_pic->pic_data= mtos_malloc(p_pic->data_len);
	memcpy(p_pic->pic_data, p_ad->p_ads_pic_data, p_pic->data_len);
	*/
		
	//	DVB_ADS_DEBUG(DVBAD, INFO,"\n\t\t\tFor current program:\n\t\t\tpic_id:\t\t\t%u\n\t\t\tpic_type:\t\t%u\n\t\t\tlen:\t\t\t%u\n\t\t\tpos_type:\t\t%u\n\t\t\tservice_id:\t\t%u\n\t\t\tx:\t\t\t%u\n\t\t\ty:\t\t\t%u\n\t\t\tw:\t\t\t%u\n\t\t\th:\t\t\t%u",(unsigned int)p_pic->pic_id,(unsigned int)p_pic->pic_type,(unsigned int)p_pic->data_len,(unsigned  int)p_pic->pos_type,(unsigned int)p_pic->service_id,(unsigned int)p_pic->x,(unsigned int)p_pic->y,(unsigned int)p_pic->w,(unsigned int)p_pic->h);
	
		return SUCCESS;
	}
	else{
		DVB_ADS_DEBUG(DVBAD,INFO,"The ad_type = %d is not filter completed!!!",ad_type);
		return ERR_FAILURE;		
	}
}

static BOOL ads_save_pic_info(ads_data_node *p_ads_pic_data_node, ads_pic_infomation *data)
{
	ads_data_node *p = p_ads_pic_data_node;
	if(NULL == p || NULL == data)
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"args passed in ERR!!!");
	}
	DVB_ADS_DEBUG(DVBAD,INFO,"data->pic_id = %u,data->pic_num = %u,data->pic_original_size = %u",
						(unsigned int)data->pic_id,
						(unsigned int)data->pic_num,
						(unsigned int)data->pic_original_size);
	while(data->pic_id != p->ads_pic_attribute_data.file_number)
	{
		if (NULL== p->next)
		{
			DVB_ADS_DEBUG(DVBAD,ERR, "find no node of the pic_id = %d in the global var list of ads_data_node.ads_pic_attribute_data.file_number\n",data->pic_id);
			return FALSE;
		}
		p = p->next;
	}
	DVB_ADS_DEBUG(DVBAD,INFO,"Find p->ads_pic_attriute_data.file_number = %u",p->ads_pic_attribute_data.file_number);
	DVB_ADS_DEBUG(DVBAD,INFO,"The pic_id = %d has been saved!!!",data->pic_id);
	memcpy(&p->p_pic_info,data, sizeof(ads_pic_infomation));
	DVB_ADS_DEBUG(DVBAD,INFO,"++++Saved pic_id = %u,pic_size = %u\n",(unsigned int)p->ads_pic_attribute_data.file_number,(unsigned int)p->p_pic_info.pic_original_size);
	return TRUE;
}

static RET_CODE ads_pic_info_parser(u8 *data, ads_data_node *ads_pic_data_node)
{
#define ONE_PIC_INFO_LENGTH		(19)
	u16 pic_num = (((u16)data[0]<<8)|(u16)data[1])&0xFFFF;
	u16 i;
	ads_pic_infomation pic_info;
	DVB_ADS_PRINTF("\n");
	DVB_ADS_DEBUG(DVBAD,INFO,"pic_info.pic_num = %d\n",pic_num);
	//parse the info of the destriptor
	for(i = 0; i<pic_num;i++)
	{
		memset(&pic_info, 0, sizeof(ads_pic_infomation));
		pic_info.pic_num = (((u16)data[0]<<8)|(u16)data[1])&0xFFFF;
		pic_info.pic_id = (((u16)data[2+(ONE_PIC_INFO_LENGTH*i)]<<8|
								(u16)data[3+(ONE_PIC_INFO_LENGTH*i)]))&0xFFFF;
		pic_info.pic_original_size = (((u32)data[4+(ONE_PIC_INFO_LENGTH*i)]<<24)|
								((u32)data[5+(ONE_PIC_INFO_LENGTH*i)]<<16)|
								((u32)data[6+(ONE_PIC_INFO_LENGTH*i)]<<8)|
								((u32)data[7+(ONE_PIC_INFO_LENGTH*i)]))&0xFFFFFFFF;
		pic_info.pic_segment_length = (((u16)data[12+(ONE_PIC_INFO_LENGTH*i)]<<8)|
								((u16)data[13]+(ONE_PIC_INFO_LENGTH*i)))&0xFFFF;
		pic_info.pic_segment_num = (((u16)data[14+(ONE_PIC_INFO_LENGTH*i)]<<8)|
								((u16)data[15+(ONE_PIC_INFO_LENGTH*i)]))&0xFFFF;
		pic_info.pic_compressed_flag = data[16+(ONE_PIC_INFO_LENGTH*i)]>>7&0x01;
		pic_info.pic_compressed_size= (((u32)data[17+(ONE_PIC_INFO_LENGTH*i)]<<24)|
								((u32)data[18+(ONE_PIC_INFO_LENGTH*i)]<<16)|
								((u32)data[19+(ONE_PIC_INFO_LENGTH*i)]<<8)|
								((u32)data[20+(ONE_PIC_INFO_LENGTH*i)]));
		DVB_ADS_PRINTF("\n");
		DVB_ADS_DEBUG(DVBAD,INFO,"pic_info.pic_id = %u,pic_info.pic_num = %u,pic_info.pic_original_size = %u,pic_segment_num = %u",
						(unsigned int)pic_info.pic_id,
						(unsigned int)pic_info.pic_num,
						(unsigned int)pic_info.pic_original_size,(unsigned int)pic_info.pic_segment_num);

		if (FALSE == ads_save_pic_info(ads_pic_data_node, &pic_info))
		{
			DVB_ADS_DEBUG(DVBAD, ERR, "ads_save_pic_info return FALSE\n");
			return FALSE;
		}
	}		
	return TRUE;
}

static void dvbad_get_parser_down_ctrl_info(u8* p_data, u32 data_len, down_ctrl_info *ads_down_ctrl_info)
{
#define XML_TABLE_ID_START	8	
	DVB_ADS_ASSERT((NULL!=p_data)&&(NULL!=ads_down_ctrl_info));
	ads_down_ctrl_info->ads_xml_info.index_xml_table_id_start= 
							(((u16)p_data[XML_TABLE_ID_START]<<8)|(u16)p_data[XML_TABLE_ID_START+1])&0xFFFF;
	ads_down_ctrl_info->ads_xml_info.index_xml_table_id_end= 
							(((u16)p_data[XML_TABLE_ID_START+2]<<8)|(u16)p_data[XML_TABLE_ID_START+3])&0xFFFF;
	ads_down_ctrl_info->ads_xml_info.index_xml_size= 
							(((u32)p_data[XML_TABLE_ID_START+4]<<24)|((u32)p_data[XML_TABLE_ID_START+5]<<16)|
							((u32)p_data[XML_TABLE_ID_START+6]<<8)|(u32)p_data[XML_TABLE_ID_START+7])&0xFFFFFFFF;
	ads_down_ctrl_info->ads_xml_info.index_xml_size++;
	ads_down_ctrl_info->ads_pic_info.pic_information_table_id_start= 
							(((u16)p_data[XML_TABLE_ID_START+8]<<8)|(u16)p_data[XML_TABLE_ID_START+9])&0xFFFF;
	ads_down_ctrl_info->ads_pic_info.pic_information_table_id_end= 
							(((u16)p_data[XML_TABLE_ID_START+10]<<8)|(u16)p_data[XML_TABLE_ID_START+11])&0xFFFF;
	ads_down_ctrl_info->ads_pic_info.pic_information_size= 
							(((u32)p_data[XML_TABLE_ID_START+12]<<24)|((u32)p_data[XML_TABLE_ID_START+13]<<16)|
							((u32)p_data[XML_TABLE_ID_START+14]<<8)|(u32)p_data[XML_TABLE_ID_START+15])&0xFFFFFFFF;
	DVB_ADS_DEBUG(DVBAD,INFO,"xml_table_id_start = %u,xml_table_id_end = %u,xml_size = %u",(unsigned int)ads_down_ctrl_info->ads_xml_info.index_xml_table_id_start,(unsigned int)ads_down_ctrl_info->ads_xml_info.index_xml_table_id_end,(unsigned int)ads_down_ctrl_info->ads_xml_info.index_xml_size);
	DVB_ADS_DEBUG(DVBAD,INFO,"pic_info_table_id_start = %u,pic_info_table_id_end = %u,pic_info_size = %u",(unsigned int)ads_down_ctrl_info->ads_pic_info.pic_information_table_id_start,(unsigned int)ads_down_ctrl_info->ads_pic_info.pic_information_table_id_end,(unsigned int)ads_down_ctrl_info->ads_pic_info.pic_information_size);

}

static RET_CODE ads_save_pic_data(ads_data_node *p_ads_pic_data_node, u8 *p_data, u32 pic_data_len, u16 pid)
{
	ads_data_node *p = p_ads_pic_data_node;

	while (pid != p->ads_pic_attribute_data.file_number)
	{
		if (NULL == p->next)
		{
			DVB_ADS_DEBUG(DVBAD,ERR, "ERROR!(NULL== *p->dvbad_pic_attribute_node)\n");
			return ERR_FAILURE;
		}
		p = p->next;
	}

	p->p_ads_pic_data = dvb_ads_mempool_alloc(&s_dvbad.mem,pic_data_len);
	if (NULL == p->p_ads_pic_data)
	{
		DVB_ADS_DEBUG(DVBAD,ERR, "alloc from memp failed!\n");
		return ERR_FAILURE;
	}
	memcpy(p->p_ads_pic_data, p_data, pic_data_len);
	/*this pic filter data has finished*/
	p->status = FINISH;
	DVB_ADS_DEBUG(DVBAD, INFO, "save pic data end.pic_data_len:%u, id:%u, service id = %d\n",(unsigned int)pic_data_len,(unsigned int)p->ads_pic_attribute_data.file_number,(u16)atoi((char*)p->ads_pic_attribute_data.type));
	return SUCCESS;
}


static RET_CODE ads_get_pic_data(u8 *p_data, u8 *p_pic_data,IN OUT u32 *pic_data_len)
{
	u32 data_len = *pic_data_len;

	if(NULL == p_data)
	{
		DVB_ADS_DEBUG(DVBAD, ERR, "NULL==p_data.ERROR\n");
		return FALSE;
	}
	*pic_data_len = ((u16)(p_data[9]&0x0F)<<8|(u16)p_data[10]);
	DVB_ADS_DEBUG(DVBAD,INFO,"The sec to be saved  table_id = 0x%x,data_len = %u\n",p_data[0],(unsigned int)*pic_data_len);
	memcpy(p_pic_data+data_len, p_data+11,  *pic_data_len);
	*pic_data_len += data_len;
	DVB_ADS_DEBUG(DVBAD, INFO, "After this sec save end ,total size of the pic is %u\n",(unsigned int)*pic_data_len);
	return TRUE;
}

static filter_buf_node  *create_filter_buf_list(u8 *p_data, u32 data_len)
{
	filter_buf_node *p_node = NULL;
	
	if((data_len<=0)||(NULL==p_data))
	{
		DVB_ADS_DEBUG(DVBAD, ERR, "Error p_data:%s, data_len:%u!!!\n", (char*)p_data, (unsigned int)data_len);
		return NULL;
	}
	DVB_ADS_DEBUG(DVBAD,INFO,"Alloc mem for filter_buf_node ,size : %d",sizeof(filter_buf_node));
   	p_node = dvb_ads_mempool_alloc(&s_dvbad.mem,sizeof(filter_buf_node));
	if (p_node != NULL)
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"----Alloc mem for filter_buf_node success,point to: 0x%x!!!",(unsigned int)p_node);
		DVB_ADS_DEBUG(DVBAD,INFO,"Alloc mem for filter_buf_node.p_data,size = %u",(unsigned int)data_len);
		p_node->p_data = dvb_ads_mempool_alloc(&s_dvbad.mem,data_len);
		p_node->data_len = data_len;
		if(NULL == p_node->p_data)
		{
			DVB_ADS_DEBUG(DVBAD, ERR, "Alloc mem for filter_buf_node.p_data failed,free the filter_buf_node mem!!!\n");
			dvb_ads_mempool_free(&s_dvbad.mem,(void*)p_node,sizeof(filter_buf_node));
			return NULL;
		}
		DVB_ADS_DEBUG(DVBAD,INFO,"----filter_buf_node.p_data point to :0x%x",(unsigned int)p_node->p_data);
		DVB_ADS_DEBUG(DVBAD,INFO,"Data saved to the filter_buf_node.p_data mem");
		memcpy(p_node->p_data, p_data, data_len);
		p_node->data_len = data_len;
	    	p_node->next = NULL;  
	}
	DVB_ADS_DEBUG(DVBAD,INFO,"filter buffer list created!!!");
    return p_node;
}

static void delete_head_node(filter_buf_node** pNode)  
{

	DVB_ADS_ASSERT((pNode != NULL)&&(NULL!=* pNode));
	DVB_ADS_DEBUG(DVBAD,INFO,"Start to free the filter buf node");
	dvb_ads_mempool_free(&s_dvbad.mem,(void*)(*pNode)->p_data,(*pNode)->data_len);

	if ((*pNode)->next)
	{
		delete_head_node(&(*pNode)->next); 
	}
	dvb_ads_mempool_free(&s_dvbad.mem,(void*)(*pNode),sizeof(filter_buf_node));   
	DVB_ADS_DEBUG(DVBAD,INFO,"Free one filter_buf_node list");
}

static RET_CODE add_filter_data(filter_buf_node** p_node, u8 *p_data, u32 data_len)
{  
	filter_buf_node* p_data_node = NULL;
	filter_buf_node *p_add_node;
	if((NULL==p_node)||(NULL==*p_node))
	{
		DVB_ADS_DEBUG(DVBAD, ERR, "null!!\n");
		return ERR_FAILURE;
	}

	p_data_node = create_filter_buf_list(p_data, data_len);
	if (p_data_node == NULL)
	{
		//mtos_printk("add_filter_data alloc failure!\n");
		DVB_ADS_ASSERT((NULL != p_data_node));  
		return ERR_FAILURE;
	}

	DVB_ADS_ASSERT((NULL != p_data_node));  
	p_add_node = *p_node;
	while(NULL != p_add_node->next)
		p_add_node = p_add_node->next;
	
	p_add_node->next = p_data_node;
	return SUCCESS;
}  

static RET_CODE add_data_to_filter_list(filter_buf_node** p_node, u8 *p_data, u32 data_len)
{
	if(NULL==*p_node)
	{
		DVB_ADS_DEBUG(DVBAD, INFO, "add new p_data\n");
		*p_node = create_filter_buf_list(p_data, data_len);
		DVB_ADS_ASSERT((NULL!=*p_node));
		return SUCCESS;
	}
	return add_filter_data(p_node, p_data, data_len);
}

static filter_buf_node *get_next_head_node(filter_buf_node *p_list, u8 old_section_number,
												u8 old_data_type, u16 old_extension_id)
{
	u8 section_number;
	u8 data_type;
	u16 table_extension_id;
	filter_buf_node *p_node;

	p_node = p_list;
	while(NULL!=p_node)
	{
		table_extension_id = (((u16)p_node->p_data[3]<<8)|((u16)p_node->p_data[4]));
		section_number = p_node->p_data[6];
		data_type = p_node->p_data[8];
		if((table_extension_id==old_extension_id)&&(section_number==old_section_number)&&
										(data_type==old_data_type))
		{
			DVB_ADS_DEBUG(DVBAD, INFO, "data_type:%d,table_extension_id:%d,section_number:%d", data_type, table_extension_id, section_number);
			return p_node;
		}
		p_node = p_node->next;
	}

	return NULL;
}



static u32 get_one_pic_size(u16 pic_pid, ads_data_node *p_node)
{
	ads_data_node *p = p_node;
	while(NULL != p)
	{
		if(pic_pid == p->ads_pic_attribute_data.file_number)
		{
			DVB_ADS_DEBUG(DVBAD, INFO, "get pic size end:%u\n", (unsigned int)(p->p_pic_info.pic_original_size));
			return p->p_pic_info.pic_original_size;
		}
		p = p->next;
	}
	return 0;
}
#if 0
static filter_buf_node* get_data_node_by_id(filter_buf_node *p_list, u16 pic_id)
{
	u16 data_pic_id;
	filter_buf_node *p_node = p_list;;
	while(NULL != p_list)
	{
		data_pic_id = (((u16)p_node->p_data[3]<<8)|((u16)p_node->p_data[4]));
		if(data_pic_id == pic_id)
		{
			DVB_ADS_DEBUG(DVBAD, INFO,"Get the sec list head node of pic_id = %d\n", pic_id);
			return p_node;
		}
		p_node = p_node->next;
	}
	DVB_ADS_DEBUG(DVBAD, ERR, "Have none pic_id node\n");
	return NULL;
}

#endif
#if 0
static filter_buf_node* get_data_node_by_id(filter_buf_node *p_list, u8 section_number, u16 pic_id)
{
	u8 data_section_number;
	u16 data_pic_id;
	filter_buf_node *p_node;

	p_node = p_list;
	while(NULL!=p_node)
	{
		data_pic_id = (((u16)p_node->p_data[3]<<8)|((u16)p_node->p_data[4]));
		data_section_number = p_node->p_data[6];
		if((data_pic_id==pic_id)&&(data_section_number==section_number))
		{
			DVB_ADS_DEBUG(DVBAD, INFO, "pic_id:%d,section_number:%d\n", pic_id, section_number);
			return p_node;
		}
		p_node = p_node->next;
	}
	DVB_ADS_DEBUG(DVBAD, ERR, "Have none pic_id node\n");
	return NULL;
}
#endif
//---------------------------------------------------------------

static filter_recoder_t   filter_rec[ADS_MAX_FLITER_REC];

static void init_fl(void)
{
	int i;
	for (i=0;i<ADS_MAX_FLITER_REC;i++)
	{
		filter_rec[i].pic_id = INVALID_PIC_ID;
		filter_rec[i].complete = 0;		
		filter_rec[i].node = NULL;
	}
	
}
static filter_buf_node *get_fl(u16 pic_id)
{
	int i,unused = -1;
	for (i=0;i<ADS_MAX_FLITER_REC;i++)
	{
		if (filter_rec[i].pic_id == pic_id)
		{
			return filter_rec[i].node;
		}
		if (unused == -1 && filter_rec[i].pic_id == INVALID_PIC_ID)
		{
			unused = i;
		}
	}
	if (unused != -1)
	{
		filter_rec[unused].pic_id = pic_id;
		filter_rec[unused].complete = 0;		
		return filter_rec[unused].node;
	}
	DVB_ADS_ASSERT(0);
	return (INVLAD_NODE_PRT);
	
}

BOOL check_fl_complete(u16 pic_id)
{
	int i,cur_fl = 0;
	u8 sec,lastsec;
	u8 *data = NULL;
	u8 *flag = NULL; 
	filter_buf_node * pn;
	DVB_ADS_DEBUG(DVBAD,INFO,"Start to check ext_id = %d filter completed or not!!!",pic_id);
	for (i=0;i<ADS_MAX_FLITER_REC;i++)
	{
		if (filter_rec[i].pic_id == pic_id)
		{
			break;
		}
	}
	if (i == ADS_MAX_FLITER_REC)
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"ext_id = %d temp saving list not exit!!!",pic_id);
		return TRUE;
	}
	/* here has two for()  and care i, use cur_fl recode current fl id*/
	cur_fl = i;
	DVB_ADS_PRINTF("\n*******************************************************************************************\n");
	
	data = filter_rec[cur_fl].node->p_data;
	sec = data[6];
	lastsec = data[7];
	DVB_ADS_DEBUG(DVBAD,INFO,"For pic_id = %d,last_sec = %d,sec = %d has been saved",pic_id,lastsec,sec);
	DVB_ADS_DEBUG(DVBAD,INFO,"Alloc mem As FLAG for pic_id = %d to record the sec has been saved",pic_id);
	flag = app_porting_ca_os_malloc(lastsec+2);
	if (flag == NULL){
		return FALSE;
	}
	memset(flag,0,lastsec+2);
	pn = filter_rec[cur_fl].node;
	/*set list head section have receiced*/
	flag[sec] = 1;
	while(pn != NULL)
	{
		data = pn->p_data;
		sec = data[6];
		//DVB_ADS_DEBUG(DVBAD, INFO, "sec|last :%d | %d\n", sec,lastsec);
		if (sec > lastsec)
		{
			return FALSE;
		}
		flag[sec] = 1;
		DVB_ADS_DEBUG(DVBAD,INFO,"ext_id = %d,sec = %d has been save to filter list!!!",pic_id,sec);
		pn = pn->next;
	}
	for(i = 0;i <= lastsec; i++)
	{
		if (flag[i] == 0)
		{
			DVB_ADS_DEBUG(DVBAD,INFO,"pic_id = %d,At least sec_num = %d data not filtered out!!!",pic_id,i);
			dvb_ads_mempool_free(&s_dvbad.mem, flag,lastsec+2);
			DVB_ADS_DEBUG(DVBAD,INFO,"free the temp flag,waiting for the next data!!!");
			DVB_ADS_PRINTF("\n*******************************************************************************************\n");
			return FALSE;
		}
	}
	DVB_ADS_DEBUG(DVBAD,INFO,"Pic_id = %d is filtered completed and free the temp flag!!!",pic_id);
	app_porting_ca_os_free(flag);
	DVB_ADS_DEBUG(DVBAD,INFO,"free the temp flag end!!!");
	filter_rec[cur_fl].complete = 1;
	if (filter_rec[cur_fl].complete == 1)
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"Picture of the pic_id = %d fitlered out completly!!!",pic_id);
		DVB_ADS_PRINTF("\n*******************************************************************************************\n");
		return TRUE;
	}
	return FALSE;	
}
static void free_fl(u16 pic_id)
{
	int i;
	for (i=0;i<ADS_MAX_FLITER_REC;i++)
	{
		if (filter_rec[i].pic_id == pic_id)
		{
			if (filter_rec[i].node != NULL)
			{
				DVB_ADS_DEBUG(DVBAD,WARNING,"U forgot to free node list outside?\n");
				delete_head_node(&filter_rec[i].node);
			}
			filter_rec[i].pic_id  = INVALID_PIC_ID;
			filter_rec[i].complete = 0;		
			filter_rec[i].node = NULL;
		}
	}
}

static void free_fl_all(void)
{
	int i;
	for (i=0;i<ADS_MAX_FLITER_REC;i++)
	{
		if (filter_rec[i].node)
		{
			DVB_ADS_DEBUG(DVBAD,INFO,"The data list is not freed outside,free now");
			delete_head_node(&filter_rec[i].node);
			DVB_ADS_DEBUG(DVBAD,INFO,"temp  xml or pic info data filter_rec[%d] list has been freed",i);
		}
		filter_rec[i].pic_id  = INVALID_PIC_ID;
		filter_rec[i].complete = 0;		
		filter_rec[i].node = NULL;
	}
}

static ST_AD_DVBAD_FUNC_TYPE  add_node_to_fl(u16 pic_id,u8 * p_data, u32 data_len)
{
	int i;
	filter_buf_node *p_node;
	for (i=0;i<ADS_MAX_FLITER_REC;i++)
	{
		if (filter_rec[i].pic_id == pic_id)
		{
			break;
		}
	}
	if (i == ADS_MAX_FLITER_REC)
	{
		return DVBAD_FUN_ERR_PARA;
	}
	//check the sec exit or not
	p_node =  filter_rec[i].node;
	while(NULL != p_node)	
	{
		if(p_data[6] == p_node->p_data[6])
		{
			DVB_ADS_DEBUG(DVBAD,INFO,"This sec = %d has been saved aready!!!\n",p_data[6]);
			return DVBAD_FUN_OK;
		}
		p_node = p_node->next;
	}
	add_data_to_filter_list(&filter_rec[i].node, p_data,data_len);
	DVB_ADS_DEBUG(DVBAD,INFO,"add data to filter list success");
	if (check_fl_complete(pic_id))
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"One ext_id data filtered out completely");
		return DVBAD_FUN_ONE_FILTER_COMPELET;
	}
	return DVBAD_FUN_OK;
}

/*
Get all the xml data has been saved together 
*/
static BOOL ads_get_head_xml_data(u8 *p_data, u8 *p_xml_data, u32 *xml_len)
{
	u32 data_len;
	u8 data_type;
	if(NULL==p_data)
	{
		DVB_ADS_DEBUG(DVBAD, ERR, "(NULL==p_data).error");
		return FALSE;
	}

	data_type = p_data[8];
	if(DATA_TYPE_XML == data_type)
	{
		data_len = *xml_len;
		*xml_len = ((u16)p_data[9]<<8|(u16)p_data[10])&0x0FFF;
		memcpy(p_xml_data + data_len, p_data+11,  *xml_len);
		*xml_len += data_len;
	}	
	else 
	{
		DVB_ADS_DEBUG(DVBAD, ERR, "xml wrong data_type :%d!!\n",data_type);
		return FALSE;
	}
	return TRUE;
}

static BOOL ads_get_head_pic_info_data(u8 *p_data, u8 *p_info_data, u32 *info_len)
{
	u32 data_len;
	u8 data_type;
	if(NULL==p_data)
	{
		DVB_ADS_DEBUG(DVBAD, ERR, "(NULL==p_data).error");
		return FALSE;
	}

	data_type = p_data[8];
	if(DATA_TYPE_PIC == data_type)
	{
		data_len = *info_len;
		*info_len = ((u16)p_data[9]<<8|(u16)p_data[10])&0x0FFF;
		memcpy(p_info_data+data_len, p_data+11,  *info_len);
		*info_len += data_len;
	}
	else 
	{
		DVB_ADS_DEBUG(DVBAD, ERR, "pic info wrong data_type :%d!!\n",data_type);
		return FALSE;
	}
	//DVB_ADS_PRINTF("\n");
	return TRUE;
}


static RET_CODE parser_head_xml_data(void)
{
	dvbad_priv_t *ad = &s_dvbad;
	filter_buf_node *p_head;
	filter_buf_node *p_node;
	
	u8 section_number;
	u8 last_section_number;
	u16 table_extension_id;
	u16 old_ext_tid = 0xffff;

	if (ad->xml == NULL)
	{
		ad->xml = dvb_ads_mempool_alloc(&s_dvbad.mem,ad->down_ctl.ads_xml_info.index_xml_size);
		DVB_ADS_DEBUG(DVBAD,INFO,"ad->xml is NULL ,Remalloc,size:ad->down_ctl.ads_xml_info.index_xml_size:%u",(unsigned int)ad->down_ctl.ads_xml_info.index_xml_size);
		if (ad->xml == NULL)
		{
			DVB_ADS_DEBUG(DVBAD, ERR, "lib memp alloc xml failure!\n");		
			return ERR_FAILURE;
		}
		memset((void*)ad->xml, 0, ad->down_ctl.ads_xml_info.index_xml_size);
		DVB_ADS_DEBUG(DVBAD,INFO,"Init ad->xml space all 0");
		ad->xml_pos = 0;
	}	
	section_number = 0;
	table_extension_id = ad->down_ctl.ads_xml_info.index_xml_table_id_start;	
	while(1)
	{
		if (old_ext_tid != table_extension_id)
		{
			DVB_ADS_DEBUG(DVBAD, INFO, "table_extension_id: %d!\n\n\n",table_extension_id);
			p_head = get_fl(table_extension_id);
			old_ext_tid = table_extension_id;
		}
		last_section_number = p_head->p_data[7];
		DVB_ADS_DEBUG(DVBAD, INFO, "sec = %d ,last_sec = %d!\n",section_number,last_section_number);
		
		p_node = get_next_head_node(p_head, section_number, DATA_TYPE_XML, table_extension_id);
		if(NULL==p_node)
		{
			DVB_ADS_DEBUG(DVBAD, ERR, "None of next head node\n");
			goto ERR_OUT;
		}
		if(FALSE==ads_get_head_xml_data(p_node->p_data,ad->xml, &ad->xml_pos))
		{
			DVB_ADS_DEBUG(DVBAD, ERR, "ads get head xml data return FALSE!\n");
			goto ERR_OUT;			
		}
		
		/* confirm next node needed*/		
		last_section_number = p_node->p_data[7];
		/*the same ext table id*/
		if(section_number < last_section_number)
		{
			section_number++;
		} /*next ext table id*/
		else if(table_extension_id < ad->down_ctl.ads_xml_info.index_xml_table_id_end)
		{
			table_extension_id++;
			section_number= 0;
		} /*last ext table id*/
		else if(table_extension_id == ad->down_ctl.ads_xml_info.index_xml_table_id_end)
		{
			
			DVB_ADS_DEBUG(DVBAD, INFO, "sizeof(dvbad_ads_pic_data) %d\n", sizeof(dvbad_ads_pic_data));
			if(NULL==ad->p_ad_data)
			{
				DVB_ADS_DEBUG(DVBAD, ERR, "NULL==ad->p_ad_data\n");
				goto ERR_OUT;	
			}
			ad->xml_pos++;
			if(SUCCESS != ads_xml_parse(ad->xml, ad->xml_pos, ad->p_ad_data))
			{
				DVB_ADS_DEBUG(DVBAD, ERR, "ads_xml_parsefailure\n");
				goto ERR_OUT;	
			}
			DVB_ADS_DEBUG(DVBAD, INFO, "ext_id:%d,type:%d,x:%d,y:%d,w:%d,h:%d\n",
							ad->p_ad_data->dvbad_pic_attribute_node.ads_pic_attribute_data.file_number,
							ad->p_ad_data->dvbad_pic_attribute_node.ads_pic_attribute_data.type,
							ad->p_ad_data->dvbad_pic_attribute_node.ads_pic_attribute_data.x,
							ad->p_ad_data->dvbad_pic_attribute_node.ads_pic_attribute_data.y,
							ad->p_ad_data->dvbad_pic_attribute_node.ads_pic_attribute_data.w,
							ad->p_ad_data->dvbad_pic_attribute_node.ads_pic_attribute_data.h);
			/*if sucess memory of xml need hold*/
			return SUCCESS;
		}
	}

ERR_OUT:
	dvb_ads_mempool_free(&s_dvbad.mem,ad->xml,ad->down_ctl.ads_xml_info.index_xml_size);
	ad->xml= NULL;	
	return ERR_FAILURE;
}



static RET_CODE parser_head_pic_info_data(void)
{
	dvbad_priv_t *ad = &s_dvbad;
	filter_buf_node *p_head;
	filter_buf_node *p_node;
	u8 section_number;
	u8 last_section_number;
	u16 table_extension_id;
	u16 old_ext_tid = 0xffff;
	

	if (ad->pic_info == NULL)
	{
		ad->pic_info = dvb_ads_mempool_alloc(&s_dvbad.mem,ad->down_ctl.ads_pic_info.pic_information_size);
		if (ad->pic_info == NULL)
		{
			DVB_ADS_DEBUG(DVBAD, ERR, "alloc pic_info memory failure!\n");	
			return ERR_FAILURE;
		}
		memset((void*)ad->pic_info, 0, ad->down_ctl.ads_pic_info.pic_information_size);
		ad->pic_info_pos = 0;
	}
	section_number = 0;
	table_extension_id = ad->down_ctl.ads_pic_info.pic_information_table_id_start;
	while(1)
	{
		if (old_ext_tid != table_extension_id)
		{
			DVB_ADS_DEBUG(DVBAD, INFO, "table_extension_id: %d!\n\n\n",table_extension_id);
			p_head = get_fl(table_extension_id);
			old_ext_tid = table_extension_id;
		}
		last_section_number = p_head->p_data[7];
		DVB_ADS_DEBUG(DVBAD, INFO, "pic_info for current ext_id:%d\tcur_sec|last_sec: %d | %d!\n",table_extension_id,section_number,last_section_number);	
		
		p_node = get_next_head_node(p_head, section_number, DATA_TYPE_PIC, table_extension_id);
		if(NULL == p_node)
		{
			DVB_ADS_DEBUG(DVBAD, ERR, "None of next head node\n");				
			goto ERR_OUT;
		}
		if(FALSE == ads_get_head_pic_info_data(p_node->p_data,ad->pic_info, &ad->pic_info_pos))
		{
			DVB_ADS_DEBUG(DVBAD, ERR, "ads get head pic info data return FALSE!\n");
			goto ERR_OUT;
		}
		/* confirm next node needed*/
		last_section_number = p_node->p_data[7];
		/*the same ext table id*/		
		if(section_number<last_section_number)
		{
			section_number++;
		} /*next ext table id*/
		else if(table_extension_id  <  ad->down_ctl.ads_pic_info.pic_information_table_id_end)
		{
			table_extension_id++;
			section_number= 0;
		} /*last ext table id*/
		else if(table_extension_id  ==  ad->down_ctl.ads_pic_info.pic_information_table_id_end)
		{
			if(FALSE==ads_pic_info_parser(ad->pic_info, &(ad->p_ad_data->dvbad_pic_attribute_node)))
			{
				DVB_ADS_DEBUG(DVBAD, ERR, "ads_pic_info_parser return FALSE\n");
				goto ERR_OUT;
			}

			DVB_ADS_DEBUG(DVBAD, INFO, "parser head data success\n");
			/*if sucess memory of pic_info need hold*/
			return SUCCESS;
		}
	}
ERR_OUT:
	dvb_ads_mempool_free(&s_dvbad.mem,ad->pic_info,ad->down_ctl.ads_pic_info.pic_information_size);
	ad->pic_info = NULL;
	return ERR_FAILURE;
}

//get all the secs of this pic and sort to save to the ad->pic_data,end save to the ads_data_node list by pic_id
static RET_CODE save_filter_pic_data(filter_buf_node *p_list, u16 pic_id)
{
	dvbad_priv_t *ad = &s_dvbad;
	filter_buf_node *p_buf_node = NULL;
	u8 section_number = 0;
	u8 last_section_number;
	u32 pic_data_size;
	RET_CODE ret;
	ads_data_node *p_data_node = &ad->p_ad_data->dvbad_pic_attribute_node;

	if (NULL == ad->pic_data)
	{
		DVB_ADS_DEBUG(DVBAD, INFO, " alloc pic data\n");
		pic_data_size = get_one_pic_size(pic_id, &(ad->p_ad_data->dvbad_pic_attribute_node));
		DVB_ADS_DEBUG(DVBAD,INFO,"pic_id = %u,pic_size = %u",(unsigned int)pic_id,(unsigned int)pic_data_size);
		ad->pic_data = dvb_ads_mempool_alloc(&s_dvbad.mem,pic_data_size);
		if (NULL == ad->pic_data)
		{
			DVB_ADS_DEBUG(DVBAD, ERR, " alloc failure!\n");
			return ERR_FAILURE;
		}
		memset((void*)ad->pic_data, 0, pic_data_size);
		ad->pic_data_pos = 0;
	}
	//one for rec of the head node ,another one for seach the list for right sec
	p_buf_node =p_list;
	// get all the sec of pic_id data and sort it ,end saving to the full sec of the pic data to  global var
	if(NULL == p_buf_node)
	{
		DVB_ADS_DEBUG(DVBAD, ERR, "The pic_id = %d is not filtered out !!!\n",pic_id);
		dvb_ads_mempool_free(&s_dvbad.mem,(u8*)ad->pic_data,pic_data_size);
		ad->pic_data = NULL;
		return ERR_FAILURE;
	}	
	//get the last sec of the pic
	last_section_number = p_buf_node->p_data[7];	

	while(section_number <= last_section_number)
	{
		//get the sec of the pic_id in the filter_buf_list order by sec number
	
			
		/*NOTE:	The ad->p_data is just a temp space for saving the ordered sec of one pic ,
			and then the complete pic is saved to the pos it should be!!!*/						
		/*pass in the cur length ,and return the length of operating ad->pic_data*/
		
		p_buf_node = p_list;

		/*find the sec in the filter_buf_node*/
		while(p_buf_node != NULL)
		{
			if(section_number == p_buf_node->p_data[6])			
			{
				break;
			}
			p_buf_node = p_buf_node->next;
		}
		//save from the sec = 0 to last_sec,all the sec has will be saved to the ad->pic_data in order 
		DVB_ADS_DEBUG(DVBAD,INFO,"The sec = %d is going to be saved to the ad->pic_data!!!",section_number);
		ads_get_pic_data(p_buf_node->p_data, ad->pic_data, &ad->pic_data_pos);
		DVB_ADS_DEBUG(DVBAD,INFO,"The sec = %d real pic data has been saved to the ad->pic_data,END real data len = %u!!!\n",section_number,(unsigned int)ad->pic_data_pos);
		
		/*while out port*/

		if(section_number == last_section_number)
		{
			//The complete of one pic will be saved to the p_data_node on right position
			DVB_ADS_DEBUG(DVBAD,INFO,"The all secs of the pic real data is going to be saved to the ad->pic_data!!!");
			ret = ads_save_pic_data(p_data_node, ad->pic_data, ad->pic_data_pos, pic_id);
			DVB_ADS_DEBUG(DVBAD,INFO,"The pic_id = %d,the real GIF pic size is %u is just been saved to the global var!!!",pic_id,(unsigned int)ad->pic_data_pos);
			dvb_ads_mempool_free(&s_dvbad.mem,(u8*)ad->pic_data,pic_data_size);
			ad->pic_data = NULL;
			return ret;
		}
		section_number++;
	}

	return ERR_FAILURE;
}

//-----------------------------------------------------------------

u16 get_fitler_id_by_filter_handle(handle_t filter_handle)
{
	u16 filterID = 0;
	dvbad_priv_t *ad = &s_dvbad;
	for(filterID = 0;filterID < DVB_ADS_FILTER_MAX_NUM;filterID++)
	{
		dvbad_ads_filter_struct_t *dvb_ads_filter = &ad->filter[filterID];
		if(dvb_ads_filter->filter_handle == filter_handle)
		{
			return filterID;
		}
	}
	return DVB_ADS_FILTER_MAX_NUM;
}
//Get the filter handle by var filterID -> check args stroed in the global var
handle_t get_filter_handle_by_filter_id(u16 filterID)
{
	dvbad_priv_t *ad = &s_dvbad;
	if(filterID < 0)
	{
		DVB_ADS_DEBUG(DVBAD,ERR,"filterID error : %d < 0",filterID);
		return 0;
	}
	dvbad_ads_filter_struct_t *dvb_ads_filter = &ad->filter[filterID];
	return dvb_ads_filter->filter_handle;	
}

handle_t get_channel_handle_by_filter_id(u16 filterID)
{
	dvbad_priv_t *ad = &s_dvbad;
	if(filterID < 0)
	{
		DVB_ADS_DEBUG(DVBAD,ERR,"filterID error : %d < 0",filterID);
		return 0;
	}
	dvbad_ads_filter_struct_t *dvb_ads_filter = &ad->filter[filterID];
	return dvb_ads_filter->channel_handle;	
}

//alloc channel handle ,filter handle and filter id
//return : filter_handle
//return 0 for FAIL and >0 for success!
//0xFFFF for table_id = 0xFE,ext_id = 0x0001->0x00F0
static handle_t ads_channel_handle = 0;
static int ads_filter_count = 0;	//record the number of filter_handle has been alloced from this channel_handle
static BOOL ads_filter_flag = FALSE;	//record the channel handle alloced or not!!!
static u16 dvbad_alloc_filter_ads(u16 ads_pid, u8 table_id, u16 ext_id)
{
	s32 ret = 0;
	handle_t channel_handle;
	handle_t filter_handle;
	u16 index;
	dvbad_priv_t *ad = &s_dvbad;
	
	app_porting_psi_demux_lock();
	if(FALSE == ads_filter_flag)
	{
		ads_channel_handle = channel_handle = GxDemux_ChannelAllocate(0, ads_pid);
		ads_filter_flag = TRUE;
		ads_filter_count++;
		DVB_ADS_DEBUG(DVBAD,INFO,"		Alloced channel_handle = 0x%x",channel_handle);
		if((0 == channel_handle) || (-1 == channel_handle))
		{
			channel_handle = 0;
			ads_channel_handle = 0;
			app_porting_psi_demux_unlock();
			DVB_ADS_DEBUG(DVBAD,INFO,"		channel_handle alloced failed!!!");
			return DVB_ADS_FILTER_MAX_NUM;
		}
	}
	else
	{
		//Get the channel handle if the channal for this pid has been alloced!!!
		channel_handle = ads_channel_handle;
		ads_filter_count++;
		DVB_ADS_DEBUG(DVBAD,INFO,"		The filters have been alloced %d for ADS_PID",ads_filter_count);
	
	}
	filter_handle = GxDemux_FilterAllocate(channel_handle);
	DVB_ADS_DEBUG(DVBAD,INFO,"		Alloced filter_handle = 0x%x",filter_handle);
	if((0 == filter_handle) ||( -1 == filter_handle))
	{
		ret = GxDemux_ChannelFree(channel_handle);
		DVB_ADS_CHECK_RET(DEMUX,ret);
		filter_handle = 0;
		app_porting_psi_demux_unlock();
		DVB_ADS_DEBUG(DVBAD,INFO,"		filter_handle alloced failed!!!");
		return DVB_ADS_FILTER_MAX_NUM;
	}
	for(index = 0;index < DVB_ADS_FILTER_MAX_NUM;index++)

	{

		if(DVB_ADS_FREE == ad->filter[index].filter_status)
		{
			DVB_ADS_DEBUG(DVBAD,INFO,"		Alloced filter_id = %d",index);
			ad->filter[index].filter_id = index;			//filter_id
			ad->filter[index].filter_status = DVB_ADS_USED;		//
			ad->filter[index].filter_handle = filter_handle;		//filter_handle
			ad->filter[index].channel_handle = channel_handle;		//channel_handle
			ad->filter[index].channel_pid = ads_pid;
			DVB_ADS_DEBUG(DVBAD,INFO,"		This filter is for pid = 0x%x",ads_pid);
			if(DATA_PID == ads_pid)
			{
				do{
					if(0xFF == table_id)
					{
						DVB_ADS_DEBUG(DVBAD,INFO,"		This table_id is not used for this filter!!!");
						break;
					}
					else if(0xFE == table_id)
					{
						ad->filter[index].table_id = table_id;
						DVB_ADS_DEBUG(DVBAD,INFO,"		This filter is for HEAD info table_id = 0x%x",table_id);
						if(0x0000 == ext_id)
						{
							ad->filter[index].ext_id = ext_id;	//pic_id
							DVB_ADS_DEBUG(DVBAD,INFO,"		This filter is for CTRL info ext_id = 0x%x",ext_id);
							break;
						}
						else if(0x0001 <= ext_id && 0x00F0 >= ext_id)
						{
							DVB_ADS_DEBUG(DVBAD,INFO,"		This ext_id is for AD_HEAD: XML or PIC info !!!");						
							break;
						}
						else
						{
							DVB_ADS_DEBUG(DVBAD,INFO,"		This wrong ext_id  for this filter!!!");
						}
					}
					else if(0xFD == table_id)
					{
						DVB_ADS_DEBUG(DVBAD,INFO,"		This filter is for PIC data table_id = 0x%x,ext_id = pic_id = %d!!!",table_id,ext_id);				
						ad->filter[index].ext_id = ext_id;
						break;
					}
					else
					{
						DVB_ADS_DEBUG(DVBAD,INFO,"		This table_id is for XML or pic info!!!");
						break;
					}
				}
				while(0);
			}
			
			//ad->p_ad_data->dvbad_pic_attribute_node.ads_pic_attribute_data.file_number = ext_id;	//pic_id
			app_porting_psi_demux_unlock();
			return index;
		}
	}
	
	DVB_ADS_DEBUG(DVBAD, INFO,"have no free filter\n");
	return DVB_ADS_FILTER_MAX_NUM;
}




//for pic data ,alloc for all secs and free for all secs ,the similarity is that they have the same table_id
static void dvbad_free_filter_ads(u16 index)
{
	u32 ret = 0;
	u16 filterID = 0;
	dvbad_priv_t *ad = &s_dvbad;
	dvbad_ads_filter_struct_t *ds_ads_filter = NULL;
	
	for(filterID = 0; filterID < DVB_ADS_FILTER_MAX_NUM; filterID++)
	{
		if(1 == ad->filter[filterID].filter_status)
		{		
			DVB_ADS_PRINTF("\n");
			DVB_ADS_DEBUG(DVBAD,INFO,"filterID = %d, ad->filter[filterID].table_id = 0x%x,ad->filter[filterID].filter_status = %d(0 for free/1 for used)\n",filterID,ad->filter[index].table_id,ad->filter[filterID].filter_status);
		}
	}

	for(filterID = 0; filterID < DVB_ADS_FILTER_MAX_NUM; filterID++)
	{	//For xml and pic_info all the sec is not set ,init 0
		if(/*(ad->filter[index].ext_id == ad->filter[filterID].ext_id)&& */ ad->filter[filterID].filter_status == DVB_ADS_USED)
		{
			ds_ads_filter = &ad->filter[filterID];
			DVB_ADS_DEBUG(DVBAD, INFO,"free filterID = %u",ds_ads_filter->filter_id);
			if(0 != ds_ads_filter->filter_handle)
			{
				ret = GxDemux_FilterFree(ds_ads_filter->filter_handle);
				DVB_ADS_CHECK_RET(DVBAD,ret);
				DVB_ADS_PRINTF("\n");
				
				ds_ads_filter->filter_handle = 0;
				ds_ads_filter->sec = 0xFF;
				ads_filter_count--;
				DVB_ADS_DEBUG(DVBAD,INFO,"%d filter have been left for ADS_PID!!!\n",ads_filter_count);
			}	
			if(0 != ds_ads_filter->channel_handle)
			{
				if( 0 == ads_filter_count)
				{
					//all the filter handle alloced by this channel handle has been freed  ,free the channel handle
					ret = GxDemux_ChannelFree(ds_ads_filter->channel_handle);
					DVB_ADS_DEBUG(DVBAD,INFO,"Channel handle freed!!!\n\n");
					DVB_ADS_CHECK_RET(DEMUX,ret);
					ads_filter_flag = FALSE;
					//ads_filter_count = 0;
				}
				ds_ads_filter->channel_handle = 0;
				ds_ads_filter->channel_pid = 0xFFFF;
			}
			ds_ads_filter->filter_id = DMX_INVALID_CHANNEL_ID;
			ds_ads_filter->filter_status = DVB_ADS_FREE;
		}
	}
}

/*
	Alloc filter for bat
*/
static u16 dvbad_alloc_filter_bat(u16 ads_pid)
{
	s32 ret = 0;
	handle_t channel_handle;
	handle_t filter_handle;
	u16 filterID;
	dvbad_priv_t *ad = &s_dvbad;
	
	app_porting_psi_demux_lock();
	channel_handle = GxDemux_ChannelAllocate(0, ads_pid);
	DVB_ADS_PRINTF("\n");
	DVB_ADS_DEBUG(DVBAD,INFO,"		Alloced channel_handle = 0x%x",channel_handle);
	if((0 == channel_handle) || (-1 == channel_handle))
	{
		channel_handle = 0;
		app_porting_psi_demux_unlock();
		DVB_ADS_DEBUG(DVBAD,INFO,"		channel_handle alloced failed!!!");
		return DVB_ADS_FILTER_MAX_NUM;
	}
	filter_handle = GxDemux_FilterAllocate(channel_handle);
	DVB_ADS_DEBUG(DVBAD,INFO,"		Alloced filter_handle = 0x%x",filter_handle);
	if((0 == filter_handle) ||( -1 == filter_handle))
	{
		ret = GxDemux_ChannelFree(channel_handle);
		DVB_ADS_CHECK_RET(DEMUX,ret);
		filter_handle = 0;
		app_porting_psi_demux_unlock();
		DVB_ADS_DEBUG(DVBAD,INFO,"		filter_handle alloced failed!!!");
		return DVB_ADS_FILTER_MAX_NUM;
	}
	for(filterID = 0;filterID < DVB_ADS_FILTER_MAX_NUM;filterID++)
	{
		if(DVB_ADS_FREE == ad->filter[filterID].filter_status)
		{
			DVB_ADS_DEBUG(DVBAD,INFO,"		Alloced filter_id = %d for BAT pid = 0x%x\n",filterID,ads_pid);
			ad->filter[filterID].filter_id = filterID;			//filter_id
			ad->filter[filterID].filter_status = DVB_ADS_USED;		//
			ad->filter[filterID].filter_handle = filter_handle;		//filter_handle
			ad->filter[filterID].channel_handle = channel_handle;		//channel_handle
			ad->filter[filterID].channel_pid = ads_pid;
			app_porting_psi_demux_unlock();
			return filterID;
		}
	}
	DVB_ADS_DEBUG(DVBAD, INFO,"have no free filter\n");
	return DVB_ADS_FILTER_MAX_NUM;
}

/*
  Free the filter handle and channe handle indexed by filter_id,, including the related infomation in the global var  
*/
static void dvbad_free_filter_bat(u16 filter_id)
{
	u32 ret = 0;
	dvbad_priv_t *ad = &s_dvbad;
	dvbad_ads_filter_struct_t *ds_ads_filter = &ad->filter[filter_id];
	DVB_ADS_DEBUG(DVBAD, INFO,"free BAT filter id = %u\n",filter_id);
	if(0 != ds_ads_filter->channel_handle)
	{
		ret = GxDemux_FilterFree(ds_ads_filter->filter_handle);
		DVB_ADS_CHECK_RET(DVBAD,ret);
		DVB_ADS_DEBUG(DVBAD,INFO,"free BAT filter handle = 0x%x",ds_ads_filter->filter_handle);
		ds_ads_filter->filter_handle = 0;
	}	
	if(0 != ds_ads_filter->channel_handle)
	{
		ret = GxDemux_ChannelFree(ds_ads_filter->channel_handle);
		DVB_ADS_CHECK_RET(DEMUX,ret);
		DVB_ADS_DEBUG(DVBAD,INFO,"free BAT filter channel handle = 0x%x",ds_ads_filter->channel_handle);
		ds_ads_filter->channel_handle = 0;
	}
	ds_ads_filter->filter_id = DMX_INVALID_CHANNEL_ID;
	ds_ads_filter->filter_status = DVB_ADS_FREE;
	ds_ads_filter->channel_pid = 0xFFFF;
	//memset(ad->filter[index].filter_mask,0,filter_num);
}


static ST_AD_DVBAD_FUNC_TYPE _dvbad_set_filter(ST_ADS_DVBAD_FILTER_INFO *pstFilterInfo)
	{
		BOOL ret = FALSE;
		u16 index = pstFilterInfo->filter_id;
		//u16 filterID = 0;
		//handle_t channel_handle;
		//dvbad_priv_t *ad = &s_dvbad;
		ca_filter_t sfilter = {0};
		DVB_ADS_DEBUG(DVBAD,INFO,"filterID = %d start setting...",index);
		if(NULL == pstFilterInfo){
			DVB_ADS_DEBUG(DVBAD,ERR,"NULL Param !!!");
			return DVBAD_FUN_ERR_PARA;
		}
		DVB_ADS_DEBUG(DVBAD, INFO, "usChannelPid:0x%x \n",pstFilterInfo->usChannelPid);
		DVB_ADS_DEBUG(DVBAD, INFO, "ucFilterLen:%d \n",pstFilterInfo->ucFilterLen);
				
		DVB_ADS_CHECK_RET(DVBAD,ret);
	
		memset(&sfilter,0,sizeof(sfilter));
		
		sfilter.handle = get_filter_handle_by_filter_id(index);
		DVB_ADS_DEBUG(DVBAD,INFO,"The filter handle get by filerID is 0x%x\n",(unsigned int)sfilter.handle);
		sfilter.usedStatus = 1;

		sfilter.filterLen = pstFilterInfo->ucFilterLen;
		memcpy(sfilter.match,pstFilterInfo->aucFilter,sfilter.filterLen)	;
		memcpy(sfilter.mask,pstFilterInfo->aucMask,sfilter.filterLen);
		
		sfilter.pid = DATA_PID;
		//sfilter.repeatFlag = 1;
		sfilter.equalFlag = 1;
		sfilter.Callback = NULL;
		sfilter.nms = 500;
		sfilter.nWaitSeconds = 10;
		DVB_ADS_DUMP("mask",sfilter.mask,sfilter.filterLen);
		DVB_ADS_DUMP("match",sfilter.match,sfilter.filterLen);

		ret = GxDemux_FilterSetup(sfilter.handle,sfilter.match,sfilter.mask, TRUE, TRUE,0,sfilter.filterLen);
		DVB_ADS_CHECK_RET(DEMUX,ret);
	
		sfilter.channelhandle = get_channel_handle_by_filter_id(index);
		DVB_ADS_DEBUG(DVBAD,INFO,"The channel handle get by filterID is 0x%x",sfilter.channelhandle);
		ret = GxDemux_ChannelEnable(sfilter.channelhandle);
		DVB_ADS_CHECK_RET(DEMUX,ret);
		ret = GxDemux_FilterEnable(sfilter.handle);
		DVB_ADS_DEBUG(DVBAD,INFO,"The filter handle get by filterID is 0x%x\n",sfilter.handle);
		DVB_ADS_CHECK_RET(DEMUX,ret);
		app_porting_psi_demux_unlock();
		return DVBAD_FUN_OK;
	}

BOOL dvbad_set_filter_bat(void)
{
	ST_AD_DVBAD_FUNC_TYPE ret = DVBAD_FUN_ERR_PARA;
	ST_ADS_DVBAD_FILTER_INFO filter_info;
	u16 filterID = 0;
	memset(&filter_info,0,sizeof(filter_info));
	DVB_ADS_DEBUG(DVBAD,INFO,"Alloced BAT filter start!!!");
	filterID = dvbad_alloc_filter_bat(DVB_BAT_PID);
	if (DVB_ADS_FILTER_MAX_NUM == filterID)
	{
		DEBUG(DVBAD, ERR, "Alloc filter err!\n");	
		return FALSE;
	}
	DVB_ADS_DEBUG(DVBAD,INFO,"alloc filter success and filterID is %d",filterID);
	filter_info.filter_id = filterID;
	filter_info.usChannelPid = DVB_BAT_PID;
	filter_info.ucFilterLen = 11;
	//filter_info.aucFilter[0] = 0x4a;
	filter_info.aucFilter[3] = 0xFF;
	filter_info.aucFilter[4] = 0; 
	filter_info.aucFilter[6] = 0;
	filter_info.aucFilter[10] = 0xF0;

	//filter_info.aucMask[0]=0xFF;
	filter_info.aucMask[3] = 0xFF;
	filter_info.aucMask[4] = 0xFF;
	filter_info.aucMask[6] = 0xFF;
	filter_info.aucMask[10] = 0xFF;

	filter_info.ucWaitSeconds = 10;
	filter_info.pfDataNotifyFunction = NULL;
	DVB_ADS_DEBUG(DVBAD,INFO,"Start to set filter for BAT...");
	ret = _dvbad_set_filter(&filter_info);
	if(ret != DVBAD_FUN_OK)
	{
		DVB_ADS_DEBUG(DVBAD, ERR, "Set filter for BAT failed!!!");
	}
	DVB_ADS_DEBUG(DVBAD, INFO,"dvbad set filter bat sucess");
	DVB_ADS_DEBUG(DVBAD,INFO,"Set filter for BAT end...\n");
	return TRUE;
}


//---------------------------The next function are for ad PIC data filter--------------------------------------//

static BOOL _set_filter_for_data_table(u16 filterid, u8 table_id, u16 ext_tid)
{
	//u16 i = 0;
	ST_ADS_DVBAD_FILTER_INFO filter_info;
	ST_AD_DVBAD_FUNC_TYPE ret = DVBAD_FUN_ERR_PARA;
	dvbad_priv_t *ad = &s_dvbad;
	memset(&filter_info,0,sizeof(ST_ADS_DVBAD_FILTER_INFO));
	DVB_ADS_DEBUG(DVBAD, INFO, "filterid: %d,table_id:0x%x,extension_id:0x%x",filterid,(unsigned int)table_id,ext_tid);
	filter_info.filter_handle = get_filter_handle_by_filter_id(filterid);
	filter_info.channel_handle = ad->filter[filterid].channel_handle;
	filter_info.filter_id = filterid;
	filter_info.usChannelPid = DATA_PID;
	filter_info.ucFilterLen = 7;
	memset((void*)filter_info.aucFilter, 0, sizeof(filter_info.aucFilter));
	memset((void*)filter_info.aucMask, 0, sizeof(filter_info.aucMask));
	filter_info.aucFilter[0] = table_id;
	filter_info.aucFilter[3] = (u8)((ext_tid&0xFF00)>>8);
	filter_info.aucFilter[4] = (u8)(ext_tid&0x00FF); 
	filter_info.aucMask[0] = 0xFF;
	filter_info.aucMask[3] = 0xFF;
	filter_info.aucMask[4] = 0xFF;
	filter_info.ucWaitSeconds = 10;
	filter_info.pfDataNotifyFunction = NULL;
	DVB_ADS_DEBUG(DVBAD,INFO,"Alloced filterid = %u for pic_id = %d\n",(unsigned int)filterid,ext_tid);
	//tag for free the sec filter
	ret = _dvbad_set_filter(&filter_info);
	if(ret != DVBAD_FUN_OK)
	{
		DVB_ADS_DEBUG(DVBAD, INFO,"set filter ERR! \n");
	}
	return TRUE;
}
#if 0
static BOOL _set_filter_for_data_table_sec(u16 filterid, u8 table_id, u16 ext_tid,u8 sec)
{
	//u16 i = 0;
	ST_ADS_DVBAD_FILTER_INFO filter_info;
	ST_AD_DVBAD_FUNC_TYPE ret = DVBAD_FUN_ERR_PARA;
	dvbad_priv_t *ad = &s_dvbad;
	memset(&filter_info,0,sizeof(ST_ADS_DVBAD_FILTER_INFO));
	DVB_ADS_DEBUG(DVBAD, INFO, "filterid: %d,table_id:0x%x,extension_id:0x%x",filterid,(unsigned int)table_id,ext_tid);
	filter_info.filter_handle = get_filter_handle_by_filter_id(filterid);
	filter_info.channel_handle = ad->filter[filterid].channel_handle;
	filter_info.filter_id = filterid;
	filter_info.usChannelPid = DATA_PID;
	filter_info.ucFilterLen = 7;
	memset((void*)filter_info.aucFilter, 0, sizeof(filter_info.aucFilter));
	memset((void*)filter_info.aucMask, 0, sizeof(filter_info.aucMask));
	filter_info.aucFilter[0] = table_id;
	filter_info.aucFilter[3] = (u8)((ext_tid&0xFF00)>>8);
	filter_info.aucFilter[4] = (u8)(ext_tid&0x00FF); 
	filter_info.aucFilter[6] = sec;
	filter_info.aucFilter[7] = 0xE;//14 SEC
	filter_info.aucMask[0] = 0xFF;
	filter_info.aucMask[3] = 0xFF;
	filter_info.aucMask[4] = 0xFF;
	filter_info.aucMask[6] = 0;
	filter_info.aucMask[7] = 0xFF;
	filter_info.ucWaitSeconds = 10;
	filter_info.pfDataNotifyFunction = NULL;

	ret = _dvbad_set_filter(&filter_info);
	if(ret != DVBAD_FUN_OK)
	{
		DVB_ADS_DEBUG(DVBAD, INFO,"set filter ERR! \n");
	}
	return TRUE;
}
#endif

static ST_AD_DVBAD_FUNC_TYPE dvbad_set_filter_for_pic_data(void)
{
	//int i;
	//handle_t filter_handle;
	u16 filter_id = 0xFFFF;
	ads_data_node *p_pic;
	//s32 ret = 0;
//	u8 sec = 0;
//	u8 sec_cnt = 0;
	dvbad_priv_t *ad = &s_dvbad;
	DVB_ADS_PRINTF("\n");
	DVB_ADS_DEBUG(DVBAD, INFO,"pic count :%d\n",ad->p_ad_data->ads_pic_count);
	p_pic = &(ad->p_ad_data->dvbad_pic_attribute_node);
	//find the next pic to filter the secs
	while(NULL != p_pic)
	{
		if(p_pic->status == FINISH)
		{
			p_pic = p_pic->next;
			continue;
		}
		break;
	}
	//set filter for all secs of this pic 
	if(NULL == p_pic)
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"The pic has been filtered out completely!!!");
		return DVBAD_FUN_FILTER_COMPELET;
	}	
	if (p_pic->status == EMPTY)
	{
		if (get_fl(p_pic->ads_pic_attribute_data.file_number) != INVLAD_NODE_PRT)
		{
			//sec_cnt =  p_pic->p_pic_info.pic_segment_num;
			//for(sec = 0; sec < sec_cnt; sec++)
			//{
				filter_id = dvbad_alloc_filter_ads(DATA_PID,AD_DATA_TABLE_ID,p_pic->ads_pic_attribute_data.file_number);
				DVB_ADS_DEBUG(DVBAD,INFO,"The filter condition for Pic_id = %d,filter_id = %u,Pid = 0x%x,tid = 0x%x,ext_id = 0x%x",p_pic->ads_pic_attribute_data.file_number,filter_id,DATA_PID,AD_DATA_TABLE_ID,p_pic->ads_pic_attribute_data.file_number);
				DVB_ADS_PRINTF("\n");
				//DVB_ADS_DEBUG(DVBAD,INFO,"\t\tAlloced filter_id = %d for pic_id = %d\n",filter_id,p_pic->ads_pic_attribute_data.file_number);
				//filter_alloc_flag = FALSE;
				if(DVB_ADS_FILTER_MAX_NUM == filter_id)
				{
					DVB_ADS_DEBUG(DVBAD,ERR,"Alloc filter handle failed!!!");
					return 	DVBAD_FUN_NO_ELEMENT;			
				}
				DVB_ADS_DEBUG(DVBAD,INFO,"Set filter for pic_id = %u",p_pic->ads_pic_attribute_data.file_number);
				_set_filter_for_data_table(filter_id,AD_DATA_TABLE_ID,p_pic->ads_pic_attribute_data.file_number);
			
			//}		 	
			p_pic->status = RECEIVING;
			DVB_ADS_PRINTF("\n**********************************************************************************************");
		}
	}
	return DVBAD_FUN_OK;
}


static BOOL dvbad_check_addata_complete(void )
{
	int i;
	ads_data_node *p_pic;
	DVB_ADS_DEBUG(DVBAD, INFO,"check pic count :%d\n",s_dvbad.p_ad_data->ads_pic_count);	
	p_pic = &s_dvbad.p_ad_data->dvbad_pic_attribute_node;	
	for(i=0;i<s_dvbad.p_ad_data->ads_pic_count;i++)
	{
		if (p_pic->status != FINISH)
		{
			return FALSE;
		}
		if (p_pic->next == NULL)
			break;
		
		p_pic = p_pic->next;
	}
	s_dvbad.p_ad_data->ad_head_data.save_all_pic_flag = TRUE;
	return TRUE;
}


//---------------------------------------------------------------------------------------------------

static ST_AD_DVBAD_FUNC_TYPE _set_filter_for_adhead(u16 filter_id, u16 ext_id,u8 data_type)
{
	ST_AD_DVBAD_FUNC_TYPE ret;
	ST_ADS_DVBAD_FILTER_INFO filter_info;
	memset(&filter_info,0,sizeof(filter_info));
	if(filter_id != DVB_ADS_FILTER_MAX_NUM && (data_type == 0x01 || data_type == 0x02))
	{
		filter_info.filter_id = filter_id;
		filter_info.usChannelPid = DATA_PID;
		filter_info.ucFilterLen = 9;
		memset((void*)filter_info.aucFilter, 0, 18*sizeof(u8));
		memset((void*)filter_info.aucMask, 0, 18*sizeof(u8));
		filter_info.aucFilter[0] = 0xFE;
		filter_info.aucFilter[3] = (u8)((ext_id&0xFF00)>>8);
		filter_info.aucFilter[4] = (u8)(ext_id & 0x00FF); 
		filter_info.aucFilter[6] = 0;
		filter_info.aucFilter[8] = data_type;//01 xmL,02 pic
		filter_info.aucMask[0] = 0xFF;
		filter_info.aucMask[3] = 0xFF;
		filter_info.aucMask[4] = 0xFF;
		filter_info.aucMask[6] = 0x0;
		filter_info.aucMask[8] = 0xFF;
		filter_info.ucWaitSeconds = 10;
		filter_info.pfDataNotifyFunction = NULL;
		ret = _dvbad_set_filter(&filter_info);
		DVB_ADS_DEBUG(DVBAD, INFO,"		[AD_HEAD] set filter_id = %d ext_id:%d end .ret:%d",filter_id,ext_id,ret);
		return ret;
	}
	else
	{
		return DVBAD_FUN_ERR_PARA;
	}
}

static ST_AD_DVBAD_FUNC_TYPE dvbad_set_filter_for_adhead(u16 ext_id, u16 last_ext_id,u8 data_type)
{
	/*need recoder if have section and table have not filtered*/
	u16 extid;
	u16 filter_id;
	/*we just process xml info less than DVB_ADS_FILTER_MAX_NUM tables*/
	for(extid = ext_id;extid <= last_ext_id; extid++)
	{
		if (get_fl(extid) != INVLAD_NODE_PRT)
		{
			filter_id = dvbad_alloc_filter_ads(DATA_PID,AD_HEAD_TABLE_ID,extid);
			DVB_ADS_DEBUG(DVBAD,INFO,"	[AD_HEAD]Alloc fitlerID = %d for extid = 0x%x",filter_id,extid);
			if(filter_id != DVB_ADS_FILTER_MAX_NUM)
			{
				_set_filter_for_adhead(filter_id, extid,data_type);
				continue;
			}
			DVB_ADS_DEBUG(DVBAD,ERR,"	[AD_HEAD]extid : 0x%x set filter failed!!",extid);
		}
	}
	return DVBAD_FUN_OK;
}


static BOOL dvbad_check_adhead_complete(u8 head_type)
{
	u16 tid;
	if (head_type == DATA_TYPE_XML)
	{
		for (tid=s_dvbad.down_ctl.ads_xml_info.index_xml_table_id_start;
				tid<=s_dvbad.down_ctl.ads_xml_info.index_xml_table_id_end;
				tid++)
		{
			if ( ! check_fl_complete(tid))
			{	
			
				DVB_ADS_DEBUG(DVBAD,INFO,"XML info has been filtered not completly!!!");
				return FALSE;
			}
		}
		DVB_ADS_DEBUG(DVBAD,INFO,"XML info has been filtered out completly!!!");
		return TRUE;
	}
	else if (head_type == DATA_TYPE_PIC)
	{
		for (tid=s_dvbad.down_ctl.ads_pic_info.pic_information_table_id_start;
				tid<=s_dvbad.down_ctl.ads_pic_info.pic_information_table_id_end;
				tid++)
		{
			if ( ! check_fl_complete(tid))
			{	
			
				DVB_ADS_DEBUG(DVBAD,INFO,"PIC info has been filtered not completly!!! tid = %d",tid);
				return FALSE;
			}
		}
		DVB_ADS_DEBUG(DVBAD,INFO,"PIC info has been filtered out completly!!!");
		return TRUE;
	}
	return FALSE;
}


static BOOL dvbad_set_filter_ctrl_info(u16 filter_id)
{
	ST_AD_DVBAD_FUNC_TYPE ret = DVBAD_FUN_ERR_PARA;
	ST_ADS_DVBAD_FILTER_INFO filter_info;
	memset(&filter_info,0,sizeof(filter_info));

	filter_info.filter_id = filter_id;
	filter_info.usChannelPid = DATA_PID;
	filter_info.ucFilterLen = 8;
	filter_info.aucFilter[0] = 0xFE;
	filter_info.aucFilter[3] = 0;
	filter_info.aucFilter[4] = 0; 
	filter_info.aucFilter[6] = 0;
	filter_info.aucFilter[7] = 0;
	filter_info.aucMask[0] = 0xFF;
	filter_info.aucMask[3] = 0xFF;
	filter_info.aucMask[4] = 0xFF;
	filter_info.aucMask[6] = 0xFF;
	filter_info.aucMask[7] = 0xFF;

	filter_info.ucWaitSeconds = 10;
	filter_info.pfDataNotifyFunction = NULL;

	ret = _dvbad_set_filter(&filter_info);
	if(ret != DVBAD_FUN_OK)
	{
		DVB_ADS_PRINTF("[drv] %s %d ERR! \n",__func__,__LINE__);
		DVB_ADS_ASSERT(0);
	}
	DVB_ADS_DEBUG(DVBAD, INFO,"dvbad_set_filter_CTRL_INFO return true\n");
	return TRUE;
}

// >> 1.run the first time and no data in flash(judge at the return value of func dvb_ads_get_logo_config_data()) ==>need to upgrade
// >> 2.compare with the version read from flash ,if version different ==>need to upgrade!!
// >> else  no need to upgrade!!!
static BOOL dvb_ads_check_need_upgrade(u16 AD_version,u8 version_number)
{
	//RET_CODE ret_flash = SUCCESS;
	//u16 *version_global = NULL;
	ad_save_info_t *save_info = &ad_save_info;
	dvb_ads_get_flash_bat_data();
	if(((save_info->ad_head_data.AD_version != AD_version)&&(save_info->ad_head_data.version_number != version_number)) || (FALSE == save_info->ad_head_data.save_all_pic_flag))
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"AD VERSION need upgrade!!!");
		//dvb_ads_version_need_update_flag = FALSE;
		return TRUE;
	}
	DVB_ADS_DEBUG(DVBAD,INFO,"Nothing changed of AD VERSION!!!");
	//dvb_ads_version_need_update_flag = TRUE;
	//return FALSE;
	return TRUE;//NOTE: UPGRADE EVERYTIME ,The menu ad is saved in the dir mnt ,once restart no file exit if not upgrade!!!
}

static RET_CODE parser_bat_data(u8 *p_data, u32 data_len)
{
	dvbad_priv_t *ad = &s_dvbad;
	bat_data ad_bat_data;
	u16 bouquet_length;
	u8 version_number;
	u16 ad_version;
	u8 private_id[16];
	u8 date_Length;
	u8 descriptor_length;
	u8 linkage_type;
	u16 filter_id;
	//u16 pid;
	DVB_ADS_DEBUG(DVBAD,INFO,"Start to parse BAT DATA!!!");
	if(NULL==p_data)
	{
		DVB_ADS_DEBUG(DVBAD, ERR, " p_data == NULL\n");
		return ERR_FAILURE;
	}
	
	version_number = p_data[5]&0x3E;
	bouquet_length = ((u16)p_data[8]&0x0F<<8)|((u16)p_data[9]);
	ad_version = ((u16)p_data[13]<<8)|((u16)p_data[14]);
	date_Length = p_data[11];
	memset(private_id, 0, sizeof(private_id));
	memcpy(private_id, p_data+15, date_Length-3);
	descriptor_length = p_data[11+date_Length+2];
	ad_bat_data.valid_flag= p_data[12]&0x01;
	ad_bat_data.stream_id = ((u16)p_data[11+date_Length+3]<<8)&(u16)p_data[11+date_Length+4];
	ad_bat_data.network_id = ((u16)p_data[11+date_Length+5]<<8)&(u16)p_data[11+date_Length+6];
	ad_bat_data.service_id = ((u16)p_data[11+date_Length+7]<<8)&(u16)p_data[11+date_Length+8];
	linkage_type = p_data[11+date_Length+9];
	if(0xA1==linkage_type)
	{
		ad_bat_data.frequency = ((u32)p_data[11+date_Length+9]<<24)|
								((u32)p_data[11+date_Length+10]<<16)|
								((u32)p_data[11+date_Length+11]<<8)|
								(u32)p_data[11+date_Length+12];
		ad_bat_data.modulation = (u16)p_data[11+date_Length+13];
		ad_bat_data.symbol_rate = ((u32)p_data[11+date_Length+14]<<24)|
								  ((u32)p_data[11+date_Length+15]<<16)|
							      ((u32)p_data[11+date_Length+16]<<8)|
								  ((u32)p_data[11+date_Length+17]&0xF0);
	}
//	DVB_ADS_DEBUG(DVBAD, INFO, "\n\t\t\tversion_number:%d\n\t\t\tvalid_flag:%d\n\t\t\tad_version:%d\n\t\t\tprivate_id:%s\n\t\t\tdate_Length:%d\n\t\t\tstream_id:%d\n\t\t\tnetwork_id:%d\n\t\t\tservice_id:%d\n\t\t\tdescriptor_length:%d\n\t\t\t", 	version_number, ad->p_ad_data->ad_bat_data.valid_flag, ad_version, private_id, date_Length, ad->p_ad_data->ad_bat_data.stream_id, ad->p_ad_data->ad_bat_data.network_id, ad->p_ad_data->ad_bat_data.service_id, descriptor_length);
	
	// check version upgrade!!!
	if( FALSE == dvb_ads_check_need_upgrade(ad_version,version_number))
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"The AD VERSION is the latest!!!");
		exit_monitor(GET_AD_TRUE);
		return SUCCESS;
	}
	else
	{
		DVB_ADS_DEBUG(DVBAD, INFO, "New version get!!!");
		ad->p_ad_data->ad_head_data.version_number = version_number;
		ad->p_ad_data->ad_head_data.AD_version = ad_version;
		memcpy(&ad->p_ad_data->ad_bat_data, &ad_bat_data, sizeof(bat_data));
		
		DVB_ADS_DEBUG(DVBAD, INFO, "Ctrl info filter start!!!\n");
		//start ctrl info filter
		filter_id = dvbad_alloc_filter_ads(DATA_PID,AD_HEAD_TABLE_ID,0x0000);
		if(DVB_ADS_FILTER_MAX_NUM == filter_id)
		{
			DVB_ADS_DEBUG(DVBAD, INFO, "Get filterID for Ctrl failed!!!");
			return ERR_FAILURE;
		}
		dvbad_set_filter_ctrl_info(filter_id);
	}
	return SUCCESS;
}

/*
NOTE: 	First get the config tp info to start the BAT filter ,then according to the data parsed from table BAT 
	start the filter of the ctrl info,then parse the ctrl info to get ad head info ,then according to the ad head
	info to start the pic or xml filter! at the end ,parse the pic or xml data ,and all the data should be saved 
	to the global var ,and the loop of the filter and parser must be step by step,only the last step excute success 
	can we start the next step !!!
*/
extern BOOL search_lock_main_tp_flag;
static void ads_dvbad_data_monitor(void *p_param)
{
//	BOOL r = TRUE;
	s32 ret = FALSE;
	//RET_CODE ads_ret = SUCCESS;
	ST_AD_DVBAD_FUNC_TYPE fret = DVBAD_FUN_OK;
	u8 index = 0;
	u8 *p_data = NULL;
	u8 *p_section_data = NULL;
	u32 data_len = 0;
	//size_t size_read = 0;
	u16 table_extension_id;
	u8 table_id = 0;
	u16 last_table_id = 0;
	u8 head_type = 0x01;
	u8 section_number;
	u8 last_section_number;
	u16 pid = 0x1fff;	//To check the logo data file number.
	u16 bouquet_id;
	u32 time_out = 0;
	u32 time;
	uint16_t            section_length = 0;
	ST_AD_DVBAD_FUNC_TYPE ad_ret;
	dvbad_priv_t *ad = &s_dvbad;
	ad_save_info_t* ad_info = &ad_save_info;
	s_dvbad.monitor_count = 0xFFFFFFFF;

	init_fl();
	time = 300;//No data queryed more than 30 times then exit the thread
#if 0
	for(index = 0; index < DVB_ADS_FILTER_MAX_NUM; index++)
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"index = %d,ad->filter[index].filter_id = %d",index,ad->filter[index].filter_id);
	
	}
#endif
//	r = app_dvb_ads_sem_create(&ad_save_info.ad_opt_sem,0);

	p_data =  dvb_mempool_alloc(&ad->mem,MAX_PSI_SECTION_BUFFER);
	while (s_dvbad.monitor_count-- > 0)
	{
		time_out++;
		//mtos_task_sleep(10);
		GxCore_ThreadDelay(10);
		if ((time_out > time)||(search_lock_main_tp_flag==FALSE) )
		{
			DVB_ADS_DEBUG(DVBAD, ERR, "Filter time out\n");
			dvb_mempool_free(&ad->mem,p_data,MAX_PSI_SECTION_BUFFER);
			exit_monitor(GET_AD_FAILURE);
		}
		for(index = 0; index < DVB_ADS_FILTER_MAX_NUM; index++)
		{
			if(ad->filter[index].filter_status == DVB_ADS_FREE)
			{
				continue;
			}
			DVB_ADS_DEBUG(DVBAD,INFO,"filter_id = %d  filter_handle = 0x%x is being used!!!",ad->filter[index].filter_id,ad->filter[index].filter_handle);
			ret = GxDemux_QueryAndGetData(ad->filter[index].filter_handle,p_data,MAX_PSI_SECTION_BUFFER,(uint32_t *)&data_len);
			if (ret > 0)
			{
				time_out = 0;
				DVB_ADS_DEBUG(DVBAD,INFO,"DATA filtered out!!!");
				DVB_ADS_DEBUG(DVBAD,INFO,"Data queryed info : table_id = 0x%x \t ext_id = 0x%x,pic_id = %d\n",p_data[0],((u16)p_data[3]<<8)|(u16)p_data[4],((u16)p_data[3]<<8)|(u16)p_data[4]);
				section_length = ((p_data[1] & 0x0F) << 8) + p_data[2] + 3;
				DVB_ADS_DEBUG(DVBAD,INFO,"section_len = %d  data_len = %u  \n",section_length,(unsigned int)data_len)	;
				bouquet_id = table_extension_id = (((u16)p_data[3]<<8)|((u16)p_data[4]));
				if((DVB_BAT_PID == ad->filter[index].channel_pid)&&(0xFF00 == bouquet_id))
				{
					/*free bat filter*/
					dvbad_free_filter_bat(index);
					DVB_ADS_DEBUG(DVBAD,INFO,"BAT data fitered out!!!");
					/*parser bat table  and cmp version, and set ctrl filter if nessery*/
					if(SUCCESS != parser_bat_data(p_data, data_len))
					{
						DVB_ADS_PRINTF("\n");
						DVB_ADS_DEBUG(DVBAD, ERR, "parser bat data failure AND exit the thread!!!\n");
						exit_monitor(GET_AD_FAILURE);
					}
					DVB_ADS_DEBUG(DVBAD,INFO,"The BAT data parsered successful!!!");
					DVB_ADS_DEBUG(DVBAD,INFO,"The Ctrl INFO filter has been set up in the fun parse_bat_data");
					//we can prepare the version data for the compare!!!
					if(GXCORE_FILE_UNEXIST == GxCore_FileExists(DVB_FLASH_FILE_PATH))
					{
						DVB_ADS_DEBUG(DVBAD,INFO,"The BAT info file is not exit!!! Running for the first time!!!");
					}
					else
					{
						DVB_ADS_DEBUG(DVBAD,INFO,"Start to read out the ad_save_info_t data info from flash!!!");
						handle_t file_handle = GxCore_Open(DVB_FLASH_FILE_PATH,"r");
						size_t size_read = GxCore_Read(file_handle,(void *)ad_info,1,sizeof(ad_save_info_t));
						if(size_read > 0)
						{
							DVB_ADS_DEBUG(DVBAD,INFO,"Write the ad_save_info_t data from temp var to global var!!!");
							memcpy(&ad->p_ad_data->ad_bat_data,&ad_info->ad_bat_data,sizeof(bat_data));
							memcpy(&ad->p_ad_data->ad_head_data,&ad_info->ad_head_data,sizeof(ad_head_flash));
						}
						else
						{
							DVB_ADS_DEBUG(DVBAD,INFO,"No ad_save_info_t data read out from  flash!!!");
						}
						GxCore_FileDelete(DVB_FLASH_FILE_PATH);	
						time = 300;
					}
				}
				
				else if((DATA_PID == ad->filter[index].channel_pid)&&(AD_HEAD_TABLE_ID == p_data[0]))
				{
					section_number = p_data[6];
					last_section_number = p_data[7];
					head_type = p_data[8];//XML or PIC_INFO
					DVB_ADS_DEBUG(DVBAD, INFO, "filter id = %d, p_data[0] = 0x%x, table_extension_id = 0x%x, section_number = %d, last_section_number %d, data_type %d\n", index,p_data[0], table_extension_id, section_number, last_section_number, head_type);
					switch(table_extension_id)
					{
					/*here we just guess ctrl info can carried by ONE section!!!*/
					case EXT_TABLEID_CTR_INFO:
						DVB_ADS_DEBUG(DVBAD,INFO,"CTRL data filtered out!!!");
						DVB_ADS_DEBUG(DVBAD,INFO,"Start to parse the CTRL data!!!");
						dvbad_get_parser_down_ctrl_info(IN p_data,IN data_len,OUT &ad->down_ctl);
						/*free ctrl info filter*/
						dvbad_free_filter_ads(index);
						DVB_ADS_DEBUG(DVBAD,INFO,"Ctrl data parse out successful!!!");
						// set head filter
						table_id = ad->down_ctl.ads_xml_info.index_xml_table_id_start;
						last_table_id = ad->down_ctl.ads_xml_info.index_xml_table_id_end;
						DVB_ADS_DEBUG(DVBAD,INFO,"Start to filter the XML data!!!");
						DVB_ADS_DEBUG(DVBAD,INFO,"Set filter for XML data!!!");
						dvbad_set_filter_for_adhead(table_id, last_table_id, DATA_TYPE_XML); //set to filter the xml info
						break;
					default://the first only get xml info ,only when the xml info parsed completely and set the pic_info filter.
						//save to fl no matter what kind of head such as XML or PIC_INFO according to the "table_extension_id"
						/* receive data  cmp ext table id to save data*/
						p_section_data = p_data;
						while(data_len > 6)
						{
							section_length = ((p_section_data[1] & 0x0F) << 8) + p_section_data[2] + 3;
							if(section_length > data_len)
							{
								ADS_ERROR("section len is wrong!!%d>%d\n",section_length,data_len);
								break;
							}
							DVB_ADS_DEBUG(DVBAD,INFO,"gw     sec_num=%d   last_sec_num=%d \n",p_section_data[6],p_section_data[7]);
							ad_ret = add_node_to_fl(table_extension_id, p_section_data, section_length);//xml_info data
							if (ad_ret == DVBAD_FUN_ONE_FILTER_COMPELET)
							{
								break;
							}	
							p_section_data += section_length;
							data_len -= section_length;

						}	
						if (ad_ret == DVBAD_FUN_ONE_FILTER_COMPELET )//all sec of one ext_id has been saved
						{
							DVB_ADS_DEBUG(DVBAD,INFO,"Free XML or PIC_INFO filter!!!");
							dvbad_free_filter_ads(index);
							/*check if all table id have receive all section*/
							if (dvbad_check_adhead_complete(head_type))						
							{
								RET_CODE r;
								if (head_type == DATA_TYPE_XML)/*parser xml data open pic info filter*/
								{
									DVB_ADS_DEBUG(DVBAD,INFO,"Start to parse the XML data!!!");
									r = parser_head_xml_data();
									DVB_ADS_DEBUG(DVBAD,INFO,"Free the xml save list!!!");
									free_fl_all();
									if (r != SUCCESS)
									{
										DVB_ADS_DEBUG(DVBAD, ERR, "parser xml data failure!!\n");
										exit_monitor(GET_AD_FAILURE);
									}
									DVB_ADS_DEBUG(DVBAD,INFO,"Xml data has been parsed out successful!!!\n");
									table_id = ad->down_ctl.ads_pic_info.pic_information_table_id_start;
									last_table_id = ad->down_ctl.ads_pic_info.pic_information_table_id_end;
									DVB_ADS_DEBUG(DVBAD,INFO,"pic_infomation_table_id_start = %d,pic_infomation_table_id_end = %d",table_id,last_table_id);
									DVB_ADS_DEBUG(DVBAD,INFO,"Start to filter the pic info data!!!");
									dvbad_set_filter_for_adhead(table_id, last_table_id,DATA_TYPE_PIC);
								}
								else 	/*parser pic info*/
								{
									DVB_ADS_DEBUG(DVBAD,INFO,"Start to parse the pic info data!!!");
									r = parser_head_pic_info_data();
									DVB_ADS_DEBUG(DVBAD,INFO,"Free the pic info save list!!!");
									free_fl_all();
									if (r  != SUCCESS)
									{
										DVB_ADS_DEBUG(DVBAD, ERR, "parser pic info data failure!!\n");
										exit_monitor(GET_AD_FAILURE);
									}

									dvbad_free_filter_ads(index);
									DVB_ADS_DEBUG(DVBAD,INFO,"parser head pic data success!!!");
									DVB_ADS_DEBUG(DVBAD,INFO,"start to filter pic data!!!");
									DVB_ADS_DEBUG(DVBAD,INFO,"Start to set filter for all ads pic data");
									//start to filter one pic data secs
									dvbad_set_filter_for_pic_data();
									DVB_ADS_DEBUG(DVBAD,INFO,"End set filter for all ads pic data");
								}
							}
						}
					}
				}
				else if((DATA_PID == ad->filter[index].channel_pid)&&(AD_DATA_TABLE_ID == p_data[0]))
				{
					//get fl check if fl finish   save
					u16 pic_pid = (((u16)p_data[3])<<8)|((u16)p_data[4]);//ext_id
					DVB_ADS_PRINTF("\n");
					DVB_ADS_DEBUG(DVBAD,INFO,"PIC DATA fitlered out!!!");
					DVB_ADS_DEBUG(DVBAD, INFO, "index = %d,filter_id = %d,pic id: %d, sec [ %d | %d ]!!\n",\
							index,ad->filter[index].filter_id,pic_pid,p_data[6],p_data[7]);					

					p_section_data = p_data;
					while(data_len > 6)
					{
						section_length = ((p_section_data[1] & 0x0F) << 8) + p_section_data[2] + 3;
						if(section_length > data_len)
						{
							ADS_ERROR("section len is wrong!!%d>%d\n",section_length,data_len);
							break;
						}
						printf("gw     sec_num=%d   last_sec_num=%d \n",p_section_data[6],p_section_data[7]);
						ad_ret = add_node_to_fl(table_extension_id, p_section_data, section_length);//xml_info data
						if (ad_ret == DVBAD_FUN_ONE_FILTER_COMPELET)
						{
							break;
						}	
						p_section_data += section_length;
						data_len -= section_length;
					}	
					if (ad_ret == DVBAD_FUN_ONE_FILTER_COMPELET)
					{

						filter_buf_node * p_node;
						DVB_ADS_DEBUG(DVBAD,INFO,"+++++++++++++++++++++++++++ONE pic:%u has been filtered out completly!!!",(unsigned int)pic_pid);
						dvbad_free_filter_ads(index);
						/*save data to list and change status*/
						p_node = get_fl(pic_pid);
						if (p_node == INVLAD_NODE_PRT)
						{
							DVB_ADS_DEBUG(DVBAD, ERR, "The pic id: %d  fl error!\n",pic_pid);					
							break;
						}
						//IN this func set the FINISH flag for one pic
						save_filter_pic_data(p_node,pic_pid);
						DVB_ADS_DEBUG(DVBAD,INFO,"The pic:%u has been saved to the global var!!!",pic_pid);				
						/*release fl*/
						free_fl(pic_pid);

						/*check if need to set filter*/
						//DVB_ADS_PRINTF("\b");
						DVB_ADS_DEBUG(DVBAD,INFO,"Start to set filter for one ads pic data");
						//alloc filter for the next pic data
						fret = dvbad_set_filter_for_pic_data();

						if(DVBAD_FUN_FILTER_COMPELET == fret)
						{
							DVB_ADS_DEBUG(DVBAD,INFO,"ALL the ads pic in pic_info has been filtered out completely!!!");
						}

						DVB_ADS_DEBUG(DVBAD,INFO,"End set filter for one ads pic data");
						/*check if all pic data have received*/
						if (dvbad_check_addata_complete())
						{
							RET_CODE ret_logo = SUCCESS;
						//	RET_CODE ret_pic = SUCCESS;
							DVB_ADS_DEBUG(DVBAD,INFO,"++++++++++++++++++++++++++++++All the pic data has been filtered out!!!\n\n\n\n\n");
							/*save flash*/
							free_fl_all();
						//	app_dvb_ads_sem_destroy(ad_save_info.ad_opt_sem);
							if(SUCCESS == dvb_ads_get_logo_num(&pid))
							{
								dvb_set_ad_flag(GET_AD_TRUE);
								DVB_ADS_DEBUG(DVBAD,INFO,"Start to save the logo data!!!");
								ret_logo = dvb_ads_save_logo_data();
								if(SUCCESS == ret_logo)
								{
									DVB_ADS_DEBUG(DVBAD,INFO,"LOGO SAVED SUCCESS !!! ");
								}
								else
								{
									DVB_ADS_DEBUG(DVBAD,INFO,"LOGO SAVED FAILED !!! ");
								}
							}		
							else 
							{
								DVB_ADS_DEBUG(DVBAD, INFO, "Logo file number get failed !!! ");
							}
							DVB_ADS_DEBUG(DVBAD,INFO,"End saving the logo data!!!\n");

							RET_CODE ret_bat = dvb_ads_save_flash_bat_data();
							if(ret_bat == SUCCESS)
							{
								DVB_ADS_DEBUG(DVBAD,INFO,"The bat info has been saved to flash!!!");
							}
							exit_monitor(GET_AD_TRUE);
						}
					}
				}
				else
				{
					DVB_ADS_DEBUG(DVBAD,INFO,"Not DVB ADS data!!!");
				}
			}
			DVB_ADS_DEBUG(DVBAD,ERR,"No data queryed!!!");
		}
	}
	dvb_mempool_free(&ad->mem,p_data,MAX_PSI_SECTION_BUFFER);
}

//DONE
/* get the version from global var*/
RET_CODE get_AD_version(u16 *param)
{
	dvbad_priv_t *ad = &s_dvbad;

	if((NULL==param)||(NULL==ad->p_ad_data))
		return ERR_FAILURE;
	*param = ad->p_ad_data->ad_head_data.AD_version;
	return SUCCESS;
}

#if 0
static RET_CODE save_ads_info_to_file(void)
{
	//BOOL r = FALSE;
	handle_t file_handle;
	dvbad_priv_t *ad = &s_dvbad;
	app_dvb_ads_sem_take(ad_save_info.ad_opt_sem,10);
	memcpy(&(ad_save_info.ad_head_data),&(ad->p_ad_data->ad_head_data),sizeof(ad_head_flash));
	memcpy(&(ad_save_info.ad_bat_data),&(ad->p_ad_data->ad_bat_data),sizeof(bat_data));
	ad_save_info.flag = TRUE;
	app_dvb_ads_sem_post(ad_save_info.ad_opt_sem);
	if(GXCORE_FILE_UNEXIST == GxCore_FileExists(DVB_FLASH_FILE_PATH))
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"FILE not exit");
		return ERR_FAILURE;
	}
	GxCore_FileDelete(DVB_FLASH_FILE_PATH);
	file_handle = GxCore_Open(DVB_FLASH_FILE_PATH,"a+");
	size_t size = GxCore_Write(file_handle,&ad_save_info,1,sizeof(ad_save_info));
	if(size > 0)
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"AD HEAD INFO has been written in file!!!");
		return SUCCESS;	
	}
	return ERR_FAILURE;
}

static RET_CODE get_ads_info_from_file(void)
{
	handle_t file_handle;
	//dvbad_priv_t *ad = &s_dvbad;
	if(GXCORE_FILE_UNEXIST == GxCore_FileExists(DVB_FLASH_FILE_PATH))
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"FILE not exit");
		return ERR_FAILURE;
	}
	file_handle = GxCore_Open(DVB_FLASH_FILE_PATH,"r");
	size_t size = GxCore_Read(file_handle,&ad_save_info,1,sizeof(ad_save_info_t));
	if((size > 0) && (TRUE == ad_save_info.flag))
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"Get AD INFO success!!!");
		return SUCCESS;
	}
	DVB_ADS_DEBUG(DVBAD,INFO,"Get AD INFO failed!!!");
	return ERR_FAILURE;
}
#endif


void dvbad_ads_client_init(void)
{
	u32 *p_stack = NULL;
	RET_CODE ret = ERR_FAILURE;
	dvbad_priv_t *ad = &s_dvbad;

	app_dvb_ads_sem_create((os_sem_t *)&s_dvbad.lock,0);
	p_stack = (u32*)app_porting_ca_os_malloc(DVB_POOL_BLOCK_SIZE*DVB_POOL_BLOCK_CNT);
	if (p_stack == NULL)
	{
		DVB_ADS_DEBUG(DVBAD,ERR,"The mempool data start addr alloc failed!");
		dvb_set_ad_flag(GET_AD_FAILURE);
		return;
	}
	ret = dvb_ads_mempool_create(&s_dvbad.mem,DVB_POOL_BLOCK_SIZE,DVB_POOL_BLOCK_CNT,(u8*)p_stack);
	if(ERR_FAILURE == ret)
	{
		dvb_set_ad_flag(GET_AD_FAILURE);
		DVB_ADS_DEBUG(DVBAD,ERR,"The mempool created failed!!!");
	}
	DVB_ADS_DEBUG(DVBAD,INFO,"Create mempool success!!!");

	DVB_ADS_DEBUG(DVBAD,INFO,"alloc from mempool sizeof(dvbad_ads_pid_data) = %d",sizeof(dvbad_ads_pic_data));
	ad->p_ad_data = dvb_ads_mempool_alloc(&s_dvbad.mem,sizeof(dvbad_ads_pic_data));
	if(NULL == ad->p_ad_data)
	{
		DVB_ADS_DEBUG(DVBAD,ERR,"Alloc %s from mempool failed!!!","dvbad_ads_pic_data");
	}
	memset(ad->p_ad_data, 0, sizeof(dvbad_ads_pic_data));
#if 0
	//read out the ad version and version number data from flash!!!

	if(GXCORE_FILE_UNEXIST == GxCore_FileExists(DVB_FLASH_FILE_PATH))
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"The file of version info unexit in flash,maybe running for the first time!!!");
	}
	else{
		ret = get_ads_info_from_file();
		if(ERR_FAILURE)
		{
			DVB_ADS_DEBUG(DVBAD,WARNING,"Version info in flash get failed!!!");
		}
		DVB_ADS_DEBUG(DVBAD,INFO,"Version info in flash get success!!!");
	}
#endif
	//p_stack remember to free the space of p_stack after exit the thread!!!
	p_stack = (u32 *)dvb_ads_mempool_alloc(&s_dvbad.mem,ADS_DVBAD_TASK_STKSIZE);
	DVB_ADS_DEBUG(DVBAD,INFO,"Alloc mem for thread addr:0x%x,size:%dKB!!!",(unsigned int)p_stack,(ADS_DVBAD_TASK_STKSIZE/KBYTES));
	DVB_ADS_ASSERT((NULL != p_stack));
	//create thread to filter and save data.the main thread only need to show ads!!!
	int32_t ret_thread =  dvb_ads_thread_create("ads_monitor", ads_dvbad_data_monitor, NULL, p_dvbad_priv->task_priority, p_stack, ADS_DVBAD_TASK_STKSIZE);
	if(0 == ret_thread)
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"Thread create success!!!priority = %d ",p_dvbad_priv->task_priority);
		return;
	}
	DVB_ADS_DEBUG(DVBAD,ERR,"Thread create failed!!!");
	dvb_set_ad_flag(GET_AD_FAILURE);
	return;
}

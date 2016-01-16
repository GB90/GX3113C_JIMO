#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "dvb_ads_xml_parser.h"
#include "dvb_parser.h"
#include "dvb_tree.h"
#include "dvb_ads_debug.h"

#define ADS_CHANNER_TEXT_MAX_LONG	256

extern dvb_mempool *dvbad_get_memp(void);

ads_data_node* create_list_data(ads_pic_attribute data)
{
	ads_data_node* p_data_node = NULL;
	dvb_mempool* mem = dvbad_get_memp();
	p_data_node = dvb_ads_mempool_alloc(mem,sizeof(ads_data_node));
	DVB_ADS_ASSERT((NULL!=p_data_node));
	memset((void*)p_data_node, 0, sizeof(ads_data_node));
	memcpy(&p_data_node->ads_pic_attribute_data, &data, sizeof(ads_pic_attribute));
	p_data_node->next = NULL;
	return p_data_node;
}

void delete_node(ads_data_node** pNode)  
{  
	ads_data_node** pNext;	
	dvb_mempool *mem = dvbad_get_memp();
	
	if(NULL == pNode || NULL == *pNode)  
		return ;  
		  
	pNext = &(*pNode)->next;  
	if(NULL!=(*pNode)->p_ads_pic_data)
	{
		dvb_ads_mempool_free(mem,(*pNode)->p_ads_pic_data,(*pNode)->p_pic_info.pic_original_size);
		(*pNode)->p_ads_pic_data = NULL;
	}
	if(NULL!=(*pNode)->ads_pic_attribute_data.text)
	{
		dvb_ads_mempool_free(mem,(*pNode)->ads_pic_attribute_data.text,(*pNode)->text_size);
		(*pNode)->ads_pic_attribute_data.text = NULL;
	}
	delete_node(pNext);   
	dvb_ads_mempool_free(mem,(u8*)(*pNode),sizeof(ads_data_node));
	*pNode = NULL;
}  

static BOOL add_list_data( ads_data_node** p_node, ads_pic_attribute data)
{
	if(NULL==p_node)
	{
		DVB_ADS_DEBUG(DVBAD, ERR,"p_node == NULL\n");
		return FALSE;
	}
	memset((void*)&(*p_node)->ads_pic_attribute_data, 0, sizeof(ads_pic_attribute));
	memcpy(&(*p_node)->ads_pic_attribute_data, &data, sizeof(ads_pic_attribute));
	(*p_node)->next = NULL;	
	return TRUE;
}

BOOL add_data(const ads_data_node** p_node, ads_pic_attribute data)  
{  
	ads_data_node* p_data_node;  
	ads_data_node *p_add_node;
	if(NULL == *p_node)  
		return FALSE;  

	p_data_node = create_list_data(data);
	DVB_ADS_ASSERT((NULL != p_data_node));  
	p_add_node = (ads_data_node*)*p_node;
	while(NULL!= p_add_node->next)
		p_add_node = p_add_node->next;
	
	p_add_node->next = p_data_node;
	return TRUE;
}  

ads_data_node* last_list_data(ads_data_node* p_data_list)
{	
	if(NULL==p_data_list->next)
		return p_data_list;
	return last_list_data(p_data_list->next);
}

static int ads_get_adhead_count(const ads_data_node* p_data_node)	
{  
	if(NULL == p_data_node)  
		return 0;  
		  
	return 1 + ads_get_adhead_count(p_data_node->next);  
}  

static DVBAD_ADS_PIC_FORMAT ads_get_pic_format(const CHAR*p_buf)
{
	if(!xmlStrcmp(p_buf, PIC_FORMAT_GIF_STR))
		return ADS_PIC_FORMAT_GIF;
	else if(!xmlStrcmp(p_buf, PIC_FORMAT_BMP_STR))
		return ADS_PIC_FORMAT_BMP;
	else if(!xmlStrcmp(p_buf, PIC_FORMAT_JPG_STR))
		return ADS_PIC_FORMAT_JPG;
	else if(!xmlStrcmp(p_buf, PIC_FORMAT_MPG_STR))
		return ADS_PIC_FORMAT_MPG;
	else if(!xmlStrcmp(p_buf, PIC_FORMAT_TIF_STR))
		return ADS_PIC_FORMAT_TIF;
	else if(!xmlStrcmp(p_buf, PIC_FORMAT_PNG_STR))
		return ADS_PIC_FORMAT_PNG;
	else if(!xmlStrcmp(p_buf, PIC_FORMAT_DIB_STR))
		return ADS_PIC_FORMAT_DIB;
	else 
		return ADS_PIC_FORMAT_UNKNOW;
}

static DVBAD_POS_TYPE get_submenu_ad_type(const CHAR*p_buf)
{
	if((!xmlStrcmp(p_buf, AD_TYPE_EPG_STR))||(!xmlStrcmp(p_buf, AD_TYPE_EPG1_STR)))
		return DVBAD_STB_EPG;
	else if((!xmlStrcmp(p_buf, AD_TYPE_CHLIST_STR))||(!xmlStrcmp(p_buf, AD_TYPE_CHLIST1_STR)))
		return DVBAD_STB_CHLIST;
	else if((!xmlStrcmp(p_buf, AD_TYPE_VOLUME_STR))||(!xmlStrcmp(p_buf, AD_TYPE_VOLUME1_STR)))
		return DVBAD_STB_VOLUME;
	else 
		return DVBAD_STB_POS_UNKNOWN;
}

static RET_CODE ads_xml_cfg_parse(ads_rcv_config *p_dvbad_cfg,xmlNodePtr cur)
{
	memcpy(p_dvbad_cfg->ads_area_name,RCV_CONFIGR_STR, sizeof(p_dvbad_cfg->ads_area_name));
	if(NULL==cur)
		return FALSE;
	if (!xmlStrcmp(cur->name, CFG_AREA_STR))
	{
		memcpy(p_dvbad_cfg->ads_area_name, xmlGetProp(cur, ATTRIBUTE_NAME_STR), sizeof(p_dvbad_cfg->ads_area_name));
		cur = XML_GET_CHILD(cur);
		if(NULL==cur)
			return FALSE;
	}
	if (!xmlStrcmp(cur->name, CFG_MANUFACTURER_STR))
	{
		memcpy(p_dvbad_cfg->ads_manufacturer_name, xmlGetProp(cur, ATTRIBUTE_NAME_STR), sizeof(p_dvbad_cfg->ads_manufacturer_name));
		cur = XML_GET_CHILD(cur);
		if(NULL==cur)
			return FALSE;
	}
	if (!xmlStrcmp(cur->name, CFG_RCVTYPE_STR))
	{
		memcpy(p_dvbad_cfg->rcvtype_name, xmlGetProp(cur, ATTRIBUTE_NAME_STR), sizeof(p_dvbad_cfg->rcvtype_name));
		cur = XML_GET_CHILD(cur);
		if(NULL==cur)
			return FALSE;
	}
	if (xmlStrcmp(cur->name, CFG_PORTION_STR))
	{
		memcpy(p_dvbad_cfg->portion_name, xmlGetProp(cur, ATTRIBUTE_NAME_STR), sizeof(p_dvbad_cfg->portion_name));
		cur = XML_GET_CHILD(cur);
	}
	while(NULL==cur)
	{
		if (!xmlStrcmp(cur->name, CFG_STB_START_ID_STR))
			p_dvbad_cfg->stb_start_id = (u32)atoi((const char *)xmlNodeGetContent(cur));
		else if(!xmlStrcmp(cur->name, CFG_STV_END_ID_STR))
			p_dvbad_cfg->stb_end_id = (u32)atoi((const char *)xmlNodeGetContent(cur));
	}

	return TRUE;
}

static RET_CODE ads_xml_pic_attribute_parse(ads_data_node *p_dvbad_pic_attribute_list,const xmlNodePtr cur_pic, DVBAD_POS_TYPE type)
{
	xmlNodePtr cur = cur_pic;
	int data_NULL_flag = 0;
	ads_pic_attribute dvbad_ads_pic_attribute;
	
	if(NULL==cur)
		return -1;
	memset((void*)&dvbad_ads_pic_attribute, 0, sizeof(ads_pic_attribute));
	
	if((xmlStrcmp(cur->name, AD_NAME_STR))&&(xmlStrcmp(cur->name, LOGI_NAME_STR)))
		return 0;
	memcpy(dvbad_ads_pic_attribute.name, xmlGetProp(cur, ATTRIBUTE_NAME_STR), sizeof(dvbad_ads_pic_attribute.name));
	cur = XML_GET_CHILD(cur);
	while(NULL!=cur){
		if(!xmlStrcmp(cur->name, FORMAT_STR))
			dvbad_ads_pic_attribute.p_format = ads_get_pic_format(xmlNodeGetContent(cur));
		else if(!xmlStrcmp(cur->name, POSITION_X_STR))
			dvbad_ads_pic_attribute.x = (u16)atoi((const char *)xmlNodeGetContent(cur));
		else if(!xmlStrcmp(cur->name, POSITION_Y_STR))
			dvbad_ads_pic_attribute.y = (u16)atoi((const char *)xmlNodeGetContent(cur));
		else if(!xmlStrcmp(cur->name, WIDTH_STR))
			dvbad_ads_pic_attribute.w = (u16)atoi((const char *)xmlNodeGetContent(cur));
		else if(!xmlStrcmp(cur->name, HEIGHT_STR))
			dvbad_ads_pic_attribute.h = (u16)atoi((const char *)xmlNodeGetContent(cur));
		else if(!xmlStrcmp(cur->name, DURATION_STR))
			dvbad_ads_pic_attribute.duration = (u16)atoi((const char *)xmlNodeGetContent(cur));
		else if(!xmlStrcmp(cur->name, FILE_VERSION_STR))
			dvbad_ads_pic_attribute.version = (u16)atoi((const char *)xmlNodeGetContent(cur));
		else if(!xmlStrcmp(cur->name, FILE_NUME_STR))
			dvbad_ads_pic_attribute.file_number = (u16)atoi((const char *)xmlNodeGetContent(cur));
		
		cur = cur->next;
		data_NULL_flag = 1;
	}
	DVB_ADS_DEBUG(DVBAD,INFO,"XML receive file_number = %d name:%s\n",dvbad_ads_pic_attribute.file_number,dvbad_ads_pic_attribute.name);
	DVB_ADS_DEBUG(DVBAD,INFO,"XML parse:\n");
	DVB_ADS_PRINTF("\t\tfile_number:\t%d\n\t\tversion:\t\t%d\n\t\tduration:\t\t%d\n\t\tx:\t\t\t%d\n\t\ty:\t\t\t%d\n\t\tw:\t\t\t%d\n\t\th:\t\t\t%d\n\t\tp_format:\t\t%d\n\t\ttype:\t\t\t%d\n",dvbad_ads_pic_attribute.file_number,				dvbad_ads_pic_attribute.version,				dvbad_ads_pic_attribute.duration,				dvbad_ads_pic_attribute.x,				dvbad_ads_pic_attribute.y,				dvbad_ads_pic_attribute.w,				dvbad_ads_pic_attribute.h,				dvbad_ads_pic_attribute.p_format,type);
	if(data_NULL_flag)
	{
		if(NULL==p_dvbad_pic_attribute_list)
		{
			DVB_ADS_DEBUG(DVBAD,ERR,"Error!NULL==dvbad_pic_attribute_list\n");
			return -1;
		}
		else
		{
			if(0==p_dvbad_pic_attribute_list->ads_pic_attribute_data.type)
			{
				dvbad_ads_pic_attribute.type = type;
				add_list_data(&p_dvbad_pic_attribute_list, dvbad_ads_pic_attribute);
			}
			else
			{
				dvbad_ads_pic_attribute.type = type;
				add_data((const ads_data_node**)&p_dvbad_pic_attribute_list, dvbad_ads_pic_attribute);
			}
		}
	}
	return 0;
}

static RET_CODE ads_xml_text_get(ads_data_node *p_node, const xmlNodePtr cur, u32 xml_len)
{
	dvb_mempool* mem = dvbad_get_memp();

	if((NULL==p_node)||(NULL==cur))
		return FALSE;

	if(!xmlStrcmp(cur->name, CHANNER_TEXT_STR))
	{
		p_node->text_size= MIN(ADS_CHANNER_TEXT_MAX_LONG,(u16)xml_len);
		p_node->ads_pic_attribute_data.text = dvb_ads_mempool_alloc(mem,p_node->text_size);
		memcpy(p_node->ads_pic_attribute_data.text, xmlNodeGetContent(cur), p_node->text_size);
		return TRUE;
	}

	return FALSE;
}

static RET_CODE ads_xml_type_parse(xmlNodePtr cur, dvbad_ads_pic_data *ads_pic_attribute_data, u32 buf_len)
{
#define CHANNEL_TYPE	2
	int i;
	xmlNodePtr cur_pic_attribute;
	xmlNodePtr cur_econd_title;
	xmlNodePtr cur_stripe;
	xmlNodePtr cur_type;
	xmlChar *channel_type[2]={CURRENT_FOLLOW_STR, CHANNER_INFO_STR};
	ads_data_node *p_last_data_list = NULL;
	DVBAD_POS_TYPE ads_type = DVBAD_STB_POS_UNKNOWN;

	if(NULL==ads_pic_attribute_data)
	{
		DVB_ADS_DEBUG(DVBAD, ERR, "ads_pic_attribute_data == NULL\n");
		return ERR_FAILURE;
	}
	while(NULL!=cur){
		if(!xmlStrcmp(cur->name, BOOTLOGO_STR))
		{
			cur_econd_title = cur;
			ads_type = DVBAD_STB_LOG;
			cur_pic_attribute = XML_GET_CHILD(cur_econd_title);
			while(NULL!=cur_pic_attribute)
			{	
					if(-1==ads_xml_pic_attribute_parse(
							&ads_pic_attribute_data->dvbad_pic_attribute_node,
							cur_pic_attribute, ads_type))
					{
						return ERR_FAILURE;
					}
				cur_pic_attribute = cur_pic_attribute->next;
			}
			cur = cur->next;
			continue;
		}
		else if(!xmlStrcmp(cur->name, MAINMENU_STR))
		{
			cur_econd_title = cur;
			ads_type = DVBAD_STB_MAINMENU;
			cur_pic_attribute = XML_GET_CHILD(cur_econd_title);			
			while(NULL!=cur_pic_attribute)
			{	
				if(cur_pic_attribute)
				{
					if(-1==ads_xml_pic_attribute_parse(&ads_pic_attribute_data->dvbad_pic_attribute_node, 
											cur_pic_attribute, ads_type))
					{
						return ERR_FAILURE;
					}
				}
				cur_pic_attribute = cur_pic_attribute->next;
			}
			cur = cur->next;
			continue;
		}
		else if(!xmlStrcmp(cur->name, SUBMENUS_STR))
		{
			cur_econd_title = cur;
			cur_type = XML_GET_CHILD(cur);/*   <Menu name="EPG">*/
			while(NULL!=cur_type)
			{	
				cur_pic_attribute = XML_GET_CHILD(cur_type); /*<ad name="ad1">*/
				while(NULL!=cur_pic_attribute)
				{
					ads_type = DVBAD_STB_POS_UNKNOWN;
					if(-1==ads_xml_pic_attribute_parse(&ads_pic_attribute_data->dvbad_pic_attribute_node, 
														cur_pic_attribute, ads_type))
					{
						return ERR_FAILURE;
					}
					p_last_data_list = last_list_data(&ads_pic_attribute_data->dvbad_pic_attribute_node);
					memcpy(p_last_data_list->ads_pic_attribute_data.type_name,
						xmlGetProp(cur_type, ATTRIBUTE_NAME_STR),
						sizeof(p_last_data_list->ads_pic_attribute_data.type_name));
					DVB_ADS_DEBUG(DVBAD,INFO,"p_last_data_list->ads_pic_attribute_data.type_name:%s\n",p_last_data_list->ads_pic_attribute_data.type_name);					
					
					p_last_data_list->ads_pic_attribute_data.type = get_submenu_ad_type(p_last_data_list->ads_pic_attribute_data.type_name);
					cur_pic_attribute = cur_pic_attribute->next;
				}
				cur_type = cur_type->next;
			}
			cur = cur->next;
			continue;
		}
		else if(!xmlStrcmp(cur->name, STRIPE_STR))
		{
			cur_econd_title = cur;
			cur_type = XML_GET_CHILD(cur);
			for(i=0;i<2;i++)
			{
				if(NULL==cur_type)
					break;
				if(!xmlStrcmp(cur_type->name, channel_type[i]))
				{
					cur_stripe = XML_GET_CHILD(cur_type);
					while(NULL!=cur_stripe)
					{	
						cur_pic_attribute = XML_GET_CHILD(cur_stripe);
						if(NULL!=cur_pic_attribute)
						{
							if(0==i)
								ads_type = DVBAD_STB_CURRENT_FOLLOW;
							else 
								ads_type = DVBAD_STB_CHANNEL_INFO;
							if(-1==ads_xml_pic_attribute_parse(&ads_pic_attribute_data->dvbad_pic_attribute_node, cur_pic_attribute, ads_type))
								return ERR_FAILURE;
							p_last_data_list = last_list_data(&ads_pic_attribute_data->dvbad_pic_attribute_node);
 							memcpy(p_last_data_list->ads_pic_attribute_data.type_name,xmlGetProp(cur_stripe, ATTRIBUTE_NAME_STR),sizeof(p_last_data_list->ads_pic_attribute_data.type_name));	
							DVB_ADS_DEBUG(DVBAD,INFO,"The type_name or service id of this current follow pic is %d",(u16)atoi((char*)p_last_data_list->ads_pic_attribute_data.type_name));										
							if(0==i)
								p_last_data_list->ads_pic_attribute_data.channel_type = ADS_CHANNEL_CURRENT_FOLLOW;
							else 
								p_last_data_list->ads_pic_attribute_data.channel_type = ADS_CHANNEL_INFO;

							cur_pic_attribute = cur_pic_attribute->next;
							if(NULL!=cur_pic_attribute)
							{
								if(FALSE==ads_xml_text_get(p_last_data_list, cur_pic_attribute, buf_len))
								{
									DVB_ADS_DEBUG(DVBAD, ERR, "get xml text failure\n");
								}
							}
						}
						cur_stripe = cur_stripe->next;
					}
				}
				cur_type = cur_type->next;
			}
			cur = cur->next;
			continue;
		}
		else if(!xmlStrcmp(cur->name, RCV_CONFIGR_STR))
		{
			cur_econd_title = cur;
			cur_type = XML_GET_CHILD(cur);
			ads_xml_cfg_parse(&ads_pic_attribute_data->dvbad_ads_rcv_config,cur_type);
			cur = cur->next;
			continue;
		}
		else 
		{
			cur_econd_title = cur;
			ads_type = DVBAD_STB_POS_UNKNOWN;	
			cur_pic_attribute = XML_GET_CHILD(cur_econd_title);
			while(NULL!=cur_pic_attribute)
			{	
					if(-1==ads_xml_pic_attribute_parse(
							&ads_pic_attribute_data->dvbad_pic_attribute_node,
							cur_pic_attribute, ads_type))
					{
						return ERR_FAILURE;
					}
				cur_pic_attribute = cur_pic_attribute->next;
			}
		}
		cur = cur->next;
	}

	ads_pic_attribute_data->ads_pic_count = ads_get_adhead_count(&ads_pic_attribute_data->dvbad_pic_attribute_node);
	return SUCCESS;
}


RET_CODE ads_xml_parse(const xmlChar *p_buf, u32 buf_len, dvbad_ads_pic_data *p_ads_pic_data)
{
	xmlDocPtr doc;
	xmlNodePtr root;
	xmlNodePtr type;
	RET_CODE ret;
	

	DVB_ADS_DEBUG(DVBAD, INFO, "xml parse start\n");
	if(NULL==p_buf)
	{
		DVB_ADS_DEBUG(DVBAD,ERR,"Error!NULL==p_buf\n");
		return ERR_FAILURE;
	}
	DVB_ADS_DEBUG(DVBAD,INFO,"start to get doc\n");
	doc = xmlParseMemory((char*)p_buf, buf_len);
	DVB_ADS_DEBUG(DVBAD, INFO, "get doc end\n");
	if(NULL==doc)
	{
		DVB_ADS_DEBUG(DVBAD,ERR,"Error! xmlParseMemory doc failure\n");
		return ERR_FAILURE;
	}
	DVB_ADS_DEBUG(DVBAD,INFO,"Start to get root\n");
	root = XML_GET_ROOT(doc);
	DVB_ADS_DEBUG(DVBAD,INFO,"Get root end\n");
	if(NULL==root)
	{
		DVB_ADS_DEBUG(DVBAD,ERR,"Error! NULL==cur get root failure\n");
		xmlFreeDoc(doc);
	}
	/*
	{
	u32 i;
	for(i = 0; i<buf_len; i++)
		OS_PRINTF("%c", p_buf[i]);
	OS_PRINTF("\n");
	}
	*/
	DVB_ADS_DEBUG(DVBAD,INFO,"Start to get child type\n");
	type = XML_GET_CHILD(root);
	DVB_ADS_DEBUG(DVBAD,INFO,"Get child type end\n");

	DVB_ADS_DEBUG(DVBAD,INFO,"Start to parse type\n");
	ret = ads_xml_type_parse(type, p_ads_pic_data, buf_len);
	DVB_ADS_DEBUG(DVBAD,INFO,"End parse type\n");
	xmlFreeDoc(doc);

	DVB_ADS_DEBUG(DVBAD, INFO, "xml parse end.ret:%u\n", (unsigned int)ret);
	return ret;
}




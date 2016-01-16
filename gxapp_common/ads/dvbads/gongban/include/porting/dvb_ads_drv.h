#ifndef __DVB_ADS_DRV_H__
#define __DVB_ADS_DRV_H__

#include "dvb_ads_type.h"
#include  "dvb_ads_function_porting.h"
#include "dvb_ads_xml_parser.h"

#define DVB_POOL_BLOCK_SIZE 	1*KBYTES	
#define DVB_POOL_BLOCK_CNT 	100

#define MAX_PSI_SECTION_BUFFER				(64*KBYTES)

#define DVBAD_ADS_PIC_DATA_SIZE				(480*576)
#define ADS_DVBAD_TASK_STKSIZE    			(128 * KBYTES)

#define DVB_BAT_PID					(0x1F41)

#define DATA_PID					(0x1F40)
#define AD_HEAD_TABLE_ID				(0xFE)
#define AD_DATA_TABLE_ID				(0xFD)

#define DATA_TYPE_XML					(0x01)
#define DATA_TYPE_PIC					(0x02)

#define EXT_TABLEID_CTR_INFO				(0)

#define DVB_ADS_FILTER_MAX_NUM    			(16)

#define DMX_INVALID_CHANNEL_ID  			0xFFFF
#define	DVBAD_FILTER_MAX_LEN				(15)

#define ADS_MAX_FLITER_REC				(256)
#define INVALID_PIC_ID					(0xffff)
#define HEAD_DUMMY_PIC_ID				(0xfffe)
#define INVLAD_NODE_PRT					((void*)-1)

typedef enum
{
    DVB_ADS_FREE = 0,
    DVB_ADS_USED,
}dvbad_ads_status_t;

typedef enum
{
	DVBAD_DATA_FREE,
	DVBAD_DATA_TRUE,
	DVBAD_DATA_FINISH,
	DVBAD_DATA_FALSE,
}dvbad_data_return;

typedef struct
{
	u16 					filter_id;
	dvbad_ads_status_t			filter_status;
	handle_t				filter_handle;
	handle_t				channel_handle;
	u16 					channel_pid;
#ifdef DVB_AD_TYPE_DVB_FLAG
	u8					table_id;
	u16 					ext_id;
	u8					sec;
#endif //DVB_AD_TYPE_DVB_FLAG
	u16 		  			filter_num;
	u8					filter_data[DVBAD_FILTER_MAX_LEN];
	u8					filter_mask[DVBAD_FILTER_MAX_LEN];
	u8					* p_buf;
	u32 					timeout;
	void (*callback_func)(u16 pid, u8 *p_data, u16 data_len);
}dvbad_ads_filter_struct_t;

typedef enum{
	DVBAD_TYPE_LOGO=1,
	DVBAD_TYPE_MAINMENU,
	DVBAD_TYPE_SUBMENU,
	DVBAD_TYPE_STRIPE,
	DVBAD_TYPE_TOTAL,
}ADS_DVBAD_TYPE;

typedef struct{
	u16 index_xml_table_id_start;
	u16 index_xml_table_id_end;
	u32 index_xml_size;
}ad_head_xml_info;

typedef struct{
	u16 pic_information_table_id_start;
	u16 pic_information_table_id_end;
	u32 pic_information_size;
}ad_head_pic_info;

typedef struct{
	ad_head_xml_info ads_xml_info;
	ad_head_pic_info ads_pic_info;
}down_ctrl_info;

typedef struct _filter_buf_node
{
	u8 *p_data;
	u32 data_len;
	struct _filter_buf_node* next;
}filter_buf_node;

typedef struct filter_recoder
{
	u16 pic_id; // 0xffff  means it is not be used  (ext_tableId)
	u16 filter_id;
	u16 complete;
	filter_buf_node *node;
}filter_recoder_t;

typedef struct{
	u8 set_section_number;
	u8 set_last_section_number;
	u8 set_data_type;
	u16 set_table_id;
	BOOL new_table_flag;
}set_filter_status;

typedef struct dvbad_priv_t{
 	os_sem_t 			lock;
  	dvbad_ads_filter_struct_t 	filter[DVB_ADS_FILTER_MAX_NUM];
 	ST_ADS_DVBAD_FILTER_INFO 	filter_data[DVB_ADS_FILTER_MAX_NUM];
 	//u8* 				filter_buf;
	down_ctrl_info 			down_ctl;
	u8 * 				xml;//xml data buffer
	u32				xml_pos;//databuffer write pointer
	u8 * 				pic_info;
	u32				pic_info_pos;
	u8 *				pic_data;
	u32				pic_data_pos;
	dvbad_ads_pic_data		*p_ad_data;
	dvb_mempool			mem;	
	u32 				monitor_count;
}dvbad_priv_t;


typedef struct _ad_save_info
{
	ad_head_flash ad_head_data;
	bat_data ad_bat_data;
	BOOL flag;
	char reserved[3];
	os_sem_t ad_opt_sem;
} ad_save_info_t;

//record one pic_id fitlered out completed or not
typedef struct _ad_pic_filter_flag
{
	u16 pic_id;
	u8 sec_cnt;
	u8 flag;
	struct _ad_pic_filter_flag* next;
} ad_pic_filter_flag_t;


//----------------------------------------------------
void dvbad_ads_client_init(void);

BOOL dvbad_set_filter_bat(void);

ads_data_node *dvbad_get_data_node(ads_data_node *p_node, DVBAD_POS_TYPE ad_type);
RET_CODE dvbad_get_ad(IN dvbad_program_para *para, dvbad_show_pic_info *p_pic, DVBAD_POS_TYPE ad_type);

RET_CODE get_AD_version(u16 *param);

dvb_mempool* dvbad_get_memp(void);

RET_CODE dvbad_get_ad_by_type_name(dvbad_show_pic_info *p_pic,char* type_name);



#endif //__DVB_ADS_DRV_H__


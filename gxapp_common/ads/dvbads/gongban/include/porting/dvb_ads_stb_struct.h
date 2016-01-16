#ifndef __DVB_ADS_STB_STRUCT_H__
#define __DVB_ADS_STB_STRUCT_H__



#define IN
#define OUT

#define DVB_LOGO_PATH			("/home/gx/logo.bin")
#define DVB_LOGO_CFG_PATH		("/home/gx/logo.cfg")

#define PRIORITY_LEVEL_DEFAULT 		(10)

#define	DVBAD_FILTER_MAX_LEN		(15)

//for saving logo config data
#define DVB_LOGO_START_ADDR		0x30000
#define DVB_LOGO_MAX_LEN 		128*KBYTES

#define DVB_FLASH_FILE_PATH 		("/home/flash.bin")
#define DVB_FLASH_FILE_SIZE 		(1152*KBYTES)

void ads_ap_init(void);

u8* ads_get_ad_version(void);

typedef struct{
	BOOL valid_flag;
	u16 stream_id;
	u16 network_id;
	u16 service_id;
	u32 frequency;
	u32 symbol_rate;
	u16 modulation;
}bat_data;

typedef struct{
	u16 ad_pic_num;
	u8 version_number;
	u16 AD_version;
	BOOL save_all_pic_flag;
	u32 buff_size;
	char sum;
}ad_head_flash;

typedef enum{
	ADS_PIC_FORMAT_GIF = 1,
	ADS_PIC_FORMAT_PNG,
	ADS_PIC_FORMAT_DIB,
	ADS_PIC_FORMAT_BMP,
	ADS_PIC_FORMAT_JPG,
	ADS_PIC_FORMAT_MPG,
	ADS_PIC_FORMAT_TIF,
	ADS_PIC_FORMAT_UNKNOW,
}DVBAD_ADS_PIC_FORMAT;

typedef enum
{
	DVBAD_STB_LOG = 1,
	DVBAD_STB_EPG = 2,          
	DVBAD_STB_VOLUME = 3,
	DVBAD_STB_MAINMENU = 4,
	DVBAD_STB_CHLIST = 5,
	DVBAD_STB_CURRENT_FOLLOW = 6,
	DVBAD_STB_CHANNEL_INFO = 7,
	DVBAD_STB_POS_UNKNOWN,
}DVBAD_POS_TYPE;

//pic show info
typedef struct{
	DVBAD_POS_TYPE 		pos_type;
	DVBAD_ADS_PIC_FORMAT	pic_type;
	u16			x;
	u16			y;
	u16			w;
	u16			h;
	//to check the sum menu ad 
	u8 			type_name[8];
	//to check the program concerned 
	u16 			service_id;
	//------------------------------
	u32			data_len;
	u16			pic_id;
	u8*			pic_data;
}dvbad_show_pic_info;

typedef enum 
{
	DVBAD_FUN_ERR_PARA,
	DVBAD_FUN_NO_ELEMENT,
	DVBAD_FUN_NOT_SUPPORT,
	DVBAD_FUN_FLASH_NOT_READY,
	DVBAD_FUN_ONE_FILTER_COMPELET,
	DVBAD_FUN_FILTER_COMPELET,
	DVBAD_FUN_OK,
}ST_AD_DVBAD_FUNC_TYPE;

typedef struct _ST_DVBAD_PROGRAM_PARA
{
	u16 usNetworkId;
	u16 usTsId;
	u16 usServiceId;
}dvbad_program_para;

typedef void (* FilterNotifyFunction)(u16 usPid, IN u8* pucDataBuffer, u16 usDataLen);

typedef struct _ST_DVBAD_DVBAD_FILTER_INFO
{
	handle_t channel_handle;
	handle_t filter_handle;
	u32 filter_id;//8
	u16 usChannelPid;//16
	u8 ucFilterLen;//8
	u8 aucFilter[18];//15*8
	u8 aucMask[18];//15*8
	u8 ucWaitSeconds;
	FilterNotifyFunction pfDataNotifyFunction;
}ST_ADS_DVBAD_FILTER_INFO;

typedef enum{
	DVBAD_NEED_UPDATE_YES = 1,
	DVBAD_NEED_UPDATE_NO,
	DVBAD_UPDATE_FAILURE,
}DVBAD_UPDATE_STATUS;

#endif //__DVB_ADS_STB_STRUCT__


/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	3h_ads_porting.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.11.27		  zhouhm 	 			creation
*****************************************************************************/
#include <gxtype.h>
#include "app_common_porting_stb_api.h"
#include "gx_demux.h"
#include "gxapp_sys_config.h"
#include "gui_timer.h"
#include "gxmsg.h"
#include "app_common_porting_ads_flash.h"
#include "divitone_ads_porting.h"
#include "app_divitone_ads_porting_stb_api.h"
#include "app_common_porting_ca_demux.h"
#include "service/gxsearch.h"
#include "app_common_search.h"
#include "app_divitone_cas_api_pop.h"
static uint8_t startPlayFlag = FALSE;
static handle_t ad_channel=0; 
static handle_t ad_filter=0; 
static uint8_t ad_fail = 0; // 1: get ad timeout, 0: get ad success, 2: close hint info. add by zs
#define MAX_ITEMS_COUNT (11)
static uint8_t OsdBuff[1024] ;
static uint32_t gDiviAdsServiceId[80];
static uint8_t gDiviAdsServiceIdCount = 0;


typedef struct ad_section_s
{
	uint8_t      recievedFlag;
	uint8_t*     addata;
	int32_t         len;
}ad_section_t;

#define SECTION_MAX_NUM (500)
typedef struct ad_items_s
{
	uint8_t		ads_Type;
	uint8_t		ads_file_type;
	uint16_t		lastSectionNum;
	uint16_t		recieveNum;
	uint8_t		ads_is_finish;
	uint8_t		ads_is_show;
	uint8_t		ads_show_time;
	uint8_t		ads_show_pos;
	uint32_t	ads_cmd;
	ad_section_t sectionData[SECTION_MAX_NUM];
}ad_items_t;

#ifdef ECOS_OS
extern void cyg_scheduler_lock(void);
extern void cyg_scheduler_unlock(void);
#endif



DIVITONE_AD_PIC_t adsdivitone_para[AD_DIVITONE_TYPE_MAX];
uint8_t      g_itemsCount = 0;
uint8_t      g_itemsversion = 32;
uint32_t      g_Filternms = 0;
uint8_t      g_Filtertimeout = 0;
#define AD_FILTER_TIMEOUT (45)
ad_items_t g_adItems[MAX_ITEMS_COUNT];
uint8_t ad_version_flag = 0;  //addbyzs for if ad version is same, don't hint user "download ad data"
//ad_version_flag is 0: init value.1:version don't change, 2:version changed

#define AD_VERSION                	"ADVersion"
#define AD_VERSION_DV                			32

//addbyzs for if ad version is same, don't hint user "download ad data" begin
uint8_t advertisement_get_ad_version_flag()
{
	return ad_version_flag;
}

void advertisement_set_ad_version_flag(uint8_t version_flag)
{
	ad_version_flag = version_flag;
}
//addbyzs for if ad version is same, don't hint user "download ad data" end

//add by zs 20110810 begin
uint8_t advertisement_set_timeout_flag(uint8_t flag)
{
	ad_fail = flag;
	return 0;
}

uint8_t advertisement_get_timeout_flag()
{
	return ad_fail;
}

uint8_t advertisement_get_startplayflag(void)
{
	return startPlayFlag;
}

uint8_t advertisement_set_startplayflag(uint8_t flag)
{	
	startPlayFlag = flag;
	return startPlayFlag;
}
BOOL divitone_ads_get_osd_for_service_id(uint32_t service_id)
{
	int i = 0;
	for (i=0;i<80;i++)
	{
		if (gDiviAdsServiceId[i]==service_id)
			return TRUE;
	}
	return FALSE;

}

int GxAD_LoadData(void) 
{
	uint8_t Buf[1024]={0};
	int fd;
	size_t   size;					
	handle_t handlejpg;
	handle_t handle;
	char inf_name[100] = {0,};
	char inf_name_mnt[100] = {0,};
	ad_divitone_items_t *pAdShowData = NULL;
	pAdShowData = GxCore_Malloc(sizeof(ad_divitone_items_t));
	if (NULL == pAdShowData)
	{
		return;
	}
	if (GxCore_FileExists("/home/gx/ad_bar_pic.inf") == 1)
	{
		printf("GxAD_LoadData banner.inf  exist\n");
		if (GxCore_FileExists("/mnt/ad_bar_pic.inf") == 1)
			GxCore_FileDelete("/mnt/ad_bar_pic.inf");
		fd = GxCore_Open("/home/gx/ad_bar_pic.inf", "r");
		GxCore_Seek(fd, 0, GX_SEEK_SET);
		handlejpg = GxCore_Open("/mnt/ad_bar_pic.inf", "a+");
		GxCore_Seek(handlejpg, 0, GX_SEEK_SET);
		size = GxCore_Read(fd, Buf, 1, 1024);
		while(size>0)
		{
			size = GxCore_Write(handlejpg, Buf, 1, size);	
			size = GxCore_Read(fd, Buf, 1, 1024);
		}
		GxCore_Seek(handlejpg, 0, GX_SEEK_SET);
		size = GxCore_Read(handlejpg, pAdShowData, 1, sizeof(ad_divitone_items_t));
		GxCore_Close(fd);	
		GxCore_Close(handlejpg);						
		switch(pAdShowData->ads_file_type)
		{
			case 2:
				strcpy(inf_name,"/home/gx/banner.bmp");
				strcpy(inf_name_mnt,"/mnt/banner.bmp");
				break;
			case 3:
				strcpy(inf_name,"/home/gx/banner.jpg");
				strcpy(inf_name_mnt,"/mnt/banner.jpg");
				break;
			case 4:
				strcpy(inf_name,"/home/gx/banner.gif");
				strcpy(inf_name_mnt,"/mnt/banner.gif");
				break;
			default:
				break;
		}
		if ((sizeof(inf_name)>0 )&&GxCore_FileExists(inf_name) == 1)
		{
			if (GxCore_FileExists(inf_name_mnt) == 1)
				GxCore_FileDelete(inf_name_mnt);
			fd = GxCore_Open(inf_name, "r");
			GxCore_Seek(fd, 0, GX_SEEK_SET);
			handlejpg = GxCore_Open(inf_name_mnt, "a+");
			GxCore_Seek(handlejpg, 0, GX_SEEK_SET);
			size = GxCore_Read(fd, Buf, 1, 1024);
			while(size>0)
			{
				size = GxCore_Write(handlejpg, Buf, 1, size);	
				size = GxCore_Read(fd, Buf, 1, 1024);
			}
			GxCore_Close(fd);	
			GxCore_Close(handlejpg);					
			g_adItems[pAdShowData->ads_Type-1].ads_is_show = pAdShowData->ads_is_show;
			g_adItems[pAdShowData->ads_Type-1].ads_cmd = pAdShowData->ads_cmd;
			g_adItems[pAdShowData->ads_Type-1].ads_show_pos = pAdShowData->ads_show_pos;
			g_adItems[pAdShowData->ads_Type-1].ads_is_finish = pAdShowData->ads_is_finish;
		}
	}

	if (GxCore_FileExists("/home/gx/ad_menu_pic.inf") == 1)
	{
		printf("GxAD_LoadData menu.inf  exist\n");
		if (GxCore_FileExists("/mnt/ad_menu_pic.inf") == 1)
			GxCore_FileDelete("/mnt/ad_menu_pic.inf");
		fd = GxCore_Open("/home/gx/ad_menu_pic.inf", "r");
		GxCore_Seek(fd, 0, GX_SEEK_SET);
		handlejpg = GxCore_Open("/mnt/ad_menu_pic.inf", "a+");
		GxCore_Seek(handlejpg, 0, GX_SEEK_SET);
		size = GxCore_Read(fd, Buf, 1, 1024);
		while(size>0)
		{
			size = GxCore_Write(handlejpg, Buf, 1, size);	
			size = GxCore_Read(fd, Buf, 1, 1024);
		}
		GxCore_Seek(handlejpg, 0, GX_SEEK_SET);
		size = GxCore_Read(handlejpg, pAdShowData, 1, sizeof(ad_divitone_items_t));
		GxCore_Close(fd);	
		GxCore_Close(handlejpg);						
		switch(pAdShowData->ads_file_type)
		{
			case 2:
				strcpy(inf_name,"/home/gx/menu.bmp");
				strcpy(inf_name_mnt,"/mnt/menu.bmp");
				break;
			case 3:
				strcpy(inf_name,"/home/gx/menu.jpg");
				strcpy(inf_name_mnt,"/mnt/menu.jpg");
				break;
			case 4:
				strcpy(inf_name,"/home/gx/menu.gif");
				strcpy(inf_name_mnt,"/mnt/menu.gif");
				break;
			default:
				break;
		}
		if ((sizeof(inf_name)>0 )&&GxCore_FileExists(inf_name) == 1)
		{
			if (GxCore_FileExists(inf_name_mnt) == 1)
				GxCore_FileDelete(inf_name_mnt);
			fd = GxCore_Open(inf_name, "r");
			GxCore_Seek(fd, 0, GX_SEEK_SET);
			handlejpg = GxCore_Open(inf_name_mnt, "a+");
			GxCore_Seek(handlejpg, 0, GX_SEEK_SET);
			size = GxCore_Read(fd, Buf, 1, 1024);
			while(size>0)
			{
				size = GxCore_Write(handlejpg, Buf, 1, size);	
				size = GxCore_Read(fd, Buf, 1, 1024);
			}
			GxCore_Close(fd);	
			GxCore_Close(handlejpg);					
			g_adItems[pAdShowData->ads_Type-1].ads_is_show = pAdShowData->ads_is_show;
			g_adItems[pAdShowData->ads_Type-1].ads_cmd = pAdShowData->ads_cmd;
			g_adItems[pAdShowData->ads_Type-1].ads_show_pos = pAdShowData->ads_show_pos;
			g_adItems[pAdShowData->ads_Type-1].ads_is_finish = pAdShowData->ads_is_finish;

		}
	}
	if (GxCore_FileExists("/home/gx/ad_list_pic.inf") == 1)
	{
		printf("GxAD_LoadData list.inf  exist\n");
		if (GxCore_FileExists("/mnt/ad_list_pic.inf") == 1)
			GxCore_FileDelete("/mnt/ad_list_pic.inf");
		fd = GxCore_Open("/home/gx/ad_list_pic.inf", "r");
		GxCore_Seek(fd, 0, GX_SEEK_SET);
		handlejpg = GxCore_Open("/mnt/ad_list_pic.inf", "a+");
		GxCore_Seek(handlejpg, 0, GX_SEEK_SET);
		size = GxCore_Read(fd, Buf, 1, 1024);
		while(size>0)
		{
			size = GxCore_Write(handlejpg, Buf, 1, size);	
			size = GxCore_Read(fd, Buf, 1, 1024);
		}
		GxCore_Seek(handlejpg, 0, GX_SEEK_SET);
		size = GxCore_Read(handlejpg, pAdShowData, 1, sizeof(ad_divitone_items_t));
		GxCore_Close(fd);	
		GxCore_Close(handlejpg);						
		switch(pAdShowData->ads_file_type)
		{
			case 2:
				strcpy(inf_name,"/home/gx/list.bmp");
				strcpy(inf_name_mnt,"/mnt/list.bmp");
				break;
			case 3:
				strcpy(inf_name,"/home/gx/list.jpg");
				strcpy(inf_name_mnt,"/mnt/list.jpg");
				break;
			case 4:
				strcpy(inf_name,"/home/gx/list.gif");
				strcpy(inf_name_mnt,"/mnt/list.gif");
				break;
			default:
				break;
		}
		if ((sizeof(inf_name)>0 )&&GxCore_FileExists(inf_name) == 1)
		{
			if (GxCore_FileExists(inf_name_mnt) == 1)
				GxCore_FileDelete(inf_name_mnt);
			fd = GxCore_Open(inf_name, "r");
			GxCore_Seek(fd, 0, GX_SEEK_SET);
			handlejpg = GxCore_Open(inf_name_mnt, "a+");
			GxCore_Seek(handlejpg, 0, GX_SEEK_SET);
			size = GxCore_Read(fd, Buf, 1, 1024);
			while(size>0)
			{
				size = GxCore_Write(handlejpg, Buf, 1, size);	
				size = GxCore_Read(fd, Buf, 1, 1024);
			}
			GxCore_Close(fd);	
			GxCore_Close(handlejpg);					
			g_adItems[pAdShowData->ads_Type-1].ads_is_show = pAdShowData->ads_is_show;
			g_adItems[pAdShowData->ads_Type-1].ads_cmd = pAdShowData->ads_cmd;
			g_adItems[pAdShowData->ads_Type-1].ads_show_pos = pAdShowData->ads_show_pos;
			g_adItems[pAdShowData->ads_Type-1].ads_is_finish = pAdShowData->ads_is_finish;

		}
	}
	if (GxCore_FileExists("/home/gx/ad_volume_pic.inf") == 1)
	{
		printf("GxAD_LoadData volume.inf  exist\n");
		if (GxCore_FileExists("/mnt/ad_volume_pic.inf") == 1)
			GxCore_FileDelete("/mnt/ad_volume_pic.inf");
		fd = GxCore_Open("/home/gx/ad_volume_pic.inf", "r");
		GxCore_Seek(fd, 0, GX_SEEK_SET);
		handlejpg = GxCore_Open("/mnt/ad_volume_pic.inf", "a+");
		GxCore_Seek(handlejpg, 0, GX_SEEK_SET);
		size = GxCore_Read(fd, Buf, 1, 1024);
		while(size>0)
		{
			size = GxCore_Write(handlejpg, Buf, 1, size);	
			size = GxCore_Read(fd, Buf, 1, 1024);
		}
		GxCore_Seek(handlejpg, 0, GX_SEEK_SET);
		size = GxCore_Read(handlejpg, pAdShowData, 1, sizeof(ad_divitone_items_t));
		GxCore_Close(fd);	
		GxCore_Close(handlejpg);						
		switch(pAdShowData->ads_file_type)
		{
			case 2:
				strcpy(inf_name,"/home/gx/volume.bmp");
				strcpy(inf_name_mnt,"/mnt/volume.bmp");
				break;
			case 3:
				strcpy(inf_name,"/home/gx/volume.jpg");
				strcpy(inf_name_mnt,"/mnt/volume.jpg");
				break;
			case 4:
				strcpy(inf_name,"/home/gx/volume.gif");
				strcpy(inf_name_mnt,"/mnt/volume.gif");
				break;
			default:
				break;
		}
		if ((sizeof(inf_name)>0 )&&GxCore_FileExists(inf_name) == 1)
		{
			if (GxCore_FileExists(inf_name_mnt) == 1)
				GxCore_FileDelete(inf_name_mnt);
			fd = GxCore_Open(inf_name, "r");
			GxCore_Seek(fd, 0, GX_SEEK_SET);
			handlejpg = GxCore_Open(inf_name_mnt, "a+");
			GxCore_Seek(handlejpg, 0, GX_SEEK_SET);
			size = GxCore_Read(fd, Buf, 1, 1024);
			while(size>0)
			{
				size = GxCore_Write(handlejpg, Buf, 1, size);	
				size = GxCore_Read(fd, Buf, 1, 1024);
			}
			GxCore_Close(fd);	
			GxCore_Close(handlejpg);					
			g_adItems[pAdShowData->ads_Type-1].ads_is_show = pAdShowData->ads_is_show;
			g_adItems[pAdShowData->ads_Type-1].ads_cmd = pAdShowData->ads_cmd;
			g_adItems[pAdShowData->ads_Type-1].ads_show_pos = pAdShowData->ads_show_pos;
			g_adItems[pAdShowData->ads_Type-1].ads_is_finish = pAdShowData->ads_is_finish;
		}
	}
	return 0;
}

void app_divitone_ads_init(void)
{
	handle_t    ADfiltertimeout;
	uint32_t symbol_rate = 0;
	uint32_t qam = 0;
	uint16_t center_fre = 0;
	memset(&adsdivitone_para[0],0,AD_DIVITONE_TYPE_MAX*sizeof(DIVITONE_AD_PIC_t));
	advertisement_play_init();
	memset(&g_adItems[0],0,MAX_ITEMS_COUNT*sizeof(ad_items_t));
	memset(OsdBuff,0,sizeof(OsdBuff));
	memset(gDiviAdsServiceId,0,sizeof(gDiviAdsServiceId));
	gDiviAdsServiceIdCount = 0;
	symbol_rate = app_flash_get_config_center_freq_symbol_rate();
	qam = app_flash_get_config_center_freq_qam();
	center_fre = app_flash_get_config_center_freq();

	GxAD_LoadData();
	advertisement_set_startplayflag(TRUE);
	ad_fail = 1;//add by zs 20110810 lock tuner fail

	//addbyzs for if ad version is same, don't hint user "download ad data" begin
	if (0 == advertisement_get_ad_version_flag())
	{
		advertisement_set_ad_version_flag(1);
	}
	//addbyzs for if ad version is same, don't hint user "download ad data" end

	return ;


}

static divitone_cas_rolling_message_st rollingmsg = {0};
private_parse_status ad_data_got(uint8_t* pSectionData, size_t Size)
{
	uint8_t      *sectionData;
	int      i=0;
	int32_t     secTotalLen = 0;
	uint8_t Filter_Type = 0;
	uint8_t Filter_data = 0;
	uint16_t Current_packet = 0;
	uint16_t Total_packet = 0;
	uint8_t AD_type = 0;
	uint8_t File_type = 0;
	uint32_t ad_cmd = 0;
	uint8_t  pic_show_flag  = 0;
	uint8_t pic_show_time = 0;
	uint8_t pic_show_pos = 0;
	int32_t      dataLength = 0;
	int32_t      dataTotalLength = 0;
	size_t   size;					
	int pic_index = 0;
	int fd;
	ad_divitone_items_t  fg_items = {0,};
	char inf_name[100]={0};
	uint32_t nOffset = 0;
	uint8_t ServiceIdBuff[320];

	Filter_Type = pSectionData[5];
	Filter_data = pSectionData[6];
	Current_packet = (pSectionData[7]<<8) + pSectionData[8] - 1;
	Total_packet = (pSectionData[9]<<8) + pSectionData[10];
	AD_type = pSectionData[11];
	File_type = pSectionData[12];
	ad_cmd = (pSectionData[13]<<16 )|(pSectionData[14]<<8) |pSectionData[15];
	secTotalLen = ((pSectionData[1] & 0x0F) << 8) + pSectionData[2] + 3;
	if (Size != secTotalLen)
	{
		printf("ad_data_got Size=%d secTotalLen=%d error\n",Size,secTotalLen);
		return PRIVATE_SECTION_OK;
	}
	pic_show_flag = pSectionData[secTotalLen-7];
	pic_show_time = pSectionData[secTotalLen-6];
	pic_show_pos = pSectionData[secTotalLen-5];
	//	printf("pic_show_flag = %d pic_show_time = %d pic_show_pos = %d\n",pic_show_flag,pic_show_time,pic_show_pos);
	secTotalLen = Size-3;
	pic_index = AD_type -1;
	g_adItems[pic_index].ads_is_show = pic_show_flag;
	if (secTotalLen>4093)
	{
		printf("ad_data_got  secTotalLen=%d err\n",secTotalLen);
		return PRIVATE_SECTION_OK;
	}

	if (( Total_packet > SECTION_MAX_NUM)||(Current_packet > Total_packet))
	{
		printf("ad_data_got  lastSectionNum=%d ,Current_packet=%d\n",Total_packet,Current_packet);
		//			Gx_DemuxFilterEnable(ad_filter);
		return PRIVATE_SECTION_OK;
	}
	if ((g_adItems[pic_index].ads_cmd==ad_cmd) &&((g_adItems[pic_index].ads_is_finish == 1)|| (TRUE == g_adItems[pic_index].sectionData[Current_packet].recievedFlag)))
	{
		return PRIVATE_SECTION_OK;		
	}
	else if ((ad_cmd >0 &&g_adItems[pic_index].ads_cmd < ad_cmd )||
			(((g_adItems[pic_index].ads_cmd == 0xFFFFFF)||(AD_type == AD_DIVITONE_TYPE_JIAO)||(AD_type == AD_DIVITONE_TYPE_PIC_OSD)||(AD_type==AD_DIVITONE_TYPE_TEXT_OSD))&&(g_adItems[pic_index].ads_cmd != ad_cmd)))	
	{
		g_adItems[pic_index].ads_is_finish = 0;
		g_adItems[pic_index].ads_cmd = ad_cmd;
		g_adItems[pic_index].ads_file_type = File_type;
		g_adItems[pic_index].ads_is_show = pic_show_flag;
		g_adItems[pic_index].ads_show_pos = pic_show_pos;
		g_adItems[pic_index].ads_show_time = pic_show_time;
		for (i = 0; i<g_adItems[pic_index].lastSectionNum;i++ )
		{
			if (NULL !=g_adItems[pic_index].sectionData[i].addata )
			{
				GxCore_Free(g_adItems[pic_index].sectionData[i].addata);
				g_adItems[pic_index].sectionData[i].addata = NULL;
			}
		}

		g_adItems[pic_index].lastSectionNum =0;
		g_adItems[pic_index].recieveNum =0;
		memset(&g_adItems[pic_index].sectionData[0],0,SECTION_MAX_NUM*sizeof(ad_section_t));
	}
	else if( g_adItems[pic_index].ads_cmd >ad_cmd )
	{
		if( g_adItems[pic_index].ads_is_finish == 0 )
		{
			g_adItems[pic_index].ads_is_finish = 0;
			g_adItems[pic_index].ads_cmd = ad_cmd;
			g_adItems[pic_index].ads_file_type = File_type;
			g_adItems[pic_index].ads_is_show = pic_show_flag;
			g_adItems[pic_index].ads_show_pos = pic_show_pos;
			g_adItems[pic_index].ads_show_time = pic_show_time;
			for (i = 0; i<g_adItems[pic_index].lastSectionNum;i++ )
			{
				if (NULL !=g_adItems[pic_index].sectionData[i].addata )
				{
					GxCore_Free(g_adItems[pic_index].sectionData[i].addata);
					g_adItems[pic_index].sectionData[i].addata = NULL;
				}
			}

			g_adItems[pic_index].lastSectionNum =0;
			g_adItems[pic_index].recieveNum =0;
			memset(&g_adItems[pic_index].sectionData[0],0,SECTION_MAX_NUM*sizeof(ad_section_t));

		}
		else
			return PRIVATE_SECTION_OK;		
	}
	if (pic_index >=9)
		return;
	g_adItems[pic_index].lastSectionNum = Total_packet;
	g_adItems[pic_index].ads_Type = AD_type;
//	printf("Filter_Type=%d Ad_type=0x%x File_type=0x%x AD_cmd=0x%x\n",Filter_Type,AD_type,File_type,ad_cmd);
//	printf("seNum=%d lastSecNum=%d\n",Current_packet,Total_packet);
	g_adItems[pic_index].sectionData[Current_packet].recievedFlag = TRUE;
	dataLength=	secTotalLen-20;
//	printf("ad_data_got dataLength=%d\n",dataLength);

	if (NULL != g_adItems[pic_index].sectionData[Current_packet].addata)
	{
		GxCore_Free(g_adItems[pic_index].sectionData[Current_packet].addata);
		g_adItems[pic_index].sectionData[Current_packet].addata = NULL;
	}
	g_adItems[pic_index].sectionData[Current_packet].addata= GxCore_Malloc(dataLength);
	if (NULL == g_adItems[pic_index].sectionData[Current_packet].addata)
	{
		printf("g_adItems[%d].sectionData[%d].data =NULL\n",pic_index,Current_packet);
		return PRIVATE_SECTION_OK;			
	}
	sectionData=&pSectionData[16];

	memcpy(g_adItems[pic_index].sectionData[Current_packet].addata,sectionData,dataLength);

	g_adItems[pic_index].sectionData[Current_packet].len = dataLength; 


	g_adItems[pic_index].recieveNum++;
//	printf("g_adItems[%d].recieveNum=%d\n",pic_index,g_adItems[pic_index].recieveNum);
	cyg_scheduler_lock();
	if (g_adItems[pic_index].recieveNum >= (g_adItems[pic_index].lastSectionNum))
	{	
		/*
		 *  当前类型广告所有SECTION接收完整
		 */

		for (i = 0; i<g_adItems[pic_index].lastSectionNum;i++ )
		{
			dataTotalLength+=g_adItems[pic_index].sectionData[i].len;
		}
		g_adItems[pic_index].ads_is_finish = 1;
		printf(" %d ads_type = 0x%x dataTotalLength=%d\n",pic_index,g_adItems[pic_index].ads_Type,dataTotalLength);
#if  1
		memcpy(&fg_items,&g_adItems[pic_index],sizeof(ad_divitone_items_t));
		if(AD_DIVITONE_TYPE_LOGO == g_adItems[pic_index].ads_Type)
		{
			if (dataTotalLength <= 200*1024)
			{	// 大小有效，保存至FLASH

				if (GxCore_FileExists("/home/gx/openpicture.bin") == 1)
					GxCore_FileDelete("/home/gx/openpicture.bin");

				fd = GxCore_Open("/home/gx/openpicture.bin", "a+");							
				GxCore_Seek(fd, 0, GX_SEEK_SET);

				for (i = 0; i<g_adItems[pic_index].lastSectionNum;i++ )
				{	
					size = GxCore_Write(fd, g_adItems[pic_index].sectionData[i].addata, 1, g_adItems[pic_index].sectionData[i].len);	
				}

				GxCore_Close(fd);

				printf(" save logo.bin\n");
				for (i = 0; i<g_adItems[pic_index].lastSectionNum;i++ )
				{
					if (NULL !=g_adItems[pic_index].sectionData[i].addata )
					{
						GxCore_Free(g_adItems[pic_index].sectionData[i].addata);
						g_adItems[pic_index].sectionData[i].addata = NULL;
					}
				}

				g_adItems[pic_index].lastSectionNum =0;
				g_adItems[pic_index].recieveNum =0;
				memset(&g_adItems[pic_index].sectionData[0],0,SECTION_MAX_NUM*sizeof(ad_section_t));							

			}				
		}
		if(AD_DIVITONE_TYPE_MENU == g_adItems[pic_index].ads_Type)
		{
			if (dataTotalLength <= 120*1024)
			{	// 大小有效，保存至FLASH
				if ( 0 != dataTotalLength)
				{
					app_flash_save_ad_data_to_flash_file((const char*)&fg_items,sizeof(ad_divitone_items_t),"/home/gx/ad_menu_pic.inf");
					switch(fg_items.ads_file_type)
					{
						case 2:
							strcpy(inf_name,"/home/gx/menu.bmp");
							break;
						case 3:
							strcpy(inf_name,"/home/gx/menu.jpg");
							break;
						case 4:
							strcpy(inf_name,"/home/gx/menu.gif");
							break;
						default:
							break;
					}
					GxCore_FileDelete("home/gx/menu.gif");
					GxCore_FileDelete("home/gx/menu.bmp");
					GxCore_FileDelete("home/gx/menu.jpg");
					printf(" %s\n",inf_name);
					fd = GxCore_Open(inf_name, "a+");							
					GxCore_Seek(fd, 0, GX_SEEK_SET);

					for (i = 0; i<g_adItems[pic_index].lastSectionNum;i++ )
					{	
						size = GxCore_Write(fd, g_adItems[pic_index].sectionData[i].addata, 1, g_adItems[pic_index].sectionData[i].len);	
					}

					GxCore_Close(fd);								
				}
			}

		}
#endif
		if(AD_DIVITONE_TYPE_BAR == g_adItems[pic_index].ads_Type)
		{
			if (dataTotalLength <= 120*1024)

			{	// 大小有效，保存至FLASH
				/*首先删除以原有文件*/
				if ( 0 != dataTotalLength)
				{
					app_flash_save_ad_data_to_flash_file((const char*)&fg_items,sizeof(ad_divitone_items_t),"/home/gx/ad_bar_pic.inf");
					switch(fg_items.ads_file_type)
					{
						case 2:
							strcpy(inf_name,"/home/gx/banner.bmp");
							break;
						case 3:
							strcpy(inf_name,"/home/gx/banner.jpg");
							break;
						case 4:
							strcpy(inf_name,"/home/gx/banner.gif");
							break;
						default:
							break;
					}
					GxCore_FileDelete("/home/gx/banner.jpg");
					GxCore_FileDelete("/home/gx/banner.gif");
					GxCore_FileDelete("/home/gx/banner.bmp");
					printf(" %s\n",inf_name);
					fd = GxCore_Open(inf_name, "a+");							
					GxCore_Seek(fd, 0, GX_SEEK_SET);

					for (i = 0; i<g_adItems[pic_index].lastSectionNum;i++ )
					{	
						size = GxCore_Write(fd, g_adItems[pic_index].sectionData[i].addata, 1, g_adItems[pic_index].sectionData[i].len);	
					}

					GxCore_Close(fd);								
				}
				printf(" save %s\n",inf_name);
			}			
		}

		if(AD_DIVITONE_TYPE_VOLUME == g_adItems[pic_index].ads_Type)
		{
			if (dataTotalLength <= 120*1024)
			{	// 大小有效，保存至FLASH
				/*首先删除以原有文件*/
				if ( 0 != dataTotalLength)
				{
					app_flash_save_ad_data_to_flash_file((const char*)&fg_items,sizeof(ad_divitone_items_t),"/home/gx/ad_volume_pic.inf");
					switch(fg_items.ads_file_type)
					{
						case 2:
							strcpy(inf_name,"/home/gx/volume.bmp");
							break;
						case 3:
							strcpy(inf_name,"/home/gx/volume.jpg");
							break;
						case 4:
							strcpy(inf_name,"/home/gx/volume.gif");
							break;
						default:
							break;
					}
					GxCore_FileDelete("/home/gx/volume.jpg");
					GxCore_FileDelete("/home/gx/volume.gif");
					GxCore_FileDelete("/home/gx/volume.bmp");
					printf(" %s\n",inf_name);
					fd = GxCore_Open(inf_name, "a+");							
					GxCore_Seek(fd, 0, GX_SEEK_SET);
					for (i = 0; i<g_adItems[pic_index].lastSectionNum;i++ )
					{	
						size = GxCore_Write(fd, g_adItems[pic_index].sectionData[i].addata, 1, g_adItems[pic_index].sectionData[i].len);	
					}
					GxCore_Close(fd);								
				}
			}			
		}
		if(AD_DIVITONE_TYPE_LIST == g_adItems[pic_index].ads_Type)
		{
			//				if (dataTotalLength <= 50*1024)

			if (dataTotalLength <= 120*1024)

			{	// 大小有效，保存至FLASH
				/*首先删除以原有文件*/
				if ( 0 != dataTotalLength)
				{
					app_flash_save_ad_data_to_flash_file((const char*)&fg_items,sizeof(ad_divitone_items_t),"/home/gx/ad_list_pic.inf");
					switch(fg_items.ads_file_type)
					{
						case 2:
							strcpy(inf_name,"/home/gx/list.bmp");
							break;
						case 3:
							strcpy(inf_name,"/home/gx/list.jpg");
							break;
						case 4:
							strcpy(inf_name,"/home/gx/list.gif");
							break;
						default:
							break;
					}
					GxCore_FileDelete("/home/gx/list.jpg");
					GxCore_FileDelete("/home/gx/list.gif");
					GxCore_FileDelete("/home/gx/list.bmp");
					printf(" %s\n",inf_name);
					fd = GxCore_Open(inf_name, "a+");							
					GxCore_Seek(fd, 0, GX_SEEK_SET);
					for (i = 0; i<g_adItems[pic_index].lastSectionNum;i++ )
					{	
						size = GxCore_Write(fd, g_adItems[pic_index].sectionData[i].addata, 1, g_adItems[pic_index].sectionData[i].len);	
					}
					GxCore_Close(fd);								
				}
			}			
		}

		if(	AD_DIVITONE_TYPE_JIAO == g_adItems[pic_index].ads_Type)
		{
			//				if (dataTotalLength <= 50*1024)

			if (dataTotalLength <= 120*1024)

			{	// 大小有效，保存至FLASH
				/*首先删除以原有文件*/
				if ( 0 != dataTotalLength)
				{
					app_save_data_to_ddram_file((const char*)&fg_items,sizeof(ad_divitone_items_t),"/mnt/ad_jiao_pic.inf");
					switch(fg_items.ads_file_type)
					{
						case 2:
							strcpy(inf_name,"/mnt/jiao.bmp");
							break;
						case 3:
							strcpy(inf_name,"/mnt/jiao.jpg");
							break;
						case 4:
							strcpy(inf_name,"/mnt/jiao.gif");
							break;
						default:
							break;
					}

					if (GxCore_FileExists(inf_name) == 1)
						GxCore_FileDelete(inf_name);
					printf(" %s\n",inf_name);
					fd = GxCore_Open(inf_name, "a+");							
					GxCore_Seek(fd, 0, GX_SEEK_SET);
					for (i = 0; i<g_adItems[pic_index].lastSectionNum;i++ )
					{	
						size = GxCore_Write(fd, g_adItems[pic_index].sectionData[i].addata, 1, g_adItems[pic_index].sectionData[i].len);	
					}
					GxCore_Close(fd);								
					adsdivitone_para[pic_index+1].status  = 0 ;
					//	app_cas_api_on_event(DVB_AD_CORNER_PIC,CAS_NAME, NULL, 0);
				}

			}			
		}
		if(AD_DIVITONE_TYPE_SERVICE_OSD == g_adItems[pic_index].ads_Type)
		{
			if (dataTotalLength <= 320)
			{	// 大小有效，保存至FLASH
				/*首先删除以原有文件*/
				nOffset = 0;
				for (i = 0; i<g_adItems[pic_index].lastSectionNum;i++ )
				{	
					memcpy(&ServiceIdBuff[nOffset],g_adItems[pic_index].sectionData[i].addata, g_adItems[pic_index].sectionData[i].len);
					nOffset += g_adItems[pic_index].sectionData[i].len;
				}
				gDiviAdsServiceIdCount =  dataTotalLength/4;
				printf("service osd count = %d \n",gDiviAdsServiceIdCount);
				for( i=0; i<gDiviAdsServiceIdCount ; i++ )
				{
					gDiviAdsServiceId[i] = (ServiceIdBuff[i*4]-0x30)*1000 +(ServiceIdBuff[i*4+1]-0x30)*100 +
						(ServiceIdBuff[i*4+2]-0x30)*10 + (ServiceIdBuff[i*4+3]-0x30);
					printf("gDiviAdsServiceId[%d] = %d",i,gDiviAdsServiceId[i]);
				}
				printf("save service_osd \n");
			}			
		}
		if(	AD_DIVITONE_TYPE_PIC_OSD == g_adItems[pic_index].ads_Type)
		{
			if (dataTotalLength <= 120*1024)
			{	// 大小有效，保存至FLASH
				/*首先删除以原有文件*/
				if ( 0 != dataTotalLength)
				{
					app_flash_save_ad_data_to_flash_file((const char*)&fg_items,sizeof(ad_divitone_items_t),"/mnt/ad_pic_osd.inf");
					switch(fg_items.ads_file_type)
					{
						case 2:
							strcpy(inf_name,"/mnt/pic_osd.bmp");
							break;
						case 3:
							strcpy(inf_name,"/mnt/pic_osd.jpg");
							break;
						case 4:
							strcpy(inf_name,"/mnt/pic_osd.gif");
							break;
						default:
							break;
					}
					GxCore_FileDelete(inf_name);
					printf(" %s\n",inf_name);
					fd = GxCore_Open(inf_name, "a+");							
					GxCore_Seek(fd, 0, GX_SEEK_SET);
					for (i = 0; i<g_adItems[pic_index].lastSectionNum;i++ )
					{	
						size = GxCore_Write(fd, g_adItems[pic_index].sectionData[i].addata, 1, g_adItems[pic_index].sectionData[i].len);	
					}
					GxCore_Close(fd);								
					adsdivitone_para[pic_index+1].status  = 0 ;
				}
			}			
		}
		if(AD_DIVITONE_TYPE_TEXT_OSD == g_adItems[pic_index].ads_Type)
		{
			if (dataTotalLength <= 1024)
			{	// 大小有效，保存至FLASH
				/*首先删除以原有文件*/
				nOffset = 0;
				for (i = 0; i<g_adItems[pic_index].lastSectionNum;i++ )
				{	
					memcpy(&OsdBuff[nOffset],g_adItems[pic_index].sectionData[i].addata, g_adItems[pic_index].sectionData[i].len);
					nOffset += g_adItems[pic_index].sectionData[i].len;
				}
				rollingmsg.from = 1;
				rollingmsg.wStatus = 1;
				rollingmsg.param1 = &OsdBuff[0];
				rollingmsg.param2 = dataTotalLength;
				app_cas_api_on_event(DVB_CA_ROLLING_INFO,CAS_NAME, &rollingmsg, sizeof(divitone_cas_rolling_message_st));
				printf("save text_osd \n");
			}			
		}
		for (i = 0; i<g_adItems[pic_index].lastSectionNum;i++ )
		{
			if (NULL !=g_adItems[pic_index].sectionData[i].addata )
			{
				GxCore_Free(g_adItems[pic_index].sectionData[i].addata);
				g_adItems[pic_index].sectionData[i].addata = NULL;
			}
		}
		g_adItems[pic_index].lastSectionNum =0;
		g_adItems[pic_index].recieveNum =0;	
		memset(&g_adItems[pic_index].sectionData[0],0,SECTION_MAX_NUM*sizeof(ad_section_t));	
		/*所有广告接收完，设置开机播放标志*/
		/*所有广告接收完，设置广告版本*/
		GxAD_LoadData();
	}
	else
	{
	}
	cyg_scheduler_unlock();
	return PRIVATE_SUBTABLE_OK;

}


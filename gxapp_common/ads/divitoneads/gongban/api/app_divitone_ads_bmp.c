/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_xinshimao_ads_bmp.c
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
//#include "ads_api_xsm_2.0.h"
#include "gxapp_sys_config.h"
#include "gui_timer.h"
#include "gxmsg.h"
#include "app_divitone_ads_porting_stb_api.h"
#include "divitone_ads_porting.h"
#include "app_common_prog.h"
#include "IMG.h"

extern DIVITONE_AD_PIC_t  adsdivitone_para[AD_DIVITONE_TYPE_MAX];
ad_play_para adsDiviPicOsd = {0,};

static GxAvRect rectgif = {0};
#define GIF_DURATION (200)
u_int8_t app_divitone_ads_play_state(uint8_t type);
u_int8_t app_divitone_ads_get_play_type(void)
{
	u_int8_t i;
	for (i = 0; i<AD_DIVITONE_TYPE_MAX; i++ )
		{
			if (TRUE == app_divitone_ads_play_state(i))
				return i;
		}
	return AD_DIVITONE_TYPE_MAX;
}
#if 1
static int timer_scroll_bmp(void *userdata)
{
	int rtn = 0;
	image_desc *image = NULL;
	int width = 0;
	int height = 0;

	if ((2 == adsDiviPicOsd.type))
		{
			handle_t flash_ad_file;
			size_t	 size;

			ad_divitone_items_t *pAdShowData = NULL;
			pAdShowData = GxCore_Malloc(sizeof(ad_divitone_items_t));
			if (NULL == pAdShowData)
				{
					return 0;
				}
			flash_ad_file = GxCore_Open("/mnt/ad_pic_osd.inf", "r+");
			GxCore_Seek(flash_ad_file, 0, GX_SEEK_SET);
			size = GxCore_Read(flash_ad_file, pAdShowData, 1, sizeof(ad_divitone_items_t));
			GxCore_Close(flash_ad_file);
			image = gal_img_load(NULL, adsDiviPicOsd.file_ddram_path);
			width = image->width;
			height = image->height;	
			gal_img_release(image);
			adsDiviPicOsd.file_flash_path = NULL;
			adsDiviPicOsd.play_mode = 2;
			adsDiviPicOsd.type = 2;
			adsDiviPicOsd.times_count= pAdShowData->ads_show_time;
			rtn = advertisement_scrolling_by_step(&adsDiviPicOsd,adsDiviPicOsd.rectdest.y ,width,height);
			if (0 == rtn)
				{
					app_divitone_ads_stop(NULL, AD_DIVITONE_TYPE_PIC_OSD);
				}
			GxCore_Free(pAdShowData);
			pAdShowData = NULL; 
			return 0;
		}

	return 0;
}

static int timer_gif_play(void *userdata)
{
	if (adsDiviPicOsd.type == 1)
		{
			adsDiviPicOsd.slice_num++;
			printf("gifplaypara.slice_num=%d gifplaypara.slice_count=%d\n",
				adsDiviPicOsd.slice_num,adsDiviPicOsd.slice_count);
			if (adsDiviPicOsd.slice_num <= adsDiviPicOsd.slice_count)
			{
				adsDiviPicOsd.rectdest.width= 0;
				adsDiviPicOsd.rectdest.height= 0;
				app_play_ad_ddram_file(&adsDiviPicOsd);
				rectgif.x = adsDiviPicOsd.rectdest.x;
				rectgif.y = adsDiviPicOsd.rectdest.y;
				if (rectgif.width < adsDiviPicOsd.rectdest.width)
					rectgif.width = adsDiviPicOsd.rectdest.width;
				if (rectgif.height < adsDiviPicOsd.rectdest.height)
					rectgif.height = adsDiviPicOsd.rectdest.height;
			}
		else
			{
				app_divitone_ads_stop(NULL,AD_DIVITONE_TYPE_PIC_OSD);
			}			
		}
	
	return 0;
}


#endif
void app_divitone_ads_stop(char* IconName,uint8_t type)
{
	GxAvRect rect;	

	if(type >= AD_DIVITONE_TYPE_MAX)
	{
		printf("%s type=%d\n",__FUNCTION__,type);	
		return ;
	}
	if (NULL != adsdivitone_para[type].ad_timer)
	{
		remove_timer(adsdivitone_para[type].ad_timer);
		adsdivitone_para[type].ad_timer = NULL;
	}
	if (IconName != NULL)
	{
		GUI_SetProperty(IconName,(char *)"load_img",(void *)NULL);
		GUI_SetProperty(IconName, (char *)"state", "hide");
		GUI_SetInterface("flush", NULL);
	}
	memset(&adsdivitone_para[type],0,sizeof(DIVITONE_AD_PIC_t));
#if  1

	if( type == AD_DIVITONE_TYPE_PIC_OSD )
	{
		if (adsDiviPicOsd.ad_timer != NULL)
		{
			remove_timer(adsDiviPicOsd.ad_timer);
			adsDiviPicOsd.ad_timer =NULL;
		}

		if (NULL != adsDiviPicOsd.file_ddram_path)
		{
			if (1 == adsDiviPicOsd.type)
			{
				memcpy(&adsDiviPicOsd.rectdest,&rectgif,sizeof(GxAvRect));
				memset(&rectgif,0,sizeof(GxAvRect));					
			}	
			rect.x = 0;
			rect.y = 0;
			rect.width = VIDEO_WINDOW_W;
			rect.height = VIDEO_WINDOW_H;
			advertisement_clear_frame(rect);			
			//			advertisement_clear_frame(ads3h_para[type].rectdest);
			memset(&adsDiviPicOsd,0,sizeof(ad_play_para));
			//			advertisement_hide();
		}
	}
#endif
	return ;
}

static void app_divitone_ad_show_gif_callback(void * data)
{
	status_t sret = 0;
	int alu = GX_ALU_ROP_COPY;//GX_ALU_ROP_COPY_INVERT;
	DIVITONE_AD_PIC_t * ptr = (DIVITONE_AD_PIC_t *)data;
	char *focus_window = NULL;

	if(!ptr)
	{
		return;
	}

	/* avoid temporay dialog box when show main menu ad */
	if(strcasecmp("win_main_menu_image_ad", ptr->IconName) == 0)
	{
		focus_window = (char*)GUI_GetFocusWindow();

		if(strcasecmp("win_main_menu", focus_window) != 0)
			return;
	}
	if(ptr->isShown == 0)
	{
		sret = GUI_SetProperty(ptr->IconName, "state", "show");
		ADS_Dbg("l=%d,sret = %d\n",__LINE__,sret);/**/
		ptr->isShown = 1;
		return;
	}
	if(ptr->isShown == 1 && ptr->ad_timer)
	{
		ptr->ad_timer->delta_time = 800;
	}
	sret = GUI_SetProperty(ptr->IconName, "draw_gif", (void *)&alu);
	GUI_SetInterface("flush", NULL);
	if(sret)
	{
		ADS_Dbg("l=%d,sret = %d\n",__LINE__,sret);
	}
}
static void app_divitone_ad_set_widget_xywh(char * widgetName,int x,int y,int w,int h)
{
	status_t sret = 0;
	sret = GUI_SetProperty(widgetName,(char *)"x",(void *)&x);
	sret = GUI_SetProperty(widgetName,(char *)"y",(void *)&y);
	sret = GUI_SetProperty(widgetName,(char *)"width",(void *)&w);
	sret = GUI_SetProperty(widgetName,(char *)"height",(void *)&h);
}
void app_divitone_ads_play(char * IconName, uint8_t type)
{
	handle_t flash_ad_file;
	size_t	 size;
	char inf_name[50]={0};
	int alu = GX_ALU_ROP_COPY;
	status_t sret = 0;

	if(type >= AD_DIVITONE_TYPE_MAX)
	{
		printf("%s type=%d\n",__FUNCTION__,type);	
		return ;
	}
	if (FALSE == advertisement_get_startplayflag())
	{
		return ;
	}
	ad_divitone_items_t *pAdShowData = NULL;
	pAdShowData = GxCore_Malloc(sizeof(ad_divitone_items_t));
	if (NULL == pAdShowData)
	{
		return;
	}

	switch(type)
	{
		case AD_DIVITONE_TYPE_NONE:
			break;
		case AD_DIVITONE_TYPE_BAR:
			if (GxCore_FileExists("/mnt/ad_bar_pic.inf") == 1)
			{
				flash_ad_file = GxCore_Open("/mnt/ad_bar_pic.inf", "r+");
				GxCore_Seek(flash_ad_file, 0, GX_SEEK_SET);
				size = GxCore_Read(flash_ad_file, pAdShowData, 1, sizeof(ad_divitone_items_t));
				GxCore_Close(flash_ad_file);
				if( pAdShowData->ads_is_show == 1 )
				{
					switch(pAdShowData->ads_file_type)
					{
						case 2:
							adsdivitone_para[type].file_ddram_path = "/mnt/banner.bmp";
							break;
						case 3:
							adsdivitone_para[type].file_ddram_path = "/mnt/banner.jpg";
							break;
						case 4:
							adsdivitone_para[type].file_ddram_path = "/mnt/banner.gif";
							break;
						default:
							break;
					}
					strcpy(adsdivitone_para[type].IconName ,IconName);
					if (GxCore_FileExists(adsdivitone_para[type].file_ddram_path) == 1)
					{
						sret = GUI_SetProperty((char *)IconName,(char *)"load_zoom_img"/*"load_img"*/,adsdivitone_para[type].file_ddram_path);
						ADS_Dbg("l=%d,sret = %d\n",__LINE__,sret);
						sret = GUI_SetProperty(IconName, "init_gif_alu_mode", (void *)&alu);
						ADS_Dbg("l=%d,sret = %d\n",__LINE__,sret);
						sret = GUI_SetProperty(IconName, "draw_gif", &alu);
						ADS_Dbg("l=%d,sret = %d\n",__LINE__,sret);
						sret = GUI_SetProperty(IconName, "state", "show");
						if( pAdShowData->ads_file_type == 4 )
						{
							if(adsdivitone_para[type].ad_timer)
							{
								remove_timer(adsdivitone_para[type].ad_timer);
							}
							adsdivitone_para[type].ad_timer = create_timer((timer_event)app_divitone_ad_show_gif_callback,10,&adsdivitone_para[type],TIMER_REPEAT);
						}
						break;
					}
				}
				break;
			}
			break;
		case AD_DIVITONE_TYPE_MENU:
			if (GxCore_FileExists("/mnt/ad_menu_pic.inf") == 1)
			{
				flash_ad_file = GxCore_Open("/mnt/ad_menu_pic.inf", "r+");
				GxCore_Seek(flash_ad_file, 0, GX_SEEK_SET);
				size = GxCore_Read(flash_ad_file, pAdShowData, 1, sizeof(ad_divitone_items_t));
				GxCore_Close(flash_ad_file);
				if( pAdShowData->ads_is_show == 1 )
				{
					switch(pAdShowData->ads_file_type)
					{
						case 2:
							adsdivitone_para[type].file_ddram_path = "/mnt/menu.bmp";
							break;
						case 3:
							adsdivitone_para[type].file_ddram_path = "/mnt/menu.jpg";
							break;
						case 4:
							adsdivitone_para[type].file_ddram_path = "/mnt/menu.gif";
							//			adsdivitone_para[type].type = 1;
							break;
						default:
							break;
					}
					strcpy(adsdivitone_para[type].IconName ,IconName);
					if (GxCore_FileExists(adsdivitone_para[type].file_ddram_path) == 1)
					{
						sret = GUI_SetProperty((char *)IconName,(char *)"load_zoom_img"/*"load_img"*/,adsdivitone_para[type].file_ddram_path);
						sret = GUI_SetProperty(IconName, "init_gif_alu_mode", (void *)&alu);
						sret = GUI_SetProperty(IconName, "draw_gif", &alu);
						sret = GUI_SetProperty(IconName, "state", "show");
						if( pAdShowData->ads_file_type == 4 )
						{
							if(adsdivitone_para[type].ad_timer)
							{
								remove_timer(adsdivitone_para[type].ad_timer);
							}
							adsdivitone_para[type].ad_timer = create_timer((timer_event)app_divitone_ad_show_gif_callback,10,&adsdivitone_para[type],TIMER_REPEAT);
						}
						break;
					}
				}
				else
				{
				}	
			}
			break;
		case AD_DIVITONE_TYPE_LIST:
			if (GxCore_FileExists("/mnt/ad_list_pic.inf") == 1)
			{
				flash_ad_file = GxCore_Open("/mnt/ad_list_pic.inf", "r+");
				GxCore_Seek(flash_ad_file, 0, GX_SEEK_SET);
				size = GxCore_Read(flash_ad_file, pAdShowData, 1, sizeof(ad_divitone_items_t));
				GxCore_Close(flash_ad_file);
				if( pAdShowData->ads_is_show == 1 )
				{
					switch(pAdShowData->ads_file_type)
					{
						case 2:
							adsdivitone_para[type].file_ddram_path = "/mnt/list.bmp";
							break;
						case 3:
							adsdivitone_para[type].file_ddram_path = "/mnt/list.jpg";
							break;
						case 4:
							adsdivitone_para[type].file_ddram_path = "/mnt/list.gif";
							break;
						default:
							break;
					}
					strcpy(adsdivitone_para[type].IconName ,IconName);
					if (GxCore_FileExists(adsdivitone_para[type].file_ddram_path) == 1)
					{
						sret = GUI_SetProperty((char *)IconName,(char *)"load_zoom_img"/*"load_img"*/,adsdivitone_para[type].file_ddram_path);
						//						ADS_Dbg("l=%d,sret = %d\n",__LINE__,sret);
						sret = GUI_SetProperty(IconName, "init_gif_alu_mode", (void *)&alu);
						//						ADS_Dbg("l=%d,sret = %d\n",__LINE__,sret);
						sret = GUI_SetProperty(IconName, "draw_gif", &alu);
						//						ADS_Dbg("l=%d,sret = %d\n",__LINE__,sret);
						sret = GUI_SetProperty(IconName, "state", "show");
						if( pAdShowData->ads_file_type == 4 )
						{
							if(adsdivitone_para[type].ad_timer)
							{
								remove_timer(adsdivitone_para[type].ad_timer);
							}
							adsdivitone_para[type].ad_timer = create_timer((timer_event)app_divitone_ad_show_gif_callback,10,&adsdivitone_para[type],TIMER_REPEAT);
						}
						break;
					}
				}
			}
			break;
		case AD_DIVITONE_TYPE_VOLUME:
			if (GxCore_FileExists("/mnt/ad_volume_pic.inf") == 1)
			{
				flash_ad_file = GxCore_Open("/mnt/ad_volume_pic.inf", "r+");
				GxCore_Seek(flash_ad_file, 0, GX_SEEK_SET);
				size = GxCore_Read(flash_ad_file, pAdShowData, 1, sizeof(ad_divitone_items_t));
				GxCore_Close(flash_ad_file);
				if( pAdShowData->ads_is_show == 1 )
				{
					switch(pAdShowData->ads_file_type)
					{
						case 2:
							adsdivitone_para[type].file_ddram_path = "/mnt/volume.bmp";
							break;
						case 3:
							adsdivitone_para[type].file_ddram_path = "/mnt/volume.jpg";
							break;
						case 4:
							adsdivitone_para[type].file_ddram_path = "/mnt/volume.gif";
							break;
						default:
							break;
					}
					strcpy(adsdivitone_para[type].IconName ,IconName);
					if (GxCore_FileExists(adsdivitone_para[type].file_ddram_path) == 1)
					{
						sret = GUI_SetProperty((char *)IconName,(char *)"load_zoom_img"/*"load_img"*/,adsdivitone_para[type].file_ddram_path);
						sret = GUI_SetProperty(IconName, "init_gif_alu_mode", (void *)&alu);
						sret = GUI_SetProperty(IconName, "draw_gif", &alu);
						sret = GUI_SetProperty(IconName, "state", "show");
						if( pAdShowData->ads_file_type == 4 )
						{
							if(adsdivitone_para[type].ad_timer)
							{
								remove_timer(adsdivitone_para[type].ad_timer);
							}
							adsdivitone_para[type].ad_timer = create_timer((timer_event)app_divitone_ad_show_gif_callback,10,&adsdivitone_para[type],TIMER_REPEAT);
						}
						break;
					}
				}
			}
			break;
		case AD_DIVITONE_TYPE_PIC_OSD:
			if ((GxCore_FileExists("/mnt/ad_pic_osd.inf") == 1 )&&(adsdivitone_para[type].status ==0))
			{
				flash_ad_file = GxCore_Open("/mnt/ad_pic_osd.inf", "r+");
				GxCore_Seek(flash_ad_file, 0, GX_SEEK_SET);
				size = GxCore_Read(flash_ad_file, pAdShowData, 1, sizeof(ad_divitone_items_t));
				GxCore_Close(flash_ad_file);
				if( pAdShowData->ads_is_show == 1 )
				{
					switch(pAdShowData->ads_file_type)
					{
						case 2:
							adsDiviPicOsd.file_ddram_path = "/mnt/pic_osd.bmp";	
							adsdivitone_para[type].file_ddram_path = "/mnt/pic_osd.bmp";
							break;
						case 3:
							adsdivitone_para[type].file_ddram_path = "/mnt/pic_osd.jpg";
							adsDiviPicOsd.file_ddram_path = "/mnt/pic_osd.jpg";	
							break;
						case 4:
							adsdivitone_para[type].file_ddram_path = "/mnt/pic_osd.gif";
							adsDiviPicOsd.file_ddram_path = "/mnt/pic_osd.gif";	
							break;
						default:
							break;
					}
					if (GxCore_FileExists(adsDiviPicOsd.file_ddram_path) == 1)
					{
						adsDiviPicOsd.file_flash_path = NULL;
						adsDiviPicOsd.play_mode = 2;
						adsDiviPicOsd.type = 2;
						adsDiviPicOsd.rectdest.x = 0;//843;
						if (pAdShowData->ads_show_pos == 0)
						{
							adsDiviPicOsd.rectdest.y = 520;
						}	
						else if (pAdShowData->ads_show_pos == 1)
						{

							adsDiviPicOsd.rectdest.y = 200;
						}
						else
							adsDiviPicOsd.rectdest.y = 30;
						adsdivitone_para[type].status = 1;
						printf("pic_osd show  \n");
						memset(&rectgif,0,sizeof(GxAvRect));
						if (2 == adsDiviPicOsd.type)
						{
							/*
							 * scroll
							 */
							adsdivitone_para[type].ad_timer = create_timer(timer_scroll_bmp, SCROLL_DURATION, NULL,	TIMER_REPEAT);												
						}
						if (1 == adsDiviPicOsd.type)
						{

							app_play_ad_ddram_file(&adsDiviPicOsd);							
							if (rectgif.width < adsDiviPicOsd.rectdest.width)
								rectgif.width = adsDiviPicOsd.rectdest.width;
							if (rectgif.height < adsDiviPicOsd.rectdest.height)
								rectgif.height = adsDiviPicOsd.rectdest.height;			

							adsdivitone_para[type].ad_timer = create_timer(timer_gif_play, GIF_DURATION, NULL,  TIMER_REPEAT);						
						}
						break;
					}
					else
					{
						app_divitone_ads_stop(NULL,AD_DIVITONE_TYPE_PIC_OSD);
					}	
				}
			}
			break;
		case AD_DIVITONE_TYPE_JIAO:
			if ((GxCore_FileExists("/mnt/ad_jiao_pic.inf") == 1)&&(adsdivitone_para[type].status == 0))
			{
				flash_ad_file = GxCore_Open("/mnt/ad_jiao_pic.inf", "r+");
				GxCore_Seek(flash_ad_file, 0, GX_SEEK_SET);
				size = GxCore_Read(flash_ad_file, pAdShowData, 1, sizeof(ad_divitone_items_t));
				GxCore_Close(flash_ad_file);
				if( pAdShowData->ads_is_show == 1 )
				{
					switch(pAdShowData->ads_file_type)
					{
						case 2:
							adsdivitone_para[type].file_ddram_path = "/mnt/jiao.bmp";
							break;
						case 3:
							adsdivitone_para[type].file_ddram_path = "/mnt/jiao.jpg";
							break;
						case 4:
							adsdivitone_para[type].file_ddram_path = "/mnt/jiao.gif";
							break;
						default:
							break;
					}
					strcpy(adsdivitone_para[type].IconName ,IconName);
					if (GxCore_FileExists(adsdivitone_para[type].file_ddram_path) == 1)
					{
						app_divitone_ad_set_widget_xywh(IconName,500,100,80,80);
						sret = GUI_SetProperty((char *)IconName,(char *)"load_zoom_img"/*"load_img"*/,adsdivitone_para[type].file_ddram_path);
						sret = GUI_SetProperty(IconName, "init_gif_alu_mode", (void *)&alu);
						sret = GUI_SetProperty(IconName, "draw_gif", &alu);
						sret = GUI_SetProperty(IconName, "state", "show");
						adsdivitone_para[type].status = 1;
						if( pAdShowData->ads_file_type == 4 )
						{
							if(adsdivitone_para[type].ad_timer)
							{
								remove_timer(adsdivitone_para[type].ad_timer);
							}

							adsdivitone_para[type].ad_timer = create_timer((timer_event)app_divitone_ad_show_gif_callback,10,&adsdivitone_para[type],TIMER_REPEAT);

						}
						break;
					}
				}
				else
				{
					app_divitone_ads_stop(IconName,AD_DIVITONE_TYPE_JIAO);
				}	
			}

			break;
		case AD_DIVITONE_TYPE_TEXT_OSD:
			break;
		default:
			break;;
	}
	GxCore_Free(pAdShowData);
	pAdShowData = NULL; 

}


u_int8_t app_divitone_ads_play_state(uint8_t type)
{
	if(type >= AD_DIVITONE_TYPE_MAX)
	{
		printf("%s type=%d\n",__FUNCTION__,type);	
		return FALSE;
	}

	if (NULL != adsdivitone_para[type].file_ddram_path)
	{
		return TRUE;
		printf("%s ddram file paht = %s\n",__FUNCTION__,adsdivitone_para[type].file_ddram_path);
	}

	return FALSE;
}





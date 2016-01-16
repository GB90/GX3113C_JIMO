#include "dvb_ads_drv.h"
#include "gui_core.h"
#include "ui_dvb_ads_porting.h"
#include "dvb_ads_function_porting.h"
#include "dvb_ads_type.h"
#include "dvb_ads_stb_struct.h"
#include "app_common_prog.h"
#include "app_common_play.h"

os_sem_t show_pic_sem;
static u8 unrelated_ads_cnt;
//the global var for logo data!!!
static dvbad_show_pic_info logo_pic_info;

static dvbad_show_pic_info epg_pic_info;
static dvbad_show_pic_info volume_pic_info;
static dvbad_show_pic_info main_menu_pic_info;
static dvbad_show_pic_info chlist_pic_info;

static dvbad_show_pic_info current_follow_pic_info;
static dvbad_show_pic_info channel_info_pic_info;

static BOOL program_unrelated_ads_got_flag = FALSE;
static BOOL program_unrelated_ads_saved_flag = FALSE;

extern dvbad_priv_t s_dvbad;
extern ad_save_info_t ad_save_info;
extern BOOL search_lock_main_tp_flag;
extern get_ad_flag g_ads_get_flag;
//extern BOOL dvb_ads_version_need_update_flag;


/*----------PIC OPT FUNC FOR DVB ADS START-------------*/

/*----------common func for dvb ads start---------*/
/*reset the widget x,y,w,h*/
#if 0
static void dvb_ads_set_widget_xywh(char* widgetName,int x,int y,int w,int h)
{
	status_t sret = 0;
	sret = GUI_SetProperty((char*)widgetName,(char *)"x",(void *)&x);
	sret = GUI_SetProperty((char*)widgetName,(char *)"y",(void *)&y);
	sret = GUI_SetProperty((char*)widgetName,(char *)"width",(void *)&w);
	sret = GUI_SetProperty((char*)widgetName,(char *)"height",(void *)&h);
}
#endif
					/*----------LOGO opt start-----------*/
/*
	>> save the logo data to file and the logo config to file 
*/
RET_CODE dvb_ads_save_logo_data(void)
{
	size_t size;
	handle_t file_handle;
	dvbad_priv_t* ad = &s_dvbad;
	ads_data_node* p_node = &(ad->p_ad_data->dvbad_pic_attribute_node);
	//read to the global the same time
	dvbad_show_pic_info *logo_pic =  dvb_ads_mempool_alloc(&s_dvbad.mem,sizeof(dvbad_show_pic_info));;
	while(DVBAD_STB_LOG != p_node->ads_pic_attribute_data.type)
	{
		if(NULL==p_node->next)
		{
			return ERR_FAILURE;
		}
		p_node = p_node->next;
	}
	if(NULL == p_node)
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"This type of ad is not exit in the TS!!!");
		return ERR_FAILURE;
	}

	logo_pic->x = p_node->ads_pic_attribute_data.x;
	logo_pic->y = p_node->ads_pic_attribute_data.y;
	logo_pic->w = p_node->ads_pic_attribute_data.w;
	logo_pic->h = p_node->ads_pic_attribute_data.h;
	logo_pic->pos_type = p_node->ads_pic_attribute_data.type;
	logo_pic->pic_type = p_node->ads_pic_attribute_data.p_format;
	logo_pic->pic_id = p_node->ads_pic_attribute_data.file_number;
	logo_pic->pic_data = p_node->p_ads_pic_data;
	DVB_ADS_DEBUG(DVBAD,INFO,"The strlen of p_node->p_ads_pic_data is %d\n",strlen((const char*)p_node->p_ads_pic_data));
	logo_pic->data_len = p_node->p_pic_info.pic_original_size;

	DVB_ADS_DEBUG(DVBAD,INFO,"LOGO data get info x:%d y:%d w:%d h:%d pos_type:%d pic_type:%d pic_id:%d data_len = %d\n",\
			logo_pic->x,logo_pic->y,logo_pic->w,logo_pic->h,logo_pic->pos_type,logo_pic->pic_type,logo_pic->pic_id ,(int)logo_pic->data_len);

	DVB_ADS_DEBUG(DVBAD,INFO,"Start to save LOGO cfg DATA to FLASH file!!!");
	
	/* >> The logo config data will be saved to file*/
	if(0 == logo_pic->data_len)
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"LOGO DATA ERROR!!!");
		return ERR_FAILURE;
	}
	
	if(1 == GxCore_FileExists(DVB_LOGO_CFG_PATH))
	{
		GxCore_FileDelete(DVB_LOGO_CFG_PATH);
	}
	file_handle = GxCore_Open(DVB_LOGO_CFG_PATH,"a+");
	GxCore_Seek(file_handle,0,GX_SEEK_SET);
	size = GxCore_Write(file_handle,(void*)logo_pic,1,sizeof(dvbad_show_pic_info));
	if(size < 0)
	{
		return ERR_FAILURE;
	}
	/*  >>TEST the data write correct or not	?*/
	GxCore_Close(file_handle);
	file_handle = GxCore_Open(DVB_LOGO_CFG_PATH,"r");
	GxCore_Seek(file_handle, 0, GX_SEEK_SET);
	memset((void *)logo_pic,0,sizeof(dvbad_show_pic_info));
	size = GxCore_Read(file_handle,logo_pic,1,sizeof(dvbad_show_pic_info));
	GxCore_Close(file_handle);
	if(size > 0)
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"LOGO config try read out:x:%d y:%d w:%d h:%d pos_type:%d pic_type:%d pic_id:%d data_len = %d\n",\
				logo_pic->x,logo_pic->y,logo_pic->w,logo_pic->h,logo_pic->pos_type,logo_pic->pic_type,logo_pic->pic_id ,(int)logo_pic->data_len);
	}
	
	DVB_ADS_DEBUG(DVBAD,INFO,"saved logo cfg data file_number = %u,format:%u\n",(unsigned int)logo_pic->pic_id,(unsigned int)logo_pic->pic_type);//6 mpeg
	if((size < 0) || (size > DVB_LOGO_MAX_LEN))
	{	
		return ERR_FAILURE;
	}
	if( 1 != GxCore_FileExists(DVB_LOGO_CFG_PATH))
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"SAVE LOGO config file failed!!!");
		return ERR_FAILURE;
	}
	DVB_ADS_DEBUG(DVBAD,INFO,"LOGO config data has been saved to file!!!");
	
	/* >> The real logo data will be saved in the file!!!*/
	if(1 == GxCore_FileExists(DVB_LOGO_PATH))
	{
		GxCore_FileDelete(DVB_LOGO_PATH);
	}
	
	file_handle = GxCore_Open(DVB_LOGO_PATH,"a+");
	GxCore_Seek(file_handle, 0, GX_SEEK_SET);
	size = GxCore_Write(file_handle,(void*)logo_pic->pic_data,1,logo_pic->data_len);
	GxCore_Close(file_handle);
	
	DVB_ADS_DEBUG(DVBAD,INFO,"The logo data strlen is %d,data_len = %d\n",strlen((const char*)logo_pic->pic_data),(int)logo_pic->data_len);
	
	GxFileInfo file_info = {0};
	GxCore_GetFileInfo(DVB_LOGO_PATH, &file_info);
	DVB_ADS_DEBUG(DVBAD,INFO,"LOGO saved file_info.size_by_bytes = %lu",(long unsigned int)file_info.size_by_bytes);

	if((size < 0) || (size > DVB_LOGO_MAX_LEN))
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"logo SIZE = %d\n",size);
		return ERR_FAILURE;
	}
	if(1 != GxCore_FileExists(DVB_LOGO_PATH))
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"SAVE LOGO  file failed!!!");
		return ERR_FAILURE;
	}
	//record the count of the ads type saved 
	DVB_ADS_DEBUG(DVBAD,INFO,"saved logo data size: %u to file : %s",(unsigned int)logo_pic->data_len,(char*)DVB_LOGO_PATH);
	return SUCCESS;
}

/*
	>> read out the logo config data  from flash to global var!!!
*/
static RET_CODE dvb_ads_get_logo_config_data(void)
{
	size_t size = 0;
	handle_t file_handle = 0;
	dvbad_show_pic_info* logo_pic = dvb_ads_mempool_alloc(&s_dvbad.mem,sizeof(dvbad_show_pic_info));
	
	if(1 == GxCore_FileExists(DVB_LOGO_CFG_PATH))
	{
		memset((void *)logo_pic,0,sizeof(dvbad_show_pic_info));
		file_handle = GxCore_Open(DVB_LOGO_CFG_PATH,"r");
		GxCore_Seek(file_handle, 0, GX_SEEK_SET);
		size = GxCore_Read(file_handle,logo_pic,1,sizeof(dvbad_show_pic_info));
		GxCore_Close(file_handle);
		DVB_ADS_DEBUG(DVBAD,INFO,"LOGO data Read info:x:%d y:%d w:%d h:%d pos_type:%d pic_type:%d pic_id:%d data_len = %u\n",\
				logo_pic->x,logo_pic->y,logo_pic->w,logo_pic->h,logo_pic->pos_type,logo_pic->pic_type,logo_pic->pic_id ,(unsigned int)logo_pic->data_len);
		if(size > 0)
		{	
			memcpy(&logo_pic_info,logo_pic,sizeof(dvbad_show_pic_info));
			return SUCCESS;
		}
	}
	DVB_ADS_DEBUG(DVBAD,INFO,"Get logo config data failed!!!");
	return ERR_FAILURE;
}

/* 
	>> show the logo data!!!	
*/
void dvb_ads_show_logo(int32_t VideoResolution)
{
	RET_CODE ret = ERR_FAILURE;
	dvbad_show_pic_info* logo_pic = dvb_ads_mempool_alloc(&s_dvbad.mem,sizeof(dvbad_show_pic_info));
	ret = dvb_ads_get_logo_config_data();
	
	if(SUCCESS != ret)
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"LOGO config file not exit now!!!\n");
		return;
	}
	memcpy(logo_pic,&logo_pic_info,sizeof(dvbad_show_pic_info));
	DVB_ADS_DEBUG(DVBAD,INFO,"LOGO config data has been read out to the global var!!!");
	if(1 != GxCore_FileExists(DVB_LOGO_PATH))
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"Get LOGO data failed!!!");
		return;
	}
	DVB_ADS_DEBUG(DVBAD,INFO,"Start to show AD LOGO\t\t");
	if(ADS_PIC_FORMAT_MPG == logo_pic->pic_type)
	{
		app_play_i_frame_logo(VideoResolution,DVB_LOGO_PATH);
		return;
	}
	else
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"The logo format is not support now!!!");
		return;
	}
}

					/*----------LOGO opt end-----------*/

					/*----------BAT opt start-----------*/
/* 
	>> save bat info to flash file!!!	
*/
RET_CODE dvb_ads_save_flash_bat_data(void)
{	
	size_t size = 0;
	handle_t file_handle = 0;
	dvbad_priv_t* ad = &s_dvbad;
	ad_save_info_t *save_info = &ad_save_info;
	memcpy(&save_info->ad_head_data,&ad->p_ad_data->ad_head_data,sizeof(ad_head_flash));
	memcpy(&save_info->ad_bat_data,&ad->p_ad_data->ad_bat_data,sizeof(bat_data));
	if(1 == GxCore_FileExists(DVB_FLASH_FILE_PATH))
	{
		GxCore_FileDelete(DVB_FLASH_FILE_PATH);
	}
	file_handle = GxCore_Open(DVB_FLASH_FILE_PATH,"a+");
	GxCore_Seek(file_handle, 0, GX_SEEK_SET);
	size = GxCore_Write(file_handle,(void*)save_info,1,sizeof(ad_save_info_t));
	GxCore_Close(file_handle);
	if(size > 0)
		return SUCCESS;
	return ERR_FAILURE;
}
/* 
	>> get bat info from flash file!!!	
*/
void dvb_ads_get_flash_bat_data(void)
{
	size_t size = 0;
	ad_save_info_t *save_info = &ad_save_info;
	handle_t file_handle = GxCore_Open(DVB_FLASH_FILE_PATH,"r");
	GxCore_Seek(file_handle, 0, GX_SEEK_SET);
	size = GxCore_Read(file_handle,(void *)save_info,1,sizeof(ad_save_info_t));
	GxCore_Close(file_handle);
	if(size > 0)
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"Read the ad_save_info_t data from flash to global var!!!");
		return;
	}
	DVB_ADS_DEBUG(DVBAD,INFO,"BAT info read failed!!!");
	return;
}
					/*----------BAT opt end-----------*/

/*---------------------show other pic start-----------*/
	//ads_data_node *dvbad_get_data_node(ads_data_node *p_node, DVBAD_POS_TYPE ad_type)
	//static dvbad_show_pic_info main_menu_pic_info; 
	//static dvbad_show_pic_info sub_menu_pic_info;
	//static dvbad_show_pic_info strip_menu_pic_info;
	//RET_CODE dvbad_get_ad(IN dvbad_program_para para,OUT dvbad_show_pic_info *p_pic,IN DVBAD_POS_TYPE ad_type)

/*get all the ads pic from the global var to the global var*/
static RET_CODE get_all_ads_pic(void)
{
	GxBusPmDataProg prog={0};
	uint32_t ppos = 0;
	dvbad_program_para stProgramPara={0};	

	s32 ret = 0;
	u8 cnt1 = 0;
	u8 cnt2 = 0;
	dvbad_show_pic_info *pic_temp = dvb_ads_mempool_alloc(&s_dvbad.mem,sizeof(dvbad_show_pic_info));
	if(NULL == pic_temp)
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"Alloc for pic_temp failed!!!");
		return ERR_FAILURE;
	}
	memset(pic_temp,0,sizeof(dvbad_show_pic_info));
		
	dvbad_show_pic_info* epg_pic = &epg_pic_info;
	dvbad_show_pic_info* volume_pic = &volume_pic_info;
	dvbad_show_pic_info* main_menu_pic =  &main_menu_pic_info;
	dvbad_show_pic_info* chlist_pic =  &chlist_pic_info;
	
	dvbad_show_pic_info* current_follow_pic =  &current_follow_pic_info;
 	dvbad_show_pic_info* channel_info_pic =  &channel_info_pic_info;
	
	//dvbad_priv_t *ad = &s_dvbad;
	//ads_data_node* p_node = &(ad->p_ad_data->dvbad_pic_attribute_node);

	/* >> LOGO data has been dealed with specified function!!!*/
	/* >> if the unrelated ads has been got,no need to get again!!!*/

	/*first check wheather the prog is exit,if not exit,return flase!!!*/
	DVB_ADS_PRINTF(".....................................................\n");
	if(FALSE == program_unrelated_ads_got_flag)
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"****************Start to get All the unrelated ads first time...");
		//-----------------------epg----------------------------
		ret = dvbad_get_ad_by_type_name(pic_temp,"EPG");
#if 0
		ret = dvbad_get_ad(NULL,pic_temp,DVBAD_STB_EPG);
#endif
		if(SUCCESS == ret)
		{
			cnt1++;
			memcpy(epg_pic,pic_temp,sizeof(dvbad_show_pic_info));
			DVB_ADS_DEBUG(DVBAD,INFO,"Get the EPG info: pic_id=%d pic_type=%d pos_type=%d type_name=%s data_len=%u x=%d y=%d w=%d h=%d\n",\
					epg_pic->pic_id,epg_pic->pic_type,epg_pic->pos_type,(char*)epg_pic->type_name,(unsigned int)epg_pic->data_len,\
					epg_pic->x,epg_pic->y,epg_pic->w,epg_pic->h);
			DVB_ADS_DEBUG(DVBAD,INFO,"********Saved the EPG ad pic to global var!!!\n");
		}
		else
		{
			epg_pic = NULL;
			DVB_ADS_DEBUG(DVBAD,INFO,"The EPG ad not parsed out!!!");
		}

		//---------------------volume----------------------------
		ret = dvbad_get_ad_by_type_name(pic_temp,"VOLUME");
#if 0
		ret = dvbad_get_ad(NULL,pic_temp,DVBAD_STB_VOLUME);
#endif
		if(SUCCESS == ret)
		{
			cnt1++;
			memcpy(volume_pic,pic_temp,sizeof(dvbad_show_pic_info));
			DVB_ADS_DEBUG(DVBAD,INFO,"Get the VOLUME info:pic_id=%d pic_type=%d pos_type=%d type_name=%s data_len=%u x=%d y=%d w=%d h=%d\n",\
					volume_pic->pic_id,volume_pic->pic_type,volume_pic->pos_type,(char*)volume_pic->type_name,(unsigned int)volume_pic->data_len,\
					volume_pic->x,volume_pic->y,volume_pic->w,volume_pic->h);
			DVB_ADS_DEBUG(DVBAD,INFO,"********Saved the VOLUME ad pic to global var!!!");
		}
		else
		{
			volume_pic = NULL;
			DVB_ADS_DEBUG(DVBAD,INFO,"The VOLUME ad not parsed out!!!");
		}

		//-----------------------main menu--------------------------
		ret = dvbad_get_ad(NULL,pic_temp,DVBAD_STB_MAINMENU);	
		if(SUCCESS == ret)
		{
			cnt1++;
			memcpy(main_menu_pic,pic_temp,sizeof(dvbad_show_pic_info));
			DVB_ADS_DEBUG(DVBAD,INFO,"Get the MAIN MENU info:pic_id=%d pic_type=%d pos_type=%d data_len=%u x=%d y=%d w=%d h=%d\n",\
				main_menu_pic->pic_id,main_menu_pic->pic_type,main_menu_pic->pos_type,(unsigned int)main_menu_pic->data_len,main_menu_pic->x,\
				main_menu_pic->y,main_menu_pic->w,main_menu_pic->h);
			DVB_ADS_DEBUG(DVBAD,INFO,"********Saved the MAIN MENU ad pic to global var!!!");
		}
		else
		{
			main_menu_pic = NULL;
			DVB_ADS_DEBUG(DVBAD,INFO,"The MAIN MENU ad not parsed out!!!");
		}

		//------------------------chlist--------------------------
		ret = dvbad_get_ad_by_type_name(pic_temp,"CHLIST");
#if 0		
		ret = dvbad_get_ad(NULL,pic_temp,DVBAD_STB_CHLIST);
#endif
		if(SUCCESS == ret)
		{
			cnt1++;
			memcpy(chlist_pic,pic_temp,sizeof(dvbad_show_pic_info));
			DVB_ADS_DEBUG(DVBAD,INFO,"Get the CHLIST info:pic_id=%d pic_type=%d pos_type=%d type_name=%s data_len=%u x=%d chlist_pic->y=%d w=%d h=%d\n",\
					chlist_pic->pic_id,chlist_pic->pic_type,chlist_pic->pic_type,(char*)chlist_pic->type_name,(unsigned int)chlist_pic->data_len,\
					chlist_pic->x,chlist_pic->y,chlist_pic->w,chlist_pic->h);
			DVB_ADS_DEBUG(DVBAD,INFO,"********The CHLIST ad pic has been saved to global var!!!");
		}
		else
		{
			chlist_pic = NULL;
			DVB_ADS_DEBUG(DVBAD,INFO,"The CHLIST ad not parsed out!!!");
		}

		DVB_ADS_DEBUG(DVBAD,INFO,"****************Unrelated ads got end!!!. %d unrelated ads has been got!!!",cnt1);
		unrelated_ads_cnt = cnt1;
		
	}
	else{
		DVB_ADS_DEBUG(DVBAD,INFO,"The program unrelated ad type has been saved aready!!!\n");
		cnt1 = unrelated_ads_cnt;
	}

	if(cnt1 > 0)
	{
		program_unrelated_ads_got_flag = TRUE;
	}
	else
	{
		program_unrelated_ads_got_flag = FALSE;
	}

	//-------------------------------------------------------------
	DVB_ADS_DEBUG(DVBAD,INFO,"****************Start to get the related ads ...");	
	/**get the program info*/
	cnt2 = 0;		
	if(GxBus_PmProgNumGet()>0)
	{
		app_prog_get_playing_pos_in_group(&ppos);
		int ret_program = GxBus_PmProgGetByPos(ppos,1,&prog);
		if(-1 == ret_program)
		{
			DVB_ADS_DEBUG(DVBAD,INFO,"No program!!!");
			dvb_ads_mempool_free(&s_dvbad.mem,(void*)pic_temp,sizeof(dvbad_show_pic_info));
			if(cnt1 >= 1)
			{
				return SUCCESS;
			}				
			return ERR_FAILURE;
		}
		stProgramPara.usNetworkId = prog.original_id;
		stProgramPara.usTsId      = prog.ts_id;
		stProgramPara.usServiceId = prog.service_id;
		DVB_ADS_DEBUG(DVBAD,INFO,"The current program NetID:%u,TsID:%u,SerID:%u",stProgramPara.usNetworkId,stProgramPara.usNetworkId,stProgramPara.usServiceId);
		//-----------------------current follow---------------------------
		ret = dvbad_get_ad(&stProgramPara,pic_temp,DVBAD_STB_CURRENT_FOLLOW);
		if(SUCCESS == ret)
		{
			cnt2++;
			memcpy(current_follow_pic,pic_temp,sizeof(dvbad_show_pic_info));
			DVB_ADS_DEBUG(DVBAD,INFO,"Get the CURRENT_FOLLOW info:pic_id = %d pic_type = %d pos_type = %d service_id = %d data_len=%u x=%d y=%d w=%d h=%d\n",\
					current_follow_pic->pic_id,current_follow_pic->pic_type, \
					current_follow_pic->pos_type,current_follow_pic->service_id,(unsigned int)current_follow_pic->data_len,current_follow_pic->x, \
					current_follow_pic->y,current_follow_pic->w,current_follow_pic->h);
			DVB_ADS_DEBUG(DVBAD,INFO,"********The CURRENT_FOLLOW ad pic has been saved to global var!!!");
		}
		else
		{
			DVB_ADS_DEBUG(DVBAD,INFO,"Service id is wrong!!!");
		}
		//----------------------channel info------------------------
		ret = dvbad_get_ad(&stProgramPara,pic_temp,DVBAD_STB_CHANNEL_INFO);
		if(SUCCESS == ret)
		{
			cnt2++;
			memcpy(channel_info_pic,pic_temp,sizeof(dvbad_show_pic_info));
			DVB_ADS_DEBUG(DVBAD,INFO,"Get the CHANNEL_INFO info:pic_id=%d pic_type=%d pos_type=%d service_id=%d data_len=%u x=%d y=%d w=%d h=%d\n",\
					channel_info_pic->pic_id,channel_info_pic->pic_type,channel_info_pic->pos_type,channel_info_pic->service_id,\
					(unsigned int)channel_info_pic->data_len,channel_info_pic->x,channel_info_pic->y,channel_info_pic->w,channel_info_pic->h);
			DVB_ADS_DEBUG(DVBAD,INFO,"********The CHANNEL_INFO ad pic has been saved to global var!!!");
		}
		DVB_ADS_DEBUG(DVBAD,INFO,"%d type of program-related ads parsed out!!!",cnt2);
	}
	else
	{
		dvb_ads_mempool_free(&s_dvbad.mem,(void*)pic_temp,sizeof(dvbad_show_pic_info));
		return ERR_FAILURE;
	}	
	
	//-------------------------------------------------------------
	if(cnt1+cnt2 > 0)
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"****************%d types ads has been got in all just now!!!\n",cnt1+cnt2);
		dvb_ads_mempool_free(&s_dvbad.mem,(void*)pic_temp,sizeof(dvbad_show_pic_info));
		return SUCCESS;
	}
	dvb_ads_mempool_free(&s_dvbad.mem,(void*)pic_temp,sizeof(dvbad_show_pic_info));
	return ERR_FAILURE;
}


/*Save the global var ads pic to the dram file!!!*/
RET_CODE save_all_pic_to_dram(void)
{
	u8 cnt_pro = 0;//The count of programed related ads
	u8 cnt_nopro = 0;//The count of programed unrelated ads
	size_t ret = 0;
	handle_t file_handle;
	dvbad_priv_t* ad = &s_dvbad;
	
	dvbad_show_pic_info* epg_pic = &epg_pic_info;
	dvbad_show_pic_info* volume_pic = &volume_pic_info;
	dvbad_show_pic_info* main_menu_pic =  &main_menu_pic_info;//
	dvbad_show_pic_info* chlist_pic =  &chlist_pic_info;
	
	dvbad_show_pic_info* current_follow_pic =  &current_follow_pic_info;
 	dvbad_show_pic_info* channel_info_pic =  &channel_info_pic_info;

//----------------------------main menu---------------------------
	if(FALSE == program_unrelated_ads_saved_flag)
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"-----------------Start to save unrelated ads first time!!!---------------------\n");
#if 0	
		ret = app_save_data_to_ddram_file((char *)main_menu_pic->pic_data,
            				            main_menu_pic->data_len,
            				            DVB_ADS_MAIN_MENU_PATH);
#endif
		if(main_menu_pic->data_len > 0)
		{

			if(1 == GxCore_FileExists(DVB_ADS_MAIN_MENU_PATH))
			{
				GxCore_FileDelete(DVB_ADS_MAIN_MENU_PATH);
			}
		
			file_handle = GxCore_Open(DVB_ADS_MAIN_MENU_PATH,"a+");
			GxCore_Seek(file_handle, 0, GX_SEEK_SET);
			ret = GxCore_Write(file_handle,(void*)main_menu_pic->pic_data,1,main_menu_pic->data_len);
			DVB_ADS_DEBUG(DVBAD,INFO,"ret = 0x%x\n",ret);
			DVB_ADS_DEBUG(DVBAD,INFO,"The main_menu_pic saved to file :%s,size = %u",(char*)DVB_ADS_MAIN_MENU_PATH,(unsigned int)main_menu_pic->data_len);
			GxCore_Close(file_handle);

			if(ret > 0)
			{
				DVB_ADS_DEBUG(DVBAD,INFO,"Save the main menu to file info:pic_id:%d x:%d y:%d w:%d h:%d",\
						main_menu_pic->pic_id,main_menu_pic->x,main_menu_pic->y,main_menu_pic->w,main_menu_pic->h);
				if(1 == GxCore_FileExists(DVB_ADS_MAIN_MENU_CFG_PATH))
				{
					GxCore_FileDelete(DVB_ADS_MAIN_MENU_CFG_PATH);
				}
				file_handle = GxCore_Open(DVB_ADS_MAIN_MENU_CFG_PATH,"a+");
				GxCore_Seek(file_handle, 0, GX_SEEK_SET);
				ret = GxCore_Write(file_handle,(void*)main_menu_pic,1,sizeof(dvbad_show_pic_info));
				DVB_ADS_DEBUG(DVBAD,INFO,"ret = 0x%x\n",ret);
				GxCore_Close(file_handle);
				
				if(ret > 0)
				{
					cnt_nopro++;
					DVB_ADS_DEBUG(DVBAD,INFO,"Saved main menu pic to dram file success!!!\n");
				}
				else
				{
					DVB_ADS_DEBUG(DVBAD,INFO,"Saved main menu pic to dram file failed!!!  cfg");
				}
				//test to read out the main menu cfg info
				file_handle = GxCore_Open(DVB_ADS_MAIN_MENU_CFG_PATH,"r");
				GxCore_Seek(file_handle, 0, GX_SEEK_SET);
				dvbad_show_pic_info temp = {0};
				ret = GxCore_Read(file_handle,(void*)&temp,1,sizeof(dvbad_show_pic_info));
				GxCore_Close(file_handle);
				DVB_ADS_DEBUG(DVBAD,INFO,"Try read out from main menu cfg file info:temp.x = %d,temp.y = %d,temp.w = %d,temp.h = %d\n",\
						temp.x,temp.y,temp.w,temp.h);
			}
			else
			{
				DVB_ADS_DEBUG(DVBAD,INFO,"Saved main menu pic to dram file failed!!! pic data");
			}
		}
	//--------------------epg------------------------------------	
		if(epg_pic->data_len > 0)
		{	
			if(1 == GxCore_FileExists(DVB_ADS_EPG_PATH))
			{
				GxCore_FileDelete(DVB_ADS_EPG_PATH);
			}
			file_handle = GxCore_Open(DVB_ADS_EPG_PATH,"a+");
			GxCore_Seek(file_handle, 0, GX_SEEK_SET);
			ret = GxCore_Write(file_handle,(void*)epg_pic->pic_data,1,epg_pic->data_len);
			DVB_ADS_DEBUG(DVBAD,INFO,"ret = 0x%x \n",ret);
			GxCore_Close(file_handle);
			if(ret > 0)
			{
				if(1 == GxCore_FileExists(DVB_ADS_EPG_CFG_PATH))
				{
					GxCore_FileDelete(DVB_ADS_EPG_CFG_PATH);
				}
				file_handle = GxCore_Open(DVB_ADS_EPG_CFG_PATH,"a+");
				GxCore_Seek(file_handle, 0, GX_SEEK_SET);
				ret = GxCore_Write(file_handle,(void*)epg_pic,1,sizeof(dvbad_show_pic_info));
				DVB_ADS_DEBUG(DVBAD,INFO,"ret = 0x%x\n",ret);
				GxCore_Close(file_handle);
				if(ret > 0)
				{
					cnt_nopro++;
					DVB_ADS_DEBUG(DVBAD,INFO,"Saved epg pic to dram file success!!!\n");
				}
				else
				{
					DVB_ADS_DEBUG(DVBAD,INFO,"Saved epg pic to dram file failed!!! cfg");
				}
			}
			else
			{
				DVB_ADS_DEBUG(DVBAD,INFO,"Saved epg pic to dram file failed!!! pic data");
			}
		}
	//------------------volume-------------------------------------
		if(volume_pic->data_len > 0)
		{
			if(1 == GxCore_FileExists(DVB_ADS_VOLUME_PATH))
			{
				GxCore_FileDelete(DVB_ADS_VOLUME_PATH);
			}
			file_handle = GxCore_Open(DVB_ADS_VOLUME_PATH,"a+");
			GxCore_Seek(file_handle, 0, GX_SEEK_SET);
			ret = GxCore_Write(file_handle,(void*)volume_pic->pic_data,1,volume_pic->data_len);
			DVB_ADS_DEBUG(DVBAD,INFO,"ret = %d",ret);
			GxCore_Close(file_handle);
			if(ret > 0)
			{
				if(1 == GxCore_FileExists(DVB_ADS_VOLUME_CFG_PATH))
				{
					GxCore_FileDelete(DVB_ADS_VOLUME_CFG_PATH);
				}
				file_handle = GxCore_Open(DVB_ADS_VOLUME_CFG_PATH,"a+");
				GxCore_Seek(file_handle, 0, GX_SEEK_SET);
				ret = GxCore_Write(file_handle,(void*)volume_pic,1,sizeof(dvbad_show_pic_info));
				DVB_ADS_DEBUG(DVBAD,INFO,"ret = %d",ret);
				GxCore_Close(file_handle);
				if(ret > 0)
				{
					cnt_nopro++;
					DVB_ADS_DEBUG(DVBAD,INFO,"Saved volume pic to dram success!!!\n");
				}
				else
				{
					DVB_ADS_DEBUG(DVBAD,INFO,"Saved volume pic to dram failed!!! cfg");
				}
			}
			else
			{
				DVB_ADS_DEBUG(DVBAD,INFO,"Saved volume pic to dram failed!!! pic data");
			}
		}
	//----------------------chlist-----------------------------------
		if(chlist_pic->data_len >0)
		{
			if(1 == GxCore_FileExists(DVB_ADS_CHLIST_PATH))
			{
				GxCore_FileDelete(DVB_ADS_CHLIST_PATH);
			}
			file_handle = GxCore_Open(DVB_ADS_CHLIST_PATH,"a+");
			GxCore_Seek(file_handle, 0, GX_SEEK_SET);
			ret = GxCore_Write(file_handle,(void*)chlist_pic->pic_data,1,chlist_pic->data_len);
			DVB_ADS_DEBUG(DVBAD,INFO,"ret = %d",ret);
			GxCore_Close(file_handle);
			if(ret > 0)
			{
				if(1 == GxCore_FileExists(DVB_ADS_CHLIST_CFG_PATH))
				{
					GxCore_FileDelete(DVB_ADS_CHLIST_CFG_PATH);
				}
				file_handle = GxCore_Open(DVB_ADS_CHLIST_CFG_PATH,"a+");
				GxCore_Seek(file_handle, 0, GX_SEEK_SET);
				ret = GxCore_Write(file_handle,(void*)chlist_pic,1,sizeof(dvbad_show_pic_info));
				DVB_ADS_DEBUG(DVBAD,INFO,"ret = %d",ret);
				GxCore_Close(file_handle);
				if(ret > 0)
				{
					cnt_nopro++;
					DVB_ADS_DEBUG(DVBAD,INFO,"Saved chlist pic to dram success!!!\n");
				}
				else
				{
					DVB_ADS_DEBUG(DVBAD,INFO,"Saved chlist pic to dram failed!!! cfg");
				}
			}
			else
			{
				DVB_ADS_DEBUG(DVBAD,INFO,"Saved chlist pic to dram failed!!! pic data");
			}
		}
//--------------------------------------------------------------
		program_unrelated_ads_saved_flag = TRUE;
		DVB_ADS_DEBUG(DVBAD,INFO,"-----------------------%d unrelated ads has been saved to dram!!!-----------------\n\n",cnt_nopro);

		DVB_ADS_DEBUG(DVBAD,INFO,"----------------------Start to save related ads to dram!!!---------------------");	
	}
//------------------------current follow------------------------------
	if(current_follow_pic->data_len >0)
	{
		if(1 == GxCore_FileExists(DVB_ADS_CURRENT_FOLLOW_PATH))
		{
			GxCore_FileDelete(DVB_ADS_CURRENT_FOLLOW_PATH);
		}
		file_handle = GxCore_Open(DVB_ADS_CURRENT_FOLLOW_PATH,"a+");
		GxCore_Seek(file_handle, 0, GX_SEEK_SET);
		ret = GxCore_Write(file_handle,(void*)current_follow_pic->pic_data,1,current_follow_pic->data_len);
		DVB_ADS_DEBUG(DVBAD,INFO,"ret = %d",ret);
		GxCore_Close(file_handle);
		if(ret > 0)
		{
			if(1 == GxCore_FileExists(DVB_ADS_CURRENT_FOLLOW_CFG_PATH))
			{
				GxCore_FileDelete(DVB_ADS_CURRENT_FOLLOW_CFG_PATH);
			}
			file_handle = GxCore_Open(DVB_ADS_CURRENT_FOLLOW_CFG_PATH,"a+");
			GxCore_Seek(file_handle, 0, GX_SEEK_SET);
			ret = GxCore_Write(file_handle,(void*)current_follow_pic,1,sizeof(dvbad_show_pic_info));
			DVB_ADS_DEBUG(DVBAD,INFO,"ret = %d",ret);
			GxCore_Close(file_handle);
			if(ret > 0)
			{
				cnt_pro++;
				DVB_ADS_DEBUG(DVBAD,INFO,"Saved chlist pic to dram success!!!\n");
			}
			else
			{
				DVB_ADS_DEBUG(DVBAD,INFO,"Saved chlist pic to dram failed!!! cfg");
			}
		}
		else
		{
			DVB_ADS_DEBUG(DVBAD,INFO,"Saved chlist pic to dram failed!!! pic data");
		}
	}
//---------------------channel info----------------------------------
	if(channel_info_pic->data_len > 0)
	{
		if(1 == GxCore_FileExists(DVB_ADS_CHANNEL_INFO_PATH))
		{
			GxCore_FileDelete(DVB_ADS_CHANNEL_INFO_PATH);
		}
		file_handle = GxCore_Open(DVB_ADS_CHANNEL_INFO_PATH,"a+");
		GxCore_Seek(file_handle, 0, GX_SEEK_SET);
		ret = GxCore_Write(file_handle,(void*)channel_info_pic->pic_data,1,channel_info_pic->data_len);
		DVB_ADS_DEBUG(DVBAD,INFO,"ret = %d",ret);
		GxCore_Close(file_handle);
		if(ret > 0)
		{
			if(1 == GxCore_FileExists(DVB_ADS_CHANNEL_INFO_CFG_PATH))
			{
				GxCore_FileDelete(DVB_ADS_CHANNEL_INFO_CFG_PATH);
			}
			file_handle = GxCore_Open(DVB_ADS_CHANNEL_INFO_CFG_PATH,"a+");
			GxCore_Seek(file_handle, 0, GX_SEEK_SET);
			ret = GxCore_Write(file_handle,(void*)channel_info_pic,1,sizeof(dvbad_show_pic_info));
			DVB_ADS_DEBUG(DVBAD,INFO,"ret = %d",ret);
			GxCore_Close(file_handle);
			if(ret > 0)
			{
				cnt_pro++;
				DVB_ADS_DEBUG(DVBAD,INFO,"Saved channel_info pic to dram success!!!\n");
			}
			else
			{
				DVB_ADS_DEBUG(DVBAD,INFO,"Saved channel_info pic to dram failed!!! cfg\n");
			}
		}
		else
		{
			DVB_ADS_DEBUG(DVBAD,INFO,"Saved channel_info pic to dram failed!!! pic data\n");
		}
	}

	DVB_ADS_DEBUG(DVBAD,INFO,"-----------------%d program related ads saved to dram end!!!-------------------------\n\n",cnt_pro);	
	if((cnt_pro+cnt_nopro) < ad->p_ad_data->ad_head_data.ad_pic_num)
	{
		ad->p_ad_data->ad_head_data.save_all_pic_flag = FALSE;
	}	
	else
	{
		ad->p_ad_data->ad_head_data.save_all_pic_flag = TRUE;	
	}
	DVB_ADS_DEBUG(DVBAD,INFO,"%hhd types ads pic has been saved just now!!!\n",cnt_pro+cnt_nopro);
	if(( cnt_pro > 0 )||(cnt_nopro > 0))
		return SUCCESS;
	return ERR_FAILURE;
}

#if 0
static void dvb_ads_show_gif_callback(void * data)
{
	status_t sret = 0;
	char * focus_window = NULL;
	int alu = GX_ALU_ROP_COPY;//GX_ALU_ROP_COPY_INVERT;
	dvb_ads_pic_t * ptr = (dvb_ads_pic_t *)data;
    	if(!ptr)
	{
		return;
	}

	/* avoid temporay dialog box when show main menu ad */
	if(strcasecmp("win_main_menu_image_ad", (char*)ptr->IconName) == 0)
	{
		focus_window = (char*)GUI_GetFocusWindow();

		if(strcasecmp("win_main_menu", focus_window) != 0)
		    return;
	}

	if(ptr->isShown == 0)
	{
		sret = GUI_SetProperty((char*)ptr->IconName, "state", "show");
		DVB_ADS_DEBUG(DVBAD,INFO,"sret = %d\n",sret);/**/
		ptr->isShown = 1;
		return;
	}
	if(ptr->isShown == 1 && ptr->ptrTimer)
	{
		ptr->ptrTimer->delta_time = 800;
	}
	sret = GUI_SetProperty((char*)ptr->IconName, "draw_gif", (void *)&alu);
	GUI_SetInterface("flush", NULL);
	if(sret)
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"sret = %d\n",sret);
	}
}
#endif

/*Show or hidden of all the fixed ADS pic*/
/*	Return: 1 for success,0 for failed!!!	 */
u8 dvb_ads_pic_ctrl(DVBAD_POS_TYPE ad_type,DVB_AD_PIC_OPT opt,char* IconName)
{
	//u32 pro_num = 0;;
	//u32* ppos = NULL;
	handle_t file_handle = 0;
	u8 bRet = 1;
	size_t size = 0;
	status_t sret = 0;
	dvbad_show_pic_info show_info = {0};
//	u16 x = 0,y = 0,w = 0,h = 0;
	int alu = GX_ALU_ROP_COPY;
	dvb_ads_pic_t ptr_pic = {0};
	
	if((FALSE == search_lock_main_tp_flag) ||  (GET_AD_TRUE != g_ads_get_flag) ||(GxBus_PmProgNumGet()<=0))
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"AD IN TROUBEL-->LOCK MAIN TP ERROR || NO AD FITLER OUT || NO PROGRAM\n\n");
		return 0;
	}
	//if the pics are show in the same windows ,just need to load once.
	if(LOAD_PIC_DATA == opt)/*load*/
	{
		//if(TRUE == dvb_ads_version_need_update_flag)	
		//{	
			RET_CODE ret = ERR_FAILURE;
			DVB_ADS_PRINTF("-------------Start to load all ads pic!!!-------------\n");
			ret = get_all_ads_pic();
		
			if(SUCCESS != ret)
			{
				DVB_ADS_DEBUG(DVBAD,INFO,"Got no data!!!!");
				bRet = 0;
				return bRet;
			}
			DVB_ADS_DEBUG(DVBAD,INFO,"Start to save all the pics to flash file!!!");
			ret = save_all_pic_to_dram();
			DVB_ADS_DEBUG(DVBAD,INFO,"End save all the pics to flash file!!!");
			if(SUCCESS != ret)
			{
				DVB_ADS_DEBUG(DVBAD,INFO,"NO DATA SAVED!!!");
				bRet = 0;
				return bRet;
			}
		//}
		DVB_ADS_DEBUG(DVBAD,INFO,"DVB AD has aready been saved to flash file!!!");
	}

	else if(SHOW_PIC_DATA == opt)
	{
		switch (ad_type)
		{
			case DVBAD_STB_EPG:
			{	
				if(1 != GxCore_FileExists(DVB_ADS_EPG_CFG_PATH))
				{
					DVB_ADS_DEBUG(DVBAD,INFO,"No epg cfg file in flash!!!");
					bRet = 0;
					break;
				}
				if(1 != GxCore_FileExists(DVB_ADS_EPG_PATH))
				{
					DVB_ADS_DEBUG(DVBAD,INFO,"No epg pic file in flash!!!");
					bRet = 0;
					break;
				}
				DVB_ADS_DEBUG(DVBAD,INFO,"-----EPG pic file exit,opening...");
				file_handle = GxCore_Open(DVB_ADS_CHANNEL_INFO_CFG_PATH,"r");
				ptr_pic.filePath = (u8*)DVB_ADS_EPG_PATH;
				break;
			}
			case  DVBAD_STB_VOLUME:
			{
				if(1 != GxCore_FileExists(DVB_ADS_VOLUME_CFG_PATH))
				{
					DVB_ADS_DEBUG(DVBAD,INFO,"No volum cfg file in flash!!!");
					bRet = 0;
					break;
				}
				if(1 != GxCore_FileExists(DVB_ADS_VOLUME_PATH))
				{
					DVB_ADS_DEBUG(DVBAD,INFO,"No volume pic file in flash!!!");
					bRet = 0;
					break;
				}
				DVB_ADS_DEBUG(DVBAD,INFO,"-----VOLUME pic file exit,opening...");
				file_handle = GxCore_Open(DVB_ADS_VOLUME_CFG_PATH,"r");
				ptr_pic.filePath = (u8*)DVB_ADS_VOLUME_PATH;
				break;
			}
			case  DVBAD_STB_MAINMENU:
			{
				if(1 != GxCore_FileExists(DVB_ADS_MAIN_MENU_CFG_PATH))
				{
					DVB_ADS_DEBUG(DVBAD,INFO,"No main menu cfg file in flash!!!");
					bRet = 0;
					break;
				}
				if(1 != GxCore_FileExists(DVB_ADS_MAIN_MENU_PATH))
				{
					DVB_ADS_DEBUG(DVBAD,INFO,"No main menu pic file in flash!!!");
					bRet = 0;
					break;
				}
				DVB_ADS_DEBUG(DVBAD,INFO,"-----MAIN MENU pic file exit,opening...");
				file_handle = GxCore_Open(DVB_ADS_MAIN_MENU_CFG_PATH,"r");
				ptr_pic.filePath = (u8*)DVB_ADS_MAIN_MENU_PATH;
				//x = ptr_pic.pic_show_info->x = 216;
				//y = ptr_pic.pic_show_info->y = 400;
				break;
			}
			case  DVBAD_STB_CHLIST:
			{
				if(1 != GxCore_FileExists(DVB_ADS_CHLIST_CFG_PATH))
				{
					DVB_ADS_DEBUG(DVBAD,INFO,"No chlist cfg file in flash!!!");
					bRet = 0;
					break;
				}
				if(1 != GxCore_FileExists(DVB_ADS_CHLIST_PATH))
				{
					DVB_ADS_DEBUG(DVBAD,INFO,"No chlist pic file in flash!!!");
					bRet = 0;
					break;
				}
				DVB_ADS_DEBUG(DVBAD,INFO,"-----CHLIST pic file exit,opening...");
				file_handle = GxCore_Open(DVB_ADS_CHLIST_CFG_PATH,"r");
				ptr_pic.filePath = (u8*)DVB_ADS_CHLIST_PATH;
				break;
			}
			case  DVBAD_STB_CURRENT_FOLLOW:
			{
				if(1 != GxCore_FileExists(DVB_ADS_CURRENT_FOLLOW_CFG_PATH))
				{
					DVB_ADS_DEBUG(DVBAD,INFO,"No current follow cfg file in flash!!!");
					bRet = 0;
					break;
				}
				if(1 != GxCore_FileExists(DVB_ADS_CURRENT_FOLLOW_PATH))
				{
					DVB_ADS_DEBUG(DVBAD,INFO,"No current follow pic file in flash!!!");
					bRet = 0;
					break;
				}
				DVB_ADS_DEBUG(DVBAD,INFO,"-----CURRENT FOLLOW pic file exit,opening...");
				file_handle = GxCore_Open(DVB_ADS_CURRENT_FOLLOW_CFG_PATH,"r");
				ptr_pic.filePath = (u8*)DVB_ADS_CURRENT_FOLLOW_PATH;
				//x = ptr_pic.pic_show_info->x = 440;
				//y = ptr_pic.pic_show_info->y = 403;
				break;
			}
			case DVBAD_STB_CHANNEL_INFO:
			{
				if(1 != GxCore_FileExists(DVB_ADS_CHANNEL_INFO_CFG_PATH))
				{
					DVB_ADS_DEBUG(DVBAD,INFO,"No channel info cfg file in flash!!!");
					bRet = 0;
					break;
				}
				if(1 != GxCore_FileExists(DVB_ADS_CHANNEL_INFO_PATH))
				{
					DVB_ADS_DEBUG(DVBAD,INFO,"No channel info pic file in flash!!!");
					bRet = 0;
					break;
				}
				DVB_ADS_DEBUG(DVBAD,INFO,"-----CHANNEL INFO pic file exit,opening...");
				file_handle = GxCore_Open(DVB_ADS_CHANNEL_INFO_CFG_PATH,"r");
				ptr_pic.filePath = (u8*)DVB_ADS_CHANNEL_INFO_PATH;
				
				break;
			}
			default:
			{
				bRet = 0;
				DVB_ADS_DEBUG(DVBAD,ERR,"Wrong type!!!");
				return bRet;
			}
	
		}

		if(bRet == 0)
		{
			GxCore_Close(file_handle);
			return bRet;
		}
		else
		{
			GxCore_Seek(file_handle, 0, GX_SEEK_SET);
			size = GxCore_Read(file_handle,(void*)&show_info,1,sizeof(dvbad_show_pic_info));
			DVB_ADS_DEBUG(DVBAD,INFO,"show_info.x = %d,show_info.y = %d,show_info.w = %d,show_info.h = %d\n",show_info.x,show_info.y,show_info.w,show_info.h);

			
			/*(dvbad_show_pic_info *)dvb_ads_mempool_alloc(&s_dvbad.mem,sizeof(dvbad_show_pic_info));
			//memcpy((void*)&ptr_pic.pic_show_info,(void *)&show_info,sizeof(dvbad_show_pic_info));
			ptr_pic.pic_show_info->x = show_info.x;
			ptr_pic.pic_show_info->y = show_info.y;
			ptr_pic.pic_show_info->w = show_info.w;
			ptr_pic.pic_show_info->h = show_info.h;
			*/
			ptr_pic.pic_show_info = &show_info;
			DVB_ADS_DEBUG(DVBAD,INFO,"pic_show_info x=%d,y=%d,w=%d,h=%d\n",\
					ptr_pic.pic_show_info->x,ptr_pic.pic_show_info->y,ptr_pic.pic_show_info->w,ptr_pic.pic_show_info->h);
			GxCore_Close(file_handle);
			if(size < 0)
			{
				DVB_ADS_DEBUG(DVBAD,INFO,"------Read out cfg data from flash failed!!!");
				bRet = 0;
				return bRet;
			}
			DVB_ADS_DEBUG(DVBAD,INFO,"-----READ OUT the type of ad success");
		}
	
		strcpy((char*)ptr_pic.IconName,IconName);
		DVB_ADS_DEBUG(DVBAD,INFO,"This type of AD will be shown in the widget IconName = %s\n",IconName);
		//dvb_ads_set_widget_xywh(ptr_pic.IconName,x,y,w,h);
		DVB_ADS_DEBUG(DVBAD,INFO,"Start to load this type of AD to widget %s!!!",IconName);
		sret = GUI_SetProperty((char*)ptr_pic.IconName,"load_zoom_img",ptr_pic.filePath);
		DVB_ADS_DEBUG(DVBAD,INFO,"LOAD PIC sret = %d",sret);
		sret = GUI_SetProperty((char*)ptr_pic.IconName,"init_gif_alu_mode",&alu);
		DVB_ADS_DEBUG(DVBAD,INFO,"LOAD PIC sret = %d",sret);
		sret = GUI_SetProperty((char*)ptr_pic.IconName,"draw_gif",&alu);
		DVB_ADS_DEBUG(DVBAD,INFO,"LOAD PIC sret = %d",sret);
		DVB_ADS_DEBUG(DVBAD,INFO,"----------------End load this type of AD to widget!!!---------------");
		/*Create GIF update timer*/
		if(ptr_pic.ptrTimer)
		{
			remove_timer(ptr_pic.ptrTimer);
		}
		//ptr_pic.ptrTimer = create_timer((timer_event)dvb_ads_show_gif_callback,10,&ptr_pic,TIMER_REPEAT);
		bRet = 1;

		sret = GUI_SetProperty((char *)ptr_pic.IconName, "state", "show");
		GUI_SetInterface("flush", NULL);
		DVB_ADS_DEBUG(DVBAD,INFO,"sret = %d\n",sret);
	}
	else if (DRAW_PIC_DATA == opt)//redraw
	{
		sret = GUI_SetProperty((char *)ptr_pic.IconName, "draw_gif", &alu);
		DVB_ADS_DEBUG(DVBAD,INFO,"sret = %d\n",sret);
	}
	else if (HIDE_PIC_DATA == opt)
	{
		sret = GUI_SetProperty((char *)ptr_pic.IconName, "state", "hide");
		DVB_ADS_DEBUG(DVBAD,INFO,"sret = %d\n",sret);
	}
	else if (FREE_PIC_DATA == opt)
	{
		if(ptr_pic.ptrTimer)
		{
			remove_timer(ptr_pic.ptrTimer);
		}
		sret = GUI_SetProperty((char *)ptr_pic.IconName,(char *)"load_img",(void *)NULL);
		DVB_ADS_DEBUG(DVBAD,INFO,"sret = %d\n",sret);
		sret = GUI_SetProperty((char *)ptr_pic.IconName, (char *)"state", "hide");
    		GUI_SetInterface("flush", NULL);

		DVB_ADS_DEBUG(DVBAD,INFO,"sret = %d\n",sret);
		memset(&ptr_pic,0,sizeof(dvb_ads_pic_t));
		if (GxCore_FileExists((char*)ptr_pic.filePath) == 1)
		{
			GxCore_FileDelete((char*)ptr_pic.filePath);	
		}
	}
	return bRet;	
}

void dvb_ads_menu_pic_display(char*  IconeName)
{
	app_dvb_ads_sem_take(show_pic_sem);
	dvb_ads_pic_ctrl(DVBAD_STB_MAINMENU,LOAD_PIC_DATA,IconeName);
	dvb_ads_pic_ctrl(DVBAD_STB_MAINMENU,SHOW_PIC_DATA,IconeName);
	app_dvb_ads_sem_post(show_pic_sem);
}
void dvb_ads_hide_menu_pic(char*  IconeName)
{
	app_dvb_ads_sem_take(show_pic_sem);
	dvb_ads_pic_ctrl(DVBAD_STB_MAINMENU,HIDE_PIC_DATA,IconeName);
	dvb_ads_pic_ctrl(DVBAD_STB_MAINMENU,FREE_PIC_DATA,IconeName);
	app_dvb_ads_sem_post(show_pic_sem);
}

void dvb_ads_epg_pic_display(char* IconeName)
{
	app_dvb_ads_sem_take(show_pic_sem);
	dvb_ads_pic_ctrl(DVBAD_STB_EPG,LOAD_PIC_DATA,IconeName);
	dvb_ads_pic_ctrl(DVBAD_STB_EPG,SHOW_PIC_DATA,IconeName);
	app_dvb_ads_sem_post(show_pic_sem);
}

void dvb_ads_hide_epg_pic(char*  IconeName)
{
	app_dvb_ads_sem_take(show_pic_sem);
	dvb_ads_pic_ctrl(DVBAD_STB_EPG,HIDE_PIC_DATA,IconeName);
	dvb_ads_pic_ctrl(DVBAD_STB_EPG,FREE_PIC_DATA,IconeName);
	app_dvb_ads_sem_post(show_pic_sem);

}
void dvb_ads_vol_pic_display(char*  IconeName)
{
	app_dvb_ads_sem_take(show_pic_sem);
	dvb_ads_pic_ctrl(DVBAD_STB_VOLUME,LOAD_PIC_DATA,IconeName);
	dvb_ads_pic_ctrl(DVBAD_STB_VOLUME,SHOW_PIC_DATA,IconeName);
	app_dvb_ads_sem_post(show_pic_sem);
}

void dvb_ads_hide_vol_pic(char*  IconeName)
{
	app_dvb_ads_sem_take(show_pic_sem);
	dvb_ads_pic_ctrl(DVBAD_STB_VOLUME,HIDE_PIC_DATA,IconeName);
	dvb_ads_pic_ctrl(DVBAD_STB_VOLUME,FREE_PIC_DATA,IconeName);
	app_dvb_ads_sem_post(show_pic_sem);
}

void dvb_ads_chlist_pic_display(char*  IconeName)
{
	app_dvb_ads_sem_take(show_pic_sem);
	dvb_ads_pic_ctrl(DVBAD_STB_CHLIST,LOAD_PIC_DATA,IconeName);
	dvb_ads_pic_ctrl(DVBAD_STB_CHLIST,SHOW_PIC_DATA,IconeName);
	app_dvb_ads_sem_post(show_pic_sem);
}

void dvb_ads_hide_chlist_pic(char*  IconeName)
{
	app_dvb_ads_sem_take(show_pic_sem);
	dvb_ads_pic_ctrl(DVBAD_STB_CHLIST,HIDE_PIC_DATA,IconeName);
	dvb_ads_pic_ctrl(DVBAD_STB_CHLIST,FREE_PIC_DATA,IconeName);
	app_dvb_ads_sem_post(show_pic_sem);
}

void dvb_ads_current_follow_pic_display(char*  IconeName)
{
	app_dvb_ads_sem_take(show_pic_sem);
	dvb_ads_pic_ctrl(DVBAD_STB_CURRENT_FOLLOW,LOAD_PIC_DATA,IconeName);
	dvb_ads_pic_ctrl(DVBAD_STB_CURRENT_FOLLOW,SHOW_PIC_DATA,IconeName);
	app_dvb_ads_sem_post(show_pic_sem);
}

void dvb_ads_hide_current_follow_pic(char*  IconeName)
{
	app_dvb_ads_sem_take(show_pic_sem);
	dvb_ads_pic_ctrl(DVBAD_STB_CURRENT_FOLLOW,HIDE_PIC_DATA,IconeName);
	dvb_ads_pic_ctrl(DVBAD_STB_CURRENT_FOLLOW,FREE_PIC_DATA,IconeName);
	app_dvb_ads_sem_post(show_pic_sem);
}

void dvb_ads_channel_info_display(char* IconeName)
{
	app_dvb_ads_sem_take(show_pic_sem);
	dvb_ads_pic_ctrl(DVBAD_STB_CHANNEL_INFO,LOAD_PIC_DATA,IconeName);
	dvb_ads_pic_ctrl(DVBAD_STB_CHANNEL_INFO,SHOW_PIC_DATA,IconeName);
	app_dvb_ads_sem_post(show_pic_sem);
}

void dvb_ads_hide_channel_info_pic(char*  IconeName)
{
	app_dvb_ads_sem_take(show_pic_sem);
	dvb_ads_pic_ctrl(DVBAD_STB_CHANNEL_INFO,HIDE_PIC_DATA,IconeName);
	dvb_ads_pic_ctrl(DVBAD_STB_CHANNEL_INFO,FREE_PIC_DATA,IconeName);
	app_dvb_ads_sem_post(show_pic_sem);
}
/*------------------show other pic end-------------*/


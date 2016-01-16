#include "app_common_porting_stb_api.h"
#include "sys_common_config.h"
#include "app_win_interface.h"
#include "desai_ca_api.h"

#define PIN_MAX_LENGTH   (6)

SIGNAL_HANDLER int win_change_pin_create(const char* widgetname, void *usrdata)
{	
	return 0;
}

SIGNAL_HANDLER int win_change_pin_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER int win_change_pin_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;

	if(event->type == GUI_KEYDOWN)
	{	
		switch(event->key.sym)
		{
			case KEY_EXIT:
			case KEY_MENU:	
				GUI_EndDialog("win_desai_cas50_changepin");
				return EVENT_TRANSFER_STOP;	
			case KEY_OK:
				GUI_EndDialog("win_desai_cas50_changepin");
				return EVENT_TRANSFER_STOP;	
			default:
				return EVENT_TRANSFER_STOP;	
		}
	}
	
	return EVENT_TRANSFER_KEEPON;
}

SIGNAL_HANDLER int win_change_pin_box_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
	uint8_t *value;
	uint32_t nSelect;

	INT8 newPassword1[PIN_MAX_LENGTH+1];
	INT8 newPassword2[PIN_MAX_LENGTH+1];
	INT8 newPassword3[PIN_MAX_LENGTH+1];

	char* osd_language=NULL;
	UINT8  ucOddTimes;
	EN_ERR_CAS ret;
	char buf[40];

	UINT8 oldPin[3]={0,};
	UINT8 newPin[3]={0,};
	int temp=0;
	if(event->type == GUI_KEYDOWN)
	{	
		switch(event->key.sym)
		{
			case KEY_OK:
				{
					GUI_GetProperty("win_change_pin_boxitem1_edit", "string", &value);
					memset(newPassword3, 0, PIN_MAX_LENGTH+1);
					memcpy(newPassword3,(char*)value, PIN_MAX_LENGTH);
					/*然后匹配新密码*/
					GUI_GetProperty("win_change_pin_boxitem2_edit", "string", &value);
					memset(newPassword1, 0, PIN_MAX_LENGTH+1);
					memcpy(newPassword1,(char*)value, PIN_MAX_LENGTH);
					GUI_GetProperty("win_change_pin_boxitem3_edit", "string", &value);
					memset(newPassword2,0,PIN_MAX_LENGTH+1);
					memcpy(newPassword2, (char*)value, PIN_MAX_LENGTH);	
					if(strcasecmp((char*)newPassword1,(char*)newPassword2) != 0)
					{
						osd_language = app_flash_get_config_osd_language();
						if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"两次PIN码不同!",POPMSG_TYPE_OK);
						else
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"New PIN different!",POPMSG_TYPE_OK);
						GUI_SetProperty("win_change_pin_boxitem1_edit","clear",NULL);
						GUI_SetProperty("win_change_pin_boxitem2_edit","clear",NULL);
						GUI_SetProperty("win_change_pin_boxitem3_edit","clear",NULL);
						nSelect = 0;
						GUI_SetProperty("win_change_pin_box", "select", &nSelect);
						return EVENT_TRANSFER_STOP;	
					}
					if(strlen((char*)newPassword2)!=PIN_MAX_LENGTH || strlen((char*)newPassword3)!=PIN_MAX_LENGTH)
					{
						osd_language = app_flash_get_config_osd_language();
						if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"PIN码必须为6位",POPMSG_TYPE_OK);
						else
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"PIN length must be six!",POPMSG_TYPE_OK);
						GUI_SetProperty("win_change_pin_boxitem1_edit","clear",NULL);
						GUI_SetProperty("win_change_pin_boxitem2_edit","clear",NULL);
						GUI_SetProperty("win_change_pin_boxitem3_edit","clear",NULL);
						nSelect = 0;
						GUI_SetProperty("win_change_pin_box", "select", &nSelect);
						return EVENT_TRANSFER_STOP;	
					}
					CAS_Dbg("oldPassword %s\n",newPassword3);
					CAS_Dbg("newPassword %s\n",newPassword2);
					temp=atoi(newPassword3);
					oldPin[0]=(UINT8)(temp>>16);
					oldPin[1]=(UINT8)(temp>>8);
					oldPin[2]=(UINT8)(temp&0xff);
					temp=atoi(newPassword2);
					newPin[0]=(UINT8)(temp>>16);
					newPin[1]=(UINT8)(temp>>8);
					newPin[2]=(UINT8)(temp&0xff);
					CAS_Dbg("%0x  %0x  %0x\n",newPin[0],newPin[1],newPin[2]);
					ret=DSCA_ChangePin(oldPin,newPin,&ucOddTimes);
					CAS_Dbg("%d\n",ret);
					if(ret == CA_CARD_CHECK_PIN_ERROR)
					{
						osd_language = app_flash_get_config_osd_language();
						if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
							snprintf(buf,40,"PIN码错误，你还可以试 %d 次",ucOddTimes);
						else
							snprintf(buf,40,"Pin Err，you can try %d times",ucOddTimes);
						app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,buf, POPMSG_TYPE_OK);
						return EVENT_TRANSFER_STOP;
					}
					if(ret == CA_CARD_CHECK_PIN_LOCK)
					{
						osd_language = app_flash_get_config_osd_language();
						if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"卡被锁定", POPMSG_TYPE_OK);
						else
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"CARD_CHECK_PIN_LOCK", POPMSG_TYPE_OK);
						return EVENT_TRANSFER_STOP;
					}
					if(ret == CA_CARD_SUCCESS)
					{
						osd_language = app_flash_get_config_osd_language();
						if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"设置成功!", POPMSG_TYPE_OK);
						else
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"Set successfully!", POPMSG_TYPE_OK);
						break;
					}
					if(ret == CA_CARD_FAILURE)
					{
						osd_language = app_flash_get_config_osd_language();
						if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"卡通讯失败!", POPMSG_TYPE_OK);
						else
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"CARD_FAILURE", POPMSG_TYPE_OK);
						return EVENT_TRANSFER_STOP;
					}
					if(ret == CA_ERROR_PARAMETER)
					{
						osd_language = app_flash_get_config_osd_language();
						if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"卡参数错误", POPMSG_TYPE_OK);
						else
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"ERROR_PARAMETER ", POPMSG_TYPE_OK);
						return EVENT_TRANSFER_STOP;
					}
				}
				return EVENT_TRANSFER_KEEPON;
				default:
					return EVENT_TRANSFER_KEEPON;	
			}
		}
	
	return EVENT_TRANSFER_KEEPON;
}



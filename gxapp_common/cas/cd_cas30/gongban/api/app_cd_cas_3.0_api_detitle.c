/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_ca_api.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2012.12.02		  zhouhm 	 			creation
*****************************************************************************/
#include "CDCASS.h"
#include "app_common_porting_stb_api.h"
#include "app_cd_cas_3.0_api_detitle.h"
#include "app_cd_cas_3.0_api_pop.h"

static cd_cas30_detitle_st ca_detitle_flag = {0};
typedef struct CA_DETITLE_INFO_S
{
    unsigned short                          m_OperaID;
    unsigned int                            m_nDetitleChkNum;
    unsigned char                           m_chDetitleChkNumReaded;
}CA_DETITLE_INFO_T;

unsigned char                      g_chDetitleNum  = 0;
CA_DETITLE_INFO_T       g_CaDetitles[CDCA_MAXNUM_OPERATOR* CDCA_MAXNUM_DETITLE];

extern 	void app_cd_cas30_detitle_flag_img_show(uint8_t showFlag);
extern char * GUI_GetFocusWindow(void);

int32_t app_cd_cas30_gxmsg_ca_on_event_detitle(GxMsgProperty0_OnEvent* event)
{
	cd_cas30_detitle_st* detitle_flag = NULL;
	if (NULL == event)
		return 0;

	detitle_flag = (cd_cas30_detitle_st*)(event->buf);

	if (NULL == detitle_flag)
		return 0;

	ca_detitle_flag.wStatus = detitle_flag->wStatus;
	ca_detitle_flag.uCount = 0;
//	memcpy(&ca_detitle_flag,detitle_flag,sizeof(cd_cas30_detitle_st));


	
	return 1;
}

/*
* 反授权图标显示、隐藏
*/
int32_t app_cd_cas30_detitle_flag_exec()
{
	uint8_t detitle_status = ca_detitle_flag.wStatus;
	char* focus_Window = (char*)GUI_GetFocusWindow();
	if ((NULL != focus_Window)&&(strcasecmp("win_full_screen", focus_Window) == 0))
		return 0;
	if ( CDCA_Detitle_Received == detitle_status)
	{	
		/* 收到新的反授权码，显示反授权码图标 */
		if (FALSE == ca_detitle_flag.wShowStatus)
		{
			app_cd_cas30_detitle_flag_img_show(CD_DETITLE_FLAG_SHOW);
			ca_detitle_flag.wShowStatus = TRUE;
		}
	}
	else if ( CDCA_Detitle_Space_Small == detitle_status)
	{
		ca_detitle_flag.uCount++;
		ca_detitle_flag.uCount = ca_detitle_flag.uCount%3;
		
		/* 反授权码空间不足，改变图标状态提示用户 */

		if (0 != ca_detitle_flag.uCount)
		{						
			app_cd_cas30_detitle_flag_img_show(CD_DETITLE_FLAG_SHOW);
			ca_detitle_flag.wShowStatus = TRUE;
		}
		else
		{
			app_cd_cas30_detitle_flag_img_show(CD_DETITLE_FLAG_TRANS_HIDE);						
			ca_detitle_flag.wShowStatus = FALSE;
		}					
	}
	else if (CDCA_Detitle_All_Read == detitle_status)
	{	
		/* 所有反授权确认码已经被读，隐藏图标 */
		if (TRUE == ca_detitle_flag.wShowStatus)
		{
			app_cd_cas30_detitle_flag_img_show(CD_DETITLE_FLAG_HIDE);
			ca_detitle_flag.wShowStatus = FALSE;
		}
	}
	return 1;
	
}



uint8_t app_cd_cas30_api_init_detitle_data(void)
{
    unsigned short wRet                                    = CDCA_RC_OK;
    unsigned char  chDetitleIndex                          = 0;
    unsigned char  cIndex                                  = 0;
    unsigned char  i                                       = 0;
	unsigned char  bReadFlag;
    unsigned short wOperaID[CDCA_MAXNUM_OPERATOR]          =
    {
        0, 0, 0, 0
    };
    unsigned int nDetitleChkNums[CDCA_MAXNUM_DETITLE]    =
    {
        0
    };

    g_chDetitleNum = 0;
    memset( nDetitleChkNums, 0, CDCA_MAXNUM_OPERATOR * sizeof( unsigned int ) );

    wRet = CDCASTB_GetOperatorIds( wOperaID );
    if( CDCA_RC_OK == wRet )
    {
        for( cIndex = 0; cIndex < CDCA_MAXNUM_OPERATOR; cIndex++ )
        {
            if( 0 != wOperaID[cIndex] )
            {
                wRet = CDCASTB_GetDetitleChkNums( wOperaID[cIndex],&bReadFlag,
                                                    (CDCA_U32*)&nDetitleChkNums[0] );
                {
                    if( CDCA_RC_OK == wRet )
                    {
                        for( i = 0; i < CDCA_MAXNUM_DETITLE; i++ )
                        {
                            if( 0 != nDetitleChkNums[i] )
                            {
                                g_CaDetitles[chDetitleIndex].m_OperaID = wOperaID[cIndex];
                                g_CaDetitles[chDetitleIndex].m_nDetitleChkNum = nDetitleChkNums[i];
                                g_CaDetitles[chDetitleIndex].m_chDetitleChkNumReaded = 1;
                                chDetitleIndex++;
                            }
                        }
                    }
                }
            }
        }
    }
    g_chDetitleNum = chDetitleIndex;
    return g_chDetitleNum;
}

char * app_cd_cas30_api_get_detitle_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0;                               
	char* osd_language=app_flash_get_config_osd_language();;
    static char   TextBuffer1[40] =
    {
        0
    };
    static char   TextBuffer2[40] =
    {
        0
    };

	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	switch(ID)
		{
			case CD_DETITLE_OPERATE_ID:
				sprintf(TextBuffer1, "%d", g_CaDetitles[pos].m_OperaID);
				return TextBuffer1;
				break;
			case CD_DETITLE_CHK_NUM_ID:
    				sprintf(TextBuffer2, "%u", g_CaDetitles[pos].m_nDetitleChkNum );
				return TextBuffer2;
				break;
			case CD_DETITLE_CHK_NUM_READED_ID:
				if (1 == g_CaDetitles[pos].m_chDetitleChkNumReaded)
				{
					if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
						return (char*)"是";
					else
						return (char*)"YES";
				}
				else
				{					
					if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
						return (char*)"否";
					else
						return (char*)"NO";
				}
				break;
			default:
				break;
		}
	return NULL;
}


uint8_t app_cd_cas30_api_get_detitle_count(ca_get_count_t *data)
{	
	if (NULL == data)
		return 0;

	data->totalnum = g_chDetitleNum;
    return g_chDetitleNum;
}

bool app_cd_cas30_api_delete_detitle(ca_delete_data_t *data)
{
	uint32_t pos = 0 ;                             
	if (NULL == data)
		return FALSE;

	pos = data->pos;

	if (pos >= g_chDetitleNum)
		return FALSE;

         CDCASTB_DelDetitleChkNum( g_CaDetitles[pos].m_OperaID,g_CaDetitles[pos].m_nDetitleChkNum );
         app_cd_cas30_api_init_detitle_data();
       
	return 0;
}

bool app_cd_cas30_api_delete_all_detitle(ca_delete_data_t *data)
{

	uint32_t pos = 0 ;                             
	if (NULL == data)
		return FALSE;

    CDCASTB_DelDetitleChkNum( g_CaDetitles[pos].m_OperaID,0 );
    app_cd_cas30_api_init_detitle_data();

	return 0;
}






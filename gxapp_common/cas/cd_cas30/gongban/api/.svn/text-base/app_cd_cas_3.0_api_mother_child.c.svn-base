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
#include "app_cd_cas_3.0_api_mother_child.h"
#include "app_common_epg.h"
#include "gui_timer.h"
#include "app_cd_cas_3.0_api_pop.h"

typedef struct CA_MOTHERCARD_INFO_S
{
    unsigned short                     m_OperaID;
    unsigned char                      m_IsChildCard;
    unsigned short                     m_DelayTime;
    CDCA_TIME                           m_LastFeedTime;
    unsigned char                      m_ParentCardSN[CDCA_MAXLEN_SN + 1];
    CDCA_BOOL                           m_BIsCanFeed;
}CA_MOTHERCARD_INFO_T;

CA_MOTHERCARD_INFO_T    g_MotherCardInfo[CDCA_MAXNUM_OPERATOR]  =
{
    {0}
};
static unsigned char		   chSpbyFeedData[256] =
{
	0
};
unsigned char					Len 				= 255;

static event_list* spfeedtimer = NULL;

extern void app_cd_cas30_feed_show(uint8_t showFlag,char* message);
extern char * GUI_GetFocusWindow(void);

static  int cafeedtimer(void *userdata)
{
	if (NULL == spfeedtimer)
	{
		return 0;
	}

	app_cd_cas30_feed_show(DVB_CA_FEED_FLAG_HIDE,NULL);
	return 0;
}

static unsigned int gx_ca_calculate_time( unsigned int wDate, com_time_t* pTimeOut )
{
    unsigned short         nData       = 0;
    unsigned char          chHour      = 0;
    unsigned char          chMin       = 0;
    unsigned char          chSec       = 0;

    com_time_t* pDataOut    = pTimeOut;

    nData = ( unsigned short  ) ( wDate >> 16 );
    chHour = ( unsigned char ) ( ( wDate >> 11 ) & 0x1f );
    chMin = ( unsigned char ) ( ( wDate >> 5 ) & 0x3f );
    chSec = ( unsigned char ) ( wDate & 0x1f );

    app_epg_convert_to_utc( nData, pDataOut );

    pDataOut->hour = chHour;
    pDataOut->minute = chMin ;

    return 0;
}

int32_t app_cd_cas30_gxmsg_ca_on_event_feed(GxMsgProperty0_OnEvent* event)
{
	cd_cas30_feed_st* feed = NULL;
	char* focus_Window = (char*)GUI_GetFocusWindow();
	if (NULL == event)
		return 0;

	feed = (cd_cas30_feed_st*)(event->buf);

	if (NULL == feed)
		return 0;
	if ((NULL != focus_Window)&&(strcasecmp("win_full_screen", focus_Window) != 0))
		return 0;

	if (NULL != spfeedtimer)
	{
		app_cd_cas30_feed_show(DVB_CA_FEED_FLAG_HIDE,NULL);
		remove_timer(spfeedtimer);
		spfeedtimer = NULL;

	}
	if(feed->bReadStatus == CDCA_TRUE)
	{
		app_cd_cas30_feed_show(DVB_CA_FEED_FLAG_SHOW,"Please insert the child-card");
		spfeedtimer = create_timer(cafeedtimer, 5000, NULL,  TIMER_ONCE);
	}
	else if(feed->bReadStatus == CDCA_FALSE)
	{
		app_cd_cas30_feed_show(DVB_CA_FEED_FLAG_SHOW,"Invalid parent-card");	
		spfeedtimer = create_timer(cafeedtimer, 5000, NULL,  TIMER_ONCE);
	}

	
	return 1;
}


int32_t app_cd_cas30_api_init_mother_child_data( void )
{
    unsigned short wRet                            = CDCA_RC_OK;
    unsigned char  cIndex                          = 0;
    unsigned short wTVSID[CDCA_MAXNUM_OPERATOR]    =
    {
        0
    };

    memset( wTVSID, 0, CDCA_MAXNUM_OPERATOR * sizeof( unsigned short ) );
    memset( g_MotherCardInfo, 0,
            CDCA_MAXNUM_OPERATOR * sizeof( CA_MOTHERCARD_INFO_T ) );

    wRet = CDCASTB_GetOperatorIds( wTVSID );
    if( wRet == CDCA_RC_OK )
    {
        for( cIndex = 0; cIndex < CDCA_MAXNUM_OPERATOR; cIndex++ )
        {
            if( 0 != wTVSID[cIndex] )
            {
                g_MotherCardInfo[cIndex].m_OperaID = wTVSID[cIndex];
                wRet = CDCASTB_GetOperatorChildStatus( wTVSID[cIndex],
                                                         &g_MotherCardInfo[cIndex].m_IsChildCard,
                                                         &g_MotherCardInfo[cIndex].m_DelayTime,
                                                         &g_MotherCardInfo[cIndex].m_LastFeedTime,
                                                         (char*)(&g_MotherCardInfo[cIndex].m_ParentCardSN),
                                                         &g_MotherCardInfo[cIndex].m_BIsCanFeed);
            }
        }
    }
	return wRet;
}


char * app_cd_cas30_api_get_mother_child_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0;  
	char* osd_language=app_flash_get_config_osd_language();;
	
    static char   cText_mother_card_tvs[40] =
    {
        0
    };
    static char   cText_mother_card_cardtype[40] =
    {
        0
    };
    static char   cText_mother_card_lastfeedtime[40] =
    {
        0
    };
    static char   cText_mother_card_feedperiod[40] =
    {
        0
    };
    static char   cText_mother_card_mothercardid[40] =
    {
        0
    };
    com_time_t  Time_t          =
    {
        0
    };


	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	switch(ID)
	{
		case CD_MOTHER_CHILD_TVS_ID:
			sprintf( cText_mother_card_tvs, "%d", g_MotherCardInfo[pos].m_OperaID );
			return cText_mother_card_tvs;
		case CD_MOTHER_CHILD_CARD_TYPE_ID:
			if (TRUE == g_MotherCardInfo[pos].m_IsChildCard)
			{
				if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
					sprintf( cText_mother_card_cardtype, "%s", "子卡" );
				else
				    sprintf( cText_mother_card_cardtype, "%s", "Child Card" );
			}
			else
			{
				if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
					sprintf( cText_mother_card_cardtype, "%s", "母卡" );
				else
				    sprintf( cText_mother_card_cardtype, "%s", "Parent Card" );						
			}
			return cText_mother_card_cardtype;
		case CD_MOTHER_CHILD_LAST_FEED_TIME_ID:
			if (TRUE == g_MotherCardInfo[pos].m_IsChildCard)
			{
				unsigned int nTFTime = g_MotherCardInfo[pos].m_LastFeedTime;
				if(0 != nTFTime)
				{
					gx_ca_calculate_time(nTFTime, &Time_t);
					//app_epg_convert_to_utc(nTFTime, &Time_t);
					sprintf( cText_mother_card_lastfeedtime, "%04d-%02d-%02d %02d:%02d", Time_t.year,
																	  Time_t.month,
																	  Time_t.day,
																	  Time_t.hour,
																	  Time_t.minute);
				}
				else
				{
					if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
						sprintf( cText_mother_card_lastfeedtime, "%s", "还未被喂养" );
					else
						sprintf( cText_mother_card_lastfeedtime, "%s", "Not Feeded Yet" );
				}					
			}
			else
				{
					cText_mother_card_lastfeedtime[0] = '\0';
				}
			return cText_mother_card_lastfeedtime;
		case CD_MOTHER_CHILD_FEED_PERIOD_ID:
			if (TRUE == g_MotherCardInfo[pos].m_IsChildCard)
			{
				if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
					sprintf( cText_mother_card_feedperiod, "%d%s", g_MotherCardInfo[pos].m_DelayTime,"小时");
				else
					sprintf( cText_mother_card_feedperiod, "%d%s", g_MotherCardInfo[pos].m_DelayTime,"H");			
			}
			else
			{
				cText_mother_card_feedperiod[0] = '\0';
			}
			return cText_mother_card_feedperiod;					
		case CD_MOTHER_CHILD_FEED_MOTHER_CARD_ID:
			if (TRUE == g_MotherCardInfo[pos].m_IsChildCard)
			{
				sprintf( cText_mother_card_mothercardid, "%s", g_MotherCardInfo[pos].m_ParentCardSN);
			}
			else
			{
				cText_mother_card_mothercardid[0] = '\0';
			}				
			return cText_mother_card_mothercardid;
		default:
			break;
	}
	return NULL;

}

uint8_t app_cd_cas30_api_get_mother_child_count(ca_get_count_t *data)
{
	unsigned char  wTVSCount = 0;
    unsigned short wTVSID[CDCA_MAXNUM_OPERATOR] = {0};
    unsigned char  cIndex = 0;

    unsigned short wRet = CDCA_RC_OK;

	if (NULL == data)
		return 0;


    wTVSCount =0;

	wRet = CDCASTB_GetOperatorIds( wTVSID );
	if( wRet == CDCA_RC_OK )
    {
        for( cIndex = 0; cIndex < CDCA_MAXNUM_OPERATOR; cIndex++ )
        {
            if( 0 != wTVSID[cIndex] )
            {
            	wTVSCount++;
            }
        }
    }

	data->totalnum = wTVSCount;
    return wTVSCount;
}

char* app_cd_cas30_api_feed_mother_child_card(ca_mother_child_card_t *data)
{
	uint32_t pos ;
	uint32_t  state; /*0: init 1: read feed data 2: write feed data */
	static char* str = NULL;
	
	unsigned short				 wRet				 = CDCA_RC_OK;
	uint32_t				 Retvalue			 = 0;


	if (NULL == data)
		return NULL;
	pos = data->pos;
	state = data->state;
	switch(state)
	{
		case 0: // init 
			memset(chSpbyFeedData,0,sizeof(chSpbyFeedData));
			Len = 255;
			 if(TRUE == g_MotherCardInfo[pos].m_IsChildCard)//\u017e每\u0161是子縗u0161
			 {
				 CDCASTB_GetOperatorChildStatus( g_MotherCardInfo[pos].m_OperaID,
									  &g_MotherCardInfo[pos].m_IsChildCard,
									  &g_MotherCardInfo[pos].m_DelayTime,
									  &g_MotherCardInfo[pos].m_LastFeedTime,
									  (char *)(&g_MotherCardInfo[pos].m_ParentCardSN),
									  &g_MotherCardInfo[pos].m_BIsCanFeed);
			
				 if(g_MotherCardInfo[pos].m_BIsCanFeed == FALSE)
				 {
				 	str = "It is not time to feeding";
				 	return str;
				 }
				 else
				 {
					 //请插入腬u017e縗u0161
					 str = "Please insert the parent-card";
					 return str;
				 }
			 }
			 else
			 {
				 wRet = CDCASTB_ReadFeedDataFromParent( g_MotherCardInfo[pos].m_OperaID,
														  chSpbyFeedData, &Len );
				 if( CDCA_RC_OK != wRet )
				 {
				 	str = "Invalid parent-card";
				 	return str;	
				 }
				 else
				 {
					 //请插入子縗u0161
					str = "Please insert the child-card";
				 	return str;	
				 }
			 }				
			break;
		case 1: // read feed data
			//请插入腬u017e縗u0161
		
			wRet = CDCASTB_ReadFeedDataFromParent( g_MotherCardInfo[pos].m_OperaID,
													 chSpbyFeedData, &Len );
			if( CDCA_RC_OK != wRet )
			{
			
		 		str = "Invalid parent-card";
				return str;				
			}
			else
			{
				//请插入子縗u0161
				str = "Please insert the child-card";
				return str;	
			}
			break;
		case 2:			
			Retvalue = CDCASTB_WriteFeedDataToChild( g_MotherCardInfo[pos].m_OperaID,chSpbyFeedData, Len );
			if( CDCA_RC_OK == Retvalue )
			{
				str = "Feeding successfully";
				return str;					
			}
			else if (CDCA_RC_FEEDTIME_NOT_ARRIVE == Retvalue)
			{
				str = "It is not time to feeding";
				return str; 				
			}
			else
			{
				str = "Feeding failed";
				return str;					
			}
			break;
		default:
			break;
	}
	return NULL;
}









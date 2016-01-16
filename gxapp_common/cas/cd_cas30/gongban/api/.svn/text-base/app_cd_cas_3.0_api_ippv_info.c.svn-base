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
#include "app_cd_cas_3.0_api_ippv_info.h"
#include "app_common_epg.h"


typedef struct CA_IPPV_ORDER_S
{
    unsigned short                 m_OperaID;
    unsigned char                  m_SlotID;
    SCDCAIppvInfo       m_IppvOrderInfo;
}CA_IPPV_ORDER_T;


CA_IPPV_ORDER_T     IppvListInfo[CDCA_MAXNUM_IPPVP] =
{
    {0}
};
SCDCAOperatorInfo   OperatorInfo_t[CDCA_MAXNUM_OPERATOR];

unsigned int                 g_nIppvOrderCount               = 0;


int32_t app_cd_cas30_api_init_ippv_info_data( void )
{
    unsigned short             wRet                            = CDCA_RC_OK;
    unsigned char              cIndex                          = 0;
    unsigned short             wIppvNum                        = CDCA_MAXNUM_IPPVP;
    unsigned short             wIppvIndex                      = 0;
    int             i                               = 0;
	int             j                               = 0;
    SCDCAIppvInfo   Ippv_t[CDCA_MAXNUM_IPPVP]       =
    {
        {0}
    };
    unsigned short              wTVSID[CDCA_MAXNUM_OPERATOR]    =
    {
        0
    };

	SCDCATVSSlotInfo s_IppvSlotInfo = {0,};

    memset( wTVSID, 0, CDCA_MAXNUM_OPERATOR * sizeof( unsigned short ) );
    memset( Ippv_t, 0, CDCA_MAXNUM_IPPVP * sizeof( SCDCAIppvInfo ) );

    g_nIppvOrderCount = 0;
    wRet = CDCASTB_GetOperatorIds( wTVSID );
    if( wRet == CDCA_RC_OK )
    {
        for( cIndex = 0; cIndex < CDCA_MAXNUM_OPERATOR; cIndex++ )
        {
            if( 0 != wTVSID[cIndex] )
            {
                wIppvNum                        = CDCA_MAXNUM_IPPVP;

                wRet = CDCASTB_GetIPPVProgram( wTVSID[cIndex], Ippv_t,
                                                &wIppvNum );
                if( wRet == CDCA_RC_OK )
                {
                    unsigned int Currency;
					j = 0;

                    for( i = 0; i < wIppvNum; i++ )
                    {
				wRet = CDCASTB_GetSlotInfo( wTVSID[cIndex],Ippv_t[i].m_bySlotID,&s_IppvSlotInfo);
				Currency = s_IppvSlotInfo.m_wCreditLimit - s_IppvSlotInfo.m_wBalance -Ippv_t[i].m_wPrice;
				if(wRet!=0 ||  Currency< 0)
				{
					continue;
				}
				else
				{
					memcpy( &IppvListInfo[wIppvIndex + j].m_IppvOrderInfo,	&Ippv_t[i],sizeof( SCDCAIppvInfo));
					IppvListInfo[wIppvIndex + j].m_OperaID = wTVSID[cIndex];
					j++;
				}
                    }
                    wIppvIndex += j;
                }
            }
        }
    }
    g_nIppvOrderCount = wIppvIndex;
    return g_nIppvOrderCount;
}                      

char * app_cd_cas30_api_get_ippv_info_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0;  
	char* osd_language=app_flash_get_config_osd_language();;
	
    static char   cText_order_oper[40] =
    {
        0
    };
    static char   cText_order_product[40] =
    {
        0
    };
    static char   cText_order_slot[40] =
    {
        0
    };
    static char   cText_order_booked[40] =
    {
        0
    };
    static char   cText_order_price[40] =
    {
        0
    };
    static char   cText_order_tapeable[40] =
    {
        0
    };
    static char   cText_order_expired[40] =
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
			case CD_IPPV_INFO_TVS_ID:
				sprintf( cText_order_oper, "%u", IppvListInfo[pos].m_OperaID );
				return cText_order_oper;
			case CD_IPPV_INFO_PROGRAM_ID:
				sprintf( cText_order_product, "%u",
						 (unsigned int)IppvListInfo[pos].m_IppvOrderInfo.m_dwProductID );
				return cText_order_product;
			case CD_IPPV_INFO_SLOT_ID:
				
				sprintf( cText_order_slot, "%d",
						  IppvListInfo[pos].m_IppvOrderInfo.m_bySlotID );
				return cText_order_slot;
			case CD_IPPV_INFO_STATUS_ID:
				switch(IppvListInfo[pos].m_IppvOrderInfo.m_byBookEdFlag)
				{
					case 0:
						if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
							sprintf( cText_order_booked, "%s","未知" );
						else
							sprintf( cText_order_booked, "%s","Unknown" );
						return cText_order_booked;
					case 1:
						if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
							sprintf( cText_order_booked, "%s","预定" );
						else
							sprintf( cText_order_booked, "%s","Booking" );							
						return cText_order_booked;
					case 2:
						if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
							sprintf( cText_order_booked, "%s","过期" );
						else
						   sprintf( cText_order_booked, "%s","Expired" );						   
						   return cText_order_booked;
					case 3:
						if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
							sprintf( cText_order_booked, "%s","已看" );
						else
						   sprintf( cText_order_booked, "%s","Viewed" );							
						   return cText_order_booked;
					default:
						break;
				}
				break;
			case CD_IPPV_INFO_PRICE_ID:
				sprintf( cText_order_price, "%u", IppvListInfo[pos].m_IppvOrderInfo.m_wPrice );
				return cText_order_price;
			case CD_IPPV_INFO_TAPEABLE_ID:
				switch(IppvListInfo[pos].m_IppvOrderInfo.m_bCanTape)
				{
					case 0:		
						if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
							sprintf(cText_order_tapeable, "%s", "不可以");
						else
							sprintf(cText_order_tapeable, "%s", "NO");
						return cText_order_tapeable;
					case 1:					
						if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
							sprintf(cText_order_tapeable, "%s", "可以");
						else
							sprintf(cText_order_tapeable, "%s", "YES");
						return cText_order_tapeable;
					default:
						break;
				}
				break;
			case CD_IPPV_INFO_ENDTIME_ID:
				app_epg_convert_to_utc( IppvListInfo[pos].m_IppvOrderInfo.m_wExpiredDate,
									  &Time_t );
				sprintf( cText_order_expired, "%04d-%02d-%02d", Time_t.year, Time_t.month,
						 Time_t.day );
				return cText_order_expired;
			default:
				break;
		}
	return NULL;
}

uint8_t app_cd_cas30_api_get_ippv_info_count(ca_get_count_t *data)
{
	if (NULL == data)
		return 0;

	data->totalnum = g_nIppvOrderCount;
    return g_nIppvOrderCount;
}






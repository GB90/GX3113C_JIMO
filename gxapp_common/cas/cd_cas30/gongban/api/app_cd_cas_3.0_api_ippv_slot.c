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
#include "app_cd_cas_3.0_api_ippv_slot.h"


typedef struct CA_IPPV_SLOT_S
{
    unsigned short                 m_OperaID;
    unsigned char                  m_SlotID;
    SCDCATVSSlotInfo    m_IppvSlotInfo;
}CA_IPPV_SLOT_T;

CA_IPPV_SLOT_T      IppvSlotInfo_t[CDCA_MAXNUM_SLOT]    =
{
    {0}
};
unsigned char                  g_chSlotCount                       = 7;



int32_t app_cd_cas30_api_init_ippv_slot_data( void )
{
    unsigned short wRet                            = CDCA_RC_OK;
    unsigned char  cIndex                          = 0;
    unsigned char  cSlotIndex                      = 0;
    unsigned char  chTotalSlotIndex                = 0;
    unsigned char  chSlotIDs[CDCA_MAXNUM_SLOT]     =
    {
        0
    };
    unsigned short wTVSID[CDCA_MAXNUM_OPERATOR]    =
    {
        0
    };

    memset( chSlotIDs, 0, CDCA_MAXNUM_SLOT );
    memset( wTVSID, 0, CDCA_MAXNUM_OPERATOR * sizeof( unsigned short ) );

    g_chSlotCount = 0;
    wRet = CDCASTB_GetOperatorIds( wTVSID );
    if( wRet == CDCA_RC_OK )
    {
        for( cIndex = 0; cIndex < CDCA_MAXNUM_OPERATOR; cIndex++ )
        {
            if( 0 != wTVSID[cIndex] )
            {
                wRet = CDCASTB_GetSlotIDs( wTVSID[cIndex], chSlotIDs );
                if( wRet == CDCA_RC_OK )
                {
                    for( cSlotIndex = 0;
                         cSlotIndex < CDCA_MAXNUM_SLOT;
                         cSlotIndex++ )
                    {
                        if( 0 != chSlotIDs[cSlotIndex] )
                        {
                            wRet = CDCASTB_GetSlotInfo( wTVSID[cIndex],
                                                         chSlotIDs[cSlotIndex],
                                                         &IppvSlotInfo_t[chTotalSlotIndex].m_IppvSlotInfo );
                            IppvSlotInfo_t[chTotalSlotIndex].m_OperaID = wTVSID[cIndex];
                            IppvSlotInfo_t[chTotalSlotIndex].m_SlotID = chSlotIDs[cSlotIndex];
                            chTotalSlotIndex++;
                        }
                    }
                }
            }
        }
    }
    g_chSlotCount = chTotalSlotIndex;
    return 0;
}


char * app_cd_cas30_api_get_ippv_slot_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0;  
	
    static char   cText_ippv_oper[40] =
    {
        0
    };
    static char   cText_ippv_slot[40] =
    {
        0
    };
    static char   cText_ippv_CreditLimit[40] =
    {
        0
    };
    static char   cText_ippv_Balance[40] =
    {
        0
    };

	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	switch(ID)
		{
			case CD_IPPV_SLOT_TVS_ID:
				sprintf(cText_ippv_oper,"%d",IppvSlotInfo_t[pos].m_OperaID);
				return cText_ippv_oper;
			case CD_IPPV_SLOT_SLOT_ID:
				sprintf( cText_ippv_slot, "%d",IppvSlotInfo_t[pos].m_SlotID );
				return cText_ippv_slot;
			case CD_IPPV_SLOT_RESETPOINT_ID:
				sprintf( cText_ippv_CreditLimit, "%u",
						 ( unsigned int ) IppvSlotInfo_t[pos].m_IppvSlotInfo.m_wCreditLimit );
				    return cText_ippv_CreditLimit;
			case CD_IPPV_SLOT_USED_ID:
				sprintf( cText_ippv_Balance, "%u",
						 ( unsigned int ) IppvSlotInfo_t[pos].m_IppvSlotInfo.m_wBalance );
				return cText_ippv_Balance;
			default:
				break;
		}
	return NULL;

}

uint8_t app_cd_cas30_api_get_ippv_slot_count(ca_get_count_t *data)
{
	if (NULL == data)
		return 0;

	data->totalnum = g_chSlotCount;
    return g_chSlotCount;
}






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
#include "app_cd_cas_3.0_api_baseinfo.h"

uint16_t				 wArrTvsID[CDCA_MAXNUM_OPERATOR] =
{
	0
};
/*SCDCAOperatorInfo	OperatorInfo					=
{
	{0}
};*/

int32_t app_cd_cas30_api_init_baseinfo_data(void)
{
	uint32_t i = 0;
	for(i=0;i<CDCA_MAXNUM_OPERATOR;i++)
		{
			wArrTvsID[i] = 0;
		}
	
	CDCASTB_GetOperatorIds( wArrTvsID );
	return 0;

}

char * app_cd_cas30_api_get_baseinfo_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0;  

    static char                  s_text_buffer[70]={0};

    uint32_t                 wRet                            = CDCA_RC_OK;
    uint8_t                  cStartHour                      = 0;
    uint8_t                  cEndHour                        = 0;
    uint8_t                  cStartMin                       = 0;
    uint8_t                  cEndMin                         = 0;
    uint8_t                  cStartSec                       = 0;
    uint8_t                  cEndSec                         = 0;
    uint8_t                  cRating                         = 0;
    CDCA_U8                  chStbNum                        = 0;
    CDCA_U8                   chACList[30]                    =
    {
        0
    };
/*    char                  chCardSN[CDCA_MAXLEN_SN + 1]    =
    {
        0
    };*/
    uint32_t                 i;
   CDCA_U16 pwPlatformID =0;
   CDCA_U32 pdwUniqueID =0;

	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	memset(s_text_buffer,0,sizeof(s_text_buffer));

	switch(ID)
	{
		case CD_BASEINFO_OPERATE1_ID:
		case CD_BASEINFO_OPERATE2_ID:
		case CD_BASEINFO_OPERATE3_ID:
		case CD_BASEINFO_OPERATE4_ID:
			memset(wArrTvsID,0,sizeof(uint16_t)*CDCA_MAXNUM_OPERATOR);
			CDCASTB_GetOperatorIds( wArrTvsID );
			i = ID;
		        if( 0 != wArrTvsID[i] )
		        {
		            //wRet = CDCASTB_GetOperatorInfo( wArrTvsID[i], &OperatorInfo );
		            sprintf( s_text_buffer, "%d", wArrTvsID[i] );
				return s_text_buffer;
		        }
			break;

		case CD_BASEINFO_PARIED_ID:
		    wRet = CDCASTB_IsPaired( &chStbNum, &chACList[0] );
		    printf("CDCASTB_IsPaired chStbNum=%d\n",chStbNum);
		    

		    for( i = 0; i < chStbNum; i++ )
		    {
		        printf( "0x%x", chACList[6 * i] );
		        printf( "%x", chACList[6 * i + 1] );
		        printf( "%x", chACList[6 * i + 2] );
		        printf( "%x", chACList[6 * i + 3] );
		        printf( "%x", chACList[6 * i + 4] );
		        printf( "%x\n", chACList[6 * i + 5] );
		    }
			if( wRet == CDCA_RC_OK )
		    {
		        sprintf( s_text_buffer,"%s", "Paired Ok" );
			    return s_text_buffer;

		    }
		    else if( wRet == CDCA_RC_CARD_INVALID )
		    {
		        sprintf( s_text_buffer,"%s", "Invalid Smartcard");
			return s_text_buffer;
				
		    }
		    else if( wRet == CDCA_RC_CARD_NOPAIR )
		    {
		        sprintf( s_text_buffer,"%s", "The card is not paired with any STB");
			    return s_text_buffer;
				
		    }
		    else if( wRet == CDCA_RC_CARD_PAIROTHER )
		    {
		        sprintf( s_text_buffer,"%s", "Paired to other STB");
			    return s_text_buffer;

		    }
		    else
		    {
		    		return NULL;
		    }
			break;
	case CD_BASEINFO_WORK_TIME_ID:
		    wRet = CDCASTB_GetWorkTime( &cStartHour, &cStartMin, &cStartSec, &cEndHour,
		                                 &cEndMin, &cEndSec );
		    if( wRet == CDCA_RC_OK )
		    {
		        sprintf( s_text_buffer, "%02d:%02d:%02d-%02d:%02d:%02d",
		                            cStartHour,
		                            cStartMin,
		                            cStartSec,
		                            cEndHour,
		                            cEndMin ,
		                            cEndSec);
			  return s_text_buffer;
		    }
		    else
		    {
		    		return NULL;			    
		    }

		break;
	case CD_BASEINFO_RATING_ID:
		    wRet = CDCASTB_GetRating( &cRating );
		    if( wRet == CDCA_RC_OK )
		    {
		        sprintf( s_text_buffer, "%d", cRating );
			 return s_text_buffer;
		    }
		    else
		    {
		    	return NULL;
		    }			
		break;
	case CD_BASEINFO_CARD_SN_ID:
		    wRet = CDCASTB_GetCardSN( s_text_buffer );
		    if( wRet == CDCA_RC_OK )
		    {
		    	 return s_text_buffer;
			 
		    }
		    else
		    {
		    	return NULL;
		    }
		break;
	case CD_BASEINFO_VERSION_ID:
		    wRet = CDCASTB_GetVer();
		    sprintf( s_text_buffer, "0x%08X", (unsigned int)wRet );//未知
		    return s_text_buffer;
		break;
	case CD_BASEINFO_STB_ID_ID:
		  CDSTBCA_GetSTBID( &pwPlatformID,
		                              &pdwUniqueID);
		    sprintf( s_text_buffer, "0x%04x%08X", pwPlatformID,(unsigned int)pdwUniqueID );//未知
		   return s_text_buffer;
		break;
		default:
			break;
	}
	return NULL;

}







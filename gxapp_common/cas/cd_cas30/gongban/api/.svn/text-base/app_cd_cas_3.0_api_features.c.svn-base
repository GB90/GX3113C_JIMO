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
#include "app_cd_cas_3.0_api_features.h"

static unsigned short wTVSID[CDCA_MAXNUM_OPERATOR] = {0};
static unsigned char  wTVSCount = 0;
unsigned int pAClist[CDCA_MAXNUM_OPERATOR][CDCA_MAXNUM_ACLIST];


int32_t app_cd_cas30_api_init_features_data( void )
{
	unsigned short wRet = CDCA_RC_OK;
	unsigned char  cIndex = 0;

	wTVSCount = 0;
	wRet = CDCASTB_GetOperatorIds( wTVSID );
	if( wRet == CDCA_RC_OK )
    {
        for( cIndex = 0; cIndex < CDCA_MAXNUM_OPERATOR; cIndex++ )
        {
            if( 0 != wTVSID[cIndex] )
            {
				CDCASTB_GetACList( wTVSID[cIndex] , (CDCA_U32*)&pAClist[cIndex][0] );
				wTVSCount++;
            }
        }
    }
	return wTVSCount;
}


char * app_cd_cas30_api_get_features_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0;  
	
    static char   cText_features_tvs[40] =
    {
        0
    };
    static char   cText_features_area[40] =
    {
        0
    };
    static char   cText_features_bouquet[40] =
    {
        0
    };
    static char   cText_features_id[40] =
    {
        0
    };

	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	switch(ID)
		{
			case CD_FEATURES_TVS_ID:
				sprintf( cText_features_tvs,"%d", wTVSID[pos] );
//				printf("cText_features_tvs=%s\n",cText_features_tvs);
				return cText_features_tvs;
			case CD_FEATURES_AREACODE_ID:
				sprintf( cText_features_area, "%d",  pAClist[pos][0]);
//				printf("cText_features_area=%s\n",cText_features_area);
				return cText_features_area;
			case CD_FEATURES_BOUQUETID_ID:
				sprintf( cText_features_bouquet, "%d",  pAClist[pos][1]);
//				printf("cText_features_bouquet=%s\n",cText_features_bouquet);
				return cText_features_bouquet;
			case CD_FEATURES_ID1_ID:
				if(wTVSCount)
					{
						sprintf( cText_features_id, "0x%08x 0x%08x 0x%08x", pAClist[pos][3],pAClist[pos][4],pAClist[pos][5] );
//						printf("cText_features_id1=%s\n",cText_features_id);
						return cText_features_id;					
					}
				else
					{
						return NULL;
					}
			case CD_FEATURES_ID2_ID:
				if(wTVSCount)
					{
						sprintf( cText_features_id, "0x%08x 0x%08x 0x%08x", pAClist[pos][6],pAClist[pos][7],pAClist[pos][8] );
//						printf("cText_features_id2=%s\n",cText_features_id);
						return cText_features_id;
					}
				else
					{
						return NULL;
					}
			case CD_FEATURES_ID3_ID:
				if(wTVSCount)
					{
						sprintf( cText_features_id, "0x%08x", pAClist[pos][9] );
//						printf("cText_features_id2=%s\n",cText_features_id);
						return cText_features_id;
					}
				else
					{
						return NULL;
					}
			default:
				break;
		}
	return NULL;

}

uint8_t app_cd_cas30_api_get_features_count(ca_get_count_t *data)
{
	if (NULL == data)
		return 0;

	data->totalnum = wTVSCount;
    return wTVSCount;
}






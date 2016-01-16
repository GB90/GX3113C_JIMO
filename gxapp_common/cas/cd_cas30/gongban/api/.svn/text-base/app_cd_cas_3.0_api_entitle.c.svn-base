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
#include "app_cd_cas_3.0_api_entitle.h"
#include "app_common_epg.h"



typedef struct CA_ENTITLE_PROG_S
{
    unsigned short                 m_OperaID;
    SCDCAEntitle        m_EntitleProgInfo;
}CA_ENTITLE_PROG_T;
unsigned short                 g_wAuthorProgNum                    = 0;
CA_ENTITLE_PROG_T   g_EntitleProg[CDCA_MAXNUM_ENTITLE]  =
{
    {0}
};

int32_t app_cd_cas30_api_init_entitle_data(void)
{
    unsigned short             wRet                            = CDCA_RC_OK;
    unsigned char              cIndex                          = 0;
    unsigned short             cAuthorPorgIndex                = 0;
    SCDCAEntitles   ServiceEntitles_t               =
    {
        0
    };
    unsigned short             wTVSID[CDCA_MAXNUM_OPERATOR]    =
    {
        0
    };
    unsigned int             i                               = 0;

    g_wAuthorProgNum = 0;
    wRet = CDCASTB_GetOperatorIds(wTVSID);
    memset( g_EntitleProg, 0, CDCA_MAXNUM_ENTITLE * sizeof( CA_ENTITLE_PROG_T ) );
    if( wRet == CDCA_RC_OK )
    {
        for( cIndex = 0; cIndex < CDCA_MAXNUM_OPERATOR; cIndex++ )
        {
            if( 0 != wTVSID[cIndex] )
            {
                wRet = CDCASTB_GetServiceEntitles( wTVSID[cIndex],
                                                    &ServiceEntitles_t );
                if( ( wRet == CDCA_RC_OK ) || wRet == CDCA_RC_UNKNOWN )
                {
                    for( i = 0; i < ServiceEntitles_t.m_wProductCount; i++ )
                    {
                        memcpy( &g_EntitleProg[cAuthorPorgIndex + i].m_EntitleProgInfo,
                                &ServiceEntitles_t.m_Entitles[i],
                                sizeof( SCDCAEntitle ) );
                        g_EntitleProg[cAuthorPorgIndex + i].m_OperaID = wTVSID[cIndex];
                    }
                    cAuthorPorgIndex += ServiceEntitles_t.m_wProductCount;
                }
            }
        }
    }
    g_wAuthorProgNum = cAuthorPorgIndex;
    return g_wAuthorProgNum;
}


char * app_cd_cas30_api_get_entitle_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0;  
	//char* osd_language=app_flash_get_config_osd_language();;
	
    static char   cTextBuffer_oper[40] ={0};
    static char   cTextBuffer_pro[40] ={0};
    static char   cTextBuffer_exp[40] ={0};
    static char   cTextBuffer_can_type[40] ={0};
    com_time_t  Time_t          ={0};

	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	switch(ID)
	{
		case CD_ENTITLE_OPERATE_ID:
			sprintf(cTextBuffer_oper,"%d",g_EntitleProg[pos].m_OperaID);
			return cTextBuffer_oper;
		case CD_ENTITLE_PRODUCT_ID:
		     sprintf( cTextBuffer_pro, "%u",
		         ( unsigned int ) g_EntitleProg[pos].m_EntitleProgInfo.m_dwProductID );
			return cTextBuffer_pro;
		case CD_ENTITLE_CAN_TYPE_ID:
		    if(g_EntitleProg[pos].m_EntitleProgInfo.m_bCanTape)
		    {
				strcpy(cTextBuffer_can_type,"YES");
			}
			else
			{
				strcpy(cTextBuffer_can_type,"NO");
		    }
		    return cTextBuffer_can_type;
		case CD_ENTITLE_EXPIRT_DATE_ID:
			app_epg_convert_to_utc( g_EntitleProg[pos].m_EntitleProgInfo.m_tExpireDate,&Time_t );
			sprintf( cTextBuffer_exp, "%04d-%02d-%02d", Time_t.year, Time_t.month,Time_t.day );
			return cTextBuffer_exp;
		default:
			break;
	}
	return NULL;

}

uint8_t app_cd_cas30_api_get_entitle_count(ca_get_count_t *data)
{
	data->totalnum = g_wAuthorProgNum;
    return g_wAuthorProgNum;
}


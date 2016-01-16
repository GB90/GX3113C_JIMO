/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_dvt_cas_api_operator.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.12.03		  zhouhm 	 			creation
*****************************************************************************/
#include "DVTCAS_STBInf.h"
#include "app_common_porting_stb_api.h"
#include "app_dvt_cas_api_operator.h"
#include "app_common_epg.h"


BYTE  poperatorCount = 5;
BYTE  pbyCount = 0;
SDVTCATvsInfo pOperatorInfo[5];

int32_t app_dvt_cas_api_init_operator_data(void)
{

	poperatorCount = 5;
	memset(&pOperatorInfo[0],0,sizeof(SDVTCATvsInfo)*5);
	if (DVTCA_OK == DVTCASTB_GetOperatorInfo((WORD)-1, &poperatorCount, &pOperatorInfo[0]))
	{
		pbyCount = poperatorCount;
		return 1;		
	}
	return 0;
}


char * app_dvt_cas_api_get_operator_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0; 

    static char   cTextBuffer_id[22] =
    {
        0
    };
    static char   cTextBuffer_name[22] =
    {
        0
    };


	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;
	
	switch(ID)
		{
			case DVT_OPERATOR_NO_ID:
				memset(cTextBuffer_id,0,sizeof(cTextBuffer_id));
			    sprintf( cTextBuffer_id, "%d",pOperatorInfo[pos].m_wTVSID);
				return cTextBuffer_id;
			case DVT_OPERATOR_NAME_ID:
				memset(cTextBuffer_name,0,sizeof(cTextBuffer_name));
				memcpy(cTextBuffer_name,pOperatorInfo[pos].m_szTVSName,20);
				return cTextBuffer_name;
			default:
				break;
		}
	return NULL;

}

uint8_t app_dvt_cas_api_get_operator_count(ca_get_count_t *data)
{
	printf("%s  totalnum=%d\n",__func__,pbyCount);	
	data->totalnum = pbyCount;
    return pbyCount;
}






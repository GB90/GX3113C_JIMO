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
#include "BYCAca_stb.h"
#include "app_common_porting_stb_api.h"
#include "app_by_cas_api_entitle.h"
#include "app_common_epg.h"


CAMenu_Product g_byca_EntitleProg = {0};
BU8				CaMenu_number = 0;

int32_t app_by_cas_api_init_entitle_data(void)
{

    memset(&g_byca_EntitleProg,0,sizeof(CAMenu_Product));
    if (TRUE == BYCASTB_GetCaMenuProduct(&g_byca_EntitleProg))
    	{
    		printf("app_by_cas_api_init_entitle_data g_byca_EntitleProg.CaMenu_number=%d\n",g_byca_EntitleProg.CaMenu_number);
    		CaMenu_number = g_byca_EntitleProg.CaMenu_number;
    	}
	return 0;

}


char * app_by_cas_api_get_entitle_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0; 
	uint16_t timetmp = 0;
//	com_time_t Time_t = {0};
	uint16_t	 year = 0;
	uint8_t  month=0;
	uint8_t  day=0;
	uint8_t  weekDay=0;


    static char   cTextBuffer_id[8] =
    {
        0
    };
    static char   cTextBuffer_name[CAMENU_PRODUCTNAME_MAXLEN+1] =
    {
        0
    };
    static char   cTextBuffer_starttime[20] =
    {
        0
    };
    static char   cTextBuffer_endtime[20] =
    {
        0
    };


	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	switch(ID)
		{
			case BY_ENTITLE_PRODUCT_ID:
				memset(cTextBuffer_id,0,sizeof(cTextBuffer_id));
			     sprintf( cTextBuffer_id, "%d",g_byca_EntitleProg.CaMenu_Name[pos].Product_ID);
				return cTextBuffer_id;
			case BY_ENTITLE_PRODUCT_NAME_ID:
				memset(cTextBuffer_name,0,sizeof(cTextBuffer_name));
				memcpy(cTextBuffer_name,g_byca_EntitleProg.CaMenu_Name[pos].ProductName,g_byca_EntitleProg.CaMenu_Name[pos].ProductName_Len);
				    return cTextBuffer_name;
			case BY_ENTITLE_PRODUCT_STARTTIME_ID:
				memset(cTextBuffer_starttime,0,sizeof(cTextBuffer_starttime));
				timetmp = (uint16_t)((g_byca_EntitleProg.Camenu_Unit[pos].Purchase_start_time[0]<<8)|g_byca_EntitleProg.Camenu_Unit[pos].Purchase_start_time[1]);
				app_epg_convert_mjd_to_ymd( timetmp, &year,  &month, &day, &weekDay );
				sprintf( cTextBuffer_starttime, "%04d-%02d-%02d", year, month,
            			 day);
				return cTextBuffer_starttime;
			case BY_ENTITLE_PRODUCT_ENDTIME_ID:
				memset(cTextBuffer_endtime,0,sizeof(cTextBuffer_endtime));
				timetmp = (uint16_t)((g_byca_EntitleProg.Camenu_Unit[pos].Purchase_end_time[0]<<8)|g_byca_EntitleProg.Camenu_Unit[pos].Purchase_end_time[1]);
				app_epg_convert_mjd_to_ymd( timetmp, &year,  &month, &day, &weekDay );
				sprintf( cTextBuffer_endtime, "%04d-%02d-%02d", year, month,
            			 day);
				return cTextBuffer_endtime;
			default:
				break;
		}
	return NULL;

}

uint8_t app_by_cas_api_get_entitle_count(ca_get_count_t *data)
{
	printf("app_by_cas_api_get_entitle_count CaMenu_number=%d\n",CaMenu_number);	
	data->totalnum = CaMenu_number;
    return CaMenu_number;
}






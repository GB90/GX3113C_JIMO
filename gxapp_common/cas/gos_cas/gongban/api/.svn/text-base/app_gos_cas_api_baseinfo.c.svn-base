/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_gos_cas_api_baseinfo.c
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.5.16		  chenth 	 			creation
*****************************************************************************/
#include "GOSCAS_STBInf.h"
#include "app_common_porting_stb_api.h"
#include "app_gos_cas_api_baseinfo.h"



char * app_gos_cas_api_get_baseinfo_data(ca_get_date_t *data)
{    
    uint32_t pos = 0 ; 
	uint32_t ID = 0;  
    unsigned char state,softVer;
    unsigned long ver,cardSn;
    
    static char  s_text_buffer[70]={0};

	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	memset(s_text_buffer,0,sizeof(s_text_buffer));

	switch(ID)
	{
	case GOS_BASEINFO_IVS_ID:
		sprintf(s_text_buffer,"%s","GOSPELL");
		return s_text_buffer;
	case GOS_BASEINFO_CAVER_ID:
        GOSCASTB_SCGetState(&state);
        if(state != GOSCAS_SC_IN)
        {
            return NULL;
        }
        if(true == GOSCASTB_GetVer((long *)&ver))
        {
            sprintf(s_text_buffer,"%02X.%02X.%02X.%02X",
            (ver>>24)&&0xff,(ver>>16)&&0xff,(ver>>8)&&0xff,ver&&0xff);
            printf("CA ver:%s\n",s_text_buffer);
            return s_text_buffer;
        }
        return NULL;
    case GOS_BASEINFO_SOFTVER_ID:
        GOSCASTB_SCGetState(&state);
        if(state != GOSCAS_SC_IN)
        {
            return NULL;
        }
        if(true == GOSCASTB_GetCardInfo(&cardSn,&softVer))
        {
            sprintf(s_text_buffer,"%02x.%02x",(softVer>>4)&&0xf,softVer&&0xf);
            printf("Soft Ver:%s\n",s_text_buffer);
            return s_text_buffer;
        }
        return NULL;
    case GOS_BASEINFO_CARDSN_ID:
		GOSCASTB_SCGetState(&state);
        if(state != GOSCAS_SC_IN)
        {
            return NULL;
        }
        if(true == GOSCASTB_GetCardInfo(&cardSn,&softVer))
        {
            sprintf(s_text_buffer,"%ld",cardSn);
            printf("Card SN:%s\n",s_text_buffer);
            return s_text_buffer;
        }
        return NULL;
	default:
		break;
	}
	return NULL;

}



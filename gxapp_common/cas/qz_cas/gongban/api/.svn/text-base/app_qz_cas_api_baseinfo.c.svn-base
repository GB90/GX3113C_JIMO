/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_qz_cas_api_baseinfo.c
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.5.16		  chenth 	 			creation
*****************************************************************************/

#include "app_common_porting_stb_api.h"
#include "app_qz_cas_api_baseinfo.h"
#include "app_qz_cas_api_smart.h"
#include "ca_process.h"

extern SMARTCARD_INFO card;

char * app_qz_cas_api_get_baseinfo_data(ca_get_date_t *data)
{    
    uint32_t pos = 0 ; 
	uint32_t ID = 0;  

    static char  s_text_buffer[70]={0};

	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	memset(s_text_buffer,0,sizeof(s_text_buffer));

	switch(ID)
	{
	case QZ_BASEINFO_IVS_ID:
		sprintf(s_text_buffer,"%s","Chengdu Cable-GS");
		return s_text_buffer;
	case QZ_BASEINFO_CARDSN_ID:
        if(Ca_Card_Ready() != ERR_OK)
    	{
            printf("Card not Ready.\n");
    		return NULL;
    	}

        sprintf(s_text_buffer,"%d",
            (card.smartcard_id[0]<<16)+(card.smartcard_id[1]<<8)+card.smartcard_id[2]);
        
        printf("CA CardSN:%s\n",s_text_buffer);
		return s_text_buffer;
    case QZ_BASEINFO_AREACODE_ID:
		if(Ca_Card_Ready() != ERR_OK)
    	{
    		return NULL;
    	}
        sprintf(s_text_buffer,"%02X%02X",
            card.area_code[2],card.area_code[3]);
        
        printf("CA AreaCode:%s\n",s_text_buffer);
		return s_text_buffer;
    case QZ_BASEINFO_EXPIRES_ID:
        {
            byte expire_date[4];
    		if(Ca_Card_Ready() != ERR_OK)
        	{
        		return NULL;
        	}
            
            if(ERR_OK == get_expire_date(expire_date))
            {
                sprintf(s_text_buffer,"%04d.%02d.%02d",(expire_date[0]<<8)|expire_date[1],
    					expire_date[2],expire_date[3]);
                printf("Expire Time:%s\n",s_text_buffer);
        		return s_text_buffer; 
            }
            else
            {
                return NULL;
            }
        }

	default:
		break;
	}
	return NULL;

}



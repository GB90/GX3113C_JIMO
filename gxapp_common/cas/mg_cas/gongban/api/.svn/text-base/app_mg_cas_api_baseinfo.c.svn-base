/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_mg_cas_api_baseinfo.c
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.4.14		  chenth 	 			creation
*****************************************************************************/
#include "mgdef.h"
#include "app_common_porting_stb_api.h"
#include "app_mg_cas_api_baseinfo.h"

int32_t app_mg_cas_api_init_baseinfo_data(void)
{
    if(!MG_Get_Card_Ready())
	{
        printf("MG Card not Ready.\n");
		return -1;
	}
	return 0;
}
char * app_mg_cas_api_get_baseinfo_data(ca_get_date_t *data)
{    
    uint32_t pos = 0 ; 
	uint32_t ID = 0;  

	uint8_t ptemp[32]={0};
    uint16_t tmpU16;
    static char  s_text_buffer[70]={0};

	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	memset(s_text_buffer,0,sizeof(s_text_buffer));

	switch(ID)
	{
	case MG_BASEINFO_IVS_ID:
		sprintf(s_text_buffer,"%s","ShangHai SANZhOUXUNCHI");
		return s_text_buffer;
    case MG_BASEINFO_VER_ID:
        tmpU16 = MG_Get_CASoft_Version();
        sprintf(s_text_buffer,"v%X.%X.%X",(uint8_t)(tmpU16>>8),(uint8_t)((tmpU16&0x00F0)>>4),(uint8_t)(tmpU16&0x000F));
        printf("CA¡¡Version:%s\n",s_text_buffer);
		return s_text_buffer;
        
	case MG_BASEINFO_CARDSN_ID:
        if(!MG_Get_Card_Ready())
    	{
            printf("MG Card not Ready.\n");
    		return NULL;
    	}

    	MG_Get_Card_SN(ptemp);
        sprintf(s_text_buffer,"%02X%02X%02X%02X%02X%02X%02X%02X",
            ptemp[0],ptemp[1],ptemp[2],ptemp[3],ptemp[4],ptemp[5],ptemp[6],ptemp[7]);
        printf("CA CardSN:%s\n",s_text_buffer);
		return s_text_buffer;
        
	case MG_BASEINFO_STATE_ID:
		if(!MG_Get_Card_Ready())
    	{
    		return NULL;
    	}
        sprintf(s_text_buffer,"%s",
            MG_Get_Partner_Status()?"Working":"unWorking");
		return s_text_buffer;
    case MG_BASEINFO_AREACODE_ID:
		if(!MG_Get_Card_Ready())
    	{
    		return NULL;
    	}
        sprintf(s_text_buffer,"%0d",MG_Get_ZoneCode());
		return s_text_buffer; 
    case MG_BASEINFO_CTRLCODE_ID:
		if(!MG_Get_Card_Ready())
    	{
    		return NULL;
    	}
        sprintf(s_text_buffer,"%0d",MG_Get_CtrlCode());
		return s_text_buffer;     
    case MG_BASEINFO_TOTAL_ID:
		if(!MG_Get_Card_Ready())
    	{
    		return NULL;
    	}
        sprintf(s_text_buffer,"%d",MG_Get_Card_Quota());
		return s_text_buffer;   
    case MG_BASEINFO_CONSUME_ID:
		if(!MG_Get_Card_Ready())
    	{
    		return NULL;
    	}
        sprintf(s_text_buffer,"%d",MG_Get_Card_Consume());
		return s_text_buffer;   
    case MG_BASEINFO_EXPIRES_ID:
		if(!MG_Get_Card_Ready())
    	{
    		return NULL;
    	}
        sprintf(s_text_buffer,"%d",MG_Get_RecentExpireDay());
		return s_text_buffer; 
    case MG_BASEINFO_SUBMAINCARD_TYPE_ID:
        {
            MG_U8 MainCard;
    	    MG_U8 MCSN4[4];
    	    MG_U16 SCLeftD;
            if(!MG_Get_Card_Ready())
        	{
        		return NULL;
        	}
            MainCard = MG_Get_MSCard_Ext(MCSN4,(MG_U8 *)&SCLeftD);
            sprintf(s_text_buffer,"%s",MainCard?"MainCard":"SubCard");
            printf("Card:%s\n",s_text_buffer);
            return s_text_buffer; 
        }
        break;
    case MG_BASEINFO_SUBMAINCARD_MAINID_ID:
        {
            MG_U8 MainCard;
    	    MG_U8 MCSN4[4];
    	    MG_U16 SCLeftD;
            if(!MG_Get_Card_Ready())
        	{
        		return NULL;
        	}
            MainCard = MG_Get_MSCard_Ext(MCSN4,(MG_U8 *)&SCLeftD);
            if(MainCard)
                sprintf(s_text_buffer,"00000000");
            else
                sprintf(s_text_buffer,"%02X%02X%02X%02X",MCSN4[0],MCSN4[1],MCSN4[2],MCSN4[3]);
            printf("CA MainID:%s\n",s_text_buffer);
    		return s_text_buffer; 
        }
        break;
    case MG_BASEINFO_SUBMAINCARD_TIME_ID:
        {
            MG_U8 MainCard;
    	    MG_U8 MCSN4[4];
    	    MG_U16 SCLeftD;
            if(!MG_Get_Card_Ready())
        	{
        		return NULL;
        	}
            MainCard = MG_Get_MSCard_Ext(MCSN4,(MG_U8 *)&SCLeftD);
            if(MainCard)
                sprintf(s_text_buffer,"0");
            else
                sprintf(s_text_buffer,"%d",SCLeftD);
    		return s_text_buffer; 
        }
        break;
    case MG_BASEINFO_FEATURESCODE_V1_ID:
        {
            MG_U8 tmpArrU8[32];
            MG_U8 *p = NULL;
    	    if(!MG_Get_Card_Ready())
        	{
        		return NULL;
        	}
    	
    	    MG_Get_EigenValue(tmpArrU8);
            p = &tmpArrU8[0];
            sprintf(s_text_buffer,"%02X%02X %02X%02X %02X%02X %02X%02X %02X%02X %02X%02X %02X%02X %02X%02X",
                p[0],p[1],p[2],p[3],
                p[4],p[5],p[6],p[7],
                p[8],p[9],p[10],p[11],
                p[12],p[13],p[14],p[15]);
            printf("CA V1:%s\n",s_text_buffer);
            return s_text_buffer; 
        }
        break;
    case MG_BASEINFO_FEATURESCODE_V2_ID:
        {
            MG_U8 tmpArrU8[32];
            MG_U8 *p = NULL;
    	    if(!MG_Get_Card_Ready())
        	{
        		return NULL;
        	}
    	
    	    MG_Get_EigenValue(tmpArrU8);
            p = &tmpArrU8[16];
            sprintf(s_text_buffer,"%02X%02X %02X%02X %02X%02X %02X%02X %02X%02X %02X%02X %02X%02X %02X%02X",
                p[0],p[1],p[2],p[3],
                p[4],p[5],p[6],p[7],
                p[8],p[9],p[10],p[11],
                p[12],p[13],p[14],p[15]);
            printf("CA V2:%s\n",s_text_buffer);
            return s_text_buffer; 
        }
        break;
	default:
		break;
	}
	return NULL;

}



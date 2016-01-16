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
2.0  2014.12.17              wangjian         modify for MGCAS3.1.2
*****************************************************************************/
#include "mg312def.h"
#include "app_common_porting_stb_api.h"
#include "app_mg_cas312_api_baseinfo.h"

int32_t app_mg_cas312_api_init_baseinfo_data(void)
{
    if(!MG_Get_Card_Ready())
	{
        printf("[app_mg_cas312_api_init_baseinfo_data]MG Card not Ready!!!\n");
		return -1;
	}
	
	return 0;
}

char * app_mg_cas312_api_get_baseinfo_data(ca_get_date_t *data)
{    
    uint32_t pos = 0 ; 
	uint32_t ID = 0;  

	uint8_t ptemp[32]={0};
    uint16_t tmpU16;
	int32_t tmpS32 = 0;
    static char  s_text_buffer[70]={0};

	if (NULL == data)
	{
		return NULL;
	}

	pos = data->pos;
	ID = data->ID;

	memset(s_text_buffer,0,sizeof(s_text_buffer));

	switch(ID)
	{
	case MG312_BASEINFO_IVS_ID:
		sprintf(s_text_buffer,"%s","ShangHai SANZhOUXUNCHI");
		return s_text_buffer;
		
    case MG312_BASEINFO_VER_ID:
        tmpU16 = MG_Get_CASoft_Version();
        sprintf(s_text_buffer,"v%X.%X.%X",(uint8_t)(tmpU16>>8),(uint8_t)((tmpU16&0x00F0)>>4),(uint8_t)(tmpU16&0x000F));
        printf("CA¡¡Version:%s\n",s_text_buffer);
		return s_text_buffer;
        
	case MG312_BASEINFO_CARDSN_ID:
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
        
	case MG312_BASEINFO_CARDVER_ID:
		if(!MG_Get_Card_Ready())
    	{
    		return NULL;
    	}

		ptemp[0] = MG_Get_Card_Version();
		sprintf(s_text_buffer, "v%02X", ptemp[0]);
		printf("CA CardVer:%s.\n", s_text_buffer);
		return s_text_buffer;

	case MG312_BASEINFO_BALANCE_ID:
		if(!MG_Get_Card_Ready())
    	{
    		return NULL;
    	}

		tmpS32 = MG_Get_Card_Balance();/*this value maybe minus.*/
		printf("MG_Get_Card_Balance is %d.\n", tmpS32);
		sprintf(s_text_buffer, "%d.%02d", tmpS32/10000, abs((tmpS32%10000)/100));
		return s_text_buffer;	

	case MG312_BASEINFO_OVERDRAFT_ID:
		if(!MG_Get_Card_Ready())
    	{
    		return NULL;
    	}

		tmpU16 = MG_Get_Card_OverDraft();
		printf("MG_Get_Card_OverDraft is %d.\n", tmpU16);
		sprintf(s_text_buffer, "%d", tmpU16);
		return s_text_buffer;		
		
    case MG312_BASEINFO_AREACODE_ID:
		if(!MG_Get_Card_Ready())
    	{
    		return NULL;
    	}
		
        sprintf(s_text_buffer, "%d", MG_Get_ZoneCode());
		return s_text_buffer;  

	case MG312_BASEINFO_PARTNER_ID:
		if(!MG_Get_Card_Ready())
    	{
    		return NULL;
    	}
		
        sprintf(s_text_buffer,"%s",
            MG_Get_Partner_Status()?"Working":"unWorking");
		return s_text_buffer;  	

	case MG312_BASEINFO_FEATURESCODE_V1_ID:
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
		
    case MG312_BASEINFO_FEATURESCODE_V2_ID:
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

	case MG312_BASEINFO_SUBMAINCARD_TYPE_ID:
		if(!MG_Get_Card_Ready())
    	{
    		return NULL;
    	}

		MG_Get_MSCard_Info(ptemp);
		if (1 == (ptemp[14] & 0x01))
		{
			sprintf(s_text_buffer, "MainCard");
		}
		else if (0 == (ptemp[14] & 0x01))
		{
			sprintf(s_text_buffer, "SubCard");
		}
		
        printf("CA card type:%s.\n",s_text_buffer);
		return s_text_buffer;

	case MG312_BASEINFO_SUBMAINCARD_MAINID_ID:
		if(!MG_Get_Card_Ready())
    	{
    		return NULL;
    	}

		MG_Get_MSCard_Info(ptemp);
		if (0 == (ptemp[14] & 0x01))
		{
			sprintf(s_text_buffer, "%02X%02X%02X%02X",\
				    ptemp[0], ptemp[1], ptemp[0], ptemp[1]);
		}

		printf("CA mothercard id:%s.\n",s_text_buffer);
		return s_text_buffer;

	case MG312_BASEINFO_SUBMAINCARD_TIME_ID:
		if(!MG_Get_Card_Ready())
    	{
    		return NULL;
    	}

		MG_Get_MSCard_Info(ptemp);
		if (0 == (ptemp[14] & 0x01))
		{	
			sprintf(s_text_buffer, "%d", ((ptemp[4] << 8) | ptemp[5]));
		}
		
        printf("CA soncard active priod:%s.\n",s_text_buffer);
		return s_text_buffer; 

	case MG312_BASEINFO_SUBMAINCARD_ACTIVE_ID:
		if(!MG_Get_Card_Ready())
    	{
    		return NULL;
    	}

		MG_Get_MSCard_Info(ptemp);
		if (0 == (ptemp[14] & 0x01))
		{	
			sprintf(s_text_buffer, "%d", ((ptemp[12] << 8) | ptemp[13]));
		}
		
        printf("CA soncard left active priod:%s.\n",s_text_buffer);
		return s_text_buffer; 	
   
	default:
		break;
	}
	
	return NULL;
}



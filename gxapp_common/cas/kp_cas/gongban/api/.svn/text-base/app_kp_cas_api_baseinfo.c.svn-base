#include "KPAPP.h"
#include "app_common_porting_stb_api.h"
#include "app_kp_cas_api_baseinfo.h"

int32_t app_kp_cas_api_init_baseinfo_data(void)
{
	return 0;
}

char * app_kp_cas_api_get_baseinfo_data(ca_get_date_t *data)
{    
    uint32_t pos = 0 ; 
	uint32_t ID = 0;  
	uint8_t ptemp[KPCAS_FEATURE_DATA_LEN]={0};
    uint32_t tmpU32;
    static char  s_text_buffer[70]={0};
	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	memset(s_text_buffer,0,sizeof(s_text_buffer));

	switch(ID)
	{
	case KP_BASEINFO_CA_ID:
		sprintf(s_text_buffer,"%s","KP CAS");
		return s_text_buffer;
    case KP_BASEINFO_VER_ID:
        if(KP_RET_SUCCESS == KPCAAPP_GetCAVersion(ptemp))
        {
            sprintf(s_text_buffer,"%s",(char *)ptemp);
            printf("CA Version:%s\n",s_text_buffer);
    		return s_text_buffer;
        }
        else return NULL;
	case KP_BASEINFO_LICENCE_ID:
        if(KP_RET_SUCCESS == KPCAAPP_GetLicenceid(ptemp))
        {
            sprintf(s_text_buffer,"%02X%02X%02X%02X%02X%02X%02X%02X",
                ptemp[0],ptemp[1],ptemp[2],ptemp[3],ptemp[4],ptemp[5],ptemp[6],ptemp[7]);
            printf("Licence ID:%s\n",s_text_buffer);
    		return s_text_buffer;
        }
        else return NULL;
    case KP_BASEINFO_UNIQUE_ID:
        if(KP_RET_SUCCESS == KPCAAPP_GetUniqueid(ptemp))
        {
            sprintf(s_text_buffer,"%02X%02X%02X%02X%02X%02X%02X%02X",
                ptemp[0],ptemp[1],ptemp[2],ptemp[3],ptemp[4],ptemp[5],ptemp[6],ptemp[7]);
            printf("Unique ID:%s\n",s_text_buffer);
    		return s_text_buffer;
        }
        else return NULL;
    case KP_BASEINFO_AREACODE_ID:
        if(KP_RET_SUCCESS == KPCAAPP_GetAreaCode(&tmpU32))
        {
            sprintf(s_text_buffer,"%08X",tmpU32);
            printf("AreaCode:%s\n",s_text_buffer);
    		return s_text_buffer;
        }
        else return NULL;
    case KP_BASEINFO_ATTIBUTE_ID:
        if(KP_RET_SUCCESS == KPCAAPP_GetAttibute(ptemp))
        {
            sprintf(s_text_buffer,"%02X%02X%02X%02X%02X%02X%02X%02X",
                ptemp[0],ptemp[1],ptemp[2],ptemp[3],ptemp[4],ptemp[5],ptemp[6],ptemp[7]);
            printf("Attibute Info:%s\n",s_text_buffer);
    		return s_text_buffer;
        }
        else return NULL;
	default:
		break;
	}
	return NULL;

}



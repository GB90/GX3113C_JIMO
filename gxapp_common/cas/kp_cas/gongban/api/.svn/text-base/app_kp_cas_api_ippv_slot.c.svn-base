#include "KPAPP.h"
#include "app_common_porting_stb_api.h"
#include "app_kp_cas_api_ippv_slot.h"

KPProvider *providerIDInfo = NULL;
static int providerNum = 0;
int32_t app_kp_cas_api_init_ippv_slot_data(void)
{
    int i;

    if(!providerIDInfo)
    {
        if(NULL == (providerIDInfo = (KPProvider *)GxCore_Malloc(KPCAS_PROVIDER_CNT*sizeof(KPProvider))))
        {
            printf("app_kp_cas_api_init_ippv_slot_data,GxCore_Malloc failed.\n");
            return 1;
        }
          
    }

    
    memset(providerIDInfo,0,KPCAS_PROVIDER_CNT*sizeof(KPProvider));
    if(KP_RET_SUCCESS != KPCAAPP_GetProviderInfo(providerIDInfo))
    {
		return 1;
    }
    providerNum= 0;
    for(i=0;i<KPCAS_PROVIDER_CNT;i++)
    {
        if(providerIDInfo[i].p_providerID ==0)
        {
            break;
        }
        providerNum ++;
        
    }
	return 0;
}

char * app_kp_cas_api_get_ippv_slot_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0;  
    static char   cText_ippv_operid[8] ={0};
    static char   cText_ippv_operinfo[32] ={0};
    static char   cText_ippv_Amount[8] ={0};
    static char   cText_ippv_Balance[8] ={0};
    KPWallet Info;
	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	switch(ID)
	{
	case KP_IPPV_SLOT_TVS_ID:
		sprintf(cText_ippv_operid,"%d",providerIDInfo[pos].p_providerID);
		return cText_ippv_operid;
        
	case KP_IPPV_SLOT_TVS_NAME:
		sprintf( cText_ippv_operinfo, "%s",(char *)providerIDInfo[pos].p_providerInfo );
		return cText_ippv_operinfo;

    case KP_IPPV_SLOT_TOTAL_ID:
        if(KP_RET_SUCCESS == KPCAAPP_GetWalletsInfo(providerIDInfo[pos].p_providerID,&Info))
        {
            sprintf(cText_ippv_Amount,"%d",Info.p_amount);
    		return cText_ippv_Amount;
        }
        else return NULL;
        
     case KP_IPPV_SLOT_USED_ID:
        if(KP_RET_SUCCESS == KPCAAPP_GetWalletsInfo(providerIDInfo[pos].p_providerID,&Info))
        {
            sprintf(cText_ippv_Balance,"%d",Info.p_balance);
    		return cText_ippv_Balance;
        }
        else return NULL;
        
	default:
		break;
	}
	return NULL;

}

uint8_t app_kp_cas_api_get_ippv_slot_count(ca_get_count_t *data)
{
	if (NULL == data)
		return 0;

	data->totalnum = providerNum;
    return providerNum;
}



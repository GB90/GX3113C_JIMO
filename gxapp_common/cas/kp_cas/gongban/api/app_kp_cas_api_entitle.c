
#include "KPAPP.h"
#include "app_common_porting_stb_api.h"
#include "app_kp_cas_api_entitle.h"


KPSubscription *pInfo = NULL;

static void CAManager_waca_Date_Translator(unsigned short wadate, unsigned short *year, unsigned char *month, unsigned char *day)
{
    unsigned short nYear,nMon,nDay,nTemp,nDate = wadate;
	
    nYear = 2000;
    do
    {
        if((nYear%4 == 0 && nYear%100 != 0) || nYear%400 == 0)
		{
            nTemp = 366;
		}
        else
		{
            nTemp = 365;
		}
        if(nDate < nTemp)
		{
            break;
		}
        nDate -= nTemp;
        nYear ++;
    }while(1);
	
    nMon = 1;
    do
    {
        if(2 == nMon)
        {
            if((nYear%4 == 0 && nYear%100 != 0) || nYear%400 == 0)
			{
                nTemp = 29;
			}
            else
			{
                nTemp = 28;
			}
        }
        else if(4 == nMon || 6 == nMon || 9 == nMon || 11 == nMon)
		{
            nTemp = 30;
		}
        else
		{
            nTemp = 31;
		}
        if(nDate < nTemp)
		{
            break;
		}
        nDate -= nTemp;
        nMon ++;
    }while(1);
    nDay = 1;
    nDay += nDate;
	
    *year	= nYear;
	*month	= (unsigned char)nMon;
	*day	= (unsigned char)nDay;
}

int32_t app_kp_cas_api_init_entitle_data(void)
{
    int i;
    KPProvider providerIDInfo[KPCAS_PROVIDER_CNT];
    if(KP_RET_SUCCESS != KPCAAPP_GetProviderInfo(providerIDInfo))
    {
		return 0;
    }
/*
    for(i=0;i<KPCAS_PROVIDER_CNT;i++)
    {
        printf("KPCAAPP_GetProviderInfo,p_providerID:%d,%s\n",providerIDInfo[i].p_providerID,(char *)providerIDInfo[i].p_providerInfo);
    }
*/
    if(pInfo == NULL)
    {
        if(NULL == (pInfo = (KPSubscription *)GxCore_Malloc(KPCAS_PROVIDER_CNT*sizeof(KPSubscription))))
        {
            printf("app_kp_cas_api_init_entitle_data,GxCore_Malloc failed.\n");
            return 1;
        }
        
    }
    if(pInfo)
    {
        memset(pInfo,0,KPCAS_PROVIDER_CNT*sizeof(KPSubscription));
        for(i=0;i<KPCAS_PROVIDER_CNT;i++)
        {
            if(KP_RET_SUCCESS == KPCAAPP_GetSubscriptionsInfo(providerIDInfo[i].p_providerID, &pInfo[i]))
            {
                /*printf("KPCAAPP_GetSubscriptionsInfo,providerID:%d,count:%d\n",providerIDInfo[i].p_providerID,pInfo[i].p_productCount);
                int j;
                for(j=0;j<pInfo[i].p_productCount;j++)
                {
                   
                   printf("productID:%d,%d,%d\n",pInfo[i].s_product[j].p_productID,pInfo[i].s_product[j].p_beginTime,pInfo[i].s_product[j].p_endTime);
                    
                }*/
            }
        }
    }
	return 0;
}

char * app_kp_cas_api_get_entitle_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0; 
	uint16_t timetmp = 0;
	uint16_t year = 0;
	uint8_t  month=0;
	uint8_t  day=0;
    KPSubscription *pInfo_temp = NULL;


    static char   cTextBuffer_id[32] ={0};
    static char   cTextBuffer_starttime[32] = {0};
    static char   cTextBuffer_endtime[32] = {0};

	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;
    
    if(!pInfo || pos >= pInfo[0].p_productCount+pInfo[1].p_productCount)
    {
        return NULL;
    }
    
    pInfo_temp = &pInfo[0];

    if(pos>=pInfo[0].p_productCount)
    {
        pInfo_temp = &pInfo[1];
        pos -= pInfo[0].p_productCount;
    }

	switch(ID)
	{
	case KP_ENTITLE_ID:
		memset(cTextBuffer_id,0,sizeof(cTextBuffer_id));
	    sprintf( cTextBuffer_id, "%d",pInfo_temp->s_product[pos].p_productID);
		return cTextBuffer_id;
	case KP_ENTITLE_BEGIN:
		memset(cTextBuffer_starttime,0,sizeof(cTextBuffer_starttime));
		timetmp = (uint16_t)(pInfo_temp->s_product[pos].p_beginTime);
		CAManager_waca_Date_Translator( timetmp, &year,  &month, &day );
		sprintf( cTextBuffer_starttime, "%04d-%02d-%02d", year, month,day);
		return cTextBuffer_starttime;
	case KP_ENTITLE_END:
		memset(cTextBuffer_endtime,0,sizeof(cTextBuffer_endtime));
		timetmp = (uint16_t)(pInfo_temp->s_product[pos].p_endTime);
		CAManager_waca_Date_Translator( timetmp, &year,  &month, &day );
		sprintf( cTextBuffer_endtime, "%04d-%02d-%02d", year, month,day);
		return cTextBuffer_endtime;
	default:
		break;
	}

	return NULL;

}

uint8_t app_kp_cas_api_get_entitle_count(ca_get_count_t *data)
{
    if(!data)
    {
        return 0;
    }
    
    if(pInfo)
    {
        data->totalnum = pInfo[0].p_productCount+pInfo[1].p_productCount;
        return data->totalnum;
    }
    data->totalnum = 0;
    return 0;
}




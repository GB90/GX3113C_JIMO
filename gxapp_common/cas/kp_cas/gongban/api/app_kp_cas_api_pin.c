#include "KPAPP.h"
#include "app_common_porting_stb_api.h"
#include "app_kp_cas_api_pin.h"

char* app_kp_cas_api_change_pin(ca_pin_t* data)
{
	uint8_t newPassword1[KPCAS_PIN_LEN+1]={0};
	uint8_t newPassword2[KPCAS_PIN_LEN+1]={0};
	uint8_t newPassword3[KPCAS_PIN_LEN+1]={0};
	U8  wRet;

	if (NULL == data)
		return NULL;
	
	memcpy(newPassword1,data->oldpin, KPCAS_PIN_LEN);
	memcpy(newPassword2, data->newpin, KPCAS_PIN_LEN);
	memcpy(newPassword3, data->newconfirmpin, KPCAS_PIN_LEN);
    if(strlen((char*)newPassword1)!=KPCAS_PIN_LEN 
        ||strlen((char*)newPassword2)!=KPCAS_PIN_LEN 
        ||strlen((char*)newPassword3)!=KPCAS_PIN_LEN )
	{
		return (char*)"PIN is null";
	}
    if(strlen((char*)newPassword1)!=KPCAS_PIN_LEN 
        ||strlen((char*)newPassword2)!=KPCAS_PIN_LEN 
        ||strlen((char*)newPassword3)!=KPCAS_PIN_LEN )
	{
		return (char*)"PIN Length Invalid";
	}
	if(strcmp((char*)newPassword3,(char*)newPassword2))
	{
	    return (char*)"Input new PIN";
	}
   	wRet = KPCAAPP_SetPIN(newPassword1,newPassword2);
	if(wRet == KP_RET_SUCCESS)
	{
		return (char*)"Set PIN Succeed";
	}
    else if(wRet == KP_RET_PIN_LENGHT_INVALID)
    {
        return (char*)"PIN Length Invalid";
    }
    else if(wRet == KP_RET_SAME_PIN)
    {
        return (char*)"PIN SAME";
    }
    else
    {
		return (char*)"PIN Error";
    }

	return NULL;
}



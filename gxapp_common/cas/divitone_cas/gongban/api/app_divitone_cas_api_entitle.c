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
#include "DiviGuardCa.h"
#include "app_common_porting_stb_api.h"
#include "app_divitone_cas_api_entitle.h"
#include "app_common_epg.h"


static PackageInfo pack; //harvey, 2009.06.19
static INT32 pack_count;  

static UINT16 DIVI_Decimal2Bcd(UINT16 Dec)
{
	UINT16 tmp;
	UINT16 result=0;	
	
	tmp=(UINT16)Dec/1000;
	result|=tmp<<12;
	Dec-=tmp*1000;
	
	tmp=(UINT16)Dec/100;
	result|=tmp<<8;
	Dec-=tmp*100;

	tmp=(UINT16)Dec/10;
	result|=tmp<<4;
	Dec-=tmp*10;
	
	result|=Dec;	

	return result;
	
}

int32_t app_divitone_cas_api_init_entitle_data(void)
{
    	pack_count = 0;
	if(2==DIVI_GetSMCEntitleInfo(&pack))
	{
		pack_count=pack.Package_Count;
	}
	else
	{
		pack_count=0;
	}
    	return pack_count;
		
}


char * app_divitone_cas_api_get_entitle_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0;  
	
	static char s_text_buffer[10];
	static char s_text_buffer2[10];
	static char s_text_buffer3[20];

	unsigned char tmp;

	INT16 PackagePrice;	
	UINT16 StartYear;
	UINT16 StartMonth;
	UINT8   StartDate;
	UINT16 EndYear;
	UINT16 EndMonth;
	UINT8   EndDate;
	
	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;
	
	switch(ID)
	{
		case DIVI_ENTITLE_ENTITLE_ID:
		//	sprintf(s_text_buffer,"%d",pack.PackageInfo[pos].PackageId);
			memset(s_text_buffer,0,sizeof(s_text_buffer));
			 tmp = pack.PackageInfo[pos].PackageId;
			 if(tmp<10)
			 {
				 s_text_buffer[0]=tmp+0x30;			
				 s_text_buffer[1]='\0';		 
			 }
			 else
			 {
				s_text_buffer[0]=tmp/10+0x30;
				s_text_buffer[1]=(tmp%10)+0x30;			
				s_text_buffer[2]='\0';
			 }		
			return s_text_buffer;
		case DIVI_ENTITLE_ENTITLE_PRICE:
		//	tmp = pack.PackageInfo[pos].PackagePrice;			
		//   sprintf( s_text_buffer, "%d",pack.PackageInfo[pos].PackagePrice);
			memset(s_text_buffer2,0,sizeof(s_text_buffer2));
			PackagePrice=pack.PackageInfo[pos].PackagePrice;
			PackagePrice=DIVI_Decimal2Bcd(PackagePrice);
			
			s_text_buffer2[0]=(unsigned char)((PackagePrice>>12)&0x0f)+0x30;
			s_text_buffer2[1]=(unsigned char)((PackagePrice>>8)&0x0f)+0x30;
			s_text_buffer2[2]=(unsigned char)((PackagePrice>>4)&0x0f)+0x30;
			s_text_buffer2[3]=(unsigned char)(PackagePrice&0x0f)+0x30;
			s_text_buffer2[4]='\0';
			 
			return s_text_buffer2;
		case DIVI_ENTITLE_ENTITLE_PERID:
			memset(s_text_buffer3,0,sizeof(s_text_buffer3));
			StartYear=DIVI_Decimal2Bcd(pack.PackageInfo[pos].StartYear);
			StartMonth=DIVI_Decimal2Bcd(pack.PackageInfo[pos].StartMonth);
			StartDate=(UINT8)DIVI_Decimal2Bcd((UINT16)pack.PackageInfo[pos].StartDate);

			EndYear=DIVI_Decimal2Bcd(pack.PackageInfo[pos].EndYear);
			EndMonth=DIVI_Decimal2Bcd(pack.PackageInfo[pos].EndMonth);
			EndDate=(UINT8)DIVI_Decimal2Bcd((UINT16)pack.PackageInfo[pos].EndDate);
			
			//开始年
			s_text_buffer3[0]=((StartYear>>12)&0x0f)+0x30;
			s_text_buffer3[1]=((StartYear>>8)&0x0f)+0x30;
			s_text_buffer3[2]=((StartYear>>4)&0x0f)+0x30;
			s_text_buffer3[3]=(StartYear&0x0f)+0x30;

			//开始月
			s_text_buffer3[4]=((StartMonth>>4)&0x0f)+0x30;
			s_text_buffer3[5]=(StartMonth&0x0f)+0x30;

			//开始日
			s_text_buffer3[6]=((StartDate>>4)&0x0f)+0x30;
			s_text_buffer3[7]=(StartDate&0x0f)+0x30;
			
			s_text_buffer3[8]='-';

			//结束年
			s_text_buffer3[9]=((EndYear>>12)&0x0f)+0x30;
			s_text_buffer3[10]=((EndYear>>8)&0x0f)+0x30;
			s_text_buffer3[11]=((EndYear>>4)&0x0f)+0x30;
			s_text_buffer3[12]=(EndYear&0x0f)+0x30;

			//结束月
			s_text_buffer3[13]=((EndMonth>>4)&0x0f)+0x30;
			s_text_buffer3[14]=(EndMonth&0x0f)+0x30;

			//结束日
			s_text_buffer3[15]=((EndDate>>4)&0x0f)+0x30;
			s_text_buffer3[16]=(EndDate&0x0f)+0x30;

			s_text_buffer3[17]='\0';			

			return s_text_buffer3;
		case DIVI_ENTITLE_ENTITLE_SCRIPT:			
			return (char *)pack.PackageInfo[pos].PackageScript;
		default:
			break;
	}
	return NULL;

}

uint8_t app_divitone_cas_api_get_entitle_count(ca_get_count_t *data)
{
	data->totalnum = pack_count;
    	return pack_count;
}






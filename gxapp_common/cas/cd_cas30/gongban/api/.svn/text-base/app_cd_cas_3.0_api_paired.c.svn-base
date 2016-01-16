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
#include "CDCASS.h"
#include "app_common_porting_stb_api.h"
#include "app_cd_cas_3.0_api_paired.h"


char				pbySTBID_List[30]	=
{
	0,
} ;

CDCA_U16				   wRet 			   = 0;
char				pbyNum				= 0;
unsigned int				  chStbIdIndex = 0;



int32_t app_cd_cas30_api_init_paired_data(void)
{
	pbyNum = 0;
	chStbIdIndex = 0;
	memset(pbySTBID_List,0,sizeof(pbySTBID_List));
    wRet = CDCASTB_IsPaired( (CDCA_U8*)&pbyNum, (CDCA_U8*)pbySTBID_List );
	return 0;
}

char * app_cd_cas30_api_get_paired_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0;
    static char            s_text_buffer[30];

    unsigned int                  i= 0;


	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	memset(s_text_buffer,0,sizeof(s_text_buffer));



    switch( ID )
    {
        case CD_PAIRED_STBID_LIST1_ID:
            i = 0;
           break;
        case CD_PAIRED_STBID_LIST2_ID:
			 i = 1;
			break;
        case CD_PAIRED_STBID_LIST3_ID:
			 i = 2;
			break;
		case CD_PAIRED_STBID_LIST4_ID:
			 i = 3;
			break;
		case CD_PAIRED_STBID_LIST5_ID:
			 i = 4;
			break;
        default:
           break;
    }
	
	switch(ID)
		{
			case CD_PAIRED_INFO_ID:
				if( wRet == CDCA_RC_OK )
			    {
			        sprintf( s_text_buffer,"%s", "Paired Ok" );
				return s_text_buffer;

			    }
			    else if( wRet == CDCA_RC_CARD_INVALID )
			    {
			        sprintf( s_text_buffer,"%s", "Invalid Smartcard");
				return s_text_buffer;
					
			    }
			    else if( wRet == CDCA_RC_CARD_NOPAIR )
			    {
			        sprintf( s_text_buffer,"%s", "The card is not paired with any STB");
				return s_text_buffer;
					
			    }
			    else if( wRet == CDCA_RC_CARD_PAIROTHER )
			    {
			        sprintf( s_text_buffer,"%s", "Paired to other STB");
				return s_text_buffer;

			    }
			    else
			    {
			    		return NULL;
			    }
				break;
		case CD_PAIRED_STBID_LIST1_ID:
		case CD_PAIRED_STBID_LIST2_ID:
		case CD_PAIRED_STBID_LIST3_ID:
		case CD_PAIRED_STBID_LIST4_ID:
		case CD_PAIRED_STBID_LIST5_ID:
			if((0 != pbySTBID_List[6*i])
				||(0 != pbySTBID_List[6*i + 1])
				||(0 != pbySTBID_List[6*i + 2])
				||(0 != pbySTBID_List[6*i + 3])
				||(0 != pbySTBID_List[6*i + 4])
				||(0 != pbySTBID_List[6*i + 5])
				)
			{
				sprintf(s_text_buffer, "STBID%d:0x%02X%02X%02X%02X%02X%02X",
										  chStbIdIndex+1,
										  pbySTBID_List[6*i],
										  pbySTBID_List[6*i + 1],
										  pbySTBID_List[6*i + 2],
										  pbySTBID_List[6*i + 3],
										  pbySTBID_List[6*i + 4],
										  pbySTBID_List[6*i + 5]
										  );
			
				chStbIdIndex++;
				return s_text_buffer;
			}
			else
				{
					return NULL;
				}
			break;
			default:
				break;
		}
	return NULL;

}







/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	tr_cas_porting_flash.c
* Author    : 	bingyu
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2012.12.02		  bingyu 	 			creation
*****************************************************************************/
#include <gxtype.h>
#include "gxmsg.h"
#include "gxapp_sys_config.h"
#include "app_common_play.h"
#include "app_common_table_pmt.h"
#include "app_common_porting_stb_api.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_porting_ca_smart.h"
#include "Tr_Cas.h"
#include "tr_cas_porting.h"


S16   TRDRV_NVRAM_Initialise(void)
{
	return 0;
}

S16 TRDRV_NVRAM_Read(U32 ulOffset, OUT U8 *pbData, U16 wLength)
{
	uint32_t len = wLength;
	int ret = 1;
	
	if (NULL == pbData)
	{
		printf("[TRDRV_NVRAM_Read]pbData is NULL!!!\n");
		return 1;
	}

	if (ulOffset > FLASH_TRCAS_COMMON_SIZE-1)
	{
		printf("[TRDRV_NVRAM_Read]invalid ulOffset(0x%x)!!!\n", (unsigned int)ulOffset);
		return 1;
	}

	if (wLength > FLASH_TRCAS_COMMON_SIZE)
	{
		printf("[TRDRV_NVRAM_Read]wLength(0x%x) exceed to MAX!!!\n", (unsigned int)wLength);
		return 1;
	}

	if ((ulOffset + wLength) > FLASH_TRCAS_COMMON_SIZE-1)
	{
		printf("[TRDRV_NVRAM_Read]ulOffset + wLength(0x%x) exceed to END!!!\n",\
				(unsigned int)((ulOffset + wLength)));
		return 1;
	}
	
	//printf("[TRDRV_NVRAM_Read]ulOffset(%d); wlen(%d).\n", (unsigned int)ulOffset, wLength);
	ret = app_porting_ca_flash_read_data(ulOffset, pbData, &len);
	if (ret == 1)
	{
		return 0;
	}
	
	printf("##[TRDRV_NVRAM_Read] fail!!!\n");
	return 1;
}

S16 TRDRV_NVRAM_Write(U32 ulOffset, IN U8 *pbData,  U16 wLength)
{
	int ret = 1;
	
	if (NULL == pbData)
	{
		printf("[TRDRV_NVRAM_Write]pbData is NULL!!!\n");
		return 1;
	}

	if (ulOffset > FLASH_TRCAS_COMMON_SIZE-1)
	{
		printf("[TRDRV_NVRAM_Write]invalid ulOffset(0x%x)!!!\n", (unsigned int)ulOffset);
		return 1;
	}

	if (wLength > FLASH_TRCAS_COMMON_SIZE)
	{
		printf("[TRDRV_NVRAM_Write]wLength(0x%x) exceed to MAX!!!\n", (unsigned int)wLength);
		return 1;
	}

	if ((ulOffset + wLength) > FLASH_TRCAS_COMMON_SIZE-1)
	{
		printf("[TRDRV_NVRAM_Write]ulOffset + wLength(0x%x) exceed to END!!!\n",\
				(unsigned int)((ulOffset + wLength)));
		return 1;
	}
	
	//printf("[TRDRV_NVRAM_Write]ulOffset(%d); wlen(%d).\n", (unsigned int)ulOffset, wLength);
	ret = app_porting_ca_flash_write_data(ulOffset, pbData, wLength);
	if (ret == 1)
	{
		return 0;
	}

	printf("##[TRDRV_NVRAM_Write]fail!!!\n");
	return 1;
}




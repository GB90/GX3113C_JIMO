/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	yg_cas_porting_flash.c
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
	
#include "gyca_def.h"
#include "gyca_pub.h"
#include "gyca_osp.h"
#include "gyca_interface.h"
#include "gyca_porting.h"

/*
** 存储空间管理函数接口定义*/
/***********************************************************************************************
** 	函数名称:GYCA_GetBuffer
************************************************************************************************
**	提 供 者：  	机顶盒。
**	使 用 者：	CA模块。
**	功能描述：	获得机顶盒提供给CA模块保存信息的起始地址和尺寸。
**	输入参数：无
**	输出参数：	lStartAddr   分配空间的起始地址
**                                lSize          分配空间的大小
**	返 回 值：	成功GYSUCCESS  失败 GYFAILURE
**	其他说明：
**                               机顶盒厂商应分配不少于64K的EEPROM空间
*************************************************************************************************/
GYS32 GYCA_GetBuffer (GYS32 *lStartAddr, GYS32* lSize)
{
    *lStartAddr = FLASH_GY_CAS_ADDR;
	*lSize = FLASH_GY_CAS_SIZE;

	return GYSUCCESS;
}

/*************************************************************************************************
** 	函数名称:GYCA_ReadBuffer
**************************************************************************************************
**	提 供 者：  	机顶盒。
**	使 用 者：	CA模块。
**	功能描述：从指定的地址读出指定数量字节放在指定的空间中
**	输入参数：	pStartAddr：		要读取数据的存储空间的地址。
**	 				pbData：			被读取数据的存放地址指针。
**	 				pBytes：		       要读的数据的长度
**	输出参数：	pbData：			被读出的数据。
**	返 回 值：	GYSUCCESS 成功， GYFAILURE 失败。
**	其他说明：
**************************************************************************************************/
GYS32 GYCA_ReadBuffer(const GYS32 lStartAddr, GYU8* pData, GYS32* nLen)
{
	app_porting_ca_flash_read_data(lStartAddr-FLASH_GY_CAS_ADDR,pData,(GYU32 *)nLen);
	
	return GYSUCCESS;
}

/*************************************************************************************************
** 	函数名称:GYCA_WriteBuffer
**************************************************************************************************
**	提 供 者：	机顶盒。
**	使 用 者：	CA模块。
**	功能描述：	向机顶盒的存储空间写信息
**	输入参数：	lStartAddr：		要写的存储空间的目标地址。
**					pData：	要写的数据
** 					nLen：		要写的数据的长度
**	输出参数：	无。
**	返 回 值：    GYSUCCESS 成功， GYFAILURE： 失败。
**	其他说明：
*************************************************************************************************/
GYS32 GYCA_WriteBuffer(const GYS32 lStartAddr, const GYU8* pData, GYS32 nLen) 
{
	app_porting_ca_flash_write_data(lStartAddr-FLASH_GY_CAS_ADDR,(uint8_t*)pData,nLen);

	return GYSUCCESS;
}




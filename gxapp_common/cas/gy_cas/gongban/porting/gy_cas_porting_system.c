/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	yg_cas_porting_system.c
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

#define MAX_SEARIAL_LEN (20)
#define SERIAL_START_ADDR (0x30000)


/***********************************************************************************************
** 	函数名称:GYCA_GetStbSn
************************************************************************************************
**	提 供 者：	 STB
**	使 用 者：	CAM
**	功能描述：	 向CA模块提供STB的序列号
**	输入参数：	sn序列号存储地址
**			len序列号长度存储地址
**	输出参数：	sn序列号存储地址
**			len序列号长度存储地址
**	返 回 值：	 无
**************************************************************************************************/
void GYCA_GetStbSn(GYU8 * sn/*MAX 20 BYTES*/, GYS16 *len)
{
	char serialNumb[MAX_SEARIAL_LEN+1]={0};
	
	if((sn==NULL)||(len==NULL)||(*len<1))
		return;

	memset(sn,0x00,*len);
	
	app_flash_get_serial_number(SERIAL_START_ADDR,(char *)serialNumb,MAX_SEARIAL_LEN);
	
	if(*len>MAX_SEARIAL_LEN)
	{
		*len = MAX_SEARIAL_LEN;
	}

	memcpy(sn,serialNumb,*len);
}



/*
** 生成一个随机数的函数接口*/
/**********************************************************************************************
** 函数名称：    GYCA_GetRandomNumber
***********************************************************************************************
**     提供者：           机顶盒
**     使用者：           CA模块
**     何时使用：         CA模块内部需要时使用
**     功能描述：         获取一个不大于限定数的随机数
**     输入参数：         limit   最大限定的数
**     输出参数：         无
**     返回值：           随机数
**     其他说明：         无
***********************************************************************************************/
GYU32 GYCA_GetRandomNumber(GYU32 limit)
{
	static unsigned int uRandomNum=0;

	if(uRandomNum==0)
		srand(uRandomNum);

	uRandomNum = rand()%limit;

	return uRandomNum;
}


/*
** 打印测试函数接口*/
/**********************************************************************************************
** 函数名称：    GYCA_DBG_Print
***********************************************************************************************
**     提供者：      机顶盒
**     使用者：      CA模块
**     何时使用： CA模块内部需要时调试测试时使用
**     功能描述： 通过串口打印格式化调试字符串
**     输入参数： fmt     c语言格式化字符串
**     输出参数： 无
**     返回值：      实际打印的字符数量.(字节数)
**     其他说明： 无
***********************************************************************************************/
GYU32 GYCA_DBG_Print (const char *fmt, ...)
{
	static char print_buf[1024]={0};
#if 0

	va_list args;
	va_start(args,fmt);
	Vsnprintf(print_buf,sizeof(print_buf), fmt, args);
	va_end(args);

	printf("[GYCA]	%s \n", print_buf);
#endif	
	return TRUE;
}



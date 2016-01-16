/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	yg_cas_porting_smartcard.c
* Author    : 	yubing
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2012.12.02		  yubing 	 			creation
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





static unsigned char gacYgcasATR[YGCAS_ART_LEN]={0};
static SC_Status_Notify_Fct_t gfScNotifyFct=NULL;


SC_Status_Notify_Fct_t GYCA_GetNotify(void)
{
	return gfScNotifyFct;
}


/*
** 智能卡部分函数接口*/
/**********************************************************************************************
** 	函数名称:GYCA_SC_DRV_Initialize
***********************************************************************************************
** 	提 供 者： 	机顶盒。
**	使 用 者：	CAM
**	何时调用：	金亚CAS在做初始化工作时提供的一个接口给机顶盒
**                                初始化机顶盒中的智能卡模块驱动程序.
**	功能描述：	 初始化智能卡驱动。
**	输入参数：	 无。
**	输出参数：	 无。
**	返 回 值：	 SUCCESS：成功， FAILURE： 失败。
**	其他说明：
**********************************************************************************************/
GYS32	GYCA_SC_DRV_Initialize(void)
{
	GxSmcParams    param;
	GxSmcTimeParams    time;
	
	memset(&time,0,sizeof(GxSmcTimeParams));
	memset(&param,0,sizeof(GxSmcParams));

	time.baud_rate	= RUNTIME_BAUD_RATE;
	time.egt		= RUNTIME_EGT;
	time.etu		= RUNTIME_ETU;
	time.tgt		= RUNTIME_TGT;
	time.twdt		= RUNTIME_TWDT;
	time.wdt		= RUNTIME_WDT;

	param.detect_pole = GXSMC_DETECT_LOW_LEVEL;
	param.io_conv = GXSMC_DATA_CONV_DIRECT;
	param.parity =  GXSMC_PARITY_ODD;
	param.protocol = DISABLE_REPEAT_WHEN_ERR;
	param.sci_sel = __GXSMART_SCI2;
	param.stop_len = GXSMC_STOPLEN_0BIT;
	param.vcc_pole = GXSMC_VCC_HIGH_LEVEL;
	param.default_etu = RUNTIME_ETU;
	param.auto_etu = 1;
	param.auto_parity = 1;

	app_porting_ca_smart_card_init(param,time);

	return 	GYSUCCESS;
}

/**********************************************************************************************
** 	函数名称: GYCA_SCStatus_RegisterEN2Notify
***********************************************************************************************
** 	提 供 者： 	机顶盒。
**	使 用 者：	CAM
**	何时使用：	CAM 初始化的时候调用该函数。
**	功能描述：	注册智能卡状态改变时通知CAM任务的回调函数。
**	输入参数：	SCStatusNotify (GYU8 bCardNumber, GYU8 bStatus,GYU8* pReserved1, GYS32 iReserved2 )
**					其中:  bCardNumber：	卡座号；
**							bStatus：		卡座状态；
**							pReserved1            保留参数1;
**							iReserved2             保留参数2;
**	输出参数：	无。
**	返 回 值：	SUCCESS：成功， FAILURE： 失败。
**	其他说明：	机顶盒须在智能卡的状态改变后立即通过该回调函数
**                                通知CAM。
**					智能卡的状态必须为GYCA_SMCSTATUS所定义的几个值之一。
**                                详细请参考gyca_def.h GYCA_SMCSTATUS定义                                     
***********************************************************************************************/
GYS32 GYCA_SCStatus_RegisterEN2Notify(SC_Status_Notify_Fct_t SCStatusNotify)
{
	gfScNotifyFct = SCStatusNotify;

	return GYSUCCESS;
}



/*********************************************************************************************
** 	函数名称:GYCA_SC_DRV_ResetCard
**********************************************************************************************
**	提 供 者：		机顶盒。
**	使 用 者：		CA模块。
**	功能描述： 	复位智能卡。
**	输入参数： 	bCardNumber，	为智能卡的卡座号。
**	输出参数： 	无。
**	返 回 值： 		无返回，总认为是成功的。
**	其他说明：
*********************************************************************************************/
void GYCA_SC_DRV_ResetCard (GYU8 bCardNumber)
{
	unsigned char ucLen;
	GxSmcTimeParams TimeParam;
		
	TimeParam.baud_rate  = RUNTIME_BAUD_RATE;
	TimeParam.egt        = RUNTIME_EGT;
	TimeParam.etu        = RUNTIME_ETU;
	TimeParam.tgt        = RUNTIME_TGT;
	TimeParam.twdt       = RUNTIME_TWDT;
	TimeParam.wdt        = RUNTIME_WDT;
#if 1
	app_porting_ca_smart_reset(TimeParam,gacYgcasATR, &ucLen);
#else

	if(TRUE==app_porting_ca_smart_reset(TimeParam,gacYgcasATR, &ucLen))
	{
		if(GYTRUE!=GYCA_SC_ValidateATR(gacYgcasATR, ucLen))
		{
			if(gfScNotifyFct!=NULL)
			{
				gfScNotifyFct(0, GYCA_CARD_NOTVALID, 0, 0);
			}
		}
	}
	else
	{
		if(gfScNotifyFct!=NULL)
		{
			gfScNotifyFct(0, GYCA_CARD_BAD, 0, 0);
		}
	}
#endif
	return;
}

/*********************************************************************************************
** 	函数名称:GYCA_SC_DRV_SendDataEx
**********************************************************************************************
**	提 供 者： 		机顶盒。
**	使 用 者：	 	CA模块。
**	何时使用：		向智能卡发送数据时。
**	功能描述：		CA模块调用此函数向智能卡发送数据。
**	输入参数：		bCardNumber：	暂时保留不用；
**						bLength：		pabMessage的长度；
**						pabMessage：		发送命令的消息指针；
**	输出参数：		pabResponse：	响应结果的数据块；
**						bRLength：		响应结果的数据块长度。
**                              pbSW1：         智能卡状态字节1
**				pbSW2：         智能卡状态字节2
**	返 回 值：		SMC_COM_OK				(0)发送命令成功
**						GYSUCCESS 成功  GYFAILURE 失败
**	其他说明：if *pbSW1=0x61 then buf[0] = pabMessage[0]&0x7f
**********************************************************************************************/
GYS32 GYCA_SC_DRV_SendDataEx
(
	GYU8 bCardNumber, 
	GYU8 bLength,
	GYU8* pabMessage, 
	GYU8* pabResponse,
	GYU8* bRLength,
	GYU8 *pbSW1,
	GYU8 *pbSW2
)
{
	unsigned char sc_result,tmpchar[1024]={0};
	uint16_t  sc_reslen;
	
	sc_result =  app_porting_ca_smart_sc_apdu(pabMessage,(uint16_t)bLength,tmpchar,&sc_reslen);
	if (TRUE == sc_result)
	{
	 	if (sc_reslen >= 2)
 		{
 			*bRLength = (GYU8)sc_reslen-2;
			memcpy(pabResponse,tmpchar,sc_reslen-2);
			*pbSW1 = tmpchar[ sc_reslen-2];
			*pbSW2 = tmpchar[ sc_reslen-1];
		}
		else
		{
			*bRLength = 0;
			return GYFAILURE;
		}	
	}
	else
	{
		*bRLength = 0;
		return GYFAILURE;
	}

	return GYSUCCESS;
}






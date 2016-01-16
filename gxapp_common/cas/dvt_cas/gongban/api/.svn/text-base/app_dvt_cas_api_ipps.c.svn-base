/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_dvt_cas_api_ipps.c
* Author    : 	
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.12.05		  name 	 			creation
*****************************************************************************/
#include "app_dvt_cas_api_ipps.h"

static BYTE viewed_ipps_count = 0;
static BYTE book_ipps_count = 0;
static SDVTCAViewedIpp DVTCAViewedIppData[250];
static SDVTCAIpp DVTCABookIppData[250];
extern SDVTCATvsInfo pOperatorInfo[5];
//char szTime[20];
//SDVTCAViewedIpp* pIpp;
//BYTE byTmp,byCount = 5;
int32_t app_dvt_cas_book_ipps_init(void)
{
	//extern  BYTE  poperatorCount ;
	BYTE operatorCount = 5;
	memset(pOperatorInfo,0,5*sizeof(SDVTCATvsInfo));
	if(0 != DVTCASTB_GetOperatorInfo(-1, &operatorCount,pOperatorInfo))
	{
		//提示失败
		return 0;
	}
	book_ipps_count = 250;
	//if( 0 != DVTCASTB_GetViewedIpps(&viewed_ipps_count, DVTCAIppData)== DVTCA_OK)//获取成功
	if(DVTCASTB_GetBookIpps(&book_ipps_count, DVTCABookIppData))
	{
		//提示失败
		return 0;
	}
	return book_ipps_count;
}
int32_t app_dvt_cas_book_ipps_get_totalcount(void)
{
	return book_ipps_count;
}
char *app_dvt_cas_book_ipps_get_progname(int progindex)//节目名称
{
	return DVTCABookIppData[progindex].m_szProdName;
}
BYTE app_dvt_cas_book_ipps_get_bookstatus(int progindex)//订购状态
{
	BYTE status;
	status = (DVTCABookIppData[progindex].m_byIppStatus)&0x02;
	return status;
}
BYTE app_dvt_cas_book_ipps_get_type(int progindex)//IPP节目类型
{
	BYTE type;
	type = (DVTCABookIppData[progindex].m_byIppStatus)&0x08;
	return type;
}

DWORD app_dvt_cas_book_ipps_get_starttime(int progindex)//开始时间
{
	return DVTCABookIppData[progindex].m_tStartTime;
}

DWORD app_dvt_cas_book_ipps_get_duration(int progindex)//持续时长(秒)
{
	return DVTCABookIppData[progindex].m_dwDuration;
}
char* app_dvt_cas_book_ipps_get_servicename(int progindex)//所在业务名
{
	return DVTCABookIppData[progindex].m_szServiceName;
}
WORD app_dvt_cas_book_ipps_get_bookedprice(int progindex)//已经订购价格(分)
{
	return DVTCABookIppData[progindex].m_wBookedPrice;
}
BYTE app_dvt_cas_book_ipps_get_bookedinterval(int progindex)//收费间隔
{
	return DVTCABookIppData[progindex].m_byBookedInterval;
}

BYTE app_dvt_cas_book_ipps_get_bookedpricetype(int progindex)//当前收费间隔
{
	return DVTCABookIppData[progindex].m_byBookedPriceType;
}

BYTE app_dvt_cas_book_ipps_get_unit(int progindex)//收费间隔时间单位
{
	return DVTCABookIppData[progindex].m_byUnit;
}
DWORD app_dvt_cas_book_ipps_get_curprice(int progindex)//当前价格(分)
{
	//return DVTCABookIppData[progindex].m_wCurCppNoTapPrice;//要回传不录像
	return DVTCABookIppData[progindex].m_wCurTppNoTapPrice;//不 回传不录像
}

BYTE app_dvt_cas_book_ipps_get_curinterval(int progindex)//当前收费间隔
{
	return DVTCABookIppData[progindex].m_byCurInterval;
}

SDVTCAIpp app_dvt_cas_book_ipps_get_curIPPs(int progindex)//当前收费间隔
{
	return DVTCABookIppData[progindex];
}


//////////////////////////////////////////////////////////////////////

int32_t app_dvt_cas_viewed_ipps_init(void)
{
	//extern  BYTE  poperatorCount ;
	BYTE operatorCount = 5;
	memset(pOperatorInfo,0,5*sizeof(SDVTCATvsInfo));
	if(DVTCA_OK != DVTCASTB_GetOperatorInfo(-1, &operatorCount,pOperatorInfo))
	{
		//提示失败
		return 0;
	}
	
	viewed_ipps_count = 250;
	if( DVTCA_OK != DVTCASTB_GetViewedIpps(&viewed_ipps_count, DVTCAViewedIppData))//获取成功
	{
		//提示失败
		return 0;
	}

	return viewed_ipps_count;
}

int32_t app_dvt_cas_viewed_ipps_get_totalcount(void)
{
	return viewed_ipps_count;
}
char *app_dvt_cas_viewed_ipps_get_tvs_name(int progindex)//运营商名称
{
	int tvsid,index;
	tvsid = DVTCAViewedIppData[progindex].m_wTVSID;
	for(index = 0;index<5;index++)
	{
		if(tvsid == pOperatorInfo[index].m_wTVSID) 
			break;
	}
	return pOperatorInfo[index].m_szTVSName;
	//return pOperatorInfo[tvsid].m_szTVSName;
}
DWORD app_dvt_cas_viewed_ipps_get_starttime(int progindex)//开始时间
{
	return DVTCAViewedIppData[progindex].m_tStartTime;
}

DWORD app_dvt_cas_viewed_ipps_get_duration(int progindex)//持续时长
{
	return DVTCAViewedIppData[progindex].m_dwDuration;
}
BYTE app_dvt_cas_viewed_ipps_get_interval(int progindex)//收费间隔
{
	return DVTCAViewedIppData[progindex].m_byBookedInterval;
}

BYTE app_dvt_cas_viewed_ipps_get_pricetype(int progindex)//价格类型
{
	return DVTCAViewedIppData[progindex].m_byBookedPriceType;
}

WORD app_dvt_cas_viewed_ipps_get_price(int progindex)//价格类型
{
	return DVTCAViewedIppData[progindex].m_wBookedPrice;
}

BYTE app_dvt_cas_viewed_ipps_get_unit(int progindex)//收费单位
{
	return DVTCAViewedIppData[progindex].m_byUnit;
}
char* app_dvt_cas_viewed_ipps_get_progname(int progindex)//产品名称
{
	return DVTCAViewedIppData[progindex].m_szProdName;
}
char* app_dvt_cas_viewed_ipps_get_otherinfo(int progindex)//其它信息
{
	return DVTCAViewedIppData[progindex].m_szOtherInfo;
}

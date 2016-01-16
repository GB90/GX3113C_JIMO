#ifndef	__wanfacas_h
#define __wanfacas_h
/*
**=============================================================================
* Copyright (C) 2003,ChengDu Wanfa Catv & Communication equipment co.,ltd
* All rights reserved.
* File name           : wanfacas.h
* Abstract            : prototype define for wanfaca system
* Current version     : V1.0
* Init version        : V1.0
* Author              : ZhouYong
* Date created        : 2003-07-02
* Date finished       : 2003-07-22
* Modification history: Mender     Date        Description
*                       ZhouYong   2008-10-07  WFCAS_SendPPTOkFlag is added
*                       ZhouYong   2004-10-27  The interface function is modified
*                       ZhouYong   2004-12-22  WFCAS_GetStbNo is added
*                       ZhouYong   2005-12-13  Adapt to huaxu smt card
*                       ZhouYong   2006-05-16  The function WFCAS_GetEntitlement is added
* 
**=============================================================================
*/

#include "wftype.h"

typedef struct
{
	U16	Mjd;/* MJD */
	U32	Utc;/* 最高字节为0，低三字节为UTC */
}WFCAS_DateTime_t;

typedef struct
{
	BOOL	ChildCard;/* 是否是子卡,TRUE＝是子卡, FALSE＝是母卡 */
	U08		Card_No[8];/* Compact BCD */
	U16		Issuance_Date;/* 以MJD表示 */
	U16		Expire_Date;/* 以MJD表示 */
	U08		Provider[32];/* String,卡的发行商 */
	U32		PairingStbNo;/* 与该卡配对的StbNo */
/* 以下的域只有子卡才有效 */
	U08		MotherCardNo[8];/* 母卡的卡号*/
	WFCAS_DateTime_t	UpdateOrigination;/* 母卡对子卡授权时的时间起点 */
	WFCAS_DateTime_t	UpdateExpiration; /* 母卡对子卡授权时的时间终点 */
}WFCAS_User_t;

typedef struct
{
	U16	Cas_Id;
	U16	Sub_Id;
	U08	Version;
	U08	Description[32];/* String,CAS的说明 */
}WFCAS_Version_t;

typedef struct
{
	U08	Mode;/* 最低三位表示调制的方式或者极化的方式,最高位B7表示LOADER频点,B6表示NIT频点,B5表示数据广播频点 */
	U32	Frequency;/* 以KHz为单位 */
	U32	SymbolRate;/* 以KS/Sec为单位 */
}WFCAS_Delivery_t;

typedef struct
{
	U32	Product_Id;/* Product identifier */
	U16	Ori_Date;/* original date,MJD */
	U32	Ori_Time;/* original time,UTC */
	U16	Exp_Date;/* expire date,MJD */
	U32	Exp_Time;/* expire time,UTC */
}WFCAS_Entitlement_t;

typedef enum
{
	FIRST,/* 第一个 */
	PREV,/* 前一个 */
	NEXT,/* 下一个 */
	LAST/* 最后一个 */
}WFCAS_Direction_t;

typedef struct
{
	U16	Network_Id;
	U16	Stream_Id;
	U16	Program_No;
}WFCAS_Program_t;

typedef struct
{
	U08	TableId;/* Table id:CAT,PMT,CMT */
	U16	Bytes;/* Number of data in byte,Must be <= 256 */
	U08	*Array;/* Body of data */
}WFCAS_CaTable_t;

typedef struct
{
	BOOL	Delivery;/* TRUE＝有线传送,FALSE＝卫星传送 */
	U08		Identity[8];
}WFSTB_Identity_t;

typedef struct
{
	U32	Money;/* 用户当前总金额：以最小货币单位为单位*/
	U32	Balance;/*用户已消费总额: 以最小货币单位为单位*/
}WFCAS_Purse_t;

U32 WFCAS_GetStbNo(U32 *StbNo);
U32 WFCAS_GetPurseInfo(WFCAS_Purse_t *Purse);
U32 WFCAS_GetUserInfo(WFCAS_User_t *UserInfo);
U32 WFCAS_GetVerInfo(WFCAS_Version_t *VerInfo);
U32 WFCAS_GetDateTime(WFCAS_DateTime_t *DateTime);
U32 WFCAS_GetDelivery(WFCAS_Delivery_t *Delivery,WFCAS_Direction_t Direction);
U32 WFCAS_GetEntitlement(WFCAS_Entitlement_t *Entitlement,WFCAS_Direction_t Direction);
U32 WFCAS_CardEventNotify(BOOL Inserted);
U32 WFCAS_SendPPTOkFlag(BOOL IsPPV);
VOID WFCAS_ReceiveIsrForCom(CHAR Ch);
U32 WFCAS_SubmitPduFromCom(U32 *BytesReceived);
U32 WFCAS_LicensedToSubStb(VOID);
U32 WFCAS_ProgramEntitle(WFCAS_Program_t *Program);
U32 WFCAS_ParseCaTable(WFCAS_CaTable_t *CaTable);
U32 WFCAS_SetZone(U08 *Zone/* 8个字节 */);
U32 WFCAS_Initialize(WFSTB_Identity_t *Identity);
#endif


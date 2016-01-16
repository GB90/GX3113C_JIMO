/****************************************************************************/
/*                                                                          */
/*  Copyright (c) 2001-2009 Shanghai Advanced Digital Technology Co., Ltd   */
/*                                                                          */
/* PROPRIETARY RIGHTS of Shanghai Advanced Digital Technology Co., Ltd      */
/* are involved in the subject matter of this material. All manufacturing,  */
/* reproduction, use, and sales rights pertaining to this subject matter    */
/* are governed by the license agreement. The recipient of this software    */
/* implicitly accepts the terms of the license.                             */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/* FILE NAME                                                                */
/*      mgdef.h                                                             */
/*                                                                          */
/* VERSION                                                                  */
/*		v3.1.2                                                              */
/*                                                                          */
/* AUTHOR                                                                   */
/*      Eileen Geng                                                         */
/*      Gaston Shi                                                          */
/*                                                                          */
/****************************************************************************/


#ifndef __MG_DEF_H
#define __MG_DEF_H


/* System defines, do NOT change!!! */
/* ===> */
typedef unsigned char				MG_U8;
typedef unsigned short				MG_U16;
typedef unsigned int				MG_U32;
typedef signed char					MG_S8;
typedef signed short				MG_S16;
typedef signed int					MG_S32;
typedef void						MG_VOID;
				
#define MG_TRUE						1
#define MG_FALSE					0
#define MG_NULL    					((MG_VOID *)0)

#define MG_BIT_PEC_INVALID			0x80
#define MG_BIT_CARD_PARTNER			0x20
#define MG_BIT_STB_PARTNER			0x10

/*wangjian add on 20141216*/
#define MG_CWUPD_BIT_ODD  		(0x01)
#define MG_CWUPD_BIT_EVEN   	(0x02)

/*wangjian add on 20141219*/
#define MG312_TYPE_PPC   (0)
#define MG312_TYPE_PPV   (1)
/*wangjian add end.*/

typedef enum
{
	MG_LIB_TEST,
	MG_LIB_FORMAL
}MG_LIB_STA;

/*wangjian add on 20141216*/
typedef enum
{ 
	CA_MG_SUCCESS = 0,
	CA_MG_ERR_SCARD_NOT_ACT,
	CA_MG_ERR_ZONE_CODE,
	CA_MG_ERR_NO_MONEY,
	CA_MG_ERR_NOT_AUTHORED,
	CA_MG_ERR_CARD_LOCKED,
	CA_MG_ERR_APP_LOCKED,
	CA_MG_ERR_PIN_AUTHEN,
	CA_MG_ERR_EXTER_AUTHEN,
	CA_MG_ERR_DONOT_TELEVIEW_PPV,
	CA_MG_ERR_CARD_NOTFOUND,
	CA_MG_ERR_CARD_NOTMATCH,
	CA_MG_ERR_UPDATE_GK,

	CA_MG_EMM_APP_STOP,
	CA_MG_EMM_APP_RESUME,
	CA_MG_EMM_PROG_UNAUTH,
	CA_MG_EMM_PROG_AUTH,
	CA_MG_EMM_EN_PARTNER,
	CA_MG_EMM_DIS_PARTNER,
	CA_MG_EMM_SONCARD,
	CA_MG_EMM_UPDATE_GNO,	
	CA_MG_EMM_UPDATE_GK,
	
	CA_MG_EMM_CHARGE,
	CA_MG_EMM_URGENT_SERVICE,
	CA_MG_EMM_MODIFY_ZONECODE,
	CA_MG_EMM_UPDATE_COUT,
	CA_MG_EMM_UPDATE_CRT,
	CA_MG_ID_MAX = CA_MG_EMM_UPDATE_CRT
}MG_CA_MG_STATUS;

/*wangjian add end.*/

typedef enum
{
	MG_ERR_SCARD_NOT_ACT			=-60, /*请激活子卡*/
	MG_ERR_SCARD_DISABLED,               /*请激活子卡*/
	MG_ERR_ZONE_CODE,                    /*智能卡区域码错误*/
	MG_ERR_NOT_RECEIVED_SPID		=-50, /*调试使用, 无需显示*/
	MG_ERR_AUTHENTICATION,                  /*调试使用, 无需显示*/
	MG_ERR_NO_MONEY					=-40,  /*智能卡余额不足*/
	MG_ERR_NOT_AUTHORED,                 /*本节目未授权*/
	MG_ERR_CARD_LOCKED,            /*智能卡锁定*/
	MG_ERR_APP_LOCKED,           /*服务锁定*/
	MG_ERR_SC_PARTNER,             /*调试使用, 无需显示*/
	MG_ERR_PIN_AUTHEN,           /*机卡配对错误*/
	MG_ERR_CARD_NOT_READY,         /*调试使用, 无需显示*/
	MG_ERR_EXTER_AUTHEN,        /*智能卡认证失败*/
	MG_ERR_INTER_AUTHEN,        /*调试使用, 无需显示*/
	MG_ERR_GET_CARD_INFO,         /*调试使用, 无需显示*/
	MG_ERR_GET_CARD_DATA,        /*调试使用, 无需显示*/
	MG_ERR_CALCU_GKAK,           /*调试使用, 无需显示*/
	MG_ERR_NEED_GKUPD				=-20,  /*调试使用, 无需显示*/
	MG_ERR_NOREG_CARD,          /*调试使用, 无需显示*/
	MG_ERR_ILLEGAL_CARD,      /*调试使用, 无需显示*/
	MG_ERR_DONOT_TELEVIEW_PPV,    /*PPV节目,请按键订购*/
	MG_ERR_QUESTION_TELEVIEW_PPV,  /*PPV节目,请按键订购*/
	MG_ERR_CARD_NOTFOUND,      /*请插入智能卡*/
	MG_ERR_CARD_NOTMATCH, /* v3.1.2 added, Eileen Geng */   /*智能卡不匹配*/
	MG_ERR_UPDATE_GK, /* v3.1.2 added, Eileen Geng */  /*智能卡升级失败*/
	MG_FAILING						=-1, /*调试使用, 无需显示*/
	MG_SUCCESS						=0,   /*调试使用, 无需显示*/
	MG_EMM_APP_STOP					=1,   /*智能卡暂停服务*/
	MG_EMM_APP_RESUME,            /*智能卡恢复服务*/
	MG_EMM_PROG_UNAUTH,             /*取消节目订购*/
	MG_EMM_PROG_AUTH,           /*增加节目订购*/
	MG_EMM_EN_PARTNER,          /*启用机卡配对*/
	MG_EMM_DIS_PARTNER,        /*取消机卡配对*/
	MG_EMM_UPDATE_GNO,        /*智能卡组号更新*/
	MG_EMM_CHILD_UNLOCK,        /*调试使用, 无需显示*/
	MG_EMM_MSG,              /*(屏显信息)*/
	MG_EMM_UPDATE_GK,            /*智能卡升级，请拔插卡*/
	MG_EMM_EMAIL,              /*(显示邮件)*/
	MG_EMM_CHARGE,                  /*智能卡成功充值xx.xx元*/
	MG_EMM_LOADER,           /*调试使用, 无需显示*/
	MG_EMM_FIGEIDENT,          /*(屏幕角上显示卡号3分钟)*/
	MG_EMM_SONCARD,              /*设置子母卡*/
	MG_EMM_URGENT_SERVICE,         /*(紧急服务)*/
	MG_EMM_MODIFY_ZONECODE,    /*智能卡区域码更新*/
	MG_EMM_UPDATE_COUT,                 /*智能卡特征值更新*/
	MG_EMM_OSD,                  /*(屏显滚动)*/
	MG_EMM_UPDATE_CRT              /*授权更新*/
}MG_STATUS;

extern MG_VOID		MG_Get_STB_Serial(MG_U8 *MG_STBSerials);
extern MG_U8		MG_Get_Card_Ready(MG_VOID);
extern MG_U8 		MG_Get_Lock_Status(MG_VOID);
extern MG_U8		MG_Get_PinErrCount(MG_VOID);
extern MG_U8 		MG_Get_Partner_Status(MG_VOID);
extern MG_VOID 		MG_Get_Card_SN(MG_U8 *MG_CardSN);
extern MG_U8 		MG_Get_Card_Version(MG_VOID);
extern MG_VOID 		MG_Get_CAS_ID(MG_U8 *MG_CASID);
extern MG_VOID		MG_Get_Card_Issue_Date(MG_U8 *MG_IssueDate);
extern MG_VOID		MG_Get_Card_Expire_Date(MG_U8 *MG_ExpireDate);
extern MG_VOID 		MG_Get_Group_ID(MG_U8 *MG_GID);
extern MG_VOID		MG_Get_MSCard_Info(MG_U8 *MG_MSCardInfo);
extern MG_U16 		MG_Get_Card_Key_Version(MG_VOID);
extern MG_U16		MG_Get_UCounter(MG_VOID);
extern MG_U16		MG_Get_GCounter(MG_VOID);
extern MG_U16		MG_Get_ACounter(MG_VOID);
extern MG_S32 		MG_Get_Card_Balance(MG_VOID);
extern MG_U16 		MG_Get_Card_OverDraft(MG_VOID);
extern MG_U8 		MG_Get_Current_Program(MG_VOID);
extern MG_U8 		MG_Get_Program_Type(MG_U8 MG_PVName);
extern MG_U16 		MG_Get_Program_Price(MG_U8 MG_PVName);
extern MG_U8*		MG_Get_Command_Data(MG_VOID);
extern MG_VOID 		MG_Get_Odd_CW(MG_U8 *MG_OCW);
extern MG_VOID 		MG_Get_Even_CW(MG_U8 *MG_ECW);
extern MG_VOID 		MG_Set_CurTeleviewPPVTag(MG_U8 MG_State);
extern MG_U8 		MG_Get_TeleviwPPVTag(MG_U8 MG_PVName);
extern MG_VOID 		MG_Clean_LastPNo_Info(MG_VOID);
extern MG_U8*		MG_Get_Mail_Head(MG_VOID);
extern MG_U8*		MG_Get_Mail_Body(MG_VOID);
/*extern MG_STATUS	MG_Check_Card( MG_U8 *MG_ATR);*/ /* v3.1.2 deleted, Eileen Geng */
extern MG_VOID 		MG_Info_CardInserted(MG_U8* MG_ATR,MG_U8 *MG_STBSerials); /* v3.1.2 added, Eileen Geng */
extern MG_VOID 		MG_Info_CardRemoved(MG_VOID);
extern MG_VOID 		MG_Init_CAS(MG_LIB_STA MG_Lib_Status); /* v3.1.2 modified, Eileen Geng */
extern MG_U32 		MG_Get_ChargeValue(MG_VOID);
extern MG_VOID 		MG_Get_Card_ID(MG_U8 *MG_CardID);
extern MG_U8 		MG_Get_Program_Auth(MG_U8 MG_PVName);
extern MG_U8* 		MG_Get_LoaderInfo( MG_VOID);
extern MG_U16		MG_Get_CASoft_Version(MG_VOID);
extern MG_VOID		MG_Get_ServiceID(MG_U8 *MG_ServiceID);
extern MG_U8		MG_Get_ZoneCode(MG_VOID);
extern MG_VOID		MG_Get_EigenValue(MG_U8 *MG_Evalue);

/*extern MG_STATUS MG_CAS_System_Register(MG_U8 *MG_STBSerials, MG_LIB_STA MG_Lib_Status);*/ /* v3.1.2 deleted, Eileen Geng */
extern MG_STATUS MG_CAS_EMM_U_Process(MG_U8 *MG_EMM_U, MG_U8 *MG_pEMMResult);
extern MG_STATUS MG_CAS_EMM_G_Process(MG_U8 *MG_EMM_G, MG_U8 *MG_pEMMResult);
extern MG_STATUS MG_CAS_EMM_A_Process(MG_U8 *MG_EMM_A, MG_U8 *MG_pEMMResult);
extern MG_STATUS MG_CAS_EMM_GA_Process(MG_U8 *MG_EMM_C, MG_U8 *MG_pEMMResult);
extern MG_STATUS MG_CAS_EMM_C_Process(MG_U8 *MG_EMM_C, MG_U8 *MG_pEMMResult);
extern MG_STATUS MG_CAS_ECM_Process(MG_U8 *MG_ECM, MG_U8 *MG_pCWUpd, MG_U8 *MG_pBalUpd,MG_U8 MG_ZoneCode);
/*extern MG_STATUS MG_CAS_Update_GK(MG_VOID);*/ /* v3.1.2 deleted, Eileen Geng */
/* <=== */


/* Added self defines here */
/*wangjian add on 20141216*/
#define MG312_EMM_U_Process	  	MG_CAS_EMM_U_Process
#define MG312_EMM_G_Process	 	MG_CAS_EMM_G_Process
#define MG312_EMM_A_Process	 	MG_CAS_EMM_A_Process
#define MG312_EMM_C_Process	 	MG_CAS_EMM_C_Process
#define MG312_ECM_Process 		MG_CAS_ECM_Process
/*wangjian add end.*/

/* Eileen Geng */
/* ... */
/* Gaston Shi */

#endif


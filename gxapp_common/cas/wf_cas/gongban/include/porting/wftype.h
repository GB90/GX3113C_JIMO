#ifndef	__wftype_h
#define __wftype_h
/*
**=============================================================================
* Copyright (C) 2003,ChengDu Wanfa Catv & Communication equipment co.,ltd
* All rights reserved.
* File name           : wftype.h
* Abstract            : Basic type & constant define
* Current version     : V1.0
* Init version        : V1.0
* Author              : ZhouYong
* Date created        : 2003-04-20
* Date finished       : 2003-04-20
* Modification history: Mender     Date        Description
*                       ZhouYong   2013-04-10  Adapt to the dcas
*                       ZhouYong   2004-10-27  Command code is deleted for STB
*                       ZhouYong   2004-10-27  Operate code is added for CAS
* 
**=============================================================================
*/

/* Constant for Type */
#ifndef	CONST
#define CONST const
#endif

#ifndef	TRUE
#define TRUE 1
#endif

#ifndef	FALSE
#define FALSE 0
#endif

#ifndef	VOID
#define VOID void
#endif

#ifndef	CHAR
#define CHAR char
#endif

#ifndef	BOOL
#define BOOL unsigned char
#endif

#ifndef	UINT
#define UINT unsigned int
#endif

#ifndef	S08
#define S08 signed char
#endif

#ifndef	S16
#define S16 signed short
#endif

#ifndef	S32
#define S32 signed long
#endif

#ifndef	U08
#define U08 unsigned char
#endif

#ifndef	U16
#define U16 unsigned short
#endif

#ifndef	U32
#define U32 unsigned long
#endif

#if defined(__C51__) || defined(__CX51__)
#undef CONST
#define CONST	const far
#define PARAM_PAST_IN_STACK	large reentrant
#else
#define PARAM_PAST_IN_STACK
#endif

/* Ca table id,过滤CMT时使用 */
#define WFCAS_EMMTABLE_ID	0x8f
#define WFCAS_ECMTABLE_ID1	0x80
#define WFCAS_ECMTABLE_ID2	0x81

/* Operate code for CAS */
#define WFCAS_OP_OPEN_BASE			0x00/* 读入智能卡中的文件 */
#define WFCAS_OP_CLOSE_BASE			0x01/* 关闭智能卡中的文件 */
#define WFCAS_OP_SAVE_FILE			0x02/* 保存文件 */
#define WFCAS_OP_UPDATE_KEY			0x03/* 更新文件密钥 */
#define WFCAS_OP_PARTNERSHIP		0x04/* 机卡配对 */
#define WFCAS_OP_PROG_ENTITLE		0x05/* 节目授权 */
#define WFCAS_OP_UPTATE_TIME		0x06/* 更新时间 */
#define WFCAS_OP_DESCRAMBLE			0x07/* 解扰 */
#define WFCAS_OP_PARSE_CADATA		0x08/* 分析CA相关表格-CAT,PMT,CMT */
#define WFCAS_OP_CHILD_ENTITLE		0x09/* 母机对子机授权 */
#define WFCAS_OP_ZONE_SET			0x0A/* 设置区域码 */
#define WFCAS_OP_MODIFY_USERPROP	0x0B/* 修改智能卡用户属性 */
#define WFCAS_OP_CARD_INIT			0x0C/* 初始化DCAS */

/* Constant for Error code */
#define	WFERR_COMMON_NO			0x00000000L/* 没有错误 */
#define	WFERR_COMMON_PARAMETER	0x00000001L/* 函数的参数非法 */
#define	WFERR_COMMON_MEMALLOC	0x00000002L/* 分配存储器失败 */
#define	WFERR_COMMON_MEMFREE	0x00000003L/* 释放存储器失败 */
#define	WFERR_COMMON_ILLEAGAL	0x00000004L/* 非法的操作 */
#define	WFERR_COMMON_VERIFY		0x00000005L/* 校验失败 */
#define	WFERR_COMMON_MEMEXEC	0x00000006L/* 内存异常 */

#define	WFERR_SMART_SW1SW2		0x00010000L/* 执行命令时状态字不等于0x9000,低16位为状态字SW1SW2 */
#define	WFERR_SMART_RWSIZE		0x00020000L/* 实际响应字节数比要求的少 */
#define	WFERR_SMART_ABSENT		0x00020001L/* 没有插入智能卡 */
#define	WFERR_SMART_INVALID		0x00020002L/* 非法的智能卡 */
#define	WFERR_SMART_DRIVER		0x00021000L/* 智能卡驱动程序错误从这里开始编号，且小于WFERR_FILE_FORMAT */

#define	WFERR_FILE_FORMAT		0x00030000L/* 文件格式错误 */
#define	WFERR_FILE_CONFLICT		0x00030001L/* 插入冲突 */
#define	WFERR_FILE_FULL			0x00030002L/* 文件已满 */
#define	WFERR_FILE_EMPTY		0x00030003L/* 文件已空 */
#define	WFERR_FILE_QUERY		0x00030004L/* 未能查询到记录 */

#define	WFERR_CAS_ABSENT		0x00040000L/* CAS尚未初始化 */
#define	WFERR_CAS_PRESENT		0x00040001L/* CAS已经初始化 */
#define	WFERR_CAS_INVALIDSTB	0x00040002L/* 非法的机顶盒,或者机卡不匹配 */
#define	WFERR_CAS_UNLICENSED	0x00040003L/* 节目未经授权 */
#define	WFERR_CAS_OUTOFPERIOD	0x00040004L/* 节目不在授权的期间内 */
#define	WFERR_CAS_FORBIDDEN		0x00040005L/* 禁止解扰 */
#define	WFERR_CAS_COMMAND		0x00040006L/* 非法的前端命令 */
#define	WFERR_CAS_LENGTH		0x00040007L/* 命令的参数长度不正确 */
#define	WFERR_CAS_ADDRESS		0x00040008L/* 前端寻址没有寻到本机 */
#define	WFERR_CAS_BALANCE		0x00040009L/* 余额不足 */
#define	WFERR_CAS_UNMATCHED		0x0004000AL/* 子机与母机不匹配 */
#define	WFERR_CAS_EXPIRATION	0x0004000BL/* 过期的子机授权 */
#define	WFERR_CAS_ZONE			0x0004000CL/* 错误的区域 */
#define	WFERR_CAS_PROTOCOL		0x0004000DL/* CMT协议不正确 */
#endif

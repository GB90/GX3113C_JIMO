#ifndef _GOSCAS_STBDATAPUBLIC_H
#define _GOSCAS_STBDATAPUBLIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
	PARE_STB_INSERT_MASTER,
	PARE_STB_INSERT_SLAVE
}Master_Pare_Slave;

#ifndef HRESULT
#define	HRESULT			long
#endif

#ifndef bool
#define bool		   unsigned char
#endif

#ifndef true
#define true           1                                          
#endif

#ifndef false
#define false          0   
#endif

//ADD BY GOS
#define GOSCA_MAX_EMAIL_TITLE					20	
#define GOSCA_MAX_EMAIL_ID_COUNT				20						
#define GOSCA_MAX_EMAIL_CONTENT					(1024+1)				

#define GOS_MAX_ENTITLES						256
/*----------------------------------------------------------------------*/
/*                       下面是长度值定义                               */
/*----------------------------------------------------------------------*/
#define GOSCA_MAC_LENGTH						8

#define GOSCA_MAXLEN_TVSNAME					20		/*最长运营商名称长度*/
#define GOSCA_MAXLEN_PINCODE					8		/*最长的PIN码长度*/
#define GOSCA_MAXLEN_MANUFACTURERNAME			20		/*最长的CAS厂商名字长度*/ 
#define GOSCA_LEN_SMARTCARD_SN					32		/*最长只能卡序列号长度（外部卡号）*/

#define MAX_FILTER_BYTES							8		/*过滤器长度*/
#define	GOSCA_STB_MAXNUMBER_ENTITLPRODUCT			250		/*一张卡保存的最大授权产品的个数*/
#define GOSCA_STB_MAXLEN_PROMPTMESSAGE				255		/*每条提示信息的最大长度*/
#define GOSCA_MAXLEN_SERVICEPERECM					2		/*每个ECM支持的最大Service数目*/

#define GOSCA_MAXLEN_PRODUCT_NAME					21		/*最长的产品名称,最多20个字符+\0*/
#define	GOSCA_MAXLEN_EMAIL_SENDERNAME				11		/*E-mail发送者姓名，最多11个字符+\0*/
#define	GOSCA_MAXLEN_EMAIL_CONTENT					1025	/*E-mail最大长度，最多1024个字符+\0*/
#define	GOSCA_MAXLEN_EMAIL_SHORT_CONTENT			20

/*请求收取私有数据的请求号*/
#define GOSCA_STB_ECM_SLOT						1
#define GOSCA_STB_EMM_Private_SLOT				2
#define GOSCA_STB_EMM_Public_SLOT				3

/*EMM接受等待最大时间*/
#define	GOSCAS_EMM_RECEIVING_WAIT_TIME			0

/*智能卡状态定义*/
#define GOSCAS_SC_OUT							0		/*智能卡不在读卡器中*/
#define GOSCAS_SC_IN							1		/*智能卡在读卡器中，工作正常*/

/*Emm过滤器状态定义*/
#define GOSCAS_EMMFILTER_RECEIVING				1		/*Emm过滤器正在接受Emm数据*/
#define GOSCAS_EMMFILTER_FREE					0		/*Emm过滤器空闲*/

/*机卡配对状态定义*/
#define GOSCAS_SC_PAIR_OK						0		/*智能卡与当前机顶盒配对*/
#define GOSCAS_SC_NO_Pairing					1		/*智能卡没有与任何机顶盒配对*/
#define GOSCAS_SC_PAIR_OTHER					2		/*智能卡与当前机顶盒没有配对*/

/*收看级别定义*/
#define GOSCAS_Rating_3							3		/*3岁*/
#define GOSCAS_Rating_4							4		/*4岁*/
#define GOSCAS_Rating_5							5		/*5岁*/
#define GOSCAS_Rating_6							6		/*6岁*/
#define GOSCAS_Rating_7							7		/*7岁*/
#define GOSCAS_Rating_8							8		/*8岁*/
#define GOSCAS_Rating_9							9		/*9岁*/
#define GOSCAS_Rating_10						10		/*10岁*/
#define GOSCAS_Rating_11						11		/*11岁*/
#define GOSCAS_Rating_12						12		/*12岁*/
#define GOSCAS_Rating_13						13		/*13岁*/
#define GOSCAS_Rating_14						14		/*14岁*/
#define GOSCAS_Rating_15						15		/*15岁*/
#define GOSCAS_Rating_16						16		/*16岁*/
#define GOSCAS_Rating_17						17		/*17岁*/
#define GOSCAS_Rating_18						18		/*18岁*/

//email 显示信息。
#define GOSCAS_EMAIL_NEW						0		/*新邮件通知*/
#define GOSCAS_EMAIL_NEW_NO_ROOM				1		/*有新邮件，但Email空间不够*/
#define GOSCAS_EMAIL_NONE						0xFF	/*隐藏标志*/

//email 状态。	
#define GOSCAS_EMAIL_STATUS_INI					0		/*初始值*/
#define GOSCAS_EMAIL_STATUS_GOT					1		/*被get过了*/
#define GOSCAS_EMAIL_STATUS_READ				2		/*被读过了*/

/*----------------------------------------------------------------------*/
/*                       下面是返回值定义                               */
/*----------------------------------------------------------------------*/

/*机顶盒返回值基值定义*/
#define GOSCAERR_BASE_STB						0xF1000000

/*返回值定义*/
#define GOSCA_OK								0 
#define GOSCA_FAILED							(-1)

#define GOSCAERR_STB_POINTER_INVALID			(GOSCAERR_BASE_STB + 1)		/*输入的指针无效*/
#define GOSCAERR_STB_DATA_LEN_ERROR				(GOSCAERR_BASE_STB + 2)		/*输入的数据长度不合法*/
#define GOSCAERR_STB_TVS_NOT_FOUND				(GOSCAERR_BASE_STB + 3)		/*没有找到符合要求的运营商*/
#define GOSCAERR_STB_EXCEED_MAX_COUNT			(GOSCAERR_BASE_STB + 4)		/*超过了允许的最大的个数*/
#define GOSCAERR_STB_KEY_NOT_FOUND				(GOSCAERR_BASE_STB + 5)		/*没有找到解密密钥*/
#define GOSCAERR_STB_PIN_INVALID				(GOSCAERR_BASE_STB + 6)		/*输入的PIN码无效*/
#define GOSCAERR_STB_PIN_LOCKED					(GOSCAERR_BASE_STB + 7)		/*PIN码被锁定*/
#define GOSCAERR_STB_CMD_INVALID				(GOSCAERR_BASE_STB + 8)		/*MAC校验错误*/
#define GOSCAERR_STB_VER_EXPIRED				(GOSCAERR_BASE_STB + 9)		/*输入的版本没有IC卡中的大，即数据已经过期*/
#define GOSCAERR_STB_EXCEED_MAX_LEN				(GOSCAERR_BASE_STB + 10)	/*运营商的名称超过最大的长度*/
#define GOSCAERR_STB_NO_SPACE					(GOSCAERR_BASE_STB + 11)	/*已经没有空间*/
#define GOSCAERR_STB_IS_PAIRED					(GOSCAERR_BASE_STB + 12)	/*智能卡与当前机顶盒对应*/
#define GOSCAERR_STB_NOT_PAIRED					(GOSCAERR_BASE_STB + 13)	/*智能卡与当前机顶盒没有对应*/
#define GOSCAERR_STB_COUNT_ISZERO				(GOSCAERR_BASE_STB + 14)	/*输入的Count为0. */
#define GOSCAERR_STB_WORKTIME_INVALID			(GOSCAERR_BASE_STB + 15)	/*工作时段设置不合法，可能是时间不在00:00:00~23:59:59之间，或者开始时间大于结束时间。*/
#define	GOSCAERR_STB_OVERFLOW					(GOSCAERR_BASE_STB + 16)	
#define	GOSCAERR_STB_CARD_INVALID				(GOSCAERR_BASE_STB + 17)	/*智能卡无效*/
#define	GOSCAERR_STB_Rating_LOW					(GOSCAERR_BASE_STB + 18)	/*智能卡设置的收看级别比当前收看的节目低*/
#define GOSCAERR_STB_UNKNOWN					(GOSCAERR_BASE_STB + 19)	/*未知错误*/

#define	GOSCAERR_STB_EMAILBOX_EMPTY				(GOSCAERR_BASE_STB + 20)	/*E-mail信箱为空*/
#define	GOSCAERR_STB_NO_EMAIL					(GOSCAERR_BASE_STB + 21)	/*没有此E-mail*/
#define	GOSCAERR_STB_EMAILBOX_FULL				(GOSCAERR_BASE_STB + 22)	/*E-mail信箱满*/
#define	GOSCAERR_STB_NO_SECTION					(GOSCAERR_BASE_STB + 23)	/*没有收到完整Section*/

#define	GOSCAPAIR_GETINF_ERROR					(GOSCAERR_BASE_STB + 24)	/*获取子母卡信息出错*/
#define	GOSCAPAIR_NOPAIR_ID						(GOSCAERR_BASE_STB + 25)	/*母卡的子卡号和子卡的母卡号不匹配*/
#define	GOSCAPAIR_SMART_MOTHER					(GOSCAERR_BASE_STB + 26)	/*当前所读卡是母卡*/
#define	GOSCAPAIR_SMART_SON						(GOSCAERR_BASE_STB + 27)	/*当前所读卡是子卡*/

//子母卡配对错误信息
#define	GOSCAPAIR_GETOPERATION_ERROR			(GOSCAERR_BASE_STB + 30)	/*获取运营商发卡信息出错，可能卡没有接受到发卡信息或卡通信错误*/
#define	GOSCAPAIR_SMART_TYPE_ERROR				(GOSCAERR_BASE_STB + 31)	/*所插入的卡和需要操作的流程不匹配，如流程中要求插入子卡而实际检测插入的不是子卡*/
#define	GOSCAPAIR_SMART_GETMASTERINF_ERROR		(GOSCAERR_BASE_STB + 32)	/*获取主卡（母卡）配对信息操作失败！*/
#define	GOSCAPAIR_SMART_GETSLAVEINF_ERROR		(GOSCAERR_BASE_STB + 33)	/*获取从卡（子卡）配对信息操作失败！*/
#define	GOSCAPAIR_SMART_NOPAREID_ERROR			(GOSCAERR_BASE_STB + 34)	/*主从卡配对号不一致！*/
#define	GOSCAPAIR_MSPARE_FAILURE_ERROR			(GOSCAERR_BASE_STB + 35)	/*主从卡配操作失败！*/
#define	GOSCAPAIR_PROCESS_TYPE_ERROR			(GOSCAERR_BASE_STB + 36)	/*操作流程步骤不合法！*/

//强制换台参数
#define GOSCA_MAXNUM_COMPONENT	5
#define GOSCA_MAXLEN_LOCKMESS      	40

/*机顶盒遥控器不能取消的显示信息列表*/
#define GOSCA_No_OSDPromt						0			/*无OSD提示*/	

#define	GOSCA_SMART_STOP						1			/*卡以被禁用*/
#define GOSCA_SMART_AREA_NOPAIR					2			/*流卡区域不匹配*/
#define GOSCA_SMART_IPPT_SWITCH					3			/*IPPT用户提示*/
#define	GOSCA_SMART_STB_NOPAIR					4			/*机卡不配对*/
#define	GOSCA_SMART_PROG_NOENTITLE				5			/*节目类未授权*/
#define	GOSCA_SMART_PROG_NOENTITLE_IPPT			6			/*节目类未授权*/
#define	GOSCA_SMART_MOTHER_SON_PAIR				7			/*子母卡需从新配对*/
#define	GOSCA_SMART_MOTHER_SON_TIMEOUT			8			/*子母卡配对超时*/
#define	GOSCA_SMART_PAIR_OK						10			/*机卡对应OK*/
#define	GOSCA_SMART_LAST_DAY					11			/*最后提示天数*/

#define GOSCA_SMART_AREA_LOCK					0x10		/*卡区域锁定 */
#define GOSCA_SMART_CARD_LOCK					0x11		/*卡已被锁定 */
#define GOSCA_SMART_IPPT						0x12		/*IPPT提示*/
#define GOSCA_SMART_LIMIT_WATCH					0x13		/*卡限制收看*/
#define GOSCA_SMART_NO_WORK_TIME				0x14		/*卡不在工作时间段*/

#define GOSCA_SMART_OUT							0x20		/*节目加扰，请插卡*/
#define	GOSCA_SMART_UNKNOWN_ERROR				0xFF		/*未知错误*/

/*----------------------------------------------------------------------*/
/*                       下面是一些数据结构定义                         */
/*----------------------------------------------------------------------*/
/*信号量定义*/

typedef unsigned int  GOSCA_Semaphore ;

typedef void (*pThreadFunc) (void);/*线程函数地址类型*/

typedef struct _SGOSDate{
	unsigned int 	m_wYear;
	unsigned int 	m_wMonth;
	unsigned int 	m_wDay;
}SGOSDate;

typedef struct _SGOSTime{
	unsigned int 	m_wHour;
	unsigned int 	m_wMinute;
	unsigned int 	m_wSecond;
}SGOSTime;

typedef struct _SGOSDateTime{
	SGOSDate   	m_tDate;
	SGOSTime  	m_tTime;
}SGOSDateTime;

typedef struct _SManufacturerInfo{
	unsigned long	m_dwCardID;							/*卡内部ID*/
	unsigned char	m_byszSerialNO[GOSCA_LEN_SMARTCARD_SN];
	unsigned long	m_dwSTBCASVer;							/*机顶盒CAS模块的版本号*/
	char			m_szSCCASManuName[GOSCA_MAXLEN_MANUFACTURERNAME];	/*CAS供应商的名称*/
	unsigned long	m_dwSCCASVer;							/*智能卡CAS的版本号*/
	unsigned long	m_dwSCCOSVer;							/*SC的版本号，也就是SC中COS的版本号*/
	unsigned long	m_tSCExpireDate;						/*智能卡的有效期，由STB初始化时判断，如果过期，给用户一个提示，但可以继续使用。*/
}SManuInfo;

typedef struct _GOSCAServiceEntitle_Ex{
	unsigned short 		m_wProductID;
	unsigned long		m_tStartTime;				/*开始时间*/
	unsigned long		m_tEndTime;					/*结束时间*/
}SGOSCAServiceEntitle_Ex;		/*普通产品授权*/

typedef struct _GOSCAServiceInfo{
	unsigned short 	m_wEcmPid;			/*主键*/
	unsigned short 	m_wOriNetworkID;	/*用于2.0扩展*/
	unsigned short 	m_wTSID;			/*用于2.0扩展*/
	unsigned char   m_byServiceCount;	/*用于2.0扩展*/
	unsigned short 	m_wServiceID;	/*用于2.0扩展*/
}SGOSCAServiceInfo;


typedef struct _SCAS2STBStatusInfo{
	unsigned char 	m_bStatusTpyeFlag;
	unsigned char 	m_bStatusInfo[6];
}SCAS2STBStatusInfo;

typedef struct _GOSPIN{
	unsigned char	m_byLen;
	unsigned char	m_byszPin[GOSCA_MAXLEN_PINCODE];
}SGOSPIN;

typedef struct _GOSCAEmailVersion{
	unsigned char	m_state;
	unsigned long	m_version;
}GOSCAEmailVersion;

typedef struct _GOSCAEmailTotal{
	unsigned char		m_Email_All;
	unsigned char		m_Email_New;
	GOSCAEmailVersion	m_Email_versions[GOSCA_MAX_EMAIL_ID_COUNT+1];
}GOSCAEmailTotal;

//ADD BY GOS
typedef struct _SCardWorkInfo{
	unsigned char	bCardPinStatus;
	unsigned char 	bWatchRating;
	unsigned char 	bMatureDays;
	SGOSTime 		tStarttime;
	SGOSTime 		tEndtime;
	unsigned char 	m_bIPPTWorkMode;
	unsigned char 	m_bReserved0;
	unsigned char 	m_bReserved1;
	unsigned char 	m_bReserved2;
	unsigned char 	m_bReserved3;
	unsigned char 	m_bReserved4;		
}SCardWorkInfo;

typedef struct _SOperatorInfo{
	unsigned long  	m_dwMotherCardID;  //1-4
	unsigned char   m_bStandardFlag;     //5
	unsigned long   m_dwDecryptCWTimes; //6-9
	unsigned long   m_dwIntervalTime;   //10-13
	unsigned int    m_wRunArea;         //14-15
	unsigned char   m_bUserVIPClass;    //16
	unsigned long   m_dwUserArea;       //17-20 
	unsigned char	m_bTVSName[GOSCA_MAXLEN_TVSNAME]; //21-40
	unsigned char   m_bTVSOtherInfo[GOSCA_MAXLEN_TVSNAME]; //41-60	
}SOperatorInfo;

typedef struct _GOSCAEmailHead{
	unsigned char		m_Slot;
	unsigned long		m_version;
	unsigned char		m_Title_len;
	unsigned char		m_Title[GOSCA_MAX_EMAIL_TITLE];
}GOSCAEmailHead;

typedef struct _GOSCAEmailContent{
	unsigned char		m_Slot;
	unsigned long		m_version;									
	unsigned int		m_Content_len;
	unsigned char		m_Content_Data[GOSCA_MAX_EMAIL_CONTENT];					
}GOSCAEmailContent;

typedef struct _GOSCAMontherSonInfo{
	unsigned long	m_dwMontherCardID;				//母卡卡号
	unsigned long	m_dwSonCardID;					//子卡卡号
	unsigned long	m_dwCardWorkTime;				//当前卡的工作时间
}SGOSCAMontherSonInfo;

typedef struct _SSC2STBConditionPlay{
	unsigned short 	m_wProductID;
	bool			m_bEnd;
	bool			m_bTapingFlag;
	unsigned long	m_tStartTime;
	unsigned long	m_tEndTime;
}SSC2STBConditionPlay;

/*-- 频道锁定信息 --*/
/*-- 节目组件信息 --*/
typedef struct {    /* 组件用于通知机顶盒节目类型及PID等信息，一个节目可能包含多个组件 */
    unsigned short   m_wCompPID;     /* 组件PID */
    unsigned short   m_wECMPID;      /* 组件对应的ECM包的PID，如果组件是不加扰的，则应取0。 */
    unsigned char    m_StreamContent;
    unsigned char    m_CompType;     /* 组件类型 */
    unsigned char    m_byReserved[3];/* 保留 */
}SGOSCAComponent;

/*-- 频道参数信息 --*/
typedef struct {    
    unsigned long    		m_dwFrequency;              /* 频率，BCD码 */
    unsigned long   	 		m_symbol_rate;              /* 符号率，BCD码 */
    unsigned short			m_wPcrPid;                  /* PCR PID */
    unsigned char    		m_Modulation;               /* 调制方式 */
    unsigned char    		m_ComponentNum;             /* 节目组件个数 */
    SGOSCAComponent 		m_CompArr[GOSCA_MAXNUM_COMPONENT];       /* 节目组件列表 */
    unsigned char    		m_fec_outer;                /* 前项纠错外码 */
    unsigned char    		m_fec_inner;                /* 前项纠错内码 */
    char       				m_szBeforeInfo[GOSCA_MAXLEN_LOCKMESS+1]; /* 保留 */
    char       				m_szQuitInfo[GOSCA_MAXLEN_LOCKMESS+1];   /* 保留 */
    char       				m_szEndInfo[GOSCA_MAXLEN_LOCKMESS+1];    /* 保留 */
    unsigned char		lock_flag;                                  //add by wei
}SGOSCALockService;

#ifdef __cplusplus
}
#endif

#endif	


#ifndef _GOSCAS_STBINF_H
#define _GOSCAS_STBINF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "GOSCAS_STBDataPublic.h"

//一下四个接口是为了维护老的集成建议不采用，采用新的接口extern HRESULT GOSCASTB_MasterSlave_PareProcess( Master_Pare_Slave process) ;
extern HRESULT GOSCASTB_GetCardMontherSonInfo(SGOSCAMontherSonInfo *MontherSonInfo);
extern HRESULT GOSCASTB_SetCardSonInfo(SGOSCAMontherSonInfo *MontherSonInfo);
extern HRESULT GOSCASTB_IsMotherCard( ) ;
extern HRESULT GOSCASTB_MotherSon_Pair( ) ;

/*-----------------------------------以下接口是CAS提供给STB调用----------------------------------*/

/*++

功能：
机顶盒启动的时候调用，对CAS模块初始化

参数：
byReaderNo：StreamGuard所属的智能卡读卡器号，一般设置0

返回值：
true     初始化成功
false    初始化失败
--*/
extern bool GOSCASTB_Init(unsigned char byReaderNo) ;

/*++

功能：
机顶盒调用该接口控制库信息的打印开个

参数：bDebugMsgSign
1：表示打开库的打印开个；0：关闭开关

返回值：
无
--*/
extern void GOSCASTB_AddDebugMsgSign(bool bDebugMsgSign) ;

/*++

功能：
主从卡配对接口函数（通常的字母卡配对接口函数）

参数：
无

返回值：
0 表示当前的处理流程OK!其它表示错误代码，详细操作请见附件实例举例：
--*/
extern HRESULT GOSCASTB_MasterSlave_PareProcess( Master_Pare_Slave process) ;

/*++

功能：
机顶盒获取机卡配对后的机号

参数：
无

返回值：
四个字节的机号。如果没有进行过机卡配对。返回0xFFFFFFFF
--*/
extern unsigned long GOSSYS_GetSerNo();

/*++

功能：
机顶盒获取卡工作信息

参数：
*pCardWorkInfo:    获取结构体SCardWorkInfo信息

返回值： ==0 成功
!=0 失败
说明：
pCardWorkInfo->bCardPinStatus <= 1 PIN未锁定，>4PIN已锁定；
pCardWorkInfo->bWatchRating   实际的年龄显示需要加3
pCardWorkInfo->bMatureDays 到期提示天数的设置参数。
pCardWorkInfo->tStarttime  卡工作开始时间
pCardWorkInfo->tEndtime    卡工作结束时间
pCardWorkInfo->m_bIPPTWorkMode(0永不使用；1换台选择使用(暂不支持）；2换台提示；3永远使用，换台不提示）；
pCardWorkInfo->m_bReserved0
pCardWorkInfo->m_bReserved1
--*/
extern HRESULT GOSCASTB_GetCardWorkInfo(SCardWorkInfo* pCardWorkInfo);
/*++

功能：
机顶盒设置卡工作信息

参数：
*pCardWorkInfo:    设置结构体SCardWorkInfo信息，如果对某个结构体变量进行设置
需要先读出该结构体变量，再修改需要设置的结构体变量；

返回值： ==0 成功
!=0 失败
说明：
同上
--*/
extern HRESULT GOSCASTB_SetCardWorkInfo(SCardWorkInfo* pCardWorkInfo);

/*++

功能：
OSD的滚动分两种。包括滚动

参数：
无

返回值：
--*/
extern void GOSCASTB_ShowOSDMsgOver(signed short wDuration);

/*++

功能：
指纹显示完后调用该接口；

参数：
无

返回值：
--*/
extern void GOSCASTB_ShowFingerOver(unsigned long version);

/*++

功能：
机顶盒通过CAS获取系统时间，该接口通常在某些没有TDT表系统中使用，通常不调用该接口；

参数：
*year:    	xxxx年
*month		x月
*day		xx日
*week		星期x
*hour		xx时
*min		xx分

返回值： ==0 成功
!=0 失败
说明：
同上
--*/
extern HRESULT GOSSYS_CasSysTime_Get( unsigned int*  year, unsigned char*month, unsigned char*day, unsigned char* week, unsigned char* hour, unsigned char* min);

/*++

功能：
机顶盒通过CAS获取一个六位的密码，超级密码281201
使用超级密码可以清空密码，该功能是某些客户要求对父母锁远程操作下使用，
及可以归SMS管理；

参数：
*pOut 密码ASC字符串

返回值： ==0 成功
!=0 失败
说明：
同上
--*/
extern HRESULT GOSSYS_Parent_PassWord_Get( unsigned char* pOut );

/*++

功能：
机顶盒通过CAS校验一个六位的密码，超级密码281201
使用超级密码可以清空密码，该功能是某些客户要求对父母锁远程操作下使用，
及可以归SMS管理；

参数：
*pOut 密码ASC字符串

返回值： ==0 成功
!=0 失败
说明：
同上
--*/
extern HRESULT GOSSYS_Parent_PassWord_Check( unsigned char* pIn );

/*++

功能：
机顶盒通过CAS设置一个六位的密码，超级密码281201，
使用超级密码可以清空密码，该功能是某些客户要求对父母锁远程操作下使用，
及可以归SMS管理；

参数：
*pOut 密码ASC字符串

返回值： ==0 成功
!=0 失败
说明：
同上
--*/
extern HRESULT GOSSYS_Parent_PassWord_Set( unsigned char* pIn );

extern HRESULT GOSSYS_Parent_PassWord_Reset( unsigned long date_time, unsigned char version );

/*++

功能：
机顶盒获取CAS库的版本号

参数：
pdwVer:    返回版本号

返回值：
true	成功
false   失败
说明：
返回CA模块的版本号
举例如果是0x00000104意思就是0.00.01.04
--*/
extern bool GOSCASTB_GetVer(long *pdwVer) ;

/*++

功能：
调用CAS模块，获得智能卡内部ID。

参数：
*pID:    返回智能卡号。
*pSoftVer 返回卡的软件版本号
举例如果是0x32意思就是3.2

返回值：
true	成功
false   失败
说明：

--*/
extern bool GOSCASTB_GetCardInfo(unsigned long* pID, unsigned char* pSoftVer) ;

/*++

功能：
调用CAS模块，获得智能卡内部ID扩展接口，建议新的版本使用该接口更能描述卡的详细信息。

参数：
*pID:    返回智能卡号。
*pSoftVer 返回卡的软件版本号，举例如果是0x32意思就是3.2
* param1
* param2
* param3 以上参数见使用说明举例§5.4  卡信息获取扩展接口参数说明 
返回值：
true	成功
false   失败
说明：

--*/
extern bool GOSCASTB_GetCardInfoEx(unsigned long* pID, unsigned char* pSoftVer, unsigned char* param1, unsigned short* param2, unsigned char* param3) ;

/*++
功能：
机顶盒获得CASID后调用该接口判断CASID是否有效，如果有效则返回true，否则返回false

参数：
wCaSystemID:		机顶盒分析SI/PSI信息获得的CASID

返回值：
true  (1)   CASID有效
false (0)   CASID无效
--*/
extern bool GOSCA_ISSystemID(const unsigned int wCaSystemID) ;

/*++
功能：
当用户插卡时机顶盒主程序调用，通知CAS模块

参数：
byReaderNo:		读卡器的编号

返回值：
--*/
extern unsigned char GOSCASTB_SCInsert(void);

/*++
功能：
获取CAS库模块里面卡的状态标识，如果插入了卡但状态
仍然是GOSCAS_SC_OUT，需要对卡复位从新执行一下插拔
卡动作。

参数：
*pState == GOSCAS_SC_OUT 库里面监控卡通信不可用。
*pState == GOSCAS_SC_IN  库里面监控卡通信正常。

返回值：
--*/
extern void GOSCASTB_SCGetState( unsigned char* pState );

/*++

功能：
当用户拔出智能卡时机顶盒主程序调用，通知CAS模块

参数：
返回值：
--*/
extern void GOSCASTB_SCRemove(void) ;

/*++
功能：
	用户转换频道的时候调用这个函数
	设置当前正在收看的节目的详细信息
参数：
pServiceInfos：	SGOSCAServiceInfo结构，描述节目的详细信息。
	byCount:		EcmPid的个数，目前只支持byCount=1
注意：该接口在每次换台前需调用一下，ECM PID = 0x1FFF;换台后分析PMT表信息，如果CASID合法，再调用一次
，ECM PID为实际的PID;

返回值
说明：
当用户换频道时，机顶盒需要调用CAS提供的此函数，将当前节目的原始信息及ECM PID传递给CAS模块，CAS模块会
根据当前频道的信息处理与当前频道相关的操作，如果当前频道为不加扰的频道，则需要将ECM的PID设为0。
--*/
extern void GOSCASTB_SetCurEcmInfos(unsigned char byCount,const SGOSCAServiceInfo* pServiceInfos) ;

/*++

功能：
设置EMMPID，CAS模块将根据设置的EMMPID信息进行EMM数据接收处理。
参数：
pwEmmPids:	CAT表中包含的descriptor中将有CA_system_ID及CA_PID，调用GOSCA_ISSystemID()校验有效的CA_PID即为EMM PID。
nEmmCount：	EmmPid的个数，目前只支持nEmmCount＝1
返回值：

说明:
当机顶盒收到CAT表后，机顶盒需要调用CA提供的此函数，将EMM PID传过来，这样CA程序就可以根据此PID收取
私有表，更新用户卡里的授权及为用户提供增值服务，当设置EMM的PID为0时，CA模块会停止收取EMM。
--*/
extern void GOSCASTB_SetEmmPids(long nEmmCount,const unsigned short* pwEmmPids) ;

/*++

功能：
机顶盒主程序在收到CAS私有数据或者Time_Out，调用这个接口将数据提供给CAS模块处理。
参数：
byReqID：			当前请求收取私有表的请求号，与GOSSTBCA_SetStreamGuardFilter里的byReqID对应
bSuccess：			收取数据有没有成功，成功为true，失败为false
wPID:				接收的数据的流的PID。
byszReceiveData：		收取的私有数据
wLen：				收取到的私有数据的长度
返回值：
true  (1)   有效
false (0)   无效
说明：
byReqID == GOSCA_STB_ECM_SLOT		供给CAS模块的数据类型是ECM
byReqID == GOSCA_STB_EMM_Private_SLOT	供给CAS模块的数据类型是EMM 私有	
byReqID == GOSCA_STB_EMM_Public_SLOT	供给CAS模块的数据类型是EMM 公有	
wLen = Section_lenth		
--*/
extern bool GOSCASTB_StreamGuardDataGet(unsigned char byReqID, bool bSuccess, unsigned short wPID,unsigned short wLen,unsigned char* byszReceiveData) ;

/*++
功能：
判断智能卡的PIN码
参数：
sPin:				PIN码
返回值：
GOSSTB_OK          			成功
GOSCAERR_STB_DATA_LEN_ERROR		PIN码数据长度错误
GOSCAERR_STB_PIN_ERR			输入PIN码错误
GOSCAERR_STB_PIN_LOCKED			PIN被锁定
说明：
HRESULT Check_PinFunc(unsigned char* PINInput_Value){
SGOSPIN  verify_pin;
verify_pin.m_byLen = 3;
verify_pin.m_byszPin[0] = (PINInput_Value[0]<<4) | PINInput_Value[1];
verify_pin.m_byszPin[1] = (PINInput_Value[2]<<4) | PINInput_Value[3];
verify_pin.m_byszPin[2] = (PINInput_Value[4]<<4) | PINInput_Value[5];
if( GOSCASTB_VerifyPin(&verify_pin) != 0 )
{
return -1;
}
return 0;
}			
--*/
extern HRESULT GOSCASTB_VerifyPin(SGOSPIN *p_sPIN) ;

/*++
功能：
	判断卡是否被锁住。

参数：
	pbLocked:					- 输出时true表示锁住了，false，表示没有。
						
返回值：
	公共返回值1					- 详见注释

--*/
extern HRESULT GOSCASTB_IsPinLocked(bool * pbLocked) ;

/*++
功能：
修改智能卡的PIN码
参数：
sNewPin:			   新的PIN码

返回值：
GOSSTB_OK          				成功
GOSCAERR_STB_PIN_LOCKED			PIN被锁定
GOSCAERR_STB_DATA_LEN_ERROR		PIN码数据长度错误

说明:
调用该函数之前要求确保调用过GOSCASTB_VerifyPin()，并且是成功的。

--*/
extern HRESULT GOSCASTB_ModifyPin(SGOSPIN *p_sNewPin) ;

/*++
功能：
获得CAS运营商信息
参数：
pOperation   指向保存返回信息的数组地址
说明:
运营商执行了发卡信息通过EMM发送给卡。相应的信息会显示处理,里面的内容可用选择显示。
if(pOperation->m_dwMotherCardID == 0 )
{
该卡是一张母卡；
}
else
{
该卡是一张子卡，且母卡号是该号；
}

pOperation->m_bStandardFlag		子母卡配对失效判断标准
pOperation->m_dwDecryptCWTimes		解密CW 的次数
pOperation->m_dwIntervalTime/86400	间隔时间(天)
pOperation->m_wRunArea			用于区分运营的地方
pOperation->m_bUserVIPClass		用户等级
pOperation->m_dwUserArea		用于区分用户区域码
pOperation->m_bTVSName			运营商的名称
pOperation->m_bTVSOtherInfo		运营商的其他信息
--*/
extern HRESULT GOSCASTB_GetOperation(SOperatorInfo *pOperation) ;

/*++

功能：
获得普通授权节目购买的情况，用于在机顶盒界面上显示。

参数：
wTVSID:							- 运营商的内部编号。
pEntitleCount					- 输入时为数组的最大个数，输出时为实际获得的个数。
psEntitles:						- 返回的授权数组。

返回值： ==0 成功
!=0 失败
说明:
psEntitles->m_wProductID;				产品号
psEntitles->m_tStartTime;				开始时间
psEntitles->m_tEndTime;					结束时间

--*/
extern HRESULT GOSCASTB_GetServiceEntitles(unsigned char *pEntitleCount, SGOSCAServiceEntitle_Ex* psEntitles) ;

/*++

功能：
锁定中心频点，将中心频点NIT的网络ID送给卡接口，某些客户的分区控制会使用以上功能

参数：
NetID:							- 中心频点NIT网络ID。
注意：该动作需要在开机时只做一次；

返回值： ==0 成功
!=0 失败

--*/
extern HRESULT GOSCASTB_SetNetWorkID(unsigned short  NetID);

/*++
功能:
机顶盒获取E-mail总信息接口。
参数:
*pEmail_Total:		输入时为数组总个数，输出时为机顶盒中E-mail总个数。
返回值： ==0 成功
!=0 失败
说明:

pEmail_Total->m_Email_All;
pEmail_Total->m_Email_New;
pEmail_Total->m_Email_versions[GOSCA_MAX_EMAIL_ID_COUNT].m_state == FREE; NEW ;OLD表示无效索引；新邮件；已读邮件
pEmail_Total->m_Email_versions[GOSCA_MAX_EMAIL_ID_COUNT].m_version 邮件的版本
--*/
extern HRESULT GOSCASTB_GetEmailTotal(GOSCAEmailTotal *pEmail_Total);

/*++
功能:
机顶盒通过邮件的版本获取单个E-mail的标题内容。
参数:
tVersion:			E-mail版本。
*pEmail:			获取的E-mail结构。
返回值： ==0 成功
!=0 失败
--*/
extern HRESULT GOSCASTB_GetEmailTitle(unsigned long tVersion, GOSCAEmailHead *pEmail) ;


/*++
功能:
机顶盒通过邮件的版本获取单个E-mail的内容。
参数:
tVersion:			E-mail版本。
*pEmail:			获取的E-mail结构。
返回值： ==0 成功
!=0 失败
--*/
extern HRESULT GOSCASTB_GetEmailContent(unsigned long tVersion, GOSCAEmailContent *pEmail) ;

/*++
功能:
机顶盒看完E-mail后，通过邮件的版本通知CAS的接口该邮件设置为已读状态。
参数:
tVersion:			E-mail版本。
返回值:
GOSCA_OK:						获取E-mail成功。
--*/
extern HRESULT GOSCASTB_EmailRead(unsigned long tVersion) ;

/*++
功能:
机顶盒通过邮件的版本删除单个E-mail接口。
参数:
tVersion:			E-mail版本。如果为-1，表示删除邮箱中所有email，否则，为具体的id。
返回值:
GOSCA_OK:						删除E-mail成功。
GOSCAERR_STB_NO_EMAIL:			要删除的E-mail不存在。
--*/
extern HRESULT GOSCASTB_DelEmail(unsigned long tVersion) ;

/*++
功能:
机顶盒删除所有E-mail接口。
参数:

返回值： ==0 成功
!=0 失败
--*/
extern HRESULT GOSCASTB_DelAllEmail( void );

/*++
功能:
机顶盒换台时调用该接口。
参数:

返回值： 无
--*/
extern void GOSCASTB_SetCurIPPT(unsigned int nServerID) ;

/*++
功能：
	设置机顶盒定义到期提示的最后剩余多少小时,该接口调用需要库特殊定义有效。
参数：
	
返回值：
	无。
--*/
extern void GOSSTBCA_SetLastDay_PromtHours( unsigned int nHours ) ;

#ifdef __cplusplus
}
#endif  

#endif

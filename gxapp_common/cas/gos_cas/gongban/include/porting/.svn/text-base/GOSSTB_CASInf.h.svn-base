#ifndef _GOSSTB_CASINF_H
#define _GOSSTB_CASINF_H

#ifdef __cplusplus
extern "C" {
#endif
#include "GOSCAS_STBDataPublic.h"


/*----------------------------------以下接口是STB提供给CAS调用--------------------------------------------*/

/*++

功能：
	CAS库在和串口通信失败一次后错误计数会自动加1，当值大于GOSCA_UART_ERROR_MAX
	会调用该接口。接口函数实现卡复位并确认ATR 正确。

参数：
-*/
extern void GOSSTBCA_SetSCReset() ;
/*++


功能：
	返回智能卡当前状态

参数：
	pbySCStatus：返回智能卡状态，为GOSCAS_SC_OUT、GOSCAS_SC_IN两种之一。
--*/
extern void GOSSTBCA_GetSCStatus(unsigned char * pbySCStatus) ;

/*++

功能：
	重新启动机定盒。

参数：
-*/
extern void GOSSTBCA_ReStartSTB()  ;

/*++


功能：
	获得机顶盒分配给CAS的存储空间的起点地址和大小（以字节为单位）

参数：
	ppStartAddr:	     机顶盒分配给CAS的存储空间的开始地址
	lSize:               输出机顶盒分配给CAS的储存空间的大小；
	
返回值：
	GOSCA_OK	成功
	  
说明：
       
--*/
extern HRESULT GOSSTBCA_GetDataBufferAddr(unsigned int* lSize, unsigned long* pStartAddr) ;

/*++

功能：
	读取保存在机顶盒中的信息

参数：
	pStartAddr:	    要读取的存储空间的开始地址
	plDataLen:       输入为要读取的最长数据值
	                返回实际读取的大小
	pData：			存放输出数据

返回值：
	GOSCA_OK	成功
	       
--*/
extern HRESULT GOSSTBCA_ReadDataBuffer(const unsigned long pStartAddr, unsigned int DataLen, unsigned char* pData) ;

/*++

功能：
	向机顶盒的存储空间写信息

参数：
	lStartAddr:	     要写的存储空间的开始地址
	plDataLen:        要写的数据的长度
					 返回为写入的实际长度
	pData：			 要写的数据
	
	                     
返回值：
	
说明：
	GOSCA_OK	成功
       
--*/
extern HRESULT GOSSTBCA_WriteDataBuffer(const unsigned long pStartAddr, const unsigned int DataLen, const unsigned char* pData)  ;
extern HRESULT GOSSTBCA_FlashErase( );
/*++

功能：
	向机顶盒的备份内存信息

参数：
	
	                     
返回值：
	
说明：
	GOSCA_OK	成功
       
--*/
HRESULT GOSSTBCA_Data_Back(const unsigned long pStartAddr, const unsigned int DataLen)  ;

/*++

功能：
	申请内存

参数：
	dwSize:		申请内存的空间
						 
返回值：
	分配内存空间的地址
			
说明：
	   
--*/
extern unsigned char *GOSSTBCA_malloc( unsigned int dwSize ) ;

/*++

功能：
	释放所申请内存

参数：
	*pAddr: 	申请内存的地址
						 
返回值：
			
说明：
	   
--*/
extern void GOSSTBCA_free( unsigned char *pAddr ) ;


/*++

功能：
	挂起当前线程

参数：
	dwSleepMicroseconds: 		毫秒
	                     
返回值：
		    
说明：
       
--*/
extern HRESULT GOSSTBCA_Sleep( long dwSleepMicroseconds ) ;

/*++

功能：
	初始化信号量,初始化后信号量没有信号。

参数：
	pSemaphore: 		指向信号量的指针
	                     
返回值：
		    
说明：
       
--*/
extern HRESULT GOSSTBCA_SemaphoreInit( GOSCA_Semaphore *pSemaphore ) ;
extern HRESULT GOSSTBCA_SemaphoreTimeOutInit( GOSCA_Semaphore *pSemaphore ) ;

/*++

功能：
	释放一个信号量

参数：
	pSemaphore: 		指向信号量的指针
	                     
返回值：
		    
说明：
       
--*/
extern HRESULT GOSSTBCA_SemaphoreDestory( GOSCA_Semaphore *pSemaphore ) ;

/*++
功能：
	给信号量加信号

参数：
	pSemaphore: 		指向信号量的指针
	                     
返回值：
		    
说明：
       
--*/
extern HRESULT GOSSTBCA_SemaphoreSignal( GOSCA_Semaphore *pSemaphore ) ;

/*++
功能：
	等待信号量,等待成功之后,信号量为无信号

参数：
	pSemaphore: 		指向信号量的指针
	                     
返回值：
		    
说明：
       
--*/
extern HRESULT GOSSTBCA_SemaphoreWait( GOSCA_Semaphore *pSemaphore ) ;
extern HRESULT GOSSTBCA_SemaphoreWait_TimeOut( GOSCA_Semaphore *pSemaphore, unsigned int timeout ) ;

/*++
功能：
	CAS模块向机顶盒注册线程

参数：
	szName:				注册的线程名称
	ProcessID:			注册的线程的ID
	pTaskFun:			要注册的线程函数	                     

返回值：
	GOSCA_OK			线程注册成功
	
说明：
       
--*/
extern HRESULT GOSSTBCA_RegisterTask(const char* szName,const unsigned long ProcessID, pThreadFunc pTaskFun) ;

/*++

功能：
	设置过滤器接收CAS私有数据。

参数：
	byReqID:	请求收表的请求号。机顶盒应该保存该数据，接收到CAS私有数据后调用GOSCASTB_StreamGuardDataGot()时应该将该数据返回。
	wEcmPID:	需要过滤的流的PID。
	szFilter:	过滤器的值，为一个8个字节的数组，相当于8*8的过滤器
	szMask:		过滤器的掩码，为一个8个字节的数组，与8*8的过滤器对应，当szMask的某一bit位为0时表示要过滤的表对应的bit位不需要与szFilter里对应的bit位相同，当szMask某一位为1时表示要过滤的表对应的bit位一定要与szFilter里对应的bit位相同
	byLen:		过滤器的长度，为8
	bContiunue:	是否连续过滤

--*/
extern HRESULT GOSSTBCA_SetStreamGuardFilter(unsigned char byReqID, unsigned short wPID,const unsigned char* szFilter,const unsigned char* szMask,
                                      unsigned char byLen, unsigned int bContiunue) ;

/*++

功能：
	CA程序用此函数设置解扰器。将当前周期及下一周期的CW送给解扰器。

参数：
	wEcmPID:				CW所属的ECMPID。
	szOddKey:				奇CW的数据
	szEvenKey:				偶CW的数据
	byKeyLen:				CW的长度
	bTaingControl           true：允许录像,false：不允许录像

	                     
返回值：
       
--*/
extern HRESULT GOSSTBCA_SetDescrCW(unsigned short wEcmPID,unsigned char byKeyLen,const unsigned char* szOddKey,const unsigned char* szEvenKey,
                             unsigned char bTaingControl) ;


/*++
功能：
	   打印调试信息

参数：
	pszMsg:     调试信息内容
	                     
返回值：
	void

说明：
      
--*/
extern void GOSSTBCA_AddDebugMsg(char *pszMsg) ;

/*++
功能：
	实现与智能卡之间的通讯。采用7816标准。
参数：
	byReaderNo：	对应的读卡器编号
	byLen:			输入为待发送命令字节的长度，输出为返回字节长度
	byszCommand:	待发送的一串命令字节
	szReply:		返回的数据，空间长度恒为256字节
	
返回值：
      
--*/
extern HRESULT GOSSTBCA_SCAPDU(unsigned char byReaderNo, unsigned char* pbyLen, unsigned char* byszCommand, unsigned char* byszReply) ;

/*------------------------------------ 类型1的界面：不允许用户通过遥控器取消显示的界面--------------------------------*/
/*
功能：
	显示用户不能观看节目等提示信息，下面两个函数对应使用。
参数:	
	byMesageNo:  要显示的消息编号
	bShowInfo==1:表示有提示信息需要显示；在提示信息的下面显示提示内容，前端发送了邮件名“$#OperSvr*”的邮件内容这里显示；
	bShowInfo==0:参数pString和参数strlen不管；

--*/
extern void   GOSSTBCA_ShowPromptMessage(const char byMesageNo, unsigned int duration, unsigned long ippt, bool bShowInfo, unsigned char *pString, unsigned int strlen);
extern void   GOSSTBCA_HidePromptMessage(void) ;

/*
功能：
	显示指纹信息。
参数:
	wDuration ：毫秒级，默认在3秒自动消失
--*/
extern void	GOSSTBCA_ShowFingerPrinting(unsigned long version, unsigned long dwCardID,unsigned short wDuration) ;	/*显示指纹，由STB主程序实现。*/

/*------------------------------------ 类型2的界面：用户可以取消显示的界面---------------------------------------------*/

/*
功能：
	显示OSD消息，可以是同步显示，也可以是异步显示。
参数:	
	szOSD：		用于显示的OSD信息
	wTimes：	显示的次数
				
说明：
	用户可以通过遥控器取消当前OSD显示。
--*/
extern void   GOSSTBCA_ShowOSDMsg(const char* szOSD, unsigned char OSD_len, unsigned long OSD_version, unsigned short wTimes) ;

/*++
功能：
	显示新邮件的标志。
参数：
	byShow:		GOSCAS_NEW_EAMIL				新邮件通知
				GOSCAS_NEW_EMAIL_NO_ROOM		有新邮件，但Email空间不够
				GOSCAS_EMAIL_NONE				没有新邮件
--*/
extern void GOSSTBCA_EmailNotify(unsigned char byShow, unsigned char index) ;

/*------------------------------------------以上接口是STB供给CAS调用---------------------------------------------------*/

/*++
功能：
	获取机顶盒序列号。
参数：

返回值：
	机顶盒序列号。保留以便后期库能使用
--*/
extern unsigned long GOSSTBCA_GetSerialNo( void ) ;

/*++
功能：
	获取机顶盒硬件版本号。
参数：

返回值：
	机顶盒硬件版本号。保留，暂时未被使用
--*/
extern unsigned long GOSSTBCA_GetHardVer(void) ;

/*++
功能：
	获取机顶盒软件版本。
参数：
返回值：
	机顶盒软件版本号。保留，暂时未被使用
--*/
extern unsigned long GOSSTBCA_GetSoftVer( ) ;


/*++
功能：
	显示强制指纹。如果不是随机显示。该指纹需要一直显示。如果随机显示，需再Interval时间进行随机变化；
参数：
	version:			指纹版本，唯一
	FontColor:		指纹字颜色
	FontSize:			指纹字号
	BackGroundColor:	指纹背景颜色，格式为ARGB8888
	ChnlCnt:			指纹显示频道数，如果为0 ，表示所有频道都显示指纹
						非0，表示ServiceId参数service id数目，目前版本最大支持指定10个频道
						，格式为ARGB8888
	ServiceId:		显示指纹频道的service id
	IDType:			保留，目前版本为0x00
	Interval:			时间间隔,两次显示之间的不显示指纹的时间长度
	Lock:			是否锁定用户操作 1: 锁定用户操作，0: 不锁定
	PosType:			指纹位置类型，0 : 随机显示，1: 固定位置
	Pos_X:			固定位置指纹的X 坐标
	Pos_Y:			固定位置指纹的Y 坐标
返回值：
	机顶盒硬件版本号。
--*/
extern void	GOSSTBCA_ShowForceFingerPrinting(unsigned long version, unsigned long FontColor, unsigned char FontSize, unsigned long BackGroundColor,
			unsigned char ChnlCnt, unsigned short *ServiceId, unsigned char IDType, unsigned short Interval, unsigned char Lock,
				unsigned char PosType, unsigned short Pos_X, unsigned short Pos_Y);


/*++
功能：
	显示强制指纹。
参数：
	Version:				当前强制指纹版本号
返回值：
--*/

extern void GOSSTBCA_CancleForceFingerPrinting(unsigned long Version);



/*++
功能：
	强制换台
参数：
	pForceLockService:		强制换台参数，参见GOSCAS_STBDataPublic.h
	Version:				当前强制换台版本号
返回值：
--*/
extern void GOSSTBCA_ForceLockService(SGOSCALockService *pForceLockService, unsigned long Version);

/*++
功能：
	取消强制换台
参数：
	
返回值：
--*/
extern void GOSSTBCA_CancleForceLockService(unsigned long Version);




/*++
功能：
	取消强制OSD
参数：
	osdID:			ID，唯一
	Style:			0: 滚动显示，1: 文本显示
	Stylevalue:		style为0 ，本参数为1: 上方滚动， 0: 下方滚动
					style为1，本参数为设定文本框占屏幕的百分比，值为20%-80%
	lock:				指纹背景颜色
	FontColor:		OSD 显示文字的颜色，格式为ARGB8888
	FontSize:			OSD 显示的字号
	BackGroundColor:	文本OSD 显示的背景颜色，格式为ARGB8888
	Showtimes:		如果为滚动显示OSD ，本参数设定滚动次数，本参数为了兼容老的OSD 方式
	Data_Body:		OSD 显示内容
	Data_Len:		OSD 显示内容长度
返回值：
--*/
extern void	GOSSTBCA_ShowForceOSD(unsigned long osdID, unsigned char Style, unsigned char Stylevalue, unsigned char lock,unsigned long FontColor,
		unsigned char FontSize, unsigned long BackGroundColor, unsigned char Showtimes, unsigned char *Data_Body, unsigned short Data_Len);

/*++
功能：
	取消强制OSD
参数：
	
返回值：
--*/
extern void GOSSTBCA_HideForceOSD();

/*++
功能：
	关闭任务调度
参数：
	
返回值：
--*/
extern void GOSSTBCA_TaskDispatchoff();

/*++
功能：
	打开任务调度
参数：
	
返回值：
--*/
extern void GOSSTBCA_TaskDispatchon();



/*------------------------------以上接口用于条件寻址-----------------------------------*/

#ifdef __cplusplus
}
#endif

#endif  

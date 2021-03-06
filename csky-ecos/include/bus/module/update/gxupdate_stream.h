#ifndef __GXUPDATE_STREAM_H__
#define __GXUPDATE_STREAM_H__

#include "gxcore.h"
#include "gxupdate_def.h"

__BEGIN_DECLS

#define E_OK                                (0)
#define E_INVALID_HANDLE                    (0)
#define GXUPDATE_STREAM_BLOCK_SIZE          (1024*4)
#define GXUPDATE_STREAM_PERCENT_RATION      (10000)
#define GXUPDATE_STREAM_FINISH              (0x1fffffff)
#define GXUPDATE_STREAM_CONTINUE            (0x2fffffff)
/*
 * Goxceed升级架构示意图
 *  -----------------------------
 *  |   Application             |<--应用层程序
 *  |---------------------------|
 *  |   Stream Protocol         |<--流层协议
 *  |---------------------------|
 *  |   Transport Protocol      |<--传输层
 *  |   -----------------       |
 *  | serial | MPEG2 TS | Tftp  |<--传输层具体协议
 *  ----------------------------
 */


/**
 * @brief       升级流初始化
 * @param [in]  ProtocolList        升级协议列表
 * @param [in]  PartitionList       升级分区列表
 * @return      int32_t             返回E_OK初始化成功，其它值失败
 * @remarks     函数要求ProtocolList是指向指针数组的指针，指针数组的最后一项必须
                是NULL，以标示数组结束。
                函数要求PartitionList是指向指针数组的指针，指针数组的最后一项必须
                是NULL，以标示数组结束。
 * @see     ::GxUpdate_StreamOpen ::GxUpdate_StreamIoctl ::GxUpdate_StreamClose
 */
int32_t GxUpdate_StreamInit(GxUpdate_ProtocolOps**ProtocolList, GxUpdate_PartitionOps** PartitionList);


/**
 * @brief       打开升级流
 * @param [in]  Name                升级流名字符串
 * @return      handle_t            升级流句柄，如果执行失败返回E_INVALID_HANDLE，
                                    其它值表示执行成功
 * @remarks
 * @see     ::GxUpdate_StreamInit ::GxUpdate_StreamIoctl ::GxUpdate_StreamClose
 */
handle_t GxUpdate_StreamOpen(const GxUpdate_StreamName Name);

/**
 * @brief       设置升级流
 * @param [in]  Handle              打开的升级流句柄
 * @param [in]  Key                 设置标识KEY
 * @param [in]  Buf                 设置标识KEY对应的参数
 * @param [in]  Size                设置标识KEY参数大小
 * @return      int32_t             执行成功返回E_OK，其它值失败
 * @remarks     在KEY值为GXUPDATE_STREAM_UPDATE_STOP时，如果程序已经调用
                GXUPDATE_STREAM_UPDATE_START启动升级，GXUPDATE_STREAM_UPDATE_STOP
                为了防止数据被破坏，这时STOP设置将不起作用。�
 * @see     ::GxUpdate_StreamInit ::GxUpdate_StreamOpen ::GxUpdate_StreamClose
        ::GXUPDATE_STREAM_PROTOCOL_SELECT ::GXUPDATE_STREAM_PROTOCOL_CTRL
        ::GXUPDATE_STREAM_UPDATE_START ::GXUPDATE_STREAM_UPDATE_STOP
        ::GXUPDATE_STREAM_UPDATE_GET_PERCENT ::GXUPDATE_STREAM_UPDATE_GET_STATUS

 */
int32_t GxUpdate_StreamIoctl(handle_t Handle, int32_t Key, void*  Buf, size_t Size);

/**
 * @brief       关闭找开的升级流
 * @param [in]  Handle              升级流句柄
 * @return      int32_t             返回E_OK执行成功，其它值失败
 * @remarks
 * @see     ::GxUpdate_StreamInit ::GxUpdate_StreamOpen ::GxUpdate_StreamIoctl
 */
int32_t GxUpdate_StreamClose(handle_t Handle);

/**
 * @brief       清除状态
 * @param [in]  Handle              升级流句柄
 * @return      int32_t             返回E_OK执行成功，其它值失败
 * @remarks
 * @see     ::GxUpdate_StreamInit ::GxUpdate_StreamOpen ::GxUpdate_StreamIoctl
 */
int32_t GxUpdate_StreamClearStatus(handle_t Handle);

/**
 * @brief      获取状态
 * @param [in] 
 * @return      int32_t    
 * @remarks
 * @see     ::GxUpdate_StreamInit ::GxUpdate_StreamOpen ::GxUpdate_StreamIoctl
 */
int32_t GxUpdate_StreamGetStatus(void);

__END_DECLS

#endif


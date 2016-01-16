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
 * Goxceed…˝º∂º‹ππ æ“‚Õº
 *  -----------------------------
 *  |   Application             |<--”¶”√≤„≥Ã–Ú
 *  |---------------------------|
 *  |   Stream Protocol         |<--¡˜≤„–≠“È
 *  |---------------------------|
 *  |   Transport Protocol      |<--¥´ ‰≤„
 *  |   -----------------       |
 *  | serial | MPEG2 TS | Tftp  |<--¥´ ‰≤„æﬂÃÂ–≠“È
 *  ----------------------------
 */


/**
 * @brief       …˝º∂¡˜≥ı ºªØ
 * @param [in]  ProtocolList        …˝º∂–≠“È¡–±Ì
 * @param [in]  PartitionList       …˝º∂∑÷«¯¡–±Ì
 * @return      int32_t             ∑µªÿE_OK≥ı ºªØ≥…π¶£¨∆‰À¸÷µ ß∞‹
 * @remarks     ∫Ø ˝“™«ÛProtocolList «÷∏œÚ÷∏’Î ˝◊Èµƒ÷∏’Î£¨÷∏’Î ˝◊Èµƒ◊Ó∫Û“ªœÓ±ÿ–Î
                 «NULL£¨“‘±Í æ ˝◊ÈΩ· ¯°£
                ∫Ø ˝“™«ÛPartitionList «÷∏œÚ÷∏’Î ˝◊Èµƒ÷∏’Î£¨÷∏’Î ˝◊Èµƒ◊Ó∫Û“ªœÓ±ÿ–Î
                 «NULL£¨“‘±Í æ ˝◊ÈΩ· ¯°£
 * @see     ::GxUpdate_StreamOpen ::GxUpdate_StreamIoctl ::GxUpdate_StreamClose
 */
int32_t GxUpdate_StreamInit(GxUpdate_ProtocolOps**ProtocolList, GxUpdate_PartitionOps** PartitionList);


/**
 * @brief       ¥Úø™…˝º∂¡˜
 * @param [in]  Name                …˝º∂¡˜√˚◊÷∑˚¥Æ
 * @return      handle_t            …˝º∂¡˜æ‰±˙£¨»Áπ˚÷¥–– ß∞‹∑µªÿE_INVALID_HANDLE£¨
                                    ∆‰À¸÷µ±Ì æ÷¥––≥…π¶
 * @remarks
 * @see     ::GxUpdate_StreamInit ::GxUpdate_StreamIoctl ::GxUpdate_StreamClose
 */
handle_t GxUpdate_StreamOpen(const GxUpdate_StreamName Name);

/**
 * @brief       …Ë÷√…˝º∂¡˜
 * @param [in]  Handle              ¥Úø™µƒ…˝º∂¡˜æ‰±˙
 * @param [in]  Key                 …Ë÷√±Í ∂KEY
 * @param [in]  Buf                 …Ë÷√±Í ∂KEY∂‘”¶µƒ≤Œ ˝
 * @param [in]  Size                …Ë÷√±Í ∂KEY≤Œ ˝¥Û–°
 * @return      int32_t             ÷¥––≥…π¶∑µªÿE_OK£¨∆‰À¸÷µ ß∞‹
 * @remarks     ‘⁄KEY÷µŒ™GXUPDATE_STREAM_UPDATE_STOP ±£¨»Áπ˚≥Ã–Ú“—æ≠µ˜”√
                GXUPDATE_STREAM_UPDATE_START∆Ù∂Ø…˝º∂£¨GXUPDATE_STREAM_UPDATE_STOP
                Œ™¡À∑¿÷π ˝æ›±ª∆∆ªµ£¨’‚ ±STOP…Ë÷√Ω´≤ª∆◊˜”√°£¨
 * @see     ::GxUpdate_StreamInit ::GxUpdate_StreamOpen ::GxUpdate_StreamClose
        ::GXUPDATE_STREAM_PROTOCOL_SELECT ::GXUPDATE_STREAM_PROTOCOL_CTRL
        ::GXUPDATE_STREAM_UPDATE_START ::GXUPDATE_STREAM_UPDATE_STOP
        ::GXUPDATE_STREAM_UPDATE_GET_PERCENT ::GXUPDATE_STREAM_UPDATE_GET_STATUS

 */
int32_t GxUpdate_StreamIoctl(handle_t Handle, int32_t Key, void*  Buf, size_t Size);

/**
 * @brief       πÿ±’’“ø™µƒ…˝º∂¡˜
 * @param [in]  Handle              …˝º∂¡˜æ‰±˙
 * @return      int32_t             ∑µªÿE_OK÷¥––≥…π¶£¨∆‰À¸÷µ ß∞‹
 * @remarks
 * @see     ::GxUpdate_StreamInit ::GxUpdate_StreamOpen ::GxUpdate_StreamIoctl
 */
int32_t GxUpdate_StreamClose(handle_t Handle);

/**
 * @brief       «Â≥˝◊¥Ã¨
 * @param [in]  Handle              …˝º∂¡˜æ‰±˙
 * @return      int32_t             ∑µªÿE_OK÷¥––≥…π¶£¨∆‰À¸÷µ ß∞‹
 * @remarks
 * @see     ::GxUpdate_StreamInit ::GxUpdate_StreamOpen ::GxUpdate_StreamIoctl
 */
int32_t GxUpdate_StreamClearStatus(handle_t Handle);

/**
 * @brief      ªÒ»°◊¥Ã¨
 * @param [in] 
 * @return      int32_t    
 * @remarks
 * @see     ::GxUpdate_StreamInit ::GxUpdate_StreamOpen ::GxUpdate_StreamIoctl
 */
int32_t GxUpdate_StreamGetStatus(void);

__END_DECLS

#endif


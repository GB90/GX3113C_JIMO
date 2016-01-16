#include "ca_process.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_porting_stb_api.h"

unsigned short g_wEcmPid = 0x1fff;
byte Ca_Flag = 0;
byte Ca_cardStatus = 0;
extern void QZCA_ShowBugMessage(uint16_t messageType);
extern void Qz_STBPort_Create_Semaphore(void);
extern void Qz_STBPort_Obtain_Semaphore(void);
extern void Qz_STBPort_Release_Semaphore(void);


void QZ_Init_CAS(byte bDebugOpen)
{
    Ca_Flag = bDebugOpen;
    Qz_STBPort_Create_Semaphore();
}

unsigned char apdu_package_v1(
	 unsigned char *cache,
	 unsigned char cla,
	 unsigned char ins,
	 unsigned char p1,
	 unsigned char p2
	 )
{
	 cache[0] = cla;
	 cache[1] = ins;
	 cache[2] = p1;
	 cache[3] = p2;
	 return 4;
}

unsigned char apdu_package_v2(
	 unsigned char *cache,
	 unsigned char cla,
	 unsigned char ins,
	 unsigned char p1,
	 unsigned char p2,
	 unsigned char le
	 )
{
	 cache[0] = cla;
	 cache[1] = ins;
	 cache[2] = p1;
	 cache[3] = p2;
	 cache[4] = le;
	 return 5;
}

unsigned char apdu_package_v3(
	 unsigned char *cache,
	 unsigned char cla, 
	 unsigned char ins, 
	 unsigned char p1, 
	 unsigned char p2,
	 unsigned char lc,
	 unsigned char *input_data
	 )
{
     unsigned char i = 0;
	 cache[0] = cla;
	 cache[1] = ins;
	 cache[2] = p1;
	 cache[3] = p2;
	 cache[4] = lc;
	 for(i = 0; i < lc; ++i){
		  cache[5 + i] = input_data[i];
	 }
	 return 5 + lc;
}

unsigned char apdu_package_v4(
	 unsigned char *cache,
	 unsigned char cla, 
	 unsigned char ins, 
	 unsigned char p1, 
	 unsigned char p2,
	 unsigned char lc,
	 unsigned char *input_data
	 )
{
     unsigned char i = 0;
	 cache[0] = cla;
	 cache[1] = ins;
	 cache[2] = p1;
	 cache[3] = p2;
	 cache[4] = lc;
	 for(i = 0; i < lc; ++i){
		  cache[5 + i] = input_data[i];
	 }
	 return 5 + lc;
}

unsigned short crc16ccitt(byte *pBuffer, unsigned long uBufSize)
{
	 unsigned short uCcitt16 = 0xFFFF;
	 unsigned long i = 0;

	 for(i = 0; i < uBufSize; i++)
	 {
		  uCcitt16 = (uCcitt16 >> 8) | (uCcitt16 << 8);
		  uCcitt16 ^= pBuffer[i];
		  uCcitt16 ^= (uCcitt16 & 0xFF) >> 4;
		  uCcitt16 ^= (uCcitt16 << 8) << 4;
		  uCcitt16 ^= ((uCcitt16 & 0xFF) << 4) << 1;
	 }
	 return ~uCcitt16;
}

void check_sw1sw2(uint16_t sw1sw2)
{
    QZCA_ShowBugMessage(sw1sw2);
    
	switch(sw1sw2)
	{
		case NO_SW1SW2:
			Qz_Dbg("NO_SW1SW2\n");
			break;
		case SW1SW2_OK: 
			Qz_Dbg("SW1SW2正常\n");
			break;
		case SW1SW2_MEMORY_ERROR:
			Qz_Dbg("智能卡内存出错\n");
			break;
		case SW1SW2_WRONG_LC_LENGTH:
			Qz_Dbg("Lc长度错误\n");
			break;
		case SW1SW2_INCORRECT_DATA:
			Qz_Dbg("crc错误\n");
			break;
		case SW1SW2_INCORRECT_PARA:
			Qz_Dbg("APDU参数错误\n");
			break;
		case SW1SW2_WRONG_LE_LENGTH:
			Qz_Dbg("APDU 返回字节长度（le）错误\n");
			break;
		case SW1SW2_INS_NOT_SUPPORTED:
			Qz_Dbg("APDU命令不支持\n");
			break;
		case SW1SW2_CLA_NOT_SUPPORTED:
			Qz_Dbg("APDU 类型码不支持\n");
			break;
		case SW1SW2_GENERAL_ERROR:
			Qz_Dbg("未知类型错误，一般是通讯错误\n");
			break;
		case SW1SW2_NOT_START:
			Qz_Dbg("未到授权起始日期\n");
			break;
		case SW1SW2_STB_SN_NOT_MATCH:
			Qz_Dbg("机顶盒序列号不匹配\n");
			break;
		case SW1SW2_CARD_NOT_INIT:
			Qz_Dbg("卡未初始化\n");
			break;
		case SW1SW2_INCORRECT_SUPER_CAS_ID:
			Qz_Dbg("错误的SUPER_CAS_ID\n");
			break;
		case SW1SW2_INCORRECT_SMARTCARD_ID:
			Qz_Dbg("错误的智能卡号\n");
			break;
		case SW1SW2_PPV_EXCEED: 
			Qz_Dbg("PPV过期\n");
			break;
		case SW1SW2_NOT_PAIRED:
			Qz_Dbg("卡没有设置为机卡配对\n");
			break;
		case SW1SW2_ECM_NOT_AUTHORIZED:
			Qz_Dbg("频道未授权\n");
			break;
		case SW1SW2_LIMIT_AREA:
			Qz_Dbg("区域限播\n");
			break;
		case SW1SW2_LIMIT_CARD:
			Qz_Dbg("卡限播\n");
			break;	
		case SW1SW2_EXPIRED:
			Qz_Dbg("授权过期\n");
			break;	
		case SW1SW2_NOT_CHILD_MODE:
			Qz_Dbg("智能卡没有设置成子卡模式\n");
			break;
		case SW1SW2_NO_CHILD_AUTH:
			Qz_Dbg("没有子卡授权\n");
			break;
		case SW1SW2_CHILD_NOT_AUTHORIZED: /*wangjian modify on 20130807*/
			Qz_Dbg("子卡未激活，请激活子卡\n");
			break;
		case SW1SW2_CHILD_AUTH_TIMEOUT:
			Qz_Dbg("设置子卡授权超时\n");
			break;
		case SW1SW2_CHILD_AUTH_EXPIRED:
			Qz_Dbg("子卡授权过期\n");
			break;
        case QZ_CARD_NOTFOUND:
            Qz_Dbg("请插入智能卡\n");
			break;
		default:
			Qz_Dbg("SW1SW2未知错误\n");
			break;
	}    
}

/* 上电握手*/
byte check_card_apdu(const char *stb_sn, byte stb_sn_len, byte *super_cas_id, byte *smartcard_id, byte *cw_mask)
{
    byte in[128];
    byte out[128];
    byte cache[32];

    uint8_t sc_result;
    uint16_t errReturn = 0;

    uint16_t bytes_readed, bytes_written;
    unsigned int crc;
    
    cache[0] = stb_sn_len;
    memcpy(&cache[1], stb_sn, stb_sn_len);
    crc = crc16ccitt(&cache[1], stb_sn_len);
    cache[stb_sn_len + 1] = (unsigned char)(crc >> 8);
    cache[stb_sn_len + 2] = (unsigned char)crc;

    bytes_written = apdu_package_v4(out, GS_CLA, INS_CHECK_SMART, 0, 0, stb_sn_len + 3, cache);
    //Qz_DUMP("[CA CAS] ATR,Write->", out, bytes_written);
    Qz_STBPort_Obtain_Semaphore();
    sc_result =  app_porting_ca_smart_sc_apdu(out,bytes_written,in,&bytes_readed);
    Qz_STBPort_Release_Semaphore();
    if (TRUE == sc_result)
    {
        //Qz_DUMP("[CA CAS] ATR,Read->", in, bytes_readed);

        crc = crc16ccitt(in, bytes_readed - 4);
        if(crc != (in[bytes_readed - 4] << 8) + in[bytes_readed - 3])
            return ERR_CRC;

        if((in[bytes_readed - 2] != 0x90) || (in[bytes_readed - 1] != 0x00))
        {
            errReturn = (in[bytes_readed-2]<<8)+in[bytes_readed-1];
            check_sw1sw2(errReturn);
            return ERR_NOT_INI;
        }
    }
    else
    {
        return ERR_COMM;		
    }

    if((in[bytes_readed - 2] == 0x90) && (in[bytes_readed - 1] == 0x00))
    {
        memcpy(super_cas_id, in, 4);
        memcpy(smartcard_id, &in[4], 3);
        memcpy(cw_mask, &in[7], 8);
        return ERR_OK;
    }
    return ERR_NOT_INI;
}

/* 上电握手, 需要相应的智能卡版本号*/
byte check_card_apdu_ex(unsigned char version_number, const char *stb_sn, byte stb_sn_len, byte *super_cas_id, byte *smartcard_id, byte *cw_mask)
{
    byte in[128];
    byte out[128];
    byte cache[32];
    uint8_t sc_result;
    uint16_t errReturn = 0;

    //const byte check_reply = 19; /* 应答包长度 */
    uint16_t bytes_readed, bytes_written;
    unsigned int crc;
    cache[0] = stb_sn_len;
    memcpy(&cache[1], stb_sn, stb_sn_len);
    crc = crc16ccitt(&cache[1], stb_sn_len);
    cache[stb_sn_len + 1] = (unsigned char)(crc >> 8);
    cache[stb_sn_len + 2] = (unsigned char)crc;
    cache[stb_sn_len + 3] = version_number;

    bytes_written = apdu_package_v4(out, GS_CLA, INS_CHECK_SMART, 0, 0, stb_sn_len + 4, cache);
    Qz_DUMP("[CA CAS] ATR,Write->", out, bytes_written);
    sc_result =  app_porting_ca_smart_sc_apdu(out,bytes_written,in,&bytes_readed);
    if (TRUE == sc_result)
    {
        Qz_DUMP("[CA CAS] ATR,Read->", in, bytes_readed);

        crc = crc16ccitt(in, bytes_readed - 4);
        if(crc != (in[bytes_readed - 4] << 8) + in[bytes_readed - 3])
            return ERR_CRC;

        if((in[bytes_readed - 2] != 0x90) || (in[bytes_readed - 1] != 0x00))
        {
            errReturn = (in[bytes_readed-2]<<8)+in[bytes_readed-1];
            check_sw1sw2(errReturn);
            return ERR_NOT_INI;
        }
        else
        {
            memcpy(super_cas_id, in, 4);
            memcpy(smartcard_id, &in[4], 3);
            memcpy(cw_mask, &in[7], 8);
            return ERR_OK;
        }
    }
    else
    {
        return ERR_COMM;		
    }
}

//处理EMM， 数据从CA_SYSTEM_ID开始，直至数据包结束
byte process_emm_apdu(const byte *emm_data, byte emm_len)
{
    byte in[128];
    byte out[128];
    uint8_t sc_result;
    static byte emm_back[128]={0};
    uint16_t errReturn = 0;
    const byte emm_reply = 2;
    uint16_t bytes_readed, bytes_written;
    byte ret = ERR_COMM;
    
    if(emm_len > 128)
    {
        return ERR_DATA_LENGTH;
    }
    if(memncmp(emm_data, (const byte *)emm_back, emm_len) == 0)
    {
        return  ERR_OLD_DATA;//已经处理过的旧数据
    }
    
    bytes_written = apdu_package_v3(out, GS_CLA, INS_PROCESS_EMM, 0, 0, emm_len, (byte *)emm_data);
    Qz_DUMP("[EMM] ATR,Write->", out, bytes_written);
    Qz_STBPort_Obtain_Semaphore();
    sc_result =  app_porting_ca_smart_sc_apdu(out,bytes_written,in,&bytes_readed);
    Qz_STBPort_Release_Semaphore();
    if (TRUE == sc_result)
    {
        Qz_DUMP("[EMM] ATR,Read->", in, bytes_readed);

        if(bytes_readed != emm_reply)
        {
            ret =  ERR_COMM;
            goto exit;
        }
        
        errReturn = (in[bytes_readed-2]<<8)+in[bytes_readed-1];
	    check_sw1sw2(errReturn);
        
        if((in[bytes_readed - 2] == 0x90) && (in[bytes_readed - 1] == 0x00))
        {
            memcpy(emm_back, emm_data, emm_len);
            ret = ERR_OK;
        }
exit:
        
        return ret;
    }
    else
    {
        return ERR_COMM;		
    }
}

//处理ECM， 数据从CA_SYSTEM_ID开始，直至数据包结束
//cw,奇在前，偶在后
byte process_ecm_apdu(const byte *ecm_data, byte ecm_len, byte *cw_mask, byte *cw)
{
	 byte in[128];
	 byte out[128];
	 static byte ecm_back[128]={0};
     uint8_t sc_result;
	 const byte ecm_reply = 20; 
     uint16_t errReturn = 0;
	 uint16_t bytes_readed, bytes_written;
	 uint16_t crc;
	 int i;
     byte ret = ERR_COMM;
     
	 if(ecm_len > 128)
	 {
		  return ERR_DATA_LENGTH;
	 }
	 if(memncmp(ecm_back, (const byte *)ecm_data, ecm_len) == 0)
	 {
		  return ERR_OLD_DATA;//已经处理过的旧数据
	 }
     
	 bytes_written = apdu_package_v4(out, GS_CLA, INS_PROCESS_ECM, 0, 0, ecm_len, (byte *)ecm_data);
     Qz_DUMP("[ECM] ATR,Write->", out, bytes_written);
     Qz_STBPort_Obtain_Semaphore();
     sc_result =  app_porting_ca_smart_sc_apdu(out,bytes_written,in,&bytes_readed);
     Qz_STBPort_Release_Semaphore();
     if (TRUE == sc_result)
     {
        Qz_DUMP("[ECM] ATR,Read->", in, bytes_readed);

        if(bytes_readed != ecm_reply)
        {
            ret =  ERR_COMM;
            goto exit;
        }

        errReturn = (in[bytes_readed-2]<<8)+in[bytes_readed-1];
	    check_sw1sw2(errReturn);

        crc = crc16ccitt(in, 16);
        if(crc != (uint16_t)((in[bytes_readed - 4] << 8) | in[bytes_readed - 3]))
        {
            Qz_Dbg("CRC :%#x,%#x\n",crc,(uint16_t)((in[bytes_readed - 4] << 8) | in[bytes_readed - 3]));

            ret =  ERR_CRC;
            goto exit;
        }
       
        if((in[bytes_readed - 2] == 0x90) && (in[bytes_readed - 1] == 0x00))
        {
            memcpy(cw, in, 16);
            for(i = 0; i < 8; ++i)
            {
                cw[i] = cw[i] ^ cw_mask[i];
                cw[8 + i] = cw[8 + i] ^ cw_mask[i];
            }
            memcpy(ecm_back, ecm_data, ecm_len);
            ret = ERR_OK;
        }
exit:
        return ret;
        
     }
     else
     {
        return ERR_COMM;		
     }
}
//取授权截止时间，返回的4个字节以yy-m-d的格式
byte get_expire_date(byte *expire_date)
{
	 byte in[128];
	 byte out[128];
	 const byte reply = 4;
	 uint8_t sc_result;
	 uint16_t bytes_readed, bytes_written;
	 bytes_written = apdu_package_v2(out, GS_CLA, INS_GET_EXPIRE_DATE, 0, 0, reply);

     Qz_DUMP("[CA CAS] ATR,Write->", out, bytes_written);
     Qz_STBPort_Obtain_Semaphore();
     sc_result =  app_porting_ca_smart_sc_apdu(out,bytes_written,in,&bytes_readed);
     Qz_STBPort_Release_Semaphore();
     if (TRUE == sc_result)
     {
        Qz_DUMP("[CA CAS] ATR,Read->", in, bytes_readed);

        if((in[bytes_readed - 2] != 0x90) || (in[bytes_readed - 1] != 0x00))
        {

            return ERR_ECM;
        }
        else
        {
            memcpy(expire_date, &in[0], 4);
		    return ERR_OK;
        }
     }
     else
     {
        return ERR_COMM;		
     }
}

//取区域码，区域码为2字节
byte get_area_code(byte *area_code)
{
	 byte in[128];
	 byte out[128];
	 const byte reply = 4;
	 uint8_t sc_result;
	 uint16_t bytes_readed, bytes_written;
	 bytes_written = apdu_package_v2(out, GS_CLA, INS_GET_AREA_CODE, 0, 0, reply);
     Qz_DUMP("[CA CAS]_area_code,Write->", out, bytes_written);
     Qz_STBPort_Obtain_Semaphore();
     sc_result =  app_porting_ca_smart_sc_apdu(out,bytes_written,in,&bytes_readed);
     Qz_STBPort_Release_Semaphore();
     if (TRUE == sc_result)
     {
        Qz_DUMP("[CA CAS]_area_code,Read->", in, bytes_readed);

        if((in[bytes_readed - 2] != 0x90) || (in[bytes_readed - 1] != 0x00))
        {
            return ERR_ECM;
        }
        else
        {
            memcpy(area_code, in, 2);
		    return ERR_OK;
        }
     }
     else
     {
        return ERR_COMM;		
     }
}

	
void set_cw(unsigned short wEcmPID,byte *cw)
{
	 byte OldCw[8];
     byte EvenCw[8];
     memcpy(OldCw,cw,8);
     memcpy(EvenCw,cw+8,8);
     Qz_DUMP("[CW OldCw]->", OldCw, 8);
     Qz_DUMP("[CW EvenCw]->", EvenCw, 8);
     app_porting_ca_descramble_set_cw(wEcmPID,OldCw,EvenCw,8,0);
     g_wEcmPid = wEcmPID;
}

byte memncmp(const byte *dst, const byte *src, byte length)
{
	 byte i;
	 for(i = 0; i < length; ++i)
	 {
		  if((*(dst + i)) != (*(src + i)))return -1;
	 }
	 return 0;
}


void Ca_CardIn()
{
    Ca_cardStatus = 1;
}

void Ca_CardOut()
{
    if(g_wEcmPid != 0x1fff)
    {
        byte cw[16];
        memset(cw,0,sizeof(cw));
        set_cw(g_wEcmPid,cw);
        g_wEcmPid = 0x1fff;
    }
    Ca_cardStatus = 0;
}

byte Ca_Card_Ready()
{
    return (Ca_cardStatus >0)?ERR_OK:ERR_UNKNOWN;
}


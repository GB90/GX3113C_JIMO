#ifndef CAS_HCN_FUNC_H
#define CAS_HCN_FUNC_H

#define	HCNCASTB_FALSE			(0)
#define	HCNCASTB_TRUE			(1)

#define HCNCASTB_SUCCESS						0x00	//解析成功
#define HCNCASTB_UNERROR 						0x01	//未知错误
#define HCNCASTB_SAME_VERSION 					0x02	//版本相同
#define HCNCASTB_UPDATE_PROPERTY_ERROR		0x03	//更新个人信息出错
#define HCNCASTB_UPDATE_ADDRESSINFO_ERROR	0x04	//更新寻址信息出错
#define HCNCASTB_UPDATE_CARD_VERSION_ERROR	0x05	//更新卡内版本出错
#define HCNCASTB_UPDATE_CREDIT_ERROR			0x06	//更新信誉点数出错
#define HCNCASTB_UPDATE_KEY_ERROR				0x07	//更新SK出错
#define HCNCASTB_UPDATE_ACTIME_ERROR			0x08	//更新ACTime出错
#define HCNCASTB_UPDATE_ACMAP_ERROR			0x09	//更新AC映射文件出错
#define HCNCASTB_NOT_STARTED					0x0A	//卡未启用
#define HCNCASTB_CASID_ERROR					0x0B	//CAS_ID不同
#define HCNCASTB_OUT_WORKTIME					0x0C	//不在工作时段
#define HCNCASTB_NO_AUTHEN						0x0D	//没有节目授权
#define HCNCASTB_ZONE_LIMITED					0x0E	//区域限播
#define HCNCASTB_NOT_PAIRED					0x0F	//机卡不配对
#define HCNCASTB_NOT_ADULT						0x10	//不满足成人级别
#define HCNCASTB_DECRYPTCW_ERROR				0x11	//解密CW出错
#define HCNCASTB_UNKNOWN_CARD					0x12	//不是本系统发行卡
#define HCNCASTB_AUTHEN_ERROR					0x13	//外部认证,获取读写权限失败
#define HCNCASTB_UPDATE_CAS_VERSION_ERROR	0x14	//更新CAS版本出错
#define HCNCASTB_H_RESET_ERROR				0x15	//硬件复位出错
#define HCNCASTB_IPPV_ERROR			0x16	//硬件复位出错
#define HCNCASTB_SET_BINDINGDATA_ERROR 0x17	//设置绑定数据出错
#define HCNCASTB_GET_BINDINGDATA_ERROR 0x18	//获取绑定数据出错
#define HCNCASTB_CHILD_PARENT_SMC_PROCESS_ERROR 0x19	//解密处理错误
#define HCNCASTB_CPSMC_TIME_AWARD_ERROR 0x20	//按次授权解密出错
#define HCNCASTB_CPSMC_PERIOD_AWARD_ERROR 0x21	//按时授权解密出错
#define HCNCASTB_GET_CURRENTTIME_IS_ZERO_ERROR 0x22	//当前时间为零

#define IRW_NO_ERROR		 0
#define IRW_ERROR			-1
#define IRW_WRITE_ERROR		-2
#define IRW_READ_ERROR		-3
#define IRW_STATUS_ERROR	-4
#define IRW_RESET_AUTHRITY_ERROR -5

#define Pack_Num_MAX	20
#define prog_num_max	30

#define sec_8_hour (8*3600L)
#define day_366_use (366*24*3600L)
#define day_365_use (365*24*3600L)
#define day_28_use  (28*24*3600L)
#define day_1_use   (24*3600L)
#define hour_1_use  3600
#define min_1_use   60

typedef struct tagHPACK11
{
	UINT8 id;	//授权代码
	UINT8 ver;		//版本
}HPACK11,*PHPACK11;

typedef struct tagPACK11
{
	UINT8 group_id[2];
	UINT8 zone_id[4];
}PACK11,*PPACK11;

typedef struct tagHPACK12
{
	UINT8 id;	//授权代码
}HPACK12,*PHPACK12;

typedef struct tagPACK12
{
	UINT8 ver;		//版本0x4X
	UINT8 vendor;	//运营商ID
	UINT8 credit[2];	//信誉点数
}PACK12,*PPACK12;

typedef struct tagPACK13
{
	UINT8 tag;
	UINT8 ver;		
	UINT8 net_id[2];	
}PACK13,*PPACK13;

typedef struct tagPACK14
{
	UINT8 id;	//授权代码
	UINT16 mailid;
	UINT16 len;
}PACK14,*PPACK14;

typedef struct tagPACK15
{
	UINT8 id;	//授权代码
	UINT16 osdid;
	UINT16 len;
}PACK15,*PPACK15;

typedef struct tagHPACK16
{
	UINT8	id;			//授权代码
	UINT8	ver;		//版本
	UINT8	ver1;		//版本
	UINT8	len[2];
	UINT8	property[2];
	UINT8	pack_count;	//业务个数
}HPACK16,*PHPACK16;

typedef struct tagPACK16
{
	UINT8 pack_id; //产品包编号
	UINT8 endtime[4];	//产品包结束时间
	UINT8 cw_i;	//SK指示
	UINT8 sk[8];	//SK密文
}PACK16,*PPACK16;

typedef struct tagPACK16_KN
{
	UINT8 pack_id; //产品包编号
	UINT8 jsjh[4]; //卡号   二代用
	UINT8 endtime[4];	//产品包结束时间
//	UINT8 cw_i;	//SK指示
//	UINT8 sk[8];	//SK密文
}PACK16_KN,*PPACK16_KN;

typedef struct tagPACK19_KN
{
	UINT8 bak_buff; //备用
	UINT8 jsjh[4]; //卡号   二代用
	UINT8 zj_jsjh[4];	//主机接收机号
//	UINT8 cw_i;	//SK指示
//	UINT8 sk[8];	//SK密文
}PACK19_KN,*PPACK19_KN;

typedef struct tagPACK16_1
{
	UINT8 pack_id; //产品包编号
	UINT8 startime[4];	//产品包结束时间
	UINT8 endtime[4];	//产品包结束时间
}PACK16_1,*PPACK16_1;

typedef struct tagPACK16_Chip1
{
	UINT8 pack_id; //产品包编号
	UINT8 startime[4];	//
	UINT8 endtime[4];	//产品包结束时间
}PACK16_Chip1,*PPACK16_Chip1;

typedef struct tagHPACK54
{
	UINT8	id;			//授权代码
	UINT8	ver;		//版本
	UINT8	productcnt;//业务包个数
}HPACK54,*PHPACK54;

typedef struct tagPACK54
{
	UINT8 acno[2]; //节目编号
}PACK54,*PPACK54;

typedef struct tagMPACK54
{
	UINT8	product_id;	//业务包编号
	UINT8	prog_num;	//节目个数
	PACK54 	prog[prog_num_max];//节目编号
}MPACK54,*PMPACK54;

typedef struct tagECM
{
	UINT8 table_id[1];
	UINT8 total_length[2];
	UINT8 cas_id[4];
	UINT8 prote[2];
	UINT8 ac[2];
	UINT8 product_num;
}HEADECM,*PHEADECM;
typedef struct ECM
{
	UINT8 product_id;//节目包编号
	UINT8 cw[32];	// 4个cw
}ECMPROG,*PECMPROG;




typedef struct ECM_KN
{
	UINT8 product_id;//节目包编号
//	UINT8 cw[24];	// 4个cw-3-1-3-1  后面一位用作副机授权天数
	UINT8 cw[23];	// 4个cw-3-1-3-1  后面一位用作副机授权天数
	UINT8 fjjh_flag;
	UINT8 cur_shijian[3];
	UINT8 fjsqjg;	//副机授权间隔
	UINT8 sq_shijian[3];
    UINT8 fjsqts;   //副机授权天数
    UINT8 cur_time[4];
}ECMPROG_KN,*PECMPROG_KN;

typedef struct ECMppv
{
	UINT8 cur_time[4];
	UINT8 ippv[2];
	UINT8 povder;
	UINT8 price[1];
	UINT8 end_time[4];
}ECMPPV,*PECMPPv;

UINT32 hcn_calcu_hex2d(UINT8 *buffer, UINT8 num)CASDRVFUNCPOSTFIX;
UINT32 hcn_hex_to_calcu(UINT8 *buffer, UINT8 num)CASDRVFUNCPOSTFIX;
UINT8 hcn_search_deal(UINT8 *buf,hcncas_icinfor* curcasinfo)CASDRVFUNCPOSTFIX;
INT32 hcncas_issameversion( UINT8 index, UINT8 iVersion)CASDRVFUNCPOSTFIX;
INT32 hcncas_update_cardversion( UINT8 index, UINT8 iVersion, hcncas_icinfor *glob_card)CASDRVFUNCPOSTFIX;
INT32 hcncas_update_addressinfo( PPACK11 ppack)CASDRVFUNCPOSTFIX;
INT32 hcncas_update_vendor_credit(UINT8 vendor,  UINT8 *credit)CASDRVFUNCPOSTFIX;
INT32 hcncas_updat_property( UINT8* piProperty)CASDRVFUNCPOSTFIX;
INT32 IC_SetSK(UINT8 *new_sk, UINT8 index)CASDRVFUNCPOSTFIX;
void hcn_set_card_status(UINT8 satus)CASDRVFUNCPOSTFIX;
INT32 STB_GetIC_NO(UINT8* ic_no_stb,UINT8 len)CASDRVFUNCPOSTFIX;
INT32 STB_SetIC_NO(UINT8* ic_no_stb,UINT8 len)CASDRVFUNCPOSTFIX;
INT32 IC_SetPair(UINT8 *pair)CASDRVFUNCPOSTFIX;
void hcncas_time_to_date(time_t *plTime, ca_time* ptDate)CASDRVFUNCPOSTFIX;
INT32 IC_GetCardVersion(UINT8 index, UINT8 *version)CASDRVFUNCPOSTFIX;
INT32 hcncas_issameversion( UINT8 index, UINT8 iVersion)CASDRVFUNCPOSTFIX;
INT32  HCNCASTB_IsCardStarted( hcncas_icinfor* curca_info)CASDRVFUNCPOSTFIX;
INT32  HCNCASTB_IsCond_Restrict( hcncas_icinfor* curca_info)CASDRVFUNCPOSTFIX;
INT32  HCNCASTB_IsAllow_Record( hcncas_icinfor* curca_info)CASDRVFUNCPOSTFIX;
INT32  HCNCASTB_IsZone_Restrict( hcncas_icinfor* curca_info)CASDRVFUNCPOSTFIX;
INT32 HCNCASTB_IsInWorkTime( time_t* piCurTime,UINT8* curca_info)CASDRVFUNCPOSTFIX;
INT32 IC_SetIPPVInfo(UINT8 *ecm_ippv_info, UINT8 num)CASDRVFUNCPOSTFIX;	//直接覆盖卡里面的所有内容
INT32 IC_GetIPPVInfo_Incard(UINT8 *ippv_infor, UINT8 *num)CASDRVFUNCPOSTFIX;
INT32 IC_SetCardVersion(UINT8 index, UINT8 version)CASDRVFUNCPOSTFIX;
INT32 IC_GetIndex(UINT8 *prog_no, UINT8 *index, UINT8 Group_id)CASDRVFUNCPOSTFIX;
INT32 IC_GetAcMap(UINT8 *buffer, UINT8 index)CASDRVFUNCPOSTFIX;
INT32 IC_SetACMap2IC(UINT8 index, UINT8 *buffer)CASDRVFUNCPOSTFIX;
INT32 IC_SetACMap2Buffer(UINT8 index, UINT8 *ac, UINT8 *buffer)CASDRVFUNCPOSTFIX;
INT32 IC_DecryptCW(UINT8 *encrypted_cw,  UINT8 sk_i, UINT8 *decrypted_cw)CASDRVFUNCPOSTFIX;
INT32 hcn_get_atr(void)CASDRVFUNCPOSTFIX;
INT32 IC_GetIC_NO(UINT8 *ic_no)CASDRVFUNCPOSTFIX;	//max 99999999
INT32 IC_Get_Chip_data(UINT8 *send_data,UINT8 *recv_data)CASDRVFUNCPOSTFIX;	//max 99999999
INT32 IC_SetIC_NO(UINT8 *ic_no)CASDRVFUNCPOSTFIX;
INT32 IC_GetSTB_NO(UINT8 *stb_no)CASDRVFUNCPOSTFIX;
INT32 IC_SetSTB_NO(UINT8 *stb_no)CASDRVFUNCPOSTFIX;
INT32 IC_GetNet_ID(UINT8 *net_id)CASDRVFUNCPOSTFIX;	//net_id 4 bytes
INT32 IC_SetNet_ID(UINT8 *net_id)CASDRVFUNCPOSTFIX;
INT32 IC_GetCAS_ID(UINT8 *cas_id)CASDRVFUNCPOSTFIX;
INT32 IC_SetCAS_ID(UINT8 *cas_id)CASDRVFUNCPOSTFIX;
INT32 IC_GetCASVersion(UINT8 *Version)CASDRVFUNCPOSTFIX;
INT32 IC_SetCASVersion(UINT8 *version)CASDRVFUNCPOSTFIX;
INT32 IC_GetWorkTime(UINT8 *work_time)CASDRVFUNCPOSTFIX;
INT32 IC_SetWorkTime(UINT8 *work_time)CASDRVFUNCPOSTFIX;
INT32 IC_GetAdultLevel(UINT8 *adult_level)CASDRVFUNCPOSTFIX;
INT32 IC_SetAdultLevel(UINT8 adult_level)CASDRVFUNCPOSTFIX;
INT32 IC_GetProperty(UINT8 *property)CASDRVFUNCPOSTFIX;
INT32 IC_SetProperty(UINT8 *property)CASDRVFUNCPOSTFIX;
INT32 IC_GetGroupNo(UINT8 *group_no)CASDRVFUNCPOSTFIX;
INT32 IC_SetGroupNo(UINT8 *group_no)CASDRVFUNCPOSTFIX;
INT32 IC_GetCommuneNo(UINT8 *commune_no)CASDRVFUNCPOSTFIX;
INT32 IC_SetCommuneNo(UINT8 *commune_no)CASDRVFUNCPOSTFIX;
INT32 IC_GetMask(UINT8 *mask)CASDRVFUNCPOSTFIX;
INT32 IC_SetMask(UINT8 *mask)CASDRVFUNCPOSTFIX;
INT32 IC_GetPair(UINT8 *pair)CASDRVFUNCPOSTFIX;
INT32 IC_VerifyPIN(UINT8 *pin, UINT8 length, UINT8 *times)CASDRVFUNCPOSTFIX;
INT32 IC_ResetAuthrity(void)CASDRVFUNCPOSTFIX;
INT32 IC_SetPIN(UINT8 *old_pin, UINT8 old_pin_len, UINT8 *new_pin, UINT8 new_pin_len, UINT8 *times)CASDRVFUNCPOSTFIX;
INT32 IC_ResetPIN(void)CASDRVFUNCPOSTFIX;
INT32 IC_GetCreaditCount(UINT8 purse_no, UINT8 *creadit_point)CASDRVFUNCPOSTFIX;
INT32 IC_GetConsumeCount(UINT8 purse_no, UINT8 *consume_point)CASDRVFUNCPOSTFIX;
INT32 IC_SetCreaditCount(UINT8 purse_no,  UINT8 *creadit_point)CASDRVFUNCPOSTFIX;
INT32 IC_SetConsumeCount(UINT8 purse_no,  UINT8 *consume_point)CASDRVFUNCPOSTFIX;
INT32 IC_GetCW_i_TIME(UINT8 index, UINT8 *cw_i, UINT8 *stime, UINT8 *dtime)CASDRVFUNCPOSTFIX;
INT32 IC_SetACTime(UINT8 index, UINT8 *cw_i, UINT8 *etime)CASDRVFUNCPOSTFIX;
INT32 IC_GetStatus(UINT8 *status)CASDRVFUNCPOSTFIX;
INT32 IC_SetStatus(UINT8 *status)CASDRVFUNCPOSTFIX;
INT32 IC_GetCommInfo(UINT8 *common_info)CASDRVFUNCPOSTFIX;
INT32 IC_GetVendor(UINT8 *vendor)CASDRVFUNCPOSTFIX;
INT32 IC_SetVendor(UINT8 *vendor)CASDRVFUNCPOSTFIX;
INT32 IC_SetIPPVInfo(UINT8 *ecm_ippv_info, UINT8 num)CASDRVFUNCPOSTFIX;	//直接覆盖卡里面的所有内容
INT32 IC_GetIPPVInfo_Incard(UINT8 *ippv_infor, UINT8 *num)CASDRVFUNCPOSTFIX;
INT32 IC_Getentile_num(UINT8 *index, UINT8 *num)CASDRVFUNCPOSTFIX;
INT32 IC_Getentile_infor(UINT8 *index, UINT8 num, UINT8 *infor)CASDRVFUNCPOSTFIX;
INT32 IC_SetPACKTime(UINT8 index, UINT8 *buffer)CASDRVFUNCPOSTFIX;
INT32 IC_Getpack_time_num(UINT8 *num)CASDRVFUNCPOSTFIX;
INT32 IC_GetPACKTime(UINT8 *pack_time, UINT8 *num)CASDRVFUNCPOSTFIX;
INT32 IC_GetZFJFlag( UINT8 *info)CASDRVFUNCPOSTFIX;
INT32 IC_SetFJSQTS( UINT8 *buffer)CASDRVFUNCPOSTFIX;
void	GetNdate(  UINT8 *date_hex, int n, UINT8 *gdate_hex ) CASDRVFUNCPOSTFIX;
void AsciiStrToHexStr(char *dest,char *source,unsigned int length) CASDRVFUNCPOSTFIX; //length必须为偶数 指ASCII字符位数
void HexStrToAsciiStr(char *dest,char *source,unsigned int length) CASDRVFUNCPOSTFIX; //length为要转换的16进制字的字节数
int Szm_IsLeapYear(int year) CASDRVFUNCPOSTFIX;
INT32 IC_Set_Chip_PACKTime(UINT8 index, UINT8 *buffer)CASDRVFUNCPOSTFIX;
INT32 IC_Get_Chip_PACKTime()CASDRVFUNCPOSTFIX;
INT32 IC_Get_Chip_Num(UINT8 *buffer)CASDRVFUNCPOSTFIX;  //返回4字节芯片号
INT32 IC_Get_Chip_Key(UINT8 index,UINT8 *buffer)CASDRVFUNCPOSTFIX;  //返回4字节KEY
#endif

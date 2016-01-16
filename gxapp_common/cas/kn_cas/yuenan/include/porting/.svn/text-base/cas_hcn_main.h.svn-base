#ifndef CAS_HCN_MAIN_H
#define CAS_HCN_MAIN_H

#include "cas_hcn.h"



#define NULL	((void *)0)
#define max_cardlabel_name 10

#define SECTION_BUFFER_SIZE		1024
#define S_SECTION_BUFFER_SIZE	4096
#define ECM_PID_DEEP 			2
#define HCNCA_IDLE					(UINT8)0
#define HCNCA_RUNING				(UINT8)1

#define  MAX_HASTEN_LEN			(98)
#define  MAX_EMAIL_LEN			(1+2+2+4+1+40+2+1024+1+20)
#define  MAX_MVOSD_LEN			(15+255/2*2+10+255*2)

#define  MAX_EMAIL_COUNT		(1)
#define  MAX_MVOSD_COUNT		(5)
#define  MAX_EMAIL_RECORD		(512)
#define  MAX_MVOSD_RECORD		(512)
#define  MAX_ENTITLE_NUM		(128)
#define  MAX_ippvincard_NUM		(60*5)

enum Current_ippv_status
{
	ippv_bought_no= 0,
	ippv_bought_yes
};
enum data_filter_msg
{
	needing = 0 ,
	dealing,
};

enum
{
	uninsert = 0,
	initial_error ,
	initial_success ,
	initial_ing
};
enum cas_msg_type 
{//to ca core
	CAS_MSG_NULL = 0,	
	CAS_MSG_ECM_START,		/*change to a scrambled channel*/
	CAS_MSG_EMM_START,
	CAS_MSG_STOP,		/*change to a FTA channel*/
/*extend for smartcard*/
	CAS_MSG_SMC_INSERT,/*insert card*/
	CAS_MSG_SMC_REMOVE,/*remove card*/
};
enum cas_msg_type_core
{
	CAS_MSG_CAT=10, 	/*got a CAT, (try to handle cas_filter within the main_thread)*/
	CAS_MSG_ECM,		/*get a ecm data*/
	CAS_MSG_EMM82,		/*get a emm data*/
	CAS_MSG_EMM83,		/*get a emm data*/	
	CAS_MSG_TIMEOUT,	/*get section timeout*/
};
enum cas_hide_message	//隐藏式消息
{
	CAS_PIN_VERIFY = 20 ,
	CAS_PIN_MADIFY,
	CAS_SET_WORK_TIME,
	CAS_GET_WORK_TIME,
	CAS_GET_ADULT_LEVEL,
	CAS_SET_ADULT_LEVEL,
	/**/
	CAS_GET_ENTITLE_INFOR,
	CAS_GET_ENTITLE_NUM,
	CAS_GET_PURSE_INFOR,	
	CAS_IPPV_BUY,
	CAS_GET_CHILDPARENTSMC_INFO,
	CAS_REQUEST_BINDING_BY_MANUAL,
	CAS_HIDE_RESET=0xfc,
};

typedef struct camsg_data
{
	UINT8			msg[20];
}hcn_msg_data;

typedef struct data_deal_satus
{
	UINT8			ECM;
	UINT8			EMM82;
	UINT8			EMM83;
	UINT8			CAT;
}DATA_DEAL_SASTUS;//过滤控制结构体
enum cas_mask_type
{
	hcn_ecm = 0 ,
	hcn_emm82=1,
	hcn_emm83,
	hcn_emm82_83,
	hcn_cat,
};
enum refresh_status
{
	refresh_none = 0,
	refresh_show ,
	refresh_close
};
typedef struct hcncas_icinfor_t
{
	UINT8 card_label[max_cardlabel_name];	
	UINT8 card_casversion[4];//智能卡版本
	UINT8 parent_rating;//成人级别
	UINT8 pairse;//配对状态
	UINT8 card_status;
	UINT8 ic_no[4];	 //卡号	
	UINT8 casid[4]; //CA系统ID
	UINT8 property[2];//0x82-0x11 属性码
	UINT8 group_id[2];//0x82-0x12 集团号
	UINT8 zone_id[4]; //0x82-0x12 小区号
	UINT8 work_Section[4]; //ecm 用到工作时间
	UINT8 netid[2]; //网络ID
	UINT8 version_card[8][2];//各种记录文件版本
	UINT8 current_time[4];	//根据ECM获得的当前时间
	UINT32 vendorid;	//运营商ID
	UINT8 product_ver;	//业务包版本
}hcncas_icinfor;

typedef struct hcncas_icin_kn1_t          //第一次插入的卡数据  （上电）
{
	UINT8 stat;          //是否第一次读卡，第一次读卡后置为0xAa
	UINT8 zf_flag;		 //主副机标志 0xa5副机 
	UINT8 ic_no[4];	 //卡号				
}hcncas_icin_kn1;

typedef struct hcncas_kn_zfj_info1          //
{
	UINT8 zf_flag;		 //主副机标志 0xa5副机 
	UINT8 zjic_no[4];	 //主机卡号
	UINT8 fjsqrq[4];     //副机授权日期
	UINT8 fjsqts;        //副机授权天数
	UINT8 current_date[4];  //当前日期
	UINT8 current_shijian[3];  //当前时间
	UINT8 sq_shijian[7];  //授权时间
	UINT8 fjjh_flag;      //副机激活标志
}hcncas_kn_zfj_info;

//单用户授权头
typedef struct tagHEMMS
{
	UINT8 table_id;
	UINT8 total_length[2];
	UINT8 cas_id[2];
	UINT8 ic_no[4];
	UINT8 total_ent;	//授权个数
}HEMMS,*PHEMMS;

void hcn_send2ca_core(UINT8 type,UINT8 *DATA,UINT32 LEN,UINT32 TIME_DELAY)CASDRVFUNCPOSTFIX;
static void hcn_deal_ecm(hcncas_icinfor* curcainfo)CASDRVFUNCPOSTFIX;
static void hcn_deal_emm82(hcncas_icinfor* curcainfo)CASDRVFUNCPOSTFIX;
#endif

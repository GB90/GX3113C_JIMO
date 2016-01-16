#ifndef CAS_HCN_H
#define CAS_HCN_H

typedef signed char INT8;
typedef unsigned char UINT8;
typedef signed short INT16;
typedef unsigned short UINT16;	
typedef int INT32;	
typedef unsigned int UINT32;	
#define  time_t int 
//#define bool  unsigned char 

typedef int cas_channel_id;
typedef int cas_filter_id;


#define MAX_SEC_MASK_LEN 	16
#define INVALID_FILTER_ID	-1

#define HCN_RET_TRUE  		1
#define HCN_RET_FALSE 		0
#define HCN_RET_SUCCESS  	0
#define HCN_RET_FAILURE  	1

#define HCN_OUT
#define HCN_IN
#define HCN_IO

#ifdef logo_liangying
#define HAS_JIAMI_CHIP
#endif
#ifdef logo_xiancheng
#define HAS_JIAMI_CHIP
#endif

/*这个宏要根据不同的平台而定,
一般来说C51需要这样定义，其他平台可以定为空
*/
//#define CASDRVFUNCPOSTFIX large reentrant 
#define CASDRVFUNCPOSTFIX


typedef enum  
{	//显示通知
	DISP_NO_MSG = 0,		//无CA信息
	DISP_PLS_INSERT_SMC,	//请插卡
	DISP_SMC_INSERT,		//智能卡插入
	DISP_SMC_REMOVED,		//智能卡取出
	DISP_SMC_INIT_SUCCEES,	//智能卡初始化成功
	DISP_SMC_INIT_FAIL,		//智能卡初始化失败
	DISP_SMC_READ_ERROR, 	//读卡出错
	DISP_CARD_NOMATCH,      //机卡不配对
	DISP_MATCH_SUCCESS,		//配对成功
	DISP_MATCH_FAIL,		//配对失败
	DISP_NO_RIGHT,			//无节目权限0   //10 index
	DISP_TIMEEXPIRED,		//有效期已过
	DISP_INSTANCE_CLOSE,	//即时关断
	DISP_OUT_WORKTIME,		//不在工作时段
	DISP_MASK_LIMITED,		//区域限播
	DISP_OUT_LEVEL,			//不满足收看级别
	DISP_TYPE_LIMIT,		//录像控制限制
	DISP_PPV,				//ppv节目
	DISP_PPC,				//ppc节目
	DISP_PPV_AWARD_SUCCESS,	//ppv节目授权成功
	DISP_PPV_AWARD_FAIL,	//ppv节目授权失败//20 index
	DISP_PPC_AWARD_SUCCESS,	//ppc节目授权成功
	DISP_PPC_AWARD_FAIL,	//ppc节目授权失败
	DISP_ZFJPD_SUCCESS,	   //主副机配对成功
	DISP_REMAIN_DAY,		//收看有效期还有 9天，请续费
//////////////////////
	DISP_CANCEL_MATCH,      //已取消配对
	DISP_CANCEL_MATCHFAIL,  //取消配对失败
	DISP_SMC_NOTOPEN,//27		//智能卡未启用
    DISP_CREDIT_CONUT_FULL,	//点数已满
    DISP_UNRECOGNISED_CARD, //无法识别智能卡
    DISP_ERRORVERSION_CARD, //智能卡版本错误
    DISP_ippv_eventzreo, 	//ippv事件号为零
 /////////////////////////      
	//命令通知
	HCNCAS_MAIL_NOTIFY ,//邮件
	HCNCAS_SUBTITLE_NOTIFY,	//字幕
	HCNCAS_DISPALY_FINGER_NOTIFY,//显示指纹
	HCNCAS_STOP_FINGER_NOTIFY,	//停止指纹	
	HCNCAS_DISPLAY_URGE_FEE_NOTIFY,//display upge_fee
	HCNCAS_STOP_URGE_FEE_NOTIFY,//stop display  upge_fee	
	HCNCAS_IPPV_SHOW,		//IPPV show
	HCNCAS_IPPV_CLOSE,		//IPPV close
	HCNCAS_REFILT_PMT,		//重新处理PMT，如果符合条件则重新启动CA
}cas_disp_msg;

#define MAX_PROVIDER_NAME 20
#define MAX_CARDPROVIDER_NAME 10
#define HCN_CHARACTER_MAX_NUM	(254+1)//字节


/****SMC info  for display***/
typedef struct hcnca_SMCinfor
{
	UINT8 CARD_LABEL[MAX_CARDPROVIDER_NAME];	//运营商
	UINT8 CAM_VER[4];	//CAS版本
	UINT8 CARD_STATUS;	//智能卡状态
	UINT8 PAIR_STATUS;	//配对状态
	UINT8 PARENTAL_RATING;	//成人级别
	UINT32 IC_NO;	//智能卡卡号
	UINT8 OPERATION_ID[3];	//运营商ID
}hcncas_smcinfo;

/****entitle  info  for display***/
typedef struct hcncastb_ppcentitle
{
	UINT8 index[1];		//ppc节目的授权信息id
	UINT8 Start_Time[4];	//开始时间
	UINT8 End_Time[4];	//结束时间
}HCNCAS_PPCENTITLE;
/****purse  info  for display***/
typedef struct hcncastb_purse
{
	UINT8 provider_id;	//运营商钱包ID
	INT32 credit;			//信誉点数
	INT32 consume;		//消费点数
}HCNCAS_PURSE;
/****ippv  info  for display***/
typedef struct hcncastb_ippv
{
	UINT8 provider_id;	//节目运营商ID
	UINT8 ippeventid[2];	//节目事件号
	UINT8 tokenprice;		//订购价格，点数表示
	UINT8 overtime[4];		//本次时间结束时间
}HCNCAS_IPPV;
/****ippv  info  for display***/
typedef struct hcncastb_ippv_all
{
	UINT8 ippeventid[2];//event id
	UINT8 overtime[4];	//end time 
}HCNCAS_IPPV_ALL;

typedef struct HCN_RESTRICT{
	UINT8  mask[MAX_SEC_MASK_LEN];
	UINT8  value[MAX_SEC_MASK_LEN];
	UINT8  mask_len;
}hcn_restrict;
/****  for move osd***/
typedef struct hcncas_osd_g{	
	UINT8 id;
	UINT8 font_color;
	UINT8 bg_color;
	UINT8 subdata[HCN_CHARACTER_MAX_NUM];	
	UINT8 len;
}hcncas_osd;

/*time struct*/
typedef struct Time_T
{
	UINT16 year;	
	UINT8  month;
	UINT8  day;	
	UINT8  hour;
	UINT8  min;
	UINT8 sec;		
}ca_time;

/*
说明 ：
	对信号量进行初始化
调用的例子：
	UINT32  Semaphore1;
	hcncas_create_signal(&Semaphore1,1);
*/
void hcncas_create_signal(UINT32* pSemaphore ,unsigned char bInitVal)CASDRVFUNCPOSTFIX;
void hcncas_send_signal(UINT32* pSemaphore)CASDRVFUNCPOSTFIX;
void hcncas_get_signal(UINT32* pSemaphore)CASDRVFUNCPOSTFIX;
void hcncas_del_signal(UINT32* pSemaphore)CASDRVFUNCPOSTFIX;
/*
说明 ：
	挂起当前任务，time 超时控制ms
调用的例子：
	hcncas_task_sleep(5);	
*/
void hcncas_task_sleep(UINT16 time)CASDRVFUNCPOSTFIX;
/*
说明 ：
	创建一个 消息队列
调用的例子：
	UINT32  msgid;
	hcncas_create_msgqe(&msgid,"HCNCAS");
*/
void hcncas_create_msgqe(UINT32 *message_queue_id ,UINT8 *queue_name)CASDRVFUNCPOSTFIX;
void hcncas_send_queue(UINT32 QUEUE_ID,UINT8 *DATA,UINT32 LEN,UINT32 TIME_DELAY)CASDRVFUNCPOSTFIX;
INT32 hcncas_receive_queue(UINT32 QUEUE_ID,UINT8 *DATA,UINT32 *LEN,UINT32 TIME_DELAY)CASDRVFUNCPOSTFIX;
/*
说明 ：
	创建一个内核任务
调用的例子：
	hcncas_register_task("HCNCASTASK",8,(void *)ca_man,NULL,0x4000);
*/
void hcncas_register_task(char *Name, UINT8 byPriority,void* pTaskFun,void* pParam, UINT32 nStackSize)CASDRVFUNCPOSTFIX;
/*
说明 ：内存管理
调用的例子:
	char  test[]="ABCDEFGHABCDEFGH";
	char* pTmp = (char*)hcncas_malloc(sizeof(test)+1);
	if(pTmp==NULL) return;
	hcncas_memset(pTmp,0,sizeof(test)+1);
	hcncas_memcpy(pTmp,test,sizeof(test));
	hcncas_free(pTmp);
*/
void hcncas_memcpy(void *des,void *src,UINT32 LEN)CASDRVFUNCPOSTFIX;
void hcncas_memset(void *des,UINT8 value,UINT32 LEN)CASDRVFUNCPOSTFIX;
INT32 hcncas_memcmp(void *src1,void *src2,UINT32 LEN)CASDRVFUNCPOSTFIX;
void* hcncas_malloc(UINT32 byBufSize)CASDRVFUNCPOSTFIX;
void  hcncas_free(void* pBuf)CASDRVFUNCPOSTFIX;
/*
说明 ：判断邮件数是否已满
调用的例子:
	if(hcncas_is_email_full(0) == 0)
		printf("邮件数已是最大值");
	else
	       printf("邮件数未达到最大值");
*/
UINT8 hcncas_is_email_full(void)CASDRVFUNCPOSTFIX;
/*
说明 ：通道过滤器管理
调用的例子:
	cas_channel_id cid;
	cas_filter_id fid;
	cid = hcncas_allocatechannel(emmpid,10);
	if(cid < 0)
	      printf("error");
	fid = hcncas_allocatefilter(cid,&mask);
	if(fid < 0)
	      printf("error");	
	hcncas_startchannel(cid);

	hcncas_freefilter(fid);
	hcncas_freechannel(cid);
*/
cas_channel_id hcncas_allocatechannel(UINT16 PID,UINT8 wait_seconds)CASDRVFUNCPOSTFIX;
INT8 hcncas_freechannel(cas_channel_id channel_id)CASDRVFUNCPOSTFIX;
INT8 hcncas_startchannel(cas_channel_id channel_id)CASDRVFUNCPOSTFIX;
cas_filter_id hcncas_allocatefilter(unsigned char byReqID,cas_channel_id channel_id, hcn_restrict* MASK)CASDRVFUNCPOSTFIX;
UINT8  hcncas_freefilter(cas_filter_id filter_id)CASDRVFUNCPOSTFIX;
void hcncas_setcw(UINT8 *pbyOddKey,UINT8 *pbyEvenKey,UINT16 ECM_PID)CASDRVFUNCPOSTFIX;
/*
说明 ：智能卡复位
调用的例子:

	if(hcncas_smc_reset() == 0)
		printf("reset ok");

*/
INT32 hcncas_smc_reset(void)CASDRVFUNCPOSTFIX;
UINT8 hcncas_smc_status(void)CASDRVFUNCPOSTFIX;//:insert status;0:remove status
INT32 hcncas_smc_write(UINT8 *apdu_header, const UINT8 *apdu_body, UINT8 *sw1sw2)CASDRVFUNCPOSTFIX;
INT32 hcncas_smc_read(UINT8 *apdu_header, UINT8 *apdu_body, UINT8 *sw1sw2 )CASDRVFUNCPOSTFIX;

INT32 hcncas_chip_read(UINT8 *send_data, UINT8 *recv_data )CASDRVFUNCPOSTFIX;
/*
说明 ：得到网络ID号
调用的例子:

	UINT8 netid[2];
	hcncas_get_netid(netid);
*/
void hcncas_get_netid(UINT8 *net_id)CASDRVFUNCPOSTFIX;
/*
说明 ：FLASH操作,内核需要存储3块内容，详见移植文档
调用的例子:

*/
INT8 hcncas_save_icno(UINT8 *src1,UINT8 LEN)CASDRVFUNCPOSTFIX;
INT8 hcncas_read_icno(UINT8 *src1,UINT8 LEN)CASDRVFUNCPOSTFIX;
INT8 hcncas_save_pack(UINT8 *src1,UINT16 LEN)CASDRVFUNCPOSTFIX;
INT8 hcncas_get_pack(UINT8 *src1,UINT16 LEN)CASDRVFUNCPOSTFIX;
INT32 hcncas_savepin_buffer(UINT8 *src1)CASDRVFUNCPOSTFIX;
INT32 hcncas_getpin_buffer(UINT8* src1)CASDRVFUNCPOSTFIX;
/*
说明 ：串口打印,必要的邋CA调试信息，正式版本建议关闭它
调用的例子:

*/
char hcncas_debug(char *Format_p, ...)CASDRVFUNCPOSTFIX;
/*
说明 ：界面管理，详见移植文档
调用的例子:

*/
UINT8 hcncas_report_status(UINT8* mesg,UINT32 para1,UINT32 para2)CASDRVFUNCPOSTFIX;
/*------------------------以上接口是STB提供给CA_LIB----------------------------------------*/


/*------------------------以下接口是CA_LIB提供给STB----------------------------------------*/
/*
说明 ：CA将创建几个任务和消息队列
*/
UINT8 hcncas_init(void)CASDRVFUNCPOSTFIX;
/*
说明 ：得到内核的版本号
调用的例子:
		INT8 version[8];
		hcncas_getca_version(version);
		printf("%s",version);
*/
void hcncas_getca_version(INT8* ver)CASDRVFUNCPOSTFIX;
/*
说明 ：得到智能卡状态
调用的例子:

*/
UINT8 hcncas_get_card_status(void)CASDRVFUNCPOSTFIX;
void  hcncas_card_insert(void)CASDRVFUNCPOSTFIX;
void  hcncas_card_remove(void)CASDRVFUNCPOSTFIX;
void hcncas_time_to_date(time_t *plTime, ca_time* ptDate)CASDRVFUNCPOSTFIX;
UINT8  hcncas_get_section(UINT16 PID, UINT8 *section_data)CASDRVFUNCPOSTFIX;
UINT8 hcncas_ismatch(UINT8* ca_systemid)CASDRVFUNCPOSTFIX;
/*
说明 ：设置或清楚pid 
调用的例子:
		hcncas_set_ecmpid(ecmpid);
		hcncas_set_emmpid(emmpid);
*/
UINT8 hcncas_set_ecmpid(UINT16 ecmpid)CASDRVFUNCPOSTFIX;
void hcncas_clean_ecmpid(void)CASDRVFUNCPOSTFIX;
void hcncas_set_emmpid(UINT16 emmpid)CASDRVFUNCPOSTFIX;
void hcncas_clean_emmpid(void)CASDRVFUNCPOSTFIX;
void hcncas_clear_interface(void)CASDRVFUNCPOSTFIX;
void hcncas_ecm_stop(void)CASDRVFUNCPOSTFIX;
void hcncas_emm_stop(void)CASDRVFUNCPOSTFIX;
/*
说明 ：处理邮件信息,详见移植文档
调用的例子:
		UINT8 num;
		num=hcncas_get_email_count();
		hcncas_get_email(ptr,num)；
*/
UINT8 hcncas_get_email_count(void)CASDRVFUNCPOSTFIX;
void hcncas_get_email(UINT8 *email,UINT8 count_in)CASDRVFUNCPOSTFIX;
/*
说明 ：屏显信息处理,详见移植文档
调用的例子:
		UINT8 num;
		num=hcncas_get_email_count();
		hcncas_get_email(ptr,num)；
*/
INT32 hcncas_get_move_osd(hcncas_osd* osd)CASDRVFUNCPOSTFIX;
/*
说明 ：得到用户信息处理,详见移植文档
调用的例子:
		hcncas_smcinfo card;
		hcncas_get_cardinfo(&card);

*/
UINT8 hcncas_get_cardinfo(hcncas_smcinfo *user)CASDRVFUNCPOSTFIX;
/*
说明 ：催费信息处理,详见移植文档
调用的例子:
		UINT16 len;
		UINT8 *bu=NULL;
		len=hcncas_get_hasten_len();
		bu=(UINT8 *)hcncas_malloc(len);
		printf("%s",bu);
*/
UINT16 hcncas_get_hasten_len(void)CASDRVFUNCPOSTFIX;
UINT16 hcncas_get_hasten(UINT8 *user)CASDRVFUNCPOSTFIX;
/*
说明 ：读取授权信息处理,详见移植文档
调用的例子:

*/
INT32 hcncas_get_entitle(HCNCAS_PPCENTITLE *entitle_infor,UINT8 *num)CASDRVFUNCPOSTFIX;
/*
说明 ：读取钱包信息处理,详见移植文档
调用的例子:

*/
INT32 hcncas_get_purse(UINT8 *credit_count,UINT8 *consume_Count)CASDRVFUNCPOSTFIX;
/*
说明 ：读取IPPV信息处理,详见移植文档
调用的例子:

*/
INT32 hcncas_get_ippvinfo(UINT8 * infor,UINT8* num)CASDRVFUNCPOSTFIX;	
/*
说明 ：读取授权信息处理,详见移植文档
调用的例子:

*/
INT32 hcncas_get_worktime(UINT8 *work_time)CASDRVFUNCPOSTFIX;
INT32 hcncas_verify_pin(UINT8 *pin,  UINT8 length, UINT8 *times)CASDRVFUNCPOSTFIX;
INT32 hcncas_modify_pin(UINT8* old_pin,  UINT8 old_pin_len,UINT8* new_pin,  UINT8 new_pin_len,UINT8* times)CASDRVFUNCPOSTFIX;
INT32 hcncas_set_worktime(UINT8 *work_time)CASDRVFUNCPOSTFIX;
INT32 hcncas_get_adultlevel(UINT8 *pa)CASDRVFUNCPOSTFIX;
INT32 hcncas_set_adultlevel(UINT8 work_time)CASDRVFUNCPOSTFIX;
UINT8 hcncas_get_is_finger(void)CASDRVFUNCPOSTFIX;
INT32 hcncas_get_ippvbuy_info(HCNCAS_IPPV *user)CASDRVFUNCPOSTFIX;
INT32 hcncas_buy_ippv(UINT8 provider_id, UINT8 *event_id, UINT8 tokenprice, UINT8 *overtime)CASDRVFUNCPOSTFIX;
void hcncas_set_netzoneid(UINT8 netzone)CASDRVFUNCPOSTFIX;
void hcncas_get_remain_day(UINT8* remain_day)CASDRVFUNCPOSTFIX;
#endif

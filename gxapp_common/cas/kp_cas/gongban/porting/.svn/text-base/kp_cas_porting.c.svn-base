
#include <gxos/gxcore_os_core.h>
#include "KPAPP.h"
#include "kp_cas.h"
#include "app_common_flash.h"
#include "app_common_porting_ca_os.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_porting_stb_api.h"
#include "app_kp_cas_api_demux.h"
#include "app_kp_cas_api_event.h"
#include "app_kp_cas_api_rolling_message.h"
#include "app_kp_cas_api_email.h"

#define MAX_SEARIAL_LEN      (8)
#define MEQUEUE_MAX_NUM      (10)
#define MEQUEUE_DATA_SIZE    (1024)

typedef struct task_mequeue_tag
{
	int 	 size;//数据单元大小
	handle_t    m_hMqueueHandle;
    void     *pbuf;
    int      curLen;//当前数据长度
}TASK_MQUEUE_T;

TASK_MQUEUE_T        g_QueueList[MEQUEUE_MAX_NUM];


/***************************************
* @4APP实现,提供给KPlus使用
***************************************/
//@Queue
U8   KPAPPCA_QueueCreate(U8* queueName, U16 maxElements, U16 elementSize, KP_MSG_QUEUE* queueID)
{
    int i;
    for(i = 0; i< MEQUEUE_MAX_NUM;i++)
    {
        if(g_QueueList[i].m_hMqueueHandle == 0)
        {
            break;
        }
    }

    if( i== MEQUEUE_MAX_NUM)
    {
        CAS_Dbg("KPAPPCA_QueueCreate failed,is full !\n");
        return KP_RET_FAIL;
    }
    
    maxElements = 10;
    elementSize = sizeof(SECTION_DATA);
 	if(GXCORE_SUCCESS != GxCore_QueueCreate(&g_QueueList[i].m_hMqueueHandle,maxElements,elementSize))
	{
        CAS_Dbg("KPAPPCA_QueueCreate failed !\n");
        return KP_RET_FAIL;
	}
    g_QueueList[i].size = elementSize;
    *queueID=(KP_MSG_QUEUE)i;

    g_QueueList[i].curLen = 0;
    g_QueueList[i].pbuf = KPAPPCA_Alloc(MEQUEUE_DATA_SIZE);
    memset(g_QueueList[i].pbuf,0,MEQUEUE_DATA_SIZE);
    
    CAS_Dbg("KPAPPCA_QueueCreate ok,[%s],[dep:%d,size:%d]queueID:%d !\n",(char *)queueName,maxElements,elementSize,*queueID);
	return KP_RET_SUCCESS;
}
U8   KPAPPCA_QueueDestroy(KP_MSG_QUEUE queueID)
{
    if(queueID >= MEQUEUE_MAX_NUM || g_QueueList[queueID].m_hMqueueHandle==0)
	{
		return KP_RET_FAIL;
	}
    if(GXCORE_SUCCESS != GxCore_QueueDelete(g_QueueList[queueID].m_hMqueueHandle))
    {
        CAS_Dbg("KPAPPCA_QueueDestroy failed !\n");
        return KP_RET_FAIL;
    }
    g_QueueList[queueID].m_hMqueueHandle = 0;
    g_QueueList[queueID].curLen = 0;
    KPAPPCA_Free(g_QueueList[queueID].pbuf);
    g_QueueList[queueID].pbuf = NULL;
    CAS_Dbg("KPAPPCA_QueueDestroy.\n");
    return KP_RET_SUCCESS;
}
U8   KPAPPCA_QueueEmpty(KP_MSG_QUEUE queueID)
{
    //U8 data[512];
	int         ret;
    do{
        
        ret = KPAPPCA_QueueRecv(queueID,KP_QUEUE_NONBLOCKING,20,NULL);
        if(ret == GXCORE_SUCCESS)
        {
            CAS_Dbg("Queue[%d] is not empty.\n",queueID);

            //KP_DUMP("[data] ->", data, (data[1]<<8|data[2])+3);
        }
        
    }while((ret == GXCORE_SUCCESS));

    g_QueueList[queueID].curLen = 0;
    memset(g_QueueList[queueID].pbuf,0,MEQUEUE_DATA_SIZE);
    
    CAS_Dbg("KPAPPCA_QueueEmpty,Queue is empty.\n");
    return KP_RET_SUCCESS;
}
U8   KPAPPCA_QueueSend(KP_MSG_QUEUE queueID, U8 *data)
{
    int ret;
    int len = (data[1]<<8|data[2])+3;

    SECTION_DATA sec_data;
        
	if(queueID>=MEQUEUE_MAX_NUM || g_QueueList[queueID].m_hMqueueHandle==0)
	{
		return KP_RET_FAIL;
	}
    if(len <= 0)
    {
        CAS_Dbg("KPAPPCA_QueueSend error \n");
        return KP_RET_FAIL;
    }

    if(len == g_QueueList[queueID].curLen 
        && !memcmp(g_QueueList[queueID].pbuf,data,len))
    {
        //CAS_Dbg("QueueList[%d]>The same data.\n",queueID);
        return KP_RET_SUCCESS;
    }
    
    memset(&sec_data,0,sizeof(SECTION_DATA));
    sec_data.datelen = len;
    if((sec_data.pdata = (U8 *)KPAPPCA_Alloc(len))==NULL)
    {
        return KP_RET_FAIL;
    }
    
    KPAPPCA_Memcpy(sec_data.pdata,data,len);
    //KP_Dbg("KPAPPCA_QueueSend[queueID:%d],len:%d\n",queueID,len);
    
    ret = GxCore_QueuePut( g_QueueList[queueID].m_hMqueueHandle, (void *)&sec_data, sizeof(SECTION_DATA),0);
	if( GXCORE_SUCCESS !=  ret)
	{
		CAS_Dbg("KPAPPCA_QueueSend error \n");
        KPAPPCA_Free(sec_data.pdata);
		return KP_RET_FAIL;
	}
    
    KPAPPCA_Memcpy(g_QueueList[queueID].pbuf,data,len);
    g_QueueList[queueID].curLen = len;
    
	return KP_RET_SUCCESS;
}
U8   KPAPPCA_QueueRecv(KP_MSG_QUEUE queueID, U8 mode, U32 timeout, U8 *data)
{
    uint32_t    sz=0;
	int32_t     Timeout;
	int         ret;
    SECTION_DATA sec_data;
    
    if(queueID>=MEQUEUE_MAX_NUM || g_QueueList[queueID].m_hMqueueHandle==0)
	{
		return KP_RET_FAIL;
	}

    //KP_Dbg("KPAPPCA_QueueRecv[queueID:%d],mode:%d,timeout:%d\n",queueID,mode,timeout);
    if(KP_QUEUE_NONBLOCKING == mode)//不阻塞
    {
        if(timeout == 0)
        {
//#if defined(ECOS_OS)
//            Timeout = -1;
//#elif defined(LINUX_OS)
            Timeout = 10;
//#endif
        }
        else
        {
            Timeout = (int32_t)timeout;
        }
    }
    else 
    {
#if defined(ECOS_OS)
        Timeout = -1;
#elif defined(LINUX_OS)
        Timeout = 0;
#endif
    }
    memset(&sec_data,0,sizeof(SECTION_DATA));
	ret = GxCore_QueueGet(g_QueueList[queueID].m_hMqueueHandle, &sec_data, sizeof(SECTION_DATA), &sz,Timeout);
	if (ret == GXCORE_SUCCESS && sz != 0)
	{
        if(sec_data.pdata!=NULL && sec_data.datelen>0)
        {
            if(data)
            {
                KPAPPCA_Memcpy(data,sec_data.pdata,sec_data.datelen);
            }
            KPAPPCA_Free(sec_data.pdata);
            
            //KP_Dbg("KPAPPCA_QueueRecv[queueID:%d] ok,len:%d\n",queueID,sz);
            //KP_DUMP("[data] ->", data, (data[1]<<8|data[2])+3);
            /*if(data[0]== 0x50 || data[0] == 0x53 )
            {
                KP_DUMP("[data] ->", data, (data[1]<<8|data[2])+3);
            }*/
        }
        
		return KP_RET_SUCCESS;
	}
	else
	{
		//KP_Dbg("KPAPPCA_QueueRecv[queueID:%d] Return value is error...\n",queueID);
		return KP_RET_FAIL;
	}
}

//@Memory
void *KPAPPCA_Alloc(U32 size)
{
    return app_porting_ca_os_malloc(size);
}
void KPAPPCA_Free(void *p)
{
    app_porting_ca_os_free(p);
}
U8   KPAPPCA_Memcpy(void *dptr, const void *sptr, U16 len)
{
    memcpy(dptr,sptr,len);
    return KP_RET_SUCCESS;
}
//@获取User id + STBID + CHIPID, buf have 8 bytes
U16  KPAPPCA_GetLicenceid(U8 *buf)
{ 
#ifndef KP_DEBUG
    U8 caUser[MAX_SEARIAL_LEN+1]={0};
	extern int get_licenceid(unsigned char *id,int len);
    if(get_licenceid(caUser,MAX_SEARIAL_LEN)>0)
    {
        memcpy(buf, caUser, MAX_SEARIAL_LEN);
        return KP_RET_SUCCESS;
    }
    return KP_RET_FAIL;
#else
    U8 temp[]={0X99,0X01,0X23,0X45,0X00,0X00,0X01,0X24};
    memcpy(buf, temp, MAX_SEARIAL_LEN);
    CAS_DUMP("Licenceid:", buf, MAX_SEARIAL_LEN);
    return KP_RET_SUCCESS;
#endif

	
}
U16  KPAPPCA_GetSTBid(U8 *buf)
{
#if 0    
    char caUser[MAX_SEARIAL_LEN+1]={0};
	app_flash_get_serial_number(0,(char *)caUser,MAX_SEARIAL_LEN);
    memcpy(buf, caUser, MAX_SEARIAL_LEN);
#endif
	return KP_RET_SUCCESS;
}
U16  KPAPPCA_GetCHIPid(U8 *buf)
{
#ifndef KP_DEBUG
    U8 caUser[MAX_SEARIAL_LEN+1]={0};
    extern int get_chipId(unsigned char *id,int len);
    if(get_chipId(caUser,MAX_SEARIAL_LEN)>0)
    {
        memcpy(buf, caUser, MAX_SEARIAL_LEN);
        return KP_RET_SUCCESS;
    }
    return KP_RET_FAIL;
#else
    U8 temp[]={0x8E,0x92,0xA4,0x30,0xDB,0x06,0x1C,0xBB};
    memcpy(buf, temp, MAX_SEARIAL_LEN);
    CAS_DUMP("\nchipid:", buf, MAX_SEARIAL_LEN);
    return KP_RET_SUCCESS;
#endif
    
}

//@输出CAS状态给STB
//ecmpid,为0 时，表示全局状态；非0 时，节目状态
void KPAPPCA_Report(U16 ecmpid, U16 code)
{
    U16  byMessageType = 0xff;
    CAS_Dbg("KPAPPCA_Report,ecmpid:%d,code:%x\n",ecmpid,code);
    switch(code)
    {
        case KPCA_MESSAGE_CANCEL:
			byMessageType = CA_KPCA_MESSAGE_SUCCESS;
			break;
        case KPCA_MESSAGE_NO_ACTIVE:
			byMessageType = CA_KPCA_MESSAGE_NO_ACTIVE;
			break;
        case KPCA_MESSAGE_AREA_NOT_MATCH:
			byMessageType = CA_KPCA_MESSAGE_AREA_NOT_MATCH;
			break;
         case KPCA_MESSAGE_AREA_FORCE:
			byMessageType = CA_KPCA_MESSAGE_AREA_FORCE;
			break;
         case KPCA_MESSAGE_ERROR_PRODUCT:
			byMessageType = CA_KPCA_MESSAGE_ERROR_PRODUCT;
			break;
         case KPCA_MESSAGE_ERROR_PROVIDER:
			byMessageType = CA_KPCA_MESSAGE_ERROR_PROVIDER;
			break;
         case KPCA_MESSAGE_NO_ENTITLE:
			byMessageType = CA_KPCA_MESSAGE_NO_ENTITLE;
			break;
         case KPCA_MESSAGE_ENTITLE_EXPIRED:
			byMessageType = CA_KPCA_MESSAGE_ENTITLE_EXPIRED;
			break;
        case KPCA_MESSAGE_ENTITLE_NOSTART:
			byMessageType = CA_KPCA_MESSAGE_ENTITLE_NOSTART;
			break;
        case KPCA_MESSAGE_PRODUCT_CLASH:
			byMessageType = CA_KPCA_MESSAGE_PRODUCT_CLASH;
			break;
        case KPCA_MESSAGE_SERVICE_FORCE:
			byMessageType = CA_KPCA_MESSAGE_SERVICE_FORCE;
			break;
        case KPCA_MESSAGE_OUT_RATING:
			byMessageType = CA_KPCA_MESSAGE_OUT_RATING;
			break;    
        case KPCA_MESSAGE_OUT_WORKTIME:
			byMessageType = CA_KPCA_MESSAGE_OUT_WORKTIME;
			break;   
        case KPCA_MESSAGE_FREEZE:
			byMessageType = CA_KPCA_MESSAGE_FREEZE;
			break;   
        default:
			break;
    }
    if (0xff != byMessageType)
	{
		if (MSG_POP_COMMON_END+byMessageType+1 >= MESSAGE_MAX_COUNT)
			return ;			
	}
	if ( TRUE != app_play_get_msg_pop_type_state(MSG_POP_COMMON_END+byMessageType+1))
	{
		app_play_clear_ca_msg();
		app_play_set_msg_pop_type(MSG_POP_COMMON_END+byMessageType+1);

	}

	return ;
}

/* 释放私有数据过滤器 */
int KPCA_ReleasePrivateDataFilter( U8  byReqID,U16 wPid )
{
	uint32_t filterId = 0;
	bool releaseChannel = TRUE;
	ca_filter_t sfilter = {0};


   CAS_Dbg("####KPCA_ReleasePrivateDataFilter,PID:%d,TableID:%#x\n",wPid,byReqID);

	/*
	* 查找对应channel id , filter id
	*/

	for (filterId = 0; filterId< MAX_FILTER_COUNT;filterId++)
	{
		app_porting_ca_demux_get_by_filter_id(filterId,&sfilter);
		if (( 0 != sfilter.usedStatus)&&(0 != sfilter.handle)&&(0 != sfilter.channelhandle)
			&&(wPid == sfilter.pid)&&(byReqID == sfilter.byReqID))
		{
			if (0x10 == wPid)
			{
				releaseChannel = FALSE;
			}
			else
			{
				releaseChannel = TRUE;
			}
            
            app_porting_ca_demux_release_filter( filterId, releaseChannel);
            CAS_Dbg("####KPCA_ReleasePrivateDataFilter OK,PID:%d,TableID:%#x \n",wPid,byReqID);

            return 1;
		}
	}

	return -1;
}

//由KPCAAPP_StartECMPid内部调用
//@输出Filter参数给STB
U16  KPAPPCA_StartFilter(U8 FilterID, U16 PID, U16 TableID, U8 Period, U16 *SectByte, U32 QueueID)
{
    ca_filter_t filter = {0};
	bool repeatFlag = TRUE;
	uint8_t     match[18] = {0,};
	uint8_t     mask[18] = {0,};
    int i;
	if(SectByte == NULL )
	{
		return KP_RET_FAIL;
	}


    mask[0] = 0xff;
    match[0] = TableID;
    
    for(i = 0;i<8;i++)
    {
        mask[i+3] = (SectByte[i]&0xff00)>>8;
        match[i+3] = SectByte[i]&0xff;
        
    }

    if(filter_list_search(PID,TableID,NULL) > 0 )
    {
        if(KPCA_ReleasePrivateDataFilter( TableID, PID)>=0)
        {
            filter_list_del(PID,TableID);
        }
    }
    

	filter.byReqID = TableID;
	filter.crcFlag = FALSE;
    //filter.crcFlag = TRUE;
	filter.filterLen = 11;
	memcpy(filter.match,match,11);
	memcpy(filter.mask,mask,11);
	filter.nWaitSeconds = 0;//Period;
	filter.pid = PID;
	filter.equalFlag = TRUE;
	filter.repeatFlag = repeatFlag;
	filter.Callback = app_kp_cas_filter_notify;//过滤器数据回调处理
	filter.timeOutCallback = app_kp_cas_filter_timeout_notify;//超时回调
	app_porting_ca_demux_start_filter(&filter);//设置CA过滤器
    filter_list_add(FilterID,PID,TableID,QueueID);

    KP_DUMP("[mask] ->", mask, filter.filterLen);
    KP_DUMP("[match] ->", match, filter.filterLen);
    CAS_Dbg("####KPAPPCA_StartFilter,FilterID:%d,PID:%d,TableID:%#x,QueueID:%d\n",FilterID,PID,TableID,QueueID);

	return KP_RET_SUCCESS;
}
U16  KPAPPCA_StopFilter(U8 FilterID)
{
    U16 PID;
    U16 TableID;
    U32 queueid;
    KP_Dbg("####KPAPPCA_StopFilter,FilterID:%d\n",FilterID);
    if(filter_list_get(FilterID,&PID,&TableID)>0)
    {
        filter_list_search(PID,TableID,&queueid);
        KP_Dbg("####KPAPPCA_StopFilter,FilterID:%d,PID:%d,TableID:%#x,queueid:%d\n",FilterID,PID,TableID,queueid);

        if(KPCA_ReleasePrivateDataFilter( TableID, PID)>=0)
        {
            filter_list_del(PID,TableID);
        }
    }
    KP_Dbg("####KPAPPCA_StopFilter ok.\n");
    return KP_RET_SUCCESS;
}
//@操作Flash
U16  KPAPPCA_ReadFlash(U16 offsetAddr, U8 *data, U16 len)
{
    return flash_readFrombuf(offsetAddr,data,len);
}

U16  KPAPPCA_WriteFlash(U16 offsetAddr, U8 *data, U16 len)
{
    KP_Dbg("KPAPPCA_WriteFlash .\n");
    return flash_write2buf(offsetAddr,data,len);
}

//@操作解扰器
U16  KPAPPCA_GetDescrambleNum(void)
{
    return 2;
}

//@输出CW 给STB
U8   KPAPPCA_SetCW(U16 ecmpid, U8 *oddKey, U8 *evenKey, U8 keyLen)
{
    KP_Dbg("ecmpid:%d",ecmpid);
    KP_DUMP("[oddKey] ->", oddKey, keyLen);
    KP_DUMP("[evenKey] ->", evenKey, keyLen);
    app_porting_ca_descramble_set_cw(ecmpid,oddKey,evenKey,keyLen,0);
	return KP_RET_SUCCESS;
}
//@输出格式化文本
/*
邮件和OSD 由FormatText 通知给应用，暂描述如下：
邮件的格式：
type + ‘T’ + len + 标题内容+ ‘C’ + len + 内容；
type, len 分别占1byte，其中type = 0
T 是邮件标题标识，标题长度不超过32bytes
C 是邮件内容标识，内容长度不超过255bytes

OSD 的格式
type + ‘P’ + value + (‘D’ + value)/(‘N’ + value) + ‘C’ + len + 内容；
type, value, len 分别占1byte，其中type =1，D 和N 同时存在一个
P 是OSD 显示位置标识，1，2，3，4 分别对应屏幕的上下左右
D 是OSD 显示时间标识，以分钟为单位
N 是OSD 滚动次数标识
C 是OSD 显示内容标识，内容长度不超过255bytes
*/

U8   KPAPPCA_FormatText(U8 *text, U16 len)
{
    U8  type;
    U16 idx  = 0;
    U8  title[KPCAS_MAIL_TITLE_LEN+1] = {0}, content[KPCAS_MAIL_CONTENT_LEN] = {0};
    U8  cTime[20];
    U8  position = 0, time_min = 0, number = 0;
    int ret;
    if(!text || !len) 
    {
        return KP_RET_PARAMETER_ERROR;
    }
    
    type = text[idx ++];
    switch(type)
    {
    case 0x0://Mail
        CAS_Dbg("##################Email################\n");
        if(Email_Analyze((char *)text,len,(char *)title,NULL,(char *)content,(char *)cTime)<0)
        {
            CAS_Dbg("Email_Analyze Error\n");
            return KP_RET_PARAMETER_ERROR;
        }
        
        CAS_Dbg("[KPAPPCA_FormatText]Mail, title->%s, content->%s ,Time->%s\n", title, content ,cTime);
        ret = EmailList_Update((char *)title,(char *)content,NULL,(char *)cTime,0);
        if(ret > 0)
        {
            CA_toEmailNotify(CAS_NEW_EAMIL);
        }
        else if(ret == 0)//邮件保存失败
        {
            CA_toEmailNotify(CAS_NEW_EMAIL_NO_ROOM);
        }
        
        break;
    case 0x1://OSD  
        CAS_Dbg("##################OSD################\n");

        if(Osd_Analyze((char *)text,len,&position,&time_min,&number,(char *)content)<0)
        {
            CAS_Dbg("Osd_Analyze Error\n");
            return KP_RET_PARAMETER_ERROR;

        }
        if(time_min!=0 ||number != 0)
        {
            KP_Dbg("[KPAPPCA_FormatText]OSD, position:%d, time_min:%d, times:%d, content->%s\n", position, time_min, number, content);
            kp_cas_sent_osdmsg(position,time_min,number,(char *)content);
        }
        break;
    default: 
        CAS_Dbg("[KPAPPCA_FormatText] unknown type %d\n", type);
        break;
    } 
    return KP_RET_SUCCESS;
}

//@Other api
U8   KPAPPCA_ShowFinger(U16 ecmpid, U8 *msg)
{
    char buf[64];
    int len = sizeof(buf)-1;
    KP_Dbg("KPAPPCA_ShowFinger Info***\n");
    if(msg)
    {
        memset(buf,0,sizeof(buf));
        len = (strlen((char *)msg) < len)?strlen((char *)msg):len;
        strncpy(buf,(char *)msg,len);
        KP_Dbg("%s\n",buf);
        KP_Dbg("*************************\n");
        KPCA_toShowFinger(TRUE,buf);
    }
    else
    {
        KPCA_toShowFinger(FALSE,NULL);
    }
    
    return KP_RET_SUCCESS;
}

//@打印信息接口
void KPAPPCA_PrintMsg(const char *fmt, ...)
{
    char buf[1024]={0};
    va_list args;
	int i;

	va_start(args,fmt);
	i = Vsnprintf(buf,sizeof(buf), fmt, args);
	va_end(args);

	printf("[KPAPPCA]:%s", buf);
}




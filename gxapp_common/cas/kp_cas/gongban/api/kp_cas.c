#include <common/list.h>
#include <gxos/gxcore_os_core.h>
#include "app_common_flash.h"
#include "app_common_porting_ca_os.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_stb_api.h"
#include "KPAPP.h"
#include "kp_cas.h"


typedef struct st_filter_node_tag
{
    U32 filterid;
	U16 pid;
	U16 tableid;
    U32 queueid;
    struct   gxlist_head ltNode;
}FILTER_NODE_T;

typedef struct st_filter_list_tag
{
    handle_t    mutex;
    struct      gxlist_head Head;
}FILTER_LIST_T;

typedef struct tag_flash_data
{
    handle_t    mutex;
    uint32_t    uCurVer;
    uint32_t    uVer;
    uint32_t    uSize;
    void *buf;
}FLASH_DATA,*pFLASH_DATA;


static FILTER_LIST_T g_filter_list;//Á´±í½á¹¹
static FLASH_DATA g_flash;

void filter_list_init()
{
     memset(&g_filter_list,0,sizeof(FILTER_LIST_T));
     GxCore_MutexCreate(&g_filter_list.mutex);
     GX_INIT_LIST_HEAD(&g_filter_list.Head);
}

int filter_list_search(U16 pid,U16 tableid,U32 *pqueueid)
{
    FILTER_NODE_T *pNode = NULL;
    struct      gxlist_head *pHead = NULL;
    struct      gxlist_head *pCur = NULL;
    
    GxCore_MutexLock (g_filter_list.mutex);
    pHead = &(g_filter_list.Head);
    gxlist_for_each(pCur,pHead)
    {
        pNode = gxlist_entry(pCur,FILTER_NODE_T,ltNode);

        if(pNode->pid == pid && pNode->tableid == tableid)
        {
            if(pqueueid)
            {
                *pqueueid = pNode->queueid;
            }
            GxCore_MutexUnlock (g_filter_list.mutex);
            return 1;
        }
    }
    GxCore_MutexUnlock (g_filter_list.mutex);
    return -1;
}

int filter_list_get(U32 filterid,U16 *ppid,U16 *ptableid)
{
    FILTER_NODE_T *pNode = NULL;
    struct      gxlist_head *pHead = NULL;
    struct      gxlist_head *pCur = NULL;
    
    GxCore_MutexLock (g_filter_list.mutex);
    pHead = &(g_filter_list.Head);
    gxlist_for_each(pCur,pHead)
    {
        pNode = gxlist_entry(pCur,FILTER_NODE_T,ltNode);

        if(pNode->filterid == filterid)
        {
            *ppid = pNode->pid;
            *ptableid = pNode->tableid;
            GxCore_MutexUnlock (g_filter_list.mutex);
            return 1;
        }
    }
    GxCore_MutexUnlock (g_filter_list.mutex);
    return -1;
}

int filter_list_add(U32 filterid,U16 pid,U16 tableid,U32 queueid)
{
    FILTER_NODE_T *pNode = NULL;
    U32 queueid_tmp;
    if(filter_list_search(pid,tableid,&queueid_tmp)>0)
    {
        CAS_Dbg("filter_list_add failed,queueid:%d,%d !\n",queueid_tmp,queueid);
        return -1;
    }
    if((pNode = (FILTER_NODE_T *)GxCore_Malloc(sizeof(FILTER_NODE_T)))==NULL)
    {
        return -1;
    }
    memset(pNode,0,sizeof(FILTER_NODE_T));
    GX_INIT_LIST_HEAD(&pNode->ltNode);
    pNode->filterid = filterid;
    pNode->pid = pid;
    pNode->tableid = tableid;
    pNode->queueid = queueid;
    
    GxCore_MutexLock (g_filter_list.mutex);
    gxlist_add(&(pNode->ltNode),&(g_filter_list.Head));
    GxCore_MutexUnlock (g_filter_list.mutex);
    return 0;
}

void filter_list_del(U16 pid,U16 tableid)
{
    FILTER_NODE_T *pNode = NULL;
    struct      gxlist_head *pHead = NULL;
    struct      gxlist_head *pCur = NULL;
    struct      gxlist_head *pTemp = NULL;

    GxCore_MutexLock (g_filter_list.mutex);
    pHead = &(g_filter_list.Head);
    gxlist_for_each_safe(pCur,pTemp,pHead)
    {
        pNode = gxlist_entry(pCur,FILTER_NODE_T,ltNode);
        if(pNode->pid == pid && pNode->tableid == tableid)
        {
            gxlist_del_init(&pNode->ltNode);
            GxCore_Free(pNode);
            break;
        }
    }
    GxCore_MutexUnlock (g_filter_list.mutex);
}


void flash_init()
{
     uint32_t len;
     memset(&g_flash,0,sizeof(FLASH_DATA));
     GxCore_MutexCreate(&g_flash.mutex);

     g_flash.uVer = 0;
     g_flash.uCurVer = 0;
     g_flash.uSize = KPCA_FLASH_SIZE;
     if(NULL == (g_flash.buf = GxCore_Malloc(g_flash.uSize)))
     {
         CAS_Dbg("flash_init GxCore_Malloc failed!\n");

     }
     memset(g_flash.buf,0,g_flash.uSize);
     len = g_flash.uSize;

    //for test
     //app_porting_ca_flash_erase(0,len);
     
#ifndef KP_DEBUG
     app_porting_ca_flash_read_data(0,g_flash.buf,&len);
#endif

     CAS_Dbg("flash_init OK!\n");
}

void flash_write2flash()
{
    GxCore_MutexLock (g_flash.mutex);
    if(g_flash.uVer != g_flash.uCurVer)
    {
        app_porting_ca_flash_write_data(0,g_flash.buf,g_flash.uSize);
        g_flash.uVer = g_flash.uCurVer;
        CAS_Dbg("flash_write2flash .....\n");	
    }
    GxCore_MutexUnlock (g_flash.mutex);
}

U16 flash_write2buf(U16 offsetAddr, U8 *data, U16 len)
{
    if (NULL == data || 0 == len)
	{
		CAS_Dbg("flash_write2buf failed.\n");		       	
		return KP_RET_FAIL;
	}
    if (offsetAddr + len > g_flash.uSize)
	{
	  	CAS_Dbg("flash_write2buf failed,Offset=0x%x DataLen=0x%x\n",offsetAddr,len);		       	
		return KP_RET_FAIL;
	}
	GxCore_MutexLock (g_flash.mutex);
    memcpy(g_flash.buf+offsetAddr,data,len);
    g_flash.uCurVer ++;
    GxCore_MutexUnlock (g_flash.mutex);
	return KP_RET_SUCCESS;
}

U16  flash_readFrombuf(U16 offsetAddr, U8 *data, U16 len)
{
    if (NULL == data || 0==len)
    {
    	CAS_Dbg("flash_readFrombuf failed.\n");		       	
        return KP_RET_FAIL;
    }
	if (offsetAddr + len > g_flash.uSize)
	{
	  	CAS_Dbg("flash_readFrombuf failed,Offset=0x%x DataLen=0x%x\n",offsetAddr,len);		       	
		return KP_RET_FAIL;
	}
    GxCore_MutexLock (g_flash.mutex);
    memcpy(data,g_flash.buf+offsetAddr,len);
    GxCore_MutexUnlock (g_flash.mutex);
	return KP_RET_SUCCESS; 
}


/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_tr_cas_api_announce.c
* Author    : 	wangjian
* Project   :	
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.12.28		  wangjian 	     creation
2.0  2015.03.26               wangjian          add announce save into flash.
*****************************************************************************/
#include "app_common_porting_stb_api.h"
#include "app_tr_cas_api_announce.h"
#include "app_tr_cas_api_email.h"
#include "app_common_porting_ca_flash.h"


#define ANNOUNCE_FLASH_OFFSET   	(EMAIL_FLASH_OFFSET+EMAIL_DATA_SIZE)

/*Extern data--------------------------------------------------------*/
extern uint8_t win_tr_cas_announce_show(void *data, int size);
extern uint8_t win_tr_cas_announce_get_show_status(void);
extern char* GUI_GetFocusWindow(void);

/*Local data--------------------------------------------------------*/
static TR_CAS_ANNOUNCE_LIST_t  s_casAnnounceList = {0};


/*API--------------------------------------------------------*/
static int announce_time_compare(const void *a ,const void *b)
{
    TR_CAS_ANNOUNCE_INFO_t *pre = (TR_CAS_ANNOUNCE_INFO_t *)a;
    TR_CAS_ANNOUNCE_INFO_t *nxt = (TR_CAS_ANNOUNCE_INFO_t *)b ;
	int ret = 0;
	
	ret = memcmp(&pre->create_time, &nxt->create_time, sizeof(CAS_TIMESTAMP));
	if (ret > 0)
	{
		return (-1);
	}
	else if (0 == ret)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

static uint8_t announce_list_get_index(void)
{
	uint8_t i = 0;
	
	for (i = 0; i < ANNOUNCE_NUM_MAX; i++)
	{
		if (ANNOUNCE_FREE == s_casAnnounceList.announce_list[i].status)
		{
			break;
		}
	}
	
	/*Lastest order first.*/
	if (i >= ANNOUNCE_NUM_MAX)
	{
		i = ANNOUNCE_NUM_MAX-1;
	}

	return i;
}

/*wangjian add on 20150326-----------------------------------------*/
static uint8_t announce_list_load_data(void)
{
	uint8_t result = 0;
    uint8_t *ptemp = NULL;
    TR_CAS_ANNOUNCE_LIST_t *pTmpList = NULL;
    uint32_t len = ANNOUNCE_DATA_SIZE-4;
	
    if (NULL == (ptemp = (uint8_t *)GxCore_Mallocz(len)))
    {
        printf("[announce_list_load_data]GxCore_Mallocz failed!!!\n");
        return 1;
    }
	
    if (!app_porting_ca_flash_read_data(ANNOUNCE_FLASH_OFFSET, ptemp, &len))
    {
        printf("[announce_list_load_data]app_porting_ca_flash_read_data failed!!!\n");
        result = 2;
        goto Exit;
    }
    
    pTmpList = (TR_CAS_ANNOUNCE_LIST_t *)ptemp;
    if ((ANNOUNCE_MAGIC != pTmpList->head_magic) 
		|| (ANNOUNCE_MAGIC != pTmpList->tail_magic))
    {
        result = 3;
        goto Exit;
    }

	if ((pTmpList->total > ANNOUNCE_NUM_MAX) 
		|| (pTmpList->new_count > ANNOUNCE_NUM_MAX))
	{
		printf("[announce_list_load_data]list count is invalid!!!\n");
		result = 4;
        goto Exit;
	}
	
    GxCore_MutexLock(s_casAnnounceList.mutex);
	s_casAnnounceList.total = pTmpList->total;
	s_casAnnounceList.new_count = pTmpList->new_count;
    memcpy(s_casAnnounceList.announce_list, pTmpList->announce_list,\
		   		sizeof(pTmpList->announce_list));
	GxCore_MutexUnlock(s_casAnnounceList.mutex);
	result = 0;
	printf("[announce_list_load_data]total(%d); new(%d).\n",\
		    s_casAnnounceList.total, s_casAnnounceList.new_count);
	
Exit:
    if (ptemp)
    {
    	GxCore_Free(ptemp);
		ptemp = NULL;
	}
	
    return result;
}

static uint8_t announce_list_save_data(void)
{
	uint8_t result = 0;
    uint8_t *pTemp = NULL;
    uint32_t len = 0;

    if (NULL == (pTemp = (uint8_t *)GxCore_Mallocz(ANNOUNCE_DATA_SIZE)))
    {
        printf("[announce_list_save_data]GxCore_Mallocz failed!!!\n");
        return 1;
    }

	printf("###[announce_list_save_data]---###\n");
    memset(pTemp, 0, ANNOUNCE_DATA_SIZE);
    
    GxCore_MutexLock(s_casAnnounceList.mutex);
    s_casAnnounceList.head_magic = (uint16_t)ANNOUNCE_MAGIC;
	s_casAnnounceList.tail_magic = (uint16_t)ANNOUNCE_MAGIC;
	len = sizeof(s_casAnnounceList);
	memcpy(pTemp, &s_casAnnounceList, len);
    GxCore_MutexUnlock(s_casAnnounceList.mutex);
    app_porting_ca_flash_write_data(ANNOUNCE_FLASH_OFFSET, pTemp, len);

    if (pTemp)
    {
    	GxCore_Free(pTemp);
		pTemp = NULL;
	}
    
    return result;    
}

static uint8_t announce_list_get_data_by_row(uint8_t row_idx, uint8_t read_flag, 
	    			  							   TR_CAS_ANNOUNCE_INFO_t *pAnnounce_data)
{	
	uint8_t bSaveData = FALSE;
	uint8_t curStatus = 0;
	
	if (row_idx >= ANNOUNCE_NUM_MAX)
	{
		printf("[announce_list_get_data_by_row]invaid row_idx!!!\n");
		return 1;
	}

	if (NULL == pAnnounce_data)
	{
		printf("[announce_list_get_data_by_row]pAnnounce_data is NULL!!!\n");
		return 2;
	}

	GxCore_MutexLock(s_casAnnounceList.mutex);
	if (1 == read_flag)
	{
		curStatus = s_casAnnounceList.announce_list[row_idx].status;
		if ((ANNOUNCE_UNREAD == curStatus) || (ANNOUNCE_HIDE == curStatus))
		{
			bSaveData = TRUE;
			s_casAnnounceList.announce_list[row_idx].status = ANNOUNCE_READED;	

			if (s_casAnnounceList.new_count > 0)
			{
				s_casAnnounceList.new_count--;
			}
		}
	}
	
	memcpy(pAnnounce_data, &s_casAnnounceList.announce_list[row_idx],\
		 		sizeof(TR_CAS_ANNOUNCE_INFO_t));
	GxCore_MutexUnlock(s_casAnnounceList.mutex);

	if (bSaveData)
	{
		announce_list_save_data();	
	}

	return 0;
}
/*wangjian add end.*/

int32_t app_tr_cas_gxmsg_ca_on_event_announce(GxMsgProperty0_OnEvent* event)
{
	TR_CAS_ANNOUNCE_INFO_t *pAnnounceMsg = NULL;
	uint8_t validIdx = 0;
	TR_CAS_ANNOUNCE_INFO_t *pTmpData = NULL;
	uint8_t tmpLen = 0;
	
	if (NULL == event)
	{
		printf("[app_tr_cas_gxmsg_ca_on_event_announce]param event is NULL!!!\n");
		return 1;
	}

	pAnnounceMsg = (TR_CAS_ANNOUNCE_INFO_t *)(event->buf);
	if (NULL == pAnnounceMsg)
	{
		printf("[app_tr_cas_gxmsg_ca_on_event_announce]pRollingMsg is NULL!!!\n");
		return 2;
	}

	GxCore_MutexLock(s_casAnnounceList.mutex);
	validIdx = announce_list_get_index();
	memcpy(&s_casAnnounceList.announce_list[validIdx], pAnnounceMsg, sizeof(TR_CAS_ANNOUNCE_INFO_t));
	s_casAnnounceList.total++;
	s_casAnnounceList.new_count++;
	s_casAnnounceList.total = TRCAS_MIN(s_casAnnounceList.total, ANNOUNCE_NUM_MAX);
	s_casAnnounceList.new_count = TRCAS_MIN(s_casAnnounceList.new_count, ANNOUNCE_NUM_MAX);
				
	/*list sort, according to create time.*/
	pTmpData = s_casAnnounceList.announce_list;
	tmpLen = ANNOUNCE_NUM_MAX;
	qsort((void *)pTmpData, tmpLen, sizeof(TR_CAS_ANNOUNCE_INFO_t), announce_time_compare);	
	GxCore_MutexUnlock(s_casAnnounceList.mutex);

	/*save data into flash.*/
	announce_list_save_data();
	
	return 0;
}

uint8_t app_tr_cas_announce_exec(void)
{
	uint8_t i = 0;
	char* curFocusWin = NULL;
	int8_t idxNew = -1;
	TR_CAS_ANNOUNCE_INFO_t tmpAnnounceData = {0};

	/*wangjian modify on 20150327 for update announce display string.*/
	curFocusWin = (char *)GUI_GetFocusWindow();
	if ((NULL != curFocusWin) 
		&& (strcasecmp(curFocusWin, "win_full_screen") != 0) 
		&& (strcasecmp(curFocusWin, "win_tr_cas_announce") != 0))
	{
		return 1;
	}
	
    GxCore_MutexLock(s_casAnnounceList.mutex);

	for (i = 0; i < ANNOUNCE_NUM_MAX; i++)
	{
		if (ANNOUNCE_UNREAD == s_casAnnounceList.announce_list[i].status)
		{
			if (-1 == idxNew)
			{
				idxNew = i;
			}
			
			s_casAnnounceList.announce_list[i].status = ANNOUNCE_HIDE;
		}
	}
	
	if (idxNew >= 0)
	{
		memcpy(&tmpAnnounceData, &s_casAnnounceList.announce_list[idxNew],\
				sizeof(tmpAnnounceData));
		s_casAnnounceList.announce_list[idxNew].status = ANNOUNCE_READED;

		if (s_casAnnounceList.new_count > 0)
		{
			s_casAnnounceList.new_count--;
		}
	}
	
    GxCore_MutexUnlock(s_casAnnounceList.mutex);

	if (idxNew >= 0)
	{
		announce_list_save_data();
		win_tr_cas_announce_show((void *)&tmpAnnounceData, sizeof(tmpAnnounceData));
	}
    
	return 0;
}

uint8_t app_tr_cas_announce_is_new(uint32_t msg_crc32)
{
	uint8_t isNew = 0;
	uint8_t i = 0;

	GxCore_MutexLock(s_casAnnounceList.mutex);
	
	for (i = 0; i < ANNOUNCE_NUM_MAX; i++)
	{
		if (ANNOUNCE_FREE == s_casAnnounceList.announce_list[i].status)
		{
			continue;
		}

		if (msg_crc32 == s_casAnnounceList.announce_list[i].data_crc32)
		{
			isNew = 0;
			break;
		}
	}

	if (i >= ANNOUNCE_NUM_MAX)
	{
		isNew = 1;
	}

	GxCore_MutexUnlock(s_casAnnounceList.mutex);

	return isNew;
}

/*wangjian add on 20150326-----------------------------------------*/
int8_t app_tr_cas_announce_init(void)
{
	static uint8_t initFlag = 0;
	uint32_t listSize = 0;
	
	if (1 == initFlag)
	{
		return 0;
	}

	//printf("app_tr_cas_announce_init.\n");
	listSize = sizeof(s_casAnnounceList);
	memset(&s_casAnnounceList, 0, listSize);
	
	if (listSize > ANNOUNCE_DATA_SIZE)
	{
		printf("[app_tr_cas_announce_init]!!!List size(%d) exceed to MAX(%d)!!!\n",\
			  		listSize, ANNOUNCE_DATA_SIZE);
	}
	
	if (GXCORE_SUCCESS != GxCore_MutexCreate(&s_casAnnounceList.mutex))
	{
		printf("[app_tr_cas_announce_init]list mutex create failed!!!\n");
	}
	
	initFlag = 1;
	announce_list_load_data();
	
	return 1;
}

uint8_t app_tr_cas_api_get_announce_count(ca_get_count_t *data)
{
    GxCore_MutexLock(s_casAnnounceList.mutex);
	data->totalnum = s_casAnnounceList.total;
	data->newnum = s_casAnnounceList.new_count;
    GxCore_MutexUnlock(s_casAnnounceList.mutex);
	//printf("[Announce]data->totalnum(%d); newnum(%d).\n", data->totalnum, data->newnum);
	
    return data->totalnum;
}

char* app_tr_cas_api_get_announce_data(ca_get_date_t *data)
{
    uint32_t pos = 0;                             
	uint32_t ID = 0; 
	uint8_t result = 0;
    TR_CAS_ANNOUNCE_INFO_t pTmpData = {0};
    static char cTextBuffer[2][15] = {{0}};
	static char strSendTime[30] = {0};
	uint8_t *tmpTitle = NULL;
	uint8_t *tmpContent = NULL;

	if (NULL == data)
	{
		return NULL;
	}

	pos = data->pos;
	ID = data->ID;
	
	switch (ID)
	{
		case TR_ANNOUNCE_FLAG_ID:
			memset(cTextBuffer[0], 0, 15);
            result = announce_list_get_data_by_row(pos, 0, &pTmpData);
            if (0 == result)
            {
                if ((ANNOUNCE_UNREAD == pTmpData.status) 
					|| (ANNOUNCE_HIDE == pTmpData.status))
				{
					strcpy(cTextBuffer[0], "New");
				}
			    else if (ANNOUNCE_READED == pTmpData.status)
				{
					strcpy(cTextBuffer[0], "Readed");
				}
            }
			
			return cTextBuffer[0];
			
		case TR_ANNOUNCE_LEVEL_ID:
			memset(cTextBuffer[1], 0, 15);
            result = announce_list_get_data_by_row(pos, 0, &pTmpData);
            if (0 == result)
            {
				if (CAS_MSG_PRIORITY_LOW == pTmpData.level)/*wj modify on 20150328*/
				{
					strcpy(cTextBuffer[1], "Priority_LOW");
				}
			    else if (CAS_MSG_PRIORITY_MED == pTmpData.level)
				{
					strcpy(cTextBuffer[1], "Priority_MED");
				}
				else if (CAS_MSG_PRIORITY_HIGH == pTmpData.level)
				{
					strcpy(cTextBuffer[1], "Priority_HIGH");
				}
				else if (CAS_MSG_PRIORITY_URGENT == pTmpData.level)
				{
					strcpy(cTextBuffer[1], "Priority_URGENT");
				}
				else
				{
					strcpy(cTextBuffer[1], "Priority_Unknown");
				}
            }
			
			return cTextBuffer[1];
			
		case TR_ANNOUNCE_TITLE_ID:
			tmpTitle = app_tr_cas_api_get_email_title_data();
			memset(tmpTitle, 0, ANNOUNCE_TITLE_LEN);
            result = announce_list_get_data_by_row(pos, 0, &pTmpData);
            if (0 == result)
            {
            	strcpy((char *)tmpTitle, (char *)pTmpData.title);
                return (char *)tmpTitle;
            }
            else
            {
                return NULL;
            }
			
		case TR_ANNOUNCE_SENDTIME_ID:
            result = announce_list_get_data_by_row(pos, 0, &pTmpData);
            if (0 == result)
            {
            	sprintf(strSendTime, "%04d.%02d.%02d %02d:%02d:%02d",\
					  	(pTmpData.create_time.bYear[0]*100 + pTmpData.create_time.bYear[1]),
						pTmpData.create_time.bMonth, pTmpData.create_time.bDay,\
						pTmpData.create_time.bHour, pTmpData.create_time.bMinute,\
						pTmpData.create_time.bSecond);
				
                return strSendTime;
            }
            else
            {
                return NULL;
            }
			
		case TR_ANNOUNCE_CONTENT_ID:
			tmpContent = app_tr_cas_api_get_email_content_data();
			memset(tmpContent, 0, ANNOUNCE_CONTENT_LEN);
            result = announce_list_get_data_by_row(pos, 1, &pTmpData);
            if (0 == result)
            {
            	strcpy((char *)tmpContent, (char *)pTmpData.content);
                return (char *)tmpContent;
            }
            else
            {
                return NULL;
            }
			
		default:
			break;
	}
	
	return NULL;
}

bool app_tr_cas_api_delete_announce(ca_delete_data_t *data)
{
	uint32_t tmpPos = 0; 
	uint8_t i = 0;
	TR_CAS_ANNOUNCE_INFO_t tmpDelAnnounce = {0};
	uint8_t curStatus = 0;
	
	if (NULL == data)
	{
		return FALSE;
	}
	
	tmpPos = data->pos;
	if (tmpPos >= ANNOUNCE_NUM_MAX)
	{
		printf("[app_tr_cas_api_delete_announce]invaid tmpPos!!!\n");
		return FALSE;
	}

	GxCore_MutexLock(s_casAnnounceList.mutex);
	i = tmpPos;
	memcpy(&tmpDelAnnounce, &s_casAnnounceList.announce_list[tmpPos], sizeof(tmpDelAnnounce));

	curStatus = s_casAnnounceList.announce_list[i].status;
	if ((ANNOUNCE_UNREAD == curStatus) || (ANNOUNCE_HIDE == curStatus))
	{
		s_casAnnounceList.new_count--;
	}
	
	for (i = tmpPos; i < s_casAnnounceList.total-1; i++)
	{
		memcpy(&s_casAnnounceList.announce_list[i],\
				&s_casAnnounceList.announce_list[i+1],\
				sizeof(TR_CAS_ANNOUNCE_INFO_t));
	}
	
	memset(&s_casAnnounceList.announce_list[i], 0, sizeof(TR_CAS_ANNOUNCE_INFO_t));
	s_casAnnounceList.announce_list[i].data_crc32 = 0x00;
	s_casAnnounceList.announce_list[i].status = ANNOUNCE_FREE;
	s_casAnnounceList.total--;
	GxCore_MutexUnlock(s_casAnnounceList.mutex);
	
	announce_list_save_data();

	return TRUE;
}

bool app_tr_cas_api_delete_all_announce(ca_delete_data_t *data)
{
	uint8_t i = 0;
	
	if (NULL == data)
	{
		return FALSE;
	}
	
	GxCore_MutexLock(s_casAnnounceList.mutex);
	s_casAnnounceList.head_magic = 0;
	s_casAnnounceList.total = 0;
	s_casAnnounceList.new_count = 0;
	s_casAnnounceList.tail_magic = 0;

	for (i = 0; i < ANNOUNCE_NUM_MAX; i++)
	{
		memset(&s_casAnnounceList.announce_list[i], 0, sizeof(TR_CAS_ANNOUNCE_INFO_t));
		s_casAnnounceList.announce_list[i].status = ANNOUNCE_FREE;
		s_casAnnounceList.announce_list[i].data_crc32 = 0x00;	
	}
	
	GxCore_MutexUnlock(s_casAnnounceList.mutex);

	announce_list_save_data();
	
	return TRUE;
}

/*wangjian add end.------------------------------------------------*/


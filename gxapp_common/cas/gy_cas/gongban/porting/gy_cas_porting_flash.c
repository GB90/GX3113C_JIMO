/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	yg_cas_porting_flash.c
* Author    : 	bingyu
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	ģ��ͷ�ļ�
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2012.12.02		  bingyu 	 			creation
*****************************************************************************/
#include <gxtype.h>
#include "gxmsg.h"
#include "gxapp_sys_config.h"
#include "app_common_play.h"
#include "app_common_table_pmt.h"
#include "app_common_porting_stb_api.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_porting_ca_smart.h"
	
#include "gyca_def.h"
#include "gyca_pub.h"
#include "gyca_osp.h"
#include "gyca_interface.h"
#include "gyca_porting.h"

/*
** �洢�ռ�������ӿڶ���*/
/***********************************************************************************************
** 	��������:GYCA_GetBuffer
************************************************************************************************
**	�� �� �ߣ�  	�����С�
**	ʹ �� �ߣ�	CAģ�顣
**	����������	��û������ṩ��CAģ�鱣����Ϣ����ʼ��ַ�ͳߴ硣
**	�����������
**	���������	lStartAddr   ����ռ����ʼ��ַ
**                                lSize          ����ռ�Ĵ�С
**	�� �� ֵ��	�ɹ�GYSUCCESS  ʧ�� GYFAILURE
**	����˵����
**                               �����г���Ӧ���䲻����64K��EEPROM�ռ�
*************************************************************************************************/
GYS32 GYCA_GetBuffer (GYS32 *lStartAddr, GYS32* lSize)
{
    *lStartAddr = FLASH_GY_CAS_ADDR;
	*lSize = FLASH_GY_CAS_SIZE;

	return GYSUCCESS;
}

/*************************************************************************************************
** 	��������:GYCA_ReadBuffer
**************************************************************************************************
**	�� �� �ߣ�  	�����С�
**	ʹ �� �ߣ�	CAģ�顣
**	������������ָ���ĵ�ַ����ָ�������ֽڷ���ָ���Ŀռ���
**	���������	pStartAddr��		Ҫ��ȡ���ݵĴ洢�ռ�ĵ�ַ��
**	 				pbData��			����ȡ���ݵĴ�ŵ�ַָ�롣
**	 				pBytes��		       Ҫ�������ݵĳ���
**	���������	pbData��			�����������ݡ�
**	�� �� ֵ��	GYSUCCESS �ɹ��� GYFAILURE ʧ�ܡ�
**	����˵����
**************************************************************************************************/
GYS32 GYCA_ReadBuffer(const GYS32 lStartAddr, GYU8* pData, GYS32* nLen)
{
	app_porting_ca_flash_read_data(lStartAddr-FLASH_GY_CAS_ADDR,pData,(GYU32 *)nLen);
	
	return GYSUCCESS;
}

/*************************************************************************************************
** 	��������:GYCA_WriteBuffer
**************************************************************************************************
**	�� �� �ߣ�	�����С�
**	ʹ �� �ߣ�	CAģ�顣
**	����������	������еĴ洢�ռ�д��Ϣ
**	���������	lStartAddr��		Ҫд�Ĵ洢�ռ��Ŀ���ַ��
**					pData��	Ҫд������
** 					nLen��		Ҫд�����ݵĳ���
**	���������	�ޡ�
**	�� �� ֵ��    GYSUCCESS �ɹ��� GYFAILURE�� ʧ�ܡ�
**	����˵����
*************************************************************************************************/
GYS32 GYCA_WriteBuffer(const GYS32 lStartAddr, const GYU8* pData, GYS32 nLen) 
{
	app_porting_ca_flash_write_data(lStartAddr-FLASH_GY_CAS_ADDR,(uint8_t*)pData,nLen);

	return GYSUCCESS;
}




/*******************************************************************************
** Notice: Copyright (c) 2008 Geeya corporation - All Rights Reserved 
********************************************************************************
** Description:
**                 The defination file of CAS library . the datap structure prototype,enum and macro
**                 is included in the file. these are common definations thar used at CAS library and 
**                 STB application.
**
** File Rev     $Revision: \main\20060816_1.0_integration_branch\1 $
**
** Date         $Date:     Wednesday Augest 16 11:54:04 2006 $
** 
** File Name: gyca_def.h
**
********************************************************************************/
#ifndef _GYCAS_PORTING_H_
#define _GYCAS_PORTING_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include "gyca_pub.h"
#include "gyca_def.h"


#define YGCAS_ART_LEN 17
#define GY_CAS_FILTER_NUM 10
#define FLASH_GY_CAS_ADDR 0x20000
#define FLASH_GY_CAS_SIZE 64*1024

#define RUNTIME_ETU                         (372)
#define RUNTIME_BAUD_RATE                   (9600*RUNTIME_ETU)
#define RUNTIME_EGT                         (RUNTIME_ETU * 20)
#define RUNTIME_WDT                         (45 * RUNTIME_ETU)
#define RUNTIME_TGT                         (0)
#define RUNTIME_TWDT                        (3*9600 * RUNTIME_ETU)


int GYCA_DemuxInit(void);
SC_Status_Notify_Fct_t GYCA_GetNotify(void);



#ifdef  __cplusplus
}
#endif

#endif		//_GYCAS_PUBS_H_




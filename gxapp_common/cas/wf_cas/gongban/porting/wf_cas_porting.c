/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_ca_api.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2012.12.02		  zhouhm 	 			creation
*****************************************************************************/
#include "interfac.h"
#include "wanfacas.h"
#include "wftype.h"               
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_os.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_table_nit.h"
#include "app_common_search.h"
#include "app_common_play.h"
#include "app_common_porting_stb_api.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_prog.h"
#include "app_common_lcn.h"

#include "gx_descrambler.h"

#include "app_wf_cas_api_smart.h"
#include "app_wf_cas_api_demux.h"

extern handle_t smart_handle;

#define MAX_FILTER_NUMS		6
typedef struct
{
	WFSTB_FilterData_t tEmmFilterDate[MAX_FILTER_NUMS];
	U08 chFilterCountNum;
}WFSTB_FilterDataUseEmm_t;

WFSTB_FilterDataUseEmm_t tFiltDataInEmm;

static U32 gs_nEnbleTable = 0;

extern uint8_t Show_time_short;		//huangyp		add

#include "app_wf_cas_api_pop.h"
/*
#include "app_by_cas_api_smart.h"
#include "app_by_cas_api_pop.h"
#include "app_by_cas_api_demux.h"
#include "app_by_cas_api_email.h"
#include "app_by_cas_api_lock_service.h"
#include "app_by_cas_api_demux.h"
*/

static wf_cas_pptv_buy_st pptvbuy = {0};
static wf_cas_email_st emailnotify= {0};
static wf_ca_rolling_message_st  rollingmsg = {0};

#if 1
#define ICC_DEFAULT_F          372 // defined by ISO 7816-3
#define ICC_DEFAULT_D          1   // defined by ISO 7816-3
#define ICC_DEFAULT_F_DIV_D    372 // defined by ISO 7816-3
#define ICC_DEFAULT_CWI        13  // defined by ISO 7816-4
#define ICC_DEFAULT_CWT        12  // defined by ISO 7816-4
#define ICC_BLOCK_GUART_TIME   22  // defined by ISO 7816-3
#define ICC_MAX_ATR_BYTES      128 // limit the max. number of ATR bytes
	
#define FAPI_ICC_COMMUNICATION_TIMEOUT   400
	
#define FAPI_ICC_MAX_ATR_BYTES           255     //!< Maximum number of ATR bytes
#define FAPI_ICC_MAX_HIST_BYTES          255     //!< Maximum number of HISTORY bytes
	
#define FAPI_ICC_NO_CARD_INTERFACE          0    // Direct connection to ICC card
#define FAPI_ICC_CARD_INTERFACE             1    // Interface chip used to control ICC card
	
#define FAPI_ICC_RESET_NORMAL               0   //!< Low impdeance
#define FAPI_ICC_RESET_INVERTED             1   //!< High impdeance
	
#define FAPI_ICC_VCC_NORMAL                 0   //!< Normal VCC enable signal
#define FAPI_ICC_VCC_INVERTED               1   //!< Inverted VCC enable (needed for Philips TDA8004T)
	
#define ICC_COLD_RESET            (uint32_t)1
#define ICC_WARM_RESET            (uint32_t)2
#define ICC_SEND_PPS              (uint32_t)3
#define ICC_CARD_DEACTIVATE       (uint32_t)4
	typedef struct
{
	uint32_t protocol;       //!< specifies the smart card protocol. (T=0,T=1,..)
	uint32_t FI;             //!< specifies the clock rate conversion factor
	uint32_t DI;             //!< specifies the bit rate adjustment factor
} FAPI_ICC_PpsParamsT;
typedef struct
{
	uint32_t                id;                // handle ID
	uint32_t                inUse;             // 1 if structure is in use (opened)
	uint32_t                blockIndex;        // index for the FREG_ICC_xxx functions (either 0 or 1 )
	uint32_t     transferCompleted; // The transfer completion semaphore of the device
	uint32_t            idleTimerCallback; // delay/timeout callback
	
		uint32_t                iccCardInterface;  // Indicates if Smart card is controled via interface chip
	
		uint32_t        gpioVpen;          // GPIO Vpen Handle
	uint32_t        gpioVcc;           // GPIO Vcc Handle
	uint32_t                gpioVccInvert;     // Invert the VCC enable output 1 if TDA8004T is avaible.
	uint32_t        gpioRst;           // GPIO Reset Handle
	uint32_t                gpioRstInvert;     // Reset Invertion 0:=LOW (default) 1:=HIGH impedance.
	uint32_t        gpioClk;           // GPIO Clock Handle
	uint32_t                gpioClkDividerExt; // Define external clk divider (default=1)
	uint32_t        gpioIn;            // GPIO In Handle
	uint32_t        gpioDataOut;       // GPIO DataOut Handle
	uint32_t        gpioDataIn;        // GPIO DataIn Handle
	uint32_t        gpioDataBidi;      // GPIO DataBidi Handle
	
		uint32_t                bufferOverApi;     // Detect buffer over flow cleared by reset the smart card
	uint8_t                 atrBuffer[FAPI_ICC_MAX_ATR_BYTES];   // ATR buffer
	uint32_t                atrBytes;          // number of bytes in ATR data buffer
	uint8_t                 atrParity;         // the parity bit send from the ATR
	uint8_t*                readBuffer;        // read data buffer
	uint32_t                readSize;          // read data buffer size in bytes
	uint32_t                readBytes;         // number of bytes in read data buffer
	uint32_t                readPos;           // read position
	uint8_t*                writeBuffer;       // write data buffer
	uint32_t                writeBytes;        // number of bytes in write data buffer
	uint32_t                writePos;          // current write position in data buffer
	uint8_t                 histBuffer[FAPI_ICC_MAX_HIST_BYTES];   // History buffer
	uint32_t                histBytes;         // number of bytes in historical data buffer
	uint32_t                cardClock;         // card clock frequency in kHz
	uint32_t                baudRate;          // current baud rate
	uint32_t                convention;        // current convention mode
	uint32_t                protocol;          // specifies the smart card protocol. (T=0 or T=1)
	uint32_t                parity;            // current parity mode, ODD, EVEN, NONE
	uint32_t                parityErrResend;
		uint32_t                dataBits;          // data bit size 8 or 9 bit
	uint32_t                cardState;         // last card state (in/out)
	uint32_t                nextState;         // next state machine state to handle
	int32_t                 status;            // current processing status, RUNNING, TIMEOUT, etc.
	
		//------- This params will be set from iccCheckATRData function -------//
		uint32_t                FI;                // specifies the clock rate conversion factor index
	uint32_t                DI;                // specifies the bit rate adjustment factor index
	uint32_t                maxCardClock;      // max card frequency in KHz
	uint8_t                 PI1;               // specifies the programming voltage factor
	uint8_t                 PI2;               // specifies the programming voltage factor
	uint8_t                 II;                // specifies the maximum programming current factor
	uint8_t                 N;                 // specifies extra guard time
	uint8_t                 SMB;               // Specific Mode Byte - No PPS possible if present (!=FF)
	uint32_t                XI;                // specifies the clock stop indicator
	uint32_t                UI;                // specifies the class indicator
	uint32_t                F_div_D;           // specifies the result of F/D
	uint32_t                _F;                // specifies the clock rate conversion factor
	uint32_t                _D;                // specifies the bit rate adjustment factor
	uint32_t                IFSC;              // information field size for the card
	uint32_t                CWI;               // character waiting time
	uint32_t                BWI;               // block waiting time
	uint32_t                CWT;               // character waiting time = ( 11 + 2**CWI ) etu
	uint32_t                BWT;               // block waiting time  = ( 11 + 2**BWI ) etu
	uint32_t                CRC;               // CRC (1) or KRC (0)
	
		//------- This params will be used for iccCheckPPSData function
		uint32_t                ppsEnable;         // specifies the usage of PPS
	uint8_t                 ppsData[5];        // specifies the pps read buffer
	FAPI_ICC_PpsParamsT     ppsParams;         // Optional PPS Params settings
	
		uint8_t*                rxBufferPtr;       // Pointer to Receive Buffer
	uint32_t                rxTimeOut;         // Transfer Timeout
	uint32_t                guardTime;         // Guard time
} iccChannelT;

uint32_t  wt           = 9600; // Waiting time in etu
int32_t   bytesToRead  = 0;              // to read TS byte
uint32_t  elapsedWTime = 0;
uint8_t*  atr_ptr      = 0;
uint32_t  ReadTCK      = 0;              // indicates if TCK byte must be read
uint32_t  TDbyte       = 0;              // ATR check byte
uint32_t  TCK          = 0;              // ATR check byte
#endif

#define WFCAS_Dbg(...)                          do {                                            \
                                                    printf("[cas]\t");                        \
                                                    printf(__VA_ARGS__);                        \
                                                } while(0)
static int32_t iccCheckATRData( iccChannelT* channelPtr )
{
	
		uint8_t  *data_ptr  = NULL;
		uint32_t td_data    = 0;
		uint32_t tabc_data  = 0;
		uint32_t p_current  = 0;
		uint32_t p_voltage  = 0;
		uint32_t p2_voltage = 0;
		int32_t  xi_ui_done = 0;
		uint32_t index      = 0;
		uint8_t  T15        = 0;
		
		channelPtr->atrBytes = 0;
		
		channelPtr->FI       = 0;
		channelPtr->DI       = 0;
		channelPtr->maxCardClock = 0;
		channelPtr->PI1      = 0;
		channelPtr->PI2      = 0;
		channelPtr->II       = 0;
		channelPtr->N        = 0;
		channelPtr->SMB      = 0xFF;
		channelPtr->XI       = 0;
		channelPtr->UI       = 0;
		channelPtr->F_div_D  = ICC_DEFAULT_F_DIV_D; // Default value
		channelPtr->IFSC     = 0;
		channelPtr->CWI      = ICC_DEFAULT_CWI;     // Default value
		channelPtr->BWI      = 0;
		channelPtr->CWT      = ICC_DEFAULT_CWT;     // Defualt value
		channelPtr->BWT      = 0;
		channelPtr->CRC      = 0;
		channelPtr->protocol = 0;                   // Default Protocol = T0
	
		// MISRA
		p_current = p_current;
		
		//--- Process TS-Byte                          ---
		data_ptr             = &channelPtr->atrBuffer[0];
		bytesToRead          = (int32_t)channelPtr->atrBytes;
		WFCAS_Dbg("[iccCheckATRData] - TS: 0x%02X\n", *data_ptr );
		bytesToRead--;
		data_ptr++;
		
		
		//--- Process T0-Byte                          ---
		WFCAS_Dbg("[iccCheckATRData] - T0: 0x%02X\n", *data_ptr );
		bytesToRead--;
		TCK     = TCK^(*data_ptr);
		td_data = *data_ptr++;
		
		tabc_data = td_data & 0x00000F;
		if( tabc_data < FAPI_ICC_MAX_HIST_BYTES  )
			channelPtr->histBytes = tabc_data;
		else
			channelPtr->histBytes = FAPI_ICC_MAX_HIST_BYTES;
				
				WFCAS_Dbg("[iccCheckATRData] - History Bytes: %d\n", channelPtr->histBytes );
				
				//--- Process TA1 / TB1 / TC1 / TD1 Byte       ---
				index = 1;
				while( ( td_data & 0x000000F0 ) && ( bytesToRead ) )
				{
					WFCAS_Dbg("[iccCheckATRData] - loop Y(%d): 0x%02X\n", index, td_data );
						
						//--- Process TA(i) ---
						if( td_data & 0x00000010 )
						{
							TCK     = TCK^(*data_ptr);
								tabc_data = *data_ptr++;
								bytesToRead--;
								
								WFCAS_Dbg("[iccCheckATRData]   - TA(%d): 0x%02X\n", index, tabc_data );
								
								//--- Process Y(1) ---
								if(index == 1 )
								{
									channelPtr->FI = (tabc_data>>4) & 0x0000000F;
										channelPtr->DI =  tabc_data     & 0x0000000F;
										//if( (channelPtr->FI<sizeof(ICC_F_Table)) &&
										//    (channelPtr->DI<sizeof(ICC_D_Coefficient_Table)) )
										{
											//channelPtr->_F    = ICC_F_Table[channelPtr->FI].fiValue;
											//channelPtr->_D    = ICC_D_Coefficient_Table[channelPtr->DI];
											//channelPtr->maxCardClock = ICC_F_Table[channelPtr->FI].fMax;
										}
									//else
									{
										// WFCAS_Dbg("[iccCheckATRData] leave (bad TS found)\n" );
										// return( FAPI_ICC_ERR_NO_ANSWER );
									}
									
										WFCAS_Dbg("[iccCheckATRData] - FI=%02X\n", channelPtr->FI );
										WFCAS_Dbg("[iccCheckATRData] - DI=%02X\n", channelPtr->DI );
										WFCAS_Dbg("[iccCheckATRData] - ClockRateConversionFactor=%d\n", channelPtr->_F );
										WFCAS_Dbg("[iccCheckATRData] - BaudRateAdjustmentFactor=%d\n",  channelPtr->_D );
										WFCAS_Dbg("[iccCheckATRData] - Maximum card frequency =%d [KHz]\n",  channelPtr->maxCardClock );
										
										if( (channelPtr->_D > 0 ) && ( channelPtr->_F > 0 ) )
											channelPtr->F_div_D = (uint32_t)(channelPtr->_F/channelPtr->_D);
												
												WFCAS_Dbg("[iccCheckATRData]     - F/D=%d\n", channelPtr->F_div_D );
								}
								else if( index == 2 )
								{
									channelPtr->SMB = (uint8_t)tabc_data;
										WFCAS_Dbg("[iccCheckATRData]     - SMB=%d\n", channelPtr->SMB );
								}
								else if( index == 3 )
								{
									channelPtr->IFSC = tabc_data;
										WFCAS_Dbg("[iccCheckATRData]     - IFSC=%d\n", channelPtr->IFSC );
								}
						}
					
						//--- Process TB(i) ---
						if( td_data & 0x00000020 )
						{
							TCK     = TCK^(*data_ptr);
								tabc_data = *data_ptr++;
								if(bytesToRead)
									bytesToRead--;
										WFCAS_Dbg("[iccCheckATRData]   - TB(%d): 0x%02X\n", index, tabc_data );
										
										if (index==1)  // -> TB is I,P parameters
										{
											channelPtr->II = (uint8_t)(tabc_data >> 5);
												//if(channelPtr->II<sizeof(ICC_I_Table))
												//     p_current = (uint32_t)ICC_I_Table[channelPtr->II];
												p_voltage = tabc_data & 0x0000001F;
												channelPtr->PI1 = (uint8_t)p_voltage;
												WFCAS_Dbg("[iccCheckATRData]     - PCurrent=%d, PVoltage=%d\n", p_current, p_voltage );
										}
										else if( index == 2 )
										{
											p2_voltage = tabc_data;
												channelPtr->PI2 = (uint8_t)p2_voltage;
												WFCAS_Dbg("[iccCheckATRData]     - PVoltage2=%d\n", p2_voltage );
										}
										else if( index == 3 )
										{
											channelPtr->CWI =  tabc_data        & 0x0000000F;
												channelPtr->BWI = (tabc_data >> 4 ) & 0x0000000F;
												
												WFCAS_Dbg("[iccCheckATRData]     - CWI=%d\n", channelPtr->CWI );
												WFCAS_Dbg("[iccCheckATRData]     - BWI=%d\n", channelPtr->BWI );
										}
						}
					//--- Process TC(i) ---
					if( td_data & 0x00000040 )
					{
						TCK     = TCK^(*data_ptr);
							tabc_data = *data_ptr++;
							if(bytesToRead)
								bytesToRead--;
									WFCAS_Dbg("[iccCheckATRData]   - TC(%d): 0x%02X\n", index, tabc_data );
									
									if(index == 1 )
									{
										channelPtr->N = (uint8_t)tabc_data;
											WFCAS_Dbg("[iccCheckATRData]     - ExtraGuardTime=%d\n", tabc_data );
									}
									else if( T15 && (xi_ui_done==0) )
									{
										channelPtr->UI =  tabc_data       & 0x0000003f;
											channelPtr->XI = (tabc_data >> 6) & 0x0000000C;
											xi_ui_done = 1;
									}
									else if( (index==2) && (channelPtr->protocol==0) )
									{
										channelPtr->CWI = tabc_data;         // used as WI, defaults to 13
										channelPtr->BWI = tabc_data;         // used as WWT max = 960 * WI * ( Fi/ f )
										WFCAS_Dbg("[iccCheckATRData]     - WI=%d\n", channelPtr->CWI );
									}
									else if( index == 3 )
									{
										channelPtr->CRC = tabc_data & 0x00000001;
											WFCAS_Dbg("[iccCheckATRData]     - CRC=%d\n", channelPtr->CRC );
									}
					}
					//--- Process TD(i) ---
					if( td_data & 0x00000080 )
					{
						TCK     = TCK^(*data_ptr);
							tabc_data = *data_ptr++;
							if(bytesToRead)
								bytesToRead--;
									WFCAS_Dbg("[iccCheckATRData]   - TD(%d): 0x%02X\n", index, tabc_data );
									
									if((tabc_data & 0x0000000F) == 15)
									{
										T15 = 1;
									}
									else
									{
										channelPtr->protocol = tabc_data & 0x0000000F;
									}
						
							if(channelPtr->protocol == 1)
							{
								ReadTCK = 1; // Read check Byte after History buffer
							}
						
							//--- Process TAi / TBi / TCi TDi Byte         ---
							td_data = tabc_data;
							WFCAS_Dbg("[iccCheckATRData]     - Protocol=%d\n", channelPtr->protocol );
					}
					else
					{
						td_data      = 0;
							bytesToRead  = 0;
					}
					index++;
				}
	
		
		//--- copy historical bytes ---//
		for( index=0; index < channelPtr->histBytes; index++)
		{
			TCK     = TCK^(*data_ptr);
				channelPtr->histBuffer[index] = *data_ptr++;
		}
	
		if( ReadTCK )
		{
			TCK     = TCK^(*data_ptr);
				data_ptr++;
				WFCAS_Dbg("[iccCheckATRData] - TCK=0x%02X %s\n",TCK
						, (TCK == 0)? "<OK>":"<<<< Corrupted ATR Data >>>>");
		}
	
		/*lint -save -e946 */
		channelPtr->atrBytes = (uint32_t)(data_ptr - &channelPtr->atrBuffer[0]);
		/*lint -restore */
		WFCAS_Dbg("[iccCheckATRData] leave - ATR<%d bytes in total>\n", channelPtr->atrBytes );
		
		//--- Check baudrate divider ---//
		if(!channelPtr->F_div_D)
		{
			channelPtr->F_div_D = ICC_DEFAULT_F_DIV_D;
		}
	
		WFCAS_Dbg("[iccCheckATRData] - CWI=%d (used as WI in ETUs)\n", channelPtr->CWI );
		WFCAS_Dbg("[iccCheckATRData] - CWT=%d (used as WI in ETUs)\n", channelPtr->CWT );
		
		WFCAS_Dbg("[iccCheckATRData] - protocol: %d\n", channelPtr->protocol );
		WFCAS_Dbg("[iccCheckATRData] - ASCII: " );
		for( index=0; index < channelPtr->atrBytes; index++)
			WFCAS_Dbg("%c", (channelPtr->atrBuffer[index]>0x20)?channelPtr->atrBuffer[index]:'?' );
				WFCAS_Dbg("\n" );
				
				WFCAS_Dbg("[iccCheckATRData] Added Extra Guard Time\n");
				if( channelPtr->N == 255 )
				{
					WFCAS_Dbg( "[iccCheckATRData]    Set - %d ETU\n", 11);
						channelPtr->guardTime = 11;
						//FREG_ICC_SetGuardtimer_Gtreg(channelPtr->blockIndex, channelPtr->guardTime);
				}
				else
				{
					WFCAS_Dbg( "[iccCheckATRData]    Add - %d ETU\n", channelPtr->N);
						channelPtr->guardTime += channelPtr->N;
						//FREG_ICC_SetGuardtimer_Gtreg(channelPtr->blockIndex, channelPtr->guardTime);
				}
		WFCAS_Dbg("[iccCheckATRData]    New - %d ETU\n", channelPtr->guardTime );
		
#if 0
		for( index=0; index < channelPtr->atrBytes; index++ )
		{
			if( (index%8) == 0 )
			{
				FAPI_SYS_PRINT_DEBUG(2, "[AtrBuffer]");
			}
			FAPI_SYS_PRINT_DEBUG( 2," 0x%02X", channelPtr->atrBuffer[index] );
				if( (((index+1) % 8) == 0) || ((index+1) == channelPtr->atrBytes) )
				{
					FAPI_SYS_PRINT_DEBUG(2, "\n" );
				}
		}
	FAPI_SYS_PRINT_DEBUG( 2,"\n" );
#endif
		
		return( 0 );
}

U32	WFSMART_OpenCard(U08 *History,U08 *HistoryLen)
{
	CAS_Dbg("%s\n",__FUNCTION__);
/*	
	GxSmcTimeParams	  time;
	int8_t smc_ret = 0;
	time.baud_rate  = 3570000;
	time.egt        = 12*372;
	time.etu        = 372;
	time.tgt        = 0;
	time.twdt       = 0x1fffffff;
	time.wdt        = 120*372;
*/
    uint8_t atr[256] = {0};
	uint8_t atrlen = 0;
	uint8_t sc_result = 0;
	//uint8_t Len = 0;
	iccChannelT iccChannelArray;
	
	memset(atr,0,sizeof(atr));
	sc_result = app_wf_cas_api_smart_reset(atr, &atrlen);
	if(sc_result == TRUE)
	{
		memset(&iccChannelArray,0,sizeof(iccChannelT));
		memcpy(iccChannelArray.atrBuffer,atr,atrlen);
		iccChannelArray.atrBytes = atrlen;
		iccCheckATRData(&iccChannelArray);
		memcpy(History,iccChannelArray.histBuffer,iccChannelArray.histBytes);
		CAS_DUMP("[SC] History", iccChannelArray.histBuffer, iccChannelArray.histBytes);
		*HistoryLen = iccChannelArray.histBytes;
	
/*		Len = atr[1]&0x0f;
		memcpy(History, &atr[atrlen -Len], Len);
		*HistoryLen = Len;
		CAS_DUMP("History",History,Len);
		CAS_Dbg("%s	success\n",__FUNCTION__);		*/
	//	smc_ret = GxSmc_Config(smart_handle, &time);
	//	CAS_Dbg("%s	GxSmc_Config		sc_result=%d\n",__FUNCTION__,smc_ret);
		return 0;
	}
	else
	{
		CAS_Dbg("%s	failure\n",__FUNCTION__);
		return 1;
	}
}
U32	WFSMART_Transfer(U08 CmdL,U08 *Cmd,U08 RespL,U08 *Resp)
{
	CAS_Dbg("%s	CmdL=%d	RespL=%d\n",__FUNCTION__,CmdL,RespL);
    uint8_t reply[1024] = {0};
	uint16_t replylen = 0;
	uint8_t CmdRespL = RespL;
    uint8_t sc_result;
    
	//CAS_DUMP("Cmd",Cmd,CmdL);
	memset(reply,0,sizeof(reply));
    sc_result =  app_porting_ca_smart_sc_apdu61(Cmd,CmdL,reply,&replylen);
	if((TRUE == sc_result)&&((replylen-2) == CmdRespL))
	{
    	memcpy(Resp, reply, CmdRespL);		
    //	CAS_DUMP("Resp",reply,CmdRespL);
    //	CAS_DUMP("Sw",reply+CmdRespL,2);
    	return 0;
	}
	else
	{
		CAS_Dbg("%s	faliure	sc_result=%d	replylen=%d\n",__FUNCTION__,sc_result,replylen-2);
		return 0;
	}
}
U32	WFSMART_CloseCard(VOID)
{
	CAS_Dbg("%s\n",__FUNCTION__);
	return 0;
    /*
    uint8_t sc_result;
	sc_result = app_porting_ca_smart_card_close();
	if(sc_result >= 0)
		return 0;
	else
		return 1;
	*/
}

VOID	WFSTB_Power(BOOL On)
{
	CAS_Dbg("%s\n",__FUNCTION__);
	return;
}
VOID	WFSTB_Panel(BOOL Lock)
{
	CAS_Dbg("%s\n",__FUNCTION__);
	return;
}
VOID	WFSTB_PPTNotify(BOOL IsPPV,U16 Interval,U16 UnitPrice)
{
	CAS_Dbg("%s	IsPPV=%d	Interval=%d	UnitPrice=%d\n",__FUNCTION__,IsPPV,Interval,UnitPrice);
	if(IsPPV == TRUE)
		pptvbuy.IsPPV = 1;
	else if(IsPPV == FALSE)
		pptvbuy.IsPPV = 2;
	pptvbuy.Interval = Interval;
	pptvbuy.UnitPrice = UnitPrice;
	pptvbuy.wStatus = TRUE;

	app_cas_api_on_event(DVB_CA_IPPV_POP_INFO,CAS_NAME, &pptvbuy, sizeof(wf_cas_pptv_buy_st));
	
	return;
}
VOID	WFSTB_EmailNotify(WFSTB_Email_t *Email)
{
//	CAS_Dbg("%s	Email->Type=%d	Email->Identfier=%d	strlen(Email->Text)=%d	Email->Text:%s\n",__FUNCTION__,Email->Type,Email->Identfier,strlen(Email->Text),Email->Text);
		
	if(Email->Type == TRUE)
	{
		emailnotify.wStatus = TRUE;
		emailnotify.buffer = (uint8_t*)Email;
		app_cas_api_on_event(DVB_CA_EMAIL_INFO,CAS_NAME, &emailnotify, sizeof(wf_cas_email_st));
	}
	else
	{
		rollingmsg.wStatus = TRUE;
		rollingmsg.buffer = (uint8_t*)Email;
		app_cas_api_on_event(DVB_CA_ROLLING_INFO,CAS_NAME, &rollingmsg, sizeof(wf_ca_rolling_message_st));
	}
	return;
}
VOID	WFSTB_EntitlementNotify(U16 Remains)
{
	CAS_Dbg("%s\n",__FUNCTION__);
	return;
}
VOID	WFSTB_DownLoadNotify(U08 Type,U08 Version,U16 CurrentPiece,U16 LastPiece,U08 Bytes,U08 *Data)
{
	CAS_Dbg("%s\n",__FUNCTION__);
	return;
}
VOID	WFSTB_SetDesDck(U08 *Dck/* 16字节 */)
{
	CAS_Dbg("%s\n",__FUNCTION__);
	return;
}
VOID	WFSTB_SetDesCw(BOOL IsCipherCw/* TRUE=CW加密，否则CW是明文 */,WFSTB_DesCw_t *DesCw)
{
	CAS_Dbg("%s\n",__FUNCTION__);
	if (DesCw->Parity == TRUE)
		CAS_DUMP("odd_key",DesCw->Cw,8);
	else
		CAS_DUMP("even_key",DesCw->Cw,8);

	int32_t ret = 0;
 	static U08 even_key[8] = {0,};
	static U08 odd_key[8] = {0,};
    
#if 0
	//uint8_t*pEvenKey = (uint8_t*)DesCw->Cw;
	int i, j, k;
	 for(i=0;i<2;i++)
	 {
		k = 0;
		for(j=0;j<3;j++)
			k +=DesCw->Cw[4*i+j];
		DesCw->Cw[4*i+3] = k&0xff;
	 }
#endif
	if (DesCw->Parity == TRUE)
	{
		memcpy(odd_key,DesCw->Cw,8);
	}
	else
	{
		memcpy(even_key,DesCw->Cw,8);
	}
    
#if 0
	static int flag=0;
//	static U08 test_even_key[8] = {0,};
//	static U08 test_odd_key[8] = {0,};

	static U08 test_odd_key[]={0xcd,0x26,0x9c,0x8f,0x98,0x25,0x08,0xc5};
	static U08 test_even_key[]={0x63,0xc2,0x3a,0x5f,0xbe,0x40,0xfe,0xfc};
	if(flag==0)
	{
		//if ((DesCw->Parity == TRUE )&&(DesCw->Cw[0]==0xcd))
		{
			//odd_key[]={0xcd,0x26,0x9c,0x8f,0x98,0x25,0x08,0xc5};
			//even_key[]={0x63,0xc2,0x3a,0x5f,0xbe,0x40,0xfe,0xfc};
			memcpy(odd_key,test_odd_key,8);
			memcpy(even_key,test_even_key,8);
			flag=1;
			printf("*******set init cw\n");
		}
	}
	else
	{
		return;
		
	}
#endif
    
#if 0
	ret = GxDescrmb_SetCW(g_descramlbeList[0].handle, odd_key, even_key, 8);
	CHECK_RET(DESCRAMBLER,ret);

	ret = GxDescrmb_SetCW(g_descramlbeList[1].handle, odd_key, even_key, 8);
	CHECK_RET(DESCRAMBLER,ret);
#else
    ret = app_porting_ca_descramble_set_cw(0,odd_key,even_key,8,0);
    CHECK_RET(DESCRAMBLER,ret);
#endif

	return;
}
VOID	WFSTB_SwitchProgram(WFCAS_Program_t *Program)
{
	CAS_Dbg("%s\n",__FUNCTION__);
	return;
}
VOID	WFSTB_OpReport(U08 OperateCode,U32 ErrorCode)
{
	CAS_Dbg("%s-----OperateCode:%d------ErrorCode:0x%8x\n",__FUNCTION__,OperateCode,(unsigned int)ErrorCode);
	U08 ucMessageType_new = 0;

	switch(ErrorCode)
	{
		case WFERR_CAS_ADDRESS :
			Show_time_short = 1;
			break;
		default :
			break;			
	}

	switch(ErrorCode)
	{
		case WFERR_COMMON_NO :
		case WFERR_COMMON_PARAMETER :
		case WFERR_COMMON_MEMALLOC :
		case WFERR_COMMON_MEMFREE :
		case WFERR_COMMON_ILLEAGAL :
		case WFERR_COMMON_VERIFY :
		case WFERR_COMMON_MEMEXEC :
			ucMessageType_new = ErrorCode;
			break;
		case WFERR_SMART_ABSENT :
			ucMessageType_new = 0x00000006L+1;
			break;
		case WFERR_SMART_INVALID:
			ucMessageType_new = 0x00000006L+2;
			break;
		case 0x00040000L:
		case 0x00040001L:
		case 0x00040002L:
		case 0x00040003L:
		case 0x00040004L:
		case 0x00040005L:
		case 0x00040006L:
		case 0x00040007L:
		case 0x00040008L:
		case 0x00040009L:
		case 0x0004000AL:
		case 0x0004000BL:
		case 0x0004000CL:
		case 0x0004000DL:
			ucMessageType_new = ErrorCode-0x00040000L+6+2+1;
			break;
		default :
			CAS_Dbg("!!!!!!!!!!!!!!!!WFSTB_OpReport Other Error!!!!!!!!!!!!!!!!!\n");
			break;
		
			
	}
	if (MSG_POP_COMMON_END+ucMessageType_new+1 >=64)
			return ;

	if ( TRUE != app_play_get_msg_pop_type_state(MSG_POP_COMMON_END+ucMessageType_new+1))
	{
		app_play_clear_ca_msg();
		app_play_set_msg_pop_type(MSG_POP_COMMON_END+ucMessageType_new+1);
	}
	return;
}
VOID	WFSTB_SendStringToCom(CHAR *String)
{
	CAS_Dbg("%s\n",__FUNCTION__);
	return;
}

BOOL WF_SetEmmDataFilter( uint8_t        byReqID,
							   const uint8_t* pbyFilter,
							   const uint8_t* pbyMask,
							   uint8_t        byLen,
							   uint16_t       wPid,
							   uint8_t        byWaitSeconds )
{
	ca_filter_t filter = {0};
	uint8_t     match[18] = {0,};
	uint8_t     mask[18] = {0,};
    	uint16_t     i=0;

	if(pbyFilter == NULL || pbyMask == NULL )
	{
		return FALSE;
	}
	
	match[0]	 = tFiltDataInEmm.tEmmFilterDate[byReqID].TableId;
	mask[0] = 0xFF;
	
	for (i = 0; i< byLen; i++)
	{

		match[i+3]	 = pbyFilter[i];
		mask[i+3] = pbyMask[i];

	}
//	DUMP(DEMUX,INFO,byLen,pbyFilter,"%x");
//	DUMP(DEMUX,INFO,byLen,pbyMask,"%x");
	CAS_DUMP("match",match,byLen+3);
	CAS_DUMP("mask",match,byLen+3);
	
	filter.byReqID = byReqID;
	filter.crcFlag = FALSE;
	filter.filterLen = byLen+3;
	memcpy(filter.match,match,byLen+3);
	memcpy(filter.mask,mask,byLen+3);
	filter.nWaitSeconds = byWaitSeconds;
	filter.pid = wPid;
	filter.equalFlag = TRUE,
	filter.repeatFlag = TRUE;
	filter.Callback = app_wf_cas_emm_filter_notify;
	filter.timeOutCallback = NULL;//app_wf_cas_filter_timeout_notify;
	return app_porting_ca_demux_start_filter(&filter);

}

BOOL WF_SetEcmDataFilter( uint8_t        byReqID,
							   uint32_t	table_id,
							   uint16_t       wPid,							   
							   uint8_t        byLen,
							   uint8_t        byWaitSeconds )
{
	ca_filter_t filter = {0};
	uint8_t     match[18] = {0,};
	uint8_t     mask[18] = {0,};

	if(gs_nEnbleTable == 1)
	{
		match[0]	 = table_id;
		mask[0] = 0xFE;
		gs_nEnbleTable = 0;
	}

//	DUMP(DEMUX,INFO,byLen,pbyFilter,"%x");
//	DUMP(DEMUX,INFO,byLen,pbyMask,"%x");
	CAS_DUMP("match",match,byLen);
	CAS_DUMP("mask",mask,byLen);
	
	filter.byReqID = byReqID;
	filter.crcFlag = FALSE;
	filter.filterLen = byLen;
	memcpy(filter.match,match,byLen);
	memcpy(filter.mask,mask,byLen);
	filter.nWaitSeconds = byWaitSeconds;
	filter.pid = wPid;
	filter.equalFlag = TRUE,
	filter.repeatFlag = FALSE;
	filter.Callback = app_wf_cas_ecm_filter_notify;
	filter.timeOutCallback = NULL;//app_wf_cas_filter_timeout_notify;
	return app_porting_ca_demux_start_filter(&filter);

}
extern uint8_t app_porting_ca_demux_restart_filter(ca_filter_t filter);
BOOL WF_ResetEcmDataFilter( uint8_t        byReqID,
							   uint32_t	table_id,
							   uint16_t	wPid,
							   uint8_t        byLen,
							   uint8_t        byWaitSeconds )
{
	ca_filter_t filter = {0};
	uint8_t     match[18] = {0,};
	uint8_t     mask[18] = {0,};

	if (table_id != 0)
	{
		match[0] = table_id;
		mask[0]  = 0xff;
	}

//	DUMP(DEMUX,INFO,byLen,pbyFilter,"%x");
//	DUMP(DEMUX,INFO,byLen,pbyMask,"%x");
	CAS_DUMP("match",match,byLen);
	CAS_DUMP("mask",mask,byLen);
	
	filter.byReqID = byReqID;
	filter.crcFlag = FALSE;
	filter.filterLen = byLen;
	memcpy(filter.match,match,byLen);
	memcpy(filter.mask,mask,byLen);
	filter.nWaitSeconds = byWaitSeconds;
	filter.pid = wPid;
	filter.equalFlag = TRUE,
	filter.repeatFlag = FALSE;
	filter.Callback = app_wf_cas_ecm_filter_notify;
	filter.timeOutCallback = NULL;//app_wf_cas_filter_timeout_notify;
	return app_porting_ca_demux_start_filter(&filter);

}
uint8_t app_wf_cas_api_set_emm_filter(uint16_t emm_pid)
{
	CAS_Dbg("%s	emm_pid=%d\n",__FUNCTION__,emm_pid);
	uint32_t nIndex=0;
	for(nIndex = 0;nIndex < tFiltDataInEmm.chFilterCountNum;nIndex ++)
		WF_SetEmmDataFilter(nIndex,tFiltDataInEmm.tEmmFilterDate[nIndex].Data,tFiltDataInEmm.tEmmFilterDate[nIndex].Mask,tFiltDataInEmm.tEmmFilterDate[nIndex].Bytes,emm_pid,1);
	
	return 1;
}
uint8_t app_wf_cas_api_set_ecm_filter(uint16_t ecm_pid)
{
	CAS_Dbg("%s	ecm_pid=%d\n",__FUNCTION__,ecm_pid);
	uint32_t ecm_byReqID = tFiltDataInEmm.chFilterCountNum;
	gs_nEnbleTable = 1;
	WF_SetEcmDataFilter(ecm_byReqID,WFCAS_ECMTABLE_ID1,ecm_pid,1,1);
	return 1;
}

uint8_t app_wf_cas_api_reset_ecm_filter(uint16_t table_id,uint16_t pid)
{
	CAS_Dbg("%s	table_id=0x%x	pid=%d\n",__FUNCTION__,table_id,pid);
	uint32_t ecm_byReqID = tFiltDataInEmm.chFilterCountNum;
	WF_ResetEcmDataFilter(ecm_byReqID,table_id,pid,1,1);
	return 1;
}
VOID	WFSTB_EmmFilterNotify(UINT Count,WFSTB_FilterData_t *FilterData)
{
	CAS_Dbg("%s	Count=%d\n",__FUNCTION__,Count);
	uint32_t nIndex=0;
	WFSTB_FilterData_t *pFilterDataPara = FilterData;
	
#if 1
	uint32_t j;
	for( nIndex=0;nIndex<Count;nIndex++ )
	{
	
		CAS_Dbg("FilterData->TableId=%0x\n", pFilterDataPara->TableId);		
		CAS_Dbg("FilterData->Bytes=%x\n",pFilterDataPara->Bytes);
	

		CAS_Dbg("FilterData->Data-----------------------\n");
		for( j=0;j<9;j++ )
		{
			printf("%02x\t", pFilterDataPara->Data[j]);		
		}
		printf("\n");
		CAS_Dbg("FilterData->Mask-----------------------\n");
		for(j=0;j<9;j++ )
		{
			printf("%02x\t", pFilterDataPara->Mask[j]);		
		}
		printf("\n\n");
	
		pFilterDataPara++;
	}

	printf("\n\n");
#endif

	if(Count > MAX_FILTER_NUMS)
	{
		Count = MAX_FILTER_NUMS;
	}
	pFilterDataPara = FilterData;

	for(nIndex = 0; nIndex < Count;nIndex++)
	{
		tFiltDataInEmm.chFilterCountNum = Count;
		memcpy(&(tFiltDataInEmm.tEmmFilterDate[nIndex]), pFilterDataPara, sizeof(WFSTB_FilterData_t));
		
		pFilterDataPara++;
	}

	return;
}

VOID	*WFMEM_Malloc(UINT Size)
{
	CAS_Dbg("%s	%d\n",__FUNCTION__,Size);
	return app_porting_ca_os_malloc(Size);
}
VOID	WFMEM_Free(VOID *Ptr)
{
	CAS_Dbg("%s\n",__FUNCTION__);
	app_porting_ca_os_free(Ptr);
}


VOID	WFOS_SemWait(VOID *Semaphore)
{
	//CAS_Dbg("%s	%d\n",__FUNCTION__,*((handle_t *)Semaphore));
	if (NULL == Semaphore)
	{
		return ;
	}	
	app_porting_ca_os_sem_wait(*((handle_t *)Semaphore));
	return ;
}
VOID	WFOS_SemSignal(VOID *Semaphore)
{
	//CAS_Dbg("%s	%d\n",__FUNCTION__,*((handle_t *)Semaphore));
	if (NULL == Semaphore)
	{
		return ;
	}
	app_porting_ca_os_sem_signal(*((handle_t *)Semaphore));
	return ;
}
VOID	WFOS_SemDestroy(VOID *Semaphore)
{
	//CAS_Dbg("%s	%d\n",__FUNCTION__,*((handle_t *)Semaphore));
	if (NULL == Semaphore)
	{
		return ;
	}
	app_porting_ca_os_sem_delete(*((handle_t *)Semaphore));
	
	free(Semaphore);
	return ;
}
VOID	*WFOS_SemCreate(S32 InitCount)
{
	handle_t *semaphore=(handle_t *)malloc(sizeof(handle_t));

    app_porting_ca_os_sem_create(semaphore,InitCount);
	CAS_Dbg("%s	%d	%ld\n",__FUNCTION__,*semaphore,InitCount);		
	return (VOID	*)semaphore;
}
VOID	*WFOS_TaskCreate(WFOS_Task_t Task,VOID *Parameter,UINT StackSize)
{
	CAS_Dbg("%s	StackSize=%d\n",__FUNCTION__,StackSize);
    	int         ret;
    	handle_t * handle = NULL;
    	uint32_t priority = GXOS_DEFAULT_PRIORITY;

	if (NULL == Task)
	{
		return FALSE;
	}

	handle = (handle_t *)calloc(1, sizeof(handle));
	if (NULL == handle)
	{
		return FALSE;
	}

    	ret = app_porting_ca_os_create_task(( char * )"CA", handle, Task,
                              Parameter, StackSize*10, priority);


	if (ret != GXCORE_SUCCESS)
	{
		CA_FAIL("WFSTBCA_RegisterTask return error = 0x%x\n", ret);
		return NULL;
	}
	CAS_Dbg("%s	End\n",__FUNCTION__);
	return (VOID	*)handle;

}

void WanFa_ReleasePrivateDataFilter(uint8_t  byReqID,uint16_t wPid)
{
	uint32_t filterId = 0;
	bool releaseChannel = TRUE;
	ca_filter_t filter = {0};

	/*
	* 查找对应channel id , filter id
	*/
	
	for (filterId = 0; filterId< MAX_FILTER_COUNT;filterId++)
	{
		app_porting_ca_demux_get_by_filter_id(filterId,&filter);
		if (( 0 != filter.usedStatus)&&(0 != filter.handle)&&(0 != filter.channelhandle)
			&&(byReqID == filter.byReqID))
		{
			if (0x10 == wPid)
				{
					releaseChannel = FALSE;
				}
			else
				{
					releaseChannel = TRUE;
				}

			  app_porting_ca_demux_release_filter(filterId, releaseChannel);
			  return ;
		}
	}
	return ;
}

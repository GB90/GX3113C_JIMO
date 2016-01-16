#ifndef _GX_SMARTCARD_PARAMS_H_
#define _GX_SMARTCARD_PARAMS_H_

/* Available ISO7816-3 protocols */
typedef enum GXSMART_Protocol_e
{
    GXSMART_T0_PROTOCOL,
    GXSMART_T1_PROTOCOL
} GXSMART_Protocol_t;

typedef enum GXSMART_SCI_e
{
    GXSMART_SCI1 = 0,
    GXSMART_SCI2
} GXSMART_SCI_t;

typedef enum GXSMART_VccPol_e
{
    GXSMART_VCC_HIGH_LEVEL,
    GXSMART_VCC_LOW_LEVEL
}GXSMART_VccPol_t;

typedef enum GXSMART_StopLen_e
{
    GXSMART_STOPLEN_0BIT,
    GXSMART_STOPLEN_1BIT,
    GXSMART_STOPLEN_1D5BIT,
    GXSMART_STOPLEN_2BIT
}GXSMART_StopLen_t;

typedef enum GXSMART_DetectPol_e
{
    GXSMART_DETECT_LOW_LEVEL        = 0,
    GXSMART_DETECT_HIGH_LEVEL       = 1
}GXSMART_DetectPol_t;

typedef enum GXSMART_SmartDataConv_e
{
    GXSMART_DATA_CONV_DIRECT       = 0,
    GXSMART_DATA_CONV_INVERSE      = 1
}GXSMART_DataConv_t;

typedef enum GXSMART_SmartParityType_e
{
    GXSMART_PARITY_ODD             = 0,
    GXSMART_PARITY_EVEN            = 1
}GXSMART_ParityType_t;

typedef enum GXSMART_CardStatus_e
{
    GXSMART_CARD_INIT              = 0,
    GXSMART_CARD_IN                = 1,
    GXSMART_CARD_OUT               = 2
}GXSMART_CardStatus_t;

typedef enum GXSMART_Parity_e
{
    GXSMART_DETECT_HARD_PARITY       = 0,
    GXSMART_DETECT_SOFT_PARITY       = 1
}GXSMART_PARITY_t;

#if 0
typedef enum GXSMART_ClkStopPol_e
{
    GXSMART_CLK_STOP_LOW           = 0,
    GXSMART_CLK_STOP_HIGH          = 1
}GXSMART_ClkStopPol_t;
#endif

typedef struct GXSMART_TimeParams_s
{
    cyg_uint32 Egt;             //SCI_EGT
    cyg_uint32 Tgt;             //SCI_TGT
    cyg_uint32 Wdt;             //SCI_WDT
    cyg_uint32 Twdt;            //SCI_TWDT
}GXSMART_TimeParams_t;

typedef enum
{
	GXSMART_TC_NONE,
	GXSMART_TC_FRE   = 0X00000001,
	GXSMART_TC_ETU   = 0X00000002,
	GXSMART_TC_EGT   = 0X00000004,
	GXSMART_TC_TGT   = 0X00000008,
	GXSMART_TC_WDT   = 0X00000010,
	GXSMART_TC_TWDT  = 0X00000020,
	GXSMART_TC_ALL   = 0X0000003F
}GXSMART_TimeConfigFlags;
typedef struct GXSMART_TimeConfig_s
{
	GXSMART_TimeConfigFlags flags;
	cyg_uint32 baud_rate;
	cyg_uint32 Etu;
    cyg_uint32 Egt;             //SCI_EGT
    cyg_uint32 Tgt;             //SCI_TGT
    cyg_uint32 Wdt;             //SCI_WDT
    cyg_uint32 Twdt;            //SCI_TWDT
}GXSMART_TimeConfig_t;


typedef struct GXSMART_OpenParams_s
{
    GXSMART_TimeParams_t    TimeParams;
    GXSMART_Protocol_t      Protocol;       //GXSMART_T1_PROTOCOL
    cyg_uint32              BaudRate;       //SCI_CLK
    cyg_uint32              Etu;            //371
    GXSMART_StopLen_t       StopLen;        //0
    GXSMART_DataConv_t      IoConv;         //GXSMART_DATA_CONV_DIRECT                   //I/O convention
    GXSMART_ParityType_t    ParityType;     //GXSMART_PARATY_EVEN        0
	 cyg_uint8			AutoEtu;
	 cyg_uint8			AutoIoConvAndParity;
}GXSMART_OpenParams_t;

typedef struct GXSMART_InitParams_s
{
    GXSMART_VccPol_t        VccPole;        //GXSMART_VCC_LOW_LEVEL
    GXSMART_DetectPol_t     DetectPole;     //GXSMART_DETECT_LOW_LEVEL
}GXSMART_InitParams_t;

typedef struct GXSMART_AtrParams_s
{
    cyg_uint32  len;                            //atr len
    void        *priv;                          //buffer addr
}GXSMART_AtrParams_t;

typedef struct GXSMART_ConfigFifoLevel_s
{
    cyg_uint32  tx_fifolevel;                            //fifolevel
    cyg_uint32  rx_fifolevel;
}GXSMART_ConfigFifoLevel_t;

typedef struct GXSMART_ConfigIntPart_s
{
    cyg_uint32	intpart;
}GXSMART_ConfigIntPart_t;

typedef struct GXSMART_ConfigEn_s
{
    cyg_uint8	module_en;
}GXSMART_ConfigEn_t;

typedef struct GXSMART_ConfigAutoRec_s
{
    cyg_uint8	module_auto;
}GXSMART_ConfigAutoRec_t;


typedef struct GXSMART_ConfigDataFifoReset_s
{
    cyg_uint8	datafiforeset;
}GXSMART_ConfigDataFifoReset_t;


typedef struct GXSMART_Configmodulereset_s
{
    cyg_uint8	modulereset;
}GXSMART_Configmodulereset_t;

typedef struct GXSMART_Configstayreceive_s
{
    cyg_uint8	stayreceive;
}GXSMART_Configstayreceive_t;

/* SCI_COMS */
#define SET_OPEN_PARAMS         0x100000
#define SET_CD_POLARITY         0x100001
#define CARD_DEACT              0x100002
#define CARD_COLD_RESET         0x100003
#define CARD_HOT_RESET          0x100004
#define GET_CARD_STATUS         0x100005
#define SELECT_SCI              0x100006
#define REGISTER_CALLBACK       0x100007
/* auto etu/parity */
#define SET_PARITY              0x100008
#define SET_VCCEN_POLARITY      0x100009
#define SET_RE                  0x10000a
#define SHOW_REGS               0x10000b
#define SET_OUTPUT_DEBUG_INFO   0x10000c
#define SET_TIME_CONFIG			0x10000d
/* GXSMART_DataConv_t */
#define SET_IO_CONV				0x10000e
/* GXSMART_Protocol_t */
#define SET_PROTOCOL            0x10000f
/* GXSMART_ParityType_t */
#define SET_PARITY_TYPE         0x100010
/* GXSMART_StopLen_t */
#define SET_STOP_LEN         0x100011


/* config type */
#define SCI_SETCONFIG_FIFOLEVEL				(0x100006)
#define SCI_SETCONFIG_INTPART				(0x100007)
#define SCI_SETCONFIG_OPEN					(0x100008)
#define SCI_SETCONFIG_CD_POLARITY			(0x100009)
#define SCI_SETCONFIG_EN					(0x10000a)
#define SCI_SETCONFIG_AUTOREC				(0x10000b)
#define SCI_SETCONFIG_DATAFIFORESET			(0x10000c)
#define SCI_SETCONFIG_MODULERESET			(0x10000d)
#define SCI_GETCONFIG_INTPART				(0x10000e)
#define SCI_SETCONFIG_TIME_CONFIG			(0x10000f)
#define SCI_SETCONFIG_STAYRECEIVE			(0x100010)

#define SCI_GETCONFIG_CONFIG	(0x10000f)
#define SCI_GETCONFIG__STATUS 	(0x100010)
#endif

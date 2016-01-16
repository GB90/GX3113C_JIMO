#ifndef __GXAV_ICAM_PROPERTYTYPES_H__
#define __GXAV_ICAM_PROPERTYTYPES_H__

/*
 * add by kk , 20140922
*/

#ifdef __cplusplus
extern "C" {
#endif

#define  TYPE_ICAM28_VCC_3V                (0x00)
#define  TYPE_ICAM28_VCC_5V                (0x01)

#define  TYPE_ICAM28_VCC_ON                (0x00)
#define  TYPE_ICAM28_VCC_OFF               (0x01)


typedef enum {
	TYPE_ICAM28_BAUD_RATE_9600     = 0,
	TYPE_ICAM28_BAUD_RATE_19200    = 1,
	TYPE_ICAM28_BAUD_RATE_38400    = 2,
	TYPE_ICAM28_BAUD_RATE_76800    = 3,
	TYPE_ICAM28_BAUD_RATE_153600   = 4,
	TYPE_ICAM28_BAUD_RATE_178560   = 5,
	TYPE_ICAM28_BAUD_RATE_223200   = 6
} TYPE_ICAM28_BaudRate_e;

typedef enum {
	EVENT_CARD_REMOVE         = 0,
	EVENT_CARD_INSERT         = 1,
	EVENT_CARD_ERROR          = 2,
	EVENT_DATA_TRANSFER_OK    = 3,
	EVENT_DATA_RECEIVE_OK     = 4,
	EVENT_DATA_OVER_FLOW      = 5,
	EVENT_DATA_BAD_PARITY     = 6
} EVENT_FLAG_e;

typedef struct {
	unsigned int m_TxReady           : 1;
	unsigned int m_RxReady           : 1;
	unsigned int m_TxError           : 1;
	unsigned int m_OverFlow          : 1;
	unsigned int m_RxError           : 1;
	unsigned int m_ConventionDirect  : 1;        // 1:direct 0:inverse
	unsigned int m_InterruptPending  : 1;
	unsigned int m_CardInsert        : 1;
} TYPE_ICAM28_UartStatus_t;

typedef struct {
	unsigned int m_IO_VIA_C7 : 1;
	unsigned int m_IO_VIA_C4 : 1;
	unsigned int m_IO_VIA_C8 : 1;
	unsigned int m_C7_ON     : 1;
	unsigned int m_C4_ON     : 1;
	unsigned int m_C8_ON     : 1;
} TYPE_ICAM28_UartCommand_t;

typedef struct {
	unsigned int m_clockdivisor;
} GxIcamProperty_ClockDivisor ;

typedef struct {
	unsigned char m_vcclevel;
} GxIcamProperty_VccLevel;

typedef struct {
#define  TYPE_ICAM28_CONVENTION_INVERSE    (0x00)
#define  TYPE_ICAM28_CONVENTION_DIRECT     (0x01)
	unsigned char m_convention;
} GxIcamProperty_Convention;

typedef struct {
	unsigned char m_uartbaudrate;
} GxIcamProperty_UartBaudRate;

typedef struct {
	unsigned char m_vccswitch;
} GxIcamProperty_VccSwitch;

typedef struct {
	unsigned char m_guardtime;
} GxIcamProperty_GuardTime;

typedef struct {
	unsigned char m_resetswitch;
} GxIcamProperty_ResetCard;

typedef struct {
	TYPE_ICAM28_UartCommand_t m_pinmask;
} GxIcamProperty_UartCommand;

typedef struct {
	TYPE_ICAM28_UartStatus_t m_uartstatus;
} GxIcamProperty_UartStatus;

typedef struct {
	unsigned char m_interrupt_event;
} GxIcamProperty_InterruptEvent;

typedef struct {
	unsigned char m_insert_or_remove;
} GxIcamProperty_InsertRemove;

typedef struct {
	unsigned char *m_p_send;
	unsigned char  m_n_send;
	unsigned char  m_null_filter;
	unsigned char  m_flow_control;
	unsigned char *m_p_rece;
	unsigned char  m_n_rece;
} GxIcamProperty_SendAndReceive;

typedef struct {
	unsigned char  m_null_filter;
	unsigned char  m_flow_control;
	unsigned char *m_p_rece;
	unsigned char  m_n_rece;
} GxIcamProperty_Receive;

typedef struct {
	unsigned int   m_chip_id[2];
	unsigned int   m_icam_version;
	unsigned char  m_hash_key[16];
} GxIcamProperty_ChipProperties;

typedef struct {
	unsigned char  m_user_enable;
	unsigned char  m_user_buff[16];
	unsigned char  m_default_time;
	unsigned char  m_nonce[16];
	unsigned char  m_responce[16];
} GxIcamProperty_ResponseToChallenge;

typedef struct {
	unsigned char  m_default_time;
	unsigned char  m_nonce[16];
	unsigned char  m_responce[16];
} GxIcamProperty_EncryptData;

typedef struct {
	unsigned int  m_offset;
	unsigned int  m_read_write_data;
} GxIcamProperty_Read_Write_Register;

typedef struct{
	unsigned int  m_cw_time;
	unsigned int  m_desc_id;

#define TYPE_ICAM28_POLARITY_ODD    (1)
#define TYPE_ICAM28_POLARITY_EVEN   (2)
	unsigned int  m_even_odd;

	unsigned int  m_command_value;
	unsigned char m_cw_data[16];
} GxIcamProperty_ControlWord;

typedef struct {
	unsigned int  m_reset_card_time;
} GxIcamProperty_ConfigInfo;


typedef struct {
	unsigned int m_m2m_key_table_num;
} GxIcamProperty_M2M_UpdateKey;

typedef struct{
#define TYPE_M2M_MODE_ENCRYPT       (1)
#define TYPE_M2M_MODE_DECRYPT       (2)
	unsigned char  m_m2m_encrypt_or_decrypt;

#define TYPE_M2M_ALGORITHM_AES      (0)
#define TYPE_M2M_ALGORITHM_TDES     (1)
#define TYPE_M2M_ALGORITHM_DES      (2)
	unsigned char  m_m2m_algorithm;

#define TYPE_M2M_OPTION_ECB         (0)
#define TYPE_M2M_OPTION_CBC         (1)
#define TYPE_M2M_OPTION_CTR         (2)
	unsigned char  m_m2m_option;

#define TYPE_M2M_RESIDUE_CLEAR      (0)
#define TYPE_M2M_RESIDUE_CTS        (1)
#define TYPE_M2M_RESIDUE_RBT        (2)
	unsigned char  m_m2m_residue;

#define TYPE_M2M_SHORTMESSAGE_CLEAR (0)
#define TYPE_M2M_SHORTMESSAGE_IV1   (1)
#define TYPE_M2M_SHORTMESSAGE_IV2   (2)
	unsigned char  m_m2m_shortmessage;

	unsigned char  m_m2m_key_table_num;

	unsigned int   m_m2m_src_addr;
	unsigned int   m_m2m_dst_addr;
	unsigned int   m_m2m_size;

	unsigned char  m_m2m_iv1[16];
	unsigned char  m_m2m_iv2[16];

} GxIcamProperty_M2M_SetParamsAndExecute ;




#define TYPE_MTC_EVNET_KEY_WRITE_DONE (1)


typedef struct {
	unsigned int  m_m2m_event_index;
	unsigned int  m_m2m_event_interrupt;
} GxIcamProperty_M2M_Event;

typedef struct {
    unsigned int m_Iemm_CAData;
    unsigned int m_Iemm_CAMask;
}GxIemmProperty_CABytes_t;


typedef struct {
#define	IEMM_PID_DISABLE   (0)
#define	IEMM_PID_ENABLE    (1)
	unsigned char  m_pid_en;

#define	IEMM_TABLE_ID_FILTER_DISABLE (0)
#define	IEMM_TABLE_ID_FILTER_ENABLE  (1)
	unsigned char  m_table_id_en;
	unsigned int   m_emm_pid;


#define IEMM_TABLE_ID_FILTER_REJECT          (0)
#define IEMM_TABLE_ID_FILTER_WITHOUT_CA_ADDR (1)
#define IEMM_TABLE_ID_FILTER_WITH_CA_ADDR    (2)
#define IEMM_TABLE_ID_FILTER_RESERVE         (3)
	unsigned char            m_table_id_filter_mode[16];
	GxIemmProperty_CABytes_t m_CABytes[8];
	unsigned char            m_CACtrlID[8];
} GxIemmProperty_Request ;


typedef struct {
	unsigned int  m_reserver;
} GxIemmProperty_Stop ;


typedef struct {
	unsigned int  m_run_en;
	unsigned int  m_delay_time;
} GxIemmProperty_Run;

typedef struct {
	unsigned char   m_only_get_overflow_interrupt;
	unsigned char   m_overflow_interrupt;
	unsigned char   m_arriver_interrupt;
	unsigned char   m_hardware_software_interrupt;
	unsigned char  *m_p_data;
} GxIemmProperty_InterruptEvent;


#ifdef __cplusplus
}
#endif

#endif


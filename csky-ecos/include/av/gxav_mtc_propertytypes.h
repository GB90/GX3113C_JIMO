
#ifndef __GXAV_MTC_PROPERTYTYPES_H__
#define __GXAV_MTC_PROPERTYTYPES_H__

/* Cooperation with C and C++ */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	CLIENT_AD_NDS = 0 ,
	CLIENT_AD_CDCAS,
	CLIENT_AD_YXSB,
	CLIENT_AD_SMSX,
	CLIENT_OR_OTP,
	CLIENT_OR_CPU,
	CLIENT_OR_MEMORY
}GXMTC_Client_e;

typedef enum
{
	CONFIG_BIT_BIG_MODE    = 0 ,
	CONFIG_BIT_DES_KEY_LOW = 1 ,
	CONFIG_BIT_DES_KEY_MULTI_LAYER_LOW = 2 ,
	CONFIG_BIT_MAX
}GXMTC_Config_e;


typedef struct
{
	unsigned int  m_config_bits      ;
}GxMTCProperty_Config ;


#define MTC_API_DATA_ENCRYPT    (1)
#define MTC_API_DATA_DECRYPT    (0)

#define MTC_API_NORMAL_MODE     (0)
#define MTC_API_CA_MODE         (1)

#define MTC_API_ALGORITM_DES     (0)
#define MTC_API_ALGORITM_AES     (1)
#define MTC_API_ALGORITM_MULTI2  (2)
#define MTC_API_ALGORITM_SMS4    (3)

#define MTC_API_GROUP_DES         (0)
#define MTC_API_GROUP_3_DES       (1)

#define MTC_API_GROUP_128BIT_AES  (0)
#define MTC_API_GROUP_192BIT_AES  (1)
#define MTC_API_GROUP_256BIT_AES  (2)

#define MTC_API_OPTION_MODE_ECB     (0)
#define MTC_API_OPTION_MODE_CBC     (1)
#define MTC_API_OPTION_MODE_CFB     (2)
#define MTC_API_OPTION_MODE_OFB     (3)
#define MTC_API_OPTION_MODE_CTR     (4)

#define MTC_API_RESIDUE_MODE_CLEAR  (0)
#define MTC_API_RESIDUE_MODE_CTS    (1)
#define MTC_API_RESIDUE_MODE_RBT    (2)

#define MTC_API_SHORT_MESSAGE_MODE_CLEAR  (0)
#define MTC_API_SHORT_MESSAGE_MODE_IV     (1)
#define MTC_API_SHORT_MESSAGE_MODE_IV2    (2)

#define MTC_API_KEY_FROM_IPBUS             (0)
#define MTC_API_KEY_FROM_OTP               (1)
#define MTC_API_KEY_FROM_NDS               (2)
#define MTC_API_KEY_FROM_CDCAS_AES_CP      (3)
#define MTC_API_KEY_FROM_CDCAS_TDES_CP     (4)
#define MTC_API_KEY_FROM_MULTI_LAYER       (5)
#define MTC_API_KEY_FROM_MEMORY            (6)

#define MTC_API_CHANNEL_MODE_CW            (0)
#define MTC_API_CHANNEL_MODE_NONCE         (1)

typedef enum
{
	LAYER_NUM_1  = 1,
	LAYER_NUM_2     ,
	LAYER_NUM_3     ,
	LAYER_NUM_4     ,
	LAYER_NUM_5     ,
	LAYER_NUM_OTHER ,  //
	LAYER_NUM_MAX
}GXMTC_LayerNum_e;

typedef struct
{
	unsigned char encrypt_decrypt;
	unsigned char m_algoritm     ;
	unsigned char m_group        ;
	unsigned char m_option_mode  ;
	unsigned char m_residue_mode ;
	unsigned char m_short_mode   ;
	unsigned char m_key_from     ;
	unsigned char m_key_set      ;
	unsigned char m_layer        ;
	unsigned char m_desc_id      ;
	unsigned int  m_main_client  ;
	unsigned int  m_item_client  ;
	unsigned int  m_dsk[6]       ;
	unsigned int  m_dck[6]       ;
	unsigned int  m_dcw[6]       ;
	unsigned int  m_vcw[6]       ;
	unsigned int  m_mcw[6]       ;
	unsigned int  m_keylen       ;   //bit , BYTE * 8 
	unsigned int  *m_pkeydata    ;
	unsigned int  m_read_addr    ;
	unsigned int  m_write_addr   ;
	unsigned int  m_rwlen        ;
	unsigned int  m_counter[4]   ;
	unsigned int  m_iv_1[4]      ;
	unsigned int  m_iv_2[4]      ;
}GxMTCProperty_Run;


#ifdef __cplusplus
}
#endif

#endif /* __GXAV_MTC_PROPERTYTYPES_H__ */

#ifndef _GS_CA_PROCESS_H_
#define _GS_CA_PROCESS_H_

typedef unsigned char byte;

extern byte Ca_Flag;

#define Qz_Dbg(...)     \
                            if(Ca_Flag){\
                                do {                               \
                                    printf("[Qz_cas]\t");            \
                                    printf(__VA_ARGS__);                \
                                } while(0);}


#define Qz_DUMP(str, ptr, size)                                                                    \
                    if(Ca_Flag){ \
                        do {                                                                        \
                            int i;                                                                  \
                                printf("\t\%s  len=%d\n\t", (str), (size));     \
                            if (size != 0) {                                                        \
                                for (i = 0; i < (size); i++) {                                      \
                                    printf("0x%02x,", (ptr)[i]);                                      \
                                }                                                                   \
                                printf("\n\t\n");                               \
                            }                                                                       \
                        } while (0);}


typedef enum
{
	HOST_MODE = 0, 
	SLAVE_MODE
}HOST_SLAVE_Mode_E;

typedef enum
{
	CA_QZ_SUCCESS=0,
    CA_QZ_ERR_NO_MEMORY,
    CA_QZ_ERR_ILLEGAL_CARD,
    CA_QZ_ERR_AUTH_OVERDUE,
    CA_QZ_ERR_AUTH_NOTSTART,
    CA_QZ_ERR_CARD_NOTMATCH,
    CA_QZ_ERR_CARD_NOTINIT,
    CA_QZ_ERR_PPV_OUTDATE,
    CA_QZ_ERR_PINERR_CARD_PAIRED,
    CA_QZ_ERR_ECM_NOT_AUTHORIZED,
    CA_QZ_ERR_ZONE_CODE,
    CA_QZ_ERR_LIMIT_CARD,
    CA_QZ_ERR_NOT_CHILD_MODE,
    CA_QZ_ERR_NO_CHILD_AUTH,
    CA_QZ_ERR_CHILD_NOT_AUTHORIZED,
    CA_QZ_ERR_CHILD_AUTH_TIMEOUT,
    CA_QZ_ERR_CHILD_AUTH_EXPIRED,
    CA_QZ_ERR_CARD_NOTFOUND,
}QZ_CA_MG_STATUS;


/* ������ */
//��ȷ
#define ERR_OK                      0x00
//EMM�����п��Ŵ���
#define ERR_INVALID_ADDRESS         0x01
//����CRC����
#define ERR_CRC                     0x02
//��δ��ʼ��
#define ERR_NOT_INI                 0x03
//�������������
#define ERR_INVALID_TYPE            0x04
//ECM����
#define ERR_ECM                     0x05
//δ֪����
#define ERR_UNKNOWN                 0x06
//����̫��
#define ERR_DATA_LENGTH             0x07
//�Ѵ�����������ݣ������ECM����������CW
#define ERR_OLD_DATA                0x08
//ͨѶ����
#define ERR_COMM                    128


//�����
#define GS_CLA                            0xE5
//ָ����

#define INS_INI_SMART                     0x20
#define INS_GET_CA_SYSTEM_ID		      0x22
#define INS_GET_SMARTCARD_ID		      0x24
#define INS_GET_CARD_NO			          0x26
#define INS_GET_CARD_ID			          0x28
#define INS_GET_AREA_CODE		          0x2A
#define INS_CHECK_SMART			          0x7C
#define INS_PROCESS_ECM			          0x7E
#define INS_PROCESS_EMM			          0x36
#define INS_GET_STB_SN			          0x38
#define INS_GET_VERSION_INFO		      0x3A
#define INS_GET_SUBSCRIBER_INFO		      0x3C
#define	INS_PAIR_STB_SMARTCARD			  0x3E
#define	INS_UNPAIR_STB_SMARTCARD		  0x40
#define	INS_DELETE_SMARTCARD              0x80
#define INS_GET_EXPIRE_DATE 		      0x2C
#define INS_GET_CHILD_AUTH_INFO           0x46
#define INS_SET_CHILD_AUTH_INFO           0x48
#define INS_READ_CHILD_AUTH_INFO          0x4a
#define INS_GET_CARD_TYPE                 0x4c
#define INS_GET_STAGE_DATA                0x82
#define INS_CHECK_SMARTCARD_APDU          0x84
#define INS_PROCESS_ECM_APDU              0x86
//������
#define PARA_DEFAULT                      0

//����ֵ
/*û��״̬��*/
#define NO_SW1SW2                         0x0000
/*״̬��ȷ*/
#define SW1SW2_OK                         0x9000
/*���ܿ��ڴ����*/
#define SW1SW2_MEMORY_ERROR               0x6581
/*APDU�����(lc)����*/
#define SW1SW2_WRONG_LC_LENGTH            0x6700
/*���ݴ��� ͨ����CRC����*/
#define SW1SW2_INCORRECT_DATA             0x6A80
/*APDU��������*/
#define SW1SW2_INCORRECT_PARA             0x6A86
/*APDU �����ֽڳ��ȣ�le������*/
#define SW1SW2_WRONG_LE_LENGTH            0x6C00
/*APDU���֧��*/
#define SW1SW2_INS_NOT_SUPPORTED          0x6D00
/*APDU �����벻֧��*/
#define SW1SW2_CLA_NOT_SUPPORTED          0x6E00
/*δ֪���ʹ���һ����ͨѶ����*/
#define SW1SW2_GENERAL_ERROR              0x6F00
/*��Ȩ����*/
#define SW1SW2_EXPIRED                    0x6FA0
/*δ����Ȩ��ʼ����*/
#define SW1SW2_NOT_START		  	      0x6FA1
/*���������*/
#define SW1SW2_STB_SN_NOT_MATCH           0x7000
/*���ܿ�δ��ʼ��*/
#define SW1SW2_CARD_NOT_INIT              0x7001
/*�����SUPER_CAS_ID*/
#define SW1SW2_INCORRECT_SUPER_CAS_ID     0x7002
/*��������ܿ��ţ� ���˵����ݲ������ſ���*/
#define SW1SW2_INCORRECT_SMARTCARD_ID     0x7003
/*PPV����*/
#define SW1SW2_PPV_EXCEED                 0x7004
/*���ܿ�û������Ϊ�������*/
#define SW1SW2_NOT_PAIRED                 0x7005
/*Ƶ��δ��Ȩ*/
#define SW1SW2_ECM_NOT_AUTHORIZED         0x7006
/*�����޲�*/
#define SW1SW2_LIMIT_AREA                 0x7007
/*���޲�*/
#define SW1SW2_LIMIT_CARD                 0x7008
/*���ܿ�û������Ϊ�ӿ�ģʽ*/
#define SW1SW2_NOT_CHILD_MODE             0x7009
/*û���ӿ���Ȩ*/
#define SW1SW2_NO_CHILD_AUTH              0x700a
/*�ӿ�δ��Ȩ*/
#define SW1SW2_CHILD_NOT_AUTHORIZED       0x700b
/*�����ӿ���Ȩ��ʱ*/
#define SW1SW2_CHILD_AUTH_TIMEOUT         0x700c
/*�ӿ���Ȩ����*/
#define SW1SW2_CHILD_AUTH_EXPIRED         0x700d


#define  QZ_CARD_NOTFOUND                 0x9900

/*
���Ÿ�ʽ��
emm_rest��ǰ2�ֽ�Ϊ���ų��ȣ����2�ֽ�Ϊ��ca_system_id��ʼ���ݵ�CRC�����㷨Ϊcrc16ccitt��ʣ���Ϊ�������ݡ�
�ʼ���ʽ��
emm_rest��ǰ2�ֽ�Ϊ�ʼ����ȣ����2�ֽ�Ϊ��ca_system_id��ʼ���ݵ�CRC�����㷨Ϊcrc16ccitt��ʣ���Ϊ�ʼ����ݡ�

*/
typedef struct emm_data
{
    unsigned char table_id;      //0x80
    unsigned char version;       //not used
    unsigned char pack_length;
    unsigned char super_cas_id[4];
    unsigned char smartcard_id[3];
    unsigned char emm_type;      //0x00 for common emm, 0x01 for message, 0x02 for mail
    unsigned char emm_rest[0];//[pack_length - 8]
}EMM_DATA;

typedef struct mail_data
{
	unsigned char mail_id;
	unsigned char year_high_byte;
	unsigned char year_low_byte;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
	unsigned char sender_length;//������
	unsigned char *psender;//[sender_length];
	unsigned char caption_length;//����
	unsigned char *pcaption;//[caption_length];
	unsigned char content_length;//����
	unsigned char *pcontent;//[content_length];
}MAIL_DATA;

typedef struct ecm_data
{
    unsigned char table_id;      //0x81
    unsigned char version;       //not used
    unsigned char pack_length;
    unsigned char super_cas_id[4];
    unsigned char ecm_rest[0];//pack_length - 4;
}ECM_DATA;


unsigned short crc16ccitt(byte *pBuffer, unsigned long uBufSize);

/*APDU �������*/
//APDU ��Ҫ�� CLA, INS, P1, P2, [lc], [data], [le]���, ���Է�Ϊ����4�����:
// 1��û�����ݣ���Ҳû�з�������
unsigned char apdu_package_v1(
    unsigned char *cache,
    unsigned char cla,
    unsigned char ins,
    unsigned char p1,
    unsigned char p2
    );
// 2��û�����ݣ������з�������
unsigned char apdu_package_v2(
    unsigned char *cache,
    unsigned char cla,
    unsigned char ins,
    unsigned char p1,
    unsigned char p2,
    unsigned char le
    );

//3�������ݣ�û�з�������
unsigned char apdu_package_v3(
    unsigned char *cache,
    unsigned char cla, 
    unsigned char ins, 
    unsigned char p1, 
    unsigned char p2,
    unsigned char lc,
    unsigned char *input_data
    );
//4�������ݣ�Ҳ�з�������,���ڲ�����le�ֽڣ��������ַ�ʽһ��
unsigned char apdu_package_v4(
    unsigned char *cache,
    unsigned char cla, 
    unsigned char ins, 
    unsigned char p1, 
    unsigned char p2,
    unsigned char lc,
    unsigned char *input_data
    );


byte check_card_apdu(const char *stb_sn, byte stb_sn_len, byte *super_cas_id, byte *smartcard_id, byte *cw_mask);
byte check_card_apdu_ex(unsigned char version_number, const char *stb_sn, byte stb_sn_len, byte *super_cas_id, byte *smartcard_id, byte *cw_mask);
byte process_emm_apdu(const byte *emm_data, byte emm_len);
byte process_ecm_apdu(const byte *ecm_data, byte ecm_len, byte *cw_mask, byte *cw);

byte get_expire_date(byte *expire_date);
byte get_area_code(byte *area_code);

void set_cw(unsigned short wEcmPID,byte *cw);
byte memncmp(const byte *dst, const byte *src, byte length);
void Ca_CardIn();
void Ca_CardOut();
byte Ca_Card_Ready();
void QZ_Init_CAS(byte bDebugOpen);

#endif



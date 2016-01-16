#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gxapp_sys_config.h"
#if FACTORY_SERIALIZATION_SUPPORT
// for test
#if 0
#define SMARTCARD_ID_ADDR   0x780
#define PROVIDER_ID_ADDR    0x788
#define CHIP_ID_ADDR        0x348
#define LOCK_ADDR           0x3ff //control bit is 7
#endif
#define SMARTCARD_ID_ADDR   0x3A3
#define PROVIDER_ID_ADDR    0x3B3
#define LOCK_ADDR           0x3C3 //control bit is 7


#define USE_0XFF_MARK_INVALID

extern int chip_otp_device_read(unsigned int addr, unsigned int read_bytes, unsigned char *data);
extern int chip_otp_device_write(unsigned int addr, unsigned int write_bytes, unsigned char *data);
extern void otp_read_hard(unsigned int start_addr, unsigned int rd_num, unsigned char *data);
extern void otp_write_hard(unsigned int start_addr, unsigned int wr_num, unsigned char *data);
extern char function_des3(unsigned char *pucKey, unsigned char *pucSourceData, unsigned int uiLen, unsigned char *pucDestOut, unsigned char ucMode);
int get_chipId(unsigned char *id,int len)
{
     int i = 0;
#define CHIDLEN 8    
	unsigned char otp_chipid[CHIDLEN]={0};
    unsigned  int *p = NULL;

    if(!id || len <CHIDLEN)
    {
        return -1;
    }

    
    p = (unsigned  int *)otp_chipid;
    *p = *(volatile unsigned int *)0xa030a564;
    //printf("h=%X\n",*p);
    
    p = (unsigned  int *)(otp_chipid+4);
    *p = *(volatile unsigned int *)0xa030a560;
    //printf("l=%X\n",*p);
    memcpy(id,otp_chipid,CHIDLEN);
#if 1
    printf("\nchipid:");
    for (i = 0; i < CHIDLEN; i++)
	{
		printf("%02X", otp_chipid[i]);
	}
    printf("\n");
#endif

    return 1;
}
int get_card_id(char *buffer, unsigned int bytes)
{
    int ret =0;
    unsigned char en_card_id[8] = {0};
    unsigned char chip_id[8] = {0};
    unsigned char card_id[8] = {0};
    if((bytes != 8) || (NULL == buffer))
    {
        return -1;
    }
    otp_read_hard(SMARTCARD_ID_ADDR, 8, en_card_id);

    get_chipId(chip_id,8);
    //ret = chip_otp_device_read(CHIP_ID_ADDR, 8, chip_id);
    function_des3(chip_id, en_card_id, 8, card_id, 1);
	ret = atoi((const char*)card_id);
	return ret;
}

int set_card_id(char *buffer, unsigned int bytes)
{
    int ret =0;
    int i = 0;
    unsigned char chip_id[8] = {0};
    unsigned char temp[8] = {0};
#ifdef USE_0XFF_MARK_INVALID
    unsigned char temp1[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
#else
    unsigned char temp1[8] = {0};
#endif
    unsigned char buf[17] = {0};
    char alpha = 0;
    char bata = 0;

    if(NULL == buffer)
    {
        return -1;
    }
    // check
     otp_read_hard(SMARTCARD_ID_ADDR, 8, temp);
    if(strncmp((char*)temp,(char*)temp1, 8) != 0)
    {// had been writed
        return -1;
    }
    memset(temp, 0, 8);

     ret =  get_chipId(chip_id,8);
//    ret = chip_otp_device_read(CHIP_ID_ADDR, 8, chip_id);
    if(ret != 1)
    {
        return -1;
    }

    if(bytes < 16)
    {// set to 16bytes
        //sprintf(buf, "%016s", buffer);
        for(i = 0; i < (16-bytes); i++)
        {
            //strcat((char*)buf, (char*)'0');
            sprintf((char*)&buf[i], "%s", "0");
        }
        strcat((char*)buf, buffer);
    }
    else
    {
        memcpy(buf, buffer, 16);
    }

    // change string to hex
    for(i = 0; i < 16; i += 2)
    {
        alpha = buf[i];
        bata  = buf[i+1];
        temp1[i/2] = ((alpha&0xf)<<4) + (bata&0xf);
    }

    // encrypt by chip id
    function_des3(chip_id, temp1, 8, temp, 0);

    otp_write_hard(SMARTCARD_ID_ADDR, bytes, temp);
   
// for check
    otp_read_hard(SMARTCARD_ID_ADDR, 8, temp1);
    if(strncmp((char*)temp,(char*)temp1, 8) != 0)
    {// had been writed
        return -1;
    }

    return ret;
}

int get_provider_id(char *buffer, unsigned int bytes)
{
    int ret =0;
    unsigned char en_provider_id[8] = {0};
    unsigned char chip_id[8] = {0};
    unsigned char provider_id[8] = {0};

    if((bytes != 8) || (NULL == buffer))
    {
        return -1;
    }
    otp_read_hard(PROVIDER_ID_ADDR, 8, en_provider_id);
    ret=  get_chipId(chip_id,8);
  //  ret = chip_otp_device_read(CHIP_ID_ADDR, 8, chip_id);

    function_des3(chip_id, en_provider_id, 8, provider_id, 1);
	ret = atoi((const char* )provider_id);
    return ret;
}

int set_provider_id(char *buffer, unsigned int bytes)
{
    int ret =0;
    unsigned char chip_id[8] = {0};
    unsigned char temp[8] = {0};
    int i = 0;
#ifdef USE_0XFF_MARK_INVALID
    unsigned char temp1[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
#else
    unsigned char temp1[8] = {0};
#endif
    unsigned char buf[17] = {0}; 

    if(NULL == buffer)
    {
        return -1;
    }
    // check
    otp_read_hard(PROVIDER_ID_ADDR, 8, temp);

    if( strncmp((char*)temp,(char*)temp1, 8) != 0)
    {
        return -1;
    }
    memset(temp, 0, 8);

   ret = get_chipId(chip_id,8);
//    ret = chip_otp_device_read(CHIP_ID_ADDR, 8, chip_id);
    if(ret!= 1)
    {
        return -1;
    }

    if(bytes < 8)
    {// set to 16bytes
        //sprintf(buf, "%016s", buffer);
        for(i = 0; i < (8-bytes); i++)
        {
            //strcat((char*)buf, (char*)'0');
            sprintf((char*)&buf[i], "%s", "0");
        }
        strcat((char*)buf, buffer);

    }
    else
    {
        memcpy(buf, buffer, 8);
    }
    // encrypt by chip id
    function_des3(chip_id, buf, 8, temp, 0);

    otp_write_hard(PROVIDER_ID_ADDR, 8, temp);
   // check
    otp_read_hard(PROVIDER_ID_ADDR, 8, temp1);
    if(strncmp((char*)temp,(char*)temp1, 8) != 0)
    {
        return -1;
    }

    return ret;
}

int get_chip_id(char *buffer, unsigned int bytes)
{
    int ret =0;
    int i = 0;
    int j = 0;
    unsigned char temp[8] = {0};

    if((NULL == buffer)|| (16 != bytes))
    {
        return -1;
    }
   // ret = chip_otp_device_read(CHIP_ID_ADDR, 8, temp);
    ret = get_chipId(temp,8);
#if 1
    if(ret != 1)
    {
        return -1;
    }
#endif
    for(i = 0; i < 8; i++)
    {
        sprintf(&buffer[j++], "%x", (temp[i]>>4)&0xf);
        sprintf(&buffer[j++], "%x", temp[i]&0xf);
    }

    return ret;
}

int lock_otp(void)
{
    int ret = 0;
    unsigned char lock_data = 0;

    otp_read_hard(LOCK_ADDR, 1, &lock_data);
    lock_data |= (1<<7);
    otp_write_hard(LOCK_ADDR, 1, &lock_data);
   
    return ret;
}

//return, 1: need serialization; 0: need not serialization; -1: error
int get_factory_serialization_flag(void)
{
    char temp[8] = {0};
#ifdef USE_0XFF_MARK_INVALID
    char temp1[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
#else
    char temp1[8] = {0};
#endif
     otp_read_hard(PROVIDER_ID_ADDR, 8, (unsigned char*)temp);

    if(strncmp(temp,temp1, 8) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}
#endif

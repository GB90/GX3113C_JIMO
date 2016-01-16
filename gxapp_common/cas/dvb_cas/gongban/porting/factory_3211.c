#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "gx_nvram.h"
#include "gxapp_sys_config.h"
#if FACTORY_SERIALIZATION_SUPPORT
#ifndef NULL
#define NULL ((void*)0)
#endif

#define DEV_NAME                    "/dev/gxotp0"
#define CHIP_OTP_IOCTL_ADDR         123

static int thiz_device_fd = 0;

int chip_otp_device_open(void)
{
    if(thiz_device_fd > 0)
    {
        return 0;// have opend
    }
    thiz_device_fd = open(DEV_NAME, O_RDWR);
    if(thiz_device_fd <= 0)
    {// open failed
        return -1;
    }
    // open device success
    return 0;
}

void chip_otp_device_close(void)
{
    if(thiz_device_fd > 0)
    {
        close(thiz_device_fd);
        thiz_device_fd = 0;
    }
}

#define TEST_OPT_IN_FS
#ifdef TEST_OPT_IN_FS
static handle_t __test_nvram_handle;
static const char *_TEST_OPT_NVRAM_NAME = "_test_opt_nvram_name.bat";
#endif
int chip_otp_device_read(unsigned int addr, unsigned int read_bytes, unsigned char *data)
{
#ifdef TEST_OPT_IN_FS
	if (__test_nvram_handle == 0)
	{
//		char * empty_blank = GxCore_Malloc(64*1024);
//		memset(empty_blank,0xff,64*1024);
		__test_nvram_handle = GxNvram_Open(_TEST_OPT_NVRAM_NAME,16*1024);
//		GxNvram_Write(__test_nvram_handle,0,empty_blank,64*1024);
	}
	GxNvram_Read(__test_nvram_handle,addr,data,read_bytes);
	return 0;
#else
    int ret = 0;
    unsigned start_addr = addr;
    if((0x7ff < addr) || (NULL == data))
    {
        return -1;
    }
    if(thiz_device_fd <= 0)
    {
        thiz_device_fd = open(DEV_NAME, O_RDWR);
        if(thiz_device_fd <= 0)
        {// open failed
            return -1;
        }
    }
    ret = ioctl(thiz_device_fd, CHIP_OTP_IOCTL_ADDR, &start_addr);
    if(0 != ret)
    {// set addr failed
        return -1;
    }
    ret = read(thiz_device_fd, data, read_bytes);
    if(ret <= 0)
    {
        return -1;
    }
    return 0;
#endif
}

int chip_otp_device_write(unsigned int addr, unsigned int write_bytes, unsigned char *data)
{
#ifdef TEST_OPT_IN_FS
	if (__test_nvram_handle == 0)
	{
		__test_nvram_handle = GxNvram_Open(_TEST_OPT_NVRAM_NAME,16*1024);
	}
	GxNvram_Write(__test_nvram_handle,addr,data,write_bytes);
	return 0;
#else
    int ret = 0;
    unsigned start_addr = addr;
    if((0x7ff < addr) || (NULL == data))
    {
        return -1;
    }
    if(thiz_device_fd <= 0)
    {
        thiz_device_fd = open(DEV_NAME, O_RDWR);
        if(thiz_device_fd <= 0)
        {// open failed
            return -1;
        }
    }
    ret = ioctl(thiz_device_fd, CHIP_OTP_IOCTL_ADDR, &start_addr);
    if(0 != ret)
    {// set addr failed
        return -1;
    }
    ret = write(thiz_device_fd, data, write_bytes);
    if(ret <= 0)
    {
        return -1;
    }
    return 0;
#endif
}

#define GX_OTP_BASE     0x00F80000
#define MAX_LEN         0x40
#define CHIP3113C_BASE  0x3A3

#define OTP_CON_REG         (*(volatile unsigned int*)(GX_OTP_BASE + 0x80))  //OTP control register
#define OTP_CFG_REG         (*(volatile unsigned int*)(GX_OTP_BASE + 0x84))  //OTP config register
#define OTP_STA_REG         (*(volatile unsigned int*)(GX_OTP_BASE + 0x88))  //OTP state register





void otp_read_hard(unsigned int start_addr, unsigned int rd_num, unsigned char *data)
{
	int i;
	unsigned int otp_con_reg;

	start_addr  = start_addr << 3;
	while(!(OTP_STA_REG & (1<<10))); //check if OTP Controller is ready
	otp_con_reg = 0x43000000 | (0x1fff & start_addr);  //start address	
	for (i = 0; i < rd_num; i++)
	{
		while(OTP_STA_REG & 0x100); //check if OTP Controller is busy now; 1 for busy
		OTP_CON_REG = otp_con_reg | (0x1 << 14); //set READEN
		OTP_CON_REG = otp_con_reg;         //clear READEN
		while(!(OTP_STA_REG & 0x200)); //chekc if OTP data is ready
		data[i] = (unsigned char)(OTP_STA_REG & 0xFF);
		otp_con_reg = otp_con_reg + 8; 
	}
}

void otp_write_hard(unsigned int start_addr, unsigned int wr_num, unsigned char *data)
{
	int i;
	unsigned int otp_con_reg;
	
	start_addr  = start_addr << 3;
	while(!(OTP_STA_REG & (1<<10))); //check if OTP Controller is ready
	otp_con_reg = 0x43000000 | (0x1fff & start_addr);  //start address	
	for (i = 0; i < wr_num; i++)
	{
		otp_con_reg &= ~(0xff<<16);
		otp_con_reg |= data[i] << 16;
		while(OTP_STA_REG & 0x100); //check if OTP Controller is busy now; 1 for busy
		OTP_CON_REG = otp_con_reg | (0x1 << 15); //set BURN
		OTP_CON_REG = otp_con_reg;         //clear READEN
		otp_con_reg = otp_con_reg + 8; 
	}
}


/* 0x3A3 - 0X3E3 */
void chip3113C_otp_test(void)
{
	int i = 0;
	unsigned char otp_buf_wr[MAX_LEN];
	unsigned char otp_buf_rd[MAX_LEN];
	unsigned int  start_addr;
	int           data_num;

    data_num = 4;
	start_addr = 0x381;
	otp_read_hard(start_addr, data_num, otp_buf_rd);

    printf("-----------------------read,len:%d\n",data_num);
    for (i = 0; i < data_num; i++)
	{
		printf("otp[0x%x]=%x\n", (start_addr+i), otp_buf_rd[i]);
	}
    
	data_num = MAX_LEN;
	start_addr = CHIP3113C_BASE;
	otp_read_hard(start_addr, data_num, otp_buf_rd);

    printf("-----------------------read,len:%d\n",data_num);
    for (i = 0; i < data_num; i++)
	{
		printf("otp[0x%x]=%x\n", (start_addr+i), otp_buf_rd[i]);
	}

	data_num = 6;
	otp_buf_wr[0] = 0x11;
	otp_buf_wr[1] = 0x22;
	otp_buf_wr[2] = 0x33;
	otp_buf_wr[3] = 0x44;
	otp_buf_wr[4] = 0x55;
	otp_buf_wr[5] = 0x66;

    printf("-----------------------write,len:%d\n",data_num);
    for (i = 0; i < data_num; i++)
	{
		printf("otp[0x%x]=%x\n", (start_addr+i), otp_buf_wr[i]);
	}
	/* write 0x3a3 - 0x3a8 addr */
	otp_write_hard(start_addr, data_num, otp_buf_wr); 

    data_num = MAX_LEN;
	otp_read_hard(start_addr, data_num, otp_buf_rd);
    printf("-----------------------read,len:%d\n",data_num);
	for (i = 0; i < data_num; i++)
	{
		printf("otp[0x%x]=%x\n", (start_addr+i), otp_buf_rd[i]);
	}

	printf("otp_operation is finish");
}

#endif

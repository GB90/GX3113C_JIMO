#ifndef __GX_GPIO_H_
#define __GX_GPIO_H_

#ifndef NULL
#define NULL 0
#endif

#define ENOERR           0     /* No error */
#define EPERM            1     /* Not permitted */
#define ENOENT           2     /* No such entity */
#define ESRCH            3     /* No such process */
#define EINTR            4     /* Operation interrupted */
#define EIO              5     /* I/O error */
#define EBADF            9     /* Bad file handle */
#define EAGAIN           11    /* Try again later */
#define EWOULDBLOCK      EAGAIN
#define ENOMEM           12    /* Out of memory */
#define EBUSY            16    /* Resource busy */
#define EXDEV            18    /* Cross-device link */    
#define ENODEV           19    /* No such device */
#define ENOTDIR          20    /* Not a directory */
#define EISDIR           21    /* Is a directory */    
#define EINVAL           22    /* Invalid argument */
#define ENFILE           23    /* Too many open files in system */
#define EMFILE           24    /* Too many open files */
#define EFBIG            27    /* File too large */    
#define ENOSPC           28    /* No space left on device */
#define ESPIPE           29    /* Illegal seek */
#define EROFS            30    /* Read-only file system */    
#define EDOM             33    /* Argument to math function outside valid */

enum
{
	GX_GPIO_INPUT = 0,
	GX_GPIO_OUTPUT
};

enum
{
	GX_GPIO_LOW = 0,
	GX_GPIO_HIGH
};

enum
{
	GX_GPIO_CONFIG_INVALID = 0,
	GX_GPIO_CONFIG_VALID
};

#define GPIO_EPDDR	0x00
#define GPIO_EPDR	0x04
#define GPIO_EPBSET	0x08
#define GPIO_EPBCLR	0x0c
#define GPIO_EPFER	0x10
#define GPIO_EPFR	0x14
#define GPIO_EPPAR	0x18
#define GPIO_EPINV	0x1c
#define GPIO_EPODR	0x20

/* GPIO entry in bootloader */
struct gpio_entry_bootloader {
	unsigned char vir_gpio;		/* gpio virtual number */
	unsigned char phy_gpio;		/* gpio physical number */
	unsigned char config_valid:1;
	unsigned char io_mode:1;	/* 0: input, 1: output */
	unsigned char output_value:1;	/* 0: low, 1: high (only for output mode) */
	unsigned char reserved:5;
	unsigned char reserverd2;
};

/* GPIO table header in SRAM */
struct gpio_table_header {
	unsigned int   magic;
	unsigned char  valid;
	unsigned char  entry_num;
};

/* final GPIO entry in SDRAM */
struct gpio_entry
{
	unsigned char valid;
	unsigned char vir_gpio;	/* gpio logical number */
	unsigned char phy_gpio;	/* gpio physical number */
	unsigned char io_mode:1;	/* 0: input, 1: output */
	unsigned char output_value:1;	/* 0: low, 1: high (only for output mode) */
	unsigned char reserved:6;
};

int gx_gpio_setio(unsigned long gpio, unsigned long io);
int gx_gpio_getio(unsigned long gpio);
int gx_gpio_getlevel(unsigned long gpio);
int gx_gpio_setlevel(unsigned long gpio, unsigned long value);
int gx_gpio_getphy(unsigned long vir_gpio, unsigned long *phy_gpio);

/* Will add driver only when there are customers to use */
/*int gx_gpio_request_irq(unsigned long gpio, GX_GPIO_IRQ_HANDLER irq_handler);
int gx_gpio_free_irq(unsigned long gpio);
int gx_gpio_setinverse(unsigned long gpio, unsigned long value);
int gx_gpio_setirqmode(unsigned long gpio, unsigned long int_enabled, enum gx_gpio_int_triggermode mode);
int gx_gpio_getirqmode(unsigned long gpio, unsigned long* int_enabled, enum gx_gpio_int_triggermode* mode);*/

#endif


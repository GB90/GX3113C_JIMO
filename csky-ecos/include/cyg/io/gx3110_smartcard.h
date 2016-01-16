#ifndef _GX3110_SMARTCARD_H_
#define _GX3110_SMARTCARD_H_

#include <cyg/infra/diag.h>            // HAL polled output

#define SYS_CLK 27000000UL 

#define SCI_CLK (9600*372)

/* Smartcard register */

typedef struct sci_register
{
    volatile cyg_uint32 rSCI_CNTL1;
    volatile cyg_uint32 rSCI_CNTL2;
    volatile cyg_uint32 rSCI_CNTL3;
    volatile cyg_uint32 rSCI_STATUS;
    volatile cyg_uint32 rSCI_INTEN;
    volatile cyg_uint32 rSCI_EGT;
    volatile cyg_uint32 rSCI_TGT;
    volatile cyg_uint32 rSCI_WDT;
    volatile cyg_uint32 rSCI_TWDT;
    volatile cyg_uint32 rSCI_DATA;
    volatile cyg_uint32 rSCI_CFG_ACK2;
    volatile cyg_uint32 rSCI_CFG_CLDRST;
    volatile cyg_uint32 rSCI_CFG_HOTRST;
    volatile cyg_uint32 rSCI_CFG_PARITY;
}sci_register;
static sci_register *gx_sci_register;

/* enums for interrupt DSR */
enum sci_interrupt_status_e
{
	SCI_INTSTATUS_INVALID = 0,
	SCI_INTSTATUS_CARD_NOACK,
	SCI_INTSTATUS_REPEAT_ERROR,
	SCI_INTSTATUS_RX_FIFO_OVERFLOW,
	SCI_INTSTATUS_SCIIN,
	SCI_INTSTATUS_SCIOUT
};

/* SCI_CTL1 */
#define SCI_CTL1_CLDRST            (1 << 0)
#define SCI_CTL1_HOTRST            (1 << 1)
#define SCI_CTL1_TE                (1 << 2)
#define SCI_CTL1_RE                (1 << 3)
#define SCI_CTL1_DEACT             (1 << 4)
#define SCI_CTL1_DATAFIFO_RST      (1 << 5)
#define SCI_CTL1_RST               (1 << 6)
#define SCI_CTL1_RCV               (1 << 7)

/* SCI_CTL2 */
#define SCI_CTL2_DETECT_POL(x)     ((x) << 0)
#define SCI_CTL2_CLK_OUT_EN        (1 << 1) //not change
#define SCI_CTL2_CLKSTP            (1 << 2) //0
#define SCI_CTL2_VCCEN             (1 << 3)
#define SCI_CTL2_PT(x)             ((x) << 4) //1
#define SCI_CTL2_IO_CONV(x)        ((x) << 5)
#define SCI_CTL2_DATA_POL          (1 << 6) //0
#define SCI_CTL2_PARITY(x)         ((x) << 7)
#define SCI_CTL2_NOPARITY          (1 << 8)

#define SCI_CTL2_AUTOREC           (1 << 9)
#define SCI_CTL2_SCI_EN            (1 << 10)
#define SCI_CTL2_STOP_LEN(x)       ((x) << 11) //0
#define SCI_CTL2_VCCEN_POL(x)      ((x) << 13)
#define SCI_CTL2_TX_FIFO_LEVEL(x)  ((x) << 14) //32
#define SCI_CTL2_RX_FIFO_LEVEL(x)  ((x) << 20) //32
#define SCI_CTL2_VCCEN_CON         (1<<26)
#define SCI_CTL2_AUTO_ETU(x)		((x)<<27)
#define SCI_CTL2_AUTO_PARITY_IOCONV(x)		((x)<<28)

/* SCI_CTL3 */
#define SCI_CTL3_CLK_DIV(x)        ((x) << 0)
#define SCI_CTL3_REPEAT_TIME(x)    ((x) << 16)
#define SCI_CTL3_SCI_ETU(x)        ((x) << 20)

/* SCI_STATUS */
#define SCI_STATUS_SCOUT_FOR_INVERTED     (1 << 0) /* NOTE: this bit is SCIN for non-inverted, SCOUT for inverted */
#define SCI_STATUS_SCIN_FOR_INVERTED      (1 << 1) /* NOTE: this bit is SCOUT for non-inverted, SCIN for inverted */
#define SCI_STATUS_RX_FIFO_LEVEL_STATUS   (1 << 2)
#define SCI_STATUS_TX_FIFO_LEVEL_STATUS   (1 << 3)
#define SCI_STATUS_TX_FIFO_EMPTY          (1 << 4)
#define SCI_STATUS_RX_FIFO_OVERFLOW       (1 << 5)
#define SCI_STATUS_REPEAT_ERROR           (1 << 6)
#define SCI_STATUS_RSTFINISH              (1 << 7)
#define SCI_STATUS_CARD_NACK              (1 << 8)
#define SCI_STATUS_RECEIVE_OVER           (1 << 9)
#define SCI_STATUS_CARD_IN                (1 << 10)

/* SCI_INTEN */
#define SCIN_INTEN                        (1 << 0)
#define SCOUT_INTEN                       (1 << 1)
#define RX_FIFO_LEVEL_STATUS_INTEN        (1 << 2)
#define TX_FIFO_LEVEL_STATUS_INTEN        (1 << 3)
#define TX_FIFO_EMPTY_INTEN               (1 << 4)
#define RX_FIFO_OVERFLOW_INTEN            (1 << 5)
#define REPEAT_ERROR_INTEN                (1 << 6)
#define RSTFINISH_INTEN                   (1 << 7) 
#define CARD_NACK_INTEN                   (1 << 8)
#define RECEIVE_OVER_INTEN                (1 << 9)

/*SCI_AUTOREC*/

#define SCI_AUTOREC 	                  (1)

/*SCI_ENABLE*/
#define DISABLE			          (0)
#define ENABLE			          (1)
/* SCI_EGT */
#define SCI_EGT                           (372 * 40) //40*SCI_EGU

/* SCI_TGT */
#define SCI_TGT                           (0)

/* SCI_WDT */
#define SCI_WDT                           (372*4800) //9600 * SCI_ETU
//#define SCI_WDT (9600*372) //9600 * SCI_ETU

/* SCI_TWDT */
#define SCI_TWDT                          (9600*372)
//#define SCI_TWDT (0x8fffff)

/* SCI_CFG_ACK2 */
#define SCI_VERNIER_ACK2                  (1 << 16)

/* SCI_CFG_CLDRST */
#define SCI_CLDRST_LENGTH                 (0)
#define SCI_CLDRST_LENGTH_CONFIG          (1 << 0)

/* SCI_CFG_HOTRST */
#define SCI_HOTRST_LENGTH                 (0)
#define SCI_HOTRST_LENGTH_CONFIG          (1 << 0)

/* SCI_CFG_PARITY */
#define SCI_NOT_CALC_PARITY               (1 << 0)

typedef struct cyg_gx3110_sci_extra 
{
	cyg_vector_t        sci_isrvec;
	int                 sci_isrpri;
	cyg_handle_t        sci_interrupt_handle;   // For initializing the interrupt
	cyg_interrupt       sci_interrupt_data;

	cyg_mutex_t         sci_read_mx;
	cyg_cond_t          sci_read_cond;
	cyg_bool            sci_read_end;

	cyg_mutex_t         sci_write_mx;
	cyg_cond_t          sci_write_cond;
	cyg_bool            sci_write_end;

	cyg_mutex_t         sci_reset_mx;
	cyg_cond_t          sci_reset_cond;
	cyg_bool            sci_reset_end;

	cyg_uint32			sci_watch_dog; //wait value * 100(ticks) with in one IO ops(read,write reset)!

#define BUFFER_MAX_SIZE 512
	cyg_uint8           sci_rbuffer[BUFFER_MAX_SIZE];
	volatile cyg_uint32 sci_rbpos;
	volatile cyg_uint32 sci_rdata_num;

	cyg_uint8           sci_wbuffer[BUFFER_MAX_SIZE];
	volatile cyg_uint32 sci_wbpos;
	volatile cyg_uint32 sci_wdata_num;
}cyg_gx3110_sci_extra;

#define CYG_GX3110_SCI_EXTRA(_name_, _isr_vec_, _isr_pri_,            \
		_bpos_, _data_num_)                                   \
static cyg_gx3110_sci_extra _name_ = {                                \
	.sci_isrvec    = _isr_vec_,                                   \
	.sci_isrpri    = _isr_pri_,                                   \
	.sci_rbpos     = _bpos_,                                      \
	.sci_rdata_num = _data_num_,                                  \
} ;      


#endif //_GX3110_SMARTCARD_H_


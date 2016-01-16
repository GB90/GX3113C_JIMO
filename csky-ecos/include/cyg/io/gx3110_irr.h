#include <cyg/hal/drv_api.h>
#ifndef _CX_IRR_H_
#define _CX_IRR_H_

#include <linux/kfifo.h>
#include <cyg/fileio/fileio.h>  
//IRR_BASE_ADDR------------------
#define IRR_BASE_ADDR(_extra_)  ((_extra_)->irr_base)
#define rIRR_CNTL(_extra_)      (*(volatile unsigned int *)(IRR_BASE_ADDR(_extra_) + 0X0000))
#define rIRR_INT(_extra_)       (*(volatile unsigned int *)(IRR_BASE_ADDR(_extra_) + 0X0004))
#define rIRR_FIFO(_extra_)      (*(volatile unsigned int *)(IRR_BASE_ADDR(_extra_) + 0X0008))
#define rIRR_DIV(_extra_)       (*(volatile unsigned int *)(IRR_BASE_ADDR(_extra_) + 0X000C))

//rIRR_CNTL------------------
#define bIRR_CNTL_FIFOTM (1)
#define bIRR_CNTL_ENIR   (3)
#define bIRR_CNTL_FIFOIL (6)
#define mIRR_CNTL_FIFOIL (0x3F)

#define CNTL_TOV				(20000 << 12) 
#define CNTL_FIFOIL				(33 << 6)
#define CNTL_POL                                (1 << 5)
#define CNTL_ENIR				(1 << 3)
#define CNTL_FIFOTM				(1 << 1)

#define IRR_MODULE_EN(_extra_) (rIRR_CNTL(_extra_) |= (0x1 << bIRR_CNTL_ENIR))
#define IRR_MODULE_DIS(_extra_) (rIRR_CNTL(_extra_) &= ~(0x1 << bIRR_CNTL_ENIR))

//rIRR_INT------------------
#define bIRR_INT_FIFOTS  (1)
#define bIRR_INT_NUM     (3)
#define mIRR_INT_NUM     (0x3F)

#define IRR_INT_TIMEOUT_CLR(_extra_) (rIRR_INT(_extra_) |= (0x1) << bIRR_INT_FIFOTS) 

#define IRR_LEVEL_NUM_STATUS(_extra_) ((rIRR_INT(_extra_) >> bIRR_INT_NUM) & mIRR_INT_NUM)

//rIRR_DIV------------------
#define bIRR_DIV_DIV (0)
#define mIRR_DIV_DIV (0xFF)

#define IRR_DIV_SET(_extra_, _sys_clk_, _clock_) do{\
    rIRR_DIV(_extra_) &= ~(mIRR_DIV_DIV << bIRR_DIV_DIV);\
    rIRR_DIV(_extra_) |= ((_sys_clk_/_clock_ - 1) << bIRR_DIV_DIV);\
}while(0)

/* the follwing macro constants are decide by HARDWARE */
#define IRR_MAX_PULSE_NUM                                       (64)
#define IRR_MAX_SIMCODE_PULSE_NUM                               (3)
#define IRR_MAX_FULLCODE_DVB40BIT_PULSE_NUM                     (42)
#define IRR_MAX_FULLCODE_PANASONIC_PULSE_NUM                    (49)
#define IRR_MAX_FULLCODE_PULSE_NUM                              (33)
#define IRR_MAX_PULSE_NUM_PHILIPS                               (13)
#define IRR_MIN_PULSE_NUM_PHILIPS                               (6)
#define IRR_MAX_PULSE_NUM_BESCON                                (16)
#define IRR_MIN_PULSE_NUM_BESCON                                (9)
#define IRR_EMBEDDED_NOISE_ONCE                                 (34)
#define IRR_EMBEDDED_NOISE_MORE                                 (48)

//IRR protocol ------------------
#define IRR_PROTCOL_TIME                    (562)
#define IRR_STD_UINT_SIG_ZERO               (IRR_PROTCOL_TIME << 2)

#define IRR_STD_UINT_SIG_ONE                (IRR_PROTCOL_TIME << 1)
#define IRR_STD_UINT_SIG_ZERO_STB40         (3*IRR_PROTCOL_TIME )
#define IRR_AVERAGE_PULSE_WIDTH             \
        ((IRR_STD_UINT_SIG_ZERO + IRR_STD_UINT_SIG_ONE) >> 1)

#define IRR_AVERAGE_PULSE_WIDTH_STB40             \
        ((IRR_STD_UINT_SIG_ZERO_STB40 + IRR_STD_UINT_SIG_ONE) >> 1)

#define IRR_RC5_PROTCOL_TIME  (889)

// 1M频率下philips1位高电平或低电平的计数值
#define IRR_PROTCOL_TIME_PHILIPS            (888)

// 1M频率下bescon 1位高电平或低电平的计数值,偏大点
#define IRR_PROTCOL_TIME_BESCON             (440)

// 1M频率下panasonic 1位高电平或低电平的计数值
#define IRR_PROTCOL_TIME_PANASONIC          (380)


/* Irr extra */
typedef struct cyg_gx_irr_extra {
	cyg_uint32          irr_base;
	cyg_vector_t        irr_isrvec;
	int                 irr_isrpri;
	cyg_drv_mutex_t     irr_lock;
	cyg_drv_cond_t      irr_wait;
	cyg_handle_t        irr_interrupt_handle;   // For initializing the interrupt
	cyg_interrupt       irr_interrupt_data;

	int                 irr_mode;
	cyg_uint32          key_code;
	cyg_uint32          counter_simple;

	cyg_uint32          pulse_val[IRR_MAX_PULSE_NUM];
	cyg_uint32          pulse_num;

	cyg_alarm           irr_alarm_obj;
	cyg_handle_t        irr_alarm;

	struct CYG_SELINFO_TAG   selinfo;     // select info

	struct kfifo        *queue;

	struct irr_algorithm *algo;
}cyg_gx_irr_extra;

struct irr_algorithm {
    int (*functionality) (cyg_gx_irr_extra *);
};


typedef struct lowpower_info_s
{
    cyg_uint32 WakeTime;
    cyg_uint32 GpioMask;
    cyg_uint32 GpioData;
    cyg_uint32 key;
    cyg_uint8  *cmdline;
}lowpower_info;

//#define IRR_ERR     3
//#define IRR_WARN    4
//#define IRR_DEBUG   7

//#define GX31X0_IRR_DEBUG

#ifdef GX31X0_IRR_DEBUG
#define IRR_DBG(fmt, args...)     diag_printf(fmt, ##args)
#else
#define IRR_DBG(fmt, args...)   ((void)0)
#endif

/* irr command */
#define IRR_NONBLOCK 					(0)
#define IRR_BLOCK    					(1)
#define IRR_LOWPOWER 					(2)
#define IRR_SIMPLE_IGNORE_CONFIG 	(3)
#define IRR_DISABLE					(4)
#define IRR_ENABLE						(5)

#endif //_CX_IRR_H_

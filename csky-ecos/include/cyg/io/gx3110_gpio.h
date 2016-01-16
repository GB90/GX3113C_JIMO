#ifndef _GX3110_GPIO_H_
#define _GX3110_GPIO_H_

#include <cyg/infra/diag.h>            // HAL polled output

#define SYS_CLK 108000000UL 

#if 0
struct gpio_register
{
volatile cyg_uint32 rGPIO_EPDDR;
volatile cyg_uint32 rGPIO_EPDR;
volatile cyg_uint32 rGPIO_EPBSET;
volatile cyg_uint32 rGPIO_EPBCLR;
volatile cyg_uint32 rGPIO_EPFER;
volatile cyg_uint32 rGPIO_EPFR;
volatile cyg_uint32 rGPIO_EPPAR;
volatile cyg_uint32 rGPIO_EPINV;
volatile cyg_uint32 rGPIO_EPODR;
};
#endif

/*GPIO_ENABLE*/
#define DISABLE			(0)
#define ENABLE			(1)


typedef struct cyg_gx3110_gpio_extra {
    cyg_vector_t    gpio_isrvec;
    int             gpio_isrpri;
    cyg_handle_t    gpio_interrupt_handle;   // For initializing the interrupt
    cyg_interrupt   gpio_interrupt_data;
    cyg_drv_mutex_t     gpio_lock;
    cyg_drv_cond_t      gpio_wait;
    cyg_drv_mutex_t     gpio_lock_flag;
    cyg_drv_cond_t      gpio_wait_flag;
    //cyg_flag_t gpio_flag_var;

    cyg_uint8 gpio_buffer[300];
	volatile    cyg_uint8 gpio_bpos;
	volatile    cyg_uint32 gpio_data_num;
	volatile    cyg_uint8 gpio_card_status;
	volatile    cyg_bool gpio_select_flag;
	volatile    cyg_bool gpio_receive_finished;
}cyg_gx3110_gpio_extra;

#define CYG_GX3110_GPIO_EXTRA(_name_, _isr_vec_, _isr_pri_,              \
                             _bpos_, _data_num_, _card_status_,         \
                            _select_flag_, _receive_finishe_)           \
  static cyg_gx3110_gpio_extra _name_ = {                                \
  .gpio_isrvec   = _isr_vec_,                                             \
  .gpio_isrpri   = _isr_pri_,                                             \
  .gpio_bpos     = _bpos_,                                                \
  .gpio_data_num = _data_num_,                                             \
  .gpio_card_status= _card_status_,                                       \
  .gpio_select_flag= _select_flag_,                                       \
  .gpio_receive_finished= _receive_finishe_,                                       \
  } ;      

/** GPIO debug option **/
//#define GPIO_DEBUG
#ifdef GPIO_DEBUG
#define gpio_log_trace(FORMAT, ARGS...) diag_printf("<trace>: ""%s()""[%d]   "FORMAT, __FUNCTION__, __LINE__, ##ARGS)
#define gpio_log_debug(FORMAT, ARGS...) diag_printf("<debug>: ""%s()""[%d]   "FORMAT, __FUNCTION__, __LINE__, ##ARGS)
#define gpio_log_error(FORMAT, ARGS...) diag_printf("<error>: ""%s()""[%d]   "FORMAT, __FUNCTION__, __LINE__, ##ARGS)
#define gpio_log_enter(FORMAT, ARGS...) diag_printf("<enter>: ""%s()""[%d]   "FORMAT, __FUNCTION__, __LINE__, ##ARGS)
#define gpio_log_leave(FORMAT, ARGS...) diag_printf("<leave>: ""%s()""[%d]   "FORMAT, __FUNCTION__, __LINE__, ##ARGS)

#else
#define gpio_log_trace(FORMAT, ARGS...)do {} while (0)
#define gpio_log_debug(FORMAT, ARGS...)do {} while (0)
#define gpio_log_error(FORMAT, ARGS...)do {} while (0)
#define gpio_log_enter(FORMAT, ARGS...)do {} while (0)
#define gpio_log_leave(FORMAT, ARGS...)do {} while (0)
#endif

#endif //_GX3110_GPIO_H_

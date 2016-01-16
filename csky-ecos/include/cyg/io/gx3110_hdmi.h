#ifndef _GX3110_hdmi_H_
#define _GX3110_hdmi_H_

#include <cyg/infra/diag.h>            // HAL polled output

#define GX3110_HDMI_DEVADDR 0x70 /* 0x70 or 0x72 */

//#define HDMI_DEBUG
#ifdef HDMI_DEBUG
#define hdmi_log_trace(FORMAT, ARGS...) diag_printf("<trace>: ""%s()""[%d]   "FORMAT, __FUNCTION__, __LINE__, ##ARGS)
#define hdmi_log_debug(FORMAT, ARGS...) diag_printf("<debug>: ""%s()""[%d]   "FORMAT, __FUNCTION__, __LINE__, ##ARGS)
#define hdmi_log_error(FORMAT, ARGS...) diag_printf("<error>: ""%s()""[%d]   "FORMAT, __FUNCTION__, __LINE__, ##ARGS)
#define hdmi_log_enter(FORMAT, ARGS...) diag_printf("<enter>: ""%s()""[%d]   "FORMAT, __FUNCTION__, __LINE__, ##ARGS)
#define hdmi_log_leave(FORMAT, ARGS...) diag_printf("<leave>: ""%s()""[%d]   "FORMAT, __FUNCTION__, __LINE__, ##ARGS)
#else
#define hdmi_log_trace(FORMAT, ARGS...)do {} while (0)
#define hdmi_log_debug(FORMAT, ARGS...)do {} while (0)
#define hdmi_log_error(FORMAT, ARGS...)do {} while (0)
#define hdmi_log_enter(FORMAT, ARGS...)do {} while (0)
#define hdmi_log_leave(FORMAT, ARGS...)do {} while (0)
#endif

struct gx3110_hdmi_operation
{
	cyg_uint16 addr;
	cyg_uint8 *pdata;
	cyg_uint16 size;
};

externC cyg_int32 hal_gx_hdmi_write(cyg_uint16 addr,
                                        cyg_uint8 *pdata,
                                        cyg_uint16 size,
                                        cyg_bool   blocking);

externC cyg_int32 hal_gx_hdmi_read(cyg_uint16 addr,
                                       cyg_uint8 *pdata,
                                       cyg_uint16 size,
                                       cyg_bool   blocking);

#endif //_GX3110_hdmi_H_


#ifndef _GX_SMARTCARD_PARAMS_H_
#define _GX_SMARTCARD_PARAMS_H_





typedef struct GXGPIO_BaseSet_s
{
	cyg_uint32 			port; // port A/B/C
    cyg_uint32			Epddr;
	cyg_uint32 			Eppar;	
	cyg_uint32 			Epinv;
	cyg_uint32 			Epodr;
}GXGPIO_BaseSet_t;

typedef struct GXGPIO_Voltage_s
{
	cyg_uint32 			port; // port A/B/C
    cyg_uint32 Epdr;
	cyg_uint32 Epbset;
	cyg_uint32 Epbclr;
}GXGPIO_Voltage_t;

typedef struct GXGPIO_Intc_s
{
	cyg_uint32 			port; // port A/B/C
    cyg_uint32 Epfer;
}GXGPIO_Intc_t;

typedef struct GXGPIO_IntcState_s
{
	cyg_uint32 			port; // port A/B/C
    cyg_uint32 Epfr;
}GXGPIO_IntcState_t;

typedef struct GXGPIO_IntcOpt_s
{
	cyg_uint32 			port; // port A/B/C
    cyg_uint8 pinn;
	void (*opt)(void);
}GXGPIO_IntcOpt_t;


#define GPIO_SETCONFIG_BASESET				(0x10010000)
#define GPIO_GETCONFIG_BASESET				(0x10010001)

#define GPIO_SETCONFIG_VOLTAGE				(0x10010002)
#define GPIO_GETCONFIG_VOLTAGE				(0x10010003)

#define GPIO_SETCONFIG_INTC					(0x10010004)
#define GPIO_GETCONFIG_INTSTATE				(0x10010005)

#define GPIO_SETCONFIG_INCOPT				(0x10010005)

#define GPIO_CLRCONFIG_VOLTAGE				(0x10010006)

#endif

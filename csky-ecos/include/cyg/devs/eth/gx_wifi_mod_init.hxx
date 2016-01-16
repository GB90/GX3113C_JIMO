/************************************************************************************
  Copyright (C) 2004 Medeli Electric Industry Co., LTD.

  System Name :NationalChip
  File   Name :gx_wifi_mod_init.hxx
  Revision    :1.00
  Date        :2015.05.05
  author      :liuyx
  usage       :#include <cyg/devs/eth/gx_wifi_mod_init.hxx>
 ************************************************************************************/
#ifndef GX_WIFI_MOD_INIT_HXX
#define GX_WIFI_MOD_INIT_HXX

extern "C" void usbwifi_init(void);
extern "C" int usb_wifi_init(void);
extern int (*proc_usb_wifi_init)(void);

class cyg_gxwifidev_init_class{
public:
	cyg_gxwifidev_init_class(void)
	{
#ifdef EVIXPKG_USB_WIFI
		usbwifi_init();
		proc_usb_wifi_init = usb_wifi_init;
#endif
	}
};

#endif


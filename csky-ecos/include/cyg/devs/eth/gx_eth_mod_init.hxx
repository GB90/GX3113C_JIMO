/************************************************************************************
  Copyright (C) 2004 Medeli Electric Industry Co., LTD.

  System Name :NationalChip
  File   Name :gx_eth_mod_init.hxx
  Revision    :1.00
  Date        :2015.05.05
  author      :liuyx
  usage       :#include <cyg/devs/eth/gx_eth_mod_init.hxx>
 ************************************************************************************/
#ifndef GX_ETH_MOD_INIT_HXX
#define GX_ETH_MOD_INIT_HXX

extern "C" void synop_mac_init(unsigned int int_num);

class cyg_gxethdev_init_class {
public:
	cyg_gxethdev_init_class(void)
	{
#ifdef CYGPKG_DEVS_ETH_SYNOP_MAC
		synop_mac_init(56);
#endif
	}
};

#endif


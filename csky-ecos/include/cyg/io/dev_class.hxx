#ifndef DEV_CLASS_HXX
#define DEV_CLASS_HXX
/*==========================================================================
name:        dev_class.hxx
Author  :    gaohaibo
Date    :    2009-09-11
usage   :    #include <cyg/io/dev_class.hxx>
==========================================================================*/
/*主要放各个驱动所声明的类的头文件*/
#include <pkgconf/system.h>

#include <cyg/fs/fatfs_init.hxx>
#ifdef CYGPKG_FS_EXFAT
#include <cyg/fs/exfatfs_init.hxx>
#endif
#include <cyg/fs/ntfs_init.hxx>

#ifndef GoXtend_ECOS
#include <jffs2_init.hxx>
#endif

#include <cyg/fs/yaffs_init.hxx>
#ifdef CYGPKG_FS_MINIFS
#include <cyg/fs/minifs_init.hxx>
#endif
#include <cyg/romfs/romfs_init.hxx>
#include <cyg/ramfs/ramfs_init.hxx>
#include <cyg/fs/cramfs_init.hxx>
#ifdef CYGPKG_NET
#ifdef CYGPKG_DEVS_ETH_CKCORE
#include <cyg/devs/eth/gx_eth_mod_init.hxx>
#include <cyg/devs/eth/gx_wifi_mod_init.hxx>
#include <cyg/devs/eth/gxnetdev_init.hxx>
#endif
#endif
#ifdef CYGPKG_HAL_ARM
#include <cyg/io/gx_cf_init.hxx>
#endif
#include <cyg/io/gx_sd_init.hxx>
#include <cyg/devs/nand/gx_nand_init.hxx>
#include <cyg/io/gx_serial_init.hxx>
#include <cyg/io/gx_irr_init.hxx>
#include <cyg/io/gx_sci_init.hxx>
#include <cyg/io/gx_wdt_init.hxx>
#include <cyg/io/gx_dev_spi_init.hxx>
#include <cyg/io/gx_flashio_init.hxx>
#include <usb/gx_usb_mod_init.hxx>
#include <cyg/io/gx_i2c_init.hxx>
#endif

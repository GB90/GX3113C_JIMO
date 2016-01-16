#ifndef _CKCORE_ETH
#define _CKCORE_ETH
//==========================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with eCos; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. However the source code for this file must still be made available
// in accordance with section (3) of the GNU General Public License.
//
// This exception does not invalidate any other reasons why a work based on
// this file might be covered by the GNU General Public License.
//
// Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
// at http://sources.redhat.com/ecos/ecos-license/
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//==========================================================================

#include <cyg/hal/drv_api.h>
#include <cyg/io/eth/eth_drv_stats.h>
#include <cyg/devs/eth/nbuf.h>
//#include <pkgconf/devs_eth_ckcore.h>

#ifdef CYGPKG_HAL_ARM
#include <cyg/hal/hal_platform_ints.h>
#else
#include <cyg/hal/plf_intr.h>
#endif

#include <cyg/hal/gxchip.h>
#include <cyg/hal/gxinterrupt.h>

 /* Ethernet controller inetrrupt priority level. */
#define CKCORE_INT_LEVEL 0

/* Bit level definitions and macros */
#define CKCORE_ETH_ECR_RESET		(0x00000001)
#define CKCORE_ETH_ECR_ETHER_EN		(0x00000002)
#define CKCORE_ETH_EIR_GRA			(0x10000000)

#define CKCORE_ETH_EIR_HBE		    (0x80000000)
#define CKCORE_ETH_EIR_BR		    (0x40000000)
#define CKCORE_ETH_EIR_BT		    (0x20000000)
#define CKCORE_ETH_EIR_GRA		    (0x10000000)
#define CKCORE_ETH_EIR_TXF		    (0x08000000)
#define CKCORE_ETH_EIR_TXB		    (0x04000000)
#define CKCORE_ETH_EIR_RXF		    (0x02000000)
#define CKCORE_ETH_EIR_RXB		    (0x01000000)
#define CKCORE_ETH_EIR_MII		    (0x00800000)
#define CKCORE_ETH_EIR_EBERR		(0x00400000)
#define CKCORE_ETH_EIR_UMINT        (1<<21)

#define CKCORE_ETH_IMR_HBEEN		(0x80000000)
#define CKCORE_ETH_IMR_BREN			(0x40000000)
#define CKCORE_ETH_IMR_BTEN			(0x20000000)
#define CKCORE_ETH_IMR_GRAEN		(0x10000000)
#define CKCORE_ETH_IMR_TXFEN		(0x08000000)
#define CKCORE_ETH_IMR_TXBEN		(0x04000000)
#define CKCORE_ETH_IMR_RXFEN		(0x02000000)
#define CKCORE_ETH_IMR_RXBEN		(0x01000000)
#define CKCORE_ETH_IMR_MIIEN		(0x00800000)
#define CKCORE_ETH_IMR_EBERREN		(0x00400000)
#define CKCORE_ETH_RCR_PROM			(0x00000008)
#define CKCORE_ETH_RCR_MII_MODE		(0x00000004)
#define CKCORE_ETH_RCR_DRT			(0x00000002)
#define CKCORE_ETH_RCR_LOOP			(0x00000001)
#define CKCORE_ETH_TCR_GTS			(0x00000001)
#define CKCORE_ETH_TCR_HBC			(0x00000002)
#define CKCORE_ETH_TCR_FDEN			(0x00000004)
#define CKCORE_ETH_RDAR_DESTACT     (0x01000000)
#define CKCORE_ETH_MFLR_BRDCAST     (0x80000000)
#define CKCORE_ETH_MFLR_MLTCAST     (0x40000000)
#define CKCORE_ETH_XWMRK_64         (0x00000000)
#define CKCORE_ETH_XWMRK_128        (0x00000002)
#define CKCORE_ETH_XWMRK_192        (0x00000003)

//define ckcore eth regs
#ifdef CYGPKG_HAL_CK
#define CKMAC_DESC_BASEADDR       ((volatile unsigned long *)0xA0000400)
#define CKMAC_REG_BASEADDR        ((volatile unsigned long *)0x90000000)

#define MAC_FIFO_TXBASE           0x80600000                  //6k sram base address
#define MAC_FIFO_RXBASE           0x10000000                  
#endif

#ifdef CYGPKG_HAL_CKMMU
#define CKMAC_DESC_BASEADDR       ((volatile unsigned long *)0xA0000400)
#define CKMAC_REG_BASEADDR        ((volatile unsigned long *)0x90000000)

#define MAC_FIFO_TXBASE           0x80600000                  //6k sram base address
#define MAC_FIFO_RXBASE           0x10000000                  
#endif


#ifdef CYGPKG_HAL_ARM
//---------------------------------------------------------------------------
// CHIP CONFIG.
#define GX3110_VA_CHIP_CONFIG_BASE_SYS                  0xd050a000
#define GX3110_CHIP_CONFIG_BASE_SYS                     0x0050a000
//---------------------------------------------------------------------------
// High DMA.
#define GX3110_VA_HIGH_DMA_BASE_SYS                     0xd0509000
#define GX3110_HIGH_DMA_BASE_SYS                        0x00509000

#define CKMAC_DESC_BASEADDR       ((volatile unsigned long *)(GX3110_VA_MAC_BASE_SYS + 0x400))
#define CKMAC_REG_BASEADDR        ((volatile unsigned long *)GX3110_VA_MACREG_BASE_SYS)
#endif
                  
#define CKMAC_MODE   0x00  // mode register
#define CKMAC_INT    0x01  // interrupt register
#define CKMAC_IMASK  0x02  // interrupt mask register

#define CKMAC_IPGT   0x03  // inter packet gap register
#define CKMAC_NIPGT1 0x04  // non inter packet gap register
#define CKMAC_NIPGT2 0x05  // non inter packet gap register

#define CKMAC_SIZE   0x06  // packet size register
#define CKMAC_COLL   0x07  // collision and retry configure

#define CKMAC_TXBD   0x08  // transmit buffer descriptor
#define CKMAC_CTL    0x09  // control module mode register

#define CKMAC_MIIM   0x0a  // mii mode register
#define CKMAC_MIICMD 0x0b  // mii command register
#define CKMAC_MIIADD 0x0c  // mii address register
#define CKMAC_MIITX  0x0d  // mii transmit data register
#define CKMAC_MIIRX  0x0e  // mii receive data register
#define CKMAC_MIISTS 0x0f  // mii status register

#define CKMAC_ADDR0  0x10  // mac individual address of LSB
#define CKMAC_ADDR1  0x11  // mac individual address of MSB

#define CKETH_HASH0  0x12  // hash0 register
#define CKETH_HASH1  0x13  // hash1 register
#define CKETH_TXCTL  0x14  // transmit control register

#define CKMAC_DESC   0x100 // basic address of descriptor 

/////////////////////////////////////////////////////////////////////////////
//mode register
#define CKMAC_MODE_BES        (1 << 18)    //Little/Big Endian Select( =0,Little Endian, ReadOnly)
#define CKMAC_MODE_LPMD_RXEN  (1 << 17)    //Low power Mode Rx Enable
#define CKMAC_MODE_RECSMALL   (1 << 16)    //Receive small packets
#define CKMAC_MODE_PAD        (1 << 15)    //Padding enable
#define CKMAC_MODE_HUGEN      (1 << 14)    //Huge packets enable
#define CKMAC_MODE_CRCEN      (1 << 13)    //CRC enable
#define CKMAC_MODE_DLYCRCEN   (1 << 12)    //Delayed CRC Enable
#define CKMAC_MODE_ENDIAN     (1 << 11)    //CONFIG ENDIAN
#define CKMAC_MODE_FULLD      (1 << 10)    //Full Duplex
#define CKMAC_MODE_EXDFREN    (1 << 9)     //Excess Defer enable
#define CKMAC_MODE_NOBCKOF    (1 << 8)     //No Backoff
#define CKMAC_MODE_LOOPBACK   (1 << 7)     //Loop Back
#define CKMAC_MODE_IFG        (1 << 6)     //Interframe gap for incoming frames
#define CKMAC_MODE_PRO        (1 << 5)     //PROMISCUOUS
#define CKMAC_MODE_IAM        (1 << 4)     //Individual address mode
#define CKMAC_MODE_BRO        (1 << 3)     //Broadcast address
#define CKMAC_MODE_NOPRE      (1 << 2)     //No preamble
#define CKMAC_MODE_TXEN       (1 << 1)     //Transmit enable
#define CKMAC_MODE_RXEN       (0x01)       //Receive enable
//Interrupt source register
#define CKMAC_INT_BER         (1 << 7)     //Bus error
#define CKMAC_INT_RXC         (1 << 6)     //Receive control frame
#define CKMAC_INT_TXC         (1 << 5)     //Transmit control frame
#define CKMAC_INT_BUSY        (1 << 4)     //Busy
#define CKMAC_INT_RXE         (1 << 3)     //Receive error
#define CKMAC_INT_RXB         0x04         //Receive frame
#define CKMAC_INT_TXE         0x02         //Transmit error
#define CKMAC_INT_TXB         0x01         //Transmit frame
//Interrupt mask register
#define CKMAC_INTERRUPT_MASK  0xff
#define CKMAC_IMASK_BER         (1 << 7)   //Bus error interrupt mask
#define CKMAC_IMASK_RXC         (1 << 6)   //Receive control frame interrupt mask
#define CKMAC_IMASK_TXC         (1 << 5)   //Transmit control frame interrupt mask
#define CKMAC_IMASK_BUSY        (1 << 4)   //Busy interrupt mask
#define CKMAC_IMASK_RXE         (1 << 3)   //Receive error interrupt mask
#define CKMAC_IMASK_RXB         0x04       //Receive frame interrupt mask
#define CKMAC_IMASK_TXE         0x02       //Transmit error interrupt mask
#define CKMAC_IMASK_TXB         0x01       //Transmit frame interrupt mask
//MII command register
#define CKMAC_MIICMD_WCTRLDATA  0x04       //Write control data
#define CKMAC_MIICMD_RSTAT      0x02       //Read staus
#define CKMAC_MIICMD_SCANSTAT   0x01       //Scan status
//MII address register
//MII status register (read only)
#define CKMAC_MIISTS_NVALID     0x04       // Mii status register invalid ( = 0, valid; = 1, invalid)
#define CKMAC_MIISTS_BUSY       0x02       // Mii busy ( = 0, ready; = 1, busy)
#define CKMAC_MIISTS_LINKFAIL   0x01       // Link fail ( = 0, ok; = 1, fail)

/////////////////////////////////////////////////////////////////////////////
#define PHYID1		0x02	// phy identifier register1 (read only)
#define PHYID2		0x03	// phy identifier register2 (read only)
#ifdef CYGPKG_HAL_CK

#define CHIPID  			0x12
#endif

#ifdef CYGPKG_HAL_CKMMU

#define CHIPID  			0x12
#endif

#ifdef CYGPKG_HAL_ARM
#define CHIPID  			0x01

#ifndef PHY_RTL8201EL
#define PHY_RTL8201EL
#endif

#endif

#ifndef BOOL
typedef unsigned int BOOL;
#endif

#ifndef FALSE
#define FALSE 0
#define TRUE  1
#endif

#define RTL8201CL_PHYID1		0x0
#define RTL8201CL_PHYID2		0x8201
#define DM9161_PHYID1			0x0180
#define DM9161_PHYID2			0xb8a0

#ifndef PhyStatus_LinkUp
#define PhyStatus_LinkUp      0x0001
#endif
#ifndef PhyStatus_FullDuplex
#define PhyStatus_FullDuplex  0x0002
#endif
#ifndef PhyStatus_100Mb
#define PhyStatus_100Mb       0x0004
#endif	
#ifndef PhyStatus_10Mb
#define PhyStatus_10Mb		  0x0008	
#endif 
#ifndef PhyAutoNego_Failed	  
#define PhyAutoNego_Failed	  0x0010
#endif

#define RTL8201_TEST_LINK_10		(1 << 1)
#define RTL8201_TEST_LINK_100		(0x01)
//#define CHIPID     CYGPKG_DEVS_ETH_CKCORE_CHIPID
//RTL8201BL physical device register
#define RTL8201_MODECTRL	0x00	// Mode control register
#define RTL8201_MODESTS		0x01	// Mode status register (read only)
#define RTL8201_PHYID1		0x02	// phy identifier register1 (read only)
#define RTL8201_PHYID2		0x03	// phy identifier register2 (read only)
#define RTL8201_ANAR		0x04	// Auto-negotiation advertisement register
#define RTL8201_ANLPAR		0x05	// Auto-negotiation link partner ability register (read only)
#define RTL8201_ANER		0x06	// Auto-negotiation expansion register (read only)
#define	RTL8201_NSR			0x16	// Nway setup register
#define RTL8201_LBREMR		0x17	// Loopback, Bypass, Receiver error mask register
#define RTL8201_REC			0x18	// Rx_er counter
#define RTL8201_10MNICR		0x19	// 10Mbps Network Interface Configuration Register
#define RTL8201_PHY1_1		0x20	// PHY 1_1 Register
#define RTL8201_PHY1_2		0x21	// PHY 1_2 Register
#define RTL8201_PHY2		0x22	// PHY 2 register
#define RTL8201_TWISTER1	0x23	// Twister_1 register
#define RTL8201_TWISTER_2	0x24	// Twister_2 register
#define RTL8201_TEST		0x25	// test register (Read only)
/////////////////////////////////////////////////////////////////////////////
//RTL8201 mode cnotrol register
#define RTL8201_MODECTRL_RESET		(1 << 15)	// reset
#define RTL8201_MODECTRL_LOOPBACK	(1 << 14)	// Loopback
#define RTL8201_MODECTRL_SPD100		(1 << 13)	// Sets the network speed, if auto-negotiation is disable
#define RTL8201_MODECTRL_ANE		(1 << 12)	// Nway Auto-Negotiation Enable
#define RTL8201_MODECTRL_POWERDOWN	(1 << 11)	// Turn down the power of the phy chip
#define RTL8201_MODECTRL_RESTARTAN	(1 << 9)	// re-start auto-negotiation
#define RTL8201_MODECTRL_DUPLEX		(1 << 8)	// set the duplex mode if auto negotiation is disable
//RTL8201 mode status register
#define RTL8201_MODESTS_100BT4		(1 << 15)	// enable 100base-t4 support
#define RTL8201_MODESTS_100BTXFD	(1 << 14)	// enable 100base-tx full duplex support
#define RTL8201_MODESTS_100BTXHD	(1 << 13)	// enable 100base-tx half duplex support
#define RTL8201_MODESTS_10BTFD		(1 << 12)	// enable 10base-t full duplex support
#define RTL8201_MODESTS_10BTHD		(1 << 11)	// enable 10base-t half duplex support
#define RTL8201_MODESTS_MFPS		(1 << 6)	// the RTL8201BL will accept management frames with preamble supporessed
#define RTL8201_MODESTS_ANC			(1 << 5)	// Auto-negotiation process completed
#define RTL8201_MODESTS_AN			(1 << 4)	// Auto-negotiation link had not been experienced fail state
#define RTL8201_MODESTS_LINKSTS		(1 << 3)	// Valid link established
#define RTL8201_MODESTS_JD			(1 << 2)	// Jabber condition detected
#define RTL8201_MODESTS_EC			(0x01)		// extended register capability
/*      Define the PHY addresss.  This address specifies which 32  attached */
/* PHY devices.                                                             */

#define CKCORE_PHY_ADD            	(0 << 23)


/*      Start of frame delimter for valid MII management frame.             */

#define CKCORE_ETH_MII_ST          (0x40000000)

/*      Operation code.  This field bust be programmed to generate a  valid */
/* MII management frame.                                                    */

#define CKCORE_ETH_MII_OP_READ     (0x20000000)
#define CKCORE_ETH_MII_OP_WRITE    (0x10000000)

/*      Register address.  Specifies one of the 32 attached PHY devices.    */

#define CKCORE_ETH_MII_RA_POS      (18)
#define CKCORE_ETH_MII_RA_MASK     (0x1F)

/*      Turn around.  Must  be programmed  to 10  to generate  a valid  MII */
/* management frame.                                                        */

#define CKCORE_ETH_MII_TA          (0x00020000)


/*      The management frame data maske.  Field  for data to be written  to */
/* or read from PHY register.                                               */

#define CKCORE_ETH_MII_DATA_MASK   (0x0000FFFF)


/*      This define the MII unchanged MII frame management bits.            */

#define CKCORE_ETH_MII_FIX_HDR     (0 |                  \
                                     CKCORE_ETH_MII_ST | \
                                     MCF5272_PHY_ADD |    \
                                     CKCORE_ETH_MII_TA)


/*      The PHY address mask.                                               */

#define CKCORE_ETH_MII_PA          (0x1F << 23)

/*      Define the interrupt mask.                                          */

#define CKCORE_ETH_INTERRUPT_MASK (CKCORE_ETH_IMR_TXFEN    | \
                                    CKCORE_ETH_IMR_RXFEN    )


/**************************************************
 * FEC diagnostic counters
 **************************************************/
typedef struct CKCORE_ETH_DIAG
{

    /*   We put all the receive statistics first.                           */

    unsigned long     rx_pk_cnt;             /*   The   total   number   of */
                                             /* received packets            */

    unsigned long     rx_pk_cnt_sec;

    unsigned long     rx_bytes_cnt;          /*   The   total   number   of */
                                             /* received bytes              */

    unsigned long     rx_bytes_cnt_sec;
											 
    unsigned long     rx_control_frame;            /*   The total  number of  control */
						   /*	 frame received             */
											 
    unsigned long     rx_busy_cnt;        /*   The total  times of busy status occured */

    unsigned long     rx_err_cnt;          /*   The total  number of  bad */
                                             /* packets received            */
											 
    unsigned long     rx_long_frm_err_cnt;   /*   The total number of  long */
                                             /* frame errors.               */

    unsigned long     rx_short_frm_err_cnt;  /*   The total number of short */
                                             /* frame errors.               */

    unsigned long     rx_crc_err_cnt;        /*   The total  number of  CRC */
                                             /* errors.                     */

    unsigned long     rx_overrun_err_cnt;    /*   The   total   number   of */
                                             /* overrun errors.             */

    unsigned long     rx_trunc_error_cnt;    /*   The  total   numbers   of */
                                             /* receieve  truncated  packet */
                                             /* errors.                     */

    /*   We  put  all  the  transmit  statistics  next.   start_of_transmit */
    /* doesn't take any  space but  only separates  the receive  statistics */
    /* from the transmit statistics.                                        */

    char              start_of_transmit[0];

    unsigned long     tx_pk_cnt;          /*   The    total    number    of */
                                          /* transmitted packet             */

    unsigned long     tx_pk_cnt_sec;

    unsigned long     tx_bytes_cnt;       /*   The    total    number    of */
                                          /* transmitted bytes              */

    unsigned long     tx_bytes_cnt_sec;
										  
    unsigned long     tx_control_frame;            /*   The total  number of  control */
						   /*	 frame transmitted            */
						   
    unsigned long     tx_err_cnt;         /*   The total  number of  failed */
                                          /* packet transmission            */

    unsigned long     tx_def_cnt;         /*   The total number of  tansmit */
                                          /* defers.                        */

    unsigned long     tx_hb_err_cnt;      /*   The total  number  of  heart */
                                          /* beat errors.                   */

    unsigned long     tx_late_col_cnt;    /*   The  total  number  of  late */
                                          /* collisions.                    */

    unsigned long     tx_exes_retry_cnt;  /*   Excessive rettry count.      */

    unsigned long     tx_retry_cnt;       /*   The total number of transmit */
                                          /* retries.                       */

    unsigned long     tx_underrun_cnt;    /*   The total number of transmit */
                                          /* underruns.                     */

    unsigned long     tx_carrrier_loss_cnt; /*   The   total   number    of */
                                            /* trasnmit carrier losses.     */

    unsigned long     tx_free_bd_cnt;     /*   The total  number  of  freed */
                                          /* buffer descriptor.             */

    unsigned long     tx_free_min_bd_cnt; /*   The minimum  number of  free */
                                          /* buffer descriptor count.       */

    unsigned long     tx_full_cnt;        /*   The  number  of  times  when */
                                          /* there is no transmit buffer.   */

    unsigned long     tx_not_complete_cnt;  /*   The number  of  times  the */
                                            /* device   driver   discovered */
                                            /* that the BD is still in  use */
                                            /* by the FEC.                  */

    unsigned long     internal_bus_error_cnt; /*   MAC bus error count.   A */
                                              /* bus  error  occurred  when */
                                              /* the MAC  was accessing  an */
                                              /* internal bus.              */

}CKCORE_ETH_DIAG __attribute__ ((aligned));

/* Ethernet driver status. */
enum eth_drv_status_t
{
    ETH_DEV_UNKNOWN = 1,
    ETH_DEV_DOWN = 2,
    ETH_DEV_UP = 3
};

/* Ethernet duplex mode. */
enum eth_drv_mode_t
{
    ETH_MODE_UNKNOWN = 1,
    ETH_MODE_SIMPLEX = 2,
    ETH_MODE_DUPLEX = 3
};


/* Ethernet speed values. */
enum eth_speed_t
{
    ETH_SPEED_10MB  =   10*1000*1000,
    ETH_SPEED_100MB =  100*1000*1000
};

/* Ethernet driver statistics information structure. */

struct ckcore_eth_drv_stats
{
    struct ifreq ifreq;                 // tell ioctl() which interface.

    char description[ DESC_LEN ];       // Textual description of hardware
    unsigned char snmp_chipset[ SNMP_CHIPSET_LEN ];
                                        // SNMP ID of chipset
    enum eth_drv_mode_t  duplex;               // 1 = UNKNOWN, 2 = SIMPLEX, 3 = DUPLEX
    enum eth_drv_status_t operational;          // 1 = UNKNOWN, 2 = DOWN, 3 = UP
    // These are general status information:
    unsigned int speed;                 // 10,000,000 or 100,000,000
                                       //     to infinity and beyond?

    CKCORE_ETH_DIAG stats;

}__attribute__ ((aligned, packed));

#ifndef FIELD_OFFSET
#define FIELD_OFFSET(type,field) (cyg_uint32)(&(((type*)0)->field)
#endif /* FIELD_OFFSET */

/* The value of 1 second in nanosecond. */
#define SEC_IN_NS 600000000    //modified by xiayu

/* 48-bit Ethernet Addresses */
typedef uint8 ETH_ADDR[6];

/* 16-bit Ethernet Frame Type, ie. Protocol */
typedef uint16 ETH_FTYPE;

/* Maximum and Minimum Ethernet Frame Size (Data Field) */
#define ETH_DATA_MAX_SIZE	(1500)
#define ETH_DATA_MIN_SIZE	(46)

/* Maximum and Minimum Ethernet Frame Size (Entire frame) */
#define ETH_MAX_SIZE	(ETH_DATA_MAX_SIZE+14)
#define ETH_MIN_SIZE	(ETH_DATA_MIN_SIZE+14)

/* Common Ethernet Frame definition */
typedef struct
{
	ETH_ADDR dest;
	ETH_ADDR src;
	ETH_FTYPE type;
	uint8 data[ETH_DATA_MAX_SIZE];
	uint32 fcs;
} eth_frame_hdr  __attribute__ ((aligned));

/* 802.1Q Ethernet Frame definition */
typedef struct
{
	ETH_ADDR dest;
	ETH_ADDR src;
	uint32 header_802_1q;
	ETH_FTYPE type;
	uint8 data[ETH_DATA_MAX_SIZE];
	uint32 fcs;
} eth_802_1Q_frame_hdr  __attribute__ ((aligned));

/* Definition of macros that access the FEC registers */

#define put_reg(_addr_,_value_) \
   CKMAC_REG_BASEADDR[_addr_] = (cyg_uint32)(_value_)

#define get_reg(_addr_) \
   (cyg_uint32)(CKMAC_REG_BASEADDR[_addr_])
   
#define Reg32(n)		*((volatile uint32 *)(n))
#define Reg16(n)		*((volatile uint16 *)(n))
#define Reg8(n)			*((volatile uint8  *)(n))

#endif /* _IF_CKCORE_ETH */



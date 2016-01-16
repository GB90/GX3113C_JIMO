#ifndef _NBUF_H
#define _NBUF_H
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

/*
 * File:		nbuf.h
 * Purpose:		Definitions for Network Buffer Allocation.
 *
 * Notes:		These routines implement a static buffer scheme.
 *				The buffer descriptors are as specified by the
 *				MPC860T/MCF5272 FEC.
 *
 * Modifications:
 *
 */

#include <cyg/hal/drv_api.h>
#include <cyg/hal/hal_intr.h>
#include <cyg/infra/cyg_type.h>
#include <cyg/infra/cyg_ass.h>
#include <cyg/io/eth/eth_drv.h>
//#include <pkgconf/devs_eth_ckcore.h>

/********************************************************************/


typedef unsigned char		uint8;  /*  8 bits */
typedef unsigned short int	uint16; /* 16 bits */
typedef unsigned long int	uint32; /* 32 bits */

typedef signed char			int8;   /*  8 bits */
typedef signed short int	int16;  /* 16 bits */
typedef signed long int		int32;  /* 32 bits */


#define Rx	1
#define Tx	0

/*
 * Buffer sizes in bytes -- The following values were chosen based
 * on TFTP maximum packet sizes.  These sizes may need to be
 * increased to implement other protocols.
 */
 //the value is default
#define BUFFER_SIZE 1536  /* mac default value, 0x600 */

/* The this label is not defined, we define it and default it to 256. */
#define CYGPKG_DEVS_ETH_CKCORE_TX_NUM_BDS 0x40
//#endif


/* Number of Receive and Transmit Buffers and Buffer Descriptors */
#define BD_NUMS	   0x80
#define NUM_TXBDS (CYGPKG_DEVS_ETH_CKCORE_TX_NUM_BDS)
#define NUM_RXBDS (BD_NUMS - CYGPKG_DEVS_ETH_CKCORE_TX_NUM_BDS)


/*
 * Buffer Descriptor Format -- must be aligned on 4-byte boundary
 * but a 16-byte boundary is recommended.  However, we cannot pack
 * them on 16-byte boundaries as this will add 8-bytes of padding
 * between structures and the FEC of the MPC860T/MCF5272 will choke.
 */
typedef struct NBUF
{
	volatile uint32 status;	/* control and status and buffer length*/
	volatile uint8  *data;	       /* buffer address */
} __attribute__ ((packed, aligned))NBUF;


/* Defines the tx key type. */
typedef enum tx_key_type_t
{
    TX_KEY_ECOS, /* eCos key */
    TX_KEY_USER  /* user key */
}tx_key_type_t;

typedef struct buf_info_t
{

    /*   Buffer descriptor indexes, the index is only change by hardware, */
    /*   so when reset mbuf, do not change these	*/

    uint32 iTxbd, iTxbd_old;
    uint32 iRxbd;

    /*   Number transmit descriptor buffers busy.                           */

    volatile int  num_busy_bd;

    /*   The maximum number of buffer decriptor used.                       */

    volatile int  max_num_busy_bd;

    /* Buffer Descriptors */
    NBUF  *ntxbuf[NUM_TXBDS];
    NBUF  *nrxbuf[NUM_RXBDS];

    volatile NBUF* RxNBUF;
    volatile NBUF* TxNBUF;

    /* Buffers */
    //uint8 TxBuffer[NUM_TXBDS][BUFFER_SIZE];//20071114 by xuyq
    //uint8 RxBuffer[NUM_RXBDS][BUFFER_SIZE];
}buf_info_t;


/////////////////////////////////////////////////////////////////////////////
// status bit of transmit decriptor 
//

#define TXD_LENGTH   (0xffff)
#define TX_RD_BIT    (1 << 15)
#define TX_IRQ_BIT   (1 << 14)
#define TX_WR_BIT    (1 << 13)
#define TX_PAD_BIT   (1 << 12)
#define TX_CRC_BIT   (1 << 11)
#define TX_UR_BIT    (1 << 8)
#define TX_RTY_BIT   (0x0f << 4)
#define TX_RL_BIT    (1 << 3)
#define TX_LC_BIT    (1 << 2)
#define TX_DF_BIT    (1 << 1)
#define TX_CS_BIT    (0x01)


/////////////////////////////////////////////////////////////////////////////
// status bit of receive decriptor 
//
#define RXD_LENGTH   (0xffff0000)
#define RX_ETY_BIT   (1 << 15)
#define RX_IRQ_BIT   (1 << 14)
#define RX_WR_BIT    (1 << 13)
#define RX_CF_BIT    (1 << 8)
#define RX_MISS_BIT  (1 << 7)
#define RX_OR_BIT    (1 << 6)
#define RX_IS_BIT    (1 << 5)
#define RX_DN_BIT    (1 << 4)
#define RX_TL_BIT    (1 << 3)
#define RX_SF_BIT    (1 << 2)
#define RX_CRC_BIT   (1 << 1)
#define RX_LC_BIT    (0x01)

/*******************************************************************/

/*
 * Functions to manipulate the network buffers.
 */

void nbuf_init (buf_info_t* pBuf);


/********************************************************************/
inline static
uint32
nbuf_get_start(buf_info_t* pBuf, uint8 direction)
{
	/*
	 * Return the address of the first buffer descriptor in the ring.
	 * This routine is needed by the FEC of the MPC860T and MCF5272
	 * in order to write the Rx/Tx descriptor ring start registers
	 */
    switch (direction){
        case Rx:
            return (uint32)pBuf->RxNBUF;
        default:
        case Tx:
            return (uint32)pBuf->TxNBUF;
    }
}






/******************************************************************************
 nbuf_rx_get_next() - Retrieve the next  receive buffer descriptor.

 INPUT:
    pBuf - Pointer to the buffer info.

RETURN:
    Returns the pointer to the next receive buffer descriptor.
*/

inline static volatile NBUF *
nbuf_rx_get_next(buf_info_t* pBuf)
{
    volatile NBUF *pBd = &pBuf->RxNBUF[pBuf->iRxbd];

    /* Check to see if the ring of BDs is full */
    if (pBd->status & RX_ETY_BIT)
    {
        return NULL;
    }

    /* increment the circular index */
    pBuf->iRxbd = (pBuf->iRxbd + 1) % (BD_NUMS - NUM_TXBDS);
    return pBd;
}


/* Return the pointer to the buffer descriptor based on the index */
inline static volatile NBUF*
nbuf_rx_get(buf_info_t* pBuf, uint32 index)
{
    return(&pBuf->RxNBUF[index]);
}

/********************************************************************
  Release the buffer descrip so that it can be use
  to receive the enxt packet.

  INPUT:
    pNbuf - pointer to the buffer descriptor.
*/

inline static void
nbuf_rx_release (volatile NBUF* pNbuf)
{
    /* Mark the buffer as empty and not in use */
    pNbuf->status |= RX_ETY_BIT;

}


/* Return the current rx buffer descriptor index */
inline static uint32
nbuf_rx_get_index(buf_info_t* pBuf)
{
    return  pBuf->iRxbd;
}

/****************************************************************
 This function checks the EMPTY bit of the next Rx buffer to be
 allocated. If the EMPTY bit is cleared, then the next buffer in
 the ring has been filled by the MAC and has not already been
 allocated and passed up the stack. In this case, the next buffer
 in the ring is ready to be allocated. Otherwise, the  buffer is
 either empty or not empty but still in use by a higher level
 protocol. The MAC receive routine uses this function to determine
 if multiple buffers where filled by the FEC during a single
 interrupt event.
 ****************************************************************/
inline static
uint32
nbuf_rx_next_ready(buf_info_t* pBuf)
{
    return ( !(pBuf->RxNBUF[pBuf->iRxbd].status & RX_ETY_BIT));
}


/******************************************************************************
  nbuf_tx_release() - Set the buffer descriptor ready for use to transmit
                      the next packet.

   INPUT:
        pNbuf - Pointer to the transmit buffer descriptor.
*/
inline static
void
nbuf_tx_release (volatile NBUF* pNbuf)
{

    /*   Clear  the  TX_BD_INUSE  to  indicate   that  we  have  read   the */
    /* transmitted buffer.                                                  */
 
    pNbuf->status &= ~TX_RD_BIT;

}

/* Return a nozero value  if the buffer is full. Otherwise, it returns a
   zero value.
*/
inline static
int nbuf_tx_full(buf_info_t* pBuf)
{
    return (pBuf->TxNBUF[pBuf->iTxbd].status & TX_RD_BIT);
}


/* Return the pointer to the transmit buffer descriptor. */
inline static volatile NBUF*
nbuf_tx_get(buf_info_t* pBuf, int index)
{
    return(&pBuf->TxNBUF[index]);
}


/*******************************************************************************
 nbuf_tx_dump() - Dump the transmit buffer information.

 INPUT:
    pBuf - pointer to the buffer info.
*/
inline static
void nbuf_tx_dump(buf_info_t* pBuf)
{
 
    diag_printf("Current index to ring buffer: %ud\n", (unsigned int)pBuf->iTxbd);
	
    diag_printf("Address to the BD:            %08X\n",
                 (unsigned int)&pBuf->TxNBUF[pBuf->iTxbd]);
 
    diag_printf("BD status:            %04X\n", (unsigned int)pBuf->TxNBUF[pBuf->iTxbd].status);
	
    diag_printf("Number of busy BDs:   %d\n", pBuf->num_busy_bd);
 
    diag_printf("Dump Transmit Queue\n");
    diag_printf("===================\n");

}


/********************************************************************
 nbuf_tx_allocate() - Alocate transmit buffer descriptor.

 INPUT:
    pBuf - pointer to the buffer info.

 OUTPUT:
    index - index to the buffer descriptor ring buffer that it returns. This
            value is invalid if this funtion returns a NULL.

 RETURN:
    Returns the pointer to the buffer descriptor. If the ring buffer
    descriptor is full, it returns NULL.

 */
inline static volatile NBUF *
nbuf_tx_allocate (buf_info_t* pBuf)
{
    volatile NBUF* pBd = &pBuf->TxNBUF[pBuf->iTxbd];

    /*   Make sure that the  buffer descriptor is still  not in use by  the */
    /* FEC .                                                                */

    CYG_ASSERT(!(pBd->status & TX_RD_BIT),
               "Buffer descriptor allocated still in use");

    /* increment the circular index */
    pBuf->iTxbd = ((pBuf->iTxbd + 1) % NUM_TXBDS);
    return pBd;
}




/*****************************************************************************/

#endif 	/* _NBUF_H */

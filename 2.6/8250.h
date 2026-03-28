/*
 *  linux/drivers/char/8250.h
 *
 *  Driver for 8250/16550-type serial ports
 *
 *  Based on drivers/char/serial.c, by Linus Torvalds, Theodore Ts'o.
 *
 *  Copyright (C) 2001 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 *  $Id: 8250.h,v 1.8 2002/07/21 21:32:30 rmk Exp $
 */

//lipeng add at 2006/06/08
#include <linux/version.h>

#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a,b,c)	((a)*65536+(b)*256+(c))
#endif
//lipeng add end

//lipeng modify at 2007/09/07
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 18)
#include <linux/config.h>
#endif
//lipeng modify end

//jinxin added to support DMA
#include <linux/pci.h>
#define TX_BUF_TOT_LEN 128
#define RX_BUF_TOT_LEN 128

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

//DMA registers
#define DMAADL (0x100 + 0x00)		//DMA Address Low
#define DMAADH (0x100 + 0x04)		//DMA Address High
#define DMATL  (0x100 + 0x08)		//DMA Transfer Length
#define DMASTA (0x100 + 0x0c)  		//DMA Status

//DMA Status
#define DMAREAD	(1UL <<  31)
#define DMAACT	(1UL << 0)
#define DMAERR	(1UL << 1)
#define DMADONE	(1UL << 2)
//jinxin added to support dtrdsr flow control
#ifndef CDTRDSR
#define CDTRDSR	  004000000000  /* DTR/DSR flow control */
#endif

#define 	ADVANTECH_EXAR_MAIGC            'x'
#define		ADV_READ_REG      				_IO(ADVANTECH_EXAR_MAIGC, 1)
#define 	ADV_WRITE_REG     				_IO(ADVANTECH_EXAR_MAIGC, 2)
#define 	ADV_GET_SERIAL_MODE				_IO(ADVANTECH_EXAR_MAIGC, 3)
#define 	ADV_SET_TURN_AROUND_DELAY		_IO(ADVANTECH_EXAR_MAIGC, 4)

/*
 * The special register set for XR17v25x UARTs.
 */

#define XR_17V25X_EXTENDED_FCTR		8
#define XR_17V25X_EXTENDED_EFR		9
#define XR_17V25X_TXFIFO_CNT		10
#define XR_17V25X_RXFIFO_CNT		11
#define XR_17V25X_EXTENDED_RXTRG	11

#define XR_17V25X_FCTR_RTS_8DELAY	0x03
#define XR_17V25X_FCTR_TRGD		192

/* 17V15X TX/RX memory mapped buffer offsets */

#define UART_17V25X_RX_OFFSET		0x100
#define UART_17V25X_TX_OFFSET 		0x100

/*
 * These are the EXTENDED definitions for the 17V25X's Interrupt
 * Enable Register
 */
#define	XR_17V25X_IER_RTSDTR	0x40
#define XR_17V25X_IER_CTSDSR	0x80


/*
 * The special register set for XR17V35x UARTs.
 */
#define XR_17v35x_UART_RHR			0 
#define	XR_17V35X_UART_DLD          2
#define	XR_17V35X_UART_MSR          6
#define XR_17V35X_EXTENDED_FCTR		8
#define XR_17V35X_EXTENDED_EFR		9
#define XR_17V35X_TXFIFO_CNT		10
#define XR_17V35X_EXTENDED_TXTRG	10
#define XR_17V35X_RXFIFO_CNT		11
#define XR_17V35X_EXTENDED_RXTRG	11
#define XR_17V35X_UART_XOFF2        13 

#define XR_17V35X_XOFF1				0xc
#define XR_17V35X_XOFF2				0xd
#define XR_17V35X_XON1				0xe
#define XR_17V35X_XON2				0xf

#define XR_17V35X_UART_XOFF1 		0xC0
#define XR_17V35X_UART_XON1		    0xE0

// Set this parameter to 1 to enabled internal loopback
#define ENABLE_INTERNAL_LOOPBACK      0


#define XR_17V35X_FCTR_RTS_8DELAY	0x03
#define XR_17V35X_FCTR_TRGD		    192
#define XR_17V35x_FCTR_RS485	    0x20

#define UART_17V35X_RX_OFFSET		0x100
#define UART_17V35X_TX_OFFSET 		0x100

#define	XR_17V35X_IER_RTSDTR	    0x40
#define XR_17V35X_IER_CTSDSR	    0x80

#define XR_17V35X_8XMODE	        0x88
#define XR_17V35X_4XMODE	        0x89

#define XR_17V35X_RESET             0x8A

#define XR_17V35x_MPIOLVL_7_0       0x90
#define XR_17V35x_MPIO3T_7_0        0x91
#define XR_17V35x_MPIOSEL_7_0       0x93
#define XR_17V35x_MPIOLVL_15_8       0x96
#define XR_17V35x_MPIO3T_15_8        0x97
#define XR_17V35x_MPIOSEL_15_8       0x99

#define XR_17V35X_ID             	0x8D

#define XR_17V35X_MPIOLVL_L		    0x90
#define XR_17V35X_MPIOLVL_H		    0x96

#define XR_17V35X_MPIOSEL_L		    0x93
#define XR_17V35X_MPIOSEL_H		    0x99

#define DIVISOR_CHANGED             0


/*
 * serial mode, RS232 RS422 Master  RS485/RS422 Slave
 */
#define SERIAL_MODE_UNKNOWN         0
#define SERIAL_MODE_RS232           1
#define SERIAL_MODE_RS422M          2
#define SERIAL_MODE_RS485ORRS422S   3


//jinxin added end
void adv_serial8250_get_irq_map(unsigned int *map);
void adv_serial8250_suspend_port(int line);
void adv_serial8250_resume_port(int line);

struct uart_8250_port {
	struct uart_port	port;
	struct timer_list	timer;		/* "no irq" timer */
	struct list_head	list;		/* ports on this IRQ */
	unsigned int		capabilities;	/* port capabilities */
	unsigned int		tx_loadsz;	    /* transmit fifo load size */	
	unsigned short		rev;
	unsigned char		acr;
	unsigned char		ier;
	unsigned char		lcr;
	unsigned char		mcr;
	unsigned char		mcr_mask;	/* mask of user bits */
	unsigned char		mcr_force;	/* mask of forced bits */
	unsigned char		lsr_break_flag;
	unsigned char		msr_saved_flags;	
	//jinxin added to support DMA
	dma_addr_t		rx_ring_dma;
	dma_addr_t 		tx_ring_dma;
	unsigned char 		*rx_ring;
	unsigned char 		*tx_ring;
	
	unsigned short 		deviceid;

	/*
	 * We provide a per-port pm hook.
	 */
	void			(*pm)(struct uart_port *port,
				      unsigned int state, unsigned int old);

	int 			    index;		    /* serial index in card, start at 0 */
	int			        serialMode;	    /* RS232 or RS422 or RS485 */	
};

struct old_serial_port {
	unsigned int uart;
	unsigned int baud_base;
	unsigned int port;
	unsigned int irq;
	unsigned int flags;
	unsigned char hub6;
	unsigned char io_type;
	unsigned char *iomem_base;
	unsigned short iomem_reg_shift;
};

#undef SERIAL_DEBUG_PCI

#if defined(__i386__) && (defined(CONFIG_M386) || defined(CONFIG_M486))
#define SERIAL_INLINE
#endif
  
#ifdef SERIAL_INLINE
#define _INLINE_ inline
#else
#define _INLINE_
#endif

#define PROBE_RSA	(1 << 0)
#define PROBE_ANY	(~0)

#define HIGH_BITS_OFFSET ((sizeof(long)-sizeof(int))*8)

#ifdef CONFIG_SERIAL_8250_SHARE_IRQ
#define SERIAL8250_SHARE_IRQS 1
#else
#define SERIAL8250_SHARE_IRQS 0
#endif

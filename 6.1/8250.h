/*
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
 */

#include <linux/version.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 7, 0)
#include <linux/serial_8250.h>
#endif

void adv_uart_write_wakeup(struct uart_port *port);
int adv_uart_resume_port(struct uart_driver *drv, struct uart_port *uport);
void adv_serial8250_resume_port(int line);
int adv_uart_suspend_port(struct uart_driver *drv, struct uart_port *uport);
void adv_serial8250_suspend_port(int line);
void adv_uart_configure_port(struct uart_driver *drv, struct uart_state *state,struct uart_port *port);
int adv_uart_match_port(struct uart_port *port1, struct uart_port *port2);
int adv_serial8250_register_port(struct uart_port *port);
int __init adv_serial8250_init(void);
int adv_uart_register_driver(struct uart_driver *drv);
int adv_uart_remove_one_port(struct uart_driver *drv, struct uart_port *uport);
int adv_uart_register_driver(struct uart_driver *drv);
int adv_uart_add_one_port(struct uart_driver *drv, struct uart_port *uport);
void __exit adv_serial8250_exit(void);
void adv_uart_unregister_driver(struct uart_driver *drv);
void adv_serial8250_unregister_port(int line);
unsigned int adv_uart_get_divisor(struct uart_port *port, unsigned int baud);
void uart_configure_port(struct uart_driver *drv, struct uart_state *state,struct uart_port *port);
void adv_uart_insert_char(struct uart_port *port, unsigned int status,unsigned int overrun, unsigned int ch, unsigned int flag);
void adv_uart_handle_dcd_change(struct uart_port *uport, unsigned int status);
void adv_uart_handle_cts_change(struct uart_port *uport, unsigned int status);
void adv_uart_configure_port(struct uart_driver *drv, struct uart_state *state,struct uart_port *port);



struct uart_8250_port {
	struct uart_port	port;
	struct timer_list	timer;		/* "no irq" timer */
	struct list_head	list;		/* ports on this IRQ */
	unsigned short		capabilities;	/* port capabilities */
	unsigned short		bugs;		/* port bugs */
	unsigned int		tx_loadsz;	/* transmit fifo load size */
	unsigned char		acr;
	unsigned char		ier;
	unsigned char		lcr;
	unsigned char		mcr;
	unsigned char		mcr_mask;	/* mask of user bits */
	unsigned char		mcr_force;	/* mask of forced bits */
	unsigned char		lsr_break_flag;
	
	unsigned char		cur_iotype;	/* Running I/O type */
	dma_addr_t	     	rx_ring_dma;
	dma_addr_t 		tx_ring_dma;
	unsigned char 		*rx_ring;
	unsigned char 		*tx_ring;

	unsigned short 		deviceid;
	int			        serialMode;	    /* RS232 or RS422 or RS485 */	

	/*
	 * Some bits in registers are cleared on a read, so they must
	 * be saved whenever the register is read but the bits will not
	 * be immediately processed.
	 */
#define LSR_SAVE_FLAGS UART_LSR_BRK_ERROR_BITS
	unsigned char		lsr_saved_flags;
#define MSR_SAVE_FLAGS UART_MSR_ANY_DELTA
	unsigned char		msr_saved_flags;

	/* 8250 specific callbacks */
	int			(*dl_read)(struct uart_8250_port *);
	void			(*dl_write)(struct uart_8250_port *, int);


	int 			    index;		    /* serial index in card, start at 0 */

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
	unsigned long irqflags;
};

struct uart_port_ext{
	unsigned int idx;
	unsigned char serial_type;
	unsigned char reuse;
};

/*
 * This replaces serial_uart_config in include/linux/serial.h
 */
struct serial8250_config {
	const char	*name;
	unsigned short	fifo_size;
	unsigned short	tx_loadsz;
	unsigned char	fcr;
	unsigned int	flags;
};

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 7, 0)
unsigned char serial8250_rx_chars(struct uart_8250_port *up, unsigned char lsr);
void serial8250_tx_chars(struct uart_8250_port *up);
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 4, 0)

static inline int serial_port_in(struct uart_port *up, int offset)
{
	return up->serial_in(up, offset);
}

static inline void serial_port_out(struct uart_port *up, int offset, int value)
{
	up->serial_out(up, offset, value);
}

void serial8250_tx_chars(struct uart_8250_port *up);
#define UPF_BUG_THRE		((__force upf_t) (1 << 26))

#endif

#define TX_BUF_TOT_LEN 128
#define RX_BUF_TOT_LEN 128
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

#ifndef CDTRDSR
#define CDTRDSR	  004000000000  /* DTR/DSR flow control */
#endif

#define UART_CAP_FIFO	(1 << 8)	/* UART has FIFO */
#define UART_CAP_EFR	(1 << 9)	/* UART has EFR */
#define UART_CAP_SLEEP	(1 << 10)	/* UART has IER sleep */
#define UART_CAP_AFE	(1 << 11)	/* MCR-based hw flow control */
#define UART_CAP_UUE	(1 << 12)	/* UART needs IER bit 6 set (Xscale) */
#define UART_CAP_RTOIE	(1 << 13)	/* UART needs IER bit 4 set (Xscale, Tegra) */

#define UART_BUG_QUOT	(1 << 0)	/* UART has buggy quot LSB */
#define UART_BUG_TXEN	(1 << 1)	/* UART has buggy TX IIR status */
#define UART_BUG_NOMSR	(1 << 2)	/* UART has buggy MSR status bits (Au1x00) */
#define UART_BUG_THRE	(1 << 3)	/* UART has buggy THRE reassertion */

#define PROBE_RSA	(1 << 0)
#define PROBE_ANY	(~0)

#define HIGH_BITS_OFFSET ((sizeof(long)-sizeof(int))*8)

#ifdef CONFIG_SERIAL_8250_SHARE_IRQ
#define SERIAL8250_SHARE_IRQS 1
#else
#define SERIAL8250_SHARE_IRQS 0
#endif

/*
 * The special register set for XR17v25x UARTs.
 */

#define 	ADVANTECH_EXAR_MAIGC            'x'
#define		ADV_READ_REG      				_IO(ADVANTECH_EXAR_MAIGC, 1)
#define 	ADV_WRITE_REG     				_IO(ADVANTECH_EXAR_MAIGC, 2)
#define 	ADV_GET_SERIAL_MODE				_IO(ADVANTECH_EXAR_MAIGC, 3)
#define 	ADV_SET_TURN_AROUND_DELAY		_IO(ADVANTECH_EXAR_MAIGC, 4)


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

#define XR_17V35X_ID             	0x8D

#define XR_17V35X_MPIOLVL_L		    0x90
#define XR_17V35X_MPIOLVL_H		    0x96

#define XR_17V35X_MPIOSEL_L		    0x93
#define XR_17V35X_MPIOSEL_H		    0x99

#define XR_17V35x_MPIOLVL_7_0       0x90
#define XR_17V35x_MPIO3T_7_0        0x91
#define XR_17V35x_MPIOSEL_7_0       0x93
#define XR_17V35x_MPIOLVL_15_8       0x96
#define XR_17V35x_MPIO3T_15_8        0x97
#define XR_17V35x_MPIOSEL_15_8       0x99

#define DIVISOR_CHANGED             0

/*
 * serial mode, RS232 RS422 Master  RS485/RS422 Slave
 */
#define SERIAL_MODE_UNKNOWN         0
#define SERIAL_MODE_RS232           1
#define SERIAL_MODE_RS422M          2
#define SERIAL_MODE_RS485ORRS422S   3

static inline int serial_in(struct uart_8250_port *up, int offset)
{
	return up->port.serial_in(&up->port, offset);
}

static inline void serial_out(struct uart_8250_port *up, int offset, int value)
{
	up->port.serial_out(&up->port, offset, value);
}

void serial8250_clear_and_reinit_fifos(struct uart_8250_port *p);

static inline int serial_dl_read(struct uart_8250_port *up)
{
	return up->dl_read(up);
}

static inline void serial_dl_write(struct uart_8250_port *up, int value)
{
	up->dl_write(up, value);
}

#if defined(__alpha__) && !defined(CONFIG_PCI)
/*
 * Digital did something really horribly wrong with the OUT1 and OUT2
 * lines on at least some ALPHA's.  The failure mode is that if either
 * is cleared, the machine locks up with endless interrupts.
 */
#define ALPHA_KLUDGE_MCR  (UART_MCR_OUT2 | UART_MCR_OUT1)
#elif defined(CONFIG_SBC8560)
/*
 * WindRiver did something similarly broken on their SBC8560 board. The
 * UART tristates its IRQ output while OUT2 is clear, but they pulled
 * the interrupt line _up_ instead of down, so if we register the IRQ
 * while the UART is in that state, we die in an IRQ storm. */
#define ALPHA_KLUDGE_MCR (UART_MCR_OUT2)
#else
#define ALPHA_KLUDGE_MCR 0
#endif

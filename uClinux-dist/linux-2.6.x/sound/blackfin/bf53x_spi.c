/*
 * File:         bf53x_spi.c 
 * Description:  low level driver for SPI port on blackfin 53x
 *               this should be moved to arch/blackfin/
 * Rev:          $Id: bf53x_spi.c,v 1.10 2005/11/29 09:16:43 royhuang Exp $
 * Created:      Tue Sep 21 10:52:42 CEST 2004
 * Author:       Luuk van Dijk
 * mail:         blackfin@mdnmttr.nl
 * 
 * Copyright (C) 2004 Luuk van Dijk, Mind over Matter B.V.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.
 * If not, write to the Free Software Foundation,
 * 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <linux/interrupt.h>
#include <linux/slab.h>
#include <asm/bug.h>
#include <asm/blackfin.h>

#include "bf53x_spi.h"

#define SSYNC __builtin_bfin_ssync()

/* this should be called by the kernel from the data irq for the SPI.
 * it will handle the callbacks. irq should contain the irq that triggered this, 
 * so this code can determine wether an error or a finish condition occured.
 * this code will determine if the irq was actually generated by the SPI,
 * so it can be called safely in shared irq environments
 */

irqreturn_t bf53x_spi_irq_handler(int irq, void *dev_id, struct pt_regs *regs )
{
	unsigned short status;
	struct bf53x_spi *spi = dev_id;

//	printk(KERN_INFO "%s enter\n", __FUNCTION__);
	spi_get_stat(&status);
	if( status & RXS ){
		spi_disable(&spi->spi_ad1836_dev);	
		spi->rx_data = spi_receive_data();
		if( spi->callback )
			spi->callback( spi, spi->private );
	}
	spi_channel_release(&spi->spi_ad1836_dev);

  	return IRQ_HANDLED;
}


/* initialize the spi port */

/* dma channel, and data and error irq's should match the settings in the SIC_IARx registers */
struct bf53x_spi* bf53x_spi_init(spi_callback callback, void *priv)
{
  struct bf53x_spi* spi = (struct bf53x_spi*) kmalloc( sizeof(struct bf53x_spi) , GFP_KERNEL);

  if( !spi ) return NULL;

  spi->callback = callback;
  spi->private = priv;

  memset(&spi->spi_ad1836_dev, 0, sizeof(spi_device_t)); 
  spi->spi_ad1836_dev.dev_name ="ad1386 spi";
  spi->spi_ad1836_dev.bdrate = 16;
  spi->spi_ad1836_dev.dma = 0;
  spi->spi_ad1836_dev.flag = (1 << CONFIG_SND_BLACKFIN_SPI_PFBIT);
  spi->spi_ad1836_dev.irq_handler = bf53x_spi_irq_handler;
  spi->spi_ad1836_dev.priv_data = spi;
 
  spi->spi_ad1836_dev.size = CFG_SPI_WORDSIZE16;
  spi->spi_ad1836_dev.master = CFG_SPI_MASTER;
  spi->spi_ad1836_dev.enable = CFG_SPI_ENABLE;
  return spi;

}

void bf53x_spi_done(struct bf53x_spi* spi){
  kfree(spi);
  return;
}

/* transmit/receive a single byte or word over a channel */
int bf53x_spi_transceive(struct bf53x_spi* spi, unsigned short data)
{
	unsigned long flags;
	
	local_irq_save(flags);
	spi_channel_request(&spi->spi_ad1836_dev);

	spi_enable(&spi->spi_ad1836_dev);
	spi_send_data(data);
	
	/* Initial the read operation */
	spi_receive_data();
	
	local_irq_restore(flags);

	return 0;
}
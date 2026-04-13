/*
 * (C) Copyright 2010
 * jung hyun kim, Nexell Co, <jhkim@nexell.co.kr>
 * Daniel Lazzari Jr, LeapFrog Inc, <dlazzari@leapfrog.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef __GPIO_H__
#define __GPIO_H__
 
#include <linux/lf2000/gpio.h>
/*------------------------------------------------------------------------------
 * 	data
 */
struct gpio_info{
	unsigned int io;		/* A, B, C, D, E, Alive + io bit */
	unsigned int mode;
	unsigned int value;		/* true, false, ...... */
};

/*------------------------------------------------------------------------------
 * 	ioctl
 */
#include "ioc_magic.h"

enum {
	IOCTL_GPIO_SET_OUTPUT		= _IO(IOC_NX_MAGIC,  1),
	IOCTL_GPIO_GET_OUTPUT		= _IO(IOC_NX_MAGIC,  2),
	IOCTL_GPIO_SET_OUTPUT_EN	= _IO(IOC_NX_MAGIC,  3),
	IOCTL_GPIO_GET_OUTPUT_EN	= _IO(IOC_NX_MAGIC,  4),
	IOCTL_GPIO_SET_DET_MODE		= _IO(IOC_NX_MAGIC,  5),
	IOCTL_GPIO_GET_DET_MODE		= _IO(IOC_NX_MAGIC,  6),
	IOCTL_GPIO_SET_INT_EN		= _IO(IOC_NX_MAGIC,  7),
	IOCTL_GPIO_GET_INT_EN		= _IO(IOC_NX_MAGIC,  8),
	IOCTL_GPIO_CLEAR_INTPENDING	= _IO(IOC_NX_MAGIC,  9),
	IOCTL_GPIO_GET_INTPENDING	= _IO(IOC_NX_MAGIC, 10),
//	IOCTL_GPIO_SET_INPUT_LV		= _IO(IOC_NX_MAGIC, 11),
	IOCTL_GPIO_GET_INPUT_LV		= _IO(IOC_NX_MAGIC, 12),
	IOCTL_GPIO_SET_PULLUP_EN	= _IO(IOC_NX_MAGIC, 13),
	IOCTL_GPIO_GET_PULLUP_EN	= _IO(IOC_NX_MAGIC, 14),
	IOCTL_GPIO_SET_ALT_MODE		= _IO(IOC_NX_MAGIC, 15),
	IOCTL_GPIO_GET_ALT_MODE		= _IO(IOC_NX_MAGIC, 16),
	IOCTL_GPIO_SET_ALV_DET_MODE	= _IO(IOC_NX_MAGIC, 17),
	IOCTL_GPIO_SET_ALV_DET_EN	= _IO(IOC_NX_MAGIC, 18),
};

/*------------------------------------------------------------------------------
 * 	GPIO Framework
 */

int lf2000_gpio_request(unsigned gpio, const char *label);
void lf2000_gpio_free(unsigned gpio);
int lf2000_gpio_irq_request(unsigned gpio, const char *label);
void lf2000_gpio_irq_free(unsigned gpio);
int lf2000_gpio_direction_input(unsigned gpio);
int lf2000_gpio_direction_output(unsigned gpio, int value);
int lf2000_gpio_get_value(unsigned gpio);
void lf2000_gpio_set_value(unsigned gpio, int value);

#define ARCH_NR_GPIOS 0x0FFF

#ifdef CONFIG_GPIOLIB
#include <asm-generic/gpio.h>		/* cansleep wrappers */

static inline int gpio_get_value(unsigned int gpio)
{
	if (gpio < GPIO_NUMBER_VALUES)
		return lf2000_gpio_get_value(gpio);
	else
		return __gpio_get_value(gpio);
}

static inline void gpio_set_value(unsigned int gpio, int value)
{
	if (gpio < GPIO_NUMBER_VALUES)
		lf2000_gpio_set_value(gpio, value);
	else
		__gpio_set_value(gpio, value);
}

static inline int gpio_cansleep(unsigned int gpio)
{
	return __gpio_cansleep(gpio);
}

static inline int gpio_to_irq(unsigned gpio)
{
	return __gpio_to_irq(gpio);
}

#else /* !CONFIG_GPIOLIB */

static inline int gpio_request(unsigned gpio, const char *label)
{
	return lf2000_gpio_request(gpio, label);
}

static inline void gpio_free(unsigned gpio)
{
	return lf2000_gpio_free(gpio);
}

static inline int gpio_direction_input(unsigned gpio)
{
	return lf2000_gpio_direction_input(gpio);
}

static inline int gpio_direction_output(unsigned gpio, int value)
{
	return lf2000_gpio_direction_output(gpio, value);
}

static inline int gpio_set_debounce(unsigned gpio, unsigned debounce)
{
	return -EINVAL;
}

static inline int __gpio_get_value(unsigned gpio)
{
	return lf2000_gpio_get_value(gpio);
}

static inline void __gpio_set_value(unsigned gpio, int value)
{
	return lf2000_gpio_set_value(gpio, value);
}

static inline int gpio_get_value(unsigned gpio)
{
	return __gpio_get_value(gpio);
}

static inline void gpio_set_value(unsigned gpio, int value)
{
	return __gpio_set_value(gpio, value);
}

static inline int gpio_to_irq(unsigned gpio)
{
	if (likely(gpio < GPIO_NUMBER_VALUES))
		return gpio + GPIO_IRQ_BASE;

	return -EINVAL;
}

#include <asm-generic/gpio.h>		/* cansleep wrappers */
#endif	/* !CONFIG_GPIOLIB */

#if 0
//GPIOLIB hook ups
#define gpio_to_irq	__gpio_to_irq
#define gpio_get_value	__gpio_get_value
#endif

#endif // __GPIO_H__

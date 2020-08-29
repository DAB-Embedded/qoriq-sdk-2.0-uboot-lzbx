/*
 * Copyright 2017 DAB-Embedded.
 *
 * SPDX-License-Identifier:      GPL-2.0+
 */

#ifndef __ARCH_LS1012AX_GPIO_H
#define __ARCH_LS1012AX_GPIO_H

#include <asm/io.h>
#include <asm/global_data.h>

/*
 * The implementation
 * is in drivers/gpio/mpc85xx_gpio.c instead of inlined here.
 *
 * Structure to keep compatible with MPC85xx
 */

struct mpc85xx_gpio_plat {
	ulong addr;
	unsigned long size;
	uint ngpios;
};

#endif

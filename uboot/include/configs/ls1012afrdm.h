/*
 * Copyright 2016 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __LS1012ARDB_H__
#define __LS1012ARDB_H__

#include "ls1012a_common.h"

#define CONFIG_FSL_LS_PPA
#if defined(CONFIG_FSL_LS_PPA)
#define CONFIG_ARMV8_SEC_FIRMWARE_SUPPORT
#define SEC_FIRMWARE_ERET_ADDR_REVERT
#define CONFIG_ARMV8_PSCI
#define CONFIG_SYS_LS_PPA_DRAM_BLOCK_MIN_SIZE	(1UL * 1024 * 1024)

#define CONFIG_SYS_LS_PPA_FW_IN_XIP
#ifdef CONFIG_SYS_LS_PPA_FW_IN_XIP
#define CONFIG_SYS_LS_PPA_FW_ADDR	0x40500000
#endif
#endif

#ifdef CONFIG_FSL_PPFE
#define EMAC1_PHY_ADDR          0x0
#define EMAC2_PHY_ADDR          0x1
#define CONFIG_PHYLIB
#define CONFIG_PHY_MARVELL
#endif

/* DDR */
#define CONFIG_DIMM_SLOTS_PER_CTLR	1
#define CONFIG_CHIP_SELECTS_PER_CTRL	1
#define CONFIG_NR_DRAM_BANKS		2
#define CONFIG_SYS_SDRAM_SIZE		0x40000000
#define CONFIG_CMD_MEMINFO
#define CONFIG_CMD_MEMTEST
#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		0x9fffffff

/*
* USB
*/
#define CONFIG_HAS_FSL_XHCI_USB

#ifdef CONFIG_HAS_FSL_XHCI_USB
#define CONFIG_USB_XHCI_FSL
#define CONFIG_USB_MAX_CONTROLLER_COUNT         1
#define CONFIG_SYS_USB_XHCI_MAX_ROOT_PORTS      2
#endif

#define CONFIG_DOS_PARTITION
#define CONFIG_PARTITION_UUIDS
#define CONFIG_EFI_PARTITION
#define CONFIG_CMD_GPT

/*  MMC  */
#define CONFIG_MMC
#ifdef CONFIG_MMC
#define CONFIG_FSL_ESDHC
#define CONFIG_SYS_FSL_MMC_HAS_CAPBLT_VS33
#define CONFIG_GENERIC_MMC
#define CONFIG_DOS_PARTITION
#endif

#define CONFIG_CMD_MEMINFO
#define CONFIG_CMD_MEMTEST
#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		0x9fffffff

#define CONFIG_FSL_CAAM			/* Enable CAAM */

#define CONFIG_BOARD_LATE_INIT
#define CONFIG_QSPI_AHB_INIT
#define CONFIG_SPI_FLASH_ISSI
#undef  CONFIG_SPI_FLASH_BAR

#endif /* __LS1012ARDB_H__ */

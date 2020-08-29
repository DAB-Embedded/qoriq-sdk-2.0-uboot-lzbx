/*
 * Copyright (C) 2015 Freescale Semiconductor
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __LS1043A_COMMON_H
#define __LS1043A_COMMON_H

#define CONFIG_REMAKE_ELF
#define CONFIG_FSL_LAYERSCAPE
#define CONFIG_FSL_LSCH2
#define CONFIG_LS1043A
#define CONFIG_MP
#define CONFIG_SYS_FSL_CLK
#define CONFIG_GICV2

#include <asm/arch/config.h>
#ifndef CONFIG_SPL_BUILD
#ifdef CONFIG_SYS_FSL_SRDS_1
#define	CONFIG_SYS_HAS_SERDES
#endif
#endif

#define CONFIG_HAS_FEATURE_GIC4K_ALIGN
#define CONFIG_HAS_FEATURE_ENHANCED_MSI

/* Link Definitions */
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_FSL_OCRAM_BASE + 0xfff0)

#define CONFIG_SUPPORT_RAW_INITRD

#define CONFIG_SKIP_LOWLEVEL_INIT
#define CONFIG_BOARD_EARLY_INIT_F	1

#ifndef CONFIG_SYS_FSL_DDR4
#define CONFIG_SYS_FSL_DDR3		/* Use DDR3 memory */
#endif

#define CONFIG_VERY_BIG_RAM
#define CONFIG_SYS_DDR_SDRAM_BASE	0x80000000
#define CONFIG_SYS_FSL_DDR_SDRAM_BASE_PHY	0
#define CONFIG_SYS_SDRAM_BASE		CONFIG_SYS_DDR_SDRAM_BASE
#define CONFIG_SYS_DDR_BLOCK2_BASE      0x880000000ULL

#define CPU_RELEASE_ADDR               secondary_boot_func

/* Generic Timer Definitions */
#define COUNTER_FREQUENCY		25000000	/* 25MHz */

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + 1024 * 1024)

/* Serial Port */
#define CONFIG_CONS_INDEX		1
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE	1
#define CONFIG_SYS_NS16550_CLK          (get_serial_clock())

#define CONFIG_BAUDRATE			115200
#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

/* SD boot SPL */
#ifdef CONFIG_SD_BOOT
#define CONFIG_SPL_FRAMEWORK
#define CONFIG_SPL_LDSCRIPT		"arch/arm/cpu/armv8/u-boot-spl.lds"
#define CONFIG_SPL_TARGET		"u-boot-with-spl.bin"
#define CONFIG_SPL_LIBCOMMON_SUPPORT
#define CONFIG_SPL_LIBGENERIC_SUPPORT
#define CONFIG_SPL_ENV_SUPPORT
#define CONFIG_SPL_MPC8XXX_INIT_DDR_SUPPORT
#define CONFIG_SPL_WATCHDOG_SUPPORT
#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SPL_DRIVERS_MISC_SUPPORT
#define CONFIG_SPL_MMC_SUPPORT
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR		0x110

#ifdef CONFIG_SECURE_BOOT
#define CONFIG_U_BOOT_HDR_SIZE				(16 << 10)
/*
 * HDR would be appended at end of image and copied to DDR along
 * with U-Boot image.
 */
#define CONFIG_SYS_U_BOOT_MAX_SIZE_SECTORS		(0x500 + \
		(CONFIG_U_BOOT_HDR_SIZE / 512)
#else
#define CONFIG_SYS_U_BOOT_MAX_SIZE_SECTORS		0x500
#endif /* ifdef CONFIG_SECURE_BOOT */

#define CONFIG_SPL_TEXT_BASE		0x10000000
/*
 * CONFIG_SPL_MAX_SIZE is limited by OCRAM memory size(128 KiB) and
 * a reserved stack size(4 KiB).
 * So notice that even if DEBUG is ON, the SPL image(spl/u-boot-spl.bin)
 * should not be > 124 KiB.
 */
#define CONFIG_SPL_MAX_SIZE		0x20000		/* 124 KiB */
#define CONFIG_SPL_STACK		0x10020000
#define CONFIG_SPL_PAD_TO		0x21000		/* 132 KiB */

#define CONFIG_SYS_SPL_MALLOC_START	(CONFIG_SPL_BSS_START_ADDR + \
					CONFIG_SPL_BSS_MAX_SIZE)
#define CONFIG_SYS_SPL_MALLOC_SIZE	0x100000
#define CONFIG_SPL_BSS_START_ADDR	0x80100000
#define CONFIG_SPL_BSS_MAX_SIZE		0x80000

#ifdef CONFIG_U_BOOT_HDR_SIZE
/*
 * HDR would be appended at end of image and copied to DDR along
 * with U-Boot image. Here u-boot max. size is 512K. So if binary
 * size increases then increase this size in case of secure boot as
 * it uses raw u-boot image instead of fit image.
 */
#define CONFIG_SYS_MONITOR_LEN		(0x100000 + CONFIG_U_BOOT_HDR_SIZE)
#else
#define CONFIG_SYS_MONITOR_LEN		0x100000
#endif /* ifdef CONFIG_U_BOOT_HDR_SIZE */
#endif

/* NAND SPL */
#ifdef CONFIG_NAND_BOOT
#define CONFIG_SPL_PBL_PAD
#define CONFIG_SPL_FRAMEWORK
#define CONFIG_SPL_LDSCRIPT		"arch/arm/cpu/armv8/u-boot-spl.lds"
#define CONFIG_SPL_TARGET		"u-boot-with-spl.bin"
#define CONFIG_SPL_LIBCOMMON_SUPPORT
#define CONFIG_SPL_LIBGENERIC_SUPPORT
#define CONFIG_SPL_ENV_SUPPORT
#define CONFIG_SPL_WATCHDOG_SUPPORT
#define CONFIG_SPL_I2C_SUPPORT
#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SPL_MPC8XXX_INIT_DDR_SUPPORT
#define CONFIG_SPL_NAND_SUPPORT
#define CONFIG_SPL_DRIVERS_MISC_SUPPORT
#define CONFIG_SPL_TEXT_BASE		0x10000000
#define CONFIG_SPL_MAX_SIZE		0x1d000		/* 116 KiB */
#define CONFIG_SPL_STACK		0x1001f000
#define CONFIG_SYS_NAND_U_BOOT_DST	CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_NAND_U_BOOT_START	CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_SPL_MALLOC_START	(CONFIG_SPL_BSS_START_ADDR + \
					CONFIG_SPL_BSS_MAX_SIZE)
#define CONFIG_SPL_BSS_START_ADDR	0x80100000
#define CONFIG_SYS_SPL_MALLOC_SIZE	0x100000
#define CONFIG_SPL_BSS_MAX_SIZE		0x80000

#ifdef CONFIG_SECURE_BOOT
#define CONFIG_U_BOOT_HDR_SIZE				(16 << 10)
#endif /* ifdef CONFIG_SECURE_BOOT */

#ifdef CONFIG_U_BOOT_HDR_SIZE
/*
 * HDR would be appended at end of image and copied to DDR along
 * with U-Boot image. Here u-boot max. size is 512K. So if binary
 * size increases then increase this size in case of secure boot as
 * it uses raw u-boot image instead of fit image.
 */
#define CONFIG_SYS_MONITOR_LEN		(0x100000 + CONFIG_U_BOOT_HDR_SIZE)
#else
#define CONFIG_SYS_MONITOR_LEN		0x100000
#endif /* ifdef CONFIG_U_BOOT_HDR_SIZE */

#endif

#if !(defined(CONFIG_SPL_BUILD) && defined(CONFIG_SD_BOOT))
/* IFC */
#if !defined(CONFIG_QSPI_BOOT) && !defined(CONFIG_SD_BOOT_QSPI)
#define CONFIG_FSL_IFC
/*
 * CONFIG_SYS_FLASH_BASE has the final address (core view)
 * CONFIG_SYS_FLASH_BASE_PHYS has the final address (IFC view)
 * CONFIG_SYS_FLASH_BASE_PHYS_EARLY has the temporary IFC address
 * CONFIG_SYS_TEXT_BASE is linked to 0x60000000 for booting
 */
#define CONFIG_SYS_FLASH_BASE			0x60000000
#define CONFIG_SYS_FLASH_BASE_PHYS		CONFIG_SYS_FLASH_BASE
#define CONFIG_SYS_FLASH_BASE_PHYS_EARLY	0x00000000

#ifndef CONFIG_SYS_NO_FLASH
#define CONFIG_FLASH_CFI_DRIVER
#define CONFIG_SYS_FLASH_CFI
#define CONFIG_SYS_FLASH_USE_BUFFER_WRITE
#define CONFIG_SYS_FLASH_QUIET_TEST
#define CONFIG_FLASH_SHOW_PROGRESS	45	/* count down from 45/5: 9..1 */
#endif
#endif
#endif

/* I2C */
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_MXC_I2C1
#define CONFIG_SYS_I2C_MXC_I2C2
#define CONFIG_SYS_I2C_MXC_I2C3
#define CONFIG_SYS_I2C_MXC_I2C4

#ifndef CONFIG_SPL_BUILD
/* PCIe */
#define FSL_PCIE_COMPAT "fsl,ls1043a-pcie"
#ifdef CONFIG_PCI
#define CONFIG_PCI_PNP
#define CONFIG_PCI_SCAN_SHOW
#define CONFIG_CMD_PCI
#endif

/* Command line configuration */
#define CONFIG_CMD_ENV
#endif

#if !(defined(CONFIG_SPL_BUILD) && defined(CONFIG_NAND_BOOT))
/*  MMC  */
#define CONFIG_MMC
#ifdef CONFIG_MMC
#define CONFIG_FSL_ESDHC
#define CONFIG_SYS_FSL_MMC_HAS_CAPBLT_VS33
#define CONFIG_GENERIC_MMC
#define CONFIG_DOS_PARTITION
#endif
#endif

#define CONFIG_FSL_CAAM			/* Enable SEC/CAAM */

#ifndef CONFIG_SPL_BUILD
/*  DSPI  */
#define CONFIG_FSL_DSPI
#ifdef CONFIG_FSL_DSPI
#define CONFIG_DM_SPI_FLASH
#define CONFIG_SPI_FLASH_STMICRO	/* cs0 */
#define CONFIG_SPI_FLASH_SST		/* cs1 */
#define CONFIG_SPI_FLASH_EON		/* cs2 */
#if !defined(CONFIG_QSPI_BOOT) && !defined(CONFIG_SD_BOOT_QSPI)
#define CONFIG_SF_DEFAULT_BUS		1
#define CONFIG_SF_DEFAULT_CS		0
#endif
#endif

#define CONFIG_SYS_DPAA_QBMAN		/* Support Q/Bman */

/* FMan ucode */
#define CONFIG_SYS_DPAA_FMAN
#ifdef CONFIG_SYS_DPAA_FMAN
#define CONFIG_SYS_FM_MURAM_SIZE	0x60000

#ifdef CONFIG_NAND_BOOT
/* Store Fman ucode at offeset 0x160000(11 blocks). */
#define CONFIG_SYS_QE_FMAN_FW_IN_NAND
#define CONFIG_SYS_FMAN_FW_ADDR		(11 * CONFIG_SYS_NAND_BLOCK_SIZE)
#elif defined(CONFIG_SD_BOOT)
/*
 * PBL SD boot image should stored at 0x1000(8 blocks), the size of the image is
 * about 1MB (2040 blocks), Env is stored after the image, and the env size is
 * 0x2000 (16 blocks), 8 + 2040 + 16 = 2064, enlarge it to 2080(0x820).
 */
#define CONFIG_SYS_QE_FMAN_FW_IN_MMC
#define CONFIG_SYS_FMAN_FW_ADDR		(512 * 0x820)
#elif defined(CONFIG_QSPI_BOOT)
#define CONFIG_SYS_QE_FW_IN_SPIFLASH
#define CONFIG_SYS_FMAN_FW_ADDR		0x400d0000
#define CONFIG_ENV_SPI_BUS		0
#define CONFIG_ENV_SPI_CS		0
#define CONFIG_ENV_SPI_MAX_HZ		1000000
#define CONFIG_ENV_SPI_MODE		0x03
#else
#define CONFIG_SYS_QE_FMAN_FW_IN_NOR
/* FMan fireware Pre-load address */
#define CONFIG_SYS_FMAN_FW_ADDR		0x60300000
#endif
#define CONFIG_SYS_QE_FMAN_FW_LENGTH	0x10000
#define CONFIG_SYS_FDT_PAD		(0x3000 + CONFIG_SYS_QE_FMAN_FW_LENGTH)
#endif
#endif

/* Miscellaneous configurable options */
#define CONFIG_SYS_LOAD_ADDR	(CONFIG_SYS_DDR_SDRAM_BASE + 0x10000000)
#define CONFIG_ARCH_EARLY_INIT_R
#define CONFIG_BOARD_LATE_INIT

#define CONFIG_HWCONFIG
#define HWCONFIG_BUFFER_SIZE		128

#ifndef CONFIG_SPL_BUILD
#if defined(CONFIG_QSPI_BOOT) || defined(CONFIG_SD_BOOT_QSPI)
#define MTDPARTS_DEFAULT "mtdparts=spi0.0:1m(uboot)," \
			"5m(kernel),1m(dtb),9m(file_system)"
#else
#define MTDPARTS_DEFAULT "mtdparts=60000000.nor:1m(nor_bank0_rcw)," \
			"1m(nor_bank0_uboot),1m(nor_bank0_uboot_env)," \
			"1m(nor_bank0_fman_uconde),40m(nor_bank0_fit)," \
			"1m(nor_bank4_rcw),1m(nor_bank4_uboot)," \
			"1m(nor_bank4_uboot_env),1m(nor_bank4_fman_ucode)," \
			"40m(nor_bank4_fit);7e800000.flash:" \
			"1m(nand_uboot),1m(nand_uboot_env)," \
			"20m(nand_fit);spi0.0:1m(uboot)," \
			"5m(kernel),1m(dtb),9m(file_system)"
#endif

/* Initial environment variables */
#define CONFIG_EXTRA_ENV_SETTINGS		\
	"hwconfig=fsl_ddr:bank_intlv=auto\0"	\
	"loadaddr=0x80100000\0"			\
	"fdt_high=0xffffffffffffffff\0"		\
	"initrd_high=0xffffffffffffffff\0"	\
	"kernel_start=0x61100000\0"		\
	"kernel_load=0xa0000000\0"		\
	"kernel_size=0x2800000\0"		\
	"console=ttyS0,115200\0"                \
	"mtdparts=" MTDPARTS_DEFAULT "\0"

#define CONFIG_BOOTARGS			"console=ttyS0,115200 root=/dev/ram0 " \
					"earlycon=uart8250,mmio,0x21c0500 "    \
					MTDPARTS_DEFAULT

#if defined(CONFIG_QSPI_BOOT) || defined(CONFIG_SD_BOOT_QSPI)
#define CONFIG_BOOTCOMMAND		"sf probe && sf read $kernel_load "    \
					"e0000 f00000 && bootm $kernel_load"
#else
#define CONFIG_BOOTCOMMAND		"cp.b $kernel_start $kernel_load "     \
					"$kernel_size && bootm $kernel_load"
#endif
#endif

/* Monitor Command Prompt */
#define CONFIG_SYS_CBSIZE		512	/* Console I/O Buffer Size */
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE /* Boot args buffer */
#define CONFIG_SYS_LONGHELP
#define CONFIG_CMDLINE_EDITING		1
#define CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_MAXARGS		64	/* max command args */

#define CONFIG_SYS_BOOTM_LEN   (64 << 20)      /* Increase max gunzip size */

/* Hash command with SHA acceleration supported in hardware */
#ifdef CONFIG_FSL_CAAM
#define CONFIG_CMD_HASH
#define CONFIG_SHA_HW_ACCEL
#endif

#endif /* __LS1043A_COMMON_H */

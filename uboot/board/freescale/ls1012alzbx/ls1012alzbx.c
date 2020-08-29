/*
 * Copyright 2016 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:    GPL-2.0+
 */

#include <common.h>
#include <i2c.h>
#include <asm/io.h>
#include <asm/arch/clock.h>
#include <asm/arch/fsl_serdes.h>
#include <asm/arch/soc.h>
#include <hwconfig.h>
#include <fsl_csu.h>
#include <environment.h>
#include <fsl_mmdc.h>
#include <netdev.h>
#ifdef CONFIG_FSL_LS_PPA
#include <asm/arch/ppa.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

int checkboard(void)
{
    puts("Board: DAB-OWRT-A64\r\n");

    return 0;
}

int dram_init(void)
{
#if (CONFIG_SYS_SDRAM_SIZE == 0x40000000)
    static const struct fsl_mmdc_info mparam = {
        0x05180000,    /* mdctl */
        0x00030035,    /* mdpdc */
        0x12554000,    /* mdotc */
        0xbabf7954,    /* mdcfg0 */
        0xdb328f64,    /* mdcfg1 */
        0x01ff00db,    /* mdcfg2 */
        0x00001680,    /* mdmisc */
        0x0f3c8000,    /* mdref */
        0x00002000,    /* mdrwd */
        0x00bf1023,    /* mdor */
        0x0000003f,    /* mdasp */
        0x0000022a,    /* mpodtctrl */
        0xa1390003,    /* mpzqhwctrl */
    };
#else
    static const struct fsl_mmdc_info mparam = {
        0x04180000,    /* mdctl */
        0x00030035,    /* mdpdc */
        0x12554000,    /* mdotc */
        0xbabf7954,    /* mdcfg0 */
        0xdb328f64,    /* mdcfg1 */
        0x01ff00db,    /* mdcfg2 */
        0x00001680,    /* mdmisc */
        0x0f3c8000,    /* mdref */
        0x00002000,    /* mdrwd */
        0x00bf1023,    /* mdor */
        0x0000003f,    /* mdasp */
        0x0000022a,    /* mpodtctrl */
        0xa1390003,    /* mpzqhwctrl */
    };
#endif
    mmdc_init(&mparam);

    gd->ram_size = CONFIG_SYS_SDRAM_SIZE;

    return 0;
}

int board_early_init_f(void)
{
    fsl_lsch2_early_init_f();

    return 0;
}

/* program the battery charger (BQ25895) */
void lzbx_program_battch(void)
{
    unsigned int i2c_bus;
    u8 i2c_device_id, i2c_reg8;

#define LS1LZBX_I2C_BUS_BQ25895     (0)
#define BQ25895_ADDR                (0x6a)
#define BQ25895_DEVICEID_MASK       (0x3b)

#define BQ25895_REG02_CONV_START    (1 << 7U)
#define BQ25895_REG02_CONV_RATE     (1 << 6U)
#define BQ25895_REG02_BOOST_FREQ    (1 << 5U)
#define BQ25895_REG02_ICO_EN        (1 << 4U)
#define BQ25895_REG02_HVDCP_EN      (1 << 3U)
#define BQ25895_REG02_MAXC_EN       (1 << 2U)
#define BQ25895_REG02_AUTO_DPDM_EN  (1 << 0U)

#define BQ25895_REG03_WD_RST        (1 << 6U)
#define BQ25895_REG03_OTG_CONFIG    (1 << 5U)
#define BQ25895_REG03_CHG_CONFIG    (1 << 4U)
#define BQ25895_REG03_SYS_MIN2      (1 << 3U)
#define BQ25895_REG03_SYS_MIN1      (1 << 2U)
#define BQ25895_REG03_SYS_MIN0      (1 << 1U)

#define BQ25895_REG07_EN_TERM       (1 << 7U)
#define BQ25895_REG07_WATCHDOG1     (1 << 5U)
#define BQ25895_REG07_WATCHDOG0     (1 << 4U)
#define BQ25895_REG07_EN_TIMER      (1 << 3U)
#define BQ25895_REG07_CHG_TIMER1    (1 << 2U)
#define BQ25895_REG07_CHG_TIMER0    (1 << 1U)
#define BQ25895_REG07_RESERVED      (1 << 0U)

#define BQ25895_REG11_VBUS_GD       (1 << 7U)


    i2c_bus = i2c_get_bus_num();
    i2c_set_bus_num(LS1LZBX_I2C_BUS_BQ25895);
    i2c_device_id = i2c_reg_read(BQ25895_ADDR, 0x14) &
                    BQ25895_DEVICEID_MASK;
    if (i2c_device_id == 0) {
        printf("The battery charger (BQ25895) does not exist.\n");
        return;
    }

    i2c_reg_write(BQ25895_ADDR, 0x00, 0x3f);
    i2c_reg_write(BQ25895_ADDR, 0x02, BQ25895_REG02_CONV_START |
                                      BQ25895_REG02_BOOST_FREQ |
                                      BQ25895_REG02_ICO_EN     |
                                      BQ25895_REG02_HVDCP_EN   |
                                      BQ25895_REG02_MAXC_EN    |
                                      BQ25895_REG02_AUTO_DPDM_EN);
    i2c_reg_write(BQ25895_ADDR, 0x07, BQ25895_REG07_EN_TERM    |
                                      BQ25895_REG07_WATCHDOG1  |
                                      BQ25895_REG07_WATCHDOG0  |
                                      BQ25895_REG07_EN_TIMER   |
                                      BQ25895_REG07_CHG_TIMER1 |
                                      BQ25895_REG07_RESERVED); /* I2C Watchdog timer = 160s, Fast charge time = 12hrs */
    i2c_reg_write(BQ25895_ADDR, 0x03, BQ25895_REG03_WD_RST     |
                                      BQ25895_REG03_OTG_CONFIG |
                                      BQ25895_REG03_CHG_CONFIG |
                                      BQ25895_REG03_SYS_MIN1   |
                                      BQ25895_REG03_SYS_MIN0); /* Minimum Vsys = 3.3V */
    i2c_reg_write(BQ25895_ADDR, 0x04, 0x3f); /* Fast Charge Current Limit = 4032mA */
    i2c_reg_write(BQ25895_ADDR, 0x06, 0x96); /* Charge Voltage Limit = 4432mV */

    i2c_reg8 = i2c_reg_read(BQ25895_ADDR, 0x0b);

    switch ((i2c_reg8 >> 3) & 3) {
        case 0:
            printf("[BQ25895] Not Charging\n");
            break;
        case 1:
            printf("[BQ25895] Pre-charge (< Vbatlowv)\n");
            break;
        case 2:
            printf("[BQ25895] Fast Charging\n");
            break;
        case 3:
            printf("[BQ25895] Charge Termination Done\n");
            break;
    }

    switch ((i2c_reg8 >> 5) & 7) {
        case 0:
        case 1:
            printf("[BQ25895] No Input 001: USB Host SDP\n");
            break;
        case 2:
            printf("[BQ25895] USB CDP (1.5A)\n");
            break;
        case 3:
            printf("[BQ25895] USB DCP (3.25A)\n");
            break;
        case 4:
            printf("[BQ25895] Adjustable High Voltage DCP (MaxCharge) (1.5A)\n");
            break;
        case 5:
            printf("[BQ25895] Unknown Adapter (500mA)\n");
            break;
        case 6:
            printf("[BQ25895] Non-Standard Adapter (1A/2A/2.1A/2.4A)\n");
            break;
        case 7:
            printf("[BQ25895] OTG\n");
            break;
    }

    /* Wait for ADC values ready */
    i2c_reg8 = 0;
    while (!i2c_reg8)
    {
        i2c_reg8 = i2c_reg_read(BQ25895_ADDR, 0x0e) & 0x7f;
    }

    printf("[BQ25895] Vbat: %d mV\n",
        ((i2c_reg8 & 0x40) ? 1280 : 0) +
        ((i2c_reg8 & 0x20) ? 640 : 0) +
        ((i2c_reg8 & 0x10) ? 320 : 0) +
        ((i2c_reg8 & 0x8) ? 160 : 0) +
        ((i2c_reg8 & 0x4) ? 80 : 0) +
        ((i2c_reg8 & 0x2) ? 40 : 0) +
        ((i2c_reg8 & 0x1) ? 20 : 0) + 2304
    );

    /* Wait for ADC values ready */
    i2c_reg8 = 0;
    while (!i2c_reg8)
    {
        i2c_reg8 = i2c_reg_read(BQ25895_ADDR, 0x0f) & 0x7f;
    }

    printf("[BQ25895] Vsys: %d mV\n",
        ((i2c_reg8 & 0x40) ? 1280 : 0) +
        ((i2c_reg8 & 0x20) ? 640 : 0) +
        ((i2c_reg8 & 0x10) ? 320 : 0) +
        ((i2c_reg8 & 0x8) ? 160 : 0) +
        ((i2c_reg8 & 0x4) ? 80 : 0) +
        ((i2c_reg8 & 0x2) ? 40 : 0) +
        ((i2c_reg8 & 0x1) ? 20 : 0) + 2304
    );

    i2c_reg8 = i2c_reg_read(BQ25895_ADDR, 0x11);

    if (i2c_reg8 & BQ25895_REG11_VBUS_GD)
    {
        if ((i2c_reg8 & 0x7f) != 0)
        {
            printf("[BQ25895] Vbus: %d mV\n",
                ((i2c_reg8 & 0x40) ? 6400 : 0) +
                ((i2c_reg8 & 0x20) ? 3200 : 0) +
                ((i2c_reg8 & 0x10) ? 1600 : 0) +
                ((i2c_reg8 & 0x8) ? 800 : 0) +
                ((i2c_reg8 & 0x4) ? 400 : 0) +
                ((i2c_reg8 & 0x2) ? 200 : 0) +
                ((i2c_reg8 & 0x1) ? 100 : 0) + 2600
            );
        }

        i2c_reg8 = i2c_reg_read(BQ25895_ADDR, 0x12) & 0x7f;
        if ((i2c_reg8 & 0x7f) != 0)
        {
            printf("[BQ25895] Ichrg: %d mA\n",
                ((i2c_reg8 & 0x40) ? 6400 : 0) +
                ((i2c_reg8 & 0x20) ? 3200 : 0) +
                ((i2c_reg8 & 0x10) ? 1600 : 0) +
                ((i2c_reg8 & 0x8) ? 800 : 0) +
                ((i2c_reg8 & 0x4) ? 400 : 0) +
                ((i2c_reg8 & 0x2) ? 200 : 0) +
                ((i2c_reg8 & 0x1) ? 100 : 0)
            );
        }
    }

    i2c_reg_write(BQ25895_ADDR, 0x02, BQ25895_REG02_CONV_RATE  |
                                      BQ25895_REG02_BOOST_FREQ |
                                      BQ25895_REG02_ICO_EN     |
                                      BQ25895_REG02_HVDCP_EN   |
                                      BQ25895_REG02_MAXC_EN    |
                                      BQ25895_REG02_AUTO_DPDM_EN);

    i2c_set_bus_num(i2c_bus);
}


int board_init(void)
{
    struct ccsr_cci400 *cci = (struct ccsr_cci400 *)CONFIG_SYS_CCI400_ADDR;
    /*
     * Set CCI-400 control override register to enable barrier
     * transaction
     */
    out_le32(&cci->ctrl_ord, CCI400_CTRLORD_EN_BARRIER);

#ifdef CONFIG_ENV_IS_NOWHERE
    gd->env_addr = (ulong)&default_environment[0];
#endif

#ifdef CONFIG_LAYERSCAPE_NS_ACCESS
    enable_layerscape_ns_access();
#endif

#ifdef CONFIG_FSL_LS_PPA
    ppa_init();
#endif

    lzbx_program_battch();

    return 0;
}

int ft_board_setup(void *blob, bd_t *bd)
{
    arch_fixup_fdt(blob);

    ft_cpu_setup(blob, bd);

    return 0;
}

void dram_init_banksize(void)
{
    /*
     * gd->arch.secure_ram tracks the location of secure memory.
     * It was set as if the memory starts from 0.
     * The address needs to add the offset of its bank.
     */
    gd->bd->bi_dram[0].start = CONFIG_SYS_SDRAM_BASE;
    if (gd->ram_size > CONFIG_SYS_DDR_BLOCK1_SIZE) {
        gd->bd->bi_dram[0].size = CONFIG_SYS_DDR_BLOCK1_SIZE;
        gd->bd->bi_dram[1].start = CONFIG_SYS_DDR_BLOCK2_BASE;
        gd->bd->bi_dram[1].size = gd->ram_size -
            CONFIG_SYS_DDR_BLOCK1_SIZE;
#ifdef CONFIG_SYS_MEM_RESERVE_SECURE
        gd->arch.secure_ram = gd->bd->bi_dram[1].start +
            gd->arch.secure_ram -
            CONFIG_SYS_DDR_BLOCK1_SIZE;
        gd->arch.secure_ram |= MEM_RESERVE_SECURE_MAINTAINED;
#endif
    } else {
        gd->bd->bi_dram[0].size = gd->ram_size;
#ifdef CONFIG_SYS_MEM_RESERVE_SECURE
        gd->arch.secure_ram = gd->bd->bi_dram[0].start +
            gd->arch.secure_ram;
        gd->arch.secure_ram |= MEM_RESERVE_SECURE_MAINTAINED;
#endif
    }
}

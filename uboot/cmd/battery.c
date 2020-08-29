/*
 * DAB-Embedded BVBA, 2018
 *
 * Battery charger access
 */
#include <common.h>
#include <errno.h>
#include <dm.h>
#include <dm/uclass-internal.h>
#include <i2c.h>

#define LIMIT_DEV    32
#define LIMIT_PARENT    20

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

static int do_status(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
    unsigned int i2c_bus;
    u8 i2c_device_id, i2c_reg8;

    printf("[BQ25895] =============================================\n");

    i2c_bus = i2c_get_bus_num();
    i2c_set_bus_num(LS1LZBX_I2C_BUS_BQ25895);
    i2c_device_id = i2c_reg_read(BQ25895_ADDR, 0x14) &
                    BQ25895_DEVICEID_MASK;
    if (i2c_device_id == 0) {
        printf("The battery charger (BQ25895) does not exist.\n");
        return CMD_RET_FAILURE;
    }

    i2c_reg_write(BQ25895_ADDR, 0x02, BQ25895_REG02_CONV_START |
                                      BQ25895_REG02_BOOST_FREQ |
                                      BQ25895_REG02_ICO_EN     |
                                      BQ25895_REG02_HVDCP_EN   |
                                      BQ25895_REG02_MAXC_EN    |
                                      BQ25895_REG02_AUTO_DPDM_EN);

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

    printf("=======================================================\n");

    return CMD_RET_SUCCESS;
}

static int do_update(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
    unsigned int i2c_bus;
    u8 i2c_device_id;

    i2c_bus = i2c_get_bus_num();
    i2c_set_bus_num(LS1LZBX_I2C_BUS_BQ25895);
    i2c_device_id = i2c_reg_read(BQ25895_ADDR, 0x14) &
                    BQ25895_DEVICEID_MASK;
    if (i2c_device_id == 0) {
        printf("The battery charger (BQ25895) does not exist.\n");
        return CMD_RET_FAILURE;
    }

    i2c_reg_write(BQ25895_ADDR, 0x00, 0x3f);
    i2c_reg_write(BQ25895_ADDR, 0x02, BQ25895_REG02_CONV_RATE  |
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

    i2c_set_bus_num(i2c_bus);

    printf("[BQ25895] Updated\n");

    return CMD_RET_SUCCESS;
}

static int do_dev(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{

    return CMD_RET_SUCCESS;
}

static cmd_tbl_t subcmd[] = {
    U_BOOT_CMD_MKENT(dev, 2, 1, do_dev, "", ""),
    U_BOOT_CMD_MKENT(status, 1, 1, do_status, "", ""),
    U_BOOT_CMD_MKENT(update, 1, 1, do_update, "", ""),
};

static int do_battery(cmd_tbl_t *cmdtp, int flag, int argc,
            char * const argv[])
{
    cmd_tbl_t *cmd;

    argc--;
    argv++;

    cmd = find_cmd_tbl(argv[0], subcmd, ARRAY_SIZE(subcmd));
    if (cmd == NULL || argc > cmd->maxargs)
        return CMD_RET_USAGE;

    return cmd->cmd(cmdtp, flag, argc, argv);
}

U_BOOT_CMD(battery, CONFIG_SYS_MAXARGS, 1, do_battery,
    " operations",
    "status     - show information from battery monitor\n"
    "battery update     - update settings\n"
);

/*
 * Copyright 2016 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <netdev.h>
#include <fm_eth.h>
#include <fsl_mdio.h>
#include <malloc.h>
#include <fsl_dtsec.h>
#include <asm/arch/soc.h>
#include <asm/arch-fsl-layerscape/config.h>
#include <asm/arch/fsl_serdes.h>

#include "../../../drivers/net/pfe_eth/pfe_eth.h"
#include <asm/arch-fsl-layerscape/immap_lsch2.h>

#define DEFAULT_PFE_MDIO_NAME "PFE_MDIO"
#define DEFAULT_PFE_MDIO1_NAME "PFE_MDIO1"

#define MASK_ETH_PHY_RST	0x00000100

void reset_phy(void)
{
}

int board_eth_init(bd_t *bis)
{
#ifdef CONFIG_FSL_PPFE
        struct mii_dev *bus;
	struct mdio_info mac1_mdio_info;
	struct ccsr_scfg *scfg = (struct ccsr_scfg *)CONFIG_SYS_FSL_SCFG_ADDR;

	reset_phy();

	/*CCI-400 QoS settings for PFE */
	out_be32(&scfg->wr_qos1, 0x0ff00000);
	out_be32(&scfg->rd_qos1, 0x0ff00000);

	/* Set RGMII into 1G + Full duplex mode */
	out_be32(&scfg->rgmiipcr, in_be32(&scfg->rgmiipcr) | (SCFG_RGMIIPCR_SETSP_1000M | SCFG_RGMIIPCR_SETFD));


	out_be32((CONFIG_SYS_DCSR_DCFG_ADDR + 0x520), 0xFFFFFFFF);
	out_be32((CONFIG_SYS_DCSR_DCFG_ADDR + 0x524), 0xFFFFFFFF);

	mac1_mdio_info.reg_base = (void *)0x04200000; /*EMAC1_BASE_ADDR*/
	mac1_mdio_info.name = DEFAULT_PFE_MDIO_NAME;

	bus = ls1012a_mdio_init(&mac1_mdio_info);
	if(!bus)
	{
		printf("Failed to register mdio \n");
		return -1;
	}

	/*We don't really need this MDIO bus,
	* this is called just to initialize EMAC2 MDIO interface*/
	mac1_mdio_info.reg_base = (void *)0x04220000; /*EMAC2_BASE_ADDR*/
	mac1_mdio_info.name = DEFAULT_PFE_MDIO1_NAME;

	bus = ls1012a_mdio_init(&mac1_mdio_info);
	if(!bus)
	{
		printf("Failed to register mdio \n");
		return -1;
	}


	/*MAC1 */
	ls1012a_set_mdio(0, miiphy_get_dev_by_name(DEFAULT_PFE_MDIO_NAME));
	ls1012a_set_phy_address_mode(0,  EMAC1_PHY_ADDR, PHY_INTERFACE_MODE_SGMII);

	/*MAC2 */
	ls1012a_set_mdio(1, miiphy_get_dev_by_name(DEFAULT_PFE_MDIO_NAME));
	ls1012a_set_phy_address_mode(1,  EMAC2_PHY_ADDR, PHY_INTERFACE_MODE_SGMII);


	cpu_eth_init(bis);
#endif
	return pci_eth_init(bis);
}

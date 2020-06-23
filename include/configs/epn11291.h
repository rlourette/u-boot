/*
 * am335x_evm.h
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __CONFIG_AM335X_EVM_H
#define __CONFIG_AM335X_EVM_H

#include <configs/ti_am335x_common.h>
#include <linux/sizes.h>

#undef CONFIG_I2C
#undef CONFIG_SYS_I2C

#ifndef CONFIG_SPL_ETH_SUPPORT
#define CONFIG_SPL_ETH_SUPPORT
#endif

#ifndef CONFIG_SPL_NET_SUPPORT
#define CONFIG_SPL_NET_SUPPORT
#endif

#ifndef CONFIG_SPL_BUILD
# define CONFIG_TIMESTAMP
#endif

#define CONFIG_SYS_BOOTM_LEN		SZ_16M

#define CONFIG_MACH_TYPE		MACH_TYPE_AM335XEVM

/* Clock Defines */
#define V_OSCK				25000000  /* Clock output from T2 */
#define V_SCLK				(V_OSCK)

/* Custom script for NOR */
#define CONFIG_SYS_LDSCRIPT		"board/flightcircuits/epn11291/u-boot.lds"


#define BOOTENV_DEV_LEGACY_MMC(devtypeu, devtypel, instance) \
	"bootcmd_" #devtypel #instance "=" \
	"setenv devtype mmc; " \
	"setenv mmcdev " #instance"; "\
	"setenv bootpart " #instance":1 ; "\
	"run boot\0"

#define BOOTENV_DEV_NAME_LEGACY_MMC(devtypeu, devtypel, instance) \
	#devtypel #instance " "

#define BOOTENV_DEV_NAND(devtypeu, devtypel, instance) \
	"bootcmd_" #devtypel "=" \
	"run nandboot\0"

#define BOOTENV_DEV_NAME_NAND(devtypeu, devtypel, instance) \
	#devtypel #instance " "

#if CONFIG_IS_ENABLED(CMD_PXE)
# define BOOT_TARGET_PXE(func) func(PXE, pxe, na)
#else
# define BOOT_TARGET_PXE(func)
#endif

#if CONFIG_IS_ENABLED(CMD_DHCP)
# define BOOT_TARGET_DHCP(func) func(DHCP, dhcp, na)
#else
# define BOOT_TARGET_DHCP(func)
#endif

#define BOOT_TARGET_DEVICES(func) \
	func(MMC, mmc, 0) \
	func(LEGACY_MMC, legacy_mmc, 0) \
	func(MMC, mmc, 1) \
	func(LEGACY_MMC, legacy_mmc, 1) \
	BOOT_TARGET_PXE(func) \
	BOOT_TARGET_DHCP(func)

#include <config_distro_bootcmd.h>

#ifndef CONFIG_SPL_BUILD
#include <environment/ti/dfu.h>
#include <environment/ti/mmc.h>

#define CONFIG_EXTRA_ENV_SETTINGS \
	DEFAULT_LINUX_BOOT_ENV \
	DEFAULT_MMC_TI_ARGS \
	DEFAULT_FIT_TI_ARGS \
	"bootpart=1:2\0" \
	"bootdir=/boot\0" \
	"bootfile=zImage\0" \
	"fdtfile=epn11291.dtb\0" \
	"console=ttyO0,115200n8\0" \
	"partitions=" \
		"uuid_disk=${uuid_gpt_disk};" \
		"name=bootloader,start=384K,size=1792K," \
			"uuid=${uuid_gpt_bootloader};" \
		"name=rootfs,start=2688K,size=-,uuid=${uuid_gpt_rootfs}\0" \
	"optargs=\0" \
	"ramroot=/dev/ram0 rw\0" \
	"ramrootfstype=ext2\0" \
	"ramargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"root=${ramroot} " \
		"rootfstype=${ramrootfstype}\0" \
	"loadramdisk=load mmc ${mmcdev} ${rdaddr} ramdisk.gz\0" \
	"pb_eeprom_hdr=" \
		"mw 82001000 ee3355aa; " \
		"mw 82001004 35333341; " \
		"mw 82001008 4c474250\0" \
	"serverip=192.168.2.1\0" \
	"ipaddr=192.168.2.2\0" \
	"if_netconsole=ping $serverip\0" \
	"start_netconsole=" \
		"setenv ncip $serverip; " \
		"setenv bootdelay 10; " \
		"setenv stdin serial,nc; " \
		"setenv stdout serial,nc; " \
		"setenv stderr serial,nc; " \
		"version\0" \
	"preboot=run if_netconsole start_netconsole\0"\
	"ramboot=echo Booting from ramdisk ...; " \
		"run ramargs; " \
		"bootz ${loadaddr} ${rdaddr} ${fdtaddr}\0" \
	"findfdt= setenv fdtfile epn11291.dtb;" \
	"init_console = setenv console ttyO0,115200n8;" \
	NETARGS \
	BOOTENV
#endif

/* NS16550 Configuration */
#define CONFIG_SYS_NS16550_COM1		0x44e09000	/* Base EVM has UART0 */

/* SPL */
#ifndef CONFIG_NOR_BOOT
/* Bootcount using the RTC block */
#define CONFIG_SYS_BOOTCOUNT_BE

/* USB gadget RNDIS */
#endif


/*
 * Disable MMC DM for SPL build and can be re-enabled after adding
 * DM support in SPL
 */
#ifdef CONFIG_SPL_BUILD
#undef CONFIG_DM_MMC
#undef CONFIG_TIMER
#endif

#if defined(CONFIG_SPL_BUILD) && defined(CONFIG_SPL_USB_ETHER)
/* Remove other SPL modes. */
/* disable host part of MUSB in SPL */
/* disable EFI partitions and partition UUID support */
#endif

/*
 * Default to using SPI for environment, etc.
 * 0x000000 - 0x020000 : SPL (128KiB)
 * 0x020000 - 0x0A0000 : U-Boot (512KiB)
 * 0x0A0000 - 0x0BFFFF : First copy of U-Boot Environment (128KiB)
 * 0x0C0000 - 0x0DFFFF : Second copy of U-Boot Environment (128KiB)
 * 0x0E0000 - 0x442000 : Linux Kernel
 * 0x442000 - 0x800000 : Userland
 */
#if defined(CONFIG_EMMC_BOOT)
#define CONFIG_SYS_MMC_ENV_DEV		1
#define CONFIG_SYS_MMC_ENV_PART		0
#define CONFIG_ENV_OFFSET		0x260000
#define CONFIG_ENV_OFFSET_REDUND	(CONFIG_ENV_OFFSET + CONFIG_ENV_SIZE)
#define CONFIG_SYS_REDUNDAND_ENVIRONMENT
#define CONFIG_SYS_MMC_MAX_DEVICE	2
#endif

#ifdef CONFIG_DRIVER_TI_CPSW
#define CONFIG_CLOCK_SYNTHESIZER
#define CLK_SYNTHESIZER_I2C_ADDR 0x65
#endif

#endif	/* ! __CONFIG_AM335X_EVM_H */

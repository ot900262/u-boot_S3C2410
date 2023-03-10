/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2011 Samsung Electronics
 * Heungjun Kim <riverful.kim@samsung.com>
 *
 * Configuation settings for the SAMSUNG TRATS (EXYNOS4210) board.
 */

#ifndef __CONFIG_TRATS_H
#define __CONFIG_TRATS_H

#include <configs/exynos4-common.h>

#ifndef CONFIG_SYS_L2CACHE_OFF
#define CFG_SYS_PL310_BASE	0x10502000
#endif

/* TRATS has 4 banks of DRAM */
#define CFG_SYS_SDRAM_BASE		0x40000000
#define PHYS_SDRAM_1			CFG_SYS_SDRAM_BASE
#define SDRAM_BANK_SIZE			(256 << 20)	/* 256 MB */

/* Tizen - partitions definitions */
#define PARTS_CSA		"csa-mmc"
#define PARTS_BOOT		"boot"
#define PARTS_QBOOT		"qboot"
#define PARTS_CSC		"csc"
#define PARTS_ROOT		"platform"
#define PARTS_DATA		"data"
#define PARTS_UMS		"ums"

#define PARTS_DEFAULT \
	"uuid_disk=${uuid_gpt_disk};" \
	"name="PARTS_CSA",start=5MiB,size=8MiB,uuid=${uuid_gpt_"PARTS_CSA"};" \
	"name="PARTS_BOOT",size=60MiB,uuid=${uuid_gpt_"PARTS_BOOT"};" \
	"name="PARTS_QBOOT",size=100MiB,uuid=${uuid_gpt_"PARTS_QBOOT"};" \
	"name="PARTS_CSC",size=150MiB,uuid=${uuid_gpt_"PARTS_CSC"};" \
	"name="PARTS_ROOT",size=1536MiB,uuid=${uuid_gpt_"PARTS_ROOT"};" \
	"name="PARTS_DATA",size=3000MiB,uuid=${uuid_gpt_"PARTS_DATA"};" \
	"name="PARTS_UMS",size=-,uuid=${uuid_gpt_"PARTS_UMS"}\0" \

#define CFG_DFU_ALT \
	"u-boot raw 0x80 0x400;" \
	"/uImage ext4 0 2;" \
	"/modem.bin ext4 0 2;" \
	"/exynos4210-trats.dtb ext4 0 2;" \
	""PARTS_CSA" part 0 1;" \
	""PARTS_BOOT" part 0 2;" \
	""PARTS_QBOOT" part 0 3;" \
	""PARTS_CSC" part 0 4;" \
	""PARTS_ROOT" part 0 5;" \
	""PARTS_DATA" part 0 6;" \
	""PARTS_UMS" part 0 7;" \
	"params.bin raw 0x38 0x8;" \
	"/Image.itb ext4 0 2\0"

#define CFG_EXTRA_ENV_SETTINGS \
	"bootk=" \
		"run loaduimage;" \
		"if run loaddtb; then " \
			"bootm 0x40007FC0 - ${fdtaddr};" \
		"fi;" \
		"bootm 0x40007FC0;\0" \
	"updatebackup=" \
		"mmc dev 0 2; mmc write 0 0x42100000 0 0x200;" \
		"mmc dev 0 0\0" \
	"updatebootb=" \
		"mmc read 0 0x42100000 0x80 0x200; run updatebackup\0" \
	"lpj=lpj=3981312\0" \
	"nfsboot=" \
		"setenv bootargs root=/dev/nfs rw " \
		"nfsroot=${nfsroot},nolock,tcp " \
		"ip=${ipaddr}:${serverip}:${gatewayip}:" \
		"${netmask}:generic:usb0:off ${console} ${meminfo}" \
		"; run bootk\0" \
	"ramfsboot=" \
		"setenv bootargs root=/dev/ram0 rw rootfstype=ext2 " \
		"${console} ${meminfo} " \
		"initrd=0x43000000,8M ramdisk=8192\0" \
	"mmcboot=" \
		"setenv bootargs root=/dev/mmcblk${mmcdev}p${mmcrootpart} " \
		"${lpj} rootwait ${console} ${meminfo} ${opts} ${lcdinfo}; " \
		"run bootk\0" \
	"bootchart=setenv opts init=/sbin/bootchartd; run bootcmd\0" \
	"boottrace=setenv opts initcall_debug; run bootcmd\0" \
	"mmcoops=mmc read 0 0x40000000 0x40 8; md 0x40000000 0x400\0" \
	"verify=n\0" \
	"rootfstype=ext4\0" \
	"console=console=ttySAC2,115200n8\0" \
	"meminfo=crashkernel=32M@0x50000000\0" \
	"nfsroot=/nfsroot/arm\0" \
	"bootblock=10\0" \
	"loaduimage=ext4load mmc ${mmcdev}:${mmcbootpart} 0x40007FC0 uImage\0" \
	"loaddtb=ext4load mmc ${mmcdev}:${mmcbootpart} ${fdtaddr} " \
		"${fdtfile}\0" \
	"mmcdev=0\0" \
	"mmcbootpart=2\0" \
	"mmcrootpart=5\0" \
	"opts=always_resume=1\0" \
	"partitions=" PARTS_DEFAULT \
	"dfu_alt_info=" CFG_DFU_ALT \
	"spladdr=0x40000100\0" \
	"splsize=0x200\0" \
	"splfile=falcon.bin\0" \
	"spl_export=" \
		   "setexpr spl_imgsize ${splsize} + 8 ;" \
		   "setenv spl_imgsize 0x${spl_imgsize};" \
		   "setexpr spl_imgaddr ${spladdr} - 8 ;" \
		   "setexpr spl_addr_tmp ${spladdr} - 4 ;" \
		   "mw.b ${spl_imgaddr} 0x00 ${spl_imgsize};run loaduimage;" \
		   "setenv bootargs root=/dev/mmcblk${mmcdev}p${mmcrootpart} " \
		   "${lpj} rootwait ${console} ${meminfo} ${opts} ${lcdinfo};" \
		   "spl export atags 0x40007FC0;" \
		   "crc32 ${spladdr} ${splsize} ${spl_imgaddr};" \
		   "mw.l ${spl_addr_tmp} ${splsize};" \
		   "ext4write mmc ${mmcdev}:${mmcbootpart}" \
		   " /${splfile} ${spl_imgaddr} ${spl_imgsize};" \
		   "setenv spl_imgsize;" \
		   "setenv spl_imgaddr;" \
		   "setenv spl_addr_tmp;\0" \
	ENV_ITB \
	"fdtaddr=40800000\0" \

/* Falcon mode definitions */

/* GPT */

/* Download menu - definitions for check keys */
#ifndef __ASSEMBLY__

#define KEY_PWR_PMIC_NAME		"MAX8997_PMIC"
#define KEY_PWR_STATUS_REG		MAX8997_REG_STATUS1
#define KEY_PWR_STATUS_MASK		(1 << 0)
#define KEY_PWR_INTERRUPT_REG		MAX8997_REG_INT1
#define KEY_PWR_INTERRUPT_MASK		(1 << 0)

#define KEY_VOL_UP_GPIO			EXYNOS4_GPIO_X20
#define KEY_VOL_DOWN_GPIO		EXYNOS4_GPIO_X21
#endif /* __ASSEMBLY__ */

#endif	/* __CONFIG_H */

#ifndef _GX_WDT_H_
#define _CX_WDT_H_

#include <cyg/hal/drv_api.h>
#define _WDT_IOR(x,y,z)	(((x)<<8)|y)
#define _WDT_IOWR(x,y,z)	(((x)<<8)|y)

#define	WATCHDOG_IOCTL_BASE	'W'

struct watchdog_info {
	unsigned int options;		/* Options the card/driver supports */
	unsigned int firmware_version;	/* Firmware version of the card */
	char  identity[32];	/* Identity of the board */
};

#define	WDIOC_GETSUPPORT	_WDT_IOR(WATCHDOG_IOCTL_BASE, 0, struct watchdog_info)
#define	WDIOC_GETSTATUS		_WDT_IOR(WATCHDOG_IOCTL_BASE, 1, int)
#define	WDIOC_GETBOOTSTATUS	_WDT_IOR(WATCHDOG_IOCTL_BASE, 2, int)
#define	WDIOC_GETTEMP		_WDT_IOR(WATCHDOG_IOCTL_BASE, 3, int)
#define	WDIOC_SETOPTIONS	_WDT_IOR(WATCHDOG_IOCTL_BASE, 4, int)
#define	WDIOC_KEEPALIVE		_WDT_IOR(WATCHDOG_IOCTL_BASE, 5, int)
#define	WDIOC_SETTIMEOUT        _WDT_IOWR(WATCHDOG_IOCTL_BASE, 6, int)
#define	WDIOC_GETTIMEOUT        _WDT_IOR(WATCHDOG_IOCTL_BASE, 7, int)
#define	WDIOC_SETPRETIMEOUT	_WDT_IOWR(WATCHDOG_IOCTL_BASE, 8, int)
#define	WDIOC_GETPRETIMEOUT	_WDT_IOR(WATCHDOG_IOCTL_BASE, 9, int)
#define	WDIOC_GETTIMELEFT	_WDT_IOR(WATCHDOG_IOCTL_BASE, 10, int)

#define	WDIOF_UNKNOWN		-1	/* Unknown flag error */
#define	WDIOS_UNKNOWN		-1	/* Unknown status error */

#define	WDIOF_OVERHEAT		0x0001	/* Reset due to CPU overheat */
#define	WDIOF_FANFAULT		0x0002	/* Fan failed */
#define	WDIOF_EXTERN1		0x0004	/* External relay 1 */
#define	WDIOF_EXTERN2		0x0008	/* External relay 2 */
#define	WDIOF_POWERUNDER	0x0010	/* Power bad/power fault */
#define	WDIOF_CARDRESET		0x0020	/* Card previously reset the CPU */
#define	WDIOF_POWEROVER		0x0040	/* Power over voltage */
#define	WDIOF_SETTIMEOUT	0x0080  /* Set timeout (in seconds) */
#define	WDIOF_MAGICCLOSE	0x0100	/* Supports magic close char */
#define	WDIOF_PRETIMEOUT	0x0200  /* Pretimeout (in seconds), get/set */
#define	WDIOF_KEEPALIVEPING	0x8000	/* Keep alive ping reply */

#define	WDIOS_DISABLECARD	0x0001	/* Turn off the watchdog timer */
#define	WDIOS_ENABLECARD	0x0002	/* Turn on the watchdog timer */
#define	WDIOS_TEMPPANIC		0x0004	/* Kernel panic on temperature trip */

#endif // _CX_ARM_GX3110_WDT_H_

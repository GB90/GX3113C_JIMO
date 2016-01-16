/*
 * <linux/usb_gadget.h>
 *
 * We call the USB code inside a Linux-based peripheral device a "gadget"
 * driver, except for the hardware-specific bus glue.  One USB host can
 * master many USB gadgets, but the gadgets are only slaved to one host.
 *
 *
 * (C) Copyright 2002-2004 by David Brownell
 * All Rights Reserved.
 *
 * This software is licensed under the GNU GPL version 2.
 */

#ifndef __EVATRONIX_USB_GADGET_H
#define __EVATRONIX_USB_GADGET_H

#include <usb/spinlock.h>
#include <usb/usb.h>

/**
 * struct usb_gadget - represents a usb slave device
 * @ops: Function pointers used to access hardware-specific operations.
 * @ep0: Endpoint zero, used when reading or writing responses to
 *      driver setup() requests
 * @ep_list: List of other endpoints supported by the device.
 * @speed: Speed of current connection to USB host.
 * @is_dualspeed: True if the controller supports both high and full speed
 *      operation.  If it does, the gadget driver must also support both.
 * @is_otg: True if the USB device port uses a Mini-AB jack, so that the
 *      gadget driver must provide a USB OTG descriptor.
 * @is_a_peripheral: False unless is_otg, the "A" end of a USB cable
 *      is in the Mini-AB jack, and HNP has been used to switch roles
 *      so that the "A" device currently acts as A-Peripheral, not A-Host.
 * @a_hnp_support: OTG device feature flag, indicating that the A-Host
 *      supports HNP at this port.
 * @a_alt_hnp_support: OTG device feature flag, indicating that the A-Host
 *      only supports HNP on a different root port.
 * @b_hnp_enable: OTG device feature flag, indicating that the A-Host
 *      enabled HNP support.
 * @name: Identifies the controller hardware type.  Used in diagnostics
 *      and sometimes configuration.
 * @dev: Driver model state for this abstract device.
 *
 * Gadgets have a mostly-portable "gadget driver" implementing device
 * functions, handling all usb configurations and interfaces.  Gadget
 * drivers talk to hardware-specific code indirectly, through ops vectors.
 * That insulates the gadget driver from hardware details, and packages
 * the hardware endpoints through generic i/o queues.  The "usb_gadget"
 * and "usb_ep" interfaces provide that insulation from the hardware.
 *
 * Except for the driver data, all fields in this structure are
 * read-only to the gadget driver.  That driver data is part of the
 * "driver model" infrastructure in 2.6 (and later) kernels, and for
 * earlier systems is grouped in a similar structure that's not known
 * to the rest of the kernel.
 *
 * Values of the three OTG device feature flags are updated before the
 * setup() call corresponding to USB_REQ_SET_CONFIGURATION, and before
 * driver suspend() calls.  They are valid only when is_otg, and when the
 * device is acting as a B-Peripheral (so is_a_peripheral is false).
 */
 
 
struct usb_gadget {
        /* readonly to gadget driver */
		spinlock_t			lock;
        enum usb_device_speed           speed;
        unsigned                        is_dualspeed:1;
        unsigned                        is_otg:1;
        unsigned                        is_a_peripheral:1;
        unsigned                        b_hnp_enable:1;
        unsigned                        a_hnp_support:1;
        unsigned                        a_alt_hnp_support:1;
        const char                      *name;
        int (* start_device )( struct usb_gadget *gadget);
        int (* stop_device )( struct usb_gadget *gadget);
        struct device                   *dev;
};
/*
static inline void set_gadget_data (struct usb_gadget *gadget, void *data)
        { dev_set_drvdata (&gadget->dev, data); }
static inline void *get_gadget_data (struct usb_gadget *gadget)
        { return dev_get_drvdata (&gadget->dev); }
*/
/**
  * usb_gadget_vbus_disconnect - notify controller about VBUS session end
  * @gadget:the device whose VBUS supply is being described
  *
  * This call is used by a driver for an external transceiver (or GPIO)
  * that detects a VBUS power session ending.  Common responses include
  * reversing everything done in usb_gadget_vbus_connect().
  *
  * Returns zero on success, else negative errno.
*/
extern int usb_gadget_vbus_disconnect(struct usb_gadget *gadget);
extern int usb_gadget_vbus_connect(struct usb_gadget *gadget);
#endif  /* __EVATRONIX_USB_GADGET_H */



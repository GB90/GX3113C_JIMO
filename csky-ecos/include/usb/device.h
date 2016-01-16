/*
 *   This file has been modified by Evatronix. 
 */

/*
 * device.h - generic, centralized driver model
 *
 * Copyright (c) 2001-2003 Patrick Mochel <mochel@osdl.org>
 *
 * This file is released under the GPLv2
 *
 * See Documentation/driver-model/ for more information.
 */

#ifndef _DEVICE_H_
#define _DEVICE_H_


#include <usb/ioport.h>
#include <usb/kobject.h>
#include <usb/klist.h>
#include <linux/list.h>
#include <linux/types.h>
#include <sys/types.h>
#include <usb/arch/semaphore.h>
#include <usb/arch/atomic.h>
#include <cyg/infra/diag.h>    //for diag_printf


#define DEVICE_NAME_SIZE	50
#define DEVICE_NAME_HALF	__stringify(20)	/* Less than half to accommodate slop */
#define DEVICE_ID_SIZE		32
#define BUS_ID_SIZE		KOBJ_NAME_LEN


struct device;
struct device_driver;
struct class;
struct class_device;
struct class_simple;

struct bus_type {
    char			* name;

    struct kset             subsys;
    struct kset             drivers;
    struct kset             devices;
    struct klist            klist_devices;
    struct klist            klist_drivers;

    int  (*match)(struct device * dev, struct device_driver * drv);
    int  (*uevent)(struct device *dev, char **envp,
                              int num_envp, char *buffer, int buffer_size);
    int  (*probe)(struct device * dev);
    int  (*remove)(struct device * dev);
    void (*shutdown)(struct device * dev);

    int (*suspend)(struct device * dev);
    int (*suspend_late)(struct device * dev);
    int (*resume_early)(struct device * dev);
    int (*resume)(struct device * dev);

    unsigned int drivers_autoprobe:1;
};

extern int  bus_register(struct bus_type * bus);
extern void bus_unregister(struct bus_type * bus);

extern int  bus_rescan_devices(struct bus_type * bus);

/* iterator helpers for buses */

int bus_for_each_dev(struct bus_type * bus, struct device * start, void * data,
                     int (*fn)(struct device *, void *));
struct device * bus_find_device(struct bus_type *bus, struct device *start,
                                void *data, int (*match)(struct device *, void *));

int  bus_for_each_drv(struct bus_type *bus,
               struct device_driver *start, void *data,
               int (*fn)(struct device_driver *, void *));

/*
 * Bus notifiers: Get notified of addition/removal of devices
 * and binding/unbinding of drivers to devices.
 * In the long run, it should be a replacement for the platform
 * notify hooks.
 */
struct notifier_block;


/* All 4 notifers below get called with the target struct device *
 * as an argument. Note that those functions are likely to be called
 * with the device semaphore held in the core, so be careful.
 */
#define BUS_NOTIFY_ADD_DEVICE           0x00000001 /* device added */
#define BUS_NOTIFY_DEL_DEVICE           0x00000002 /* device removed */
#define BUS_NOTIFY_BOUND_DRIVER         0x00000003 /* driver bound to device */
#define BUS_NOTIFY_UNBIND_DRIVER        0x00000004 /* driver about to be
                                                      unbound */

struct device_driver {
	char			* name;
	struct bus_type		* bus;

    struct kobject          kobj;
    struct klist            klist_devices;
    struct klist_node       knode_bus;

    int     (*probe)        (struct device * dev);
    int     (*remove)       (struct device * dev);
    void    (*shutdown)     (struct device * dev);
    int     (*suspend)      (struct device * dev);
    int     (*resume)       (struct device * dev);
};


extern int  driver_register(struct device_driver * drv);
extern void driver_unregister(struct device_driver * drv);

extern struct device_driver * get_driver(struct device_driver * drv);
extern void put_driver(struct device_driver * drv);
extern struct device_driver *driver_find(const char *name, struct bus_type *bus);
extern int driver_probe_done(void);

extern int  driver_for_each_device(struct device_driver * drv,
                struct device *start, void *data,
                int (*fn)(struct device *, void *));
struct device * driver_find_device(struct device_driver *drv,
                                   struct device *start, void *data,
                                   int (*match)(struct device *, void *));

/*
 * device classes
 */
struct class {
        char              * name;

        struct kset             subsys;
        struct list_head        children;
        struct list_head        devices;
        struct list_head        interfaces;
        struct kset             class_dirs;
        cyg_sem_t        sem;    /* locks both the children and interfaces lists */

        int     (*uevent)(struct class_device *dev, char **envp,
                           int num_envp, char *buffer, int buffer_size);
        int     (*dev_uevent)(struct device *dev, char **envp, int num_envp,
                                char *buffer, int buffer_size);

        void    (*release)(struct class_device *dev);
        void    (*class_release)(struct class *class);
        void    (*dev_release)(struct device *dev);

//      int     (*suspend)(struct device *);
        int     (*resume)(struct device *);
};

extern int  class_register(struct class *);
extern void class_unregister(struct class *);

/**
 * struct class_device - class devices
 * @class: pointer to the parent class for this class device.  This is required.
 * @devt: for internal use by the driver core only.
 * @node: for internal use by the driver core only.
 * @kobj: for internal use by the driver core only.
 * @dev: if set, a symlink to the struct device is created in the sysfs
 * directory for this struct class device.
 * @class_data: pointer to whatever you want to store here for this struct
 * class_device.  Use class_get_devdata() and class_set_devdata() to get and
 * set this pointer.
 * @parent: pointer to a struct class_device that is the parent of this struct
 * class_device.  If NULL, this class_device will show up at the root of the
 * struct class in sysfs (which is probably what you want to have happen.)
 * @release: pointer to a release function for this struct class_device.  If
 * set, this will be called instead of the class specific release function.
 * Only use this if you want to override the default release function, like
 * when you are nesting class_device structures.
 * @uevent: pointer to a uevent function for this struct class_device.  If
 * set, this will be called instead of the class specific uevent function.
 * Only use this if you want to override the default uevent function, like
 * when you are nesting class_device structures.
 */
struct class_device {
        struct list_head        node;

        struct kobject          kobj;
        struct class            * class;        /* required */
        dev_t                   devt;           /* dev_t, creates the sysfs "dev" */
        struct device           * dev;          /* not necessary, but nice to have */
        void                    * class_data;   /* class-specific data */
        struct class_device     *parent;        /* parent of this child device, if there is one */

	void    (*release)(struct class_device *dev);
        int     (*uevent)(struct class_device *dev, char **envp,
                           int num_envp, char *buffer, int buffer_size);
        char    class_id[BUS_ID_SIZE];  /* unique to this class */
};

static inline void *
class_get_devdata (struct class_device *dev)
{
        return dev->class_data;
}

static inline void
class_set_devdata (struct class_device *dev, void *data)
{
        dev->class_data = data;
}


extern int  class_device_register(struct class_device *);
extern void class_device_unregister(struct class_device *);
extern void class_device_initialize(struct class_device *);
extern int  class_device_add(struct class_device *);
extern void class_device_del(struct class_device *);

extern struct class_device * class_device_get(struct class_device *);
extern void class_device_put(struct class_device *);

struct class_interface {
        struct list_head        node;
        struct class            *class;

        int (*add)      (struct class_device *, struct class_interface *);
        void (*remove)  (struct class_device *, struct class_interface *);
        int (*add_dev)          (struct device *, struct class_interface *);
        void (*remove_dev)      (struct device *, struct class_interface *);
};

extern int  class_interface_register(struct class_interface *);
extern void class_interface_unregister(struct class_interface *);

extern struct class *class_create(void *owner, const char *name);
extern void class_destroy(struct class *cls);
extern struct class_device *class_device_create(struct class *cls,
                                                struct class_device *parent,
                                                dev_t devt,
                                                struct device *device,
                                                const char *fmt, ...)
                                        __attribute__((format(printf,5,6)));
extern void class_device_destroy(struct class *cls, dev_t devt);

/*
 * The type of device, "struct device" is embedded in. A class
 * or bus can contain devices of different types
 * like "partitions" and "disks", "mouse" and "event".
 * This identifies the device type and carries type-specific
 * information, equivalent to the kobj_type of a kobject.
 * If "name" is specified, the uevent will contain it in
 * the DEVTYPE variable.
 */
struct device_type {
        const char *name;
        int (*uevent)(struct device *dev, char **envp, int num_envp,
                      char *buffer, int buffer_size);
        void (*release)(struct device *dev);
//        int (*suspend)(struct device * dev);
        int (*resume)(struct device * dev);
};


/* device resource management */
typedef void (*dr_release_t)(struct device *dev, void *res);
typedef int (*dr_match_t)(struct device *dev, void *res, void *match_data);


struct device {
        struct klist            klist_children;
        struct klist_node       knode_parent;           /* node in sibling list */
        struct klist_node       knode_driver;
        struct klist_node       knode_bus;
        struct device           *parent;

        struct kobject kobj;
        char    bus_id[BUS_ID_SIZE];    /* position on parent bus */
        struct device_type      *type;
        unsigned                is_registered:1;
        unsigned                uevent_suppress:1;

	cyg_sem_t        sem;    /* semaphore to synchronize calls to
                                         * its driver.
                                         */

        struct bus_type * bus;          /* type of bus device is on */
        struct device_driver *driver;   /* which driver has allocated this
                                           device */
        void            *driver_data;   /* data private to the driver */
        void            *platform_data; /* Platform specific data, device
                                           core doesn't touch it */


        spinlock_t              devres_lock;
        struct list_head        devres_head;

        /* class_device migration path */
        struct list_head        node;
        struct class            *class;
        dev_t                   devt;           /* dev_t, creates the sysfs "dev" */

        void    (*release)(struct device * dev);
};

static inline void *
dev_get_drvdata (struct device *dev)
{
       return dev->driver_data;
}

static inline void
dev_set_drvdata (struct device *dev, void *data)
{
        dev->driver_data = data;
}

static inline int device_is_registered(struct device *dev)
{
        return dev->is_registered;
}

void driver_init(void);

/*
 * High level routines for use by the bus drivers
 */
extern int  device_register(struct device * dev);
extern void device_unregister(struct device * dev);
extern void device_initialize(struct device * dev);
extern int  device_add(struct device * dev);
extern void device_del(struct device * dev);
extern int device_for_each_child(struct device *, void *,
                     int (*fn)(struct device *, void *));
extern struct device *device_find_child(struct device *, void *data,
                                        int (*match)(struct device *, void *));
extern int device_rename(struct device *dev, char *new_name);
extern int device_move(struct device *dev, struct device *new_parent);

/*
 * Manual binding of a device to driver. See drivers/base/bus.c
 * for information on use.
 */
extern int  device_bind_driver(struct device *dev);
extern void device_release_driver(struct device * dev);
extern int   device_attach(struct device * dev);
extern int  driver_attach(struct device_driver *drv);
extern int  device_reprobe(struct device *dev);

/*
 * Easy functions for dynamically creating devices on the fly
 */
extern struct device *device_create(struct class *cls, struct device *parent,
                                    dev_t devt, const char *fmt, ...)
                                    __attribute__((format(printf,4,5)));
extern void device_destroy(struct class *cls, dev_t devt);

/*
 * Platform "fixup" functions - allow the platform to have their say
 * about devices and actions that the general device layer doesn't
 * know about.
 */
/* Notify platform of device discovery */
extern int (*platform_notify)(struct device * dev);

extern int (*platform_notify_remove)(struct device * dev);


/**
 * get_device - atomically increment the reference count for the device.
 *
 */
extern struct device * get_device(struct device * dev);
extern void put_device(struct device * dev);


/* drivers/base/power/shutdown.c */
extern void device_shutdown(void);


/* debugging and troubleshooting/diagnostic helpers. */
extern const char *dev_driver_string(struct device *dev);

#if 1
#define dev_printk(level, dev, format, arg...)  \
        diag_printf("%s %s: " format , dev_driver_string(dev) , (dev)->bus_id , ## arg)
#else
#define dev_printk(level, dev, format, arg...)
#endif

#define dev_printk1(level, dev, format, arg...)  \
        diag_printf("%s %s: " format , dev_driver_string(dev) , (dev)->bus_id , ## arg)
#if 1
#define dev_dbg(dev, format, arg...)            \
        dev_printk1(KERN_DEBUG , dev , format , ## arg)
#else
static inline int __attribute__ ((format (printf, 2, 3)))
dev_dbg(struct device * dev, const char * fmt, ...)
{
        return 0;
}
#endif

#define dev_err(dev, format, arg...)            \
        dev_printk(KERN_ERR , dev , format , ## arg)
#define dev_info(dev, format, arg...)           \
        dev_printk(KERN_INFO , dev , format , ## arg)
#define dev_warn(dev, format, arg...)           \
        dev_printk(KERN_WARNING , dev , format , ## arg)
#define dev_notice(dev, format, arg...)         \
        dev_printk(KERN_NOTICE , dev , format , ## arg)

#define device_init_wakeup(dev,val) 
#define device_can_wakeup(dev) 1
#define device_may_wakeup(dev) 1
//static inline   int device_suspend(void){ return 0; } 

#endif /* _DEVICE_H_ */


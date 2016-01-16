/*
 *   This file has been modified by Evatronix. 
 */
 
extern int devices_init(void);
extern int buses_init(void);
extern int classes_init(void);
extern int firmware_init(void);
extern int platform_bus_init(void);

extern int bus_add_device(struct device * dev);
extern void bus_attach_device(struct device * dev);
extern void bus_remove_device(struct device * dev);
extern struct bus_type *get_bus(struct bus_type * bus);
extern void put_bus(struct bus_type * bus);
 
extern int bus_add_driver(struct device_driver *);
extern void bus_remove_driver(struct device_driver *);

static inline struct class_device *to_class_dev(struct kobject *obj)
{
	return container_of(obj, struct class_device, kobj);
}

extern struct kset devices_subsys;


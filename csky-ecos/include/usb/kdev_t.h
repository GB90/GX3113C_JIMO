/*
 *   This file has been modified by Evatronix. 
 */
#ifndef _LINUX_KDEV_T_H
#define _LINUX_KDEV_T_H

#define MINORBITS       20
#define MINORMASK       ((1U << MINORBITS) - 1)

#define MAJOR(dev)      ((unsigned int) ((dev) >> MINORBITS))
#define MINOR(dev)      ((unsigned int) ((dev) & MINORMASK))
#define MKDEV(ma,mi)    (((ma) << MINORBITS) | (mi))

#define print_dev_t(buffer, dev)                                        \
        sprintf((buffer), "%u:%u\n", MAJOR(dev), MINOR(dev))

#define format_dev_t(buffer, dev)                                       \
        ({                                                              \
                sprintf(buffer, "%u:%u", MAJOR(dev), MINOR(dev));       \
                buffer;                                                 \
        })


static inline int new_valid_dev(dev_t dev)
{
        return 1;
}

static inline u32 new_encode_dev(dev_t dev)
{
        unsigned major = MAJOR(dev);
        unsigned minor = MINOR(dev);
        return (minor & 0xff) | (major << 8) | ((minor & ~0xff) << 12);
}

static inline dev_t new_decode_dev(u32 dev)
{
        unsigned major = (dev & 0xfff00) >> 8;
        unsigned minor = (dev & 0xff) | ((dev >> 12) & 0xfff00);
        return MKDEV(major, minor);
}

 #endif
 

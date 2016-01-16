/*
 *   This file has been modified by Evatronix. 
 */
#ifndef _LINUX_INIT_H
#define _LINUX_INIT_H
 
/* These are for everybody (although not all archs will actually
   discard it in modules) */
#define __init         
#define __exit 

/*
 * Used for initialization calls..
 */
typedef int (*initcall_t)(void);
typedef void (*exitcall_t)(void);

#define __devinit __init

#endif /* _LINUX_INIT_H */

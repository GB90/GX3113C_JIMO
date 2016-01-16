//*******************************************************************
//
//  Copyright (c) 2007  Evatronix SA
//  This file is released under the GPLv2.
//
// *******************************************************************
//
// *******************************************************************
//  Project name        : USBHS-HB Software Stack
//  Project description : software stack for Evatronix USB Controler
//  File name           : include/mutex.h
//  File version        : FileName 001
//  File contents       : implementation wraper beetween Linux and Ecos mutex mechanism
//  Design Engineer     : Pawel Laszczak
//  Quality Engineer    : Pawel Laszczak
//  Last modification   : 5.02.2008
// *******************************************************************

#ifndef __LINUX_MUTEX_H
#define __LINUX_MUTEX_H

#include <linux/list.h>
#include <usb/linkage.h>
#include <usb/errno.h>
#include <usb/arch/atomic.h>

struct mutex
{
    cyg_atomic          locked;         /* true if locked               */
    cyg_thread          *owner;         /* Current locking thread       */
    cyg_threadqueue     queue;          /* Queue of waiting threads     */

#ifdef CYGSEM_KERNEL_SYNCH_MUTEX_PRIORITY_INVERSION_PROTOCOL_DYNAMIC
    cyg_mutex_protocol_t protocol;       /* this mutex's protocol        */
#endif    
#ifdef CYGSEM_KERNEL_SYNCH_MUTEX_PRIORITY_INVERSION_PROTOCOL_CEILING
    cyg_priority_t      ceiling;        /* mutex priority ceiling       */
#endif
};

# define mutex_init(mutex) \
		cyg_mutex_init((cyg_mutex_t*)mutex);

# define mutex_destroy(mutex)          cyg_mutex_destroy((cyg_mutex_t*)mutex);

#define NONE  0                       // no inversion protocol
#define INHERIT 1                        // priority inheritance protocol
#define CEILING 2

#if defined(CYGSEM_KERNEL_SYNCH_MUTEX_PRIORITY_INVERSION_PROTOCOL_DEFAULT) && \
    defined(CYGSEM_KERNEL_SYNCH_MUTEX_PRIORITY_INVERSION_PROTOCOL_DYNAMIC)

	#ifdef CYGSEM_KERNEL_SYNCH_MUTEX_PRIORITY_INVERSION_PROTOCOL_DEFAULT_INHERIT	
		#define value1	\
			, .protocol    = INHERIT										
	#endif
	#ifdef CYGSEM_KERNEL_SYNCH_MUTEX_PRIORITY_INVERSION_PROTOCOL_DEFAULT_CEILING
		#define value1	\
			, .protocol    = CEILING	\
			, .ceiling     = CYGSEM_KERNEL_SYNCH_MUTEX_PRIORITY_INVERSION_PROTOCOL_DEFAULT_PRIORITY
	#endif
	#ifdef CYGSEM_KERNEL_SYNCH_MUTEX_PRIORITY_INVERSION_PROTOCOL_DEFAULT_NONE
		#define value1	\
			, .protocol    = NONE;
	#endif
#else
	#ifdef CYGSEM_KERNEL_SYNCH_MUTEX_PRIORITY_INVERSION_PROTOCOL_CEILING    
		#ifdef CYGSEM_KERNEL_SYNCH_MUTEX_PRIORITY_INVERSION_PROTOCOL_DEFAULT_PRIORITY
			#define value1 \
				, .ceiling = CYGSEM_KERNEL_SYNCH_MUTEX_PRIORITY_INVERSION_PROTOCOL_DEFAULT_PRIORITY
		#else    
			#define value1 \
				, .ceiling = 0
		#endif
	#endif
#endif	
	
#define DEFINE_MUTEX(mutexname) \
	struct mutex mutexname = {	\
     .locked      = false	\
    , .owner       = NULL	\
    value1	\
	}


/********************************************************/
struct lock_class_key;		
extern void __mutex_init(struct mutex *lock, const char *name,
                         struct lock_class_key *key);


static inline void  mutex_lock(struct mutex *lock){
	 cyg_bool_t result;
//	 printk("owener mutex %lx\n", Cyg_Thread::self());
	 result = cyg_mutex_lock((cyg_mutex_t*)lock);
}

static inline int mutex_lock_interruptible(struct mutex *lock){
	 cyg_bool_t result;
	 result = cyg_mutex_lock((cyg_mutex_t*)lock);
	 if(result)
		return 0;
	 else
		return -EINTR;
}


# define mutex_lock_nested(lock, subclass) mutex_lock(lock)
# define mutex_lock_interruptible_nested(lock, subclass) mutex_lock_interruptible(lock)


/*
 * NOTE: mutex_trylock() follows the spin_trylock() convention,
 *       not the down_trylock() convention!
 */
static inline int fastcall mutex_trylock(struct mutex *lock){
	return cyg_mutex_trylock((cyg_mutex_t *) lock);
}

static inline void fastcall mutex_unlock(struct mutex *lock){
//	 printk("owener mutex %lx\n", Cyg_Thread::self());
	cyg_mutex_unlock((cyg_mutex_t*) lock);
}

#endif


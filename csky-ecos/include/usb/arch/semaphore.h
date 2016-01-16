//*******************************************************************
//
//  Copyright (c) 2007  Evatronix SA
//
// *******************************************************************
//
// *******************************************************************
//  Project name        : USBHS-HB Software Stack
//  Project description : software stack for Evatronix USB Controler
//  File name           : include\arm\semaphore.h
//  File version        : FileName 001
//  File contents       : wraper from ECOS semaphore to Linux semaphore
//  Design Engineer     : Pawel Laszczak
//  Quality Engineer    : Pawel Laszczak
//  Last modification   : 5.02.2008
// *******************************************************************
#ifndef _ARM_SEMAPHORE_H
#define _ARM_SEMAPHORE_H


#include <usb/linkage.h>
#include <usb/spinlock.h>
#include <usb/arch/system.h>

#include <usb/arch/atomic.h>
#include <cyg/kernel/kapi.h>

static inline void sema_init(cyg_sem_t *sem, int val)
{
	cyg_semaphore_init((cyg_sem_t*)sem, val);
}


static inline void init_MUTEX (cyg_sem_t *sem)
{
        sema_init(sem, 1);
}

static inline void init_MUTEX_LOCKED (cyg_sem_t *sem)
{
        sema_init(sem, 0);
}

static inline void down(cyg_sem_t *sem) {
	cyg_semaphore_wait((cyg_sem_t*)sem);
}

static inline int down_interruptible(cyg_sem_t *sem){
	return cyg_semaphore_wait((cyg_sem_t*)sem);
}

static inline void up(cyg_sem_t *sem){
	cyg_semaphore_post((cyg_sem_t*)sem);
}

static inline int down_trylock(cyg_sem_t *sem){
	return cyg_semaphore_trywait((cyg_sem_t*)sem);
}

#endif


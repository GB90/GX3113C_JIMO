/*
 *   This file has been modified by Evatronix. 
 */
#ifndef _LINUX_JIFFIES_H
#define _LINUX_JIFFIES_H

/*
 *	These inlines deal with timer wrapping correctly. You are 
 *	strongly encouraged to use them
 *	1. Because people otherwise forget
 *	2. Because if the timer wrap changes in future you won't have to
 *	   alter your driver code.
 *
 * time_after(a,b) returns true if the time a is after time b.
 *
 * Do this with "<0" and ">=0" to only test the sign of the result. A
 * good compiler would generate better code (and a really good compiler
 * wouldn't care). Gcc is currently neither.
 */
#define time_after(a,b)	 ((sizeof(a)==4) && (sizeof(b)==4 &&  ((long)(b) - (long)(a) < 0))
#define time_before(a,b)	time_after(b,a)

#define time_after_eq(a,b)	(sizeof(a)==4 &&  sizeof(b)==4 && ((long)(a) - (long)(b) >= 0))
#define time_before_eq(a,b)	time_after_eq(b,a)

#define  msecs_to_jiffies(x) (x/10)
#endif

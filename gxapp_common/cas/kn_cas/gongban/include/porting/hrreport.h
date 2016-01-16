#ifndef	_HRREPORT_H_
#define	_HRREPORT_H_

//#include "stddefs.h"

#define REPORTFUNCPOSTFIX 

#define	SERIAL_PRINT(x)	hcncas_debug x

extern char	hcncas_debug(char *fmt, ...)REPORTFUNCPOSTFIX;

#endif


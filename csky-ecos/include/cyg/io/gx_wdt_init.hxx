#ifndef	GX3110_WDT_INIT_H
#define	GX3110_WDT_INIT_H

extern "C" void gx3110_wdt_mod_init(void);

class cyg_wdt_init_class {		//smart card
public:
   cyg_wdt_init_class(void) 
	{ 
        gx3110_wdt_mod_init();
	}
};

#endif	

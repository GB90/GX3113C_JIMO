#ifndef	GX3110_SCI_INIT_H
#define	GX3110_SCI_INIT_H

extern "C" void gx3110_sci_mod_init(void);

class cyg_sci_init_class {		//smart card
public:
   cyg_sci_init_class(void) 
	{ 
        gx3110_sci_mod_init();
	}
};

#endif	


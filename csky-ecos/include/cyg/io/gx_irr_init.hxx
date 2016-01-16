#ifndef	GX3110_IRR_INIT_H
#define	GX3110_IRR_INIT_H

#ifdef __cplusplus
extern "C" void gx3110_irr_mod_init(void);

class cyg_irr_init_class {		//smart card
public:
   cyg_irr_init_class(void) 
	{ 
        gx3110_irr_mod_init();
	}
};


#endif
#endif	

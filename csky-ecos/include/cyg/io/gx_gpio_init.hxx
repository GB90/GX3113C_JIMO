#ifndef	GX3110_GPIO_INIT_H
#define	GX3110_GPIO_INIT_H

#ifdef __cplusplus
extern "C" void gx3110_gpio_mod_init(void);

class cyg_gpio_init_class {		//smart card
public:
   cyg_gpio_init_class(void) 
	{ 
        gx3110_gpio_mod_init();
	}
};


#endif
#endif	

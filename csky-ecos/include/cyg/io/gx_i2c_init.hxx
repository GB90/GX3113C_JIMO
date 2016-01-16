#ifndef	GX3110_HDMI_INIT_H
#define	GX3110_HDMI_INIT_H

#ifdef __cplusplus
extern "C" void gxi2c_mod_init(void);

class cyg_i2c_init_class {
public:
   cyg_i2c_init_class(void) 
	{ 
        gxi2c_mod_init();
	}
};


#endif
#endif	

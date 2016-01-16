#ifndef GX_DEV_SPI_INIT_HXX
#define GX_DEV_SPI_INIT_HXX
/************************************************************************************															
	System Name	:	NationalChip													
	File   Name	:	gx3110_sd_init.h												
	Revision	:	1.00															
	Date		:	2009.11.22	
    author      :   gaohaibo
	usage       :   #include <cyg/io/gx_dev_spi_init.hxx>
************************************************************************************/
#include <cyg/infra/diag.h>            // HAL polled output
extern "C" void gx_devs_spi_init(void);
extern "C" void gx_spiflash_init(void);
class cyg_spi_gx3110_init_class {
public:
  cyg_spi_gx3110_init_class(void) {
    cyg_uint32 chip_type;
	chip_type = gx_chip_probe();
		switch(chip_type){
			case 0x3001:
			case 0x3110:
			case 0x6107:
			case 0x3200:
			case 0x3201:
			case 0x3211:
			case 0x6131:
			case 0x3130:
			case 0x6211:
			  	gx_devs_spi_init();
				gx_spiflash_init();
				break;
			default:
				diag_printf("spi flash not support GX%x now\n",chip_type);
				break;
	  	}
  	}
};
//static cyg_spi_gx3110_init_class spi_gx3110_init 
#endif   

#ifndef EXFATFS_INIT_HXX
#define EXFATFS_INIT_HXX
extern "C" void exfatfs_register(void);

class exfatfs_init_class {
public:
	exfatfs_init_class(void)
	{
	   exfatfs_register();
	}
};

#endif


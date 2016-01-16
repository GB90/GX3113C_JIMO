#ifndef	GXNETDEV_INIT_H
#define	GXNETDEV_INIT_H
#include <cyg/kernel/kapi.h>
#include <cyg/infra/diag.h>
#include <sys/param.h>

extern "C" void gxnetdev_init(void);
extern "C" void cyg_net_init(void);
extern "C" void net_io_mod_init(void);

/*--------used now--------*/
extern "C" void interfaces_net_init(void);
extern "C" void devs_init(void);
extern "C" void domain_init(void);
extern "C" void if_loop_init(void);
extern "C" void route_init_x(void);
extern "C" void route_init_rtsock(void);
extern "C" void inet_init(void);
extern "C" void mbuf_init(void);

/*--------unused now--------*/
extern "C" void netinet6init2_init(void);
extern "C" void sysctl_init(void);
#ifdef __FreeBSD__
extern "C" void key_init(void);
extern "C" void inet6_init(void);
#endif
extern "C" void if_ppp_init(void);

extern "C" unsigned int hal_uncached_map(unsigned int addr);
extern "C" void* malloc(size_t size);

class cyg_gxnetdev_init_class {		//smart card
public:
	cyg_gxnetdev_init_class(void) {
		mbuf_init();
		devs_init();
		if_loop_init();
		interfaces_net_init();
		domain_init();
		inet_init();
		route_init_rtsock();
		route_init_x();

		net_io_mod_init();
		cyg_net_init();
	}
};

#endif


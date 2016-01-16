/************************************************************ 
Copyright (C), 2007-2015, GX S&T Co., Ltd. 
FileName   :	debug_Service.c
Author     : 	zhangling
Version    : 	1.0
Date	   :
Description:	
Version    :	
History    :	
Date				Author  	Modification 
2015.06.15     zhangling		 create
***********************************************************/

#ifndef __GXDEBUG_H__
#define __GXDEBUG_H__

__BEGIN_DECLS

/*以下控制可以 GXBUS_DEBUG_WATCH_HEAP | GXBUS_DEBUG_WATCH_CODE
 * 默认GXBUS_DEBUG_WATCH_CODE*/
#define GXBUS_DEBUG_WATCH_OFF (0)
#define GXBUS_DEBUG_WATCH_HEAP (1)
#define GXBUS_DEBUG_WATCH_CODE (2)

#define GXBUS_DEBUG_CTR "gxdebug_ctr"///<控制监控内容

__END_DECLS

#endif				/* __DEBUG_SERVICE_H__ */
/*@}*/


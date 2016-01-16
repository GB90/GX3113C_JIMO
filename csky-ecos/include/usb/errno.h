/*
 *   This file has been modified by Evatronix. 
 */
#ifndef _LINUX_ERRNO_H
#define _LINUX_ERRNO_H
#include <errno.h>       /* for Cyg_ErrNo */

/*
#undef EPERM  
#undef ENOENT 
#undef EINTR  
#undef EIO    
#undef ENXIO  
#undef E2BIG  
#undef ENOEXEC
#undef EAGAIN 
#undef ENOMEM 
#undef EFAULT  
#undef EBUSY   
#undef EXDEV   
#undef ENODEV  
#undef EISDIR  
#undef EINVAL  
#undef EPIPE   

#undef ENOSYS         
#undef ENOMSG         
#undef EIDRM          
#undef EPROTO         
#undef EILSEQ         
#undef EMSGSIZE       
#undef EOPNOTSUPP     
#undef ECONNRESET     
#undef ENOTCONN       
#undef ESHUTDOWN      
#undef ETIMEDOUT       
#undef EHOSTUNREACH    
#undef EALREADY       
#undef EINPROGRESS     
#undef EREMOTEIO       
#undef EDQUOT          
*/
#if 1
//#define EPERM            1      /* Operation not permitted */
//#define ENOENT           2      /* No such file or directory */
//#define EINTR            4      /* Interrupted system call */
//#define EIO              5      /* I/O error */
//#define ENXIO            6      /* No such device or address */
#define E2BIG            7      /* Argument list too long */
//#define EAGAIN          11      /* Try again */
//#define ENOMEM          12      /* Out of memory */
#define EFAULT          14      /* Bad address */
//#define EBUSY           16      /* Device or resource busy */
//#define EXDEV           18      /* Cross-device link */
//#define ENODEV          19      /* No such device */
//#define EISDIR          21      /* Is a directory */
//#define EINVAL          22      /* Invalid argument */
//#define EPIPE           32      /* Broken pipe */
//#define ENOSYS          38      /* Function not implemented */
#define ENOMSG          42      /* No message of desired type */
#define EIDRM           43      /* Identifier removed */
#define EPROTO          71      /* Protocol error */
#define EILSEQ          84      /* Illegal byte sequence */
//#define EMSGSIZE        90      /* Message too long */
//#define EOPNOTSUPP      95      /* Operation not supported on transport endpoint */
//#define ECONNRESET      104     /* Connection reset by peer */
//#define ENOTCONN        107     /* Transport endpoint is not connected */
//#define ESHUTDOWN       108     /* Cannot send after transport endpoint shutdown */
//#define ETIMEDOUT       110     /* Connection timed out */
//#define EHOSTUNREACH    113     /* No route to host */
//#define EALREADY        114     /* Operation already in progress */
//#define EINPROGRESS     115     /* Operation now in progress */
#define EREMOTEIO       121     /* Remote I/O error */
//#define EDQUOT          122     /* Quota exceeded */
#endif
#endif


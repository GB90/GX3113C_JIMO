#ifndef CYGONCE_HAL_HAL_ARCH_H
#define CYGONCE_HAL_HAL_ARCH_H

#ifndef __ASSEMBLER__
#include <pkgconf/hal.h>
#include <cyg/infra/cyg_type.h>

#include <cyg/hal/ck610m_arch.h>
//--------------------------------------------------------------------------
// Processor saved states:
// The layout of this structure is also defined in "arch.inc", for assembly
// code. Do not change this without changing that (or vice versa).
// Notes: This structure is carefully laid out. It is a multiple of 8
// bytes and the pc and badvr fields are positioned to ensure that
// they are on 8 byte boundaries. 

#ifdef CYGHWR_HAL_CK_64BIT
# define CYG_HAL_CK_REG CYG_WORD64
# define CYG_HAL_CK_REG_SIZE 8
#else
# define CYG_HAL_CK_REG CYG_WORD32
# define CYG_HAL_CK_REG_SIZE 4
#endif

#if defined(CYGHWR_HAL_CK_FPU)
# if defined(CYGHWR_HAL_CK_FPU_64BIT)
#  define CYG_HAL_FPU_REG CYG_WORD64
# elif defined(CYGHWR_HAL_CK_FPU_32BIT)
#  define CYG_HAL_FPU_REG CYG_WORD32
# else
# error CK FPU register size not defined
# endif
#endif

typedef struct 
{
    // These are common to all saved states
    CYG_HAL_CK_REG    d[16];          /* Data regs                    */
    CYG_HAL_CK_REG    hi;             /* hi word of mpy/div reg       */
    CYG_HAL_CK_REG    lo;             /* lo word of mpy/div reg       */

    CYG_HAL_CK_REG    pc;             /* Program Counter              */
    CYG_WORD32          epc;             /* Status Reg                   */
    CYG_WORD32          psr;            /* Vector number                */
    CYG_WORD32          temp;           /*Îª8×Ö½Ú¶ÔÆë   lubin 070607*/
//    CYG_WORD32          HAL_SavedRegisterspc;            /*                 */
//    CYG_WORD32          epsr;           /*                   */
} HAL_SavedRegisters;

//--------------------------------------------------------------------------
// Exception handling function.
// This function is defined by the kernel according to this prototype. It is
// invoked from the HAL to deal with any CPU exceptions that the HAL does
// not want to deal with itself. It usually invokes the kernel's exception
// delivery mechanism.

externC void cyg_hal_deliver_exception( CYG_WORD code, CYG_ADDRWORD data );

//--------------------------------------------------------------------------
// Bit manipulation macros

#ifndef CYGHWR_HAL_BIT_INDEXES_DEFINED

externC cyg_uint32 hal_lsbit_index(cyg_uint32 mask);
externC cyg_uint32 hal_msbit_index(cyg_uint32 mask);

#define HAL_LSBIT_INDEX(index, mask) index = hal_lsbit_index(mask);

#define HAL_MSBIT_INDEX(index, mask) index = hal_msbit_index(mask);

#endif

#define HAL_DEFAULT_ISR hal_default_isr

// Initialize the context of a thread.
// Arguments:
// _sparg_ name of variable containing current sp, will be written with new sp
// _thread_ thread object address, passed as argument to entry point
// _entry_ entry point address.
// _id_ bit pattern used in initializing registers, for debugging.
#define HAL_THREAD_INIT_CONTEXT( _sparg_, _thread_, _entry_, _id_ )            \
{   HAL_SavedRegisters *_regs_;                                      \
    int _i_;                                                                  \
    _regs_ = (HAL_SavedRegisters *)(((_sparg_) - sizeof(HAL_SavedRegisters))&0xFFFFFFF0);  \
    for( _i_ = 0; _i_ < 15; _i_++ ) (_regs_)->d[_i_] = (_id_)|_i_;            \
    (_regs_)->d[0] = (CYG_WORD)(_regs_);       /* SP = top of stack      */   \
    (_regs_)->d[02] = (CYG_WORD)(_thread_);    /* R4 = arg1 = thread ptr */   \
    (_regs_)->lo = 0;                          /* lo = 0               */     \
    (_regs_)->hi = 0;                          /* hi = 0               */     \
    (_regs_)->temp = 0;                          /*add temp by lubin 20070606   */     \
    (_regs_)->d[15] = (CYG_WORD)(_entry_);      /* r15 = entry point       */   \
    (_regs_)->psr = 0x80000150;                         /* PSR = enable interrupts */   \
    (_regs_)->pc = (CYG_WORD)(_entry_);        /* set PC for thread dbg  */   \
    _sparg_ = (CYG_ADDRESS)_regs_;                                           }

//--------------------------------------------------------------------------
// Context switch macros.
// The arguments are pointers to locations where the stack pointer
// of the current thread is to be stored, and from where the sp of the
// next thread is to be fetched.

externC void hal_thread_switch_context( CYG_ADDRESS to, CYG_ADDRESS from );
externC void hal_thread_load_context( CYG_ADDRESS to )
    __attribute__ ((noreturn));

#define HAL_THREAD_SWITCH_CONTEXT(_fspptr_,_tspptr_)                    \
        hal_thread_switch_context( (CYG_ADDRESS)_tspptr_,               \
                                   (CYG_ADDRESS)_fspptr_);

#define HAL_THREAD_LOAD_CONTEXT(_tspptr_)                               \
        hal_thread_load_context( (CYG_ADDRESS)_tspptr_ );

//--------------------------------------------------------------------------
// Execution reorder barrier.
// When optimizing the compiler can reorder code. In multithreaded systems
// where the order of actions is vital, this can sometimes cause problems.
// This macro may be inserted into places where reordering should not happen.
// The "memory" keyword is potentially unnecessary, but it is harmless to
// keep it.

#define HAL_REORDER_BARRIER() asm volatile ( "" : : : "memory" )

//--------------------------------------------------------------------------
// Breakpoint support
// HAL_BREAKPOINT() is a code sequence that will cause a breakpoint to
// happen if executed.
// HAL_BREAKINST is the value of the breakpoint instruction and
// HAL_BREAKINST_SIZE is its size in bytes.
// HAL_BREAKINST_TYPE is the type.

#define HAL_BREAKPOINT(_label_)                 \
asm volatile (" .globl  " #_label_ ";"          \
              #_label_":"                       \
              " break   5"                      \
    );

#define HAL_BREAKINST           0x0005000d

#define HAL_BREAKINST_SIZE      4

#define HAL_BREAKINST_TYPE      cyg_uint32

//--------------------------------------------------------------------------
// Thread register state manipulation for GDB support.

// Default to a 32 bit register size for GDB register dumps.
#ifndef CYG_HAL_GDB_REG
#define CYG_HAL_GDB_REG CYG_WORD32
#endif

// Translate a stack pointer as saved by the thread context macros above into
// a pointer to a HAL_SavedRegisters structure.
#define HAL_THREAD_GET_SAVED_REGISTERS( _sp_, _regs_ )          \
        (_regs_) = (HAL_SavedRegisters *)(_sp_)




typedef struct {
    cyg_uint32 r0;     
    cyg_uint32 r1;    
    cyg_uint32 r2;
    cyg_uint32 r3;
	cyg_uint32 r4;
	cyg_uint32 r5;
	cyg_uint32 r6;
	cyg_uint32 r7;
	cyg_uint32 r8;
	cyg_uint32 r9;
    cyg_uint32 r10;
    cyg_uint32 r11;
    cyg_uint32 r12;
    cyg_uint32 r13;
    cyg_uint32 r14;
	cyg_uint32 r15;
} hal_jmp_buf_t;

#define CYGARC_JMP_BUF_SIZE      (sizeof(hal_jmp_buf_t) / sizeof(cyg_uint32))

typedef cyg_uint32 hal_jmp_buf[ CYGARC_JMP_BUF_SIZE ];

externC int hal_setjmp(hal_jmp_buf env);
externC void hal_longjmp(hal_jmp_buf env, int val);

//-------------------------------------------------------------------------
// Idle thread code.
// This macro is called in the idle thread loop, and gives the HAL the
// chance to insert code. Typical idle thread behaviour might be to halt the
// processor.

externC void hal_idle_thread_action(cyg_uint32 loop_count);

#define HAL_IDLE_THREAD_ACTION(_count_) hal_idle_thread_action(_count_)

//--------------------------------------------------------------------------
// Minimal and sensible stack sizes: the intention is that applications
// will use these to provide a stack size in the first instance prior to
// proper analysis.  Idle thread stack should be this big.

//    THESE ARE NOT INTENDED TO BE MICROMETRICALLY ACCURATE FIGURES.
//           THEY ARE HOWEVER ENOUGH TO START PROGRAMMING.
// YOU MUST MAKE YOUR STACKS LARGER IF YOU HAVE LARGE "AUTO" VARIABLES!

// This is not a config option because it should not be adjusted except
// under "enough rope" sort of disclaimers.

// Typical case stack frame size: return link + 4 pushed registers + some locals.
#define CYGNUM_HAL_STACK_FRAME_SIZE (48)

// Stack needed for a context switch:
#if defined(CYGHWR_HAL_CK_FPU)
# if defined(CYGHWR_HAL_CK_FPU_64BIT)
#define CYGNUM_HAL_STACK_CONTEXT_SIZE (((32+12)*CYG_HAL_CK_REG_SIZE)+(32*8))
# elif defined(CYGHWR_HAL_CK_FPU_32BIT)
#define CYGNUM_HAL_STACK_CONTEXT_SIZE (((32+12)*CYG_HAL_CK_REG_SIZE)+(32*4))
# else
# error CK FPU register size not defined
# endif
#else
#define CYGNUM_HAL_STACK_CONTEXT_SIZE ((32+10)*CYG_HAL_CK_REG_SIZE)
#endif



// Interrupt + call to ISR, interrupt_end() and the DSR
#define CYGNUM_HAL_STACK_INTERRUPT_SIZE (4+2*CYGNUM_HAL_STACK_CONTEXT_SIZE) 

#ifdef CYGIMP_HAL_COMMON_INTERRUPTS_USE_INTERRUPT_STACK

// An interrupt stack which is large enough for all possible interrupt
// conditions (and only used for that purpose) exists.  "User" stacks
// can be much smaller

#define CYGNUM_HAL_STACK_SIZE_MINIMUM (CYGNUM_HAL_STACK_CONTEXT_SIZE+      \
                                       CYGNUM_HAL_STACK_INTERRUPT_SIZE*2+  \
                                       CYGNUM_HAL_STACK_FRAME_SIZE*8)
#define CYGNUM_HAL_STACK_SIZE_TYPICAL (CYGNUM_HAL_STACK_SIZE_MINIMUM+1024)

#else // CYGIMP_HAL_COMMON_INTERRUPTS_USE_INTERRUPT_STACK 

// No separate interrupt stack exists.  Make sure all threads contain
// a stack sufficiently large.

#define CYGNUM_HAL_STACK_SIZE_MINIMUM (4096)
#define CYGNUM_HAL_STACK_SIZE_TYPICAL (4096)

#endif

#endif /* __ASSEMBLER__ */

// Convenience macros for accessing memory cached or uncached
#define CYGARC_KSEG_MASK                               (0xE0000000)
#define CYGARC_KSEG_CACHED                             (0x80000000)
#define CYGARC_KSEG_UNCACHED                           (0xA0000000)
#define CYGARC_KSEG_CACHED_BASE                        (0x80000000)
#define CYGARC_KSEG_UNCACHED_BASE                      (0xA0000000)
#ifndef __ASSEMBLER__
#define CYGARC_CACHED_ADDRESS(x)                       ((((CYG_ADDRESS)(x)) & ~CYGARC_KSEG_MASK) | CYGARC_KSEG_CACHED)
#define CYGARC_UNCACHED_ADDRESS(x)                     ((((CYG_ADDRESS)(x)) & ~CYGARC_KSEG_MASK) | CYGARC_KSEG_UNCACHED)
#define CYGARC_PHYSICAL_ADDRESS(x)                     (((CYG_ADDRESS)(x)) & ~CYGARC_KSEG_MASK)
#else // __ASSEMBLER__
#define CYGARC_CACHED_ADDRESS(x)                       ((((x)) & ~CYGARC_KSEG_MASK) | CYGARC_KSEG_CACHED)
#define CYGARC_UNCACHED_ADDRESS(x)                     ((((x)) & ~CYGARC_KSEG_MASK) | CYGARC_KSEG_UNCACHED)
#define CYGARC_PHYSICAL_ADDRESS(x)                     (((x)) & ~CYGARC_KSEG_MASK)
#define CYGARC_ADDRESS_REG_CACHED(reg)                 \
        and     reg, reg, ~CYGARC_KSEG_MASK;           \
        or      reg, reg, CYGARC_KSEG_CACHED
#define CYGARC_ADDRESS_REG_UNCACHED(reg)               \
        and     reg, reg, ~CYGARC_KSEG_MASK;           \
        or      reg, reg, CYGARC_KSEG_UNCACHED
#endif /* __ASSEMBLER__ */

//--------------------------------------------------------------------------
// Macros for switching context between two eCos instances (jump from
// code in ROM to code in RAM or vice versa).
#define CYGARC_HAL_SAVE_GP() 
#define CYGARC_HAL_RESTORE_GP()

//--------------------------------------------------------------------------
// Macro for finding return address. 
#define CYGARC_HAL_GET_RETURN_ADDRESS(_x_, _dummy_)

#define CYGARC_HAL_GET_RETURN_ADDRESS_BACKUP(_dummy_)

//--------------------------------------------------------------------------
#endif // CYGONCE_HAL_HAL_ARCH_H
// End of hal_arch.h

#ifndef CYGONCE_HAL_ARCH_INC
#define CYGONCE_HAL_ARCH_INC

#include <cyg/hal/ck.inc>
	
#include <cyg/hal/ck610m.inc>

#ifndef INITIAL_SR

# ifndef INITIAL_SR_VAR
#  define INITIAL_SR_VAR 0x00000000
# endif
# ifndef INITIAL_SR_PLF
#  define INITIAL_SR_PLF 0x00000000
# endif

#if defined(CYG_HAL_STARTUP_RAM) || defined(CYG_HAL_STARTUP_ROMRAM)
# if defined(CYGPKG_HAL_CK_SIM) || !defined(CYGSEM_HAL_USE_ROM_MONITOR)
#  define INITIAL_SR_ARCH 	0x1000ff00 /* CP0 usable, Ints enabled, master interrupt disable */
# else
#  define INITIAL_SR_ARCH 	0x1040ff00	/* as above + ROM vectors used	*/
# endif	
#elif defined(CYG_HAL_STARTUP_ROM)
# define INITIAL_SR_ARCH	0x1040ff00	/* as above + ROM vectors used	*/
#endif

#define INITIAL_SR 	(INITIAL_SR_ARCH|INITIAL_SR_VAR|INITIAL_SR_PLF)

#endif

##-----------------------------------------------------------------------------
## Setup the initial value for the config0 register

#ifndef INITIAL_CONFIG0

#define	INITIAL_CONFIG0	0x00000002

#endif

##-----------------------------------------------------------------------------
## CK thread and interrupt saved state. This must match the layout of the
## HAL_SavedRegisters in hal_arch.h. Do not change this without changing the
## layout there, or viceversa.	

# Size of registers that change size between 32 and 64 bit implementations
#ifdef CYGHWR_HAL_CK_64BIT
# define ck_regsize 	8
#else
# define ck_regsize 	4
#endif

# Size of registers that stay the same size in all implementations
# define ck_regsize32	4

# Size of FPU registers.
#if defined(CYGHWR_HAL_CK_FPU)
# if defined(CYGHWR_HAL_CK_FPU_64BIT)
#  define ck_fpuregsize 8
# elif defined(CYGHWR_HAL_CK_FPU_32BIT)
#  define ck_fpuregsize 4
# else
#  error CK FPU register size not defined
# endif
#endif


#define	ckreg_regs	0
#define	ckreg_hi	(ck_regsize*32)
#define	ckreg_lo	(ckreg_hi+ck_regsize)
#ifdef CYGHWR_HAL_CK_FPU
# define ckreg_fpureg	(ckreg_lo+ck_regsize)
# define ckreg_fcr31	(ckreg_fpureg+(ck_fpuregsize*32))
# define ckreg_fppad  (ckreg_fcr31+ck_regsize32)
# define ckreg_vector	(ckreg_fppad+ck_regsize32)
#else
# define ckreg_vector	(ckreg_lo+ck_regsize)
#endif
#define	ckreg_sr	(ckreg_vector+ck_regsize32)
#define	ckreg_pc	(ckreg_sr+ck_regsize32)
#define	ckreg_cachectrl (ckreg_pc+ck_regsize)
#define	ckreg_cause	(ckreg_cachectrl+ck_regsize32)
#define	ckreg_badvr	(ckreg_cause+ck_regsize32)
#define	ckreg_prid	(ckreg_badvr+ck_regsize)
#define	ckreg_config	(ckreg_prid+ck_regsize32)
#define	ckreg_size	(ckreg_config+ck_regsize32)

#define ck_exception_decrement	((ckreg_size*2)&~0xF)

##-----------------------------------------------------------------------------
## Minimal stack frame size uses to call functions from asm.
	
#define ck_stack_frame_size		32	// 4 (64 bit) args worth

##-----------------------------------------------------------------------------
## Load Address and Relocate. This macro is used in code that may be linked
## to execute out of RAM but is actually executed from ROM. If that is the
## case a suitable version of this macro will have been defined elsewhere.
## This is just a default version for use when that does not happen.

#ifndef CYGPKG_HAL_CK_LAR_DEFINED

	.macro	lar	reg,addr
	la	\reg,\addr
	.endm

#define CYGPKG_HAL_CK_LAR_DEFINED

#endif
	
	# Initialize CPU
	.macro	hal_cpu_init

	# Initialize/clear watchpoint registers
	mvatc0	zero, C0_WATCHLO
	nop
	nop
	nop
	mtc0	zero, C0_WATCHHI 
	nop
	nop
	nop

	mtc0	zero,cause		# zero cause reg
	nop
#if !defined(CYGSEM_HAL_USE_ROM_MONITOR)
	la	v0,INITIAL_SR 		# initialize status register
	mtc0	v0,status
	nop
	nop
	nop
	la	v0,INITIAL_CONFIG0
	mtc0	v0,config0
	nop
	nop
	nop
#endif
	.endm

	# Enable interrupts
#ifdef CYG_HAL_CK_R3900	
	.macro hal_cpu_int_enable
	mfc0	v0,status
	nop
	nop
	ori	v0,v0,0x0001		# set IE bit
	mtc0	v0,status
	nop
	nop
	nop
	.endm		
#else
	.macro hal_cpu_int_enable	
	mfc0	v0,status
	la	v1,0xFFFFFFF9
	and	v0,v0,v1		# clear EXL and ERL bits	
	ori	v0,v0,0x0001		# set IE bit
	mtc0	v0,status
	nop
	nop
	nop	
	.endm	
#endif		


	# Disable interrupts
	.macro hal_cpu_int_disable
	mfc0	v0,status
	la	v1,0xFFFFFFFE
	and	v0,v0,v1
	mtc0	v0,status
	nop
	nop
	nop
	.endm	

	# Merge the interrupt enable state of the status register in
	# \sr with the current sr.
#ifdef CYG_HAL_CK_R3900
#define HAL_SR_INT_MASK	0x00000001		// IEc only
#else
#define HAL_SR_INT_MASK	0x00000007		// IE, EXL, ERL
#endif			
	.macro	hal_cpu_int_merge sr
	mfc0	v0,status			# V0 = current SR
	la	v1,HAL_SR_INT_MASK		# V1 = SR interrupt bits mask
	and	\sr,\sr,v1			# Isolate interrupt bits of \sr
	nor	v1,v1,zero			# Invert mask
	and	v0,v0,v1			# V0 = current SR except int bits
	or	v0,v0,\sr			# V0 = New SR
	mtc0	v0,status			# Return to SR
	.endm

	# Enable further exception processing, and disable
	# interrupt processing.
#ifdef CYG_HAL_CK_R3900	
	.macro hal_cpu_except_enable
	hal_cpu_int_disable
	.endm		
#else
	.macro hal_cpu_except_enable
	mfc0	v0,status
	la	v1,0xFFFFFFF0
	and	v0,v0,v1		# clear EXL, ERL and IE bits	
	mtc0	v0,status
	nop
	nop
	nop
	.endm
#endif		
	
	# Return from exception.
#ifdef CYG_HAL_CK_R3900
	.macro	hal_cpu_eret pc,sr
	mtc0	\sr,status			# Load status register
	nop
	nop
	nop
	sync					# settle things down
	jr	\pc				# jump back to interrupted code
	rfe					# restore state	(delay slot)	
	.endm
#else
	.macro	hal_cpu_eret pc,sr
        .set ck3
	ori     \sr,\sr,2                       # prevent interrupts until eret
	mtc0	\sr,status			# put SR back
	nop
	nop
	nop
	mvatc0	\pc,epc				# put PC in EPC
	nop
	nop
	nop
	sync					# settle things down
	eret					# return
	nop					# just to be safe
        .set ck0
	.endm
#endif	
	
##-----------------------------------------------------------------------------
# Default CK interrupt decoding macros. This uses the basic interrupt
# support provided by CP0 in the cause and status registers. If there is
# a more complex external interrupt controller, or the default stuff is
# interpreted differently (as in the TX3904) then these macros will be 
# overridden and CYGPKG_HAL_CK_INTC_DEFINED will be defined.

#ifndef CYGPKG_HAL_CK_INTC_DEFINED

#ifndef CYGPKG_HAL_CK_INTC_INIT_DEFINED
	# initialize all interrupts to disabled
	.macro	hal_intc_init
	mfc0	v0,status
	nop
	lui	v1,0xFFFF
	ori	v1,v1,0x00FF
	and	v0,v0,v1		# clear the IntMask bits
	mtc0	v0,status
	nop
	nop
	nop
	.endm
#endif

#ifndef CYGPKG_HAL_CK_INTC_DECODE_DEFINED
	.macro	hal_intc_decode vnum
	mfc0	v1,status		# get status register (interrupt mask)
	nop				# delay slot
	mfc0	v0,cause		# get cause register
	nop				# delay slot
	and	v0,v0,v1		# apply interrupt mask
	srl	v0,v0,10		# shift interrupt bits down
	andi	v0,v0,0x3f		# isolate 6 interrupt bits
	la	v1,hal_intc_translation_table
	add	v0,v0,v1		# index into table
	lb	\vnum,0(v0)		# pick up vector number
	.endm
#endif

#ifndef CYGPKG_HAL_CK_INTC_TRANSLATE_DEFINED
#ifdef CYGIMP_HAL_COMMON_INTERRUPTS_CHAIN
	.macro	hal_intc_translate inum,vnum
	move	\vnum,zero			# Just vector zero is supported
	.endm
#else			
	.macro	hal_intc_translate inum,vnum
	move	\vnum,\inum			# Vector == interrupt number
	.endm
#endif
#endif

	.macro	hal_intc_decode_data
hal_intc_translation_table:	
	.byte	0, 0, 1, 0
	.byte	2, 0, 1, 0
	.byte	3, 0, 1, 0
	.byte	2, 0, 1, 0
	.byte	4, 0, 1, 0
	.byte	2, 0, 1, 0
	.byte	3, 0, 1, 0
	.byte	2, 0, 1, 0
	.byte	5, 0, 1, 0
	.byte	2, 0, 1, 0
	.byte	3, 0, 1, 0
	.byte	2, 0, 1, 0
	.byte	4, 0, 1, 0
	.byte	2, 0, 1, 0
	.byte	3, 0, 1, 0
	.byte	2, 0, 1, 0
	.endm
#endif

#------------------------------------------------------------------------------
# Register save and restore macros. These expect a pointer to a CPU save state
# area in the register \ptr. The GPR indicated by \reg will be saved into its
# slot in that structure.

#ifdef CYGHWR_HAL_CK_64BIT

	.macro sgpr reg,ptr
	sd	$\reg,(ckreg_regs+\reg*ck_regsize)(\ptr)
	.endm

	.macro lgpr reg,ptr
	ld	$\reg,(ckreg_regs+\reg*ck_regsize)(\ptr)
	.endm

	.macro slo reg,ptr
	sd	\reg,(ckreg_lo)(\ptr)
	.endm

	.macro shi reg,ptr
	sd	\reg,(ckreg_hi)(\ptr)
	.endm

	.macro llo reg,ptr
	ld	\reg,(ckreg_lo)(\ptr)
	.endm

	.macro lhi reg,ptr
	ld	\reg,(ckreg_hi)(\ptr)
	.endm

	.macro ssp reg,ptr
	sd	\reg,(ckreg_regs+29*ck_regsize)(\ptr)
	.endm

	.macro lsp reg,ptr
	ld	\reg,(ckreg_regs+29*ck_regsize)(\ptr)
	.endm

	.macro sva reg,val
	sd	\reg,\val
	.endm

	.macro lva reg,val
	ld	\reg,\val
	.endm

	.macro mvafc0 gpr,cpr
	dmfc0	\gpr,\cpr
	.endm

	.macro mvatc0 gpr,cpr
	dmtc0	\gpr,\cpr
	.endm

	.macro lpc reg,ptr
	ld	\reg,(ckreg_pc)(\ptr)
	.endm

	.macro spc reg,ptr
	sd	\reg,(ckreg_pc)(\ptr)
	.endm

#else

	.macro sgpr reg,ptr
	sw	$\reg,(ckreg_regs+\reg*ck_regsize)(\ptr)
	.endm

	.macro lgpr reg,ptr
	lw	$\reg,(ckreg_regs+\reg*ck_regsize)(\ptr)
	.endm

	.macro slo reg,ptr
	sw	\reg,(ckreg_lo)(\ptr)
	.endm

	.macro shi reg,ptr
	sw	\reg,(ckreg_hi)(\ptr)
	.endm

	.macro llo reg,ptr
	lw	\reg,(ckreg_lo)(\ptr)
	.endm

	.macro lhi reg,ptr
	lw	\reg,(ckreg_hi)(\ptr)
	.endm

	.macro ssp reg,ptr
	sw	\reg,(ckreg_regs+29*ck_regsize)(\ptr)
	.endm

	.macro lsp reg,ptr
	lw	\reg,(ckreg_regs+29*ck_regsize)(\ptr)
	.endm

	.macro sva reg,val
	sw	\reg,\val
	.endm

	.macro lva reg,val
	lw	\reg,\val
	.endm

	.macro mvafc0 gpr,cpr
	mfc0	\gpr,\cpr
	.endm

	.macro mvatc0 gpr,cpr
	mtc0	\gpr,\cpr
	.endm

	.macro lpc reg,ptr
	lw	\reg,(ckreg_pc)(\ptr)
	.endm

	.macro spc reg,ptr
	sw	\reg,(ckreg_pc)(\ptr)
	.endm
#endif

#------------------------------------------------------------------------------
# FPU macros.
# The CK floating point unit essentially operates in two modes. In the first
# it supplies 32 32bit FP registers that may be paired into 16 64 bit registers.
# In the second it supplies 32 64bit registers. Which mode is to be used depends
# not only on the specific implementation in use, but also on the setting of the
# FR bit in the status register (if it is implemented) and on the expectations of
# the toolchain.			
	
#ifndef CYGPKG_HAL_CK_FPU_DEFINED

#ifdef CYGHWR_HAL_CK_FPU

#if defined(CYGHWR_HAL_CK_FPU_64BIT)
#define sfpr	sdc1
#define lfpr	ldc1
#define CYG_HAL_CK_FPU_SR_INIT	0x24000000	
#elif defined(CYGHWR_HAL_CK_FPU_32BIT)
#define sfpr	swc1
#define lfpr	lwc1
#define CYG_HAL_CK_FPU_SR_INIT	0x20000000		
#else
#error CK FPU register size not defined
#endif		

#ifndef CYG_HAL_CK_FCSR_INIT
#define CYG_HAL_CK_FCSR_INIT 0
#endif

	.macro	hal_fpu_init
	mfc0	v0,status			# Get sr
	la	v1,0xDBFFFFFF			# Clear bits to be changed
	and	v0,v0,v1
	la	v1,CYG_HAL_CK_FPU_SR_INIT	# Set the bits we want
	or	v0,v0,v1			# Set sr to required value
	mtc0	v0,status			# return to sr
	nop
	nop
	nop
        la	v0,CYG_HAL_CK_FCSR_INIT	# Get initial value for FCR31
	ctc1	v0,$31				# set Fp control reg
	nop
	.endm

	# Save the caller-saved registers as defined by the ABI.
	# These only really need saving during interrupts.
	.macro	hal_fpu_save_caller regs
	cfc1	v0,$31
	sw	v0,ckreg_fcr31(\regs)
	sfpr	f0,(ckreg_fpureg+0*ck_fpuregsize)(\regs)
	sfpr	f1,(ckreg_fpureg+1*ck_fpuregsize)(\regs)
	sfpr	f2,(ckreg_fpureg+2*ck_fpuregsize)(\regs)
	sfpr	f3,(ckreg_fpureg+3*ck_fpuregsize)(\regs)
	sfpr	f4,(ckreg_fpureg+4*ck_fpuregsize)(\regs)
	sfpr	f5,(ckreg_fpureg+5*ck_fpuregsize)(\regs)
	sfpr	f6,(ckreg_fpureg+6*ck_fpuregsize)(\regs)
	sfpr	f7,(ckreg_fpureg+7*ck_fpuregsize)(\regs)
	sfpr	f8,(ckreg_fpureg+8*ck_fpuregsize)(\regs)
	sfpr	f9,(ckreg_fpureg+9*ck_fpuregsize)(\regs)
	sfpr	f10,(ckreg_fpureg+10*ck_fpuregsize)(\regs)
	sfpr	f11,(ckreg_fpureg+11*ck_fpuregsize)(\regs)
	sfpr	f12,(ckreg_fpureg+12*ck_fpuregsize)(\regs)
	sfpr	f13,(ckreg_fpureg+13*ck_fpuregsize)(\regs)
	sfpr	f14,(ckreg_fpureg+14*ck_fpuregsize)(\regs)
	sfpr	f15,(ckreg_fpureg+15*ck_fpuregsize)(\regs)
	sfpr	f16,(ckreg_fpureg+16*ck_fpuregsize)(\regs)
	sfpr	f17,(ckreg_fpureg+17*ck_fpuregsize)(\regs)
	sfpr	f18,(ckreg_fpureg+18*ck_fpuregsize)(\regs)
	sfpr	f19,(ckreg_fpureg+19*ck_fpuregsize)(\regs)
	sfpr	f31,(ckreg_fpureg+31*ck_fpuregsize)(\regs)
	.endm

	# Save the callee-saved registers as defined by the ABI.
	# These are the only registers that need to be saved
	# across thread switches.
	.macro	hal_fpu_save_callee regs
	sfpr	f20,(ckreg_fpureg+20*ck_fpuregsize)(\regs)
	sfpr	f21,(ckreg_fpureg+21*ck_fpuregsize)(\regs)
	sfpr	f22,(ckreg_fpureg+22*ck_fpuregsize)(\regs)
	sfpr	f23,(ckreg_fpureg+23*ck_fpuregsize)(\regs)
	sfpr	f24,(ckreg_fpureg+24*ck_fpuregsize)(\regs)
	sfpr	f25,(ckreg_fpureg+25*ck_fpuregsize)(\regs)
	sfpr	f26,(ckreg_fpureg+26*ck_fpuregsize)(\regs)
	sfpr	f27,(ckreg_fpureg+27*ck_fpuregsize)(\regs)
	sfpr	f28,(ckreg_fpureg+28*ck_fpuregsize)(\regs)
	sfpr	f29,(ckreg_fpureg+29*ck_fpuregsize)(\regs)
	sfpr	f30,(ckreg_fpureg+30*ck_fpuregsize)(\regs)
	.endm

	# General macro to save everything
	.macro	hal_fpu_save regs
	hal_fpu_save_caller \regs
	hal_fpu_save_callee \regs
	.endm

	# Reload the caller-saved registers.
	.macro	hal_fpu_load_caller regs
	lfpr	f0,(ckreg_fpureg+0*ck_fpuregsize)(\regs)
	lfpr	f1,(ckreg_fpureg+1*ck_fpuregsize)(\regs)
	lfpr	f2,(ckreg_fpureg+2*ck_fpuregsize)(\regs)
	lfpr	f3,(ckreg_fpureg+3*ck_fpuregsize)(\regs)
	lfpr	f4,(ckreg_fpureg+4*ck_fpuregsize)(\regs)
	lfpr	f5,(ckreg_fpureg+5*ck_fpuregsize)(\regs)
	lfpr	f6,(ckreg_fpureg+6*ck_fpuregsize)(\regs)
	lfpr	f7,(ckreg_fpureg+7*ck_fpuregsize)(\regs)
	lfpr	f8,(ckreg_fpureg+8*ck_fpuregsize)(\regs)
	lfpr	f9,(ckreg_fpureg+9*ck_fpuregsize)(\regs)
	lfpr	f10,(ckreg_fpureg+10*ck_fpuregsize)(\regs)
	lfpr	f11,(ckreg_fpureg+11*ck_fpuregsize)(\regs)
	lfpr	f12,(ckreg_fpureg+12*ck_fpuregsize)(\regs)
	lfpr	f13,(ckreg_fpureg+13*ck_fpuregsize)(\regs)
	lfpr	f14,(ckreg_fpureg+14*ck_fpuregsize)(\regs)
	lfpr	f15,(ckreg_fpureg+15*ck_fpuregsize)(\regs)
	lfpr	f16,(ckreg_fpureg+16*ck_fpuregsize)(\regs)
	lfpr	f17,(ckreg_fpureg+17*ck_fpuregsize)(\regs)
	lfpr	f18,(ckreg_fpureg+18*ck_fpuregsize)(\regs)
	lfpr	f19,(ckreg_fpureg+19*ck_fpuregsize)(\regs)
	lfpr	f31,(ckreg_fpureg+31*ck_fpuregsize)(\regs)
	lw	v0,ckreg_fcr31(\regs)
        ctc1	v0,$31
	.endm

	# Reload the callee-saved registers.
	.macro	hal_fpu_load_callee regs
	lfpr	f20,(ckreg_fpureg+20*ck_fpuregsize)(\regs)
	lfpr	f21,(ckreg_fpureg+21*ck_fpuregsize)(\regs)
	lfpr	f22,(ckreg_fpureg+22*ck_fpuregsize)(\regs)
	lfpr	f23,(ckreg_fpureg+23*ck_fpuregsize)(\regs)
	lfpr	f24,(ckreg_fpureg+24*ck_fpuregsize)(\regs)
	lfpr	f25,(ckreg_fpureg+25*ck_fpuregsize)(\regs)
	lfpr	f26,(ckreg_fpureg+26*ck_fpuregsize)(\regs)
	lfpr	f27,(ckreg_fpureg+27*ck_fpuregsize)(\regs)
	lfpr	f28,(ckreg_fpureg+28*ck_fpuregsize)(\regs)
	lfpr	f29,(ckreg_fpureg+29*ck_fpuregsize)(\regs)
	lfpr	f30,(ckreg_fpureg+30*ck_fpuregsize)(\regs)
	.endm

	# Reload everything.
	.macro	hal_fpu_load regs
	hal_fpu_load_caller \regs
	hal_fpu_load_callee \regs
	.endm
#else	

	# Default macros for non-fpu implementations
	
	.macro	hal_fpu_init
	.endm
		
	.macro	hal_fpu_save regs
	.endm

	.macro	hal_fpu_save_caller regs
	.endm

	.macro	hal_fpu_save_callee regs
	.endm

	.macro	hal_fpu_load_caller regs
	.endm
	
	.macro	hal_fpu_load_callee regs
	.endm
	
	.macro	hal_fpu_load regs
	.endm
	
#endif
			
#endif	

#------------------------------------------------------------------------------
# MMU macros.
	
#ifndef CYGPKG_HAL_CK_MMU_DEFINED

	.macro	hal_mmu_init
	.endm

#endif	

#------------------------------------------------------------------------------
# MEMC macros.
	
#ifndef CYGPKG_HAL_CK_MEMC_DEFINED

	.macro	hal_memc_init
	.endm

#endif	
	
#------------------------------------------------------------------------------
# Cache macros.
	
#ifndef CYGPKG_HAL_CK_CACHE_DEFINED

#ifdef CYG_HAL_CK_R3900
	.macro	hal_cache_init
	mfc0	v0,config		# disable cache in config register
	nop
	nop
	la	v1,0xffffffcf
	and	v0,v0,v1
	mtc0	v0,config
	nop
	nop
	nop
	.endm
#else
	.macro	hal_cache_init

	mfc0	v0,config0		# disable Kseg0 caching in config0 register
	nop
	nop
	la	v1,0xfffffff8
	and	v0,v0,v1
	ori	v0,v0,2
	mtc0	v0,config0
	nop
	nop
	nop

	.endm
#endif

#endif	

#------------------------------------------------------------------------------
# Diagnostics macros.
	
#ifndef CYGPKG_HAL_CK_DIAG_DEFINED

	.macro	hal_diag_init
	.endm

	.macro	hal_diag_excpt_start
	.endm

	.macro	hal_diag_intr_start
	.endm

	.macro	hal_diag_restore
	.endm
#endif	

#------------------------------------------------------------------------------
# Timer initialization.
	
#ifndef CYGPKG_HAL_CK_TIMER_DEFINED

	.macro	hal_timer_init
	.endm

#endif	

#------------------------------------------------------------------------------
# Monitor initialization.
	
#ifndef CYGPKG_HAL_CK_MON_DEFINED

	.macro	hal_mon_init
	.endm

#endif	

#------------------------------------------------------------------------------
#endif // ifndef CYGONCE_HAL_ARCH_INC
# end of arch.inc

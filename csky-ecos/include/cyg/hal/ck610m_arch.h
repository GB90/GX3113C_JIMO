#ifndef CYGONCE_HAL_CK610M_ARCH_H
#define CYGONCE_HAL_CK610M_ARCH_H

#define HAL_THREAD_ATTACH_STACK_DEFINED
#define HAL_THREAD_ATTACH_STACK( _ptr_, _base_, _size_) \
    CYG_MACRO_START                        \
	_ptr_ = (_base_ + _size_) & 0xfffffff8; \
    CYG_MACRO_END
//--------------------------------------------------------------------------
#endif // CYGONCE_HAL_VAR_ARCH_H

MEMORY
{
       sdram : ORIGIN = 0x90000000 , LENGTH = 32M
}

hal_vsr_table = 0x90000000;
hal_virtual_vector_table = 0x90000000;

STARTUP(vectors.o)
ENTRY(__reset_vector)

GROUP(
    extras.o
                libsupc++.a
                libgcc.a
                libtarget.a
  libgxlowpower.a
  libgxcore.a
          )


SECTIONS
{

    .text : {
            _start_text = .;
            . = ALIGN (0x4);
        . = ALIGN (0x4); *(.rel.*) *(.rela.*)
        . = ALIGN (0x4); _stext = .; _etext = .; _ftext = . ; *(.text*) *(.stub)

        . = ALIGN (0x4);
                                _sdata = . ;
                                __data_start = . ;
                                data_start = . ;
                                *(.got_plt)
                                *(.got)
                                FILL(0);
                                . = ALIGN(0x20) ;
                                LONG(-1)
                . = ALIGN(0x20) ;
                *(.rodata)
                *(.rodata*)
                *(.rodata1)
                *(.rodata.*)
                *(.rodata.str1.4)
                *(.gnu.linkonce.r*)
                *(.data)
                *(.data*)
                *(.data1)
                *(.data.*)
                *(.rdata)
                *(.rdata*)
                *(.rdata1)
                *(.rdata.*)
                *(.gnu.linkonce.d*)
                *(.data1)
                *(.eh_frame)
                *(.gcc_except_table*)
                *(.sdata)
                *(.sdata.*)
                *(.gnu.linkonce.s.*)
                *(__libc_atexit)
                *(__libc_subinit)
                *(__libc_subfreeres)
                *(.note.ABI-tag)

        . = ALIGN (0x4); SORT(CONSTRUCTORS)
        . = ALIGN (0x4); KEEP (*crtbegin.o(.ctors)) __CTOR_LIST__ = .;
                         PROVIDE (__CTOR_LIST__ = .);
                         KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
                         KEEP (*(SORT(.ctors.*)))
                         KEEP (*(.ctors)) __CTOR_END__ = .;
                         PROVIDE (__CTOR_END__ = .);
        . = ALIGN (0x4); KEEP (*crtbegin.o(.dtors)) __DTOR_LIST__ = .;
                                         PROVIDE (__DTOR_LIST__ = .);
                                         KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
                                         KEEP (*(SORT(.dtors.*)))
                                         KEEP (*(.dtors)) __DTOR_END__ = .;
                                         PROVIDE (__DTOR_END__ = .);
        . = ALIGN (0x4); *(.sdata*) _edata = . ; PROVIDE (edata = .);
        _end_text = .;
    } > sdram

    .devtab ALIGN (0x4) : { . = .; KEEP(*( SORT (.ecos.table.*))) ; } > sdram

   .bss : {
        . = ALIGN (0x4); __bss_start = .; _fbss = .;
            . = ALIGN (0x4); *(.sbss) *(.sbss.*)
        . = ALIGN (0x4); *(.bss*) *(COMMON)
                . = ALIGN (0x4); __bss_end = .;
    } > sdram

   . = ALIGN(4); _end = .; PROVIDE (end = .);
   heap_start = ALIGN (0x8);
}

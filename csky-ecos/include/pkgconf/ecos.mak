ECOS_GLOBAL_CFLAGS = -O2 -g -Wall -Wpointer-arith -Wstrict-prototypes -Wundef -Woverloaded-virtual -Wno-write-strings -ffunction-sections -fdata-sections -fno-exceptions
ECOS_GLOBAL_LDFLAGS = -nostdlib -Wl,--gc-sections -Wl,-static,--fatal-warnings
ECOS_COMMAND_PREFIX = csky-elf-

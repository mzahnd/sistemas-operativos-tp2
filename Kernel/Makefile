include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_DRIVERS=$(wildcard drivers/*.c)
SOURCES_MEMORY=$(wildcard mem/*.c)
SOURCES_SCHEDULER=$(wildcard Scheduler/*.c)

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_DRIVERS=$(SOURCES_DRIVERS:.c=.o)
OBJECTS_MEMORY=$(SOURCES_MEMORY:.c=.o)
OBJECTS_SCHEDULER=$(SOURCES_SCHEDULER:.c=.o)

LOADERSRC=loader.asm
LOADEROBJECT=$(LOADERSRC:.asm=.o)

STATICLIBS=

ALL_OBJECTS = $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) \
			  $(OBJECTS_DRIVERS) $(OBJECTS_SCHEDULER) $(OBJECTS_MEMORY)

all: $(KERNEL)

$(KERNEL): $(ALL_OBJECTS)
	$(LD) $(LDFLAGS) $(EXTRA_LDFLAGS) -T kernel.ld \
		--oformat=elf64-x86-64 \
		-o kernel.elf \
		$(ALL_OBJECTS) $(STATICLIBS)
	$(LD) $(LDFLAGS) $(EXTRA_LDFLAGS) -T kernel.ld \
		-o $(KERNEL) \
		$(ALL_OBJECTS) $(STATICLIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf *.bin $(ALL_OBJECTS)

.PHONY: all clean

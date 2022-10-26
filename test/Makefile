# Compile using Makefile in the root directory. Depends on the successful
# compilation of the non-test source code.
#
# Based on
# https://stackoverflow.com/a/53138757

CC			:= clang
LD			:= clang
RMDIR		:= rm -Rf

EXE_TEST	:= test_all.out

INCLUDE		:= ./include

SRCS_TEST	:= $(wildcard *.c)
OBJS_TEST	:= $(SRCS_TEST:.c=.o)

SRCS_TEST_KERNEL	:= $(wildcard Kernel/lib.c) $(wildcard Kernel/mem/*.c) \
					   $(wildcard Kernel/semaphore.c) \
					   $(wildcard Kernel/pipes.c)
OBJS_TEST_KERNEL	:= $(SRCS_TEST_KERNEL:.c=.o)

OBJS_KERNEL	:=	$(wildcard ../Kernel/lib.o) \
				$(wildcard ../Kernel/mem/*.o) \
				$(wildcard ../Kernel/semaphore.o) \
				$(wildcard ../Kernel/pipes.o)

ALL_OBJS	:= $(OBJS_TEST_KERNEL) $(OBJS_TEST) $(OBJS_KERNEL)

CCSTD	:= gnu11

CCFLAGS	:=	-fdiagnostics-color=auto --std=$(CCSTD) -Wall \
			--pedantic -g -fno-exceptions -fno-asynchronous-unwind-tables \
			-msse -msse2 -msse3 -msse4 \
			-mno-red-zone -ffreestanding -Wno-int-to-pointer-cast \
			-fno-common -D_POSIX_C_SOURCE=200809L -DTESTING

LDFLAGS	:=	-g --std=$(CCSTD) -Wall -z max-page-size=0x1000 \
			-D_POSIX_C_SOURCE=200809L -lm -lpthread

.PHONY: all clean

all: $(EXE_TEST)

$(EXE_TEST): $(ALL_OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) -c $(CCFLAGS) -I$(INCLUDE) $< -o $@

clean:
	$(RMDIR) $(EXE_TEST) $(ALL_OBJS)


# Based on
# https://stackoverflow.com/a/53138757

CC			:= gcc
LD			:= clang
RMDIR		:= rm -Rf

EXE_TEST	:= test_all.out

INCLUDE		:= ./include

SRCS_TEST	:= $(wildcard *.c)
OBJS_TEST	:= $(SRCS_TEST:.c=.o)
SRCS_KERNEL	:= $(wildcard Kernel/mem/*.c)
OBJS_KERNEL	:= $(SRCS_KERNEL:.c=.o)

CCFLAGS	:= 	-fdiagnostics-color=auto --std=gnu11 -Wall \
			--pedantic -g -fno-exceptions -fno-asynchronous-unwind-tables \
			-mno-mmx -mno-sse -mno-sse2 -msse -msse2 -msse3 -msse4 \
			-fno-builtin-malloc -fno-builtin-free -fno-builtin-realloc \
			-mno-red-zone -ffreestanding -Wno-int-to-pointer-cast \
			-fno-common -D_POSIX_C_SOURCE=200809L

LDFLAGS	:= --warn-common -z max-page-size=0x1000 -D_POSIX_C_SOURCE=200809L -lm

.PHONY: all clean

all: $(EXE_TEST)

$(EXE_TEST): $(OBJS_KERNEL) $(OBJS_TEST)
	$(LD) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

clean:
	$(RMDIR) $(EXE_TEST) *.o


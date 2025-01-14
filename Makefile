all: bootloader kernel userland image

bootloader:
	cd Bootloader; make all

buddy: bootloader userland
	cd Kernel; make EXTRA_CFLAGS="-DBUDDY" all
	$(MAKE) image

kernel:
	cd Kernel; make all

userland:
	cd Userland; make all

image: kernel bootloader userland
	cd Image; make all

test:
	cd Kernel; make clean; make EXTRA_CFLAGS="-DTESTING -mcmodel=medium" all
	cd test; make all

buddyTest:
	cd Kernel; make clean; make EXTRA_CFLAGS="-DTESTING -DBUDDY -mcmodel=medium" all
	cd test; make EXTRA_CFLAGS="-DBUDDY" all

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Userland; make clean
	cd test; make clean

.PHONY: bootloader image collections kernel userland all clean test buddy buddyTest

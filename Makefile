#* Copyright (C) 2020 lsenekOS-Team
#	 This file is part of the lsenek Kernel.
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU Affero General Public License as
#    published by the Free Software Foundation, either version 3 of the
#    License, or (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU Affero General Public License for more details.#
#
#    You should have received a copy of the GNU Affero General Public License
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.
#*/
LOS = ~/lsene
INCPATH = $(LOS)/include
MKDIR =  $(LOS)/build
CC = gcc
CPP = g++
LD = ld
GAS = as
IAS = nasm
LIBDIRM = -I $(INCPATH)
CMKLG = 
CMKFLGS=-fno-builtin $(LIBDIRM) -m32 -c -Wall -nostdinc -nostdlib -fno-pie
CMKFLGSTD= $(LIBDIRM) -m32 -Wall
#/*-fno-builtin $(LIBDIRM) -m32 -c -Wall -nostdinc -nostdlib*/
#/*-mcmodel=large -fno-builtin $(LIBDIRM) -m64 -c -Wall -nostdinc -nostdlib*/
MKDEVROOT = /dev/sdb
MKDEV = /dev/sdb1
export CC
export CPP
export LD
export GAS
export IAS
export MKDIR
export LIBDIRM
export CMKLG
export CMKFLGS
export CMKFLGSTD
system:
	@echo "\033[34m[II] Setting INCPATH to $(INCPATH)\033[0m"
	time make all
	@echo "\033[34m[II] Compile Complete.\033[0m"
	@echo "\033[34m[II] Kernel:$(MKDIR)/lsenek.elf\033[0m"
	@echo "\033[0m"
	ls -l $(MKDIR)/lsenek.elf
	@echo "\033[34m"
	objdump -f $(MKDIR)/lsenek.elf
	@echo "\033[34m[II] Checking Vaild MB2\033[0m"
	make chkmb
	@echo "\033[32m KERNEL : OK\033[0m"

all:kernel.o
	@echo "\033[34m[II] Linking\033[0m"
	sync
	sync
	sync
	ld -z max-page-size=0x1000 -m elf_i386 -T kernel.lds --build-id=none $(MKDIR)/*.o  -o $(MKDIR)/lsenek~dirty.o
	@echo "\033[34m[II] Cleaning Other Section\033[0m"
	objcopy -R ".eh_frame" -R ".comment" -O elf32-i386 $(MKDIR)/lsenek~dirty.o $(MKDIR)/lsenek.elf
	rm -f $(MKDIR)/lsenek~dirty.o
clean:
	rm -rf $(MKDIR)
	mkdir $(MKDIR)
kernel.o:libs kheader.o
	@echo "\033[34m[II] Making Kernel\033[0m"
	$(CC) $(CMKFLGS) kernel.c -o $(MKDIR)/kernel.o $(CMKLG)
	@echo "\033[34m[II] EveryLib is Ok,Start Linking\033[0m"
kheader.o:kheader.s
	@echo "\033[34m[II] Making KernelHeader\033[0m"
	$(GAS) --32 -o $(MKDIR)/kheader.o kheader.s
	objcopy -O elf32-i386 $(MKDIR)/kheader.o $(MKDIR)/kheader.o
	@echo "\033[34m[II] Header is OK\033[0m"
dmp:
	objdump -s $(MKDIR)/lsenek.elf
sdmp:
	objdump -S $(MKDIR)/lsenek.elf
chkmb:
	grub-file --is-x86-multiboot2  $(MKDIR)/lsenek.elf && echo "\033[34m[II] Vaild Multiboot Header\033[0m" ||echo "\033[34m[II] Invaild Multiboot Header\033[0m"
install:system
	sudo cp $(MKDIR)/lsenek.elf /lsenek.elf
d8g:install
	sudo mount $(MKDEV) $(MKDIR)
	sudo cp /lsenek.elf $(MKDIR)/lsenek.elf > /dev/null
	sync
	sync
	sync
	sudo umount $(MKDEV) || sudo umount $(MKDEV) || sudo umount $(MKDEV) || sudo umount $(MKDEV) || sudo umount $(MKDEV) || sudo umount $(MKDEV) || sudo umount $(MKDEV) || sudo umount $(MKDEV)
	sudo qemu-system-i386 -s -S -m 256  -drive id=disk,file=$(MKDEVROOT),if=none -device ahci,id=ahci -device ide-drive,drive=disk,bus=ahci.0 -monitor stdio -net nic
run:install
	sudo mount $(MKDEV) $(MKDIR)
	sudo cp /lsenek.elf $(MKDIR)/lsenek.elf -f
	sync
	sync
	sync
	sudo umount $(MKDEV) || sudo umount $(MKDEV) || sudo umount $(MKDEV) || sudo umount $(MKDEV) || sudo umount $(MKDEV) || sudo umount $(MKDEV) || sudo umount $(MKDEV) || sudo umount $(MKDEV) 
	sudo qemu-system-i386 -s -m 256 -drive id=disk,file=$(MKDEVROOT),if=none -device ahci,id=ahci -device ide-drive,drive=disk,bus=ahci.0  -monitor stdio -serial file:COM0   -net nic
libs:
	make -C ./asm all
	make -C ./mem all
	@echo "\033[34m[II] All Libs Ok\033[0m"


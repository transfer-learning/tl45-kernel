
LLC=./toolchain/bin/llc
LLD=./toolchain/bin/lld
LLVM_MC=./toolchain/bin/llvm-mc
OBJCOPY=./toolchain/bin/llvm-objcopy
CLANG=./toolchain/bin/clang

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Source Files Configuration. Place all compile targets here. #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# Basic 
_OBJS = crt0.o lib/lcd.o lib/util.o simple.o

# Advanced
# _OBJS = crt0.o lib/sdcard.o lib/lcd.o lib/soft_impl.o lib/util.o ff14/diskio.o ff14/ff.o main.o
# _OBJS = crt0.o lib/lcd.o lib/soft_impl.o lib/util.o main.o
# _OBJS = crt0.o lib/soft_impl.o sha1.o
# _OBJS = crt0.o lib/soft_impl.o scomp.o
# _OBJS ?= crt0.o lib/soft_impl.o div_test.o

ODIR=obj
CFLAGS=-Wall -Iinclude

OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

.PHONY: default deploy build

build: $(ODIR)/a.out

$(ODIR)/crt0.o: crt0.s
	mkdir -p $(ODIR)
	$(LLVM_MC) --filetype obj -triple=tl45-unknown-unknown $< -o $@

$(ODIR)/testcase.o: testcase.s
	mkdir -p $(ODIR)
	$(LLVM_MC) --filetype obj -triple=tl45-unknown-unknown $< -o $@

$(ODIR)/%.o: src/%.c $(DEPS)
	mkdir -p $(ODIR)/ff14
	mkdir -p $(ODIR)/lib
	mkdir -p $(ODIR)/crypt
	$(CLANG) --target=tl45-unknown-none -fintegrated-as -O3 -c -v $(CFLAGS) $< -o $@
	$(CLANG) --target=tl45-unknown-none -fintegrated-as -O3 -c -v $(CFLAGS) $< -S -o $@.s

$(ODIR)/a.out: $(OBJS)
	$(LLD) -flavor gnu --oformat binary -image-base 0 $^ -o $@

default: deploy

deploy: build
	python2 serial_write.py $(ODIR)/a.out

simulate: build
	hardware-src/verilator/build/sim_tl45_comp $(ODIR)/a.out

clean:
	rm -rf obj/*

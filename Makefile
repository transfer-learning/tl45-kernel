
LLC=./toolchain/bin/llc
LLD=./toolchain/bin/lld
LLVM_MC=./toolchain/bin/llvm-mc
OBJCOPY=./toolchain/bin/llvm-objcopy
CLANG=./toolchain/bin/clang

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Source Files Configuration. Place all compile targets here. #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# Basic
_CSRCS = $(wildcard src/*.c src/libc/*.c src/**/*.c)
_OBJS = crt0.o
_OBJS += $(patsubst src/%.c, %.o, $(_CSRCS))

ODIR=obj
CFLAGS=-Wall -Iinclude -Itl45-libc/include

OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

.PHONY: default deploy build

build: $(ODIR)/a.bin

$(ODIR)/crt0.o: crt0.s
	mkdir -p $(ODIR)
	$(LLVM_MC) --filetype obj -triple=tl45-unknown-unknown $< -o $@

$(ODIR)/%.o: src/%.c $(DEPS)
	mkdir -p $(@D)
	$(CLANG) --target=tl45-unknown-none -fintegrated-as -O3 -c $(CFLAGS) $< -o $@
	$(CLANG) --target=tl45-unknown-none -fintegrated-as -O3 -c $(CFLAGS) $< -S -o $@.s

$(ODIR)/a.elf: $(OBJS)
	$(LLD) -flavor gnu --gc-sections -T tl45.ld tl45-libc/libc.a $^ -o $@

$(ODIR)/a.bin: $(ODIR)/a.elf
	$(OBJCOPY) --only-section=.raw -O binary $^ $@

default: deploy

deploy: build
	./tl-flash $(ODIR)/a.bin

simulate: build
	hardware-src/verilator/build/sim_tl45_comp $(ODIR)/a.out

clean:
	rm -rf obj/*

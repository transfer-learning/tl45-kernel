
LLC=./toolchain/bin/llc
LLD=./toolchain/bin/lld
LLVM_MC=./toolchain/bin/llvm-mc
OBJCOPY=./toolchain/bin/llvm-objcopy
CLANG=./toolchain/bin/clang
CLANGPP=./toolchain/bin/clang++

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Source Files Configuration. Place all compile targets here. #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# Basic
_CSRCS = $(wildcard src/*.c src/libc/*.c src/**/*.c)
_CXXSRCS = $(wildcard src/*.cpp)
_OBJS = crt0.o
_OBJS += $(patsubst src/%.c, %.o, $(_CSRCS))
_OBJS += $(patsubst src/%.cpp, %.o, $(_CXXSRCS))

ODIR=obj
CFLAGS=-Wall -Iinclude -Itl45-libc/include

OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

.PHONY: default build clean

build: $(ODIR)/a.bin

$(ODIR)/crt0.o: crt0.s
	mkdir -p $(ODIR)
	$(LLVM_MC) --filetype obj -triple=tl45-unknown-unknown $< -o $@

$(ODIR)/%.o: src/%.c $(DEPS)
	mkdir -p $(@D)
	$(CLANG) --target=tl45-unknown-none -fintegrated-as -O3 -c $(CFLAGS) $< -o $@
	$(CLANG) --target=tl45-unknown-none -fintegrated-as -O3 -c $(CFLAGS) $< -S -o $@.s

$(ODIR)/%.o: src/%.cpp $(DEPS)
	mkdir -p $(@D)
	$(CLANG) --target=tl45-unknown-none -fintegrated-as -O3 -c $(CFLAGS) $< -o $@
# 	$(CLANG) --target=tl45-unknown-none -fintegrated-as -O3 -c $(CFLAGS) $< -S -o $@.s

$(ODIR)/a.elf: $(OBJS)
	$(LLD) -flavor gnu --gc-sections -T tl45.ld tl45-libc/libc.a $^ -o $@

$(ODIR)/a.bin: $(ODIR)/a.elf
	$(OBJCOPY) --only-section=.raw -O binary $^ $@

$(ODIR)/a.out: $(_CSRCS)
	${CC} -D _NATIVE $(_CSRCS) $(_CXXSRCS) -O0 -o $(ODIR)/a.out

# TODO: add stuff to build libc.a

default: build

native: $(ODIR)/a.out

clean:
	rm -rf obj/*

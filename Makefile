PSP2REF_DIR=psp2ref

SRCDIR	= source
SRCS	= $(wildcard source/*.c)
OBJS	= $(SRCS:.c=.o) source/ex.ao source/vector.ao source/export.ao source/xconfig.ao source/util.s

PREFIX	= arm-vita-eabi
CC	= $(PREFIX)-gcc
OBJCOPY	= $(PREFIX)-objcopy
CFLAGS	= -mcpu=cortex-a9 -mthumb-interwork -mno-unaligned-access -Os -Wall -Wno-unused-const-variable -ffreestanding -std=gnu2x -I$(PSP2REF_DIR)
LDFLAGS	= -T linker.x -nostartfiles -nostdlib
ASFLAGS	=

# size limits, (re)configurable in cfg.x
ALICE_MAX_SIZE=16384 # spad32k
#ALICE_MAX_SIZE=196608 # DRAM

all: output/alice.bin

output/alice.bin: alice.bin
	@echo "\033[0;35mPost-compilation:\033[0;33m"
	@echo " | clean source dir"
	@-rm source/*.o
	@-rm source/*.ao
	@-rm -rf output
	@echo " | create output tree"
	@mkdir output
	@echo " | create alice binary include"
	@echo static const > output/alice.bin.h
	@xxd -i $< >> output/alice.bin.h
	@echo " | create alice exports header"
	@readelf -sW alice.elf | awk '{if ($$5 == "GLOBAL" && ($$4 == "FUNC" || $$4 == "OBJECT")) printf("#define alice_%s_addr 0x%s\n", $$8, $$2)}' > output/alice_exports.h
	@echo " | create alice linker"
	@echo ".alice_linker : {" > output/alice_linker.x && readelf -sW alice.elf | awk '{if ($$5 == "GLOBAL" && ($$4 == "FUNC" || $$4 == "OBJECT")) printf("\t%s = 0x%s;\n", $$8, $$2)}' >> output/alice_linker.x && echo "}" >> output/alice_linker.x
	@echo " | alice -> output"
	@mv alice.elf output/alice.elf
	@mv alice.bin output/alice.bin
	@echo " | ensure alice fits (ALICE_MAX_SIZE: $(ALICE_MAX_SIZE))"
	@if [ $$(stat -c %s output/alice.bin) -gt $(ALICE_MAX_SIZE) ]; then echo "\033[0;31m\nERROR: alice.bin is too large\033[0m"; exit 1; fi
	@echo "  \-> \033[0;32mall done\n\033[0m"

%.bin: %.elf
	@echo "\033[0;35mStrip:\033[0;33m $< -> $@\033[0m"
	@$(OBJCOPY) -S -O binary $^ $@

alice.elf: $(OBJS)
	@echo "\033[0;35mLink:\033[0;33m $^ -> $@\033[0m"
	@$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	@echo "\033[0;35mCompile:\033[0;33m $< -> $@\033[0m"
	@$(CC) $(CFLAGS) -c $< -o $@

%.ao: %.s
	@echo "\033[0;35mAssemble:\033[0;33m $< -> $@\033[0m"
	@$(CC) $(ASFLAGS) -c $< -o $@

clean:
	-rm source/*.o
	-rm source/*.ao
	-rm -rf output
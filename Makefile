PSP2REF_DIR=psp2ref

SRCDIR	= source
SRCS	= $(wildcard source/*.c)
OBJS	= $(SRCS:.c=.o) source/ex.ao source/vector.ao source/export.ao source/xconfig.ao

PREFIX	= arm-vita-eabi
CC	= $(PREFIX)-gcc
OBJCOPY	= $(PREFIX)-objcopy
CFLAGS	= -mcpu=cortex-a9 -mthumb-interwork -mno-unaligned-access -Os -Wall -Wno-unused-const-variable -ffreestanding -I$(PSP2REF_DIR)
LDFLAGS	= -T linker.x -nostartfiles -nostdlib
ASFLAGS	=

all: output/alice.bin

output/alice.bin: alice.bin
	-rm source/*.o
	-rm source/*.ao
	-rm -rf output
	mkdir output
	echo static const > output/alice.bin.h
	xxd -i $< >> output/alice.bin.h
	readelf -sW alice.elf | awk '{if ($$5 == "GLOBAL" && ($$4 == "FUNC" || $$4 == "OBJECT")) printf("#define alice_%s_addr 0x%s\n", $$8, $$2)}' > output/alice_exports.h
	echo ".alice_linker : {" > output/alice_linker.x && readelf -sW alice.elf | awk '{if ($$5 == "GLOBAL" && ($$4 == "FUNC" || $$4 == "OBJECT")) printf("\t%s = 0x%s;\n", $$8, $$2)}' >> output/alice_linker.x && echo "}" >> output/alice_linker.x
	mv alice.elf output/alice.elf
	mv alice.bin output/alice.bin

%.bin: %.elf
	$(OBJCOPY) -S -O binary $^ $@

alice.elf: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.ao: %.s
	$(CC) $(ASFLAGS) -c $< -o $@

clean:
	-rm source/*.o
	-rm source/*.ao
	-rm -rf output
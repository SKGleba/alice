SRCDIR	= source
SRCS	= $(wildcard source/*.c)
OBJS	= $(SRCS:.c=.o)

PREFIX	= arm-vita-eabi
CC	= $(PREFIX)-gcc
AS	= $(PREFIX)-as
OBJCOPY	= $(PREFIX)-objcopy
CFLAGS	= -mcpu=cortex-a9 -mthumb-interwork -mno-unaligned-access -Os -Wall -Wno-unused-const-variable -ffreestanding
LDFLAGS	= -T linker.x -nostartfiles -nostdlib
ASFLAGS	=

all: output/rpcp.bin

output/rpcp.bin: rpcp.bin
	-rm source/*.o
	-rm -rf output
	mkdir output
	echo static const > output/rpcp.bin.h
	xxd -i $< >> output/rpcp.bin.h
	mv rpcp.elf output/rpcp.elf
	mv rpcp.bin output/rpcp.bin

%.bin: %.elf
	$(OBJCOPY) -S -O binary $^ $@

rpcp.elf: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.ao: %.s
	$(CC) $(ASFLAGS) -c $< -o $@

clean:
	-rm source/*.o
	-rm -rf output
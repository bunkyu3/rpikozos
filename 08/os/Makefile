PREFIX	= /usr/local
ARCH	= aarch64-elf
BINDIR	= $(PREFIX)/bin
ADDNAME = $(ARCH)-

AR		= $(BINDIR)/$(ADDNAME)ar
AS		= $(BINDIR)/$(ADDNAME)as
CC		= $(BINDIR)/$(ADDNAME)gcc
LD		= $(BINDIR)/$(ADDNAME)ld
NM		= $(BINDIR)/$(ADDNAME)nm
OBJCOPY	= $(BINDIR)/$(ADDNAME)objcopy
OBJDUMP	= $(BINDIR)/$(ADDNAME)objdump
RANLIB	= $(BINDIR)/$(ADDNAME)ranlib
STRIP	= $(BINDIR)/$(ADDNAME)strip

OBJS  = startup.o main.o interrupt.o
OBJS += lib.o serial.o

OBJS += utils.o

# sources of kozos
OBJS += kozos.o syscall.o test08_1.o

TARGET	= rpikozos

CFLAGS	= -Wall -nostdinc -nostdlib -ffreestanding
CFLAGS	+= -I.
CFLAGS	+= -Os

LFLAGS	= -static -T ld.scr -L.

.SUFFIXES: .c .o
.SUFFIXES: .s .o
.SUFFIXES: .S .o

all :		$(TARGET)

$(TARGET) :	$(OBJS)
			$(CC) $(OBJS) -o $(TARGET) $(CFLAGS) $(LFLAGS)
			cp $(TARGET) $(TARGET).elf

.c.o :		$<
		$(CC) -c $(CFLAGS) $<

.s.o :		$<
		$(CC) -c $(CFLAGS) $<

.S.o :		$<
		$(CC) -c $(CFLAGS) $<

clean :
		rm -f $(OBJS) $(TARGET) $(TARGET).elf

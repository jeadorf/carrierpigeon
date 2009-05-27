XPATH=/usr/bin
CC=$(XPATH)/avr-gcc
OBJCOPY=$(XPATH)/avr-objcopy
STRIP=$(XPATH)/avr-strip
AVRDUDE=$(XPATH)/avrdude
OBJDUMP=$(XPATH)/avr-objdump

# 16 MHz
DEFINES=-DF_CPU=16000000

MCU=atmega8515

PROG_METHOD=avrisp2  # USB Prommer
PORT=-P usb

SRECS=\
	main.srec \
	$(NULL) 

all: $(SRECS)

main: main.o

PROGRAMS=$(SRECS:.srec=)
OBJFILES=$(SRECS:.srec=.o)
ASSEMBLY=$(SRECS:.srec=.asm)

clean:
	rm -f $(SRECS) $(PROGRAMS) *.o $(ASSEMBLY) *.flash *.hex *~

%.o: %.c
	$(CC)  -O2 $(DEFINES) -mmcu=$(MCU) -c $^ -o $@

%: %.o 
	$(CC)  -O2 -mmcu=$(MCU) $(LDFLAGS) $^ -o $@

%.asm: %
	$(OBJDUMP) -S -d $^ > $@

%-stripped: %
	$(STRIP) $^ -o $@

%.srec: %-stripped
	$(OBJCOPY) -O srec $^ $@
	$(OBJCOPY) -O ihex $^ main.hex
	$(OBJCOPY) -O binary $^ main.flash

%.hex: %-stripped
	$(OBJCOPY) -O ihex $^ $@

program: main.srec
	$(AVRDUDE) -c $(PROG_METHOD) $(PORT) -p $(MCU) \
		-U hfuse:w:0xdd:m \
		-U lfuse:w:0x3f:m \
		-U lock:w:0x3f:m \
		-U flash:w:$^

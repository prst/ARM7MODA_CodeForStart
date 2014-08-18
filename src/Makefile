##################################
#    Makefile for AT91SAM7S      #
##################################

NAME    =	ARM7MODA
PATH_PR =	/usr/local/arm
PATH_EX =	$(PWD)
CROSS   =	$(PATH_PR)/bin/arm-elf-
CC      =	$(CROSS)gcc
CPP     =	$(CROSS)cpp
AS      =	$(CROSS)as
LD      =	$(CROSS)ld 
OBJCOPY =	$(CROSS)objcopy
OBJDUMP =	$(CROSS)objdump
STRIP   =	$(CROSS)strip
PROGRAMMATOR = ~/bin/samba

HOSTCC =	gcc
#---------------------------------

LDSCRPT = Flash
#LDSCRPT = RAM

#---------------------------------
#LIBPATH	= $(PATH_PR)/arm-elf/lib
#LIBGCC	= $(PATH_PR)/lib/gcc-lib/arm-elf/3.3.6/
#INCPATH	= $(PATH_PR)/lib/gcc-lib/arm-elf/3.3.6/include
#CFLAGS	 = -g -marm -mapcs-frame
#CFLAGS  = -g -marm -Wcomment -Wconversion  -Wno-deprecated-declarations
CPPFLAGS = -nostdinc -I$(INCLUDE)
CFLAGS  = -g -marm -Wall
LDFLAGS = -nostdlib
#---------------------------------
OBJS	=	startup.o
OBJS	+=	exit.o
OBJS	+=	goto.o
OBJS	+=	$(NAME).o
OBJS	+=	init.o
OBJS	+=	delays.o
OBJS	+=	lcd.o
OBJS	+=	pwm.o
OBJS	+=	adc.o
#OBJS	+=	debug.o

#---------------------------------

.PHONY: all clean x prog

#---------------------------------
all: $(BIN)/$(NAME).bin $(BIN)/$(NAME).hex

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@
	@chmod a-x $@

%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@
	@chmod a-x $@

%.elf: $(OBJS) $(LINKER)/$(LDSCRPT).ld
	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o $@
	@chmod a-x $@

%.d: %.c
	$(CC) $(CPPFLAGS) -MM -MG -MT '$(@:.d=.o) $@' -MF $@ $<

%.d: %.S
	$(CC) $(CPPFLAGS) -MM -MG -MT '$(@:.d=.o) $@' -MF $@ $<

DEPS    = $(OBJS:.o=.d)

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif

#---------------------------------
clean:
	$(RM) *.o *.d
	$(RM) $(BIN)/$(NAME).elf
	$(RM) $(BIN)/$(NAME).bin
	$(RM) $(BIN)/dump
	$(RM) $(BIN)/$(NAME).txt
	$(RM) $(PATH_EX)/$(NAME).bin
#	$(RM) /home/max/smbmnt/temp/prog/test.bin	

#---------------------------------
x:
#	echo "************************************************************************"
	make clean
	make all
#	cat ./bin/SAMBA_init.txt>/dev/ttyS0 && cat ./bin/$(NAME).txt>/dev/ttyS0
#	echo "************************************************************************"
	$(OBJDUMP) -h $(BIN)/$(NAME).elf 
	ls -alF --color ./bin

#---------------------------------
prog:
#	$(PROGRAMMATOR) -s SAM-BA.commands
	sam-ba--start-dev.sh && sam-ba &

#	$(BIN)/cmdgen $(BIN)/$(NAME).bin > $(BIN)/$(NAME).txt 
#	cat ./bin/SAMBA_init.txt>/dev/ttyS0 && cat ./bin/$(NAME).txt>/dev/ttyS0

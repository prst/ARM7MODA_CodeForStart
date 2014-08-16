##################################
#    Makefile for AT91SAM7S      #
##################################

NAME    =	ARM7MODA
PATH_PR =	/usr/local/arm
PATH_GL =	$(PATH_PR)/bin
PATH_EX =	`pwd`
CROSS   =	arm-elf-
CC      =	$(PATH_GL)/$(CROSS)gcc
CPP     =	$(PATH_GL)/$(CROSS)cpp
AS      =	$(PATH_GL)/$(CROSS)as
LD      =	$(PATH_GL)/$(CROSS)ld 
OBJCOPY =	$(PATH_GL)/$(CROSS)objcopy
OBJDUMP =	$(PATH_GL)/$(CROSS)objdump
STRIP   =	$(PATH_GL)/$(CROSS)strip
PROGRAMMATOR = ~/bin/samba

HOSTCC =	gcc
#---------------------------------
OBJ     = ./obj
BIN     = ./bin
INCLUDE = ./inc
SOURCE  = ./src
LINKER  = ./lnk
#---------------------------------
LDSCRPT = Flash
#LDSCRPT = RAM
#---------------------------------
LIBPATH	= $(PATH_PR)/arm-elf/lib
LIBGCC	= $(PATH_PR)/lib/gcc-lib/arm-elf/3.3.6/
INCPATH	= $(PATH_PR)/lib/gcc-lib/arm-elf/3.3.6/include
#CFLAGS	 = -c -g -marm -mapcs-frame
#CFLAGS  = -c -g -marm -Wcomment -Wconversion  -Wno-deprecated-declarations
CFLAGS  = -c -g -marm -Wall
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
OBJFILE	=	$(foreach d, $(OBJS), $(OBJ)/$(d))

#---------------------------------
all: $(OBJFILE)
#	$(LD) -o $(BIN)/$(NAME).elf $(OBJFILE) -I$(INCPATH) -L$(LIBPATH) -L$(LIBGCC) -lc -lgcc   \
#      -lnosys  --script=$(LINKER)/$(LDSCRPT).ld
	$(LD) -o $(BIN)/$(NAME).elf $(OBJFILE) -I$(INCPATH) -L$(LIBPATH) -L$(LIBGCC) -lc -lnosys \
      --script=$(LINKER)/$(LDSCRPT).ld
#	$(OBJDUMP) -SD $(BIN)/$(NAME).elf > $(BIN)/dump
	rm -f $(PATH_EX)/$(NAME).bin
	$(OBJCOPY) -O binary $(BIN)/$(NAME).elf $(BIN)/$(NAME).bin
	$(OBJCOPY) -O ihex $(BIN)/$(NAME).elf $(BIN)/$(NAME).hex
	$(BIN)/cmdgen $(BIN)/$(NAME).bin > $(BIN)/$(NAME).txt 
#	cat ./bin/SAMBA_init.txt>/dev/ttyS0 && cat ./bin/$(NAME).txt>/dev/ttyS0
$(OBJ)/%.o: $(SOURCE)/%.c $(INCLUDE)/*.h
	$(CC) -I$(INCLUDE) $(CFLAGS) $< -o $@
$(OBJ)/%.o: $(SOURCE)/%.s
	$(AS) $< -o $@

#---------------------------------
clean:
	rm -f $(OBJ)/*.o
	rm -f $(BIN)/$(NAME).elf
	rm -f $(BIN)/$(NAME).bin
	rm -f $(BIN)/dump
	rm -f $(BIN)/$(NAME).txt
	rm -f $(PATH_EX)/$(NAME).bin
#	rm -f /home/max/smbmnt/temp/prog/test.bin	

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

.PHONY: all x clean

export VPATH=$(PWD)/src

export BIN     = $(PWD)/bin
export INCLUDE = $(PWD)/inc
export SOURCE  = $(PWD)/src
export LINKER  = $(PWD)/lnk

all x clean:
	$(MAKE) -f $(VPATH)/Makefile -C obj $@

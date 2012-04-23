CROSS=
CC=$(CROSS)gcc
LD=$(CROSS)ld
AR=$(CROSS)ar
ARCH=$(shell uname -m)

ifndef PROJ_ROOT
	PROJ_ROOT=.
endif

INCLUDES=-I$(PROJ_ROOT)/includes
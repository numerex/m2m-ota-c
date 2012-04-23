
include includes.mk

SOURCES=$(filter-out OTA_Message.c OTA_Message_Embedded.c,$(wildcard *.c))

ifdef EMBEDDED
	CFLAGS += -DEMBEDDED
	EMBEDDED="_e"
	SOURCES+=OTA_Message_Embedded.c
else
	SOURCES+=OTA_Message.c
endif

LIBRARY=libm2mota$(EMBEDDED).a

include rules.mk
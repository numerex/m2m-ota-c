OBJECTS=$(patsubst %.c, $(ARCH)/%.o, $(SOURCES))

$(LIBRARY):  $(ARCH) $(OBJECTS)
	@echo "Making $(LIBRARY) with $(OBJECTS)"
	$(AR) vrcs $(ARCH)/$(LIBRARY) $(OBJECTS)


$(APPLICATION):	$(ARCH) $(OBJECTS)
	@echo "Making $(APPLICATION) with $(OBJECTS)"
	$(CC) -o $(APPLICATION) $(OBJECTS) -L$(PROJ_ROOT)/$(ARCH) $(LIBRARIES)

clean:
	rm -rf $(OBJECTS) $(APPLICATION) $(LIBRARY) $(ARCH) *~

$(ARCH)/%.o:	%.c
	$(CC) $(INCLUDES) -c $< -o $@

$(ARCH):	
	mkdir -p $(ARCH)

.PHONY:  $(ARCH) clean
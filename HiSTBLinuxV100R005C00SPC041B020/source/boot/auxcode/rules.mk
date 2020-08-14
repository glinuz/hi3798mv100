
OBJS  := $(COBJS) $(AOBJS)
DEPS  += $(OBJS:.o=.d)
TEMPS += $(OBJS) $(DEPS)
TEMPS += $(LIBS)

$(LIBS): $(OBJS)
	@$(RM) -f $@
	$(AR) $(ARFLAGS) $@ $^ 

%.o: %.c
	$(CC) $(CFLAGS) -Wall -Wstrict-prototypes \
		-fno-stack-protector -c $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) -Wall -Wstrict-prototypes \
		-fno-stack-protector -c $< -o $@

ifneq ("$(MAKECMDGOALS)","clean")
sinclude $(DEPS)
endif

%.d : %.c
	@set -e; $(CC) $(CFLAGS) -M $< | sed 's,$*.o:,$*.o $*.d:,g' > $@

%.d : %.S
	@set -e; $(CC) $(CFLAGS) -M $< | sed 's,$*.o:,$*.o $*.d:,g' > $@

.PHONY: clean
clean:
	$(RM) -rf $(TEMPS)


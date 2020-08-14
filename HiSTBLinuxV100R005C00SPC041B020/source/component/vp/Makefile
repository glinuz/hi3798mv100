
#===============================================================================
#   rules
#===============================================================================
.PHONY: all clean install uninstall

all:
	$(AT)make -C src
	$(AT)make -C rtp	

clean:
	$(AT)make -C rtp clean
	$(AT)make -C src clean

install: all
	$(AT)make -C src install
	$(AT)make -C rtp install

uninstall:
	$(AT)make -C rtp uninstall
	$(AT)make -C src uninstall
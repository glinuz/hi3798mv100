all:
	@echo "use default ddr training"
	rm -f libddr.a
	$(MAKE) -C ../default/cmd_bin TOPDIR=$(TOPDIR) CROSS_COMPILE=$(CROSS_COMPILE)
	$(MAKE) -C ../default
	cp -f ../default/libddr.a ./

clean:
	$(MAKE) -C ../default/cmd_bin TOPDIR=$(TOPDIR) CROSS_COMPILE=$(CROSS_COMPILE) clean
	$(MAKE) -C ../default clean
	rm -rf libddr.a
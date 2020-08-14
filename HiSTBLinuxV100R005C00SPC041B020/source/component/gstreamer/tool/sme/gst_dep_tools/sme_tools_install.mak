TARGET_INTERMEDIATE_DIR := $(HOME)/sme_tool/intermediate
TARGET_INSTALL_PRFIX_DIR := $(HOME)/sme_tool/usr
#TARGET_INSTALL_PRFIX_DIR := /usr/local/

#for lib we isntall to $(TARGET_INSTALL_DIR)/lib
#for exe we isntall to $(TARGET_INSTALL_DIR)/bin


.PHONY := all clean

all:prepare gettext ffi glib expat XML-Parser intltool libtool

prepare:
	@mkdir -p $(TARGET_INTERMEDIATE_DIR)
	@mkdir -p $(TARGET_INSTALL_PRFIX_DIR)/bin
	@mkdir -p $(TARGET_INSTALL_PRFIX_DIR)/lib
	@mkdir -p $(TARGET_INSTALL_PRFIX_DIR)/include

gettext:
	@echo "install gettext begin ..."
	tar -xf gettext-0.18.2.tar.gz -C $(TARGET_INTERMEDIATE_DIR)
	cd $(TARGET_INTERMEDIATE_DIR)/gettext-0.18.2;\
		./configure --prefix="${TARGET_INSTALL_PRFIX_DIR}"
	-cd $(TARGET_INTERMEDIATE_DIR)/gettext-0.18.2;\
		make uninstall ; make clean
	cd $(TARGET_INTERMEDIATE_DIR)/gettext-0.18.2;\
		make; make install
	@echo "install gettext end"

gettext-clean:
	-@ cd $(TARGET_INTERMEDIATE_DIR)/gettext-0.18.2;\
		make uninstall ; make clean
	rm -rf $(TARGET_INTERMEDIATE_DIR)/gettext-0.18.2
	@echo "uninstall gettext!"

ffi:
	@echo "install ffi begin ..."
	tar -xf libffi-3.0.13.tar.gz -C $(TARGET_INTERMEDIATE_DIR)
	cd $(TARGET_INTERMEDIATE_DIR)/libffi-3.0.13;\
		./configure --prefix="${TARGET_INSTALL_PRFIX_DIR}"
	-cd $(TARGET_INTERMEDIATE_DIR)/libffi-3.0.13;\
		make uninstall ; make clean
	cd $(TARGET_INTERMEDIATE_DIR)/libffi-3.0.13;\
		make; make install
	@echo "install ffi end"
ffi-clean:
	-@ cd $(TARGET_INTERMEDIATE_DIR)/libffi-3.0.13;\
		make uninstall ; make clean
	rm -rf $(TARGET_INTERMEDIATE_DIR)/libffi-3.0.13
	@echo "uninstall ffi!"

glib:
	@echo "install glib begin ..."
	tar -xf glib-2.38.2.tar.gz -C $(TARGET_INTERMEDIATE_DIR)
	cd $(TARGET_INTERMEDIATE_DIR)/glib-2.38.2;\
		./configure --prefix="${TARGET_INSTALL_PRFIX_DIR}" PKG_CONFIG_PATH=$(SME_TOOL_PKG_CONFIG)
	-cd $(TARGET_INTERMEDIATE_DIR)/glib-2.38.2;\
		make uninstall ; make clean
	cd $(TARGET_INTERMEDIATE_DIR)/glib-2.38.2;\
		make; make install
	@echo "install glib end"
glib-clean:
	-@ cd $(TARGET_INTERMEDIATE_DIR)/glib-2.38.2;\
		make uninstall ; make clean
	rm -rf $(TARGET_INTERMEDIATE_DIR)/glib-2.38.2
	@echo "uninstall glib!"

expat:
	@echo "install expat begin ..."
	tar -jxf expat-2.1.1.tar.bz2 -C $(TARGET_INTERMEDIATE_DIR)
	cd $(TARGET_INTERMEDIATE_DIR)/expat-2.1.1;\
		./configure --prefix="${TARGET_INSTALL_PRFIX_DIR}"
	-cd $(TARGET_INTERMEDIATE_DIR)/expat-2.1.1;\
		make uninstall ; make clean
	cd $(TARGET_INTERMEDIATE_DIR)/expat-2.1.1;\
		make; make install
	@echo "install expat end"

expat-clean:
	-@ cd $(TARGET_INTERMEDIATE_DIR)/expat-2.1.1;\
		make uninstall ; make clean
	rm -rf $(TARGET_INTERMEDIATE_DIR)/expat-2.1.1
	@echo "uninstall expat!"

XML-Parser:
	@echo "install XML-Parser begin ..."
	tar -xf XML-Parser-2.34.tar.gz -C $(TARGET_INTERMEDIATE_DIR)
	cd $(TARGET_INTERMEDIATE_DIR)/XML-Parser-2.34;\
		perl Makefile.PL PREFIX=${TARGET_INSTALL_PRFIX_DIR} INSTALLDIRS=${TARGET_INSTALL_PRFIX_DIR}/perl \
		EXPATINCPATH=${TARGET_INSTALL_PRFIX_DIR}/include EXPATLIBPATH=${TARGET_INSTALL_PRFIX_DIR}/lib
	-cd $(TARGET_INTERMEDIATE_DIR)/XML-Parser-2.34;\
		make uninstall ;
	cd $(TARGET_INTERMEDIATE_DIR)/XML-Parser-2.34;\
		make; make install_perl
	@echo "install XML-Parser end"

XML-Parser-clean:
	-@ cd $(TARGET_INTERMEDIATE_DIR)/XML-Parser-2.34;\
		make uninstall ; make clean
	rm -rf $(TARGET_INTERMEDIATE_DIR)/XML-Parser-2.34
	@echo "uninstall XML-Parser!"

intltool:
	@echo "install intltool begin ..."
	tar -xf intltool-0.50.2.tar.gz -C $(TARGET_INTERMEDIATE_DIR)
	cd $(TARGET_INTERMEDIATE_DIR)/intltool-0.50.2;\
		./configure --prefix="${TARGET_INSTALL_PRFIX_DIR}"
	-cd $(TARGET_INTERMEDIATE_DIR)/intltool-0.50.2;\
		make uninstall ; make clean
	cd $(TARGET_INTERMEDIATE_DIR)/intltool-0.50.2;\
		make; make install
	@echo "install intltool end"
intltool-clean:
	-@ cd $(TARGET_INTERMEDIATE_DIR)/intltool-0.50.2;\
		make uninstall ; make clean
	rm -rf $(TARGET_INTERMEDIATE_DIR)/intltool-0.50.2
	@echo "uninstall intltool!"

libtool:
	@echo "install libtool begin ..."
	tar -xf libtool-2.4.6.tar.gz -C $(TARGET_INTERMEDIATE_DIR)
	cd $(TARGET_INTERMEDIATE_DIR)/libtool-2.4.6;\
		./configure --prefix="${TARGET_INSTALL_PRFIX_DIR}"
	-cd $(TARGET_INTERMEDIATE_DIR)/libtool-2.4.6;\
		make uninstall ; make clean
	cd $(TARGET_INTERMEDIATE_DIR)/libtool-2.4.6;\
		make; make install
	@echo "install libtool end"
libtool-clean:
	-@ cd $(TARGET_INTERMEDIATE_DIR)/libtool-2.4.6;\
		make uninstall ; make clean
	rm -rf $(TARGET_INTERMEDIATE_DIR)/libtool-2.4.6
	@echo "uninstall libtool!"

clean:libtool-clean intltool-clean XML-Parser-clean expat-clean glib-clean ffi-clean gettext-clean

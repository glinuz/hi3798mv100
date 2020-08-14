PROJECT=libsecurec.so
#if you need a debug version library, use "-g" instead of "-s -DNDEBUG -O2".
# If you compiler report a warning on "break strict-aliasing rules", there is no problem. If you need to clear all warnings, you can add "-fno-strict-aliasing" option to your compiler, but this will impact the performance a little.

CFLAG= -I ../include -Wall  -s -DNDEBUG -O2 
CXXFLAG= $(CFLAG)
CC=gcc
GCC=gcc
ARCH:=$(shell getconf LONG_BIT)

ifeq ($(ARCH), 64)
    CFLAG += -fPIC
    CXXFLAG += -fPIC
endif

SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

$(PROJECT):$(OBJECTS)
	@mkdir -p ../obj
	mkdir -p ../lib
	$(GCC)  -shared -o ../lib/$@ $(patsubst %.o,../obj/%.o,$^) $(CFLAG) 
	ar crv libsecurec.a $(patsubst %.o,../obj/%.o,$^)
	ranlib libsecurec.a 
	#cp ../lib/libsecurec.so /usr/local/lib/libsecurec.so
	#you may add you custom commands here
	@echo "finish $(PROJECT)"
.c.o:
	@mkdir -p ../obj
	$(GCC) -c $< $(CFLAG) -o ../obj/$(patsubst %.c,%.o,$<)

clean:
	rm -rf *.o ../obj ../lib $(PROJECT)


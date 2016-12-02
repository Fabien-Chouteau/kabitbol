include ../Makefile.rules

LDFLAGS += -T ../buildtools/kernel.lds
CFLAGS  += -Iinclude -I$(KABITBOL_PATH)/shared


TARGET  = ../libgeorge.a
HEADERS =

SRC = printf.c stream.c malloc.c kikou.c
ASM = syscall.S

OBJ =  $(SRC:.c=.o)
OBJ += $(ASM:.S=.o)

SUBDIRS=

all: $(TARGET)

$(TARGET): $(OBJ) $(LIBS)
	$(AR) $(ARFLAGS) $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.o: %.S
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJ)
	$(foreach dir,$(SUBDIRS), make -C $(dir) clean; )

distclean: clean
	rm -f $(TARGET)
	rm -f .depend
	$(foreach dir,$(SUBDIRS), make -C $(dir) distclean; )

depend: $(SRC) $(ASM)
	$(CC) -E -MM $(CFLAGS) $(SRC) $(ASM) > .depend
	$(foreach dir,$(SUBDIRS), make -C $(dir) depend; )

indent:
#see  http://www.gnu.org/software/indent/manual/indent.html
		indent $(INDENTFLAGS) $(SRC) $(HEADERS)


-include .depend

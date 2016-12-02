include ../Makefile.rules

LDFLAGS += -T ../buildtools/partition.lds
CFLAGS  += -I$(KABITBOL_PATH)/shared -I$(KABITBOL_PATH)/libgeorge/include

TARGET  = $(KABITBOL_PATH)/init.part
LIBGEORGE = $(KABITBOL_PATH)/libgeorge.a
HEADERS =
SRC = init.c
ASM =

OBJ = $(SRC:.c=.o) $(LIBGEORGE)
OBJ += $(ASM:.S=.o)

.PHONY:$(LIBGEORGE)

all: $(TARGET)

$(TARGET): $(OBJ) $(LIBS)
	$(LD) $(LDFLAGS) -o initpartition.o $(OBJ)
	$(LD) -r -b binary -o $(TARGET) initpartition.o
	$(OBJCOPY) --rename-section .data=.rodata,alloc,load,readonly,data,contents $(TARGET) $(TARGET)

$(LIBGEORGE):
	make -C $(KABITBOL_PATH)/libgeorge

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.o: %.S
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJ)
	rm -f initpartition.o
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

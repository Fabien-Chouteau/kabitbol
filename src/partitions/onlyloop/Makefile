include ../../Makefile.rules

LDFLAGS += -T ../../../buildtools/partition.lds

TARGET = ../onlyloop.part
SRC = onlyloop.c
ASM =

OBJ = $(SRC:.c=.o)
OBJ += $(ASM:.S=.o)

SUBDIRS=

all: $(TARGET)
	$(foreach dir,$(SUBDIRS), make -C $(dir) all; )

$(TARGET): $(OBJ)
	$(LD) $(LDFLAGS) -o $(TARGET) $(OBJ)


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

-include .depend

include ../Makefile.rules

LDFLAGS += -T ../buildtools/kernel.lds
CFLAGS  += -Iinclude -I$(KABITBOL_PATH)/shared


TARGET  = kabitbol
INITPART = $(KABITBOL_PATH)/init.part

HEADERS = include/config.h							\
				  include/context.h							\
				  include/partition.h						\
				  include/timer.h								\
				  include/utils.h								\
				  include/core.h								\
				  include/irq.h									\
				  include/scheduler.h						\
				  include/debug.h								\
				  include/traps.h								\
				  include/asm.h									\
				  include/cons.h								\
				  include/uart.h								\
				  include/psr.h									\
				  include/mm/kalloc.h						\
				  include/mm/mm.h								\
				  include/mm/ppages.h						\
				  include/mm/vm_block.h					\
				  include/mm/vm_stacks.h				\
				  include/mm/pt_alloc.h					\
				  include/elf.h									\
				  include/svga.h								\
				  include/syscall.h							\
				  include/amba.h

SRC =	traps.c						\
			cons.c						\
			core.c						\
			amba.c						\
			timer.c						\
			irq.c							\
			utils.c						\
			uart.c						\
			debug.c						\
			svga.c						\
			context.c					\
			partition.c				\
			syscall.c					\
			scheduler.c				\
			thread.c					\
			list.c						\
			mutex.c

ASM =	trapvector.S			\
			mul.S							\
			umul.S						\
			udiv.S						\
			urem.S

# MM sources #
SRC += mm/mm.c mm/ppages.c mm/kalloc.c mm/vm_block.c mm/vm_stacks.c mm/mapping.c mm/pt_alloc.c mm/vm_manager.c mm/page_fault.c mm/access.c

OBJ = $(SRC:.c=.o) $(INITPART)
OBJ += $(ASM:.S=.o)

SUBDIRS=

.PHONY: $(INITPART)

all: $(TARGET)

$(INITPART):
	make -C $(KABITBOL_PATH)/init

$(TARGET): $(OBJ) $(LIBS)
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

indent:
#see  http://www.gnu.org/software/indent/manual/indent.html
		indent $(INDENTFLAGS) $(SRC) $(HEADERS)


-include .depend

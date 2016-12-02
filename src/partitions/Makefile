include ../../Makefile.rules

SUBDIRS= onlyloop loop_n_trap loop_n_trap_2

PARTITIONS= loop_n_trap.part loop_n_trap.part

all: partitions.o sizes.o

partitions.o:
	$(foreach dir,$(SUBDIRS), make -C $(dir) all; )
	cat $(PARTITIONS) > partitions.bin
	$(LD) -r -b binary -o partitions.o partitions.bin
	$(OBJCOPY) --rename-section .data=.rodata,alloc,load,readonly,data,contents partitions.o partitions.o

sizes.o: partitions.o
	rm -f sizes.c
	touch sizes.c
	echo "unsigned int g_partitions_sizes[] = {" >> sizes.c
	CNT=0; N=1; for v in $(PARTITIONS); do \
		if test $$N -eq 0; then echo "," >> sizes.c ; fi ; N=0 ;CNT=$$(($$CNT + 1));\
		ls -l $$v | awk '{print $$5}' >> sizes.c ; \
	done; \
	echo "}; unsigned int g_partitions_bin_nbr = $$CNT;" >> sizes.c
	$(CC) $(CFLAGS) -o sizes.o -c sizes.c

clean:
	rm -f sizes.c sizes.o partitions.o partitions.bin
	$(foreach dir,$(SUBDIRS), make -C $(dir) clean; )

distclean: clean
	$(foreach dir,$(SUBDIRS), make -C $(dir) distclean; )

depend:
	$(foreach dir,$(SUBDIRS), make -C $(dir) depend; )

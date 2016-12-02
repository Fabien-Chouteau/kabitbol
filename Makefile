QEMU=qemu-system-sparc -M leon3_generic -nographic -kernel

SUBDIRS = src libgeorge init

VERSION   = $(shell date "+%Y-%m-%d")
TARPREFIX = kabitbol
TARBALL   = $(TARPREFIX)-$(VERSION).tar.gz

all:
	make -C src all

clean:
	rm -f *.files *.out
	$(foreach dir,$(SUBDIRS), make -C $(dir) clean;)

distclean: clean
	find . -name '*~' -exec rm -f '{}' ';'
	rm -f flash.img cscope.*
	$(foreach dir,$(SUBDIRS), make -C $(dir) distclean;)
	rm -f Makefile.rules


depend:
	$(foreach dir,$(SUBDIRS), make -C $(dir) depend;)

dist:
	git archive --format=tar --prefix=$(TARPREFIX)/ master | gzip > $(TARBALL)


run: all
	$(QEMU) src/kabitbol

check: dist
	cp $(TARBALL) /tmp/      &&\
	cd /tmp                  &&\
	rm -Rf /tmp/$(TARPREFIX) &&\
	tar xf /tmp/$(TARBALL)   &&\
	cd $(TARPREFIX)          &&\
	./configure.sh           &&\
	make
	rm -Rf /tmp/$(TARPREFIX) /tmp/$(TARBALL)

stats:
	@echo -n "Lines : "; find . -name '*.[chS]' | xargs cat | wc -l

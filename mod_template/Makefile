  KDIR ?= ~/kernel/linux-6.0.5

FNAME_C := mod_template

PWD            := $(shell pwd)
obj-m          += ${FNAME_C}.o
EXTRA_CFLAGS   += -DDEBUG



all:
	
	make -C $(KDIR) M=$(PWD) modules
install:
	make
	sudo make -C $(KDIR) M=$(PWD) modules_install
	sudo depmod
clean:
	make -C $(KDIR) M=$(PWD) clean
	

INDENT := indent

code-style:
	make indent
	make checkpatch

indent:
	mkdir bkp 2> /dev/null; cp -f *.[chsS] bkp/
	${INDENT} -linux --line-length95 *.[chsS]

checkpatch:
	make clean
	$(KDIR)/scripts/checkpatch.pl --no-tree -f --max-line-length=95 *.[ch]
sa:
	make sa_sparse
	make sa_gcc
	make sa_flawfinder
	make sa_cppcheck
sa_sparse:
	make clean
	make C=2 CHECK="/usr/bin/sparse" -C $(KDIR) M=$(PWD) modules
sa_gcc:
	make clean
	make W=1 -C $(KDIR) M=$(PWD) modules
sa_flawfinder:
	make clean
	flawfinder *.[ch]
sa_cppcheck:
	make clean
	cppcheck -v --force --enable=all -i .tmp_versions/ -i *.mod.c -i bkp/ --suppress=missingIncludeSystem .

PKG_NAME := ${FNAME_C}
tarxz-pkg:
	rm -f ../${PKG_NAME}.tar.xz 2>/dev/null
	make clean
	tar caf ../${PKG_NAME}.tar.xz *
	ls -l ../${PKG_NAME}.tar.xz

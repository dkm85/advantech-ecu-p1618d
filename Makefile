
SRC_DIR := $(shell uname -r|awk -F. \
	'{if($$1>=3) print "above_3.0";\
	else if($$1==2&&$$2==4) print "2.4";\
	else if($$1==2&&$$2==6) print "2.6";\
	else print "ERROR"}')
ifeq ($(SRC_DIR),ERROR)
$(error $(shell uname -r) not support!)
endif

all:
	cd ${SRC_DIR};make $@
install:
	cd ${SRC_DIR};make $@
node:
	cd ${SRC_DIR};make $@
uninstall:
	cd ${SRC_DIR};make $@
clean:
	cd ${SRC_DIR};make $@
test:
	cd ${SRC_DIR};make $@

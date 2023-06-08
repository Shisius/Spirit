
SRC_PATH := .

SPIRIT_DIR_PATH := $(src)/../..

obj-m += $(SRC_PATH)/spiritko.o

ccflags-y:=-I$(SPIRIT_DIR_PATH)/definitions

export KERNEL_SRC=/lib/modules/$(shell uname -r)/build

all:
	@echo "src path = $(SRC_PATH) pwd = $(shell pwd)"
	$(MAKE) -C $(KERNEL_SRC) M=$(shell pwd) modules

modules_install: all
	$(MAKE) -C $(KERNEL_SRC) M=$(shell pwd) modules_install
	$(DEPMOD)

clean:
	rm -f $(SRC_PATH)/*.o *~ core .depend $(SRC_PATH)/.*.cmd $(SRC_PATH)/*.ko $(SRC_PATH)/*.mod.c $(SRC_PATH)/*.mod
	rm -f Module.markers Module.symvers modules.order
	rm -rf .tmp_versions Modules.symvers

SRC_DIR=src
BUILD_DIR=build

APP_NAME=EFIOS1
ARCH=X64
GCCV=GCC5

FAR_BUILD=../../Build/MdeModule/DEBUG_$(GCCV)/$(ARCH)

COMPILE = OvmfPkg/build.sh
COMPILE_FLAGS = -a $(ARCH) -t $(GCCV) -p MdeModulePkg/MdeModulePkg.dsc

$(BUILD_DIR)/BOOTX64.efi: $(BUILD_DIR)/$(APP_NAME).efi
	mv $(BUILD_DIR)/$(APP_NAME).efi $(BUILD_DIR)/BOOTX64.efi
	rm $(BUILD_DIR)/$(APP_NAME).efi

$(BUILD_DIR)/$(APP_NAME).efi: $(FAR_BUILD)/$(APP_NAME).efi
	cp $^ $@

$(FAR_BUILD)/$(APP_NAME).efi: $(SRC_DIR)/main.c $(SRC_DIR)/efilibs.h
	@cd ../.. && ./$(COMPILE) $(COMPILE_FLAGS)

arm/hi3798mx/boot/start.o                 (.text)
arm/hi3798mx/boot/early_uart.o            (.text)
arm/hi3798mx/boot/lowlevel_init.o         (.text)
arm/hi3798mx/boot/emmc_boot.o             (.text)
arm/hi3798mx/boot/chip.o                  (.text)
drivers/ddr/hi3798mx/ddr_training_impl.o           (.text)
drivers/ddr/hi3798mx/ddr_training_ctl.o            (.text)
drivers/ddr/hi3798mx/ddr_training_custom.o         (.text)
drivers/ddr/hi3798mx/ddr_training_boot.o           (.text)
__init_end = .;
ASSERT(((__init_end - __text_start) < 0x4f00), "init sections too big!");

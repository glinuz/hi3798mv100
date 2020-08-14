arm/hi3716mv410/boot/start.o                 (.text)
arm/hi3716mv410/boot/early_uart.o            (.text)
arm/hi3716mv410/boot/lowlevel_init.o         (.text)
arm/hi3716mv410/boot/emmc_boot.o             (.text)
arm/hi3716mv410/boot/chip.o                  (.text)
drivers/ddr/hi3716mv410/ddr_training_impl.o              (.text)
drivers/ddr/hi3716mv410/ddr_training_ctl.o               (.text)
drivers/ddr/hi3716mv410/ddr_training_custom.o            (.text)
drivers/ddr/hi3716mv410/ddr_training_boot.o              (.text)
__init_end = .;
ASSERT(((__init_end - __text_start) < 0x4f00), "init sections too big!");

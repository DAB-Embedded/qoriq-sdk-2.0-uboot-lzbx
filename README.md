# DAB-OWRT-A64 industrial router

## U-boot build instructions

Default configuration: ls1012alzbx_qspi_defconfig

Standalone U-boot build:
```
make mrproper
ARCH=aarch64 make ls1012alzbx_qspi_defconfig
CROSS_COMPILE=aarch64-linux-gnu- ARCH=aarch64 make -j 8
```

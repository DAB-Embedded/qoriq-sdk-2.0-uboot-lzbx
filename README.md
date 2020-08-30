# DAB-OWRT-A64 industrial router

## U-boot build instructions

Default configuration: ls1012alzbx_qspi_defconfig

Script:
```
make mrproper
ARCH=arm make ls1012alzbx_qspi_defconfig
CROSS_COMPILE=<path to your gcc compiler> ARCH=arm make
```

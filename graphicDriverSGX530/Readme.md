# Graphic library compilation for For BeagleBone Black (SGX530)

1- Download latest SGX libraries: <http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/gfxsdk/latest/index_FDS.html>

user guide: <http://processors.wiki.ti.com/index.php/Graphics_SDK_Quick_installation_and_user_guide>

2- Give the rights to executable

    chmod +x Graphics_SDK_setuplinux_hardfp_5_01_01_02.bin

3- Run binary and install appropriate directory

    ./Graphics_SDK_setuplinux_hardfp_5_01_01_02.bin

select only 8.x and SDK for BeagleBone black.

4- Edit Rules.make

    ################# FIELDS MODIFIABLE BY THE USER ###############################
    ############### All the fields below are MANDATORY ############################
    SDK_INSTALL_DIR=/home/sedat/MMIS_ARGE
    LINUX_DEVKIT_PATH=/home/sedat/MMIS_ARGE/toolchain/gcc-linaro-arm-linux-gnueabihf-4.9-2014.09_linux
    CROSS_COMPILE=arm-linux-gnueabihf-
    LINUXKERNEL_INSTALL_DIR=/home/sedat/MMIS_ARGE/KERNEL/ref_kernels/linux-3.14.51-ti-r74
    DESTDIR=/home/sedat/MMIS_ARGE/filesystems/ROOTFS

    #By default all below variables have been initialized with equivalent AMSDK variables
    # Change them to appropriate paths as per your machine/build environment paths.
    # Set home area (ex /home/user/)
    HOME=$(SDK_INSTALL_DIR)/graphic_libs

    # Set Toolchain path (ex /home/user/toolchain/arago-2011.09/armv7a)
    CSTOOL_DIR=$(LINUX_DEVKIT_PATH)

    # Set Tool chain prefix (ex arm-arago-linux-gnueabi- )
    CSTOOL_PREFIX=arm-linux-gnueabihf-

    # Set kernel installation path ( ex /home/user/linux-04.00.01.13 )
    KERNEL_INSTALL_DIR=$(LINUXKERNEL_INSTALL_DIR)

    # Set Target filesystem path ( ex /home/user/targetfs )
    TARGETFS_INSTALL_DIR=$(DESTDIR)

    # Set installation folder
    GRAPHICS_INSTALL_DIR=$(HOME)/Graphics_SDK_5_01_01_02

5- build : Note: \_km can be used for only building linux kernel modules. Instead, now every demo and sdk will be build via all keyword.

    make BUILD=release OMAPES=8.x all

6- Install kernel modules to rootfs

Note that; kernel build may not be necessary, but modules have to be built!!! Below explanation also builds kernel too.

    cd KERNELDIR
    make clean
    make LOADADDR=0x80008000 uImage -j30 ; make dtbs
    sudo cp arch/arm/boot/uImage /var/lib/tftpboot/. -f
    sudo cp arch/arm/boot/dts/am335x-boneblack-4dcape-70t.dtb /var/lib/tftpboot/. -f
    make modules -j30
    make INSTALL_MOD_PATH=/home/sedat/MMIS_ARGE/filesystems/ROOTFS modules_install

7- Return the SGX graphic directoy and install the libraries to rootfs.

    make BUILD=release OMAPES=8.x install

8- Run BeagleBone Balck. Edit env variable in uboot parameters: mem=256M earlyprintk=serial vram=50M

    editenv bootargs
    root=/dev/nfs rw ip=192.168.0.100 console=ttyO0 nfsroot=192.168.0.1:/home/sedat/MMIS_ARGE/filesystems/ROOTFS mem=256M earlyprintk=serial vram=50M
    saveenv
    reset

9- Load modules and run examples:

    /etc/init.d/335x-demo
    /opt/gfxsdkdemos/ogles2/OGLES2ChameleonMan

If there is a problem related to loading module; put below lines into "/lib/modules/3.14.52/modules.dep" file

    extra/omaplfb.ko: extra/pvrsrvkm.ko
    extra/pvrsrvkm.ko:

10- If every things goes fine, you will see graphics on screen. Extra; edit "/etc/init.d/335x-demo" in order to fast initialization

    if test $BUILDTYPE = "release8.x"
    then
      echo "installing 8.x SGX release user libraries"
      cd /opt/gfxlibraries/gfx_rel_es8.x
      ./install.sh --no-x
    else 
      devmem2 0x44E0007C w 0x2 
      echo "installing 8.x SGX debug user libraries"
      cd /opt/gfxlibraries/gfx_dbg_es8.x
      ./install.sh --no-x
    fi


# References

* [Graphics_SDK_Quick_installation_and_user_guide](http://processors.wiki.ti.com/index.php/Graphics_SDK_Quick_installation_and_user_guide)
* [Building_Qt_for_OMAP35xx_with_OpenGL_ES_accelerated_by_SGX](http://processors.wiki.ti.com/index.php/Building_Qt_for_OMAP35xx_with_OpenGL_ES_accelerated_by_SGX)
* [SGX Release_mode_operation](http://processors.wiki.ti.com/index.php?title=SGXDbg#Release_mode_operation)

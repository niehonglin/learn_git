# Tool chain settings
XTENSA_CORE=KF1_prod_rel_2012_4
XTENSA_TOOLS_ROOT=/cad/tensilica/tools/RD-2012.4-linux/XtensaTools
XTENSA_ROOT=/cad/tensilica/chips/kingfisher/RD-2012.4-linux/$XTENSA_CORE

XTENSA_SYSTEM=$XTENSA_ROOT/config

#LM_LICENSE_FILE=/home/zhuzuoliang/AR4004-3.2/trunk/code/target/xtensa/license.dat
LM_LICENSE_FILE=~/IOT/license.dat
echo $LM_LICENSE_FILE
echo "SDK_ROOT="$SDK_ROOT
ifconfig eth0 down
ifconfig eth0 hw ether 00:0c:29:01:02:03
ifconfig eth0 up
    
PATH=$PATH:$XTENSA_TOOLS_ROOT/bin

export LM_LICENSE_FILE XTENSA_TOOLS_ROOT XTENSA_ROOT XTENSA_SYSTEM XTENSA_CORE PATH

export XTENSA_PREFER_LICENSE=XT-GENERIC

. sdkenv.sh

#prepare for make flash image
echo $SDK_ROOT
cd $SDK_ROOT/demo/sdk_shell
#cd $SDK_ROOT/demo/uart_at

make clean

make 

cd $SDK_ROOT


echo "++++++++++++++++++++++++++++++++++make img++++++++++++++++++++++++++++++++++"
cd $SDK_ROOT/demo/sdk_flash
echo "make"
make clean
make
echo $*

source make_flash.sh

cd $SDK_ROOT



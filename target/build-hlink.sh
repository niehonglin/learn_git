# Tool chain settings
XTENSA_CORE=KF1_prod_rel_2012_4
XTENSA_TOOLS_ROOT=/cad/tensilica/tools/RD-2012.4-linux/XtensaTools
XTENSA_ROOT=/cad/tensilica/chips/kingfisher/RD-2012.4-linux/$XTENSA_CORE

XTENSA_SYSTEM=$XTENSA_ROOT/config

LM_LICENSE_FILE=~/qca4004-tx-1-4_qca_oem_ext.git/license.dat
PATH=$PATH:$XTENSA_TOOLS_ROOT/bin

export LM_LICENSE_FILE XTENSA_TOOLS_ROOT XTENSA_ROOT XTENSA_SYSTEM XTENSA_CORE PATH

export XTENSA_PREFER_LICENSE=XT-GENERIC

. sdkenv.sh

#DUAL_UART="yes"
 
#COM_UART="PIN2_PIN7"
COM_UART="PIN6_PIN7"
#COM_UART="PIN10_PIN11"

##prepare for make flash image
#echo "make apps lib++++++++++"
#echo $SDK_ROOT
#cd $SDK_ROOT/apps/ceacdemo
#make clean
#make 
##prepare for make flash image
#echo "make airkiss lib++++++++++"
#echo $SDK_ROOT
#cd $SDK_ROOT/airkiss/lib
#make clean
#make 
#cd $SDK_ROOT/airkiss/
#make clean
#make 


#cd $SDK_ROOT
#cd $SDK_ROOT/http_ota/
#make clean

#make 
#if [ $? != 0 ]; then
#echo "make failed!";
#exit 1;
#fi


#prepare for make flash image
echo $SDK_ROOT
echo "R00113.2 release+++++++++++++++++++++++sdk shell++++++++++++++++++++++++++++++"


if [ -n "$DUAL_UART" ]; then

#sudo apt-get install libxml2
#sudo apt-get install libxml2-dev


	echo   "DUAL_UART DEMO"
	cd $SDK_ROOT/demo/dual_uart_demo
	make clean
	make 
	if [ $? != 0 ]; then
	echo "make failed!";
	exit 1;
	fi

	cd $SDK_ROOT/image
	cp -fv dual_uart_demo.out iot_demo.out
	cd $SDK_ROOT

else
	echo "Console DEMO"

	cd $SDK_ROOT/demo/wewins_hlink
	make clean
	
	make 2>error.txt
#	make	
	if [ $? != 0 ]; then
	echo "make failed!";
	exit 1;
	fi
	
	cd $SDK_ROOT
fi

cd $SDK_ROOT/tool

if [ $COM_UART = "PIN2_PIN7" ];
then 
	echo "COM_UART is PIN2_PIN7"
	cp -fv tunable/tunable_input_sp144_hostless_1bitflash_dual_uart_demo.txt tunable/tunable_input.txt
fi

if [ $COM_UART = "PIN6_PIN7" ];
then  
	echo "COM_UART is PIN6_PIN7"
	cp -fv tunable/tunable_input_uart0_67_uart1_1011.txt      tunable/tunable_input.txt
fi

if [ $COM_UART = "PIN10_PIN11" ];
then 
	echo "COM_UART is PIN10_PIN11"
	cp -fv tunable/tunable_input_uart0_1011_uart1_67.txt      tunable/tunable_input.txt
fi


./qonstruct.sh --qons tunable/

rm -f tunable/tunable_input.txt
rm -f tunable/.qons_cache

cd $SDK_ROOT
echo "make img finish++++"

echo "++++++++++++++++++++++++++++++++++copy img++++++++++++++++++++++++++++++++++"
cp -fv $SDK_ROOT/bin/raw_flashimage_AR400X_REV4_IOT_hostless_unidev_dualband.bin  raw_hlink.bin

#exit 1;


#!/bin/bash

# Copyright (c) 2014 Qualcomm Atheros Inc.
# All rights reserved.

# Create an image to be used with an Over The Air (OTA) flash firmware upgrade.
# Works by prepending an OTA header to a partition flash image.
# Usage ./ota.sh input_image ota_output_image

PRINTF="/usr/bin/printf"

dbg()
{
    if [ "$DEBUG" ]; then
        echo $0: $*
    fi
}

# Append a 32-bit word to the output file.
print_32bit()
{
	word32=$($PRINTF "%08x\n" $1)

	byte0=$(echo $word32 | cut -b 7-8)
	byte1=$(echo $word32 | cut -b 5-6)
	byte2=$(echo $word32 | cut -b 3-4)
	byte3=$(echo $word32 | cut -b 1-2)

	$PRINTF "\x$byte3" >> $out_file
	$PRINTF "\x$byte2" >> $out_file
	$PRINTF "\x$byte1" >> $out_file
	$PRINTF "\x$byte0" >> $out_file
}


# Add a file (optionally compressing it) at a specified address to the output file.
add_ota_header() {
	in_file=$1
	out_file=$2
	LENGTH=$(stat --format=%s $in_file)
	dbg original image length=$LENGTH

	# Remove the leading 6 words which are always in the raw_flashimage:
	#   pointer to partition table
	#   3 TBD words
	#   1 Magic number word
	#   1 partition age word
	# These last two words will be added back by OTA Upgrade firmware
	# when the data is committed to flash.
	let LENGTH=LENGTH-24

	tail -c $LENGTH $in_file > tmp.bin

	MD5=$(md5sum tmp.bin | cut -f1 -d " ")
	dbg md5=$MD5
	
	rm -f $out_file

	print_32bit 0x1234
	print_32bit $LENGTH
	print_32bit 0x$(echo $MD5 | cut -b 1-8)
	print_32bit 0x$(echo $MD5 | cut -b 9-16)
	print_32bit 0x$(echo $MD5 | cut -b 17-24)
	print_32bit 0x$(echo $MD5 | cut -b 25-32)
	cat tmp.bin >> $out_file
	rm -f tmp.bin
}

print_32bit_little_endian()
{
   word32=$($PRINTF "%08x\n" $1)

   byte0=$(echo $word32 | cut -b 7-8)
   byte1=$(echo $word32 | cut -b 5-6)
   byte2=$(echo $word32 | cut -b 3-4)
   byte3=$(echo $word32 | cut -b 1-2)

   $PRINTF "\x$byte0" >> $out_file
   $PRINTF "\x$byte1" >> $out_file
   $PRINTF "\x$byte2" >> $out_file
   $PRINTF "\x$byte3" >> $out_file
}

revise_ro_start () {
#   hexdump $1

   WORD_L=`tail -c 16 $1 | hexdump | cat | cut -c 19-22`
   WORD_H=`tail -c 16 $1 | hexdump | cat | cut -c 24-27`

   HEX_STR_L="0x${WORD_L}"
   HEX_STR_H="0x${WORD_H}"

   VAL_L=$(($HEX_STR_L))
   VAL_H=$(($HEX_STR_H))

   VAL=$(($VAL_L + $VAL_H * 65536))
   VAL2=$(($VAL-16))

   out_file="tmp.bin"
   out_file2="tmp2.bin"
   out_file3="tmp3.bin"

   head -c 20 $1 > $out_file
   print_32bit_little_endian $VAL2
   tail -c 8 $1 > $out_file2
   cat $out_file $out_file2 > $out_file3
   mv $out_file3 $2

   rm -f $out_file
   rm -f $out_file2
   rm -f $out_file3

#   hexdump $2
}

dbg Start $*

if [ \! -r $1 ]; then
    echo $0 Error: Cannot access $1
    exit 1
fi

FW_SIZE=$3
RO_START=$4
RO_SIZE=$5

#printf "FW size=%d  RO start=%d  size=%d\n" $FW_SIZE $RO_START $RO_SIZE

NVRAM_CONF="nvram_config.bin"
NVRAM_CONF_NEW="nvram_config.new"

dd if=$1 ibs=1 obs=1 skip=$FW_SIZE count=32 of=$NVRAM_CONF
revise_ro_start $NVRAM_CONF $NVRAM_CONF_NEW

#OTA_TMP="ota_tmp.bin"
OTA_TMP="ota_tmp.bin"
OTA_TMP_ALL="ota_tmp_all.bin"
TMP="tmp.bin"
TMP2="tmp2.bin"

TOTAL_SIZE=$(($RO_START + $RO_SIZE))
head -c $TOTAL_SIZE $1 > $OTA_TMP_ALL

head -c $FW_SIZE $1 > $OTA_TMP
cat $OTA_TMP $NVRAM_CONF_NEW > $TMP
LEFT=$(($TOTAL_SIZE - $FW_SIZE - 32));
tail -c $LEFT $OTA_TMP_ALL > $TMP2
cat $TMP $TMP2 > $OTA_TMP

rm $NVRAM_CONF
rm $NVRAM_CONF_NEW
rm $OTA_TMP_ALL
rm $TMP
rm $TMP2

OTA_LENGTH=$(stat --format=%s $OTA_TMP)
#printf "OTA size=%d \n" $OTA_LENGTH

add_ota_header $OTA_TMP $2

rm $OTA_TMP

if [ \! -f "$2" ]; then
    echo $0 Error: failed to create $2
    exit 1
else
    echo Creating image: $(basename "$2")
fi

dbg End
exit 0

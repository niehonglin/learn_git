#!/bin/bash

# Customizable variables
PARTITION_ENTRY_1=${PARTITION_ENTRY_1:-0x00040000}
PARTITION_ENTRY_2=${PARTITION_ENTRY_2:-0xFFFFFFFF}
PARTITION_ENTRY_3=${PARTITION_ENTRY_3:-0xFFFFFFFF}
PARTITION_TABLE_SIZE=${PARTITION_TABLE_SIZE:-12}
# By default, the partition table is located at the end of the first partition
PARTITION_TABLE_START=${PARTITION_TABLE_START:-$(($PARTITION_ENTRY_1 - $PARTITION_TABLE_SIZE))}
FLASH_CAPACITY=${FLASH_CAPACITY:-0x00080000}
FLASH_BLOCKSIZE=${FLASH_BLOCKSIZE:-0x00010000}
FLASH_SECTORSIZE=${FLASH_SECTORSIZE:-0x00001000}
FLASH_PAGESIZE=${FLASH_PAGESIZE:-0x00000100}
FLASH_HDR=${FLASH_HDR:-""}

# Sample values for ~64KB RO dsets and ~64KB RW dsets
# positioned at the end of the flash partition
# (For NO datasets, use 0xffffffff for all four R*DATASET_* values)
RODATASET_MAXSIZE=${RODATASET_MAXSIZE:-$((0x6000))}
RWDATASET_MAXSIZE=${RWDATASET_MAXSIZE:-$((0xa000))}
RWDATASET_END=${RWDATASET_END:-$(($PARTITION_TABLE_START & ~($FLASH_SECTORSIZE-1)))}
RWDATASET_START=${RWDATASET_START:-$(($RWDATASET_END- $RWDATASET_MAXSIZE))}
#RODATASET_START=${RODATASET_START:-$(($RWDATASET_START - $RODATASET_MAXSIZE))}

# SBL_CFG_FILENAME - Secondary boot loader configuration
SBL_CFG_FILENAME="sbl_cfg_hostless.bin"
# SBL_CFG_NAME - is the flag name
SBL_CFG_NAME="sbl_config"
SBL_TRIGGER_METHOD=${SBL_TRIGGER_METHOD:-0x00000000}
METHOD1_ASSERT_PIN=${METHOD1_ASSERT_PIN:-0x00000015}
METHOD2_DELAY_TIME=${METHOD2_DELAY_TIME:-0x00000005}
METHOD2_RCV_CHAR=${METHOD2_RCV_CHAR:-0x00000078}
METHOD2_RCV_TIMES=${METHOD2_RCV_TIMES:-0x00000005}
MENU_CMD_UPGRADE_NAME=${MENU_CMD_UPGRADE_NAME:-0x00000075}
MENU_CMD_RESET_NAME=${MENU_CMD_RESET_NAME:-0x00000072}
DBG_UART_RXPIN=${DBG_UART_RXPIN:-0x0000000A}
DBG_UART_TXPIN=${DBG_UART_TXPIN:-0x0000000B}
SBL_OUTPUT_ENABLE=${SBL_OUTPUT_ENABLE:-0x00000001}

TUNE_APP=${TUNE_APP:-$IMAGEDIR/tune.out}
SDK_PROXY_APP=${SDK_PROXY_APP:-$IMAGEDIR/sdk_proxy.out}
BOOT_ROUTER_APP=${BOOT_ROUTER_APP:-$IMAGEDIR/boot_router.out}
OTP_APP=${OTP_APP:-$IMAGEDIR/otp_iot.out}
SBL_APP=${SBL_APP:-$IMAGEDIR/sbl.out}
MAKE_FLASH_APPS_ONLY=${MAKE_FLASH_APPS_ONLY:-0}

dbg()
{
    if [ "$DEBUG" ]; then
        echo $(basename $0) DEBUG: $*
    fi
}

# Switch endianness of a 32-bit word
swizzle_word()
{
	word32=$($PRINTF "%08x\n" $1)

	byte0=$(echo $word32 | cut -b 7-8)
	byte1=$(echo $word32 | cut -b 5-6)
	byte2=$(echo $word32 | cut -b 3-4)
	byte3=$(echo $word32 | cut -b 1-2)

        echo 0x$byte0$byte1$byte2$byte3
}

to_hex()
{
	echo $($PRINTF "0x%08x\n" $1)
}

dbg Start $*

if [ -z "$APPS_OUTFILE" ]; then
    echo $0 ERROR: No APPS_OUTFILE specified
    exit 1
fi
if [ \! -r "$APPS_OUTFILE" ]; then
    echo $0 ERROR: Cannot access APPS_OUTFILE, $APPS_OUTFILE
    exit 1
fi
dbg APPS_OUTFILE=$APPS_OUTFILE

OBJDUMP=${OBJDUMP:-objdump}
NM=${NM:-nm}
PATTERN_FIND_TOOL="extract.out"

OTA_FILENAME="${BINDIR}/ota_image${FLASH_FILE_SUFFIX}"
FLASHER_FILENAME="${BINDIR}/flash${FLASH_FILE_SUFFIX}"
RAW_FILENAME="${BINDIR}/raw_flashimage${FLASH_FILE_SUFFIX}"

# Lots of internal variables
INCLUDEDIR=${FW}/include
BIN_FILENAME="flashimage_hostless.bin"
CFG_FILENAME="config_header_hostless.bin"
FLASH_APP_BIN="sdk_flash.bin"
FLASH_APP_OUT="sdk_flash.out"
PART_TABLE_FILENAME="parttable_hostless.bin"
FLASH_DESC_PATTERN_OFFSET=20
INC_LENGTH=0
CURR_DIR=$((pwd))
# FLASH_DESC_FILENAME - is a file that describes the flash chip
FLASH_DESC_FILENAME="flash_desc_hostless.bin"
# FLASH_DESC_NAME - is the input name for nvram_seek()
FLASH_DESC_NAME="flash_descriptor"


#
# Hostproxy param is athwlan_start_addr<<8 + EXT_BD_LENGTH
# this results in the start address and the BD_LENGTH overlapping by 4 bits which is ok as
# it is assumed the lower 4-bits of the start address will always be 0.
#

START_ADDR=0x$($OBJDUMP -f $APPS_OUTFILE | grep "start address" | cut -b 17-24)
dbg START_ADDR=$(to_hex $START_ADDR)
START_ADDR=$($PRINTF "%d\n" $START_ADDR)
START_ADDR=$(($START_ADDR*256))
PROXY_PARAM=$(($START_ADDR+$PROXY_PARAM_DEV_MODE))
dbg proxy param =$(to_hex $PROXY_PARAM)

#
# If we did not specify an APPS_FILENAME, supply
# a default and build it from scratch.  If we DID
# supply such a name, then only build it if it
# doesn't already exist.
#
if [ -z "$APPS_FILENAME" ]; then
    APPS_FILENAME=flashapps.bin
    rm -f $APPS_FILENAME
else
    dbg Using pre-built flashapps.bin
fi

dbg DSET_ADDR=$(to_hex $DSET_ADDR)
dbg VER=$AR6002_REV7_VER
dbg TUNE_PARAM=$(to_hex "$TUNE_PARAM")
########################################
# Create the applications segment of the flash image from ELF files
# NVRAM format:
#     32-bit pointer to partition table
#     32-bit unused
#     32-bit unused
#     32-bit unused
#     (Start of partition 0)
#     32-bit partition magic value (validates partition)
# START post-REV72 addition {
#     32-bit partition age (used when selecting which partition to use)
#     32-bit unused
#     32-bit unused
#     32-bit unused
#     32-bit unused
#     32-bit unused
#     32-bit unused
# END post_rev72 addition }
#     first meta-data entry
#     first data
#     second meta-data entry
#     second data
#      ....
########################################

# Partition header preceded by the 4-word flash header
# for partition 0.  This will be stripped out later if
# the image is not intended for use as partition 0.

${TOOLDIR}/makeimg.sh \
        -out $APPS_FILENAME \
        -new \
        -word $PARTITION_TABLE_START \
        -fill 0xffffffff 3 \
        -magic \
        -word 0 \
        -fill 0xffffffff 6

if [ "$TUNE_APP" != "none" ]; then
    dbg ${TOOLDIR}/makeimg.sh -out $APPS_FILENAME -name tune -exec "$TUNE_APP" $TUNE_PARAM
    ${TOOLDIR}/makeimg.sh -out $APPS_FILENAME -name tune -exec "$TUNE_APP" $TUNE_PARAM
fi

if [ "$SDK_PROXY_APP" != "none" ]; then
    dbg ${TOOLDIR}/makeimg.sh -out $APPS_FILENAME -name hostproxy -exec "$SDK_PROXY_APP" $PROXY_PARAM
    ${TOOLDIR}/makeimg.sh -out $APPS_FILENAME -name hostproxy -exec "$SDK_PROXY_APP" $PROXY_PARAM
fi

if [ "$BD_FILENAME" != "none" ]; then
    dbg ${TOOLDIR}/makeimg.sh -out $APPS_FILENAME -name boarddata -bddata "$BD_FILENAME"
    ${TOOLDIR}/makeimg.sh -out $APPS_FILENAME -name boarddata -bddata "$BD_FILENAME"
fi

if [ "$BOOT_ROUTER_APP" != "none" ]; then
    dbg ${TOOLDIR}/makeimg.sh -out $APPS_FILENAME -name bootrouter -exec "$BOOT_ROUTER_APP" 0x0
    ${TOOLDIR}/makeimg.sh -out $APPS_FILENAME -name bootrouter -exec "$BOOT_ROUTER_APP" 0x0
fi

if [ "$OTP_APP" != "none" ]; then
    dbg ${TOOLDIR}/makeimg.sh -out $APPS_FILENAME -name otp -exec "$OTP_APP" 0x2
    ${TOOLDIR}/makeimg.sh -out $APPS_FILENAME -name otp -exec "$OTP_APP" 0x2
fi

if [ "$SBL_APP" != "none" ]; then
    dbg ${TOOLDIR}/makeimg.sh -out $APPS_FILENAME -name sbl -exec "$SBL_APP" 0x0
    ${TOOLDIR}/makeimg.sh -out $APPS_FILENAME -name sbl -exec "$SBL_APP" 0x0
fi

if [ "$DSET_PATCH_FILENAME" != "none" ]; then
    dbg ${TOOLDIR}/makeimg.sh -out $APPS_FILENAME -name dsetpatch -data "$DSET_PATCH_FILENAME" $DSET_ADDR
    ${TOOLDIR}/makeimg.sh -out $APPS_FILENAME -name dsetpatch -data "$DSET_PATCH_FILENAME" $DSET_ADDR
fi

# Finally, add the main application to the flash image
dbg ${TOOLDIR}/makeimg.sh -out $APPS_FILENAME -name main_app -stop -load $APPS_OUTFILE 0x0
${TOOLDIR}/makeimg.sh -out $APPS_FILENAME -name main_app -stop -load $APPS_OUTFILE 0x0

if [ "$MAKE_FLASH_APPS_ONLY" -eq 1 ]; then
    exit
fi


########################################
# Create RAW flash image that can be written to flash chips directly.
# (e.g. via JTAG using gdb.sdk_flash)
########################################

# Prepare to construct the remainder of the flash image, starting
# with the "apps section" that we just made.
cp $APPS_FILENAME $BIN_FILENAME

# Construct ReadOnly DataSet Image
# File to contain Read-Only DataSet image
export RODATASET_FILENAME=${RODATASET_FILENAME:-"rodsetimage.bin"}
./makedsets.sh

dbg RWDATASET START=$(to_hex $RWDATASET_START)  SIZE=$RWDATASET_MAXSIZE
dbg RODATASET START=$(to_hex $RODATASET_START)  SIZE=$RODATASET_MAXSIZE

if [ -r "$RODATASET_FILENAME" ]; then
    RODATASET_LENGTH=$(stat --format=%s $RODATASET_FILENAME)
    if [ $RODATASET_LENGTH -gt $RODATASET_MAXSIZE ]; then
        echo "Error: ReadOnly DataSet image is too large."
        exit
    fi
else
    dbg Not using RODATASET
    RODATASET_FILENAME=""
    RODATASET_LENGTH=0
fi

FLASH_UNUSED=0xffffffff
FLASH_RODSET_START=$RODATASET_START
FLASH_RWDSET_START=$RWDATASET_START
FLASH_RWDSET_SZ=$RWDATASET_MAXSIZE



# SBL configuration
${TOOLDIR}/makeimg.sh \
        -out $SBL_CFG_FILENAME \
        -new \
        -word  $SBL_TRIGGER_METHOD \
        -word  $METHOD1_ASSERT_PIN \
        -word  $METHOD2_DELAY_TIME \
        -word  $METHOD2_RCV_CHAR \
        -word  $METHOD2_RCV_TIMES \
        -word  $MENU_CMD_UPGRADE_NAME \
        -word  $MENU_CMD_RESET_NAME \
        -word  $DBG_UART_RXPIN \
        -word  $DBG_UART_TXPIN \
        -word  $SBL_OUTPUT_ENABLE

# Append sbl_configuration
${TOOLDIR}/makeimg.sh \
        -out $BIN_FILENAME \
        -name $SBL_CFG_NAME -noload $SBL_CFG_FILENAME

# Flash descriptor, stored in flash
${TOOLDIR}/makeimg.sh \
        -out $FLASH_DESC_FILENAME \
        -new \
        -word  $FLASH_CAPACITY \
        -word  $FLASH_BLOCKSIZE \
        -word  $FLASH_SECTORSIZE \
        -word  $FLASH_PAGESIZE \
	-word  $FLASH_UNUSED \
	-word  $FLASH_UNUSED \
	-word  $FLASH_RWDSET_START \
	-word  $FLASH_RWDSET_SZ

# Get current length of image so we know how much padding to add
FW_LENGTH=$(stat --format=%s $BIN_FILENAME)
FLASH_DESC_LEN=$(stat --format=%s $FLASH_DESC_FILENAME)

# 40 -- meta data size for each of section
IMG_LENGTH=$(($FW_LENGTH + $FLASH_DESC_LEN + 40))
IMG_LENGTH_FW=$(($FW_LENGTH + 40))

dbg IMG_LENGTH $IMG_LENGTH $FW_LENGTH

FLASH_RODSET_START=${RODATASET_START:-$((($IMG_LENGTH + 15)&~0x0F))}

rm -f $FLASH_DESC_FILENAME

# Flash descriptor, stored in flash
${TOOLDIR}/makeimg.sh \
        -out $FLASH_DESC_FILENAME \
        -new \
        -word  $FLASH_CAPACITY \
        -word  $FLASH_BLOCKSIZE \
        -word  $FLASH_SECTORSIZE \
        -word  $FLASH_PAGESIZE \
        -word  $FLASH_UNUSED \
        -word  $FLASH_RODSET_START \
        -word  $FLASH_RWDSET_START \
        -word  $FLASH_RWDSET_SZ

# Append flash_descriptor to the flash image
${TOOLDIR}/makeimg.sh \
        -out $BIN_FILENAME \
        -name $FLASH_DESC_NAME -noload $FLASH_DESC_FILENAME


# Get current length of image so we know how much padding to add
FLASH_LENGTH=$(stat --format=%s $BIN_FILENAME)

rm -f sub_fill1.bin sub_fill2.bin
if [ $RODATASET_LENGTH -gt 0 ]; then
    # pad1 covers from end of flash image to start of RO dsets
    PAD1_LENGTH=$(($FLASH_RODSET_START - $FLASH_LENGTH))

    # pad2 covers from end of RO dsets to start of partition table
    PAD2_LENGTH=$(($PARTITION_TABLE_START - $FLASH_RODSET_START - $RODATASET_LENGTH))
else
    # Pad the whole area from end of flash to start of partition table
    # This works regardless of whether or a RW dset area is defined.
    PAD1_LENGTH=0
    PAD2_LENGTH=$(($PARTITION_TABLE_START - $FLASH_LENGTH))
fi

if [ $PAD1_LENGTH -gt 0 ]; then
    tail -c $PAD1_LENGTH fill.bin >> sub_fill1.bin
else
    touch sub_fill1.bin
fi

if [ $PAD2_LENGTH -gt 0 ]; then
    tail -c $PAD2_LENGTH fill.bin >> sub_fill2.bin
else
    touch sub_fill2.bin
fi

# Create the partition table, to be appended to the flash image
# (If not partition 0, will be stripped out later.)
${TOOLDIR}/makeimg.sh \
        -out $PART_TABLE_FILENAME \
        -new \
        -word $PARTITION_ENTRY_1 \
        -word $PARTITION_ENTRY_2 \
        -word $PARTITION_ENTRY_3

echo Creating image: $(basename "$RAW_FILENAME")
cat $BIN_FILENAME sub_fill1.bin $RODATASET_FILENAME sub_fill2.bin $PART_TABLE_FILENAME > $RAW_FILENAME
# Note: At this point, the image does not yet contain RO DataSets (e.g. HTML)

rm -f sub_fill1.bin
rm -f sub_fill2.bin

# Done creating RAW image


########################################
# At this point we have created a raw flash image that can be
# committed to flash (RAW_FILENAME). We use that raw flash image
# to create an image that can be used for OTA upgrades.
########################################

#
# TBD: Should use a *segmented* image for OTA upgrade
#

${TOOLDIR}/ota.sh $RAW_FILENAME $OTA_FILENAME $IMG_LENGTH_FW $FLASH_RODSET_START $RODATASET_LENGTH

if [ $? -ne 0 ]; then
    echo $0 Error: ota.sh failed to create OTA image
    exit 1
fi

dbg End
exit 0

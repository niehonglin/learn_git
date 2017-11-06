# *****************************************************************************
# Copyright 2013-2014, Qualcomm Connected Experiences, Inc.
#
# *****************************************************************************

#############################################################
# Required variables for each makefile
# Discard this section from all parent makefiles
# Expected variables (with automatic defaults):
#   CSRCS (all "C" files in the dir)
#   SUBDIRS (all subdirs with a Makefile)
#   GEN_LIBS - list of libs to be generated ()
#   GEN_IMAGES - list of images to be generated ()
#   COMPONENTS_xxx - a list of libs/objs in the form
#     subdir/lib to be extracted and rolled up into
#     a generated lib/image xxx.a ()
#
ifndef PDIR

GEN_LIBS = liballjoyn.a
CSRCS = \
	../core/ajtcl/src/aj_about.c \
	../core/ajtcl/src/aj_bufio.c \
	../core/ajtcl/src/aj_bus.c \
	../core/ajtcl/src/aj_cert.c \
	../core/ajtcl/src/aj_connect.c \
	../core/ajtcl/src/aj_crc16.c \
	../core/ajtcl/src/aj_creds.c \
	../core/ajtcl/src/aj_crypto.c \
	../core/ajtcl/src/aj_debug.c \
	../core/ajtcl/src/aj_disco.c \
	../core/ajtcl/src/aj_guid.c \
	../core/ajtcl/src/aj_helper.c \
	../core/ajtcl/src/aj_init.c \
	../core/ajtcl/src/aj_introspect.c \
	../core/ajtcl/src/aj_keyauthentication.c \
	../core/ajtcl/src/aj_keyexchange.c \
	../core/ajtcl/src/aj_link_timeout.c \
	../core/ajtcl/src/aj_msg.c \
	../core/ajtcl/src/aj_nvram.c \
	../core/ajtcl/src/aj_peer.c \
	../core/ajtcl/src/aj_std.c \
	../core/ajtcl/src/aj_util.c \
	../core/ajtcl/crypto/ecc/aj_crypto_ecc.c \
	../core/ajtcl/crypto/ecc/aj_crypto_sha2.c \
	../core/ajtcl/external/sha2/sha2.c \
    ../core/ajtcl/malloc/aj_malloc.c \
	../aj_qca4004/src/aj_malloc.c \
	../aj_qca4004/src/aj_net.c \
	../aj_qca4004/src/aj_target_crypto.c \
	../aj_qca4004/src/aj_target_nvram.c \
	../aj_qca4004/src/aj_target_util.c \
	../aj_qca4004/src/aj_wifi_ctrl.c \
	../aj_qca4004/src/alljoyn.c \

#	../core/ajtcl/crypto/aj_sw_crypto.c \

REMOVED_CSRCS = \



#############################################################

endif

SRCS    = $(CSRCS) $(SSRCS)
COBJS   = $(CSRCS:.c=.o)
SOBJS   = $(SSRCS:.S=.o)
OBJS    = $(COBJS) $(SOBJS)
DOBJS   = $(DSRCS:.c=.o)


CCFLAGS := $(CCFLAGS) 

CCFLAGS := $(CCFLAGS) -g3
CCFLAGS := $(CCFLAGS) -Os
ifeq ($(AR6002_REV), 7) # {
CCFLAGS := $(CCFLAGS) -DATH_TARGET -DAR6K -DAR6002 -DAR6002_REV7
ifeq ($(AR6002_REV7_VER), 4) # {
CCFLAGS := $(CCFLAGS) -DAR6002_REV74
endif # AR6002_REV7_VER4 }
ifeq ($(AR6002_REV7_VER), 5) # {
CCFLAGS := $(CCFLAGS) -DAR6002_REV75
endif # AR6002_REV7_VER5 }
endif # AR6002_REV7 }

CCFLAGS := $(CCFLAGS) -include fwconfig_AR6006.h -Wno-return-type
CCFLAGS := $(CCFLAGS) -fno-inline-functions -nostdlib -mlongcalls -ffunction-sections

CCFLAGS := $(CCFLAGS) -DALIGNED_ACCESS_REQUIRED 

# set the default value to EMPTY, which implies a non-debug build
AJ_DEBUG_RESTRICT?=EMPTY

ifeq ($(AJ_DEBUG_RESTRICT), EMPTY)
DEFINES += -DNDEBUG
else
# use the supplied value to control how much debug info is available
DEFINES += -DAJ_DEBUG_RESTRICT=$(AJ_DEBUG_RESTRICT)
endif

CCFLAGS := $(CCFLAGS) $(DEFINES)



#############################################################
# Configuration i.e. compile options etc.
# Target specific stuff (defines etc.) goes in here!
# Generally values applying to a tree are captured in the
#   makefile at its root level - these are then overridden
#   for a subtree within the makefile rooted therein
#

INCLUDES := $(INCLUDES) \
			-I ../core/ajtcl/inc \
			-I ../core/ajtcl/malloc \
			-I ../core/ajtcl/external/sha2 \
			-I ../aj_qca4004/inc \
			-I$(WLANCMN_DIR)/include \
			-I$(WLANCMN_DIR)/include/$(TARGET) \
			-I$(WLANCMN_DIR)/include/$(HW_HDRS) \
			-I$(WLANFW_DIR)/import/$(BUILD_VARIANT)/include \
			-I $(WLANFW_DIR)/include 

INCLUDES := $(INCLUDES) -I . -I $(PDIR)include 
INCLUDES := $(INCLUDES) -I $(WLANCMN_DIR)/include/cust 
INCLUDES := $(INCLUDES) -I $(WLANCMN_DIR)/include/qcom 
INCLUDES := $(INCLUDES) -I $(WLANFW_DIR)/import/rev7/include/ 

#-DREV74_TEST_ENV1 -DAR6002_REV74





############################################################
# Create a subdirectory for AllJoyn objects 
#

all: liballjoyn.a

liballjoyn.a: $(OBJS)
	xt-ar $(ARFLAGS) $@ $(OBJS)

clean: 
	rm -f ../core/ajtcl/src/*.o
	rm -f ../core/ajtcl/malloc/*.o
	rm -f ../core/ajtcl/external/sha2/*.o
	rm -f ../core/ajtcl/crypto/ecc/*.o
	rm -f ../aj_qca4004/src/*.o
	rm -f liballjoyn.a

clobber: clean

%.o: %.c
	xt-xcc $(CCFLAGS) $(INCLUDES) -c $< -o $@ 



# Copyright (c) 2011 Qualcomm Atheros, Inc.
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.
# $ATH_LICENSE_TARGET_MK$
#
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

GEN_LIBS = libramcust.a
SRC_DIR := ..
endif


#############################################################
# Configuration i.e. compile options etc.
# Target specific stuff (defines etc.) goes in here!
# Generally values applying to a tree are captured in the
#   makefile at its root level - these are then overridden
#   for a subtree within the makefile rooted therein
#


#############################################################
# Recursion Magic - Don't touch this!!
#
# Each subtree potentially has an include directory
#   corresponding to the common APIs applicable to modules
#   rooted at that subtree. Accordingly, the INCLUDE PATH
#   of a module can only contain the include directories up
#   its parent path, and not its siblings
#
# Required for each makefile to inherit from the parent
#
DEFINES += -UCONFIG_STRRCL_FEATURE -UP2P_ENABLED -UCONFIG_P2P_FEATURE -UUARTCB_SUPPORT
INCLUDES := $(INCLUDES) \
		-I. \
		-I $(PDIR)include  \
		-I$(WLANFW_DIR)/include \
		-I$(WLANCMN_DIR)/include/qcom \
		-I$(WLANFW_DIR)/src/ \
		-I$(WLANFW_DIR)/src/wlan/include \
		-I$(WLANFW_DIR)/src/wlan/wps/include/ \
		-I$(WLANFW_DIR)/src/wlan/wlan_wmi \
		-I$(WLANFW_DIR)/src/wlan/wlan_init \
		-I$(WLANFW_DIR)/src/wlan/wps/include/ \
		-I$(WLANFW_DIR)/src/wlan/include \
		-I$(WLANFW_DIR)/src/wlan/ap \
		-I$(WLANFW_DIR)/src/wlan/txrx/include \
		-I$(WLANFW_DIR)/src/hostif/htc/ \
		-I$(WLANFW_DIR)/src/ipstack/port \
		-I$(WLANFW_DIR)/src/ipstack/socketlayer \
		-I$(WLANFW_DIR)/src/ipstack/ip \
		-I$(WLANFW_DIR)/src/ipstack/h \
		-I$(WLANFW_DIR)/src/ipstack/tcp \
		-I$(WLANFW_DIR)/src/security/include \
		-I$(WLANFW_DIR)/import/rev7/include/ \
		-I$(WLANFW_DIR)/src/wlan/p2p_fw


PDIR := ../$(PDIR)
sinclude $(PDIR)Makefile

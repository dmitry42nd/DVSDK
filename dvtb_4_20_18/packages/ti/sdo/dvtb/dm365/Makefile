#
# Makefile
#
# Makefile for DM365 Linux DVTB
#
# Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/ 
# 
# 
#  Redistribution and use in source and binary forms, with or without 
#  modification, are permitted provided that the following conditions 
#  are met:
#
#    Redistributions of source code must retain the above copyright 
#    notice, this list of conditions and the following disclaimer.
#
#    Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the 
#    documentation and/or other materials provided with the   
#    distribution.
#
#    Neither the name of Texas Instruments Incorporated nor the names of
#    its contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
#  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
#  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
#  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
#  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#

DVTB_ROOTDIR = ../../../../..
-include ../../../../../../Rules.make
include pal.src
include TSPA.src
include NONTSPA.src
ifndef CODECS
#CODECS=TSPA
CODECS=NONTSPA
endif

TARGET = dvtb
TARGET_DIR = ./bin
REL_LIB = $(TARGET_DIR)/$(TARGET)-r.a
DBG_LIB = $(TARGET_DIR)/$(TARGET)-d.a
EDITLINE_LIB = ./linux/libs/libedit.a
REL_BIN = $(TARGET_DIR)/$(TARGET)-r
DBG_BIN = $(TARGET_DIR)/$(TARGET)-d

FCTOOLS_INSTALL_DIR = $(FC_INSTALL_DIR)/fctools

XDC_PATH = $(CODEC_INSTALL_DIR)/packages;$(FC_INSTALL_DIR)/packages;$(FCTOOLS_INSTALL_DIR)/packages;$(CE_INSTALL_DIR)/packages;$(XDAIS_INSTALL_DIR)/packages;$(CMEM_INSTALL_DIR)/packages;$(CETOOLS_INSTALL_DIR)/packages;$(CE_INSTALL_DIR)/examples

#Configuro related
#Base name of app's config script 
CONFIGPKG = $(TARGET)_$(notdir $(CURDIR))
CONFIG_BLD := config.bld
XDCTARGET = gnu.targets.codesourcery.GCArmv5T
XDCPLATFORM = ti.platforms.evmDM365

#Configuro generated files
XDC_COMPILER_FILE = $(CONFIGPKG)/compiler.opt
XDC_LINKER_FILE = $(CONFIGPKG)/linker.cmd

#Path to Configuro utility
CONFIGURO = XDCPATH="$(XDC_PATH)" $(XDC_INSTALL_DIR)/xs xdc.tools.configuro

DBG_FLAGS = -g -D_DVEVM_ST_DEBUG_
REL_FLAGS =

C_FLAGS += -Wall -DDM365

INCLUDES += -I $(LINUXKERNEL_INSTALL_DIR)/include -I $(XDAIS_INSTALL_DIR)/packages/ti/xdais/include  -I $(DVTB_ROOTDIR)/packages/ti/sdo/dvtb/core/inc/common -I $(DVTB_ROOTDIR)/packages/ti/sdo/dvtb/core/inc/linux -I $(DVTB_ROOTDIR)/packages/ti/sdo/dvtb/pal/linux/v4l2 -I $(CURDIR)/linux -I $(DVTB_ROOTDIR)/packages/ti/sdo/dvtb/pal/linux/alsa  -I $(LINUXLIBS_INSTALL_DIR)/include

#LD_LIBS += -lpthread $(EDITLINE_LIB) -lstdc++ $(LINUXLIBS_INSTALL_DIR)/lib/libasound.so.2 $(LINUXLIBS_INSTALL_DIR)/lib/libncurses.so
LD_LIBS = -lpthread $(EDITLINE_LIB) -lstdc++ -L$(LINUXLIBS_INSTALL_DIR)/lib -Wl,-rpath-link,$(LINUXLIBS_INSTALL_DIR)/lib -lasound -lncursesw 

AR_FLAGS += -r 

ifeq ($(CODECS),TSPA)
VPATH= $(PAL_SRCVPATH) $(TSPA_SRCVPATH)
else
VPATH= $(PAL_SRCVPATH) $(NONTSPA_SRCVPATH)
endif

CC = $(MVTOOL_PREFIX)gcc $(INCLUDES) $(C_FLAGS) $(shell cat $(XDC_COMPILER_FILE)) -c
AR = $(MVTOOL_PREFIX)ar
LN = $(MVTOOL_PREFIX)gcc

ifeq ($(CODECS),TSPA)
REL_FLAGS =
DBG_FLAGS = -g -D_DVEVM_ST_DEBUG_
REL_OBJ = $(PAL_SOURCE:%.c=%.rel.o) $(TSPA_SOURCE:%.c=%.rel.o)
DBG_OBJ = $(PAL_SOURCE:%.c=%.dbg.o) $(TSPA_SOURCE:%.c=%.dbg.o)
else
REL_FLAGS = -D_DVTB_NONTSPA_
DBG_FLAGS = -g -D_DVEVM_ST_DEBUG_ -D_DVTB_NONTSPA_
REL_OBJ = $(PAL_SOURCE:%.c=%.rel.o) $(NONTSPA_SOURCE:%.c=%.rel.o)
DBG_OBJ = $(PAL_SOURCE:%.c=%.dbg.o) $(NONTSPA_SOURCE:%.c=%.dbg.o)
endif


HEADERS = $(wildcard linux/*.h) $(wildcard ../core/inc/linux/*.h) $(wildcard ../core/inc/common/*.h)


.PHONY: clean

.NOTPARALLEL: $(REL_BIN) $(DBG_BIN)

all :: $(REL_BIN) $(DBG_BIN)

#Dependency on Config package
$(XDC_COMPILER_FILE) $(XDC_LINKER_FILE) :: $(CONFIGPKG)

#Config package dependency
$(CONFIGPKG) :: $(CONFIGPKG).cfg
	$(CONFIGURO) -c $(MVTOOL_DIR) -o $(CONFIGPKG) -t $(XDCTARGET) -p $(XDCPLATFORM) -b $(CONFIG_BLD)  $(CONFIGPKG).cfg
	@mkdir -p bin
	
$(REL_BIN) :: $(REL_LIB) $(XDC_LINKER_FILE)
	$(LN) -o $@ $^ $(LD_LIBS)

$(REL_LIB) :: $(REL_OBJ)
	$(AR) $(AR_FLAGS) $(REL_LIB) $(REL_OBJ)

$(REL_OBJ) :: %.rel.o: %.c $(HEADERS) $(XDC_COMPILER_FILE)
	$(CC) $(XDC_INCLUDES) $(REL_FLAGS) -o $@ $<

$(DBG_BIN) :: $(DBG_LIB) $(XDC_LINKER_FILE)
	$(LN) -o $@ $^ $(LD_LIBS)
	
$(DBG_LIB) :: $(DBG_OBJ) 
	$(AR) $(AR_FLAGS) $(DBG_LIB) $(DBG_OBJ)

$(DBG_OBJ) :: %.dbg.o: %.c $(HEADERS) $(XDC_COMPILER_FILE)
	$(CC) $(XDC_INCLUDES) $(DBG_FLAGS) -o $@ $<

clean::
	-rm -rf $(REL_OBJ) 
	-rm -rf $(REL_LIB)
	-rm -rf $(DBG_OBJ)
	-rm -rf $(DBG_LIB)
	-rm -rf $(REL_BIN)
	-rm -rf $(DBG_BIN)
	-rm -rf $(TARGET_DIR)
	-rm -rf $(CONFIGPKG)

#/********************************************************************16**
#
#            (c) Copyright 2012 by RadiSys Corporation. All rights reserved.
#
#     This software is confidential and proprietary to RadiSys Corporation.
#     No part of this software may be reproduced, stored, transmitted, 
#     disclosed or used in any form or by any means other than as expressly
#     provided by the written Software License Agreement between Radisys 
#     and its licensee.
#
#     Radisys warrants that for a period, as provided by the written
#     Software License Agreement between Radisys and its licensee, this
#     software will perform substantially to Radisys specifications as
#     published at the time of shipment, exclusive of any updates or 
#     upgrades, and the media used for delivery of this software will be 
#     free from defects in materials and workmanship.  Radisys also warrants 
#     that has the corporate authority to enter into and perform under the 
#     Software License Agreement and it is the copyright owner of the software 
#     as originally delivered to its licensee.
#
#     RADISYS MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
#     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
#     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
#     MATERIALS.
#
#     IN NO EVENT SHALL RADISYS BE LIABLE FOR ANY INDIRECT, SPECIAL,
#     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
#     OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
#     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
#     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
#     OF THE POSSIBILITY OF SUCH DAMAGE.
#
#                       Restricted Rights Legend:
#
#     This software and all related materials licensed hereby are
#     classified as "restricted computer software" as defined in clause
#     52.227-19 of the Federal Acquisition Regulation ("FAR") and were
#     developed entirely at private expense for nongovernmental purposes,
#     are commercial in nature and have been regularly used for
#     nongovernmental purposes, and, to the extent not published and
#     copyrighted, are trade secrets and confidential and are provided
#     with all rights reserved under the copyright laws of the United
#     States.  The government's rights to the software and related
#     materials are limited and restricted as provided in clause
#     52.227-19 of the FAR.
#
#                    IMPORTANT LIMITATION(S) ON USE
#
#     The use of this software is limited to the use set
#     forth in the written Software License Agreement between Radisys and
#     its Licensee. Among other things, the use of this software
#     may be limited to a particular type of Designated Equipment, as 
#     defined in such Software License Agreement.
#     Before any installation, use or transfer of this software, please
#     consult the written Software License Agreement or contact Radisys at
#     the location set forth below in order to confirm that you are
#     engaging in a permissible use of the software.
#
#                    RadiSys Corporation
#                    Tel: +1 (858) 882 8800
#                    Fax: +1 (858) 777 3388
#                    Email: support@trillium.com
#                    Web: http://www.radisys.com 
# 
#*********************************************************************17*/
# Makefile for building OSAL library for the Linux
#
# Copyright 2008,2009 by Mindspeed Technologies
# All rights reserved
#
include makecfg
CWD:=$(shell pwd)
CP := cp -f
RM := rm -f
MAKE := make

# QCOM compile c code
LNX_QCOM_CC=arm-v7a-linux-gnueabi-gcc
#QCOM flags option
#QCOM compiling libraries with flags
LNX_QCOM_LCOPTS= -Wl,--copy-dt-needed-entries -Wl,-rpath-link,/home/labadmin/BUILD_ROOT/LINUX/project/fsm9900_cdp-fapi/platform-fsm9900_cdp-fapi/sysroot-target/lib
OAMCL_LIBS_DIR=$(shell pwd)/../../../build/libs
#QCOM comman path
QCOM_PATH = $(shell pwd)/../../../../../../LINUX/
QCOM_SYSROOT=$(QCOM_PATH)/project/fsm9900_cdp-fapi/platform-fsm9900_cdp-fapi/sysroot-target
OAMCL_DIR=$(shell pwd)/../../../oamcl/
QCOMOAMCL_IOPTS += -I$(QCOM_SYSROOT)/usr/include/libxml2
QCOMOAMCL_IOPTS += -I$(QCOM_SYSROOT)/include/oam/
QCOMOAMCL_IOPTS += -I$(QCOM_SYSROOT)/usr/include
QCOMOAMCL_IOPTS += -I$(QCOM_SYSROOT)/include
INCL_PATH1 = $(shell pwd)/../../inc  
INCL_PATH2 = $(shell pwd)/../inc
INCL_PATH3 = $(QCOM_PATH)/source/nl/api/include/
INCL_PATH4 = $(QCOM_PATH)/source/linux-infra/privs/include/
INCL_PATH5 = $(QCOM_PATH)/source/linux-infra/platform/arm/include/
INCL_PATH6 = $(QCOM_PATH)/source/qmi-framework/inc/
INCL_PATH = $(INCL_PATH1) $(INCL_PATH2) $(INCL_PATH3) $(INCL_PATH4) $(INCL_PATH5) $(INCL_PATH6) $(OAMCL_DIR) $(QCOMOAMCL_IOPTS)

TARGET = libremcl.a

#
# Source files
#
CPPSRC = nmmcl_lte.cpp  nl_rem_decodeltemsgs.cpp nmmcl_interface.cpp
CSRC = nl_rem_interface.c
ASRC =

#
# Object files
#
OBJS = $(CPPSRC:.cpp=.o) $(CSRC:.c=.o) $(ASRC:.s=.o)

#
# Include files
#
INC = -I$(INCL_PATH1) -I$(INCL_PATH2) -I$(INCL_PATH3) -I$(INCL_PATH4) -I$(INCL_PATH5) -I$(INCL_PATH6) -I$(OAMCL_DIR) $(QCOMOAMCL_IOPTS)
#
# Default target
#
CFLAGS = -g
CPPFLAGS = -g -DDEBUG_PRINT -DLTE_QCOM -ULTE_TDD 
all:    $(TARGET)

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $(TARGET) $(OBJS)

vpath %.cpp %.c %.h %.hpp $(INCLUDE_PATHS) $(INC)

#
# Default rule for building object files from source code
#
%.o: %.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) $(INC) -g -c $< -o $@

%.o: %.c
	$(LNX_QCOM_CC) $(LNX_QCOM_LCOPTS) -g $(CFLAGS) $(CPPFLAGS) $(INC) -c $< -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) $(INC) -c $< -o $@

.c.o:
	$(LNX_QCOM_CC) $(LNX_QCOM_LCOPTS)  $(CFLAGS) $(CPPFLAGS) $(INC) -c $< -o $@

.s.o:
	$(AS) $(ASFLAGS) $(INC) $< -o $@

.PHONY : clean
clean:
	-$(RM) $(OBJS)
	-$(RM) $(TARGET)

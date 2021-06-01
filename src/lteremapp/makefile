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

###############################################################################
#
# Makefile Options:
#
#  make [clean] PRODUCT=<product> [BUILD=<build>]
#
# Copyright radisys Limited
#
###############################################################################

CWD:= $(shell pwd)
TOP = $(shell pwd)/../tenb_commonplatform/software
PATH_REMCL = $(shell pwd)/remcl/src
PATH_LIBSSI = $(shell pwd)/libs/src/
BIN_PATH = $(shell pwd)
REM_BIN_PATH=./bin
SRC_DIRS =src
CP := cp -f
QCOM_PATH=$(shell pwd)/../../../../LINUX/
QCOM_SYSROOT=$(shell pwd)/../../../../LINUX/project/fsm9900_cdp-fapi/platform-fsm9900_cdp-fapi/sysroot-target/
INCL_PATH1 =  $(QCOM_PATH)/source/nl/api/include
INCL_PATH2 =  $(QCOM_PATH)/source/linux-infra/privs/include/
INCL_PATH3 =  $(QCOM_PATH)/source/linux-infra/platform/arm/include/
INCL_PATH4 =  $(QCOM_PATH)/source/qmi-framework/inc/
INCL_PATH = $(INCL_PATH1) $(INCL_PATH2) $(INCL_PATH3) $(INCL_PATH4)
#QCOM including threads library
LNX_QCOM_LOPTS=-Wl,-rpath-link,$(QCOM_PATH)/project/fsm9900_cdp-fapi/platform-fsm9900_cdp-fapi/sysroot-target/lib -lpthread -lqmi_csi -lqmi_cci -lqmi_encdec -lqmi_common -lstringl -lQctPrivs -lnlapiqmi -loamcl
#Libraries to be linked
LIB_PATH1 = -L $(QCOM_PATH)/project/fsm9900_cdp-fapi/platform-fsm9900_cdp-fapi/root/opt/qcom/lib/
LIB_PATH2 = -L $(QCOM_PATH)/project/fsm9900_cdp-fapi/platform-fsm9900_cdp-fapi/build-target/nl-1.0.0/lib
LIB_PATH2 = -L ../build/obj
QCOMLOPTS=-L$(QCOM_SYSROOT)/lib -lFsmOamClientProtocol -lFsmOamMgmtXml -lqmi_csi -lqmi_cci -lqmi_encdec -lqmi_common -lQctInfra -lQctInfraC -lQctOsal -lsharedqmi -lrt
QCOMCMNLOPTS=-L$(QCOM_SYSROOT)/usr/lib -lxml2 -lboost_program_options-mt -lboost_thread-mt -lboost_chrono-mt -lboost_regex-mt -lboost_system-mt -lz -lstringl -lfidwd -lboost_date_time-mt -lrt

LIB_PATH = $(LIB_PATH1) $(LIB_PATH2) $(LNX_QCOM_LOPTS)
#STATICALLY_LINKED=yes

include $(TOP)/makefrags/products.mk

newall: libasn librcl build

TARGET_NAME=rem

LIBREMCL= libremcl

###compiling and copying the REMCL 
librcl:
	cd $(PATH_REMCL); $(MAKE) -f Makefile
	mkdir -p libs
	mkdir -p libs/$(BUILD)
	$(CP) $(PATH_REMCL)/libremcl.a libs/$(BUILD)/

libasn:
	rm -rf libs/src/libssi.a libs/$(BUILD)/libssi.a
	cd $(PATH_LIBSSI); $(MAKE) -f makefile
	$(CP) $(PATH_LIBSSI)/libssi.a libs/$(BUILD)/

ifneq ($(STATICALLY_LINKED),yes)
LIB_DEPS = \
	$(TOP)/libs/messaging/obj/$(PRODUCT)/$(HARDWARE)/$(BUILD)/libthreeway-messaging \
	$(TOP)/libs/common/mib-common/obj/$(PRODUCT)/$(HARDWARE)/$(BUILD)/libRadisys-mib-common \
	$(TOP)/libs/common/mobnet/obj/$(HARDWARE)/$(BUILD)/libthreeway-mobnet \
	$(TOP)/libs/common/system/obj/$(HARDWARE)/$(BUILD)/libthreeway-system \
	$(TOP)/libs/common/comms/obj/$(HARDWARE)/$(BUILD)/libthreeway-comms \
	$(TOP)/libs/mib-core/obj/$(PRODUCT)/$(HARDWARE)/$(BUILD)/libRadisys-mib-core 
endif

LDFLAGS=-lssi -lrt -lremcl $(LIB_PATH) $(QCOMLOPTS) $(QCOMCMNLOPTS) $(LNX_QCOM_LCOPTS)  -Wl,-R/root/OAM/lib/cpp/:/root/OAM/lib/oam/ 
LIB_PATHS = -L$(CWD)/libs/$(BUILD) 
SYSTEM_LIBS = pthread 

LIB_DEPS_PREBUILT = libs/$(BUILD)/$(LIBREMCL) libs/$(BUILD)/libssi

ifneq ($(findstring bfin-uclinux-2008,$(BUILD)),bfin-uclinux-2008)
  SYSTEM_LIBS+=dl
endif

# Default SVN_VERSION if not supplied.
ifeq ($(SVN_VERSION),)
  SVN_VERSION:=unknown
endif

INCLUDE_PATHS = \
  $(CWD) \
  $(CWD)/inc \
  $(CWD)/remcl/inc \
  $(CWD)/../cm \
  $(CWD)/../mt \
  $(TOP)/libs/messaging \
  $(TOP)/libs/messaging/messages/rem \
  $(TOP)/libs/messaging/transport \
  $(TOP)/libs/common/mib-common/obj/$(PRODUCT)/generic/generic \
  $(TOP)/libs/common/system \
  $(TOP)/libs/common/mib-common/messages \
  $(TOP)/libs/mib-core \
  $(INCL_PATH)

vpath %.cpp $(SRC_DIRS) 
vpath %.h $(INCLUDE_PATHS) 

ifeq ($(STATICALLY_LINKED),yes)
CPPFLAGS = -Dlinux -DSVN_VERSION=\"$(SVN_VERSION)\" -g -DSUNOS -DANSI -D_GNU_SOURCE -DSS_LINUX -DSS -DLTE_QCOM -DSS_MT -DCMFILE_REORG_1  -Dx__ARMCC_VERSION -ULTE_TDD -UUSE_RADISYS_OAM -UDEBUG_PRINT -L/usr/lib/i386-linux-gnu/ -Wl,-R/root/OAM/lib/cpp/:/root/OAM/lib/oam/
include $(TOP)/makefrags/staticlibrary.mk
else
CPPFLAGS = -Dlinux -DSVN_VERSION=\"$(SVN_VERSION)\" -g -DSUNOS -DANSI -D_GNU_SOURCE -DSS_LINUX -DSS -DLTE_QCOM -DSS_MT -DCMFILE_REORG_1 -Dx__ARMCC_VERSION -DLTE_REM -ULTE_TDD -DUSE_RADISYS_OAM -DDEBUG_PRINT -L/usr/lib/i386-linux-gnu/ -Wl,-R/root/OAM/lib/cpp/:/root/OAM/lib/oam/
include $(TOP)/makefrags/application.mk
endif

clean:
	make -C remcl/src/ -f Makefile clean; rm -rf src/*.o libs/src/libssi.a libs/$(BUILD)

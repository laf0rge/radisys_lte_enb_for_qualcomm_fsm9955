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
# All rights reserved
#
include makecfg
CWD:=$(shell pwd)
PATH_REMCL=$(CWD)/../remcl/src
PATH_LIBSSI=$(CWD)/../libs/src
CP := cp -f
RM := rm -f
MAKE := make
OAMCL_DIR= $(shell pwd)/../../oamcl/
INCL_PATH1 = $(shell pwd)/../inc/ 
INCL_PATH2 = $(shell pwd)/../remcl/inc/ 
INCL_PATH3 = $(shell pwd)/../../cm/ 
INCL_PATH4 = $(shell pwd)/../../mt/ 
QCOM_PATH=$(shell pwd)/../../../../../LINUX/
INCL_PATH5 =  $(QCOM_PATH)/source/nl/api/include
INCL_PATH6 =  $(QCOM_PATH)/source/linux-infra/privs/include/
INCL_PATH7 =  $(QCOM_PATH)/source/linux-infra/platform/arm/include/
INCL_PATH8 =  $(QCOM_PATH)/source/qmi-framework/inc/
INCL_PATH = $(INCL_PATH1) $(INCL_PATH2) $(INCL_PATH3) $(INCL_PATH4) \
   $(INCL_PATH5) $(INCL_PATH6) $(INCL_PATH7) $(INCL_PATH8) $(OAMCL_DIR)

LNX_QCOM_LOPTS=-Wl,-rpath-link,$(QCOM_PATH)/project/fsm9900_cdp-fapi/platform-fsm9900_cdp-fapi/sysroot-target/lib -lpthread -lqmi_csi -lqmi_cci -lqmi_encdec -lqmi_common -lstringl -lQctPrivs -lnlapiqmi 

LIB_PATH1 = -L $(QCOM_PATH)/project/fsm9900_cdp-fapi/platform-fsm9900_cdp-fapi/root/opt/qcom/lib/
LIB_PATH2 = -L $(QCOM_PATH)/project/fsm9900_cdp-fapi/platform-fsm9900_cdp-fapi/build-target/nl-1.0.0/lib
TARGET=librem.a

#newall: librcl $(TARGET)
newall: libasn librcl $(TARGET)

###compiling and copying the REMCL 
librcl:
	cd $(PATH_REMCL); $(MAKE) -f Makefile
	$(CP) $(PATH_REMCL)/libremcl.a ./../libs/$(BUILD)/

libasn:
	rm -rf libs/src/libssi.a libs/$(BUILD)/libssi.a
	cd $(PATH_LIBSSI); $(MAKE) -f makefile
	$(CP) $(PATH_LIBSSI)/libssi.a ./../libs/$(BUILD)

all:  $(TARGET)

LDFLAGS=-lssi -lrt -lremcl
LIB_PATHS = -L$(CWD)/../libs/$(BUILD)  $(LIB_PATH1) $(LIB_PATH2) $(LNX_QCOM_LOPTS)
SYSTEM_LIBS = pthread
#
# Source files
#
CPPSRC = rem_eventhandler.cpp rem_interface.cpp rem_lte_rrcmsg_decoder.cpp rem_wcdma_event_handler.cpp rem_application.cpp rem_fsm.cpp rem_lte_event_handler.cpp rem_lteconfig.cpp rem_wcdmaconfig.cpp rem_3g_rrcmsg_decoder.cpp rem_multiInstanceManager.cpp Pasn_BchDecodeParams.cpp rem_rat_config.cpp rem_lte_freq_sync_event_handler.cpp

ASRC =

#
# Object files
#
OBJS = $(CPPSRC:.cpp=.o) $(ASRC:.s=.o)

#
# Include files
#
INC = -I$(INCL_PATH2) -I$(INCL_PATH1) -I$(INCL_PATH3) -I$(INCL_PATH4) \
   -I$(INCL_PATH5) -I$(INCL_PATH6) -I$(INCL_PATH7) -I$(INCL_PATH8)
#
# Default target
#

CPPFLAGS = -Dlinux -DSVN_VERSION=\"$(SVN_VERSION)\" -g -DSUNOS -DANSI -D_GNU_SOURCE -DSS_LINUX -DSS -DSS_MT -DCMFILE_REORG_1 -Dx__ARMCC_VERSION -DLTE_REM -UUSE_RADISYS_OAM -DDEBUG_PRINT -DLTE_QCOM -L/usr/lib/i386-linux-gnu/ 

$(TARGET): $(OBJS) libasn librcl
	$(AR) $(ARFLAGS) $(TARGET) $(OBJS) $(PATH_REMCL)/libremcl.a  ./../libs/$(BUILD)/libssi.a

#	$(CC) $(LIB_PATHS) $(LDFLAGS) $(OBJS) -o $@

vpath %.cpp %.h $(INCL_PATH)

#
# Default rule for building object files from source code
#
buildrem: $(OBJS)
		$(CPP) $(CFLAGS) $(CPPFLAGS) $(INC) -c $@

.cpp.o:
	@echo "$(CC) $(CFLAGS) $(CPPFLAGS) $(INC) -c $< -o $@"
	$(CPP) $(CFLAGS) $(CPPFLAGS) $(INC) -c $< -o $@


.s.o:
	$(AS) $(ASFLAGS) $(INC) $< -o $@

#
#	.cpp.o:
#		$(CC) $(CFLAGS) $(CPPFLAGS) $(INC) -c $< -o $@
#
#.s.o:
#	$(AS) $(ASFLAGS) $(INC) $< -o $@

.PHONY : clean
clean:
	-$(RM) $(OBJS)
	-$(RM) $(TARGET)
	-$(RM) $(PATH_REMCL)/*.o
	-$(RM) $(PATH_REMCL)/libremcl.a
	-$(RM) ./../libs/$(BUILD)/libssi.a

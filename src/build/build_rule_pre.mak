#********************************************************************16**
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
#    52.227-19 of the Federal Acquisition Regulation ("FAR") and were
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
#*********************************************************************17*
#
#********************************************************************20**
#
#       Name:   BUILD_RULE - Build rule for all c and cpp files
#
#       Type:   Makefile
#
#       Desc:   Build the C and CPP files and create corresponding .o files 
#
#               For  make:
#
#               1) ensure ALL Header File dependency is mentione in HEADER_FILES
#
#       File:   build_rule.mak
#
#       Sid:      build_rule.mak@@/main/TeNB_QCOM_REL_BR/1 - Fri Mar 28 15:37:42 2014
#
#       Prg:    amith
#
#  
#********************************************************************21*/

OUT_DIR_NAME=$(subst /,,$(subst .,,$(OUT_DIR)))
#-----------------------------------------------------------------------
# DO NOT EDIT THIS SECTION
# Header files on which this layer is dependant 
HEADER_FILES= \
    $(CM_DIR)/*.h \
    $(CM_DIR)/*.x \
    $(IN_DIR)/*.h \
    $(IN_DIR)/*.x

#-----------------------------------------------------------------------
# FANCY STUFF... you can play around with this if you want to
COLOR=\e[34m\e[1m
REVERT=\e[m

#-----------------------------------------------------------------------
# DO NOT EDIT THIS SECTION
# Source files on which this layer is dependant 
C_SRC=$(patsubst %.i,%.c, $(subst $(OUT_DIR_NAME),$(IN_DIR),$(C_OBJS)))
CPP_SRC=$(patsubst %.i,%.cpp, $(subst $(OUT_DIR_NAME),$(IN_DIR),$(CPP_OBJS)))

all: $(C_OBJS) $(CPP_OBJS)  

$(C_OBJS): $(HEADER_FILES) $(C_SRC) 
	@echo -e "Compiling $(COLOR)$(IN_DIR)/$(patsubst %.i,%.c, $(subst $(OUT_DIR_NAME)/,,$@)) $(REVERT)..."
	$(QUIET)$(CC) -E $(CCOPTS) -o $@ $(IN_DIR)/$(patsubst %.i,%.c, $(subst $(OUT_DIR_NAME),,$@))

$(CPP_OBJS): $(HEADER_FILES) $(CPP_SRC)
	@echo -e "Compiling $(COLOR)$(IN_DIR)/$(patsubst %.i,%.cpp, $(subst $(OUT_DIR_NAME)/,,$@)) $(REVERT)..."
	$(QUIET)$(CPP) -E $(CPPOPTS) -o $@ $(IN_DIR)/$(patsubst %.i,%.cpp, $(subst $(OUT_DIR_NAME),,$@))

clean:
	@echo -e "$(COLOR)Cleaning files built from $(IN_DIR)... $(REVERT)"
	$(QUIET)/bin/rm -rf $(C_OBJS) $(CPP_OBJS)

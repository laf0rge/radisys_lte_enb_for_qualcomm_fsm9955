#********************************************************************16**
#
#                         (c) COPYRIGHT 2012 by
#                         Continuous Computing Corporation.
#                         All rights reserved.
#
#     This software is confidential and proprietary to Continuous Computing
#     Corporation (CCPU).  No part of this software may be reproduced,
#     stored, transmitted, disclosed or used in any form or by any means
#     other than as expressly provided by the written Software License
#     Agreement between CCPU and its licensee.
#
#     CCPU warrants that for a period, as provided by the written
#     Software License Agreement between CCPU and its licensee, this
#     software will perform substantially to CCPU specifications as
#     published at the time of shipment, exclusive of any updates or
#     upgrades, and the media used for delivery of this software will be
#     free from defects in materials and workmanship.  CCPU also warrants
#     that has the corporate authority to enter into and perform under the
#     Software License Agreement and it is the copyright owner of the software
#     as originally delivered to its licensee.
#
#     CCPU MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
#     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
#     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
#     MATERIALS.
#
#     IN NO EVENT SHALL CCPU BE LIABLE FOR ANY INDIRECT, SPECIAL,
#     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
#     OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
#     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
#     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
#     OF THE POSSIBILITY OF SUCH DAMAGE.
#
#                       Restricted Rights Legend
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
#     forth in the written Software License Agreement between CCPU and
#     its Licensee. Among other things, the use of this software
#     may be limited to a particular type of Designated Equipment, as
#     defined in such Software License Agreement.
#     Before any installation, use or transfer of this software, please
#     consult the written Software License Agreement or contact CCPU at
#     the location set forth below in order to confirm that you are
#     engaging in a permissible use of the software.
#
#                    Continuous Computing Corporation
#                    9380, Carroll Park Drive
#                    San Diego, CA-92121, USA
#
#                    Tel: +1 (858) 882 8800
#                    Fax: +1 (858) 777 3388
#
#                    Email: support@trillium.com
#                    Web: http://www.ccpu.com
#
#*********************************************************************17*
#
#********************************************************************20**
#
#       Name:   LTE Qcom L2 Makefile
#
#       Type:   make file
#
#       Desc:   Compile L2 software for Hexagon.
#               embedded in the file.
#
#       Env:    Hexagon 
#
#               BUILD TARGETS: HEXAGON V4
#
#               clean   remove all object files
#
#       File:  qcom_l2.mak
#
#
#       Prg:   amith
#
#********************************************************************21*/
#=======================================================================

#-----------------------------------------------------------------------
# macro for output file name and makefile name
#
export OUT_DIR=./obj_l2
export BUILD_ROOT=.

#-----------------------------------------------------------------------
# macros for sw and include directories
#
CODEPATH=..
QU_DIR=$(CODEPATH)/qu
export CM_DIR=$(CODEPATH)/cm
VL_DIR=$(CODEPATH)/lteclqc
RG_DIR=$(CODEPATH)/ltemac
KW_DIR=$(CODEPATH)/lterlc
PJ_DIR=$(CODEPATH)/ltepdcp
RL_DIR=$(CODEPATH)/rlog

#-----------------------------------------------------------------------
# Have a variable to include all the directories where Qcom file dependencies
# are present
#
QCOM_CODE_PATH=../../../../fsm99xx_ltel2
QCOM_DIR=$(QCOM_CODE_PATH)/qurt/QURT.02.02.19.01/install/fsm99xx-v5/include/qurt
QCOM_DIR_QUBE=$(QCOM_CODE_PATH)/qurt/QURT.02.02.19.01/install/fsm99xx-v5/include/qube
QCOM_PLATFORM_DIR=$(QCOM_CODE_PATH)/platform/drivers/ethDrv/i82574/include/
QCOM_COMMON_DIR=$(QCOM_CODE_PATH)/app/common/include/
QCOM_INFRA_DIR=$(QCOM_CODE_PATH)/infra/include/
QCOM_QURT_DIR=$(QCOM_CODE_PATH)/osal/qurt/include/
QCOM_GEN_DIR=$(QCOM_CODE_PATH)/_hexBuild/_fsm99xxEP/_genInclude/
QCOM_GEN_HEX2_DIR=$(QCOM_CODE_PATH)/../LINUX/project/fsm9900_cdp-fapi/platform-fsm9900_cdp-fapi/build-target/lte-l2-1.0.0/_hexBuild/_fsm99xxCDP_1/_genInclude
QCOM_OAMRECORD_DIR=$(QCOM_CODE_PATH)/app/common/oamRecord/include/
QCOM_FSM_DIR=$(QCOM_CODE_PATH)/targets/fsm99xxEP/include/
QCOM_OAM_DIR=$(QCOM_CODE_PATH)/app/common/oam/include/
QCOM_OAMAPI_DIR=$(QCOM_CODE_PATH)/messages/include/
QCOM_WALLCLOCKDRV_DIR=$(QCOM_CODE_PATH)/platform/drivers/wallClockDrv/include/
QCOM_PLATFORM_CMN_DIR=$(QCOM_CODE_PATH)/platform/drivers/common/include/
QCOM_MACSIM_PATH=$(QCOM_CODE_PATH)/app/modules/test/macsimMod/
QCOM_SEC_PATH1=$(QCOM_CODE_PATH)/_hexBuild/_fsm99xxCDP_1/_genInclude/
QCOM_SEC_PATH2=$(QCOM_CODE_PATH)/targets/fsm99xxCDP_1/include
QCOM_SEC_PATH3=$(QCOM_CODE_PATH)/platform/drivers/cryptoDrv/common/include
QCOM_IPCFIFO_INC=$(QCOM_CODE_PATH)/platform/drivers/ipcFifoDrv/include/
QCOM_IPC_INC=$(QCOM_CODE_PATH)/platform/drivers/ipc/include/
QCOM_HEX2_PATH=$(QCOM_CODE_PATH)/platform/hardware/fsm99xxCDP/hexagon2/include
QCOM_PLAT_CMN_PATH=$(QCOM_CODE_PATH)/platform/hardware/fsm99xxCDP/common/include
QCOM_MACUTL_PATH=$(QCOM_CODE_PATH)/platform/drivers/macUtl/include
QCOM_INC=-I$(QCOM_DIR) \
         -I$(QCOM_DIR_QUBE) \
         -I$(QCOM_PLATFORM_DIR) \
         -I$(QCOM_COMMON_DIR) \
         -I$(QCOM_INFRA_DIR) \
         -I$(QCOM_QURT_DIR) \
         -I$(QCOM_WALLCLOCKDRV_DIR) \
         -I$(QCOM_GEN_DIR) \
         -I$(QCOM_GEN_HEX2_DIR) \
         -I$(QCOM_OAMRECORD_DIR) \
         -I$(QCOM_FSM_DIR) \
         -I$(QCOM_OAM_DIR) \
         -I$(QCOM_OAMAPI_DIR) \
         -I$(QCOM_PLATFORM_CMN_DIR)\
         -I$(QCOM_SEC_PATH1)\
         -I$(QCOM_SEC_PATH2)\
         -I$(QCOM_SEC_PATH3)\
         -I$(QCOM_IPC_INC)\
         -I$(QCOM_IPCFIFO_INC)\
         -I$(QCOM_MACSIM_PATH)\
         -I$(QCOM_PLAT_CMN_PATH)\
         -I$(QCOM_MACUTL_PATH)\
         -I$(QCOM_HEX2_PATH)

#-----------------------------------------------------------------------
# macros for td specific options
#
# The options below are used to pass on to the actual product compilations
# Just this one variable is passed for individual compilations and hence
# all the needed options are added from various other defined option macros

#-----------------------------------------------------------------------
# OS specific compilation flags
ALL_L2_OPTS = -DSUNOS \
               -DANSI \
               -DSS_LINUX \
               -D_REENTRANT \
               -D__EXTENSIONS__ \
               -DDEBUGNOEXIT \
               -DDEBUG_LEWRL=$(DEBUG)

TENB_AS_SECURITY = -DTENB_AS_SECURITY

#System Specific 
TENB_MAX_UE_SUPPORTED= -DTENB_MAX_UE_SUPPORTED=32

# MTSS specific defines
ALL_L2_OPTS += -DSS \
               -DSS_MT \
               -DSS_MT_TMR \
               -UCONRD \
               -UCONAVL \
               -DSS_TICKS_SEC=1000 \
               -DSS_M_PROTO_REGION \
               -DSS_FLOAT \
               -DSSINT2 \
               -DSS_PERF \
               -DSS_WL_REGION=1 \
               -USS_MULTICORE_SUPPORT \
               -DUSE_LINUX \
               -ULONG_MSG \
               -USS_DRVR_SUPPORT \
               -DNOFILESYS \
               -UNOFILESYS \
               -USS_DATETIME_USEC \
               -DSS_LICENSE_CHECK \
               -USS_DEBG_MEM_CORRUP_LEVEL1 \
               -UQCOM_TOBE_REMOVED

# L2 End to End Common Compilation flags
ALL_L2_OPTS += -DNO_ERRCLS \
               -DNOERRCHK \
               -DDEBUGP \
               -DLTE_HO_SUPPORT \
               -DSI_NEW \
               -DCL_MAC_LWLC \
               -DTFU_UPGRADE \
               -DRGR_CQI_REPT \
               -DTENB_AS_SECURITY \
               -UMAC_SCH_STATS \
               -DLTE_L2_MEAS \
               -DLA_VER2 \
               -ULA_VER3 \
               -DUL_LA \
               -DDL_LA \
               -DTENB_RTLIN_CHANGES \
               -DTTI_PROC_FIRST \
               -DLTEMAC_DLUE_TMGOPTMZ \
               -DLTEMAC_RGU_PAD \
               -DTENB_SPLIT_ARCH \
               -DDLHQ_RTT_OPT \
               -ULTE_START_UL_DL_DATA \
               -DSLES9_PLUS \
               -DLTERRC_REL9 \
               -DLTE_QCOM \
               -DQCOM_DBG \
               -USCH_STATS \
               -DQCOM_STATS \
					-DTENB_STATS \
 					-UENB_CPU_OVERLOAD_CONTROL\
					-ULTEMAC_SPS\
					-UTENB_UL_FP \
					-UPDCP_RLC_DL_RBUF \
	            -UDAM_PDCP_OPT \
					-UMAC_RLC_UL_RBUF \
					-URLC_NO_HDR_CPY \
					-DKWSELFPSTDLCLEAN \
					$(TENB_MAX_UE_SUPPORTED)

# cm specific compilation flags
ALL_L2_OPTS += -DCMFILE_REORG_1 \
               -DCMFILE_REORG_2 \
               -DCMKV2  \
               -DCTF_VER3 \
               -DREVIEW \
               -DCPJ_V2 \
               -USM \
               -DAPI_MAIN

# All LTE L2 interface options
ALL_L2_OPTS += -DLCNHLICRG \
               -DKW \
               -UNH \
               -DPJ \
               -DYS \
               -DLCPJMILPJ \
               -DLCLPJ \
               -DLCCRG \
               -DLCRG \
               -DLWLCRGLITFU \
               -ULCRGLITFU \
               -DLCRGUICRG \
               -DTFU_WRR_2 \
               -DTFU_TDD \
               -DLTE_TDD \
               -DLCTFU \
               -DLCYSUITFU \
               -DTFU_VER_2 \
               -DLCKWLIRGU \
               -DLCRGU \
               -DLCRGUIRGU \
               -DCCPU_OPT \
               -DLWLCKWLIRGU \
               -DLWLCRGUIRGU \
               -DLCSMMILRG \
               -DLCLRG \
               -DLCRGMILRG \
               -DRM_INTF \
               -DRGM_LWLC \
               -DRGM_LC \
               -DLCWRLIRGR \
               -DLWLCWRLIRGR \
               -DLCRGUIRGR \
               -DLCKWUIKWU \
               -DLWLCKWUIKWU \
               -DLCKWU \
               -DLCNHLIKWU \
               -DLCKWMILKW \
               -DLCLKW \
               -DLCPJU \
               -DLCWRLIPJU \
               -DLCPJUIPJU \
               -DLCNHLIPJU \
               -DLCCPJ \
               -DLCNHLICPJ \
               -DLCPJUICPJ \
               -DLCKWUICKW \
               -DLCYSUICTF \
               -DLCYSMILYS \
               -DLCLYS \
               -DLWLCYSUITFU \
               -DQCOM\

#ALL RRC/Application interface options
ALL_L2_OPTS += -DLCCTF \
               -DWR \
               -DLCWRLICTF \
               -DLCTFUITFU \
               -DTF \
               -DLCSMMILCL \
               -DLCTFUICTF
#aghosh changed LCSCH 
# MAC layer specific compilation flags
ALL_L2_OPTS += -URG_DEBUG \
               -UTTI_PROC \
               -UPROC_DL \
               -USCH_TTI_PROC \
               -UTOM \
               -DRGR_RRM_TICK \
               -DRGR_SI_SCH \
               -DLTEMAC_MIMO \
               -DRGR_V1 \
               -DRGR_V2 \
               -UPAL_MAC_PROC \
               -UUL_PROC_MAC \
               -DRG_DL_DELTA_CHANGE \
               -DRG_CMNCH_SCHDELTA_CHANGE \
               -URG_ULSCHED_AT_CRC \
               -DTFU_RECPREQ_DELTA_CHANGE \
               -DTFU_DL_DELTA_CHANGE \
               -DTFU_UL_DELTA_CHANGE \
               -DTFU_DLHQFBK_DELTA_CHANGE \
               -UBUG_FIX \
               -DLCSCH  \
               -DRG_PHASE2_SCHED \
               -DRG \
               -DRGR_RRM_TICK \
               -DLCRGR \
               -DRM_INTF
          
# PDCP layer specific compilation flags
ALL_L2_OPTS += -DPTPJLIB 

#All Convergence Layer options
ALL_L2_OPTS += -DLCLCL\
               -DCL \
               -DLCCLMILCL \
               -DLOWLATENCY #L2Split

# RLC layer specific compilation flags
ALL_L2_OPTS += -DLCNHLICKW \
               -DLCCKW \
               -URLC_STATUS_GEN \
               -UKW_PDCP \
               -DRG

# RLOG layer specific compilation flags
# Disabling the RLOG in L2
RLOG_CMN = -DTENB_TTI_PERF -DQCOM_RLOG
RLOG_OPTS_BIN = $(RLOG_CMN) -URLOG_ENABLE_TEXT_LOGGING -DRLOG_DEBUG_MODE -DRLOG_USE_CIRCULAR_BUFFER -DRLOG_USE_TTI_LOGGING  
RLOG_OPTS_TEXT = $(RLOG_CMN) -DRLOG_ENABLE_TEXT_LOGGING -DRLOG_DEBUG_MODE 

ALL_L2_OPTS += -UWR_DBG_CIRLOG $(RLOG_OPTS_BIN) -DLTE_QCOM_L2_RLOG

# compiler:
export CC=hexagon-gcc
export LD=hexagon-gcc
export CPP=hexagon-g++

COPTS= -O3 -g -mv5 -fsigned-char -pipe -fno-strict-aliasing -fno-short-enums

# object files:
# o     UNIX
OBJ=o                                    # object files, sunos
OBJ_I=i                                    # object files, sunos

#######################################################################
# library path

# include options:
CMIOPTS  =  -I$(QU_DIR) \
            -I$(CM_DIR) \
            -I$(RL_DIR) \
            -I. \
            $(QCOM_INC)

ifeq ($(VERBOSE),YES)
export QUIET=
MK_QUIET=
else
export QUIET=@
MK_QUIET=-s # Make works in quite mode
endif

ifeq ($(BLD_ENV),PRE)
BLD_ENV_HEX=HEX
OBJ1=i
else
BLD_ENV_HEX=
OBJ1=o
endif

#-----------------------------------------------------------------------
# FANCY STUFF... you can play around with this if you want to
COLOR=\e[31m\e[1m
REVERT=\e[m
#
#
# Linux Default GNU acceptance test build
#
all: SETUP_DEP L2_DEPS

L2_DEPS: PJ_OBJS \
         KW_OBJS \
         MT_OBJS \
         CM_OBJS \
         RG_OBJS \
         CL_OBJS \
			RL_OBJS

SETUP_DEP:
	@mkdir -p $(OUT_DIR)

CL_OBJS: 
	@echo -e "$(COLOR)*******************************************************"
	@echo -e "********** Building Convergence Layer ******************"
	@echo -e "*******************************************************$(REVERT)"
	@$(MAKE) $(MK_QUIET) -f vl.mak  \
                COPTS='$(COPTS)' \
                POPTS='$(ALL_L2_OPTS)' \
                IOPTS='$(CMIOPTS)'  \
                OBJ='$(OBJ1)' \
                IN_DIR='$(VL_DIR)' \
					 BLD_ENV='$(BLD_ENV)' 

CM_OBJS: 
	@echo -e "$(COLOR)*******************************************************"
	@echo -e "************ Building Common Files ********************"
	@echo -e "*******************************************************$(REVERT)"
	@$(MAKE) $(MK_QUIET) -f cm_l2.mak  \
                COPTS='$(COPTS)' \
                POPTS='$(ALL_L2_OPTS)' \
                IOPTS='$(CMIOPTS)'  \
                OBJ='$(OBJ)' \
                IN_DIR='$(CM_DIR)' \
					 BLD_ENV='$(BLD_ENV_HEX)'


MT_OBJS: 
	@echo -e "$(COLOR)*******************************************************"
	@echo -e "**************** Building SSI *************************"
	@echo -e "*******************************************************$(REVERT)"
	@$(MAKE) $(MK_QUIET) -f qu.mak \
                COPTS='$(COPTS)' \
                POPTS='$(ALL_L2_OPTS)' \
                IOPTS='$(CMIOPTS)' \
                OBJ='$(OBJ)' \
                IN_DIR='$(QU_DIR)' \
					 BLD_ENV='$(BLD_ENV_HEX)'

RG_OBJS: 
	@echo -e "$(COLOR)*******************************************************"
	@echo -e "**************** Building MAC *************************"
	@echo -e "*******************************************************$(REVERT)"
	@$(MAKE) $(MK_QUIET) -f rg.mak \
		          COPTS='$(COPTS)' \
					 POPTS='$(ALL_L2_OPTS)' \
					 IOPTS='$(CMIOPTS)' \
					 OBJ='$(OBJ1)' \
					 IN_DIR='$(RG_DIR)' \
					 BLD_ENV='$(BLD_ENV)'

KW_OBJS: 
	@echo -e "$(COLOR)*******************************************************"
	@echo -e "**************** Building RLC *************************"
	@echo -e "*******************************************************$(REVERT)"
	@$(MAKE) $(MK_QUIET) -f kw_split.mak \
		          COPTS='$(COPTS)' \
					 POPTS='$(ALL_L2_OPTS)' \
					 IOPTS='$(CMIOPTS)' \
					 OBJ='$(OBJ1)' \
					 IN_DIR='$(KW_DIR)' \
					 BLD_ENV='$(BLD_ENV)'

PJ_OBJS: 
	@echo -e "$(COLOR)*******************************************************"
	@echo -e "**************** Building PDCP *************************"
	@echo -e "*******************************************************$(REVERT)"
	@$(MAKE) $(MK_QUIET) -f pj.mak \
		          COPTS='$(COPTS)' \
					 POPTS='$(ALL_L2_OPTS)' \
					 IOPTS='$(CMIOPTS)' \
					 OBJ='$(OBJ1)' \
					 IN_DIR='$(PJ_DIR)' \
					 SEC_FLAG='$(TENB_AS_SECURITY)' \
					 BLD_ENV='$(BLD_ENV)'


RL_OBJS: 
	@echo -e "$(COLOR)*******************************************************"
	@echo -e "**************** Building RLOG *************************"
	@echo -e "*******************************************************$(REVERT)"
	@$(MAKE) $(MK_QUIET) -f rl_l2.mak \
		          COPTS='$(COPTS)' \
					 POPTS='$(ALL_L2_OPTS)' \
					 IOPTS='$(CMIOPTS)' \
					 OBJ='$(OBJ)' \
					 IN_DIR='$(RL_DIR)'  \
					 BLD_ENV='$(BLD_ENV_HEX)'


clean:
	@$(MAKE) $(MK_QUIET) -f vl.mak clean OBJ='$(OBJ)' IN_DIR='$(VL_DIR)'
	@$(MAKE) $(MK_QUIET) -f cm_l2.mak clean OBJ='$(OBJ)' IN_DIR='$(CM_DIR)'
	@$(MAKE) $(MK_QUIET) -f qu.mak clean OBJ='$(OBJ)' IN_DIR='$(QU_DIR)'
	@$(MAKE) $(MK_QUIET) -f rg.mak clean OBJ='$(OBJ)' IN_DIR='$(RG_DIR)'
	@$(MAKE) $(MK_QUIET) -f kw_split.mak clean OBJ='$(OBJ)' IN_DIR='$(KW_DIR)'
	@$(MAKE) $(MK_QUIET) -f pj.mak clean OBJ='$(OBJ)' IN_DIR='$(PJ_DIR)' SEC_FLAG='$(TENB_AS_SECURITY)'
	@$(MAKE) $(MK_QUIET) -f rl_l2.mak clean OBJ='$(OBJ)' IN_DIR='$(RL_DIR)'
	@$(MAKE) $(MK_QUIET) -f vl.mak clean OBJ='$(OBJ_I)' IN_DIR='$(VL_DIR)'
	@$(MAKE) $(MK_QUIET) -f rg.mak clean OBJ='$(OBJ_I)' IN_DIR='$(RG_DIR)'
	@$(MAKE) $(MK_QUIET) -f kw_split.mak clean OBJ='$(OBJ_I)' IN_DIR='$(KW_DIR)'
	@$(MAKE) $(MK_QUIET) -f pj.mak clean OBJ='$(OBJ_I)' IN_DIR='$(PJ_DIR)' SEC_FLAG='$(TENB_AS_SECURITY)'


# DO NOT DELETE THIS LINE -- make depend depends on it.

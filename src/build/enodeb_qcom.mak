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
#
#                    Email: support@trillium.com
#                    Web: http://www.radisys.com 
# 
#*********************************************************************17*/

#
#*********************************************************************17*/
#
#********************************************************************20**
#
#       Name:   LTE eNodeB Sample Application
#
#       Type:   make file
#
#       Desc:   Compile, assemble and link product software for
#               various configurations. Further comments are
#               embedded in the file.
#
#               This file supports a variety of environments and
#               build targets. The default build target will be the
#               portable target for the Linu 2.4.x with GNU C(gcc)
#
#       Env:    Linux 2.4.x with GNU C (gcc)
#
#               BUILD TARGETS:
#
#               clean            remove all object files
#
#       File:  enodeb_qcom.mak
#
#
#       Prg:   mraj
#
#********************************************************************21*/
#=======================================================================

#-----------------------------------------------------------------------
# macro for output file name and makefile name
#
ROOT=enodeb
SY_DIR=./obj
#LIBS_DIR=./libs
RRM_LIBS_DIR=./libs
SON_LIBS_DIR=./libs
STOPTS=-DENB

OUTNAME=$(ROOT)

V=1
Q=
S=
# Check for verbose build
ifneq ($(V),1)
    Q:=@
    S:=-s
endif
export Q
export S


#-----------------------------------------------------------------------
# macros for td specific options
#

# MTSS defines
MTOPTS=-DSS -DSS_MT -UCONRD -DCONAVL -UNOFILESYS

#MTOPTS=-DSS -DSS_MT -UCONRD -DNOCONAVL -UNOFILESYS -USS_DEBG_MEM_CORRUP_LEVEL1 -DCCPU_OPT

#Handover Support flag
LTE_HO_SUPPORT = -DLTE_HO_SUPPORT
TENB_AS_SECURITY = -DTENB_AS_SECURITY
RLOG_CMN = -DTENB_TTI_PERF -UBRDCM_RLOG
RLOG_OPTS_BIN = $(RLOG_CMN) -URLOG_ENABLE_TEXT_LOGGING -DRLOG_DEBUG_MODE -DRLOG_USE_CIRCULAR_BUFFER -DRLOG_USE_TTI_LOGGING 
RLOG_OPTS_TEXT = $(RLOG_CMN) -DRLOG_ENABLE_TEXT_LOGGING -DRLOG_DEBUG_MODE 

#System Specific 
TENB_MAX_UE_SUPPORTED= -DTENB_MAX_UE_SUPPORTED=32
#SSI Files
SSOPTS=-DSS

# EnodeB End to End Compile Options
CMENBE2EOPTS=-USS_LIGHT_MEM_LEAK_STS -DCMINET_BSDCOMPAT -DSS_TICKS_SEC=100 -DCMFILE_REORG_1 -DCM_INET2 \
          -DCMFILE_REORG_2 -DSSINT2 -DCMKV2 -DHI_MULTI_THREADED \
          -DSS_PERF -DSS_WL_REGION=1 -DNO_ERRCLS -DNOERRCHK -DSS_M_PROTO_REGION \
          -DDEBUGP -UCM_PASN_DBG -DREL_850 -DSS_FLOAT -DRGR_RRM_TICK \
          -DSLES9_PLUS -USS_4GMX_UCORE -DHI_NO_CHK_RES -DLSZV1 $(LTE_HO_SUPPORT) \
          -DRGR_SI_SCH -DRGR_V1 -DLTERRC_REL9 -DNHU_VER1 -DLNH_VER1 -DSI_NEW -DLTEMAC_MIMO -DDISABLE_RRCSEC \
          -DLEGV2 -DEGTP_U_REL_9 -DCL_MAC_LWLC -DCTF_VER3 \
          -DSZ_ENB -DTFU_UPGRADE -DRGR_CQI_REPT -DRGR_V2 -DTENB_RTLIN_CHANGES -UNOFILESYS -DMLOG_ENABLED \
          -DTTI_PROC_FIRST -UYS_MS_NO_ULCQI -USS_DIAG \
          -UDISABLE_PUCCH_PWRCNTRL -DSZTV2 -DTENB_SPLIT_ARCH_SUPPORT -UTENB_T2K3K_SPECIFIC_CHANGES \
          -DLTEMAC_DLUE_TMGOPTMZ -DLTEMAC_RGU_PAD -DTENB_STATS -DTENB_DISABLE_DL_ZBC \
          -USS_MULTICORE_SUPPORT -DSS_USE_ICC_MEMORY -USS_LOCKLESS_MEMORY -DTENB_SPLIT_ARCH -DSZTV3 \
          -DDLHQ_RTT_OPT -DYS_PHY_3_8_2 -DLOWLATENCY\
          -DS1AP_REL9A6 -DSS_USE_ZBC_MEMORY -UHI_USE_ZBC_MEMORY -DTUCL_TTI_RCV \
          -UHI_USE_ZBC_MEMORY_UL -DUSE_LINUX -DSS_THR_REG_MAP -DMSPD_T2K_TRACK_BUG -DREVIEW -DCZTV1 \
			 $(TENB_AS_SECURITY) -DCPJ_V2 -UPHY_SHUTDOWN_ENABLE -UE_TM -DWR_RSYS_KPI -DLTE_L2_MEAS \
          -UMAC_SCH_STATS -UPHY_ERROR_LOGING -UMS_MBUF_CORRUPTION -USSI_MEM_CORR_PREVENTION -DLA_VER2 -ULA_VER3 -DUL_LA -DDL_LA \
          -USS_ROUTE_MSG_CORE1 -UPJ_PKT_FWD -ULTE_START_UL_DL_DATA -UREM_ENABLE -DWR_WATCHDOG\
          -DWR_RSYS_KPI -DSPACC_NONBLOCKING -DLA_VER2 -UKWSELFPSTDLCLEAN -DNL -USS_LICENSE_CHECK \
          -UFLAT_BUFFER_OPT -USS_DBUF_REFLOCK_DISABLE -DLTE_QCOM -URSYS_WIRESHARK \
			 $(TENB_MAX_UE_SUPPORTED) -UENB_CPU_OVERLOAD_CONTROL -ULTEMAC_SPS -UTENB_UL_FP -UPDCP_RLC_DL_RBUF \
			 -UDAM_PDCP_OPT -UMAC_RLC_UL_RBUF -URLC_NO_HDR_CPY -USS_MEM_LEAK_DETEC_LEVEL1 -USS_SELECTIVE_MEM_LEAK_STS\
			 -USS_DEBG_MEM_CORRUP_LEVEL1 -DLTE_TDD


ifeq (${BLDENV}, lnx_e2e_ms)
CMENBE2EOPTS += $(RLOG_OPTS_TEXT)
endif
ifeq (${BLDENV}, lnx_e2e_ms_blog)
CMENBE2EOPTS += $(RLOG_OPTS_BIN)
endif
ifeq (${BLDENV}, lnx_ms_pre)
 CMENBE2EOPTS += $(RLOG_OPTS_BIN)
endif


# The options below are used to pass on to the actual product compilations
# Just this one variable is passed for individual compilations and hence
# all the needed options are added from various other defined option macros

#ALL PAL layer options
TFUOPTS=-DTFU_WRR_2 -DTFU_TDD -ULCTFU

# All LTE MAC product options
CRGOPTS= -DLCNHLICRG -DLCCRG -DLCRGUICRG
RGUOPTS=-URG -DKW -DLCKWLIRGU -DLCRGU -DLCRGUIRGU -DCCPU_OPT -DLWLCKWLIRGU -DLWLCRGUIRGU
LRGOPTS=-DLCSMMILRG -DLCLRG -DSM -URG -DLCRGMILRG -DRM_INTF -DRGM_LWLC -DRGM_LC 
RGROPTS=-URG -DLCWRLIRGR -DLCRGR -DLWLCWRLIRGR -DLCRGUIRGR
RGOPTS=-URG -DRG_PHASE2_SCHED $(RGUOPTS) $(CRGOPTS) $(LRGOPTS) $(RGROPTS) $(TFUOPTS) -URG_DEBUG -UTTI_PROC -UPROC_DL -USCH_TTI_PROC -UTOM -UPAL_MAC_PROC -UUL_PROC_MAC -DRG_DL_DELTA_CHANGE -DRG_CMNCH_SCHDELTA_CHANGE -DRG_ULSCHED_AT_CRC -DTFU_RECPREQ_DELTA_CHANGE -DTFU_DL_DELTA_CHANGE -DTFU_UL_DELTA_CHANGE -DTFU_DLHQFBK_DELTA_CHANGE -UBUG_FIX -ULWLCRGLITFU -ULCSCH  -DLCRG


# All LTE EGTP-U product options
EGTOPTS=-DEG -DLCEGT -DLCEGUIEGT -DCCPU_OPT -DLCEULIEGT -DEGTPU_HDR_ALWAYS_PRSNT
LEGOPTS=-DLCSMMILEG -DLCLEG -DSM -DEG -DLCEGMILEG -DLCSMEGMILEG -DHI
EGOPTS=-DEG $(EGTOPTS) $(LEGOPTS) -UEG_DEBUG -DLCEGLIHIT -DWR


#All Convergence Layer options
LCLOPTS=-DLCLCL -DSM -DCL
CLOPTS=-DCL $(LCLOPTS) -DLCCLMILCL -DDLHQ_RTT_OPT -DLOWLATENCY #L2Split

#All RLC product options
CKWOPTS=-DLCNHLICKW -DLCCKW -DLCKWUICKW
KWUOPTS=-DKW -DNH -DLCKWUIKWU -DLWLCKWUIKWU -DLCKWU -DLCNHLIKWU -DCCPU_OPT
LKWOPTS=-DSM -DKW -DLCKWMILKW -DLCLKW
LPJOPTS=-DSM -UPJ -DLCPJMILPJ -DLCLPJ
PJUWROPTS=-UPJ -DNH -DLCPJU  -DLCWRLIPJU -DLCPJUIPJU
PJUNHOPTS=-UPJ  -DLCPJU  -DLCNHLIPJU -DLCPJUIPJU
CPJOPTS=-DLCCPJ -DNH -DKW -DLCNHLICPJ -DLCPJUICPJ
KWOPTS=-DKW -DKW_PDCP $(CKWOPTS) $(KWUOPTS) $(CPJOPTS) $(LPJOPTS) $(LKWOPTS) $(RGUOPTS) $(PJUNHOPTS) -URLC_STATUS_GEN -UUL_PROC_MAC
#ALL RRC product options
LNHOPTS=-DNH -DSM -DLCLNH -DLCSMMILNH -DLCSMNHMILNH -DLCNHMILNH
NHUOPTS=-DLCNHU -DWR -DNX -DLCWRLINHU -DLCNHUINHU
NHOPTS=-DNH $(LNHOPTS) $(NHUOPTS) $(CRGOPTS) $(CKWOPTS) $(CPJOPTS) $(KWUOPTS) $(PJUNHOPTS)\
       -DRX -DRNC_INTEG_CCPU

#ALL RRC application product options
LWROPTS=-DWR -DSM -DLCLWR -DLCSMMILWR -DLCWRMILWR
CTFOPTS=-DLCCTF -DWR -DLCWRLICTF -DLCTFUICTF
WROPTS=-DWR $(LWROPTS) $(NHUOPTS) $(CTFOPTS) $(RGROPTS) $(PJUWROPTS) $(SZTOPTS) $(EGTOPTS) $(CZTOPTS) \
		 -UWR_PERF_MEAS -UWR_WITHOUT_CNE -DWR_SIBS_ENBLD -UWR_UM_MODE -UWR_UM_MODE_BI_DIR -UWR_TEST_CODE \
		 -UWR_TEST_CLI -UWR_SUPPORT_RLC_UM_MODE -UWR_RLC_UM_MODE_ENABLE -DWR_DL_CQI -DWR_GRPPWR_CNTRL \
		 -UWR_CLI_TARGET_BOARD -UWR_CQI_REPT -ULTE_ENB_PERF -UENB_OVRLOAD -DMME_LDBAL -DWR_DETECT_RLF

#All product options
SZTOPTS=-DSZ -DUZ -DLCUZLISZT -DLWLCUZLISZT -DLCSZUISZT -DLCSZT -DPTSZUISZT -US1AP_REL8A0 -DCM_PASN_CRIT_HANDL -DLSZV1 -DSZTV1 -DSZTV3
LSZOPTS=-DLCLSZ -DLCSCT -DSM -DSZ -DLCSMSZMILSZ  -DLCSZMILSZ

#All product options
CZTOPTS=-DCZ -DRS -DLCRSLICZT -DLWLCRSLICZT -DLCCZUICZT -DLCCZT -DPTCZUICZT
LCZOPTS=-DLCLCZ -DLCSCT -DSM -DCZ -DLCSMCZMILCZ  -DLCSZMILCZ


#All SCTP product options
LSBOPTS=-DLCLSB -DSM -DSB -DLSB4  -DLSB8 -DHI_REL_1_4
SCTOPTS=-DSB  -DLCSBUISCT -DLCSZLISCT -DLCCZLISCT
HITOPTS=-DLCSBLIHIT -DLCHIUIHIT -DHI_REL_1_4 -DLCEGLIHIT
SBOPTS = $(LSBOPTS) $(HITOPTS) $(SCTOPTS) -DLCSBMILSB -DLCSMSBMILSB -DSB_CHECKSUM -DSB_CHECKSUM_CRC

SZOPTS=-DSZ $(SZTOPTS) $(LSZOPTS) $(SCTOPTS) -DLCSZMILSZ -DSM \
	  -DSZ_ENB -DSZ_DYNAMIC_PEER -DSZ_USTA

CZOPTS=-DCZ $(CZTOPTS) $(LCZOPTS) $(SCTOPTS) -DLCCZMILCZ -DSM \
	  -DCZ_ENB -DCZ_DYNAMIC_PEER -DCZ_USTA

#All TUCL product options
LHIOPTS=-DLCHIMILHI -DSM -DLCLHI
HIOPTS= $(LHIOPTS) $(HITOPTS) -DHI -DLCHIMILHI -DSM -DLCSMHIMILHI -DEG -DHI_NO_CHK_RES

#ALL RRC product options
LRXOPTS=-DRX -DSM -DLCLRX

#All SON product options
NLOPTS = -DNL

#Stack Manager Options
SMOPTS= -DLCSMMILCL -DLCCLMILCL

#-----------------------------------------------------------------------
# macros for sw and include directories
#
CODEPATH=../
MT_DIR=$(CODEPATH)/mt
CM_DIR=$(CODEPATH)/cm
RL_DIR=$(CODEPATH)/rlog/
WR_DIR=$(CODEPATH)/enbapp
RM_DIR=$(CODEPATH)/lterrm/src
NL_DIR=$(CODEPATH)/lteson/src
YS_DIR=$(CODEPATH)/lteclms
NH_DIR=$(CODEPATH)/lterrc
SZ_DIR=$(CODEPATH)/s1ap
CZ_DIR=$(CODEPATH)/x2ap
SB_DIR=$(CODEPATH)/sctp
HI_DIR=$(CODEPATH)/tucl
EG_DIR=$(CODEPATH)/egtpu
RG_DIR=$(CODEPATH)/ltemac
KW_DIR=$(CODEPATH)/lterlc
PJ_DIR=$(CODEPATH)/ltepdcp
REM_DIR=$(CODEPATH)/lteremapp/
REMCL_DIR=$(REM_DIR)/remcl/src/
OAMCL_DIR=$(CODEPATH)/oamcl/
WATCHDOG_DIR=$(CODEPATH)/watchdog/

BLD_LNX_CM_OBJS=$(SY_DIR)/libcm.a
BLD_LNX_SS_OBJS=$(SY_DIR)/libmt.a



ifeq (${BLDENV}, lnx_e2e_ms)
BLD_LNX_SB_OBJS1=$(SY_DIR)/liblnxsb.a
BLD_LNX_HI_OBJS1=$(SY_DIR)/liblnxhi.a
BLD_LNX_WR_E2E_OBJS1=$(SY_DIR)/libe2elnxwr_ms.a
BLD_LNX_EG_OBJS1=$(SY_DIR)/liblnxeg.a
BLD_LNX_RM_OBJS1=$(RRM_LIBS_DIR)/liblnxrm.a
BLD_LNX_NL_OBJS1=$(SON_LIBS_DIR)/liblnxnl.a
BLD_LNX_NH_OBJS1=$(SY_DIR)/liblnxnh.a
BLD_LNX_SZ_OBJS1=$(SY_DIR)/liblnxsz.a
BLD_LNX_OAMCL_OBJS=$(SY_DIR)/liboamcl.a
ifeq ($(LTE_HO_SUPPORT),-DLTE_HO_SUPPORT)
BLD_LNX_CZ_OBJS1=$(SY_DIR)/liblnxcz.a
endif
endif

ifeq (${BLDENV}, lnx_e2e_ms_blog)
BLD_LNX_SB_OBJS=$(SY_DIR)/libsb.a
BLD_LNX_HI_OBJS=$(SY_DIR)/libhi.a
BLD_LNX_WR_E2E_OBJS=$(SY_DIR)/libe2elnxwr_ms.a
BLD_LNX_EG_OBJS=$(SY_DIR)/libeg.a
BLD_LNX_RM_OBJS=$(RRM_LIBS_DIR)/librm.a
BLD_LNX_NL_OBJS=$(SON_LIBS_DIR)/libnl.a
BLD_LNX_NH_OBJS=$(SY_DIR)/libnh.a
BLD_LNX_SZ_OBJS=$(SY_DIR)/libsz.a
BLD_LNX_OAMCL_OBJS=$(SY_DIR)/liboamcl.a
ifeq ($(LTE_HO_SUPPORT),-DLTE_HO_SUPPORT)
BLD_LNX_CZ_OBJS=$(SY_DIR)/libcz.a
endif
endif

BLD_LNX_NH_PREPROC=$(SY_DIR)/nhprepro
BLD_LNX_SZ_PREPROC=$(SY_DIR)/szprepro
BLD_LNX_CZ_PREPROC=$(SY_DIR)/czprepro
BLD_LNX_SB_PREPROC=$(SY_DIR)/sbprepro
BLD_LNX_HI_PREPROC=$(SY_DIR)/hiprepro
BLD_LNX_WR_E2E_PREPROC=$(SY_DIR)/wrprepro
BLD_LNX_EG_PREPROC=$(SY_DIR)/egprepro
BLD_LNX_RM_PREPROC=$(SY_DIR)/rmprepro
BLD_LNX_NL_PREPROC=$(SY_DIR)/nlprepro



# make the list for .x and .h dependent

# common include files(no makedepend)
CM_INC=\
	$(CM_DIR)/envdep.h         $(CM_DIR)/envind.h         \
	$(CM_DIR)/envopt.h                                    \
	$(CM_DIR)/gen.h            $(CM_DIR)/gen.x            \
	$(CM_DIR)/cm_hash.x        $(CM_DIR)/cm_hash.h        \
	$(CM_DIR)/cm_lte.x         $(CM_DIR)/cm_lte.h        \
	$(CM_DIR)/cm5.h            $(CM_DIR)/cm5.x            \
	$(CM_DIR)/cm_umts.h        $(CM_DIR)/cm_umts.x        \
	$(CM_DIR)/cm_err.h         $(CM_DIR)/cm_lib.x         \
	$(CM_DIR)/cm_mblk.h        $(CM_DIR)/cm_mblk.x        \
	$(CM_DIR)/cm_tkns.h        $(CM_DIR)/cm_tkns.x        \
	$(CM_DIR)/cm_llist.h       $(CM_DIR)/cm_llist.x       \
	$(CM_DIR)/cm_hash.h        $(CM_DIR)/cm_hash.x        \
	$(CM_DIR)/cm_inet.h        $(CM_DIR)/cm_inet.x        \
	$(CM_DIR)/cm_gen.h         $(CM_DIR)/cm_gen.x         \
	$(CM_DIR)/cm_tpt.h         $(CM_DIR)/cm_tpt.x         \
	$(CM_DIR)/cm_dns.h         $(CM_DIR)/cm_dns.x         \
	$(CM_DIR)/ssi.h            $(CM_DIR)/ssi.x            \
	$(CM_DIR)/nhu.h            $(CM_DIR)/nhu.x            \
	$(CM_DIR)/crg.h            $(CM_DIR)/crg.x            \
	$(CM_DIR)/egt.h            $(CM_DIR)/egt.x            \
	$(CM_DIR)/hit.h            $(CM_DIR)/hit.x            \
	$(CM_DIR)/kwu.h            $(CM_DIR)/kwu.x            \
	$(CM_DIR)/szt.h            $(CM_DIR)/szt.x            \
	$(CM_DIR)/czt.h            $(CM_DIR)/czt.x            \
	$(CM_DIR)/sct.h            $(CM_DIR)/sct.x            \
	$(CM_DIR)/tfu.h            $(CM_DIR)/tfu.x            \
	$(CM_DIR)/rgr.h            $(CM_DIR)/rgr.x            \
	$(CM_DIR)/rgm.h            $(CM_DIR)/rgm.x            \
	$(CM_DIR)/pju.h            $(CM_DIR)/pju.x            \
	$(CM_DIR)/cm_pint.h        $(CM_DIR)/cm_pint.x        \
	$(CM_DIR)/cm_perr.h                                   \
	$(CM_DIR)/nhu_pk.x         $(CM_DIR)/nhu_unpk.x       \
	$(CM_DIR)/nhu_asn.h        $(CM_DIR)/nhu_asn.x       \
	$(CM_DIR)/szt_pk.x         $(CM_DIR)/szt_unpk.x       \
	$(CM_DIR)/czt_pk.x         $(CM_DIR)/czt_unpk.x       \
	$(CM_DIR)/szt_asn.h        $(CM_DIR)/szt_asn.x       \
	$(CM_DIR)/czt_asn.h        $(CM_DIR)/czt_asn.x       \
	$(CM_DIR)/leg.h            $(CM_DIR)/leg.x            \
	$(CM_DIR)/lhi.h            $(CM_DIR)/lhi.x            \
	$(CM_DIR)/lkw.h            $(CM_DIR)/lkw.x            \
	$(CM_DIR)/lnh.h            $(CM_DIR)/lnh.x            \
	$(CM_DIR)/lys.h            $(CM_DIR)/lys.x            \
	$(CM_DIR)/lrg.h            $(CM_DIR)/lrg.x            \
	$(CM_DIR)/lsb.h            $(CM_DIR)/lsb.x            \
	$(CM_DIR)/lsz.h            $(CM_DIR)/lsz.x            \
	$(CM_DIR)/lcz.h            $(CM_DIR)/lcz.x            \
	$(CM_DIR)/lpj.h            $(CM_DIR)/lpj.x            \
	$(CM_DIR)/cm_pasn.x                                   \
	$(CM_DIR)/cm_mem.h         $(CM_DIR)/cm_mem_wl.x         \
        $(CM_DIR)/lnl.h            $(CM_DIR)/nlu.h            \
        $(CM_DIR)/lnl.x            $(CM_DIR)/nlu.x       \

RL_INC=\
	$(RL_DIR)/rl_common.h   $(RL_DIR)/rl_rlog.h \
        $(RL_DIR)/rl_interface.h  $(RL_DIR)/rl_redirect.h

#-----------------------------------------------------------------------
# macros for compile and link options

LNXENV=-DSUNOS -DSS -DSS_MT -DANSI -D_GNU_SOURCE -DSS_LINUX -D_REENTRANT -D__EXTENSIONS__ \
    -DDEBUGNOEXIT -DDEBUG_LEWRL=$(DEBUG) -DIPV6_SUPPORTED -DSB_IPV6_SUPPORTED -DEG_IPV6_SUPPORTED -DWR_IPV6_OAM_WORKAROUND

# compiler:
ARCH = cortex-a9

#LNX_CC="arm-v7a-linux-gnueabi-gcc -mcpu=$(ARCH) "
LNX_CC="arm-v7a-linux-gnueabi-gcc"
LNX_CC_STRIP=arm-v7a-linux-gnueabi-strip --strip-all 
#LNX_LD="arm-v7a-linux-gnueabi-gcc -mcpu=$(ARCH) "
LNX_LD="arm-v7a-linux-gnueabi-gcc"
LNX_AR=arm-v7a-linux-gnueabi-ar
#LNX_GPP_CC=arm-v7a-linux-gnueabi-g++ -mcpu=$(ARCH) 
#LNX_GPP_LD="arm-v7a-linux-gnueabi-g++ -mcpu=$(ARCH) "
LNX_GPP_CC=arm-v7a-linux-gnueabi-g++ 
LNX_GPP_LD="arm-v7a-linux-gnueabi-g++"
LNX_QCOM_CC=arm-v7a-linux-gnueabi-g++

#-Wstrict-prototypes -Wmissing-prototypes -fno-strict-aliasing -fsigned-char -Wno-variadic-macros --std=c99"

#LNXCOPTS=" -O3 -g  -pipe -pedantic -Wall -Werror -Wno-comment -Wshadow -Wcast-qual \
-Wno-variadic-macros -fno-strict-aliasing -fsigned-char --std=c99"


LNXTMPCOPTS=" -O3 -g  -pipe -pedantic -Wall -Wno-comment -Wshadow -Wcast-qual \
-Wstrict-prototypes -Wmissing-prototypes -fno-strict-aliasing -fsigned-char -Wno-variadic-macros --std=c99"

LNXCOPTS=$(LNXTMPCOPTS)

LNXCPPOPTS=" -O3 -g  -pipe -pedantic -Wall -Wno-long-long -Wno-comment -Wshadow -Wcast-qual \
-fno-strict-aliasing -fsigned-char -Wno-variadic-macros"

LNXLOPTSCPP=-lstdc++
# object files:
# o     UNIX
OBJ=o                                    # object files, sunos

#######################################################################
# library path
# for linking with TurnerProject
LNXMSLIBPATH= 
# for linking with only phystub
#######################################################################

#for linking with Qcom libraries
QCOM_LOPTS=-Wl,-rpath-link, -L$(QCOM_SYSROOT)/lib -lqmi_csi -lqmi_cci -lqmi_encdec -lqmi_common -lstringl -lQctPrivs -lnlapiqmi 
#path for  MS libs and objs
LNXMSLOPTS= -lm  -lrt -lpthread  $(QCOM_LOPTS)
# for linking with only phystub
LNXLOPTS=-lpthread -lnsl -lrt -lm -losal -lbsp -turnerlinux

# include options:
LNXIOPTS  =  -I$(MT_DIR) -I$(CM_DIR) -I$(EG_DIR) -I$(SZ_DIR) -I$(CZ_DIR) -I$(RM_DIR) \
	-I$(KW_DIR)  -I$(NL_DIR) -I$(RL_DIR)  -I$(OAMCL_DIR)

# compiler options:

ifeq ($(APP_WITH_OAM),YES)
ENABLE_APP_WITH_OAM=OAM_ENABLE
endif

ifeq ($(OAM_ENABLE),OAM_ENABLE)
BLD_LNX_OAM_LIB=lnx_sm_oam_lib
endif

ifeq ($(VENDOR),RSYS)
MSM_ROOT_DIR=$(CODEPATH)/tenb_commonplatform/software
OAM_LIB_MAKE_PATH=$(MSM_ROOT_DIR)/products/fap/hbs2/hbs2-iu/
LNXMSLOPTS+= -ldl -lsm -lRadisys-mib-common -lthreeway-comms -lthreeway-messaging -lthreeway-mobnet -lthreeway-system -lstdc++
else
OAM_LIB_MAKE_PATH=.
LNXMSLOPTS+= -L$(OAM_LIB_PATH) -lsm-oam
endif

QCOMLOPTS=-L$(QCOM_SYSROOT)/lib -lFsmOamClientProtocol -lFsmOamMgmtXml -lqmi_csi -lqmi_cci -lqmi_encdec -lqmi_common -lQctInfra -lQctInfraC -lQctOsal -lsharedqmi -lrt
QCOMCMNLOPTS=-L$(QCOM_SYSROOT)/usr/lib -lxml2 -lboost_program_options-mt -lboost_thread-mt -lboost_chrono-mt -lboost_regex-mt -lboost_system-mt -lz -lstringl -lfidwd -lboost_date_time-mt -lrt

# MTSS defines
CCLNXMTOPTS=$(DEBUGPOPTS) $(LNXENV) $(MTOPTS) $(CMOPTS) $(STOPTS)

# SM defines
CCLNXSMOPTS= $(LNXENV) $(SMOPTS) $(CMOPTS) $(STOPTS)

# NH defines
CCLNXNHOPTS=  $(LNXENV) $(NHOPTS) $(CMOPTS) $(STNHOPTS)

#KW defines
CCLNXKWOPTS= $(LNXENV) $(KWOPTS)  $(STOPTS) $(CMOPTS)

#PJ defines
CCLNXPJOPTS= $(LNXENV) $(KWOPTS)  $(STOPTS) $(CMOPTS)

#RG defines
CCLNXRGOPTS= $(LNXENV) $(RGOPTS)  $(STOPTS) $(CMOPTS)

#EG defines
CCLNXEGOPTS= $(LNXENV) $(EGOPTS)  $(STOPTS) $(CMOPTS)

#SZ defines
CCLNXSZOPTS= $(LNXENV) $(SZOPTS)  $(STOPTS) $(CMOPTS)

#CZ defines
CCLNXCZOPTS= $(LNXENV) $(CZOPTS)  $(STOPTS) $(CMOPTS)

#WR defines
CCLNXWROPTS= $(LNXENV) $(WROPTS)  $(STOPTS) $(CMOPTS)

#SB defines
CCLNXSBOPTS= $(LNXENV) $(SBOPTS)  $(STOPTS) $(CMOPTS)

#HI defines
CCLNXHIOPTS= $(LNXENV) $(HIOPTS)  $(STOPTS) $(CMOPTS)

#RRM defines 
CCLNXRMOPTS= $(LNXENV) $(RMOPTS)  $(STOPTS) $(CMOPTS)

#SON defines
CCLNXNLOPTS= $(LNXENV) $(NLOPTS)  $(STOPTS) $(CMOPTS)

#common options
CCLNXCMOPTS= $(LNXENV) $(CMOPTS) $(SMOPTS) \
         $(MKUOPTS) $(CMKOPTS)  \
         $(CRLOPTS) $(RLUOPTS)  $(STOPTS) \
         $(CTCOPTS) $(TCUOPTS) $(LRXOPTS) 


#/* Libraries for Pre-processing */
ifeq ($(RSYS_RRM),YES)
LNXPRDENB_E2E_QCOM_PRES: $(BLD_LNX_RM_PREPROC)
endif

ifeq ($(RSYS_SON),YES)
LNXPRDENB_E2E_QCOM_PRES: $(BLD_LNX_NL_PREPROC)
endif

LNXPRDENB_E2E_QCOM_PRES:\
   $(BLD_LNX_NH_PREPROC) \
   $(BLD_LNX_SZ_PREPROC) \
   $(BLD_LNX_CZ_PREPROC) \
   $(BLD_LNX_SB_PREPROC) \
   $(BLD_LNX_HI_PREPROC) \
   $(BLD_LNX_WR_E2E_PREPROC) \
   $(BLD_LNX_EG_PREPROC)
   

#/* Libraries for Object files with-Out Pre-processing */
ifeq ($(RSYS_RRM),YES)
LNXPRDENB_E2E_QCOM_OBJS: $(BLD_LNX_RM_OBJS1)
endif

ifeq ($(OAM_ENABLE),OAM_ENABLE)
LNXPRDENB_E2E_QCOM_OBJS: $(BLD_LNX_OAM_LIB) 
endif

ifeq ($(RSYS_REMLIB),YES)
BLD_LNX_REM_LIB=librem.a
LNXPRDENB_E2E_QCOM_OBJS: $(BLD_LNX_REM_LIB) 
endif

ifeq ($(RSYS_OAMCLLIB),YES)
BLD_LNX_OAMCL_OBJS=liboamcl.a
LNXPRDENB_E2E_QCOM_OBJS: $(BLD_LNX_OAMCL_OBJS) 
endif

ifeq ($(RSYS_REMAPP),YES)
BLD_LNX_REM_OBJS=rem
LNXPRDENB_E2E_QCOM_OBJS: $(BLD_LNX_REM_OBJS)
endif

ifeq ($(CREATE_ENB),YES)
LNXPRDENB_E2E_QCOM_OBJS: \
    $(BLD_LNX_CM_OBJS) \
    $(BLD_LNX_SS_OBJS) \
    $(BLD_LNX_NH_OBJS1) \
    $(BLD_LNX_SZ_OBJS1) \
    $(BLD_LNX_CZ_OBJS1) \
    $(BLD_LNX_SB_OBJS1) \
    $(BLD_LNX_HI_OBJS1) \
    $(BLD_LNX_WR_E2E_OBJS1) \
    $(BLD_LNX_EG_OBJS1)\
    $(BLD_LNX_NL_OBJS1)
endif


#/* Libraries for Object files with Pre-processing */
ifeq ($(RSYS_RRM),YES)
LNXPRDENB_E2E_QCOM_OBJS_FROMPRE: $(BLD_LNX_RM_OBJS)
endif

ifeq ($(RSYS_SON),YES)
LNXPRDENB_E2E_QCOM_OBJS_FROMPRE: $(BLD_LNX_NL_OBJS)
endif

ifeq ($(OAM_ENABLE),OAM_ENABLE)
LNXPRDENB_E2E_QCOM_OBJS_FROMPRE: $(BLD_LNX_OAM_LIB) 
endif

ifeq ($(RSYS_REMLIB),YES)
BLD_LNX_REM_LIB=librem.a
LNXPRDENB_E2E_QCOM_OBJS_FROMPRE: $(BLD_LNX_REM_LIB) 
endif

ifeq ($(RSYS_OAMCLLIB),YES)
BLD_LNX_OAMCL_OBJS=liboamcl.a
LNXPRDENB_E2E_QCOM_OBJS_FROMPRE: $(BLD_LNX_OAMCL_OBJS) 
endif


ifeq ($(RSYS_REMAPP),YES)
BLD_LNX_REM_OBJS=rem
LNXPRDENB_E2E_QCOM_OBJS_FROMPRE: $(BLD_LNX_REM_OBJS)
endif

ifeq ($(CREATE_ENB),YES)
	@echo "RIGHT PATH##################"
LNXPRDENB_E2E_QCOM_OBJS_FROMPRE: \
    $(BLD_LNX_CM_OBJS) \
    $(BLD_LNX_SS_OBJS) \
    $(BLD_LNX_NH_OBJS) \
    $(BLD_LNX_SZ_OBJS) \
    $(BLD_LNX_CZ_OBJS) \
    $(BLD_LNX_SB_OBJS) \
    $(BLD_LNX_HI_OBJS) \
    $(BLD_LNX_WR_E2E_OBJS) \
    $(BLD_LNX_EG_OBJS) 
endif

LNXENBE2EBIN_QCOM_FROMPRE=enb_e2e_ms_blog
LNXENBE2EBIN_MS=enb_e2e_ms

BASENAME=enodeb_qcom
MAKENAME=$(BASENAME).mak

BINLNXE2E_MS=lnx_e2e_ms
BINLNXE2E_QCOM_FROMPRE=lnx_e2e_ms_blog
LNXENBE2EPRE_MS=ms_prepro
PRELNXE2E_MS=lnx_ms_pre

#
#
# Linux Default GNU acceptance test build
#
strip:
	@echo "Stripping the binaries"
	mkdir -p ../../bin/unstripped
	cp -rf ../../bin/rsys ../../bin/unstripped/
	$(LNX_CC_STRIP) ../../bin/rsys/bin/post-office
	$(LNX_CC_STRIP) ../../bin/rsys/bin/rem 
	$(LNX_CC_STRIP) ../../bin/rsys/bin/tenpin 
	$(LNX_CC_STRIP) ../../bin/rsys/bin/ike-tunnel-ind 
	$(LNX_CC_STRIP) ../../bin/rsys/bin/cli 
	$(LNX_CC_STRIP) ../../bin/rsys/bin/oam 
	$(LNX_CC_STRIP) ../../bin/rsys/bin/enodeb 
	$(LNX_CC_STRIP) ../../bin/rsys/bin/tr069-v2 
	$(LNX_CC_STRIP) ../../bin/rsys/bin/watchdogd 
	$(LNX_CC_STRIP) ../../bin/rsys/bin/fileTransMgr 
#	$(LNX_CC_STRIP) ../../bin/rsys/l1/ltel1 

strip_wooam:
	mkdir -p ../bin/unstripped
	cp -rf ../../bin/rsys_withoutoam  ../../bin/unstripped/
	$(LNX_CC_STRIP) ../../bin/rsys_withoutoam/bin/enodeb 

#	$(LNX_CC_STRIP) ../bin/rsys_withoutoam/ike-tunnel-ind 


acc:
	@echo "Compiling acc##################"
	@echo $(MAKENAME)
	@$(MAKE) -f $(MAKENAME) $(BLDENV)

$(PRELNXE2E_MS):
	@$(MAKE) -f $(MAKENAME) $(LNXENBE2EPRE_MS) CC=$(LNX_CC) \
	LL=$(LNX_LD) COPTS=$(LNXCOPTS) CMOPTS='$(CMENBE2EOPTS)'

$(BINLNXE2E_QCOM_FROMPRE):
	@echo "Linux End to End BLOG############################"
	@echo "SYDIR::"'$(SY_DIR)'
	@mkdir -p $(SY_DIR)
	@$(MAKE) -f $(MAKENAME) $(LNXENBE2EBIN_QCOM_FROMPRE) CC=$(LNX_CC) \
	LL=$(LNX_LD) COPTS=$(LNXCOPTS) CMOPTS='$(CMENBE2EOPTS)'

#	@echo "Linux End To End Build"
	#@mkdir -p $(LIBS_DIR)
$(BINLNXE2E_MS):
	@echo "Linux End to End ############################"
	@mkdir -p $(SY_DIR)
	@$(MAKE) -f $(MAKENAME) $(LNXENBE2EBIN_MS) CC=$(LNX_CC) \
	LL=$(LNX_LD) COPTS=$(LNXCOPTS) CMOPTS='$(CMENBE2EOPTS)'

ifeq ($(CREATE_ENB),YES)
ifeq ($(APP_WITH_OAM),YES)
$(LNXENBE2EBIN_MS): LNXPRDENB_E2E_QCOM_OBJS
	$(LNX_QCOM_CC)  $(SY_DIR)/*.o -Wl,-R../lib/  $(QCOMCMNLOPTS) $(QCOMLOPTS) $(RRM_LIBS_DIR)/*.a  $(BLD_LNX_OAMCL_OBJS) -L$(OAM_LIBS_DIR) -L$(OAM_CPP_LIBS_DIR) $(LNXMSLIBPATH) $(LNXMSLOPTS) $(LNXLOPTSCPP) -o ./$(SY_DIR)/$(OUTNAME)
	cd $(WATCHDOG_DIR); $(MAKE) clean PRODUCT=hbs2-iu HARDWARE=hbs2-4 BUILD=arm-v7a-linux-gnueabi; $(MAKE) PRODUCT=hbs2-iu HARDWARE=hbs2-4 BUILD=arm-v7a-linux-gnueabi; cd -;
else
$(LNXENBE2EBIN_MS): LNXPRDENB_E2E_QCOM_OBJS
	$(LNX_QCOM_CC)  $(SY_DIR)/*.o -Wl,-R../lib/ $(QCOMCMNLOPTS) $(QCOMLOPTS) $(RRM_LIBS_DIR)/*.a $(BLD_LNX_OAMCL_OBJS) -L$(OAM_LIBS_DIR) -L$(OAM_CPP_LIBS_DIR) $(LNXMSLIBPATH) $(LNXMSLOPTS) $(LNXLOPTSCPP) -o ./$(SY_DIR)/$(OUTNAME)
	cd $(WATCHDOG_DIR); $(MAKE) clean PRODUCT=hbs2-iu HARDWARE=hbs2-4 BUILD=arm-v7a-linux-gnueabi; $(MAKE) PRODUCT=hbs2-iu HARDWARE=hbs2-4 BUILD=arm-v7a-linux-gnueabi; cd -;
endif
else
$(LNXENBE2EBIN_MS): LNXPRDENB_E2E_QCOM_OBJS
	cd $(WATCHDOG_DIR); $(MAKE) clean PRODUCT=hbs2-iu HARDWARE=hbs2-4 BUILD=arm-v7a-linux-gnueabi; $(MAKE) PRODUCT=hbs2-iu HARDWARE=hbs2-4 BUILD=arm-v7a-linux-gnueabi; cd -;
endif

ifeq ($(CREATE_ENB),YES)
ifeq ($(APP_WITH_OAM),YES)
$(LNXENBE2EBIN_QCOM_FROMPRE): LNXPRDENB_E2E_QCOM_OBJS_FROMPRE
	$(LNX_QCOM_CC)  $(SY_DIR)/*.o  -Wl,-R../lib/  $(QCOMCMNLOPTS) $(QCOMLOPTS) $(RRM_LIBS_DIR)/*.a $(BLD_LNX_OAMCL_OBJS) -L$(OAM_LIBS_DIR) -L$(OAM_CPP_LIBS_DIR) $(LNXMSLIBPATH) $(LNXMSLOPTS) $(LNXLOPTSCPP) -o ./$(SY_DIR)/$(OUTNAME)
	cd $(WATCHDOG_DIR); make clean PRODUCT=hbs2-iu HARDWARE=hbs2-4 BUILD=arm-v7a-linux-gnueabi; make PRODUCT=hbs2-iu HARDWARE=hbs2-4 BUILD=arm-v7a-linux-gnueabi; cd -;
else
$(LNXENBE2EBIN_QCOM_FROMPRE): LNXPRDENB_E2E_QCOM_OBJS_FROMPRE
	@echo "Building OBJ from PRE #######################"
	$(LNX_QCOM_CC)  $(SY_DIR)/*.o -Wl,-R../lib/ $(QCOMCMNLOPTS) $(QCOMLOPTS) $(RRM_LIBS_DIR)/*.a $(BLD_LNX_OAMCL_OBJS) -L$(OAM_LIBS_DIR) -L$(OAM_CPP_LIBS_DIR) $(LNXMSLIBPATH) $(LNXMSLOPTS) $(LNXLOPTSCPP) -o ./$(SY_DIR)/$(OUTNAME)
	cd $(WATCHDOG_DIR); $(MAKE) clean PRODUCT=hbs2-iu HARDWARE=hbs2-4 BUILD=arm-v7a-linux-gnueabi; $(MAKE) PRODUCT=hbs2-iu HARDWARE=hbs2-4 BUILD=arm-v7a-linux-gnueabi; cd -;
endif
else
$(LNXENBE2EBIN_QCOM_FROMPRE): LNXPRDENB_E2E_QCOM_OBJS_FROMPRE
	cd $(WATCHDOG_DIR); $(MAKE) clean PRODUCT=hbs2-iu HARDWARE=hbs2-4 BUILD=arm-v7a-linux-gnueabi; $(MAKE) PRODUCT=hbs2-iu HARDWARE=hbs2-4 BUILD=arm-v7a-linux-gnueabi; cd -;
endif

$(LNXENBE2EPRE_MS): LNXPRDENB_E2E_QCOM_PRES
# Portable compilation -> Point to acc directly
# Portability is not supported
pt:acc

#-----------------------------------------------------------------------
# UNIX link - absolute object file depends on relocatable object
# (.o) files

#
# Making Common Libraries
#
$(BLD_LNX_CM_OBJS):
	@$(MAKE) -f cm.mak $(SY_DIR)/libcm.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXCMOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(CM_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)'  CM_INC='$(CM_INC)'
	@$(MAKE) -f rl.mak $(SY_DIR)/librl COPTS=$(LNXCOPTS) \
   LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXCMOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(RL_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC) -c' LL='$(LL)' RL_INC='$(RL_INC)' SRC="c"

#
#Making eNodeB Sample application
#
$(BLD_LNX_WR_E2E_PREPROC):
	@$(MAKE) -f wr.mak $(SY_DIR)/wrprepro COPTS=$(LNXCOPTS) CTMPOPTS=$(LNXTMPCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXWROPTS)' SMPOPTS='$(CCLNXSMOPTS)' \
	HIOPTS='$(HIOPTS)' SBOPTS='$(SBOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(WR_DIR)' IN_DIR='$(WR_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='i' SRC="c"  CC='$(CC) -E' LL='$(LL)' CM_INC='$(CM_INC)' OAM_ENABLE='$(ENABLE_APP_WITH_OAM)'

$(BLD_LNX_WR_E2E_OBJS):
	@$(MAKE) -f wr.mak $(SY_DIR)/libe2elnxwr_ms.a COPTS=$(LNXCOPTS) CTMPOPTS=$(LNXTMPCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXWROPTS)' SMPOPTS='$(CCLNXSMOPTS)' \
	HIOPTS='$(HIOPTS)' SBOPTS='$(SBOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(WR_DIR)' IN_DIR='$(SY_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC='i'  CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)' OAM_ENABLE='$(ENABLE_APP_WITH_OAM)'

$(BLD_LNX_WR_E2E_OBJS1):
	@$(MAKE) -f wr.mak $(SY_DIR)/libe2elnxwr_ms.a COPTS=$(LNXCOPTS) CTMPOPTS=$(LNXTMPCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXWROPTS)' SMPOPTS='$(CCLNXSMOPTS)' \
	HIOPTS='$(HIOPTS)' SBOPTS='$(SBOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(WR_DIR)' IN_DIR='$(WR_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="c"  CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)' OAM_ENABLE='$(ENABLE_APP_WITH_OAM)'

#making RRM objects
$(BLD_LNX_RM_PREPROC):
	@$(MAKE) -f rm.mak $(SY_DIR)/rmprepro COPTS=$(LNXCPPOPTS) \
	LOPTS='$(LNXLOPTS)' LOPTSCPP='$(LNXLOPTSCPP)' POPTS='$(CCLNXWROPTS)' SMPOPTS='$(CCLNXSMOPTS)' \
	HIOPTS='$(HIOPTS)' SBOPTS='$(SBOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(RM_DIR)' IN_DIR='$(RM_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='i' SRC="c" SRC1="cpp" CC='$(LNX_GPP_CC) -E' LL='$(LNX_GPP_LD)' CM_INC='$(CM_INC)' 

$(BLD_LNX_RM_OBJS):
	@echo "REACHED::BLD_LNX_RM_OBJ"
	@mkdir -p $(RRM_LIBS_DIR)
	@$(MAKE) -f rm.mak $(SY_DIR)/librm.a COPTS=$(LNXCPPOPTS) \
	LOPTS='$(LNXLOPTS)' LOPTSCPP='$(LNXLOPTSCPP)' POPTS='$(CCLNXWROPTS)' SMPOPTS='$(CCLNXSMOPTS)' \
	HIOPTS='$(HIOPTS)' SBOPTS='$(SBOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(RM_DIR)'  IN_DIR='$(SY_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC1='i' SRC='i' CC='$(LNX_GPP_CC) -c' LL='$(LNX_GPP_LD)' CM_INC='$(CM_INC)' \
	AR='$(LNX_AR)'
	cp $(SY_DIR)/librm.a ./libs/ 

$(BLD_LNX_RM_OBJS1):
	@mkdir -p $(RRM_LIBS_DIR)
	@$(MAKE) -f rm.mak $(SY_DIR)/librm.a COPTS=$(LNXCPPOPTS) \
	LOPTS='$(LNXLOPTS)' LOPTSCPP='$(LNXLOPTSCPP)' POPTS='$(CCLNXWROPTS)' SMPOPTS='$(CCLNXSMOPTS)' \
	HIOPTS='$(HIOPTS)' SBOPTS='$(SBOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(RM_DIR)'  IN_DIR='$(RM_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC1='cpp' SRC='c' CC='$(LNX_GPP_CC) -c' LL='$(LNX_GPP_LD)' CM_INC='$(CM_INC)' \
	AR='$(LNX_AR)'
	cp $(SY_DIR)/librm.a ./libs/ 
#
#making SON module
#

$(BLD_LNX_NL_PREPROC):
	@$(MAKE) -f nl.mak $(SY_DIR)/nlprepro COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXNLOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(NL_DIR)' IN_DIR='$(NL_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='i' CC='$(CC) -E' LL='$(LL)' CM_INC='$(CM_INC)' SRC="c"
	
$(BLD_LNX_NL_OBJS):
	@$(MAKE) -f nl.mak $(SY_DIR)/libnl.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXNLOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(NL_DIR)' IN_DIR='$(SY_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)' SRC="i"
	mkdir -p ./libs/
	cp $(SY_DIR)/libnl.a ./libs/ 
$(BLD_LNX_NL_OBJS1):
	@$(MAKE) -f nl.mak $(SY_DIR)/libnl.a COPTS=$(LNXCOPTS) AR=$(LNX_AR) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXNLOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(NL_DIR)' IN_DIR='$(NL_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)' SRC="c"

	mkdir -p ./libs/
	cp $(SY_DIR)/libnl.a ./libs/ 


#
#Making OAM-CL
#
$(BLD_LNX_OAMCL_OBJS):
	@echo "OAMCL_DIR"'$(OAMCL_DIR)'
	@$(MAKE) -f oamcl.mak $(SY_DIR)/liboamcl.a COPTS=$(LNXCPPOPTS) \
	LOPTS='$(LNXLOPTS)' LOPTSCPP='$(LNXLOPTSCPP)' POPTS='$(CCLNXWROPTS)' SMPOPTS='$(CCLNXSMOPTS)' \
	HIOPTS='$(HIOPTS)' SBOPTS='$(SBOPTS)' IOPTS='$(LNXIOPTS)'  IN_DIR='$(OAMCL_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC1='cpp' SRC='c' CC='$(LNX_GPP_CC) -c' LL='$(LNX_GPP_LD)' CM_INC='$(CM_INC)' \
	AR='$(LNX_AR)'
	cp $(SY_DIR)/liboamcl.a ./libs/ 

#Making REM library	
$(BLD_LNX_REM_LIB):
	cd $(REM_DIR)/src; $(MAKE) clean; $(MAKE) PRODUCT=hbs2-iu HARDWARE=hbs2-4 BUILD=arm-v7a-linux-gnueabi

#Making REM objects
$(BLD_LNX_REM_OBJS):
	echo "######Compiling REM ######"
	cd $(REMCL_DIR);rm -f *.o;rm -f libremcl.a;
	cd $(REM_DIR);$(MAKE) PRODUCT=hbs2-iu HARDWARE=hbs2-4 BUILD=arm-v7a-linux-gnueabi;


#
#Making LTE RRC Layer
#

$(BLD_LNX_NH_PREPROC):
	$(MAKE) -f nh.mak $(SY_DIR)/nhprepro COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXNHOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(NH_DIR)'  IN_DIR='$(NH_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='i' SRC="c" CC='$(CC) -E' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_NH_OBJS):
	$(MAKE) -f nh.mak $(SY_DIR)/libnh.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXNHOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(NH_DIR)' IN_DIR='$(SY_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC='i' CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_NH_OBJS1):
	$(MAKE) -f nh.mak $(SY_DIR)/libnh.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXNHOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(NH_DIR)' IN_DIR='$(NH_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="c" CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'

#
#Making RLC Layer
#
$(BLD_LNX_KW_PREPROC):
	@$(MAKE) -f kw_split.mak $(SY_DIR)/kwprepro COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXKWOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(KW_DIR)' IN_DIR='$(KW_DIR)' \
	OUT_DIR='$(SY_DIR)' SRC='c' OBJ='i' CC='$(CC) -E' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_KW_OBJS):
	@$(MAKE) -f kw_split.mak $(SY_DIR)/libkw.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXKWOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(KW_DIR)' IN_DIR='$(SY_DIR)' \
	OUT_DIR='$(SY_DIR)' SRC='i' OBJ='$(OBJ)' CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_KW_OBJS1):
	@$(MAKE) -f kw_split.mak $(SY_DIR)/libkw.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXKWOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(KW_DIR)' IN_DIR='$(KW_DIR)' \
	OUT_DIR='$(SY_DIR)' SRC='c' OBJ='$(OBJ)' CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'
#
#Making PDCP Layer
#

$(BLD_LNX_PJ_PREPROC):
	@$(MAKE) -f pj.mak $(SY_DIR)/pjprepro COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXPJOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(PJ_DIR)' IN_DIR='$(PJ_DIR)' \
	OUT_DIR='$(SY_DIR)' SRC='c' OBJ='i' CC='$(CC) -E' LL='$(LL)' CM_INC='$(CM_INC)' SEC_FLAG='$(TENB_AS_SECURITY)'

$(BLD_LNX_PJ_OBJS):
	@$(MAKE) -f pj.mak $(SY_DIR)/libpj.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXPJOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(PJ_DIR)' IN_DIR='$(SY_DIR)' \
	OUT_DIR='$(SY_DIR)' SRC='i' OBJ='$(OBJ)' CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)' SEC_FLAG='$(TENB_AS_SECURITY)'

$(BLD_LNX_PJ_OBJS1):
	@$(MAKE) -f pj.mak $(SY_DIR)/libpj.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXPJOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(PJ_DIR)' IN_DIR='$(PJ_DIR)' \
	OUT_DIR='$(SY_DIR)' SRC='c' OBJ='$(OBJ)' CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)' SEC_FLAG='$(TENB_AS_SECURITY)'
#
#Making MAC Layer
#
$(BLD_LNX_RG_PREPROC):
	#Added copying rg_env.h here because this file is diferent for Integration compilation 
	#and acceptance compilation
	cp -f $(RG_DIR)/rg_env_t2k.h $(RG_DIR)/rg_env.h
	@$(MAKE) -f rg.mak $(SY_DIR)/rgprepro COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXRGOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(RG_DIR)' IN_DIR='$(RG_DIR)' \
	OUT_DIR='$(SY_DIR)' SRC='c' OBJ='i' CC='$(CC) -E' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_RG_OBJS):
	#Added copying rg_env.h here because this file is diferent for Integration compilation 
	#and acceptance compilation
	cp -f $(RG_DIR)/rg_env_t2k.h $(RG_DIR)/rg_env.h
	@$(MAKE) -f rg.mak $(SY_DIR)/librg.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXRGOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(RG_DIR)' IN_DIR='$(SY_DIR)' \
	OUT_DIR='$(SY_DIR)' SRC='i' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_RG_OBJS1):
	#Added copying rg_env.h here because this file is diferent for Integration compilation 
	#and acceptance compilation
	cp -f $(RG_DIR)/rg_env_t2k.h $(RG_DIR)/rg_env.h
	$(MAKE) -f rg.mak $(SY_DIR)/librg.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LNXLOPTS)' POPTS='$(CCLNXRGOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(RG_DIR)' IN_DIR='$(RG_DIR)' \
	OUT_DIR='$(SY_DIR)' SRC='c' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)' CM_INC='$(CM_INC)'

#
#Making CL Layer
#

ifeq ($(VENDOR),RSYS)
$(BLD_LNX_OAM_LIB):
	cd $(OAM_LIB_MAKE_PATH); $(MAKE) distro PRODUCT=hbs2-iu HARDWARE=hbs2-4 BUILD=arm-v7a-linux-gnueabi;
else
$(BLD_LNX_OAM_LIB):
	cd $(OAM_LIB_MAKE_PATH); $(MAKE)
endif
#
#
#Making S1AP Layer
#
$(BLD_LNX_SZ_PREPROC):
	@$(MAKE) -f sz.mak $(SY_DIR)/szprepro COPTS=$(LNXCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXSZOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(SZ_DIR)' IN_DIR='$(SZ_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='i' SRC="c" CC='$(CC) -E' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_SZ_OBJS):
	@$(MAKE) -f sz.mak $(SY_DIR)/libsz.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXSZOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(SZ_DIR)'  IN_DIR='$(SY_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC='i' CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_SZ_OBJS1):
	@$(MAKE) -f sz.mak $(SY_DIR)/libsz.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXSZOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(SZ_DIR)'  IN_DIR='$(SZ_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="c" CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'

#
#
#Making X2AP Layer
#
$(BLD_LNX_CZ_PREPROC):
	@$(MAKE) -f cz.mak $(SY_DIR)/czprepro COPTS=$(LNXCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXCZOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(CZ_DIR)'   IN_DIR='$(CZ_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='i' SRC="c" CC='$(CC) -E' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_CZ_OBJS):
	@$(MAKE) -f cz.mak $(SY_DIR)/libcz.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXCZOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(CZ_DIR)' IN_DIR='$(SY_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC='i' CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_CZ_OBJS1):
	@$(MAKE) -f cz.mak $(SY_DIR)/libcz.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXCZOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(CZ_DIR)' IN_DIR='$(CZ_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="c" CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'

#
#Making SCTP Layer
#
$(BLD_LNX_SB_PREPROC):
	@$(MAKE) -f sb.mak $(SY_DIR)/sbprepro COPTS=$(LNXCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXSBOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(SB_DIR)'  IN_DIR='$(SB_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='i' SRC="c" CC='$(CC) -E' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_SB_OBJS):
	@$(MAKE) -f sb.mak $(SY_DIR)/libsb.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXSBOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(SB_DIR)'  IN_DIR='$(SY_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC='i' CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_SB_OBJS1):
	@$(MAKE) -f sb.mak $(SY_DIR)/libsb.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXSBOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(SB_DIR)'  IN_DIR='$(SB_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="c" CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'

#
#Making TUCL Layer
#
$(BLD_LNX_HI_PREPROC):
	@$(MAKE) -f hi.mak $(SY_DIR)/hiprepro COPTS=$(LNXCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXHIOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(HI_DIR)' IN_DIR='$(HI_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='i' SRC="c" CC='$(CC) -E' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_HI_OBJS):
	@$(MAKE) -f hi.mak $(SY_DIR)/libhi.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXHIOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(HI_DIR)' IN_DIR='$(SY_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC='i'  CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_HI_OBJS1):
	@$(MAKE) -f hi.mak $(SY_DIR)/libhi.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXHIOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(HI_DIR)' IN_DIR='$(HI_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="c"  CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'

#
#Making eGTP Layer
#
$(BLD_LNX_EG_PREPROC):
	@$(MAKE) -f eg.mak $(SY_DIR)/egprepro COPTS=$(LNXCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXEGOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(EG_DIR)' IN_DIR='$(EG_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='i' SRC="c" CC='$(CC) -E' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_EG_OBJS):
	@echo "VS_DIR"'$(EG_DIR)'
	@$(MAKE) -f eg.mak $(SY_DIR)/libeg.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXEGOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(EG_DIR)' IN_DIR='$(SY_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC='i' CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'

$(BLD_LNX_EG_OBJS1):
	@$(MAKE) -f eg.mak $(SY_DIR)/libeg.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXEGOPTS)' IOPTS='$(LNXIOPTS)' VS_DIR='$(EG_DIR)' IN_DIR='$(EG_DIR)' \
	OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' SRC="c" CC='$(CC) -c' LL='$(LL)' CM_INC='$(CM_INC)'


#
# making Multi Threaded SSI
#
$(BLD_LNX_SS_OBJS):
	@$(MAKE) -f mt.mak $(SY_DIR)/libmt.a COPTS=$(LNXCOPTS) \
	LOPTS='$(LOPTS)' POPTS='$(CCLNXMTOPTS)' IOPTS='$(LNXIOPTS)' IN_DIR='$(MT_DIR)' \
   OUT_DIR='$(SY_DIR)' OBJ='$(OBJ)' CC='$(CC)' LL='$(LL)'  \
   CM_INC='$(CM_INC)'

clean:
	rm -f $(SY_DIR)/$(OUTNAME) $(SY_DIR)/* $(SY_DIR)/*.a $(SY_DIR)/core $(SY_DIR)/*~ \
	$(SY_DIR)/*.bak $(SY_DIR)/*.err $(RRM_LIBS_DIR)/* $(SON_LIBS_DIR)/*

clean_remlib:
	@echo "************* CLEANING REM library **************"
	cd $(REM_DIR)/src; $(MAKE) clean

clean_remapp:
	@echo "************* CLEANING REM app     **************"
	cd $(REMCL_DIR);rm -f *.o;rm -f libremcl.a;
	cd $(REM_DIR);$(MAKE) PRODUCT=hbs2-iu HARDWARE=hbs2-4 BUILD=arm-v7a-linux-gnueabi clean;

clean_oam:
	@echo "************* CLEANING OAM    **************"
	cd $(OAM_LIB_MAKE_PATH); $(MAKE) clean PRODUCT=hbs2-iu HARDWARE=hbs2-4 BUILD=arm-v7a-linux-gnueabi;

# DO NOT DELETE THIS LINE -- make depend depends on it.

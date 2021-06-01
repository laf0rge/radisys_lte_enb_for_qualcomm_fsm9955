#-------------------------------------------------------------#
#Makefile for product PJ - script generated.
#-------------------------------------------------------------#


# Product objects-------------------------------------------------------
C_OBJS= \
    $(OUT_DIR)/pj_cfg_dl.$(OBJ) \
    $(OUT_DIR)/pj_cfg_ul.$(OBJ) \
    $(OUT_DIR)/pj_udx.$(OBJ) \
    $(OUT_DIR)/pj_udx_ptdl.$(OBJ) \
    $(OUT_DIR)/pj_udx_ptul.$(OBJ) \
    $(OUT_DIR)/pj_udx_ul.$(OBJ) \
    $(OUT_DIR)/pj_udx_dl.$(OBJ) \
    $(OUT_DIR)/pj_dlm.$(OBJ) \
    $(OUT_DIR)/pj_ulm.$(OBJ) \
    $(OUT_DIR)/pj_utl.$(OBJ) \
    $(OUT_DIR)/pj_ul_utl.$(OBJ) \
    $(OUT_DIR)/pj_dl_utl.$(OBJ) \
    $(OUT_DIR)/pj_dbm.$(OBJ) \
    $(OUT_DIR)/pj_dbm_ul.$(OBJ) \
    $(OUT_DIR)/pj_dbm_dl.$(OBJ) \
    $(OUT_DIR)/pj_lim.$(OBJ) \
    $(OUT_DIR)/pj_uim.$(OBJ) \
    $(OUT_DIR)/pj_lmm.$(OBJ) \
    $(OUT_DIR)/pj_tmr.$(OBJ) \
    $(OUT_DIR)/pj_ul_ex_ms.$(OBJ) \
    $(OUT_DIR)/pj_dl_ex_ms.$(OBJ) \
    $(OUT_DIR)/pj_lib.$(OBJ) \
    $(OUT_DIR)/pj_ptmi.$(OBJ) \
    $(OUT_DIR)/pj_ptli.$(OBJ) \
    $(OUT_DIR)/pj_ptui.$(OBJ) \
    $(OUT_DIR)/pj_id.$(OBJ) \
    $(OUT_DIR)/pj_tenb_stats.$(OBJ) \
    $(OUT_DIR)/pj_ptlib.$(OBJ) 

ifeq ($(SEC_FLAG),-DTENB_AS_SECURITY)
CPP_OBJS= \
    $(OUT_DIR)/pj_qcom_ptsec.$(OBJ)
endif


#-----------------------------------------------------------------------
# CAN BE MODIFIED
# This section can be used to pass specific options for the layer
# Normally this section should not be used unless absolutely necessary

ROOT=$(BUILD_ROOT)/../../../
include $(BUILD_ROOT)/../../../targets/fsm99xxCDP_1/make.target.vars

#QCOM_SPECIFIC_DEFS= -DCPU_SPEED=$(CPU_SPEED) 
QCOM_SPECIFIC_DEFS=  \
                    -DCPU_THREADS=$(CPU_THREADS) \
                    -Dfsm99xxCDP_1\
-D_FSM99XX_HEXAGON2_

CCOPTS=$(COPTS) $(POPTS) $(IOPTS) -ULCUDX -ULWLCUDX
CPPOPTS=$(COPTS) $(POPTS) $(IOPTS) $(QCOM_SPECIFIC_DEFS)

#-----------------------------------------------------------------------
# CAN BE MODIFIED
# This section can be used to pass specific options for the layer
# Normally this section should not be used unless absolutely necessary


#-----------------------------------------------------------------------
# DO NOT EDIT THIS SECTION
# include for the building rules
#
ifeq ($(BLD_ENV),HEX)
include $(BUILD_ROOT)/build_rule.mak
else 
ifeq ($(BLD_ENV),PRE)
include $(BUILD_ROOT)/build_rule_pre.mak
else 
ifeq ($(BLD_ENV),POST)
include $(BUILD_ROOT)/build_rule_post.mak
else
include $(BUILD_ROOT)/build_rule.mak
endif
endif
endif

# DO NOT DELETE THIS LINE -- make depend depends on it.


#-------------------------------------------------------------#
#Makefile for product KW - script generated.
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

# Product objects-------------------------------------------------------
C_OBJS= \
	$(OUT_DIR)/kw_lmm.$(OBJ)      \
	$(OUT_DIR)/kw_cfg_dl.$(OBJ)      \
	$(OUT_DIR)/kw_cfg_ul.$(OBJ)      \
	$(OUT_DIR)/kw_dbm_dl.$(OBJ)      \
	$(OUT_DIR)/kw_dbm_ul.$(OBJ)      \
	$(OUT_DIR)/kw_lim.$(OBJ)      \
	$(OUT_DIR)/kw_tmm_dl.$(OBJ)      \
	$(OUT_DIR)/kw_tmm_ul.$(OBJ)      \
	$(OUT_DIR)/kw_utl_dl.$(OBJ)      \
	$(OUT_DIR)/kw_utl_ul.$(OBJ)      \
	$(OUT_DIR)/kw_umm_dl.$(OBJ)      \
	$(OUT_DIR)/kw_umm_ul.$(OBJ)      \
	$(OUT_DIR)/kw_uim.$(OBJ)      \
	$(OUT_DIR)/kw_ptmi.$(OBJ)     \
	$(OUT_DIR)/kw_ptui.$(OBJ)     \
	$(OUT_DIR)/kw_dl_ex_ms.$(OBJ)    \
	$(OUT_DIR)/kw_ul_ex_ms.$(OBJ)    \
	$(OUT_DIR)/kw_tmr.$(OBJ)      \
	$(OUT_DIR)/kw_amm_ul.$(OBJ)  \
	$(OUT_DIR)/kw_amm_dl.$(OBJ)  \
	$(OUT_DIR)/kw_udx.$(OBJ)    \
	$(OUT_DIR)/kw_udx_ul.$(OBJ)    \
	$(OUT_DIR)/kw_udx_dl.$(OBJ)    \
	$(OUT_DIR)/kw_udx_ptul.$(OBJ) \
	$(OUT_DIR)/kw_udx_ptdl.$(OBJ) \
	$(OUT_DIR)/kw_ptli.$(OBJ)  \
	$(OUT_DIR)/kw_id.$(OBJ) 

#-----------------------------------------------------------------------
# CAN BE MODIFIED
# This section can be used to pass specific options for the layer
# Normally this section should not be used unless absolutely necessary

CCOPTS=$(COPTS) $(POPTS) $(IOPTS) -DLCKWUIKWU -DLCKWU -DLCKWULUDX -DLCUDX -DLWLCKWULUDX -DLWLCKWUIKWU
CPPOPTS=$(COPTS) $(POPTS) $(IOPTS)

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

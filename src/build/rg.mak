#-------------------------------------------------------------#
#Makefile for product RG - script generated.
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#
# Including RG_PHASE2_SCHED for supporting more than one schedulers 
# supported by mac
# Product objects-------------------------------------------------------
C_OBJS= \
    $(OUT_DIR)/rg_cfg.$(OBJ) \
    $(OUT_DIR)/rg_com.$(OBJ) \
    $(OUT_DIR)/rg_dbm.$(OBJ) \
    $(OUT_DIR)/rg_dhm.$(OBJ) \
    $(OUT_DIR)/rg_dux.$(OBJ) \
    $(OUT_DIR)/rg_ex_ms.$(OBJ) \
    $(OUT_DIR)/rg_id.$(OBJ) \
    $(OUT_DIR)/rg_lim.$(OBJ) \
    $(OUT_DIR)/rg_lmm.$(OBJ) \
    $(OUT_DIR)/rg_mux.$(OBJ) \
    $(OUT_DIR)/rg_ptli.$(OBJ) \
    $(OUT_DIR)/rg_ptmi.$(OBJ) \
    $(OUT_DIR)/rg_ptui.$(OBJ) \
    $(OUT_DIR)/rg_ram.$(OBJ) \
    $(OUT_DIR)/rg_rom.$(OBJ) \
    $(OUT_DIR)/rg_l2m.$(OBJ) \
    $(OUT_DIR)/rg_sch.$(OBJ) \
    $(OUT_DIR)/rg_sch_l2m.$(OBJ) \
    $(OUT_DIR)/rg_tom.$(OBJ) \
    $(OUT_DIR)/rg_uhm.$(OBJ) \
    $(OUT_DIR)/rg_uim.$(OBJ) \
    $(OUT_DIR)/rg_utl.$(OBJ)  \
    $(OUT_DIR)/rg_sch_cfg.$(OBJ) \
    $(OUT_DIR)/rg_sch_cmn.$(OBJ) \
    $(OUT_DIR)/rg_sch_dbm.$(OBJ) \
    $(OUT_DIR)/rg_sch_gom.$(OBJ)\
    $(OUT_DIR)/rg_sch_dhm.$(OBJ)\
    $(OUT_DIR)/rg_sch_dlfs.$(OBJ) \
    $(OUT_DIR)/rg_sch_ex_ms.$(OBJ) \
    $(OUT_DIR)/rg_sch_inf.$(OBJ) \
    $(OUT_DIR)/rg_sch_lmm.$(OBJ) \
    $(OUT_DIR)/rg_sch_mga.$(OBJ) \
    $(OUT_DIR)/rg_sch_pt.$(OBJ) \
    $(OUT_DIR)/rg_sch_pwr.$(OBJ) \
    $(OUT_DIR)/rg_sch_ram.$(OBJ) \
    $(OUT_DIR)/rg_sch_rr.$(OBJ) \
    $(OUT_DIR)/rg_sch_tmr.$(OBJ) \
    $(OUT_DIR)/rg_sch_uhm.$(OBJ) \
    $(OUT_DIR)/rg_sch_utl.$(OBJ) \
    $(OUT_DIR)/rg_sch_tom.$(OBJ) \
    $(OUT_DIR)/rg_sch_utl_clist.$(OBJ) \
    $(OUT_DIR)/rg_sch_drx.$(OBJ) \
    $(OUT_DIR)/rg_sch_sps.$(OBJ)



#-----------------------------------------------------------------------
# CAN BE MODIFIED
# This section can be used to pass specific options for the layer
# Normally this section should not be used unless absolutely necessary

CCOPTS=$(COPTS) $(POPTS) $(IOPTS)
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

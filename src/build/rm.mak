#-------------------------------------------------------------#
#Makefile for RRM
#-------------------------------------------------------------#
include ./rsys_fancy.mak
COLOR=$(COLOR_BLUE)
#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#
CCrmFLAGS=-DRGM_LC -DRGM_LWLC -DLRM_LWLC -DRMU_LWLC -URMU_NPLC -DLWLCSMRMMILRM -DLCSMRMMILRM -DSM -DSS_MT_TMR -DRM_INTF

# Product sources-------------------------------------------------------
rmSRCS= \
    $(IN_DIR)/rm_application.$(SRC1)          \
    $(IN_DIR)/rm_admissioncontroller.$(SRC1)  \
    $(IN_DIR)/rm_acb.$(SRC1)  \
    $(IN_DIR)/rm_uecontext.$(SRC1)            \
    $(IN_DIR)/rm_uemanager.$(SRC1)          \
    $(IN_DIR)/rm_radiobearercontroller.$(SRC1) \
    $(IN_DIR)/rm_cellmanager.$(SRC1)           \
    $(IN_DIR)/rm_resourceconfigmanager.$(SRC1)				\
    $(IN_DIR)/rm_schedulingrequestmgr.$(SRC1)				\
	 $(IN_DIR)/rm_rcmpcqi.$(SRC1) 					\
	 $(IN_DIR)/rm_outinterface.$(SRC1)			\
    $(IN_DIR)/rm_rcmdrx.$(SRC1)           \
    $(IN_DIR)/rm_ptui.$(SRC)					\
    $(IN_DIR)/rm_ptmi.$(SRC)					\
    $(IN_DIR)/rm_ptli.$(SRC)					\
    $(IN_DIR)/rm_lmm.$(SRC)					\
    $(IN_DIR)/rm_mmemanager.$(SRC)					\
    $(IN_DIR)/rm_utils.$(SRC1)					\
    $(IN_DIR)/rm_ex_ms.$(SRC)            \
    $(IN_DIR)/rm_rcmsps.$(SRC1)           \
    $(IN_DIR)/rm_prbbasedadmissioncontroller.$(SRC1)     \
    $(IN_DIR)/rm_prbreportcollector.$(SRC1)     \
    $(IN_DIR)/rm_qcibasedadmissioncontroller.$(SRC1)     \
    $(IN_DIR)/rm_preEmptableERABFinder.$(SRC1)     \
    $(IN_DIR)/rm_rbadmissioncontroller.$(SRC1)     \
    $(IN_DIR)/rm_prbcongestioncontroller.$(SRC1)


# Product objects-------------------------------------------------------
rmOBJS= \
    $(OUT_DIR)/rm_application.$(OBJ)           \
    $(OUT_DIR)/rm_acb.$(OBJ)   \
    $(OUT_DIR)/rm_admissioncontroller.$(OBJ)   \
    $(OUT_DIR)/rm_uecontext.$(OBJ)             \
    $(OUT_DIR)/rm_uemanager.$(OBJ)          \
    $(OUT_DIR)/rm_radiobearercontroller.$(OBJ) \
    $(OUT_DIR)/rm_cellmanager.$(OBJ)	         \
    $(OUT_DIR)/rm_resourceconfigmanager.$(OBJ)					\
    $(OUT_DIR)/rm_schedulingrequestmgr.$(OBJ)					\
    $(OUT_DIR)/rm_rcmpcqi.$(OBJ)						\
    $(OUT_DIR)/rm_outinterface.$(OBJ)				\
    $(OUT_DIR)/rm_rcmdrx.$(OBJ)                 \
    $(OUT_DIR)/rm_ptui.$(OBJ)							\
    $(OUT_DIR)/rm_ptmi.$(OBJ)							\
    $(OUT_DIR)/rm_ptli.$(OBJ)							\
    $(OUT_DIR)/rm_lmm.$(OBJ)							\
    $(OUT_DIR)/rm_mmemanager.$(OBJ)							\
    $(OUT_DIR)/rm_utils.$(OBJ)							\
    $(OUT_DIR)/rm_ex_ms.$(OBJ)                 \
    $(OUT_DIR)/rm_rcmsps.$(OBJ)                \
    $(OUT_DIR)/rm_prbbasedadmissioncontroller.$(OBJ)     \
    $(OUT_DIR)/rm_prbcongestioncontroller.$(OBJ)     \
    $(OUT_DIR)/rm_prbreportcollector.$(OBJ)     \
    $(OUT_DIR)/rm_qcibasedadmissioncontroller.$(OBJ)     \
    $(OUT_DIR)/rm_preEmptableERABFinder.$(OBJ)     \
    $(OUT_DIR)/rm_rbadmissioncontroller.$(OBJ)

RM_INC=$(VS_DIR)/rm.h  $(VS_DIR)/rm_admissioncontroller.h \
		 	$(VS_DIR)/rm_application.h $(VS_DIR)/rm_cellmanager.h \
		 	$(VS_DIR)/rm_uecontext.h $(VS_DIR)/rm_radiobearercontroller.h \
		 	$(VS_DIR)/rm_uemanager.h $(VS_DIR)/rm_resourceconfigmanager.h \
		 	$(VS_DIR)/rm_acb.h $(VS_DIR)/rm_cmn.h  $(VS_DIR)/rm_rcmpcqi.h \
		 	$(VS_DIR)/rm_outinterface.h  $(VS_DIR)/rm_utils.h  \
		 	$(VS_DIR)/rm_mmemanager.h $(VS_DIR)/rm_rcmdrx.h \
		 	$(VS_DIR)/rm_schedulingrequestmgr.h $(VS_DIR)/rm_rcmsps.h \
		 	$(VS_DIR)/rm_prbbasedadmissioncontroller.h \
		 	$(VS_DIR)/rm_prbcongestioncontroller.h \
		 	$(VS_DIR)/rm_prbreportcollector.h \
		 	$(VS_DIR)/rm_qcibasedadmissioncontroller.h \
		 	$(VS_DIR)/rm_preEmptableERABFinder.h \
		 	$(VS_DIR)/rm_rbadmissioncontroller.h 

ALL_INC=$(RM_INC) $(CM_INC)
   
#-------------------------------------------------------------#
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#

$(OUT_DIR)/rm_application.$(OBJ): $(IN_DIR)/rm_application.$(SRC1) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_application.$(SRC1) -o \
	$(OUT_DIR)/rm_application.$(OBJ)
$(OUT_DIR)/rm_admissioncontroller.$(OBJ): $(IN_DIR)/rm_admissioncontroller.$(SRC1) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_admissioncontroller.$(SRC1) -o \
	$(OUT_DIR)/rm_admissioncontroller.$(OBJ)
$(OUT_DIR)/rm_uecontext.$(OBJ): $(IN_DIR)/rm_uecontext.$(SRC1) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_uecontext.$(SRC1) -o \
	$(OUT_DIR)/rm_uecontext.$(OBJ)
$(OUT_DIR)/rm_uemanager.$(OBJ): $(IN_DIR)/rm_uemanager.$(SRC1) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_uemanager.$(SRC1) -o \
	$(OUT_DIR)/rm_uemanager.$(OBJ)
$(OUT_DIR)/rm_radiobearercontroller.$(OBJ): $(IN_DIR)/rm_radiobearercontroller.$(SRC1) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_radiobearercontroller.$(SRC1) -o \
	$(OUT_DIR)/rm_radiobearercontroller.$(OBJ)
$(OUT_DIR)/rm_cellmanager.$(OBJ): $(IN_DIR)/rm_cellmanager.$(SRC1) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_cellmanager.$(SRC1) -o \
	$(OUT_DIR)/rm_cellmanager.$(OBJ)
$(OUT_DIR)/rm_acb.$(OBJ): $(IN_DIR)/rm_acb.$(SRC1) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_acb.$(SRC1) -o \
	$(OUT_DIR)/rm_acb.$(OBJ)
$(OUT_DIR)/rm_resourceconfigmanager.$(OBJ): $(IN_DIR)/rm_resourceconfigmanager.$(SRC1) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_resourceconfigmanager.$(SRC1) -o \
	$(OUT_DIR)/rm_resourceconfigmanager.$(OBJ)
$(OUT_DIR)/rm_schedulingrequestmgr.$(OBJ): $(IN_DIR)/rm_schedulingrequestmgr.$(SRC1) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_schedulingrequestmgr.$(SRC1) -o \
	$(OUT_DIR)/rm_schedulingrequestmgr.$(OBJ)
$(OUT_DIR)/rm_rcmpcqi.$(OBJ): $(IN_DIR)/rm_rcmpcqi.$(SRC1) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_rcmpcqi.$(SRC1) -o \
	$(OUT_DIR)/rm_rcmpcqi.$(OBJ)
$(OUT_DIR)/rm_rcmdrx.$(OBJ): $(IN_DIR)/rm_rcmdrx.$(SRC1) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_rcmdrx.$(SRC1) -o \
	$(OUT_DIR)/rm_rcmdrx.$(OBJ)
$(OUT_DIR)/rm_outinterface.$(OBJ): $(IN_DIR)/rm_outinterface.$(SRC1) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_outinterface.$(SRC1) -o \
	$(OUT_DIR)/rm_outinterface.$(OBJ)
$(OUT_DIR)/rm_utils.$(OBJ): $(IN_DIR)/rm_utils.$(SRC1) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_utils.$(SRC1) -o \
	$(OUT_DIR)/rm_utils.$(OBJ)
$(OUT_DIR)/rm_mmemanager.$(OBJ): $(IN_DIR)/rm_mmemanager.$(SRC1) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_mmemanager.$(SRC1) -o \
	$(OUT_DIR)/rm_mmemanager.$(OBJ)
$(OUT_DIR)/rm_ptui.$(OBJ): $(IN_DIR)/rm_ptui.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_ptui.$(SRC) -o \
	$(OUT_DIR)/rm_ptui.$(OBJ)
$(OUT_DIR)/rm_ptmi.$(OBJ): $(IN_DIR)/rm_ptmi.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_ptmi.$(SRC) -o \
	$(OUT_DIR)/rm_ptmi.$(OBJ)
$(OUT_DIR)/rm_ptli.$(OBJ): $(IN_DIR)/rm_ptli.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_ptli.$(SRC) -o \
	$(OUT_DIR)/rm_ptli.$(OBJ)
$(OUT_DIR)/rm_lmm.$(OBJ): $(IN_DIR)/rm_lmm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_lmm.$(SRC) -o \
	$(OUT_DIR)/rm_lmm.$(OBJ)
$(OUT_DIR)/rm_ex_ms.$(OBJ): $(IN_DIR)/rm_ex_ms.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_ex_ms.$(SRC) -o \
	$(OUT_DIR)/rm_ex_ms.$(OBJ)
$(OUT_DIR)/rm_rcmsps.$(OBJ): $(IN_DIR)/rm_rcmsps.$(SRC1) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_rcmsps.$(SRC1) -o \
	$(OUT_DIR)/rm_rcmsps.$(OBJ)
$(OUT_DIR)/rm_prbbasedadmissioncontroller.$(OBJ): $(IN_DIR)/rm_prbbasedadmissioncontroller.$(SRC1) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_prbbasedadmissioncontroller.$(SRC1) -o \
	$(OUT_DIR)/rm_prbbasedadmissioncontroller.$(OBJ)
$(OUT_DIR)/rm_prbreportcollector.$(OBJ): $(IN_DIR)/rm_prbreportcollector.$(SRC1) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_prbreportcollector.$(SRC1) -o \
	$(OUT_DIR)/rm_prbreportcollector.$(OBJ)
$(OUT_DIR)/rm_qcibasedadmissioncontroller.$(OBJ): $(IN_DIR)/rm_qcibasedadmissioncontroller.$(SRC1) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_qcibasedadmissioncontroller.$(SRC1) -o \
	$(OUT_DIR)/rm_qcibasedadmissioncontroller.$(OBJ)
$(OUT_DIR)/rm_preEmptableERABFinder.$(OBJ): $(IN_DIR)/rm_preEmptableERABFinder.$(SRC1) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_preEmptableERABFinder.$(SRC1) -o \
	$(OUT_DIR)/rm_preEmptableERABFinder.$(OBJ)
$(OUT_DIR)/rm_rbadmissioncontroller.$(OBJ): $(IN_DIR)/rm_rbadmissioncontroller.$(SRC1) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_rbadmissioncontroller.$(SRC1) -o \
	$(OUT_DIR)/rm_rbadmissioncontroller.$(OBJ)
$(OUT_DIR)/rm_prbcongestioncontroller.$(OBJ): $(IN_DIR)/rm_prbcongestioncontroller.$(SRC1) $(ALL_INC)
	$(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCrmFLAGS) $(IN_DIR)/rm_prbcongestioncontroller.$(SRC1) -o \
	$(OUT_DIR)/rm_prbcongestioncontroller.$(OBJ)

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(OUT_DIR)/librm.a:$(rmOBJS) 
	@echo -e "Creating Archive $(COLOR) $@ $(REVERT_COLOR)"
	$(AR) -cr $(OUT_DIR)/librm.a $(rmOBJS)

$(OUT_DIR)/rmprepro:$(rmOBJS)

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f $(OUT_DIR)/librm.a $(rmOBJS)


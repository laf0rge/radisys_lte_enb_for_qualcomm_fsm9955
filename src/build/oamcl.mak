#-------------------------------------------------------------#
#Makefile for OAM-CL
#-------------------------------------------------------------#
include ./rsys_fancy.mak
COLOR=$(COLOR_BLUE)
#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

#Qualcomm OAM integration options
QCOMOPTS=-DQCT_DYNAMIC_CAST=dynamic_cast -Wno-trigraphs -Wno-unused-but-set-variable -Wno-maybe-uninitialized -Wno-array-bounds -Wno-unused-result -DQCT_INLINE=__inline__ -DQCT_MAYBE_UNUSED= -Wpointer-arith -Wno-unused-but-set-variable  -MD -MP -D__packed__= -DIMAGE_APPS_PROC -DFEATURE_Q_SINGLE_LINK -DFEATURE_Q_NO_SELF_QPTR -DFEATURE_LINUX -DFEATURE_NATIVELINUX -DFEATURE_DSM_DUP_ITEMS -DTARGET_fsm9900_cdp -DSUBTYPE_fapi -DOAM_TEST -DUMTS_ENABLED -DOAM_APPS_BIND_IPV4_AND_IPV6
CCoamclFLAGS+=$(QCOMOPTS)

QCOMOAMCL_IOPTS += -I$(QCOM_OAM_SRC_DIR)/libs/include
QCOMOAMCL_IOPTS += -I$(QCOM_OAM_SRC_DIR)/libs/oamProtocol/include
QCOMOAMCL_IOPTS += -I$(QCOM_OAM_SRC_DIR)/libs/oamClientProtocol/include
QCOMOAMCL_IOPTS += -I$(QCOM_OAM_SRC_DIR)/libs/mgmtXml/include
QCOMOAMCL_IOPTS += -I$(QCOM_OAM_SRC_DIR)/apps/include
QCOMOAMCL_IOPTS += -I$(QCOM_SYSROOT)/usr/include/libxml2
QCOMOAMCL_IOPTS += -I$(QCOM_SYSROOT)/include/oam/
QCOMOAMCL_IOPTS += -I$(QCOM_SYSROOT)/usr/include/
QCOMOAMCL_IOPTS += -I$(QCOM_SYSROOT)/include/

# Product objects-------------------------------------------------------
oamclOBJS= \
    $(OUT_DIR)/oamClientInit.$(OBJ)           \
    $(OUT_DIR)/oamClientProtoWrapper.$(OBJ)           \
    $(OUT_DIR)/oamClientTrx.$(OBJ)

#RM_INC=$(VS_DIR)/rm.h  $(VS_DIR)/rm_admissioncontroller.h \
		 	$(VS_DIR)/rm_rbadmissioncontroller.h 

#ALL_INC=$(RM_INC) $(CM_INC)
   
#-------------------------------------------------------------#
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#

$(OUT_DIR)/oamClientInit.$(OBJ): $(IN_DIR)/oamClientInit.$(SRC1) $(ALL_INC)
	@echo -e "  QCOM OPTS $(QCOMOAMCL_IOPTS)"
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(QCOMOAMCL_IOPTS) $(POPTS) $(CCoamclFLAGS) $(IN_DIR)/oamClientInit.$(SRC1) -o \
	$(OUT_DIR)/oamClientInit.$(OBJ)

$(OUT_DIR)/oamClientProtoWrapper.$(OBJ): $(IN_DIR)/oamClientProtoWrapper.$(SRC1) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(QCOMOAMCL_IOPTS) $(POPTS) $(CCoamclFLAGS) $(IN_DIR)/oamClientProtoWrapper.$(SRC1) -o \
	$(OUT_DIR)/oamClientProtoWrapper.$(OBJ)

$(OUT_DIR)/oamClientTrx.$(OBJ): $(IN_DIR)/oamClientTrx.$(SRC1) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(QCOMOAMCL_IOPTS) $(POPTS) $(CCoamclFLAGS) $(IN_DIR)/oamClientTrx.$(SRC1) -o \
	$(OUT_DIR)/oamClientTrx.$(OBJ)


#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(OUT_DIR)/liboamcl.a:$(oamclOBJS) 
	@echo -e "Creating Archive $(COLOR) $@ $(REVERT_COLOR)"
	$(AR) -cr $(OUT_DIR)/liboamcl.a $(oamclOBJS)

#$(OUT_DIR)/rmprepro:$(rmOBJS)

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f $(OUT_DIR)/liboamcl.a $(oamclOBJS)


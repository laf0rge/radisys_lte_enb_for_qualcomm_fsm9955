#-------------------------------------------------------------#
#Makefile for SON without BRDCM-SON
#-------------------------------------------------------------#
include ./rsys_fancy.mak
COLOR=$(COLOR_YELLOW)

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#
CCnlFLAGS= -DSS_MT_TMR -DLWLCSMSONILNL -DNLU_LWLC -DLCSMSONILNL -DNLU_TC -DTCSMSONILNL

# Product sources-------------------------------------------------------
nlSRCS= \
    $(IN_DIR)/nl_init.$(SRC) \
    $(IN_DIR)/nl_ptui.$(SRC) \
    $(IN_DIR)/nl_ptmi.$(SRC) \
    $(IN_DIR)/nl_lmm.$(SRC) \
    $(IN_DIR)/nl_ex_ms.$(SRC)\
    $(IN_DIR)/nl_pci_sel.$(SRC)\
    $(IN_DIR)/nl_events.$(SRC)\
    $(IN_DIR)/nl_alarm.$(SRC)\
    $(IN_DIR)/nl_earfcn_sel.$(SRC)\
    $(IN_DIR)/nl_prach_sel.$(SRC)\
    $(IN_DIR)/nl_tmr.$(SRC)\
    $(IN_DIR)/nl_pt_son.$(SRC)\
    $(IN_DIR)/nl_txpower_sel.$(SRC)

# Product objects-------------------------------------------------------
nlOBJS= \
    $(OUT_DIR)/nl_init.$(OBJ) \
    $(OUT_DIR)/nl_ptui.$(OBJ) \
    $(OUT_DIR)/nl_ptmi.$(OBJ) \
    $(OUT_DIR)/nl_lmm.$(OBJ)  \
    $(OUT_DIR)/nl_ex_ms.$(OBJ) \
    $(OUT_DIR)/nl_pci_sel.$(OBJ) \
    $(OUT_DIR)/nl_earfcn_sel.$(OBJ) \
    $(OUT_DIR)/nl_events.$(OBJ) \
    $(OUT_DIR)/nl_alarm.$(OBJ) \
    $(OUT_DIR)/nl_prach_sel.$(OBJ)\
    $(OUT_DIR)/nl_tmr.$(OBJ)\
    $(OUT_DIR)/nl_pt_son.$(OBJ)\
    $(OUT_DIR)/nl_txpower_sel.$(OBJ)

NL_INC=$(VS_DIR)/nl.h $(VS_DIR)/nl.x
ALL_INC=$(NL_INC) $(CM_INC)
   
#-------------------------------------------------------------#
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#

$(OUT_DIR)/nl_init.$(OBJ): $(IN_DIR)/nl_init.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCnlFLAGS) $(IN_DIR)/nl_init.$(SRC) -o \
	$(OUT_DIR)/nl_init.$(OBJ)
$(OUT_DIR)/nl_ptui.$(OBJ): $(IN_DIR)/nl_ptui.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCnlFLAGS) $(IN_DIR)/nl_ptui.$(SRC) -o \
	$(OUT_DIR)/nl_ptui.$(OBJ)
$(OUT_DIR)/nl_ptmi.$(OBJ): $(IN_DIR)/nl_ptmi.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCnlFLAGS) $(IN_DIR)/nl_ptmi.$(SRC) -o \
	$(OUT_DIR)/nl_ptmi.$(OBJ)
$(OUT_DIR)/nl_lmm.$(OBJ): $(IN_DIR)/nl_lmm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCnlFLAGS) $(IN_DIR)/nl_lmm.$(SRC) -o \
	$(OUT_DIR)/nl_lmm.$(OBJ)
$(OUT_DIR)/nl_ex_ms.$(OBJ): $(IN_DIR)/nl_ex_ms.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCnlFLAGS) $(IN_DIR)/nl_ex_ms.$(SRC) -o \
	$(OUT_DIR)/nl_ex_ms.$(OBJ)
$(OUT_DIR)/nl_pci_sel.$(OBJ): $(IN_DIR)/nl_pci_sel.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCnlFLAGS) $(IN_DIR)/nl_pci_sel.$(SRC) -o \
	$(OUT_DIR)/nl_pci_sel.$(OBJ)
$(OUT_DIR)/nl_earfcn_sel.$(OBJ): $(IN_DIR)/nl_earfcn_sel.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCnlFLAGS) $(IN_DIR)/nl_earfcn_sel.$(SRC) -o \
	$(OUT_DIR)/nl_earfcn_sel.$(OBJ)
$(OUT_DIR)/nl_events.$(OBJ): $(IN_DIR)/nl_events.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCnlFLAGS) $(IN_DIR)/nl_events.$(SRC) -o \
	$(OUT_DIR)/nl_events.$(OBJ)
$(OUT_DIR)/nl_alarm.$(OBJ): $(IN_DIR)/nl_alarm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCnlFLAGS) $(IN_DIR)/nl_alarm.$(SRC) -o \
	$(OUT_DIR)/nl_alarm.$(OBJ)
$(OUT_DIR)/nl_prach_sel.$(OBJ): $(IN_DIR)/nl_prach_sel.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCnlFLAGS) $(IN_DIR)/nl_prach_sel.$(SRC) -o \
	$(OUT_DIR)/nl_prach_sel.$(OBJ)
$(OUT_DIR)/nl_txpower_sel.$(OBJ): $(IN_DIR)/nl_txpower_sel.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCnlFLAGS) $(IN_DIR)/nl_txpower_sel.$(SRC) -o \
	$(OUT_DIR)/nl_txpower_sel.$(OBJ)
$(OUT_DIR)/nl_pt_son.$(OBJ): $(IN_DIR)/nl_pt_son.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCnlFLAGS) $(IN_DIR)/nl_pt_son.$(SRC) -o \
	$(OUT_DIR)/nl_pt_son.$(OBJ)
$(OUT_DIR)/nl_tmr.$(OBJ): $(IN_DIR)/nl_tmr.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCnlFLAGS) $(IN_DIR)/nl_tmr.$(SRC) -o \
	$(OUT_DIR)/nl_tmr.$(OBJ)
#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(OUT_DIR)/libnl.a:$(nlOBJS)
	@echo -e "Creating Archive $(COLOR) $@ $(REVERT_COLOR)"
	$(AR) -cr $(OUT_DIR)/libnl.a $(nlOBJS)	

$(OUT_DIR)/nlprepro:$(nlOBJS)

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f $(OUT_DIR)/libnl.a $(nlOBJS)


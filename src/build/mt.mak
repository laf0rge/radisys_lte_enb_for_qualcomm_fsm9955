#-------------------------------------------------------------#
#Makefile for product mt - script generated.
#Only the $(CCmtFLAGS) may be modified.
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

CCmtFLAGS=-UBIT_64 -DSS_MT_TMR -USS_LOGGER_SUPPORT -DSS_DRVR_SUPPORT #<---Insert mt specific defines here

#-------------------------------------------------------------#
#File/Obj macros             
#-------------------------------------------------------------#
mtSRCS= \
	$(IN_DIR)/mt_id.c\
	$(IN_DIR)/mt_ss.c\
	$(IN_DIR)/ss_gen.c\
	$(IN_DIR)/ss_id.c\
	$(IN_DIR)/ss_mem.c\
	$(IN_DIR)/ss_msg.c\
	$(IN_DIR)/ss_pack.c\
	$(IN_DIR)/ss_queue.c\
	$(IN_DIR)/ss_strm.c\
	$(IN_DIR)/ss_task.c\
	$(IN_DIR)/ss_drvr.c\
	$(IN_DIR)/ss_timer.c\
	$(IN_DIR)/qu_ipc_fifo_inf.c\
	$(IN_DIR)/qu_utl_circq.c\
	$(IN_DIR)/qu_krait_hex_comm.c
#	$(IN_DIR)/ss_diag.c
#ifeq (${BLDENV},lnx_e2e_ms)
#	mtSRCS+=$(IN_DIR)/mt_4gmx.c
#else
#ifeq (${BLDENV},lnx_ms_withoutdiag)
#	mtSRCS+=$(IN_DIR)/mt_4gmx.c
#endif
#endif

mtOBJS= \
	$(OUT_DIR)/mt_id.$(OBJ)\
	$(OUT_DIR)/mt_ss.$(OBJ)\
	$(OUT_DIR)/ss_gen.$(OBJ)\
	$(OUT_DIR)/ss_id.$(OBJ)\
	$(OUT_DIR)/ss_mem.$(OBJ)\
	$(OUT_DIR)/ss_msg.$(OBJ)\
	$(OUT_DIR)/ss_pack.$(OBJ)\
	$(OUT_DIR)/ss_queue.$(OBJ)\
	$(OUT_DIR)/ss_strm.$(OBJ)\
	$(OUT_DIR)/ss_task.$(OBJ)\
	$(OUT_DIR)/ss_drvr.$(OBJ)\
	$(OUT_DIR)/ss_timer.$(OBJ)\
	$(OUT_DIR)/qu_ipc_fifo_inf.$(OBJ)\
	$(OUT_DIR)/qu_utl_circq.$(OBJ)\
	$(OUT_DIR)/qu_krait_hex_comm.$(OBJ)
#	$(OUT_DIR)/ss_diag.$(OBJ)
#ifeq (${BLDENV},lnx_e2e_ms)
#	mtOBJS+=$(OUT_DIR)/mt_4gmx.$(OBJ)
#else
#ifeq (${BLDENV},lnx_ms_withoutdiag)
#	mtOBJS+=$(OUT_DIR)/mt_4gmx.$(OBJ)
#endif
#endif

MT_INC=\
   $(IN_DIR)/ss_msg.x $(IN_DIR)/ss_drvr.x \
   $(IN_DIR)/ss_strm.h $(IN_DIR)/ss_strm.x  $(IN_DIR)/ss_task.h \
   $(IN_DIR)/ss_task.x $(IN_DIR)/ss_timer.x $(IN_DIR)/ss_gen.x \
   $(IN_DIR)/mt_ss.x $(IN_DIR)/ss_diag.h $(IN_DIR)/qu_utl_circq.x \
   $(IN_DIR)/qu_krait_hex_comm.h


ALL_INC=$(MT_INC) $(CM_INC)

#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#
$(OUT_DIR)/mt_id.$(OBJ): $(IN_DIR)/mt_id.c $(ALL_INC)
	$(CC) -c -o $(OUT_DIR)/mt_id.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/mt_id.c
$(OUT_DIR)/mt_ss.$(OBJ): $(IN_DIR)/mt_ss.c $(ALL_INC)
	$(CC) -c -o $(OUT_DIR)/mt_ss.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/mt_ss.c
#ifeq (${BLDENV},lnx_e2e_ms)
#$(OUT_DIR)/mt_4gmx.$(OBJ): $(IN_DIR)/mt_4gmx.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/mt_4gmx.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/mt_4gmx.c
#else
#ifeq (${BLDENV},lnx_ms_withoutdiag)
#$(OUT_DIR)/mt_4gmx.$(OBJ): $(IN_DIR)/mt_4gmx.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/mt_4gmx.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/mt_4gmx.c
#endif
#endif
$(OUT_DIR)/ss_drvr.$(OBJ): $(IN_DIR)/ss_drvr.c $(ALL_INC)
	$(CC) -c -o $(OUT_DIR)/ss_drvr.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/ss_drvr.c
$(OUT_DIR)/ss_gen.$(OBJ): $(IN_DIR)/ss_gen.c $(ALL_INC)
	$(CC) -c -o $(OUT_DIR)/ss_gen.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/ss_gen.c
$(OUT_DIR)/ss_id.$(OBJ): $(IN_DIR)/ss_id.c $(ALL_INC)
	$(CC) -c -o $(OUT_DIR)/ss_id.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/ss_id.c
$(OUT_DIR)/ss_mem.$(OBJ): $(IN_DIR)/ss_mem.c $(ALL_INC)
	$(CC) -c -o $(OUT_DIR)/ss_mem.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/ss_mem.c
$(OUT_DIR)/ss_msg.$(OBJ): $(IN_DIR)/ss_msg.c $(ALL_INC)
	$(CC) -c -o $(OUT_DIR)/ss_msg.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/ss_msg.c
$(OUT_DIR)/ss_pack.$(OBJ): $(IN_DIR)/ss_pack.c $(ALL_INC)
	$(CC) -c -o $(OUT_DIR)/ss_pack.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/ss_pack.c
$(OUT_DIR)/ss_queue.$(OBJ): $(IN_DIR)/ss_queue.c $(ALL_INC)
	$(CC) -c -o $(OUT_DIR)/ss_queue.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/ss_queue.c
$(OUT_DIR)/ss_rtr.$(OBJ): $(IN_DIR)/ss_rtr.c $(ALL_INC)
	$(CC) -c -o $(OUT_DIR)/ss_rtr.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/ss_rtr.c
$(OUT_DIR)/ss_strm.$(OBJ): $(IN_DIR)/ss_strm.c $(ALL_INC)
	$(CC) -c -o $(OUT_DIR)/ss_strm.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/ss_strm.c
$(OUT_DIR)/ss_task.$(OBJ): $(IN_DIR)/ss_task.c $(ALL_INC)
	$(CC) -c -o $(OUT_DIR)/ss_task.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/ss_task.c
$(OUT_DIR)/ss_timer.$(OBJ): $(IN_DIR)/ss_timer.c $(ALL_INC)
	$(CC) -c -o $(OUT_DIR)/ss_timer.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/ss_timer.c
$(OUT_DIR)/qu_ipc_fifo_inf.$(OBJ): $(IN_DIR)/qu_ipc_fifo_inf.c $(ALL_INC)
	$(CC) -c -o $(OUT_DIR)/qu_ipc_fifo_inf.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/qu_ipc_fifo_inf.c
$(OUT_DIR)/qu_utl_circq.$(OBJ): $(IN_DIR)/qu_utl_circq.c $(ALL_INC)
	$(CC) -c -o $(OUT_DIR)/qu_utl_circq.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/qu_utl_circq.c
$(OUT_DIR)/qu_krait_hex_comm.$(OBJ): $(IN_DIR)/qu_krait_hex_comm.c $(ALL_INC)
	$(CC) -c -o $(OUT_DIR)/qu_krait_hex_comm.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/qu_krait_hex_comm.c
#$(OUT_DIR)/ss_diag.$(OBJ): $(IN_DIR)/ss_diag.c $(ALL_INC)
#	$(CC) -c -o $(OUT_DIR)/ss_diag.o $(COPTS) $(IOPTS) $(POPTS) $(CCmtFLAGS) $(IN_DIR)/ss_diag.c

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(OUT_DIR)/libmt.a:$(mtOBJS)
	$(AR) -cr $(OUT_DIR)/libmt.a $(mtOBJS)	

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f libmt.a $(mtOBJS)


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
#                    Email: support@trillium.com
#                    Web: http://www.radisys.com 
# 
#*********************************************************************17*/
#
#********************************************************************20**
#
#       Name:   MTSS - acceptance test
#
#       Type:   Makefile
#
#       Desc:   Build the System Services acceptance test with
#               MTSS-Solaris.
#
#               The default setup of this file is for UNIX  
#
#               For UNIX make:
#
#               1) ensure DOS Microsoft 5.1 link is commented out
#               2) ensure DOS Microsoft 6.0 link is commented out
#               3) ensure UNIX link is not commented out
#               4) ensure all	$(CC)'s are preceded by tab
#               5) ensure macros are set correctly for UNIX
#
#               Note:
#
#               The full path for .h files has to be specified
#               in INC_DIR macro
#
#       File:   mt_acc.mak
#
#       Sid:      mt_acc.mak@@/main/3 - Mon Nov 17 15:54:10 2008
#
#       Prg:    ada
#  
#********************************************************************21*/
#
#-----------------------------------------------------------------------
#

include makecfg
CWD:=$(shell pwd)
CP := cp -f
RM := rm -f
MAKE := make



#gcc -m32 -c -DSUNOS -DANSI -DSS_LINUX -Os -g3 -ansi -Wall -fno-strict-aliasing -Wno-comment -pipe -Wshadow -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -pedantic -Wimplicit -Wunused -I. -I. -I. -I. -I.  -Iu/prd/rtr/mt -I. -I. -I/u/prd/rtr/mt -I/u/prd/rtr/mt -DSS -DCMFILE_REORG_1 -DCMFILE_REORG_2  -DSS_MT -DCMFILE_REORG_1 -DSS_SINGLE_THREADED -D_REENTRANT -DDEBUGNOEXIT -DSS_DRVR_SUPPORT -DNH -DCMFILE_REORG_1 -DCMFILE_REORG_2 -DDEBUGP -DCM_PASN_DBG -DCM_INET2 -DCCPU_OPT -DRRC_PHY_CFG_SUPP -UNH_ASN_COND_VALIDATE -DNHU_MULT_SAP_SUPPRT -DLTERRC_REL9 -DNHU_VER2 -DCTF_VER2 -DNX -DSM -DDM -DCMXTA_MASTER -UNO_ERRCLS   ./nh_db.c

SSOPTS= -DCMFILE_REORG_1 -DxSTDIO_INCLD -fPIC #-Dsparc -fPIC
MTOPTS= -DCMFILE_REORG_1 -DxSTDIO_INCLD -fPIC #-Dsparc -fPIC
ACOPTS= -DCMFILE_REORG_1 -DxSTDIO_INCLD -fPIC #-Dsparc -fPIC
CMOPTS= -DCMFILE_REORG_1 -DxSTDIO_INCLD -fPIC #-Dsparc -fPIC

#-----------------------------------------------------------------------
# macro for output file name and makefile name
#

# Target Library
TARGET = libssi.a
#

#LIBNAME=libssi.so
#OUTNAME=mt_acc
MAKENAME=mt_acc.mak

#-----------------------------------------------------------------------
# macro for include directories
#

SY_DIR=.
SS_DIR=.
CM_DIR=.
MT_DIR=.
AC_DIR=.
INC_DIR=-I. -I$(CM_DIR) -I$(SS_DIR)  # include directory
MT_INC_DIR=-I$(CM_DIR) -I$(MT_DIR)
#Enable this Macro for C++ compilation.
#INC_DIR=-I. -I$(CM_DIR) -I$(SS_DIR) #-I/usr/include/nptl   # include directory

#-----------------------------------------------------------------------
# macros for compile and link options
#
  
TRACE=                                   # trace
LIST=                                    # listing

# environment:
# -DMSC60    microsoft c 6.0
# -DMSC51    microsoft c 5.1
# -DMSC70    microsoft c 7.0
# -DBOR31    borland c++ 3.1
# -DANSI     ansi
#
#            dos
# -DUNIX     unix
# -DSUNOS    sunos

#ENV=-DANSI -DWIN32    DSS_LINUX                    # environment, microsoft
ENV=-DANSI -DSUNOS -D_REENTRANT -DLTERRC_REL9 -DSS_LINUX -DSS_MT -DSS_INT2 -DCM_INET2 -ULONG_MSG -DCMFILE_REORG_2  -DxSS_MULTIPLE_PROCS -DxSS_MULTICORE_SUPPORT -DSS_M_PROTO_REGION -DDEBUGNOEXIT -USLES9_PLUS -USS_DRVR_SUPPORT -USS_SINGLE_THREADED -DxSS_WATCHDOG -DxSS_HISTOGRAM_SUPPORT -DxSS_LOGGER_SUPPORT -DxSS_MEM_LEAK_STS -DxSS_MEM_LEAK_SOL -DxSS_THREAD_PROFILE -DxSS_FBSED_TSK_REG -USS_DATETIME_USEC -USS_LICENSE_CHECK -DDEBUGP -UBIT_64 -D__EXTENSIONS__ -DAPI_MAIN -DSS_FAP -DCMINET_BSDCOMPAT  -DHI_MULTI_THREADED -DSS_PERF -DCM_PASN_DBG -DTENB_ACC -DCL_ACC -DNO_ERRCLS -DNOCONAVL# environment, sunosX
#ENV=-DANSI -DSUNOS -D_REENTRANT -DSS_MT -DSS_MULTICORE_SUPPORT -DSS_M_PROTO_REGION -DDEBUGNOEXIT \
#    -USS_DRVR_SUPPORT -USS_RTR_SUPPORT -DSS_DRVR_SUPPORT -DSS_SINGLE_THREADED   # environment, sunos

# lint:

LINT=lin.bat                             # dos batch command

# lint options:

LNTOPTS=-dDSTDIO_INCLD -dCONSTDIO          # lint option (same as MTOPTS)
LNTENV=-dMSC60 -dANSI -dDOS                # lint environment (same as ENV-DSS_DRVR_SUPPORT -DSS_SINGLE_THREADED)
LNTOPTION=$(LNTOPTS) $(LNTENV) -i$(CM_DIR)

# compiler:
# cl    DOS
# cc    UNIX

#CC=cl                                    # compiler, microsoft
#CC=cc                                    # compiler, sunos
#PREFIX=
#PREFIX=arm-linux-
#PREFIX=mipsel-unknown-linux-gnu-
CC=$(PREFIX)gcc                                   # compiler, gnu
LD=$(PREFIX)ld
#LD=/usr/ccs/bin/ld

# PURIIFY
PURE=
#PURE=/usr/ccpu/rational/releases/purecov.i386_linux2.7.0/purecov
#PURE=/opt/pure/purify/purify
 
# object files:
# obj   DOS
# o     UNIX

#OBJ=obj                                  # object files, microsoft
OBJ=o                                    # object files, sunos

# memory model:
# -AS   small     UNIX
# -AM   medium
# -AC   compact
# -AL   large
# -AH   huge  

#MODEL=                                  # memory model, microsoft
MODEL=                                   # memory model, sunos

# compiler options: 
#
# microsoft
#
# -D    define
# -W3   warning level 3
# -Od   optimization disabled
# -Za   disable language extensions
# -Zi   required for Code View
# -c    compile without linking
# -I    include path
# -Gs   disable stack check
# -EM   extended memory manager
#
# sunos
#
# -g    debugger
# -fast optimize for speed
# -Xa   conforming ANSI mode
# -Xc   maximally conforming ANSI mode

#COPTS=-Od -W3                            # compiler options, microsoft
#COPTS=-g3 -traditional -Wall -Wno-comment -pipe -Wshadow -Wcast-qual \
#      -Wconversion
                                          # GNU K&R compiler options
#COPTS=-g3 -Wall -Wno-comment -pipe -Wshadow -Wcast-qual \
      -Wstrict-prototypes -Wmissing-prototypes
                                          # GNU ansi compiler options
#COPTS=-Xa -g -mt                          # compiler options, solaris
COPTS= -g -ansi -pg

# linker options:
# /CO   required for Code View 
# /NOI  preserve case sensitivity
# /ST:<val>  stack siz

#LOPTS=/CO /NOI /NOE /ST:25000            # linker options
#LOPTS=-lpthread -lposix4
#LOPTS=-L/lib64/tls -lpthread -lrt
#LOPTS=-L/usr/lib/nptl -lpthread -lrt
LOPTS=-lpthread -lrt

CMTOPTS=$(TRACE) $(MODEL) $(MTOPTS) $(ENV) $(COPTS) $(MT_INC_DIR)
CACOPTS=$(TRACE) $(MODEL) $(ACOPTS) $(ENV) $(COPTS) $(INC_DIR)
CCMOPTS=$(TRACE) $(MODEL) $(SSOPTS) $(ENV) $(COPTS) $(INC_DIR)
CSSOPTS=$(TRACE) $(MODEL) $(SSOPTS) $(ENV) $(COPTS) $(INC_DIR)
LOPTION=$(LOPTS)

#-----------------------------------------------------------------------
# macros for include files
#

ENV_INC=$(CM_DIR)/envdep.h $(CM_DIR)/envind.h $(CM_DIR)/envopt.h   # environment
SSI_INC=$(CM_DIR)/ssi.h $(CM_DIR)/ssi.x                     # system services
GEN_INC=$(CM_DIR)/gen.h $(CM_DIR)/gen.x                     # general layer
SS_INC=$(SS_DIR)/ss_dep.h $(SS_DIR)/ss_dep.x $(SS_DIR)/ss_err.h $(SS_DIR)/ss_drvr.x $(SS_DIR)/ss_gen.h $(SS_DIR)/ss_gen.x $(SS_DIR)/ss_mem.h $(SS_DIR)/ss_mem.x $(SS_DIR)/ss_msg.h $(SS_DIR)/ss_msg.x $(SS_DIR)/ss_queue.h $(SS_DIR)/ss_queue.x $(SS_DIR)/ss_strm.h $(SS_DIR)/ss_strm.x $(SS_SIR)/ss_task.h $(SS_DIR)/ss_task.x $(SS_DIR)/ss_timer.x                                        # common SS
MT_INC=$(MT_DIR)/mt_ss.h $(MT_DIR)/mt_ss.x $(MT_DIR)/mt_err.h      # MTSS

#-----------------------------------------------------------------------
# macro for object files
#

CM_SRCS= \
	$(CM_DIR)/cm_mem.c \
	$(CM_DIR)/cm_mblk.c \
	$(CM_DIR)/cm_bdy5.c \
	$(CM_DIR)/cm_hash.c \
	$(CM_DIR)/cm_lib.c \
	$(CM_DIR)/cm_pasn1.c \
	$(CM_DIR)/cm_pasn2.c \
	$(CM_DIR)/cm_pasn3.c \
	$(CM_DIR)/cm_pasn4.c \
	$(CM_DIR)/nh_db.c \
	$(CM_DIR)/rr_db.c \
	$(CM_DIR)/cm_task.c 

SS_SRCS= \
        $(SS_DIR)/ss_gen.c \
        $(SS_DIR)/ss_task.c \
        $(SS_DIR)/ss_drvr.c \
        $(SS_DIR)/ss_timer.c \
        $(SS_DIR)/ss_mem.c \
        $(SS_DIR)/ss_strm.c \
        $(SS_DIR)/ss_msg.c \
        $(SS_DIR)/ss_queue.c \
        $(SS_DIR)/ss_pack.c \
        $(SS_DIR)/ss_rtr.c 

MT_SRCS= \
	$(MT_DIR)/mt_ss.c \
	$(MT_DIR)/mt_id.c
##	$(SS_DIR)/ss_acc.c 

CM_OBJS= \
	$(CM_DIR)/cm_mem.$(OBJ) \
	$(CM_DIR)/cm_mblk.$(OBJ) \
	$(CM_DIR)/cm_bdy5.$(OBJ) \
	$(CM_DIR)/cm_hash.$(OBJ) \
	$(CM_DIR)/cm_lib.$(OBJ) \
	$(CM_DIR)/cm_pasn1.$(OBJ) \
	$(CM_DIR)/cm_pasn2.$(OBJ) \
	$(CM_DIR)/cm_pasn3.$(OBJ) \
	$(CM_DIR)/cm_pasn4.$(OBJ) \
	$(CM_DIR)/nh_db.$(OBJ) \
	$(CM_DIR)/rr_db.$(OBJ) \
	$(CM_DIR)/cm_task.$(OBJ) 

SS_OBJS= \
        $(SS_DIR)/ss_gen.$(OBJ) \
        $(SS_DIR)/ss_task.$(OBJ) \
        $(SS_DIR)/ss_drvr.$(OBJ) \
        $(SS_DIR)/ss_timer.$(OBJ) \
        $(SS_DIR)/ss_mem.$(OBJ) \
        $(SS_DIR)/ss_strm.$(OBJ) \
        $(SS_DIR)/ss_msg.$(OBJ) \
        $(SS_DIR)/ss_queue.$(OBJ) \
        $(SS_DIR)/ss_pack.$(OBJ) \
        $(SS_DIR)/ss_rtr.$(OBJ) 

MT_OBJS= \
##	$(SS_DIR)/ss_acc.$(OBJ) 

SS_LIBOBJS=  \
	$(CM_OBJS) \
	$(MT_DIR)/mt_ss.$(OBJ) \
	$(MT_DIR)/mt_id.$(OBJ) \
	$(SS_DIR)/ss_gen.$(OBJ) \
	$(SS_DIR)/ss_task.$(OBJ) \
	$(SS_DIR)/ss_drvr.$(OBJ) \
	$(SS_DIR)/ss_timer.$(OBJ) \
	$(SS_DIR)/ss_mem.$(OBJ) \
	$(SS_DIR)/ss_strm.$(OBJ) \
	$(SS_DIR)/ss_msg.$(OBJ) \
	$(SS_DIR)/ss_queue.$(OBJ) \
	$(SS_DIR)/ss_pack.$(OBJ) \
	$(SS_DIR)/ss_rtr.$(OBJ) 






#-----------------------------------------------------------------------
# All the Objects file
OBJS = $(CM_OBJS) $(SS_OBJS) $(MT_OBJS) $(SS_LIBOBJS)

#
# Default target
#
all:    $(TARGET)

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $(TARGET) $(OBJS)

#-----------------------------------------------------------------------
# UNIX link - absolute object file depends on relocatable object
# (.o) files
#



#$(LIBNAME): $(SS_LIBOBJS)
#	$(PURE) $(LD) -shared -o $(LIBNAME) $(SS_LIBOBJS) $(LOPTS)
#
#$(OUTNAME): $(MT_OBJS) $(LIBNAME)
#	$(PURE)	$(CC) -o $(OUTNAME) $(LIBNAME) $(MT_OBJS) $(LOPTS) 
#
#depend: $(CM_SRCS) $(SS_SRCS) $(MT_SRCS)
##	makedepend -f $(MAKENAME) -- $(ENV) $(MTOPTS) $(CMOPTS) $(INC_DIR) -- $(SS_SRCS) $(CM_SRCS) $(MT_SRCS)
#
clean:
	-/bin/rm -f $(SS_DIR)/*.o $(CM_DIR)/*.o $(MT_DIR)/*.o $(SS_DIR)*.i $(CM_DIR)/*.i $(MT_DIR)/*.i $(SS_DIR)/*.a

#-----------------------------------------------------------------------
# lint
#

#lint:
# $(LINT) $(LNTOPTION) $(OUTNAME).lnt > $(OUTNAME).ler

#-----------------------------------------------------------------------
# DOS Microsoft 6.0 link - absolute object (.exe) file depends on
# relocatable object (.obj) files
#

#$(OUTNAME).exe: $(OBJS) $(OUTNAME).lnk
# link $(LOPTION) @$(OUTNAME).lnk
   
#-----------------------------------------------------------------------
# relocatable object (.obj) files depend on respective source (.c) and
# include (.h and .x) files
#
# note: if an include file includes another include file then the second
# include file must also be shown on the dependency list
#

$(SS_DIR)/ss_gen.$(OBJ): $(SS_DIR)/ss_gen.c
	$(PURE) $(CC) -c $(CSSOPTS) $(SS_DIR)/ss_gen.c -o $(SS_DIR)/ss_gen.o

$(SS_DIR)/ss_task.$(OBJ): $(SS_DIR)/ss_task.c
	$(PURE) $(CC) -c $(CSSOPTS) $(SS_DIR)/ss_task.c -o $(SS_DIR)/ss_task.o

$(SS_DIR)/ss_drvr.$(OBJ): $(SS_DIR)/ss_drvr.c
	$(PURE) $(CC) -c $(CSSOPTS) $(SS_DIR)/ss_drvr.c -o $(SS_DIR)/ss_drvr.o

$(SS_DIR)/ss_timer.$(OBJ): $(SS_DIR)/ss_timer.c
	$(PURE) $(CC) -c $(CSSOPTS) $(SS_DIR)/ss_timer.c -o $(SS_DIR)/ss_timer.o

$(SS_DIR)/ss_mem.$(OBJ): $(SS_DIR)/ss_mem.c
	$(PURE) $(CC) -c $(CSSOPTS) $(SS_DIR)/ss_mem.c -o $(SS_DIR)/ss_mem.o

$(SS_DIR)/ss_strm.$(OBJ): $(SS_DIR)/ss_strm.c
	$(PURE) $(CC) -c $(CSSOPTS) $(SS_DIR)/ss_strm.c -o $(SS_DIR)/ss_strm.o

$(SS_DIR)/ss_msg.$(OBJ): $(SS_DIR)/ss_msg.c
	$(PURE) $(CC) -c $(CSSOPTS) $(SS_DIR)/ss_msg.c -o $(SS_DIR)/ss_msg.o

$(SS_DIR)/ss_queue.$(OBJ): $(SS_DIR)/ss_queue.c
	$(PURE) $(CC) -c $(CSSOPTS) $(SS_DIR)/ss_queue.c -o $(SS_DIR)/ss_queue.o

$(SS_DIR)/ss_pack.$(OBJ): $(SS_DIR)/ss_pack.c
	$(PURE) $(CC) -c $(CSSOPTS) $(SS_DIR)/ss_pack.c -o $(SS_DIR)/ss_pack.o

$(SS_DIR)/ss_rtr.$(OBJ): $(SS_DIR)/ss_rtr.c
	$(PURE) $(CC) -c $(CSSOPTS) $(SS_DIR)/ss_rtr.c -o $(SS_DIR)/ss_rtr.o

#$(SS_DIR)/ss_acc.$(OBJ): $(SS_DIR)/ss_acc.c
#	$(PURE) $(CC) -c $(CSSOPTS) $(SS_DIR)/ss_acc.c -o $(SS_DIR)/ss_acc.o

$(CM_DIR)/cm_mem.$(OBJ): $(CM_DIR)/cm_mem.c
	$(PURE)	$(CC) -c $(CCMOPTS) $(CM_DIR)/cm_mem.c -o $(CM_DIR)/cm_mem.o

$(CM_DIR)/cm_mblk.$(OBJ): $(CM_DIR)/cm_mblk.c
	$(PURE)	$(CC) -c $(CCMOPTS) $(CM_DIR)/cm_mblk.c -o $(CM_DIR)/cm_mblk.o

$(CM_DIR)/cm_bdy5.$(OBJ): $(CM_DIR)/cm_bdy5.c
	$(PURE)	$(CC) -c $(CCMOPTS) $(CM_DIR)/cm_bdy5.c -o $(CM_DIR)/cm_bdy5.o

$(CM_DIR)/cm_hash.$(OBJ): $(CM_DIR)/cm_hash.c
	$(PURE)	$(CC) -c $(CCMOPTS) $(CM_DIR)/cm_hash.c -o $(CM_DIR)/cm_hash.o
$(CM_DIR)/cm_lib.$(OBJ): $(CM_DIR)/cm_lib.c
	$(PURE)	$(CC) -c $(CCMOPTS) $(CM_DIR)/cm_lib.c -o $(CM_DIR)/cm_lib.o
$(CM_DIR)/cm_pasn1.$(OBJ): $(CM_DIR)/cm_pasn1.c
	$(PURE)	$(CC) -c $(CCMOPTS) $(CM_DIR)/cm_pasn1.c -o $(CM_DIR)/cm_pasn1.o
$(CM_DIR)/cm_pasn2.$(OBJ): $(CM_DIR)/cm_pasn2.c
	$(PURE)	$(CC) -c $(CCMOPTS) $(CM_DIR)/cm_pasn2.c -o $(CM_DIR)/cm_pasn2.o
$(CM_DIR)/cm_pasn3.$(OBJ): $(CM_DIR)/cm_pasn3.c
	$(PURE)	$(CC) -c $(CCMOPTS) $(CM_DIR)/cm_pasn3.c -o $(CM_DIR)/cm_pasn3.o
$(CM_DIR)/cm_pasn4.$(OBJ): $(CM_DIR)/cm_pasn4.c
	$(PURE)	$(CC) -c $(CCMOPTS) $(CM_DIR)/cm_pasn4.c -o $(CM_DIR)/cm_pasn4.o
$(CM_DIR)/nh_db.$(OBJ): $(CM_DIR)/nh_db.c
	$(PURE)	$(CC) -c $(CCMOPTS) $(CM_DIR)/nh_db.c -o $(CM_DIR)/nh_db.o
$(CM_DIR)/rr_db.$(OBJ): $(CM_DIR)/rr_db.c
	$(PURE)	$(CC) -c $(CCMOPTS) $(CM_DIR)/rr_db.c -o $(CM_DIR)/rr_db.o
$(CM_DIR)/cm_task.$(OBJ): $(CM_DIR)/cm_task.c
	$(PURE)	$(CC) -c $(CCMOPTS) $(CM_DIR)/cm_task.c -o $(CM_DIR)/cm_task.o
$(MT_DIR)/mt_ss.$(OBJ): $(MT_DIR)/mt_ss.c
	$(PURE)	$(CC) -c $(CMTOPTS) $(MT_DIR)/mt_ss.c -o $(MT_DIR)/mt_ss.o
$(MT_DIR)/mt_id.$(OBJ): $(MT_DIR)/mt_id.c
	$(PURE)	$(CC) -c $(CMTOPTS) $(MT_DIR)/mt_id.c -o $(MT_DIR)/mt_id.o



#
# Default target
#
#all:    $(TARGET)
#
#$(TARGET): $(OBJS)
#	$(AR) $(ARFLAGS) $(TARGET) $(OBJS)
#
#-----------------------------------------------------------------------
# DOS Microsoft 5.1 link - absolute object (.exe) file depends on
# relocatable object (.obj) files
#

#$(OUTNAME).exe: $(OBJS) $(OUTNAME).lnk
# link $(LOPTION) @$(OUTNAME).lnk
 
#----------------------------------------------------------------
# lint
#
 
#lint:
# $(LINT) $(LNTOPTION) $(OUTNAME).lnt > $(OUTNAME).ler

# DO NOT DELETE THIS LINE -- make depend depends on it.


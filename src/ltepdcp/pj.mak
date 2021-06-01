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
#       Name:   LTE RLC-PDCP - makefile
#
#       Type:   Makefile
#
#       Desc:   Compile, assemble and link product software for
#               various configurations. Further comments are
#               embedded in the file. The following targets are
#               defined for use:
#
#               This file supports a variety of environments and
#               build targets.
#
#               ENVIRONMENTS:
#
#               Solaris 2.x with SUNpro C
#
#               Solaris 2.x with GNU C (gcc)
#
#               Solaris 2.x with MRI C (mcc68k) cross compiled for
#               a relatively standard 68040 processor board
# 
#               Microsoft Visual C++ 4.x for Windows '95 and Windows NT
#               environments(msvc)
#
#               BUILD TARGETS:
#
#               acc              acceptance tests (with MTSS) -- default
#               pt               portable software (without MTSS) 
#               lib              library (with MTSS)
#
#               depend.acc       makedepend for acceptance tests
#               depend.pt        makedepend for portable software
#
#               clean            remove all object files
#               win_clean        remove all object files(Windows environment)
#
#       Sid:      pj.mak@@/main/TeNB_Main_BR/6 - Mon Aug 11 16:42:43 2014
#
#       Ppj:   eq
#
#********************************************************************21*/
#
# Modifications required :
# For WinNT SSI :
#          BLDENV=win_acc (to be supplied on command line)
#          OBJ=$(NS_OBJ_SUFFIX)
#          make SS_DIR and MT_DIR point to path for WinNT SSI files
#          Replace MT_SRCS with NS_SRCS in PRDACC_SRCS
#          Replace MT_OBJS with NS_OBJS in PRDACC_OBJS
#          Put OSOPTS=$(WINOSOPTS) in acc/pt/lib target
#          LM_ACC_CMD_LINE shouldn't be present in PRDACCOPTS
#          use the ACC_INC_PATH in which VW_INC is not included
#
# For VxWorks SSI :
#          BLDENV=ppc_acc (to be supplied on command line)
#          OBJ=$(SOL_OBJ_SUFFIX)
#          make VW_DIR point to path for VxWorks SSI files
#          Put SS_DIR and MT_DIR as $(VW_DIR)
#          Replace MT_SRCS with VW_SRCS in PRDACC_SRCS
#          Replace MT_OBJS with VW_OBJS in PRDACC_OBJS
#          Put OSOPTS=$(VWOSOPTS) for acc rule
#          LM_ACC_CMD_LINE shouldn't be present in PRDOPTS
#          modify VW_INC appropriately to point to correct header files
#          use ACC_INC_PATH in which VW_INC is included
#
#=======================================================================

#=======================================================================
#
# macros for product specific options
#
# SECTION 1

#----------------- RLC product options --------------------------
# 

#LPJ Inerface Loose coupling
PRDLCLPJOPTS=-DLCLPJ -DLCSMKWMILPJ -DLCPJMILPJ

#CKW Inerface Loose coupling
PRDLCCKWOPTS=-DLCCKW -DLCSMKWUICKW -DLCKWUICKW
#PRDLCCKWOPTS=

#CPJ Inerface Loose coupling
PRDLCCPJOPTS=-DLCCPJ -DLCSMPJUICPJ -DLCPJUICPJ

#KWU Inerface Loose coupling
PRDLCKWUOPTS=-DLCKWU -DLCSMKWUIKWU -DLCKWUIKWU
#PRDLCKWUOPTS=

#PJU Inerface Loose coupling
PRDLCPJUOPTS=-DLCPJU -DLCSMPJUIPJU -DLCPJUIPJU

#RGU Inerface Loose coupling
PRDLCRGUOPTS=-DLCRGU -DLCSMKWLIRGU -DLCKWLIRGU

PRDOPTS=-DSS_MT_TMR -DKW -DPJ -UNH -USM -DCMFILE_REORG_1 -DCMFILE_REORG_2 -UDEBUGP -DPJ_DEBUG -DPTPJCPJ\
		  -UKW_PDCP -DCCPU_OPT -UPJ_CMP_ASYNC -UPJ_SEC_ASYNC -DPJ -ULWLCPJULUDX -ULCUDX -UNO_ERRCLS -UPTPJLIB -UTENB_AS_SECURITY\
		  -DTENB_SPLIT_ARCH -DSS_LOCKLESS_MEMORY -UTENB_T2K3K_SPECIFIC_CHANGES -DTENB_ACC -DLTE_PAL_ENB -DTENB_T2K_ACC -DRLOG_ENABLE_TEXT_LOGGING \
		  $(PRDLCKWUOPTS) $(PRDLCLPJOPTS) $(PRDLCCPJOPTS) $(PRDLCPJUOPTS)

#-------------------System Service options----------------------
GENOSOPTS=-DCMFILE_REORG_1 -DCMFILE_REORG_2

# For No error class

# Product options for portable build
PRDPTOPTS=$(PRDOPTS) $(GENOSOPTS) -DPTPJLIB

# Product options for acceptance test build
# Remove -DKW_ACC_CMD_LINE if your system doesn't support SGetOpts (command
# line inputs)
# Comment out CMD_LINE given below for VxWorks

CMD_LINE=-DPJ_ACC_CMD_LINE

CM_INETOPTS=-DCM_INET2 #-D_GNU_SOURCE -DCMINET_BSDCOMPAT
#For Windows Add -DSS_DRVR_SUPPORT -DENB_RELAY
PRDACCOPTS=$(PRDOPTS) -DKW -DNH -DPX -DPJ -DSM $(CMD_LINE)\
			  -DCMXTA_MASTER $(CM_INETOPTS) 

#Enable Below PRDACCOPTS for VxWorks compilation
#PRDACCOPTS=$(PRDOPTS) -DKW -DxNH -DxPJ -DxRG -DxSM $(CMD_LINE)\
#			  -DxCMXTA_MASTER -DxNO_ERRCLS $(CM_INETOPTS)
# Product options for library build
PRDLIBOPTS=$(PRDOPTS) -DDBG4 

#=======================================================================
#
# macros for specific OS
#
# SECTION 2

# MOS Product specific option define
MSCOPTS=-DMS -DCMFILE_REORG_1 

# MTSS Product specific option define
MTCOPTS = 

# Options for compilation with different OS's
# Default is for Portable build
# For acceptance test build either uncomment specific OS environment
# required or specify on command line
# Portable build options
#PTOSOPTS="-DPORTVER"
PTOSOPTS="-DPORTVER -DDONT_USE_SYS_LIB"

# Options for acceptance test build with VrTx Os
VXOSOPTS="-DVX"

# Options for acceptance test build with VxWorks Os
#VWOSOPTS="-DVW"
VWOSOPTS=-DSS_VW -DSS -DCMFILE_REORG_1 -DSS_DRVR_SUPPORT -DVW_COMPILE -DCMINETFLATBUF -DCMIET_LARGE_MAX_UDPRAW_SIZE -DDEBUGNOEXIT


# Options for acceptance test build with WINNT Os kernel build
WINOSOPTS="-DWINNT"
#WINOSOPTS="-DWINNT -DNS -DSS -DCMFILE_REORG_1"

# Options for acceptance test build with MTSS - MOS Multithread OS
# To be enabled for multithreaded RLC
MTOSOPTS=-DSS_MT -DSS -DCMFILE_REORG_1 -DSS_SINGLE_THREADED -D_REENTRANT \
-DDEBUGNOEXIT -DSS_DRVR_SUPPORT -DSS_MT_TMR

# Options for library build with MTSS - MOS multithread OS
# To be enabled for multithreaded RLC
#LIBOSOPTS="-DSS_MT -DSS  "
LIBOSOPTS="-DSS_MT -DSS  -DSS_SINGLE_THREADED"

# General OSOPTS. For portable build PTOSOPTS, for acceptante MTOSOPTS if using
# Trillium's MTSS solaris system services, for all others use one of the above
# or customer specific flag

#OSOPTS=$(PTOSOPTS)
OSOPTS=$(MTOSOPTS)
#VxWorks Options
#OSOPTS=$(GENOSOPTS) $(VWOSOPTS)

# For Windows
#NSOSOPTS=-DNS -DSS_OLD_THREAD -DNS_ENB_MGMT -DSS_DRVR_SUPPORT
#OSOPTS=$(GENOSOPTS) $(NSOSOPTS)



#=======================================================================
# SECTION 3
# Treat each of the following as examples and modify to specific product
# requirements
#
# macro for output file name and makefile name 
#
BASENAME=pj

MAKENAME=$(BASENAME).mak

# default acceptance test for Solaris cc environment
#BINACC=$(BASENAME)_acc
BINACC=pj_acc

# default portable for Solaris cc environment
BINPT="$(BASENAME)_pt"

# default portable build for KlocWork
BINPJ="$(BASENAME)_kw"

# default library for Solaris cc environment
BINLIB="$(BASENAME)_lib.lib"

# Internal targets for the acceptance test build
MAKEACC=mkacc
ACCBIN=$(BASENAME)_acc

MAKEINT=mkint
INTBIN=$(BASENAME)_int

# Internal target for the portable build
MAKEPT=mkpt
PTBIN=$(BASENAME)_pt

# Internal target for the kw build
MAKEKW=mkkw
KWBIN=$(BASENAME)_kw

# Internal target for the library build
MAKELIB=mklib
LIBBIN=$(BASENAME)_lib.lib

# Internal target for makedepend
DEPEND=depend

# Build targets for testing in different environments

# Build with the Solaris CC Compiler and linker
# Acceptance Test Build
BINSOLACC=sol_acc
BINSOLINT=sol_int
# Portable Build
BINSOLPT=sol_pt
# Library Build
BINSOLLIB=sol_lib


# Build with the GNU Compiler and linker on Solaris
# Acceptance Test Build
BINGNUACC=gnu_acc
BINGNUINT=gnu_int
# Portable Build
BINGNUPT=gnu_pt
# library Build
BINGNULIB=gnu_lib
# KlocWork Build
BINGNUKW=gnu_kw

# Build with the GNU Compiler and linker on Solaris
# Acceptance Test Build
BINGPPACC=gpp_acc
BINGPPINT=gpp_int
BINGPPPT=gpp_pt
BINGPPLIB=gpp_lib

# Build with the GNU Compiler and linker on Solaris
# Acceptance Test Build
BINCPPACC=cpp_acc
BINCPPINT=cpp_int
BINCPPPT=cpp_pt
BINCPPLIB=cpp_lib


# Build with the Insite Memory Check Programs - Acceptance Tests Only
# Acceptance Test Build
BININSITEACC=ins_acc
BININSITEINT=ins_int

# Build with the Visual C++ on Windows NT or Windows 95 (user space only)
# Acceptance Test Build
BINWINACC=win_acc
BINWININT=win_int
# Portable Build
BINWINPT=win_pt
# Library Build
BINWINLIB=win_lib

# Build with the Microtek MRI C Compiler on Solaris - portable only
BINMRIPT=mri_pt

# Build with the Solaris cross compiler for VxWorks Power PC
# Acceptance Test Build
BINPPCACC=ppc_acc
BINPPCINT=ppc_int

#Build with the cc386 compiler and liner on 
BINVWACC=vw_acc

# Build with the GNU compiler and linker on Linux
# Acceptance Test Build
BINLNXACC=lnx_acc
BINLNXINT=lnx_int
# Portable Build
BINLNXPT=lnx_pt
#purcov build
BINLNXPACC=lnx_pacc
#Library Build
BINLNXLIB=lnx_lib

# purify
BINPURACC=pur_acc

#=======================================================================
#
# macro for directories and include paths
#
# SECTION 4
# Treat each of the following as examples and modify to specific product
# requirements

CODEPATH = .
# Common files directory 
CM_DIR=../../cm

# RLC directory
PJ_DIR=../../ltepdcp

# Parent RLC directory
GP_DIR=../../lterlc

# Sample RLC User/provider directory
PJ_SAMPLE_DIR=$(CODEPATH)

# common stack manager files directory 
SM_DIR=$(CODEPATH)

# RLC stack manager directory
SM_DIR=$(CODEPATH)

# WIN Include directory
NS_DIR=/u/prd/rtr/ns

# mos directory
#Windows Directory
#OS_DIR=$(NS_DIR)
#VxWorks Directory
# OS_DIR=$(VW_DIR)

OS_DIR=$(CODEPATH)

# VxWorks include directory
VW_DIR=/u/prd/rtr/vw

# Common SSI directory
SS_DIR=../../mt_acc
#SS_DIR=$(VW_DIR)
#SS_DIR=$(NS_DIR)

# Solaris MTSS specific dir.
MT_DIR=$(SS_DIR)

# acceptance directory
PJACC_DIR=$(CODEPATH)

# acceptance directory(added for CC compilation on solaris)
# XTA-Python specific
#PY_DIR=C:\Python24\include
PY_DIR=/usr/ccpu/python/linux/32bit/python-2.4.4/include/python2.4
#PY_DIR=/usr/ccpu/python/linux/64bit/python-2.4.4/include/python2.4
#PY_DIR=/usr/ccpu/python/solaris/10/32bit/python-2.4.4/include/python2.4
#PY_DIR=/usr/ccpu/python/solaris/10/64bit/python-2.4.4/include/python2.4
#build directory
SY_DIR=$(CODEPATH)

#RLOG directory
RL_DIR=../../rlog/

#include path for VxWorks
# include path VxWorks environment
#VW_INC=C:/WindRiver/vxworks-6.3/target/h
#VW_INC1=C:/WindRiver/vxworks-6.3/target/h/wrn
#VW_INC2=C:/WindRiver/vxworks-6.3/target/h/wrn/coreip
#VW_INC3=C:/WindRiver/vxworks-6.3/target/h/wrn/coreip/netinet
#VW_INC4=C:/WindRiver/vxworks-6.3/sys-include


# portable include path
PT_INC_PATH="-I$(CM_DIR) -I$(PJ_DIR) -I$(../cm)"
# for m68k 
#PT_INC_PATH="-I$(CM_DIR) -I$(LM_DIR) -I/usr/include"

# library include path
LIB_INC_PATH="-I$(PJ_DIR) -I$(SY_DIR) -I$(CM_DIR) -I$(../cm) -I$(SS_DIR) -I$(MT_DIR)"

# acceptance test include path
ACC_INC_PATH="-I$(PJ_DIR) -I$(CM_DIR) -I$(PJACC_DIR) -I$(SM_DIR) \
	          -I$(SM_DIR)  -I$(OS_DIR) -I$(PJ_SAMPLE_DIR)\
              -I$(MT_DIR) -I$(SS_DIR) -I$(PY_DIR) -I$(RL_DIR)"

# acceptance test include path for VXWORKS
#ACC_INC_PATH="-I$(PJ_DIR) -I$(CM_DIR) -I$(PJACC_DIR) -I$(SM_DIR) \
#	          -I$(SM_DIR)  -I$(OS_DIR) -I$(PJ_SAMPLE_DIR)\
#              -I$(VW_DIR) -I$(VW_INC) -I$(VW_INC1) -I$(VW_INC2) -I$(VW_INC3)\
#	       -I$(VW_INC4)"

# MTSS - files include path
MTINC_PATH=-I$(CM_DIR) -I$(SS_DIR) -I$(MT_DIR)

#=======================================================================
#
# macros for environments
#
# SECTION 5

  
#----------------------------------------- Sun C for SPARC
#
# These flags define the environment required to build an image on
# a Sun/SPARC workstation with Sun Spro C version 4.0 running
# Solaris 2.4
#
# For ANSI Compilation 
SOLENV="-DSUNOS -DANSI -D__EXTENSIONS__"
#SOLENV="-DSUNOS -DANSI"
SOLCOPTS="-Xa -g -xs"

# For non-ANSI Compilation 
#SOLENV="-DSUNOS  -D__EXTENSIONS__"
#SOLCOPTS="-g "

#For 64 Bit Compilation
#SOLCOPTS="-g -mt -xarch=v9 -DALIGN_64BIT -DBIT_64"
#SOLLOPTS="-mt -xarch=v9 -o $(OUTNAME) -lpthread -lrt -lpython2.4 -lsocket -lnsl"
#SOLPTLOPTS="-mt -xarch=v9 -o $(OUTNAME) -lpthread -lrt -lsocket -lnsl"
#SOLLD="cc -xarch=v9"

SOLCC="cc"
SOLLD="cc"
SOLLIBLD="/usr/ccs/bin/ld"
SOLLOPTS="-o $(OUTNAME) -lpthread -lrt -lsocket -lnsl -lpython2.4 -lm -ldl"
SOLPTLOPTS="-o $(OUTNAME) -lpthread -lrt -lsocket -lnsl -lm -ldl"

SOLLIBS=""
SOLLINT="lint"
SOLLNTOPTS="-c -F -p -v"

# for library compilation
SOLLIBOPTS="-r -o $(OUTNAME)"


# SUN SPARC Compiler With Insight---------------------------------------------
#
# These flags define the environment required to build an image on
# a Sun/SPARC workstation with SUN Spro C version 4.0 C compiler running
# Solaris 2.4 , with the INSURE(insight) run time memory check tool.
#
# For ANSI Compilation
INSENV="-DSUNOS -DANSI -DSS_LINUX -DNOCONAVL -DUSE_PURE -DPURE -D_GNU_SOURCE -UNOERRCHK"
INSCOPTS="-Xa -g -insure++ -uninit_min_size=1"
#INSCOPTS=""
 
# For non-ANSI Compilation
#INSENV=-DSUNOS -DPURE
#INSCOPTS="-g"
INSCOPTS="-g -DSO_INSURE "

 
#INSCC="insure"
#INSLD="insure"
INSCC="/usr/local/insure71/bin/insure -m32"
INSLD="/usr/local/insure71/bin/insure -m32"
INSLOPTS="-o $(OUTNAME)"
INSLIBS="-Xlinker -export-dynamic -lnsl -lpthread -lpython2.4 -lm -ldl -lutil -lrt"
INSLINT="lint"
INSLNTOPTS="-c -F -p -v"


#----------------------------------------- GNU gcc for SPARC 
#
# These flags define the environment required to build an image on
# a Sun/SPARC workstation with GNU C version 2.5.7 (or greater) running
# Solaris 2.4
#
# For ANSI Compilation
GNUENV="-DSUNOS -DANSI -DSOLARIS -DSPARC -DPROC_SPARC -DUNIX -D__EXTENSIONS__"

# For non-ANSI Compilation
#GNUENV="-DSUNOS -D__EXTENSIONS__"

#/* for m68k compiler */
#GNUENV="-DSUNOS -DANSI -DELVIS_68040 -DNOERRCHK -DNO_ERRCLS -DNOCONAVL"
#GNUCC="m68k-coff-gcc"
#GNULD="m68k-coff-ld"
#GNULIBLD="ld"
#GNUCOPTS="-m68040 -Wall -fno-strict-aliasing -Wno-comment -pipe -Wshadow \
#-Wcast-qual  -Wstrict-prototypes -Wmissing-prototypes"
#GNULOPTS="-A -o $(OUTNAME) -Tlinker.ld -Map=lm_pt.map" 

GNUCC=gcc
GNULD=gcc


GNULIBLD="/usr/ccs/bin/ld"

#for ANSI compilation
GNUCOPTS=" -g3 -Wimplicit -Wunused -Wcomment -Wchar-subscripts \
-Wparentheses -Wformat -Winline -Wreturn-type -fPIC \
-Wmissing-prototypes -pedantic"

#for non-ANSI
#GNUCOPTS="-g3 -Wall -Wno-comment -pipe -Wshadow -Wcast-qual "

#For 64 Bit compilation
#GNUCOPTS="-g -m64 -DALIGN_64BIT -DBIT_64"
#GNULOPTS="-m64 -DALIGN_64BIT -DBIT_64 -o $(OUTNAME) -lsocket -lnsl -lpython2.4 -lm -ldl -lposix4"
#GNUPTLOPTS="-m64 -DALIGN_64BIT -DBIT_64 -o $(OUTNAME) -lsocket -lnsl -lm -ldl -lposix4"

#g++ options
#GNUCOPTS="-g3 -Wall -fno-strict-aliasing -Wno-comment -pipe -Wcast-qual \
#          -Wstrict-prototypes -Wmissing-prototypes"

GNULOPTS="-o $(OUTNAME) -lsocket -lnsl -lpython2.4 -lm -ldl -lposix4"
GNUPTLOPTS="-o $(OUTNAME) -lsocket -lnsl -lm -ldl -lposix4"
#GNULOPTS="-o $(OUTNAME) -lpthread" 
GNULIBS=""
GNULINT="lint"
GNULNTOPTS="-c -F -p -v"

# for library compilation
GNULIBOPTS="-r -o $(OUTNAME)"

#----------------------------------------- c++ compiler
#
# These flags define the environment required to build an image on
# a Sun/SPARC workstation with GNU C version 2.5.7 (or greater) running
# Solaris 2.4
#
CPPENV="-DSUNOS -DANSI -D__EXTENSIONS__"
CPPCC=CC
CPPLD=CC
#change for LIB complaion. verify the path od ld.
CPPLIBLD=/usr/ccpu/suntools/bin/ld
#CPPCOPTS="-g -Wall -fno-strict-aliasing -Wno-comment -pipe -Wcast-qual  \
#	  -Wredundant-decls"
CPPCOPTS="-g " 
CPPLOPTS="-o $(OUTNAME) -lpthread -lposix4 -lsocket -lnsl -lpython2.4 -lm -ldl"
CPPPTLOPTS="-o $(OUTNAME) -lpthread -lposix4 -lsocket -lnsl -lm -ldl"
CPPLIBOPTS="-r -o $(OUTNAME)"
CPPLIBS=""
CPPLINT="lint"
CPPLNTOPTS="-c -F -p -v"

#----------------------------------------- g++ compiler
#
# These flags define the environment required to build an image on
# a Sun/SPARC workstation with GNU C version 2.5.7 (or greater) running
# Solaris 2.4
#
GPPENV="-DSUNOS -DANSI "
GPPCC=g++
GPPLD=g++
GPPLIBLD=/usr/ccpu/suntools/bin/ld
GPPCOPTS="-g -Wall -fno-strict-aliasing -Wno-comment -pipe -Wcast-qual  \
	     -Wredundant-decls"
GPPLOPTS="-o $(OUTNAME) -lpthread -lposix4 -lpython2.4 -lsocket -lnsl -lm -ldl"
GPPPTLOPTS="-o $(OUTNAME) -lpthread -lposix4 -lsocket -lnsl -lm -ldl"
GPPLIBOPTS="-r -o $(OUTNAME)"
GPPLIBS=""
GPPLINT="lint"
GPPLNTOPTS="-c -F -p -v"

# GNU gcc compiler for Linux------------------------------------------
#
# These flags define the environment required to build an image on
# a Linux workstation with GNU C version 2.5.7 (or greater)
#
# For ANSI Compilation
LNXENV="-DSUNOS -DANSI -DSS_LINUX -DNOCONAVL"

LNXCC="gcc -m32"
LNXLD="gcc -m32"
LNXLIBLD="ld"

#LNXCC="/usr/local/Rational/releases/PurifyPlus.2003a.06.13/i386_linux2/bin/purify gcc -g"
#LNXLD="/usr/local/Rational/releases/PurifyPlus.2003a.06.13/i386_linux2/bin/purify gcc -g"
#LNXCOPTS="-O3 -ansi -Wall -fno-strict-aliasing -Wno-comment -pipe -Wshadow -Wcast-qual \
#	-Wuninitialized -Wstrict-prototypes -Wmissing-prototypes -pedantic \
#	-Wimplicit -Wunused -fsigned-char"
LNXCOPTS="-g3 -std=c99 -Wall -fno-strict-aliasing -Wno-comment -pipe -Wshadow -Wcast-qual \
	-Wstrict-prototypes -Wmissing-prototypes -pedantic \
	-Wimplicit -Wunused -fsigned-char"
LNXLOPTS="-o $(OUTNAME) -lpthread -lnsl -lrt -Xlinker -zmuldefs -export-dynamic -lpython2.4 -lm -ldl -lutil"
LNXPTLOPTS="-o $(OUTNAME) -lpthread -lnsl -lrt -Xlinker -zmuldefs -export-dynamic -lm -ldl -lutil"
LNXLIBOPTS="-r -o $(OUTNAME)"

# For non-ANSI Compilation
#LNXENV="-DSUNOS -DSS_LINUX"
#LNXCOPTS="-g3 -pipe -pedantic -Wall -Wno-comment -Wshadow \
#          -Wcast-align -Wcast-qual"

#For 64 Bit compilation
#LNXCC="gcc -m64 -DALIGN_64BIT -DBIT_64"
#LNXLD="gcc -m64"
#LNXCOPTS="-g3 -ansi -Wall -fno-strict-aliasing -Wno-comment -pipe -Wshadow -Wcast-qual \
#        -Wmissing-prototypes -pedantic \
#        -Wimplicit -Wunused"
#LNXLOPTS="-o $(OUTNAME) -lpthread -lrt -Xlinker -zmuldefs -export-dynamic -lpython2.4 -lm -ldl -lutil"
#LNXPTLOPTS="-o $(OUTNAME) -lpthread -lrt -Xlinker -zmuldefs -export-dynamic -lm -ldl -lutil"

LNXLIBS=""
LNXLINT="lint"
LNXLNTOPTS="-c -F -p -v"
LNXLIBOSOPTS="-DSS_MT -D_REENTRANT -D__EXTENSIONS__  -DDEBUGNOEXIT -D_GNU_SOURCE -DSS_LINUX"

#PURECOV
PLNXCC="purecov gcc -m32 "
PLNXLD="purecov gcc -m32"

#PURIFY 
PURCC="purify --cache-dir=/tmp gcc -m32"
PURLD="purify --cache-dir=/tmp gcc -m32"
PURLIB="purify --cache-dir=/tmp gcc -m32"

# MRI mcc68k compiler for 68040-----------------------------------------
#
# These flags define the environment required to build an image for
# a Motorola MVME162 Board (68040) with Microtec Research C version 4.4
# for a portable environment.
#
# For ANSI Compilation
#MRIENV="-DELVIS_68040 -DANSI -DNOERRCHK -DNO_ERRCLS -DNOCONAVL -DSTDIO_INCLD"
MRIENV="-DELVIS_68040 -DANSI -UNOERRCHK -UNO_ERRCLS -DNOCONAVL -DSTDIO_INCLD"
#MRIENV="-DELVIS_68040 -DANSI  -DNOCONAVL -DSTDIO_INCLD"
 
# For non-ANSI Compilation
#MRIENV="-DELVIS_68040 -DNOERRCHK -DNO_ERRCLS -DNOCONAVL"
 
MRICC="mcc68k"
MRILD="mcc68k"
MRICOPTS="-p68040 -H -A -v -Fsm -O -Ob -Oe -Ot -nKc -nKm  -Mcp -Mdp -Ml -Gf"
MRILOPTS="-e$(OUTNAME).lnk -Wl,\"-m > $(OUTNAME).map -o $(OUTNAME).vcs -F s\""
MRILOPTS="-Wl,\"-m > $(OUTNAME).map -o $(OUTNAME).vcs -F s\""
MRILIBS="$(LIB_PATH)/mcc68kpc040.lib"
MRILINT=""
MRILNTOPTS=""
 
 
# Microsoft Visual C++ for Win 95/NT------------------------------------
#
# These flags define the environment required to build an image for
# a Windows '95 or Windows NT environment
#
#
# For ANSI Compilation
#WINENV="-DWIN32 -DANSI -DWIN2K -DWIN32_LEAN_AND_MEAN"
WINENV="-DWIN32 -DANSI -DWIN2K -DWIN32_LEAN_AND_MEAN -DSS_DRVR_SUPPORT -D_CRT_SECURE_NO_DEPRECATE"
 
# For non-ANSI Compilation
#WINENV="-DWIN32"
 
WINCC="cl"
WINLD="cl"
WINCOPTS="-Od -W3 -Zi -Yd"
WINLOPTS="-o $(OUTNAME) -F25000 -Zd -link -debug:full -debugtype:cv"
WINLIBS="ws2_32.lib"
WINLINT=""
WINLNTOPTS=""

# for library compilation
WINLIBOPTS="-r -o $(OUTNAME)"

# cc386 for VxWorks - Pentium Target
# These flags define the environment required to build an image on Vxworks
# with cc386 on Pentium Target
#
# For ANSI Compilation
VWINTENV="-DANSI -DCPU=PENTIUM -DCPU_VARIANT=PENTIUMPRO -DPQUICCADS"
VWINTCOPTS="-g -mpentium -msoft-float -fvolatile -fno-builtin -Wall \
	      -Wno-comment -ansi -pedantic -nostdinc -Wshadow \
         -Wcast-qual -fsigned-char"
VWINTLIBS="-lsocket"

# For non-ANSI Compilation
#VWINTENV="-DCPU=PENTIUM -DCPU_VARIANT=PENTIUMPRO -DPQUICCADS"
#VWINTCOPTS="-g -mpentium -msoft-float -fvolatile -fno-builtin -Wall \
#	       -Wno-comment -pipe -nostdinc -Wshadow -Wredundant-decls \
#         -Wcast-qual -fsigned-char"

VWINTCC="cc386 -mtune=pentium"
VWINTLD="ld386"
VWINTLOPTS="-o $(OUTNAME) -r"
VWINTLIBS=""
 
# ccppc cross compiler on Solaris for Vx Works on PPC--------------------
# These flags define the environment required to build an image on a Sun/SPARC
# workstation with ccppc v. 2.7.2 running Solaris 2.5.1
#
# For ANSI Compilation
#For VXWORKS
PPCENV=-DCPU=SIMNT -DANSI -DPQUICCADS -DVW_COMPILE -DCMINETFLATBUF
PPCCOPTS=-g -mtune=i486 -fvolatile -fno-builtin -Wall \
	      -Wno-comment -ansi -pedantic -pipe -nostdinc -Wshadow \
	      -Wcast-qual -fsigned-char

#PPCENV="-DCPU=PPC860 -DCCPPC -DANSI -DPQUICCADS"
#PPCENV=-DPCORE -DCPU=PPC604 -DANSI -DCCPPC
#PPCCOPTS="-mcpu=860 -msoft-float -fvolatile -fno-builtin -Wall -fno-strict-aliasing \
#              -Wno-comment -ansi -pedantic -pipe -nostdinc -Wshadow \
#              -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes"

PPCCC=ccpentium
PPCLD=ldpentium
#PPCCC="/tools/wind/host/sun4-solaris2/bin/ccppc"
#PPCLD="/tools/wind/host/sun4-solaris2/bin/ldppc"
#PPCLOPTS=-r -Map $(OUTNAME).map -o $(OUTNAME)
PPCLOPTS=-r -e vwMain -Map $(OUTNAME).map -o $(OUTNAME)
PPCLIBS=
PPCLINT=
PPCLNTOPTS=

# Microsoft Visual C++ for DOS------------------------------------------
#
# These flags define the environment required to build an image for
# a DOS environment
#
# This environment is TBD
#
# For ANSI Compilation
DOSENV="-DMSC60 -DDOS -DANSI"
 
# For non-ANSI Compilation
#DOSENV="-DMSC60 -DDOS"
 
DOSCC="cl"
DOSLD="cl"
DOSCOPTS="-Od -W3 -Zi -Yd -Gx- -Gd -Gy"
DOSLOPTS="@$(OUTNAME).lnk /ST:25000 /CO /NOI /NOE"
DOSLIBS=""
DOSLINT=""
DOSLNTOPTS=""
# legacy flag for x86 machines
#
# define if required (only for DOS!).
 
#
# -AH -- huge model
# -AL -- lakwe model
# -AS -- small model
#
MODEL=
 
 
# Actual Build Environment----------------------------------------------
#
# This is the actual environment used, set it appropriately
# (default Solaris)
# Uncomment this for default compilation under the Solaris
# environment for portable build and the acceptance test build
#ENV=$(SOLENV)
#CC=$(SOLCC)
#LD=$(SOLLD)
#COPTS=$(SOLCOPTS)
#LOPTS=$(SOLLOPTS)
#LIBS=$(SOLLIBS)
#LINT=$(SOLLINT)
#LNTOPTS=$(SOLLNTOPTS)
 
# object file suffix----------------------------------------------------
#
# this defaults to 'o' for most UNIX based systems
# this can be set to 'obj' for DOS/Windows/NT platforms
#
SOL_OBJ_SUFFIX=o
NS_OBJ_SUFFIX=obj
OBJ=$(SOL_OBJ_SUFFIX)
#OBJ=$(NS_OBJ_SUFFIX)
 


#=======================================================================
#
# macros for compile time options
#
 
# portable options
CPTOPTS=$(MODEL) $(ENV) $(COPTS) $(INC_PATH) $(OSOPTS) -DPJ_PT_BUILD -DSS_MT_TMR -DSS_FLOAT -ULTE_L2_MEAS 
 
# acceptance test options - OSOPTS should be set to required environment
COSOPTS=$(MODEL) $(ENV) $(COPTS) $(INC_PATH) $(OSOPTS) -DSS_FLOAT -ULTE_L2_MEAS 
 
# Combined environment, OS, compiler options and include path
CENVOPTS=$(MODEL) $(ENV) $(COPTS) $(INC_PATH) $(OSOPTS)
 
# product options for portable build
CPRDPTOPTS="$(CENVOPTS) $(PRDPTOPTS)"
 
# product options for library build
CPRDLIBOPTS="$(CENVOPTS) $(PRDLIBOPTS)"

# product options for acceptance test build
CPRDACCOPTS="$(COSOPTS) $(PRDACCOPTS)"

# product options for acceptance test build
CPRDINTOPTS="$(COSOPTS) $(PRDACCOPTS)"
 
# Common Stack Manager File Options for library build
CSMLIBOPTS="$(CENVOPTS) $(PRDOPTS)"

# Stack Manager file option for acceptance test build
SMOPTS=-DCMFILE_REORG_1 -DCMFILE_REORG_2 -DDEBUGP -DPJ_DEBUG -DPJ -UKW_PDCP
 
# Common Stack Manager File Options for acceptance test build
#CSKWCCOPTS="$(CENVOPTS) $(PRDOPTS) $(SMOPTS)"
CSPJCCOPTS="$(CENVOPTS) $(SMOPTS)"
 
# Common Stack Manager File Options for portable build
#CSMPTOPTS="$(CENVOPTS) $(PRDOPTS)"
CSMPTOPTS="$(CENVOPTS) $(PRDOPTS)"
 
# MTSS options - for use with the MOS files in test environment
CMTOPTS=$(CENVOPTS) $(OSOPTS) -UTENB_T2K3K_SPECIFIC_CHANGES
 

#=======================================================================
#
# macros for depend and lint options
#
 
# depend acceptance test
DEPOPTS_ACC="$(PRDACCOPTS)"
 
# depend portable test
DEPOPTS_PT="$(PRDPTOPTS)"
 

#=======================================================================
#
# macros for hardcoded dependencies 
#
# SECTION 6

#RLOG includes
RL_INC=$(RL_DIR)/rl_common.h $(RL_DIR)/rl_rlog.h $(RL_DIR)/rl_interface.h 

# 
# The following are enabled  by default since we assume makedepend
# is not available. If makedepend is  available in your environemnt
# uncomment out this section.
# 

# environment 
ENV_INC=$(CM_DIR)/envdep.h $(CM_DIR)/envind.h $(CM_DIR)/envopt.h

# system service 
SSI_INC=$(CM_DIR)/ssi.h $(CM_DIR)/ssi.x

# general 
GEN_INC=$(CM_DIR)/gen.h $(CM_DIR)/gen.x

# all system includes 
SYS_INC=$(ENV_INC) $(GEN_INC) $(SSI_INC)

# Product layer management - LPJ includes
LPJ_INC=$(CM_DIR)/lpj.h $(CM_DIR)/lpj.x


# Product Upper Interface - CPJ, and PJU includes 
PJ_U_INC=$(CM_DIR)/cpj.h $(CM_DIR)/cpj.x $(CM_DIR)/pju.h $(CM_DIR)/pju.x 

# Product Lower Interface - RGU includes
PJ_L_INC=$(CM_DIR)/kwu.h $(CM_DIR)/kwu.x

# common function library includes
CM_INC=$(CM_DIR)/cm5.x $(CM_DIR)/cm5.h     \
	$(CM_DIR)/cm_lib.x $(CM_DIR)/cm_err.h   \
	$(CM_DIR)/cm_tkns.x $(CM_DIR)/cm_tkns.h \
	$(CM_DIR)/cm_lte.x $(CM_DIR)/cm_lte.h

# PDCP layer includes
PJ_INC=$(PJ_DIR)/pj.x $(PJ_DIR)/pj_dl.x $(PJ_DIR)/pj_ul.x\
    $(PJ_DIR)/pj_env.h $(PJ_DIR)/pj.h $(PJ_DIR)/pj_dl.h

# PDCP acceptance test includes 
LPJAC_INC= $(PJ_DIR)/pj_acc.x $(PJ_DIR)/pj_acc.h

# ALL Product includes 
RGALL_INC=$(PJ_INC) $(LPJ_INC) $(PJ_U_INC) $(PJ_L_INC) $(LPJ_INC)

# Product - sample acceptance test includes
PJACC_INC=$(PJACC_DIR)/pjac_acc.h $(PJACC_DIR)/pjac_acc.x $(PJACC_DIR)/pjac_kwu.h \
				$(PJACC_DIR)/pjac_kwu.x $(PJACC_DIR)/pjac_lpj.h $(PJACC_DIR)/pjac_lpj.x \
				$(PJACC_DIR)/pjac_pju.x $(PJACC_DIR)/pjac_pju.h $(PJACC_DIR)/pjac_cpj.x \
				$(PJACC_DIR)/pjac_cpj.h $(PJACC_DIR)/pjac_lib.h  $(PJACC_DIR)/pjac_cpj.h \
				$(PJACC_DIR)/pjac_lib.h $(PJACC_DIR)/pjac_lib.x 

# sample acceptance tests all includes
PJACCALL_INC=$(RGALL_INC) $(PJACC_INC)

# OS specific includes - MOS includes
#MS_INC=$(OS_DIR)/ms.h $(OS_DIR)/ms.x $(OS_DIR)/ms_err.h
#MS_INC

# System Services common
SS_INC=$(SS_DIR)/ss_dep.h $(SS_DIR)/ss_dep.x $(SS_DIR)/ss_drvr.x \
       $(SS_DIR)/ss_err.h $(SS_DIR)/ss_gen.h $(SS_DIR)/ss_gen.x \
       $(SS_DIR)/ss_mem.h $(SS_DIR)/ss_mem.x $(SS_DIR)/ss_msg.h \
       $(SS_DIR)/ss_msg.x $(SS_DIR)/ss_queue.h $(SS_DIR)/ss_queue.x \
       $(SS_DIR)/ss_strm.h $(SS_DIR)/ss_strm.x $(SS_DIR)/ss_task.h \
       $(SS_DIR)/ss_task.x $(SS_DIR)/ss_timer.x $(SS_DIR)/ss_gen.x \
       $(SS_DIR)/ss_queue.x



# include path VxWorks environment
#VW_INC=/tools/vxworks/ppc/target/h

#=======================================================================
#
# macros for source and object files
# SECTION 7
#

#----------------------------------------- acceptance test source
PJAC_SRCS = \
	$(PJACC_DIR)/cm_xta_tmr.c         \
	$(PJACC_DIR)/cm_xta_txn.c         \
	$(PJACC_DIR)/cm_xta_res.c         \
	$(PJACC_DIR)/cm_xta_queue.c       \
	$(PJACC_DIR)/cm_xta_master.c      \
	$(PJACC_DIR)/cm_xta_reg.c         \
	$(PJACC_DIR)/cm_xta_tst.c         \
	$(PJACC_DIR)/cm_xta_log.c         \
	$(PJACC_DIR)/cm_xta_ipc.c         \
	$(PJACC_DIR)/cm_xta_flag.c        \
	$(PJACC_DIR)/cm_xta_intreg.c      \
	$(PJACC_DIR)/cm_xta_inthdl.c      \
	$(PJACC_DIR)/cm_xta_intutl.c      \
	$(PJACC_DIR)/cm_xta_py.c          \
	$(PJACC_DIR)/cm_xta_tcfsm.c       \
	$(PJACC_DIR)/cm_xta_ldfsm.c       \
	$(PJACC_DIR)/cm_xta_xml.c         \
	$(PJACC_DIR)/cm_xta_ex_ms.c       \
	$(PJACC_DIR)/cm_xta_tgfsm.c       \
	$(PJACC_DIR)/cm_xta_tafsm.c       \
	$(PJACC_DIR)/cm_xta_slave.c       \
	$(PJACC_DIR)/pjac_kw_dl_ex_ms.c   \
	$(PJACC_DIR)/pjac_kw_ul_ex_ms.c   \
	$(PJACC_DIR)/pjac_prof.c       \
	$(PJACC_DIR)/pjac_tst.c        \
	$(PJACC_DIR)/pjac_cpjreg.c     \
	$(PJACC_DIR)/pjac_cpjcb.c      \
	$(PJACC_DIR)/pjac_cpjhdl.c     \
	$(PJACC_DIR)/pjac_cpjutl.c     \
	$(PJACC_DIR)/pjac_kwureg.c     \
	$(PJACC_DIR)/pjac_kwucb.c      \
	$(PJACC_DIR)/pjac_kwuhdl.c     \
	$(PJACC_DIR)/pjac_kwuutl.c     \
	$(PJACC_DIR)/pjac_pjureg.c     \
	$(PJACC_DIR)/pjac_pjucb.c      \
	$(PJACC_DIR)/pjac_pjuhdl.c     \
	$(PJACC_DIR)/pjac_pjuutl.c     \
	$(PJACC_DIR)/pjac_nh_ptli.c    \
	$(PJACC_DIR)/pjac_nh_ex_ms.c   \
	$(PJACC_DIR)/pjac_px_ptli.c    \
	$(PJACC_DIR)/pjac_px_ex_ms.c   \
	$(PJACC_DIR)/pjac_ptli.c    \
	$(PJACC_DIR)/pjac_kw_ptui.c    \
	$(PJACC_DIR)/pjac_lpjreg.c     \
	$(PJACC_DIR)/pjac_lpjcb.c      \
	$(PJACC_DIR)/pjac_lpjhdl.c     \
	$(PJACC_DIR)/pjac_lpjutl.c		 \
	$(PJACC_DIR)/pjac_flag.c     \
	$(PJACC_DIR)/pjac_load.c		 \
	$(PJACC_DIR)/pjac_py.c      \
   $(PJACC_DIR)/pjac_lib.c      \
	$(PJACC_DIR)/pjac_libcb.c    \
	$(PJACC_DIR)/pjac_libhdl.c   \
	$(PJACC_DIR)/pjac_libreg.c   \
	$(PJACC_DIR)/pjac_libutl.c   \
	$(PJACC_DIR)/pjac_lib_ex_ms.c   \
	$(PJACC_DIR)/pjac_lib_ptli.c   
#----------------------------------------- acceptance test objects
PJAC_OBJS = \
	$(SY_DIR)/pjac_flag.$(OBJ)     \
	$(SY_DIR)/cm_xta_tmr.$(OBJ)      \
	$(SY_DIR)/cm_xta_txn.$(OBJ)      \
	$(SY_DIR)/cm_xta_res.$(OBJ)      \
	$(SY_DIR)/cm_xta_queue.$(OBJ)    \
	$(SY_DIR)/cm_xta_master.$(OBJ)   \
	$(SY_DIR)/cm_xta_reg.$(OBJ)      \
	$(SY_DIR)/cm_xta_tst.$(OBJ)      \
	$(SY_DIR)/cm_xta_log.$(OBJ)      \
	$(SY_DIR)/cm_xta_ipc.$(OBJ)      \
	$(SY_DIR)/cm_xta_flag.$(OBJ)     \
	$(SY_DIR)/cm_xta_intreg.$(OBJ)   \
	$(SY_DIR)/cm_xta_inthdl.$(OBJ)   \
	$(SY_DIR)/cm_xta_intutl.$(OBJ)   \
	$(SY_DIR)/cm_xta_py.$(OBJ)       \
	$(SY_DIR)/cm_xta_tcfsm.$(OBJ)    \
	$(SY_DIR)/cm_xta_ldfsm.$(OBJ)    \
	$(SY_DIR)/cm_xta_xml.$(OBJ)      \
	$(SY_DIR)/cm_xta_ex_ms.$(OBJ)    \
	$(SY_DIR)/cm_xta_tgfsm.$(OBJ)    \
	$(SY_DIR)/cm_xta_tafsm.$(OBJ)    \
	$(SY_DIR)/cm_xta_slave.$(OBJ)    \
	$(SY_DIR)/pjac_prof.$(OBJ)       \
	$(SY_DIR)/pjac_tst.$(OBJ)        \
	$(SY_DIR)/pjac_cpjreg.$(OBJ)     \
	$(SY_DIR)/pjac_cpjcb.$(OBJ)      \
	$(SY_DIR)/pjac_cpjhdl.$(OBJ)     \
	$(SY_DIR)/pjac_cpjutl.$(OBJ)     \
	$(SY_DIR)/pjac_kwureg.$(OBJ)     \
	$(SY_DIR)/pjac_kwucb.$(OBJ)      \
	$(SY_DIR)/pjac_kwuhdl.$(OBJ)     \
	$(SY_DIR)/pjac_kwuutl.$(OBJ)     \
	$(SY_DIR)/pjac_pjureg.$(OBJ)     \
	$(SY_DIR)/pjac_pjucb.$(OBJ)      \
	$(SY_DIR)/pjac_pjuhdl.$(OBJ)     \
	$(SY_DIR)/pjac_pjuutl.$(OBJ)     \
	$(SY_DIR)/pjac_nh_ptli.$(OBJ)    \
	$(SY_DIR)/pjac_nh_ex_ms.$(OBJ)   \
	$(SY_DIR)/pjac_kw_ul_ex_ms.$(OBJ)   \
	$(SY_DIR)/pjac_kw_dl_ex_ms.$(OBJ)   \
	$(SY_DIR)/pjac_px_ptli.$(OBJ)    \
	$(SY_DIR)/pjac_px_ex_ms.$(OBJ)   \
	$(SY_DIR)/pjac_ptli.$(OBJ)    \
	$(SY_DIR)/pjac_kw_ptui.$(OBJ)    \
	$(SY_DIR)/pjac_ex_ms.$(OBJ)   \
	$(SY_DIR)/pjac_lpjreg.$(OBJ)     \
	$(SY_DIR)/pjac_lpjcb.$(OBJ)      \
	$(SY_DIR)/pjac_lpjhdl.$(OBJ)     \
	$(SY_DIR)/pjac_lpjutl.$(OBJ)		\
	$(SY_DIR)/pjac_load.$(OBJ)		\
	$(SY_DIR)/pjac_py.$(OBJ)       \
	$(SY_DIR)/pjac_lib.$(OBJ)      \
	$(SY_DIR)/pjac_libcb.$(OBJ)    \
	$(SY_DIR)/pjac_libhdl.$(OBJ)   \
	$(SY_DIR)/pjac_libreg.$(OBJ)   \
	$(SY_DIR)/pjac_libutl.$(OBJ)   \
	$(SY_DIR)/pjac_lib_ex_ms.$(OBJ)   \
	$(SY_DIR)/pjac_lib_ptli.$(OBJ)   
	#$(SY_DIR)/pjac_lib.$(OBJ)      \
	$(SY_DIR)/pjac_libcb.$(OBJ)    \
	$(SY_DIR)/pjac_libhdl.$(OBJ)   \
	$(SY_DIR)/pjac_libreg.$(OBJ)   \
	$(SY_DIR)/pjac_libutl.$(OBJ)   \
	$(SY_DIR)/pjac_lib_ex_ms.$(OBJ)   \
	$(SY_DIR)/pjac_lib_ptli.$(OBJ)   \


#PJAC source for VX wroks compilation
PJACCM_SRCS=\
   $(CM_DIR)/cm_xta_slave.c 	\
	$(CM_DIR)/cm_xta_reg.c 		\
	$(CM_DIR)/cm_xta_ipc.c 		\
	$(PJACC_DIR)/pjac_prof.c 	\
	$(PJACC_DIR)/pjac_tst.c

PJACCM_OBJS=\
   $(PJACC_DIR)/cm_xta_slave.$(OBJ) \
	$(PJACC_DIR)/cm_xta_reg.$(OBJ) 	\
	$(PJACC_DIR)/cm_xta_ipc.$(OBJ) 	\
	$(PJACC_DIR)/pjac_prof.$(OBJ) 	\
	$(PJACC_DIR)/pjac_tst.$(OBJ)

#----------------------------------------- rlc id source
PJ_ID_SRCS= \
	$(PJ_DIR)/pj_id.c

#----------------------------------------- rlc id objects
PJ_ID_OBJS= \
	$(SY_DIR)/pj_id.$(OBJ)

#----------------------------------------- rlc id source
PJ_ID_SRCS= \
	$(PJ_DIR)/pj_id.c

#----------------------------------------- rlc id objects
PJ_ID_OBJS= \
	$(SY_DIR)/pj_id.$(OBJ)

#-------------pdcp  source

PJ_SRCS= \
   $(PJ_ID_SRCS)			\
	$(PJ_DIR)/pj_cfg_dl.c    \
	$(PJ_DIR)/pj_cfg_ul.c    \
	$(PJ_DIR)/pj_dbm_dl.c    \
	$(PJ_DIR)/pj_dbm_ul.c    \
	$(PJ_DIR)/pj_dlm.c    \
	$(PJ_DIR)/pj_lib.c    \
	$(PJ_DIR)/pj_lim.c    \
	$(PJ_DIR)/pj_lmm.c    \
	$(PJ_DIR)/pj_tmr.c    \
	$(PJ_DIR)/pj_uim.c    \
	$(PJ_DIR)/pj_ulm.c    \
	$(PJ_DIR)/pj_dl_utl.c    \
	$(PJ_DIR)/pj_ul_utl.c    \
	$(PJ_DIR)/pj_udx.c \
	$(PJ_DIR)/pj_udx_dl.c \
	$(PJ_DIR)/pj_udx_ul.c \
	$(PJ_DIR)/pj_udx_ptul.c \
   $(PJ_DIR)/pj_udx_ptdl.c \
   $(PJ_DIR)/pj_dl_ex_ms.c \
   $(PJ_DIR)/pj_ul_ex_ms.c \
	$(PJ_DIR)/pj_utl.c \
   $(PJ_DIR)/pj_dbm.c  \
	$(PJ_DIR)/pj_ptmi.c \
   $(PJ_DIR)/pj_ptui.c \
   $(PJ_DIR)/pj_ptli.c 
    
PJ_SRCS1= \
	$(PJ_DIR)/pj_ptlib.c

#---------------RL parent source
RL_SRCS=$(RL_DIR)/rl_rlog.c \
		  $(RL_DIR)/rl_common.c

#---------------RL OBJECTS
RL_OBJS=$(SY_DIR)/rl_rlog.$(OBJ) \
		  $(SY_DIR)/rl_common.$(OBJ)


#----------------------------------------- rlc objects

PJ_OBJS= \
	$(PJ_ID_OBJS)                \
	 $(SY_DIR)/pj_cfg_dl.$(OBJ)   \
	 $(SY_DIR)/pj_cfg_ul.$(OBJ)    \
	 $(SY_DIR)/pj_dbm_dl.$(OBJ)   \
	 $(SY_DIR)/pj_dbm_ul.$(OBJ)    \
	 $(SY_DIR)/pj_dlm.$(OBJ)    \
	 $(SY_DIR)/pj_lib.$(OBJ)    \
	 $(SY_DIR)/pj_lim.$(OBJ)    \
	 $(SY_DIR)/pj_lmm.$(OBJ)    \
	 $(SY_DIR)/pj_tmr.$(OBJ)    \
	 $(SY_DIR)/pj_uim.$(OBJ)    \
	 $(SY_DIR)/pj_ulm.$(OBJ)    \
	 $(SY_DIR)/pj_dl_utl.$(OBJ)    \
	 $(SY_DIR)/pj_ul_utl.$(OBJ)    \
	 $(SY_DIR)/pj_udx.$(OBJ) \
	 $(SY_DIR)/pj_udx_dl.$(OBJ) \
	 $(SY_DIR)/pj_udx_ul.$(OBJ) \
	 $(SY_DIR)/pj_udx_ptul.$(OBJ) \
	 $(SY_DIR)/pj_udx_ptdl.$(OBJ) \
    $(SY_DIR)/pj_dl_ex_ms.$(OBJ)\
    $(SY_DIR)/pj_ul_ex_ms.$(OBJ)\
    $(SY_DIR)/pj_utl.$(OBJ)  \
	 $(SY_DIR)/pj_dbm.$(OBJ)  \
	 $(SY_DIR)/pj_ptmi.$(OBJ) \
	 $(SY_DIR)/pj_ptui.$(OBJ) \
	 $(SY_DIR)/pj_ptli.$(OBJ)   
 
PJ_OBJS1= \
	$(SY_DIR)/pj_ptlib.$(OBJ)


PJ_SRCS= \
   $(PJ_ID_SRCS)			\
	$(PJ_DIR)/pj_cfg_dl.c    \
	$(PJ_DIR)/pj_cfg_ul.c    \
	$(PJ_DIR)/pj_dbm_dl.c    \
	$(PJ_DIR)/pj_dbm_ul.c    \
	$(PJ_DIR)/pj_dlm.c    \
	$(PJ_DIR)/pj_lib.c    \
	$(PJ_DIR)/pj_lim.c    \
	$(PJ_DIR)/pj_lmm.c    \
	$(PJ_DIR)/pj_tmr.c    \
	$(PJ_DIR)/pj_uim.c    \
	$(PJ_DIR)/pj_ulm.c    \
	$(PJ_DIR)/pj_dl_utl.c    \
	$(PJ_DIR)/pj_ul_utl.c    \
	$(PJ_DIR)/pj_udx.c \
	$(PJ_DIR)/pj_udx_dl.c \
	$(PJ_DIR)/pj_udx_ul.c \
	$(PJ_DIR)/pj_udx_ptul.c \
   $(PJ_DIR)/pj_udx_ptdl.c \
   $(PJ_DIR)/pj_dl_ex_ms.c \
   $(PJ_DIR)/pj_ul_ex_ms.c \
	$(PJ_DIR)/pj_utl.c \
   $(PJ_DIR)/pj_dbm.c  \
	$(PJ_DIR)/pj_ptmi.c \
   $(PJ_DIR)/pj_ptui.c \
   $(PJ_DIR)/pj_ptlib.c \
   $(PJ_DIR)/pj_ptli.c 


PJ_OBJS= \
	 $(PJ_ID_OBJS)                \
	 $(SY_DIR)/pj_cfg_dl.$(OBJ)   \
	 $(SY_DIR)/pj_cfg_ul.$(OBJ)    \
	 $(SY_DIR)/pj_dbm_dl.$(OBJ)   \
	 $(SY_DIR)/pj_dbm_ul.$(OBJ)    \
	 $(SY_DIR)/pj_dlm.$(OBJ)    \
	 $(SY_DIR)/pj_lib.$(OBJ)    \
	 $(SY_DIR)/pj_lim.$(OBJ)    \
	 $(SY_DIR)/pj_lmm.$(OBJ)    \
	 $(SY_DIR)/pj_tmr.$(OBJ)    \
	 $(SY_DIR)/pj_uim.$(OBJ)    \
	 $(SY_DIR)/pj_ulm.$(OBJ)    \
	 $(SY_DIR)/pj_dl_utl.$(OBJ)    \
	 $(SY_DIR)/pj_ul_utl.$(OBJ)    \
	 $(SY_DIR)/pj_udx.$(OBJ) \
	 $(SY_DIR)/pj_udx_dl.$(OBJ) \
	 $(SY_DIR)/pj_udx_ul.$(OBJ) \
	 $(SY_DIR)/pj_udx_ptul.$(OBJ) \
	 $(SY_DIR)/pj_udx_ptdl.$(OBJ) \
	 $(SY_DIR)/pj_dl_ex_ms.$(OBJ)\
	 $(SY_DIR)/pj_ul_ex_ms.$(OBJ)\
  	 $(SY_DIR)/pj_utl.$(OBJ)  \
	 $(SY_DIR)/pj_dbm.$(OBJ)  \
	 $(SY_DIR)/pj_ptmi.$(OBJ) \
	 $(SY_DIR)/pj_ptui.$(OBJ) \
    $(SY_DIR)/pj_ptlib.$(OBJ) \
	 $(SY_DIR)/pj_ptli.$(OBJ)   

#----------------------------------------- common source
PJCM_SRCS= \
	$(CM_DIR)/cpj.c     \
	$(CM_DIR)/kwu.c \
	$(CM_DIR)/pju.c \
	$(CM_DIR)/lpj.c     \
	$(CM_DIR)/cm_lte.c \
	$(CM_DIR)/cm_bdy5.c \
	$(CM_DIR)/cm_gen.c  \
	$(CM_DIR)/cm_tkns.c     \
	$(CM_DIR)/cm_lib.c  \
	$(CM_DIR)/cm_hash.c \
	$(CM_DIR)/cm_mblk.c \
	$(CM_DIR)/cm_inet.c \
	$(CM_DIR)/cm_llist.c  

PJCM_SRCS= \
	$(CM_DIR)/cpj.c     \
	$(CM_DIR)/kwu.c \
	$(CM_DIR)/pju.c \
	$(CM_DIR)/lpj.c     \
	$(CM_DIR)/cm_lte.c \
	$(CM_DIR)/cm_bdy5.c \
	$(CM_DIR)/cm_gen.c  \
	$(CM_DIR)/cm_tkns.c     \
	$(CM_DIR)/cm_lib.c  \
	$(CM_DIR)/cm_hash.c \
	$(CM_DIR)/cm_mblk.c \
	$(CM_DIR)/cm_inet.c \
	$(CM_DIR)/cm_llist.c  
#----------------------------------------- common headers
PJCMH_SRCS= \
	$(CM_DIR)/cm5.h     \
	$(CM_DIR)/cm5.x     \
	$(CM_DIR)/cm_err.h  \
	$(CM_DIR)/cm_lib.x  \
	$(CM_DIR)/envopt.h  \
	$(CM_DIR)/envdep.h  \
	$(CM_DIR)/envind.h  \
	$(CM_DIR)/gen.h     \
	$(CM_DIR)/gen.x     \
	$(CM_DIR)/lpj.h     \
	$(CM_DIR)/lpj.x     \
	$(CM_DIR)/kwu.h     \
	$(CM_DIR)/kwu.x     \
	$(CM_DIR)/pju.h     \
	$(CM_DIR)/pju.x     \
	$(CM_DIR)/ckw.h     \
	$(CM_DIR)/ckw.x     \
	$(CM_DIR)/cpj.h     \
	$(CM_DIR)/cpj.x     \
	$(CM_DIR)/ssi.h     \
	$(CM_DIR)/ssi.x

#----------------------------------------- common object
PJCM_OBJS=\
	$(SY_DIR)/cpj.$(OBJ)     \
	$(SY_DIR)/kwu.$(OBJ) \
	$(SY_DIR)/pju.$(OBJ) \
	$(SY_DIR)/lpj.$(OBJ)     \
	$(SY_DIR)/cm_lte.$(OBJ) \
	$(SY_DIR)/cm_bdy5.$(OBJ) \
	$(SY_DIR)/cm_gen.$(OBJ)  \
	$(SY_DIR)/cm_tkns.$(OBJ)     \
	$(SY_DIR)/cm_lib.$(OBJ)  \
	$(SY_DIR)/cm_mblk.$(OBJ) \
	$(SY_DIR)/cm_hash.$(OBJ) \
	$(SY_DIR)/cm_inet.$(OBJ) \
	$(SY_DIR)/cm_llist.$(OBJ)  

PJCM_OBJS=\
	$(SY_DIR)/cpj.$(OBJ)     \
	$(SY_DIR)/kwu.$(OBJ) \
	$(SY_DIR)/pju.$(OBJ) \
	$(SY_DIR)/lpj.$(OBJ)     \
	$(SY_DIR)/cm_lte.$(OBJ) \
	$(SY_DIR)/cm_bdy5.$(OBJ) \
	$(SY_DIR)/cm_gen.$(OBJ)  \
	$(SY_DIR)/cm_tkns.$(OBJ)     \
	$(SY_DIR)/cm_lib.$(OBJ)  \
	$(SY_DIR)/cm_mblk.$(OBJ) \
	$(SY_DIR)/cm_hash.$(OBJ) \
	$(SY_DIR)/cm_inet.$(OBJ) \
	$(SY_DIR)/cm_llist.$(OBJ)  
#----------------------------------------- common stack manager source
SM_SRCS=\
	$(SM_DIR)/sm_bdy1.c \
	$(SM_DIR)/sm_ex_ms.c 

#----------------------------------------- common stack manager objects
SM_OBJS= \
	$(SY_DIR)/sm_bdy1.$(OBJ) \
	$(SY_DIR)/sm_ex_ms.$(OBJ) 

#----------------------------------------- product stack manager source
PJSM_SRCS= \
	$(SM_DIR)/sm_pj_ex_ms.c \
	$(SM_DIR)/sm_pj_ptmi.c

#----------------------------------------- product stack manager objects
PJSM_OBJS= \
	$(SY_DIR)/sm_pj_ex_ms.$(OBJ) \
	$(SY_DIR)/sm_pj_ptmi.$(OBJ)

#----------------------------------------- os - mos source
MS_SRCS= \
	$(OS_DIR)/ms_id.c    \
	$(OS_DIR)/ms_bdy1.c  \
	$(OS_DIR)/ms_bdy2.c  \
	$(CM_DIR)/cm_ss.c    \
	$(OS_DIR)/ms_ex_ms.c \
	$(OS_DIR)/ms_ptmi.c

#----------------------------------------- mos objects
MS_OBJS= \
	$(SY_DIR)/ms_id.$(OBJ)    \
	$(SY_DIR)/ms_bdy1.$(OBJ)  \
	$(SY_DIR)/ms_bdy2.$(OBJ)  \
	$(SY_DIR)/cm_ss.$(OBJ)    \
	$(SY_DIR)/ms_ex_ms.$(OBJ) \
	$(SY_DIR)/ms_ptmi.$(OBJ)

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

SS_OBJS=  \
	$(SY_DIR)/ss_gen.$(OBJ) \
	$(SY_DIR)/ss_task.$(OBJ) \
	$(SY_DIR)/ss_drvr.$(OBJ) \
	$(SY_DIR)/ss_timer.$(OBJ) \
	$(SY_DIR)/ss_mem.$(OBJ) \
	$(SY_DIR)/ss_strm.$(OBJ) \
	$(SY_DIR)/ss_msg.$(OBJ) \
	$(SY_DIR)/ss_queue.$(OBJ) \
	$(SY_DIR)/ss_pack.$(OBJ) \
	$(SY_DIR)/ss_rtr.$(OBJ) 

# OS - mtss source-------------------------------------------------------
MT_SRCS=$(MT_DIR)/mt_ss.c \
        $(MT_DIR)/mt_id.c \
    $(MT_DIR)/cm_mem.c

# OS - mtss objects-------------------------------------------------------
MT_OBJS=$(SY_DIR)/mt_ss.$(OBJ) \
        $(SY_DIR)/mt_id.$(OBJ) \
    $(SY_DIR)/cm_mem.$(OBJ)

# OS - VxWorks source---------------------------------------------------
VW_SRCS= \
    $(VW_DIR)/vw_ss.c \
    $(VW_DIR)/cm_mem.c \
    $(VW_DIR)/vw_id.c

# OS - VxWorks objects--------------------------------------------------
VW_OBJS= \
    $(VW_DIR)/vw_ss.$(OBJ) \
    $(VW_DIR)/cm_mem.$(OBJ) \
    $(VW_DIR)/vw_id.$(OBJ)

# OS - WinNT source-----------------------------------------------------
NS_SRCS= \
    $(SS_DIR)/ns_gen.c \
    $(SS_DIR)/smnsexms.c \
    $(SS_DIR)/smnsbdy1.c \
    $(SS_DIR)/ns_ex_ms.c \
    $(SS_DIR)/ns_id.c \
    $(SS_DIR)/ns_ptmi.c \
    $(SS_DIR)/ns_space.c \
    $(SS_DIR)/ns_task.c \
    $(SS_DIR)/ns_timer.c \
        $(SS_DIR)/cm_mem.c

# OS - WinNT objects-----------------------------------------------------
NS_OBJS= \
    $(SY_DIR)/ns_gen.$(OBJ) \
    $(SY_DIR)/smnsexms.$(OBJ) \
    $(SY_DIR)/smnsbdy1.$(OBJ) \
    $(SY_DIR)/ns_ex_ms.$(OBJ) \
    $(SY_DIR)/ns_id.$(OBJ) \
    $(SY_DIR)/ns_ptmi.$(OBJ) \
    $(SY_DIR)/ns_space.$(OBJ) \
    $(SY_DIR)/ns_task.$(OBJ) \
    $(SY_DIR)/ns_timer.$(OBJ) \
        $(SY_DIR)/cm_mem.$(OBJ)


#----------------------------------------- portable build source
PT_SRCS= \
	$(PJ_DIR)/pj_tst.c   \
	$(CM_DIR)/ss_ptsp.c

#----------------------------------------- portable build objects
PT_OBJS= \
	$(SY_DIR)/pj_tst.$(OBJ)   \
	$(SY_DIR)/ss_ptsp.$(OBJ)


#----------------------------------------- all acceptance test source
#replace MT_SRCS with VW_SRCS if using vx works or NT_SRCS if using windows NT
#replace  PJAC_SRCS  with PJACCM_SRCS if using vx works 
PRDACC_SRCS= 		\
	$(PJ_SRCS)    	\
	$(PJ_SRCS1)   	\
	$(RL_SRCS)   	\
	$(PJAC_SRCS) 	\
	$(PJCM_SRCS)  	\
	$(SM_SRCS)    	\
	$(PJSM_SRCS)  	\
	$(SS_SRCS)   	\
	$(MT_SRCS)

#   $(PJACCM_SRCS)
#    $(MT_SRCS)
#   $(NS_SRCS)
#   $(VW_SRCS)


#----------------------------------------- all acceptance test objects
#replace MT_OBJS with VW_OBJS if using vx works or NT_OBJS if using windows NT
#replace  PJAC_OBJS  with PJACCM_OBJS if using vx works 
PRDACC_OBJS= 		\
	$(PJ_OBJS)    	\
	$(PJ_OBJS1)    	\
	$(RL_OBJS)    	\
	$(SM_OBJS)  	\
	$(PJSM_OBJS)  	\
	$(PJCM_OBJS)  	\
	$(PJAC_OBJS) 	\
	$(SS_OBJS) 		\
	$(MT_OBJS)

#	$(VW_OBJS)
#	$(NS_OBJS)
#	$(MT_OBJS)
#	$(PJACCM_OBJS)
# 
#----------------------------------------- all acceptance test source
#replace MT_SRCS with VW_SRCS if using vx works or NT_SRCS if using windows NT
#replace  PJAC_SRCS  with PJACCM_SRCS if using vx works 
PRDINT_SRCS= \
	$(PJ_SRCS1)    \
	$(SM_SRCS)    \
	$(PJSM_SRCS)  \
	$(PJAC_SRCS) \
	$(PJCM_SRCS)  \
	$(SS_SRCS)   \
	$(GP_SRCS)    \
	$(MT_SRCS)


#----------------------------------------- all acceptance test objects
#replace MT_OBJS with VW_OBJS if using vx works or NT_OBJS if using windows NT
#replace  PJAC_OBJS  with PJACCM_OBJS if using vx works 
PRDINT_OBJS= 	\
	$(SM_OBJS)  \
	$(PJSM_OBJS)\
	$(SS_OBJS) 	\
	$(EQ_OBJS)  \
	$(PJ_OBJS1)  \
	$(PJAC_OBJS)\
	$(PJCM_OBJS)\
	$(MT_OBJS)


#----------------------------------------- all portable source
PRDPT_SRCS= \
	$(PJ_SRCS) \
	$(PT_SRCS) \
	$(PJCM_SRCS)

#----------------------------------------- all portable objects
PRDPT_OBJS= \
	$(PJ_OBJS) \
	$(PT_OBJS) \
	$(PJCM_OBJS)

#----------------------------------------- Klocwork analysis objects
PRDKW_OBJS= \
	$(PJ_OBJS) \
	$(PJ_OBJS1) \
	$(PT_OBJS) 

# all library sources--------------------------------------------------
PRDLIB_SRCS=  $(PJ_SRCS) $(PJ_SRCS1)

# all library objects--------------------------------------------------
PRDLIB_OBJS=  $(PJ_OBJS) $(PJ_OBJS1)


#=======================================================================
# SECTION 8
#
# special programs required for Internal Development by makefile
#
# adjust as required
#
# System Binary Dir - replace with appropriate directory and programe
# for system
BIN=/bin
MAKEDEPEND=makedepend
REMOVE=$(BIN)/rm -f
DELETE=del


#=======================================================================
# SECTION 9
#
# build targets
#

# clean-----------------------------------------------------------------
#
# cleans up everything...
#
clean:
	$(REMOVE) *.$(OBJ) $(BINACC) $(BINPT) *.ln *.s *.i
 
win_clean:
	$(DELETE) *.$(OBJ)
	$(DELETE) $(BINACC).exe
	$(DELETE) $(BINPT).exe
	$(DELETE) *.ln
	$(DELETE) *.s

# External target for acceptance test build
# replace MTOSOPTS with appropriate OSOPTS
acc:
	@$(MAKE) -f $(MAKENAME) $(BLDENV) INC_PATH=$(ACC_INC_PATH) \
	 OUTNAME=$(BINACC)
 
int:
	@$(MAKE) -f $(MAKENAME) $(BLDENV) INC_PATH=$(ACC_INC_PATH) \
	OUTNAME=$(BINACC)

# External target for portable build
pt:
	@$(MAKE) -f $(MAKENAME) $(BLDENV) OUTNAME=$(BINPT) OSOPTS=$(PTOSOPTS) \
	INC_PATH=$(PT_INC_PATH)

# External target for portable build
lib:
	@$(MAKE) -f $(MAKENAME) $(BLDENV) OUTNAME=$(BINLIB)  \
    OSOPTS=$(LIBOSOPTS) INC_PATH=$(LIB_INC_PATH)

# External target for portable build
pj:
	@$(MAKE) -f $(MAKENAME) $(BLDENV) OUTNAME=$(BINPJ) OSOPTS=$(PTOSOPTS) \
   INC_PATH=$(PT_INC_PATH)

#==================================================================
# Portable Target builds for Solaris CC, Solaris GNU, MRI mcc68k on
# Solaris and Microsoft Visual C++ for WINNT/WIN95
#
# Solaris CC portable build---------------------------------------------
#
$(BINSOLPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(SOLENV) CC=$(SOLCC)  \
	LD=$(SOLLD) COPTS=$(SOLCOPTS) LOPTS=$(SOLPTLOPTS) LIBS=$(SOLLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)
#
# Solaris GNU portable build--------------------------------------------
#
$(BINGNUPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(GNUENV) CC=$(GNUCC)  \
	LD=$(GNULD) COPTS=$(GNUCOPTS) LOPTS=$(GNUPTLOPTS) LIBS=$(GNULIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)
#
# Linux GNU portable build--------------------------------------------
$(BINLNXPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(LNXENV) CC=$(LNXCC) \
	LD=$(LNXLD) COPTS=$(LNXCOPTS)  LIBS=$(LNXLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX) LOPTS=$(LNXPTLOPTS) 
#
# Microsoft Visual C++ for WINNT/WIN95 portable build-------------------
$(BINWINPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(WINENV) CC=$(WINCC)  \
	LD=$(WINLD) COPTS=$(WINCOPTS) LOPTS=$(WINLOPTS) LIBS=$(WINLIBS) \
	OBJ=$(NS_OBJ_SUFFIX)
#
# MicroTec Research Compiler in Solaris Environment---------------------
$(BINMRIPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(MRIENV) CC=$(MRICC) \
	LD=$(MRILD) COPTS=$(MRICOPTS) LOPTS=$(MRILOPTS) LIBS=$(MRILIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)
#
# Acceptance test target builds for Solaris CC, Solaris GNU,
# Inisite Memory Check tool and Microsoft Visual C++ for WINNT/WIN95
#
# Solaris CC acceptance test build--------------------------------------
#
$(BINSOLACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(SOLENV) CC=$(SOLCC) \
	LD=$(SOLLD) COPTS=$(SOLCOPTS) LOPTS=$(SOLLOPTS) LIBS=$(SOLLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)
$(BINSOLINT):
	@$(MAKE) -f $(MAKENAME) $(MAKEINT) -e ENV=$(SOLENV) CC=$(SOLCC) \
	LD=$(SOLLD) COPTS=$(SOLCOPTS) LOPTS=$(SOLLOPTS) LIBS=$(SOLLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)
#
# Solaris GNU acceptance test build-------------------------------------
#
$(BINGNUACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(GNUENV) CC=$(GNUCC) \
	LD=$(GNULD) COPTS=$(GNUCOPTS) LOPTS=$(GNULOPTS) LIBS=$(GNULIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)
$(BINGNUINT):
	@$(MAKE) -f $(MAKENAME) $(MAKEINT) -e ENV=$(GNUENV) CC=$(GNUCC) \
	LD=$(GNULD) COPTS=$(GNUCOPTS) LOPTS=$(GNULOPTS) LIBS=$(GNULIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)

# Solaris GPP(g++) acceptance test build-------------------------------------
#
$(BINGPPACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(GPPENV) CC=$(GPPCC) \
	LD=$(GPPLD) COPTS=$(GPPCOPTS) LOPTS=$(GPPLOPTS) LIBS=$(GPPLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)
$(BINGPPINT):
	@$(MAKE) -f $(MAKENAME) $(MAKEINT) -e ENV=$(GPPENV) CC=$(GPPCC) \
	LD=$(GPPLD) COPTS=$(GPPCOPTS) LOPTS=$(GPPLOPTS) LIBS=$(GPPLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)
$(BINGPPPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(GPPENV) CC=$(GPPCC) \
	LD=$(GPPLD) COPTS=$(GPPCOPTS) LOPTS=$(GPPPTLOPTS) LIBS=$(GPPLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)
$(BINGPPLIB):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB) -e ENV=$(GPPENV) CC=$(GPPCC) \
	LD=$(GPPLIBLD) COPTS=$(GPPCOPTS) LOPTS=$(GPPLIBOPTS) LIBS=$(GPPLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)

# Solaris GNU acceptance test build-------------------------------------
#
$(BINCPPACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(CPPENV) CC=$(CPPCC) \
	LD=$(CPPLD) COPTS=$(CPPCOPTS) LOPTS=$(CPPLOPTS) LIBS=$(CPPLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)
$(BINCPPINT):
	@$(MAKE) -f $(MAKENAME) $(MAKEINT) -e ENV=$(CPPENV) CC=$(CPPCC) \
	LD=$(CPPLD) COPTS=$(CPPCOPTS) LOPTS=$(CPPLOPTS) LIBS=$(CPPLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)
$(BINCPPPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(CPPENV) CC=$(CPPCC) \
	LD=$(CPPLD) COPTS=$(CPPCOPTS) LOPTS=$(CPPPTLOPTS) LIBS=$(CPPLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)
$(BINCPPLIB):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB) -e ENV=$(CPPENV) CC=$(CPPCC) \
	LD=$(CPPLIBLD) COPTS=$(CPPCOPTS) LOPTS=$(CPPLIBOPTS) LIBS=$(CPPLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)

#
# Linux GNU acceptance test build-------------------------------------
$(BINLNXACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(LNXENV) CC=$(LNXCC) \
	LD=$(LNXLD)   LIBS=$(LNXLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX) LOPTS=$(LNXLOPTS) COPTS=$(LNXCOPTS)
$(BINLNXINT):
	@$(MAKE) -f $(MAKENAME) $(MAKEINT) -e ENV=$(LNXENV) CC=$(LNXCC) \
	LD=$(LNXLD)   LIBS=$(LNXLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX) LOPTS=$(LNXLOPTS) COPTS=$(LNXCOPTS)

# Linux purecov acceptance test build-------------------------------------
$(BINLNXPACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(LNXENV) CC=$(PLNXCC) \
	LD=$(PLNXLD)   LIBS=$(LNXLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX) LOPTS=$(LNXLOPTS) COPTS=$(LNXCOPTS)

# Linux purify acceptance test build-------------------------------------
$(BINPURACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(INSENV) CC=$(PURCC) \
	LD=$(PURLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLOPTS) LIBS=$(LNXLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)

#Linux GNU lib build---------------------------------------------------
$(BINLNXLIB):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB) -e ENV=$(LNXENV) CC=$(LNXCC) \
	LD=$(LNXLIBLD)   LIBS=$(LNXLIBS) OSOPTS=$(LNXLIBOSOPTS) \
	OBJ=$(SOL_OBJ_SUFFIX) LOPTS=$(LNXLIBOPTS) COPTS=$(LNXCOPTS) \
	INC_PATH=$(LIB_INC_PATH)


#
# Microsoft Visual C++ for WINNT/WIN95 acceptance test build------------
$(BINWINACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(WINENV) CC=$(WINCC) \
	LD=$(WINLD) COPTS=$(WINCOPTS) LOPTS=$(WINLOPTS) LIBS=$(WINLIBS) \
	OBJ=$(NS_OBJ_SUFFIX) OS_SRCS=$(NS_SRCS) OS_OBJS=$(NS_OBJS) OS_DIR=$(NS_DIR)
$(BINWININT):
	@$(MAKE) -f $(MAKENAME) $(MAKEINT) -e ENV=$(WINENV) CC=$(WINCC) \
	LD=$(WINLD) COPTS=$(WINCOPTS) LOPTS=$(WINLOPTS) LIBS=$(WINLIBS) \
	OBJ=$(MS_OBJ_SUFFIX) OS_OBJS=$(NS_OBJS) OS_DIR=$(NS_DIR)
#
# Inisite Memory Check Toolin Solaris acceptance test build-------------
$(BININSITEACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(INSENV) CC=$(INSCC) \
	LD=$(INSLD) COPTS=$(INSCOPTS) LOPTS=$(INSLOPTS) LIBS=$(INSLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)
$(BININSITEINT):
	@$(MAKE) -f $(MAKENAME) $(MAKEINT) -e ENV=$(INSENV) CC=$(INSCC) \
	LD=$(INSLD) COPTS=$(INSCOPTS) LOPTS=$(INSLOPTS) LIBS=$(INSLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)

#
# Solaris GNU acceptance test build-------------------------------------
#
$(BINPPCACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e "ENV=$(PPCENV)" CC=$(PPCCC) \
	LD=$(PPCLD) "COPTS=$(PPCCOPTS)" "LOPTS=$(PPCLOPTS)" LIBS=$(PPCLIBS)\
	OBJ=$(SOL_OBJ_SUFFIX)
$(BINPPCINT):
	@$(MAKE) -f $(MAKENAME) $(MAKEINT) -e ENV=$(PPCENV) CC=$(PPCCC) \
	LD=$(PPCLD) COPTS=$(PPCCOPTS) LOPTS=$(PPCLOPTS) LIBS=$(PPCLIBS)

#
# Vxworks cc386 acceptance test build on Pentium Target----------------
#
$(BINVWACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(VWINTENV) CC=$(VWINTCC) \
	LD=$(VWINTLD) COPTS=$(VWINTCOPTS) LOPTS=$(VWINTLOPTS) LIBS=$(VWINTLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(VWOSOPTS)" OS_SRCS=$(VW_SRCS) \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS=$(VW_OBJS) OS_DIR=$(VW_DIR)

 
# portable--------------------------------------------------------------
#
# The following assumes that the compile and linker options are
# set in the make-file before the definition of this target
#
$(MAKEPT):
	@$(MAKE) -f $(MAKENAME) $(PTBIN) CPRDOPTS=$(CPRDPTOPTS) \
	CSMOPTS=$(CSMPTOPTS)
 
$(MAKEPJ):
	@$(MAKE) -f $(MAKENAME) $(PJBIN) CPRDOPTS=$(CPRDPTOPTS) \
	CSMOPTS=$(CSMPTOPTS)
 
$(PTBIN): $(PRDPT_OBJS)
	$(LD) $(LOPTS) $(PRDPT_OBJS) $(LIBS)
 
$(PJBIN): $(PRDPJ_OBJS)
	$(LD) $(LOPTS) $(PRDPJ_OBJS) $(LIBS)
 
$(LIBBIN): $(PRDLIB_OBJS)
	$(LD) $(LOPTS) $(PRDLIB_OBJS) $(LIBS)

#
# Library target builds for Solaris CC &  Solaris GNU
#
# Solaris CC library build--------------------------------------
#
$(BINSOLLIB):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB) -e ENV=$(SOLENV) CC=$(SOLCC) \
    LD=$(SOLLIBLD) COPTS=$(SOLCOPTS) LOPTS=$(SOLLIBOPTS) LIBS=$(SOLLIBS) \
    OBJ=$(SOL_OBJ_SUFFIX)
#
# Solaris GNU library build-------------------------------------
#
$(BINGNULIB):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB) -e ENV=$(GNUENV) CC=$(GNUCC) \
    LD=$(GNULIBLD) COPTS=$(GNUCOPTS) LOPTS=$(GNULIBOPTS) LIBS=$(GNULIBS) \
    OBJ=$(SOL_OBJ_SUFFIX)

#
# Window NT library build-------------------------------------
#
$(BINWINLIB):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB) -e ENV=$(WINENV) CC=$(WINCC) \
    LD=$(WINLIBLD) COPTS=$(WINCOPTS) LOPTS=$(WINLIBOPTS) LIBS=$(WINLIBS) \
    OBJ=$(WIN_OBJ_SUFFIX)

#
# Solaris  PJ build--------------------------------------------
#
$(BINGNUPJ):
	@$(MAKE) -f $(MAKENAME) $(MAKEPJ) -e ENV=$(GNUENV) CC=$(GNUCC) \
    LD=$(GNULD) COPTS=$(GNUCOPTS) LOPTS=$(GNULOPTS) LIBS=$(GNULIBS) \
    OBJ=$(SOL_OBJ_SUFFIX)

# acceptance test-------------------------------------------------------
#
# The following assumes that the compile and linker options are
# set in the make-file before the definition of this target
#
#For VXWORKS
#$(MAKEACC):
#	@$(MAKE) -f $(MAKENAME) $(ACCBIN) CPRDOPTS=$(CPRDACCOPTS) \
#	CSMOPTS=$(CSPJCCOPTS) VWCSMOPTS=$(VWINTACCOPTS)

$(MAKEACC):
	@$(MAKE) -f $(MAKENAME) $(ACCBIN) CPRDOPTS=$(CPRDACCOPTS) \
	CSMOPTS=$(CSPJCCOPTS)
 
$(MAKEINT):
	@$(MAKE) -f $(MAKENAME) $(INTBIN) CPRDOPTS=$(CPRDINTOPTS) \
	CSMOPTS=$(CSPJCCOPTS)

$(ACCBIN): $(PRDACC_OBJS)
	$(LD)  $(PRDACC_OBJS) $(LIBS) $(LOPTS)

$(INTBIN): $(PRDINT_OBJS)
	$(LD)  $(PRDINT_OBJS) $(LIBS) $(LOPTS)

$(MAKELIB):
	@$(MAKE) -f $(MAKENAME) $(LIBBIN) CPRDOPTS=$(CPRDLIBOPTS) \
    CSMOPTS=$(CSMLIBOPTS)

 
#
# Make Depend
# depend portable test--------------------------------------------------
# dependency for portable source
depend_pt: $(PRDPT_SRCS)
	@$(MAKE) -f $(MAKENAME) $(DEPEND) -e OSOPTS=$(PTOSOPTS) \
	DEPOPTS=$(DEPOPTS_PT) INC_PATH=$(PT_INC_PATH) DEP_SRCS="$(PRDPT_SRCS)"
 
# depend acceptance test------------------------------------------------
# dependency for acceptance test
depend_acc: $(PRDACC_SRCS)
	@$(MAKE) -f $(MAKENAME) $(DEPEND) -e OSOPTS=$(MTOSOPTS) \
	DEPOPTS=$(DEPOPTS_ACC) INC_PATH=$(ACC_INC_PATH) DEP_SRCS="$(PRDACC_SRCS)"
 
depend_int: $(PRDINT_SRCS)
	@$(MAKE) -f $(MAKENAME) $(DEPEND) -e OSOPTS=$(MTOSOPTS) \
	DEPOPTS=$(DEPOPTS_INT) INC_PATH=$(ACC_INC_PATH) DEP_SRCS="$(PRDINT_SRCS)"

# Build the dependency
$(DEPEND):
	$(MAKEDEPEND) -f $(MAKENAME) $(OSOPTS) $(DEPOPTS) $(INC_PATH) $(DEP_SRCS)


#=======================================================================
#
# Source code build directives
# SECTION 10


#---------------------------- RLC objects ---------------------------

$(SY_DIR)/pj_id.$(OBJ): $(PJ_DIR)/pj_id.c $(SYS_INC) $(CM_INC) 
	$(CC) -c $(CPRDOPTS)  $(PJ_DIR)/pj_id.c -o $(SY_DIR)/pj_id.$(OBJ)

$(SY_DIR)/pj_ptlib.$(OBJ): $(PJ_DIR)/pj_ptlib.c $(SYS_INC) $(CM_INC) 
	$(CC) -c $(CPRDOPTS)  $(PJ_DIR)/pj_ptlib.c -o $(SY_DIR)/pj_ptlib.$(OBJ)

$(SY_DIR)/pj_cfg_dl.$(OBJ): $(PJ_DIR)/pj_cfg_dl.c $(SYS_INC) $(CM_INC) 
	$(CC) -c $(CPRDOPTS)  $(PJ_DIR)/pj_cfg_dl.c -o $(SY_DIR)/pj_cfg_dl.$(OBJ)

$(SY_DIR)/pj_cfg_ul.$(OBJ): $(PJ_DIR)/pj_cfg_ul.c $(SYS_INC) $(CM_INC) 
	$(CC) -c $(CPRDOPTS)  $(PJ_DIR)/pj_cfg_ul.c -o $(SY_DIR)/pj_cfg_ul.$(OBJ)

$(SY_DIR)/pj_dbm.$(OBJ): $(PJ_DIR)/pj_dbm.c $(SYS_INC) $(CM_INC) 
	$(CC) -c $(CPRDOPTS)  $(PJ_DIR)/pj_dbm.c -o $(SY_DIR)/pj_dbm.$(OBJ)

$(SY_DIR)/pj_dbm_dl.$(OBJ): $(PJ_DIR)/pj_dbm_dl.c $(SYS_INC) $(CM_INC) 
	$(CC) -c $(CPRDOPTS)  $(PJ_DIR)/pj_dbm_dl.c -o $(SY_DIR)/pj_dbm_dl.$(OBJ)
$(SY_DIR)/pj_dbm_ul.$(OBJ): $(PJ_DIR)/pj_dbm_ul.c $(SYS_INC) $(CM_INC) 
	$(CC) -c $(CPRDOPTS)  $(PJ_DIR)/pj_dbm_ul.c -o $(SY_DIR)/pj_dbm_ul.$(OBJ)

$(SY_DIR)/pj_dlm.$(OBJ): $(PJ_DIR)/pj_dlm.c $(SYS_INC) $(CM_INC) 
	$(CC) -c $(CPRDOPTS)  $(PJ_DIR)/pj_dlm.c -o $(SY_DIR)/pj_dlm.$(OBJ)

$(SY_DIR)/pj_lib.$(OBJ): $(PJ_DIR)/pj_lib.c $(SYS_INC) $(CM_INC) 
	$(CC) -c $(CPRDOPTS)  $(PJ_DIR)/pj_lib.c -o $(SY_DIR)/pj_lib.$(OBJ)

$(SY_DIR)/pj_lim.$(OBJ): $(PJ_DIR)/pj_lim.c $(SYS_INC) $(CM_INC) 
	$(CC) -c $(CPRDOPTS)  $(PJ_DIR)/pj_lim.c -o $(SY_DIR)/pj_lim.$(OBJ)

$(SY_DIR)/pj_lmm.$(OBJ): $(PJ_DIR)/pj_lmm.c $(SYS_INC) $(CM_INC) 
	$(CC) -c $(CPRDOPTS)  $(PJ_DIR)/pj_lmm.c -o $(SY_DIR)/pj_lmm.$(OBJ)

$(SY_DIR)/pj_tmr.$(OBJ): $(PJ_DIR)/pj_tmr.c $(SYS_INC) $(CM_INC) 
	$(CC) -c $(CPRDOPTS)  $(PJ_DIR)/pj_tmr.c -o $(SY_DIR)/pj_tmr.$(OBJ)

$(SY_DIR)/pj_uim.$(OBJ): $(PJ_DIR)/pj_uim.c $(SYS_INC) $(CM_INC) 
	$(CC) -c $(CPRDOPTS)  $(PJ_DIR)/pj_uim.c -o $(SY_DIR)/pj_uim.$(OBJ)

$(SY_DIR)/pj_ulm.$(OBJ): $(PJ_DIR)/pj_ulm.c $(SYS_INC) $(CM_INC) 
	$(CC) -c $(CPRDOPTS)  $(PJ_DIR)/pj_ulm.c -o $(SY_DIR)/pj_ulm.$(OBJ)

$(SY_DIR)/pj_utl.$(OBJ): $(PJ_DIR)/pj_utl.c $(SYS_INC) $(CM_INC) 
	$(CC) -c $(CPRDOPTS)  $(PJ_DIR)/pj_utl.c -o $(SY_DIR)/pj_utl.$(OBJ)

$(SY_DIR)/pj_dl_utl.$(OBJ): $(PJ_DIR)/pj_dl_utl.c $(SYS_INC) $(CM_INC) 
	$(CC) -c $(CPRDOPTS)  $(PJ_DIR)/pj_dl_utl.c -o $(SY_DIR)/pj_dl_utl.$(OBJ)

$(SY_DIR)/pj_ul_utl.$(OBJ): $(PJ_DIR)/pj_ul_utl.c $(SYS_INC) $(CM_INC) 
	$(CC) -c $(CPRDOPTS)  $(PJ_DIR)/pj_ul_utl.c -o $(SY_DIR)/pj_ul_utl.$(OBJ)

$(SY_DIR)/pj_udx.$(OBJ): $(PJ_DIR)/pj_udx.c $(SYS_INC) $(CM_INC) 
	$(CC) -c $(CPRDOPTS)  $(PJ_DIR)/pj_udx.c -o $(SY_DIR)/pj_udx.$(OBJ)

$(SY_DIR)/pj_udx_dl.$(OBJ): $(PJ_DIR)/pj_udx_dl.c $(SYS_INC) $(CM_INC) 
	$(CC) -c $(CPRDOPTS)  $(PJ_DIR)/pj_udx_dl.c -o $(SY_DIR)/pj_udx_dl.$(OBJ)

$(SY_DIR)/pj_udx_ul.$(OBJ): $(PJ_DIR)/pj_udx_ul.c $(SYS_INC) $(CM_INC) 
	$(CC) -c $(CPRDOPTS)  $(PJ_DIR)/pj_udx_ul.c -o $(SY_DIR)/pj_udx_ul.$(OBJ)

$(SY_DIR)/pj_udx_ptul.$(OBJ): $(PJ_DIR)/pj_udx_ptul.c $(SYS_INC) $(CM_INC) 
	$(CC) -c $(CPRDOPTS)  $(PJ_DIR)/pj_udx_ptul.c -o $(SY_DIR)/pj_udx_ptul.$(OBJ)
$(SY_DIR)/pj_udx_ptdl.$(OBJ): $(PJ_DIR)/pj_udx_ptdl.c $(SYS_INC) $(CM_INC) 
	$(CC) -c $(CPRDOPTS)  $(PJ_DIR)/pj_udx_ptdl.c -o $(SY_DIR)/pj_udx_ptdl.$(OBJ)

$(SY_DIR)/pj_ul_ex_ms.$(OBJ): $(PJ_DIR)/pj_ul_ex_ms.c $(SYS_INC) $(CM_INC) 
	$(CC) -c $(CPRDOPTS)  $(PJ_DIR)/pj_ul_ex_ms.c -o $(SY_DIR)/pj_ul_ex_ms.$(OBJ)

$(SY_DIR)/pj_dl_ex_ms.$(OBJ): $(PJ_DIR)/pj_dl_ex_ms.c $(SYS_INC) $(CM_INC) 
	$(CC) -c $(CPRDOPTS)  $(PJ_DIR)/pj_dl_ex_ms.c -o $(SY_DIR)/pj_dl_ex_ms.$(OBJ)

$(SY_DIR)/pj_ptmi.$(OBJ): $(PJ_DIR)/pj_ptmi.c $(SYS_INC) $(CM_INC) 
	$(CC) -c $(CPRDOPTS)  $(PJ_DIR)/pj_ptmi.c -o $(SY_DIR)/pj_ptmi.$(OBJ)

$(SY_DIR)/pj_ptli.$(OBJ): $(PJ_DIR)/pj_ptli.c $(SYS_INC) $(CM_INC) 
	$(CC) -c $(CPRDOPTS)  $(PJ_DIR)/pj_ptli.c -o $(SY_DIR)/pj_ptli.$(OBJ)

$(SY_DIR)/pj_ptui.$(OBJ): $(PJ_DIR)/pj_ptui.c $(SYS_INC) $(CM_INC) 
	$(CC) -c $(CPRDOPTS)  $(PJ_DIR)/pj_ptui.c -o $(SY_DIR)/pj_ptui.$(OBJ)

# rlog files------------------------------------------------
$(SY_DIR)/rl_rlog.$(OBJ): $(RL_DIR)/rl_rlog.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(RL_DIR)/rl_rlog.c -o $(SY_DIR)/rl_rlog.$(OBJ)

$(SY_DIR)/rl_common.$(OBJ): $(RL_DIR)/rl_common.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(RL_DIR)/rl_common.c -o $(SY_DIR)/rl_common.$(OBJ)


#----------------------------------------- acceptance test objects

$(SY_DIR)/cm_xta_ex_ms.$(OBJ): $(PJACC_DIR)/cm_xta_ex_ms.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/cm_xta_ex_ms.c -o $(SY_DIR)/cm_xta_ex_ms.$(OBJ)

$(SY_DIR)/cm_xta_xml.$(OBJ): $(PJACC_DIR)/cm_xta_xml.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/cm_xta_xml.c -o $(SY_DIR)/cm_xta_xml.$(OBJ)

$(SY_DIR)/cm_xta_ldfsm.$(OBJ): $(PJACC_DIR)/cm_xta_ldfsm.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/cm_xta_ldfsm.c -o $(SY_DIR)/cm_xta_ldfsm.$(OBJ)

$(SY_DIR)/cm_xta_tcfsm.$(OBJ): $(PJACC_DIR)/cm_xta_tcfsm.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/cm_xta_tcfsm.c -o $(SY_DIR)/cm_xta_tcfsm.$(OBJ)

$(SY_DIR)/cm_xta_tgfsm.$(OBJ): $(PJACC_DIR)/cm_xta_tgfsm.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/cm_xta_tgfsm.c -o $(SY_DIR)/cm_xta_tgfsm.$(OBJ)

$(SY_DIR)/cm_xta_tafsm.$(OBJ): $(PJACC_DIR)/cm_xta_tafsm.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/cm_xta_tafsm.c -o $(SY_DIR)/cm_xta_tafsm.$(OBJ)

$(SY_DIR)/cm_xta_py.$(OBJ): $(PJACC_DIR)/cm_xta_py.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/cm_xta_py.c -o $(SY_DIR)/cm_xta_py.$(OBJ)

$(SY_DIR)/cm_xta_inthdl.$(OBJ): $(PJACC_DIR)/cm_xta_inthdl.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/cm_xta_inthdl.c -o $(SY_DIR)/cm_xta_inthdl.$(OBJ)

$(SY_DIR)/cm_xta_intreg.$(OBJ): $(PJACC_DIR)/cm_xta_intreg.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/cm_xta_intreg.c -o $(SY_DIR)/cm_xta_intreg.$(OBJ)

$(SY_DIR)/cm_xta_res.$(OBJ): $(PJACC_DIR)/cm_xta_res.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/cm_xta_res.c -o $(SY_DIR)/cm_xta_res.$(OBJ)

$(SY_DIR)/cm_xta_queue.$(OBJ): $(PJACC_DIR)/cm_xta_queue.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/cm_xta_queue.c -o $(SY_DIR)/cm_xta_queue.$(OBJ)

$(SY_DIR)/cm_xta_flag.$(OBJ): $(PJACC_DIR)/cm_xta_flag.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/cm_xta_flag.c -o $(SY_DIR)/cm_xta_flag.$(OBJ)

$(SY_DIR)/cm_xta_log.$(OBJ): $(PJACC_DIR)/cm_xta_log.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/cm_xta_log.c -o $(SY_DIR)/cm_xta_log.$(OBJ)

$(SY_DIR)/cm_xta_slave.$(OBJ): $(PJACC_DIR)/cm_xta_slave.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/cm_xta_slave.c -o $(SY_DIR)/cm_xta_slave.$(OBJ)

$(SY_DIR)/cm_xta_master.$(OBJ): $(PJACC_DIR)/cm_xta_master.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/cm_xta_master.c -o $(SY_DIR)/cm_xta_master.$(OBJ)

$(SY_DIR)/cm_xta_reg.$(OBJ): $(PJACC_DIR)/cm_xta_reg.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/cm_xta_reg.c -o $(SY_DIR)/cm_xta_reg.$(OBJ)

$(SY_DIR)/cm_xta_tst.$(OBJ): $(PJACC_DIR)/cm_xta_tst.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/cm_xta_tst.c -o $(SY_DIR)/cm_xta_tst.$(OBJ)

$(SY_DIR)/cm_xta_ipc.$(OBJ): $(PJACC_DIR)/cm_xta_ipc.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/cm_xta_ipc.c -o $(SY_DIR)/cm_xta_ipc.$(OBJ)

$(SY_DIR)/cm_xta_txn.$(OBJ): $(PJACC_DIR)/cm_xta_txn.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/cm_xta_txn.c -o $(SY_DIR)/cm_xta_txn.$(OBJ)

$(SY_DIR)/cm_xta_intutl.$(OBJ): $(PJACC_DIR)/cm_xta_intutl.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/cm_xta_intutl.c -o $(SY_DIR)/cm_xta_intutl.$(OBJ)

$(SY_DIR)/cm_xta_tmr.$(OBJ): $(PJACC_DIR)/cm_xta_tmr.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/cm_xta_tmr.c -o $(SY_DIR)/cm_xta_tmr.$(OBJ)


#CPJ Interface Rules

$(SY_DIR)/pjac_cpjreg.$(OBJ): $(PJACC_DIR)/pjac_cpjreg.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_cpjreg.c -o $(SY_DIR)/pjac_cpjreg.$(OBJ)

$(SY_DIR)/pjac_cpjcb.$(OBJ): $(PJACC_DIR)/pjac_cpjcb.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_cpjcb.c -o $(SY_DIR)/pjac_cpjcb.$(OBJ)

$(SY_DIR)/pjac_cpjhdl.$(OBJ): $(PJACC_DIR)/pjac_cpjhdl.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_cpjhdl.c -o $(SY_DIR)/pjac_cpjhdl.$(OBJ)

$(SY_DIR)/pjac_cpjutl.$(OBJ): $(PJACC_DIR)/pjac_cpjutl.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_cpjutl.c -o $(SY_DIR)/pjac_cpjutl.$(OBJ)

#PJU Interface
$(SY_DIR)/pjac_pjureg.$(OBJ): $(PJACC_DIR)/pjac_pjureg.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_pjureg.c -o $(SY_DIR)/pjac_pjureg.$(OBJ)

$(SY_DIR)/pjac_pjucb.$(OBJ): $(PJACC_DIR)/pjac_pjucb.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_pjucb.c -o $(SY_DIR)/pjac_pjucb.$(OBJ)

$(SY_DIR)/pjac_pjuhdl.$(OBJ): $(PJACC_DIR)/pjac_pjuhdl.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_pjuhdl.c -o $(SY_DIR)/pjac_pjuhdl.$(OBJ)

$(SY_DIR)/pjac_pjuutl.$(OBJ): $(PJACC_DIR)/pjac_pjuutl.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_pjuutl.c -o $(SY_DIR)/pjac_pjuutl.$(OBJ)


$(SY_DIR)/pjac_px_ptli.$(OBJ): $(PJACC_DIR)/pjac_px_ptli.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_px_ptli.c -o $(SY_DIR)/pjac_px_ptli.$(OBJ)

$(SY_DIR)/pjac_px_ex_ms.$(OBJ): $(PJACC_DIR)/pjac_px_ex_ms.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_px_ex_ms.c -o $(SY_DIR)/pjac_px_ex_ms.$(OBJ)


#KWU Interface
$(SY_DIR)/pjac_kwureg.$(OBJ): $(PJACC_DIR)/pjac_kwureg.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_kwureg.c -o $(SY_DIR)/pjac_kwureg.$(OBJ)

$(SY_DIR)/pjac_kwucb.$(OBJ): $(PJACC_DIR)/pjac_kwucb.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_kwucb.c -o $(SY_DIR)/pjac_kwucb.$(OBJ)

$(SY_DIR)/pjac_kwuhdl.$(OBJ): $(PJACC_DIR)/pjac_kwuhdl.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_kwuhdl.c -o $(SY_DIR)/pjac_kwuhdl.$(OBJ)

$(SY_DIR)/pjac_kwuutl.$(OBJ): $(PJACC_DIR)/pjac_kwuutl.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_kwuutl.c -o $(SY_DIR)/pjac_kwuutl.$(OBJ)


$(SY_DIR)/pjac_nh_ptli.$(OBJ): $(PJACC_DIR)/pjac_nh_ptli.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_nh_ptli.c -o $(SY_DIR)/pjac_nh_ptli.$(OBJ)

$(SY_DIR)/pjac_nh_ex_ms.$(OBJ): $(PJACC_DIR)/pjac_nh_ex_ms.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_nh_ex_ms.c -o $(SY_DIR)/pjac_nh_ex_ms.$(OBJ)

$(SY_DIR)/pjac_ptli.$(OBJ): $(PJACC_DIR)/pjac_ptli.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_ptli.c -o $(SY_DIR)/pjac_ptli.$(OBJ)

$(SY_DIR)/pjac_kw_ptui.$(OBJ): $(PJACC_DIR)/pjac_kw_ptui.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_kw_ptui.c -o $(SY_DIR)/pjac_kw_ptui.$(OBJ)


#LPJ Interface
$(SY_DIR)/pjac_lpjreg.$(OBJ): $(PJACC_DIR)/pjac_lpjreg.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_lpjreg.c -o $(SY_DIR)/pjac_lpjreg.$(OBJ)

$(SY_DIR)/pjac_lpjcb.$(OBJ): $(PJACC_DIR)/pjac_lpjcb.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_lpjcb.c -o $(SY_DIR)/pjac_lpjcb.$(OBJ)

$(SY_DIR)/pjac_lpjhdl.$(OBJ): $(PJACC_DIR)/pjac_lpjhdl.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_lpjhdl.c -o $(SY_DIR)/pjac_lpjhdl.$(OBJ)

$(SY_DIR)/pjac_lpjutl.$(OBJ): $(PJACC_DIR)/pjac_lpjutl.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_lpjutl.c -o $(SY_DIR)/pjac_lpjutl.$(OBJ)

#LIB Interface
$(SY_DIR)/pjac_lib.$(OBJ): $(PJACC_DIR)/pjac_lib.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_lib.c -o $(SY_DIR)/pjac_lib.$(OBJ)

$(SY_DIR)/pjac_libcb.$(OBJ): $(PJACC_DIR)/pjac_libcb.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_libcb.c -o $(SY_DIR)/pjac_libcb.$(OBJ)

$(SY_DIR)/pjac_libhdl.$(OBJ): $(PJACC_DIR)/pjac_libhdl.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_libhdl.c -o $(SY_DIR)/pjac_libhdl.$(OBJ)

$(SY_DIR)/pjac_libreg.$(OBJ): $(PJACC_DIR)/pjac_libreg.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_libreg.c -o $(SY_DIR)/pjac_libreg.$(OBJ)

$(SY_DIR)/pjac_libutl.$(OBJ): $(PJACC_DIR)/pjac_libutl.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_libutl.c -o $(SY_DIR)/pjac_libutl.$(OBJ)

$(SY_DIR)/kwac_lkwcb.$(OBJ): $(PJACC_DIR)/kwac_lkwcb.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/kwac_lkwcb.c -o $(SY_DIR)/kwac_lkwcb.$(OBJ)

$(SY_DIR)/kwac_lkwhdl.$(OBJ): $(PJACC_DIR)/kwac_lkwhdl.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/kwac_lkwhdl.c -o $(SY_DIR)/kwac_lkwhdl.$(OBJ)

$(SY_DIR)/kwac_lkwutl.$(OBJ): $(PJACC_DIR)/kwac_lkwutl.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/kwac_lkwutl.c -o $(SY_DIR)/kwac_lkwutl.$(OBJ)

$(SY_DIR)/pjac_load.$(OBJ): $(PJACC_DIR)/pjac_load.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_load.c -o $(SY_DIR)/pjac_load.$(OBJ)

$(SY_DIR)/pjac_flag.$(OBJ): $(PJACC_DIR)/pjac_flag.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_flag.c -o $(SY_DIR)/pjac_flag.$(OBJ)

$(SY_DIR)/pjac_py.$(OBJ): $(PJACC_DIR)/pjac_py.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_py.c -o $(SY_DIR)/pjac_py.$(OBJ)


$(SY_DIR)/pjac_lib_ex_ms.$(OBJ): $(PJACC_DIR)/pjac_lib_ex_ms.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_lib_ex_ms.c -o $(SY_DIR)/pjac_lib_ex_ms.$(OBJ)

$(SY_DIR)/pjac_lib_ptli.$(OBJ): $(PJACC_DIR)/pjac_lib_ptli.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_lib_ptli.c -o $(SY_DIR)/pjac_lib_ptli.$(OBJ)

#Some utility functions
$(SY_DIR)/pjac_prof.$(OBJ): $(PJACC_DIR)/pjac_prof.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_prof.c -o $(SY_DIR)/pjac_prof.$(OBJ)

$(SY_DIR)/pjac_kw_dl_ex_ms.$(OBJ): $(PJACC_DIR)/pjac_kw_dl_ex_ms.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_kw_dl_ex_ms.c -o $(SY_DIR)/pjac_kw_dl_ex_ms.$(OBJ)

$(SY_DIR)/pjac_kw_ul_ex_ms.$(OBJ): $(PJACC_DIR)/pjac_kw_ul_ex_ms.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_kw_ul_ex_ms.c -o $(SY_DIR)/pjac_kw_ul_ex_ms.$(OBJ)

$(SY_DIR)/pjac_ex_ms.$(OBJ): $(PJACC_DIR)/pjac_ex_ms.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_ex_ms.c -o $(SY_DIR)/pjac_ex_ms.$(OBJ)

#finally RLCAC Tst function
$(SY_DIR)/pjac_tst.$(OBJ): $(PJACC_DIR)/pjac_tst.c $(SYS_INC) $(PJACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(PJACC_DIR)/pjac_tst.c -o $(SY_DIR)/pjac_tst.$(OBJ)


#----------------------------------------- common objects---------------------

$(SY_DIR)/cpj.$(OBJ): $(CM_DIR)/cpj.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cpj.c

$(SY_DIR)/kwu.$(OBJ): $(CM_DIR)/kwu.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/kwu.c

$(SY_DIR)/pju.$(OBJ): $(CM_DIR)/pju.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/pju.c

$(SY_DIR)/lpj.$(OBJ): $(CM_DIR)/lpj.c  $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/lpj.c

$(SY_DIR)/cm_gen.$(OBJ): $(CM_DIR)/cm_gen.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cm_gen.c

#$(SY_DIR)/cm_tkns.$(OBJ): $(CM_DIR)/cm_tkns.c $(SYS_INC) $(LPJ_INC) $(CM_INC)
#	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cm_tkns.c -o $(SY_DIR)/cm_tkns.$(OBJ)

$(SY_DIR)/cm_tkns.$(OBJ): $(CM_DIR)/cm_tkns.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cm_tkns.c -o $(SY_DIR)/cm_tkns.$(OBJ)

$(SY_DIR)/cm_ss.$(OBJ): $(CM_DIR)/cm_ss.c  $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cm_ss.c

$(SY_DIR)/cm_lib.$(OBJ): $(CM_DIR)/cm_lib.c  $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cm_lib.c

$(SY_DIR)/cm_mblk.$(OBJ): $(CM_DIR)/cm_mblk.c  $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cm_mblk.c

$(SY_DIR)/cm_hash.$(OBJ): $(CM_DIR)/cm_hash.c  $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cm_hash.c

$(SY_DIR)/cm_lte.$(OBJ): $(CM_DIR)/cm_lte.c  $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cm_lte.c

$(SY_DIR)/cm_inet.$(OBJ): $(CM_DIR)/cm_inet.c  $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cm_inet.c

$(SY_DIR)/cm_llist.$(OBJ): $(CM_DIR)/cm_llist.c  $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cm_llist.c

$(SY_DIR)/cm_bdy5.$(OBJ): $(CM_DIR)/cm_bdy5.c  $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cm_bdy5.c

#----------------------------------------- mos objects

$(SY_DIR)/ms_bdy1.$(OBJ): $(OS_DIR)/ms_bdy1.c $(SYS_INC) $(MS_INC)
	$(PCC) $(CC) -c $(CMTOPTS) $(MSOPTS) $(OS_DIR)/ms_bdy1.c

$(SY_DIR)/ms_bdy2.$(OBJ): $(OS_DIR)/ms_bdy2.c $(SYS_INC) $(MS_INC)
	$(PCC) $(CC) -c $(CMTOPTS) $(MSOPTS) $(OS_DIR)/ms_bdy2.c

$(SY_DIR)/ms_ptmi.$(OBJ): $(OS_DIR)/ms_ptmi.c $(SYS_INC) $(MS_INC)
	$(PCC) $(CC) -c $(CMTOPTS) $(MSOPTS) $(OS_DIR)/ms_ptmi.c

$(SY_DIR)/ms_id.$(OBJ): $(OS_DIR)/ms_id.c $(SYS_INC) $(MS_INC)
	$(PCC) $(CC) -c $(CMTOPTS) $(MSOPTS) $(OS_DIR)/ms_id.c

$(SY_DIR)/ms_ex_ms.$(OBJ): $(OS_DIR)/ms_ex_ms.c $(SYS_INC) $(MS_INC)
	$(PCC) $(CC) -c $(CMTOPTS) $(MSOPTS) $(OS_DIR)/ms_ex_ms.c

#----------------------------------------- stack manager objects

$(SY_DIR)/sm_bdy1.$(OBJ): $(SM_DIR)/sm_bdy1.c $(SYS_INC) $(LPJ_INC)
	$(CC) -c $(CSMOPTS) -DSM $(SM_DIR)/sm_bdy1.c

$(SY_DIR)/sm_ex_ms.$(OBJ): $(SM_DIR)/sm_ex_ms.c $(SYS_INC) $(LPJ_INC)
	$(CC) -c $(CSMOPTS) -DSM $(SM_DIR)/sm_ex_ms.c

$(SY_DIR)/sm_pj_ptmi.$(OBJ): $(PJ_SAMPLE_DIR)/sm_pj_ptmi.c $(SYS_INC) $(LPJ_INC)
	$(CC) -c $(CPRDOPTS) -DSM $(PJ_SAMPLE_DIR)/sm_pj_ptmi.c

$(SY_DIR)/sm_pj_bdy1.$(OBJ): $(PJ_SAMPLE_DIR)/sm_pj_bdy1.c $(SYS_INC) $(LPJ_INC)
	$(CC) -c $(CPRDOPTS) -DSM $(PJ_SAMPLE_DIR)/sm_pj_bdy1.c

$(SY_DIR)/sm_pj_ex_ms.$(OBJ): $(PJ_SAMPLE_DIR)/sm_pj_ex_ms.c $(SYS_INC) $(LPJ_INC)
	$(CC) -c $(CPRDOPTS) -DSM $(PJ_SAMPLE_DIR)/sm_pj_ex_ms.c

#----------------------------------------- portable objects

$(SY_DIR)/pj_tst.$(OBJ): $(PJ_DIR)/pj_tst.c 
	$(CC) -c $(CPTOPTS) $(PJ_DIR)/pj_tst.c

$(SY_DIR)/ss_ptsp.$(OBJ): $(CM_DIR)/ss_ptsp.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPTOPTS) $(CM_DIR)/ss_ptsp.c

#------------------------------------------ MTSS objects
$(SY_DIR)/ss_gen.$(OBJ): $(SS_DIR)/ss_gen.c $(SS_INC)
	$(PURE) $(CC) -c $(CMTOPTS) $(SS_DIR)/ss_gen.c

$(SY_DIR)/ss_task.$(OBJ): $(SS_DIR)/ss_task.c $(SS_INC)
	$(PURE) $(CC) -c $(CMTOPTS) $(SS_DIR)/ss_task.c

$(SY_DIR)/ss_drvr.$(OBJ): $(SS_DIR)/ss_drvr.c $(SS_INC)
	$(PURE) $(CC) -c $(CMTOPTS) $(SS_DIR)/ss_drvr.c

$(SY_DIR)/ss_timer.$(OBJ): $(SS_DIR)/ss_timer.c $(SS_INC)
	$(PURE) $(CC) -c $(CMTOPTS) $(SS_DIR)/ss_timer.c

$(SY_DIR)/ss_mem.$(OBJ): $(SS_DIR)/ss_mem.c $(SS_INC)
	$(PURE) $(CC) -c $(CMTOPTS) $(SS_DIR)/ss_mem.c

$(SY_DIR)/ss_strm.$(OBJ): $(SS_DIR)/ss_strm.c $(SS_INC)
	$(PURE) $(CC) -c $(CMTOPTS) $(SS_DIR)/ss_strm.c

$(SY_DIR)/ss_msg.$(OBJ): $(SS_DIR)/ss_msg.c $(SS_INC)
	$(PURE) $(CC) -c $(CMTOPTS) $(SS_DIR)/ss_msg.c

$(SY_DIR)/ss_queue.$(OBJ): $(SS_DIR)/ss_queue.c $(SS_INC)
	$(PURE) $(CC) -c $(CMTOPTS) $(SS_DIR)/ss_queue.c

$(SY_DIR)/ss_pack.$(OBJ): $(SS_DIR)/ss_pack.c $(SS_INC)
	$(PURE) $(CC) -c $(CMTOPTS) $(SS_DIR)/ss_pack.c

$(SY_DIR)/ss_rtr.$(OBJ): $(SS_DIR)/ss_rtr.c $(SS_INC)
	$(PURE) $(CC) -c $(CMTOPTS) $(SS_DIR)/ss_rtr.c

#-----------------------------------------  mt files build
$(SY_DIR)/cm_mem.$(OBJ): $(CM_DIR)/cm_mem.c $(SYS_INC) $(CM_INC) 
	$(PURE) $(CC) -c $(CMTOPTS) $(CM_DIR)/cm_mem.c

$(SY_DIR)/mt_ss.$(OBJ): $(MT_DIR)/mt_ss.c 
	$(PURE) $(CC) -c $(CMTOPTS) $(MT_DIR)/mt_ss.c

$(SY_DIR)/mt_id.$(OBJ): $(MT_DIR)/mt_id.c 
	$(PURE) $(CC) -c $(CMTOPTS) $(MT_DIR)/mt_id.c

#-----------------------VXWORKS
$(SY_DIR)/vw_ss.$(OBJ): $(VW_DIR)/vw_ss.c $(SYS_INC) $(SS_INC) $(VW_INC)
	$(CC) -c $(CMTOPTS) $(VW_DIR)/vw_ss.c

$(SY_DIR)/vw_id.$(OBJ): $(VW_DIR)/vw_id.c $(SYS_INC) $(SS_INC) $(VW_INC)
	$(CC) -c $(CMTOPTS) $(VW_DIR)/vw_id.c

$(SY_DIR)/vw_acc.$(OBJ): $(VW_DIR)/vw_acc.c $(SYS_INC) $(SS_INC) $(VW_INC)
	$(CC) -c $(CMTOPTS) $(VW_DIR)/vw_acc.c

# WinNT Files Build -----------------------------------------
$(SY_DIR)/ns_gen.$(OBJ): $(SS_DIR)/ns_gen.c
	$(CC) -c $(CMTOPTS) $(SS_DIR)/ns_gen.c

$(SY_DIR)/smnsexms.$(OBJ): $(SS_DIR)/smnsexms.c
	$(CC) -c $(CPRDOPTS) $(SS_DIR)/smnsexms.c

$(SY_DIR)/smnsbdy1.$(OBJ): $(SS_DIR)/smnsbdy1.c
	$(CC) -c $(CPRDOPTS) $(SS_DIR)/smnsbdy1.c

$(SY_DIR)/ns_ex_ms.$(OBJ): $(SS_DIR)/ns_ex_ms.c
	$(CC) -c $(CMTOPTS) $(SS_DIR)/ns_ex_ms.c

$(SY_DIR)/ns_id.$(OBJ): $(SS_DIR)/ns_id.c
	$(CC) -c $(CMTOPTS) $(SS_DIR)/ns_id.c

$(SY_DIR)/ns_ptmi.$(OBJ): $(SS_DIR)/ns_ptmi.c
	$(CC) -c $(CMTOPTS) $(SS_DIR)/ns_ptmi.c

$(SY_DIR)/ns_space.$(OBJ): $(SS_DIR)/ns_space.c
	$(CC) -c $(CMTOPTS) $(SS_DIR)/ns_space.c

$(SY_DIR)/ns_task.$(OBJ): $(SS_DIR)/ns_task.c
	$(CC) -c $(CMTOPTS) $(SS_DIR)/ns_task.c

$(SY_DIR)/ns_timer.$(OBJ): $(SS_DIR)/ns_timer.c
	$(CC) -c $(CMTOPTS) $(SS_DIR)/ns_timer.c


#
#********************************************************************30**
#  
#        End of file:     pj.mak@@/main/TeNB_Main_BR/6 - Mon Aug 11 16:42:43 2014
#    
#********************************************************************31*/
#
#********************************************************************40**
#  
#        Notes:
#  
#********************************************************************41*/
#
#********************************************************************50**
#
#********************************************************************51*/
#
#********************************************************************60**
#  
#       Revision history:
#  
#********************************************************************61*/
#  
#********************************************************************90**
#
#   ver       pat    init                  description
#---------- -------- ---- -----------------------------------------------
#/main/1      ---     gk   1. Initial Release.
#/main/1      ---     nm   1. LTE RLC 2.1 release
#********************************************************************91*/

# DO NOT DELETE THIS LINE -- make depend depends on it.


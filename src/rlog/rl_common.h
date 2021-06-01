/********************************************************************16**

            (c) Copyright 2012 by RadiSys Corporation. All rights reserved.

     This software is confidential and proprietary to RadiSys Corporation.
     No part of this software may be reproduced, stored, transmitted, 
     disclosed or used in any form or by any means other than as expressly
     provided by the written Software License Agreement between Radisys 
     and its licensee.

     Radisys warrants that for a period, as provided by the written
     Software License Agreement between Radisys and its licensee, this
     software will perform substantially to Radisys specifications as
     published at the time of shipment, exclusive of any updates or 
     upgrades, and the media used for delivery of this software will be 
     free from defects in materials and workmanship.  Radisys also warrants 
     that has the corporate authority to enter into and perform under the 
     Software License Agreement and it is the copyright owner of the software 
     as originally delivered to its licensee.

     RADISYS MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
     MATERIALS.

     IN NO EVENT SHALL RADISYS BE LIABLE FOR ANY INDIRECT, SPECIAL,
     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
     OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
     OF THE POSSIBILITY OF SUCH DAMAGE.

                       Restricted Rights Legend:

     This software and all related materials licensed hereby are
     classified as "restricted computer software" as defined in clause
     52.227-19 of the Federal Acquisition Regulation ("FAR") and were
     developed entirely at private expense for nongovernmental purposes,
     are commercial in nature and have been regularly used for
     nongovernmental purposes, and, to the extent not published and
     copyrighted, are trade secrets and confidential and are provided
     with all rights reserved under the copyright laws of the United
     States.  The government's rights to the software and related
     materials are limited and restricted as provided in clause
     52.227-19 of the FAR.

                    IMPORTANT LIMITATION(S) ON USE

     The use of this software is limited to the use set
     forth in the written Software License Agreement between Radisys and
     its Licensee. Among other things, the use of this software
     may be limited to a particular type of Designated Equipment, as 
     defined in such Software License Agreement.
     Before any installation, use or transfer of this software, please
     consult the written Software License Agreement or contact Radisys at
     the location set forth below in order to confirm that you are
     engaging in a permissible use of the software.

                    RadiSys Corporation
                    Tel: +1 (858) 882 8800
                    Fax: +1 (858) 777 3388
                    Email: support@trillium.com
                    Web: http://www.radisys.com 
 
*********************************************************************17*/

/********************************************************************20**

     Name:     Radisys Logging Framework
     Type:     C include file
     Desc:     This file contains logging framework include file for library.
     File:     rl_common.h
     Sid:      rl_common.h@@/main/TeNB_Main_BR/3 - Mon Aug 11 16:57:03 2014
     Prg:      Total eNB
     Auth:		Vikram Korde, Aug 2013

*********************************************************************21*/
/*************************************************************************
@ author : Vikram Korde  
@ description: This is header file is used by logging framework module. This
file should not be cirectly included by any other application although it is
common file to logging framework and LOG MACROs used by any applicatoin.
***************************************************************************/

#ifndef __RL_COMMON_H__
#define __RL_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	L_ALWAYS=0,
	L_FATAL,
	L_ERROR,
	L_WARNING,
	L_EVENT,
	L_INFO,
	L_DEBUG,
	L_UNUSED,
	L_MAX_LOG_LEVEL
} R_LOG_LEVEL;

typedef enum {
	DBG_CELLID,
   DBG_PEERID,
   DBG_ENBID,
 	DBG_MMEID,
 	DBG_CRNTI,
   DBG_UEIDX,
 	DBG_UEID,
 	DBG_RBID,
 	DBG_LCID,
 	DBG_LCGID,
	DBG_TRNSID,
   DBG_INSTID,
	DBG_MAX_IDs
} R_SPL_ARG;

#ifdef USE_RLOG_DATA_TYPES
typedef const char* PSTR;
typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;
typedef int S32;
typedef signed short S16;
#else
#include "envdep.h"
typedef const char* PSTR;
#endif

typedef U32 LOGID;

#ifdef RLOG_ENABLE_TEXT_LOGGING
#include <stdio.h>
extern FILE* g_fp;
void logLev0(PSTR strLogLevel, PSTR modName, PSTR file, int lineno, PSTR fmtStr, ...);
void logLev1(PSTR strLogLevel, PSTR modName, PSTR file, int lineno, PSTR fmtStr, U32 arg1, ...);
void logLev2(PSTR strLogLevel, PSTR modName, PSTR file, int lineno, PSTR fmtStr, U32 arg1, U32 arg2, ...);
void logLev3(PSTR strLogLevel, PSTR modName, PSTR file, int lineno, PSTR fmtStr, U32, U32, U32, ...);
void logLev4(PSTR strLogLevel, PSTR modName, PSTR file, int lineno, PSTR fmtStr, U32, U32, U32, U32, ...);
void logLevN(int logLevel, const char* modName, const char* file, int lineno, const char* fmtStr, ...);
void logLevE(PSTR strLogLevel, PSTR modName, PSTR file, int lineno, PSTR fmtStr, R_SPL_ARG splType,
   U32 splVal, U32 arg1, U32 arg2, U32 arg3, U32 arg4, ...);
void logLevH(PSTR strLogLevel, PSTR modName, PSTR file, int lineno, PSTR fmtStr, PSTR hexdump, int hexlen, ...);
void logLevS(PSTR strLogLevel, PSTR modName, PSTR file, int lineno, PSTR fmtStr, PSTR str, ...);
#else
void logLevH( LOGID logId, R_LOG_LEVEL logLevel, PSTR hex, int hexlen, ...);
void logLev0( LOGID logId, R_LOG_LEVEL logLevel, ...);
void logLev1( LOGID logId, R_LOG_LEVEL logLevel, U32 arg1, ...);
void logLev2( LOGID logId, R_LOG_LEVEL logLevel, U32 arg1, U32 arg2, ...);
void logLev3( LOGID logId, R_LOG_LEVEL logLevel, U32 arg1, U32 arg2, U32 arg3, ...);
void logLev4( LOGID logId, R_LOG_LEVEL logLevel, U32 arg1, U32 arg2, U32 arg3, U32 arg4, ...);
void logLevE(LOGID logId, R_LOG_LEVEL logLevel, R_SPL_ARG splType, U32 splVal,
U32 arg1, U32 arg2, U32 arg3, U32 arg4, ...);
void logLevS( LOGID logId, R_LOG_LEVEL logLevel, const char* str, ...);
#endif

void hextostr(char* p, PSTR h, int hexlen);

extern int g_logLevel;
extern U32 g_modMask;
extern const char* g_logStr[L_MAX_LOG_LEVEL]; 
extern const char* g_splStr[DBG_MAX_IDs];

#define RLOG_SEGFAULT_STR "Segmentation Fault Occurred\n%s"

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RLOG_COMMON_H__ */

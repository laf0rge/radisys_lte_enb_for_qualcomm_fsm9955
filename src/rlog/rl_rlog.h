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
     File:     rl.h
     Sid:      rl_rlog.h@@/main/TeNB_Main_BR/4 - Mon Aug 11 16:57:05 2014
     Prg:      Total eNB
	  Auth:		Vikram Korde, Aug 2013

*********************************************************************21*/
/****************************************************************************
@ author : Vikram Korde  
@ description: This is header file is used by logging framework module. This
file should not be included by any other application. This is internal
header file logging framework.
*****************************************************************************/

#ifndef __RL_H__
#define __RL_H__

#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include "rl_common.h"

#define MAX_FILE_SIZE  			3145728 /* 3MB */
#define MAX_LOG_LEN				256
#define MAX_FILENAME_LEN 		300
#define MAX_FILENAME_LENGTH 	700
#define LOG_TIME_LEN 			64
#define MAX_LOG_BUF_SIZE 		5000
#define RLOG_MAX_CIRBUF_SIZE			(1024*100)
#define RLOG_REMOTE_LOGGING_PORT		9099
#define RLOG_MAX_FILES		 			5
#define RLOG_MAX_TIME_STAMP 			80
#define RLOG_MAX_TAX_NAME				16
#define RLOG_FIFO_FILE					"/tmp/l2logs"
#define RLOG_CIRBUF_READ_INTERVAL	1	/* 60 seconds read interval */
#define RLOG_MAX_THREADS				16
#define RLOG_TIME_ZONE_LEN				8
#define RLOG_MAX_STACK_DEPTH 		24
#define RLOG_MAX_BACKTRACE_BUFSZ	2048
#define RLOG_READ_POS_THRESHOLD 300
#define RLOG_FIXED_LENGTH_BUFFER_SIZE 50
#define RLOGTICKSCNTTOPRCL2LOGS 10

/* Console handling */
#define RLOG_CTRL_L    12
#define RLOG_CTRL_Y    25
#define RLOG_ENTER_KEY 10
#define RLOG_SET_LOGLEVEL  1
#define RLOG_SET_MODMASK   2

/*L2 Logging */
#define PROCESS_L2LOG_TTI 10

typedef enum {
LOG_ARG_INT,
LOG_ARG_STR,
LOG_ARG_HEX,
LOG_ARG_SPL
} LOG_ARG_TYPE;

typedef enum rlLogCntLmt
{
   RL_LOG_COUNT_LIMIT_START = 1,
   RL_LOG_COUNT_LIMIT_STOP
}RLLogCntLmt;

#ifndef RLOG_ENABLE_TEXT_LOGGING

typedef enum
{
	L_TIME_REFERENCE=0,
	L_TIME_DELIMITER,
	L_SIGSEGV,
    L_TIME_TTI_UPDT
} LOGID_TYPE;

typedef struct
{
	time_t tv_sec;
	U32	ms_tti;
} __attribute__ ((__packed__)) LOGTIME;

typedef struct
{
	LOGID			logId;		/* 4 bytes | 32 bits */
	LOGTIME		logTime;		/* 8 bytes */
	unsigned int argType 	:2;
	unsigned int logLevel	:3;
	unsigned int numOfArgs	:3;
	U16			len;
} __attribute__ ((__packed__)) LOGDATA;

typedef struct
{
	LOGDATA 	logData;
	char 		buf[MAX_LOG_BUF_SIZE];
}  __attribute__ ((__packed__)) ARGDATA;

typedef struct
{
	LOGDATA logData;
	U32 arg1;
	U32 arg2;
	U32 arg3;
	U32 arg4;
   char unusedByte[19]; /* To make it as 50 byte */
}  __attribute__ ((__packed__)) ARG4DATA;

typedef struct
{
	LOGDATA logData;
	U8		  splEnum;
	U32 splArg;
	U32 arg1;
	U32 arg2;
	U32 arg3;
	U32 arg4;
   char unusedByte[14]; /* To make it as 50 byte */
}  __attribute__ ((__packed__)) SPL_ARGDATA;

typedef enum _endian {little_endian, big_endian} EndianType;

typedef struct
{
	U16			version;
	U32			dummy32;
	U8				endianType;
	char			szTimeZone[RLOG_TIME_ZONE_LEN+1];
	U16			END_MARKER;
   time_t      time_sec;
} __attribute__ ((__packed__)) FILE_HEADER;

#endif /* RLOG_ENABLE_TEXT_LOGGING */

typedef struct {
	
	char	szTaskName[RLOG_MAX_TAX_NAME];
	U8*	logBuff;	 	/* LOG Buffer */
	U32 	logBufLen;  /* Data Written till now */
	U32	logReadPos; /* Reader thread position */
	U8 	listIndex;	/* Index to global list */

} THREAD_DATA;

extern void readL2LogBuff(void);
extern void processL2LogBuff(void);
extern S16  sendL2LogBuftoL3(void);
extern void rlInitL2Log(void);
/* Extern for soc specific file */
extern void rlProcessLogBufFromL2(void *mBuf);
extern void rlInitL2SocSpecific(void);
//extern void processL2LogBuff(void);
extern void rlProcessTicks(void);
extern void rlGetL2LogBufPtr (void *mBuf, U32 *logLen,U8 **logPtr);
extern void rlInvalidateL2LogsInCache(U8 *ptr,U32 len);

extern U8     *g_l2rlogBuf;        /* buffer pointer for shared memory allocation */
extern U8     *g_l2LogBufStartPtr; /* buffer pointer where logs has to be written */
extern U8     *g_l2LogBufBasePtr;  /* Base pointer for log buffer */
extern U8     *g_logBufRcvdFromL2; /* Buffer pointer received from L2 at L3*/
extern U8     *g_l2LogBaseBuff;    /* Base log buffer received at L3 */
extern U32     g_l2LogBufLen;      /* Log Buffer length written at L2 */
extern U32     startL2Logging;     /* flag to start processing of L2 logs */
extern U32     g_l2logBuffPos;     /* Log Buffer block which is in use for L2 logging */
extern U8      g_writeCirBuf;      /* Flag to indicate whether to write logs or not */
//extern Pst    g_rlog_pst;

#endif /* __RL_H__*/

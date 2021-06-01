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

     Name:     EnodeB Application

     Type:     C source file

     Desc:     This file contains


     File:     wr_dbg_log.c

     Sid:      fc_dbg_log.c@@/main/TeNB_Main_BR/tenb_main_ccb/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:50:47 2016

     Prg:      sasahni

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="ENBAPP";
static int RLOG_MODULE_ID=64;
static int RLOG_FILE_ID=109;


/* Header include files (.h) */
#include "ssi.h"
#include "wr.h"
#include "wr_cmn.h"
#include "wr_emm.h"
#include "wr_umm.h"
#include "wr_umm_trans.h"
#include "wr_dam.h"
#include "wr_umm_msg.h"
#include "wr_smm_init.h"
#include "wr_dbg_log.h"
#include <stdlib.h>
#include <stdarg.h>
#ifdef SS_SEGV_SIG_HDLR
#include <execinfo.h>
#endif /* SS_SEGV_SIG_HDLR */

#ifdef WR_DBG_CIRLOG
EXTERN Bool wrInitSuccess;
WrDbgCirBuf wrGbDbgCirBuf;
WrDbgFP wrGbDbgFP;
PUBLIC WrSetPF wrSetPf[MAX_LOG_TYPE] = {wrSetCons,wrSetCirBuf};
PUBLIC WrPrntPF wrPrntPf[MAX_LOG_TYPE] = {wrConsPrnt, wrCirBufPrnt};

PRIVATE S16 wrDbgCreateCirFile ARGS((void));
PRIVATE S16 wrDbgCreateCirBuf ARGS((void));
PUBLIC S16 wrSigSegvActvTsk ARGS((void));
const char * wrGlSwVersion;
const char * wrGlPfVersion;
/*
*
*       Fun:   wrDbgCreateCirFile
*
*       Desc:  This function will open all files as per configuration.
*
*       Ret:   ROK on success
*              RFAILED on failure
*
*       Notes:
*
*       File:  wr_dbg_log.c
*
*/
#ifdef ANSI
PRIVATE S16 wrDbgCreateCirFile
(
void
)
#else
PRIVATE S16 wrDbgCreateCirFile()
#endif
{
   U32 countFileNo;

   for(countFileNo=0;countFileNo<smCfgCb.numLogFiles;countFileNo++)
   {
      wrGbDbgFP.dbgLogFp[countFileNo] = NULL;
      wrGbDbgFP.spaceLeftInFile[countFileNo] = (smCfgCb.logFileSize);
      RLOG2(L_DEBUG, "Cnt: %d space:%u",
             countFileNo, wrGbDbgFP.spaceLeftInFile[countFileNo]);
   }
   wrGbDbgFP.currentWritingFile = 0;
   RETVALUE(ROK);
}

/*
*
*       Fun:   wrDbgCreateCirBuf
*
*       Desc:  This function will create circular buffer of filesize.
*
*       Ret:   ROK on success
*              RFAILED on failure
*
*       Notes:
*
*       File:  wr_dbg_log.c
*
*/
#ifdef ANSI
PRIVATE S16 wrDbgCreateCirBuf
(
void
)
#else
PRIVATE S16 wrDbgCreateCirBuf()
#endif
{

   /*setting no of logs can be storred in circBuffer/circBuffer size*/
   wrGbDbgCirBuf.bufLen = MAX_CIRBUF_LEN;
   if((SGetSBuf(WR_CIRBUF_REGION, WR_CIRBUF_POOL,
      (Data**)&wrGbDbgCirBuf.dbgLogBuf, MAX_CIRBUF_LEN) != ROK) ||
      (wrGbDbgCirBuf.dbgLogBuf == NULLP))
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(NULLP);
   }
   wrGbDbgCirBuf.bufRear = wrGbDbgCirBuf.dbgLogBuf;
   wrGbDbgCirBuf.bufFront = wrGbDbgCirBuf.dbgLogBuf;
   wrGbDbgCirBuf.bufHead = wrGbDbgCirBuf.dbgLogBuf;
   wrGbDbgCirBuf.bufTail = wrGbDbgCirBuf.bufHead + wrGbDbgCirBuf.bufLen - 1;

   RETVALUE(ROK);
}/*end of ssdCreateCirbuf*/

/*
*
*       Fun:   wrDbgLogInit
*
*       Desc:  This function Initializes Circular buffer and Files.
*
*       Ret:   ROK on success
*              RFAILED on failure
*
*       Notes:
*
*       File:  wr_dbg_log.c
*
*/
#ifdef ANSI
PUBLIC S16 wrDbgLogInit
(
void
)
#else
PUBLIC S16 wrDbgLogInit()
#endif
{
   S16 ret = ROK;

   ret = wrDbgCreateCirFile();
   if(ret != ROK)
   {
      RLOG0(L_ERROR, "File creation failed");
      RETVALUE(RFAILED);
   }
   ret = wrDbgCreateCirBuf();
   if(ret != ROK)
   {
      RLOG0(L_ERROR, "Circular buffer creation failed");
      RETVALUE(RFAILED);
   }
   wrGlSwVersion = wrGetSwVersion();
   wrGlPfVersion = wrGetPfVersion();
   wrInitSuccess = TRUE;
   RETVALUE(ROK);
}

/*
*
*       Fun:   wrDbgLogDeinit
*
*       Desc:  This function De-Initializes Circular buffer and Files.
*
*       Ret:   ROK on success
*              RFAILED on failure
*
*       Notes:
*
*       File:  wr_dbg_log.c
*
*/
#ifdef ANSI
PUBLIC S16 wrDbgLogDeinit
(
void
)
#else
PUBLIC S16 wrDbgLogDeinit()
#endif
{
   U32 countFileNo;
   S16 ret = ROK;
   for(countFileNo=0;countFileNo<smCfgCb.numLogFiles;countFileNo++)
   {
      RLOG1(L_DEBUG, "****Closing Log File DebugLog%d.txt ",countFileNo);
      ret = fclose(wrGbDbgFP.dbgLogFp[countFileNo]);
      if(ret != ROK)
      {
         RLOG0(L_ERROR, "Files closing failed");
         RETVALUE(RFAILED);
      }
      wrGbDbgFP.dbgLogFp[countFileNo] = NULL;
   }

   SPutSBuf(WR_CIRBUF_REGION, WR_CIRBUF_POOL, (Data*)wrGbDbgCirBuf.dbgLogBuf, MAX_CIRBUF_LEN);

   RETVALUE(ROK);
}

/*
*
*       Fun:   wrDbgWriteToFile
*
*       Desc:  This function writes contents in logging cirular buffer to File.
*
*       Ret:   ROK on success
*              RFAILED on failure
*
*       Notes:
*
*       File:  wr_dbg_log.c
*
*/
#ifdef ANSI
PUBLIC S16 wrDbgWriteToFile
(
U32  sizeToCopy,
Txt  *startPtr,
Txt  *endPtr
)
#else
PUBLIC S16 wrDbgWriteToFile(sizeToCopy,front,rear)
U32  sizeToCopy;
Txt  *startPtr;
Txt  *endPtr;
#endif
{
    S32 ret;
    U32 spaceInFile;
    S8 buf[MAX_LOG_BUF_SIZE];
    S8 fileName[MAX_FILENAME_LEN];

    /* Opens file:If not yet created */
    if(NULL == wrGbDbgFP.dbgLogFp[wrGbDbgFP.currentWritingFile])
    {
       sprintf(fileName,smCfgCb.logFilePath, wrGbDbgFP.currentWritingFile);
       wrGbDbgFP.dbgLogFp[wrGbDbgFP.currentWritingFile] = fopen(fileName, "w+");
       if(NULL == wrGbDbgFP.dbgLogFp[wrGbDbgFP.currentWritingFile])
       {
          wrInitSuccess = FALSE;
          RLOG_STR(L_ERROR, "Opening Log File Failed :%s",fileName);
          RETVALUE(RFAILED);
       }
       /* resetting space left in file to MAX */
       wrGbDbgFP.spaceLeftInFile[wrGbDbgFP.currentWritingFile] =
          (smCfgCb.logFileSize);

       /*Writing Header to File*/
       SGetTimeStamp(buf);
       ret = fprintf(wrGbDbgFP.dbgLogFp[wrGbDbgFP.currentWritingFile],
             "*************************************\nFile Creation Time:%s\n"
             "SOFTWARE VERSION:%s\nPLATFORM:%s\n*********************"
             "****************\n",buf,wrGlSwVersion,wrGlPfVersion);
       wrGbDbgFP.spaceLeftInFile[wrGbDbgFP.currentWritingFile] -= ret;
    }
    /*
     * copy To file
     */
    while(sizeToCopy)
    {
       spaceInFile = wrGbDbgFP.spaceLeftInFile[wrGbDbgFP.currentWritingFile];
       /*
        * If space in file is not sufficient, then write upto available
        * size in file  and open new file to write remaining log.
        */
       if(spaceInFile < sizeToCopy)
       {
          fwrite(startPtr,spaceInFile,1,
                wrGbDbgFP.dbgLogFp[wrGbDbgFP.currentWritingFile]);
          fflush(wrGbDbgFP.dbgLogFp[wrGbDbgFP.currentWritingFile]);
          wrGbDbgFP.dbgLogFp[wrGbDbgFP.currentWritingFile] = NULL;
          sizeToCopy -= spaceInFile;
          startPtr += spaceInFile;

          /* opening new file to copy remaining  log*/
          wrGbDbgFP.currentWritingFile =
             ((wrGbDbgFP.currentWritingFile+1) % smCfgCb.numLogFiles);
          sprintf(fileName,smCfgCb.logFilePath, wrGbDbgFP.currentWritingFile);
          wrGbDbgFP.dbgLogFp[wrGbDbgFP.currentWritingFile] =
             fopen(fileName, "w+");
          if(NULL == wrGbDbgFP.dbgLogFp[wrGbDbgFP.currentWritingFile])
          {
             wrInitSuccess = FALSE;
             RLOG_STR(L_ERROR, "Creating Log File Failed :%s",fileName);
             RETVALUE(RFAILED);
          }
          /* resetting space left in file to MAX */
          wrGbDbgFP.spaceLeftInFile[wrGbDbgFP.currentWritingFile] =
             (smCfgCb.logFileSize);

          /*Writing Header to File*/
          SGetTimeStamp(buf);
          ret = fprintf(wrGbDbgFP.dbgLogFp[wrGbDbgFP.currentWritingFile],
                "*************************************\nFile Creation Time:%s\n"
                "SOFTWARE VERSION:%s\nPLATFORM:%s\n*********************"
                "****************\n",buf,wrGlSwVersion,wrGlPfVersion);
          wrGbDbgFP.spaceLeftInFile[wrGbDbgFP.currentWritingFile] -= ret;
       }
       else
       {
          fwrite(startPtr,sizeToCopy,1,
                wrGbDbgFP.dbgLogFp[wrGbDbgFP.currentWritingFile]);
          wrGbDbgFP.spaceLeftInFile[wrGbDbgFP.currentWritingFile] -= sizeToCopy;
          fflush(wrGbDbgFP.dbgLogFp[wrGbDbgFP.currentWritingFile]);
          sizeToCopy = 0;
       }
    }
    RETVALUE(ROK);
}

/*
*
*       Fun:   wrDbgWriteCircBufToFile
*
*       Desc:  This function cirular buffer to Files.
*
*       Ret:   ROK on success
*              RFAILED on failure
*
*       Notes:
*
*       File:  wr_dbg_log.c
*
*/
#ifdef ANSI
PUBLIC S16 wrDbgWriteCircBufToFile
(
void
)
#else
PUBLIC S16 wrDbgWriteCircBufToFile()
#endif
{
   U32             sizeToCopy = 0;
   Txt             *bufRear;

   /*check is there any log*/
   if(wrGbDbgCirBuf.bufFront != wrGbDbgCirBuf.bufRear)
   {
      bufRear = wrGbDbgCirBuf.bufRear;
      if(wrGbDbgCirBuf.bufFront < wrGbDbgCirBuf.bufRear )
      {
         /*when front is less then rear direct copy*/
         /*Copy from front to rear*/
         sizeToCopy = (wrGbDbgCirBuf.bufRear - wrGbDbgCirBuf.bufFront);
         wrDbgWriteToFile(sizeToCopy ,wrGbDbgCirBuf.bufFront, wrGbDbgCirBuf.bufRear);
      }
      else
      {
         /*when front is greater than rear chop and copy*/
         /*Copy from front to Tail*/
         sizeToCopy = (wrGbDbgCirBuf.bufTail - wrGbDbgCirBuf.bufFront);
         wrDbgWriteToFile(sizeToCopy ,wrGbDbgCirBuf.bufFront, wrGbDbgCirBuf.bufTail);

         /*Copy from Head to rear*/
         sizeToCopy = (wrGbDbgCirBuf.bufRear - wrGbDbgCirBuf.bufHead);
         wrDbgWriteToFile(sizeToCopy ,wrGbDbgCirBuf.bufHead, wrGbDbgCirBuf.bufRear);
      }
      wrGbDbgCirBuf.bufFront = bufRear;
   }
   RETVALUE(ROK);
}

/*
*
*       Fun:   wrSetCons
*
*       Desc:  This function sets the log buffer for dumping the logs on the
*              console.
*
*       Ret:   ROK on success
*              RFAILED on failure
*
*       Notes:
*
*       File:  wr_dbg_log.c
*
*/
PUBLIC S32 wrSetCons
(
TskInit *init,
S8 *fileName,
S32 lineNo,
S8 *layerName
)
{
   S32 ret;
#ifdef WR_DBG_CIRLOG_TIMESTAMP
   S8 buf[MAX_TIME_STAMP];
#endif /* WR_DBG_CIRLOG_TIMESTAMP */

#ifdef DEBUGP
   init->prntCirBuf = (init)->prntBuf;
#endif
#ifndef WR_DBG_CIRLOG_TIMESTAMP
   ret = snprintf(init->prntCirBuf, MAX_LOG_BUF_SIZE, \
         "[%s %d:0x%x:%x] %s:%d ",layerName,init->procId,
         (int)init->ent, (int)init->inst,fileName, lineNo);
#else
   (Void) SGetTimeStamp(buf);
   ret = snprintf(init->prntCirBuf, MAX_LOG_BUF_SIZE,\
         "[%s][%s %d:0x%x:%x] %s:%d ", buf, layerName,init->procId,
         (int)init->ent, (int)init->inst,fileName, (int)lineNo);
#endif /* WR_DBG_CIRLOG_TIMESTAMP */
   printf("%s",init->prntCirBuf);
   RETVALUE(ROK);
} /* wrSetCons */

/*
*
*       Fun:   wrSetCirBuf
*
*       Desc:  This function sets the circular log buffer for dumping each log.
*
*       Ret:   ROK on success
*              RFAILED on failure
*
*       Notes:
*
*       File:  wr_dbg_log.c
*
*/
PUBLIC S32 wrSetCirBuf
(
TskInit *init,
S8 *fileName,
S32 lineNo,
S8 *layerName
)
{
   S32 ret;
#ifdef WR_DBG_CIRLOG_TIMESTAMP
   S8 buf[MAX_TIME_STAMP];
#endif /* WR_DBG_CIRLOG_TIMESTAMP */

   /* If size is not available in circular buffer then move to
    * start of the circular buffer.
    */
   if(((2*MAX_LOG_BUF_SIZE) > (wrGbDbgCirBuf.bufTail - wrGbDbgCirBuf.bufRear))
         && (wrGbDbgCirBuf.bufRear > wrGbDbgCirBuf.bufFront))
   {
      wrGbDbgCirBuf.bufFront =  wrGbDbgCirBuf.bufHead;
      wrGbDbgCirBuf.bufRear  =  wrGbDbgCirBuf.bufHead;
   }
#ifndef WR_DBG_CIRLOG_TIMESTAMP
   ret = snprintf(wrGbDbgCirBuf.bufRear, MAX_LOG_BUF_SIZE,\
         "[%s %d:0x%x:%x] %s:%d ",layerName,(init)->procId,
         (int)(init)->ent, (int)(init)->inst, fileName, lineNo);
#else
   (Void) SGetTimeStamp(buf);
   ret = snprintf(wrGbDbgCirBuf.bufRear,MAX_LOG_BUF_SIZE,\
         "[%s][%s %d:0x%x:%x] %s:%d ",buf, layerName,(init)->procId,
         (int)(init)->ent, (int)(init)->inst,fileName, (int)lineNo);
#endif /* WR_DBG_CIRLOG_TIMESTAMP */
   wrGbDbgCirBuf.bufRear = wrGbDbgCirBuf.bufRear + ret;
   (init)->prntCirBuf = wrGbDbgCirBuf.bufRear;

   RETVALUE(ROK);
} /* wrSetCirBuf */

/*
*
*       Fun:   wrConsPrnt
*
*       Desc:  This function prints the logs on the console.
*
*       Ret:   ROK on success
*              RFAILED on failure
*
*       Notes: This function prototype resembles the C library function
*              snprintf
*
*       File:  wr_dbg_log.c
*
*/
PUBLIC S32 wrConsPrnt
(
S8 *prntBuf, S32 maxLogSize, S8 *fmtStr,...
)
{
   va_list list;
   S32 ret = ROK;
   va_start(list,fmtStr);
   vsnprintf(prntBuf, maxLogSize, fmtStr, list);
   printf("%s",prntBuf);
   va_end(list);

   RETVALUE(ret);
} /* wrConsPrnt */

/*
*
*       Fun:   wrCirBufPrnt
*
*       Desc:  This function prints the logs on to the circular buffer.
*
*       Ret:   ROK on success
*              RFAILED on failure
*
*       Notes: This function prototype resembles the C library function
*              snprintf
*
*       File:  wr_dbg_log.c
*
*/
PUBLIC S32 wrCirBufPrnt
(
S8 *prntBuf, S32 maxLogSize, S8 *fmtStr,...
)
{
   va_list list;
   S32 ret = ROK;
   va_start(list,fmtStr);
   ret = vsnprintf(prntBuf, maxLogSize, fmtStr, list);
   va_end(list);

   wrGbDbgCirBuf.bufRear += ret;
   RETVALUE(ROK);
} /* wrCirBufPrnt */

#endif /*WR_DBG_CIRLOG*/

/*
*
*       Fun:   wrLogDump
*
*       Desc:  This function dump the logs specific to the application.
*
*       Ret:   ROK on success
*              RFAILED on failure
*
*       Notes:
*
*       File:  wr_dbg_log.c
*
*/
#ifdef ANSI
PUBLIC S16 wrLogDump
(
void
)
#else
PUBLIC S16 wrLogDump()
#endif
{
   S32 idx;
   WrCellCb *cellCb = NULLP;

   RLOG0(L_DEBUG, "Dumping wrCb Info:");
   RLOG2(L_DEBUG, "Mem info: region: %d pool: %d",
            wrCb.mem.region, wrCb.mem.pool);

   if(wrEmmCb.cellCb != NULLP)
   {
      for(idx = 0; idx < WR_EMM_MAX_CELLS; idx++)
      {
         if(wrEmmCb.cellCb[idx] != NULLP)
         {
            cellCb = wrEmmCb.cellCb[idx];
            RLOG0(L_DEBUG, "wrCellCb Info:");
            RLOG_ARG2(L_DEBUG, DBG_CELLID, cellCb->cellId,
                  "dlCarrierFreq: %d numAntPorts: %d",
                  cellCb->dlCarrierFreq, cellCb->numAntPorts);
            RLOG2(L_DEBUG, "numSibsCfg: %d CellState: %d",
                  cellCb->numSibsCfg, cellCb->cellState); 
         }

      }
   }

   wrUmmEnbStats();
   RETVALUE(ROK);
} /* wrLogDump() */

#ifdef SS_SEGV_SIG_HDLR
/*
*
*       Fun:   wrSigSegvActvTsk
*
*       Desc:  This is a handler for SIGSEGV
*              Will call API to write circular buffer to file.
*
*       Ret:   ROK on success
*              RFAILED on failure
*
*       Notes:
*
*       File:  wr_dbg_log.c
*
*/
#ifdef ANSI
PUBLIC S16 wrSigSegvActvTsk
(
void
)
#else
PUBLIC S16 wrSigSegvActvTsk()
#endif
{
   Void                *trace[255];
   S8                 **funcName;
   S32                  traceSize;
   S32                  traceCnt;
   /* Dump the logs to the file and make sure logs are not
    * refreshed during the dump.
    */
#ifdef WR_DBG_CIRLOG
   wrDbgWriteCircBufToFile();
#endif /*WR_DBG_CIRLOG*/
   wrLogDump();

   /* Dump the backtrace */
   RLOG0(L_DEBUG, "Dumping Backtrace Info:");
   traceSize = backtrace(trace, MAX_BKTRACE_LEVEL);
   funcName = backtrace_symbols(trace, traceSize);
   for (traceCnt=0; traceCnt < traceSize; ++traceCnt)
   {
      RLOG_STR(L_DEBUG, "[bt]=> %s", funcName[traceCnt]);
   }
#ifdef WR_DBG_CIRLOG
   wrDbgWriteCircBufToFile();
#endif /*WR_DBG_CIRLOG*/
   RETVALUE(ROK);
}
#endif /* SS_SEGV_SIG_HDLR */

/********************************************************************30**

           End of file:     fc_dbg_log.c@@/main/TeNB_Main_BR/tenb_main_ccb/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:50:47 2016

*********************************************************************31*/


/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/

   
/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/
  
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
        ---      sasahni         1. initial release logging phase2
*********************************************************************91*/

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
               

     File:     wr_smm_diag_hdl.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

#ifdef SS_DIAG
static const char* RLOG_MODULE_NAME="SMM";
static int RLOG_FILE_ID=174;
static int RLOG_MODULE_ID=2;
#endif

/* header include files (.h) */
#include "wr.h"
#include "wr_cmn.h"
#include "wr_smm_init.h"
#include <pthread.h>
#include <unistd.h>
#ifdef SS_DIAG
#define WR_SMM_WBUF_SIZE 1024
#define WR_SMM_MAX_FLAT_BUFF_SIZE 512
/* @brief Private function forward declaration */
PRIVATE Void *wrDiagWriterProcess ARGS ((Void *arg));
PRIVATE S16 wrDiagWriteLogs(U8 *sndBuf, U32 msgLen);
PRIVATE S16 wrDiagSndAll(Void);
PRIVATE S16 wrDiagPkCirBuf(SsDiagCirBuf *cirBufPtr, U8 *sndBuf, U32 *msgLen);
PRIVATE S16 wrDiagSndTilRecRear(U32 *recordedRear);


/* @brief Public variable forward declaration */
EXTERN U8 wrDiagSndToPort;

/* @brief Private variable forward declaration */
PRIVATE Void *wrDiagWriterProcess ARGS ((Void *arg));
PRIVATE FILE *wrFp; 
EXTERN  WrSockAddr wrDiagDstAddr;
EXTERN U32 wrDiagSockFd;
PRIVATE U32 sizeOfLogBuf = sizeof(SsDiagBuf) - sizeof(U32); 

/** 
 * @brief This function is used to read logs from all global circular
 *  buffers and store them into a Flat buffer called sndBuf.
 *
 * @details This function packs the diagonstic logs in to a flat buffer.
 *
 *     Function: wrDiagPkCirBuf
 *
 *         Processing steps:
 *           - read data from circular buffer 
 *           - Check the type of the log
 *           - If log is flat buffer copy its data block directly 
 *           - If Log id variable /HexDump copy the flat buffer associated with it first and variable log later
 *           - Copy the contents into the flat buffer.
 *
 * @param[in] cirBufPtr: Circular buffer pointer
 * @param[in] sndBuf   : Flat buffer pointer
 * @param[in] msgLen   : Buffer index of Flat buffer  
 * @return S16
 *        -# ROK     - success.
 *        -# RFAILED - failure.
 */
PRIVATE S16 wrDiagPkCirBuf(SsDiagCirBuf *cirBufPtr, U8 *sndBuf, U32 *msgLen)
{  
   U32 front =  cirBufPtr->front;
   U32 ret;

   switch(cirBufPtr->logBuf[front].type)
   {
      case SS_DIAG_MSG_TYPE_FIXED:
         {
            cmMemcpy(&sndBuf[*msgLen], (U8*)&cirBufPtr->logBuf[front], sizeOfLogBuf);
            *msgLen += sizeOfLogBuf;
            if(*msgLen >= WR_SMM_WBUF_SIZE - sizeOfLogBuf)
            {
               ret = wrDiagWriteLogs(&sndBuf[0], *msgLen);
               if(ret != ROK)
               {
                  RETVALUE(RFAILED);
               }
               *msgLen = 0;
            }   
         }
         break;

      case SS_DIAG_MSG_TYPE_HEXDUMP:
         {
            /* asn Hex Dump implementation Start */
            Buffer   *mBuf1;
            MsgLen   mBufLen = 0;
            U8       pkArray[WR_SMM_MAX_FLAT_BUFF_SIZE];
            MsgLen   cnt = 0;

            /* convert the asn encoded mBuf in fixed size buffer */
            mBuf1 = (Buffer*)cirBufPtr->logBuf[front].splArg;
            SFndLenMsg (mBuf1, &mBufLen);
            SCpyMsgFix (mBuf1, 0, mBufLen, &pkArray[0], &cnt);
            WR_FREEMBUF (mBuf1);
            if( cnt != mBufLen)
            {
               /* some error happened while converting mBuf to fixed buffer */
               /*ccpu00127802*/
               RLOG0(L_ERROR, "SCpyFixMsg Failed");
               RETVALUE(RFAILED);
            }

            /* 
             * Initialize arg1 with size of asn message. Which is used in post
             * processing thread 
             */
            cirBufPtr->logBuf[front].arg1 = mBufLen;
            /* Copy buffer element into temp array */
            cmMemcpy(&sndBuf[*msgLen], (U8*)&cirBufPtr->logBuf[front], sizeOfLogBuf);
            *msgLen += sizeOfLogBuf;
            /* check has sufficient  space to insert the Hexdump
             * if not, send the present logs in the temp buffer 
             * and make some space */
            if(*msgLen >= (WR_SMM_WBUF_SIZE - mBufLen))
            {
               ret = wrDiagWriteLogs(&sndBuf[0], *msgLen);
               if(ret != ROK)
               {
                  RETVALUE(RFAILED);
               }
               *msgLen = 0;
            }

            /* copy the asn Hex Dump into the temp buffer */
            cmMemcpy( &sndBuf[*msgLen], (U8*)&pkArray, mBufLen);
            *msgLen += mBufLen;
            /* asn Hex Dump implementation end */
         }
         break;

      case SS_DIAG_MSG_TYPE_VAR:
         {
            SsDiagVarBuf *ptr;
            cmMemcpy( &sndBuf[*msgLen], (U8*)&cirBufPtr->logBuf[front], sizeOfLogBuf);
            *msgLen += sizeOfLogBuf;

            /* check has sufficient  space to insert the variable log
             * if not, send the present logs in the temp buffer 
             * and make some space */
            if(*msgLen >= (WR_SMM_WBUF_SIZE -  cirBufPtr->logBuf[front].arg1))
            {
               ret = wrDiagWriteLogs(&sndBuf[0], *msgLen);
               if(ret != ROK)
               {
                  RETVALUE(RFAILED);
               }
               *msgLen = 0;
            } 

            ptr = (SsDiagVarBuf*)cirBufPtr->logBuf[front].arg2;
            cmMemcpy(&sndBuf[*msgLen], (U8*)ptr->varStr, cirBufPtr->logBuf[front].arg1);
            *msgLen += cirBufPtr->logBuf[front].arg1;
            ptr->isValid = FALSE;
         }
         break;

      default:
         RETVALUE(RFAILED);
   }

   cirBufPtr->logBuf[front].isValid = FALSE;
   front++;
   front = front % cirBufPtr->nLen;
   cirBufPtr->front = front;
     
   RETVALUE(ROK);
} /* end of wrDiagPkCirBuf */

/** 
 * @brief This function is used to read logs from global buffer
 *  and send to file or port completely.
 *
 * @details
 * This function is used to read logs from global buffer
 *  and send to file or port completely.
 *
 *     Function: wrDiagSndAll
 *
 *     Processing steps:
 *           - read data from circular buffer and write it into a file or port
 *
 * @param[in] Void
 * @return S16
 *        -# ROK     - success.
 *        -# RFAILED - failure.
 */
PRIVATE S16 wrDiagSndAll()
{
   U8 sndBuf[WR_SMM_WBUF_SIZE];
   U32 msgLen = 0;
   U32  ret;
   U32  idx;
   SsDiagCirBuf *cirBufPtr;
   TRC2(wrDiagSndAll); 

   for(idx = 0; idx <SS_DIAG_NUM_CIRBUF_UARM; idx++ )
   {
      cirBufPtr = &ssDiagCirLogBuf[idx];

      while((cirBufPtr->rear != cirBufPtr->front ) && 
            (cirBufPtr->logBuf[ cirBufPtr->front].isValid == TRUE))
      {
         ret = wrDiagPkCirBuf(&ssDiagCirLogBuf[idx], &sndBuf[0], &msgLen);
         if(ret != ROK)
         {
            RETVALUE(RFAILED);
         }
      }
   }
   for(idx = SS_DIAG_NUM_CIRBUF_UARM; idx <(SS_DIAG_MAX_CIRC_BUF - SS_DIAG_NUM_CIRBUF_UARM); idx++)
   {
      cirBufPtr = &ssDiagCirLogBuf[idx];

      while(cirBufPtr->rear != cirBufPtr->front)
      {
         ret = wrDiagPkCirBuf(&ssDiagCirLogBuf[idx], &sndBuf[0], &msgLen);
         if(ret != ROK)
         {
            RETVALUE(RFAILED);
         }
      }        
   }  
   ret = wrDiagWriteLogs(&sndBuf[0], msgLen);
   if(ret != ROK)
   {
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of wrDiagSndAll */

/**
 * @brief This function is used to read logs from global Circular buffer and 
 * store it into file /socket up to the recorded rear.
 *
 * @details
 *
 * This function is used to read logs from global Circular buffer and 
 * store it into file /socket up to the recorded rear.
 *
 *     Function: wrDiagSndTilRecRear
 *
 *         Processing steps:
 *                - read data from circular buffer up to the rear point received in
 *                  recordedRear, append sort marker and write it into file or port
 *
 * @param[in] U32 *recordedRear
 * @return S16
 *        -# ROK     - success.
 *        -# RFAILED - failure.
 */ 
PRIVATE S16 wrDiagSndTilRecRear(U32 *recordedRear)
{
   U8 sndBuf[WR_SMM_WBUF_SIZE];
   U16 ret;
   U32 msgLen = 0;
   U32 idx = 0;
   SsDiagBuf sortMarker;
   SsDiagCirBuf *cirBufPtr;
   TRC2(wrDiagSndTilRecRear); 

   for(idx = 0; idx <SS_DIAG_NUM_CIRBUF_UARM; idx++ )
   {
      cirBufPtr = &ssDiagCirLogBuf[idx];
      while((cirBufPtr->front != recordedRear[idx]) &&
            (cirBufPtr->logBuf[cirBufPtr->front].isValid == TRUE))
      {
         ret = wrDiagPkCirBuf(cirBufPtr, &sndBuf[0], &msgLen);
         if(ret != ROK)
         {
            RETVALUE(RFAILED);
         }
      }

   }
   for(idx = SS_DIAG_NUM_CIRBUF_UARM; idx <(SS_DIAG_MAX_CIRC_BUF - SS_DIAG_NUM_CIRBUF_UARM); idx++ )
   {
      cirBufPtr = &ssDiagCirLogBuf[idx];
      while(cirBufPtr->front != recordedRear[idx])
      {
         ret = wrDiagPkCirBuf(cirBufPtr, &sndBuf[0], &msgLen);
         if(ret != ROK)
         {
            RETVALUE(RFAILED);
         }
      }  
   }
   SM_SET_ZERO(&sortMarker, sizeof(SsDiagBuf));
   sortMarker.type = SS_DIAG_MSG_TYPE_MARKER;
   sortMarker.entInstId = 0;
   cmMemcpy(&sndBuf[msgLen],(U8*)&sortMarker, sizeOfLogBuf);    
   msgLen += sizeOfLogBuf;            

   ret = wrDiagWriteLogs(&sndBuf[0], msgLen);
   if(ret != ROK)
   {
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of wrDiagSndTilRecRear */  

/** 
 * @brief This function is Entry function to Logging thread it will check 
 * for the circular buffer content and if any call the handler function.
 *
 * @details
 * This function is Entry function to Logging thread it will check 
 * for the circular buffer content and if any call the handler function.
 *
 *     Function: wrDiagWriterProcess
 *
 *         Processing steps:
 *         - check for the Circular buffer content 
 *         - if any logs are present call the handler function
 *
 * @param[in] arg : NULL is passed 
 * @return Void
 *        -# ROK     - success.
 *        -# RFAILED - failure.
 */ 

PRIVATE Void *wrDiagWriterProcess(Void *arg)
{  
   U32       firstRearRead[SS_DIAG_MAX_CIRC_BUF];
   U32       secondRearRead[SS_DIAG_MAX_CIRC_BUF];
   U32       idx;
   U32       emptyFlag = FALSE;
   SsDiagBuf sortMarker;

   TRC2(wrDiagWriterProcess);

   wrFp = SS_DIAG_FILE_OPEN("./TotaleNodedBlogs.txt", "w+");
   if(wrFp == NULLP)
   {
      /* kworks fix */
   } 

   while(TRUE)
   {
      if((ssDiagCirLogBuf[SS_DIAG_BUF1].rear != ssDiagCirLogBuf[SS_DIAG_BUF1].front) ||
            (ssDiagCirLogBuf[SS_DIAG_BUF2].rear != ssDiagCirLogBuf[SS_DIAG_BUF2].front) ||
            (ssDiagCirLogBuf[SS_DIAG_BUF3].rear != ssDiagCirLogBuf[SS_DIAG_BUF3].front) ||
            (ssDiagCirLogBuf[SS_DIAG_BUF4].rear != ssDiagCirLogBuf[SS_DIAG_BUF4].front) ||
            (ssDiagCirLogBuf[SS_DIAG_BUF5].rear != ssDiagCirLogBuf[SS_DIAG_BUF5].front) ||
            (ssDiagCirLogBuf[SS_DIAG_BUF6].rear != ssDiagCirLogBuf[SS_DIAG_BUF6].front))
      {
         /*we checking twice to find thread switching is append in between collecting rear or not*/
         for(idx = 0; idx < SS_DIAG_MAX_CIRC_BUF; idx++)
         {
            firstRearRead[idx] = ssDiagCirLogBuf[idx].rear;
         }
         for(idx = 0; idx < SS_DIAG_MAX_CIRC_BUF; idx++)
         {
            secondRearRead[idx] = ssDiagCirLogBuf[idx].rear;
         } 
         for(idx = 0; idx < SS_DIAG_MAX_CIRC_BUF; idx++)
         {
            if(firstRearRead[idx] != secondRearRead[idx])
            {
               wrDiagSndAll();
               emptyFlag = FALSE;
               break;
            }
         }
         if(idx == SS_DIAG_MAX_CIRC_BUF)
         {
            wrDiagSndTilRecRear(&firstRearRead[0]);
            emptyFlag = TRUE;
         }    
      }
      else if(emptyFlag != TRUE)
      {
         sortMarker.type = SS_DIAG_MSG_TYPE_MARKER;
         sortMarker.entInstId = 0;
         wrDiagWriteLogs((U8*)&sortMarker, (sizeof(sortMarker) - sizeof(U32)));
         emptyFlag = TRUE;
         fflush(wrFp);
      }
      /* Writer thread will sleep for 100 ms and it will check for new 
       * log buffers */
      usleep(SS_DIAG_SLEEP);
   }
} /* end of wrDiagWriterProcess */


/** 
 * @brief This function is used to create a log writer thread with low priority. 
 *
 * @details
 * This function is used to create a log writer thread with low priority. 
 *
 *     Function:smCreateLogThr
 *
 *         Processing steps:
 *         - This function is will create a lowest priority thread which will
 *           start execute wrDiagWriterProcess function. 
 *
 *
 *     @param[in] Void
 *     @return S16
 *        -# ROK     - success.
 *        -# RFAILED - failure.
 */


PUBLIC S16 smCreateLogThr(Void)
{  
   pthread_t            tid;
   pthread_attr_t       attr;
   struct sched_param   param_sched;

   TRC2(smCreateLogThr);

   pthread_attr_init(&attr);
   pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
   pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
   param_sched.sched_priority = sched_get_priority_min(SCHED_FIFO);
   pthread_attr_setschedparam(&attr, &param_sched);

   if (pthread_create(&tid, &attr, wrDiagWriterProcess, NULL) != 0)
   {
      ALARM("\nERROR: Failed to create Logging thread\n");
      pthread_attr_destroy(&attr);
      RETVALUE(RFAILED); 
   }
   pthread_attr_destroy(&attr);

   RETVALUE(ROK);
} /* end of smCreateLogThr */

/** 
 * @brief This function is used to write logs to file or socket
 *  
 * @details
 * This function is used to write logs to file or socket
 *
 *     Function: wrDiagWriteLogs
 *
 *         Processing steps:
 *           - write logs to file or socket
 *           
 * @param[in] sndBuf  : Flat buffer 
 * @param[in] msgLen  : length of Flat buffer
 * @return S16
 *        -# ROK     - success.
 *        -# RFAILED - failure.
 */       
PRIVATE S16 wrDiagWriteLogs(U8 *sndBuf, U32 msgLen)
{
   if(wrDiagSndToPort == TRUE)
   {
      if (sendto(wrDiagSockFd, sndBuf, msgLen, 0,
               (const struct sockaddr*)&(wrDiagDstAddr.u),wrDiagDstAddr.len)==-1)
      {
         ALARM("sendto() failed\n");
      }
   }
   SS_DIAG_FILE_WRITE(sndBuf, 1, msgLen, wrFp);
   RETVALUE(ROK);
} /* end of wrDiagWriteLogs */
#endif


/********************************************************************30**

           End of file:    $SID$

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
$SID$        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/

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
     Type:     C source file
     Desc:     This file contains logging framework implementation.
     File:     rl_soc.c
     Sid:      lf_soc_qcom.c@@/main/tenb_5.0_RIB_GA/1 - Mon Feb 15 12:52:12 2016
     Prg:      Total eNB
	  Auth:		Sushant , Feb 2015

*********************************************************************21*/
/**********************************************************************
 @ author : Sushant Kumar 
 @ description: This source file has SOC specific functions required for 
 logging framework
************************************************************************/

#include "rl_interface.h"
#include "rl_rlog.h"
#include "rl_platform.h"

#include "gen.h"
#include "ssi.h"
#include "ss_msg.h"
#include "ss_task.h"
#include "gen.x"
#include "ssi.x"
#include "ss_queue.h"
#include "ss_queue.x"
#include "ss_task.x"
#include "ss_msg.x"
#ifndef LTE_QCOM_L2_RLOG
#include "cm_inet.h"
#include "cm_inet.x"
#endif
#include "ctf.h"

#include "rl_interface.h"
#include "rl_soc.h"

#define NUM_LOG_BLOCK_BUF 4

Data *gLogBlockBufArray[NUM_LOG_BLOCK_BUF];

//////////////////////////////////////////////////////////////////////////
//  @Function    : rlInitLogBlock 
//  @Description : Initialize the Log Block Buffer. Allocate Four Log Block
//                 Buf's and store in an Array. Every time the index is
//                 increamented and buffer with that Index will be used for that
//                 log period. This Log Block Buf is not freed while sending to
//                 L3 through SMD.
//  @in          : void
//  @out         : Data *
//////////////////////////////////////////////////////////////////////////

PRIVATE void rlInitLogBlock(void)
{

   Data *bufLogBlock;
   U8 index = 0;

   while(index < NUM_LOG_BLOCK_BUF)
   {

      /* Call api for Log Block Buf  memory allocation for 
         allocating L2LOG_BUFF_BLOCK_SIZE (2048) 50*4*10 = 2000 bytes buffer */

      if(SGetStaticBuffer(0, /* value does not matter in this case  */
               0, /* value does not matter in this case  */ 
               &bufLogBlock,
               L2LOG_BUFF_BLOCK_SIZE, /* size to be allocated */
               0) != ROK) /* Memory type is SS_SHARABLE_MEMORY */
      {
         QU_CRITLOG(QU_LOG_MESSAGE,"Log-Buffer initialization Failed at L2 ");
         RETVOID;
      }
     
      gLogBlockBufArray[index] = bufLogBlock;
      index++;
   }

   RETVOID;
}
//////////////////////////////////////////////////////////////////////////
//  @Function    : rlAllocBufferLogBlock 
//  @Description : Allocate the buffer for L2 Log BLock and return the Buffer
//                 It returns the buffer holding one byte. This byte is required
//                 while sending the buffer to L3(KRAIT), to ccomodate the
//                 message type over SHM interface from Hexagon to krait.
//  @in          : void
//  @out         : Data *
//////////////////////////////////////////////////////////////////////////

PRIVATE Data* rlAllocBufferLogBlock(void)
{
   Data * bufLogBlock;
   static U8 index;

   bufLogBlock = gLogBlockBufArray[index];

   index++;

   index = index % NUM_LOG_BLOCK_BUF;

   RETVALUE((bufLogBlock + 1));
}
//////////////////////////////////////////////////////////////////////////
//  @Function    : rlSendL2LogBlockBuf 
//  @Description : Sends the L2 Log BLock to L3.
//                 Before sending it moves the pointer back by one byte. This 
//                 is required to accomodate the message type over SHM interface 
//                 while sending the buffer from Hexagon to krait(L3).
//  @in          : Data *
//  @out         : void
//////////////////////////////////////////////////////////////////////////
PRIVATE void rlSendL2LogBlockBuf(Data * l2rlogBuf_post)
{
   Data *log = l2rlogBuf_post - 1;

#ifdef LTE_QCOM_L2_RLOG
   ssdRouteLogRsysSmd(log);
#else
   UNUSED(log);
#endif
   RETVOID;
}

//////////////////////////////////////////////////////////////////////////
//  @Function    : rlInitL2SocSpecific 
//  @Description : This function initializes L2 Specific variables at 
//                 starting time
//  @in          : void
//  @out         : void
//////////////////////////////////////////////////////////////////////////

PUBLIC void rlInitL2SocSpecific(void)
{
   g_logLevel = L_EVENT;
#ifndef RLOG_ENABLE_TEXT_LOGGING
   rlInitLogBlock();
   /* Fill global Pst structure to send message to Application */
   g_l2rlogBuf = rlAllocBufferLogBlock();

   if(NULLP == g_l2rlogBuf )
   {
      g_writeCirBuf = 1; /* This will avoid writing logs 
                            to log buffers */
      RETVOID;
   }
   g_l2LogBufStartPtr = g_l2rlogBuf + sizeof(g_l2LogBufLen);

#endif
}

//////////////////////////////////////////////////////////////////////////
//  @Function    : processL2LogBuff
//  @Description : This function will get triggered after every 10 tti 
//                 and this will reset log buffers for L2 and also  sends log
//                 buffer to L3 which was collected
//  @in          : void
//  @out         : void
//////////////////////////////////////////////////////////////////////////

PUBLIC void processL2LogBuff(void)
{
#ifndef RLOG_ENABLE_TEXT_LOGGING
   if(g_l2LogBufLen > 0)
   {
      Data *l2rlogBuf_post = NULLP;

      g_writeCirBuf = 1; /* This will avoid contention while 
                            switching log buffers */

      if(NULLP != g_l2rlogBuf)
      {

         l2rlogBuf_post = g_l2rlogBuf; /* copy logBufferPointer for later use */


#if 0
         *((U32 *)g_l2rlogBuf) = g_l2LogBufLen; /* length in first four bytes 
                                                   of Log Buffer */
#else

         g_l2rlogBuf[0] = g_l2LogBufLen & 0xFF;
         g_l2rlogBuf[1] = (g_l2LogBufLen >> 8) & 0xFF;
         g_l2rlogBuf[2] = (g_l2LogBufLen >> 16) & 0xFF;
         g_l2rlogBuf[3] = (g_l2LogBufLen >> 24) & 0xFF;

#endif
         /* Post rlog buffer to application */
         rlSendL2LogBlockBuf(l2rlogBuf_post);
      }

      g_l2rlogBuf = rlAllocBufferLogBlock();

      if(NULLP != g_l2rlogBuf)
      {
         g_l2LogBufStartPtr = g_l2rlogBuf + sizeof(g_l2LogBufLen);
         g_l2LogBufLen = 0;

         g_writeCirBuf = 0; /* To avoid contention while switching log buffers */
      }
   }
   else
   {
   }
#endif   
   RETVOID; 
}

//////////////////////////////////////////////////////////////////////////
//  @Function    : rlGetL2LogBufPtr 
//  @Description : This function reads LogBufPointer and Length of log Buffer 
//                 and stores pointer and length variable so that these 
//                 values can be used while reading L2 Log Buffer 
//  @in          : mBuf     - Log Buffer
//                 logLen   - Length of log Buffer
//                 logPtr   - Address of log Buffer
//  @out         : void
//////////////////////////////////////////////////////////////////////////

PUBLIC void rlGetL2LogBufPtr
(
   void *mBuf,
   U32 *logLen,
   Data **logPtr
)
{

   /* Get Buffer pointer and length */
   *logPtr = ((Data *)mBuf) + sizeof(logLen);
   *logLen = *((U32 *) mBuf);
}

//////////////////////////////////////////////////////////////////////////
//  @Function    : rlInvalidateL2LogsInCache 
//  @Description : Invalidate L2 Logs from cache after reading it from Buffer
//  @in          : void
//  @out         : void
//////////////////////////////////////////////////////////////////////////

PUBLIC void rlInvalidateL2LogsInCache
(
   Data *ptr,
   U32   len
)
{
   /* For T3300 Invalidating buffer is not required */
   RETVOID;
}


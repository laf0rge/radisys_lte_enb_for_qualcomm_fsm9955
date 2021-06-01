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



///////////////////////////////////////////////////////////////////////////////////////////////////
// @ author Vikram
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static const char* RLOG_MODULE_NAME="NMM";
static int RLOG_FILE_ID=114;
static int RLOG_MODULE_ID=1;
#include<stdlib.h>
#include<stdio.h>
#ifndef NMM_PHYINTERFACE_CPP_
#define NMM_PHYINTERFACE_CPP_

#include <sys/select.h>
#include <mqueue.h>

#include "wr.h"
#include "wr_nmm.h"
#include "wr_smm_init.h"
#include "cm_mem.h"
#include "cm_mem.x"
#include <unistd.h>
#include "LtePhyApi.h"

#define MAX_NMM_MSG_LEN 1024

void* phyNmmListner(void* pArg);
void  NmmDispatch(U32 size, PTR msg);
void dump_hex(int len, U8 *p);
void fillAntennaPortInfo(U32 *antennaPort);

mqd_t g_RemMsgQIdSend;
mqd_t g_RemMsgQIdRecv;


///////////////////////////////////////////////////////////////////////////////////////////////////
//    @Function : initClNmmRecvTask
//        @param[in]: bMsgType - uint8_t
//    This funtions is used to create a thread which listens for NMM message from BRDCM L1.
///////////////////////////////////////////////////////////////////////////////////////////////////

void initNmmTask()
{
   pthread_t taskId;
   pthread_attr_t attr;
   struct mq_attr qattr;
   int policy;

   struct sched_param schedParams;
   qattr.mq_flags=0;
   qattr.mq_maxmsg=100;
   qattr.mq_msgsize=1024;
   qattr.mq_curmsgs=0;

   g_RemMsgQIdSend = mq_open("/REMQ", O_WRONLY | O_CREAT,(S_IRWXU|S_IRWXG|S_IRWXO),&qattr);
   g_RemMsgQIdRecv = mq_open("/ENBQ", O_RDONLY | O_CREAT,(S_IRWXU|S_IRWXG|S_IRWXO),&qattr);

   if (g_RemMsgQIdSend == -1 || g_RemMsgQIdRecv == -1)
   {
      /*127802*/
     RLOG0(L_ERROR, "Failed to open REMQ ..exiting");
     perror("NMMTask:");
     exit(0);
   }

   pthread_attr_init(&attr);

   pthread_create(&taskId, &attr, phyNmmListner, NULL);

   schedParams.sched_priority = SCHED_RR;
   pthread_getschedparam(taskId, &policy, &schedParams);
   pthread_setschedparam(taskId, SCHED_RR, &schedParams);

}

void NmmDispatch(U32 size, PTR msg)
{
   dump_hex(size, (U8*)msg);

   if (mq_send(g_RemMsgQIdSend, (const char*)msg, size, ((CPHY_MSG_HEADER *)msg)->msg_code) < 0 )
      {
         RLOG0(L_ERROR,"NmmDispatch: Failed to send message to REM");
      }
   RLOG0(L_DEBUG, "\nREM : Msg sent to remMsgQ...\n");
   return;
}


void* phyNmmListner(void* pArg)
{
   U8 szNmmMsg[MAX_NMM_MSG_LEN];
   /*U8* pNmmMsg=NULL;*/
   int nmmMsgLen, nmmMsgType, dest;
   Buffer *mBuf = NULLP;
   Pst     pst;
   /*127802*/
    RLOG0(L_DEBUG, "NMM Task Started ....");

#ifdef WR_RSYS_OAM
   sleep(8);
   /*U32 antennaPort;*/
   sleep(8);
#endif

   while( (nmmMsgLen = mq_receive(g_RemMsgQIdRecv, (char*)szNmmMsg, MAX_NMM_MSG_LEN, (unsigned*)&dest)) >= 0 )
   {
      if( nmmMsgLen == 0 )
         continue;
      nmmMsgType = *(U16*)szNmmMsg;
      printf("\n REM : Receieved NMM Msg %d with Len=%d from REM dest=%d  \n", nmmMsgType,nmmMsgLen, dest);
      dump_hex(nmmMsgLen, (U8*)szNmmMsg);
      #if 0
      printf("\n");
      for(i=0;i<nmmMsgLen;i++)
         printf("%x ",(U32*)szNmmMsg[i]);
      printf("\n");
      #endif

      /*when we receive the message, same is converted into Mbuf and Posted to the NMM TAPA Task.
        Later based the Message type, Appropriate action is taken.
        */

      /*SS_MAX_REGS-1 is the Timer Region as as of now TIMER is not running,
        hence using the same to post message to CL */
      /* SGetMsg(SS_MAX_REGS-1, 0, (Buffer **) &mBuf);   */ //

      SGetMsg(1, 1, (Buffer **) &mBuf);
      cmMemset((U8*)&pst,0x00,sizeof(Pst));
      pst.region = YS_MEM_REGION;
      pst.pool = YS_POOL;
      pst.dstEnt = ENTTF;
      pst.srcEnt = ENTWR;
      pst.dstProcId = SM_YS_PROC;
      pst.srcProcId = SM_WR_PROC;
      pst.event = EVTCTFSNFREQMSGS;
      /* copy recieved message to mBuf */
      SAddPstMsgMult((Data*)szNmmMsg,nmmMsgLen,mBuf);
      SPstTsk(&pst,mBuf);

      printf("\n REM : MSG Sent to CL\n");
   }

     /*127802*/
   RLOG0(L_INFO, "NMM Task Exiting"); 
   perror("NMMTask:");

   return NULL;
}

void dump_hex(int len, U8 *p)
{
   for(int i=0; i<len; i++)
   {
      //RLOG1(L_DEBUG,"%x ", p[i]);
      if( i % 10 == 0 )
      {
         //RLOG0(L_DEBUG,"\n");
      }
   }
}
/* This function converts mBuf received from Convergence layer to (Data *)
   and then (Data *) is passed to message REM Message Queue */
void wrHandleSnfMsgs(Buffer *ptrClMsg)
{
   MsgLen msgLength;
   U8 data[MAX_NMM_MSG_LEN];
   /* Get length of the data in buffer */
   SFndLenMsg(ptrClMsg,&msgLength);
   /* Get the data in data buffer*/
   memcpy(data,ptrClMsg->b_cont->b_rptr,msgLength);
   //SGetDataFrmMsg(ptrClMsg,(Data *)data,0,msgLength);
   /* Send Message in Message queue */
   /* Free buffer */
   SPutMsg(ptrClMsg);
   NmmDispatch(msgLength,(PTR)data);
}

#endif

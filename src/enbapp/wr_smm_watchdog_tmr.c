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
               

     File:     wr_smm_watchdog_tmr.c

     Sid:      fc_smm_watchdog_tmr.c@@/main/broadcom_changes/1 - Fri Jun 21 16:45:54 2013

     Prg:      mhalihal

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="SMM";
static int RLOG_MODULE_ID=2;
static int RLOG_FILE_ID=20;

#include "wr.h"
#include "wr_smm_init.h"
#include "wr_smm_watchdog_tmr.h"
#include "wr_emm.h"
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

extern U32 wrMonitorTtiInd;
extern U32 wrNonrtTick;
PRIVATE WrSockAddr wrWatchDogDestAddr;
PRIVATE S32    wrWatchSockFd = 0;
PRIVATE pid_t   enbPid = 0;

PRIVATE S16  wrCheckNonrtTick(Void);
Void wrWatchDogHdl(int arg);
PRIVATE S16 wrCheckForTtiRecv(Void);
PRIVATE Void wrSendKick(Void);
PRIVATE S16 wrSendWatchDogMsg(U8 *buff, U32 msgLen);
PRIVATE Void wrStartWatchDogTmr(Void);
PRIVATE S16 wrCheckEmmCellState(Void);

extern void (*watchdogSigHandler)(int);
U16 softTimeout;
U16 hardTimeout;
U8 missedKick;
Bool sendKick = TRUE;

/*
*
*       Fun:   wrStartWatchDogTmr
*
*       Desc: start linux timer with softTimeout interval,
*
*       Ret:   Void
*
*       Notes: ssi timer are tied with the lower arm tti if the ttis are not 
*             receieved ssi timers also wont expire to avoid that we are using system timer
*             rather ssi timer
*
*       File:  wr_smm_watchdog_tmr.c
*
*/

PRIVATE Void wrStartWatchDogTmr()
{
  struct itimerval tout_val;
  tout_val.it_interval.tv_sec = 0;
  tout_val.it_interval.tv_usec = 0;
  tout_val.it_value.tv_sec = softTimeout; 
  tout_val.it_value.tv_usec = 0;
  setitimer(ITIMER_REAL, &tout_val,0);
}/*end of wrStartWatchDogTmr*/

/*
*
*       Fun:   wrWatchDogHdl
*
*       Desc:  function to handel timer expire once the timer expired check is
*               lower arm is sending tti or not if ttis are received lower arm is working fine
*               we can send kick if not stop sending kick so that system may restart.
*
*       Ret:   Void
*
*       Notes: None
*
*       File:  wr_smm_watchdog_tmr.c
*
*/

PUBLIC Void wrWatchDogHdl(int arg)
{
   S16 ret = RFAILED;
   S16 ret1 = RFAILED;
   S16 ret2 = RFAILED;
   ret = wrCheckForTtiRecv(); 
   ret1 = wrCheckNonrtTick(); 
   ret2 = wrCheckEmmCellState();

   if(((ret != ROK) || (ret1 != ROK)) && (ret2 == ROK))
   {
      RLOG0(L_FATAL, "Phy or Non RT thread are not responding");
      missedKick++;
   }
   else
   {
     wrSendKick();
     missedKick = 0;
   }
   if (missedKick < 3)
   {
   wrStartWatchDogTmr();
   }
}

/** @brief This function is used to indicate tti are received or not 
 *
 * @details
 *
 *     Function: wrCheckForTtiRecv
 *
 *         Processing steps:
 *         - . 
 *
 *
 * @param[out] trans: true if ttis are comming else false.  
 * @param[in] void
 * @return void 
 */
PRIVATE S16 wrCheckForTtiRecv()
{
#ifdef WR_WATCHDOG
  static U32 preValueTti = 0;
  if(wrMonitorTtiInd == 0)
  {
    RETVALUE(ROK);
  } 
  else if(preValueTti != wrMonitorTtiInd)
  {
    preValueTti = wrMonitorTtiInd;
    RETVALUE(ROK);
  }
#endif
  RETVALUE(RFAILED);
}
/*
*
*       Fun:   wrFillWatchDogHdr
*
*       Desc:  wrFillWatchDogHdr: function to fill watch dog header parameter
*
*       Ret:   Void
*
*       Notes: None
*
*       File:  wr_smm_watchdog_tmr.c
*
*/
PRIVATE Void wrFillWatchDogHdr(U8 *msgBuff, U32 msgType, U32 *len)
{
  U32 srcEnt = 0;
  U32 dstEnt = 0;
  U32 tmpMsg = 0;
  pid_t tmpId = 0;

  srcEnt = htonl(srcEnt);
  cmMemcpy(&msgBuff[*len], (U8*)&srcEnt, sizeof(srcEnt));
  *len += sizeof(srcEnt);

  dstEnt = htonl(dstEnt);
  cmMemcpy(&msgBuff[*len], (U8*)&dstEnt, sizeof(dstEnt));
  *len += sizeof(dstEnt);

  tmpMsg = htonl(msgType);
  cmMemcpy(&msgBuff[*len], (U8*)&tmpMsg, sizeof(tmpMsg));
  *len += sizeof(tmpMsg);

  tmpId = htonl(enbPid);
  cmMemcpy(&msgBuff[*len], (U8*)&tmpId, sizeof(tmpId));
  *len += sizeof(tmpId);
}/*end of wrFillWatchDogHdr*/

/*
*
*       Fun:   wrWatchdogSendRemovePid
*
*       Desc:  function to send remove pid request to watchdog
*
*       Ret:   Void
*
*       Notes: None
*
*       File:  wr_smm_watchdog_tmr.c
*
*/
PUBLIC Void wrWatchdogSendRemovePid(int dummy)
{
  U8 msgBuff[100];
  U32 msgLen = 0;

  /*fill the header*/
  wrFillWatchDogHdr(msgBuff, WR_MSG_TYPE_WATCHDOG_KICK_STOP_REQ, &msgLen);

   RLOG0(L_ERROR,"RADISYS:Send Kick stop message");
   printf("RADISYS:Send Kick stop message");
  if(ROK != wrSendWatchDogMsg(msgBuff, msgLen))
  {
     RLOG0(L_ERROR,"Failed to Send Kick Stop message to watch dog Application");
  }

  msgLen = 0;
  wrFillWatchDogHdr(msgBuff, WR_MSG_TYPE_WATCHDOG_REMOVE_PID_REQ, &msgLen);
   RLOG0(L_ERROR,"RADISYS:Send remove PID message");
   printf("RADISYS:Send remove PID message");

  if(ROK != wrSendWatchDogMsg(msgBuff, msgLen))
  {
     RLOG0(L_ERROR,"Failed to Send Remove PID message to watch dog Application");
  }

   sendKick = FALSE;
}/*end of wrWatchdogSendRemovePid*/

/*
*
*       Fun:   wrRegWatchDog
*
*       Desc:  wrRegWatchDog : function to refister with watch dog application.
*
*       Ret:   Void
*
*       Notes: None
*
*       File:  wr_smm_watchdog_tmr.c
*
*/
PUBLIC Void wrRegWatchDog()
{
   U8 msgBuff[100];
   U32 msgLen = 0;
   U16 strLen = 0;
   U16 p = 0;
   S8 appName[]="enodeb";
   sigset_t set;
   /*get pid of the enodeb*/
   enbPid  = getpid();

   /*unblock the signal first this is needed ssi is blocking some signal 
     In pal unless unblock if you register for signal we wont receive signal
     for the platform we no need to unlock the signal*/
   sigemptyset(&set);
   sigaddset(&set, SIGALRM);
   sigprocmask(SIG_UNBLOCK, &set, NULL);
   signal(SIGALRM, wrWatchDogHdl); /* set the Alarm signal capture */
   /*start the timer which will expire every softTimeout time interval we receive alram signal 
     once we receive check is the lower arm sending tti or not if the tti's are not received we will stop sending 
     kick to watch dog application */
   wrStartWatchDogTmr();

   cmMemset((U8*)&wrWatchDogDestAddr, 0, sizeof(wrWatchDogDestAddr));
   /*open udp socket to send message watchdog process*/
   if(smCfgCb.enbIpAddr[WR_IPV4_IDX].type == CM_IPV4ADDR_TYPE)
   {
     wrWatchSockFd = socket(PF_INET, SOCK_DGRAM, 0);
     wrWatchDogDestAddr.len = sizeof(struct sockaddr_in);
     wrWatchDogDestAddr.type = CM_IPV4ADDR_TYPE;
     wrWatchDogDestAddr.u.addr.sin_family = AF_INET;
     wrWatchDogDestAddr.u.addr.sin_port = htons(WR_WATCHDOG_TMR_PORT);
#ifdef IPV6_SUPPORTED
     cmInetPton((CmInetIpAddr*)&wrWatchDogDestAddr.u.addr.sin_addr, "127.0.0.1");
#endif /* IPV6_SUPPORTED */

   }
   else if(smCfgCb.enbIpAddr[WR_IPV6_IDX].type == CM_IPV6ADDR_TYPE)
   {
     wrWatchSockFd = socket(PF_INET6, SOCK_DGRAM, 0);
     wrWatchDogDestAddr.len = sizeof(struct sockaddr_in6);
     wrWatchDogDestAddr.type = CM_IPV6ADDR_TYPE;
     wrWatchDogDestAddr.u.addr6.sin6_family = AF_INET6;
     wrWatchDogDestAddr.u.addr6.sin6_port = htons(WR_WATCHDOG_TMR_PORT);
#ifdef IPV6_SUPPORTED
     cmInetPton6((CmInetIpAddr6*)&wrWatchDogDestAddr.u.addr6.sin6_addr, "::1");
#endif /* IPV6_SUPPORTED */

   }
   if(wrWatchSockFd < 0)
   {
      RLOG0(L_FATAL,"failed to open socket for watchdog");
   }
   /*fill the header*/
   wrFillWatchDogHdr(msgBuff, WR_MSG_TYPE_WATCHDOG_ADD_PID_REQ, &msgLen);

   strLen = strlen(appName);
   p = strLen;
   strLen = htons(strLen);
   cmMemcpy(&msgBuff[msgLen], (U8*)&strLen, sizeof(strLen));
   msgLen += sizeof(strLen);

   cmMemcpy(&msgBuff[msgLen], (U8*)&appName,p);
   msgLen += strLen;

   if(ROK != wrSendWatchDogMsg(msgBuff, msgLen))
   {
      RLOG0(L_ERROR,"Fail to Send Register Message to watch dog Application");
   }
   else
   {
      watchdogSigHandler = &wrWatchdogSendRemovePid;
   }
}/*end of wrRegWatchDog*/
/*
*
*       Fun:   wrSendKick
*
*       Desc:  function to send kick request to watchdog applicatio.
*
*       Ret:   Void
*
*       Notes: None
*
*       File:  wr_smm_watchdog_tmr.c
*
*/
PRIVATE Void wrSendKick()
{
   if(TRUE == sendKick)
   {
      U8 msgBuff[100];
      U32 msgLen = 0;
      U16 softLimit = 0;
      U16 hardLimit = 0;

      /*fill the header*/
      wrFillWatchDogHdr(msgBuff, WR_MSG_TYPE_WATCHDOG_KICK_REQ, &msgLen);

      softLimit = htons(softTimeout);
      cmMemcpy(&msgBuff[msgLen], (U8*)&softLimit, sizeof(softLimit));
      msgLen += sizeof(softLimit);

      hardLimit = htons(hardTimeout);
      cmMemcpy(&msgBuff[msgLen], (U8*)&hardLimit, sizeof(hardLimit));
      msgLen += sizeof(hardLimit);

      if(ROK != wrSendWatchDogMsg(msgBuff, msgLen))
      {
         RLOG0(L_ERROR,"Fail to Send Kick Message to watch dog Application");
      }
   }
}/*end of wrSendKick*/

/*
*
*       Fun:   wrSendWatchDogMsg
*
*       Desc:  function to send message to watchdog application.
*
*       Ret:   ROK on success, RFAILED on error
*
*       Notes: None
*
*       File:  wr_smm_watchdog_tmr.c
*
*/
PRIVATE S16 wrSendWatchDogMsg(U8 *buff, U32 msgLen)
{
  S16 ret = 0;
  ret = sendto(wrWatchSockFd, buff, msgLen, 0,(const struct sockaddr *)&(wrWatchDogDestAddr.u),wrWatchDogDestAddr.len);
  if(ret == -1)
  {
    RETVALUE(RFAILED); 
  }
  RETVALUE(ROK);
}/*end of wrSendWatchDogMsg*/

/** @brief This function is used to indicate whether ticks from DAM are received 
 * or not. 
 *
 * @details
 *
 *     Function: wrCheckNonrtTick
 *
 *         Processing steps:
 *         - . 
 *
 *
 * @param[out] trans: true if ticks are coming  from DAM module else false.  
 * @param[in] void
 * @return S16 
 */

PRIVATE S16 wrCheckNonrtTick()
{
  static U32 preValueTick = 0;
  if(wrNonrtTick == 0)
  {
    RETVALUE(ROK);
  } 
  else if(preValueTick != wrNonrtTick)
  {
    preValueTick = wrNonrtTick;
    RETVALUE(ROK);
  }
  RETVALUE(RFAILED);
}

PRIVATE S16 wrCheckEmmCellState()
{  
   S16 idx;
   for(idx = 0; idx < WR_EMM_MAX_CELLS; idx++) 
   {
      if ((wrEmmCb.cellCb[idx] != NULLP) &&  
            (WR_CELL_STATE_PWR_DOWN == wrEmmCb.cellCb[idx]->cellState))
      {
            RLOG0(L_INFO, "Cell is Down, because MME is Down");
            RETVALUE(RFAILED);
      }
      if ((wrEmmCb.cellCb[idx] != NULLP) &&
            (WR_CELL_STATE_RECFG == wrEmmCb.cellCb[idx]->cellState))
      {
            RLOG0(L_INFO, "Cell is Down, because PCI modification is in progress by SON \n");
            RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}

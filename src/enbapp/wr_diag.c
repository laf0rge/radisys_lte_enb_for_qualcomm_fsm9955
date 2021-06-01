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
               

     File:     wr_diag.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="EMM";
static int RLOG_MODULE_ID=1;
static int RLOG_FILE_ID=45;
#include "wr.h"
#include "wr_umm.h"
#include "unistd.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


/* @brief Public function forward declaration */
PUBLIC S16  wrDiagCreatSock(U8 *ipaddr, U32 port);
PUBLIC S16  wrDiagPrntPlmnId(WrPlmnId *plmnId, U8 diagLvl);

/* @brief Public declaration */
PUBLIC WrSockAddr wrDiagDstAddr;
PUBLIC U32 wrDiagSockFd; 
/**
 *  @brief This is an utility function to print PlmnId from the message
 *  @details
 *  This is an utility function to print PlmnId from the message
 *  
 *       Function: wrDiagPrntPlmnId
 *  
 *           Processing steps:
 *           -  decode the PLMN from the message
 *           - provide fixed log
 *  
 *   @param[in]  plmnId : pointer to PLMN 
 *   @param[in]  diagLvl: Diagnostic level
 *   @return S16
 *            -# Success : ROK
 *            -# Failure : RFAILED
 */
PUBLIC S16 wrDiagPrntPlmnId(WrPlmnId *plmnId, U8 diagLvl)
{
   U32 mcc = 0;
   U32 mnc = 0;
   if(NULLP == plmnId)
   {
      RETVALUE(RFAILED);
   }
   if(plmnId->numMncDigits == 2)
   {
      mcc = (plmnId->mcc[2] | (plmnId->mcc[1] <<4) | (plmnId->mcc[0] <<8));
      mnc = (plmnId->mnc[1] | (plmnId->mnc[0] <<4));
   }
   else
   {
      mcc = (plmnId->mcc[2] | (plmnId->mcc[1] <<4) | (plmnId->mcc[0] <<8));
      mnc = (plmnId->mnc[2] | (plmnId->mnc[1] <<4) | (plmnId->mnc[0] <<8));
   }
   switch(diagLvl)
   {
      case 0:
         RLOG2(L_DEBUG, "PLMN ID: MCC [%03x] MNC [%02x]", mcc, mnc);
         break;
      case 1:
         RLOG2(L_DEBUG, "PLMN ID: MCC [%03x] MNC [%02x]", mcc, mnc);
         break;
      case 2:
         RLOG2(L_DEBUG, "PLMN ID: MCC [%03x] MNC [%02x]", mcc, mnc);
         break;
      case 3:
         RLOG2(L_DEBUG, "PLMN ID: MCC [%03x] MNC [%02x]", mcc, mnc);
         break;
      case 4:
         RLOG2(L_DEBUG, "PLMN ID: MCC [%03x] MNC [%02x]", mcc, mnc);
         break;
      default:
         RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of wrDiagPrntPlmnId */


/** 
 * @brief This function is used to create a socket is used to send logs
 *  
 * @details
 * This function is used to create a socket is used to send logs
 *
 *     Function:wrDiagCreatSock 
 *
 *         Processing steps:
 *           - create socket for sending logs
 *           
 * @param[in] ipaddr : Local IP address
 * @param[in] port   : Local Port
 * @return S16
 */   
PUBLIC S16 wrDiagCreatSock(U8 *ipaddr, U32 port)
{
   S32 ret;
   CmInetAddrInfo addrInfo;
   CmInetAddrInfo *res;
   CmInetAddrInfo *tmp;
   ret = ROK;
   U16 domain = 0;

   cmMemset((U8*)&addrInfo, 0, sizeof(CmInetAddrInfo));
   memset((char *) &wrDiagDstAddr, 0, sizeof(wrDiagDstAddr));

   /* set the caller to accept any address family */
   addrInfo.ai_family = PF_UNSPEC;
   /* set the socket type */
   /* We are setting to connection oriented socket type for 
    * retrieving the IP address type 
    */
   addrInfo.ai_socktype = SOCK_STREAM;
   /* Fill the hosts canonical(real) name in the result */
   addrInfo.ai_flags |= AI_CANONNAME;
   ret = cmInetGetAddrInfo((S8 *)ipaddr, NULLP,&addrInfo,&res);
   if(ret != ROK)
   {
      RLOG_STR(L_ERROR, "wrDiagCreatSock: IP addr validation failed:[%s]", (S8*)ipaddr);
      RETVALUE(ret);
   }
   tmp = res;
   if (res)
   {
      switch (res->ai_family)
      {
        case AF_INET:
          domain = AF_INET;
          wrDiagDstAddr.len = sizeof(struct sockaddr_in);
          wrDiagDstAddr.type = CM_IPV4ADDR_TYPE;
          /* Set address family of IPV4 */
          wrDiagDstAddr.u.addr.sin_family = AF_INET;
          wrDiagDstAddr.u.addr.sin_port = htons(port);
          /* Copy IPv4 address(4bytes) into the destination */ 
          cmMemcpy((U8 *)&(wrDiagDstAddr.u.addr.sin_addr), 
             (U8 *) &((struct sockaddr_in *) res->ai_addr)->sin_addr, 4);
          break;
        case AF_INET6:
          domain = AF_INET6;
          wrDiagDstAddr.len = sizeof(struct sockaddr_in6);
          /* Set address family of IPV6 */
          wrDiagDstAddr.u.addr6.sin6_family = AF_INET6;
          wrDiagDstAddr.u.addr6.sin6_port = htons(port);
          /* Copy IPv6 address(16bytes) into the destination */ 
          cmMemcpy((U8 *)(&wrDiagDstAddr.u.addr6.sin6_addr.s6_addr), 
            (U8 *) &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr, 16);
          break;
      }
      res = res->ai_next;
   }

   cmInetFreeAddrInfo(tmp);

   if (( wrDiagSockFd = socket(domain, SOCK_DGRAM, 0))==-1)
   {
      ALARM("failed to open socketfd\n");
      RETVALUE(RFAILED);
   }

   ALARM("Sending logs to ipaddr = %s port %lu\n",ipaddr, port);
   RETVALUE(ROK);
} /* end of wrDiagCreatSock */


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

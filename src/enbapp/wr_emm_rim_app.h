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

     Type:     C include file

     Desc:     This file contains 
               

     File:     wr_emm_rimApp.h

     Sid:      wr_emm_rim_app.h@@/main/Br_Tenb_Rim_Csfb_Intg/1 - Tue Oct 14 09:52:20 2014

     Prg:      Jkaur 

*********************************************************************21*/



#ifndef __WR_EMM_RIMAPP_H__
#define __WR_EMM_RIMAPP_H__

#include "wr_emm_rim.h"

#ifdef __cplusplus
extern "C" {
#endif


#define  WR_EMM_RIM_GERAN_SI_LEN         21  /* Expected length for NACC -SI */
#define  WR_EMM_RIM_GERAN_PSI_LEN        22  /* Expected length for NACC -PSI */

/* Error Type to initimate the RIM User for error -
 * 1. Timer expiry and retransmission count reached maximum
 * 2. RIM PDU error received or
 * 3. RIM Application error receied
 */
#define  WR_EMM_RIM_ERR_TMR_EXP        1   /* Timer expiry Cause */
#define  WR_EMM_RIM_ERR_PROT           2   /* RIM PDU error Cause */
#define  WR_EMM_RIM_ERR_RI_VAL_FAILED  3   /* RIM APP error Cause */
#define  WR_EMM_RIM_ERR_APP_RECVD      4   /* RIM APP error Cause */

#define  WR_EMM_RIM_NACC_MAX_SI        10    /* Max  NACC SI */
/** @brief This structure used to indicate about the RIR procedure failure 
 * and contains the following 
  WrRimAppId         appId;   
  U8                 errType; 
  U8                 errCause; 
  WrRimRptCell       rptCellId;
*/
typedef struct wrRimErrInd
{
  WrRimAppId          appId;      /* RIM Application Identity */
  U8                  errType;    /* Timer expiry or RIM PDU error or RIM APP
                                     error*/ 
  U8                  errCause;   /* cause if errType is  RIM PDU error or 
                                     RIM APP error*/
  WrRimRptCell        rptCellId;  /* Reporting cell Identifier */
}WrRimErrInd;

/** @brief This structure used to indicate the RIM User about the Application
 * Information or Application error received in RAN_INFORMATION PDU and
 * contains the following 
   U8                rptType; 
   WrRimAppId        appId; 
   TknStrOSXL        utraSI; 
   WrRimNacc         nacc; 
   WrRimRptCell      rptCellId; 
   TknU8             appErrCause; 
*/ 
typedef struct wrEmmRimRspInfo
{
   U8                 rptType;  /* RI report Type - multiple or end */
   WrRimAppId         appId;    /* RIM application Identity */
   union 
   {
     TknStrOSXL           utraSI; /* UTRA SI octet string */
     WrRimNacc            nacc;   /* Indicate GERAN SI or PSI octet string */
     /* TODO: structures for the remaining applciations could 
        be included based on the support
     */
   } app;
   WrRimRptCell  rptCellId;    /* Reporting cell Identity */
   TknU8         appErrCause;  /* Application error cause,this will be present,
                                  only if RI received with error container. */ 
} WrEmmRimRspInfo;


EXTERN S16 wrEmmRimCfgReq (WrRimCfg  *rimCfg);
EXTERN S16 wrEmmRimInfoReq( WrRimReqInfo *rimReqInfo);
EXTERN S16 wrEmmRimMmeDownHdlr(Void);
EXTERN S16 wrEmmRimShutdown(Void);
EXTERN S16 wrEmmRimPrcTmrExp(PTR cb,S16 event);   /* RIM APP will invoke - wrRimTimoutHdl API of rim protocol and take required 
                                             action based on that */

EXTERN S16 wrEmmPrcMmeDirectInfoTransfer(U32 peerId, S1apPdu *pdu);
EXTERN S16 wrEmmBldEnbDirectInfoTransfer(S1apPdu  **pdu, Buffer *rimPdu,WrRimRoutingInfo *dstAddr); 
EXTERN S16 wrEmmRimPrcRimPdu(TknStrOSXL *rimPdu);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __WR_EMM_RIM_H__ */


/********************************************************************30**

           End of file:     wr_emm_rim_app.h@@/main/Br_Tenb_Rim_Csfb_Intg/1 - Tue Oct 14 09:52:20 2014

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
/main/1        ---      Jkaur         1. initial release TotaleNodeB 1.1
*********************************************************************91*/


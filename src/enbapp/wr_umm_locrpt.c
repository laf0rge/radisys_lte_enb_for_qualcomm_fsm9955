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
               

     File:     wr_umm_locrpt.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_MODULE_ID=32;
static int RLOG_FILE_ID=128;

#include "wr.h"
#include "wr_utils.h"
#include "wr_umm.h"
#include "wr_umm_trans.h"
#include "wr_lwr.h"
#include "wr_umm_s1ap_msg.h"
#include "wr_ifm_s1ap.h"

/** @brief This function is responsible for handling Location report Control 
 *         message
 *
 * @details
 *
 * Function:wrUmmLocReptProcessor
 *
 *   Processing steps:
 *    -Get the Request type
 *    -If the request type is 'Update whenever UE changes cell', store the
 *     request type in UE control block and return
 *    -If the request type is 'Direct', send the Location Report message
 *     to UE
 *    -If the request type is 'Cancel whenever UE changes cell', reset the
 *     same in UE control block and return
 *
 * @param[in]  transCb: pointer to transaction control block
 * @return S16
 *  -#Success : ROK
 *  -#Failure : RFAILED
 */
PUBLIC S16 wrUmmLocReptProcessor
(
 WrUmmTransCb                *transCb
)
{
   SztRqstTyp    *rqstType;
   WrUeCb        *ueCb;
   S1apPdu       *locRptFail;

   ueCb = transCb->ueCb;

   /* Check if the UE is in handover state, if yes, send out the location control 
    * failure indication to MME with appropriate cause */
   if(ueCb->mobCtrlState != WR_HO_NORMAL)
   {
      wrUmmS1apBldLocReportFail(ueCb, &locRptFail);
      wrIfmS1apSndUeMsg(ueCb->s1ConCb->spConnId, locRptFail);
      transCb->state = WR_UMM_TRANS_DONE;
      RETVALUE(ROK);
   }

   /* Get the Request Type IE from the message */
   wrSzGetIE(&(transCb->msg->u.s1Pdu->pdu->pdu), Sztid_RqstTyp,
             (TknU8**)(&rqstType));

   if(rqstType == NULLP)
   {
      transCb->state = WR_UMM_TRANS_DONE;
      RETVALUE(RFAILED);
   }

   RLOG_ARG3(L_EVENT, DBG_CRNTI,ueCb->crnti,"[S1AP]:Received LOCATION REPORTING"
             "CONTROL [MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu] [ReqType:%d]",
             ueCb->s1ConCb->mme_ue_s1ap_id, ueCb->s1ConCb->enb_ue_s1ap_id,rqstType->eventTyp.val);

   /* Check the Request type, if the Request type is 'chage of serve cell',
    * store the value and return. If the request type is 'stop change
    * serve cell' then make the flag as false. If the request tupe is
    * 'Direct', send the Location Report Message */
   switch(rqstType->eventTyp.val)
   {
      case SztEventTypchange_of_serve_cellEnum:
      {
         ueCb->locReportType = TRUE;
         break;
      }
      case SztEventTypstop_change_of_serve_cellEnum:
      {
         ueCb->locReportType = FALSE;
         break;
      }
      case SztEventTypdirectEnum:
      {
         /* Build and send the location report message to MME */
         wrUmmBldSndLocRept(transCb);
         break;
      }
      default:
         break;
   }

   /* End the transaction here as we have done with this trasaction */
   transCb->state = WR_UMM_TRANS_DONE;
   RETVALUE(ROK);
}

/** @brief This function is Location Report Control specific transaction release
 *         function. There is nothing held by this transaction and so a simple
 *         return function.
 *
 * @details
 *
 *     Function: wrUmmLocReptTransRel
 *
 *         Processing steps:
 *         - No Release Action Performed
 *
 * @param[in]  transCb : transaction information
 * @return  ROK
 */
PUBLIC S16 wrUmmLocReptTransRel
(
 WrUmmTransCb                 *transCb
)
{
   RETVALUE(ROK);
}

/** @brief This function is called by HO module to send the Location Report
 *         message to MME if the MME is requested to report the 'Update
 *         whenever change in cell' or if the 'Request type IE' is present
 *         in the Handover Request.
 *
 * @details
 *
 *     Function: wrUmmBldSndLocRept
 *
 *         Processing steps:
 *         - Build the Location Control Message
 *         - Send the Location control message
 *
 * @param[in]  transCb : transaction information
 * @return  ROK
 */
PUBLIC Void wrUmmBldSndLocRept
(
 WrUmmTransCb                 *transCb
)
{
   WrUeCb        *ueCb;
   S1apPdu       *locRpt;

   /* Get the UeCb from the transaction CB */
   ueCb = transCb->ueCb;

   wrUmmS1apBldLocReport(ueCb, &locRpt);
   wrIfmS1apSndUeMsg(ueCb->s1ConCb->spConnId, locRpt);

   RETVOID;
}


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

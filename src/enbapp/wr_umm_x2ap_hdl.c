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
               

     File:     wr_umm_x2ap_hdl.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=77;


static int RLOG_MODULE_ID=32;

#include "wr.h"        /* Common Includes */
#include "wr_emm.h"
#include "wr_umm.h"
#include "wr_umm_trans.h"

/* ********************************************************************** */
/*                                                                        */
/* Transaction ID determination from the message is needed only for       */
/* successful response or a failure. If it is an initiating PDU, a new    */
/* transaction should be allocated for such procedures                    */
/*                                                                        */
/* This function returns ROK to suggest successful processing. It returns */
/* RFAILED to indicate failure.                                           */
/*                                                                        */
/* transId is populated with transaction identifier if a match is found.  */
/* If the PDU includes an initiating procedure, it returns 0 to suggest   */
/* that a new transaction needs to be allocated                           */
/*                                                                        */
/* ********************************************************************** */
PUBLIC S16 wrUmmX2GetTransIdFromMsg
(
WrUeCb                       *ueCb,
WrUmmIncMsg                  *msg,
U32                          *transId
)
{
   CztX2AP_PDU               *pdu = &msg->u.x2Pdu->pdu;
   S16                       msgType = CZT_MSG_UNKNOWN;

   msgType = wrUtlX2GetMsgIdx (pdu);
   if(CZT_MSG_HO_REQ == msgType)
   {
      *transId = WR_CREATE_NEW_TRANS;
   }
   else
   {
      *transId = ueCb->hoTransId;
   }
   RETVALUE(ROK);
}

/* ********************************************************************** */
/*                                                                        */
/* This function is responsible for identifying transaction type based    */
/* on the received message. All transactions start with initiating        */
/* messages.                                                              */
/*                                                                        */
/* ********************************************************************** */
PUBLIC S16 wrUmmGetX2TransType
(
WrUmmIncMsg                  *msg,
U32                          *transTyp
)
{
   CztX2AP_PDU               *pdu = &msg->u.x2Pdu->pdu;
   /* S16                       msgType = CZT_MSG_UNKNOWN; */
   S16                        msgType = CZT_MSG_UNKNOWN; 
   msgType = wrUtlX2GetMsgIdx(pdu);
   /*ccpu00127802*/
   RLOG1(L_DEBUG, "=====Message Type=====[%d]",msgType);
   switch(msgType)
   {
      case CZT_MSG_HO_REQ:
      case CZT_MSG_HO_CANCEL:
      case CZT_MSG_SN_STATUS_TRFR:
      {
         *transTyp = WR_UMM_X2_TGT_HO_TRANS;
         RETVALUE(ROK);
      }
      case CZT_MSG_HO_ACK:
      case CZT_MSG_HO_FAIL:
      case CZT_MSG_UE_CXT_RELEASE:
      {
         *transTyp = WR_UMM_X2_SRC_HO_TRANS;
         RETVALUE(ROK);
      }
      default:
      {
        *transTyp = CZT_MSG_UNKNOWN;
         RETVALUE(ROK);
      }
   }
   RETVALUE(RFAILED);
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

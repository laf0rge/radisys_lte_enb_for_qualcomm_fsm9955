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
               

     File:     wr_ifm_x2ap.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/


static const char* RLOG_MODULE_NAME="IFM";
static int RLOG_MODULE_ID=4;
static int RLOG_FILE_ID=113;

/* header include files (.hx) */
#include "wr.h"
#include "wr_emm.h"
#include "wr_umm.h"
#include "czt.x"
#include "czt_asn.x"

#include "wr_ifm_x2ap.h"
#include "wr_lwr.h"
#include "wr_emm_neighenb.h"

EXTERN S16 wrUmmPrcX2BmpInd(CztEvnt   *cztInd);
EXTERN S16 wrUmmPrcX2BmpCfm(CztEvnt   *cztInd);
EXTERN S16 wrUmmPrcX2ErrInd(CztLclErrInd   *cztLclErrInd);


/* Following matrices define the mapping between the primitives called by the
 * X2AP user (as RsLiCztXXXReq) and the corresponding primitives of the 
 * X2AP layer (service provider) (as CzUiCztXXXReq).
 * The mapping is based on pst->selector.
 * Selector          #define     Coupling      Primitive
 * 0 (SEL_LC_NEW)   LCRSLICZT    loose         cmPkCztXXXReq
 * 1                 CZ          tight         CzUiCztXXXReq
 * 2                LWLCRSLICZT  LW loose      cmPkCztXXXReq
 */

PUBLIC CztBndReq WrIfmX2apBndReqMt [] =
{
   cmPkCztBndReq,        /* 0 - loosely coupled */
   cmPkCztBndReq,        /* 1 - light weight loosely coupled */
#ifdef CZ
   CzUiCztBndReq         /* 2 - tightly coupled, stub layer */
#endif /* CZ */
};


PUBLIC CztUbndReq WrIfmX2apUbndReqMt [] =
{
   cmPkCztUbndReq,        /* 0 - loosely coupled */
   cmPkCztUbndReq,        /* 1 - light weight loosely coupled */
#ifdef CZ
   CzUiCztUbndReq         /* 2 - tightly coupled, stub layer */
#endif /* CZ */
};



PUBLIC CztGpReq WrIfmX2apGpReqMt [] =
{
   cmPkCztGpReq,        /* 0 - loosely coupled */
   cmPkCztGpReq,        /* 1 - light weight loosely coupled */
#ifdef CZ
   CzUiCztGpReq         /* 2 - tightly coupled, stub layer */
#endif /* CZ */
};


PUBLIC CztGpRsp WrIfmX2apGpRspMt [] =
{
   cmPkCztGpRsp,        /* 0 - loosely coupled */
   cmPkCztGpRsp,        /* 1 - light weight loosely coupled */
#ifdef CZ
   CzUiCztGpRsp         /* 2 - tightly coupled, stub layer */
#endif /* CZ */
};

PUBLIC CztGpErrReq WrIfmX2apGpErrReqMt [] =
{
   cmPkCztGpErrReq,        /* 0 - loosely coupled */
   cmPkCztGpErrReq,        /* 1 - light weight loosely coupled */
#ifdef CZ
   CzUiCztGpErrReq         /* 2 - tightly coupled, stub layer */
#endif /* CZ */
};


PUBLIC CztGpRetxReq WrIfmX2apGpRetxReqMt [] =
{
   cmPkCztGpRetxReq,        /* 0 - loosely coupled */
   cmPkCztGpRetxReq,        /* 1 - light weight loosely coupled */
#ifdef CZ
   CzUiCztGpRetxReq         /* 2 - tightly coupled, stub layer */
#endif /* CZ */
};


PUBLIC CztRelReq WrIfmX2apRelReqMt [] =
{
   cmPkCztRelReq,        /* 0 - loosely coupled */
   cmPkCztRelReq,        /* 1 - light weight loosely coupled */
#ifdef CZ
   CzUiCztRelReq         /* 2 - tightly coupled, stub layer */
#endif /* CZ */
};




PUBLIC CztBmpReq WrIfmX2apBmpReqMt [] =
{
   cmPkCztBmpReq,        /* 0 - loosely coupled */
   cmPkCztBmpReq,        /* 1 - light weight loosely coupled */
#ifdef CZ
   CzUiCztBmpReq         /* 2 - tightly coupled, stub layer */
#endif /* CZ */
};


PUBLIC CztBmpRsp WrIfmX2apBmpRspMt [] =
{
   cmPkCztBmpRsp,        /* 0 - loosely coupled */
   cmPkCztBmpRsp,        /* 1 - light weight loosely coupled */
#ifdef CZ
   CzUiCztBmpRsp         /* 2 - tightly coupled, stub layer */
#endif /* CZ */
};



PUBLIC CztDynamPeerAssocRsp WrIfmX2apDynamPeerAssocRspMt [] =
{
   cmPkCztDynamPeerAssocRsp,   /* 0 - loosely coupled */
   cmPkCztDynamPeerAssocRsp,   /* 1 - light weight loosely coupled */
#ifdef CZ
   CzUiCztDynamPeerAssocRsp         /* 2 - tightly coupled, stub layer */
#endif /* CZ */
};

PUBLIC CztAudReq WrIfmX2apAudReqMt [] =
{
   cmPkCztAudReq,        /* 0 - loosely coupled */
   cmPkCztAudReq,        /* 1 - light weight loosely coupled */
#ifdef CZ
   CzUiCztAudReq         /* 2 - tightly coupled, stub layer */
#endif /* CZ */
};

/************************************************************************
                       CZT Interface Functions
************************************************************************/
/** @brief This function is responsible for resolving the CztBndReq primitive
 *
 *  @details
 *
 *  Function:WrIfmX2apBndReq
 *
 *  Processing steps:
 *     - based on selector present in Post structure call appropriate function pointer.
 *
 *   @param[in]  pst: Pointer to the Post Structure
 *   @param[in]  suId: Service user Identifier
 *   @param[in]  spId: Service provider Identifier
 *   @return S16
 *     SUCCESS : ROK
 **/
PUBLIC S16 WrIfmX2apBndReq
(
Pst *pst,
SuId suId,
SpId spId
)
{
   TRC3(WrIfmX2apBndReq)

   (*WrIfmX2apBndReqMt[pst->selector])
      (pst, suId, spId);

   RETVALUE(ROK);
} /* WrIfmX2apBndReq */

/** @brief This function is responsible for resolving the CztUbndReq primitive
 *
 *  @details
 *
 *  Function:WrIfmX2apUbndReq
 *
 *  Processing steps:
 *     - based on selector present in Post structure call appropriate function pointer.
 *
 *   @param[in]  pst: Pointer to the Post Structure
 *   @param[in]  spId: Service provider Identifier
 *   @param[in]  reason: Reason
 *   @return S16
 *     SUCCESS : ROK
 **/
PUBLIC S16 WrIfmX2apUbndReq
(
Pst *pst,
SpId spId,
Reason reason
)
{
   TRC3(WrIfmX2apUbndReq)

   (*WrIfmX2apUbndReqMt[pst->selector])
      (pst, spId, reason);

   RETVALUE(ROK);
} /* WrIfmX2apUbndReq */

/** @brief This function is responsible for resolving the CztGpRetxReq primitive
 *
 *  @details
 *
 *  Function:WrIfmX2apGpRetxReq
 *
 *  Processing steps:
 *     - based on selector present in Post structure call appropriate function pointer.
 *
 *   @param[in]  pst: Pointer to the Post Structure
 *   @param[in]  spId: Service provider Identifier
 *   @param[in]  cztReTxInfo: Pointer to the CztReTxInfo structure
 *   @return S16
 *     SUCCESS : ROK
 **/

PUBLIC S16 WrIfmX2apGpRetxReq
(
CztReTxInfo *cztReTxInfo
)
{
   Pst                       *pst = &wrCb.cztSap[0]->pst;
   TRC3(WrIfmX2apGpRetxReq)

   (*WrIfmX2apGpRetxReqMt[pst->selector])
      (pst, wrCb.cztSap[0]->spId, cztReTxInfo);

   RETVALUE(ROK);
} /* WrIfmX2apGpRetxReq */

/** @brief This function is responsible for resolving the CztGpReq primitive
 *
 *  @details
 *
 *  Function:WrIfmX2apGpReq
 *
 *  Processing steps:
 *     - based on selector present in Post structure call appropriate function pointer.
 *
 *   @param[in]  pst: Pointer to the Post Structure  
 *   @param[in]  spId: Service provider Identifier
 *   @param[in]  cztEvnt: Pointer to the CztEvnt structure
 *   @return S16
 *     SUCCESS : ROK
 **/

PUBLIC S16 WrIfmX2apGpReq
(
CztEvnt                 *cztEvnt
)
{
   Pst                       *pst = &wrCb.cztSap[0]->pst;
   TRC3(WrIfmX2apGpReq)

   (*WrIfmX2apGpReqMt[pst->selector])
                     (pst, wrCb.cztSap[0]->spId, cztEvnt);

   RETVALUE(ROK);
} /* WrIfmX2apGpReq */

/** @brief This function is responsible for resolving the CztGpRsp primitive
 *
 *  @details
 *
 *  Function:WrIfmX2apGpRsp
 *
 *  Processing steps:
 *     - based on selector present in Post structure call appropriate function pointer.
 *
 *   @param[in]  pst: Pointer to the Post Structure
 *   @param[in]  spId: Service provider Identifier
 *   @param[in]  cztEvnt: Pointer to the CztEvnt structure
 *   @return S16
 *     SUCCESS : ROK
 **/
PUBLIC S16 WrIfmX2apGpRsp
(
CztEvnt *cztEvnt
)
{
   Pst                       *pst = &wrCb.cztSap[0]->pst;
   TRC3(WrIfmX2apGpRsp)

   (*WrIfmX2apGpRspMt[pst->selector])
      (pst, wrCb.cztSap[0]->spId, cztEvnt);

   RETVALUE(ROK);
} /* WrIfmX2apGpRsp */

/** @brief This function is responsible for resolving the CztGpErrReq primitive
 *
 *  @details
 *
 *  Function:WrIfmX2apGpErrReq
 *
 *  Processing steps:
 *     - based on selector present in Post structure call appropriate function pointer.
 *
 *   @param[in]  cztEvnt: Pointer to the CztEvnt structure
 *   @return S16
 *     SUCCESS : ROK
 **/
PUBLIC S16 WrIfmX2apGpErrReq
(
CztEvnt *cztEvnt
)
{
   Pst                       *pst = &wrCb.cztSap[0]->pst;

   TRC3(WrIfmX2apGpErrReq)

   (*WrIfmX2apGpErrReqMt[pst->selector])
      (pst, wrCb.cztSap[0]->spId, cztEvnt);

   RETVALUE(ROK);
} /* WrIfmX2apGpReq */

/** @brief This function is responsible for resolving the CztRlsReq primitive
 *
 *  @details
 *
 *  Function:WrIfmX2apRelReq
 *
 *  Processing steps:
 *     - based on selector present in Post structure call appropriate function pointer.
 *
 *   @param[in]  pst: Pointer to the Post Structure
 *   @param[in]  spId: Service provider Identifier
 *   @param[in]  relReq: Pointer to the CztRlsReq structure
 *   @return S16
 *     SUCCESS : ROK
 **/

PUBLIC S16 WrIfmX2apRelReq
(
CztRlsReq *relReq
)
{
   Pst                       *pst = &wrCb.cztSap[0]->pst;

   TRC3(WrIfmX2apRelReq)

   (*WrIfmX2apRelReqMt[pst->selector])
      (pst, wrCb.cztSap[0]->spId, relReq);

   RETVALUE(ROK);
} /* WrIfmX2apRelReq */

/** @brief This function is responsible for resolving the CztBmpReq primitive
 *
 *  @details
 *
 *  Function:WrIfmX2apBmpReq
 *
 *  Processing steps:
 *     - based on selector present in Post structure call appropriate function pointer.
 *
 *   @param[in]  pst: Pointer to the Post Structure
 *   @param[in]  spId: Service provider Identifier
 *   @param[in]  cztEvnt: Pointer to the CztEvnt structure
 *   @return S16
 *     SUCCESS : ROK
 **/
PUBLIC S16 WrIfmX2apBmpReq
(
CztEvnt *cztEvnt
)
{
   Pst                       *pst = &wrCb.cztSap[0]->pst;

   (*WrIfmX2apBmpReqMt[pst->selector])(pst, wrCb.cztSap[0]->spId, cztEvnt);

   RETVALUE(ROK);
} /* WrIfmX2apBmpReq */
/** @brief This function is responsible for resolving the CztBmpRsp primitive
 *
 *  @details
 *
 *  Function:WrIfmX2apBmpRsp
 *
 *  Processing steps:
 *     - based on selector present in Post structure call appropriate function pointer.
 *
 *   @param[in]  pst: Pointer to the Post Structure
 *   @param[in]  spId: Service provider Identifier
 *   @param[in]  cztEvnt: Pointer to the CztEvnt structure
 *   @return S16
 *     SUCCESS : ROK
 **/

PUBLIC S16 WrIfmX2apBmpRsp
(
CztEvnt *cztEvnt
)
{
   Pst                     *pst = &wrCb.cztSap[0]->pst;

   (*WrIfmX2apBmpRspMt[pst->selector])(pst, wrCb.cztSap[0]->spId,cztEvnt);

   RETVALUE(ROK);
} /* WrIfmX2apBmpRsp */
/** @brief This function is responsible for resolving the CztPeerAssocRsp primitive
 *
 *  @details
 *
 *  Function:WrIfmX2apPeerAssocRsp
 *
 *  Processing steps:
 *     - based on selector present in Post structure call appropriate function pointer.
 *
 *   @param[in]  pst: Pointer to the Post Structure
 *   @param[in]  spId: Service provider Identifier
 *   @param[in]  peerAssocRsp: Pointer to the CztPeerAssocRsp structure
 *   @return S16
 *     SUCCESS : ROK
 **/
PUBLIC S16 WrIfmX2apDynamPeerAssocRsp
(
Pst *pst,
SpId spId,
CztPeerAssocRsp *peerAssocRsp
)
{
   TRC3(WrIfmX2apPeerAssocRsp)

   (*WrIfmX2apDynamPeerAssocRspMt[pst->selector])
      (pst, spId, peerAssocRsp);

   RETVALUE(ROK);
} /* WrIfmX2apDynamPeerAssocRsp */

/** @brief This function is responsible for resolving the CztAudReq primitive
 *
 *  @details
 *
 *  Function:WrIfmX2apAudReq
 *
 *  Processing steps:
 *     - based on selector present in Post structure call appropriate function pointer.
 *
 *   @param[in]  pst: Pointer to the Post Structure
 *   @param[in]  spId: Service provider Identifier
 *   @param[in]  audEvnt: Pointer to the CztAudEvnt          structure
 *   @return S16
 *     SUCCESS : ROK
 **/

PUBLIC S16 WrIfmX2apAudReq
(
Pst *pst,
SpId spId,
CztAudEvnt          *audEvnt
)
{
   TRC3(WrIfmX2apAudReq)

   (*WrIfmX2apAudReqMt[pst->selector]) (pst, spId, audEvnt);

   RETVALUE(ROK);
} /* WrIfmX2apAudReq */


/************************************************************************
                    Indications/Confirms from X2AP
************************************************************************/

/** @brief This function is responsible for resolving the CztBndCfm primitive
 *
 *  @details
 *
 *  Function:RsLiCztBndCfm
 *
 *  Processing steps:
 *     - Process the bind confirm message.
 *
 *   @param[in]  pst: Pointer to the Post Structure
 *   @param[in]  spId: Service provider Identifier
 *   @param[in]  status: Status
 *   @return S16
 *     SUCCESS : ROK
 **/

PUBLIC S16 RsLiCztBndCfm
(
Pst   *pst,
SpId  spId,
U8    status
)
{
   TRC3(RsLiCztBndCfm);

   WrIfmX2apBndCfm (pst, spId, status);

   RETVALUE(ROK);
} /* RsLiCztBndCfm */

/** @brief This function is responsible for resolving the CztLclErrInd primitive
 *
 *  @details
 *
 *  Function:RsLiCztLclErrorInd
 *
 *  Processing steps:
 *     - Process the Error indication message.
 *
 *   @param[in]  pst: Pointer to the Post Structure
 *   @param[in]  spId: Service provider Identifier
 *   @param[in]  cztLclErrInd: Pointer to the CztLclErrInd structure
 *   @return S16
 *     SUCCESS : ROK
 **/
PUBLIC S16 RsLiCztLclErrorInd
(
Pst   *pst,
SpId  spId,
CztLclErrInd      *cztLclErrInd
)
{
   TRC3(RsLiCztLclErrorInd);

   WrIfmX2apLclErrorInd(pst, spId, cztLclErrInd);

   RETVALUE(ROK);
} /* RsLiCztLclErrorInd  */


/**************************************************************
   RS interface handler functions
***************************************************************/
/** @brief This function is responsible for handling for Bind Confirmation
 *
 *  @details
 *
 *  Function:WrIfmX2apBndCfm
 *
 *  Processing steps:
 *     - Check the status
       - Send the alarm.
 *
 *   @param[in]  pst: Pointer to the Post Structure
 *   @param[in]  suId: Service user Identifier
 *   @param[in]  status: Status
 *   @return S16
 *     SUCCESS : ROK
 **/

PUBLIC S16 WrIfmX2apBndCfm
(
Pst            *pst,
SuId           suId,
U8             status
)
{
   TRC2(WrIfmX2apBndCfm);
 
   RLOG0(L_DEBUG, " CZT Bind Confirm ");

   /* Send alarm */
   if (CM_BND_OK == status)
   {
      wrStopTmr((PTR)wrCb.cztSap[0],WR_TMR_CZT_SAP_BND);
      RLOG0(L_INFO, "[ALARM] Sending CZT_SAP_BND_OK to OAM");
      wrSendLmAlarm(LCM_CATEGORY_INTERFACE,\
            LCM_EVENT_BND_OK, LWR_CAUSE_CZT_SAP_BOUND);
   }
   else
   {
      RLOG0(L_FATAL, "[ALARM] Sending NLU_SAP_BND_FAIL to OAM");
      wrSendLmAlarm(LCM_CATEGORY_INTERFACE,\
            LCM_EVENT_BND_FAIL, LCM_CAUSE_UNKNOWN);
   }
   RETVALUE(ROK);
} /* End of WrIfmX2apBndCfm */


/*
*
*       Fun:   WrIfmX2apGpInd
*
*       Desc:  Call handler for Release Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  wr_ifm_x2ap.c
*       
*/
PUBLIC S16 WrIfmX2apGpInd
(
Pst            *pst,
SuId           suId,
CztEvnt      *cztInd
)
{  U32 ret = ROK;
   TRC2(WrIfmX2apGpInd);

   RLOG0(L_DEBUG, " CZT General Procedure Indication ");

   ret = wrEmmNghEnbPrcRcvdPdu(cztInd->peerId,cztInd);
   if(ret == RFAILED)
   {
      RLOG0(L_ERROR, " wrEmmPrcRcvdPdu Failed ");
   }   
  cmFreeMem((Ptr *)cztInd);
  RETVALUE(ROK);
} /* End of WrIfmX2apGpInd */


/*
*
*       Fun:   WrIfmX2apGpCfm
*
*       Desc:  Call handler for Release Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  wr_ifm_x2ap.c
*       
*/
PUBLIC S16 WrIfmX2apGpCfm
(
Pst          *pst,
SuId         suId,
CztEvnt      *cztCfm
)
{

   TRC2(WrIfmX2apGpCfm);

   RLOG0(L_DEBUG, " CZT General Procedure Indication ");

   if(ROK != wrEmmNghEnbPrcRcvdPdu(cztCfm->peerId,cztCfm))
   {
      RLOG0(L_ERROR, " wrEmmPrcRcvdPdu Failed ");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* End of WrIfmX2apGpCfm */


/*
*
*       Fun:   WrIfmX2apBmpInd
*
*       Desc:  Call handler for Release Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  wr_ifm_x2ap.c
*       
*/
PUBLIC S16 WrIfmX2apBmpInd
(
Pst            *pst,
SuId           suId,
CztEvnt        *cztInd
)
{

   TRC2(WrIfmX2apBmpInd);

   wrUmmPrcX2BmpInd(cztInd);
   
   RETVALUE(ROK);
} /* End of WrIfmX2apBmpInd */


/*
*
*       Fun:   WrIfmX2apBmpCfm
*
*       Desc:  Call handler for Connection Confirmation
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  wr_ifm_x2ap.c
*       
*/
PUBLIC S16 WrIfmX2apBmpCfm
(
Pst          *pst,
SuId         suId,
CztEvnt      *cztCfm
)
{

   TRC2(WrIfmX2apBmpCfm);

   wrUmmPrcX2BmpCfm(cztCfm);
   
   RETVALUE(ROK);
} /* End of WrIfmX2apBmpCfm */

/** @brief This function is responsible for handling for Connection based data indication
 *
 *  @details
 *
 *  Function:WrIfmX2apGpErrInd
 *
 *  Processing steps:
 *     - Free the message.
 *
 *   @param[in]  pst: Pointer to the Post Structure
 *   @param[in]  suId: Service user Identifier
 *   @param[in]  cztEvnt: Pointer to the CztEvnt structure.
 *   @return S16
 *     SUCCESS : ROK
 **/

PUBLIC S16 WrIfmX2apGpErrInd
(
Pst      *pst,
SuId     suId,
CztEvnt  *cztEvnt
)
{
   U32 ret = ROK;
   TRC2(WrIfmX2apGpErrInd);

   RLOG0(L_ERROR, " CZT Gp Error Indication ");
   ret = wrEmmNghEnbPrcRcvdPdu(cztEvnt->peerId, cztEvnt);
   if(ret == RFAILED)
   {
      RLOG0(L_ERROR, " wrEmmPrcRcvdPdu Failed ");
   }
   cmFreeMem((Ptr *)cztEvnt);
   RETVALUE(ROK);
} /* End of WrIfmX2apGpErrInd */
/** @brief This function is responsible for handling for Error  indication
 *
 *  @details
 *
 *  Function:WrIfmX2apLclErrorInd
 *
 *  Processing steps:
 *     - Check the status
       - Send the alarm.
 *
 *   @param[in]  pst: Pointer to the Post Structure
 *   @param[in]  suId: Service user Identifier
 *   @param[in]  cztLclErrInd: Pointer to the CztLclErrInd structure.
 *   @return S16
 *     SUCCESS : ROK
 **/
PUBLIC S16 WrIfmX2apLclErrorInd
(
Pst            *post,
SuId           suId,
CztLclErrInd   *cztLclErrInd
)
{
   TRC2(WrIfmX2apLclErrorInd);
  
   RLOG0(L_ERROR, " CZT Gp Local Error Indication ");

   RLOG3(L_DEBUG, " Received X2AP error indication for peerId [%ld] msgType [%d] causeType [%d]",
         cztLclErrInd->peerId, cztLclErrInd->msgType, cztLclErrInd->causeType);

   RLOG2(L_DEBUG, " Received X2AP error indication for causeVal [%d] oldUeX2apId [%d] ",
         cztLclErrInd->causeValue, cztLclErrInd->u.ueInfo.oldUEX2APId);

   /* sri - HOFIX */
   if(cztLclErrInd->msgType < 6)
   {
      wrUmmPrcX2ErrInd(cztLclErrInd);
      RETVALUE(ROK);
   }
   else
   {
      wrEmmNghHdlLocalErrInd(cztLclErrInd);
      /* fix for ccpu00138051  */
   WR_FREE(cztLclErrInd,sizeof(CztLclErrInd));
   }
   RETVALUE(ROK);
} /* End of WrIfmX2apLclErrorInd */

/** @brief This function is responsible for handling for Connection Confirmation
 *
 *  @details
 *
 *  Function:WrIfmX2apRelCfm
 *
 *  Processing steps:
 *     - Free the message.
 *
 *   @param[in]  pst: Pointer to the Post Structure
 *   @param[in]  suId: Service user Identifier
 *   @param[in]  rlsCfm: Pointer to the CztRlsCfm structure.
 *   @return S16
 *     SUCCESS : ROK
 **/
PUBLIC S16 WrIfmX2apRelCfm
(
Pst            *pst,
SuId           suId,
CztRlsCfm      *rlsCfm
)
{
   TRC2(WrIfmX2apRelCfm);

   /* Free the memory */
   SPutSBuf(pst->region, pst->pool,(Data *)rlsCfm, sizeof(CztRlsCfm));

   RETVALUE(ROK);
} /* End of WrIfmX2apRelCfm */


/*
*
*       Fun:   WrIfmX2apStaInd
*
*       Desc:  Call handler for Status Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  wr_ifm_x2ap.c
*       
*/
PUBLIC S16 WrIfmX2apStaInd
(
Pst            *post,
SuId           suId,
CztStaInd      *cztSta
)
{
   S16       ret;

   TRC2(WrIfmX2apStaInd);
 
  switch(cztSta->status)
   {
     case CZT_STATUS_ASSOC_DOWN:
     case CZT_STATUS_SETUPREQ_TW_EXPIRY:
     case CZT_STATUS_CFGUPD_TW_EXPIRY:
      {
         ret = wrEmmNghEnbPrcStaInd(cztSta);   
         if(ROK != ret)
         {
            RLOG0(L_ERROR, " Failure in procesisng STATUS IND ");
            RETVALUE(RFAILED);   
         }
         break;
      }
      /* ccpu00130220 */
     case CZT_STATUS_UE_TRELOCPREP_EXPIRY:
     case CZT_STATUS_UE_TX2RELOCOVERALL_EXPIRY:
      {
         ret = wrUmmX2TmrPrcStaInd(cztSta);
         if(ROK != ret)
         {
            RLOG0(L_ERROR, " Failure in procesisng X2 Timers STATUS IND ");
            RETVALUE(RFAILED);            
         }
         break;
         /* ccpu00130220 */
      }
      default:
      {
        RLOG1(L_ERROR, "WrIfmX2apStaInd: Unknown STATUS IND : [%d] is recevied",
              cztSta->status);
        RETVALUE(RFAILED);
        break;
      }
   }/* End of switch() */
   RETVALUE(ROK);
} /* End of WrIfmX2apStaInd */



/*
*
*       Fun:   WrIfmX2apDynamPeerAssocInd
*
*       Desc:  Call handler for Peer Assoc Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  wr_ifm_x2ap.c
*       
*/
PUBLIC S16 WrIfmX2apDynamPeerAssocInd
(
Pst              *pst,
SuId             suId,
CztPeerAssocInd  *peerAssocInd
)
{
   CztPeerAssocRsp *peerAssocRsp = NULLP;
   Pst              rspPst;
   SpId             spId;

   TRC2(WrIfmX2apDynamPeerAssocInd);

   
#if 1 

   if(peerAssocInd != NULLP)
   {
      /* Allocate memory for Peer Association Response */
      WR_ALLOC(&peerAssocRsp, sizeof(CztPeerAssocRsp));
      if(NULLP == peerAssocRsp)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }

      /* get the peer ID, set the eNB ID as zero */
      WR_GET_X2AP_PEER_ID(peerAssocRsp->peerId, 0);
        
      cmMemcpy( (U8 *)&peerAssocRsp->dstAddrLst, (U8 *)&peerAssocInd->dstAddrLst, sizeof(CztNetAddrLst));
      peerAssocRsp->dstPort = peerAssocInd->dstPort;

      /* TOD: remove the stram id hard coding */
      peerAssocRsp->globalStreamId = 0;
      peerAssocRsp->locOutStrms = 5;
      /* Dynamic peer is accepted irrespective of the IP address as of now */
      peerAssocRsp->status.status  = CZT_PRIM_OK;

      cmMemset((U8 *)&rspPst, 0, sizeof(Pst) );
      rspPst.event = EVTCZTPEERASSOCRSP;
      rspPst.srcProcId = SFndProcId();
      rspPst.dstProcId = SFndProcId();
      rspPst.srcEnt = ENTWR;
      rspPst.dstEnt = ENTCZ;
#ifndef TENB_ACC
      rspPst.selector = 2 /* WRSM_WRCZSEL*/;
#else
      /* This is valid in case of acc compilation */
      /* KW P1 warning fix */
      rspPst.selector = 1 /* WRSM_WRCZSEL*/;
#endif
      spId = wrCb.cztSap[0]->spId;
      /* Build and send the peer Assoc Rsp admitting the peer */
      WrIfmX2apDynamPeerAssocRsp(&rspPst, spId, peerAssocRsp);
      /* Free the peerAssociation Indication */
      WR_FREE(peerAssocInd,sizeof(CztPeerAssocInd));
   }
   else
   {
      RLOG0(L_ERROR, " AssocInd Received NULLP ");
   } /* end of else part */
#endif
   RETVALUE(ROK);
} /* End of WrIfmX2apPeerAssocInd */


/*
*
*       Fun:   WrIfmX2apAudCfm
*
*       Desc:  Call handler for Audit confirm
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  wr_ifm_x2ap.c
*       
*/
PUBLIC S16 WrIfmX2apAudCfm
(
Pst              *pst,
SuId             suId,
CztAudEvnt  *audEvnt
)
{
   TRC2(WrIfmX2apAudCfm);

   WR_FREE(audEvnt,sizeof(CztAudEvnt));
   RETVALUE(ROK);
} /* End of WrIfmX2apAudCfm */


/**************************************************************
   RS interface wrapper functions
***************************************************************/
/** @brief This function is responsible for to receive the Non UE Associated messages from X2.
 *
 *  @details
 *
 *  Function:RsLiCztGpInd
 *
 *  Processing steps:
 *     - Process Indication message.
 *
 *   @param[in]  pst: Pointer to the Post Structure
 *   @param[in]  suId: Service user Identifier
 *   @param[in]  cztInd: Pointer to the CztEvnt structure.
 *   @return S16
 *     SUCCESS : ROK
 **/

PUBLIC S16 RsLiCztGpInd
(
Pst     *pst,
SuId    suId,
CztEvnt *cztInd
)
{
   TRC2(RsLiCztGpInd);
   RETVALUE(WrIfmX2apGpInd(pst, suId, cztInd));
} /* end of RsLiCztGpInd */
/** @brief This function is responsible for to receive comfirmation of the Non UE Associated messages from X2.
 *
 *  @details
 *
 *  Function:RsLiCztGpCfm
 *
 *  Processing steps:
 *     - Process Indication message.
 *
 *   @param[in]  pst: Pointer to the Post Structure
 *   @param[in]  suId: Service user Identifier
 *   @param[in]  cztCfm: Pointer to the CztEvnt structure.
 *   @return S16
 *     SUCCESS : ROK
 **/

PUBLIC S16 RsLiCztGpCfm
(
Pst     *pst,
SuId    suId,
CztEvnt *cztCfm
)
{
   TRC2(RsLiCztGpCfm);
   
    RETVALUE(WrIfmX2apGpCfm(pst, suId, cztCfm));
} /* end of RsLiCztGpCfm */

/** @brief This function is responsible for to receive comfirmation of the UE Associated messages from X2.
 *
 *  @details
 *
 *  Function:RsLiCztBmpInd
 *
 *  Processing steps:
 *     - Process Indication message.
 *
 *   @param[in]  pst: Pointer to the Post Structure
 *   @param[in]  suId: Service user Identifier
 *   @param[in]  cztInd: Pointer to the CztEvnt structure.
 *   @return S16
 *     SUCCESS : ROK
 **/

PUBLIC S16 RsLiCztBmpInd
(
Pst     *pst,
SuId    suId,
CztEvnt *cztInd
)
{
   TRC2(RsLiCztBmpInd);
   RETVALUE(WrIfmX2apBmpInd(pst, suId, cztInd));
} /* end of RsLiCztBmpInd */
/** @brief This function is responsible for to receive comfirmation of the UE Associated messages from X2.
 *
 *  @details
 *
 *  Function:RsLiCztBmpCfm
 *
 *  Processing steps:
 *     - Process confirm message.
 *
 *   @param[in]  pst: Pointer to the Post Structure
 *   @param[in]  suId: Service user Identifier
 *   @param[in]  cztCfm: Pointer to the CztEvnt structure.
 *   @return S16
 *     SUCCESS : ROK
 **/

PUBLIC S16 RsLiCztBmpCfm
(
Pst     *pst,
SuId    suId,
CztEvnt *cztCfm
)
{
   TRC2(RsLiCztBmpCfm);
   RETVALUE(WrIfmX2apBmpCfm(pst, suId, cztCfm));
} /* end of RsLiCztBmpCfm */

/** @brief This function is responsible for to receive comfirmation of the UE Associated messages from X2.
 *
 *  @details
 *
 *  Function:RsLiCztGpErrInd
 *
 *  Processing steps:
 *     - Process Error indication message.
 *
 *   @param[in]  pst: Pointer to the Post Structure
 *   @param[in]  suId: Service user Identifier
 *   @param[in]  cztEvnt: Pointer to the CztEvnt structure.
 *   @return S16
 *     SUCCESS : ROK
 **/
PUBLIC S16 RsLiCztGpErrInd
(
Pst     *pst,
SuId    suId,
CztEvnt *cztEvnt
)
{
   TRC2(RsLiCztGpErrInd);
   RETVALUE(WrIfmX2apGpErrInd(pst, suId, cztEvnt));
} /* end of RsLiCztGpErrInd */

/** @brief This function is responsible for to receive dynamic peer Associated information from X2.
 *
 *  @details
 *
 *  Function:RsLiCztDynamPeerAssocInd
 *
 *  Processing steps:
 *     - Process Error indication message.
 *
 *   @param[in]  pst: Pointer to the Post Structure
 *   @param[in]  suId: Service user Identifier
 *   @param[in]  peerAssocInd: Pointer to the CztPeerAssocInd structure.
 *   @return S16
 *     SUCCESS : ROK
 **/

PUBLIC S16 RsLiCztDynamPeerAssocInd
(
Pst     *pst,
SuId    suId,
CztPeerAssocInd *peerAssocInd
)
{
   TRC2(RsLiCztDynamPeerAssocInd);
   RETVALUE(WrIfmX2apDynamPeerAssocInd(pst, suId, peerAssocInd));
} /* end of RsLiCztDynamPeerAssocInd */

/** @brief This function is responsible for resolving the status indication primitive
 *
 *  @details
 *
 *  Function:RsLiCztStatusInd
 *
 *  Processing steps:
 *     - Process Error indication message.
 *
 *   @param[in]  pst: Pointer to the Post Structure
 *   @param[in]  spId: Service Provider Identifier
 *   @param[in]  cztSta: Pointer to the CztStaInd structure.
 *   @return S16
 *     SUCCESS : ROK
 **/

PUBLIC S16 RsLiCztStatusInd
(
Pst   *pst,
SpId  spId,
CztStaInd * cztSta
)
{
   S16   retVal = ROK;

   TRC3(RsLiCztStaInd);

   retVal = WrIfmX2apStaInd(pst, spId, cztSta);
   
   /* Freeing the Memory of sta indication */
   WR_FREE(cztSta,sizeof(CztStaInd));
   RETVALUE(retVal);

} /* RsLiCztStaInd */

/** @brief This function is responsible for to receive UE Release confirmation from X2.
 *
 *  @details
 *
 *  Function:RsLiCztRelCfm
 *
 *  Processing steps:
 *     - Process release confirm message.
 *
 *   @param[in]  pst: Pointer to the Post Structure
 *   @param[in]  suId: Service user Identifier
 *   @param[in]  rlsCfm: Pointer to the CztRlsCfm structure.
 *   @return S16
 *     SUCCESS : ROK
 **/

PUBLIC S16 RsLiCztRelCfm
(
Pst     *pst,
SuId    suId,
CztRlsCfm *rlsCfm
)
{
   TRC2(RsLiCztRelCfm);
   RETVALUE(WrIfmX2apRelCfm(pst, suId, rlsCfm));
} /* end of RsLiCztRelCfm */
/** @brief This function is responsible for to receive Audit confirmation from X2.
 *
 *  @details
 *
 *  Function:RsLiCztAudCfm
 *
 *  Processing steps:
 *     - Process release confirm message.
 *
 *   @param[in]  pst: Pointer to the Post Structure
 *   @param[in]  suId: Service user Identifier
 *   @param[in]  audEvnt: Pointer to the CztAudEvnt structure.
 *   @return S16
 *     SUCCESS : ROK
 **/

PUBLIC S16 RsLiCztAudCfm
(
Pst     *pst,
SuId    suId,
CztAudEvnt *audEvnt
)
{
   TRC2(RsLiCztAudCfm);
   RETVALUE(WrIfmX2apAudCfm(pst, suId, audEvnt));
} /* end of RsLiCztAudCfm */




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

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
               

     File:     wr_ifm_rrc.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/


/* header include files (.h) */
static const char* RLOG_MODULE_NAME="IFM";

static int RLOG_FILE_ID=24;

static int RLOG_MODULE_ID=4;

#include "wr.h"
#include "wr_ifm_rrc.h"
#include "wr_lwr.h"

#include "wr_emm.h"
#include "wr_umm.h"

#include "wr_smm_smallcell.h"

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */

/* Bind Request Primitive */
PRIVATE NhuBndReq WrIfmRrcBndReqMt [] =
{
   cmPkNhuBndReq,            /* 0 - loosely coupled */
#ifdef NH
   NhUiNhuBndReq,            /* 2 - tightly coupled,stub layer */
#endif /* NH */
   cmPkNhuBndReq             /* 1 - light weight loosely coupled */
};

/* Unbind Request Primitive */
PRIVATE NhuUbndReq WrIfmRrcUbndReqMt [] =
{
   cmPkNhuUbndReq,            /* 0 - loosely coupled */
#ifdef NH
   NhUiNhuUbndReq,            /* 2 - tightly coupled,stub layer */
#endif /* NH */
   cmPkNhuUbndReq             /* 1 - light weight loosely coupled */
};

/* Data Request Primitive */
PRIVATE NhuDatReq WrIfmRrcDatReqMt [] =
{
   cmPkNhuDatReq,            /* 0 - loosely coupled */
#ifdef NH
   NhUiNhuDatReq,            /* 2 - tightly coupled,stub layer */
#endif /* NH */
   cmPkNhuDatReq             /* 1 - light weight loosely coupled */
};

/* Data Response Primitive */ 
PRIVATE NhuDatRsp WrIfmRrcDatRspMt [] = 
{ 
   cmPkNhuDatRsp,            /* 0 - loosely coupled */
#ifdef NH
   NhUiNhuDatRsp,            /* 2 - tightly coupled,stub layer */
#endif /* NH */
   cmPkNhuDatRsp             /* 1 - light weight loosely coupled */
};

/* Delete Transaction Request Primitive */
PRIVATE NhuDeleteTransReq WrIfmRrcDeleteTransReqMt[] =
{
   cmPkNhuDeleteTransReq,      /* 0 - loosely coupled */
#ifdef NH
   NhUiNhuDeleteTransReq,      /* 2 - tightly coupled,stub layer */
#endif /* NH */
   cmPkNhuDeleteTransReq       /* 1 - light weight loosely coupled */
};

/* Configuration Request Primitive */
PRIVATE NhuCfgReq WrIfmRrcCfgReqMt [] =
{
   cmPkNhuCfgReq,            /* 0 - loosely coupled */
#ifdef NH
   NhUiNhuCfgReq,            /* 2 - tightly coupled,stub layer */
#endif /* NH */
   cmPkNhuCfgReq             /* 1 - light weight loosely coupled */
};

/* Cancel UE Request primitive */
PRIVATE NhuCnclUeReq WrIfmRrcCnclUeReqMt [] =
{ 
   cmPkNhuCnclUeReq,            /* 0 - loosely coupled */
#ifdef NH
   NhUiNhuCnclUeReq,            /* 2 - tightly coupled,stub layer */
#endif /* NH */
   cmPkNhuCnclUeReq             /* 1 - loosely coupled */
};

/* Cell Config Request primitive */
PRIVATE NhuCellCfgReq WrIfmRrcCellCfgReqMt [] =
{ 
   cmPkNhuCellCfgReq,            /* 0 - loosely coupled */
#ifdef NH
   NhUiNhuCellCfgReq,            /* 2 - tightly coupled,stub layer */
#endif /* NH */
   cmPkNhuCellCfgReq             /* 1 - loosely coupled */
};

/* Decode Request Primitive */
PRIVATE NhuDecodeReq WrIfmRrcDecodeReqMt [] =
{
   cmPkNhuDecodeReq,            /* 0 - loosely coupled */
#ifdef NH
   NhUiNhuDecodeReq,            /* 2 - tightly coupled,stub layer */
#endif /* NH */
   cmPkNhuDecodeReq             /* 1 - light weight loosely coupled */
};

/* Count Request Primitive */
PRIVATE NhuCountReq WrIfmRrcCountReqMt [] =
{
   cmPkNhuCountReq,            /* 0 - loosely coupled */
#ifdef NH
   NhUiNhuCountReq,            /* 2 - tightly coupled,stub layer */
#endif /* NH */
   cmPkNhuCountReq             /* 1 - light weight loosely coupled */
};

/* Enocde Request Primitive */
PRIVATE NhuEncReq WrIfmRrcEncodeReqMt [] =
{
   cmPkNhuEncodeReq,            /* 0 - loosely coupled */
#ifdef NH
   NhUiNhuEncodeReq,            /* 2 - tightly coupled,stub layer */
#endif /* NH */
   cmPkNhuEncodeReq             /* 1 - light weight loosely coupled */
};

/* SDU Status Request Primitive */
PRIVATE NhuSduStaReq WrIfmRrcPdcpSduStaReqMt [] =
{
   cmPkNhuPdcpSduStaReq,            /* 0 - loosely coupled */
#ifdef NH
   NhUiNhuPdcpSduStaReq,            /* 2 - tightly coupled,stub layer */
#endif /* NH */
   cmPkNhuPdcpSduStaReq             /* 1 - light weight loosely coupled */
};

/* Data Forward Request Primitive */
PRIVATE NhuDataFwdReq WrIfmRrcStartPdcpDataFwdReqMt [] =
{
   cmPkNhuStartPdcpDataFwdReq,            /* 0 - loosely coupled */
#ifdef NH
   NhUiNhuStartPdcpDataFwdReq,            /* 2 - tightly coupled,stub layer */
#endif /* NH */
   cmPkNhuStartPdcpDataFwdReq             /* 1 - light weight loosely coupled */
};


/* Data Resume Request Primitive */
PRIVATE NhuDatResumeReq WrIfmRrcPdcpDatResumeReqMt [] =
{
   cmPkNhuPdcpDatResumeReq,            /* 0 - loosely coupled */
#ifdef NH
   NhUiNhuPdcpDatResumeReq,            /* 2 - tightly coupled,stub layer */
#endif /* NH */
   cmPkNhuPdcpDatResumeReq             /* 1 - light weight loosely coupled */
};
EXTERN S16 wrUmmPrcPdcpDatResumeCfm(NhuPdcpDatResumeCfm *pdcpDatResumeCfm);
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* LI primtives for NHU  */

/** @brief This function is responsible for indicating the receipt
 *  of a bind request.
 * 
 *   @details
 * 
 *   Function:WrIfmRrcBndReq
 * 
 *  Processing steps:
 *   - based on selector present in Post structure return appropriate function
 *    pointer.
 *    
 *   @param[in]  pst: Pointer to the Post Structure
 *   @param[in]  suId: Service user Identifier
 *   @param[in]  spId: Service provider Identifier
 *   @return S16
 *     SUCCESS : ROK
 **/
   
PUBLIC S16 WrIfmRrcBndReq
(
Pst                          *pst,
SuId                         suId,
SpId                         spId
)
{
   RETVALUE((*WrIfmRrcBndReqMt[pst->selector])(pst, suId, spId));
}
 /** @brief This function is responsible for indicating the receipt of a Unbind
  *  request.
  * 
  *    @details
  * 
  *  Function:WrIfmRrcUbndReq
  * 
  *  Processing steps:
  *     - based on selector present in Post structure return appropriate
  *     function pointer.
  *
  *   @param[in]  pst: Pointer to the Post Structure
  *   @param[in]  spId: Service provider Identifier
  *   @param[in]  reason: Reason
  *   @return S16
  *     SUCCESS : ROK
  **/

PUBLIC S16 WrIfmRrcUbndReq
(
Pst                          *pst,
SpId                         spId,
Reason                       reason
)
{
   RETVALUE((*WrIfmRrcUbndReqMt[pst->selector])(pst, spId, reason));
}
/** @brief This function is responsible for indicating the receipt
 * of a Data request.
 *
 *  @details
 *
 *  Function:WrIfmRrcDatReq
 *
 *  Processing steps:
 *     - based on selector present in Post structure return appropriate function
 *     pointer.
 *
 *   @param[in]  nhuDatReqSdus: Pointer to the NhuDatReqSdus structure
 *   @return S16
 *     SUCCESS : ROK
 **/

PRIVATE S16 WrIfmRrcDatReq
(
NhuDatReqSdus                *nhuDatReqSdus
)
{
   Pst                       *pst = &wrCb.nhuSap[0]->pst;
   SpId                      spId = wrCb.nhuSap[0]->spId;

   RETVALUE((*WrIfmRrcDatReqMt[pst->selector])(pst, spId, nhuDatReqSdus));
}

/** @brief This function is responsible for indicating the receipt
 * of a Data response.
 *
 *  @details
 *
 *  Function:WrIfmRrcDatRsp
 *
 *  Processing steps:
 *     - based on selector present in Post structure return appropriate function
 *     pointer.
 *
 *   @param[in]  nhuDatRspSdus: Pointer to the NhuDatRspSdus structure
 *   @return S16
 *     SUCCESS : ROK
 **/

PRIVATE S16 WrIfmRrcDatRsp
(
NhuDatRspSdus                *nhuDatRspSdus
)
{
   Pst                       *pst = &wrCb.nhuSap[0]->pst;
   SpId                      spId = wrCb.nhuSap[0]->spId;

   RETVALUE((*WrIfmRrcDatRspMt[pst->selector])(pst, spId, nhuDatRspSdus));
}
/*
 *  @brief This function is responsible for indicating the receipt of a 
 *  Configuration Request.
 *
 *  @details
 *
 *  Function:WrIfmRrcCfgReq
 *
 *  Processing steps:
 *     - based on selector present in Post structure return appropriate function
 *     pointer.
 *
 *   @param[in]  nhuCfgReqSdus: Pointer to the NhuCfgReqSdus structure
 *   @return S16
 *     SUCCESS : ROK
 **/

PUBLIC S16 WrIfmRrcCfgReq
(
NhuCfgReqSdus                *nhuCfgReqSdus
)
{
   Pst                       *pst = &wrCb.nhuSap[0]->pst;
   SpId                      spId = wrCb.nhuSap[0]->spId;

   RETVALUE((*WrIfmRrcCfgReqMt[pst->selector])(pst, spId, nhuCfgReqSdus));
}
/** @brief This function is responsible for indicating the receipt
 * of a Cancel UE Request.
 *
 *  @details
 *
 *  Function:WrIfmRrcCnclUeReq
 *
 *  Processing steps:
 *     - based on selector present in Post structure return appropriate function
 *     pointer.
 *
 *   @param[in]  cellId: cell Identifier
 *   @param[in]  crnti: Crnti
 *   @return S16
 *     SUCCESS : ROK
 **/

PRIVATE S16 WrIfmRrcCnclUeReq
(
NhuCellId                    cellId,
NhuCrnti                     crnti
)
{
   Pst                       *pst = &wrCb.nhuSap[0]->pst;
   SpId                      spId = wrCb.nhuSap[0]->spId;

   RETVALUE((*WrIfmRrcCnclUeReqMt[pst->selector])(pst, spId, cellId, crnti));
}
/** @brief This function is responsible for indicating the receipt
 * of a Cell Config Request.
 *
 *  @details
 *
 *  Function:WrIfmRrcCellCfgReq
 *
 *  Processing steps:
 *     - based on selector present in Post structure return appropriate function
 *     pointer.
 *
 *   @param[in]  nhuCellCfgReqSdus: Pointer to the NhuCellCfgReqSdus structure.
 *   @return S16
 *     SUCCESS : ROK
 **/

PRIVATE S16 WrIfmRrcCellCfgReq
(
NhuCellCfgReqSdus            *nhuCellCfgReqSdus
)
{
   Pst                       *pst = &wrCb.nhuSap[0]->pst;
   SpId                      spId = wrCb.nhuSap[0]->spId;

   RETVALUE((*WrIfmRrcCellCfgReqMt[pst->selector])(pst, spId, 
                                                       nhuCellCfgReqSdus));
}
/** @brief This function is responsible for indicating the receipt
 * of a Delete Trans Request from RRC User to RRC.
 *
 *  @details
 *
 *  Function:WrIfmRrcDeleteTransReq
 *
 *  Processing steps:
 *     - based on selector present in Post structure return appropriate function
 *     pointer.
 *
 *   @param[in]  cellId: cell Identifier.
 *   @param[in]  ueId: Ue identifier.
 *   @param[in]  transId: transaction identifier.
 *   @return S16
 *     SUCCESS : ROK
 **/

PUBLIC S16 WrIfmRrcDeleteTransReq
(
NhuCellId                    cellId,
NhuCrnti                     ueId,
U32                          transId
)
{
   Pst                       *pst = &wrCb.nhuSap[0]->pst;
   SpId                      spId = wrCb.nhuSap[0]->spId;

   RETVALUE((*WrIfmRrcDeleteTransReqMt[pst->selector])(pst, spId, cellId, 
                                                             ueId, transId)); 
}
/** @brief This function is responsible for indicating the receipt
 * of a PDCP Sdu Status request.
 *
 *  @details
 *
 *  Function:WrIfmRrcPdcpSduStaReq
 *
 *  Processing steps:
 *     - based on selector present in Post structure return appropriate function
 *     pointer.
 *
 *   @param[in]  nhuHdr: Nhu Header.
 *   @return S16
 *     SUCCESS : ROK
 **/

PUBLIC S16 WrIfmRrcPdcpSduStaReq
(
NhuHdr                       nhuHdr
)
{
   Pst                       *pst = &wrCb.nhuSap[0]->pst;
   SpId                      spId = wrCb.nhuSap[0]->spId;

   RETVALUE((*WrIfmRrcPdcpSduStaReqMt[pst->selector])(pst, spId, nhuHdr)); 
}

/** @brief This function is responsible for indicating the receipt
 * of a PDCP Data Forward Request
 *
 *  @details
 *
 *  Function:WrIfmRrcStartPdcpDataFwdReq
 *
 *  Processing steps:
 *     - based on selector present in Post structure return appropriate function
 *     pointer.
 *
 *   @param[in]  nhuHdr: Nhu Header.
 *   @return S16
 *     SUCCESS : ROK
 **/

PUBLIC S16 WrIfmRrcStartPdcpDataFwdReq
(
NhuHdr                       nhuHdr
)
{
   Pst                       *pst = &wrCb.nhuSap[0]->pst;
   SpId                      spId = wrCb.nhuSap[0]->spId;

   RETVALUE((*WrIfmRrcStartPdcpDataFwdReqMt[pst->selector])(pst, spId, nhuHdr)); 
}


/** @brief This function is responsible for indicating the receipt
 * of a PDCP Data Resume Request
 *
 *  @details
 *
 *  Function:WrIfmRrcPdcpDatResumeReq
 *
 *  Processing steps:
 *     - based on selector present in Post structure return appropriate function
 *     pointer.
 *
 *   @param[in]  nhuHdr: Nhu Header.
 *   @return S16
 *     SUCCESS : ROK
 **/

PUBLIC S16 WrIfmRrcPdcpDatResumeReq
(
NhuHdr                       nhuHdr
)
{
   Pst                       *pst = &wrCb.nhuSap[0]->pst;
   SpId                      spId = wrCb.nhuSap[0]->spId;

   RETVALUE((*WrIfmRrcPdcpDatResumeReqMt[pst->selector])(pst, spId, nhuHdr));
}

/** @brief This function is responsible for indicating the receipt
 * of a RRC Decode Request.
 *
 *  @details
 *
 *  Function:WrIfmRrcDecodeReq
 *
 *  Processing steps:
 *     - based on selector present in Post structure return appropriate function
 *     pointer.
 *
 *   @param[in]  nhuDecodeSduReq: Pointer to the NhuDecodeSduReq structure.
 *   @return S16
 *     SUCCESS : ROK
 **/
PRIVATE S16 WrIfmRrcDecodeReq
(
NhuDecodeSduReq              *nhuDecodeSduReq
)
{
   Pst                       *pst = &wrCb.nhuSap[0]->pst;
   SpId                      spId = wrCb.nhuSap[0]->spId;

   RETVALUE((*WrIfmRrcDecodeReqMt[pst->selector])(pst, spId, nhuDecodeSduReq));
}
/** @brief This function is responsible for indicating the receipt
 * of a RRC Count Request.
 *
 *  @details
 *
 *  Function:WrIfmRrcCountReq
 *
 *  Processing steps:
 *     - based on selector present in Post structure return appropriate function
 *     pointer.
 *
 *   @param[in]  countHdr: Pointer to the NhuHdr structure.
 *   @return S16
 *     SUCCESS : ROK
 **/

PUBLIC S16 WrIfmRrcCountReq
(
NhuHdr                       *countHdr
)
{
   Pst                       *pst = &wrCb.nhuSap[0]->pst;
   SpId                      spId = wrCb.nhuSap[0]->spId;

   RETVALUE((*WrIfmRrcCountReqMt[pst->selector])(pst, spId, countHdr));
}
/** @brief This function is responsible for indicating the receipt
 * of a RRC Encode Request.
 *
 *  @details
 *
 *  Function:WrIfmRrcEncodeReq
 *
 *  Processing steps:
 *     - based on selector present in Post structure return appropriate function
 *     pointer.
 *
 *   @param[in]  nhuEncReqSdus: Pointer to the NhuEncReqSdus structure.
 *   @return S16
 *     SUCCESS : ROK
 **/

PRIVATE S16 WrIfmRrcEncodeReq
(
NhuEncReqSdus *nhuEncReqSdus
)
{
   Pst                       *pst = &wrCb.nhuSap[0]->pst;
   SpId                      spId = wrCb.nhuSap[0]->spId;

   RETVALUE((*WrIfmRrcEncodeReqMt[pst->selector])(pst, spId, nhuEncReqSdus));
}

/**************************************************************
   NX interface handler functions
***************************************************************/
/** @brief This function is responsible for to present Bind Confirm to NHU
 *
 *  @details
 *
 *  Function:WrIfmRrcBndCfm
 *
 *  Processing steps:
 *     - Check the Status and trigger alarm towards Stack manager.
 *
 *   @param[in]  pst: Pointer to the Post Structure
 *   @param[in]  suId: Service User Identifier
 *   @param[in]  status: Status.
 *   @return S16
 *     SUCCESS : ROK
 **/
PRIVATE S16 WrIfmRrcBndCfm
(
Pst                          *pst,
SuId                         suId,
U8                           status
)
{
   RLOG0(L_DEBUG, "NHU Bind Confirm primitive");

   /* Send alarm */
   if (CM_BND_OK == status)
   {
      wrStopTmr((PTR)wrCb.nhuSap[0],WR_TMR_NHU_SAP_BND);
      RLOG0(L_INFO, "[ALARM] Sending NHU_SAP_BND_OK to OAM");
      wrSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_OK, 
                                                  LWR_CAUSE_NHU_SAP_BOUND);
   }
   else
   {
      RLOG0(L_FATAL, "[ALARM] Sending NHU_SAP_BND_FAIL to OAM");
      wrSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,
                                                        LCM_CAUSE_UNKNOWN);
   }

   RETVALUE(ROK);
}
/** @brief This function is responsible for to present Data Indication to NHU
 *
 *  @details
 *
 *  Function:WrIfmRrcDatInd
 *
 *  Processing steps:
 *     - Process the Uu Data indication Sdu message
 *
 *   @param[in]  pst: Pointer to the Post Structure
 *   @param[in]  suId: Service User Identifier
 *   @param[in]  nhuDatIndSdus: pointer to the NhuDatIndSdus structure.
 *   @return S16
 *     SUCCESS : ROK
 **/
PRIVATE S16 WrIfmRrcDatInd
(
Pst                          *pst,          
SuId                         suId,
NhuDatIndSdus                *nhuDatIndSdus           
)
{

   if (wrUmmPrcUuDatInd(nhuDatIndSdus) != ROK)
   {
      RLOG0(L_ERROR, "Dat Indication processing failed");
   }

   /* TODO: Make sure UMM is releasing this memory */

   RETVALUE(ROK);
}

/** @brief This function is responsible for to present Data Confirmation to NHU
 *
 *  @details
 *
 *  Function:WrIfmRrcDatCfm
 *
 *  Processing steps:
 *     - Process the RRC Data confirm Sdu message
 *
 *   @param[in]  nhDatCfmSdus: pointer to the NhuDatCfmSdus structure.
 *   @return S16
 *     SUCCESS : ROK
 **/

PRIVATE S16 WrIfmRrcDatCfm
(
NhuDatCfmSdus                *nhDatCfmSdus           
)
{
   wrUmmPrcRrcDatCfm(nhDatCfmSdus);
   RETVALUE(ROK);
}

/** @brief This function is responsible for to present NHU status Indication
 *
 *  @details
 *
 *  Function:WrIfmRrcStaInd
 *
 *  Processing steps:
 *     - free the status indication Sdu message
 *
 *   @param[in]  nhStaIndSdus: pointer to the NhStaIndSdus structure.
 *   @return S16
 *     SUCCESS : ROK
 **/

PRIVATE S16 WrIfmRrcStaInd
(
NhuStaIndSdus                *nhStaIndSdus           
)
{
   WR_FREE(nhStaIndSdus, sizeof(NhuStaIndSdus));
   RETVALUE(ROK);
}

/** @brief This function is responsible for to present Error indication
 *  information to NHU.
 *
 *  @details
 *
 *  Function:WrIfmRrcErrInd
 *
 *  Processing steps:
 *     - free the Error Indication Sdus structure.
 *
 *   @param[in]  nhErrIndSdus: pointer to the NhuErrIndSdus structure.
 *   @return S16
 *     SUCCESS : ROK
 **/

PRIVATE S16 WrIfmRrcErrInd
(
NhuErrIndSdus                *nhErrIndSdus           
)
{
   RLOG0(L_ERROR, "NHU Error Ind");

#ifdef ALIGN_64BIT
   RLOG3(L_ERROR, "Header(Cell:Ue:TransId): %d:%d:%d",
            nhErrIndSdus->hdr.cellId,
            nhErrIndSdus->hdr.ueId,
            nhErrIndSdus->hdr.transId
            );
#else
   RLOG3(L_ERROR, "Header(Cell:Ue:TransId): %d:%d:%ld",
            nhErrIndSdus->hdr.cellId,
            nhErrIndSdus->hdr.ueId,
            nhErrIndSdus->hdr.transId
            );
#endif
   RLOG2(L_ERROR, "ErrorInfo(Type:Cause): %d:%d",
            nhErrIndSdus->sdu.errType,
            nhErrIndSdus->sdu.errCause
            );

   /* Free the SDU */
   wrUmmPrcRrcErrInd(nhErrIndSdus);
   RETVALUE(ROK);
}

/** @brief This function is responsible for to present configuration confirm
 * information to NHU.
 *
 *  @details
 *
 *  Function:WrIfmRrcCfgCfm
 *
 *  Processing steps:
 *     - Process the config confirm sdus.
 *
 *   @param[in]  nhuCfgCfmSdus: pointer to the NhuCfgCfmSdus structure.
 *   @return S16
 *     SUCCESS : ROK
 **/
PRIVATE S16 WrIfmRrcCfgCfm
(
NhuCfgCfmSdus                *nhuCfgCfmSdus
)
{
   RLOG0(L_DEBUG, "NHU Config. Cfm");

   if((nhuCfgCfmSdus->sdu.isCfgSuccess == FALSE) && (nhuCfgCfmSdus->sdu.cfgError.errPres))
   {
      RLOG_ARG4(L_ERROR, DBG_UEID, nhuCfgCfmSdus->hdr.ueId,
            "RRC Configuration Confirmation Status = Failed, Message ID = %d, Error Type =%d, Cause = %d",
            nhuCfgCfmSdus->hdr.transId, nhuCfgCfmSdus->sdu.cfgError.errType, 
            nhuCfgCfmSdus->sdu.cfgError.errCause, 0);
   }
   else if(nhuCfgCfmSdus->sdu.isCfgSuccess)
   {
      RLOG_ARG1(L_INFO,  DBG_UEID, nhuCfgCfmSdus->hdr.ueId,
            "RRC Configuration Confirmation Status = Success, Message ID = %d", nhuCfgCfmSdus->hdr.transId);
   }

   if (wrUmmPrcRrcCfgCfm(nhuCfgCfmSdus) != ROK)
   {
      RLOG0(L_ERROR, "RRC CRG CFM proc failed");
      RETVALUE(RFAILED);
   }   

   RETVALUE(ROK);
}

/** @brief This function is responsible for to present cell configuration status 
 * confirm information to NHU.
 *
 *  @details
 *
 *  Function:WrIfmRrcCellCfgCfm
 *
 *  Processing steps:
 *     - Check the config status.
 *     - If status fails free the cell config confirm sdus.
 *     - send the alarm.
 *
 *   @param[in]  nhuCellCfgCfmSdus: pointer to the NhuCellCfgCfmSdus structure.
 *   @return S16
 *     SUCCESS : ROK
 **/
PRIVATE S16 WrIfmRrcCellCfgCfm
(
NhuCellCfgCfmSdus             *nhuCellCfgCfmSdus           
)
{
   RLOG0(L_DEBUG, "NHU Cell Cfg Cfm");

   if (nhuCellCfgCfmSdus->sdu.isCfgSuccess == FALSE)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, nhuCellCfgCfmSdus->hdr.cellId, "Cell configuration failed");
      /* Free the SDU */
      WR_FREE(nhuCellCfgCfmSdus, sizeof(NhuCellCfgCfmSdus));
      /* Send alarm */
      RLOG0(L_FATAL, "[ALARM] Sending CELL_CFG_FAILED to OAM");
      wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_CELL_CFG_FAILED, 
                                                       LCM_CAUSE_UNKNOWN);

      RETVALUE(RFAILED);
   }

   /* Check the Transaction Id to check if CellCfm is for Cell Release */
   if(WR_VAL_TWO_CELL_RELEASE != nhuCellCfgCfmSdus->hdr.transId) 
   {      
      if (wrEmmRrcCellCfgCfm(nhuCellCfgCfmSdus->hdr.cellId) != ROK)
      {
         RLOG_ARG0(L_ERROR, DBG_CELLID, nhuCellCfgCfmSdus->hdr.cellId, "wrEmmRrcCellCfgCfm failed");
         RETVALUE(RFAILED);
      }
   }
   else /* Cell Release Case */
   {
      if (wrEmmRrcCellDelCfgCfm(nhuCellCfgCfmSdus->hdr.cellId) != ROK)
      {
         RLOG_ARG0(L_ERROR, DBG_CELLID, nhuCellCfgCfmSdus->hdr.cellId, "wrEmmRrcCellDelCfgCfm failed");
         RETVALUE(RFAILED);
      }
   }
   /* Free the SDU */
   WR_FREE(nhuCellCfgCfmSdus, sizeof(NhuCellCfgCfmSdus));
   RETVALUE(ROK);
}

/** @brief This function is responsible for to inform decode confirmation to
 * RRC user.
 *
 *  @details
 *
 *  Function:WrIfmRrcDecodeCfm
 *
 *  Processing steps:
 *     - Process the RRC Decode confirm message.
 *
 *   @param[in]  nhuDecodeSduCfm: pointer to the NhuDecodeSduCfm structure.
 *   @return S16
 *     SUCCESS : ROK
*/
PRIVATE S16 WrIfmRrcDecodeCfm
(
NhuDecodeSduCfm              *nhuDecodeSduCfm
)
{
   RLOG0(L_DEBUG, "NHU Decode Cfm Received");
   wrUmmPrcRrcDecodeCfm(nhuDecodeSduCfm);

   RETVALUE(ROK);
}

/** @brief This function is responsible for to inform COUNT values of DRBs to
 * RRC user.
 *
 *  @details
 *
 *  Function:WrIfmRrcCountCfm
 *
 *  Processing steps:
 *     - Free the Drb Count Info List message.
 *
 *   @param[in]  nhuDRBCountInfoLst: pointer to the NhuDRBCountInfoLst structure.
 *   @return S16
 *     SUCCESS : ROK
*/
PRIVATE S16 WrIfmRrcCountCfm
(
NhuDRBCountInfoLst           *nhuDRBCountInfoLst
)
{
   /* Free the SDU */
   /* Fix : Memory leak */
   WR_FREE_EVNT(nhuDRBCountInfoLst);
   RETVALUE(ROK);
}

/** @brief This function is responsible for to inform Encode confirmation to RRC
 * user.
 *
 *  @details
 *
 *  Function:WrIfmRrcEncodeCfm
 *
 *  Processing steps:
 *     - Process the RRC encode confirm sdu.
 *
 *   @param[in]  rrcEncCfmSdus: pointer to the NhuEncCfmSdus structure.
 *   @return S16
 *     SUCCESS : ROK
*/
PRIVATE S16 WrIfmRrcEncodeCfm
(
NhuEncCfmSdus                *rrcEncCfmSdus
)
{
   RLOG0(L_DEBUG, "Received RRC Encode Confirm");
   wrUmmPrcRrcEncodeCfm(rrcEncCfmSdus);

   RETVALUE(ROK);
}

/** @brief This function is responsible for to inform PDCP SDU Status
 * confirmation to RRC user.
 *
 *  @details
 *
 *  Function:WrIfmRrcPdcpSduStaCfm
 *
 *  Processing steps:
 *     - Free the Pdcp Sdu Status Confirm.
 *
 *   @param[in]  nhuPdcpSduStaCfm: pointer to the NhuPdcpSduStaCfm structure.
 *   @return S16
 *     SUCCESS : ROK
*/
PRIVATE S16 WrIfmRrcPdcpSduStaCfm
(
NhuPdcpSduStaCfm             *nhuPdcpSduStaCfm
)
{
   TRC2(WrIfmRrcPdcpSduStaCfm);

   wrUmmPrcPdcpSduStaCfm(nhuPdcpSduStaCfm);
   RETVALUE(ROK);
}

/** @brief This function is responsible for to inform PDCP Data Resume
 * confirmation to RRC user.
 *
 *  @details
 *
 *  Function:WrIfmRrcPdcpDatResumeCfm
 *
 *  Processing steps:
 *     - Free the Pdcp Data Resume Confirm.
 *
 *   @param[in]  nhuPdcpDatResumeCfm: pointer to the NhuPdcpDatResumeCfm structure.
 *   @return S16
 *     SUCCESS : ROK
*/
PRIVATE S16 WrIfmRrcPdcpDatResumeCfm
(
NhuPdcpDatResumeCfm             *nhuPdcpDatResumeCfm
)
{
   TRC2(WrIfmRrcPdcpDatResumeCfm);

   wrUmmPrcPdcpDatResumeCfm(nhuPdcpDatResumeCfm);
   RETVALUE(ROK);
}

/**************************************************************
   NX interface wrapper functions
***************************************************************/
/** @brief This function is responsible for to present Bind Cfm to NHU.
 *
 *  @details
 *
 *  Function:NxLiNhuBndCfm 
 *
 *  Processing steps:
 *     - Process message.
 *
 *   @param[in]  pst: Pointer to the Post structure.
 *   @param[in]  suId: service user Id.
 *   @param[in]  status: status.
 *   @return S16
 *     SUCCESS : ROK
*/

#ifdef ANSI
PUBLIC S16 NxLiNhuBndCfm
(
Pst     *pst,
SuId    suId,
U8      status
)
#else
PUBLIC S16 NxLiNhuBndCfm(pst,suId,status)
Pst     *pst;  
SuId    suId;
U8      status;
#endif
{
   TRC2(NxLiNhuBndCfm);
   RETVALUE(WrIfmRrcBndCfm(pst, suId, status));
} /* end of NxLiNhuBndCfm */

/** @brief This function is responsible for to present Data Indication to NHU.
 *
 *  @details
 *
 *  Function:NxLiNhuDatInd 
 *
 *  Processing steps:
 *     - Process Indication message.
 *
 *   @param[in]  pst: Pointer to the Post structure.
 *   @param[in]  suId: service user Id.
 *   @param[in]  nhuDatIndSdus: Pointer to the NhuDatIndSdus structure.
 *   @return S16
 *     SUCCESS : ROK
*/

PUBLIC S16 NxLiNhuDatInd
(
Pst                          *pst,          
SuId                         suId,
NhuDatIndSdus                *nhuDatIndSdus           
)
{
   RETVALUE(WrIfmRrcDatInd (pst, suId, nhuDatIndSdus));
}

/** @brief This function is responsible for to present Data Confirmation to NHU.
 *
 *  @details
 *
 *  Function:NxLiNhuDatCfm 
 *
 *  Processing steps:
 *     - Process Confirm message.
 *
 *   @param[in]  pst: Pointer to the Post structure.
 *   @param[in]  suId: service user Id.
 *   @param[in]  nhDatCfmSdus: Pointer to the NhuDatCfmSdus structure.
 *   @return S16
 *     SUCCESS : ROK
*/

#ifdef ANSI
PUBLIC S16 NxLiNhuDatCfm
(
Pst     *pst,          
SuId    suId,
NhuDatCfmSdus *nhDatCfmSdus           
)
#else
PUBLIC S16 NxLiNhuDatCfm (pst,suId,nhDatCfmSdus)
Pst     *pst;          
SuId     suId;
NhuDatCfmSdus *nhDatCfmSdus;
#endif
{
   TRC2(NxLiNhuDatCfm);
   RETVALUE(WrIfmRrcDatCfm(nhDatCfmSdus));
}

/** @brief This function is responsible for to present status indication to NHU.
 *
 *  @details
 *
 *  Function:NxLiNhuStaInd 
 *
 *  Processing steps:
 *     - Process Status indication message.
 *
 *   @param[in]  pst: Pointer to the Post structure.
 *   @param[in]  suId: service user Id.
 *   @param[in]  nhStaIndSdus: Pointer to the NhuStaIndSdus structure.
 *   @return S16
 *     SUCCESS : ROK
*/

PUBLIC S16 NxLiNhuStaInd
(
Pst                          *pst,          
SuId                         suId,
NhuStaIndSdus                *nhStaIndSdus           
)
{
   RETVALUE(WrIfmRrcStaInd(nhStaIndSdus));
}

/** @brief This function is responsible for to present configuration confirm
 * information to NHU.
 *
 *  @details
 *
 *  Function:NxLiNhuErrInd 
 *
 *  Processing steps:
 *     - Process Error indication message.
 *
 *   @param[in]  pst: Pointer to the Post structure.
 *   @param[in]  suId: service user Id.
 *   @param[in]  nhErrIndSdus: Pointer to the NhuErrIndSdus structure.
 *   @return S16
 *     SUCCESS : ROK
*/
PUBLIC S16 NxLiNhuErrInd
(
Pst                          *pst,          
SuId                         suId,
NhuErrIndSdus                *nhErrIndSdus           
)
{
   RETVALUE(WrIfmRrcErrInd(nhErrIndSdus));
}

/** @brief This function is responsible for to present configuration confirm
 * information to NHU.
 *
 *  @details
 *
 *  Function:NxLiNhuCfgCfm 
 *
 *  Processing steps:
 *     - Process config confirm message.
 *
 *   @param[in]  pst: Pointer to the Post structure.
 *   @param[in]  suId: service user Id.
 *   @param[in]  nhuCfgCfmSdus: Pointer to the NhuCfgCfmSdus structure.
 *   @return S16
 *     SUCCESS : ROK
*/
PUBLIC S16 NxLiNhuCfgCfm
(
Pst                          *pst,
SuId                         suId,
NhuCfgCfmSdus                *nhuCfgCfmSdus
)
{
   RETVALUE(WrIfmRrcCfgCfm(nhuCfgCfmSdus));
}

/** @brief This function is responsible for to present cell configuration
 * confirm information to NHU.
 *
 *  @details
 *
 *  Function:NxLiNhuCellCfgCfm
 *
 *  Processing steps:
 *     - Process Cell config confirm message.
 *
 *   @param[in]  pst: Pointer to the Post structure.
 *   @param[in]  suId: service user Id.
 *   @param[in]  nhuCellCfgCfmSdus: Pointer to the NhuCellCfgCfmSdus structure.
 *   @return S16
 *     SUCCESS : ROK
*/
PUBLIC S16 NxLiNhuCellCfgCfm
(
Pst                          *pst,          
SuId                         suId,
NhuCellCfgCfmSdus            *nhuCellCfgCfmSdus           
)
{
   RETVALUE(WrIfmRrcCellCfgCfm(nhuCellCfgCfmSdus));
}

/** @brief This function is responsible for to inform decode confirm to RRC
 * User.
 *
 *  @details
 *
 *  Function:NxLiNhuDecodeCfm
 *
 *  Processing steps:
 *     - Process RRC Decode confirm message.
 *
 *   @param[in]  pst: Pointer to the Post structure.
 *   @param[in]  suId: service user Id.
 *   @param[in]  nhuDecodeSduCfm: Pointer to the NhuDecodeSduCfm structure.
 *   @return S16
 *     SUCCESS : ROK
*/
PUBLIC S16 NxLiNhuDecodeCfm
(
Pst                          *pst,
SuId                         suId,
NhuDecodeSduCfm              *nhuDecodeSduCfm
)
{
   RETVALUE(WrIfmRrcDecodeCfm(nhuDecodeSduCfm));
}

/** @brief This function is responsible for to inform COUNT values of DRBs to RRC
 * User.
 *
 *  @details
 *
 *  Function:NxLiNhuCountCfm
 *
 *  Processing steps:
 *     - Process DRB count Info list.
 *
 *   @param[in]  pst: Pointer to the Post structure.
 *   @param[in]  suId: service user Id.
 *   @param[in]  nhuDRBCountInfoLst: Pointer to the NhuDRBCountInfoLst structure.
 *   @return S16
 *     SUCCESS : ROK
*/
PUBLIC S16 NxLiNhuCountCfm
(
Pst                          *pst,
SuId                         suId,
NhuDRBCountInfoLst           *nhuDRBCountInfoLst
)
{
   RETVALUE(WrIfmRrcCountCfm(nhuDRBCountInfoLst));
}

/** @brief This function is responsible for to inform Encode confirmation to RRC
 * User.
 *
 *  @details
 *
 *  Function:NxLiNhuEncodeCfm
 *
 *  Processing steps:
 *     - Process Encode confirm message.
 *
 *   @param[in]  pst: Pointer to the Post structure.
 *   @param[in]  suId: service user Id.
 *   @param[in]  nhuEncCfmSdus: Pointer to the NhuEncCfmSdus structure.
 *   @return S16
 *     SUCCESS : ROK
*/
PUBLIC S16 NxLiNhuEncodeCfm
(
Pst                          *pst,
SuId                         suId,
NhuEncCfmSdus                *nhuEncCfmSdus
)
{
   RETVALUE(WrIfmRrcEncodeCfm(nhuEncCfmSdus));
}

/** @brief This function is responsible for to inform PDCP SDU Status
 * confirmation to RRC User.
 *
 *  @details
 *
 *  Function:NxLiNhuPdcpSduStaCfm
 *
 *  Processing steps:
 *     - Process PDCP Sdu status confirm message.
 *
 *   @param[in]  pst: Pointer to the Post structure.
 *   @param[in]  suId: service user Id.
 *   @param[in]  nhuPdcpSduStaCfm: Pointer to the NhuPdcpSduStaCfm structure.
 *   @return S16
 *     SUCCESS : ROK
*/
PUBLIC S16 NxLiNhuPdcpSduStaCfm
(
Pst                          *pst,
SuId                         suId,
NhuPdcpSduStaCfm             *nhuPdcpSduStaCfm
)
{
   RETVALUE(WrIfmRrcPdcpSduStaCfm(nhuPdcpSduStaCfm));
}


/** @brief This function is responsible for to inform PDCP Data Resume
 * confirmation to RRC User.
 *
 *  @details
 *
 *  Function:NxLiNhuPdcpDatResumeCfm
 *
 *  Processing steps:
 *     - Process PDCP Data Resume confirm message.
 *
 *   @param[in]  pst: Pointer to the Post structure.
 *   @param[in]  suId: service user Id.
 *   @param[in]  nhuPdcpDatResumeCfm: Pointer to the NhuPdcpDatResumeCfm structure.
 *   @return S16
 *     SUCCESS : ROK
*/
PUBLIC S16 NxLiNhuPdcpDatResumeCfm
(
Pst                          *pst,
SuId                         suId,
NhuPdcpDatResumeCfm             *nhuPdcpDatResumeCfm
)
{
   RETVALUE(WrIfmRrcPdcpDatResumeCfm(nhuPdcpDatResumeCfm));
}
/** @brief This function is responsible for to sending of NHU cell config
 * request.
 *
 *  @details
 *
 *  Function:wrSndNhuCellCfgReq
 *
 *  Processing steps:
 *     - Process Cell config message.
 *
 *   @param[in]  nhuCellCfgReq: Pointer to the NhuCellCfgReqSdus structure.
 *   @return S16
 *     SUCCESS : ROK
 */
PUBLIC S16 wrSndNhuCellCfgReq 
(
NhuCellCfgReqSdus            **nhuCellCfgReq
)
{
   RLOG0(L_DEBUG, "Triggering Cell Cfg Req over the NHU Interface");

   if (WrIfmRrcCellCfgReq(*nhuCellCfgReq) != ROK)
   {
      RLOG0(L_ERROR, "WrIfmRrcCellCfgReq failed.");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/** @brief This function is responsible for to sending the DL CCCH message
 *
 *  @details
 *
 *  Function:wrIfmRrcSndCcchRspToUe
 *
 *  Processing steps:
 *     - Process Data response Event Sdu message.
 *
 *   @param[in] nhuDatRspEvntSdu: Pointer to the NhuDatRspSdus structure.
 *   @return S16
 *     SUCCESS : ROK
 */
PUBLIC S16 wrIfmRrcSndCcchRspToUe
(
NhuDatRspSdus                *nhuDatRspEvntSdu
)
{
   if (WrIfmRrcDatRsp(nhuDatRspEvntSdu) != ROK)
   {
      RLOG0(L_ERROR, "WrIfmRrcDatRsp failed.");      
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/** @brief This function is responsible for to sending the RRC send request
 *   to UE.
 *  @details
 *
 *  Function:wrIfmRrcSndReqToUe
 *
 *  Processing steps:
 *     - Process RRC Data request message.
 *
 *   @param[in] nhuDatReqSdu: Pointer to the NhuDatReqSdus structure.
 *   @return S16
 *     SUCCESS : ROK
 */
PUBLIC S16 wrIfmRrcSndReqToUe
(
NhuDatReqSdus                *nhuDatReqSdu
)
{
   if (WrIfmRrcDatReq(nhuDatReqSdu) != ROK)
   {
      RLOG0(L_ERROR, "WrIfmRrcDatReq failed.");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/** @brief This function is responsible for to sending the RRC send request
 *
 *  @details
 *
 *  Function:wrIfmRrcSndMsg
 *
 *  Processing steps:
 *     - Process RRC Data request message.
 *
 *   @param[in] nhuDatReqSdu: Pointer to the NhuDatReqSdus structure.
 *   @return S16
 *     SUCCESS : ROK
 */
PUBLIC S16 wrIfmRrcSndMsg
(
NhuDatReqSdus                *nhuDatReqSdu
)
{
   /* Issue the primitive */
   if (WrIfmRrcDatReq(nhuDatReqSdu) != ROK)
   {
      RLOG0(L_ERROR, "WrIfmRrcDatReq failed.");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/** @brief This function is responsible for to sending the NHU Decode request
 *
 *  @details
 *
 *  Function:wrIfmRrcSndDecodeReq
 *
 *  Processing steps:
 *     - Process NHU decode Message.
 *
 *   @param[in] evt: Pointer to the NhuDecodeSduReq structure.
 *   @return S16
 *     SUCCESS : ROK
 */
PUBLIC S16 wrIfmRrcSndDecodeReq
(
NhuDecodeSduReq              *evt
)
{
   if (WrIfmRrcDecodeReq(evt) != ROK)
   {
      RLOG0(L_ERROR, "WrIfmRrcDecodeReq failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/** @brief This function is responsible for to sending the NHU Encode request
 *
 *  @details
 *
 *  Function:wrIfmRrcSndEncodeReq
 *
 *  Processing steps:
 *     - Process NHU encode Message.
 *
 *   @param[in] evt: Pointer to the NhuEncReqSdus structure.
 *   @return S16
 *     SUCCESS : ROK
 */
PUBLIC S16 wrIfmRrcSndEncodeReq
(
NhuEncReqSdus                *evt
)
{
   /* Issue the primitive */
   if (WrIfmRrcEncodeReq(evt) != ROK)
   {
      RLOG0(L_ERROR, "WrIfmRrcEncodeReq failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/** @brief This function is responsible for to Release Ue.
 *
 *  @details
 *
 *  Function:wrIfmRrcRelUe
 *
 *  Processing steps:
 *     - Process RRC Ue release message.
 *
 *   @param[in] cellId: cell Id.
 *   @param[in] crnti: crnti.
 *   @return S16
 *     SUCCESS : ROK
 */

PUBLIC S16 wrIfmRrcRelUe
(
NhuCellId                    cellId,
NhuCrnti                     crnti
)
{
   if (WrIfmRrcCnclUeReq(cellId, crnti) != ROK)
   {
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to Release Ue.
 *
 *  @details
 *
 *  Function:wrIfmRrcRelUe
 *
 *  Processing steps:
 *     - Process RRC Ue release message.
 *
 *   @param[in] cellId: cell Id.
 *   @param[in] crnti: crnti.
 *   @return S16
 *     SUCCESS : ROK
 */
PUBLIC S16 WrIfmRrcGetPdcpSduSta
(
NhuHdr                       nhuHdr
)
{
   WrIfmRrcPdcpSduStaReq(nhuHdr);
   RETVALUE(ROK);
}

/** @brief This function is responsible for to sending PDCP
 *   Data forwarding Request
 *  @details
 *
 *  Function: WrIfmRrcSendStartPdcpDataFwdReq
 *
 *  Processing steps:
 *     - 
 *
 *   @param[in] NhuHdr: 
 *   @return S16
 *     SUCCESS : ROK
 */
PUBLIC S16 WrIfmRrcSendStartPdcpDataFwdReq
(
NhuHdr                       nhuHdr
)
{
   WrIfmRrcStartPdcpDataFwdReq(nhuHdr);
   RETVALUE(ROK);
}

/** @brief This function is responsible for to sending PDCP
 *   Data Resume Request
 *  @details
 *
 *  Function: WrIfmRrcSendPdcpDatResumeReq
 *
 *  Processing steps:
 *     - 
 *
 *   @param[in] NhuHdr: 
 *   @return S16
 *     SUCCESS : ROK
 */
PUBLIC S16 WrIfmRrcSendPdcpDatResumeReq
(
NhuHdr                       nhuHdr
)
{
   WrIfmRrcPdcpDatResumeReq(nhuHdr);
   RETVALUE(ROK);
}

PUBLIC S16 wrIfmRrcSndCfgReq
(
NhuCfgReqSdus   *nhuCfgReqSdus
)
{
   if(ROK != WrIfmRrcCfgReq(nhuCfgReqSdus))
   {
      RLOG0(L_ERROR, "wrIfmRrcSndCfgReq: WrIfmRrcCfgReq failed.");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* wrIfmRrcSndCfgReq */


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

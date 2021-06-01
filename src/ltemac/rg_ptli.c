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




/************************************************************************
 
     Name:     LTE-MAC layer
  
     Type:     C source file
  
     Desc:     C source code for Entry point fucntions
  
     File:     rg_ptli.c
  
     Sid:      gk_ptli.c@@/main/3 - Sat Jul 30 02:21:32 2011
  
     Prg:      ps
  
**********************************************************************/

/** @file rg_dhm.c
@brief APIs related to Downlink HARQ.
*/

/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hash List Defines */
#include "cm_mblk.h"
#include "cm_lte.h"        /* Common LTE Defines */
#include "tfu.h"           /* RGU Interface defines */

/* header/extern include files (.x) */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_mblk.x"
#include "cm_lte.x"        /* Common LTE Defines */
#include "tfu.x"           /* RGU Interface includes */


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if !(defined(LCRGLITFU)  && defined(TF) && defined(LWLCRGLITFU))
#define PTRGLITFU
#endif


/* MAX Number of Service Providers of RG */
#define RG_MAX_TFU_PROV   3

#ifdef PTRGLITFU
/** @brief This API is used to send a Bind Request from MAC to PHY.
 * @param pst Pointer to the post structure.
 * @param suId SAP ID of the service user.
 * @param spId SAP ID of the service provider.
 * @return ROK/RFAILED
 */
EXTERN S16 PtLiTfuBndReq ARGS((Pst * pst, SuId suId, SpId spId));
/** @brief This API is used to send a Bind Request from Scheduler to PHY.
 * @param pst Pointer to the post structure.
 * @param suId SAP ID of the service user.
 * @param spId SAP ID of the service provider.
 * @return ROK/RFAILED
 */
EXTERN S16 PtLiTfuSchBndReq ARGS((Pst * pst, SuId suId, SpId spId));
/** @brief This API is used to send an Unbind Request from MAC to PHY.
 * @param pst Pointer to the post structure.
 * @param suId SAP ID of the service provider.
 * @param reason Reason for Unbind request.
 * @return ROK/RFAILED
 */
EXTERN S16 PtLiTfuUbndReq ARGS((Pst * pst, SpId spId, Reason reason));
/** @brief This API is used to send an Unbind Request from Scheduler to PHY.
 * @param pst Pointer to the post structure.
 * @param suId SAP ID of the service provider.
 * @param reason Reason for Unbind request.
 * @return ROK/RFAILED
 */
EXTERN S16 PtLiTfuSchUbndReq ARGS((Pst * pst, SpId spId, Reason reason));
/** @brief This primitive is sent from Scheduler to PHY.
 * @details This primitive provides PHY with all the information required by 
 * PHY to decode transmissions from the UE on either PUCCH or PUSCH.
 * -# On PUCCH UE can transmit the following
 *    -# SR
 *    -# HARQ feedback
 *    -# CQI report
 *    -# HARQ + CQI
 *    -# HARQ + SR
 * -# On PUSCH UE can transmit the following
 *    -# Data
 *    -# Data + CQI
 *    -# Data + HARQ Feedback
 * This primitive carries all the information for the expected subframe for all
 * the UEs that have been scheduled to transmit.
 * @param pst Pointer to the post structure.
 * @param spId SAP ID of the service provider.
 * @param recpReq Pointer to the TfuRecpReqInfo structure.
 * @return ROK/RFAILED
 */
EXTERN S16 PtLiTfuRecpReq ARGS((Pst * pst, SpId spId, TfuRecpReqInfo * recpReq));
/** @brief This Primitive is sent from Scheduler to PHY. It provides PHY with
  * all the control information
  * @details This primitive carries the information sent on the following
  * channels - 
  * -# PDCCH
  * -# PHICH
  * -# PCFICH
  * 
  * @param pst
  * @param spId
  * @param cntrlReq pointer to TfuCntrlReqInfo
  * @return ROK/RFAILED
  */
EXTERN S16 PtLiTfuCntrlReq ARGS((Pst * pst, SpId spId, TfuCntrlReqInfo * cntrlReq));
/** @brief This Primitive carries the Data PDUs from MAC to PHY for
  * transmission. 
  * @details The data being sent in this primitive is meant to be transmitted on
  * the downlink channel PDSCH and PBCH (if present). To facilitate physical
  * layer processing, requisite control information is also sent along with the
  * data. 
  * @sa TfUiTfuCntrlReq
  * @param pst 
  * @param spId
  * @param tfuDatReq pointer to TfuDatReqInfo
  * @return 
  */
EXTERN S16 PtLiTfuDatReq ARGS((Pst * pst, SpId spId, TfuDatReqInfo * datReq));
#endif /*--#ifdef PTRGLITFU--*/

/** @brief This API is used to send a Bind Request from MAC to PHY.
 * @param pst Pointer to the post structure.
 * @param suId SAP ID of the service user.
 * @param spId SAP ID of the service provider.
 * @return ROK/RFAILED
 */
PRIVATE CONSTANT TfuBndReq RgLiTfuBndReqMt[RG_MAX_TFU_PROV] =
{
#ifdef LCRGLITFU
   cmPkTfuBndReq,
#else
   PtLiTfuBndReq,
#endif
#ifdef TF
   TfUiTfuBndReq,
#else
   PtLiTfuBndReq,
#endif
#ifdef LWLCRGLITFU
   cmPkTfuBndReq
#else
   PtLiTfuBndReq
#endif
};

/** @brief This API is used to send a Bind Request from Scheduler to PHY.
 * @param pst Pointer to the post structure.
 * @param suId SAP ID of the service user.
 * @param spId SAP ID of the service provider.
 * @return ROK/RFAILED
 */
PRIVATE CONSTANT TfuSchBndReq RgLiTfuSchBndReqMt[RG_MAX_TFU_PROV] =
{
#ifdef LCRGLITFU
   cmPkTfuSchBndReq,
#else
   PtLiTfuSchBndReq,
#endif
#ifdef TF
   TfUiTfuSchBndReq,
#else
   PtLiTfuSchBndReq,
#endif
#ifdef LWLCRGLITFU
   cmPkTfuSchBndReq
#else
   PtLiTfuSchBndReq
#endif
};

/** @brief This API is used to send an Unbind Request from MAC to PHY.
 * @param pst Pointer to the post structure.
 * @param suId SAP ID of the service provider.
 * @param reason Reason for Unbind request.
 * @return ROK/RFAILED
 */
PRIVATE CONSTANT TfuUbndReq RgLiTfuUbndReqMt[RG_MAX_TFU_PROV] =
{
#ifdef LCRGLITFU
   cmPkTfuUbndReq,
#else
   PtLiTfuUbndReq,
#endif
#ifdef TF
   TfUiTfuUbndReq,
#else
   PtLiTfuUbndReq,
#endif
#ifdef LWLCRGLITFU
   cmPkTfuUbndReq
#else
   PtLiTfuUbndReq
#endif
};

/** @brief This API is used to send an Unbind Request from Scheduler to PHY.
 * @param pst Pointer to the post structure.
 * @param suId SAP ID of the service provider.
 * @param reason Reason for Unbind request.
 * @return ROK/RFAILED
 */
PRIVATE CONSTANT TfuSchUbndReq RgLiTfuSchUbndReqMt[RG_MAX_TFU_PROV] =
{
#ifdef LCRGLITFU
   cmPkTfuSchUbndReq,
#else
   PtLiTfuSchUbndReq,
#endif
#ifdef TF
   TfUiTfuSchUbndReq,
#else
   PtLiTfuSchUbndReq,
#endif
#ifdef LWLCRGLITFU
   cmPkTfuSchUbndReq
#else
   PtLiTfuSchUbndReq
#endif
};

/** @brief This primitive is sent from Scheduler to PHY.
 * @details This primitive provides PHY with all the information required by 
 * PHY to decode transmissions from the UE on either PUCCH or PUSCH.
 * -# On PUCCH UE can transmit the following
 *    -# SR
 *    -# HARQ feedback
 *    -# CQI report
 *    -# HARQ + CQI
 *    -# HARQ + SR
 * -# On PUSCH UE can transmit the following
 *    -# Data
 *    -# Data + CQI
 *    -# Data + HARQ Feedback
 * This primitive carries all the information for the expected subframe for all
 * the UEs that have been scheduled to transmit.
 * @param pst Pointer to the post structure.
 * @param spId SAP ID of the service provider.
 * @param recpReq Pointer to the TfuRecpReqInfo structure.
 * @return ROK/RFAILED
 */
PRIVATE CONSTANT TfuRecpReq RgLiTfuRecpReqMt[RG_MAX_TFU_PROV] =
{
#ifdef LCRGLITFU
   cmPkTfuRecpReq,
#else
   PtLiTfuRecpReq,
#endif
#ifdef TF
   TfUiTfuRecpReq,
#else
   PtLiTfuRecpReq,
#endif
#ifdef LWLCRGLITFU
   cmPkTfuRecpReq
#else
   PtLiTfuRecpReq
#endif
};

/** @brief This Primitive is sent from Scheduler to PHY. It provides PHY with
  * all the control information
  * @details This primitive carries the information sent on the following
  * channels - 
  * -# PDCCH
  * -# PHICH
  * -# PCFICH
  * 
  * @param pst
  * @param spId
  * @param cntrlReq pointer to TfuCntrlReqInfo
  * @return ROK/RFAILED
  */
PRIVATE CONSTANT TfuCntrlReq RgLiTfuCntrlReqMt[RG_MAX_TFU_PROV] =
{
#ifdef LCRGLITFU
   cmPkTfuCntrlReq,
#else
   PtLiTfuCntrlReq,
#endif
#ifdef TF
   TfUiTfuCntrlReq,
#else
   PtLiTfuCntrlReq,
#endif
#ifdef LWLCRGLITFU
   cmPkTfuCntrlReq
#else
   PtLiTfuCntrlReq
#endif
};

/** @brief This Primitive carries the Data PDUs from MAC to PHY for
  * transmission. 
  * @details The data being sent in this primitive is meant to be transmitted on
  * the downlink channel PDSCH and PBCH (if present). To facilitate physical
  * layer processing, requisite control information is also sent along with the
  * data. 
  * @sa TfUiTfuCntrlReq
  * @param pst 
  * @param spId
  * @param tfuDatReq pointer to TfuDatReqInfo
  * @return 
  */
PRIVATE CONSTANT TfuDatReq RgLiTfuDatReqMt[RG_MAX_TFU_PROV] =
{
#ifdef LCRGLITFU
   cmPkTfuDatReq,
#else
   PtLiTfuDatReq,
#endif
#ifdef TF
   TfUiTfuDatReq,
#else
   PtLiTfuDatReq,
#endif
#ifdef LWLCRGLITFU
   cmPkTfuDatReq
#else
   PtLiTfuDatReq
#endif
};


#ifdef RG



/***********************************************************
*
*     Func : RgLiTfuBndReq
*
*
*     Desc : This API is used to send a Bind Request from MAC to PHY.
 * @param pst Pointer to the post structure.
 * @param suId SAP ID of the service user.
 * @param spId SAP ID of the service provider.
 * @return ROK/RFAILED
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 RgLiTfuBndReq
(
Pst * pst,
SuId suId,
SpId spId
)
#else
PUBLIC S16 RgLiTfuBndReq(pst, suId, spId)
Pst * pst;
SuId suId;
SpId spId;
#endif
{

   TRC3(RgLiTfuBndReq)

   RETVALUE((*RgLiTfuBndReqMt[pst->selector])(pst, suId, spId));

}



/***********************************************************
*
*     Func : RgLiTfuSchBndReq
*
*
*     Desc : This API is used to send a Bind Request from Scheduler to PHY.
 * @param pst Pointer to the post structure.
 * @param suId SAP ID of the service user.
 * @param spId SAP ID of the service provider.
 * @return ROK/RFAILED
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 RgLiTfuSchBndReq
(
Pst * pst,
SuId suId,
SpId spId
)
#else
PUBLIC S16 RgLiTfuSchBndReq(pst, suId, spId)
Pst * pst;
SuId suId;
SpId spId;
#endif
{

   TRC3(RgLiTfuSchBndReq)

   RETVALUE((*RgLiTfuSchBndReqMt[pst->selector])(pst, suId, spId));

}



/***********************************************************
*
*     Func : RgLiTfuUbndReq
*
*
*     Desc : This API is used to send an Unbind Request from MAC to PHY.
 * @param pst Pointer to the post structure.
 * @param suId SAP ID of the service provider.
 * @param reason Reason for Unbind request.
 * @return ROK/RFAILED
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 RgLiTfuUbndReq
(
Pst * pst,
SpId spId,
Reason reason
)
#else
PUBLIC S16 RgLiTfuUbndReq(pst, spId, reason)
Pst * pst;
SpId spId;
Reason reason;
#endif
{

   TRC3(RgLiTfuUbndReq)

   RETVALUE((*RgLiTfuUbndReqMt[pst->selector])(pst, spId, reason));

}



/***********************************************************
*
*     Func : RgLiTfuSchUbndReq
*
*
*     Desc : This API is used to send an Unbind Request from Scheduler to PHY.
 * @param pst Pointer to the post structure.
 * @param suId SAP ID of the service provider.
 * @param reason Reason for Unbind request.
 * @return ROK/RFAILED
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 RgLiTfuSchUbndReq
(
Pst * pst,
SpId spId,
Reason reason
)
#else
PUBLIC S16 RgLiTfuSchUbndReq(pst, spId, reason)
Pst * pst;
SpId spId;
Reason reason;
#endif
{

   TRC3(RgLiTfuSchUbndReq)

   RETVALUE((*RgLiTfuSchUbndReqMt[pst->selector])(pst, spId, reason));

}



/***********************************************************
*
*     Func : RgLiTfuRecpReq
*
*
*     Desc : This primitive is sent from Scheduler to PHY.
 * @details This primitive provides PHY with all the information required by 
 * PHY to decode transmissions from the UE on either PUCCH or PUSCH.
 * -# On PUCCH UE can transmit the following
 *    -# SR
 *    -# HARQ feedback
 *    -# CQI report
 *    -# HARQ + CQI
 *    -# HARQ + SR
 * -# On PUSCH UE can transmit the following
 *    -# Data
 *    -# Data + CQI
 *    -# Data + HARQ Feedback
 * This primitive carries all the information for the expected subframe for all
 * the UEs that have been scheduled to transmit.
 * @param pst Pointer to the post structure.
 * @param spId SAP ID of the service provider.
 * @param recpReq Pointer to the TfuRecpReqInfo structure.
 * @return ROK/RFAILED
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 RgLiTfuRecpReq
(
Pst * pst,
SpId spId,
TfuRecpReqInfo * recpReq
)
#else
PUBLIC S16 RgLiTfuRecpReq(pst, spId, recpReq)
Pst * pst;
SpId spId;
TfuRecpReqInfo * recpReq;
#endif
{

   TRC3(RgLiTfuRecpReq)

   RETVALUE((*RgLiTfuRecpReqMt[pst->selector])(pst, spId, recpReq));

}



/***********************************************************
*
*     Func : RgLiTfuCntrlReq
*
*
*     Desc : This Primitive is sent from Scheduler to PHY. It provides PHY with
  * all the control information
  * @details This primitive carries the information sent on the following
  * channels - 
  * -# PDCCH
  * -# PHICH
  * -# PCFICH
  * 
  * @param pst
  * @param spId
  * @param cntrlReq pointer to TfuCntrlReqInfo
  * @return ROK/RFAILED
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 RgLiTfuCntrlReq
(
Pst * pst,
SpId spId,
TfuCntrlReqInfo * cntrlReq
)
#else
PUBLIC S16 RgLiTfuCntrlReq(pst, spId, cntrlReq)
Pst * pst;
SpId spId;
TfuCntrlReqInfo * cntrlReq;
#endif
{

   TRC3(RgLiTfuCntrlReq)

   RETVALUE((*RgLiTfuCntrlReqMt[pst->selector])(pst, spId, cntrlReq));

}



/***********************************************************
*
*     Func : RgLiTfuDatReq
*
*
*     Desc : This Primitive carries the Data PDUs from MAC to PHY for
  * transmission. 
  * @details The data being sent in this primitive is meant to be transmitted on
  * the downlink channel PDSCH and PBCH (if present). To facilitate physical
  * layer processing, requisite control information is also sent along with the
  * data. 
  * @sa TfUiTfuCntrlReq
  * @param pst 
  * @param spId
  * @param tfuDatReq pointer to TfuDatReqInfo
  * @return
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 RgLiTfuDatReq
(
Pst * pst,
SpId spId,
TfuDatReqInfo * datReq
)
#else
PUBLIC S16 RgLiTfuDatReq(pst, spId, datReq)
Pst * pst;
SpId spId;
TfuDatReqInfo * datReq;
#endif
{

   TRC3(RgLiTfuDatReq)

   RETVALUE((*RgLiTfuDatReqMt[pst->selector])(pst, spId, datReq));

}

#endif /*--ifdef RG--*/

#ifdef PTRGLITFU



/***********************************************************
*
*     Func : PtLiTfuBndReq
*
*
*     Desc : This API is used to send a Bind Request from MAC to PHY.
 * @param pst Pointer to the post structure.
 * @param suId SAP ID of the service user.
 * @param spId SAP ID of the service provider.
 * @return ROK/RFAILED
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 PtLiTfuBndReq
(
Pst * pst,
SuId suId,
SpId spId
)
#else
PUBLIC S16 PtLiTfuBndReq(pst, suId, spId)
Pst * pst;
SuId suId;
SpId spId;
#endif
{

   TRC3(PtLiTfuBndReq)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(spId);

   RETVALUE(ROK);

}



/***********************************************************
*
*     Func : PtLiTfuSchBndReq
*
*
*     Desc : This API is used to send a Bind Request from Scheduler to PHY.
 * @param pst Pointer to the post structure.
 * @param suId SAP ID of the service user.
 * @param spId SAP ID of the service provider.
 * @return ROK/RFAILED
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 PtLiTfuSchBndReq
(
Pst * pst,
SuId suId,
SpId spId
)
#else
PUBLIC S16 PtLiTfuSchBndReq(pst, suId, spId)
Pst * pst;
SuId suId;
SpId spId;
#endif
{

   TRC3(PtLiTfuSchBndReq)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(spId);

   RETVALUE(ROK);

}



/***********************************************************
*
*     Func : PtLiTfuUbndReq
*
*
*     Desc : This API is used to send an Unbind Request from MAC to PHY.
 * @param pst Pointer to the post structure.
 * @param suId SAP ID of the service provider.
 * @param reason Reason for Unbind request.
 * @return ROK/RFAILED
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 PtLiTfuUbndReq
(
Pst * pst,
SpId spId,
Reason reason
)
#else
PUBLIC S16 PtLiTfuUbndReq(pst, spId, reason)
Pst * pst;
SpId spId;
Reason reason;
#endif
{

   TRC3(PtLiTfuUbndReq)

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(reason);

   RETVALUE(ROK);

}



/***********************************************************
*
*     Func : PtLiTfuSchUbndReq
*
*
*     Desc : This API is used to send an Unbind Request from Scheduler to PHY.
 * @param pst Pointer to the post structure.
 * @param suId SAP ID of the service provider.
 * @param reason Reason for Unbind request.
 * @return ROK/RFAILED
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 PtLiTfuSchUbndReq
(
Pst * pst,
SpId spId,
Reason reason
)
#else
PUBLIC S16 PtLiTfuSchUbndReq(pst, spId, reason)
Pst * pst;
SpId spId;
Reason reason;
#endif
{

   TRC3(PtLiTfuSchUbndReq)

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(reason);

   RETVALUE(ROK);

}



/***********************************************************
*
*     Func : PtLiTfuRecpReq
*
*
*     Desc : This primitive is sent from Scheduler to PHY.
 * @details This primitive provides PHY with all the information required by 
 * PHY to decode transmissions from the UE on either PUCCH or PUSCH.
 * -# On PUCCH UE can transmit the following
 *    -# SR
 *    -# HARQ feedback
 *    -# CQI report
 *    -# HARQ + CQI
 *    -# HARQ + SR
 * -# On PUSCH UE can transmit the following
 *    -# Data
 *    -# Data + CQI
 *    -# Data + HARQ Feedback
 * This primitive carries all the information for the expected subframe for all
 * the UEs that have been scheduled to transmit.
 * @param pst Pointer to the post structure.
 * @param spId SAP ID of the service provider.
 * @param recpReq Pointer to the TfuRecpReqInfo structure.
 * @return ROK/RFAILED
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 PtLiTfuRecpReq
(
Pst * pst,
SpId spId,
TfuRecpReqInfo * recpReq
)
#else
PUBLIC S16 PtLiTfuRecpReq(pst, spId, recpReq)
Pst * pst;
SpId spId;
TfuRecpReqInfo * recpReq;
#endif
{

   TRC3(PtLiTfuRecpReq)

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(recpReq);

   RETVALUE(ROK);

}



/***********************************************************
*
*     Func : PtLiTfuCntrlReq
*
*
*     Desc : This Primitive is sent from Scheduler to PHY. It provides PHY with
  * all the control information
  * @details This primitive carries the information sent on the following
  * channels - 
  * -# PDCCH
  * -# PHICH
  * -# PCFICH
  * 
  * @param pst
  * @param spId
  * @param cntrlReq pointer to TfuCntrlReqInfo
  * @return ROK/RFAILED
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 PtLiTfuCntrlReq
(
Pst * pst,
SpId spId,
TfuCntrlReqInfo * cntrlReq
)
#else
PUBLIC S16 PtLiTfuCntrlReq(pst, spId, cntrlReq)
Pst * pst;
SpId spId;
TfuCntrlReqInfo * cntrlReq;
#endif
{

   TRC3(PtLiTfuCntrlReq)

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(cntrlReq);

   RETVALUE(ROK);

}



/***********************************************************
*
*     Func : PtLiTfuDatReq
*
*
*     Desc : This Primitive carries the Data PDUs from MAC to PHY for
  * transmission. 
  * @details The data being sent in this primitive is meant to be transmitted on
  * the downlink channel PDSCH and PBCH (if present). To facilitate physical
  * layer processing, requisite control information is also sent along with the
  * data. 
  * @sa TfUiTfuCntrlReq
  * @param pst 
  * @param spId
  * @param tfuDatReq pointer to TfuDatReqInfo
  * @return
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 PtLiTfuDatReq
(
Pst * pst,
SpId spId,
TfuDatReqInfo * datReq
)
#else
PUBLIC S16 PtLiTfuDatReq(pst, spId, datReq)
Pst * pst;
SpId spId;
TfuDatReqInfo * datReq;
#endif
{

   TRC3(PtLiTfuDatReq)

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(datReq);

   RETVALUE(ROK);

}


#endif /*--ifdef PTRGLITFU--*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

/**********************************************************************
 
         End of file:     gk_ptli.c@@/main/3 - Sat Jul 30 02:21:32 2011
 
**********************************************************************/
 
/**********************************************************************
 
        Notes:
 
**********************************************************************/
 
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     ps              1. Initial Release.
/main/2      ---     ps              1. LTE MAC 2.1 release
/main/3      ---        gvj  1. Updated for LTE MAC Rel 3.1           
$SID$        ---        rt   1. LTE MAC 4.1 release
*********************************************************************91*/

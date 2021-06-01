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
  
     Desc:     C source code for Upper Interface Module 
  
     File:     rg_ptui.c 
  
     Sid:      gk_ptui.c@@/main/3 - Sat Jul 30 02:21:33 2011
  
     Prg:      sm
  
**********************************************************************/

/** @file rg_ptui.c
@brief This file contains the definitions for Upper Interface(RGR/CRG/RGU) 
       primitives that are invoked from MAC to its service users.
       Portable functions corresponding to these primitives are also defined.
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
#include "cm_lte.h"        /* Common LTE Defines */
#include "rg_env.h"        /* MAC Environment Defines */
#include "crg.h"           /* CRG Interface defines */
#include "rgr.h"           /* RGR Interface defines */
#include "rgu.h"           /* RGU Interface defines */
#include "rgm.h"           /* RGM Interface defines*/

/* header/extern include files (.x) */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_lte.x"        /* Common LTE Defines */
#include "crg.x"           /* CRG Interface includes */
#include "rgr.x"           /* RGR Interface includes */
#include "rgu.x"           /* RGU Interface includes */
#include "rgm.x"           /* RGM Interface includes*/
#ifdef SS_RBUF
#include "ss_rbuf.h"
#include "ss_rbuf.x"
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if !(defined(LCRGUIRGU) && defined(LWLCRGUIRGU)  && defined(KW))
#define PTRGUIRGU
#endif

#if !(defined(LCRGUICRG)  && defined(NH))
#define PTRGUICRG
#endif

#if !(defined(LCRGUIRGR)  && defined(NX))
#define PTRGUIRGR
#endif

#if !(defined(LCRGUIRGM)  && defined(RM))
#define PTRGUIRGM
#endif

/* MAX Number of Service Users of RG */
#define RG_MAX_RGU_USR   3

/* MAX Number of Service Users of RG */
#define RG_MAX_CRG_USR   2

/* MAX Number of Service Users of RG */
#define RG_MAX_RGR_USR   2

/* MAX Number of Service Users of RG */
#define RG_MAX_RGM_USR   2
#ifdef PTRGUIRGU
/** @brief Confirmation from MAC to RLC for the bind/Unbind 
 * request for the interface saps */
EXTERN S16 PtUiRguBndCfm ARGS((Pst* pst, SuId suId, U8 status));
/** @brief Data Indication from MAC to RLC to 
 * forward the data received for common channels*/
EXTERN S16 PtUiRguCDatInd ARGS((Pst* pst, SuId suId, RguCDatIndInfo  * datInd));
/** @brief Data Indication from MAC to RLC to 
 * forward the data received for dedicated channels*/
EXTERN S16 PtUiRguDDatInd ARGS((Pst* pst, SuId suId, RguDDatIndInfo  * datInd));
/** @brief Status Indication from MAC to RLC  
 * as a response to the staRsp primitive from RLC.
 * Informs RLC of the totalBufferSize and Timing Info 
 * for the transmission on common channels. */
EXTERN S16 PtUiRguCStaInd ARGS((Pst* pst, SuId suId, RguCStaIndInfo  * staInd));
/** @brief Status Indication from MAC to RLC  
 * as a response to the staRsp primitive from RLC.
 * Informs RLC of the totalBufferSize and Timing Info 
 * for the transmission on dedicated channels. */
EXTERN S16 PtUiRguDStaInd ARGS((Pst* pst, SuId suId, RguDStaIndInfo  * staInd));
#ifdef LTE_L2_MEAS
/**@brief HARQ Status Indication from MAC to RLC */
EXTERN S16 PtUiRguHqStaInd ARGS((Pst* pst,SuId suId,
         RguHarqStatusInd *harqStatusInd));
#endif
/**@brief PDB FLow Control Indication from MAC to RLC */
EXTERN S16 PtUiRguFlowCntrlInd ARGS((Pst* pst,SuId suId,
         RguFlowCntrlInd *flowCntrlInd));
#endif /*--#ifdef PTRGUIRGU--*/
S16 RgUiRguFlowCntrlInd(Pst* pst, SuId suId, RguFlowCntrlInd *flowCntrlInd);

#if defined(MAC_RLC_UL_RBUF) && !defined(SS_RBUF)
PRIVATE S16  RgUiRguDDatIndRbuf ARGS((RguDDatIndInfo  *datInd));
#endif
EXTERN Void rgFreeSharableSBuf ARGS((Data **data, Size size));
#ifdef MAC_RLC_HARQ_STA_RBUF
PUBLIC S16 RgUiRguHqStaIndRbuf ARGS((Pst* pst,SuId suId,RguHarqStatusInd *harqStatusInd));
#endif 

#ifdef RGR_CQI_REPT 
EXTERN S16 PtUiRgrStaInd ARGS(( Pst* pst, SuId suId, RgrStaIndInfo *staInd));
#endif
/* LTE_ADV_FLAG_REMOVED_START */
EXTERN S16 PtUiRgrLoadInfInd ARGS(( Pst* pst, SuId suId, RgrLoadInfIndInfo *loadInfInd));
/* LTE_ADV_FLAG_REMOVED_END */
EXTERN S16 PtUiRgrUeStaInd ARGS
((
Pst* pst, 
SuId suId, 
RgrUeStaIndInfo *staInd
));
#ifdef PTRGUICRG
/** @brief Confirmation from MAC to RRC for the bind/Unbind 
 * request for the interface saps */
EXTERN S16 PtUiCrgBndCfm ARGS((Pst* pst, SuId suId, U8 status));
/** @brief Configuration Confirm from MAC to RRC */
EXTERN S16 PtUiCrgCfgCfm ARGS((Pst* pst, SuId suId, CrgCfgTransId transId, U8 status));
#endif /*--#ifdef PTRGUICRG--*/

#ifdef PTRGUIRGR
/** @brief Confirmation from MAC to RRM for the bind/Unbind 
 * request for the interface saps */
EXTERN S16 PtUiRgrBndCfm ARGS((Pst* pst, SuId suId, U8 status));
/** @brief Configuration Confirm from MAC to RRM */
EXTERN S16 PtUiRgrCfgCfm ARGS((Pst* pst, SuId suId, RgrCfgTransId transId, U8 status));
EXTERN S16 PtUiRgrTtiInd ARGS((Pst* pst, SuId suId, RgrTtiIndInfo *ttiInd));
/* Added for SI Enhancement*/
#ifdef RGR_SI_SCH
EXTERN S16 PtUiRgrSiCfgCfm ARGS((Pst* pst, SuId suId, RgrCfgTransId transId, U8 status));
EXTERN S16 PtUiRgrWarningSiCfgCfm ARGS((Pst* pst, SuId suId, 
                                RgrCfgTransId transId, U8 siId, U8 status));
#endif/*RGR_SI_SCH*/
#endif /*--#ifdef PTRGUIRGR--*/

#ifdef PTRGUIRGM
PUBLIC S16 PtUiRgmPrbRprtInd ARGS((Pst* pst, SuId suId, RgmPrbRprtInd *prbRprtInd));
PUBLIC S16 PtUiRgmBndCfm ARGS((Pst* pst, SuId suId, U8 status));
PUBLIC S16 PtUiRgmTransModeInd  ARGS((Pst* pst, SuId suId, RgmTransModeInd *transModeInd));
#endif
PUBLIC S16 RgUiRgmSendPrbRprtInd ARGS((Pst* pst, SuId suId, RgmPrbRprtInd *prbRprtInd));
PUBLIC S16 RgUiRgmChangeTransModeInd ARGS((Pst* pst, SuId suId, RgmTransModeInd *transModeInd));

PUBLIC S16 RgUiRguFlowCntrlInd ARGS((Pst* pst, SuId suId, RguFlowCntrlInd *flowCntrlInd));
/* Added for sending TTI tick to RRM */
/** @brief TTI indication from MAC to RRM */
PRIVATE CONSTANT RgrTtiInd RgUiRgrTtiIndMt[RG_MAX_RGR_USR] =
{
#ifdef LCRGUIRGR
   cmPkRgrTtiInd,
#else
   PtUiRgrTtiInd,
#endif
#ifdef NX
   NxLiRgrTtiInd,
#else
   PtUiRgrTtiInd,
#endif
};


/** @brief Confirmation from MAC to RRM for the bind/Unbind 
 * request for the interface saps */
PRIVATE CONSTANT RgrBndCfm RgUiRgrBndCfmMt[RG_MAX_RGR_USR] =
{
#ifdef LCRGUIRGR
   cmPkRgrBndCfm,
#else
   PtUiRgrBndCfm,
#endif
#ifdef NX
   NxLiRgrBndCfm,
#else
   PtUiRgrBndCfm,
#endif
};

/** @brief Confirmation from MAC to RRM for the bind/Unbind 
 * request for the interface saps  via RGM interface*/
PRIVATE CONSTANT RgmBndCfm RgUiRgmBndCfmMt[RG_MAX_RGM_USR] =
{
#ifdef RGM_LC
   cmPkRgmBndCfm,
#else
   PtUiRgmBndCfm,
#endif
#ifdef RM
   RmLiRgmBndCfm, /*To be added by RRM*/
#else
   PtUiRgmBndCfm,
#endif
};


/** @brief Configuration Confirm from MAC to RRM */
PRIVATE CONSTANT RgrCfgCfm RgUiRgrCfgCfmMt[RG_MAX_RGR_USR] =
{
#ifdef LCRGUIRGR
   cmPkRgrCfgCfm,
#else
   PtUiRgrCfgCfm,
#endif
#ifdef NX
   NxLiRgrCfgCfm,
#else
   PtUiRgrCfgCfm,
#endif
};

/* Added for SI Enhancement*/
#ifdef RGR_SI_SCH
/** @brief SI Configuration Confirm from MAC to RRM */
PRIVATE CONSTANT RgrSiCfgCfm RgUiRgrSiCfgCfmMt[RG_MAX_RGR_USR] =
{
#ifdef LCRGUIRGR
   cmPkRgrSiCfgCfm,
#else
   PtUiRgrSiCfgCfm,
#endif
#ifdef NX
   NxLiRgrSiCfgCfm,
#else
   PtUiRgrSiCfgCfm,
#endif
};


/** @brief Warning SI Configuration Confirm from MAC to RRM */
PRIVATE CONSTANT RgrWarningSiCfgCfm RgUiRgrWarningSiCfgCfmMt[RG_MAX_RGR_USR] =
{
#ifdef LCRGUIRGR
   cmPkRgrWarningSiCfgCfm,
#else
   PtUiRgrWarningSiCfgCfm,
#endif
#ifdef NX
   NxLiRgrWarningSiCfgCfm,
#else
   PtUiRgrWarningSiCfgCfm,
#endif
};

#endif/*RGR_SI_SCH */
/** @brief Confirmation from MAC to RRC for the bind/Unbind 
 * request for the interface saps */
PRIVATE CONSTANT CrgBndCfm RgUiCrgBndCfmMt[RG_MAX_CRG_USR] =
{
#ifdef LCRGUICRG
   cmPkCrgBndCfm,
#else
   PtUiCrgBndCfm,
#endif
#ifdef NH
   NhLiCrgBndCfm,
#else
   PtUiCrgBndCfm,
#endif
};

/** @brief Configuration Confirm from MAC to RRC */
PRIVATE CONSTANT CrgCfgCfm RgUiCrgCfgCfmMt[RG_MAX_CRG_USR] =
{
#ifdef LCRGUICRG
   cmPkCrgCfgCfm,
#else
   PtUiCrgCfgCfm,
#endif
#ifdef NH
   NhLiCrgCfgCfm,
#else
   PtUiCrgCfgCfm,
#endif
};


/** @brief Confirmation from MAC to RLC for the bind/Unbind 
 * request for the interface saps */
PRIVATE CONSTANT RguBndCfm RgUiRguBndCfmMt[RG_MAX_RGU_USR] =
{
#ifdef LCRGUIRGU
   cmPkRguBndCfm,
#else
   PtUiRguBndCfm,
#endif
#ifdef KW
   KwLiRguBndCfm,
#else
   PtUiRguBndCfm,
#endif
#ifdef LWLCRGUIRGU
   cmPkRguBndCfm,
#else
   PtUiRguBndCfm,
#endif
};

/** @brief Data Indication from MAC to RLC to 
 * forward the data received for common channels*/
PRIVATE CONSTANT RguCDatInd RgUiRguCDatIndMt[RG_MAX_RGU_USR] =
{
#ifdef LCRGUIRGU
   cmPkRguCDatInd,
#else
   PtUiRguCDatInd,
#endif
#ifdef KW
   KwLiRguCDatInd,
#else
   PtUiRguCDatInd,
#endif
#ifdef LWLCRGUIRGU
   cmPkRguCDatInd,
#else
   PtUiRguCDatInd,
#endif
};

/** @brief Data Indication from MAC to RLC to 
 * forward the data received for dedicated channels*/
PRIVATE CONSTANT RguDDatInd RgUiRguDDatIndMt[RG_MAX_RGU_USR] =
{
#ifdef LCRGUIRGU
   cmPkRguDDatInd,
#else
   PtUiRguDDatInd,
#endif
#ifdef KW
   KwLiRguDDatInd,
#else
   PtUiRguDDatInd,
#endif
#ifdef LWLCRGUIRGU
   cmPkRguDDatInd,
#else
   PtUiRguDDatInd,
#endif
};

/** @brief Status Indication from MAC to RLC  
 * as a response to the staRsp primitive from RLC.
 * Informs RLC of the totalBufferSize and Timing Info 
 * for the transmission on common channels. */
PRIVATE CONSTANT RguCStaInd RgUiRguCStaIndMt[RG_MAX_RGU_USR] =
{
#ifdef LCRGUIRGU
   cmPkRguCStaInd,
#else
   PtUiRguCStaInd,
#endif
#ifdef KW
   KwLiRguCStaInd,
#else
   PtUiRguCStaInd,
#endif
#ifdef LWLCRGUIRGU
   cmPkRguCStaInd,
#else
   PtUiRguCStaInd,
#endif
};

/** @brief Status Indication from MAC to RLC  
 * as a response to the staRsp primitive from RLC.
 * Informs RLC of the totalBufferSize and Timing Info 
 * for the transmission on dedicated channels. */
PRIVATE CONSTANT RguDStaInd RgUiRguDStaIndMt[RG_MAX_RGU_USR] =
{
#ifdef LCRGUIRGU
   cmPkRguDStaInd,
#else
   PtUiRguDStaInd,
#endif
#ifdef KW
   KwLiRguDStaInd,
#else
   PtUiRguDStaInd,
#endif
#ifdef LWLCRGUIRGU
   cmPkRguDStaInd,
#else
   PtUiRguDStaInd,
#endif
};

#ifdef LTE_L2_MEAS
/** @brief HARQ Status Indication from MAC to RLC  
 * as a response to the DdatReq primitive from RLC.
 * Informs RLC of the successful transmission of TB's 
 * (ACK/NACK) along with Mapping Info. */
PRIVATE CONSTANT RguHqStaInd RgUiRguHqStaIndMt[RG_MAX_RGU_USR] =
{
#ifdef LCRGUIRGU
   cmPkRguHqStaInd,
#else
   PtUiRguHqStaInd,
#endif
#ifdef KW
   KwLiRguHqStaInd,
#else
   PtUiRguHqStaInd,
#endif
#ifdef LWLCRGUIRGU
   cmPkRguHqStaInd,
#else
   PtUiRguHqStaInd,
#endif
};
#endif /* LTE_L2_MEAS */

/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
/** @brief TTI indication from MAC to RRM */
PRIVATE CONSTANT RgrStaInd RgUiRgrStaIndMt[RG_MAX_RGR_USR] =
{
#ifdef LCRGUIRGR
   cmPkRgrStaInd,
#else
   PtUiRgrStaInd,
#endif
#ifdef NX
   NxLiRgrStaInd,
#else
   PtUiRgrStaInd,
#endif
};
#endif /* RGR_CQI_REPT */

/* LTE_ADV_FLAG_REMOVED_START */
/** @brief LOAD INF indication from MAC to RRM */
PRIVATE CONSTANT RgrLoadInfInd RgUiRgrLoadInfIndMt[RG_MAX_RGR_USR] =
{
#ifdef LCRGUIRGR
   cmPkRgrLoadInfInd,
#else
   PtUiRgrLoadInfInd,
#endif
#ifdef NX
   NxLiRgrLoadInfInd,
#else
   PtUiRgrLoadInfInd,
#endif
};
/* LTE_ADV_FLAG_REMOVED_END */

/** @brief TTI indication from MAC to RRM */
PRIVATE CONSTANT RgrUeStaInd RgUiRgrUeStaIndMt[RG_MAX_RGR_USR] =
{
#ifdef LCRGUIRGR
   cmPkRgrUeStaInd,
#else
   PtUiRgrUeStaInd,
#endif
#ifdef NX
   NxLiRgrUeStaInd,
#else
   PtUiRgrUeStaInd,
#endif
};

/** @brief Average PRB usage indication from MAC to RRM */
PRIVATE CONSTANT RgmPrbRprtIndFptr RgUiRgmSendPrbRprtIndMt[RG_MAX_RGM_USR] =
{
#ifdef RGM_LC
   cmPkRgmPrbRprtInd,
#else
   PtUiRgmPrbRprtInd,
#endif
#ifdef RM
   RmLiRgmPrbRprtInd,
#else
   PtUiRgmPrbRprtInd,
#endif
};


/** @brief Transmission Mode Change indication from MAC to RRM */
PRIVATE CONSTANT RgmTransModeIndFptr RgUiRgmChangeTransModeIndMt[RG_MAX_RGM_USR] =
{
#ifdef RGM_LC
   cmPkRgmTransModeInd,
#else
   PtUiRgmTransModeInd,
#endif
#ifdef RM
   RmLiRgmTransModeInd,
#else
   PtUiRgmTransModeInd,
#endif
};

PRIVATE CONSTANT RguFlowCntrlIndInfo RgUiRguFlowCntrlIndMt[RG_MAX_RGU_USR] =
{
#ifdef LCRGUIRGU
   cmPkRguFlowCntrlInd,
#else
   PtUiRguFlowCntrlInd,
#endif
#ifdef KW
   KwLiRguFlowCntrlInd,
#else
   PtUiRguFlowCntrlInd,
#endif
#ifdef LWLCRGUIRGU
   cmPkRguFlowCntrlInd,
#else
   PtUiRguFlowCntrlInd,
#endif
};
#ifdef __cplusplus
}
#endif /* __cplusplus */

#ifdef RG

/**
* @brief Confirmation from MAC to RRM for the bind/Unbind 
 * request for the interface saps
*
* @details
*
*     Function : RgUiRgrBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgUiRgrBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 RgUiRgrBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{

   TRC3(RgUiRgrBndCfm);

   RETVALUE((*RgUiRgrBndCfmMt[pst->selector])(pst, suId, status));

}
/**
* @brief Confirmation from MAC to RRM for the bind/Unbind 
 * request for the interface saps via RGM interface
*
* @details
*
*     Function : RgUiRgmBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgUiRgmBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 RgUiRgmBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{

   TRC3(RgUiRgmBndCfm);

   RETVALUE((*RgUiRgmBndCfmMt[pst->selector])(pst, suId, status));

}



/* Added for sending TTI tick to RRM */

/**
* @brief TTI indication from MAC to RGR user.
*
* @details
*
*     Function : RgUiRgrTtiInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgrTtiIndInfo  ttiInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgUiRgrTtiInd
(
Pst* pst,
SuId suId,
RgrTtiIndInfo  *ttiInd
)
#else
PUBLIC S16 RgUiRgrTtiInd(pst, suId, ttiInd)
Pst* pst;
SuId suId;
RgrTtiIndInfo *ttiInd;
#endif
{

   TRC3(RgUiRgrTtiInd);

   RETVALUE((*RgUiRgrTtiIndMt[pst->selector])(pst, suId, ttiInd));

}



/**
* @brief Configuration Confirm from MAC to RRM
*
* @details
*
*     Function : RgUiRgrCfgCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgrCfgTransId  transId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgUiRgrCfgCfm
(
Pst* pst,
SuId suId,
RgrCfgTransId transId,
U8 status
)
#else
PUBLIC S16 RgUiRgrCfgCfm(pst, suId, transId, status)
Pst* pst;
SuId suId;
RgrCfgTransId transId;
U8 status;
#endif
{

   TRC3(RgUiRgrCfgCfm);

   RETVALUE((*RgUiRgrCfgCfmMt[pst->selector])(pst, suId, transId, status));

}


/**
* @brief Confirmation from MAC to RRC for the bind/Unbind 
 * request for the interface saps
*
* @details
*
*     Function : RgUiCrgBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgUiCrgBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 RgUiCrgBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{

   TRC3(RgUiCrgBndCfm);

   RETVALUE((*RgUiCrgBndCfmMt[pst->selector])(pst, suId, status));

}



/**
* @brief Configuration Confirm from MAC to RRC
*
* @details
*
*     Function : RgUiCrgCfgCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   CrgCfgTransId  transId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgUiCrgCfgCfm
(
Pst* pst,
SuId suId,
CrgCfgTransId transId,
U8 status
)
#else
PUBLIC S16 RgUiCrgCfgCfm(pst, suId, transId, status)
Pst* pst;
SuId suId;
CrgCfgTransId transId;
U8 status;
#endif
{

   TRC3(RgUiCrgCfgCfm);

   RETVALUE((*RgUiCrgCfgCfmMt[pst->selector])(pst, suId, transId, status));

}


/**
* @brief Confirmation from MAC to RLC for the bind/Unbind 
 * request for the interface saps
*
* @details
*
*     Function : RgUiRguBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgUiRguBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 RgUiRguBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{

   TRC3(RgUiRguBndCfm);

   RETVALUE((*RgUiRguBndCfmMt[pst->selector])(pst, suId, status));

}

PUBLIC int macDDatIndSnt;
PUBLIC int macCDatIndSnt;


/**
* @brief Data Indication from MAC to RLC to 
 * forward the data received for common channels
*
* @details
*
*     Function : RgUiRguCDatInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RguCDatIndInfo  *  datInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgUiRguCDatInd
(
Pst* pst,
SuId suId,
RguCDatIndInfo  * datInd
)
#else
PUBLIC S16 RgUiRguCDatInd(pst, suId, datInd)
Pst* pst;
SuId suId;
RguCDatIndInfo  * datInd;
#endif
{
   macCDatIndSnt++;

   TRC3(RgUiRguCDatInd);

   RETVALUE((*RgUiRguCDatIndMt[pst->selector])(pst, suId, datInd));

}


/**
* @brief Data Indication from MAC to RLC to 
 * forward the data received for dedicated channels
*
* @details
*
*     Function : RgUiRguDDatInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RguDDatIndInfo  *  datInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgUiRguDDatInd
(
Pst* pst,
SuId suId,
RguDDatIndInfo  * datInd
)
#else
PUBLIC S16 RgUiRguDDatInd(pst, suId, datInd)
Pst* pst;
SuId suId;
RguDDatIndInfo  * datInd;
#endif
{
   macDDatIndSnt++;

   TRC3(RgUiRguDDatInd);

#if defined(MAC_RLC_UL_RBUF) && !defined(SS_RBUF)
   RETVALUE(RgUiRguDDatIndRbuf(datInd));
#else 
   RETVALUE((*RgUiRguDDatIndMt[pst->selector])(pst, suId, datInd));
#endif    
}



/**
* @brief Status Indication from MAC to RLC  
 * as a response to the staRsp primitive from RLC.
 * Informs RLC of the totalBufferSize and Timing Info 
 * for the transmission on common channels.
*
* @details
*
*     Function : RgUiRguCStaInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RguCStaIndInfo  *  staInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgUiRguCStaInd
(
Pst* pst,
SuId suId,
RguCStaIndInfo  * staInd
)
#else
PUBLIC S16 RgUiRguCStaInd(pst, suId, staInd)
Pst* pst;
SuId suId;
RguCStaIndInfo  * staInd;
#endif
{

   TRC3(RgUiRguCStaInd);

   RETVALUE((*RgUiRguCStaIndMt[pst->selector])(pst, suId, staInd));

}



/**
* @brief Status Indication from MAC to RLC  
 * as a response to the staRsp primitive from RLC.
 * Informs RLC of the totalBufferSize and Timing Info 
 * for the transmission on dedicated channels.
*
* @details
*
*     Function : RgUiRguDStaInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RguDStaIndInfo  *  staInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgUiRguDStaInd
(
Pst* pst,
SuId suId,
RguDStaIndInfo  * staInd
)
#else
PUBLIC S16 RgUiRguDStaInd(pst, suId, staInd)
Pst* pst;
SuId suId;
RguDStaIndInfo  * staInd;
#endif
{

   TRC3(RgUiRguDStaInd);

   RETVALUE((*RgUiRguDStaIndMt[pst->selector])(pst, suId, staInd));

}
#ifdef LTE_L2_MEAS
/* TODO: Function header */
#ifdef ANSI
PUBLIC S16 RgUiRguHqStaInd
(
Pst* pst,
SuId suId,
RguHarqStatusInd *harqStatusInd
)
#else
PUBLIC S16 RgUiRguHqStaInd(pst, suId, harqStatusInd)
Pst* pst;
SuId suId;
RguHarqStatusInd *harqStatusInd;
#endif
{

   TRC3(RgUiRguHqStaInd);
#if defined(SPLIT_RLC_DL_TASK) && defined(MAC_RLC_HARQ_STA_RBUF)
   S16 ret=ROK;
   ret = RgUiRguHqStaIndRbuf(pst,suId,harqStatusInd);
   RETVALUE(ret);
#else 
   RETVALUE((*RgUiRguHqStaIndMt[pst->selector])(pst, suId, harqStatusInd));
#endif 

} /* RgUiRguHqStaInd */
#endif /* LTE_L2_MEAS */

#ifdef ANSI
PUBLIC S16 RgUiRguFlowCntrlInd
(
Pst* pst,
SuId suId,
RguFlowCntrlInd *flowCntrlInd
)
#else
PUBLIC S16 RgUiRguFlowCntrlInd(pst, suId, harqStatusInd)
Pst* pst;
SuId suId;
RguFlowCntrlInd *flowCntrlInd;
#endif
{
  RETVALUE((*RgUiRguFlowCntrlIndMt[pst->selector])(pst, suId, flowCntrlInd));
}
/* Added for SI Enhancement*/
#ifdef RGR_SI_SCH
/**
* @brief SI Configuration Confirm from MAC to RRM
*
* @details
*
*     Function : RgUiRgrSiCfgCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgrCfgTransId  transId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgUiRgrSiCfgCfm
(
Pst* pst,
SuId suId,
RgrCfgTransId transId,
U8 status
)
#else
PUBLIC S16 RgUiRgrSiCfgCfm(pst, suId, transId, status)
Pst* pst;
SuId suId;
RgrCfgTransId transId;
U8 status;
#endif
{

   TRC3(RgUiRgrSiCfgCfm);

   RETVALUE((*RgUiRgrSiCfgCfmMt[pst->selector])(pst, suId, transId, status));

}

/**
* @brief Warning SI Configuration Confirm from MAC to RRM
*
* @details
*
*     Function : RgUiRgrWarningSiCfgCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgrCfgTransId  transId
*  @param[in]   U8   siId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgUiRgrWarningSiCfgCfm
(
Pst* pst,
SuId suId,
RgrCfgTransId transId,
U8   siId,
U8 status
)
#else
PUBLIC S16 RgUiRgrWarningSiCfgCfm(pst, suId, transId, siId,status)
Pst* pst;
SuId suId;
RgrCfgTransId transId;
U8  siId;
U8 status;
#endif
{
   TRC3(RgUiRgrWarningSiCfgCfm);

   RETVALUE((*RgUiRgrWarningSiCfgCfmMt[pst->selector])
         (pst, suId, transId, siId, status));
}

#endif/*RGR_SI_SCH*/


/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
/**
* @brief STA indication from MAC to RGR user.
*
* @details
*
*     Function : RgUiRgrStaInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgrStaIndInfo  *staInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgUiRgrStaInd
(
Pst* pst,
SuId suId,
RgrStaIndInfo  *staInd
)
#else
PUBLIC S16 RgUiRgrStaInd(pst, suId, staInd)
Pst* pst;
SuId suId;
RgrStaIndInfo *staInd;
#endif
{

   TRC3(RgUiRgrStaInd);

   RETVALUE((*RgUiRgrStaIndMt[pst->selector])(pst, suId, staInd));

}
#endif /* End of RGR_CQI_REPT */

/* LTE_ADV_FLAG_REMOVED_START */
/**
 * @brief LOAD INF indication from MAC to RGR user.
 *
 * @details
 *
 *     Function : RgUiRgrLoadInfInd
 *
 *  @param[in]   Pst*  pst
 *  @param[in]   SuId  suId
 *  @param[in]   RgrLoadInfIndInfo  *loadInfInd
 *  @return   S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 RgUiRgrLoadInfInd
(
 Pst* pst,
 SuId suId,
 RgrLoadInfIndInfo  *loadInfInd
 )
#else
PUBLIC S16 RgUiRgrLoadInfInd(pst, suId, loadInfInd)
   Pst* pst;
   SuId suId;
   RgrLoadInfIndInfo *loadInfInd;
#endif
{

   TRC3(RgUiRgrLoadInfInd);

   RETVALUE((*RgUiRgrLoadInfIndMt[pst->selector])(pst, suId, loadInfInd));

}
/* LTE_ADV_FLAG_REMOVED_END */

/**
* @brief UESTA indication from MAC to RGR user.
*
* @details
*
*     Function : RgUiRgrUeStaInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgrStaIndInfo  *staInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgUiRgrUeStaInd
(
Pst             *pst,
SuId            suId,
RgrUeStaIndInfo *ueStaInd
)
#else
PUBLIC S16 RgUiRgrUeStaInd(pst, suId, ueStaInd)
Pst             *pst;
SuId            suId;
RgrUeStaIndInfo *ueStaInd;
#endif
{

   TRC3(RgUiRgrUeStaInd);

   RETVALUE((*RgUiRgrUeStaIndMt[pst->selector])(pst, suId, ueStaInd));

}
#endif /*--ifdef RG--*/

#ifdef PTRGUICRG

/**
* @brief Confirmation from MAC to RRC for the bind/Unbind 
 * request for the interface saps
*
* @details
*
*     Function : PtUiCrgBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiCrgBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 PtUiCrgBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{

   TRC3(PtUiCrgBndCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(status);

   RETVALUE(ROK);

}



/**
* @brief Configuration Confirm from MAC to RRC
*
* @details
*
*     Function : PtUiCrgCfgCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   CrgCfgTransId  transId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiCrgCfgCfm
(
Pst* pst,
SuId suId,
CrgCfgTransId transId,
U8 status
)
#else
PUBLIC S16 PtUiCrgCfgCfm(pst, suId, transId, status)
Pst* pst;
SuId suId;
CrgCfgTransId transId;
U8 status;
#endif
{

   TRC3(PtUiCrgCfgCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(transId);
   UNUSED(status);

   RETVALUE(ROK);

}
#endif /*--ifdef PTRGUICRG--*/

#ifdef PTRGUIRGU

/**
* @brief Confirmation from MAC to RLC for the bind/Unbind 
 * request for the interface saps
*
* @details
*
*     Function : PtUiRguBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiRguBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 PtUiRguBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{

   TRC3(PtUiRguBndCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(status);

   RETVALUE(ROK);

}



/**
* @brief Data Indication from MAC to RLC to 
 * forward the data received for common channels
*
* @details
*
*     Function : PtUiRguCDatInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RguCDatIndInfo  *  datInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiRguCDatInd
(
Pst* pst,
SuId suId,
RguCDatIndInfo  * datInd
)
#else
PUBLIC S16 PtUiRguCDatInd(pst, suId, datInd)
Pst* pst;
SuId suId;
RguCDatIndInfo  * datInd;
#endif
{

   TRC3(PtUiRguCDatInd);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(datInd);

   RETVALUE(ROK);

}



/**
* @brief Data Indication from MAC to RLC to 
 * forward the data received for dedicated channels
*
* @details
*
*     Function : PtUiRguDDatInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RguDDatIndInfo  *  datInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiRguDDatInd
(
Pst* pst,
SuId suId,
RguDDatIndInfo  * datInd
)
#else
PUBLIC S16 PtUiRguDDatInd(pst, suId, datInd)
Pst* pst;
SuId suId;
RguDDatIndInfo  * datInd;
#endif
{

   TRC3(PtUiRguDDatInd);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(datInd);

   RETVALUE(ROK);

}



/**
* @brief Status Indication from MAC to RLC  
 * as a response to the staRsp primitive from RLC.
 * Informs RLC of the totalBufferSize and Timing Info 
 * for the transmission on common channels.
*
* @details
*
*     Function : PtUiRguCStaInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RguCStaIndInfo  *  staInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiRguCStaInd
(
Pst* pst,
SuId suId,
RguCStaIndInfo  * staInd
)
#else
PUBLIC S16 PtUiRguCStaInd(pst, suId, staInd)
Pst* pst;
SuId suId;
RguCStaIndInfo  * staInd;
#endif
{

   TRC3(PtUiRguCStaInd);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(staInd);

   RETVALUE(ROK);

}



/**
* @brief Status Indication from MAC to RLC  
 * as a response to the staRsp primitive from RLC.
 * Informs RLC of the totalBufferSize and Timing Info 
 * for the transmission on dedicated channels.
*
* @details
*
*     Function : PtUiRguDStaInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RguDStaIndInfo  *  staInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiRguDStaInd
(
Pst* pst,
SuId suId,
RguDStaIndInfo  * staInd
)
#else
PUBLIC S16 PtUiRguDStaInd(pst, suId, staInd)
Pst* pst;
SuId suId;
RguDStaIndInfo  * staInd;
#endif
{

   TRC3(PtUiRguDStaInd);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(staInd);

   RETVALUE(ROK);

}
#ifdef LTE_L2_MEAS
/* TODO: Function Header */
#ifdef ANSI
PUBLIC S16 PtUiRguHqStaInd
(
Pst* pst,
SuId suId,
RguHarqStatusInd *harqStatusInd
)
#else
PUBLIC S16 PtUiRguHqStaInd(pst, suId, harqStatusInd)
Pst* pst;
SuId suId;
RguHarqStatusInd *harqStatusInd;
#endif
{

   TRC3(PtUiRguHqStaInd);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(harqStatusInd);

   RETVALUE(ROK);

}
#endif /* LTE_L2_MEAS */

#ifdef ANSI
PUBLIC S16 PtUiRguFlowCntrlInd
(
Pst* pst,
SuId suId,
RguFlowCntrlInd *flowCntrlInd
)
#else
PUBLIC S16 PtUiRguFlowCntrlInd(pst, suId, flowCntrlInd)
Pst* pst;
SuId suId;
RguHarqStatusInd *flowCntrlInd;
#endif
{

   TRC3(PtUiRguFlowCntrlInd);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(flowCntrlInd);

   RETVALUE(ROK);

}

#endif /*--ifdef PTRGUIRGU--*/

#ifdef PTRGUIRGR

/**
* @brief Confirmation from MAC to RRM for the bind/Unbind 
 * request for the interface saps
*
* @details
*
*     Function : PtUiRgrBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiRgrBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 PtUiRgrBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{

   TRC3(PtUiRgrBndCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(status);

   RETVALUE(ROK);

}

/* Added for sending TTI tick to RRM */

/**
* @brief TTI indication from MAC to RGR user.
*
* @details
*
*     Function : PtUiRgrTtiInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgrTtiIndInfo  *ttiInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiRgrTtiInd
(
Pst* pst,
SuId suId,
RgrTtiIndInfo *ttiInd
)
#else
PUBLIC S16 PtUiRgrTtiInd(pst, suId, ttiInd)
Pst* pst;
SuId suId;
RgrTtiIndInfo *ttiInd;
#endif
{

   TRC3(PtUiRgrTtiInd);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(ttiInd);

   RETVALUE(ROK);

}



/**
* @brief Configuration Confirm from MAC to RRM
*
* @details
*
*     Function : PtUiRgrCfgCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgrCfgTransId  transId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiRgrCfgCfm
(
Pst* pst,
SuId suId,
RgrCfgTransId transId,
U8 status
)
#else
PUBLIC S16 PtUiRgrCfgCfm(pst, suId, transId, status)
Pst* pst;
SuId suId;
RgrCfgTransId transId;
U8 status;
#endif
{

   TRC3(PtUiRgrCfgCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(transId);
   UNUSED(status);

   RETVALUE(ROK);

}

/* Added for SI Enhancement*/
#ifdef RGR_SI_SCH
/**
* @brief SI Configuration Confirm from MAC to RRM
*
* @details
*
*     Function : PtUiRgrSiCfgCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgrCfgTransId  transId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiRgrSiCfgCfm
(
Pst* pst,
SuId suId,
RgrCfgTransId transId,
U8 status
)
#else
PUBLIC S16 PtUiRgrSiCfgCfm(pst, suId, transId, status)
Pst* pst;
SuId suId;
RgrCfgTransId transId;
U8 status;
#endif
{

   TRC3(PtUiRgrSiCfgCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(transId);
   UNUSED(status);

   RETVALUE(ROK);

}

/**
* @brief Warning SI Configuration Confirm from MAC to RRM
*
* @details
*
*     Function : PtUiRgrWarningSiCfgCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgrCfgTransId  transId
*  @param[in]   U8 siId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiRgrWarningSiCfgCfm
(
Pst* pst,
SuId suId,
RgrCfgTransId transId,
U8 siId,
U8 status
)
#else
PUBLIC S16 PtUiRgrWarningSiCfgCfm(pst, suId, transId, siId, status)
Pst* pst;
SuId suId;
RgrCfgTransId transId;
U8  siId;
U8 status;
#endif
{
   TRC3(PtUiRgrWarningSiCfgCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(transId);
   UNUSED(siId);
   UNUSED(status);

   RETVALUE(ROK);
}
#endif/* RGR_SI_SCH */


/* ccpu00117452 - MOD - Changed macro name from
   RGR_RRM_DLPWR_CNTRL to RGR_CQI_REPT */
#ifdef RGR_CQI_REPT
/**
* @brief STA indication from MAC to RGR user.
*
* @details
*
*     Function : PtUiRgrStaInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgrStaIndInfo  *staInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiRgrStaInd
(
Pst* pst,
SuId suId,
RgrStaIndInfo  *staInd
)
#else
PUBLIC S16 PtUiRgrStaInd(pst, suId, staInd)
Pst* pst;
SuId suId;
RgrStaIndInfo *staInd;
#endif
{

   TRC3(PtUiRgrStaInd);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(staInd);

   RETVALUE(ROK);

}
#endif /* End of RGR_CQI_REPT */
/* LTE_ADV_FLAG_REMOVED_START */
/**
 * @brief LOAD INF indication from MAC to RGR user.
 *
 * @details
 *
 *     Function : PtUiRgrLoadInfInd
 *
 *  @param[in]   Pst*  pst
 *  @param[in]   SuId  suId
 *  @param[in]   RgrLoadInfIndInfo  *loadInfInd
 *  @return   S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 PtUiRgrLoadInfInd
(
 Pst* pst,
 SuId suId,
 RgrLoadInfIndInfo  *loadInfInd
 )
#else
PUBLIC S16 PtUiRgrLoadInfInd(pst, suId, loadInfInd)
   Pst* pst;
   SuId suId;
   RgrLoadInfIndInfo *loadInfInd;
#endif
{

   TRC3(PtUiRgrLoadInfInd);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(loadInfInd);

   RETVALUE(ROK);

}
/* LTE_ADV_FLAG_REMOVED_END */

/**
* @brief STA indication from MAC to RGR user.
*
* @details
*
*     Function : PtUiRgrUeStaInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgrUeStaIndInfo  *ueStaInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiRgrUeStaInd
(
Pst* pst,
SuId suId,
RgrUeStaIndInfo  *ueStaInd
)
#else
PUBLIC S16 PtUiRgrUeStaInd(pst, suId, ueStaInd)
Pst* pst;
SuId suId;
RgrUeStaIndInfo *ueStaInd;
#endif
{

   TRC3(PtUiRgrUeStaInd);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(ueStaInd);

   RETVALUE(ROK);

}
#endif /*--ifdef PTRGUIRGR--*/


#ifdef PTRGUIRGM
/**
* @brief Confirmation from MAC to RRM for the bind/Unbind 
 * request for the interface saps via RGM interface
*
* @details
*
*     Function : PtUiRgmBndCfm
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   U8  status
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiRgmBndCfm
(
Pst* pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 PtUiRgmBndCfm(pst, suId, status)
Pst* pst;
SuId suId;
U8 status;
#endif
{

   TRC3(PtUiRgmBndCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(status);

   RETVALUE(ROK);

}

/**
* @brief Average PRB indication from MAC to RRM
*
* @details
*
*     Function : PtUiRgmPrbRprtInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgrUeStaIndInfo  *ueStaInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiRgmPrbRprtInd
(
Pst* pst,
SuId suId,
RgmPrbRprtInd *prbRprtInd
)
#else
PUBLIC S16 PtUiRgmPrbRprtInd(pst, suId, prbRprtInd)
Pst* pst;
SuId suId;
RgmPrbRprtInd *prbRprtInd;
#endif
{

   TRC3(PtUiRgmPrbRprtInd);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(prbRprtInd);

   RETVALUE(ROK);

}

/**
* @brief Transmission Mode Change indication from MAC to RRM
*
* @details
*
*     Function : PtUiRgmTransModeInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgmTransModeInd *transModeInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtUiRgmTransModeInd
(
Pst* pst,
SuId suId,
RgmTransModeInd *transModeInd
)
#else
PUBLIC S16 PtUiRgmTransModeInd(pst, suId, transModeInd)
Pst* pst;
SuId suId;
RgmTransModeInd *transModeInd;
#endif
{

   TRC3(PtUiRgmTransModeInd);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(transModeInd);

   RETVALUE(ROK);

}
#endif

/**
* @brief Average PRB indication from MAC to RRM
*
* @details
*
*     Function : RgUiRgmSendPrbRprtInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgmPrbRprtInd prbRprtInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgUiRgmSendPrbRprtInd
(
Pst* pst,
SuId suId,
RgmPrbRprtInd *prbRprtInd
)
#else
PUBLIC S16 RgUiRgmSendPrbRprtInd(pst, suId, prbRprtInd)
Pst* pst;
SuId suId;
RgmPrbRprtInd *prbRprtInd;
#endif

{
   TRC2(RgUiRgmSendPrbRprtInd);
   RETVALUE((*RgUiRgmSendPrbRprtIndMt[pst->selector])(pst, suId, prbRprtInd));
}

/**
* @brief Transmission Mode change indication from MAC to RRM
*
* @details
*
*     Function : RgUiRgmChangeTransModeInd
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RgmTransModeInd transModeInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgUiRgmChangeTransModeInd
(
Pst* pst,
SuId suId,
RgmTransModeInd *transModeInd
)
#else
PUBLIC S16 RgUiRgmChangeTransModeInd(pst, suId, transModeInd)
Pst* pst;
SuId suId;
RgmTransModeInd *transModeInd;
#endif

{
   TRC2(RgUiRgmChangeTransModeInd);
   RETVALUE((*RgUiRgmChangeTransModeIndMt[pst->selector])(pst, suId, transModeInd));
}

#if defined(MAC_RLC_UL_RBUF) && !defined(SS_RBUF)
PRIVATE S16 RgUiRguDDatIndRbuf(RguDDatIndInfo  *datInd)
{
      S16  ret = ROK;
      Void *elem = NULLP;
      RguDedDatInd1 *rguDatIndl = NULLP;
      elem = SRngGetWIndx(SS_RNG_BUF_ULMAC_TO_ULRLC);
      rguDatIndl = (RguDedDatInd1 *) elem;
      if (NULLP == elem)
      {
         SsRngInfoTbl[SS_RNG_BUF_ULMAC_TO_ULRLC].pktDrop++;
         U32 i,j;
         for(i = 0; i< datInd->numLch; i++)
         {
            for(j = 0; j < datInd->lchData[i].pdu.numPdu; j++)
            {
               if(datInd->lchData[i].pdu.mBuf[j])
               {
                  SPutMsg(datInd->lchData[i].pdu.mBuf[j]);
               }
            }
         }
         rgFreeSharableSBuf((Data **)&datInd,sizeof(RguDDatIndInfo));
         ret = RFAILED;
      }
      else
      {
         rguDatIndl->msg = datInd;
         SRngIncrWIndx(SS_RNG_BUF_ULMAC_TO_ULRLC);
         SsRngInfoTbl[SS_RNG_BUF_ULMAC_TO_ULRLC].pktRate++;
      }
  RETVALUE(ret);
}
#endif
#ifdef RLC_MAC_DAT_REQ_RBUF
#ifdef ANSI
PUBLIC S16 rgDlDatReqBatchProc
(
Void
)
#else
PUBLIC S16 rgDlDatReqBatchProc()
Void;
#endif
{
/* Read from Ring Buffer and process PDCP packets */

   U8 rngBufDeqIndx = 0;
   PRIVATE Pst rgDDatRbfuPst ={1,1,ENTRG,0,ENTKW,1,PRIOR0,RTESPEC,EVTRGUDDATREQ,0,0,2,0};
   PRIVATE Pst rgCDatRbfuPst ={1,1,ENTRG,0,ENTKW,1,PRIOR0,RTESPEC,EVTRGUCDATREQ,0,0,2,0};
   Void *elmIndx = NULLP;
   RguInfoRingElem *datReqRing=NULLP;
   elmIndx = SRngGetRIndx(SS_RNG_BUF_DLRLC_TO_DLMAC_DAT_REQ);
   while(NULLP != elmIndx)
   {
      datReqRing= (RguInfoRingElem *)elmIndx;
      SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC_DAT_REQ].nPktProc++;;//Number of pkt processed in tti
      if(datReqRing->msg)
      {
         if(datReqRing->event == EVTRGUDDATREQ)
         {
         RgUiRguDDatReq(&rgDDatRbfuPst, datReqRing->spId, (RguDDatReqInfo *) datReqRing->msg);
         }
         else
         { 
          //  printf("CSHP:*** Received CDatReq in MAC Ring  \n");
         RgUiRguCDatReq(&rgCDatRbfuPst, datReqRing->spId, (RguCDatReqInfo *) datReqRing->msg); 
         }
      }
      else
      {
	      printf(" Buf Pinter is NULL RBUF Read(%ld) write (%ld) \n",SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC_DAT_REQ].r_addr->read,
              SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC_DAT_REQ].r_addr->write);
	      /* Due to the cache issue we are verifying the mbuf pointer again and sending it to rlc if avilable*/
              if(datReqRing->msg)
	      {
		      if(datReqRing->event == EVTRGUDDATREQ)
			      RgUiRguDDatReq(&rgDDatRbfuPst, datReqRing->spId, (RguDDatReqInfo *) datReqRing->msg);
		      else 
			      RgUiRguCDatReq(&rgCDatRbfuPst, datReqRing->spId, (RguCDatReqInfo *) datReqRing->msg); 
	      }else 
	      {
		      printf(" still Buf Pinter is NULL RBUF Read(%ld) write (%ld) \n",SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC_DAT_REQ].r_addr->read,
		      SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC_DAT_REQ].r_addr->write);
	      } 
      }
      datReqRing->msg=NULLP;
      SRngIncrRIndx(SS_RNG_BUF_DLRLC_TO_DLMAC_DAT_REQ);
      datReqRing->event=0; 
      elmIndx = NULLP;
      datReqRing= NULLP;
      rngBufDeqIndx++;

      if(rngBufDeqIndx >= SS_RNG_MAX_DLRLC_TO_DLMAC_DAT_REQ_DQ_CNT)
        break;

      if((elmIndx = SRngGetRIndx(SS_RNG_BUF_DLRLC_TO_DLMAC_DAT_REQ)) == NULLP)
      break;
   }
 
   RETVALUE(ROK);
}
#endif 

#ifdef RLC_MAC_STA_RSP_RBUF
#ifdef ANSI
PUBLIC S16 rgDlStaRspBatchProc
(
Void
)
#else
PUBLIC S16 rgDlStaRspBatchProc()
Void;
#endif
{
/* Read from Ring Buffer and process PDCP packets */

   U8 rngBufDeqIndx = 0;
   PRIVATE Pst rgDStaRbfuPst ={1,1,ENTRG,0,ENTKW,1,PRIOR0,RTESPEC,EVTRGUDSTARSP,0,0,2,0};
   PRIVATE Pst rgCStaRbfuPst ={1,1,ENTRG,0,ENTKW,1,PRIOR0,RTESPEC,EVTRGUCSTARSP,0,0,2,0};
   Void *elmIndx = NULLP;
   RguInfoRingElem *staRspRing=NULLP;

   elmIndx = SRngGetRIndx(SS_RNG_BUF_DLRLC_TO_DLMAC);
   while(NULLP != elmIndx)
   {
      staRspRing= (RguInfoRingElem *)elmIndx;
      SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC].nPktProc++;;//Number of pkt processed in tti
      
      if(staRspRing->msg!= NULLP)
      {
         if( staRspRing->event == EVTRGUDSTARSP)
         {
         RgUiRguDStaRsp(&rgDStaRbfuPst, staRspRing->spId, (RguDStaRspInfo *) staRspRing->msg);
         }
         else
         {
         RgUiRguCStaRsp(&rgCStaRbfuPst, staRspRing->spId, (RguCStaRspInfo *) staRspRing->msg);
         } 
      }
      else
      {
	      printf(" Buf Pinter is NULL RBUF Read(%ld) write (%ld) \n",SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC].r_addr->read,
			      SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC].r_addr->write);
	      /* Due to the cache issue we are verifying the mbuf pointer again and sending it to rlc if avilable*/
              if(staRspRing->msg!= NULLP)
	      {
		      if( staRspRing->event == EVTRGUDSTARSP)
			      RgUiRguDStaRsp(&rgDStaRbfuPst, staRspRing->spId, (RguDStaRspInfo *) staRspRing->msg);
		      else  
			      RgUiRguCStaRsp(&rgCStaRbfuPst, staRspRing->spId, (RguCStaRspInfo *) staRspRing->msg);
	      }else 
	      {
		      printf(" still Buf Pinter is NULL RBUF Read(%ld) write (%ld) \n",SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC].r_addr->read,
				      SsRngInfoTbl[SS_RNG_BUF_DLRLC_TO_DLMAC].r_addr->write);
	      } 
      }
      staRspRing->msg=NULLP;
      SRngIncrRIndx(SS_RNG_BUF_DLRLC_TO_DLMAC);
      staRspRing->event =0;    
      elmIndx = NULLP;
      staRspRing= NULLP;

      rngBufDeqIndx++;

      if(rngBufDeqIndx >= SS_RNG_MAX_DLRLC_TO_DLMAC_STA_RSP_DQ_CNT)
        break;

      if((elmIndx = SRngGetRIndx(SS_RNG_BUF_DLRLC_TO_DLMAC)) == NULLP)
      break;
   }
 
   RETVALUE(ROK);
}
#endif 
#ifdef MAC_RLC_HARQ_STA_RBUF
#ifdef ANSI
PUBLIC S16 RgUiRguHqStaIndRbuf 
(
Pst* pst,
SuId suId,
RguHarqStatusInd *harqStatusInd
)
#else
PUBLIC S16 RgUiRguHqStaIndRbuf(pst, suId, harqStatusInd)
Pst* pst;
SuId suId;
RguHarqStatusInd *harqStatusInd;
#endif
{
   S16  ret = ROK;
   Void *elem = NULLP;
   RguHarqStatusInd *harqStaInd = NULLP;
   elem = SRngGetWIndx(SS_RNG_BUF_MAC_HARQ);
   harqStaInd = (RguHarqStatusInd*) elem;
   if (NULLP == elem )
   {
      SsRngInfoTbl[SS_RNG_BUF_MAC_HARQ].pktDrop++;
      ret = RFAILED;
   }
   else
   {
      harqStaInd = (RguHarqStatusInd *)elem;
      cmMemcpy((U8 *)harqStaInd, (U8 *)harqStatusInd, sizeof(RguHarqStatusInd));
      SRngIncrWIndx(SS_RNG_BUF_MAC_HARQ);
      SsRngInfoTbl[SS_RNG_BUF_MAC_HARQ].pktRate++;
   }
   RETVALUE(ret);
}
#endif


/**********************************************************************
 
         End of file:     gk_ptui.c@@/main/3 - Sat Jul 30 02:21:33 2011
 
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
/main/1      ---     sm                1. Initial Release.
/main/2      ---     sm                1. LTE MAC 2.1 release
             rg001.201 sm            1. Added TTI indication towards 
                                        RGR user.
             rg002.201 ns            1. Fixed PT build issue for RGR RRM tick.
             rg004.201 rnt           1. Updated for SI enhancement
             rg009.201 rnt           1. Updated for TFU Upgrade feature
/main/3      ---        gvj  1. Updated for LTE MAC Rel 3.1           
$SID$        ---        rt   1. LTE MAC 4.1 release
*********************************************************************91*/

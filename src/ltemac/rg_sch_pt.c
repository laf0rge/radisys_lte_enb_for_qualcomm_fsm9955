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
  
     Desc:     C source code for INF Interface Module 
  
     File:     rg_sch_pt.c 
  
     Sid:      gk_sch_pt.c@@/main/2 - Sat Jul 30 02:21:46 2011
  
     Prg:      rr
  
**********************************************************************/

/** @file rg_sch_pt.c
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
#include "cm_mblk.h"       /* memory management */
#include "cm_lte.h"        /* Common LTE Defines */
#include "tfu.h"           /* RGU Interface defines */
#ifdef LTE_L2_MEAS
#include "lrg.h"
#endif
#include "rg_sch_inf.h"           /* RGU Interface defines */

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
#include "cm_mblk.x"       /* memory management */
#include "cm_lte.x"        /* Common LTE Defines */
#include "tfu.x"           /* RGU Interface includes */
#ifdef LTE_L2_MEAS
#include "lrg.x"
#endif
#include "rg_sch_inf.x"           /* RGU Interface includes */

#define RG_MAX_SCH 1

PRIVATE CONSTANT CellRegReq RgSchMacCellRegReqMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkSchMacCellRegReq,
#else
   RgSchMacCellRegReq
#endif
};

PRIVATE CONSTANT DedBoUpdtReq RgMacSchDedBoUpdtReqMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkMacSchDedBoUpdtReq,
#else
   RgMacSchDedBoUpdtReq
#endif
};

PRIVATE CONSTANT CmnBoUpdtReq RgMacSchCmnBoUpdtReqMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkMacSchCmnBoUpdtReq,
#else
   RgMacSchCmnBoUpdtReq
#endif
};

PRIVATE CONSTANT SfRecpInd RgMacSchSfRecpIndMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkMacSchSfRecpInd
#else
   RgMacSchSfRecpInd,
#endif
};
/*Fix: start: Inform UE delete to scheduler*/
PRIVATE CONSTANT UeDelInd RgMacSchUeDelIndMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkMacSchUeDelInd
#else
   RgMacSchUeDelInd,
#endif
};
/*Fix: end: Inform UE delete to scheduler*/
PRIVATE CONSTANT SfAllocReq RgSchMacSfAllocReqMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkSchMacSfAllocReq,
#else
   RgSchMacSfAllocReq
#endif
};

PRIVATE CONSTANT RlsHqReq RgSchMacRlsHqReqMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkSchMacRlsHqReq,
#else
   RgSchMacRlsHqReq
#endif
};

PRIVATE CONSTANT RlsRntiReq RgSchMacRlsRntiReqMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkSchMacRlsRntiReq,
#else
   RgSchMacRlsRntiReq
#endif
};

PRIVATE CONSTANT LcgReg RgSchMacLcgRegReqMt[RG_MAX_SCH] =
{
//#ifdef LCSCH
#if 0
   cmPkSchMacLcgRegReq,
#else
   RgSchMacLcgRegReq
#endif
};

#ifdef LTEMAC_SPS
PRIVATE CONSTANT SpsLcReg RgSchMacSpsLcRegReqMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkSchMacSpsLcRegReq,
#else
   RgSchMacSpsLcRegReq
#endif
};

PRIVATE CONSTANT UlSpsReset RgSchMacUlSpsResetMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkSchMacUlSpsReset,
#else
   RgSchMacUlSpsResetReq
#endif
};



PRIVATE CONSTANT SpsLcDereg RgSchMacSpsLcDeregReqMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkSchMacSpsLcDeregReq,
#else
   RgSchMacSpsLcDeregReq
#endif
};

PRIVATE CONSTANT SpsRelInd RgMacSchSpsRelIndMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkMacSchSpsRelInd,
#else
   RgMacSchSpsRelInd
#endif
};

#endif /* LTEMAC_SPS */

#ifdef LTE_L2_MEAS
PRIVATE CONSTANT L2MeasReq RgSchMacL2MeasReqMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkSchMacL2MeasReq
#else
   RgSchMacL2MeasReq
#endif
};
/*Added for Rsys oam*/
PRIVATE CONSTANT L2MeasStopReq RgSchMacL2MeasStopReqMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkSchMacL2MeasStopReq
#else
   RgSchMacL2MeasStopReq
#endif
};

PRIVATE CONSTANT L2MeasSendReq RgSchMacL2MeasSendReqMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkSchMacL2MeasSendReq
#else
   RgSchMacL2MeasSendReq
#endif
};

PRIVATE CONSTANT L2MeasCfm RgMacSchL2MeasCfmMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkMacSchL2MeasCfm
#else
   RgMacSchL2MeasCfm
#endif
};
PRIVATE CONSTANT L2MeasStopCfm RgMacSchL2MeasStopCfmMt[RG_MAX_SCH] =
{
#ifdef LCSCH
   cmPkMacSchL2MeasStopCfm
#else
   RgMacSchL2MeasStopCfm
#endif
};
#endif/*LTE_L2_MEAS*/
/**
* @brief Data Indication from MAC to RLC to 
 * forward the data received for dedicated channels
*
* @details
*
*     Function : RgSchMacCellReg
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RguDDatIndInfo  *  datInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgSchMacCellReg
(
Pst* pst,
RgInfCellReg*   regReq
)
#else
PUBLIC S16 RgSchMacCellReg(pst, regReq)
Pst* pst;
RgInfCellReg*   regReq;
#endif
{

   TRC3(RgSchMacCellReg);

   RETVALUE((*RgSchMacCellRegReqMt[0])(pst, regReq));
}

/**
* @brief Data Indication from MAC to RLC to 
 * forward the data received for dedicated channels
*
* @details
*
*     Function : RgMacSchDedBoUpdt
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RguDDatIndInfo  *  datInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgMacSchDedBoUpdt
(
Pst* pst,
RgInfDedBoRpt*  boRpt
)
#else
PUBLIC S16 RgMacSchDedBoUpdt(pst, boRpt)
Pst* pst;
RgInfDedBoRpt*  boRpt;
#endif
{

   TRC3(RgMacSchDedBoUpdt);

   RETVALUE((*RgMacSchDedBoUpdtReqMt[0])(pst, boRpt));
}

/**
* @brief Data Indication from MAC to RLC to 
 * forward the data received for dedicated channels
*
* @details
*
*     Function : RgMacSchCmnBoUpdt
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RguDDatIndInfo  *  datInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgMacSchCmnBoUpdt
(
Pst* pst,
RgInfCmnBoRpt*  boRpt
)
#else
PUBLIC S16 RgMacSchCmnBoUpdt(pst, boRpt)
Pst* pst;
RgInfCmnBoRpt*  boRpt;
#endif
{

   TRC3(RgMacSchCmnBoUpdt);

   RETVALUE((*RgMacSchCmnBoUpdtReqMt[0])(pst, boRpt));
}

/*Fix: start: Inform UE delete to scheduler*/
/**
* @brief This primitive is used to indicate deletion of UE
*        at MAC to scheduler. 
*
* @details
*
*     Function : RgMacSchUeDel
*
*  @param[in]   Pst*  pst
*  @param[in]   RgInfSfDatInd*  ueDelInd 
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgMacSchUeDel
(
Pst* pst,
RgInfUeDelInd*  ueDelInd
)
#else
PUBLIC S16 RgMacSchUeDel(pst, datInd)
Pst* pst;
RgInfUeDelInd*  ueDelInd;
#endif
{

   TRC3(RgMacSchUeDel);

   RETVALUE((*RgMacSchUeDelIndMt[0])(pst, ueDelInd));
}
/*Fix: end: Inform UE delete to scheduler*/
/**
* @brief Data Indication from MAC to RLC to 
 * forward the data received for dedicated channels
*
* @details
*
*     Function : RgMacSchSfRecp
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RguDDatIndInfo  *  datInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgMacSchSfRecp
(
Pst* pst,
RgInfSfDatInd*  datInd
)
#else
PUBLIC S16 RgMacSchSfRecp(pst, datInd)
Pst* pst;
RgInfSfDatInd*  datInd;
#endif
{

   TRC3(RgMacSchSfRecp);

   RETVALUE((*RgMacSchSfRecpIndMt[0])(pst, datInd));
}

/**
* @brief Data Indication from MAC to RLC to 
 * forward the data received for dedicated channels
*
* @details
*
*     Function : RgSchMacSfAlloc
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RguDDatIndInfo  *  datInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgSchMacSfAlloc
(
Pst* pst,
RgInfSfAlloc*   resAllocReq
)
#else
PUBLIC S16 RgSchMacSfAlloc(pst, resAllocReq)
Pst* pst;
RgInfSfAlloc*   resAllocReq;
#endif
{

   TRC3(RgSchMacSfAlloc);

   RETVALUE((*RgSchMacSfAllocReqMt[0])(pst, resAllocReq));
}

/**
* @brief Data Indication from MAC to RLC to 
 * forward the data received for dedicated channels
*
* @details
*
*     Function : RgSchMacRlsHq
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RguDDatIndInfo  *  datInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgSchMacRlsHq
(
Pst* pst,
RgInfRlsHqInfo* sfHqInfo
)
#else
PUBLIC S16 RgSchMacRlsHq(pst, sfHqInfo)
Pst* pst;
RgInfRlsHqInfo* sfHqInfo;
#endif
{

   TRC3(RgSchMacRlsHq);

   RETVALUE((*RgSchMacRlsHqReqMt[0])(pst, sfHqInfo));
}

/**
* @brief Data Indication from MAC to RLC to 
 * forward the data received for dedicated channels
*
* @details
*
*     Function : RgSchMacRlsRnti
*
*  @param[in]   Pst*  pst
*  @param[in]   SuId  suId
*  @param[in]   RguDDatIndInfo  *  datInd
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgSchMacRlsRnti
(
Pst* pst,
RgInfRlsRnti*   rlsRnti
)
#else
PUBLIC S16 RgSchMacRlsRnti(pst, rlsRnti)
Pst* pst;
RgInfRlsRnti*   rlsRnti;
#endif
{

   TRC3(RgSchMacRlsRnti);

   RETVALUE((*RgSchMacRlsRntiReqMt[0])(pst, rlsRnti));
}

/**
* @brief Request from SCH to MAC to register the GBR LCG of a UE
*
* @details
*
*     Function : RgSchMacLcgReg
*
*  @param[in]   Pst*           pst
*  @param[in]   RgInfLcgRegReq *lcgRegReq
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgSchMacLcgReg
(
Pst*                pst,
RgInfLcgRegReq      *lcgRegReq
)
#else
PUBLIC S16 RgSchMacLcgReg(pst, lcgRegReq)
Pst*                pst;
RgInfLcgRegReq       *lcgRegReq;
#endif
{

   TRC3(RgSchMacLcgReg);

   RETVALUE((*RgSchMacLcgRegReqMt[0])(pst, lcgRegReq));
} /* end of RgSchMacLcgReg */

#ifdef LTEMAC_SPS
/**
* @brief Data Indication from MAC to SCH to 
 * indicate the arrival of the data on SPS logical channels
*
* @details
*
*     Function : RgMacSchSpsRel
*
*  @param[in]   Pst*  pst
*  @param[in]   RgInfSpsRelInfo* relInfo
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgMacSchSpsRel
(
Pst*                pst,
RgInfSpsRelInfo*    relInfo
)
#else
PUBLIC S16 RgMacSchSpsRel(pst, relInfo)
Pst*                pst;
RgInfSpsRelInfo*    relInfo;
#endif
{

   TRC3(RgMacSchS);

   RETVALUE((*RgMacSchSpsRelIndMt[0])(pst, relInfo));
} /* end of RgMacSchSpsRel */

/**
* @brief Request from SCH to MAC to register the SPS logical channels of a UE
*
* @details
*
*     Function : RgSchMacSpsLcReg
*
*  @param[in]   Pst*           pst
*  @param[in]   RgInfSpsLcInfo *lcInfo
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgSchMacSpsLcReg
(
Pst*                pst,
RgInfSpsLcInfo      *lcInfo
)
#else
PUBLIC S16 RgSchMacSpsLcReg(pst, lcInfo)
Pst*                pst;
RgInfSpsLcInfo      *lcInfo;
#endif
{

   TRC3(RgSchMacSpsLcReg);

   RETVALUE((*RgSchMacSpsLcRegReqMt[0])(pst, lcInfo));
} /* end of RgSchMacSpsLcReg */



/**
* @brief Request from SCH to MAC to reset UL SPS params
*
* @details
*
*     Function : RgSchMacUlSpsReset
*
*  @param[in]   Pst*           pst
*  @param[in]   RgInfUlSpsReset *lcInfo
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgSchMacUlSpsReset
(
Pst*                pst,
RgInfUlSpsReset      *ulSpsResetInfo
)
#else
PUBLIC S16 RgSchMacUlSpsReset(pst, ulSpsResetInfo)
Pst*                pst;
RgInfUlSpsReset      *ulSpsResetInfo;
#endif
{

   TRC3(RgSchMacUlSpsReset);

   RETVALUE((*RgSchMacUlSpsResetMt[0])(pst, ulSpsResetInfo));
} /* end of RgSchMacUlSpsReset */



/**
* @brief Request from SCH to MAC to deregister the SPS logical channels of a UE
*
* @details
*
*     Function : RgSchMacSpsLcDereg
*
*  @param[in]   Pst*           pst
*  @param[in]   RgInfSpsLcInfo *lcInfo
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgSchMacSpsLcDereg
(
Pst*                pst,
CmLteCellId         cellId,
CmLteRnti           crnti
)
#else
PUBLIC S16 RgSchMacSpsLcDereg(pst, cellId, crnti)
Pst*                pst;
CmLteCellId         cellId;
CmLteRnti           crnti;
#endif
{

   TRC3(RgSchMacSpsLcDereg);

   RETVALUE((*RgSchMacSpsLcDeregReqMt[0])(pst, cellId, crnti));
} /* end of RgSchMacSpsLcDereg */

#endif /* LTEMAC_SPS */
#ifdef LTE_L2_MEAS
/**
* @brief Request from SCH to MAC for L2 Measurement
*
* @details
*
*     Function : RgSchMacL2Meas
*
*  @param[in]   Pst*           pst
*  @param[in]   RgInfL2MeasReq *measInfo
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgSchMacL2Meas
(
Pst*                pst,
RgInfL2MeasReq      *measInfo
)
#else
PUBLIC S16 RgSchMacL2Meas(pst, measInfo)
Pst*                pst;
RgInfL2MeasReq      *measInfo;
#endif
{

   TRC3(RgSchMacL2Meas);

   RETVALUE((*RgSchMacL2MeasReqMt[0])(pst, measInfo));
} /* end of RgSchMacL2Meas */

/**
* @brief Request from SCH to MAC for Stopping L2 Measurement
*
* @details
*
*     Function : RgSchMacL2MeasStop
*     
*  @param[in]   Pst*           pst
*  @param[in]   RgInfL2MeasReq *measInfo
*  @return   S16
*      -# ROK
**/   
#ifdef ANSI
PUBLIC S16 RgSchMacL2MeasStop
(       
Pst*                pst,
RgInfL2MeasStopReq *measInfo
)     
#else 
PUBLIC S16 RgSchMacL2MeasStop(pst, measInfo)
Pst*                pst;
RgInfL2MeasStopReq *measInfo;
#endif
{  
     
   TRC3(RgSchMacL2MeasStop);
        
   RETVALUE((*RgSchMacL2MeasStopReqMt[0])(pst, measInfo));
} /* end of RgSchMacL2Meas */

/**
* @brief Request from SCH to MAC for Sending L2 Measurement
*
* @details
*
*     Function : RgSchMacL2MeasSend
*     
*  @param[in]   Pst*           pst
*  @param[in]   RgInfL2MeasReq *measInfo
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgSchMacL2MeasSend
(
Pst*                pst,
RgInfL2MeasSndReq *measInfo
)
#else
PUBLIC S16 RgSchMacL2MeasSend(pst, measInfo)
Pst*                pst;
RgInfL2MeasSndReq *measInfo;
#endif
{

   TRC3(RgSchMacL2MeasSend);

   RETVALUE((*RgSchMacL2MeasSendReqMt[0])(pst, measInfo));
} /* end of RgSchMacL2MeasSend */

/**
* @brief Request from MAC to SCH for L2 Measurement
*
* @details
*
*     Function : RgMacSchL2Meas
*
*  @param[in]   Pst*           pst
*  @param[in]   RgInfL2MeasCfm *measInfo
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgMacSchL2Meas
(
Pst*                pst,
RgInfL2MeasCfm      *measInfo
)
#else
PUBLIC S16 RgMacSchL2Meas(pst, measInfo)
Pst*                pst;
RgInfL2MeasCfm      *measInfo;
#endif
{

   TRC3(RgMacSchL2Meas);

   RETVALUE((*RgMacSchL2MeasCfmMt[0])(pst, measInfo));
} /* end of RgSchMacL2Meas */
/**
* @brief Request from MAC to SCH for L2 Measurement
*  
* @details
*
*     Function : RgMacSchL2MeasStop
*
*  @param[in]   Pst*           pst
*  @param[in]   RgInfL2MeasCfm *measInfo
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 RgMacSchL2MeasStop
(
Pst*                pst,   
RgInfL2MeasCfm      *measInfo
)
#else
PUBLIC S16 RgMacSchL2MeasStop(pst, measInfo)
Pst*                pst;
RgInfL2MeasCfm      *measInfo;
#endif
{
    
   TRC3(RgMacSchL2MeasStop);

   RETVALUE((*RgMacSchL2MeasStopCfmMt[0])(pst, measInfo));
} /* end of RgSchMacL2MeasStop*/
#endif/*LTE_L2_MEAS*/
/**********************************************************************
 
         End of file:     gk_sch_pt.c@@/main/2 - Sat Jul 30 02:21:46 2011
 
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
/main/1      ---     rr                1. LTE MAC 2.1 release
/main/1     rg008.201  rsharon         2. Added for SPS support.
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1           
$SID$        ---        rt  1. LTE MAC 4.1 release
*********************************************************************91*/

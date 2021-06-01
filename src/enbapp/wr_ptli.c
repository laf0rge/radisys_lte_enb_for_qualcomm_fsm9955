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

     Name:     EnbApp Layer

     Type:     C source file

     Desc:     C Source code for Upper Interface procedures

     File:     wr_ptui.c

     Sid:      fc_ptli.c@@/main/TeNB_Main_BR/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Mon Dec 29 16:48:03 2014

     Prg:      rb

*********************************************************************21*/

/* header include files (.h)                              --*/
#include "envopt.h"        /* environment options         --*/
#include "envdep.h"        /* environment dependent       --*/
#include "envind.h"        /* environment independent     --*/
#include "gen.h"           /* general                     --*/
#include "ssi.h"           /* system services             --*/
#include "cm5.h"           /* common timer library        --*/
#include "cm_hash.h"       /* common hash library         --*/
#include "cm_llist.h"      /* common linked list library  --*/
#include "cm_inet.h"       /* common socket library       --*/
#include "cm_tpt.h"        /* common transport library    --*/
#include "cm_tkns.h"       /* common tokens               --*/
#include "cm_mblk.h"       /* Common memory allocation      */
#include "cm_inet.h"       /* Common socket library         */
#include "nlu.h"           /* SON module defines */
#include "rmu.h"           /* RMU interface structures      */
#include "gen.x"           /* general                     --*/
#include "cm_lte.x"
#include "cm_lte_rrm.x"
#include "rmu.x"           /* RMU interface structures      */
#include "nlu.x"           /* SON module structures */

#define WR_MAX_UIRMU_SEL   5
#define WR_MAX_NLU_SEL     3

/************************************************************************
                              Prototypes
************************************************************************/

PUBLIC RmuFsmStatusIndFPtr wrLiRmuFsmStatusIndMt[WR_MAX_UIRMU_SEL] =
{
   cmPkRmuFsmStatusInd,      /* 0 - loosely coupled */
#ifdef RMU_LWLC
   cmPkLwLcRmuFsmStatusInd,  /* 1 - LWLC */
#endif /*RMU_LWLC*/
#ifdef RMU_NPLC
   cmNoPkRmuFsmStatusInd,    /* 2 - No packing loosely coupled*/
#endif /*RMU_NPLC*/
#ifdef RM_INTF
#ifdef RMU_ACC
   DmUiRmuFsmStatusInd,      /* 3 - For ACC Test*/
#else 
#ifdef RMU_TC
   RmUiRmuFsmStatusInd       /* 3 - tightly coupled */
#endif
#endif      
#endif /*RMU_INTF */
};

PUBLIC RmuMmeOvldStartIndFPtr wrLiRmuMmeOverloadStartIndMt[WR_MAX_UIRMU_SEL] =
{
   cmPkRmuMmeOverloadStartInd,      /* 0 - loosely coupled */
#ifdef RMU_LWLC
   cmPkLwLcRmuMmeOverloadStartInd,  /* 1 - LWLC */
#endif /*RMU_LWLC*/
#ifdef RMU_NPLC
   cmNoPkRmuMmeOverloadStartInd,    /* 2 - No packing loosely coupled*/
#endif /*RMU_NPLC*/
#ifdef RM_INTF
#ifdef RMU_ACC
   DmUiRmuMmeOverloadStartInd,      /* 3 - For ACC Test*/
#else 
#ifdef RMU_TC
   RmUiRmuMmeOverloadStartInd       /* 3 - tightly coupled */
#endif
#endif      
#endif /*RMU_INTF */
};


PUBLIC RmuMmeOvldStopIndFPtr wrLiRmuMmeOverloadStopIndMt[WR_MAX_UIRMU_SEL] =
{
   cmPkRmuMmeOverloadStopInd,      /* 0 - loosely coupled */
#ifdef RMU_LWLC             
   cmPkLwLcRmuMmeOverloadStopInd,  /* 1 - LWLC */
#endif /*RMU_LWLC*/
#ifdef RMU_NPLC
   cmNoPkRmuMmeOverloadStopInd,    /* 2 - No packing loosely coupled (NPLC)*/
#endif /*RMU_NPLC*/
#ifdef RM_INTF
#ifdef RMU_ACC
   DmUiRmuMmeOverloadStopInd,     /* 3 - For ACC Test*/
#else
#ifdef RMU_TC
   RmUiRmuMmeOverloadStopInd       /*3 - tightly coupled */
#endif      
#endif      
#endif /*RMU_INTF */
};


PUBLIC RmuUeAdmitReqFPtr wrLiRmuUeAdmitReqMt[WR_MAX_UIRMU_SEL] =
{
   cmPkRmuUeAdmitReq,        /* 0 - loosely coupled */
#ifdef RMU_LWLC
   cmPkLwLcRmuUeAdmitReq,    /* 1 - LWLC */
#endif /*RMU_LWLC*/
#ifdef RMU_NPLC
   cmNoPkRmuUeAdmitReq,      /* 2 - NPLC */
#endif /*RMU_NPLC*/
#ifdef RM_INTF
#ifdef RMU_ACC
   DmUiRmuUeAdmitReq,       /* 3 - For ACC Test*/
#else
#ifdef RMU_TC
   RmUiRmuUeAdmitReq         /* 3 - tightly coupled */
#endif      
#endif      
#endif /*RMU_INTF */
};

/*RRM_SP3*/
PUBLIC RmuUeInactIndFPtr wrLiRmuUeInactIndMt[WR_MAX_UIRMU_SEL] =
{
   cmPkRmuUeInactInd,        /* 0 - loosely coupled */
#ifdef RMU_LWLC
   cmPkLwLcRmuUeInactInd,    /* 1 - LWLC */
#endif /*RMU_LWLC*/
#ifdef RMU_NPLC
   cmNoPkRmuUeInactInd,      /* 2 - NPLC */
#endif /*RMU_NPLC*/
#ifdef RM_INTF
#ifdef RMU_ACC
   DmUiRmuUeInactInd,       /* 3 - For ACC Test*/
#else
#ifdef RMU_TC
   RmUiRmuUeInactInd         /* 3 - tightly coupled */
#endif      
#endif      
#endif /*RMU_INTF */
};


PUBLIC RmuUeHoReqFPtr wrLiRmuUeHoReqMt[WR_MAX_UIRMU_SEL] =
{
   cmPkRmuUeHoReq,           /* 0 - loosely coupled */
#ifdef RMU_LWLC
   cmPkLwLcRmuUeHoReq,       /* 1 - LWLC */
#endif /*RMU_LWLC*/
#ifdef RMU_NPLC
   cmNoPkRmuUeHoReq,         /* 2 - NPLC */
#endif /*RMU_NPLC*/
#ifdef RM_INTF
#ifdef RMU_ACC
   DmUiRmuUeHoReq,           /* 3 - For ACC Test*/
#else
#ifdef RMU_TC
   RmUiRmuUeHoReq            /* 3 - tightly coupled */
#endif      
#endif      
#endif /*RMU_INTF */
};


PUBLIC RmuUeRelReqFPtr wrLiRmuUeRelReqMt[WR_MAX_UIRMU_SEL] =
{
   cmPkRmuUeRelReq,          /* 0 - loosely coupled */
#ifdef RMU_LWLC
   cmPkLwLcRmuUeRelReq,      /* 1 - LWLC */
#endif /*RMU_LWLC*/
#ifdef RMU_NPLC
   cmNoPkRmuUeRelReq,        /* 2 - NPLC */
#endif /*RMU_NPLC*/
#ifdef RM_INTF
#ifdef RMU_ACC
   DmUiRmuUeRelReq,          /* 3 - For ACC Test*/
#else
#ifdef RMU_TC
   RmUiRmuUeRelReq           /* 3 - tightly coupled */
#endif      
#endif      
#endif /*RMU_INTF */
};


PUBLIC RmuUeRecfgReqFPtr wrLiRmuUeRecfgReqMt[WR_MAX_UIRMU_SEL] =
{
   cmPkRmuUeRecfgReq,        /* 0 - loosely coupled */
#ifdef RMU_LWLC
   cmPkLwLcRmuUeRecfgReq,    /* 1 - LWLC */
#endif /*RMU_LWLC*/
#ifdef RMU_NPLC
   cmNoPkRmuUeRecfgReq,      /* 2 - NPLC */
#endif /*RMU_NPLC*/
#ifdef RM_INTF
#ifdef RMU_ACC
   DmUiRmuUeRecfgReq,        /* 3 - For ACC Test*/
#else
#ifdef RMU_TC
   RmUiRmuUeRecfgReq         /* 3 - tightly coupled */
#endif      
#endif      
#endif /*RMU_INTF */
};

/* SPS changes starts */
PUBLIC RmuUeSpsDisIndFPtr wrLiRmuUeSpsDisIndMt[WR_MAX_UIRMU_SEL] =
{
   cmPkRmuUeSpsDisInd,        /* 0 - loosely coupled */
#ifdef RMU_LWLC
   cmPkLwLcRmuUeSpsDisInd,    /* 1 - LWLC */
#endif /*RMU_LWLC*/
#ifdef RMU_NPLC
   cmNoPkRmuUeSpsDisInd,      /* 2 - NPLC */
#endif /*RMU_NPLC*/
#ifdef RM_INTF
#ifdef RMU_ACC
   DmUiRmuUeSpsDisInd,        /* 3 - For ACC Test*/
#else
#ifdef RMU_TC
   RmUiRmuUeSpsDisInd         /* 3 - tightly coupled */
#endif      
#endif      
#endif /*RMU_INTF */
};
/* SPS changes ends */

PUBLIC RmuBndReqFPtr wrLiRmuBndReqMt[WR_MAX_UIRMU_SEL] =
{
   cmPkRmuBndReq,        /* 0 - loosely coupled */
#ifdef RMU_LWLC
   cmPkLwLcRmuBndReq,    /* 1 - LWLC */
#endif /*RMU_LWLC*/
#ifdef RMU_NPLC
   cmNoPkRmuBndReq,      /* 2 - NPLC */
#endif /*RMU_NPLC*/
#ifdef RM_INTF
#ifdef RMU_ACC
   DmUiRmuBndReq,        /* 3 - For ACC Test*/
#else
#ifdef RMU_TC
   RmUiRmuBndReq         /* 3 - tightly coupled */
#endif      
#endif      
#endif /*RMU_INTF */
};

PUBLIC NluBndReq   wrLiNluBndReqMt[WR_MAX_NLU_SEL]=
{
#ifdef NLU_LC
   cmPkNluBndReq,           /* 0 - loosely coupled */
#else
   ptUiNluBndReq,
#endif /* NLU_LC */
#ifdef NLU_TC
   NlUiNluBndReq,           /* 1 - Tightly coupled */
#else
   ptUiNluBndReq,
#endif /* NLU_TC */
#ifdef NLU_LWLC
   cmPkNluBndReq,           /* 2 - lightweight loosely coupled */
#else
   ptUiNluBndReq,
#endif /* NLU_LWLC */
};

PUBLIC NluNeighborReq   wrLiNluNeighborReqMt[WR_MAX_NLU_SEL]=
{
#ifdef NLU_LC
   cmPkNluNeighborReq,           /* 0 - loosely coupled */
#else
   ptUiNluNeighborReq,
#endif /* NLU_LC */
#ifdef NLU_TC
   NlUiNluNeighborReq,           /* 1 - Tightly coupled */
#else
   ptUiNluNeighborReq,
#endif /* NLU_TC */
#ifdef NLU_LWLC
   cmPkNluNeighborReq,           /* 2 - lightweight loosely coupled */
#else
   ptUiNluNeighborReq,
#endif /* NLU_LWLC */
};

PUBLIC NluPciModCfm   wrLiNluPciModIndCfmMt[WR_MAX_NLU_SEL]=
{
#ifdef NLU_LC
   cmPkNluPciModIndCfm,           /* 0 - loosely coupled */
#else
   ptUiNluPciModIndCfm,
#endif /* NLU_LC */
#ifdef NLU_TC
   NlUiNluPciModIndCfm,           /* 1 - Tightly coupled */
#else
   ptUiNluPciModIndCfm,
#endif /* NLU_TC */
   cmPkNluPciModIndCfm,           /* 2 - lightweight loosely coupled */
};

#ifdef RGR_CQI_REPT
PUBLIC NluUeCqiRptInd   wrLiNluUeCqiRptIndMt[WR_MAX_NLU_SEL]=
{
#ifdef NLU_LC
   cmPkNluUeCqiReportInd,           /* 0 - loosely coupled */
#else
   ptUiNluUeCqiReportInd,
#endif /* NLU_LC */
#ifdef NLU_TC
   NlUiNluUeCqiReportInd,           /* 1 - Tightly coupled */
#else
   ptUiNluUeCqiReportInd,
#endif /* NLU_TC */
   cmPkNluUeCqiReportInd,           /* 2 - lightweight loosely coupled */
};
#endif

PUBLIC NluUeMeasRptInd   wrLiNluUeMeasRptIndMt[WR_MAX_NLU_SEL]=
{
#ifdef NLU_LC
   cmPkNluUeMeasReportInd,           /* 0 - loosely coupled */
#else
   ptUiNluUeMeasReportInd,
#endif /* NLU_LC */
#ifdef NLU_TC
   NlUiNluUeMeasReportInd,           /* 1 - Tightly coupled */
#else
   ptUiNluUeMeasReportInd,
#endif /* NLU_TC */
   cmPkNluUeMeasReportInd,           /* 2 - lightweight loosely coupled */
};

PUBLIC NluUeInd   wrLiNluUeIndMt[WR_MAX_NLU_SEL]=
{
#ifdef NLU_LC
   cmPkNluUeInd,           /* 0 - loosely coupled */
#else
   ptUiNluUeInd,
#endif /* NLU_LC */
#ifdef NLU_TC
   NlUiNluUeInd,           /* 1 - Tightly coupled */
#else
   ptUiNluUeInd,
#endif /* NLU_TC */
   cmPkNluUeInd,           /* 2 - lightweight loosely coupled */
};

/************************************************************************
/* Periodic REM scan for TPM */
PUBLIC NluPerCellSrchRsp   wrLiNluPerCellSrchRspMt[WR_MAX_NLU_SEL]=
{
#ifdef NLU_LC
   cmPkNluPerCellSrchRsp,           /* 0 - loosely coupled */
#else
   cmPkNluPerCellSrchRsp,           /* 0 - loosely coupled */
//   ptUiNluPerCellSrchRsp,
#endif /* NLU_LC */
#ifdef NLU_TC
   cmPkNluPerCellSrchRsp,           /* 0 - loosely coupled */
//   NlUiNluPerCellSrchRsp,           /* 1 - Tightly coupled */
#else
   cmPkNluPerCellSrchRsp,           /* 0 - loosely coupled */
//   ptUiNluPerCellSrchRsp,
#endif /* NLU_TC */
   cmPkNluPerCellSrchRsp,           /* 2 - lightweight loosely coupled */
};
/* Periodic REM scan for TPM End */


/************************************************************************
                       Upper Interface Functions
************************************************************************/

/*
 *
 *       Fun:   WrLiWrtFsmStatusInd
 *
 *       Desc:  This function resolves the WR_RM _MME_OVERLOAD_START_IND  primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  wr_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 WrLiRmuFsmStatusInd
(
Pst *pst,
SuId suId,
RmuFsmStatusInd *statusInd
)
#else
PUBLIC S16 WrLiRmuFsmStatusInd (pst, suId, statusInd)
Pst *pst;
SuId suId;
RmuFsmStatusInd *statusInd;
#endif
{
   TRC3(WrLiRmuFsmStatusInd)

   (*wrLiRmuFsmStatusIndMt[pst->selector])
      (pst, suId, statusInd);

   RETVALUE(ROK);
} /* WrLiRmuFsmStatusInd */


/*
 *
 *       Fun:   WrLiWrtMmeOverloadStartInd
 *
 *       Desc:  This function resolves the WR_RM _MME_OVERLOAD_START_IND  primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  wr_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 WrLiRmuMmeOverloadStartInd
(
Pst *pst,
SuId suId,
RmuMmeOvldStartInd *startInfo
)
#else
PUBLIC S16 WrLiRmuMmeOverloadStartInd (pst, suId, startInfo)
Pst *pst;
SuId suId;
RmuMmeOvldStartInd *startInfo;
#endif
{
   TRC3(WrLiRmuMmeOverloadStartInd)

   (*wrLiRmuMmeOverloadStartIndMt[pst->selector])
      (pst, suId, startInfo);

   RETVALUE(ROK);
} /* WrLiRmuMmeOverloadStartInd */


/*
 *
 *       Fun:   WrLiWrtMmeOverloadStopInd
 *
 *       Desc:  This function resolves the WR_RM_MME_OVERLOAD_STOP_IND primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  wr_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 WrLiRmuMmeOverloadStopInd
(
Pst *pst,
SuId suId,
RmuMmeOvldStopInd *stopinfo
)
#else
PUBLIC S16 WrLiRmuMmeOverloadStopInd (pst, suId, stopinfo)
Pst *pst;
SuId suId;
RmuMmeOvldStopInd *stopinfo;
#endif
{
   TRC3(WrLiRmuMmeOverloadStopInd)

   (*wrLiRmuMmeOverloadStopIndMt[pst->selector])
      (pst, suId, stopinfo);

   RETVALUE(ROK);
} /* WrLiRmuMmeOverloadStopInd */

/*
 *
 *       Fun:   WrLiRmuUeAdmitReq
 *
 *       Desc:  This function resolves the WR_RM_UE_ADMIT_REQ primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  wr_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 WrLiRmuUeAdmitReq
(
Pst *pst,
SuId suId,
RmuUeAdmitReq *admitinfo
)
#else
PUBLIC S16 WrLiRmuUeAdmitReq (pst, suId, admitinfo)
Pst *pst;
SuId suId;
RmuUeAdmitReq *admitinfo;
#endif
{
   TRC3(WrLiRmuUeAdmitReq)

   (*wrLiRmuUeAdmitReqMt[pst->selector])
      (pst, suId, admitinfo);

   RETVALUE(ROK);
} /* WrLiRmuUeAdmitReq */

/*RRM_SP3*/
/*
 *
 *       Fun:   WrLiRmuUeInactInd
 *
 *       Desc:  This function is used to send the UE inactivity info via the 
 *              correspondig pst interface info.
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  wr_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 WrLiRmuUeInactInd  
(
Pst *pst,
SuId suId,
RmuUeInactInd   *rrmUeInactInd
)
#else
PUBLIC S16 WrLiRmuUeInactInd(pst, suId, rrmUeInactInd)
Pst *pst;
SuId suId;
RmuUeInactInd   *rrmUeInactInd;
;
#endif
{
   TRC3(WrLiRmuUeInactInd)

   (*wrLiRmuUeInactIndMt[pst->selector]) (pst, suId, rrmUeInactInd);

   RETVALUE(ROK);
} /*WrLiRmuUeInactInd*/

/*
 *
 *       Fun:   WrLiRmuUeHoReq
 *
 *       Desc:  This function resolves the WR_RM_UE_HO_REQ  primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  wr_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 WrLiRmuUeHoReq
(
Pst *pst,
SuId suId,
RmuUeHoReq *hoinfo
)
#else
PUBLIC S16 WrLiRmuUeHoReq (pst, suId, hoinfo)
Pst *pst;
SuId suId;
RmuUeHoReq *hoinfo;
#endif
{
   TRC3(WrLiRmuUeHoReq)

   (*wrLiRmuUeHoReqMt[pst->selector])
      (pst, suId, hoinfo);

   RETVALUE(ROK);
} /* WrLiRmuUeHoReq */

/*
 *
 *       Fun:   WrLiRmuUeRelReq
 *
 *       Desc:  This function resolves the WR_RM_UE_REL_REQ primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  wr_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 WrLiRmuUeRelReq
(
Pst *pst,
SuId suId,
RmuUeRelReq *relinfo
)
#else
PUBLIC S16 WrLiRmuUeRelReq (pst, suId, relinfo)
Pst *pst;
SuId suId;
RmuUeRelReq *relinfo;
#endif
{
   TRC3(WrLiRmuUeRelReq)

   (*wrLiRmuUeRelReqMt[pst->selector])
      (pst, suId, relinfo);

   RETVALUE(ROK);
} /* WrLiWrtUeRelReq */

/*
 *
 *       Fun:   WrLiRmuUeRecfgReq
 *
 *       Desc:  This function resolves the WR_RM_UE_RECONFIG_REQ primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  wr_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 WrLiRmuUeRecfgReq
(
Pst *pst,
SuId suId,
RmuUeRecfgReq *reconfiginfo
)
#else
PUBLIC S16 WrLiRmuUeRecfgReq (pst, suId, reconfiginfo)
Pst *pst;
SuId suId;
RmuUeRecfgReq *reconfiginfo;
#endif
{
   TRC3(WrLiRmuUeRecfgReq)

   (*wrLiRmuUeRecfgReqMt[pst->selector])
      (pst, suId, reconfiginfo);

   RETVALUE(ROK);
} /* WrLiRmuUeRecfgReq */

/* SPS changes starts */
/*
 *
 *       Fun:   WrLiRmuUeSpsDisInd
 *
 *       Desc:  This function resolves the WR_RM_UE_SPS_DISABLE_IND primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  wr_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 WrLiRmuUeSpsDisInd
(
Pst *pst,
SuId suId,
RmuUeSpsDisInd *ueSpsDisInd
)
#else
PUBLIC S16 WrLiRmuUeSpsDisInd (pst, suId, reconfiginfo)
Pst *pst;
SuId suId;
RmuUeSpsDisInd *ueSpsDisInd;
#endif
{
   TRC3(WrLiRmuUeSpsDisInd)

   (*wrLiRmuUeSpsDisIndMt[pst->selector])
      (pst, suId, ueSpsDisInd);

   RETVALUE(ROK);
} /* WrLiRmuUeSpsDisInd */
/* SPS changes ends */
/*
 *
 *       Fun:   WrLiRmuBndReq
 *
 *       Desc:  This function resolves the BNDREQ primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  wr_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 WrLiRmuBndReq
(
Pst *pst,
SuId suId,
SpId spId
)
#else
PUBLIC S16 WrLiRmuBndReq (pst, suId, spId)
Pst *pst;
SuId suId;
SpId spId;
#endif
{
   TRC3(WrLiRmuBndReq)

   (*wrLiRmuBndReqMt[pst->selector])
      (pst, suId, spId);

   RETVALUE(ROK);
} /* WrLiRmuUeRecfgReq */

/*
 *
 *       Fun:   WrLiNluBndReq
 *
 *       Desc:  This function resolves the BNDREQ primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  wr_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 WrLiNluBndReq
(
Pst *pst,
SuId suId,
SpId spId
)
#else
PUBLIC S16 WrLiNluBndReq (pst, suId, spId)
Pst *pst;
SuId suId;
SpId spId;
#endif
{
   TRC3(WrLiNluBndReq)

   (*wrLiNluBndReqMt[pst->selector])
      (pst, suId, spId);

   RETVALUE(ROK);
} /* WrLiNluBndReq */

/*
 *
 *       Fun:   WrLiNluPciModIndCfm
 *
 *       Desc:  This function is used to send PCI modification ind confirm to SON
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  wr_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 WrLiNluPciModIndCfm
(
Pst *pst,
SuId suId,
NluPciModIndCfm *pciMod
)
#else
PUBLIC S16 WrLiNluPciModIndCfm (pst, suId, pciMod)
Pst *pst;
SuId suId;
NluPciModIndCfm *pciMod;
#endif
{
   TRC3(WrLiNluPciModIndCfm)

   (*wrLiNluPciModIndCfmMt[pst->selector])
      (pst,suId, pciMod);
   RETVALUE(ROK);
}

#ifdef RGR_CQI_REPT
/* @brief This function is used to send UE CQI report toSON
 *
 * @details
 *
 *     This API is used to send UE CQI report indication to SON
 *
 * @param[in] pst      : post structure
 * @param[in] suId     : post structure
 * @param[in] cqiRpt   : UE CQI report message structure
 *
 * @return  S16
 *        -# Success : ROK
 *        -# failure : RFAILED
 */
#ifdef ANSI
PUBLIC S16 WrLiNluUeCqiRptInd
(
Pst           *pst,
SuId           suId,
NluUeCqiRpt   *cqiRpt
)
#else
PUBLIC S16 WrLiNluUeCqiRptInd (pst, suId, cqiRpt)
Pst           *pst;
SuId           suId;
NluUeCqiRpt   *cqiRpt;
#endif
{
   S16 ret = ROK;
   ret = (*wrLiNluUeCqiRptIndMt[pst->selector])
      (pst,suId, cqiRpt);
   RETVALUE(ret);
}
#endif

/* @brief This function is used to send UE measurement report to SON
 *
 * @details
 *
 *     This API is used to send UE measurement report indication to SON
 *
 * @param[in] pst      : post structure
 * @param[in] suId     : post structure
 * @param[in] measRpt   : UE Measurement report message structure
 *
 * @return  S16
 *        -# Success : ROK
 *        -# failure : RFAILED
 */
#ifdef ANSI
PUBLIC S16 WrLiNluUeMeasRptInd
(
Pst           *pst,
SuId           suId,
NluUeMeasRpt   *measRpt
)
#else
PUBLIC S16 WrLiNluUeMeasRptInd (pst, suId, measRpt)
Pst           *pst;
SuId           suId;
NluUeMeasRpt   *measRpt;
#endif
{
   S16 ret = ROK;
   ret = (*wrLiNluUeMeasRptIndMt[pst->selector])
      (pst,suId, measRpt);
   RETVALUE(ret);
}

/* @brief This function is used to send UE Indication toSON
 *
 * @details
 *
 *     This API is used to send UE indication to SON
 *
 * @param[in] pst      : post structure
 * @param[in] suId     : post structure
 * @param[in] cqiRpt   : UE information message structure
 *
 * @return  S16
 *        -# Success : ROK
 *        -# failure : RFAILED
 */
#ifdef ANSI
PUBLIC S16 WrLiNluUeInd
(
Pst           *pst,
SuId           suId,
NluUeInfo     *ueInfo
)
#else
PUBLIC S16 WrLiNluUeInd (pst, suId, ueInfo)
Pst           *pst;
SuId           suId;
NluUeInfo     *ueInfo;
#endif
{
   S16 ret = ROK;
   ret = (*wrLiNluUeIndMt[pst->selector])
      (pst,suId, ueInfo);
   RETVALUE(ret);
}


/*
 *
 *       Fun:   wrIfmSonNeighborReq
 *
 *       Desc:  This function is used to report neighbor information to SON
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  wr_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 WrLiNluNeighborReq
(
Pst *pst,
SuId suId,
NluNeighborReport *nghRpt
)
#else
PUBLIC S16 WrLiNluAddNeighborReq (pst, suId, nghRpt)
Pst *pst;
SuId suId;
NluNeighborReport *nghRpt;
#endif
{
   TRC3(WrLiNluNeighborReq)

   (*wrLiNluNeighborReqMt[pst->selector])
      (pst,suId, nghRpt);

   RETVALUE(ROK);
}

/* Periodic REM scan fro TPM */
/*
 *
 *       Fun:   WrLiNluPerCellSrchRsp
 *
 *       Desc:  This function is used to send Periodic Cell search Information to SON
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  wr_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 WrLiNluPerCellSrchRsp
(
Pst *pst,
SuId suId,
NluPeriodicRemCellSearchRsp *cellSrchRsp
)
#else
PUBLIC S16 WrLiNluPerCellSrchRsp (pst, suId, cellSrchRsp)
Pst *pst;
SuId suId;
NluPeriodicRemCellSearchRsp cellSrchRsp;
#endif
{
   TRC3(WrLiNluPerCellSrchRsp)

   (*wrLiNluPerCellSrchRspMt[pst->selector])
      (pst,suId, cellSrchRsp);
   RETVALUE(ROK);
}
/* Periodic REM scan for TPM End */


/**********************************************************************

         End of file:     fc_ptli.c@@/main/TeNB_Main_BR/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Mon Dec 29 16:48:03 2014

**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------

*********************************************************************91*/





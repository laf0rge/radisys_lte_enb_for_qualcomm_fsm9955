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



/**********************************************************************

     Name:     LTE-RRC Layer 
  
     Type:     C Source file
  
     Desc:     C source code for LTE RRC's Upper interface outgoing 
               primitives towards LTE RRC user(NX).

     File:     nh_ptui.c

     Sid:      ni_ptui.c@@/main/3 - Fri Jul  1 01:13:21 2011

     Prg:      vk 

**********************************************************************/

/* header include files -- defines (.h) */
static const char* RLOG_MODULE_NAME="RRC";
static int RLOG_MODULE_ID=512;
static int RLOG_FILE_ID=94;
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hashlist Defines */
#include "cm_lte.h"        /* Common LTE Defines */
#include "lnh.h"           /* LNH Interface defines */
#include "nhu.h"           /* NHU Interface defines */
#include "nh.h"            /* RRC defines */

/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */
#include "cm5.x"           /* timer services */
#include "cm_lib.x"        /* Common library definitions */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_hash.x"       /* Common Hashlist Definitions */
#include "cm_lte.x"        /* Common LTE includes */
#include "lnh.x"           /* LNH Interface includes */
#include "nhu.x"           /* NHU Interface includes */
#include "nh.x"            /* RRC includes */

/* local defines */
#define MAXNHUI   3        /* Size of Matrix used for LC/TC */

/* local typedefs */

/* local externs */

/* forward references */



#if (!(defined(LCNHUINHU) && defined(LWLCNHUINHU)))
#define PTNHUINHU    1        /* Portable NHU Interface */
#else /*(!(defined(LCNHUINHU) && defined(LWLCNHUINHU))) */
#ifndef NX                    /* NX - RRC Service User */
#define PTNHUINHU    1        /* Portable NHU Interface */
#endif /* NX */
#endif /*(!(defined(LCNHUINHU) && defined(LWLCNHUINHU))) */

/* Prototypes of Portable Functions */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef PTNHUINHU
EXTERN S16 PtUiNhuBndCfm ARGS((Pst *pst, SuId suId, U8 status));
EXTERN S16 PtUiNhuDatCfm ARGS((Pst *pst, SuId suId, 
                                 NhuDatCfmSdus  *nhuDataCfmSdus));
EXTERN S16 PtUiNhuStaInd ARGS((Pst *pst, SuId suId, 
                                 NhuStaIndSdus  *nhuStatusIndSdus));            
EXTERN S16 PtUiNhuDatInd ARGS((Pst *pst, SuId suId, 
                                       NhuDatIndSdus *nhuDataIndSdus));
EXTERN S16 PtUiNhuErrInd ARGS((Pst *pst, SuId suId, 
                                       NhuErrIndSdus *nhuErrorIndSdus));
EXTERN S16 PtUiNhuCfgCfm ARGS((Pst *pst, SuId suId, 
                                       NhuCfgCfmSdus *nhuConfigCfmSdus));
EXTERN S16 PtUiNhuCellCfgCfm ARGS((Pst *pst, SuId suId, 
                                       NhuCellCfgCfmSdus *nhuCfgCellCfmSdus));
EXTERN S16 PtUiNhuPdcpSduStaCfm ARGS((Pst *pst, SuId suId, 
                                      NhuPdcpSduStaCfm *nhuPdcpSduStatCfm));
EXTERN S16 PtUiNhuEncodeCfm ARGS((Pst *pst, SuId suId, 
                                NhuEncCfmSdus *nhuEncodeCfmSdus));
EXTERN S16 PtUiNhuDecodeCfm ARGS((Pst *pst, SuId suId, 
                                NhuDecodeSduCfm *nhuDecSduCfm));
EXTERN S16 PtUiNhuCountCfm ARGS((Pst *pst, SuId suId, 
                                NhuDRBCountInfoLst *nhuDRBCntInfoLst));
EXTERN S16 PtUiNhuPdcpDatResumeCfm ARGS((Pst *pst, SuId suId, 
                                      NhuPdcpDatResumeCfm *nhuPdcpDatResumeCfm));

#endif /* PTNHUINHU */

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*
   The following matrices define the mapping between the primitives
   called by the service user of RRC layer and the corresponding
   primitives of the RRC layer.

   The parameter MAXNHUI defines the maximum number of service users on
   top of RRC. There is an array of functions per primitive invoked by 
   RRC. Every array is MAXNHUI long (that is, there are as many functions 
   as the number of service users).

   The dispatching is performed by the configurable variable: Selector.
   The selector is configured on a per SAP basis. The selectors are:

   NH_SEL_LC - Loosely coupled
   NH_SEL_LWLC - Light-weight loosely coupled
   NH_SEL_TC_RX - Tightly coupled (#define RX )
*/
/* Bind Confirm Primitive */
PRIVATE CONSTANT NhuBndCfm NhUiNhuBndCfmMt [MAXNHUI] =
{
#ifdef LCNHUINHU
   cmPkNhuBndCfm,       /* 0 - loosely coupled */
#else /* LCNHUINHU */
   PtUiNhuBndCfm,       /* 0 - loosely coupled, portable */
#endif /* LCNHUINHU */
#ifdef NX
   NxLiNhuBndCfm,       /* 1 - tight coupled, RX */
#else /* NX */
   PtUiNhuBndCfm,       /* 1 - tight coupled, portable */
#endif /* NX */
#ifdef LWLCNHUINHU
   cmPkNhuBndCfm        /* 2 - light-weight loosely coupled */
#else /* LWLCNHUINHU */
   PtUiNhuBndCfm        /* 2 - light-weight loosely coupled, portable */
#endif /* LWLCNHUINHU */
};

/* Data Indication Primitive */
PRIVATE CONSTANT NhuDatInd NhUiNhuDatIndMt [MAXNHUI] =
{
#ifdef LCNHUINHU
   cmPkNhuDatInd,       /* 0 - loosely coupled */
#else /* LCNHUINHU */
   PtUiNhuDatInd,       /* 0 - loosely coupled, portable */
#endif /* LCNHUINHU */
#ifdef NX
   NxLiNhuDatInd,       /* 1 - tight coupled, RX */
#else /* NX */
   PtUiNhuDatInd,       /* 1 - tight coupled, portable */
#endif /* NX */
#ifdef LWLCNHUINHU
   cmPkNhuDatInd        /* 2 - light-weight loosely coupled */
#else /* LWLCNHUINHU */
   PtUiNhuDatInd        /* 2 - light-weight loosely coupled, portable */
#endif /* LWLCNHUINHU */
};

/* Error Indication Primitive */
PRIVATE CONSTANT NhuErrInd NhUiNhuErrIndMt [MAXNHUI] =
{
#ifdef LCNHUINHU
   cmPkNhuErrInd,       /* 0 - loosely coupled */
#else /* LCNHUINHU */
   PtUiNhuErrInd,       /* 0 - loosely coupled, portable */
#endif /* LCNHUINHU */
#ifdef NX
   NxLiNhuErrInd,       /* 1 - tight coupled, RX */
#else /* NX */
   PtUiNhuErrInd,       /* 1 - tight coupled, portable */
#endif /* NX */
#ifdef LWLCNHUINHU
   cmPkNhuErrInd        /* 2 - light-weight loosely coupled */
#else /* LWLCNHUINHU */
   PtUiNhuErrInd        /* 2 - light-weight loosely coupled, portable */
#endif /* LWLCNHUINHU */
};

/* Error Indication Primitive */
PRIVATE CONSTANT NhuDatCfm NhUiNhuDatCfmMt [MAXNHUI] =
{
#ifdef LCNHUINHU
   cmPkNhuDatCfm,       /* 0 - loosely coupled */
#else /* LCNHUINHU */
   PtUiNhuDatCfm,       /* 0 - loosely coupled, portable */
#endif /* LCNHUINHU */
#ifdef NX
   NxLiNhuDatCfm,       /* 1 - tight coupled, RX */
#else /* NX */
   PtUiNhuDatCfm,       /* 1 - tight coupled, portable */
#endif /* NX */
#ifdef LWLCNHUINHU
   cmPkNhuDatCfm        /* 2 - light-weight loosely coupled */
#else /* LWLCNHUINHU */
   PtUiNhuDatCfm        /* 2 - light-weight loosely coupled, portable */
#endif /* LWLCNHUINHU */
};

/* Status Indication  Primitive */
PRIVATE CONSTANT NhuStaInd NhUiNhuStaIndMt [MAXNHUI] =
{
#ifdef LCNHUINHU
   cmPkNhuStaInd,       /* 0 - loosely coupled */
#else /* LCNHUINHU */
   PtUiNhuStaInd,       /* 0 - loosely coupled, portable */
#endif /* LCNHUINHU */
#ifdef NX
   NxLiNhuStaInd,       /* 1 - tight coupled, RX */
#else /* NX */
   PtUiNhuStaInd,       /* 1 - tight coupled, portable */
#endif /* NX */
#ifdef LWLCNHUINHU
   cmPkNhuStaInd        /* 2 - light-weight loosely coupled */
#else /* LWLCNHUINHU */
   PtUiNhuStaInd        /* 2 - light-weight loosely coupled, portable */
#endif /* LWLCNHUINHU */
};


/* Configuration Confirm Primitive */
PRIVATE CONSTANT NhuCfgCfm NhUiNhuCfgCfmMt [MAXNHUI] =
{
#ifdef LCNHUINHU
   cmPkNhuCfgCfm,       /* 0 - loosely coupled */
#else /* LCNHUINHU */
   PtUiNhuCfgCfm,       /* 0 - loosely coupled, portable */
#endif /* LCNHUINHU */
#ifdef NX
   NxLiNhuCfgCfm,       /* 1 - tight coupled, RX */
#else /* NX */
   PtUiNhuCfgCfm,       /* 1 - tight coupled, portable */
#endif /* NX */
#ifdef LWLCNHUINHU
   cmPkNhuCfgCfm        /* 2 - light-weight loosely coupled */
#else /* LWLCNHUINHU */
   PtUiNhuCfgCfm        /* 2 - light-weight loosely coupled, portable */
#endif /* LWLCNHUINHU */
};

/* Cell Configuration Primitive */
PRIVATE CONSTANT NhuCellCfgCfm NhUiNhuCellCfgCfmMt [MAXNHUI] =
{
#ifdef LCNHUINHU
   cmPkNhuCellCfgCfm,       /* 0 - loosely coupled */
#else /* LCNHUINHU */
   PtUiNhuCellCfgCfm,       /* 0 - loosely coupled, portable */
#endif /* LCNHUINHU */
#ifdef NX
   NxLiNhuCellCfgCfm,       /* 1 - tight coupled, RX */
#else /* NX */
   PtUiNhuCellCfgCfm,       /* 1 - tight coupled, portable */
#endif /* NX */
#ifdef LWLCNHUINHU
   cmPkNhuCellCfgCfm        /* 2 - light-weight loosely coupled */
#else /* LWLCNHUINHU */
   PtUiNhuCellCfgCfm        /* 2 - light-weight loosely coupled, portable */
#endif /* LWLCNHUINHU */
};

/* Decode confirm primitive */
PRIVATE CONSTANT NhuDecodeCfm NhUiNhuDecodeCfmMt [MAXNHUI] =
{
#ifdef LCNHUINHU
   cmPkNhuDecodeCfm,       /* 0 - loosely coupled */
#else /* LCNHUINHU */
   PtUiNhuDecodeCfm,       /* 0 - loosely coupled, portable */
#endif /* LCNHUINHU */
#ifdef NX
   NxLiNhuDecodeCfm,       /* 1 - tight coupled, RX */
#else /* NX */
   PtUiNhuDecodeCfm,       /* 1 - tight coupled, portable */
#endif /* NX */
#ifdef LWLCNHUINHU
   cmPkNhuDecodeCfm        /* 2 - light-weight loosely coupled */
#else /* LWLCNHUINHU */
   PtUiNhuDecodeCfm        /* 2 - light-weight loosely coupled, portable */
#endif /* LWLCNHUINHU */
};

/* Encode confirm primitive */
PRIVATE CONSTANT NhuEncCfm NhUiNhuEncodeCfmMt [MAXNHUI] =
{
#ifdef LCNHUINHU
   cmPkNhuEncodeCfm,       /* 0 - loosely coupled */
#else /* LCNHUINHU */
   PtUiNhuEncodeCfm,       /* 0 - loosely coupled, portable */
#endif /* LCNHUINHU */
#ifdef NX
   NxLiNhuEncodeCfm,       /* 1 - tight coupled, RX */
#else /* NX */
   PtUiNhuEncodeCfm,       /* 1 - tight coupled, portable */
#endif /* NX */
#ifdef LWLCNHUINHU
   cmPkNhuEncodeCfm        /* 2 - light-weight loosely coupled */
#else /* LWLCNHUINHU */
   PtUiNhuEncodeCfm        /* 2 - light-weight loosely coupled, portable */
#endif /* LWLCNHUINHU */
};

/* Count Confirm primitive */
PRIVATE CONSTANT NhuCountCfm NhUiNhuCountCfmMt [MAXNHUI] =
{
#ifdef LCNHUINHU
   cmPkNhuCountCfm,       /* 0 - loosely coupled */
#else /* LCNHUINHU */
   PtUiNhuCountCfm,       /* 0 - loosely coupled, portable */
#endif /* LCNHUINHU */
#ifdef NX
   NxLiNhuCountCfm,       /* 1 - tight coupled, RX */
#else /* NX */
   PtUiNhuCountCfm,       /* 1 - tight coupled, portable */
#endif /* NX */
#ifdef LWLCNHUINHU
   cmPkNhuCountCfm        /* 2 - light-weight loosely coupled */
#else /* LWLCNHUINHU */
   PtUiNhuCountCfm        /* 2 - light-weight loosely coupled, portable */
#endif /* LWLCNHUINHU */
};

/* PDCP SDU status confirm primitive */
PRIVATE CONSTANT NhuSduStaCfm NhUiNhuPdcpSduStaCfmMt [MAXNHUI] =
{
#ifdef LCNHUINHU
   cmPkNhuPdcpSduStaCfm,       /* 0 - loosely coupled */
#else /* LCNHUINHU */
   PtUiNhuPdcpSduStaCfm,       /* 0 - loosely coupled, portable */
#endif /* LCNHUINHU */
#ifdef NX
   NxLiNhuPdcpSduStaCfm,       /* 1 - tight coupled, RX */
#else /* NX */
   PtUiNhuPdcpSduStaCfm,       /* 1 - tight coupled, portable */
#endif /* NX */
#ifdef LWLCNHUINHU
   cmPkNhuPdcpSduStaCfm        /* 2 - light-weight loosely coupled */
#else /* LWLCNHUINHU */
   PtUiNhuPdcpSduStaCfm        /* 2 - light-weight loosely coupled, portable */
#endif /* LWLCNHUINHU */
};

/* PDCP Data Resume confirm primitive */
PRIVATE CONSTANT NhuDatResumeCfm NhUiNhuPdcpDatResumeCfmMt [MAXNHUI] =
{
#ifdef LCNHUINHU
   cmPkNhuPdcpDatResumeCfm,       /* 0 - loosely coupled */
#else /* LCNHUINHU */
   PtUiNhuPdcpDatResumeCfm,       /* 0 - loosely coupled, portable */
#endif /* LCNHUINHU */
#ifdef NX
   NxLiNhuPdcpDatResumeCfm,       /* 1 - tight coupled, RX */
#else /* NX */
   PtUiNhuPdcpDatResumeCfm,       /* 1 - tight coupled, portable */
#endif /* NX */
#ifdef LWLCNHUINHU
   cmPkNhuPdcpDatResumeCfm/* 2 - light-weight loosely coupled */
#else /* LWLCNHUINHU */
   PtUiNhuPdcpDatResumeCfm/* 2 - light-weight loosely coupled, portable */
#endif /* LWLCNHUINHU */
};
/**************************************************************************
 * LTE RRC Upper interface primitives
 *************************************************************************/
/**************************************************************************
 *
 *      Fun:   NhUiNhuBndCfm
 *
 *      Desc:  RRC's Bind Confirm to the RRC User (Upper layer).
 *
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_ptui.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 NhUiNhuBndCfm
(
Pst *pst,               /* Post structure */
SuId suId,              /* Service user identifier */
U8 status               /* Status */
)
#else /* ANSI */
PUBLIC S16 NhUiNhuBndCfm (pst, suId, status)
Pst *pst;               /* Post structure */
SuId suId;              /* Service user identifier */
U8 status;              /* Status */
#endif /* ANSI */
{
   TRC3(NhUiNhuBndCfm);

   RLOG2(L_DEBUG, "suId[%d], status[%d]", suId, status);

   RETVALUE((*NhUiNhuBndCfmMt[pst->selector])(pst, suId, status));
} /* end of NhUiNhuBndCfm */

/**************************************************************************
 *
 *      Fun:   NhUiNhuDatInd
 *
 *      Desc:  Data Indication to the RRC User (Upper layer).
 *
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_ptui.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 NhUiNhuDatInd
(
Pst *pst,
SpId spId,
NhuDatIndSdus *nhuDataIndSdus
)
#else /* ANSI */
PUBLIC S16 NhUiNhuDatInd(pst, spId, nhuDataIndSdus)
Pst *pst;
SpId spId;
NhuDatIndSdus *nhuDataIndSdus;
#endif /* ANSI */
{
   TRC3(NhUiNhuDatInd);

   RLOG1(L_DEBUG, "spId[%d]", spId);

   RETVALUE((*NhUiNhuDatIndMt[pst->selector])(pst, spId, nhuDataIndSdus));
} /* end of function NhUiNhuDatInd */

/**************************************************************************
 *
 *      Fun:   NhUiNhuErrInd
 *
 *      Desc:  Error Indication to the RRC User (Upper layer).
 *
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_ptui.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 NhUiNhuErrInd
(
Pst *pst,
SuId suId,
NhuErrIndSdus *nhuErrorIndSdus
)
#else /* ANSI */
PUBLIC S16 NhUiNhuErrInd(pst, suId, nhuErrorIndSdus)
Pst *pst;
SuId suId;
NhuErrIndSdus *nhuErrorIndSdus;
#endif /* ANSI */
{
   TRC3(NhUiNhuErrInd);

   RLOG1(L_DEBUG, "suId[%d]", suId);

   RETVALUE((*NhUiNhuErrIndMt[pst->selector])(pst, suId, nhuErrorIndSdus));
} /* end of function NhUiNhuErrInd */

/**************************************************************************
 *
 *      Fun:   NhUiNhuStaInd
 *
 *      Desc:  Status Indication to the RRC User (Upper layer).
 *
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_ptui.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 NhUiNhuStaInd
(
Pst           *pst,
SuId          suId,
NhuStaIndSdus *nhuStatusIndSdus
)
#else /* ANSI */
PUBLIC S16 NhUiNhuStaInd(pst, suId, nhuStatusIndSdus)
Pst           *pst;
SuId          suId;
NhuStaIndSdus *nhuStatusIndSdus;
#endif /* ANSI */
{
   TRC3(NhUiNhuStaInd);

   RLOG1(L_DEBUG, "suId[%d]", suId);

   RETVALUE((*NhUiNhuStaIndMt[pst->selector])(pst, suId, nhuStatusIndSdus));
} /* end of function NhUiNhuStaInd */

/**************************************************************************
 *
 *      Fun:   NhUiNhuDatCfm
 *
 *      Desc:  Data Confirm to the RRC User (Upper layer).
 *
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_ptui.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 NhUiNhuDatCfm
(
Pst            *pst,
SuId           suId,
NhuDatCfmSdus  *nhuDataCfmSdus
)
#else /* ANSI */
PUBLIC S16 NhUiNhuDatCfm(pst, suId, nhuDataCfmSdus)
Pst            *pst;
SuId           suId;
NhuDatCfmSdus  *nhuDataCfmSdus;
#endif /* ANSI */
{
   TRC3(NhUiNhuDatCfm);

   RLOG1(L_DEBUG, "suId[%d]", suId);

   RETVALUE((*NhUiNhuDatCfmMt[pst->selector])(pst, suId, nhuDataCfmSdus));
} /* end of function NhUiNhuDatCfm */



/**************************************************************************
 *
 *      Fun:   NhUiNhuCfgCfm
 *
 *      Desc:  Config Confirm to the RRC User (Upper layer).
 *
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_ptui.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 NhUiNhuCfgCfm
(
Pst *pst,
SpId spId,
NhuCfgCfmSdus *nhuConfigCfmSdus
)
#else /* ANSI */
PUBLIC S16 NhUiNhuCfgCfm(pst, spId, nhuConfigCfmSdus)
Pst *pst;
SpId spId;
NhuCfgCfmSdus *nhuConfigCfmSdus;
#endif /* ANSI */
{
   TRC3(NhUiNhuCfgCfm);

   RLOG1(L_DEBUG, "spId[%d]", spId);

   RETVALUE((*NhUiNhuCfgCfmMt[pst->selector])(pst, spId, nhuConfigCfmSdus));
} /* end of function RrUiRruCfgCfm */

/**************************************************************************
 *
 *      Fun:   NhUiNhuCellCfgCfm
 *
 *      Desc:  Cell Config Confirm to the RRC User (Upper layer).
 *
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_ptui.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 NhUiNhuCellCfgCfm
(
Pst *pst,
SpId spId,
NhuCellCfgCfmSdus *nhuCellConfigCfmSdus
)
#else /* ANSI */
PUBLIC S16 NhUiNhuCellCfgCfm(pst, spId, nhuCellConfigCfmSdus)
Pst *pst;
SpId spId;
NhuCellCfgCfmSdus *nhuCellConfigCfmSdus;
#endif /* ANSI */
{
   TRC3(NhUiNhuCellCfgCfm);


   RLOG1(L_DEBUG, "spId[%d]", spId);

   RETVALUE((*NhUiNhuCellCfgCfmMt[pst->selector])(pst, spId,
                                                  nhuCellConfigCfmSdus));
} /* end of function NhUiNhuCellCfgCfm */


/**************************************************************************
 *
 *      Fun:   NhUiNhuDecodeCfm
 *
 *      Desc:  Config Confirm to the RRC User (Upper layer).
 *
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_ptui.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 NhUiNhuDecodeCfm
(
Pst *pst,
SpId spId,
NhuDecodeSduCfm *nhuDecSduCfm
)
#else /* ANSI */
PUBLIC S16 NhUiNhuDecodeCfm(pst, spId, nhuDecSduCfm)
Pst *pst;
SpId spId;
NhuDecodeSduCfm *nhuDecSduCfm;
#endif /* ANSI */
{
   TRC3(NhUiNhuDecodeCfm);

   RLOG1(L_DEBUG, "spId[%d]", spId);

   RETVALUE((*NhUiNhuDecodeCfmMt[pst->selector])(pst, spId, nhuDecSduCfm));
} /* end of function NhUiNhuDecodeCfm */
/* ICNC : other primitives follow here: added other primitives*/

/**************************************************************************
 *
 *      Fun:   NhUiNhuCountCfm
 *
 *      Desc:  Config Confirm to the RRC User (Upper layer).
 *
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_ptui.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 NhUiNhuCountCfm
(
Pst *pst,
SpId spId,
NhuDRBCountInfoLst *nhuDRBCntInfoLst
)
#else /* ANSI */
PUBLIC S16 NhUiNhuCountCfm(pst, spId, nhuDRBCntInfoLst)
Pst *pst;
SpId spId;
NhuDRBCountInfoLst *nhuDRBCntInfoLst;
#endif /* ANSI */
{
   TRC3(NhUiNhuCountCfm);

   RLOG1(L_DEBUG, "spId[%d]", spId);

   RETVALUE((*NhUiNhuCountCfmMt[pst->selector])(pst, spId, nhuDRBCntInfoLst));
} /* end of function NhUiNhuCountCfm */

/**************************************************************************
 *
 *      Fun:   NhUiNhuPdcpSduStaCfm
 *
 *      Desc:  PDCP SDU status confirm to the RRC User (Upper layer).
 *
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_ptui.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 NhUiNhuPdcpSduStaCfm
(
Pst  *pst,
SuId suId,
NhuPdcpSduStaCfm *nhuPdcpSduStatCfm
)
#else /* ANSI */
PUBLIC S16 NhUiNhuPdcpSduStaCfm(pst, suId, nhuPdcpSduStatCfm)
Pst   *pst;
SuId  suId;
NhuPdcpSduStaCfm *nhuPdcpSduStatCfm;
#endif /* ANSI */
{
   TRC3(NhUiNhuPdcpSduStaCfm);

   RLOG1(L_DEBUG, "suId[%d]", suId);

   RETVALUE((*NhUiNhuPdcpSduStaCfmMt[pst->selector])(pst, suId, 
                                    nhuPdcpSduStatCfm));
} /* end of function NhUiNhuCountCfm */

/**************************************************************************
 *
 *      Fun:   NhUiNhuPdcpDatResumeCfm
 *
 *      Desc:  PDCP Data Resume confirm to the RRC User (Upper layer).
 *
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_ptui.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 NhUiNhuPdcpDatResumeCfm
(
Pst  *pst,
SuId suId,
NhuPdcpDatResumeCfm *nhuPdcpDatResumeCfm
)
#else /* ANSI */
PUBLIC S16 NhUiNhuPdcpDatResumeCfm(pst, suId, nhuPdcpDatResumeCfm)
Pst   *pst;
SuId  suId;
NhuPdcpDatResumeCfm *nhuPdcpDatResumeCfm;
#endif /* ANSI */
{
   TRC3(NhUiNhuPdcpDatResumeCfm);

   RLOG1(L_INFO, " NhUiNhuPdcpDatResumeCfm(pst, spId(%d) ", suId);

   RETVALUE((*NhUiNhuPdcpDatResumeCfmMt[pst->selector])(pst, suId, 
                                    nhuPdcpDatResumeCfm));
} /* end of function NhUiNhuCountCfm */
/**************************************************************************
 *
 *      Fun:   NhUiNhuEncodeCfm
 *
 *      Desc:  Encode Confirm to the RRC User (Upper layer).
 *
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_ptui.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 NhUiNhuEncodeCfm
(
Pst *pst,
SuId suId,
NhuEncCfmSdus *nhuEncodeCfmSdus
)
#else /* ANSI */
PUBLIC S16 NhUiNhuEncodeCfm(pst, suId, nhuEncodeCfmSdus)
Pst *pst;
SuId suId;
NhuEncCfmSdus *nhuEncodeCfmSdus;
#endif /* ANSI */
{
   TRC3(NhUiNhuEncodeCfm);

   RLOG1(L_DEBUG, "suId[%d]", suId);

   RETVALUE((*NhUiNhuEncodeCfmMt[pst->selector])(pst, suId, nhuEncodeCfmSdus));
} /* end of function NhUiNhuEncodeCfm */
/* ICNC : other primitives follow here: added other primitives*/


/**************************************************************************
 * LTE RRC Upper interface PORTABALE functions
 *************************************************************************/
#ifdef PTNHUINHU

/**************************************************************************
 *
 *      Fun:   PtUiNhuBndCfm
 *
 *      Desc:  Bind Confirm from RRC to the RRC User.
 * 
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_ptui.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 PtUiNhuBndCfm
(
Pst *pst,               /* post structure */
SuId suId,              /* service user identifier */
U8 status               /* status */
)
#else /* ANSI */
PUBLIC S16 PtUiNhuBndCfm (pst, suId, status)
Pst *pst;               /* post structure */
SuId suId;              /* service user identifier */
U8 status;              /* status */
#endif /* ANSI */
{
   TRC3(PtUiNhuBndCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(status);

   RLOG0(L_DEBUG, "PtUiNhuBndCfm() called");

   RETVALUE(ROK);
} /* end of function PtUiNhuBndCfm */


/**************************************************************************
 *
 *      Fun:   PtUiNhuDatInd
 *
 *      Desc:  Data Indication from RRC to the RRC User.
 * 
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_ptui.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 PtUiNhuDatInd
(
Pst *pst,                     /* Post structure */
SuId suId,                    /* Service user identifier */
NhuDatIndSdus *nhuDataIndSdus  /* Data Indication SDU to RRC user */
)
#else /* ANSI */
PUBLIC S16 PtUiNhuDatInd (pst, suId, nhuDataIndSdus)
Pst *pst;                     /* Post structure */
SuId suId;                    /* Service user identifier */
NhuDatIndSdus *nhuDataIndSdus; /* Data Indication SDU to RRC user */
#endif /* ANSI */
{
   TRC3(PtUiNhuDatInd);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(nhuDataIndSdus);

   RLOG0(L_DEBUG, "PtUiNhuDatInd() called");

   RETVALUE(ROK);
} /* end of function PtUiNhuDatInd */

/**************************************************************************
 *
 *      Fun:   PtUiNhuErrInd
 *
 *      Desc:  Error Indication from RRC to the RRC User.
 * 
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_ptui.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 PtUiNhuErrInd
(
Pst *pst,                     /* Post structure */
SuId suId,                    /* Service user identifier */
NhuErrIndSdus *nhuErrorIndSdus  /* Error Indication SDU to RRC user */
)
#else /* ANSI */
PUBLIC S16 PtUiNhuErrInd (pst, suId, nhuErrorIndSdus)
Pst *pst;                     /* Post structure */
SuId suId;                    /* Service user identifier */
NhuErrIndSdus *nhuErrorIndSdus; /* Error Indication SDU to RRC user */
#endif /* ANSI */
{
   TRC3(PtUiNhuErrInd);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(nhuErrorIndSdus);

   RLOG0(L_DEBUG, "PtUiNhuErrInd() called");

   RETVALUE(ROK);
} /* end of function PtUiNhuErrInd */

/**************************************************************************
 *
 *      Fun:   PtUiNhuStaInd
 *
 *      Desc:  Status Indication from RRC to the RRC User.
 * 
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_ptui.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 PtUiNhuStaInd
(
Pst    *pst,                     /* Post structure */
SuId   suId,                     /* Service user identifier */
NhuStaIndSdus  *nhuStatusIndSdus    /* SDU */
)
#else /* ANSI */
PUBLIC S16 PtUiNhuStaInd (pst, suId, nhuStatusIndSdus)
Pst    *pst;                     /* Post structure */
SuId   suId;                     /* Service user identifier */
NhuStaIndSdus  *nhuStatusIndSdus;   /* SDU */
#endif /* ANSI */
{
   TRC3(PtUiNhuStaInd);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(nhuStatusIndSdus);

   RLOG0(L_DEBUG, "PtUiNhuStaInd() called");

   RETVALUE(ROK);
} /* end of function PtUiNhuStaInd */

/**************************************************************************
 *
 *      Fun:   PtUiNhuDatCfm
 *
 *      Desc:  Data Confirm from RRC to the RRC User.
 * 
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_ptui.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 PtUiNhuDatCfm
(
Pst    *pst,                     /* Post structure */
SuId   suId,                     /* Service user identifier */
NhuDatCfmSdus  *nhuDataCfmSdus    /* SDU */
)
#else /* ANSI */
PUBLIC S16 PtUiNhuDatCfm (pst, suId, nhuDataCfmSdus)
Pst    *pst;                     /* Post structure */
SuId   suId;                     /* Service user identifier */
NhuDatCfmSdus  *nhuDataCfmSdus;   /* SDU */
#endif /* ANSI */
{
   TRC3(PtUiNhuDatCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(nhuDataCfmSdus);

   RLOG0(L_DEBUG, "PtUiNhuDatCfm() called");

   RETVALUE(ROK);
} /* end of function PtUiNhuDatCfm */


/**************************************************************************
 *
 *      Fun:   PtUiNhuCfgCfm
 *
 *      Desc:  Configuration confirm from RRC to the RRC User.
 * 
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_ptui.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 PtUiNhuCfgCfm
(
Pst *pst,                     /* Post structure */
SuId suId,                    /* Service user identifier */
NhuCfgCfmSdus *nhuConfigCfmSdus  /* Config confirm info to RRC user */
)
#else /* ANSI */
PUBLIC S16 PtUiNhuCfgCfm (pst, suId, nhuConfigCfmSdus)
Pst *pst;                     /* Post structure */
SuId suId;                    /* Service user identifier */
NhuCfgCfmSdus *nhuConfigCfmSdus; /* Config confirm info to RRC user */
#endif /* ANSI */
{
   TRC3(PtUiNhuCfgCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(nhuConfigCfmSdus);

   RLOG0(L_DEBUG, "PtUiNhuCfgCfm() called");

   RETVALUE(ROK);
} /* end of function PtUiNhuCfgCfm */


/**************************************************************************
 *
 *      Fun:   PtUiNhuCellCfgCfm
 *
 *      Desc:  Cell configuration confirm from RRC to the RRC User.
 * 
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_ptui.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 PtUiNhuCellCfgCfm
(
Pst *pst,                              /* Post structure */
SuId suId,                             /* Service user identifier */
NhuCellCfgCfmSdus *nhuCellConfigCfmSdus   /* Config confirm info to RRC user */
)
#else /* ANSI */
PUBLIC S16 PtUiNhuCellCfgCfm(pst, suId, nhuCellConfigCfmSdus)
Pst *pst;                              /* Post structure */
SuId suId;                             /* Service user identifier */
NhuCellCfgCfmSdus *nhuCellConfigCfmSdus;  /* Config confirm info to RRC user */
#endif /* ANSI */
{
   TRC3(PtUiNhuCellCfgCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(nhuCellConfigCfmSdus);

   RLOG0(L_DEBUG, "PtUiNhuCellCfgCfm() called");

   RETVALUE(ROK);
} /* end of function PtUiNhuCellCfgCfm */

/**************************************************************************
 *
 *      Fun:   PtUiNhuDecodeCfm
 *
 *      Desc:  Decode confirm from RRC to the RRC User.
 * 
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_ptui.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 PtUiNhuDecodeCfm
(
Pst *pst,                              /* Post structure */
SuId suId,                             /* Service user identifier */
NhuDecodeSduCfm *nhuDecSduCfm   /* Config confirm info to RRC user */
)
#else /* ANSI */
PUBLIC S16 PtUiNhuDecodeCfm(pst, suId, nhuDecSduCfm)
Pst *pst;                              /* Post structure */
SuId suId;                             /* Service user identifier */
NhuDecodeSduCfm *nhuDecSduCfm;  /* Config confirm info to RRC user */
#endif /* ANSI */
{
   TRC3(PtUiNhuDecodeCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(nhuDecSduCfm);

   RLOG0(L_DEBUG, "PtUiNhuDecodeCfm() called");

   RETVALUE(ROK);
} /* end of function PtUiNhuDecodeCfm */

/**************************************************************************
 *
 *      Fun:   PtUiNhuCountCfm
 *
 *      Desc:  Count confirm  from RRC to the RRC User.
 * 
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_ptui.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 PtUiNhuCountCfm
(
Pst *pst,                              /* Post structure */
SuId suId,                             /* Service user identifier */
NhuDRBCountInfoLst *nhuDRBCntInfoLst   /* Config confirm info to RRC user */
)
#else /* ANSI */
PUBLIC S16 PtUiNhuCountCfm(pst, suId, nhuDRBCntInfoLst)
Pst *pst;                              /* Post structure */
SuId suId;                             /* Service user identifier */
NhuDRBCountInfoLst *nhuDRBCntInfoLst;  /* Config confirm info to RRC user */
#endif /* ANSI */
{
   TRC3(PtUiNhuCountCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(nhuDRBCntInfoLst);

   RLOG0(L_DEBUG, "PtUiNhuCountCfm() called");

   RETVALUE(ROK);
} /* end of function PtUiNhuCountCfm */

/**************************************************************************
 *
 *      Fun:   PtUiNhuPdcpSduStaCfm
 *
 *      Desc:  PDCP SDU Status confirm  from RRC to the RRC User.
 * 
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_ptui.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 PtUiNhuPdcpSduStaCfm
(
Pst  *pst,                             /* Post structure */
SuId suId,                             /* Service user identifier */
NhuPdcpSduStaCfm *nhuPdcpSduStatCfm     /* PDCP SDU Status confirm info to RRC user */
)
#else /* ANSI */
PUBLIC S16 PtUiNhuPdcpSduStaCfm(pst, suId, nhuPdcpSduStatCfm)
Pst *pst;                              /* Post structure */
SuId suId;                             /* Service user identifier */
NhuPdcpSduStaCfm *nhuPdcpSduStatCfm;    /* PDCP SDU Status confirm info to RRC user */
#endif /* ANSI */
{
   TRC3(PtUiNhuPdcpSduStaCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(nhuPdcpSduStatCfm);

   RLOG0(L_DEBUG, "PtUiNhuPdcpSduStaCfm() called");

   RETVALUE(ROK);
} /* end of function PtUiNhuPdcpSduStaCfm */


/**************************************************************************
 *
 *      Fun:   PtUiNhuPdcpDatResumeCfm
 *
 *      Desc:  PDCP Data Resume Confirm  from RRC to the RRC User.
 * 
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_ptui.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 PtUiNhuPdcpDatResumeCfm
(
Pst  *pst,                             /* Post structure */
SuId suId,                             /* Service user identifier */
NhuPdcpDatResumeCfm *nhuPdcpDatResumeCfm     /* PDCP Data Resume confirm info to RRC user */
)
#else /* ANSI */
PUBLIC S16 PtUiNhuPdcpDatResumeCfm(pst, suId, nhuPdcpDatResumeCfm)
Pst *pst;                              /* Post structure */
SuId suId;                             /* Service user identifier */
NhuPdcpDatResumeCfm *nhuPdcpDatResumeCfm;    /* PDCP Data Resume confirm info to RRC user */
#endif /* ANSI */
{
   TRC3(PtUiNhuPdcpDatResumeCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(nhuPdcpDatResumeCfm);

   RLOG0(L_ERROR, " Function called ");
   RETVALUE(ROK);
} /* end of function PtUiNhuPdcpDatResumeCfm */
/**************************************************************************
 *
 *      Fun:   PtUiNhuEncodeCfm
 *
 *      Desc:  Encode confirm from RRC to the RRC User.
 * 
 *      Ret:   ROK - OK
 *
 *      Notes: None
 *
 *      File:  nh_ptui.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 PtUiNhuEncodeCfm
(
Pst *pst,                          /* Post structure */
SuId suId,                         /* Service user identifier */
NhuEncCfmSdus *nhuEncodeCfmSdus       /* Encode confirm info to RRC user */
)
#else /* ANSI */
PUBLIC S16 PtUiNhuEncodeCfm(pst, suId, nhuEncodeCfmSdus)
Pst *pst;                          /* Post structure */
SuId suId;                         /* Service user identifier */
NhuEncCfmSdus *nhuEncodeCfmSdus;      /* Encode confirm info to RRC user */
#endif /* ANSI */
{
   TRC3(PtUiNhuEncodeCfm);

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(nhuEncodeCfmSdus);

   RLOG0(L_DEBUG, "PtUiNhuEncodeCfm() called");

   RETVALUE(ROK);
} /* end of function PtUiNhuEncodeCfm */

#endif /* PTNHUINHU */


/**********************************************************************
  
         End of file:     ni_ptui.c@@/main/3 - Fri Jul  1 01:13:21 2011
   
**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      rer              1. LTE-RRC Initial Release.
/main/2      ---      ch               1. RRC Release 2.1.
/main/3      ---      sbalakrishna     1. Updated for Release of 3.1.
*********************************************************************91*/

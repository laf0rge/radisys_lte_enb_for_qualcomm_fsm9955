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
  
     Desc:     RRC Lower interface incoming primitives. 

     File:     nh_ptli.c

     Sid:      ni_ptli.c@@/main/3 - Fri Jul  1 01:13:21 2011

     Prg:      mc

**********************************************************************/
/*
*     This software may be combined with the following TRILLIUM
*     software:
*
*     part no.                      description
*     --------    ----------------------------------------------
*     1000369     LTE-RRC
*
*/

/** @defgroup ptli_group Portable Lower Interface Module
 *  @{
 */
static const char* RLOG_MODULE_NAME="RRC";
static int RLOG_MODULE_ID=512;
static int RLOG_FILE_ID=92;
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */
  
#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer defines */
#include "cm_tkns.h"       /* Common tokens defines */
#include "cm_mblk.h"       /* Common memory allocation library defines */
#include "cm_llist.h"      /* Common link list defines */
#include "cm_hash.h"       /* Common hashlist defines */
#include "cm_lte.h"        /* Common LTE defines */
#include "cm_pasn.h"       /* Common Pasn defines */

#include "nhu.h"           /* NHU Interface defines */
#include "lnh.h"           /* LNH Interface defines */
#include "kwu.h"           /* KWU Interface defines */
#include "ckw.h"           /* CKW Interface defines */
#include "crg.h"           /* CRG Interface defines */
#include "pju.h"           /* PJU Interface defines */
#include "cpj.h"           /* CPJ Interface defines */
#ifdef RRC_PHY_CFG_SUPP
#include "ctf.h"           /* CTF Interface defines */
#endif /* RRC_PHY_CFG_SUPP */

#include "nh.h"            /* RRC defines */

/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */

#include "cm5.x"           /* Common timer library */
#include "cm_tkns.x"       /* Common tokens */
#include "cm_mblk.x"       /* Common memory allocation */
#include "cm_llist.x"      /* Common link list */
#include "cm_hash.x"       /* Common hashlist */
#include "cm_lte.x"        /* Common LTE includes */
#include "cm_pasn.x"       /* Common Pasn includes */
#include "cm_lib.x"        /* Common memory allocation library */

#include "kwu.x"           /* KWU Interface includes */
#include "ckw.x"           /* CKW Interface includes */
#include "nhu.x"           /* NHU Interface includes */
#include "lnh.x"           /* LNH Interface includes */
#include "crg.x"           /* CRG Interface includes */
#include "cpj.x"           /* CPJ Interface includes */
#ifdef RRC_PHY_CFG_SUPP
#include "ctf.x"           /* CTF Interface includes */
#endif /* RRC_PHY_CFG_SUPP */
#include "pju.x"           /* PJU Interface includes */
#include "nh.x"            /* RRC includes */



/* local defines */
#define MAXNHLI  3         /* The size of the LC/TC matrix */


/* KWU Interface flags */
#ifndef LCNHLIKWU
#define PTNHLIKWU   1      /* Portable KWU Interface */
#else /* LCNHLIKWU */
#if(!(defined KW)  || !(defined DM))
#define PTNHLIKWU   1      /* Portable KWU Interface */
#endif /* KW && DM */
#endif /* LCNHLIKWU */

/* CKW Interface flags */
#ifndef LCNHLICKW
#define PTNHLICKW   1      /* Portable CKW Interface */
#else /* LCNHLIKWU */
#if(!(defined KW)  || !(defined DM))
#define PTNHLICKW   1      /* Portable CKW Interface */
#endif /* KW && DM */
#endif /* PTNHLICKW */

/* CRG Interface flags */
#ifndef LCNHLICRG
#define PTNHLICRG   1      /* Portable CRG Interface */
#else/* LCNHLICRG */
#if(!(defined RG)  || !(defined DM))
#define PTNHLICRG   1      /* Portable CRG Interface */
#endif /* RG && DM */
#endif /* LCNHLICRG */

/* CPJ Interface flags */
#ifndef LCNHLICPJ
#define PTNHLICPJ   1      /* Portable CPJ Interface */
#else/* LCNHLICPJ */
#if(!(defined PJ)  || !(defined DM))
#define PTNHLICPJ   1      /* Portable CPJ Interface */
#endif /* PJ && DM */
#endif /* LCNHLICPJ */

#ifdef RRC_PHY_CFG_SUPP
/* CTF Interface flags */
#ifndef LCNHLICTF
#define PTNHLICTF   1      /* Portable CTF Interface */
#else/* LCNHLICTF */
#if(!(defined TF)  || !(defined DM))
#define PTNHLICTF   1      /* Portable CTF Interface */
#endif /* TF && DM */
#endif /* LCNHLICTF */
#endif /* RRC_PHY_CFG_SUPP */

/* PJU Interface flags */
#ifndef LCNHLIPJU
#define PTNHLIPJU   1      /* Portable PJU Interface */
#else/* LCNHLIPJU */
#if(!(defined PJ)  || !(defined DM))
#define PTNHLIPJU   1      /* Portable PJU Interface */
#endif /* PJ && DM */
#endif /* LCNHLIPJU */

#ifdef __cplusplus
extern "C" {
#endif /* cplusplus */
 
/* portable functions */
#ifdef PTNHLIKWU
PRIVATE S16 PtLiKwuBndReq ARGS((Pst *post, SuId suId, SpId spId));
PRIVATE S16 PtLiKwuUbndReq ARGS((Pst *post, SpId spId, Reason reason));
PRIVATE S16 PtLiKwuDatReq ARGS((Pst *post, SpId spId, KwuDatReqInfo *datReq, 
                                Buffer *datBuf));

#endif /* PTNHLIKWU */

#ifdef PTNHLICKW
PRIVATE S16 PtLiCkwBndReq ARGS((Pst *post, SuId suId, SpId spId));
PRIVATE S16 PtLiCkwUbndReq ARGS((Pst *post, SpId spId, Reason reason));
PRIVATE S16 PtLiCkwCfgReq ARGS((Pst *post, SpId spId, CkwCfgInfo *cfgInfo ));
PRIVATE S16 PtLiCkwUeIdChgReq ARGS((Pst *post, SpId spId,U32 transId, 
                                    CkwUeInfo *oldUeInfo,
                                    CkwUeInfo *newUeInfo));

#endif /* PTNHLICKW */

#ifdef PTNHLICRG
PRIVATE S16 PtLiCrgBndReq ARGS((Pst *post, SuId suId, SpId spId));
PRIVATE S16 PtLiCrgUbndReq ARGS((Pst *post, SpId spId, Reason reason));
PRIVATE S16 PtLiCrgCfgReq ARGS((Pst *post, SpId spId, CrgCfgTransId transId,
CrgCfgReqInfo *cmacCfg));
#endif /* PTNHLICRG */


#ifdef PTNHLICPJ
PRIVATE S16 PtLiCpjBndReq ARGS((Pst *post, SuId suId, SpId spId));
PRIVATE S16 PtLiCpjUbndReq ARGS((Pst *post, SpId spId, Reason reason));
PRIVATE S16 PtLiCpjCfgReq ARGS((Pst *post, SpId spId, CpjCfgReqInfo *pdCfgReq));
PRIVATE S16 PtLiCpjUeIdChgReq ARGS((Pst *post, SpId spId,U32 transId, 
                                    CpjUeInfo *oldUeInfo,
                                    CpjUeInfo *newUeInfo));

PRIVATE S16 PtLiCpjSecCfgReq ARGS((Pst *post, SpId spId, 
                                   CpjSecCfgReqInfo *pdcpSecCfgReq));
PRIVATE S16 PtLiCpjCountReq ARGS((Pst *post, SpId spId, 
                                   CpjCountReqInfo *pdcpCountReq));
PRIVATE S16 PtLiCpjReEstReq ARGS(( Pst *post, SpId spId,
                                   CpjReEstReqInfo *restbInfo));
PRIVATE S16 PtLiCpjSduStaReq ARGS(( Pst *post, SpId spId,
                                   CpjSduStaReqInfo *statReqInfo));
PRIVATE S16 PtLiCpjStartDataFwdReq ARGS(( Pst *post, SpId spId,
                                   CpjDataFwdReqInfo *dataFwdReqInfo));
PRIVATE S16 PtLiCpjDatResumeReq ARGS(( Pst *post, SpId spId,
                                   CpjDatResumeReqInfo *DatResumeReqInfo));

#endif /* PTNHLICPJ */

#ifdef RRC_PHY_CFG_SUPP
#ifdef PTNHLICTF
/** Request from RRC to PHY to bind the CTF interface sap. */
EXTERN S16 PtLiCtfBndReq ARGS((Pst* post, SuId suId, SpId spId));
/** Request from RRC to PHY to Unbind the CTF interface sap. */
EXTERN S16 PtLiCtfUbndReq ARGS((Pst* post, SpId spId, Reason reason));
/** Configuration Request from RRC to PHY for 
 * cell or dedicated configuration. 
*/
EXTERN S16 PtLiCtfCfgReq ARGS((Pst* post, SpId spId, CtfCfgTransId transId, 
                               CtfCfgReqInfo* cfgReqInfo));
/** UE ID change Request from RRC to PHY. */
EXTERN S16 PtLiCtfUeIdChgReq ARGS((Pst* post, SpId spId, CtfCfgTransId transId, 
                                   CtfUeInfo * ueInfo, CtfUeInfo * newUeInfo));
#endif /*--#ifdef PTNHLICTF--*/
#endif /* RRC_PHY_CFG_SUPP */

#ifdef PTNHLIPJU
PRIVATE S16 PtLiPjuBndReq ARGS((Pst *post, SuId suId, SpId spId));
PRIVATE S16 PtLiPjuUbndReq ARGS((Pst *post, SpId spId, Reason reason));
PRIVATE S16 PtLiPjuDatReq ARGS((Pst *post, SpId spId, CmLtePdcpId *pdcpId, 
                                PjuSduId sduId, Buffer *mBuf));
#endif/* PTNHLIPJU */
/*
  
   The following matrices define the mapping between the primitives called by 
   the lower interface of RRC and the corresponding primitives of the lower layers.

   The parameter MAXNHLI defines the maximum number of service providers.
   There is an array of functions per primitive invoked by RRC.  Every array 
   is MAXNHLI long (that is, there are as many functions as the number of service 
   providers).

   The dispatching is performed by the configurable variable: selector.  The 
   selector is configured on a per SAP basis.

   The selectors are:

   NH_SEL_LC - Loosely coupled
   NH_SEL_TC - Tightly coupled lower layer 

*/

/**************************************************************************
 *KWU Interface Matrices
 *************************************************************************/

 /* KWU Interface Bind Request primitive */
  
PRIVATE CONSTANT KwuBndReq NhLiKwuBndReqMt [MAXNHLI] =
{
#ifdef LCNHLIKWU
   cmPkKwuBndReq,          /* 0 - loosely coupled */
#else  /* LCNHLIKWU */
   PtLiKwuBndReq,          /* 0 - loosely coupled, portable */
#endif /* LCNHLIKWU */
#ifdef KW 
   KwUiKwuBndReq,          /* 1 - tightly coupled, KWU */
#else /* KW */
   PtLiKwuBndReq,          /* 1 - tightly coupled, portable */
#endif /* KW */
#ifdef DM 
   DmUiKwuBndReq,          /* 2 - tightly coupled, KWU */
#else /* DM */
   PtLiKwuBndReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};

/* KWU Interface Unbind Request primitive */
PRIVATE CONSTANT KwuUbndReq NhLiKwuUbndReqMt [MAXNHLI] =
{
#ifdef LCNHLIKWU
 cmPkKwuUbndReq,          /* 0 - loosely coupled */
#else /* LCNHLIKWU */ 
 PtLiKwuUbndReq,          /* 0 - loosely coupled, portable */
#endif /* LCNHLIKWU */
#ifdef KW 
 KwUiKwuUbndReq,          /* 1 - tightly coupled, KWU */
#else /* KW */
 PtLiKwuUbndReq,          /* 1 - tightly coupled, portable */
#endif /* KW */
#ifdef DM 
 DmUiKwuUbndReq,          /* 2 - tightly coupled, KWU */
#else /* DM */
 PtLiKwuUbndReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};

/* KWU Interface Data Request primitive */
PRIVATE CONSTANT KwuDatReq NhLiKwuDatReqMt [MAXNHLI] =
{
#ifdef LCNHLIKWU
   cmPkKwuDatReq,          /* 0 - loosely coupled */
#else  /* LCNHLIKWU */
   PtLiKwuDatReq,          /* 0 - loosely coupled, portable */
#endif /* LCNHLIKWU */
#ifdef KW
   KwUiKwuDatReq,          /* 1 - tightly coupled, KWU */
#else /* KW */
   PtLiKwuDatReq,          /* 1 - tightly coupled, portable */
#endif /* KW */
#ifdef DM
   DmUiKwuDatReq,          /* 2 - tightly coupled, KWU */
#else /* DM */
   PtLiKwuDatReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};

/**************************************************************************
 * CKW Interface Matrices
 *************************************************************************/

/* CKW Interface Bind Request primitive */
PRIVATE CONSTANT CkwBndReq NhLiCkwBndReqMt [MAXNHLI] =
{
#ifdef LCNHLICKW
   cmPkCkwBndReq,          /* 0 - loosely coupled */
#else /* LCNHLICKW */ 
   PtLiCkwBndReq,          /* 0 - loosely coupled, portable */
#endif /* LCNHLICKW */
#ifdef KW 
   KwUiCkwBndReq,          /* 1 - tightly coupled, CKW */
#else /* KW */
   PtLiCkwBndReq,          /* 1 - tightly coupled, portable */
#endif /* KW */
#ifdef DM 
   DmUiCkwBndReq,          /* 2 - tightly coupled, CKW */
#else /* DM */
   PtLiCkwBndReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};

/* CKW Interface Unbind Request primitive */
PRIVATE CONSTANT CkwUbndReq NhLiCkwUbndReqMt [MAXNHLI] =
{
#ifdef LCNHLICKW
 cmPkCkwUbndReq,          /* 0 - loosely coupled */
#else /* LCNHLICKW */
 PtLiCkwUbndReq,          /* 0 - loosely coupled, portable */
#endif /* LCNHLICKW */
#ifdef KW 
 KwUiCkwUbndReq,          /* 1 - tightly coupled, CKW */
#else /* KW */
 PtLiCkwUbndReq,          /* 1 - tightly coupled, portable */
#endif /* KW */
#ifdef DM 
 DmUiCkwUbndReq,          /* 2 - tightly coupled, CKW */
#else /* DM */
 PtLiCkwUbndReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};


/* CKW Interface Configuration Request primitive */
PRIVATE CONSTANT CkwCfgReq NhLiCkwCfgReqMt [MAXNHLI] =
{
#ifdef LCNHLICKW
   cmPkCkwCfgReq,          /* 0 - loosely coupled */
#else  /* LCNHLICKW */
   PtLiCkwCfgReq,          /* 0 - loosely coupled, portable */
#endif /* LCNHLICKW */
#ifdef KW
   KwUiCkwCfgReq,          /* 1 - tightly coupled, CKW */
#else /* KW */
   PtLiCkwCfgReq,          /* 1 - tightly coupled, portable */
#endif /* KW */
#ifdef DM
   DmUiCkwCfgReq,          /* 2 - tightly coupled, CKW */
#else /* DM */
   PtLiCkwCfgReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};

/* CKW Interface UE Id Change Request primitive */
PRIVATE CONSTANT CkwUeIdChgReq NhLiCkwUeIdChgReqMt [MAXNHLI] =
{
#ifdef LCNHLICKW
   cmPkCkwUeIdChgReq,          /* 0 - loosely coupled */
#else /* LCNHLICKW */
   PtLiCkwUeIdChgReq,          /* 0 - loosely coupled, portable */
#endif /* LCNHLICKW */
#ifdef KW
   KwUiCkwUeIdChgReq,          /* 1 - tightly coupled, CKW */
#else /* KW */
   PtLiCkwUeIdChgReq,          /* 1 - tightly coupled, portable */
#endif /* KW */
#ifdef DM
   DmUiCkwUeIdChgReq,          /* 2 - tightly coupled, CKW */
#else /* DM */
   PtLiCkwUeIdChgReq,          /* 2 - tightly coupled, portable */
#endif/* DM */
};
/**************************************************************************
 * CRG Interface Matrices
 *************************************************************************/

/* CRG Interface Bind Request primitive */
PRIVATE CONSTANT CrgBndReq NhLiCrgBndReqMt [MAXNHLI] =
{
#ifdef LCNHLICRG
   cmPkCrgBndReq,          /* 0 - loosely coupled */
#else /* LCNHLICRG */
   PtLiCrgBndReq,          /* 0 - loosely coupled, portable */
#endif /* LCNHLICRG */
#ifdef RG 
   RgUiCrgBndReq,          /* 1 - tightly coupled, CRG */
#else /* RG */
   PtLiCrgBndReq,          /* 1 - tightly coupled, portable */
#endif /* RG */
#ifdef DM 
   DmUiCrgBndReq,          /* 2 - tightly coupled, CRG */
#else /* DM */
   PtLiCrgBndReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};

/* CRG Interface Unbind Request primitive */
PRIVATE CONSTANT CrgUbndReq NhLiCrgUbndReqMt [MAXNHLI] =
{
#ifdef LCNHLICRG
 cmPkCrgUbndReq,          /* 0 - loosely coupled */
#else /* LCNHLICRG */
 PtLiCrgUbndReq,          /* 0 - loosely coupled, portable */
#endif /* LCNHLICRG */
#ifdef RG 
 RgUiCrgUbndReq,          /* 1 - tightly coupled, CRG */
#else /* RG */
 PtLiCrgUbndReq,          /* 1 - tightly coupled, portable */
#endif /* RG */
#ifdef DM 
 DmUiCrgUbndReq,          /* 2 - tightly coupled, CRG */
#else /* DM */
 PtLiCrgUbndReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};

/* CRG Interface Config Request primitive */
PRIVATE CONSTANT CrgCfgReq NhLiCrgCfgReqMt[MAXNHLI] =
{
#ifdef LCNHLICRG
   cmPkCrgCfgReq,          /* 0 - loosely coupled */
#else /* LCNHLICRG */
   PtLiCrgCfgReq,          /* 0 - loosely coupled, portable */
#endif /* LCNHLICRG */
#ifdef RG 
   RgUiCrgCfgReq,          /* 1 - tightly coupled, CRG */
#else /* RG */
   PtLiCrgCfgReq,          /* 1 - tightly coupled, portable */
#endif /* RG */
#ifdef DM 
   DmUiCrgCfgReq,          /* 2 - tightly coupled, CRG */
#else /* DM */
   PtLiCrgCfgReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};


/**************************************************************************
 * CPJ Interface Matrices
 *************************************************************************/

/* CPJ Interface Bind Request primitive */
PRIVATE CONSTANT CpjBndReq NhLiCpjBndReqMt [MAXNHLI] =
{
#ifdef LCNHLICPJ
   cmPkCpjBndReq,          /* 0 - loosely coupled */
#else /* LCNHLICPJ */
   PtLiCpjBndReq,          /* 0 - loosely coupled, portable */
#endif /* LCNHLICPJ */
#ifdef PJ
   PjUiCpjBndReq,          /* 1 - tightly coupled, CTC */
#else /* PJ */
   PtLiCpjBndReq,          /* 1 - tightly coupled, portable */
#endif /* PJ */
#ifdef DM
   DmUiCpjBndReq,          /* 2 - tightly coupled, CTC */
#else /* DM */
   PtLiCpjBndReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};

/* CPJ Interface Unbind Request primitive */
PRIVATE CONSTANT CpjUbndReq NhLiCpjUbndReqMt [MAXNHLI] =
{
#ifdef LCNHLICPJ
 cmPkCpjUbndReq,          /* 0 - loosely coupled */
#else /* LCNHLICPJ */
 PtLiCpjUbndReq,          /* 0 - loosely coupled, portable */
#endif /* LCNHLICPJ */
#ifdef PJ 
 PjUiCpjUbndReq,          /* 1 - tightly coupled, CPJ */
#else /* PJ */
 PtLiCpjUbndReq,          /* 1 - tightly coupled, portable */
#endif /* PJ */
#ifdef DM 
 DmUiCpjUbndReq,          /* 2 - tightly coupled, CPJ */
#else /* DM */
 PtLiCpjUbndReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};

/* CPJ Interface Config Request primitive */
PRIVATE CONSTANT CpjCfgReq NhLiCpjCfgReqMt [MAXNHLI] =
{
#ifdef LCNHLICPJ
 cmPkCpjCfgReq,          /* 0 - loosely coupled */
#else /* LCNHLICPJ */
 PtLiCpjCfgReq,          /* 0 - loosely coupled, portable */
#endif /* LCNHLICPJ */
#ifdef PJ
 PjUiCpjCfgReq,          /* 1 - tightly coupled, CPJ */
#else /* PJ */
 PtLiCpjCfgReq,          /* 1 - tightly coupled, portable */
#endif /* PJ */
#ifdef DM
 DmUiCpjCfgReq,          /* 2 - tightly coupled, CPJ */
#else /* DM */
 PtLiCpjCfgReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};

/* CPJ Interface UE Id Change Request primitive */
PRIVATE CONSTANT CpjUeIdChgReq NhLiCpjUeIdChgReqMt [MAXNHLI] =
{
#ifdef LCNHLICPJ
   cmPkCpjUeIdChgReq,          /* 0 - loosely coupled */
#else /* LCNHLICPJ */
   PtLiCpjUeIdChgReq,          /* 0 - loosely coupled, portable */
#endif /* LCNHLICPJ */
#ifdef PJ
   PjUiCpjUeIdChgReq,          /* 1 - tightly coupled, CPJ */
#else /* PJ */
   PtLiCpjUeIdChgReq,          /* 1 - tightly coupled, portable */
#endif /* PJ */
#ifdef DM
   DmUiCpjUeIdChgReq,          /* 2 - tightly coupled, CPJ */
#else /* DM */
   PtLiCpjUeIdChgReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};

/* CPJ Interface Security Config Request primitive */
PRIVATE CONSTANT CpjSecCfgReq NhLiCpjSecCfgReqMt [MAXNHLI] =
{
#ifdef LCNHLICPJ
   cmPkCpjSecCfgReq,          /* 0 - loosely coupled */
#else /* LCNHLICPJ */
   PtLiCpjSecCfgReq,          /* 0 - loosely coupled, portable */
#endif /* LCNHLICPJ */
#ifdef PJ
   PjUiCpjSecCfgReq,          /* 1 - tightly coupled, CPJ */
#else /* PJ */
   PtLiCpjSecCfgReq,          /* 1 - tightly coupled, portable */
#endif/* PJ */
#ifdef DM
   DmUiCpjSecCfgReq,          /* 2 - tightly coupled, CPJ */
#else/* DM */
   PtLiCpjSecCfgReq,          /* 2 - tightly coupled, portable */
#endif/* DM */
};

/* CPJ Interface COUNT Request primitive */
PRIVATE CONSTANT CpjCountReq NhLiCpjCountReqMt [MAXNHLI] =
{
#ifdef LCNHLICPJ
   cmPkCpjCountReq,          /* 0 - loosely coupled */
#else /* LCNHLICPJ */
   PtLiCpjCountReq,          /* 0 - loosely coupled, portable */
#endif /* LCNHLICPJ */
#ifdef PJ
   PjUiCpjCountReq,          /* 1 - tightly coupled, CPJ */
#else /* PJ */
   PtLiCpjCountReq,          /* 1 - tightly coupled, portable */
#endif /* PJ */
#ifdef DM
   DmUiCpjCountReq,          /* 2 - tightly coupled, CPJ */
#else /* DM */
   PtLiCpjCountReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};

/* CPJ Interface PDCP Status Request primitive */
PRIVATE CONSTANT CpjSduStaReq NhLiCpjSduStaReqMt [MAXNHLI] =
{
#ifdef LCNHLICPJ
   cmPkCpjSduStaReq,          /* 0 - loosely coupled */
#else /* LCNHLICPJ */
   PtLiCpjSduStaReq,          /* 0 - loosely coupled, portable */
#endif /* LCNHLICPJ */
#ifdef PJ
   PjUiCpjSduStaReq,          /* 1 - tightly coupled, CPJ */
#else /* PJ */
   PtLiCpjSduStaReq,          /* 1 - tightly coupled, portable */
#endif /* PJ */
#ifdef DM
   DmUiCpjSduStaReq,          /* 2 - tightly coupled, CPJ */
#else /* DM */
   PtLiCpjSduStaReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};

/* CPJ Interface PDCP Data Fwd Request primitive */
PRIVATE CONSTANT CpjDataFwdReq NhLiCpjStartDataFwdReqMt [MAXNHLI] =
{
#ifdef LCNHLICPJ
   cmPkCpjStartDataFwdReq,          /* 0 - loosely coupled */
#else /* LCNHLICPJ */
   PtLiCpjStartDataFwdReq,          /* 0 - loosely coupled, portable */
#endif /* LCNHLICPJ */ 
#ifdef PJ /* SUBBU TODO- Need to implemnted */
   PjUiCpjDatForwardReq,          /* 1 - tightly coupled, CPJ */
#else /* PJ */
   PtLiCpjStartDataFwdReq,          /* 1 - tightly coupled, portable */
#endif /* PJ */
#ifdef DM
   DmUiCpjStartDataFwdReq,          /* 2 - tightly coupled, CPJ */
#else /* DM */
   PtLiCpjStartDataFwdReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};


/* CPJ Interface PDCP Data Fwd Request primitive */
PRIVATE CONSTANT CpjDatResumeReq NhLiCpjDatResumeReqMt [MAXNHLI] =
{
#ifdef LCNHLICPJ
   cmPkCpjDatResumeReq,          /* 0 - loosely coupled */
#else /* LCNHLICPJ */
   PtLiCpjDatResumeReq,          /* 0 - loosely coupled, portable */
#endif /* LCNHLICPJ */ 
#ifdef PJ /* SUBBU TODO- Need to implemnted */
   PjUiCpjSduStaReq,          /* 1 - tightly coupled, CPJ */
#else /* PJ */
   PtLiCpjDatResumeReq,          /* 1 - tightly coupled, portable */
#endif /* PJ */
#ifdef DM
   DmUiCpjDatResumeReq,          /* 2 - tightly coupled, CPJ */
#else /* DM */
   PtLiCpjDatResumeReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};

#ifdef RRC_PHY_CFG_SUPP
/**************************************************************************
 * CTF Interface Matrices
 *************************************************************************/

/* Request from RRC to PHY to bind the CTF interface sap. */
PRIVATE CONSTANT CtfBndReq NhLiCtfBndReqMt [MAXNHLI] =
{
#ifdef LCNHLICTF
   cmPkCtfBndReq,          /* 0 - loosely coupled */
#else /* LCNHLICTF */
   PtLiCtfBndReq,          /* 0 - loosely coupled, portable */
#endif /* LCNHLICTF */
#ifdef TF 
   TfUiCtfBndReq,          /* 1 - tightly coupled, CTF */
#else /* TF */
   PtLiCtfBndReq,          /* 1 - tightly coupled, portable */
#endif /* TF */
#ifdef DM 
   DmUiCtfBndReq,          /* 2 - tightly coupled, CTF */
#else /* DM */
   PtLiCtfBndReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};

/* Request from RRC to PHY to Unbind the CTF interface sap. */
PRIVATE CONSTANT CtfUbndReq NhLiCtfUbndReqMt [MAXNHLI] =
{
#ifdef LCNHLICTF
 cmPkCtfUbndReq,          /* 0 - loosely coupled */
#else /* LCNHLICTF */
 PtLiCtfUbndReq,          /* 0 - loosely coupled, portable */
#endif /* LCNHLICTF */
#ifdef TF 
 TfUiCtfUbndReq,          /* 1 - tightly coupled, CTF */
#else /* TF */
 PtLiCtfUbndReq,          /* 1 - tightly coupled, portable */
#endif /* TF */
#ifdef DM 
 DmUiCtfUbndReq,          /* 2 - tightly coupled, CTF */
#else /* DM */
 PtLiCtfUbndReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};

/* Configuration Request from RRC to PHY for 
 * cell or dedicated configuration. 
*/
PRIVATE CONSTANT CtfCfgReq NhLiCtfCfgReqMt[MAXNHLI] =
{
#ifdef LCNHLICTF
   cmPkCtfCfgReq,          /* 0 - loosely coupled */
#else /* LCNHLICTF */ 
   PtLiCtfCfgReq,          /* 0 - loosely coupled, portable */
#endif /* LCNHLICTF */
#ifdef TF
   TfUiCtfCfgReq,          /* 1 - tightly coupled, CTF */
#else /* TF */
   PtLiCtfCfgReq,          /* 1 - tightly coupled, portable */
#endif /* TF */
#ifdef DM 
   DmUiCtfCfgReq,          /* 2 - tightly coupled, CTF */
#else /* DM */
   PtLiCtfCfgReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};

/** UE ID change Request from RRC to PHY. */
PRIVATE CONSTANT CtfUeIdChgReq NhLiCtfUeIdChgReqMt[MAXNHLI] =
{
#ifdef LCNHLICTF
   cmPkCtfUeIdChgReq,
#else /* LCNHLICTF */
   PtLiCtfUeIdChgReq,
#endif /* LCNHLICTF */
#ifdef TF
   TfUiCtfUeIdChgReq,
#else /* TF */
   PtLiCtfUeIdChgReq,
#endif /* TF */
#ifdef DM
   DmUiCtfUeIdChgReq,
#else /* DM */
   PtLiCtfUeIdChgReq,
#endif /* DM */
};
#endif /* RRC_PHY_CFG_SUPP */

/* CPJ Interface Re-Establishment Request primitive */
PRIVATE CONSTANT CpjReEstReq NhLiCpjReEstReqMt [MAXNHLI] =
{
#ifdef LCNHLICPJ
   cmPkCpjReEstReq,          /* 0 - loosely coupled */
#else /* LCNHLICPJ */
   PtLiCpjReEstReq,          /* 0 - loosely coupled, portable */
#endif /* LCNHLICPJ */
#ifdef PJ
   PjUiCpjReEstReq,          /* 1 - tightly coupled, CPJ */
#else /* PJ */
   PtLiCpjReEstReq,          /* 1 - tightly coupled, portable */
#endif /* PJ */
#ifdef DM
   DmUiCpjReEstReq,          /* 2 - tightly coupled, CPJ */
#else /* DM */
   PtLiCpjReEstReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};

/**************************************************************************
 * PJU Interface Matrices
 *************************************************************************/

/* PJU Interface Bind Request primitive */
PRIVATE CONSTANT PjuBndReq NhLiPjuBndReqMt [MAXNHLI] =
{
#ifdef LCNHLIPJU
   cmPkPjuBndReq,          /* 0 - loosely coupled */
#else /* LCNHLIPJU */  
   PtLiPjuBndReq,          /* 0 - loosely coupled, portable */
#endif /* LCNHLIPJU */
#ifdef PJ 
   PjUiPjuBndReq,          /* 1 - tightly coupled, PJU */
#else /* PJ */
   PtLiPjuBndReq,          /* 1 - tightly coupled, portable */
#endif /* PJ */
#ifdef DM 
   DmUiPjuBndReq,          /* 2 - tightly coupled, PJU */
#else /* DM */
   PtLiPjuBndReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};

/* PJU Interface Unbind Request primitive */
PRIVATE CONSTANT PjuUbndReq NhLiPjuUbndReqMt [MAXNHLI] =
{
#ifdef LCNHLIPJU
 cmPkPjuUbndReq,          /* 0 - loosely coupled */
#else /* LCNHLIPJU */
 PtLiPjuUbndReq,          /* 0 - loosely coupled, portable */
#endif /* LCNHLIPJU */
#ifdef PJ 
 PjUiPjuUbndReq,          /* 1 - tightly coupled, PJU */
#else /* PJ */
 PtLiPjuUbndReq,          /* 1 - tightly coupled, portable */
#endif /* PJ */
#ifdef DM 
 DmUiPjuUbndReq,          /* 2 - tightly coupled, PJU */
#else /* DM */
 PtLiPjuUbndReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};

/* PJU Interface Config Request primitive */
PRIVATE CONSTANT PjuDatReq NhLiPjuDatReqMt [MAXNHLI] =
{
#ifdef LCNHLIPJU
 cmPkPjuDatReq,          /* 0 - loosely coupled */
#else /* LCNHLIPJU */
 PtLiPjuDatReq,          /* 0 - loosely coupled, portable */
#endif /* LCNHLICPJ */
#ifdef PJ
 PjUiPjuDatReq,          /* 1 - tightly coupled, PJU */
#else /* PJ */
 PtLiPjuDatReq,          /* 1 - tightly coupled, portable */
#endif /* PJ */
#ifdef DM
 DmUiPjuDatReq,          /* 2 - tightly coupled, PJU */
#else /* DM */
 PtLiPjuDatReq,          /* 2 - tightly coupled, portable */
#endif /* DM */
};

/*
 *     support functions
 */

/**************************************************************************
 * RRC Lower interface primitives
 *************************************************************************/

/**************************************************************************
 * The KWU Interface
 *************************************************************************/

/**
 * @brief  This function binds the RRC service user with the RLC service
 *         provider.
 *
 *      @details  This function binds the RRC service user with the RLC 
 *             service provider (this is the RLC data plane). The RLC 
 *             registers this new service user and allocates 
 *             a Service Access Point (SAP) for this bind. The service 
 *             user specifies the reference number that is used 
 *             for the duration of this bind.
 *
 */

#ifdef ANSI
PUBLIC S16 NhLiKwuBndReq
(
Pst *post,                       /* Post structure */
SuId suId,                      /* Service user ID */
SpId spId                       /* Service provider ID */
)
#else /* ANSI */
PUBLIC S16 NhLiKwuBndReq(post, suId, spId)
Pst *post;                       /* Post structure */
SuId suId;                      /* Service user ID */
SpId spId;                      /* Service provider ID */
#endif /* ANSI */
{
   TRC3(NhLiKwuBndReq);
   RETVALUE((*NhLiKwuBndReqMt[post->selector])(post, suId, spId));

} /* end of function NhLiKwuBndReq */


/**
*      @brief  This function unbinds the RRC service user with the RLC 
*             service provider (this is the RLC data plane).
*
*      @details This function unbinds the RRC service user with the RLC 
*             service provider (this is the RLC data plane).
*
*/
#ifdef ANSI
PUBLIC S16 NhLiKwuUbndReq
(
Pst *post,                       /* Post structure */
SpId spId,                      /* Service provider ID */
Reason reason                   /* Reason */
)
#else /* ANSI */
PUBLIC S16 NhLiKwuUbndReq(post, spId, reason)
Pst *post;                       /* Post structure */
SpId spId;                      /* Service provider ID */
Reason reason;                  /* Reason */
#endif /* ANSI */
{

   TRC3(NhLiKwuUbndReq);
   RETVALUE((*NhLiKwuUbndReqMt[post->selector])(post, spId, reason));

} /* end of function NhLiKwuUbndReq */



/**
 *      @brief  This function sends a Data Request on the KWU interface.
 *      @details This function sends a Data Request on the KWU interface.
 *
 */
#ifdef ANSI
PUBLIC S16 NhLiKwuDatReq
(
Pst *post,                       /* Post structure */
SpId spId,                      /* Service provider ID */
KwuDatReqInfo *datReqParam,     /* Data Request parameters */
Buffer *datBuf                  /* Data Buffer */
)
#else /* ANSI */
PUBLIC S16 NhLiKwuDatReq(post, spId, datReqParam, datBuf)
Pst *post;                       /* Post structure */
SpId spId;                      /* Service provider ID */
KwuDatReqInfo  *datReqParam;    /* Data Request parameters */
Buffer *datBuf;                 /* Data Buffer */
#endif /* ANSI */
{

   TRC3(NhLiKwuDatReq);
   RETVALUE((*NhLiKwuDatReqMt[post->selector])(post, spId, 
                                              datReqParam, datBuf));

} /* end of function NhLiKwuDatReq */


/**************************************************************************
 * The CKW Interface
 *************************************************************************/
/*
 *
 *      @brief  This function binds the RRC service user with the RLC 
 *             service provider (this is the CKW interface).
 *
 *      @details This function binds the RRC service user with the RLC 
 *             service provider (this is the CKW interface). The RLC 
 *             registers this new service user and allocates 
 *             a Service Access Point (SAP) for this bind. The service 
 *             user specifies the reference number that is used 
 *             for the duration of this bind.
 *
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 NhLiCkwBndReq
(
Pst *post,                       /* Post structure */
SuId suId,                      /* Service user ID */
SpId spId                       /* Service provider ID */
)
#else /* ANSI */
PUBLIC S16 NhLiCkwBndReq(post, suId, spId)
Pst *post;                       /* Post structure */
SuId suId;                      /* Service user ID */
SpId spId;                      /* Service provider ID */
#endif /* ANSI */
{

   TRC3(NhLiCkwBndReq);
   RETVALUE((*NhLiCkwBndReqMt[post->selector])(post, suId, spId));

} /* end of function NhLiCkwBndReq */


/**
*
*      @brief  This function unbinds the RRC service user with the RLC 
*             service provider (this is the CKW interface).
*
*      @details This function unbinds the RRC service user with the RLC 
*             service provider (this is the CKW interface).
*
*/
#ifdef ANSI
PUBLIC S16 NhLiCkwUbndReq
(
Pst *post,                       /* Post structure */
SpId spId,                      /* Service provider ID */
Reason reason                   /* Reason */
)
#else /* ANSI */
PUBLIC S16 NhLiCkwUbndReq(post, spId, reason)
Pst *post;                       /* Post structure */
SpId spId;                      /* Service provider ID */
Reason reason;                  /* Reason */
#endif /* ANSI */
{

   TRC3(NhLiCkwUbndReq);
   RETVALUE((*NhLiCkwUbndReqMt[post->selector])(post, spId, reason));

} /* end of function NhLiCkwUbndReq */



/**
 *      @brief  Sends a Configuration request to RLC on the CRL interface.
 *      @details Sends a Configuration request to RLC on the CRL interface.
 *
 */
#ifdef ANSI
PUBLIC S16 NhLiCkwCfgReq
(
Pst *post,                /* Post structure */
SpId spId,               /* Service provider */
CkwCfgInfo *cfgInfo
)
#else /* ANSI */
PUBLIC S16 NhLiCkwCfgReq(post, spId, cfgInfo) 
Pst *post;                /* Post structure */
SpId spId;               /* Service provider */
CkwCfgInfo *cfgInfo;
#endif /* ANSI */
{
   TRC3(NhLiCkwCfgReq);
   RETVALUE((*NhLiCkwCfgReqMt[post->selector])(post, spId, cfgInfo));

} /* end of function NhLiCkwCfgReq */

/**
 *
 *      @brief  Sends an UE ID Change Indication to RLC on the CKW interface.
 *      @details Sends an UE ID Change Indication to RLC on the CKW interface.
 *
 */
#ifdef ANSI
PUBLIC S16 NhLiCkwUeIdChgReq
(
Pst       *post,          /* post structure */
SpId       spId,         /* service provider */
U32        transId,      /* RRC Transaction Id */
CkwUeInfo *oldUeInfo,    /* current UE Id */
CkwUeInfo *newUeInfo     /* new UE Id */
)
#else /* ANSI */
PUBLIC S16 NhLiCkwUeIdChgReq(post, spId, transId, oldUeInfo, newUeInfo)
Pst       *post;              /* post structure */
SpId       spId;             /* service provider */
U32        transId;          /* RRC Transaction Id */
CkwUeInfo *oldUeInfo;        /* current UE Id */
CkwUeInfo *newUeInfo;        /* new UE Id */
#endif /* ANSI */
{
  TRC3(NhLiCkwUeIdChgReq);
  RETVALUE((*NhLiCkwUeIdChgReqMt[post->selector])(post, spId, transId,
            (CkwUeInfo *)oldUeInfo, (CkwUeInfo *)newUeInfo ));

} /* end of function NhLiCkwUeIdChgReq */


/**************************************************************************
 * The CRG Interface
 *************************************************************************/
/**
 *      @brief This function binds the RRC service user with the MAC 
 *             service provider (this is the CRG interface).
 *
 *      @details This function binds the RRC service user with the MAC 
 *             service provider (this is the CRG interface). The MAC 
 *             registers this new service user and allocates 
 *             a Service Access Point (SAP) for this bind. The service 
 *             user specifies the reference number that is used 
 *             for the duration of this bind.
 *
 */
#ifdef ANSI
PUBLIC S16 NhLiCrgBndReq
(
Pst *post,                       /* Post structure */
SuId suId,                      /* Service user ID */
SpId spId                       /* Service provider ID */
)
#else /* ANSI */
PUBLIC S16 NhLiCrgBndReq(post, suId, spId)
Pst *post;                       /* Post structure */
SuId suId;                      /* Service user ID */
SpId spId;                      /* Service provider ID */
#endif /* ANSI */
{

   TRC3(NhLiCrgBndReq);
   RETVALUE((*NhLiCrgBndReqMt[post->selector])(post, suId, spId));

} /* end of function NhLiCrgBndReq */


/**
*      @brief  This function unbinds the RRC service user with the MAC 
*             service provider (this is the CRG interface).
*
*      @details This function unbinds the RRC service user with the MAC 
*             service provider (this is the CRG interface).
*/

#ifdef ANSI
PUBLIC S16 NhLiCrgUbndReq
(
Pst *post,                       /* Post structure */
SpId spId,                      /* Service provider ID */
Reason reason                   /* Reason */
)
#else /* ANSI */
PUBLIC S16 NhLiCrgUbndReq(post, spId, reason)
Pst *post;                       /* Post structure */
SpId spId;                      /* Service provider ID */
Reason reason;                  /* Reason */
#endif /* ANSI */
{

   TRC3(NhLiCrgUbndReq);
   RETVALUE((*NhLiCrgUbndReqMt[post->selector])(post, spId, reason));

} /* end of function NhLiCrgUbndReq */


/**
*      @brief Sends a Configuration request to the MAC on the CMK Interface.
*      @details Sends a Configuration request to the MAC on the CMK Interface.
*
*/
#ifdef ANSI
PUBLIC S16 NhLiCrgCfgReq
(
Pst *post,                       /* Post structure */
SpId spId,                      /* Service provider ID */
CrgCfgTransId transId,          /* cfg trans ID */
CrgCfgReqInfo *cmacCfg              /* MAC Configuration information */
)
#else /* ANSI */
PUBLIC S16 NhLiCrgCfgReq(post, spId, transId, cmacCfg)
Pst *post;                       /* Post structure */
SpId spId;                      /* Service provider ID */
CrgCfgTransId transId;          /* cfg trans ID */
CrgCfgReqInfo *cmacCfg;         /* MAC Configuration information */
#endif /* ANSI */
{

   TRC3(NhLiCrgCfgReq);
   RETVALUE((*NhLiCrgCfgReqMt[post->selector])(post, spId, transId, cmacCfg));

} /* end of function NhLiCrgCfgReq */




/**************************************************************************
 * The CPJ Interface
 *************************************************************************/
/**
 *
 *   @brief  This function binds the RRC service user with the PDCP 
 *             service provider.
 *
 *   @details  This function binds the RRC service user with the PDCP 
 *             service provider. The PDCP registers this new service 
 *             user and allocates a Service Access Point (SAP) for 
 *             this bind. The service user specifies the reference 
 *             number that is used for the duration of this bind.
 */
#ifdef ANSI
PUBLIC S16 NhLiCpjBndReq
(
Pst *post,                       /* Post structure */
SuId suId,                      /* Service user ID */
SpId spId                       /* Service provider ID */
)
#else /* ANSI */
PUBLIC S16 NhLiCpjBndReq(post, suId, spId)
Pst *post;                       /* Post structure */
SuId suId;                      /* Service user ID */
SpId spId;                      /* Service provider ID */
#endif /* ANSI */
{

   TRC3(NhLiCpjBndReq);
   RETVALUE((*NhLiCpjBndReqMt[post->selector])(post, suId, spId));

} /* end of function NhLiCpjBndReq */


/*
*      @brief  This function unbinds the RRC service user with the PDCP 
*             service provider. 
*
*      @details This function unbinds the RRC service user with the PDCP 
*             service provider. 
*
*/
#ifdef ANSI
PUBLIC S16 NhLiCpjUbndReq
(
Pst *post,                       /* Post structure */
SpId spId,                      /* Service provider ID */
Reason reason                   /* Reason */
)
#else /* ANSI */
PUBLIC S16 NhLiCpjUbndReq(post, spId, reason)
Pst *post;                       /* Post structure */
SpId spId;                      /* Service provider ID */
Reason reason;                  /* Reason */
#endif /* ANSI */
{

   TRC3(NhLiCpjUbndReq);
   RETVALUE((*NhLiCpjUbndReqMt[post->selector])(post, spId, reason));

} /* end of function NhLiCpjUbndReq */


/**
*
*      @brief  This function is used to pass Signalling Info to PDCP related 
*             to PDCP configuration.
*
*      @details This function is used to pass signaling Info to PDCP related 
*             to PDCP configuration.
*
*/
#ifdef ANSI
PUBLIC S16 NhLiCpjCfgReq
(
Pst *post,                       /* Post structure */
SpId spId,                      /* Service provider ID */
CpjCfgReqInfo *pdCfgReq              /* PDCP Config parameters */
)
#else /* ANSI */
PUBLIC S16 NhLiCpjCfgReq(post, spId, pdCfgReq)
Pst *post;                       /* Post structure */
SpId spId;                      /* Service provider ID */
CpjCfgReqInfo *pdCfgReq;             /* PDCP Config parameters */
#endif /* ANSI */
{

  TRC3(NhLiCpjCfgReq);
  RETVALUE((*NhLiCpjCfgReqMt[post->selector])(post, spId, pdCfgReq));

} /* end of function NhLiCpjCfgReq */

/**
 *
 *      @brief  Sends an UE Id Change Indication to PDCP on Ckw interface
 *      @details Sends an UE Id Change Indication to PDCP on Ckw interface
 *
 */
#ifdef ANSI
PUBLIC S16 NhLiCpjUeIdChgReq
(
Pst       *post,          /* post structure */
SpId       spId,         /* service provider */
U32        transId,      /* RRC Transaction Id */
CpjUeInfo *oldUeInfo,    /* current UE Id */
CpjUeInfo *newUeInfo     /* new UE Id */
)
#else /* ANSI */
PUBLIC S16 NhLiCpjUeIdChgReq(post, spId, transId, oldUeInfo, newUeInfo)
Pst       *post;              /* post structure */
SpId       spId;             /* service provider */
U32        transId;          /* RRC Transaction Id */
CpjUeInfo *oldUeInfo;        /* current UE Id */
CpjUeInfo *newUeInfo;        /* new UE Id */
#endif /* ANSI */
{
  TRC3(NhLiCpjUeIdChgReq);
  RETVALUE((*NhLiCpjUeIdChgReqMt[post->selector])(post, spId, transId,
            (CpjUeInfo *)oldUeInfo, (CpjUeInfo *)newUeInfo ));

} /* end of function NhLiCpjUeIdChgReq */

/**
*
*      @brief  This function is used to configure PDCP security parameters 
*              (parametrs of Integrity protection and ciphering)
*
*      @details  This function is used to configure PDCP security parameters 
*              (parametrs of Integrity protection and ciphering)
*
*/
#ifdef ANSI
PUBLIC S16 NhLiCpjSecCfgReq
(
Pst *post,                                /* Post structure */
SpId spId,                               /* Service provider ID */
CpjSecCfgReqInfo *cpjSecConfigReqInfo    /* PDCP Security Config parameters */
)
#else /* ANSI */
PUBLIC S16 NhLiCpjSecCfgReq(post, spId, cpjSecConfigReqInfo)
Pst *post;                                /* Post structure */
SpId spId;                               /* Service provider ID */
CpjSecCfgReqInfo *cpjSecConfigReqInfo;   /* PDCP Security Config parameters */
#endif /* ANSI */
{

  TRC3(NhLiCpjSecCfgReq);
  RETVALUE((*NhLiCpjSecCfgReqMt[post->selector])(post, spId, 
                            cpjSecConfigReqInfo));

} /* end of function NhLiCpjSecCfgReq */

/**
*
*      @brief  This function is used to get COUNT values of each established DRB 
*
*      @details  This function is used to get COUNT values of each established DRB 
*
*/
#ifdef ANSI
PUBLIC S16 NhLiCpjCountReq
(
Pst *post,                       /* Post structure */
SpId spId,                      /* Service provider ID */
CpjCountReqInfo *pdcpCountReq              /* PDCP COUNT request paramters */
)
#else /* ANSI */
PUBLIC S16 NhLiCpjCountReq(post, spId, pdcpCountReq)
Pst *post;                       /* Post structure */
SpId spId;                      /* Service provider ID */
CpjCountReqInfo *pdcpCountReq;              /* PDCP COUNT request paramters */
#endif /* ANSI */
{

  TRC3(NhLiCpjCountReq);
  RETVALUE((*NhLiCpjCountReqMt[post->selector])(post, spId, pdcpCountReq));

} /* end of function NhLiCpjCountReq */

/**
*
*      @brief  This function is used to get PDCP Sdu Status for all DRBs.
*
*      @details  This function is used to get PDCP Sdu Status for all DRBs
*                from PDCP at source during HO.
*
*/
#ifdef ANSI
PUBLIC S16 NhLiCpjSduStaReq
(
Pst *post,                       /* Post structure */
SpId spId,                      /* Service provider ID */
CpjSduStaReqInfo *pdcpSduStaReq     /* PDCP SDU status request parameters */
)
#else /* ANSI */
PUBLIC S16 NhLiCpjSduStaReq(post, spId, pdcpSduStaReq)
Pst *post;                       /* Post structure */
SpId spId;                      /* Service provider ID */
CpjSduStaReqInfo *pdcpSduStaReq;    /* PDCP SDU status request parameters */
#endif /* ANSI */
{

  TRC3(NhLiCpjSduStaReq);
  RETVALUE((*NhLiCpjSduStaReqMt[post->selector])(post, spId, pdcpSduStaReq));

} /* end of function NhLiCpjSduStaReq */

/**
*
*      @brief  This function is used to get PDCP Data Fwd
*
*      @details  This function is used to get PDCP Data Forward
*                to PDCP at source during HO.
*
*/
#ifdef ANSI
PUBLIC S16 NhLiCpjStartDataFwdReq
(
Pst *post,                       /* Post structure */
SpId spId,                      /* Service provider ID */
CpjDataFwdReqInfo *pdcpDataFwdReq     /* PDCP Data Forward request parameters */
)
#else /* ANSI */
PUBLIC S16 NhLiCpjStartDataFwdReq(post, spId, pdcpDataFwdReq)
Pst *post;                       /* Post structure */
SpId spId;                      /* Service provider ID */
CpjDataFwdReqInfo *pdcpDataFwdReq;    /* PDCP Data Fwd request parameters */
#endif /* ANSI */
{

  TRC3(NhLiCpjStartDataFwdReq);

  RETVALUE((*NhLiCpjStartDataFwdReqMt[post->selector])(post, spId, pdcpDataFwdReq));

} /* end of function NhLiCpjStartDataFwdReq */


/**
*
*      @brief  This function is used to get PDCP Data Resume
*
*      @details  This function is used to get PDCP Data Resume
*                to PDCP at during Restablismet/HO.
*
*/
#ifdef ANSI
PUBLIC S16 NhLiCpjDatResumeReq
(
Pst *post,                       /* Post structure */
SpId spId,                      /* Service provider ID */
CpjDatResumeReqInfo *pdcpDatResumeReq     /* PDCP Data Resume request parameters */
)
#else /* ANSI */
PUBLIC S16 NhLiCpjDatResumeReq(post, spId, pdcpDatResumeReq)
Pst *post;                       /* Post structure */
SpId spId;                      /* Service provider ID */
CpjDatResumeReqInfo *pdcpDatResumeReq;    /* PDCP Data Resume request parameters */
#endif /* ANSI */
{

  TRC3(NhLiCpjDatResumeReq);

  RETVALUE((*NhLiCpjDatResumeReqMt[post->selector])(post, spId, pdcpDatResumeReq));

} /* end of function NhLiCpjDatResumeReq */


/**
 *
 *      @brief  Sends a Re-establish request to PDCP on CPJ interface
 *      @details Sends a Re-establish request to PDCP on CPJ interface 
 *
 */
#ifdef ANSI
PUBLIC S16 NhLiCpjReEstReq
(
Pst       *post,          /* post structure */
SpId       spId,         /* service provider */
CpjReEstReqInfo *restbInfo     /* Re-establishment Info */
)
#else /* ANSI */
PUBLIC S16 NhLiCpjReEstReq(post, spId, restbInfo)
Pst       *post;              /* post structure */
SpId       spId;             /* service provider */
CpjReEstReqInfo *restbInfo;        /* new UE Id */
#endif /* ANSI */
{
  TRC3(NhLiCpjReEstReq);

  RETVALUE((*NhLiCpjReEstReqMt[post->selector])(post, spId, restbInfo));

} /* end of function NhLiCpjReEstReq */

#ifdef RRC_PHY_CFG_SUPP
/**************************************************************************
 * The CTF Interface
 *************************************************************************/
/**
 * @brief Request from RRC to PHY to bind the CTF interface sap.
 *
 * @details This function binds the RRC service user with the PHY 
 *          service provider (this is the CTF interface). The PHY 
 *          registers this new service user and allocates 
 *          a Service Access Point (SAP) for this bind. The service 
 *          user specifies the reference number that is used 
 *          for the duration of this bind.
 *
 *     Function : NhLiCtfBndReq
 *
 *  @param[in]   Pst*  post
 *  @param[in]   SuId  suId
 *  @param[in]   SpId  spId
 *  @return   S16
 *      -# ROK
 *
 */
#ifdef ANSI
PUBLIC S16 NhLiCtfBndReq
(
Pst* post,
SuId suId,
SpId spId
)
#else /* ANSI */
PUBLIC S16 NhLiCtfBndReq(post, suId, spId)
Pst* post;
SuId suId;
SpId spId;
#endif /* ANSI */
{

   TRC3(NhLiCtfBndReq);
   RETVALUE((*NhLiCtfBndReqMt[post->selector])(post, suId, spId));

} /* end of function NhLiCtfBndReq */


/**
 * @brief  Request from RRC to PHY to Unbind the CTF interface sap.
 *
 * @details This function unbinds the RRC service user with the PHY 
 *          service provider (this is the CTF interface).
 *
 *     Function : NhLiCtfUbndReq
 *
 *  @param[in]   Pst*  post
 *  @param[in]   SpId  spId
 *  @param[in]   Reason  reason
 *  @return   S16
 *      -# ROK
 */
#ifdef ANSI
PUBLIC S16 NhLiCtfUbndReq
(
Pst* post,
SpId spId,
Reason reason
)
#else /* ANSI */
PUBLIC S16 NhLiCtfUbndReq(post, spId, reason)
Pst* post;
SpId spId;
Reason reason;
#endif /* ANSI */
{

   TRC3(NhLiCtfUbndReq);
   RETVALUE((*NhLiCtfUbndReqMt[post->selector])(post, spId, reason));

} /* end of function NhLiCtfUbndReq */


/**
 * @brief Configuration Request from RRC to PHY for 
 * cell or dedicated configuration.
 *
 * @details Sends a Configuration request to the PHY on the CTF Interface.
 *
 *     Function : NhLiCtfCfgReq
 *
 *  @param[in]   Pst*  post
 *  @param[in]   SpId  spId
 *  @param[in]   CtfCfgTransId  transId
 *  @param[in]   CtfCfgReqInfo*  cfgReqInfo
 *  @return   S16
 *      -# ROK
 *
 */
#ifdef ANSI
PUBLIC S16 NhLiCtfCfgReq
(
Pst *post,                       /* Post structure */
SpId spId,                      /* Service provider ID */
CtfCfgTransId transId,          /* cfg trans ID */
CtfCfgReqInfo *cfgReqInfo       /* MAC Configuration information */
)
#else /* ANSI */
PUBLIC S16 NhLiCtfCfgReq(post, spId, transId, cfgReqInfo)
Pst *post;                       /* Post structure */
SpId spId;                      /* Service provider ID */
CtfCfgTransId transId;          /* cfg trans ID */
CtfCfgReqInfo *cfgReqInfo;      /* PHY Configuration information */
#endif /* ANSI */
{
   TRC3(NhLiCtfCfgReq);
   RETVALUE((*NhLiCtfCfgReqMt[post->selector])(post, spId, transId, 
                             cfgReqInfo));

} /* end of function NhLiCtfCfgReq */


/**
* @brief UE ID change Request from RRC to PHY.
*
* @details
*
*     Function : NhLiCtfUeIdChgReq
*
*  @param[in]   Pst*  post
*  @param[in]   SpId  spId
*  @param[in]   CtfCfgTransId  transId
*  @param[in]   CtfUeInfo *  ueInfo
*  @param[in]   CtfUeInfo *  newUeInfo
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 NhLiCtfUeIdChgReq
(
Pst* post,
SpId spId,
CtfCfgTransId transId,
CtfUeInfo * ueInfo,
CtfUeInfo * newUeInfo
)
#else /* ANSI */
PUBLIC S16 NhLiCtfUeIdChgReq(post, spId, transId, ueInfo, newUeInfo)
Pst* post;
SpId spId;
CtfCfgTransId transId;
CtfUeInfo * ueInfo;
CtfUeInfo * newUeInfo;
#endif /* ANSI */
{

   TRC3(NhLiCtfUeIdChgReq)
   RETVALUE((*NhLiCtfUeIdChgReqMt[post->selector])(post, spId, transId, ueInfo, 
                                 newUeInfo));

} /* end of NhLiCtfUeIdChgReq */


#endif /* RRC_PHY_CFG_SUPP */


/**************************************************************************
 * The PJU Interface
 *************************************************************************/
/**
 *
 *      @brief  This function binds the RRC service user with the PDCP 
 *             service provider.
 *
 *      @details  This function binds the RRC service user with the PDCP 
 *             service provider. The PDCP registers this new service 
 *             user and allocates a Service Access Point (SAP) for 
 *             this bind. The service user specifies the reference 
 *             number that is used for the duration of this bind.
 *
 *
 *
 *     @param[in] post            Post Structure
 *     @param[in] suId            Service User ID
 *     @param[in] spId            Service Provider ID
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/


#ifdef ANSI
PUBLIC S16 NhLiPjuBndReq
(
Pst *post,                       /* Post structure */
SuId suId,                      /* Service user ID */
SpId spId                       /* Service provider ID */
)
#else /* ANSI */
PUBLIC S16 NhLiPjuBndReq(post, suId, spId)
Pst *post;                       /* Post structure */
SuId suId;                      /* Service user ID */
SpId spId;                      /* Service provider ID */
#endif /* ANSI */
{

   TRC3(NhLiPjuBndReq);
   RETVALUE((*NhLiPjuBndReqMt[post->selector])(post, suId, spId));

} /* end of function NhLiPjuBndReq */


/**
*
*      @brief This function unbinds the RRC service user with the PDCP 
*             service provider. 
*
*      @details  This function unbinds the RRC service user with the PDCP 
*             service provider. 
*
*     @param[in] post            Post Structure
*     @param[in] spId            Service Provider ID
*     @param[in] reason          Reason
*
*  @return  S16
*      -# ROK
*      -# RFAILED
**/
#ifdef ANSI
PUBLIC S16 NhLiPjuUbndReq
(
Pst *post,                       /* Post structure */
SpId spId,                      /* Service provider ID */
Reason reason                   /* Reason */
)
#else /* ANSI */
PUBLIC S16 NhLiPjuUbndReq(post, spId, reason)
Pst *post;                       /* Post structure */
SpId spId;                      /* Service provider ID */
Reason reason;                  /* Reason */
#endif /* ANSI */
{

   TRC3(NhLiPjuUbndReq);
   RETVALUE((*NhLiPjuUbndReqMt[post->selector])(post, spId, reason));

} /* end of function NhLiPjuUbndReq */


/**
*
*      @brief This function is used to pass Signalling Info to PDCP related 
*             to PDCP configuration.
*
*      @details This function is used to pass signaling Info to PDCP related 
*             to PDCP configuration.
*
*     @param[in] post            Post Structure
*     @param[in] spId            Service Provider ID
*     @param[in] pdcpId          PDCP ID
*     @param[in] sduID           SDU ID
*     @param[in] mBuf            Buffer
*
*  @return  S16
*      -# ROK
*      -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 NhLiPjuDatReq
(
Pst *post,                       /* Post structure */
SpId spId,                      /* Service provider ID */
CmLtePdcpId *pdcpId,
PjuSduId sduId,
Buffer *mBuf
)
#else /* ANSI */
PUBLIC S16 NhLiPjuDatReq(post, spId, pdcpId, sduId, mBuf)
Pst *post;                       /* Post structure */
SpId spId;                      /* Service provider ID */
CmLtePdcpId *pdcpId;
PjuSduId sduId;
Buffer *mBuf;
#endif /* ANSI */
{

   TRC3(NhLiPjuDatReq);
   RETVALUE((*NhLiPjuDatReqMt[post->selector])(post, spId, pdcpId, 
                             sduId, mBuf));

} /* end of function NhLiPjuDatReq */


/**************************************************************************
 * RRC Lower Interface (Portable) primitives
 *************************************************************************/

#ifdef PTNHLIKWU
/**************************************************************************
 * The KWU (Portable) Interface
 *************************************************************************/

/**
 *
 *      @brief  Portable KwuBndReq
 */

#ifdef ANSI
PRIVATE S16 PtLiKwuBndReq
(
Pst *post,                      /* Post structure */
SuId suId,                      /* Service user ID */
SpId spId                       /* Service provider ID */
)
#else /* ANSI */
PRIVATE S16 PtLiKwuBndReq(post, suId, spId)
Pst *post;                      /* Post structure */
SuId suId;                      /* Service user ID */
SpId spId;                      /* Service provider ID */
#endif /* ANSI */
{

   TRC3(PtLiKwuBndReq);

   UNUSED(post);
   UNUSED(suId);
   UNUSED(spId);

   RLOG0(L_DEBUG, "PtLiKwuBndReq() called");

   RETVALUE(ROK);

} /* end of function PtLiKwuBndReq */


/**
*
*      @brief Portable KwuUbndReq
*
*/
#ifdef ANSI
PRIVATE S16 PtLiKwuUbndReq
(
Pst *post,                      /* Post structure */
SpId spId,                      /* Service provider ID */
Reason reason                   /* Reason */
)
#else /* ANSI */
PRIVATE S16 PtLiKwuUbndReq(post, spId, reason)
Pst *post;                      /* Post structure */
SpId spId;                      /* Service provider ID */
Reason reason;                  /* Reason */
#endif /* ANSI */
{

   TRC3(PtLiKwuUbndReq);

   UNUSED(post);
   UNUSED(spId);
   UNUSED(reason);

   RLOG0(L_DEBUG, "PtLiKwuUbndReq() called");

   RETVALUE(ROK);

} /* end of function PtLiKwuUbndReq */


/**
 *
 *     @brief  
 *
 *     @details
 *
 *
 *     @param[in] post            Post Structure
 *     @param[in] spId            Service Provider ID
 *     @param[in] rlDatReqParam   RLC Data Request Parameters
 *     @param[in] datBuf          Data Buffer
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 PtLiKwuDatReq
(
Pst *post,                      /* Post structure */
SpId spId,                      /* Service provider ID */
KwuDatReqInfo *rlDatReqParam,   /* Data Request parameters */
Buffer *datBuf                  /* Data Buffer */
)
#else /* ANSI */
PRIVATE S16 PtLiKwuDatReq(post, spId, rlDatReqParam, datBuf)
Pst *post;                      /* Post structure */
SpId spId;                      /* Service provider ID */
KwuDatReqInfo *rlDatReqParam;   /* Data Request parameters */
Buffer *datBuf;                 /* Data Buffer */
#endif /* ANSI */
{

   TRC3(PtLiKwuDatReq);

   UNUSED(post);
   UNUSED(spId);
   UNUSED(rlDatReqParam);
   UNUSED(datBuf);

   RLOG0(L_DEBUG, "PtLiKwuDatReq() called");

   RETVALUE(ROK);

} /* end of function PtLiKwuDatReq */

#endif /* PTNHLIKWU */
  
#ifdef PTNHLICKW
/**************************************************************************
 * The CKW (Portable) Interface
 *************************************************************************/

/**************************************************************************
 *
 *      Fun:   PtLiCkwBndReq
 *
 *      Desc:  Portable CkwBndReq
 *
 *      Ret:   ROK 
 *
 *      Notes: 
 *
 *      File:  nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtLiCkwBndReq
(
Pst *post,                      /* Post structure */
SuId suId,                      /* Service user ID */
SpId spId                       /* Service provider ID */
)
#else /* ANSI */
PRIVATE S16 PtLiCkwBndReq(post, suId, spId)
Pst *post;                      /* Post structure */
SuId suId;                      /* Service user ID */
SpId spId;                      /* Service provider ID */
#endif /* ANSI */
{

   TRC3(PtLiCkwBndReq);

   UNUSED(post);
   UNUSED(suId);
   UNUSED(spId);

   RLOG0(L_DEBUG, "PtLiCwuBndReq() called");

   RETVALUE(ROK);

} /* end of function PtLiCkwBndReq */


/**************************************************************************
*
*      Fun:   PtLiCkwUbndReq
*
*      Desc:  Portable CkwUbndReq primitive
*
*      Ret:   ROK 
*
*      Notes: 
*
*      File:  nh_ptli.c
*
*************************************************************************/
#ifdef ANSI
PRIVATE S16 PtLiCkwUbndReq
(
Pst *post,                      /* Post structure */
SpId spId,                      /* Service provider ID */
Reason reason                   /* Reason */
)
#else /* ANSI */
PRIVATE S16 PtLiCkwUbndReq(post, spId, reason)
Pst *post;                      /* Post structure */
SpId spId;                      /* Service provider ID */
Reason reason;                  /* Reason */
#endif /* ANSI */
{

   TRC3(PtLiCkwUbndReq);
   UNUSED(post);
   UNUSED(spId);
   UNUSED(reason);

   RLOG0(L_DEBUG, "PtLiCwuUbndReq() called");

   RETVALUE(ROK);

} /* end of function PtLiCkwUbndReq */


/**************************************************************************
 *
 *      Fun:   PtLiCkwCfgReq
 *
 *      Desc:  Portable CkwCfgReq primitive
 *
 *      Ret:   ROK 
 *
 *      Notes: 
 *
 *      File:  nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtLiCkwCfgReq
(
Pst *post,                      /* Post structure */
SpId spId,                      /* Service provider ID */
CkwCfgInfo *cfgInfo
)
#else /* ANSI */
PRIVATE S16 PtLiCkwCfgReq(post, spId, cfgInfo)
Pst *post;                      /* Post structure */
SpId spId;                      /* Service provider ID */
CkwCfgInfo *cfgInfo;
#endif /* ANSI */
{
  TRC3(PtLiCkwCfgReq);

  UNUSED(post);
  UNUSED(spId);
  UNUSED(cfgInfo);

  RLOG0(L_DEBUG, "PtLiCwuCfgReq() called");

  RETVALUE(ROK);

} /* end of function PtLiCkwCfgReq */

/**************************************************************************
 *
 *      Fun:   PtLiCkwUeIdChgReq
 *
 *      Desc:  Portable CkwUeIdChgReq primitive
 *
 *      Ret:   ROK 
 *
 *      Notes: 
 *
 *      File:  nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtLiCkwUeIdChgReq
(
Pst       *post,           /* post structure */
SpId      spId,            /* service provider */
U32       transId,         /* TransId */
CkwUeInfo *oldUeInfo,      /* current UE Id */
CkwUeInfo *newUeInfo       /* new UE Id */
)
#else /* ANSI */
PRIVATE S16 PtLiCkwUeIdChgReq(post, spId, transId, oldUeInfo, newUeInfo)
Pst       *post;           /* post structure */
SpId       spId;           /* service provider */
U32        transId;        /* TransId */
CkwUeInfo *oldUeInfo;      /* current UE Id */
CkwUeInfo *newUeInfo;      /* new UE Id */
#endif /* ANSI */
{
  TRC3(PtLiCkwUeIdChgReq);

  UNUSED(post);
  UNUSED(spId);
  UNUSED(transId);
  UNUSED(oldUeInfo);
  UNUSED(newUeInfo);

  RLOG0(L_DEBUG, "PtLiCwuUeIdChgReq() called");

  RETVALUE(ROK);

} /* end of function PtLiCkwUeIdChgReq */
#endif /* PTNHLICKW */


#ifdef PTNHLICRG
/**************************************************************************
 * The CRG (Portable) Interface
 *************************************************************************/
/**************************************************************************
 *
 *      Fun:   PtLiCrgBndReq
 *
 *      Desc:  Portable CrgBndReq primitive
 *
 *      Ret:   ROK 
 *
 *      Notes: 
 *
 *      File:  nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtLiCrgBndReq
(
Pst *post,                      /* Post structure */
SuId suId,                      /* Service user ID */
SpId spId                       /* Service provider ID */
)
#else /* ANSI */
PRIVATE S16 PtLiCrgBndReq(post, suId, spId)
Pst *post;                      /* Post structure */
SuId suId;                      /* Service user ID */
SpId spId;                      /* Service provider ID */
#endif /* ANSI */
{

   TRC3(PtLiCrgBndReq);

   UNUSED(post);
   UNUSED(suId);
   UNUSED(spId);

   RLOG0(L_DEBUG, "PtLiCrgBndReq() called");

   RETVALUE(ROK);

} /* end of function PtLiCrgBndReq */


/**************************************************************************
*
*      Fun:   PtLiCrgUbndReq
*
*      Desc:  Portable CrgUbndReq primitive
*
*      Ret:   ROK 
*
*      Notes: 
*
*      File:  nh_ptli.c
*
*************************************************************************/
#ifdef ANSI
PRIVATE S16 PtLiCrgUbndReq
(
Pst *post,                      /* Post structure */
SpId spId,                      /* Service provider ID */
Reason reason                   /* Reason */
)
#else /* ANSI */
PRIVATE S16 PtLiCrgUbndReq(post, spId, reason)
Pst *post;                      /* Post structure */
SpId spId;                      /* Service provider ID */
Reason reason;                  /* Reason */
#endif /* ANSI */
{

   TRC3(PtLiCrgUbndReq);

   UNUSED(post);
   UNUSED(spId);
   UNUSED(reason);

   RLOG0(L_DEBUG, "PtLiCrgUbndReq() called");

   RETVALUE(ROK);

} /* end of function PtLiCrgUbndReq */


/**************************************************************************
*
*      Fun:   PtLiCrgCfgReq
*
*      Desc:  Portable CrgCfgReq
*
*      Ret:   ROK 
*
*      Notes: 
*
*      File:  nh_ptli.c
*
*************************************************************************/
#ifdef ANSI
PRIVATE S16 PtLiCrgCfgReq
(
Pst *post,
SpId spId,
CrgCfgTransId transId,          /* cfg trans ID */
CrgCfgReqInfo *cmacCfg         /* MAC Configuration information */
)
#else /* ANSI */
PRIVATE S16 PtLiCrgCfgReq(post, spId, transId, cmacCfg)
Pst *post;
SpId spId;
CrgCfgTransId transId;          /* cfg trans ID */
CrgCfgReqInfo *cmacCfg;         /* MAC Configuration information */
#endif /* ANSI */
{

  TRC3(PtLiCrgCfgReq);

  UNUSED(post);
  UNUSED(spId);
  UNUSED(transId);
  UNUSED(cmacCfg);

  RLOG0(L_DEBUG, "PtLiCrgCfgReq() called");

  RETVALUE(ROK);

} /* end of function PtLiCrgCfgReq */


#endif /* PTNHLICRG */

/**************************************************************************
 * The CPJ (Portable) Interface
 *************************************************************************/
#ifdef PTNHLICPJ
/**************************************************************************
 *
 *      Fun:   PtLiCpjBndReq
 *
 *      Desc:  Portable CpjBndReq primitive
 *
 *      Ret:   ROK 
 *
 *      Notes: 
 *
 *      File:  nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtLiCpjBndReq
(
Pst *post,                      /* Post structure */
SuId suId,                      /* Service user ID */
SpId spId                       /* Service provider ID */
)
#else /* ANSI */
PRIVATE S16 PtLiCpjBndReq(post, suId, spId)
Pst *post;                      /* Post structure */
SuId suId;                      /* Service user ID */
SpId spId;                      /* Service provider ID */
#endif /* ANSI */
{

   TRC3(PtLiCpjBndReq);

   UNUSED(post);
   UNUSED(suId);
   UNUSED(spId);

   RLOG0(L_DEBUG, "PtLiCpjBndReq() called");

   RETVALUE(ROK);

} /* end of function PtLiCpjBndReq */


/**************************************************************************
*
*      Fun:   PtLiCpjUbndReq
*
*      Desc:  Portable CpjUbndReq primitive
*
*      Ret:   ROK 
*
*      Notes: 
*
*      File:  nh_ptli.c
*
*************************************************************************/
#ifdef ANSI
PRIVATE S16 PtLiCpjUbndReq
(
Pst *post,                      /* Post structure */
SpId spId,                      /* Service provider ID */
Reason reason                   /* Reason */
)
#else /* ANSI */
PRIVATE S16 PtLiCpjUbndReq(post, spId, reason)
Pst *post;                      /* Post structure */
SpId spId;                      /* Service provider ID */
Reason reason;                  /* Reason */
#endif /* ANSI */
{

   TRC3(PtLiCpjUbndReq);

   UNUSED(post);
   UNUSED(spId);
   UNUSED(reason);

   RLOG0(L_DEBUG, "PtLiCpjUbndReq() called");

   RETVALUE(ROK);

} /* end of function PtLiCpjUbndReq */


/**************************************************************************
*
*      Fun:   PtLiCpjCfgReq
*
*      Desc:  Portable CpjCfgReq primitive
*
*      Ret:   ROK 
*
*      Notes: 
*
*      File:  nh_ptli.c
*
*************************************************************************/
#ifdef ANSI
PRIVATE S16 PtLiCpjCfgReq
(
Pst *post,                      /* Post structure */
SpId spId,                      /* Service provider ID */
CpjCfgReqInfo *pdCfgReq         /* PDCP Config parameters */
)
#else /* ANSI */
PRIVATE S16 PtLiCpjCfgReq(post, spId, pdCfgReq)
Pst *post;                      /* Post structure */
SpId spId;                      /* Service provider ID */
CpjCfgReqInfo *pdCfgReq;        /* PDCP Config parameters */
#endif /* ANSI */
{

   TRC3(PtLiCpjCfgReq);

   UNUSED(post);
   UNUSED(spId);
   UNUSED(pdCfgReq);

   RLOG0(L_DEBUG, "PtLiCpjCfgReq() called");

   RETVALUE(ROK);

} /* end of function PtLiCpjCfgReq */

/**************************************************************************
 *
 *      Fun:   PtLiCpjUeIdChgReq
 *
 *      Desc:  Portable CpjUeIdChgReq primitive
 *
 *      Ret:   ROK 
 *
 *      Notes: 
 *
 *      File:  nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtLiCpjUeIdChgReq
(
Pst       *post,           /* post structure */
SpId      spId,            /* service provider */
U32       transId,         /* TransId */
CpjUeInfo *oldUeInfo,      /* current UE Id */
CpjUeInfo *newUeInfo       /* new UE Id */
)
#else /* ANSI */
PRIVATE S16 PtLiCpjUeIdChgReq(post, spId, transId, oldUeInfo, newUeInfo)
Pst       *post;           /* post structure */
SpId       spId;           /* service provider */
U32        transId;        /* TransId */
CpjUeInfo *oldUeInfo;      /* current UE Id */
CpjUeInfo *newUeInfo;      /* new UE Id */
#endif /* ANSI */
{
  TRC3(PtLiCpjUeIdChgReq);

  UNUSED(post);
  UNUSED(spId);
  UNUSED(transId);
  UNUSED(oldUeInfo);
  UNUSED(newUeInfo);

   RLOG0(L_DEBUG, "PtLiCpjUeIdChgReq() called");

  RETVALUE(ROK);

} /* end of function PtLiCkwUeIdChgReq */

/**************************************************************************
 *
 *      Fun:   PtLiCpjSecCfgReq
 *
 *      Desc:  Portable CpjSecCfgReq primitive
 *
 *      Ret:   ROK 
 *
 *      Notes: 
 *
 *      File:  nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtLiCpjSecCfgReq
(
Pst       *post,                  /* post structure */
SpId      spId,                   /* service provider */
CpjSecCfgReqInfo *secCfgReq       /* PDCP Security Config paramters */
)
#else /* ANSI */
PRIVATE S16 PtLiCpjSecCfgReq(post, spId, secCfgReq)
Pst       *post;                  /* post structure */
SpId       spId;                  /* service provider */
CpjSecCfgReqInfo *secCfgReq;      /* PDCP Security Config paramters */
#endif /* ANSI */
{
  TRC3(PtLiCpjSecCfgReq);

  UNUSED(post);
  UNUSED(spId);
  UNUSED(secCfgReq);

  RLOG0(L_DEBUG, "PtLiCpjSecCfgReq() called");

  RETVALUE(ROK);

} /* end of function PtLiCpjSecCfgReq */

/**************************************************************************
 *
 *      Fun:   PtLiCpjCountReq
 *
 *      Desc:  Portable CpjCountReq primitive
 *
 *      Ret:   ROK 
 *
 *      Notes: 
 *
 *      File:  nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtLiCpjCountReq
(
Pst       *post,                /* post structure */
SpId      spId,                 /* service provider */
CpjCountReqInfo *countReq       /* PDCP Security Config paramters */
)
#else /* ANSI */
PRIVATE S16 PtLiCpjCountReq(post, spId, countReq)
Pst       *post;                /* post structure */
SpId       spId;                /* service provider */
CpjCountReqInfo *countReq;      /* Count value paramters */
#endif /* ANSI */
{
  TRC3(PtLiCpjCountReq);

  UNUSED(post);
  UNUSED(spId);
  UNUSED(countReq);

  RLOG0(L_DEBUG, "PtLiCpjCountReq() called");

  RETVALUE(ROK);

} /* end of function PtLiCpjCountReq */


/**************************************************************************
 *
 *      Fun:   PtLiCpjStartDataFwdReq
 *
 *      Desc:  Portable CpjDataFwdReq primitive
 *
 *      Ret:   ROK 
 *
 *      Notes: 
 *
 *      File:  nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtLiCpjStartDataFwdReq
(
Pst          *post,               /* post structure */
SpId         spId,                /* service provider */
CpjDataFwdReqInfo *dataFwdReq       /* PDCP Data Forward parameters */
)
#else /* ANSI */
PRIVATE S16 PtLiCpjStartDataFwdReq(post, spId, dataFwdReq)
Pst          *post;               /* post structure */
SpId         spId;                /* service provider */
CpjDataFwdReqInfo *dataFwdReq;      /* PDCP Data Forward parameters */
#endif /* ANSI */
{
  TRC3(PtLiCpjStartDataFwdReq);

  UNUSED(post);
  UNUSED(spId);
  UNUSED(dataFwdReq);
  RETVALUE(ROK);

} /* end of function PtLiCpjStartDataFwdReq */


/**************************************************************************
 *
 *      Fun:   PtLiCpjDatResumeReq
 *
 *      Desc:  Portable CpjDatResumeReq primitive
 *
 *      Ret:   ROK 
 *
 *      Notes: 
 *
 *      File:  nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtLiCpjDatResumeReq
(
Pst          *post,               /* post structure */
SpId         spId,                /* service provider */
CpjDatResumeReqInfo *dataResReq       /* PDCP Data Resume parameters */
)
#else /* ANSI */
PRIVATE S16 PtLiCpjDatResumeReq(post, spId, dataResReq)
Pst          *post;               /* post structure */
SpId         spId;                /* service provider */
CpjDatResumeReqInfo *dataResReq;      /* PDCP Data Resume parameters */
#endif /* ANSI */
{
  TRC3(PtLiCpjDatResumeReq);

  UNUSED(post);
  UNUSED(spId);
  UNUSED(dataResReq);
  RETVALUE(ROK);

} /* end of function PtLiCpjStartDatResumeReq */
/**************************************************************************
 *
 *      Fun:   PtLiCpjSduStaReq
 *
 *      Desc:  Portable CpjSduStaReq primitive
 *
 *      Ret:   ROK 
 *
 *      Notes: 
 *
 *      File:  nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtLiCpjSduStaReq
(
Pst          *post,               /* post structure */
SpId         spId,                /* service provider */
CpjSduStaReqInfo *sduStaReq       /* PDCP SDU status parameters */
)
#else /* ANSI */
PRIVATE S16 PtLiCpjSduStaReq(post, spId, sduStaReq)
Pst          *post;               /* post structure */
SpId         spId;                /* service provider */
CpjSduStaReqInfo *sduStaReq;      /* PDCP SDU status parameters */
#endif /* ANSI */
{
  TRC3(PtLiCpjSduStaReq);

  UNUSED(post);
  UNUSED(spId);
  UNUSED(sduStaReq);
  RETVALUE(ROK);

} /* end of function PtLiCpjSduStaReq */


/**************************************************************************
 *
 *      Fun:   PtLiCpjReEstReq
 *
 *      Desc:  Portable CpjReEstReq primitive
 *
 *      Ret:   ROK 
 *
 *      Notes: 
 *
 *      File:  nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtLiCpjReEstReq
(
Pst       *post,                 /* post structure */
SpId      spId,                  /* service provider */
CpjReEstReqInfo *restbInfo       /* PDCP Re-establishment Config paramters */
)
#else /* ANSI */
PRIVATE S16 PtLiCpjReEstReq(post, spId, restbInfo)
Pst       *post;                 /* post structure */
SpId       spId;                 /* service provider */
CpjReEstReqInfo *restbInfo;      /* PDCP Re-establishment Config paramters */
#endif /* ANSI */
{
  TRC3(PtLiCpjReEstReq);

  UNUSED(post);
  UNUSED(spId);
  UNUSED(restbInfo);
  RETVALUE(ROK);

} /* end of function PtLiCpjReEstReq */
#endif /* #ifdef PTNHLICPJ */

#ifdef RRC_PHY_CFG_SUPP

#ifdef PTNHLICTF
/**************************************************************************
 * The CTF (Portable) Interface
 *************************************************************************/
/**************************************************************************
 *
 *      Fun:   PtLiCtfBndReq
 *
 *      Desc:  Portable CtfBndReq primitive
 *
 *      Ret:   ROK 
 *
 *      Notes: 
 *
 *      File:  nh_ptli.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 PtLiCtfBndReq
(
Pst *post,                      /* Post structure */
SuId suId,                      /* Service user ID */
SpId spId                       /* Service provider ID */
)
#else /* ANSI */
PUBLIC S16 PtLiCtfBndReq(post, suId, spId)
Pst *post;                      /* Post structure */
SuId suId;                      /* Service user ID */
SpId spId;                      /* Service provider ID */
#endif /* ANSI */
{

   TRC3(PtLiCtfBndReq);

   UNUSED(post);
   UNUSED(suId);
   UNUSED(spId);

   RLOG0(L_DEBUG, "PtLiCtfBndReq() called");

   RETVALUE(ROK);

} /* end of function PtLiCtfBndReq */


/**************************************************************************
*
*      Fun:   PtLiCtfUbndReq
*
*      Desc:  Portable CtfUbndReq primitive
*
*      Ret:   ROK 
*
*      Notes: 
*
*      File:  nh_ptli.c
*
*************************************************************************/
#ifdef ANSI
PUBLIC S16 PtLiCtfUbndReq
(
Pst *post,                      /* Post structure */
SpId spId,                      /* Service provider ID */
Reason reason                   /* Reason */
)
#else /* ANSI */
PUBLIC S16 PtLiCtfUbndReq(post, spId, reason)
Pst *post;                      /* Post structure */
SpId spId;                      /* Service provider ID */
Reason reason;                  /* Reason */
#endif /* ANSI */
{

   TRC3(PtLiCtfUbndReq);

   UNUSED(post);
   UNUSED(spId);
   UNUSED(reason);

   RLOG0(L_DEBUG, "PtLiCtfUbndReq() called");

   RETVALUE(ROK);

} /* end of function PtLiCtfUbndReq */


/**************************************************************************
*
*      Fun:   PtLiCtfCfgReq
*
*      Desc:  Portable CtfCfgReq
*
*      Ret:   ROK 
*
*      Notes: 
*
*      File:  nh_ptli.c
*
*************************************************************************/
#ifdef ANSI
PUBLIC S16 PtLiCtfCfgReq
(
Pst *post,
SpId spId,
CtfCfgTransId transId,          /* cfg trans ID */
CtfCfgReqInfo *cfgReqInfo       /* PHY Configuration information */
)
#else /* ANSI */
PUBLIC S16 PtLiCtfCfgReq(post, spId, transId, cfgReqInfo)
Pst *post;
SpId spId;
CtfCfgTransId transId;          /* cfg trans ID */
CtfCfgReqInfo *cfgReqInfo;       /* PHY Configuration information */
#endif /* ANSI */
{

  TRC3(PtLiCtfCfgReq);

  UNUSED(post);
  UNUSED(spId);
  UNUSED(transId);
  UNUSED(cfgReqInfo);

  RLOG0(L_DEBUG, "PtLiCtfCfgReq() called");

  RETVALUE(ROK);

} /* end of function PtLiCtfCfgReq */


/**
* @brief UE ID change Request from RRC to PHY.
*
* @details
*
*     Function : PtLiCtfUeIdChgReq
*
*  @param[in]   Pst*  post
*  @param[in]   SpId  spId
*  @param[in]   CtfCfgTransId  transId
*  @param[in]   CtfUeInfo *  ueInfo
*  @param[in]   CtfUeInfo *  newUeInfo
*  @return   S16
*      -# ROK
**/
#ifdef ANSI
PUBLIC S16 PtLiCtfUeIdChgReq
(
Pst* post,
SpId spId,
CtfCfgTransId transId,
CtfUeInfo * ueInfo,
CtfUeInfo * newUeInfo
)
#else /* ANSI */
PUBLIC S16 PtLiCtfUeIdChgReq(post, spId, transId, ueInfo, newUeInfo)
Pst* post;
SpId spId;
CtfCfgTransId transId;
CtfUeInfo * ueInfo;
CtfUeInfo * newUeInfo;
#endif /* ANSI */
{

   TRC3(PtLiCtfUeIdChgReq)

   UNUSED(post);
   UNUSED(spId);
   UNUSED(transId);
   UNUSED(ueInfo);
   UNUSED(newUeInfo);

   RLOG0(L_DEBUG, "PtLiCtfUeIdChgReq() called");

   RETVALUE(ROK);

} /* end of PtLiCtfUeIdChgReq */

#endif /* PTNHLICTF */
#endif /* RRC_PHY_CFG_SUPP */

/**************************************************************************
 * The PJU Interface
 *************************************************************************/
#ifdef PTNHLIPJU
/**************************************************************************
 *
 * 
 *      @brief  This function binds the RRC service user with the PDCP 
 *             service provider.
 *
 *      @details  This function binds the RRC service user with the PDCP 
 *             service provider. The PDCP registers this new service 
 *             user and allocates a Service Access Point (SAP) for 
 *             this bind. The service user specifies the reference 
 *             number that is used for the duration of this bind.
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiPjuBndReq
(
Pst *post,                      /* Post structure */
SuId suId,                      /* Service user ID */
SpId spId                       /* Service provider ID */
)
#else /* ANSI */
PRIVATE S16 PtLiPjuBndReq(post, suId, spId)
Pst *post;                      /* Post structure */
SuId suId;                      /* Service user ID */
SpId spId;                      /* Service provider ID */
#endif /* ANSI */
{
   TRC3(PtLiPjuBndReq);

   UNUSED(post);
   UNUSED(suId);
   UNUSED(spId);

   RLOG0(L_DEBUG, "PtLiPjuBndReq() called");

   RETVALUE(ROK);


} /* end of function PtLiPjuBndReq */


/**
*
*      @brief This function unbinds the RRC service user with the PDCP 
*             service provider. 
*
*      @details  This function unbinds the RRC service user with the PDCP 
*             service provider. 
*
*/

#ifdef ANSI
PRIVATE S16 PtLiPjuUbndReq
(
Pst *post,                      /* Post structure */
SpId spId,                      /* Service provider ID */
Reason reason                   /* Reason */
)
#else /* ANSI */
PRIVATE S16 PtLiPjuUbndReq(post, spId, reason)
Pst *post;                      /* Post structure */
SpId spId;                      /* Service provider ID */
Reason reason;                  /* Reason */
#endif /* ANSI */
{

   TRC3(PtLiPjuUbndReq);

   UNUSED(post);
   UNUSED(spId);
   UNUSED(reason);

   RLOG0(L_DEBUG, "PtLiPjuUbndReq() called");

   RETVALUE(ROK);

} /* end of function PtLiPjuUbndReq */


/**
*
*      @brief This function is used to pass Signalling Info to PDCP related 
*             to PDCP configuration.
*
*      @details This function is used to pass Signalling Info to PDCP related 
*             to PDCP configuration.
*/
#ifdef ANSI
PRIVATE S16 PtLiPjuDatReq
(
Pst *post,                      /* Post structure */
SpId spId,                      /* Service provider ID */
CmLtePdcpId *pdcpId,
PjuSduId sduId,
Buffer *mBuf
)
#else /* ANSI */
PRIVATE S16 PtLiPjuDatReq(post, spId, pdcpId, sduId, mBuf)
Pst *post;                      /* Post structure */
SpId spId;                      /* Service provider id */
CmLtePdcpId *pdcpId;
PjuSduId sduId;
Buffer *mBuf;
#endif /* ANSI */
{

   TRC3(PtLiPjuDatReq);

   UNUSED(post);
   UNUSED(spId);
   UNUSED(pdcpId);
   UNUSED(sduId);
   UNUSED(mBuf);

   RLOG0(L_DEBUG, "PtLiPjuDatReq() called");

   RETVALUE(ROK);

} /* end of function PtLiPjuDatReq */

#endif /* PTNHLIPJU */
#ifdef __cplusplus
}
#endif /* cplusplus */

/** @} */

/**********************************************************************
  
         End of file:     ni_ptli.c@@/main/3 - Fri Jul  1 01:13:21 2011
   
**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      mc               1. Initial LTE-RRC release.
/main/2      ---      ch               1. RRC Release 2.1.
/main/3      ---      sbalakrishna     1. Updated for Release of 3.1.
*********************************************************************91*/

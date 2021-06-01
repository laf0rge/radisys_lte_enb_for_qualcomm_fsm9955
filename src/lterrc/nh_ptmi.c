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
 
     Name:    RRC Layer's confirm and indication primitives to the layer 
              manager.
  
     Type:    C source file
  
     Desc:    C source code for the RRC outgoing primitives to the layer 
              manager. Layer manager provides the necessary functions 
              to control and monitor the condition of each protocol layer.
 
              The following functions are provided in this file:

              NhMiLnhCfgCfm      Configuration Confirm 
              NhMiLnhCntrlCfm    Control Confirm 
              NhMiLnhStsCfm      Statistics Confirm
              NhMiLnhStaInd      Status Indication
              NhMiLnhStaCfm      Status Confirm
              NhMiLnhTrcInd      Trace Indication
              It is assumed that the following functions are provided 
              in the RRC protocol file:
  
              NhMiLnhCfgReq      Configuration Request
              NhMiLnhCntrlReq    Control Request
              NhMiLnhStsReq      Statistics Request
              NhMiLnhStaReq      Status Request
  
     File:    nh_ptmi.c
  
     Sid:      ni_ptmi.c@@/main/3 - Fri Jul  1 01:13:21 2011
  
     Prg:     rcs
  
**********************************************************************/
  

/*
*     This software may be combined with the following TRILLIUM
*     software:
*
*     part no.                      description
*     --------    ----------------------------------------------
*     1000369     LTE RRC
*
*/


/* header include files -- defines (.h) */
static const char* RLOG_MODULE_NAME="RRC";
static int RLOG_FILE_ID=100;
static int RLOG_MODULE_ID=512;
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */
  
#include "gen.h"           /* General */
#include "ssi.h"           /* System services */

#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_mblk.h"       /* Common Mem Allocation Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hash List Defines */
#include "cm_lte.h"        /* Common LTE Defines */
#include "ckw.h"            /* C-RLC interface */
#include "kwu.h"            /* D-RLC interface */
#include "crg.h"            /* C-MAC interface */
#include "cpj.h"            /* C-PDCP interface */
#include "pju.h"            /* D-PDCP interface */
#include "cm_pasn.h"       /* Common Pasn Defines */

#include "nhu.h"           /* NHU Interface defines */
#include "lnh.h"           /* LNH Interface defines */

#include "nh.h"            /* RRC defines */

/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */
#include "cm5.x"           /* Timer services */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_mblk.x"       /* Common Mem Allocation */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_pasn.x"       /* Common Pasn Definitions */
#include "cm_lte.x"        /* Common LTE Defines */
#include "ckw.x"            /* C-RLC interface */
#include "kwu.x"            /* D-RLC interface */
#include "crg.x"            /* C-MAC interface */
#include "cpj.x"            /* C-PDCP interface */
#include "pju.x"            /* D-PDCP interface */

#include "nhu.x"           /* NHU Interface includes */
#include "lnh.x"           /* LNH Interface includes */

#include "nh.x"            /* RRC includes */
/* local defines */

/* local typedefs */
 
/* local externs */
 
/* forward references */


/* local defines */
#define MAXNHMI 2          /* Size of Matrix used for TC/LC */

#ifndef LCNHMILNH
#define PTNHMILNH   1      /* Portable LNH Interface */
#else /* LCNHMILNH */
#ifndef SM
#define PTNHMILNH   1      /* Portable LNH Interface */
#endif /* SM */
#endif /* LCNHMILNH */

/* local externs */
  
/* forward references */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
 
#ifdef PTNHMILNH
PRIVATE S16 PtMiLnhStaInd ARGS((Pst *pst, NhMngmt *usta));
PRIVATE S16 PtMiLnhStaCfm ARGS((Pst *pst, NhMngmt *cfm));
PRIVATE S16 PtMiLnhStsCfm ARGS((Pst *pst, NhMngmt *cfm));
#ifdef NH_TRACE_ENABLED
PRIVATE S16 PtMiLnhTrcInd ARGS((Pst *pst, NhMngmt *trc));
#endif /* NH_TRACE_ENABLED */
PRIVATE S16 PtMiLnhCfgCfm ARGS((Pst *pst, NhMngmt *cfm));
PRIVATE S16 PtMiLnhCntrlCfm ARGS((Pst *pst, NhMngmt *cfm));
#endif /* PTNHMILNH */ 

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*

   The following matrices define the mapping between the primitives 
   called by the layer manager interface of RRC and the 
   corresponding primitives of the RRC service user(s).
 
   The parameter MAXNHMI defines the maximum number of service users on
   top of RRC. There is an array of functions invoked by 
   RRC per primitive. Every array is MAXNHMI long (that is, there are as many functions 
   as the number of service users).
 
   The dispatching is performed by the configurable variable, selector.
   The selector is configured during the general configuration.
 
   The selectors are:
 
   0 - Loosely Coupled - forward compatibility (#define LCNHMILNH)
   1 - Tightly Coupled  (#define SM)
 
*/

/* Status Indication primitive */
PRIVATE LnhStaInd NhMiLnhStaIndMt[MAXNHMI] =
{
#ifdef LCNHMILNH
   cmPkLnhStaInd,          /* 0 - loosely coupled  */
#else /* LCNHMILNH */
   PtMiLnhStaInd,          /* 0 - tightly coupled, portable */
#endif /* LCNHMILNH */
#ifdef SM
   SmMiLnhStaInd,          /* 1 - tightly coupled, layer management */
#else /* SM */
   PtMiLnhStaInd,          /* 1 - tightly coupled, portable */
#endif /* SM */
};

/* Status confirm primitive */
PRIVATE LnhStaCfm NhMiLnhStaCfmMt[MAXNHMI] =
{
#ifdef LCNHMILNH
   cmPkLnhStaCfm,          /* 0 - loosely coupled  */
#else /* LCNHMILNH */
   PtMiLnhStaCfm,          /* 0 - tightly coupled, portable */
#endif /* LCNHMILNH */
#ifdef SM
   SmMiLnhStaCfm,          /* 1 - tightly coupled, layer management */
#else /* SM */
   PtMiLnhStaCfm,          /* 1 - tightly coupled, portable */
#endif /* SM */
};

/* Statistics confirm primitive */
PRIVATE LnhStsCfm NhMiLnhStsCfmMt[MAXNHMI] =
{
#ifdef LCNHMILNH
   cmPkLnhStsCfm,          /* 0 - loosely coupled  */
#else /* LCNHMILNH */
   PtMiLnhStsCfm,          /* 0 - tightly coupled, portable */
#endif /* LCNHMILNH */
#ifdef SM
   SmMiLnhStsCfm,          /* 1 - tightly coupled, layer management */
#else /* SM */
   PtMiLnhStsCfm,          /* 1 - tightly coupled, portable */
#endif /* SM */
};

#ifdef NH_TRACE_ENABLED
/* Trace indication primitive */
PRIVATE LnhTrcInd NhMiLnhTrcIndMt[MAXNHMI] =
{
#ifdef LCNHMILNH
   cmPkLnhTrcInd,          /* 0 - loosely coupled  */
#else /* LCNHMILNH */
   PtMiLnhTrcInd,          /* 0 - tightly coupled, portable */
#endif /* LCNHMILNH */
#ifdef SM
   SmMiLnhTrcInd,          /* 1 - tightly coupled, layer management */
#else /* SM */
   PtMiLnhTrcInd,          /* 1 - tightly coupled, portable */
#endif /* SM */
};
#endif /* NH_TRACE_ENABLED */

/* Configuration confirm primitive */
PRIVATE LnhCfgCfm NhMiLnhCfgCfmMt[MAXNHMI] =
{
#ifdef LCNHMILNH
   cmPkLnhCfgCfm,          /* 0 - loosely coupled  */
#else /* LCNHMILNH */
   PtMiLnhCfgCfm,          /* 0 - tightly coupled, portable */
#endif /* LCNHMILNH */
#ifdef SM
   SmMiLnhCfgCfm,          /* 1 - tightly coupled, layer management */
#else /* SM */
   PtMiLnhCfgCfm,          /* 1 - tightly coupled, portable */
#endif /* SM */
};

/* Control confirm primitive */
PRIVATE LnhCntrlCfm NhMiLnhCntrlCfmMt[MAXNHMI] =
{
#ifdef LCNHMILNH
   cmPkLnhCntrlCfm,          /* 0 - loosely coupled  */
#else /* LCNHMILNH */
   PtMiLnhCntrlCfm,          /* 0 - tightly coupled, portable */
#endif /* LCNHMILNH */
#ifdef SM
   SmMiLnhCntrlCfm,          /* 1 - tightly coupled, layer management */
#else /* SM */
   PtMiLnhCntrlCfm,          /* 1 - tightly coupled, portable */
#endif /* SM */
};


/**************************************************************************
 * Layer Manager interface functions
 *************************************************************************/

/**************************************************************************
 *
 *      Fun:  NhMiLnhStaInd  
 *
 *      Desc: Used to indicate the status of the RRC layer to the layer manager. 
 *
 *      Ret:  ROK - OK 
 *
 *      Notes:None 
 *
 *      File:  nh_ptmi.c
 *
 *************************************************************************/

#ifdef ANSI
PUBLIC S16 NhMiLnhStaInd
(
Pst *pst,                 /* Post structure */
NhMngmt *usta             /* Unsolicited status */
)
#else /* ANSI */
PUBLIC S16 NhMiLnhStaInd(pst, usta)
Pst *pst;                 /* Post structure */   
NhMngmt *usta;            /* Unsolicited status */
#endif /* ANSI */
{
   TRC3(NhMiLnhStaInd);

   RLOG0(L_DEBUG, "NhMiLnhStaInd");

   /* jump to specific primitive depending on configured selector */
   (*NhMiLnhStaIndMt[pst->selector])(pst, usta); 

   RETVALUE(ROK);
} /* end of function NhMiLnhStaInd */


/**************************************************************************
 *
 *      Fun:  NhMiLnhStaCfm  
 *
 *      Desc: Used to return the status of the RRC layer to the layer manager. 
 *
 *      Ret:  ROK - OK 
 *
 *      Notes:None 
 *
 *      File:  nh_ptmi.c
 *
 *************************************************************************/

#ifdef ANSI
PUBLIC S16 NhMiLnhStaCfm
(
Pst *pst,                 /* Post structure */
NhMngmt *cfm              /* Solicited status */
)
#else /* ANSI */
PUBLIC S16 NhMiLnhStaCfm(pst, cfm)
Pst *pst;                 /* Post structure */   
NhMngmt *cfm;             /* Solicited status */
#endif /* ANSI */
{
   TRC3(NhMiLnhStaCfm);

   RLOG0(L_DEBUG, "NhMiLnhStaCfm");

   /* jump to specific primitive depending on configured selector */
   (*NhMiLnhStaCfmMt[pst->selector])(pst, cfm); 

   RETVALUE(ROK);
} /* end of function NhMiLnhStaCfm */


/**************************************************************************
 *
 *      Fun:  NhMiLnhStsCfm  
 *
 *      Desc: Used to return the statistics of the RRC layer to the layer manager. 
 *
 *      Ret:  ROK - OK 
 *
 *      Notes:None 
 *
 *      File:  nh_ptmi.c
 *
 *************************************************************************/

#ifdef ANSI
PUBLIC S16 NhMiLnhStsCfm
(
Pst *pst,                 /* Post structure */
NhMngmt *cfm              /* Statistics */
)
#else /* ANSI */
PUBLIC S16 NhMiLnhStsCfm(pst, cfm)
Pst *pst;                 /* Post structure */   
NhMngmt *cfm;             /* Statistics */
#endif /* ANSI */
{
   TRC3(NhMiLnhStsCfm);

   RLOG0(L_DEBUG, "NhMiLnhStsCfm");

   /* jump to specific primitive depending on configured selector */
   (*NhMiLnhStsCfmMt[pst->selector])(pst, cfm); 

   RETVALUE(ROK);
} /* end of function NhMiLnhStsCfm */

#ifdef NH_TRACE_ENABLED

/**************************************************************************
 *
 *      Fun:  NhMiLnhTrcInd  
 *
 *      Desc: Used to indicate the trace of the RRC layer to the layer manager. 
 *
 *      Ret:  ROK - OK 
 *
 *      Notes:None 
 *
 *      File:  nh_ptmi.c
 *
 *************************************************************************/

#ifdef ANSI
PUBLIC S16 NhMiLnhTrcInd
(
Pst *pst,                 /* Post structure */
NhMngmt *trc              /* Trace */
)
#else /* ANSI */
PUBLIC S16 NhMiLnhTrcInd(pst, trc)
Pst *pst;                 /* Post structure */   
NhMngmt *trc;             /* Trace */
#endif /* ANSI */
{
   TRC3(NhMiLnhTrcInd);

   RLOG0(L_DEBUG, "NhMiLnhTrcInd ");

   /* jump to specific primitive depending on configured selector */
   (*NhMiLnhTrcIndMt[pst->selector])(pst, trc); 

   RETVALUE(ROK);
} /* end of function NhMiLnhTrcInd */
#endif /* NH_TRACE_ENABLED */

/**************************************************************************
 *
 *      Fun:  NhMiLnhCfgCfm  
 *
 *      Desc: Used to confirm the receipt of Config request from the layer manager. 
 *             
 *
 *      Ret:  ROK - OK 
 *
 *      Notes:None 
 *
 *      File:  nh_ptmi.c
 *
 *************************************************************************/

#ifdef ANSI
PUBLIC S16 NhMiLnhCfgCfm
(
Pst *pst,                 /* Post structure */
NhMngmt *cfm              /* Configuration confirm */
)
#else /* ANSI */
PUBLIC S16 NhMiLnhCfgCfm(pst, cfm)
Pst *pst;                 /* Post structure */   
NhMngmt *cfm;             /* Configuration confirm */
#endif /* ANSI */
{
   TRC3(NhMiLnhCfgCfm);

   RLOG0(L_DEBUG, "NhMiLnhCfgCfm");

   /* jump to specific primitive depending on configured selector */
   (*NhMiLnhCfgCfmMt[pst->selector])(pst, cfm); 

   RETVALUE(ROK);
} /* end of function NhMiLnhCfgCfm */


/**************************************************************************
 *
 *      Fun:  NhMiLnhCntrlCfm  
 *
 *      Desc: Used to confirm the receipt of control request from the layer manager. 
 *            
 *
 *      Ret:  ROK - OK 
 *
 *      Notes:None 
 *
 *      File:  nh_ptmi.c
 *
 *************************************************************************/

#ifdef ANSI
PUBLIC S16 NhMiLnhCntrlCfm
(
Pst *pst,                 /* Post structure */
NhMngmt *cfm              /* Control confirm */
)
#else /* ANSI */
PUBLIC S16 NhMiLnhCntrlCfm(pst, cfm)
Pst *pst;                 /* Post structure */   
NhMngmt *cfm;             /* Control confirm */
#endif /* ANSI */
{
   TRC3(NhMiLnhCntrlCfm);

   RLOG0(L_DEBUG, "NhMiLnhCntrlCfm");

   /* jump to specific primitive depending on configured selector */
   (*NhMiLnhCntrlCfmMt[pst->selector])(pst, cfm); 

   RETVALUE(ROK);
} /* end of function NhMiLnhCntrlCfm */

/**************************************************************************
 * Layer Manager interface PORTABLE functions
 *************************************************************************/

#ifdef PTNHMILNH

/**************************************************************************
 *
 *      Fun:  PtMiLnhStaInd (Portable) 
 *
 *      Desc: Used to indicate the status of the RRC layer to the layer manager. 
 *
 *      Ret:  ROK - OK 
 *
 *      Notes:None 
 *
 *      File:  nh_ptmi.c
 *
 *************************************************************************/

#ifdef ANSI
PUBLIC S16 PtMiLnhStaInd
(
Pst *pst,                 /* Post structure */
NhMngmt *usta             /* Unsolicited status */
)
#else /* ANSI */
PUBLIC S16 PtMiLnhStaInd(pst, usta)
Pst *pst;                 /* Post structure */   
NhMngmt *usta;            /* Unsolicited status */
#endif /* ANSI */
{
   TRC3(PtMiLnhStaInd);

   UNUSED(pst);
   UNUSED(usta);

   RLOG0(L_DEBUG, "PtMiLnhStaInd() called");

   RETVALUE(ROK);
} /* end of PtMiLnhStaInd */


/**************************************************************************
 *
 *      Fun:  PtMiLnhStaCfm (Portable) 
 *
 *      Desc: Used to return the status of the RRC layer to the layer manager. 
 *
 *      Ret:  ROK - OK 
 *
 *      Notes:None 
 *
 *      File:  nh_ptmi.c
 *
 *************************************************************************/

#ifdef ANSI
PUBLIC S16 PtMiLnhStaCfm
(
Pst *pst,                 /* Post structure */
NhMngmt *cfm              /* Solicited status confirm */
)
#else /* ANSI */
PUBLIC S16 PtMiLnhStaCfm(pst, cfm)
Pst *pst;                 /* Post structure */   
NhMngmt *cfm;             /* Solicited status confirm */
#endif /* ANSI */
{
   TRC3(PtMiLnhStaCfm);

   UNUSED(pst);
   UNUSED(cfm);

   RLOG0(L_DEBUG, "PtMiLnhStaCfm() called");

   RETVALUE(ROK);
} /* end of PtMiLnhStaCfm */


/**************************************************************************
 *
 *      Fun:  PtMiLnhStsCfm (Portable) 
 *
 *      Desc: Used to return the statistics of the RRC layer to the layer manager. 
 *
 *      Ret:  ROK - OK 
 *
 *      Notes:None 
 *
 *      File:  nh_ptmi.c
 *
 *************************************************************************/

#ifdef ANSI
PUBLIC S16 PtMiLnhStsCfm
(
Pst *pst,                 /* Post structure */
NhMngmt *cfm              /* Statistics confirm */
)
#else /* ANSI */
PUBLIC S16 PtMiLnhStsCfm(pst, cfm)
Pst *pst;                 /* Post structure */   
NhMngmt *cfm;             /* Statistics confirm */
#endif /* ANSI */
{
   TRC3(PtMiLnhStsCfm);

   UNUSED(pst);
   UNUSED(cfm);

   RLOG0(L_DEBUG, "PtMiLnhStsCfm() called");

   RETVALUE(ROK);
} /* end of PtMiLnhStsCfm */

#ifdef NH_TRACE_ENABLED
/**************************************************************************
 *
 *      Fun:  PtMiLnhTrcInd (Portable) 
 *
 *      Desc: Used to indicate the trace of the RRC layer to the layer manager. 
 *
 *      Ret:  ROK - OK 
 *
 *      Notes:None 
 *
 *      File:  nh_ptmi.c
 *
 *************************************************************************/

#ifdef ANSI
PUBLIC S16 PtMiLnhTrcInd
(
Pst *pst,                 /* Post structure */
NhMngmt *trc              /* Trace */
)
#else /* ANSI */
PUBLIC S16 PtMiLnhTrcInd(pst, trc)
Pst *pst;                 /* Post structure */   
NhMngmt *trc;             /* Trace */
#endif /* ANSI */
{
   TRC3(PtMiLnhTrcInd);

   UNUSED(pst);
   UNUSED(trc);

   RLOG0(L_DEBUG, "PtMiLnhTrcInd() called");

   RETVALUE(ROK);
} /* end of PtMiLnhTrcInd */
#endif /* ifdef NH_TRACE_ENABLED */

/**************************************************************************
 *
 *      Fun:  PtMiLnhCfgCfm (Portable) 
 *
 *      Desc: Used to confirm the receipt of config request to the layer manager. 
 *
 *      Ret:  ROK - OK 
 *
 *      Notes:None 
 *
 *      File:  nh_ptmi.c
 *
 *************************************************************************/

#ifdef ANSI
PUBLIC S16 PtMiLnhCfgCfm
(
Pst *pst,                 /* Post structure */
NhMngmt *cfm              /* Configuration confirm */
)
#else /* ANSI */
PUBLIC S16 PtMiLnhCfgCfm(pst, cfm)
Pst *pst;                 /* Post structure */   
NhMngmt *cfm;             /* Configuration confirm */
#endif /* ANSI */
{
   TRC3(PtMiLnhCfgCfm);

   UNUSED(pst);
   UNUSED(cfm);

   RLOG0(L_DEBUG, "PtMiLnhCfgCfm() called");

   RETVALUE(ROK);
} /* end of PtMiLnhCfgCfm */


/**************************************************************************
 *
 *      Fun:  PtMiLnhCntrlCfm (Portable) 
 *
 *      Desc: Used to confirm the receipt of control request to the layer manager. 
 *
 *      Ret:  ROK - OK 
 *
 *      Notes:None 
 *
 *      File:  nh_ptmi.c
 *
 *************************************************************************/

#ifdef ANSI
PUBLIC S16 PtMiLnhCntrlCfm
(
Pst *pst,                 /* Post structure */
NhMngmt *cfm              /* Control confirm */
)
#else /* ANSI */
PUBLIC S16 PtMiLnhCntrlCfm(pst, cfm)
Pst *pst;                 /* Post structure */   
NhMngmt *cfm;             /* Control confirm */
#endif /* ANSI */
{
   TRC3(PtMiLnhCntrlCfm);

   UNUSED(pst);
   UNUSED(cfm);

   RLOG0(L_DEBUG, "PtMiLnhCntrlCfm() called");

   RETVALUE(ROK);
} /* end of PtMiLnhCntrlCfm */

#endif /* PTNHMILNH */

/**********************************************************************
         End of file:     ni_ptmi.c@@/main/3 - Fri Jul  1 01:13:21 2011
**********************************************************************/
 
 
/**********************************************************************
        Revision history:
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      rer              1. LTE-RRC Initial Release.
/main/2      ---      ch               1. RRC Release 2.1.
/main/3      ---      sbalakrishna     1. Updated for Release of 3.1.
*********************************************************************91*/

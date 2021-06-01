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
  
     Desc:     C source code for portable definitions for Layer Manager
               Interface primitives.
  
     File:     rg_ptmi.c 
  
     Sid:      gk_ptmi.c@@/main/3 - Sat Jul 30 02:21:32 2011
  
     Prg:      sm
  
**********************************************************************/

/** @file rg_ptmi.c
@brief This file contains the definitions for Layer Manager
       Interface primitives that are invoked from MAC to Layer Manager
       (Confirmations and Indications) Portable functions corresponding
       to these primitives are also defined.
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
#include "lrg.h"           /* LRG Interface defines */

/* header/extern include files (.x) */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_lte.x"        /* Common LTE Defines */
#include "lrg.x"           /* LRG Interface includes */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if !(defined(LCRGMILRG) && defined(SM))
#define PTRGMILRG
#endif

#define RG_MAX_LRG_USR 2 

/* portable functions at LRG interface */
#ifdef PTRGMILRG
EXTERN S16 PtMiLrgCfgCfm ARGS((Pst *pst, RgMngmt *cfm));
EXTERN S16 PtMiLrgSchCfgCfm ARGS((Pst *pst, RgMngmt *cfm));
EXTERN S16 PtMiLrgStsCfm ARGS((Pst *pst, RgMngmt *cfm));
EXTERN S16 PtMiLrgStaCfm ARGS((Pst *pst, RgMngmt *cfm));
EXTERN S16 PtMiLrgStaInd ARGS((Pst *pst, RgMngmt *usta));
EXTERN S16 PtMiLrgSchStaInd ARGS((Pst *pst, RgMngmt *usta));
EXTERN S16 PtMiLrgCntrlCfm ARGS((Pst *pst, RgMngmt *cfm));
EXTERN S16 PtMiLrgSchCntrlCfm ARGS((Pst *pst, RgMngmt *cfm));
EXTERN S16 PtMiLrgTrcInd ARGS((Pst *pst, RgMngmt *trc,Buffer *mBuf));
#ifdef LTE_L2_MEAS
EXTERN S16 PtMiLrgSchL2MeasCfm ARGS((Pst *pst, LrgSchMeasCfmInfo *cfm));
EXTERN S16 PtMiLrgSchL2MeasStopCfm ARGS((Pst *pst,LrgSchMeasCfmInfo *cfm ));
#endif
#endif

/* ***********LRG interface Mapping matrices ********************/

/** @brief Configuration Confirm primitive Matrix 
 *  This matrix defines the mapping between the configuration confirm
 *  primitive called by the upper interface of MAC layer and the 
 *  corresponding primitives of the MAC layer service user(s).
 *
 *  The parameter RG_MAX_LRG_USR defines the maximum number of Layer 
 *  managers of MAC at the LRG interface. This is an array of functions
 *  per primitive invoked by MAC whose size is RG_MAX_LRG_USR.
 * 
 *  The dispatching is performed by the configurable variable: selector.
 *  The selector is configured on a per SAP basis. The selectors are:
 *
 *  LCRGMILRG - loosely coupled LMK interface
 *  TCRGMILRG - Tightly coupled LMK interface
 *  
 *  In a similar fashion, the matrices for Status, Statistics and
 *  Control confirms and, Status and trace indications are defined.
 **/
PRIVATE CONSTANT LrgCfgCfm RgMiLrgCfgCfmMt[RG_MAX_LRG_USR] =
{
#ifdef LCRGMILRG
   cmPkLrgCfgCfm,                  /* 0 - loosely coupled */
#else
   PtMiLrgCfgCfm,                  /* 0 - loosely coupled */
#endif
#ifdef SM 
   SmMiLrgCfgCfm,                  /* 1 - Tightly coupled SM */
#else
   PtMiLrgCfgCfm,                  /* 1 - Tightly coupled SM  */
#endif
};

/** @brief Scheduler Config Confirm primitive Matrix */
PRIVATE CONSTANT LrgSchCfgCfm RgMiLrgSchCfgCfmMt[RG_MAX_LRG_USR] =
{
#ifdef LCRGMILRG
   cmPkLrgSchCfgCfm,                  /* 0 - loosely coupled */
#else
   PtMiLrgSchCfgCfm,                  /* 0 - loosely coupled */
#endif
#ifdef SM 
   SmMiLrgSchCfgCfm,                  /* 1 - Tightly coupled SM */
#else
   PtMiLrgSchCfgCfm,                  /* 1 - Tightly coupled SM  */
#endif
};

/** @brief Statistics Confirm primitive Matrix */
PRIVATE CONSTANT LrgStsCfm RgMiLrgStsCfmMt[RG_MAX_LRG_USR] =
{
#ifdef LCRGMILRG
   cmPkLrgStsCfm,                  /* 0 - loosely coupled */
#else
   PtMiLrgStsCfm,                  /* 0 - loosely coupled */
#endif
#ifdef SM 
   SmMiLrgStsCfm,                  /* 1 - Tightly coupled SM */
#else
   PtMiLrgStsCfm,                  /* 1 - Tightly coupled SM  */
#endif
};

/** @brief Status Confirm primitive Matrix */
PRIVATE CONSTANT LrgStaCfm RgMiLrgStaCfmMt[RG_MAX_LRG_USR] =
{
#ifdef LCRGMILRG
   cmPkLrgStaCfm,                  /* 0 - loosely coupled */
#else
   PtMiLrgStaCfm,                  /* 0 - loosely coupled */
#endif
#ifdef SM 
   SmMiLrgStaCfm,                  /* 1 - Tightly coupled SM */
#else
   PtMiLrgStaCfm,                  /* 1 - Tightly coupled SM  */
#endif
};


/** @brief Status Indication primitive Matrix */
PRIVATE CONSTANT LrgStaInd RgMiLrgStaIndMt[RG_MAX_LRG_USR] =
{
#ifdef LCRGMILRG
   cmPkLrgStaInd,                  /* 0 - loosely coupled */
#else
   PtMiLrgStaInd,                  /* 0 - loosely coupled */
#endif
#ifdef SM 
   SmMiLrgStaInd,                  /* 1 - Tightly coupled SM */
#else
   PtMiLrgStaInd,                  /* 1 - Tightly coupled SM  */
#endif
};

/** @brief Scheduler  Status Indication primitive Matrix */
PRIVATE CONSTANT LrgSchStaInd RgMiLrgSchStaIndMt[RG_MAX_LRG_USR] =
{
#ifdef LCRGMILRG
   cmPkLrgSchStaInd,                  /* 0 - loosely coupled */
#else
   PtMiLrgSchStaInd,                  /* 0 - loosely coupled */
#endif
#ifdef SM 
   SmMiLrgSchStaInd,                  /* 1 - Tightly coupled SM */
#else
   PtMiLrgSchStaInd,                  /* 1 - Tightly coupled SM  */
#endif
};


/** @brief Control Confirm primitive Matrix */
PRIVATE CONSTANT LrgCntrlCfm RgMiLrgCntrlCfmMt[RG_MAX_LRG_USR] =
{
#ifdef LCRGMILRG
   cmPkLrgCntrlCfm,                  /* 0 - loosely coupled */
#else
   PtMiLrgCntrlCfm,                  /* 0 - loosely coupled */
#endif
#ifdef SM 
   SmMiLrgCntrlCfm,                  /* 1 - Tightly coupled SM */
#else
   PtMiLrgCntrlCfm,                  /* 1 - Tightly coupled SM  */
#endif
};

/** @brief Scheduler Control Confirm primitive Matrix */
PRIVATE CONSTANT LrgSchCntrlCfm RgMiLrgSchCntrlCfmMt[RG_MAX_LRG_USR] =
{
#ifdef LCRGMILRG
   cmPkLrgSchCntrlCfm,                  /* 0 - loosely coupled */
#else
   PtMiLrgSchCntrlCfm,                  /* 0 - loosely coupled */
#endif
#ifdef SM 
   SmMiLrgSchCntrlCfm,                  /* 1 - Tightly coupled SM */
#else
   PtMiLrgSchCntrlCfm,                  /* 1 - Tightly coupled SM  */
#endif
};

/** @brief Trace Indication primitive Matrix */
PRIVATE CONSTANT LrgTrcInd RgMiLrgTrcIndMt[RG_MAX_LRG_USR] =
{
#ifdef LCRGMILRG
   cmPkLrgTrcInd,                  /* 0 - loosely coupled */
#else
   PtMiLrgTrcInd,                  /* 0 - loosely coupled */
#endif
#ifdef SM 
   SmMiLrgTrcInd,                  /* 1 - Tightly coupled SM */
#else
   PtMiLrgTrcInd,                  /* 1 - Tightly coupled SM  */
#endif
};
#ifdef LTE_L2_MEAS
/** @brief L2 Meas Cfm primitive Matrix */
PRIVATE CONSTANT LrgSchL2MeasCfm RgMiLrgSchL2mMeasCfmMt[RG_MAX_LRG_USR] =
{
#ifdef LCRGMILRG
   cmPkLrgSchL2MeasCfm,                /* 0 - loosely coupled */
#else
   PtMiLrgSchL2MeasCfm,                  /* 0 - loosely coupled */
#endif
#ifdef SM 
   SmMiLrgSchL2MeasCfm,                  /* 1 - Tightly coupled SM */
#else
   PtMiLrgSchL2MeasCfm,                  /* 1 - loosely coupled */
#endif
};
/** @brief L2 Meas Stop Cfm primitive Matrix */
PRIVATE CONSTANT LrgSchL2MeasStopCfm RgMiLrgSchL2mMeasStopCfmMt[RG_MAX_LRG_USR] =
{
#ifdef LCRGMILRG
   cmPkLrgSchL2MeasStopCfm,                /* 0 - loosely coupled */
#else
   PtMiLrgSchL2MeasStopCfm,                  /* 0 - loosely coupled */
#endif
#ifdef SM 
   SmMiLrgSchL2MeasStopCfm,                  /* 1 - Tightly coupled SM */
#else
   PtMiLrgSchL2MeasStopCfm,                  /* 1 - loosely coupled */
#endif
};
#endif /* LTE_L2_MEAS */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#ifdef RG


/**
 * @brief Layer Manager Configuration confirm handler. 
 *
 * @details
 *
 *     Function : RgMiLrgCfgCfm
 *     
 *     This function handles the configuration
 *     confirm invoked by MAC to Layer Manager.
 *     -# Based on the pst->selector value it invokes one of the
 *        functions cmPkLrgCfgCfm() or SmMiLrgCfgCfm().
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the configuration confirm structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 RgMiLrgCfgCfm
(
Pst      *pst,    /* post structure  */
RgMngmt  *cfm     /* config confirm structure  */
)
#else
PUBLIC S16 RgMiLrgCfgCfm(pst, cfm)
Pst      *pst;    /* post structure  */
RgMngmt  *cfm;    /* config confirm structure  */
#endif    
{
   
   TRC3(RgMiLrgCfgCfm);

   (*RgMiLrgCfgCfmMt[pst->selector])(pst, cfm);

   RETVALUE(ROK);
   
}/*-- RgMiLrgCfgCfm --*/

/**
 * @brief Layer Manager scheduler Configuration confirm handler. 
 *
 * @details
 *
 *     Function : RgMiLrgSchCfgCfm
 *     
 *     This function handles the configuration
 *     confirm invoked by Scheduler to Layer Manager.
 *     -# Based on the pst->selector value it invokes one of the
 *        functions cmPkLrgSchCfgCfm() or SmMiLrgSchCfgCfm().
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the configuration confirm structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 RgMiLrgSchCfgCfm
(
Pst      *pst,    /* post structure  */
RgMngmt  *cfm     /* config confirm structure  */
)
#else
PUBLIC S16 RgMiLrgSchCfgCfm(pst, cfm)
Pst      *pst;    /* post structure  */
RgMngmt  *cfm;    /* config confirm structure  */
#endif    
{
   
   TRC3(RgMiLrgSchCfgCfm);

   (*RgMiLrgSchCfgCfmMt[pst->selector])(pst, cfm);

   RETVALUE(ROK);
   
}/*-- RgMiLrgSchCfgCfm --*/



/**
 * @brief Layer Manager Statistics confirm handler. 
 *
 * @details
 *
 *     Function : RgMiLrgStsCfm
 *     
 *     This function handles the statistics
 *     confirm invoked by MAC to Layer Manager.
 *     -# Based on the pst->selector value it invokes one of the
 *        functions cmPkLrgStsCfm() or SmMiLrgStsCfm().
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the statistics confirm structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 RgMiLrgStsCfm
(
Pst      *pst,    /* post structure  */
RgMngmt  *cfm     /* statistics confirm structure  */
)
#else
PUBLIC S16 RgMiLrgStsCfm(pst, cfm)
Pst      *pst;    /* post structure  */
RgMngmt  *cfm;    /* statistics confirm structure  */
#endif    
{
   
   TRC3(RgMiLrgStsCfm);

   (*RgMiLrgStsCfmMt[pst->selector])(pst, cfm);

   RETVALUE(ROK);
   
}/*-- RgMiLrgStsCfm --*/


/**
 * @brief Layer Manager Status confirm handler. 
 *
 * @details
 *
 *     Function : RgMiLrgStaCfm
 *     
 *     This function handles the status
 *     confirm invoked by MAC to Layer Manager.
 *     -# Based on the pst->selector value it invokes one of the
 *        functions cmPkLrgStaCfm() or SmMiLrgStaCfm().
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the status confirm structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 RgMiLrgStaCfm
(
Pst      *pst,    /* post structure  */
RgMngmt  *cfm     /* status confirm structure  */
)
#else
PUBLIC S16 RgMiLrgStaCfm(pst, cfm)
Pst      *pst;    /* post structure  */
RgMngmt  *cfm;    /* status confirm structure  */
#endif    
{
   
   TRC3(RgMiLrgStaCfm);
    
   (*RgMiLrgStaCfmMt[pst->selector])(pst,cfm);

   RETVALUE(ROK);
   
}/*-- RgMiLrgStaCfm --*/


/**
 * @brief Layer Manager Control confirm handler. 
 *
 * @details
 *
 *     Function : RgMiLrgCntrlCfm
 *     
 *     This function handles the control
 *     confirm invoked by MAC to Layer Manager.
 *     -# Based on the pst->selector value it invokes one of the
 *        functions cmPkLrgCntrlCfm() or SmMiLrgCntrlCfm().
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the control confirm structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 RgMiLrgCntrlCfm
(
Pst      *pst,    /* post structure  */
RgMngmt  *cfm     /* control confirm structure  */
)
#else
PUBLIC S16 RgMiLrgCntrlCfm(pst, cfm)
Pst      *pst;    /* post structure  */
RgMngmt  *cfm;    /* control confirm structure  */
#endif    
{
   
   TRC3(RgMiLrgCntrlCfm);

   (*RgMiLrgCntrlCfmMt[pst->selector])(pst,cfm);

   RETVALUE(ROK);
   
}/*-- RgMiLrgCntrlCfm --*/

/**
 * @brief Layer Manager scheduler Control confirm handler. 
 *
 * @details
 *
 *     Function : RgMiLrgSchCntrlCfm
 *     
 *     This function handles the control
 *     confirm invoked by scheduler to Layer Manager.
 *     -# Based on the pst->selector value it invokes one of the
 *        functions cmPkLrgSchCntrlCfm() or SmMiLrgSchCntrlCfm().
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the control confirm structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 RgMiLrgSchCntrlCfm
(
Pst      *pst,    /* post structure  */
RgMngmt  *cfm     /* control confirm structure  */
)
#else
PUBLIC S16 RgMiLrgSchCntrlCfm(pst, cfm)
Pst      *pst;    /* post structure  */
RgMngmt  *cfm;    /* control confirm structure  */
#endif    
{
   
   TRC3(RgMiLrgSchCntrlCfm);

   (*RgMiLrgSchCntrlCfmMt[pst->selector])(pst,cfm);

   RETVALUE(ROK);
   
}/*-- RgMiLrgSchCntrlCfm --*/


/**
 * @brief Layer Manager Unsolicited Status Indication handler. 
 *
 * @details
 *
 *     Function : RgMiLrgStaInd
 *     
 *     This function handles the unsolicited status
 *     Indication invoked by MAC to Layer Manager.
 *     -# Based on the pst->selector value it invokes one of the
 *        functions cmPkLrgStaInd() or SmMiLrgStaInd().
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *usta, the status indication structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 RgMiLrgStaInd
(
Pst      *pst,    /* post structure  */
RgMngmt  *usta    /* status indication structure  */
)
#else
PUBLIC S16 RgMiLrgStaInd(pst, usta)
Pst      *pst;    /* post structure  */
RgMngmt  *usta;    /* status indication structure  */
#endif    
{
   
   TRC3(RgMiLrgStaInd);

   (*RgMiLrgStaIndMt[pst->selector])(pst,usta);

   RETVALUE(ROK);
   
}/*-- RgMiLrgStaInd --*/

/**
 * @brief Layer Manager Unsolicited Status Indication handler from scheduler 
 *
 * @details
 *
 *     Function : RgMiLrgSchStaInd
 *     
 *     This function handles the unsolicited status
 *     Indication invoked by Scheduler to Layer Manager.
 *     -# Based on the pst->selector value it invokes one of the
 *        functions cmPkLrgSchStaInd() or SmMiLrgSchStaInd().
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *usta, the status indication structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 RgMiLrgSchStaInd
(
Pst      *pst,    /* post structure  */
RgMngmt  *usta    /* status indication structure  */
)
#else
PUBLIC S16 RgMiLrgSchStaInd(pst, usta)
Pst      *pst;    /* post structure  */
RgMngmt  *usta;    /* status indication structure  */
#endif    
{
   
   TRC3(RgMiLrgSchStaInd);

   (*RgMiLrgSchStaIndMt[pst->selector])(pst,usta);

   RETVALUE(ROK);
   
}/*-- RgMiLrgSchStaInd --*/


/**
 * @brief Layer Manager Trace Indication handler. 
 *
 * @details
 *
 *     Function : RgMiLrgTrcInd
 *     
 *     This function handles the trace
 *     Indication invoked by MAC to Layer Manager.
 *     -# Based on the pst->selector value it invokes one of the
 *        functions cmPkLrgTrcInd() or SmMiLrgTrcInd().
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *trc, the trace event
 *  @param[in]  Buffer *mBuf, the trace message
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 RgMiLrgTrcInd
(
Pst      *pst,    /* post structure  */
RgMngmt  *trc,    /* Trace event  */
Buffer   *mBuf    /* Trace message  */
)
#else
PUBLIC S16 RgMiLrgTrcInd(pst, trc, mBuf)
Pst      *pst;    /* post structure  */
RgMngmt  *trc;    /* Trace event  */
Buffer   *mBuf;   /* Trace message  */
#endif    
{
   
   TRC3(RgMiLrgTrcInd);

   (*RgMiLrgTrcIndMt[pst->selector])(pst,trc,mBuf);

   RETVALUE(ROK);
   
}/*-- RgMiLrgTrcInd --*/

#ifdef LTE_L2_MEAS

/* TODO: Function header */
#ifdef ANSI
PUBLIC S16 RgMiLrgSchL2MeasCfm
(
Pst               *pst,          /* post structure  */
LrgSchMeasCfmInfo *cfm       /* Meas Cfm Info */
)
#else
PUBLIC S16 RgMiLrgSchL2MeasCfm(pst, cfm)
Pst               *pst;          /* post structure  */
LrgSchMeasCfmInfo *cfm;       /* Meas Cfm Info */
#endif    
{
   TRC3(RgMiLrgSchL2MeasCfm)
   
   (*RgMiLrgSchL2mMeasCfmMt[pst->selector])(pst,cfm);
   
   RETVALUE(ROK);

} /* RgMiLrgSchL2MeasCfm */

/**
 * @brief This function used to send L2 measurement stop confirmation 
 *        to Layer manager.
 *         
 *
 * @details
 *
 *     Function : RgMiLrgSchL2MeasStopCfm
 *     This function sends reposnes to L2 Measurement stop request
 *
 *
 *  @param[in]  Pst *pst, the post structure
 *  @param[in]  LrgSchMeasCfmInfo *cfm, theconfirm structure
 *  @return  S16
 *      -# ROK
 **/

#ifdef ANSI 
PUBLIC S16 RgMiLrgSchL2MeasStopCfm
(        
Pst               *pst,          /* post structure  */
LrgSchMeasCfmInfo *cfm       /* Meas Cfm Info */
)        
#else    
PUBLIC S16 RgMiLrgSchL2MeasStopCfm(pst, cfm)
Pst               *pst;          /* post structure  */
LrgSchMeasCfmInfo *cfm;       /* Meas Cfm Info */
#endif    
{
   TRC3(RgMiLrgSchL2MeasStopCfm)
        
   (*RgMiLrgSchL2mMeasStopCfmMt[pst->selector])(pst,cfm);
          
   RETVALUE(ROK);
        
} /* RgMiLrgSchL2MeasStopCfm */

#endif /* LTE_L2_MEAS */

#endif /*-- MK --*/

#ifdef PTRGMILRG

/**
 * @brief Portable Function definition for Layer Manager Configuration
 *         confirm handler. 
 *
 * @details
 *
 *     Function : PtMiLrgCfgCfm
 *     
 *     This function handles the configuration
 *     confirm invoked by MAC to Layer Manager.
 *     Users of MAC who intend to provide a glue logic 
 *     for portability of Configuration Confirm are expected 
 *     to fill in the code in this function definition.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the configuration confirm structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 PtMiLrgCfgCfm
(
Pst *pst,               /* post structure */
RgMngmt *cfm            /* Config Confirm */
)
#else
PUBLIC S16 PtMiLrgCfgCfm  (pst, cfm)
Pst *pst;               /* post structure */
RgMngmt *cfm;           /* Config Confirm */
#endif
{
   TRC3(PtMiLrgCfgCfm)

   RETVALUE(ROK);
}/* end of PtMiLrgCfgCfm */

/**
 * @brief Portable Function definition for Layer Manager Configuration
 *         confirm handler. 
 *
 * @details
 *
 *     Function : PtMiLrgSchCfgCfm
 *     
 *     This function handles the configuration
 *     confirm invoked by scheduler to Layer Manager.
 *     Users of MAC who intend to provide a glue logic 
 *     for portability of Configuration Confirm are expected 
 *     to fill in the code in this function definition.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the configuration confirm structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 PtMiLrgSchCfgCfm
(
Pst *pst,               /* post structure */
RgMngmt *cfm            /* Config Confirm */
)
#else
PUBLIC S16 PtMiLrgSchCfgCfm  (pst, cfm)
Pst *pst;               /* post structure */
RgMngmt *cfm;           /* Config Confirm */
#endif
{
   TRC3(PtMiLrgSchCfgCfm)

   RETVALUE(ROK);
}/* end of PtMiLrgSchCfgCfm */



/**
 * @brief Portable Function definition for Layer Manager Statistics 
 *         confirm handler. 
 *
 * @details
 *
 *     Function : PtMiLrgStsCfm 
 *     
 *     This function handles the statistics 
 *     confirm invoked by MAC to Layer Manager.
 *     Users of MAC who intend to provide a glue logic 
 *     for portability of statistics Confirm are expected 
 *     to fill in the code in this function definition.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the statistics confirm structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 PtMiLrgStsCfm
(
Pst *pst,               /* post structure */
RgMngmt *cfm            /* Statistics Confirm */
)
#else
PUBLIC S16 PtMiLrgStsCfm  (pst, cfm)
Pst *pst;               /* post structure */
RgMngmt *cfm;           /* Statistics Confirm */
#endif
{
   TRC3(PtMiLrgStsCfm)

   RETVALUE(ROK);
}/* end of PtMiLrgStsCfm */



/**
 * @brief Portable Function definition for Layer Manager Status 
 *         confirm handler. 
 *
 * @details
 *
 *     Function : PtMiLrgStaCfm 
 *     
 *     This function handles the status 
 *     confirm invoked by MAC to Layer Manager.
 *     Users of MAC who intend to provide a glue logic 
 *     for portability of status Confirm are expected 
 *     to fill in the code in this function definition.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the status confirm structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 PtMiLrgStaCfm
(
Pst *pst,               /* post structure */
RgMngmt *cfm            /* Status Confirm */
)
#else
PUBLIC S16 PtMiLrgStaCfm  (pst, cfm)
Pst *pst;               /* post structure */
RgMngmt *cfm;           /* Status Confirm */
#endif
{
   TRC3(PtMiLrgStaCfm)
   RETVALUE(ROK);
}/* end of PtMiLrgStaCfm */



/**
 * @brief Portable Function definition for Layer Manager Status 
 *        Indication handler. 
 *
 * @details
 *
 *     Function : PtMiLrgStaInd 
 *     
 *     This function handles the status 
 *     indication invoked by MAC to Layer Manager.
 *     Users of MAC who intend to provide a glue logic 
 *     for portability of status indication are expected 
 *     to fill in the code in this function definition.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the status indication structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 PtMiLrgStaInd
(
Pst *pst,               /* post structure */
RgMngmt *usta           /* Status Indication */
)
#else
PUBLIC S16 PtMiLrgStaInd  (pst, usta)
Pst *pst;               /* post structure */
RgMngmt *usta;          /* Status indication */
#endif
{
   TRC3(PtMiLrgStaInd)
   RETVALUE(ROK);
}/* end of PtMiLrgStaInd */

/**
 * @brief Portable Function definition for Layer Manager Status 
 *        Indication handler. 
 *
 * @details
 *
 *     Function : PtMiLrgSchStaInd 
 *     
 *     This function handles the status 
 *     indication invoked by Scheduler to Layer Manager.
 *     Users of MAC who intend to provide a glue logic 
 *     for portability of status indication are expected 
 *     to fill in the code in this function definition.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the status indication structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 PtMiLrgSchStaInd
(
Pst *pst,               /* post structure */
RgMngmt *usta           /* Status Indication */
)
#else
PUBLIC S16 PtMiLrgSchStaInd  (pst, usta)
Pst *pst;               /* post structure */
RgMngmt *usta;          /* Status indication */
#endif
{
   TRC3(PtMiLrgSchStaInd)
   RETVALUE(ROK);
}/* end of PtMiLrgSchStaInd */


/**
 * @brief Portable Function definition for Layer Manager Control 
 *         confirm handler. 
 *
 * @details
 *
 *     Function : PtMiLrgCntrlCfm 
 *     
 *     This function handles the control 
 *     confirm invoked by MAC to Layer Manager.
 *     Users of MAC who intend to provide a glue logic 
 *     for portability of control Confirm are expected 
 *     to fill in the code in this function definition.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the control confirm structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 PtMiLrgCntrlCfm
(
Pst *pst,               /* post structure */
RgMngmt *cfm            /* Control Confirm */
)
#else
PUBLIC S16 PtMiLrgCntrlCfm (pst, cfm)
Pst *pst;               /* post structure */
RgMngmt *cfm;           /* Control Confirm */
#endif
{
   TRC3(PtMiLrgCntrlCfm)
   RETVALUE(ROK);
}/* end of PtMiLrgCntrlCfm */

/**
 * @brief Portable Function definition for Layer Manager Control 
 *         confirm handler. 
 *
 * @details
 *
 *     Function : PtMiLrgSchCntrlCfm 
 *     
 *     This function handles the control 
 *     confirm invoked by scheduler to Layer Manager.
 *     Users of MAC who intend to provide a glue logic 
 *     for portability of control Confirm are expected 
 *     to fill in the code in this function definition.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the control confirm structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 PtMiLrgSchCntrlCfm
(
Pst *pst,               /* post structure */
RgMngmt *cfm            /* Control Confirm */
)
#else
PUBLIC S16 PtMiLrgSchCntrlCfm (pst, cfm)
Pst *pst;               /* post structure */
RgMngmt *cfm;           /* Control Confirm */
#endif
{
   TRC3(PtMiLrgSchCntrlCfm)
   RETVALUE(ROK);
}/* end of PtMiLrgSchCntrlCfm */


/**
 * @brief Portable Function definition for Layer Manager Trace 
 *        Indication handler. 
 *
 * @details
 *
 *     Function : PtMiLrgTrcInd 
 *     
 *     This function handles the trace 
 *     indication invoked by MAC to Layer Manager.
 *     Users of MAC who intend to provide a glue logic 
 *     for portability of trace indication are expected 
 *     to fill in the code in this function definition.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfm, the trace indication structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 PtMiLrgTrcInd
(
Pst *pst,               /* post structure */
RgMngmt *trc,           /* Trace Event */
Buffer *mBuf            /* Trace message */
)
#else
PUBLIC S16 PtMiLrgTrcInd (pst, trc,mBuf)
Pst *pst;               /* post structure */
RgMngmt *trc;           /* Trace Event */
Buffer *mBuf;           /* Trace message */
#endif
{
   TRC3(PtMiLrgTrcInd)
   RETVALUE(ROK);
}/* end of PtMiLrgTrcInd */
#ifdef LTE_L2_MEAS
/**
 * @brief Portable Function definition for L2 Measurement Configuration
 *         confirm handler. 
 *
 * @details
 *
 *     Function : PtMiLrgSchL2MeasCfm
 *     
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  *cfm, the measurement confirm structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 PtMiLrgSchL2MeasCfm
(
Pst *pst,               /* post structure */
LrgSchMeasCfmInfo *cfm  /* Measurement Confirm */
)
#else
PUBLIC S16 PtMiLrgSchL2MeasCfm(pst, cfm)
Pst *pst;               /* post structure */
LrgSchMeasCfmInfo *cfm; /* Measurement Confirm */
#endif
{
   TRC3(PtMiLrgSchL2MeasCfm)

   RETVALUE(ROK);
}/* end of PtMiLrgSchL2MeasCfm */

/**
 * @brief Portable Function definition for L2 Measurement stop 
 *         confirm handler. 
 *
 * @details
 *
 *     Function : PtMiLrgSchL2MeasStopCfm
 *     
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  *cfm, the measurement confirm structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 PtMiLrgSchL2MeasStopCfm
(
Pst *pst,               /* post structure */
LrgSchMeasCfmInfo *cfm  /* Measurement Confirm */
)
#else
PUBLIC S16 PtMiLrgSchL2MeasStopCfm(pst, cfm)
Pst *pst;               /* post structure */
LrgSchMeasCfmInfo *cfm; /* Measurement Confirm */
#endif
{
   TRC3(PtMiLrgSchL2MeasStopCfm)

   RETVALUE(ROK);
}/* end of PtMiLrgSchL2MeasStopCfm */
#endif
#endif /*--PTRGMILRG--*/


/**********************************************************************
 
         End of file:     gk_ptmi.c@@/main/3 - Sat Jul 30 02:21:32 2011
 
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
/main/3      ---        gvj  1. Updated for LTE MAC Rel 3.1           
$SID$        ---        rt   1. LTE MAC 4.1 release
*********************************************************************91*/

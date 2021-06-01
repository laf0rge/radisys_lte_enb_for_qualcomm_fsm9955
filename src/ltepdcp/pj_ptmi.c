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

        Name:     RLC Layer Management interface

        Type:     C source file

        Desc:     This file contains source code for RLC layer management
                  interface primitives. It includes the definition of the
                  following functions and their portable function
                  definitions.
                     -- PjMiLpjCfgCfm
                     -- PjMiLpjCntrlCfm
                     -- PjMiLpjStaInd


        File:     kw_ptmi.c

        Sid:      eq_ptmi.c@@/main/TeNB_Main_BR/tenb_main_ccb/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:52:51 2016

        Prg:      cp 

*********************************************************************21*/


/* header (.h) include files */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timer defines */
#include "cm_tkns.h"       /* common tokens defines */
#include "cm_mblk.h"       /* common memory allocation library defines */
#include "cm_llist.h"      /* common link list  defines  */
#include "cm_hash.h"       /* common hash list  defines */
#include "cm_lte.h"        /* common LTE defines */
#include "lpj.h"           /* LPJ defines */


/* extern (.x) include files */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */

#include "cm5.x"           /* common timer library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_mblk.x"       /* common memory allocation */
#include "cm_llist.x"      /* common link list */
#include "cm_hash.x"       /* common hash list */
#include "cm_lte.x"        /* common LTE includes */
#include "cm_lib.x"        /* common memory allocation library */
#include "lpj.x"           /* LPJ */


/* local defines */
#ifndef LCPJMILPJ
#define PTPJLPJ
#endif

#ifndef SM
#define PTPJLPJ
#endif


#ifdef PTPJLPJ

EXTERN S16 PtMiLpjCfgCfm ARGS((Pst * pst, PjMngmt * cfgCfm));

EXTERN S16 PtMiLpjCntrlCfm ARGS((Pst * pst, PjMngmt * cntlCfm));

EXTERN S16 PtMiLpjStaInd ARGS((Pst * pst, PjMngmt * staInd));

EXTERN S16 PtMiLpjStaCfm ARGS((Pst * pst, PjMngmt * staCfm));

EXTERN S16 PtMiLpjStsCfm ARGS((Pst * pst, Action actn, PjMngmt * stsCfm));

EXTERN S16 PtMiLpjTrcInd ARGS((Pst * pst, PjMngmt * trc, Buffer * mBuf));

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
EXTERN S16 PtMiLpjL2MeasCfm   ARGS((Pst *pst, PjL2MeasCfmEvt *measEvt));
PRIVATE S16 PtMiLpjL2MeasStopCfm   ARGS((Pst *pst, U16 measType,U8 status));
#endif /*  LTE_L2_MEAS */

#endif /*--#ifdef PTPJLPJ--*/

/*********************************************************************
 *             Primitives for LPJ interface 
 ********************************************************************/
PRIVATE CONSTANT LpjCfgCfm PjMiLpjCfgCfmMt[] =
{
#ifdef LCPJMILPJ
   cmPkLpjCfgCfm,
#else
   PtMiLpjCfgCfm,
#endif
#ifdef SM
   SmMiLpjCfgCfm,
#else
   PtMiLpjCfgCfm,
#endif
};


PRIVATE CONSTANT LpjCntrlCfm PjMiLpjCntrlCfmMt[] =
{
#ifdef LCPJMILPJ
   cmPkLpjCntrlCfm,
#else
   PtMiLpjCntrlCfm,
#endif
#ifdef SM
   SmMiLpjCntrlCfm,
#else
   PtMiLpjCntrlCfm,
#endif
};


PRIVATE CONSTANT LpjStaInd PjMiLpjStaIndMt[] =
{
#ifdef LCPJMILPJ
   cmPkLpjStaInd,
#else
   PtMiLpjStaInd,
#endif
#ifdef SM
   SmMiLpjStaInd,
#else
   PtMiLpjStaInd,
#endif
};


PRIVATE CONSTANT LpjStaCfm PjMiLpjStaCfmMt[] =
{
#ifdef LCPJMILPJ
   cmPkLpjStaCfm,
#else
   PtMiLpjStaCfm,
#endif
#ifdef SM
   SmMiLpjStaCfm,
#else
   PtMiLpjStaCfm,
#endif
};


PRIVATE CONSTANT LpjStsCfm PjMiLpjStsCfmMt[] =
{
#ifdef LCPJMILPJ
   cmPkLpjStsCfm,
#else
   PtMiLpjStsCfm,
#endif
#ifdef SM
   SmMiLpjStsCfm,
#else
   PtMiLpjStsCfm,
#endif
};


PRIVATE CONSTANT LpjTrcInd PjMiLpjTrcIndMt[] =
{
#ifdef LCPJMILPJ
   cmPkLpjTrcInd,
#else
   PtMiLpjTrcInd,
#endif
#ifdef SM
   SmMiLpjTrcInd,
#else
   PtMiLpjTrcInd,
#endif
};

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
PRIVATE CONSTANT LpjL2MeasCfm PjMiLpjL2MeasCfmMt[] =
{
#ifdef LCPJMILPJ
   cmPkLpjL2MeasCfm,
#else
   PtMiLpjL2MeasCfm,
#endif
#ifdef SM
   SmMiLpjL2MeasCfm,
#else
   PtMiLpjL2MeasCfm,
#endif
};
PRIVATE CONSTANT LpjL2MeasStopCfm PjMiLpjL2MeasStopCfmMt[] =
{
#ifdef LCPJMILPJ
   cmPkLpjL2MeasStopCfm,
#else
   PtMiLpjL2MeasStopCfm,
#endif
#ifdef SM
   SmMiLpjL2MeasStopCfm,
#else
   PtMiLpjL2MeasStopCfm,
#endif
};
#endif /*  LTE_L2_MEAS */

/****************************************************************************
 *                         LPJ Interface Mt functions
 ***************************************************************************/
#ifdef ANSI
PUBLIC S16 PjMiLpjCfgCfm
(
Pst * pst,
PjMngmt * cfgCfm
)
#else
PUBLIC S16 PjMiLpjCfgCfm(pst, cfgCfm)
Pst * pst;
PjMngmt * cfgCfm;
#endif
{

   TRC3(PjMiLpjCfgCfm)

   (*PjMiLpjCfgCfmMt[pst->selector])(pst, cfgCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PjMiLpjCntrlCfm
(
Pst * pst,
PjMngmt * cntlCfm
)
#else
PUBLIC S16 PjMiLpjCntrlCfm(pst, cntlCfm)
Pst * pst;
PjMngmt * cntlCfm;
#endif
{

   TRC3(PjMiLpjCntrlCfm)

   (*PjMiLpjCntrlCfmMt[pst->selector])(pst, cntlCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PjMiLpjStaInd
(
Pst * pst,
PjMngmt * staInd
)
#else
PUBLIC S16 PjMiLpjStaInd(pst, staInd)
Pst * pst;
PjMngmt * staInd;
#endif
{

   TRC3(PjMiLpjStaInd)

   (*PjMiLpjStaIndMt[pst->selector])(pst, staInd);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PjMiLpjStaCfm
(
Pst * pst,
PjMngmt * staCfm
)
#else
PUBLIC S16 PjMiLpjStaCfm(pst, staCfm)
Pst * pst;
PjMngmt * staCfm;
#endif
{

   TRC3(PjMiLpjStaCfm)

   (*PjMiLpjStaCfmMt[pst->selector])(pst, staCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PjMiLpjStsCfm
(
Pst * pst,
Action actn,
PjMngmt * stsCfm
)
#else
PUBLIC S16 PjMiLpjStsCfm(pst, actn, stsCfm)
Pst * pst;
Action actn;
PjMngmt * stsCfm;
#endif
{

   TRC3(PjMiLpjStsCfm)

   (*PjMiLpjStsCfmMt[pst->selector])(pst, actn, stsCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PjMiLpjTrcInd
(
Pst * pst,
PjMngmt * trc,
Buffer * mBuf
)
#else
PUBLIC S16 PjMiLpjTrcInd(pst, trc, mBuf)
Pst * pst;
PjMngmt * trc;
Buffer * mBuf;
#endif
{

   TRC3(PjMiLpjTrcInd)

   (*PjMiLpjTrcIndMt[pst->selector])(pst, trc, mBuf);

   RETVALUE(ROK);

}

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#ifdef ANSI
PUBLIC S16 PjMiLpjL2MeasCfm
(
Pst * pst,
PjL2MeasCfmEvt *measEvt
)
#else
PUBLIC S16 PjMiLpjL2MeasCfm(pst, trc, mBuf)
Pst * pst;
PjL2MeasCfmEvt *measEvt;
#endif
{

   TRC3(PjMiLpjL2MeasCfm)

   (*PjMiLpjL2MeasCfmMt[pst->selector])(pst, measEvt);

   RETVALUE(ROK);

}
#ifdef ANSI
PUBLIC S16 PjMiLpjL2MeasStopCfm
(  
Pst *pst,
U16  measType,
U8  status
)
#else
PUBLIC S16 PjMiLpjL2MeasStopCfm(pst, measType,status)
Pst *pst;
U16  measType;
U8  status;
#endif
{

   TRC3(PjMiLpjL2MeasStopCfm)

   (*PjMiLpjL2MeasStopCfmMt[pst->selector])(pst, measType,status);

   RETVALUE(ROK);

}
#endif /*  LTE_L2_MEAS */


#ifdef PTPJLPJ


#ifdef ANSI
PUBLIC S16 PtMiLpjCfgCfm
(
Pst * pst,
PjMngmt * cfgCfm
)
#else
PUBLIC S16 PtMiLpjCfgCfm(pst, cfgCfm)
Pst * pst;
PjMngmt * cfgCfm;
#endif
{

   TRC3(PtMiLpjCfgCfm)

   UNUSED(pst);
   UNUSED(cfgCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtMiLpjCntrlCfm
(
Pst * pst,
PjMngmt * cntlCfm
)
#else
PUBLIC S16 PtMiLpjCntrlCfm(pst, cntlCfm)
Pst * pst;
PjMngmt * cntlCfm;
#endif
{

   TRC3(PtMiLpjCntrlCfm)

   UNUSED(pst);
   UNUSED(cntlCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtMiLpjStaInd
(
Pst * pst,
PjMngmt * staInd
)
#else
PUBLIC S16 PtMiLpjStaInd(pst, staInd)
Pst * pst;
PjMngmt * staInd;
#endif
{

   TRC3(PtMiLpjStaInd)

   UNUSED(pst);
   UNUSED(staInd);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtMiLpjStaCfm
(
Pst * pst,
PjMngmt * staCfm
)
#else
PUBLIC S16 PtMiLpjStaCfm(pst, staCfm)
Pst * pst;
PjMngmt * staCfm;
#endif
{

   TRC3(PtMiLpjStaCfm)

   UNUSED(pst);
   UNUSED(staCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtMiLpjStsCfm
(
Pst * pst,
Action actn,
PjMngmt * stsCfm
)
#else
PUBLIC S16 PtMiLpjStsCfm(pst, actn, stsCfm)
Pst * pst;
Action actn;
PjMngmt * stsCfm;
#endif
{

   TRC3(PtMiLpjStsCfm)

   UNUSED(pst);
   UNUSED(actn);
   UNUSED(stsCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtMiLpjTrcInd
(
Pst * pst,
PjMngmt * trc,
Buffer * mBuf
)
#else
PUBLIC S16 PtMiLpjTrcInd(pst, trc, mBuf)
Pst * pst;
PjMngmt * trc;
Buffer * mBuf;
#endif
{

   TRC3(PtMiLpjTrcInd)

   UNUSED(pst);
   UNUSED(trc);
   UNUSED(mBuf);

   RETVALUE(ROK);

}

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#ifdef ANSI
PUBLIC S16 PtMiLpjL2MeasCfm
(
Pst * pst,
PjL2MeasCfmEvt * measEvt
)
#else
PUBLIC S16 PtMiLpjL2MeasCfm(pst, measEvt)
Pst * pst;
PjL2MeasCfmEvt * measEvt;
#endif
{

   TRC3(PtMiLpjL2MeasCfm)

   UNUSED(pst);
   UNUSED(measEvt);

   RETVALUE(ROK);

}
#ifdef ANSI
PRIVATE S16 PtMiLpjL2MeasStopCfm
(
Pst * pst,
U16 measType,
U8 status
)
#else
PRIVATE S16 PtMiLpjL2MeasStopCfm(pst, measType,status)
Pst * pst;
U16  measType;
U8  status;
#endif
{

   TRC3(PtMiLpjL2MeasStopCfm)

   UNUSED(pst);
   UNUSED(measType);
   UNUSED(status);

   RETVALUE(ROK);

}
#endif /*  LTE_L2_MEAS */
#endif /*--ifdef PTPJLPJ--*/

/********************************************************************30**

         End of file:     eq_ptmi.c@@/main/TeNB_Main_BR/tenb_main_ccb/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:52:51 2016

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
/main/1      ---      gk   1. initial release.
/main/2      ---      gk   1. LTERLC Release 2.1
/main/3      kw005.201 ap  1. Added support for L2 Measurement.
*********************************************************************91*/


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
  
        Name:    LTE-RLC Layer - Upper Interface
    
        Type:    C file
  
        Desc:    C source code for the upper interface of LTE-RLC
 
        File:    kw_ptui.c

        Sid:      eq_udx_ptdl.c@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 18:21:40 2015
  
        Prg:     gk
  
*********************************************************************21*/

/** @file gp_ptui.c
@brief RLC Upper Interface
*/

/*
 *      This software may be combined with the following TRILLIUM
 *      software:
 *
 *      part no.                      description
 *      --------    ----------------------------------------------
 *      1000371     LTE-RLC
 */


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
#include "cpj.h"           /* UDX defines */
#include "pju.h"           /* PJU defines */
#include "lpj.h"

#include "pj_err.h"
#include "pj_env.h"        /* RLC environment options */
#include "pj_udx.h"

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
#include "cpj.x"           /* UDX defines */
#include "pju.x"           /* PJU defines */
#include "lpj.x"           /* LPJ defines */
#include "pj.h"
#include "pj.x"
#include "pj_udx.x"


#if !(defined(LCPJULUDX)  && defined(PJ))
#define PTPJUDX
#endif


#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */

  
/*********************************************************************
 *             Forward Declartion for UDX Porting Functions
 ********************************************************************/
#ifdef PTPJUDX

EXTERN S16 PtDlUdxBndCfm ARGS((Pst * pst, SuId suId, U8 status));

EXTERN S16 PtDlUdxCfgCfm ARGS((Pst * pst, SuId suId, UdxCfgCfmInfo * cfgCfm));

EXTERN S16 PtDlUdxReEstCfm ARGS((Pst * pst, SuId suId, UdxReEstCfmInfo * reEstCfm));

EXTERN S16 PtDlUdxSecCfgCfm ARGS((Pst * pst, SuId suId, UdxSecCfgCfmInfo * secCfgCfm));

EXTERN S16 PtDlUdxUeIdChgCfm ARGS((Pst * pst, SuId suId, U32 transId, UdxUeInfo  * ueInfo, CmStatus status));

EXTERN S16 PtDlUdxCountCfm ARGS((Pst * pst, SuId suId, UdxCountCfmInfo * countCfm));

EXTERN S16 PtDlUdxSduStaCfm ARGS((Pst * pst, SuId suId, UdxSduStaCfmInfo * staCfm));

EXTERN S16 PtDlUdxDatResumeCfm ARGS((Pst * pst, SuId suId, UdxDatResumeCfmInfo * datResume));

EXTERN S16 PtDlUdxDatForwardCfm ARGS((Pst * pst, SuId suId, UdxDatFwdCfmInfo * datFwdCfm));
#endif /* PTPJUDX */

  
/*********************************************************************
 *             Forward Declartion for PJU Porting Functions
 ********************************************************************/

  
/*********************************************************************
 *                         Primitives for UDX interface 
 ********************************************************************/
PRIVATE CONSTANT UdxBndCfm PjDlUdxBndCfmMt[] =
{
#ifdef LCPJULUDX
   cmPkUdxBndCfm,
#else
   PtDlUdxBndCfm,
#endif
#ifdef LWLCPJULUDX
   cmPkUdxBndCfm,
#else
   PtDlUdxBndCfm,
#endif
#ifdef PJ
   PjUlUdxBndCfm,
#else
   PtDlUdxBndCfm,
#endif
};


PRIVATE CONSTANT UdxCfgCfm PjDlUdxCfgCfmMt[] =
{
#ifdef LCPJULUDX
   cmPkUdxCfgCfm,
#else
   PtDlUdxCfgCfm,
#endif
#ifdef LWLCPJULUDX
   cmPkUdxCfgCfm,
#else
   PtDlUdxCfgCfm,
#endif
#ifdef PJ
   PjUlUdxCfgCfm,
#else
   PtDlUdxCfgCfm,
#endif
};


PRIVATE CONSTANT UdxReEstCfm PjDlUdxReEstCfmMt[] =
{
#ifdef LCPJULUDX
   cmPkUdxReEstCfm,
#else
   PtDlUdxReEstCfm,
#endif
#ifdef LWLCPJULUDX
   cmPkUdxReEstCfm,
#else
   PtDlUdxReEstCfm,
#endif
#ifdef PJ
   PjUlUdxReEstCfm,
#else
   PtDlUdxReEstCfm,
#endif
};


PRIVATE CONSTANT UdxSecCfgCfm PjDlUdxSecCfgCfmMt[] =
{
#ifdef LCPJULUDX
   cmPkUdxSecCfgCfm,
#else
   PtDlUdxSecCfgCfm,
#endif
#ifdef LWLCPJULUDX
   cmPkUdxSecCfgCfm,
#else
   PtDlUdxSecCfgCfm,
#endif
#ifdef PJ
   PjUlUdxSecCfgCfm,
#else
   PtDlUdxSecCfgCfm,
#endif
};


PRIVATE CONSTANT UdxUeIdChgCfm PjDlUdxUeIdChgCfmMt[] =
{
#ifdef LCPJULUDX
   cmPkUdxUeIdChgCfm,
#else
   PtDlUdxUeIdChgCfm,
#endif
#ifdef LWLCPJULUDX
   cmPkUdxUeIdChgCfm,
#else
   PtDlUdxUeIdChgCfm,
#endif
#ifdef PJ
   PjUlUdxUeIdChgCfm,
#else
   PtDlUdxUeIdChgCfm,
#endif
};


PRIVATE CONSTANT UdxCountCfm PjDlUdxCountCfmMt[] =
{
#ifdef LCPJULUDX
   cmPkUdxCountCfm,
#else
   PtDlUdxCountCfm,
#endif
#ifdef LWLCPJULUDX
   cmPkUdxCountCfm,
#else
   PtDlUdxCountCfm,
#endif
#ifdef PJ
   PjUlUdxCountCfm,
#else
   PtDlUdxCountCfm,
#endif
};


PRIVATE CONSTANT UdxSduStaCfm PjDlUdxSduStaCfmMt[] =
{
#ifdef LCPJULUDX
   cmPkUdxSduStaCfm,
#else
   PtDlUdxSduStaCfm,
#endif
#ifdef LWLCPJULUDX
   cmPkUdxSduStaCfm,
#else
   PtDlUdxSduStaCfm,
#endif
#ifdef PJ
   PjUlUdxSduStaCfm,
#else
   PtDlUdxSduStaCfm,
#endif
};

PRIVATE CONSTANT UdxDatResumeCfm PjDlUdxDatResumeCfmMt[] =
{
#ifdef LCPJULUDX
   cmPkUdxDatResumeCfm,
#else
   PtDlUdxDatResumeCfm,
#endif
#ifdef LWLCPJULUDX
   cmPkUdxDatResumeCfm,
#else
   PtDlUdxDatResumeCfm,
#endif
#ifdef PJ
   PjUlUdxDatResumeCfm,
#else
   PtDlUdxDatResumeCfm,
#endif
};

PRIVATE CONSTANT UdxDatForwardCfm PjDlUdxDatFwdCfmMt[] =
{
#ifdef LCPJULUDX
   cmPkUdxDatForwardCfm,
#else
   PtDlUdxDatForwardCfm,
#endif
#ifdef LWLCPJULUDX
   cmPkUdxDatForwardCfm,
#else
   PtDlUdxDatForwardCfm,
#endif
#ifdef PJ
   PjUlUdxDatForwardCfm,
#else
   PtDlUdxDatForwardCfm,
#endif
};

  
/****************************************************************************
 *                         UDX Interface Mt functions
 ***************************************************************************/
#ifdef PJ
#ifdef ANSI
PUBLIC S16 PjDlUdxBndCfm
(
Pst * pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 PjDlUdxBndCfm(pst, suId, status)
Pst * pst;
SuId suId;
U8 status;
#endif
{

   TRC3(PjDlUdxBndCfm)

   (*PjDlUdxBndCfmMt[pst->selector])(pst, suId, status);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjDlUdxCfgCfm
(
Pst * pst,
SuId suId,
UdxCfgCfmInfo * cfgCfm
)
#else
PUBLIC S16 PjDlUdxCfgCfm(pst, suId, cfgCfm)
Pst * pst;
SuId suId;
UdxCfgCfmInfo * cfgCfm;
#endif
{

   TRC3(PjDlUdxCfgCfm)

   (*PjDlUdxCfgCfmMt[pst->selector])(pst, suId, cfgCfm);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjDlUdxReEstCfm
(
Pst * pst,
SuId suId,
UdxReEstCfmInfo * reEstCfm
)
#else
PUBLIC S16 PjDlUdxReEstCfm(pst, suId, reEstCfm)
Pst * pst;
SuId suId;
UdxReEstCfmInfo * reEstCfm;
#endif
{

   TRC3(PjDlUdxReEstCfm)

   (*PjDlUdxReEstCfmMt[pst->selector])(pst, suId, reEstCfm);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjDlUdxSecCfgCfm
(
Pst * pst,
SuId suId,
UdxSecCfgCfmInfo * secCfgCfm
)
#else
PUBLIC S16 PjDlUdxSecCfgCfm(pst, suId, secCfgCfm)
Pst * pst;
SuId suId;
UdxSecCfgCfmInfo * secCfgCfm;
#endif
{

   TRC3(PjDlUdxSecCfgCfm)

   (*PjDlUdxSecCfgCfmMt[pst->selector])(pst, suId, secCfgCfm);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjDlUdxUeIdChgCfm
(
Pst * pst,
SuId suId,
U32 transId,
UdxUeInfo  * ueInfo,
CmStatus status
)
#else
PUBLIC S16 PjDlUdxUeIdChgCfm(pst, suId, transId, ueInfo, status)
Pst * pst;
SuId suId;
U32 transId;
UdxUeInfo  * ueInfo;
CmStatus status;
#endif
{

   TRC3(PjDlUdxUeIdChgCfm)

   (*PjDlUdxUeIdChgCfmMt[pst->selector])(pst, suId, transId, ueInfo, status);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjDlUdxCountCfm
(
Pst * pst,
SuId suId,
UdxCountCfmInfo * countCfm
)
#else
PUBLIC S16 PjDlUdxCountCfm(pst, suId, countCfm)
Pst * pst;
SuId suId;
UdxCountCfmInfo * countCfm;
#endif
{

   TRC3(PjDlUdxCountCfm)

   (*PjDlUdxCountCfmMt[pst->selector])(pst, suId, countCfm);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjDlUdxSduStaCfm
(
Pst * pst,
SuId suId,
UdxSduStaCfmInfo * staCfm
)
#else
PUBLIC S16 PjDlUdxSduStaCfm(pst, suId, staCfm)
Pst * pst;
SuId suId;
UdxSduStaCfmInfo * staCfm;
#endif
{

   TRC3(PjDlUdxSduStaCfm)

   (*PjDlUdxSduStaCfmMt[pst->selector])(pst, suId, staCfm);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjDlUdxDatResumeCfm
(
Pst * pst,
SuId suId,
UdxDatResumeCfmInfo * staCfm
)
#else
PUBLIC S16 PjDlUdxDatResumeCfm(pst, suId, staCfm)
Pst * pst;
SuId suId;
UdxDatResumeCfmInfo * staCfm;
#endif
{

   TRC3(PjDlUdxDatResumeCfm)

   (*PjDlUdxDatResumeCfmMt[pst->selector])(pst, suId, staCfm);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjDlUdxDatFwdCfm
(
Pst * pst,
SuId  suId,
UdxDatFwdCfmInfo  * datFwdCfm
)
#else
PUBLIC S16 PjDlUdxDatFwdCfm(pst, suId, datFwdCfm)
Pst * pst;
SuId  suId;
UdxDatFwdCfmInfo  * datFwdCfm;
#endif
{

   TRC3(PjDlUdxDatFwdCfm)

   (*PjDlUdxDatFwdCfmMt[pst->selector])(pst, suId, datFwdCfm);

   RETVALUE(ROK);
}
#endif
  
/*************************************************************************
 *                         UDX Porting Functions
 ************************************************************************/
#ifdef PTPJUDX

#ifdef ANSI
PUBLIC S16 PtDlUdxBndCfm
(
Pst * pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 PtDlUdxBndCfm(pst, suId, status)
Pst * pst;
SuId suId;
U8 status;
#endif
{

   TRC3(PtDlUdxBndCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(status);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtDlUdxCfgCfm
(
Pst * pst,
SuId suId,
UdxCfgCfmInfo * cfgCfm
)
#else
PUBLIC S16 PtDlUdxCfgCfm(pst, suId, cfgCfm)
Pst * pst;
SuId suId;
UdxCfgCfmInfo * cfgCfm;
#endif
{

   TRC3(PtDlUdxCfgCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(cfgCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtDlUdxReEstCfm
(
Pst * pst,
SuId suId,
UdxReEstCfmInfo * reEstCfm
)
#else
PUBLIC S16 PtDlUdxReEstCfm(pst, suId, reEstCfm)
Pst * pst;
SuId suId;
UdxReEstCfmInfo *reEstCfm;
#endif
{

   TRC3(PtDlUdxReEstCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(reEstCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtDlUdxSecCfgCfm
(
Pst * pst,
SuId suId,
UdxSecCfgCfmInfo * secCfgCfm
)
#else
PUBLIC S16 PtDlUdxSecCfgCfm(pst, suId, secCfgCfm)
Pst * pst;
SuId suId;
UdxSecCfgCfmInfo * secCfgCfm;
#endif
{

   TRC3(PtDlUdxSecCfgCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(secCfgCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtDlUdxUeIdChgCfm
(
Pst * pst,
SuId suId,
U32 transId,
UdxUeInfo  * ueInfo,
CmStatus status
)
#else
PUBLIC S16 PtDlUdxUeIdChgCfm(pst, suId, transId, ueInfo, status)
Pst * pst;
SuId suId;
U32 transId;
UdxUeInfo  * ueInfo;
CmStatus status;
#endif
{

   TRC3(PtDlUdxUeIdChgCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(transId);
   UNUSED(ueInfo);
   UNUSED(status);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtDlUdxCountCfm
(
Pst * pst,
SuId suId,
UdxCountCfmInfo * countCfm
)
#else
PUBLIC S16 PtDlUdxCountCfm(pst, suId, countCfm)
Pst * pst;
SuId suId;
UdxCountCfmInfo * countCfm;
#endif
{

   TRC3(PtDlUdxCountCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(countCfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 PtDlUdxSduStaCfm
(
Pst * pst,
SuId suId,
UdxSduStaCfmInfo * staCfm
)
#else
PUBLIC S16 PtDlUdxSduStaCfm(pst, suId, staCfm)
Pst * pst;
SuId suId;
UdxSduStaCfmInfo * staCfm;
#endif
{

   TRC3(PtDlUdxSduStaCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(staCfm);

   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 PtDlUdxDatResumeCfm
(
Pst * pst,
SuId suId,
UdxDatResumeCfmInfo * staCfm
)
#else
PUBLIC S16 PtDlUdxDatResumeCfm(pst, suId, staCfm)
Pst * pst;
SuId suId;
UdxDatResumeCfmInfo * staCfm;
#endif
{

   TRC3(PtDlUdxDatResumeCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(staCfm);

   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 PtDlUdxDatForwardCfm
(
 Pst               *pst,
 SuId              suId,
 UdxDatFwdCfmInfo  *datFwdCfm
 )
#else
PUBLIC S16 PtDlUdxDatForwardCfm(pst, suId, datFwdCfm)
   Pst               *pst;
   SuId              suId;
   UdxDatFwdCfmInfo  *datFwdCfm;
#endif
{

   TRC3(PtDlUdxDatForwardCfm)

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(datFwdCfm); 

   RETVALUE(ROK);
}
#endif /*--ifdef PTPJUDX--*/


#ifdef __cplusplus
}
#endif /* __cplusplus */
/********************************************************************30**
         End of file:     eq_udx_ptdl.c@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 18:21:40 2015
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
/main/3      kw005.201 ap  1. Added support for L2 Measurement feature.
*********************************************************************91*/

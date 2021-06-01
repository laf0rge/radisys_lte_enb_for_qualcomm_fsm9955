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

     Name:     DAM Layer

     Type:     C source file

     Desc:     C Source code for DMA  upper interface structures

     File:     wr_ifm_dam_utils.x

     Sid:      wr_ifm_dam_utils.x@@/main/TeNB_Main_BR/5 - Mon Aug 11 16:53:14 2014

     Prg:      sn

*********************************************************************21*/

#ifndef __WRIFMUTL__X
#define __WRIFMUTL__X

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/************************************************************************
 * H E A D E R     I N C L U D E        F I L E S                       *
 ***********************************************************************/
#include "envopt.h"           /* environment options */
#include "envdep.h"           /* environment dependent */
#include "gen.h"              /* general layer */
#include "ssi.h"              /* system services */
#include "cm_tkns.h"          /* common tokens */
#include "cm_mblk.h"          /* Memory Link List */
#include "wr_ifm_dam_utils.h" /* Event structure file */
#include "wr_ifm_dam.h"       /* Event structure file */

/************************************************************************
 * E X T E R N     I N C L U D E        F I L E S                       *
 ***********************************************************************/
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */
#include "cm_tkns.x"       /* common tokens */
#include "cm_mblk.x"       /* Memory Link List typedefs */
#include "cm_lib.x"        /* common ss7 */


/** @file wr_ifm_dam_utils.x
    @brief DAT Interface File (wr_ifm_dam_utils.x) 
*/
/*typedef S16 (*PrnThptReq) ARGS((
      void)); 
*/


/* Pack Function Prototype */

EXTERN S16 cmPkUeDelReq ARGS ((Pst *pst,U32 transId,U16 cellId,U16 crnit));

EXTERN S16 cmPkUeDelCfm ARGS ((Pst *pst,U32 transId,U16 cellId,U16 crnit));

EXTERN S16 cmPkCellAddReq ARGS ((Pst *pst,U32 transId,WrDamCellInfo *cellInfo));

EXTERN S16 cmPkTnlCreatReq ARGS((Pst *pst,U32 transId,WrDamTnlInfo *tnlInfo));

EXTERN S16 cmPkTnlCreatCfm ARGS((Pst * pst,U32 transId,U8 status,U32 lclTeid));

EXTERN S16 cmPkTnlDelReq   ARGS((Pst *pst,U32 transId,WrDamTnlId *tnlId ));

EXTERN S16 cmPkTnlStChngReq ARGS((Pst *pst,WrDamTnlStInfo *tnlStInfo));

EXTERN S16 cmPkReestabReq ARGS((Pst *pst,U32 transId,U16 cellId,U16 oldcrnti,U16  newcrnti));

EXTERN S16 cmPkReestabCfm ARGS((Pst * pst,U32 transId,U8 status));

EXTERN S16 cmPkReCfgReq ARGS((Pst *pst,U32 transId,U16 cellId,U16 crnti));

EXTERN S16 cmPkPrnThptReq ARGS((Pst* pst));

EXTERN S16 cmPkStartInactivityTimer ARGS((Pst *pst,U16 cellId,U16 crnti));
EXTERN S16 cmPkStopUeTmr ARGS((Pst *pst,U16 cellId,U16 ueId));
EXTERN S16 cmPkDamSendCtxtRel ARGS((Pst *pst,U16 crnti,U16 CellId,U8 event));
EXTERN S16 cmPkSetDataRcvdFlag ARGS((Pst *pst,U16 cellId,U16 ueId));/*ccpu00138576*/

EXTERN S16 DaUiDatBndReq ARGS (( Pst *pst, SuId suId, SpId spId));

/* Element Pack Functions */
EXTERN S16 cmPkCmTnlInfo ARGS (( WrDamTnlInfo  *tnlInfo,Buffer  *mBuf)); 


EXTERN S16 cmPkDamTnlId ARGS ((WrDamTnlId  *tnlId,Buffer  *mBuf ));
EXTERN S16 cmPkDamCellInfo ARGS ((WrDamCellInfo  *cellInfo,Buffer  *mBuf ));

EXTERN S16 cmPkHdlHoFailure ARGS((Pst *pst,U16 cellId,U16 crnti ));

/* Unpack function prototype */

EXTERN Void cmUnpkPrnThptReq
ARGS((
Pst *pst,
Buffer *mBuf
));
EXTERN Void cmUnPkTnlCreatReq 
ARGS((
Pst *pst,
Buffer *mBuf
));
EXTERN Void cmUnPkTnlCreatCfm 
ARGS((
Pst *pst,
Buffer *mBuf
));
EXTERN Void cmUnPkTnlDelReq
ARGS((
Pst *pst,
Buffer *mBuf
));

EXTERN Void cmUnPkUeDelReq
ARGS((
Pst *pst,
Buffer *mBuf
));

EXTERN Void cmUnPkReestabReq
ARGS((
Pst *pst,
Buffer *mBuf
));

EXTERN Void cmUnPkReCfgReq 
ARGS((
Pst *pst,
Buffer *mBuf
));

EXTERN Void cmUnPkCellAddReq 
ARGS((
Pst *pst,
Buffer *mBuf
));


EXTERN Void cmUnPkTnlStChngReq 
ARGS((
Pst *pst,
Buffer *mBuf
));

EXTERN Void cmUnPkReestabCfm
ARGS((
Pst *pst,
Buffer *mBuf
));

EXTERN Void cmUnPkStartInactivityTimer 
ARGS((
Pst *pst,
Buffer *mBuf
));

EXTERN Void cmUnPkStopUeTmr 
ARGS((
Pst *pst,
Buffer *mBuf
));

EXTERN Void cmUnPkDamSendCtxtRel 
ARGS((
Pst *pst,
Buffer *mBuf
));

EXTERN Void cmUnHdlHoFailure
ARGS((
Pst *pst,
Buffer *mBuf
));

EXTERN Void cmUnPkStartInactivityTmr
ARGS((Pst *pst,
Buffer *mBuf
));

/*ccpu00138576 - start*/
EXTERN Void cmUnPkSetDataRcvdFlag 
ARGS((
Pst *pst,
Buffer *mBuf
));
/*ccpu00138576 - end*/


#endif

/********************************************************************30**

           End of file:    $SID$

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
$SID$        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/

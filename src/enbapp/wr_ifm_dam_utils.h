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



/***********************************************************************
^L
************************************************************************

   Name: DAM Layer

   Type: C Include File
   
   Desc: Defines required by DAM

   File: wr_ifm_dam_utils.h

   Sid:      wr_ifm_dam_utils.h@@/main/TeNB_Main_BR/5 - Mon Aug 11 16:53:14 2014

   Prg : mm

************************************************************************/

#ifndef __WRIFMDAMUTL_H__
#define __WRIFMDAMUTL_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "wr.h"
#include "wr_umm.h"
#include "wr_dam.h"
#include "wr_lwr.h"

#define DAM_SEL_LC 2
#define DAM_SEL_TC 1
#define DAM_SEL_LWLC 0

/* added event types for DAM  */
#define    EVTDAMPRNTTHPTREQ        1  /*  DAM  Throughput Req */
#define    EVTDAMTNLCRETREQ         2  /*  DAM  Tunnel Create Request */
#define    EVTDAMTNLCRETCFM         3  /*  DAM  Tunnel Create Confirm */
#define    EVTDAMTNLDELTREQ         4  /*  DAM  Tunnel Delete Request */
#define    EVTDAMUEDELTREQ          5  /*  DAM  UE Delete Request */
#define    EVTDAMUEDELTCFM          6  /*  DAM  UE Delete Confirm */
#define    EVTDAMCELLADDREQ         7  /*  DAM  CELL Add Request */
#define    EVTDAMRECFGREQ           8  /*  DAM  Reconfiguration Request */
#define    EVTDAMRESTREQ            9   /* DAM Re-Establishment Request */
#define    EVTDAMTNLSTCHNGREQ       10  /* DAM Tunnel State Change Req */
#define    EVTDAMREESTABCFM         11  /* DAM Re-establishement Confirm */
#define    EVTDAMINACTIVITYTMR      12  /* DAM UE Inactivity Timer */
#define    EVTDAMUESTOPTMR          13  /* DAM UE Stop Timer    */
#define    EVTDAMALARMIND           14  /* DAM Alarm indication    */
#define    EVTDAMCFGREQ             15  /* DAM Gen Config Request    */
#define    EVTDAMCNTRLREQ           16  /* DAM Control Request    */
#define    EVTDAMCFGCFM             17  /* DAM Gen Config Confirm    */
#define    EVTDAMCNTRLCFM           18  /* DAM Control Confirm    */
#define    EVTDAMTNLMODCFM          19  /* DAM  Tunnel Modify Confirm */
#define    EVTDAMTNLMODREQ          20  /* DAM  Tunnel Modify Req */
#define    EVTDAMPDCPDATCFM         21  /* DAM  PDCP DAT CFM*/
#define    EVTDAMCTXTRELREQ         22  /* DAM  UE Ctxt Rel Req*/
#define    EVTDAMHDLHOFAIL          23  /* DAM  Handle HO Req */
#define    EVTDAMEGTERRIND          24  /* DAM  to WR EGT-U Err Ind*/
#define    EVTDAMDATRCVDFLAG        25  /* DAM Set Data Rcvd Flag */
#define   ERRDAM       0         /* error base */

#define   EDAM001      (ERRDAM +    1)   /*wr_ifm_dam_utils.c: 163 */
#define   EDAM002      (ERRDAM +    2)   /*wr_ifm_dam_utils.c: 164 */
#define   EDAM003      (ERRDAM +    3)   /*wr_ifm_dam_utils.c: 187 */
#define   EDAM004      (ERRDAM +    4)   /*wr_ifm_dam_utils.c: 188 */
#define   EDAM005      (ERRDAM +    5)   /*wr_ifm_dam_utils.c: 214 */
#define   EDAM006      (ERRDAM +    6)   /*wr_ifm_dam_utils.c: 215 */
#define   EDAM007      (ERRDAM +    7)   /*wr_ifm_dam_utils.c: 239 */
#define   EDAM008      (ERRDAM +    8)   /*wr_ifm_dam_utils.c: 240 */
#define   EDAM009      (ERRDAM +    9)   /*wr_ifm_dam_utils.c: 265 */
#define   EDAM010      (ERRDAM +   10)   /*wr_ifm_dam_utils.c: 266 */
#define   EDAM011      (ERRDAM +   11)   /*wr_ifm_dam_utils.c: 290 */
#define   EDAM012      (ERRDAM +   12)   /*wr_ifm_dam_utils.c: 291 */
#define   EDAM013      (ERRDAM +   13)   /*wr_ifm_dam_utils.c: 329 */
#define   EDAM014      (ERRDAM +   14)   /*wr_ifm_dam_utils.c: 332 */
#define   EDAM015      (ERRDAM +   15)   /*wr_ifm_dam_utils.c: 334 */
#define   EDAM016      (ERRDAM +   16)   /*wr_ifm_dam_utils.c: 335 */
#define   EDAM017      (ERRDAM +   17)   /*wr_ifm_dam_utils.c: 336 */
#define   EDAM018      (ERRDAM +   18)   /*wr_ifm_dam_utils.c: 337 */
#define   EDAM019      (ERRDAM +   19)   /*wr_ifm_dam_utils.c: 338 */
#define   EDAM020      (ERRDAM +   20)   /*wr_ifm_dam_utils.c: 339 */
#define   EDAM021      (ERRDAM +   21)   /*wr_ifm_dam_utils.c: 340 */
#define   EDAM022      (ERRDAM +   22)   /*wr_ifm_dam_utils.c: 384 */
#define   EDAM023      (ERRDAM +   23)   /*wr_ifm_dam_utils.c: 385 */
#define   EDAM024      (ERRDAM +   24)   /*wr_ifm_dam_utils.c: 386 */
#define   EDAM025      (ERRDAM +   25)   /*wr_ifm_dam_utils.c: 387 */
#define   EDAM026      (ERRDAM +   26)   /*wr_ifm_dam_utils.c: 388 */
#define   EDAM027      (ERRDAM +   27)   /*wr_ifm_dam_utils.c: 389 */
#define   EDAM028      (ERRDAM +   28)   /*wr_ifm_dam_utils.c: 390 */
#define   EDAM029      (ERRDAM +   29)   /*wr_ifm_dam_utils.c: 398 */
#define   EDAM030      (ERRDAM +   30)   /*wr_ifm_dam_utils.c: 435 */
#define   EDAM031      (ERRDAM +   31)   /*wr_ifm_dam_utils.c: 438 */
#define   EDAM032      (ERRDAM +   32)   /*wr_ifm_dam_utils.c: 440 */
#define   EDAM033      (ERRDAM +   33)   /*wr_ifm_dam_utils.c: 441 */
#define   EDAM034      (ERRDAM +   34)   /*wr_ifm_dam_utils.c: 442 */

EXTERN S16 cmPkDamCfgCfm(Pst *pst,LwrMngmt *cfg,CmStatus *cmStatus);
EXTERN S16 cmPkDamCntrlCfm(Pst *pst,CmStatus *cmStatus,LwrMngmt *cntrl);
EXTERN S16 cmPkDamSendCtxtRel(Pst *pst,U16 crnti,U16 cellId,U8  event);
EXTERN S16 cmPkDamSendAlarmInd(Pst *pst,U16 category,U16   event,U16  cause);
EXTERN S16 cmPkDamPdcpDatCfm(Pst *pst,CmLtePdcpId  pdcpId);
EXTERN S16 cmPkTnlCreatCfm(Pst *pst,U32 transId,U8 status,U32 lclTeid);
EXTERN S16 cmPkTnlModCfm (Pst *pst,U32 transId,U8 status,U32 lclTeid);
EXTERN Void cmUnpkDamSendAlarmInd(Pst *pst,Buffer *mBuf);
EXTERN S16 cmPkDamCfgReq(Pst *pst,LwrMngmt *cfg);
EXTERN S16 cmPkDamCntrlReq(Pst *pst,LwrMngmt *cntrl);
EXTERN Void cmUnpkDamCfgReq(Pst *pst,Buffer *mBuf);
EXTERN Void cmUnpkDamCntrlReq(Pst *pst,Buffer *mBuf);
EXTERN Void cmUnpkDamCfgCfm(Pst *pst,Buffer *mBuf);
EXTERN Void cmUnpkDamCntrlCfm(Pst *pst,Buffer *mBuf);
EXTERN S16 cmPkTnlModReq(Pst *pst,U32 transId,WrDamTnlModInfo *tnlModInfo);
EXTERN Void cmUnpkTnlModReq(Pst *pst,Buffer *mBuf);
EXTERN Void cmUnpkTnlModCfm(Pst *pst,Buffer *mBuf);
EXTERN Void cmUnPkUeDelCfm(Pst *pst,Buffer *mBuf);
EXTERN Void cmUnpkDamPdcpDatCfm(Pst    *pst,Buffer *mBuf);
EXTERN S16 cmPkUeDelCfm (Pst *pst,U32 transId,U16 cellId,U16 crnti);
EXTERN S16 cmPkReestabCfm(Pst *pst,U32 transId,U8 status);
EXTERN Void cmUnPkHdlHoFailure( Pst *pst,Buffer *mBuf);
EXTERN S16 cmPkEgtpErrInd(Pst *pst,U32 lclTeid,U32 numDrbs,U16 crnti);
EXTERN Void cmUnpkEgtpErrInd(Pst *pst,Buffer *mBuf);

#endif /* __WRIFMDAMUTL_H__ */

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

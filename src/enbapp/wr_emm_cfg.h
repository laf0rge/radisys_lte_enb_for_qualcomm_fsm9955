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

     Name:     EnodeB Application

     Type:     C include file

     Desc:     This file contains 
               

     File:     wr_emm_cfg.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

#ifndef __WR_EMM_CFG_H__
#define __WR_EMM_CFG_H__

#ifdef __cplusplus
extern "C" {
#endif /* end of __cplusplus */

typedef struct wrEmmEnodCfgParama
{
   U8                        numEnb;
   WrNeighEnbCb              *neighEnbCb[MAX_NUM_NEIGH_CELLS];
} WrEmmEnodCfgParama;

EXTERN S16 WrEmmCfgReq (LwrMngmt *cfg,CmStatus *status);
EXTERN S16 WrEmmDynCfgReq(Void *cfg, CmStatus *cfmSts, U32 cfgType, Bool cfgPriority);
#if 0
 PUBLIC WrCdma2kNeighCellCb * wrEmmFindCdma1xRTTNeighCell
 (
 U8                          cellIndex,
 U16                         pnOffset
 );
 PUBLIC WrCdma2kNeighCellCb * wrEmmFindCdmaHRPDNeighCell
 (
 U8                          cellIndex,
 U16                         pnOffset
 );
#endif
PUBLIC WrUtraNeighCellCb  * wrEmmFindUtranNeighCell
(
U8              cellIndex,
U16               psc
);
PUBLIC S16 wrEmmBldEnbCfgTranFrmEnbId
(
WrEmmEnodCfgParama     *enbCfgparama,
U16                     cellId
);

PUBLIC S16 wrEmmProcRmCellRecfgInd
(
RmuCellRecfgInd         *rrmWrCellRecfgInd
);

PUBLIC  S16 wrEmmFindServedNeighCell
(
WrNeighEnbCb                *neignEnb,
U16                         pci
);

PUBLIC S16 wrEmmSndLwrAddNeighCfm
(
  LwrMngmt                     *cfg
);

EXTERN Bool wrEmmPlmnPlmnsEqual
(
 WrPlmnId                   *plmnId1,
 WrPlmnId                   *plmnId2
);

PUBLIC WrNeighEnbCb* wrEmmAddNewNeighEnb
(
 WrEutranNeighCellCb           *neighCellCb,
 U16                          cellId
);

EXTERN Void wrEmmMmeBldEnbConfigTransfer(WrEmmEnodCfgParama *enbCfgparama,U16 cellId);

EXTERN S16 wrEmmCfgCellAddSib8
(
 WrCellCb                     *cellCb,
 LwrCellSib8CfgGrp            *sib8CfgGrp
);
EXTERN S16 wrEmmCfgCellAddCsfbCfg
(
 WrCellCb                 *cellCb,
 LwrCsfbCfgGrp            *csfbCfgGrp
);
EXTERN S16 wrEmmCfgValidateNeighFreqAddCdma1xRtt( WrCellCb *cellCb,
LwrNeighFreqAddModCfg *nbrFreqAdd);
EXTERN U16 wrEmmCfgNeighBandClassCfg(LwrNeighBandClass *neighBandClsCfg);

EXTERN S16 wrEmmCfgNeighFreqAddCdma1xFreq( WrCellCb *cellCb,
LwrNeighCdma1xFreq               *cdma1xFreq);

EXTERN S16 wrEmmCfgNeighCellAddCdma1xRtt
(
LwrNeighCellAddCfg           *neighCellAdd,
WrCellCb                     *cellCb
);

EXTERN S16 wrEmmPciModTmrExp
(
 PTR   cb
);


/* CSG_DEV start */
EXTERN S16 wrEmmCsgCopyCsgCfg (WrCellCb *cellCb, LwrAddCellCfg *cfgAdd);
EXTERN S16 wrEmmNbrCfgCopyCsgCfg (WrEutranNeighCellCb *eutNbrcellCb,
      LwrNeighCellAddCfg *nbrCellCfg);
/* CSG_DEV end */

EXTERN S16 wrEmmSendAddEutraNghInd(WrEutranNeighCellCb *eutraNgh,LwrActType actType);
EXTERN S16 wrEmmSendUtraNghInd(WrUtraNeighCellCb *utraNgh,LwrActType actType);
EXTERN S16 wrEmmSendAddGeranNghInd(WrGeranCellCb *eutraNgh,LwrActType actType);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of __WR_EMM_CFG_H__ */

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

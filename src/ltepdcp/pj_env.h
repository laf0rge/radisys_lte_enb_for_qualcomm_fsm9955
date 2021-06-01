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
  
        Name:     LTE RLC file 
    
        Type:     C include file
  
        Desc:     This file contains the constants required for LTE-RLC.
                  The defines in this file need to be changed by the customer 
                  to reflect the software architecture needed to run under the 
                  target system hardware architecture.

        File:     kw_env.h

        Sid:      pj_env.h@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 18:21:46 2015
  
        Prg:      nm 
  
*********************************************************************21*/
/** @file pj_env_brcm.h
@brief PDCP Hash definitions
*/

#ifndef __PJENVBRCMH__
#define __PJENVBRCMH__

#define PJ_MAX_PDCP_INSTANCES            2 
#define PJ_FWD_MAX_SDU_CNT            16 

#define PJ_ALLOC_BUF(gCb,_buf)                                 \
{                                                          \
   if (SGetMsg(1, 0, (Buffer **)&_buf) != ROK) \
   {                                                       \
      (_buf) = NULLP;                                      \
   }                                                       \
}

#define PJ_CPY_MSG(_cb,_x, _y, _ret)                           \
   _ret =(SAddMsgRef((_x), 1, PJ_GET_MEM_POOL(_cb), (_y)))


#define PJ_SND_CFM(_cb,_pjRbCb, _txEnt, _cause)                \
{                                                          \
   PjuDatCfmInfo _datCfm;                                 \
   CmLtePdcpId _pdcpId;                                   \
   PjPjuSapCb  *_pjuSap;                                   \
   /* KLOCK warning fix */                           \
   _datCfm.numSdus = 0;                               \
   if (_pjRbCb->rbType == PJ_SRB)                          \
      _pjuSap   = &(_cb->u.dlCb->pjuSap[PJ_SRB_SAP]);                  \
   else                                                    \
      _pjuSap   = &(_cb->u.dlCb->pjuSap[PJ_DRB_SAP]);                  \
                                                           \
   _datCfm.cfmSta[_datCfm.numSdus].sduId = _txEnt->sduId;\
   _datCfm.cfmSta[_datCfm.numSdus].status = _cause;   \
   _datCfm.numSdus++;                                  \
   _pdcpId.rbId   = _pjRbCb->rbId;                     \
   _pdcpId.rbType = _pjRbCb->rbType;                   \
   _pdcpId.ueId   = _pjRbCb->ueCb->key.ueId;           \
   _pdcpId.cellId = _pjRbCb->ueCb->key.cellId;         \
   PjUiPjuDatCfm(&(_pjuSap->pst), _pjuSap->suId, &_pdcpId, &_datCfm);\
   pjDbmDelTxEnt(_cb, &(pjRbCb->dlCb.txBuf), _txEnt->count);   \
}

#define PJ_ADD_PRE_MSG(_pdu, _hdr, _numBytes, _ret)        \
{                                                          \
   Data _liByte;                                           \
   U16  _bytes;                                            \
   _bytes = 0;                                             \
   while (_numBytes > _bytes )                             \
   {                                                       \
      _liByte = (U8) (_hdr >> (8*_bytes));                 \
      _ret = SAddPreMsg((Data )_liByte, _pdu);             \
      if ( _ret != ROK )                                   \
      {                                                    \
         break;                                            \
      }                                                    \
      _bytes++;                                            \
   }                                                       \
}
#endif /* __PJENVBRCMH__ */
  
/********************************************************************30**
  
         End of file:     pj_env.h@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 18:21:46 2015
  
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
/main/1      ---       nm   1. Initial release.
/main/2      ---       nm   1. LTERLC Release 2.1
/main/3      kw003.201 vp   1. Fixes for re-segmentation, segement offset
                               value and optimization 
/main/4      kw004.201 ap   1. Modified the values of KW_AM_MAX_PDUS_RLS and
                               KW_AM_MAX_UL_PDUS
*********************************************************************91*/

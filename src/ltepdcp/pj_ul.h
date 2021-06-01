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
  
        Desc:     This file contain the hash definations for RLC 
 
        File:     kw.h

        Sid:      pj_ul.h@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 18:21:53 2015
  
        Prg:      na 
  
*********************************************************************21*/
/** @file kw.h
@brief RLC Hash definitions
*/

#ifndef __PJ_ULH__
#define __PJ_ULH__
#define PJ_MAX_NUM_OF_BITS 4096

#define PJ_DBM_FETCH_UL_RBCB(_rbId, _rbType, _ueCb, _rbCb)     \
{                                                           \
   _rbCb = (_rbType==CM_LTE_SRB)?_ueCb->srbCb[_rbId]:_ueCb->drbCb[_rbId];\
}

/* HO_FIX: Changed the success value to ROK as RRC expects
           ROK or RFAILED */
#define  PJ_FILL_UL_SDU_CFM_INFO(_cb,_cfmInfo,_tempDlStaInfo,_ueCb)                    \
{                                                                 \
   U8 _rbCnt;                                                     \
   U8 _numRb = 0;                                                 \
   PjUlHoCfmInfo *_hoCfmInfo;                                       \
   CpjSduStaInfo  *_staInfo;                                      \
   Bool _rbPres;                                                  \
   _cfmInfo->ueId    = _ueCb->key.ueId;                           \
   _cfmInfo->cellId  = _ueCb->key.cellId;                         \
   _cfmInfo->transId = _ueCb->hoInfo->transId;                    \
   for(_rbCnt = 0; _rbCnt < PJ_MAX_DRB_PER_UE && _numRb < CPJ_MAX_DRB; _rbCnt++)          \
   {                                                              \
       _rbPres     = FALSE;                                       \
      _hoCfmInfo   =  &_ueCb->hoInfo->hoCfmInfo[_rbCnt];          \
      _staInfo     =  &_cfmInfo->sduStaInfo[_numRb];              \
      if(_hoCfmInfo->pres == TRUE)                                \
      {                                                           \
      _rbPres = TRUE;                                             \
      _staInfo->rbId  =  _hoCfmInfo->rbId;                        \
      _staInfo->dir   |= _hoCfmInfo->dir;                         \
      _staInfo->ulSduStaInfo.numBits   =  _hoCfmInfo->numBits; \
      _staInfo->ulSduStaInfo.ulBitMap  =  _hoCfmInfo->ulBitMap;\
      _staInfo->ulSduStaInfo.count     =  _hoCfmInfo->count;   \
      _hoCfmInfo->ulBitMap          =  NULLP;                  \
      }                                                        \
      if(tempDlStaInfo[_rbCnt].dlSduStaInfo.hoPres == TRUE)  \
      {                                                                \
       _rbPres = TRUE;                                                 \
       _staInfo->rbId  =  _tempDlStaInfo[_rbCnt].rbId;        \
       _staInfo->dir   |= _tempDlStaInfo[_rbCnt].dir;                         \
       _staInfo->dlSduStaInfo.count = _tempDlStaInfo[_rbCnt].dlSduStaInfo.count; \
      }                                                        \
      if(_rbPres != TRUE)                                      \
      {                                                        \
         continue;                                             \
      }                                                        \
      else                                                     \
      {                                                        \
        _numRb++;                                              \
      }                                                        \
  }                                                              \
   _cfmInfo->numRb = _numRb;                                   \
   _cfmInfo->status = ROK;                                     \
   _cfmInfo->reason = CPJ_CFG_REAS_NONE;                          \
   PJ_FREE(_cb,_ueCb->hoInfo->hoCfmInfo,                          \
              (PJ_MAX_DRB_PER_UE * sizeof(PjUlHoCfmInfo)));       \
   PJ_FREE(_cb,_ueCb->hoInfo, sizeof(PjUlHoInfo));                \
}

#define PJ_DBM_DELETE_UL_RBCB(_cb, _rbCb)                           \
{                                                           \
   /* Free memory for rbCb */                               \
   PJ_FREE(_cb, _rbCb, sizeof(PjUlRbCb));                            \
   PJ_LMM_RB_STS_DEC(_cb);                                     \
}

#endif /* __PJH__ */
  
/********************************************************************30**
  
         End of file:     pj_ul.h@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 18:21:53 2015
  
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
/main/1      ---       mn  1. Initial release.
/main/2      ---       nm  1. LTERLC Release 2.1
*********************************************************************91*/

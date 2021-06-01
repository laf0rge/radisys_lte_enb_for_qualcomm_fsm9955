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
  
        Name:     LTE RLC file for downlink and non real time tasks 
    
        Type:     C include file
  
        Desc:     This file contains helper macros for RLC downlink
                  and non real time task
 
        File:     kw_dl.h

        Sid:      kw_dl.h@@/main/TeNB_Main_BR/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Tue Jan 13 11:32:07 2015
  
        Prg:      na 
  
*********************************************************************21*/
/** @file kw_dl.h
@brief RLC Downlink helper macros 
*/

#ifndef __KW_DLH__
#define __KW_DLH__
#define KW_DL_INST 1
#define KW_MAX_RNG_DEQ_INDX 8

/* Below are the status used within RLC */
#define KW_SDU_OPAT_NOUPDATE 0
#define KW_SDU_OPAT_UPD_TX 1
#define KW_SDU_OPAT_UPD_RETX   2

/* Below are the status which needs to be updated to MAC */
#define KW_RGU_SDU_OPAT_NOUPDATE 0
#define KW_RGU_SDU_OPAT_UPDATE   1
#define KW_RGU_SDU_OPAT_BREACH   2

/* PDB Values for various QCI bearers : This value is considered 
   after taking into account a 20ms delay at the network */
#define KW_PDB_VALUE_FOR_QCI1    80
#define KW_PDB_VALUE_FOR_QCI2    130
#define KW_PDB_VALUE_FOR_QCI3    30
#define KW_PDB_VALUE_FOR_QCI4    280
#define KW_PDB_VALUE_FOR_QCI5    80
#define KW_PDB_VALUE_FOR_QCI6    280
#define KW_PDB_VALUE_FOR_QCI7    80
#define KW_PDB_VALUE_FOR_QCI8    280
#define KW_PDB_VALUE_FOR_QCI9    280

/**
 * @def KW_MEAS_IS_DL_IP_MEAS_ON_FOR_RB
 *
 *    This macro is used to check if DL IP throughput measurement is ON
 *    or off for the passed rb
 *
 *    Returns TRUE (non-zero) if measurement is ON else FALSE (zero)
 *
 * @param[in] _gCb     RLC DL Cb
 * @param[in] _rbCb    RLC downlink control block
 *
*/ 
#ifdef LTE_L2_MEAS
#define KW_MEAS_IS_DL_ANY_MEAS_ON_FOR_RB(_gCb, _rbCb)  \
     ((_rbCb->rlcId.rbType == CM_LTE_DRB) && \
            (_gCb->u.dlCb->kwL2Cb.measOn[_rbCb->qci]))

#define KW_MEAS_IS_DL_IP_MEAS_ON_FOR_RB(_gCb, _rbCb)  \
     ((_rbCb->rlcId.rbType == CM_LTE_DRB) && \
      (_gCb->u.dlCb->kwL2Cb.measOn[_rbCb->qci] & LKW_L2MEAS_DL_IP) && \
      (_rbCb->rbL2Cb.measOn & LKW_L2MEAS_DL_IP))

#define KW_MEAS_IS_DL_DELAY_MEAS_ON_FOR_RB(_gCb, _rbCb)  \
     ((_rbCb->rlcId.rbType == CM_LTE_DRB) && \
            (_gCb->u.dlCb->kwL2Cb.measOn[_rbCb->qci] & LKW_L2MEAS_DL_DELAY))

#define KW_MEAS_IS_DL_UU_LOSS_MEAS_ON_FOR_RB(_gCb, _rbCb)  \
     ((_rbCb->rlcId.rbType == CM_LTE_DRB) && \
            (_gCb->u.dlCb->kwL2Cb.measOn[_rbCb->qci] & LKW_L2MEAS_UU_LOSS))
#define KW_UPD_PDCP_L2_DLDELAY_STS(_kwCb, _kwRbCb, _delay)                     \
{                                                                              \
   if(((_kwRbCb)->rlcId.rbType == CM_LTE_DRB) &&                               \
      ((_kwCb)->u.dlCb->kwL2Cb.measOn[_kwRbCb->qci] & LKW_L2MEAS_DL_DELAY))    \
   {                                                                           \
      (_kwRbCb)->rbL2Cb.l2Sts[KW_L2MEAS_DL_DELAY]->dlPjSduDelay.sduDelay += _delay; \
      (_kwRbCb)->rbL2Cb.l2Sts[KW_L2MEAS_DL_DELAY]->dlPjSduDelay.numSdus++; \
   }                                                                    \
}

#define KW_UPD_L2_UU_LOSS_PKTS(_kwCb, _kwRbCb, _val)                         \
{                                                                 \
   if(((_kwRbCb)->rlcId.rbType == CM_LTE_DRB) &&                              \
      ((_kwCb)->u.dlCb->kwL2Cb.measOn[_kwRbCb->qci] & LKW_L2MEAS_UU_LOSS))              \
   {                                                              \
      (_kwRbCb)->rbL2Cb.l2Sts[KW_L2MEAS_UU_LOSS]->uuLoss.dLoss += _val;     \
   }                                                              \
}
#define KW_UPD_L2_UU_LOSS_POS_PKTS(_kwCb,_kwRbCb,_val)                         \
{                                                                 \
   if(((_kwRbCb)->rlcId.rbType == CM_LTE_DRB) &&                              \
      ((_kwCb)->u.dlCb->kwL2Cb.measOn[_kwRbCb->qci] & LKW_L2MEAS_UU_LOSS))              \
   {                                                              \
      (_kwRbCb)->rbL2Cb.l2Sts[KW_L2MEAS_UU_LOSS]->uuLoss.posPkts += _val;     \
   }                                                              \
}
/* Discard new changes starts */
#define KW_UPD_L2_DL_DISC_SDU_STS(_kwCb,_kwRbCb)                         \
{                                                                 \
   if(((_kwRbCb)->rlcId.rbType == CM_LTE_DRB) &&                              \
      ((_kwCb)->u.dlCb->kwL2Cb.measOn[_kwRbCb->qci] & LKW_L2MEAS_DL_DISC))              \
   {                                                              \
      (_kwRbCb)->rbL2Cb.l2Sts[KW_L2MEAS_DL_DISC]->dlDisc.discSdus++;     \
   }                                                              \
}                                              

#define KW_UPD_L2_DL_TOT_SDU_STS(_kwCb,_kwRbCb)                             \
{                                                                 \
   if(((_kwRbCb)->rlcId.rbType == CM_LTE_DRB) &&                              \
      ((_kwCb)->u.dlCb->kwL2Cb.measOn[_kwRbCb->qci] & LKW_L2MEAS_DL_DISC))   \
   {                                                              \
      (_kwRbCb)->rbL2Cb.l2Sts[KW_L2MEAS_DL_DISC]->dlDisc.totSdus++;      \
   }                                                              \
}

#define KW_UPD_L2_DECR_NONIP_PER_QCI_RB_COUNT(_kwCb, _kwRbCb)                 \
{                                                                 \
   if(((_kwRbCb)->rlcId.rbType == CM_LTE_DRB) &&                    \
      ((_kwCb)->u.dlCb->kwL2Cb.measOn[_kwRbCb->qci] & LKW_L2MEAS_DL_DISC))   \
   {                                                              \
      U32 idx1;                                                    \
      for (idx1 = 0; idx1 < LKW_MAX_L2MEAS; idx1++)                  \
      {                                                           \
         if(_kwCb->u.dlCb->kwL2Cb.kwL2EvtCb[idx1].measCb.measType & LKW_L2MEAS_DL_DISC)                 \
         {                                                        \
            if(_kwCb->u.dlCb->kwL2Cb.kwL2EvtCb[idx1].measCb.val.nonIpThMeas.measData[(_kwRbCb)->qci].totDrbsPerQci > 0) \
            {                                                                                               \
               _kwCb->u.dlCb->kwL2Cb.kwL2EvtCb[idx1].measCb.val.nonIpThMeas.measData[(_kwRbCb)->qci].totDrbsPerQci--;   \
               if (_kwCb->u.dlCb->kwL2Cb.kwL2EvtCb[idx1].measCb.val.nonIpThMeas.measData[(_kwRbCb)->qci].totDrbsPerQci == 0) \
               {                                                                                                 \
                  _kwCb->u.dlCb->kwL2Cb.kwL2EvtCb[idx1].measCb.val.nonIpThMeas.qci[(_kwRbCb)->qci] = 0;                      \
                  cmMemset((U8 *)&_kwCb->u.dlCb->kwL2Cb.kwL2EvtCb[idx1].measCb.val.nonIpThMeas.measData[(_kwRbCb)->qci], 0,  \
                     sizeof(_kwCb->u.dlCb->kwL2Cb.kwL2EvtCb[idx1].measCb.val.nonIpThMeas.measData[(_kwRbCb)->qci]));            \
               }                                                                                                 \
            }                                                                                                    \
         }                                                                                                       \
      }                                                                                                          \
   }                                                                                                             \
}
#else
#define KW_UPD_PDCP_L2_DLDELAY_STS(_kwCb, _kwRbCb, _delay)
#define KW_MEAS_IS_DL_IP_MEAS_ON_FOR_RB(_gCb, _rbCb)
#define KW_MEAS_IS_DL_DELAY_MEAS_ON_FOR_RB(_gCb, _rbCb)
#define KW_UPD_L2_DL_DISC_SDU_STS(_kwCb,_kwRbCb)
#define KW_UPD_L2_DL_TOT_SDU_STS(_kwCb, _kwRbCb)
#define KW_UPD_L2_DECR_NONIP_PER_QCI_RB_COUNT(_kwCb, _kwRbCb)
#endif


#endif /* __KW_DLH__ */
  
/********************************************************************30**
  
         End of file:     kw_dl.h@@/main/TeNB_Main_BR/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Tue Jan 13 11:32:07 2015
  
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
*********************************************************************91*/

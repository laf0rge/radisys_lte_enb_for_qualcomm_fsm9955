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
               

     File:     wr_emm_ecsfb_utils.h

     Sid:      wr_emm_ecsfb_utils.h@@/main/TeNB_Main_BR/4 - Mon Aug 11 16:53:09 2014

     Prg:      Sriky 

*********************************************************************21*/



#ifndef __WR_EMM_ECSFB_UTILS_H__
#define __WR_EMM_ECSFB_UTILS_H__

#include "wr_cmn.h"
#include "wr_emm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define WR_MSEC_DIFF                   315964800000
#define WR_EMM_INVALID_RAT_PRIORITY    0xFFFF

/* Start Fix for SIB8 */
#define WR_FILL_BIT_STR_VAR_LEN(_ptr, _len, _val, _pdu) {\
   S16      i;                                           \
   U8     _byteCnt = 0;                                  \
   U8     _reminder = 0;                          \
   U32    _val2= _val; \
   _ptr.pres = PRSNT_NODEF;                            \
   _ptr.len =  _len;                                   \
   if((_len % 8) > 0)                                     \
   {                                                    \
      _byteCnt = (_len/8)+1;                             \
   }                                                     \
   else                                                  \
   {                                                     \
      _byteCnt = _len/8;                                 \
   }                                                     \
   _reminder = ((_byteCnt * 8) - _len);                  \
   _val2 = _val2 << _reminder;                             \
   WR_GET_MEM(_pdu, _byteCnt * sizeof(U8), (Ptr *) & (_ptr.val));  \
                                                         \
   if(_ptr.val != NULLP)                                 \
   {                                                     \
      for(i = _byteCnt - 1; i >= 0; i--)                 \
      {                                                  \
         _ptr.val[i] = (U8)(0x000000FF & _val2);          \
         _val2 = _val2 >> 8;                               \
      }                                                  \
   }                                                     \
}
#define WR_FILL_BIT_STR_LEN15(_ptr, _len, _val, _pdu) {\
   S16      i;                                    \
   U16     _tmpLen;                             \
   U32    _val2= _val; \
   _ptr.pres = PRSNT_NODEF;                     \
   _ptr.len =  _len;                            \
   _tmpLen = _len;                              \
   if((_len%8) > 0)                              \
   {                                             \
      _tmpLen = (_len/8)+1;                         \
   }                                             \
   else                                          \
   {                                             \
      _tmpLen = _len/8;                             \
   }                                             \
   WR_GET_MEM(_pdu, _tmpLen * sizeof(U8), (Ptr *)&(_ptr.val));  \
                      \
   if(_ptr.val != NULLP)                         \
   {                                             \
      for(i = _tmpLen - 1; i >= 0; i--)                  \
      {                                          \
         _ptr.val[i] = (U8)(0x000000ff & _val2);                  \
         _val2 = _val2>>7;\
      }                                          \
   }                                             \
}
 
#define WR_FILL_BIT_STR_LEN12(_ptr, _len, _val, _pdu) {\
   S16      i;                                    \
   U16     _tmpLen;                             \
   U32    _val2= _val; \
   _ptr.pres = PRSNT_NODEF;                     \
   _ptr.len =  _len;                            \
   _tmpLen = _len;                              \
   if((_len%8) > 0)                              \
   {                                             \
      _tmpLen = (_len/8)+1;                         \
   }                                             \
   else                                          \
   {                                             \
      _tmpLen = _len/8;                             \
   }                                             \
   WR_GET_MEM(_pdu, _tmpLen * sizeof(U8), (Ptr *)&(_ptr.val));  \
                      \
   if(_ptr.val != NULLP)                         \
   {                                             \
      for(i = _tmpLen - 1; i >= 0; i--)                  \
      {                                          \
         _ptr.val[i] = (U8)(0x000000ff & _val2);                  \
         _val2 = _val2>>4;\
      }                                          \
   }                                             \
}
#define WR_EMM_GET_CEIL_POS_VALUE(X) ((X-(U64)(X)) > 0 ? (U64)(X+1) : (U64)(X))
#define WR_EMM_GET_CEIL_NEG_VALUE(X) ((U64)(X))
#define WR_EMM_GET_CEILING_VALUE(X) (((X) > 0) ? WR_EMM_GET_CEIL_POS_VALUE(X) : WR_EMM_GET_CEIL_NEG_VALUE(X))

EXTERN S16 wrEmmGetRatPriority(WrPlmnId *plmn, WrRatPriorityList *ratPriority, WrCellId cellId);
EXTERN Bool wrEmmIsDualRxSupported(WrCellId cellId);
EXTERN S16 wrEmmIsCsfbOrEcsfbMeasSup
(
WrCellId                    cellId, 
Bool                        csfbOrEcsfbSup,
U8                          *csfbOreCsfbSup
);
EXTERN Bool wrEmmIs1xcCdmaBandCfg(U32  bandClass, WrCellId cellId);
EXTERN S16 wrFillTknBStr64
(
TknStrBSXL                   *ie,
U16                          len,
U64                          val,
CmMemListCp                  *mem
);
EXTERN S16 wrEmmAnrGetCdma1xRttCell
(
   U8                     cellId,
   WrCdmaRttPilotLst      *cdmaRttPilotLst
);
EXTERN S16 wrEmmAnrGetCdma1xRttCellByPnOffset
(
   U8                     cellId,
   WrCdma1xRttPnOffset    *cdmaRttPnOffset,
   WrCdma1xRttPilotInfo   *cdmaPilotInfo
);
EXTERN Bool wrEmmIsDualRxTxSupported(WrCellId cellId);
EXTERN Bool wrEmmGetEcsfbAndDualRxTxSuppCfgs(WrCellId cellId);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __WR_EMM_ECSFB_UTILS_H__ */


/********************************************************************30**

           End of file:     wr_emm_ecsfb_utils.h@@/main/TeNB_Main_BR/4 - Mon Aug 11 16:53:09 2014

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
/main/1        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/

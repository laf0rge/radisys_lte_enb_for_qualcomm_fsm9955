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
               

     File:     wr_utils.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

#ifndef __WR_UTILS_H__
#define __WR_UTILS_H__

#include "wr_cmn.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of __cplusplus */

/*rel10*/
#define INVALID_ECN0                     0XFF

EXTERN S16 wrCpyCmTptAddr(CmTptAddr *dst, CmTptAddr *src);
EXTERN S16 wrCpyTptToNetAddr(CmNetAddr *dst, CmTptAddr *src);
EXTERN S16 wrFillTknU8(TknU8 *encParam, U8 val);
EXTERN S16 wrFillTknU16(TknU16 *encParam, U16 val);
EXTERN S16 wrFillTknU32(TknU32 *encParam, U32 val);
EXTERN S16 wrFillTknBStr32(TknBStr32 *ptr, U16 len, U32 val);
EXTERN S16 wrFillTknBitStr4(TknStr4 *ptr, U8 len, U8 *val);
EXTERN S16 wrFillTknStr4(TknStr4 *ptr, U8 len, U32 val);
EXTERN S16 wrFill2TknStr4(TknStr4 *ptr, U8 len, U8 *val);
EXTERN Bool wrUtlPlmnsEqual(WrPlmnId *plmnId1, WrPlmnId *plmnId2);
EXTERN S16 wrUtlGetS1apMsgIdx (U8 *msgIdx, S1apPdu *pdu);
EXTERN S16 wrUtlGetPlmnId (WrPlmnId *plmnId, SztPLMNidentity *plmnIe);
EXTERN S16 wrGetBitRate (SztBitRate *ie, U32 *val);
EXTERN S16 wrUtlFillBitRate (CmMemListCp *mem, TknStrOSXL *ptr,
      U16 len, U32 val);
EXTERN S16 wrSzGetIE (SztS1AP_PDU *pdu, U32 id, TknU8 **ie);
EXTERN S16 wrCzGetIE (CztX2AP_PDU *pdu, U32 id, TknU8 **ie);
EXTERN S16 wrUtlGetDscp (U16 cellId, U32 qci, U8 *dscp);
EXTERN S16 wrUtlGetRbMode (U16 cellId, U32 qci,U8 *rbMode);
EXTERN S16 wrUtlGetRbDir (U16 cellId, U32 qci, U8 *rbDir);
EXTERN S16 wrSztUtilsGetMsgIdx(U8 *msgIdx, S1apPdu *pdu);
EXTERN S16 wrUtlX2GetMsgIdx(CztX2AP_PDU   *pdu);
EXTERN S16 wrUtlGetTportAddr(TknStrBSXL *ie, CmTptAddr *addr);
EXTERN U32 wrUtlGetTeid(TknStrOSXL *ie);
EXTERN S16 wrUtlFillU32BSXL(CmMemListCp *mem, TknStrBSXL *ie, U32 val, U8 len);
EXTERN S16 wrFillTknStr4096BSXL(TknStrBSXL *ptr,U16 len,Data *val,
                                             CmMemListCp *mem);
EXTERN S16 wrFillTknStr16384BSXL(TknStrBSXL *ptr,U16 len,Data *val,
                                             CmMemListCp *mem);
EXTERN S16 wrRetrvBitMapfrm4096BSXL(TknStrBSXL *ptr, U16 *numOfBits, U8 **ulBitMap);
EXTERN S16 wrRetrvBitMapfrm16384BSXL(TknStrBSXL *ptr, U16 *numOfBits, U8 **ulBitMap);
EXTERN S16 wrUtlGetU32FrmBSXL(U32 *val, TknStrBSXL *ie);
EXTERN S16 wrUtlGetU16FrmBSXL(U16 *val, TknStrBSXL *ie);
EXTERN S16 wrGetU32FrmTknStr4(U32 *val, TknStr4  *ie);
EXTERN S16 wrGetU16FrmTknStr4(U16 *val, TknStr4  *ie);
EXTERN S16 wrFillTknStrOSXL1(TknStrOSXL *ie, U16 len, U32 val,
                                                            CmMemListCp *mem);
EXTERN S16 wrFillTknStrOSXL(TknStrOSXL *ie, U16 len, Data *val,
                                                            CmMemListCp *mem);
EXTERN S16 wrFillTknStrBSXL(TknStrBSXL *ie, U16 len, Data *val,
                                                            CmMemListCp *mem);
EXTERN S16 wrUtlFillPlmn(WrPlmnId *plmnId, SztPLMNidentity *plmnIe);

EXTERN S16 wrUtlGetDedCrnti(U16 *rnti, U16 cellId);
EXTERN S16 wrUtlFreeDedCrnti(U16 rnti, U16 cellId);
EXTERN S16 wrUtlGetDedPreamble (U16 *prmblId, U16 cellId);
EXTERN S16 wrUtlFreeDedPreamble (U16 prmblId, U16 cellId);
EXTERN S16 wrUtlGetU32FrmBStr32 (U32 *val, TknBStr32 *ie);
EXTERN S16 wrUtlX2GetCztIE (CztX2AP_PDU *pdu, U32 id, void **ie);
EXTERN S16 wrUtlX2GetX2UeId (S16 msgType, CztX2AP_PDU *pdu, U32 *ueX2apId);
EXTERN S16 wrUtlCopyPlmnId (WrPlmnId *plmn1, WrPlmnId *plmn2);
EXTERN S32 wrUpdateIpAddr (U8 *strIpAddr,CmTptAddr *binIpAddr);

EXTERN S16 wrFillTknS32(TknS32 *encParam,S32 val);
EXTERN void wrUtlPrntIncTansType(U8 transTyp,U16 crnti,U16 msgTyp, U32 transId);
EXTERN void getLogTimeStrng(char* ts);
   
EXTERN S16 wrUmmEncCsfbSectorId
(
 U16              marketId,
 U8               switchNum,
 U16              cellId,
 U8               sectorNum,
 TknStrOSXL *ptr,
 U8          len
 );
EXTERN Void wrGetCdmaParamFromCID
(
 U16  *sId,
 U16  *nId,
 U16  *baseId,
 U32  ratType,
 U64  cId
);
EXTERN S16 wrGetDrxQCIFeatureFlag(WrCellId cellId,Bool *flag);
EXTERN Bool wrChkUtranFddSupp(WrUeCapInfo *ueCap);

/* SPS changes starts */
PUBLIC S16 wrUtlGetSpsRnti(U16 *spsRnti, U16 cellId);
PUBLIC S16 wrUtlFreeSpsRnti(U16 spsRnti, U16 cellId);
/* SPS changes ends */

typedef enum wrRedirectionUtraSupp
{
	WR_REDIRECTION_NOT_SUPP,
	WR_REDIRECTION_REL9_SUPP,
	WR_REDIRECTION_REL10_SUPP
}WrRedirectionUtraSupp;

EXTERN Void  wrChkUtranTddERedirSupp
(
 WrUeCapInfo *ueCap,
 WrRedirectionUtraSupp *redirType
);

EXTERN Bool wrEmmGetNeighUtraTddSIInfo
(
 U16         psc,
 U16         arfcn,
 U16         cellId,
 TknStrOSXL  *utraSI
 );

/* DYNAMIC_OAM and HRL interaction changes -start */
EXTERN S16 wrUpdtHrlUeCb(U8  cellId);
/* DYNAMIC_OAM and HRL interaction changes -end */

EXTERN U8 wrPrintS1apCause 
(
   SztCause     *cause
);

EXTERN Void wrPrintS1apCauseInfo
(
U8                           causeType,
U8                           causeVal
);

EXTERN Void wrCpuLoadCntrl(CmLteCellId cellId);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of __WR_UTILS_H__ */

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

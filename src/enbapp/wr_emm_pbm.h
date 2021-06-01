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
               

     File:     wr_emm_pbm.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
#ifndef __WR_EMM_PBM_H__
#define __WR_EMM_PBM_H__


#ifdef __cplusplus
extern "C" {
#endif


#define RINVALID     (255)

#define WR_PBM_PWS_5SFN_OFFSET 5

/** @brief This enum is used to specify the type of action need to perform on
 *    schduling info list in SIB1.
 **/
typedef enum wrSibAction 
{
   WR_SIB_ACTION_ADD = 1,
   WR_SIB_ACTION_REMOVE
} WrSibAction;

typedef enum wrSiChange 
{
   WR_PBM_SEND_NONE = 0,
   WR_PBM_SEND_ONE_SI_MSG,
   WR_PBM_SEND_ALL_SI_MSGS
} WrSiChange;
/** @brief This enum is used to identify the pbm info is 
 *         present pbm info entry in pbmSchInfo table
 */
typedef enum 
{
   WR_PBM_TYPE_START_PAG_SIMOD = 1,
   WR_PBM_TYPE_START_PAG_ETWS,
   WR_PBM_TYPE_START_PAG_CMAS, 
   WR_PBM_TYPE_STOP_PAG_SIMOD,
   WR_PBM_TYPE_STOP_PAG_ETWS,
   WR_PBM_TYPE_STOP_PAG_CMAS,
   WR_PBM_TYPE_START_SI_MOD,
   WR_PBM_TYPE_NORMAL_PAG,
   WR_PBM_TYPE_PRI_ETWS, 
   WR_PBM_TYPE_SEC_ETWS, 
   WR_PBM_TYPE_CMAS,
   WR_PBM_TYPE_SIB8
} WrPbmInfoType;

/** @brief This enum specifies the paging message types
 * like ETWS indication or System information chages 
 * and add CMAS indicaion for future use also.
 */
typedef enum wrPageType 
{
   WR_PAGE_ETWS_IND = 0,
   WR_PAGE_CMAS_IND, 
   WR_PAGE_SYS_SI_CHAGE,
   WR_PAGE_SI_TYPE1
}WrPageType;


/** @brief This enum specifies the type of warning message to
 * be started.
 */
typedef enum wrWarningType
{
   WR_START_PRI_ETWS_IND = 3,
   WR_START_SEC_ETWS_IND,
   WR_START_BOTH_ETWS_IND,
   WR_START_CMAS_IND
}WrWarningType;




/** @brief This structure is used to store the Paging Record of Paging Message.
 */
typedef struct wrPagingRecord
{
   U8       ueIdType;   /*!< Type of UE-ID */
   union 
   {
      WrSTmsi  sTMSI;      /*!< S-TMSI of the UE */
      U8       imsi[22];   /*!< IMSI of the UE. IMSI size is 
                                min-6 Integer digits and 
                                max-21 Integer Digits */ /* KW FIX */
   } ueId;
   U8       domIndType; /*!< Domain indication type is either PS or CS */
   U32     arrvTimeInfo; /*!< Time info at which pag message receievd */
   U8      pagPriority;  /*!< Priority of paging Message */
}WrPagingRecord;


#define WR_PBM_INVALID_PO   255

#define WR_SET_PO_BIT_MASK(poBitMask, po)  poBitMask |= (1 << po)

#define WR_PBM_MAX_UEID     1024 

/** @def WR_PBM_MAX_PAGING_RECORDS 
 * This Macro indicates the maximum no of 
 * paging records per Paging Message.
 */
#define WR_PBM_MAX_PAGING_RECORDS  16  /*!< Macro for Maximum no of 
                                            Paging records per Paging Mesage */

/* Max value of "System Value Info Tag" in SIB1 */
#define WR_EMM_PBM_SIB1_MAX_VALUE_TAG 0x1f

/** @brief This structure is used to store Pending Paging Message 
 *         information in pending paging message table.
 */
typedef struct wrPagingMsgInfo
{
    U16                 pf;   /*!< Paging Frame */
    U8                  po;   /*!< Paging Occusion */
    U8                  numPagRcrds; /*!< Number of paging records 
                                          present in this Paging message */
    WrPagingRecord      pagRec[WR_PBM_MAX_PAGING_RECORDS]; /*!< Paging records */
}WrPagingMsgInfo;

/** @brief This structure is PBM Info entry in PBM Info table.
 */
typedef struct wrPbmInfo
{
   CmLList            lnk;   /*!< Link to next node of PBM Info*/
   WrPbmInfoType      pbmInfoType;   /*!< PBM Info type */
   WrPagingMsgInfo    pendPagMsg;   /*!< Pending Paging message with one or 
                                         multiple paging records */
}WrPbmInfo;

#define MAX_POSSIBLE_POS   4
/** @brief This structure is temparerly store on stack which contains 
 * UE specific Paging Message information.
 */
typedef struct wrUePagingMsgInfo
{
   U8      ueIdenType;    /*!< UE-identity in paging record is 
                               either IMSI type or S-TMSI type */
   union
   {
      WrSTmsi  sTMSI;      /*!< S-TMSI of the UE */
      U8       imsi[22];   /*!< IMSI of the UE. IMSI size is
                             min-6 Integer digits and
                             max-21 Integer Digits.As we are using
                             the first index for the storing
                             the length of IMSI */
   } ueIden;     /*!< It contians either IMSI or S-TMSI */
   U32     ue_Id;   /*!< UE-Id used for paging frame calculation*/
   Bool    pagingDrxPres;   /*!< set to TRUE, if UE specific DRX cycle present,
                                 otherwise set to FALSE */
   U8      ueSpcPagDrx; /* UE specific DRX Cycle */
   U16     pf;   /*!< Paging frame number which
                      is calculated based on TS 36.304*/
   U8      po;   /*!< Paging occusion which is
                      calculated based on TS 36.304*/
   U16     T;   /*!< T is Min (Default DRX/Paging Cycle, 
                               UE specific DRX paging cycle)*/
   U8      N;   /*!< N is Min( T, nB) where is nB is configured by OAM */
   U8      Ns;   /*!< Ns is Nb/T, which is used to calculate PO */
   U8      domIndType;   /*!< Domain indication is either PS or CS 
                              which is received thru S1-AP paging message*/
   U8      pagPriority;  /*!< Priority of paging Message */
   U32     arrvTimeInfo;
} WrUePagingMsgInfo;

/** @brief This structure is used when we need to configure MAC with one or all
 * SIs based on the Updating SIs and it is local strucure stored on stack 
 */
typedef struct wrSibMaping
{
   U8      sibNum;   /*!< SIB Number */
   U8      siIndx;    /*!< SI Index */
   U8      sibAction;   /*!< Add or Delete action */
   U8      reCfgOneOrAllSis;   /*!< Reconfigure the MAC with one or all SIs */
} WrSibMaping;
 
#define WR_MAX_NUM_ETWS_SIBS     2
#define WR_PBM_UE_PAGEID_S_TMSI   UEPAGID_S_TMSI
#define WR_PBM_UE_PAGEID_IMSI     UEPAGID_IMSI
EXTERN S16 wrBldDlBcchBchPdu(NhuDatReqSdus *nhDatReqEvntSdu, Buffer  **encBuf);
EXTERN S16 wrBldDlBcchDlSchPdu(NhuDatReqSdus *nhDatReqEvnt, Buffer  **encBuf);
EXTERN S16 wrSibBldMIBMsg(WrCellCb *, NhuDatReqSdus *, U16 );
EXTERN S16 wrSibBldSIB1Msg(WrCellCb *, NhuDatReqSdus *, U16 );
EXTERN S16 wrEmmPbmBldSI(WrCellCb *, NhuDatReqSdus *, WrSISchdInfo *, U16, U8 , U8 );
EXTERN S16 wrEmmPbmPorcPagingMsg(S1apPdu *s1apPagMsg);
EXTERN S16 wrPbmTTIInd(RgrTtiIndInfo *tmngInfo);
EXTERN S16 wrPbmAddOrUpdateSibs(U8 numSibs, U8 *sibs, U16 cellId);
EXTERN S16 wrEmmPbmSib1Config(WrCellId cellId, U16 cfgType); 
EXTERN S16 wrEmmPbmMibConfig(WrCellId  cellId);
EXTERN S16 wrEmmPbmCellCfg(WrCellCb *cellCb);
EXTERN S16 wrEmmPbmCellDel(WrCellCb *cellCb);
EXTERN S16 wrEmmPbmModifyPlmn(WrPlmnId *plmnId, Bool activate);
EXTERN S16 wrEmmFillRrcEvntHdr(NhuHdr *nhuHdr, U16 cellId,
                               U16 crnti, U32  transId);
EXTERN S16 wrEmmPbmSibConfig(U16 cellId);
EXTERN S16 wrPbmUpdSiSchedInfoLstInSIB1(WrCellCb *cellCb, WrSibAction action,
                                        U8 sibNum, U8 *retransSiIndx);
EXTERN S16 wrEmmPbmTrgNrUpdt(U16 cellId, U8 updtNrBitMap);
EXTERN S16 wrPbmStartPagingMsg(WrCellCb *cellCb, WrPageType pageType);

EXTERN S16 wrPbmDelSibs(U8 numSibs, U8 *sibs, U16  cellId);

/* PH04_CMAS */
EXTERN S16 wrPbmStopPagingMsg
(
 WrCellCb      *cellCb,
 WrPageType    pageType
);

EXTERN S16 wrEmmPbmBldEncWrngSeg
(
WrCellId                     cellId,
U8                           siIdx,
Buffer                       **pdu
);

EXTERN S16 wrEmmPbmWrngSiConfig
(
WrCellId                     cellId,
U8                           siIdx
);

EXTERN S16 wrEmmPbmWrngStopSi
(
WrCellId                     cellId,
U8                           siIdx
);




EXTERN S16 wrEmmPbmProcPagingMsg(S1apPdu *s1apPagMsg);
EXTERN S16 wrPbmAddOrUpdateSibs(U8   numSibs,U8   *sibs,U16  cellId );
EXTERN S16 wrSibBldCellAccInfo
(
WrCellCb                                  *cellCb,
NhuDatReqSdus                             *nhDatReqEvntSdu,
NhuSysInfoBlockTyp1cellAccessRelatedInfo  *cellAcc
);
EXTERN S16 wrPbmStartWarningMsg
(
    WrCellCb      *cellCb,
    WrWarningType    pageType
);
EXTERN S16 wrEmmPbmCellSchdReCfg
(
  U16                          cellId
);
EXTERN  S16 wrPbmSib8InsertRecord
(
 WrCellCb       *cellCb, 
 WrPbmInfo      *pbmInfo
);
EXTERN S16 wrSibBldSIHeader
(
WrCellCb                     *cellCb,
NhuDatReqSdus                *sibSdu,
U16                          schedSFN,
U8                           subframe,
U8                           numSibs
);
EXTERN Void wrEmmPbmFreePendMsgLst
(
 U16         sfn,
 WrCellCb    *cellCb
);

/* Maximum PAGING tti reference time. This is used when difference is taken
 * between the current time and the time in which paging message has been 
 * received. Once we reach this value, will be starting from zero. 
 * */
#define WR_MAX_PAGING_TTI_TIME        10240
#define WR_DEFAULT_PAGING_PRI        255

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __WR_EMM_PBM_H__ */


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

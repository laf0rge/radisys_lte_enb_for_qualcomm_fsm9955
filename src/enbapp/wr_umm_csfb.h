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
               

     File:     wr_umm_csfb.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

#ifndef __WR_UMM_CSFB_H__
#define __WR_UMM_CSFB_H__

#ifdef __cplusplus
extern "C" {
#endif /* end of __cplusplus */

typedef enum wrUmmCsfbTransState
{
   WR_UMM_CSFB_TRANS_STATE_RRC_CONREL = 1, 
   WR_UMM_CSFB_TRANS_STATE_MEAS_CFGD,
   WR_UMM_CSFB_TRANS_STATE_CSFB_INIT, /* no CSFB */
   WR_UMM_CSFB_TRANS_STATE_REDIR_SENT /* set after sending out RRC Connection Release with redirection info */
}WrUmmCsfbTransState;

//#define WR_UMM_MAX_CELLS_PER_MEASREPORT    8

#define WR_UMM_CSFB_RECFG_TRANSID          1 
#define WR_UMM_CSFB_RRC_CON_REL_TRANSID    2 
/*Internal Measurement Config structure:*/

/** @brief This structure used to store Arfcn , Report Type and Cell Type
 *  * information.
 *
 *   -  WrUmmMeasCellTyp      cellTyp      Cell type
 *   -  U8                    duplexTyp    Duplex Type
 *   -  WrMeasObjFreq         arfcn        Arfcn
 *   -  WrMeasRptCfgType      rptCfgType   Report Config Type
 **/
typedef struct wrUmmMeasRptnfo
{
   WrUmmMeasCellTyp          cellTyp;
   U8                        duplexTyp;
   WrMeasObjFreq             arfcn;
   WrMeasRptCfgType          rptCfgType;
} WrUmmMeasRptInfo;
#if 0
/** @brief This structure used to store CSFB related information in internal
 * message.

 - TknU32         csFallBackInd     CS fallback indicator
 - WrRegLai       regLai            Ue registered Location area identifier
 */ 
typedef struct wrUmmIntCsfbTransMsg
{
   TknU32                    csFallBackInd;
   WrRegLai                  regLai;
} WrUmmIntCsfbTransMsg;
#endif
/** @brief This structure used to store CSFB Transaction Specific
 * information.
 *
 *  -  U8               recvMeasRptCnt   Total Measuremrent report configured
 *  -  CmTimer          csfbCfgTmr       Timer for CS Fallback
 *  -  TknU32            csFallBackInd   CSFallBack Indicator
 *  -  WrRegLai          regLai          LAI
 *  -  WrUmmCsfbCandCellInfo  candCellInfo   candidate Cell Information
 *  -  WrUmmMeasTransInfo   measTransCb     measurement transaction control block
 **/
typedef struct wrUmmCsfbTransCb
{
   U8                       recvMeasRptCnt;
   WrUmmCsfbTransState      csfbState;/*CSFB Transaction state*/
   WrUmmTimer               csfbCfgTmr;
   TknU32                   csFallBackInd;
   WrRegLai                 regLai;
/* eCSFB - Updated for ecsfb */
   WrUmmRedirectInfo        *redirectInfo;
   WrUmmMeasTransInfo       measTransCb;
   U8                       ratPriority;
   WrCdmaRttPilotLst        *cdmaRttPilotLst;
} WrUmmCsfbTransCb;
/* Transaction States */
#if 0
typedef enum wrUmmCsfbGsmTransState
{
   WR_CSFB_GSM_TRANS_STATE_CSFB_INIT, /* no CSFB */
   WR_CSFB_GSM_TRANS_STATE_REDIR_SENT /* set after sending out RRC Connection Release with redirection info */
}WrUmmCsfbGsmTransState;
#endif /* 0 */
typedef enum wrUmmCsfbGsmUeState
{
   WR_CSFB_GSM_UE_STATE_CSFB_READY, /* no CSFB */
   WR_CSFB_GSM_UE_STATE_CCO_INIT, /* CCO has been chosen for CSFB */
   WR_CSFB_GSM_UE_STATE_CCO_IN_PROGRESS, /* CCO sent */
   WR_CSFB_GSM_UE_STATE_RRC_REDIR /* RRC Connection release with Redirection has been chosen for CSFB */
}WrUmmCsfbGsmUeState;
typedef struct wrUmmGeranFreqCb
{
   CmLList        freqLstEnt;         /*This is not used. This is declared just
                                       to maintain similarity with wrGeranFreqCb*/
   U16            arfcn;              /*BCCH ARFCN*/
   TknU8          pMaxGeran;          /*Max allowed Tx power on a UL carrier freq*/
   U8             bandInd;            /*Band Indicator: GSM 850, GSM 900, 
                                        DCS 1800, PCS 1900*/
   U8             qRxLevMin;          /*Min required Rx level of GERAN 
                                        cell*/
   U8             cellReselPriority;  /*Absolute priority of the concerned
                                        carrier frequency*/
   U8             threshXLo;          /*Threshold used when re-selecting towards
                                        a lower priority GERAN frequency group 
                                        than current serving E-UTRA frequency*/
   U8             threshXHi;          /*Threshold used when re-selecting towards
                                        a higher priority GERAN frequency group 
                                        than current serving E-UTRA frequency*/
   U8             nccPermitted;       /*Broadcasted in SIB 7*/
}WrUmmGeranFreqCb;
typedef struct wrUmmGeranCellCb
{
   CmLList         cellLstEnt;         /*This is not used. This is declared just to 
                                         maintain similarity with wrGeranCellCb*/
   WrUmmGeranFreqCb   *freqCb;
   WrPlmnId        plmnId;             /*PLMN Id*/
   U32	          nrIndex;	         /*NRT index*/	
   U16             lac;                /*Location Area Code*/
   U8              rac;                /*Routing Area Code*/
   U16             cId;                /*Cell Id*/
   U8              bsic;               /*Base Station Identifier Code: 
                                         bits 7 & 6 = 0, bits 5 4 3 = NCC, 
                                         bits 2 1 0 = BCC*/
   U8              nccPermittedMeas;   /*NCC permitted per Cell used for 
                                         measurement configuration*/
   Bool            isDtmCapable;       /*DTM capability of this cell*/
   U8              siPsiType;          /*to indicate SI or PSI*/
   U8              noOfsiPsi;          /*no. of SIs or PSIs*/
   WrSiInfoGeran   siOrPsi;            /*SI or PSI*/
   TknStrBSXL      nco;                /*network control order*/
   S8              offsetFreq;/*Geran Qoffset*/ 
}WrUmmGeranCellCb;

typedef struct wrUmmCsfbCcoTransCb
{
   U8                       choice;/* 00 -> redirection, 01 -> CCO w/o meas, 11 -> CCO with meas */
   WrUmmCsfbTransState           csfbState;/*CSFB GERAN Transaction state*/
   U8                       ratPriority;/*for KPIs*/
   U8                       cfgdNeighCnt;/*Number of neighbours cfgd -measObjCfgdNeig*/
   TknU32                   csFallBackInd;/*CS Fallback Indicator*/
   WrRegLai                 regLai; /*Registered LAI*/     
   WrUmmTimer               t304WaitTmr;/*T304 wait timer for re-establishment*/
   WrUmmTimer               measWaitTmr;/*Measurement wait timer for meas reports*/
   WrUmmGeranCellCb         *measObjCfgdNeig[LWR_MAX_NUM_NEIGH_GERAN_CELLS];/*array of pointers to all the neigh cells configured in measObj*/
   WrUmmMeasTransInfo       measCfgInfo;
}WrUmmCsfbCcoTransCb;

#if 0
/** @brief This function is responsible for configuring measurements for CSFB
 * Module.
 *
 * @details
 *
 * Function:wrUmmMeasCsfbCfg
 *
 *   Processing steps:
 *    - Delete the configured measuremnent to UE.
 *    - configure CSFB related measurements 
 *
 * @param[in]  crnti: Crnti
 * @param[in]  csfbTransCb: CSFB transaction
 * @return SUCCESS : ROK
 *         FAILURE : RFAILED
 */
PUBLIC S16 wrUmmMeasCsfbCfg(U16 crnti, WrUmmCsfbTransCb *csfbTransCb);

/** @brief This function is responsible for Providing Arfcn, rptConfig type and
 * Cell type from measuremnet report by measuremnt module.
 *
 * @details
 *
 * Function:wrUmmMeasGetArfcnAndRptType
 *
 *   Processing steps:
 *    - Get Arfcn, rptConfig type and Cell type from measurement Id.
 *
 * @param[in]  measRprt: pointer to measurement report
 * @param[out] measRprtInfo: pointer to measurement report information
 * @return SUCCESS : ROK
 *         FAILURE : RFAILED
 */
PUBLIC S16 wrUmmMeasGetArfcnAndRptType(NhuMeasResults *measRprt,WrUmmMeasRptInfo *measRptInfo);

/** @brief This function is responsible for handling CSFB related messages.
 *
 * @details
 *
 * Function:wrUmmCsfbProcessor
 *
 *   Processing steps:
 *    - Check the message type 
 *    - If  message type is measuremnet report call the measurement handler API
 *    - If  message type is reconfiguration complete start the CSFB measuremnt Timer
 *
 * @param[in]  *transCb: transaction control block 
 * @return SUCCESS : ROK
 *         FAILURE : RFAILED
 */
PUBLIC S16 wrUmmCsfbProcessor(wrTransCb *tranCb);

/** @brief This function will be called when CS Fallback Indicator
 *          is received either in context modication or intial context 
 *          setup Request.   
 * @details
 *
 *     Function: wrUmmCsfbReq
 *
 *         Processing steps:
 *         - Read the Configuration Parameter "if measurement required".
 *         - If measurement for CS Fallback is required, call the measurement 
 *           API for measurement reports.
 *         - If above mentioned parameter is not set,call wrUmmCsfbSndRrcConRelMsg()
 *             to send RRC Connection Release message to UE.In this case UE Cb will 
 *             have PSC's for fallback based on REM/periodic measurement for ANR. 
 *           
 *
 *
 * @param [in] ueCb
 * @param [in] cellId 
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 
PUBLIC S16 wrUmmCsfbReq
(
 WrUeCb                      *ueCb
 )
{
   RETVALUE(ROK);
}


/** @brief This function selects the WCDMA candidate arfcn and cells for CS fallback   
 *         from measurement  report, function will be called after
 *         getting measurement report for CSFB Module
 * @details
 *
 *     Function: wrUmmCsfbUpdtCandCell
 *
 *         Processing steps:
 *         1. check if UE context's PSC list for CS fallback have any element
 *             if no then allocate memory to store WCDMA PSCs.
 *             if yes then 
 *         2. Read PSC from the list of PSCs from measurement report.
 *         3. Check if PLMN ID and LAC of PSC read in above step, is same 
 *             as used currently.
 *         4. If yes ,check if power of this PSC is greater than 
 *             candidate PSC stored in UE Context,
 *                -if yes,delete existing content of PSC list  
 *                   in UE Control blcok and copy the PSC (measurement report) 
 *                   to ueCb and goto step 6.
 *                - if no, ignore this measurement report and return from function.
 *         5. If PLMN ID and LAC doesn't match. goto step 2.    
 *         6. Read the next PSC(until PSCs are there in the list ) from  
 *             measurement report.
 *         7. Check if PLMN ID and LAC of PSC read in above step, is same 
 *             as used currently,
 *             -If yes, copy/append the PSC to UE control blcok's PSC list.
 *             -If no, delete the PSC from measrement report and goto step 6.
 *
 * @param [in] ueCb
 * @param [in] measRpt 
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmCsfbUpdtCandCell
(
 WrUeCb                       *ueCb,
 NhuMeasResults               *measRpt
)
{
   RETVALUE(ROK);
}

/** @brief This function will create RRC Connection Release message for CSFB.    
 *        and should be called after cf fallback timer expires or all expected 
 *        Measurement reports received.  
 * @details
 *
 *     Function: wrUmmCsfbSndRrcConRelMsg
 *
 *         Processing steps:
 *         - Read PSC from UeCb's PSC list.
 *         - If WCDMA PSC exists in UECb, Check if UE is release-8 or release-9 supported 
 *           by reading UE's capability information(Stratum Release Indicator).
 *         - If UE is release-8 supported, fill the RRC Connection Release Message with
 *             arfcn read in first step and send it to UE.  
 *         - Else if UE is release-9 supported,check if redirection to UTRA with SI is supported
 *            by reading  e-RedirectionUTRA-r9(UE capability), if yes fill RRC Connection Release
 *            Message with SI of PSC read in first step else send without SI.
 *
 * @param [in] ueCb
 * @param [in] cellId 
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmCsfbSndRrcConRelMsg
(
 WrUeCb     *ueCb,
 U16        cellId
 )
{
   RETVALUE(ROK);
}

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*End of _WR_UMM_CSFB_H__*/

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

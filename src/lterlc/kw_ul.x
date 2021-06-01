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
  
        Name:    LTE RLC file for uplink and non real time tasks
    
        Type:    C include file
  
        Desc:    This file contains all the data structures and 
                 prototypes for LTE RLC in the uplink.
 
        File:    kw_ul.x

        Sid:      kw_ul.x@@/main/TeNB_Main_BR/5 - Mon Aug 11 16:43:30 2014
  
        Prg:     N/A 
  
*********************************************************************21*/
/** 
 * @file kw_ul.x
 * @brief RLC uplink structures, prototypes
*/

#ifndef __KW_ULX__
#define __KW_ULX__

/*
 *     The structures and variables declared in this file
 *     correspond to structures and variables used by
 *     the following TRILLIUM software:
 *
 *     part no.             description
 *     --------     ----------------------------------------------
 *     1000371      LTE-RLC
 *
*/
 
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct kwUlUeCb KwUlUeCb;

/** @defgroup um_uplink UM Uplink Module
*/

/** 
 * @brief  Structure to hold a  UM PDU
 *
 * @details
 *    - pdu : Buffer holding the UM PDU data
 *    - umHdr : UM PDU Header Information
 *    - pduSz : Length of the PDU excluding the header
*/
typedef struct kwUmRecBuf
{
   Buffer    *pdu;    /**< Buffer holding the UM PDU */
   KwUmHdr   umHdr;   /**< UM PDU Header Information */
   MsgLen    pduSz;   /**< PDU Size */
}KwUmRecBuf;

/** 
 * @brief  Structure to hold uplink information in UM mode for a particular RB
 *
 * @details
 *    - snLen : The sequence number length can be 5 bits or 10 bits. 
 *              Here it is stored as 1 or 2 (as the number of bytes)
 *    - recBuf : Holds all the received PDUs. PDU's are removed from this 
 *               after a SDU is formed or during restablishment
 *    - umWinSz : The window size is 512 for 10 bits sequence number and 16 
 *                for 5 bits sequence number
 *    - partialSdu : This is used to store the partially completed SDU. 
 *                   It remains till complete SDU is received
*/
typedef struct kwUmUl
{
   U8           snLen;         /**< Sequence number length */
   U8           reOrdTmrInt;   /**< Timer Interval */
   KwUmRecBuf   **recBuf;      /**< Reception buffer */
   KwSn         umWinSz;       /**< UM window size */
   U16          modBitMask;    /**< Bitmask for modulus to wrap around variables */
   KwSn         sn;            /**< Sequence number */
   KwSn         vrUr;          /**< VR(UR) - Receive state variable */
   KwSn         vrUh;          /**< VR(UH) - Highest received state variable */
   KwSn         vrUx;          /**< VR(UX) - Reordering state variable */
   CmTimer      reOrdTmr;      /**< Reordering Timer */
   Buffer       *partialSdu;   /**< Partial SDU - Remains till the complete SDU
                                                               is received */
}KwUmUl;
/*@}*/

/** 
 * @brief  Structure to hold a RLC AM PDU segment
 *
 * @details
 *    - lstEnt : This is required for the linked list in which the segments
 *               are stored
 *    - seg    : Holds the segment data
 *    - segSz  : The length of the segment in bytes
 *    - soEnd  : SOEnd
 *    - amHdr  : The AM Header for the PDU segment
 *
*/
typedef struct kwSeg
{
   CmLList   lstEnt;   /**< List entry for PDU segment */
   Buffer    *seg;     /**< PDU segment */
   MsgLen    segSz;    /**< Buffer Size */
   U16       soEnd;    /**< Segment Offset End */
   KwAmHdr   amHdr;    /**< AM header */
}KwSeg;

/*@}*/

/** 
 * @brief  Structure to hold a received AM PDU or segments of a PDU
 *
 * @details
 *    - pdu         : Holds the PDU data
 *    - pduSz       : Length of the PDU in bytes
 *    - amHdr       : The AM Header for the PDU  
 *    - segLst      : The length of the segment in bytes
 *    - expByteSeg  : The next expected segment for re-ordering
 *    - expSo       : The next expected SO so to be in sequence
 *    - allRcvd     : Whether all the segments for this PDU has been recevied
 *
*/
typedef struct kwAmRecBuf
{
   Buffer      *pdu;          /**< PDU buffer */
   MsgLen      pduSz;         /**< Buffer Size */
   KwAmHdr     amHdr;         /**< AM header Info */
   CmLListCp   segLst;        /**< PDU Segments list */
   KwSeg       *expByteSeg;   /**< Next expected byte segment */
   U16         expSo;         /**< Next expected seg offset */
   Bool        allRcvd;       /**< All bytes received or not */
}KwAmRecBuf;

/** @addtogroup ammode */
/*@{*/

/** 
 * @brief  Structure to hold information about an uplink AM Radio Bearer
 *
 * @details
 *    - recBuf           : Reception buffer
 *    - vrR              :  VR(R) - Receive state variable 
 *    - vrH              : VR(H) - Highest received state variable
 *    - vrX              : VR(X) - reorderig state variable
 *    - vrMr             : VR(MR) - Maximum acceptable receive state variable
 *    - vrMs             : VR(MS) - Maximum STATUS transmit state variable
 *    - staTrg           : Flag to indicate if status trigger occured
 *    - partialSdu       : Partial SDU - Remains till the complete SDU 
 *                         is received
 *    - expSn            : The expected sequence number for reassembly  
 *    - expSo            : The expected SO for reassembly
 *    - staProhTmr       : The Status Probihit Timer
 *    - staProhTmrInt    : Status Prohibit Timer interval (in ??)
 *    - reOrdTmr         : The Reordering Timer
 *    - reOrdTmrInt      : Re-ordering timer interval
 *    - gatherStaPduInfo : Whether to gather information required to create 
 *                         the STATUS PDU
 *
*/
typedef struct kwAmUl
{
   KwAmRecBuf   **recBuf;              /**< Reception buffer */
   KwSn         vrR;                   /**< VR(R) */
   KwSn         vrH;                   /**< VR(H) */
   KwSn         vrX;                   /**< VR(X)*/
   KwSn         vrMr;                  /**< VR(MR) */ 
   KwSn         vrMs;                  /**< VR(MS)*/
   Bool         staTrg;                /**< Whether status trigger occured */
   Buffer       *partialSdu;           /**< Partially received SDU */
   KwSn         expSn;                 /**< Expected SN for reassembly */
   U16          expSo;                 /**< Expected SO for reassembly */
   CmTimer      staProhTmr;            /**< T_status_prohibit Timer */
   U16          staProhTmrInt;         /**< Timer Interval */
   CmTimer      reOrdTmr;              /**< T_reordering Timer */
   U8           reOrdTmrInt;           /**< Timer Interval */
   Bool         gatherStaPduInfo;      /**< Gather STATUS PDU creation info*/
   Bool         isOutOfSeq;            /**< To identify whether packets are Out-Of-Seq or not */
}KwAmUl;

/*@}*/

/** 
 * @brief  Structure to hold uplink information about a Radio Bearer
 *
 * @details
 *    - rlcId    : RLC identifier, uniquely identifies a Radio Bearer
 *    - lch      : Information (type and id) of the logical channel associated 
 *                 with this Radio  Bearer.
 *    - mode     : The mode of the Radio Bearer; UM or AM
 *    - dir      : The direction of the Radio Bearer, downlink or uplink or both
 *    - inst     : Id of RLC instance where this Radio Bearer is present. Used
 *                 to find the instance from the Radio Bearer for memory needs 
 *                 as different instances might have different memory.
 *    - kwuSapId : KWU SAP identifier
 *    - udxSapId : UDX SAP idenrifier
 *    - transId  : Stores the transaction identifier used to communicate 
 *                 with MAC, the same value as sent by MAC is passed back 
 *                 for it to be able to corelate
 *    - m        : Mode of the RB (TM/UM/AM)
 *      - umDl   : Unacknowledged Mode downlink information
 *      - amDl   : Acknowledged Mode downlink information
*/
typedef struct _kwUlRbCb
{
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   KwL2MeasRbCb    rbL2Cb;              /**< RB measurement L2 Cb */
   KwUlUeCb        *ueCb;              /*!< Pointer to UeCb  */
   U8              qci;                 /**< qci of the RB */
   KwL2MeasIpThruput l2MeasIpThruput;   /**< Holds related parameter for
                                             DL/Ul ip throughput>*/
#endif /* LTE_L2_MEAS */
   CmLteRlcId     rlcId;      /**< RLC Identifier */
   KwLchInfo      lch;        /**< Logical Channel Info */
   CmLteRlcMode   mode;       /**< Entity Mode */
   U8             dir;        /**< Direction for UL/DL */
   Inst           inst;       /**< Tapa where Rb created Instance id */
   SpId           kwuSapId;   /**< KWU sap Id, to get the KwuSapCb */
   SpId           udxSapId;   /**< KWU sap Id, to get the KwuSapCb */
   U32            transId;    /**< Transaction Id for RLC */
   union          
   {
      KwUmUl   umUl;   /**< UM  mode Ul elements */
      KwAmUl   amUl;   /**< AM mode uplink elements */
   }m;   /**< RLC mode specific Info */
}KwUlRbCb;

/** 
 * @brief  Structure to hold mapping between logical channel and Radio Bearer
 *
 * @details
 *    - ulRbCb   : Pointer to the uplink Radio Bearer
*/
typedef struct kwUlLch
{
   KwUlRbCb *ulRbCb;   /**< Pointer to Uplink RbCb */
}KwUlLch;                                  

/** 
 * @brief  Structure to hold uplink information about the Cells
 *
 * @details
 *    - cellHlEnt : Information about cells are stored in a hash table. This is
 *                  required for that.
 *    - cellId    : Identity of the cell
 *    - rbCb      : Radio Bearers in the cell
 *    - lCh       : Logical Channels in the cell
 *    - selfPstUl : Pst structure for sending messages to self
*/
typedef struct kwUlCellCb
{
   CmHashListEnt   cellHlEnt;                 /**< Hash list entry for CellCb */
   CmLteCellId     cellId;                    /**< Cell Id */
   KwUlRbCb        *rbCb[KW_MAX_RB_PER_CELL]; /**< RbCbs within a Cell */
   KwUlLch         lCh[KW_MAX_LCH_PER_CELL];  /**< Logical channels in a cell */
   Pst             selfPstUl;
}KwUlCellCb;

/** 
 * @brief  Structure to hold uplink information about the UEs
 *
 * @details
 *    - ueHlEnt : Information about cells are stored in a hash table. This is
 *                required for that.
 *    - key     : Key to store/find the UE in the hashtable
 *    - srbCb   : Signaling Radio Bearers configured for the UE
 *    - drbCb   : Data Radio Bearers configured for the UE
 *    - lCh     : Logical Channels in the UE
*/
struct kwUlUeCb
{
   CmHashListEnt   ueHlEnt;                   /**< Hash list entry for UeCb */
   KwUeKey         key;                       /**< Hash list key for UeCb */
   KwUlRbCb        *srbCb[KW_MAX_SRB_PER_UE]; /**< SRB RbCbs within an UE */ 
   KwUlRbCb        *drbCb[KW_MAX_DRB_PER_UE]; /**< DRB RbCbs within an UE */ 
   KwUlLch         lCh[KW_MAX_LCH_PER_UE];    /**< Logical channels of an UE*/
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   U32             firstPacketTTI;            /*!< is first packet of the burst */
   U16             numActRb[LKW_MAX_QCI];     /**< number of RBs Active */
   Bool              isUlBurstActive;   /*!<Has the Burst started for UL IP Thrpt meas */
#endif /* LTE_L2_MEAS */
};

/** 
 * @brief  Structure to hold temporary data of configuration 
 *
 * @details
 *    - entUlCfgCfm      :  Holds the UL configuration status  
 *    - rbCb             :  Rb Block
 */
typedef struct kwUlEntTmpData
{
   CkwEntCfgCfmInfo   entUlCfgCfm; /**< Ul Configuration status*/ 
   KwUlRbCb           *rbCb;       /**< Rb Block */
}KwUlEntTmpData;

/** 
 * @brief  Structure to hold transaction information in configuration request 
 *
 * @details
 *    - transHlEnt   : List entry for transaction block. 
 *    - transId      : Transaction Id
 *    - ueId         : ue Id
 *    - cellId       : cell Id
 *    - ueCb         : Ue Block
 *    - cellCb       : Cell Block
 *    - cfgInfo      : Configuration Information 
 *    - ueInfo       : UE Information
 *    - newUeInfo    : New Ue Information
 *    - cfgTmpData   : Temporary data per Configuration entity
*/
typedef struct kwUlCfgTmpData
{
   CmHashListEnt    transHlEnt;                  /**< List Entry of 
                                                   Transaction*/
   U32              transId;                     /**< Locally generated Transaction Id */
   U32              uprLyrTransId;               /**< Transaction Id generated by upper layer. 
                                                      This is used while sending confirm to the User Layer */
   CmLteCellId      cellId;                      /**< Cell Id */
   CmLteRnti        ueId;                        /**< Ue Id */
   KwUlUeCb         *ueCb;                       /**< Ue Block */
   KwUlCellCb       *cellCb;                     /**< Cell Block */
   CkwCfgInfo       *cfgInfo;                    /**< Config Information*/
   CkwUeInfo        *ueInfo;                     /**< Ue Information */
   CkwUeInfo        *newUeInfo;                  /**< New Ue Information */
   KwUlEntTmpData   cfgEntData[CKW_MAX_ENT_CFG]; /**< Entity Data */
} KwUlCfgTmpData;
/****************************************************************************
 *                      EXTERN Declarations
 ***************************************************************************/
/****************************************************************************
 *                    Configuration Functions 
 ***************************************************************************/

EXTERN S16 kwValidateRbCfgParams ARGS ((KwCb *gCb,
                                        CmLteRnti   ueId,
                                        CmLteCellId cellId,
                                        CkwEntCfgInfo  *cfgToValidate,
                                        CmStatus        *status));
EXTERN S16 kwCfgValidateUlRb ARGS (( KwCb *gCb,
                                    CkwEntCfgInfo *cfgToValidate,
                                    KwUlEntTmpData *cfgInfo, 
                                    KwUlCfgTmpData *cfg));

EXTERN S16 kwCfgRollBackUlRb ARGS ((KwCb *gCb,
                                    CmLteRnti ueId,
                                    CkwEntCfgInfo *cfgToValidate,
                                    KwUlEntTmpData *cfgTempData));

EXTERN Void kwCfgApplyUlRb ARGS ((KwCb *gCb,
                                  CkwEntCfgInfo *cfgToAply,
                                  KwUlEntTmpData *cfgTmpData,
                                  KwUlCfgTmpData *cfgTmpInfo));

EXTERN S16 kwCfgValidateReEstRb ARGS ((KwCb *gCb,
                                       CmLteRnti  ueId,
                                       CmLteCellId cellId,
                                       CkwEntCfgInfo *cfgToValidate,
                                       KwUlEntTmpData   *cfgTmpData));

EXTERN Void kwCfgApplyReEstUlRb ARGS ((KwCb *gCb,
                                      CmLteRnti ueId,
                                      CmLteCellId cellId,
                                      Bool sndReEstInd,
                                      KwUlEntTmpData *cfgTmpData));

EXTERN Void kwCfgApplyDelUlCell ARGS ((KwCb *gCb,
                                      KwUlCfgTmpData *cfgTmpData));

EXTERN S16 kwCfgValidateDelUlCell ARGS ((KwCb *gCb,
                                         CmLteCellId cellId,
                                         CkwEntCfgInfo *cfgToValidate,
                                         KwUlEntTmpData   *cfgTmpData,
                                         KwUlCfgTmpData *cfgInfo));

EXTERN S16 kwCfgValidateDelUlUe ARGS ((KwCb *gCb,
                                       CkwEntCfgInfo *cfgToValidate,
                                       KwUlEntTmpData *cfgTmpData,
                                       KwUlCfgTmpData *cfgInfo));

EXTERN Void kwCfgApplyDelUlUe ARGS ((KwCb *gCb,
                                    KwUlCfgTmpData *cfgTmpData));

EXTERN Void kwHdlUiCkwUlCfgReq ARGS ((KwCb *gCb,
                                     KwUlCfgTmpData *cfgInfo,
                                     CkwCfgInfo *cfg));

EXTERN Void kwCfgApplyUlUeIdChng ARGS ((KwCb *gCb,
                                        CkwUeInfo *ueInfo,
                                        CkwUeInfo *newUeInfo,
                                        KwUlCfgTmpData *cfgTmpInfo));

EXTERN S16 kwCfgValidateUeIdChng ARGS ((KwCb *gCb,
                                        CkwUeInfo *ueInfo,
                                        CkwUeInfo *newUeInfo,
                                        KwUlCfgTmpData *cfgTmpInfo));

/****************************************************************************
 *                    DBM module Functions 
 ***************************************************************************/
EXTERN S16 kwDbmUlInit ARGS ((KwCb *gCb));

EXTERN Void kwDbmUlDeInit ARGS ((KwCb *gCb));

EXTERN S16 kwDbmAddUlUeCb ARGS ((KwCb *gCb,
                                    CmLteRnti ueId,
                                    CmLteCellId cellId,
                                    KwUlUeCb *ueCb));

EXTERN S16 kwDbmFetchUlUeCb ARGS ((KwCb *gCb,
                                   CmLteRnti ueId,
                                   CmLteCellId cellId,
                                   KwUlUeCb **ueCb));

EXTERN Void kwDbmDelUlUeCb ARGS ((KwCb *gCb,   
                                  KwUlUeCb *ueCb,
                                  Bool abortFlag));

EXTERN Void kwDbmDelAllUlUe ARGS ((KwCb *gCb));

EXTERN S16 kwDbmAddUlCellCb ARGS ((KwCb *gCb,
                                      CmLteCellId cellId,
                                      KwUlCellCb *cellCb));

EXTERN Void kwDbmFetchUlCellCb ARGS ((KwCb *gCb, 
                                      CmLteCellId cellId,
                                      KwUlCellCb **cellCb));

EXTERN Void kwDbmDelUlCellCb ARGS ((KwCb *gCb,  
                                   KwUlCellCb *cellCb));

EXTERN Void kwDbmDelAllUlCell ARGS ((KwCb *gCb));

EXTERN Void kwDbmFetchUlRbCbByRbId ARGS ((KwCb *gCb, 
                                          CmLteRlcId *rlcId, 
                                          KwUlRbCb **rbCb));

EXTERN Void kwDbmFetchUlRbCbFromLchId ARGS ((KwCb *gCb, 
                                             CmLteRnti ueId, 
                                             CmLteCellId cellId, 
                                             CmLteLcId lcId,  
                                             KwUlRbCb **rbCb));

EXTERN Void kwDbmDelAllUlRb ARGS ((KwCb *gCb, 
                                   KwUlRbCb **rbCbLst, 
                                   U8 numRbCb));

EXTERN S16 kwDbmAddUlTransaction ARGS((KwCb *gCb, KwUlCfgTmpData *cfg));

EXTERN S16 kwDbmFindUlTransaction ARGS((KwCb *gCb, 
                                        U32 transId, 
                                        KwUlCfgTmpData **cfg));

EXTERN S16 kwDbmDelUlTransaction ARGS((KwCb *gCb, KwUlCfgTmpData *cfg));

EXTERN S16 kwDbmDelAllUlTransactions ARGS((KwCb *gCb));

EXTERN Void kwDbmUlShutdown ARGS ((KwCb *gCb));

/****************************************************************************
 *                    Transparent Mode Functions 
 ***************************************************************************/
#ifdef CCPU_OPT
EXTERN PUBLIC Void kwTmmRcvFrmLi ARGS ((KwCb *gCb, 
                                       KwUlRbCb *rbCb,
                                       CmLteRnti tCrnti, 
                                       Buffer *pdu));
#else /* CCPU_OPT */
EXTERN PUBLIC Void kwTmmRcvFrmLi ARGS ((KwCb *gCb,
                                       KwUlRbCb *rbCb,
                                       Buffer *pdu));
#endif /* CCPU_OPT */

EXTERN Void kwTmmUlReEstablish ARGS ((KwCb *gCb, KwUlRbCb *rbCb));

/****************************************************************************
 *                    Unacknowledged Mode Functions 
 ***************************************************************************/
#ifdef LTE_L2_MEAS
EXTERN Void kwUmmProcessPdus ARGS((KwCb *gCb,
                                  KwUlRbCb *rbCb, 
                                  KwPduInfo *pduInfo,
                                  U32 ttiCnt));
#else 
EXTERN Void kwUmmProcessPdus ARGS ((KwCb *gCb, 
                                    KwUlRbCb *rbCb,
                                    KwPduInfo *pduInfo));
#endif 
EXTERN Void kwUmmUlReEstablish ARGS ((KwCb *gCb, 
                                     CmLteRlcId *rlcId, 
                                     KwUlRbCb *rbCb));

EXTERN Void kwUmmReOrdTmrExp ARGS((KwCb *gCb, KwUlRbCb  *rbCb));


EXTERN Void kwUmmFreeUlRbCb ARGS ((KwCb *gCb, KwUlRbCb *rbCb)); 

/****************************************************************************
 *                    Acknowledged Mode Functions 
 ***************************************************************************/
EXTERN Void kwAmmUlReEstablish ARGS((KwCb *gCb, 
                                     CmLteRlcId rlcId, 
                                     Bool sndReEst,
                                     KwUlRbCb *rbCb));
#ifdef LTE_L2_MEAS
EXTERN Void kwAmmProcessPdus ARGS((KwCb *gCb,
                                  KwUlRbCb *rbCb, 
                                  KwPduInfo *pduInfo,
                                  U32 ttiCnt));
#else
EXTERN Void kwAmmProcessPdus ARGS((KwCb *gCb, 
                                  KwUlRbCb *rbCb,
                                  KwPduInfo *pduInfo));
#endif 

EXTERN Void kwAmmReOrdTmrExp ARGS((KwCb *gCb, KwUlRbCb *rbCb));

EXTERN Void kwAmmStaProTmrExp ARGS((KwCb *gCb, KwUlRbCb *rbCb));

EXTERN Void kwAmmFreeUlRbCb ARGS ((KwCb *gCb, KwUlRbCb *rbCb));

/****************************************************************************
 *                    Utility Functions 
 ***************************************************************************/

EXTERN S16 kwUtlRcvFrmLi ARGS ((KwCb *gCb, KwDatIndInfo *datIndInfo));

EXTERN S16 kwUtlSndDatInd ARGS ((KwCb *gCb,KwUlRbCb *rbCb, Buffer *sdu));

#ifdef LTE_L2_MEAS
EXTERN S16 kwUtlHdlL2TmrExp   ARGS (( KwCb *gCb, KwL2MeasEvtCb *measEvtCb));

EXTERN  Void kwUtlCalUlIpThrPutIncTTI ARGS ((KwCb *gCb, 
                                             KwUlRbCb  *rbCb,
                                             U32 ttiCnt));

EXTERN Void kwUtlCalUlIpThrPut ARGS((KwCb *gCb, 
                                     KwUlRbCb *rbCb, 
                                     Buffer *pdu, 
                                     U32 ttiCnt));

EXTERN S16 kwUtlSndUlL2MeasCfm  ARGS ((KwCb *gCb, KwL2MeasEvtCb *measEvtCb));

EXTERN S16 kwUtlSndUlL2MeasNCfm ARGS ((KwCb *gCb, 
                                       KwL2MeasReqEvt *measReqEvt,
                                       KwL2MeasCfmEvt *measCfmEvt));

EXTERN S16 kwUtlL2MeasUlInit ARGS((KwCb *gCb));

EXTERN Void kwUtlResetUlL2MeasInKwRb ARGS((KwCb *gCb,
                                           KwL2MeasCb *measCb,
                                           U8 measType));

EXTERN S16 kwUtlValidateIpThL2Meas ARGS ((KwL2MeasReqEvt *measReqEvt,
         KwL2MeasCfmEvt *measCfmEvt));

#endif /*  LTE_L2_MEAS */
/****************************************************************************
 *                    Activation Functions 
 ***************************************************************************/
EXTERN S16 kwUlActvInit ARGS ((Ent ent, 
                               Inst inst, 
                               Region region, 
                               Reason reason));

EXTERN S16 kwUlActvTsk ARGS ((Pst *pst, Buffer *mBuf));

/****************************************************************************
 *                    Debug Functions 
 ***************************************************************************/
EXTERN Void DumpRLCUlDebugInformation ARGS((Void));

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __KW_ULX__ */

  
/********************************************************************30**
  
         End of file:     kw_ul.x@@/main/TeNB_Main_BR/5 - Mon Aug 11 16:43:30 2014
  
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
/main/1      ---       nm  1. Initial release.
*********************************************************************91*/

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
  
        Name:    LTE RLC file 
    
        Type:    C include file
  
        Desc:    This file contains all the data structures and 
                 prototypes for LTE RLC.
 
        File:    kw_dl.x

        Sid:      kw_dl.x@@/main/TeNB_Main_BR/tenb_br_tdd_e1.2_4.0.0_cr/1 - Thu Nov 20 22:41:29 2014
  
        Prg:     N/A 
  
*********************************************************************21*/
/** 
 * @file kw_dl.x
 * @brief RLC downlink structures, prototypes
*/

#ifndef __KW_DLX__
#define __KW_DLX__


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
#ifdef TENB_STATS
#ifndef L2_l3_SPLIT
#include "pj_tenb_stats.x"
#else
#include "l2_tenb_stats.x"
#endif
#endif
typedef struct kwDlUeCb KwDlUeCb;

/** 
 * @brief  Structure to hold the SN of the PDU onto which a SDU is mapped
 *
 * @details
 *    - sn : Sequence number of the AM pdu
*/
typedef struct kwPduMap
{
  KwSn   sn;     /*!< Mapped PDU's SN */
}KwPduMap;

/** 
 * @brief  Structure to hold information regarding a SDU received from PDCP
 *
 * @details
 *    - lstEnt : Used for linking ( we have linked list of SDUs) which forms
 *               the SDU queue
 *    - mBuf   : Pointer to the SDU data
 *    - sduSz  : Size of the mBuf above
 *    - actSz  : Length of the SDU when received from PDCP, this might be 
 *               different from sduSz, in case when the SDU is split
 *    - mode   : Depending on the type, holds AM, UM or TM mode information
 *      - tm
 *        - sfn      : System Frame number for BCCH/PCCH
 *        - subframe : Subframe number
 *        - rnti     : RNTI for CCCH
 *      - um
 *        - sduId       : Unique identity for the SDU
 *        - isSegmented : Whether this SDU is segmented into multiple PDUs
 *      - am
 *        - sduId       : Unique identity for the SDU
 *        - rcvdSz      : Length of this SDU which is received by the peer
 *        - isSegmented : Whether this SDU is segmented into multiple PDUs
*/
typedef struct kwSdu                        
{
   CmLList   lstEnt;     /*!< List entry for SDU */
   Buffer    *mBuf;      /*!< SDU buffer */ 
   MsgLen    sduSz;      /*!< Buffer Size */
   MsgLen    actSz;      /*!< Actual buffer Size */
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   KwSduSnMap  *snMap;   /*!< Sdu and the list of SN it maps to */
#endif /* LTE_L2_MEAS */

   union
   {
      struct   _t 
      {
         U16         sfn;        /*!< Sub frame number for BCCH/PCCH */
         U8          subframe;   /*!< Subframe number */
         CmLteRnti   rnti;       /*!< RNTI for CCCH */
      }tm;   /*!< SDU for TM mode */

      struct   _u
      {
         U32    sduId;         /*!< SDU Identifier */
         Bool   isSegmented;   /*!< Flag to indicate if the SDU is segmented */
      }um;   /*!< SDU for UM mode */

      struct   _a
      {
         U32        sduId;                    /*!< SDU Identifier */
         MsgLen     rcvdSz;                   /*!< Recvd SDU Size from peer */
         Bool       isSegmented;              /*!< Is SDU segmented */
      }am;   /*!< SDU for AM mode */
   }mode;   /*!< RLC mode specific SDUs */
   /* Discard new changes starts */
   Ticks               arrTime;     /*!< Arrival time of the PDCP PDU at RLC */
   /* Discard new changes ends */

}KwSdu;

/** @addtogroup tmmode */
/*@{*/

/** 
 * @brief  Structure to hold Transparent Mode information for a RB
 *
 * @details
 *    - sduQ : Queue of SDUs
*/
typedef struct kwTm
{
   CmLListCp sduQ; /*!< SDU queue for TM */
}KwTm;
/*@}*/

/** 
 * @brief  Structure to hold buffer occupancy report sent to MAC
 *
 * @details
 *    - bo         : Size of the buffers at RLC
 *    - estHdrSz   : Size for all the headers as estimated by RLC
 *    - staPduPrst : Whether RLC wants to send a STATUS PDU or not
*/
typedef struct kwBoRep
{
   S32    bo;            /*!< Buffer occupancy */
   U16    estHdrSz;      /*!< Estimated header size for the bo */
   Bool   staPduPrsnt;   /*!< Flag to indicate if the bo is control PDU */
   U32    oldestSduArrTime;
} KwBoRep;

/** 
 * @brief  Structure to hold information sent to MAC in a Data Request
 *
 * @details
 *    - pduSz   : Sum total of sizes of all the PDUs being sent
 *    - pduInfo : Information about each PDU
 *    - boRep   : Current buffer occupancy report
*/
typedef struct kwDatReq
{
   MsgLen      pduSz;         /*!< PDU(s) size indicated by MAC */ 
   KwPduInfo   pduInfo;       /*!< PDU Information for lChs within UE */
   KwBoRep     boRep;         /*!< BO report */ 
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   KwLchMapInfo lchMapInfo;   /*!< Logical channel mapping info */
   U32          totMacGrant;
#endif /* LTE_L2_MEAS */
}KwDatReq;

/** @defgroup ummode UM Module Info 
*/

/** @addtogroup ummode */
/*@{*/
/** 
 * @brief  Structure to hold Unacknowledged mode downlink information for a RB
 *
 * @details
 *    - sduQ        : Control point for the queue of SDUs
 *    - snLen       : length of the squence number. Here it is stored 
 *                    as 1 for 5 bit sequence numbers and 
 *                    2 for 10 bit sequence numbers
 *    - bo          : Current buffer occupancy for this RB
 *    - estHdrSz    : Estimated size of headers required to send 
 *                    all the SDUs in the queue
 *    - vtUs        : VT(US)
 *    - modBitMask  : Bitmask used to do modulus operations to wrap around 
 *                    state variables, value is 0x1f or 
 *                    0x3ff for 5-bit and 10-bit sequence numbers respectively
 *    - numLi       : Number of length indicators to be sent in the next pdu
 *    - li          : Values of the lenght indicators
*/
typedef struct kwUmDl
{
   CmLListCp   sduQ;            /*!< SDU queue for UM */
   U8          snLen;           /*!< Sequence number length */
   S32         bo;              /*!< Buffer occupancy */
   S32         estHdrSz;        /*!< Estimated header size for BO */
   KwSn        vtUs;            /*!< VT(US) */
   U16         modBitMask;      /*!< Bitmask for modulus to wrap around vars*/   
   U16         numLi;           /*!< Number of LIs */
   U16         li[KW_MAX_LI];   /*!< Length Indicator array */
}KwUmDl;

/** 
 * @brief  Structure to hold information about which SDUs a PDU/segment carries
 *
 * @details
 *    - sduSz  : Size of the SDU or part of it
 *    - sdu    : Pointer to the SDU
*/
typedef struct kwSduMap
{
   /*MsgLen   actSz;*/         /*!< SDU Size */
   MsgLen   sduSz;         /*!< SDU Size */
   /*Bool     isSegmented;*/   /*!< Flag to indicate if the SDU is segmented */
   KwSdu*   sdu;           /*!< Pointer to the associated SDU */
}KwSduMap;

/** 
 * @brief  Structure to hold info about a PDU/segment to be re-transmitted
 *
 * @details
 *    - lstEnt           : Control point to link the re-transmit (the 
 *                         re-transmit PDUs  ,segments are stored as a 
 *                         link list) nodes
 *    - seg              : Pointer to the data
 *    - segSz            : Size of the PDU/segment including the header
 *    - hdrSz            : Size of the AM header for this in bytes
 *    - amHdr            : The header for this PDU/segment
 *    - soEnd            : SO End
 *    - retxCnt          : Number of times this has been re-transmitted
 *    - yetToConst       : Whether the PDU/segment has been constructed (headers
 *                         appended) or not
 *    - pendingReTrans   : TRUE if this node is to be re-transmitted else FALSE
 *    - numSdu           : Number of SDUs this PDU/segment carries
 *    - sduMap           : Infomation about the SDUs this node carries
*/
typedef struct kwRetx
{
   CmLList    lstEnt;              /*!< List entry for Retransmission PDU */
   Buffer     *seg;                /*!< Buffer of PDU or Segment */
   MsgLen     segSz;               /*!< Segment Size */
   MsgLen     hdrSz;               /*!< Header size for the segment */
   KwAmHdr    amHdr;               /*!< AM header Info */
   U16        soEnd;               /*!< Segment Offset End */
   KwSn       retxCnt;             /*!< Retransmission count */
   Bool       yetToConst;          /*!< Yet to construct seg flag */
   Bool       pendingReTrans;      /*!< Is node waiting for re-transmission */
   U8         numSdu;              /*!< Number of PDUs in sduMap */
   KwSduMap   sduMap[KW_MAX_LI];   /*!< Array of SDUs mapped with PDU */
}KwRetx;

/** 
 * @brief  Structure to hold info about a PDU that has been transmitted
 *
 * @details
 *    - pdu    : Pointer to the data
 *    - pduSz  : Size of the pdu, includes the size of the AM header
 *    - hdrSz  : Size of the AM header for this in bytes
 *    - amHdr  : The header for this PDU
 *    - numSdu : Number of SDUs this PDU carries
 *    - sduMap : Infomation about the SDUs this node carries
 *    - rlsLnk : Link to store on release list, used for 
*/
typedef struct kwTx
{
   Buffer     *pdu;                /*!< Buffer of PDU */
   MsgLen     pduSz;               /*!< PDU Size */
   MsgLen     hdrSz;               /*!< Header size for the PDU */
   KwAmHdr    amHdr;               /*!< AM header Info */
   U8         numSdu;              /*!< Number of PDUs in sduMap */
   KwSduMap   sduMap[KW_MAX_LI];   /*!< Array of SDUs for this PDU */
   CmLList    rlsLnk;              /*!< Lnk to store on release lst */
}KwTx;

/** @addtogroup ammode */
/*@{*/

/** 
 * @brief  Structure to hold Acknowledged Mode Downlink information for a RB
 *
 * @details
 *    - sduQ           : SDU queue
 *    - nxtTx          : Pointer to the next SDU to be transmitted
 *    - pollRetxTmr    : Poll re-transmission timer
 *    - pollRetxTmrInt : Timer interval (?? secs) of poll re-transmission timer
 *    - pollPdu        : Number of PDUs to send before sending a poll bit
 *    - pollByte       : Number of bytes to send before sending a poll bit
 *    - maxRetx        : Maximum number of re-transmissions to be done for a 
 *                       PDU/segment
 *    - cntrlBo        : Size of the STATUS PDU if it is to be sent, else 0
 *    - retxBo         : Bo of the PDUs/segments to be re-transmitted
 *    - estHdrSz       : Extimated header size of all the SDUs in the queue
 *    - bo             : Bo of the PDUs in the transmission buffer
 *    - txBuf          : Transmission buffer. It's contiguous index by sequence
 *                       number
 *    - rextLst        : Re-transmission buffer. It is a doubly linked list
 *    - nxtRetx        : Next node to be re-transmitted
 *    - vtS            : VT(S) - Send state variable 
 *    - vtMs           : VT(MS) - Maximum send state variable
 *    - vtA            : VT(A) - Acknowledgement state variable
 *    - pduWoPoll      : Number of PDUs sent without sending a poll bit
 *    - byteWoPoll     : Number of bytes sent without sending a poll bit
 *    - pollSn         : The sequence number on which the last poll bit was sent
 *    - numSduPdu      : ??
 *    - mBuf           : The encoded STATUS pdu to be sent
 *    - maxReTxReached : Whether maximum number of re-transmissions has been
 *                       reached or not
 *    - pStaPdu        : Stores the control pdu (information required to
 *                       create the STATUS PDU) passed from AM uplink
*/
typedef struct kwAmDl
{
   CmLListCp        sduQ;             /*!< SDU queue */
   KwSdu            *nxtTx;           /*!< Next SDU to be transmitted */
   CmTimer          pollRetxTmr;      /*!< T_poll_retransmit Timer */
   U16              pollRetxTmrInt;   /*!< Timer Interval */
   S16              pollPdu;          /*!< Poll_PDU */
   S32              pollByte;         /*!< Poll_Byte */
   U8               maxRetx;          /*!< Max_Retx_Threshold */
   S32              cntrlBo;          /*!< BO of control pdu */ 
   S32              retxBo;           /*!< BO of Retransmission PDUs */ 
   U32              estHdrSz;         /*!< Estimated header size for the BO */
   S32              bo;               /*!< BO of new PDUs */
   KwTx             **txBuf;          /*!< Transmission buffer */
   CmLListCp        retxLst;          /*!< Retransmission PDU list */
   KwRetx           *nxtRetx;         /*!< Next node to be retransmitted */
   KwSn             vtS;              /*!< VT(S) */
   KwSn             vtMs;             /*!< VT(MS) */
   KwSn             vtA;              /*!< VT(A)*/ 
   S16              pduWoPoll;        /*!< PDU without poll bit counter */
   S32              byteWoPoll;       /*!< Byte without poll bit counter */
   KwSn             pollSn;           /*!< Poll SN */
   S16              numSduPdu;        /*!< Number of SDUs/PDU to be sent*/
   Buffer           *mBuf;            /*!< this can be removed later 
                                           Stores the Control Pdu */
   Bool             maxReTxReached;   /*!< Set when the DL MAX Retx exceeded */
   KwUdxDlStaPdu*   pStaPdu;          /*!< The Control Pdu passed from UL*/
}KwAmDl;

/*@}*/

/** 
 * @brief  Structure to hold information about a Radio Bearer as required by RLC
 *
 * @details
 *    - rlcId          : RLC identifier, uniquely identifies a Radio Bearer
 *    - lch            : Information (type and id) of the logical channel 
 *                       associated with this Radio  Bearer.
 *    - mode           : The mode of the Radio Bearer TM or UM or AM
 *    - dir            : The direction of the Radio Bearer, downlink or 
 *                       uplink or both
 *    - inst           : Id of RLC instance where this Radio Bearer is present. 
 *                       Used to find the instance from the Radio Bearer for 
 *                       memory needs as different instances might have 
 *                       different memory.
 *    - kwuSapId       : KWU SAP identifier
 *    - udxSapId       : UDX SAP idenrifier
 *    - transId        : Stores the transaction identifier used to communicate 
 *                       with MAC, the same value as sent by MAC is passed back 
 *                       for it to be able to corelate
 *    - reestInProgress: Whether re-establishment is going on for this RB
 *    - m              : Mode of the RB (TM/UM/AM)
 *      - tm           : Transparent Mode information
 *      - umDl         : Unacknowledged Mode downlink information
 *      - amDl         : Acknowledged Mode downlink information
 *    - rlsLnk         : Link list used to free the RbCb
 *    - cleanupStarted : Whether cleanup of RB is in progress or not
*/
typedef struct _kwDlRbCb
{
   U8              qci;                /*!< qci of the RB */
#ifdef LTE_L2_MEAS
   KwDlUeCb        *ueCb;              /*!< Pointer to UeCb  */
   KwL2MeasRbCb    rbL2Cb;             /*!< RB measurement L2 Cb */
   CmLListCp       sduSnMapQ;          /*!< SDU SN map queue for UM */

   KwL2MeasIpThruput l2MeasIpThruput;  /*!< Holds related parameter for
                                                 DL/UL ip throughput>*/
#endif                                                 

   /* Discard new changes starts */
   S16             discTmrInt;        /*!< The discard time in milli 
                                           seconds for an PDCP SDU for this RB */
   /* Discard new changes ends */
   CmLteRlcId     rlcId;           /*!< RLC Identifier */
   KwLchInfo      lch;             /*!< Logical Channel Info  */
   CmLteRlcMode   mode;            /*!< Entity Mode */
   U8             dir;             /*!< Direction for UL/DL */
   Inst           inst;            /*!< Tapa Instance where rb created */
   SpId           kwuSapId;        /*!< KWU sap Id, to get the KwuSapCb */
   SpId           udxSapId;        /*!< UDX sap Id, to get the UdxSapCb */
   U32            transId;         /*!< Transaction Id for RLC */
   Bool  reestInProgress; /*!< Is reestablishment in progress */
   union          
   {
      KwTm     tm;     /*!< TM mode specific elements */
      KwUmDl   umDl;   /*!< UM downlink elements */
      KwAmDl   amDl;   /*!< AM downlink elements */
   }m;   /*!< RLC mode specific Info */
   CmLList   rlsLnk;   /*!< Link to add to release list */
   Bool      cleanupStarted; /*!< Whether cleanup of RB is in progress or not */
   U32       lastRprtdBoToMac; /*!< Last Reported BO to MAC Layer */
   U32       boUnRprtdCnt; /*!< Count to keep track of periodic BO Update */
}KwDlRbCb;

/** 
 * @brief  Structure to hold mapping between logical channel and Radio Bearer
 *
 * @details
 *    - dlRbCb   : Pointer to the downlink Radio Bearer
*/
typedef struct kwDlLch
{
   KwDlRbCb *dlRbCb;   /*!< Pointer to Downlink RbCb */
}KwDlLch;                                  

/** 
 * @brief  Structure to hold information about the Cells
 *
 * @details
 *    - cellHlEnt : Information about cells are stored in a hash table. This is
 *                  required for that.
 *    - cellId    : Identity of the cell
 *    - rbCb      : Radio Bearers in the cell
 *    - lCh       : Logical Channels in the cell
*/
typedef struct kwDlCellCb
{
   CmHashListEnt   cellHlEnt;                 /*!< Hash list entry for CellCb */
   CmLteCellId     cellId;                    /*!< Cell Id */
   KwDlRbCb        *rbCb[KW_MAX_RB_PER_CELL]; /*!< RbCbs within a Cell */
   KwDlLch         lCh[KW_MAX_LCH_PER_CELL];  /*!< Array of Logical channels */
}KwDlCellCb;

#ifdef LTE_L2_MEAS
/** @struct KwContSduLst
 * Structure contains list of rbId and index to KwOutStngSduLst
 */
typedef struct kwContSduLst
{
   CmLteLcId   lcId;    /*!< Holds the lcId of rbCb */
   U8          numSdus; /*!< Number sdus in sduIdx array */
   U8          sduIdx[KW_L2MEAS_MAX_OUTSTNGSDU];  /*!< Hold the index to KwOutStngSduLst */
}KwContSduLst;
typedef struct kwSduInfo
{
   Bool  isRetxPdu;
   Ticks arvlTime; /* stores the the arrival time of each PDCP PDU */
}KwSduInfo;

typedef struct kwlchInfo
{
   CmLteLcId   lcId;    /*!< Holds the lcId of rbCb */
   U8         numSdus; /*!< Number sdus belonged to the this LC */
   KwSduInfo sduInfo[KW_L2MEAS_SDUIDX];
}KwlchInfo;

/** @struct KwL2MeasTb
 * Structure containes list of rbId and  index to KwOutStngSduLst
 */
typedef struct kwL2MeasTb
{
   U8              numLcId;       /* number of logical channels in this TbCb */
   KwContSduLst    sduInfo[KW_MAX_ACTV_DRB]; /*!< Contained sduLst for 
                                               outstanding sdu */
   U8              numLchInfo;
   KwlchInfo       lchInfo[KW_MAX_ACTV_DRB]; /* Holds the LC info details 
                                                  used for DL delay L2 Meas*/
   U32             txSegSduCnt; /*!< This holds the no.of partially 
                                     transmitted SDU per TB which can be used for UU loss Meas*/
}KwL2MeasTb;
#endif

/** 
 * @brief  Structure to hold information about the UEs
 *
 * @details
 *    - ueHlEnt : Information about cells are stored in a hash table. This is
 *                required for that.
 *    - key     : Key to store/find the UE in the hashtable
 *    - srbCb   : Signaling Radio Bearers configured for the UE
 *    - drbCb   : Data Radio Bearers configured for the UE
 *    - lCh     : Logical Channels in the UE
*/
struct kwDlUeCb
{
   CmHashListEnt   ueHlEnt;                   /*!< Hash list entry for UeCb */
   KwUeKey         key;                       /*!< Hash list key for UeCb */
   KwDlRbCb        *srbCb[KW_MAX_SRB_PER_UE]; /*!< SRB RbCbs within a UE */ 
   KwDlRbCb        *drbCb[KW_MAX_DRB_PER_UE]; /*!< DRB RbCbs within a UE */ 
   KwDlLch         lCh[KW_MAX_LCH_PER_UE];    /*!< Array of Logical channels */
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   U16           numActRb[LKW_MAX_QCI];     /*!< number of RBs Active */
   U8            tbIdx;                /*!< TBIDx for  measTbCb */
   KwL2MeasTb    *l2MeasTbCb[KW_MAX_TB_PER_UE]; /*!< Array of Measurement tbCb */
#endif /* LTE_L2_MEAS */
#ifdef TENB_STATS
   TSL2UeStatsCb   *tenbStats; /*!< UE Stats Holder */
#endif
};

/****************************************************************************
 *                      EXTERN Declarations
 ***************************************************************************/
/****************************************************************************
 *                    Configuration Functions 
 ***************************************************************************/ 
EXTERN S16 kwCfgAddDlRb ARGS ((KwCb *gCb, 
                              CmLteRnti ueId, 
                              CmLteCellId cellId, 
                              CkwEntCfgInfo *entCfg, 
                              CkwEntCfgCfmInfo *entCfm));

EXTERN S16 kwCfgReCfgDlRb ARGS ((KwCb *gCb, 
                                 CmLteRnti ueId, 
                                 CmLteCellId cellId, 
                                 CkwEntCfgInfo *entCfg, 
                                 CkwEntCfgCfmInfo *entCfm));

EXTERN S16 kwCfgDelDlRb ARGS ((KwCb *gCb, 
                               CmLteRnti ueId, 
                               CmLteCellId cellId, 
                               CkwEntCfgInfo *entCfg, 
                               CkwEntCfgCfmInfo *entCfm));

EXTERN S16 kwCfgDelDlUe ARGS ((KwCb *gCb, 
                               CmLteRnti ueId, 
                               CmLteCellId cellId, 
                               CkwEntCfgInfo *entCfg, 
                               CkwEntCfgCfmInfo *entCfm));

EXTERN S16 kwCfgReEstDlRb ARGS ((KwCb *gCb, 
                                 CmLteRnti ueId, 
                                 CmLteCellId cellId, 
                                 Bool sndReEst,
                                 CkwEntCfgInfo *entCfg, 
                                 CkwEntCfgCfmInfo *entCfm));

EXTERN S16 kwCfgDelDlCell ARGS ((KwCb *gCb, 
                                 CmLteCellId cellId, 
                                 CkwEntCfgInfo *entCfg,
                                 CkwEntCfgCfmInfo *entCfm));

EXTERN S16 kwCfgDlUeIdChng ARGS ((KwCb *gCb, 
                                  CkwUeInfo *ueInfo, 
                                  CkwUeInfo *newUeInfo, 
                                  CmStatus  *status));

/****************************************************************************
 *                    DBM module Functions 
 ***************************************************************************/
EXTERN S16 kwDbmDlInit ARGS ((KwCb *gCb));

EXTERN Void kwDbmDlDeInit ARGS ((KwCb *gCb));

EXTERN S16 kwDbmCreateDlUeCb ARGS ((KwCb *gCb,     
                                    CmLteRnti ueId,   
                                    CmLteCellId cellId,
                                    KwDlUeCb **ueCb));

EXTERN S16 kwDbmFetchDlUeCb ARGS ((KwCb *gCb,        
                                   CmLteRnti ueId,      
                                   CmLteCellId cellId,     
                                   KwDlUeCb **ueCb));

EXTERN Void kwDbmDelDlUeCb ARGS ((KwCb *gCb,       
                                  KwDlUeCb *ueCb, 
                                  Bool abortFlag));

EXTERN Void kwDbmDelAllDlUe ARGS ((KwCb *gCb));

EXTERN S16 kwDbmCreateDlCellCb ARGS ((KwCb *gCb,      
                                      CmLteCellId cellId,   
                                      KwDlCellCb **cellCb));

EXTERN S16 kwDbmFetchDlCellCb ARGS ((KwCb *gCb, 
                                     CmLteCellId cellId, 
                                     KwDlCellCb **cellCb));

EXTERN Void kwDbmDelDlCellCb ARGS ((KwCb *gCb, KwDlCellCb *cellCb));

EXTERN Void kwDbmDelAllDlCell ARGS ((KwCb *gCb));

EXTERN Void kwDbmFetchDlRbCbByRbId ARGS ((KwCb *gCb, 
                                          CmLteRlcId *rlcId, 
                                          KwDlRbCb **rbCb));

EXTERN Void kwDbmFetchDlRbCbFromLchId ARGS ((KwCb *gCb,       
                                             CmLteRnti ueId,     
                                             CmLteCellId cellId,    
                                             CmLteLcId lcId,      
                                             KwDlRbCb **rbCb));

EXTERN Void kwDbmDelAllDlRb ARGS ((KwCb *gCb, KwDlRbCb **rbCbLst, U8 numRbCb));

EXTERN S16 kwDbmDlShutdown ARGS ((KwCb *gCb));

EXTERN Void kwUtlGetCurrTime ARGS((U32 *time));
EXTERN PUBLIC Void kwUtlTrigPdbFlowCntrl ARGS((KwCb *gCb, KwDlRbCb *rbCb, U32 pktAdmitCnt ));
#ifdef LTE_L2_MEAS
EXTERN Void kwDbmDelAllDlL2MeasTbFrmUe ARGS ((KwCb *gCb, KwDlUeCb *ueCb));

#if 0
EXTERN KwDlUeCb* kwDbmGetNextDlUeFromUeList ARGS ((KwCb *gCb, KwDlUeCb *ueCb));
#endif
#endif

/****************************************************************************
 *                    Transparent Mode Functions 
 ***************************************************************************/
EXTERN Void kwTmmQSdu ARGS ((KwCb *gCb,
                            KwDlRbCb *rbCb,
                            KwuDatReqInfo *datReqInfo,
                            Buffer *mBuf));
EXTERN Void kwTmmSndToLi ARGS ((KwCb *gCb,
                                KwDlRbCb *rbCb, 
                                RguCStaIndInfo *staInd));

EXTERN Void kwDlTmmReEstablish ARGS ((KwCb *gCb, KwDlRbCb *rbCb));

/****************************************************************************
 *                    Unacknowledged Mode Functions 
 ***************************************************************************/
EXTERN Void kwUmmQSdu ARGS ((KwCb *gCb, 
                             KwDlRbCb *rbCb,
                             KwuDatReqInfo *datReq, 
                             Buffer *mBuf));

EXTERN Void kwUmmDiscSdu ARGS ((KwCb *gCb, KwDlRbCb *rbCb, U32 sduId));

EXTERN Void kwDlUmmReEstablish ARGS ((KwCb *gCb, 
                                      CmLteRlcId rlcId, 
                                      Bool sndReEst,
                                      KwDlRbCb *rbCb));

EXTERN Void kwUmmProcessSdus ARGS ((KwCb *gCb,KwDlRbCb *rbCb,KwDatReq *datReq));

EXTERN Void kwUmmFreeDlRbCb ARGS ((KwCb *gCb, KwDlRbCb *rbCb));

/****************************************************************************
 *                    Acknowledged Mode Functions 
 ***************************************************************************/

EXTERN S32 kwAmmCalculateBo ARGS ((KwAmDl *amDl));

EXTERN Void kwAmmSendDStaRsp ARGS ((KwCb *gCb, KwDlRbCb *rbCb, KwAmDl *amDl));

EXTERN Void kwAmmQSdu ARGS((KwCb *gCb,
                            KwDlRbCb *rbCb,
                            Buffer *mBuf, 
                            KwuDatReqInfo *datReq));

EXTERN Void kwAmmProcessSdus ARGS((KwCb *gCb, 
                                   KwDlRbCb *rbCb, 
                                   KwDatReq *kwDatReq));

EXTERN Void kwAmmDlReEstablish ARGS((KwCb *gCb, 
                                     CmLteRlcId rlcId, 
                                     KwDlRbCb *rbCb));

EXTERN Void kwAmmDlHndlStatusPdu ARGS ((KwCb  *gCb,
                                        KwDlRbCb  *rbCb,
                                        KwUdxStaPdu *pStaPdu));

EXTERN S16 kwAmmDiscSdu ARGS((KwCb *gCb, KwDlRbCb *rbCb, U32 sduId));

EXTERN Void kwAmmPollRetxTmrExp ARGS((KwCb *gCB, KwDlRbCb *rbCb));

EXTERN Void kwAmmFreeDlRbCb ARGS ((KwCb *gCb, KwDlRbCb *rbCb));

/****************************************************************************
 *                    Utility Functions 
 ***************************************************************************/
EXTERN S16 kwUtlSndDStaRsp ARGS ((KwCb *gCb,
                                  KwDlRbCb *rbCb,
                                  S32 bo,
                                  S32 estHdrSz,
                                  Bool staPduPrsnt));

#ifdef LTE_L2_MEAS_RLC
EXTERN Void kwUtlEmptySduQ ARGS ((KwCb *gCb, KwDlRbCb *rbCb, CmLListCp *sduQ));
#else /* LTE_L2_MEAS */
EXTERN Void kwUtlEmptySduQ ARGS ((KwCb *gCb,CmLListCp *sduQ));
#endif /* LTE_L2_MEAS */

EXTERN Void kwUtlCalcLiForSdu ARGS ((KwCb *gCb,
                                     U16 numLi, 
                                     MsgLen msgLen,
                                     S16 *pduSz));

EXTERN S16 kwUtlSndToLi ARGS ((KwCb *gCb, KwDStaIndInfo *staIndInfo));

EXTERN Void kwUtlIncrementKwuStsSduTx ARGS((KwKwuSapCb *kwuSap));

EXTERN Void kwUtlIncrementGenStsBytesAndPdusSent ARGS((KwGenSts *genSts, 
                                                       Buffer *pdu));

EXTERN Void kwUtlFreeDlMemory ARGS ((KwCb *gCb));

EXTERN Void kwUtlInitToBeFreed ARGS ((KwCb *gCb, KwDlDataToBeFreed *toBeFreed));

EXTERN Void kwUtlInitializeSelfPst ARGS((KwCb *gCb));

EXTERN Void kwUtlRaiseDlCleanupEvent ARGS((KwCb *gCb));

EXTERN Void kwUtlAddSduToBeFreedQueue ARGS((KwCb *gCb, KwSdu *sdu));

EXTERN Void kwUtlAddReTxPduToBeFreedQueue ARGS((KwCb *gCb, KwRetx *retx));

EXTERN Void kwUtlAddTxPduToBeFreedQueue ARGS((KwCb *gCb, KwTx *pdu));

#ifdef LTE_L2_MEAS
EXTERN S16 kwUtlL2MeasDlInit ARGS((KwCb *gCb));
#endif

/****************************************************************************
 *                    Debug Functions 
 ***************************************************************************/
EXTERN Void ResetRLCStats ARGS((Void));

EXTERN Void PrintRLCStats ARGS((Void));

EXTERN Void DumpRLCDlDebugInformation ARGS((Void));

/****************************************************************************
 *                    Activation Functions 
 ***************************************************************************/
EXTERN S16 kwDlActvInit ARGS ((Ent ent,Inst inst,Region region,Reason reason)); 

EXTERN S16 kwDlActvTsk ARGS ((Pst *pst, Buffer *mBuf));

EXTERN Bool kwDlUtlIsReestInProgress ARGS ((KwDlRbCb *rbCb));

EXTERN Void kwDlUtlResetReestInProgress ARGS ((KwDlRbCb *rbCb));

EXTERN Void kwDlUtlResetReestInProgress ARGS (( KwDlRbCb *rbCb));

EXTERN Void kwDlUtlSetReestInProgressForAllRBs ARGS ((KwCb *gCb, KwDlUeCb
         *ueCb));
EXTERN Void kwDlUtlSetReestInProgressForRB ARGS (( KwCb *gCb, KwDlRbCb *rbCb));

#ifdef LTE_L2_MEAS
EXTERN Void kwUtlUpdateContainedSduLst ARGS ((
U8  sduIdx, 
KwContSduLst  *contSduLst
));
EXTERN Void kwUtlUpdateOutStandingSduLst ARGS ((
KwL2MeasDlIpTh   *dlIpThPut, 
U8               sduIdx, 
MsgLen           sduLen, 
U32              sduId,
Bool             newIdx
));
EXTERN Void kwUtlUpdateBurstSdus ARGS((
KwCb            *gCb,         
KwDlRbCb        *rbCb,
KwContSduLst    *contSduLst,
S32             dataVol,
U32             schPduSz
));

EXTERN KwL2MeasTb * kwUtlGetCurMeasTb ARGS((
KwCb     *gCb,
KwDlRbCb *rbCb
));

EXTERN S16 kwUtlSndDlL2MeasNCfm ARGS((KwCb *gCb,
                                      KwL2MeasReqEvt *measReqEvt,
                                      KwL2MeasCfmEvt *measCfmEvt));

EXTERN S16 kwUtlSndDlL2MeasCfm  ARGS ((KwCb *gCb, KwL2MeasEvtCb *measEvtCb));

EXTERN S16 kwUtlProcHarqInd  ARGS (( KwCb *gCb, RguHarqStatusInd *staInd, KwDlUeCb *ueCb, 
                                     U8 tbIdx));
#if 0
EXTERN S16 kwUtlSndDiscSduCfm ARGS (( KwRbCb *kwRbCb, U32    sduId));
EXTERN S16 kwUtlUpdSduSnMap   ARGS (( KwRbCb *kwRbCb, KwSdu  *sdu, 
                                      KwDatReq *kwDatReq, Bool   lastSegSent));
EXTERN Void kwUtlProcInitialActvUe ARGS ((KwL2MeasCb *measCb));
#endif
EXTERN Void kwUtlResetDlL2MeasInKwRb ARGS ((KwCb *gCb,
                                            KwL2MeasCb *measCb,
                                            U8 measType));

EXTERN S16 KwMiLkwL2MeasStopCfm ARGS((Pst *pst, U8  measType,U8  status));

EXTERN S16 kwUtlValidateIpThL2Meas ARGS((KwL2MeasReqEvt *measReqEvt, 
                                     KwL2MeasCfmEvt *measCfmEvt)); 
                                    
EXTERN S16 kwFetchLchInfo ARGS ((KwL2MeasReqEvt *measReqEvt,
                                 KwL2MeasCfmEvt *measCfmEvt,
                                 U16             ueId,
                                 CmLteLcId *lChId,
                                 U8 *numLch));

#if 0
EXTERN Void kwUtlResetKwRbL2Meas ARGS(( KwRbCb  *rbCb ));
#endif
EXTERN S16 kwUtlNotifyMacUlIp ARGS ((KwL2MeasCb *measCb,U16 ueIdx, Bool enable,
                                    CmLteLcId  *lChId, U8 *numLCh));
#endif
EXTERN Void kwUtlFreeDlMem ARGS(( Void));
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __KW_DLX__ */

  
/********************************************************************30**
  
         End of file:     kw_dl.x@@/main/TeNB_Main_BR/tenb_br_tdd_e1.2_4.0.0_cr/1 - Thu Nov 20 22:41:29 2014
  
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
/main/2      ---       nm  1. LTERLC Release 2.1
/main/3      kw002.201 vp  1. Patch for optimization,fixes for multi-region
                              and retransmission.
/main/4      kw003.201 vp  1. Fixes for re-segmentation, segement offset
                               value and optimization 
/main/5      kw005.201 ap  1. Updated KwRbCb for statistics calculation.
                           2. Removed compilation warning.
                           3. Added cellId and ueId KwL2MeasCb
                           4. Added declaration for kwUtlValidateL2Meas.
*********************************************************************91*/

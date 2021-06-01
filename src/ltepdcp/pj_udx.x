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
  
        Name:    LTE PDCP Interface  
    
        Type:    C include file
  
        Desc:    This file contains the Data structures for LTE PDCP Interface
 
        File:    kw_pj_udx.x

        Sid:      pj_udx.x@@/main/TeNB_Main_BR/tenb_main_ccb/tenb_5.0_RIB/tenb_5.0_SIB_KNOWNSENSE/5 - Tue Feb  9 11:15:56 2016
  
        Prg:      
  
*********************************************************************21*/
#ifndef __PJ_UDX_X__
#define __PJ_UDX_X__

#ifdef __cplusplus
EXTERN "C" {
#endif  /*for extern "C"*/

/**
 * @file 
 * @brief UDX Interface file
 */

/*************************************************************************
 *                      START OF STRUCTURES
 ************************************************************************/
/**
 * @brief
 * Cipher Key type information.
 */
typedef U8        UdxCipherKey;

/**
 * @brief
 * Integrity Key type information.
 */
typedef U8        UdxIntegrityKey;

/**
  * @brief
  * RLC Parameter Information in AM mode.
 */
typedef struct udxRlcAm
{
   Bool           statusRepReqd; /*!< Status report required */
   U8             pdcpSNSize; /*!< Length of PDCP sequence number */
}UdxRlcAm;

/**
 * @brief
 * RLC Parameter Information in UM mode.
 */
typedef struct udxRlcUm
{
   U8             pdcpSNSize; /*!< Length of PDCP sequence number */
}UdxRlcUm;

/**
 * @brief
 * Mapped RLC Entity Information in RLC for the PDCP Entity
 */
typedef struct udxRlcInfo
{
   U8             dir;       /*!< UPLINK/DOWNLINK/BOTH */
   U8             mode;      /*!< RLC Mode */ 
   union
   {
      UdxRlcAm    rlcAm;     /*!< RLC in AM mode */
      UdxRlcUm    rlcUm;     /*!< RLC in UM mode */
   }m;
}UdxRlcInfo;


/**
 * @brief
 * Ciphering Information to be configured per UE in PDCP.
 */
typedef struct udxCipherInfo         
{
   UdxCipherKey   ctrlplnKey[UDX_CIPHER_KEY_LEN];   /*!< Control plane
                                                        ciphering key */
   UdxCipherKey   usrplnKey[UDX_CIPHER_KEY_LEN];    /*!< User plane
                                                        ciphering key */
   U8             algoType;                         /*!< Algorithm type.
                                                    Allowed values are:
                                                    -# UDX_SEC_ALG_NULL 
                                                    -# UDX_SEC_ALG_SNOW3G
                                                    -# UDX_SEC_ALG_AES
                                                    */
}UdxCipherInfo;

/**
 * @brief
 * Integrity protection paramater information. 
*/
typedef struct udxIntegInfo
{
   U8                algoType;                  /*!< Algorithm type.
                                                   Allowed values are:
                                                   -# UDX_SEC_ALG_AES
                                                   -# UDX_SEC_ALG_SNOW3G
                                                   */
   UdxIntegrityKey   intKey[UDX_INTG_KEY_LEN];  /*!< Integrity key */
} UdxIntegInfo;

/**
 * @brief
 * PDCP Security configuration parameter.
 */
typedef struct udxSecCfg 
{
   Bool           selSecAct;        /*!< Selective security activation flag.

   PDCP applies only Integrity algorithm for the first UL and first DL
   messages (security activation procedure), applies both Integrity and
   ciphering for all the other messages (including handover which involves
   reconfiguration messages). selSecAct enables to identify whether
   PDCP needs to selectively apply only Integrity for first UL/DL message.
   */
   Bool           isHo;             /*!< Will be set true in case of HO > */
   UdxIntegInfo   integInfo;        /*!< Integrity configuration */
   UdxCipherInfo  cipherInfo;       /*!< Ciphering configuration */
} UdxSecCfg;

/**
 * @brief
 * ROHC Compression protocol information structure. 
 */
typedef struct udxRohcInfo
{
   U16            maxCid;                          /*!< Maximum CID */
   Bool           profLst[UDX_MAX_ROHC_PROF_LIST]; /*!< ROHC profile
                                                     information */
}UdxRohcInfo;

/**
 * @brief
 * ROHC Configuration information for PDCP entity. 
 */
typedef struct udxHdrCompCfg
{
   Bool           hdrCompUsed;   /*!< Header Compression Used/Not */
   UdxRohcInfo    rohcInfo;      /*!< ROHC configuration information */
}UdxHdrCompCfg;

/**
 * @brief
 * Handover Information for RB.
 */
typedef struct udxHoInfo
{
   U8             isHoPres;   /*!< Last 2bits (from LSB)is used to convey handover information is present or
                                not. @n Allowed values are:
                                    -# UDX_HO_NOT_PRSNT  00
                                    -# UDX_HO_UL         01
                                    -# UDX_HO_DL         10
                                    */
   U8             snLen;         /*!< PDCP-SN length 12/15 bits */
   U32            dlCount;       /*!< DL COUNT to assign for new SDUs */
   U32            ulCount;       /*!< UL First Missing SN */
   U16             numBits;       /*!< Number of bits in UL bitmap */
   U8             *ulBitMap;     /*!< UL SN bitmap excluding FMS to be used 
                                      as status report */
} UdxHoInfo;

/**
 * @brief
 * Configuraiton Structure to add an RB.
 */
typedef struct udxAddCfgEnt
{        
   Bool           discReqd;   /*!< Discard required/not for PDUs
                                with Integrity Verification failure */
   Bool           cfmReqd;    /*!< Confirm required/not for SDUs 
                                received by the peer PDCP entity. */
  /* cpj_x_001.main_3 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   U8             qci;        /*!< qCI value Associated with that RB */
#endif /* LTE_L2_MEAS */

   UdxRlcInfo     rlcInfo;    /*!< Corresponding RLC entity information. */
   S16            discardTmr; /*!< Discard timer. @n The value for this parameter
                                is expected in milli seconds. @n Value of infinity
                                is also allowed and -1 must be passed for
                                the same. */
   UdxHdrCompCfg  hdrCompCfg; /*!< PDCP compression information */
}UdxAddCfgEnt;         
         
/**
 * @brief
 * Configuraiton Structure to modify an RB.
 */
typedef struct udxModCfgEnt
{
      
   U8             bitFlag;     /*!< List of Modified Elements 
                                   Each bit respresents one element  */

   Bool           discReqd;   /*!< Discard required/not for PDUs
                                with Integrity Verification failure.
                                bitFlag=0x01 */
   Bool           cfmReqd;    /*!< Confirm required/not for SDUs 
                                received by the peer PDCP entity.
                                bitFlag=0x02 */
   UdxHdrCompCfg  hdrCompCfg; /*!< PDCP compression information */
   UdxHoInfo      hoInfo;     /*!< Handover Information */
}UdxModCfgEnt;


/**
 * @brief
 * Configuration structure for PDCP entity. 
 */
typedef struct udxCfgEnt
{
   U8             rbId;       /*!< PDCP ID. */
   U8             cfgType;    /*!< Configuration request type -
                                -# UDX_CFG_ADD,
                                -# UDX_CFG_MODIFY,
                                -# UDX_CFG_DELETE,
                                -# UDX_REESTABLISH,
                                -# UDX_CFG_DELETE_UE */
   U8             rbType;     /*!< RB type.  
                                -# CM_LTE_SRB
                                -# CM_LTE_DRB */

   union{
        UdxAddCfgEnt     addCfg; /*!< Configuration parameters for
                                    cfgType UDX_CFG_ADD */
        UdxModCfgEnt     modCfg; /*!< Configuration parameters for
                                    cfgType UDX_CFG_MODIFY */
   }m;
}UdxCfgEnt;



/**
 * @brief
 * PDCP Configuration structure. Parameters are filled by RRC 
   layer while configuring the PDCP layer.  
 */
typedef struct udxCfgReqInfo
{
   CmLteRnti      ueId;                 /*!< UE ID */
   CmLteCellId    cellId;               /*!< Cell ID */
   U32            transId;              /*!< Transaction ID */
   U8             numEnt;               /*!< Number of entities to be
                                          configured in the Config request.*/
   U8             isHo;
   UdxCfgEnt      cfgEnt[UDX_MAX_CFG];  /*!< Config paramater for
                                          PDCP entity. */
}UdxCfgReqInfo;


/**
 * @brief
 * Status report from PDCP-UL to PDCP-DL during Reestablishment.  
 */
typedef struct udxUlStaRepInfo
{
   CmLtePdcpId pdcpId;
   Buffer      *sta;
}UdxUlStaRepInfo;

/**
 * @brief
 * Status report received from peer RLC entity.  
 */
typedef struct udxDlStaRepInfo
{
   CmLtePdcpId pdcpId;
   U16         fms;
   Buffer      *staPdu;
}UdxDlStaRepInfo;


/**
 * @brief
 * ROHC Feedback packet Info from PDCP-UL to PDCP-DL.
 */
typedef struct udxUlFdbkPktInfo
{
    CmLtePdcpId pdcpId;
    Buffer      *fbPkt;
}UdxUlFdbkPktInfo;

/**
 * @brief
 * ROHC Feedback packet Info received from UE. Send to PDCP-DL.
 */
typedef struct udxDlFdbkPktInfo
{
    CmLtePdcpId pdcpId;
    Buffer      *fbPkt;
}UdxDlFdbkPktInfo;


/**
 * @brief
 * PDCP Confirm parameter structure for PDCP entity. 
 */
typedef struct udxCfmEnt
{
   U8             rbId;       /*!< RB ID */
   U8             rbType;     /*!< RB Type */
   Status         status;     /*!< ROK or RFAILED */
   Reason         reason;     /*!< Reason for failure */
}UdxCfmEnt;

/**
 * @brief
 * PDCP Configuration Confirm structure. 
 */
typedef struct udxCfgCfmInfo
{
   CmLteRnti      ueId;                   /*!< UE ID */
   CmLteCellId    cellId;                 /*!< Cell ID */
   U32            transId;                /*!< Transcation ID */
   U8             numEnt;                 /*!< Number of entities */
   UdxCfmEnt      cfmEnt[UDX_MAX_CFM];    /*!< Confirm parameter for
                                               PDCP entity */
}UdxCfgCfmInfo;

/**
 * @brief
 * Re-estabishment Request structure for SRB1 of an UE.
 */
typedef struct udxReEstReqInfo
{
   CmLtePdcpId    pdcpId;                          /*!< PDCP ID */
   U32            transId;                         /*!< Transcation ID */
   U8             intKey[UDX_INTG_KEY_LEN];         /*!< Integrity key */
   UdxCipherKey   ctrlplnKey[UDX_CIPHER_KEY_LEN];  /*!< Control plane
                                                        ciphering key */
   UdxCipherKey   usrplnKey[UDX_CIPHER_KEY_LEN];   /*!< User plane
                                                        ciphering key */
}UdxReEstReqInfo;

/**
 * @brief
 * Re-establishment Confirmation structure for an UE.
 */
typedef struct udxReEstCfmInfo
{
   CmLteRnti      ueId;             /*!< UE ID */
   CmLteCellId    cellId;           /*!< Cell ID */
   U32            transId;          /*!< Transcation ID */
   Status       status;           /*!< Status of reestablishment.
                                     Allowed values are:
                                     -# LCM_PRIM_OK
                                     -# LCM_PRIM_NOK
                                     */
}UdxReEstCfmInfo;

/**
 * @brief
 * PDCP Security Configuration structure. Parameters are filled by RRC
 * while configuring the security parameters.
 */
typedef struct udxSecCfgReqInfo
{
   CmLteRnti      ueId;       /*!< UE ID */
   CmLteCellId    cellId;     /*!< Cell ID */
   U32            transId;    /*!< Transcation ID */
   UdxSecCfg      secCfg;     /*!< Configuration paramater for PDCP entity */
}UdxSecCfgReqInfo;

/**
 * @brief
 * PDCP Security Configuration confirm structure. 
 */
typedef struct udxSecCfgCfmInfo
{
   CmLteRnti      ueId;       /*!< UE ID */
   CmLteCellId    cellId;     /*!< Cell ID */
   U32            transId;    /*!< Transcation ID */
   Status         status;     /*!< ROK or RFAILED */
   Reason         reason;     /*!< Reason for failure */
}UdxSecCfgCfmInfo;

/**
 *  @brief
 *  UE ID Information
 */
typedef struct udxUeInfo
{
   CmLteCellId    cellId;     /*!< Cell ID */
   CmLteRnti      ueId;       /*!< UE ID */
}UdxUeInfo;

/**
 *  @brief
 *  PDCP Count Request Information from RRC.
 */
typedef struct udxCountReqInfo
{
   CmLteRnti      ueId;       /*!< UE ID */
   CmLteCellId    cellId;     /*!< Cell ID */
   U32            transId;    /*!< Transaction ID */
} UdxCountReqInfo;

/**
 *  @brief
 *  PDCP Count Information for a DRB.
 */
typedef struct udxCountInfo
{
   U8             rbId;       /*!< RB ID */
   U8             dir;        /*!< Direction of the RB */
   U32            ulCount;    /*!< UL Count value */
   U32            dlCount;    /*!< DL Count value */
} UdxCountInfo;
/**
 *  @brief
 *  PDCP Count Request Confirmation to RRC.
 */
typedef struct udxCountCfmInfo
{
   CmLteRnti      ueId;                   /*!< UE ID */
   CmLteCellId    cellId;                 /*!< Cell ID */
   U32            transId;                /*!< Transaction ID */
   U8             numRb;                  /*!< Number of DRBs */
   UdxCountInfo   countInfo[UDX_MAX_DRB]; /*!< CountInfo of DRBs */
   Status         status;                 /*!< ROK or RFAILED */
   Reason         reason;                 /*!< Reason for failure */
} UdxCountCfmInfo;

/**
 *  @brief
 *  PDCP SDU Status Request Information from RRC - Used for reestablishment
 *  during handover.
 */
typedef struct udxSduStaReqInfo
{
   CmLteRnti      ueId;       /*!< UE ID */
   CmLteCellId    cellId;     /*!< Cell ID */
   U32            transId;    /*!< Transaction ID */
} UdxSduStaReqInfo;

/**
 *  @brief
 *  PDCP DL SDU Status Information for a DRB mapped to RLC AM.
 */
typedef struct udxDlSduStaInfo
{  
   Bool           hoPres;     
   U32            count;      /*!< Count with the next SN to be assigned
                                for a SDU */
} UdxDlSduStaInfo;

/**
 *  @brief
 *  PDCP SDU Status Information for a DRB mapped to RLC AM.
 */
typedef struct udxSduStaInfo
{
   U8                   rbId;         /*!< RB ID */
   U8                   dir;          /*!< Direction of the RB */
   UdxDlSduStaInfo      dlSduStaInfo; /*!< SDU Status Info for DL */ 
} UdxSduStaInfo;

/**
 *  @brief
 *  PDCP SDU Status Request Confirmation for an UE.
 */
typedef struct udxSduStaCfmInfo
{
   CmLteRnti      ueId;                   /*!< UE ID */
   CmLteCellId    cellId;                 /*!< Cell ID */
   U32            transId;                /*!< Transaction ID */
   U8             numRb;                  /*!< Number of RBs */
   UdxSduStaInfo  sduStaInfo[UDX_MAX_DRB]; /*!< SDU Status Info of DRBs mapped
                                             to RLC AM */
   Status         status;                 /*!< ROK or RFAILED */
   Reason         reason;                 /*!< Reason for Failure */
} UdxSduStaCfmInfo;

/**
 *  @brief
 *  Data Resume
 */
typedef struct udxDatResumeInfo
{
   CmLteRnti      ueId;                   /*!< UE ID */
   CmLteCellId    cellId;                 /*!< Cell ID */
   U32            transId;                         /*!< Transcation ID */
} UdxDatResumeInfo;

/**
 *  @brief
 *  Data Fwd Cfm
 */
typedef struct udxDatFwdCfmInfo
{
   CmLteRnti      ueId;                   /*!< UE ID */
   CmLteCellId    cellId;                 /*!< Cell ID */
   Status         status;                 /*!< ROK or RFAILED */
   Reason         reason;                 /*!< Reason for Failure */
}UdxDatFwdCfmInfo;


typedef struct udxDatResumeCfmInfo
{
   CmLteRnti      ueId;                   /*!< UE ID */
   CmLteCellId    cellId;                 /*!< Cell ID */
   U32            transId;                         /*!< Transcation ID */
   Status         status;                 /*!< ROK or RFAILED */
   Reason         reason;                 /*!< Reason for Failure */
} UdxDatResumeCfmInfo;
/**
 *  @brief
 *  Data Forward
 */
typedef struct udxDatForwardInfo
{
   CmLteRnti      ueId;                   /*!< UE ID */
   CmLteCellId    cellId;                 /*!< Cell ID */
   U32            transId;                         /*!< Transcation ID */
   Bool           ulFwding;               /*!< UL fwding to be done or not */
} UdxDatForwardInfo;

/*************************************************************************
 *                      END OF STRUCTURES
 ************************************************************************/

/*************************************************************************
 *                   Start of Pointer Interface primitives
 ************************************************************************/
typedef S16 (*UdxBndReq)      ARGS((Pst *pst, SuId suId, SpId spId));
typedef S16 (*UdxBndCfm)      ARGS((Pst *pst, SuId suId, U8 status));
typedef S16 (*UdxUbndReq)     ARGS((Pst *pst, SpId spId, Reason reason));
typedef S16 (*UdxCfgReq)      ARGS((Pst *pst, SpId spId, UdxCfgReqInfo *cfgReq));
typedef S16 (*UdxCfgCfm)      ARGS((Pst *pst, SuId suId, UdxCfgCfmInfo *cfgCfm));
typedef S16 (*UdxReEstReq)    ARGS((Pst *pst, SpId spId, UdxReEstReqInfo *reEstReq));
typedef S16 (*UdxReEstCfm)    ARGS((Pst *pst, SuId suId, UdxReEstCfmInfo *reEstCfm));
typedef S16 (*UdxSecCfgReq)   ARGS((Pst *pst, SpId spId, UdxSecCfgReqInfo *secCfgReq));
typedef S16 (*UdxSecCfgCfm)   ARGS((Pst *pst, SuId suId, UdxSecCfgCfmInfo *secCfgCfm));
typedef S16 (*UdxUeIdChgReq)  ARGS((Pst *pst, SpId spId, U32 transId, 
                                    UdxUeInfo *oldUeInfo,
                                    UdxUeInfo *newUeInfo));
typedef S16 (*UdxUeIdChgCfm)  ARGS((Pst *pst, SuId suId, U32 transId, 
                                    UdxUeInfo  *ueInfo, CmStatus status));
typedef S16 (*UdxCountReq)   ARGS((Pst *pst, SpId spId, UdxCountReqInfo *countReq));
typedef S16 (*UdxCountCfm)   ARGS((Pst *pst, SuId suId, UdxCountCfmInfo *countCfm));
typedef S16 (*UdxSduStaReq)  ARGS((Pst *pst, SpId spId, UdxSduStaReqInfo *staReq));
typedef S16 (*UdxSduStaCfm)  ARGS((Pst *pst, SuId suId, UdxSduStaCfmInfo *staCfm));
typedef S16 (*UdxDatResumeCfm)  ARGS((Pst *pst, SuId suId, UdxDatResumeCfmInfo
         *datResumeCfm));
typedef S16 (*UdxDatForwardCfm)  ARGS((Pst *pst, SuId suId, UdxDatFwdCfmInfo *datFwdCfm));
typedef S16 (*UdxUlStaRep)     ARGS((Pst *pst, SpId spId, UdxUlStaRepInfo *staRep));
typedef S16 (*UdxDlStaRep)     ARGS((Pst *pst, SpId spId, UdxDlStaRepInfo *staRep));
typedef S16 (*UdxUlFdbkPkt)     ARGS((Pst *pst, SpId spId, UdxUlFdbkPktInfo *fdbkPktInfo));
typedef S16 (*UdxDlFdbkPkt)     ARGS((Pst *pst, SpId spId, UdxDlFdbkPktInfo *fdbkPktInfo));

#ifdef LTE_L2_MEAS
typedef S16 (*UdxL2MeasReq) ARGS((Pst* pst,
                                PjL2MeasReqEvt *measReqEvt));

typedef S16 (*UdxL2MeasSendReq) ARGS((Pst* pst,
                                    U8 measType));

typedef S16 (*UdxL2MeasStopReq) ARGS((Pst* pst,
                                    U8 measType));
#endif

typedef S16 (*UdxDatResumeReq) ARGS((Pst  *pst,
                                    UdxDatResumeInfo  *udxDatResumeInfo));
typedef S16 (*UdxDatForwardReq) ARGS((Pst  *pst,
                                    UdxDatForwardInfo  *udxDatForwardInfo));

/*************************************************************************
 *                   End of Pointer Interface primitives
 ************************************************************************/
/* cpj_x_001.main_2 */
/*************************************************************************
 *                   Start of Extern Interface Declarations
 ************************************************************************/
/**
 * @brief Bind request primitive
 * @details
 * The function binds the PDCP service user entity (RRC) to the PDCP product.
 * PDCP acknowledges the receipt of the bind request by sending a PjDlUdxBndCfm
 * primitive.
 *
 * @param[in] pst     - Pointer to the post structure.
 * @param[in] suId    - Service user ID. This is the reference number used by
 * PDCP service user to identify its SAP. This reference number is 
 * provided by PDCP in any further primitives sent by PDCP to the service user.
 * @param[in] spId    - Service provider ID. This is the reference number used
 * by PDCP service user to identify its SAP. This reference number is
 * provided by PDCP in any further primitives sent by PDCP to the service user.
 *
 * @return ROK
 */
EXTERN S16 PjUlUdxBndReq ARGS((Pst *pst, SuId suId, SpId spId));
EXTERN S16 PjDlUdxBndReq ARGS((Pst *pst, SuId suId, SpId spId));

/**
 * @brief Bind confirmation primitive
 * @details
 * PDCP invokes the bind confirm primitive to acknowledge the receipt of
 * the PjDlUdxBndReq primitive from the RRC layer. The status field indicates
 * whether the bind request succeeded or failed. The bind request may fail due
 * to invalid parameters in the bind request from the service user
 * (for example, an invalid spId).
 *
 * @param[in] pst    - Pointer to the pst structure.
 * @param[in] suId   - Service user ID. This is the reference number used
 * by PDCP service user (RRC layer) to identify its SAP.
 * @param[in] status - Status field indicates whether the bind request
 * succeeded or failed. 
 *  -# TRUE, for successful bind.
 *  -# FALSE, otherwise.
 *
 *  @return ROK.
 */
EXTERN S16 PjDlUdxBndCfm ARGS((Pst *pst, SuId suId, U8 status));
EXTERN S16 PjUlUdxBndCfm ARGS((Pst *pst, SuId suId, U8 status));

/**
 * @brief Unbind request primitive
 * @details
 * The PDCP service user (RRC) initiates this primitive for performing
 * an unbind operation. This function brings the link between RRC and PDCP 
 * down if it is already up, and releases all the resources associated for
 * this SAP at PDCP.
 *
 * @param[in] pst    - Pointer to the post structure.
 * @param[in] spId   - Service provider ID. This is the reference number used
 * by PDCP to identify its SAP.
 * @param[in] reason - Unbind reason. Not used in the current implementation. 
 *
 * @return ROK
 */
EXTERN S16 PjUlUdxUbndReq ARGS((Pst *pst, SpId spId, Reason reason));
EXTERN S16 PjDlUdxUbndReq ARGS((Pst *pst, SpId spId, Reason reason));

/**
 * @brief Configuration request primitive
 * @details
 * RRC uses this primitive to configure PDCP system parameters. The
 * various parameters are configured by RRC. In one configuration request,
 * multiple PDCP entities can be configured based on the rbId.
 * For PDCP on target eNB, RRC can configure the handover information.
 *
 * @param[in] pst    - Pointer to post structure.
 * @param[in] spId   - Service provider ID. This is the reference number used
 * by PDCP to identify its SAP.
 * @param[in] pdcpCfg - PDCP configuration structure. 
 *
 * @return ROK
 */
EXTERN S16 PjUlUdxCfgReq ARGS((Pst *pst, SpId spId, UdxCfgReqInfo *cfgReq));
EXTERN S16 PjDlUdxCfgReq ARGS((Pst *pst, SpId spId, UdxCfgReqInfo *cfgReq));
EXTERN S16 PjDlUdxUlStaRep ARGS ((Pst *pst, SpId spId, UdxUlStaRepInfo   *staRep));
EXTERN S16 PjDlUdxDlStaRep ARGS ((Pst *pst, SpId spId, UdxDlStaRepInfo   *staRep));
EXTERN S16 PjUlUdxUlFdbkPktInfo ARGS((Pst *pst, SpId spId, UdxUlFdbkPktInfo *fdbkPktInfo));
EXTERN S16 PjUlUdxDlFdbkPktInfo ARGS((Pst *pst, SpId spId, UdxDlFdbkPktInfo *fdbkPktInfo));


/**
 * @brief Configuration confirmation primitive
 * @details
 * This primitive is invoked by PDCP to inform to PDCP User (RRC) about
 * the PDCP configuration request primitive execution status.
 *
 * @param[in] pst    - Pointer to the post structure.
 * @param[in] suId   - Service user ID. This is the reference number used
 * by PDCP to identify its SAP.
 * @param[in] pdcpCfgCfm - PDCP configuration confirm structure. 
 *
 * @return ROK
 */
EXTERN S16 PjUlUdxCfgCfm ARGS((Pst *pst, SuId suId, UdxCfgCfmInfo *cfgCfm));
EXTERN S16 PjDlUdxCfgCfm ARGS((Pst *pst, SuId suId, UdxCfgCfmInfo *cfgCfm));

/**
 * @brief Reestablishment request primtive
 * @details
 * RRC uses this primitive to reestablish the PDCP entity.
 * This primitive is initiation of reestablishment procedure.
 * This prmitive and corresponding confirmation are followed by
 * PjDlUdxCfgReq procedure for entire reestablishment procedure to complete.
 *
 * @param[in] pst    - Pointer to post structure.
 * @param[in] spId   - Service provider ID. This is the reference number used by
 * PDCP to identify its SAP.
 * @param[in] reEstReq - PDCP reestablishment request structure. 
 *
 */
EXTERN S16 PjUlUdxReEstReq ARGS((Pst *pst, SpId spId, UdxReEstReqInfo *reEstReq));
EXTERN S16 PjDlUdxReEstReq ARGS((Pst *pst, SpId spId, UdxReEstReqInfo *reEstReq));

/**
 * @brief Reestablishment confirmation primitive
 * @details
 * This primitive is invoked by PDCP to inform PDCP User (RRC) about the PDCP
 * (SRB1) reestablishment request primitive execution status.
 *
 * @param[in] pst    - Pointer to the post structure.
 * @param[in] suId   - Service user ID. This is the reference number used
 * by PDCP to identify its SAP.
 * @param[in] reEstCfm - PDCP reestablishment confirm structure. 
 *
 */
EXTERN S16 PjUlUdxReEstCfm ARGS((Pst *pst, SuId suId, UdxReEstCfmInfo *reEstCfm));
EXTERN S16 PjDlUdxReEstCfm ARGS((Pst *pst, SuId suId, UdxReEstCfmInfo *reEstCfm));

/**
 * @brief Security Configuration request primitive
 * @details
 * RRC uses this primitive to configure PDCP security parameters.
 * Integrity protection and/or Ciphering are configured by RRC based on the
 * selSecAct flag. Integrity Protection/Ciphering are configured per UE and applicable
 * to all PDCP entities in that UE. Both Integrity protection and ciphering are
 * configured during reestablishment. 
 *
 * @param[in] pst    - Pointer to post structure.
 * @param[in] spId   - Service provider ID. This is the reference number used by
 * PDCP to identify its SAP.
 * @param[in] pdcpSecCfg - PDCP security configuration structure. 
 *
 * @return ROK
 */
EXTERN S16 PjUlUdxSecCfgReq ARGS((Pst *pst, SpId spId,
         UdxSecCfgReqInfo *secCfgReq));
EXTERN S16 PjDlUdxSecCfgReq ARGS((Pst *pst, SpId spId,
         UdxSecCfgReqInfo *secCfgReq));

/**
 * @brief Security Configuration confirmation primitive.
 * @details
 * This primitive is invoked by PDCP to inform to PDCP User (RRC) about
 * the PDCP secutiry configuration request primitive execution status.
 *
 * @param[in] pst    - Pointer to the post structure.
 * @param[in] suId   - Service user ID. This is the reference number used
 * by PDCP to identify its SAP.
 * @param[in] pdcpSecCfgCfm - PDCP configuration confirm structure. 
 *
 * @return ROK
 */
EXTERN S16 PjUlUdxSecCfgCfm ARGS( (Pst *pst, SuId suId,
         UdxSecCfgCfmInfo *secCfgCfm));
EXTERN S16 PjDlUdxSecCfgCfm ARGS( (Pst *pst, SuId suId,
         UdxSecCfgCfmInfo *secCfgCfm));

/**
 * @brief UE ID change request primitive
 * @details
 * This primitive is used by RRC to change the UeId for the existing UE context.
 *
 * @param[in] pst       - Point to the post structure
 * @param[in] spId      - The ID of the service provider SAP in the PDCP layer.
 * @param[in] transId   - Transaction ID. This field uniquely identifies
 * transaction between RRC and PDCP.
 * @param[in] oldUeInfo - Old UE ID info for which the change request has come.
 * @param[in] newUeInfo - New UE ID info for existing UE context. 
 *
 * @return ROK
 */
EXTERN S16 PjUlUdxUeIdChgReq ARGS((Pst *pst, SpId spId, U32 transId,
                                  UdxUeInfo *oldUeInfo, UdxUeInfo  *newUeInfo));
EXTERN S16 PjDlUdxUeIdChgReq ARGS((Pst *pst, SpId spId, U32 transId,
                                  UdxUeInfo *oldUeInfo, UdxUeInfo  *newUeInfo));

/**
 * @brief UE ID change confirmation primitive
 * @details
 * This primitive is used by PDCP to confirm the UeId change requested by RRC.
 * The different values for status are:\n
 * ROK       -  Success\n
 * RFAILED   -  Failure\n
 *
 * The different reasons for the failure are:
 * UDX_CFG_REAS_UE_CREAT_FAIL -  UE Cb update failure \n
 * UDX_CFG_REAS_UE_UNKWN      -  Request received for an unknown UE ID. 
 *
 * @param [in] pst       - Point to the post structure.
 * @param [in] suId      - The ID of the service user SAP associated with PDCP
 * @param [in] transId   - Transaction ID. This field uniquely identifies
 *                         transaction between RRC and PDCP.
 * @param [in] ueInfo    - UE Identity which is changed with the new ID
 *                         for the same context.
 * @param [in] CmStatus  - Confirmation status with appropriate reason
 *                         information.
 * @return ROK
 */
EXTERN S16 PjUlUdxUeIdChgCfm  ARGS((Pst *pst, SuId suId, U32 transId,
                                  UdxUeInfo *ueInfo, CmStatus status));
EXTERN S16 PjDlUdxUeIdChgCfm  ARGS((Pst *pst, SuId suId, U32 transId,
                                  UdxUeInfo *ueInfo, CmStatus status));

/**
 * @brief Count request primitive
 * @details
 * RRC uses this primitive to request count value for all DRBs in a UE.
 * PDCP sends the UL and DL count values in the confirmation of this request.
 *
 * @param[in] pst    - Pointer to post structure.
 * @param[in] spId   - Service provider ID. This is the reference number used
 * by PDCP to identify its SAP.
 * @param[in] countReq - PDCP Count Request Information. 
 *
 * @return ROK
 */
EXTERN S16 PjUlUdxCountReq ARGS((Pst *pst, SpId spId, UdxCountReqInfo *countReq));
EXTERN S16 PjDlUdxCountReq ARGS((Pst *pst, SpId spId, UdxCountReqInfo *countReq));

/**
 * @brief Count confirmation primitive
 * @details
 * PDCP uses this primitive to send the count values for all
 * DRBs in a UE to RRC. PDCP sends the RB ID, direction 
 * and count value(s) ( UL, DL or both based on direction of the RB)
 * for each DRB. 
 *
 * @param[in] pst       - Pointer to post structure.
 * @param[in] suId      - The ID of the service user SAP associated with PDCP.
 * @param[in] countCfm  - PDCP Count Confirmation Info. 
 *
 * @return ROK
*/
EXTERN S16 PjUlUdxCountCfm ARGS((Pst *pst, SuId suId, UdxCountCfmInfo *countCfm));
EXTERN S16 PjDlUdxCountCfm ARGS((Pst *pst, SuId suId, UdxCountCfmInfo *countCfm));

/**
 * @brief SDU Status Request primitive
 * @details
 * RRC uses this primitive to request the status of the SDUs for DRBs in an UE.
 * Its used as a reestablishment request during handover.
 * This primitive is used to forward the handover control Information to the target
 * eNodeB.
 *
 * @param[in] pst    - Pointer to post structure.
 * @param[in] spId   - Service provider ID. This is the reference number used
 * by PDCP to identify its SAP.
 * @param[in] sduStaReq - PDCP SDU Status Request Information. 
 *
 * @return ROK
 */
EXTERN S16 PjUlUdxSduStaReq ARGS((Pst *pst, SpId spId, UdxSduStaReqInfo *staReq));
EXTERN S16 PjDlUdxSduStaReq ARGS((Pst *pst, SpId spId, UdxSduStaReqInfo *staReq));

/**
 * @brief SDU Status confirmation primitive
 * @details
 * PDCP uses this primitive to send the SDU status for the DRBs in 
 * the requested UE. PDCP sends the RB ID, direction of the RB,status report 
 * and count(to indicate the first missing SN) in UL and count(to indicate 
 * the next SN to be assigned) in DL for each requested DRB mapped on to RLC AM.
 *
 * @param[in] pst       - Pointer to post structure.
 * @param[in] suId      - The ID of the service user SAP associated with PDCP.
 * @param[in] sduStaCfm - PDCP SDU Status Confirmation Info. 
 *
 * @return ROK
*/
EXTERN S16 PjDlUdxSduStaCfm ARGS((Pst *pst, SuId suId, UdxSduStaCfmInfo *staCfm));
EXTERN S16 PjUlUdxSduStaCfm ARGS((Pst *pst, SuId suId, UdxSduStaCfmInfo *staCfm));


EXTERN S16 PjDlUdxDatResumeCfm ARGS((Pst *pst, SuId suId, UdxDatResumeCfmInfo
         *datResumeCfm));
EXTERN S16 PjUlUdxDatResumeCfm ARGS((Pst *pst, SuId suId, UdxDatResumeCfmInfo
         *datResumeCfm));
EXTERN S16 PjDlUdxDatFwdCfm ARGS ((Pst *pst, SuId suId, UdxDatFwdCfmInfo  
         *datFwdCfm));
EXTERN S16 PjUlUdxDatForwardCfm ARGS ((Pst *pst, SuId suId, UdxDatFwdCfmInfo  
         *datFwdCfm));

EXTERN S16 PjDlUdxUlFdbkPktInfo ARGS((Pst *pst, SpId spId, UdxUlFdbkPktInfo *fdbkPktInfo));
EXTERN S16 PjDlUdxDlFdbkPktInfo ARGS((Pst * pst, SpId spId, UdxDlFdbkPktInfo * fdbkPktInfo));

#ifdef LTE_L2_MEAS
EXTERN S16 PjDlUdxL2MeasReq ARGS((Pst *pst,
                                PjL2MeasReqEvt *measReqEvt)); 

EXTERN S16 PjDlUdxL2MeasSendReq ARGS((Pst *pst,
                                    U8 status)); 

EXTERN S16 PjDlUdxL2MeasStopReq ARGS((Pst *pst,
                                    U8 status)); 

EXTERN S16 PjUlUdxL2MeasReq ARGS((Pst* pst, 
                                 PjL2MeasReqEvt *measReqEvt));

EXTERN S16 PjUlUdxL2MeasSendReq ARGS((Pst *pst, U8 status));

EXTERN S16 PjUlUdxL2MeasStopReq ARGS((Pst* pst, U8 measType));
#endif
EXTERN S16 PjDlUdxDatResumeReq ARGS((Pst *pst, UdxDatResumeInfo  *udxDatResumeInfo));
EXTERN S16 PjUlUdxDatResumeReq ARGS((Pst *pst, UdxDatResumeInfo  *udxDatResumeInfo));
EXTERN S16 PjDlUdxDatForwardReq ARGS((Pst *pst, UdxDatForwardInfo  *udxDatForwardInfo));
EXTERN S16 PjUlUdxDatForwardReq ARGS((Pst *pst, UdxDatForwardInfo  *udxDatForwardInfo));
EXTERN S16 PjUlUdxDatResumeReqFB ARGS((Pst *pst, UdxDatResumeInfo  *udxDatResumeInfo));
EXTERN S16 PjUlUdxDatForwardReqFB ARGS((Pst *pst, UdxDatForwardInfo  *udxDatForwardInfo));
#ifdef FLAT_BUFFER_OPT
EXTERN S16 PjDlUdxDatResumeReqFB ARGS((Pst *pst,UdxDatResumeInfo *datResumeInfo));
EXTERN S16 PjDlUdxDatForwardReqFB ARGS((Pst *pst,UdxDatForwardInfo *datFwdInfo));
#endif
/*************************************************************************
 *                   End of Extern Interface Declarations                *
 ************************************************************************/

/***********************************************************************
 *                    Start of pack/unpack functions                   *
 * *********************************************************************/
#ifdef LCUDX

PUBLIC S16 cmPkUdxBndReq ARGS((
Pst * pst,
SuId suId,
SpId spId
));
PUBLIC S16 cmUnpkUdxBndReq ARGS((
UdxBndReq func,
Pst *pst,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxBndCfm ARGS((
Pst * pst,
SuId suId,
U8 status
));
PUBLIC S16 cmUnpkUdxBndCfm ARGS((
UdxBndCfm func,
Pst *pst,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxUbndReq ARGS((
Pst * pst,
SpId spId,
Reason reason
));
PUBLIC S16 cmUnpkUdxUbndReq ARGS((
UdxUbndReq func,
Pst *pst,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxCfgReq ARGS((
Pst * pst,
SpId spId,
UdxCfgReqInfo * cfgReq
));
PUBLIC S16 cmUnpkUdxCfgReq ARGS((
UdxCfgReq func,
Pst *pst,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxCfgCfm ARGS((
Pst * pst,
SuId suId,
UdxCfgCfmInfo * cfgCfm
));
PUBLIC S16 cmUnpkUdxCfgCfm ARGS((
UdxCfgCfm func,
Pst *pst,
Buffer *mBuf
));

PUBLIC S16 cmPkUdxUlStaRep ARGS ((
Pst * pst,
SpId spId,
UdxUlStaRepInfo *staRep
));

PUBLIC S16 cmPkUdxDlStaRep ARGS ((
Pst * pst,
SpId spId,
UdxDlStaRepInfo *staRep
));

PUBLIC S16 cmUnpkUdxUlStaRepInfo ARGS ((
Pst *pst,
UdxUlStaRepInfo *param,
Buffer *mBuf
));
 
PUBLIC S16 cmPkUdxDlFdbkPktInfo ARGS ((
UdxDlFdbkPktInfo  *param,
Pst            *pst,
Buffer         *mBuf
));
 
PUBLIC S16 cmUnpkUdxUlStaRep ARGS ((
UdxUlStaRep func,
Pst *pst,
Buffer *mBuf
));

PUBLIC S16 cmUnpkUdxDlStaRep ARGS ((
UdxDlStaRep func,
Pst *pst,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxUlFdbkPkt ARGS ((
UdxUlFdbkPkt func,
Pst *pst,
Buffer *mBuf
));


PUBLIC S16 cmPkUdxUlStaRepInfo ARGS ((
UdxUlStaRepInfo  *param,
Pst            *pst,
Buffer         *mBuf
));


PUBLIC S16 cmPkUdxDlStaRepInfo ARGS ((
UdxDlStaRepInfo  *param,
Pst            *pst,
Buffer         *mBuf
));

PUBLIC S16 cmUnpkUdxDlStaRepInfo ARGS ((
Pst *pst,
UdxDlStaRepInfo *param,
Buffer *mBuf
));

PUBLIC S16 cmUnpkUdxDlFdbPktInfo ARGS ((
Pst *pst,
UdxDlFdbkPktInfo *param,
Buffer *mBuf
));


PUBLIC S16 cmPkUdxUlFdbkPkt ARGS((
Pst * pst,
SpId spId,
UdxUlFdbkPktInfo  *fdbkPktInfo
));

PUBLIC S16 cmPkUdxDlFdbkPkt ARGS((
Pst * pst,
SpId spId,
UdxDlFdbkPktInfo  *fdbkPktInfo
));

PUBLIC S16 cmPkUdxUlFdbkPktInfo ARGS((
UdxUlFdbkPktInfo  *fdbkPktInfo,
Pst *pst,
Buffer *mBuf
));

PUBLIC S16 cmUnpkUdxDlFdbkPkt ARGS((
UdxDlFdbkPkt func,
Pst *pst,
Buffer *mBuf
));

PUBLIC S16 cmUnpkUdxDlFdbkPktInfo ARGS((
Pst *pst,
UdxDlFdbkPktInfo  *fdbkPktInfo,
Buffer *mBuf
));

PUBLIC S16 cmUnpkUdxUlFdbkPktInfo ARGS ((
Pst *pst,
UdxUlFdbkPktInfo *param,
Buffer *mBuf
));

PUBLIC S16 cmPkUdxReEstReq ARGS((
Pst * pst,
SpId spId,
UdxReEstReqInfo * reEstReq
));
PUBLIC S16 cmUnpkUdxReEstReq ARGS((
UdxReEstReq func,
Pst *pst,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxReEstCfm ARGS((
Pst * pst,
SuId suId,
UdxReEstCfmInfo * reEstCfm
));
PUBLIC S16 cmUnpkUdxReEstCfm ARGS((
UdxReEstCfm func,
Pst *pst,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxSecCfgReq ARGS((
Pst * pst,
SpId spId,
UdxSecCfgReqInfo * secCfgReq
));
PUBLIC S16 cmUnpkUdxSecCfgReq ARGS((
UdxSecCfgReq func,
Pst *pst,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxSecCfgCfm ARGS((
Pst * pst,
SuId suId,
UdxSecCfgCfmInfo * secCfgCfm
));
PUBLIC S16 cmUnpkUdxSecCfgCfm ARGS((
UdxSecCfgCfm func,
Pst *pst,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxUeIdChgReq ARGS((
Pst * pst,
SpId spId,
U32 transId,
UdxUeInfo * oldUeInfo,
UdxUeInfo * newUeInfo
));
PUBLIC S16 cmUnpkUdxUeIdChgReq ARGS((
UdxUeIdChgReq func,
Pst *pst,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxUeIdChgCfm ARGS((
Pst * pst,
SuId suId,
U32 transId,
UdxUeInfo  * ueInfo,
CmStatus status
));
PUBLIC S16 cmUnpkUdxUeIdChgCfm ARGS((
UdxUeIdChgCfm func,
Pst *pst,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxCountReq ARGS((
Pst * pst,
SpId spId,
UdxCountReqInfo * countReq
));
PUBLIC S16 cmUnpkUdxCountReq ARGS((
UdxCountReq func,
Pst *pst,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxCountCfm ARGS((
Pst * pst,
SuId suId,
UdxCountCfmInfo * countCfm
));
PUBLIC S16 cmUnpkUdxCountCfm ARGS((
UdxCountCfm func,
Pst *pst,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxSduStaReq ARGS((
Pst * pst,
SpId spId,
UdxSduStaReqInfo * staReq
));
PUBLIC S16 cmUnpkUdxSduStaReq ARGS((
UdxSduStaReq func,
Pst *pst,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxSduStaCfm ARGS((
Pst * pst,
SuId suId,
UdxSduStaCfmInfo * staCfm
));
PUBLIC S16 cmUnpkUdxSduStaCfm ARGS((
UdxSduStaCfm func,
Pst *pst,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxDatResumeCfm ARGS((
Pst * pst,
SuId suId,
UdxDatResumeCfmInfo * staCfm
));
PUBLIC S16 cmPkUdxDatForwardCfm ARGS((
Pst * pst,
SuId suId,
UdxDatFwdCfmInfo * datFwdCfm
));
PUBLIC S16 cmUnpkUdxDatResumeCfm ARGS((
UdxDatResumeCfm func,
Pst *pst,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxDatForwardCfm ARGS((
 UdxDatForwardCfm func,
 Pst *pst,
 Buffer *mBuf
 ));
PUBLIC S16 cmPkUdxRlcAm ARGS((
UdxRlcAm *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxRlcAm ARGS((
UdxRlcAm *param,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxRlcUm ARGS((
UdxRlcUm *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxRlcUm ARGS((
UdxRlcUm *param,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxRlcInfo ARGS((
UdxRlcInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxRlcInfo ARGS((
UdxRlcInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxCipherInfo ARGS((
UdxCipherInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxCipherInfo ARGS((
UdxCipherInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxIntegInfo ARGS((
UdxIntegInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxIntegInfo ARGS((
UdxIntegInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxSecCfg ARGS((
UdxSecCfg *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxSecCfg ARGS((
UdxSecCfg *param,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxRohcInfo ARGS((
UdxRohcInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxRohcInfo ARGS((
UdxRohcInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxHdrCompCfg ARGS((
UdxHdrCompCfg *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxHdrCompCfg ARGS((
UdxHdrCompCfg *param,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxHoInfo ARGS((
Pst *pst,
UdxHoInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxHoInfo ARGS((
Pst *pst,
UdxHoInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxCfgEnt ARGS((
Pst *pst,
UdxCfgEnt *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxCfgEnt ARGS((
Pst *pst,
UdxCfgEnt *param,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxAddCfgEnt ARGS((
UdxAddCfgEnt *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxAddCfgEnt ARGS((
UdxAddCfgEnt *param,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxModCfgEnt ARGS((
Pst *pst,
UdxModCfgEnt *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxModCfgEnt ARGS((
Pst *pst,
UdxModCfgEnt *param,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxCfgReqInfo ARGS((
Pst *pst,
UdxCfgReqInfo *param,
Buffer *mBuf
));

PUBLIC S16 cmUnpkUdxCfgReqInfo ARGS((
Pst *pst,
UdxCfgReqInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxCfmEnt ARGS((
UdxCfmEnt *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxCfmEnt ARGS((
UdxCfmEnt *param,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxCfgCfmInfo ARGS((
UdxCfgCfmInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxCfgCfmInfo ARGS((
UdxCfgCfmInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxReEstReqInfo ARGS((
UdxReEstReqInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxReEstReqInfo ARGS((
UdxReEstReqInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxReEstCfmInfo ARGS((
UdxReEstCfmInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxReEstCfmInfo ARGS((
UdxReEstCfmInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxSecCfgReqInfo ARGS((
UdxSecCfgReqInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxSecCfgReqInfo ARGS((
UdxSecCfgReqInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxSecCfgCfmInfo ARGS((
UdxSecCfgCfmInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxSecCfgCfmInfo ARGS((
UdxSecCfgCfmInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxUeInfo ARGS((
UdxUeInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxUeInfo ARGS((
UdxUeInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxCountReqInfo ARGS((
UdxCountReqInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxCountReqInfo ARGS((
UdxCountReqInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxCountInfo ARGS((
UdxCountInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxCountInfo ARGS((
UdxCountInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxCountCfmInfo ARGS((
UdxCountCfmInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxCountCfmInfo ARGS((
UdxCountCfmInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxSduStaReqInfo ARGS((
UdxSduStaReqInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxSduStaReqInfo ARGS((
UdxSduStaReqInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxDlSduStaInfo ARGS((
UdxDlSduStaInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxDlSduStaInfo ARGS((
UdxDlSduStaInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxSduStaInfo ARGS((
Pst *pst,
UdxSduStaInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxSduStaInfo ARGS((
Pst *pst,
UdxSduStaInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxSduStaCfmInfo ARGS((
Pst *pst,
UdxSduStaCfmInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxSduStaCfmInfo ARGS((
Pst *pst,
UdxSduStaCfmInfo *param,
Buffer *mBuf
));

PUBLIC S16 cmPkUdxDatResumeCfmInfo ARGS((
Pst *pst,
UdxDatResumeCfmInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmPkUdxDatForwardCfmInfo ARGS((
Pst *pst,
UdxDatFwdCfmInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxDatResumeCfmInfo ARGS((
Pst *pst,
UdxDatResumeCfmInfo *param,
Buffer *mBuf
));
PUBLIC S16 cmUnpkUdxDatForwardCfmInfo ARGS((
Pst *pst,
UdxDatFwdCfmInfo *param,
Buffer *mBuf
));

#ifdef LTE_L2_MEAS
EXTERN S16 cmPkUdxL2MeasReq ARGS((Pst *pst, PjL2MeasReqEvt *measReqEvt)); 
EXTERN S16 cmPkUdxL2MeasSendReq ARGS((Pst *pst, U8 measType)); 
EXTERN S16 cmPkUdxL2MeasStopReq ARGS((Pst *pst, U8 measType)); 
EXTERN S16 cmUnpkUdxL2MeasReq ARGS((UdxL2MeasReq func, Pst* pst, Buffer *mBuf));
EXTERN S16 cmUnpkUdxL2MeasSendReq ARGS((UdxL2MeasSendReq func, Pst* pst, Buffer *mBuf));
EXTERN S16 cmUnpkUdxL2MeasStopReq ARGS((UdxL2MeasStopReq func, Pst* pst, Buffer *mBuf));
#endif
#endif

/***********************************************************************
 *                    End of pack/unpack functions                     *
 * *********************************************************************/

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __UDX_X__ */

/********************************************************************30**
  
         End of file:     pj_udx.x@@/main/TeNB_Main_BR/tenb_main_ccb/tenb_5.0_RIB/tenb_5.0_RIB_GA/10 - Mon Feb 15 12:52:46 2016
  
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
/main/1      ---        dm            1. LTE RRC Initial Release.
/main/2      ---        gk            1. LTERLC Release 2.1.
/main/3      ---      cpj_x_001.main_2  nm            1. Removed PJ flag.
/main/4 cpj_x_001.main_3 ap           1. Added support for L2 Measurement 
*********************************************************************91*/


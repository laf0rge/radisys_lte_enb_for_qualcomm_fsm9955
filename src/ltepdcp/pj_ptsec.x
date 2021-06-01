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
  
        Name:    LTE PDCP-MSPD t2k Interface file 
    
        Type:    C include file
  
        Desc:    This file contains all the data structures and 
                 prototypes for LTE PDCP - MSPD T2K SPAcc access.
 
        File:    pj_mspdt2k_ptsec.x

        Sid:      pj_qcom_ptsec.x@@/main/TeNB_QCOM_REL_BR/3 - Wed Sep 10 15:44:49 2014
  
        Prg:     N/A 
  
*********************************************************************21*/
/** @file pj_mspdt2k_ptsec.x
@brief PDCP Product Structures, prototypes
*/

#ifndef __PjQcomPDT2KPTSECX__
#define __PjQcomPDT2KPTSECX__


/*
 *     The structures and variables declared in this file
 *     correspond to structures and variables used by
 *     the following TRILLIUM software:
 *
 *     part no.             description
 *     --------     ----------------------------------------------
 *     1000371      LTE-PDCP
 *
*/
 
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
*@brief: This holds the information for mapping the o/p from SPAcc
*	 (ciphered/deciphered) and send it to RLC/Upper layer.
*
*@details
*    - cellId           : Cell id
*    - ueId	        : Ue-Id
*    - rbType		: SRB/DRB (0 for SRB, 1 for DRB)
*    - rbId		: Rb Id
*    - count		: Count value of the PDU to be ciphered/deciphered
*    - snLen		: Length of the SN. 7bits or 12bits.
*    - pdcpInst		: UL or DL PDCP Instance
*/
typedef struct pjSPACCOutputMapInfo{
   CmLtePdcpId pdcpId;
   U8 snLen;
   U8 pdcpInstance;
   U32 count;
}PjQcomOutputMapInfo;


/**
*@brief: This holds the list of PDU's to be passed as i/p to SPAcc along 
*	 with the mapping info for processing the o/p from SPAcc. It is 
*	 basically the structure into which PDCP PDU's are queued.
*
*@details
*    - ciphQLink	: Link to next node
*    - pduInfo		: Mapping Info for processing o/p from SPAcc
*    - spaccInput		: i/p structure to SPAcc.
*/
typedef struct PjQcomDataCb{
   CmLList ciphQLink;
   Buffer *mBuf;
   Buffer *opSdu;
   PjLibTrans *libTrans;
}PjQcomDataCb;


typedef struct pjQcomKeyReq
{
   Bool isFree;
   struct 
   {
     U32 intgKeyIdx;
     U32 upCiphKeyIdx;
     U32 cpCiphKeyIdx;
   };
   CmLteRnti ueId;
   U32 coreId;
}PjQcomKeyReq;


/** @brief This structure the security information
 *         sent from the PDCP Layer 
 **/
typedef struct PjQcomSecInfo
{
    CmLteCellId cellId;
    CmLteRnti ueId;
    U32  rbId;
    U8   rbType;
    U8   dir;
    U8   snLen;
    U8   pdcpInstance;
    U8   hash_key[PJ_MS_KENB_LEN];
    U8   strS[PJ_MS_STR_S_LEN];
    U8   strSlen;
    U8   integ_key[CPJ_INTG_KEY_LEN];
    U8   intAlgoType;
    /*U8   count[PJ_NUM_BYTES_COUNT];*/
    U32  count;
    /*U8   fresh[PJ_NUM_BYTES_FRESH];*/
    U32   fresh;
    U8   upCiph_key[CPJ_CIPHER_KEY_LEN];
    U8   cpCiph_key[CPJ_CIPHER_KEY_LEN];
    U8   ciphAlgoType;
    PjLibTrans *libTrans;
}PjQcomSecInfo;

/** @brief This enum defines the Security Operation 
 **/
typedef enum
{
    PJ_CIPHERING,
    PJ_INTEGRITY,
    PJ_KDF
}PjQcomSecOp;

/** @brief This enum defines the Integrity Algorithm
 *         Types
 *  */
typedef enum
{
    PJ_SPACC_EIA0,
    PJ_SPACC_EIA1,
    PJ_SPACC_EIA2,
    PJ_SPACC_EIA3

}PjQcomIntAlgo;

/** @brief This enum defines the Ciphering Algorithm
 *         Types
 *  */
typedef enum
{
    PJ_SPACC_EEA0,
    PJ_SPACC_EEA1,
    PJ_SPACC_EEA2,
    PJ_SPACC_EEA3
}PjQcomCiphAlgo;

/******************************************************************************
*                     EXTERN Declarations 
******************************************************************************/

#ifdef SPACC_NONBLOCKING
EXTERN S32 PjQcomPollUlQueue ARGS ((Void));
EXTERN S32 PjQcomPollDlQueue ARGS ((Void));
EXTERN Void PjQcomSetResetDlQueueFree ARGS ((U8 setReset));
EXTERN Void PjQcomSetResetUlQueueFree ARGS ((U8 setReset));
#endif

EXTERN Void ysMsSPAccInitDlMemRegionPool ARGS ((
Region region,
Pool   pool
));

EXTERN Void ysMsSPAccInitUlMemRegionPool ARGS ((
Region region,
Pool   pool
));

EXTERN U32 pjSpaccHdlDlfdOpenReq ARGS ((Void));

EXTERN U32 pjSpaccHdlUlfdOpenReq ARGS ((Void));

EXTERN Void PjQcomInitDlQue ARGS ((Void));

EXTERN Void PjQcomInitUlQue ARGS ((Void));

EXTERN Void PjQcomInitDlMemRegionPool ARGS((
Region region,
Pool   pool
));

EXTERN Void PjQcomInitUlMemRegionPool ARGS ((
Region region,
Pool   pool
));

EXTERN S16 PjQcomKDF ARGS ((
PjQcomSecInfo *pSecInfo,
U8   *key
));

EXTERN U32 PjQcomPrepareAndQPacketForDeCiph ARGS ((
PjQcomSecInfo *pSecInfo, 
Buffer *mBuf
));

EXTERN U32 PjQcomPrepareAndQPacketForCiph ARGS ((
PjQcomSecInfo *pSecInfo, 
Buffer *mBuf
));


EXTERN U32 PjQcomBatchAndCipherPackets ARGS((Void));

EXTERN U32 PjQcomBatchAndDeCipherPackets ARGS((Void));

EXTERN U32 PjQcomHdlDlfdOpenReq ARGS((Void));

EXTERN U32 PjQcomHdlUlfdOpenReq ARGS((Void));

EXTERN Void PjQcomDlfdCloseReq ARGS((Void));

EXTERN Void PjQcomUlfdCloseReq ARGS((Void));

EXTERN U32 PjQcomFetchUlPduInfo ARGS((
PjQcomOutputMapInfo *procInfo,
Buffer **opSdu,
U32 jobNum,
U32 errCode
));

EXTERN U32 PjQcomFetchDlPduInfo ARGS((
PjQcomOutputMapInfo *procInfo,
Buffer **opSdu,
U32 jobNum,
U32 errCode
));

EXTERN U32 PjQcomIntegVerifyPkt ARGS ((
PjQcomSecInfo *pSecInfo, 
Buffer **mBuf,
U32 macI
));


EXTERN U32 PjQcomAesIntegProtPkt ARGS ((
PjQcomSecInfo *pSecInfo, 
Buffer **mBuf
));

EXTERN U32 PjQcomIntegProtPkt ARGS ((
PjQcomSecInfo *pSecInfo, 
Buffer **mBuf
));

EXTERN U32 PjQcomCheckSpaccQueue ARGS((Void));

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PjQcomPDT2KPTSECX__ */

  
/********************************************************************30**
  
         End of file:     pj_qcom_ptsec.x@@/main/TeNB_QCOM_REL_BR/3 - Wed Sep 10 15:44:49 2014
  
*********************************************************************31*/

/********************************************************************40**

        Notes:

*********************************************************************41*/
/********************************************************************50**

        Revision history:

*********************************************************************51*/
/********************************************************************90**

     ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
              ---      chebli         1. initial release for Qualcomm CL
*********************************************************************91*/

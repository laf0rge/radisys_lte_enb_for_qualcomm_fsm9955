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


/************************************************************************

     Name:     LTE MAC PDCP Interface 

     Type:     CPP source file

     Desc:     CPP source code to interface with Qualcomm Crypto

     File:     

     Sid:      pj_qcom_ptsec.cpp@@/main/TeNB_QCOM_REL_BR/7 - Tue Nov 18 12:17:13 2014

     Prg:     

**********************************************************************/
static const char* RLOG_MODULE_NAME="PDCP";
static int RLOG_FILE_ID=6;
static int RLOG_MODULE_ID=1024;

#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer defines */
#include "cm_tkns.h"       /* Common tokens defines */
#include "cm_mblk.h"       /* Common memory allocation library defines */
#include "cm_llist.h"      /* Common link list defines  */
#include "cm_hash.h"       /* Common hashlist defines */
#include "cm_lte.h"        /* Common LTEE defines */
#include "cpj.h"           /* CPJ Interface defines */
#include "pj_ptsec.h"
#include "lpj.h"
#include "cpj.h"
#include "pju.h"
#include "pj.h"
#include "pj_env.h"

#include "custcommon.h"

#include "qurt.h"
#include "assert.h"
#include "cryptoDrv/cryptoJobReq.hpp"
#include "cryptoDrv/cryptoSetKeyReq.hpp"
#include "mainProc.hpp"
#include "errorCodes.hpp"

#include "gen.x"           /* General */
#include "ssi.x"           /* System services */
#include "cm5.x"           /* Common timer library */
#include "cm_tkns.x"       /* Common tokens */
#include "cm_mblk.x"       /* Common memory allocation */
#include "cm_llist.x"      /* Common link list */
#include "cm_hash.x"       /* Common hashlist */
#include "cm_lte.x"        /* Common LTE includes */
#include "cm_lib.x"
#include "lpj.x"
#include "cpj.x"
#include "pju.x"
#include "pj.x"
#include "pj_udx.h"          /* LIB */
#include "pj_udx.x"          /* LIB */
#include "pj_dl.x"
#include "pj_ul.x"
#include "pj_lib.x"
#include "pj_ptsec.x"

#ifdef __cplusplus
extern "C" {
#endif
#define PJ_QCOM_MAX_QUEUE_SIZE 10000

using namespace CryptoDrStructs;
PUBLIC U32 ulMemFailCnt;
PUBLIC U32 numCryptoFail=0;

PRIVATE CmLListCp PjQcomCryptoQ[PJ_QCOM_MAX_CRYPTO_CORE];
PRIVATE Qcom::GmodId cryptCore[PJ_QCOM_MAX_CRYPTO_CORE]={Qcom::CRYPTO1_MOD, Qcom::CRYPTO2_MOD};
PUBLIC U32 numOutStandingCryptoJobs[PJ_QCOM_MAX_CRYPTO_CORE],maxQuSiz;
/*static unsigned long long startPcyclesE1_;*/
EXTERN U8 keyChange;

PRIVATE Qcom::CryptoJobReq* pjQcomCreateCryptoJobIntg ARGS ((
PjLibTrans *libTrans,
PjQcomSecOp secOp, 
Buffer *mBuf
));

PRIVATE Qcom::CryptoJobReq* pjQcomCreateCryptoJobCiph ARGS ((
PjLibTrans *libTrans,
Buffer *mBuf,
Buffer *opSdu
));

/**
 * @brief     - File scope post structure to be used in call back funtion 
 *              after packing relavent data for integrity protection/verification.
 *
 */
PRIVATE Pst gPstIntProt = 
{
   0,
   0,
   ENTPJ,
   1/*DL INST*/,
   ENTTF,
   0,
   0,
   0,
   0xa1/*OBD_EVT_INT_PROT_CFM*/,
   0,
   0,
   0,
   0
};

PRIVATE Pst gPstIntVer = 
{
   0,
   0,
   ENTPJ,
   0/*UL INST*/,
   ENTTF,
   0,
   0,
   0,
   0xa2/*OBD_EVT_INT_VER_CFM*/,
   0,
   0,
   0,
   0
};

PRIVATE Pst gPstCiph = 
{
   0,
   0,
   ENTPJ,
   1/*DL INST*/,
   ENTTF,
   0,
   0,
   0,
   0xb2/*OBD_EVT_CIPHER_CFM*/,
   0,
   0,
   0,
   0
};

PRIVATE Pst gPstDeciph = 
{
   0,
   0,
   ENTPJ,
   0/*UL INST*/,
   ENTTF,
   0,
   0,
   0,
   0xb3/*OBD_EVT_DECIPHER_CFM*/,
   0,
   0,
   0,
   0
};

/**
 * @brief     - Qualcomm crypto call back funtion for ciphering operation
 *
 * @details   - Qualcomm crypto calls this funtion after krypto operation with below
 *              arguments.
 *
 * @param[in] - cbParam0 - Ptr user context 0 
 * @param[in] - cbParam1 - Ptr user context 1 
 * @param[in] - cbParam2 - Ptr user context 2
 * @param[in] - cbParam3 - User variable
 * @param[in] - result - result
 * @param[in] - macI - maci 
 * @param[in] - ueIndex - UE index 
 *
 * @return    - Void 
 */

Void pjQcomCiphCallBk
( 
Void           *cbParam0,    
Void           *cbParam1,    
Void           *cbParam2,    
QcomUint32_t   cbParam3,
CryptoResult   result,    
QcomUint32_t   macI,    
QcomUint32_t   ueIndex
)
{
     PjLibTrans *libTrans = (PjLibTrans*)cbParam0;
     Buffer *opSdu=(Buffer *)cbParam1;

#ifdef QCOM_TOBE_REMOVED 
     if(SValidateDfltRegnPtr(libTrans) != 0)
     {
        CRASH();
     }
#endif

     if(result == CryptoDrStructs::SUCCESS/*0 = success*/)
     {
        if(cbParam3 == PJ_SEC_DIR_DL)
        {
           //QU_CRITLOG(QU_LOG_MESSAGE,"PDCP:Call Back After Ciphering");
            cmPkLibObdCipherCfm(&gPstCiph,libTrans,opSdu);
        }
        else
        {
            cmPkLibObdDecipherCfm(&gPstDeciph,libTrans,opSdu);
        }
     }
     else
     {
        QU_CRITLOG(QU_LOG_MESSAGE,"CIPH CALL BK RES: %d,dir=%d rbTp=%d", result, cbParam3, libTrans->rbType);
        if(cbParam3 == PJ_SEC_DIR_UL)
        {
           libTrans->res = 9;
           numCryptoFail ++;
           //QU_CRITLOG(QU_LOG_MESSAGE, "ULFIFO:Aloc(%ld),cryptRcd=%ld,TotAlc(%ld)", ulMemAlloc, ulCryptoRcvd, myipcFifoAllocCnt);
           //cmPkLibObdDecipherCfm(&gPstDeciph,libTrans,opSdu);
        }
     }
}

/**
 * @brief     - Qualcomm crypto call back funtion for integrity operation
 *
 * @details   - Qualcomm crypto calls this funtion after krypto operation with below
 *              arguments.
 *
 * @param[in] - cbParam0 - Ptr user context 0 
 * @param[in] - cbParam1 - Ptr user context 1 
 * @param[in] - cbParam2 - Ptr user context 2
 * @param[in] - cbParam3 - User variable
 * @param[in] - result - result
 * @param[in] - macI - maci 
 * @param[in] - ueIndex - UE index 
 *
 * @return    - pointer to SPACC input of struct elp_spacc_ioctl *
 */

Void pjQcomIntgCallBk
( 
Void            *cbParam0,    
Void            *cbParam1,    
Void            *cbParam2,    
QcomUint32_t    cbParam3,
CryptoResult    result,    
QcomUint32_t    macI,    
QcomUint32_t    ueIndex
)
{
     PjLibTrans *libTrans = (PjLibTrans*)cbParam0;
     PjCb    *tPjCb;
     U32 recvMaci = libTrans->macI;

#ifdef QCOM_TOBE_REMOVED 
     if(SValidateDfltRegnPtr(libTrans) != 0)
     {
        CRASH();
     }
#endif

     /*RsysSHMLog("pjQcomIntgCallBk called result: %d macI %lu time %d \n", result, macI, elapsedUs);*/
     /*QU_CRITLOG(QU_LOG_MESSAGE,"INTG CALL BK RES: %d macI %ld rcvMacI %ld\n", result, macI, recvMaci);*/
     if(result == CryptoDrStructs::SUCCESS/*0 = success*/)
     {
        if(cbParam3 == PJ_SEC_DIR_DL)
        {

            tPjCb = pjCb[PJ_SEC_DIR_DL];
            cmPkLibObdIntProtCfm(&gPstIntProt,  libTrans, (U32)macI);
            /*PJ_FREE(tPjCb,libTrans, sizeof(PjLibTrans));*/
        }
        else
        {
            Status status=RFAILED;

            tPjCb = pjCb[PJ_SEC_DIR_UL];
            if(macI == recvMaci)
            {
               status=ROK;
            }
            cmPkLibObdIntVerCfm(&gPstIntVer,  libTrans, status);
            /*PJ_FREE(tPjCb,libTrans, sizeof(PjLibTrans));*/
         }
     }
}


/**
 * @brief     - Funtion to prepare data structures for Ciphering and integrity
 *              protection.
 *
 * @details   - This function accepts inputs from PDCP to prepare mindspeed
 *              internal data structure before calling the funtion to perform
 *              actual operation.
 *
 * @param[in] - pSecInfo - Inputs from PDCP 
 * @param[in] - mBuf - Input Buffer 
 * @param[in] - opSdu - Output Buffer 
 *
 * @return    - pointer to SPACC input of struct elp_spacc_ioctl *
 */
#ifdef ANSI
PRIVATE Qcom::CryptoJobReq* pjQcomCreateCryptoJobCiph
(
PjLibTrans *libTrans,
Buffer *mBuf,
Buffer *opSdu
)
#else
PRIVATE CryptoJobReq* pjQcomCreateCryptoJobCiph(libTrans, mBuf, opSdu)
PjLibTrans *libTrans;
Buffer *mBuf;
Buffer *opSdu;
#endif
{
   MsgLen                  mBufLen;
   Qcom::CryptoJobReq*   reqPtr; 
   CryptoJob               job;
   Buffer                  *tmpMBuf;
   Buffer                  *tmpMBuf1;
   U32                      index=0;
   U32                     ciphAlgoType; 


   TRC3(pjQcomCreateCryptoJobCiph);


    cmMemset((U8*)&job,0,sizeof(CryptoJob));

    /*QU_CRITLOG(QU_LOG_MESSAGE,"pjQcomCreateCryptoJobCiph START \n");*/

    /*Assign the scattered dBuf addresses into job->src array
      Common code for Ciphering and integrity*/
    tmpMBuf = mBuf->b_cont;
    while(tmpMBuf)
    {
       job.src[index].buffer = tmpMBuf->b_rptr;
       job.src[index].len = tmpMBuf->b_wptr - tmpMBuf->b_rptr;
       index++; 
       tmpMBuf = tmpMBuf->b_cont;
 //      if(tmpMBuf != NULL && index >= 15)
       if(tmpMBuf != NULL && index >= 4)
       {
           QU_CRITLOG(QU_LOG_MESSAGE,"Index exceed 4, dropping packet %d\n",libTrans->dir);
           RETVALUE(NULLP);
       }
    }

    job.numSrcBuf = index; /*dBuf count*/

    /*Initialize destination buffer. Number of Destiantion buffer is always 1*/
    tmpMBuf1 = opSdu->b_cont;

#if 0
    /* to verify address */
    if((libTrans->dir == PJ_SEC_DIR_UL) && (libTrans->rbType == PJ_DRB) && 
        (( 0x3EC00000 > (U32)tmpMBuf1->b_rptr) || (0x3F000000 < (U32)tmpMBuf1->b_rptr)))
    {
        QU_CRITLOG(QU_LOG_MESSAGE," b_rptr: %x, tmpMBuf1:%x ",
			tmpMBuf1->b_rptr,tmpMBuf1);
        /* THis should not occur. SO crashing enodeb here */
        U8 *p = 0;
        *p = 8;
    }
#endif	

    job.dst[0].buffer = tmpMBuf1->b_rptr;
    job.dst[0].len = tmpMBuf1->b_wptr - tmpMBuf1->b_rptr;
    job.numDstBuf = 1; /*No dst Buf for MACI*/
    mBufLen = job.dst[0].len;

    /*If operation is ciphering or deciphering*/

    job.encPduSize = mBufLen;
    job.encPduOffset = 0;

    job.pduSize = mBufLen;

    job.encKeyRev = keyChange;

    if(libTrans->rbType == PJ_SRB)
    {
       /*job.encKeyPtr = (QcomUint8_t *) pSecInfo->cpCiph_key;*/
       if(libTrans->dir == PJ_SEC_DIR_DL)
       {
          job.encKeyPtr = (QcomUint8_t *) ((PjDlUeCb *)libTrans->ueCb)->secInfo.cipherInfo.cpKey;
          ciphAlgoType = ((PjDlUeCb *)libTrans->ueCb)->secInfo.cipherInfo.algoType; 
       }
       else
       {
          job.encKeyPtr = (QcomUint8_t *) ((PjUlUeCb *)libTrans->ueCb)->secInfo.cipherInfo.cpKey;
          ciphAlgoType = ((PjUlUeCb *)libTrans->ueCb)->secInfo.cipherInfo.algoType; 
       }
    }
    else
    {
       if(libTrans->dir == PJ_SEC_DIR_DL)
       {
          job.encKeyPtr = (QcomUint8_t *) ((PjDlUeCb *)libTrans->ueCb)->secInfo.cipherInfo.upKey;
          ciphAlgoType = ((PjDlUeCb *)libTrans->ueCb)->secInfo.cipherInfo.algoType; 
       /*QU_CRITLOG(QU_LOG_MESSAGE,"cipAlgo %d key %x %x %x %x DOWN\n",ciphAlgoType, job.encKeyPtr[0],job.encKeyPtr[1],job.encKeyPtr[14], job.encKeyPtr[15]);*/
       }
       else
       {
          job.encKeyPtr = (QcomUint8_t *) ((PjUlUeCb *)libTrans->ueCb)->secInfo.cipherInfo.upKey;
          ciphAlgoType = ((PjUlUeCb *)libTrans->ueCb)->secInfo.cipherInfo.algoType; 
       /*QU_CRITLOG(QU_LOG_MESSAGE,"Alg %d:%x %x %x %x UP index %d dst %p\n",ciphAlgoType, job.encKeyPtr[0],job.encKeyPtr[1],job.encKeyPtr[14], job.encKeyPtr[15], index, job.dst[0].buffer);*/
       }
    }
    job.direction = (CryptoDrStructs::Direction)libTrans->dir;
    job.count = libTrans->count;
    job.bearer = libTrans->rbId;

    job.doneCallback = pjQcomCiphCallBk;

    /*QU_CRITLOG(QU_LOG_MESSAGE,"DIR %d CNT %d RB %d\n", job.direction, job.count, job.bearer);*/
    /*call back variable :Integrity Ver/Prot pack funtions called based on below*/
    job.cbParam3 = libTrans->dir;

#ifdef QCOM_TOBE_REMOVED 
     if(SValidateDfltRegnPtr(libTrans) != 0)
     {
        CRASH();
     }
#endif

    /*call back context*/
    job.cbParam0 = libTrans; /*Type PjLibTrans*/
    job.cbParam1 = opSdu; /*Type PjLibTrans*/

    switch (ciphAlgoType)
    {
        /*ZUC alogorithm Ciphering*/
        case PJ_SPACC_EEA3:
            job.encAlg = ENC_ZUC;
            break;
        /*AES alogorithm Ciphering*/
        case PJ_SPACC_EEA2:
            job.encAlg = ENC_AES;
            break;
        case PJ_SPACC_EEA1:
    /*QU_CRITLOG(QU_LOG_MESSAGE,"numSrcBuf %d numDstBuf %d authPduSize %d PduSize %d  \n", job.numSrcBuf, job.numDstBuf, job.encPduSize, job.pduSize);*/
            job.encAlg = ENC_SNOW3G;
            break;
        case PJ_SPACC_EEA0:
            job.encAlg = ENC_NONE;
            break;
    }

    /*Get memory for Job Requset object*/
    reqPtr = new Qcom::CryptoJobReq();
    if( reqPtr == NULLP)
    {
       RsysSHMLog("ERROR: pjQcomCreateCryptoJobIntg job or reqPtr NULL !!!!!\n");
       RETVALUE(NULL);
    }
    reqPtr->job(&job);

    /*QU_CRITLOG(QU_LOG_MESSAGE,"pjQcomCreateCryptoJobCiph END\n");*/
    return reqPtr;
}

/**
 * @brief     - Funtion to prepare data structures for Ciphering and integrity
 *              protection.
 *
 * @details   - This function accepts inputs from PDCP to prepare mindspeed
 *              internal data structure before calling the funtion to perform
 *              actual operation.
 *
 * @param[in] - pSecInfo - Inputs from PDCP 
 * @param[in] - secOp - Ciphering or Integrity Protection
 * @param[in] - mBuf - Input Buffer 
 *
 * @return    - pointer to SPACC input of struct elp_spacc_ioctl *
 */
#ifdef ANSI
PRIVATE Qcom::CryptoJobReq* pjQcomCreateCryptoJobIntg
(
PjLibTrans *libTrans, 
PjQcomSecOp secOp, 
Buffer *mBuf
)
#else
PRIVATE CryptoJobReq* pjQcomCreateCryptoJobIntg(libTrans, secOp, mBuf)
PjLibTrans *libTrans;
PjQcomSecOp secOp;
Buffer *mBuf;
#endif
{
   MsgLen                  mBufLen;
   Qcom::CryptoJobReq*   reqPtr; 
   CryptoJob               job;
   Buffer                  *tmpMBuf;
   U32                      index=0;
   U8                      intAlgoType; 


   TRC3(pjQcomCreateCryptoJobIntg);

   SFndLenMsg(mBuf, &mBufLen);

   if(mBufLen == 0)
   {
      return NULLP;
   }

    /*Get memory for Job strcutyure*/
    /*for now keeping region and pool as 0 0*/
    cmMemset((U8*)&job,0,sizeof(CryptoJob));

    /*QU_CRITLOG(QU_LOG_MESSAGE,"pjQcomCreateCryptoJobIntg START\n");*/
    /*Assign the scattered dBuf addresses into job->src array
      Common code for Ciphering and integrity*/
    tmpMBuf = mBuf->b_cont;

    while(tmpMBuf)
    {
       job.src[index].buffer = tmpMBuf->b_rptr;
       job.src[index].len = tmpMBuf->b_wptr - tmpMBuf->b_rptr;
       
       job.dst[index].buffer = tmpMBuf->b_rptr;
       job.dst[index].len = tmpMBuf->b_wptr - tmpMBuf->b_rptr;
        
       index++;

       tmpMBuf = tmpMBuf->b_cont;
       if(tmpMBuf != NULL && index >= 2)
       {
           RsysSHMLog("ERROR: scattered buffer count greater than 15\n");
           RETVALUE(NULLP);
       }
    }
    

    /*Dont need to initialize ouput buff I think as MAC-I is retuned as arg in call bk funtion*/
    /*job.dst[0].buffer = outbuf;
    job.dst[0].len = mBufLen;*/

    job.authPduSize = mBufLen;
    job.authPduOffset = 0;

    job.pduSize = mBufLen;

    job.authKeyRev = keyChange; 
   
    /*job.authKeyPtr = (QcomUint8_t *) pSecInfo->integ_key; */
    if(libTrans->dir == PJ_SEC_DIR_DL)
    {
       job.authKeyPtr = (QcomUint8_t *) ((PjDlUeCb *)libTrans->ueCb)->secInfo.intInfo.intKey;
       intAlgoType = ((PjDlUeCb *)libTrans->ueCb)->secInfo.intInfo.algoType;
    }
    else
    {
       job.authKeyPtr = (QcomUint8_t *) ((PjUlUeCb *)libTrans->ueCb)->secInfo.intInfo.intKey; 
       intAlgoType = ((PjUlUeCb *)libTrans->ueCb)->secInfo.intInfo.algoType; 
    }

    job.direction = (CryptoDrStructs::Direction)libTrans->dir;
    job.count = libTrans->count;
    job.bearer = libTrans->rbId;
   
    job.doneCallback = pjQcomIntgCallBk;

    job.numSrcBuf = index; /*dBuf count*/
    job.numDstBuf = index; /*No dst Buf for MACI*/
    
    /*call back variable :Integrity Ver/Prot pack funtions called based on below*/ 
    job.cbParam3 = libTrans->dir;
    
    /*call back context*/
    job.cbParam0 = libTrans; /*Type PjLibTrans*/
    /*QU_CRITLOG(QU_LOG_MESSAGE,"numSrcBuf %d authPduSize %d PduSize %d  \n", job.numSrcBuf, job.authPduSize, job.pduSize);*/

#ifdef QCOM_TOBE_REMOVED 
     if(SValidateDfltRegnPtr(libTrans) != 0)
     {
        CRASH();
     }
#endif
 
    switch (intAlgoType)
    {
        /*ZUC alogorithm Integrity*/
        case PJ_SPACC_EIA3:
            job.authAlg = AUTH_ZUC;
            break;
        /*AES alogorithm Integrity*/
        case PJ_SPACC_EIA2:
            job.authAlg = AUTH_AES;
            break;
        /*SNOW3G alogorithm Integrity*/
        case PJ_SPACC_EIA1:
            job.authAlg = AUTH_SNOW3G;
            job.fresh = (libTrans->rbId << 27);
    
            break;
    }
    /*QU_CRITLOG(QU_LOG_MESSAGE,"count %lu bearer %d dir %d fresh %d\n", job.count, job.bearer, job.direction, job.fresh);*/
    /*QU_CRITLOG(QU_LOG_MESSAGE,"pjQcomCreateCryptoJobIntg END\n");*/

   /*Get memory for Job Requset object*/
   reqPtr = new Qcom::CryptoJobReq();
   if( reqPtr == NULLP)
   {
      RsysSHMLog("ERROR: pjQcomCreateCryptoJobIntg job or reqPtr NULL !!!!!\n");
      RETVALUE(NULL);
   }

   reqPtr->job(&job);
   return reqPtr;
}


/**
 * @brief This function is used for Integrity Verification for
 *        the buffer received as input.
 *
 * @details This function sets the context for the integrity 
 *          verification. 
 *
 * @param[in] pSecInfo - Information passed from PDCP.
 * @param[in] mBuf - Buffer for Integrity Verification  
 *
 * @return             - ROK/RFAILED
 */
#ifdef ANSI
PUBLIC U32 pjQcomIntegOperation
(
PjLibTrans *libTrans, 
Buffer     *mBuf
)
#else
PUBLIC U32 pjQcomIntegOperation(pSecInfo, mBuf)
PjQcomSecInfo *pSecInfo;
Buffer        *mBuf;
#endif
{
   Qcom::CryptoJobReq*    reqPtr;

   TRC3(pjQcomIntegOperation);

   /*QU_CRITLOG(QU_LOG_MESSAGE,"pjQcomIntegVerifyPkt()\n");*/
   reqPtr = pjQcomCreateCryptoJobIntg(libTrans, PJ_INTEGRITY, mBuf); 
   if(reqPtr == NULLP)
   {
      RETVALUE(RFAILED);
   }
   /*Invoke asynchronously */
    /*startPcyclesE1_ = qurt_get_core_pcycles();*/
#if 1
   if(ssdRouteMsg(ssdGetMyModuleId(), Qcom::CRYPTO1_MOD, reqPtr, MSG_CIRC_QUEUE) != ROK)
   {
      delete reqPtr;
      RETVALUE(RFAILED);
   }
#else
   mainProc()->getMsgRouter().routeMsg(Qcom::CRYPTO2_MOD, reqPtr, Qcom::INVALID_GMODID);
#endif
   
   RETVALUE(ROK);
}


/********************************************************************************************/
/* CIPHERING SECTION START*/
/********************************************************************************************/

/**
*@brief: Function to determine whether Crypto Core 1 is slected or 2 
*
*@inparam[in] - libTrans PDCP packet transaction CB. 
*
*@return      - Core 1 or Core 2
*/

PUBLIC U32 pjQcomGetCryptoCoreId(PjLibTrans *libTrans)
{
   if(libTrans->dir == PJ_SEC_DIR_UL)
      return PJ_SEC_DIR_UL;
   else
      return PJ_SEC_DIR_DL;
}


/**
*@brief: Function to initialiaze Qcom DeCipherJob  Queue
*
*@inparam[in] - Void
*
*@return      - Void
*/

#ifdef ANSI
PUBLIC Void pjQcomInitUlQue(Void)
#else
PUBLIC Void pjQcomInitUlQue(Void)
#endif
{
   TRC3(pjQcomInitUlQue);

   cmLListInit(&PjQcomCryptoQ[0]);
   cmLListInit(&PjQcomCryptoQ[1]);

   RETVOID;
}

/**
 * @brief         - This function is used to prepare the buffer received as
 *                  input for ciphering/deciphering.
 *
 * @details       - This function sets the context for the deciphering.
 *                  It deques the packet from Crypto1/Crypt2 queue of PDUs waiting for
 *                  ciphering/deciphering
 *
 * @param[in]     - cryptoPath - Crypt Core 1 or 2.
 *
 * @return ROK/RFAILED
 */


PRIVATE U32 checkAndSendPacketsToCrypto(U8 cryptoPath)
{
   PjQcomDataCb *node = NULLP;
   U32 numJobs = 0;
   Qcom::CryptoJobReq*    reqPtr;	
   U32 numMsgsToSend;

   TRC3(pjQcomBatchAndDeCipherPackets);
  
   /*PJ_QCOM_MAX_JOBS_SENT is currently set to 4*/ 
   numMsgsToSend = PJ_QCOM_MAX_JOBS_SENT<(128 - (numOutStandingCryptoJobs[cryptoPath]))? \
                     PJ_QCOM_MAX_JOBS_SENT:128 - (numOutStandingCryptoJobs[cryptoPath]) ;

   for(numJobs =0; numJobs < numMsgsToSend; numJobs++)
   {
      node = (PjQcomDataCb*)cmLListFirst(&PjQcomCryptoQ[cryptoPath]);
      if(node == NULLP)
      {
         break;
      }
      {
         PjLibTrans* libTrans = node->libTrans; 
         if(libTrans->dir == PJ_SEC_DIR_UL)
         {
           if(((PjRxEnt *)(libTrans->rxEnt))->state == PJ_PDU_TO_DELETE)
           {
              QU_CRITLOG(QU_LOG_MESSAGE, "rxEnt state PJ_PDU_TO_DELETE node(%p)",node);
              /*Free rxEnt and libtrans*/
              PJ_FREE(pjCb[PJ_SEC_DIR_UL], libTrans->rxEnt, sizeof(PjRxEnt));
              PJ_FREE(pjCb[PJ_SEC_DIR_UL],libTrans, sizeof(PjLibTrans));

              cmLListDelFrm(&PjQcomCryptoQ[cryptoPath], &(node->ciphQLink));
              //node->ciphQLink.node = NULLP;
              PJ_FREE(pjCb[PJ_SEC_DIR_UL],node, sizeof(PjQcomDataCb));

              continue;
           }

           if(node->opSdu == NULLP) /* if the opSdu is already allocated don't allocated again */
           {
              MsgLen len;
              SFndLenMsg(node->mBuf, &len);
              /*QU_CRITLOG(QU_LOG_MESSAGE, "rxEnt state %d  node(%p) len %d ",((PjRxEnt *)(libTrans->rxEnt))->state, node, len);*/
              if (SGetUlMemMsg(&(node->opSdu), len) == 1)
              {
                 ulMemFailCnt++;
                 RETVALUE(ROK);
              }
           }
         }
         else
         {
           if(((PjTxEnt *)(libTrans->txEnt))->state == PJ_PDU_TO_DELETE)
           {
              QU_CRITLOG(QU_LOG_MESSAGE, "txEnt state is PJ_PDU_TO_DELETE");
              /*Free txEnt and libtrans*/
              PJ_FREE(pjCb[PJ_SEC_DIR_DL], libTrans->txEnt, sizeof(PjTxEnt));
              PJ_FREE(pjCb[PJ_SEC_DIR_DL],libTrans, sizeof(PjLibTrans));

              cmLListDelFrm(&PjQcomCryptoQ[cryptoPath], &(node->ciphQLink));
              //node->ciphQLink.node = NULLP;
              PJ_FREE(pjCb[PJ_SEC_DIR_DL],node, sizeof(PjQcomDataCb));

              continue;
           }
         }
      }
      /*QU_CRITLOG(QU_LOG_MESSAGE,"$$$$$ %ld %d %p %p %p %p\n", node->libTrans->count, node->libTrans->rbId, node->libTrans, node->mBuf, node->opSdu, &(node->opSdu));*/
      reqPtr = pjQcomCreateCryptoJobCiph(node->libTrans, node->mBuf, (node->opSdu));
      if(NULLP == reqPtr)
      {
         QU_CRITLOG(QU_LOG_MESSAGE, "Double Free Failure cryptoJobCreate");
         RETVALUE(RFAILED);
      }

      if(ssdRouteMsg(ssdGetMyModuleId(), cryptCore[cryptoPath], reqPtr, MSG_CIRC_QUEUE) != ROK)
      {
         delete reqPtr;
         QU_CRITLOG(QU_LOG_MESSAGE, "Double Free Failure ssdRouteMsg");
         RETVALUE(ROK);
      }
      numOutStandingCryptoJobs[cryptoPath]++;

      cmLListDelFrm(&PjQcomCryptoQ[cryptoPath], &(node->ciphQLink));
      node->ciphQLink.node = NULLP;
      node->mBuf = NULLP;
      PJ_FREE(pjCb[PJ_SEC_DIR_DL],node, sizeof(PjQcomDataCb));
   }
   RETVALUE(ROK);
}

/**
 * @brief         - This function is used to prepare the buffer received as 
 *                  input for deciphering.
 *
 * @details       - This function sets the context for the deciphering.
 *                  It enqueues the packet into the queue of PDUs waiting for
 *                  deciphering
 *                  
 * @param[in]     - pSecInfo - Information passed from PDCP.
 * @param[in]     - mBuf - Bufer to be deciphered.
 *
 * @return ROK/RFAILED
 */

#ifdef ANSI
PUBLIC U32 pjQcomPrepPacketForCiphDeciph
(
PjLibTrans *libTrans,
Buffer *mBuf,
Buffer *opSdu
)
#else
PUBLIC U32 pjQcomPrepPacketForCiphDeciph (libTrans, mBuf, opSdu)
PjLibTrans *libTrans;
Buffer *mBuf;
Buffer *opSdu;
#endif
{
   
   TRC3(pjQcomPrepPacketForCiphDeciph);
   
   /*QU_CRITLOG(QU_LOG_MESSAGE,"pjQcomPrepareAndQPacketForDeCiph()\n");*/
 
   /*QU_CRITLOG(QU_LOG_MESSAGE,"DEC REQ %ld %d %p %p %p %p\n", libTrans->count, libTrans->rbId, libTrans, mBuf, opSdu, *opSdu);*/
   if (libTrans->rbType == PJ_SRB)
   {
      Qcom::CryptoJobReq*    reqPtr;	
      reqPtr = pjQcomCreateCryptoJobCiph(libTrans, mBuf,opSdu);
      if(NULLP == reqPtr)
      {
        //PJ_FREE(gCb,libTrans, sizeof(PjLibTrans));
        //SPutSBuf(0, 0 , (Data *) libTrans, sizeof(PjLibTrans));
        QU_CRITLOG(QU_LOG_MESSAGE, "ysQcomCipherPkt: pjQcomCreateCryptoJob returned NULL !!!!!\n");
        RETVALUE(RFAILED);
      }
#if 1
      if(ssdRouteMsg(ssdGetMyModuleId(), Qcom::CRYPTO1_MOD, reqPtr, MSG_CIRC_QUEUE) != ROK)
      {
         //PJ_FREE(gCb,libTrans, sizeof(PjLibTrans));
         //SPutSBuf(0, 0 , (Data *) libTrans, sizeof(PjLibTrans));
         delete reqPtr;
         RETVALUE(RFAILED);
      }
#else
      mainProc()->getMsgRouter().routeMsg(Qcom::CRYPTO2_MOD, reqPtr, Qcom::INVALID_GMODID);
#endif
   }
   else
   {
      PjQcomDataCb *node=NULLP;
     /*Get memory for the queue node*/
     PJ_ALLOC(pjCb[PJ_SEC_DIR_DL], node , sizeof(PjQcomDataCb));
     if(node == NULLP)
     {
       //PJ_FREE(gCb,libTrans, sizeof(PjLibTrans));
       //SPutSBuf(0, 0 , (Data *) libTrans, sizeof(PjLibTrans));
       RETVALUE(RFAILED);
     }

     node->mBuf = mBuf;
     node->opSdu = opSdu;
     node->libTrans = libTrans;

     /*Add to the list of PDUs to be deciphered*/
     node->ciphQLink.node = (PTR)node;
     cmLListAdd2Tail(&PjQcomCryptoQ[pjQcomGetCryptoCoreId(libTrans)], &node->ciphQLink);
     if(maxQuSiz < PjQcomCryptoQ[pjQcomGetCryptoCoreId(libTrans)].count)
     {
        maxQuSiz = PjQcomCryptoQ[pjQcomGetCryptoCoreId(libTrans)].count;
     }
     checkAndSendPacketsToCrypto(pjQcomGetCryptoCoreId(libTrans));
     //checkAndSendPacketsToCrypto(pjQcomGetCryptoCoreId(libTrans));
   }
   RETVALUE(ROK);	

}

/**
 * @brief         - This function is used to perform batch deciphering with
 *                  SPACC.
 *
 * @details       - This function is called on TTI indication .
 *                  It fetches the packets from the Cryto core queues and
 *                  sends to Qualcomm Crypto 1 and 2 for deciphering.
 *                  
 * @param[in]     - Void 
 *
 * @return         -ROK/RFAILED
 */

#ifdef ANSI
PUBLIC U32 pjQcomBatchAndDeCipherPackets ()
#else
PUBLIC U32 pjQcomBatchAndDeCipherPackets ()
#endif
{
  checkAndSendPacketsToCrypto(0);
  checkAndSendPacketsToCrypto(1);

  RETVALUE(ROK);
}

/**
 *  @brief This fn is called to chek the state of the PDCP pkts in DL before
 *         Ciphering operation
 *         and appropriate action is taken accordingly
 *
 *  @details
 *      Function : pjCiphStateChek
 *
 *  Processing Steps:
 *
 *
 * @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjCiphStateChek
(
 PjCb        *gCb,
 PjTxEnt     *txEnt
 )
#else
   PUBLIC S16 pjCiphStateChek(gCb, txEnt)
   PjCb        *gCb;
   PjTxEnt     *txEnt;
#endif
{
     TRC3(pjCiphStateChek);
     PJ_FREE_BUF(txEnt->sdu);
 #ifdef FLAT_BUFFER_OPT
            if(txEnt->fb.startAddr)
                  {
                            PJ_FREE_FLAT_BUF(gCb, (&(txEnt->fb)));
                               }
#endif
   PJ_FREE_BUF(txEnt->pdu);
   /*This txEnt is deleted in checkAndSendPacketsToCrypto() in
    * pj_ptsec_qcom.cpp*/
   if(txEnt->state == PJ_WAIT_FOR_CIPHER)
   {
      txEnt->state = PJ_PDU_TO_DELETE;
      RETVALUE(ROK);
   }

              PJ_FREE(gCb, txEnt, sizeof(PjTxEnt));
                RETVALUE(ROK);
}
/**
 *  @brief This fn is called to chek the state of the PDCP pkts in UL before
 *         Deciphering operation
 *         and appropriate action is taken accordingly
 *
 *  @details
 *      Function : pjDeciphStateChek
 *
 *  Processing Steps:
 *
 *
 * @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjDeciphStateChek
(
 PjCb        *gCb,
 PjRxEnt     *delEnt
 )
#else
   PUBLIC S16 pjDeciphStateChek(gCb, delEnt)
   PjCb        *gCb;
   PjRxEnt     *delEnt;
#endif
{
     TRC3(pjDeciphStateChek);
       PJ_FREE_BUF(delEnt->mBuf);
         PJ_FREE(gCb, delEnt, sizeof(PjRxEnt));
           RETVALUE(ROK);
}
/**
 *  @brief This fn cheks whether the RING buffer is used and therefore DL pkts
 *         need to be enqueued before forwarding it for ciphering.
 *         Note: It acts as a dummy fn so far in case of Mindspeed pltfm
 *
 *  @details
 *      Function : pjUtlChekTxEnqReq
 *
 *          Processing Steps:
 *
 *
 * @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUtlChekTxEnqReq
(
 PjCb       *gCb,
 PjDlRbCb   *pjRbCb,
 PjTxEnt    *txEnt
 )
#else
   PUBLIC S16 pjUtlChekTxEnqReq(gCb, pjRbCb, txEnt)
   PjCb       *gCb;
   PjDlRbCb   *pjRbCb;
   PjTxEnt    *txEnt;
#endif
{
     RETVALUE(ROK);
}
/**
 *  @brief This fn cheks whether the RING buffer is used and therefore UL pkts
 *         need to be enqueued before forwarding it for deciphering.
 *         Note: It acts as a dummy fn so far in case of Mindspeed pltfm
 *
 *  @details
 *      Function : pjUtlChekRxEnqReq
 *
 *          Processing Steps:
 *
 *
 * @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUtlChekRxEnqReq
(
 PjCb       *gCb, 
 PjUlRbCb   *pjRbCb,
 PjRxEnt    *rxEnt
 )
#else
   PUBLIC S16 pjUtlChekRxEnqReq(gCb, pjRbCb, rxEnt)
   PjCb       *gCb;
   PjUlRbCb   *pjRbCb;
   PjRxEnt    *rxEnt;
#endif
{
     RETVALUE(ROK);
}
/**
 *  @brief This fn does Batch Processing on every tick received from L1
 *         Note: It acts as a dummy fn so far in case of Mindspeed pltfm
 *
 *  @details
 *      Function : pjBatchProcTick
 *
 *          Processing Steps:
 *
 *
 * @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjBatchProcTick
(
 Void
 )
#else
   PUBLIC S16 pjBatchProcTick()
   Void;
#endif
{
     RETVALUE(ROK);
}

#ifdef DAM_PDCP_OPT
/**
 * @details
 * This primitive is used by the PDCP service user to request PDCP
 * for data transfer.
 *
 * @param[in] pst    - Pointer to Pst structure.
 * @param[in] spId  - Service provider ID. This is the reference number
 * used by PDCP service user (Relay) to identify its SAP. This reference number
 * will be provided by PDCP in any further primitives sent by PDCP
 * to the service user.
 *
 * @param[in] pdcpId - Pointer to CmLtePdcpId structure, PDCP id is used to 
 * uniquely identify an PDCP entity. 
 * @param[in] sduId  - SDU ID. 
 * @param[in] mBuf   - Pointer to Flat buffer.
 *
 * @return ROK
 */

#ifdef ANSI
   PUBLIC S16 PjUiPjuDatReqFBPdcpOpt
(
 U16            cellId,            
 U16            ueId,               
 U8            rbId,         
 U32           sduId, 
 FlatBuffer    *mBuf          
 )
#endif
{
   S16         ret;           /* Return Value */
   PjDlRbCb   *pjRbCb;       /* RB Control Block */
   PjDlUeCb   *ueCb;         /* UE Control Block */
   PjUeKey  key;              /* Key for UE Hash List */
   PjTxEnt *txEnt=NULLP;
   key.ueId = ueId;
   key.cellId = cellId;
   PjLibTrans  libTrans;

   struct elp_spacc_ioctl *spaccInput=NULLP;   
   PjMsDataForSPACC *node=NULLP;

   ret = cmHashListFind(&(pjCb[1]->u.dlCb->ueLstCp), (U8 *)&(key), sizeof(PjUeKey),
         PJ_DEF_SEQ_NUM, (PTR *) &ueCb);
   if (ret != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID, cellId, "UeId[%u]:cmHashListFind Failed",ueId);
      PJ_FREE_FLAT_BUF(pjCb[1],mBuf);
      RETVALUE(ret);
   }
   pjRbCb = ueCb->drbCb[rbId];
   if (pjRbCb == NULLP)
   {     
      PJ_FREE_FLAT_BUF(pjCb[1], mBuf);
      RETVALUE(RFAILED);
   }

#ifdef PTPJLIB 
   if (pjLibPktAdmCtrl(pjRbCb, NULL) != ROK)
   {
      PJ_FREE_FLAT_BUF(pjCb[1], mBuf);
      gPdcpStats.numPdcpSdusDiscarded++;
      PJ_UPD_L2_DLDISC_STS(pjCb[1], pjRbCb);

      numPdcpSdusDiscarded++;
      RETVALUE(RFAILED);
   }
#endif
#if 1 
   if((ueCb->libInfo.state != PJ_STATE_NORMAL) && (pjRbCb->rbType != PJ_SRB))
   {
      if(pjRbCb->dlCb.dlPktQ.count < 500)
      {
         /*         MSPD_DBG("
          *         Queuing
          *         the
          *         Packets
          *         in
          *         Reestablish/Ho
          *         state
          *         \n");*/
         pjDlmEnqueueDlPktFB(pjCb[1], pjRbCb, PJ_DATA_NRM_PKT, NULL, sduId, mBuf); 
         RETVALUE(ROK);
      }
      else
      {
         PJ_FREE_FLAT_BUF(pjCb[1], mBuf);
         gPdcpStats.numPdcpSdusDiscarded++;
         PJ_UPD_L2_DLDISC_STS(pjCb[1], pjRbCb);
         RETVALUE(RFAILED);
      }
   }
#endif
   PJ_ALLOC(pjCb[1],txEnt, sizeof(PjTxEnt));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (txEnt == NULLP)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      PJ_FREE_FLAT_BUF(gCb,sdu);
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_RES */
   PJ_DLM_UPD_VAR(pjRbCb,  txEnt->sn, txEnt->txHfn);      
   PJ_UPD_COUNT(pjRbCb,  txEnt->sn, txEnt->txHfn,  pjRbCb->dlCb.count);
   txEnt->count =  pjRbCb->dlCb.count;
   txEnt->state = PJ_SDU_RECEIVED;
   txEnt->sduId = sduId;
   txEnt->fb   = *mBuf;
   txEnt->pdu   = NULLP;
   txEnt->sdu   = NULLP;
   pjDbmInsTxEnt(pjCb[1], &(pjRbCb->dlCb.txBuf), txEnt);
#ifdef MSPD
   {
      extern U32 dlrate_pju;
      dlrate_pju += mBuf->len;
   }
#endif
   txEnt->state  = PJ_WAIT_FOR_CIPHER;
   if(ueCb->secInfo.secAct && ueCb->secInfo.cipherInfo.algoType != 0)
   {
      txEnt->pdu = txEnt->sdu;
      libTrans.pdcpInstance = pjCb[1]->init.inst;
      libTrans.count = txEnt->count;
      libTrans.rbId = rbId-1;
      libTrans.dir = PJ_SEC_DIR_DL;
      libTrans.rbType = CM_LTE_DRB;
      libTrans.snLen = pjRbCb->snLen;
      libTrans.ciphAlgoType = ueCb->secInfo.cipherInfo.algoType;   
      libTrans.ueId = ueId;
      libTrans.cellId =cellId;
      libTrans.key =  ueCb->secInfo.cipherInfo.upKey;
      spaccInput = pjMsSPAccCreateCryptVecFB(&libTrans, PJ_CIPHERING, mBuf);

      if(NULLP == spaccInput)
      {
         RLOG0(L_ERROR,"CreateCryptVecFB returned NULL");
         RETVALUE(RFAILED);
      }

      /*Get
       * memory
       * for
       * the
       * queue
       * node*/
      SGetSBuf(qSPAcc.dlregion, 
            qSPAcc.dlpool, 
            (Data**)&node,
            sizeof(PjMsDataForSPACC));

      if(node == NULLP)
      {
         RLOG0(L_FATAL, "Memory Allocation failed.");
         pjMsSpaccFreeSharblBuf(spaccInput, PJ_SEC_DIR_DL, TRUE);
         RETVALUE(RFAILED);
      }

      node->spaccInput = spaccInput;
      node->pduInfo.pdcpId.ueId = ueId ;
      node->pduInfo.pdcpId.rbType = CM_LTE_DRB;
      node->pduInfo.pdcpId.cellId = cellId;
      node->pduInfo.pdcpId.rbId = rbId;
      node->pduInfo.count = txEnt->count; 
      node->pduInfo.snLen = pjRbCb->snLen;
      node->pduInfo.pdcpInstance = pjCb[1]->init.inst;
      node->ciphQLink.node = (PTR)node;
      cmLListAdd2Tail(&pjMsCiphQ, &node->ciphQLink);
   }
   else
   {
      ret = pjDlmHdlNullCiph(pjCb[1], pjRbCb, txEnt);
      txEnt->state = PJ_PDU_SUBMITTED;
      ret = pjDlmDeliverPdu(pjCb[1], pjRbCb, txEnt);
      RETVALUE(ROK);
   }
   RETVALUE(ROK);

} /* PjUiPjuDatReqFB */
#endif


#ifdef __cplusplus
}
#endif

/********************************************************************30**

         End of file:     pj_qcom_ptsec.cpp@@/main/TeNB_QCOM_REL_BR/7 - Tue Nov 18 12:17:13 2014

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


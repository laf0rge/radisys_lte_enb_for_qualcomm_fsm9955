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

     Type:     C source file

     Desc:     This file contains functions related to PAL Setup. 
               

     File:     wr_plat.c

     Sid:      fc_plat.c@@/main/TeNB_Main_BR/tenb_main_ccb/tenb_5.0_RIB/tenb_5.0_RIB_GA/7 - Mon Feb 15 12:50:57 2016

     Prg:      spoovali 

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="ENBAPP";
static int RLOG_MODULE_ID=64;
static int RLOG_FILE_ID=111;

#include "wr.h" 
#include "wr_utils.h"
#include "wr_umm.h"
#include "wr_emm.h" 
#include "wr_smm_smallcell.h"
#include "wr_umm_trans.h"
#include "wr_umm_rrc_msg.h"
#include "wr_umm_s1ap_msg.h"
#include "wr_ifm_rrc.h"
#include "wr_ifm_dam.h"
#include "wr_ifm_schd.h"
#include "wr_ifm_s1ap.h"
#include "wr_ifm_rrm.h"
#include "wr_kpi.h"
#include "wr_umm_sch_msg.h"
#include "wr_umm_meas_fun.h"
#include "wr_ifm_l1.h"
#include "wr_umm_drx.h"
#include "wr_ifm_dam.h"
#ifndef TENB_ACC
#include "hmacsha256.h"
#endif /*TENB_ACC */
#define VE_STRING_S_LEN 9

#define VE_INTG_KEY_LEN 16
#define VE_CIPH_KEY_LEN 16
#define VE_KENB_KEY_LEN 32

#define VE_CP_CIPH_KEY 0
#define VE_UP_CIPH_KEY 1
#define VE_INTG_KEY    2
PUBLIC S16 wrIfmPdcpKdfReq(CpjKdfReqInfo *kdfReqInf,U32 transId);
PUBLIC S16 wrUmmSecSndSecModeCmd(WrUmmTransCb  *transCb);
EXTERN S16 veKDF(U8 *secKey, U8 *strS, U8 sLen, U8 *derKey);
U32 wrNonrtTick;
/* This structure is used for storing the platform specific parameters */
typedef struct WrMsCellCfgReq
{
   U16           opMode;
   U32           counter;
   U32           period;
}WrMsCellCfgReq;

PUBLIC S16 wrFillCtfVndrCellCfg(WrCellCb *cellCb,CtfCfgReqInfo *cellCfgReq);
PUBLIC Void wrPlatNonrtTickHdl()
{
      wrNonrtTick++;
}
extern void PrintRLCStats();
EXTERN Void PrintEGTPStats(void);
EXTERN Void ResetEGTPStats(void); 
#ifdef TENB_AS_SECURITY
EXTERN S16 wrPlatSecExtractSecKeys2
(
 WrUmmTransCb *transCb,
 U8 keyDerType
);
EXTERN S16 wrPlatSecExtractSecKeys1
(
 WrUeCb                       *ueCb,
 U32                          transId,
 U8                           keyDerType
 );
EXTERN S16 wrPlatSecExtractSecKeys
(
 WrUeSecCfg                   *secCfg,
 TknStrBSXL                   *secKey,
 Bool                         *asKeyGenerated,
 U32                          transId
 );
EXTERN S16 wrPlatRrcReEstabExtSecKeys
(
 WrUmmTransCb *transCb,
 U8 keyDerType
 );
EXTERN S16 wrPlatS1SecExtractSecKeys
(
 WrUeSecCfg                   *secCfg,
 Bool                         *asKeyGenerated,
 U32                          transId
 );
EXTERN S16 wrPlatX2HoGetKeNBStar
(
 WrUmmTransCb                 *transCb
 );

#endif
#ifdef RGR_RRM_TICK

EXTERN U32 hoSucessHopCount;
EXTERN U32 hovrAttempted;

extern void ResetRLCStats();
extern int rlcDatCfmsSent,pdcpDatCfmsReceived;

PUBLIC Void wrPlatRgrTtiHndl(U32 tticnt)
{
   /*extern Bool g_usettitmr;*/
   extern Void mtTmrHdlrPublic(void);
   /*static U32 tticnt = 0; */

   if((tticnt % 3000) == 0)
   {
      printf("Time : %lu sec\n", (tticnt/100));
      RLOG1(L_ALWAYS, "Time : %ld sec", (tticnt/100));
   }
}
#endif

#ifndef TENB_ACC
#ifdef ANSI
PUBLIC S16 veKDF
(
 U8  *secKey,
 U8  *strS,
 U8  sLen,
 U8  *derKey
 )
#else
PUBLIC S16 veKDF(secKey, strS, sLen, derKey)
   U8  *secKey;
   U8  *strS;
   U8  sLen;
   U8  *derKey;
#endif
{
   U8              digest[512] = {'\0'};
   hmacsha256Param sp;
   TRC2(veKDF);
   hmacsha256Setup(&sp, (const byte*)secKey, (size_t)VE_KENB_KEY_LEN * 8);
   if (strS[0] == 0x13)
   {
      hmacsha256Update(&sp, (const byte*)strS, (size_t)sLen);
   }
   else
   {
         hmacsha256Update(&sp, (const byte*)strS, 7);
   }
   hmacsha256Digest(&sp, (byte*)digest);
   if(sLen != VE_STRING_S_LEN)
   {
      cmMemcpy(derKey, digest + (32 - VE_INTG_KEY_LEN), VE_INTG_KEY_LEN);
   }
   else
   {
      cmMemcpy(derKey, digest, 32);
   }
   RETVALUE(ROK);
} /* veKDF */                                     
#endif /* TENB_ACC */

#ifdef TENB_AS_SECURITY
/* ccpu00126532 */
PUBLIC S16 wrPlatSecExtractSecKeys2
(
WrUmmTransCb *transCb,
U8 keyDerType
)
{
   CpjKdfReqInfo             *kdfInfo;
   WrUeSecCfg                *secCfg;
   WrUeSecCfg                *hzsecCfg;
   WrCellCb                  *cellCb = NULLP;
   WrUeCb                    *ueCb = transCb->ueCb;

   secCfg = (WrUeSecCfg *)(transCb->u.initCtxtCb.secCfg);
   hzsecCfg = (WrUeSecCfg *)(transCb->u.initCtxtCb.hzSecCfg);

   RLOG0(L_DEBUG, "Extract Intg. and ciphering keys");

   WR_ALLOC(&kdfInfo, sizeof(CtfKdfReqInfo));
   if (kdfInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(ROK);
   }
   if(keyDerType == CTF_KDF_TYPE2)
   {
      secCfg = ueCb->secCfg;
      kdfInfo->keyDerType = CTF_KDF_TYPE2;
      WR_GET_CELLCB(cellCb, ueCb->cellId);
      if(cellCb == NULLP)
      {
          RLOG0(L_ERROR, "Invaid Cell ID");
         RETVALUE(RFAILED);
      }
      kdfInfo->u.kEnbStarInf.dlCarrFreq = cellCb->dlCarrierFreq;
      kdfInfo->u.kEnbStarInf.physCellId = cellCb->physCellId;
      cmMemcpy((U8*)kdfInfo->u.kEnbStarInf.secKey,\
            secCfg->kENB, WR_KENB_KEY_LEN);
   }
   else if(keyDerType == CTF_KDF_TYPE1)
   {
      kdfInfo->keyDerType = CTF_KDF_TYPE1;
      kdfInfo->u.kEnbInf.intgAlgoType = hzsecCfg->intgAlgo;
      kdfInfo->u.kEnbInf.ciphAlgoType = hzsecCfg->ciphAlgo;
      cmMemcpy((U8*)kdfInfo->u.kEnbInf.secKey,\
            (U8*)hzsecCfg->kENB, WR_KENB_KEY_LEN);
    }
   /*Update keNB configuration only*/

   if (wrIfmPdcpKdfReq(kdfInfo, transCb->transId) != ROK)
   {
      WR_DBGP(WR_ERROR,(WR_PRNT_BUF,"wrIfmPdcpKdfReq failed.\n"));
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

PUBLIC S16 wrPlatSecExtractSecKeys1 
(
WrUeCb                       *ueCb,
U32                          transId,
U8                           keyDerType
)
{
   CpjKdfReqInfo             *kdfInfo;
   WrUeSecCfg                *secCfg;
   WrUeSecCfg                *hzsecCfg;
   WrCellCb                  *cellCb = NULLP;

   RLOG0(L_DEBUG, "Extract Intg. and ciphering keys");

   WR_ALLOC(&kdfInfo, sizeof(CtfKdfReqInfo));
   if (kdfInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(ROK);
   }
   if(keyDerType == CTF_KDF_TYPE2)
   {
      secCfg = ueCb->secCfg;
      kdfInfo->keyDerType = CTF_KDF_TYPE2;
      WR_GET_CELLCB(cellCb, ueCb->cellId);
      if(cellCb == NULLP)
      {
         RLOG0(L_ERROR, "Invaid Cell ID");
         RETVALUE(RFAILED);
      }
      kdfInfo->u.kEnbStarInf.dlCarrFreq = cellCb->dlCarrierFreq;
      kdfInfo->u.kEnbStarInf.physCellId = cellCb->physCellId;
      cmMemcpy((U8*)kdfInfo->u.kEnbStarInf.secKey,\
            secCfg->kENB, WR_KENB_KEY_LEN);
   }
   else if(keyDerType == CTF_KDF_TYPE1)
   {
      hzsecCfg = ueCb->hzSecCfg;
      kdfInfo->keyDerType = CTF_KDF_TYPE1;
      kdfInfo->u.kEnbInf.intgAlgoType = hzsecCfg->intgAlgo;
      kdfInfo->u.kEnbInf.ciphAlgoType = hzsecCfg->ciphAlgo;
      cmMemcpy((U8*)kdfInfo->u.kEnbInf.secKey,\
            (U8*)hzsecCfg->kENB, WR_KENB_KEY_LEN);
   }
   /*Update keNB configuration only*/

   if (wrIfmPdcpKdfReq(kdfInfo, transId) != ROK)
   {
      WR_DBGP(WR_ERROR,(WR_PRNT_BUF,"wrIfmPdcpKdfReq failed.\n"));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/** @brief This function is responsible for deriving the ciphering and 
 *         integrity keys, and storing them in local data structure.
 *
 * @details
 *
 *     Function: wrUmmSecExtractSecKeys
 *
 *         Processing steps:
 *         - invoke wrUmmSecKeyDervFunc to derive the control plane
 *           ciphering key
 *         - store the derived control plane ciphering key to the local data
 *           structure
 *         - invoke wrUmmSecKeyDervFunc to derive the user plane
 *           ciphering key
 *         - store the derived user plane ciphering key to the local data
 *           structure
 *         - invoke wrUmmSecKeyDervFunc to derive the integrity key
 *         - store the derived integrity key to the local data structure
 *
 * @param[out] secCfg  : local data structure to store the security info
 * @param[in]  secKey  : KeNB
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrPlatSecExtractSecKeys
(
WrUeSecCfg                   *secCfg,
TknStrBSXL                   *secKey,
Bool                         *asKeyGenerated,
U32                          transId
)
{
   /* ccpu00126532 */
   U8                        idx = 0;
   CpjKdfReqInfo             *kdfInfo;

   RLOG0(L_DEBUG, "Extract Intg. and ciphering keys");

   /* storing the base key (KENB)                                         */
   cmMemcpy((U8*)&secCfg->kENB[0], (U8*)&secKey->val[idx], WR_KENB_KEY_LEN);
   
   if(secCfg->intgAlgo == WR_UMM_NULL_ALGO)
   {
      RLOG0(L_DEBUG, "Integrity Algo is NULL No need for KDF");
      (*asKeyGenerated) = TRUE;   
      RETVALUE(ROK);
   }

   WR_ALLOC(&kdfInfo, sizeof(CtfKdfReqInfo));
   if (kdfInfo == NULLP)
   {
      RLOG0(L_ERROR,"mem alloc failed.\n");
      RETVALUE(RFAILED);
   }

   kdfInfo->keyDerType = CTF_KDF_TYPE1;
   kdfInfo->u.kEnbInf.ciphAlgoType = secCfg->ciphAlgo;
   kdfInfo->u.kEnbInf.intgAlgoType = secCfg->intgAlgo;
   cmMemcpy((U8*)kdfInfo->u.kEnbInf.secKey, (U8*)secCfg->kENB, WR_KENB_KEY_LEN);

   /* Update keNB configuration only                                      */
   if (wrIfmPdcpKdfReq(kdfInfo, transId) != ROK)
   {
      WR_DBGP(WR_ERROR,(WR_PRNT_BUF,"wrIfmPdcpKdfReq failed.\n"));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}
PUBLIC S16 wrPlatRrcReEstabExtSecKeys
(
WrUmmTransCb *transCb,
U8 keyDerType
)
{
   CpjKdfReqInfo             *kdfInfo;
   WrCellCb                  *cellCb = NULLP;
   WrUeCb                    *ueCb = transCb->ueCb;


   RLOG0(L_DEBUG, " Extract Intg. and ciphering keys ");

   if(ueCb->secCfg->intgAlgo == WR_UMM_NULL_ALGO)
   {
      RLOG0(L_INFO, " Integrity Algo is NULL No need for KDF ");
      RETVALUE(ROK);
   }
   
    WR_ALLOC(&kdfInfo, sizeof(CtfKdfReqInfo));
    if (kdfInfo == NULLP)
    {
       RLOG0(L_ERROR," mem alloc failed.");
       RETVALUE(ROK);
    }
    if(keyDerType == CTF_KDF_TYPE2)
    {
       kdfInfo->keyDerType = CTF_KDF_TYPE2;
       WR_GET_CELLCB(cellCb, transCb->ueCb->cellId);
       if(cellCb == NULLP)
       {
          RLOG0(L_ERROR," Invaid Cell ID");
          RETVALUE(RFAILED);
       }

       kdfInfo->u.kEnbStarInf.dlCarrFreq = cellCb->dlCarrierFreq;
       kdfInfo->u.kEnbStarInf.physCellId = cellCb->physCellId;
       cmMemcpy((U8*)kdfInfo->u.kEnbStarInf.secKey,\
                       ueCb->hzSecCfg->kENB, WR_KENB_KEY_LEN);
   }
   else if(keyDerType == CTF_KDF_TYPE1)
   {
       kdfInfo->keyDerType = CTF_KDF_TYPE1;
       kdfInfo->u.kEnbInf.intgAlgoType = ueCb->secCfg->intgAlgo;
       kdfInfo->u.kEnbInf.ciphAlgoType = ueCb->secCfg->ciphAlgo;
       cmMemcpy((U8*)kdfInfo->u.kEnbInf.secKey,\
                       (U8*)ueCb->hzSecCfg->kENB, WR_KENB_KEY_LEN);
   }
   /*Update keNB configuration only*/

   if (wrIfmPdcpKdfReq(kdfInfo, transCb->transId) != ROK)
   {
      RLOG0(L_ERROR,"wrIfmPdcpKdfReq failed.");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

PUBLIC S16 wrPlatS1SecExtractSecKeys
(
WrUeSecCfg                   *secCfg,
Bool                         *asKeyGenerated,
U32                          transId
)
{
   CpjKdfReqInfo             *kdfInfo;

   RLOG0(L_DEBUG, "Extract Intg. and ciphering keys");

   if(secCfg->intgAlgo == WR_UMM_NULL_ALGO)
   {
      RLOG0(L_DEBUG, "Integrity Algo is NULL No need for KDF");
      (*asKeyGenerated) = TRUE;   
      RETVALUE(ROK);
   }

   WR_ALLOC(&kdfInfo, sizeof(CtfKdfReqInfo));
   if (kdfInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   kdfInfo->keyDerType = CTF_KDF_TYPE1;
   kdfInfo->u.kEnbInf.ciphAlgoType = secCfg->ciphAlgo;
   kdfInfo->u.kEnbInf.intgAlgoType = secCfg->intgAlgo;
   cmMemcpy((U8*)kdfInfo->u.kEnbInf.secKey, (U8*)secCfg->kENB, WR_KENB_KEY_LEN);

   /* Update keNB configuration only                                      */
   if (wrIfmPdcpKdfReq(kdfInfo, transId) != ROK)
   {
      RLOG0(L_ERROR, "wrIfmPdcpKdfReq failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

PUBLIC S16 wrUmmSecExtractSecKeys3
(
WrUeSecCfg                   *secCfg,
TknStrBSXL                   *secKey,
Bool                         *asKeyGenerated,
WrUmmTransCb                 *transCb
)
{
   U8                        idx = 0;
   /*CtfKdfReqInfo             *kdfInfo;*/

   RLOG0(L_INFO, "Extract Intg. and ciphering keys");
   cmMemcpy((U8*)&secCfg->kENB[0], (U8*)&secKey->val[idx], WR_KENB_KEY_LEN);
   //for(idx = 0; idx < WR_KENB_KEY_LEN; idx++)
   //{
     // printf("%x ",secCfg->kENB[idx]);
   //}
   if(secCfg->intgAlgo == WR_UMM_NULL_ALGO)
   {
      RLOG0(L_INFO, "Integrity Algo is NULL No need for KDF");
      (*asKeyGenerated) = TRUE;   
      RETVALUE(ROK);
   }
#if 0
#else
   {
      {
         WrInitCntxtSetupTransCb   *initCtxtCb = &(transCb->u.initCtxtCb);
         U8  stringS[VE_STRING_S_LEN];
         U8  dervdKey[VE_CIPH_KEY_LEN];
         U8  type = 0;

         cmMemset((U8 *)&stringS[0], '\0', VE_STRING_S_LEN);
         stringS[0] = 0x15;
         stringS[2] = 0x00;
         stringS[3] = 0x01;
         stringS[5] = 0x00;
         stringS[6] = 0x01;
         for(type = 0; type <= VE_INTG_KEY; type++)
         {
            cmMemset((U8 *)&dervdKey[0], '\0', VE_CIPH_KEY_LEN);
            switch(type)
            {
               case VE_CP_CIPH_KEY:
                  stringS[1] = 0x03;
                  stringS[4] = (U8)secCfg->ciphAlgo;
                  veKDF((U8 *)secKey->val, (U8 *)stringS, (VE_STRING_S_LEN - 2), (U8 *)dervdKey);
                  //printf("\n CP CIPH KEY:");
                  for (idx = 0; idx < VE_CIPH_KEY_LEN; idx++)
                  {
                     initCtxtCb->secCfg->cpCiphKey[idx] = dervdKey[idx];
                     //printf(" %x", dervdKey[idx]);
                  }
                  break;
               case VE_UP_CIPH_KEY:
                  stringS[1] = 0x05;
                  stringS[4] = (U8)secCfg->ciphAlgo;
                  veKDF((U8 *)secKey->val, (U8 *)stringS, (VE_STRING_S_LEN - 2), (U8 *)dervdKey);
                  //printf("\n UP_CIPH_KEY:");
                  for (idx = 0; idx < VE_CIPH_KEY_LEN; idx++)
                  {
                     initCtxtCb->secCfg->upCiphKey[idx] = dervdKey[idx];
                     //printf(" %x", dervdKey[idx]);
                  }
                  break;
               case VE_INTG_KEY:
                  stringS[1] = 0x04;
                  stringS[4] = (U8)secCfg->intgAlgo;
      /*   printf("\n Kenb: ");
                  for (idx = 0; idx < 32; idx++)
                  {
                     printf(" %x",secKey->val[idx]);
                  }*/
                  veKDF((U8 *)secKey->val, (U8 *)stringS, (VE_STRING_S_LEN - 2), (U8 *)dervdKey);
                  //printf("\n INTG KEY:");
                  for (idx = 0; idx < VE_INTG_KEY_LEN; idx++)
                  {
                     initCtxtCb->secCfg->intgKey[idx] = dervdKey[idx];
                     //printf(" %x", dervdKey[idx]);
                  }
                     //printf("\n");
                  break;
               default:
                  RETVALUE(RFAILED); 
                  break;
            } /* switch */
         } /* for loop */
         initCtxtCb->asKeyGenerated = TRUE;
         if(initCtxtCb->state == WR_UMM_INIT_CTXT_UECAP_DONE)
         {
            wrUmmSecSndSecModeCmd(transCb);
            initCtxtCb->asSecModeInit = TRUE;
         }
      }
   }                                     
#endif
   RETVALUE(ROK);
}

PUBLIC S16 wrUmmSecExtractHzSecKeys3
(
WrInitCntxtSetupTransCb *initCtxtCb,
WrCellId                  cellId,
U8                      kdfType
)
{
   /*U8                        idx = 0;*/
   WrUeSecCfg                *secCfg; /* Security related info */
   U8  stringS[VE_STRING_S_LEN];
   U8  dervdKey[VE_CIPH_KEY_LEN];
   U8  dervdKENBKey[VE_KENB_KEY_LEN];
   U8  type = 0;
   /*U8 i=0;*/
   RLOG0(L_DEBUG, "Extract Intg. and ciphering keys");

   if(initCtxtCb->secCfg->intgAlgo == WR_UMM_NULL_ALGO)
   {
      RLOG0(L_DEBUG, "Integrity Algo is NULL NO Need for KDF");
      RETVALUE(ROK);
   }
   //printf("InitCtxt: kENB\n");
   //for (i = 0; i < 32;i++)
   //printf("%x ",initCtxtCb->secCfg->kENB[i]);

   //printf("\n");
   switch (kdfType)
   { 
     case CTF_KDF_TYPE3:
     {
        secCfg = initCtxtCb->hzSecCfg;

        cmMemset((U8 *)&stringS[0], '\0', VE_STRING_S_LEN);
        stringS[0] = 0x15;
        stringS[2] = 0x00;
        stringS[3] = 0x01;
        stringS[5] = 0x00;
        stringS[6] = 0x01;
        for(type = 0; type <= VE_INTG_KEY; type++)
        {
           cmMemset((U8 *)&dervdKey[0], '\0', VE_CIPH_KEY_LEN);
           switch(type)
           {
              case VE_CP_CIPH_KEY:
                 secCfg->ciphAlgo = initCtxtCb->secCfg->ciphAlgo;
                 stringS[1] = 0x03;
                 stringS[4] = (U8)secCfg->ciphAlgo;
                 veKDF((U8 *)secCfg->kENB, (U8 *)stringS, (VE_STRING_S_LEN - 2), (U8 *)dervdKey);
                 cmMemcpy((U8 *)secCfg->cpCiphKey,(U8 *)dervdKey,VE_CIPH_KEY_LEN);
                 /*printf("\n CP CIPH KEY:");
                 for (idx = 0; idx < VE_CIPH_KEY_LEN; idx++)
                 {
                    printf(" %x", dervdKey[idx]);
                 } */
                 break;
              case VE_UP_CIPH_KEY:
                 secCfg->ciphAlgo = initCtxtCb->secCfg->ciphAlgo;
                 stringS[1] = 0x05;
                 stringS[4] = (U8)secCfg->ciphAlgo;
                 veKDF((U8 *)secCfg->kENB, (U8 *)stringS, (VE_STRING_S_LEN - 2), (U8 *)dervdKey);
                 cmMemcpy((U8 *)secCfg->upCiphKey,(U8 *)dervdKey,VE_CIPH_KEY_LEN);
                 /*printf("\n UP_CIPH_KEY:");
                 for (idx = 0; idx < VE_CIPH_KEY_LEN; idx++)
                 {
                    printf(" %x", dervdKey[idx]);
                 } */
                 break;
              case VE_INTG_KEY:
                 secCfg->intgAlgo = initCtxtCb->secCfg->intgAlgo;
                 stringS[1] = 0x04;
                 stringS[4] = (U8)secCfg->intgAlgo;
                 veKDF((U8 *)secCfg->kENB, (U8 *)stringS, (VE_STRING_S_LEN - 2), (U8 *)dervdKey);
                 cmMemcpy((U8 *)secCfg->intgKey,(U8 *)dervdKey,VE_INTG_KEY_LEN);
                 /*printf("\n INTG KEY:");
                 for (idx = 0; idx < VE_INTG_KEY_LEN; idx++)
                 {
                    printf(" %x", dervdKey[idx]);
                 } */
                 // printf("\n");
                 break;
              default:
                 RETVALUE(RFAILED); 
                 break;
           } /* switch */
        } /* for loop */

   break;
      } 
     case CTF_KDF_TYPE2:
     {
        WrCellCb   *cellCb = NULLP;

       WR_GET_CELLCB(cellCb, cellId);
       if(cellCb == NULLP)
       {
          RLOG0(L_ERROR, "Invalid Cell Id");
          RETVALUE(RFAILED);
       }
       cmMemset((U8 *)&stringS[0], '\0', VE_STRING_S_LEN);

       stringS[0] = 0x13;
       stringS[1] = (cellCb->physCellId & 0xff00) >> 8;
       stringS[2] = cellCb->physCellId & 0x00ff; /*TODO: PCI length*/
       stringS[3] = 0x00;
       stringS[4] = 0x02; /*TODO: PCI length*/
       stringS[5] = (cellCb->dlCarrierFreq & 0xff00) >> 8;
       stringS[6] = cellCb->dlCarrierFreq & 0x00ff;
       stringS[7] = 0x00;
       stringS[8] = 0x02;/*TODO: dlCarrierFreq length */

       //printf("PCI HI:%x\n",(U16)stringS[1]);
       //printf("PCI LO:%x\n",(U16)stringS[2]);
       //printf("DL freq HI:%x\n",(U16)stringS[5]);
       //printf("DL freq LO:%x\n",(U16)stringS[6]);
       //printf("PCI:%x,dlFreq:%x\n",cellCb->physCellId,cellCb->dlCarrierFreq);
       cmMemset((U8 *)&dervdKENBKey[0], '\0', VE_KENB_KEY_LEN);
       secCfg = initCtxtCb->hzSecCfg;

        veKDF((U8 *) initCtxtCb->secCfg->kENB, (U8 *)stringS, (VE_STRING_S_LEN), (U8 *)dervdKENBKey);
        cmMemcpy((U8 *)secCfg->kENB,(U8 *)dervdKENBKey,VE_KENB_KEY_LEN);

   //printf("InitCtxt: kENB*\n");
   //for (i = 0; i < 32;i++)
   //printf("%x ",initCtxtCb->hzSecCfg->kENB[i]);

   break;
     }
   }  

   RETVALUE(ROK);
}

/* ccpu00126532 */
PUBLIC S16 wrUmmSecExtractSecKeys1 
(
WrUeCb                       *ueCb,
U32                          transId,
U8                           keyDerType
)
{
   CtfKdfReqInfo             *kdfInfo;
   WrUeSecCfg                *secCfg;
   WrUeSecCfg                *hzsecCfg;
   WrCellCb                  *cellCb = NULLP;

   RLOG0(L_DEBUG, "Extract Intg. and ciphering keys");

   WR_ALLOC(&kdfInfo, sizeof(CtfKdfReqInfo));
   if (kdfInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(ROK);
   }
   if(keyDerType == CTF_KDF_TYPE2)
   {
      secCfg = ueCb->secCfg;
      kdfInfo->keyDerType = CTF_KDF_TYPE2;
      WR_GET_CELLCB(cellCb, ueCb->cellId);
      if(cellCb == NULLP)
      {
         RLOG0(L_ERROR, "Invaid Cell ID");
         RETVALUE(RFAILED);
      }
      kdfInfo->u.kEnbStarInf.dlCarrFreq = cellCb->dlCarrierFreq;
      kdfInfo->u.kEnbStarInf.physCellId = cellCb->physCellId;
      cmMemcpy((U8*)kdfInfo->u.kEnbStarInf.secKey,\
            secCfg->kENB, WR_KENB_KEY_LEN);
   }
   else if(keyDerType == CTF_KDF_TYPE1)
   {
      hzsecCfg = ueCb->hzSecCfg;
      kdfInfo->keyDerType = CTF_KDF_TYPE1;
      kdfInfo->u.kEnbInf.intgAlgoType = hzsecCfg->intgAlgo;
      kdfInfo->u.kEnbInf.ciphAlgoType = hzsecCfg->ciphAlgo;
      cmMemcpy((U8*)kdfInfo->u.kEnbInf.secKey,\
            (U8*)hzsecCfg->kENB, WR_KENB_KEY_LEN);
   }

   RETVALUE(ROK);
}

/** @brief This function is responsible for deriving the ciphering and 
 *         integrity keys, and storing them in local data structure.
 *
 * @details
 *
 *     Function: wrUmmSecExtractSecKeys
 *
 *         Processing steps:
 *         - invoke wrUmmSecKeyDervFunc to derive the control plane
 *           ciphering key
 *         - store the derived control plane ciphering key to the local data
 *           structure
 *         - invoke wrUmmSecKeyDervFunc to derive the user plane
 *           ciphering key
 *         - store the derived user plane ciphering key to the local data
 *           structure
 *         - invoke wrUmmSecKeyDervFunc to derive the integrity key
 *         - store the derived integrity key to the local data structure
 *
 * @param[out] secCfg  : local data structure to store the security info
 * @param[in]  secKey  : KeNB
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
#if 0
PUBLIC S16 wrUmmSecExtractSecKeys
(
WrUeSecCfg                   *secCfg,
TknStrBSXL                   *secKey,
Bool                         *asKeyGenerated,
U32                          transId
)
{
   /* ccpu00126532 */
   U8                        idx = 0;
   CtfKdfReqInfo             *kdfInfo;

   RLOG0(L_DEBUG, "Extract Intg. and ciphering keys");

   /* storing the base key (KENB)                                         */
   cmMemcpy((U8*)&secCfg->kENB[0], (U8*)&secKey->val[idx], WR_KENB_KEY_LEN);
//   for(idx = 0; idx < WR_KENB_KEY_LEN; idx++)
//   {
      //printf("%x ",secCfg->kENB[idx]);
//   }

   if(secCfg->intgAlgo == WR_UMM_NULL_ALGO)
   {
      RLOG0(L_ERROR, "Integrity Algo is NULL No need for KDF");
      (*asKeyGenerated) = TRUE;   
      RETVALUE(ROK);
   }
   WR_ALLOC(&kdfInfo, sizeof(CtfKdfReqInfo));
   if (kdfInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }   
   kdfInfo->keyDerType = CTF_KDF_TYPE1;
   kdfInfo->u.kEnbInf.ciphAlgoType = secCfg->ciphAlgo;
   kdfInfo->u.kEnbInf.intgAlgoType = secCfg->intgAlgo;

   if (wrIfmPhyKdfReq(kdfInfo, transId) != ROK)
   {
      RLOG0(L_ERROR, "WrSndCtfUeCfg failed.");
      RETVALUE(RFAILED);

   }  

   RETVALUE(ROK);
}
#endif


PUBLIC S16 wrUmmRrcReEstabExtSecKeys
(
WrUmmTransCb *transCb,
U8 keyDerType
)
{
   WrUeCb                    *ueCb = transCb->ueCb;

   U8  stringS[VE_STRING_S_LEN];
   U8  dervdKey[VE_CIPH_KEY_LEN];
   U8  dervdKENBKey[VE_KENB_KEY_LEN];
   U8  type = 0;
   /*U8 i=0;
   U8 idx=0;*/
   RLOG0(L_DEBUG, "Extract Intg. and ciphering keys");

   if(ueCb->secCfg->intgAlgo == WR_UMM_NULL_ALGO)
   {
      RLOG0(L_DEBUG, "Integrity Algo is NULL No Need for KDF");
      RETVALUE(ROK);
   }
   /*
   printf("Re-establishment: kENB\n");
   for (i = 0; i < 32;i++)
   printf("%x ",ueCb->secCfg->kENB[i]);

   printf("\n");
   */
   if (CTF_KDF_TYPE3 == keyDerType) 
   { 
       /* up/cp keys for ciphering and integrity key*/ 
        cmMemset((U8 *)&stringS[0], '\0', VE_STRING_S_LEN);
        stringS[0] = 0x15;
        stringS[2] = 0x00;
        stringS[3] = 0x01;
        stringS[5] = 0x00;
        stringS[6] = 0x01;
        for(type = 0; type <= VE_INTG_KEY; type++)
        {
           cmMemset((U8 *)&dervdKey[0], '\0', VE_CIPH_KEY_LEN);
           switch(type)
           {
              case VE_CP_CIPH_KEY:
                 ueCb->hzSecCfg->ciphAlgo = (U8)ueCb->secCfg->ciphAlgo;
                 stringS[1] = 0x03;
                 stringS[4] = (U8)ueCb->secCfg->ciphAlgo;
                 veKDF((U8 *)ueCb->hzSecCfg->kENB, (U8 *)stringS, (VE_STRING_S_LEN - 2), (U8 *)dervdKey);
                 cmMemcpy((U8 *)ueCb->hzSecCfg->cpCiphKey,(U8 *)dervdKey,VE_CIPH_KEY_LEN);
                 /*printf("\n CP CIPH KEY:");
                 for (idx = 0; idx < VE_CIPH_KEY_LEN; idx++)
                 {
                    printf(" %x", dervdKey[idx]);
                 }*/
                 break;
              case VE_UP_CIPH_KEY:
                 ueCb->hzSecCfg->ciphAlgo = (U8)ueCb->secCfg->ciphAlgo;
                 stringS[1] = 0x05;
                 stringS[4] = (U8)ueCb->secCfg->ciphAlgo;
                 veKDF((U8 *)ueCb->hzSecCfg->kENB, (U8 *)stringS, (VE_STRING_S_LEN - 2), (U8 *)dervdKey);
                 cmMemcpy((U8 *)ueCb->hzSecCfg->upCiphKey,(U8 *)dervdKey,VE_CIPH_KEY_LEN);
                 /*printf("\n UP_CIPH_KEY:");
                 for (idx = 0; idx < VE_CIPH_KEY_LEN; idx++)
                 {
                    printf(" %x", dervdKey[idx]);
                 }*/
                 break;
              case VE_INTG_KEY:
                 ueCb->hzSecCfg->intgAlgo = ueCb->secCfg->intgAlgo;
                 stringS[1] = 0x04;
                 stringS[4] = (U8)ueCb->secCfg->intgAlgo;
                 veKDF((U8 *)ueCb->hzSecCfg->kENB, (U8 *)stringS, (VE_STRING_S_LEN - 2), (U8 *)dervdKey);
                 cmMemcpy((U8 *)ueCb->hzSecCfg->intgKey,(U8 *)dervdKey,VE_INTG_KEY_LEN);
                 /*printf("\n INTG KEY:");
                 for (idx = 0; idx < VE_INTG_KEY_LEN; idx++)
                 {
                    printf(" %x", dervdKey[idx]);
                 } 
       printf("\n"); */
                 break;
              default:
                 RETVALUE(RFAILED); 
                 break;
           } /* switch */
        } /* for loop */
     } /* if loop for up/cp cipher & integ key derviation */ 
     else if(CTF_KDF_TYPE2 == keyDerType)
     {
        /* Horizontal key derivation for KENB* */
        WrCellCb   *cellCb = NULLP;
        WR_GET_CELLCB(cellCb, ueCb->cellId);
       if(cellCb == NULLP)
       {
          RLOG0(L_ERROR," Invalid Cell ID\n");
          RETVALUE(RFAILED);
       }
       cmMemset((U8 *)&stringS[0], '\0', VE_STRING_S_LEN);

       stringS[0] = 0x13;
       stringS[1] = (cellCb->physCellId & 0xff00) >> 8;
       stringS[2] = cellCb->physCellId & 0x00ff; /*TODO: PCI length*/
       stringS[3] = 0x00;
       stringS[4] = 0x02; /*TODO: PCI length*/
       stringS[5] = (cellCb->dlCarrierFreq & 0xff00) >> 8;
       stringS[6] = cellCb->dlCarrierFreq & 0x00ff;
       stringS[7] = 0x00;
       stringS[8] = 0x02;/*TODO: dlCarrierFreq length */

     /*  printf("PCI HI:%x\n",(U16)stringS[1]);
       printf("PCI LO:%x\n",(U16)stringS[2]);
       printf("DL freq HI:%x\n",(U16)stringS[5]);
       printf("DL freq LO:%x\n",(U16)stringS[6]);
       printf("PCI:%x,dlFreq:%x\n",cellCb->physCellId,cellCb->dlCarrierFreq);*/
       cmMemset((U8 *)&dervdKENBKey[0], '\0', VE_CIPH_KEY_LEN);

        veKDF((U8 *)ueCb->secCfg->kENB, (U8 *)stringS, (VE_STRING_S_LEN), (U8 *)dervdKENBKey);
        cmMemcpy((U8 *)ueCb->hzSecCfg->kENB,(U8 *)dervdKENBKey,VE_KENB_KEY_LEN);
/*
   printf("Re-establishment: kENB*\n");
   for (i = 0; i < 32;i++)
   printf("%x ",ueCb->hzSecCfg->kENB[i]);
*/
     }
   RETVALUE(ROK);
}

PUBLIC S16 wrUmmS1SecExtractSecKeys
(
WrUeSecCfg                   *secCfg,
Bool                         *asKeyGenerated,
U32                          transId
)
{
   CtfKdfReqInfo             *kdfInfo;

   RLOG0(L_DEBUG, "Extract Intg. and ciphering keys");

   if(secCfg->intgAlgo == WR_UMM_NULL_ALGO)
   {
      RLOG0(L_DEBUG, "Integrity Algo is NULL No need for KDF");
      (*asKeyGenerated) = TRUE;   
      RETVALUE(ROK);
   }

   WR_ALLOC(&kdfInfo, sizeof(CtfKdfReqInfo));
   if (kdfInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   kdfInfo->keyDerType = CTF_KDF_TYPE1;
   kdfInfo->u.kEnbInf.ciphAlgoType = secCfg->ciphAlgo;
   kdfInfo->u.kEnbInf.intgAlgoType = secCfg->intgAlgo;
   cmMemcpy((U8*)kdfInfo->u.kEnbInf.secKey, (U8*)secCfg->kENB, WR_KENB_KEY_LEN);

   /* Update keNB configuration only                                      */
   if (wrIfmPhyKdfReq(kdfInfo, transId) != ROK)
   {
      RLOG0(L_ERROR, "wrIfmPhyKdfReq failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

PUBLIC S8 wrUmmSecExtractHOSecKeys
(
WrUmmS1TgtHoTrans *s1TgtHoTrans,
WrCellId          cellId,
U8                kdfType
)
{
   /*U8                        idx = 0;*/
   WrUeSecCfg                *secCfg; /* Security related info */
   U8  stringS[VE_STRING_S_LEN];
   U8  dervdKey[VE_CIPH_KEY_LEN];
   U8  dervdKENBKey[VE_KENB_KEY_LEN];
   U8  type = 0;
   /*U8 i=0;*/

   RLOG0(L_INFO, "Extract Intg. and ciphering keys");

   if (CTF_KDF_TYPE4 == kdfType)
       secCfg = s1TgtHoTrans->hzSecCfg;
   else
   secCfg = s1TgtHoTrans->secCfg;
   if(secCfg->intgAlgo == WR_UMM_NULL_ALGO)
   {
      RLOG0(L_INFO, "Integrity Algo is NULL No need for KDF");
      s1TgtHoTrans->asKeyGenerated = TRUE;
      RETVALUE(ROK);
   }
   switch (kdfType)
   { 
     case CTF_KDF_TYPE3:
     case CTF_KDF_TYPE4:
     {
        cmMemset((U8 *)&stringS[0], '\0', VE_STRING_S_LEN);
        stringS[0] = 0x15;
        stringS[2] = 0x00;
        stringS[3] = 0x01;
        stringS[5] = 0x00;
        stringS[6] = 0x01;
        for(type = 0; type <= VE_INTG_KEY; type++)
        {
           cmMemset((U8 *)&dervdKey[0], '\0', VE_CIPH_KEY_LEN);
           switch(type)
           {
              case VE_CP_CIPH_KEY:
                 stringS[1] = 0x03;
                 stringS[4] = (U8)secCfg->ciphAlgo;
                 veKDF((U8 *)secCfg->kENB, (U8 *)stringS, (VE_STRING_S_LEN - 2), (U8 *)dervdKey);
                 cmMemcpy((U8 *)secCfg->cpCiphKey,(U8 *)dervdKey,VE_CIPH_KEY_LEN);
                 /*printf("\n CP CIPH KEY:");
                 for (idx = 0; idx < VE_CIPH_KEY_LEN; idx++)
                 {
                    printf(" %x", dervdKey[idx]);
                 } */
                 break;
              case VE_UP_CIPH_KEY:
                 stringS[1] = 0x05;
                 stringS[4] = (U8)secCfg->ciphAlgo;
                 veKDF((U8 *)secCfg->kENB, (U8 *)stringS, (VE_STRING_S_LEN - 2), (U8 *)dervdKey);
                 cmMemcpy((U8 *)secCfg->upCiphKey,(U8 *)dervdKey,VE_CIPH_KEY_LEN);
                 /*printf("\n UP_CIPH_KEY:");
                 for (idx = 0; idx < VE_CIPH_KEY_LEN; idx++)
                 {
                    printf(" %x", dervdKey[idx]);
                 } */
                 break;
              case VE_INTG_KEY:
                 stringS[1] = 0x04;
                 stringS[4] = (U8)secCfg->intgAlgo;
                 veKDF((U8 *)secCfg->kENB, (U8 *)stringS, (VE_STRING_S_LEN - 2), (U8 *)dervdKey);
                 cmMemcpy((U8 *)secCfg->intgKey,(U8 *)dervdKey,VE_INTG_KEY_LEN);
                 break;
              default:
                 RETVALUE(RFAILED); 
                 break;
           } /* switch */
        } /* for loop */
        s1TgtHoTrans->asKeyGenerated = TRUE;

   break;
      } 
     case CTF_KDF_TYPE5:
     case CTF_KDF_TYPE2:
     {
        WrCellCb   *cellCb = NULLP;

       WR_GET_CELLCB(cellCb, cellId);
       if(cellCb == NULLP)
       {
          RLOG0(L_ERROR, " Invaid Cell ID");
          RETVALUE(RFAILED);
       }
       cmMemset((U8 *)&stringS[0], '\0', VE_STRING_S_LEN);

       stringS[0] = 0x13;
       stringS[1] = (cellCb->physCellId & 0xff00) >> 8;
       stringS[2] = cellCb->physCellId & 0x00ff; 
       stringS[3] = 0x00;
       stringS[4] = 0x02; /*PCI length*/
       stringS[5] = (cellCb->dlCarrierFreq & 0xff00) >> 8;
       stringS[6] = cellCb->dlCarrierFreq & 0x00ff;
       stringS[7] = 0x00;
       stringS[8] = 0x02;/* dlCarrierFreq length */
       cmMemset((U8 *)&dervdKENBKey[0], '\0', VE_KENB_KEY_LEN);

        veKDF((U8 *) secCfg->kENB, (U8 *)stringS, (VE_STRING_S_LEN), (U8 *)dervdKENBKey);
        if (kdfType == CTF_KDF_TYPE5)
            cmMemcpy((U8*)s1TgtHoTrans->hzSecCfg->kENB,(U8 *)dervdKENBKey,VE_KENB_KEY_LEN);
        else
        {
            cmMemcpy((U8*)secCfg->kENB,(U8 *)dervdKENBKey,VE_KENB_KEY_LEN);
            WR_ALLOC(&(s1TgtHoTrans->hzSecCfg), sizeof(WrUeSecCfg));
            if (s1TgtHoTrans->hzSecCfg == NULLP)
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(RFAILED);
            }

            /* Copy the security capabilities and the selected algorithm */
            s1TgtHoTrans->hzSecCfg->ciphAlgo = s1TgtHoTrans->secCfg->ciphAlgo;
            s1TgtHoTrans->hzSecCfg->intgAlgo = s1TgtHoTrans->secCfg->intgAlgo;
            s1TgtHoTrans->hzSecCfg->encrypCap = s1TgtHoTrans->secCfg->encrypCap;
            s1TgtHoTrans->hzSecCfg->integCap = s1TgtHoTrans->secCfg->integCap;
        }

   break;
     }
   }  

   RETVALUE(ROK);

}

PUBLIC S16 wrUmmSecExtractX2HOSecKeys3
(
WrUmmTransCb                 *transCb,
TknStrBSXL                   *secKey,
Bool                         *asKeyGenerated,
U8                           kdfType
)
{
   /*U8                        idx = 0;
   CtfKdfReqInfo             *kdfInfo;*/
   U8  type = 0;
   WrUmmX2TgtHoTrans  *x2TgtHoTrans = &(transCb->u.x2TgtHoTrans);
   WrUeSecCfg *secCfg = NULLP;
   WrCellCb *cellCb;
   U8  stringS[VE_STRING_S_LEN];
   U8  dervdKey[VE_CIPH_KEY_LEN];
   U8  dervdKENBKey[VE_KENB_KEY_LEN];

   if (kdfType == CTF_KDF_TYPE2)
   {  
      secCfg = x2TgtHoTrans->secCfg;
   }
   else if ((kdfType == CTF_KDF_TYPE3) || (kdfType == CTF_KDF_TYPE4))
   {
     secCfg = x2TgtHoTrans->hzSecCfg;
   }
   if(NULLP == secCfg)
   {
       RLOG0(L_ERROR, "SecCfg is NULL");
       RETVALUE(RFAILED);
   }
   if(secCfg->intgAlgo == WR_UMM_NULL_ALGO)
   {
      (*asKeyGenerated) = TRUE;   
      RETVALUE(ROK);
   }
   else
   {
      RLOG_ARG1(L_INFO,DBG_CRNTI,transCb->ueCb->crnti," Integrity Algo %d ",
            secCfg->intgAlgo);
   }
   switch (kdfType)
   {
     case CTF_KDF_TYPE2:
     case CTF_KDF_TYPE3:

       cmMemcpy((U8*)&secCfg->kENB[0], (U8*)&secKey->val[0], WR_KENB_KEY_LEN);
       cmMemset((U8 *)&stringS[0], '\0', VE_STRING_S_LEN);
       stringS[0] = 0x15;
       stringS[2] = 0x00;
       stringS[3] = 0x01;
       stringS[5] = 0x00;
       stringS[6] = 0x01;
       for(type = 0; type <= VE_INTG_KEY; type++)
       {
          cmMemset((U8 *)&dervdKey[0], '\0', VE_CIPH_KEY_LEN);
          switch(type)
          {
             case VE_CP_CIPH_KEY:
                stringS[1] = 0x03;
                stringS[4] = (U8)secCfg->ciphAlgo;
                veKDF((U8 *)secCfg->kENB, (U8 *)stringS, (VE_STRING_S_LEN - 2), (U8 *)dervdKey);
           cmMemcpy((U8 *)secCfg->cpCiphKey,(U8 *)dervdKey,VE_CIPH_KEY_LEN);
                break;
             case VE_UP_CIPH_KEY:
                stringS[1] = 0x05;
                stringS[4] = (U8)secCfg->ciphAlgo;
                veKDF((U8 *)secKey->val, (U8 *)stringS, (VE_STRING_S_LEN - 2), (U8 *)dervdKey);
           cmMemcpy((U8 *)secCfg->upCiphKey,(U8 *)dervdKey,VE_CIPH_KEY_LEN);
                break;
             case VE_INTG_KEY:
                stringS[1] = 0x04;
                stringS[4] = (U8)secCfg->intgAlgo;
                veKDF((U8 *)secKey->val, (U8 *)stringS, (VE_STRING_S_LEN - 2), (U8 *)dervdKey);
           cmMemcpy((U8 *)secCfg->intgKey,(U8 *)dervdKey,VE_CIPH_KEY_LEN);
                break;
             default:
                RETVALUE(RFAILED); 
                break;
          } /* switch */
       } /* for loop */
         break;
       case  CTF_KDF_TYPE4:
       {
         WrCellId cellId = transCb->ueCb->cellId;
         WR_GET_CELLCB(cellCb, cellId);
         if(cellCb == NULLP)
         {
            RLOG0(L_ERROR, "Invalid Cell Id");
            RETVALUE(RFAILED);
         }
         cmMemset((U8 *)&stringS[0], '\0', VE_STRING_S_LEN);

         stringS[0] = 0x13;
         stringS[1] = (cellCb->physCellId & 0xff00) >> 8;
         stringS[2] = cellCb->physCellId & 0x00ff; /*TODO: PCI length*/
         stringS[3] = 0x00;
         stringS[4] = 0x02; /*TODO: PCI length*/
         stringS[5] = (cellCb->dlCarrierFreq & 0xff00) >> 8;
         stringS[6] = cellCb->dlCarrierFreq & 0x00ff;
         stringS[7] = 0x00;
         stringS[8] = 0x02;/*TODO: dlCarrierFreq length */

         cmMemset((U8 *)&dervdKENBKey[0], '\0', VE_KENB_KEY_LEN);

         veKDF((U8 *) secCfg->kENB, (U8 *)stringS, (VE_STRING_S_LEN), (U8 *)dervdKENBKey);
         cmMemcpy((U8 *)secCfg->kENB,(U8 *)dervdKENBKey,VE_KENB_KEY_LEN);

         break;
       }
   }
   *asKeyGenerated = TRUE;   
   RETVALUE(ROK);
}

/**
 * @brief 
 *    This function is used to get KeNB Star Value.
 *
 * @details
 *    Function: wrUmmX2HoGetKeNBStar
 *
 *    Processing steps:
 *       - Send kEnb * key generation request to KDF.
 *       
 * @param[in]
 *    transCb      : transaction control block
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrPlatX2HoGetKeNBStar
(
WrUmmTransCb                 *transCb
)
{
   WrUmmX2SrcHoTrans         *x2SrcHoTrans = &(transCb->u.x2SrcHoTrans);
#ifdef TENB_AS_SECURITY
   WrUeSecCfg                *secCfg = NULLP;
   U8  stringS[VE_STRING_S_LEN];
   U8  kEnbStar[WR_KENB_KEY_LEN];
   U8  kENB[WR_KENB_KEY_LEN];

   RLOG2(L_INFO, " Generate kEnb * for Target PCI [%d] and EARFCN [%ld] ",
            x2SrcHoTrans->targetPci, x2SrcHoTrans->targetEarfcnDl);
      secCfg = transCb->ueCb->secCfg;
      stringS[0] = 0x13;
      stringS[1] = (U8) (x2SrcHoTrans->targetPci >> 8);
      stringS[2] = (U8) (x2SrcHoTrans->targetPci);
      stringS[3] = 0x00;    
      stringS[4] = 0x02;
      stringS[5] = (U8) (x2SrcHoTrans->targetEarfcnDl >> 8);
      stringS[6] = (U8) (x2SrcHoTrans->targetEarfcnDl);
      stringS[7] = 0x00;
      stringS[8] = 0x02;
      cmMemcpy((U8 *)&kENB[0],(U8 *)&secCfg->kENB[0],WR_KENB_KEY_LEN);
      veKDF((U8 *)&kENB[0],(U8 *)&stringS[0], VE_STRING_S_LEN,(U8 *)&kEnbStar[0]);
      cmMemcpy((U8 *)&x2SrcHoTrans->kEnbStar[0],(U8 *)&kEnbStar[0],WR_KENB_KEY_LEN);
      x2SrcHoTrans->isKenbKeyGenerated = TRUE;
#else
   U16 i;
   for(i = 0; i < WR_KENB_KEY_LEN; i++)
      x2SrcHoTrans->kEnbStar[i] = 0xff;
#endif

   RETVALUE(ROK);
}

#endif




/***********************************************************************
                    CTF related functions
************************************************************************/
/** @brief This function fills vendor specific parameters related to cell 
 *         configuration for PHY layer. 
 *
 * @details This function prepares and fills vendor specific parameters 
 *          related to cell configuration for PHY layer.
 *
 *     Function: wrFillCtfVndrCellCfg
 *
 *         Processing steps:
 *         - Fill the vendor specific PHY related configuration parameters.
 *
 * @param[in] cellCb: Cell control block.  
 * @param[in, out] cellCfgReq: Cell configuration.  
 * @return S16 
 *        -# Success : ROK
 *        -# Failure : RFAILED. 
 */
PUBLIC S16 wrFillCtfVndrCellCfg 
(
WrCellCb          *cellCb,
CtfCfgReqInfo     *cellCfgReq
)
{
   WrMsCellCfgReq    *msCellCfgReq;

#ifdef CTF_VER3
   WR_ALLOC(&cellCfgReq->vendorParams.paramBuffer, sizeof(WrMsCellCfgReq));
   if(cellCfgReq->vendorParams.paramBuffer == NULLP)
{
#ifdef DEBUGP
      RLOG0(L_ERROR,"paramBuffer is NULL.");
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */

   msCellCfgReq = (WrMsCellCfgReq *)cellCfgReq->vendorParams.paramBuffer;
   /*changed from sizeof(msCellCfgReq) to
    * sizeof(WrMsCellCfgReq) */
   cellCfgReq->vendorParams.buffLen = sizeof(WrMsCellCfgReq);
   msCellCfgReq->opMode  = cellCb->ctfCfg.dfltOpMode;/*chethan*/
   msCellCfgReq->counter = 0;
   msCellCfgReq->period  = cellCb->ctfCfg.dfltTagPeriod;/*chethan*/
   RLOG2(L_ERROR, "OP MODE : %d  PERIOD :%ld",msCellCfgReq->opMode,
                                      msCellCfgReq->period);

#endif
   RETVALUE(ROK);
} /* end of wrFillCtfVndrCellCfg */
/********************************************************************30**

           End of file:     fc_plat.c@@/main/TeNB_Main_BR/tenb_main_ccb/tenb_5.0_RIB/tenb_5.0_RIB_GA/7 - Mon Feb 15 12:50:57 2016

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
/main/0        ---      spoovali         1. T3K code cleanup
*********************************************************************91*/





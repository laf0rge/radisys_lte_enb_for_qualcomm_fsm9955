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

     Desc:     This file contains 
               

     File:     wr_umm_sec.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=74;


static int RLOG_MODULE_ID=32;

#include "wr.h"
#include "wr_emm.h"
#include "wr_umm.h"
#include "wr_umm_rrc_msg.h"
#include "wr_umm_trans.h"
#include "wr_ifm_rrc.h"

/*
 *      Fun:   wrUmmSecSndSecModeCmd
 *
 *      Desc:  RRC Security Mode Command
 *
 *      Ret:   ROK
 *
 *      Notes: None
 */
PUBLIC S16 wrUmmSecSndSecModeCmd
(
WrUmmTransCb                 *transCb
)
{
   S16                       ret = RFAILED;
   NhuUeCfgInfo              *ueCfg = NULLP;
   NhuDatReqSdus             *rrcDatReqSdu = NULLP;
   WrUeCb                    *ueCb = transCb->ueCb;
   WrInitCntxtSetupTransCb   *initCtxtCb = &(transCb->u.initCtxtCb);
   U8                        transId;

   /* Allocate memory for event structure */
   WR_ALLOCEVNT(&rrcDatReqSdu, sizeof(NhuDatReqSdus));
   if (rrcDatReqSdu == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Fill HDR Info */
   /* setting the subTrans bits to identify the security config procedure */
   wrUmmFillRrcEvntHdr(&(rrcDatReqSdu->hdr), ueCb->cellId, ueCb->crnti,
                                      transCb->transId | C4_SECURITYMODECMD);

   /* Fetch an RRC transaction id for this procedure.                     */
   if (ROK != wrUmmGetRrcTransId(ueCb, C4_SECURITYMODECMD, 
                      WR_GET_TRANS_FRM_TRANSID(transCb->transId), 
                      &transId))
   {
      RLOG0(L_ERROR, "RRC Transaction Not avialble");
      WR_FREEEVNT(rrcDatReqSdu);
      RETVALUE(RFAILED);
 
   }
 
   /* Build RRC Con Setup Message */
   if (wrUmmRrcFillSecModeCmd(transId,
                              initCtxtCb->secCfg, rrcDatReqSdu) != ROK)
   {
      RLOG0(L_ERROR, "wrUmmSecSndSecModeCmd Failed");
      WR_FREEEVNT(rrcDatReqSdu);
      RETVALUE(RFAILED);
   }

   /* Fill Config values */
   rrcDatReqSdu->sdu.isUeCfgPres = TRUE;
   ueCfg = &(rrcDatReqSdu->sdu.ueCfg);
   ueCfg->ueCfgBitMsk = NHU_EXP_CFG_SEC_CFG;
   wrUmmRrcFillSecCfg(&(ueCfg->secCfg), initCtxtCb->secCfg);

   RLOG_ARG2(L_EVENT, DBG_CRNTI,ueCb->crnti,"[OTA] Sending SecurityModeCommand"
         " CipherAlgo[%lu] IntegrityAlgo[%lu]",initCtxtCb->secCfg->ciphAlgo, 
         initCtxtCb->secCfg->intgAlgo);
   /* Send Security Mode Command through NhuDatRsp Primitive*/
   if (wrIfmRrcSndReqToUe(rrcDatReqSdu) != ROK)
   {
      RLOG0(L_ERROR, "wrIfmRrcSndReqToUe failed");
      WR_FREEEVNT(rrcDatReqSdu);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/*
*       Fun:   wrUmmSecKeyDervFunc 
*
*       Desc:  This is the Key Derivation Function.
*
*       Ret:   ROK
*
*       Notes: None
*/
PUBLIC S16 wrUmmSecKeyDervFunc
(
U8                           *secKey,
U8                           *strS,
U8                           *derKey
)
{
   U8                        extractKey[32];
   U8                        idx = 0;
   U8                        idxKey = 0;

   RLOG0(L_DEBUG, "Key Derivation Function");

   cmMemset(&extractKey[0], '\0', 32);

   /* Call a HMAC-SHA256 Algorithm API  currently not implemented */
   /* truncate the extracted key to 16 bytes long */
   for(idx = 16; idx < 32; idx++)
   {
      derKey[idxKey] = extractKey[idx];
      idxKey++;
   }

   RETVALUE(ROK);
}

/*
*       Fun:   wrUmmSecExtractSecKeys 
*
*       Desc:  This function extracts the keys from the Bit string(KENB)
*
*       Ret:   ROK
*
*       Notes: None
*/
PUBLIC S16 wrUmmSecExtractSecKeys
(
WrUeSecCfg                   *secCfg,
TknStrBSXL                   *secKey
)
{
   U8                        stringS[WR_STRING_S_LEN];
   U8                        dervdKey[WR_CIPH_KEY_LEN];
   U8                        type = 0;
   U8                        idx = 0;

   RLOG0(L_DEBUG, "Extract Intg. and ciphering keys");

   /* storing the base key (KENB) */
   for(idx = 0; idx < WR_KENB_KEY_LEN; idx++)
   {
      secCfg->kENB[idx] = secKey->val[idx];
   }

   /* Memset */
   cmMemset((U8 *)&stringS[0], '\0', WR_STRING_S_LEN);

   /* Building the String S = FC || P0 || L0 || P1 || L1 */
   /* FC */
   stringS[0] = 0x15;

   /* L0 */
   stringS[2] = 0x00;
   stringS[3] = 0x01;

   /* L1 */
   stringS[5] = 0x00;
   stringS[6] = 0x01;

   for(type = 0; type <= WR_INTG_KEY; type++)
   {
      /* Memset to zero */
      cmMemset((U8 *)&dervdKey[0], '\0', WR_CIPH_KEY_LEN);

      /* Fill P0 & P1 */ 
      switch(type)
      {
         case WR_CP_CIPH_KEY:
            /* P0 */
            stringS[1] = 0x03;
            /* P1 */
            stringS[4] = secCfg->ciphAlgo;

            /* Use the Key Derivation Function */
            wrUmmSecKeyDervFunc((U8 *)secKey->val, (U8 *)stringS, 
                  (U8 *)dervdKey);

            for (idx = 0; idx < WR_CIPH_KEY_LEN; idx++)
            {
               secCfg->cpCiphKey[idx] = dervdKey[idx];
            }

            break;

         case WR_UP_CIPH_KEY:
            /* P0 */
            stringS[1] = 0x05;
            /* P1 */
            stringS[4] = secCfg->ciphAlgo;

            /* Use the Key Derivation Function */
            wrUmmSecKeyDervFunc((U8 *)secKey->val, (U8 *)stringS, 
                  (U8 *)dervdKey);

            for (idx = 0; idx < WR_CIPH_KEY_LEN; idx++)
            {
               secCfg->upCiphKey[idx] = dervdKey[idx];
            }
            break;

         case WR_INTG_KEY:
            /* P0 */
            stringS[1] = 0x04;
            /* P1 */
            stringS[4] = secCfg->intgAlgo;

            /* Use the Key Derivation Function */
            wrUmmSecKeyDervFunc((U8 *)secKey->val, (U8 *)stringS, 
                  (U8 *)dervdKey);

            for (idx = 0; idx < WR_INTG_KEY_LEN; idx++)
            {
               secCfg->intgKey[idx] = dervdKey[idx];
            }
            break;

         default:
            RETVALUE(RFAILED); 
            break;
      }
   }
   RETVALUE(ROK); 
}

/*
*       Fun:   wrUmmSecGetSecCap
*
*       Desc:  This function decodes the security information and filles 
*              the secCfg structure.
*
*       Ret:   ROK
*
*       Notes: None
*/
PUBLIC S16 wrUmmSecGetSecCap
(
SztUESecurCapabilities       *ueSecCap,
WrUeSecCfg                   *secCfg
)
{
   TknBStr32                 *bitStr = NULLP;

   RLOG0(L_DEBUG, "Fetching Security info. from S1AP Pdu");

   if(secCfg == NULLP || ueSecCap == NULLP)
   {
      RLOG0(L_ERROR, "invalid parameters");
      RETVALUE(RFAILED);
   }

   /* fetch ciphering algo */
   bitStr = &(ueSecCap->encryptionAlgorithms);

   if (bitStr->val[0] & 0x80)
   {
      /* means EEA0 */
      secCfg->ciphAlgo = 0; 
   }
   else if (bitStr->val[0] & 0x40)
   {
      /* means EEA1 */
      secCfg->ciphAlgo = 1; 
   }
   else if (bitStr->val[0] & 0x20)
   {
      /* means EEA2 */
      secCfg->ciphAlgo = 2; 
   }
/* ZUC_ALGO - start */
   else if (bitStr->val[0] & 0x10)
   {
      /* means EEA3 */
      secCfg->ciphAlgo = 3; 
   }
/* ZUC_ALGO - end */

   /* fetch Integrity protection algo */
   bitStr = &(ueSecCap->integrityProtectionAlgorithms);

   if (bitStr->val[0] & 0x80)
   {
      /* means EIA1 */
      secCfg->intgAlgo = 1; 
   }
   else if (bitStr->val[0] & 0x40)
   {
      /* means EIA2 */
      secCfg->intgAlgo = 2; 
   }
/* ZUC_ALGO  */
   else if (bitStr->val[0] & 0x20)
   {
      /* means EIA3 */
      secCfg->intgAlgo = 3; 
   } 
/* ZUC_ALGO */
   RETVALUE(ROK);
} /* End of wrUmmSecGetSecCap */




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

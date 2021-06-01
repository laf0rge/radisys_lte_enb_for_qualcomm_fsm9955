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
               

     File:     wr_smm_dyncfg.c

     Sid:      fc_smm_dyncfg.c@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/Br_Tenb_Mspd_T2K_Intg_MS8.0_RC/2 - Mon Jul 29 15:49:07 2013


**********************************************************************/

static const char* RLOG_MODULE_NAME="SMM";
static int RLOG_MODULE_ID=2;
static int RLOG_FILE_ID=36;

#include "wr.h"
#include "wr_lwr.h"
#include "wr_cmn.h"
#include "wr_smm_smallcell.h"
#include "wr_smm_init.h"
#include "wr_utils.h"
#include "wr_msm_common.h" 
#include "lnl.h"
#include "lnl.x"



EXTERN MsmLteeNodeBparams lteeNodeBparams; 
EXTERN S16 smStartTmr ARGS((PTR cb, S16 tmrEvnt,U32 delay));

EXTERN Void FapAlarmInd(U32 alarmId, U8 severity, U8 causeType, U8 causeVal);
PRIVATE S16  smHdlNeighUtraFreqCfgDel(Void);
PRIVATE S16  smHdlNeighEutraFreqCfgDel(SmNeigEutraFreqList *nghEutraFreqList);
PRIVATE S16 smHdlNeighUtraCellCfgMod(LwrUtraNeighCell *utraCellCfg,
                                  MsmUtraNeighCell *updatedUtraCellCfg);
PRIVATE S16 smHdlNeighUtraCellCfgDel(Void);
PRIVATE S16 smHdlDynNeighUtraCellCfg(MsmNeighCellCfg *updatedCellList);
PRIVATE S16 smHdlNeighiEutraCellCfgAdd(MsmEUTRANeighCell *nghEutraAddCell);
PRIVATE S16 smHdlNeighiEutraCellCfgMod(MsmEUTRANeighCell *updatedNhgEutraCell,
                                     LwrEUTRANeighCell *nghEutraAddCell);
PRIVATE S16 smHdlNeighiEutraCellCfgDel(SmNeigEutraCellList *eutraCellList);
PRIVATE S16 smHdlDynNeighEutraCellCfg(MsmDynNeighCellCfg *updatedCellList);
PUBLIC S16 smHdlNeighEutraDelCellWithFreq(LwrNeighFreqCfg *delNghEutraFreq);
PRIVATE S16 smHdlNeighUtraDelCellWithFreq(LwrNeighFreqCfg *delNghUtraFreq);
PUBLIC S16 smHdlNeighEnbCfgDel(Void);
PRIVATE S16 smPackSendDynCfg(Void *cfg, U32 len, U32 cfgType);
extern PUBLIC Void msmDynConfigComplete(MsmCfmType msmcfmsuccess,MsmCauseType msmcausena, MsmDynCfmInfo dynCfmInfo);
PRIVATE S16 smHdlSonDynNeighEutraCellCfg(MsmDynNeighCellCfg *nghCellCfg);
EXTERN U32 smDynTmrEvent;
#ifdef TENB_STATS
EXTERN S16 TSInfInitStatsFrmWork ARGS((
   Bool   tenbSocketStats,
   Bool   tenbFileStats,
   Bool   tenbConsoleStats,
   U32    tenbStatsPeriodicity,
   U32    tendStatsSocketPort
));
EXTERN Void TSInfSetEnbIp ARGS((
   U32 enbIpAddr
));
#endif

/*
*
*       Fun:   smFillNghHashKeyInit
*
*       Desc:  This function is used for filling Neighbor hash key
*
*       Ret:   Void
*
*       Notes: None
*              
*       File:  wr_smm_dynCfg.c 
*
*/
#ifdef ANSI 
PUBLIC Void smFillNghHashKeyInit(smNghCellHasKey *hashKey, LwrPlmnId *plmnId, U16 cId)
#else
PUBLIC Void smFillNghHashKeyInit(hashKey,plmnId, cId)
smNghCellHasKey *hashKey;
LwrPlmnId *plmnId;
U16 cId;
#endif
{
   TRC2(smFillNghHashKeyInit);
   hashKey->cellId = cId; 
   hashKey->plmnId.numMncDigits = plmnId->numMncDigits; 
   hashKey->plmnId.mcc[0] = plmnId->mcc[0];
   hashKey->plmnId.mcc[1] = plmnId->mcc[1];
   hashKey->plmnId.mcc[2] = plmnId->mcc[2];

   hashKey->plmnId.mnc[0] = plmnId->mnc[0];
   hashKey->plmnId.mnc[1] = plmnId->mnc[1];
   if(plmnId->numMncDigits  == 3)
   {
     hashKey->plmnId.mnc[2] = plmnId->mnc[2];
   }
   else
   {
     hashKey->plmnId.mnc[2] = 0;
   }

}/* end of smFillNghHashKeyInit*/
 
/*
*
*       Fun:  smValidateSecMmeIp 
*
*       Desc:  validate for duplicacy and fill mme Ip
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_smm_dyncfg.c 
*
*/

PRIVATE S16 smValidateSecMmeIp
(
 U8          idx,
 Mmeiptype   ip
)
{
   U8 emtpyIpv6Addr[WR_IPV6_ADDR_LEN] = {0}; 
   U8 ret = RFAILED;

   if((0 == ip.mmeipv4)&&(ROK == cmMemcmp((U8*)&ip.mmeipv6, (U8*)&emtpyIpv6Addr,sizeof(CmInetIpAddr6))))
   {
      ret = ROK;
   }
   else if(ip.ipv4pres == TRUE)/*Check If Secondary MMEIP is configured with IPV4 address*/
   {
       if((ip.mmeipv4 == smCfgCb.mmeCfg[idx]->mmeAddr[1].u.ipv4TptAddr.address) || 
          (ip.mmeipv4 == smCfgCb.mmeCfg[idx]->mmeAddr[2].u.ipv4TptAddr.address) || 
          (ip.mmeipv4 == smCfgCb.mmeCfg[idx]->mmeAddr[3].u.ipv4TptAddr.address) || 
          (ip.mmeipv4 == smCfgCb.mmeCfg[idx]->mmeAddr[4].u.ipv4TptAddr.address ) )
        {
                ret = ROK;
        }
   }
   else if(ip.ipv6pres == TRUE)/*Check If Secondary MMEIP is configured with IPV6 address*/
   {
       if((ROK == cmMemcmp((U8*)ip.mmeipv6,(U8*)smCfgCb.mmeCfg[idx]->mmeAddr[1].u.ipv4TptAddr.address,
                   sizeof(CmInetIpAddr6))) || 
          (ROK == cmMemcmp((U8*)ip.mmeipv6,(U8*)smCfgCb.mmeCfg[idx]->mmeAddr[2].u.ipv6TptAddr.ipv6NetAddr,sizeof(CmInetIpAddr6))) || 
          (ROK == cmMemcmp((U8*)ip.mmeipv6,(U8*) smCfgCb.mmeCfg[idx]->mmeAddr[3].u.ipv6TptAddr.ipv6NetAddr,sizeof(CmInetIpAddr6)))|| 
          (ROK == cmMemcmp((U8*)ip.mmeipv6,(U8*) smCfgCb.mmeCfg[idx]->mmeAddr[3].u.ipv6TptAddr.ipv6NetAddr,sizeof(CmInetIpAddr6) )))
          {
                ret = ROK;
          }
   }

   RETVALUE(ret);
} /* end of smValidateSecMmeIp */
 
/*
*
*       Fun:  smValidateSecIp 
*
*       Desc:  validate for duplicacy and fill mme Ip
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_smm_dyncfg.c 
*
*/

PRIVATE S16 smValidateSecIp
(
 MsmLteeNodeBIPparams *mmeIpCfg,
 U8          idx,
 Mmeiptype   ip
)
{
   U8 ret = RFAILED;
   U8 emtpyIpv6Addr[WR_IPV6_ADDR_LEN] = {0};

   if((0 != ip.mmeipv4) ||
      (ROK != cmMemcmp((U8*)&ip.mmeipv6, (U8*)&emtpyIpv6Addr, sizeof(CmInetIpAddr6))))
   {
      /* Check if IPv4 address is same as received */
      if(ip.ipv4pres == TRUE)
      {
         if((ip.mmeipv4 == mmeIpCfg->lteSecIp1[idx].mmeipv4) || (ip.mmeipv4 == mmeIpCfg->lteSecIp2[idx].mmeipv4) || (ip.mmeipv4 == mmeIpCfg->lteSecIp3[idx].mmeipv4) || (ip.mmeipv4 == mmeIpCfg->lteSecIp4[idx].mmeipv4))
         {
            ret = ROK;
         }
      }

      /* Check if IPv6 address is same as received */
      if(ip.ipv6pres == TRUE)
      {
         if((ip.mmeipv6 == mmeIpCfg->lteSecIp1[idx].mmeipv6) || (ip.mmeipv6 == mmeIpCfg->lteSecIp2[idx].mmeipv6) || (ip.mmeipv6 == mmeIpCfg->lteSecIp3[idx].mmeipv6) || (ip.mmeipv6 == mmeIpCfg->lteSecIp4[idx].mmeipv6))
         {
            ret = ROK;
         }
      }
   }

   RETVALUE(ret);
} /* end of smValidateSecIp */
 
/*
 * 
 *        Fun:   smHdlDynMmeCfg
 * 
 *        Desc:  fill in cellCfg for WR
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_sm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smHdlDynMmeCfg
(
MsmLteeNodeBIPparams *mmeIpCfg 
)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlDynMmeCfg(mmeIpCfg)
MsmLteeNodeBIPparams *mmeIpCfg;
#endif /* end of ANSI*/
{
   U32                  delFlag =0;
   U32                  newListIdx = 0;
   U32                  oldListIdx = 0;
   U32                  mmeId = 0;
   U32                  idx = 0;
   LwrSmMmeCfg          *mmeCfg = NULLP;
   MsmLteeNodeBIPparams *mmeIpParam = NULLP;
   U32                  secIpInc = FALSE;
   U8                   idx1 = 1;
   Mmeiptype ip1;
   Mmeiptype ip2;
   Mmeiptype ip3;
   Mmeiptype ip4;
   TRC2(smHdlDynMmeCfg);

   mmeIpParam = &lteeNodeBparams.lteeNodeBIPparams;
   lteeNodeBparams.lteeNodeBIPparams.lteS1connmode = mmeIpCfg->lteS1connmode; 
   for(idx = 0; idx < mmeIpCfg->lteS1connmode; idx++)
   {
      /*Check If MME Ip Is IPV4 and Fill it from MSM*/
      if(mmeIpCfg->lteSiglinkserverlist[idx].ipv4pres == TRUE)
      {
         lteeNodeBparams.lteeNodeBIPparams.lteSiglinkserverlist[idx].mmeipv4 = mmeIpCfg->lteSiglinkserverlist[idx].mmeipv4;
         lteeNodeBparams.lteeNodeBIPparams.lteSecIp1[idx].mmeipv4 = mmeIpCfg->lteSecIp1[idx].mmeipv4;
         lteeNodeBparams.lteeNodeBIPparams.lteSecIp2[idx].mmeipv4 = mmeIpCfg->lteSecIp2[idx].mmeipv4;
         lteeNodeBparams.lteeNodeBIPparams.lteSecIp3[idx].mmeipv4 = mmeIpCfg->lteSecIp3[idx].mmeipv4;
         lteeNodeBparams.lteeNodeBIPparams.lteSecIp4[idx].mmeipv4 = mmeIpCfg->lteSecIp4[idx].mmeipv4;
      }
      /*Check If MME Ip Is IPV6 and Fill it from MSM*/
      if(mmeIpCfg->lteSiglinkserverlist[idx].ipv6pres == TRUE)
      {
         cmMemcpy((U8*)lteeNodeBparams.lteeNodeBIPparams.lteSiglinkserverlist[idx].mmeipv6,
               (U8*)mmeIpCfg->lteSiglinkserverlist[idx].mmeipv6,sizeof(U8)*16);
         cmMemcpy((U8*)lteeNodeBparams.lteeNodeBIPparams.lteSecIp1[idx].mmeipv6,
               (U8*) mmeIpCfg->lteSecIp1[idx].mmeipv6,sizeof(sizeof(U8)*16));
         cmMemcpy((U8*)lteeNodeBparams.lteeNodeBIPparams.lteSecIp2[idx].mmeipv6,
                (U8*) mmeIpCfg->lteSecIp2[idx].mmeipv6,sizeof(sizeof(U8)*16));
         cmMemcpy((U8*)lteeNodeBparams.lteeNodeBIPparams.lteSecIp3[idx].mmeipv6,
               (U8*) mmeIpCfg->lteSecIp3[idx].mmeipv6,sizeof(sizeof(U8)*16));
         cmMemcpy((U8*)lteeNodeBparams.lteeNodeBIPparams.lteSecIp4[idx].mmeipv6,
               (U8*) mmeIpCfg->lteSecIp4[idx].mmeipv6,sizeof(sizeof(U8)*16));
      }
   }

   for(oldListIdx = 0; oldListIdx < smCfgCb.noOfCfg; oldListIdx++)
   {
      for(newListIdx = 0; newListIdx < mmeIpCfg->lteS1connmode; newListIdx++)
      {
         if(smCfgCb.mmeCfg[oldListIdx]->mmeAddr[0].type == CM_TPTADDR_IPV4)
         {

            if(smCfgCb.mmeCfg[oldListIdx]->mmeAddr[0].u.ipv4TptAddr.address == mmeIpCfg->lteSiglinkserverlist[newListIdx].mmeipv4)
            {
               ip1.mmeipv4 = smCfgCb.mmeCfg[oldListIdx]->mmeAddr[1].u.ipv4TptAddr.address;
               ip2.mmeipv4 = smCfgCb.mmeCfg[oldListIdx]->mmeAddr[2].u.ipv4TptAddr.address;
               ip3.mmeipv4 = smCfgCb.mmeCfg[oldListIdx]->mmeAddr[3].u.ipv4TptAddr.address;
               ip4.mmeipv4 = smCfgCb.mmeCfg[oldListIdx]->mmeAddr[4].u.ipv4TptAddr.address;
            if((ROK == smValidateSecIp(mmeIpCfg, oldListIdx, ip1)) || (ROK == smValidateSecIp(mmeIpCfg, oldListIdx,ip2)) || (ROK == smValidateSecIp(mmeIpCfg, oldListIdx, ip3)) || (ROK == smValidateSecIp(mmeIpCfg, oldListIdx, ip4)))
            {
               break;
            }
            
           }
         }
         if(smCfgCb.mmeCfg[oldListIdx]->mmeAddr[0].type == CM_TPTADDR_IPV6)
         {
            if(ROK == cmMemcmp((U8*)smCfgCb.mmeCfg[oldListIdx]->mmeAddr[0].u.ipv6TptAddr.ipv6NetAddr,
                        (U8*)mmeIpCfg->lteSiglinkserverlist[newListIdx].mmeipv6,sizeof(CmInetIpAddr6)))
            {
               ip1.mmeipv4 = ip2.mmeipv4 = ip3.mmeipv4 = ip4.mmeipv4 = 0;
               cmMemcpy((U8*)ip1.mmeipv6,(U8*)smCfgCb.mmeCfg[oldListIdx]->mmeAddr[1].u.ipv6TptAddr.ipv6NetAddr,sizeof(CmInetIpAddr6));
               cmMemcpy((U8*)ip2.mmeipv6,(U8*)smCfgCb.mmeCfg[oldListIdx]->mmeAddr[2].u.ipv6TptAddr.ipv6NetAddr,sizeof(CmInetIpAddr6));
               cmMemcpy((U8*)ip3.mmeipv6,(U8*)smCfgCb.mmeCfg[oldListIdx]->mmeAddr[3].u.ipv6TptAddr.ipv6NetAddr,sizeof(CmInetIpAddr6));
               cmMemcpy((U8*)ip4.mmeipv6,(U8*)smCfgCb.mmeCfg[oldListIdx]->mmeAddr[4].u.ipv6TptAddr.ipv6NetAddr,sizeof(CmInetIpAddr6));

            if((ROK == smValidateSecIp(mmeIpCfg, oldListIdx, ip1)) || (ROK == smValidateSecIp(mmeIpCfg, oldListIdx,ip2)) || (ROK == smValidateSecIp(mmeIpCfg, oldListIdx, ip3)) || (ROK == smValidateSecIp(mmeIpCfg, oldListIdx, ip4)))
            {
               break;
            }
            
           }
         }

      }
      if((mmeIpCfg->lteS1connmode == newListIdx ) && (smCfgCb.mmeCfg[oldListIdx]->mmeAddr[0].type != 0))
      {
         delFlag = 1;
         smCfgCb.freeMmeIdList[smCfgCb.freeMmeIdx] = smCfgCb.mmeCfg[oldListIdx]->mmeId; 
         smCfgCb.freeMmeIdx++;
         smCfgCb.numMmeDel++;
         smBldSzPeerDelCntrlReq(smCfgCb.mmeCfg[oldListIdx]->mmeId, oldListIdx);
      }
   }
   if(delFlag == 0)
   {
      for(newListIdx = 0; newListIdx < mmeIpCfg->lteS1connmode; newListIdx++)
      {
         for(oldListIdx = 0; oldListIdx < smCfgCb.noOfCfg; oldListIdx++)
         {
            if(mmeIpCfg->lteSiglinkserverlist[newListIdx].ipv4pres == TRUE)
            {
               if(mmeIpCfg->lteSiglinkserverlist[newListIdx].mmeipv4 ==
                                 smCfgCb.mmeCfg[oldListIdx]->mmeAddr[0].u.ipv4TptAddr.address)
               {
                  if((ROK != smValidateSecMmeIp(newListIdx, mmeIpCfg->lteSecIp1[newListIdx])) || (ROK != smValidateSecMmeIp(newListIdx, mmeIpCfg->lteSecIp2[newListIdx])) || (ROK != smValidateSecMmeIp(newListIdx, mmeIpCfg->lteSecIp3[newListIdx])) || (ROK != smValidateSecMmeIp(newListIdx, mmeIpCfg->lteSecIp4[newListIdx])))
               {
                  secIpInc = TRUE;
               }
               break;
              }
             }
             if(mmeIpCfg->lteSiglinkserverlist[newListIdx].ipv6pres == TRUE)
             {
               if(ROK == cmMemcmp((U8*)mmeIpCfg->lteSiglinkserverlist[newListIdx].mmeipv6,
                                (U8*)smCfgCb.mmeCfg[oldListIdx]->mmeAddr[0].u.ipv6TptAddr.ipv6NetAddr,sizeof(CmInetIpAddr6)))
               {
                  if((ROK != smValidateSecMmeIp(newListIdx, mmeIpCfg->lteSecIp1[newListIdx])) || (ROK != smValidateSecMmeIp(newListIdx, mmeIpCfg->lteSecIp2[newListIdx])) || (ROK != smValidateSecMmeIp(newListIdx, mmeIpCfg->lteSecIp3[newListIdx])) || (ROK != smValidateSecMmeIp(newListIdx, mmeIpCfg->lteSecIp4[newListIdx])))
               {
                  secIpInc = TRUE;
               }
               break;
              }

             }
         }
         /* check if secondary IP has been added and delete flag is false
          * then delete that MME */
         if((TRUE == secIpInc) && (TRUE != delFlag))
         {
            delFlag = TRUE;
            smCfgCb.freeMmeIdList[smCfgCb.freeMmeIdx] = smCfgCb.mmeCfg[oldListIdx]->mmeId; 
            smCfgCb.freeMmeIdx++;
            smCfgCb.numMmeDel++;
            smBldSzPeerDelCntrlReq(smCfgCb.mmeCfg[oldListIdx]->mmeId, oldListIdx);
         }

         if(smCfgCb.noOfCfg == oldListIdx)
         {
            if(smCfgCb.freeMmeIdx > 0)
            {
               mmeId = smCfgCb.freeMmeIdList[smCfgCb.freeMmeIdx-1]; 
               smCfgCb.mmeCfg[mmeId - 1]->mmeId = mmeId;
               if(smCfgCb.mmeCfg[mmeId - 1]->mmeAddr[0].type == CM_TPTADDR_IPV4)
               {
                 smCfgCb.mmeCfg[mmeId - 1]->mmeAddr[0].u.ipv4TptAddr.address =
                      lteeNodeBparams.lteeNodeBIPparams.lteSiglinkserverlist[newListIdx].mmeipv4;
               }
               else if(smCfgCb.mmeCfg[mmeId - 1]->mmeAddr[0].type == CM_TPTADDR_IPV6)
               {
                  cmMemcpy((U8*)smCfgCb.mmeCfg[mmeId - 1]->mmeAddr[0].u.ipv6TptAddr.ipv6NetAddr,
                     (U8*)lteeNodeBparams.lteeNodeBIPparams.lteSiglinkserverlist[newListIdx].mmeipv6,
                      sizeof(CmInetIpAddr6));
               }
                 if(RFAILED != smValidateFillMmeIp(smCfgCb.mmeCfg[mmeId -1],&idx1,mmeIpParam->lteSecIp1[newListIdx]))
                 {
                    if(RFAILED != smValidateFillMmeIp(smCfgCb.mmeCfg[mmeId -1],&idx1,mmeIpParam->lteSecIp2[newListIdx]))
                    {
                       if(RFAILED != smValidateFillMmeIp(smCfgCb.mmeCfg[mmeId -1],&idx1,mmeIpParam->lteSecIp3[newListIdx]))
                       {
                          smValidateFillMmeIp(smCfgCb.mmeCfg[mmeId -1],&idx1,mmeIpParam->lteSecIp4[newListIdx]);
                       }
                    }
                 }
               
                 smCfgCb.mmeCfg[mmeId - 1]->noOfIps = idx1;
                 smCfgCb.freeMmeIdx--;
            }
            else
            {
               WR_ALLOC(&mmeCfg, sizeof(LwrSmMmeCfg));
               if(mmeCfg == NULLP)
               {
                  RETVALUE(RFAILED);
               }
               mmeCfg->mmeId   = smCfgCb.noOfCfg + 1;
               mmeId = mmeCfg->mmeId; 
               if(lteeNodeBparams.lteeNodeBIPparams.lteSiglinkserverlist[newListIdx].ipv4pres == TRUE)
               {
                  mmeCfg->mmeAddr[0].type = CM_TPTADDR_IPV4;
                  mmeCfg->mmeAddr[0].u.ipv4TptAddr.address = lteeNodeBparams.lteeNodeBIPparams.lteSiglinkserverlist[newListIdx].mmeipv4;
               }
               if(lteeNodeBparams.lteeNodeBIPparams.lteSiglinkserverlist[newListIdx].ipv6pres == TRUE)
               {
                  mmeCfg->mmeAddr[0].type = CM_TPTADDR_IPV6;
                  cmMemcpy((U8*)mmeCfg->mmeAddr[0].u.ipv6TptAddr.ipv6NetAddr,(U8*)lteeNodeBparams.lteeNodeBIPparams.lteSiglinkserverlist[newListIdx].mmeipv6,sizeof(CmInetIpAddr6));
               }
               if(RFAILED != smValidateFillMmeIp(mmeCfg,&idx1,mmeIpParam->lteSecIp1[newListIdx]))
               {
                  if(RFAILED != smValidateFillMmeIp(mmeCfg,&idx1,mmeIpParam->lteSecIp2[newListIdx]))
                  {
                     if(RFAILED != smValidateFillMmeIp(mmeCfg,&idx1,mmeIpParam->lteSecIp3[newListIdx]))
                     {
                        smValidateFillMmeIp(mmeCfg,&idx1,mmeIpParam->lteSecIp4[newListIdx]);
                     }
                  }
               }
               mmeCfg->noOfIps = idx1;
               smCfgCb.mmeCfg[mmeId-1] = mmeCfg;
               smCfgCb.noOfCfg++;
            }
            smCfgCb.addMmeList[smCfgCb.numMmeAdded] = mmeId; 
            smCfgCb.numMmeAdded++;
            smBuildDynSzPeerCfg(mmeId,(mmeId-1));
         }
      }
   }
   RETVALUE(ROK);
} /* end of smHdlDynMmeCfg */

/*
 * 
 *        Fun:   wrSendDynmmeDelcfg
 * 
 *        Desc:  fill in cellCfg for WR
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_sm_dyncfg.c 
 */
#ifdef ANSI
PUBLIC S16 wrSendDynMmeDelcfg
(
U32 mmeIndex
)
#else /* if ANSI not defined*/
PUBLIC S16 wrSendDynMmeDelcfg(mmeIndex)
U32 mmeIndex;
#endif /* end of ANSI*/
{
   LwrMmeCfg    *mmeCfg = NULLP;
   Pst          dyPst;
   LwrDynMngmt  *lwrDynMngmt = NULLP;
   Header       *hdr;
   U8           idx = 0;

   TRC2(wrSendDynMmeDelcfg);

   WR_ALLOC(&lwrDynMngmt, sizeof(LwrDynMngmt));

   if(lwrDynMngmt == NULLP)
   {
      RETVALUE(RFAILED);
   }

   WR_ALLOC(&mmeCfg, sizeof(LwrMmeCfg));
   if(mmeCfg == NULLP)
   {
      WR_FREE(lwrDynMngmt, sizeof(LwrDynMngmt));
      RETVALUE(RFAILED);
   }

   SM_SET_ZERO(&dyPst, sizeof(Pst));

   mmeCfg->action = WR_ACT_DEL;
   /*Filling IP Address*/
   mmeCfg->t.delMme.mmeIndex        = (U8)smCfgCb.mmeCfg[mmeIndex]->mmeId;
   mmeCfg->t.delMme.peerId          = (U8)smCfgCb.mmeCfg[mmeIndex]->mmeId;

   smCfgCb.mmeCfg[mmeIndex]->mmeId = 0;
   for(idx = 0; idx < LWR_MAX_IP_PER_MME; idx++)
   {
     /* Set invalid MME IP Address Type */
        smCfgCb.mmeCfg[mmeIndex]->mmeAddr[idx].type = 0;
   }
   /*Filling MME state*/

   hdr = &lwrDynMngmt->hdr; 
   lwrDynMngmt->cfgType = LWR_DYN_EVNT_MME_IP_PRAMS; 

   /* Fill Header */
   hdr->msgType             = TCFG;
   hdr->msgLen              = 0;
   hdr->entId.ent           = ENTWR;
   hdr->entId.inst          = SM_INST_ZERO;
   hdr->elmId.elmnt         = STWRDYNCFG;
   hdr->elmId.elmntInst1    = LWR_DYN_EVNT_MME_IP_PRAMS;
   hdr->elmId.elmntInst2    = 0;
   hdr->elmId.elmntInst3    = 0;
   hdr->seqNmb              = 0;
   hdr->version             = 0;
   /*hdr->transId             = mmeIndex;*/
   hdr->transId             = LWR_TRANSID_MME_DYN_DELCFG;
   hdr->response.prior      = PRIOR0;
   hdr->response.route      = RTESPEC;
   hdr->response.mem.region = smCb.init.region;
   hdr->response.mem.pool   = WR_POOL;
   hdr->response.selector   = SM_SELECTOR_LWLC;
   /* Fill Pst */
   dyPst.selector  = SM_SELECTOR_LWLC;
   dyPst.srcEnt    = ENTSM;
   dyPst.dstEnt    = ENTWR;
   dyPst.dstProcId = SM_WR_PROC;
   dyPst.srcProcId = SM_SM_PROC;

   lwrDynMngmt->cfgPriority = 0;
   lwrDynMngmt->dynCfg = mmeCfg;
   SmMiLwrDynCfgReq(&dyPst, lwrDynMngmt);
   RETVALUE(ROK);
} /* end of wrSendDynmmeDelcfg */ 

 /*
 * 
 *        Fun:  wrSendDynAddMmeCfg
 * 
 *        Desc:  fill in AddMme for WR
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_sm_dyncfg.c 
 */
#ifdef ANSI
PUBLIC S16 wrSendDynAddMmeCfg
(
U32 mmeIndex
)
#else /* if ANSI not defined*/
PUBLIC S16 wrSendDynAddMmeCfg(mmeIndex)
U32 mmeIndex;
#endif /* end of ANSI*/
{
   LwrMmeCfg    *mmeCfg = NULLP;
   Pst          dyPst;
   LwrDynMngmt  *lwrDynMngmt = NULLP;
   Header       *hdr;
   U8           idx = 0;

   TRC2(wrSendDynAddMmeCfg);
   WR_ALLOC(&lwrDynMngmt, sizeof(LwrDynMngmt));

   if(lwrDynMngmt == NULLP)
   {
      RETVALUE(RFAILED);
   }

   WR_ALLOC(&mmeCfg, sizeof(LwrMmeCfg));

   if(mmeCfg == NULLP)
   {
      WR_FREE(lwrDynMngmt, sizeof(LwrDynMngmt));
      RETVALUE(RFAILED);
   }

   SM_SET_ZERO(&dyPst, sizeof(Pst));

   mmeCfg->action = WR_ACT_ADD;
   /*Filling IP Address*/
   mmeCfg->t.addMme.mmeIndex        = (U8)smCfgCb.mmeCfg[mmeIndex]->mmeId;
   mmeCfg->t.addMme.peerId          = (U8)smCfgCb.mmeCfg[mmeIndex]->mmeId;
   for(idx = 0; idx < smCfgCb.mmeCfg[mmeIndex]->noOfIps; idx++)
   {
      wrCpyCmTptAddr(&mmeCfg->t.addMme.ipAddr[idx], &smCfgCb.mmeCfg[mmeIndex]->mmeAddr[idx]);
   }
   /*Filling MME state*/
   hdr = &lwrDynMngmt->hdr; 
   lwrDynMngmt->cfgType = LWR_DYN_EVNT_MME_IP_PRAMS; 

   /* Fill Header */
   hdr->msgType             = TCFG;
   hdr->msgLen              = 0;
   hdr->entId.ent           = ENTWR;
   hdr->entId.inst          = SM_INST_ZERO;
   hdr->elmId.elmnt         = STWRDYNCFG;
   hdr->elmId.elmntInst1    = LWR_DYN_EVNT_MME_IP_PRAMS;
   hdr->elmId.elmntInst2    = 0;
   hdr->elmId.elmntInst3    = 0;
   hdr->seqNmb              = 0;
   hdr->version             = 0;
   hdr->transId             = LWR_TRANSID_MME_DYN_ADDCFG;
   hdr->response.prior      = PRIOR0;
   hdr->response.route      = RTESPEC;
   hdr->response.mem.region = smCb.init.region;
   hdr->response.mem.pool   = WR_POOL;
   hdr->response.selector   = SM_SELECTOR_LWLC;
   /* Fill Pst */
   dyPst.selector  = SM_SELECTOR_LWLC;
   dyPst.srcEnt    = ENTSM;
   dyPst.dstEnt    = ENTWR;
   dyPst.dstProcId = SM_WR_PROC;
   dyPst.srcProcId = SM_SM_PROC;

   lwrDynMngmt->cfgPriority = 0;
   lwrDynMngmt->dynCfg = mmeCfg;
   SmMiLwrDynCfgReq(&dyPst, lwrDynMngmt);
   RETVALUE(ROK);
} /* end of wrSendDynAddMmeCfg*/     

 /*
 * 
 *        Fun:  wrSmDynAddMme
 * 
 *        Desc:  fill in AddMme for WR
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_sm_dyncfg.c 
 */
#ifdef ANSI
PUBLIC S16 wrSmDynAddMme( )
#else /* if ANSI not defined*/
PUBLIC S16 wrSmDynAddMme()
#endif /* end of ANSI*/
{
   U16         newListIdx = 0;
   U16         oldListIdx = 0;
   U16         mmeId = 0;
   LwrSmMmeCfg *mmeCfg;
   U8          idx1 = 0;
   MsmLteeNodeBIPparams *mmeIpParam = NULLP;

   mmeIpParam = &lteeNodeBparams.lteeNodeBIPparams;
   if((lteeNodeBparams.lteeNodeBIPparams.lteS1connmode == 1) && (lteeNodeBparams.lteeNodeBIPparams.lteSiglinkserverlist[0].mmeipv4 == 0))
   {
      RETVALUE(ROK);
   }
   for(newListIdx = 0; newListIdx < lteeNodeBparams.lteeNodeBIPparams.lteS1connmode; newListIdx++)
   {
      for(oldListIdx = 0; oldListIdx < smCfgCb.noOfCfg; oldListIdx++)
      {
        
         if((lteeNodeBparams.lteeNodeBIPparams.lteSiglinkserverlist[newListIdx].ipv4pres==TRUE)&&(lteeNodeBparams.lteeNodeBIPparams.lteSiglinkserverlist[newListIdx].mmeipv4 == smCfgCb.mmeCfg[oldListIdx]->mmeAddr[0].u.ipv4TptAddr.address))
         {
            break;
         }

         if((lteeNodeBparams.lteeNodeBIPparams.lteSiglinkserverlist[newListIdx].ipv6pres==TRUE)&&(lteeNodeBparams.lteeNodeBIPparams.lteSiglinkserverlist[newListIdx].mmeipv6 == smCfgCb.mmeCfg[oldListIdx]->mmeAddr[0].u.ipv6TptAddr.ipv6NetAddr))
         {
            break;
         }
      }
      if(oldListIdx == smCfgCb.noOfCfg)
      {
         if(smCfgCb.freeMmeIdx > 0)
         {
            mmeId = smCfgCb.freeMmeIdList[smCfgCb.freeMmeIdx-1]; 
            smCfgCb.mmeCfg[mmeId - 1]->mmeId = mmeId;
            if(lteeNodeBparams.lteeNodeBIPparams.lteSiglinkserverlist[newListIdx].ipv4pres == TRUE)
            {
               smCfgCb.mmeCfg[mmeId - 1]->mmeAddr[0].u.ipv4TptAddr.address = lteeNodeBparams.lteeNodeBIPparams.lteSiglinkserverlist[newListIdx].mmeipv4;
            }

            if(lteeNodeBparams.lteeNodeBIPparams.lteSiglinkserverlist[newListIdx].ipv6pres == TRUE)
            {
               cmMemcpy((U8*)smCfgCb.mmeCfg[mmeId - 1]->mmeAddr[0].u.ipv6TptAddr.ipv6NetAddr,(U8*)lteeNodeBparams.lteeNodeBIPparams.lteSiglinkserverlist[newListIdx].mmeipv6,sizeof(CmInetIpAddr6));
            }

            if(RFAILED != smValidateFillMmeIp(smCfgCb.mmeCfg[mmeId -1],&idx1,mmeIpParam->lteSecIp1[newListIdx]))
            {
               if(RFAILED != smValidateFillMmeIp(smCfgCb.mmeCfg[mmeId -1],&idx1,mmeIpParam->lteSecIp2[newListIdx]))
               {
                  if(RFAILED != smValidateFillMmeIp(smCfgCb.mmeCfg[mmeId -1],&idx1,mmeIpParam->lteSecIp3[newListIdx]))
                  {
                     smValidateFillMmeIp(smCfgCb.mmeCfg[mmeId -1],&idx1,mmeIpParam->lteSecIp4[newListIdx]);
                  }
               }
            }
            smCfgCb.mmeCfg[mmeId - 1]->noOfIps = idx1; 
            smCfgCb.freeMmeIdx--;
         }
         else
         {
            WR_ALLOC(&mmeCfg, sizeof(LwrSmMmeCfg));
            if(mmeCfg == NULLP)
            {
               RETVALUE(RFAILED);
            } 
            if(smCfgCb.noOfCfg < (LWR_SM_MAX_MMES - 1))
            {
               mmeCfg->mmeId   = smCfgCb.noOfCfg + 1;
               mmeId = mmeCfg->mmeId; 
              if(lteeNodeBparams.lteeNodeBIPparams.lteSiglinkserverlist[newListIdx].ipv4pres == TRUE)
              {
                 mmeCfg->mmeAddr[0].u.ipv4TptAddr.address = lteeNodeBparams.lteeNodeBIPparams.lteSiglinkserverlist[newListIdx].mmeipv4; 
              }
              else if(lteeNodeBparams.lteeNodeBIPparams.lteSiglinkserverlist[newListIdx].ipv6pres == TRUE)
              {
                 cmMemcpy((U8*)mmeCfg->mmeAddr[0].u.ipv6TptAddr.ipv6NetAddr,(U8*)lteeNodeBparams.lteeNodeBIPparams.lteSiglinkserverlist[newListIdx].mmeipv6,sizeof(CmInetIpAddr6));

              }
               if(RFAILED != smValidateFillMmeIp(mmeCfg,&idx1,mmeIpParam->lteSecIp1[newListIdx]))
               {
                  if(RFAILED != smValidateFillMmeIp(mmeCfg,&idx1,mmeIpParam->lteSecIp2[newListIdx]))
                  {
                     if(RFAILED != smValidateFillMmeIp(mmeCfg,&idx1,mmeIpParam->lteSecIp3[newListIdx]))
                     {
                        smValidateFillMmeIp(mmeCfg,&idx1,mmeIpParam->lteSecIp4[newListIdx]);
                     }
                  }
               }
               mmeCfg->noOfIps = idx1;
               smCfgCb.mmeCfg[mmeId - 1] = mmeCfg;
               smCfgCb.noOfCfg++;
            }
         }
         smCfgCb.addMmeList[smCfgCb.numMmeAdded] = mmeId; 
         smCfgCb.numMmeAdded++;
         smBuildDynSzPeerCfg(mmeId,(mmeId-1));
      }
   }
   RETVALUE(ROK);
}

/*
 * 
 *        Fun:   smHdlDynMibCfg
 * 
 *        Desc:  fill in cellCfg for WR
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smHdlDynMibCfg
(
 MsmLteCellMibCfgGrp *modMibCfg, Void **dynCfg
)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlDynMibCfg(modMibCfg, dynCfg)
MsmLteCellMibCfgGrp *modMibCfg;
Void **dynCfg;
#endif /* end of ANSI*/
{
   LwrCellMibCfgGrp *mibCfg=NULLP;

   TRC2(smHdlDynMibCfg);
 
   WR_ALLOC(&mibCfg, sizeof(LwrCellMibCfgGrp));
   if(mibCfg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);      
   }
   mibCfg->reCfgType |= LWR_CELL_MIB_CFG_DLBW;
   mibCfg->dlBw = modMibCfg->dlBw[0];
   *dynCfg = (Void*)mibCfg;
   RETVALUE(ROK);      
} /*end of smHdlDynMibCfg */



/*
 *
 *        Fun:   smHdlDynSchdCfg
 *
 *        Desc:  fill in cellCfg for WR
 *
 *        Ret:   ROK
 *
 *        Notes: None
 *
 *        File:  wr_smm_dyncfg.c
 */
#ifdef ANSI
PRIVATE S16 smHdlDynSchdCfg
(
 MsmLteCellDlSchdCfgGrp *modSchdCfg, Void **dynCfg
)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlDynSchdCfg(modSchdCfg, dynCfg)
MsmLteCellDlSchdCfgGrp *modSchdCfg;
Void **dynCfg;
#endif /* end of ANSI*/
{
   LwrCellDlSchdCfgGrp *schdCfg = NULLP;

   TRC2(smHdlDynSchdCfg);

   WR_ALLOC(&schdCfg, sizeof(LwrCellDlSchdCfgGrp));
   if(schdCfg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   schdCfg->dlfsSchd = modSchdCfg->dlfsSchd;
   schdCfg->reCfgType |= LWR_SCHD_CFG_DLFS_SCHD;

   *dynCfg = (Void*)schdCfg;

   RETVALUE(ROK);
} /*end of smHdlDynSchdCfg */



/* ccpu00135220 */
/*
 * 
 *        Fun:   smHdlDynTimerCfg
 * 
 *        Desc:  fill in cellCfg for WR
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smHdlDynTimerCfg
(
 MsmLteProcTimerCfg *modTimerCfg, Void **dynCfg
)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlDynTimerCfg(modTimerCfg, dynCfg)
MsmLteProcTimerCfg *modTimerCfg;
Void **dynCfg;
#endif /* end of ANSI*/
{
   LwrProcTimerCfgGrp *timerCfg=NULLP;

   TRC2(smHdlDynTimerCfg);
 
   WR_ALLOC(&timerCfg, sizeof(LwrProcTimerCfgGrp));
   if(timerCfg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);      
   }
   timerCfg->s1HandoverPrepTmr = modTimerCfg->s1PrepTimerVal;
   timerCfg->reCfgType |= LWR_TMR_CFG_S1PREP_TMR_CFG;

   timerCfg->s1RelocOverallTmr = modTimerCfg->s1OvrAllTimerVal;
   timerCfg->reCfgType |= LWR_TMR_CFG_S1OVRALL_TMR_CFG;

   timerCfg->x2HandoverPrepTmr = modTimerCfg->x2PrepTimerVal;
   timerCfg->reCfgType |= LWR_TMR_CFG_X2PREP_TMR_CFG;

   timerCfg->x2RelocOverallTmr = modTimerCfg->x2OvrAllTimerVal;
   timerCfg->reCfgType |= LWR_TMR_CFG_X2OVRALL_TMR_CFG;

   *dynCfg = (Void*)timerCfg;

   RETVALUE(ROK);      
} /*end of smHdlDynTimerCfg */


/*
 * 
 *        Fun:   smHdlDynSmCellCfg
 * 
 *        Desc:  fill in cellCfg for WR
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smHdlDynSmCellCfg
(
 MsmLteSmCellCfg *modSmCellCfg
)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlDynSmCellCfg(modSmCellCfg)
MsmLteSmCellCfg *modSmCellCfg;
#endif /* end of ANSI*/
{
#ifdef TENB_AS_SECURITY
   U32 loopCnt = 0;

   for(loopCnt = 0; loopCnt < WR_MAX_AS_SEC_ALGOS; loopCnt++)
   {
      wreNodBPriIntgAlgo[loopCnt].pres = FALSE;
      wreNodBPriCiphAlgo[loopCnt].pres = FALSE;
   }
   for(loopCnt = 0; loopCnt < WR_MAX_AS_SEC_ALGOS; loopCnt++)
   {
      if((modSmCellCfg->genUeCfg.intgAlgo[loopCnt] < WR_MAX_AS_SEC_ALGOS) && 
         (modSmCellCfg->genUeCfg.intgAlgo[loopCnt] > 0))
      {
         wreNodBPriIntgAlgo[loopCnt].pres= TRUE;
         wreNodBPriIntgAlgo[loopCnt].val = modSmCellCfg->genUeCfg.intgAlgo[loopCnt];
      }
   }
   for(loopCnt = 0; loopCnt < WR_MAX_AS_SEC_ALGOS; loopCnt++)
   {
      if((modSmCellCfg->genUeCfg.ciphAlgo[loopCnt] < WR_MAX_AS_SEC_ALGOS) && 
         (modSmCellCfg->genUeCfg.ciphAlgo[loopCnt] > 0))
      {
         wreNodBPriCiphAlgo[loopCnt].pres= TRUE;
         wreNodBPriCiphAlgo[loopCnt].val = modSmCellCfg->genUeCfg.ciphAlgo[loopCnt];
      }
   }
#endif
   RETVALUE(ROK);      
} /*end of smHdlDynSmCellCfg */
/* ccpu00135220 */


/*
 * 
 *        Fun:   smHdlDynSib3Cfg
 * 
 *        Desc:  fill in cellCfg for WR
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smHdlDynSib3Cfg
(
 MsmLteCellSib3CfgGrp *modSib3Cfg, Void **dynCfg
)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlDynSib3Cfg(modSib3Cfg, dynCfg)
MsmLteCellSib3CfgGrp *modSib3Cfg;
Void **dynCfg;
#endif /* end of ANSI*/
{
   LwrCellSib3CfgGrp *sib3Cfg=NULLP;

   TRC2(smHdlDynSib3Cfg);

   WR_ALLOC(&sib3Cfg, sizeof(LwrCellSib3CfgGrp));
   if(sib3Cfg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);      
   }
   sib3Cfg->qHyst      =  modSib3Cfg->qHyst;
   if(lteeNodeBparams.lteCellSib3CfgGrp.qHyst != sib3Cfg->qHyst) /*Check against old value*/
   {
      sib3Cfg->reCfgType |= LWR_CELL_SIB3_CFG_QHYST;
      lteeNodeBparams.lteCellSib3CfgGrp.qHyst = sib3Cfg->qHyst; /*Update the old value*/
   }

   sib3Cfg->sfMedium   = modSib3Cfg->sfMedium;
   if(lteeNodeBparams.lteCellSib3CfgGrp.sfMedium != sib3Cfg->sfMedium) /*Check against old value*/
   {
      sib3Cfg->reCfgType |= LWR_CELL_SIB3_CFG_SF_MEDIUM;
      lteeNodeBparams.lteCellSib3CfgGrp.sfMedium = sib3Cfg->sfMedium; /*Update the old value*/
   }

   sib3Cfg->sfHigh     = modSib3Cfg->sfHigh;
   if(lteeNodeBparams.lteCellSib3CfgGrp.sfHigh != sib3Cfg->sfHigh) /*Check against old value*/
   {
      sib3Cfg->reCfgType |= LWR_CELL_SIB3_CFG_SF_HIG;
      lteeNodeBparams.lteCellSib3CfgGrp.sfHigh = sib3Cfg->sfHigh; /*Update the old value*/
   }
   sib3Cfg->mobStateParams.tEvalulation   =  modSib3Cfg->mobStateParams.tEvalulation ;
   sib3Cfg->mobStateParams.tHystNormal   =  modSib3Cfg->mobStateParams.tHystNormal ;
   sib3Cfg->mobStateParams.nCellChangeMedium   =  modSib3Cfg->mobStateParams.nCellChangeMedium ;
   sib3Cfg->mobStateParams.nCellChangeHigh   =  modSib3Cfg->mobStateParams.nCellChangeHigh ;
   if(
         (lteeNodeBparams.lteCellSib3CfgGrp.mobStateParams.tEvalulation != sib3Cfg->mobStateParams.tEvalulation) ||
         (lteeNodeBparams.lteCellSib3CfgGrp.mobStateParams.tHystNormal  != sib3Cfg->mobStateParams.tHystNormal) ||
         (lteeNodeBparams.lteCellSib3CfgGrp.mobStateParams.nCellChangeMedium != sib3Cfg->mobStateParams.nCellChangeMedium) ||
         (lteeNodeBparams.lteCellSib3CfgGrp.mobStateParams.nCellChangeHigh   != sib3Cfg->mobStateParams.nCellChangeHigh)) /*Check against old value*/
   {
      sib3Cfg->reCfgType |= LWR_CELL_SIB3_CFG_MOB_ST_PAR;
      /*Update the old values*/
      lteeNodeBparams.lteCellSib3CfgGrp.mobStateParams.tEvalulation = sib3Cfg->mobStateParams.tEvalulation;
      lteeNodeBparams.lteCellSib3CfgGrp.mobStateParams.tHystNormal  = sib3Cfg->mobStateParams.tHystNormal;
      lteeNodeBparams.lteCellSib3CfgGrp.mobStateParams.nCellChangeMedium = sib3Cfg->mobStateParams.nCellChangeMedium;
      lteeNodeBparams.lteCellSib3CfgGrp.mobStateParams.nCellChangeHigh   = sib3Cfg->mobStateParams.nCellChangeHigh;
   }
   /* commenting it out as below parameters will be updated from neighbour 
    * frequency configuration */
   *dynCfg = (Void*)sib3Cfg;
   RETVALUE(ROK);

} /* end of smHdlDynSib3Cfg */

 /*
 * 
 *        Fun:   smHdlDynSib1Cfg
 * 
 *        Desc:  fill in cellCfg for WR
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smHdlDynSib1Cfg
(
 MsmLteCellSib1CfgGrp *modSib1Cfg, Void **dynCfg
)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlDynSib1Cfg(modSib1Cfg, dynCfg)
MsmLteCellSib1CfgGrp *modSib1Cfg;
Void **dynCfg;
#endif /* end of ANSI*/
{
   LwrCellSib1CfgGrp *sib1Cfg=NULLP;
   LwrCellAccessInfo *cellAccessInfo = NULLP;

   TRC2(smHdlDynSib1Cfg);

   WR_ALLOC(&sib1Cfg, sizeof(LwrCellSib1CfgGrp));
   if(sib1Cfg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);      
   }
   cellAccessInfo                = &sib1Cfg->cellAccessInfo;
   cellAccessInfo->tac           = modSib1Cfg->cellAccessInfo.tac;
   cellAccessInfo->numOfPlmns    = modSib1Cfg->cellAccessInfo.numOfPlmns;
   cellAccessInfo->cellBarred    = modSib1Cfg->cellAccessInfo.cellBarred;
   cellAccessInfo->intraFreqReSelection  = (Bool)wrSmDfltIntraFreqResel;

   sib1Cfg->cellSelectInfo.qRxLevMin       = modSib1Cfg->cellSelectInfo.qRxLevMin;
   sib1Cfg->cellSelectInfo.qRxLevMinOffset.pres = TRUE;
   sib1Cfg->cellSelectInfo.qRxLevMinOffset.val = modSib1Cfg->cellSelectInfo.qRxLevMinOffset;
   if((lteeNodeBparams.lteCellSib1CfgGrp.cellSelectInfo.qRxLevMin != sib1Cfg->cellSelectInfo.qRxLevMin) || (lteeNodeBparams.lteCellSib1CfgGrp.cellSelectInfo.qRxLevMinOffset != sib1Cfg->cellSelectInfo.qRxLevMinOffset.val))
   {
      sib1Cfg->reCfgType |= LWR_CELL_SIB1_CFG_CELL_SELECT_INFO;
      lteeNodeBparams.lteCellSib1CfgGrp.cellSelectInfo.qRxLevMin = sib1Cfg->cellSelectInfo.qRxLevMin;
      lteeNodeBparams.lteCellSib1CfgGrp.cellSelectInfo.qRxLevMinOffset = sib1Cfg->cellSelectInfo.qRxLevMinOffset.val;
   }

   sib1Cfg->pMax        = modSib1Cfg->pMax;
   if(lteeNodeBparams.lteCellSib1CfgGrp.pMax != sib1Cfg->pMax)
   {
      sib1Cfg->reCfgType |= LWR_CELL_SIB1_CFG_PMAX;
      lteeNodeBparams.lteCellSib1CfgGrp.pMax = sib1Cfg->pMax;
   }

   sib1Cfg->freqBandInd = modSib1Cfg->freqBandInd;
   if(lteeNodeBparams.lteCellSib1CfgGrp.freqBandInd != sib1Cfg->freqBandInd)
   {
      sib1Cfg->reCfgType |= LWR_CELL_SIB1_CFG_FREQ_BAND_IND;
      lteeNodeBparams.lteCellSib1CfgGrp.freqBandInd = sib1Cfg->freqBandInd;
   }

   *dynCfg = (Void*)sib1Cfg;
   RETVALUE(ROK);      
} /* end of smHdlDynSib1Cfg */ 

/*
 *
 *        Fun:   smHdlDynSib6Cfg
 *
 *        Desc:  fill in cellCfg for WR
 *
 *        Ret:   ROK
 *
 *        Notes: None
 *
 *        File:  wr_smm_dyncfg.c
 */
#ifdef ANSI
PRIVATE S16 smHdlDynSib6Cfg
(
 MsmLteCellSib6CfgGrp *modSib6Cfg, Void **dynCfg
 )
#else /* if ANSI not defined*/
PRIVATE S16 smHdlDynSib6Cfg(modSib6Cfg, dynCfg)
    MsmLteCellSib6CfgGrp *modSib6Cfg;
    Void **dynCfg;
#endif /* end of ANSI*/
{
   LwrCellSib6CfgGrp *sib6Cfg=NULLP;

   TRC2(smHdlDynSib6Cfg);

   WR_ALLOC(&sib6Cfg, sizeof(LwrCellSib6CfgGrp));
   if(sib6Cfg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   sib6Cfg->tReselection      =  modSib6Cfg->tReselection;
   if(lteeNodeBparams.lteCellSib6CfgGrp.tReselection != sib6Cfg->tReselection) /*Check against old value*/
   {
      sib6Cfg->reCfgType |= LWR_UTRAN_SIB6_TRESELECT;
      lteeNodeBparams.lteCellSib6CfgGrp.tReselection = sib6Cfg->tReselection; /*Update the old value*/
   }

   sib6Cfg->tReselectionSfMedium   =  modSib6Cfg->tReselectionSfMedium;
   if(lteeNodeBparams.lteCellSib6CfgGrp.tReselectionSfMedium != sib6Cfg->tReselectionSfMedium) /*Check against old value*/
   {
      sib6Cfg->reCfgType |= LWR_UTRAN_SIB6_TRESELECT_SF_MEDIUM;
      lteeNodeBparams.lteCellSib6CfgGrp.tReselectionSfMedium = sib6Cfg->tReselectionSfMedium; /*Update the old value*/
   }

   sib6Cfg->tReselectionSfHigh      =  modSib6Cfg->tReselectionSfHigh;
   if(lteeNodeBparams.lteCellSib6CfgGrp.tReselectionSfHigh != sib6Cfg->tReselectionSfHigh) /*Check against old value*/
   {
      sib6Cfg->reCfgType |= LWR_UTRAN_SIB6_TRESELECT_SF_HIGH;
      lteeNodeBparams.lteCellSib6CfgGrp.tReselectionSfHigh = sib6Cfg->tReselectionSfHigh; /*Update the old value*/
   }

   *dynCfg = (Void*)sib6Cfg;
   RETVALUE(ROK);
} /* end of smHdlDynSib6Cfg */

/*ronin*/
/*
 *        Fun:  smFillRmuProtModCellCfgWithCsg 
 * 
 *        Desc:  fill in cellCfg for RMM with CSG info
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smFillRmuProtModCellCfgWithCsg
(
 LrmCellConfiguration *cellCfg,
 MsmLteFapAccessParams *cfg
)
#else /* if ANSI not defined*/
PRIVATE S16 smFillRmuProtModCellCfgWithCsg(cellCfg,cfg)
 LrmCellConfiguration *cellCfg;
 MsmLteFapAccessParams *cfg;
#endif /* end of ANSI*/
{

   TRC2(smFillRmuProtModCellCfgWithCsg);

   cellCfg->usReCfgType |= LRM_CELL_RECFGTYPE_CSG_PARAM;
   cellCfg->stCellCfgInfo.usMaxNonCsgUes    = cfg->maxCSGMembers; 
   cellCfg->stCellCfgInfo.usMaxCsgUes = cfg->maxNonCSGMembers;
   
   RETVALUE(ROK);      
} /* end of smFillRmuProtModCellCfgWithCsg*/


/*
 *        Fun:   smHdlDynFapAccsCfg
 * 
 *        Desc:  fill in cellCfg for WR
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smHdlDynFapAccsCfg
(
MsmLteFapAccessParams *cfg, 
Void **dynCfg
)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlDynFapAccsCfg(cellCfg, dynCfg)
MsmLteFapAccessParams *cfg;
Void **dynCfg;
#endif /* end of ANSI*/
{
   LwrHCsgParam   *lwrHcsgParam = NULLP;

   TRC2(smHdlDynFapAccsCfg);

   WR_ALLOC(&lwrHcsgParam, sizeof(LwrHCsgParam));

   if(lwrHcsgParam == NULLP)
   {
      RLOG0(L_FATAL, "Failed to allocate memory.");
      RETVALUE(RFAILED);      
   }
   lwrHcsgParam->maxCsgUeSup    = cfg->maxCSGMembers; 
   lwrHcsgParam->maxNonCsgUeSup = cfg->maxNonCSGMembers;
   *dynCfg = (Void *)lwrHcsgParam;
   RETVALUE(ROK);      
} /* end of smHdlDynFapAccsCfg */

/*
 * 
 *        Fun:   smHdlDynCellCfg
 * 
 *        Desc:  fill in cellCfg for WR
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smHdlDynCellCfg
(
MsmLteAddCellCfg  *cfg, 
Void **dynCfg
)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlDynCellCfg(cellCfg, dynCfg)
MsmLteAddCellCfg *cfg;
Void **dynCfg;
#endif /* end of ANSI*/
{
   LwrHCsgParam   *lwrHcsgParam = NULLP;

   TRC2(smHdlDynCellCfg);

   WR_ALLOC(&lwrHcsgParam, sizeof(LwrHCsgParam));

   if(lwrHcsgParam == NULLP)
   {
      RLOG0(L_FATAL, "Failed to allocate memory.");
      RETVALUE(RFAILED);      
   } 
   lwrHcsgParam->minDlResNonCsg = cfg->minDlResNonCsg; 
   lwrHcsgParam->minUlResNonCsg = cfg->minUlResNonCsg;
   *dynCfg = (Void *)lwrHcsgParam;
   RETVALUE(ROK);      
} /* end of smHdlDynCellCfg */

/*
 * 
 *        Fun:   smHdlDynEnbProtoCfg
 * 
 *        Desc:  fill in cellCfg for WR
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smHdlDynEnbProtoCfg
(
MsmLteEnbProtoCfg *enbProtoCfg, Void **dynCfg
)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlDynEnbProtoCfg(enbProtoCfg, dynCfg)
MsmLteEnbProtoCfg *enbProtoCfg;
Void **dynCfg;
#endif /* end of ANSI*/
{
   LwrEnbProtoCfg *lwrProtoCfg = NULLP;
   LwrENodeBCfg   *enbCfg = NULLP;
   MsgLen         namelen = 0;        

   TRC2(smHdlDynEnbProtoCfg);

   WR_ALLOC(&enbCfg, sizeof(LwrENodeBCfg));

   if(enbCfg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);      
   } 

   lwrProtoCfg = &(enbCfg->protoCfg);
   enbCfg->reCfgType = LWR_ENB_PROTOCFG;
    
   lwrProtoCfg->sctpPortNumS1 = enbProtoCfg->sctpPortNumS1;
   lwrProtoCfg->sctpPortNumX2 = enbProtoCfg->sctpPortNumX2;
   lwrProtoCfg->enbType       = enbProtoCfg->enbType;
   lwrProtoCfg->enbId         = enbProtoCfg->enbId;
   lwrProtoCfg->numPlmns      = enbProtoCfg->numPlmns;

   namelen = cmStrlen(enbProtoCfg->henbName);

   if(0 != namelen)
   {
      WR_ALLOC(&lwrProtoCfg->enbName.val, namelen);
   if(lwrProtoCfg->enbName.val == NULLP)
   {
         RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE(enbCfg, sizeof(LwrENodeBCfg));
      RETVALUE(RFAILED);      
   }
   cmMemcpy((U8 *) lwrProtoCfg->enbName.val, (U8 *) enbProtoCfg->henbName, namelen);
   }
   lwrProtoCfg->enbName.len = namelen;


/*  need to fill below IEs when MSM have them
   lwrProtoCfg->bhLinkCapacity
   lwrProtoCfg->enbIpAddr.type
   lwrProtoCfg->enbIpAddr.u.ipv4TptAddr.port
   lwrProtoCfg->enbIpAddr.u.ipv4TptAddr.address
   lwrProtoCfg->plmns
*/

   *dynCfg = (Void *)enbCfg;
   RETVALUE(ROK);      
} /* end of smHdlDynEnbProtoCfg */



/*
 * 
 *        Fun:   smHdlDynSib2Cfg
 * 
 *        Desc:  fill in SIB2 cellCfg for WR
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smHdlDynSib2Cfg
(
 U32 cfgType, Void *reCfgParams, Void **sib2CfgParms
)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlDynSib2Cfg(cfgType, reCfgParams, sib2CfgParms)
U32 cfgType;
Void *reCfgParams;
Void **sib2CfgParms;
#endif /* end of ANSI*/
{
   LwrCellSib2CfgGrp         *sib2Cfg     = NULLP;
   MsmLtePrachCfgCommon      *modPrachCfg = NULLP;
   MsmLteRachCfgCommonParams *modRachCfg  = NULLP;
   LwrPrachCfgCommon         *prachCfg    = NULLP;
   LwrRachCfgCommon          *rachCfg     = NULLP;

   TRC2(smHdlDynSib2Cfg);
 
   WR_ALLOC(&sib2Cfg, sizeof(LwrCellSib2CfgGrp));
   if(sib2Cfg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);      
   }
   switch(cfgType)
   {
      case MSM_LTE_PRACH_CONFIG_COMMON_PARAMETERS:
         {

            modPrachCfg = (MsmLtePrachCfgCommon*)reCfgParams; 
            prachCfg = &sib2Cfg->radioResourceCommon.prachCfgCommon;

            sib2Cfg->reCfgType = LWR_CELL_SIB2_CFG_RADIO_RSRC_CMN;

            sib2Cfg->radioResourceCommon.reCfgType = LWR_CELL_SIB2_RRSRCMN_PRACH_CFG_CMN; 

            prachCfg->rootSequenceIndx = modPrachCfg->rootSequenceIndx; 
            prachCfg->prachCfgInfo.prachCfgIdx = modPrachCfg->prachCfgInfo.prachCfgIdx;
            prachCfg->prachCfgInfo.highSpeedFlag = modPrachCfg->prachCfgInfo.highSpeedFlag;
            prachCfg->prachCfgInfo.zeroCorrelationZoneCfg = modPrachCfg->prachCfgInfo.zeroCorrelationZoneCfg;
            prachCfg->prachCfgInfo.prachFreqOffset = modPrachCfg->prachCfgInfo.prachFreqOffset;
            prachCfg->prachPosition = modPrachCfg->prachPosition; 
         }
         break;
      case MSM_LTE_RACH_CONFIG_COMMON_PARAMETERS:
         {

            modRachCfg =(MsmLteRachCfgCommonParams*)reCfgParams; 
            rachCfg= &sib2Cfg->radioResourceCommon.rachCfgCommon;

            sib2Cfg->reCfgType = LWR_CELL_SIB2_CFG_RADIO_RSRC_CMN;
            sib2Cfg->radioResourceCommon.reCfgType = LWR_CELL_SIB2_RRSRCMN_RACH_CFG_CMN;

            rachCfg->preambleCfg.numOfRAPreambles =  modRachCfg->ltePreambleCfg.numOfRAPreambles; 
            rachCfg->preambleCfg.groupASize = modRachCfg->ltePreambleCfg.groupASize;       
            rachCfg->preambleCfg.msgSizeGroupA = modRachCfg->ltePreambleCfg.msgSizeGroupA;    
            rachCfg->preambleCfg.pwrOffstGrpB = modRachCfg->ltePreambleCfg.pwrOffstGrpB;     

            rachCfg->powerRamping.powerRampingstep = modRachCfg->ltePowerRamping.powerRampingstep;
            rachCfg->powerRamping.initRcvdTrgtPwr  = modRachCfg->ltePowerRamping.initRcvdTrgtPwr;

            rachCfg->raSupervisionCfg.preambleTransMax = modRachCfg->lteRaSupervisionCfg.preambleTransMax;
            rachCfg->raSupervisionCfg.rarWindowSize    = modRachCfg->lteRaSupervisionCfg.rarWindowSize;
            rachCfg->raSupervisionCfg.macContResTimer  = modRachCfg->lteRaSupervisionCfg.macContResTimer;

            rachCfg->maxHARQMsg3 = modRachCfg->maxHARQMsg3;
         }
         break;
      case MSM_LTE_PDSCH_CONFIG_COMMON_PARAMETERS:
         {

            MsmLtePdschCfgCommon *modPdschCfg = (MsmLtePdschCfgCommon*)reCfgParams;

            sib2Cfg->reCfgType = LWR_CELL_SIB2_CFG_RADIO_RSRC_CMN;
            sib2Cfg->radioResourceCommon.reCfgType = LWR_CELL_SIB2_RRSRCMN_PDSCH_CFG_CMN;

            sib2Cfg->radioResourceCommon.pdschCfgCommon.referenceSignalPwr = modPdschCfg->referenceSignalPwr[0];
            sib2Cfg->radioResourceCommon.pdschCfgCommon.pB = modPdschCfg->pB[0];
         }
         break;
      case MSM_LTE_SRS_CFG_COMMON_PARAMETERS:
         {

            MsmLteSRSCfgCommon *modSrsCfgCm = (MsmLteSRSCfgCommon*)reCfgParams;
            LwrSRSCfgCommon    *srsCfgCommon = &sib2Cfg->radioResourceCommon.srsCfgCommon;

            sib2Cfg->reCfgType = LWR_CELL_SIB2_CFG_RADIO_RSRC_CMN;
            sib2Cfg->radioResourceCommon.reCfgType = LWR_CELL_SIB2_RRSRCMN_SRS_CFG_CMN;

            srsCfgCommon->cSrs = modSrsCfgCm->cSrs;
            /* srsCfgCommon->subframeCfg = modSrsCfgCm->subframeCfg;*/
            srsCfgCommon->simulAckNackSrs = modSrsCfgCm->simulAckNackSrs;
            srsCfgCommon->srsMaxUpts = modSrsCfgCm->srsMaxUpts;
         }
         break;
      case MSM_LTE_PUSCH_BASIC_CFG_COMMON_PARAMETERS:
         {

            LwrPuschCfgCommon *puschCfgCommon = &sib2Cfg->radioResourceCommon.puschCfgCommon; 
            MsmLtePuschBasicCfgCommon *modPuschBasicCfg = (MsmLtePuschBasicCfgCommon*)reCfgParams; 

            sib2Cfg->reCfgType = LWR_CELL_SIB2_CFG_RADIO_RSRC_CMN;
            sib2Cfg->radioResourceCommon.reCfgType = LWR_CELL_SIB2_RRSRCMN_PUSCH_CFG_CMN;
            puschCfgCommon->puschBasicCfg.reCfgType = LWR_CELL_SIB2_RRSRCMN_PUSCH_BASIC_CFG;

            puschCfgCommon->puschBasicCfg.nSb = modPuschBasicCfg->nSb;
            puschCfgCommon->puschBasicCfg.hoppingMode = modPuschBasicCfg->hoppingMode;
            puschCfgCommon->puschBasicCfg.hoppingOffset = modPuschBasicCfg->hoppingOffset;
            puschCfgCommon->puschBasicCfg.enable64QAM = modPuschBasicCfg->enable64QAM;
         }
         break;
      case MSM_LTE_PUSCH_REF_SIGNAL_CFG_PARAMETERS:
         {

            LwrPuschCfgCommon *puschCfgCommon = &sib2Cfg->radioResourceCommon.puschCfgCommon; 
            MsmLtePuschRefSignalCfg *modPuschCfgCm = (MsmLtePuschRefSignalCfg*)reCfgParams;

            sib2Cfg->reCfgType = LWR_CELL_SIB2_CFG_RADIO_RSRC_CMN;
            sib2Cfg->radioResourceCommon.reCfgType = LWR_CELL_SIB2_RRSRCMN_PUSCH_CFG_CMN;
            puschCfgCommon->puschRefSigCfg.reCfgType = LWR_CELL_SIB2_RRSRCMN_PUSCH_REFSIG_CFG;

            puschCfgCommon->puschRefSigCfg.grpHopEnabled = modPuschCfgCm->grpHopEnabled;
            puschCfgCommon->puschRefSigCfg.grpAsgnPusch = modPuschCfgCm->grpAsgnPusch;
            puschCfgCommon->puschRefSigCfg.seqHopEnabled = modPuschCfgCm->seqHopEnabled;
            puschCfgCommon->puschRefSigCfg.cyclicShift = modPuschCfgCm->cyclicShift;
         }
         break;
      case MSM_LTE_UL_POWER_CONTROL_COMMON_PARAMETERS:
         {

            MsmLteULPwrCtrlCommon *modPwrCtrlCmParam = (MsmLteULPwrCtrlCommon*)reCfgParams;
            LwrULPwrCtrlCommon  *pwrCtrlCmParam = &sib2Cfg->radioResourceCommon.ulPwrCtrlCommon;  

            sib2Cfg->reCfgType = LWR_CELL_SIB2_CFG_RADIO_RSRC_CMN;
            sib2Cfg->radioResourceCommon.reCfgType = LWR_CELL_SIB2_RRSRCMN_UL_PWR_CNRL_CFG_CMN;
            pwrCtrlCmParam->pNomialPusch = modPwrCtrlCmParam->pNomialPusch; 
            pwrCtrlCmParam->alpha = modPwrCtrlCmParam->alpha; 
            pwrCtrlCmParam->pNomialPucch = modPwrCtrlCmParam->pNomialPucch; 
            pwrCtrlCmParam->deltaMsg3 = modPwrCtrlCmParam->deltaMsg3; 

            pwrCtrlCmParam->deltaFLst.deltaPucchFormat1 = modPwrCtrlCmParam->deltaFLst.deltaPucchFormat1;
            pwrCtrlCmParam->deltaFLst.deltaPucchFormat1b = modPwrCtrlCmParam->deltaFLst.deltaPucchFormat1b; 
            pwrCtrlCmParam->deltaFLst.deltaPucchFormat2 = modPwrCtrlCmParam->deltaFLst.deltaPucchFormat2; 
            pwrCtrlCmParam->deltaFLst.deltaPucchFormat2a = modPwrCtrlCmParam->deltaFLst.deltaPucchFormat2a; 
            pwrCtrlCmParam->deltaFLst.deltaPucchFormat2b = modPwrCtrlCmParam->deltaFLst.deltaPucchFormat2b; 
         }
         break;
      case MSM_LTE_PUCCH_CFG_COMMON_PARAMETERS:
         {

            MsmLtePucchCfgCommon *modPucchCfg = (MsmLtePucchCfgCommon*)reCfgParams;
            LwrPucchCfgCommon *pucchCfg = &sib2Cfg->radioResourceCommon.pucchCfgCommon; 

            sib2Cfg->reCfgType = LWR_CELL_SIB2_CFG_RADIO_RSRC_CMN;
            sib2Cfg->radioResourceCommon.reCfgType = LWR_CELL_SIB2_RRSRCMN_PUCCH_CFG_CMN;

            pucchCfg->deltaShift = modPucchCfg->deltaShift; 
            pucchCfg->nRBCqi = modPucchCfg->nRBCqi; 
            pucchCfg->nCSAN = modPucchCfg->nCSAN; 
            pucchCfg->n1PucchAn = modPucchCfg->n1PucchAn; 
         }
         break;
      case MSM_LTE_UE_TIMER_CONSTANTS_PARAMETERS:
         {
            MsmLteUeTimerConstants *modUeTimerConstants = (MsmLteUeTimerConstants*)reCfgParams; 
            LwrUeTimerConstants   *ueTimerConstants = &sib2Cfg->ueTimerConstants;

            sib2Cfg->reCfgType = LWR_CELL_SIB2_CFG_UE_TMR_CONS;
            ueTimerConstants->t300 = modUeTimerConstants->t300;       
            ueTimerConstants->t301 = modUeTimerConstants->t301;       
            ueTimerConstants->t302 = modUeTimerConstants->t302;       
            ueTimerConstants->t304 = modUeTimerConstants->t304;       
            ueTimerConstants->ccoT304 = modUeTimerConstants->ccoT304;       
            ueTimerConstants->t310 = modUeTimerConstants->t310;       
            ueTimerConstants->n310 = modUeTimerConstants->n310;       
            ueTimerConstants->t311 = modUeTimerConstants->t311;       
            ueTimerConstants->n311 = modUeTimerConstants->n311;       
            ueTimerConstants->t320 = modUeTimerConstants->t320;       
         }
         break;
      case MSM_LTE_UL_FREQUENCY_INFO_PARAMETERS:
         {
            LwrULFrequencyInfo *ulFreqInfo = &sib2Cfg->ulFreqInfo; 
            MsmLteULFrequencyInfo *modUlFreqInfo = (MsmLteULFrequencyInfo*)reCfgParams;
            sib2Cfg->reCfgType = LWR_CELL_SIB2_CFG_UE_FREQ_INFO;

            ulFreqInfo->eARFCN = modUlFreqInfo->eARFCN; 
            ulFreqInfo->ulBandwidth = modUlFreqInfo->ulBandwidth; 
            /*       ulFreqInfo->addSpectEmission = modUlFreqInfo->addSpectEmission; */
         }
         break;
      default:
         {
            RLOG1(L_ERROR, "smHdlDynSib2Cfg: Invalid CfgType %lu", cfgType);
            WR_FREE(sib2Cfg, sizeof(LwrCellSib2CfgGrp));
            RETVALUE(RFAILED);      
         }
         break;
   }
   *sib2CfgParms = (Void*)sib2Cfg;
   RETVALUE(ROK);
} /* end of smHdlDynSib2Cfg */
 
/*
 * 
 *        Fun:   smHdlDynRabPolicyCfg
 * 
 *        Desc:  fill in cellCfg for WR
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smHdlDynRabPolicyCfg
(
 U32 cfgType, MsmLteRabPolicyCfgGrp *modRabPolicyCfg, Void **dynCfg
)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlDynRabPolicyCfg(cfgType, modRabPoliCfg, dynCfg)
U32 cfgType;
MsmLteRabPolicyCfgGrp *modRabPolicyCfg;
Void **dynCfg;
#endif /* end of ANSI*/
{
   LwrRabPolicyCfgGrp   *rabPolicyCfg=NULLP;
   LwrSrbPolicy         *srbPolicy = NULLP;
   MsmLteSrbPolicy      *msmSrbPolicy = NULLP;  

   TRC2(smHdlDynRabPolicyCfg);
 
   WR_ALLOC(&rabPolicyCfg, sizeof(LwrRabPolicyCfgGrp));
   if(rabPolicyCfg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);      
   }
   if(cfgType == MSM_LTE_RAB_SRB1_POLICY_CFG_GRP_PARAMETERS) 
   {
     msmSrbPolicy = &modRabPolicyCfg->srbPolicy[MSM_SRB_ID_1]; 
   }
   else if(cfgType ==MSM_LTE_RAB_SRB2_POLICY_CFG_GRP_PARAMETERS)
   {
     msmSrbPolicy = &modRabPolicyCfg->srbPolicy[MSM_SRB_ID_2];
   }
   srbPolicy =&rabPolicyCfg->srbPolicy[0];  
   rabPolicyCfg->reCfgType |= LWR_CELL_RAB_SRB_POLICY; 
   rabPolicyCfg->numSrbPolicies = 1;

   if(msmSrbPolicy == NULLP)
   {
      WR_FREE(rabPolicyCfg, sizeof(LwrRabPolicyCfgGrp));
      RETVALUE(RFAILED);      
   }

   srbPolicy->reCfgType  |= LWR_CELL_SRB_SRBID; 
   srbPolicy->srbId = msmSrbPolicy->srbId;

   srbPolicy->reCfgType  |= LWR_CELL_SRB_RLC_INFO;
   srbPolicy->rlcInfo.srbId = msmSrbPolicy->rlcInfo.srbId;
   srbPolicy->rlcInfo.tpRetransmit = msmSrbPolicy->rlcInfo.tpRetransmit;
   srbPolicy->rlcInfo.pollPdu = msmSrbPolicy->rlcInfo.pollPdu;
   srbPolicy->rlcInfo.pollByte = msmSrbPolicy->rlcInfo.pollByte;
   srbPolicy->rlcInfo.maxRetransThresh = msmSrbPolicy->rlcInfo.maxRetransThresh;
   srbPolicy->rlcInfo.timerReorderingDl = msmSrbPolicy->rlcInfo.timerReordering;
   srbPolicy->rlcInfo.timerReorderingUl = msmSrbPolicy->rlcInfo.timerReordering;
   srbPolicy->rlcInfo.timerStatusProhibit = msmSrbPolicy->rlcInfo.timerStatusProhibit;
   *dynCfg = (Void*)rabPolicyCfg;
   RETVALUE(ROK);      
} /* end of smHdlDynRabPolicyCfg */

/*
 * 
 *        Fun:   smHdlNeighUtraFreqCfgAdd 
 * 
 *        Desc:  
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smHdlNeighUtraFreqCfgAdd(SmNeigUtraFreqList *nghUtraFreqList)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlNeighUtraFreqCfgAdd(nghUtraFreqList)
SmNeigUtraFreqList *nghUtraFreqList;
#endif /* end of ANSI*/
{
   LwrNeighFreqCfg *addNghUtraFreq = NULLP;
   LwrNeighUtraFreq *utraFreq = NULLP;
   TRC2(smHdlNeighUtraFreqCfgAdd);

   WR_ALLOC(&addNghUtraFreq, sizeof(LwrNeighFreqCfg));
   if(NULLP == addNghUtraFreq)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   addNghUtraFreq->cellId = 1; 
   addNghUtraFreq->action = WR_ACT_ADD; 
   addNghUtraFreq->numFreq = 1; 
   addNghUtraFreq->t.addNeighFreq[0].type = WR_NEIGH_CELL_UTRA_FDD;
   utraFreq = &addNghUtraFreq->t.addNeighFreq[0].t.utranFreq;

   /*both are same structure we copy struture to struture*/
   *utraFreq = nghUtraFreqList->nghUtraFreqCfg;
   utraFreq->reCfgType    = wrSmDfltReCfgType; 

   smSendDynCfgReq(MSM_LTE_UTRA_NEIGH_FREQ_CFG_PARAMETERS,
                       LWR_DYN_EVNT_NEIGH_UTRA_FREQ_CFG, addNghUtraFreq);
   RETVALUE(ROK);
}/*end of smHdlNeighUtraFreqCfgAdd*/
/*
 * 
 *        Fun:   smHdlNeighUtraFreqCfgMod
 * 
 *        Desc:  
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smHdlNeighUtraFreqCfgMod(SmNeigUtraFreqList *nghUtraFreqList,
                                        MsmLteNeighUtraFreq *updatedFreqList)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlNeighUtraFreqCfgMod(nghUtraFreqList, updatedFreqList)
SmNeigUtraFreqList *nghUtraFreqList;
MsmLteNeighUtraFreq *updatedFreqList;
#endif /* end of ANSI*/
{
   LwrNeighFreqCfg *modNghUtraFreq = NULLP;
   LwrNeighUtraFreq *utraFreq = NULLP;
   U32 recfgTyep = 0;
   TRC2(smHdlNeighUtraFreqCfgMod);

   if(nghUtraFreqList->nghUtraFreqCfg.t.utraFdd.arfcnUl != updatedFreqList->arfcnUl)
   {
     recfgTyep |= LWR_NBR_UTRAN_FREQ_ARFCNUL;
     nghUtraFreqList->nghUtraFreqCfg.t.utraFdd.arfcnUl = updatedFreqList->arfcnUl;
   }
   if(nghUtraFreqList->nghUtraFreqCfg.priority.val != updatedFreqList->priority) 
   {
     recfgTyep |= LWR_NBR_UTRAN_FREQ_PRIORITY;
     nghUtraFreqList->nghUtraFreqCfg.priority.val = updatedFreqList->priority;
   }
   if(nghUtraFreqList->nghUtraFreqCfg.threshXhigh != updatedFreqList->threshXhigh) 
   {
     recfgTyep |= LWR_NBR_UTRAN_FREQ_THRESHXHIGH;
     nghUtraFreqList->nghUtraFreqCfg.threshXhigh = updatedFreqList->threshXhigh;
   }
   if(nghUtraFreqList->nghUtraFreqCfg.threshXlow != updatedFreqList->threshXlow)
   {
     recfgTyep |= LWR_NBR_UTRAN_FREQ_THRESHXLOW;
     nghUtraFreqList->nghUtraFreqCfg.threshXlow = updatedFreqList->threshXlow;
   }
   if(nghUtraFreqList->nghUtraFreqCfg.qRxLevMin != updatedFreqList->qRxLevMin)
   {
     recfgTyep |= LWR_NBR_UTRAN_FREQ_QRXLEVELMIN;
     nghUtraFreqList->nghUtraFreqCfg.qRxLevMin = updatedFreqList->qRxLevMin;
   }
   if(nghUtraFreqList->nghUtraFreqCfg.pMaxUtra != updatedFreqList->pMaxUtra)
   {
     recfgTyep |= LWR_NBR_UTRAN_FREQ_PMAXUTRA;
     nghUtraFreqList->nghUtraFreqCfg.pMaxUtra = updatedFreqList->pMaxUtra;
   }
   if(nghUtraFreqList->nghUtraFreqCfg.t.utraFdd.pQualMin != updatedFreqList->pQualMin)  
   {
     recfgTyep |= LWR_NBR_UTRAN_FREQ_PQUALMIN;
     nghUtraFreqList->nghUtraFreqCfg.t.utraFdd.pQualMin = updatedFreqList->pQualMin; 
   }
   if(nghUtraFreqList->nghUtraFreqCfg.offsetFreq != updatedFreqList->offsetFreq)
   {
     recfgTyep |= LWR_NBR_UTRAN_FREQ_OFFSETFREQ;
     nghUtraFreqList->nghUtraFreqCfg.offsetFreq= updatedFreqList->offsetFreq;
   }
   if(nghUtraFreqList->nghUtraFreqCfg.t.utraFdd.thresR9.thresXHighQR9 != updatedFreqList->threshXHighQR9)
   {
     recfgTyep |= LWR_NBR_UTRAN_FREQ_THRESHXHIGHQR9;
     nghUtraFreqList->nghUtraFreqCfg.t.utraFdd.thresR9.thresXHighQR9= updatedFreqList->threshXHighQR9;
   }
   if(nghUtraFreqList->nghUtraFreqCfg.t.utraFdd.thresR9.thresXLowQR9 != updatedFreqList->threshXLowQR9)
   {
     recfgTyep |= LWR_NBR_UTRAN_FREQ_THRESHXLOWQR9;
     nghUtraFreqList->nghUtraFreqCfg.t.utraFdd.thresR9.thresXLowQR9= updatedFreqList->threshXLowQR9;
   }
   if(nghUtraFreqList->nghUtraFreqCfg.duplexMode != updatedFreqList->duplexModeUtra)
   {
     recfgTyep |= LWR_NBR_UTRAN_FREQ_DUPLEXMODE;
     nghUtraFreqList->nghUtraFreqCfg.duplexMode= updatedFreqList->duplexModeUtra;
   }
   if(recfgTyep != 0)
   {
      WR_ALLOC(&modNghUtraFreq, sizeof(LwrNeighFreqCfg));
      if(NULLP == modNghUtraFreq)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      modNghUtraFreq->cellId = 1; 
      modNghUtraFreq->action = WR_ACT_MOD; 
      modNghUtraFreq->numFreq = 1; 
      modNghUtraFreq->t.addNeighFreq[0].type = WR_NEIGH_CELL_UTRA_FDD;
      utraFreq = &modNghUtraFreq->t.addNeighFreq[0].t.utranFreq;

     *utraFreq = nghUtraFreqList->nghUtraFreqCfg;
      utraFreq->reCfgType    = recfgTyep; 
      smSendDynCfgReq(MSM_LTE_UTRA_NEIGH_FREQ_CFG_PARAMETERS,
                       LWR_DYN_EVNT_NEIGH_UTRA_FREQ_CFG, modNghUtraFreq);
   }
   RETVALUE(ROK);
}/*end of smHdlNeighUtraFreqCfgMod*/

/*
 * 
 *        Fun:   smHdlNeighUtraFreqCfgDel 
 * 
 *        Desc:  
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smHdlNeighUtraFreqCfgDel()
#else /* if ANSI not defined*/
PRIVATE S16 smHdlNeighUtraFreqCfgDel()
#endif /* end of ANSI*/
{
   SmNeigUtraFreqList *utrafreqList;
   LwrNeighFreqCfg    *delNghUtraFreq = NULLP;
   PTR                 prevEntry = NULLP;
   PTR                 delArray[MAX_NUM_FREQ_CFG];
   U8                  fistFreqDel = FALSE;
   U16                 numNghDel= 0;
   U16                 delIdx= 0;

   TRC2(smHdlNeighUtraFreqCfgDel);
   while(ROK == cmHashListGetNext(&(smCfgCb.neighUtraFreqLstCp), prevEntry, (PTR *) &utrafreqList))
   {
      if(utrafreqList->dynCfgInd != WR_CFG_FOUND) 
      {
         if(fistFreqDel == FALSE)
         {
            WR_ALLOC(&delNghUtraFreq, sizeof(LwrNeighFreqCfg));
            if(NULLP == delNghUtraFreq)
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(RFAILED);
            }
            delNghUtraFreq->cellId = 1; 
            delNghUtraFreq->action = WR_ACT_DEL; 
            fistFreqDel = TRUE;
         }
         delNghUtraFreq->numFreq = numNghDel; 
         delNghUtraFreq->t.delNeighFreq[numNghDel].type = WR_NEIGH_CELL_UTRA_FDD;
         delNghUtraFreq->t.delNeighFreq[numNghDel].t.utranFreq.freqIndex = 
            utrafreqList->nghUtraFreqCfg.freqIndex;
         delNghUtraFreq->t.delNeighFreq[numNghDel].t.utranFreq.arfcn =
            utrafreqList->nghUtraFreqCfg.t.utraFdd.arfcnDl;
         delArray[numNghDel] = (PTR)utrafreqList;
         numNghDel++;
      }
      else
      {
         utrafreqList->dynCfgInd = WR_CFG_DONE;
      }
      prevEntry = (PTR )utrafreqList; 
   }
   if(fistFreqDel != FALSE)
   {
      smHdlNeighUtraDelCellWithFreq(delNghUtraFreq);

      smSendDynCfgReq(MSM_LTE_UTRA_NEIGH_FREQ_CFG_PARAMETERS,
            LWR_DYN_EVNT_NEIGH_UTRA_FREQ_CFG, delNghUtraFreq);
   }
   for(delIdx = 0; delIdx < numNghDel; delIdx++)
   {
      cmHashListDelete(&(smCfgCb.neighUtraFreqLstCp), delArray[delIdx]);
      WR_FREE(delArray[delIdx], sizeof(SmNeigUtraFreqList));
   }
   RETVALUE(ROK);
}/*end of smHdlNeighUtraFreqCfgDel*/


/*
 * 
 *        Fun:  smHdlDynNeighUtraFreqCfg 
 * 
 *        Desc: function to handal dynamic Utar frequency configuration
 *              if updated list contain new frequecny trigger add else
 *              check for is there any parameter within the frequcny configuration is modified and
 *              mark all the new frequency  and old freq which are part of updated list to WR_CFG_FOUND
 *              the freq which are already configured but not part of updated list are consider as deleted 
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smHdlDynNeighUtraFreqCfg(MsmLteNeighFreqCfg *updatedFreqList)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlDynNeighUtraFreqCfg(updatedFreqList)
MsmLteNeighFreqCfg *updatedFreqList;
#endif /* end of ANSI*/
{
   MsmLteNeighUtraFreq *utraFreqList = NULLP;
   SmNeigUtraFreqList  *nghUtraFreqList = NULLP;
   U32                  idx = 0;

   TRC2(smHdlDynNeighUtraFreqCfg);
   for(idx = 0; idx < updatedFreqList->numUtraFreq; idx++)
   {
      utraFreqList = &updatedFreqList->t.addNeighFreq[idx].utranFreq; 
      if(ROK == cmHashListFind(&(smCfgCb.neighUtraFreqLstCp), (U8*)&utraFreqList->arfcnDl,
                  sizeof(utraFreqList->arfcnDl), 0,(PTR*) &nghUtraFreqList))
      {
         /* Modify*/
         smHdlNeighUtraFreqCfgMod(nghUtraFreqList, utraFreqList);
         nghUtraFreqList->dynCfgInd = WR_CFG_FOUND;
      }
      else
      {
         /* Add */
         WR_ALLOC(&nghUtraFreqList, sizeof(SmNeigUtraFreqList));
         if(NULLP == nghUtraFreqList)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }

         smFillNghUtraFreqParam(&nghUtraFreqList->nghUtraFreqCfg, utraFreqList, wrSmDfltReCfgType);

         if(cmHashListInsert(&(smCfgCb.neighUtraFreqLstCp),
                  (PTR)nghUtraFreqList, (U8 *)&nghUtraFreqList->nghUtraFreqCfg.t.utraFdd.arfcnDl,
                    sizeof(nghUtraFreqList->nghUtraFreqCfg.t.utraFdd.arfcnDl)) != ROK)
         {
            RLOG1(L_ERROR, "cmHashListInsert failed arfcn = %d",
               nghUtraFreqList->nghUtraFreqCfg.t.utraFdd.arfcnDl);
            WR_FREE(nghUtraFreqList, sizeof(SmNeigUtraFreqList));
            RETVALUE(RFAILED);
         }
         smHdlNeighUtraFreqCfgAdd(nghUtraFreqList);
         nghUtraFreqList->dynCfgInd = WR_CFG_FOUND;
      }
   }
   smHdlNeighUtraFreqCfgDel();
   RETVALUE(ROK);
}/*end of smHdlDynNeighUtraFreqCfg*/

/*
 * 
 *        Fun:   smHdlNeighEutraFreqCfgAdd 
 * 
 *        Desc:  add new eutra frequency  
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smHdlNeighEutraFreqCfgAdd(SmNeigEutraFreqList *nghEutraFreqList)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlNeighEutraFreqCfgAdd(nghEutraFreqList)
SmNeigEutraFreqList *nghEutraFreqList;
#endif /* end of ANSI*/
{
   LwrNeighFreqCfg *addNeighFreqCfg = NULLP;
   LwrNeighEutraFreq *eutraFreqCfg = NULLP;
   TRC2(smHdlNeighEutraFreqCfgAdd);

   WR_ALLOC(&addNeighFreqCfg, sizeof(LwrNeighFreqCfg));
   if(NULLP == addNeighFreqCfg)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   addNeighFreqCfg->cellId = 1; 
   addNeighFreqCfg->action = WR_ACT_ADD; 
   addNeighFreqCfg->numFreq = 1; 
   addNeighFreqCfg->t.addNeighFreq[0].type = WR_NEIGH_CELL_EUTRA;
   eutraFreqCfg = &addNeighFreqCfg->t.addNeighFreq[0].t.eutranFreq;
   *eutraFreqCfg = nghEutraFreqList->nghEutraFreqCfg;

   smSendDynCfgReq(MSM_LTE_EUTRAN_NEIGH_FREQ_CFG_PARAMETERS,
                       LWR_DYN_EVNT_NEIGH_EUTRA_FREQ_CFG, addNeighFreqCfg);
   RETVALUE(ROK);
}/*end of smHdlNeighEutraFreqCfgAdd*/


/*
 * 
 *        Fun:  checkNeighEutraCellCfgValues
 * 
 *        Desc:  
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 checkNeighEutraCellCfgValues(LwrEUTRANeighCell *nghEutraAddCell, MsmEUTRANeighCell *updatedNhgEutraCell)
#else /* if ANSI not defined*/
PRIVATE S16 checkNeighEutraCellCfgValues(nghEutraAddCell, updatedNhgEutraCell)
LwrEUTRANeighCell *nghEutraAddCell;
MsmEUTRANeighCell *updatedNhgEutraCell;
#endif /* end of ANSI*/
{
   TRC2(checkNeighEutraCellCfgValues);

   if(nghEutraAddCell->pci != updatedNhgEutraCell->pci)
   {
       RETVALUE(RFAILED);
   }
   if(nghEutraAddCell->earfcnDl != updatedNhgEutraCell->earfcnDl)
   {
       RETVALUE(RFAILED);
   }
   if(nghEutraAddCell->blackListed!= updatedNhgEutraCell->BlackListed)
   {
       RETVALUE(RFAILED);
   }
   if(nghEutraAddCell->qoffset != updatedNhgEutraCell->qoffset)
   {
       RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}  /* End of checkNeighEutraCellCfgValues */
/*
 * 
 *        Fun:   smHdlNeighEutraFreqCfgMod
 * 
 *        Desc:  
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smHdlNeighEutraFreqCfgMod(SmNeigEutraFreqList *nghEutraFreqList, MsmLteNeighEutraFreq *modEutraFreqCfg)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlNeighEutraFreqCfgMod(nghEutraFreqList, modEutraFreqCfg)
SmNeigEutraFreqList *nghEutraFreqList;
MsmLteNeighEutraFreq *modEutraFreqCfg;
#endif /* end of ANSI*/
{
   LwrNeighFreqCfg   *modNghEutraFreq = NULLP;
   LwrNeighEutraFreq *eutraFreqCfg = NULLP;
   U32 recfgTyep = 0;

   TRC2(smHdlNeighEutraFreqCfgMod);

   /*Inter Frequency Modifcation*/
   if( lteeNodeBparams.lteAddCellCfg.earfdnDl != nghEutraFreqList->nghEutraFreqCfg.earfcnDl)
   {
      RLOG0(L_DEBUG,"Inter Frequency Modification");
      if(nghEutraFreqList->nghEutraFreqCfg.earfcnUl != modEutraFreqCfg->earfcnUl)
      {
         recfgTyep |= LWR_NBR_EUTRAN_FREQ_EARFCNUL;
         nghEutraFreqList->nghEutraFreqCfg.earfcnUl = modEutraFreqCfg->earfcnUl;
      }
      if(nghEutraFreqList->nghEutraFreqCfg.priority != modEutraFreqCfg->priority)
      {
        recfgTyep |= LWR_NBR_EUTRAN_FREQ_PRIORITY;
        nghEutraFreqList->nghEutraFreqCfg.priority = modEutraFreqCfg->priority;
      }
      if(nghEutraFreqList->nghEutraFreqCfg.qRxLevMin != modEutraFreqCfg->qRxLevMin)
      {
        recfgTyep |= LWR_NBR_EUTRAN_FREQ_QRXLEVMIN;
        nghEutraFreqList->nghEutraFreqCfg.qRxLevMin = modEutraFreqCfg->qRxLevMin;
      }
      if(nghEutraFreqList->nghEutraFreqCfg.pMax != modEutraFreqCfg->pMax)
      {
        recfgTyep |= LWR_NBR_EUTRAN_FREQ_PMAX;
        nghEutraFreqList->nghEutraFreqCfg.pMax = modEutraFreqCfg->pMax;
      }
      if(nghEutraFreqList->nghEutraFreqCfg.tReselection != modEutraFreqCfg->tReselection)
      {
        recfgTyep |= LWR_NBR_EUTRAN_FREQ_TRESELECTION;
        nghEutraFreqList->nghEutraFreqCfg.tReselection = modEutraFreqCfg->tReselection;
      }
      if(nghEutraFreqList->nghEutraFreqCfg.tReselectionSFMed != modEutraFreqCfg->tReselectionSFMed)
      {
        recfgTyep |= LWR_NBR_EUTRAN_FREQ_TRESELECTIONSFMED;
        nghEutraFreqList->nghEutraFreqCfg.tReselectionSFMed = modEutraFreqCfg->tReselectionSFMed;
      }
      if(nghEutraFreqList->nghEutraFreqCfg.tReselectionSFHigh != modEutraFreqCfg->tReselectionSFHigh)
      {
        recfgTyep |= LWR_NBR_EUTRAN_FREQ_TRESELECTIONSFHIGH;
        nghEutraFreqList->nghEutraFreqCfg.tReselectionSFHigh = modEutraFreqCfg->tReselectionSFHigh;
      }
      if(nghEutraFreqList->nghEutraFreqCfg.sIntraSearch.val != modEutraFreqCfg->sIntraSearch)
      {
        recfgTyep |= LWR_NBR_EUTRAN_FREQ_SINTRASEARCH;
        nghEutraFreqList->nghEutraFreqCfg.sIntraSearch.val = modEutraFreqCfg->sIntraSearch;
      }
      if(nghEutraFreqList->nghEutraFreqCfg.threshXHigh != modEutraFreqCfg->threshXHigh)
      {
        recfgTyep |= LWR_NBR_EUTRAN_FREQ_THRESHXHIGH;
        nghEutraFreqList->nghEutraFreqCfg.threshXHigh = modEutraFreqCfg->threshXHigh;
      }
      if(nghEutraFreqList->nghEutraFreqCfg.threshXLow != modEutraFreqCfg->threshXLow)
      {
        recfgTyep |= LWR_NBR_EUTRAN_FREQ_THRESHXLOW;

        nghEutraFreqList->nghEutraFreqCfg.threshXLow = modEutraFreqCfg->threshXLow;
      }
      if(nghEutraFreqList->nghEutraFreqCfg.qOffsetFreq != modEutraFreqCfg->qOffsetFreq)
      {
        recfgTyep |= LWR_NBR_EUTRAN_FREQ_OFFSETFREQ;
        nghEutraFreqList->nghEutraFreqCfg.qOffsetFreq = modEutraFreqCfg->qOffsetFreq;
      }
      if(nghEutraFreqList->nghEutraFreqCfg.presAntennaPort1 != modEutraFreqCfg->presAntennaPort1)
      {
        recfgTyep |= LWR_NBR_EUTRAN_FREQ_PRESANTENNAPORT1;
        nghEutraFreqList->nghEutraFreqCfg.presAntennaPort1 = modEutraFreqCfg->presAntennaPort1;
      }
      if(nghEutraFreqList->nghEutraFreqCfg.measBandwidth.val != modEutraFreqCfg->measBandwidth)
      {
        recfgTyep |= LWR_NBR_EUTRAN_FREQ_MEASBANDWIDTH;
        nghEutraFreqList->nghEutraFreqCfg.measBandwidth.val = modEutraFreqCfg->measBandwidth;
      }
      if(nghEutraFreqList->nghEutraFreqCfg.qQualMinRel9.val != modEutraFreqCfg->qQualMinRel9)
      {
        recfgTyep |= LWR_NBR_EUTRAN_FREQ_QQUALMINREL9;
        nghEutraFreqList->nghEutraFreqCfg.qQualMinRel9.val = modEutraFreqCfg->qQualMinRel9;
      } 
      if(nghEutraFreqList->nghEutraFreqCfg.threshXQRel9.threshXHighQR9 != modEutraFreqCfg->threshXHighQR9)
      {
        recfgTyep |= LWR_NBR_EUTRAN_FREQ_THRESHXQREL9HIGH;
        nghEutraFreqList->nghEutraFreqCfg.threshXQRel9.threshXHighQR9 = modEutraFreqCfg->threshXHighQR9;
      }
      if(nghEutraFreqList->nghEutraFreqCfg.threshXQRel9.threshXLowQR9 != modEutraFreqCfg->threshXLowQR9)
      {
        recfgTyep |= LWR_NBR_EUTRAN_FREQ_THRESHXQREL9LOW;
        nghEutraFreqList->nghEutraFreqCfg.threshXQRel9.threshXLowQR9 = modEutraFreqCfg->threshXLowQR9;
      }
  }
  else
  {
      /*Intra Frequency Modifcation*/
      RLOG0(L_DEBUG,"Intra Frequency Modification");
      if(nghEutraFreqList->nghEutraFreqCfg.qRxLevMin != modEutraFreqCfg->qRxLevMin)
      {
        recfgTyep |= LWR_NBR_EUTRAN_FREQ_QRXLEVMIN;
        nghEutraFreqList->nghEutraFreqCfg.qRxLevMin = modEutraFreqCfg->qRxLevMin;
      }
      if(nghEutraFreqList->nghEutraFreqCfg.pMax != modEutraFreqCfg->pMax)
      {
        recfgTyep |= LWR_NBR_EUTRAN_FREQ_PMAX;
        nghEutraFreqList->nghEutraFreqCfg.pMax = modEutraFreqCfg->pMax;
      }
      if(nghEutraFreqList->nghEutraFreqCfg.tReselectionSFMed != modEutraFreqCfg->tReselectionSFMed)
      {
        recfgTyep |= LWR_NBR_EUTRAN_FREQ_TRESELECTIONSFMED;
        nghEutraFreqList->nghEutraFreqCfg.tReselectionSFMed = modEutraFreqCfg->tReselectionSFMed;
      }
      if(nghEutraFreqList->nghEutraFreqCfg.tReselectionSFHigh != modEutraFreqCfg->tReselectionSFHigh)
      {
        recfgTyep |= LWR_NBR_EUTRAN_FREQ_TRESELECTIONSFHIGH;
        nghEutraFreqList->nghEutraFreqCfg.tReselectionSFHigh = modEutraFreqCfg->tReselectionSFHigh;
      }
      if(nghEutraFreqList->nghEutraFreqCfg.sNonIntraSearch.val != modEutraFreqCfg->sNonIntraSearch)
      {
        recfgTyep |= LWR_NBR_EUTRAN_FREQ_SNONINTRASEARCH;
        nghEutraFreqList->nghEutraFreqCfg.sNonIntraSearch.val = modEutraFreqCfg->sNonIntraSearch;
      }
      if(nghEutraFreqList->nghEutraFreqCfg.servThresholdLow != modEutraFreqCfg->servThresholdLow)
      {
        recfgTyep |= LWR_NBR_EUTRAN_FREQ_SERVTHRESHOLDLOW;
        nghEutraFreqList->nghEutraFreqCfg.servThresholdLow = modEutraFreqCfg->servThresholdLow;
      }
      if(nghEutraFreqList->nghEutraFreqCfg.priority != modEutraFreqCfg->priority)
      {
        recfgTyep |= LWR_NBR_EUTRAN_FREQ_PRIORITY;
        nghEutraFreqList->nghEutraFreqCfg.priority = modEutraFreqCfg->priority;
      }    
  }
  if(recfgTyep != 0)
  {
      WR_ALLOC(&modNghEutraFreq, sizeof(LwrNeighFreqCfg));
      if(NULLP == modNghEutraFreq)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      modNghEutraFreq->cellId = 1; 
      modNghEutraFreq->action = WR_ACT_MOD; 
      modNghEutraFreq->numFreq = 1; 
      modNghEutraFreq->t.addNeighFreq[0].type = WR_NEIGH_CELL_EUTRA;
      eutraFreqCfg = &modNghEutraFreq->t.addNeighFreq[0].t.eutranFreq;
      *eutraFreqCfg = nghEutraFreqList->nghEutraFreqCfg;
      eutraFreqCfg->reCfgType = recfgTyep;
      smSendDynCfgReq(MSM_LTE_EUTRAN_NEIGH_FREQ_CFG_PARAMETERS,
                       LWR_DYN_EVNT_NEIGH_EUTRA_FREQ_CFG, modNghEutraFreq);

  }
  else
  {
      RETVALUE(RFAILED);
  }
  RETVALUE(ROK);
}/*end of smHdlNeighEutraFreqCfgMod*/


/*
 * 
 *        Fun:   smHdlNeighEutraFreqCfgDel
 * 
 *        Desc:  freq which are not part up updated list considered for the delete
 *               before delete freq SM will trigger delete cell for all the cells
 *               which are configured with same freq
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smHdlNeighEutraFreqCfgDel(SmNeigEutraFreqList *nghEutraFreqList)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlNeighEutraFreqCfgDel(nghEutraFreqList)
SmNeigEutraFreqList *nghEutraFreqList;   
#endif /* end of ANSI*/
{
   LwrNeighFreqCfg     *delNghEutraFreq = NULLP;
   TRC2(smHdlNeighEutraFreqCfgDel);
   WR_ALLOC(&delNghEutraFreq, sizeof(LwrNeighFreqCfg));
   if(NULLP == delNghEutraFreq)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   delNghEutraFreq->cellId = 1; 
   delNghEutraFreq->action = WR_ACT_DEL; 
   delNghEutraFreq->t.delNeighFreq[0].type = WR_NEIGH_CELL_EUTRA;
   delNghEutraFreq->t.delNeighFreq[0].t.eutranFreq.freqIndex = 
      nghEutraFreqList->nghEutraFreqCfg.freqIndex;
   delNghEutraFreq->t.delNeighFreq[0].t.eutranFreq.earfcn = 
      nghEutraFreqList->nghEutraFreqCfg.earfcnDl;
   delNghEutraFreq->numFreq = 1;
   /*Delete freq which is received as part of updated list*/
   smSendDynCfgReq(MSM_LTE_EUTRAN_NEIGH_FREQ_CFG_PARAMETERS,
         LWR_DYN_EVNT_NEIGH_EUTRA_FREQ_CFG, delNghEutraFreq);
   RETVALUE(ROK);
}/*end of smHdlNeighEutraFreqCfgDel*/


/*
 * 
 *        Fun: smHdlDynNeighEutraFreqCfg 
 * 
 *        Desc:function to handal dynamic Eutar frequency configuration
 *             if updated list contain new frequecny trigger add else
 *             check for is there any parameter within the frequcny configuration is modified and
 *             mark all the new frequency  and old freq which are part of updated list to WR_CFG_FOUND
 *             the freq which are already configured but not part of updated list are consider as deleted  
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smHdlDynNeighEutraFreqCfg(MsmLteNeighFreqCfg *updatedFreqList)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlDynNeighEutraFreqCfg(updatedFreqList)
MsmLteNeighFreqCfg *updatedFreqList;
#endif /* end of ANSI*/
{
   MsmLteNeighEutraFreq *eutraFreqList = NULLP;
   MsmLteNeighEutraDelFreq *eutraDelFreqList = NULLP;
   SmNeigEutraFreqList  *nghEutraFreqList = NULLP;
   U8                    nodeFound = FALSE;

   TRC2(smHdlDynNeighEutraFreqCfg);
   if (MSM_DYN_ADD == updatedFreqList->actType)
   {
      eutraFreqList = &updatedFreqList->t.dynNeighFreq.eutranFreq;
      if (lteeNodeBparams.lteNeighFreqCfg.numEutraFreq == MAX_NUM_FREQ_CFG)
      {
            MsmDynCfmInfo dynCfmInfo;
            RLOG0(L_WARNING, "Max number of the frequency configurations already reached.");

            dynCfmInfo.cfgType = MSM_LTE_EUTRAN_NEIGH_FREQ_CFG_PARAMETERS; 
            dynCfmInfo.action = MSM_DYN_ADD;
            dynCfmInfo.u.neighFreqIndx = eutraFreqList->freqIndex;

            msmDynConfigComplete(MSM_CFM_FAILURE,MSM_CAUSE_CFG_FAIL,dynCfmInfo);
            RLOG0(L_ERROR, "[ALARM] Sending NEIGH_FREQ_CFG_FAIL to OAM");
            FapAlarmInd( NEIGH_FREQ_CFG_FAIL, SM_ALARM_SEVERITY_MAJOR, SM_ALARM_CAUSE_TYPE_ERR_IND, SM_ALARM_CAUSE_VAL_ABNORMAL);
            RETVALUE(ROK);

      }
      if(ROK == cmHashListFind(&(smCfgCb.neighEutraFreqLstCp), (U8*)&eutraFreqList->earfcnDl,
               sizeof(eutraFreqList->earfcnDl), 0,(PTR*) &nghEutraFreqList))
      {
            MsmDynCfmInfo dynCfmInfo;
            RLOG0(L_WARNING, "Frequency configuration already exists.");

            dynCfmInfo.cfgType = MSM_LTE_EUTRAN_NEIGH_FREQ_CFG_PARAMETERS; 
            dynCfmInfo.action = MSM_DYN_ADD;
            dynCfmInfo.u.neighFreqIndx = eutraFreqList->freqIndex;

            msmDynConfigComplete(MSM_CFM_FAILURE,MSM_CAUSE_CFG_FAIL,dynCfmInfo);
            RLOG0(L_ERROR, "[ALARM] Sending NEIGH_FREQ_CFG_FAIL to OAM");
            FapAlarmInd( NEIGH_FREQ_CFG_FAIL, SM_ALARM_SEVERITY_MAJOR, SM_ALARM_CAUSE_TYPE_ERR_IND, SM_ALARM_CAUSE_VAL_ABNORMAL);
            RETVALUE(ROK);
      }
      WR_ALLOC(&nghEutraFreqList, sizeof(SmNeigEutraFreqList));
      if(NULLP == nghEutraFreqList)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }

      smFillNghEutraFreqParam(&nghEutraFreqList->nghEutraFreqCfg, eutraFreqList, wrSmDfltReCfgType);

      if(cmHashListInsert(&(smCfgCb.neighEutraFreqLstCp),
               (PTR)nghEutraFreqList, (U8 *)&nghEutraFreqList->nghEutraFreqCfg.earfcnDl,
               sizeof(nghEutraFreqList->nghEutraFreqCfg.earfcnDl)) != ROK)
      {
         RLOG1(L_ERROR, "cmHashListInsert failed earfcn = %d",
               nghEutraFreqList->nghEutraFreqCfg.earfcnDl);
         WR_FREE(nghEutraFreqList, sizeof(SmNeigEutraFreqList));
         RETVALUE(RFAILED);
      }
      nghEutraFreqList->dynCfgInd = WR_CFG_ADD;
      smHdlNeighEutraFreqCfgAdd(nghEutraFreqList);
   }
   else if (MSM_ACT_MOD == updatedFreqList->actType)
   {
      eutraFreqList = &updatedFreqList->t.dynNeighFreq.eutranFreq; 
      if(ROK == cmHashListFind(&(smCfgCb.neighEutraFreqLstCp), (U8*)&eutraFreqList->earfcnDl,
               sizeof(eutraFreqList->earfcnDl), 0,(PTR*) &nghEutraFreqList))
      {
         nghEutraFreqList->dynCfgInd = WR_CFG_MOD;
         if (ROK != smHdlNeighEutraFreqCfgMod(nghEutraFreqList, eutraFreqList))
         {
            MsmDynCfmInfo dynCfmInfo;

            dynCfmInfo.cfgType = MSM_LTE_EUTRAN_NEIGH_FREQ_CFG_PARAMETERS; 
            dynCfmInfo.action = MSM_ACT_MOD;
            dynCfmInfo.u.neighFreqIndx = eutraFreqList->freqIndex;

            msmDynConfigComplete(MSM_CFM_SUCCESS,MSM_CAUSE_NA,dynCfmInfo);

            RETVALUE(ROK);
         }
      }
      else
      {
         MsmDynCfmInfo dynCfmInfo;
         RLOG0(L_WARNING, "Failed to get the frequency node from frequency list.");

         dynCfmInfo.cfgType = MSM_LTE_EUTRAN_NEIGH_FREQ_CFG_PARAMETERS; 
         dynCfmInfo.action = MSM_ACT_MOD;
         dynCfmInfo.u.neighFreqIndx = eutraFreqList->freqIndex;

         msmDynConfigComplete(MSM_CFM_FAILURE,MSM_CAUSE_CFG_FAIL,dynCfmInfo);
         RLOG0(L_ERROR, "[ALARM] Sending NEIGH_FREQ_CFG_FAIL to OAM");
         FapAlarmInd( NEIGH_FREQ_CFG_FAIL, SM_ALARM_SEVERITY_MAJOR, SM_ALARM_CAUSE_TYPE_ERR_IND, SM_ALARM_CAUSE_VAL_ABNORMAL);
         RETVALUE(ROK);
      }
   }
   else if (MSM_ACT_DEL == updatedFreqList->actType)
   {
      PTR                  prevFreqEntry = NULLP;
      PTR                  prevCellEntry = NULLP;
      SmNeigEutraCellList *nghEutraCellList = NULLP;
      eutraDelFreqList = &updatedFreqList->t.delNeighFreq.eutranFreq; 
      while(ROK == cmHashListGetNext(&(smCfgCb.neighEutraFreqLstCp), prevFreqEntry, (PTR *)&nghEutraFreqList))
      {
         if((nghEutraFreqList->nghEutraFreqCfg.freqIndex == eutraDelFreqList->freqIndex)&&
            (lteeNodeBparams.lteAddCellCfg.earfdnDl != nghEutraFreqList->nghEutraFreqCfg.earfcnDl))
         {
            nodeFound = TRUE;
            while(ROK == cmHashListGetNext(&(smCfgCb.neighEutraCellLstCp), prevCellEntry, (PTR *)&nghEutraCellList))
            {
               if(nghEutraCellList->nghEutraCellCfg.earfcnDl == nghEutraFreqList->nghEutraFreqCfg.earfcnDl)
               {
                  nodeFound = FALSE;
                  break;
               }
	           prevCellEntry =(PTR)nghEutraCellList;
            }
            break;
         }
	     prevFreqEntry = (PTR)nghEutraFreqList;
      }
      if (nodeFound == TRUE)
      {
         nghEutraFreqList->dynCfgInd = WR_CFG_DEL;
         smHdlNeighEutraFreqCfgDel(nghEutraFreqList);
      }
      else
      {
         MsmDynCfmInfo dynCfmInfo;
         RLOG0(L_WARNING, "Failed to get the frequency node from the frequency list.");

         dynCfmInfo.cfgType = MSM_LTE_EUTRAN_NEIGH_FREQ_CFG_PARAMETERS; 
         dynCfmInfo.action = MSM_ACT_DEL;
         dynCfmInfo.u.neighFreqIndx = eutraDelFreqList->freqIndex;

         msmDynConfigComplete(MSM_CFM_SUCCESS,MSM_CAUSE_NA,dynCfmInfo);

         RETVALUE(ROK);
      }
   }
   else
   {
      RLOG0(L_WARNING, "MSM Action type is not valid");
   }   
   RETVALUE(ROK);
}/*end of smHdlDynNeighEutraFreqCfg*/

/*
 * 
 *        Fun:smHdlNeighUtraCellCfgAdd 
 * 
 *        Desc:  
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PUBLIC S16 smHdlNeighUtraCellCfgAdd(LwrUtraNeighCell *updateddUtraCellCfg)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlNeighUtraCellCfgAdd(updateddUtraCellCfg)
LwrUtraNeighCell *updateddUtraCellCfg;
#endif /* end of ANSI*/
{
  LwrNeighCellCfg  *addNghUtraCellCfg = NULLP;
  TRC2(smHdlNeighUtraCellCfgAdd);

  WR_ALLOC(&addNghUtraCellCfg, sizeof(LwrNeighCellCfg));
  if(NULLP == addNghUtraCellCfg)
  {
     RLOG0(L_FATAL, "Memory allocation failed.");
     RETVALUE(RFAILED);
  }
  addNghUtraCellCfg->cellId = 1; 
  addNghUtraCellCfg->action = WR_ACT_ADD; 
  addNghUtraCellCfg->numCells = 1; 
  addNghUtraCellCfg->t.neighAddCfg[0].type = WR_NEIGH_CELL_UTRA_FDD;
  addNghUtraCellCfg->t.neighAddCfg[0].t.utranCell = *updateddUtraCellCfg; 
    /*need to ckeck structure to structure copy workes or not*/
  smSendDynCfgReq(MSM_LTE_UTRA_NEIGHBORLIST_IN_USE_PARAMETERS,
                      LWR_DYN_EVNT_NEIGH_UTRA_CELL_CFG, addNghUtraCellCfg);
  RETVALUE(ROK);
}/*end of smHdlNeighUtraCellCfgAdd*/
/*
 * 
 *        Fun:smHdlNeighUtraCellCfgMod 
 * 
 *        Desc:  
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smHdlNeighUtraCellCfgMod(LwrUtraNeighCell *utraCellCfg,
                                  MsmUtraNeighCell *updatedUtraCellCfg)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlNeighUtraCellCfgMod()
LwrUtraNeighCell *utraCellCfg;
MsmUtraNeighCell *updatedUtraCellCfg;
#endif /* end of ANSI*/
{
   LwrNeighCellCfg  *modNghUtraCellCfg = NULLP;
   LwrUtraNeighCell *nrUtraCellCfg = NULLP;
   U32 recfgTyep = 0;
   TRC2(smHdlNeighUtraCellCfgMod);
   if(utraCellCfg->rncId != updatedUtraCellCfg->rncId)
   {
     recfgTyep |= LWR_NBR_UTRAN_CELL_RNCID;
     utraCellCfg->rncId = updatedUtraCellCfg->rncId;
   } 
   if(utraCellCfg->lac != updatedUtraCellCfg->lac)
   {
     recfgTyep |= LWR_NBR_UTRAN_CELL_LAC;
     utraCellCfg->lac = updatedUtraCellCfg->lac;
   } 
   if(utraCellCfg->rac != updatedUtraCellCfg->rac)
   {
     recfgTyep |= LWR_NBR_UTRAN_CELL_RAC;
     utraCellCfg->rac = updatedUtraCellCfg->rac;
   } 
   if(utraCellCfg->t.utraFdd.psc != updatedUtraCellCfg->psc)
   {
     recfgTyep |= LWR_NBR_UTRAN_CELL_PSCMCODE;
     utraCellCfg->t.utraFdd.psc = updatedUtraCellCfg->psc;
   } 
   if(utraCellCfg->t.utraFdd.arfcnDl != updatedUtraCellCfg->arfcnDl)
   {
     recfgTyep |= LWR_NBR_UTRAN_CELL_ARFCN_DL;
     utraCellCfg->t.utraFdd.arfcnDl = updatedUtraCellCfg->arfcnDl;
   } 
   if(utraCellCfg->t.utraFdd.arfcnUl != updatedUtraCellCfg->arfcnUl)
   {
     recfgTyep |= LWR_NBR_UTRAN_CELL_ARFCN_UL;
     utraCellCfg->t.utraFdd.arfcnUl = updatedUtraCellCfg->arfcnUl;
   } 
   if(recfgTyep != 0)
   {
      WR_ALLOC(&modNghUtraCellCfg, sizeof(LwrNeighCellCfg));
      if(NULLP == modNghUtraCellCfg)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      modNghUtraCellCfg->cellId = 1; 
      modNghUtraCellCfg->action = WR_ACT_MOD; 
      modNghUtraCellCfg->numCells = 1; 
      modNghUtraCellCfg->t.neighAddCfg[0].type = WR_NEIGH_CELL_UTRA_FDD;
      nrUtraCellCfg = &modNghUtraCellCfg->t.neighAddCfg[0].t.utranCell;
      *nrUtraCellCfg = *utraCellCfg;
      nrUtraCellCfg->reCfgType = recfgTyep; 

      smSendDynCfgReq(MSM_LTE_UTRA_NEIGHBORLIST_IN_USE_PARAMETERS,
                       LWR_DYN_EVNT_NEIGH_UTRA_CELL_CFG, modNghUtraCellCfg);
   }
   RETVALUE(ROK);
}/*end of smHdlNeighUtraCellCfgMod*/
/*
 * 
 *        Fun:  smHdlNeighUtraDelCellWithFreq 
 * 
 *        Desc:  
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smHdlNeighUtraDelCellWithFreq(LwrNeighFreqCfg *delNghUtraFreq)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlNeighUtraDelCellWithFreq(delNghUtraFreq)
LwrNeighFreqCfg *delNghUtraFreq;
#endif /* end of ANSI*/
{
   SmNeigUtraCellList *utraCellList = NULLP;
   LwrNeighCellCfg    *delNghUtraCellCfg = NULLP;  
   PTR                 prevEntry = NULLP;
   PTR                 delArray[MAX_NUM_NEIGH_CELLS];
   U32                 idx = 0;
   U16                 numNghDel= 0;
   U8                  delFlag = FALSE;

   TRC2(smHdlNeighUtraDelCellWithFreq);
   while(ROK == cmHashListGetNext(&(smCfgCb.neighUtraCellLstCp), prevEntry, (PTR *)&utraCellList))
   {
     for(idx = 0; idx < delNghUtraFreq->numFreq; idx++)
     {
       if(utraCellList->nghUtraCellCfg.t.utraFdd.arfcnDl == 
                   delNghUtraFreq->t.delNeighFreq[idx].t.utranFreq.arfcn)
       {
          if(delFlag == FALSE)
          {
            WR_ALLOC(&delNghUtraCellCfg, sizeof(LwrNeighCellCfg));
            if(NULLP == delNghUtraCellCfg)
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(RFAILED);
            }
            delNghUtraCellCfg->cellId = 1; 
            delNghUtraCellCfg->action = WR_ACT_DEL; 
            delFlag = TRUE;
          }
          delNghUtraCellCfg->t.neighDelCfg[numNghDel].type = WR_NEIGH_CELL_UTRA_FDD;
          delNghUtraCellCfg->t.neighDelCfg[numNghDel].t.utranCell.nrIndex = 
                                                      utraCellList->nghUtraCellCfg.nrIndex;
          delNghUtraCellCfg->t.neighDelCfg[numNghDel].t.utranCell.arfcn   = 
                                                      utraCellList->nghUtraCellCfg.t.utraFdd.arfcnDl;
          delNghUtraCellCfg->t.neighDelCfg[numNghDel].t.utranCell.psc     = 
                                                        utraCellList->nghUtraCellCfg.t.utraFdd.psc;
          delNghUtraCellCfg->t.neighDelCfg[numNghDel].t.utranCell.cid =  
                                                         utraCellList->nghUtraCellCfg.cId;

          delNghUtraCellCfg->t.neighDelCfg[numNghDel].t.utranCell.plmnId.numMncDigits =
                                         utraCellList->nghUtraCellCfg.plmnId[0].numMncDigits;
          delNghUtraCellCfg->t.neighDelCfg[numNghDel].t.utranCell.plmnId.mcc[0] =
                                          utraCellList->nghUtraCellCfg.plmnId[0].mcc[0];
          delNghUtraCellCfg->t.neighDelCfg[numNghDel].t.utranCell.plmnId.mcc[1] =
                                           utraCellList->nghUtraCellCfg.plmnId[0].mcc[1];
          delNghUtraCellCfg->t.neighDelCfg[numNghDel].t.utranCell.plmnId.mcc[2] = 
                                         utraCellList->nghUtraCellCfg.plmnId[0].mcc[2];
          delNghUtraCellCfg->t.neighDelCfg[numNghDel].t.utranCell.plmnId.mnc[0] = 
                                            utraCellList->nghUtraCellCfg.plmnId[0].mnc[0];
          delNghUtraCellCfg->t.neighDelCfg[numNghDel].t.utranCell.plmnId.mnc[1] =
                                               utraCellList->nghUtraCellCfg.plmnId[0].mnc[1];

          if(3 == utraCellList->nghUtraCellCfg.plmnId[0].numMncDigits)
          {
             delNghUtraCellCfg->t.neighDelCfg[numNghDel].t.utranCell.plmnId.mnc[2] = 
                                               utraCellList->nghUtraCellCfg.plmnId[0].mnc[2];
          }
          delArray[numNghDel] = (PTR)utraCellList;
          numNghDel++;
          delNghUtraCellCfg->numCells = numNghDel; 
       }
     }
     prevEntry = (PTR )utraCellList;
   }
   if(delFlag != FALSE)
   {
      smSendDynCfgReq(MSM_LTE_UTRA_NEIGHBORLIST_IN_USE_PARAMETERS,
                       LWR_DYN_EVNT_NEIGH_UTRA_CELL_CFG, delNghUtraCellCfg);

      for(idx = 0; idx < numNghDel; idx++)
      {
        cmHashListDelete(&(smCfgCb.neighUtraCellLstCp), delArray[idx]);
        WR_FREE(delArray[idx], sizeof(SmNeigUtraCellList));
      }
    }
    RETVALUE(ROK);
}/*end of smHdlNeighUtraDelCellWithFreq*/


/*
 * 
 *        Fun:smHdlNeighUtraCellCfgDel 
 * 
 *        Desc:  
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smHdlNeighUtraCellCfgDel()
#else /* if ANSI not defined*/
PRIVATE S16 smHdlNeighUtraCellCfgDel()
#endif /* end of ANSI*/
{
   SmNeigUtraCellList *utraCellList = NULLP;
   LwrNeighCellCfg    *delNghUtraCellCfg = NULLP;  
   PTR                 prevEntry = NULLP;
   PTR                 delArray[MAX_NUM_NEIGH_CELLS];
   U32                 idx = 0;
   U16                 numNghDel= 0;
   U8                  delFlag = FALSE;
   TRC2(smHdlNeighUtraCellCfgDel);
   while(ROK == cmHashListGetNext(&(smCfgCb.neighUtraCellLstCp), prevEntry, (PTR *)&utraCellList))
   {
     if(utraCellList->dynCfgInd != WR_CFG_FOUND)
     {
        if(delFlag == FALSE)
        {
          WR_ALLOC(&delNghUtraCellCfg, sizeof(LwrNeighCellCfg));
          if(NULLP == delNghUtraCellCfg)
          {
             RLOG0(L_FATAL, "Memory allocation failed.");
             RETVALUE(RFAILED);
          }
          delNghUtraCellCfg->cellId = 1; 
          delNghUtraCellCfg->action = WR_ACT_DEL; 
          delFlag = TRUE;
        }

        delNghUtraCellCfg->t.neighDelCfg[numNghDel].type = WR_NEIGH_CELL_UTRA_FDD;
        delNghUtraCellCfg->t.neighDelCfg[numNghDel].t.utranCell.nrIndex = 
                                                    utraCellList->nghUtraCellCfg.nrIndex;
        delNghUtraCellCfg->t.neighDelCfg[numNghDel].t.utranCell.arfcn   = 
                                                    utraCellList->nghUtraCellCfg.t.utraFdd.arfcnDl;
        delNghUtraCellCfg->t.neighDelCfg[numNghDel].t.utranCell.psc     = 
                                                      utraCellList->nghUtraCellCfg.t.utraFdd.psc;
        delNghUtraCellCfg->t.neighDelCfg[numNghDel].t.utranCell.cid =  
                                                       utraCellList->nghUtraCellCfg.cId;

        delNghUtraCellCfg->t.neighDelCfg[numNghDel].t.utranCell.plmnId.numMncDigits =
                                       utraCellList->nghUtraCellCfg.plmnId[0].numMncDigits;
        delNghUtraCellCfg->t.neighDelCfg[numNghDel].t.utranCell.plmnId.mcc[0] =
                                        utraCellList->nghUtraCellCfg.plmnId[0].mcc[0];
        delNghUtraCellCfg->t.neighDelCfg[numNghDel].t.utranCell.plmnId.mcc[1] =
                                         utraCellList->nghUtraCellCfg.plmnId[0].mcc[1];
        delNghUtraCellCfg->t.neighDelCfg[numNghDel].t.utranCell.plmnId.mcc[2] = 
                                       utraCellList->nghUtraCellCfg.plmnId[0].mcc[2];
        delNghUtraCellCfg->t.neighDelCfg[numNghDel].t.utranCell.plmnId.mnc[0] = 
                                          utraCellList->nghUtraCellCfg.plmnId[0].mnc[0];
        delNghUtraCellCfg->t.neighDelCfg[numNghDel].t.utranCell.plmnId.mnc[1] =
                                             utraCellList->nghUtraCellCfg.plmnId[0].mnc[1];

        if(3 == utraCellList->nghUtraCellCfg.plmnId[0].numMncDigits)
        {
           delNghUtraCellCfg->t.neighDelCfg[numNghDel].t.utranCell.plmnId.mnc[2] = 
                                             utraCellList->nghUtraCellCfg.plmnId[0].mnc[2];
        }
        delArray[numNghDel] = (PTR)utraCellList;
        numNghDel++;
        delNghUtraCellCfg->numCells = numNghDel; 
     }
     utraCellList->dynCfgInd = WR_CFG_DONE;
     prevEntry = (PTR )utraCellList;
   }
   if(delFlag != FALSE)
   {
      smSendDynCfgReq(MSM_LTE_UTRA_NEIGHBORLIST_IN_USE_PARAMETERS,
                       LWR_DYN_EVNT_NEIGH_UTRA_CELL_CFG, delNghUtraCellCfg);

      for(idx = 0; idx < numNghDel; idx++)
      {
        cmHashListDelete(&(smCfgCb.neighUtraCellLstCp), delArray[idx]);
        WR_FREE(delArray[idx], sizeof(SmNeigUtraCellList));
      }
    }
    RETVALUE(ROK);
}/*end of smHdlNeighUtraCellCfgDel*/

/*
 * 
 *        Fun: smHdlDynNeighUtraCellCfg 
 * 
 *        Desc: function to handal dynamic Utra cell configuration
 *              if updated list contain new cell trigger add else
 *              check for is there any parameter within the cell is modified and
 *              mark all the new cell and old cell which are part of updated list to WR_CFG_FOUND
 *              the cells which are already configured but not part of updated list are consider as deleted  
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smHdlDynNeighUtraCellCfg(MsmNeighCellCfg *updatedCellList)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlDynNeighUtraCellCfg(updatedCellList)
MsmNeighCellCfg *updatedCellList
#endif /* end of ANSI*/
{
   SmNeigUtraCellList *nghUtraCellList = NULLP;
   SmNeigUtraFreqList  *nghUtraFreqList = NULLP;
   MsmUtraNeighCell   *utraCellCfg = NULLP;
   smNghCellHasKey    hashKey;
   U32                idx = 0;

   TRC2(smHdlDynNeighUtraCellCfg);

   cmMemset((U8*)&hashKey, 0, sizeof(hashKey));
   for(idx = 0; idx< updatedCellList->numUtraCells; idx++)
   {
      utraCellCfg = &updatedCellList->t.neighAddCfg[idx].utranCell;

      smFillNghHashKeyInit(&hashKey, (LwrPlmnId *)&utraCellCfg->plmnId[0], utraCellCfg->cId);

      if(ROK == cmHashListFind(&(smCfgCb.neighUtraCellLstCp), (U8*)&hashKey,
               sizeof(smNghCellHasKey), 0,(PTR*) &nghUtraCellList))
      {
         /* Modify*/
         smHdlNeighUtraCellCfgMod(&nghUtraCellList->nghUtraCellCfg, utraCellCfg);
         nghUtraCellList->dynCfgInd = WR_CFG_FOUND;
      }
      else
      {
         if(ROK != cmHashListFind(&(smCfgCb.neighUtraFreqLstCp), (U8*)&utraCellCfg->arfcnDl,
                  sizeof(utraCellCfg->arfcnDl), 0,(PTR*) &nghUtraFreqList))
         {
            RLOG1(L_ERROR, "can't add cell because corresponding frequeancy "
               "is not configured earfcn = %d", utraCellCfg->arfcnDl);
            continue;
         }
         WR_ALLOC(&nghUtraCellList, sizeof(SmNeigUtraCellList));
         if(NULLP == nghUtraCellList)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         smFillNghUtraCellParam(&nghUtraCellList->nghUtraCellCfg, utraCellCfg, wrSmDfltReCfgType);

         smFillNghHashKeyInit(&nghUtraCellList->hashKey, 
               &nghUtraCellList->nghUtraCellCfg.plmnId[0],
               nghUtraCellList->nghUtraCellCfg.cId);

         if(cmHashListInsert(&(smCfgCb.neighUtraCellLstCp),(PTR)nghUtraCellList,
                  (U8 *)&nghUtraCellList->hashKey, sizeof(nghUtraCellList->hashKey)) != ROK)
         {
            RLOG1(L_ERROR, "cmHashListInsert failed Two Utra cells configured "
               "same plmnId and cellId CID = %d", utraCellCfg->cId);
            WR_FREE(nghUtraCellList, sizeof(SmNeigUtraCellList));
            RETVALUE(RFAILED);
         }

         smHdlNeighUtraCellCfgAdd(&nghUtraCellList->nghUtraCellCfg);
         nghUtraCellList->dynCfgInd = WR_CFG_FOUND;
      }
   }
   smHdlNeighUtraCellCfgDel();
   RETVALUE(ROK);
}/*end of smHdlDynNeighUtraCellCfg*/

/*
 * 
 *        Fun: smHdlNeighiEutraCellCfgAdd 
 * 
 *        Desc:  
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smHdlNeighiEutraCellCfgAdd(MsmEUTRANeighCell *nghEutraAddCell)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlNeighiEutraCellCfgAdd(nghEutraAddCell)
MsmEUTRANeighCell *nghEutraAddCell;
#endif /* end of ANSI*/
{
   SmNeigEutraCellList *nghEutraCellList = NULLP;
   SmNeigEutraFreqList *nghEutraFreqList = NULLP;
   SmNeigEnbList       *nghEnbCfgList = NULLP;
   LwrNeighCellCfg     *addNghCell = NULLP; 
   U32                  enbId =0;
   TRC2(smHdlNeighiEutraCellCfgAdd);
   if(ROK != cmHashListFind(&(smCfgCb.neighEutraFreqLstCp), (U8*)&nghEutraAddCell->earfcnDl,
                  sizeof(nghEutraAddCell->earfcnDl), 0,(PTR*) &nghEutraFreqList))
   {
      RLOG1(L_ERROR, "can't add cell because corresponding frequency is not "
         "configured earfcn = %d", nghEutraAddCell->earfcnDl);
      RETVALUE(RFAILED);
   }
   if(WR_ENODEB_TYPE_MACRO == smGetEnbType(nghEutraAddCell->pci))
   {
     enbId = nghEutraAddCell->cid >>8;
   }
   else
   {
     enbId = nghEutraAddCell->cid;
   }
   if(ROK == cmHashListFind(&(smCfgCb.neighEnbLstCp), (U8*)&enbId,
               sizeof(enbId), 0,(PTR*) &nghEnbCfgList))
   {
         nghEnbCfgList->numCell++;
   }
   else
   { 
      WR_ALLOC(&nghEnbCfgList, sizeof(SmNeigEnbList));
      if(NULLP == nghEnbCfgList)
      {
        RLOG0(L_FATAL, "Memory allocation failed.");
        RETVALUE(RFAILED);
      }
      smFillNghEnbParam(nghEnbCfgList, nghEutraAddCell);

      if(cmHashListInsert(&(smCfgCb.neighEnbLstCp),
               (PTR)nghEnbCfgList, (U8 *)&nghEnbCfgList->nghEnbCfg.enbId,
                      sizeof(nghEnbCfgList->nghEnbCfg.enbId)) != ROK)
      {
         RLOG0(L_ERROR, "cmHashListInsert failed");
         WR_FREE(nghEnbCfgList, sizeof(SmNeigEnbList));
         RETVALUE(RFAILED);
      }
    }

    WR_ALLOC(&nghEutraCellList, sizeof(SmNeigEutraCellList));
    if(NULLP == nghEutraCellList)
    {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
    }
    smFillNghEutraCellParam(&nghEutraCellList->nghEutraCellCfg, nghEutraAddCell,
                                wrSmDfltReCfgType);
    /*hash key */
    smFillNghHashKeyInit(&nghEutraCellList->hashKey, 
                  &nghEutraCellList->nghEutraCellCfg.enbPlmn,
                   nghEutraCellList->nghEutraCellCfg.enbId);
    nghEutraCellList->dynCfgInd = WR_CFG_ADD;
    if(cmHashListInsert(&(smCfgCb.neighEutraCellLstCp),
                         (PTR)nghEutraCellList, (U8 *)&nghEutraCellList->hashKey,
                          sizeof(smNghCellHasKey)) != ROK)
    {
       RLOG1(L_ERROR, "cmHashListInsert failed Two Utra cells configured same "
          "plmnId and cellId CID =%ld",nghEutraCellList->nghEutraCellCfg.enbId);
       WR_FREE(nghEutraCellList, sizeof(SmNeigEutraCellList));
       RETVALUE(RFAILED);
    }
    /*SEND TO ENBAPP ONLY AFTER CELL IS UP*/
    if((WR_SM_STATE_CELL_UP == smCb.smState) ||
         (WR_SM_STATE_OAM_DYN_START == smCb.smState))
    {

       WR_ALLOC(&addNghCell, sizeof(LwrNeighCellCfg));
       if(NULLP == addNghCell)
       {
          RLOG0(L_FATAL, "Memory allocation failed.");
          RETVALUE(RFAILED);
       }
       addNghCell->cellId = 1;
       addNghCell->t.neighAddCfg[0].t.eutraCell.isHeMsCell = nghEutraAddCell->isHeMsCell;
       addNghCell->action = WR_ACT_ADD;
       addNghCell->numCells = 1;
       addNghCell->t.neighAddCfg[0].type = WR_NEIGH_CELL_EUTRA;
       addNghCell->t.neighAddCfg[0].cfgType = WR_NEIGH_CELL_CFG_DYNAMIC;
       addNghCell->t.neighAddCfg[0].t.eutraCell = nghEutraCellList->nghEutraCellCfg;

       smSendDynCfgReq(MSM_LTE_EUTRAN_NEIGHBORLIST_IN_USE_PARAMETERS,
                                   LWR_DYN_EVNT_NEIGH_EUTRA_CELL_CFG, addNghCell);
    }
    else
    {
       MsmDynCfmInfo dynCfmInfo;

       dynCfmInfo.cfgType = MSM_LTE_EUTRAN_NEIGHBORLIST_IN_USE_PARAMETERS; 
       dynCfmInfo.action = MSM_DYN_ADD;
       dynCfmInfo.u.neighCellIndex = nghEutraAddCell->nrIndex;

       msmDynConfigComplete(MSM_CFM_SUCCESS,MSM_CAUSE_NA,dynCfmInfo);

       RLOG0(L_DEBUG,"Not triggering Dynamic Configuration of Neighbor TYPE:ADD to WR \n");
    }

    RETVALUE(ROK);
}/*end of smHdlNeighiEutraCellCfgAdd*/

/*
 * 
 *        Fun: smHdlNeighiEutraCellCfgMod 
 * 
 *        Desc:  
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smHdlNeighiEutraCellCfgMod(MsmEUTRANeighCell *updatedNhgEutraCell, 
                                     LwrEUTRANeighCell *nghEutraAddCell)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlNeighiEutraCellCfgMod(updatedNhgEutraCell,nghEutraAddCell)
LwrEUTRANeighCell *nghEutraAddCell;
MsmEUTRANeighCell *updatedNhgEutraCell;
#endif /* end of ANSI*/
{
  LwrNeighCellCfg     *addNghCell = NULLP; 
  U16  recfgType = 0;
  U8   modFlag = FALSE;  

  TRC2(smHdlNeighiEutraCellCfgMod);
  if(nghEutraAddCell->qoffset != updatedNhgEutraCell->qoffset)
  {
    modFlag = TRUE;
    recfgType |= LWR_EUTRAN_NGH_CELL_QOFFSET;
    nghEutraAddCell->qoffset = updatedNhgEutraCell->qoffset;
  }
  if(nghEutraAddCell->tac != updatedNhgEutraCell->tac)
  {
    modFlag = TRUE;
    recfgType |= LWR_EUTRAN_NGH_CELL_TAC;
    nghEutraAddCell->tac = updatedNhgEutraCell->tac;
  }
  if(nghEutraAddCell->csgId != updatedNhgEutraCell->csgId)
  {
    modFlag = TRUE;
    recfgType |= LWR_EUTRAN_NGH_CELL_CSG_ID;
    nghEutraAddCell->csgId = updatedNhgEutraCell->csgId;
  }
  if(nghEutraAddCell->csgAccessMode != updatedNhgEutraCell->csgAccessMode)
  {
    modFlag = TRUE;
    recfgType |= LWR_EUTRAN_NGH_CELL_CSG_ACC_MODE;
    nghEutraAddCell->csgAccessMode = updatedNhgEutraCell->csgAccessMode;
  }
  if(nghEutraAddCell->blackListed != updatedNhgEutraCell->BlackListed)
  {
     recfgType |= LWR_EUTRAN_NGH_CELL_BLACK_LIST;
     nghEutraAddCell->blackListed= updatedNhgEutraCell->BlackListed;
     modFlag = TRUE;
  }
  if(nghEutraAddCell->pci != updatedNhgEutraCell->pci)
  {
     recfgType |= LWR_EUTRAN_NGH_CELL_PCI;
     nghEutraAddCell->oldPci = nghEutraAddCell->pci;
     nghEutraAddCell->pci = updatedNhgEutraCell->pci;
     modFlag = TRUE;
  }
  if(nghEutraAddCell->earfcnDl!= updatedNhgEutraCell->earfcnDl)
  {
     recfgType |= LWR_EUTRAN_NGH_CELL_DL_EARFCN;
     nghEutraAddCell->oldEarfcnDl= nghEutraAddCell->earfcnDl;
     nghEutraAddCell->earfcnDl= updatedNhgEutraCell->earfcnDl;
     modFlag = TRUE;
  }
  
  if(modFlag == TRUE)
  {
    /*SEND TO ENBAPP ONLY AFTER CELL IS UP*/
    if((WR_SM_STATE_CELL_UP == smCb.smState) ||
         (WR_SM_STATE_OAM_DYN_START == smCb.smState))
    {
       WR_ALLOC(&addNghCell, sizeof(LwrNeighCellCfg));
       if(NULLP == addNghCell)
       {
          RLOG0(L_FATAL, "Memory allocation failed.");
          RETVALUE(RFAILED);
       }
       addNghCell->cellId = 1;
       addNghCell->action = WR_ACT_MOD;
       addNghCell->numCells = 1;
       addNghCell->t.neighAddCfg[0].type = WR_NEIGH_CELL_EUTRA;
       nghEutraAddCell->reCfgType = recfgType;
       addNghCell->t.neighAddCfg[0].t.eutraCell = *nghEutraAddCell;

       smSendDynCfgReq(MSM_LTE_EUTRAN_NEIGHBORLIST_IN_USE_PARAMETERS,
                                   LWR_DYN_EVNT_NEIGH_EUTRA_CELL_CFG, addNghCell);
     }
     else
     {
        MsmDynCfmInfo dynCfmInfo;

        dynCfmInfo.cfgType = MSM_LTE_EUTRAN_NEIGHBORLIST_IN_USE_PARAMETERS; 
        dynCfmInfo.action = MSM_ACT_MOD;
        dynCfmInfo.u.neighCellIndex = updatedNhgEutraCell->nrIndex;

        msmDynConfigComplete(MSM_CFM_SUCCESS,MSM_CAUSE_NA,dynCfmInfo);

        RLOG0(L_INFO,"Not triggering Dynamic Configuration of Neighbor TYPE:MOD to WR \n");
     }
  }
  else
  {
      RETVALUE(RFAILED);
  }
  RETVALUE(ROK);
}/*end of smHdlNeighiEutraCellCfgMod*/
/*
 * 
 *        Fun: smHdlNeighEutraDelCellWithFreq 
 * 
 *        Desc: delete cells which have freq which is part of delete eutra freq list
 *              if the all cells deleted corresponding to the enb then delete the neighbors 
 *              for which all the cells are deleted
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PUBLIC S16 smHdlNeighEutraDelCellWithFreq(LwrNeighFreqCfg *delNghEutraFreq)
#else /* if ANSI not defined*/
PUBLIC S16 smHdlNeighEutraDelCellWithFreq(delNghEutraFreq)
LwrNeighFreqCfg *delNghEutraFreq;
#endif /* end of ANSI*/
{
  SmNeigEutraCellList *eutraCellList = NULLP;
  LwrNeighCellCfg     *delNghCell = NULLP; 
  SmNeigEnbList       *nghEnbCfgList = NULLP;
  PTR                 prevEntry = NULLP;
  PTR                 delArray[MAX_NUM_FREQ_CFG];
  U32                 enbId = 0;
  U16                 numNghDel = 0;
  U16                 idx = 0;
  U8                  delIndFlag = FALSE;

  TRC2(smHdlNeighEutraDelCellWithFreq);
  while(ROK == cmHashListGetNext(&(smCfgCb.neighEutraCellLstCp),
                                      prevEntry, (PTR *) &eutraCellList))
  {
     for(idx = 0; idx < delNghEutraFreq->numFreq; idx++)
     {
        if(eutraCellList->nghEutraCellCfg.earfcnDl == 
             delNghEutraFreq->t.delNeighFreq[idx].t.eutranFreq.earfcn)
        {
          if(delIndFlag ==  FALSE)
          {
            WR_ALLOC(&delNghCell, sizeof(LwrNeighCellCfg));
            if(NULLP == delNghCell)
            {
              RLOG0(L_FATAL, "Memory allocation failed.");
              RETVALUE(RFAILED);
            }
            delNghCell->cellId = 1;
            delNghCell->action = WR_ACT_DEL;
            delIndFlag = TRUE;
            delNghCell->t.neighDelCfg[numNghDel].type = WR_NEIGH_CELL_EUTRA;
            delNghCell->t.neighDelCfg[numNghDel].t.eutranCell.nrIndex = 
                                                 eutraCellList->nghEutraCellCfg.nrIndex;
            delNghCell->t.neighDelCfg[numNghDel].t.eutranCell.earfcn = 
                                                  eutraCellList->nghEutraCellCfg.earfcnDl;
            delNghCell->t.neighDelCfg[numNghDel].t.eutranCell.phyCellId = 
                                                     eutraCellList->nghEutraCellCfg.pci;
            delNghCell->t.neighDelCfg[numNghDel].t.eutranCell.cid =
                                                eutraCellList->nghEutraCellCfg.enbId;

            delNghCell->t.neighDelCfg[numNghDel].t.eutranCell.plmnId = 
                                          eutraCellList->nghEutraCellCfg.plmnId[0];
            delArray[numNghDel] = (PTR)eutraCellList;
            delNghCell->numCells = ++numNghDel;
            if(WR_ENODEB_TYPE_MACRO == smGetEnbType(eutraCellList->nghEutraCellCfg.pci))
            {
              enbId = eutraCellList->nghEutraCellCfg.enbId>>8; 
            }
            else
            {
              enbId = eutraCellList->nghEutraCellCfg.enbId; 
            }
            if(ROK == cmHashListFind(&(smCfgCb.neighEnbLstCp), (U8*)&enbId,
                        sizeof(enbId), 0,(PTR*) &nghEnbCfgList))
            {
                  nghEnbCfgList->numCell--;
            }
          }
        }
     prevEntry = (PTR)eutraCellList;
    }/*end of for loop*/
  }
  if(delIndFlag != FALSE)
  {
    /*send delete cell Request*/
    smSendDynCfgReq(MSM_LTE_EUTRAN_NEIGHBORLIST_IN_USE_PARAMETERS,
                                 LWR_DYN_EVNT_NEIGH_EUTRA_CELL_CFG, delNghCell);
    for(idx = 0; idx < numNghDel; idx++)
    {
      cmHashListDelete(&(smCfgCb.neighEutraCellLstCp), delArray[idx]);
      WR_FREE(delArray[idx], sizeof(SmNeigEutraCellList));
    }
   /*if there is no cell in neighbor*/
    smHdlNeighEnbCfgDel();
  }
  RETVALUE(ROK);
}/*end of smHdlNeighEutraDelCellWithFreq*/

/*
 * 
 *        Fun: smHdlNeighiEutraCellCfgDel 
 * 
 *        Desc:  
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smHdlNeighiEutraCellCfgDel(SmNeigEutraCellList *eutraCellList)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlNeighiEutraCellCfgDel(eutraCellList)
SmNeigEutraCellList *eutraCellList;
#endif /* end of ANSI*/
{
   LwrNeighCellCfg     *delNghCell = NULLP; 
   U32                 enbId = 0;
   U16                 numNghDel = 0;

   TRC2(smHdlNeighiEutraCellCfgDel);
   /*SEND TO ENBAPP ONLY AFTER CELL IS UP*/
    if((WR_SM_STATE_CELL_UP == smCb.smState) ||
         (WR_SM_STATE_OAM_DYN_START == smCb.smState))
   {
      WR_ALLOC(&delNghCell, sizeof(LwrNeighCellCfg));
      if(NULLP == delNghCell)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      delNghCell->cellId = 1;
      delNghCell->action = WR_ACT_DEL;
      delNghCell->numCells = 1;
      delNghCell->t.neighDelCfg[numNghDel].type = WR_NEIGH_CELL_EUTRA;

      delNghCell->t.neighDelCfg[numNghDel].t.eutranCell.nrIndex = 
         eutraCellList->nghEutraCellCfg.nrIndex;
      delNghCell->t.neighDelCfg[numNghDel].t.eutranCell.earfcn = 
         eutraCellList->nghEutraCellCfg.earfcnDl;
      delNghCell->t.neighDelCfg[numNghDel].t.eutranCell.phyCellId = 
         eutraCellList->nghEutraCellCfg.pci;
      delNghCell->t.neighDelCfg[numNghDel].t.eutranCell.cid =
         eutraCellList->nghEutraCellCfg.enbId;
      delNghCell->t.neighDelCfg[numNghDel].t.eutranCell.plmnId= 
         eutraCellList->nghEutraCellCfg.plmnId[0];
      if(WR_ENODEB_TYPE_MACRO == smGetEnbType(eutraCellList->nghEutraCellCfg.pci))
      {
         enbId = eutraCellList->nghEutraCellCfg.enbId>>8; 
      }
      else
      {
         enbId = eutraCellList->nghEutraCellCfg.enbId; 
      }
      /*send delete cell Request*/
      smSendDynCfgReq(MSM_LTE_EUTRAN_NEIGHBORLIST_IN_USE_PARAMETERS,
                                 LWR_DYN_EVNT_NEIGH_EUTRA_CELL_CFG, delNghCell);
   }
   else
   {
       MsmDynCfmInfo dynCfmInfo;

       dynCfmInfo.cfgType = MSM_LTE_EUTRAN_NEIGHBORLIST_IN_USE_PARAMETERS; 
       dynCfmInfo.action = MSM_ACT_DEL;
       dynCfmInfo.u.neighCellIndex = eutraCellList->nghEutraCellCfg.nrIndex;

       RLOG0(L_INFO,"Not triggering Dynamic Configuration of Neighbor TYPE:DEL to WR \n");
       msmDynConfigComplete(MSM_CFM_SUCCESS,MSM_CAUSE_NA,dynCfmInfo);
   }

   RETVALUE(ROK);
}/*end of smHdlNeighiEutraCellCfgDel*/
/*
 * 
 *        Fun: smHdlNeighEnbDel
 * 
 *        Desc:  
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PUBLIC S16 smHdlNeighEnbCfgDel()
#else /* if ANSI not defined*/
PUBLIC S16 smHdlNeighEnbCfgDel()
#endif /* end of ANSI*/
{
   SmNeigEnbList       *nghEnbCfgList = NULLP;
   LwrNeighCfg         *nghEnbDelList = NULLP;
   PTR                 prevEntry = NULLP;
   PTR                 delArray[MAX_NUM_FREQ_CFG];
   U16                 idx = 0;
   
   U8                  delIndFlag = FALSE;

   TRC2(smHdlNeighEnbCfgDel);
   while(ROK == cmHashListGetNext(&(smCfgCb.neighEnbLstCp),prevEntry,(PTR *)&nghEnbCfgList))
   {
     if(nghEnbCfgList->numCell == 0)
     {
       if(delIndFlag == FALSE)
       {
          WR_ALLOC(&nghEnbDelList, sizeof(LwrNeighCfg));
          if(NULLP == nghEnbDelList)
          {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
          }
          nghEnbDelList->cellId = 1;
          nghEnbDelList->action = WR_ACT_DEL;
          delIndFlag = TRUE;
          nghEnbDelList->numEnb = 0; 
       }
       nghEnbDelList->t.neighDel[nghEnbDelList->numEnb].enbIndx = 
                                     nghEnbCfgList->nghEnbCfg.enbIndx; 
       nghEnbDelList->t.neighDel[nghEnbDelList->numEnb].enbId = 
                                     nghEnbCfgList->nghEnbCfg.enbId;
       delArray[nghEnbDelList->numEnb] = (PTR)nghEnbCfgList;   
       nghEnbDelList->numEnb++;

       smBldrCzPeerDelCntrlReq(nghEnbCfgList->nghEnbCfg.peerId, 
                                   nghEnbCfgList->nghEnbCfg.enbId);
       smReturnPeerIdToFreeList(nghEnbCfgList->nghEnbCfg.peerId);

     }
     prevEntry = (PTR)nghEnbCfgList;
   }
   if(delIndFlag == TRUE)
   {
      smSendDynCfgReq(MSM_LTE_ENB_PROTO_CFG_PARAMETERS,
                                 LWR_DYN_EVNT_NEIGH_ENB_CFG, nghEnbDelList);
      for(idx = 0; idx < nghEnbDelList->numEnb; idx++)
      {
        cmHashListDelete(&(smCfgCb.neighEnbLstCp), delArray[idx]);
        WR_FREE(delArray[idx], sizeof(SmNeigEnbList));
      }
   }
   RETVALUE(ROK);
}/*end of smHdlNeighEnbCfgDel*/
/*
 * 
 *        Fun: smHdlDynNeighEutraCellCfg 
 * 
 *        Desc: function to handal dynamic Euta cell configuration
 *              if updated list contain new cell trigger add else
 *              check for is there any parameter within the cell is modified and
 *              mark all the new cell and old cell which are part of updated list to WR_CFG_FOUND
 *              the cells which are already configured but not part of updated list are consider as deleted 
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_smm_dyncfg.c 
 */
#ifdef ANSI
PRIVATE S16 smHdlDynNeighEutraCellCfg(MsmDynNeighCellCfg *updatedCellList)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlDynNeighEutraCellCfg(updatedCellList)
MsmDynNeighCellCfg *updatedCellList;
#endif /* end of ANSI*/
{
   MsmEUTRANeighCell   *eutraCellList = NULLP;
   MsmEutranCellDelCfg *eutraDelCellList = NULLP;
   SmNeigEutraCellList *nghEutraCellList = NULLP;
   SmNeigEutraFreqList  *nghEutraFreqList = NULLP;
   smNghCellHasKey      hashKey;

   TRC2(smHdlDynNeighEutraCellCfg);

   RLOG1(L_DEBUG, "Received Dynamic Configuration of Type %u",
         updatedCellList->actType);
   cmMemset((U8*)&hashKey, 0, sizeof(hashKey));
   if (MSM_DYN_ADD == updatedCellList->actType)
   {
      if (lteeNodeBparams.lteNeighCellCfg.numEutraCells == MSM_MAX_NUM_EUTRA_NEIGH_CELLS)
      {
            MsmDynCfmInfo dynCfmInfo;
            RLOG0(L_ERROR, "Max number of neighbor EUTRA cell configurations already reached.");

            dynCfmInfo.cfgType = MSM_LTE_EUTRAN_NEIGHBORLIST_IN_USE_PARAMETERS; 
            dynCfmInfo.action = MSM_DYN_ADD;
            dynCfmInfo.u.neighCellIndex = updatedCellList->t.addModCfg.t.eutraCell.nrIndex;
            
            msmDynConfigComplete(MSM_CFM_FAILURE,MSM_CAUSE_CFG_FAIL,dynCfmInfo);
            RLOG0(L_ERROR, "[ALARM] Sending NEIGH_CELL_CFG_FAIL to OAM");
            FapAlarmInd( NEIGH_CELL_CFG_FAIL, SM_ALARM_SEVERITY_MAJOR, SM_ALARM_CAUSE_TYPE_ERR_IND, SM_ALARM_CAUSE_VAL_ABNORMAL);

            RETVALUE(ROK);

      }
      eutraCellList = &updatedCellList->t.addModCfg.t.eutraCell;
      smFillNghHashKeyInit(&hashKey, (LwrPlmnId *)&eutraCellList->plmnId[0], eutraCellList->cid);
      if(ROK == cmHashListFind(&(smCfgCb.neighEutraCellLstCp), (U8*)&hashKey,
               sizeof(smNghCellHasKey), 0,(PTR*) &nghEutraCellList))
      {
         if (ROK == checkNeighEutraCellCfgValues(&(nghEutraCellList->nghEutraCellCfg), eutraCellList))
         {
            MsmDynCfmInfo dynCfmInfo;

            dynCfmInfo.cfgType = MSM_LTE_EUTRAN_NEIGHBORLIST_IN_USE_PARAMETERS; 
            dynCfmInfo.action = MSM_DYN_ADD;
            dynCfmInfo.u.neighCellIndex = eutraCellList->nrIndex;

            msmDynConfigComplete(MSM_CFM_SUCCESS,MSM_CAUSE_NA,dynCfmInfo);

            RETVALUE(ROK);
         }
         else
         {
            MsmDynCfmInfo dynCfmInfo;

            dynCfmInfo.cfgType = MSM_LTE_EUTRAN_NEIGHBORLIST_IN_USE_PARAMETERS; 
            dynCfmInfo.action = MSM_DYN_ADD;
            dynCfmInfo.u.neighCellIndex = eutraCellList->nrIndex;

            msmDynConfigComplete(MSM_CFM_FAILURE,MSM_CAUSE_CFG_FAIL,dynCfmInfo);
            RLOG0(L_ERROR, "[ALARM] Sending NEIGH_CELL_CFG_FAIL to OAM");
            FapAlarmInd( NEIGH_CELL_CFG_FAIL, SM_ALARM_SEVERITY_MAJOR, SM_ALARM_CAUSE_TYPE_ERR_IND, SM_ALARM_CAUSE_VAL_ABNORMAL);

            RETVALUE(ROK);
         }
      }
      smHdlNeighiEutraCellCfgAdd(eutraCellList);
   }
   else if (MSM_ACT_MOD == updatedCellList->actType)
   {
      eutraCellList = &updatedCellList->t.addModCfg.t.eutraCell;
      smFillNghHashKeyInit(&hashKey, (LwrPlmnId *)&eutraCellList->plmnId[0], eutraCellList->cid);
      if((ROK == cmHashListFind(&(smCfgCb.neighEutraCellLstCp), (U8*)&hashKey,
               sizeof(smNghCellHasKey), 0,(PTR*) &nghEutraCellList))&&
         (ROK == cmHashListFind(&(smCfgCb.neighEutraFreqLstCp), (U8*)&eutraCellList->earfcnDl,
                  sizeof(eutraCellList->earfcnDl), 0,(PTR*) &nghEutraFreqList)))
      {
         nghEutraCellList->dynCfgInd = WR_CFG_MOD; 
         if (ROK != smHdlNeighiEutraCellCfgMod(eutraCellList, &nghEutraCellList->nghEutraCellCfg))
         {
            MsmDynCfmInfo dynCfmInfo;

            dynCfmInfo.cfgType = MSM_LTE_EUTRAN_NEIGHBORLIST_IN_USE_PARAMETERS; 
            dynCfmInfo.action = MSM_ACT_MOD;
            dynCfmInfo.u.neighCellIndex = eutraCellList->nrIndex;

            msmDynConfigComplete(MSM_CFM_SUCCESS,MSM_CAUSE_NA,dynCfmInfo);

            RETVALUE(ROK);
         }
      }
      else
      {
         MsmDynCfmInfo dynCfmInfo;

         RLOG0(L_ERROR, "Failed to get the cell node from neighbour cell list"
            "or Modified Frequency doesn't exists in the frequency list");
         dynCfmInfo.cfgType = MSM_LTE_EUTRAN_NEIGHBORLIST_IN_USE_PARAMETERS; 
         dynCfmInfo.action = MSM_ACT_MOD;
         dynCfmInfo.u.neighCellIndex = eutraCellList->nrIndex;

         msmDynConfigComplete(MSM_CFM_FAILURE,MSM_CAUSE_CFG_FAIL,dynCfmInfo);
         RLOG0(L_ERROR, "[ALARM] Sending NEIGH_CELL_CFG_FAIL to OAM");
         FapAlarmInd( NEIGH_CELL_CFG_FAIL, SM_ALARM_SEVERITY_MAJOR, SM_ALARM_CAUSE_TYPE_ERR_IND, SM_ALARM_CAUSE_VAL_ABNORMAL);

         RETVALUE(ROK);
      }
   }
   else if (MSM_ACT_DEL == updatedCellList->actType)
   {
      U8                   nodeFound = FALSE;
      PTR                  prevEntry = NULLP;
      eutraDelCellList = &updatedCellList->t.delCfg.t.eutranCell;
      while(ROK == cmHashListGetNext(&(smCfgCb.neighEutraCellLstCp), prevEntry, (PTR *)&nghEutraCellList))
      {
         if(nghEutraCellList->nghEutraCellCfg.nrIndex == eutraDelCellList->nrIndex)
         {
            nodeFound = TRUE;
            break;
         }
         prevEntry = (PTR)nghEutraCellList;
      }
      if (TRUE == nodeFound)
      {
         nghEutraCellList->dynCfgInd = WR_CFG_DEL; 
         smHdlNeighiEutraCellCfgDel(nghEutraCellList);
      }
      else
      {
         MsmDynCfmInfo dynCfmInfo;

         RLOG0(L_ERROR, "Failed to get the cell node from the neighbour cell list.");
         dynCfmInfo.cfgType = MSM_LTE_EUTRAN_NEIGHBORLIST_IN_USE_PARAMETERS; 
         dynCfmInfo.action = MSM_ACT_DEL;
         dynCfmInfo.u.neighCellIndex = eutraDelCellList->nrIndex;

         msmDynConfigComplete(MSM_CFM_SUCCESS,MSM_CAUSE_NA,dynCfmInfo);

         RETVALUE(ROK);
      }
   }
   else
   {
      RLOG0(L_ERROR, "MSM Action type is not valid");
   }   
   RETVALUE(ROK);
}/*end of smHdlDynNeighEutraCellCfg*/

/*
 *
 *        Fun: smHdlSonDynNeighEutraCellCfg
 *
 *        Desc: function to handal dynamic Euta neighbor cell configuration
 *        Ret:   ROK
 *
 *        Notes: None
 *
 *        File:  wr_smm_dynCfg.c
 */
#ifdef ANSI
PRIVATE S16 smHdlSonDynNeighEutraCellCfg
(
MsmDynNeighCellCfg *nghCellCfg
)
#else /* if ANSI not defined*/
PRIVATE S16 smHdlSonDynNeighEutraCellCfg(nghCellCfg)
MsmDynNeighCellCfg *nghCellCfg;
#endif /* end of ANSI*/
{
   NlMngmt             *nlMngmt = NULLP;
   Pst                 pst;
   U32                 enbId;
   U8                  count = 0;
   Bool                nodeFound = FALSE;
   PTR                 prevEntry = NULLP;
   MsmEUTRANeighCell   *msmNghCell = NULLP;
   LnlNghCellCfg       *lnlNghCell = NULLP;
   LnlNghCellCfgParam  *lnlNghCellParam = NULLP;
   LnlNghCellInfo      *lnlNghCellInfo = NULLP;
   MsmEutranCellDelCfg *eutraDelCellList = NULLP;
   SmNeigEutraCellList *nghEutraCellList = NULLP;
   LnlNghCellDel       *lnlNghCellDel    = NULLP;
   TRC3(smHdlSonDynNeighEutraCellCfg);
   /*Allocate memory for the message. This is done since we are using LWLC*/
   SGetSBuf(smCb.init.region, smCb.init.pool, (Data**)&nlMngmt, sizeof(NlMngmt));
   if(NULLP == nlMngmt)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(nlMngmt, sizeof(NlMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   nlMngmt->u.protoCfg.bCfgType = LNL_NGH_CELL_CFG;
   lnlNghCell = &(nlMngmt->u.protoCfg.u.nghCellCfg);
   lnlNghCell->servCellId =
     /*lteeNodeBparams.lteCellSib1CfgGrp.cellAccessInfo.cellId*/ 1;
   lnlNghCell->numNghEnb = 1;
   lnlNghCellParam = &(lnlNghCell->nghEnbCellCfg[0].nghCellParam);
   switch(nghCellCfg->actType)
   {
      case MSM_ACT_ADD :
      case MSM_DYN_ADD :
      {
         lnlNghCellParam->cfgType = LNL_NGH_ADD;
         /* Since only one cell can be added dynamically */
         lnlNghCellParam->u.nghCellAdd.numNgh = 1; 
            msmNghCell = &(nghCellCfg->t.addModCfg.t.eutraCell);
            if(WR_ENODEB_TYPE_MACRO == smGetEnbType(msmNghCell->pci))
            {
               enbId = msmNghCell->cid >>8;
            }
            else
            {
               enbId = msmNghCell->cid;
            }
            lnlNghCell->nghEnbCellCfg[0].nghEnbId = enbId;
            lnlNghCellInfo = &(lnlNghCellParam->
               u.nghCellAdd.nghCellAddInfo[count].nghCellInfo);
            lnlNghCellInfo->pci = msmNghCell->pci;
            lnlNghCellInfo->dlEarfcn = msmNghCell->earfcnDl;
            lnlNghCellInfo->ulEarfcn = msmNghCell->earfcnUl;
            lnlNghCellInfo->ecgi.cellId = msmNghCell->cid;
            cmMemcpy((U8 *) &(lnlNghCellInfo->ecgi.plmn),
              (U8 *) &(msmNghCell->plmnId[0]), sizeof(MsmLtePlmnId));

      }
      break;
      case MSM_ACT_MOD :
      {
         lnlNghCellParam->cfgType = LNL_NGH_MOD;
         lnlNghCellParam->u.nghCellMod.numNgh = 1;
         msmNghCell = &(nghCellCfg->t.addModCfg.t.eutraCell);
         if(WR_ENODEB_TYPE_MACRO == smGetEnbType(msmNghCell->pci))
         {
            enbId = msmNghCell->cid >>8;
         }
         else
         {
            enbId = msmNghCell->cid;
         }
         RLOG2(L_DEBUG,"eNB ID being modified is %d, Cell ID modified is %d\n",
               enbId, msmNghCell->cid);
         lnlNghCell->nghEnbCellCfg[0].nghEnbId = enbId;
         lnlNghCellInfo = &(lnlNghCellParam->
               u.nghCellMod.nghCellModInfo[0].nghCellInfo);
         lnlNghCellParam->u.nghCellMod.nghCellModInfo[0].oldEcgi.cellId =
            msmNghCell->cid;
         cmMemcpy((U8 *)&(lnlNghCellParam->
               u.nghCellMod.nghCellModInfo[0].oldEcgi.plmn),
              (U8 *) &(msmNghCell->plmnId[0]), sizeof(MsmLtePlmnId));

         RLOG3(L_DEBUG,"SM->SON: Mod cell Config -- PCI %d, DL EARFCN %d, CellId %d\n", 
               msmNghCell->pci, msmNghCell->earfcnDl,
               msmNghCell->cid);
         RLOG3(L_DEBUG,"SM->SON: Mod cell Config -- MCC[0] %d, MCC[1] %d, MCC[2] %d\n", 
               msmNghCell->plmnId[0].mcc[0], msmNghCell->plmnId[0].mcc[1],
               msmNghCell->plmnId[0].mcc[2]);
         RLOG2(L_DEBUG,"SM->SON: Mod cell Config -- MNC[0] %d, MNC[1] %d\n", 
               msmNghCell->plmnId[0].mnc[0], msmNghCell->plmnId[0].mnc[1]);

         lnlNghCellInfo->pci = msmNghCell->pci;
         lnlNghCellInfo->dlEarfcn = msmNghCell->earfcnDl;
         lnlNghCellInfo->ulEarfcn = msmNghCell->earfcnUl;
         lnlNghCellInfo->ecgi.cellId = msmNghCell->cid;
         cmMemcpy((U8 *) &(lnlNghCellInfo->ecgi.plmn),
              (U8 *) &(msmNghCell->plmnId[0]), sizeof(MsmLtePlmnId));
         lnlNghCellInfo->ecgi.plmn.numMncDigits = 2;
         lnlNghCellInfo->ecgi.plmn.mnc[2] = 0;
      }
      break;
      case MSM_ACT_DEL :
      {
         lnlNghCellParam->cfgType = LNL_NGH_DEL;
         lnlNghCellParam->u.nghCellDel.numNgh = 1;
         eutraDelCellList = &nghCellCfg->t.delCfg.t.eutranCell;
         while(ROK == cmHashListGetNext(&(smCfgCb.neighEutraCellLstCp),
                      prevEntry, (PTR *)&nghEutraCellList))
         {
            if(nghEutraCellList->nghEutraCellCfg.nrIndex == eutraDelCellList->nrIndex)
            {
               nodeFound = TRUE;
               break;
            }
	    prevEntry =(PTR)nghEutraCellList;
         }
         if (TRUE == nodeFound)
         {
            if(WR_ENODEB_TYPE_MACRO == smGetEnbType(nghEutraCellList->nghEutraCellCfg.enbId))
            {
               enbId = nghEutraCellList->nghEutraCellCfg.enbId >>8;
            }
            else
            {
               enbId = nghEutraCellList->nghEutraCellCfg.enbId;
            }
            lnlNghCell->nghEnbCellCfg[0].nghEnbId = enbId;
            lnlNghCellDel = &(lnlNghCellParam->u.nghCellDel);
            lnlNghCellDel->ecgi[0].cellId = enbId;
            cmMemcpy((U8 *) &(lnlNghCellDel->ecgi[0].plmn),
              (U8 *) &(nghEutraCellList->nghEutraCellCfg.plmnId[0]), sizeof(LwrPlmnId));
/*            lnlNghCellDel->ecgi.plmn.numMncDigits = 2;
            lnlNghCellDel->ecgi.plmn.mnc[2] = 0; */

         }
         else
         {
              RLOG0(L_WARNING," during dynamic Deletion of Neighbor not found !!!\n");	
         }
      }
      break;
      default :
      {
         RETVALUE(RFAILED);
      }
   }

   /* Fill Header */
   nlMngmt->hdr.msgType             = TCFG;
   nlMngmt->hdr.msgLen              = 0;
   nlMngmt->hdr.entId.ent           = ENTNL;
   nlMngmt->hdr.entId.inst          = SM_INST_ZERO;
   nlMngmt->hdr.elmId.elmnt         = STNLPROTOCFG;
   nlMngmt->hdr.seqNmb              = 0;
   nlMngmt->hdr.version             = 0;
   nlMngmt->hdr.transId             = LNL_TRANSID_NGH_CELL;

   nlMngmt->hdr.response.prior      = PRIOR0;
   nlMngmt->hdr.response.route      = RTESPEC;
   nlMngmt->hdr.response.mem.region = nlMemMngr.nlRegion;
   nlMngmt->hdr.response.mem.pool   = nlMemMngr.nlPool;
   nlMngmt->hdr.response.selector   = SM_SELECTOR_LWLC;

   /* Fill Pst */
   pst.event     = EVTLNLCFGREQ;
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNL;
   pst.dstProcId = SM_NL_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.dstInst   = SM_INST_ZERO;
   pst.region    = smCb.init.region;
   pst.pool      = smCb.init.pool;

   /* Send the request to the SON */
   SmMiLnlCfgReq(&pst, nlMngmt);
   RETVALUE(ROK);

   RETVALUE(ROK);
}
 
/*
*
*       Fun:   smPackSendDynCfg
*
*       Desc:  packing and sending dynamic configuration to SM
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_dyncfg.c
*
*/ 

PRIVATE S16 smPackSendDynCfg(Void *cfg, U32 len, U32 cfgType)
{
   Pst    smPst;
   Buffer *mBuf;          /* message buffer */

   TRC2(smPackSendDynCfg);

   SM_SET_ZERO(&smPst, sizeof(Pst));

   smPst.dstProcId = SM_SM_PROC;
   smPst.srcProcId = SM_SM_PROC;
   smPst.dstEnt = ENTSM;
   smPst.srcEnt = ENTWR;
   smPst.dstInst = 0;
   smPst.srcInst = 0;
   smPst.event  = EVTSMDYNCFG;
   smPst.pool   = WR_POOL;
   smPst.selector= SM_SELECTOR_LC;

   if(SGetMsg(DFLT_REGION, WR_POOL, &mBuf) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   } /* end of if statement */

   SAddPstMsgMult((Data *)&(cfgType), (MsgLen) sizeof(U32), mBuf);
   SAddPstMsgMult((Data *)&(len), (MsgLen) sizeof(U32), mBuf);
   SAddPstMsgMult((Data *)cfg, (MsgLen)len, mBuf);

   RETVALUE(SPstTsk(&smPst, mBuf));
} /* end of smPackSendDynCfg */

/*
*
*       Fun:   MsmSmmDynCfgReq
*
*       Desc:  Dynamic configuration handler function from MSM to enbapp
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_dyncfg.c
*
*/ 

PUBLIC S16 MsmSmmDynCfgReq(Void *cfg, U32 cfgType, U8 priority)
{
   MsmDynCfmInfo dynCfmInfo;
   U32          ret = ROK;
   MsmEnodebIpAddr *enbIp = NULLP;

   if (cfgType >= MSM_LTE_CONFIGURATION_COMPLETE_END)
   {
      RLOG0(L_ERROR,"MsmSmmDynCfgReq: Configuration Type is Out of range. \n");
      RETVALUE(RFAILED);
   }

   switch(cfgType)
   {
      case MSM_LTE_PRACH_CONFIG_COMMON_PARAMETERS:
         {
	    if(smCb.smState == WR_SM_STATE_CELL_UP ||
#ifdef WR_RSYS_OAM
	       smCb.smState == WR_SM_STATE_OAM_DYN_START
#endif 
	    )
	    {
               RLOG0(L_ERROR,"MsmSmmDynCfgReq: Dynamic Modification of PRACH parameters not allowed after Cell is UP \n");
               RETVALUE(RFAILED);
	    }
            ret = smPackSendDynCfg(cfg, sizeof(MsmLtePrachCfgCommon),cfgType);
            if(ROK != ret)
            {
               RLOG0(L_ERROR,"MsmSmmDynCfgReq: smPackSendDynCfg failed");
               RETVALUE(RFAILED);
            } 
         }
         break;
      case MSM_LTE_RACH_CONFIG_COMMON_PARAMETERS:
         { 
            ret = smPackSendDynCfg(cfg, sizeof(MsmLteRachCfgCommonParams), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_ERROR,"MsmSmmDynCfgReq: smPackSendDynCfg failed");
               RETVALUE(RFAILED);
            } 
         }
         break;
      case MSM_LTE_PDSCH_CONFIG_COMMON_PARAMETERS:
         { 
            ret = smPackSendDynCfg(cfg, sizeof(MsmLtePdschCfgCommon), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_ERROR,"MsmSmmDynCfgReq: smPackSendDynCfg failed");
               RETVALUE(RFAILED);
            } 
         }
         break;
      case MSM_LTE_SRS_CFG_COMMON_PARAMETERS:
         { 
            ret = smPackSendDynCfg(cfg, sizeof(MsmLteSRSCfgCommon), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_ERROR,"MsmSmmDynCfgReq: smPackSendDynCfg failed");
               RETVALUE(RFAILED);
            } 
         }
         break;
      case MSM_LTE_PUSCH_REF_SIGNAL_CFG_PARAMETERS:
         { 
            ret = smPackSendDynCfg(cfg, sizeof(MsmLtePuschRefSignalCfg), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_ERROR,"MsmSmmDynCfgReq: smPackSendDynCfg failed");
               RETVALUE(RFAILED);
            } 
         }
         break;
      case MSM_LTE_UL_POWER_CONTROL_COMMON_PARAMETERS:
         { 
            ret = smPackSendDynCfg(cfg, sizeof(MsmLteULPwrCtrlCommon), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_ERROR,"MsmSmmDynCfgReq: smPackSendDynCfg failed");
               RETVALUE(RFAILED);
            } 
         }
         break;
      case MSM_LTE_UL_FREQUENCY_INFO_PARAMETERS:
         { 
            ret = smPackSendDynCfg(cfg, sizeof(MsmLteULFrequencyInfo), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_ERROR,"MsmSmmDynCfgReq: smPackSendDynCfg failed");
               RETVALUE(RFAILED);
            } 
         }
         break;
      case MSM_LTE_PUSCH_BASIC_CFG_COMMON_PARAMETERS:
         { 
            ret = smPackSendDynCfg(cfg, sizeof(MsmLtePuschBasicCfgCommon), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_ERROR,"MsmSmmDynCfgReq: smPackSendDynCfg failed");
               RETVALUE(RFAILED);
            } 
         }
         break;
      case MSM_LTE_UE_TIMER_CONSTANTS_PARAMETERS:
         { 
            ret = smPackSendDynCfg(cfg, sizeof(MsmLteUeTimerConstants), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_ERROR,"MsmSmmDynCfgReq: smPackSendDynCfg failed");
               RETVALUE(RFAILED);
            } 
         }
         break;
         /*ronin*/
      case MSM_LTE_AC_BARRING_INFO_PARAMETERS:
         {
            ret = smPackSendDynCfg(cfg, sizeof(MsmLteAcBarringInfo), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_FATAL,"MsmSmmDynCfgReq: smPackSendDynCfg failed \n");
               RETVALUE(RFAILED);
            } 
         }
         break;
      case MSM_LTE_ENODEB_IP_PARAMETERS:
         {
            ret = smPackSendDynCfg(cfg, sizeof(MsmLteeNodeBIPparams), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_ERROR,"MsmSmmDynCfgReq: smPackSendDynCfg failed");
               RETVALUE(RFAILED);
            } 
         }
         break;
      case MSM_LTE_CELL_SIB3_CONFIG_GROUP_PARAMETERS:
         {
            ret = smPackSendDynCfg(cfg, sizeof(MsmLteCellSib3CfgGrp), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_ERROR,"MsmSmmDynCfgReq: smPackSendDynCfg failed");
               RETVALUE(RFAILED);
            } 
         }
         break;
      case MSM_LTE_DYNAMIC_CFI_PARAMETERS:
         {
            ret = smPackSendDynCfg(cfg, sizeof(MsmLteDynCfiGrp), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_ERROR,"MsmSmmDynCfgReq: smPackSendDynCfg failed for DYN CFI");
               RETVALUE(RFAILED);
            } 
         }
         break;
      case MSM_LTE_CELL_SIB1_CONFIG_GROUP_PARAMETERS:
         {
            ret = smPackSendDynCfg(cfg, sizeof(MsmLteCellSib1CfgGrp), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_ERROR,"MsmSmmDynCfgReq: smPackSendDynCfg failed");
               RETVALUE(RFAILED);
            }
         }
         break;
      case MSM_LTE_CELL_SIB6_CONFIG_GROUP_PARAMETERS:
         {
            ret = smPackSendDynCfg(cfg, sizeof(MsmLteCellSib6CfgGrp), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_ERROR,"MsmSmmDynCfgReq: smPackSendDynCfg failed");
               RETVALUE(RFAILED);
            }
         }
         break;
      case MSM_LTE_CELL_MIB_CONFIG_PARAMETERS:
         {
            ret = smPackSendDynCfg(cfg, sizeof(MsmLteCellMibCfgGrp), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_ERROR,"MsmSmmDynCfgReq: smPackSendDynCfg failed");
               RETVALUE(RFAILED);
            } 
         }
         break;
      case MSM_LTE_UTRA_NEIGH_FREQ_CFG_PARAMETERS:
         {
            ret = smPackSendDynCfg(cfg, sizeof(MsmLteNeighFreqCfg), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_ERROR,"MsmSmmDynCfgReq: smPackSendDynCfg failed");
               RETVALUE(RFAILED);
            } 
         }
         break;
      case MSM_LTE_EUTRAN_NEIGH_FREQ_CFG_PARAMETERS:
         {
            ret = smPackSendDynCfg(cfg, sizeof(MsmLteNeighFreqCfg), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_ERROR,"MsmSmmDynCfgReq: smPackSendDynCfg failed");
               RETVALUE(RFAILED);
            } 
         }
         break;
      case MSM_LTE_UTRA_NEIGHBORLIST_IN_USE_PARAMETERS:
         {
            ret = smPackSendDynCfg(cfg, sizeof(MsmNeighCellCfg), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_ERROR,"MsmSmmDynCfgReq: smPackSendDynCfg failed");
               RETVALUE(RFAILED);
            } 
         }
         break;
      case MSM_LTE_EUTRAN_NEIGHBORLIST_IN_USE_PARAMETERS:
         {
            ret = smPackSendDynCfg(cfg, sizeof(MsmDynNeighCellCfg), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_ERROR,"MsmSmmDynCfgReq: smPackSendDynCfg failed");
               RETVALUE(RFAILED);
            } 
         }
         break;
      case MSM_LTE_RAB_SRB1_POLICY_CFG_GRP_PARAMETERS:
      case MSM_LTE_RAB_SRB2_POLICY_CFG_GRP_PARAMETERS:
         {
            ret = smPackSendDynCfg(cfg, sizeof(MsmLteRabPolicyCfgGrp), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_ERROR,"MsmSmmDynCfgReq: smPackSendDynCfg failed");
               RETVALUE(RFAILED);
            } 
         } 
         break;
      case MSM_LTE_ENB_PROTO_CFG_PARAMETERS:
         {
            ret = smPackSendDynCfg(cfg, sizeof(MsmLteEnbProtoCfg), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_ERROR,"MsmSmmDynCfgReq: smPackSendDynCfg failed");
               RETVALUE(RFAILED); 
            }
         }
         break;
      case MSM_ENB_IP_ADDRESS:
        enbIp = (MsmEnodebIpAddr*)cfg;
        if(enbIp->ipv4pres == TRUE)
        {
           lteeNodeBparams.lteEnodebIpAddr.enbIpAddr = enbIp->enbIpAddr;
           lteeNodeBparams.lteEnodebIpAddr.ipv4pres = TRUE;
        }
        if(enbIp->ipv6pres == TRUE)      
        {
           cmMemcpy((U8*)lteeNodeBparams.lteEnodebIpAddr.enbIpv6Addr,(U8*)enbIp->enbIpv6Addr,sizeof(enbIp->enbIpv6Addr));
           lteeNodeBparams.lteEnodebIpAddr.ipv6pres = TRUE;
        }

        if(lteeNodeBparams.lteEnodebIpAddr.ipv4pres == TRUE)
        {
           smCfgCb.enbIpAddr[WR_IPV4_IDX].type = CM_TPTADDR_IPV4;
           smCfgCb.enbIpAddr[WR_IPV4_IDX].u.ipv4TptAddr.address = lteeNodeBparams.lteEnodebIpAddr.enbIpAddr;
           wrCpyCmTptAddr(&wrSmDfltEnbIpAddr[WR_IPV4_IDX],&smCfgCb.enbIpAddr[WR_IPV4_IDX]);
           wrCpyCmTptAddr(&smCfgCb.sctpIpAddr[WR_IPV4_IDX], &smCfgCb.enbIpAddr[WR_IPV4_IDX]);
        }
         

         cmMemset((U8*)&dynCfmInfo, 0, sizeof(MsmDynCfmInfo));
         dynCfmInfo.cfgType = MSM_ENB_IP_ADDRESS; 
         dynCfmInfo.action = MSM_ACT_MOD;

#ifdef TENB_STATS
         printf("\n ********ENB IP DYN CALLED *******\n");
         TSInfSetEnbIp(lteeNodeBparams.lteEnodebIpAddr.enbIpAddr);
#endif
         msmDynConfigComplete(MSM_CFM_SUCCESS,MSM_CAUSE_NA,dynCfmInfo);

         RETVALUE(ROK);
         break;
      case MSM_LTE_TM_PARAMETERS:
         {
            MsmTMCellConfig *modDynTmCellCfg = NULLP;
            modDynTmCellCfg = (MsmTMCellConfig *)cfg;
            if((lteeNodeBparams.lteTransModeCellCfg.transModeUse != modDynTmCellCfg->transModeUse) ||
               (lteeNodeBparams.lteTransModeCellCfg.bIsAutoConfigEnabled != modDynTmCellCfg->bIsAutoConfigEnabled
               ))
            {
               ret = smPackSendDynCfg(cfg, sizeof(MsmTMCellConfig), cfgType);
               if(ROK != ret)
               {
                  RLOG0(L_FATAL,"MsmSmmDynCfgReq: smPackSendDynCfg failed \n");
                  RETVALUE(RFAILED);
               }
            }
            else
            {
               cmMemset((U8*)&dynCfmInfo, 0, sizeof(MsmDynCfmInfo));
                  dynCfmInfo.cfgType = MSM_LTE_TM_PARAMETERS;
               dynCfmInfo.action = MSM_ACT_MOD;
               msmDynConfigComplete(MSM_CFM_SUCCESS,MSM_CAUSE_NA,dynCfmInfo);
               RETVALUE(ROK);

            }
         }
         break;
         /* ccpu00135220 */
      case MSM_LTE_SM_CELL_CONFIG_PARAMETERS:
         {
            ret = smPackSendDynCfg(cfg, sizeof(MsmLteSmCellCfg), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_ERROR,"MsmSmmDynCfgReq: smPackSendDynCfg failed");
               RETVALUE(RFAILED); 
            }
         }
         break;
      case MSM_LTE_PROC_TIMER_CFG_PARAMETERS:
         {
            ret = smPackSendDynCfg(cfg, sizeof(MsmLteProcTimerCfg), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_ERROR,"MsmSmmDynCfgReq: smPackSendDynCfg failed");
               RETVALUE(RFAILED); 
            }
         }
         break;
         /* ccpu00135220 */ 
       case MSM_LTE_CELL_DL_SCHD_CONFIG_PARAMETERS:
         {
            ret = smPackSendDynCfg(cfg, sizeof(MsmLteCellDlSchdCfgGrp), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_ERROR,"MsmSmmDynCfgReq: smPackSendDynCfg failed");
               RETVALUE(RFAILED);
            }
         }
         break;
         /*For SON dynamic PCI configuration*/
      case MSM_LTE_ADD_CELL_CONFIG_PARAMETERS :
         {
            ret = smPackSendDynCfg(cfg, sizeof(MsmLteAddCellCfg), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_ERROR,"MsmSmmDynCfgReq: smPackSendDynCfg failed");
               RETVALUE(RFAILED); 
            }
         }
         break;
      case MSM_TENB_STATS_CFG:
         {  
            ret = smPackSendDynCfg(cfg, sizeof(MsmTenbStatsCfg), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_FATAL,"MsmSmmDynCfgReq: smPackSendDynCfg failed \n");
               RETVALUE(RFAILED); 
            }
         }
         break;
  case MSM_LTE_FAP_ACCESS_PARAMETERS :
         {
            ret = smPackSendDynCfg(cfg, sizeof(MsmLteFapAccessParams), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_FATAL,"MsmLteFapAccessParams: smPackSendDynCfg failed \n");
               RETVALUE(RFAILED);
            }
         }
         break;

      case MSM_LTE_LOG_LVL_PARAMETERS:
         {
            ret = smPackSendDynCfg(cfg, sizeof(MsmEnbLogLvlData), cfgType);
            if(ROK != ret)
            {
               RLOG0(L_ERROR,"MsmSmmDynCfgReq: smPackSendDynCfg failed for LOG LEVEL");
               RETVALUE(RFAILED);
            } 
         }
         break;
      default:
         {
            RLOG0(L_ERROR,"MsmSmmDynCfgReq: Configuration Type received has\
                  No Handling.");
            RETVALUE(RFAILED);
         }
   } /* end of switch */
   RETVALUE(ROK);
} /* end of MsmSmmDynCfgReq */

/*ronin*/
PRIVATE Bool smChkSib2AcBarringMod
(
   MsmLteAcBarringCfg   *modLteAcBarringInfo,
   MsmLteAcBarringCfg   *lteAcBarringInfo
)
{

   if(lteAcBarringInfo->acBarringFactor !=
         modLteAcBarringInfo->acBarringFactor)
   {
      RETVALUE(TRUE);
   }
   if(lteAcBarringInfo->acBarringTime !=
         modLteAcBarringInfo->acBarringTime)
   {
      RETVALUE(TRUE);
   }
   if(lteAcBarringInfo->acBarringAC11 !=
         modLteAcBarringInfo->acBarringAC11)
   {
      RETVALUE(TRUE);
   }
   if(lteAcBarringInfo->acBarringAC12 !=
         modLteAcBarringInfo->acBarringAC12)
   {
      RETVALUE(TRUE);
   }
   if(lteAcBarringInfo->acBarringAC13 !=
         modLteAcBarringInfo->acBarringAC13)
   {
      RETVALUE(TRUE);
   }
   if(lteAcBarringInfo->acBarringAC14 !=
         modLteAcBarringInfo->acBarringAC14)
   {
      RETVALUE(TRUE);
   }
   if(lteAcBarringInfo->acBarringAC15 !=
         modLteAcBarringInfo->acBarringAC15)
   {
      RETVALUE(TRUE);
   }
   RETVALUE(FALSE);
}
/*ronin*/
PRIVATE U16 smHdlDynSib2AcBarringInfo
(
 Void *reCfgParams,
 LrmCellConfiguration *modCellCfg
)
{
   MsmLteAcBarringInfo  *modLteAcBarringInfo;
   MsmLteAcBarringInfo  *lteAcBarringInfo;
   U16   mask = 0x00;
   U8    acbMask = 0x00;

   modLteAcBarringInfo = (MsmLteAcBarringInfo*) reCfgParams;
   lteAcBarringInfo = &lteeNodeBparams.lteAcBarringInfo;
   
   
   /*Indicate to RRM whether MOSIG or MODATA info has been changed*/
   if(modLteAcBarringInfo->acBarringForEmergency != 
         lteAcBarringInfo->acBarringForEmergency)
   {
      mask |= LRM_CELL_RECFGTYPE_ACB_EMERGENCY;
      modCellCfg->stCellCfgInfo.stAcbInfo.bAcBarringEmergency = 
         modLteAcBarringInfo->acBarringForEmergency;
   }

   if(smChkSib2AcBarringMod(&modLteAcBarringInfo->acBarringForMOSignal,
                         &lteAcBarringInfo->acBarringForMOSignal) ||
      (modLteAcBarringInfo->barringSignalUsage != 
      lteAcBarringInfo->barringSignalUsage))
   {
      mask |= LRM_CELL_RECFGTYPE_ACB_MOSIG;

      modCellCfg->stCellCfgInfo.stAcbInfo.bIsacBarringMoSig = 
         modLteAcBarringInfo->barringSignalUsage;
      modCellCfg->stCellCfgInfo.stAcbInfo.stAcBarringMoSig.enAcbTime = 
         modLteAcBarringInfo->acBarringForMOSignal.acBarringTime; 
      modCellCfg->stCellCfgInfo.stAcbInfo.stAcBarringMoSig.enAcbFactor = 
         modLteAcBarringInfo->acBarringForMOSignal.acBarringFactor;

      if(modLteAcBarringInfo->acBarringForMOSignal.acBarringAC11)
      {
         acbMask |= LRM_ACB_SPECIAL_AC_11; 
      }
      if(modLteAcBarringInfo->acBarringForMOSignal.acBarringAC12)
      {
         acbMask |= LRM_ACB_SPECIAL_AC_12;
      }
      if(modLteAcBarringInfo->acBarringForMOSignal.acBarringAC13)
      {
         acbMask |= LRM_ACB_SPECIAL_AC_13;
      }
      if(modLteAcBarringInfo->acBarringForMOSignal.acBarringAC14)
      {
         acbMask |= LRM_ACB_SPECIAL_AC_14;
      }
      if(modLteAcBarringInfo->acBarringForMOSignal.acBarringAC15)
      {
         acbMask |= LRM_ACB_SPECIAL_AC_15;
      }
      modCellCfg->stCellCfgInfo.stAcbInfo.stAcBarringMoSig.bAcbForSpac = acbMask;
   }

   if(smChkSib2AcBarringMod(&modLteAcBarringInfo->acBarringForMOData,
                         &lteAcBarringInfo->acBarringForMOData) ||
       (modLteAcBarringInfo->barringDataUsage != 
      lteAcBarringInfo->barringDataUsage))
   {
      mask |= LRM_CELL_RECFGTYPE_ACB_MODATA;
   
      modCellCfg->stCellCfgInfo.stAcbInfo.bIsacBarringMoData = 
         modLteAcBarringInfo->barringDataUsage;
      modCellCfg->stCellCfgInfo.stAcbInfo.stAcBarringMoData.enAcbTime = 
         modLteAcBarringInfo->acBarringForMOData.acBarringTime; 
      modCellCfg->stCellCfgInfo.stAcbInfo.stAcBarringMoData.enAcbFactor = 
         modLteAcBarringInfo->acBarringForMOData.acBarringFactor;

      if(modLteAcBarringInfo->acBarringForMOData.acBarringAC11)
      {
         acbMask |= LRM_ACB_SPECIAL_AC_11; 
      }
      if(modLteAcBarringInfo->acBarringForMOData.acBarringAC12)
      {
         acbMask |= LRM_ACB_SPECIAL_AC_12;
      }
      if(modLteAcBarringInfo->acBarringForMOData.acBarringAC13)
      {
         acbMask |= LRM_ACB_SPECIAL_AC_13;
      }
      if(modLteAcBarringInfo->acBarringForMOData.acBarringAC14)
      {
         acbMask |= LRM_ACB_SPECIAL_AC_14;
      }
      if(modLteAcBarringInfo->acBarringForMOData.acBarringAC15)
      {
         acbMask |= LRM_ACB_SPECIAL_AC_15;
      }
      modCellCfg->stCellCfgInfo.stAcbInfo.stAcBarringMoData.bAcbForSpac = acbMask;
   }

   RETVALUE(mask);
}

/*
*
*       Fun:   smDynCfgReq
*
*       Desc:  Dynamic configuration handler function from MSM to enbapp
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_dyncfg.c
*
*/ 

PUBLIC S16 smDynCfgReq(Void *cfg, U32 cfgType, U8 priority)
{
   Pst          dyPst;
   LwrDynMngmt  *lwrDynMngmt = NULLP; 
   MsmLteAddCellCfg *cellAddCfg = NULLP; 
   U32          ret;
   Header       *hdr;
   Bool         sendCfg = FALSE;
   U16   maxCsgUEs;
   U16   maxNonCsgUEs;
   MsmLteFapAccessParams *fapAccsCfg = NULLP;
   LrmCellConfiguration   cellCfg;
   TRC2(smDynCfgReq);

   RLOG1(L_ALWAYS, "Received DYNAMIC CFG REQ for %d", cfgType);
   WR_ALLOC(&lwrDynMngmt, sizeof(LwrDynMngmt));
   SM_SET_ZERO(&dyPst, sizeof(Pst));
   SM_SET_ZERO(&cellCfg, sizeof(LrmCellConfiguration));

   if(lwrDynMngmt == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");        
      RETVALUE(RFAILED);
   }
   if (cfgType >= MSM_LTE_CONFIGURATION_COMPLETE_END)
   {
      RLOG0(L_ERROR, "Configuration Type is Out of range. Returning RFAILED");
      WR_FREE(lwrDynMngmt, sizeof(LwrDynMngmt));
      RETVALUE(RFAILED);
   }

   switch(cfgType)
   {
      case MSM_LTE_PRACH_CONFIG_COMMON_PARAMETERS:
      case MSM_LTE_RACH_CONFIG_COMMON_PARAMETERS:
      case MSM_LTE_PDSCH_CONFIG_COMMON_PARAMETERS:
      case MSM_LTE_SRS_CFG_COMMON_PARAMETERS:
      case MSM_LTE_PUSCH_REF_SIGNAL_CFG_PARAMETERS:
      case MSM_LTE_UL_POWER_CONTROL_COMMON_PARAMETERS:
      case MSM_LTE_UL_FREQUENCY_INFO_PARAMETERS:
      case MSM_LTE_PUSCH_BASIC_CFG_COMMON_PARAMETERS:
      case MSM_LTE_UE_TIMER_CONSTANTS_PARAMETERS:
         { 
            smHdlDynSib2Cfg(cfgType, cfg, &lwrDynMngmt->dynCfg);
            lwrDynMngmt->cfgType = LWR_DYN_EVNT_CELL_SIB2_CONFIG_GROUP_PRAMS; 
            sendCfg = TRUE;
         }
         break;
      case MSM_LTE_AC_BARRING_INFO_PARAMETERS:/*ronin*/
         {
            MsmDynCfmInfo dynCfmInfo;

            U16 mask = 0x0000;
            MsmLteAcBarringInfo  *lteAcBarringInfo;

            mask = smHdlDynSib2AcBarringInfo(cfg, &cellCfg);

            lteAcBarringInfo = (MsmLteAcBarringInfo*)cfg;
            
            cellCfg.usReCfgType = mask;
            smSndRmuProtModCellCfg(1 ,cellCfg);

            /*UPDATE lteeNodeBparams with dynamic values*/
            msmSetAcBarringInfoParameters(lteAcBarringInfo);
            dynCfmInfo.cfgType = MSM_LTE_AC_BARRING_INFO_PARAMETERS; 
            dynCfmInfo.action = MSM_ACT_MOD;
            msmDynConfigComplete(MSM_CFM_SUCCESS,MSM_CAUSE_NA,dynCfmInfo);
         }
         break;
      case MSM_LTE_ENODEB_IP_PARAMETERS:
         {
            smHdlDynMmeCfg((MsmLteeNodeBIPparams *) cfg);
         }
         break;
      case MSM_LTE_TM_PARAMETERS:
         {
            MsmTMCellConfig *modDynTmCellCfg = NULLP;
            modDynTmCellCfg = (MsmTMCellConfig *)cfg;
            LwrTxnModeInfo *txModeCfg = NULLP;
            lteeNodeBparams.lteTransModeCellCfg.transModeUse = modDynTmCellCfg->transModeUse;
            WR_ALLOC(&txModeCfg, sizeof(LwrTxnModeInfo));
            if(NULLP == txModeCfg)
            {
               RLOG0(L_FATAL, "Failed to allocate mem for tm mode chnage");
               WR_FREE(lwrDynMngmt, sizeof(LwrDynMngmt));
               RETVALUE(RFAILED);
            }
            txModeCfg->txPrefMode = modDynTmCellCfg->transModeUse; 
            txModeCfg->isAutoCfgModeEnb = modDynTmCellCfg->bIsAutoConfigEnabled;
            lteeNodeBparams.lteTransModeCellCfg.bIsAutoConfigEnabled = modDynTmCellCfg->bIsAutoConfigEnabled;
            lwrDynMngmt->dynCfg = (Void*)txModeCfg;
            lwrDynMngmt->cfgType = LWR_DYN_EVNT_CELL_TM_CONFIG_PRAMS; 
            sendCfg = TRUE;

            /* Sending Dynamic Config request to RRM */
            cellCfg.stCellCfgInfo.flagTransMode = modDynTmCellCfg->transModeUse;
            cellCfg.usReCfgType |= LRM_CELL_RECFGTYPE_TRANS_MODE;
            smSndRmuProtModCellCfg(1, cellCfg);
         }
         break;
      case MSM_LTE_CELL_SIB3_CONFIG_GROUP_PARAMETERS:
         {
            ret = smHdlDynSib3Cfg((MsmLteCellSib3CfgGrp *)cfg, &lwrDynMngmt->dynCfg);
            if(ret != ROK)
            {
               RLOG0(L_ERROR, "smHdlDynSib3Cfg: failed");
               WR_FREE(lwrDynMngmt, sizeof(LwrDynMngmt));
               RETVALUE(RFAILED);
            }
            lwrDynMngmt->cfgType = LWR_DYN_EVNT_CELL_SIB3_CONFIG_GROUP_PRAMS; 
            sendCfg = TRUE;
         }
         break;
      case MSM_LTE_DYNAMIC_CFI_PARAMETERS:
         {
            MsmLteDynCfiGrp  *modDynCfiCfg; 
            modDynCfiCfg =    (MsmLteDynCfiGrp *)cfg;
            LwrCellSchdCfg   *dynCfiCfg = NULLP;
            if(lteeNodeBparams.lteDynCfiParams.isDynCfiEnb != modDynCfiCfg->isDynCfiEnb) /*Check against old value*/
            {
               WR_ALLOC(&dynCfiCfg, sizeof(LwrCellSchdCfg));
               if(dynCfiCfg == NULLP)
               {
                  RLOG0(L_FATAL, "Memory allocation failed.");
                  WR_FREE(lwrDynMngmt, sizeof(LwrDynMngmt));
                  RETVALUE(RFAILED);
               }
               dynCfiCfg->isDynCfiEnb      =  modDynCfiCfg->isDynCfiEnb;

               /* Once the Dynamic Configuration Framework is ready, this value wil be 
                * updated after receiving confirmation from the application */
               lteeNodeBparams.lteDynCfiParams.isDynCfiEnb = modDynCfiCfg->isDynCfiEnb; /*Update the old value*/
               lwrDynMngmt->dynCfg = (Void*)dynCfiCfg;
            }
            lwrDynMngmt->cfgType = LWR_DYN_EVNT_CELL_DYN_CFI_CONFIG_PRAMS; 
            sendCfg = TRUE;
         }
         break;
      case MSM_LTE_CELL_SIB1_CONFIG_GROUP_PARAMETERS:
         {
            ret = smHdlDynSib1Cfg((MsmLteCellSib1CfgGrp *)cfg, &lwrDynMngmt->dynCfg);
            if(ret != ROK)
            {
               RLOG0(L_ERROR, "smHdlDynSib1Cfg: failed");
               WR_FREE(lwrDynMngmt, sizeof(LwrDynMngmt));
               RETVALUE(RFAILED);
            }
            lwrDynMngmt->cfgType = LWR_DYN_EVNT_CELL_SIB1_CONFIG_GROUP_PRAMS;
            sendCfg = TRUE;
         }
         break;
      case MSM_LTE_CELL_SIB6_CONFIG_GROUP_PARAMETERS:
         {
            ret = smHdlDynSib6Cfg((MsmLteCellSib6CfgGrp *)cfg, &lwrDynMngmt->dynCfg);
            if(ret != ROK)
            {
               RLOG0(L_ERROR, "smHdlDynSib6Cfg: failed");
               WR_FREE(lwrDynMngmt, sizeof(LwrDynMngmt));
               RETVALUE(RFAILED);
            }
            lwrDynMngmt->cfgType = LWR_DYN_EVNT_CELL_SIB6_CONFIG_GROUP_PRAMS;
            sendCfg = TRUE;
         }
         break;
      case MSM_LTE_CELL_MIB_CONFIG_PARAMETERS:
         {
            ret = smHdlDynMibCfg((MsmLteCellMibCfgGrp*)cfg, &lwrDynMngmt->dynCfg);
            if(ret != ROK)
            {
               RLOG0(L_ERROR, "MsmLteCellMibCfgGrp: failed ");
               WR_FREE(lwrDynMngmt, sizeof(LwrDynMngmt));
               RETVALUE(RFAILED);
            } 
            lwrDynMngmt->cfgType = LWR_DYN_EVNT_CELL_MIB_CONFIG_PRAMS;
            sendCfg = TRUE;
         }
         break;
      case MSM_LTE_UTRA_NEIGH_FREQ_CFG_PARAMETERS:
         {
            ret = smHdlDynNeighUtraFreqCfg((MsmLteNeighFreqCfg*)cfg);
         }
         break;
      case MSM_LTE_EUTRAN_NEIGH_FREQ_CFG_PARAMETERS:
         {
            smDynTmrEvent = LWR_DYN_EVNT_NEIGH_EUTRA_FREQ_CFG; 
            ret = smHdlDynNeighEutraFreqCfg((MsmLteNeighFreqCfg*)cfg);
         }
         break;
      case MSM_LTE_UTRA_NEIGHBORLIST_IN_USE_PARAMETERS:
         {
            ret = smHdlDynNeighUtraCellCfg((MsmNeighCellCfg*)cfg);
         }
         break;
      case MSM_LTE_EUTRAN_NEIGHBORLIST_IN_USE_PARAMETERS:
         {
            smDynTmrEvent = LWR_DYN_EVNT_NEIGH_EUTRA_CELL_CFG; 

            ret = smHdlSonDynNeighEutraCellCfg((MsmDynNeighCellCfg*)cfg);
            ret = smHdlDynNeighEutraCellCfg((MsmDynNeighCellCfg*)cfg);
         }
         break;
      case MSM_LTE_RAB_SRB1_POLICY_CFG_GRP_PARAMETERS:
      case MSM_LTE_RAB_SRB2_POLICY_CFG_GRP_PARAMETERS:
         {
            ret = smHdlDynRabPolicyCfg(cfgType, (MsmLteRabPolicyCfgGrp*)cfg, &lwrDynMngmt->dynCfg);
            if(ret != ROK)
            {
               RLOG0(L_ERROR, "smHdlDynRabPolicyCfg: failed");
               WR_FREE(lwrDynMngmt, sizeof(LwrDynMngmt));
               RETVALUE(RFAILED);
            } 

            lwrDynMngmt->cfgType = LWR_DYN_EVNT_CELL_RABPOLICY_CONFIG_GROUP_PRAMS;
            sendCfg = TRUE;
         } 
         break;
      case MSM_LTE_ENB_PROTO_CFG_PARAMETERS:
         {
            if(ROK != smHdlDynEnbProtoCfg((MsmLteEnbProtoCfg *)cfg, &lwrDynMngmt->dynCfg))
            {
               RLOG0(L_ERROR, "smHdlDynEnbProtoCfg: failed");
               WR_FREE(lwrDynMngmt, sizeof(LwrDynMngmt));
               RETVALUE(RFAILED); 
            }
            lwrDynMngmt->cfgType = LWR_DYN_EVNT_ENB_PROTO_CONFIG_PRAMS;
            sendCfg = TRUE;
         }
         break;
         /* ccpu00135220 */
      case MSM_LTE_SM_CELL_CONFIG_PARAMETERS:
         {
            if(ROK != smHdlDynSmCellCfg((MsmLteSmCellCfg *)cfg))
            {
               RLOG0(L_ERROR, "smHdlDynSmCellCfg: failed");
               WR_FREE(lwrDynMngmt, sizeof(LwrDynMngmt));
               RETVALUE(RFAILED); 
            }
            lwrDynMngmt->cfgType = LWR_DYN_EVNT_SM_CELL_CONFIG_PRAMS;
            sendCfg = TRUE;
         }
         break;
      case MSM_LTE_PROC_TIMER_CFG_PARAMETERS:
         {
            if(ROK != smHdlDynTimerCfg((MsmLteProcTimerCfg *)cfg,
                     &lwrDynMngmt->dynCfg))
            {
               RLOG0(L_ERROR, "smHdlDynTimerCfg: failed");
               WR_FREE(lwrDynMngmt, sizeof(LwrDynMngmt));
               RETVALUE(RFAILED); 
            }
             lwrDynMngmt->cfgType = LWR_DYN_EVNT_CELL_TIMER_CONFIG_GROUP_PRAMS;
             sendCfg = TRUE;
          }
          break;
      /*For SON dynamic PCI configuration*/
      case MSM_LTE_ADD_CELL_CONFIG_PARAMETERS :
         {
           MsmDynCfmInfo dynCfmInfo;
           cellAddCfg = (MsmLteAddCellCfg *)cfg;
           lteeNodeBparams.lteAddCellCfg.pciListCfg.numPci = 
                cellAddCfg->pciListCfg.numPci;
           if((lteeNodeBparams.lteAddCellCfg.minDlResNonCsg != 
               cellAddCfg->minDlResNonCsg) ||
                (lteeNodeBparams.lteAddCellCfg.minUlResNonCsg  !=
                 cellAddCfg->minUlResNonCsg))
           {
              lteeNodeBparams.lteAddCellCfg.minDlResNonCsg  = 
                 cellAddCfg->minDlResNonCsg ;
              lteeNodeBparams.lteAddCellCfg.minUlResNonCsg = 
                 cellAddCfg->minUlResNonCsg;
              if(ROK != smHdlDynCellCfg(cellAddCfg, &lwrDynMngmt->dynCfg))
              {
                  RLOG0(L_FATAL, "smHdlDynCellCfg: failed \n");
                  WR_FREE(lwrDynMngmt, sizeof(LwrDynMngmt));
                  RETVALUE(RFAILED); 
              }
              lwrDynMngmt->cfgType = LWR_DYN_EVNT_ADD_CELL_CONFIG_PRAMS;
              sendCfg = TRUE;
           }
           cmMemcpy((U8 *)(lteeNodeBparams.lteAddCellCfg.pciListCfg.pciLst),
            (U8 *) (cellAddCfg->pciListCfg.pciLst),
             sizeof(cellAddCfg->pciListCfg.pciLst[0]) * (cellAddCfg->pciListCfg.numPci));

           smSendNlCellReCfg();
           if(sendCfg != TRUE)
           {
           RLOG0(L_DEBUG, "Sending Add Cell Dynamic Config Cfm for PCI List Modification");
           dynCfmInfo.cfgType = MSM_LTE_ADD_CELL_CONFIG_PARAMETERS; 
           dynCfmInfo.action = MSM_ACT_MOD;
           msmDynConfigComplete(MSM_CFM_SUCCESS,MSM_CAUSE_NA,dynCfmInfo);
         }
         }
         break;
         /* ccpu00135220 */
       case MSM_LTE_CELL_DL_SCHD_CONFIG_PARAMETERS:
         {
            if(ROK != smHdlDynSchdCfg((MsmLteCellDlSchdCfgGrp*)cfg,
                  &lwrDynMngmt->dynCfg))
            {
               RLOG0(L_ERROR, "smHdlDynSchdCfg: failed");
               RETVALUE(RFAILED);
            }
             lwrDynMngmt->cfgType = LWR_DYN_EVNT_CELL_SCHD_CONFIG_GROUP_PRAMS;
             sendCfg = TRUE;
         }
         break;
      case MSM_LTE_LOG_LVL_PARAMETERS:
         {
            MsmDynCfmInfo dynCfmInfo;
            MsmEnbLogLvlData  *modDynLogLvl; 
            modDynLogLvl =    (MsmEnbLogLvlData *)cfg;
            dynCfmInfo.cfgType = MSM_LTE_LOG_LVL_PARAMETERS; 
            dynCfmInfo.action = MSM_ACT_MOD;
            if(lteeNodeBparams.lteLogLvlCfg.enbL3LogLvl != modDynLogLvl->enbL3LogLvl) /*Check against old value*/
            {
               RLOG2(L_ALWAYS, "L3_LOG_LEVEL is being modified from %d to %d", lteeNodeBparams.lteLogLvlCfg.enbL3LogLvl, modDynLogLvl->enbL3LogLvl);
               rlSetLogLevel(modDynLogLvl->enbL3LogLvl);
               lteeNodeBparams.lteLogLvlCfg.enbL3LogLvl = modDynLogLvl->enbL3LogLvl; /*Update the old value*/
            }
            if(lteeNodeBparams.lteLogLvlCfg.enbL2LogLvl != modDynLogLvl->enbL2LogLvl) /*Check against old value*/
            {
               RLOG2(L_ALWAYS, "L2_LOG_LEVEL is being modified from %d to %d", lteeNodeBparams.lteLogLvlCfg.enbL2LogLvl, modDynLogLvl->enbL2LogLvl);
               /* Invoke L2 RLOG function to set modified Log Level */
#ifdef BRDCM_RLOG
               smYsSendRlogInd(modDynLogLvl->enbL2LogLvl);
#endif
               lteeNodeBparams.lteLogLvlCfg.enbL2LogLvl = modDynLogLvl->enbL2LogLvl; /*Update the old value*/
            }
            if(lteeNodeBparams.lteLogLvlCfg.remoteLogin != modDynLogLvl->remoteLogin)
            {
               RLOG2(L_ALWAYS, "ENBLOG_REMOTE_LOGGING is being modified from %d to %d", lteeNodeBparams.lteLogLvlCfg.enbL2LogLvl , modDynLogLvl->enbL2LogLvl );
               rlSetRemoteLoggingFlag(modDynLogLvl->enbL2LogLvl);
               lteeNodeBparams.lteLogLvlCfg.remoteLogin = modDynLogLvl->remoteLogin;
            }
            if(lteeNodeBparams.lteLogLvlCfg.enbLog_mask != modDynLogLvl->enbLog_mask)
            {
               RLOG2(L_ALWAYS, "ENBLOG_MASK is being modified from %d to %d", lteeNodeBparams.lteLogLvlCfg.enbLog_mask, modDynLogLvl->enbLog_mask);
               rlSetModuleMask(modDynLogLvl->enbLog_mask);
               lteeNodeBparams.lteLogLvlCfg.enbLog_mask = modDynLogLvl->enbLog_mask;
            }
           RLOG0(L_ALWAYS, "Sending Log Level Config Cfm for Modification ");
           msmDynConfigComplete(MSM_CFM_SUCCESS,MSM_CAUSE_NA,dynCfmInfo);
         }
         break;
      case MSM_TENB_STATS_CFG:
#ifdef TENB_STATS
         {
            MsmTenbStatsCfg *tsCfg =(MsmTenbStatsCfg *)cfg;
            if  (TSInfInitStatsFrmWork(tsCfg->tenbSocketStats,
                                       tsCfg->tenbFileStats,
                                       tsCfg->tenbConsoleStats,
                                       tsCfg->tenbStatsPeriodicity,
                                       tsCfg->tendStatsSocketPort) != ROK)
            {
               RLOG0(L_ERROR, "TENB_STATS Wrong Cfgn");
               RETVALUE(RFAILED);
            }
         }
#endif
         break;
       case MSM_LTE_FAP_ACCESS_PARAMETERS:
         {
           fapAccsCfg = (MsmLteFapAccessParams *)cfg;
           if((lteeNodeBparams.lteFapAccessParams.maxCSGMembers != 
               fapAccsCfg->maxCSGMembers) ||
                (lteeNodeBparams.lteFapAccessParams.maxNonCSGMembers  !=
                fapAccsCfg->maxNonCSGMembers))
           {
              maxCsgUEs = fapAccsCfg->maxCSGMembers;
              maxNonCsgUEs = fapAccsCfg->maxNonCSGMembers;
              if((maxCsgUEs > smCfgCb.maxUeSupp) || 
                    (maxNonCsgUEs > smCfgCb.maxUeSupp) ||
                    ((maxNonCsgUEs == maxCsgUEs) && 
                     (maxCsgUEs < smCfgCb.maxUeSupp) && 
                    ((maxCsgUEs + maxNonCsgUEs) != smCfgCb.maxUeSupp)) ||
                     (maxCsgUEs + maxNonCsgUEs) < smCfgCb.maxUeSupp)
              {
                 printf("Invalid Non CSG UE CFGD [%d] CSG UE CFGD "
                      "[%d] Max UE Supported [%d]", maxNonCsgUEs,
                       maxCsgUEs, smCfgCb.maxUeSupp);
                 RETVALUE(ROK);
              }
              lteeNodeBparams.lteFapAccessParams.maxCSGMembers =
                           fapAccsCfg->maxCSGMembers;
              lteeNodeBparams.lteFapAccessParams.maxNonCSGMembers  =
                fapAccsCfg->maxNonCSGMembers;
              if(ROK != smHdlDynFapAccsCfg(fapAccsCfg, &lwrDynMngmt->dynCfg))
              {
                  RLOG0(L_FATAL, "smHdlDynFapAccsCfg: failed \n");
                  WR_FREE(lwrDynMngmt, sizeof(LwrDynMngmt));
                  RETVALUE(RFAILED); 
              }
              lwrDynMngmt->cfgType = LWR_DYN_EVNT_FAP_ACCS_PRAMS;
              sendCfg = TRUE;
               /*Passing Cell ID as 1 as of now, because this CellID is
               * internal to enodeB and RRM . TODO */
              //ronin: write a util func for filling csg details
              smFillRmuProtModCellCfgWithCsg(&cellCfg,fapAccsCfg);
              smSndRmuProtModCellCfg((U8)lteeNodeBparams.lteCellSib1CfgGrp.cellAccessInfo.cellId
                    ,cellCfg);
           }
         }
         break;
      default:
         {
            RLOG0(L_ERROR, "Configuration Type received has No Handling. ");
            WR_FREE(lwrDynMngmt, sizeof(LwrDynMngmt));
            RETVALUE(RFAILED);
         }
   } /* end of switch */
   /* Sending the Dynamic configuration  request to the eNB app */
   if (sendCfg != FALSE)
   {
      hdr = &lwrDynMngmt->hdr;
      /* Fill Header */
      hdr->msgType             = TCFG;
      hdr->msgLen              = 0;
      hdr->entId.ent           = ENTWR;
      hdr->entId.inst          = SM_INST_ZERO;
      hdr->elmId.elmnt         = STWRDYNCFG;
      hdr->elmId.elmntInst1    = (ElmntInst1)lwrDynMngmt->cfgType;
      hdr->elmId.elmntInst2    = 0;
      hdr->elmId.elmntInst3    = 0;
      hdr->seqNmb              = 0;
      hdr->version             = 0; 
      LWR_GEN_TRANSID(hdr->transId, cfgType,  (smCb.transIdMdfy));
      hdr->response.prior      = PRIOR0;
      hdr->response.route      = RTESPEC;
      hdr->response.mem.region = smCb.init.region;
      hdr->response.mem.pool   = WR_POOL;
      hdr->response.selector   = SM_SELECTOR_LWLC;
      /* Fill Pst */
      dyPst.selector  = SM_SELECTOR_LWLC;
      dyPst.srcEnt    = ENTSM;
      dyPst.dstEnt    = ENTWR;
      dyPst.dstProcId = SM_WR_PROC;
      dyPst.srcProcId = SM_SM_PROC;
      dyPst.event    = EVTLWRDYNCFGREQ;
      SmMiLwrDynCfgReq(&dyPst, lwrDynMngmt); 

   }
   else
   {
      WR_FREE(lwrDynMngmt, sizeof(LwrDynMngmt));
   }
   RETVALUE(ROK);
} /* end of MsmSmmDynCfgReq */

 /*
 * 
 *        Fun: smSendDynCfgReq
 * 
 *        Desc:  fill in send dynamic cfg request to FSM 
 * 
 *        Ret:   ROK
 * 
 *        Notes: None
 *               
 *        File:  wr_sm_dyncfg.c 
 */
#ifdef ANSI
PUBLIC S16 smSendDynCfgReq(MsmSubscriptionGroup msmCfgType, 
                              LwrDynCfgEvntType lwrCfgType,Void *data)
#else /* if ANSI not defined*/
PUBLIC S16 smSendDynCfgReq(msmCfgType, lwrCfgType, data)
MsmSubscriptionGroup msmCfgType;
LwrDynCfgEvntType    lwrCfgType;
Void                 *data;
#endif /* end of ANSI*/
{
   Pst    dyPst;
   LwrDynMngmt  *lwrDynMngmt = NULLP; 
   Header       *hdr;

   TRC2(smSendDynCfgReq);

   WR_ALLOC(&lwrDynMngmt, sizeof(LwrDynMngmt));
   SM_SET_ZERO(&dyPst, sizeof(Pst));

   RLOG1(L_INFO,"Triggering smSendDynCfgReq to OAM: Config Type %d\n", msmCfgType);
   if(lwrDynMngmt == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   lwrDynMngmt->cfgPriority = 0;
   lwrDynMngmt->dynCfg = data;
   lwrDynMngmt->cfgType = lwrCfgType;

   hdr = &lwrDynMngmt->hdr;
   /* Fill Header */
   hdr->msgType             = TCFG;
   hdr->msgLen              = 0;
   hdr->entId.ent           = ENTWR;
   hdr->entId.inst          = SM_INST_ZERO;
   hdr->elmId.elmnt         = STWRDYNCFG;
   hdr->elmId.elmntInst1    = (ElmntInst1)lwrDynMngmt->cfgType;
   hdr->elmId.elmntInst2    = 0;
   hdr->elmId.elmntInst3    = 0;
   hdr->seqNmb              = 0;
   hdr->version             = 0; 
   LWR_GEN_TRANSID(hdr->transId, msmCfgType,  (smCb.transIdMdfy));
   hdr->response.prior      = PRIOR0;
   hdr->response.route      = RTESPEC;
   hdr->response.mem.region = smCb.init.region;
   hdr->response.mem.pool   = WR_POOL;
   hdr->response.selector   = SM_SELECTOR_LWLC;
   /* Fill Pst */
   dyPst.selector  = SM_SELECTOR_LWLC;
   dyPst.srcEnt    = ENTSM;
   dyPst.dstEnt    = ENTWR;
   dyPst.dstProcId = SM_WR_PROC;
   dyPst.srcProcId = SM_SM_PROC;
   dyPst.event    = EVTLWRDYNCFGREQ;
   /* starting Tmr for Dyn Cfg */
   smStartTmr((PTR)&(smCb),SM_TMR_DYN_CFG_TMR, SM_TMR_DYN_CFG_TMR_VAL);
   SmMiLwrDynCfgReq(&dyPst, lwrDynMngmt); 
   RETVALUE(ROK);
} /*end of smSendDynCfgReq*/

/********************************************************************30**

           End of file:     fc_smm_dyncfg.c@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/Br_Tenb_Mspd_T2K_Intg_MS8.0_RC/2 - Mon Jul 29 15:49:07 2013

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
SID$        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/

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

/**************************************************************************************************

     Name:     UMTS REM - BCH DECODE

     Type:     C++ Source file

     Desc:     This file is interface between UMTS decoder and rem

     File:     Pasn_BchDecodeParams.cpp

     Sid:      Pasn_BchDecodeParams.cpp@@/main/tenb_main_ccb/1 - Thu Jan 21 17:51:26 2016

     Prg:      Amit Ghosh, Rohit Kumar 


**************************************************************************************************/


///////////////////////////////////////////////////////////////////////////////
// System Includes
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <fstream>
//#include <messaging/messages/cphy/CPhyRemSibDecodeInd.h>


///////////////////////////////////////////////////////////////////////////////
// Local Includes
///////////////////////////////////////////////////////////////////////////////

//#include <system/Trace.h>
#include "pasn_lib25331.h"
#include"Pasn_BchDecodeParams.h"

using namespace threeway;
using namespace boost;

///////////////////////////////////////////////////////////////////////////////
// Class Functions
///////////////////////////////////////////////////////////////////////////////

Pasn_BchDecodeParams::Pasn_BchDecodeParams()
{
   Reset();
}

Pasn_BchDecodeParams::~Pasn_BchDecodeParams()
{
   //nothing to do
}

bool Pasn_BchDecodeParams::QuerySibTypeNeedsProcessing(U32 sibType)
{

   switch(sibType)
   {
      case RruSIB_TypmastInfBlockEnum:
         {
            return !m_decodedMIB;
         }
      case RruSIB_TypsysInfoBlockTyp1Enum:
         {
            return !m_decodedSIB1;
         }
      case RruSIB_TypsysInfoBlockTyp3Enum:
         {
            return !m_decodedSIB3;
         }
      case RruSIB_TypsysInfoBlockTyp5Enum:
         {
            return !m_decodedSIB5;
         }
      case RruSIB_TypsysInfoBlockTyp6Enum:
         {
            return !m_decodedSIB6;
         }
      case RruSIB_TypsysInfoBlockTyp15Enum:
         {
            return !m_decodedSIB15;
         }
      case RruSIB_TypschengBlock1Enum:
         {
            return !m_decodedSB1;
         } 
   }

   return false;
}

void Pasn_BchDecodeParams::MarkSibTypeProcessed(U32 sibType)
{
   if ((sibType <= RRU_SIB_20) && (sibType >= RRU_SIB_1))
   {
      m_sibTypeStatus[sibType].decoded = true;
   }
}

void Pasn_BchDecodeParams::Reset()
{
   m_consecutiveCrcFails = 0;
   m_bcchBchCount = 0;

   memset(&m_sibTypeStatus[0], 0, sizeof(m_sibTypeStatus));

   // initializing with invalid value

   m_mcc           = 0;
   m_mnc           = 0;
   m_lac           = 0;
   m_rac           = 0;
   m_cellId        = 0;
   m_pcpichTxPwr   = 0;
   m_csgId         = 0;
   m_plmnType      = 20; 
   m_csgInd        = false;
   m_startPsc      = 0;
   m_numOfPsc      = 20;
   m_pscRange      = 0;
   m_latiSign      = 0;
   m_latitude      = 0;
   m_longitude     = 0;
   m_unSemiMajor   = 0;
   m_unSemiMinor   = 0;
   m_orieMajorAxis = 0;
   m_confid        = 0;

   m_transmitSB1   = false;
   m_transmitSIB1  = false;
   m_transmitSIB3  = false;
   m_transmitSIB5  = false;
   m_transmitSIB6  = false;
   m_transmitSIB15 = false;

   m_decodedMIB    = false; 
   m_decodedSB1    = false; 
   m_decodedSIB1   = false;
   m_decodedSIB3   = false;
   m_decodedSIB5   = false;
   m_decodedSIB6   = false;
   m_decodedSIB11  = false;
   m_decodedSIB15  = false;

   m_sib11String.clear();
}

void Pasn_BchDecodeParams::SibDecodePlmnIdentity(RruPLMN_Ident * plmnIdentity)
{
   RruMCC *mcc = &plmnIdentity->mcc;

   m_mcc = 0;

   if(mcc->noComp.pres != NOTPRSNT)
   {
      U16 i;
      for(i = 0; i < mcc->noComp.val; i++)
      {
         m_mcc *= 10;
         m_mcc += mcc->member[i].val;
      }
   }

   RruMNC *mnc = &plmnIdentity->mnc;

   m_mnc = 0;

   if(mnc->noComp.pres != NOTPRSNT)
   {
      U16 i;
      for(i = 0; i < mnc->noComp.val; i++)
      {
         m_mnc *= 10;
         m_mnc += mnc->member[i].val;
      }
   }

   TRACE_PRINTF("Decoded MCC=%u MNC=%u\n", m_mcc, m_mnc);

}

void Pasn_BchDecodeParams::ProcessSibType(RruSIB_Typ   sibType,
      const U8 * octets,
      u32        length)
{

   CPASN_LIB25331 s_pasn_lib25331;

   TRACE_PRINTF("%s: SIB_Type=%d\n",__FUNCTION__,(unsigned int)sibType.val);

   if (!QuerySibTypeNeedsProcessing(sibType.val))
   {
      TRACE_PRINTF("%s: SIB_Type=%d already decoded/ignored\n",__FUNCTION__,(u32)sibType.val);
      return;
   }

   switch (sibType.val)
   {
      case RruSIB_TypmastInfBlockEnum:
         {
            TRACE_PRINTF("%s: RRU_SIB_MIB\n",__FUNCTION__);

            RruMasteInfBlock *mib = (RruMasteInfBlock*) s_pasn_lib25331.Decode_BCCH_BCH(octets, length, RruSIB_TypmastInfBlockEnum, false);        

            if (mib)
            {
               TRACE_PRINTF(" In MIB AFTER DECODE \n");
               /* Sending to check for SIB's info in Mib which are 
                * being transmitted */
               checkTransmittedSibInMib(&(mib->sibSb_ReferList));

               /* check for ext. Sibs */
               //checkTransmittedExtSibInMib(mib); 

               RruPLMN_Typ * plmnType = &mib->plmn_Typ;
               TRACE_PRINTF("MIB received, token presnt=%d\n",mib->pres.pres);

               if(plmnType->choice.pres == NOTPRSNT)
               {
                  TRACE_PRINTF("error decoding MIB, plmnType not present\n");
                  return;
               }

               switch (m_plmnType = plmnType->choice.val)
               {
                  case PLMN_TYP_GSM_MAP:
                     SibDecodePlmnIdentity(&plmnType->val.gsm_MAP.plmn_Ident);
                     break;

                  case PLMN_TYP_ANSI_41:
                     SibDecodePlmnIdentity(&plmnType->val.gsm_MAP_and_ANSI_41.plmn_Ident);
                     break;

                  default:
                     break;
               }

               // filling CsgInd 
               if((mib->v690NCExts.pres.pres) && (mib->v690NCExts.v6b0NCExts.pres.pres)
                     && (mib->v690NCExts.v6b0NCExts.v860NCExts.pres.pres)
                     && (mib->v690NCExts.v6b0NCExts.v860NCExts.mastInfBlock_v860ext.pres.pres)
                     && (mib->v690NCExts.v6b0NCExts.v860NCExts.mastInfBlock_v860ext.csg_Indtr.pres))
               {
                  m_csgInd = true;
               }

               MarkSibTypeProcessed(sibType.val);
               m_decodedMIB = true;
            }
         }
         break;

      case RruSIB_TypsysInfoBlockTyp1Enum:
         {  
            TRACE_PRINTF("%s: RRU_SIB_1\n",__FUNCTION__);

            RruSysInfoTyp1 *si = (RruSysInfoTyp1 *) s_pasn_lib25331.Decode_BCCH_BCH(octets, length, RruSIB_TypsysInfoBlockTyp1Enum, false);
            if (si)
            {
               TRACE_PRINTF("Inside ... SIB-1\n");
               m_decodedSIB1 = true;

               TRACE_PRINTF("SIB1 received, token presnt=%d\n",si->pres.pres);

               RruNAS_SysInfoGSM_MAP * lacStruct = &si->cn_CmnGSM_MAP_NAS_SysInfo;
               RruCN_DmnSysInfoList* cn_DomainSysInfoList_ = &(si->cn_DmnSysInfoList);

               if(cn_DomainSysInfoList_->noComp.pres == NOTPRSNT)
               {
                  TRACE_PRINTF("error decoding SIB1 for LAC RAC, domain sys info not present\n");
                  return;
               }

               for (U32 index = 0; index < cn_DomainSysInfoList_->noComp.val; index++)
               {
                  //make sure we get the correct RAC - for the ps Domain.            
                  if(cn_DomainSysInfoList_->member[index].cn_DmnIdent.pres != NOTPRSNT)
                  {
                     if (cn_DomainSysInfoList_->member[index].cn_DmnIdent.val == RruCN_DmnIdentps_dmnEnum)
                     {
                        m_rac = cn_DomainSysInfoList_->member[index].cn_Typ.val.gsm_MAP.val[0];
                        break;
                     }
                  }
               }

               m_lac = 0;

               AIRV_ASSERT(lacStruct->len < sizeof(lacStruct->val));

               for (U32 i = 0; i < lacStruct->len; i++)
               {
                  U8 byte = lacStruct->val[i];

                  m_lac <<= 8;
                  m_lac += byte;
               }

               TRACE_PRINTF("Decoded LAC=%u RAC=%u\n", m_lac, m_rac);

               MarkSibTypeProcessed(sibType.val);
            }
         }
         break;

      case RruSIB_TypsysInfoBlockTyp2Enum:
         {
            RruSysInfoTyp2 *si = (RruSysInfoTyp2 *) s_pasn_lib25331.Decode_BCCH_BCH(octets, length, RruSIB_TypsysInfoBlockTyp2Enum, false);
            if (si)
            {
               MarkSibTypeProcessed(sibType.val);
            }
         }
         break;

      case RruSIB_TypsysInfoBlockTyp3Enum:
         {
            TRACE_PRINTF("%s: RRU_SIB_3\n",__FUNCTION__);

            RruSysInfoTyp3 *si = (RruSysInfoTyp3 *) s_pasn_lib25331.Decode_BCCH_BCH(octets, length, RruSIB_TypsysInfoBlockTyp3Enum, false);
            if (si)
            {
               TRACE_PRINTF("Inside ... SIB-3\n");
               m_decodedSIB3 = true;

               RruCellIdent *cellId = &(si->cellIdent);
               if(cellId->pres == NOTPRSNT)
               {
                  TRACE_PRINTF("error decoding SIB3 cellId not present\n");
                  return;
               }

               U8 bitsLeft = cellId->len;
               U8* octet = (U8*) cellId->val;

               m_cellId = 0;

               while (bitsLeft > 7)
               {
                  m_cellId <<= 8;
                  m_cellId += ((*octet++) & 0xff);
                  bitsLeft -= 8;
               }

               m_cellId <<= bitsLeft;
               m_cellId += ((((*octet) >> (8 - bitsLeft))) & 0xff);

               TRACE_PRINTF("Decoded CellId=%u\n", m_cellId);

               RruSysInfoTyp3_v860ext_IEs *sib3_v860ext = &(si->v4b0NCExts.v590NCExt.v5c0NoncritExt.v670NCExt.v770NCExt.v830NCExt.v860NCExt.sysInfoTyp3_v860ext);

               if((si->v4b0NCExts.pres.pres) && (sib3_v860ext->pres.pres))
               {
                  if(sib3_v860ext->csgIdent.pres)
                  {
                     // filling CsgId
                     memcpy(&m_csgId, sib3_v860ext->csgIdent.val, sib3_v860ext->csgIdent.len);
                  }

                  // filling CsgSplitInfo
                  if(sib3_v860ext->csg_PSCSplitInfo.pres.pres)
                  {
                     if(sib3_v860ext->csg_PSCSplitInfo.starPSC.pres)
                     {
                        m_startPsc = sib3_v860ext->csg_PSCSplitInfo.starPSC.val;
                     }
                     if(sib3_v860ext->csg_PSCSplitInfo.pscRange2Ofst.pres)
                     {
                        m_pscRange = sib3_v860ext->csg_PSCSplitInfo.pscRange2Ofst.val;
                     }
                     if(sib3_v860ext->csg_PSCSplitInfo.numOfPSCs.pres)
                     {
                        m_numOfPsc = sib3_v860ext->csg_PSCSplitInfo.numOfPSCs.val;
                     }
                  }
               }

               MarkSibTypeProcessed(sibType.val);
            }
         }
         break;

      case RruSIB_TypsysInfoBlockTyp4Enum:
         {
            RruSysInfoTyp4 *si = (RruSysInfoTyp4 *) s_pasn_lib25331.Decode_BCCH_BCH(octets, length, RruSIB_TypsysInfoBlockTyp4Enum, false);
            if (si)
            {
               MarkSibTypeProcessed(sibType.val);
            }
         }
         break;

      case RruSIB_TypsysInfoBlockTyp5Enum:
         {
            /*Workaround for ccpu00146161 defect. This should be removed once BRDM solves the SIB5 Corruption issue*/
            m_decodedSIB5 = true;
            MarkSibTypeProcessed(sibType.val);
#if 0
            /* SysInfoType5bis uses the same structure as SysInfoType5 */
            RruSysInfoTyp5 *si = (RruSysInfoTyp5 *) s_pasn_lib25331.Decode_BCCH_BCH(octets, length, RruSIB_TypsysInfoBlockTyp5Enum, false);
            if (si)
            {
               TRACE_PRINTF("Inside ... SIB-5\n");
               m_decodedSIB5 = true;

               if(si->pich_PwrOfst.pres)
               {
                  memcpy(&m_pcpichTxPwr,&si->pich_PwrOfst.val,sizeof(si->pich_PwrOfst.val));
               }

               MarkSibTypeProcessed(sibType.val);
            }
#endif
         }
         break;

      case RruSIB_TypsysInfoBlockTyp6Enum:
         {
            RruSysInfoTyp6 *si = (RruSysInfoTyp6 *) s_pasn_lib25331.Decode_BCCH_BCH(octets, length, RruSIB_TypsysInfoBlockTyp6Enum, false);
            if (si)
            {
               TRACE_PRINTF("Inside ... SIB-6\n");
               m_decodedSIB6 = true;

               if(si->pich_PwrOfst.pres)
               {
                  memcpy(&m_pcpichTxPwr,&si->pich_PwrOfst.val,sizeof(si->pich_PwrOfst.val));
               }

               MarkSibTypeProcessed(sibType.val);
            }
         }
         break;

      case RruSIB_TypsysInfoBlockTyp7Enum:
         {
            RruSysInfoTyp7 *si = (RruSysInfoTyp7 *) s_pasn_lib25331.Decode_BCCH_BCH(octets, length, RruSIB_TypsysInfoBlockTyp7Enum, false);
            if (si)
            {
               MarkSibTypeProcessed(sibType.val);
            }
         }
         break;

      case RruSIB_TypsysInfoBlockTyp11Enum:
         {
            RruSysInfoTyp11 * si = (RruSysInfoTyp11 *) s_pasn_lib25331.Decode_BCCH_BCH(octets, length, RruSIB_TypsysInfoBlockTyp11Enum, false);


            BitstreamToSIB11String(octets, length);
            TRACE_PRINTF("SIB11 received = %s, token presnt=%d\n",m_sib11String.c_str(), si->pres.pres);
            m_decodedSIB11 = true;

            MarkSibTypeProcessed(sibType.val);
         }
         break;

      case RruSIB_TypsysInfoBlockTyp12Enum:
         {
            RruSysInfoTyp12 * si = (RruSysInfoTyp12 *) s_pasn_lib25331.Decode_BCCH_BCH(octets, length, RruSIB_TypsysInfoBlockTyp12Enum, false);

            TRACE_PRINTF("SIB12 received, token presnt=%d\n",si->pres.pres);
            MarkSibTypeProcessed(sibType.val);
         }
         break;

      case RruSIB_TypsysInfoBlockTyp13Enum:
         {
            RruSysInfoTyp13 * si = (RruSysInfoTyp13 *) s_pasn_lib25331.Decode_BCCH_BCH(octets, length, RruSIB_TypsysInfoBlockTyp13Enum, false);

            TRACE_PRINTF("SIB13 received, token presnt=%d\n",si->pres.pres);
            MarkSibTypeProcessed(sibType.val);
         }
         break;

      case RruSIB_TypsysInfoBlockTyp13_1Enum:
         {
            RruSysInfoTyp13_1 * si = (RruSysInfoTyp13_1 *) s_pasn_lib25331.Decode_BCCH_BCH(octets, length, RruSIB_TypsysInfoBlockTyp13_1Enum, false);

            TRACE_PRINTF("SIB13_1 received, token presnt=%d\n",si->pres.pres);
            MarkSibTypeProcessed(sibType.val);
         }
         break;

      case RruSIB_TypsysInfoBlockTyp13_2Enum:
         {
            RruSysInfoTyp13_2 * si = (RruSysInfoTyp13_2 *) s_pasn_lib25331.Decode_BCCH_BCH(octets, length, RruSIB_TypsysInfoBlockTyp13_2Enum, false);

            TRACE_PRINTF("SIB13_2 received, token presnt=%d\n",si->pres.pres);
            MarkSibTypeProcessed(sibType.val);
         }
         break;

      case RruSIB_TypsysInfoBlockTyp13_3Enum:
         {
            RruSysInfoTyp13_3 * si = (RruSysInfoTyp13_3 *) s_pasn_lib25331.Decode_BCCH_BCH(octets, length, RruSIB_TypsysInfoBlockTyp13_3Enum, false);

            TRACE_PRINTF("SIB13_3 received, token presnt=%d\n",si->pres.pres);
            MarkSibTypeProcessed(sibType.val);
         }
         break;

      case RruSIB_TypsysInfoBlockTyp13_4Enum:
         {
            RruSysInfoTyp13_4 * si = (RruSysInfoTyp13_4 *) s_pasn_lib25331.Decode_BCCH_BCH(octets, length, RruSIB_TypsysInfoBlockTyp13_4Enum, false);

            TRACE_PRINTF("SIB13_4 received, token presnt=%d\n",si->pres.pres);
            MarkSibTypeProcessed(sibType.val);
         }
         break;

      case RruSIB_TypsysInfoBlockTyp15Enum:
         {
            RruSysInfoTyp15 * si = (RruSysInfoTyp15 *) s_pasn_lib25331.Decode_BCCH_BCH(octets, length, RruSIB_TypsysInfoBlockTyp15Enum, false);

            TRACE_PRINTF("SIB15 received, token presnt=%d\n",si->pres.pres);
            if(si)
            {
               TRACE_PRINTF("Inside ... SIB-15\n");
               m_decodedSIB15 = true;

               // Filling BCCH Reference Position
               if((si->ue_posing_GPS_ReferLocat.pres.pres)
                     && (si->ue_posing_GPS_ReferLocat.elliPointAltitEllip.pres.pres))
               {
                  if(si->ue_posing_GPS_ReferLocat.elliPointAltitEllip.lati.pres)
                  {
                     m_latitude = si->ue_posing_GPS_ReferLocat.elliPointAltitEllip.lati.val;
                  }
                  if(si->ue_posing_GPS_ReferLocat.elliPointAltitEllip.latiSign.pres)
                  {
                     m_latiSign = si->ue_posing_GPS_ReferLocat.elliPointAltitEllip.latiSign.val;
                  }
                  if(si->ue_posing_GPS_ReferLocat.elliPointAltitEllip.lng.pres)
                  {
                     m_longitude = si->ue_posing_GPS_ReferLocat.elliPointAltitEllip.lng.val;
                  }
                  if(si->ue_posing_GPS_ReferLocat.elliPointAltitEllip.unceSemiMajor.pres)
                  {
                     m_unSemiMajor = si->ue_posing_GPS_ReferLocat.elliPointAltitEllip.unceSemiMajor.val;
                  }
                  if(si->ue_posing_GPS_ReferLocat.elliPointAltitEllip.unceSemiMinor.pres)
                  {
                     m_unSemiMinor = si->ue_posing_GPS_ReferLocat.elliPointAltitEllip.unceSemiMinor.val;
                  }
                  if(si->ue_posing_GPS_ReferLocat.elliPointAltitEllip.orieMajorAxis.pres)
                  {
                     m_orieMajorAxis = si->ue_posing_GPS_ReferLocat.elliPointAltitEllip.orieMajorAxis.val;
                  }
                  if(si->ue_posing_GPS_ReferLocat.elliPointAltitEllip.conf.pres)
                  {
                     m_confid = si->ue_posing_GPS_ReferLocat.elliPointAltitEllip.conf.val;
                  }
               }
            }

            MarkSibTypeProcessed(sibType.val);
         }
         break;

      case RruSIB_TypsysInfoBlockTyp15_1Enum:
         {
            RruSysInfoTyp15_1 * si = (RruSysInfoTyp15_1 *) s_pasn_lib25331.Decode_BCCH_BCH(octets, length, RruSIB_TypsysInfoBlockTyp15_1Enum, false);

            TRACE_PRINTF("SIB15_1 received, token presnt=%d\n",si->pres.pres);
            MarkSibTypeProcessed(sibType.val);
         }
         break;

      case RruSIB_TypsysInfoBlockTyp15_2Enum:
         {
            RruSysInfoTyp15_2 * si = (RruSysInfoTyp15_2 *) s_pasn_lib25331.Decode_BCCH_BCH(octets, length, RruSIB_TypsysInfoBlockTyp15_2Enum, false);

            TRACE_PRINTF("SIB15_2 received, token presnt=%d\n",si->pres.pres);
            MarkSibTypeProcessed(sibType.val);
         }
         break;

      case RruSIB_TypsysInfoBlockTyp15_3Enum:
         {
            RruSysInfoTyp15_3 * si = (RruSysInfoTyp15_3 *) s_pasn_lib25331.Decode_BCCH_BCH(octets, length, RruSIB_TypsysInfoBlockTyp15_3Enum, false);

            TRACE_PRINTF("SIB15_3 received, token presnt=%d\n",si->pres.pres);
            MarkSibTypeProcessed(sibType.val);
         }
         break;

      case RruSIB_TypsysInfoBlockTyp15_4Enum:
         {
            RruSysInfoTyp15_4 * si = (RruSysInfoTyp15_4 *) s_pasn_lib25331.Decode_BCCH_BCH(octets, length, RruSIB_TypsysInfoBlockTyp15_4Enum, false);

            TRACE_PRINTF("SIB15_4 received, token presnt=%d\n",si->pres.pres);
            MarkSibTypeProcessed(sibType.val);
         }
         break;

      case RruSIB_TypsysInfoBlockTyp15_5Enum:
         {
            RruSysInfoTyp15_5 * si = (RruSysInfoTyp15_5 *) s_pasn_lib25331.Decode_BCCH_BCH(octets, length, RruSIB_TypsysInfoBlockTyp15_5Enum, false);

            TRACE_PRINTF("SIB15_5 received, token presnt=%d\n",si->pres.pres);
            MarkSibTypeProcessed(sibType.val);
         }
         break;


      case RruSIB_TypsysInfoBlockTyp16Enum:
         {
            RruSysInfoTyp16 * si = (RruSysInfoTyp16 *) s_pasn_lib25331.Decode_BCCH_BCH(octets, length, RRU_SIB_16, false);

            TRACE_PRINTF("SIB16 received, token presnt=%d\n",si->pres.pres);
            MarkSibTypeProcessed(sibType.val);
         }
         break;

      case RruSIB_TypsysInfoBlockTyp18Enum:
         {
            RruSysInfoTyp18 * si = (RruSysInfoTyp18 *) s_pasn_lib25331.Decode_BCCH_BCH(octets, length, RruSIB_TypsysInfoBlockTyp18Enum, false);

            TRACE_PRINTF("SIB18 received, token presnt=%d\n",si->pres.pres);
            MarkSibTypeProcessed(sibType.val);
         }
         break;

      case RruSIB_TypschengBlock1Enum:
         {
            RruSysInfoTypSB1 * si = (RruSysInfoTypSB1 *) s_pasn_lib25331.Decode_BCCH_BCH(octets, length, RruSIB_TypschengBlock1Enum, false);

            TRACE_PRINTF("SIB_SB1 received, token presnt=%d\n",si->pres.pres);
            checkTransmittedSibInSB(&(si->sib_ReferList));
            MarkSibTypeProcessed(sibType.val);
            m_decodedSB1 = true;
         }
         break;

      case RruSIB_TypschengBlock2Enum:
         {
            RruSysInfoTypSB2 * si = (RruSysInfoTypSB2 *) s_pasn_lib25331.Decode_BCCH_BCH(octets, length, RruSIB_TypschengBlock2Enum, false);

            TRACE_PRINTF("SIB_SIB2 received, token presnt=%d\n",si->pres.pres);
            MarkSibTypeProcessed(sibType.val);
         }
         break;

      default:
         TRACE_PRINTF("BchBcch: Unhandled SIB-%" PRIu32 "length %d" PRIu32 " \n" , (u32)sibType.val, length);
         break;

   }
}

void Pasn_BchDecodeParams::ProcessCompleteSibList(RruComplSIB_List &sibList)
{
   TRACE_PRINTF("ProcessCompleteSibList: noComp.val=%d\n",sibList.noComp.val);

   if( sibList.noComp.pres != NOTPRSNT ) 
   {
      U16 i;
      for(i = 0; i < sibList.noComp.val; i++)
      {
         RruComplSIBshor * sib = &(sibList.member[i]);
         TRACE_PRINTF("ProcessCompleteSibList: SIBType=%ld\n",sib->sib_Typ.val);            
         ProcessSibType(sib->sib_Typ, sib->sib_Data_vari.val, (sib->sib_Data_vari.len + 7) / 8);         
      }
   }        
}

void Pasn_BchDecodeParams::SegInfo(const char *msg, RruSIB_Typ &sibType, u32 segmentIndex)
{
   TRACE_PRINTF("%s of SIB_Type %d\n", msg, (int)sibType.val);
   TRACE_PRINTF("index=%u segCount=%u segLastProcessed=%u",
         segmentIndex,
         m_sibTypeStatus[sibType.val].segCount,
         m_sibTypeStatus[sibType.val].segLastProcessed);
}

bool Pasn_BchDecodeParams::GetBit(U8 *array, U32 bit)
{
   const U32 byte = bit / 8;
   const U8 mask = 1 << (7 - (bit % 8));

   return (array[byte] & mask) != 0;
}

void Pasn_BchDecodeParams::SetBit(U8 *array, U32 bit, bool value)
{
   const U32 byte = bit / 8;
   const U8 mask = 1 << (7 - (bit % 8));

   if (value)
      array[byte] |= mask;
   else
      array[byte] &= ~mask;
}

void Pasn_BchDecodeParams::ClearBitsToOctet(U8 *array, U32 bit)
{
   while (bit % 8 != 0)
   {
      SetBit(array, bit, 0);
      bit++;
   }
}

void Pasn_BchDecodeParams::ProcessSegmentDataFixed(RruSIB_Typ sibType, RruSIB_Data_fixe &df)
{
   RruSIB_Data_vari dv;

   dv.len = df.len;
   dv.val = df.val; // Should always be 222

   ProcessSegmentDataVariable(sibType, dv);
}

void Pasn_BchDecodeParams::ProcessSegmentDataVariable(RruSIB_Typ sibType, RruSIB_Data_vari &dv)
{
   if ((sibType.val <= RruSIB_TypsysInfoBlockTyp5bisEnum) && (sibType.val >= RruSIB_TypmastInfBlockEnum))
   {
      if ((m_sibTypeStatus[sibType.val].dataBit + dv.len) / 8 < sizeof(m_sibTypeStatus[sibType.val].concatStore))
      {
         U32 t;

         TRACE_PRINTF("  Adding %" PRIu32 " bits at %" PRIu32 " bits\n",
               dv.len, m_sibTypeStatus[sibType.val].dataBit);

         for (t = 0; t < dv.len; t++)
         {
            SetBit(m_sibTypeStatus[sibType.val].concatStore,
                  m_sibTypeStatus[sibType.val].dataBit,
                  GetBit(dv.val, t));
            m_sibTypeStatus[sibType.val].dataBit++;
         }
      }
      else
      {
         TRACE_PRINTF("SIB_Type=%" PRIu32 " exceeds concatStore\n", (u32)sibType.val);
      }
   }
}

void Pasn_BchDecodeParams::ProcessFirstSegment(RruFirstSegme &fs)
{
   RruSIB_Typ sibType = fs.sib_Typ;

   if(!((sibType.val >= RruSIB_TypmastInfBlockEnum) && 
         (sibType.val <= RruSIB_TypsysInfoBlockTyp5bisEnum)))
   {
      TRACE_PRINTF("In function %s sibType.val = %d\n", __FUNCTION__, (u32)sibType.val);
      return;
   }

   if (!m_sibTypeStatus[sibType.val].decoded)
   {
      // Reset the segment buffer
      m_sibTypeStatus[sibType.val].segCount = fs.seg_Count.val;
      m_sibTypeStatus[sibType.val].segLastProcessed = 0;
      m_sibTypeStatus[sibType.val].dataBit = 0;

      SegInfo("FirstSegmentFixed", sibType, 0);

      ProcessSegmentDataFixed(sibType, fs.sib_Data_fixe);
   }
   else
   {
      // Discard data for this SIB
      m_sibTypeStatus[sibType.val].segCount = 0;
   }
}

void Pasn_BchDecodeParams::ProcessFirstSegmentShort(RruFirstSegmeShort &fss)
{
   RruSIB_Typ sibType = fss.sib_Typ;

#if 0
   AIRV_ASSERT((sibType.val >= RruSIB_TypmastInfBlockEnum) && 
         (sibType.val <= RruSIB_TypsysInfoBlockTyp5bisEnum));
#endif

   if(!((sibType.val >= RruSIB_TypmastInfBlockEnum) && 
            (sibType.val <= RruSIB_TypsysInfoBlockTyp5bisEnum)))
   {
      // Discard data for this SIB
      m_sibTypeStatus[sibType.val].segCount = 0;

      TRACE_PRINTF("In function %s sibType.val = %d\n", __FUNCTION__, (u32)sibType.val);
      return;
   }

   if (!m_sibTypeStatus[sibType.val].decoded)
   {
      // Reset the segment buffer
      m_sibTypeStatus[sibType.val].segCount = fss.seg_Count.val;
      m_sibTypeStatus[sibType.val].segLastProcessed = 0;
      m_sibTypeStatus[sibType.val].dataBit = 0;

      SegInfo("FirstSegmentShort", sibType, 0);

      ProcessSegmentDataVariable(sibType, fss.sib_Data_vari);
   }
   else
   {
      // Discard data for this SIB
      m_sibTypeStatus[sibType.val].segCount = 0;
   }
}

void Pasn_BchDecodeParams::ProcessSubsequentSegment(RruSubseSegme &ss)
{
   RruSIB_Typ sibType = ss.sib_Typ;

#if 0
   AIRV_ASSERT((sibType.val >= RruSIB_TypmastInfBlockEnum) && 
         (sibType.val <= RruSIB_TypsysInfoBlockTyp5bisEnum));
#endif

   if(!((sibType.val >= RruSIB_TypmastInfBlockEnum) && 
            (sibType.val <= RruSIB_TypsysInfoBlockTyp5bisEnum)))
   {
      // Discard data for this SIB
      m_sibTypeStatus[sibType.val].segCount = 0;

      TRACE_PRINTF("In function %s sibType.val = %d\n", __FUNCTION__, (u32)sibType.val);
      return;
   }

   if (!m_sibTypeStatus[sibType.val].decoded)
   {
      SegInfo("SubsequentSegmentFixed", sibType, (U32)ss.segmIdx.val);

      if (ss.segmIdx.val == m_sibTypeStatus[sibType.val].segLastProcessed + 1)
      {
         ProcessSegmentDataFixed(sibType, ss.sib_Data_fixe);
         m_sibTypeStatus[sibType.val].segLastProcessed++;
      }
      else
      {
         // Discard data for this SIB
         m_sibTypeStatus[sibType.val].segCount = 0;
      }
   }
}

void Pasn_BchDecodeParams::ProcessLastSegmentShort(RruLastSegmeShort &lss)
{
   RruSIB_Typ sibType = lss.sib_Typ;

#if 0
   AIRV_ASSERT((sibType.val >= RruSIB_TypmastInfBlockEnum) && 
         (sibType.val <= RruSIB_TypsysInfoBlockTyp5bisEnum));
#endif

   if(!((sibType.val >= RruSIB_TypmastInfBlockEnum) && 
            (sibType.val <= RruSIB_TypsysInfoBlockTyp5bisEnum)))
   {
      // Discard data for this SIB
      m_sibTypeStatus[sibType.val].segCount = 0;

      TRACE_PRINTF("In function %s sibType.val = %d\n", __FUNCTION__, (u32)sibType.val);
      return;
   }

   if (!m_sibTypeStatus[sibType.val].decoded)
   {
      SegInfo("LastSegmentShort", sibType, lss.segmIdx.val);

      if(lss.segmIdx.val == m_sibTypeStatus[sibType.val].segCount - 1 &&
            lss.segmIdx.val == m_sibTypeStatus[sibType.val].segLastProcessed + 1)
      {
         ProcessSegmentDataVariable(sibType, lss.sib_Data_vari);

         ClearBitsToOctet(m_sibTypeStatus[sibType.val].concatStore,
               m_sibTypeStatus[sibType.val].dataBit);

         ProcessSibType(sibType,
               m_sibTypeStatus[sibType.val].concatStore,
               (m_sibTypeStatus[sibType.val].dataBit + 7) / 8);
      }

      m_sibTypeStatus[sibType.val].segCount = 0;
   }
}

void Pasn_BchDecodeParams::ProcessLastSegment(RruLastSegme &ls)
{
   RruSIB_Typ sibType = ls.sib_Typ;

   if(!((sibType.val >= RruSIB_TypmastInfBlockEnum) && 
         (sibType.val <= RruSIB_TypsysInfoBlockTyp5bisEnum)))
   {
      TRACE_PRINTF("In function %s sibType.val = %d\n", __FUNCTION__, (u32)sibType.val);
      return;
   }

   if (!m_sibTypeStatus[sibType.val].decoded)
   {
      SegInfo("LastSegmentFixed", sibType, ls.segmIdx.val);

      if(ls.segmIdx.val == m_sibTypeStatus[sibType.val].segCount - 1 &&
            ls.segmIdx.val == m_sibTypeStatus[sibType.val].segLastProcessed + 1)
      {
         ProcessSegmentDataFixed(sibType, ls.sib_Data_fixe);

         ClearBitsToOctet(m_sibTypeStatus[sibType.val].concatStore,
               m_sibTypeStatus[sibType.val].dataBit);

         ProcessSibType(sibType,
               m_sibTypeStatus[sibType.val].concatStore,
               (m_sibTypeStatus[sibType.val].dataBit + 7) / 8);
      }

      m_sibTypeStatus[sibType.val].segCount = 0;
   }
}


void Pasn_BchDecodeParams::ProcessSibData(const U8 *data)
{
   CPASN_LIB25331 pasn_lib25331;

   RruSysInfo_BCH * pBchBcch = 
      (RruSysInfo_BCH*) pasn_lib25331.Decode_BCCH_BCH((const U8 *) data, 31, 
            RRU_MSG_DLBCCHBCH, true);
   if (pBchBcch == NULL)
      return;

   TRACE_PRINTF("%s: pBchBcch->payl.choice.val=%d\n",
         __FUNCTION__,pBchBcch->payl.choice.val);

   switch(pBchBcch->payl.choice.val)
   {
      case SYSINFO_BCHPAYL_NOSEGME:
         /* Do nothing */
         break;

      case SYSINFO_BCHPAYL_FIRSSEGME:
         TRACE_PRINTF("%s: SYSINFO_BCHPAYL_FIRSSEGME\n",__FUNCTION__);            
         ProcessFirstSegment(pBchBcch->payl.val.firsSegme);
         break;

      case SYSINFO_BCHPAYL_SUBSSEGME:
         TRACE_PRINTF("%s: SYSINFO_BCHPAYL_SUBSSEGME\n",__FUNCTION__);
         ProcessSubsequentSegment(pBchBcch->payl.val.subsSegme);
         break;

      case SYSINFO_BCHPAYL_LASTSEGMESHORT:
         TRACE_PRINTF("%s: SYSINFO_BCHPAYL_LASTSEGMESHORT\n",__FUNCTION__);
         ProcessLastSegmentShort(pBchBcch->payl.val.lastSegmeShort);
         break;

      case SYSINFO_BCHPAYL_LASTANDFIRST:
         TRACE_PRINTF("%s: SYSINFO_BCHPAYL_LASTANDFIRST\n",__FUNCTION__);
         ProcessLastSegmentShort(pBchBcch->payl.val.lastAndFirst.lastSegmeShort);
         ProcessFirstSegmentShort(pBchBcch->payl.val.lastAndFirst.firsSegme);
         break;

      case SYSINFO_BCHPAYL_LASTSEGME:
         TRACE_PRINTF("%s: SYSINFO_BCHPAYL_LASTSEGME\n",__FUNCTION__);
         ProcessLastSegment(pBchBcch->payl.val.lastSegme);
         break;

      case SYSINFO_BCHPAYL_LASTANDCOMPL:
         TRACE_PRINTF("%s: SYSINFO_BCHPAYL_LASTSEGME\n",__FUNCTION__);
         ProcessLastSegmentShort(pBchBcch->payl.val.lastAndCompl.lastSegmeShort);
         ProcessCompleteSibList(pBchBcch->payl.val.lastAndCompl.compSIB_List);
         break;

      case SYSINFO_BCHPAYL_LASTANDCOMPLANDFIRST:
         TRACE_PRINTF("%s: SYSINFO_BCHPAYL_LASTANDCOMPLANDFIRST\n",__FUNCTION__);
         ProcessLastSegmentShort(pBchBcch->payl.val.lastAndComplAndFirst.lastSegmeShort);
         ProcessCompleteSibList(pBchBcch->payl.val.lastAndComplAndFirst.compSIB_List);
         ProcessFirstSegmentShort(pBchBcch->payl.val.lastAndComplAndFirst.firsSegme);
         break;

      case SYSINFO_BCHPAYL_COMPSIB_LIST:
         TRACE_PRINTF("%s: SYSINFO_BCHPAYL_COMPSIB_LIST\n",__FUNCTION__);
         ProcessCompleteSibList(pBchBcch->payl.val.compSIB_List);
         break;

      case SYSINFO_BCHPAYL_COMPANDFIRST:
         TRACE_PRINTF("%s: SYSINFO_BCHPAYL_COMPANDFIRST\n",__FUNCTION__);
         ProcessCompleteSibList(pBchBcch->payl.val.compAndFirst.compSIB_List);
         ProcessFirstSegmentShort(pBchBcch->payl.val.compAndFirst.firsSegme);
         break;

      case SYSINFO_BCHPAYL_COMPSIB:
         {
            TRACE_PRINTF("%s: SYSINFO_BCHPAYL_COMPSIB\n",__FUNCTION__);
            RruComplSIB *sib = &pBchBcch->payl.val.compSIB;

            ProcessSibType(sib->sib_Typ, sib->sib_Data_fixe.val, (sib->sib_Data_fixe.len + 7) / 8);
            break;
         }

      default:
         TRACE_PRINTF("BchBcch sfn_Prime=%" PRIu32 " payload.choice=%u\n", (u32)pBchBcch->sfn_Prime.val, pBchBcch->payl.choice.val);
         break;
   }

}

void Pasn_BchDecodeParams::checkTransmittedSibInSB(RruSIB_ReferList *sib_ReferList)
{
   U32 numSibs = 0;

   if(sib_ReferList->noComp.pres)
   {
      numSibs = sib_ReferList->noComp.val; 
      TRACE_PRINTF("numSibs in sib_ReferList = %d\n", sib_ReferList->noComp.val); 
   }

   for(U32 i = 0; i < numSibs; i++)
   {
      if(sib_ReferList->member[i].pres.pres)
      {
         if(sib_ReferList->member[i].sib_Typ.choice.pres)
         {
            markSIBAsBeingTransmitted(sib_ReferList->member[i].sib_Typ.choice.val + 1);
         }
      }
   }
}

void Pasn_BchDecodeParams::markSIBAsBeingTransmitted(U32 sibType)
{
   switch(sibType)
   {
      case RruSIB_TypsysInfoBlockTyp1Enum:
         {
            m_transmitSIB1 = true;
            TRACE_PRINTF("SIB-1 being transmitted\n");
         }
         break;

      case RruSIB_TypsysInfoBlockTyp3Enum:
         {
            m_transmitSIB3 = true;
            TRACE_PRINTF("SIB-3 being transmitted\n");
         }
         break;

      case RruSIB_TypsysInfoBlockTyp5Enum:
         {
            m_transmitSIB5 = true;
            TRACE_PRINTF("SIB-5 being transmitted\n");
         }
         break;

      case RruSIB_TypsysInfoBlockTyp6Enum:
         {
            m_transmitSIB6 = true;
            TRACE_PRINTF("SIB-6 being transmitted\n");
         }
         break;

      case RruSIB_TypsysInfoBlockTyp15Enum:
         {
            m_transmitSIB15 = true;
            TRACE_PRINTF("SIB-15 being transmitted\n");
         }
         break;

      case RruSIB_TypschengBlock1Enum:
         {
            m_transmitSB1 = true;
            TRACE_PRINTF("SB-1 being transmitted\n");
         }
         break;

      default:
         {
            break;
         }
   }
}

void Pasn_BchDecodeParams::checkTransmittedSibInMib(RruSIBSb_ReferList *sibSb_ReferList)
{
   U32 numSibs = 0;

   if(sibSb_ReferList->noComp.pres)
   {
      numSibs = sibSb_ReferList->noComp.val; 
      TRACE_PRINTF("numSibs in sibSb_RegerList = %d\n", sibSb_ReferList->noComp.val); 
   }

   for(U32 i = 0; i < numSibs; i++)
   {
      if(sibSb_ReferList->member[i].pres.pres)
      {
         TRACE_PRINTF("choice val = %d\n", 
               sibSb_ReferList->member[i].sibSb_Typ.choice.val); 

         if(sibSb_ReferList->member[i].sibSb_Typ.choice.pres)
         {
            /* Work around for the moment as we have a mismatch in enum values for SB1 with
               what is reported and what we have as enum */
            if(sibSb_ReferList->member[i].sibSb_Typ.choice.val == 21)
            {
               markSIBAsBeingTransmitted(RruSIB_TypschengBlock1Enum);
            }
            else
            {
               markSIBAsBeingTransmitted(sibSb_ReferList->member[i].sibSb_Typ.choice.val + 1);
            }
         }
      }
   }
}

#if 0           //For now we don't required check for extension SIB's
void Pasn_BchDecodeParams::checkTransmittedExtSibInMib(RruMasteInfBlock *mib,
      ExtnSibs extnSIBType)
{

   if(mib->v690NCExts.pres.pres != NOTPRSNT)
   {
      if(mib->v690NCExts.v6b0NCExts.pres.pres != NOTPRSNT)
      {
         if(mib->v690NCExts.v6b0NCExts.mastInfBlock_v6b0ext.pres.pres != NOTPRSNT)
         {
            RruExtSIBTypInfoSchedngInfo_List *sibList = 
               &(mib->v690NCExts.v6b0NCExts.mastInfBlock_v6b0ext.extSIBTypInfoSchedngInfo_List);
            U32 noComp = sibList->noComp;

            for(U32 i = 0; i < noComp; i++)
            {
               if(sibList->member[i].pres.pres != NOTPRSNT)
               {


                  if(sibList->member[i].extSIB_Typ.choice.val == extnSIBType)
                  {
                     return true;
                  }
               }
            }
         }     
      }
   }
}
#endif

bool Pasn_BchDecodeParams::HaveWeGotEverythingFromBch()
{
   // TODO: This function can be written in a much more compressed
   // form. Look into it
   if(m_decodedMIB == false)
   {
      return false;
   }
   if(m_transmitSB1 && !m_decodedSB1)
   {
      return false;
   }
   if(m_transmitSIB1 && !m_decodedSIB1)
   {
      return false;
   }
   if(m_transmitSIB3 && !m_decodedSIB3)
   {
      return false;
   }
   if(m_transmitSIB5 && !m_decodedSIB5)
   {
      return false;
   }
   if(m_transmitSIB6 && !m_decodedSIB6)
   {
      return false;
   }
   if(m_transmitSIB15 && !m_decodedSIB15)
   {
      return false;
   }

   return true;
}


// END OF FILE

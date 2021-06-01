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
               

     File:     wr_umm_mobparms.c

     Sid:      fc_umm_mobparms.c@@/main/TeNB_Main_BR/4 - Mon Aug 11 16:52:53 2014

     Prg:      Sriky 

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_MODULE_ID=32;
static int RLOG_FILE_ID=122;

#define WR_SID_RESERVED_ZERO 0
#define WR_NID_RESERVED_ZERO 0
#define WR_NID_RESERVED_MAX  65535

#define CDMA2000_PDU_TYPE_MOB_PARAMS 0

#include "wr.h"            /* Common headers and Data Structures */
#include "wr_umm.h"        /* UMM Specific files */
#include "wr_umm_mobparms.h"        /* UMM Specific files */
#include <math.h>

/** @brief This function validates mobility params received from OAM.
 *
 * @details
 *     Function: wrEmmValidMobParams
 *
 *         Processing steps:
 *         - validate the mobility params 
 *
 *   @param[in]  mobParams: mobility parameters.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
/** @brief This function used to fill the inclusion field into Octet Buffer.
 *
 * @details
 *     Function: wrEncCdma2000FldIncl
 *
 *         Processing steps:
 *         - fill the inclusion field into Octet Buffer.
 *
 *   @param[in]  cdma2000Fld:  pointer to CDMA2K field structure.
 *   @param[out]  encCdma2000Pdu: pointer to CDMA2K encoder buffer PDU.
 *   @param[in,out]  octetPos: pointer to octet position for encoder buffer.
 *   @param[in,out]  bitPos: pointer to bit position for encoder buffer.
 *   @param[in,out]  noOfBitsEnc: pointer to num of bit position.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrEncCdma2000FldIncl
(
WrCdma2000Field              *cdma2000Fld,
U8                           *encCdma2000Pdu,
U16                          *octetPos,
U8                           *bitPos,
U8                           *noOfBitsEnc
)
{
   /* Fill the inclusion field */
   encCdma2000Pdu[*octetPos] |= ((cdma2000Fld->fldIncl) << (*bitPos-1));

   /* Inclusion bit is always 1 bit, so increase noOfBitsEnc by 1 */
   (*noOfBitsEnc)++;

   /* If number of bits encoded is multiple of 8, increase octet position.
      Move the bitPos to MSB i.e 8 */
   if ((*noOfBitsEnc)%8 == 0)
   {
      (*octetPos)++;
      *bitPos = 8;
   }
   else
   {
      (*bitPos)--;
   }

   RETVALUE(ROK);
}


/** @brief This function enocode the mobility params structure.
 *
 * @details
 *     Function: wrEncCdma2000FldValue
 *
 *         Processing steps:
 *         - enocode the mobility params structure 
 *
 *   @param[in]  cdma2000Fld:  pointer to CDMA2K field structure.
 *   @param[out]  encCdma2000Pdu: pointer to CDMA2K encoder buffer PDU.
 *   @param[in,out]  octetPos: pointer to octet position for encoder buffer.
 *   @param[in,out]  bitPos: pointer to bit position for encoder buffer.
 *   @param[in,out]  noOfBitsEnc: pointer to num of bit position.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrEncCdma2000FldValue
(
WrCdma2000Field              *cdma2000Fld,
U8                           *encCdma2000Pdu,
U16                          *octetPos,
U8                           *bitPos,
U8                           *noOfBitsEnc
)
{
   U8                        noOfBitsRemainCurOct, noOfBitsFilledCurOct;
   U32                       bitMsk;
   U8                        bitMskLen;
   U8                        datLen;
   

   noOfBitsRemainCurOct = *bitPos;
   noOfBitsFilledCurOct = 8 - (*bitPos);

   if (cdma2000Fld->fldSize <= noOfBitsRemainCurOct)
   {
      bitMsk = 0x01;
      bitMskLen = cdma2000Fld->fldSize;
      while (--bitMskLen)
      {
         bitMsk |= (0x01 << bitMskLen);
      }
      encCdma2000Pdu[*octetPos] |= 
        ((cdma2000Fld->fldData & bitMsk) << ((*bitPos) - cdma2000Fld->fldSize));

      (*noOfBitsEnc)+=cdma2000Fld->fldSize;
      if ((*noOfBitsEnc)%8 == 0)
      {
         (*octetPos)++;
         *bitPos = 8;
      }
      else
      {
   /*      (*bitPos)--; */
           (*bitPos) -= cdma2000Fld->fldSize;
      }
   }
   else
   {
      U8   bitsEncoded = 0;
      datLen=cdma2000Fld->fldSize;
      while(datLen)
      {
         noOfBitsRemainCurOct = *bitPos;
         bitMsk = 0x80000000;
         bitMskLen = noOfBitsRemainCurOct;
         while (--bitMskLen)
         {
               bitMsk |= (0x80000000 >> bitMskLen);
          }

/*         encCdma2000Pdu[*octetPos] |= 
            ((cdma2000Fld->fldData) << (32-datLen)) & bitMsk;  */
 
         U32 word = 
            ((cdma2000Fld->fldData) << (32-datLen)) & bitMsk;
         U8 byte = (U8) (word >> 24);
         byte = byte >> (8 - noOfBitsRemainCurOct);
         encCdma2000Pdu[*octetPos] |= byte;

         if (*bitPos == 8)
         {
            if(datLen >= 8)
            {
               bitsEncoded = 8;
            }
            else
            {
               bitsEncoded = datLen;
            }
         }
         else
         {
            bitsEncoded = *bitPos;
         }

         (*noOfBitsEnc) += bitsEncoded;

/*         (*noOfBitsEnc)+= noOfBitsRemainCurOct; */

         if ((*noOfBitsEnc)%8 == 0)
         {
            (*octetPos)++;
            *bitPos = 8;
         }
         else
         {
/*             (*bitPos)--; */
            (*bitPos) -= bitsEncoded;
         }
         datLen = datLen - bitsEncoded;

/*
         datLen = datLen - noOfBitsRemainCurOct;
         if(datLen >= 8)
         {
             noOfBitsRemainCurOct = 8;
         }
         else
         {
             noOfBitsRemainCurOct = datLen;
         }
*/
      }  
   }

   RETVALUE (ROK);
}

/** @brief This function encode mobility params strutures for few parameters.
 *
 * @details
 *     Function: wrEncIntCdma2000FldValue
 *
 *         Processing steps:
 *         - encode mobility params strutures 
 *
 *   @param[in]  data:  Data to encode.
 *   @param[in]  len:  length of data.
 *   @param[out]  encCdma2000Pdu: pointer to CDMA2K encoder buffer PDU.
 *   @param[in,out]  octetPos: pointer to octet position for encoder buffer.
 *   @param[in,out]  bitPos: pointer to bit position for encoder buffer.
 *   @param[in,out]  noOfBitsEnc: pointer to num of bit position.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrEncIntCdma2000FldValue
(
 U32                         data,
 U8                          len,
 U8                          *encCdma2000Pdu,
 U16                         *octetPos,
 U8                          *bitPos,
 U8                          *noOfBitsEnc
)
{
   U8                         noOfBitsRemainCurOct, noOfBitsFilledCurOct;
   U32                        bitMsk;
   U8                         bitMskLen;
   U8                         datLen;
   

   noOfBitsRemainCurOct = *bitPos;
   noOfBitsFilledCurOct = 8 - (*bitPos);

   if (len <= noOfBitsRemainCurOct)
   {
      bitMsk = 0x01;
      bitMskLen = len;
      while (--bitMskLen)
      {
         bitMsk |= (0x01 << bitMskLen);
      }
      encCdma2000Pdu[*octetPos] |= 
        ((data & bitMsk) << ((*bitPos) - len));

      (*noOfBitsEnc)+=len;
      if ((*noOfBitsEnc)%8 == 0)
      {
         (*octetPos)++;
         *bitPos = 8;
      }
      else
      {
/*          (*bitPos)--; */
          (*bitPos) -= len;
      }
   }
   else
   {
      U8    bitsEncoded = 0;
      datLen=len;
      while(datLen)
      {
         noOfBitsRemainCurOct = *bitPos;
         bitMsk = 0x80000000;
         bitMskLen = noOfBitsRemainCurOct;
         while (--bitMskLen)
         {
            bitMsk |= (0x80000000 >> bitMskLen);
         }

/*         encCdma2000Pdu[*octetPos] |= 
            ((cdma2000Fld->fldData) << (32-datLen)) & bitMsk;  */

         U32 word = 
            (data << (32-datLen)) & bitMsk;
         U8 byte = (U8) (word >> 24);
         byte = byte >> (8 - noOfBitsRemainCurOct);
         encCdma2000Pdu[*octetPos] |= byte;

         if (*bitPos == 8)
         {
            if(datLen >= 8)
            {
               bitsEncoded = 8;
         }
         else
         {
               bitsEncoded = datLen;
         }
         }
         else
         {
            bitsEncoded = *bitPos;
         }

         (*noOfBitsEnc) += bitsEncoded;

/*         (*noOfBitsEnc)+= noOfBitsRemainCurOct; */

         if ((*noOfBitsEnc)%8 == 0)
         {
            (*octetPos)++;
            *bitPos = 8;
         }
         else
         {
/*             (*bitPos)--; */
            (*bitPos) -= bitsEncoded;
         }
         datLen = datLen - bitsEncoded;
      }  
   }

   RETVALUE (ROK);
}

/** @brief This function encode the mobility parameter into octet sting.
 *
 * @details
 *     Function: wrEncCdma2000MobParams 
 *
 *         Processing steps:
 *         - Encode the mobility params and return buffer 
 *
 *   @param[in,out]  cellCb: pointer to cell Cb.
 *   @param[in]      cellMobParams: pointer to cell mobility params.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrEncCdma2000MobParams
(
 WrUmmCellCb                   *cellCb,
 WrMobParams                   *mobParams
)
{
   U8                         *buf;
   U16                        octetPos = 0;
   U8                         bitPos = 0;
   U8                         noOfBitsEnc = 0;
   U16                        cnt;

   /* Allocate Buf of Max length for Mobility params */
   WR_ALLOC(&(buf), MAX_CDMA2OOO_PARAMS_LEN);
   if (buf == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Fill the record type - 0x00. Length = 8 bits */
   buf[octetPos++] = 0x00;
   /* Considering bit positions from 1 to 8. 1-LSB, 8-MSB */
   bitPos = 8;
   noOfBitsEnc = 8;

   for (cnt=0; cnt < CDMA2000_MOB_PARAMS_MAX_FIELDS; cnt++)
   {
      /* Encode inclusion field */
      wrEncCdma2000FldIncl(&(mobParams->fields[cnt]), buf,
            &octetPos, &bitPos, &noOfBitsEnc);

      if((cnt == 8) && (mobParams->fields[cnt].fldIncl == TRUE))
      {
         if(mobParams->pzHystEnabled.pres == TRUE)
         {
            /* Fill the inclusion field */
            buf[octetPos] |= ((mobParams->pzHystEnabled.val) << (bitPos-1));

            /* Inclusion bit is always 1 bit, so increase noOfBitsEnc by 1 */
            noOfBitsEnc++;

            /* If number of bits encoded is multiple of 8, increase octet 
             * position. Move the bitPos to MSB i.e 8 */
            if ((noOfBitsEnc%8) == 0)
            {
               octetPos++;
               bitPos = 8;
            }
            else
            {
               bitPos--;
            }
         }

         if(mobParams->pzHystInfoIncl.pres == TRUE)
         {
            /* Fill the inclusion field */
            buf[octetPos] |= ((mobParams->pzHystInfoIncl.val) << (bitPos-1));

            /* Inclusion bit is always 1 bit, so increase noOfBitsEnc by 1 */
            noOfBitsEnc++;

            /* If number of bits encoded is multiple of 8, increase octet 
             * position. Move the bitPos to MSB i.e 8 */
            if ((noOfBitsEnc%8) == 0)
            {
               octetPos++;
               bitPos = 8;
            }
            else
            {
               bitPos--;
            }
         }
         if(mobParams->pzHystListLen.pres == TRUE)
         {
            /* Fill the inclusion field */
            buf[octetPos] |= ((mobParams->pzHystListLen.val) << (bitPos-1));

            /* Inclusion bit is always 1 bit, so increase noOfBitsEnc by 1 */
            noOfBitsEnc++;

            /* If number of bits encoded is multiple of 8, increase octet 
             * position. Move the bitPos to MSB i.e 8 */
            if ((noOfBitsEnc%8) == 0)
            {
               octetPos++;
               bitPos = 8;
            }
            else
            {
               bitPos--;
            }
         }

         if(mobParams->pzHystActTimer.pres == TRUE)
         {
            wrEncIntCdma2000FldValue(mobParams->pzHystActTimer.val, 8, buf,
                  &octetPos, &bitPos, &noOfBitsEnc);
         }

         if(mobParams->pzHystTimerMul.pres == TRUE)
         {
            wrEncIntCdma2000FldValue(mobParams->pzHystTimerMul.val, 3, buf,
                  &octetPos, &bitPos, &noOfBitsEnc);
         }

         if(mobParams->pzHystTimerExp.pres == TRUE)
         {
            wrEncIntCdma2000FldValue(mobParams->pzHystTimerExp.val, 5, buf,
                  &octetPos, &bitPos, &noOfBitsEnc);
         }

         continue;
      }
      if((cnt == 50) && (mobParams->fields[cnt].fldIncl == TRUE))
      {
         wrEncIntCdma2000FldValue(mobParams->fpcFchInitSetptRC3, 8, buf,
               &octetPos, &bitPos, &noOfBitsEnc);
         wrEncIntCdma2000FldValue(mobParams->fpcFchInitSetptRC4, 8, buf,
               &octetPos, &bitPos, &noOfBitsEnc);
         wrEncIntCdma2000FldValue(mobParams->fpcFchInitSetptRC5, 8, buf,
               &octetPos, &bitPos, &noOfBitsEnc);
         wrEncIntCdma2000FldValue(mobParams->fpcFchInitSetptRC11, 8, buf,
               &octetPos, &bitPos, &noOfBitsEnc);
         wrEncIntCdma2000FldValue(mobParams->fpcFchInitSetptRC12, 8, buf,
               &octetPos, &bitPos, &noOfBitsEnc);
         continue;
      }

      if (cnt != 50)
      {
         if(mobParams->fields[cnt].fldIncl == TRUE)
         {
            wrEncCdma2000FldValue(&(mobParams->fields[cnt]),buf,
                  &octetPos, &bitPos, &noOfBitsEnc);
         }
      }
   }

   cellCb->mobParam1xOct.pres = TRUE; 
   cellCb->mobParam1xOct.len = octetPos; 
   cellCb->mobParam1xOct.val = buf;
   RETVALUE (ROK);
}

/** @brief This function used for encode the struture into CDMA format.
 *
 * @details
 *     Function: wrUmmEncCdmaPdu    
 *
 *         Processing steps:
 *         - encode the structure into CDMA format. 
 *
 *   @param[in,out]  cellCb: pointer to cellCb.
 *   @param[in]  mobParams: pointer to mobility params.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrUmmEncCdmaPdu
(
WrUmmCellCb                  *cellCb,
LwrCdma2k1xMobParams         *mobParams
)
{
   WrMobParams                cellMobParams;           

   TRC2(wrUmmEncCdmaPdu)

   cmMemset((U8*)&cellMobParams, 0, sizeof(WrMobParams));
   wrUmmFillMobParamsIntoCellCb(cellCb, &cellMobParams,  mobParams);
 
   if(ROK != wrEncCdma2000MobParams(cellCb, &cellMobParams))
   {
      RLOG0(L_ERROR, " wrEncCdmaPdu:wrEncCdma2000MobParams Failed. ");
      RETVALUE(RFAILED);
   }
    
   RETVALUE (ROK);
}

/** @brief This function used to conver LWR structures into WR structure format.
 *
 * @details
 *     Function: wrUmmFillMobParamsIntoCellCb
 *
 *         Processing steps:
 *         - convert LWR structures into WR structure format for mobility params 
 *
 *   @param[in]  cellCb: pointer to CellCb.
 *   @param[out] cellMobParams: cell mobility parameters.
 *   @param[in]  mobParams: mobility parameters.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrUmmFillMobParamsIntoCellCb
(
 WrUmmCellCb                 *cellCb,
 WrMobParams                 *cellMobParams,
 LwrCdma2k1xMobParams        *mobParams
)
{
   WrCdma2000Field            *pField;

   TRC2(wrUmmFillMobParamsIntoCellCb)

   pField = cellMobParams->fields;
   
   if(mobParams->sidIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 15, mobParams->sid)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;
  
   if(mobParams->nidIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 16, mobParams->nid)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->multSidsIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 1, mobParams->multSids)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->multNidsIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 1, mobParams->multNids)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->regZoneIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 12, mobParams->regZone)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->totalZonesIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 3, mobParams->totalZones)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->zoneTimerIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 3, mobParams->zoneTimer)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->pktZoneIdIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 8, mobParams->pktZoneId)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->pzIdHysParamIncl == TRUE)
   {
      if(mobParams->pktZoneId != 0)
      {
         cellMobParams->pzHystEnabled.pres = TRUE; 
         cellMobParams->pzHystEnabled.val = mobParams->pzHystEnabled;
      }
      else
      {
         cellMobParams->pzHystEnabled.pres = FALSE; 
      }

      if((cellMobParams->pzHystEnabled.pres == TRUE) && 
                 (cellMobParams->pzHystEnabled.val != 0))
      {
         cellMobParams->pzHystInfoIncl.pres = TRUE; 
         cellMobParams->pzHystInfoIncl.val = mobParams->pzHystInfoIncl; 
      }
      else
      {
         cellMobParams->pzHystInfoIncl.pres = FALSE; 
      }
      if((cellMobParams->pzHystInfoIncl.pres == TRUE) && 
            (cellMobParams->pzHystInfoIncl.val != 0))
      {
         cellMobParams->pzHystListLen.pres = TRUE; 
         cellMobParams->pzHystListLen.val = mobParams->pzHystListLen; 
      }
      else
      {
         cellMobParams->pzHystListLen.pres = FALSE; 
      }

      if((cellMobParams->pzHystInfoIncl.pres == TRUE) && 
                    (cellMobParams->pzHystInfoIncl.val != 0))
      {
         cellMobParams->pzHystActTimer.pres = TRUE; 
         cellMobParams->pzHystActTimer.val = mobParams->pzHystActTimer; 
      }
      else
      {
         cellMobParams->pzHystActTimer.pres = FALSE; 
      }

      if((cellMobParams->pzHystInfoIncl.pres == TRUE) && 
                    (cellMobParams->pzHystInfoIncl.val != 0))
      {
         cellMobParams->pzHystTimerMul.pres = TRUE; 
         cellMobParams->pzHystTimerMul.val = mobParams->pzHystTimerMul; 
      }
      else
      {
         cellMobParams->pzHystTimerMul.pres = FALSE; 
      }

      if((cellMobParams->pzHystInfoIncl.pres == TRUE) && 
                 (cellMobParams->pzHystInfoIncl.val != 0))
      {
         cellMobParams->pzHystTimerExp.pres = TRUE; 
         cellMobParams->pzHystTimerExp.val = mobParams->pzHystTimerExp; 
      }
      else
      {
         cellMobParams->pzHystTimerExp.pres = FALSE; 
      }
      pField->fldIncl = TRUE;
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->pRevIncl == TRUE)
   {
/* Sunita */
      WR_UMM_INCLUDE_FIELD(pField, 8, mobParams->pRev)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->minPRevIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 8, mobParams->minPRev)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->negSlotCycIndxSupIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 1, mobParams->negSlotCycIndxSup)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->encModeIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 2, mobParams->encMode)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->encSupIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 1, mobParams->encSup)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->sigEncSupIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 8, mobParams->sigEncSup)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->msgIntSupIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 1, mobParams->msgIntSup)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->sigIntSupINCLIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 1, mobParams->sigIntSupINCL)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->sigIntSupIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 8, mobParams->sigIntSup)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->authIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 2, mobParams->auth)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->maxNumAltoSo == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 3, mobParams->maxNumAltoSo)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->useSyncId == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 1, mobParams->useSyncId)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->msInitPosLocSupIndIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 1, mobParams->msInitPosLocSupInd)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->mobQosIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 1, mobParams->mobQos)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->bandClsInfoReqIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 1, mobParams->bandClsInfoReq)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->bandClsIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 5, mobParams->bandCls)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->bypassRegIndIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 5, mobParams->bypassRegInd)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->altBandClsIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 5, mobParams->altBandCls)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->maxAddServInstIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 3, mobParams->maxAddServInst)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->homeRegIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 1, mobParams->homeReg)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->forSidRegIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 1, mobParams->forSidReg)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->forNidRegIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 1, mobParams->forNidReg)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->powerUpRegIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 1, mobParams->powerUpReg)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->powerDownRegIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 1, mobParams->powerDownReg)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->parmRegIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 1, mobParams->parmReg)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->regPrdIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 7, mobParams->regPrd)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->regDistIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 11, mobParams->regDist)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->prefMsIdTypeIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 2, mobParams->prefMsIdType)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->extPrefMsIdTypeIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 2, mobParams->extPrefMsIdType)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->meIdReqdIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 1, mobParams->meIdReqd)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->mccIncl == TRUE)
   {
       U16 idx, div, rem;
       U16 mcc = mobParams->mcc, enc_mcc = 0;
       for(idx = 0; idx < 3; idx++)
       {
          div = ((rem = mcc % 10) == 0)? 0xA:rem;
          enc_mcc += div * pow(10, idx);
          mcc = mcc / 10;
       }
       enc_mcc -= 111;
       mobParams->mcc = enc_mcc;
      WR_UMM_INCLUDE_FIELD(pField, 10, mobParams->mcc)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->imsi1112Incl == TRUE)
   {
       U16 idx, div, rem;
       U8 imsi1112 = mobParams->imsi1112, enc_imsi1112 = 0;
       for(idx = 0; idx < 2; idx++)
       {
          div = ((rem = imsi1112 % 10) == 0)? 0xA:rem;
          enc_imsi1112 += div * pow(10, idx);
          imsi1112 = imsi1112 / 10;
       }
      enc_imsi1112 -= 11;
      mobParams->imsi1112 = enc_imsi1112;
      WR_UMM_INCLUDE_FIELD(pField, 7, mobParams->imsi1112)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->imsiTSupIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 1, mobParams->imsiTSup)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->recntMsgIndIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 1, mobParams->recntMsgInd)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->rerModeSupIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 1, mobParams->rerModeSup)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->tzkModeSupIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 1, mobParams->tzkModeSup)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->tzkIdIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 1, mobParams->tzkId)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->pilRepIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 1, mobParams->pilRep)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->sdbSupIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 1, mobParams->sdbSup)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->autoFcsoAllIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 1, mobParams->autoFcsoAll)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->sdbInRcnmIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 1, mobParams->sdbInRcnm)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->fpcFchIncl == TRUE)
   {
      pField->fldIncl = TRUE;
      cellMobParams->fpcFchInitSetptRC3 = mobParams->fpcFchInitSetptRC3;
      cellMobParams->fpcFchInitSetptRC4 = mobParams->fpcFchInitSetptRC4;
      cellMobParams->fpcFchInitSetptRC5 = mobParams->fpcFchInitSetptRC5;
      cellMobParams->fpcFchInitSetptRC11 = mobParams->fpcFchInitSetptRC11;
      cellMobParams->fpcFchInitSetptRC12 = mobParams->fpcFchInitSetptRC12;
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->tAddIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 6, mobParams->tAdd)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->pilotIncIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 4, mobParams->PilotInc)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->randIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 32, mobParams->rand)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->lpSecIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 8, mobParams->lpSec)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->ltmOffIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 6, mobParams->ltmOff)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->dayltIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 1, mobParams->daylt)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->gcsnaL2ActTimerIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 8, mobParams->gcsnaL2ActTimer)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

   if(mobParams->gcsnaSeqCtxtTimerIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 8, mobParams->gcsnaSeqCtxtTimer)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;

/* 
   if(mobParams->regPrdIncl == TRUE)
   {
      WR_UMM_INCLUDE_FIELD(pField, 8, mobParams->regPrd)
   }
   else
   {
      pField->fldIncl = FALSE;
   }
   pField++;
*/

   RETVALUE(ROK);
}
/********************************************************************30**

           End of file:     fc_umm_mobparms.c@@/main/TeNB_Main_BR/4 - Mon Aug 11 16:52:53 2014

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
/main/1        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/

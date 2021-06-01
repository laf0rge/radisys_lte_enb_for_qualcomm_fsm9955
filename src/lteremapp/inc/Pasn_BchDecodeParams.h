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

     Desc:     This header file for Pasn_BchDecodeParams.cpp

     File:     Pasn_BchDecodeParams.h

     Sid:      Pasn_BchDecodeParams.h@@/main/tenb_main_ccb/1 - Thu Jan 21 17:51:18 2016

     Prg:      Rohit Kumar


**************************************************************************************************/

#ifndef __Pasn_BchDecodeParams_h__
#define __Pasn_BchDecodeParams_h__

///////////////////////////////////////////////////////////////////////////////
// System Includes
///////////////////////////////////////////////////////////////////////////////
#include <sstream>

///////////////////////////////////////////////////////////////////////////////
// Local Includes
///////////////////////////////////////////////////////////////////////////////
#include "envdep.h"
#include "gen.h"
#include "ssi.h"
#include "rru_asn.h"
#include "rru.h"
#include "cm_umts.h"
#include "rr_env.h"
#include "gen.x"
#include "ssi.x"
#include "cm_tkns.x"
#include "cm_mblk.x"
#include "rru_asn.x"
#include "rru.x"

namespace threeway
{

///////////////////////////////////////////////////////////////////////////////
// Classes
///////////////////////////////////////////////////////////////////////////////

   class Pasn_BchDecodeParams
   {
      public:
         // Count of consecutive SIB decode failures due to CRC that cause the cell
         //  to be abandoned.  This is arbitarity selected.
         static const U32 MAX_SIB_CONSECUTIVE_CRC_FAILS = 10; // 20ms per SIB so = 200ms

         // Count of SibDecodeInds after which decoding is abandonded.
         //  25.331 section 10.3.8.16 shows that SIB_REP can be a value between 4 and
         //  4096.  We therefore give the slowest possible repitition rate to ensure
         //  good odds of getting every possible SIB at least once.
         // Note: An optimal timeout could be used if the MIB, SB1 and SB2 were decoded.
         //static const U32 MAX_SIB_RX_BLOCKS = 4096; // 81.9 seconds!
         static const U32 MAX_SIB_RX_BLOCKS = 1000; 

         Pasn_BchDecodeParams();
         ~Pasn_BchDecodeParams();

         void Reset(void);
         void ProcessSibData(const U8 *data);
         void BitstreamToSIB11String(const U8* byteStream,U32 byteStreamLen)
         {
            char convertbuff[3];

            for(U32 bCtr = 0;bCtr < byteStreamLen;bCtr++)
            {
               sprintf(convertbuff,"%02X",byteStream[bCtr]);
               m_sib11String += convertbuff;
            }
         }

         bool HaveWeGotEverythingFromBch();
         void checkTransmittedSibInMib(RruMasteInfBlock *mib);

         // declaration for mandatory params to get from decoder after scan
         u32      m_plmnType;
         u32      m_mcc; 
         u32      m_mnc;
         u32      m_lac;
         U8       m_rac;
         u32      m_cellId;
         int      m_pcpichTxPwr;
         bool     m_csgInd;
         U32      m_csgId;
         char     m_uarfcnDlLst[128];
         U32      m_startPsc;
         U32      m_numOfPsc;
         U32      m_pscRange;
         S32      m_latiSign;
         U32      m_latitude;
         U32      m_longitude;
         U32      m_unSemiMajor;
         U32      m_unSemiMinor;
         U32      m_orieMajorAxis;
         U32      m_confid;

         // Flags for check mandatory SB/SIBs transmitted
         bool m_transmitSB1;
         bool m_transmitSIB1;
         bool m_transmitSIB3;
         bool m_transmitSIB5;
         bool m_transmitSIB6;
         bool m_transmitSIB15;

         // Flags for check mandatory SB/SIBs decoded
         bool m_decodedMIB;
         bool m_decodedSB1;
         bool m_decodedSIB1;
         bool m_decodedSIB3;
         bool m_decodedSIB5;
         bool m_decodedSIB6;
         bool m_decodedSIB11;
         bool m_decodedSIB15;

         std::string m_sib11String;

         U32 m_bcchBchCount;
         U32 m_consecutiveCrcFails;


      private:	
         bool GetBit(U8 *array, U32 bit);
         void SetBit(U8 *array, U32 bit, bool value);

         void ClearBitsToOctet(U8 *array, U32 bit);

         void ProcessSegmentDataFixed(RruSIB_Typ sibType, RruSIB_Data_fixe &df);
         void ProcessSegmentDataVariable(RruSIB_Typ sibType, RruSIB_Data_vari &dv);
         void ProcessCompleteSibList(RruComplSIB_List &sibList);
         void ProcessFirstSegment(RruFirstSegme &fs);
         void ProcessFirstSegmentShort(RruFirstSegmeShort &fss);
         void ProcessSubsequentSegment(RruSubseSegme &ss);
         void ProcessLastSegmentShort(RruLastSegmeShort &lss);
         void ProcessLastSegment(RruLastSegme &ls);
         void ProcessSibType( RruSIB_Typ RruSIB_Typ, const U8 * octets, u32 length);
         bool QuerySibTypeNeedsProcessing(U32 sibType);
         void MarkSibTypeProcessed(U32 sibType);
         void SibDecodePlmnIdentity(RruPLMN_Ident * plmnIdentity);
         void SegInfo(const char *msg, RruSIB_Typ &sibType, u32 segmentIndex);
         void markSIBAsBeingTransmitted(U32 sibType);
         void checkTransmittedSibInSB(RruSIB_ReferList *sib_ReferList);
         void checkTransmittedSibInMib(RruSIBSb_ReferList *sibSb_ReferList);

         static const U8 NUMBER_OF_SIB_TABLES = (RRU_SIB_20 + 1);// total of 31+1

         struct sibTypeStatus
         {
            bool decoded;          // If TRUE, the SIB has been decoded

            u32 segCount;          // Count of fragments in the SIB
            u32 segLastProcessed;  // Last recieved fragment

            U16 dataBit;
            U8  concatStore[444];  // (16*222)/8 ; Storage to accomodate 16 SIB
                                   // segments each of length 222 bits.
         }

         m_sibTypeStatus[NUMBER_OF_SIB_TABLES];

         enum ExtnSibs
         {
            SIB_11bis,
            SIB_15bis
         };

   };

}
#endif 

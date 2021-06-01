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

     Name:     REM-APP

     Type:     C++ Source file

     Desc:     This file consists of the fsm functions for REM-APP

     File:     rem_wcdmaconfig.cpp

     Sid:      rem_wcdmaconfig.cpp@@/main/Tenb_T3k_Perf/1 - Wed Jan  9 20:53:24 2013

     Prg:      Ambika S


**************************************************************************************************/

#include "rem_datatype.h"
#include "rem_wcdmaconfig.h"
#include "rem_common.h"
#include <stdlib.h>
#include <string.h>
#ifdef USE_RADISYS_OAM
#include "Trace.h"
#endif

CRemWcdmaConfigData CRemWcdmaConfigData::m_Instance;

////////////////////////////////////////////////////////////////////////////////
//   @struct __stWcdmaBandinfo
////////////////////////////////////////////////////////////////////////////////
typedef struct {
 U8 	band;
 U16 	usNDL_Low;
 U16 	usNDL_Max;
} __stWcdmaBandinfo;

__stWcdmaBandinfo g_stWcdmaBandList[] =
{
	// BAND , NDL_Low, NDL_Max
	{1, 10562, 10838},
	{2, 9662, 9938},
	{3, 1162, 1513},
	{4, 1537, 1738},
	{5, 4357, 4458},
	{6, 4387, 4413},
	{7, 2237, 2563},
	{8, 2937, 3088},
	{9, 9237, 9387},
	{10,3112, 3388},
	{11,3712, 3787},
	{12,3842, 3903},
	{13,4017, 4043},
	{14,4117, 4113},
	{19,712 ,  763},
	{20,4512, 4638},
	{21,862 ,  912},
	{22,4662, 5038},
	{25,5112, 5413},
	{26,5762, 5913},
	{0, 0, 0}
};

typedef struct {
   char* romanBand;
   U8    intBand;
} __stWcdmaRomanToIntBand;

__stWcdmaRomanToIntBand g_stWcdmaRomanIntBandMap[]=
{
   {(char *) "I", 1},
   {(char *) "II", 2},
   {(char *) "III", 3},
   {(char *) "IV", 4},
   {(char *) "V", 5},
   {(char *) "VI", 6},
   {(char *) "VII", 7},
   {(char *) "VIII", 8},
   {(char *) "IX", 9},
   {(char *) "X", 10},
   {(char *) "XI", 11},
   {(char *) "XII", 12},
   {(char *) "XIII", 13},
   {(char *) "XIV", 14},
   {(char *) "XIX", 19},
   {(char *) "XX", 20},
   {(char *) "XXI", 21},
   {(char *) "XXII", 22},
   {(char *) "XXV", 25},
   {(char *) "XXVI", 26}
};

CRemWcdmaConfigData::CRemWcdmaConfigData()
{
	m_usScanTimeout = 300;
	m_usRssiTimeout = 0;
	m_bMaxCellEntry = 64;
}


////////////////////////////////////////////////////////////////////////////////
// @ param[in] void
//
////////////////////////////////////////////////////////////////////////////////
CRemWcdmaConfigData & CRemWcdmaConfigData::getInstance()
{
	return m_Instance;
}
////////////////////////////////////////////////////////////////////////////////
// @ param[in] const char* band : the band in roman numerals which needs to be
//                                converted to integer 
//
////////////////////////////////////////////////////////////////////////////////
unsigned int CRemWcdmaConfigData::convertFromRomanToInt(const char* romanBand)
{
   int mapSize = sizeof(g_stWcdmaRomanIntBandMap)/
                   sizeof(g_stWcdmaRomanIntBandMap[0]);

   for(int i = 0; i < mapSize; i++)
   {
      if(!strcmp(g_stWcdmaRomanIntBandMap[i].romanBand, romanBand))
      {
         return g_stWcdmaRomanIntBandMap[i].intBand;
      }
   }

   return 0;
}
void CRemWcdmaConfigData::saveBandList(std::string strBandList)
{
   std::string strBand;
   size_t	commaPos;  //coma position
   size_t  currPos = 0;
   int band = 0;

   if( strBandList.empty() )
      return;

   m_tmBandList.clear();

   while((commaPos = strBandList.find(',',currPos))!= std::string::npos)
   {
      strBand = strBandList.substr(currPos, commaPos-currPos);
      currPos = commaPos+1;
      band = convertFromRomanToInt(strBand.c_str());
#ifdef ENABLE_REM_LOGS
      TRACE_PRINTF("saving band info [ %d ]\n",band);
#endif
      if(band != 0)
      {
         m_tmBandList[band].clear();
#ifdef ENABLE_REM_LOGS
         TRACE_PRINTF("saving band info [ %d ]\n",band);
#endif
      }
      else
      {
         TRACE_PRINTF("Invalid BAND Configured [ %s ]\n",strBand.c_str());
      }
   }

   strBand = strBandList.substr(currPos, commaPos);
   band = convertFromRomanToInt(strBand.c_str());

   if((band > 0 && band < 45) && (band != 15 && band != 16))
   {
      m_tmBandList[band].clear();
#ifdef ENABLE_REM_LOGS
      TRACE_PRINTF("saving band info [ %d ]\n",band);
#endif
   }
   else
   {
      TRACE_PRINTF("Invalid BAND Configured [ %d ]\n",band);
   }

   setRssiTimeoutForBandSearch((int)atoi(strBand.c_str()));
#ifdef ENABLE_REM_LOGS
   TRACE_PRINTF("BANDLIST SIZE: %d\n",m_tmBandList.size());

   TRACE_PRINTF("BANDS  :  ");
   for(BANDLIST::const_iterator it = m_tmBandList.begin();
         it != m_tmBandList.end(); ++it)
   {
      TRACE_PRINTF("%d   \n",it->first);
   }
#endif
}

void CRemWcdmaConfigData::setRssiTimeoutForBandSearch(int band)
{
   for(int i=0; g_stWcdmaBandList[i].band !=0; i++)
	{
      if( g_stWcdmaBandList[i].band  != band )
         continue;

      m_usRssiTimeout =(g_stWcdmaBandList[i].usNDL_Max -
                        g_stWcdmaBandList[i].usNDL_Low)*RSSI_TIMEOUT_PER_EARFCN;
	}
}


////////////////////////////////////////////////////////////////////////////////
// @ param[in] usEARFCN - EUTRA ARFCN
//
// This function gets the UTRA band from the scanned ARFCN
////////////////////////////////////////////////////////////////////////////////
U8 CRemWcdmaConfigData::getBand(U16 usARFCN)
{
	for(U8 cnt=0;cnt<REM_MAX_ARFCN;cnt++)
   {
      if((usARFCN >= g_stWcdmaBandList[cnt].usNDL_Low ) && 
         (usARFCN <=g_stWcdmaBandList[cnt].usNDL_Max))
      {
         return g_stWcdmaBandList[cnt].band;
      }
   }
   return 0;
}

void CRemWcdmaConfigData::configureAllBands()
{
	for(int i=0; g_stWcdmaBandList[i].band !=0; i++)
		m_tmBandList[g_stWcdmaBandList[i].band].clear();

}


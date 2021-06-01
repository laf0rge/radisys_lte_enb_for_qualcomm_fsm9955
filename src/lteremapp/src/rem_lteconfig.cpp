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

     File:     rem_lteconfig.cpp

     Sid:      rem_lteconfig.cpp@@/main/Tenb_T3k_Perf/1 - Wed Jan  9 20:53:23 2013

     Prg:      Vikram K
      	  	   Ambika S


**************************************************************************************************/

#include "rem_datatype.h"
#include "rem_lteconfig.h"
#include "rem_common.h"
#include <stdlib.h>
#ifdef USE_RADISYS_OAM
#include "Trace.h"
#endif

CRemLteConfigData CRemLteConfigData::m_Instance;

typedef struct {
 U8             band;
 U16            usNDL_Low;
 U16            usNDL_Max;
} __stLteBandnfo;

__stLteBandnfo g_stLteBandList[] =
{
        // BAND , NDL_Low, NDL_Max
        { 1, 0, 599 },
        { 2, 600, 1199 },
        { 3, 1200,1949 },
        { 4, 1950,2399 },
        { 5, 2400,2649 },
        { 6, 2650,2749 },
        { 7, 2750,3449 },
        { 8, 3450,3779 },
        { 9, 3800,4149 },
        { 10,4150,4749 },
        { 11,4750,4949 },
        { 12,5010,5179 },
        { 13,5180,5279 },
        { 14,5280,5379 },
        { 17,5730,5849 },
        { 18,5850,5999 },
        { 19,6000,6149 },
        { 20,6150,6449 },
        { 21,6450,6599 },
        { 22,6600,7399 },
        { 23,7500,7699 },
        { 24,7700,8039 },
        { 25,8040,8689 },
        { 26,8690,9039 },
        { 27,9040,9209 },
        { 28,9210,9659 },
        { 33,36000,36199},
        { 34,36200,36349},
        { 35,36350,36949},
        { 36,36950,37549},
        { 37,37550,37749},
        { 38,37750,38249},
        { 39,38250,38649},
        { 40,38650,39649},
        { 41,39650,41589},
        { 42,41590,43589},
        { 43,43590,45589},
        { 44,45590,46589},
        { 0, 0, 0 }
};

CRemLteConfigData::CRemLteConfigData()
{
	m_usScanTimeout = 300;
	m_usRssiTimeout = 0;
	m_bMaxCellEntry = 64;
   /* CNM Changes */
   m_bCnmEnabled = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] void
// This function returns the class object
///////////////////////////////////////////////////////////////////////////////////////////////////
CRemLteConfigData & CRemLteConfigData::getInstance()
{
	return m_Instance;
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] strBandList - Comma separated Band list
//
// This function extracts the Bands from a comma separated list and copies it to member list of
// the class
////////////////////////////////////////////////////////////////////////////////
void CRemLteConfigData::saveBandList(std::string strBandList)
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
      band = atoi(strBand.c_str());
#ifdef ENABLE_REM_LOGS
      TRACE_PRINTF("saving band info [ %d ]\n",band);
#endif

      if((band > 0 && band < 45) && (band != 15 && band != 16))
      {
         m_tmBandList[atoi(strBand.c_str())].clear();
#ifdef ENABLE_REM_LOGS
         TRACE_PRINTF("saving band info [ %d ]\n",band);
#endif
      }
      else
      {
         TRACE_PRINTF("Invalid BAND Configured [ %d ]\n",band);
      }
   }

   strBand = strBandList.substr(currPos, commaPos);
   band = atoi(strBand.c_str());
   if((band > 0 && band < 45) && (band != 15 && band != 16))
   {
      m_tmBandList[atoi(strBand.c_str())].clear();
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
   TRACE_PRINTF("BANDS        :  ");

      for(BANDLIST::const_iterator it = m_tmBandList.begin();
            it != m_tmBandList.end(); ++it)
      {
         TRACE_PRINTF("%d   ",it->first);
      }
#endif
}

void CRemLteConfigData::setRssiTimeoutForBandSearch(int band)
{
	for(int i=0; g_stLteBandList[i].band !=0; i++)
	{
		if(  g_stLteBandList[i].band  != band )
			continue;

		m_usRssiTimeout = (g_stLteBandList[i].usNDL_Max-g_stLteBandList[i].usNDL_Low)*RSSI_TIMEOUT_PER_EARFCN;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] usEARFCN - EUTRA ARFCN
//
// This function gets the EUTRA band from the scanned EARFCN
///////////////////////////////////////////////////////////////////////////////////////////////////
U8 CRemLteConfigData::getBand(U16 usEARFCN)
{
	for(int i=0; g_stLteBandList[i].band !=0; i++)
	{
		if((usEARFCN >= g_stLteBandList[i].usNDL_Low ) && (usEARFCN <=g_stLteBandList[i].usNDL_Max))
			return g_stLteBandList[i].band;
	}
	return 0;
}

#ifdef LTE_TDD
///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] iTimeout
//
// This function populates the timeout configuration parameter to member variable
///////////////////////////////////////////////////////////////////////////////////////////////////
void CRemLteConfigData::savesubframeAssignment(U8 subframeAssignment)
{
   m_subframeAssignment = subframeAssignment;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] iTimeout
//
// This function populates the timeout configuration parameter to member variable
///////////////////////////////////////////////////////////////////////////////////////////////////
void CRemLteConfigData::savespecialSubframePatterns(U8 specialSubframePatterns)
{
   m_specialSubframePatterns = specialSubframePatterns;
}
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] enDlBw Nfft
//
// This function populates the timeout configuration parameter to member variable
///////////////////////////////////////////////////////////////////////////////////////////////////
void CRemLteConfigData::saveDlBw(RemBandwidth enDlBw)
{
   m_enDlBw = enDlBw;
}

#ifdef LTE_QCOM
void CRemLteConfigData::saveBandSupported( U16 bandSupported)
{
   m_bandSupported = bandSupported;

}

void CRemLteConfigData::saveSupportedTech( U8 supportedTech)
{
   m_supportedTech = supportedTech;
}
#endif



void CRemLteConfigData::configureAllBands()
{
	for(int i=0; g_stLteBandList[i].band !=0; i++)
		m_tmBandList[g_stLteBandList[i].band].clear();

}

#ifdef LTE_TDD
U8 CRemLteConfigData::getsubframeAssignment()
{
   return m_subframeAssignment;
}


U8 CRemLteConfigData::getspecialSubframePatterns()
{
   return m_specialSubframePatterns;
}
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] Nfft
//
// This function returns the DL Bandwidth Configured to the Cell
///////////////////////////////////////////////////////////////////////////////////////////////////
RemBandwidth CRemLteConfigData::getDlBw()
{
   return m_enDlBw;
}


/* CNM Api's*/
void CRemLteConfigData::saveCnmEnableFlag(bool cnmEnable)
{
   m_bCnmEnabled = cnmEnable;
}

#ifdef ENABLE_CNM
void CRemLteConfigData::saveCnmServCellPci(U16 servCellPci)
{
   m_servCellPci = servCellPci;
}
void CRemLteConfigData::saveCnmCsgStartPhyId(U16 csgStartId)
{
   m_csgPhysCellIdStart = csgStartId;
}
void CRemLteConfigData::saveCnmCsgPhyIdRange(U16 csgRange)
{
   m_csgPhysCellIdRange = csgRange;
}

bool CRemLteConfigData::getCnmEnableFlag()
{
   return m_bCnmEnabled;
}

U16 CRemLteConfigData::getCnmServCellPci()
{
   return m_servCellPci;
}
U16 CRemLteConfigData::getCnmCsgStartPhyId()
{
   return m_csgPhysCellIdStart;
}
U16 CRemLteConfigData::getCnmCsgPhyIdRange()
{
   return m_csgPhysCellIdRange;
}
#endif

#ifdef LTE_QCOM
U16 CRemLteConfigData::getBandSupported()
{
   return m_bandSupported;
}

U8 CRemLteConfigData::getSupportedTech()
{
   return m_supportedTech;
}
#endif

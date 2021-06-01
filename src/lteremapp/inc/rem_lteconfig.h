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

*************************************************************************************************17*/


/**************************************************************************************************

     Name:     REM-APP

     Type:     C++ Header file

     Desc:     Consists of LTE configuration class and member functions

     File:     rem_lteconfig.h

     Sid:      rem_lteconfig.h@@/main/Tenb_T3k_Perf/1 - Wed Jan  9 20:53:12 2013

     Prg:      Vikram K
     	 	   Ambika S


**************************************************************************************************/

#include <string>
#include "rem_common.h"
#include "rem_rat_config.h"
#ifndef REM_LTECONFIG_H_
#define REM_LTECONFIG_H_

#define REM_EARFCN_HIGH_VAL 46589

class CRemLteConfigData : public CRemRatConfigData
{
public:
	static CRemLteConfigData & getInstance();

	void saveBandList(std::string strBandList);

   /* CNM related apis */
	void saveCnmEnableFlag(bool cnmEnable);
#ifdef ENABLE_CNM
	void saveCnmServCellPci(U16 servCellPci);
	void saveCnmCsgStartPhyId(U16 csgStartId);
   void saveCnmCsgPhyIdRange(U16 csgRange);


   bool getCnmEnableFlag();
   U16 getCnmServCellPci();
   U16 getCnmCsgStartPhyId();
   U16 getCnmCsgPhyIdRange();
#endif
#ifdef LTE_TDD
   void savesubframeAssignment(U8 subframeAssignment);
   void savespecialSubframePatterns(U8 specialSubframePatterns);
   U8 getsubframeAssignment();
   U8 getspecialSubframePatterns();
#endif
   /* Nfft changes for DL BW */
   void saveDlBw(RemBandwidth enDlBw);
   RemBandwidth getDlBw();

#ifdef LTE_QCOM
   void saveBandSupported( U16 bandSupported);
   U16 getBandSupported();
   void saveSupportedTech( U8 supportedTech);
   U8 getSupportedTech();
#endif


private:
	U8 getBand(U16 usEARFCN);
	void setRssiTimeoutForBandSearch(int band);
	void configureAllBands();
	CRemLteConfigData();

private:
	std::list <U16> 		m_tlPCIList;
#ifdef LTE_TDD
   U8 m_subframeAssignment;
   U8 m_specialSubframePatterns;
#endif
   /* Nfft changes for DL BW */
   RemBandwidth m_enDlBw;
   /* CNM related configuration expected from OAM */
   bool  m_bCnmEnabled;
#ifdef ENABLE_CNM
   U16   m_servCellPci;
   U16   m_csgPhysCellIdStart;
   U16   m_csgPhysCellIdRange;
#endif

#ifdef LTE_QCOM
   U16 m_bandSupported;
   U8 m_supportedTech;
#endif

	static CRemLteConfigData m_Instance;
};

#endif /* REM_LTECONFIG_H_ */

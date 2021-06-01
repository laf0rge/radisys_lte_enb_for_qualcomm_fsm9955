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

     Name:     RRM Application

     Type:     CPP source file

     Desc:     This file contains 
               

     File:     rm_utils.cpp

     Sid:      qf_utils.cpp@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:37 2013

     Prg:     hkrishna 

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="RRM";
static int RLOG_MODULE_ID=128;
static int RLOG_FILE_ID=160;

/* header include files */
#include "rm_cmn.h"
#include "rm_utils.h"

U8 g_bQciToPriorityMap[RM_QCI_MAX_VALUE] =
{
	RM_QCI_PRIORITY_VAL2,
	RM_QCI_PRIORITY_VAL4,
	RM_QCI_PRIORITY_VAL3,
	RM_QCI_PRIORITY_VAL5,
	RM_QCI_PRIORITY_VAL1,
	RM_QCI_PRIORITY_VAL6,
	RM_QCI_PRIORITY_VAL7,
	RM_QCI_PRIORITY_VAL8,
	RM_QCI_PRIORITY_VAL9
};

U8	getQciPriority(U8 bQci)
{
	return  ((bQci > RM_QCI_MAX_VALUE) || (bQci == 0)) ? RM_LEAST_QCI_PRIORITY : (g_bQciToPriorityMap[bQci-1]);
}

/* Number of DL subframes present till a particular subframe 
 * rows are different config Mode , and columns are till now how many total 
 * Dl+SL subframes are available 
 **/
PUBLIC RmTddNumDlSubfrmTbl rmTddNumDlSubfrmTbl = {
        {1, 2, 2, 2, 2, 3, 4, 4, 4, 4},
        {1, 2, 2, 2, 3, 4, 5, 5, 5, 6},
        {1, 2, 2, 3, 4, 5, 6, 6, 7, 8},
        {1, 2, 2, 2, 2, 3, 4, 5, 6, 7},
        {1, 2, 2, 2, 3, 4, 5, 6, 7, 8},
        {1, 2, 2, 3, 4, 5, 6, 7, 8, 9},
        {1, 2, 2, 2, 2, 3, 4, 4, 4, 5}
};

/* Number of UL subframes present before a particular subframe 
 * rows are different config Mode , and columns are till now how many total 
 * UL subframes  
 */
PUBLIC RmTddNumUlSubfrmTbl rmTddNumUlSubfrmTbl = {
        {0, 0, 1, 2, 3, 3, 3, 4, 5, 6},
        {0, 0, 1, 2, 2, 2, 2, 3, 4, 4},
        {0, 0, 1, 1, 1, 1, 1, 2, 2, 2},
        {0, 0, 1, 2, 3, 3, 3, 3, 3, 3},
        {0, 0, 1, 2, 2, 2, 2, 2, 2, 2},
        {0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 1, 2, 3, 3, 3, 4, 5, 5}
};

void getTotalNumberOfDlUlSubframes(LrmUlDlConfig cfgMode, 
                                   U32 timingReqInMs,
                                   U32 *totalNumDlSf,
                                   U32 *totalNumUlSf)
{
   /* stores the Dl plus special subframes per frame based on cfg mode*/
   U8  numOfDlPlusSpecialSfsPerFrame = rmTddNumDlSubfrmTbl[cfgMode][RM_NUM_SF_PER_SFN-1];
   /* stores number of Ul subframes per frame based on cfg mode*/
   U8  numOfUlSfsPerFrame = rmTddNumUlSubfrmTbl[cfgMode][RM_NUM_SF_PER_SFN-1];
   U8  numOfSubFramesLessThanFrame;
   
   /* stores the number of DL plus special sub-frames per frame based on timing
    * requirement*/
   *totalNumDlSf = numOfDlPlusSpecialSfsPerFrame * (timingReqInMs/RM_NUM_SF_PER_SFN);
   
   /* stores the number of UL sub-frames per frame based on timing
          * requirement*/
   *totalNumUlSf = numOfUlSfsPerFrame * (timingReqInMs/RM_NUM_SF_PER_SFN);
   
   /* number of subframes less than a frame <10 ms */
   numOfSubFramesLessThanFrame = timingReqInMs % RM_NUM_SF_PER_SFN;

   /* update total DL and UL subframes with number of sub frames < a frame */
   if(numOfSubFramesLessThanFrame != 0)
   {
      (*totalNumDlSf) += rmTddNumDlSubfrmTbl[cfgMode][numOfSubFramesLessThanFrame];    
      (*totalNumUlSf) += rmTddNumUlSubfrmTbl[cfgMode][numOfSubFramesLessThanFrame];    
   }
   return;
}

bool isGbrBearer(U32 bQci)
{
   return (bQci >= RM_RBC_GBR_MIN_QCI && bQci <= RM_RBC_GBR_MAX_QCI);
}

bool isValidQci(U32 bQci)
{
   return (bQci >= RM_RBC_GBR_MIN_QCI && bQci <= RM_RBC_MAX_QCI);
}

U32 getNumDlSubFramesPerFrame(U32 bCfgMode)
{
   return (rmTddNumDlSubfrmTbl[bCfgMode][RM_NUM_SF_PER_SFN-1]);
}

U32 getNumUlSubFramesPerFrame(U32 bCfgMode)
{
   return (rmTddNumUlSubfrmTbl[bCfgMode][RM_NUM_SF_PER_SFN-1]);
}

U8 getNumPrbs(LrmCellBandwidth enDlBw)
{
   U8 bNumRbs;

   switch(enDlBw)
   {
      case LRM_BANDWIDTH_25:
      {
         bNumRbs = 25;
      }
      break;

      case LRM_BANDWIDTH_50:
      {
         bNumRbs = 50;
      }
      break;

      case LRM_BANDWIDTH_75:
      {   
         bNumRbs = 75;
      }
      break; 

      case LRM_BANDWIDTH_100:
      {   
         bNumRbs = 100;
      }
      break;

      default:
      {
         RLOG1 (L_ERROR, "Invalid Bandwidth Configuration [%d]. "
               "Assuming MAX PRBS as 100", enDlBw);
         bNumRbs = 100;
         break;
      }
   }
   return bNumRbs;
}

RmuUErabAddModIe *getRbAddModIe(RmuErabConfigIe & stERAB, U8 bErabId)
{
   U32   uiRbIdx;
   for(uiRbIdx = 0; uiRbIdx < stERAB.usNoErabsToAddMod; uiRbIdx++)
   {
      if(stERAB.stErabAddModLst[uiRbIdx].bErabId == bErabId)
      {
         RETVALUE(&stERAB.stErabAddModLst[uiRbIdx]);
      }
   }
   RETVALUE(NULLP);
}


/********************************************************************30**

           End of file:     qf_utils.cpp@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:37 2013

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
*********************************************************************91*/

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

     Type:     CPP include file

     Desc:     This file contains 
               
     File:     rm_utils.h

     Sid:      rm_utils.h@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:42 2013

     Prg:      vkorade

*********************************************************************21*/

/** @file rm_utils.h
 *  @brief This file contains utilities required for RRM
 */

#ifndef RM_UTILS_H_
#define RM_UTILS_H_
#include "rm_cmn.h"

#define RM_LEAST_QCI_PRIORITY   255        /*!< Least QCI Priority */
#define RM_ABS_VAL(_x,_y) ((_x > _y)?((_x) - (_y)):((_y) - (_x)))

/*!< It contains all possible QCI Priority Values */

typedef enum rmQciPriority
{
   RM_QCI_PRIORITY_VAL1 = 1,  /*!< QCI Priority 1 */
   RM_QCI_PRIORITY_VAL2,      /*!< QCI Priority 2 */
   RM_QCI_PRIORITY_VAL3,      /*!< QCI Priority 3 */
   RM_QCI_PRIORITY_VAL4,      /*!< QCI Priority 4 */
   RM_QCI_PRIORITY_VAL5,      /*!< QCI Priority 5 */
   RM_QCI_PRIORITY_VAL6,      /*!< QCI Priority 6*/
   RM_QCI_PRIORITY_VAL7,      /*!< QCI Priority 7*/
   RM_QCI_PRIORITY_VAL8,      /*!< QCI Priority 8*/
   RM_QCI_PRIORITY_VAL9,      /*!< QCI Priority 9*/ 
   RM_QCI_MAX_VALUE           /*!< Maximum QCI Priority Value*/
} RmQciPriority;

U8	getQciPriority(U8 bQci);

/* Number of DL subframes and Special subframes with DwPTS */
typedef U8 RmTddNumDlSubfrmTbl[LRM_UL_DL_CONFIG_MAX][RM_NUM_SF_PER_SFN];
EXTERN RmTddNumDlSubfrmTbl rmTddNumDlSubfrmTbl;

/* Number of UL subframes */
typedef U8 RmTddNumUlSubfrmTbl[LRM_UL_DL_CONFIG_MAX][RM_NUM_SF_PER_SFN];
EXTERN RmTddNumUlSubfrmTbl rmTddNumUlSubfrmTbl;

/**
 * @brief Function to get the total number of UL and DL subframes based on QCI
 *        timing requirement and configuration mode
 *
 * @details Function to get the total number of UL and DL subframes based on QCI
 *          timing requirement and configuration mode
 *
 * @param[in] cfgMode COnfiguration modes from 0 to 6
 * @param[in] timingReqInMs Timing requirements in milliseconds
 * @param[in/out] totalNumDlSf total number of DL subframes
 * @param[in/out] totalNumUlSf total number of UL subframes
 *
 * @return void
 */
void getTotalNumberOfDlUlSubframes(LrmUlDlConfig cfgMode, 
                                   U32 timingReqInMs,
                                   U32 *totalNumDlSf,
                                   U32 *totalNumUlSf);

/**
 * @brief The function to get number of Prbs
 *
 * @details The function to get number of Prbs
 *
 * @param[in] enDlBw holds the DL bandwidth
 *
 * @return U32
           returns the number of PRB's            
 */
U8 getNumPrbs(LrmCellBandwidth enDlBw);

/**
 * @brief function to get number of DL subframes per frame
 *
 * @details function to get number of DL subframes per frame
 *
 * @param[in] bCfgMode holds the configuration mode between 0 to 6
 *
 * @return U32
 *         returns the number of DL subrames in a given frame based on the
 *         configuration mode
 */
U32 getNumDlSubFramesPerFrame(U32 bCfgMode);

/**
 * @brief function to get number of UL subframes per frame
 *
 * @details function to get number of UL subframes per frame
 *
 * @param[in] bCfgMode holds the configuration mode
 *
 * @return U32
 *         returns the number of UL subframes in a given frame based on the
 *         configuration mode
 */
U32 getNumUlSubFramesPerFrame(U32 bCfgMode);

/**
 * @brief Function to fetch ERAB info corresponding to the ERAB id.
 *
 * @detail Function to fetch the ERAB add info from list of incoming ERABs 
 *         for the particular ERAB ID.
 *
 * @param[in]     stErab   List of incoming ERABs
 * @param[in]     bErabId  ERAB Identity
 *
 * @return  RmuUErabAddModIe *
 *          Returns address to ERAB add IE from list of incoming ERABs 
 *          for the corresponding ERAB ID
 */
RmuUErabAddModIe *getRbAddModIe(RmuErabConfigIe & stERAB, U8 bErabId);

/*!< True: if it is a GBR bearer; else False */
bool isGbrBearer(U32 qci);

/*!< True: if it is a valid QCI; else False */
bool isValidQci(U32 qci);

#endif /* RM_UTILS_H_ */
/********************************************************************30**

           End of file:     rm_utils.h@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:42 2013

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

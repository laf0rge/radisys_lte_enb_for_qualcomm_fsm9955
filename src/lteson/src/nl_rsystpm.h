/********************************************************************16**

            (c) Copyright 2015 by RadiSys Corporation. All rights reserved.

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

     Name:     LTE SON Application

     Type:     C header file

     Desc:     This file contains 
               

     File:     nl_rsystpm.h

     Sid:      nl_rsystpm.h@@/main/tenb_main_ccb/1 - Thu Jan 21 17:51:28 2016

     Prg:      raghu

*********************************************************************21*/

#ifndef NL_RSYSTPM_H_
#define NL_RSYSTPM_H_

#define NL_MAX_GAMMA_RANGE    6
#define NL_MAX_RSRP_DIFF      20
#define NL_NGH_REF_POWER     -5  /* Neighbor Cell's RSRP TX power */
#define NL_MIN_RSRP          -60 /* SIB2 EPRE */

/* The below table is used to derive the referenceSignal Power
   of the source cell. The values in the table are derived 
   based on the the formula as below :
   
   rsrpRatio = 10 ^ ((remRsrp - tgtRsrp) / 10*gamma); 
   referenceSigPower = nghRsTxPwr - 10*gamma*Log10(rsrpRatio/1-rsrpRatio);
   
   Where,
   remRsrp    is the Neighbor cell's REM scan RSRP,
   tgtRsrp    is the source cell's target RSRP at cell edge,
   gamma      is the attenuation factor
   nghRsTxPwr is the referenceSignal power of Neighbor cell from it's SIB.   
  
   Below are some examples :-
   
   1) If Neighbor Cell RS TX Power = -5
         Neighbor Cell REM Rsrp    = -110
         Serving  Cell Target Rsrp = -90
      then
         referenceSignalPower      = 14.7
   
   2) If Neighbor Cell RS TX Power = -5
         Neighbor Cell REM Rsrp    = -109
         Serving  Cell Target Rsrp = -90
      then
         referenceSignalPower      = 13.6

   3) If Neighbor Cell RS TX Power = -10
         Neighbor Cell REM Rsrp    = -110
         Serving  Cell Target Rsrp = -90
      then
         referenceSignalPower      = 9.7 = (14.7 - 5)

   It can be seen from the above examples 1 and 2 that the 
   referenceSignalPower mainly depends on the difference between the 
   REM RSRP and configured target RSRP. 
   
   From example 3 it is clear that for the same difference of 20 between
   tgtRsrp and remRsrp, the referenceSignalPower can be easily derived for
   any Neighbor Rs Tx power.
   
   So a single table is derived for a range of 20 delta values between
   REM RSRP and target RSRP. Delta range 1 to 20. This table is derived
   by setting nghRsTxPwr = -5.  
    
   These  set of 20 values are derived for different attenuation factors.
   In the measurement data collected, we found that the attenuation factor
   is approximately 2. Suitable attenuation factor may be selected based
   on test results */  

S8 rsysTpmTable[NL_MAX_GAMMA_RANGE][NL_MAX_RSRP_DIFF] = 
{
   /* gamma = 1.5 */
   {-17,-12,-9,-7,-5,-3,-1,1,3,4,5,6,7,9,10,11,12,13,14,15}, 
   /* gamma = 2 */
   {-24,-17,-13,-10,-8,-5,-4,-2,1,2,4,5,6,8,9,10,11,12,13,14}, 
   /* gamma = 2.5 */
   {-31,-23,-18,-14,-11,-9,-7,-5,-3,-1,2,3,5,6,7,9,10,11,12,14}, 
   /* gamma = 3 */
   {-38,-29,-23,-19,-15,-12,-10,-8,-6,-4,-2,1,2,4,5,7,8,10,11,12}, 
   /* gamma = 3.5 */
   {-46,-35,-29,-24,-20,-16,-14,-11,-9,-7,-5,-3,-1,2,3,5,6,8,9,11}, 
   /* gamma = 4 */
   {-55,-42,-34,-29,-25,-21,-18,-15,-12,-10,-8,-6,-4,-2,1,3,4,6,7,9}, 
};   

/* Range check is to limit the RSRP to below 0 dBm for femto cell */
#define NL_TPM_RSRP_CHK_RANGE(_cellCb, _rsrpPwr) \
{\
   if (_rsrpPwr > _cellCb->rsysTpmRsrpMax)\
   {\
      _rsrpPwr = _cellCb->rsysTpmRsrpMax;\
   }\
   else if (_rsrpPwr < _cellCb->rsysTpmRsrpMin)\
   {\
      _rsrpPwr = _cellCb->rsysTpmRsrpMin;\
   }\
}

#endif

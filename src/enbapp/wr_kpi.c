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
  
     Name:     Key Performance Indices Collector Module
  
     Type:     Functionality fileNew Employee's Induction
  
     Desc:     Provides List of KPIs supported
  
     File:     wr_kpi.c
  
     Sid:      
  
     Prg:      sp      

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="ENBAPP";
static int RLOG_MODULE_ID=64;
static int RLOG_FILE_ID=10;
#include "wr_umm.h"
#include "wr_emm.h"
#include "wr_kpi.h"
#include "wr_kpiIds.h"
#include "wr_smm_init.h"
#include "wr_msm_common.h"
#include "wr_smm_init.h"

PUBLIC WrKpiInfo kpiInfo;
EXTERN MsmWcdmaNodeBparams wcdmaNodeBparams;
EXTERN Void smGetS1apStats(Void);

/** @brief This function recevies Stop Kpi Collection Cfm 
 *
 * @details This function recevies Stop Kpi Collection Cfm
 *   from MAC/RLC/PDCP layer
 *
 *
 *     Function: wrKpiStopKpiCollecPrcCfm
 *
 * @param[out]
 * @param[in]
 * @return  S16
 *        -# Success : ROK
 */
 
PUBLIC S16 wrKpiStopKpiCollecPrcCfm
(
 Void
 )
{
   TRC2(wrKpiStopKpiCollecPrcCfm);

   RETVALUE(ROK);  
} /* wrKpiStopKpiCollecPrcCfm*/

/** @brief This function starts the Kpis Collection Procedure 
 *          
 * @details This function starts the Kpis Collection Procedure
*          
 *          
 *     Function: wrKpiStopKpiCollecPrc
 *          
 * @param[out]            
 * @param[in]        
 * @return  S16
 *        -# Success : ROK
 */

PUBLIC S16 wrKpiStopKpiCollecPrc
(
Void
)
{
   TRC2(wrKpiStopKpiCollecPrc);

#ifdef LTE_L2_MEAS
   wrMemCpuL3MeasSendReq();
   wrMemCpuL3MeasStopReq();
#endif
   if(kpiInfo.l2CollecEna == TRUE)
   {
#ifdef LTE_L2_MEAS
      wrSndL2MeasSendReq();
      wrSndL2MeasStopReq();
#endif
   }
   else
   {
      //wrKpiDInitKpiInfo(); 
      wrKpiStopKpiCollecPrcCfm();
   }
   RETVALUE(ROK);
} /* end of wrKpiStopKpiCollecPrc */
 
/** @brief This function initialise/resets kpiInfo structure
 *          
 * @details This function resets kpiInfo structure
 *          
 *          
 *     Function: wrKpiResetKpiInfo
 *          
 * @param[out]            
 * @param[in]        
 * @return  S16
 *        -# Success : ROK
 */

PUBLIC S16 wrKpiInitKpiInfo
(
S16 evnt,
Bool l2CollecEna
)
{
   TRC2(wrKpiInitKpiInfo);

   if(evnt == WR_KPI_INIT_KPI_INFO || evnt == WR_KPI_RESET_KPI_INFO)
   {

      (Void)cmMemset((U8*)&kpiInfo.kpiMeanInfo,0,sizeof(WrKpiMeanInfo));
      (Void)cmMemset((U8*)&kpiInfo.kpiMaxInfo,0,sizeof(WrKpiMaxInfo));
      kpiInfo.startCollec = TRUE;

      RLOG0(L_INFO, "Marking startCollec as true");
      kpiInfo.kpiCollectionTmrExpry = FALSE;
#ifdef LTE_L2_MEAS
      if(l2CollecEna == TRUE)
      {
         kpiInfo.l2CollecEna = TRUE;
         kpiInfo.transId = WR_KPI_SET_TO_ONE;
      }
     else
     {
         kpiInfo.l2CollecEna = FALSE;
         RLOG0(L_INFO, "Marking kpiInfo.l2CollecEna to false");
     }
#endif/*LTE_L2_MEAS*/
   }
   else /*WR_TMR_KPI_PREDFND_INTRVL_TMR*/
   {
      kpiInfo.kpiMeanInfo.rrcConnMean.numOfRrcCon = WR_KPI_SET_TO_ZERO;
      kpiInfo.kpiMeanInfo.rrcConnMean.numOfRrcConEmergency = WR_KPI_SET_TO_ZERO;
      kpiInfo.kpiMeanInfo.rrcConnMean.numOfRrcConHighPriorAcc = WR_KPI_SET_TO_ZERO;
      kpiInfo.kpiMeanInfo.rrcConnMean.numOfRrcConMtAcc = WR_KPI_SET_TO_ZERO;
      kpiInfo.kpiMeanInfo.rrcConnMean.numOfRrcConMoSig = WR_KPI_SET_TO_ZERO;
      kpiInfo.kpiMeanInfo.rrcConnMean.numOfRrcConMoData = WR_KPI_SET_TO_ZERO;
      kpiInfo.kpiMeanInfo.rrcConnMean.numOfRrcConDelTolAcc = WR_KPI_SET_TO_ZERO;
      kpiInfo.kpiMeanInfo.erabMean.numOfEarb = WR_KPI_SET_TO_ZERO;
      kpiInfo.kpiMaxInfo.rrcConnMax.numOfRrcCon = WR_KPI_SET_TO_ZERO;
      kpiInfo.kpiMaxInfo.erabMax.numOfErab = WR_KPI_SET_TO_ZERO;
      /*CSG_DEV Start */
      kpiInfo.kpiMeanInfo.csgUeMean.numOfCsgUe = WR_KPI_SET_TO_ZERO;
   }

   RETVALUE(ROK);
} /* end of wrKpiInitKpiInfo */
 
/** @brief This function starts the Kpis Collection Procedure 
 *          
 * @details This function starts the Kpis Collection Procedure
*          
 *          
 *     Function: wrKpiStartKpiCollecPrc
 *          
 * @param[out]            
 * @param[in]        
 * @return  S16
 *        -# Success : ROK
 */

PUBLIC S16 wrKpiStartKpiCollecPrc
(
Bool l2CollecEna 
)
{
   TRC2(wrKpiStartKpiCollecPrc);
   kpiInfo.l2CollecEna = l2CollecEna;
  
   RLOG2(L_DEBUG, "smCb.smState(%d) cellstate(%d)",
    smCb.smState,smCb.cellState); 
   //if(smCb.smState != WR_SM_STATE_CELL_UP)
   if(FALSE == smCb.cellState)
   {
     kpiInfo.startCollec = FALSE;
#ifdef LTE_L2_MEAS
     kpiInfo.measConfigRecv = 0;
#endif
     RLOG0(L_DEBUG, "Marking startCollec as FALSE");
     SGetEpcTime(&kpiInfo.startTime);
     //kpiInfo.kpiMeasPrd = 60000;
     /*Print message cell should be up*/
     RETVALUE(ROK);
   }
   
   if(kpiInfo.startCollec == TRUE)
   {
     /*Kpi Measurement is already going on, first stop on going collection*/
     wrStopTmr((PTR)NULLP,WR_TMR_KPI_PREDFND_INTRVL_TMR);
     wrKpiStopKpiCollecPrc();
     SGetEpcTime(&kpiInfo.startTime);
   }

   cmInitTimers(&(kpiInfo.kpiPredfndIntrvlTmr), 1);
   /*Intialise Kpi Info strucutre*/
   wrKpiInitKpiInfo(WR_KPI_INIT_KPI_INFO,l2CollecEna);
   /*Starts a predefined timer of value 5mins for sampling*/
   wrStartTmr((PTR)NULLP,WR_TMR_KPI_PREDFND_INTRVL_TMR,WR_TMR_KPI_PREDFND_INTRVL_TMR_VAL);
#ifdef LTE_L2_MEAS
   wrMemCpuUtilMeasStartReq();
   if(l2CollecEna == TRUE)
   {
    /*Sends L2 Meas Req to lower layer*/
    wrSendL2MeasStartReq(); 
  }
#endif /*LTE_L2_MEAS*/
 RETVALUE(ROK);
} /* end of wrKpiStartKpiCollecPrc */

/** @brief This function initialise/resets kpiInfo structure
 *          
 * @details This function initialise/resets kpiInfo structure
*          
 *          
 *     Function: wrKpiDInitKpiInfo 
 *          
 * @param[out]            
 * @param[in]        
 * @return  S16
 *        -# Success : ROK
 */


PUBLIC S16 wrKpiDInitKpiInfo
(
Void
)
{
   RLOG0(L_DEBUG, " ********* Resetting KPI info ************");
   TRC2(wrKpiDInitKpiInfo);

   (Void)cmMemset((U8*)&kpiInfo,0,sizeof(WrKpiInfo));
   RETVALUE(ROK);
} /* end of wrKpiDInitKpiInfo */

/** @brief This function resets kpiInfo structure
 *          
 * @details This function resets kpiInfo structure
*          
 *          
 *     Function: wrKpiResetKpiInfo
 *          
 * @param[out]            
 * @param[in]        
 * @return  S16
 *        -# Success : ROK
 */


PUBLIC S16 wrKpiResetKpiInfo
(
S16 evnt
)
{
   TRC2(wrKpiResetKpiInfo);

   /*it reset kpiInfo*/
   if(evnt == WR_KPI_INIT_KPI_INFO)
   {

      (Void)cmMemset((U8*)&kpiInfo.kpiMeanInfo,0,sizeof(WrKpiMeanInfo));
      (Void)cmMemset((U8*)&kpiInfo.kpiMaxInfo,0,sizeof(WrKpiMaxInfo));
      kpiInfo.kpiCollectionTmrExpry = FALSE; 
   }
   else /*WR_TMR_KPI_PREDFND_INTRVL_TMR*/
   {
      kpiInfo.kpiMeanInfo.rrcConnMean.numOfRrcCon = WR_KPI_SET_TO_ZERO;
      kpiInfo.kpiMeanInfo.rrcConnMean.numOfRrcConEmergency = WR_KPI_SET_TO_ZERO;
      kpiInfo.kpiMeanInfo.rrcConnMean.numOfRrcConHighPriorAcc = WR_KPI_SET_TO_ZERO;
      kpiInfo.kpiMeanInfo.rrcConnMean.numOfRrcConMtAcc = WR_KPI_SET_TO_ZERO;
      kpiInfo.kpiMeanInfo.rrcConnMean.numOfRrcConMoSig = WR_KPI_SET_TO_ZERO;
      kpiInfo.kpiMeanInfo.rrcConnMean.numOfRrcConMoData = WR_KPI_SET_TO_ZERO;
      kpiInfo.kpiMeanInfo.rrcConnMean.numOfRrcConDelTolAcc = WR_KPI_SET_TO_ZERO;
      kpiInfo.kpiMeanInfo.erabMean.numOfEarb = WR_KPI_SET_TO_ZERO;
      kpiInfo.kpiMaxInfo.rrcConnMax.numOfRrcCon = WR_KPI_SET_TO_ZERO;
      kpiInfo.kpiMaxInfo.erabMax.numOfErab = WR_KPI_SET_TO_ZERO;
/* CSG_DEV */
      kpiInfo.kpiMeanInfo.csgUeMean.numOfCsgUe = WR_KPI_SET_TO_ZERO;

   }

   RETVALUE(ROK);
} /* end of wrKpiResetKpiInfo */

/** @brief This function fills kpiInfo structure for Multi-instance KPI
 *          
 * @details This unction fills kpiInfo structure for Multi-instance KPI
 *          
 *          
 *     Function: wrFillKpiInfoeCgi
 *          
 * @param[out]            
 * @param[in]         
 * @return  S16
 * -# Success : ROK
 */

PRIVATE S16 wrFillKpiInfoeCgi(KpiInstInfo *kpiInfo1, WrEcgi *eCgi)
{

   TRC2(wrFillKpiInfoeCgi);
   if (eCgi != NULL)
   {
      kpiInfo1->kpiInstType       = KP_INST_TYPE_ECGI;
      kpiInfo1->u.eCgi.cellId     = eCgi->eutranCellId;
      kpiInfo1->u.eCgi.numMncDigs = eCgi->plmnId.numMncDigits;
      kpiInfo1->u.eCgi.mcc[0]     = eCgi->plmnId.mcc[0];
      kpiInfo1->u.eCgi.mcc[1]     = eCgi->plmnId.mcc[1];
      kpiInfo1->u.eCgi.mcc[2]     = eCgi->plmnId.mcc[2];
      kpiInfo1->u.eCgi.mnc[0]     = eCgi->plmnId.mnc[0];
      kpiInfo1->u.eCgi.mnc[1]     = eCgi->plmnId.mnc[1];
      if (eCgi->plmnId.numMncDigits > 2)
      {
         kpiInfo1->u.eCgi.mnc[2]  = eCgi->plmnId.mnc[2];
      }
      else
      {
         kpiInfo1->u.eCgi.mnc[2]  = 0;
      }

   }
   RETVALUE(ROK);
}

/** @brief This function updates Mean and Max of RRC Connection Estab in kpiInfo
 *          
 * @details This function updates Mean and Max of number of Rrc Connection Estab
 *  in kpiInfo strucutre
 *          
 *          
 *     Function: wrUpdateRrcConnEstabTimeMeanMax
 *          
 * @param[out]            
 * @param[in]       U32 estabCause and U32 estabInterval          
 * @return  S16 
 *        -# Success 
 */

PUBLIC S16 wrUpdateRrcConnEstabTimeMeanMax
(
U32 estabCause,
EpcTime rrcConReqRcvdTm,
EpcTime rrcConCmplRcvdTm
)
{

   U32 *prvSum;
   U32 *prvMax;
   U32 estabInterval;

   TRC2(wrUpdateRrcConnEstabTimeMeanMax);

   prvSum = NULLP;
   prvMax = NULLP;
   estabInterval = 0;
   /*calculate inteval in ms*/
   estabInterval = (U32)(rrcConCmplRcvdTm - rrcConReqRcvdTm); 


   /* The mean will be calculated as arithmetic mean of all samples and not 
      average per subsequent KPI updates */
   switch(estabCause)
   {
      case NhuEstablishmentCauseemergencyEnum:
         prvSum = &(kpiInfo.kpiMeanInfo.rrcConnEstabTimeSum.emergency); 
         prvMax = &(kpiInfo.kpiMaxInfo.rrcConnEstabTimeMax.emergency);       
         break;
      case NhuEstablishmentCausehighPriorityAccessEnum:
         prvSum = &(kpiInfo.kpiMeanInfo.rrcConnEstabTimeSum.
               highPriorityAccess);
         prvMax = &(kpiInfo.kpiMaxInfo.rrcConnEstabTimeMax.
               highPriorityAccess);
         break;
      case NhuEstablishmentCausemt_AccessEnum:
         prvSum = &(kpiInfo.kpiMeanInfo.rrcConnEstabTimeSum.mtAccess);
         prvMax = &(kpiInfo.kpiMaxInfo.rrcConnEstabTimeMax.mtAccess);
         break;
      case NhuEstablishmentCausemo_SignallingEnum:
         prvSum = &(kpiInfo.kpiMeanInfo.rrcConnEstabTimeSum.moSignalling);
         prvMax = &(kpiInfo.kpiMaxInfo.rrcConnEstabTimeMax.moSignalling);
         break;
      case NhuEstablishmentCausemo_DataEnum:
         prvSum = &(kpiInfo.kpiMeanInfo.rrcConnEstabTimeSum.moData);
         prvMax = &(kpiInfo.kpiMaxInfo.rrcConnEstabTimeMax.moData);
         break;
      case NhuEstablishmentCausedelayTolerantAccess_v1020Enum:
         prvSum = &(kpiInfo.kpiMeanInfo.rrcConnEstabTimeSum.delayTolAccess);
         prvMax = &(kpiInfo.kpiMaxInfo.rrcConnEstabTimeMax.delayTolAccess);
         break;
      default:
         RLOG0(L_ERROR, "wrUpdateRrcConnEstabTimeMeanMax :Invalid\
                  estab cause");
         RETVALUE(RFAILED); 
   }

   WR_CAL_SUM(prvSum,estabInterval); 
   WR_CAL_MAX(prvMax,estabInterval);

   RETVALUE(ROK);
} /* end of wrUpdateRrcConnEstabTimeMeanMax */

/** @brief This function updates Mean and Max of  Erab Estab time in kpiInfo
 * structure
 *          
 * @details This function updates Mean and Max of number of Erab Estab time in
 * kpiInfo structure
 *          
 *          
 *     Function: wrUpdateErabEstabTimeMeanMax
 *          
 * @param[out]            
 * @param[in]          U32 qciType and U32 estabInterval
 * @return  S16
 *        -# Success : 
 */

PUBLIC S16 wrUpdateErabEstabTimeMeanMax
(
U32 qciType,
EpcTime ErabReqRcvdTm,
EpcTime ErabEstabTm)
{

   U32  estabInterval;
   U32 *prvMax;

   TRC2(wrUpdateErabEstabTimeMeanMax);

   prvMax = NULLP;
   estabInterval = 0;
   /*calcualte interval in ms */
   estabInterval = (U32)(ErabEstabTm - ErabReqRcvdTm);

   if(qciType <= MAX_QCI_VALUE)
   {
      /* ccpu00149628 */
      kpiInfo.kpiMeanInfo.erabEstabTimeMeanPrQci[qciType - 1] += estabInterval; 
      kpiInfo.kpiMeanInfo.noOfSuccEstERabs[qciType-1]++;
      /* ccpu00149628 */
      prvMax = &(kpiInfo.kpiMaxInfo.erabEstabTimeMaxPrQci[qciType - 1]);
   }
   else
   {
      RLOG0(L_ERROR, "wrUpdateErabEstabTimeMeanMax: Invalid QCI\
               value");
      RETVALUE(RFAILED);
   }


   WR_CAL_MAX(prvMax,estabInterval);

   RETVALUE(ROK);
} /* end of wrUpdateErabEstabTimeMeanMax */

/** @brief This function increments KPI for RRC Connection Attempted per
 * establishment cause
 *          
 * @details This function increments KPI for RRC Connection Attempted  per
 * establishment cause
 *          
 *          
 *     Function: wrIncKpiFrRrcConAttmpted
 *          
 * @param[out]            
 * @param[in]          :  cause value. 
 * @return  S16
 *        -# Success : ROK
 */

PUBLIC S16 wrIncKpiFrRrcConAttmptd
(
U32 estabCause
)
{

   KpiId kpiType;

   TRC2(wrIncKpiFrRrcConAttmptd);

   kpiType = INVALID_KPI;

   switch(estabCause)
   {
      case NhuEstablishmentCauseemergencyEnum:
         kpiType = KPI_ID_LTE_RRC_ATTCONESTAB_EMERGENCY;
         break;
      case NhuEstablishmentCausehighPriorityAccessEnum:
         kpiType = KPI_ID_LTE_RRC_ATTCONESTAB_HIGHPRIORITYACCES;
         break;
      case NhuEstablishmentCausemt_AccessEnum:
         kpiType = KPI_ID_LTE_RRC_ATTCONESTAB_MTACCESS;
         break;
      case NhuEstablishmentCausemo_SignallingEnum:
         kpiType = KPI_ID_LTE_RRC_ATTCONESTAB_MOSIGNALLING;
         break;
      case NhuEstablishmentCausemo_DataEnum:
         kpiType = KPI_ID_LTE_RRC_ATTCONESTAB_MODATA;
         break;
         /* RRC-REL10-Upgrade */ 
      case NhuEstablishmentCausedelayTolerantAccess_v1020Enum:
         kpiType = KPI_ID_LTE_RRC_ATTCONESTAB_DE_TO_ACCESS;
         break;
      default:
         RLOG0(L_ERROR, "wrIncKpiFrRrcConAttmptd:Invalid cause");
         RETVALUE(RFAILED); 
   }

   WR_INC_KPI(kpiType,KPI_VALUE_TYPE_INTEGER,INC_KPI_VALUE_BY_ONE);
   WR_INC_KPI(KPI_ID_LTE_RRC_ATTCONESTAB_SUM,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);

   RETVALUE(ROK);
} /* end of wrIncKpiFrRrcConAttmptd */

/** @brief This function increments KPI for S1AP failure Causes
 *          
 * @details This function increments  KPI for S1AP failure Causes
 *          
 *          
 *     Function: wrIncKpiFrS1apFailCause
 *          
 * @param[out]            
 * @param[in]          causeType, causeVal
 * @return  S16
 *        -# Success : ROK
 */
/*  */
PUBLIC S16 wrIncKpiFrS1apFailCause(U32 causeType, U32 causeVal, U32 val)
{
    TRC2(wrIncKpiFrS1apFailCause);
    
    switch(causeType)
    {
       case WR_CAUSE_RADIONW:
            wrIncKpiFrS1apFailCauseForRadioNW(causeVal, val);
         break;
      case WR_CAUSE_TRANSPORT:
            wrIncKpiFrS1apFailCauseForTransport(causeVal, val);
         break;
      case WR_CAUSE_NAS:
            wrIncKpiFrS1apFailCauseForNas(causeVal, val);
         break;
      case  WR_CAUSE_PROTOCOL:
            wrIncKpiFrS1apFailCauseForProtocol(causeVal, val);
         break;
      case  WR_CAUSE_MISC:
            wrIncKpiFrS1apFailCauseForMisc(causeVal, val);
         break;
      default:
         RLOG0(L_ERROR, "wrIncKpiFrS1apFailCause: Invalid\
                  Cause Catagory");
         RETVALUE(RFAILED);
    }
    
    RETVALUE(ROK);
}/* end of wrIncKpiFrS1apFailCause */

/** @brief This function increments KPI for S1AP failure Causes w.r.t to RadioNW
 *          
 * @details This function increments  KPI for S1AP failure Causes w.r.t to RadioNW
 *          
 *          
 *     Function: wrIncKpiFrS1apFailCauseForRadioNW
 *          
 * @param[out]            
 * @param[in]          causeVal
 * @return  S16
 *        -# Success : ROK
 */
/*  */

PUBLIC S16 wrIncKpiFrS1apFailCauseForRadioNW(U32 causeVal, U32 val)
{
   TRC2(wrIncKpiFrS1apFailCauseForRadioNW);
   
   switch(causeVal)
   {
      case SztCauseRadioNwunspecifiedEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_00,KPI_VALUE_TYPE_INTEGER, val);
         break; 
      case SztCauseRadioNwtx2relocoverall_expiryEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_01,KPI_VALUE_TYPE_INTEGER, val);      
         break;      
      case SztCauseRadioNwsuccessful_handoverEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_02,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwrelease_due_to_eutran_generated_reasonEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_03,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwhandover_cancelledEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_04,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwpartial_handoverEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_05,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwho_failure_in_target_EPC_eNB_or_target_systemEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_06,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwho_target_not_allowedEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_07,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwtS1relocoverall_expiryEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_08,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwtS1relocprep_expiryEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_09,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwcell_not_availableEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_10,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwunknown_targetIDEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_11,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwno_radio_resources_available_in_target_cellEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_12,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwunknown_mme_ue_s1ap_idEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_13,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwunknown_enb_ue_s1ap_idEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_14,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwunknown_pair_ue_s1ap_idEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_15,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwhandover_desirable_for_radio_reasonEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_16,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwtime_critical_handoverEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_17,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwresource_optimisation_handoverEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_18,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwreduce_load_in_serving_cellEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_19,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwuser_inactivityEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_20,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwradio_connection_with_ue_lostEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_21,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwload_balancing_tau_requiredEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_22,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwcs_fallback_triggeredEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_23,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwue_not_available_for_ps_serviceEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_24,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwradio_resources_not_availableEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_25,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwfailure_in_radio_interface_procedureEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_26,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwinvalid_qos_combinationEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_27,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwinterrat_redirectionEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_28,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwinteraction_with_other_procedureEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_29,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwunknown_E_RAB_IDEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_30,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwmultiple_E_RAB_ID_instancesEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_31,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwencryption_and_or_integrity_protection_algorithms_not_supportedEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_32,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNws1_intra_system_handover_triggeredEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_33,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNws1_inter_system_handover_triggeredEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_34,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwx2_handover_triggeredEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_35,KPI_VALUE_TYPE_INTEGER, val);            
         break;
      case SztCauseRadioNwredirection_towards_1xRTTEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_36,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwnot_supported_QCI_valueEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_37,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      case SztCauseRadioNwinvalid_CSG_IdEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_RN_38,KPI_VALUE_TYPE_INTEGER, val);            
         break;      
      default:
         RLOG0(L_ERROR,"wrIncKpiFrS1apFailCauseForRadioNW: Invalid\
                  Cause Catagory");
         RETVALUE(RFAILED);        
   }
   RETVALUE(ROK);
}/* end of wrIncKpiFrS1apFailCauseForRadioNW */

/** @brief This function increments KPI for S1AP failure Causes w.r.t to Transport
 *          
 * @details This function increments  KPI for S1AP failure Causes w.r.t to Transport
 *          
 *          
 *     Function: wrIncKpiFrS1apFailCauseForTransport
 *          
 * @param[out]            
 * @param[in]          causeVal
 * @return  S16
 *        -# Success : ROK
 */
/*  */

PUBLIC S16 wrIncKpiFrS1apFailCauseForTransport(U32 causeVal, U32 val)
{
   TRC2(wrIncKpiFrS1apFailCauseForTransport);
   
   switch(causeVal)
   {
      case SztCauseTporttransport_resource_unavailableEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_TP_00,KPI_VALUE_TYPE_INTEGER, val);
         break; 
      case SztCauseTportunspecifiedEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_TP_01,KPI_VALUE_TYPE_INTEGER, val);      
         break;      
      default:
         RLOG0(L_ERROR,"wrIncKpiFrS1apFailCauseForTransport: Invalid\
                  Cause Catagory");
         RETVALUE(RFAILED);        
   }
   RETVALUE(ROK);
}/* end of wrIncKpiFrS1apFailCauseForTransport */

/** @brief This function increments KPI for S1AP failure Causes w.r.t to NAS
 *          
 * @details This function increments  KPI for S1AP failure Causes w.r.t to NAS
 *          
 *          
 *     Function: wrIncKpiFrS1apFailCauseForNas
 *          
 * @param[out]            
 * @param[in]          causeVal
 * @return  S16
 *        -# Success : ROK
 */
/*  */

PUBLIC S16 wrIncKpiFrS1apFailCauseForNas(U32 causeVal, U32 val)
{
   TRC2(wrIncKpiFrS1apFailCauseForNas);
   
   switch(causeVal)
   {
      case SztCauseNasnormal_releaseEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_NAS_00,KPI_VALUE_TYPE_INTEGER, val);
         break; 
      case SztCauseNasauthentication_failureEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_NAS_01,KPI_VALUE_TYPE_INTEGER, val);      
         break;
      case SztCauseNasdetachEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_NAS_02,KPI_VALUE_TYPE_INTEGER, val);
         break; 
      case SztCauseNasunspecifiedEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_NAS_03,KPI_VALUE_TYPE_INTEGER, val);      
         break;  
      case SztCauseNascsg_subscription_expiryEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_NAS_04,KPI_VALUE_TYPE_INTEGER, val);      
         break;  
      default:
         RLOG0(L_ERROR,"wrIncKpiFrS1apFailCauseForNas: Invalid\
                  Cause Catagory");
         RETVALUE(RFAILED);        
   }
   RETVALUE(ROK);
}/* end of wrIncKpiFrS1apFailCauseForNas */

/** @brief This function increments KPI for S1AP failure Causes w.r.t to Protocol
 *          
 * @details This function increments  KPI for S1AP failure Causes w.r.t to Protocol
 *          
 *          
 *     Function: wrIncKpiFrS1apFailCauseForProtocol
 *          
 * @param[out]            
 * @param[in]          causeVal
 * @return  S16
 *        -# Success : ROK
 */
/*  */

PUBLIC S16 wrIncKpiFrS1apFailCauseForProtocol(U32 causeVal, U32 val)
{
   TRC2(wrIncKpiFrS1apFailCauseForProtocol);
   
   switch(causeVal)
   {
      case SztCauseProttransfer_syntax_errorEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_PROT_00,KPI_VALUE_TYPE_INTEGER, val);
         break; 
      case SztCauseProtabstract_syntax_error_rejectEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_PROT_01,KPI_VALUE_TYPE_INTEGER, val);      
         break;
      case SztCauseProtabstract_syntax_error_ignore_and_notifyEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_PROT_02,KPI_VALUE_TYPE_INTEGER, val);
         break; 
      case SztCauseProtmessage_not_compatible_with_receiver_stateEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_PROT_03,KPI_VALUE_TYPE_INTEGER, val);      
         break;
      case SztCauseProtsemantic_errorEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_PROT_04,KPI_VALUE_TYPE_INTEGER, val);
         break; 
      case SztCauseProtabstract_syntax_error_falsely_constructed_messageEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_PROT_05,KPI_VALUE_TYPE_INTEGER, val);      
         break;
      case SztCauseProtunspecifiedEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_PROT_06,KPI_VALUE_TYPE_INTEGER, val);
         break; 
      default:
         RLOG0(L_ERROR, "wrIncKpiFrS1apFailCauseForProtocol: Invalid\
                  Cause Catagory");
         RETVALUE(RFAILED);        
   }
   RETVALUE(ROK);
}/* end of wrIncKpiFrS1apFailCauseForProtocol */

/** @brief This function increments KPI for S1AP failure Causes w.r.t to Misc
 *          
 * @details This function increments  KPI for S1AP failure Causes w.r.t to Misc
 *          
 *          
 *     Function: wrIncKpiFrS1apFailCauseForMisc
 *          
 * @param[out]            
 * @param[in]          causeVal
 * @return  S16
 *        -# Success : ROK
 */
/*  */

PUBLIC S16 wrIncKpiFrS1apFailCauseForMisc(U32 causeVal, U32 val)
{
   TRC2(wrIncKpiFrS1apFailCauseForMisc);
   
   switch(causeVal)
   {
      case SztCauseMisccontrol_processing_overloadEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_MISC_00,KPI_VALUE_TYPE_INTEGER, val);
         break; 
      case SztCauseMiscnot_enough_user_plane_processing_resourcesEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_MISC_01,KPI_VALUE_TYPE_INTEGER, val);      
         break;
      case SztCauseMischardware_failureEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_MISC_02,KPI_VALUE_TYPE_INTEGER, val);
         break; 
      case SztCauseMiscom_interventionEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_MISC_03,KPI_VALUE_TYPE_INTEGER, val);      
         break;
      case SztCauseMiscunspecifiedEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_MISC_04,KPI_VALUE_TYPE_INTEGER, val);
         break; 
      case SztCauseMiscunknown_PLMNEnum:
         WR_INC_KPI(KPI_ID_LTE_CFR_S1AP_MISC_05,KPI_VALUE_TYPE_INTEGER, val);      
         break; 
      default:
         RLOG0(L_ERROR, "wrIncKpiFrS1apFailCauseForMisc: Invalid\
                  Cause Catagory");
         RETVALUE(RFAILED);        
   }
   RETVALUE(ROK);
}/* end of wrIncKpiFrS1apFailCauseForMisc */


/** @brief This function increments KPI for egtp Failure Cause Counters
 *          
 * @details This function increments  KPI for egtp Failure Cause Counters
 *          
 *          
 *     Function: wrIncKpiFrEgtpFailure
 *          
 * @param[out]            
 * @param[in]          causeVal
 * @return  S16
 *        -# Success : ROK
 */
/*  */

PUBLIC S16 wrIncKpiFrEgtpFailure(U32 causeVal)
{
   TRC2(wrIncKpiFrEgtpFailure);
   
   switch(causeVal)
   {
      case EGT_GTPU_MSG_ERROR_IND:
         WR_INC_KPI( KPI_ID_LTE_CFR_GTP_00,KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
         break; 
      case EGT_GTPU_DEST_UNREACHABLE:
         WR_INC_KPI( KPI_ID_LTE_CFR_GTP_01,KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);      
         break;
      default:
         RLOG0(L_ERROR, "wrIncKpiFrEgtpFailure: Invalid\
                  Cause Catagory");
         RETVALUE(RFAILED);        
   }
   RETVALUE(ROK);
}/* end of wrIncKpiFrEgtpFailure */




/** @brief This function increments KPI for RLC Failure Cause Counters
 *          
 * @details This function increments  KPI for RLC Failure Cause Counters
 *          
 *          
 *     Function: wrIncKpiFrRlcFailure
 *          
 * @param[out]            
 * @param[in]          Void
 * @return  S16
 *        -# Success : ROK
 */
/*  */

PUBLIC S16 wrIncKpiFrRlcFailure(Void)
{
   TRC2(wrIncKpiFrRlcFailure);
   
   WR_INC_KPI( KPI_ID_LTE_CFR_RLC_00,KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
   
   RETVALUE(ROK);
}/* end of wrIncKpiFrRlcFailure */


/** @brief This function increments KPI for RRC Security Failure Cause Counter
 *          
 * @details This function increments  KPI for RRC Security Failure Cause Counter
 *          
 *          
 *     Function: wrIncKpiFrRrcSecFailCause
 *          
 * @param[out]            
 * @param[in]          Void
 * @return  S16
 *        -# Success : ROK
 */
/*  */

PUBLIC S16 wrIncKpiFrRrcSecFailCause(Void)
{
   TRC2(wrIncKpiFrRrcSecFailCause);
   
   WR_INC_KPI( KPI_ID_LTE_CFR_RRC_01,KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
   
   RETVALUE(ROK);
}/* end of wrIncKpiFrRrcSecFailCause */

/** @brief This function increments KPI for RRC S1AP Failure Cause Counter
 *          
 * @details This function increments  KPI for RRC S1AP Failure Cause Counter
 *          
 *          
 *     Function: wrIncKpiFrRrcS1apFailCause
 *          
 * @param[out]            
 * @param[in]          Void
 * @return  S16
 *        -# Success : ROK
 */
/*  */

PUBLIC S16 wrIncKpiFrRrcS1apFailCause(Void)
{
   TRC2(wrIncKpiFrRrcS1apFailCause);
   
   WR_INC_KPI( KPI_ID_LTE_RRC_S1AP_FAIL_COUNT,KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
   
   RETVALUE(ROK);
}/* end of wrIncKpiFrRrcS1apFailCause */

/** @brief This function increments KPI for RRC PDCP Failure Cause Counter
 *          
 * @details This function increments  KPI for RRC PDCP Failure Cause Counter
 *          
 *          
 *     Function: wrIncKpiFrRrcPdcpFailCause
 *          
 * @param[out]            
 * @param[in]          Void
 * @return  S16
 *        -# Success : ROK
 */
/*  */

PUBLIC S16 wrIncKpiFrRrcPdcpFailCause(Void)
{
   TRC2(wrIncKpiFrRrcPdcpFailCause);
   
   WR_INC_KPI( KPI_ID_LTE_RRC_PDCP_FAIL_COUNT,KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
   
   RETVALUE(ROK);
}/* end of wrIncKpiFrRrcPdcpFailCause */

/** @brief This function increments KPI for RRC RLC Failure Cause Counter
 *          
 * @details This function increments  KPI for RRC RLC Failure Cause Counter
 *          
 *          
 *     Function: wrIncKpiFrRrcRlcFailCause
 *          
 * @param[out]            
 * @param[in]          Void
 * @return  S16
 *        -# Success : ROK
 */
/*  */

PUBLIC S16 wrIncKpiFrRrcRlcFailCause(Void)
{
   TRC2(wrIncKpiFrRrcRlcFailCause);
   
   WR_INC_KPI( KPI_ID_LTE_RRC_RLC_FAIL_COUNT,KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
   
   RETVALUE(ROK);
}/* end of wrIncKpiFrRrcRlcFailCause */

/** @brief This function increments KPI for RRC MAC Failure Cause Counter
 *          
 * @details This function increments  KPI for RRC MAC Failure Cause Counter
 *          
 *          
 *     Function: wrIncKpiFrRrcMacFailCause
 *          
 * @param[out]            
 * @param[in]          Void
 * @return  S16
 *        -# Success : ROK
 */
/*  */

PUBLIC S16 wrIncKpiFrRrcMacFailCause(Void)
{
   TRC2(wrIncKpiFrRrcMacFailCause);
   
   WR_INC_KPI( KPI_ID_LTE_RRC_MAC_FAIL_COUNT,KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
   
   RETVALUE(ROK);
}/* end of wrIncKpiFrRrcMacFailCause */

/** @brief This function increments KPI for RRC RRC Failure Cause Counter
 *          
 * @details This function increments  KPI for RRC RRC Failure Cause Counter
 *          
 *          
 *     Function: wrIncKpiFrRrcRrcFailCause
 *          
 * @param[out]            
 * @param[in]          Void
 * @return  S16
 *        -# Success : ROK
 */
/*  */

PUBLIC S16 wrIncKpiFrRrcRrcFailCause(Void)
{
   TRC2(wrIncKpiFrRrcRrcFailCause);
   
   WR_INC_KPI( KPI_ID_LTE_RRC_RRC_FAIL_COUNT,KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
   
   RETVALUE(ROK);
}/* end of wrIncKpiFrRrcRrcFailCause */

/** @brief This function increments KPI for RRC Connection succuessful per
 * establishment cause
 *          
 * @details This function increments KPI for RRC Connection successful to
 * release per establishment cause
 *          
 *          
 *     Function: wrIncKpiFrRrcConSuccd
 *          
 * @param[out]            
 * @param[in]           cause value. 
 * @return  S16
 *        -# Success : ROK
 */

PUBLIC S16 wrIncKpiFrRrcConSuccd
(
U32 estabCause
)
{
   KpiId kpiType;

   TRC2(wrIncKpiFrRrcConSuccd);

   kpiType = INVALID_KPI;

   switch(estabCause)
   {

      case NhuEstablishmentCauseemergencyEnum:
         kpiType = KPI_ID_LTE_RRC_SUCCCONESTAB_EMERGENCY;
         kpiInfo.kpiMeanInfo.rrcConnMean.numOfRrcConEmergency++;
         break;
      case NhuEstablishmentCausehighPriorityAccessEnum:
         kpiType = KPI_ID_LTE_RRC_SUCCCONESTAB_HIGHPRIORITYACCES;
         kpiInfo.kpiMeanInfo.rrcConnMean.numOfRrcConHighPriorAcc++;
         break;
      case NhuEstablishmentCausemt_AccessEnum:
         kpiType = KPI_ID_LTE_RRC_SUCCCONESTAB_MTACCESS;
         kpiInfo.kpiMeanInfo.rrcConnMean.numOfRrcConMtAcc++;
         break;
      case NhuEstablishmentCausemo_SignallingEnum:
         kpiType = KPI_ID_LTE_RRC_SUCCCONESTAB_MOSIGNALLING;
         kpiInfo.kpiMeanInfo.rrcConnMean.numOfRrcConMoSig++;
         break;
      case NhuEstablishmentCausemo_DataEnum:
         kpiType = KPI_ID_LTE_RRC_SUCCCONESTAB_MODATA;
         kpiInfo.kpiMeanInfo.rrcConnMean.numOfRrcConMoData++;
         break;
      case NhuEstablishmentCausedelayTolerantAccess_v1020Enum:
         kpiType = KPI_ID_LTE_RRC_SUCCCONESTAB_DE_TO_ACCESS;
         kpiInfo.kpiMeanInfo.rrcConnMean.numOfRrcConDelTolAcc++;
         break;
      default:
         RLOG0(L_ERROR, "wrIncKpiFrRrcConSuccd: Invalid cause");
         RETVALUE(RFAILED);
   }
   /*Incrementing num of rrc conn within a predefined interval*/  
   kpiInfo.kpiMeanInfo.rrcConnMean.numOfRrcCon++;
   kpiInfo.kpiMaxInfo.rrcConnMax.numOfRrcCon++;

   WR_INC_KPI(kpiType,KPI_VALUE_TYPE_INTEGER,INC_KPI_VALUE_BY_ONE);
   WR_INC_KPI(KPI_ID_LTE_RRC_SUCCCONNESTAB_SUM,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);

   RETVALUE(ROK);

} /* end of wrIncKpiFrRrcConSuccd */

/** @brief This function increments KPI for succuessful CSG UE Attach
 * 
 *          
 * @details This function increments KPI for the Number of successful attach
 * CSG UE's 
 *          
 *          
 *     Function: wrIncKpiFrCsgUeAttachSuccd
 *          
 * @param[out]            
 * @param[in]           cause value. 
 * @return  S16
 *        -# Success : ROK
 */

PUBLIC S16 wrIncKpiFrCsgUeAttachSuccd 
(
Void
)
{

   TRC2(wrIncKpiFrCsgUeAttachSuccd);


   /*Incrementing num of  Successful Attached  UE for the CSG in a 
     predefined interval
   */  
   kpiInfo.kpiMeanInfo.csgUeMean.numOfCsgUe++;

   RETVALUE(ROK);
} /* end of wrIncKpiFrRrcConSuccd */

/** @brief This function increments KPI for RRC connection failure due to non
 *  availability of Uu resources per establishment cause       
 * @details This function increments KPI for RRC connection failure due to non
 *  availability of Uu resources per establishment cause       
 *          
 *     Function: wrIncKpiFrRrcCnFailDuToNoUuRsrc 
 *          
 * @param[out]            
 * @param[in]          cause value. 
 * @return  S16
 *        -# Success : ROK
 */

PUBLIC S16 wrIncKpiFrRrcCnFailDuToNoUuRsrc
(
U32 estabCause
)
{
   KpiId kpiType;

   TRC2(wrIncKpiFrRrcCnFailDuToNoUuRsrc);

   kpiType = INVALID_KPI;

   switch(estabCause)
   {

      case NhuEstablishmentCauseemergencyEnum:
         kpiType = KPI_ID_LTE_RRC_FAILCONESTAB_EMERGENCY;
         break;
      case NhuEstablishmentCausehighPriorityAccessEnum:
         kpiType = KPI_ID_LTE_RRC_FAILCONESTAB_HIGHPRIORITYACCES;
         break;
      case NhuEstablishmentCausemt_AccessEnum:
         kpiType = KPI_ID_LTE_RRC_FAILCONESTAB_MTACCESS;
         break;
      case NhuEstablishmentCausemo_SignallingEnum:
         kpiType = KPI_ID_LTE_RRC_FAILCONESTAB_MOSIGNALLING;
         break;
      case NhuEstablishmentCausemo_DataEnum:
         kpiType = KPI_ID_LTE_RRC_FAILCONESTAB_MODATA;
         break;
         /* RRC-REL10-Upgrade */ 
      case NhuEstablishmentCausedelayTolerantAccess_v1020Enum:
         kpiType = KPI_ID_LTE_RRC_FAILCONESTAB_DE_TO_ACCESS;
         break;
      default:
         RLOG0(L_ERROR, "wrIncKpiFrRrcCnFailDuToNoUuRsrc: Invalid\
                  cause" );
         RETVALUE(RFAILED);
   }

   WR_INC_KPI(kpiType,KPI_VALUE_TYPE_INTEGER,INC_KPI_VALUE_BY_ONE);
   WR_INC_KPI(KPI_ID_LTE_RRC_FAILCONNESTAB_SUM,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);
   RETVALUE(ROK);
} /* end of wrIncKpiFrRrcCnFailDuToNoUuRsrc */

/** @brief This function increments KPI for RRC Connection failure due to
 *  degraded radio per establishment cause
 * @details This function increments KPI for RRC Connection failure due to
 *  degraded radio per establishment cause
 *          
 *     Function: wrIncKpiFrRrcCnFailDuToDgrdRdio 
 *          
 * @param[out]            
 * @param[in]          cause: estabCause. 
 * @return  S16
 *        -# Success : ROK
 */

PUBLIC S16 wrIncKpiFrRrcCnFailDuToDgrdRdio
(
U32 estabCause
)
{
   KpiId kpiType;

   TRC2(wrIncKpiFrRrcCnFailDuToDgrdRdio);

   kpiType = INVALID_KPI;

   switch(estabCause)
   {

      case SztRRC_Establishment_CauseemergencyEnum:
         kpiType = KPI_ID_LTE_RRC_FAILCONESTAB_DEGRADEDRADIO_EMERGENCY;
         break;
      case  SztRRC_Establishment_CausehighPriorityAccessEnum:
         kpiType = KPI_ID_LTE_RRC_FAILCONESTAB_DEGRADEDRADIO_HIGHPRIORITYACCES;
         break;
      case SztRRC_Establishment_Causemt_AccessEnum:
         kpiType = KPI_ID_LTE_RRC_FAILCONESTAB_DEGRADEDRADIO_MTACCESS;
         break;
      case  SztRRC_Establishment_Causemo_SignallingEnum:
         kpiType = KPI_ID_LTE_RRC_FAILCONESTAB_DEGRADEDRADIO_MOSIGNALLING;
         break;
      case  SztRRC_Establishment_Causemo_DataEnum:
         kpiType = KPI_ID_LTE_RRC_FAILCONESTAB_DEGRADEDRADIO_MODATA;
         break;
         /* RRC-REL10-Upgrade */ 
      case  SztRRC_Establishment_Causedelay_TolerantAccessEnum:
         kpiType = KPI_ID_LTE_RRC_FAILCONESTAB_DEGRADEDRADIO_DE_TO_ACCESS;
         break;
      default:
         RLOG0(L_ERROR, "wrIncKpiFrRrcCnFailDuToDgrdRdio: Invalid\
                  cause");
         RETVALUE(RFAILED);
   }

   WR_INC_KPI(kpiType,KPI_VALUE_TYPE_INTEGER,INC_KPI_VALUE_BY_ONE);
   WR_INC_KPI(KPI_ID_LTE_RRC_FAILCONNESTAB_DEGRADEDRADIO_SUM,
         KPI_VALUE_TYPE_INTEGER,INC_KPI_VALUE_BY_ONE);
   RETVALUE(ROK);

} /* end of wrIncKpiFrRrcCnFailDuToDgrdRdio */

/** @brief This function increments KPI for RRC-Reestablishment Attempted per
 * establishment cause
 *          
 * @details This function increments KPI for RRC-Reestablishment Attempted per
 * establishment cause
 *          
 *          
 *     Function: wrIncKpiFrReestbAtmptdPrEtbCaus
 *          
 * @param[out]            
 * @param[in]          cause: estabcause . 
 * @return  S16
 *        -# Success : ROK
 */

PUBLIC S16 wrIncKpiFrReestbAtmptdPrEtbCaus 
(
U32 estabCause
)
{
   KpiId kpiType;

   TRC2(wrIncKpiFrReestbAtmptdPrEtbCaus);

   kpiType = INVALID_KPI;

   switch(estabCause)
   {

      case  NhuReestbCausereconfigurationFailEnum:
         kpiType = KPI_ID_LTE_RRC_CONNREESTABATT_RECONF_FAIL;
         break;
      case NhuReestbCausehandoverFailEnum:
         kpiType = KPI_ID_LTE_RRC_CONNREESTABATT_HO_FAIL;
         break;
      case  NhuReestbCauseotherFailEnum:
         kpiType = KPI_ID_LTE_RRC_CONNREESTABATT_OTHER;
         break;
      case  NhuReestbCausespare1Enum:
         break;
      default: 
         RLOG0(L_ERROR,"wrIncKpiFrReestbAtmptdPrEtbCaus: Invalid\
                  cause");
         RETVALUE(RFAILED);
   }
  
   if(kpiType != INVALID_KPI)
   {
     WR_INC_KPI(kpiType,KPI_VALUE_TYPE_INTEGER,INC_KPI_VALUE_BY_ONE);
   } 
   WR_INC_KPI(KPI_ID_LTE_RRC_CONNREESTABATT_SUM,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);
   RETVALUE(ROK);
} /* end of wrIncKpiFrReestbAtmptdPrEtbCaus */

/** @brief This function increments KPI for RRC-Reestablishment Successfull per
 * establishment cause
 *          
 * @details This function increments KPI for RRC-Reestablishmnet Successful per
 * establishment cause
 *          
 *          
 *     Function: wrIncKpiFrReestbSuccdPrEtabCaus
 *          
 * @param[out]            
 * @param[in]          cause: estabCause. 
 * @return  S16
 *        -# Success : ROK
 */

PUBLIC S16 wrIncKpiFrReestbSuccdPrEtabCaus
(
U32 estabCause
)
{
   KpiId kpiType;

   TRC2(wrIncKpiFrReestbSuccdPrEtabCaus);

   kpiType = INVALID_KPI;

   switch(estabCause)
   {
      case  NhuReestbCausereconfigurationFailEnum:
         kpiType = KPI_ID_LTE_RRC_CONNREESTABSUCC_RECONF_FAIL;
         break;
      case NhuReestbCausehandoverFailEnum:
         kpiType = KPI_ID_LTE_RRC_CONNREESTABSUCC_HO_FAIL;
         break;
      case  NhuReestbCauseotherFailEnum:
         kpiType = KPI_ID_LTE_RRC_CONNREESTABSUCC_OTHER;
         break;
      case  NhuReestbCausespare1Enum:        
         break;
      default:
         RLOG0(L_ERROR, "wrIncKpiFrReestbSuccdPrEtabCaus: Invalid\
                  cause");
         RETVALUE(RFAILED);
   }

  if(kpiType != INVALID_KPI)
  {
     WR_INC_KPI(kpiType,KPI_VALUE_TYPE_INTEGER,INC_KPI_VALUE_BY_ONE);
  }
  WR_INC_KPI(KPI_ID_LTE_RRC_CONNREESTABSUCC_SUM,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);
   RETVALUE(ROK);

} /* end of wrIncKpiFrReestbSuccdPrEtabCaus */

/** @brief This function increments KPI for RRC-Reestablishment failed with
 * establishment cause 
 *          
 * @details This function increments KPI for RRC-Reestablihsmnet failed wiht
 * establishment cause
 *          
 *          
 *     Function: wrIncKpiFrReestbFaildPrEtabCaus
 *          
 * @param[out]            
 * @param[in]          cause: cause type and cause value. 
 * @return  S16
 *        -# Success : ROK
 */

PUBLIC S16 wrIncKpiFrReestbFaildPrEtabCaus 
(
U32 estabCause
)
{
   KpiId kpiType;

   TRC2(wrIncKpiFrReestbFaildPrEtabCaus);

   kpiType = INVALID_KPI;

   switch(estabCause)
   {
      case  NhuReestbCausereconfigurationFailEnum:
         kpiType = KPI_ID_LTE_RRC_CONNREESTABFAIL_RECONF_FAIL;
         break;
      case NhuReestbCausehandoverFailEnum:
         kpiType = KPI_ID_LTE_RRC_CONNREESTABFAIL_HO_FAIL;
         break;
      case  NhuReestbCauseotherFailEnum:
         kpiType = KPI_ID_LTE_RRC_CONNREESTABFAIL_OTHER;
         break;
      case NhuReestbCausespare1Enum:
         break;
      default:
         RLOG0(L_ERROR, "wrIncKpiFrReestbFaildPrEtabCaus: Invalid\
                  cause");
         RETVALUE(RFAILED);

   }

   if(kpiType != INVALID_KPI)
   {
     WR_INC_KPI(kpiType,KPI_VALUE_TYPE_INTEGER,INC_KPI_VALUE_BY_ONE);
   } 
   WR_INC_KPI(KPI_ID_LTE_RRC_CONNREESTABFAIL_SUM,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);
   RETVALUE(ROK);
} /* end of wrIncKpiFrReestbFaildPrEtabCaus */

/** @brief This function increments KPI for Number of E-RABs failed to setup.
 *          
 * @details This function increments KPI for Number of E-RABs failed to setup.
 *          
 *          
 *     Function: wrIncKpiFrERABFailPrCause
 *          
 * @param[out]            
 * @param[in]          cause: cause type and cause value. 
 * @return  S16
 *        -# Success : ROK
 */                  
PUBLIC S16 wrIncKpiFrERABFailPrCause    
(           
WrUmmMsgCause *cause
)        
{          
   KpiId kpiType;

   TRC2(wrIncKpiFrERABFailPrCause);

   kpiType = INVALID_KPI;

   switch(cause->causeTyp)
   {
      case WR_CAUSE_RADIONW:
         switch(cause->causeVal)
         {
            case SztCauseRadioNwradio_resources_not_availableEnum:
               kpiType=KPI_ID_LTE_ERAB_ESTABFAILNBR_RADIORESOURCESNOTAVAILABLE;
               break;
            case SztCauseRadioNwfailure_in_radio_interface_procedureEnum:
               kpiType =  KPI_ID_LTE_ERAB_ESTABFAILNBR_FAILUREINRADIOINTERFACEPROCEDURE;
               break;
            case SztCauseRadioNwinvalid_qos_combinationEnum:
               kpiType = KPI_ID_LTE_ERAB_ESTABFAILNBR_INVALIDQOSCOMBINATIONS;
               break;
            case SztCauseRadioNwnot_supported_QCI_valueEnum:
               kpiType = KPI_ID_LTE_ERAB_ESTABFAILNBR_MULTIPLEERABIDINSTANCES;
               break;
            case SztCauseRadioNwmultiple_E_RAB_ID_instancesEnum:
               kpiType = KPI_ID_LTE_ERAB_ESTABFAILNBR_NOTSUPPORTEDQCIVALUE;
               break;
            case SztCauseRadioNws1_intra_system_handover_triggeredEnum:
               kpiType = KPI_ID_LTE_ERAB_ESTABFAILNBR_S1INTRASYSTEMHANDOVERTRIGGERED;
               break;
            case SztCauseRadioNws1_inter_system_handover_triggeredEnum:
               kpiType = KPI_ID_LTE_ERAB_ESTABFAILNBR_S1INTERSYSTEMHANDOVERTRIGGERED;
               break;
            case SztCauseRadioNwx2_handover_triggeredEnum:
               kpiType = KPI_ID_LTE_ERAB_ESTABFAILNBR_X2HANDOVERTRIGGERED;
               break;
            default:
               RLOG0(L_WARNING,"wrIncKpiFrERABFailPrCause: Invalid\
                        radio cause");
               RETVALUE(RFAILED); 
         }
         break;
      case WR_CAUSE_TRANSPORT:
         break;
      case WR_CAUSE_NAS:
         break;
      case  WR_CAUSE_PROTOCOL:
         break;
      case  WR_CAUSE_MISC:
         break;
      default:
         RLOG0(L_ERROR,"wrIncKpiFrERABFailPrCause: Invalid  Cause\
                  catagory");
         RETVALUE(RFAILED);
   }

   /*Increment KPI by 1*/
   if(kpiType != INVALID_KPI)
   { 
      WR_INC_KPI(kpiType,KPI_VALUE_TYPE_INTEGER,INC_KPI_VALUE_BY_ONE);
      WR_INC_KPI(KPI_ID_LTE_ERAB_ESTABFAILNBR_SUM,KPI_VALUE_TYPE_INTEGER,
            INC_KPI_VALUE_BY_ONE);
   }
   else
   {
      RLOG0(L_ERROR, "wrIncKpiFrERABFailPrCause:Invalid KPI Id");
   }

   RETVALUE(ROK);
} /* end of wrIncKpiFrERABFailPrCause */


/** @brief This function increments KPI for ERAB Requested to release per cause
 *          
 * @details This function increments KPI for ERAB Requested to release per cause
 *          
 *          
 *     Function: wrIncKpiFrERABReqToRelPrCause
 *          
 * @param[out]            
 * @param[in]          cause: cause type and cause value. 
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 wrIncKpiFrERABReqToRelPrCause
(
 WrUmmMsgCause                  *cause,
 U32                             numRabs
)
{
   KpiId kpiType;

   TRC2(wrIncKpiFrERABReqToRelPrCause);

   kpiType = INVALID_KPI;

   switch(cause->causeTyp)
   {

      case WR_CAUSE_RADIONW:
         switch(cause->causeVal)
         {
            case SztCauseRadioNwradio_resources_not_availableEnum:
               kpiType = KPI_ID_LTE_ERAB_RELENBNBR_RADIORESOURCESNOTAVAILABLE;
               break;
            case SztCauseRadioNwreduce_load_in_serving_cellEnum:
               kpiType =  KPI_ID_LTE_ERAB_RELENBNBR_REDUCELOADINSERVINGCELL;
               break;
            case SztCauseRadioNwfailure_in_radio_interface_procedureEnum:
               kpiType = KPI_ID_LTE_ERAB_RELENBNBR_FAILUREINTHERADIOINTERFACEPROCEDURE;
               break;
            case SztCauseRadioNwrelease_due_to_eutran_generated_reasonEnum:
               kpiType = KPI_ID_LTE_ERAB_RELENBNBR_RELEASEDUETOEUTRANGENERATEDREASONS;
               break;
            case SztCauseRadioNwuser_inactivityEnum:
               kpiType = KPI_ID_LTE_ERAB_RELENBNBR_USERINACTIVITY;
               break;
            case SztCauseRadioNwradio_connection_with_ue_lostEnum:
               kpiType = KPI_ID_LTE_ERAB_RELENBNBR_RADIOCONNECTIONWITHUELOST;
               break;
            default:
               RLOG0(L_ERROR, "wrIncKpiFrERABReqToRelPrCause:\
                        Invalid Radio Cause");
               RETVALUE(RFAILED); 
         }
         break;
      case WR_CAUSE_TRANSPORT:
         break;
      case WR_CAUSE_NAS:
         break;  
      case WR_CAUSE_PROTOCOL:
         break;
      case WR_CAUSE_MISC:
         switch(cause->causeVal)
         {
            case SztCauseMiscom_interventionEnum: 
               kpiType = KPI_ID_LTE_ERAB_RELENBNBR_OAMINTERVENTION;
               break;
            default:
               RLOG0(L_ERROR, "wrIncKpiFrERABReqToRelPrCause:\
                        Invalid Misc Cause"); 
               RETVALUE(RFAILED);
         }
         break;
      default:
         RLOG0(L_ERROR, "wrIncKpiFrERABReqToRelPrCause:Invalid\
                  Cause");
         RETVALUE(RFAILED); 
   }
   if(kpiType != INVALID_KPI)
   {
      /*Increment KPI pr cause*/
      WR_INC_KPI(kpiType,KPI_VALUE_TYPE_INTEGER,numRabs);
   }
   else
   {
      RLOG0(L_ERROR, "wrIncKpiFrERABReqToRelPrCause:Invalid KPI\
               ID");
   }
   RETVALUE(ROK);

} /* end of wrIncKpiFrERABReqToRelPrCause */

/** @brief This function increments KPI for ERAB Requested to release per QCI 
 *          
 * @details This function increments KPI for ERAB Requested to release per QCI
 *          
 *          
 *     Function: wrIncKpiFrERABReqToRelPrQci
 *          
 * @param[out]            
 * @param[in]          U8 qciVal. 
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 wrIncKpiFrERABReqToRelPrQci
(
U8 qciVal
)
{
   KpiId kpiId;
   TRC2(wrIncKpiFrERABReqToRelPrQci);
    
   kpiId = KPI_ID_LTE_ERAB_RELENBNBR_QCI1 + qciVal - 1;
   /*Increment KPI pr cause*/
   WR_INC_KPI(kpiId,KPI_VALUE_TYPE_INTEGER,1);
   /*Increment KPI total sum*/
   WR_INC_KPI(KPI_ID_LTE_ERAB_RELENBNBR_QCI_SUM,KPI_VALUE_TYPE_INTEGER, 1);

   RETVALUE(ROK);

} /* end of wrIncKpiFrERABReqToRelPrQci */

/** @brief This function increments KPI for ERAB Requested to release per cause
 *          
 * @details This function increments KPI for ERAB Requested to release per cause
 *          
 *          
 *     Function: wrIncKpiFrERABReqToRelPrCause
 *          
 * @param[out]            
 * @param[in]          cause: cause type and cause value. 
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 wrIncKpiFrERABRelCmdPrCause
(
WrUmmMsgCause                  *cause
)
{
   KpiId kpiType;

   TRC2(wrIncKpiFrERABRelCmdPrCause);

   kpiType = INVALID_KPI;

   switch(cause->causeTyp)
   {
      case WR_CAUSE_RADIONW:
         switch(cause->causeVal)
         {
            case SztCauseRadioNwunknown_E_RAB_IDEnum:
               kpiType = KPI_ID_LTE_ERAB_RELFAILNBR_UNKNOWNERABID;
               break;
            case  SztCauseRadioNwho_failure_in_target_EPC_eNB_or_target_systemEnum:
               kpiType = KPI_ID_LTE_ERAB_RELFAILNBR_HOFAILUREINTARGETSYSTEM;
               break;
            case  SztCauseRadioNwunknown_mme_ue_s1ap_idEnum:
               kpiType = KPI_ID_LTE_ERAB_RELFAILNBR_UNKNOWNAREALREADYALLOCATEDMMEUES1APID;
               break;
            case  SztCauseRadioNwunknown_enb_ue_s1ap_idEnum:
               kpiType = KPI_ID_LTE_ERAB_RELFAILNBR_UNKNOWNAREALREADYALLOCATEDENBUES1APID;
               break;
            default:
               RLOG0(L_ERROR, "wrIncKpiFrERABRelCmdPrCause: Invalid\
                        Radio Cause");
               RETVALUE(RFAILED);
         }
         break;
      case WR_CAUSE_TRANSPORT:
         break;
      case WR_CAUSE_NAS:
         break;
      case  WR_CAUSE_PROTOCOL:
         break;
      case  WR_CAUSE_MISC:
         break;
      default:
         RLOG0(L_ERROR, "wrIncKpiFrERABRelCmdPrCause: Invalid Cause\
                  Catagory");
         RETVALUE(RFAILED);
   }

   if(kpiType != INVALID_KPI)
   {   
      /*Increment KPI by pr cause*/
      WR_INC_KPI(kpiType,KPI_VALUE_TYPE_INTEGER,INC_KPI_VALUE_BY_ONE);
      /*Increment KPI for all causes */
      WR_INC_KPI(KPI_ID_LTE_ERAB_RELFAILNBR_SUM,KPI_VALUE_TYPE_INTEGER,
            INC_KPI_VALUE_BY_ONE);
   }
   else
   {
      RLOG0(L_ERROR, "wrIncKpiFrERABRelCmdPrCause:Invalid\
               KPI ID");
   }
   RETVALUE(ROK);
} /* end of wrIncKpiFrERABRelCmdPrCause */

/** @brief This function increments KPI for E-RAB modification Number of E-RABs
 * failed to modify the QoS parameter
 *          
 * @details This function increments KPI for *E-RAB modification Number of
 *  E-RABs failed to modify the QoS parameter
 *          
 *          
 *     Function: wrIncKpiFrERABModFailPrCause
 *          
 * @param[out]            
 * @param[in]          cause: cause type and cause value. 
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 wrIncKpiFrERABModFailPrCause
(
WrUmmMsgCause                  *cause
)
{
   KpiId kpiType;

   TRC2(wrIncKpiFrERABModFailPrCause);

   kpiType = INVALID_KPI;

   switch(cause->causeTyp)
   {
      case WR_CAUSE_RADIONW:
         switch(cause->causeVal)
         {
            case  SztCauseRadioNwunknown_E_RAB_IDEnum:
               kpiType = KPI_ID_LTE_ERAB_MODQOSFAILNBR_UNKNOWNERABID;
               break;
            case  SztCauseRadioNwmultiple_E_RAB_ID_instancesEnum:
               kpiType = KPI_ID_LTE_ERAB_MODQOSFAILNBR_MULTIPLEERABIDINSTANCES;
               break;
            case  SztCauseRadioNwradio_resources_not_availableEnum:
               kpiType=KPI_ID_LTE_ERAB_MODQOSFAILNBR_RADIORESOURCESNOTAVAILABLE;
               break;
            case   SztCauseRadioNwfailure_in_radio_interface_procedureEnum:
               kpiType = KPI_ID_LTE_ERAB_MODQOSFAILNBR_FAILUREINTHERADIOINTERFACEPROCEDURE;
               break;
            case  SztCauseRadioNwinvalid_qos_combinationEnum:
               kpiType = KPI_ID_LTE_ERAB_MODQOSFAILNBR_INVALIDQOSCOMBINATION;
               break;
            default:
               RLOG0(L_ERROR, "wrIncKpiFrERABModFailPrCause:Invalid\
                        Radio cause");
               RETVALUE(RFAILED);
         }

         break;
      case WR_CAUSE_TRANSPORT:
         break;
      case WR_CAUSE_NAS:
         break;
      case  WR_CAUSE_PROTOCOL:
         break;
      case  WR_CAUSE_MISC:
         break;
      default:
         RLOG0(L_ERROR, "wrIncKpiFrERABModFailPrCause:Invalid Cause\
                  catagory");
         RETVALUE(RFAILED); 
   }

   if(kpiType != INVALID_KPI)
   {
      /*Increment KPI by pr cause*/
      WR_INC_KPI(kpiType,KPI_VALUE_TYPE_INTEGER,INC_KPI_VALUE_BY_ONE);
      /*Increment KPI for all causes*/
      WR_INC_KPI(KPI_ID_LTE_ERAB_MODQOSFAILNBR_SUM,KPI_VALUE_TYPE_INTEGER,
            INC_KPI_VALUE_BY_ONE);
   }
   else
   {
      RLOG0(L_ERROR, "wrIncKpiFrERABModFailPrCause: Invalid KPI\
               ID");
   }

   RETVALUE(ROK);
} /* end of wrIncKpiFrERABModFailPrCause */

/* CSG_DEV Start */

/** @brief This function increments KPI for Number of attempted inbound Mobility
 * for CSG or Hybrid cell. 
 *          
 * @details This function increments KPI number of attempted inbound mobility
 * for a CSG cell or Hybrid cell.
 *          
 *          
 *     Function: wrIncKpiFrOutCsgHybridHovrAttmptd 
 *          
 * @param[out]            
 * @param[in]          cause: cause type and cause value. 
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 wrIncKpiFrInboundCsgMobilityAttmptd(Void)
{
   TRC2(wrIncKpiFrInboundCsgMobilityAttmptd);

   WR_INC_KPI(KPI_ID_LTE_CSG_ATTINBOUNDMOBILITY,
           KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);

   RETVALUE(ROK);

}/* wrIncKpiFrInboundCsgMobilityAttmptd */


/** @brief This function increments KPI for Number of attempted inbound Mobility
 * for CSG or Hybrid cell. 
 *          
 * @details This function increments KPI number of attempted inbound mobility
 * for a CSG cell or Hybrid cell.
 *          
 *          
 *     Function: wrIncKpiFrInboundCsgMobilitySuccd 
 *          
 * @param[out]            
 * @param[in]          cause: cause type and cause value. 
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 wrIncKpiFrInboundCsgMobilitySuccd(Void)
{
   TRC2(wrIncKpiFrInboundCsgMobilitySuccd);

   WR_INC_KPI(KPI_ID_LTE_CSG_SUCCINBOUNDMOBILITY,
           KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);

   RETVALUE(ROK);
}/* wrIncKpiFrInboundCsgMobilitySuccd */


/** @brief This function increments KPI for Number of attempted inbound Mobility
 * for CSG or Hybrid cell. 
 *          
 * @details This function increments KPI number of attempted inbound mobility
 * for a CSG cell or Hybrid cell.
 *          
 *          
 *     Function: wrIncKpiFrInboundCsgMobilityFaild 
 *          
 * @param[out]            
 * @param[in]          cause: cause type and cause value. 
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 wrIncKpiFrInboundCsgMobilityFaild(Void)
{
   TRC2(wrIncKpiFrInboundCsgMobilityFaild);

   WR_INC_KPI(KPI_ID_LTE_CSG_FAILEDINBOUNDMOBILITY_SUM,
           KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);

   RETVALUE(ROK);

}/* wrIncKpiFrInboundCsgMobilityFaild */
/* CSG_DEV Ends */

/** @brief This function increments KPI for Intra LTE HO specific to Outgoing
 *  X2 Based (source FAP) per cause
 *          
 * @details Attempted X2 outgoing inter-eNB handover executions per handover cause
 *          and sum.
 *          
 * Function: wrIncKpiFrOutX2BsdHovrAttmptd
 *          
 * @param[out]            
 * @param[in] cztCause, info  
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 wrIncKpiFrOutX2BsdHovrAttmptd
(
 WrUmmMsgCause    *cztCause,
 WrEcgi           *info
) 
{
   KpiInstInfo  kpiInfo1; /* Named with suffix1, so that later if different 
                             counters are incremented in the same function which
                             require different information to be passed to OAM,
                             new variable can be filled */

   TRC2(wrIncKpiFrOutX2BsdHovrAttmptd);

   /*  Fill KPI info required for this KPI */  
   wrFillKpiInfoeCgi(&kpiInfo1, info);

   switch(cztCause->causeTyp)
   {
      case WR_CAUSE_RADIONW:
         switch(cztCause->causeVal)
         {
            case CztCauseRadioNwtime_critical_handoverEnum:
               {
                  WR_INC_KPI(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_ATT_CAUSE1,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
                  WR_INC_KPI_INST(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_ATT_CAUSE1_PER_TGT_CELL,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE,kpiInfo1);
                  break;
               }
            case CztCauseRadioNwhandover_desirable_for_radio_reasonsEnum:
               {
                  WR_INC_KPI(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_ATT_CAUSE2,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
                  WR_INC_KPI_INST(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_ATT_CAUSE2_PER_TGT_CELL,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE,kpiInfo1);
                  break;
               }
            case CztCauseRadioNwresource_optimisation_handoverEnum:
               {
                  WR_INC_KPI(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_ATT_CAUSE3,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
                  WR_INC_KPI_INST(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_ATT_CAUSE3_PER_TGT_CELL,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE,kpiInfo1);
                  break;
               }
            case CztCauseRadioNwreduce_load_in_serving_cellEnum:
               {
                  WR_INC_KPI(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_ATT_CAUSE4,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
                  WR_INC_KPI_INST(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_ATT_CAUSE4_PER_TGT_CELL,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE,kpiInfo1);
                  break;
               }
            default:
               RLOG0(L_ERROR,"Invalid czt cause");
               RETVALUE(RFAILED);
         }
         break;
      default:
         RLOG0(L_ERROR,"Invalid cause catagory");
         RETVALUE(RFAILED);
   }

   WR_INC_KPI(KPI_ID_LTE_HO_TOENBATT_SUM,
         KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
   WR_INC_KPI_INST(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_ATT_SUM_PER_TGT_CELL,
         KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE,kpiInfo1);

   RETVALUE(ROK);
} /* end of wrIncKpiFrOutX2BsdHovrAttmptd */

/** @brief This function increments KPI for Intra LTE HO specific to Outgoing X2
 * Based (source FAP) per cause
 *          
 * @details Sucessful X2 outgoing inter-eNB handover executions per handover cause
 *          and sum.
 *          
 *     Function: wrIncKpiFrOutX2BsdHovrSuccd
 *          
 * @param[out]            
 * @param[in]  cztCause, info
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 wrIncKpiFrOutX2BsdHovrSuccd
(
 WrUmmMsgCause    *cztCause,
 WrEcgi           *info
)
{
   KpiInstInfo  kpiInfo1; /* Named with suffix1, so that later if different 
                             counters are incremented in the same function which
                             require different information to be passed to OAM,
                             new variable can be filled */
   TRC2(wrIncKpiFrOutX2BsdHovrSuccd);

   /*  Fill KPI info required for this KPI */  
   wrFillKpiInfoeCgi(&kpiInfo1,  info);

   switch(cztCause->causeTyp)
   {
      case WR_CAUSE_RADIONW:
         switch(cztCause->causeVal)
         {
            case CztCauseRadioNwtime_critical_handoverEnum:
               {
                  WR_INC_KPI(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_SUCC_CAUSE1,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);  
                  WR_INC_KPI_INST(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_SUCC_CAUSE1_PER_TGT_CELL,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE,kpiInfo1);
                  break;
               }
            case CztCauseRadioNwhandover_desirable_for_radio_reasonsEnum:
               {
                  WR_INC_KPI(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_SUCC_CAUSE2,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);  
                  WR_INC_KPI_INST(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_SUCC_CAUSE2_PER_TGT_CELL,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE,kpiInfo1);
                  break;
               }
            case CztCauseRadioNwresource_optimisation_handoverEnum:
               {
                  WR_INC_KPI(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_SUCC_CAUSE3,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);  
                  WR_INC_KPI_INST(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_SUCC_CAUSE3_PER_TGT_CELL,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE,kpiInfo1);
                  break;
               }
            case CztCauseRadioNwreduce_load_in_serving_cellEnum:
               {
                  WR_INC_KPI(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_SUCC_CAUSE4,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);  
                  WR_INC_KPI_INST(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_SUCC_CAUSE4_PER_TGT_CELL,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE,kpiInfo1);
                  break;
               } 
            default:
               RLOG0(L_ERROR,"Invalid Radio cause");
               RETVALUE(RFAILED);
         }
         break;
      default:
         RLOG0(L_ERROR,"Invalid cause catagory");
         RETVALUE(RFAILED);
   }
   /* Increment Sum KPI counter */
   WR_INC_KPI(KPI_ID_LTE_HO_TOENBSUCC_SUM,
         KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);  
   WR_INC_KPI_INST(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_SUCC_SUM_PER_TGT_CELL,
         KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE,kpiInfo1);
   RETVALUE(ROK);
} /* end of wrIncKpiFrOutX2BsdHovrSuccd */

/** @brief This function increments KPI for Intra LTE HO specific to Outgoing S1
 * Based (source FAP) per cause
 *          
 * @details Attempted S1 outgoing inter-eNB handover executions per handover cause
 *          and sum.
 *          
 * Function: wrIncKpiFrOutS1BsdLTEHovAttmptd
 *          
 * @param[out]            
 * @param[in] sztCause, info
 * @return  S16
 * -# Success : ROK
 */
PUBLIC S16 wrIncKpiFrOutS1BsdLTEHovAttmptd
(
 SztCause      *sztCause,
 WrEcgi        *info
)
{

   KpiInstInfo  kpiInfo1; /* Named with suffix1, so that later if different 
                             counters are incremented in the same function which
                             require different information to be passed to OAM,
                             new variable can be filled */
   TRC2(wrIncKpiFrOutS1BsdLTEHovAttmptd);

   /*  Fill KPI info required for this KPI */  
   wrFillKpiInfoeCgi(&kpiInfo1, (WrEcgi *) info);
   switch(sztCause->choice.val)
   {
      case WR_CAUSE_RADIONW:
         switch(sztCause->val.radioNw.val)
         {
            case SztCauseRadioNwtime_critical_handoverEnum:
               {
                  WR_INC_KPI(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_ATT_CAUSE1,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);   
                  WR_INC_KPI_INST(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_ATT_CAUSE1_PER_TGT_CELL,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE,kpiInfo1);
                  break;
               }
            case SztCauseRadioNwhandover_desirable_for_radio_reasonEnum:
               {
                  WR_INC_KPI(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_ATT_CAUSE2,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);   
                  WR_INC_KPI_INST(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_ATT_CAUSE2_PER_TGT_CELL,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE,kpiInfo1);
                  break;
               }
            case SztCauseRadioNwresource_optimisation_handoverEnum:
               {
                  WR_INC_KPI(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_ATT_CAUSE3,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);   
                  WR_INC_KPI_INST(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_ATT_CAUSE3_PER_TGT_CELL,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE,kpiInfo1);
                  break;
               }
            case SztCauseRadioNwreduce_load_in_serving_cellEnum:
               {
                  WR_INC_KPI(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_ATT_CAUSE4,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);   
                  WR_INC_KPI_INST(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_ATT_CAUSE4_PER_TGT_CELL,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE,kpiInfo1);
                  break;
               }
            default:
               RLOG0(L_ERROR,"Invalid Radio cause");
               RETVALUE(RFAILED);
         }
         break;
      default:
         RLOG0(L_ERROR,"Invalid cause catagory.");
         RETVALUE(RFAILED);
   }
   /* incrementing Out going HO KPI */
   WR_INC_KPI(KPI_ID_LTE_HO_TOENBATT_SUM,
         KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);   
   WR_INC_KPI_INST(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_ATT_SUM_PER_TGT_CELL,
         KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE,kpiInfo1);
   RETVALUE(ROK);
} /* end of wrIncKpiFrOutS1BsdLTEHovAttmptd */

/** @brief This function increments KPI for Intra LTE HO specific to Outgoing
 * S1 Based (source FAP) per cause
 *          
 * @details This function increments KPI for Intra LTE HO specific to Outgoing
 * S1 Based (source FAP) per cause
 *          
 *          
 *     Function: wrIncKpiFrS1BsdLTEtoUtrnHoAtmpt
 *          
 * @param[out]            
 * @param[in]          cause: cause type and cause value. 
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16  wrIncKpiFrS1BsdLTEtoUtrnHoAtmpt 
(
   SztCause      *sztCause
)
{
   TRC2(wrIncKpiFrS1BsdLTEtoUtrnHoAtmpt);

   /* Switch statement to select the CAUSE TYPE */
   switch(sztCause->choice.val)
   {
      case WR_CAUSE_RADIONW:
         {
            /* Switch statement to select the CAUSE VALUE */
            switch(sztCause->val.radioNw.val)
            {
               case SztCauseRadioNwtime_critical_handoverEnum:
                  {
                     WR_INC_KPI(KPI_ID_LTE_OUT_INTER_RAT_UTRA_HO_EXE_ATT_CAUSE1,
                           KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);                  
                     break;
                  }
               case SztCauseRadioNwhandover_desirable_for_radio_reasonEnum:
                  {
                     WR_INC_KPI(KPI_ID_LTE_OUT_INTER_RAT_UTRA_HO_EXE_ATT_CAUSE2,
                           KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);                  
                     break;
                  }
               case SztCauseRadioNwresource_optimisation_handoverEnum:
                  {
                     WR_INC_KPI(KPI_ID_LTE_OUT_INTER_RAT_UTRA_HO_EXE_ATT_CAUSE3,
                           KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);   
                     break;
                  }
               case SztCauseRadioNwreduce_load_in_serving_cellEnum:
                  {
                     WR_INC_KPI(KPI_ID_LTE_OUT_INTER_RAT_UTRA_HO_EXE_ATT_CAUSE4,
                           KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);   
                     break;
                  }
               default :
                  {
                     RLOG0(L_ERROR, "wrIncKpiFrS1BsdLTEtoUtrnHoAtmpt:\
                           Invalid Radio cuase"); 
                     RETVALUE(RFAILED); 
                  }
            }
         }
         break;
      case WR_CAUSE_TRANSPORT:
         break;
      case WR_CAUSE_NAS:
         break;
      case WR_CAUSE_PROTOCOL:
         break;
      case WR_CAUSE_MISC:
         break;
      default :
         {
            RLOG0(L_ERROR, "wrIncKpiFrS1BsdLTEtoUtrnHoAtmpt:Invalid\
                     cause category"); 
            RETVALUE(RFAILED);
         }
   }

   /*Increment KPI by 1*/
   WR_INC_KPI(KPI_ID_LTE_HO_TOUTRANATT_TOUTRANCELL_SUM,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);
   WR_INC_KPI(KPI_ID_LTE_INTER_RAT_UTRA_ATT_SUM,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);

   RETVALUE(ROK);
} /* end of wrIncKpiFrS1BsdLTEtoUtrnHoAtmpt */

/** @brief This function increments KPI for Intra LTE HO specific to Outgoing
 * S1 Based (source FAP) per cause
 *          
 * @details This function increments KPI for Intra LTE HO specific to Outgoing
 * S1 Based (source FAP) per cause
 *          
 *          
 *     Function: wrIncKpiFrS1BsdLTEtoGernHoAtmpt
 *          
 * @param[out]            
 * @param[in]          cause: cause type and cause value. 
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 wrIncKpiFrS1BsdLTEtoGernHoAtmpt 
(
   SztCause      *sztCause
)
{
   TRC2(wrIncKpiFrS1BsdLTEtoGernHoAtmpt);

   /*switch case shall use for updating HO KPI per cause*/
   /*Increment KPI by 1*/
   WR_INC_KPI(KPI_ID_LTE_HO_TOGERANATT_TOGERANCELL_SUM,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);   
   RETVALUE(ROK);
} /* end of wrIncKpiFrS1BsdLTEtoGernHoAtmpt */

/** @brief This function increments KPI for Intra LTE HO specific to Outgoing
 * S1 Based (source FAP) per cause
 *          
 * @details Sucessful S1 outgoing inter-eNB handover executions per handover cause
 *  *          and sum.
 *
 *     Function: wrIncKpiFrOutS1BsdLTEHovSuccd
 *          
 * @param[out]            
 * @param[in]   sztCause, info
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 wrIncKpiFrOutS1BsdLTEHovSuccd
(
   SztCause     *sztCause,
   WrEcgi       *info
)
{
   KpiInstInfo  kpiInfo1; /* Named with suffix1, so that later if different 
                             counters are incremented in the same function which
                             require different information to be passed to OAM,
                             new variable can be filled */
   TRC2(wrIncKpiFrOutS1BsdLTEHovSuccd);

   /*  Fill KPI info required for this KPI */  
   wrFillKpiInfoeCgi(&kpiInfo1, info);
   switch(sztCause->choice.val)
   {
      case WR_CAUSE_RADIONW:
         switch(sztCause->val.radioNw.val)
         {
            case SztCauseRadioNwtime_critical_handoverEnum:
               {
                  WR_INC_KPI(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_SUCC_CAUSE1,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);  
                  WR_INC_KPI_INST(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_SUCC_CAUSE1_PER_TGT_CELL,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE,kpiInfo1);
                  break;
               }
            case SztCauseRadioNwhandover_desirable_for_radio_reasonEnum:
               {
                  WR_INC_KPI(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_SUCC_CAUSE2,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);  
                  WR_INC_KPI_INST(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_SUCC_CAUSE2_PER_TGT_CELL,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE,kpiInfo1);
                  break;
               }
            case SztCauseRadioNwresource_optimisation_handoverEnum:
               {
                  WR_INC_KPI(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_SUCC_CAUSE3,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);  
                  WR_INC_KPI_INST(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_SUCC_CAUSE3_PER_TGT_CELL,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE,kpiInfo1);
                  break;
               }
            case SztCauseRadioNwreduce_load_in_serving_cellEnum:
               {
                  WR_INC_KPI(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_SUCC_CAUSE4,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);  
                  WR_INC_KPI_INST(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_SUCC_CAUSE4_PER_TGT_CELL,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE,kpiInfo1);
                  break;
               } 
            default:
               RLOG0(L_ERROR, " Invalid Radio cause ");
               RETVALUE(RFAILED);
         }
         break;
      default:
         RLOG0(L_ERROR,"Invalid cause catagory");
         RETVALUE(RFAILED);
   }
   /* Incrementing Sum KPI counters */
   WR_INC_KPI(KPI_ID_LTE_HO_TOENBSUCC_SUM, KPI_VALUE_TYPE_INTEGER, 
         INC_KPI_VALUE_BY_ONE);   
   WR_INC_KPI(KPI_ID_LTE_HO_S1_BASED_HANDOUT_SUCC_SUM,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);   
   WR_INC_KPI_INST(KPI_ID_LTE_OUT_INTER_ENB_HO_EXE_SUCC_SUM_PER_TGT_CELL,
         KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE,kpiInfo1);
   RETVALUE(ROK);
} /* end of wrIncKpiFrOutS1BsdLTEHovSuccd */

/** @brief This function increments KPI for Intra LTE HO specific to Outgoing
 * S1 Based (source FAP) per cause
 *          
 * @details This function increments KPI for Intra LTE HO specific to Outgoing
 * S1 Based (source FAP) per cause
 *          
 *          
 *     Function: wrIncKpiFrS1BsdLTEtoUtrnHoSuccd
 *          
 * @param[out]            
 * @param[in]          cause: cause type and cause value. 
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 wrIncKpiFrS1BsdLTEtoUtrnHoSuccd 
(
   SztCause      *sztCause
)
{
   TRC2(wrIncKpiFrS1BsdLTEtoUtrnHoSuccd);

   /* Switch statement to select CAUSE TYPE */
   switch(sztCause->choice.val)
   {
      case WR_CAUSE_RADIONW:
         {
            switch(sztCause->val.radioNw.val)
            {
               case SztCauseRadioNwtime_critical_handoverEnum:
                  {
                     WR_INC_KPI(KPI_ID_LTE_OUT_INTER_RAT_UTRA_HO_EXE_SUCC_CAUSE1,
                           KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);  
                     break;
                  }
               case  SztCauseRadioNwhandover_desirable_for_radio_reasonEnum:
                  {
                     WR_INC_KPI(KPI_ID_LTE_OUT_INTER_RAT_UTRA_HO_EXE_SUCC_CAUSE2,
                           KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);  
                     break;
                  }
               case SztCauseRadioNwresource_optimisation_handoverEnum:
                  {
                     WR_INC_KPI(KPI_ID_LTE_OUT_INTER_RAT_UTRA_HO_EXE_SUCC_CAUSE3,
                           KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);  
                     break;
                  }
               case SztCauseRadioNwreduce_load_in_serving_cellEnum:
                  {
                     WR_INC_KPI(KPI_ID_LTE_OUT_INTER_RAT_UTRA_HO_EXE_SUCC_CAUSE4,
                           KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);  
                     break;
                  }
               default :
                  {
                     RLOG0(L_ERROR, "wrIncKpiFrS1BsdLTEtoUtrnHoSuccd:\
                              Invalid Radio cuase"); 
                     RETVALUE(RFAILED); 
                  }
            }
            break;
         }
      case WR_CAUSE_TRANSPORT:
         break;
      case WR_CAUSE_NAS:
         break;
      case WR_CAUSE_PROTOCOL:
         break;
      case WR_CAUSE_MISC:
         break;
      default :
         {
            RLOG0(L_ERROR, "wrIncKpiFrS1BsdLTEtoUtrnHoSuccd:Invalid\
                     cause category"); 
            RETVALUE(RFAILED);
         }
   }
   /*Increment KPI by 1*/
   WR_INC_KPI(KPI_ID_LTE_HO_TOUTRANSUCC_TOUTRANCELL_SUM,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);  
   WR_INC_KPI(KPI_ID_LTE_INTER_RAT_UTRA_SUCC_SUM,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);  
   RETVALUE(ROK);
} /* end of wrIncKpiFrS1BsdLTEtoUtrnHoSuccd */

/** @brief This function increments KPI for Intra LTE HO specific to Outgoing
 * S1 Based (source FAP) per cause
 *          
 * @details This function increments KPI for Intra LTE HO specific to Outgoing
 * S1 Based (source FAP) per cause
 *          
 *          
 *     Function: wrIncKpiFrS1BsdLTEtoGernHoSuccd
 *          
 * @param[out]            
 * @param[in]          cause: cause type and cause value. 
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 wrIncKpiFrS1BsdLTEtoGernHoSuccd 
(
   SztCause      *sztCause
)
{
   TRC2(wrIncKpiFrS1BsdLTEtoGernHoSuccd);

   /*switch case shall use for updating HO KPI per cause,currently updating SUM HO KPI*/
   /*Increment KPI by 1*/
   WR_INC_KPI(KPI_ID_LTE_HO_TOGERANSUCC_TOGERANCELL_SUM,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);   
   RETVALUE(ROK);
} /* end of wrIncKpiFrS1BsdLTEtoGernHoSuccd */

/** @brief This function increments KPI for Intra LTE HO specific to Outgoing
 * S1 Based (source FAP) per cause
 *          
 * @details This function increments KPI for Intra LTE HO specific to Outgoing
 * S1 Based (source FAP) per cause
 *          
 *          
 *     Function: wrIncKpiFrOutS1BsdLTEHovFaild
 *          
 * @param[out]            
 * @param[in]          cause: cause type and cause value. 
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 wrIncKpiFrOutS1BsdLTEHovFaild
(
   SztCause      *sztCause
)
{
   TRC2(wrIncKpiFrOutS1BsdLTEHovFaild);

   /*switch case shall use for updating Ho KPI per cause,currently updating SUM HO KPI*/
   /*Increment KPI by 1*/
   WR_INC_KPI(KPI_ID_LTE_HO_TOENBFAIL_SUM,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);  
   RETVALUE(ROK);
} /* end of wrIncKpiFrOutS1BsdLTEHovFaild */

/** @brief This function increments KPI for Intra LTE HO specific to Outgoing
 * S1 Based (source FAP) per cause
 *          
 * @details This function increments KPI for Intra LTE HO specific to Outgoing
 * S1 Based (source FAP) per cause
 *          
 *          
 *     Function: wrIncKpiFrS1BsdLTEtoUtrnHoFaild
 *          
 * @param[out]            
 * @param[in]          cause: cause type and cause value. 
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 wrIncKpiFrS1BsdLTEtoUtrnHoFaild 
(
   SztCause      *sztCause
)
{
   TRC2(wrIncKpiFrS1BsdLTEtoUtrnHoFaild);

   switch(sztCause->choice.val)
   {
      case WR_CAUSE_RADIONW:
         {
            switch(sztCause->val.radioNw.val)
            {
               case SztCauseRadioNwtime_critical_handoverEnum:
                  {
                     WR_INC_KPI(KPI_ID_LTE_OUT_INTER_RAT_UTRA_HO_EXE_FAIL_CAUSE1,
                           KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);                  
                     break;
                  }
               case SztCauseRadioNwhandover_desirable_for_radio_reasonEnum:
                  {
                     WR_INC_KPI(KPI_ID_LTE_OUT_INTER_RAT_UTRA_HO_EXE_FAIL_CAUSE2,
                           KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);                  
                     break;
                  }
               case SztCauseRadioNwresource_optimisation_handoverEnum:
                  {
                     WR_INC_KPI(KPI_ID_LTE_OUT_INTER_RAT_UTRA_HO_EXE_FAIL_CAUSE3,
                           KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);                  
                     break;
                  }
               case SztCauseRadioNwreduce_load_in_serving_cellEnum:
                  {
                     WR_INC_KPI(KPI_ID_LTE_OUT_INTER_RAT_UTRA_HO_EXE_FAIL_CAUSE4,
                           KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);                  
                     break; 
                  }
               default:
                  {
                     RLOG0(L_ERROR, "wrIncKpiFrS1BsdLTEtoUtrnHoFaild:Invalid\
                              Radio cause"); 
                     RETVALUE(RFAILED); 
                  }
            }
            break;
         }
      case WR_CAUSE_TRANSPORT:
         break;
      case WR_CAUSE_NAS:
         break;
      case WR_CAUSE_PROTOCOL:
         break;
      case WR_CAUSE_MISC:
         break;
      default :
         {
            RLOG0(L_ERROR, "wrIncKpiFrS1BsdLTEtoUtrnHoFaild:Invalid\
                     cause category"); 
            RETVALUE(RFAILED); 
         }
   }   

   /*Increment KPI by 1*/
   WR_INC_KPI(KPI_ID_LTE_HO_TOUTRANFAIL_TOUTRANCELL_SUM,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);   
   WR_INC_KPI(KPI_ID_LTE_INTER_RAT_UTRA_FAIL_SUM,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);   
   RETVALUE(ROK);
} /* end of wrIncKpiFrS1BsdLTEtoUtrnHoFaild */

/** @brief This function increments KPI for Intra LTE HO specific to Outgoing
 * S1 Based (source FAP) per cause
 *          
 * @details This function increments KPI for Intra LTE HO specific to Outgoing
 * S1 Based (source FAP) per cause
 *          
 *          
 *     Function: wrIncKpiFrS1BsdLTEtoGernHoFaild
 *          
 * @param[out]            
 * @param[in]          cause: cause type and cause value. 
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 wrIncKpiFrS1BsdLTEtoGernHoFaild 
(
   SztCause      *sztCause
)
{
   TRC2(wrIncKpiFrS1BsdLTEtoGernHoFaild);

   /* switch case shall use for updating Ho KPI per cause,currently updating SUM
    * HO KPI*/
   /*Increment KPI by 1*/
   WR_INC_KPI(KPI_ID_LTE_HO_TOGERANFAIL_TOGERANCELL_SUM,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);   
   RETVALUE(ROK);
} /* end of wrIncKpiFrS1BsdLTEtoGernHoFaild */

/** @brief This function increments KPI for Intra LTE HO specific to outgoing
 * X2 Based(source FAP) per cause
 *          
 * @details This function increments KPI for Intra LTE HO specific to outgoing
 * X2 Based(source FAP) per cause
 *          
 *          
 *     Function: wrIncKpiFrOutX2BsdLTEHovFaild
 *          
 * @param[out]            
 * @param[in]          cause: cause type and cause value. 
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 wrIncKpiFrOutX2BsdLTEHovFaild
(
   CztCause                  *cztCause
)
{
   TRC2(wrIncKpiFrOutX2BsdLTEHovFaild);

   /*switch case shall use for updating Ho KPI per cause,currently updating SUM HO KPI*/
   /*Increment KPI by 1*/
   WR_INC_KPI(KPI_ID_LTE_HO_TOENBFAIL_SUM,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);   
   RETVALUE(ROK);
} /* end of wrIncKpiFrOutX2BsdLTEHovFaild */

/** @brief This function increments KPI for Intra LTE HO specific to Incoming
 * S1 Based (target FAP) per cause
 *          
 * @details This function increments KPI for Intra LTE HO specific to Incoming
 * S1 Based (target FAP) per cause
 *          
 *          
 *     Function: wrIncKpiFrIncS1BsdLTEHovAttmptd
 *          
 * @param[out]            
 * @param[in]          cause: cause type and cause value. 
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 wrIncKpiFrIncS1BsdLTEHovAttmptd
(
   SztCause            *sztCause
)
{
   TRC2(wrIncKpiFrIncS1BsdLTEHovAttmptd);

   WR_INC_KPI(KPI_ID_LTE_HO_FROMENBATT_SUM,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);
   RETVALUE(ROK);
} /* end of wrIncKpiFrIncS1BsdLTEHovAttmptd */

/** @brief This function increments KPI for Intra LTE HO specific to Incoming
 * X2 Based (target FAP) per cause
 *          
 * @details This function increments KPI for Intra LTE HO specific to Incoming
 * X2 Based (target FAP) per cause
 *          
 *          
 * Function: wrIncKpiFrIncX2BsdLTEHovAttmptd
 *          
 * @param[out]            
 * @param[in]          cause: cause type and cause value. 
 * @return  S16
 *        -# Success : ROK
 */

PUBLIC S16 wrIncKpiFrIncX2BsdLTEHovAttmptd
(
   WrUmmMsgCause  *cztCause
)
{
   TRC2(wrIncKpiFrIncX2BsdLTEHovAttmptd);

   /*switch case shall use for updating Ho KPI per cause,currently updating SUM HO KPI*/
   /*Increment KPI by 1*/
   WR_INC_KPI(KPI_ID_LTE_HO_FROMENBATT_SUM,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);   
   RETVALUE(ROK);
} /* end of wrIncKpiFrIncX2BsdLTEHovAttmptd */

/** @brief This function increments KPI for Intra LTE HO specific to Incoming
 * X2 Based (target FAP) per cause
 *          
 * @details This function increments KPI for Intra LTE HO specific to Incoming
 * X2 Based (target FAP) per cause
 *          
 *          
 *     Function: wrIncKpiFrIncX2BsdLTEHovSuccd
 *          
 * @param[out]            
 * @param[in]          cause: cause type and cause value. 
 * @return  S16
 *        -# Success : ROK
 */

PUBLIC S16 wrIncKpiFrIncX2BsdLTEHovSuccd
(
   CztCause *cztCause
)
{
   TRC2(wrIncKpiFrIncX2BsdLTEHovSuccd);

   /*switch case shall use for updating Ho KPI per cause,currently updating SUM HO KPI*/
   /*Increment KPI by 1*/
   WR_INC_KPI(KPI_ID_LTE_HO_FROMENBSUCC_SUM,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);   
   RETVALUE(ROK);
} /* end of wrIncKpiFrIncX2BsdLTEHovSuccd */

/** @brief This function increments KPI for Intra LTE HO specific to Incoming
 * X2 Based (target FAP) per cause
 *          
 * @details This function increments KPI for Intra LTE HO specific to Incoming
 * X2 Based (target FAP) per cause
 *          
 *          
 *     Function: wrIncKpiFrIncX2BsdLTEHovFaild
 *          
 * @param[out]            
 * @param[in]          cause: cause type and cause value. 
 * @return  S16
 *        -# Success : ROK
 */

PUBLIC S16 wrIncKpiFrIncX2BsdLTEHovFaild
(
   CztCause *cztCause
)
{
   TRC2(wrIncKpiFrIncX2BsdLTEHovFaild);

   /*switch case shall use for updating Ho KPI per cause,currently updating SUM HO KPI*/
   /*Increment KPI by 1*/
   WR_INC_KPI(KPI_ID_LTE_HO_FROMENBFAIL_SUM,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);  
   RETVALUE(ROK);
} /* end of wrIncKpiFrIncX2BsdLTEHovFaild */

/** @brief This function increments KPI for Intra LTE HO specific to Incoming
 * S1 Based (target FAP) per cause
 *          
 * @details This function increments KPI for Intra LTE HO specific to Incoming
 * S1 Based (target FAP) per cause
 *          
 *          
 *     Function: wrIncKpiFrIncS1BsdLTEHovSuccd
 *          
 * @param[out]            
 * @param[in]          cause: cause type and cause value. 
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 wrIncKpiFrIncS1BsdLTEHovSuccd
(
   SztCause *sztCause
)
{
   TRC2(wrIncKpiFrIncS1BsdLTEHovSuccd);

   /*switch case shall use for updating Ho KPI per cause,currently updating SUM HO KPI*/
   /*Increment KPI by 1*/
   WR_INC_KPI(KPI_ID_LTE_HO_FROMENBSUCC_SUM,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);
   WR_INC_KPI(KPI_ID_LTE_HO_S1_BASED_HANDIN_SUCC_SUM,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);
   RETVALUE(ROK);
} /* end of wrIncKpiFrIncS1BsdLTEHovSuccd */

/** @brief This function increments KPI for Intra LTE HO specific to Incoming
 * X2 Based (target FAP) per cause
 *          
 * @details This function increments KPI for Intra LTE HO specific to Incoming
 * X2 Based (target FAP) per cause
 *          
 *          
 *     Function: wrIncKpiFrIncS1BsdLTEHovFaild
 *          
 * @param[out]            
 * @param[in]          cause: cause type and cause value. 
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 wrIncKpiFrIncS1BsdLTEHovFaild
(
   SztCause *sztCause
)
{
   TRC2(wrIncKpiFrIncS1BsdLTEHovFaild);

   /*switch case shall use for updating Ho KPI per cause,currently updating SUM HO KPI*/
   /*Increment KPI by 1*/
   WR_INC_KPI(KPI_ID_LTE_HO_FROMENBFAIL_SUM,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);   
   RETVALUE(ROK);
} /* end of wrIncKpiFrIncS1BsdLTEHovFaild */

/** @brief This function increments KPI for Inter LTE HO Attempted
 *          
 * @details This measurement provides the number of attempted inbound/outbound
 *          handover from/to eNB per handover cause
 *          
 *          
 * Function: wrIncKpiFrHovrAttmptd
 *          
 * @param[out]            
 * @param[in]   hovInf, hovType, hoDir, *hovCausei, *kpiInfo
 * @return      S16
 * -# Success : ROK
 */

PUBLIC S16 wrIncKpiFrHovrAttmptd
(
 U8         hovInf,
 U32        hovType,
 U8         hoDir,
 Void       *hovCause,
 Void       *kpiMInfo
)
{
   SztCause       *sztCause;

   TRC2(wrIncKpiFrHovrAttmptd);

   switch(hoDir)
   {
      case INC_HOV:
         switch(hovInf)
         {
            case WR_HO_TYPE_X2:
               wrIncKpiFrIncX2BsdLTEHovAttmptd((WrUmmMsgCause *)hovCause);
               break;
            case WR_HO_TYPE_INTRA_LTE_S1:
               sztCause = (SztCause *)hovCause;
               switch(hovType)
               {
                  case SztHovrTypintralteEnum:
                     wrIncKpiFrIncS1BsdLTEHovAttmptd(sztCause);               
                     break;
                  case SztHovrTypltetoutranEnum:
                     break;
                  case SztHovrTypltetogeranEnum:
                     break;
                  default:
                     RLOG0(L_ERROR, "wrIncKpiFrHovrAttmptd:Invalid\
                              Szt Hov cause");
                     RETVALUE(RFAILED);
               }
               break;
            default:
               RLOG0(L_ERROR, "wrIncKpiFrHovrSuccd: Invalid Hov\
                        type");
               RETVALUE(RFAILED);
         }
         break;
      case OUT_HOV:
         switch(hovInf)
         {

            case WR_HO_TYPE_X2:
               /* Increment KPI for X2 Handovers Attempted outgoing Handover to
                * eNodeB per Handover cause */
               wrIncKpiFrOutX2BsdHovrAttmptd((WrUmmMsgCause *)hovCause, (WrEcgi *) kpiMInfo);
               break;
            /* Same cause values have been used for Intra LTE and Inter RAT HO 
             * Hence adding fall through case */
            case WR_HO_TYPE_INTRA_LTE_S1:
            case WR_HO_TYPE_IRAT_UTRA:
               sztCause = (SztCause *)hovCause;
               switch(hovType)
               {
                  case SztHovrTypintralteEnum:
                     /*Increment KPI for Intra-LTE Handovers Attempted
                      * outgoing Handover to eNodeB per Handover cause*/
                     wrIncKpiFrOutS1BsdLTEHovAttmptd(sztCause,(WrEcgi *) kpiMInfo);
                     break;
                  case SztHovrTypltetoutranEnum:
                     /*Increment KPI for Inter-RAT Handovers Attempted
                      * outgoing Handovers to UTRAN per Handover cause*/
                     wrIncKpiFrS1BsdLTEtoUtrnHoAtmpt(sztCause);
                     break;
                  case SztHovrTypltetogeranEnum:
                     wrIncKpiFrS1BsdLTEtoGernHoAtmpt(sztCause);
                     break;
                  default:
                     RLOG0(L_ERROR, "wrIncKpiFrHovrAttmptd:Invalid\
                              Szt Hov cause");
                     RETVALUE(RFAILED);
               }
               break;
            default:
               RLOG0(L_ERROR, "wrIncKpiFrHovrAttmptd: Invalid\
                        Handover Type");
               RETVALUE(RFAILED);
         }
         break;
      default:
         RLOG0(L_ERROR, "wrIncKpiFrHovrAttmptd:Invalid Handover\
                  direction");
         RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of wrIncKpiFrHovrAttmptd */

/** @brief This function increments KPI for Inter LTE HO Success
 *          
 * @details This measurement provides the number of Successful inbound/outbound
 *          handover from/to eNB per handover cause
 *          
 *     Function: wrIncKpiFrHovrSuccd
 *          
 * @param[out]            
 * @param[in]  hovInf, hovType, hoDir, hovCause, kpiMInfo
 * @return  S16
 *        -# Success : ROK
 */

PUBLIC S16 wrIncKpiFrHovrSuccd
(
 U8       hovInf,
 U32      hovType,
 U8       hoDir,
 Void     *hovCause,
 Void     *kpiMInfo
)
{
   CztCause       *cztCause;
   SztCause       *sztCause;

   TRC2(wrIncKpiFrHovrSuccd);

   switch(hoDir)
   {
      case INC_HOV:
         switch(hovInf)
         {
            case WR_HO_TYPE_X2:
               cztCause = (CztCause *)hovCause;
               wrIncKpiFrIncX2BsdLTEHovSuccd(cztCause); 
               break;
            case WR_HO_TYPE_INTRA_LTE_S1:
               sztCause = (SztCause *)hovCause;
               switch(hovType)
               {
                  case SztHovrTypintralteEnum:
                     wrIncKpiFrIncS1BsdLTEHovSuccd(sztCause);
                     break;
                  case SztHovrTypltetoutranEnum:
                     break;
                  case SztHovrTypltetogeranEnum:
                     break;
                  default:
                     RLOG0(L_ERROR, "wrIncKpiFrHovrSuccd: Invalid\
                              szt hov cause");
                     RETVALUE(RFAILED);
               }
               break;
            default:
               RLOG0(L_ERROR, "wrIncKpiFrHovrSuccd: Invalid Hov\
                        type");
               RETVALUE(RFAILED);
         }
         break;
      case OUT_HOV:
         switch(hovInf)
         {
            case WR_HO_TYPE_X2:
               /* Increment KPI for X2 Handovers Successful outgoing Handover to
                * eNodeB per Handover cause*/
               wrIncKpiFrOutX2BsdHovrSuccd((WrUmmMsgCause *) hovCause, (WrEcgi *) kpiMInfo);
               break;
            /* Same cause values have been used for Intra LTE and Inter RAT HO 
             * Hence adding fall through case */
            case WR_HO_TYPE_INTRA_LTE_S1:
            case WR_HO_TYPE_IRAT_UTRA:
               sztCause = (SztCause *)hovCause;
               switch(hovType)
               {
                  case SztHovrTypintralteEnum:
                     /* Increment KPI for Intra-LTE Handovers Successful 
                      * outgoing Handover to eNodeB per Handover cause*/
                     wrIncKpiFrOutS1BsdLTEHovSuccd(sztCause, (WrEcgi *) kpiMInfo);
                     break;
                  case SztHovrTypltetoutranEnum:
                     /*Increment KPI for Inter-RAT Handovers Successful 
                      * outgoing Handovers to UTRAN per Handover cause*/
                     wrIncKpiFrS1BsdLTEtoUtrnHoSuccd(sztCause);
                     break;
                  case SztHovrTypltetogeranEnum:
                     wrIncKpiFrS1BsdLTEtoGernHoSuccd(sztCause);
                     break;
                  default:
                     RLOG0(L_ERROR, "wrIncKpiFrHovrSuccd: Invalid\
                              szt hov cause");
                     RETVALUE(RFAILED);
               }
               break;
            default:
               RLOG0(L_ERROR, "wrIncKpiFrHovrSuccd: Invalid Hov\
                        type");
               RETVALUE(RFAILED);
         }
         break;
      default:
         RLOG0(L_ERROR, "wrIncKpiFrHovrSuccd: Invalid Handover\
                  direction"); 
         RETVALUE(ROK);  
   }

   RETVALUE(ROK);
} /* end of wrIncKpiFrHovrSuccd */

/** @brief This function increments KPI for Inter LTE HO Failed
 *          
 * @details This function increments KPI for Inter LTE HO Failed
 *          
 *          
 *     Function: wrIncKpiFrHovrFaild
 *          
 * @param[out]            
 * @param[in]          HovrType,sztCause 
 * @return  S16
 *        -# Success : ROK
 */

PUBLIC S16 wrIncKpiFrHovrFaild
(
U8 hovType,
U32 hov,
U8 dir,
Void *hovCause
 )
{
   CztCause *cztCause;
   SztCause *sztCause;

   TRC2(wrIncKpiFrHovrFaild);

   switch(dir)
   {
      case INC_HOV:
         switch(hovType)
         {
            case WR_HO_TYPE_X2:
               cztCause = (CztCause *)hovCause;
               wrIncKpiFrIncX2BsdLTEHovFaild(cztCause); 
               break;
            case WR_HO_TYPE_INTRA_LTE_S1:
               sztCause = (SztCause *)hovCause;
               switch(hov)
               {
                  case SztHovrTypintralteEnum:
                     wrIncKpiFrIncS1BsdLTEHovFaild(sztCause);   
                     break;
                  case SztHovrTypltetoutranEnum:
                     break;
                  case SztHovrTypltetogeranEnum:
                     break;
                  default:
                     RLOG0(L_ERROR,"wrIncKpiFrHovrFaild:Invalid\
                              Szt Hov cause");
                     RETVALUE(RFAILED);
               }
               break;
            default:
               RLOG0(L_ERROR, "wrIncKpiFrHovrFaild:Invalid Handover\
                        Type"); 
               RETVALUE(RFAILED);
         }
         break;
      case OUT_HOV:
         switch(hovType)
         {

            case WR_HO_TYPE_X2:
               cztCause  = (CztCause *)hovCause;
               wrIncKpiFrOutX2BsdLTEHovFaild(cztCause);
               break;
            /* Same cause values have been used for Intra LTE and Inter RAT HO 
             * Hence adding fall through case */
            case WR_HO_TYPE_INTRA_LTE_S1:
            case WR_HO_TYPE_IRAT_UTRA:
               sztCause = (SztCause *)hovCause;
               switch(hov)
               {
                  case SztHovrTypintralteEnum:
                     /*Increment KPI for Intra-LTE Handovers Attempted outgoing
                      * Handover to eNodeB per Handover cause*/
                     wrIncKpiFrOutS1BsdLTEHovFaild(sztCause);
                     break;
                  case SztHovrTypltetoutranEnum:
                     /*Increment KPI for Inter-RAT Handovers Attempted outgoing
                      * Handovers to UTRAN per Handover cause*/
                     wrIncKpiFrS1BsdLTEtoUtrnHoFaild(sztCause);
                     break;
                  case SztHovrTypltetogeranEnum:
                     wrIncKpiFrS1BsdLTEtoGernHoFaild(sztCause); 
                     break;
                  default:
                     RLOG0(L_ERROR, "wrIncKpiFrHovrFaild:Invalid\
                              Szt Hov cause");
                     RETVALUE(RFAILED);
               }
               break;
            default:
               RLOG0(L_ERROR, "wrIncKpiFrHovrFaild:Invalid Hov\
                        type");
               RETVALUE(RFAILED);
         }
         break;
      default:
         RLOG0(L_ERROR, "wrIncKpiFrHovrFaild:Invalid Handover\
                  direction"); 
         RETVALUE(RFAILED);
   }


   RETVALUE(ROK);
} /* end of wrIncKpiFrHovrFaild */
/** @brief This function increments KPI for SRVCC Attempted
 *          
 * @details This measurement provides the number of attempted inbound/outbound
 *          SRVCC
 *          
 *          
 * Function: wrIncKpiFrSrvccAttmptd
 *          
 * @param[out]            
 * @param[in]   hovType, hoDir, *hovCause
 * @return      S16
 * -# Success : ROK
 */

PUBLIC S16 wrIncKpiFrSrvccAttmptd
(
 U32        hovType,
 U8         hoDir,
 Void       *hovCause
)
{

   TRC2(wrIncKpiFrSrvccAttmptd);

   switch(hoDir)
   {
      case INC_HOV:
        break;
      case OUT_HOV:
         switch(hovType)
         {
            case SztHovrTypltetoutranEnum:
              WR_INC_KPI(KPI_ID_LTE_OUT_INTER_RAT_UTRA_SRVCC_ATT_SUM,
                    KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);  
               break;
            case SztHovrTypltetogeranEnum:
              WR_INC_KPI(KPI_ID_LTE_OUT_INTER_RAT_GERAN_SRVCC_ATT_SUM,
                    KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);  
               break;
            default:
               RLOG0(L_ERROR, "wrIncKpiFrSrvccAttmptd:Invalid\
                     Szt HO Type");
         }
         break;
      default:
         RLOG0(L_ERROR, "wrIncKpiFrSrvccAttmptd: Invalid\
                        Handover  Dir");
         break;
   }
   RETVALUE(ROK);
} /* end of wrIncKpiFrSrvccAttmptd */
/** @brief This function increments KPI for SRVCC Success
 *          
 * @details This measurement provides the number of successful inbound/outbound
 *          SRVCC
 *          
 *          
 * Function: wrIncKpiFrSrvccSucc
 *          
 * @param[out]            
 * @param[in]   hovType, hoDir, *hovCause
 * @return      S16
 * -# Success : ROK
 */

PUBLIC S16 wrIncKpiFrSrvccSucc
(
 U32        hovType,
 U8         hoDir,
 Void       *hovCause
)
{

   TRC2(wrIncKpiFrSrvccSucc);

   switch(hoDir)
   {
      case INC_HOV:
        break;
      case OUT_HOV:
         switch(hovType)
         {
            case SztHovrTypltetoutranEnum:
               WR_INC_KPI(KPI_ID_LTE_OUT_INTER_RAT_UTRA_SRVCC_SUCC_SUM,
                    KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);  
               break;
            case SztHovrTypltetogeranEnum:
               WR_INC_KPI(KPI_ID_LTE_OUT_INTER_RAT_GERAN_SRVCC_SUCC_SUM,
                    KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);  
               break;
            default:
               RLOG0(L_ERROR, "wrIncKpiFrSrvccAttmptd:Invalid\
                     Szt HO Type");
         }
         break;
      default:
         RLOG0(L_ERROR, "wrIncKpiFrSrvccAttmptd: Invalid\
                        Handover  Dir");
         break;
   }
   RETVALUE(ROK);
} /* end of wrIncKpiFrSrvccSucc */
/** @brief This function increments KPI for SRVCC Failure
 *          
 * @details This measurement provides the number of failed inbound/outbound
 *          SRVCC
 *          
 *          
 * Function: wrIncKpiFrSrvccFail
 *          
 * @param[out]            
 * @param[in]   hovType, hoDir, *hovCause
 * @return      S16
 * -# Success : ROK
 */

PUBLIC S16 wrIncKpiFrSrvccFail
(
 U32        hovType,
 U8         hoDir,
 Void       *hovCause
)
{

   TRC2(wrIncKpiFrSrvccFail);

   switch(hoDir)
   {
      case INC_HOV:
        break;
      case OUT_HOV:
         switch(hovType)
         {
            case SztHovrTypltetoutranEnum:
               WR_INC_KPI(KPI_ID_LTE_OUT_INTER_RAT_UTRA_SRVCC_FAIL_SUM,
                    KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);  
               break;
            case SztHovrTypltetogeranEnum:
               WR_INC_KPI(KPI_ID_LTE_OUT_INTER_RAT_GERAN_SRVCC_FAIL_SUM,
                    KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);  
               break;
            default:
               RLOG0(L_ERROR, "wrIncKpiFrSrvccFail:Invalid\
                     Szt HO Type");
         }
         break;
      default:
         RLOG0(L_ERROR, "wrIncKpiFrSrvccFail: Invalid\
                        Handover  Dir");
         break;
   }
   RETVALUE(ROK);
} /* end of wrIncKpiFrSrvccFail */

/** @brief This function increments KPI for Attempted outgoing inter-eNB
 * handover preparations
 *          
 * @details This function increments KPI for Attempted outgoing inter-eNB
 * handover preparations
 *          
 *          
 *     Function: wrIncKpiFrOutIntrEnbHoPrpAtmpt
 *          
 * @param[out]            
 * @param[in]          none 
 * @return  S16
 *        -# Success : ROK
 */
/*  */
PUBLIC S16 wrIncKpiFrOutIntrEnbHoPrpAtmpt 
(
Void
)
{
   TRC2(wrIncKpiFrOutIntrEnbHoPrpAtmpt);

   /*Increment KPI by 1*/
   WR_INC_KPI(KPI_ID_LTE_HO_INTERENBOUTPREPATT,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);
   RETVALUE(ROK);
} /* end of wrIncKpiFrOutIntrEnbHoPrpAtmpt */

/* @brief This function increments KPI for Attempted outgoing intra-frequency
 * handovers
 *          
 * @details This function increments KPI for Attempted outgoing intra-frequency
 * handovers
 *          
 *          
 *     Function:wrIncKpiFrOutHoAtmptBsdOnFrqTyp
 *          
 * @param[out]            
 * @param[in]          none 
 * @return  S16
 *        -# Success : ROK
 */
/*  */
PUBLIC S16 wrIncKpiFrOutHoAtmptBsdOnFrqTyp 
(
U8 freqTyp,
Bool measGapUsd
)
{
   TRC2(wrIncKpiFrOutHoAtmptBsdOnFrqTyp);

   switch(freqTyp)
   {
      case INTRA_FREQ:
         wrIncKpiFrOutIntraFrqHoAttmptd(); 
         break;
      case INTER_FREQ:
         wrIncKpiFrInterFrqHOGpAsstAtmpt(measGapUsd);
         break;
      default:
         RLOG0(L_ERROR, "wrIncKpiFrOutHoAtmptBsdOnFrqTyp:Invalid\
                  frequency type"); 
         RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of wrIncKpiFrOutHoAtmptBsdOnFrqTyp */

/** @brief This function increments KPI for Attempted outgoing intra-frequency
 * handovers
 *          
 * @details This function increments KPI for Attempted outgoing intra-frequency
 * handovers
 *          
 *          
 *     Function: wrIncKpiFrOutIntraFrqHoAttmptd
 *          
 * @param[out]            
 * @param[in]          none 
 * @return  S16
 *        -# Success : ROK
 */
/*  */
PUBLIC S16 wrIncKpiFrOutIntraFrqHoAttmptd 
(
Void
)
{
   TRC2(wrIncKpiFrOutIntraFrqHoAttmptd);

   /*Increment KPI by 1*/
   WR_INC_KPI(KPI_ID_LTE_HO_INTRAFREQOUTATT,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);
   RETVALUE(ROK);
} /* end of wrIncKpiFrOutIntraFrqHoAttmptd */

/* @brief This function increments KPI for Attempted outgoing intra-frequency
 * handovers
 *          
 * @details This function increments KPI for Attempted outgoing intra-frequency
 * handovers
 *          
 *          
 *     Function: wrIncKpiFrOutHoSuccdBsdOnFrqTyp 
 *          
 * @param[out]            
 * @param[in]          none 
 * @return  S16
 *        -# Success : ROK
 */
/*  */
PUBLIC S16 wrIncKpiFrOutHoSuccdBsdOnFrqTyp
(
U8 freqTyp,
Bool measGapUsd
)
{
   TRC2(wrIncKpiFrOutHoSuccdBsdOnFrqTyp);

   switch(freqTyp)
   {
      case INTRA_FREQ:
         wrIncKpiFrOutIntraFrqHoSuccd();
         break;
      case INTER_FREQ:
         wrIncKpiFrInterFrqHOGpAsstSuccd(measGapUsd);
         break;
      default:
         RLOG0(L_ERROR, "wrIncKpiFrOutHoSuccdBsdOnFrqTyp :Invalid\
                  frequency Type"); 
         RETVALUE(RFAILED);     
   }
   RETVALUE(ROK);
} /* end of wrIncKpiFrOutHoSuccdBsdOnFrqTyp */

/** @brief This function increments KPI for Successful outgoing intra-frequency
 * handovers
 *          
 * @details This function increments KPI for Successful outgoing intra-frequency
 * handovers
 *          
 *          
 *     Function: wrIncKpiFrOutIntraFreqHoSuccd
 *          
 * @param[out]            
 * @param[in]          none 
 * @return  S16
 *        -# Success : ROK
 */
/*  */
PUBLIC S16 wrIncKpiFrOutIntraFrqHoSuccd
(
Void
)
{
   TRC2(wrIncKpiFrOutIntraFrqHoSuccd);

   /*Increment KPI by 1*/
   WR_INC_KPI(KPI_ID_LTE_HO_INTRAFREQOUTSUCC,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);
   RETVALUE(ROK);
} /* end of wrIncKpiFrOutIntraFrqHoSuccd */

/** @brief This function increments KPI for Handover related measurements for
 * DRX/non-DRX Attempted
 *          
 * @details This function increments KPI for Handover related measurements for
 * DRX/non-DRX Attempted
 *          
 *          
 *     Function: wrIncKpiFrHONoDrxOutAttmptd
 *          
 * @param[out]            
 * @param[in]          Bool drxStatus 
 * @return  S16
 *        -# Success : ROK
 */
/*  */
PUBLIC S16 wrIncKpiFrHONoDrxOutAttmptd
(
Bool drxStatus
)
{
   TRC2(wrIncKpiFrHONoDrxOutAttmptd);

   if(drxStatus)/*DRX cycle is used*/
   {
      WR_INC_KPI(KPI_ID_LTE_HO_DRXOUTATT,KPI_VALUE_TYPE_INTEGER,
            INC_KPI_VALUE_BY_ONE);
   }       
   else
   {
      WR_INC_KPI(KPI_ID_LTE_HO_NODRXOUTATT,KPI_VALUE_TYPE_INTEGER,
            INC_KPI_VALUE_BY_ONE);
   }

   RETVALUE(ROK);
} /* end of wrIncKpiFrHONoDrxOutAttmptd */

/** @brief This function increments KPI for Handover related measurements for
 * DRX/non-DRX Successful
 *          
 * @details This function increments KPI for Handover related measurements for
 * DRX/non-DRX Successful
 *          
 *          
 *     Function: wrIncKpiFrHONoDrxOutAttmptd
 *          
 * @param[out]            
 * @param[in]          Bool drxStatus 
 * @return  S16
 *        -# Success : ROK
 */
/*  */
PUBLIC S16 wrIncKpiFrHONoDrxOutSuccd
(
Bool drxStatus
)
{
   TRC2(wrIncKpiFrHONoDrxOutSuccd);

   if(drxStatus) /*DRX Cycle is used*/
   {
      WR_INC_KPI(KPI_ID_LTE_HO_DRXOUTSUCC,KPI_VALUE_TYPE_INTEGER,
            INC_KPI_VALUE_BY_ONE);
   }
   else
   {
      WR_INC_KPI(KPI_ID_LTE_HO_NODRXOUTSUCC,KPI_VALUE_TYPE_INTEGER,
            INC_KPI_VALUE_BY_ONE);
   } 
   RETVALUE(ROK);
} /* end of wrIncKpiFrHONoDrxOutSuccd */

/** @brief This function increments KPI for Attempted outgoing inter-frequency
 * handovers gap-assisted measurement
 *          
 * @details This function increments KPI for Attempted outgoing inter-frequency
 * handovers gap-assisted measurement
 *          
 *          
 *     Function: wrIncKpiFrInterFrqHOGpAsstAtmpt
 *          
 * @param[out]            
 * @param[in]          Bool measGapSetUp 
 * @return  S16
 *        -# Success : ROK
 */
/*  */
PUBLIC S16 wrIncKpiFrInterFrqHOGpAsstAtmpt 
(
Bool measGapSetUp
)
{
   TRC2(wrIncKpiFrInterFrqHOGpAsstAtmpt);

   if(measGapSetUp) /*Meas Gap is used*/
   {
      WR_INC_KPI(KPI_ID_LTE_HO_INTERFREQMEASGAPOUTATT,KPI_VALUE_TYPE_INTEGER,
            INC_KPI_VALUE_BY_ONE);
   }
   else
   {
      WR_INC_KPI(KPI_ID_LTE_HO_INTERFREQNOMEASGAPOUTATT,KPI_VALUE_TYPE_INTEGER,
            INC_KPI_VALUE_BY_ONE);
   } 
   RETVALUE(ROK);
} /* end of wrIncKpiFrInterFrqHOGpAsstAtmpt */

/** @brief This function increments KPI for Successful outgoing inter-frequency
 * handovers gap-assisted measurement
 *          
 * @details This function increments KPI for Successful outgoing inter-frequency
 * handovers gap-assisted measurement
 *          
 *          
 *     Function: wrIncKpiFrInterFrqHOGpAsstSuccd
 *          
 * @param[out]            
 * @param[in]          Bool measGapSetUp 
 * @return  S16
 *        -# Success : ROK
 */
/*  */
PUBLIC S16 wrIncKpiFrInterFrqHOGpAsstSuccd 
(
Bool measGapSetUp
)
{
   TRC2(wrIncKpiFrInterFrqHOGpAsstSuccd);

   if(measGapSetUp) /*Meas Gap is used*/
   {
      WR_INC_KPI(KPI_ID_LTE_HO_INTERFREQMEASGAPOUTSUCC,
            KPI_VALUE_TYPE_INTEGER,INC_KPI_VALUE_BY_ONE);
   }
   else
   {
      WR_INC_KPI(KPI_ID_LTE_HO_INTERFREQNOMEASGAPOUTSUCC,
            KPI_VALUE_TYPE_INTEGER,INC_KPI_VALUE_BY_ONE);
   }

   RETVALUE(ROK);
} /* end of wrIncKpiFrInterFrqHOGpAsstSuccd */

/** @brief This function increments KPI for UE CONTEXT Release : Number of UE
 * CONTEXT Release Request initiated by eNodeB
 *          
 * @details This function increments KPI for UE CONTEXT Release : Number of UE
 * CONTEXT Release Request initiated by eNodeB
 *          
 *          
 *     Function: wrIncKpiUeCtxRelReqAttmptd
 *          
 * @param[out]            
 * @param[in]          none 
 * @return  S16
 *        -# Success : ROK
 */
/*  */
PUBLIC S16 wrIncKpiUeCtxRelReqAttmptd
(
Void
)
{
   TRC2(wrIncKpiUeCtxRelReqAttmptd);

   /*Increment KPI by 1*/
   WR_INC_KPI(KPI_ID_LTE_UECNTX_RELREQ_SUM,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);
   RETVALUE(ROK);
} /* end of wrIncKpiUeCtxRelReqAttmptd */

/** @brief This function increments KPI for UE CONTEXT Release : Successful UE
 * CONTEXT Release
 *          
 * @details This function increments KPI for UE CONTEXT Release : Successful UE
 * CONTEXT Release
 *          
 *          
 *     Function: wrIncKpiUeCtxRelReqSuccd
 *          
 * @param[out]            
 * @param[in]          none 
 * @return  S16
 *        -# Success : ROK
 */
/*  */
PUBLIC S16 wrIncKpiUeCtxRelReqSuccd
(
Void
)
{
   TRC2(wrIncKpiUeCtxRelReqSuccd);

   /*Increment KPI by 1*/
   WR_INC_KPI(KPI_ID_LTE_UECONTEXT_RELSUCCNBR,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);
   RETVALUE(ROK);
} /* end of wrIncKpiUeCtxRelReqSuccd */

/** @brief This function increments KPI for Number of Initial E-RABs attempted
 * to Setup
 *          
 * @details This function increments KPI for Number of Initial E-RABs attempted
 * to Setup
 *          
 *          
 *     Function: wrIncKpiFrERABEstbInitAttPrQci
 *          
 * @param[out]            
 * @param[in]          none 
 * @return  S16
 *        -# Success : ROK
 */
/*  */
PUBLIC S16 wrIncKpiFrERABEstbInitAttPrQci
(
U8 qciVal
)
{
  KpiId kpiId;
  TRC2(wrIncKpiFrERABEstbInitAttPrQci);
 
  kpiId = KPI_ID_LTE_ERAB_ESTABINITATTNBR_QCI1 + qciVal - 1; 
  /*Increment KPI by 1*/
   WR_INC_KPI(kpiId,KPI_VALUE_TYPE_INTEGER,
        INC_KPI_VALUE_BY_ONE); 
  WR_INC_KPI(KPI_ID_LTE_ERAB_ESTABINITATTNBR_QCI_SUM,KPI_VALUE_TYPE_INTEGER,
        INC_KPI_VALUE_BY_ONE); 

  RETVALUE(ROK);
} /* end of wrIncKpiFrERABEstbInitAttPrQci*/

/** @brief This function increments KPI for E-RAB modification:Number of E-RABs
 * successfully modified the QoS parameter
 *          
 * @details This function increments KPI for E-RAB modification:Number of E-RABs
 * successfully modified the QoS parameter
 *          
 *          
 *     Function: wrIncKpiFrERABMdQoSSuccNbrPrQci 
 *          
 * @param[out]            
 * @param[in]        qciVal 
 * @return  S16
 *        -# Success : ROK
 */
/*  */
PUBLIC S16 wrIncKpiFrERABMdQoSSuccNbrPrQci
(
U8 qciVal
)
{
   KpiId kpiId;
   TRC2(wrIncKpiFrERABMdQoSSuccNbrPrQci);
 
   kpiId = KPI_ID_LTE_ERAB_MODQOSSUCCNBR_QCI1 + qciVal -1;  
    /*Increment KPI*/
   WR_INC_KPI(kpiId,KPI_VALUE_TYPE_INTEGER,1);
   /*Increment KPI*/
   WR_INC_KPI(KPI_ID_LTE_ERAB_MODQOSSUCCNBR_QCI_SUM,KPI_VALUE_TYPE_INTEGER,
         1);
 
   RETVALUE(ROK);
} /* end of wrIncKpiFrERABMdQoSSuccNbrPrQci*/

/** @brief This function increments KPI for UE-associated logical S1-connection
 * establishment:Succesful UE-associated logical S1-connection establishment
 * from eNB to MME
 *          
 * @details This function increments KPI for UE-associated logical S1-connection
 *  establishment:Succesful UE-associated logical S1-connection establishment
 *  from eNB to MME
 *          
 *          
 *     Function: wrIncKpiFrS1SIGConnEstabSucc
 *          
 * @param[out]            
 * @param[in]          none 
 * @return  S16
 *        -# Success : ROK
 */
/*  */
PUBLIC S16 wrIncKpiFrS1SIGConnEstabSucc
(
Void
)
{
   TRC2(wrIncKpiFrS1SIGConnEstabSucc);

   /*Increment KPI by 1*/
   WR_INC_KPI(KPI_ID_LTE_S1SIG_CONNESTABSUCC,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);
   RETVALUE(ROK);
} /* end of wrIncKpiFrS1SIGConnEstabSucc */

/** @brief This function increments KPI for UE-associated logical S1-connection
 * establishmentSuccesful UE-associated logical S1-connection establishment from
 * eNB to MME
 *          
 * @details This function increments KPI for UE-associated logical S1-connection
 * establishmentSuccesful UE-associated logical S1-connection establishment from
 * eNB to MME
 *          
 *          
 *     Function: wrIncKpiFrS1SIGConnEstabAttmptd
 *          
 * @param[out]            
 * @param[in]          none 
 * @return  S16
 *        -# Success : ROK
 */
/*  */
PUBLIC S16 wrIncKpiFrS1SIGConnEstabAttmptd
(
Void
)
{
   TRC2(wrIncKpiFrS1SIGConnEstabAttmptd);

   /*Increment KPI by 1*/
   WR_INC_KPI(KPI_ID_LTE_S1SIG_CONNESTABATT,KPI_VALUE_TYPE_INTEGER,
         INC_KPI_VALUE_BY_ONE);
   RETVALUE(ROK);
} /* end of wrIncKpiFrS1SIGConnEstabAttmptd */


/** @brief This function increments KPI for Number of E-RABs attempted to setup
 *          
 * @details This function increments KPI for Number of E-RABs attempted to setup
 *          
 *          
 *     Function: wrIncKpiFrERABEstabAttPrQci
 *          
 * @param[out]            
 * @param[in]          qciVal
 * @return  S16
 *        -# Success : ROK
 */
/*  */
PUBLIC S16 wrIncKpiFrERABEstabAttPrQci
(
U8 qciVal 
)
{
   KpiId kpiId;

   TRC2(wrIncKpiFrERABEstabAttPrQci);

   kpiId = KPI_ID_LTE_ERAB_ESTABATTNBR_QCI1 + qciVal -1;
   WR_INC_KPI(kpiId,KPI_VALUE_TYPE_INTEGER,1);
   /*Increment KPI*/
   WR_INC_KPI(KPI_ID_LTE_ERAB_ESTABATTNBR_QCI_SUM,KPI_VALUE_TYPE_INTEGER,
         1);
   RETVALUE(ROK);
} /* end of wrIncKpiFrERABEstabAttPrQci*/

/** @brief This function increments KPI for Number of Initial E-RABs
 * successfully established
 *          
 * @details This function increments KPI for Number of Initial E-RABs
 * successfully established
 *          
 *          
 *     Function: wrIncKpiFrERABEstbIntSuccPrQci
 *          
 * @param[out]            
 * @param[in]          qciVal
 * @return  S16
 *        -# Success : ROK
 */
/*  */
PUBLIC S16 wrIncKpiFrERABEstbIntSuccPrQci
(
U8 qciVal
)
{
   KpiId kpiId; 
   TRC2(wrIncKpiFrERABEstbIntSuccPrQci);

    kpiId = KPI_ID_LTE_ERAB_ESTABINITSUCCNBR_QCI1 + qciVal - 1; 
   /*Counts number Of Erab established in predefied interval*/
   kpiInfo.kpiMeanInfo.erabMean.numOfEarb++;
   kpiInfo.kpiMaxInfo.erabMax.numOfErab++;
   /*Increment KPi*/
   /*Increment KPi*/
   WR_INC_KPI(kpiId,KPI_VALUE_TYPE_INTEGER,
         1);
   WR_INC_KPI(KPI_ID_LTE_ERAB_ESTABINITSUCCNBR_QCI_SUM,KPI_VALUE_TYPE_INTEGER,
         1);
   RETVALUE(ROK);
} /* end of wrIncKpiFrERABEstbIntSuccPrQci*/

/** @brief This function increments KPI for Number of initial E-RABs failed to
 * setup
 *          
 * @details This function increments KPI for Number of initial E-RABs failed to
 * setup
 *          
 *          
 *     Function: wrIncKpiFrERABEstabInitFailSum
 *          
 * @param[out]            
 * @param[in]          numFailedErabs 
 * @return  S16
 *        -# Success : ROK
 */
/*  */
PUBLIC S16 wrIncKpiFrERABEstbInitFailSum
(
U16 numFailedErabs
)
{
   TRC2(wrIncKpiFrERABEstbInitFailSum);

   /*Increment KPI*/
   WR_INC_KPI(KPI_ID_LTE_ERAB_ESTABINITFAILNBR_SUM,KPI_VALUE_TYPE_INTEGER,
         numFailedErabs); 
   RETVALUE(ROK);
} /* end of wrIncKpiFrERABEstbInitFailSum */

/** @brief This function increments KPI for Number of E-RABs successfully
 * released
 *          
 * @details This function increments KPI for Number of E-RABs successfully
 * released
 *          
 *          
 *     Function: wrIncKpiFrERABRelSuccPrQci
 *          
 * @param[out]            
 * @param[in]         qciVal 
 * @return  S16
 *        -# Success : ROK
 */
/*  */
PUBLIC S16 wrIncKpiFrERABRelSuccPrQci
(
U8 qciVal
)
{
   KpiId kpiId;

   TRC2(wrIncKpiFrERABRelSuccPrQci);

   kpiId = KPI_ID_LTE_ERAB_RELSUCCNBR_QCI1 + qciVal -1;
   /*Increment KPI*/
   WR_INC_KPI(kpiId,KPI_VALUE_TYPE_INTEGER,1);

   /*Increment KPI*/
   WR_INC_KPI(KPI_ID_LTE_ERAB_RELSUCCNBR_QCI_SUM,KPI_VALUE_TYPE_INTEGER,1);
   RETVALUE(ROK);
} /* end of wrIncKpiFrERABRelSuccPrQci*/

/** @brief This function increments KPI for Number of E-RABs successfully
 * established
 *          
 * @details This function increments KPI for Number of E-RABs successfully
 * established
 *          
 *          
 *     Function: wrIncKpiFrERABEstabSuccPrQci
 *          
 * @param[out]            
 * @param[in]          qciVal
 * @return  S16
 *        -# Success : ROK
 */
/*  */
PUBLIC S16 wrIncKpiFrERABEstabSuccPrQci
(
U8 qciVal
)
{
   KpiId kpiId;

   TRC2(wrIncKpiFrERABEstabSuccPrQci);

    kpiId = KPI_ID_LTE_ERAB_ESTABSUCCNBR_QCI1 + qciVal -1;
   /*Counts number Of Erab established in predefied interval*/ 
   kpiInfo.kpiMeanInfo.erabMean.numOfEarb++;
   kpiInfo.kpiMaxInfo.erabMax.numOfErab++;
   WR_INC_KPI(kpiId,KPI_VALUE_TYPE_INTEGER,
         1); 
   /*Incremnt KPI by 1*/
   WR_INC_KPI(KPI_ID_LTE_ERAB_ESTABSUCCNBR_QCI_SUM,KPI_VALUE_TYPE_INTEGER,
         1);
   RETVALUE(ROK);
} /* end of wrIncKpiFrERABEstabSuccPrQci*/

/** @brief This function increments KPI for Number of E-RABs attempted to
 * release
 *          
 * @details This function increments KPI for Number of E-RABs attempted to
 * release
 *          
 *          
 *     Function: wrIncKpiFrERABRelAttPrQci
 *          
 * @param[out]            
 * @param[in]          qciVal
 * @return  S16
 *        -# Success : ROK
 */
/*  */
PUBLIC S16 wrIncKpiFrERABRelAttPrQci
(
U8 qciVal
)
{
   KpiId kpiId;

   TRC2(wrIncKpiFrERABRelAttPrQci);

   kpiId = KPI_ID_LTE_ERAB_RELATTNBR_QCI1 + qciVal - 1;
   /*Increment KPI*/
   WR_INC_KPI(kpiId,KPI_VALUE_TYPE_INTEGER,1);
   /*Increment KPI*/
   WR_INC_KPI(KPI_ID_LTE_ERAB_RELATTNBR_QCI_SUM,KPI_VALUE_TYPE_INTEGER,1); 
   RETVALUE(ROK);
} /* end of wrIncKpiFrERABRelAttPrQci*/

/** @brief This function increments KPI for E-RAB modification Number of E-RABs
 * attempted to modify the QoS parameter
 *          
 * @details This function increments KPI for E-RAB modification Number of E-RABs
 * attempted to modify the QoS parameter
 *          
 *          
 *     Function: wrIncKpiFrERABModQoSAttPrQci
 *          
 * @param[out]            
 * @param[in]          qciVal
 * @return  S16
 *        -# Success : ROK
 */
/*  */
PUBLIC S16 wrIncKpiFrERABModQoSAttPrQci
(
U8 qciVal
)
{
   KpiId kpiId;
   
   TRC2(wrIncKpiFrERABModQoSAttPrQci);

   kpiId = KPI_ID_LTE_ERAB_MODQOSATTNBR_QCI1+ qciVal -1;
   /*Increment KPI*/
   WR_INC_KPI(kpiId,KPI_VALUE_TYPE_INTEGER,
         1);
   /*Increment KPI*/
   WR_INC_KPI(KPI_ID_LTE_ERAB_MODQOSATTNBR_QCI_SUM,KPI_VALUE_TYPE_INTEGER,
         1);

   RETVALUE(ROK);
} /* end of wrIncKpiFrERABModQoSAttPrQci*/

/** @brief This function sends  kpi info after expiry of Collection tmr 
 *          
 * @details This function sends kpi info (MEAN/MAX kpis) after the expiry of timer 
 *            Collection Periodic tmr to OAM-SM
 *          
 *          
 *          
 *     Function: wrSendKpisInfo 
 *          
 * @param[out]            
 * @param[in]           
 * @return  S16
 *        -# Success : ROK
 */
/*  */
PUBLIC S16 wrSendKpisInfo
(
Void
)
{
   U32 *ptr;
   U32 *sumPtr,*conCntPtr;
   U32  indx;
   U32  kpiId;
   U32  kpiVal = 0;
   U32  *eRabsEst, *eRabEstMeanVal, cntrVal=0;

   TRC2(wrSendKpisInfo);

   sumPtr = NULLP;
   conCntPtr = NULLP;

   kpiInfo.kpiCollectionTmrExpry = TRUE;

   SGetEpcTime(&kpiInfo.endTime);
   kpiInfo.kpiMeasPrd = kpiInfo.endTime - kpiInfo.startTime;
   kpiInfo.startTime = kpiInfo.endTime;

   /*Stoping predefined timer*/
   wrStopTmr((PTR)NULLP,WR_TMR_KPI_PREDFND_INTRVL_TMR);
   /*if WR_TMR_KPI_COLLECTION_TMR  expired and WR_TMR_KPI_PREDFND_INTRVL_TMR
     is stopped then collect KPIs info those were collected when T2 was running before
     reset kpiInfo*/
   wrIncKpiPredfndIntrvlTmrExpiry();
   sumPtr = (U32 *)&(kpiInfo.kpiMeanInfo.rrcConnEstabTimeSum.emergency);
   conCntPtr = (U32 *)&(kpiInfo.kpiMeanInfo.rrcConnMean.numOfRrcConEmergency);
   kpiId= KPI_ID_LTE_RRC_CONNESTABTIMEMEAN_EMERGENCY;
   /*Here indx is number of estab cause for rrc connection*/
   for(indx = 0; indx < NUM_RRC_CONN_ESTAB_CAUSE; indx++)
   {
      if (*conCntPtr)
      {
         kpiVal = (*sumPtr)/(*conCntPtr);
      }
      if(kpiVal)
      { 
         WR_INC_KPI(kpiId,KPI_VALUE_TYPE_INTEGER,kpiVal);
      }
      kpiId++;
      sumPtr++;
      conCntPtr++;
      kpiVal = 0;
   }

   ptr = (U32 *)&(kpiInfo.kpiMaxInfo.rrcConnEstabTimeMax);
   kpiId= KPI_ID_LTE_RRC_CONNESTABTIMEMAX_EMERGENCY;
   /*Here indx is number of estab cause for rrc connection*/
   for(indx = 0; indx < NUM_RRC_CONN_ESTAB_CAUSE; indx++)
   {   
      if(*(ptr+indx))
      {
         WR_INC_KPI(kpiId,KPI_VALUE_TYPE_INTEGER,*(ptr+indx));
      }
      kpiId++;
   }   

   eRabEstMeanVal=(U32 *)&(kpiInfo.kpiMeanInfo.erabEstabTimeMeanPrQci);
   eRabsEst=(U32 *)&(kpiInfo.kpiMeanInfo.noOfSuccEstERabs);
   kpiId= KPI_ID_LTE_ERAB_ESTABTIMEMEAN_QCI1;
   /*Here indx is number of qci value (max is 9)*/
   for(indx = 0; indx < MAX_QCI_VALUE;indx++ )
   {   
      if(*(eRabEstMeanVal + indx))
      {
         cntrVal = (*(eRabEstMeanVal+indx))/(*(eRabsEst+indx));
         WR_INC_KPI(kpiId,KPI_VALUE_TYPE_INTEGER,cntrVal);
      }
      kpiId++;
   }   

   ptr=(U32 *)&(kpiInfo.kpiMaxInfo.erabEstabTimeMaxPrQci);
   kpiId= KPI_ID_LTE_ERAB_ESTABTIMEMAX_QCI1;
   /*Here indx is number of qci value (max is 9)*/
   for(indx = 0; indx < MAX_QCI_VALUE;indx++ )
   {
      if(*(ptr+indx))
      {
         WR_INC_KPI(kpiId,KPI_VALUE_TYPE_INTEGER,*(ptr+indx));
      }
      kpiId++;
   }
   if(kpiInfo.kpiMeanInfo.rrcConnMean.meanOfNumOfRrcCon != 0 )
   {
      WR_INC_KPI(KPI_ID_LTE_RRC_CONNMEAN,KPI_VALUE_TYPE_INTEGER,kpiInfo.
            kpiMeanInfo.rrcConnMean.meanOfNumOfRrcCon);
   } 
   if(kpiInfo.kpiMaxInfo.rrcConnMax.maxNmOfRrcCon != 0)
   {
      WR_INC_KPI(KPI_ID_LTE_RRC_CONNMAX,KPI_VALUE_TYPE_INTEGER,
            kpiInfo.kpiMaxInfo.rrcConnMax.maxNmOfRrcCon);
   }
   if(kpiInfo.kpiMeanInfo.erabMean.meanOfNumOfErab != 0)
   {
      WR_INC_KPI(KPI_ID_LTE_ERAB_USAGENBRMEAN_QCI_SUM,KPI_VALUE_TYPE_INTEGER,
            kpiInfo.kpiMeanInfo.erabMean.meanOfNumOfErab);
   }

   if(kpiInfo.kpiMaxInfo.erabMax.maxOfNumOfErab != 0)
   {
      WR_INC_KPI(KPI_ID_LTE_ERAB_USAGENBRMAX_QCI_SUM,KPI_VALUE_TYPE_INTEGER,
            kpiInfo.kpiMaxInfo.erabMax.maxOfNumOfErab);
   }
/* CSG_DEV Start */
   if(kpiInfo.kpiMeanInfo.csgUeMean.meanOfNumOfCsgUe!= 0 )
   {
      WR_INC_KPI(KPI_ID_LTE_CSG_UEATTACHED_MEAN,KPI_VALUE_TYPE_INTEGER,kpiInfo.
            kpiMeanInfo.csgUeMean.meanOfNumOfCsgUe);
   }
/* CSG_DEV Ends */ 
   /*S1AP Failure statistics*/
   smGetS1apStats();
#ifdef LTE_L2_MEAS 
   wrMemCpuL3MeasSendReq();
   if(kpiInfo.l2CollecEna == TRUE)
   {
      wrSndL2MeasSendReq();
   }
   else
#endif /*LTE_L2_MEAS*/  
   {  
#ifdef WR_RSYS_OAM
      KpisInfoSentNotification();
#endif
      wrKpiInitKpiInfo(WR_KPI_RESET_KPI_INFO,kpiInfo.l2CollecEna);
   }
   RETVALUE(ROK);
} /* end of wrIncKpiCollectionTmrExpry */

/** @brief This function handles the expiry of timer T2 Tmr 
 *          
 * @details This function handles the expiry of timer onTmr and updates Kpis
 *           (Mean/MAx Kpis)in KPI module.
 *          
 *          
 *     Function: wrIncKpiPredfndIntrvlTmrExpiry 
 *          
 * @param[out]            
 * @param[in]           
 * @return  S16
 *        -# Success : ROK
 */

PUBLIC S16 wrIncKpiPredfndIntrvlTmrExpiry
(
Void
)
{
   TRC2(wrIncKpiPredfndIntrvlTmrExpiry);

   if(kpiInfo.kpiMeanInfo.rrcConnMean.numOfRrcCon != 0)
   {
      WR_CAL_MEAN(&(kpiInfo.kpiMeanInfo.rrcConnMean.meanOfNumOfRrcCon),kpiInfo.
            kpiMeanInfo.rrcConnMean.numOfRrcCon);
   }

   WR_CAL_MAX(&(kpiInfo.kpiMaxInfo.rrcConnMax.maxNmOfRrcCon),kpiInfo.kpiMaxInfo.
         rrcConnMax.numOfRrcCon);
   if(kpiInfo.kpiMeanInfo.erabMean.numOfEarb != 0)
   {
      WR_CAL_MEAN(&(kpiInfo.kpiMeanInfo.erabMean.meanOfNumOfErab),kpiInfo.
            kpiMeanInfo.erabMean.numOfEarb);
   }

   WR_CAL_MAX(&(kpiInfo.kpiMaxInfo.erabMax.maxOfNumOfErab),kpiInfo.kpiMaxInfo.
         erabMax.numOfErab);
   /*Again start  accumulated Pm counters(MEAN/MAX related) updataion timer*/
   if(!kpiInfo.kpiCollectionTmrExpry)
   {
      wrStartTmr((PTR)NULLP,WR_TMR_KPI_PREDFND_INTRVL_TMR,
            kpiInfo.kpiPredfndIntrvlTmrVal);
   }
   /* CSG_DEV */
   if(kpiInfo.kpiMeanInfo.csgUeMean.numOfCsgUe != 0)
   {
      WR_CAL_MEAN(&(kpiInfo.kpiMeanInfo.csgUeMean.meanOfNumOfCsgUe),kpiInfo.
            kpiMeanInfo.csgUeMean.numOfCsgUe);
   }

   RETVALUE(ROK);
} /* end of wrIncKpiPredfndIntrvlTmrExpiry */


/** @brief This function increments KPI for Number of RRC Connection reject
 *           
 * @details  This KPI provides the number of failed RRC establishment per 
 * failure cause. Transmission of an RRCConnectionReject message by the 
 * eNodeB to the UE. Each transmitted RRCConnectionReject message caused by 
 * "congestion" is added to the measurement cause "Congestion", and each 
 * transmitted RRCConnectionReject message caused by the other reasons is 
 * added to measurement cause "Unspecified".
 *          
 *          
 *  Function: wrIncKpiFrRrcConReject
 *          
 * @param[out]            
 * @param[in]         qciVal 
 * @return  S16
 *        -# Success : ROK
 */
/*  */
PUBLIC S16 wrIncKpiFrRrcConReject
(
 U8 cause
)
{

   TRC2(wrIncKpiFrRrcConReject);


   if(cause == 1)
   {
      WR_INC_KPI(KPI_ID_LTE_RRC_FAILCONESTAB_CONGESTION, 
            KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
   }
   else
   {
      WR_INC_KPI(KPI_ID_LTE_RRC_FAILCONESTAB_UNSPECIFIED,
            KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
   }

   RETVALUE(ROK);
} /* end of wrIncKpiFrERABRelSuccPrQci*/ 


/** @brief This function increments KPI for Attempted CS Fallback 
 *    i. with Re-Direction
 *    ii. without Re-Direction
 *    iii. enhanced CS Fallback
 *    iv. Cell change order
 *          
 * @details This function increments KPI for Attempted CS Fallback
 *          
 *     Function: wrIncKpiCsfbAtt
 *          
 * @param[out]            
 * @param[in]          none 
 * @return  S16
 *        -# Success : ROK
 */
/*  */
PUBLIC S16 wrIncKpiCsfbAtt
(
 U8    redirType
 )
{
   TRC2(wrIncKpiCsfbAtt);

   RLOG0(L_INFO, "wrIncKpiCsfbAtt Called ");
   /*Incrementing Generic CSFB Counter by Default*/
   WR_INC_KPI(KPI_ID_LTE_CSFB_ATT_SUM, 
         KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
   switch(redirType)
   {
      case CDMA2000_CSFB_WITHOUT_PS_HO:
         {
            /*Increment CSFB Attempt KPI*/
            WR_INC_KPI(KPI_ID_LTE_CSFB_CDMA2000_WO_PSHO_ATT, 
                  KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
         }   
         break;
         /*CDMA2000_CSFB needs to be removed once KPIs for UTRAN is introduced 
          * as this case hits the generic CSFB Attempt counter*/
      case CDMA2000_CSFB:
         {
            /* Incremented above */
         }
         break;
      case GERAN_CSFB:
         {
            /*Increment GERAN CSFB Attempt KPI*/
            WR_INC_KPI(KPI_ID_LTE_CSFB_GERAN_ATT_SUM, 
                  KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
         }
         break;
      case UTRA_CSFB:
         {
            /* Increment the UTRA CSFB attempted counter */
            WR_INC_KPI(KPI_ID_LTE_CSFB_UTRA_ATT_SUM,
                  KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
         }
         break;
      case UNKNOWN_CSFB_REDIRECTION_TYPE:
         {
            RLOG0(L_ERROR, "wrIncKpiCsfbAtt :None of the RAT Selected for CSFB"); 
         }
         break;
      default:
         {
            RLOG0(L_ERROR, "wrIncKpiCsfbAtt :Invalid RAT selected for CSFB"); 
         }
   }
   RETVALUE(ROK);
} /* end of wrIncKpiCsfbAtt */

/** @brief This function increments KPI for Successful CS Fallback 
 * based on i) ratType = CDMA2000_1xRTT
 *              -redirType = with Re-Direction
 *              -redirType = without Re-Direction
 *              -redirType = enhanced CS Fallback or without PS HO
 *         ii) ratType = GERAN_CSFB
 *              -redirType = with Re-direction 
 *              -redirType = Cell Change Order
 * @details This function increments KPI for Successful CS Fallback
 *          
 *          
 *     Function: wrIncKpiCsfbSucc
 *          
 * @param[out]            
 * @param[in]          none 
 * @return  S16
 *        -# Success : ROK
 */
/*  */
PUBLIC S16 wrIncKpiCsfbSucc
(
 U32   ratType,
 U8    redirType
 )
{
   TRC2(wrIncKpiCsfbSucc);

   RLOG0(L_ERROR, "wrIncKpiCsfbSucc Called ");
   /*incrementing  the the CSFB SUCC Counters */
   WR_INC_KPI(KPI_ID_LTE_CSFB_SUCC_SUM, KPI_VALUE_TYPE_INTEGER, 
         INC_KPI_VALUE_BY_ONE);

   switch(ratType)
   {
      case CDMA2000_1XRTT:
         {
            RLOG0(L_INFO, "wrIncKpiCsfbSucc: Success CSFB to CDMA  incremented ");
            switch(redirType)
            {
               case CDMA2000_CSFB_WITHOUT_REDIRECTION:
                  {
                     WR_INC_KPI(KPI_ID_LTE_CSFB_CDMA2000_DUALRX_WO_REDIR, 
                           KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
                  }
                  break;
               case CDMA2000_CSFB_WITH_REDIRECTION:
                  {
                     WR_INC_KPI(KPI_ID_LTE_CSFB_CDMA2000_REDIR,
                           KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
                  }
                  break;
               case CDMA2000_CSFB_WITHOUT_PS_HO:
                  {
                     WR_INC_KPI(KPI_ID_LTE_CSFB_CDMA2000_WO_PSHO_SUCC,
                           KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
                  }   
                  break;
                  /*RRC Rel10 Upgrade - start*/
               case CDMA2000_CSFB_DUALRXTX_REDIR:
                  {
                     WR_INC_KPI(KPI_ID_LTE_CSFB_CDMA2000_DUALRXTX_REDIR,
                           KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
                  }   
                  break;
               case CDMA2000_CSFB_DUALRXTX_DLNAS:
                  {
                     WR_INC_KPI(KPI_ID_LTE_CSFB_CDMA2000_DUALRXTX_DLNAS,
                           KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
                  }   
                  break;
                  /*RRC Rel10 Upgrade - end*/
               default:
                  {
                     RLOG0(L_ERROR, "wrIncKpiCsfbSucc :Invalid CSFB KPI Called"); 
                  }
            }
         }
         break;
      case GERAN:
         {
            RLOG0(L_INFO, "wrIncKpiCsfbSucc: Success CSFB to GERAN incremented ");

            switch(redirType)
            {
               case GERAN_CSFB_CCO:
                  {
                     /*Increment SUCC SUM */
                     WR_INC_KPI(KPI_ID_LTE_CSFB_GERAN_SUCC_SUM,
                           KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
                     /*Increment for CCO*/
                     WR_INC_KPI(KPI_ID_LTE_CSFB_GERAN_CCO,
                           KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
                  }
                  break;
               case GERAN_CSFB_WITH_REDIRECTION: 
                  {
                     /*Increment SUCC SUM */
                     WR_INC_KPI(KPI_ID_LTE_CSFB_GERAN_SUCC_SUM,
                           KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
                     /*Increment for redirection*/
                     WR_INC_KPI(KPI_ID_LTE_CSFB_GERAN_REDIR,
                           KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
                  }
                  break;
               case GERAN_CSFB_WITHOUT_REDIRECTION:
                  break;
               default:
                  {
                     RLOG0(L_ERROR, "wrIncKpiCsfbSucc :Invalid CSFB KPI Called");
                  }
            }
         }
         break;

      case UTRA_TDD:
      case UTRA_FDD:
         {
            /*Increment CSFB SUCC*/
            RLOG0(L_INFO, "wrIncKpiCsfbSucc: Success UTRA CSFB incremented ");
            /*Increment UTRA CSFB SUCC SUM */
            WR_INC_KPI(KPI_ID_LTE_CSFB_UTRA_SUCC_SUM,
                  KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);

            switch(redirType)
            {
               case UTRA_CSFB_WITH_REDIRECTION_WITH_SI:
                  {
                     /*Increment for Redirection with SI*/
                     WR_INC_KPI(KPI_ID_LTE_CSFB_UTRA_WITH_REDIR_WITH_SI,
                           KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
                     break;
                  }
               case UTRA_CSFB_WITHOUT_REDIRECTION:
                  {
                     WR_INC_KPI(KPI_ID_LTE_CSFB_UTRA_WITHOUT_REDIR,
                           KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
                     break;
                  }
            }
         }
         break;

      case MAX_RAT_TYPE:
         {
            RLOG0(L_ERROR, "wrIncKpiCsfbSucc : None of the RATs selected"); 
         }
         break;
      default: 
         {
            RLOG0(L_ERROR, "wrIncKpiCsfbSucc :Invalid RAT Type"); 
         }
   }
   RETVALUE(ROK);
} /* end of wrIncKpiCsfbSucc */

/** 
 *     Function: wrIncKpiCsfbFail
 *          
 * @details This function increments KPI for CS Fallback Failure
 * based on i) ratType = CDMA2000_1xRTT
 *             - redirType = with Re-Direction
 *             - redirType = without Re-Direction
 *             - redirType = enhanced CS Fallback or without PS HO
 *          ii)ratType = GERAN_CSFB
 * @param[out]            
 * @param[in]          none 
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 wrIncKpiCsfbFail
(
 U8    redirType
 )
{
   TRC2(wrIncKpiCsfbFail);
   RLOG0(L_INFO, "wrIncKpiCsfbFail: Failure CSFB Counter");

   switch(redirType)
   {
      case CDMA2000_CSFB_WITHOUT_PS_HO:
         {
            WR_INC_KPI(KPI_ID_LTE_CSFB_CDMA2000_WO_PSHO_FAIL, 
                  KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
         }   
         break;
      case CDMA2000_CSFB:
         {
            WR_INC_KPI(KPI_ID_LTE_CSFB_FAIL_SUM, 
                  KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
         }
         break;
      case CDMA2000_CSFB_WO_PSHO_FAIL_REDIR_SUCC:
         {
                WR_INC_KPI(KPI_ID_LTE_CSFB_CDMA2000_WO_PSHO_FAIL_REDIR_SUCC, 
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
         }
         break;
      case GERAN_CSFB:
         {
            WR_INC_KPI(KPI_ID_LTE_CSFB_GERAN_FAIL,
                  KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
         }
         break;
      case UTRA_CSFB:
         {
            WR_INC_KPI(KPI_ID_LTE_CSFB_UTRA_FAIL_SUM,
                  KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
         }
         break;
      default:
         {
            RLOG0(L_ERROR, "wrIncKpiCsfbFail :Invalid"
                     "CSFB KPI Called"); 
         }
   }
   RETVALUE(ROK);
} /* end of wrIncKpiCsfbFail */

/** 
 *     Function: wrIncIratMobLai 
 *          
 * @details This function increments IRAT MOBILITY per LAI
 * based on i) Find the LAI of the UE with the LAI list maintianed by eNB for
 *             GERAN neighbours
 *          ii)All the LAIs in this LAI list will be unique.
 *          iii)If match is found then increment the respective LAI counter by
 *              one.
 *          iv) If match is NOT found with any of the LAIs from LAI list then we
 *              increment the OTHER LAI counter
 * @param[in]  cellId : Cell ID to derive UMM cellCb 
 * @param[in]  plmnId : Selected PLMN Index sent by UE
 * @param[in]  lac    :  Location Area Code
 * @return  Void
 */
PUBLIC Void wrIncIratMobLai
(
 WrCellId  cellId,
 WrPlmnId  plmnId,
 U16       lac
)
{
   U32          idx;
   Bool         flag = FALSE;
   WrUmmCellCb     *cellCb = NULLP;

   WR_UMM_GET_CELLCB(cellCb,cellId);
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR,"cellCb is NULL");
      RETVOID;
   }
   for(idx = 0;idx < WR_KPI_MAX_NUM_MBLTY_LAI;idx++)
   {
      if(PRSNT_NODEF == cellCb->lstLai[idx].pres)
      { 
         if((TRUE == wrEmmPlmnPlmnsEqual(&plmnId,&cellCb->lstLai[idx].plmnId)) &&
               (cellCb->lstLai[idx].lac == lac))
         {
            WR_INC_KPI(KPI_ID_LTE_IRAT_MOB_LAI_1 + idx,
                  KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
            flag = TRUE;
            break;
         }
      }
      /* end of list is nothing but pres is not PRSNT_NODEF */
      else
      {
         break;
      }
   }
   if(FALSE == flag)
   {
      WR_INC_KPI(KPI_ID_LTE_IRAT_MOB_LAI_OTH,
            KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
   }
   RETVOID;
}

/** 
 *     Function: wrIncKpiCsfbFail
 *          
 * @details This function increments KPI for CS Fallback with SI
 * based on i) ratType = UTRA/GERAN
 *          ii) redirType = with Re-Direction with SI
 * @param[out]            
 * @param[in]          none 
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 wrIncKpiCsfbWithSi
(
 U8    redirType
 )
{
   TRC2(wrIncKpiCsfbWithSi);

   switch(redirType)
   {
      case GERAN_CSFB_WITH_REDIRECTION_WITH_SI:
         {
            /*Increment for Redirection with SI*/
            WR_INC_KPI(KPI_ID_LTE_CSFB_GERAN_REDIR_WITH_SI,
                  KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
         }
         break;
      case GERAN_CSFB_CCO_WITH_NACC:
         {
            /*Increment CCO with NACC KPI*/
            WR_INC_KPI(KPI_ID_LTE_CSFB_GERAN_CCO_WITH_NACC,
                  KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
         }
         break;

      default:
         {
            RLOG0(L_ERROR, "wrIncKpiCsfbWithSi :Invalid"
                     "CSFB KPI Called"); 
         }
   }
   RETVALUE(ROK);
} /* end of wrIncKpiCsfbWithSi */
/** @brief This function increments KPI for RIM Attempted
 *          
 * @details This measurement provides the number of attempted RIR Procedure
 *          
 *          
 * Function: wrIncKpiFrRimAttmptd
 *          
 * @param[out]            
 * @param[in]   
 * @return      S16
 * -# Success : ROK
 */

PUBLIC S16 wrIncKpiFrRimAttmptd
(
 Void
 )
{
   TRC2(wrIncKpiFrRimAttmptd);

   WR_INC_KPI(KPI_ID_LTE_RIM_PROC_ATT,
         KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);

   RETVALUE(ROK);

}
/** @brief This function increments KPI for RIM  success
 *          
 * @details This measurement provides the number of success RIR Procedure
 *          
 *          
 * Function: wrIncKpiFrRimSuccd
 *          
 * @param[out]            
 * @param[in]   
 * @return      S16
 * -# Success : ROK
 */

PUBLIC S16 wrIncKpiFrRimSuccd
(
 Void
 )
{
   TRC2(wrIncKpiFrRimSuccd);

   WR_INC_KPI(KPI_ID_LTE_RIM_PROC_SUCC,
         KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);

   RETVALUE(ROK);

}
/** @brief This function increments KPI for RIM Failure
 *          
 * @details This measurement provides the number of failed RIR Procedure
 *          
 *          
 * Function: wrIncKpiFrRimFaild
 *          
 * @param[out]            
 * @param[in]   
 * @return      S16
 * -# Success : ROK
 */

PUBLIC S16 wrIncKpiFrRimFaild
(
 Void
 )
{
   TRC2(wrIncKpiFrRimFaild);

   WR_INC_KPI(KPI_ID_LTE_RIM_PROC_FAIL,
         KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);

   RETVALUE(ROK);
}

/** @brief This function increments KPI for Full Reconfiguration executed for Handover procedure
 *          
 * @details This measurement provides the number of Full Reconfiguration executed for Handover procedure
 *          
 *          
 * Function: wrIncKpiFullRecfgHo
 *          
 * @param[out]            
 * @param[in]   
 * @return      S16
 * -# Success : ROK
 */

PUBLIC S16 wrIncKpiFullRecfgHo
(
 Void
 )
{
   TRC2(wrIncKpiFullRecfgHo);

   WR_INC_KPI(KPI_ID_LTE_HO_FROMENB_FULL_RECFG,
         KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);

   RETVALUE(ROK);
}

/** @brief This function increments KPI for Full Reconfiguration executed for Re-Establishment procedure.
 *          
 * @details This measurement provides the number of Full Reconfiguration executed for Re-Establishment procedure.
 *          
 *          
 * Function: wrIncKpiFullReCfgReestb
 *          
 * @param[out]            
 * @param[in]   
 * @return      S16
 * -# Success : ROK
 */

PUBLIC S16 wrIncKpiFullReCfgReestb
(
 Void
)
{
   TRC2(wrIncKpiFullReCfgReestb);

   WR_INC_KPI(KPI_ID_LTE_RRC_CONNREESTAB_FULL_RECFG,
         KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);

   RETVALUE(ROK);
}

/* ccpu00141451 */
/** @brief This function increments HO successful after RRC Restablishment 
 *          
 * @details Total number of Incoming handover successful after RRC Restablishment 
 *  with target eNB
 *          
 *          
 * Function: wrIncKpiIncHoSuccAftrReEst 
 *          
 * @param[out]            
 * @param[in]   
 * @return      S16
 * -# Success : ROK
 */
PUBLIC S16 wrIncKpiIncHoSuccAftrReEst
(
   Void
)
{
  TRC2(wrIncKpiIncHoSuccAftrReEst);
  WR_INC_KPI(KPI_ID_LTE_HO_FROMENBSUCC_SUM_AFTR_REEST,
                KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
           RETVALUE(ROK);
} /* wrIncKpiIncHoSuccAftrReEst */


/********************************************************************30**

         End of file:

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
     -   ---     
*********************************************************************91*/

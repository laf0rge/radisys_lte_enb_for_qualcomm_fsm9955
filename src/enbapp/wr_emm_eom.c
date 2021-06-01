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
               

     File:     wr_emm_eom.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

/* header include files (.h) */
static const char* RLOG_MODULE_NAME="EMM";
static int RLOG_MODULE_ID=1;
static int RLOG_FILE_ID=7;

#include "wr.h"            /* Common headers and Data Structures */
#include "wr_emm.h"        /* Environment options */
#include "wr_emm_pbm.h"



PUBLIC U8 wrTgrEnbOverLoad;
#ifdef ENB_OVRLOAD
PRIVATE S16 wrProcessOvldFlag(U8  ovldFlag);
PRIVATE S16 wrCalcMemUsage(S16 *memStatus);
PUBLIC S16 wrSetACBarringParams (U8 ovldFlag, WrCellCb *cellCb);
PRIVATE S16 wrEmmUpdOvldSt(U32 cpuLoad, U32 memStatus,U8 *ovldFlag);
PUBLIC S16 wrEmmSampleData(Void);
PUBLIC S16 wrEmmUpdOvrLdStat(Void);
PRIVATE U16 wrCalcCPULoad(U32 *cpuLoad, WrCpuParam *CpuParam);

#ifdef WR_DBG_CIRLOG 
Bool wrInitSuccess;
#endif

/**
 *  @brief This function is usedc to collect Memory and  CPU sample
 * 
 *   @details 
 *
 *       Function :wrEmmSampleData
 *
 *           Processing Steps:
 *              - This function is invoked on every Sampling Timer expiry.
 *              - This function calls wrCalcCPULoad & wrCalcMemUsage to collect
 *              - CPU and memory usage related data.-
 *               CPU and memory usage related data.
 *
 *   @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
*/
PUBLIC S16 wrEmmSampleData
(
Void
)
{
   U32 cpuLoad;
   Status  memStatus;
   /********************************
    * Initialize the memory status *
    ********************************/
   memStatus = 0;
   cpuLoad  = 0;

   if(wrCalcCPULoad(&cpuLoad,&wrEmmCb.cpuUsageparam) != ROK)
   {
      RLOG0(L_ERROR,"wremmeom: getting memory statistics failed ");
      RETVALUE(RFAILED);
   }

   if(wrCalcMemUsage(&memStatus) != ROK)
   {
      RLOG0(L_ERROR,"wremmeom: getting memory statistics failed ");
      RETVALUE(RFAILED);
   }

   wrEmmCb.memSample += memStatus;
   wrEmmCb.cpuSample += cpuLoad;

   RETVALUE(ROK);
}

/**
 *  @brief This function analyzes and updates overload status.
 *
 *
 *  @details
 *      Function : wrEmmUpdOvrLdStat
 *
 *          Processing Steps:
 *              This function is called whenever Overload Monitor timer expires. 
 *              Once invoked this function calculates total CPU and Memory load based 
 *              on the samples taken and call appropriate function to decide the overload state.
 *
 * @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
*/
PUBLIC S16 wrEmmUpdOvrLdStat
(
Void
)
{
   U32 memStatus;
   U32 cpuLoad;
   U8  ovldFlag;
   U8  preOvldSt;
   WrEmmCb *emmCb;
   WrCellCb *cellCb = NULLP;
   U32 idx;


   emmCb = &wrEmmCb;
   memStatus = wrEmmCb.memSample / wrEmmCb.eNBOvldParams.eNBOvldMonfreq;
   cpuLoad = wrEmmCb.cpuSample / wrEmmCb.eNBOvldParams.eNBOvldMonfreq;
   preOvldSt = emmCb->eNBOvldParams.prevOvldFlag;

   switch(preOvldSt)
   {
      case CRITICAL:
         if(cpuLoad >= emmCb->eNBOvldParams.critRcvryVal)
            ovldFlag = CRITICAL;
         else
            wrEmmUpdOvldSt(cpuLoad,memStatus,&ovldFlag);
         break;

      case MAJOR:
         if((cpuLoad < emmCb->eNBOvldParams.eNBMemMajThresVal)
               && (cpuLoad >= emmCb->eNBOvldParams.majRcvryVal))
            if(memStatus >= emmCb->eNBOvldParams.eNBMemCritThresVal)
               ovldFlag = CRITICAL;
            else
               ovldFlag = MAJOR;
         else
            wrEmmUpdOvldSt(cpuLoad,memStatus,&ovldFlag);
         break;

      case MINOR:
         if((cpuLoad < emmCb->eNBOvldParams.eNBMemMajThresVal)
               &&(cpuLoad >= emmCb->eNBOvldParams.majRcvryVal))
            if(memStatus >= emmCb->eNBOvldParams.eNBMemCritThresVal)
               ovldFlag = CRITICAL;
            else if(memStatus >= emmCb->eNBOvldParams.eNBMemCritThresVal)
               ovldFlag = MAJOR;
            else
               ovldFlag = MAJOR;
         else
            wrEmmUpdOvldSt(cpuLoad,memStatus,&ovldFlag);
         break;

      case NORMAL:
            wrEmmUpdOvldSt(cpuLoad,memStatus,&ovldFlag);
         break;

      default:
         RLOG0(L_WARNING, "Incorrect Previous State : Possible Corruption : Taking Corrective Action : Setting State = NORMAL");
         ovldFlag = NORMAL;
         break;
   }

   wrProcessOvldFlag(ovldFlag);

   if (emmCb->eNBOvldParams.ovldCntVal == emmCb->eNBOvldParams.maxCntval )
   {
      wrEmmCb.eNBOvldParams.ovldCntVal = 0;
      for(idx = 0; idx < WR_EMM_MAX_CELLS; idx++)
      {
         /*Assign to the local pointer*/
         cellCb = wrEmmCb.cellCb[idx];
         if(NULLP != cellCb)
         {
            wrSetACBarringParams(ovldFlag, cellCb);
         }
      }
   }

   /* resetting the values for next itterattions */
   wrEmmCb.cpuSample = 0;
   wrEmmCb.memSample = 0;

   RETVALUE(ROK);
}


/**
 *  @brief This functoin decides the overload state.

    @details:

        Function : wrEmmUpdOvldSt

            Processing State: This function based upon the passed CPU and memory load 
            and configured threshold values decides the overload status of eNodeB.
     @param[in] cpuLoad: Average CPU load over the sampling period       
     @param[in] memStatus: Average Memory loadd over the sampling period.
     @param[out] ovldFlag: eNodeB Overload status 
 *   @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
*/
PRIVATE S16 wrEmmUpdOvldSt(U32 cpuLoad, U32 memStatus,U8 *ovldFlag)
{
   WrEmmCb *emmCb;
   emmCb = &wrEmmCb;

   if ((memStatus >= emmCb->eNBOvldParams.eNBMemCritThresVal)
         || ( cpuLoad >= emmCb->eNBOvldParams.critThrldVal ))
   {
      *ovldFlag = CRITICAL;
   }
   else if((memStatus >= emmCb->eNBOvldParams.eNBMemMajThresVal)
         || (cpuLoad >= emmCb->eNBOvldParams.majThrldVal))
   {
      *ovldFlag = MAJOR;
   }
   else if ((memStatus >= emmCb->eNBOvldParams.eNBMemMinThresVal)
         || ( cpuLoad >= emmCb->eNBOvldParams.minThrldVal ))
   {
     *ovldFlag = MINOR;
   }
   else
   {
      *ovldFlag = NORMAL;
   }

   RETVALUE(ROK);
}




/**
 *  @brief  This function is used to update overload related paramters. 
 *    
 *  @details:
 *          Function :wrProcessOvldFlag
 *              Processing Steps:
 *                 This function compares current overload status with previous 
 *                 status and updates the counter value. In case the old and current
 *                 overload status doesn't match counter is reset and current 
 *                 overload status is assigned to oldstatus.
 *
 *  @param[in] ovldFlag : Current Overload status                   
 *  @return  S16
 *       -# Success : ROK
 *       -# Failure : RFAILED
*/
PRIVATE S16 wrProcessOvldFlag
(
U8  ovldFlag
)
{
    if (ovldFlag == wrEmmCb.eNBOvldParams.prevOvldFlag)
    {
      wrEmmCb.eNBOvldParams.ovldCntVal++;
    }
    else
    {
       wrEmmCb.eNBOvldParams.ovldCntVal = 0;
       wrEmmCb.eNBOvldParams.ovldCntVal++;
       wrEmmCb.eNBOvldParams.prevOvldFlag = ovldFlag;
    }
    RETVALUE(ROK);
}



/**
 *  @brief This function is used to set access barring parameters based upon the overload state
 * 
 *   @details
 *       
 *       Function: wrSetACBarringParams
 *           Processing Steps: This function updates access barring parameters for a given Cell's SIB2
 *           based upon the current overload status.
 *
 *   @param[in] ovldFlag
 *   @param[in] cellCb
 *
 *   @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
*/
PUBLIC S16 wrSetACBarringParams
(
U8 ovldFlag,
WrCellCb *cellCb
)
{
   WrSib2Params              *sib2 = &(cellCb->sib2);
   U8                        sibs[WR_MAX_SIBS];
   U8                        numSibs = 0;
   static U8                 count;

   if (wrEmmCb.eNBOvldParams.prevOvldFlag == ovldFlag)
   {
     /*No nned to set the AC Barring params */
      if (count <= wrEmmCb.eNBOvldParams.eNBOvldMonfreq)
      {
         count++;
         RETVALUE(ROK);
      }
      else
      {
         count = 0;
      }
   }

   sib2->barringInfoPres = PRSNT_NODEF;
   sib2->barringInfo.signalling.pres = PRSNT_NODEF;
   sib2->barringInfo.dataCall.pres = PRSNT_NODEF;

   if (ovldFlag == CRITICAL)
   {
     sib2->barringInfo.emergency = TRUE;
     sib2->barringInfo.signalling.acBarringFactor =  WR_SIB2_ACBRFTR0;
     sib2->barringInfo.signalling.acBarringTime =    WR_SIB2_ACBRTIME128;
     sib2->barringInfo.signalling.acBarringForSpecialAC = 0x1f;
     sib2->barringInfo.dataCall.acBarringFactor =  WR_SIB2_ACBRFTR0;
     sib2->barringInfo.dataCall.acBarringTime =    WR_SIB2_ACBRTIME128;
     sib2->barringInfo.dataCall.acBarringForSpecialAC = 0x1f;
   }
   else if (ovldFlag == MAJOR)
   {
     sib2->barringInfo.emergency = FALSE;
     sib2->barringInfo.signalling.acBarringFactor =  WR_SIB2_ACBRFTR90;
     sib2->barringInfo.signalling.acBarringTime =    WR_SIB2_ACBRTIME32;
     sib2->barringInfo.signalling.acBarringForSpecialAC = 0x1f;
     sib2->barringInfo.dataCall.acBarringFactor =  WR_SIB2_ACBRFTR70;
     sib2->barringInfo.dataCall.acBarringTime =    WR_SIB2_ACBRTIME32;
     sib2->barringInfo.dataCall.acBarringForSpecialAC = 0x1f;
   }
   else if (ovldFlag == MINOR)
   {
     sib2->barringInfo.emergency = FALSE;
     sib2->barringInfo.signalling.acBarringFactor =  WR_SIB2_ACBRFTR95;
     sib2->barringInfo.signalling.acBarringTime =    WR_SIB2_ACBRTIME16;
     sib2->barringInfo.signalling.acBarringForSpecialAC = 0x0;
     sib2->barringInfo.dataCall.acBarringFactor =  WR_SIB2_ACBRFTR80;
     sib2->barringInfo.dataCall.acBarringTime =    WR_SIB2_ACBRTIME16;
     sib2->barringInfo.dataCall.acBarringForSpecialAC = 0x0;
   }
   else
   {
     sib2->barringInfo.emergency = FALSE;
     sib2->barringInfo.signalling.acBarringFactor =  WR_SIB2_ACBRFTR50;
     sib2->barringInfo.signalling.acBarringTime =    WR_SIB2_ACBRTIME4;
     sib2->barringInfo.signalling.acBarringForSpecialAC = 0x0;
     sib2->barringInfo.dataCall.acBarringFactor =    WR_SIB2_ACBRFTR50;
     sib2->barringInfo.dataCall.acBarringTime =      WR_SIB2_ACBRTIME4;
     sib2->barringInfo.dataCall.acBarringForSpecialAC = 0x0;
   }

   RLOG_ARG2(L_DEBUG, DBG_CELLID,cellCb->cellId,"CPU overload status changed "
      "ac-BarringFactor modified, signalling ac-BarringFactor[%d] dataCall "
      "ac-BarringFactor[%d]", sib2->barringInfo.signalling.acBarringFactor,
      sib2->barringInfo.dataCall.acBarringFactor);

   sibs[numSibs++] = WR_SIB_2;

   if(ROK != wrPbmAddOrUpdateSibs(numSibs, sibs, cellCb->cellId))
   {
      RLOG0(L_ERROR,"PBM Updation is Failed ");
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
   RLOG0(L_INFO," ENB OverLoaded,SIB parameters are set accordingly;"
      "please observe the same in SIB2");

   wrEmmCb.eNBOvldParams.prevOvldFlag = ovldFlag;
   RETVALUE(ROK);
}
/**
 *  @brief This function is used to collect CPU sample
 *
 *  @details
 *       
 *     Function: wrCalcCPULoad
 *         Processing Steps: This function open /ect/stat file to read current CUP usage 
 *         parameters. based upon the current parameters and last saved parameters 
 *         Current CPU load is estimated.
 *
 *   @param[out] cpuLoad: 
 *   @param[in] CpuParam:
 *
 *  @return  S16
 *       -# Success : ROK
 *       -# Failure : RFAILED
*/
PRIVATE U16 wrCalcCPULoad
(
U32 *cpuLoad,
WrCpuParam *CpuParam
)
{
#define STATS "/proc/stat"
#define PCNT_VAL(m,n,p) ((100 * ((m) - (n))) / p)

   FILE *stats_file;
   char buf[80];
   WrCpuParam curCpuUsage = {0};

   if ((stats_file = fopen(STATS, "r")) == NULL)
   {
      fprintf(stderr, "Cannot open %s.", STATS);
      RETVALUE(RFAILED);
   }

   memset(buf, 0, 80);
   while (fgets(buf, 80, stats_file) != NULL)
   {
      if (strncmp(buf, "cpu ", 4) == 0)
      {
         sscanf(buf + 5, "%u %u %u %lu",
               &curCpuUsage.usr_time, &curCpuUsage.nice_time, &curCpuUsage.sys_time, &curCpuUsage.idle_time);
         curCpuUsage.uptime = curCpuUsage.usr_time + curCpuUsage.nice_time + curCpuUsage.sys_time + curCpuUsage.idle_time; 
         break;
      }
   }

  /* *cpuLoad = 100 - PCNT_VAL(curCpuUsage.idle_time, CpuParam->idle_time, curCpuUsage.uptime - CpuParam->uptime);*/

/*   *cpuLoad = ((1000*((curCpuUsage.uptime - CpuParam->uptime) - (curCpuUsage.idle_time - CpuParam->idle_time))/((curCpuUsage.idle_time - CpuParam->idle_time) + 5))/10);*/

   *cpuLoad = 100 * (curCpuUsage.usr_time - CpuParam->usr_time) / (curCpuUsage.uptime - CpuParam->uptime );
   *cpuLoad = *cpuLoad + (100 * (curCpuUsage.sys_time - CpuParam->sys_time) /(curCpuUsage.uptime - CpuParam->uptime ));
   memcpy((void*)CpuParam,(void*)&curCpuUsage, sizeof(WrCpuParam));

   fclose(stats_file);
   RETVALUE(ROK);
}

/**
 *  @brief This function is used to collect memory sample
 *
 *  @details
 *   Function: wrCalcMemUsage 
 *       Processing Steps: This function invokes the SSI functionality to inquire current memory status.
 *
 * @param[out] memStatus Memory usage statistics as reported by SSI
 * @return  S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
*/
PRIVATE S16 wrCalcMemUsage
(
  S16 *memStatus
)
{
   /*TODO: Memory Usage  function*/
   S16        retVal = 0;               /* Return Value */

   /**************************************
    * get the memory status of the Layer *
    **************************************/
   retVal = SChkRes(wrCb.init.region, wrCb.init.pool, memStatus);
   if (retVal != ROK)
   {
     RLOG0(L_ERROR,"Getting memory statistics failed ");
     RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/**
 *  @brief This function provides interface towards OAM to facilitate modification of 
 *         Cell Access and Baring Related Parameters. 
 *  @details
 *      Function : wrEmmOamCfg
 *          Processing Steps: This function is used to update Access Barring parameters by OAM.
 *          Based upon the type of update request, relevant system imformation block parameters are 
 *          modified to reflect OAM configurations.
 *
 *  @param[in]  oamParam OAM specific Access barring parameters
 *  @return  S16
 *       -# Success : ROK
 *       -# Failure : RFAILED
*/
PUBLIC S16  wrEmmOamCfg(WrOamAccessBarring  *oamParam)
{
   U8                        updType;
   U32                       TEST;
   U16                       cellId;
   WrCellCb                  *cellCb = NULLP;
   WrSib1Params              *sib1;
   WrSib2Params              *sib2;

   if(NULLP == oamParam)
   {
      RLOG0(L_ERROR,"NULL parameter passed from OAM ");
      RETVALUE(RFAILED);
   }

   updType = oamParam->updType;
   cellId = oamParam->cellId;
   WR_GET_CELLCB(cellCb, cellId);
   if(cellCb == NULLP)
   {
      RLOG1(L_ERROR,"Incorrect CellId=[%d] passed from OAM ",cellId);
      RETVALUE(RFAILED);
   }

   switch(updType)
   {
      case WR_OAM_CELL_BARING:
         sib1 = &cellCb->sib1;
         sib1->cellBarred = oamParam->u.cellBarred;
         break;
      case WR_OAM_ENB_OVRLD_MAN:
         sib2 = &cellCb->sib2;
         sib2->barringInfo.emergency = oamParam->u.barringInfo.emergency;
         sib2->barringInfo.signalling.acBarringFactor =  oamParam->u.barringInfo.signalling.acBarringFactor;
         sib2->barringInfo.signalling.acBarringTime   =  oamParam->u.barringInfo.signalling.acBarringTime;
         sib2->barringInfo.dataCall.acBarringFactor   =  oamParam->u.barringInfo.dataCall.acBarringFactor;
         sib2->barringInfo.dataCall.acBarringTime     =  oamParam->u.barringInfo.dataCall.acBarringTime;
         break;
      case WR_OAM_1RTT_ACC_BARING:
         TEST = oamParam->u.RTT.acBarr0to9;
         TEST = oamParam->u.RTT.acBarr10;
         TEST = oamParam->u.RTT.acBarr11;
         TEST = oamParam->u.RTT.acBarr12;
         TEST = oamParam->u.RTT.acBarr13;
         TEST = oamParam->u.RTT.acBarr14;
         TEST = oamParam->u.RTT.acBarr15;
         TEST = oamParam->u.RTT.acBarrMsg;
         TEST = oamParam->u.RTT.acBarrReg;
         TEST = oamParam->u.RTT.acBarrEmg;
         break;
      default:
         RLOG1(L_ERROR,"Incorrect UpdateType=[%d] passed from OAM ",updType);
         RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}
#endif




/********************************************************************30**

           End of file:    $SID$

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
$SID$        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/

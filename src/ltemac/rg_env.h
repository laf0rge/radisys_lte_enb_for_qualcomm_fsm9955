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



/**********************************************************************
 
    Name:   LTE MAC layer
 
    Type:   H include file
 
    Desc:   Defines required by LTE MAC
 
    File:   rg_env.h
 
    Sid:      rg_env.h@@/main/3 - Sat Jul 30 02:21:58 2011
 
    Prg:    apany
 
**********************************************************************/

 

/*
*     The defines in this file correspond to structures and variables 
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*     1000372       LTE MAC
*
*/
 
#ifndef __RGENVH__
#define __RGENVH__

/* Number of Unserved UE's Facotr for BI value calculation */
/* R8 Upgrade */
#define RG_SCH_CMN_BI_NUMUE_FACTOR    1
#define RGSCH_MAX_RNTI_PER_RARNTI    10
/* The number of schedulers supported currently */ 
#ifdef RG_PHASE2_SCHED
#define RGSCH_NUM_SCHEDULERS    4
#define RGSCH_NUM_DLFS_SCHEDULERS    1
#else
#define RGSCH_NUM_SCHEDULERS    1
#define RGSCH_NUM_DLFS_SCHEDULERS    0
#endif
/* Added support for SPS*/

#ifdef LTEMAC_SPS
#define RG_SCH_NUM_SPS_OCC_AFTR_EXP_REL 6 /* Number of empty SPS Occasions
                                             after sending explicit release after
                                             which SPS resources of the UE shall 
                                             be deallocated */
#define RGSCH_SPS_CQI_SCALE_FACTOR 1
#endif


/* List of scheduler init functions to initialise rgSchSchdInits[] array with */
#ifdef RG_PHASE2_SCHED
#define RGSCH_ULSCHED_INITS  {rgSCHRrUlInit, rgSCHRrUlInit, rgSCHRrUlInit, rgSCHRrUlInit}
#define RGSCH_DLSCHED_INITS  {rgSCHRrDlInit, rgSCHRrDlInit, rgSCHRrDlInit, rgSCHRrDlInit}
#ifdef TFU_UPGRADE
#define RGSCH_DLFSSCHED_INITS {rgSCHDlfsInit}
#endif
#else
#define RGSCH_ULSCHED_INITS  {rgSCHSc1UlInit}
#define RGSCH_DLSCHED_INITS  {rgSCHSc1DlInit}
#define RGSCH_DLFSSCHED_INITS {}
#endif



/* Common Scheduler Tunable Parameters */
#define RG_SCH_CMN_UE_IDLETIME_FCTR        3
/* QCOM Changes: 
   Changes for allocating all 96 RBs in UL scheduling. 
   Earlier it was allocating only 90 RB eventhough with high mcs values */
#define RG_SCH_CMN_MAX_BITS_RATIO          1
#define RG_SCH_CMN_UL_COM_DENOM            1 
#define RG_SCH_CMN_UL_NUM_CQI              16
#define RG_SCH_CMN_MAX_UE_PER_UL_SF        1 /* If more than 1 is required it can be controlled from configuration*/
/* Fix: MUE_PERTTI_DL*/
#define RG_SCH_CMN_MAX_UE_PER_DL_SF       4
/* Mapping RG_MAX_NUM_UE_PER_TTI to RGU Interface define RGU_MAX_NUM_UE_PER_TTI
 * to use it in MAC code, If you  change the value of below macro then you have
 * to change value of RGU_MAX_NUM_UE_PER_TTI in rgu.h, both should be same */
#define RG_MAX_NUM_UE_PER_TTI              8
/* Added configuration for maximum number of MSG3s */
#define RG_SCH_CMN_MAX_MSG3_PER_UL_SF      1 /* If more than 1 is required it can be controlled from configuration*/
#define RG_SCH_CMN_MAX_UL_BW_PER_UE        100
#define RG_SCH_CMN_MAX_DL_RETX_BW          100 
#define RG_SCH_CMN_MAX_DL_BW_PERUE         100 
#define RG_SCH_CMN_DEF_BCCHPCCH_CODERATE   512
#define RG_SCH_CMN_MAX_DL_AMBR             0xFFFFFFFF 
#define RG_SCH_CMN_MAX_UL_UEBR             0xFFFFFFFF 
#define RG_SCH_CMN_DED_MAX_HDRSIZE         3
#define RG_SCH_CMN_MAX_DED_SDU             5

#ifdef MAC_SCH_STATS
#define RG_SCH_CMN_CMNDL_DELTA             1
#endif /* MAC_SCH_STATS */

/* ccpu00126002 introduced buffer to track cqi allocation and RI allocation
   for a particular UE. As the allocation is stored in UeCb, if the 
   occasion between cqi and ri are short cqi is overwritten. To avoid that
   this buffer is introduced.*/
#define MAX_CQI_RI_RPT_BUFF           (2 * 2)

/* GBR priorities occupy a set of contiguous priorities      */
/* starting always at 1.                                     */
/* RG_SCH_CMN_MAX_PRIO is the total number of priority queues    */
/* defined in the scheduler. This variable can have affect   */
/* on the performance of the scheduler and should be chosen  */
/* based on the same.                                        */
#define RG_SCH_CMN_MAX_PRIO              8

#ifdef RG_SC1
/* UL Scheduler1 tunable params */
#define RG_SC1_BSR_BS         4
#define RG_SC1_SR_BS          RG_SC1_BSR_BS
#define RG_SC1_UL_RATIO       14  /* UL_RATIO/COM_DENOM of bs for LCGs other than */
                                /* highest priority LCG used */

/* Priority 0 is used as a FIFO service where the priority   */
/* of UEs within the priority Q is based purely on time at   */
/* which the service is reported to the scheduler. This      */
/* priority queue is suitable for DCCH services.             */


/* This is the maximum known QCIs to the scheduler.          */
/* RG_SC1_QCI_TO_PRIO maps QCIs to respective priorities and */
/* there by behavior of the service                          */

/* This table is used to translate a CQI to an applicable */
/* aggregation level to be used for a UE when allocating  */
/* a PDCCH.                                               */
/* This table is used to translate a CQI to an applicable  */
/* coding rate for dedicated PDSCH allocation. The number  */
/* in the table represents the number of bits to allocated */
/* per 1024 REs.                                           */

#endif
#define RGSCH_MAX_UE_PER_DL_SF 12
#define RGSCH_MAX_RARNTI_PER_DL_SF 4
#define  RGSCH_INST_START 1
#define RGSCH_MAX_INST 1


/* Twice of difference in power levels between successive uplink
 * CQIs (in dB) */
#define RG_SCH_UL_CQI_DB_STEP_2   2

#define RG_SCH_CMN_MAX_NUM_TPC_PUCCH_RNTI 100
#define RG_SCH_CMN_MAX_NUM_TPC_PUSCH_RNTI 100

/* [ccpu00138532]-DEL-Removed the Guard timer macro. Moved
   it into CellCb and value is configured based the maxMsg4Tx */

/* moving rgSchCmnUlCqiTbl values here to enable customer to 
 * fine tune these values 
 */
/* Adding modulation order & efficiency hash defines for
 * UL. This can be tuned by customer. These values are used
 * in rgSchCmnUlCqiTbl in rg_sch_cmn.c
 */

/* for CQI 1 */
#define RGSCH_CMN_QM_CQI_1      RGSCH_QM_BPSK
#define RGSCH_CMN_UL_EFF_CQI_1      156

/* for CQI 2 */
#define RGSCH_CMN_QM_CQI_2      RGSCH_QM_BPSK
#define RGSCH_CMN_UL_EFF_CQI_2      240 

/* for CQI 3 */
#define RGSCH_CMN_QM_CQI_3      RGSCH_QM_BPSK
#define RGSCH_CMN_UL_EFF_CQI_3      386 

/* for CQI 4 */
#define RGSCH_CMN_QM_CQI_4      RGSCH_QM_BPSK
#define RGSCH_CMN_UL_EFF_CQI_4      616

/* for CQI 5 */
#define RGSCH_CMN_QM_CQI_5      RGSCH_QM_BPSK
#define RGSCH_CMN_UL_EFF_CQI_5      898

/* for CQI 6 */
#define RGSCH_CMN_QM_CQI_6      RGSCH_QM_BPSK
#define RGSCH_CMN_UL_EFF_CQI_6      1204 

/* for CQI 7 */
#define RGSCH_CMN_QM_CQI_7      RGSCH_QM_QPSK
#define RGSCH_CMN_UL_EFF_CQI_7      1512 

/* for CQI 8 */
#define RGSCH_CMN_QM_CQI_8      RGSCH_QM_QPSK
#define RGSCH_CMN_UL_EFF_CQI_8      1960 

/* for CQI 9 */
#define RGSCH_CMN_QM_CQI_9      RGSCH_QM_QPSK
#define RGSCH_CMN_UL_EFF_CQI_9      2464 

/* for CQI 10 */
#define RGSCH_CMN_QM_CQI_10      RGSCH_QM_64QAM
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
#define RGSCH_CMN_UL_EFF_CQI_10      2796
#else
#define RGSCH_CMN_UL_EFF_CQI_10      3402
#endif /* TENB_T2K3K_SPECIFIC_CHANGES */

/* for CQI 11 */
#define RGSCH_CMN_QM_CQI_11      RGSCH_QM_64QAM
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
#define RGSCH_CMN_UL_EFF_CQI_11      3402
#else
#define RGSCH_CMN_UL_EFF_CQI_11      3996 
#endif /* TENB_T2K3K_SPECIFIC_CHANGES */

/* for CQI 12 */
#define RGSCH_CMN_QM_CQI_12      RGSCH_QM_64QAM
#define RGSCH_CMN_UL_EFF_CQI_12      3996 

/* for CQI 13 */
#define RGSCH_CMN_QM_CQI_13      RGSCH_QM_64QAM
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
#define RGSCH_CMN_UL_EFF_CQI_13      4632 
#else
#define RGSCH_CMN_UL_EFF_CQI_13      4332
#endif /* TENB_T2K3K_SPECIFIC_CHANGES */

/* for CQI 14 */
#define RGSCH_CMN_QM_CQI_14      RGSCH_QM_64QAM
#define RGSCH_CMN_UL_EFF_CQI_14      5238 

/* for CQI 15 */
#define RGSCH_CMN_QM_CQI_15      RGSCH_QM_64QAM
#define RGSCH_CMN_UL_EFF_CQI_15      5728 

/* Freeing up the HARQ proc blocked for
 * indefinite time in case of Retx */
#define RG_SCH_MAX_RETX_ALLOC_FAIL  10 

/* Window Size within which buffering paging and Broadcast BO is done */
#define RGSCH_PCCHBCCH_WIN 5120
#define RGSCH_MAX_SUBFRM         10240
#define RGSCH_UL_SYM_DMRS_SRS      3
#define RGSCH_UL_16QAM_MAX_ITBS    20
#ifdef LTE_TDD
/* Itbs Adjustment for DwPts scheduling. Only 
 * Spl Sf cfg 7 is currently supported. The adjustment 
 * is based on simulation results */
#define RG_SCH_DWPTS_ITBS_ADJ {0,0,0,0,0,0,0,-1,0}
#endif


/* The below MACROs are used by the BLER based LA algorithm to 
 * maintain the iTbs delta based on the HARQ feedback. For the first 
 * HARQ Ack received, the delta itbs value is incremented by the STEP_UP value.
 * For the first HARQ Nack received, the delta iTbs value is decreased by
 * the STEP_DOWN value 
 */ 
#ifdef DL_LA
#define DL_LA_STEPDOWN           30 
#define DL_LA_STEPUP              3
#endif

#ifdef UL_LA
#define UL_LA_STEPDOWN           30 
#define UL_LA_STEPUP              3
#endif

#define RGSCH_CFI_TTI_MON_INTRVL        1000 /* In TTI, Max value 10000 */  
#define RGSCH_CFI_STEP_UP_TTI_PRCNTG    10   /* Percentage of CCE failures */
#define RGSCH_CFI_STEP_DOWN_TTI_PERCNTG 90   /* Percentage of TTI in which CCE usage is less than the
                                                next lower CFI */
#define RGSCH_CFI_CCE_PERCNTG           90   /* Percentage of total CCE used/total CCE available 
                                                in the next lower CFI */

/* Following tunable Macros are applied in APerCQI
 * trigger determination */
#define RG_APER_CQI_ACK_WGT       25  /*Weight Associated with each HARQ ACK*/
#define RG_APER_CQI_NACK_WGT      125 /*Weight Associated with each HARQ ACK*/
#define RG_APER_CQI_THRESHOLD_WGT 1000/*Threshold against which the accumulated
                                        aCqiTrigWt is compared to */

#define RG_SCH_ITBS_STEP_DOWN_PCQI_RI_HQ 1   /* iTbs will be reduced by this value 
                                              for UL with PCQIi/RI/HQ */
#define RG_SCH_ITBS_STEP_DOWN_ACQI       2   /* iTbs will be reduced by this value 
                                                for ACQI report*/
#define RG_SCH_PCQI_RI_HQ_EFF_TGT        930 /* Target efficeincy for 
                                                UL with PCQI/RI/HQ  */
#define RG_SCH_ACQI_EFF_TGT              530 /* Target efficeincy for UL with ACQI */ 

/* Following #defines are used in calculating the cqi to TBS mapping 
 * In 36.213 table 7.1.7.1-1 defines mapping b/w Itbs and mcs and table 
 * 7.2.3-1 defines mapping b/w CQI to mcs and code rate.
 * The values of these tables are chosen such that average of tbs value for 
 * different nprbs with code rate nearest to the code rate defined in table
 * 7.2.3-1 for particular CQI value.We should also take care of the fact that 
 * mapping defined in table 7.1.7.1-1 is not overruled.*/
/* ADD fix for CFI0*/
#define RG_SCH_CMN_CQI_TO_PDSCH_EFF_CFI0 \
    {207, 271, 333, 431, 648, 899, 1161, 1482, 1889, 2103,\
        2905, 3412, 3956, 4474, 4655, 5397}

#if 0 /* Changed the Efficiency tables for CFI 2 and 3 */  
#define RG_SCH_CMN_CQI_TO_PDSCH_EFF_CFI1 \
    {178, 222, 296, 364, 577, 856, 1125, 1411, 1833, 2460,\
        2612, 3453, 4026, 4595, 5090, 5902}

#define RG_SCH_CMN_CQI_TO_PDSCH_EFF_CFI2 \
    {175, 255, 326, 400, 636, 944, 1240, 1556, 2021, 2536, 2881,\
        3504, 4116, 4772, 5398, 5616}

#define RG_SCH_CMN_CQI_TO_PDSCH_EFF_CFI3 \
    {208, 295, 362, 446, 708, 869, 1207, 1734, 1991, 2540,\
        3212, 3561, 4248, 4590, 5322, 5653}
#else
/* The below tables are defined to fix the BLER issue
   for CFI 2, 3, 4 */
#define RG_SCH_CMN_CQI_TO_PDSCH_EFF_CFI1 \
    {226, 226, 226, 364, 471, 708, 983, 1411, 1833, 2299, 2612,\
        3177, 3731, 4326, 4893, 5090}

#define RG_SCH_CMN_CQI_TO_PDSCH_EFF_CFI2 \
    {248, 248, 248, 326, 519, 780, 1084, 1399, 1786, 2278, 2714, 3194,\
        3810, 4442, 5069, 5398}

#define RG_SCH_CMN_CQI_TO_PDSCH_EFF_CFI3 \
    {276, 276, 276, 276, 446, 708, 1052, 1382, 1734, 2254, 2827,\
        3212, 3908, 4590, 4953, 5653}

#endif        
/* Number of information bits per 1024 Phy bits for PDCCH */
/* This is a customer tunable */
#define RG_SCH_CMN_CQI_TO_PDCCH_EFF \
       {  400,  100,  150,  200,  250,  280, 340, 365,\
         380, 400, 500, 600, 700, 800, 900, 1000}

#define RG_SCH_CMN_2LYR_CQI_TO_PDSCH_EFF_CFI0 \
    {188, 226, 309, 366, 412, 539, 662, 859, 1052, \
        1293, 1535, 2585, 2957, 3340, 4775, 5300}

#define RG_SCH_CMN_2LYR_CQI_TO_PDSCH_EFF_CFI1 \
    {197, 232, 306, 322, 408, 450, 588, 723, 938,\
        1150, 1413, 1679, 2826, 3233, 5222, 5796}

#define RG_SCH_CMN_2LYR_CQI_TO_PDSCH_EFF_CFI2 \
    {166, 207, 233, 307, 343, 408, 422, 495, 586,\
        648, 797, 1035, 1268, 1558, 3117, 3567}

#define RG_SCH_CMN_2LYR_CQI_TO_PDSCH_EFF_CFI3 \
    {177, 200, 221, 287, 323, 386, 433, 497, 522, 676,\
        722, 888, 1153, 1413, 1737, 3476}

#endif /* __RGENVH__ */


/**********************************************************************
         End of file:     rg_env.h@@/main/3 - Sat Jul 30 02:21:58 2011
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---   apany 1. Initial Release.
/main/2      ---   apany 1. LTE MAC 2.1 release
             rg001.201 sm 1. Removed extra comments. 
             rg005.201 sm 1. Fix for CR ccpu00111069
             rg006.201 sd 1. ccpu00112789: Added MACRO for default maximum
                             MSG3 per UL SF.
             rg008.201 rsharon 1.Added support for SPS.
             rg009.201 rsharon 1.[ccpu00112372]Changes for CR timer.
                       dv      2.[ccpu00116499]RG_UL_DELTA defined only if not defined
/main/3      ---         gvj 1. Updated for LTE MAC Rel 3.1
             rg001.301 asehgal 1.Changes for the ticket ccpu00117270 .
             rg002.301 asehgal 1.ccpu00118351-seperate deltas for UL & DL.
             rg003.301 hsingh  1. ccpu00120794-Freeing up the HARQ proc blocked 
                                  for indefinite time in case of Retx
$SID$        ---       rt   1. LTE MAC 4.1 Release.                           
*********************************************************************91*/

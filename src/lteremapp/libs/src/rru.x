/********************************************************************16**

                         (c) COPYRIGHT 2006 by 
                         Continuous Computing Corporation.
                         All rights reserved.

     This software is confidential and proprietary to Continuous Computing 
     Corporation (CCPU).  No part of this software may be reproduced,
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written Software License 
     Agreement between CCPU and its licensee.

     CCPU warrants that for a period, as provided by the written
     Software License Agreement between CCPU and its licensee, this
     software will perform substantially to CCPU specifications as
     published at the time of shipment, exclusive of any updates or 
     upgrades, and the media used for delivery of this software will be 
     free from defects in materials and workmanship.  CCPU also warrants 
     that has the corporate authority to enter into and perform under the   
     Software License Agreement and it is the copyright owner of the software 
     as originally delivered to its licensee.

     CCPU MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
     MATERIALS.

     IN NO EVENT SHALL CCPU BE LIABLE FOR ANY INDIRECT, SPECIAL,
     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
     OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
     OF THE POSSIBILITY OF SUCH DAMAGE.

                       Restricted Rights Legend

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
     forth in the written Software License Agreement between CCPU and
     its Licensee. Among other things, the use of this software
     may be limited to a particular type of Designated Equipment, as 
     defined in such Software License Agreement.
     Before any installation, use or transfer of this software, please
     consult the written Software License Agreement or contact CCPU at
     the location set forth below in order to confirm that you are
     engaging in a permissible use of the software.

                    Continuous Computing Corporation
                    9380, Carroll Park Drive
                    San Diego, CA-92121, USA

                    Tel: +1 (858) 882 8800
                    Fax: +1 (858) 777 3388

                    Email: support@trillium.com
                    Web: http://www.ccpu.com

*********************************************************************17*/


/**********************************************************************
 
    Name:   RRC layer
 
    Type:   C include file
 
    Desc:   Defines required by RRC user
 
    File:   rru.x
 
    Sid:      rru.x@@/main/16 - Wed May 22 15:03:14 2013
 
    Prg:    vk
 
**********************************************************************/
 

/*
*     The defines in this file correspond to structures and variables 
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*     1000206       RRC
*
*/

 
 
#ifndef __RRUX__
#define __RRUX__

/* include files */
#include "rru_asn.x"         /* ASN.1 definitions */
#include "cm_umts.x"       /* common UMTS related definitions */
#include "cm_mblk.x"       /* common memory allocation */

typedef U8 RruUmtsRbId;          /* Radio Bearer ID */

typedef U32 RruUmtsUrnti;        /* Urnti UE Id */

/*rru_x_001.main_9*/
typedef enum rruIsKeyNew
{
  RRU_NEW_KEY,
  RRU_OLD_KEY
} RruIsKeyNew;


/* As per "6.2.7.12 Paging Indication bitmap (PI-bitmap)" 
 * in 25.435. This information will be transparently passed
 * to FP via RRC, RLC and MAC
*/
typedef struct rruPiBitMap
{
   U8  len;
   U8  map[RRU_MAX_PI_BITMAP_LEN];
} RruPiBitMap;

typedef struct rruDlPcchMsg
{
   U16           po;             /* Paging Occasion (SFN) */
   RruPiBitMap   piBitMap;       /* Page Indicator */
   RruPCCH_Msg   pcchMsg;        /* Paging Message */
} RruDlPcchMsg;

typedef struct rruDatReqUeMsg      /* UE Message */
{
   U8         msgCategory;
   U8         cnDmn;       /* CS/PS for Integrity Protection */
   union
   {
      RruBCCH_FACH_Msg   dlBcchFachMsg; /* DL-BCCH SDU */
      RruDlPcchMsg       dlPcchMsg;     /* DL-PCCH SDU */
      RruDL_CCCH_Msg     dlCcchMsg;     /* DL-CCCH SDU */
      RruDL_DCCH_Msg     dlDcchMsg;     /* DL-DCCH SDU */
   } m;
} RruDatReqUeMsg;

typedef struct rruRbActInfo
{
   U32      hfn;  /* The same shall contain macDHfn for MAC in case of TM RBs*/
   U16      actTime; /* DL Activation time */
} RruRbActInfo;

typedef struct rruAmUmRbSecInfo
{
   RruRbActInfo   actInfo;
   U16            count;  /* Count required for suspend request */
} RruAmUmRbSecInfo;

typedef struct rruTmRbSecInfo
{
   RruRbActInfo   actInfo;
} RruTmRbSecInfo;


typedef struct rruRbSecInfo
{
   RruUmtsRbId    rbId;
   U8             choice;  /* AM/UM or TM */
   union 
   {
      RruAmUmRbSecInfo     amUmRbSecInfo;
      RruTmRbSecInfo       tmRbSecInfo;
   }m;
} RruRbSecInfo;

typedef struct rruCiphKeyAlgoInfo
{
   U8             ciphAlgo; /* Ciphering Algorigthm */
   U8             cipherKey[RRU_CIPH_KEY_LEN]; /* Ciphering Key */ 
} RruCiphKeyAlgoInfo;

typedef struct rruCiphInfo
{
   TknPres              pres;
   U8                   dmnMsk;    /* CS/PS */
   RruCiphKeyAlgoInfo   *keyAlgo;  /* CS/PS */
   U8                   numRbs;           /* Number of RBs */
   RruRbSecInfo         rruRbSecInfo[RRU_MAX_RB_PER_RRC_CONN];/* RB Related security Information */
} RruCiphInfo;


typedef struct rruIntgStartInfo
{
   U32      fresh; /* Fresh */
} RruIntgStartInfo;

typedef struct rruIntgModInfo
{
   U32           dlCountI[RRU_MAX_INTG_SRBS];
} RruIntgModInfo;

/*rru_x_001.main_11 Update the integrity info at TRNC after SRNC relocation */
typedef struct rruIntgRelocInfo
{
   U32           ulCountI[RRU_MAX_INTG_SRBS];
   U32           dlCountI[RRU_MAX_INTG_SRBS];
   U32           fresh; /* Fresh */
} RruIntgRelocInfo;

typedef struct rruSrbIntgInfo
{
   U8          intgOpt; /* Start or Modify */
   union
   {
      RruIntgStartInfo   start;
      RruIntgModInfo     modify;
      /*rru_x_001.main_11 Update the integrity info at TRNC after SRNC relocation */
      RruIntgRelocInfo   reloc;
   } u;   
} RruSrbIntgInfo;


typedef struct rruIntgProtInfo
{
   TknPres        pres;
   TknU8          intgProtAlgo;
   U8             cnDmn;   /* CS/PS */
   U8             intgKey[RRU_INTG_KEY_LEN];
   RruSrbIntgInfo srbIntgInfo;  
} RruIntgProtInfo;

typedef struct rruSecCfg /* SDU */
{
   RruCiphInfo     ciphInfo; /* Ciphering Configuration */
   RruIntgProtInfo intgProtInfo; /* Integrity Configuration */
   /*rru_x_001.main_9 Addition of a  Enum isKeyNew */ 
   RruIsKeyNew     isKeyNew; /*Value is RRU_NEW_KEY if new CK and IK 
                               have been generated else value is RRU_OLD_KEY*/
} RruSecCfg;



typedef enum rruLogChType    /* Logical Channel type */
{
   RRU_LOGCH_BCCH,
   RRU_LOGCH_PCCH,
   RRU_LOGCH_CCCH,
   RRU_LOGCH_CTCH,
   RRU_LOGCH_DCCH,
   RRU_LOGCH_DTCH,
   RRU_LOGCH_MCCH,
   RRU_LOGCH_MSCH,
   RRU_LOGCH_MTCH
} RruLogChType;

typedef struct rruLogChId    /* Logical Channel Id */
{
   RruLogChType logChType;  /* Channel Type */
   U8           inst;       /* Logical Channel Instance */
} RruLogChId;

typedef enum rruTrChType    /* Transport Channel type */
{
   RRU_TRCH_BCH,
   RRU_TRCH_PCH,
   RRU_TRCH_FACH,
   RRU_TRCH_RACH,
   RRU_TRCH_DL_DCH,
   RRU_TRCH_UL_DCH,
   RRU_TRCH_E_DCH,
   RRU_TRCH_HS_DSCH
} RruTrChType;

typedef struct rruTrChId    /* Transport Channel Id */
{
   RruTrChType  trchType;      /* Transport Channel Type */
   U16          trchIdentity;  /* Transport Channel identity */
} RruTrChId;

typedef struct rruTbInfo
{
   U16   tbSize;
   U16   numofTb;
} RruTbInfo;

typedef struct rruRbRlcSizeList    /* RLC size list */
{
   U8        numOfRlcSizes;
   RruTbInfo rbRlcSizeInfo[RRU_MAX_RLC_SIZE_LST_LEN];
} RruRbRlcSizeList;

typedef struct rruEDchActvSetInfo
{
   U8      nmbOfNodeBs; /* number of node Bs in E-DCH active set */  
   U32     nodeBsIds[RRU_NMB_NODEB_PER_EDCH_ACTV_SET]; /* Node Bs Ids */
   U8      servingNodebIdx; /* serving node B Index */
   U8      tmr; /* Timer used for selective combining */
} RruEDchActvSetInfo;

typedef struct rruEDchInfo
{
   U8                 flowId; /* E-DCH flow Id */
   Bool               resetInd; /* MAC e-DCH reset indicator */
   Bool               isTti2ms; /* is TTI 2 ms */
   U8                 reordWinSize; /* reordering window sixe */
   U8                 t1;  /* reordering release timer */
   U8                 ddiVal;  /* DDI value */
   U8                 nmbOfPduSize; /* number of RLC pdu sizes */
   U32                pduSize[RRU_MAX_NMB_RLC_PDU_PER_LOG_CHAN]; /* pdu size */
   RruEDchActvSetInfo eDchActvSetInfo; /* E-DCH active set info */
   /* rru_x_001.main_12 - Added element firstCfn */
   U16                firstCfn;
} RruEDchInfo;

typedef struct rruUlLogChMapInfo
{
   RruTrChId         trchId;
   RruLogChId        logChId;
   U8                rbMlp;
   U8                cmChPi; /* Scheduling Priority Indicator used for FACH and HS-DSCH */
   RruEDchInfo       eDchInfo;
} RruUlLogChMapInfo;

typedef struct RruHsDschQElm
{
   U8              flowId; /* HS-DSCH flow Id */
   U8              schedPriorInd; /* scheduling priority indicator */
   U16             hsCredits; /* MAC-HS credits */
   U8              numOfPdus; /* Number of HS-DSCH MAC flow PDU sizes */
   U32             pduSizeList[RRU_NMB_MACD_PDU_SIZES]; /* Array of HS-DSCH MAC flow PDU sizes */
} RruHsDschQElm;

typedef struct rruHsDschInfo
{
#ifdef RRC_IUR_SUPP
   Bool            isRemote; /* True for Drifted UE Configuration */
#endif
   Bool            ehsCap;   /* Is Iub ehs capable */
   U8              flowType; /* HS-DSCH flow type: UE/Common/Paging */
   RruHsDschQElm   priorQ; /* queue element info */
   /* rru_x_001.main_12 - Added element firstCfn */
   U16         firstCfn;  /* first CFN */
} RruHsDschInfo;

/* HSDSCH config info for DRNC.
  * When a UE is drifted, that time RB MAP is not required at DRNC.*/

typedef struct rruDrncHsDschInfo
{
   U8              cfgAction;
   Bool            isRemote; /* True for Drifted UE Configuration */
   Bool            ehsCap;   /* Is Iub ehs capable */
   U8              flowType; /* HS-DSCH flow type: UE/Common/Paging */
   U8              numPriQ;  /* Number of queue element */
   RruHsDschQElm   *priorQ; /* queue element info */
} RruDrncHsDschInfo;


typedef struct rruDlLogChMapInfo
{
   RruTrChId         trchId;
   RruLogChId        logChId;
   U8                rbMlp;
   U8                cmChPi; /* Scheduling Priority Indicator used for FACH and HS-DSCH */
   Bool              segInd; /* if segmentation is configured for this TM RB */
   Bool              discardInd; /* if SDU discard is configured for TM RB */
   RruRbRlcSizeList  rbRlcSizeList; /* this is always present */
   Bool              tctfPresence; /* applicable if logCh is MCCH/MSCH */
   RruHsDschInfo     hsDschInfo; /* HS-DSCH Info */
} RruDlLogChMapInfo;

typedef enum rruTmrMrw
{
   RRU_TMR_MRW_TE50,
   RRU_TMR_MRW_TE60,
   RRU_TMR_MRW_TE70,
   RRU_TMR_MRW_TE80,
   RRU_TMR_MRW_TE90,
   RRU_TMR_MRW_TE100,
   RRU_TMR_MRW_TE120,
   RRU_TMR_MRW_TE140,
   RRU_TMR_MRW_TE160,
   RRU_TMR_MRW_TE180,
   RRU_TMR_MRW_TE200,
   RRU_TMR_MRW_TE300,
   RRU_TMR_MRW_TE400,
   RRU_TMR_MRW_TE500,
   RRU_TMR_MRW_TE700,
   RRU_TMR_MRW_TE900
} RruTmrMrw;

typedef enum rruTmrDiscard
{
   RRU_TMR_DISC_TD0_1,
   RRU_TMR_DISC_TD0_25,
   RRU_TMR_DISC_TD0_5,
   RRU_TMR_DISC_TD0_75,
   RRU_TMR_DISC_TD1,
   RRU_TMR_DISC_TD1_25,
   RRU_TMR_DISC_TD1_5,
   RRU_TMR_DISC_TD1_75,
   RRU_TMR_DISC_TD2,
   RRU_TMR_DISC_TD2_5,
   RRU_TMR_DISC_TD3,
   RRU_TMR_DISC_TD3_5,
   RRU_TMR_DISC_TD4,
   RRU_TMR_DISC_TD4_5,
   RRU_TMR_DISC_TD5,
   RRU_TMR_DISC_TD7_5
} RruTmrDiscard;

typedef enum rruMaximumMrw
{
   RRU_MAX_MRW_MM1,
   RRU_MAX_MRW_MM4,
   RRU_MAX_MRW_MM6,
   RRU_MAX_MRW_MM8,
   RRU_MAX_MRW_MM12,
   RRU_MAX_MRW_MM16,
   RRU_MAX_MRW_MM24,
   RRU_MAX_MRW_MM32
} RruMaximumMrw;

typedef struct rruTmrBasedExp
{
   RruTmrMrw      tmrMrw;
   RruTmrDiscard  tmrDiscard;
   RruMaximumMrw  maximumMrw;
} RruTmrBasedExp;

typedef enum rruMaximumDAT
{
   RRU_MAX_DAT_DAT1,
   RRU_MAX_DAT_DAT2,
   RRU_MAX_DAT_DAT3,
   RRU_MAX_DAT_DAT4,
   RRU_MAX_DAT_DAT5,
   RRU_MAX_DAT_DAT6,
   RRU_MAX_DAT_DAT7,
   RRU_MAX_DAT_DAT8,
   RRU_MAX_DAT_DAT9,
   RRU_MAX_DAT_DAT10,
   RRU_MAX_DAT_DAT15,
   RRU_MAX_DAT_DAT20,
   RRU_MAX_DAT_DAT25,
   RRU_MAX_DAT_DAT30,
   RRU_MAX_DAT_DAT35,
   RRU_MAX_DAT_DAT40
} RruMaximumDAT;

typedef struct rruMaxDatRetrans
{
   RruMaximumDAT  maxDat;
   RruTmrMrw      tmrMRW;
   RruMaximumMrw  maxMRW;
} RruMaxDatRetrans;

typedef enum rruTmrBasedNoExpDiscard
{
   RRU_NOEXP_DISCARD_DT10,
   RRU_NOEXP_DISCARD_DT20,
   RRU_NOEXP_DISCARD_DT30,
   RRU_NOEXP_DISCARD_DT40,
   RRU_NOEXP_DISCARD_DT50,
   RRU_NOEXP_DISCARD_DT60,
   RRU_NOEXP_DISCARD_DT70,
   RRU_NOEXP_DISCARD_DT80,
   RRU_NOEXP_DISCARD_DT90,
   RRU_NOEXP_DISCARD_DT100
} RruTmrBasedNoExpDiscard;

typedef struct rruTxRlcDiscard
{
   U8 infoChoice;
   union 
   {
      RruTmrBasedExp            tmrBasedExp;
      RruTmrBasedNoExpDiscard   tmrBasedNoExpDisc;
      RruMaxDatRetrans          maxDatRetrans;
      RruMaximumDAT             noDiscard;
   }m;
} RruTxRlcDiscard;

typedef enum rruTmrPollProh
{
   RRU_TMR_POLL_PROH_TPP10,
   RRU_TMR_POLL_PROH_TPP20,
   RRU_TMR_POLL_PROH_TPP30,
   RRU_TMR_POLL_PROH_TPP40,
   RRU_TMR_POLL_PROH_TPP50,
   RRU_TMR_POLL_PROH_TPP60,
   RRU_TMR_POLL_PROH_TPP70,
   RRU_TMR_POLL_PROH_TPP80,
   RRU_TMR_POLL_PROH_TPP90,
   RRU_TMR_POLL_PROH_TPP100,
   RRU_TMR_POLL_PROH_TPP110,
   RRU_TMR_POLL_PROH_TPP120,
   RRU_TMR_POLL_PROH_TPP130,
   RRU_TMR_POLL_PROH_TPP140,
   RRU_TMR_POLL_PROH_TPP150,
   RRU_TMR_POLL_PROH_TPP160,
   RRU_TMR_POLL_PROH_TPP170,
   RRU_TMR_POLL_PROH_TPP180,
   RRU_TMR_POLL_PROH_TPP190,
   RRU_TMR_POLL_PROH_TPP200,
   RRU_TMR_POLL_PROH_TPP210,
   RRU_TMR_POLL_PROH_TPP220,
   RRU_TMR_POLL_PROH_TPP240,
   RRU_TMR_POLL_PROH_TPP250,
   RRU_TMR_POLL_PROH_TPP260,
   RRU_TMR_POLL_PROH_TPP270,
   RRU_TMR_POLL_PROH_TPP280,
   RRU_TMR_POLL_PROH_TPP290,
   RRU_TMR_POLL_PROH_TPP300,
   RRU_TMR_POLL_PROH_TPP310,
   RRU_TMR_POLL_PROH_TPP320,
   RRU_TMR_POLL_PROH_TPP330,
   RRU_TMR_POLL_PROH_TPP340,
   RRU_TMR_POLL_PROH_TPP350,
   RRU_TMR_POLL_PROH_TPP360,
   RRU_TMR_POLL_PROH_TPP370,
   RRU_TMR_POLL_PROH_TPP380,
   RRU_TMR_POLL_PROH_TPP390,
   RRU_TMR_POLL_PROH_TPP400,
   RRU_TMR_POLL_PROH_TPP410,
   RRU_TMR_POLL_PROH_TPP420,
   RRU_TMR_POLL_PROH_TPP430,
   RRU_TMR_POLL_PROH_TPP440,
   RRU_TMR_POLL_PROH_TPP450,
   RRU_TMR_POLL_PROH_TPP460,
   RRU_TMR_POLL_PROH_TPP470,
   RRU_TMR_POLL_PROH_TPP480,
   RRU_TMR_POLL_PROH_TPP490,
   RRU_TMR_POLL_PROH_TPP500,
   RRU_TMR_POLL_PROH_TPP510,
   RRU_TMR_POLL_PROH_TPP520,
   RRU_TMR_POLL_PROH_TPP530,
   RRU_TMR_POLL_PROH_TPP540,
   RRU_TMR_POLL_PROH_TPP550,
   RRU_TMR_POLL_PROH_TPP600,
   RRU_TMR_POLL_PROH_TPP650,
   RRU_TMR_POLL_PROH_TPP700,
   RRU_TMR_POLL_PROH_TPP750,
   RRU_TMR_POLL_PROH_TPP800,
   RRU_TMR_POLL_PROH_TPP850,
   RRU_TMR_POLL_PROH_TPP900,
   RRU_TMR_POLL_PROH_TPP950,
   RRU_TMR_POLL_PROH_TPP1000
} RruTmrPollProh;

typedef enum rruTmrPoll
{
   RRU_TMR_POLL_TP10,
   RRU_TMR_POLL_TP20,
   RRU_TMR_POLL_TP30,
   RRU_TMR_POLL_TP40,
   RRU_TMR_POLL_TP50,
   RRU_TMR_POLL_TP60,
   RRU_TMR_POLL_TP70,
   RRU_TMR_POLL_TP80,
   RRU_TMR_POLL_TP90,
   RRU_TMR_POLL_TP100,
   RRU_TMR_POLL_TP110,
   RRU_TMR_POLL_TP120,
   RRU_TMR_POLL_TP130,
   RRU_TMR_POLL_TP140,
   RRU_TMR_POLL_TP150,
   RRU_TMR_POLL_TP160,
   RRU_TMR_POLL_TP170,
   RRU_TMR_POLL_TP180,
   RRU_TMR_POLL_TP190,
   RRU_TMR_POLL_TP200,
   RRU_TMR_POLL_TP210,
   RRU_TMR_POLL_TP220,
   RRU_TMR_POLL_TP230,
   RRU_TMR_POLL_TP240,
   RRU_TMR_POLL_TP250,
   RRU_TMR_POLL_TP260,
   RRU_TMR_POLL_TP270,
   RRU_TMR_POLL_TP280,
   RRU_TMR_POLL_TP290,
   RRU_TMR_POLL_TP300,
   RRU_TMR_POLL_TP310,
   RRU_TMR_POLL_TP320,
   RRU_TMR_POLL_TP330,
   RRU_TMR_POLL_TP340,
   RRU_TMR_POLL_TP350,   
   RRU_TMR_POLL_TP360,
   RRU_TMR_POLL_TP370,
   RRU_TMR_POLL_TP380,
   RRU_TMR_POLL_TP390,
   RRU_TMR_POLL_TP400,
   RRU_TMR_POLL_TP410,
   RRU_TMR_POLL_TP420,
   RRU_TMR_POLL_TP430,
   RRU_TMR_POLL_TP440,
   RRU_TMR_POLL_TP450,
   RRU_TMR_POLL_TP460,
   RRU_TMR_POLL_TP470,
   RRU_TMR_POLL_TP480,
   RRU_TMR_POLL_TP490,
   RRU_TMR_POLL_TP500,
   RRU_TMR_POLL_TP510,
   RRU_TMR_POLL_TP520,
   RRU_TMR_POLL_TP530,
   RRU_TMR_POLL_TP540,
   RRU_TMR_POLL_TP550,
   RRU_TMR_POLL_TP600,
   RRU_TMR_POLL_TP650,
   RRU_TMR_POLL_TP700,
   RRU_TMR_POLL_TP750,
   RRU_TMR_POLL_TP800,
   RRU_TMR_POLL_TP850,
   RRU_TMR_POLL_TP900,
   RRU_TMR_POLL_TP950,
   RRU_TMR_POLL_TP1000
} RruTmrPoll;

typedef enum rruPollPdu
{
   RRU_POLL_PDU_PDU1,
   RRU_POLL_PDU_PDU2,
   RRU_POLL_PDU_PDU4,
   RRU_POLL_PDU_PDU8,
   RRU_POLL_PDU_PDU16,
   RRU_POLL_PDU_PDU32,
   RRU_POLL_PDU_PDU64,
   RRU_POLL_PDU_PDU128
} RruPollPdu;

typedef enum rruPollSdu
{
   RRU_POLL_SDU_SDU1,
   RRU_POLL_SDU_SDU4,
   RRU_POLL_SDU_SDU16,
   RRU_POLL_SDU_SDU64
} RruPollSdu;

typedef enum rruPollingWindow
{
   RRU_POLLING_WINDOW_PW50,
   RRU_POLLING_WINDOW_PW60,
   RRU_POLLING_WINDOW_PW70,
   RRU_POLLING_WINDOW_PW80,
   RRU_POLLING_WINDOW_PW85,
   RRU_POLLING_WINDOW_PW90,
   RRU_POLLING_WINDOW_PW95,
   RRU_POLLING_WINDOW_PW99
} RruPollingWindow;

typedef enum rruTmrPollPeriodic
{
   RRU_TMR_POLL_PERIODIC_TPER100,
   RRU_TMR_POLL_PERIODIC_TPER200,
   RRU_TMR_POLL_PERIODIC_TPER300,
   RRU_TMR_POLL_PERIODIC_TPER400,
   RRU_TMR_POLL_PERIODIC_TPER500,
   RRU_TMR_POLL_PERIODIC_TPER750,
   RRU_TMR_POLL_PERIODIC_TPER1000,
   RRU_TMR_POLL_PERIODIC_TPER2000
} RruTmrPollPeriodic;

typedef struct rruPollInfo
{
   U8                  bitMskPollInfo;
   RruTmrPollProh      tmrPollProh;
   RruTmrPoll          tmrPoll;
   RruPollPdu          tmrPollPdu;
   RruPollSdu          tmrPollSdu;
   Bool                lastTxPduPoll;
   Bool                lastReTxPduPoll;
   RruPollingWindow    pollWindow;
   RruTmrPollPeriodic  tmrPollPeriodic;
} RruPollInfo;

typedef enum rruTxWindowSize
{
   RRU_TX_WINDOW_SIZE_TW1,
   RRU_TX_WINDOW_SIZE_TW8,
   RRU_TX_WINDOW_SIZE_TW16,
   RRU_TX_WINDOW_SIZE_TW32,
   RRU_TX_WINDOW_SIZE_TW64,
   RRU_TX_WINDOW_SIZE_TW128,
   RRU_TX_WINDOW_SIZE_TW256,
   RRU_TX_WINDOW_SIZE_TW512,
   RRU_TX_WINDOW_SIZE_TW768,
   RRU_TX_WINDOW_SIZE_TW1024,
   RRU_TX_WINDOW_SIZE_TW1536,
   RRU_TX_WINDOW_SIZE_TW2047,
   RRU_TX_WINDOW_SIZE_TW2560,
   RRU_TX_WINDOW_SIZE_TW3072,
   RRU_TX_WINDOW_SIZE_TW3584,
   RRU_TX_WINDOW_SIZE_TW4095
} RruTxWindowSize;

typedef enum rruTmrRst
{
   RRU_TMR_RST_TR50,
   RRU_TMR_RST_TR100,
   RRU_TMR_RST_TR150,
   RRU_TMR_RST_TR200,
   RRU_TMR_RST_TR250,
   RRU_TMR_RST_TR300,
   RRU_TMR_RST_TR350,
   RRU_TMR_RST_TR400,
   RRU_TMR_RST_TR450,
   RRU_TMR_RST_TR500,
   RRU_TMR_RST_TR550,
   RRU_TMR_RST_TR600,
   RRU_TMR_RST_TR700,
   RRU_TMR_RST_TR800,
   RRU_TMR_RST_TR900,
   RRU_TMR_RST_TR1000
} RruTmrRst;

typedef enum rruMaximumRst
{
   RRU_MAX_RST_RST1,
   RRU_MAX_RST_RST4,
   RRU_MAX_RST_RST6,
   RRU_MAX_RST_RST8,
   RRU_MAX_RST_RST12,
   RRU_MAX_RST_RST16,
   RRU_MAX_RST_RST24,
   RRU_MAX_RST_RST32
} RruMaximumRst;

typedef enum rruRcvWindowSize
{
   RRU_RCV_WINDOW_SIZE_RW1,
   RRU_RCV_WINDOW_SIZE_RW8,
   RRU_RCV_WINDOW_SIZE_RW16,
   RRU_RCV_WINDOW_SIZE_RW32,
   RRU_RCV_WINDOW_SIZE_RW64,
   RRU_RCV_WINDOW_SIZE_RW128,
   RRU_RCV_WINDOW_SIZE_RW256,
   RRU_RCV_WINDOW_SIZE_RW512,
   RRU_RCV_WINDOW_SIZE_RW768,
   RRU_RCV_WINDOW_SIZE_RW1024,
   RRU_RCV_WINDOW_SIZE_RW1536,
   RRU_RCV_WINDOW_SIZE_RW2047,
   RRU_RCV_WINDOW_SIZE_RW2560,
   RRU_RCV_WINDOW_SIZE_RW3072,
   RRU_RCV_WINDOW_SIZE_RW3584,
   RRU_RCV_WINDOW_SIZE_RW4095
} RruRcvWindowSize;

typedef enum rruTmrStatProh
{
   RRU_TMR_STAT_PROH_TSP10,
   RRU_TMR_STAT_PROH_TSP20,
   RRU_TMR_STAT_PROH_TSP30,
   RRU_TMR_STAT_PROH_TSP40,
   RRU_TMR_STAT_PROH_TSP50,
   RRU_TMR_STAT_PROH_TSP60,
   RRU_TMR_STAT_PROH_TSP70,
   RRU_TMR_STAT_PROH_TSP80,
   RRU_TMR_STAT_PROH_TSP90,
   RRU_TMR_STAT_PROH_TSP100,
   RRU_TMR_STAT_PROH_TSP110,
   RRU_TMR_STAT_PROH_TSP120,
   RRU_TMR_STAT_PROH_TSP130,
   RRU_TMR_STAT_PROH_TSP140,
   RRU_TMR_STAT_PROH_TSP150,
   RRU_TMR_STAT_PROH_TSP160,
   RRU_TMR_STAT_PROH_TSP170,
   RRU_TMR_STAT_PROH_TSP180,
   RRU_TMR_STAT_PROH_TSP190,
   RRU_TMR_STAT_PROH_TSP200,
   RRU_TMR_STAT_PROH_TSP210,
   RRU_TMR_STAT_PROH_TSP220,
   RRU_TMR_STAT_PROH_TSP230,
   RRU_TMR_STAT_PROH_TSP240,
   RRU_TMR_STAT_PROH_TSP250,
   RRU_TMR_STAT_PROH_TSP260,
   RRU_TMR_STAT_PROH_TSP270,
   RRU_TMR_STAT_PROH_TSP280,
   RRU_TMR_STAT_PROH_TSP290,
   RRU_TMR_STAT_PROH_TSP300,
   RRU_TMR_STAT_PROH_TSP310,
   RRU_TMR_STAT_PROH_TSP320,
   RRU_TMR_STAT_PROH_TSP330,
   RRU_TMR_STAT_PROH_TSP340,
   RRU_TMR_STAT_PROH_TSP350,
   RRU_TMR_STAT_PROH_TSP360,
   RRU_TMR_STAT_PROH_TSP370,
   RRU_TMR_STAT_PROH_TSP380,
   RRU_TMR_STAT_PROH_TSP390,
   RRU_TMR_STAT_PROH_TSP400,
   RRU_TMR_STAT_PROH_TSP410,
   RRU_TMR_STAT_PROH_TSP420,
   RRU_TMR_STAT_PROH_TSP430,
   RRU_TMR_STAT_PROH_TSP440,
   RRU_TMR_STAT_PROH_TSP450,
   RRU_TMR_STAT_PROH_TSP460,
   RRU_TMR_STAT_PROH_TSP470,
   RRU_TMR_STAT_PROH_TSP480,
   RRU_TMR_STAT_PROH_TSP490,
   RRU_TMR_STAT_PROH_TSP500,
   RRU_TMR_STAT_PROH_TSP510,
   RRU_TMR_STAT_PROH_TSP520,
   RRU_TMR_STAT_PROH_TSP530,
   RRU_TMR_STAT_PROH_TSP540,
   RRU_TMR_STAT_PROH_TSP550,
   RRU_TMR_STAT_PROH_TSP600,
   RRU_TMR_STAT_PROH_TSP650,
   RRU_TMR_STAT_PROH_TSP700,
   RRU_TMR_STAT_PROH_TSP750,
   RRU_TMR_STAT_PROH_TSP800,
   RRU_TMR_STAT_PROH_TSP850,
   RRU_TMR_STAT_PROH_TSP900,
   RRU_TMR_STAT_PROH_TSP950,
   RRU_TMR_STAT_PROH_TSP1000
} RruTmrStatProh;

typedef enum rruTmrEpc
{
   RRU_TMR_EPC_TE50,
   RRU_TMR_EPC_TE60,
   RRU_TMR_EPC_TE70,
   RRU_TMR_EPC_TE80,
   RRU_TMR_EPC_TE90,
   RRU_TMR_EPC_TE100,
   RRU_TMR_EPC_TE120,
   RRU_TMR_EPC_TE140,
   RRU_TMR_EPC_TE160,
   RRU_TMR_EPC_TE180,
   RRU_TMR_EPC_TE200,
   RRU_TMR_EPC_TE300,
   RRU_TMR_EPC_TE400,
   RRU_TMR_EPC_TE500,
   RRU_TMR_EPC_TE700,
   RRU_TMR_EPC_TE900
} RruTmrEpc;

typedef enum rruTmrStatPeriodic
{
   RRU_TMR_STAT_PERIODIC_TSP100,
   RRU_TMR_STAT_PERIODIC_TSP200,
   RRU_TMR_STAT_PERIODIC_TSP300,
   RRU_TMR_STAT_PERIODIC_TSP400,
   RRU_TMR_STAT_PERIODIC_TSP500,
   RRU_TMR_STAT_PERIODIC_TSP750,
   RRU_TMR_STAT_PERIODIC_TSP1000,
   RRU_TMR_STAT_PERIODIC_TSP2000
} RruTmrStatPeriodic;

typedef struct rruStatusInfo
{
   U8                   bitMskPres;
   RruTmrStatProh       tmrStatusProh;
   RruTmrEpc            tmrEpc;
   Bool                 missingPDUInd;
   RruTmrStatPeriodic   tmrStatPeriodic;
} RruStatusInfo;

typedef struct rruUlAmRlcModeInfo
{
   Bool useSplHdrExt; /* Special usage of Header Extension*/
   RruRcvWindowSize  rcvWindowSize;
   RruStatusInfo     statusInfo;
   Bool              inSeqDelivery;
   U16               rlPduSize;
} RruUlAmRlcModeInfo;

typedef struct rruUlUmRlcModeInfo
{
   Bool              useAltEBit; /* Alternate E-Bit Interpretation */
   Bool              outOfSqRecpt; /* Out of sequence reception flag*/
   RruRcvWindowSize  rcvWindowSize;
} RruUlUmRlcModeInfo;

typedef struct rruUlTmRlcModeInfo
{
   Bool          segInd;
   U8            errSduCfg; /* delivery of erroneous SDUs */
} RruUlTmRlcModeInfo;

typedef struct rruUlRlcMode
{
   U8 infoChoice;
   union
   {
      RruUlAmRlcModeInfo amRlc;
      RruUlUmRlcModeInfo umRlc;
      RruUlTmRlcModeInfo tmRlc;
   }m;
} RruUlRlcMode;

typedef struct rruDlAmRlcModeInfo
{
   RruTxRlcDiscard    txRlcDiscard;
   RruTxWindowSize    txWindowSize;
   RruTmrRst          tmrRst;
   RruMaximumRst      maxRst;
   RruPollInfo        pollingInfo;
   RruStatusInfo      statusInfo;
   U16                rlPduSize;
   U16                minWsz; /* minimum window size */
   Bool               sendMrw; /* flag for send MRW indication */
   Bool useSplHdrExt; /* Special usage of Header Extension*/
   Bool               flexPduSz; /* flag to indicate the usage of flex pdu size */
   U8                 liSize; /* length indicator size when flxblPduSz is true */
} RruDlAmRlcModeInfo;

typedef enum rruDlUmRlcLIsize
{
   RRU_DL_UM_RLC_LI_SIZE_SIZE7,
   RRU_DL_UM_RLC_LI_SIZE_SIZE15
} RruDlUmRlcLIsize;

typedef struct rruDlUmRlcModeInfo
{
   Bool useAltEBit; /* Alternate E-Bit Interpretation */
   RruDlUmRlcLIsize   dlUmRlcLIsize;
   U16 pduSz; /* largest size of the PDU */
   RruTxRlcDiscard    txRlcDiscard;
} RruDlUmRlcModeInfo;

typedef struct rruDlTmRlcModeInfo
{
   Bool              segInd;
   RruTxRlcDiscard   txRlcDiscard;
} RruDlTmRlcModeInfo;

typedef struct rruDlRlcMode
{
   U8 infoChoice;
   union
   {
      RruDlAmRlcModeInfo amRlc;
      RruDlUmRlcModeInfo umRlc;
      RruDlTmRlcModeInfo tmRlc;
   }m;
} RruDlRlcMode;



typedef enum rruXpctReordering
{
   RRU_XPECT_REORDERING_REORDERINGNOTEXPECTED,
   RRU_XPECT_REORDERING_REORDERINGEXPECTED
} RruXpctReordering;

typedef struct rruRfc2507Info
{
   U8    bitMskPres;
   U16   fMaxPeriod;
   U8    fMaxTime;
   U16   maxHdr;
   U8    tcpSpace;
   RruXpctReordering    xpectReordering;   
} RruRfc2507Info;

typedef struct rruUlRfc3095
{
   U16 maxCID;
} RruUlRfc3095;

typedef struct rruDlRfc3095
{
   U16 maxCID;
   U16 reverseDeCompressDepth;
} RruDlRfc3095;

typedef struct rruRfc3095Info
{
   U8 numOfrohcProfiles;
   U8 rohcProfileList[RRU_MAX_ROHC_PROF_LST_LEN];
   RruUlRfc3095   ulRfc3095;
   RruDlRfc3095   dlRfc3095;
} RruRfc3095Info;

typedef struct rruPdcpHdrComprInfo
{
   U8 infoChoice;
   union
   {
      RruRfc2507Info   rfc2507Info;
      RruRfc3095Info   rfc3095Info;
   }m;
} RruPdcpHdrComprInfo;

typedef enum rruMaxPdcpSnWindowSize
{
   RRU_MAX_PDCP_SN_WINDOW_SIZE_SN255,
   RRU_MAX_PDCP_SN_WINDOW_SIZE_SN65535
} RruMaxPdcpSnWindowSize;

typedef enum rruPdcpPduHdr
{
   RRU_PDCP_PDU_HDR_PRESENT,
   RRU_PDCP_PDU_HDR_ABSENT
} RruPdcpPduHdr;

typedef enum rruRbType    /* status type */
{
   RRU_RB_COMMON,    /* common RB */
   RRU_RB_DEDICATED  /* dedicated RB */
} RruRbType;

/* rru_x_001.main_14 : To be used for Packet manager configuration*/
#ifdef USE_PKTMAN
typedef enum rruCiphType
{
   RRU_F8_CIPH,    /* F8 ciphering */
   RRU_PKTMAN_CIPH /* Packet Manager */
} RruCiphType;
#endif

typedef struct rruCiphCfgInfo
{
   U16    macDHfn;   /* MAC-d HFN */
   U32    cipherKey; /* Ciphering Key */
   U16    actvnTime; /* activation time */
/* rru_x_001.main_14 : To be used for Packet manager configuration*/
#ifdef USE_PKTMAN
   RruCiphType cipherType; /* Hardware support for ciphering to be used */
#endif
} RruCiphCfgInfo;

typedef struct rruHdr
{
   TknPres        pres;     /* Token Present */
   U8  idChoice;            /* indicates the choice in the union */
   union 
   { 
      U32             cellId;   /* Cell Id */
      U32             cellGrpId; /* Cell Group Id, for MTCH */
      RruUmtsUrnti    ueId;     /* Ue Id */
   } id; 
   RruUmtsRbId    rbId;     /* radio bearer Id */
   RruRbType      rbType; /* radio bearer type - common/dedicated */
   U32            transId; /* Transaction identifier for simultaneous procedures */
} RruHdr;

/* SRNS RELOCATION INFORMATION */
typedef struct rruRohcDeCompCmnDb
{
   U16                  ulCidType;
   U16                  k2;
   U16                  k1;
   U16                  n2;
   U16                  n1;
   U16                  modeChCnt;
   U16                  ackGenCnt;
   U16                  nackGenCnt;
   U8                   feedBackEnb;
   U32                  fbPrevTick;
   U8                   k;  
}RruRohcDeCompCmnDb;
typedef struct rruCompRohcProfInfo
{
   U8                   support[RRU_ROHC_PROF_LIST];
}RruRohcCompProfInfo;

typedef struct rruRohcCompCmnDb
{
   U8                   fEna;
   U8                   noPktsForConf;
   U8                   nbo;
   U8                   dlCidType;
   U8                   noPkt2EstRtpProf;
   U8                   noPktsNullEnc;
   U16                  ipIdWindowSize;
   U16                  snWindowSize;
   RruRohcCompProfInfo   sprofInfo;
}RruRohcCompCmnDb;

typedef struct rruRohcCmnDb
{
   
     U8                    dbType;
     U32                   rohcTick;
     Region                region;
     Pool                  pool;
     RruRohcCompCmnDb      compDb;
     RruRohcDeCompCmnDb    deCompDb;
}RruRohcCmnDb;

typedef struct rruPdcpRelocInfo
{
   U16 tcFirstUnAckSn; /* DL_Send PDCP SN of the first transmitted but not yet acknowledged PDCP SDU. */
   U16 recvUlSn;       /* PDCP Receive UL Sequence No. */
   U16 numSdu;         /* Number of SDUs those are not yet transmitted*/
   Buffer **sduList;   /* Buffered SDU List */
} RruPdcpRelocInfo;

typedef struct rruPdcpLosslessRelocInfo
{
   U16 nextPdcpSndSn;/* Next Send PDCP SN */
   U16 nextPdcpRcvSn;/* Next Recv PDCP SN */
   RruPdcpRelocInfo relocInfo; /* Relocation Info */
} RruPdcpLosslessRelocInfo;

typedef struct rruPdcpSeamlessRelocInfo
{
   U16 noDlCntx;
   Buffer **dlPtr;
   U16 noUlCntx;
   Buffer **ulPtr;
   RruRohcCmnDb cmDb;
} RruPdcpSeamlessRelocInfo;

typedef struct rruRelocPrepInfo /* Container Info */
{
   U8  rbId;
   U8  bitMskPrsnt;
   U32 dlCountC;  /* DL_CountC */
   U32 ulCountC;  /* UL-CountC */
} RruRelocPrepInfo;

typedef struct rruRelocCommitInfo /* Relocation info to be fetched from lower layers */
{
   U8 rbId;          /* Conerned RB Id */
   U8 bitMskPrsnt;   /* Bit mask to represent the member in the Union */
   union
   {
      RruPdcpLosslessRelocInfo losslessReloc; /* PDCP Info During Lossless Reloc */
      RruPdcpSeamlessRelocInfo seamlessReloc; /* PDCP Info During SeamLess Reloc */
   }type;
} RruRelocCommitInfo;

/* rru_x_001.main_8 Count-I and MAC Count-C Support */
typedef struct rruCountIInfo
{
   U8    rbId;        /* RB ID */
   U8    bitMskPrsnt; /* CountI presence Mask */
   U32   dlCountI;    /* DL_CountI */
   U32   ulCountI;    /* UL-CountI */
} RruCountIInfo;

typedef struct rruCountIInfoLst
{
   U8  numRbs;             /* Number of RBs */
   RruCountIInfo *countI;
} RruCountIInfoLst;

typedef struct rruTmCountCInfo
{
   U8  bitMskPrsnt; /* CountC presence Mask for CS Domain and/or PS Domain */
   U32 csDmnCountC;     /* CS CountC */
   U32 psDmnCountC;     /* PS CountC */
} RruTmCountCInfo;


typedef struct rruRelocPrepInfoCfm /* SRNC relocation Ind Info SDU */
{
   CmMemListCp memCp;
   RruHdr hdr;            /* sdu Header */
   struct
   {
      U8 numRbs;         /* Number of RBs */
      RruRelocPrepInfo *relocInfo;/* Relocation Info */
      /* rru_x_001.main_8 Count-I and MAC Count-C Support */
      Bool  tmCountCPrsnt; /* Transparent Mode Count-C Information Present */
      RruTmCountCInfo *tmCountCInfo; /* TM Count-C information*/
      RruCountIInfoLst countILst; /* Count-I List */
   } sdu;
} RruRelocPrepInfoCfm;

typedef struct rruRelocCommitInfoCfm /* SRNC relocation Ind Info SDU */
{
   CmMemListCp memCp;
   RruHdr hdr;            /* sdu Header */
   struct
   {
      U8 numRbs;         /* Number of RBs */
      RruRelocCommitInfo *relocInfo;/* Relocation Info */
   } sdu;
} RruRelocCommitInfoCfm;

typedef struct rruRelocInfoReq /* MBMS Transmit Req */
{
   RruHdr        hdr;            /* header */
   RruUmtsUrnti  ueId;
   U8            bitMsk;         /* bit mask for Count Info/ PDCP lossless
                                    relocation info/ PDCP seamless relocation
                                    info */
} RruSrnsRelocInfoReq;

typedef struct rruRbPdcpCfgInfo
{
   U8                      bitMskPres;
   U8                      srnsRelocSupport;
   RruMaxPdcpSnWindowSize  maxPdcpSnWindowSize; /* present only if 
                                              srnsRelocSupChoice is supported */
   RruPdcpPduHdr           pdcpPduHdr;
   U8                      numOfHdrComprInfo;
   RruPdcpHdrComprInfo     hdrComprInfoList[RRU_MAX_HDR_COMPR_INFO_LIST_LEN];
   U16                      dlSendSn;
   RruPdcpSeamlessRelocInfo ctxtInfo;  /* Context Info */
   RruPdcpRelocInfo relocInfo; /* Relocation Information */
} RruRbPdcpCfgInfo;

typedef struct rruUlRbCfg
{
   U8                  ulBitMsk;
   U8                  numOfUlLogChMap;
   RruUlLogChMapInfo   ulLogChMapInfo[RRU_MAX_LOGCH_PER_RLC];  /* UL Log ch mapping info */
   RruUlRlcMode        ulRlcMode;
   RruCiphCfgInfo      ciphCfgInfo;      /* Ciphering Info */
} RruUlRbCfg;

typedef struct rruDlRbCfg
{
   U8                  dlBitMsk;
   U8                  numOfDlLogChMap;
   RruDlLogChMapInfo   dlLogChMapInfo[RRU_MAX_LOGCH_PER_RLC];  /* DL Log ch mapping info */
   RruDlRlcMode        dlRlcMode;
   RruCiphCfgInfo      ciphCfgInfo;      /* Ciphering Info */
} RruDlRbCfg;

typedef struct rruRbCfgInfo /* RB map info */
{
   RruUmtsRbId       rbId;    /* radio bearer Id */
   U8                sigOrDataRb;  /* type: signalling or data rb */
   U8                cnDmn;   /* CS/PS/Common */
   U8                bitMsk;  /* UL/DL/PDCP Info */
   RruUlRbCfg        ulRbCfg;
   RruDlRbCfg        dlRbCfg;
   RruRbPdcpCfgInfo  pdcpCfgInfo;  /* PDCP Info */
} RruRbCfgInfo;

typedef struct rruRbCfgList /* RB Map Config Info */
{
   U8 numOfRbCfgInfo;
   RruRbCfgInfo rbCfgInfoList[RRU_MAX_RB_PER_UE_CFG]; /* linked list of RruRbMapInfo */
} RruRbCfgList;

typedef struct rruRbRlsCfgList /* RB release Config Info */
{
   U8 numOfRbCfgInfo;
   RruUmtsRbId   rbsToRel[RRU_MAX_RB_PER_UE_CFG]; /* linked list of Rbs to release */
} RruRbRlsCfgList;

typedef struct rruRbCfgAllList /* RB map info */
{
   U8   bitMskPres;
   RruRbCfgList     rbToAddCfgList;   /* RB config for Add */
   RruRbCfgList     rbToModCfgList;   /* RB config for Modify */
   RruRbRlsCfgList  rbToRlsCfgList;   /* RB config for Release */
} RruRbCfgAllList;

typedef struct rruTfcCfg
{
   U8    numOfTfi;  /* # of TFI in the TFC */
   U8    tfc[RRU_MAX_RRC_TRCHS];   /* TFC, each element is a TFI */
} RruTfcCfg;

typedef struct rruTfcsCfgList
{
   U16         firstCfn;  /* first CFN for the CcTrch */ 
   U32         numOfTfcs;
   RruTfcCfg   tfcs[RRU_MAX_TFCS_CFG];  
} RruTfcsCfgList;

typedef enum rruTti
{
   RRU_TTI_TTI10,
   RRU_TTI_TTI20,
   RRU_TTI_TTI40,
   RRU_TTI_TTI80
} RruTti;

typedef enum rruChanCodingRate
{
   RRU_CHAN_CODING_RATE_HALF,
   RRU_CHAN_CODING_RATE_THIRD
} RruChanCodingRate;

typedef enum rruChanCodingType
{
   RRU_CHAN_CODING_TYPE_NOCODING,
   RRU_CHAN_CODING_TYPE_CONVOLUTIONAL,
   RRU_CHAN_CODING_TYPE_TURBO
} RruChanCodingType;

typedef enum RruCrcSize
{
   RRU_CRC_SIZE_CRC0,
   RRU_CRC_SIZE_CRC8,
   RRU_CRC_SIZE_CRC12,
   RRU_CRC_SIZE_CRC16,
   RRU_CRC_SIZE_CRC24
} RruCrcSize;

typedef struct rruTf
{
   RruTti               tti;
   U8                   numOfTbInfo;
   RruTbInfo            tbInfo[RRU_MAX_TF_PER_TRCH];
} RruTf;

typedef struct rruTfsCfgList  /* TrchId, Type, TFS */
{
   RruTrChId   trchId;
   U8          cmChPi; /* Scheduling Priority Indicator used for FACH  */
   U8          numOfTtiTfs;
   RruTf       ttiTfs[RRU_MAX_TTI_PER_TRCH];
} RruTfsCfgList;

typedef struct rruCctrchCfgInfo
{
   U8                tfsBitMsk;         /* ADD/MODIFY/RLS */
   U8                addCfgCnt;
   RruTfsCfgList     tfsAddCfgList[RRU_MAX_TFS_PER_CFG];  /* TrChs to Add */
   U8                modCfgCnt;
   RruTfsCfgList     tfsModCfgList[RRU_MAX_TFS_PER_CFG];  /* TrChs to Modify */
   U8                rlsCfgCnt;
   RruTrChId         tfsRlsCfgList[RRU_MAX_TFS_PER_CFG];  /* TrChs to Release */
   RruTfcsCfgList    tfcsCfgList;       /* TFCS for CcTrch */
} RruCctrchCfgInfo;

typedef struct rruCctrchCfgAllList
{
   U8                bitMskAll;  /* DL/UL/ALL */
   RruCctrchCfgInfo  ulCctrchCfg;
   RruCctrchCfgInfo  dlCctrchCfg;
} RruCctrchCfgAllList;

typedef U16 RruUmtsCrnti;        /* Crnti UE Id */

typedef struct rruUeCfgInfo
{
#ifdef RRC_IUR_SUPP
   Bool           isRemote; /* isRemote will be true for a drifted UE 
                               configuration at both srnc & drnc */
#endif /* RRC_IUR_SUPP */
   TknPres        urntiPres;
   RruUmtsUrnti   newUrnti;
   TknPres        crntiPres;
   RruUmtsCrnti   newCrnti;
   TknU16         hrnti; /* H-RNTI */
   TknU8          hsMesRslt;  /* MAC-HS measure results */
/* rru_x_001.main_13 - Added cellId to track the cell to which UE belongs. */
#ifdef RRU_VER1 
   U32            cellId; /* ID of the cell to which the UE belongs. */
#endif /* RRU_VER1 */
} RruUeCfgInfo;

#ifdef RRC_IUR_SUPP
typedef struct rruFachCmchpi
{
   U8 cmChPi;
   U8 cfgSrc; /* Configuration is for SRNC or DRNC */
   union 
   {   
      struct 
        {
           U8 fachCredits; 
           U16 pduSz;       
        }srnc;
      struct 
        {
           U16 fachId;  
        }drnc;
   }src;
} RruFachCmchpi;

typedef struct rruFachConfig
{
   U8 actionMsk; /* Config, Reconfig or Rls */
   U8 dlCcTrchId;
   U8 numCmchpi;
   RruFachCmchpi cmchpiArr[16];   
} RruFachConfig;

typedef struct rruRachConfig
{
   U8  actionMsk; /* Config, Reconfig or Rls */
   U16 rachId;   /* RACH Id to which UE is mapped */
   U8  ulCcTrchId;
} RruRachConfig;

typedef struct rruDrncCfgForDUe
{
   U32           cellId;
   U8            bitMskPres; /* BitMask to know which parameters are present */
   RruFachConfig rruFachCfg;
   RruRachConfig rruRachCfg;
   RruDrncHsDschInfo rruHsdschCfg;
} RruDrncCfgForDUe;
/* rru_x_001.main_6  Added Drx Cycle Coefficient parameter */
typedef struct rruSrncCfgForDUe
{
   U8            drxCyclCoef;
   U8            bitMskPres; /* cfgType to know whether the configuration 
                                is for FACH, RACH  or HS-DSCH */
   U8            rachActMsk;
   RruFachConfig rruFachCfg;
} RruSrncCfgForDUe;

#endif /* RRC_IUR_SUPP */

typedef struct rruExpCfg    /* explicit config info */
{
   U8 bitMskPres;
   RruRbCfgAllList      rbCfgAllList;   /* RB Config Info */
   RruCctrchCfgAllList  ccTrchCfgInfoList; /* CcTrch config info */
   RruSecCfg            secCfg;   /* Security Configuration */
   RruUeCfgInfo         ueCfgInfo;/* UE-Id related config info */
   RruUmtsUrnti         newUeCtxtId;     /* Ueid to create the UE context */
#ifdef RRC_IUR_SUPP
   RruSrncCfgForDUe     srncCfgForDUe;  /* Configuration at SRNC for Drift-UE */
   RruDrncCfgForDUe     drncCfgForDUe;  /* Configuration at DRNC for Drift-UE */
#endif /* RRC_IUR_SUPP */
   Bool                 rlcRestIndRb2to5; /* True means Rbs 2 to 5 have to be 
                                             reestablished if configured already */
   Bool                 rlcRestIndRb5AndAbove;/* True means Rbs above 5 have to be 
                                                 reestablished if configured already */
} RruExpCfg;

typedef U8 RruDefaultCfgId;        /* Default config id */

typedef struct rruRbInfoToChg
{
   RruUmtsRbId     rbId; /* rb Id to be changed */
   U8  idChoice;
   RruUmtsRbId     rbIdRef; /* rb Id to refer */ 
} RruRbInfoToChg;

typedef struct rruRbInfoToChgLst
{
   U8 rbCnt;
   RruRbInfoToChg rbInfChg[RRU_MAX_RB_PER_UE_CFG];
} RruRbInfoToChgLst;

typedef struct rruDefCfg    /* default config info */
{
   RruDefaultCfgId   defCfgId;
   U16 firstCfn;
   Bool chgLstPres;
   RruRbInfoToChgLst rbInfChgLst; /*RB information to change list.
                                   *Applicable for RB setup message (R6 and R7)*/
   RruUeCfgInfo      ueCfgInfo;  /* should be filled by user if new URNTI/CRNTI
                                  * is present in ue msg.
                                  */
} RruDefCfg;

typedef U8 RruPreDefCfgId;      /* Predefined configuration id */

typedef struct rruPreDefCfg    /* pre-defined config SDU */
{
   RruPreDefCfgId   predefCfgId;
   RruUeCfgInfo     ueCfgInfo;  /* should be filled by user if new URNTI/CRNTI
                                 * is present in ue msg.
                                 */
} RruPreDefCfg;

/* pres value of ABSENT indicates default activation time NOW */
typedef struct rruActvnTime
{
   U8   pres; /* PRESENT/ABSENT */
   U8   cfn;  /* CFN (0..255) */
} RruActvnTime;

 
/* rru_x_001.main_5 : multiple lower sap support and drift RNC feature. */
#ifdef RRC_MULT_SAP_SUPPRT
typedef struct rruLLyrSapIdInfo
{
   U8 bitMskPres;
   U8        rluSapId;      /* RLU Sap Id */
   U8        crlSapId;      /* CRL Sap Id */
   U8        cmkSapId;      /* CMK Sap Id */
   U8        ctcSapId;      /* CTC Sap Id */
} RruLLyrSapIdInfo;
#endif
typedef struct rruUeCfgAtRnc        /* UE Configuration at RNC */
{
   RruActvnTime        actvnTime; /* Activation Time */
   U8                  cfgSrc;        /* explicit/default/predefined */
#ifdef RRC_MULT_SAP_SUPPRT
   RruLLyrSapIdInfo    llyrSapIdInfo;  /* lower sap Id info */   
#endif
   union 
   {
      RruExpCfg      explicitCfg;  /* Explicit Config */
      RruDefCfg      defaultCfg;   /* default config */
      RruPreDefCfg   preDefCfg;    /* pre-defined config */
   } cfg;
} RruUeCfgAtRnc;


typedef struct rruError            /* Error Information */
{
   U8    errType;
   U16   errCause;
} RruError;

typedef struct rruDatReqSdus      /* Data request SDU */
{
   CmMemListCp          memCp;
   RruHdr               hdr;     /* sdu Header */
   struct
   {
      U8   bitMskDatReq;
      RruDatReqUeMsg   msg;
      RruUeCfgAtRnc    ueCfg;
   }sdu;
} RruDatReqSdus;

typedef struct rruDatIndSdus      /* Data Indication SDU */
{
   CmMemListCp          memCp;
   RruHdr               hdr;     /* sdu Header */
   U8                   errPres; /* Error Presence flag TRUE/FALSE */
   RruError             err; /* Error value */
   struct
   {
      U8         msgCategory;
      union
      {
         RruUL_CCCH_Msg     ulCcchMsg;     /* UL-CCCH SDU */
         RruUL_DCCH_Msg     ulDcchMsg;     /* UL-DCCH SDU */
      } m;
   }sdu;
} RruDatIndSdus;

typedef struct rruDatRspSdus      /* Data Response SDU */
{
   CmMemListCp          memCp;
   RruHdr               hdr;     /* sdu Header */
   struct
   {
      U8         msgCategory;
      U8         cnDmn;
      union
      {
         RruDL_CCCH_Msg     dlCcchMsg;     /* DL-CCCH SDU */
         RruDL_DCCH_Msg     dlDcchMsg;     /* DL-DCCH SDU */
      } m;
   }sdu;
} RruDatRspSdus;

typedef struct rruSysInfoBlock            /* Sys Info block SDU */
{
      U8         sysInfoType;
      union
      {
         RruSysInfoTyp1      sib1;
         RruSysInfoTyp2      sib2;
         RruSysInfoTyp3      sib3;
         RruSysInfoTyp4      sib4;
         RruSysInfoTyp5      sib5;
         RruSysInfoTyp5      sib5bis;
         RruSysInfoTyp6      sib6;
         RruSysInfoTyp7      sib7;
         RruSysInfoTyp8      sib8;
         RruSysInfoTyp9      sib9;
         RruSysInfoTyp10     sib10;
         RruSysInfoTyp11     sib11;
         RruSysInfoTyp11bis  sib11bis;
         RruSysInfoTyp12     sib12;
         RruSysInfoTyp13     sib13;
         RruSysInfoTyp13_1   sib13_1;
         RruSysInfoTyp13_2   sib13_2;
         RruSysInfoTyp13_3   sib13_3;
         RruSysInfoTyp13_4   sib13_4;
         RruSysInfoTyp15     sib15;
         RruSysInfoTyp15_1   sib15_1;
         RruSysInfoTyp15_2   sib15_2;
         RruSysInfoTyp15_3   sib15_3;
         RruSysInfoTyp15_4   sib15_4;
         RruSysInfoTyp15_5   sib15_5;
         RruSysInfoTyp16     sib16;
         RruSysInfoTyp18     sib18;
         RruSysInfoTyp20     sib20;/*rru_x_001.main_10 upgraded to support SIB20*/
         RruSysInfoTypSB1    sb1;
         RruSysInfoTypSB2    sb2;
         RruMasteInfBlock    mib;
      } m;
} RruSysInfoBlock;


typedef struct rruEncDatReqSdus            /* Encode data SDU */
{
   CmMemListCp      memCp;
   RruHdr           hdr;       /* sdu Header */
   struct
   {
      U8         msgCategory;
      union
      {
         RruDL_DCCH_Msg    dlDcchMsg;
         RruUL_DCCH_Msg    ulDcchMsg;
         RruDL_CCCH_Msg    dlCcchMsg;
         RruUL_CCCH_Msg    ulCcchMsg;
         RruPCCH_Msg       pcchMsg;
         RruBCCH_FACH_Msg  bcchFachMsg;
         RruBCCH_BCH_Msg   bcchMsg;
         RruMCCH_Msg       mcchMsg;
         RruMSCH_Msg       mschMsg;
         RruSysInfoBlock   sibMsg;
         RruToTrgtRNC_Conta srncToTrncCont;
         RruTrgtRNC_ToSourcRNC_Conta trncToSrncCont;
         RruHOToUTRANCmd   hoToUTRANCmd;
         RruHOToUTRANCmd_r7_IEs   hoToUTRANCmd_r7;
/* rru_x_001.main_7 Adding RruUE_CapbConta_IEs in the RruEncDatReq Sdus. */
         RruUE_CapbConta_IEs      ueCapabConta;
      } m;
   }sdu;
}RruEncDatReqSdus;

typedef struct rruEncDatCfmSdus      /* Encoded data SDU */
{
   RruHdr         hdr;       /* sdu Header */
   struct
   {
      U8         msgCategory;
      U32        msgLen;        /* size of encoded data in bits */
      Buffer     *mBuf;         /* encoded data */
   } sdu;
} RruEncDatCfmSdus;

typedef struct rruDecDatReqSdus      /* data SDU to be decoded */
{
   CmMemListCp      memCp;
   RruHdr           hdr;         /* sdu Header */
   struct
   {
      U8         msgCategory;
      Buffer     *mBuf;        /* data for decoding */
   } sdu;
} RruDecDatReqSdus;

typedef struct rruDecDatCfmSdus       /* Decoded data SDU */
{
   CmMemListCp      memCp;
   RruHdr           hdr;         /* sdu Header */
   struct
   {
      U8         msgCategory;
      union
      {
         RruDL_DCCH_Msg       dlDcchMsg;
         RruUL_DCCH_Msg       ulDcchMsg;
         RruDL_CCCH_Msg       dlCcchMsg;
         RruUL_CCCH_Msg       ulCcchMsg;
         RruPCCH_Msg          pcchMsg;
         RruBCCH_FACH_Msg     bcchFachMsg;
         RruBCCH_BCH_Msg      bcchMsg;
         RruMCCH_Msg          mcchMsg;
         RruMSCH_Msg          mschMsg;
         RruSysInfoBlock      sibMsg;
         RruToTrgtRNC_Conta   srncToTrncCont;
         RruTrgtRNC_ToSourcRNC_Conta trncToSrncCont;
         RruInterRATHOInfo    interRatHoInfo;
         RruUE_CapbConta_IEs  ueCapabConta;
      } m;
   }sdu;
} RruDecDatCfmSdus;

typedef struct rruErrIndSdus          /* Error SDU */
{
   RruHdr           hdr;              /* sdu Header */
   RruError         sdu;              /* RRC error */
} RruErrIndSdus;

typedef struct rruCfgReqSdus        /* configuration SDU */
{
   CmMemListCp      memCp;
   RruHdr           hdr;         /* sdu Header */
   struct
   {
      RruActvnTime    actvnTime; /* Activation Time */
      RruUeCfgAtRnc   cfgInfo;
   } sdu;
} RruCfgReqSdus;

typedef enum rruCfgResult    /* configuration result */
{
   RRU_CFG_SUCCESS,
   RRU_CFG_FAILURE
} RruCfgResult;

typedef TknU8 RruCfgErrType;

typedef TknU8 RruCfgErrCause;

typedef struct rruCfgErrorInfo
{
   RruCfgErrType    cfgErrType;
   RruCfgErrCause   cfgErrCause;
} RruCfgErrorInfo;

typedef struct rruCfgCfmSdus    /* config confirm */
{
   RruHdr             hdr;          /* sdu Header */
   struct
   {
      RruCfgResult      result;    /* Success/failure */
      RruCfgErrorInfo   cfgError;  /* valid only when the result is failure */
   } sdu;
} RruCfgCfmSdus;

typedef struct rruMkMesCfg   /* Measurement request info */
{
   U8           instId;        /* valid if common channel(PRACH instance) */
   RruTrChId    trChId;        /* Tranport Channel Id */
   U8           mesId;         /* Measurement Id */
   U8           rptMode;       /* Reporting Mode */
   U8           rptBO;         /* BO Reporting Quantity Flag */
   U8           rptAverage;    /* Average Reporting Quantity Flag */
   U8           rptVariance;   /* Variance Reporting Quantity Flag */
   U16          samplingTim;   /* Sampling Time */
   U16          rptTim;        /* Reporting Time(for periodic meas) */
   U32          upperThrhld;   /* Upper Threshold */
   U32          lowerThrhld;   /* Lower Threshold */
} RruMkMesCfg;

typedef struct rruMesCfgReqSdus    /* measurement config SDU */
{
   CmMemListCp   memCp;
   RruHdr        hdr;              /* sdu Header */
   struct
   {
      U8            cfgType;           /* config type (start/modify/stop) */
      U16           listSize;          /* Size of List */
      RruMkMesCfg   mkMesCfgList[RRU_MAX_MES_PER_CFG];  /* measurements list */
   } sdu;
} RruMesCfgReqSdus;

typedef struct rruMacMesRpt    /* measurment report */
{
   U8             mesId;      /* Measurement Request Id */
   RruUmtsRbId    rbId;       /* UE-Id + RB - Id */
   U8             mode;       /* Traffic Volume measurement mode */
   U8             evtId;      /* Event Id used only for Event Trg */
   U8             boRpt;      /* BO Reporting Quantity Flag */
   U8             avgRpt;     /* Average Reporting Quantity Flag */
   U8             varRpt;     /* Variance Reporting Quantity Flag */
   U32            trfVolBo;   /* Traffic Volume For BO */
   U32            trfVolAv;   /* Traffic Volume For Average */
   U32            trfVolVar;  /* Traffic Volume For Variance */
} RruMacMesRpt;

typedef struct rruMesRptIndSdus    /* measurement report SDU */
{
   CmMemListCp   memCp;
   RruHdr        hdr;                /* sdu Header */
   struct
   {
      U16            listSize;          /* Size of List */
      RruMacMesRpt   macMesRptList[RRU_MAX_MES_PER_CFG]; /* measurements report list */
   } sdu;
} RruMesRptIndSdus;

typedef struct rruCountCInfo
{
   RruUmtsRbId    rbId;
   U8             sndRcvMsk;  /* UL/DL CountC presence Mask */
   U32            ulCountC;
   U32            dlCountC;
} RruCountCInfo;

typedef struct rruCountCIndInfo    /* measurement report SDU */
{
   CmMemListCp   memCp;
   RruHdr        hdr;                /* sdu Header */
   struct
   {
      U8             numRbs;
      RruCountCInfo  *countCInfo; 
   } sdu;
} RruCountCIndInfo;

typedef struct rruStatusIndSdus    /* status confirm SDU */
{
   RruHdr        hdr;              /* sdu Header */
   U8            status;          /* status event values */
} RruStatusIndSdus;

typedef struct rruUmtsUeId    /* UE Identifier */
{
   U8            bitMsk;
   RruUmtsCrnti  crnti;
   RruUmtsUrnti  urnti;
} RruUmtsUeId;

/* All elements in this structure are present. no bitmask is used */
typedef struct rruPreDefCfgInfo    /* predef config Info */
{
   RruRbCfgList     rbCfgList;     /* RB config to setup */
   U8               ulTfsCfgCnt;
   RruTfsCfgList    ulTfsCfgList[RRU_MAX_TFS_PER_CFG]; /* TrChs to Add/Reconf in UL */
   U8               dlTfsCfgCnt;
   RruTfsCfgList    dlTfsCfgList[RRU_MAX_TFS_PER_CFG]; /* TrChs to Add/Reconf in DL */
   RruTfcsCfgList   dlTfcsCfgList;   /* TFCS for CcTrch in DL */
} RruPreDefCfgInfo;

typedef struct rruPreDefCfgReqSdus    /* predef config SDU */
{
   CmMemListCp        memCp;
   RruHdr             hdr;              /* sdu Header */
   struct
   {
      U16               preDefCfgId;
      RruPreDefCfgInfo  preDefCfg; /* predef config */
   } sdu;
} RruPreDefCfgReqSdus;

typedef struct rruCellCfgInfo /* cell config info per PRACH/SCCPCH */
{
   U8               instId;        /* PRACH/SCCPCH instace Id */
   U8               bitMskPres;    /* RB/TFCS/TFS */
   RruRbCfgList     rbCfgList;     /* RB config information */
   RruTfcsCfgList   tfcsCfgList;   /* TFCS configuration */
   U8               numOfTfsCfg;   /* number of TFS */
   RruTfsCfgList    tfsCfgList[RRU_MAX_TFS_PER_CFG]; /* TFS */
#ifdef RRC_MULT_SAP_SUPPRT
   RruLLyrSapIdInfo    llyrSapIdInfo;  /* lower sap Id info */   
#endif
} RruCellCfgInfo;

typedef struct rruCellCfgReqSdus /* SCCPCH config SDU */
{
   CmMemListCp     memCp;
   RruHdr          hdr; /* sdu Header */
   struct
   {
      U8              cfgType; /* Setup/Release */
      U8              cfgChoice; /* PRACH/SCCPCH */
      RruCellCfgInfo  cfgInfo; /* PRACH/SCCPCH config info */
   } sdu;
} RruCellCfgReqSdus;

typedef struct rruCellCfgCfmSdus /* Cell config status confirm */
{
   RruHdr hdr; /* sdu Header */
   struct
   {
      U8                cfgType;   /* Setup/Modify/Release/Stop/Continue */
      RruCfgResult      result;    /* Success/failure */
      RruCfgErrorInfo   cfgError;  /* valid only when the result is failure */
   } sdu;
} RruCellCfgCfmSdus;

/* MBMS changes */
typedef struct rruMcchModInfo
{
   U8                msgBitMsk;  /* bit mask for msgs present */
   RruMCCH_Msg       mbmsModSrvInfo;  /* Modified Services Information */
   RruMCCH_Msg       mbmsGenInfo; /* MBMS General Information */
   RruMCCH_Msg       mbmsCommonInfo; /* MBMS Common RB Information */
   RruMCCH_Msg       mbmsCurCellInfo; /* MBMS Current Cell Information */
   U8                numNeighCells; /* no of meighbor cells */
   RruMCCH_Msg       *mbmsNeighCellsInfo; /* neighbor cell(s) information */
} RruMcchModInfo;

typedef struct rruMcchUnmodInfo
{
   U8                msgBitMsk;  /* bit mask for msgs present */
   RruMCCH_Msg       mbmsUnmodSrvInfo;  /* Unmodified Services Information */
   RruMCCH_Msg       mbmsGenInfo; /* MBMS General Information */
   RruMCCH_Msg       mbmsCommonInfo; /* MBMS Common RB Information */
   RruMCCH_Msg       mbmsCurCellInfo; /* MBMS Current Cell Information */
   U8                numNeighCells; /* no of meighbor cells */
   RruMCCH_Msg       *mbmsNeighCellsInfo; /* neighbor cell(s) information */
} RruMcchUnmodInfo;

typedef struct rruMcchMsg
{
   U8                mcchInfoBitMsk; /* bit mask for Access/Mod/Unmod Info */
   RruMCCH_Msg       mbmsAcsInfo; /* MBMS Access Information */
   RruMcchModInfo    mbmsModInfo; /* MBMS Modified Information */
   RruMcchUnmodInfo  mbmsUnmodInfo; /* MBMS Unmodified Information */
} RruMcchMsg;

typedef struct rruMschMsg
{
   U32    txCfn;                /* MSCH CFN for transmission */
   RruMSCH_Msg  mbmsSchedInfo;  /* MBMS Scheduling Information */
} RruMschMsg;

typedef struct rruMbmsDatReqSdus /* MBMS Data Req */
{
   CmMemListCp          memCp;  /* memory control point */
   RruHdr               hdr;    /* sdus Header */
   struct
   {
      U8                msgCategory; /* MCCH msg or MSCH msg */
      union
      {
         RruMcchMsg     mcchMsg;   /* MCCH message */
         RruMschMsg     mschMsg;   /* MSCH message */
      } u;
   } sdu;
} RruMbmsDatReqSdus;

typedef struct rruMbmsTxReq /* MBMS Transmit Req */
{
   RruHdr        hdr;            /* header */
   U8            mcchInfoBitMsk; /* bit mask for Access/Mod/Unmod Info */
   U32           mcchTxCfn;      /* Transmission Start CFN */
} RruMbmsTxReq;


/*******************************************************************RR**
  Interface Primitives
***********************************************************************/
typedef S16 (*RruBndReq)    ARGS((
               Pst          *pst,     /* post structure */
               SuId         suId,     /* RRU serivce user SAP ID */ 
               SpId         spId));   /* RRU SAP ID */

typedef S16 (*RruBndCfm)    ARGS((
               Pst          *pst,     /* post structure */       
               SuId         suId,     /* RRU Service user SAP ID */
               U8           status)); /* status structure for Bind confirm */

typedef S16 (*RruUbndReq)   ARGS((
               Pst          *pst,     /* post structure */
               SpId         spId,     /* RRU SAP ID */
               Reason       reason)); /* Reason for Unbind */

typedef S16 (*RruDatReq)    ARGS((
               Pst            *pst,          /* post structure */
               SpId           spId,          /* RRC Service provider SAP ID */
               RruDatReqSdus   *rruDatReqSdus  /* SDU */
               ));

typedef S16 (*RruCountCReq)    ARGS((
               Pst            *pst, /* post structure */
               SpId           spId, /* RRC Service provider SAP ID */
               RruUmtsUrnti   ueId /* Ue Id */
               ));

typedef S16 (*RruCountCInd)    ARGS((
               Pst            *pst,          /* post structure */
               SuId           suId,          /* RRC Service user SAP ID */
               RruCountCIndInfo   *rruCountCIndInfo  /* SDU */
               ));

typedef S16 (*RruDatInd)    ARGS((
               Pst            *pst,          /* post structure */
               SuId           suId,          /* RRC Service user SAP ID */
               RruDatIndSdus   *rruDatIndSdus  /* SDU */
               ));

typedef S16 (*RruDatRsp)    ARGS((
               Pst            *pst,          /* post structure */
               SpId           spId,          /* RRC Service provider SAP ID */
               RruDatRspSdus   *rruDatRspSdus  /* SDU */
               ));
typedef S16 (*RruEncDatReq)    ARGS((
               Pst            *pst,          /* post structure */
               SpId           spId,          /* RRC Service provider SAP ID */
               RruEncDatReqSdus   *rruEncDatReqSdus  /* SDU */
               ));
typedef S16 (*RruEncDatCfm)    ARGS((
               Pst            *pst,          /* post structure */
               SuId           suId,          /* RRC Service user SAP ID */
               RruEncDatCfmSdus   *rruEncDatCfmSdus  /* SDU */
               ));
typedef S16 (*RruDecDatReq)    ARGS((
               Pst            *pst,          /* post structure */
               SpId           spId,          /* RRC Service provider SAP ID */
               RruDecDatReqSdus   *rruDecDatReqSdus  /* SDU */
               ));
typedef S16 (*RruDecDatCfm)    ARGS((
               Pst            *pst,          /* post structure */
               SuId           suId,          /* RRC Service user SAP ID */
               RruDecDatCfmSdus   *rruDecDatCfmSdus  /* SDU */
               ));

typedef S16 (*RruErrInd)    ARGS((
               Pst            *pst,          /* post structure */
               SuId           suId,          /* RRC Service user SAP ID */
               RruErrIndSdus   *rruErrIndSdus  /* SDU */
               ));

typedef S16 (*RruCfgReq)    ARGS((
               Pst            *pst,          /* post structure */
               SpId           spId,          /* RRC Service provider SAP ID */
               RruCfgReqSdus   *rruCfgReqSdus  /* SDU */
               ));

typedef S16 (*RruCfgCfm)    ARGS((
               Pst            *pst,          /* post structure */
               SuId           suId,          /* RRC Service user SAP ID */
               RruCfgCfmSdus   *rruCfgCfmSdus  /* SDU */
               ));

typedef S16 (*RruMesCfgReq)    ARGS((
               Pst            *pst,          /* post structure */
               SpId           spId,          /* RRC Service provider SAP ID */
               RruMesCfgReqSdus   *rruMesCfgReqSdus  /* SDU */
               ));

typedef S16 (*RruMesRptInd)    ARGS((
               Pst            *pst,          /* post structure */
               SuId           suId,          /* RRC Service user SAP ID */
               RruMesRptIndSdus   *rruMesRptIndSdus  /* SDU */
               ));

typedef S16 (*RruStatusInd)    ARGS((
               Pst            *pst,          /* post structure */
               SuId           suId,          /* RRC Service user SAP ID */
               RruStatusIndSdus   *rruStatusIndSdus  /* SDU */
               ));

typedef S16 (*RruCnclUeReq)    ARGS((
               Pst            *pst,          /* post structure */
               SpId           spId,        /* RRC Service provider SAP ID */
               RruUmtsUrnti   *ueId     /* Ue Id */
               ));

typedef S16 (*RruPreDefCfgReq)    ARGS((
               Pst            *pst,          /* post structure */
               SpId           spId,          /* RRC Service provider SAP ID */
               RruPreDefCfgReqSdus   *rruPreDefCfgReqSdus  /* SDU */
               ));

typedef S16 (*RruCellCfgReq)    ARGS((
               Pst            *pst,          /* post structure */
               SpId           spId,          /* RRC Service provider SAP ID */
               RruCellCfgReqSdus   *rruCellCfgReqSdus  /* SDU */
               ));

typedef S16 (*RruCellCfgCfm)    ARGS((
               Pst            *pst,          /* post structure */
               SuId           suId,          /* RRC Service user SAP ID */
               RruCellCfgCfmSdus   *rruCellCfgCfmSdus  /* SDU */
               ));

typedef S16 (*RruDeleteTransReq)    ARGS((
               Pst            *pst,        /* post structure */
               SpId           spId,        /* RRC Service provider SAP ID */
               RruUmtsUrnti   ueId,       /* Ue Id */
               U32            transId      /* Transaction Id */
               ));
typedef S16 (*RruMbmsDatReq) ARGS((Pst *pst, SpId spId, RruMbmsDatReqSdus *rruMbmsDatReqSdus));
typedef S16 (*RruMbmsTransmitReq) ARGS((Pst *pst, SpId spId, RruMbmsTxReq *rruMbmsTxReq));

typedef S16 (*RruRelocInfoReq)    ARGS((
               Pst            *pst,     /* post structure */
               SpId           spId,     /* RRC Service provider SAP ID */
               RruSrnsRelocInfoReq *ueId
               ));

typedef S16 (*RruRelocPrepCfm)    ARGS((
               Pst            *pst,          /* post structure */
               SuId           suId,          /* RRC Service user SAP ID */
               RruRelocPrepInfoCfm *relocInd /* SDU */
               ));
typedef S16 (*RruRelocCommitCfm)    ARGS((
               Pst            *pst,          /* post structure */
               SuId           suId,          /* RRC Service user SAP ID */
               RruRelocCommitInfoCfm *relocInd /* SDU */
               ));

/* Extern declarations */
/* Extern for Pack functions */
EXTERN S16 cmPkRruBndReq ARGS((Pst *pst, SuId suId, SpId spId));
EXTERN S16 cmPkRruBndCfm ARGS((Pst *pst, SuId suId, U8 status));
EXTERN S16 cmPkRruUbndReq ARGS((Pst *pst, SpId spId, Reason reason));

EXTERN S16 cmPkRruSIB ARGS((RruSysInfoBlock *param, Buffer *mBuf));
EXTERN S16 cmPkRruDatReqUeMsg ARGS((RruDatReqUeMsg *param, Buffer *mBuf));
EXTERN S16 cmPkRruPcchMsg ARGS((RruDlPcchMsg *param,Buffer *mBuf));
EXTERN S16 cmPkRruPrimHdr ARGS((RruHdr *param, Buffer *mBuf));
EXTERN S16 cmPkRruTrchId ARGS((RruTrChId *param, Buffer *mBuf));
EXTERN S16 cmPkRruLogchId ARGS((RruLogChId *param, Buffer *mBuf));
EXTERN S16 cmPkRruTf ARGS((RruTf *param, Buffer *mBuf));
EXTERN S16 cmPkRruCiphCfgInfo ARGS((RruCiphCfgInfo *param, Buffer *mBuf));
EXTERN S16 cmPkRruPdcpHdrRfc3095Info ARGS((RruRfc3095Info *param, Buffer *mBuf));
EXTERN S16 cmPkRruPdcpHdrRfc2507Info ARGS((RruRfc2507Info *param, Buffer *mBuf));
EXTERN S16 cmPkRruPdcpHdrComprInfo ARGS(( RruPdcpHdrComprInfo *param, Buffer *mBuf));
EXTERN S16 cmPkRruPdcpCfgInfo ARGS((RruRbPdcpCfgInfo *param, Buffer *mBuf));
EXTERN S16 cmPkRruStatusInfo ARGS((RruStatusInfo *param, Buffer *mBuf));
EXTERN S16 cmPkRruDlAmRlcModeInfo ARGS((RruDlAmRlcModeInfo *param, Buffer *mBuf));
EXTERN S16 cmPkRruDlUmRlcModeInfo ARGS((RruDlUmRlcModeInfo *param, Buffer *mBuf));
EXTERN S16 cmPkRruTxRlcDiscardTmrExp ARGS((RruTmrBasedExp *param, Buffer *mBuf));
EXTERN S16 cmPkRruTxRlcDiscardMaxDatRetrans ARGS((RruMaxDatRetrans *param, Buffer *mBuf));
EXTERN S16 cmPkRruTxRlcDiscard ARGS((RruTxRlcDiscard *param, Buffer *mBuf));
EXTERN S16 cmPkRruDlTmRlcModeInfo ARGS((RruDlTmRlcModeInfo *param, Buffer *mBuf));
EXTERN S16 cmPkRruDlRlcMode ARGS((RruDlRlcMode *param, Buffer *mBuf));
EXTERN S16 cmPkRruPollInfo ARGS((RruPollInfo *param, Buffer *mBuf));
EXTERN S16 cmPkRruUlAmRlcModeInfo ARGS((RruUlAmRlcModeInfo *param, Buffer *mBuf));
EXTERN S16 cmPkRruUlUmRlcModeInfo ARGS((RruUlUmRlcModeInfo *param, Buffer *mBuf));
EXTERN S16 cmPkRruUlTmRlcModeInfo ARGS((RruUlTmRlcModeInfo *param, Buffer *mBuf));
EXTERN S16 cmPkRruUlRlcMode ARGS((RruUlRlcMode *param, Buffer *mBuf));
EXTERN S16 cmPkRruDlLogChMapInfo ARGS((RruDlLogChMapInfo *param, Buffer *mBuf));
EXTERN S16 cmPkRruHsDschInfo ARGS((RruHsDschInfo *param, Buffer *mBuf));
EXTERN S16 cmPkRruHsDschQElm ARGS((RruHsDschQElm *param, Buffer *mBuf));
EXTERN S16 cmPkRruRlcSizeList ARGS((RruRbRlcSizeList *param, Buffer *mBuf));
EXTERN S16 cmPkRruUlLogChMapInfo ARGS((RruUlLogChMapInfo *param, Buffer *mBuf));
EXTERN S16 cmPkRruEDchInfo ARGS((RruEDchInfo *param, Buffer *mBuf));
EXTERN S16 cmPkRruEDchActvSetInfo ARGS((RruEDchActvSetInfo *param, Buffer *mBuf));
EXTERN S16 cmPkRruUlRbCfg ARGS((RruUlRbCfg *param,Buffer *mBuf));
EXTERN S16 cmPkRruDlRbCfg ARGS((RruDlRbCfg *param,Buffer *mBuf));
EXTERN S16 cmPkRruRbCfgInfo ARGS((RruRbCfgInfo *param, Buffer *mBuf));
EXTERN S16 cmPkRruTfcCfg ARGS((RruTfcCfg *param,Buffer *mBuf));
EXTERN S16 cmPkRruTfcsCfgList ARGS((RruTfcsCfgList *param, Buffer *mBuf));
EXTERN S16 cmPkRruTfsCfgList ARGS((RruTfsCfgList *param, Buffer *mBuf));
EXTERN S16 cmPkRruCcTrchCfgInfo ARGS((RruCctrchCfgInfo *param, Buffer *mBuf));
EXTERN S16 cmPkRruCcTrchCfgInfoList ARGS((RruCctrchCfgAllList *param, Buffer *mBuf));
EXTERN S16 cmPkRruRbAllCfgList ARGS((RruRbCfgAllList *param, Buffer *mBuf));
EXTERN  S16 cmPkRruUeCfgInfo ARGS((RruUeCfgInfo *param,Buffer *mBuf));
EXTERN S16 cmPkRruActvnTime ARGS((RruActvnTime *param,Buffer *mBuf));
EXTERN S16 cmPkRruExpCfg ARGS((RruExpCfg *param, Buffer *mBuf));
EXTERN S16 cmPkRruDefaultCfg ARGS((RruDefCfg *param, Buffer *mBuf));
EXTERN S16 cmPkRbInfoToChg ARGS((RruRbInfoToChg *param, Buffer *mBuf));
EXTERN S16 cmPkRbInfoToChgLst ARGS((RruRbInfoToChgLst *param, Buffer *mBuf));
EXTERN S16 cmPkRruPreDefCfg ARGS((RruPreDefCfg *param, Buffer *mBuf));
EXTERN S16 cmPkRruDatReqUeCfg ARGS((RruUeCfgAtRnc *param, Buffer *mBuf));
EXTERN S16 cmPkRruDatReqSdus ARGS((RruDatReqSdus *param, Buffer *mBuf));
EXTERN S16 cmPkRruDatIndSdus ARGS((RruDatIndSdus *param, Buffer *mBuf));
EXTERN S16 cmPkRruDatRspSdus ARGS((RruDatRspSdus *param, Buffer *mBuf));
EXTERN S16 cmPkRruEncDatReqSdus ARGS((RruEncDatReqSdus *param, Buffer *mBuf));
EXTERN S16 cmPkRruEncDatCfmSdus ARGS((RruEncDatCfmSdus *param, Buffer *mBuf));
EXTERN S16 cmPkRruDecDatReqSdus ARGS((RruDecDatReqSdus *param, Buffer *mBuf));
EXTERN S16 cmPkRruDecDatCfmSdus ARGS((RruDecDatCfmSdus *param, Buffer *mBuf));
EXTERN S16 cmPkRruError ARGS((RruError *param, Buffer *mBuf));
EXTERN S16 cmPkRruErrIndSdus ARGS((RruErrIndSdus *param, Buffer *mBuf));
EXTERN S16 cmPkRruCfgReqSdus ARGS((RruCfgReqSdus *param, Buffer *mBuf));
EXTERN S16 cmPkRruCfgErrInfo ARGS((RruCfgErrorInfo *param, Buffer *mBuf));
EXTERN S16 cmPkRruCfgCfmSdus ARGS((RruCfgCfmSdus *param, Buffer *mBuf));
EXTERN S16 cmPkRruMkMesCfg ARGS((RruMkMesCfg *param,Buffer *mBuf));
EXTERN S16 cmPkRruMesCfgReqSdus ARGS((RruMesCfgReqSdus *param,Buffer *mBuf));
EXTERN S16 cmPkRruMacMesRpt ARGS((RruMacMesRpt *param,Buffer *mBuf));
EXTERN S16 cmPkRruMesRptIndSdus ARGS((RruMesRptIndSdus *param,Buffer *mBuf));
EXTERN S16 cmPkRruStatusIndSdus ARGS((RruStatusIndSdus *param,Buffer *mBuf));
EXTERN S16 cmPkRruPreDefCfgInfo ARGS((RruPreDefCfgInfo *param,Buffer *mBuf));
EXTERN S16 cmPkRruPreDefCfgReqSdus ARGS((RruPreDefCfgReqSdus *param,Buffer *mBuf));
EXTERN S16 cmPkRruRbCfgList ARGS((RruRbCfgList *param,Buffer *mBuf));
EXTERN S16 cmPkRruCellCfgInfo ARGS((RruCellCfgInfo *param,Buffer *mBuf));
EXTERN S16 cmPkRruCellCfgReqSdus ARGS((RruCellCfgReqSdus *param,Buffer *mBuf));
EXTERN S16 cmPkRruCfgErrorInfo ARGS((RruCfgErrorInfo *param,Buffer *mBuf));
EXTERN S16 cmPkRruCellCfgCfmSdus ARGS((RruCellCfgCfmSdus *param,Buffer *mBuf));
EXTERN S16 cmPkRruMbmsDatReqSdus ARGS((RruMbmsDatReqSdus *param, Buffer *mBuf));
EXTERN S16 cmPkRruMcchMsg ARGS((RruMcchMsg *param, Buffer *mBuf));
EXTERN S16 cmPkRruMcchUnmodInfo ARGS((RruMcchUnmodInfo *param, Buffer *mBuf));
EXTERN S16 cmPkRruMcchModInfo ARGS((RruMcchModInfo *param, Buffer *mBuf));
EXTERN S16 cmPkRruMschMsg ARGS((RruMschMsg *param, Buffer *mBuf));
#ifdef RRC_MULT_SAP_SUPPRT
EXTERN S16 cmPkRruLLyrSapIdInfo ARGS((RruLLyrSapIdInfo *param,Buffer *mBuf));
#endif

/* Extern for Primitive Pack functions */
EXTERN S16 cmPkRruDatReq ARGS((Pst *pst, SpId spId, RruDatReqSdus *rruDatReqSdus));
EXTERN S16 cmPkRruDatInd ARGS((Pst *pst, SuId suId, RruDatIndSdus *rruDatIndSdus));
EXTERN S16 cmPkRruCountCInd ARGS((Pst *pst, SuId suId, RruCountCIndInfo *rruCountCIndInfo));
EXTERN S16 cmPkRruDatRsp ARGS((Pst *pst, SpId spId, RruDatRspSdus *rruDatRspSdus));
EXTERN S16 cmPkRruEncDatReq ARGS((Pst *pst, SpId spId, RruEncDatReqSdus *rruEncDatReqSdus));
EXTERN S16 cmPkRruEncDatCfm ARGS((Pst *pst, SuId suId, RruEncDatCfmSdus *rruEncDatCfmSdus));
EXTERN S16 cmPkRruDecDatReq ARGS((Pst *pst, SpId spId, RruDecDatReqSdus *rruDecDatReqSdus));
EXTERN S16 cmPkRruDecDatCfm ARGS((Pst *pst, SuId suId, RruDecDatCfmSdus *rruDecDatCfmSdus));
EXTERN S16 cmPkRruErrInd ARGS((Pst *pst, SuId suId, RruErrIndSdus *rruErrIndSdus));
EXTERN S16 cmPkRruCfgReq ARGS((Pst *pst, SpId spId, RruCfgReqSdus *rruCfgReqSdus));
EXTERN S16 cmPkRruCfgCfm ARGS((Pst *pst, SpId spId, RruCfgCfmSdus *rruCfgCfmSdus));
EXTERN S16 cmPkRruMesCfgReq ARGS((Pst *pst,SpId spId,RruMesCfgReqSdus *rruMesCfgReqSdus));
EXTERN S16 cmPkRruMesRptInd ARGS((Pst *pst,SpId spId,RruMesRptIndSdus *rruMesRptIndSdus));
EXTERN S16 cmPkRruStatusInd ARGS((Pst *pst,SpId spId,RruStatusIndSdus *rruStatusIndSdus));
EXTERN S16 cmPkRruCancelUeReq ARGS((Pst *pst,SpId spId,RruUmtsUrnti *ueId));
EXTERN S16 cmPkRruPreDefCfgReq ARGS((Pst *pst,SpId spId,RruPreDefCfgReqSdus *rruPreDefCfgReqSdus));
EXTERN S16 cmPkRruCellCfgReq ARGS((Pst *pst,SpId spId,RruCellCfgReqSdus *rruCellCfgReqSdus));
EXTERN S16 cmPkRruCellCfgCfm ARGS((Pst *pst,SpId spId,RruCellCfgCfmSdus *rruCellCfgCfmSdus));
EXTERN S16 cmPkRruDeleteTransReq ARGS((Pst *pst,SpId spId,RruUmtsUrnti ueId,U32 transId));
EXTERN S16 cmPkRruCountCReq ARGS((Pst *pst,SpId spId,RruUmtsUrnti ueId));
EXTERN S16 cmPkRruMbmsDatReq ARGS((Pst *pst, SpId spId, RruMbmsDatReqSdus *rruMbmsDatReqSdus));
EXTERN S16 cmPkRruMbmsTxReq ARGS((Pst *pst, SpId spId, RruMbmsTxReq *rruMbmsTxReq));
EXTERN S16 cmPkRruCiphKeyAlgo ARGS((RruCiphKeyAlgoInfo *param, Buffer *mBuf));
EXTERN S16 cmPkRruCiphInfo ARGS((RruCiphInfo *param, Buffer *mBuf));
EXTERN S16 cmPkRruSrbIntgInfo ARGS((RruSrbIntgInfo *param, Buffer *mBuf));
EXTERN S16 cmPkRruIntgInfo ARGS((RruIntgProtInfo *param, Buffer *mBuf));
EXTERN S16 cmPkRruSecCfgInfo ARGS((RruSecCfg *param, Buffer *mBuf));
EXTERN S16 cmPkRruRelocCommitInfoReq ARGS((Pst *pst,SpId spId,RruSrnsRelocInfoReq *ueId));
EXTERN S16 cmPkRruRelocPrepInfoReq ARGS((Pst *pst,SpId spId,RruSrnsRelocInfoReq *ueId));
EXTERN S16 cmPkPdcpBuf ARGS((U16 numSdu,Buffer **sduBuf,Buffer **mBuf));
EXTERN S16 cmPkRruRohcCmnDb ARGS((RruRohcCmnDb *cmDb,Buffer  *mBuf)); 
EXTERN S16 cmPkRruSeamlessRelocInfo ARGS((Pst *pst,RruPdcpSeamlessRelocInfo *param,Buffer *mBuf));
EXTERN S16 cmPkRruLosslessRelocInfo ARGS((Pst *pst,RruPdcpLosslessRelocInfo *param,Buffer *mBuf));
EXTERN S16 cmPkRruRelocPrepInfoCfm ARGS((Pst *pst,SuId suId,RruRelocPrepInfoCfm *rruRelocInd));
EXTERN S16 cmPkRruRelocCommitInfoCfm ARGS((Pst *pst,SuId suId,RruRelocCommitInfoCfm *rruRelocInd));
EXTERN S16 cmPkRruRelocCommitInfo ARGS((Pst *pst,RruRelocCommitInfo *param,Buffer *mBuf));
EXTERN S16 cmPkRruRelocPrepInfo ARGS((Pst *pst,RruRelocPrepInfo *param,Buffer *mBuf));
EXTERN S16 cmPkRruTmCountCInfo ARGS((Pst *pst,RruTmCountCInfo *param,Buffer *mBuf));
EXTERN S16 cmPkRruCountIInfo ARGS((Pst *pst,RruCountIInfo *param,Buffer *mBuf));
#ifdef RRC_IUR_SUPP
EXTERN S16 cmPkRruDrncCfgForDUe ARGS((RruDrncCfgForDUe *param,Buffer *mBuf));
EXTERN S16 cmPkRruSrncCfgForDUe ARGS((RruSrncCfgForDUe *param,Buffer *mBuf));
EXTERN S16 cmPkRruFachCfg ARGS((RruFachConfig *param,Buffer *mBuf));
#endif

/*rru_x_001.main_11 - Fixes for compilation warnings*/

/* Extern for Unpack functions */
EXTERN S16 cmUnpkRruBndReq ARGS((RruBndReq func,Pst *pst,Buffer *mBuf));
EXTERN S16 cmUnpkRruBndCfm ARGS((RruBndCfm func,Pst *pst,Buffer *mBuf));
EXTERN S16 cmUnpkRruUbndReq ARGS((RruUbndReq func,Pst *pst,Buffer *mBuf));
EXTERN S16 cmUnpkRruPrimHdr ARGS((RruHdr *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruSIB ARGS((RruSysInfoBlock *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruDatReqUeMsg ARGS((RruDatReqUeMsg *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruPcchMsg ARGS((RruDlPcchMsg *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruTrchId ARGS((RruTrChId *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruLogchId ARGS((RruLogChId *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruRlcSizeList ARGS((RruRbRlcSizeList *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruUlLogChMapInfo ARGS((RruUlLogChMapInfo *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruEDchInfo ARGS((RruEDchInfo *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruEDchActvSetInfo ARGS((RruEDchActvSetInfo *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruDlLogChMapInfo ARGS((RruDlLogChMapInfo *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruHsDschInfo ARGS((RruHsDschInfo *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruHsDschQElm ARGS((RruHsDschQElm *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruTxRlcDiscardTmrExp ARGS((RruTmrBasedExp *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruTxRlcDiscardMaxDatRetrans ARGS((RruMaxDatRetrans *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruTxRlcDiscard ARGS((RruTxRlcDiscard *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruPollInfo ARGS((RruPollInfo *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruUlAmRlcModeInfo ARGS((RruUlAmRlcModeInfo *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruUlUmRlcModeInfo ARGS((RruUlUmRlcModeInfo *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruUlTmRlcModeInfo ARGS((RruUlTmRlcModeInfo *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruUlRlcMode ARGS((RruUlRlcMode *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruStatusInfo ARGS((RruStatusInfo *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruDlAmRlcModeInfo ARGS((RruDlAmRlcModeInfo *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruDlUmRlcModeInfo ARGS((RruDlUmRlcModeInfo *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruDlTmRlcModeInfo ARGS((RruDlTmRlcModeInfo *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruDlRlcMode ARGS((RruDlRlcMode *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruPdcpHdrRfc2507Info ARGS((RruRfc2507Info *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruPdcpHdrRfc3095Info ARGS((RruRfc3095Info *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruPdcpHdrComprInfo ARGS((RruPdcpHdrComprInfo *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruPdcpCfgInfo ARGS((RruRbPdcpCfgInfo *param,Ptr ptr,Buffer **mBuf));
EXTERN S16 cmUnpkRruCiphCfgInfo ARGS((RruCiphCfgInfo *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruCiphInfo ARGS((RruCiphInfo *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkRruUlRbCfg ARGS((RruUlRbCfg *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruDlRbCfg ARGS((RruDlRbCfg *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruRbCfgInfo ARGS((RruRbCfgInfo *param,Ptr ptr,Buffer **mBuf));
EXTERN S16 cmUnpkRruRbAllCfgList ARGS((RruRbCfgAllList *param,Ptr ptr,Buffer **mBuf));
EXTERN S16 cmUnpkRruTf ARGS((RruTf *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruTfsCfgList ARGS((RruTfsCfgList *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruTfcCfg ARGS((RruTfcCfg *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruTfcsCfgList ARGS((RruTfcsCfgList *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruCcTrchCfgInfo ARGS((RruCctrchCfgInfo *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruCcTrchCfgInfoList ARGS((RruCctrchCfgAllList *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruUeCfgInfo ARGS((RruUeCfgInfo *param,Buffer *mBuf));
EXTERN S16 cmUnpkRruActvnTime ARGS((RruActvnTime *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruExpCfg ARGS((RruExpCfg *param,Ptr ptr,Buffer **mBuf));
EXTERN S16 cmUnpkRruDefaultCfg ARGS((RruDefCfg *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRbInfoToChg ARGS((RruRbInfoToChg *param, Buffer *mBuf));
EXTERN S16 cmUnpkRbInfoToChgLst ARGS((RruRbInfoToChgLst *param, Buffer *mBuf));
EXTERN S16 cmUnpkRruPreDefCfg ARGS((RruPreDefCfg *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruDatReqUeCfg ARGS((RruUeCfgAtRnc *param,Ptr ptr,Buffer **mBuf));
EXTERN S16 cmUnpkRruDatReqSdus ARGS((RruDatReqSdus *param,Mem *sMem,Size maxBlkSize,Buffer **mBuf));
/* rru_x_001.main_11 : Fixes for compilation warnings */

EXTERN S16 cmUnpkRruDatIndSdus ARGS((RruDatIndSdus *param,Mem *sMem,Size maxBlkSize,Buffer *mBuf));
EXTERN S16 cmUnpkRruDatRspSdus ARGS((RruDatRspSdus *param,Mem *sMem,Size maxBlkSize,Buffer *mBuf));
EXTERN S16 cmUnpkRruEncDatReqSdus ARGS((RruEncDatReqSdus *param,Mem *sMem,Size maxBlkSize,Buffer *mBuf));
EXTERN S16 cmUnpkRruEncDatCfmSdus ARGS((RruEncDatCfmSdus *param,Mem *sMem,Size maxBlkSize,Buffer *mBuf));
EXTERN S16 cmUnpkRruDecDatReqSdus ARGS((RruDecDatReqSdus *param,Mem *sMem,Size maxBlkSize,Buffer *mBuf));
EXTERN S16 cmUnpkRruDecDatCfmSdus ARGS((RruDecDatCfmSdus *param,Mem *sMem,Size maxBlkSize,Buffer *mBuf));
EXTERN S16 cmUnpkRruError ARGS((RruError *param,Buffer *mBuf));
EXTERN S16 cmUnpkRruErrIndSdus ARGS((RruErrIndSdus *param,Mem *sMem,Size maxBlkSize,Buffer *mBuf));
EXTERN S16 cmUnpkRruCfgReqSdus ARGS((RruCfgReqSdus *param,Mem *sMem,Size maxBlkSize,Buffer **mBuf));
EXTERN S16 cmUnpkRruCfgErrInfo ARGS((RruCfgErrorInfo *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruCfgCfmSdus ARGS((RruCfgCfmSdus *param,Mem *sMem,Size maxBlkSize,Buffer *mBuf));
EXTERN S16 cmUnpkRruMkMesCfg ARGS((RruMkMesCfg *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruMesCfgReqSdus ARGS((RruMesCfgReqSdus *param,Mem *sMem,Size maxBlkSize,Buffer *mBuf));
EXTERN S16 cmUnpkRruMacMesRpt ARGS((RruMacMesRpt *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruMesRptIndSdus ARGS((RruMesRptIndSdus *param,Mem *sMem,Size maxBlkSize,Buffer *mBuf));
EXTERN S16 cmUnpkRruStatusIndSdus ARGS((RruStatusIndSdus *param,Mem *sMem,Size maxBlkSize,Buffer *mBuf));
EXTERN S16 cmUnpkRruPreDefCfgInfo ARGS((RruPreDefCfgInfo *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruPreDefCfgReqSdus ARGS((RruPreDefCfgReqSdus *param,Mem *sMem,Size maxBlkSize,Buffer *mBuf));
EXTERN S16 cmUnpkRruRbCfgList ARGS((RruRbCfgList *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruCellCfgInfo ARGS((RruCellCfgInfo *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruCellCfgReqSdus ARGS((RruCellCfgReqSdus *param,Mem *sMem,Size maxBlkSize,Buffer *mBuf));
EXTERN S16 cmUnpkRruCfgErrorInfo ARGS((RruCfgErrorInfo *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruCellCfgCfmSdus ARGS((RruCellCfgCfmSdus *param,Mem *sMem,Size maxBlkSize,Buffer *mBuf));
EXTERN S16 cmUnpkRruMbmsDatReqSdus ARGS((RruMbmsDatReqSdus *param, Buffer *mBuf));
EXTERN S16 cmUnpkRruMcchMsg ARGS((RruMcchMsg *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkRruMcchUnmodInfo ARGS((RruMcchUnmodInfo *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkRruMcchModInfo ARGS((RruMcchModInfo *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkRruMschMsg ARGS((RruMschMsg *param, Ptr ptr, Buffer *mBuf));
#ifdef RRC_MULT_SAP_SUPPRT
EXTERN S16 cmUnpkRruLLyrSapIdInfo ARGS((RruLLyrSapIdInfo *param,Ptr ptr,Buffer *mBuf));
#endif

/* Extern for Primitive Unpack functions */
EXTERN S16 cmUnpkRruDatReq ARGS((RruDatReq func,Pst *pst,Buffer *mBuf,Mem *sMem, Size maxBlkSize));
EXTERN S16 cmUnpkRruDatInd ARGS((RruDatInd func,Pst *pst,Buffer *mBuf,Mem *sMem,Size maxBlkSize));
EXTERN S16 cmUnpkRruCountCInd ARGS((RruCountCInd func,Pst *pst,Buffer *mBuf,Mem *sMem,Size maxBlkSize));
EXTERN S16 cmUnpkRruDatRsp ARGS((RruDatRsp func,Pst *pst,Buffer *mBuf,Mem *sMem,Size maxBlkSize));
EXTERN S16 cmUnpkRruEncDatReq ARGS((RruEncDatReq func,Pst *pst,Buffer *mBuf,Mem *sMem,Size maxBlkSize));
EXTERN S16 cmUnpkRruEncDatCfm ARGS((RruEncDatCfm func,Pst *pst,Buffer *mBuf,Mem *sMem,Size maxBlkSize));
EXTERN S16 cmUnpkRruDecDatReq ARGS((RruDecDatReq func,Pst *pst,Buffer *mBuf,Mem *sMem,Size maxBlkSize));
EXTERN S16 cmUnpkRruDecDatCfm ARGS((RruDecDatCfm func,Pst *pst,Buffer *mBuf,Mem *sMem,Size maxBlkSize));
EXTERN S16 cmUnpkRruErrInd ARGS((RruErrInd func,Pst *pst,Buffer *mBuf,Mem *sMem,Size maxBlkSize));
EXTERN S16 cmUnpkRruCfgReq ARGS((RruCfgReq func,Pst *pst,Buffer *mBuf,Mem *sMem,Size maxBlkSize));
EXTERN S16 cmUnpkRruCfgCfm ARGS((RruCfgCfm func,Pst *pst,Buffer *mBuf,Mem *sMem,Size maxBlkSize));
EXTERN S16 cmUnpkRruMesCfgReq ARGS((RruMesCfgReq func,Pst *pst,Buffer *mBuf,Mem *sMem,Size maxBlkSize));
EXTERN S16 cmUnpkRruMesRptInd ARGS((RruMesRptInd func,Pst *pst,Buffer *mBuf,Mem *sMem,Size maxBlkSize));
EXTERN S16 cmUnpkRruStatusInd ARGS((RruStatusInd func,Pst *pst,Buffer *mBuf,Mem *sMem,Size maxBlkSize));
EXTERN S16 cmUnpkRruCancelUeReq ARGS((RruCnclUeReq func,Pst *pst,Buffer *mBuf));
EXTERN S16 cmUnpkRruPreDefCfgReq ARGS((RruPreDefCfgReq func,Pst *pst,Buffer *mBuf,Mem *sMem,Size maxBlkSize));
EXTERN S16 cmUnpkRruCellCfgReq ARGS((RruCellCfgReq func,Pst *pst,Buffer *mBuf,Mem *sMem,Size maxBlkSize));
EXTERN S16 cmUnpkRruCellCfgCfm ARGS((RruCellCfgCfm func,Pst *pst,Buffer *mBuf,Mem *sMem,Size maxBlkSize));
EXTERN S16 cmUnpkRruDeleteTransReq ARGS((RruDeleteTransReq func, Pst *pst,Buffer *mBuf));
EXTERN S16 cmUnpkRruCountCReq ARGS((RruCountCReq func, Pst *pst,Buffer *mBuf, Mem *sMem, Size maxBlkSize));
EXTERN S16 cmUnpkRruMbmsDatReq ARGS((RruMbmsDatReq func,  Pst *pst, Buffer *mBuf, Mem *sMem, Size maxBlkSize));
EXTERN S16 cmUnpkRruMbmsTxReq ARGS((RruMbmsTransmitReq func,  Pst *pst, Buffer *mBuf, Mem *sMem, Size maxBlkSize));
EXTERN S16 cmUnpkRruCiphKeyAlgo ARGS((RruCiphKeyAlgoInfo *param, Buffer *mBuf));
EXTERN S16 cmUnpkRruSrbIntgInfo ARGS((RruSrbIntgInfo *param, Buffer *mBuf));
EXTERN S16 cmUnpkRruIntgInfo ARGS((RruIntgProtInfo *param, Buffer *mBuf));
EXTERN S16 cmUnpkRruSecCfgInfo ARGS((RruSecCfg *param, Ptr ptr, Buffer *mBuf));
EXTERN S16 cmUnpkRruRohcCmnDb ARGS((RruRohcCmnDb *cmDb,Buffer  *mBuf));
EXTERN S16 cmUnpkRruSeamlessRelocInfo ARGS((RruPdcpSeamlessRelocInfo *param,Ptr ptr,Buffer  **mBuf));
EXTERN S16 cmUnpkRruLosslessRelocInfo ARGS((Pst *pst,RruPdcpLosslessRelocInfo *param,
Ptr  ptr,Buffer  **mBuf));
EXTERN S16 cmUnpkPdcpBuf ARGS((U16    numBuf,Buffer **sduBuf,Buffer **mBuf)); 
EXTERN S16 cmUnpkRruRelocPrepInfoReq ARGS((RruRelocInfoReq func,Pst *pst,Buffer *mBuf));
EXTERN S16 cmUnpkRruRelocCommitInfoReq ARGS((RruRelocInfoReq func,Pst *pst,Buffer *mBuf));
EXTERN S16 cmUnpkRruRelocPrepInfoCfm ARGS(( RruRelocPrepCfm func,Pst *pst,Buffer *mBuf,Mem *sMem,Size maxBlkSize) );
EXTERN S16 cmUnpkRruRelocCommitInfoCfm ARGS(( RruRelocCommitCfm func,Pst *pst,Buffer *mBuf,Mem *sMem,Size maxBlkSize) );
EXTERN S16 cmUnpkRruPdcpRelocInfo ARGS ((RruPdcpRelocInfo *param,Ptr ptr,Buffer **mBuf));

EXTERN S16 cmUnpkRruRelocPrepInfo ARGS((Pst *pst,RruRelocPrepInfo *relocInfo,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruTmCountCInfo ARGS((Pst *pst,RruTmCountCInfo *tmCountCInfo,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruCountIInfo ARGS((Pst *pst,RruCountIInfo *countI,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruRelocCommitInfo ARGS((Pst *pst,RruRelocCommitInfo *relocInfo,Ptr ptr,Buffer **mBuf));
#ifdef RRC_IUR_SUPP
EXTERN S16 cmUnpkRruDrncCfgForDUe ARGS((RruDrncCfgForDUe *param,Ptr ptr,Buffer *mBuf));
EXTERN S16 cmUnpkRruSrncCfgForDUe ARGS((RruSrncCfgForDUe *param, Buffer *mBuf));
EXTERN S16 cmUnpkRruFachCfg ARGS((RruFachConfig  *param,Buffer *mBuf));

#endif

/*rru_x_001.main_11 - Fixes for compilation warnings*/

/*rru_x_001.main_15 - ccpu00129401 */
#ifdef RX /* For RRC User */
EXTERN S16 RxLiRruBndCfm ARGS((Pst *pst, SuId suId, U8 status));
EXTERN S16 RxLiRruDatInd ARGS((Pst *pst, SpId spId, RruDatIndSdus *rruDatIndSdus));
EXTERN S16 RxLiRruCountCInd ARGS((Pst *pst, SpId spId, RruCountCIndInfo *rruCountCIndInfo));
EXTERN S16 RxLiRruEncDatCfm ARGS((Pst *pst, SpId spId, RruEncDatCfmSdus *rruEncDatCfmSdus));
EXTERN S16 RxLiRruDecDatCfm ARGS((Pst *pst, SpId spId, RruDecDatCfmSdus *rruDecDatCfmSdus));
EXTERN S16 RxLiRruErrInd ARGS((Pst *pst, SpId spId, RruErrIndSdus *rruErrIndSdus));
EXTERN S16 RxLiRruCfgCfm ARGS((Pst *pst, SpId spId, RruCfgCfmSdus *rruCfgCfmSdus));
EXTERN S16 RxLiRruMesRptInd ARGS((Pst *pst, SpId spId, RruMesRptIndSdus *rruMesRptIndSdus));
EXTERN S16 RxLiRruStatusInd ARGS((Pst *pst, SpId spId, RruStatusIndSdus *rruStatusIndSdus));
EXTERN S16 RxLiRruCellCfgCfm ARGS((Pst *pst, SpId spId, RruCellCfgCfmSdus *rruCellCfgCfmSdus));
EXTERN S16 RxLiRruRelocPrepInfoCfm ARGS((Pst *pst, SpId spId, RruRelocPrepInfoCfm *rruInfo));
EXTERN S16 RxLiRruRelocCommitInfoCfm ARGS((Pst *pst, SpId spId, RruRelocCommitInfoCfm *rruInfo));
#endif /* RX */
#ifdef QY /* For RRC User */
EXTERN S16 QyLiRruBndReq ARGS((Pst *pst, SuId suId, SpId spId));
EXTERN S16 QyLiRruEncDatReq ARGS((Pst *pst, SpId spId, RruEncDatReqSdus *rruEncDatReqSdus, RruEncDatCfmSdus *rruEncDatCfmSdus));
EXTERN S16 QyLiRruDecDatReq  ARGS(( Pst *pst, SpId spId, RruDecDatReqSdus *rruDecDatReqSdus, RruDecDatCfmSdus *rruDecDatCfmSdus));
EXTERN S16 QyLiRruCellCfgReq ARGS((Pst *pst, SpId spId, RruCellCfgReqSdus *rruCellCfgReqSdus));
EXTERN S16 QyLiRruUbndReq ARGS((Pst *pst, SpId spId, Reason reason));

 
EXTERN S16 QyLiRruBndCfm ARGS((Pst *pst, SuId suId, U8 status));
EXTERN S16 QyLiRruDatInd ARGS((Pst *pst, SpId spId, RruDatIndSdus *rruDatIndSdus));
EXTERN S16 QyLiRruCountCInd ARGS((Pst *pst, SpId spId, RruCountCIndInfo *rruCountCIndInfo));
EXTERN S16 QyLiRruEncDatCfm ARGS((Pst *pst, SpId spId, RruEncDatCfmSdus *rruEncDatCfmSdus));
EXTERN S16 QyLiRruDecDatCfm ARGS((Pst *pst, SpId spId, RruDecDatCfmSdus *rruDecDatCfmSdus));
EXTERN S16 QyLiRruErrInd ARGS((Pst *pst, SpId spId, RruErrIndSdus *rruErrIndSdus));
EXTERN S16 QyLiRruCfgCfm ARGS((Pst *pst, SpId spId, RruCfgCfmSdus *rruCfgCfmSdus));
EXTERN S16 QyLiRruMesRptInd ARGS((Pst *pst, SpId spId, RruMesRptIndSdus *rruMesRptIndSdus));
EXTERN S16 QyLiRruStatusInd ARGS((Pst *pst, SpId spId, RruStatusIndSdus *rruStatusIndSdus));
EXTERN S16 QyLiRruCellCfgCfm ARGS((Pst *pst, SpId spId, RruCellCfgCfmSdus *rruCellCfgCfmSdus));
EXTERN S16 QyLiRruRelocPrepInfoCfm ARGS((Pst *pst, SpId spId, RruRelocPrepInfoCfm *rruInfo));
EXTERN S16 QyLiRruRelocCommitInfoCfm ARGS((Pst *pst, SpId spId, RruRelocCommitInfoCfm *rruInfo));
#endif /* QY */

#ifdef RR

/* RRC to RRC user */
EXTERN S16 RrUiRruBndCfm ARGS((Pst *pst, SuId suId, U8 status));
EXTERN S16 RrUiRruDatInd ARGS((Pst *pst, SpId spId, RruDatIndSdus *rruDatIndSdus));
EXTERN S16 RrUiRruEncDatCfm ARGS((Pst *pst, SpId spId, RruEncDatCfmSdus *rruEncDatCfmSdus));
EXTERN S16 RrUiRruDecDatCfm ARGS((Pst *pst, SpId spId, RruDecDatCfmSdus *rruDecDatCfmSdus));
EXTERN S16 RrUiRruErrInd ARGS((Pst *pst, SpId spId, RruErrIndSdus *rruErrIndSdus));
EXTERN S16 RrUiRruCfgCfm ARGS((Pst *pst, SpId spId, RruCfgCfmSdus *rruCfgCfmSdus));
EXTERN S16 RrUiRruMesRptInd ARGS((Pst *pst, SpId spId, RruMesRptIndSdus *rruMesRptIndSdus));
EXTERN S16 RrUiRruStatusInd ARGS((Pst *pst, SpId spId, RruStatusIndSdus *rruStatusIndSdus));
EXTERN S16 RrUiRruCellCfgCfm ARGS((Pst *pst, SpId spId, RruCellCfgCfmSdus *rruCfgCellCfmSdus));
EXTERN S16 RrUiRruCountCInd ARGS((Pst *pst, SpId spId, RruCountCIndInfo *rruCountCIndInfo));

/* RRC user to RRC */
EXTERN S16 RrUiRruBndReq ARGS((Pst *pst, SuId suId, SpId spId));
EXTERN S16 RrUiRruUbndReq ARGS((Pst *pst, SpId spId, Reason reason));
EXTERN S16 RrUiRruDatReq ARGS((Pst *pst, SpId spId, RruDatReqSdus *rruDatReqSdus));
EXTERN S16 RrUiRruDatRsp ARGS((Pst *pst, SpId spId, RruDatRspSdus *rruDatRspSdus));
#ifdef RRC_MT
EXTERN S16 rrPstDatReqToWrkrThrd ARGS((Pst *pst, SpId spId, RruDatReqSdus *rruDatReqSdus));
EXTERN S16 rrPstDatRspToWrkrThrd ARGS((Pst *pst, SpId spId, RruDatRspSdus *rruDatRspSdus));
EXTERN S16 rrPstCfgReqToWrkrThrd ARGS((Pst *pst, SpId spId, RruCfgReqSdus *rruCfgReqSdus));
#endif
EXTERN S16 RrUiRruEncDatReq ARGS((Pst *pst, SpId spId, RruEncDatReqSdus *rruEncDatReqSdus));
EXTERN S16 RrUiRruDecDatReq ARGS((Pst *pst, SpId spId, RruDecDatReqSdus *rruDecDatReqSdus));
EXTERN S16 RrUiRruCfgReq ARGS((Pst *pst, SpId spId, RruCfgReqSdus *rruCfgReqSdus));
EXTERN S16 RrUiRruMesCfgReq ARGS((Pst *pst, SpId spId, RruMesCfgReqSdus *rruMesCfgReqSdus));
EXTERN S16 RrUiRruCnclUeReq ARGS((Pst *pst, SpId spId, RruUmtsUrnti *ueId));
EXTERN S16 RrUiRruPreDefCfgReq ARGS((Pst *pst, SpId spId, RruPreDefCfgReqSdus *rruPreDefCfgReqSdus));
EXTERN S16 RrUiRruCellCfgReq ARGS((Pst *pst, SpId spId, RruCellCfgReqSdus *rruCellCfgReqSdus));
EXTERN S16 RrUiRruDeleteTransReq ARGS((Pst *pst, SpId spId, RruUmtsUrnti ueId, U32 transId));
EXTERN S16 RrUiRruCountCReq ARGS((Pst *pst, SpId spId, 
                                 RruUmtsUrnti ueId));
EXTERN S16 RrUiRruMbmsDatReq ARGS((Pst *pst, SpId spId, RruMbmsDatReqSdus *rruMbmsDatReqSdus));
EXTERN S16 RrUiRruMbmsTxReq ARGS((Pst *pst, SpId spId, RruMbmsTxReq *rruMbmsTxReq));
EXTERN S16 RrUiRruRelocPrepInfoReq ARGS((Pst *pst, SpId spId, RruSrnsRelocInfoReq *ueId));
EXTERN S16 RrUiRruRelocCommitInfoReq ARGS((Pst *pst, SpId spId, RruSrnsRelocInfoReq *ueId));

EXTERN S16 RrUiRruRelocPrepInfoCfm ARGS((Pst *pst, SpId spId, RruRelocPrepInfoCfm *rIndInfo));
EXTERN S16 RrUiRruRelocCommitInfoCfm ARGS((Pst *pst, SpId spId, RruRelocCommitInfoCfm *rIndInfo));
/*rru_x_001.main_15 - ccpu00130069 - Added function prototypes for new functions */
EXTERN S16 RrPAsnEncDat ARGS((Pst *pst, SpId spId, RruEncDatReqSdus *rruEncDatReqSdus, RruEncDatCfmSdus *rruEncDatCfmSdus));
EXTERN S16 RrPAsnDecDat ARGS((Pst *pst, SpId spId, RruDecDatReqSdus *rruDecDatReqSdus, RruDecDatCfmSdus *rruDecDatCfmSdus));

EXTERN S16 PAsnEncDat ARGS((Pst *pst, SpId spId, RruEncDatReqSdus *rruEncDatReqSdus, RruEncDatCfmSdus *rruEncDatCfmSdus));
EXTERN S16 PAsnDecDat ARGS((Pst *pst, SpId spId, RruDecDatReqSdus *rruDecDatReqSdus, RruDecDatCfmSdus *rruDecDatCfmSdus));
#endif /* RR */


#endif /* __RRUX__ */

/**********************************************************************
         End of file:     rru.x@@/main/16 - Wed May 22 15:03:14 2013
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/2      ---     vk       1. RRC Initial Release.
/main/3      ---     vk       1. RRC Release 2.1
/main/5      ---     ds       1. RRC Release 2.2
/main/6      ---     dm       1. rru_x_001.main_5. multiple lower sap 
                                 support and drift RNC feature.
/main/7      ---     ds       1. rru_x_001.main_6. Added drxCyclCoef
                                 member in rruSrncCfgForDUe. 
/main/8      ---     ds       1. rru_x_001.main_7 Adding RruUE_CapbConta_IEs
                                 in the RruEncDatReq Sdus.
/main/9      ---     gm       1. rru_x_001.main_8 Added Count-I and MAC 
                                Count-C support.
/main/10     ---   chebli      1. rru_x_001.main_9 Added Enumeration to 
                                 determine if new integrity keys are received.
/main/11     ---      rru_x_001.main_10    vg       1.  upgraded to support SIB20 
/main/12     ---      rru_x_001.main_11    pkn      1. Update the integrity info at TRNC after SRNC relocation 
/main/13     ---      rru_x_001.main_12    slakshmi 1. Added new element firstCfn in 
                                                    RruEDchInfo and RruHsDschInfo
/main/14     ---   rru_x_001.main_13   vkulkarni   1. Added cellId to UE config, indicating the cell to which UE belongs.
/main/13     ---      rru_x_001.main_12    slakshmi 1. Added new element firstCfn in
                                                    RruEDchInfo and RruHsDschInfo
/main/14   rru_x_001.main_13   vkulkarni 1. Added cellId to UE config,
                                       indicating the cell to which UE belongs.
/main/15     ---   rru_x_001.main_14  ss     1. To added configuration for Packet manager.
/main/16     ---      rru_x_001.main_15 rpagoti 1. ccpu00129401-Interface naming convention changes
                                        related to new FAP &Cl.
                                     2.ccpu00130069 - Added function prototypes for
                                       new functions added for rrcencode & decode dat req
*********************************************************************91*/

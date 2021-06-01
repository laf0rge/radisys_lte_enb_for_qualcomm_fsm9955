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

     Name:     LTE SON Application

     Type:     C include file

     Desc:     This file contains 
               

     File:     nl.h

     Sid:      nl.h@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/3 - Mon Jan  5 16:15:44 2015

     Prg:      subhamay 

*********************************************************************21*/

#ifndef NL_H_
#define NL_H_


/* header include files (.h) */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */
#include <malloc.h>
#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer defines */
#include "cm_tkns.h"       /* Common tokens defines */
#include "cm_mblk.h"       /* Common memory allocation library defines */
#include "cm_llist.h"      /* Common link list  defines  */
#include "cm_hash.h"       /* Common hashlist  defines */
#include "cm_lte.h"        /* Common LTE defines */
#include "cm_inet.h"
#include "cm_tpt.h"

/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */

#include "cm5.x"           /* Common timer library */
#include "cm_tkns.x"       /* Common tokens */
#include "cm_mblk.x"       /* Common memory allocation */
#include "cm_llist.x"      /* Common link list */
#include "cm_hash.x"       /* Common hashlist */
#include "cm_lte.x"       /*  Common UMTS includes */
#include "cm_lib.x"
#include "cm_inet.x"
#include "cm_tpt.x"


/* header include files (.h) */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer defines */
#include "cm_tkns.h"       /* Common tokens defines */
#include "cm_mblk.h"       /* Common memory allocation library defines */
#include "cm_llist.h"      /* Common link list  defines  */
#include "cm_hash.h"       /* Common hashlist  defines */
#include "cm_lte.h"        /* Common LTE defines */
#include "cm_inet.h"
#include "cm_tpt.h"

/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */

#include "cm5.x"           /* Common timer library */
#include "cm_tkns.x"       /* Common tokens */
#include "cm_mblk.x"       /* Common memory allocation */
#include "cm_llist.x"      /* Common link list */
#include "cm_hash.x"       /* Common hashlist */
#include "cm_lte.x"       /*  Common UMTS includes */
#include "cm_lib.x"
#include "cm_inet.x"
#include "cm_tpt.x"

#define NL_POOL                 nlCb.init.pool 
#define NL_REGION               nlCb.init.region 
#define NL_EUTRAN_HASH_NO_BINS  32
#define NL_MIN_PCI_VAL          0
#define NL_MAX_PCI_VAL          503
#define NL_MAX_PCI_KEY_LEN      10
#define NL_MAX_NGH_ALARM        5
#define NL_NUM_PCFICH_REG         4
#define NL_NUM_REG_PER_CCE        9
#define NL_NUM_REG_PER_PHICH_GRP  3

#define NL_NUM_ANT_PORT_FOUR                4
#define NL_NUM_REGS_1ST_SYM                 2
#define NL_NUM_REGS_2ND_SYM_FOUR_ANT_PORT   2
#define NL_NUM_REGS_2ND_SYM_1OR2_ANT_PORT   3
#define NL_NUM_REGS_3RD_SYM                 3
#define NL_NUM_REGS_4TH_SYM_NOR_CP          3
#define NL_NUM_REGS_4TH_SYM_EXT_CP          2

#define NL_MODE_FDD              1
#define NL_MODE_TDD              2

#define NL_NUM_SUBCAR                12
#define NL_TDD_MAX_P_PLUS_ONE_VAL    5
#define NL_MAX_PCI_MOD_VAL           4

#define EVTNLVENDORDSONDATIND               100
#define NL_RSRP_OFFSET                      144 /*!<Required to in sync with broadcom son interface*/
#define NL_RSRQ_OFFSET                      39  /*!<Required to in sync with broadcom son interface*/
#define NL_DFLT_CRNTI_VAL                   65535
#define NL_NUM_TQENTRY                      20
#define NL_INVALID_RSRP                    -127
/**  
 * @details Macro for freeing the memory allocated for mbuf 
 */
#define NL_FREE_MBUF(_mBuf)                              \
{                                                        \
   if(NULLP != _mBuf)                                    \
   {                                                     \
      (Void)SPutMsg((_mBuf));                            \
      (_mBuf) = NULLP;                                   \
   }                                                     \
}
                                                    
/**
 * @details Macros for Static memory allocation
 */
#define NL_ALLOC(_region, _pool,_buf, _size)                      \
{                                                                 \
   if (ROK == SGetSBuf(_region, _pool, (Data **)(_buf), (_size)) )\
   {                                                              \
      cmMemset((U8 *)(*(_buf)), 0, (_size));                      \
   }                                                              \
   else                                                           \
   {                                                              \
      (*(_buf)) = NULLP;                                          \
   }                                                              \
}

/**
 * @details Macro for freeing static memeory
 */
#define NL_FREE(_region, _pool, _buf, _size)                      \
{                                                                 \
    (Void)SPutSBuf((_region), (_pool),                            \
     (Data *)(_buf), (Size)(_size));                              \
     (_buf) = NULLP;                                              \
}

#define NL_DSON_ALLOC(_buf, _size)                                \
{                                                                 \
   _buf = NULLP;                                                  \
   _buf = malloc(_size);                                          \
   if(NULLP != _buf)                                              \
   {                                                              \
      memset((U8 *)_buf, 0, _size);                               \
   }                                                              \
}

#define NL_DSON_FREE(_buf)                                         \
{                                                                  \
   if(NULLP != _buf)                                               \
   {                                                               \
      free(_buf);                                                  \
      _buf = NULLP;                                                \
   }                                                               \
}
 


/**
 * @details  Macros for generic content copy from source to 
 * destination memory location 
 */
#define NL_MEM_COPY(_dst, _src, _len)                             \
{                                                                 \
   cmMemcpy((U8*) (_dst), (U8*) (_src), (_len));                  \
}

#define NL_NGH_CELL_COLLISION_DETECT(_cellCb, _ecgi, _pci, _dlEarfcn,_pciColl)     \
{                                                                                  \
   _pciColl = FALSE;                                                               \
   if((_cellCb->cellInfo.pci == _pci) && (_cellCb->cellInfo.dlEarfcn == _dlEarfcn))\
   {                                                                               \
      if(_ecgi->cellId > 0)                                                        \
      {                                                                            \
         if(FALSE != cmMemcmp((U8 *)&(_cellCb->cellInfo.ecgi), (U8 *) _ecgi, sizeof(LnlEcgi)))\
         {                                                                         \
            _pciColl = TRUE;                                                       \
            _cellCb->state = NL_CELL_STATE_PCI_CONFLICT;                           \
         }                                                                         \
      }                                                                            \
      else                                                                         \
      {                                                                            \
         _pciColl = TRUE;                                                          \
         _cellCb->state = NL_CELL_STATE_PCI_CONFLICT;                              \
      }                                                                            \
   }                                                                               \
}

#define NL_FILL_PCI_COLLISON_ALARM(_cellId, _cellCb, _alarm)                        \
{                                                                                   \
   _alarm->isAlarmRaised = TRUE;                                                    \
   _alarm->isAlarmSend   = TRUE;                                                    \
   _alarm->alarmRaised[_alarm->numAlarmRaised].dgn.type = LNL_PCI_CONFLICT;             \
   _alarm->alarmRaised[_alarm->numAlarmRaised].dgn.alarmId = LNL_ALARM_PCI_CONFLICT;    \
   _alarm->alarmRaised[_alarm->numAlarmRaised].dgn.severity = LNL_ALARM_SEVERITY_CRITICAL;\
   _alarm->alarmRaised[_alarm->numAlarmRaised].dgn.causeType = 0;                       \
   _alarm->alarmRaised[_alarm->numAlarmRaised].dgn.causeVal  = 0;                       \
   _alarm->alarmRaised[_alarm->numAlarmRaised].dgn.u.addInfo.pci = _cellCb->cellInfo.pci;\
   _alarm->alarmRaised[_alarm->numAlarmRaised].dgn.u.addInfo.cellId = _cellId;          \
   _alarm->alarmRaised[_alarm->numAlarmRaised].dgn.u.addInfo.action = LNL_ALARM_MANUAL_INTER_REQ;\
   _alarm->alarmRaised[_alarm->numAlarmRaised].peerType = NL_ALARM_PEER_SERV_CELL;   \
   _alarm->alarmRaised[_alarm->numAlarmRaised].peerAddr = _cellCb;                   \
   _alarm->numAlarmRaised ++;                                                       \
}
#define NL_FILL_PCI_UPDATE_ALARM(_cellCb, _alarm)                                   \
{                                                                                   \
   U8   count;                                                                      \
   _alarm->isAlarmRaised = FALSE;                                                   \
   if(TRUE == _cellCb->isSmSonCfgDone)                                              \
   {                                                                                \
      _alarm->isPciUpdated = TRUE;                                                  \
      for(count = 0; count < _alarm->numAlarmRaised; count ++)                      \
      {                                                                             \
         if(_cellCb->cellInfo.pci == _alarm->alarmRaised[count].dgn.u.addInfo.pci)      \
         {                                                                          \
            _alarm->alarmRaised[count].dgn.u.addInfo.action = LNL_ALARM_AUTO_RESOLVE;   \
            _cellCb->pciModAlarmInfo.oldPci = _cellCb->cellInfo.pci;                \
            _cellCb->pciModAlarmInfo.alarmType = _alarm->alarmRaised[count].dgn.type;   \
            _cellCb->pciModAlarmInfo.alarmId = _alarm->alarmRaised[count].dgn.alarmId;  \
         }                                                                          \
      }                                                                             \
   }                                                                                \
   else                                                                             \
   {                                                                                \
       _alarm->isAlarmSend     = FALSE;                                              \
   }                                                                                \
}

#define NL_FILL_CLEAR_ALARM(_nghCellInfo, _alarm)                                 \
{                                                                                  \
   U8   count;                                                                     \
   if(_nghCellInfo.alarmInfo.isAlarmRaised)                                        \
   {                                                                               \
      _alarm->isAlarmCleared = TRUE;                                               \
      for(count = 0; count < NL_MAX_NGH_ALARM; count ++)                           \
      {                                                                            \
         if(TRUE == _nghCellInfo.alarmInfo.peerInfo[count].isActive)                  \
         {                                                                             \
            _alarm->alarmCleared[_alarm->numAlarmCleared].u.addInfo.pci =                \
            _nghCellInfo.pci;                                                            \
            _alarm->alarmCleared[_alarm->numAlarmCleared].u.addInfo.cellId =             \
            _nghCellInfo.ecgi.cellId;                                                    \
            if(NL_ALARM_PCI_CONFUSION == _nghCellInfo.alarmInfo.peerInfo[count].alarmId)\
            {                                                                            \
               _alarm->alarmCleared[_alarm->numAlarmCleared].alarmId = LNL_ALARM_PCI_CONFUSION; \
               _alarm->alarmCleared[_alarm->numAlarmCleared].type = LNL_PCI_CONFUSION;   \
            }                                                                         \
            if(NL_ALARM_PCI_CONFLICT == _nghCellInfo.alarmInfo.peerInfo[count].alarmId)\
            {                                                                            \
               _alarm->alarmCleared[_alarm->numAlarmCleared].alarmId = LNL_ALARM_PCI_CONFLICT;\
               _alarm->alarmCleared[_alarm->numAlarmCleared].type = LNL_PCI_CONFLICT;    \
            }                                                                            \
            _alarm->alarmCleared[_alarm->numAlarmCleared].severity = LNL_ALARM_SEVERITY_CLEARED;  \
            _alarm->alarmCleared[_alarm->numAlarmCleared].causeType = 0;                 \
            _alarm->alarmCleared[_alarm->numAlarmCleared].causeVal = 0;                  \
            _alarm->numAlarmCleared ++;                                                  \
         }                                                                               \
      }                                                                               \
   }                                                                               \
}

#define NL_FILL_NGH_CELL_PCI_CONFUSION_ALARM(_pci, _cellId, _nghServCell, _alarm)   \
{                                                                                   \
   _alarm->isAlarmRaised = TRUE;                                                    \
   _alarm->isAlarmSend   = TRUE;                                                    \
   _alarm->alarmRaised[_alarm->numAlarmRaised].dgn.type = LNL_PCI_CONFUSION;            \
   _alarm->alarmRaised[_alarm->numAlarmRaised].dgn.alarmId = LNL_ALARM_PCI_CONFUSION;   \
   _alarm->alarmRaised[_alarm->numAlarmRaised].dgn.severity = LNL_ALARM_SEVERITY_CRITICAL;\
   _alarm->alarmRaised[_alarm->numAlarmRaised].dgn.causeType = 0;                       \
   _alarm->alarmRaised[_alarm->numAlarmRaised].dgn.causeVal = 0;                        \
   _alarm->alarmRaised[_alarm->numAlarmRaised].dgn.u.addInfo.pci = _pci;                \
   _alarm->alarmRaised[_alarm->numAlarmRaised].dgn.u.addInfo.cellId = _cellId;          \
   _alarm->alarmRaised[_alarm->numAlarmRaised].dgn.u.addInfo.action = LNL_ALARM_MANUAL_INTER_REQ;\
   _alarm->alarmRaised[_alarm->numAlarmRaised].peerType = NL_ALARM_PEER_NGH_SERV_CELL;\
   _alarm->alarmRaised[_alarm->numAlarmRaised].peerAddr = _nghServCell;               \
   _alarm->numAlarmRaised ++;                                                      \
}

#define NL_FILL_NGH_NGH_CELL_PCI_CONFUSION_ALARM(_pci, _cellId, _nghNghCell, _alarm)   \
{                                                                                   \
   _alarm->isAlarmRaised = TRUE;                                                    \
   _alarm->isAlarmSend   = TRUE;                                                    \
   _alarm->alarmRaised[_alarm->numAlarmRaised].dgn.type = LNL_PCI_CONFUSION;            \
   _alarm->alarmRaised[_alarm->numAlarmRaised].dgn.alarmId = LNL_ALARM_PCI_CONFUSION;   \
   _alarm->alarmRaised[_alarm->numAlarmRaised].dgn.severity = LNL_ALARM_SEVERITY_CRITICAL; \
   _alarm->alarmRaised[_alarm->numAlarmRaised].dgn.causeType = 0;                       \
   _alarm->alarmRaised[_alarm->numAlarmRaised].dgn.causeVal = 0;                        \
   _alarm->alarmRaised[_alarm->numAlarmRaised].dgn.u.addInfo.pci = _pci;                \
   _alarm->alarmRaised[_alarm->numAlarmRaised].dgn.u.addInfo.cellId = _cellId;          \
   _alarm->alarmRaised[_alarm->numAlarmRaised].dgn.u.addInfo.action = LNL_ALARM_MANUAL_INTER_REQ;\
   _alarm->alarmRaised[_alarm->numAlarmRaised].peerType = NL_ALARM_PEER_NGH_NGH_CELL; \
   _alarm->alarmRaised[_alarm->numAlarmRaised].peerAddr = _nghNghCell;               \
   _alarm->numAlarmRaised ++;                                                        \
}

#define NL_UPDATE_CELL_STATE(_cellCb, _state)   \
{                                               \
   _cellCb->state = _state;                     \
}

#define NL_UPDATE_VENDOR_DSON_STATE(_cellCb, _state)   \
{                                                      \
   _cellCb->dSonState = _state;                        \
}

#define NL_COPY_NGH_CELL_INFO(_tgtCell, _srcCell)                   \
{                                                                   \
   _tgtCell.pci = _srcCell.pci;                                     \
   _tgtCell.cellId = _srcCell.ecgi.cellId;                          \
   _tgtCell.dlEarfcn = _srcCell.dlEarfcnNum;                        \
   _tgtCell.ulEarfcn = _srcCell.ulEarfcnNum;                        \
   cmMemcpy((U8 *)&_tgtCell.ecgi, (U8 *)&_srcCell.ecgi,             \
      sizeof(LnlEcgi));                                             \
}


#define NL_SWAP_ROOTSEQIDX_IN_CELLCB(_rootSeqIdxLstInCellCb, _index)  \
{                                                                     \
   U8 rootSeqInCellCbAtIndexPos = _rootSeqIdxLstInCellCb[_index];     \
   _rootSeqIdxLstInCellCb[_index] = _rootSeqIdxLstInCellCb[0];        \
   _rootSeqIdxLstInCellCb[0] = rootSeqInCellCbAtIndexPos;             \
}

#define NL_CEIL(x, y) (((x) + (y)-1) / (y))

#define NL_CALC_WAIT_TIME(_delay) ((_delay * SS_TICKS_SEC) / nlCb.timeRes)

#endif /* NL_H_ */
/********************************************************************30**

           End of file:     nl.h@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/3 - Mon Jan  5 16:15:44 2015

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


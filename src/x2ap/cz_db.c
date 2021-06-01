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
               

     File:     wr_smm_init.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
#include "envopt.h"           /* environment options */
#include "envdep.h"           /* environment dependent */
#include "envind.h"           /* environment independent */
#include "gen.h"              /* general layer */
#include "ssi.h"              /* system services */
#include "cm_mblk.h"
#include "cm_tkns.h"
#include "cm_hash.h"
#include "cm_pasn.h"          /* common defines for ASN */
#include "cm5.h"
/* header/extern include files (.x) */
#include "cm_llist.h"         /* */
#include "cm_hash.h"
#include "gen.x"              /* general layer */
#include "ssi.x"              /* system services */
#include "cm5.x"              /* */
#include "cm_tkns.x"          /* */
#include "cm_mblk.x"
#include "cm_llist.x"
#include "cm_pasn.x"          /* PASN */
#include <czt_asn.h>
#include <czt_asn.x>


U32 flagCz3O = CM_PASN_ELM_OPT | CM_PASN_POST_DECODE_ESC_CALL;
U32 flagCz3M = CM_PASN_ELM_MAND | CM_PASN_POST_DECODE_ESC_CALL;
U32 flagCz3D = CM_PASN_ELM_DEF | CM_PASN_POST_DECODE_ESC_CALL;
CmPAsnElmDef czConstTerm =
{
#ifdef CM_PASN_DBG
   " czConstTerm (Set-Sequence Terminator) ",
#endif /* CM_PASN_DEBUG */
   CM_PASN_TET_SETSEQ_TERM,
   CM_PASN_EXT_MKR_ABS,
   0,
   &flagCz3M,    
   CM_PASN_ESC_FUNC_NULLP,
   NULLP 
}; 
PUBLIC CmPAsnElmDef czNull =
{
#ifdef CM_PASN_DBG
   " czNull ",
#endif /* CM_PASN_DEBUG */
  CM_PASN_TET_NULL,
  CM_PASN_EXT_MKR_ABS,
  0,        
  &flagCz3M,
  CM_PASN_ESC_FUNC_NULLP,
  NULLP
}; 
CmPAsnElmDef czExtMkr =
{  
#ifdef CM_PASN_DBG
  " czExtMkr(Extension Marker) ",
#endif /* CM_PASN_DEBUG */
  CM_PASN_TET_EXT_MKR,
  CM_PASN_EXT_MKR_ABS,
  0,
  &flagCz3M,
  CM_PASN_ESC_FUNC_NULLP,
  NULLP 
};
/* Element ----- Criticality ------- */
CmPAsnTknEnum czCriticalityLst[4] =
{
	3,
	CztCriticalityrejectEnum,
	CztCriticalityignoreEnum,
	CztCriticalitynotifyEnum
};
CmPAsnEnumDef czCriticalityspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czCriticalityLst,
	NULLP
};
CmPAsnElmDef czOptCriticality =
{
#ifdef CM_PASN_DBG
	" czCriticality ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztCriticality),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czCriticalityspec),
	NULLP
};
CmPAsnElmDef czCriticality =
{
#ifdef CM_PASN_DBG
	" czCriticality ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztCriticality),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czCriticalityspec),
	NULLP
};

/* Element ----- Presence ------- */
CmPAsnTknEnum czPresenceLst[4] =
{
	3,
	CztPresenceoptionalEnum,
	CztPresenceconditionalEnum,
	CztPresencemandatoryEnum
};
CmPAsnEnumDef czPresencespec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czPresenceLst,
	NULLP
};
CmPAsnElmDef czPresence =
{
#ifdef CM_PASN_DBG
	" czPresence ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztPresence),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czPresencespec),
	NULLP
};

/* Element ----- local ------- */
CmPAsnUIntDef czPrivIE_IDlocalspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	CztmaxPrivIEs
};
CmPAsnElmDef czPrivIE_IDlocal =
{
#ifdef CM_PASN_DBG
	" czPrivIE_IDlocal ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztPrivIE_IDlocal),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czPrivIE_IDlocalspec),
	NULLP
};

PUBLIC CmPAsnElmDef *czPrivIE_IDTkn[] =
{
	&czPrivIE_IDlocal,
	&czConstTerm
};
CmPAsnChoiceDef czPrivIE_IDspec = 
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CHOICE,
#endif
	2,
	0,
	&czPrivIE_IDTkn[0]  /* Token List */
};
CmPAsnElmDef czPrivIE_ID =
{
#ifdef CM_PASN_DBG
	" czPrivIE_ID ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CHOICE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztPrivIE_ID),
	&flagCz3M,
	NULLP,
	(U8 *)(&czPrivIE_IDspec),
	NULLP
};
/* Element ----- ProcedureCode ------- */
CmPAsnUIntDef czProcedureCodespec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	255UL
};
CmPAsnElmDef czProcedureCode =
{
#ifdef CM_PASN_DBG
	" czProcedureCode ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProcedureCode),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProcedureCodespec),
	NULLP
};
CmPAsnElmDef czOptProcedureCode =
{
#ifdef CM_PASN_DBG
	" czProcedureCode ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProcedureCode),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProcedureCodespec),
	NULLP
};

/* Element ----- ProtocolIE-ID ------- */
CmPAsnUIntDef czProtIE_IDspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	CztmaxProtIEs
};
CmPAsnElmDef czProtIE_ID =
{
#ifdef CM_PASN_DBG
	" czProtIE_ID ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_ID),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_IDspec),
	NULLP
};

/* Element ----- TriggeringMessage ------- */
CmPAsnTknEnum czTrgMsgLst[4] =
{
	3,
	CztTrgMsginitiating_messageEnum,
	CztTrgMsgsuccessful_outcomeEnum,
	CztTrgMsgunsuccessful_outcomeEnum
};
CmPAsnEnumDef czTrgMsgspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czTrgMsgLst,
	NULLP
};
CmPAsnElmDef czOptTrgMsg =
{
#ifdef CM_PASN_DBG
	" czTrgMsg ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztTrgMsg),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czTrgMsgspec),
	NULLP
};

CmPAsnBitStrDef czABSInformFDDabs_pattern_infospec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTRXL,
#endif /* CM_PASN_DEBUG */
	40,
	40
};
CmPAsnElmDef czABSInformFDDabs_pattern_info =
{
#ifdef CM_PASN_DBG
	" czABSInformFDDabs_pattern_info ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTRXL,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztABSInformFDDabs_pattern_info),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czABSInformFDDabs_pattern_infospec),
	NULLP
};
/* Element ----- numberOfCellSpecificAntennaPorts ------- */
CmPAsnTknEnum czABSInformFDDnumberOfCellSpecificAntennaPortsLst[4] =
{
	3,
	CztABSInformFDDnumberOfCellSpecificAntennaPortsoneEnum,
	CztABSInformFDDnumberOfCellSpecificAntennaPortstwoEnum,
	CztABSInformFDDnumberOfCellSpecificAntennaPortsfourEnum
};
CmPAsnEnumDef czABSInformFDDnumberOfCellSpecificAntennaPortsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czABSInformFDDnumberOfCellSpecificAntennaPortsLst,
	NULLP
};
CmPAsnElmDef czABSInformFDDnumberOfCellSpecificAntennaPorts =
{
#ifdef CM_PASN_DBG
	" czABSInformFDDnumberOfCellSpecificAntennaPorts ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztABSInformFDDnumberOfCellSpecificAntennaPorts),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czABSInformFDDnumberOfCellSpecificAntennaPortsspec),
	NULLP
};

CmPAsnBitStrDef czABSInformFDDmeasurement_subsetspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTRXL,
#endif /* CM_PASN_DEBUG */
	40,
	40
};
CmPAsnElmDef czABSInformFDDmeasurement_subset =
{
#ifdef CM_PASN_DBG
	" czABSInformFDDmeasurement_subset ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTRXL,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztABSInformFDDmeasurement_subset),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czABSInformFDDmeasurement_subsetspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnABSInformFDD_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnABSInformFDD_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnABSInformFDD_ExtIEsClsInfo =
{
	0,
	&cz_ExtnABSInformFDD_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnABSInformFDD_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnABSInformFDD_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnABSInformFDD_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnABSInformFDD_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnABSInformFDD_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnABSInformFDD_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnABSInformFDD_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_ABSInformFDD_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnABSInformFDD_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_ABSInformFDD_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_ABSInformFDD_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_ABSInformFDD_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_ABSInformFDD_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_ABSInformFDD_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_ABSInformFDD_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_ABSInformFDD_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_ABSInformFDD_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_ABSInformFDD_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_ABSInformFDD_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_ABSInformFDD_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_ABSInformFDD_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_ABSInformFDD_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_ABSInformFDD_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_ABSInformFDD_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czABSInformFDDTkn[] =
{
	&czABSInformFDDabs_pattern_info,
	&czABSInformFDDnumberOfCellSpecificAntennaPorts,
	&czABSInformFDDmeasurement_subset,
	&czOptProtExtnCont_ABSInformFDD_ExtIEs,
		&czProtExtnField_ABSInformFDD_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czABSInformFDDspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czABSInformFDDTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czABSInformFDD =
{
#ifdef CM_PASN_DBG
	" czABSInformFDD ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztABSInformFDD),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czABSInformFDDspec),
	NULLP
};
CmPAsnBitStrDef czABSInformTDDabs_pattern_infospec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTRXL,
#endif /* CM_PASN_DEBUG */
	1,
	70
};
CmPAsnElmDef czABSInformTDDabs_pattern_info =
{
#ifdef CM_PASN_DBG
	" czABSInformTDDabs_pattern_info ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTRXL,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztABSInformTDDabs_pattern_info),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czABSInformTDDabs_pattern_infospec),
	NULLP
};
/* Element ----- numberOfCellSpecificAntennaPorts ------- */
CmPAsnTknEnum czABSInformTDDnumberOfCellSpecificAntennaPortsLst[4] =
{
	3,
	CztABSInformTDDnumberOfCellSpecificAntennaPortsoneEnum,
	CztABSInformTDDnumberOfCellSpecificAntennaPortstwoEnum,
	CztABSInformTDDnumberOfCellSpecificAntennaPortsfourEnum
};
CmPAsnEnumDef czABSInformTDDnumberOfCellSpecificAntennaPortsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czABSInformTDDnumberOfCellSpecificAntennaPortsLst,
	NULLP
};
CmPAsnElmDef czABSInformTDDnumberOfCellSpecificAntennaPorts =
{
#ifdef CM_PASN_DBG
	" czABSInformTDDnumberOfCellSpecificAntennaPorts ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztABSInformTDDnumberOfCellSpecificAntennaPorts),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czABSInformTDDnumberOfCellSpecificAntennaPortsspec),
	NULLP
};

CmPAsnBitStrDef czABSInformTDDmeasurement_subsetspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTRXL,
#endif /* CM_PASN_DEBUG */
	1,
	70
};
CmPAsnElmDef czABSInformTDDmeasurement_subset =
{
#ifdef CM_PASN_DBG
	" czABSInformTDDmeasurement_subset ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTRXL,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztABSInformTDDmeasurement_subset),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czABSInformTDDmeasurement_subsetspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnABSInformTDD_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnABSInformTDD_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnABSInformTDD_ExtIEsClsInfo =
{
	0,
	&cz_ExtnABSInformTDD_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnABSInformTDD_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnABSInformTDD_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnABSInformTDD_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnABSInformTDD_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnABSInformTDD_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnABSInformTDD_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnABSInformTDD_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_ABSInformTDD_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnABSInformTDD_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_ABSInformTDD_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_ABSInformTDD_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_ABSInformTDD_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_ABSInformTDD_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_ABSInformTDD_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_ABSInformTDD_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_ABSInformTDD_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_ABSInformTDD_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_ABSInformTDD_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_ABSInformTDD_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_ABSInformTDD_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_ABSInformTDD_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_ABSInformTDD_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_ABSInformTDD_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_ABSInformTDD_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czABSInformTDDTkn[] =
{
	&czABSInformTDDabs_pattern_info,
	&czABSInformTDDnumberOfCellSpecificAntennaPorts,
	&czABSInformTDDmeasurement_subset,
	&czOptProtExtnCont_ABSInformTDD_ExtIEs,
		&czProtExtnField_ABSInformTDD_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czABSInformTDDspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czABSInformTDDTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czABSInformTDD =
{
#ifdef CM_PASN_DBG
	" czABSInformTDD ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztABSInformTDD),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czABSInformTDDspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czABSInformTkn[] =
{
	&czABSInformFDD,
	&czABSInformTDD,
	&czNull,
	&czExtMkr,
	&czConstTerm
};
CmPAsnChoiceDef czABSInformspec = 
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CHOICE,
#endif
	3,
	0,
	&czABSInformTkn[0]  /* Token List */
};
CmPAsnElmDef czABSInform =
{
#ifdef CM_PASN_DBG
	" czABSInform ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CHOICE,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztABSInform),
	&flagCz3M,
	NULLP,
	(U8 *)(&czABSInformspec),
	NULLP
};
/* Element ----- DL-ABS-status ------- */
CmPAsnUIntDef czDL_ABS_statusspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	100UL
};
CmPAsnElmDef czDL_ABS_status =
{
#ifdef CM_PASN_DBG
	" czDL_ABS_status ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztDL_ABS_status),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czDL_ABS_statusspec),
	NULLP
};

CmPAsnBitStrDef czUsableABSInformFDDusable_abs_pattern_infospec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTRXL,
#endif /* CM_PASN_DEBUG */
	40,
	40
};
CmPAsnElmDef czUsableABSInformFDDusable_abs_pattern_info =
{
#ifdef CM_PASN_DBG
	" czUsableABSInformFDDusable_abs_pattern_info ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTRXL,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztUsableABSInformFDDusable_abs_pattern_info),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czUsableABSInformFDDusable_abs_pattern_infospec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnUsableABSInformFDD_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnUsableABSInformFDD_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnUsableABSInformFDD_ExtIEsClsInfo =
{
	0,
	&cz_ExtnUsableABSInformFDD_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnUsableABSInformFDD_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnUsableABSInformFDD_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnUsableABSInformFDD_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnUsableABSInformFDD_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnUsableABSInformFDD_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnUsableABSInformFDD_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnUsableABSInformFDD_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_UsableABSInformFDD_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnUsableABSInformFDD_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_UsableABSInformFDD_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_UsableABSInformFDD_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_UsableABSInformFDD_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_UsableABSInformFDD_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_UsableABSInformFDD_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_UsableABSInformFDD_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_UsableABSInformFDD_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_UsableABSInformFDD_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_UsableABSInformFDD_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_UsableABSInformFDD_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_UsableABSInformFDD_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_UsableABSInformFDD_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_UsableABSInformFDD_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_UsableABSInformFDD_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_UsableABSInformFDD_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czUsableABSInformFDDTkn[] =
{
	&czUsableABSInformFDDusable_abs_pattern_info,
	&czOptProtExtnCont_UsableABSInformFDD_ExtIEs,
		&czProtExtnField_UsableABSInformFDD_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czUsableABSInformFDDspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czUsableABSInformFDDTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czUsableABSInformFDD =
{
#ifdef CM_PASN_DBG
	" czUsableABSInformFDD ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztUsableABSInformFDD),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czUsableABSInformFDDspec),
	NULLP
};
CmPAsnBitStrDef czUsableABSInformTDDusaable_abs_pattern_infospec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTRXL,
#endif /* CM_PASN_DEBUG */
	1,
	70
};
CmPAsnElmDef czUsableABSInformTDDusaable_abs_pattern_info =
{
#ifdef CM_PASN_DBG
	" czUsableABSInformTDDusaable_abs_pattern_info ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTRXL,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztUsableABSInformTDDusaable_abs_pattern_info),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czUsableABSInformTDDusaable_abs_pattern_infospec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnUsableABSInformTDD_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnUsableABSInformTDD_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnUsableABSInformTDD_ExtIEsClsInfo =
{
	0,
	&cz_ExtnUsableABSInformTDD_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnUsableABSInformTDD_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnUsableABSInformTDD_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnUsableABSInformTDD_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnUsableABSInformTDD_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnUsableABSInformTDD_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnUsableABSInformTDD_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnUsableABSInformTDD_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_UsableABSInformTDD_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnUsableABSInformTDD_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_UsableABSInformTDD_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_UsableABSInformTDD_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_UsableABSInformTDD_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_UsableABSInformTDD_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_UsableABSInformTDD_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_UsableABSInformTDD_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_UsableABSInformTDD_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_UsableABSInformTDD_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_UsableABSInformTDD_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_UsableABSInformTDD_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_UsableABSInformTDD_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_UsableABSInformTDD_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_UsableABSInformTDD_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_UsableABSInformTDD_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_UsableABSInformTDD_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czUsableABSInformTDDTkn[] =
{
	&czUsableABSInformTDDusaable_abs_pattern_info,
	&czOptProtExtnCont_UsableABSInformTDD_ExtIEs,
		&czProtExtnField_UsableABSInformTDD_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czUsableABSInformTDDspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czUsableABSInformTDDTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czUsableABSInformTDD =
{
#ifdef CM_PASN_DBG
	" czUsableABSInformTDD ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztUsableABSInformTDD),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czUsableABSInformTDDspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czUsableABSInformTkn[] =
{
	&czUsableABSInformFDD,
	&czUsableABSInformTDD,
	&czExtMkr,
	&czConstTerm
};
CmPAsnChoiceDef czUsableABSInformspec = 
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CHOICE,
#endif
	2,
	0,
	&czUsableABSInformTkn[0]  /* Token List */
};
CmPAsnElmDef czUsableABSInform =
{
#ifdef CM_PASN_DBG
	" czUsableABSInform ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CHOICE,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztUsableABSInform),
	&flagCz3M,
	NULLP,
	(U8 *)(&czUsableABSInformspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnABS_Status_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnABS_Status_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnABS_Status_ExtIEsClsInfo =
{
	0,
	&cz_ExtnABS_Status_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnABS_Status_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnABS_Status_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnABS_Status_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnABS_Status_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnABS_Status_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnABS_Status_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnABS_Status_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_ABS_Status_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnABS_Status_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_ABS_Status_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_ABS_Status_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_ABS_Status_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_ABS_Status_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_ABS_Status_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_ABS_Status_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_ABS_Status_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_ABS_Status_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_ABS_Status_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_ABS_Status_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_ABS_Status_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_ABS_Status_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_ABS_Status_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_ABS_Status_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_ABS_Status_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czABS_StatusTkn[] =
{
	&czDL_ABS_status,
	&czUsableABSInform,
	&czOptProtExtnCont_ABS_Status_ExtIEs,
		&czProtExtnField_ABS_Status_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czABS_Statusspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czABS_StatusTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czABS_Status =
{
#ifdef CM_PASN_DBG
	" czABS_Status ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztABS_Status),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czABS_Statusspec),
	NULLP
};
/* Element ----- AdditionalSpecialSubframePatterns ------- */
CmPAsnTknEnum czAdditionalSpecialSubframePatternsLst[11] =
{
	10,
	CztAdditionalSpecialSubframePatternsssp0Enum,
	CztAdditionalSpecialSubframePatternsssp1Enum,
	CztAdditionalSpecialSubframePatternsssp2Enum,
	CztAdditionalSpecialSubframePatternsssp3Enum,
	CztAdditionalSpecialSubframePatternsssp4Enum,
	CztAdditionalSpecialSubframePatternsssp5Enum,
	CztAdditionalSpecialSubframePatternsssp6Enum,
	CztAdditionalSpecialSubframePatternsssp7Enum,
	CztAdditionalSpecialSubframePatternsssp8Enum,
	CztAdditionalSpecialSubframePatternsssp9Enum
};
CmPAsnEnumDef czAdditionalSpecialSubframePatternsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czAdditionalSpecialSubframePatternsLst,
	NULLP
};
CmPAsnElmDef czAdditionalSpecialSubframePatterns =
{
#ifdef CM_PASN_DBG
	" czAdditionalSpecialSubframePatterns ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztAdditionalSpecialSubframePatterns),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czAdditionalSpecialSubframePatternsspec),
	NULLP
};

/* Element ----- CyclicPrefixDL ------- */
CmPAsnTknEnum czCyclicPrefixDLLst[3] =
{
	2,
	CztCyclicPrefixDLnormalEnum,
	CztCyclicPrefixDLextendedEnum
};
CmPAsnEnumDef czCyclicPrefixDLspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czCyclicPrefixDLLst,
	NULLP
};
CmPAsnElmDef czCyclicPrefixDL =
{
#ifdef CM_PASN_DBG
	" czCyclicPrefixDL ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztCyclicPrefixDL),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czCyclicPrefixDLspec),
	NULLP
};

/* Element ----- CyclicPrefixUL ------- */
CmPAsnTknEnum czCyclicPrefixULLst[3] =
{
	2,
	CztCyclicPrefixULnormalEnum,
	CztCyclicPrefixULextendedEnum
};
CmPAsnEnumDef czCyclicPrefixULspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czCyclicPrefixULLst,
	NULLP
};
CmPAsnElmDef czCyclicPrefixUL =
{
#ifdef CM_PASN_DBG
	" czCyclicPrefixUL ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztCyclicPrefixUL),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czCyclicPrefixULspec),
	NULLP
};

PUBLIC CmPAsnElmDef *cz_ExtnAdditionalSpecialSubframe_Info_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnAdditionalSpecialSubframe_Info_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnAdditionalSpecialSubframe_Info_ExtIEsClsInfo =
{
	0,
	&cz_ExtnAdditionalSpecialSubframe_Info_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnAdditionalSpecialSubframe_Info_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnAdditionalSpecialSubframe_Info_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnAdditionalSpecialSubframe_Info_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnAdditionalSpecialSubframe_Info_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnAdditionalSpecialSubframe_Info_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnAdditionalSpecialSubframe_Info_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnAdditionalSpecialSubframe_Info_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_AdditionalSpecialSubframe_Info_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnAdditionalSpecialSubframe_Info_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_AdditionalSpecialSubframe_Info_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_AdditionalSpecialSubframe_Info_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_AdditionalSpecialSubframe_Info_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_AdditionalSpecialSubframe_Info_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_AdditionalSpecialSubframe_Info_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_AdditionalSpecialSubframe_Info_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_AdditionalSpecialSubframe_Info_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_AdditionalSpecialSubframe_Info_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_AdditionalSpecialSubframe_Info_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_AdditionalSpecialSubframe_Info_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_AdditionalSpecialSubframe_Info_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_AdditionalSpecialSubframe_Info_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_AdditionalSpecialSubframe_Info_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_AdditionalSpecialSubframe_Info_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_AdditionalSpecialSubframe_Info_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czAdditionalSpecialSubframe_InfoTkn[] =
{
	&czAdditionalSpecialSubframePatterns,
	&czCyclicPrefixDL,
	&czCyclicPrefixUL,
	&czOptProtExtnCont_AdditionalSpecialSubframe_Info_ExtIEs,
		&czProtExtnField_AdditionalSpecialSubframe_Info_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czAdditionalSpecialSubframe_Infospec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czAdditionalSpecialSubframe_InfoTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czAdditionalSpecialSubframe_Info =
{
#ifdef CM_PASN_DBG
	" czAdditionalSpecialSubframe_Info ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztAdditionalSpecialSubframe_Info),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czAdditionalSpecialSubframe_Infospec),
	NULLP
};
CmPAsnBitStrDef czKey_eNodeB_Starspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTRXL,
#endif /* CM_PASN_DEBUG */
	256,
	256
};
CmPAsnElmDef czKey_eNodeB_Star =
{
#ifdef CM_PASN_DBG
	" czKey_eNodeB_Star ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTRXL,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztKey_eNodeB_Star),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czKey_eNodeB_Starspec),
	NULLP
};
/* Element ----- NextHopChainingCount ------- */
CmPAsnUIntDef czNextHopChainingCountspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	7UL
};
CmPAsnElmDef czNextHopChainingCount =
{
#ifdef CM_PASN_DBG
	" czNextHopChainingCount ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztNextHopChainingCount),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czNextHopChainingCountspec),
	NULLP
};

PUBLIC CmPAsnElmDef *cz_ExtnAS_SecurInform_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnAS_SecurInform_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnAS_SecurInform_ExtIEsClsInfo =
{
	0,
	&cz_ExtnAS_SecurInform_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnAS_SecurInform_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnAS_SecurInform_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnAS_SecurInform_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnAS_SecurInform_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnAS_SecurInform_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnAS_SecurInform_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnAS_SecurInform_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_AS_SecurInform_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnAS_SecurInform_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_AS_SecurInform_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_AS_SecurInform_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_AS_SecurInform_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_AS_SecurInform_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_AS_SecurInform_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_AS_SecurInform_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_AS_SecurInform_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_AS_SecurInform_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_AS_SecurInform_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_AS_SecurInform_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_AS_SecurInform_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_AS_SecurInform_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_AS_SecurInform_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_AS_SecurInform_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_AS_SecurInform_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czAS_SecurInformTkn[] =
{
	&czKey_eNodeB_Star,
	&czNextHopChainingCount,
	&czOptProtExtnCont_AS_SecurInform_ExtIEs,
		&czProtExtnField_AS_SecurInform_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czAS_SecurInformspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czAS_SecurInformTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czAS_SecurInform =
{
#ifdef CM_PASN_DBG
	" czAS_SecurInform ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztAS_SecurInform),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czAS_SecurInformspec),
	NULLP
};
/* Element ----- PriorityLevel ------- */
CmPAsnUIntDef czPriorityLvlspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	15UL
};
CmPAsnElmDef czPriorityLvl =
{
#ifdef CM_PASN_DBG
	" czPriorityLvl ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztPriorityLvl),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czPriorityLvlspec),
	NULLP
};

/* Element ----- Pre-emptionCapability ------- */
CmPAsnTknEnum czPre_emptionCapbltyLst[3] =
{
	2,
	CztPre_emptionCapbltyshall_not_trigger_pre_emptionEnum,
	CztPre_emptionCapbltymay_trigger_pre_emptionEnum
};
CmPAsnEnumDef czPre_emptionCapbltyspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czPre_emptionCapbltyLst,
	NULLP
};
CmPAsnElmDef czPre_emptionCapblty =
{
#ifdef CM_PASN_DBG
	" czPre_emptionCapblty ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztPre_emptionCapblty),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czPre_emptionCapbltyspec),
	NULLP
};

/* Element ----- Pre-emptionVulnerability ------- */
CmPAsnTknEnum czPre_emptionVulnerabilityLst[3] =
{
	2,
	CztPre_emptionVulnerabilitynot_pre_emptableEnum,
	CztPre_emptionVulnerabilitypre_emptableEnum
};
CmPAsnEnumDef czPre_emptionVulnerabilityspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czPre_emptionVulnerabilityLst,
	NULLP
};
CmPAsnElmDef czPre_emptionVulnerability =
{
#ifdef CM_PASN_DBG
	" czPre_emptionVulnerability ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztPre_emptionVulnerability),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czPre_emptionVulnerabilityspec),
	NULLP
};

PUBLIC CmPAsnElmDef *cz_ExtnAllocnAndRetentionPriority_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnAllocnAndRetentionPriority_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnAllocnAndRetentionPriority_ExtIEsClsInfo =
{
	0,
	&cz_ExtnAllocnAndRetentionPriority_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnAllocnAndRetentionPriority_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnAllocnAndRetentionPriority_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnAllocnAndRetentionPriority_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnAllocnAndRetentionPriority_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnAllocnAndRetentionPriority_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnAllocnAndRetentionPriority_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnAllocnAndRetentionPriority_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_AllocnAndRetentionPriority_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnAllocnAndRetentionPriority_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_AllocnAndRetentionPriority_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_AllocnAndRetentionPriority_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_AllocnAndRetentionPriority_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_AllocnAndRetentionPriority_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_AllocnAndRetentionPriority_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_AllocnAndRetentionPriority_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_AllocnAndRetentionPriority_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_AllocnAndRetentionPriority_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_AllocnAndRetentionPriority_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_AllocnAndRetentionPriority_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_AllocnAndRetentionPriority_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_AllocnAndRetentionPriority_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_AllocnAndRetentionPriority_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_AllocnAndRetentionPriority_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_AllocnAndRetentionPriority_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czAllocnAndRetentionPriorityTkn[] =
{
	&czPriorityLvl,
	&czPre_emptionCapblty,
	&czPre_emptionVulnerability,
	&czOptProtExtnCont_AllocnAndRetentionPriority_ExtIEs,
		&czProtExtnField_AllocnAndRetentionPriority_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czAllocnAndRetentionPriorityspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czAllocnAndRetentionPriorityTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czAllocnAndRetentionPriority =
{
#ifdef CM_PASN_DBG
	" czAllocnAndRetentionPriority ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztAllocnAndRetentionPriority),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czAllocnAndRetentionPriorityspec),
	NULLP
};
CmPAsnOctStrDef czPLMN_Identityspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_OCTSTRXL,
#endif /* CM_PASN_DEBUG */
	3,
	3
};
CmPAsnElmDef czPLMN_Identity =
{
#ifdef CM_PASN_DBG
	" czPLMN_Identity " ,
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_OCTSTRXL,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztPLMN_Identity),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czPLMN_Identityspec),
	NULLP
};
CmPAsnBitStrDef czEUTRANCellIdentifierspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTR,
#endif /* CM_PASN_DEBUG */
	28,
	28
};
CmPAsnElmDef czEUTRANCellIdentifier =
{
#ifdef CM_PASN_DBG
	" czEUTRANCellIdentifier ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTR,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztEUTRANCellIdentifier),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czEUTRANCellIdentifierspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnECGI_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnECGI_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnECGI_ExtIEsClsInfo =
{
	0,
	&cz_ExtnECGI_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnECGI_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnECGI_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnECGI_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnECGI_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnECGI_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnECGI_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnECGI_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_ECGI_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnECGI_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_ECGI_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_ECGI_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_ECGI_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_ECGI_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_ECGI_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_ECGI_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_ECGI_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_ECGI_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_ECGI_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_ECGI_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_ECGI_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_ECGI_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_ECGI_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_ECGI_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_ECGI_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czECGITkn[] =
{
	&czPLMN_Identity,
	&czEUTRANCellIdentifier,
	&czOptProtExtnCont_ECGI_ExtIEs,
		&czProtExtnField_ECGI_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czECGIspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czECGITkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czECGI =
{
#ifdef CM_PASN_DBG
	" czECGI ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztECGI),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czECGIspec),
	NULLP
};
CmPAsnSetSeqOfDef czCellIdLstforMDTspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxnoofCellIDforMDT,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czCellIdLstforMDT =
{
#ifdef CM_PASN_DBG
	" czCellIdLstforMDT ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztCellIdLstforMDT),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czCellIdLstforMDTspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnCellBasedMDT_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnCellBasedMDT_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnCellBasedMDT_ExtIEsClsInfo =
{
	0,
	&cz_ExtnCellBasedMDT_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnCellBasedMDT_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnCellBasedMDT_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnCellBasedMDT_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnCellBasedMDT_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnCellBasedMDT_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnCellBasedMDT_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnCellBasedMDT_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_CellBasedMDT_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnCellBasedMDT_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_CellBasedMDT_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_CellBasedMDT_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_CellBasedMDT_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_CellBasedMDT_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_CellBasedMDT_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_CellBasedMDT_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_CellBasedMDT_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_CellBasedMDT_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_CellBasedMDT_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_CellBasedMDT_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_CellBasedMDT_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_CellBasedMDT_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_CellBasedMDT_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_CellBasedMDT_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_CellBasedMDT_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czCellBasedMDTTkn[] =
{
	&czCellIdLstforMDT,
		&czECGI,
		&czConstTerm,
	&czOptProtExtnCont_CellBasedMDT_ExtIEs,
		&czProtExtnField_CellBasedMDT_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czCellBasedMDTspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czCellBasedMDTTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czCellBasedMDT =
{
#ifdef CM_PASN_DBG
	" czCellBasedMDT ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztCellBasedMDT),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czCellBasedMDTspec),
	NULLP
};
CmPAsnOctStrDef czTACspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_OCTSTR,
#endif /* CM_PASN_DEBUG */
	2,
	2
};
CmPAsnElmDef czTAC =
{
#ifdef CM_PASN_DBG
	" czTAC " ,
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_OCTSTR,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztTAC),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czTACspec),
	NULLP
};
CmPAsnSetSeqOfDef czTALstforMDTspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxnoofTAforMDT,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czTALstforMDT =
{
#ifdef CM_PASN_DBG
	" czTALstforMDT ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztTALstforMDT),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czTALstforMDTspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnTABasedMDT_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnTABasedMDT_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnTABasedMDT_ExtIEsClsInfo =
{
	0,
	&cz_ExtnTABasedMDT_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnTABasedMDT_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnTABasedMDT_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnTABasedMDT_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnTABasedMDT_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnTABasedMDT_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnTABasedMDT_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnTABasedMDT_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_TABasedMDT_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnTABasedMDT_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_TABasedMDT_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_TABasedMDT_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_TABasedMDT_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_TABasedMDT_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_TABasedMDT_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_TABasedMDT_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_TABasedMDT_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_TABasedMDT_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_TABasedMDT_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_TABasedMDT_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_TABasedMDT_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_TABasedMDT_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_TABasedMDT_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_TABasedMDT_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_TABasedMDT_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czTABasedMDTTkn[] =
{
	&czTALstforMDT,
		&czTAC,
		&czConstTerm,
	&czOptProtExtnCont_TABasedMDT_ExtIEs,
		&czProtExtnField_TABasedMDT_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czTABasedMDTspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czTABasedMDTTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czTABasedMDT =
{
#ifdef CM_PASN_DBG
	" czTABasedMDT ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztTABasedMDT),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czTABasedMDTspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnTAI_Item_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnTAI_Item_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnTAI_Item_ExtIEsClsInfo =
{
	0,
	&cz_ExtnTAI_Item_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnTAI_Item_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnTAI_Item_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnTAI_Item_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnTAI_Item_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnTAI_Item_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnTAI_Item_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnTAI_Item_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_TAI_Item_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnTAI_Item_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_TAI_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_TAI_Item_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_TAI_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_TAI_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_TAI_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_TAI_Item_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_TAI_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_TAI_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_TAI_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_TAI_Item_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_TAI_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_TAI_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_TAI_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_TAI_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_TAI_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czTAI_ItemTkn[] =
{
	&czTAC,
	&czPLMN_Identity,
	&czOptProtExtnCont_TAI_Item_ExtIEs,
		&czProtExtnField_TAI_Item_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czTAI_Itemspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czTAI_ItemTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czTAI_Item =
{
#ifdef CM_PASN_DBG
	" czTAI_Item ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztTAI_Item),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czTAI_Itemspec),
	NULLP
};
CmPAsnSetSeqOfDef czTAILstforMDTspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxnoofTAforMDT,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czTAILstforMDT =
{
#ifdef CM_PASN_DBG
	" czTAILstforMDT ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztTAILstforMDT),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czTAILstforMDTspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnTAIBasedMDT_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnTAIBasedMDT_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnTAIBasedMDT_ExtIEsClsInfo =
{
	0,
	&cz_ExtnTAIBasedMDT_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnTAIBasedMDT_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnTAIBasedMDT_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnTAIBasedMDT_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnTAIBasedMDT_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnTAIBasedMDT_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnTAIBasedMDT_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnTAIBasedMDT_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_TAIBasedMDT_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnTAIBasedMDT_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_TAIBasedMDT_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_TAIBasedMDT_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_TAIBasedMDT_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_TAIBasedMDT_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_TAIBasedMDT_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_TAIBasedMDT_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_TAIBasedMDT_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_TAIBasedMDT_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_TAIBasedMDT_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_TAIBasedMDT_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_TAIBasedMDT_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_TAIBasedMDT_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_TAIBasedMDT_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_TAIBasedMDT_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_TAIBasedMDT_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czTAIBasedMDTTkn[] =
{
	&czTAILstforMDT,
		&czTAI_Item,
		&czConstTerm,
	&czOptProtExtnCont_TAIBasedMDT_ExtIEs,
		&czProtExtnField_TAIBasedMDT_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czTAIBasedMDTspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czTAIBasedMDTTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czTAIBasedMDT =
{
#ifdef CM_PASN_DBG
	" czTAIBasedMDT ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztTAIBasedMDT),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czTAIBasedMDTspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czAreaScopeOfMDTTkn[] =
{
	&czCellBasedMDT,
	&czTABasedMDT,
	&czNull,
	&czExtMkr,
	&czTAIBasedMDT,
	&czConstTerm
};
CmPAsnChoiceDef czAreaScopeOfMDTspec = 
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CHOICE,
#endif
	3,
	1,
	&czAreaScopeOfMDTTkn[0]  /* Token List */
};
CmPAsnElmDef czAreaScopeOfMDT =
{
#ifdef CM_PASN_DBG
	" czAreaScopeOfMDT ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CHOICE,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztAreaScopeOfMDT),
	&flagCz3M,
	NULLP,
	(U8 *)(&czAreaScopeOfMDTspec),
	NULLP
};
/* Element ----- BitRate ------- */
CmPAsnUIntXLDef czBitRatespec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINTXL,
#endif /* CM_PASN_DEBUG */
	{
		1,
		{0} 
	},
	{
		5,
		{2, 84, 11, 228, 0} 
	}
};
CmPAsnElmDef czBitRate =
{
#ifdef CM_PASN_DBG
	" czBitRate ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINTXL,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztBitRate),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czBitRatespec),
	NULLP
};

CmPAsnSetSeqOfDef czBroadcastPLMNs_Itemspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxnoofBPLMNs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czBroadcastPLMNs_Item =
{
#ifdef CM_PASN_DBG
	" czBroadcastPLMNs_Item ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztBroadcastPLMNs_Item),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czBroadcastPLMNs_Itemspec),
	NULLP
};
/* Element ----- CapacityValue ------- */
CmPAsnUIntDef czCapacityValuespec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	100UL
};
CmPAsnElmDef czCapacityValue =
{
#ifdef CM_PASN_DBG
	" czCapacityValue ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztCapacityValue),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czCapacityValuespec),
	NULLP
};

/* Element ----- CellCapacityClassValue ------- */
CmPAsnUIntDef czCellCapacityClassValuespec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	1,
	100UL
};
CmPAsnElmDef czOptCellCapacityClassValue =
{
#ifdef CM_PASN_DBG
	" czCellCapacityClassValue ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztCellCapacityClassValue),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czCellCapacityClassValuespec),
	NULLP
};

/* Element ----- CauseRadioNetwork ------- */
CmPAsnTknEnum czCauseRadioNwExtLst[8] =
{
	7,
	CztCauseRadioNwload_balancingEnum,
	CztCauseRadioNwhandover_optimisationEnum,
	CztCauseRadioNwvalue_out_of_allowed_rangeEnum,
	CztCauseRadioNwmultiple_E_RAB_ID_instancesEnum,
	CztCauseRadioNwswitch_off_ongoingEnum,
	CztCauseRadioNwnot_supported_QCI_valueEnum,
	CztCauseRadioNwmeasurement_not_supported_for_the_objectEnum
};
CmPAsnTknEnum czCauseRadioNwLst[23] =
{
	22,
	CztCauseRadioNwhandover_desirable_for_radio_reasonsEnum,
	CztCauseRadioNwtime_critical_handoverEnum,
	CztCauseRadioNwresource_optimisation_handoverEnum,
	CztCauseRadioNwreduce_load_in_serving_cellEnum,
	CztCauseRadioNwpartial_handoverEnum,
	CztCauseRadioNwunknown_new_eNB_UE_X2AP_IDEnum,
	CztCauseRadioNwunknown_old_eNB_UE_X2AP_IDEnum,
	CztCauseRadioNwunknown_pair_of_UE_X2AP_IDEnum,
	CztCauseRadioNwho_target_not_allowedEnum,
	CztCauseRadioNwtx2relocoverall_expiryEnum,
	CztCauseRadioNwtrelocprep_expiryEnum,
	CztCauseRadioNwcell_not_availableEnum,
	CztCauseRadioNwno_radio_resources_available_in_target_cellEnum,
	CztCauseRadioNwinvalid_MME_GroupIDEnum,
	CztCauseRadioNwunknown_MME_CodeEnum,
	CztCauseRadioNwencryption_and_or_integrity_protection_algorithms_not_supportedEnum,
	CztCauseRadioNwreportCharacteristicsEmptyEnum,
	CztCauseRadioNwnoReportPeriodicityEnum,
	CztCauseRadioNwexistingMeasurementIDEnum,
	CztCauseRadioNwunknown_eNB_Measurement_IDEnum,
	CztCauseRadioNwmeasurement_temporarily_not_availableEnum,
	CztCauseRadioNwunspecifiedEnum
};
CmPAsnEnumDef czCauseRadioNwspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czCauseRadioNwLst,
	czCauseRadioNwExtLst
};
CmPAsnElmDef czCauseRadioNw =
{
#ifdef CM_PASN_DBG
	" czCauseRadioNw ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztCauseRadioNw),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czCauseRadioNwspec),
	NULLP
};

/* Element ----- CauseTransport ------- */
CmPAsnTknEnum czCauseTportLst[3] =
{
	2,
	CztCauseTporttransport_resource_unavailableEnum,
	CztCauseTportunspecifiedEnum
};
CmPAsnEnumDef czCauseTportspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czCauseTportLst,
	NULLP
};
CmPAsnElmDef czCauseTport =
{
#ifdef CM_PASN_DBG
	" czCauseTport ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztCauseTport),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czCauseTportspec),
	NULLP
};

/* Element ----- CauseProtocol ------- */
CmPAsnTknEnum czCauseProtLst[8] =
{
	7,
	CztCauseProttransfer_syntax_errorEnum,
	CztCauseProtabstract_syntax_error_rejectEnum,
	CztCauseProtabstract_syntax_error_ignore_and_notifyEnum,
	CztCauseProtmessage_not_compatible_with_receiver_stateEnum,
	CztCauseProtsemantic_errorEnum,
	CztCauseProtunspecifiedEnum,
	CztCauseProtabstract_syntax_error_falsely_constructed_messageEnum
};
CmPAsnEnumDef czCauseProtspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czCauseProtLst,
	NULLP
};
CmPAsnElmDef czCauseProt =
{
#ifdef CM_PASN_DBG
	" czCauseProt ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztCauseProt),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czCauseProtspec),
	NULLP
};

/* Element ----- CauseMisc ------- */
CmPAsnTknEnum czCauseMiscLst[6] =
{
	5,
	CztCauseMisccontrol_processing_overloadEnum,
	CztCauseMischardware_failureEnum,
	CztCauseMiscom_interventionEnum,
	CztCauseMiscnot_enough_user_plane_processing_resourcesEnum,
	CztCauseMiscunspecifiedEnum
};
CmPAsnEnumDef czCauseMiscspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czCauseMiscLst,
	NULLP
};
CmPAsnElmDef czCauseMisc =
{
#ifdef CM_PASN_DBG
	" czCauseMisc ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztCauseMisc),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czCauseMiscspec),
	NULLP
};

PUBLIC CmPAsnElmDef *czCauseTkn[] =
{
	&czCauseRadioNw,
	&czCauseTport,
	&czCauseProt,
	&czCauseMisc,
	&czExtMkr,
	&czConstTerm
};
CmPAsnChoiceDef czCausespec = 
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CHOICE,
#endif
	4,
	0,
	&czCauseTkn[0]  /* Token List */
};
CmPAsnElmDef czCause =
{
#ifdef CM_PASN_DBG
	" czCause ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CHOICE,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztCause),
	&flagCz3M,
	NULLP,
	(U8 *)(&czCausespec),
	NULLP
};
/* Element ----- Cell-Size ------- */
CmPAsnTknEnum czCell_SizeLst[5] =
{
	4,
	CztCell_SizeverysmallEnum,
	CztCell_SizesmallEnum,
	CztCell_SizemediumEnum,
	CztCell_SizelargeEnum
};
CmPAsnEnumDef czCell_Sizespec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czCell_SizeLst,
	NULLP
};
CmPAsnElmDef czCell_Size =
{
#ifdef CM_PASN_DBG
	" czCell_Size ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztCell_Size),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czCell_Sizespec),
	NULLP
};

PUBLIC CmPAsnElmDef *cz_ExtnCellTyp_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnCellTyp_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnCellTyp_ExtIEsClsInfo =
{
	0,
	&cz_ExtnCellTyp_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnCellTyp_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnCellTyp_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnCellTyp_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnCellTyp_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnCellTyp_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnCellTyp_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnCellTyp_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_CellTyp_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnCellTyp_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_CellTyp_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_CellTyp_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_CellTyp_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_CellTyp_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_CellTyp_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_CellTyp_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_CellTyp_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_CellTyp_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_CellTyp_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_CellTyp_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_CellTyp_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_CellTyp_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_CellTyp_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_CellTyp_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_CellTyp_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czCellTypTkn[] =
{
	&czCell_Size,
	&czOptProtExtnCont_CellTyp_ExtIEs,
		&czProtExtnField_CellTyp_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czCellTypspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czCellTypTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czCellTyp =
{
#ifdef CM_PASN_DBG
	" czCellTyp ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztCellTyp),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czCellTypspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnCompositeAvailableCapacity_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnCompositeAvailableCapacity_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnCompositeAvailableCapacity_ExtIEsClsInfo =
{
	0,
	&cz_ExtnCompositeAvailableCapacity_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnCompositeAvailableCapacity_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnCompositeAvailableCapacity_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnCompositeAvailableCapacity_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnCompositeAvailableCapacity_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnCompositeAvailableCapacity_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnCompositeAvailableCapacity_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnCompositeAvailableCapacity_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_CompositeAvailableCapacity_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnCompositeAvailableCapacity_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_CompositeAvailableCapacity_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_CompositeAvailableCapacity_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_CompositeAvailableCapacity_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_CompositeAvailableCapacity_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_CompositeAvailableCapacity_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_CompositeAvailableCapacity_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_CompositeAvailableCapacity_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_CompositeAvailableCapacity_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_CompositeAvailableCapacity_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_CompositeAvailableCapacity_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_CompositeAvailableCapacity_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_CompositeAvailableCapacity_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_CompositeAvailableCapacity_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_CompositeAvailableCapacity_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_CompositeAvailableCapacity_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czCompositeAvailableCapacityTkn[] =
{
	&czOptCellCapacityClassValue,
	&czCapacityValue,
	&czOptProtExtnCont_CompositeAvailableCapacity_ExtIEs,
		&czProtExtnField_CompositeAvailableCapacity_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czCompositeAvailableCapacityspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	2,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czCompositeAvailableCapacityTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czCompositeAvailableCapacity =
{
#ifdef CM_PASN_DBG
	" czCompositeAvailableCapacity ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztCompositeAvailableCapacity),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czCompositeAvailableCapacityspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnCompositeAvailableCapacityGroup_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnCompositeAvailableCapacityGroup_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnCompositeAvailableCapacityGroup_ExtIEsClsInfo =
{
	0,
	&cz_ExtnCompositeAvailableCapacityGroup_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnCompositeAvailableCapacityGroup_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnCompositeAvailableCapacityGroup_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnCompositeAvailableCapacityGroup_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnCompositeAvailableCapacityGroup_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnCompositeAvailableCapacityGroup_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnCompositeAvailableCapacityGroup_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnCompositeAvailableCapacityGroup_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_CompositeAvailableCapacityGroup_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnCompositeAvailableCapacityGroup_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_CompositeAvailableCapacityGroup_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_CompositeAvailableCapacityGroup_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_CompositeAvailableCapacityGroup_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_CompositeAvailableCapacityGroup_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_CompositeAvailableCapacityGroup_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_CompositeAvailableCapacityGroup_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_CompositeAvailableCapacityGroup_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_CompositeAvailableCapacityGroup_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_CompositeAvailableCapacityGroup_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_CompositeAvailableCapacityGroup_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_CompositeAvailableCapacityGroup_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_CompositeAvailableCapacityGroup_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_CompositeAvailableCapacityGroup_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_CompositeAvailableCapacityGroup_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_CompositeAvailableCapacityGroup_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czCompositeAvailableCapacityGroupTkn[] =
{
	&czCompositeAvailableCapacity,
	&czCompositeAvailableCapacity,
	&czOptProtExtnCont_CompositeAvailableCapacityGroup_ExtIEs,
		&czProtExtnField_CompositeAvailableCapacityGroup_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czCompositeAvailableCapacityGroupspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czCompositeAvailableCapacityGroupTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czCompositeAvailableCapacityGroup =
{
#ifdef CM_PASN_DBG
	" czCompositeAvailableCapacityGroup ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztCompositeAvailableCapacityGroup),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czCompositeAvailableCapacityGroupspec),
	NULLP
};
/* Element ----- PDCP-SN ------- */
CmPAsnUIntDef czPDCP_SNspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	4095UL
};
CmPAsnElmDef czPDCP_SN =
{
#ifdef CM_PASN_DBG
	" czPDCP_SN ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztPDCP_SN),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czPDCP_SNspec),
	NULLP
};

/* Element ----- HFN ------- */
CmPAsnUIntDef czHFNspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	1048575UL
};
CmPAsnElmDef czHFN =
{
#ifdef CM_PASN_DBG
	" czHFN ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztHFN),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czHFNspec),
	NULLP
};

PUBLIC CmPAsnElmDef *cz_ExtnCOUNTvalue_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnCOUNTvalue_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnCOUNTvalue_ExtIEsClsInfo =
{
	0,
	&cz_ExtnCOUNTvalue_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnCOUNTvalue_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnCOUNTvalue_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnCOUNTvalue_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnCOUNTvalue_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnCOUNTvalue_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnCOUNTvalue_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnCOUNTvalue_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_COUNTvalue_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnCOUNTvalue_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_COUNTvalue_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_COUNTvalue_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_COUNTvalue_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_COUNTvalue_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_COUNTvalue_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_COUNTvalue_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_COUNTvalue_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_COUNTvalue_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_COUNTvalue_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_COUNTvalue_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_COUNTvalue_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_COUNTvalue_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_COUNTvalue_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_COUNTvalue_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_COUNTvalue_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czCOUNTvalueTkn[] =
{
	&czPDCP_SN,
	&czHFN,
	&czOptProtExtnCont_COUNTvalue_ExtIEs,
		&czProtExtnField_COUNTvalue_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czCOUNTvaluespec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czCOUNTvalueTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czCOUNTvalue =
{
#ifdef CM_PASN_DBG
	" czCOUNTvalue ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztCOUNTvalue),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czCOUNTvaluespec),
	NULLP
};
/* Element ----- PDCP-SNExtended ------- */
CmPAsnUIntDef czPDCP_SNExtendedspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	32767UL
};
CmPAsnElmDef czPDCP_SNExtended =
{
#ifdef CM_PASN_DBG
	" czPDCP_SNExtended ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztPDCP_SNExtended),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czPDCP_SNExtendedspec),
	NULLP
};

/* Element ----- HFNModified ------- */
CmPAsnUIntDef czHFNMdfdspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	131071UL
};
CmPAsnElmDef czHFNMdfd =
{
#ifdef CM_PASN_DBG
	" czHFNMdfd ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztHFNMdfd),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czHFNMdfdspec),
	NULLP
};

PUBLIC CmPAsnElmDef *cz_ExtnCOUNTValueExtended_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnCOUNTValueExtended_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnCOUNTValueExtended_ExtIEsClsInfo =
{
	0,
	&cz_ExtnCOUNTValueExtended_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnCOUNTValueExtended_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnCOUNTValueExtended_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnCOUNTValueExtended_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnCOUNTValueExtended_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnCOUNTValueExtended_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnCOUNTValueExtended_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnCOUNTValueExtended_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_COUNTValueExtended_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnCOUNTValueExtended_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_COUNTValueExtended_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_COUNTValueExtended_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_COUNTValueExtended_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_COUNTValueExtended_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_COUNTValueExtended_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_COUNTValueExtended_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_COUNTValueExtended_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_COUNTValueExtended_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_COUNTValueExtended_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_COUNTValueExtended_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_COUNTValueExtended_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_COUNTValueExtended_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_COUNTValueExtended_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_COUNTValueExtended_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_COUNTValueExtended_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czCOUNTValueExtendedTkn[] =
{
	&czPDCP_SNExtended,
	&czHFNMdfd,
	&czOptProtExtnCont_COUNTValueExtended_ExtIEs,
		&czProtExtnField_COUNTValueExtended_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czCOUNTValueExtendedspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czCOUNTValueExtendedTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czCOUNTValueExtended =
{
#ifdef CM_PASN_DBG
	" czCOUNTValueExtended ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztCOUNTValueExtended),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czCOUNTValueExtendedspec),
	NULLP
};
/* Element ----- TypeOfError ------- */
CmPAsnTknEnum czTypOfErrLst[3] =
{
	2,
	CztTypOfErrnot_understoodEnum,
	CztTypOfErrmissingEnum
};
CmPAsnEnumDef czTypOfErrspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czTypOfErrLst,
	NULLP
};
CmPAsnElmDef czTypOfErr =
{
#ifdef CM_PASN_DBG
	" czTypOfErr ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztTypOfErr),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czTypOfErrspec),
	NULLP
};

PUBLIC CmPAsnElmDef *cz_ExtnCriticalityDiag_IE_Lst_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnCriticalityDiag_IE_Lst_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnCriticalityDiag_IE_Lst_ExtIEsClsInfo =
{
	0,
	&cz_ExtnCriticalityDiag_IE_Lst_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnCriticalityDiag_IE_Lst_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnCriticalityDiag_IE_Lst_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnCriticalityDiag_IE_Lst_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnCriticalityDiag_IE_Lst_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnCriticalityDiag_IE_Lst_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnCriticalityDiag_IE_Lst_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnCriticalityDiag_IE_Lst_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_CriticalityDiag_IE_Lst_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnCriticalityDiag_IE_Lst_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_CriticalityDiag_IE_Lst_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_CriticalityDiag_IE_Lst_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_CriticalityDiag_IE_Lst_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_CriticalityDiag_IE_Lst_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_CriticalityDiag_IE_Lst_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_CriticalityDiag_IE_Lst_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_CriticalityDiag_IE_Lst_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_CriticalityDiag_IE_Lst_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_CriticalityDiag_IE_Lst_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_CriticalityDiag_IE_Lst_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_CriticalityDiag_IE_Lst_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_CriticalityDiag_IE_Lst_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_CriticalityDiag_IE_Lst_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_CriticalityDiag_IE_Lst_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_CriticalityDiag_IE_Lst_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czCriticalityDiag_IE_LstMemberTkn[] =
{
	&czCriticality,
	&czProtIE_ID,
	&czTypOfErr,
	&czOptProtExtnCont_CriticalityDiag_IE_Lst_ExtIEs,
		&czProtExtnField_CriticalityDiag_IE_Lst_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czCriticalityDiag_IE_LstMemberspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czCriticalityDiag_IE_LstMemberTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czCriticalityDiag_IE_LstMember =
{
#ifdef CM_PASN_DBG
	" czCriticalityDiag_IE_LstMember ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztCriticalityDiag_IE_LstMember),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czCriticalityDiag_IE_LstMemberspec),
	NULLP
};
CmPAsnSetSeqOfDef czCriticalityDiag_IE_Lstspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxNrOfErrs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptCriticalityDiag_IE_Lst =
{
#ifdef CM_PASN_DBG
	" czCriticalityDiag_IE_Lst ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztCriticalityDiag_IE_Lst),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czCriticalityDiag_IE_Lstspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnCriticalityDiag_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnCriticalityDiag_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnCriticalityDiag_ExtIEsClsInfo =
{
	0,
	&cz_ExtnCriticalityDiag_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnCriticalityDiag_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnCriticalityDiag_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnCriticalityDiag_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnCriticalityDiag_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnCriticalityDiag_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnCriticalityDiag_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnCriticalityDiag_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_CriticalityDiag_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnCriticalityDiag_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_CriticalityDiag_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_CriticalityDiag_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_CriticalityDiag_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_CriticalityDiag_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_CriticalityDiag_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_CriticalityDiag_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_CriticalityDiag_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_CriticalityDiag_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_CriticalityDiag_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_CriticalityDiag_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_CriticalityDiag_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_CriticalityDiag_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_CriticalityDiag_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_CriticalityDiag_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_CriticalityDiag_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czCriticalityDiagTkn[] =
{
	&czOptProcedureCode,
	&czOptTrgMsg,
	&czOptCriticality,
	&czOptCriticalityDiag_IE_Lst,
		&czCriticalityDiag_IE_LstMember,
		&czConstTerm,
	&czOptProtExtnCont_CriticalityDiag_ExtIEs,
		&czProtExtnField_CriticalityDiag_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czCriticalityDiagspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	5,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czCriticalityDiagTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czCriticalityDiag =
{
#ifdef CM_PASN_DBG
	" czCriticalityDiag ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztCriticalityDiag),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czCriticalityDiagspec),
	NULLP
};
CmPAsnBitStrDef czCRNTIspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTR,
#endif /* CM_PASN_DEBUG */
	16,
	16
};
CmPAsnElmDef czCRNTI =
{
#ifdef CM_PASN_DBG
	" czCRNTI ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTR,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztCRNTI),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czCRNTIspec),
	NULLP
};
/* Element ----- CSGMembershipStatus ------- */
CmPAsnTknEnum czCSGMembershipStatusLst[3] =
{
	2,
	CztCSGMembershipStatusmemberEnum,
	CztCSGMembershipStatusnot_memberEnum
};
CmPAsnEnumDef czCSGMembershipStatusspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czCSGMembershipStatusLst,
	NULLP
};
CmPAsnElmDef czCSGMembershipStatus =
{
#ifdef CM_PASN_DBG
	" czCSGMembershipStatus ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztCSGMembershipStatus),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czCSGMembershipStatusspec),
	NULLP
};

CmPAsnBitStrDef czCSG_Idspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTR,
#endif /* CM_PASN_DEBUG */
	27,
	27
};
CmPAsnElmDef czCSG_Id =
{
#ifdef CM_PASN_DBG
	" czCSG_Id ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTR,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztCSG_Id),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czCSG_Idspec),
	NULLP
};
/* Element ----- DeactivationIndication ------- */
CmPAsnTknEnum czDeactivationIndLst[2] =
{
	1,
	CztDeactivationInddeactivatedEnum
};
CmPAsnEnumDef czDeactivationIndspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czDeactivationIndLst,
	NULLP
};
CmPAsnElmDef czDeactivationInd =
{
#ifdef CM_PASN_DBG
	" czDeactivationInd ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztDeactivationInd),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czDeactivationIndspec),
	NULLP
};

/* Element ----- DL-Forwarding ------- */
CmPAsnTknEnum czDL_FwdingLst[2] =
{
	1,
	CztDL_FwdingdL_forwardingProposedEnum
};
CmPAsnEnumDef czDL_Fwdingspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czDL_FwdingLst,
	NULLP
};
CmPAsnElmDef czOptDL_Fwding =
{
#ifdef CM_PASN_DBG
	" czDL_Fwding ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztDL_Fwding),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czDL_Fwdingspec),
	NULLP
};

/* Element ----- DL-GBR-PRB-usage ------- */
CmPAsnUIntDef czDL_GBR_PRB_usagespec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	100UL
};
CmPAsnElmDef czDL_GBR_PRB_usage =
{
#ifdef CM_PASN_DBG
	" czDL_GBR_PRB_usage ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztDL_GBR_PRB_usage),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czDL_GBR_PRB_usagespec),
	NULLP
};

/* Element ----- DL-non-GBR-PRB-usage ------- */
CmPAsnUIntDef czDL_non_GBR_PRB_usagespec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	100UL
};
CmPAsnElmDef czDL_non_GBR_PRB_usage =
{
#ifdef CM_PASN_DBG
	" czDL_non_GBR_PRB_usage ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztDL_non_GBR_PRB_usage),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czDL_non_GBR_PRB_usagespec),
	NULLP
};

/* Element ----- DL-Total-PRB-usage ------- */
CmPAsnUIntDef czDL_Total_PRB_usagespec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	100UL
};
CmPAsnElmDef czDL_Total_PRB_usage =
{
#ifdef CM_PASN_DBG
	" czDL_Total_PRB_usage ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztDL_Total_PRB_usage),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czDL_Total_PRB_usagespec),
	NULLP
};

/* Element ----- EARFCN ------- */
CmPAsnUIntDef czEARFCNspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	CztmaxEARFCN
};
CmPAsnElmDef czEARFCN =
{
#ifdef CM_PASN_DBG
	" czEARFCN ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztEARFCN),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czEARFCNspec),
	NULLP
};

/* Element ----- EARFCNExtension ------- */
CmPAsnUIntDef czEARFCNExtnspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	154550280,
	CztnewmaxEARFCN
};
CmPAsnElmDef czEARFCNExtn =
{
#ifdef CM_PASN_DBG
	" czEARFCNExtn ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztEARFCNExtn),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czEARFCNExtnspec),
	NULLP
};

/* Element ----- Transmission-Bandwidth ------- */
CmPAsnTknEnum czTransmission_BandwidthLst[7] =
{
	6,
	CztTransmission_Bandwidthbw6Enum,
	CztTransmission_Bandwidthbw15Enum,
	CztTransmission_Bandwidthbw25Enum,
	CztTransmission_Bandwidthbw50Enum,
	CztTransmission_Bandwidthbw75Enum,
	CztTransmission_Bandwidthbw100Enum
};
CmPAsnEnumDef czTransmission_Bandwidthspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czTransmission_BandwidthLst,
	NULLP
};
CmPAsnElmDef czTransmission_Bandwidth =
{
#ifdef CM_PASN_DBG
	" czTransmission_Bandwidth ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztTransmission_Bandwidth),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czTransmission_Bandwidthspec),
	NULLP
};

PUBLIC CmPAsnElmDef *cz_ExtnFDD_Info_ExtIEsClsInst[] =
{
	&czEARFCNExtn,
	&czEARFCNExtn
};
PUBLIC U32 cz_ExtnFDD_Info_ExtIEsClsInstId[] =
{
	Cztid_UL_EARFCNExtn,
	Cztid_DL_EARFCNExtn
};
PUBLIC CmPAsnClassInfo cz_ExtnFDD_Info_ExtIEsClsInfo =
{
	2,
	&cz_ExtnFDD_Info_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnFDD_Info_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnFDD_Info_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnFDD_Info_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnFDD_Info_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnFDD_Info_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnFDD_Info_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnFDD_Info_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_FDD_Info_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnFDD_Info_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_FDD_Info_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_FDD_Info_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_FDD_Info_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_FDD_Info_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_FDD_Info_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_FDD_Info_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_FDD_Info_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_FDD_Info_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_FDD_Info_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_FDD_Info_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_FDD_Info_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_FDD_Info_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_FDD_Info_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_FDD_Info_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_FDD_Info_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czFDD_InfoTkn[] =
{
	&czEARFCN,
	&czEARFCN,
	&czTransmission_Bandwidth,
	&czTransmission_Bandwidth,
	&czOptProtExtnCont_FDD_Info_ExtIEs,
		&czProtExtnField_FDD_Info_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czFDD_Infospec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czFDD_InfoTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czFDD_Info =
{
#ifdef CM_PASN_DBG
	" czFDD_Info ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztFDD_Info),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czFDD_Infospec),
	NULLP
};
/* Element ----- SubframeAssignment ------- */
CmPAsnTknEnum czSubframeAssignmentLst[8] =
{
	7,
	CztSubframeAssignmentsa0Enum,
	CztSubframeAssignmentsa1Enum,
	CztSubframeAssignmentsa2Enum,
	CztSubframeAssignmentsa3Enum,
	CztSubframeAssignmentsa4Enum,
	CztSubframeAssignmentsa5Enum,
	CztSubframeAssignmentsa6Enum
};
CmPAsnEnumDef czSubframeAssignmentspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czSubframeAssignmentLst,
	NULLP
};
CmPAsnElmDef czSubframeAssignment =
{
#ifdef CM_PASN_DBG
	" czSubframeAssignment ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztSubframeAssignment),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czSubframeAssignmentspec),
	NULLP
};

/* Element ----- SpecialSubframePatterns ------- */
CmPAsnTknEnum czSpecialSubframePatternsLst[10] =
{
	9,
	CztSpecialSubframePatternsssp0Enum,
	CztSpecialSubframePatternsssp1Enum,
	CztSpecialSubframePatternsssp2Enum,
	CztSpecialSubframePatternsssp3Enum,
	CztSpecialSubframePatternsssp4Enum,
	CztSpecialSubframePatternsssp5Enum,
	CztSpecialSubframePatternsssp6Enum,
	CztSpecialSubframePatternsssp7Enum,
	CztSpecialSubframePatternsssp8Enum
};
CmPAsnEnumDef czSpecialSubframePatternsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czSpecialSubframePatternsLst,
	NULLP
};
CmPAsnElmDef czSpecialSubframePatterns =
{
#ifdef CM_PASN_DBG
	" czSpecialSubframePatterns ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztSpecialSubframePatterns),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czSpecialSubframePatternsspec),
	NULLP
};

PUBLIC CmPAsnElmDef *cz_ExtnSpecialSubframe_Info_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnSpecialSubframe_Info_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnSpecialSubframe_Info_ExtIEsClsInfo =
{
	0,
	&cz_ExtnSpecialSubframe_Info_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnSpecialSubframe_Info_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnSpecialSubframe_Info_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnSpecialSubframe_Info_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnSpecialSubframe_Info_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnSpecialSubframe_Info_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnSpecialSubframe_Info_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnSpecialSubframe_Info_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_SpecialSubframe_Info_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnSpecialSubframe_Info_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_SpecialSubframe_Info_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_SpecialSubframe_Info_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_SpecialSubframe_Info_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_SpecialSubframe_Info_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_SpecialSubframe_Info_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_SpecialSubframe_Info_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_SpecialSubframe_Info_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_SpecialSubframe_Info_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_SpecialSubframe_Info_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_SpecialSubframe_Info_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_SpecialSubframe_Info_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_SpecialSubframe_Info_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_SpecialSubframe_Info_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_SpecialSubframe_Info_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_SpecialSubframe_Info_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czSpecialSubframe_InfoTkn[] =
{
	&czSpecialSubframePatterns,
	&czCyclicPrefixDL,
	&czCyclicPrefixUL,
	&czOptProtExtnCont_SpecialSubframe_Info_ExtIEs,
		&czProtExtnField_SpecialSubframe_Info_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czSpecialSubframe_Infospec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czSpecialSubframe_InfoTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czSpecialSubframe_Info =
{
#ifdef CM_PASN_DBG
	" czSpecialSubframe_Info ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztSpecialSubframe_Info),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czSpecialSubframe_Infospec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnTDD_Info_ExtIEsClsInst[] =
{
	&czAdditionalSpecialSubframe_Info,
	&czEARFCNExtn
};
PUBLIC U32 cz_ExtnTDD_Info_ExtIEsClsInstId[] =
{
	Cztid_AdditionalSpecialSubframe_Info,
	Cztid_eARFCNExtn
};
PUBLIC CmPAsnClassInfo cz_ExtnTDD_Info_ExtIEsClsInfo =
{
	2,
	&cz_ExtnTDD_Info_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnTDD_Info_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnTDD_Info_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnTDD_Info_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnTDD_Info_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnTDD_Info_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnTDD_Info_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnTDD_Info_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_TDD_Info_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnTDD_Info_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_TDD_Info_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_TDD_Info_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_TDD_Info_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_TDD_Info_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_TDD_Info_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_TDD_Info_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_TDD_Info_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_TDD_Info_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_TDD_Info_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_TDD_Info_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_TDD_Info_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_TDD_Info_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_TDD_Info_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_TDD_Info_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_TDD_Info_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czTDD_InfoTkn[] =
{
	&czEARFCN,
	&czTransmission_Bandwidth,
	&czSubframeAssignment,
	&czSpecialSubframe_Info,
	&czOptProtExtnCont_TDD_Info_ExtIEs,
		&czProtExtnField_TDD_Info_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czTDD_Infospec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czTDD_InfoTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czTDD_Info =
{
#ifdef CM_PASN_DBG
	" czTDD_Info ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztTDD_Info),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czTDD_Infospec),
	NULLP
};
PUBLIC CmPAsnElmDef *czEUTRA_Mode_InfoTkn[] =
{
	&czFDD_Info,
	&czTDD_Info,
	&czExtMkr,
	&czConstTerm
};
CmPAsnChoiceDef czEUTRA_Mode_Infospec = 
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CHOICE,
#endif
	2,
	0,
	&czEUTRA_Mode_InfoTkn[0]  /* Token List */
};
CmPAsnElmDef czEUTRA_Mode_Info =
{
#ifdef CM_PASN_DBG
	" czEUTRA_Mode_Info ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CHOICE,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztEUTRA_Mode_Info),
	&flagCz3M,
	NULLP,
	(U8 *)(&czEUTRA_Mode_Infospec),
	NULLP
};
CmPAsnBitStrDef czENB_IDmacro_eNB_IDspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTR,
#endif /* CM_PASN_DEBUG */
	20,
	20
};
CmPAsnElmDef czENB_IDmacro_eNB_ID =
{
#ifdef CM_PASN_DBG
	" czENB_IDmacro_eNB_ID ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTR,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztENB_IDmacro_eNB_ID),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czENB_IDmacro_eNB_IDspec),
	NULLP
};
CmPAsnBitStrDef czENB_IDhome_eNB_IDspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTR,
#endif /* CM_PASN_DEBUG */
	28,
	28
};
CmPAsnElmDef czENB_IDhome_eNB_ID =
{
#ifdef CM_PASN_DBG
	" czENB_IDhome_eNB_ID ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTR,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztENB_IDhome_eNB_ID),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czENB_IDhome_eNB_IDspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czENB_IDTkn[] =
{
	&czENB_IDmacro_eNB_ID,
	&czENB_IDhome_eNB_ID,
	&czExtMkr,
	&czConstTerm
};
CmPAsnChoiceDef czENB_IDspec = 
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CHOICE,
#endif
	2,
	0,
	&czENB_IDTkn[0]  /* Token List */
};
CmPAsnElmDef czENB_ID =
{
#ifdef CM_PASN_DBG
	" czENB_ID ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CHOICE,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztENB_ID),
	&flagCz3M,
	NULLP,
	(U8 *)(&czENB_IDspec),
	NULLP
};
CmPAsnBitStrDef czEncryptionAlgorithmsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTR,
#endif /* CM_PASN_DEBUG */
	16,
	16
};
CmPAsnElmDef czEncryptionAlgorithms =
{
#ifdef CM_PASN_DBG
	" czEncryptionAlgorithms ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTR,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztEncryptionAlgorithms),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czEncryptionAlgorithmsspec),
	NULLP
};
CmPAsnSetSeqOfDef czEPLMNsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxnoofEPLMNs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptEPLMNs =
{
#ifdef CM_PASN_DBG
	" czEPLMNs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztEPLMNs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czEPLMNsspec),
	NULLP
};
/* Element ----- E-RAB-ID ------- */
CmPAsnUIntDef czE_RAB_IDspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	15UL
};
CmPAsnElmDef czE_RAB_ID =
{
#ifdef CM_PASN_DBG
	" czE_RAB_ID ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztE_RAB_ID),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czE_RAB_IDspec),
	NULLP
};

/* Element ----- QCI ------- */
CmPAsnUIntDef czQCIspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	255UL
};
CmPAsnElmDef czQCI =
{
#ifdef CM_PASN_DBG
	" czQCI ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztQCI),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czQCIspec),
	NULLP
};

PUBLIC CmPAsnElmDef *cz_ExtnGBR_QosInform_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnGBR_QosInform_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnGBR_QosInform_ExtIEsClsInfo =
{
	0,
	&cz_ExtnGBR_QosInform_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnGBR_QosInform_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnGBR_QosInform_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnGBR_QosInform_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnGBR_QosInform_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnGBR_QosInform_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnGBR_QosInform_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnGBR_QosInform_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_GBR_QosInform_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnGBR_QosInform_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_GBR_QosInform_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_GBR_QosInform_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_GBR_QosInform_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_GBR_QosInform_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_GBR_QosInform_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_GBR_QosInform_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_GBR_QosInform_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_GBR_QosInform_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_GBR_QosInform_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_GBR_QosInform_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_GBR_QosInform_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_GBR_QosInform_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_GBR_QosInform_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_GBR_QosInform_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_GBR_QosInform_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czGBR_QosInformTkn[] =
{
	&czBitRate,
	&czBitRate,
	&czBitRate,
	&czBitRate,
	&czOptProtExtnCont_GBR_QosInform_ExtIEs,
		&czProtExtnField_GBR_QosInform_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czGBR_QosInformspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czGBR_QosInformTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czGBR_QosInform =
{
#ifdef CM_PASN_DBG
	" czGBR_QosInform ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztGBR_QosInform),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czGBR_QosInformspec),
	NULLP
};
PUBLIC CmPAsnElmDef czOptGBR_QosInform =
{
#ifdef CM_PASN_DBG
	" czGBR_QosInform ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztGBR_QosInform),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czGBR_QosInformspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnE_RAB_Lvl_QoS_Params_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnE_RAB_Lvl_QoS_Params_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnE_RAB_Lvl_QoS_Params_ExtIEsClsInfo =
{
	0,
	&cz_ExtnE_RAB_Lvl_QoS_Params_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnE_RAB_Lvl_QoS_Params_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnE_RAB_Lvl_QoS_Params_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnE_RAB_Lvl_QoS_Params_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnE_RAB_Lvl_QoS_Params_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnE_RAB_Lvl_QoS_Params_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnE_RAB_Lvl_QoS_Params_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnE_RAB_Lvl_QoS_Params_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_E_RAB_Lvl_QoS_Params_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnE_RAB_Lvl_QoS_Params_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_E_RAB_Lvl_QoS_Params_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_E_RAB_Lvl_QoS_Params_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_E_RAB_Lvl_QoS_Params_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_E_RAB_Lvl_QoS_Params_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_E_RAB_Lvl_QoS_Params_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_E_RAB_Lvl_QoS_Params_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_E_RAB_Lvl_QoS_Params_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_E_RAB_Lvl_QoS_Params_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_E_RAB_Lvl_QoS_Params_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_E_RAB_Lvl_QoS_Params_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_E_RAB_Lvl_QoS_Params_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_E_RAB_Lvl_QoS_Params_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_E_RAB_Lvl_QoS_Params_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_E_RAB_Lvl_QoS_Params_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_E_RAB_Lvl_QoS_Params_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czE_RAB_Lvl_QoS_ParamsTkn[] =
{
	&czQCI,
	&czAllocnAndRetentionPriority,
	&czOptGBR_QosInform,
	&czOptProtExtnCont_E_RAB_Lvl_QoS_Params_ExtIEs,
		&czProtExtnField_E_RAB_Lvl_QoS_Params_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czE_RAB_Lvl_QoS_Paramsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	2,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czE_RAB_Lvl_QoS_ParamsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czE_RAB_Lvl_QoS_Params =
{
#ifdef CM_PASN_DBG
	" czE_RAB_Lvl_QoS_Params ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztE_RAB_Lvl_QoS_Params),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czE_RAB_Lvl_QoS_Paramsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnE_RAB_Item_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnE_RAB_Item_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnE_RAB_Item_ExtIEsClsInfo =
{
	0,
	&cz_ExtnE_RAB_Item_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnE_RAB_Item_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnE_RAB_Item_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnE_RAB_Item_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnE_RAB_Item_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnE_RAB_Item_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnE_RAB_Item_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnE_RAB_Item_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_E_RAB_Item_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnE_RAB_Item_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_E_RAB_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_E_RAB_Item_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_E_RAB_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_E_RAB_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_E_RAB_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_E_RAB_Item_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_E_RAB_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_E_RAB_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_E_RAB_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_E_RAB_Item_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_E_RAB_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_E_RAB_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_E_RAB_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_E_RAB_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_E_RAB_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czE_RAB_ItemTkn[] =
{
	&czE_RAB_ID,
	&czCause,
	&czOptProtExtnCont_E_RAB_Item_ExtIEs,
		&czProtExtnField_E_RAB_Item_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czE_RAB_Itemspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czE_RAB_ItemTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czE_RAB_Item =
{
#ifdef CM_PASN_DBG
	" czE_RAB_Item ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztE_RAB_Item),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czE_RAB_Itemspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueE_RAB_ItemIEsClsInst[] =
{
	&czE_RAB_Item
};
PUBLIC U32 cz_ValueE_RAB_ItemIEsClsInstId[] =
{
	Cztid_E_RAB_Item
};
PUBLIC CmPAsnClassInfo cz_ValueE_RAB_ItemIEsClsInfo =
{
	1,
	&cz_ValueE_RAB_ItemIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueE_RAB_ItemIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueE_RAB_ItemIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueE_RAB_ItemIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueE_RAB_ItemIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueE_RAB_ItemIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueE_RAB_ItemIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueE_RAB_ItemIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_E_RAB_ItemIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueE_RAB_ItemIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_E_RAB_ItemIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_E_RAB_ItemIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_E_RAB_ItemIEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_E_RAB_ItemIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_E_RAB_ItemIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_E_RAB_ItemIEsspec),
	NULLP
};
#define czProtIE_Single_Cont_E_RAB_ItemIEs czProtIE_Field_E_RAB_ItemIEs
CmPAsnSetSeqOfDef czE_RAB_Lstspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxnoofBrs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czE_RAB_Lst =
{
#ifdef CM_PASN_DBG
	" czE_RAB_Lst ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztE_RAB_Lst),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czE_RAB_Lstspec),
	NULLP
};
CmPAsnOctStrDef czEUTRANTraceIDspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_OCTSTRXL,
#endif /* CM_PASN_DEBUG */
	8,
	8
};
CmPAsnElmDef czEUTRANTraceID =
{
#ifdef CM_PASN_DBG
	" czEUTRANTraceID " ,
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_OCTSTRXL,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztEUTRANTraceID),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czEUTRANTraceIDspec),
	NULLP
};
/* Element ----- EventType ------- */
CmPAsnTknEnum czEventTypLst[2] =
{
	1,
	CztEventTypchange_of_serving_cellEnum
};
CmPAsnEnumDef czEventTypspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czEventTypLst,
	NULLP
};
CmPAsnElmDef czEventTyp =
{
#ifdef CM_PASN_DBG
	" czEventTyp ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztEventTyp),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czEventTypspec),
	NULLP
};

/* Element ----- ForbiddenInterRATs ------- */
CmPAsnTknEnum czForbiddenInterRATsExtLst[3] =
{
	2,
	CztForbiddenInterRATsgeranandutranEnum,
	CztForbiddenInterRATscdma2000andutranEnum
};
CmPAsnTknEnum czForbiddenInterRATsLst[5] =
{
	4,
	CztForbiddenInterRATsallEnum,
	CztForbiddenInterRATsgeranEnum,
	CztForbiddenInterRATsutranEnum,
	CztForbiddenInterRATscdma2000Enum
};
CmPAsnEnumDef czForbiddenInterRATsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czForbiddenInterRATsLst,
	czForbiddenInterRATsExtLst
};
CmPAsnElmDef czOptForbiddenInterRATs =
{
#ifdef CM_PASN_DBG
	" czForbiddenInterRATs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztForbiddenInterRATs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czForbiddenInterRATsspec),
	NULLP
};

CmPAsnSetSeqOfDef czForbiddenTACsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxnoofForbTACs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czForbiddenTACs =
{
#ifdef CM_PASN_DBG
	" czForbiddenTACs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztForbiddenTACs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czForbiddenTACsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnForbiddenTAs_Item_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnForbiddenTAs_Item_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnForbiddenTAs_Item_ExtIEsClsInfo =
{
	0,
	&cz_ExtnForbiddenTAs_Item_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnForbiddenTAs_Item_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnForbiddenTAs_Item_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnForbiddenTAs_Item_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnForbiddenTAs_Item_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnForbiddenTAs_Item_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnForbiddenTAs_Item_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnForbiddenTAs_Item_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_ForbiddenTAs_Item_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnForbiddenTAs_Item_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_ForbiddenTAs_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_ForbiddenTAs_Item_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_ForbiddenTAs_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_ForbiddenTAs_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_ForbiddenTAs_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_ForbiddenTAs_Item_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_ForbiddenTAs_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_ForbiddenTAs_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_ForbiddenTAs_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_ForbiddenTAs_Item_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_ForbiddenTAs_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_ForbiddenTAs_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_ForbiddenTAs_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_ForbiddenTAs_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_ForbiddenTAs_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czForbiddenTAs_ItemTkn[] =
{
	&czPLMN_Identity,
	&czForbiddenTACs,
		&czTAC,
		&czConstTerm,
	&czOptProtExtnCont_ForbiddenTAs_Item_ExtIEs,
		&czProtExtnField_ForbiddenTAs_Item_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czForbiddenTAs_Itemspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czForbiddenTAs_ItemTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czForbiddenTAs_Item =
{
#ifdef CM_PASN_DBG
	" czForbiddenTAs_Item ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztForbiddenTAs_Item),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czForbiddenTAs_Itemspec),
	NULLP
};
CmPAsnSetSeqOfDef czForbiddenTAsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxnoofEPLMNsPlusOne,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptForbiddenTAs =
{
#ifdef CM_PASN_DBG
	" czForbiddenTAs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztForbiddenTAs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czForbiddenTAsspec),
	NULLP
};
CmPAsnOctStrDef czLACspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_OCTSTR,
#endif /* CM_PASN_DEBUG */
	2,
	2
};
CmPAsnElmDef czLAC =
{
#ifdef CM_PASN_DBG
	" czLAC " ,
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_OCTSTR,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztLAC),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czLACspec),
	NULLP
};
CmPAsnSetSeqOfDef czForbiddenLACsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxnoofForbLACs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czForbiddenLACs =
{
#ifdef CM_PASN_DBG
	" czForbiddenLACs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztForbiddenLACs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czForbiddenLACsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnForbiddenLAs_Item_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnForbiddenLAs_Item_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnForbiddenLAs_Item_ExtIEsClsInfo =
{
	0,
	&cz_ExtnForbiddenLAs_Item_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnForbiddenLAs_Item_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnForbiddenLAs_Item_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnForbiddenLAs_Item_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnForbiddenLAs_Item_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnForbiddenLAs_Item_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnForbiddenLAs_Item_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnForbiddenLAs_Item_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_ForbiddenLAs_Item_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnForbiddenLAs_Item_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_ForbiddenLAs_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_ForbiddenLAs_Item_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_ForbiddenLAs_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_ForbiddenLAs_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_ForbiddenLAs_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_ForbiddenLAs_Item_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_ForbiddenLAs_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_ForbiddenLAs_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_ForbiddenLAs_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_ForbiddenLAs_Item_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_ForbiddenLAs_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_ForbiddenLAs_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_ForbiddenLAs_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_ForbiddenLAs_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_ForbiddenLAs_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czForbiddenLAs_ItemTkn[] =
{
	&czPLMN_Identity,
	&czForbiddenLACs,
		&czLAC,
		&czConstTerm,
	&czOptProtExtnCont_ForbiddenLAs_Item_ExtIEs,
		&czProtExtnField_ForbiddenLAs_Item_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czForbiddenLAs_Itemspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czForbiddenLAs_ItemTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czForbiddenLAs_Item =
{
#ifdef CM_PASN_DBG
	" czForbiddenLAs_Item ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztForbiddenLAs_Item),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czForbiddenLAs_Itemspec),
	NULLP
};
CmPAsnSetSeqOfDef czForbiddenLAsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxnoofEPLMNsPlusOne,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptForbiddenLAs =
{
#ifdef CM_PASN_DBG
	" czForbiddenLAs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztForbiddenLAs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czForbiddenLAsspec),
	NULLP
};
CmPAsnBitStrDef czFourframesspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTR,
#endif /* CM_PASN_DEBUG */
	24,
	24
};
CmPAsnElmDef czFourframes =
{
#ifdef CM_PASN_DBG
	" czFourframes ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTR,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztFourframes),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czFourframesspec),
	NULLP
};
/* Element ----- FreqBandIndicator ------- */
CmPAsnUIntDef czFreqBandIndicatorspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	1,
	256UL
};
CmPAsnElmDef czFreqBandIndicator =
{
#ifdef CM_PASN_DBG
	" czFreqBandIndicator ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztFreqBandIndicator),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czFreqBandIndicatorspec),
	NULLP
};

PUBLIC CmPAsnElmDef *cz_ExtnGlobalENB_ID_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnGlobalENB_ID_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnGlobalENB_ID_ExtIEsClsInfo =
{
	0,
	&cz_ExtnGlobalENB_ID_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnGlobalENB_ID_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnGlobalENB_ID_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnGlobalENB_ID_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnGlobalENB_ID_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnGlobalENB_ID_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnGlobalENB_ID_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnGlobalENB_ID_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_GlobalENB_ID_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnGlobalENB_ID_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_GlobalENB_ID_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_GlobalENB_ID_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_GlobalENB_ID_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_GlobalENB_ID_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_GlobalENB_ID_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_GlobalENB_ID_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_GlobalENB_ID_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_GlobalENB_ID_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_GlobalENB_ID_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_GlobalENB_ID_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_GlobalENB_ID_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_GlobalENB_ID_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_GlobalENB_ID_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_GlobalENB_ID_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_GlobalENB_ID_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czGlobalENB_IDTkn[] =
{
	&czPLMN_Identity,
	&czENB_ID,
	&czOptProtExtnCont_GlobalENB_ID_ExtIEs,
		&czProtExtnField_GlobalENB_ID_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czGlobalENB_IDspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czGlobalENB_IDTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czGlobalENB_ID =
{
#ifdef CM_PASN_DBG
	" czGlobalENB_ID ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztGlobalENB_ID),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czGlobalENB_IDspec),
	NULLP
};
CmPAsnBitStrDef czTportLyrAddrspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTRXL,
#endif /* CM_PASN_DEBUG */
	1,
	160
};
CmPAsnElmDef czTportLyrAddr =
{
#ifdef CM_PASN_DBG
	" czTportLyrAddr ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTRXL,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztTportLyrAddr),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czTportLyrAddrspec),
	NULLP
};
CmPAsnOctStrDef czGTP_TEIspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_OCTSTRXL,
#endif /* CM_PASN_DEBUG */
	4,
	4
};
CmPAsnElmDef czGTP_TEI =
{
#ifdef CM_PASN_DBG
	" czGTP_TEI " ,
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_OCTSTRXL,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztGTP_TEI),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czGTP_TEIspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnGTPtunnelEndpoint_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnGTPtunnelEndpoint_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnGTPtunnelEndpoint_ExtIEsClsInfo =
{
	0,
	&cz_ExtnGTPtunnelEndpoint_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnGTPtunnelEndpoint_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnGTPtunnelEndpoint_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnGTPtunnelEndpoint_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnGTPtunnelEndpoint_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnGTPtunnelEndpoint_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnGTPtunnelEndpoint_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnGTPtunnelEndpoint_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_GTPtunnelEndpoint_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnGTPtunnelEndpoint_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_GTPtunnelEndpoint_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_GTPtunnelEndpoint_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_GTPtunnelEndpoint_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_GTPtunnelEndpoint_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_GTPtunnelEndpoint_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_GTPtunnelEndpoint_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_GTPtunnelEndpoint_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_GTPtunnelEndpoint_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_GTPtunnelEndpoint_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_GTPtunnelEndpoint_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_GTPtunnelEndpoint_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_GTPtunnelEndpoint_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_GTPtunnelEndpoint_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_GTPtunnelEndpoint_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_GTPtunnelEndpoint_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czGTPtunnelEndpointTkn[] =
{
	&czTportLyrAddr,
	&czGTP_TEI,
	&czOptProtExtnCont_GTPtunnelEndpoint_ExtIEs,
		&czProtExtnField_GTPtunnelEndpoint_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czGTPtunnelEndpointspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czGTPtunnelEndpointTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czGTPtunnelEndpoint =
{
#ifdef CM_PASN_DBG
	" czGTPtunnelEndpoint ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztGTPtunnelEndpoint),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czGTPtunnelEndpointspec),
	NULLP
};
PUBLIC CmPAsnElmDef czOptGTPtunnelEndpoint =
{
#ifdef CM_PASN_DBG
	" czGTPtunnelEndpoint ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztGTPtunnelEndpoint),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czGTPtunnelEndpointspec),
	NULLP
};
CmPAsnOctStrDef czMME_Group_IDspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_OCTSTR,
#endif /* CM_PASN_DEBUG */
	2,
	2
};
CmPAsnElmDef czMME_Group_ID =
{
#ifdef CM_PASN_DBG
	" czMME_Group_ID " ,
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_OCTSTR,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztMME_Group_ID),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czMME_Group_IDspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnGU_Group_ID_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnGU_Group_ID_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnGU_Group_ID_ExtIEsClsInfo =
{
	0,
	&cz_ExtnGU_Group_ID_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnGU_Group_ID_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnGU_Group_ID_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnGU_Group_ID_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnGU_Group_ID_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnGU_Group_ID_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnGU_Group_ID_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnGU_Group_ID_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_GU_Group_ID_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnGU_Group_ID_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_GU_Group_ID_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_GU_Group_ID_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_GU_Group_ID_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_GU_Group_ID_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_GU_Group_ID_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_GU_Group_ID_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_GU_Group_ID_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_GU_Group_ID_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_GU_Group_ID_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_GU_Group_ID_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_GU_Group_ID_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_GU_Group_ID_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_GU_Group_ID_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_GU_Group_ID_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_GU_Group_ID_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czGU_Group_IDTkn[] =
{
	&czPLMN_Identity,
	&czMME_Group_ID,
	&czOptProtExtnCont_GU_Group_ID_ExtIEs,
		&czProtExtnField_GU_Group_ID_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czGU_Group_IDspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czGU_Group_IDTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czGU_Group_ID =
{
#ifdef CM_PASN_DBG
	" czGU_Group_ID ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztGU_Group_ID),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czGU_Group_IDspec),
	NULLP
};
CmPAsnSetSeqOfDef czGUGroupIDLstspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxPools,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czGUGroupIDLst =
{
#ifdef CM_PASN_DBG
	" czGUGroupIDLst ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztGUGroupIDLst),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czGUGroupIDLstspec),
	NULLP
};
CmPAsnOctStrDef czMME_Codespec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_OCTSTR,
#endif /* CM_PASN_DEBUG */
	1,
	1
};
CmPAsnElmDef czMME_Code =
{
#ifdef CM_PASN_DBG
	" czMME_Code " ,
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_OCTSTR,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztMME_Code),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czMME_Codespec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnGUMMEI_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnGUMMEI_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnGUMMEI_ExtIEsClsInfo =
{
	0,
	&cz_ExtnGUMMEI_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnGUMMEI_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnGUMMEI_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnGUMMEI_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnGUMMEI_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnGUMMEI_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnGUMMEI_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnGUMMEI_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_GUMMEI_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnGUMMEI_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_GUMMEI_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_GUMMEI_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_GUMMEI_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_GUMMEI_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_GUMMEI_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_GUMMEI_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_GUMMEI_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_GUMMEI_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_GUMMEI_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_GUMMEI_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_GUMMEI_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_GUMMEI_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_GUMMEI_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_GUMMEI_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_GUMMEI_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czGUMMEITkn[] =
{
	&czGU_Group_ID,
	&czMME_Code,
	&czOptProtExtnCont_GUMMEI_ExtIEs,
		&czProtExtnField_GUMMEI_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czGUMMEIspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czGUMMEITkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czGUMMEI =
{
#ifdef CM_PASN_DBG
	" czGUMMEI ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztGUMMEI),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czGUMMEIspec),
	NULLP
};
/* Element ----- HandoverReportType ------- */
CmPAsnTknEnum czHovrReportTypExtLst[2] =
{
	1,
	CztHovrReportTypinterRATpingpongEnum
};
CmPAsnTknEnum czHovrReportTypLst[3] =
{
	2,
	CztHovrReportTyphoTooEarlyEnum,
	CztHovrReportTyphoToWrongCellEnum
};
CmPAsnEnumDef czHovrReportTypspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czHovrReportTypLst,
	czHovrReportTypExtLst
};
CmPAsnElmDef czHovrReportTyp =
{
#ifdef CM_PASN_DBG
	" czHovrReportTyp ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztHovrReportTyp),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czHovrReportTypspec),
	NULLP
};

PUBLIC CmPAsnElmDef *cz_ExtnHovrRestrnLst_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnHovrRestrnLst_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnHovrRestrnLst_ExtIEsClsInfo =
{
	0,
	&cz_ExtnHovrRestrnLst_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnHovrRestrnLst_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnHovrRestrnLst_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnHovrRestrnLst_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnHovrRestrnLst_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnHovrRestrnLst_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnHovrRestrnLst_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnHovrRestrnLst_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_HovrRestrnLst_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnHovrRestrnLst_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_HovrRestrnLst_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_HovrRestrnLst_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_HovrRestrnLst_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_HovrRestrnLst_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_HovrRestrnLst_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_HovrRestrnLst_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_HovrRestrnLst_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_HovrRestrnLst_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_HovrRestrnLst_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_HovrRestrnLst_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_HovrRestrnLst_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_HovrRestrnLst_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_HovrRestrnLst_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_HovrRestrnLst_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_HovrRestrnLst_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czHovrRestrnLstTkn[] =
{
	&czPLMN_Identity,
	&czOptEPLMNs,
		&czPLMN_Identity,
		&czConstTerm,
	&czOptForbiddenTAs,
		&czForbiddenTAs_Item,
		&czConstTerm,
	&czOptForbiddenLAs,
		&czForbiddenLAs_Item,
		&czConstTerm,
	&czOptForbiddenInterRATs,
	&czOptProtExtnCont_HovrRestrnLst_ExtIEs,
		&czProtExtnField_HovrRestrnLst_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czHovrRestrnLstspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	5,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czHovrRestrnLstTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czHovrRestrnLst =
{
#ifdef CM_PASN_DBG
	" czHovrRestrnLst ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztHovrRestrnLst),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czHovrRestrnLstspec),
	NULLP
};
PUBLIC CmPAsnElmDef czOptHovrRestrnLst =
{
#ifdef CM_PASN_DBG
	" czHovrRestrnLst ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztHovrRestrnLst),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czHovrRestrnLstspec),
	NULLP
};
/* Element ----- LoadIndicator ------- */
CmPAsnTknEnum czLoadIndicatorLst[5] =
{
	4,
	CztLoadIndicatorlowLoadEnum,
	CztLoadIndicatormediumLoadEnum,
	CztLoadIndicatorhighLoadEnum,
	CztLoadIndicatoroverLoadEnum
};
CmPAsnEnumDef czLoadIndicatorspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czLoadIndicatorLst,
	NULLP
};
CmPAsnElmDef czLoadIndicator =
{
#ifdef CM_PASN_DBG
	" czLoadIndicator ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztLoadIndicator),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czLoadIndicatorspec),
	NULLP
};

PUBLIC CmPAsnElmDef *cz_ExtnHWLoadIndicator_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnHWLoadIndicator_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnHWLoadIndicator_ExtIEsClsInfo =
{
	0,
	&cz_ExtnHWLoadIndicator_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnHWLoadIndicator_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnHWLoadIndicator_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnHWLoadIndicator_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnHWLoadIndicator_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnHWLoadIndicator_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnHWLoadIndicator_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnHWLoadIndicator_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_HWLoadIndicator_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnHWLoadIndicator_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_HWLoadIndicator_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_HWLoadIndicator_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_HWLoadIndicator_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_HWLoadIndicator_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_HWLoadIndicator_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_HWLoadIndicator_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_HWLoadIndicator_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_HWLoadIndicator_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_HWLoadIndicator_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_HWLoadIndicator_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_HWLoadIndicator_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_HWLoadIndicator_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_HWLoadIndicator_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_HWLoadIndicator_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_HWLoadIndicator_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czHWLoadIndicatorTkn[] =
{
	&czLoadIndicator,
	&czLoadIndicator,
	&czOptProtExtnCont_HWLoadIndicator_ExtIEs,
		&czProtExtnField_HWLoadIndicator_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czHWLoadIndicatorspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czHWLoadIndicatorTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czHWLoadIndicator =
{
#ifdef CM_PASN_DBG
	" czHWLoadIndicator ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztHWLoadIndicator),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czHWLoadIndicatorspec),
	NULLP
};
PUBLIC CmPAsnElmDef czOptHWLoadIndicator =
{
#ifdef CM_PASN_DBG
	" czHWLoadIndicator ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztHWLoadIndicator),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czHWLoadIndicatorspec),
	NULLP
};
/* Element ----- InvokeIndication ------- */
CmPAsnTknEnum czInvokeIndLst[2] =
{
	1,
	CztInvokeIndabs_informationEnum
};
CmPAsnEnumDef czInvokeIndspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czInvokeIndLst,
	NULLP
};
CmPAsnElmDef czInvokeInd =
{
#ifdef CM_PASN_DBG
	" czInvokeInd ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztInvokeInd),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czInvokeIndspec),
	NULLP
};

CmPAsnBitStrDef czIntegrityProtectionAlgorithmsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTR,
#endif /* CM_PASN_DEBUG */
	16,
	16
};
CmPAsnElmDef czIntegrityProtectionAlgorithms =
{
#ifdef CM_PASN_DBG
	" czIntegrityProtectionAlgorithms ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTR,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztIntegrityProtectionAlgorithms),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czIntegrityProtectionAlgorithmsspec),
	NULLP
};
CmPAsnBitStrDef czIntfsToTracespec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTR,
#endif /* CM_PASN_DEBUG */
	8,
	8
};
CmPAsnElmDef czIntfsToTrace =
{
#ifdef CM_PASN_DBG
	" czIntfsToTrace ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTR,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztIntfsToTrace),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czIntfsToTracespec),
	NULLP
};
/* Element ----- Time-UE-StayedInCell ------- */
CmPAsnUIntDef czTime_UE_StayedInCellspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	4095UL
};
CmPAsnElmDef czTime_UE_StayedInCell =
{
#ifdef CM_PASN_DBG
	" czTime_UE_StayedInCell ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztTime_UE_StayedInCell),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czTime_UE_StayedInCellspec),
	NULLP
};

/* Element ----- Time-UE-StayedInCell-EnhancedGranularity ------- */
CmPAsnUIntDef czTime_UE_StayedInCell_EnhancedGranularityspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	40950UL
};
CmPAsnElmDef czTime_UE_StayedInCell_EnhancedGranularity =
{
#ifdef CM_PASN_DBG
	" czTime_UE_StayedInCell_EnhancedGranularity ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztTime_UE_StayedInCell_EnhancedGranularity),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czTime_UE_StayedInCell_EnhancedGranularityspec),
	NULLP
};

PUBLIC CmPAsnElmDef *cz_ExtnLastVisitedEUTRANCellInform_ExtIEsClsInst[] =
{
	&czTime_UE_StayedInCell_EnhancedGranularity,
	&czCause
};
PUBLIC U32 cz_ExtnLastVisitedEUTRANCellInform_ExtIEsClsInstId[] =
{
	Cztid_Time_UE_StayedInCell_EnhancedGranularity,
	Cztid_HO_cause
};
PUBLIC CmPAsnClassInfo cz_ExtnLastVisitedEUTRANCellInform_ExtIEsClsInfo =
{
	2,
	&cz_ExtnLastVisitedEUTRANCellInform_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnLastVisitedEUTRANCellInform_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnLastVisitedEUTRANCellInform_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnLastVisitedEUTRANCellInform_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnLastVisitedEUTRANCellInform_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnLastVisitedEUTRANCellInform_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnLastVisitedEUTRANCellInform_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnLastVisitedEUTRANCellInform_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_LastVisitedEUTRANCellInform_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnLastVisitedEUTRANCellInform_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_LastVisitedEUTRANCellInform_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_LastVisitedEUTRANCellInform_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_LastVisitedEUTRANCellInform_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_LastVisitedEUTRANCellInform_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_LastVisitedEUTRANCellInform_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_LastVisitedEUTRANCellInform_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_LastVisitedEUTRANCellInform_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_LastVisitedEUTRANCellInform_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_LastVisitedEUTRANCellInform_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_LastVisitedEUTRANCellInform_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_LastVisitedEUTRANCellInform_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_LastVisitedEUTRANCellInform_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_LastVisitedEUTRANCellInform_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_LastVisitedEUTRANCellInform_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_LastVisitedEUTRANCellInform_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czLastVisitedEUTRANCellInformTkn[] =
{
	&czECGI,
	&czCellTyp,
	&czTime_UE_StayedInCell,
	&czOptProtExtnCont_LastVisitedEUTRANCellInform_ExtIEs,
		&czProtExtnField_LastVisitedEUTRANCellInform_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czLastVisitedEUTRANCellInformspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czLastVisitedEUTRANCellInformTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czLastVisitedEUTRANCellInform =
{
#ifdef CM_PASN_DBG
	" czLastVisitedEUTRANCellInform ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztLastVisitedEUTRANCellInform),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czLastVisitedEUTRANCellInformspec),
	NULLP
};
CmPAsnOctStrDef czLastVisitedUTRANCellInformspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_OCTSTRXL,
#endif /* CM_PASN_DEBUG */
	0,
	CM_PASN_PLUS_INF
};
CmPAsnElmDef czLastVisitedUTRANCellInform =
{
#ifdef CM_PASN_DBG
	" czLastVisitedUTRANCellInform " ,
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_OCTSTRXL,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztLastVisitedUTRANCellInform),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czLastVisitedUTRANCellInformspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czLastVisitedGERANCellInformTkn[] =
{
	&czNull,
	&czExtMkr,
	&czConstTerm
};
CmPAsnChoiceDef czLastVisitedGERANCellInformspec = 
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CHOICE,
#endif
	1,
	0,
	&czLastVisitedGERANCellInformTkn[0]  /* Token List */
};
CmPAsnElmDef czLastVisitedGERANCellInform =
{
#ifdef CM_PASN_DBG
	" czLastVisitedGERANCellInform ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CHOICE,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztLastVisitedGERANCellInform),
	&flagCz3M,
	NULLP,
	(U8 *)(&czLastVisitedGERANCellInformspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czLastVisitedCell_ItemTkn[] =
{
	&czLastVisitedEUTRANCellInform,
	&czLastVisitedUTRANCellInform,
	&czLastVisitedGERANCellInform,
	&czExtMkr,
	&czConstTerm
};
CmPAsnChoiceDef czLastVisitedCell_Itemspec = 
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CHOICE,
#endif
	3,
	0,
	&czLastVisitedCell_ItemTkn[0]  /* Token List */
};
CmPAsnElmDef czLastVisitedCell_Item =
{
#ifdef CM_PASN_DBG
	" czLastVisitedCell_Item ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CHOICE,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztLastVisitedCell_Item),
	&flagCz3M,
	NULLP,
	(U8 *)(&czLastVisitedCell_Itemspec),
	NULLP
};
/* Element ----- Links-to-log ------- */
CmPAsnTknEnum czLinks_to_logLst[4] =
{
	3,
	CztLinks_to_loguplinkEnum,
	CztLinks_to_logdownlinkEnum,
	CztLinks_to_logboth_uplink_and_downlinkEnum
};
CmPAsnEnumDef czLinks_to_logspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czLinks_to_logLst,
	NULLP
};
CmPAsnElmDef czLinks_to_log =
{
#ifdef CM_PASN_DBG
	" czLinks_to_log ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztLinks_to_log),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czLinks_to_logspec),
	NULLP
};

/* Element ----- ReportArea ------- */
CmPAsnTknEnum czReportAreaLst[2] =
{
	1,
	CztReportAreaecgiEnum
};
CmPAsnEnumDef czReportAreaspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czReportAreaLst,
	NULLP
};
CmPAsnElmDef czReportArea =
{
#ifdef CM_PASN_DBG
	" czReportArea ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztReportArea),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czReportAreaspec),
	NULLP
};

PUBLIC CmPAsnElmDef *cz_ExtnLocRprtngInform_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnLocRprtngInform_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnLocRprtngInform_ExtIEsClsInfo =
{
	0,
	&cz_ExtnLocRprtngInform_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnLocRprtngInform_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnLocRprtngInform_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnLocRprtngInform_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnLocRprtngInform_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnLocRprtngInform_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnLocRprtngInform_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnLocRprtngInform_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_LocRprtngInform_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnLocRprtngInform_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_LocRprtngInform_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_LocRprtngInform_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_LocRprtngInform_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_LocRprtngInform_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_LocRprtngInform_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_LocRprtngInform_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_LocRprtngInform_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_LocRprtngInform_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_LocRprtngInform_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_LocRprtngInform_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_LocRprtngInform_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_LocRprtngInform_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_LocRprtngInform_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_LocRprtngInform_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_LocRprtngInform_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czLocRprtngInformTkn[] =
{
	&czEventTyp,
	&czReportArea,
	&czOptProtExtnCont_LocRprtngInform_ExtIEs,
		&czProtExtnField_LocRprtngInform_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czLocRprtngInformspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czLocRprtngInformTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czLocRprtngInform =
{
#ifdef CM_PASN_DBG
	" czLocRprtngInform ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztLocRprtngInform),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czLocRprtngInformspec),
	NULLP
};
PUBLIC CmPAsnElmDef czOptLocRprtngInform =
{
#ifdef CM_PASN_DBG
	" czLocRprtngInform ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztLocRprtngInform),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czLocRprtngInformspec),
	NULLP
};
/* Element ----- M3period ------- */
CmPAsnTknEnum czM3periodLst[4] =
{
	3,
	CztM3periodms100Enum,
	CztM3periodms1000Enum,
	CztM3periodms10000Enum
};
CmPAsnEnumDef czM3periodspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czM3periodLst,
	NULLP
};
CmPAsnElmDef czM3period =
{
#ifdef CM_PASN_DBG
	" czM3period ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztM3period),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czM3periodspec),
	NULLP
};

PUBLIC CmPAsnElmDef *cz_ExtnM3Config_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnM3Config_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnM3Config_ExtIEsClsInfo =
{
	0,
	&cz_ExtnM3Config_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnM3Config_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnM3Config_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnM3Config_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnM3Config_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnM3Config_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnM3Config_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnM3Config_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_M3Config_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnM3Config_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_M3Config_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_M3Config_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_M3Config_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_M3Config_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_M3Config_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_M3Config_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_M3Config_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_M3Config_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_M3Config_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_M3Config_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_M3Config_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_M3Config_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_M3Config_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_M3Config_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_M3Config_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czM3ConfigTkn[] =
{
	&czM3period,
	&czOptProtExtnCont_M3Config_ExtIEs,
		&czProtExtnField_M3Config_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czM3Configspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czM3ConfigTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czM3Config =
{
#ifdef CM_PASN_DBG
	" czM3Config ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztM3Config),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czM3Configspec),
	NULLP
};
/* Element ----- M4period ------- */
CmPAsnTknEnum czM4periodLst[6] =
{
	5,
	CztM4periodms1024Enum,
	CztM4periodms2048Enum,
	CztM4periodms5120Enum,
	CztM4periodms10240Enum,
	CztM4periodmin1Enum
};
CmPAsnEnumDef czM4periodspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czM4periodLst,
	NULLP
};
CmPAsnElmDef czM4period =
{
#ifdef CM_PASN_DBG
	" czM4period ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztM4period),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czM4periodspec),
	NULLP
};

PUBLIC CmPAsnElmDef *cz_ExtnM4Config_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnM4Config_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnM4Config_ExtIEsClsInfo =
{
	0,
	&cz_ExtnM4Config_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnM4Config_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnM4Config_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnM4Config_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnM4Config_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnM4Config_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnM4Config_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnM4Config_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_M4Config_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnM4Config_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_M4Config_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_M4Config_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_M4Config_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_M4Config_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_M4Config_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_M4Config_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_M4Config_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_M4Config_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_M4Config_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_M4Config_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_M4Config_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_M4Config_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_M4Config_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_M4Config_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_M4Config_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czM4ConfigTkn[] =
{
	&czM4period,
	&czLinks_to_log,
	&czOptProtExtnCont_M4Config_ExtIEs,
		&czProtExtnField_M4Config_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czM4Configspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czM4ConfigTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czM4Config =
{
#ifdef CM_PASN_DBG
	" czM4Config ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztM4Config),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czM4Configspec),
	NULLP
};
/* Element ----- M5period ------- */
CmPAsnTknEnum czM5periodLst[6] =
{
	5,
	CztM5periodms1024Enum,
	CztM5periodms2048Enum,
	CztM5periodms5120Enum,
	CztM5periodms10240Enum,
	CztM5periodmin1Enum
};
CmPAsnEnumDef czM5periodspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czM5periodLst,
	NULLP
};
CmPAsnElmDef czM5period =
{
#ifdef CM_PASN_DBG
	" czM5period ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztM5period),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czM5periodspec),
	NULLP
};

PUBLIC CmPAsnElmDef *cz_ExtnM5Config_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnM5Config_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnM5Config_ExtIEsClsInfo =
{
	0,
	&cz_ExtnM5Config_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnM5Config_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnM5Config_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnM5Config_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnM5Config_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnM5Config_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnM5Config_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnM5Config_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_M5Config_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnM5Config_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_M5Config_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_M5Config_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_M5Config_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_M5Config_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_M5Config_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_M5Config_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_M5Config_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_M5Config_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_M5Config_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_M5Config_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_M5Config_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_M5Config_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_M5Config_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_M5Config_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_M5Config_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czM5ConfigTkn[] =
{
	&czM5period,
	&czLinks_to_log,
	&czOptProtExtnCont_M5Config_ExtIEs,
		&czProtExtnField_M5Config_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czM5Configspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czM5ConfigTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czM5Config =
{
#ifdef CM_PASN_DBG
	" czM5Config ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztM5Config),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czM5Configspec),
	NULLP
};
/* Element ----- MDT-Activation ------- */
CmPAsnTknEnum czMDT_ActvnLst[3] =
{
	2,
	CztMDT_Actvnimmediate_MDT_onlyEnum,
	CztMDT_Actvnimmediate_MDT_and_TraceEnum
};
CmPAsnEnumDef czMDT_Actvnspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czMDT_ActvnLst,
	NULLP
};
CmPAsnElmDef czMDT_Actvn =
{
#ifdef CM_PASN_DBG
	" czMDT_Actvn ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztMDT_Actvn),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czMDT_Actvnspec),
	NULLP
};

CmPAsnBitStrDef czMeasurementsToActivatespec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTR,
#endif /* CM_PASN_DEBUG */
	8,
	8
};
CmPAsnElmDef czMeasurementsToActivate =
{
#ifdef CM_PASN_DBG
	" czMeasurementsToActivate ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTR,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztMeasurementsToActivate),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czMeasurementsToActivatespec),
	NULLP
};
/* Element ----- M1ReportingTrigger ------- */
CmPAsnTknEnum czM1RprtngTriggerExtLst[2] =
{
	1,
	CztM1RprtngTriggera2eventtriggered_periodicEnum
};
CmPAsnTknEnum czM1RprtngTriggerLst[3] =
{
	2,
	CztM1RprtngTriggerperiodicEnum,
	CztM1RprtngTriggera2eventtriggeredEnum
};
CmPAsnEnumDef czM1RprtngTriggerspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czM1RprtngTriggerLst,
	czM1RprtngTriggerExtLst
};
CmPAsnElmDef czM1RprtngTrigger =
{
#ifdef CM_PASN_DBG
	" czM1RprtngTrigger ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztM1RprtngTrigger),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czM1RprtngTriggerspec),
	NULLP
};

/* Element ----- Threshold-RSRP ------- */
CmPAsnUIntDef czThreshold_RSRPspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	97UL
};
CmPAsnElmDef czThreshold_RSRP =
{
#ifdef CM_PASN_DBG
	" czThreshold_RSRP ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztThreshold_RSRP),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czThreshold_RSRPspec),
	NULLP
};

/* Element ----- Threshold-RSRQ ------- */
CmPAsnUIntDef czThreshold_RSRQspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	34UL
};
CmPAsnElmDef czThreshold_RSRQ =
{
#ifdef CM_PASN_DBG
	" czThreshold_RSRQ ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztThreshold_RSRQ),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czThreshold_RSRQspec),
	NULLP
};

PUBLIC CmPAsnElmDef *czMeasurementThresholdA2Tkn[] =
{
	&czThreshold_RSRP,
	&czThreshold_RSRQ,
	&czExtMkr,
	&czConstTerm
};
CmPAsnChoiceDef czMeasurementThresholdA2spec = 
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CHOICE,
#endif
	2,
	0,
	&czMeasurementThresholdA2Tkn[0]  /* Token List */
};
CmPAsnElmDef czMeasurementThresholdA2 =
{
#ifdef CM_PASN_DBG
	" czMeasurementThresholdA2 ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CHOICE,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztMeasurementThresholdA2),
	&flagCz3M,
	NULLP,
	(U8 *)(&czMeasurementThresholdA2spec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnM1ThresholdEventA2_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnM1ThresholdEventA2_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnM1ThresholdEventA2_ExtIEsClsInfo =
{
	0,
	&cz_ExtnM1ThresholdEventA2_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnM1ThresholdEventA2_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnM1ThresholdEventA2_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnM1ThresholdEventA2_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnM1ThresholdEventA2_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnM1ThresholdEventA2_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnM1ThresholdEventA2_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnM1ThresholdEventA2_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_M1ThresholdEventA2_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnM1ThresholdEventA2_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_M1ThresholdEventA2_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_M1ThresholdEventA2_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_M1ThresholdEventA2_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_M1ThresholdEventA2_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_M1ThresholdEventA2_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_M1ThresholdEventA2_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_M1ThresholdEventA2_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_M1ThresholdEventA2_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_M1ThresholdEventA2_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_M1ThresholdEventA2_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_M1ThresholdEventA2_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_M1ThresholdEventA2_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_M1ThresholdEventA2_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_M1ThresholdEventA2_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_M1ThresholdEventA2_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czM1ThresholdEventA2Tkn[] =
{
	&czMeasurementThresholdA2,
	&czOptProtExtnCont_M1ThresholdEventA2_ExtIEs,
		&czProtExtnField_M1ThresholdEventA2_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czM1ThresholdEventA2spec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czM1ThresholdEventA2Tkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czM1ThresholdEventA2 =
{
#ifdef CM_PASN_DBG
	" czM1ThresholdEventA2 ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztM1ThresholdEventA2),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czM1ThresholdEventA2spec),
	NULLP
};
PUBLIC CmPAsnElmDef czOptM1ThresholdEventA2 =
{
#ifdef CM_PASN_DBG
	" czM1ThresholdEventA2 ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztM1ThresholdEventA2),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czM1ThresholdEventA2spec),
	NULLP
};
/* Element ----- ReportIntervalMDT ------- */
CmPAsnTknEnum czReportIntervalMDTLst[14] =
{
	13,
	CztReportIntervalMDTms120Enum,
	CztReportIntervalMDTms240Enum,
	CztReportIntervalMDTms480Enum,
	CztReportIntervalMDTms640Enum,
	CztReportIntervalMDTms1024Enum,
	CztReportIntervalMDTms2048Enum,
	CztReportIntervalMDTms5120Enum,
	CztReportIntervalMDTms10240Enum,
	CztReportIntervalMDTmin1Enum,
	CztReportIntervalMDTmin6Enum,
	CztReportIntervalMDTmin12Enum,
	CztReportIntervalMDTmin30Enum,
	CztReportIntervalMDTmin60Enum
};
CmPAsnEnumDef czReportIntervalMDTspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czReportIntervalMDTLst,
	NULLP
};
CmPAsnElmDef czReportIntervalMDT =
{
#ifdef CM_PASN_DBG
	" czReportIntervalMDT ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztReportIntervalMDT),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czReportIntervalMDTspec),
	NULLP
};

/* Element ----- ReportAmountMDT ------- */
CmPAsnTknEnum czReportAmountMDTLst[9] =
{
	8,
	CztReportAmountMDTr1Enum,
	CztReportAmountMDTr2Enum,
	CztReportAmountMDTr4Enum,
	CztReportAmountMDTr8Enum,
	CztReportAmountMDTr16Enum,
	CztReportAmountMDTr32Enum,
	CztReportAmountMDTr64Enum,
	CztReportAmountMDTrinfinityEnum
};
CmPAsnEnumDef czReportAmountMDTspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czReportAmountMDTLst,
	NULLP
};
CmPAsnElmDef czReportAmountMDT =
{
#ifdef CM_PASN_DBG
	" czReportAmountMDT ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztReportAmountMDT),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czReportAmountMDTspec),
	NULLP
};

PUBLIC CmPAsnElmDef *cz_ExtnM1PeriodicRprtng_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnM1PeriodicRprtng_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnM1PeriodicRprtng_ExtIEsClsInfo =
{
	0,
	&cz_ExtnM1PeriodicRprtng_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnM1PeriodicRprtng_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnM1PeriodicRprtng_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnM1PeriodicRprtng_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnM1PeriodicRprtng_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnM1PeriodicRprtng_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnM1PeriodicRprtng_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnM1PeriodicRprtng_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_M1PeriodicRprtng_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnM1PeriodicRprtng_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_M1PeriodicRprtng_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_M1PeriodicRprtng_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_M1PeriodicRprtng_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_M1PeriodicRprtng_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_M1PeriodicRprtng_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_M1PeriodicRprtng_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_M1PeriodicRprtng_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_M1PeriodicRprtng_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_M1PeriodicRprtng_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_M1PeriodicRprtng_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_M1PeriodicRprtng_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_M1PeriodicRprtng_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_M1PeriodicRprtng_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_M1PeriodicRprtng_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_M1PeriodicRprtng_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czM1PeriodicRprtngTkn[] =
{
	&czReportIntervalMDT,
	&czReportAmountMDT,
	&czOptProtExtnCont_M1PeriodicRprtng_ExtIEs,
		&czProtExtnField_M1PeriodicRprtng_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czM1PeriodicRprtngspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czM1PeriodicRprtngTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czM1PeriodicRprtng =
{
#ifdef CM_PASN_DBG
	" czM1PeriodicRprtng ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztM1PeriodicRprtng),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czM1PeriodicRprtngspec),
	NULLP
};
PUBLIC CmPAsnElmDef czOptM1PeriodicRprtng =
{
#ifdef CM_PASN_DBG
	" czM1PeriodicRprtng ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztM1PeriodicRprtng),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czM1PeriodicRprtngspec),
	NULLP
};
CmPAsnBitStrDef czMDT_Loc_Infospec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTR,
#endif /* CM_PASN_DEBUG */
	8,
	8
};
CmPAsnElmDef czMDT_Loc_Info =
{
#ifdef CM_PASN_DBG
	" czMDT_Loc_Info ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTR,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztMDT_Loc_Info),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czMDT_Loc_Infospec),
	NULLP
};
CmPAsnSetSeqOfDef czMDTPLMNLstspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxnoofMDTPLMNs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czMDTPLMNLst =
{
#ifdef CM_PASN_DBG
	" czMDTPLMNLst ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztMDTPLMNLst),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czMDTPLMNLstspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnMDT_Config_ExtIEsClsInst[] =
{
	&czM3Config,
	&czM4Config,
	&czM5Config,
	&czMDT_Loc_Info,
	&czMDTPLMNLst,
		&czPLMN_Identity,
		&czConstTerm,
	
};
PUBLIC U32 cz_ExtnMDT_Config_ExtIEsClsInstId[] =
{
	Cztid_M3Config,
	Cztid_M4Config,
	Cztid_M5Config,
	Cztid_MDT_Loc_Info,
	Cztid_SignallingBasedMDTPLMNLst,
		Cztid_Invalid,
		Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnMDT_Config_ExtIEsClsInfo =
{
	7,
	&cz_ExtnMDT_Config_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnMDT_Config_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnMDT_Config_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnMDT_Config_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnMDT_Config_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnMDT_Config_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnMDT_Config_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnMDT_Config_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_MDT_Config_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnMDT_Config_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_MDT_Config_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_MDT_Config_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_MDT_Config_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_MDT_Config_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_MDT_Config_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_MDT_Config_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_MDT_Config_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_MDT_Config_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_MDT_Config_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_MDT_Config_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_MDT_Config_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_MDT_Config_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_MDT_Config_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_MDT_Config_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_MDT_Config_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czMDT_ConfigTkn[] =
{
	&czMDT_Actvn,
	&czAreaScopeOfMDT,
	&czMeasurementsToActivate,
	&czM1RprtngTrigger,
	&czOptM1ThresholdEventA2,
	&czOptM1PeriodicRprtng,
	&czOptProtExtnCont_MDT_Config_ExtIEs,
		&czProtExtnField_MDT_Config_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czMDT_Configspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	3,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czMDT_ConfigTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czMDT_Config =
{
#ifdef CM_PASN_DBG
	" czMDT_Config ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztMDT_Config),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czMDT_Configspec),
	NULLP
};
/* Element ----- Measurement-ID ------- */
CmPAsnUIntDef czMeasurement_IDspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	1,
	4095UL
};
CmPAsnElmDef czMeasurement_ID =
{
#ifdef CM_PASN_DBG
	" czMeasurement_ID ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztMeasurement_ID),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czMeasurement_IDspec),
	NULLP
};

CmPAsnOctStrDef czMBMS_Service_Area_Identityspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_OCTSTR,
#endif /* CM_PASN_DEBUG */
	2,
	2
};
CmPAsnElmDef czMBMS_Service_Area_Identity =
{
#ifdef CM_PASN_DBG
	" czMBMS_Service_Area_Identity " ,
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_OCTSTR,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztMBMS_Service_Area_Identity),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czMBMS_Service_Area_Identityspec),
	NULLP
};
CmPAsnSetSeqOfDef czMBMS_Service_Area_Identity_Lstspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxnoofMBMSServiceAreaIdentities,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czMBMS_Service_Area_Identity_Lst =
{
#ifdef CM_PASN_DBG
	" czMBMS_Service_Area_Identity_Lst ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztMBMS_Service_Area_Identity_Lst),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czMBMS_Service_Area_Identity_Lstspec),
	NULLP
};
/* Element ----- RadioframeAllocationPeriod ------- */
CmPAsnTknEnum czRadioframeAllocnPeriodLst[7] =
{
	6,
	CztRadioframeAllocnPeriodn1Enum,
	CztRadioframeAllocnPeriodn2Enum,
	CztRadioframeAllocnPeriodn4Enum,
	CztRadioframeAllocnPeriodn8Enum,
	CztRadioframeAllocnPeriodn16Enum,
	CztRadioframeAllocnPeriodn32Enum
};
CmPAsnEnumDef czRadioframeAllocnPeriodspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czRadioframeAllocnPeriodLst,
	NULLP
};
CmPAsnElmDef czRadioframeAllocnPeriod =
{
#ifdef CM_PASN_DBG
	" czRadioframeAllocnPeriod ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztRadioframeAllocnPeriod),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czRadioframeAllocnPeriodspec),
	NULLP
};

/* Element ----- RadioframeAllocationOffset ------- */
CmPAsnUIntDef czRadioframeAllocnOffsetspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	7UL
};
CmPAsnElmDef czRadioframeAllocnOffset =
{
#ifdef CM_PASN_DBG
	" czRadioframeAllocnOffset ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztRadioframeAllocnOffset),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czRadioframeAllocnOffsetspec),
	NULLP
};

CmPAsnBitStrDef czOneframespec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTR,
#endif /* CM_PASN_DEBUG */
	6,
	6
};
CmPAsnElmDef czOneframe =
{
#ifdef CM_PASN_DBG
	" czOneframe ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTR,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztOneframe),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czOneframespec),
	NULLP
};
PUBLIC CmPAsnElmDef *czSubframeAllocnTkn[] =
{
	&czOneframe,
	&czFourframes,
	&czExtMkr,
	&czConstTerm
};
CmPAsnChoiceDef czSubframeAllocnspec = 
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CHOICE,
#endif
	2,
	0,
	&czSubframeAllocnTkn[0]  /* Token List */
};
CmPAsnElmDef czSubframeAllocn =
{
#ifdef CM_PASN_DBG
	" czSubframeAllocn ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CHOICE,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztSubframeAllocn),
	&flagCz3M,
	NULLP,
	(U8 *)(&czSubframeAllocnspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnMBSFN_Subframe_Info_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnMBSFN_Subframe_Info_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnMBSFN_Subframe_Info_ExtIEsClsInfo =
{
	0,
	&cz_ExtnMBSFN_Subframe_Info_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnMBSFN_Subframe_Info_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnMBSFN_Subframe_Info_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnMBSFN_Subframe_Info_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnMBSFN_Subframe_Info_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnMBSFN_Subframe_Info_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnMBSFN_Subframe_Info_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnMBSFN_Subframe_Info_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_MBSFN_Subframe_Info_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnMBSFN_Subframe_Info_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_MBSFN_Subframe_Info_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_MBSFN_Subframe_Info_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_MBSFN_Subframe_Info_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_MBSFN_Subframe_Info_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_MBSFN_Subframe_Info_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_MBSFN_Subframe_Info_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_MBSFN_Subframe_Info_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_MBSFN_Subframe_Info_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_MBSFN_Subframe_Info_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_MBSFN_Subframe_Info_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_MBSFN_Subframe_Info_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_MBSFN_Subframe_Info_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_MBSFN_Subframe_Info_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_MBSFN_Subframe_Info_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_MBSFN_Subframe_Info_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czMBSFN_Subframe_InfoTkn[] =
{
	&czRadioframeAllocnPeriod,
	&czRadioframeAllocnOffset,
	&czSubframeAllocn,
	&czOptProtExtnCont_MBSFN_Subframe_Info_ExtIEs,
		&czProtExtnField_MBSFN_Subframe_Info_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czMBSFN_Subframe_Infospec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czMBSFN_Subframe_InfoTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czMBSFN_Subframe_Info =
{
#ifdef CM_PASN_DBG
	" czMBSFN_Subframe_Info ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztMBSFN_Subframe_Info),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czMBSFN_Subframe_Infospec),
	NULLP
};
CmPAsnSetSeqOfDef czMBSFN_Subframe_Infolistspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxnoofMBSFN,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czMBSFN_Subframe_Infolist =
{
#ifdef CM_PASN_DBG
	" czMBSFN_Subframe_Infolist ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztMBSFN_Subframe_Infolist),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czMBSFN_Subframe_Infolistspec),
	NULLP
};
/* Element ----- ManagementBasedMDTallowed ------- */
CmPAsnTknEnum czManagementBasedMDTallowedLst[2] =
{
	1,
	CztManagementBasedMDTallowedallowedEnum
};
CmPAsnEnumDef czManagementBasedMDTallowedspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czManagementBasedMDTallowedLst,
	NULLP
};
CmPAsnElmDef czManagementBasedMDTallowed =
{
#ifdef CM_PASN_DBG
	" czManagementBasedMDTallowed ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztManagementBasedMDTallowed),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czManagementBasedMDTallowedspec),
	NULLP
};

/* Element ----- handoverTriggerChangeLowerLimit ------- */
CmPAsnSIntDef czMobilityParamsModificationRangehandoverTriggerChangeLowerLimitspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SINT32,
#endif /* CM_PASN_DEBUG */
	-20,
	20UL
};
CmPAsnElmDef czMobilityParamsModificationRangehandoverTriggerChangeLowerLimit =
{
#ifdef CM_PASN_DBG
	" czMobilityParamsModificationRangehandoverTriggerChangeLowerLimit ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztMobilityParamsModificationRangehandoverTriggerChangeLowerLimit),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czMobilityParamsModificationRangehandoverTriggerChangeLowerLimitspec),
	NULLP
};

/* Element ----- handoverTriggerChangeUpperLimit ------- */
CmPAsnSIntDef czMobilityParamsModificationRangehandoverTriggerChangeUpperLimitspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SINT32,
#endif /* CM_PASN_DEBUG */
	-20,
	20UL
};
CmPAsnElmDef czMobilityParamsModificationRangehandoverTriggerChangeUpperLimit =
{
#ifdef CM_PASN_DBG
	" czMobilityParamsModificationRangehandoverTriggerChangeUpperLimit ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztMobilityParamsModificationRangehandoverTriggerChangeUpperLimit),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czMobilityParamsModificationRangehandoverTriggerChangeUpperLimitspec),
	NULLP
};

PUBLIC CmPAsnElmDef *czMobilityParamsModificationRangeTkn[] =
{
	&czMobilityParamsModificationRangehandoverTriggerChangeLowerLimit,
	&czMobilityParamsModificationRangehandoverTriggerChangeUpperLimit,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czMobilityParamsModificationRangespec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czMobilityParamsModificationRangeTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czMobilityParamsModificationRange =
{
#ifdef CM_PASN_DBG
	" czMobilityParamsModificationRange ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztMobilityParamsModificationRange),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czMobilityParamsModificationRangespec),
	NULLP
};
/* Element ----- handoverTriggerChange ------- */
CmPAsnSIntDef czMobilityParamsInformhandoverTriggerChangespec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SINT32,
#endif /* CM_PASN_DEBUG */
	-20,
	20UL
};
CmPAsnElmDef czMobilityParamsInformhandoverTriggerChange =
{
#ifdef CM_PASN_DBG
	" czMobilityParamsInformhandoverTriggerChange ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztMobilityParamsInformhandoverTriggerChange),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czMobilityParamsInformhandoverTriggerChangespec),
	NULLP
};

PUBLIC CmPAsnElmDef *czMobilityParamsInformTkn[] =
{
	&czMobilityParamsInformhandoverTriggerChange,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czMobilityParamsInformspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czMobilityParamsInformTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czMobilityParamsInform =
{
#ifdef CM_PASN_DBG
	" czMobilityParamsInform ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztMobilityParamsInform),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czMobilityParamsInformspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnBandInfo_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnBandInfo_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnBandInfo_ExtIEsClsInfo =
{
	0,
	&cz_ExtnBandInfo_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnBandInfo_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnBandInfo_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnBandInfo_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnBandInfo_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnBandInfo_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnBandInfo_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnBandInfo_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_BandInfo_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnBandInfo_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_BandInfo_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_BandInfo_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_BandInfo_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_BandInfo_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_BandInfo_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_BandInfo_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_BandInfo_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_BandInfo_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_BandInfo_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_BandInfo_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_BandInfo_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_BandInfo_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_BandInfo_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_BandInfo_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_BandInfo_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czBandInfoTkn[] =
{
	&czFreqBandIndicator,
	&czOptProtExtnCont_BandInfo_ExtIEs,
		&czProtExtnField_BandInfo_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czBandInfospec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czBandInfoTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czBandInfo =
{
#ifdef CM_PASN_DBG
	" czBandInfo ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztBandInfo),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czBandInfospec),
	NULLP
};
CmPAsnSetSeqOfDef czMultibandInfoLstspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxnoofBands,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czMultibandInfoLst =
{
#ifdef CM_PASN_DBG
	" czMultibandInfoLst ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztMultibandInfoLst),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czMultibandInfoLstspec),
	NULLP
};
/* Element ----- PCI ------- */
CmPAsnUIntDef czPCIspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	503UL
};
CmPAsnElmDef czPCI =
{
#ifdef CM_PASN_DBG
	" czPCI ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztPCI),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czPCIspec),
	NULLP
};

PUBLIC CmPAsnElmDef *cz_ExtnNeighbour_Inform_ExtIEsClsInst[] =
{
	&czTAC,
	&czEARFCNExtn
};
PUBLIC U32 cz_ExtnNeighbour_Inform_ExtIEsClsInstId[] =
{
	Cztid_NeighbourTAC,
	Cztid_eARFCNExtn
};
PUBLIC CmPAsnClassInfo cz_ExtnNeighbour_Inform_ExtIEsClsInfo =
{
	2,
	&cz_ExtnNeighbour_Inform_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnNeighbour_Inform_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnNeighbour_Inform_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnNeighbour_Inform_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnNeighbour_Inform_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnNeighbour_Inform_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnNeighbour_Inform_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnNeighbour_Inform_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_Neighbour_Inform_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnNeighbour_Inform_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_Neighbour_Inform_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_Neighbour_Inform_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_Neighbour_Inform_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_Neighbour_Inform_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_Neighbour_Inform_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_Neighbour_Inform_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_Neighbour_Inform_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_Neighbour_Inform_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_Neighbour_Inform_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_Neighbour_Inform_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_Neighbour_Inform_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_Neighbour_Inform_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_Neighbour_Inform_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_Neighbour_Inform_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_Neighbour_Inform_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czNeighbour_InformMemberTkn[] =
{
	&czECGI,
	&czPCI,
	&czEARFCN,
	&czOptProtExtnCont_Neighbour_Inform_ExtIEs,
		&czProtExtnField_Neighbour_Inform_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czNeighbour_InformMemberspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czNeighbour_InformMemberTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czNeighbour_InformMember =
{
#ifdef CM_PASN_DBG
	" czNeighbour_InformMember ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztNeighbour_InformMember),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czNeighbour_InformMemberspec),
	NULLP
};
CmPAsnSetSeqOfDef czNeighbour_Informspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxnoofNeighbours,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptNeighbour_Inform =
{
#ifdef CM_PASN_DBG
	" czNeighbour_Inform ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztNeighbour_Inform),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czNeighbour_Informspec),
	NULLP
};
/* Element ----- Number-of-Antennaports ------- */
CmPAsnTknEnum czNumber_of_AntennaportsLst[4] =
{
	3,
	CztNumber_of_Antennaportsan1Enum,
	CztNumber_of_Antennaportsan2Enum,
	CztNumber_of_Antennaportsan4Enum
};
CmPAsnEnumDef czNumber_of_Antennaportsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czNumber_of_AntennaportsLst,
	NULLP
};
CmPAsnElmDef czNumber_of_Antennaports =
{
#ifdef CM_PASN_DBG
	" czNumber_of_Antennaports ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztNumber_of_Antennaports),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czNumber_of_Antennaportsspec),
	NULLP
};

/* Element ----- rootSequenceIndex ------- */
CmPAsnUIntDef czPRACH_ConfigrootSequenceIdxspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	837UL
};
CmPAsnElmDef czPRACH_ConfigrootSequenceIdx =
{
#ifdef CM_PASN_DBG
	" czPRACH_ConfigrootSequenceIdx ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztPRACH_ConfigrootSequenceIdx),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czPRACH_ConfigrootSequenceIdxspec),
	NULLP
};

/* Element ----- zeroCorrelationIndex ------- */
CmPAsnUIntDef czPRACH_ConfigzeroCorrelationIdxspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	15UL
};
CmPAsnElmDef czPRACH_ConfigzeroCorrelationIdx =
{
#ifdef CM_PASN_DBG
	" czPRACH_ConfigzeroCorrelationIdx ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztPRACH_ConfigzeroCorrelationIdx),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czPRACH_ConfigzeroCorrelationIdxspec),
	NULLP
};

CmPAsnElmDef czPRACH_ConfighighSpeedFlag =
{
#ifdef CM_PASN_DBG
	" czPRACH_ConfighighSpeedFlag ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BOOL,
	CM_PASN_EXT_MKR_ABS,
	sizeof(TknU8),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	NULLP,
	(U8 *) NULLP
};

/* Element ----- prach-FreqOffset ------- */
CmPAsnUIntDef czPRACH_Configprach_FreqOffsetspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	94UL
};
CmPAsnElmDef czPRACH_Configprach_FreqOffset =
{
#ifdef CM_PASN_DBG
	" czPRACH_Configprach_FreqOffset ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztPRACH_Configprach_FreqOffset),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czPRACH_Configprach_FreqOffsetspec),
	NULLP
};

/* Element ----- prach-ConfigIndex ------- */
CmPAsnUIntDef czPRACH_Configprach_ConfigIdxspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	63UL
};
CmPAsnElmDef czOptPRACH_Configprach_ConfigIdx =
{
#ifdef CM_PASN_DBG
	" czPRACH_Configprach_ConfigIdx ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztPRACH_Configprach_ConfigIdx),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czPRACH_Configprach_ConfigIdxspec),
	NULLP
};

PUBLIC CmPAsnElmDef *cz_ExtnPRACH_Config_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnPRACH_Config_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnPRACH_Config_ExtIEsClsInfo =
{
	0,
	&cz_ExtnPRACH_Config_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnPRACH_Config_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnPRACH_Config_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnPRACH_Config_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnPRACH_Config_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnPRACH_Config_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnPRACH_Config_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnPRACH_Config_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_PRACH_Config_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnPRACH_Config_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_PRACH_Config_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_PRACH_Config_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_PRACH_Config_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_PRACH_Config_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_PRACH_Config_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_PRACH_Config_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_PRACH_Config_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_PRACH_Config_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_PRACH_Config_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_PRACH_Config_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_PRACH_Config_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_PRACH_Config_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_PRACH_Config_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_PRACH_Config_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_PRACH_Config_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czPRACH_ConfigTkn[] =
{
	&czPRACH_ConfigrootSequenceIdx,
	&czPRACH_ConfigzeroCorrelationIdx,
	&czPRACH_ConfighighSpeedFlag,
	&czPRACH_Configprach_FreqOffset,
	&czOptPRACH_Configprach_ConfigIdx,
	&czOptProtExtnCont_PRACH_Config_ExtIEs,
		&czProtExtnField_PRACH_Config_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czPRACH_Configspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	2,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czPRACH_ConfigTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czPRACH_Config =
{
#ifdef CM_PASN_DBG
	" czPRACH_Config ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztPRACH_Config),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czPRACH_Configspec),
	NULLP
};
/* Element ----- UL-GBR-PRB-usage ------- */
CmPAsnUIntDef czUL_GBR_PRB_usagespec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	100UL
};
CmPAsnElmDef czUL_GBR_PRB_usage =
{
#ifdef CM_PASN_DBG
	" czUL_GBR_PRB_usage ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztUL_GBR_PRB_usage),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czUL_GBR_PRB_usagespec),
	NULLP
};

/* Element ----- UL-non-GBR-PRB-usage ------- */
CmPAsnUIntDef czUL_non_GBR_PRB_usagespec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	100UL
};
CmPAsnElmDef czUL_non_GBR_PRB_usage =
{
#ifdef CM_PASN_DBG
	" czUL_non_GBR_PRB_usage ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztUL_non_GBR_PRB_usage),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czUL_non_GBR_PRB_usagespec),
	NULLP
};

/* Element ----- UL-Total-PRB-usage ------- */
CmPAsnUIntDef czUL_Total_PRB_usagespec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	100UL
};
CmPAsnElmDef czUL_Total_PRB_usage =
{
#ifdef CM_PASN_DBG
	" czUL_Total_PRB_usage ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztUL_Total_PRB_usage),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czUL_Total_PRB_usagespec),
	NULLP
};

PUBLIC CmPAsnElmDef *cz_ExtnRadioResStatus_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnRadioResStatus_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnRadioResStatus_ExtIEsClsInfo =
{
	0,
	&cz_ExtnRadioResStatus_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnRadioResStatus_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnRadioResStatus_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnRadioResStatus_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnRadioResStatus_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnRadioResStatus_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnRadioResStatus_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnRadioResStatus_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_RadioResStatus_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnRadioResStatus_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_RadioResStatus_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_RadioResStatus_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_RadioResStatus_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_RadioResStatus_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_RadioResStatus_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_RadioResStatus_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_RadioResStatus_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_RadioResStatus_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_RadioResStatus_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_RadioResStatus_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_RadioResStatus_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_RadioResStatus_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_RadioResStatus_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_RadioResStatus_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_RadioResStatus_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czRadioResStatusTkn[] =
{
	&czDL_GBR_PRB_usage,
	&czUL_GBR_PRB_usage,
	&czDL_non_GBR_PRB_usage,
	&czUL_non_GBR_PRB_usage,
	&czDL_Total_PRB_usage,
	&czUL_Total_PRB_usage,
	&czOptProtExtnCont_RadioResStatus_ExtIEs,
		&czProtExtnField_RadioResStatus_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czRadioResStatusspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czRadioResStatusTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czRadioResStatus =
{
#ifdef CM_PASN_DBG
	" czRadioResStatus ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztRadioResStatus),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czRadioResStatusspec),
	NULLP
};
PUBLIC CmPAsnElmDef czOptRadioResStatus =
{
#ifdef CM_PASN_DBG
	" czRadioResStatus ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztRadioResStatus),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czRadioResStatusspec),
	NULLP
};
CmPAsnBitStrDef czReceiveStatusofULPDCPSDUsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTRXL,
#endif /* CM_PASN_DEBUG */
	4096,
	4096
};
CmPAsnElmDef czOptReceiveStatusofULPDCPSDUs =
{
#ifdef CM_PASN_DBG
	" czReceiveStatusofULPDCPSDUs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTRXL,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztReceiveStatusofULPDCPSDUs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czReceiveStatusofULPDCPSDUsspec),
	NULLP
};
CmPAsnElmDef czReceiveStatusofULPDCPSDUs =
{
#ifdef CM_PASN_DBG
	" czReceiveStatusofULPDCPSDUs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTRXL,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztReceiveStatusofULPDCPSDUs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czReceiveStatusofULPDCPSDUsspec),
	NULLP
};
CmPAsnBitStrDef czReceiveStatusOfULPDCPSDUsExtendedspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTRXL,
#endif /* CM_PASN_DEBUG */
	1,
	16384
};
CmPAsnElmDef czReceiveStatusOfULPDCPSDUsExtended =
{
#ifdef CM_PASN_DBG
	" czReceiveStatusOfULPDCPSDUsExtended ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTRXL,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztReceiveStatusOfULPDCPSDUsExtended),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czReceiveStatusOfULPDCPSDUsExtendedspec),
	NULLP
};
/* Element ----- Registration-Request ------- */
CmPAsnTknEnum czRegistration_RqstLst[3] =
{
	2,
	CztRegistration_RqststartEnum,
	CztRegistration_RqststopEnum
};
CmPAsnEnumDef czRegistration_Rqstspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czRegistration_RqstLst,
	NULLP
};
CmPAsnElmDef czRegistration_Rqst =
{
#ifdef CM_PASN_DBG
	" czRegistration_Rqst ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztRegistration_Rqst),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czRegistration_Rqstspec),
	NULLP
};

/* Element ----- RNTP-Threshold ------- */
CmPAsnTknEnum czRNTP_ThresholdLst[17] =
{
	16,
	CztRNTP_ThresholdminusInfinityEnum,
	CztRNTP_ThresholdminusElevenEnum,
	CztRNTP_ThresholdminusTenEnum,
	CztRNTP_ThresholdminusNineEnum,
	CztRNTP_ThresholdminusEightEnum,
	CztRNTP_ThresholdminusSevenEnum,
	CztRNTP_ThresholdminusSixEnum,
	CztRNTP_ThresholdminusFiveEnum,
	CztRNTP_ThresholdminusFourEnum,
	CztRNTP_ThresholdminusThreeEnum,
	CztRNTP_ThresholdminusTwoEnum,
	CztRNTP_ThresholdminusOneEnum,
	CztRNTP_ThresholdzeroEnum,
	CztRNTP_ThresholdoneEnum,
	CztRNTP_ThresholdtwoEnum,
	CztRNTP_ThresholdthreeEnum
};
CmPAsnEnumDef czRNTP_Thresholdspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czRNTP_ThresholdLst,
	NULLP
};
CmPAsnElmDef czRNTP_Threshold =
{
#ifdef CM_PASN_DBG
	" czRNTP_Threshold ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztRNTP_Threshold),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czRNTP_Thresholdspec),
	NULLP
};

CmPAsnBitStrDef czRelativeNarrowbandTxPowerrNTP_PerPRBspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTRXL,
#endif /* CM_PASN_DEBUG */
	6,
	110
};
CmPAsnElmDef czRelativeNarrowbandTxPowerrNTP_PerPRB =
{
#ifdef CM_PASN_DBG
	" czRelativeNarrowbandTxPowerrNTP_PerPRB ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTRXL,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztRelativeNarrowbandTxPowerrNTP_PerPRB),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czRelativeNarrowbandTxPowerrNTP_PerPRBspec),
	NULLP
};
/* Element ----- numberOfCellSpecificAntennaPorts ------- */
CmPAsnTknEnum czRelativeNarrowbandTxPowernumberOfCellSpecificAntennaPortsLst[4] =
{
	3,
	CztRelativeNarrowbandTxPowernumberOfCellSpecificAntennaPortsoneEnum,
	CztRelativeNarrowbandTxPowernumberOfCellSpecificAntennaPortstwoEnum,
	CztRelativeNarrowbandTxPowernumberOfCellSpecificAntennaPortsfourEnum
};
CmPAsnEnumDef czRelativeNarrowbandTxPowernumberOfCellSpecificAntennaPortsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czRelativeNarrowbandTxPowernumberOfCellSpecificAntennaPortsLst,
	NULLP
};
CmPAsnElmDef czRelativeNarrowbandTxPowernumberOfCellSpecificAntennaPorts =
{
#ifdef CM_PASN_DBG
	" czRelativeNarrowbandTxPowernumberOfCellSpecificAntennaPorts ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztRelativeNarrowbandTxPowernumberOfCellSpecificAntennaPorts),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czRelativeNarrowbandTxPowernumberOfCellSpecificAntennaPortsspec),
	NULLP
};

/* Element ----- p-B ------- */
CmPAsnUIntDef czRelativeNarrowbandTxPowerp_Bspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	3UL
};
CmPAsnElmDef czRelativeNarrowbandTxPowerp_B =
{
#ifdef CM_PASN_DBG
	" czRelativeNarrowbandTxPowerp_B ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztRelativeNarrowbandTxPowerp_B),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czRelativeNarrowbandTxPowerp_Bspec),
	NULLP
};

/* Element ----- pDCCH-InterferenceImpact ------- */
CmPAsnUIntDef czRelativeNarrowbandTxPowerpDCCH_InterferenceImpactspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	4UL
};
CmPAsnElmDef czRelativeNarrowbandTxPowerpDCCH_InterferenceImpact =
{
#ifdef CM_PASN_DBG
	" czRelativeNarrowbandTxPowerpDCCH_InterferenceImpact ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztRelativeNarrowbandTxPowerpDCCH_InterferenceImpact),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czRelativeNarrowbandTxPowerpDCCH_InterferenceImpactspec),
	NULLP
};

PUBLIC CmPAsnElmDef *cz_ExtnRelativeNarrowbandTxPower_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnRelativeNarrowbandTxPower_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnRelativeNarrowbandTxPower_ExtIEsClsInfo =
{
	0,
	&cz_ExtnRelativeNarrowbandTxPower_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnRelativeNarrowbandTxPower_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnRelativeNarrowbandTxPower_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnRelativeNarrowbandTxPower_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnRelativeNarrowbandTxPower_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnRelativeNarrowbandTxPower_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnRelativeNarrowbandTxPower_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnRelativeNarrowbandTxPower_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_RelativeNarrowbandTxPower_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnRelativeNarrowbandTxPower_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_RelativeNarrowbandTxPower_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_RelativeNarrowbandTxPower_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_RelativeNarrowbandTxPower_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_RelativeNarrowbandTxPower_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_RelativeNarrowbandTxPower_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_RelativeNarrowbandTxPower_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_RelativeNarrowbandTxPower_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_RelativeNarrowbandTxPower_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_RelativeNarrowbandTxPower_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_RelativeNarrowbandTxPower_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_RelativeNarrowbandTxPower_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_RelativeNarrowbandTxPower_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_RelativeNarrowbandTxPower_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_RelativeNarrowbandTxPower_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_RelativeNarrowbandTxPower_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czRelativeNarrowbandTxPowerTkn[] =
{
	&czRelativeNarrowbandTxPowerrNTP_PerPRB,
	&czRNTP_Threshold,
	&czRelativeNarrowbandTxPowernumberOfCellSpecificAntennaPorts,
	&czRelativeNarrowbandTxPowerp_B,
	&czRelativeNarrowbandTxPowerpDCCH_InterferenceImpact,
	&czOptProtExtnCont_RelativeNarrowbandTxPower_ExtIEs,
		&czProtExtnField_RelativeNarrowbandTxPower_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czRelativeNarrowbandTxPowerspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czRelativeNarrowbandTxPowerTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czRelativeNarrowbandTxPower =
{
#ifdef CM_PASN_DBG
	" czRelativeNarrowbandTxPower ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztRelativeNarrowbandTxPower),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czRelativeNarrowbandTxPowerspec),
	NULLP
};
PUBLIC CmPAsnElmDef czOptRelativeNarrowbandTxPower =
{
#ifdef CM_PASN_DBG
	" czRelativeNarrowbandTxPower ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztRelativeNarrowbandTxPower),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czRelativeNarrowbandTxPowerspec),
	NULLP
};
CmPAsnBitStrDef czReportCharacteristicsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTR,
#endif /* CM_PASN_DEBUG */
	32,
	32
};
CmPAsnElmDef czReportCharacteristics =
{
#ifdef CM_PASN_DBG
	" czReportCharacteristics ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTR,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztReportCharacteristics),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czReportCharacteristicsspec),
	NULLP
};
CmPAsnOctStrDef czRRC_Cntxtspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_OCTSTRXL,
#endif /* CM_PASN_DEBUG */
	0,
	CM_PASN_PLUS_INF
};
CmPAsnElmDef czRRC_Cntxt =
{
#ifdef CM_PASN_DBG
	" czRRC_Cntxt " ,
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_OCTSTRXL,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztRRC_Cntxt),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czRRC_Cntxtspec),
	NULLP
};
/* Element ----- RRCConnReestabIndicator ------- */
CmPAsnTknEnum czRRCConnReestabIndicatorLst[4] =
{
	3,
	CztRRCConnReestabIndicatorreconfigurationFailEnum,
	CztRRCConnReestabIndicatorhandoverFailEnum,
	CztRRCConnReestabIndicatorotherFailEnum
};
CmPAsnEnumDef czRRCConnReestabIndicatorspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czRRCConnReestabIndicatorLst,
	NULLP
};
CmPAsnElmDef czRRCConnReestabIndicator =
{
#ifdef CM_PASN_DBG
	" czRRCConnReestabIndicator ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztRRCConnReestabIndicator),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czRRCConnReestabIndicatorspec),
	NULLP
};

/* Element ----- RRCConnSetupIndicator ------- */
CmPAsnTknEnum czRRCConnSetupIndicatorLst[2] =
{
	1,
	CztRRCConnSetupIndicatorrrcConnSetupEnum
};
CmPAsnEnumDef czRRCConnSetupIndicatorspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czRRCConnSetupIndicatorLst,
	NULLP
};
CmPAsnElmDef czRRCConnSetupIndicator =
{
#ifdef CM_PASN_DBG
	" czRRCConnSetupIndicator ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztRRCConnSetupIndicator),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czRRCConnSetupIndicatorspec),
	NULLP
};

PUBLIC CmPAsnElmDef *cz_ExtnS1TNLLoadIndicator_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnS1TNLLoadIndicator_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnS1TNLLoadIndicator_ExtIEsClsInfo =
{
	0,
	&cz_ExtnS1TNLLoadIndicator_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnS1TNLLoadIndicator_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnS1TNLLoadIndicator_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnS1TNLLoadIndicator_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnS1TNLLoadIndicator_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnS1TNLLoadIndicator_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnS1TNLLoadIndicator_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnS1TNLLoadIndicator_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_S1TNLLoadIndicator_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnS1TNLLoadIndicator_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_S1TNLLoadIndicator_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_S1TNLLoadIndicator_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_S1TNLLoadIndicator_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_S1TNLLoadIndicator_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_S1TNLLoadIndicator_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_S1TNLLoadIndicator_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_S1TNLLoadIndicator_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_S1TNLLoadIndicator_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_S1TNLLoadIndicator_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_S1TNLLoadIndicator_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_S1TNLLoadIndicator_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_S1TNLLoadIndicator_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_S1TNLLoadIndicator_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_S1TNLLoadIndicator_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_S1TNLLoadIndicator_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czS1TNLLoadIndicatorTkn[] =
{
	&czLoadIndicator,
	&czLoadIndicator,
	&czOptProtExtnCont_S1TNLLoadIndicator_ExtIEs,
		&czProtExtnField_S1TNLLoadIndicator_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czS1TNLLoadIndicatorspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czS1TNLLoadIndicatorTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czS1TNLLoadIndicator =
{
#ifdef CM_PASN_DBG
	" czS1TNLLoadIndicator ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztS1TNLLoadIndicator),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czS1TNLLoadIndicatorspec),
	NULLP
};
PUBLIC CmPAsnElmDef czOptS1TNLLoadIndicator =
{
#ifdef CM_PASN_DBG
	" czS1TNLLoadIndicator ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztS1TNLLoadIndicator),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czS1TNLLoadIndicatorspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnSrvdCell_Inform_ExtIEsClsInst[] =
{
	&czNumber_of_Antennaports,
	&czPRACH_Config,
	&czMBSFN_Subframe_Infolist,
		&czMBSFN_Subframe_Info,
		&czConstTerm,
	&czCSG_Id,
	&czMBMS_Service_Area_Identity_Lst,
		&czMBMS_Service_Area_Identity,
		&czConstTerm,
	&czMultibandInfoLst,
		&czBandInfo,
		&czConstTerm,
	
};
PUBLIC U32 cz_ExtnSrvdCell_Inform_ExtIEsClsInstId[] =
{
	Cztid_Number_of_Antennaports,
	Cztid_PRACH_Config,
	Cztid_MBSFN_Subframe_Info,
		Cztid_Invalid,
		Cztid_Invalid,
	Cztid_CSG_Id,
	Cztid_MBMS_Service_Area_Lst,
		Cztid_Invalid,
		Cztid_Invalid,
	Cztid_MultibandInfoLst,
		Cztid_Invalid,
		Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnSrvdCell_Inform_ExtIEsClsInfo =
{
	12,
	&cz_ExtnSrvdCell_Inform_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnSrvdCell_Inform_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnSrvdCell_Inform_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnSrvdCell_Inform_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnSrvdCell_Inform_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnSrvdCell_Inform_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnSrvdCell_Inform_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnSrvdCell_Inform_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_SrvdCell_Inform_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnSrvdCell_Inform_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_SrvdCell_Inform_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_SrvdCell_Inform_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_SrvdCell_Inform_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_SrvdCell_Inform_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_SrvdCell_Inform_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_SrvdCell_Inform_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_SrvdCell_Inform_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_SrvdCell_Inform_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_SrvdCell_Inform_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_SrvdCell_Inform_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_SrvdCell_Inform_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_SrvdCell_Inform_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_SrvdCell_Inform_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_SrvdCell_Inform_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_SrvdCell_Inform_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czSrvdCell_InformTkn[] =
{
	&czPCI,
	&czECGI,
	&czTAC,
	&czBroadcastPLMNs_Item,
		&czPLMN_Identity,
		&czConstTerm,
	&czEUTRA_Mode_Info,
	&czOptProtExtnCont_SrvdCell_Inform_ExtIEs,
		&czProtExtnField_SrvdCell_Inform_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czSrvdCell_Informspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czSrvdCell_InformTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czSrvdCell_Inform =
{
#ifdef CM_PASN_DBG
	" czSrvdCell_Inform ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztSrvdCell_Inform),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czSrvdCell_Informspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnSrvdCell_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnSrvdCell_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnSrvdCell_ExtIEsClsInfo =
{
	0,
	&cz_ExtnSrvdCell_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnSrvdCell_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnSrvdCell_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnSrvdCell_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnSrvdCell_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnSrvdCell_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnSrvdCell_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnSrvdCell_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_SrvdCell_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnSrvdCell_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_SrvdCell_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_SrvdCell_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_SrvdCell_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_SrvdCell_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_SrvdCell_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_SrvdCell_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_SrvdCell_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_SrvdCell_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_SrvdCell_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_SrvdCell_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_SrvdCell_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_SrvdCell_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_SrvdCell_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_SrvdCell_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_SrvdCell_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czSrvdCellsMemberTkn[] =
{
	&czSrvdCell_Inform,
	&czOptNeighbour_Inform,
		&czNeighbour_InformMember,
		&czConstTerm,
	&czOptProtExtnCont_SrvdCell_ExtIEs,
		&czProtExtnField_SrvdCell_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czSrvdCellsMemberspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	2,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czSrvdCellsMemberTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czSrvdCellsMember =
{
#ifdef CM_PASN_DBG
	" czSrvdCellsMember ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztSrvdCellsMember),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czSrvdCellsMemberspec),
	NULLP
};
CmPAsnSetSeqOfDef czSrvdCellsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxCellineNB,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czSrvdCells =
{
#ifdef CM_PASN_DBG
	" czSrvdCells ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztSrvdCells),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czSrvdCellsspec),
	NULLP
};
CmPAsnBitStrDef czShortMAC_Ispec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTR,
#endif /* CM_PASN_DEBUG */
	16,
	16
};
CmPAsnElmDef czShortMAC_I =
{
#ifdef CM_PASN_DBG
	" czShortMAC_I ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTR,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztShortMAC_I),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czShortMAC_Ispec),
	NULLP
};
/* Element ----- SRVCCOperationPossible ------- */
CmPAsnTknEnum czSRVCCOperationPossibleLst[2] =
{
	1,
	CztSRVCCOperationPossiblepossibleEnum
};
CmPAsnEnumDef czSRVCCOperationPossiblespec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czSRVCCOperationPossibleLst,
	NULLP
};
CmPAsnElmDef czSRVCCOperationPossible =
{
#ifdef CM_PASN_DBG
	" czSRVCCOperationPossible ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztSRVCCOperationPossible),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czSRVCCOperationPossiblespec),
	NULLP
};

/* Element ----- SubscriberProfileIDforRFP ------- */
CmPAsnUIntDef czSubscriberProfileIDforRFPspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	1,
	256UL
};
CmPAsnElmDef czOptSubscriberProfileIDforRFP =
{
#ifdef CM_PASN_DBG
	" czSubscriberProfileIDforRFP ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztSubscriberProfileIDforRFP),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czSubscriberProfileIDforRFPspec),
	NULLP
};

CmPAsnOctStrDef czTgetCellInUTRANspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_OCTSTRXL,
#endif /* CM_PASN_DEBUG */
	0,
	CM_PASN_PLUS_INF
};
CmPAsnElmDef czTgetCellInUTRAN =
{
#ifdef CM_PASN_DBG
	" czTgetCellInUTRAN " ,
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_OCTSTRXL,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztTgetCellInUTRAN),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czTgetCellInUTRANspec),
	NULLP
};
CmPAsnOctStrDef czTgeteNBtoSrc_eNBTprntContspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_OCTSTRXL,
#endif /* CM_PASN_DEBUG */
	0,
	CM_PASN_PLUS_INF
};
CmPAsnElmDef czTgeteNBtoSrc_eNBTprntCont =
{
#ifdef CM_PASN_DBG
	" czTgeteNBtoSrc_eNBTprntCont " ,
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_OCTSTRXL,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztTgeteNBtoSrc_eNBTprntCont),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czTgeteNBtoSrc_eNBTprntContspec),
	NULLP
};
/* Element ----- TimeToWait ------- */
CmPAsnTknEnum czTimeToWaitLst[7] =
{
	6,
	CztTimeToWaitv1sEnum,
	CztTimeToWaitv2sEnum,
	CztTimeToWaitv5sEnum,
	CztTimeToWaitv10sEnum,
	CztTimeToWaitv20sEnum,
	CztTimeToWaitv60sEnum
};
CmPAsnEnumDef czTimeToWaitspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czTimeToWaitLst,
	NULLP
};
CmPAsnElmDef czTimeToWait =
{
#ifdef CM_PASN_DBG
	" czTimeToWait ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztTimeToWait),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czTimeToWaitspec),
	NULLP
};

/* Element ----- TraceDepth ------- */
CmPAsnTknEnum czTraceDepthLst[7] =
{
	6,
	CztTraceDepthminimumEnum,
	CztTraceDepthmediumEnum,
	CztTraceDepthmaximumEnum,
	CztTraceDepthminimumWithoutVendorSpecificExtnEnum,
	CztTraceDepthmediumWithoutVendorSpecificExtnEnum,
	CztTraceDepthmaximumWithoutVendorSpecificExtnEnum
};
CmPAsnEnumDef czTraceDepthspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czTraceDepthLst,
	NULLP
};
CmPAsnElmDef czTraceDepth =
{
#ifdef CM_PASN_DBG
	" czTraceDepth ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztTraceDepth),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czTraceDepthspec),
	NULLP
};

CmPAsnBitStrDef czTraceCollectionEntityIPAddrspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTRXL,
#endif /* CM_PASN_DEBUG */
	1,
	160
};
CmPAsnElmDef czTraceCollectionEntityIPAddr =
{
#ifdef CM_PASN_DBG
	" czTraceCollectionEntityIPAddr ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTRXL,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztTraceCollectionEntityIPAddr),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czTraceCollectionEntityIPAddrspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnTraceActvn_ExtIEsClsInst[] =
{
	&czMDT_Config
};
PUBLIC U32 cz_ExtnTraceActvn_ExtIEsClsInstId[] =
{
	Cztid_MDTConfig
};
PUBLIC CmPAsnClassInfo cz_ExtnTraceActvn_ExtIEsClsInfo =
{
	1,
	&cz_ExtnTraceActvn_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnTraceActvn_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnTraceActvn_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnTraceActvn_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnTraceActvn_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnTraceActvn_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnTraceActvn_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnTraceActvn_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_TraceActvn_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnTraceActvn_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_TraceActvn_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_TraceActvn_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_TraceActvn_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_TraceActvn_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_TraceActvn_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_TraceActvn_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_TraceActvn_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_TraceActvn_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_TraceActvn_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_TraceActvn_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_TraceActvn_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_TraceActvn_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_TraceActvn_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_TraceActvn_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_TraceActvn_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czTraceActvnTkn[] =
{
	&czEUTRANTraceID,
	&czIntfsToTrace,
	&czTraceDepth,
	&czTraceCollectionEntityIPAddr,
	&czOptProtExtnCont_TraceActvn_ExtIEs,
		&czProtExtnField_TraceActvn_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czTraceActvnspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czTraceActvnTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czTraceActvn =
{
#ifdef CM_PASN_DBG
	" czTraceActvn ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztTraceActvn),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czTraceActvnspec),
	NULLP
};
CmPAsnSetSeqOfDef czUE_HistoryInformspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxnoofCells,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czUE_HistoryInform =
{
#ifdef CM_PASN_DBG
	" czUE_HistoryInform ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztUE_HistoryInform),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czUE_HistoryInformspec),
	NULLP
};
/* Element ----- UE-S1AP-ID ------- */
CmPAsnUIntDef czUE_S1AP_IDspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	4294967295UL
};
CmPAsnElmDef czUE_S1AP_ID =
{
#ifdef CM_PASN_DBG
	" czUE_S1AP_ID ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztUE_S1AP_ID),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czUE_S1AP_IDspec),
	NULLP
};

/* Element ----- UE-X2AP-ID ------- */
CmPAsnUIntDef czUE_X2AP_IDspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_UINT32,
#endif /* CM_PASN_DEBUG */
	0,
	4095UL
};
CmPAsnElmDef czUE_X2AP_ID =
{
#ifdef CM_PASN_DBG
	" czUE_X2AP_ID ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_UINT32,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztUE_X2AP_ID),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czUE_X2AP_IDspec),
	NULLP
};

PUBLIC CmPAsnElmDef *cz_ExtnUEAgg_MaxBitrate_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnUEAgg_MaxBitrate_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnUEAgg_MaxBitrate_ExtIEsClsInfo =
{
	0,
	&cz_ExtnUEAgg_MaxBitrate_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnUEAgg_MaxBitrate_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnUEAgg_MaxBitrate_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnUEAgg_MaxBitrate_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnUEAgg_MaxBitrate_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnUEAgg_MaxBitrate_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnUEAgg_MaxBitrate_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnUEAgg_MaxBitrate_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_UEAgg_MaxBitrate_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnUEAgg_MaxBitrate_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_UEAgg_MaxBitrate_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_UEAgg_MaxBitrate_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_UEAgg_MaxBitrate_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_UEAgg_MaxBitrate_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_UEAgg_MaxBitrate_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_UEAgg_MaxBitrate_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_UEAgg_MaxBitrate_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_UEAgg_MaxBitrate_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_UEAgg_MaxBitrate_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_UEAgg_MaxBitrate_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_UEAgg_MaxBitrate_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_UEAgg_MaxBitrate_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_UEAgg_MaxBitrate_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_UEAgg_MaxBitrate_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_UEAgg_MaxBitrate_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czUEAggMaxBitRateTkn[] =
{
	&czBitRate,
	&czBitRate,
	&czOptProtExtnCont_UEAgg_MaxBitrate_ExtIEs,
		&czProtExtnField_UEAgg_MaxBitrate_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czUEAggMaxBitRatespec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czUEAggMaxBitRateTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czUEAggMaxBitRate =
{
#ifdef CM_PASN_DBG
	" czUEAggMaxBitRate ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztUEAggMaxBitRate),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czUEAggMaxBitRatespec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnUESecurCapabilities_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnUESecurCapabilities_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnUESecurCapabilities_ExtIEsClsInfo =
{
	0,
	&cz_ExtnUESecurCapabilities_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnUESecurCapabilities_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnUESecurCapabilities_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnUESecurCapabilities_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnUESecurCapabilities_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnUESecurCapabilities_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnUESecurCapabilities_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnUESecurCapabilities_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_UESecurCapabilities_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnUESecurCapabilities_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_UESecurCapabilities_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_UESecurCapabilities_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_UESecurCapabilities_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_UESecurCapabilities_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_UESecurCapabilities_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_UESecurCapabilities_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_UESecurCapabilities_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_UESecurCapabilities_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_UESecurCapabilities_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_UESecurCapabilities_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_UESecurCapabilities_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_UESecurCapabilities_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_UESecurCapabilities_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_UESecurCapabilities_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_UESecurCapabilities_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czUESecurCapabilitiesTkn[] =
{
	&czEncryptionAlgorithms,
	&czIntegrityProtectionAlgorithms,
	&czOptProtExtnCont_UESecurCapabilities_ExtIEs,
		&czProtExtnField_UESecurCapabilities_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czUESecurCapabilitiesspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czUESecurCapabilitiesTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czUESecurCapabilities =
{
#ifdef CM_PASN_DBG
	" czUESecurCapabilities ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztUESecurCapabilities),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czUESecurCapabilitiesspec),
	NULLP
};
/* Element ----- UL-InterferenceOverloadIndication-Item ------- */
CmPAsnTknEnum czUL_InterferenceOverloadInd_ItemLst[4] =
{
	3,
	CztUL_InterferenceOverloadInd_Itemhigh_interferenceEnum,
	CztUL_InterferenceOverloadInd_Itemmedium_interferenceEnum,
	CztUL_InterferenceOverloadInd_Itemlow_interferenceEnum
};
CmPAsnEnumDef czUL_InterferenceOverloadInd_Itemspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czUL_InterferenceOverloadInd_ItemLst,
	NULLP
};
CmPAsnElmDef czUL_InterferenceOverloadInd_Item =
{
#ifdef CM_PASN_DBG
	" czUL_InterferenceOverloadInd_Item ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztUL_InterferenceOverloadInd_Item),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czUL_InterferenceOverloadInd_Itemspec),
	NULLP
};

CmPAsnSetSeqOfDef czUL_InterferenceOverloadIndspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxnoofPRBs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptUL_InterferenceOverloadInd =
{
#ifdef CM_PASN_DBG
	" czUL_InterferenceOverloadInd ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztUL_InterferenceOverloadInd),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czUL_InterferenceOverloadIndspec),
	NULLP
};
CmPAsnBitStrDef czUL_HighInterferenceIndspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTRXL,
#endif /* CM_PASN_DEBUG */
	1,
	110
};
CmPAsnElmDef czUL_HighInterferenceInd =
{
#ifdef CM_PASN_DBG
	" czUL_HighInterferenceInd ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTRXL,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztUL_HighInterferenceInd),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czUL_HighInterferenceIndspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnUL_HighInterferenceIndInfo_Item_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnUL_HighInterferenceIndInfo_Item_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnUL_HighInterferenceIndInfo_Item_ExtIEsClsInfo =
{
	0,
	&cz_ExtnUL_HighInterferenceIndInfo_Item_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnUL_HighInterferenceIndInfo_Item_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnUL_HighInterferenceIndInfo_Item_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnUL_HighInterferenceIndInfo_Item_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnUL_HighInterferenceIndInfo_Item_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnUL_HighInterferenceIndInfo_Item_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnUL_HighInterferenceIndInfo_Item_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnUL_HighInterferenceIndInfo_Item_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_UL_HighInterferenceIndInfo_Item_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnUL_HighInterferenceIndInfo_Item_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_UL_HighInterferenceIndInfo_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_UL_HighInterferenceIndInfo_Item_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_UL_HighInterferenceIndInfo_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_UL_HighInterferenceIndInfo_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_UL_HighInterferenceIndInfo_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_UL_HighInterferenceIndInfo_Item_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_UL_HighInterferenceIndInfo_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_UL_HighInterferenceIndInfo_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_UL_HighInterferenceIndInfo_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_UL_HighInterferenceIndInfo_Item_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_UL_HighInterferenceIndInfo_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_UL_HighInterferenceIndInfo_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_UL_HighInterferenceIndInfo_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_UL_HighInterferenceIndInfo_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_UL_HighInterferenceIndInfo_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czUL_HighInterferenceIndInfo_ItemTkn[] =
{
	&czECGI,
	&czUL_HighInterferenceInd,
	&czOptProtExtnCont_UL_HighInterferenceIndInfo_Item_ExtIEs,
		&czProtExtnField_UL_HighInterferenceIndInfo_Item_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czUL_HighInterferenceIndInfo_Itemspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czUL_HighInterferenceIndInfo_ItemTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czUL_HighInterferenceIndInfo_Item =
{
#ifdef CM_PASN_DBG
	" czUL_HighInterferenceIndInfo_Item ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztUL_HighInterferenceIndInfo_Item),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czUL_HighInterferenceIndInfo_Itemspec),
	NULLP
};
CmPAsnSetSeqOfDef czUL_HighInterferenceIndInfospec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxCellineNB,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptUL_HighInterferenceIndInfo =
{
#ifdef CM_PASN_DBG
	" czUL_HighInterferenceIndInfo ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztUL_HighInterferenceIndInfo),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czUL_HighInterferenceIndInfospec),
	NULLP
};
CmPAsnOctStrDef czUE_RLF_Report_Contspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_OCTSTRXL,
#endif /* CM_PASN_DEBUG */
	0,
	CM_PASN_PLUS_INF
};
CmPAsnElmDef czUE_RLF_Report_Cont =
{
#ifdef CM_PASN_DBG
	" czUE_RLF_Report_Cont " ,
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_OCTSTRXL,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztUE_RLF_Report_Cont),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czUE_RLF_Report_Contspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnE_RABs_ToBeSetup_ItemExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnE_RABs_ToBeSetup_ItemExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnE_RABs_ToBeSetup_ItemExtIEsClsInfo =
{
	0,
	&cz_ExtnE_RABs_ToBeSetup_ItemExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnE_RABs_ToBeSetup_ItemExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnE_RABs_ToBeSetup_ItemExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnE_RABs_ToBeSetup_ItemExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnE_RABs_ToBeSetup_ItemExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnE_RABs_ToBeSetup_ItemExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnE_RABs_ToBeSetup_ItemExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnE_RABs_ToBeSetup_ItemExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_E_RABs_ToBeSetup_ItemExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnE_RABs_ToBeSetup_ItemExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_E_RABs_ToBeSetup_ItemExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_E_RABs_ToBeSetup_ItemExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_E_RABs_ToBeSetup_ItemExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_E_RABs_ToBeSetup_ItemExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_E_RABs_ToBeSetup_ItemExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_E_RABs_ToBeSetup_ItemExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_E_RABs_ToBeSetup_ItemExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_E_RABs_ToBeSetup_ItemExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_E_RABs_ToBeSetup_ItemExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_E_RABs_ToBeSetup_ItemExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_E_RABs_ToBeSetup_ItemExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_E_RABs_ToBeSetup_ItemExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_E_RABs_ToBeSetup_ItemExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_E_RABs_ToBeSetup_ItemExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_E_RABs_ToBeSetup_ItemExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czE_RABs_ToBeSetup_ItemTkn[] =
{
	&czE_RAB_ID,
	&czE_RAB_Lvl_QoS_Params,
	&czOptDL_Fwding,
	&czGTPtunnelEndpoint,
	&czOptProtExtnCont_E_RABs_ToBeSetup_ItemExtIEs,
		&czProtExtnField_E_RABs_ToBeSetup_ItemExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czE_RABs_ToBeSetup_Itemspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	2,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czE_RABs_ToBeSetup_ItemTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czE_RABs_ToBeSetup_Item =
{
#ifdef CM_PASN_DBG
	" czE_RABs_ToBeSetup_Item ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztE_RABs_ToBeSetup_Item),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czE_RABs_ToBeSetup_Itemspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueE_RABs_ToBeSetup_ItemIEsClsInst[] =
{
	&czE_RABs_ToBeSetup_Item
};
PUBLIC U32 cz_ValueE_RABs_ToBeSetup_ItemIEsClsInstId[] =
{
	Cztid_E_RABs_ToBeSetup_Item
};
PUBLIC CmPAsnClassInfo cz_ValueE_RABs_ToBeSetup_ItemIEsClsInfo =
{
	1,
	&cz_ValueE_RABs_ToBeSetup_ItemIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueE_RABs_ToBeSetup_ItemIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueE_RABs_ToBeSetup_ItemIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueE_RABs_ToBeSetup_ItemIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueE_RABs_ToBeSetup_ItemIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueE_RABs_ToBeSetup_ItemIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueE_RABs_ToBeSetup_ItemIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueE_RABs_ToBeSetup_ItemIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_E_RABs_ToBeSetup_ItemIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueE_RABs_ToBeSetup_ItemIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_E_RABs_ToBeSetup_ItemIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_E_RABs_ToBeSetup_ItemIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_E_RABs_ToBeSetup_ItemIEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_E_RABs_ToBeSetup_ItemIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_E_RABs_ToBeSetup_ItemIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_E_RABs_ToBeSetup_ItemIEsspec),
	NULLP
};
#define czProtIE_Single_Cont_E_RABs_ToBeSetup_ItemIEs czProtIE_Field_E_RABs_ToBeSetup_ItemIEs
CmPAsnSetSeqOfDef czE_RABs_ToBeSetup_Lstspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxnoofBrs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czE_RABs_ToBeSetup_Lst =
{
#ifdef CM_PASN_DBG
	" czE_RABs_ToBeSetup_Lst ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztE_RABs_ToBeSetup_Lst),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czE_RABs_ToBeSetup_Lstspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnUE_CntxtInform_ExtIEsClsInst[] =
{
	&czManagementBasedMDTallowed,
	&czMDTPLMNLst,
		&czPLMN_Identity,
		&czConstTerm,
	
};
PUBLIC U32 cz_ExtnUE_CntxtInform_ExtIEsClsInstId[] =
{
	Cztid_ManagementBasedMDTallowed,
	Cztid_ManagementBasedMDTPLMNLst,
		Cztid_Invalid,
		Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnUE_CntxtInform_ExtIEsClsInfo =
{
	4,
	&cz_ExtnUE_CntxtInform_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnUE_CntxtInform_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnUE_CntxtInform_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnUE_CntxtInform_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnUE_CntxtInform_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnUE_CntxtInform_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnUE_CntxtInform_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnUE_CntxtInform_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_UE_CntxtInform_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnUE_CntxtInform_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_UE_CntxtInform_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_UE_CntxtInform_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_UE_CntxtInform_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_UE_CntxtInform_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_UE_CntxtInform_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_UE_CntxtInform_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_UE_CntxtInform_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_UE_CntxtInform_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_UE_CntxtInform_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_UE_CntxtInform_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_UE_CntxtInform_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_UE_CntxtInform_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_UE_CntxtInform_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_UE_CntxtInform_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_UE_CntxtInform_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czUE_CntxtInformTkn[] =
{
	&czUE_S1AP_ID,
	&czUESecurCapabilities,
	&czAS_SecurInform,
	&czUEAggMaxBitRate,
	&czOptSubscriberProfileIDforRFP,
	&czE_RABs_ToBeSetup_Lst,
		&czProtIE_Field_E_RABs_ToBeSetup_ItemIEs,
		&czConstTerm,
	&czRRC_Cntxt,
	&czOptHovrRestrnLst,
	&czOptLocRprtngInform,
	&czOptProtExtnCont_UE_CntxtInform_ExtIEs,
		&czProtExtnField_UE_CntxtInform_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czUE_CntxtInformspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	4,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czUE_CntxtInformTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czUE_CntxtInform =
{
#ifdef CM_PASN_DBG
	" czUE_CntxtInform ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztUE_CntxtInform),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czUE_CntxtInformspec),
	NULLP
};
CmPAsnBitStrDef czMobilityInformspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_BITSTR,
#endif /* CM_PASN_DEBUG */
	32,
	32
};
CmPAsnElmDef czMobilityInform =
{
#ifdef CM_PASN_DBG
	" czMobilityInform ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_BITSTR,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztMobilityInform),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czMobilityInformspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueHovrRqst_IEsClsInst[] =
{
	&czUE_X2AP_ID,
	&czCause,
	&czECGI,
	&czGUMMEI,
	&czUE_CntxtInform,
	&czUE_HistoryInform,
		&czLastVisitedCell_Item,
		&czConstTerm,
	&czTraceActvn,
	&czSRVCCOperationPossible,
	&czCSGMembershipStatus,
	&czMobilityInform
};
PUBLIC U32 cz_ValueHovrRqst_IEsClsInstId[] =
{
	Cztid_Old_eNB_UE_X2AP_ID,
	Cztid_Cause,
	Cztid_TgetCell_ID,
	Cztid_GUMMEI_ID,
	Cztid_UE_CntxtInform,
	Cztid_UE_HistoryInform,
		Cztid_Invalid,
		Cztid_Invalid,
	Cztid_TraceActvn,
	Cztid_SRVCCOperationPossible,
	Cztid_CSGMembershipStatus,
	Cztid_MobilityInform
};
PUBLIC CmPAsnClassInfo cz_ValueHovrRqst_IEsClsInfo =
{
	12,
	&cz_ValueHovrRqst_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueHovrRqst_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueHovrRqst_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueHovrRqst_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueHovrRqst_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueHovrRqst_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueHovrRqst_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueHovrRqst_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_HovrRqst_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueHovrRqst_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_HovrRqst_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_HovrRqst_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_HovrRqst_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_HovrRqst_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_HovrRqst_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_HovrRqst_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_HovrRqst_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_HovrRqst_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_HovrRqst_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_HovrRqst_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_HovrRqst_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czHovrRqstTkn[] =
{
	&czProtIE_Cont_HovrRqst_IEs,
		&czProtIE_Field_HovrRqst_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czHovrRqstspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czHovrRqstTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czHovrRqst =
{
#ifdef CM_PASN_DBG
	" czHovrRqst ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztHovrRqst),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czHovrRqstspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnE_RABs_Admtd_Item_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnE_RABs_Admtd_Item_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnE_RABs_Admtd_Item_ExtIEsClsInfo =
{
	0,
	&cz_ExtnE_RABs_Admtd_Item_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnE_RABs_Admtd_Item_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnE_RABs_Admtd_Item_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnE_RABs_Admtd_Item_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnE_RABs_Admtd_Item_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnE_RABs_Admtd_Item_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnE_RABs_Admtd_Item_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnE_RABs_Admtd_Item_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_E_RABs_Admtd_Item_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnE_RABs_Admtd_Item_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_E_RABs_Admtd_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_E_RABs_Admtd_Item_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_E_RABs_Admtd_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_E_RABs_Admtd_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_E_RABs_Admtd_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_E_RABs_Admtd_Item_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_E_RABs_Admtd_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_E_RABs_Admtd_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_E_RABs_Admtd_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_E_RABs_Admtd_Item_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_E_RABs_Admtd_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_E_RABs_Admtd_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_E_RABs_Admtd_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_E_RABs_Admtd_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_E_RABs_Admtd_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czE_RABs_Admtd_ItemTkn[] =
{
	&czE_RAB_ID,
	&czOptGTPtunnelEndpoint,
	&czOptGTPtunnelEndpoint,
	&czOptProtExtnCont_E_RABs_Admtd_Item_ExtIEs,
		&czProtExtnField_E_RABs_Admtd_Item_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czE_RABs_Admtd_Itemspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	3,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czE_RABs_Admtd_ItemTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czE_RABs_Admtd_Item =
{
#ifdef CM_PASN_DBG
	" czE_RABs_Admtd_Item ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztE_RABs_Admtd_Item),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czE_RABs_Admtd_Itemspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueE_RABs_Admtd_ItemIEsClsInst[] =
{
	&czE_RABs_Admtd_Item
};
PUBLIC U32 cz_ValueE_RABs_Admtd_ItemIEsClsInstId[] =
{
	Cztid_E_RABs_Admtd_Item
};
PUBLIC CmPAsnClassInfo cz_ValueE_RABs_Admtd_ItemIEsClsInfo =
{
	1,
	&cz_ValueE_RABs_Admtd_ItemIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueE_RABs_Admtd_ItemIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueE_RABs_Admtd_ItemIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueE_RABs_Admtd_ItemIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueE_RABs_Admtd_ItemIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueE_RABs_Admtd_ItemIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueE_RABs_Admtd_ItemIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueE_RABs_Admtd_ItemIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_E_RABs_Admtd_ItemIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueE_RABs_Admtd_ItemIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_E_RABs_Admtd_ItemIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_E_RABs_Admtd_ItemIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_E_RABs_Admtd_ItemIEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_E_RABs_Admtd_ItemIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_E_RABs_Admtd_ItemIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_E_RABs_Admtd_ItemIEsspec),
	NULLP
};
#define czProtIE_Single_Cont_E_RABs_Admtd_ItemIEs czProtIE_Field_E_RABs_Admtd_ItemIEs
CmPAsnSetSeqOfDef czE_RABs_Admtd_Lstspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxnoofBrs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czE_RABs_Admtd_Lst =
{
#ifdef CM_PASN_DBG
	" czE_RABs_Admtd_Lst ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztE_RABs_Admtd_Lst),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czE_RABs_Admtd_Lstspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueHovrRqstAckg_IEsClsInst[] =
{
	&czUE_X2AP_ID,
	&czUE_X2AP_ID,
	&czE_RABs_Admtd_Lst,
		&czProtIE_Field_E_RABs_Admtd_ItemIEs,
		&czConstTerm,
	&czE_RAB_Lst,
		&czProtIE_Field_E_RAB_ItemIEs,
		&czConstTerm,
	&czTgeteNBtoSrc_eNBTprntCont,
	&czCriticalityDiag
};
PUBLIC U32 cz_ValueHovrRqstAckg_IEsClsInstId[] =
{
	Cztid_Old_eNB_UE_X2AP_ID,
	Cztid_New_eNB_UE_X2AP_ID,
	Cztid_E_RABs_Admtd_Lst,
		Cztid_Invalid,
		Cztid_Invalid,
	Cztid_E_RABs_NotAdmtd_Lst,
		Cztid_Invalid,
		Cztid_Invalid,
	Cztid_TgeteNBtoSrc_eNBTprntCont,
	Cztid_CriticalityDiag
};
PUBLIC CmPAsnClassInfo cz_ValueHovrRqstAckg_IEsClsInfo =
{
	10,
	&cz_ValueHovrRqstAckg_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueHovrRqstAckg_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueHovrRqstAckg_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueHovrRqstAckg_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueHovrRqstAckg_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueHovrRqstAckg_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueHovrRqstAckg_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueHovrRqstAckg_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_HovrRqstAckg_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueHovrRqstAckg_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_HovrRqstAckg_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_HovrRqstAckg_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_HovrRqstAckg_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_HovrRqstAckg_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_HovrRqstAckg_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_HovrRqstAckg_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_HovrRqstAckg_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_HovrRqstAckg_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_HovrRqstAckg_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_HovrRqstAckg_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_HovrRqstAckg_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czHovrRqstAckgTkn[] =
{
	&czProtIE_Cont_HovrRqstAckg_IEs,
		&czProtIE_Field_HovrRqstAckg_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czHovrRqstAckgspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czHovrRqstAckgTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czHovrRqstAckg =
{
#ifdef CM_PASN_DBG
	" czHovrRqstAckg ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztHovrRqstAckg),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czHovrRqstAckgspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueHovrPrepFail_IEsClsInst[] =
{
	&czUE_X2AP_ID,
	&czCause,
	&czCriticalityDiag
};
PUBLIC U32 cz_ValueHovrPrepFail_IEsClsInstId[] =
{
	Cztid_Old_eNB_UE_X2AP_ID,
	Cztid_Cause,
	Cztid_CriticalityDiag
};
PUBLIC CmPAsnClassInfo cz_ValueHovrPrepFail_IEsClsInfo =
{
	3,
	&cz_ValueHovrPrepFail_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueHovrPrepFail_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueHovrPrepFail_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueHovrPrepFail_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueHovrPrepFail_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueHovrPrepFail_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueHovrPrepFail_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueHovrPrepFail_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_HovrPrepFail_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueHovrPrepFail_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_HovrPrepFail_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_HovrPrepFail_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_HovrPrepFail_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_HovrPrepFail_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_HovrPrepFail_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_HovrPrepFail_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_HovrPrepFail_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_HovrPrepFail_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_HovrPrepFail_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_HovrPrepFail_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_HovrPrepFail_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czHovrPrepFailTkn[] =
{
	&czProtIE_Cont_HovrPrepFail_IEs,
		&czProtIE_Field_HovrPrepFail_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czHovrPrepFailspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czHovrPrepFailTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czHovrPrepFail =
{
#ifdef CM_PASN_DBG
	" czHovrPrepFail ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztHovrPrepFail),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czHovrPrepFailspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueHovrReport_IEsClsInst[] =
{
	&czHovrReportTyp,
	&czCause,
	&czECGI,
	&czECGI,
	&czECGI,
	&czTgetCellInUTRAN,
	&czCRNTI,
	&czMobilityInform,
	&czUE_RLF_Report_Cont
};
PUBLIC U32 cz_ValueHovrReport_IEsClsInstId[] =
{
	Cztid_HovrReportTyp,
	Cztid_Cause,
	Cztid_SrcCellECGI,
	Cztid_FailCellECGI,
	Cztid_Re_establishmentCellECGI,
	Cztid_TgetCellInUTRAN,
	Cztid_SrcCellCRNTI,
	Cztid_MobilityInform,
	Cztid_UE_RLF_Report_Cont
};
PUBLIC CmPAsnClassInfo cz_ValueHovrReport_IEsClsInfo =
{
	9,
	&cz_ValueHovrReport_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueHovrReport_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueHovrReport_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueHovrReport_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueHovrReport_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueHovrReport_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueHovrReport_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueHovrReport_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_HovrReport_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueHovrReport_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_HovrReport_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_HovrReport_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_HovrReport_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_HovrReport_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_HovrReport_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_HovrReport_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_HovrReport_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_HovrReport_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_HovrReport_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_HovrReport_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_HovrReport_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czHovrReportTkn[] =
{
	&czProtIE_Cont_HovrReport_IEs,
		&czProtIE_Field_HovrReport_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czHovrReportspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czHovrReportTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czHovrReport =
{
#ifdef CM_PASN_DBG
	" czHovrReport ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztHovrReport),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czHovrReportspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnE_RABs_SubjToStatusTfr_ItemExtIEsClsInst[] =
{
	&czReceiveStatusOfULPDCPSDUsExtended,
	&czCOUNTValueExtended,
	&czCOUNTValueExtended
};
PUBLIC U32 cz_ExtnE_RABs_SubjToStatusTfr_ItemExtIEsClsInstId[] =
{
	Cztid_ReceiveStatusOfULPDCPSDUsExtended,
	Cztid_ULCOUNTValueExtended,
	Cztid_DLCOUNTValueExtended
};
PUBLIC CmPAsnClassInfo cz_ExtnE_RABs_SubjToStatusTfr_ItemExtIEsClsInfo =
{
	3,
	&cz_ExtnE_RABs_SubjToStatusTfr_ItemExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnE_RABs_SubjToStatusTfr_ItemExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnE_RABs_SubjToStatusTfr_ItemExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnE_RABs_SubjToStatusTfr_ItemExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnE_RABs_SubjToStatusTfr_ItemExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnE_RABs_SubjToStatusTfr_ItemExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnE_RABs_SubjToStatusTfr_ItemExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnE_RABs_SubjToStatusTfr_ItemExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_E_RABs_SubjToStatusTfr_ItemExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnE_RABs_SubjToStatusTfr_ItemExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_E_RABs_SubjToStatusTfr_ItemExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_E_RABs_SubjToStatusTfr_ItemExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_E_RABs_SubjToStatusTfr_ItemExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_E_RABs_SubjToStatusTfr_ItemExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_E_RABs_SubjToStatusTfr_ItemExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_E_RABs_SubjToStatusTfr_ItemExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_E_RABs_SubjToStatusTfr_ItemExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_E_RABs_SubjToStatusTfr_ItemExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_E_RABs_SubjToStatusTfr_ItemExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_E_RABs_SubjToStatusTfr_ItemExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_E_RABs_SubjToStatusTfr_ItemExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_E_RABs_SubjToStatusTfr_ItemExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_E_RABs_SubjToStatusTfr_ItemExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_E_RABs_SubjToStatusTfr_ItemExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_E_RABs_SubjToStatusTfr_ItemExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czE_RABs_SubjToStatusTfr_ItemTkn[] =
{
	&czE_RAB_ID,
	&czOptReceiveStatusofULPDCPSDUs,
	&czCOUNTvalue,
	&czCOUNTvalue,
	&czOptProtExtnCont_E_RABs_SubjToStatusTfr_ItemExtIEs,
		&czProtExtnField_E_RABs_SubjToStatusTfr_ItemExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czE_RABs_SubjToStatusTfr_Itemspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	2,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czE_RABs_SubjToStatusTfr_ItemTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czE_RABs_SubjToStatusTfr_Item =
{
#ifdef CM_PASN_DBG
	" czE_RABs_SubjToStatusTfr_Item ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztE_RABs_SubjToStatusTfr_Item),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czE_RABs_SubjToStatusTfr_Itemspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueE_RABs_SubjToStatusTfr_ItemIEsClsInst[] =
{
	&czE_RABs_SubjToStatusTfr_Item
};
PUBLIC U32 cz_ValueE_RABs_SubjToStatusTfr_ItemIEsClsInstId[] =
{
	Cztid_E_RABs_SubjToStatusTfr_Item
};
PUBLIC CmPAsnClassInfo cz_ValueE_RABs_SubjToStatusTfr_ItemIEsClsInfo =
{
	1,
	&cz_ValueE_RABs_SubjToStatusTfr_ItemIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueE_RABs_SubjToStatusTfr_ItemIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueE_RABs_SubjToStatusTfr_ItemIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueE_RABs_SubjToStatusTfr_ItemIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueE_RABs_SubjToStatusTfr_ItemIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueE_RABs_SubjToStatusTfr_ItemIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueE_RABs_SubjToStatusTfr_ItemIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueE_RABs_SubjToStatusTfr_ItemIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_E_RABs_SubjToStatusTfr_ItemIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueE_RABs_SubjToStatusTfr_ItemIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_E_RABs_SubjToStatusTfr_ItemIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_E_RABs_SubjToStatusTfr_ItemIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_E_RABs_SubjToStatusTfr_ItemIEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_E_RABs_SubjToStatusTfr_ItemIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_E_RABs_SubjToStatusTfr_ItemIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_E_RABs_SubjToStatusTfr_ItemIEsspec),
	NULLP
};
#define czProtIE_Single_Cont_E_RABs_SubjToStatusTfr_ItemIEs czProtIE_Field_E_RABs_SubjToStatusTfr_ItemIEs
CmPAsnSetSeqOfDef czE_RABs_SubjToStatusTfr_Lstspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxnoofBrs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czE_RABs_SubjToStatusTfr_Lst =
{
#ifdef CM_PASN_DBG
	" czE_RABs_SubjToStatusTfr_Lst ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztE_RABs_SubjToStatusTfr_Lst),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czE_RABs_SubjToStatusTfr_Lstspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueSNStatusTfr_IEsClsInst[] =
{
	&czUE_X2AP_ID,
	&czUE_X2AP_ID,
	&czE_RABs_SubjToStatusTfr_Lst,
		&czProtIE_Field_E_RABs_SubjToStatusTfr_ItemIEs,
		&czConstTerm,
	
};
PUBLIC U32 cz_ValueSNStatusTfr_IEsClsInstId[] =
{
	Cztid_Old_eNB_UE_X2AP_ID,
	Cztid_New_eNB_UE_X2AP_ID,
	Cztid_E_RABs_SubjToStatusTfr_Lst,
		Cztid_Invalid,
		Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ValueSNStatusTfr_IEsClsInfo =
{
	5,
	&cz_ValueSNStatusTfr_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueSNStatusTfr_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueSNStatusTfr_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueSNStatusTfr_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueSNStatusTfr_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueSNStatusTfr_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueSNStatusTfr_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueSNStatusTfr_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_SNStatusTfr_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueSNStatusTfr_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_SNStatusTfr_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_SNStatusTfr_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_SNStatusTfr_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_SNStatusTfr_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_SNStatusTfr_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_SNStatusTfr_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_SNStatusTfr_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_SNStatusTfr_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_SNStatusTfr_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_SNStatusTfr_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_SNStatusTfr_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czSNStatusTfrTkn[] =
{
	&czProtIE_Cont_SNStatusTfr_IEs,
		&czProtIE_Field_SNStatusTfr_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czSNStatusTfrspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czSNStatusTfrTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czSNStatusTfr =
{
#ifdef CM_PASN_DBG
	" czSNStatusTfr ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztSNStatusTfr),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czSNStatusTfrspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueUECntxtRls_IEsClsInst[] =
{
	&czUE_X2AP_ID,
	&czUE_X2AP_ID
};
PUBLIC U32 cz_ValueUECntxtRls_IEsClsInstId[] =
{
	Cztid_Old_eNB_UE_X2AP_ID,
	Cztid_New_eNB_UE_X2AP_ID
};
PUBLIC CmPAsnClassInfo cz_ValueUECntxtRls_IEsClsInfo =
{
	2,
	&cz_ValueUECntxtRls_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueUECntxtRls_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueUECntxtRls_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueUECntxtRls_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueUECntxtRls_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueUECntxtRls_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueUECntxtRls_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueUECntxtRls_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_UECntxtRls_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueUECntxtRls_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_UECntxtRls_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_UECntxtRls_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_UECntxtRls_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_UECntxtRls_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_UECntxtRls_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_UECntxtRls_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_UECntxtRls_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_UECntxtRls_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_UECntxtRls_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_UECntxtRls_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_UECntxtRls_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czUECntxtRlsTkn[] =
{
	&czProtIE_Cont_UECntxtRls_IEs,
		&czProtIE_Field_UECntxtRls_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czUECntxtRlsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czUECntxtRlsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czUECntxtRls =
{
#ifdef CM_PASN_DBG
	" czUECntxtRls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztUECntxtRls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czUECntxtRlsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueHovrCancel_IEsClsInst[] =
{
	&czUE_X2AP_ID,
	&czUE_X2AP_ID,
	&czCause
};
PUBLIC U32 cz_ValueHovrCancel_IEsClsInstId[] =
{
	Cztid_Old_eNB_UE_X2AP_ID,
	Cztid_New_eNB_UE_X2AP_ID,
	Cztid_Cause
};
PUBLIC CmPAsnClassInfo cz_ValueHovrCancel_IEsClsInfo =
{
	3,
	&cz_ValueHovrCancel_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueHovrCancel_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueHovrCancel_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueHovrCancel_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueHovrCancel_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueHovrCancel_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueHovrCancel_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueHovrCancel_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_HovrCancel_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueHovrCancel_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_HovrCancel_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_HovrCancel_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_HovrCancel_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_HovrCancel_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_HovrCancel_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_HovrCancel_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_HovrCancel_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_HovrCancel_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_HovrCancel_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_HovrCancel_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_HovrCancel_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czHovrCancelTkn[] =
{
	&czProtIE_Cont_HovrCancel_IEs,
		&czProtIE_Field_HovrCancel_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czHovrCancelspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czHovrCancelTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czHovrCancel =
{
#ifdef CM_PASN_DBG
	" czHovrCancel ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztHovrCancel),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czHovrCancelspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueErrInd_IEsClsInst[] =
{
	&czUE_X2AP_ID,
	&czUE_X2AP_ID,
	&czCause,
	&czCriticalityDiag
};
PUBLIC U32 cz_ValueErrInd_IEsClsInstId[] =
{
	Cztid_Old_eNB_UE_X2AP_ID,
	Cztid_New_eNB_UE_X2AP_ID,
	Cztid_Cause,
	Cztid_CriticalityDiag
};
PUBLIC CmPAsnClassInfo cz_ValueErrInd_IEsClsInfo =
{
	4,
	&cz_ValueErrInd_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueErrInd_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueErrInd_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueErrInd_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueErrInd_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueErrInd_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueErrInd_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueErrInd_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_ErrInd_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueErrInd_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_ErrInd_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_ErrInd_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_ErrInd_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_ErrInd_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_ErrInd_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_ErrInd_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_ErrInd_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_ErrInd_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_ErrInd_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_ErrInd_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_ErrInd_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czErrIndTkn[] =
{
	&czProtIE_Cont_ErrInd_IEs,
		&czProtIE_Field_ErrInd_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czErrIndspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czErrIndTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czErrInd =
{
#ifdef CM_PASN_DBG
	" czErrInd ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztErrInd),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czErrIndspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueResetRqst_IEsClsInst[] =
{
	&czCause
};
PUBLIC U32 cz_ValueResetRqst_IEsClsInstId[] =
{
	Cztid_Cause
};
PUBLIC CmPAsnClassInfo cz_ValueResetRqst_IEsClsInfo =
{
	1,
	&cz_ValueResetRqst_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueResetRqst_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueResetRqst_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueResetRqst_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueResetRqst_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueResetRqst_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueResetRqst_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueResetRqst_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_ResetRqst_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueResetRqst_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_ResetRqst_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_ResetRqst_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_ResetRqst_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_ResetRqst_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_ResetRqst_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_ResetRqst_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_ResetRqst_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_ResetRqst_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_ResetRqst_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_ResetRqst_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_ResetRqst_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czResetRqstTkn[] =
{
	&czProtIE_Cont_ResetRqst_IEs,
		&czProtIE_Field_ResetRqst_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czResetRqstspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czResetRqstTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czResetRqst =
{
#ifdef CM_PASN_DBG
	" czResetRqst ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztResetRqst),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czResetRqstspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueResetResp_IEsClsInst[] =
{
	&czCriticalityDiag
};
PUBLIC U32 cz_ValueResetResp_IEsClsInstId[] =
{
	Cztid_CriticalityDiag
};
PUBLIC CmPAsnClassInfo cz_ValueResetResp_IEsClsInfo =
{
	1,
	&cz_ValueResetResp_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueResetResp_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueResetResp_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueResetResp_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueResetResp_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueResetResp_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueResetResp_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueResetResp_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_ResetResp_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueResetResp_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_ResetResp_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_ResetResp_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_ResetResp_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_ResetResp_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_ResetResp_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_ResetResp_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_ResetResp_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_ResetResp_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_ResetResp_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_ResetResp_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_ResetResp_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czResetRespTkn[] =
{
	&czProtIE_Cont_ResetResp_IEs,
		&czProtIE_Field_ResetResp_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czResetRespspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czResetRespTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czResetResp =
{
#ifdef CM_PASN_DBG
	" czResetResp ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztResetResp),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czResetRespspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueX2SetupRqst_IEsClsInst[] =
{
	&czGlobalENB_ID,
	&czSrvdCells,
		&czSrvdCellsMember,
		&czConstTerm,
	&czGUGroupIDLst,
		&czGU_Group_ID,
		&czConstTerm,
	
};
PUBLIC U32 cz_ValueX2SetupRqst_IEsClsInstId[] =
{
	Cztid_GlobalENB_ID,
	Cztid_SrvdCells,
		Cztid_Invalid,
		Cztid_Invalid,
	Cztid_GUGroupIDLst,
		Cztid_Invalid,
		Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ValueX2SetupRqst_IEsClsInfo =
{
	7,
	&cz_ValueX2SetupRqst_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueX2SetupRqst_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueX2SetupRqst_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueX2SetupRqst_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueX2SetupRqst_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueX2SetupRqst_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueX2SetupRqst_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueX2SetupRqst_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_X2SetupRqst_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueX2SetupRqst_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_X2SetupRqst_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_X2SetupRqst_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_X2SetupRqst_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_X2SetupRqst_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_X2SetupRqst_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_X2SetupRqst_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_X2SetupRqst_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_X2SetupRqst_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_X2SetupRqst_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_X2SetupRqst_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_X2SetupRqst_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czX2SetupRqstTkn[] =
{
	&czProtIE_Cont_X2SetupRqst_IEs,
		&czProtIE_Field_X2SetupRqst_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czX2SetupRqstspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czX2SetupRqstTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czX2SetupRqst =
{
#ifdef CM_PASN_DBG
	" czX2SetupRqst ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztX2SetupRqst),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czX2SetupRqstspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueX2SetupResp_IEsClsInst[] =
{
	&czGlobalENB_ID,
	&czSrvdCells,
		&czSrvdCellsMember,
		&czConstTerm,
	&czGUGroupIDLst,
		&czGU_Group_ID,
		&czConstTerm,
	&czCriticalityDiag
};
PUBLIC U32 cz_ValueX2SetupResp_IEsClsInstId[] =
{
	Cztid_GlobalENB_ID,
	Cztid_SrvdCells,
		Cztid_Invalid,
		Cztid_Invalid,
	Cztid_GUGroupIDLst,
		Cztid_Invalid,
		Cztid_Invalid,
	Cztid_CriticalityDiag
};
PUBLIC CmPAsnClassInfo cz_ValueX2SetupResp_IEsClsInfo =
{
	8,
	&cz_ValueX2SetupResp_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueX2SetupResp_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueX2SetupResp_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueX2SetupResp_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueX2SetupResp_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueX2SetupResp_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueX2SetupResp_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueX2SetupResp_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_X2SetupResp_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueX2SetupResp_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_X2SetupResp_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_X2SetupResp_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_X2SetupResp_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_X2SetupResp_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_X2SetupResp_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_X2SetupResp_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_X2SetupResp_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_X2SetupResp_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_X2SetupResp_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_X2SetupResp_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_X2SetupResp_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czX2SetupRespTkn[] =
{
	&czProtIE_Cont_X2SetupResp_IEs,
		&czProtIE_Field_X2SetupResp_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czX2SetupRespspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czX2SetupRespTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czX2SetupResp =
{
#ifdef CM_PASN_DBG
	" czX2SetupResp ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztX2SetupResp),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czX2SetupRespspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueX2SetupFail_IEsClsInst[] =
{
	&czCause,
	&czTimeToWait,
	&czCriticalityDiag
};
PUBLIC U32 cz_ValueX2SetupFail_IEsClsInstId[] =
{
	Cztid_Cause,
	Cztid_TimeToWait,
	Cztid_CriticalityDiag
};
PUBLIC CmPAsnClassInfo cz_ValueX2SetupFail_IEsClsInfo =
{
	3,
	&cz_ValueX2SetupFail_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueX2SetupFail_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueX2SetupFail_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueX2SetupFail_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueX2SetupFail_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueX2SetupFail_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueX2SetupFail_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueX2SetupFail_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_X2SetupFail_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueX2SetupFail_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_X2SetupFail_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_X2SetupFail_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_X2SetupFail_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_X2SetupFail_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_X2SetupFail_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_X2SetupFail_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_X2SetupFail_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_X2SetupFail_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_X2SetupFail_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_X2SetupFail_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_X2SetupFail_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czX2SetupFailTkn[] =
{
	&czProtIE_Cont_X2SetupFail_IEs,
		&czProtIE_Field_X2SetupFail_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czX2SetupFailspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czX2SetupFailTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czX2SetupFail =
{
#ifdef CM_PASN_DBG
	" czX2SetupFail ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztX2SetupFail),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czX2SetupFailspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnCellInform_Item_ExtIEsClsInst[] =
{
	&czABSInform,
	&czInvokeInd
};
PUBLIC U32 cz_ExtnCellInform_Item_ExtIEsClsInstId[] =
{
	Cztid_ABSInform,
	Cztid_InvokeInd
};
PUBLIC CmPAsnClassInfo cz_ExtnCellInform_Item_ExtIEsClsInfo =
{
	2,
	&cz_ExtnCellInform_Item_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnCellInform_Item_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnCellInform_Item_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnCellInform_Item_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnCellInform_Item_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnCellInform_Item_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnCellInform_Item_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnCellInform_Item_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_CellInform_Item_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnCellInform_Item_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_CellInform_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_CellInform_Item_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_CellInform_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_CellInform_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_CellInform_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_CellInform_Item_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_CellInform_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_CellInform_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_CellInform_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_CellInform_Item_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_CellInform_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_CellInform_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_CellInform_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_CellInform_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_CellInform_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czCellInform_ItemTkn[] =
{
	&czECGI,
	&czOptUL_InterferenceOverloadInd,
		&czUL_InterferenceOverloadInd_Item,
		&czConstTerm,
	&czOptUL_HighInterferenceIndInfo,
		&czUL_HighInterferenceIndInfo_Item,
		&czConstTerm,
	&czOptRelativeNarrowbandTxPower,
	&czOptProtExtnCont_CellInform_Item_ExtIEs,
		&czProtExtnField_CellInform_Item_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czCellInform_Itemspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	4,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czCellInform_ItemTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czCellInform_Item =
{
#ifdef CM_PASN_DBG
	" czCellInform_Item ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztCellInform_Item),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czCellInform_Itemspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueCellInform_ItemIEsClsInst[] =
{
	&czCellInform_Item
};
PUBLIC U32 cz_ValueCellInform_ItemIEsClsInstId[] =
{
	Cztid_CellInform_Item
};
PUBLIC CmPAsnClassInfo cz_ValueCellInform_ItemIEsClsInfo =
{
	1,
	&cz_ValueCellInform_ItemIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueCellInform_ItemIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueCellInform_ItemIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueCellInform_ItemIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueCellInform_ItemIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueCellInform_ItemIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueCellInform_ItemIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueCellInform_ItemIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_CellInform_ItemIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueCellInform_ItemIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_CellInform_ItemIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_CellInform_ItemIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_CellInform_ItemIEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_CellInform_ItemIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_CellInform_ItemIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_CellInform_ItemIEsspec),
	NULLP
};
#define czProtIE_Single_Cont_CellInform_ItemIEs czProtIE_Field_CellInform_ItemIEs
CmPAsnSetSeqOfDef czCellInform_Lstspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxCellineNB,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czCellInform_Lst =
{
#ifdef CM_PASN_DBG
	" czCellInform_Lst ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztCellInform_Lst),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czCellInform_Lstspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueLoadInform_IEsClsInst[] =
{
	&czCellInform_Lst,
		&czProtIE_Field_CellInform_ItemIEs,
		&czConstTerm,
	
};
PUBLIC U32 cz_ValueLoadInform_IEsClsInstId[] =
{
	Cztid_CellInform,
		Cztid_Invalid,
		Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ValueLoadInform_IEsClsInfo =
{
	3,
	&cz_ValueLoadInform_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueLoadInform_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueLoadInform_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueLoadInform_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueLoadInform_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueLoadInform_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueLoadInform_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueLoadInform_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_LoadInform_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueLoadInform_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_LoadInform_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_LoadInform_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_LoadInform_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_LoadInform_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_LoadInform_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_LoadInform_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_LoadInform_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_LoadInform_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_LoadInform_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_LoadInform_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_LoadInform_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czLoadInformTkn[] =
{
	&czProtIE_Cont_LoadInform_IEs,
		&czProtIE_Field_LoadInform_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czLoadInformspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czLoadInformTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czLoadInform =
{
#ifdef CM_PASN_DBG
	" czLoadInform ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztLoadInform),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czLoadInformspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnSrvdCellsToMdfy_Item_ExtIEsClsInst[] =
{
	&czDeactivationInd
};
PUBLIC U32 cz_ExtnSrvdCellsToMdfy_Item_ExtIEsClsInstId[] =
{
	Cztid_DeactivationInd
};
PUBLIC CmPAsnClassInfo cz_ExtnSrvdCellsToMdfy_Item_ExtIEsClsInfo =
{
	1,
	&cz_ExtnSrvdCellsToMdfy_Item_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnSrvdCellsToMdfy_Item_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnSrvdCellsToMdfy_Item_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnSrvdCellsToMdfy_Item_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnSrvdCellsToMdfy_Item_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnSrvdCellsToMdfy_Item_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnSrvdCellsToMdfy_Item_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnSrvdCellsToMdfy_Item_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_SrvdCellsToMdfy_Item_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnSrvdCellsToMdfy_Item_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_SrvdCellsToMdfy_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_SrvdCellsToMdfy_Item_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_SrvdCellsToMdfy_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_SrvdCellsToMdfy_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_SrvdCellsToMdfy_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_SrvdCellsToMdfy_Item_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_SrvdCellsToMdfy_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_SrvdCellsToMdfy_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_SrvdCellsToMdfy_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_SrvdCellsToMdfy_Item_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_SrvdCellsToMdfy_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_SrvdCellsToMdfy_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_SrvdCellsToMdfy_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_SrvdCellsToMdfy_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_SrvdCellsToMdfy_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czSrvdCellsToMdfy_ItemTkn[] =
{
	&czECGI,
	&czSrvdCell_Inform,
	&czOptNeighbour_Inform,
		&czNeighbour_InformMember,
		&czConstTerm,
	&czOptProtExtnCont_SrvdCellsToMdfy_Item_ExtIEs,
		&czProtExtnField_SrvdCellsToMdfy_Item_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czSrvdCellsToMdfy_Itemspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	2,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czSrvdCellsToMdfy_ItemTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czSrvdCellsToMdfy_Item =
{
#ifdef CM_PASN_DBG
	" czSrvdCellsToMdfy_Item ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztSrvdCellsToMdfy_Item),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czSrvdCellsToMdfy_Itemspec),
	NULLP
};
CmPAsnSetSeqOfDef czSrvdCellsToMdfyspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxCellineNB,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czSrvdCellsToMdfy =
{
#ifdef CM_PASN_DBG
	" czSrvdCellsToMdfy ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztSrvdCellsToMdfy),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czSrvdCellsToMdfyspec),
	NULLP
};
CmPAsnSetSeqOfDef czOld_ECGIsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxCellineNB,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOld_ECGIs =
{
#ifdef CM_PASN_DBG
	" czOld_ECGIs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztOld_ECGIs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czOld_ECGIsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueENBConfigUpd_IEsClsInst[] =
{
	&czSrvdCells,
		&czSrvdCellsMember,
		&czConstTerm,
	&czSrvdCellsToMdfy,
		&czSrvdCellsToMdfy_Item,
		&czConstTerm,
	&czOld_ECGIs,
		&czECGI,
		&czConstTerm,
	&czGUGroupIDLst,
		&czGU_Group_ID,
		&czConstTerm,
	&czGUGroupIDLst,
		&czGU_Group_ID,
		&czConstTerm,
	
};
PUBLIC U32 cz_ValueENBConfigUpd_IEsClsInstId[] =
{
	Cztid_SrvdCellsToAdd,
		Cztid_Invalid,
		Cztid_Invalid,
	Cztid_SrvdCellsToMdfy,
		Cztid_Invalid,
		Cztid_Invalid,
	Cztid_SrvdCellsToDelete,
		Cztid_Invalid,
		Cztid_Invalid,
	Cztid_GUGroupIDToAddLst,
		Cztid_Invalid,
		Cztid_Invalid,
	Cztid_GUGroupIDToDeleteLst,
		Cztid_Invalid,
		Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ValueENBConfigUpd_IEsClsInfo =
{
	15,
	&cz_ValueENBConfigUpd_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueENBConfigUpd_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueENBConfigUpd_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueENBConfigUpd_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueENBConfigUpd_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueENBConfigUpd_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueENBConfigUpd_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueENBConfigUpd_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_ENBConfigUpd_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueENBConfigUpd_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_ENBConfigUpd_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_ENBConfigUpd_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_ENBConfigUpd_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_ENBConfigUpd_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_ENBConfigUpd_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_ENBConfigUpd_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_ENBConfigUpd_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_ENBConfigUpd_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_ENBConfigUpd_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_ENBConfigUpd_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_ENBConfigUpd_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czENBConfigUpdTkn[] =
{
	&czProtIE_Cont_ENBConfigUpd_IEs,
		&czProtIE_Field_ENBConfigUpd_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czENBConfigUpdspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czENBConfigUpdTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czENBConfigUpd =
{
#ifdef CM_PASN_DBG
	" czENBConfigUpd ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztENBConfigUpd),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czENBConfigUpdspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueENBConfigUpdAckg_IEsClsInst[] =
{
	&czCriticalityDiag
};
PUBLIC U32 cz_ValueENBConfigUpdAckg_IEsClsInstId[] =
{
	Cztid_CriticalityDiag
};
PUBLIC CmPAsnClassInfo cz_ValueENBConfigUpdAckg_IEsClsInfo =
{
	1,
	&cz_ValueENBConfigUpdAckg_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueENBConfigUpdAckg_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueENBConfigUpdAckg_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueENBConfigUpdAckg_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueENBConfigUpdAckg_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueENBConfigUpdAckg_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueENBConfigUpdAckg_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueENBConfigUpdAckg_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_ENBConfigUpdAckg_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueENBConfigUpdAckg_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_ENBConfigUpdAckg_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_ENBConfigUpdAckg_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_ENBConfigUpdAckg_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_ENBConfigUpdAckg_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_ENBConfigUpdAckg_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_ENBConfigUpdAckg_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_ENBConfigUpdAckg_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_ENBConfigUpdAckg_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_ENBConfigUpdAckg_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_ENBConfigUpdAckg_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_ENBConfigUpdAckg_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czENBConfigUpdAckgTkn[] =
{
	&czProtIE_Cont_ENBConfigUpdAckg_IEs,
		&czProtIE_Field_ENBConfigUpdAckg_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czENBConfigUpdAckgspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czENBConfigUpdAckgTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czENBConfigUpdAckg =
{
#ifdef CM_PASN_DBG
	" czENBConfigUpdAckg ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztENBConfigUpdAckg),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czENBConfigUpdAckgspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueENBConfigUpdFail_IEsClsInst[] =
{
	&czCause,
	&czTimeToWait,
	&czCriticalityDiag
};
PUBLIC U32 cz_ValueENBConfigUpdFail_IEsClsInstId[] =
{
	Cztid_Cause,
	Cztid_TimeToWait,
	Cztid_CriticalityDiag
};
PUBLIC CmPAsnClassInfo cz_ValueENBConfigUpdFail_IEsClsInfo =
{
	3,
	&cz_ValueENBConfigUpdFail_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueENBConfigUpdFail_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueENBConfigUpdFail_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueENBConfigUpdFail_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueENBConfigUpdFail_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueENBConfigUpdFail_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueENBConfigUpdFail_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueENBConfigUpdFail_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_ENBConfigUpdFail_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueENBConfigUpdFail_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_ENBConfigUpdFail_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_ENBConfigUpdFail_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_ENBConfigUpdFail_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_ENBConfigUpdFail_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_ENBConfigUpdFail_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_ENBConfigUpdFail_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_ENBConfigUpdFail_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_ENBConfigUpdFail_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_ENBConfigUpdFail_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_ENBConfigUpdFail_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_ENBConfigUpdFail_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czENBConfigUpdFailTkn[] =
{
	&czProtIE_Cont_ENBConfigUpdFail_IEs,
		&czProtIE_Field_ENBConfigUpdFail_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czENBConfigUpdFailspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czENBConfigUpdFailTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czENBConfigUpdFail =
{
#ifdef CM_PASN_DBG
	" czENBConfigUpdFail ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztENBConfigUpdFail),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czENBConfigUpdFailspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnCellToReport_Item_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnCellToReport_Item_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnCellToReport_Item_ExtIEsClsInfo =
{
	0,
	&cz_ExtnCellToReport_Item_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnCellToReport_Item_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnCellToReport_Item_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnCellToReport_Item_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnCellToReport_Item_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnCellToReport_Item_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnCellToReport_Item_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnCellToReport_Item_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_CellToReport_Item_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnCellToReport_Item_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_CellToReport_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_CellToReport_Item_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_CellToReport_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_CellToReport_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_CellToReport_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_CellToReport_Item_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_CellToReport_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_CellToReport_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_CellToReport_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_CellToReport_Item_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_CellToReport_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_CellToReport_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_CellToReport_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_CellToReport_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_CellToReport_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czCellToReport_ItemTkn[] =
{
	&czECGI,
	&czOptProtExtnCont_CellToReport_Item_ExtIEs,
		&czProtExtnField_CellToReport_Item_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czCellToReport_Itemspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czCellToReport_ItemTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czCellToReport_Item =
{
#ifdef CM_PASN_DBG
	" czCellToReport_Item ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztCellToReport_Item),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czCellToReport_Itemspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueCellToReport_ItemIEsClsInst[] =
{
	&czCellToReport_Item
};
PUBLIC U32 cz_ValueCellToReport_ItemIEsClsInstId[] =
{
	Cztid_CellToReport_Item
};
PUBLIC CmPAsnClassInfo cz_ValueCellToReport_ItemIEsClsInfo =
{
	1,
	&cz_ValueCellToReport_ItemIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueCellToReport_ItemIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueCellToReport_ItemIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueCellToReport_ItemIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueCellToReport_ItemIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueCellToReport_ItemIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueCellToReport_ItemIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueCellToReport_ItemIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_CellToReport_ItemIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueCellToReport_ItemIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_CellToReport_ItemIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_CellToReport_ItemIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_CellToReport_ItemIEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_CellToReport_ItemIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_CellToReport_ItemIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_CellToReport_ItemIEsspec),
	NULLP
};
#define czProtIE_Single_Cont_CellToReport_ItemIEs czProtIE_Field_CellToReport_ItemIEs
CmPAsnSetSeqOfDef czCellToReport_Lstspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxCellineNB,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czCellToReport_Lst =
{
#ifdef CM_PASN_DBG
	" czCellToReport_Lst ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztCellToReport_Lst),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czCellToReport_Lstspec),
	NULLP
};
/* Element ----- ReportingPeriodicity ------- */
CmPAsnTknEnum czRprtngPeriodicityLst[5] =
{
	4,
	CztRprtngPeriodicityone_thousand_msEnum,
	CztRprtngPeriodicitytwo_thousand_msEnum,
	CztRprtngPeriodicityfive_thousand_msEnum,
	CztRprtngPeriodicityten_thousand_msEnum
};
CmPAsnEnumDef czRprtngPeriodicityspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czRprtngPeriodicityLst,
	NULLP
};
CmPAsnElmDef czRprtngPeriodicity =
{
#ifdef CM_PASN_DBG
	" czRprtngPeriodicity ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztRprtngPeriodicity),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czRprtngPeriodicityspec),
	NULLP
};

/* Element ----- PartialSuccessIndicator ------- */
CmPAsnTknEnum czPartialSuccessIndicatorLst[2] =
{
	1,
	CztPartialSuccessIndicatorpartial_success_allowedEnum
};
CmPAsnEnumDef czPartialSuccessIndicatorspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_ENUM,
#endif /* CM_PASN_DEBUG */
	czPartialSuccessIndicatorLst,
	NULLP
};
CmPAsnElmDef czPartialSuccessIndicator =
{
#ifdef CM_PASN_DBG
	" czPartialSuccessIndicator ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_ENUM,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztPartialSuccessIndicator),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czPartialSuccessIndicatorspec),
	NULLP
};

PUBLIC CmPAsnElmDef *cz_ValueResStatusRqst_IEsClsInst[] =
{
	&czMeasurement_ID,
	&czMeasurement_ID,
	&czRegistration_Rqst,
	&czReportCharacteristics,
	&czCellToReport_Lst,
		&czProtIE_Field_CellToReport_ItemIEs,
		&czConstTerm,
	&czRprtngPeriodicity,
	&czPartialSuccessIndicator
};
PUBLIC U32 cz_ValueResStatusRqst_IEsClsInstId[] =
{
	Cztid_ENB1_Measurement_ID,
	Cztid_ENB2_Measurement_ID,
	Cztid_Registration_Rqst,
	Cztid_ReportCharacteristics,
	Cztid_CellToReport,
		Cztid_Invalid,
		Cztid_Invalid,
	Cztid_RprtngPeriodicity,
	Cztid_PartialSuccessIndicator
};
PUBLIC CmPAsnClassInfo cz_ValueResStatusRqst_IEsClsInfo =
{
	9,
	&cz_ValueResStatusRqst_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueResStatusRqst_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueResStatusRqst_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueResStatusRqst_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueResStatusRqst_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueResStatusRqst_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueResStatusRqst_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueResStatusRqst_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_ResStatusRqst_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueResStatusRqst_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_ResStatusRqst_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_ResStatusRqst_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_ResStatusRqst_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_ResStatusRqst_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_ResStatusRqst_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_ResStatusRqst_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_ResStatusRqst_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_ResStatusRqst_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_ResStatusRqst_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_ResStatusRqst_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_ResStatusRqst_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czResStatusRqstTkn[] =
{
	&czProtIE_Cont_ResStatusRqst_IEs,
		&czProtIE_Field_ResStatusRqst_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czResStatusRqstspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czResStatusRqstTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czResStatusRqst =
{
#ifdef CM_PASN_DBG
	" czResStatusRqst ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztResStatusRqst),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czResStatusRqstspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnMeasurementFailCause_Item_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnMeasurementFailCause_Item_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnMeasurementFailCause_Item_ExtIEsClsInfo =
{
	0,
	&cz_ExtnMeasurementFailCause_Item_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnMeasurementFailCause_Item_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnMeasurementFailCause_Item_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnMeasurementFailCause_Item_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnMeasurementFailCause_Item_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnMeasurementFailCause_Item_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnMeasurementFailCause_Item_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnMeasurementFailCause_Item_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_MeasurementFailCause_Item_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnMeasurementFailCause_Item_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_MeasurementFailCause_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_MeasurementFailCause_Item_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_MeasurementFailCause_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_MeasurementFailCause_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_MeasurementFailCause_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_MeasurementFailCause_Item_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_MeasurementFailCause_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_MeasurementFailCause_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_MeasurementFailCause_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_MeasurementFailCause_Item_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_MeasurementFailCause_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_MeasurementFailCause_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_MeasurementFailCause_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_MeasurementFailCause_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_MeasurementFailCause_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czMeasurementFailCause_ItemTkn[] =
{
	&czReportCharacteristics,
	&czCause,
	&czOptProtExtnCont_MeasurementFailCause_Item_ExtIEs,
		&czProtExtnField_MeasurementFailCause_Item_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czMeasurementFailCause_Itemspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czMeasurementFailCause_ItemTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czMeasurementFailCause_Item =
{
#ifdef CM_PASN_DBG
	" czMeasurementFailCause_Item ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztMeasurementFailCause_Item),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czMeasurementFailCause_Itemspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueMeasurementFailCause_ItemIEsClsInst[] =
{
	&czMeasurementFailCause_Item
};
PUBLIC U32 cz_ValueMeasurementFailCause_ItemIEsClsInstId[] =
{
	Cztid_MeasurementFailCause_Item
};
PUBLIC CmPAsnClassInfo cz_ValueMeasurementFailCause_ItemIEsClsInfo =
{
	1,
	&cz_ValueMeasurementFailCause_ItemIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueMeasurementFailCause_ItemIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueMeasurementFailCause_ItemIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueMeasurementFailCause_ItemIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueMeasurementFailCause_ItemIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueMeasurementFailCause_ItemIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueMeasurementFailCause_ItemIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueMeasurementFailCause_ItemIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_MeasurementFailCause_ItemIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueMeasurementFailCause_ItemIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_MeasurementFailCause_ItemIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_MeasurementFailCause_ItemIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_MeasurementFailCause_ItemIEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_MeasurementFailCause_ItemIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_MeasurementFailCause_ItemIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_MeasurementFailCause_ItemIEsspec),
	NULLP
};
#define czProtIE_Single_Cont_MeasurementFailCause_ItemIEs czProtIE_Field_MeasurementFailCause_ItemIEs
CmPAsnSetSeqOfDef czMeasurementFailCause_Lstspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxFailedMeasObjects,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptMeasurementFailCause_Lst =
{
#ifdef CM_PASN_DBG
	" czMeasurementFailCause_Lst ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztMeasurementFailCause_Lst),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czMeasurementFailCause_Lstspec),
	NULLP
};
PUBLIC CmPAsnElmDef czMeasurementFailCause_Lst =
{
#ifdef CM_PASN_DBG
	" czMeasurementFailCause_Lst ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztMeasurementFailCause_Lst),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czMeasurementFailCause_Lstspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnMeasurementInitiationResult_Item_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnMeasurementInitiationResult_Item_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnMeasurementInitiationResult_Item_ExtIEsClsInfo =
{
	0,
	&cz_ExtnMeasurementInitiationResult_Item_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnMeasurementInitiationResult_Item_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnMeasurementInitiationResult_Item_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnMeasurementInitiationResult_Item_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnMeasurementInitiationResult_Item_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnMeasurementInitiationResult_Item_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnMeasurementInitiationResult_Item_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnMeasurementInitiationResult_Item_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_MeasurementInitiationResult_Item_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnMeasurementInitiationResult_Item_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_MeasurementInitiationResult_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_MeasurementInitiationResult_Item_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_MeasurementInitiationResult_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_MeasurementInitiationResult_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_MeasurementInitiationResult_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_MeasurementInitiationResult_Item_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_MeasurementInitiationResult_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_MeasurementInitiationResult_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_MeasurementInitiationResult_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_MeasurementInitiationResult_Item_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_MeasurementInitiationResult_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_MeasurementInitiationResult_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_MeasurementInitiationResult_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_MeasurementInitiationResult_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_MeasurementInitiationResult_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czMeasurementInitiationResult_ItemTkn[] =
{
	&czECGI,
	&czOptMeasurementFailCause_Lst,
		&czProtIE_Field_MeasurementFailCause_ItemIEs,
		&czConstTerm,
	&czOptProtExtnCont_MeasurementInitiationResult_Item_ExtIEs,
		&czProtExtnField_MeasurementInitiationResult_Item_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czMeasurementInitiationResult_Itemspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	2,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czMeasurementInitiationResult_ItemTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czMeasurementInitiationResult_Item =
{
#ifdef CM_PASN_DBG
	" czMeasurementInitiationResult_Item ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztMeasurementInitiationResult_Item),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czMeasurementInitiationResult_Itemspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueMeasurementInitiationResult_ItemIEsClsInst[] =
{
	&czMeasurementInitiationResult_Item
};
PUBLIC U32 cz_ValueMeasurementInitiationResult_ItemIEsClsInstId[] =
{
	Cztid_MeasurementInitiationResult_Item
};
PUBLIC CmPAsnClassInfo cz_ValueMeasurementInitiationResult_ItemIEsClsInfo =
{
	1,
	&cz_ValueMeasurementInitiationResult_ItemIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueMeasurementInitiationResult_ItemIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueMeasurementInitiationResult_ItemIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueMeasurementInitiationResult_ItemIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueMeasurementInitiationResult_ItemIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueMeasurementInitiationResult_ItemIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueMeasurementInitiationResult_ItemIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueMeasurementInitiationResult_ItemIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_MeasurementInitiationResult_ItemIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueMeasurementInitiationResult_ItemIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_MeasurementInitiationResult_ItemIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_MeasurementInitiationResult_ItemIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_MeasurementInitiationResult_ItemIEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_MeasurementInitiationResult_ItemIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_MeasurementInitiationResult_ItemIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_MeasurementInitiationResult_ItemIEsspec),
	NULLP
};
#define czProtIE_Single_Cont_MeasurementInitiationResult_ItemIEs czProtIE_Field_MeasurementInitiationResult_ItemIEs
CmPAsnSetSeqOfDef czMeasurementInitiationResult_Lstspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxCellineNB,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czMeasurementInitiationResult_Lst =
{
#ifdef CM_PASN_DBG
	" czMeasurementInitiationResult_Lst ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztMeasurementInitiationResult_Lst),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czMeasurementInitiationResult_Lstspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueResStatusResp_IEsClsInst[] =
{
	&czMeasurement_ID,
	&czMeasurement_ID,
	&czCriticalityDiag,
	&czMeasurementInitiationResult_Lst,
		&czProtIE_Field_MeasurementInitiationResult_ItemIEs,
		&czConstTerm,
	
};
PUBLIC U32 cz_ValueResStatusResp_IEsClsInstId[] =
{
	Cztid_ENB1_Measurement_ID,
	Cztid_ENB2_Measurement_ID,
	Cztid_CriticalityDiag,
	Cztid_MeasurementInitiationResult_Lst,
		Cztid_Invalid,
		Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ValueResStatusResp_IEsClsInfo =
{
	6,
	&cz_ValueResStatusResp_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueResStatusResp_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueResStatusResp_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueResStatusResp_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueResStatusResp_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueResStatusResp_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueResStatusResp_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueResStatusResp_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_ResStatusResp_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueResStatusResp_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_ResStatusResp_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_ResStatusResp_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_ResStatusResp_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_ResStatusResp_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_ResStatusResp_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_ResStatusResp_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_ResStatusResp_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_ResStatusResp_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_ResStatusResp_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_ResStatusResp_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_ResStatusResp_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czResStatusRespTkn[] =
{
	&czProtIE_Cont_ResStatusResp_IEs,
		&czProtIE_Field_ResStatusResp_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czResStatusRespspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czResStatusRespTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czResStatusResp =
{
#ifdef CM_PASN_DBG
	" czResStatusResp ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztResStatusResp),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czResStatusRespspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnCompleteFailCauseInform_Item_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnCompleteFailCauseInform_Item_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnCompleteFailCauseInform_Item_ExtIEsClsInfo =
{
	0,
	&cz_ExtnCompleteFailCauseInform_Item_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnCompleteFailCauseInform_Item_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnCompleteFailCauseInform_Item_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnCompleteFailCauseInform_Item_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnCompleteFailCauseInform_Item_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnCompleteFailCauseInform_Item_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnCompleteFailCauseInform_Item_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnCompleteFailCauseInform_Item_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_CompleteFailCauseInform_Item_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnCompleteFailCauseInform_Item_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_CompleteFailCauseInform_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_CompleteFailCauseInform_Item_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_CompleteFailCauseInform_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_CompleteFailCauseInform_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_CompleteFailCauseInform_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_CompleteFailCauseInform_Item_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_CompleteFailCauseInform_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_CompleteFailCauseInform_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_CompleteFailCauseInform_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_CompleteFailCauseInform_Item_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_CompleteFailCauseInform_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_CompleteFailCauseInform_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_CompleteFailCauseInform_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_CompleteFailCauseInform_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_CompleteFailCauseInform_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czCompleteFailCauseInform_ItemTkn[] =
{
	&czECGI,
	&czMeasurementFailCause_Lst,
		&czProtIE_Field_MeasurementFailCause_ItemIEs,
		&czConstTerm,
	&czOptProtExtnCont_CompleteFailCauseInform_Item_ExtIEs,
		&czProtExtnField_CompleteFailCauseInform_Item_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czCompleteFailCauseInform_Itemspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czCompleteFailCauseInform_ItemTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czCompleteFailCauseInform_Item =
{
#ifdef CM_PASN_DBG
	" czCompleteFailCauseInform_Item ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztCompleteFailCauseInform_Item),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czCompleteFailCauseInform_Itemspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueCompleteFailCauseInform_ItemIEsClsInst[] =
{
	&czCompleteFailCauseInform_Item
};
PUBLIC U32 cz_ValueCompleteFailCauseInform_ItemIEsClsInstId[] =
{
	Cztid_CompleteFailCauseInform_Item
};
PUBLIC CmPAsnClassInfo cz_ValueCompleteFailCauseInform_ItemIEsClsInfo =
{
	1,
	&cz_ValueCompleteFailCauseInform_ItemIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueCompleteFailCauseInform_ItemIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueCompleteFailCauseInform_ItemIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueCompleteFailCauseInform_ItemIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueCompleteFailCauseInform_ItemIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueCompleteFailCauseInform_ItemIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueCompleteFailCauseInform_ItemIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueCompleteFailCauseInform_ItemIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_CompleteFailCauseInform_ItemIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueCompleteFailCauseInform_ItemIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_CompleteFailCauseInform_ItemIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_CompleteFailCauseInform_ItemIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_CompleteFailCauseInform_ItemIEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_CompleteFailCauseInform_ItemIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_CompleteFailCauseInform_ItemIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_CompleteFailCauseInform_ItemIEsspec),
	NULLP
};
#define czProtIE_Single_Cont_CompleteFailCauseInform_ItemIEs czProtIE_Field_CompleteFailCauseInform_ItemIEs
CmPAsnSetSeqOfDef czCompleteFailCauseInform_Lstspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxCellineNB,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czCompleteFailCauseInform_Lst =
{
#ifdef CM_PASN_DBG
	" czCompleteFailCauseInform_Lst ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztCompleteFailCauseInform_Lst),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czCompleteFailCauseInform_Lstspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueResStatusFail_IEsClsInst[] =
{
	&czMeasurement_ID,
	&czMeasurement_ID,
	&czCause,
	&czCriticalityDiag,
	&czCompleteFailCauseInform_Lst,
		&czProtIE_Field_CompleteFailCauseInform_ItemIEs,
		&czConstTerm,
	
};
PUBLIC U32 cz_ValueResStatusFail_IEsClsInstId[] =
{
	Cztid_ENB1_Measurement_ID,
	Cztid_ENB2_Measurement_ID,
	Cztid_Cause,
	Cztid_CriticalityDiag,
	Cztid_CompleteFailCauseInform_Lst,
		Cztid_Invalid,
		Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ValueResStatusFail_IEsClsInfo =
{
	7,
	&cz_ValueResStatusFail_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueResStatusFail_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueResStatusFail_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueResStatusFail_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueResStatusFail_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueResStatusFail_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueResStatusFail_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueResStatusFail_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_ResStatusFail_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueResStatusFail_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_ResStatusFail_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_ResStatusFail_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_ResStatusFail_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_ResStatusFail_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_ResStatusFail_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_ResStatusFail_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_ResStatusFail_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_ResStatusFail_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_ResStatusFail_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_ResStatusFail_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_ResStatusFail_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czResStatusFailTkn[] =
{
	&czProtIE_Cont_ResStatusFail_IEs,
		&czProtIE_Field_ResStatusFail_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czResStatusFailspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czResStatusFailTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czResStatusFail =
{
#ifdef CM_PASN_DBG
	" czResStatusFail ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztResStatusFail),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czResStatusFailspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnCellMeasurementResult_Item_ExtIEsClsInst[] =
{
	&czCompositeAvailableCapacityGroup,
	&czABS_Status
};
PUBLIC U32 cz_ExtnCellMeasurementResult_Item_ExtIEsClsInstId[] =
{
	Cztid_CompositeAvailableCapacityGroup,
	Cztid_ABS_Status
};
PUBLIC CmPAsnClassInfo cz_ExtnCellMeasurementResult_Item_ExtIEsClsInfo =
{
	2,
	&cz_ExtnCellMeasurementResult_Item_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnCellMeasurementResult_Item_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnCellMeasurementResult_Item_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnCellMeasurementResult_Item_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnCellMeasurementResult_Item_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnCellMeasurementResult_Item_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnCellMeasurementResult_Item_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnCellMeasurementResult_Item_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_CellMeasurementResult_Item_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnCellMeasurementResult_Item_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_CellMeasurementResult_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_CellMeasurementResult_Item_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_CellMeasurementResult_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_CellMeasurementResult_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_CellMeasurementResult_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_CellMeasurementResult_Item_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_CellMeasurementResult_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_CellMeasurementResult_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_CellMeasurementResult_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_CellMeasurementResult_Item_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_CellMeasurementResult_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_CellMeasurementResult_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_CellMeasurementResult_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_CellMeasurementResult_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_CellMeasurementResult_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czCellMeasurementResult_ItemTkn[] =
{
	&czECGI,
	&czOptHWLoadIndicator,
	&czOptS1TNLLoadIndicator,
	&czOptRadioResStatus,
	&czOptProtExtnCont_CellMeasurementResult_Item_ExtIEs,
		&czProtExtnField_CellMeasurementResult_Item_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czCellMeasurementResult_Itemspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	4,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czCellMeasurementResult_ItemTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czCellMeasurementResult_Item =
{
#ifdef CM_PASN_DBG
	" czCellMeasurementResult_Item ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztCellMeasurementResult_Item),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czCellMeasurementResult_Itemspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueCellMeasurementResult_ItemIEsClsInst[] =
{
	&czCellMeasurementResult_Item
};
PUBLIC U32 cz_ValueCellMeasurementResult_ItemIEsClsInstId[] =
{
	Cztid_CellMeasurementResult_Item
};
PUBLIC CmPAsnClassInfo cz_ValueCellMeasurementResult_ItemIEsClsInfo =
{
	1,
	&cz_ValueCellMeasurementResult_ItemIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueCellMeasurementResult_ItemIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueCellMeasurementResult_ItemIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueCellMeasurementResult_ItemIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueCellMeasurementResult_ItemIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueCellMeasurementResult_ItemIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueCellMeasurementResult_ItemIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueCellMeasurementResult_ItemIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_CellMeasurementResult_ItemIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueCellMeasurementResult_ItemIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_CellMeasurementResult_ItemIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_CellMeasurementResult_ItemIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_CellMeasurementResult_ItemIEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_CellMeasurementResult_ItemIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_CellMeasurementResult_ItemIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_CellMeasurementResult_ItemIEsspec),
	NULLP
};
#define czProtIE_Single_Cont_CellMeasurementResult_ItemIEs czProtIE_Field_CellMeasurementResult_ItemIEs
CmPAsnSetSeqOfDef czCellMeasurementResult_Lstspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxCellineNB,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czCellMeasurementResult_Lst =
{
#ifdef CM_PASN_DBG
	" czCellMeasurementResult_Lst ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztCellMeasurementResult_Lst),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czCellMeasurementResult_Lstspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueResStatusUpd_IEsClsInst[] =
{
	&czMeasurement_ID,
	&czMeasurement_ID,
	&czCellMeasurementResult_Lst,
		&czProtIE_Field_CellMeasurementResult_ItemIEs,
		&czConstTerm,
	
};
PUBLIC U32 cz_ValueResStatusUpd_IEsClsInstId[] =
{
	Cztid_ENB1_Measurement_ID,
	Cztid_ENB2_Measurement_ID,
	Cztid_CellMeasurementResult,
		Cztid_Invalid,
		Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ValueResStatusUpd_IEsClsInfo =
{
	5,
	&cz_ValueResStatusUpd_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueResStatusUpd_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueResStatusUpd_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueResStatusUpd_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueResStatusUpd_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueResStatusUpd_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueResStatusUpd_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueResStatusUpd_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_ResStatusUpd_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueResStatusUpd_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_ResStatusUpd_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_ResStatusUpd_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_ResStatusUpd_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_ResStatusUpd_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_ResStatusUpd_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_ResStatusUpd_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_ResStatusUpd_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_ResStatusUpd_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_ResStatusUpd_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_ResStatusUpd_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_ResStatusUpd_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czResStatusUpdTkn[] =
{
	&czProtIE_Cont_ResStatusUpd_IEs,
		&czProtIE_Field_ResStatusUpd_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czResStatusUpdspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czResStatusUpdTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czResStatusUpd =
{
#ifdef CM_PASN_DBG
	" czResStatusUpd ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztResStatusUpd),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czResStatusUpdspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValuePrivMsg_IEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ValuePrivMsg_IEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ValuePrivMsg_IEsClsInfo =
{
	0,
	&cz_ValuePrivMsg_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValuePrivMsg_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValuePrivMsg_IEsClsInfo,
	0,
	&cz_ValuePrivMsg_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValuePrivMsg_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValuePrivMsg_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValuePrivMsg_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValuePrivMsg_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czPrivIE_Field_PrivMsg_IEsTkn[] =
{
	&czPrivIE_ID,
	&czCriticality,
	&cz_ValuePrivMsg_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czPrivIE_Field_PrivMsg_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czPrivIE_Field_PrivMsg_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czPrivIE_Field_PrivMsg_IEs =
{
#ifdef CM_PASN_DBG
	" czPrivIE_Field_PrivMsg_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztPrivIE_Field_PrivMsg_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czPrivIE_Field_PrivMsg_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czPrivIE_Cont_PrivMsg_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxPrivIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czPrivIE_Cont_PrivMsg_IEs =
{
#ifdef CM_PASN_DBG
	" czPrivIE_Cont_PrivMsg_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztPrivIE_Cont_PrivMsg_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czPrivIE_Cont_PrivMsg_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czPrivMsgTkn[] =
{
	&czPrivIE_Cont_PrivMsg_IEs,
		&czPrivIE_Field_PrivMsg_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czPrivMsgspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czPrivMsgTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czPrivMsg =
{
#ifdef CM_PASN_DBG
	" czPrivMsg ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztPrivMsg),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czPrivMsgspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueMobilityChangeRqst_IEsClsInst[] =
{
	&czECGI,
	&czECGI,
	&czMobilityParamsInform,
	&czMobilityParamsInform,
	&czCause
};
PUBLIC U32 cz_ValueMobilityChangeRqst_IEsClsInstId[] =
{
	Cztid_ENB1_Cell_ID,
	Cztid_ENB2_Cell_ID,
	Cztid_ENB1_Mobility_Params,
	Cztid_ENB2_Proposed_Mobility_Params,
	Cztid_Cause
};
PUBLIC CmPAsnClassInfo cz_ValueMobilityChangeRqst_IEsClsInfo =
{
	5,
	&cz_ValueMobilityChangeRqst_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueMobilityChangeRqst_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueMobilityChangeRqst_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueMobilityChangeRqst_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueMobilityChangeRqst_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueMobilityChangeRqst_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueMobilityChangeRqst_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueMobilityChangeRqst_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_MobilityChangeRqst_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueMobilityChangeRqst_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_MobilityChangeRqst_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_MobilityChangeRqst_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_MobilityChangeRqst_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_MobilityChangeRqst_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_MobilityChangeRqst_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_MobilityChangeRqst_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_MobilityChangeRqst_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_MobilityChangeRqst_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_MobilityChangeRqst_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_MobilityChangeRqst_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_MobilityChangeRqst_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czMobilityChangeRqstTkn[] =
{
	&czProtIE_Cont_MobilityChangeRqst_IEs,
		&czProtIE_Field_MobilityChangeRqst_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czMobilityChangeRqstspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czMobilityChangeRqstTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czMobilityChangeRqst =
{
#ifdef CM_PASN_DBG
	" czMobilityChangeRqst ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztMobilityChangeRqst),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czMobilityChangeRqstspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueMobilityChangeAckg_IEsClsInst[] =
{
	&czECGI,
	&czECGI,
	&czCriticalityDiag
};
PUBLIC U32 cz_ValueMobilityChangeAckg_IEsClsInstId[] =
{
	Cztid_ENB1_Cell_ID,
	Cztid_ENB2_Cell_ID,
	Cztid_CriticalityDiag
};
PUBLIC CmPAsnClassInfo cz_ValueMobilityChangeAckg_IEsClsInfo =
{
	3,
	&cz_ValueMobilityChangeAckg_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueMobilityChangeAckg_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueMobilityChangeAckg_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueMobilityChangeAckg_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueMobilityChangeAckg_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueMobilityChangeAckg_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueMobilityChangeAckg_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueMobilityChangeAckg_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_MobilityChangeAckg_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueMobilityChangeAckg_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_MobilityChangeAckg_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_MobilityChangeAckg_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_MobilityChangeAckg_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_MobilityChangeAckg_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_MobilityChangeAckg_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_MobilityChangeAckg_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_MobilityChangeAckg_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_MobilityChangeAckg_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_MobilityChangeAckg_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_MobilityChangeAckg_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_MobilityChangeAckg_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czMobilityChangeAckgTkn[] =
{
	&czProtIE_Cont_MobilityChangeAckg_IEs,
		&czProtIE_Field_MobilityChangeAckg_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czMobilityChangeAckgspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czMobilityChangeAckgTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czMobilityChangeAckg =
{
#ifdef CM_PASN_DBG
	" czMobilityChangeAckg ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztMobilityChangeAckg),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czMobilityChangeAckgspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueMobilityChangeFail_IEsClsInst[] =
{
	&czECGI,
	&czECGI,
	&czCause,
	&czMobilityParamsModificationRange,
	&czCriticalityDiag
};
PUBLIC U32 cz_ValueMobilityChangeFail_IEsClsInstId[] =
{
	Cztid_ENB1_Cell_ID,
	Cztid_ENB2_Cell_ID,
	Cztid_Cause,
	Cztid_ENB2_Mobility_Params_Modification_Range,
	Cztid_CriticalityDiag
};
PUBLIC CmPAsnClassInfo cz_ValueMobilityChangeFail_IEsClsInfo =
{
	5,
	&cz_ValueMobilityChangeFail_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueMobilityChangeFail_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueMobilityChangeFail_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueMobilityChangeFail_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueMobilityChangeFail_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueMobilityChangeFail_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueMobilityChangeFail_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueMobilityChangeFail_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_MobilityChangeFail_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueMobilityChangeFail_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_MobilityChangeFail_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_MobilityChangeFail_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_MobilityChangeFail_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_MobilityChangeFail_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_MobilityChangeFail_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_MobilityChangeFail_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_MobilityChangeFail_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_MobilityChangeFail_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_MobilityChangeFail_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_MobilityChangeFail_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_MobilityChangeFail_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czMobilityChangeFailTkn[] =
{
	&czProtIE_Cont_MobilityChangeFail_IEs,
		&czProtIE_Field_MobilityChangeFail_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czMobilityChangeFailspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czMobilityChangeFailTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czMobilityChangeFail =
{
#ifdef CM_PASN_DBG
	" czMobilityChangeFail ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztMobilityChangeFail),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czMobilityChangeFailspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueRLFInd_IEsClsInst[] =
{
	&czPCI,
	&czECGI,
	&czCRNTI,
	&czShortMAC_I,
	&czUE_RLF_Report_Cont,
	&czRRCConnSetupIndicator,
	&czRRCConnReestabIndicator
};
PUBLIC U32 cz_ValueRLFInd_IEsClsInstId[] =
{
	Cztid_FailCellPCI,
	Cztid_Re_establishmentCellECGI,
	Cztid_FailCellCRNTI,
	Cztid_ShortMAC_I,
	Cztid_UE_RLF_Report_Cont,
	Cztid_RRCConnSetupIndicator,
	Cztid_RRCConnReestabIndicator
};
PUBLIC CmPAsnClassInfo cz_ValueRLFInd_IEsClsInfo =
{
	7,
	&cz_ValueRLFInd_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueRLFInd_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueRLFInd_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueRLFInd_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueRLFInd_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueRLFInd_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueRLFInd_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueRLFInd_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_RLFInd_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueRLFInd_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_RLFInd_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_RLFInd_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_RLFInd_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_RLFInd_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_RLFInd_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_RLFInd_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_RLFInd_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_RLFInd_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_RLFInd_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_RLFInd_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_RLFInd_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czRLFIndTkn[] =
{
	&czProtIE_Cont_RLFInd_IEs,
		&czProtIE_Field_RLFInd_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czRLFIndspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czRLFIndTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czRLFInd =
{
#ifdef CM_PASN_DBG
	" czRLFInd ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztRLFInd),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czRLFIndspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnSrvdCellsToActivate_Item_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnSrvdCellsToActivate_Item_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnSrvdCellsToActivate_Item_ExtIEsClsInfo =
{
	0,
	&cz_ExtnSrvdCellsToActivate_Item_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnSrvdCellsToActivate_Item_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnSrvdCellsToActivate_Item_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnSrvdCellsToActivate_Item_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnSrvdCellsToActivate_Item_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnSrvdCellsToActivate_Item_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnSrvdCellsToActivate_Item_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnSrvdCellsToActivate_Item_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_SrvdCellsToActivate_Item_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnSrvdCellsToActivate_Item_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_SrvdCellsToActivate_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_SrvdCellsToActivate_Item_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_SrvdCellsToActivate_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_SrvdCellsToActivate_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_SrvdCellsToActivate_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_SrvdCellsToActivate_Item_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_SrvdCellsToActivate_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_SrvdCellsToActivate_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_SrvdCellsToActivate_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_SrvdCellsToActivate_Item_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_SrvdCellsToActivate_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_SrvdCellsToActivate_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_SrvdCellsToActivate_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_SrvdCellsToActivate_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_SrvdCellsToActivate_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czSrvdCellsToActivate_ItemTkn[] =
{
	&czECGI,
	&czOptProtExtnCont_SrvdCellsToActivate_Item_ExtIEs,
		&czProtExtnField_SrvdCellsToActivate_Item_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czSrvdCellsToActivate_Itemspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czSrvdCellsToActivate_ItemTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czSrvdCellsToActivate_Item =
{
#ifdef CM_PASN_DBG
	" czSrvdCellsToActivate_Item ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztSrvdCellsToActivate_Item),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czSrvdCellsToActivate_Itemspec),
	NULLP
};
CmPAsnSetSeqOfDef czSrvdCellsToActivatespec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxCellineNB,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czSrvdCellsToActivate =
{
#ifdef CM_PASN_DBG
	" czSrvdCellsToActivate ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztSrvdCellsToActivate),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czSrvdCellsToActivatespec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueCellActvnRqst_IEsClsInst[] =
{
	&czSrvdCellsToActivate,
		&czSrvdCellsToActivate_Item,
		&czConstTerm,
	
};
PUBLIC U32 cz_ValueCellActvnRqst_IEsClsInstId[] =
{
	Cztid_SrvdCellsToActivate,
		Cztid_Invalid,
		Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ValueCellActvnRqst_IEsClsInfo =
{
	3,
	&cz_ValueCellActvnRqst_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueCellActvnRqst_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueCellActvnRqst_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueCellActvnRqst_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueCellActvnRqst_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueCellActvnRqst_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueCellActvnRqst_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueCellActvnRqst_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_CellActvnRqst_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueCellActvnRqst_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_CellActvnRqst_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_CellActvnRqst_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_CellActvnRqst_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_CellActvnRqst_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_CellActvnRqst_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_CellActvnRqst_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_CellActvnRqst_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_CellActvnRqst_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_CellActvnRqst_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_CellActvnRqst_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_CellActvnRqst_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czCellActvnRqstTkn[] =
{
	&czProtIE_Cont_CellActvnRqst_IEs,
		&czProtIE_Field_CellActvnRqst_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czCellActvnRqstspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czCellActvnRqstTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czCellActvnRqst =
{
#ifdef CM_PASN_DBG
	" czCellActvnRqst ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztCellActvnRqst),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czCellActvnRqstspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ExtnActivatedCellLst_Item_ExtIEsClsInst[] =
{
	NULLP
};
PUBLIC U32 cz_ExtnActivatedCellLst_Item_ExtIEsClsInstId[] =
{
	Cztid_Invalid
};
PUBLIC CmPAsnClassInfo cz_ExtnActivatedCellLst_Item_ExtIEsClsInfo =
{
	0,
	&cz_ExtnActivatedCellLst_Item_ExtIEsClsInstId[0]
};
CmPAsnClassIEDef cz_ExtnActivatedCellLst_Item_ExtIEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ExtnActivatedCellLst_Item_ExtIEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ExtnActivatedCellLst_Item_ExtIEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ExtnActivatedCellLst_Item_ExtIEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ExtnActivatedCellLst_Item_ExtIEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ExtnActivatedCellLst_Item_ExtIEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ExtnActivatedCellLst_Item_ExtIEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtExtnField_ActivatedCellLst_Item_ExtIEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ExtnActivatedCellLst_Item_ExtIEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtExtnField_ActivatedCellLst_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtExtnField_ActivatedCellLst_Item_ExtIEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtExtnField_ActivatedCellLst_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnField_ActivatedCellLst_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnField_ActivatedCellLst_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtExtnField_ActivatedCellLst_Item_ExtIEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtExtnCont_ActivatedCellLst_Item_ExtIEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxProtExtns,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czOptProtExtnCont_ActivatedCellLst_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_ActivatedCellLst_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_ActivatedCellLst_Item_ExtIEs),
	&flagCz3O,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_ActivatedCellLst_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef czProtExtnCont_ActivatedCellLst_Item_ExtIEs =
{
#ifdef CM_PASN_DBG
	" czProtExtnCont_ActivatedCellLst_Item_ExtIEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtExtnCont_ActivatedCellLst_Item_ExtIEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtExtnCont_ActivatedCellLst_Item_ExtIEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czActivatedCellLst_ItemTkn[] =
{
	&czECGI,
	&czOptProtExtnCont_ActivatedCellLst_Item_ExtIEs,
		&czProtExtnField_ActivatedCellLst_Item_ExtIEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czActivatedCellLst_Itemspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	1,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czActivatedCellLst_ItemTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czActivatedCellLst_Item =
{
#ifdef CM_PASN_DBG
	" czActivatedCellLst_Item ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztActivatedCellLst_Item),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czActivatedCellLst_Itemspec),
	NULLP
};
CmPAsnSetSeqOfDef czActivatedCellLstspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	1,        /* start of the size */
	CztmaxCellineNB,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czActivatedCellLst =
{
#ifdef CM_PASN_DBG
	" czActivatedCellLst ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztActivatedCellLst),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czActivatedCellLstspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueCellActvnResp_IEsClsInst[] =
{
	&czActivatedCellLst,
		&czActivatedCellLst_Item,
		&czConstTerm,
	&czCriticalityDiag
};
PUBLIC U32 cz_ValueCellActvnResp_IEsClsInstId[] =
{
	Cztid_ActivatedCellLst,
		Cztid_Invalid,
		Cztid_Invalid,
	Cztid_CriticalityDiag
};
PUBLIC CmPAsnClassInfo cz_ValueCellActvnResp_IEsClsInfo =
{
	4,
	&cz_ValueCellActvnResp_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueCellActvnResp_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueCellActvnResp_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueCellActvnResp_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueCellActvnResp_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueCellActvnResp_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueCellActvnResp_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueCellActvnResp_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_CellActvnResp_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueCellActvnResp_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_CellActvnResp_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_CellActvnResp_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_CellActvnResp_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_CellActvnResp_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_CellActvnResp_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_CellActvnResp_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_CellActvnResp_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_CellActvnResp_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_CellActvnResp_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_CellActvnResp_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_CellActvnResp_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czCellActvnRespTkn[] =
{
	&czProtIE_Cont_CellActvnResp_IEs,
		&czProtIE_Field_CellActvnResp_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czCellActvnRespspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czCellActvnRespTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czCellActvnResp =
{
#ifdef CM_PASN_DBG
	" czCellActvnResp ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztCellActvnResp),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czCellActvnRespspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_ValueCellActvnFail_IEsClsInst[] =
{
	&czCause,
	&czCriticalityDiag
};
PUBLIC U32 cz_ValueCellActvnFail_IEsClsInstId[] =
{
	Cztid_Cause,
	Cztid_CriticalityDiag
};
PUBLIC CmPAsnClassInfo cz_ValueCellActvnFail_IEsClsInfo =
{
	2,
	&cz_ValueCellActvnFail_IEsClsInstId[0]
};
CmPAsnClassIEDef cz_ValueCellActvnFail_IEsClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_ValueCellActvnFail_IEsClsInfo,
	(sizeof(CztProtIE_ID) + sizeof(CztCriticality)),
	&cz_ValueCellActvnFail_IEsClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_ValueCellActvnFail_IEsCls =
{
#ifdef CM_PASN_DBG
	" cz_ValueCellActvnFail_IEsCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_ValueCellActvnFail_IEsCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_ValueCellActvnFail_IEsClsSpec)
};
PUBLIC CmPAsnElmDef *czProtIE_Field_CellActvnFail_IEsTkn[] =
{
	&czProtIE_ID,
	&czCriticality,
	&cz_ValueCellActvnFail_IEsCls,
	&czConstTerm
};
CmPAsnSetSeqDef czProtIE_Field_CellActvnFail_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czProtIE_Field_CellActvnFail_IEsTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czProtIE_Field_CellActvnFail_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Field_CellActvnFail_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Field_CellActvnFail_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czProtIE_Field_CellActvnFail_IEsspec),
	NULLP
};
CmPAsnSetSeqOfDef czProtIE_Cont_CellActvnFail_IEsspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQOF,
#endif /* CM_PASN_DEBUG */
	0,        /* start of the size */
	CztmaxProtIEs,        /* start of the size */
	3*sizeof(PTR)
};
PUBLIC CmPAsnElmDef czProtIE_Cont_CellActvnFail_IEs =
{
#ifdef CM_PASN_DBG
	" czProtIE_Cont_CellActvnFail_IEs ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQOF,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztProtIE_Cont_CellActvnFail_IEs),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *) (&czProtIE_Cont_CellActvnFail_IEsspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czCellActvnFailTkn[] =
{
	&czProtIE_Cont_CellActvnFail_IEs,
		&czProtIE_Field_CellActvnFail_IEs,
		&czConstTerm,
	&czExtMkr,
	&czConstTerm
};
CmPAsnSetSeqDef czCellActvnFailspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czCellActvnFailTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czCellActvnFail =
{
#ifdef CM_PASN_DBG
	" czCellActvnFail ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztCellActvnFail),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czCellActvnFailspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_InitiatingMsgClsInst[] =
{
	&czHovrRqst,
	&czResetRqst,
	&czX2SetupRqst,
	&czResStatusRqst,
	&czENBConfigUpd,
	&czMobilityChangeRqst,
	&czCellActvnRqst,
	&czSNStatusTfr,
	&czUECntxtRls,
	&czHovrCancel,
	&czErrInd,
	&czResStatusUpd,
	&czLoadInform,
	&czPrivMsg,
	&czRLFInd,
	&czHovrReport
};
PUBLIC U32 cz_InitiatingMsgClsInstId[] =
{
	Cztid_handoverPrep,
	Cztid_reset,
	Cztid_x2Setup,
	Cztid_resourceStatusRprtngInitiation,
	Cztid_eNBConfigUpd,
	Cztid_mobilitySettingsChange,
	Cztid_cellActvn,
	Cztid_snStatusTfr,
	Cztid_uECntxtRls,
	Cztid_handoverCancel,
	Cztid_errorInd,
	Cztid_resourceStatusRprtng,
	Cztid_loadInd,
	Cztid_privateMsg,
	Cztid_rLFInd,
	Cztid_handoverReport
};
PUBLIC CmPAsnClassInfo cz_InitiatingMsgClsInfo =
{
	16,
	&cz_InitiatingMsgClsInstId[0]
};
CmPAsnClassIEDef cz_InitiatingMsgClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_InitiatingMsgClsInfo,
	(sizeof(CztProcedureCode) + sizeof(CztCriticality)),
	&cz_InitiatingMsgClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_InitiatingMsgCls =
{
#ifdef CM_PASN_DBG
	" cz_InitiatingMsgCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_InitiatingMsgCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_InitiatingMsgClsSpec)
};
PUBLIC CmPAsnElmDef *czInitiatingMsgTkn[] =
{
	&czProcedureCode,
	&czCriticality,
	&cz_InitiatingMsgCls,
	&czConstTerm
};
CmPAsnSetSeqDef czInitiatingMsgspec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czInitiatingMsgTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czInitiatingMsg =
{
#ifdef CM_PASN_DBG
	" czInitiatingMsg ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztInitiatingMsg),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czInitiatingMsgspec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_SuccessfulOutcomeClsInst[] =
{
	&czHovrRqstAckg,
	&czResetResp,
	&czX2SetupResp,
	&czResStatusResp,
	&czENBConfigUpdAckg,
	&czMobilityChangeAckg,
	&czCellActvnResp
};
PUBLIC U32 cz_SuccessfulOutcomeClsInstId[] =
{
	Cztid_handoverPrep,
	Cztid_reset,
	Cztid_x2Setup,
	Cztid_resourceStatusRprtngInitiation,
	Cztid_eNBConfigUpd,
	Cztid_mobilitySettingsChange,
	Cztid_cellActvn
};
PUBLIC CmPAsnClassInfo cz_SuccessfulOutcomeClsInfo =
{
	7,
	&cz_SuccessfulOutcomeClsInstId[0]
};
CmPAsnClassIEDef cz_SuccessfulOutcomeClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_SuccessfulOutcomeClsInfo,
	(sizeof(CztProcedureCode) + sizeof(CztCriticality)),
	&cz_SuccessfulOutcomeClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_SuccessfulOutcomeCls =
{
#ifdef CM_PASN_DBG
	" cz_SuccessfulOutcomeCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_SuccessfulOutcomeCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_SuccessfulOutcomeClsSpec)
};
PUBLIC CmPAsnElmDef *czSuccessfulOutcomeTkn[] =
{
	&czProcedureCode,
	&czCriticality,
	&cz_SuccessfulOutcomeCls,
	&czConstTerm
};
CmPAsnSetSeqDef czSuccessfulOutcomespec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czSuccessfulOutcomeTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czSuccessfulOutcome =
{
#ifdef CM_PASN_DBG
	" czSuccessfulOutcome ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztSuccessfulOutcome),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czSuccessfulOutcomespec),
	NULLP
};
PUBLIC CmPAsnElmDef *cz_UnsuccessfulOutcomeClsInst[] =
{
	&czHovrPrepFail,
	&czX2SetupFail,
	&czResStatusFail,
	&czENBConfigUpdFail,
	&czMobilityChangeFail,
	&czCellActvnFail
};
PUBLIC U32 cz_UnsuccessfulOutcomeClsInstId[] =
{
	Cztid_handoverPrep,
	Cztid_x2Setup,
	Cztid_resourceStatusRprtngInitiation,
	Cztid_eNBConfigUpd,
	Cztid_mobilitySettingsChange,
	Cztid_cellActvn
};
PUBLIC CmPAsnClassInfo cz_UnsuccessfulOutcomeClsInfo =
{
	6,
	&cz_UnsuccessfulOutcomeClsInstId[0]
};
CmPAsnClassIEDef cz_UnsuccessfulOutcomeClsSpec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CLASSIE,
#endif /* CM_PASN_DEBUG */
	&cz_UnsuccessfulOutcomeClsInfo,
	(sizeof(CztProcedureCode) + sizeof(CztCriticality)),
	&cz_UnsuccessfulOutcomeClsInst[0],
#ifdef CM_PASN_CLASS_IDTYPE
	CM_PASN_TET_UINT32
#endif
};
PUBLIC CmPAsnElmDef cz_UnsuccessfulOutcomeCls =
{
#ifdef CM_PASN_DBG
	" cz_UnsuccessfulOutcomeCls ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CLASSIE,
	CM_PASN_EXT_MKR_ABS,
	sizeof(Czt_UnsuccessfulOutcomeCls),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&cz_UnsuccessfulOutcomeClsSpec)
};
PUBLIC CmPAsnElmDef *czUnsuccessfulOutcomeTkn[] =
{
	&czProcedureCode,
	&czCriticality,
	&cz_UnsuccessfulOutcomeCls,
	&czConstTerm
};
CmPAsnSetSeqDef czUnsuccessfulOutcomespec =
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_SEQ,
#endif /* CM_PASN_DEBUG */
	0,      /* number of optional elements */
	0,      /* number of extension elements */
	NULLP, /* not supported yet */
	NULLP, /* not supported yet */
	&czUnsuccessfulOutcomeTkn[0], /* ptr to first token*/
	FALSE      /* Event Memory Allocated */
};
PUBLIC CmPAsnElmDef czUnsuccessfulOutcome =
{
#ifdef CM_PASN_DBG
	" czUnsuccessfulOutcome ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_SEQ,
	CM_PASN_EXT_MKR_ABS,
	sizeof(CztUnsuccessfulOutcome),
	&flagCz3M,
	CM_PASN_ESC_FUNC_NULLP,
	(U8 *)(&czUnsuccessfulOutcomespec),
	NULLP
};
PUBLIC CmPAsnElmDef *czX2AP_PDUTkn[] =
{
	&czInitiatingMsg,
	&czSuccessfulOutcome,
	&czUnsuccessfulOutcome,
	&czExtMkr,
	&czConstTerm
};
CmPAsnChoiceDef czX2AP_PDUspec = 
{
#ifdef CM_PASN_DBG
	CM_PASN_TET_CHOICE,
#endif
	3,
	0,
	&czX2AP_PDUTkn[0]  /* Token List */
};
CmPAsnElmDef czX2AP_PDU =
{
#ifdef CM_PASN_DBG
	" czX2AP_PDU ",
#endif /* CM_PASN_DEBUG */
	CM_PASN_TET_CHOICE,
	CM_PASN_EXT_MKR_PRES,
	sizeof(CztX2AP_PDU),
	&flagCz3M,
	NULLP,
	(U8 *)(&czX2AP_PDUspec),
	NULLP
};
PUBLIC CmPAsnElmDef *czMsgDb[] =
{
   &czX2AP_PDU,
   NULLP
};

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
$SID$        ---      chebli         1. Updated DB for X2AP R11 for TotaleNodeB GA3.0
$SID$        ---      chebli         1. Updated DB for X2AP R11.8.0 for TotaleNodeB GA5.0
*********************************************************************91*/


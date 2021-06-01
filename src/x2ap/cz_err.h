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

     Name:     X2AP Layer

     Type:     C source file

     Desc:     C Source code for error definitions

     File:     cz_err.h

     Sid:      cz_err.h@@/main/2 - Tue Aug 30 18:36:04 2011

     Prg:      sy

*********************************************************************21*/

#ifndef __CZERRH__
#define __CZERRH__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef SLOGERROR
#define SLOGERROR  SLogError
#endif /* SLOGERROR */

#ifdef SS_MULTIPLE_PROCS

#define CZLOGERROR(errCls, errCode, errVal, errDesc) \
        SLogError(czCbPtr->init.ent, czCbPtr->init.inst, czCbPtr->init.procId, \
                   __FILE__, __LINE__, errCls, errCode, errVal, errDesc)

#if (ERRCLASS & ERRCLS_DEBUG)    
#define CZLOGERROR_DEBUG(errCode, errVal, errDesc) \
        SLogError(czCbPtr->init.ent, czCbPtr->init.inst, czCbPtr->init.procId, \
                   __FILE__, __LINE__, ERRCLS_DEBUG, errCode, errVal, errDesc)
#else /* (ERRCLASS & ERRCLS_DEBUG) */
#define CZLOGERROR_DEBUG(errCode, errVal, errDesc) 
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

#if (ERRCLASS & ERRCLS_INT_PAR)    
#define CZLOGERROR_INTFC(errCode, errVal, errDesc) \
        SLogError(czCbPtr->init.ent, czCbPtr->init.inst, czCbPtr->init.procId, \
                   __FILE__, __LINE__, ERRCLS_INT_PAR, errCode, errVal, errDesc)
#else /* (ERRCLASS & ERRCLS_INT_PAR) */
#define CZLOGERROR_INTFC(errCode, errVal, errDesc) 
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

#if (ERRCLASS & ERRCLS_ADD_RES)    
#define CZLOGERROR_ADD_RES(errCode, errVal, errDesc) \
        SLogError(czCbPtr->init.ent, czCbPtr->init.inst, czCbPtr->init.procId, \
                   __FILE__, __LINE__, ERRCLS_ADD_RES, errCode, errVal, errDesc)
#else /* (ERRCLASS & ERRCLS_INT_PAR) */
#define CZLOGERROR_ADD_RES(errCode, errVal, errDesc) 
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

#else /* SS_MULTIPLE_PROCS */

#define CZLOGERROR(errCls, errCode, errVal, errDesc) \
        SLogError(czCb.init.ent, czCb.init.inst, czCb.init.procId, \
                   __FILE__, __LINE__, errCls, errCode, errVal, errDesc)

#if (ERRCLASS & ERRCLS_DEBUG)    
#define CZLOGERROR_DEBUG(errCode, errVal, errDesc) \
        SLogError(czCb.init.ent, czCb.init.inst, czCb.init.procId, \
                   __FILE__, __LINE__, ERRCLS_DEBUG, errCode, errVal, errDesc)
#else /* (ERRCLASS & ERRCLS_DEBUG) */
#define CZLOGERROR_DEBUG(errCode, errVal, errDesc) 
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

#if (ERRCLASS & ERRCLS_INT_PAR)    
#define CZLOGERROR_INTFC(errCode, errVal, errDesc) \
        SLogError(czCb.init.ent, czCb.init.inst, czCb.init.procId, \
                   __FILE__, __LINE__, ERRCLS_INT_PAR, errCode, errVal, errDesc)
#else /* (ERRCLASS & ERRCLS_INT_PAR) */
#define CZLOGERROR_INTFC(errCode, errVal, errDesc) 
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

#if (ERRCLASS & ERRCLS_ADD_RES)    
#define CZLOGERROR_ADD_RES(errCode, errVal, errDesc) \
        SLogError(czCb.init.ent, czCb.init.inst, czCb.init.procId, \
                   __FILE__, __LINE__, ERRCLS_ADD_RES, errCode, errVal, errDesc)
#else /* (ERRCLASS & ERRCLS_INT_PAR) */
#define CZLOGERROR_ADD_RES(errCode, errVal, errDesc) 
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

#endif /* SS_MULTIPLE_PROCS */

#define   ERRCZ       0
#define   ECZXXX      ERRCZ

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CZERRH__ */

/**********************************************************************

         End of file:     cz_err.h@@/main/2 - Tue Aug 30 18:36:04 2011

**********************************************************************/

/********************************************************************40**

        Notes:

*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/

/********************************************************************60**

        Revision history:

*********************************************************************61*/

/********************************************************************80**

*********************************************************************81*/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      sy    1. LTE-X2AP Initial Release
/main/2      ---      pt    1. LTE-X2AP 3.1 release.
*********************************************************************91*/

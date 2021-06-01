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

     Name:    SSI Memory Usage File

     Type:    CPP source file

     Desc:    Code for SSI memory wrapper for SDK

     File:    SsiMem.cpp

     Sid:      SsiMem.cpp@@/main/1 - Fri Aug 29 19:50:48 2008

     Prg:     sv

*********************************************************************21*/



/* header include files (.h) */
#include "envopt.h"         /* environment options          */
#include "envdep.h"         /* environment dependent        */
#include "envind.h"         /* environment independent      */
#include "gen.h"            /* general layer                */
#include "ssi.h"            /* system services              */
#ifdef SS
#include "ss_err.h"            /* system services              */
#endif

/* header/extern include files (.x) */
#include "gen.x"            /* general layer                */
#include "ssi.x"            /* system services              */
#include "cm5.x"            /* common timer module          */
#include "cm_lib.x"         /* common library               */


#ifdef SSI_MEM
 #include <new> 
#endif

#include "SsiMem.h"

Mem SsiMem::ssiMem = { 0 };

#ifdef SSI_MEM
Void * SsiMem::operator new(size_t size) throw (std::bad_alloc)
{
   Void *buffer = NULLP;
  
   TRC2(SsiMem::new);
 
   // SSI function call to get the memory here
   if (SGetSBuf(ssiMem.region, ssiMem.pool, (Data **) &buffer,(Size) size) == ROK)  
   {
      cmMemset((U8 *)(buffer), 0, size);   
   }
   else
   { 
#ifdef SS
      SSLOGERROR(ERRCLS_DEBUG, ESS648, ERRZERO, "Memory Allocation failed");
#endif
      (buffer) = NULLP;
      throw(std::bad_alloc()); // Inform the caller.
   }
   RETVALUE(buffer);

}

Void SsiMem::operator delete(Void *delPtr, size_t size) throw()
{
   TRC2(SsiMem::delete);
   if(NULLP != delPtr)
   {
      (Void) SPutSBuf(ssiMem.region, ssiMem.pool, (Data *) delPtr,  (Size) size);
   }
}

/* No need to catch the exceptions here. Inform the respective exception through throw.
   size input would be n*sizeof(class required) typical case. */

Void * SsiMem::operator new[](size_t size) throw(std::bad_alloc)
{
   Void *buffer = NULLP;

   TRC2(SsiMem::new[]);

   // SSI function call to get the memory here
   if (SGetSBuf(ssiMem.region, ssiMem.pool, (Data **)&buffer,(Size) size) == ROK)  
   {
      cmMemset((U8 *)(buffer), 0, size);   
   }
   else
   { 
#ifdef SS
      SSLOGERROR(ERRCLS_DEBUG, ESS648, ERRZERO, "Memory Allocation in blocks failed");
#endif
      (buffer) = NULLP;
      throw(std::bad_alloc()); // Inform the caller.
   }
   RETVALUE(buffer);
}

Void SsiMem::operator delete[](Void *delPtr, size_t size) throw()
{
   TRC2(SsiMem::delete[]);
   if(NULLP != delPtr)
   {
      (Void) SPutSBuf(ssiMem.region, ssiMem.pool, (Data *) delPtr,  (Size) size);
   }
}

#endif

Void SsiMem::ssiSetMem(const Mem *inMem)
{
   cmMemset ((U8 *)&(ssiMem), 0, sizeof (Mem));
   cmMemcpy ((U8 *)&(ssiMem), (U8 *)(const_cast<Mem *>(inMem)), sizeof (Mem));
}

const Mem SsiMem::ssiGetMem()
{
   TRC2(SsiMem::ssiGetMem);
   RETVALUE(ssiMem);
} 



/********************************************************************30**

         End of file:     SsiMem.cpp@@/main/1 - Fri Aug 29 19:50:48 2008

*********************************************************************31*/



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
/main/1      ---     sv                1. Initial release.
*********************************************************************91*/


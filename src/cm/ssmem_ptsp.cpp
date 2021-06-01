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
  
     Name:     system services - portable service provider
  
     Type:     CPP source file
  
     Desc:     Prototype CPP source code for the System Service
               service provider primitives that must be supplied
               by the customer.
  
     File:     ss_ptsp.cpp
  
     Sid:      ssmem_ptsp.cpp@@/main/1 - Fri Aug 29 20:18:13 2008
   
     Prg:      sv
  
*********************************************************************21*/
  

/*
  
System services are the functions required by the protocol
layer for buffer management, timer management, date/time
management, resource checking and initialization.

The following functions are provided in this file for the
system services service user with the help of SsiMem class:

     SsiMem::operator new  Operator overloading of new for all generic scope class objects.
     SsiMem::operator delete Operator overloading of delete for all generic scope class objects.
     SsiMem::operator new[]  Operator overloading of new[] for all generic scope class objects.
     SsiMem::operator delete[] Operator overloading of delete[] for all generic scope class objects.

It should be noted that not all of these functions may be required
by a particular system services service user.

*     This software may be combined with the following TRILLIUM
*     software:
*
*     part no.      description
*     --------    ----------------------------------------------
*     1000358     The Trillium SIP SDK.
*
*/
 

/* header include files (.h) */
  
#include "envopt.h"        /* environment options */  
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
  
/* header/extern include files (.x) */
  
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */
#include "cm5.x"            /* common timer module          */
#include "cm_lib.x"         /* common library               */

#ifdef SSI_MEM
 #include <new> 
#endif

#include "SsiMem.h"


/* local defines */
  
/* local typedefs */
  
/* local externs */
  
/* forward references */
  
/* functions in other modules */
  
/* public variable declarations */

/* private variable declarations */
  

/*
*     interface functions to system services service user
*/
 

#ifdef SSI_MEM
Void * SsiMem::operator new(size_t size) throw (std::bad_alloc)
{
  
   TRC2(SsiMem::new);
 
   // SSI function call to get the memory here
   RETVALUE(NULLP);

}

Void SsiMem::operator delete(Void *delPtr, size_t size) throw()
{
   TRC2(SsiMem::delete);
   RETVOID;
}

Void * SsiMem::operator new[](size_t size) throw(std::bad_alloc)
{
   TRC2(SsiMem::new[]);
   RETVALUE(NULLP);
}

Void SsiMem::operator delete[](Void *delPtr, size_t size) throw()
{
   TRC2(SsiMem::delete[]);
   RETVOID;
}

#endif

Void SsiMem::ssiSetMem(const Mem *inMem)
{
   TRC2(SsiMem::ssiSetMem);
   RETVOID;
}

const Mem SsiMem::ssiGetMem()
{
   Mem locMem;
   
   TRC2(SsiMem::ssiGetMem);
   RETVALUE(locMem);
} 



/********************************************************************30**
  
         End of file:     ssmem_ptsp.cpp@@/main/1 - Fri Aug 29 20:18:13 2008
  
*********************************************************************31*/
  

/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/

 
/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/
  
/********************************************************************70**
********************************************************************90**
  
  version    initials                   description
-----------  ---------  ------------------------------------------------
/main/1      ---     sv                1. Initial release.
*********************************************************************91*/

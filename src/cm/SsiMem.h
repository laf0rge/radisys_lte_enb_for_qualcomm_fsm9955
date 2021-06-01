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

     Type:    CPP header file

     Desc:    Code for SSI memory wrapper for C++ classes.

     File:    SsiMem.h

     Sid:      SsiMem.h@@/main/1 - Fri Aug 29 19:50:49 2008

     Prg:     sv

*********************************************************************21*/


#ifndef __SSIMEMH__
#define __SSIMEMH__

/** @defgroup SSI_MEM The Trillium SSI Interface */
/** @ingroup SSI_MEM
 *  @class SsiMem 
 *  @brief This is a base class for using TAPA SSI memory management. 
 *  @details All the classes must inherit from this class, to use SSI memory
 *  It internally uses Region and Pool parameters initially set from the user for class objects which 
 *    require SSI usage.
 *  Application must initialize the static Mem field in this class (for region and pool fields), before creating the first new class of its interest. 
 *  It must use ssiSetMem static API for this purpose. 
 *  The same Mem field is then used by operator new and delete functions
 *  for accessing Region and Pool parameters.<br>
 *  The example for the usage of this class is found from below: @example swSdSsiMem.h.
 */


class SsiMem 
{

public:

#ifdef SSI_MEM

   /** @brief This operator overloads new operator for all generic scope class objects.
    *
    *  @details If there is a mismatch in passed size and expected size, as per class size, 
    *  function delegates the allocating functionality to default opeartor new to handle the same.
    *
    *  @param[in] size number of bytes to allocate the memory.
    *
    *  @return  Actual memory allocated if success for size, else throws exception accordingly.
    */
   Void * operator new (size_t size) throw (std::bad_alloc);

   /** @brief This operator overloads delete operator for all generic scope class objects.
    *
    *  @details If there is a mismatch in passed size and expected size, as per class size, 
    *  function delegates the allocating functionality to default opeartor delete to handle the same.
    *
    *  @param[in] delPtr The actual pointer intended to be deleted as void Type.
    *
    *  @param[in] size The size to delete which was previously allocated with new.
    *
    *  @exception std::badalloc If the memory for size cannot be allocated.
    *
    *  @return  none. deltPtr is nonNull if there is a failure during the processing.
    */
   Void  operator delete (Void *delPtr, size_t size) throw();

   /** @brief This operator overloads new[] operator for all generic scope class objects.
    *
    *  @details This function is invoked to allocate multiple blocks of same object type.
    *  If there is a mismatch in passed size and expected size, as per class size, 
    *  function delegates as runtime error.
    *
    *  @param[in] size number of bytes to to allocate the memory.
    *
    *  @exception std::badalloc If the memory for size cannot be allocated.
    *  @return  Actual memory allocated if success for size, else throws exception accordingly.
    */
   Void * operator new[] (size_t size) throw(std::bad_alloc);

   /** @brief This operator overloads delete[] operator for all generic scope class objects.
    *
    *  @details This function is invoked when multiple blocks of same object type were allocated before.
    *  If there is a mismatch in passed size and expected size, as per class size, 
    *  function delegates the allocating functionality to default opeartor delete to handle the same.
    *
    *  @param[in] delPtr The actual pointer intended to be deleted as void type.
    *
    *  @param[in] size The size to delete which was previously allocated with new[].
    *
    *  @return none. deltPtr is nonNull if there is a failure during the processing.
    */
   Void  operator delete[] (Void *delPtr, size_t size) throw(); 

#endif
 
   /** @brief This static function is used to set the Mem field.
    *  @details Service user must initialize the static Mem field in this class (for region and pool fields), 
    *  before creating the first new class of its interest. 
    *  The same Mem field is then used by new and delete functions for accessing Region and Pool parameters 
    *  with respect to SSI memory management.
    *  @param[in] inMem The constant pointer initialized with Region and Pool values.
    *  @return none. 
    */
   static Void ssiSetMem(const Mem *inMem);

   /** @brief This static function is used to get the Mem field.
    *  @details Incase if the service user wishes to use SSI memory management services, can use this API 
    *  to retrieve region and pool parameters' values. 
    *  @return Memory field as a constant structure[Mem].
    *  The same Mem is used by new and delete functions for accessing Region and Pool parameters 
    *  with respect to SSI memory management.
    */
   static const Mem ssiGetMem();
  
private:
   static Mem ssiMem;

};
#endif

/********************************************************************30**

         End of file:     SsiMem.h@@/main/1 - Fri Aug 29 19:50:49 2008

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
/main/1      ---      sv                1. Initial release.
*********************************************************************91*/

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

     Name:     RRM Application

     Type:     CPP include file

     Desc:     This file contains 
               
     File:     rm_erabpreemptionfinder.h

     Sid:      rm_preEmptableERABFinder.h@@/main/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:51:58 2016

     Prg:      Ronins

*********************************************************************21*/

/** @file rm_preEmptableERABFinder.h 
 * @brief This file is to find the premptable bearer(s) when there is a 
 *        incoming bearer is rejected and also having high priority. To admit
 *        the high priority bearer, need to pre-empt anexisting bearer 
 *        with lower priority. High Priority is considered in order of 
 *        CSG membership, Premption vulnerable, ARP value.
 */

#ifndef RM_ERABPREEMPTIONFINDER_H_
#define RM_ERABPREEMPTIONFINDER_H_
#include <list>
#include <map>
#include "rm_cmn.h"
#include "rm_uecontext.h"

/**
 * @brief  Structure to hold the required GBR PRBs in % for a bearer.
 * 
 * @details
 * - uiUlPrbs: Number of UL PRBs required in % for to satisfy the UL GBR.
 * - uiDlPrbs: Number of DL PRBs required in % for to satisfy the DL GBR.
 */
typedef struct rrmPrbsPerErab
{
   U32   uiUlPrbs; /*!< Number of PRBs required for UL */ 
   U32   uiDlPrbs; /*!< Number of PRBs required for DL */
}RrmPrbsPerErab;

/*!< Indicates a non-CSG member UE */
#define NON_CSG_MEMBER  0

/*!< Indicates a CSG member UE */
#define CSG_MEMBER      1

/*!< vulnerable ERAB  */
#define VULNERABLE      0

/*!< non-vulnerable ERAB  */
#define NON_VULNERABLE  1

/**
 * @brief Class is holds all the GBR ERABs and provide an utility to find suitable
 *        candidate(s) for pre-emption when if a new ERAB is to admit or
 *        hits the max usage of PRB for GBR. 
 */
class CPreEmptableERABFinder {

public:
   
   /**
    * @brief Function to stores the required cell configuration data.
    * 
    * @details Function is a constructor of the class and is used to update the
    *          required cell configuration ex MAX GBR PRB usage, Emergency
    *          bearer ARP etc.,
    *         
    * @param[in] stCellCfg 
    * 
    * @return void 
    */
    CPreEmptableERABFinder(LrmCellConfigData & stCellCfg);

   /**
    * @brief Function is called  whenever a new ERAB is being added, 
    *        this indication is given to the pre-emption finder for 
    *        book-keeping of GBR ERABs.
    * 
    * @details Function updates the multi MAP table with the new \
    *          ERAB information with the key as CSG membership, 
    *          ERAB vulnarble, ARP value. 
    *         
    * @param[in]  stRbInfo  Pointer to the object of the new ERAB. 
    * @param[in]  pUE       Pointer to the object of the UE to find CSG membership
    * 
    * @return void 
    */
   void erabAdditionInd(CERABInfo* stRbInfo, CUEContext* pUE);

   /**
    * @brief Funtion to process the rejected ERABs and find preemtable ERABs.
    *
    * @details Function to traverse the list of rejected ERABs and find 
    *          candidate ERABs that can be preempted to allow service to the
    *          incoming ERAB.
    * 
    * @param[in] isCsgMember         Indicates if UE is CSG member or not
    * @param[in/out] erabsAccRejLst  Received Request contains accepted and
    *                                rejected ERAB information based QCI and
    *                                PRB and also moves the rejected erabs to
    *                                accepted ERABs list if a pre-emptable ERAB 
    *                                is found.
    * @param[out] preEmptedERABs     List of ERABs to be preempted. Release 
    *                                indication is sent to MME for all the
    *                                ERABs in the list.
    *
    * @return bool 
    *         Returns true in case of at least one Pre-empted ERAB is found;
    *         else false. 
    */
   bool findPreEmptableERABs(bool isCsgMember,
                             RrmRbcRspInfo & erabsAccRejLst,
                             CERABInfoLst & preEmptedERABs);

#if 0
   /**
    * @brief Function to deletes all the ERABs for a particular UE 
    * 
    * @details Function to delete all the ERABs for a praticular UE. 
    *         
    * @param[in] usCrnti CRNTI of the UE.  
    * 
    * @return void 
    */
   void deleteUeInd(U32 usCrnti);
#endif

   /**
    * @brief Function to remove the specific ERAB information 
    *        for a UE from the Pre-emptable ERABs table
    * 
    * @details Function to remove the specific ERAB info for a UE 
    *          from the multi map table.
    *         
    * @param[in] stRbInfo  Pointer to the object of an ERAB which need to be
    *                      removed
    * @param[in] pUE       Pointer to the object of the UE for which an ERAB ned
    *                      to be removed
    * 
    * @return void 
    */
   void deleteERABInd(CERABInfo* stRbInfo, CUEContext* pUE);

   /**
    * @brief Function modify the contents of existing ERAB entry in m_rabList
    * 
    * @details Function update the pre-emptable GBR ERABS table based in case 
    *          of ERAB modify request.
    *         
    * @param[in] pstNewRbInfo     Pointer to the new ERAB info requested for
    *                              modifcation
    * @param[in] pObjOldERabInfo  Pointer to the ERAB object to which is
    *                              already existed in the Pre-emptable ERABs 
    *                              table.
    * @param[in] pUE              Pointer to the UE, used to find the CSG or
    *                             non-CSG UE.
    * 
    * @return void 
    */
   void erabModificationInd(RmERabInfo* pstNewRbInfo,
                            CERABInfo* pObjOldERabInfo,
                            CUEContext* pUE);

   /**
    * @brief Function modify the contents of existing ERAB entry in m_rabList
    * 
    * @details Function update the pre-emptable GBR ERABS table based in case 
    *          of CSG membership status is changed.
    *         
    * @param[in] pObjUE       Pointer to the UE, to find the CSG or non CSG
    *                         UE before CSG status change.
    * @param[in] pObjERabInfo Pointer to the object of the ERAB  
    * @param[in] bIsCSGMember is the CSG member or not after modication. 
    * @return void 
    */
   void erabModificationInd(CUEContext* pObjUE, 
                            CERABInfo* pObjERabInfo,
                            bool bIsCSGMember);

   /**
    * @brief Function to get lowest priority ERAB to delete.
    * 
    * @details This function fetches and returns the least priority ERAB to be
    *          released.
    *         
    * @param[out] preEmptedERABs   List of ERABs to be preempted
    *
    * @return bool 
    *         Returns true in case of at least one Pre-empted ERAB is found;
    *         else false. 
    */
   bool findLeastPriorityPreEmptableERAB(CERABInfoLst & preEmptedERAB);
 
   /**
    * @brief Function to find the candidate ERAB is pre-empatble or not
    * 
    * @details This function to find the candidate ERAB is pre-emptable or
    *          not based on preEmption valnaurablity of the ERAB and the
    *          same UE and same ERAB should not choose as candidate ERAB for
    *          pre-emption.
    *         
    * @param[in] usCRNTI  Incoming RAB's UE CRNTI
    * @param[in] stRejectedErab Incoming RAB for admission but which is rejected
    * @param[in] objCandidateErab This is the cadidate ERAB for pre-emption.
    *
    * @return bool 
    *         Returns true incase of the candidate ERAB is pre-emptable ERAB
    *         else false. 
    */
   bool isPremptableErabBasedOnIncmoingErab(U16 usCRNTI, 
                                            RrmErabRejected *stRejectedErab,  
                                            CERABInfo *objCandidateErab);

private:
   typedef std::multimap<U32, CERABInfo*> ErabsMap;

   typedef std::multimap<U32, RrmErabRejected*> ErabsRejMap;

   typedef ErabsMap::iterator RbListIterator;

   typedef ErabsRejMap::iterator RbRejListIterator;

   typedef std::pair<RbListIterator, RbListIterator> ErabIteratorPair;

   /*!< Multimap for storing all the GBR ERAB inforamtion */
   ErabsMap m_rabList;

   /*!< Multimap array for storing particular every QCI GBR ERABs */
   ErabsMap m_rabQCIList[RM_QCI_FOUR];

   /*!< Max GBR PRB usage limit in % of PRBs s used for GBRs */
   U8  m_usMaxGBRPRBUsageLimit;

   /*!< Configured ARP value of the bearer is to identify Emergency bearer */
   U8   m_bArpForEmergencyBearer;

   /**
    * @brief Function to generate the key
    * 
    * @details This funtion makes the key to find the premptable RB based on 
    *          the CSG membership status, ARP preemption vulnerability and
    *          ARP priority value. The information is stored in the below 
    *          format.
    * BIT:   |            5              |             4                      | 3| 2| 1| 0|
    *        =============================================================================
    * FIELD: | CSG membership status     |    Pre-emption vulnerability       |    ARP    |
    *        =============================================================================
    * VALUE: |Member = 1; non-Member = 0 | Vulnerable = 0; not vulnerable = 1 |  1 to 15  |
    *
    *
    * @param[in] isCsgMember     CSG membership status
    * @param[in] isVulnerable    Vulnerability of ERAB
    * @param[in] arpVal          ARP value of ERAB
    * 
    * @return U32
    *         Return the derived key
    */
   U32 generateKey(U32 isCsgMember, U32 isVulnerable, U32 arpVal)
   {
      U32 key;
      //fill the ARP value in least significant nibble. Lowest ARP val, having
      //the highest priority, shall be made to store towards the higher
      //indices such that lowest priority ERAB will be chosen first during
      //pre-emption
      key  = (MAX_ARP_VAL - arpVal);
      //fill 5th bit with vulnerablity
      key |= (isVulnerable << 4);
      //fill 6th bit with CSG membership status
      key |= (isCsgMember << 5);

      return key;
   }

   /**
    * @brief Function for to insert the new ERAB into GBR ERAB multi map table
    * 
    * @details Function for to insert the new ERAB into GBR ERAB multi map table
    *      
    * @param[in] key       KEY of the ERAB to identify in the map table  
    * @param[in] stRbInfo  Pointer to the object of ERAB which need to be
    *                      inserted.
    * @return void 
    */
   void insertToList(U32 key, CERABInfo* stRbInfo) 
   {
      m_rabList.insert(std::pair<U32, CERABInfo*> (key,stRbInfo));
   };

   /**
    * @brief Function for to insert the new ERAB into GBR ERAB QCI multi 
    *        map table
    * 
    * @details Function for to insert the new ERAB into GBR ERAB QCI 
    *          multi map table
    *      
    * @param[in] qciVal    Value of QCI     
    * @param[in] key       KEY of the ERAB to identify in the map table  
    * @param[in] stRbInfo  Pointer to the object of ERAB which need to be
    *                      inserted.
    * @return void 
    */
   void insertToQCIList(U32 qciVal, U32 key, CERABInfo* stRbInfo) 
   {
      m_rabQCIList[qciVal-1].insert(std::pair<U32,CERABInfo*> (key,stRbInfo));
   };

   /**
    * @brief Function for to remove the existed ERAB from GBR ERAB multi map table
    * 
    * @details Function for to remove the new ERAB from  GBR ERAB multi map
    *          table.  
    *      
    * @param[in] it       Iterator of the ERAB   
    * @return void 
    */
   void removeFromList(RbListIterator it){m_rabList.erase(it);};

   /**
    * @brief Function for to remove the existed ERAB from GBR ERAB QCI 
    *        multi map table
    * 
    * @details Function for to remove the new ERAB from  GBR ERAB QCI multi map
    *          table.  
    *      
    * @param[in] qciVal    Value of QCI
    * @param[in] it        Iterator of the ERAB   
    * @return void 
    */
   void removeFromQCIList(U32 qciVal,RbListIterator it)
   {
      m_rabQCIList[qciVal-1].erase(it);
   };

#if 0
   RbListIterator findFromList(U32 key){return(m_rabList.find(key));};
#endif

   /**
    * @brief Function to find list of all ERABs having same key and 
    *        may belong to different UEs
    *
    * @details Function to find the list of all ERABs with same key.  
    *      
    * @param[in] key     KEY of the ERABs to identify in the map table  
    * @param[in] rabList List for which the iterator pair needs to be fetched
    *
    * @return ErabIteratorPair 
    *         Returns an iterator which is having  the list of ERABs 
    *         with same key for all UEs.
    */
   ErabIteratorPair findERABsWithSameKey(U32 key, ErabsMap *rabList)
   {
      return rabList->equal_range(key);
   }; 

   /**
    * @brief Function to find a list of ERABs that can be preempted inorder to 
    *        admit/continue service for the stERAB.
    * 
    * @details Function is used to find the list of ERABs that can be preempted
    *          by traversing the ERAB list from the begining till the upper 
    *          limit.
    *      
    * @param[in] stERAB           Upper bound KEY to identify the list of ERABs 
    * @param[out] preEmptedERABs  KEY of the ERAB to identify in the map table  
    * @param[in] isCsgMember      Indicates if the UE is a CSG member or not.
    *
    * @return bool
    *         Returns true if at least one pre-emptable ERAB is found;
    *         else false.
    */
   void findPreEmptableERABsForPRBRejection(RrmRbcRspInfo & erabsAccRejLst,
                                            ErabsRejMap rejList,
                                            CERABInfoLst & preEmptedERAB,
                                            bool isCsgMember);
   /**
    * @brief Function to fetch the ERAB add mod info from the incoming ERAB list
    *        for the requested ERAB ID.
    * 
    * @details Traverse the incoming ERAB list and return the ERAB add info 
    *          corresponding to the ERAB ID.
    *      
    * @param[in] stErab      Incoming ERAB list 
    * @param[in] uiErabId    ERAB Identity
    *
    * @return RmuUErabAddModIe*
    *               Return a valid ERAB add info pointer on success,
    *               else, NULL
    */
   RmuUErabAddModIe *getErabAddModIe(RmuErabConfigIe &stERAB, U32 uiErabId);

   /**
    * @brief Function to create a ordered list of rejected incoming bearers.
    * 
    * @details 
    *          - for each rejected ERAB in the rejected ERAB list,
    *            - generate the key and insert into the map
    *      
    * @param[in] eRabRejectedLst  List of rejected ERABs
    * @param[in] isCsgMember      Indicates if the UE is a CSG member or not
    * @param[out] rejectLstMap    Ordered list of rejected ERABs
    * @param[in] numERABsRej      Number of rejected ERABs in the rejected list
    *
    * @return void
    */
   void createOrderedRejectedERABslist(RrmErabRejected *eRabRejectedLst,
                                       bool isCsgMember, 
                                       ErabsRejMap &rejectLstMap,
                                       U32 numERABsRej);

   /**
    * @brief Function to fetch a ERAB that can be preempted inorder to 
    *        admit the incoming bearer.
    * 
    * @details Function is used to preempt the first ERABs of the corresponding
    *          QCI type.
    *
    * @param[in] stERAB           Upper bound KEY to identify the list of ERABs 
    * @param[out] preEmptedERABs  KEY of the ERAB to identify in the map table  
    * @param[in] isCsgMember      Indicates if the UE is a CSG member or not.
    *
    * @return bool
    *         Returns true if at least one pre-emptable ERAB is found;
    *         else false.
    */
   void findPreEmptableERABsForQCIRejection(RrmRbcRspInfo & erabsAccRejLst,
                                            ErabsRejMap rejList,
                                            CERABInfoLst& preEmptedERABs, 
                                            bool isCSGMember);

   /**
    * @brief Function to fetch the position in map until which the candidates
    *        can be preempted.
    * 
    * @details This function derives the search limit iterator based on the
    *          incoming CSG membership of the UE and ARP priority.
    *      
    * @param[in] isCsgMember  Indicates if the UE is a CSG member or not.
    * @param[in] uiArpVal     Indicates the ERAB ARP priority value.
    * @param[in] rabList      List from which the upper limit has to be derived
    *
    * @return RbListIterator
    *         Returns an iterator to the rabList
    */
   RbListIterator findSearchLimitIt(bool isCsgMember,
                                    U32  uiArpVal,
                                    ErabsMap *rabList);
    /**
    * @brief Function to reorder the rejected ERAB list.
    * 
    * @details This function reorders the rejected ERAB list after QCI and PRB
    *          based accept criteria is met. 
    *      
    * @param[in/out] stERABAccRejLst  Accepted/Rejected ERAB list.
    *
    * @return Void 
    */
    void reorderRejectedERABsList(RrmRbcRspInfo & stERABAccRejLst);

#ifdef DBG_MAC_RRM_PRB_PRINT
   /**
    * @brief Function to display the key and info of ERAB currently present
    *        in the ERAB Map.
    * 
    * @details Traverse the list and display the ERAB and Key information.
    *      
    * @return void
    */
    void displayErabMap();
#endif

   /**
    * @brief Function to fetch the ERABs to be preempted.
    * 
    * @details This function traverses the rabList from begin till  
    *          upperIt, and add the suitable ERABs into preemted ERAB list.
    *          The sum of DL and UL PRBs from total preempted ERABs are updated.
    *      
    * @param[in] upperIt         Upper iterator till which to traverse.
    * @param[in] usCRNTI         UE CRNTI of incoming RAB
    * @param[in] pstRejectedErab Rejected ERAB information
    * @param[out] uiDlPrbSum     Sum of preempted DL PRBs
    * @param[out] uiUlPrbSum     Sum of preempted UL PRBs
    * @param[out] preEmptedERABs List of preempted ERABs
    *
    * @return bool
    *         Returns true if bearers are preempted, else false
    */
    bool getErabsForPreemption(RbListIterator upperIt,
                               U16 usCRNTI, 
                               RrmErabRejected *pstRejectedErab,  
                               U32 *uiDlPrbSum,
                               U32 *uiUlPrbSum,
                               CERABInfoLst & preEmptedERABs);

   /**
    * @brief Function to fetch the DL and UL PRB sum from preempted ERAB list.
    * 
    * @details This function updates the sum of DL and UL PRBs from the
    *          preempted ERABs list.
    *      
    * @param[in] preEmptedERABs  List of preempted ERABs
    * @param[out] uiDlPrbSum     Sum of preempted DL PRBs
    * @param[out] uiUlPrbSum     Sum of preempted UL PRBs
    *
    * @return void
    */
    void getDlUlPrbSum(CERABInfoLst & preEmptedERABs,
                       U32 *uiDlPrbSum,
                       U32 *uiUlPrbSum);
};

#endif  /*RM_ERABPREEMPTIONFINDER_H_ */

/********************************************************************30**

       End of file:     rm_preEmptableERABFinder.h@@/main/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:51:58 2016

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



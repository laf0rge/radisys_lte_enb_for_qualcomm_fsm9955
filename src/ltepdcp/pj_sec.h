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

 
 #include "prc_types.h"
/* lenghth of the pointer variable */
#define BC_ICPU_MSG_LEN  sizeof(BcIcpuMsg)
#define BC_LOCAL_REG     0
#define BC_SHARED_REG    1
#define BC_ICPU_MSG_NOR  0
#define BC_ICPU_MSG_FREE 1

#define BC_MAX_PDU_CIPHER_REQ 1
#define BC_MAX_PDU_INTPROT_REQ 1
#define BC_MAX_CPU 2


#define PJ_SEC_ALLOC(_cb, _buf, _size)                         \
{                                                          \
 if (SGetSBuf(1,0, (Data **)&_buf,\
                (Size) _size) == ROK)                      \
   {                                                       \
      cmMemset((U8 *)(_buf), 0, _size);                    \
   }                                                       \
   else                                                    \
   {                                                       \
      (_buf) = NULLP;                                      \
   }                                                       \
}
typedef struct intProtCallBkParams
{
   PRC_SNOW_INTEGRITY_PARAMS *intgrtyParams;
   U32                       numPdu;
   PRC_SNOW_INTEGRITY_DATA   *pdu_list;
   PjLibTrans                libTrans; 
   Pst                    *pst;
   Buffer                 *pduInfoBuffer;
   Buffer                 *pduListBuffer;
}IntProtCallBkParams;
typedef struct intVerCallBkParams
{
   PRC_SNOW_INTEGRITY_PARAMS *intgrtyParams;
   U32                       numPdu;
   PRC_SNOW_INTEGRITY_DATA   *pdu_list;
   PjLibTrans                libTrans; 
   Pst                    *pst;
   Buffer                 *pduInfoBuffer;
   Buffer                 *pduListBuffer;
   U32                    recvMacI;
}IntVerCallBkParams;

#ifdef CIPH_BATCH_PROC
typedef struct pjTxEntDtl {
  void   *txEnt;
  void  *rbCb;
}PjTxEntDtl;

typedef struct pjRxEntDtl {
  void   *rxEnt;
  void  *rbCb;
}PjRxEntDtl;
#endif
typedef struct cipherCallBkParams
{
#ifdef CIPH_BATCH_PROC
   Data                  *cipheredBuf[32];
   PjTxEnt                *txEntLst[32];      
   PjRxEnt                *rxEntLst[32];      
   PRC_SNOW_CIPHER_DATA *pduList;
#else
   PRC_SNOW_CIPHER_DATA   *pdu_list;
   PjLibTrans             libTrans; 
   Buffer                 *mBuf[10];
   Buffer                 *pduInfoBuffer;
   Buffer                 *pduListBuffer;
   Pst                    *pst;
#endif
   U32                    numTxPdu;
   U32                    numRxPdu;
   PRC_SNOW_CIPHER_PARAMS *cipherParams;
}CipherCallBkParams;

/* ZUC_ALGO start */
typedef struct zucIntProtCallBkParams
{
   PRC_ZUC_INTEGRITY_PARAMS *intgrtyParams;
   U32                       numPdu;
   PRC_ZUC_INTEGRITY_DATA   *pdu_list;
   PjLibTrans                libTrans; 
   Pst                    *pst;
   Buffer                 *pduInfoBuffer;
   Buffer                 *pduListBuffer;
}ZucIntProtCallBkParams;

typedef struct zucIntVerCallBkParams
{
   PRC_ZUC_INTEGRITY_PARAMS *intgrtyParams;
   U32                       numPdu;
   PRC_ZUC_INTEGRITY_DATA   *pdu_list;
   PjLibTrans                libTrans; 
   Pst                    *pst;
   Buffer                 *pduInfoBuffer;
   Buffer                 *pduListBuffer;
   U32                    recvMacI;
}ZucIntVerCallBkParams;

typedef struct zucEncryptCallBkParams
{
#ifdef CIPH_BATCH_PROC
   Data                  *encryptBuf[32];
   PjTxEnt                *txEntLst[32];      
   PjRxEnt                *rxEntLst[32];
   PRC_ZUC_ENCRYPTION_DATA  *pduList;      
#else
   PRC_ZUC_ENCRYPTION_DATA   *pdu_list;
   PjLibTrans             libTrans; 
   Buffer                 *mBuf[10];
   Buffer                 *pduInfoBuffer;
   Buffer                 *pduListBuffer;
   Pst                    *pst;
#endif
   U32                    numTxPdu;
   U32                    numRxPdu;
   PRC_ZUC_ENCRYPTION_PARAMS *encryptParams;
}ZucEncryptCallBkParams;
/* ZUC_ALGO - end */

#ifdef AES
typedef struct aesIntProtCallBkParams
{
   PRC_AES_INTEGRITY_PARAMS *intgrtyParams;
   U32                       numPdu;
   PRC_AES_INTEGRITY_DATA   *pdu_list;
   PjLibTrans                libTrans; 
   Pst                    *pst;
   Buffer                 *pduInfoBuffer;
   Buffer                 *pduListBuffer;
}AesIntProtCallBkParams;

typedef struct aesIntVerCallBkParams
{
   PRC_AES_INTEGRITY_PARAMS *intgrtyParams;
   U32                       numPdu;
   PRC_AES_INTEGRITY_DATA   *pdu_list;
   PjLibTrans                libTrans; 
   Pst                    *pst;
   Buffer                 *pduInfoBuffer;
   Buffer                 *pduListBuffer;
   U32                    recvMacI;
}AesIntVerCallBkParams;

typedef struct aesEncryptCallBkParams
{
#ifdef CIPH_BATCH_PROC
   Data                  *encryptBuf[32];
   PjTxEnt                *txEntLst[32];      
   PjRxEnt                *rxEntLst[32];
   PRC_AES_ENCRYPTION_DATA  *pduList;      
#else
   PRC_AES_ENCRYPTION_DATA   *pdu_list;
   PjLibTrans             libTrans; 
   Buffer                 *mBuf[10];
   Buffer                 *pduInfoBuffer;
   Buffer                 *pduListBuffer;
   Pst                    *pst;
#endif
   U32                    numTxPdu;
   U32                    numRxPdu;
   PRC_AES_ENCRYPTION_PARAMS *encryptParams;
}AesEncryptCallBkParams;
#endif /* AES */

#ifdef CIPH_BATCH_PROC
typedef struct deCipherCallBkParams
{
   U32                    numPdu;
   PjRxEnt                *rxEntLst[32];      
   //Buffer                 *deCipheredBuf[16];
   PRC_SNOW_CIPHER_DATA *pduList;
   PRC_SNOW_CIPHER_PARAMS *deCipherParams;
}DecipherCallBkParams;

typedef struct decryptCallBkParams
{
   U32                    numPdu;
   PjRxEnt                *rxEntLst[32];      
   //Buffer                 *deCipheredBuf[16];
   PRC_ZUC_ENCRYPTION_DATA *pduList;
   PRC_ZUC_ENCRYPTION_PARAMS *decryptParams;
}DecryptCallBkParams;

#ifdef AES
typedef struct decryptAesCallBkParams
{
   U32                    numPdu;
   PjRxEnt                *rxEntLst[32];      
   //Buffer                 *deCipheredBuf[16];
   PRC_AES_ENCRYPTION_DATA *pduList;
   PRC_AES_ENCRYPTION_PARAMS *decryptParams;
}DecryptAesCallBkParams;
#endif
#endif

#if 0
extern UINT16 prc_snow_ciphering(int mode,
            int number_of_pdu, 
         PRC_SNOW_CIPHER_DATA *pdu_list,
            int wait_mode, 
         PRC_CALLBACK_FUNC callback_func,
            void *callback_params);

extern UINT16 prc_snow_integrity(int number_of_pdu, 
         PRC_SNOW_INTEGRITY_DATA *pdu_list,
         int wait_mode, 
         PRC_CALLBACK_FUNC callback_func,
         void *callback_params);
#endif

#ifdef PJ_INT_ASYNC
extern  S16 pjUtlIntProtCallBk ARGS((
U32 rc, 
Void* callback_params
));
extern  S16 pjUtlIntVerCallBk ARGS((
U32 rc, 
Void* callback_params
));


extern S16 bcIntProtReq ARGS((
Pst       *pst,
PjSecInp   secInp,                  /* Input for Integrity protection */
PjLibTrans libTrans,
Buffer     *mBuf                  /* SDU to be compressed */
));
#else
extern S16 bcIntProtReq ARGS((
PjSecInp   secInp,                  /* Input for Integrity protection */
PjDlUeCb *ueCb,
Buffer     *mBuf,                  /* SDU to be compressed */
U32        *macI                   /* Message authentication code for the SDU */
));
#endif

extern S16 pjUtlCipherCallBk ARGS((U32 rc, Void* callback_params));
#ifdef CIPH_BATCH_PROC
extern S16 pjUtlBatchCipherCallBk ARGS((U32 rc, Void* callback_params));
#endif

#if 0
EXTERN U16 prc_snow_ciphering(int mode, int number_of_pdu, 
       PRC_SNOW_CIPHER_DATA *pdu_list,int wait_mode, 
       PRC_CALLBACK_FUNC callback_func,
            void *callback_params);

EXTERN U16 prc_snow_integrity(int number_of_pdu, 
         PRC_SNOW_INTEGRITY_DATA *pdu_list,
         int wait_mode, 
         PRC_CALLBACK_FUNC callback_func,
         void *callback_params);
#endif



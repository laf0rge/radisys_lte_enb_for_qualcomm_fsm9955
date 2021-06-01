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
/********************************************************************************************************
*
*   Copyright (c) 2009-2012 Broadcom Corporation
*   All rights reserved
*
*   FILE NAME     :  prc_lte_cphy_msg.h
*
*   DATE CREATED  :  13-March-2012
*
*   LAST UPDATED  :
*
*   VERSION       :
*
*   DESCRIPTION   :  This file includes the definition of the interface between L1 and PS layers
*
********************************************************************************************************/

#ifndef _PHY_INTERFACE_API_H_
#define _PHY_INTERFACE_API_H_

#ifdef __cplusplus
    extern "C" {
#endif
       enum NMM_API_ERROR_CODE {
          NMM_LTE_TDD_NOT_SUPPORTED = 0x80,
          NMM_NOT_ENABLED,
          NMM_UNEXPECTED_CMD,
          NMM_INVALID_FREQ_BAND,
          NMM_INVALID_EARFCN,
          NMM_FREQ_BAND_NOT_SUPPORTED_BY_HW,
          NMM_INVALID_NR_CARRIER_LIST,
          NMM_INVALID_CARRIER_LIST,
          NMM_LTE_IN_USE,
          NMM_LTE_OUT_OF_MEMORY,
          NMM_SIB_INCOMPLETE,
          NMM_LTE_DI_ERROR
       };





/*
 * Status codes definitions
 */
       typedef U8 UINT8;
       typedef U16 UINT16;
       typedef S16 INT16;
       typedef U32 UINT32;
       typedef S32 INT32;
       typedef U8 BOOL8;

#define CPHY_MSG_STATUS_SUCCESS                                 0
#define CPHY_MSG_STATUS_NOT_IN_SNIFFER_MODE                     17
#define CPHY_MSG_STATUS_ILLEGAL_FREQUENCY_BAND                  18
#define CPHY_MSG_STATUS_NO_SUCCESSFUL_BSIC_READING              19
#define CPHY_MSG_STATUS_UNSPECIFIED                             255

#define MAX_VENDOR_LIST  6
#define MAX_SI_LIST                     32

#define CPHY_EVENT						          0x1000
#define CPHY_ENB_PARA_REQ__EVENT           	      (CPHY_EVENT + 0x00)
#define CPHY_ENB_PARA_RSP__EVENT           	      (CPHY_EVENT + 0x01)
#define CPHY_ENB_CONFIG_REQ__EVENT         	      (CPHY_EVENT + 0x02)
#define CPHY_ENB_CONFIG_RSP__EVENT         	      (CPHY_EVENT + 0x03)
#define CPHY_ENB_START_REQ__EVENT          	      (CPHY_EVENT + 0x04)
#define CPHY_ENB_STOP_REQ__EVENT           	      (CPHY_EVENT + 0x05)
#define CPHY_ENB_STOP_IND__EVENT           	      (CPHY_EVENT + 0x06)
#define CPHY_UE_CONFIG_REQ__EVENT      	          (CPHY_EVENT + 0x07)
#define CPHY_UE_CONFIG_RSP__EVENT      	          (CPHY_EVENT + 0x08)
#define CPHY_ERROR_IND__EVENT          	          (CPHY_EVENT + 0x09)
#define CPHY_UE_RELEASE_REQ__EVENT                (CPHY_EVENT + 0x0a)
#define CPHY_UE_RELEASE_RSP__EVENT     	          (CPHY_EVENT + 0x0b)
#define CPHY_MEASUREMENT_INITIATION_REQ__EVENT    (CPHY_EVENT + 0x0c)
#define CPHY_MEASUREMENT_INITIATION_RSP__EVENT    (CPHY_EVENT + 0x0d)
#define CPHY_MEASUREMENT_TERMINATION_REQ__EVENT   (CPHY_EVENT + 0x0e)
#define CPHY_MEASUREMENT_TERMINATION_RSP__EVENT   (CPHY_EVENT + 0x0f)
#define CPHY_MEASUREMENT_IND__EVENT               (CPHY_EVENT + 0x10)
#define CPHY_REF_SIGNAL_POWER_REQ__EVENT          (CPHY_EVENT + 0x11)
#define CPHY_REF_SIGNAL_POWER_RSP__EVENT          (CPHY_EVENT + 0x12)

       /* 0x11~0x7F reserved */
#define PHY_FIRST_EVENT                           (CPHY_EVENT + 0x80)
#define PHY_DL_CONFIG_REQ__EVENT       	          (CPHY_EVENT + 0x80)
#define PHY_UL_CONFIG_REQ__EVENT       	          (CPHY_EVENT + 0x81)
#define PHY_SUBFRAME_IND__EVENT                   (CPHY_EVENT + 0x82)
#define PHY_HI_DCI0_REQ__EVENT         	          (CPHY_EVENT + 0x83)
#define PHY_TX_REQ__EVENT              	          (CPHY_EVENT + 0x84)
#define PHY_HARQ_IND__EVENT            	          (CPHY_EVENT + 0x85)
#define PHY_CRC_IND__EVENT             	          (CPHY_EVENT + 0x86)
#define PHY_RX_ULSCH_IND__EVENT        	          (CPHY_EVENT + 0x87)
#define PHY_RACH_IND__EVENT            	          (CPHY_EVENT + 0x88)
#define PHY_SRS_IND__EVENT             	          (CPHY_EVENT + 0x89)
#define PHY_SR_IND__EVENT           	          (CPHY_EVENT + 0x8A)
#define PHY_CQI_IND__EVENT          	          (CPHY_EVENT + 0x8B)
#define PHY_AGC_INFO_IND__EVENT        	          (CPHY_EVENT + 0x8C)

#define PHY_LAST_EVENT                            (CPHY_EVENT + 0x8C)

#define CPHY_L1C_INIT_REQ__EVENT                   (CPHY_EVENT + 0x100)
#define CPHY_L1C_INIT_RSP__EVENT                   (CPHY_EVENT + 0x101)

       /* General sniffer events */
#define  CPHY_SNIFFER_CONTROL_REQ__EVENT                                (CPHY_EVENT + 0x120)
#define  CPHY_SNIFFER_CONTROL_RSP__EVENT                                (CPHY_EVENT + 0x121)
#define  CPHY_SNIFFER_RTWP_REQ__EVENT                                   (CPHY_EVENT + 0x122)
#define  CPHY_SNIFFER_RTWP_RSP__EVENT                                   (CPHY_EVENT + 0x123)

       /* UNTS Sniffer Events */
#define  CPHY_SNIFFER_CELL_SEARCH_REQ__EVENT                            (CPHY_EVENT + 0x124)
#define  CPHY_SNIFFER_CELL_SEARCH_RSP__EVENT                            (CPHY_EVENT + 0x125)
#define  CPHY_SNIFFER_READ_BCH_REQ__EVENT                               (CPHY_EVENT + 0x126)
#define  CPHY_SNIFFER_READ_BCH_RSP__EVENT                               (CPHY_EVENT + 0x127)
#define  PHY_SNIFFER_BCH_DATA_IND__EVENT                                (CPHY_EVENT + 0x128)
#define  CPHY_SNIFFER_STOP_BCH_REQ__EVENT                               (CPHY_EVENT + 0x129)
#define  CPHY_SNIFFER_STOP_BCH_RSP__EVENT                               (CPHY_EVENT + 0x130)

/* Additional general events */
#define  CPHY_SNIFFER_RTWP_LIST_REQ__EVENT                              (CPHY_EVENT + 0x134)
#define  CPHY_SNIFFER_RTWP_LIST_RSP__EVENT                              (CPHY_EVENT + 0x135)

       /* GSM Sniffer Events */                                                               
#define  CPHY_GSM_READ_FCCH_REQ__EVENT                                  (CPHY_EVENT + 0x140)
#define  CPHY_GSM_READ_FCCH_RSP__EVENT                                  (CPHY_EVENT + 0x141)
#define  CPHY_GSM_READ_BSIC_REQ__EVENT                                  (CPHY_EVENT + 0x142)
#define  CPHY_GSM_READ_BSIC_RSP__EVENT                                  (CPHY_EVENT + 0x143)
#define  CPHY_GSM_BCCH_READ_CONTROL_REQ__EVENT                          (CPHY_EVENT + 0x144)
#define  CPHY_GSM_BCCH_READ_CONTROL_RSP__EVENT                          (CPHY_EVENT + 0x145)
#define  CPHY_GSM_BCCH_IND__EVENT                                       (CPHY_EVENT + 0x146)

       /* LTE sniffer events */
#define CPHY_LTE_SNIFFER_CELL_SEARCH_REQ__EVENT                         (CPHY_EVENT + 0x150)
#define CPHY_LTE_SNIFFER_CELL_SEARCH_RSP__EVENT                         (CPHY_EVENT + 0x151)
#define CPHY_LTE_SNIFFER_READ_SIB_REQ__EVENT                            (CPHY_EVENT + 0x152)
#define CPHY_LTE_SNIFFER_READ_SIB_RSP__EVENT                            (CPHY_EVENT + 0x153)
#define CPHY_LTE_SNIFFER_READ_SIB1_REQ__EVENT                           (CPHY_EVENT + 0x154)
#define CPHY_LTE_SNIFFER_READ_SIB1_RSP__EVENT                           (CPHY_EVENT + 0x155)
#define CPHY_LTE_SNIFFER_STOP_READ_SIB_REQ__EVENT                       (CPHY_EVENT + 0x156)
#define CPHY_LTE_SNIFFER_STOP_READ_SIB_RSP__EVENT                       (CPHY_EVENT + 0x157)
#define CPHY_LTE_SNIFFER_READ_SIB_IND__EVENT                            (CPHY_EVENT + 0x158)

       /* Maintenance messages */
#define  CPHY_MAINTENANCE_TEST_MODEL_REQ__EVENT                         (CPHY_EVENT + 0x160)
#define  CPHY_MAINTENANCE_TEST_MODEL_RSP__EVENT                         (CPHY_EVENT + 0x161)

       /* SPI messages */
#define CPHY_SPI_WRITE_REQ__EVENT                                       (CPHY_EVENT + 0x170)
#define CPHY_SPI_WRITE_RSP__EVENT                                       (CPHY_EVENT + 0x171)
#define CPHY_SPI_READ_REQ__EVENT                                        (CPHY_EVENT + 0x172)
#define CPHY_SPI_READ_RSP__EVENT                                        (CPHY_EVENT + 0x173)

       /* AFC messages */
#define CPHY_AFC_WRITE_REQ__EVENT                                       (CPHY_EVENT + 0x174)
#define CPHY_AFC_WRITE_RSP__EVENT                                       (CPHY_EVENT + 0x175)
#define CPHY_AFC_READ_REQ__EVENT                                        (CPHY_EVENT + 0x176)
#define CPHY_AFC_READ_RSP__EVENT                                        (CPHY_EVENT + 0x177)

#define  CPHY_LAST__EVENT                                               (CPHY_EVENT + 0x200)

       typedef struct
       {
          UINT16 msg_code;       
          UINT16 length;      
       }PHY_MSG_HEADER, CPHY_MSG_HEADER;

#define PHY_DUPLEX_TDD          0
#define PHY_DUPLEX_FDD          1
#define PHY_DUPLEX_HD_FDD       2

#define LAYER1_STATE_IDLE       0   
#define LAYER1_STATE_CONFIGED   1   
#define LAYER1_STATE_RUNNING    2   


#define PHY_TRANS_MODE_SINGLE_ANTENNA_PORT_0				0	
#define PHY_TRANS_MODE_TX_DIVERSITY							1	
#define PHY_TRANS_MODE_TX_DIV_LARGE_DELAY_CDD 				2
#define PHY_TRANS_MODE_CLOSED_LOOP_SPATIAL_MUX				3
#define PHY_TRANS_MODE_MULI_USER_MIMO						4
#define PHY_TRANS_MODE_CLOSED_LOOP_RANK_1_PRECODING		    5
#define PHY_TRANS_MODE_SINGLE_ANTENNA_PORT_5				6
#define PHY_TRANS_MODE_NUM									7

       /* DCI UL PDU format defines */
#define PHY_UL_DCI_0_PDU_TYPE 		0
#define PHY_UL_DCI_3_PDU_TYPE		1
#define PHY_UL_DCI_3A_PDU_TYPE		2

       /* DCI DL PDU format defines */
#define PHY_DCI_1_PDU_TYPE 		0
#define PHY_DCI_1A_PDU_TYPE		1
#define PHY_DCI_1B_PDU_TYPE		2
#define PHY_DCI_1C_PDU_TYPE		3
#define PHY_DCI_1D_PDU_TYPE		4
#define PHY_DCI_2_PDU_TYPE 		5
#define PHY_DCI_2A_PDU_TYPE		6

#define MAX_DCI_PDU_NUM 	255
#define MAX_BCH_PDU_NUM 	1               
#define MAX_MCH_PDU_NUM 	1               
#define MAX_DLSCH_PDU_NUM   32          
#define MAX_PCH_PDU_NUM     1           

#define MAX_ULSCH_PDU_NUM   				16
#define MAX_ULSCH_CQI_RI_PDU_NUM    		16  
#define MAX_ULSCH_HARQ_PDU_NUM      		16  
#define MAX_ULSCH_CQI_HARQ_RI_PDU_NUM    	16
#define MAX_UCI_CQI_PDU_NUM         		16
#define MAX_UCI_SR_PDU_NUM         			16
#define MAX_UCI_HARQ_PDU_NUM        		16  
#define MAX_UCI_SR_HARQ_PDU_NUM     		16  
#define MAX_UCI_CQI_HARQ_PDU_NUM      		16  
#define MAX_UCI_CQI_SR_PDU_NUM       		16  
#define MAX_UCI_CQI_SR_HARQ_PDU_NUM    		16  
#define MAX_SRS_PDU_NUM						16
#define MAX_HARQ_BUFFER_PDU_NUM			    16

#define MAX_HI_PDU_NUM 16  
#define MAX_UL_DCI_PDU_NUM 16  


#define MAX_NUM_OF_UE   16       
#define MAX_NUM_OF_RB_PER_UE    100        
#define MAX_NUM_OF_SRS_4RB_GROUPS   (24)

#define MSG_OK                  0 /* Message is OK */
#define MSG_INVALID_STATE       1 /* The received message is not valid in the PHY's current state */
#define MSG_INVALID_CONFIG      2 /* The configuration provided in the CONFIG.request message was invalid */

#define MSG_SFN_OUT_OF_SYNC     3 /* The DL_CONFIG.request was received with a different SFN than the PHY expected */
#define MSG_SUBFRAME_ERR        4 /* An error was received in DL_CONFIG.request or UL_CONFIG.request. The sub-error code should be analyzed */
#define MSG_BCH_MISSING         5 /* A BCH PDU was expected in the DL_CONFIG.request message for this subframe. However, it was not present */
#define MSG_INVALID_SFN         6 /* The received HI_DCI0.request or TX.request message included a SFN/SF value which was not expected. The message has been ignored */
#define MSG_HI_ERR              7 /* An error was received in HI_DCI.request. The sub-error code should be analyzed */
#define MSG_TX_ERR              8 /* An error was received in TX.request. The sub-error code should be analyzed */

#define MSG_PDU_ERR             9       

       /* Measurements messages */
#define    CPHY_MEAS_ON_DEMAND              0
#define    CPHY_MEAS_PERIODIC               1

#define    CPHY_MEAS_RIP_AND_TNP            0     // Recieved Interference Power & Thermal noise

       /*
        * UMTS Sniffer Messages
        */

#define SNIFFER_NORMAL_MODE     0
#define SNIFFER_UMTS_MODE       1
#define SNIFFER_GSM_MODE        2
#define SNIFFER_LTE_MODE        3
       /*
        * CPHY_SNIFFER_CONTROL_REQ:
        * Direction: PS->L1C
        * Description: This message sets L1 to UMTS or GSM sniffing mode
        */
       typedef struct {
          CPHY_MSG_HEADER  header;

          UINT8     mode;                         /* SNIFFER_NORMAL_MODE, SNIFFER_UMTS_MODE, SNIFFER_GSM_MODE, SNIFFER_LTE_MODE */
          UINT16    activationSfn;               /* 0..4095, 0xFFFF – Immediate reconfiguration */
       } CPHY_SNIFFER_CONTROL_REQ;

       /*
        * CPHY_SNIFFER_CONTROL_RSP:
        * Direction: L1C->PS
        * Description: This message indicates that L1C is now ready to receive sniffing mode commands.
        */
       typedef struct {
          CPHY_MSG_HEADER  header;

          UINT16     status;                                                  /* "Success" */
       } CPHY_SNIFFER_CONTROL_RSP;

#define SNIFFER_RTWP_FILTER_WIDE    0
#define SNIFFER_RTWP_FILTER_NARROW  1
#define SNIFFER_RTWP_LIST_MAX_LEN   100
       /*
        * CPHY_SNIFFER_RTWP_REQ:
        * Direction: PS->L1C
        * Description: This message is a request for Received Total Wideband Power measurement.
        */
       typedef struct {
          CPHY_MSG_HEADER  header;

          UINT16     uarfcn;              /* Frequency channel to measure */
          UINT16     RxFilterType;      /* SNIFFER_RTWP_FILTER_WIDE:Wide mode (3.84Mhz)  SNIFFER_RTWP_FILTER_NARROW:narrow mode (300Khz) */                           
       } CPHY_SNIFFER_RTWP_REQ;

       /*
        * CPHY_SNIFFER_RTWP_RSP:
        * Direction: L1C->PS
        * Description: This message is the RTWP measurement result for the CPHY_SNIFFER_RSSI_REQ.
        */
       typedef struct {
          CPHY_MSG_HEADER  header;

          UINT16     status;                                                  /* "Success", "Not in sniffer mode" */
          UINT16     uarfcn;                                                  /* Frequency channel to measure */
          UINT32     receivedTotalWidebandPower;                    
       } CPHY_SNIFFER_RTWP_RSP;

typedef struct
{
     UINT16     uarfcn;                                                  /* Frequency channel measured */
     UINT32     receivedTotalWidebandPower;   
} CPHY_RTWP_RESULT;

/*
 * CPHY_SNIFFER_RTWP_LIST_REQ:
 * Direction: PS->L1C
 * Description: This message is a request for Received Total Wideband Power measurement for multiple uarfcn.
 */
typedef struct 
{
   CPHY_MSG_HEADER  header;

   UINT16     num_of_uarfcn;                       /* 0..SNIFFER_RTWP_LIST_MAX_LEN */
   UINT16     RxFilterType;                        /* SNIFFER_RTWP_FILTER_WIDE:Wide mode (3.84Mhz)  SNIFFER_RTWP_FILTER_NARROW:narrow mode (300Khz) */  
   UINT16     uarfcn[SNIFFER_RTWP_LIST_MAX_LEN];   /* Frequency channel to measure */
} CPHY_SNIFFER_RTWP_LIST_REQ;

/*
 * CPHY_SNIFFER_RTWP_LIST_RSP:
 * Direction: L1C->PS
 * Description: This message is the RTWP measurement result for the CPHY_SNIFFER_RSSI_LIST_REQ.
 */
typedef struct {
   CPHY_MSG_HEADER  header;

   UINT16     status;                                                  /* "Success", "Not in sniffer mode" */
   UINT16     num_of_uarfcn;                                           /* 0..SNIFFER_RTWP_LIST_MAX_LEN */
   CPHY_RTWP_RESULT results[SNIFFER_RTWP_LIST_MAX_LEN];                 
} CPHY_SNIFFER_RTWP_LIST_RSP;

#define	SNIFFER_SEARCH_ALL_CELLS                 0
#define	SNIFFER_SEARCH_STRONGEST_CELL            1
#define	SNIFFER_SEARCH_SPECIFIC_CELL             2
#define	SNIFFER_SEARCH_ALL_CELLS_AFC_LOCK        3
#define SNIFFER_SEARCH_STRONGEST_CELL_AFC_LOCK   4
#define	SNIFFER_SEARCH_SPECIFIC_CELL_AFC_LOCK    5
       /*
        * CPHY_SNIFFER_CELL_SEARCH_REQ:
        * Direction: PS->L1C
        * Description: This message is a request for cell search on a frequency channel.
        */
       typedef struct {
          CPHY_MSG_HEADER  header;

          UINT8     searchtype;                                     /* 0: Cell search for all cells. 1: Cell search for the strongest cell. 2: Cell search for a specific cell. 3: Cell search for all cells with AFC lock  4: Cell search for the strongest cell with AFC lock*/
          UINT16     uarfcn;                                        /* Frequency channel to measure */
          UINT16     primaryScramblingCode;                         /* 0..511, Used in case of option 3 – Cell search for a specific cell */
       } CPHY_SNIFFER_CELL_SEARCH_REQ;

       /*
        * CPHY_SNIFFER_CELL_SEARCH_RSP:
        * Direction: L1C->PS
        * Description: This message contains the neighbors list on a given frequency channel.
        */
       typedef struct {
          UINT16     primaryScramblingCode;                         /* 0..511 */
          INT16      ecio;
          UINT16     position;                                      /* 0..38399 */
          INT16      frequencyOffset;                               /* -500 .. 500 Hz */
          INT32      rscp;                                          /* in dBm. Relevant only for TDS sniffer */
       } SNIFFER_CELL_SEARCH_NEIGHBOUR_TYPE;

       typedef struct {
          CPHY_MSG_HEADER  header;

          UINT16     status;                                          /* "Success", "Not in sniffer mode" */
          UINT16     uarfcn;                                          /* Frequency channel to measure */
          UINT16     numberOfNeighbours;                              /* 0..16 */
          SNIFFER_CELL_SEARCH_NEIGHBOUR_TYPE neighboursList[16];
       } CPHY_SNIFFER_CELL_SEARCH_RSP;

       /*
        * CPHY_SNIFFER_READ_BCH_REQ:
        * Direction: PS->L1C
        * Description: This message starts reading of BCH information from a neighbor cell.
        */
       typedef struct {
          CPHY_MSG_HEADER  header;

          UINT16     uarfcn;                                        /* Frequency channel to measure */
          UINT16     primaryScramblingCode;                         /* 0..511 */
       } CPHY_SNIFFER_READ_BCH_REQ;

       /*
        * CPHY_SNIFFER_READ_BCH_RSP:
        * Direction: L1C->PS
        * Description: This message is a confirmation for CPHY_SNIFFER_READ_BCH_REQ.
        */
       typedef struct {
          CPHY_MSG_HEADER  header;

          UINT16     status;                                                  /* "Success", "Not in sniffer mode" */
       } CPHY_SNIFFER_READ_BCH_RSP;

       /*
        * PHY_SNIFFER_BCH_DATA_IND:
        * Direction: L1C->PS
        * Description: This message is a confirmation for CPHY_SNIFFER_READ_BCH_REQ.
        */
       typedef struct {
          PHY_MSG_HEADER  header;

          UINT8    crcStatus;                /* 0..1, 0 – Correct, 1 – Error */
          UINT16   localSfn;              /* The radio frame the data was received locally on this cell - Relevant only for TDS sniffer */
          UINT16   nbrCellSfn;            /* Neighbor cell SFN corresponding to local SFN - Relevant only for TDS sniffer */
          INT32    nbrCellPosition;       /* In units of half a chip. Position from frame boundary. Relevant only for TDS sniffer */
          UINT8    dataPtr[31];              /* Pointer to bit-string of 246 bits */
       } PHY_SNIFFER_BCH_DATA_IND;

       /*
        * CPHY_SNIFFER_STOP_BCH_REQ:
        * Direction: PS->L1C
        * Description: This message ends reading of BCH information.
        */
       typedef struct {
          CPHY_MSG_HEADER  header;

       } CPHY_SNIFFER_STOP_BCH_REQ;

       /*
        * CPHY_SNIFFER_STOP_BCH_RSP:
        * Direction: L1C->PS
        * Description: This message is a confirmation for CPHY_SNIFFER_STOP_BCH_REQ.
        */
       typedef struct {
          CPHY_MSG_HEADER  header;

          UINT16     status;                                                  /* "Success", "Not in sniffer mode" */
       } CPHY_SNIFFER_STOP_BCH_RSP;

       /*
        * GSM Sniffer
        */

       /*
        * CPHY_GSM_READ_FCCH_REQ:
        * Direction: PS->L1C
        * Description: This message is a request for a Frequency Correction Channel read.
        */
       typedef struct {
          CPHY_MSG_HEADER  header;

          UINT16     uarfcn;                       /* DL Frequency channel. Refer to 25.101 chapter 5.4 */
          BOOL8      afcLock;                      /* True - Lock AFC, False - Read without lock */
       } CPHY_GSM_READ_FCCH_REQ;


       /*
        * CPHY_GSM_READ_FCCH_RSP:
        * Direction: L1C->PS
        * Description: This message is a confirmation for CPHY_GSM_READ_FCCH_REQ.
        */
       typedef struct {
          CPHY_MSG_HEADER  header;

          UINT16		status;
          UINT16     uarfcn;                        /* DL Frequency channel. Refer to 25.101 chapter 5.4 */
          INT16		frequencyOffset;              /* -10000..10000Hz. Neighbour carrier frequency offset in Hertz */
          UINT32		totalPower;                   /* FCCH total power*/
          UINT32		peakPower;                    /* FCCH peak power*/
       } CPHY_GSM_READ_FCCH_RSP;


       /*
        * CPHY_GSM_READ_BSIC_REQ:
        * Direction: PS->L1C
        * Description: This message is a request for a BSIC (Base Station Identity Code) information.
        */
       typedef struct {
          CPHY_MSG_HEADER  header;

          UINT16     uarfcn;                        /* DL Frequency channel. Refer to 25.101 chapter 5.4 */
       } CPHY_GSM_READ_BSIC_REQ;

       /*
        * CPHY_GSM_READ_BSIC_RSP:
        * Direction: L1C->PS
        * Description: This message is a BSIC information reply.
        */
       typedef struct {
          CPHY_MSG_HEADER  header;

          UINT16    status;
          UINT16    uarfcn;                        /* DL Frequency channel. Refer to 25.101 chapter 5.4 */
          UINT8     ncc;                          /* 0..7, Network Color Code, 0xFF – Failed to get BSIC */
          UINT8     bcc;                          /* 0..7, Base Station Color Code, 0xFF – Failed to get BSIC */
          INT16     frequencyOffset;             /* -10000..10000Hz. Neighbour carrier frequency offset in Hertz */
          UINT32    totalPower;                  /* FCCH total power*/
          UINT32    peakPower;                   /* FCCH peak power*/
       } CPHY_GSM_READ_BSIC_RSP;

       /*
        * CPHY_GSM_BCCH_READ_CONTROL_REQ:
        * Direction: PS->L1C
        * Description: This message is a request for a BCCH readings on the last cell with successful BSIC reading.
        */
       typedef struct {
          CPHY_MSG_HEADER  header;

          BOOL8     control;                        /* True – Start BCCH read, False – Stop BCCH read */
          UINT16    uarfcn;                         /* DL Frequency channel. Refer to 25.101 chapter 5.4 */
       } CPHY_GSM_BCCH_READ_CONTROL_REQ;

       /*
        * CPHY_GSM_BCCH_READ_CONTROL_RSP:
        * Direction: L1C->PS
        * Description: This message is a confirmation for the CPHY_GSM_BCCH_READ_CONTROL_REQ message.
        */
       typedef struct {
          CPHY_MSG_HEADER  header;

          UINT16    status;
          UINT16    uarfcn;                      /* DL Frequency channel. Refer to 25.101 chapter 5.4 */
          INT16     frequencyOffset;             /* -10000..10000Hz. Neighbour carrier frequency offset in Hertz */
          UINT32    totalPower;                  /* FCCH total power*/
          UINT32    peakPower;                   /* FCCH peak power*/
       } CPHY_GSM_BCCH_READ_CONTROL_RSP;

       /*
        * CPHY_GSM_BCCH_IND:
        * Direction: L1C->PS
        * Description: This message contains one multi-frame of BCCH.
        */
#define MAX_BCCH_DATA_SIZE     23

       typedef struct {
          CPHY_MSG_HEADER  header;

          UINT8     bcchCrcStatus;                                 /* 0..1, 0 – Correct, 1 – Error */
          UINT8     extendedBcchCrcStatus;                         /* 0..1, 0 – Correct, 1 – Error */
          UINT8     bcchData[MAX_BCCH_DATA_SIZE];                  /* 184 bit string */
          UINT8     extendedBcchData[MAX_BCCH_DATA_SIZE];          /* 184 bit string */
       } CPHY_GSM_BCCH_IND;


#define  LTE_SNIFFER_MAX_CELL_SEARCH  50

       /*
        * CPHY_LTE_SNIFFER_CELL_SEARCH_REQ:
        * Direction: PS->L1C
        * Description: This message is a request for lte cell search.
        */
       typedef struct {
          CPHY_MSG_HEADER  header;

          UINT8     SearchType;                       /* 0: search for all cells.  1:  search for strongest cell.  2: Search for specific cell  */
          UINT16    earfcn;                           /* Frequency channel. */
          BOOL8     MibRequest;                       /* TRUE: With MIB information */
          UINT8     measBandwidth;                    /* # of resource blocks for measuring RSRP: 6. 15. 25. 50. 75. 100 */
          UINT16    nr_pciList;                       /* Used in case of search for specific cells. Number of cells to be searched */
          UINT16    pci[LTE_SNIFFER_MAX_CELL_SEARCH]; /* List of cells to be searched */
       } CPHY_LTE_SNIFFER_CELL_SEARCH_REQ;


       typedef struct {
          UINT16    sfn;           /* The radio frame the data was receieved on */
          UINT8     numTx;         /* Number of TX antgennas of the cell */
          UINT8     phichDuration; /* The PHICH duration of the cell. 0-Normal  1-Extended */
          UINT8     phichResource; /* 0:1/6  1:1/2  2:1  3:2 */
          UINT8     dlSystemBanWidth; /* 6, 15, 25, 50, 75, 100 */
       } CPHY_MIB_PARAMS;

       typedef struct {
          UINT16    pci;
          UINT8     RSRP;           /* 0..128 (-144..-16) Value in dBm */
          UINT32    position;       /* Frame boundary in chips */
          UINT16    local_sfn;
          INT16     FrequencOffset; /* -3500..3500 in Herz */
          UINT8     cause;          /* 0:MIB OK  1: MIB_FAILED */ 
          CPHY_MIB_PARAMS MibInfo;
       } CPHY_LTE_CELL_SEARCH_INFO;
       /*
        * CPHY_LTE_SNIFFER_CELL_SEARCH_RSP:
        * Direction: L1C->PS
        * Description: This message is a response for the cell search request.
        */
       typedef struct {
          CPHY_MSG_HEADER  header;

          UINT16    status;
          UINT16    earfcn;                          /* Frequency channel. */
          UINT16    NumberOfNeighbours;              /* Number of cells found */

          CPHY_LTE_CELL_SEARCH_INFO    pci[LTE_SNIFFER_MAX_CELL_SEARCH]; /* List of cells to be searched */
       } CPHY_LTE_SNIFFER_CELL_SEARCH_RSP;


       /*
        * CPHY_LTE_SNIFFER_READ_SIB1_REQ:
        * Direction: PS->L1C
        * Description: This message is a request for SIB1.
        */
       typedef struct {
          CPHY_MSG_HEADER  header;

          UINT16    earfcn;       /* Frequency channel. */
          UINT16    pci;          /* cell id */
          UINT8     retryCount;   /* 1..8 */
       } CPHY_LTE_SNIFFER_READ_SIB1_REQ;

#define CPHY_LTE_MAX_BCCH_SIZE 280

       /*
        * CPHY_LTE_SNIFFER_READ_SIB1_RSP:
        * Direction: L1C->PS
        * Description: This message is a response for CPHY_LTE_SNIFFER_READ_SIB1_REQ.
        */
       typedef struct {
          CPHY_MSG_HEADER  header;

          UINT16    earfcn;       /* Frequency channel. */
          UINT16    pci;          /* cell id */
          UINT16    sfnSf;        /* SfnSf the data was recieved on */
          UINT16    payloadLength; /* Length in bytes */
          UINT8     payload[CPHY_LTE_MAX_BCCH_SIZE];
       } CPHY_LTE_SNIFFER_READ_SIB1_RSP;


#define CPHY_LTE_MAX_SIB_PERIODS  32

       /*
        * CPHY_LTE_SNIFFER_READ_SIB_REQ:
        * Direction: PS->L1C
        * Description: This message is a request for SIB.
        */
       typedef struct {
          CPHY_MSG_HEADER  header;

          UINT16    earfcn;       /* Frequency channel. */
          UINT16    pci;          /* cell id */
          UINT8     NumOfPeriodicity;
          UINT16    siPeriodicity[CPHY_LTE_MAX_SIB_PERIODS];
          UINT8     siWindow;     /* 1, 2, 5, 10, 15, 20, 40 */
          UINT8     retryCount;   /* 1..8 */
          UINT8     subframeAssignment; /* 0..6 (For TDD mode) */
          UINT8     specialSubframePatterns; /* 0..8 (For TDD mode) */
       } CPHY_LTE_SNIFFER_READ_SIB_REQ;

       /*
        * CPHY_LTE_SNIFFER_READ_SIB_RSP:
        * Direction: L1C->PS
        * Description: This message is a response for CPHY_LTE_SNIFFER_READ_SIB1_REQ.
        */
       typedef struct {
          CPHY_MSG_HEADER  header;

          UINT16    status;
       } CPHY_LTE_SNIFFER_READ_SIB_RSP;


       /*
        * CPHY_LTE_SNIFFER_READ_SIB_IND:
        * Direction: L1C->PS
        * Description: This message returns the SIB for the requested cell.
        */
       typedef struct {
          CPHY_MSG_HEADER  header;

          UINT16    earfcn;       /* Frequency channel. */
          UINT16    pci;          /* cell id */
          UINT16    sfnSf;        /* SfnSf the data was recieved on */
          UINT16    payloadLength; /* Length in bytes */
          UINT8     payload[CPHY_LTE_MAX_BCCH_SIZE];
       } CPHY_LTE_SNIFFER_READ_SIB_IND;


       /*
        * CPHY_LTE_SNIFFER_STOP_READ_SIB_REQ:
        * Direction: PS->L1C
        * Description: This message is a request to stop the SIB read operation.
        */
       typedef struct {
          CPHY_MSG_HEADER  header;
       } CPHY_LTE_SNIFFER_STOP_READ_SIB_REQ;

       /*
        * CPHY_LTE_SNIFFER_STOP_READ_SIB_RSP:
        * Direction: L1C->PS
        * Description: This message is a response for CPHY_LTE_SNIFFER_STOP_READ_SIB_REQ.
        */
       typedef struct {
          CPHY_MSG_HEADER  header;
       } CPHY_LTE_SNIFFER_STOP_READ_SIB_RSP;


#define CPHY_SPI_MAX_VALUES 6

#define     PRC_PROCESS_PS                    0
#define     PRC_PROCESS_RRM                   1
#define     PRC_PROCESS_MAINTENANCE           2

#define PRC_TRACER_THREAD_ID        0
#define PRC_L1B_THREAD_ID           1
#define PRC_SNIFFER_THREAD_ID       2
#define PRC_L1C_THREAD_ID           3
#define PRC_L1D_THREAD_ID           4
#define PRC_COMMS_THREAD_ID         5
#define PRC_SHELL_THREAD_ID         6
#define PRC_PSE_THREAD_ID           7
#define PRC_FREE_HANDLER_THREAD_ID  8
#define PRC_SCHED_THREAD_ID         9
#define PRC_DSP_PRINTS_THREAD_ID   10

#ifdef __cplusplus
    }
#endif /* __cplusplus */
#endif /* _PHY_INTERFACE_API_H_ */


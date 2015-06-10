/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : MtaRrcInterface.h
  �� �� ��   : ����
  ��������   : 2012��06��01��
  ����޸�   :
  ��������   : MTA��GU�����֮��Ľӿ�ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��06��01��
    �޸�����   : �����ļ�

 ******************************************************************************/

#ifndef __MTARRCINTERFACE_H__
#define __MTARRCINTERFACE_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "PsTypeDef.h"
#include "AgpsParaDef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(4)

/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define  NMR_MAX_FREQ_NUM                       (3)  /* �ϱ�NMR���ݵ�Ƶ��������,W���ֻ֧������Ƶ�㣬TD����ʱ���� */
#define  NMR_MAX_CELL_FREQ_NUM                  (32) /* �ϱ�NMR���ݵ�С�������������� */
#define  RRC_MTA_UTRAN_NMR                      (0)
#define  RRC_MTA_GSM_NMR                        (1)
#define  RRC_MTA_NMR_MAX_GSM_ELEM_NUM           (15) /* �ϱ�NMR���ݵ�GSMԪ�ص������� */

#define  MTA_WRR_AUTOTEST_MAX_PARA_NUM          (10) /* autotest�����·����������� */
#define  WRR_MTA_AUTOTEST_MAX_RSULT_NUM         (20) /* autotest�����ϱ���������� */
#define  WRR_MTA_MEANRPT_MAX_CELL_NUM           (8)  /* һ�β���������С�������� */
#define  WRR_MTA_MAX_MEANRPT_NUM                (10) /* �ϱ������������������� */

#define  WRR_MTA_MAX_NCELL_NUM                  (8)  /* WAS����������� */

#define  MTA_LRRC_MSG_TYPE_BASE                 (0x1000)            /* MTAģ����LTE�������Ϣ���� */
#define  LRRC_MAX_NCELL_NUM                     (8)

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
/*****************************************************************************
 ö����    : MTA_RRC_MSG_TYPE_ENUM
 �ṹ˵��  : ԭ���usMsgName
 1.��    ��   : 2012��06��01��
   �޸�����   : Added for AGPS
 2.��    ��   : 2012��11��23��
   �޸�����   : ����NMR����MTA��GU�����֮�����Ϣ
 3.��    ��   : 2012��12��26��
   �޸�����   : �����WAS���AT�������ϢID
 4.��    ��   : 2013��5��31��
   �޸�����   : V9R1_SVLTE:�����GAS���AT�������ϢID
*****************************************************************************/
enum MTA_RRC_MSG_TYPE_ENUM
{
    /* ��Ϣ���� */                              /* ��ϢID */                    /* ��ע */
    ID_RRC_MTA_MSG_POSITION_REQ                    = 0x0001,                    /* _H2ASN_MsgChoice MTA_RRC_POSITION_REQ_STRU */

    ID_MTA_RRC_QRY_NMR_REQ                         = 0x0003,                    /* _H2ASN_MsgChoice MTA_RRC_QRY_NMR_REQ_STRU */

    ID_MTA_RRC_RESEL_OFFSET_CFG_NTF                = 0x0005,                    /* _H2ASN_MsgChoice MTA_RRC_RESEL_OFFSET_CFG_NTF_STRU */

    ID_MTA_WRR_AUTOTEST_QRY_REQ                    = 0x0007,                    /* _H2ASN_MsgChoice MTA_WRR_AUTOTEST_QRY_REQ_STRU     */

    ID_MTA_WRR_CELLINFO_QRY_REQ                    = 0x0009,                    /* _H2ASN_MsgChoice MTA_WRR_CELLINFO_QRY_REQ_STRU     */

    ID_MTA_WRR_MEASRPT_QRY_REQ                     = 0x000B,                    /* _H2ASN_MsgChoice MTA_WRR_MEANRPT_QRY_REQ_STRU      */

    ID_MTA_WRR_FREQLOCK_SET_REQ                    = 0x000D,                    /* _H2ASN_MsgChoice MTA_WRR_FREQLOCK_SET_REQ_STRU     */

    ID_MTA_WRR_RRC_VERSION_SET_REQ                 = 0x000F,                    /* _H2ASN_MsgChoice MTA_WRR_RRC_VERSION_SET_REQ_STRU  */

    ID_MTA_WRR_CELLSRH_SET_REQ                     = 0x0011,                    /* _H2ASN_MsgChoice MTA_WRR_CELLSRH_SET_REQ_STRU      */

    ID_MTA_WRR_FREQLOCK_QRY_REQ                    = 0x0013,                    /* _H2ASN_MsgChoice MTA_WRR_FREQLOCK_QRY_REQ_STRU     */

    ID_MTA_WRR_RRC_VERSION_QRY_REQ                 = 0x0015,                    /* _H2ASN_MsgChoice MTA_WRR_RRC_VERSION_QRY_REQ_STRU  */

    ID_MTA_WRR_CELLSRH_QRY_REQ                     = 0x0017,                    /* _H2ASN_MsgChoice MTA_WRR_CELLSRH_QRY_REQ_STRU      */

    ID_MTA_GRR_NCELL_MONITOR_SET_REQ               = 0x0019,                    /* _H2ASN_MsgChoice MTA_GRR_NCELL_MONITOR_SET_REQ_STRU      */

    ID_MTA_GRR_NCELL_MONITOR_QRY_REQ               = 0x001B,                    /* _H2ASN_MsgChoice MTA_GRR_NCELL_MONITOR_QRY_REQ_STRU      */

    ID_MTA_RRC_MSG_POSITION_CNF                    = 0x0002,                    /* _H2ASN_MsgChoice MTA_RRC_POSITION_CNF_STRU */

    ID_RRC_MTA_QRY_NMR_CNF                         = 0x0004,                    /* _H2ASN_MsgChoice RRC_MTA_QRY_NMR_CNF_STRU */

    ID_WRR_MTA_AUTOTEST_QRY_CNF                    = 0x0008,                    /* _H2ASN_MsgChoice WRR_MTA_AUTOTEST_QRY_CNF_STRU     */

    ID_WRR_MTA_CELLINFO_QRY_CNF                    = 0x000A,                    /* _H2ASN_MsgChoice WRR_MTA_CELLINFO_QRY_CNF_STRU     */

    ID_WRR_MTA_MEASRPT_QRY_CNF                     = 0x000C,                    /* _H2ASN_MsgChoice WRR_MTA_MEANRPT_QRY_CNF_STRU      */

    ID_WRR_MTA_FREQLOCK_SET_CNF                    = 0x000E,                    /* _H2ASN_MsgChoice WRR_MTA_FREQLOCK_SET_CNF_STRU     */

    ID_WRR_MTA_RRC_VERSION_SET_CNF                 = 0x0010,                    /* _H2ASN_MsgChoice WRR_MTA_RRC_VERSION_SET_CNF_STRU  */

    ID_WRR_MTA_CELLSRH_SET_CNF                     = 0x0012,                    /* _H2ASN_MsgChoice WRR_MTA_CELLSRH_SET_CNF_STRU      */

    ID_WRR_MTA_FREQLOCK_QRY_CNF                    = 0x0014,                    /* _H2ASN_MsgChoice WRR_MTA_FREQLOCK_QRY_CNF_STRU     */

    ID_WRR_MTA_RRC_VERSION_QRY_CNF                 = 0x0016,                    /* _H2ASN_MsgChoice WRR_MTA_RRC_VERSION_QRY_CNF_STRU  */

    ID_WRR_MTA_CELLSRH_QRY_CNF                     = 0x0018,                    /* _H2ASN_MsgChoice WRR_MTA_CELLSRH_QRY_CNF_STRU      */


    ID_GRR_MTA_NCELL_MONITOR_SET_CNF              = 0x001A,                    /* _H2ASN_MsgChoice GRR_MTA_NCELL_MONITOR_SET_CNF_STRU      */

    ID_GRR_MTA_NCELL_MONITOR_QRY_CNF              = 0x001C,                    /* _H2ASN_MsgChoice GRR_MTA_NCELL_MONITOR_QRY_CNF_STRU      */

    ID_GRR_MTA_NCELL_MONITOR_IND                  = 0x001E,                    /* _H2ASN_MsgChoice GRR_MTA_NCELL_MONITOR_IND_STRU      */

    /* ��Ϣ����MTA->LRRC */
    ID_MTA_LRRC_CELLINFO_QRY_REQ                = MTA_LRRC_MSG_TYPE_BASE + 1,   /* _H2ASN_MsgChoice MTA_LRRC_CELLINFO_QRY_REQ_STRU      */

    /* ��Ϣ����LRRC->MTA */
    ID_LRRC_MTA_CELLINFO_QRY_CNF                = MTA_LRRC_MSG_TYPE_BASE + 2,   /* _H2ASN_MsgChoice LRRC_MTA_CELLINFO_QRY_CNF_STRU      */

    ID_MTA_RRC_MSG_TYPE_BUTT
};
typedef VOS_UINT32 MTA_RRC_MSG_TYPE_ENUM_UINT32;

/*****************************************************************************
 ö����    : MTA_RRC_RESULT_ENUM
 �ṹ˵��  : MTAģ���������Ľ����ö��

  1.��    ��   : 2013��12��09��
    �޸�����   : ����
*****************************************************************************/
enum MTA_RRC_RESULT_ENUM
{
    MTA_RRC_RESULT_NO_ERROR                     = 0x000000,                     /* ��Ϣ�������� */
    MTA_RRC_RESULT_ERROR,                                                       /* ��Ϣ������� */

    MTA_RRC_RESULT_BUTT
};
typedef VOS_UINT32 MTA_RRC_RESULT_ENUM_UINT32;

/*******************************************************************************
 ö����    : NMR_UTRAN_MEASURED_TYPE_ENUM
 �ṹ˵��  : NMR������Ϣ����ѡ��:FDD TDD
 1.��    ��   : 2012��11��23��
   �޸�����   : Added for NMR
*******************************************************************************/
enum NMR_UTRAN_MEASURED_TYPE_ENUM
{
    NMR_UTRAN_MEASURED_TYPE_FDD = 0,
    NMR_UTRAN_MEASURED_TYPE_TDD,
    NMR_UTRAN_MEASURED_TYPE_BUTT
};
typedef VOS_UINT32 NMR_UTRAN_MEASURED_TYPE_ENUM_UINT32;

/*******************************************************************************
 ö����    : GRR_MTA_NCELL_STATE_ENUM
 �ṹ˵��  : ����״̬ö��
 1.��    ��   : 2013��06��04��
   �޸�����   : for V9R1_SVLTE
*******************************************************************************/
enum GRR_MTA_NCELL_STATE_ENUM
{
    GRR_MTA_NCELL_STATE_NULL    = 0x00,                                         /* ��û��TD��������Ҳû��4G���� */
    GRR_MTA_NCELL_STATE_3G      = 0x01,                                         /* ֻ��TD������ */
    GRR_MTA_NCELL_STATE_4G      = 0x02,                                         /* ֻ��4G������ */
    GRR_MTA_NCELL_STATE_3G4G    = 0x03,                                         /* ����TD��������Ҳ��4G���� */
    GRR_MTA_NCELL_STATE_BUTT

};
typedef VOS_UINT8 GRR_MTA_NCELL_STATE_ENUM_UINT8;

/*****************************************************************************
  4 ȫ�ֱ�������
*****************************************************************************/

/*****************************************************************************
  5 ��Ϣͷ����
*****************************************************************************/

/*****************************************************************************
  6 ��Ϣ����
*****************************************************************************/

/*****************************************************************************
  7 STRUCT����
*****************************************************************************/
/*******************************************************************************
 �ṹ��    : MTA_RRC_POSITION_REQ_STRU
 �ṹ˵��  : ����㷢�͸�GPSоƬ�Ķ�λ����Ĳ���
 1.��    ��   : 2012��06��01��
   �޸�����   : Added for AGPS
*******************************************************************************/
typedef struct
{
    MSG_HEADER_STRU                             stMsgHeader;                    /*_H2ASN_Skip*/

    VOS_UINT32                                  bitOpAssistData   : 1;
    VOS_UINT32                                  bitOpPosMeas      : 1;
    VOS_UINT32                                  bitOpSpare        : 30;

    PS_BOOL_ENUM_UINT8                          enDeleteAssistDataFlag;         /* �Ƿ���Ҫ����������ݵı�� */
    VOS_UINT8                                   aucRsv[3];                      /* ����λ */
    AGPS_ASSIST_DATA_STRU                       stAssistData;                   /* assist_data,�������� */
    AGPS_MEASURE_REQ_STRU                       stPosMeas;                      /* pos_meas,λ�ò������� */
}MTA_RRC_POSITION_REQ_STRU;

/*******************************************************************************
 �ṹ��    : MTA_RRC_POSITION_CNF_STRU
 �ṹ˵��  : GPS�Ķ�λ�ظ�
 1.��    ��   : 2012��06��01��
   �޸�����   : Added for AGPS
*******************************************************************************/
typedef struct
{
    MSG_HEADER_STRU                                     stMsgHeader;            /*_H2ASN_Skip*/

    VOS_UINT8                                           aucRsv[3];              /* ����λ */
    AGPS_POSITION_RESULT_TYPE_ENUM_UINT8                enResultTypeChoice;     /* �����Ϣ���� */
    union
    {
        AGPS_LOCATION_INFO_STRU                         stLocationInfo;         /* location,λ����Ϣ */
        AGPS_GPS_MEASURE_INFO_STRU                      stGpsMeasInfo;          /* GPS_meas,GPS������Ϣ */
        AGPS_POSITION_GPS_ADDITION_ASSIST_REQ_STRU      stAssistDataReq;        /* GPS_assist_req,������������ */
        AGPS_POSITION_ERR_STRU                          stPosErr;               /* pos_err,λ�ô�����Ϣ */
    }u;
}MTA_RRC_POSITION_CNF_STRU;

/*******************************************************************************
 �ṹ��    : MTA_RRC_RESEL_OFFSET_CFG_NTF
 �ṹ˵��  : W��L��ѡ���Ͳ�������
 1.��    ��   : 2012��12��07��
   �޸�����   : �����ṹ

*******************************************************************************/
typedef struct
{
    MSG_HEADER_STRU                             stMsgHeader;                    /*_H2ASN_Skip*/
    PS_BOOL_ENUM_UINT8                          enOffsetEnable;                 /* 0:������1:����*/
    VOS_UINT8                                   aucRsv[3];                      /* ����λ*/
}MTA_RRC_RESEL_OFFSET_CFG_NTF_STRU;


/*******************************************************************************
 �ṹ��    : NMR_UTRAN_FREQUENCY_INFO_FDD_STRU
 �ṹ˵��  : NMR Ƶ����Ϣ

 1.��    ��   : 2012��11��23��
   �޸�����   : Added for NMR

   FrequencyInfoFDD ::= SEQUENCE {uarfcn-UL UARFCN OPTIONAL,
                                  uarfcn-DL UARFCN,...}
*******************************************************************************/
typedef struct
{
    VOS_UINT32                                  bitOpUarfcn_UL        : 1;
    VOS_UINT32                                  bitOpSpare            : 31;
    VOS_UINT16                                  usUlUarfcn;           /* ����Ƶ�� */
    VOS_UINT16                                  usDlUarfcn;           /* ����Ƶ�� */
}NMR_UTRAN_FREQUENCY_INFO_FDD_STRU;

/*******************************************************************************
 �ṹ��    : NMR_UTRAN_FREQUENCY_INFO_STRU
 �ṹ˵��  : NMR FrequencyInfo�ṹ
 1.��    ��   : 2012��11��23��
   �޸�����   : Added for NMR
*******************************************************************************/
typedef struct
{
    NMR_UTRAN_MEASURED_TYPE_ENUM_UINT32           enFreqInfoChoice;     /* NMRƵ����Ϣ����:0:FDD,1:TDD */
    union
    {
        NMR_UTRAN_FREQUENCY_INFO_FDD_STRU         stFreqInfoFDD;         /* fdd */
        VOS_UINT32                                ulFreqInfoTDD;          /*  TDD */
    }u;

}NMR_UTRAN_FREQUENCY_INFO_STRU;
/*******************************************************************************
 �ṹ��    : NMR_UTRAN_CELL_MEAS_RESULTS_FDD_STRU
 �ṹ˵��  : NMR Cell Measured Results FDD
 1.��    ��   : 2012��11��23��
   �޸�����   : Added for NMR
*******************************************************************************/

typedef struct
{
    VOS_UINT32                                  bitOpEcn0      : 1;
    VOS_UINT32                                  bitOpRscp      : 1;
    VOS_UINT32                                  bitOpPathloss  : 1;
    VOS_UINT32                                  bitOpSpare     : 29;

    VOS_UINT16                                  usCellPSC;
    VOS_UINT8                                   ucEcn0;              /* INTEGER(0..63) */
    VOS_UINT8                                   ucRscp;              /*  INTEGER(0..127)*/
    VOS_UINT8                                   ucPathloss;           /* INTEGER(46..173)  */
    VOS_UINT8                                   aucRsv[3];


}NMR_UTRAN_CELL_MEAS_RESULTS_FDD_STRU;

/*******************************************************************************
 �ṹ��    : NMR_UTRAN_MEAS_RESULTS_STRU
 �ṹ˵��  : NMR Cell Measured Results TDD
 1.��    ��   : 2012��11��23��
   �޸�����   : Added for NMR
*******************************************************************************/
typedef struct
{
    VOS_UINT32                                  bitOpProposedTGSN  : 1;
    VOS_UINT32                                  bitOpRscp          : 1;
    VOS_UINT32                                  bitOpPathloss      : 1;
    VOS_UINT32                                  bitOpTimeslotISCP  : 1;
    VOS_UINT32                                  bitOpSpare         : 28;

    VOS_UINT8                                   ucCellParaID;         /* INTEGER(0..127) */
    VOS_UINT8                                   ucProposedTGSN;       /* INTEGER(0..14)  */
    VOS_UINT8                                   ucRscp;               /* INTEGER(0..127) */
    VOS_UINT8                                   ucPathloss;           /* INTEGER(46..173) */
    VOS_UINT8                                   ucTimeslotISCP;       /* INTEGER(46..173) */
    VOS_UINT8                                   aucRsv[3];

}NMR_UTRAN_CELL_MEAS_RESULTS_TDD_STRU;

/*******************************************************************************
 �ṹ��    : NMR_UTRAN_CELL_MEAS_RESULTS_STRU
 �ṹ˵��  : NMR Cell Measured Results
 1.��    ��   : 2012��11��23��
   �޸�����   : Added for NMR
*******************************************************************************/
typedef struct
{
    VOS_UINT32                                  bitOpCellID               : 1;
    VOS_UINT32                                  bitOpSpare                : 31;

    VOS_UINT32                                  ulCellID;                 /*  */
    NMR_UTRAN_MEASURED_TYPE_ENUM_UINT32         enCellMeasTypeChoice;     /* NMRƵ����Ϣ����:FDD,TDD */
    union
    {
        NMR_UTRAN_CELL_MEAS_RESULTS_FDD_STRU    stCellMeasResultsFDD;         /* FDD */
        NMR_UTRAN_CELL_MEAS_RESULTS_TDD_STRU    stCellMeasResultsTDD;         /*  TDD */
    }u;

}NMR_UTRAN_CELL_MEAS_RESULTS_STRU;

/*******************************************************************************
 �ṹ��    : NMR_MEASURED_RESULTS_LIST_STRU
 �ṹ˵��  : NMR Cell Measured Results List
 1.��    ��   : 2012��11��23��
   �޸�����   : Added for NMR
*******************************************************************************/
typedef struct
{
    VOS_UINT32                                  ulMeasCellNum;            /* (0,32) */
    NMR_UTRAN_CELL_MEAS_RESULTS_STRU            astCellMeasResults[NMR_MAX_CELL_FREQ_NUM];
}NMR_UTRAN_CELL_MEAS_RESULTS_LIST_STRU;

/*******************************************************************************
 �ṹ��    : NMR_UTRAN_MEASURED_RESULTS_STRU
 �ṹ˵��  : 3Gģʽ�²������
 1.��    ��   : 2012��11��23��
   �޸�����   : Added for NMR
*******************************************************************************/
typedef struct
{
    VOS_UINT32                                  bitOpFrequencyInfo        : 1;
    VOS_UINT32                                  bitOpUTRA_CarrierRSSI     : 1;
    VOS_UINT32                                  bitOpCellMeasResultsList  : 1;
    VOS_UINT32                                  bitOpSpare                : 29;

    NMR_UTRAN_FREQUENCY_INFO_STRU               stFrequencyInfo;
    VOS_UINT32                                  ulUTRA_CarrierRSSI; /* Ƶ���RSSI */
    NMR_UTRAN_CELL_MEAS_RESULTS_LIST_STRU       stCellMeasResultsList;
}NMR_UTRAN_MEASURED_RESULTS_STRU;

/*******************************************************************************
 �ṹ��    : RRC_MTA_UTRAN_NMR_STRU
 �ṹ˵��  : 3Gģʽ��NMR������Ϣ
 1.��    ��   : 2012��11��23��
   �޸�����   : Added for NMR
*******************************************************************************/
typedef struct
{
    VOS_UINT32                                ulFreqNum;            /* (0,8) */
    NMR_UTRAN_MEASURED_RESULTS_STRU           astMeasResults[NMR_MAX_FREQ_NUM];
}RRC_MTA_UTRAN_NMR_STRU;

/*******************************************************************************
 �ṹ��    : RRC_MTA_GSM_NMR_ELEMENT_STRU
 �ṹ˵��  : 2Gģʽ��NMR ELEMENT������Ϣ
 1.��    ��   : 2012��11��26��
   �޸�����   : Added for NMR
*******************************************************************************/
typedef struct
{
    VOS_UINT16                                usArfcn;            /* С������Ƶ���, ȡֵ��Χ[0,1023] */
    VOS_UINT8                                 ucBsic;             /* С��BSIC, [0,63] */
    VOS_UINT8                                 ucRxlev;            /* С��������ƽ, �Ѿ�ӳ��Ϊ[0,63]��ȡֵ��Χ��ȡֵ */
}RRC_MTA_GSM_NMR_ELEMENT_STRU;


/*******************************************************************************
 �ṹ��    : RRC_MTA_GSM_NMR_STRU
 �ṹ˵��  : 2Gģʽ��NMR������Ϣ
 1.��    ��   : 2012��11��26��
   �޸�����   : Added for NMR
*******************************************************************************/
typedef struct
{
    VOS_UINT32                                ulElemNum;                        /* GSM NMRԪ�ظ��� [0,15] */

    /* GSM NMRԪ������, ��һ��Ԫ�ط���С��, ֮���Ԫ����GSM����, GSM���������ź�ǿ���ɸߵ������� */
    RRC_MTA_GSM_NMR_ELEMENT_STRU              astNmrElem[RRC_MTA_NMR_MAX_GSM_ELEM_NUM];
}RRC_MTA_GSM_NMR_STRU;

/*****************************************************************************
 �ṹ��    : MTA_RRC_QRY_NMR_REQ_STRU
 �ṹ˵��  : ID_MTA_RRC_QRY_NMR_REQ��Ϣ�ṹ
 1.��    ��   : 2012��11��23��
   �޸�����   : ����
*****************************************************************************/
typedef struct
{
    MSG_HEADER_STRU                     MsgHeader;          /* ��Ϣͷ    */     /*_H2ASN_Skip*/
    /* MTA_RRC_MSG_TYPE_ENUM_UINT32        enMsgId; */

    VOS_UINT8                           aucReserve[4];                          /* Ԥ������ʹ�� */
} MTA_RRC_QRY_NMR_REQ_STRU;

/*****************************************************************************
 �ṹ��    : RRC_MTA_QRY_NMR_CNF_STRU
 �ṹ˵��  : ID_RRC_MTA_QRY_NMR_CNF��Ϣ�ṹ
 1.��    ��   : 2012��11��23��
   �޸�����   : ����
*****************************************************************************/
typedef struct
{
    MSG_HEADER_STRU                     MsgHeader;          /* ��Ϣͷ    */     /*_H2ASN_Skip*/

    /* ulChoice �ĺ궨�� */
    VOS_UINT32                          ulChoice;
    union
    {
        RRC_MTA_UTRAN_NMR_STRU          stUtranNMRData;
        RRC_MTA_GSM_NMR_STRU            stGsmNMRData;
    }u;
} RRC_MTA_QRY_NMR_CNF_STRU;

/*****************************************************************************
 �ṹ��    : MTA_WRR_AUTOTEST_PARA_STRU
 �ṹ˵��  : AS�����������

  1.��    ��   : 2012��12��29��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucCmd;                                  /* �Զ����������� */
    VOS_UINT8                           ucParaNum;                              /* ����������� */
    VOS_UINT16                          usReserve;                              /* ���ֽڶ��� */
    VOS_UINT32                          aulPara[MTA_WRR_AUTOTEST_MAX_PARA_NUM]; /* ������� */
}MTA_WRR_AUTOTEST_PARA_STRU;

/*****************************************************************************
 �ṹ��    : MTA_WRR_AUTOTEST_QRY_REQ_STRU
 �ṹ˵��  : AS��������ṹ

  1.��    ��   : 2012��12��29��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgName;                              /*_H2ASN_Skip*/
    MTA_WRR_AUTOTEST_PARA_STRU          stWrrAutotestPara;
}MTA_WRR_AUTOTEST_QRY_REQ_STRU;

/*****************************************************************************
 �ṹ��    : WRR_MTA_AUTOTEST_QRY_RSLT_STRU
 �ṹ˵��  : WAS��������ؽ��

  1.��    ��   : 2012��12��29��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulRsltNum;                              /* ��ѯ������� */
    VOS_UINT32                          aulRslt[WRR_MTA_AUTOTEST_MAX_RSULT_NUM];/* ��ѯ��� */
}WRR_MTA_AUTOTEST_QRY_RSLT_STRU;

/*****************************************************************************
 �ṹ��    : WRR_MTA_AUTOTEST_QRY_CNF_STRU
 �ṹ˵��  : WAS��������ؽ����Ϣ�ṹ

  1.��    ��   : 2012��12��29��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgName;                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulResult;
    WRR_MTA_AUTOTEST_QRY_RSLT_STRU      stWrrAutoTestRslt;
} WRR_MTA_AUTOTEST_QRY_CNF_STRU;

/*****************************************************************************
 �ṹ��    : MTA_WRR_CELLINFO_SET_REQ_STRU
 �ṹ˵��  : MTA����WAS cellinfo��Ϣ�ṹ

  1.��    ��   : 2012��12��29��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgName;                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulSetCellInfo;                          /* 0:��ѯ��С����Ϣ 1:��ѯW������Ϣ */
} MTA_WRR_CELLINFO_QRY_REQ_STRU;

/*****************************************************************************
 �ṹ��    : WRR_MTA_WCDMA_CELLINFO_STRU
 �ṹ˵��  : С����Ϣ�ṹ

  1.��    ��   : 2012��12��29��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_UINT16                      usCellFreq;                                 /* Ƶ�� */
    VOS_UINT16                      usPrimaryScramCode;                         /* ���� */
    VOS_INT16                       sCpichRscp;                                 /* RSCPֵ */
    VOS_INT16                       sCpichEcN0;                                 /* ECN0ֵ */

}WRR_MTA_WCDMA_CELLINFO_STRU;

/*****************************************************************************
 �ṹ��    : WRR_MTA_WCDMA_CELLINFO_RSLT_STRU
 �ṹ˵��  : WRR�ظ�MTAС����Ϣ��ѯ�ṹ

  1.��    ��   : 2012��12��29��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
     VOS_UINT32                     ulCellNum;
     WRR_MTA_WCDMA_CELLINFO_STRU    astWCellInfo[WRR_MTA_MAX_NCELL_NUM];         /*���֧��W 8�������Ĳ�ѯ*/

} WRR_MTA_WCDMA_CELLINFO_RSLT_STRU;
/*****************************************************************************
 �ṹ��    : WRR_MTA_CELLINFO_QRY_CNF_STRU
 �ṹ˵��  : WRR�ظ�MTAС����Ϣ��ѯ�ṹ

  1.��    ��   : 2012��12��29��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    VOS_UINT32                              ulMsgName;                          /*_H2ASN_Skip*/
    VOS_UINT32                              ulResult;
    WRR_MTA_WCDMA_CELLINFO_RSLT_STRU        stWrrCellInfo;
} WRR_MTA_CELLINFO_QRY_CNF_STRU;

/*****************************************************************************
 �ṹ��    : MTA_WRR_MEANRPT_QRY_REQ_STRU
 �ṹ˵��  : MTA�·�MEANRPT QRY��Ϣ�ṹ

  1.��    ��   : 2012��12��29��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgName;
} MTA_WRR_MEANRPT_QRY_REQ_STRU;

/*****************************************************************************
 �ṹ��    : WRR_MTA_MEANRPT_STRU
 �ṹ˵��  : ���������е��¼�+С��

  1.��    ��   : 2012��12��29��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_UINT16                      usEventId;                                  /* ��Ӧ���¼����� */
    VOS_UINT16                      usCellNum;                                  /* һ�β��������ϱ�С������*/
    VOS_UINT16                      ausPrimaryScramCode[WRR_MTA_MEANRPT_MAX_CELL_NUM];/* ���� */
}WRR_MTA_MEANRPT_STRU;

/*****************************************************************************
 �ṹ��    : WRR_MTA_MEANRPT_RSLT_STRU
 �ṹ˵��  : �Զ�������AT^MEANRPT,
             ��ѯ������10�β��������е��¼�+С��

  1.��    ��   : 2012��12��29��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_UINT32                      ulRptNum;                                   /* ����Ĳ���������� */
    WRR_MTA_MEANRPT_STRU            astMeanRptInfo[WRR_MTA_MAX_MEANRPT_NUM];    /* ���������ϱ���С�� */
}WRR_MTA_MEANRPT_RSLT_STRU;


/*****************************************************************************
 �ṹ��    : WRR_MTA_MEANRPT_QRY_CNF_STRU
 �ṹ˵��  : WRR��MTA�ظ�^MEANRPT�������Ϣ�ṹ

  1.��    ��   : 2012��12��29��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgName;                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulResult;
    WRR_MTA_MEANRPT_RSLT_STRU           stMeanRptRslt;
} WRR_MTA_MEANRPT_QRY_CNF_STRU;

/*****************************************************************************
 �ṹ��    : MTA_WRR_FREQLOCK_CTRL_STRU
 �ṹ˵��  : WAS��Ƶ���ƽṹ

  1.��    ��   : 2012��12��29��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucFreqLockEnable;                       /* �Ƿ���Ƶ 0:δ��Ƶ 1:��Ƶ */
    VOS_UINT8                           aucReserved[1];
    VOS_UINT16                          usLockedFreq;                           /* ������Ƶ�� */
} MTA_WRR_FREQLOCK_CTRL_STRU;

/*****************************************************************************
 �ṹ��    : MTA_WRR_FREQLOCK_SET_REQ_STRU
 �ṹ˵��  : MTA�·�MEANRPT QRY��Ϣ�ṹ

  1.��    ��   : 2012��12��29��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgName;                              /*_H2ASN_Skip*/
    MTA_WRR_FREQLOCK_CTRL_STRU          stFrelock;
} MTA_WRR_FREQLOCK_SET_REQ_STRU;

/*****************************************************************************
 �ṹ��    : WRR_MTA_FREQLOCK_SET_CNF_STRU
 �ṹ˵��  : MTA��AT�ص���Ƶ���ý��

  1.��    ��   : 2012��12��29��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgName;                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulResult;
} WRR_MTA_FREQLOCK_SET_CNF_STRU;

/*****************************************************************************
 �ṹ��    : MTA_WRR_FREQLOCK_QRY_REQ_STRU
 �ṹ˵��  : MTA�·�MEANRPT QRY��Ϣ�ṹ

  1.��    ��   : 2012��12��29��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgName;
} MTA_WRR_FREQLOCK_QRY_REQ_STRU;

/*****************************************************************************
 �ṹ��    : WRR_MTA_FREQLOCK_QRY_CNF_STRU
 �ṹ˵��  : MTA��AT�ص���Ƶ���ƽṹ

  1.��    ��   : 2012��12��29��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgName;                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulResult;
    MTA_WRR_FREQLOCK_CTRL_STRU          stFreqLockInfo;
} WRR_MTA_FREQLOCK_QRY_CNF_STRU;

/*****************************************************************************
 �ṹ��    : MTA_WRR_RRC_VERSION_SET_REQ_STRU
 �ṹ˵��  : MTA����WRR version

  1.��    ��   : 2012��12��29��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgName;                              /*_H2ASN_Skip*/
    VOS_UINT8                           ucRRCVersion;                           /* RRC�汾 */
    VOS_UINT8                           aucReserv[3];
} MTA_WRR_RRC_VERSION_SET_REQ_STRU;

/*****************************************************************************
 �ṹ��    : MTA_WRR_RRC_VERSION_QRY_REQ_STRU
 �ṹ˵��  : MTA��ѯWRR version

  1.��    ��   : 2012��12��29��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgName;
} MTA_WRR_RRC_VERSION_QRY_REQ_STRU;

/*****************************************************************************
 �ṹ��    : WRR_MTA_RRC_VERSION_SET_CNF_STRU
 �ṹ˵��  : WRR��MTA�ϱ�version���ý��

  1.��    ��   : 2012��12��29��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgName;                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulResult;
} WRR_MTA_RRC_VERSION_SET_CNF_STRU;

/*****************************************************************************
 �ṹ��    : WRR_MTA_RRC_VERSION_QRY_CNF_STRU
 �ṹ˵��  : WRR��MTA�ϱ�version��ѯ���

  1.��    ��   : 2012��12��29��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgName;                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulResult;
    VOS_UINT8                           ucRrcVersion;                           /* RRC�汾 */
    VOS_UINT8                           aucReserved[3];
} WRR_MTA_RRC_VERSION_QRY_CNF_STRU;

/*****************************************************************************
 �ṹ��    : MTA_WRR_CELLSRH_SET_REQ_STRU
 �ṹ˵��  : MTA�·�WRR cellsrh��������

  1.��    ��   : 2012��12��29��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgName;                              /*_H2ASN_Skip*/
    VOS_UINT8                           ucCellSrh;                              /* ����ģʽ 0:����ģʽ 1:��ʹ��������Ϣģʽ */
    VOS_UINT8                           aucReserve[3];
} MTA_WRR_CELLSRH_SET_REQ_STRU;

/*****************************************************************************
 �ṹ��    : WRR_MTA_CELLSRH_SET_CNF_STRU
 �ṹ˵��  : WRR��MTA�ϱ�cellsrh���ý��

  1.��    ��   : 2012��12��29��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgName;                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulResult;
} WRR_MTA_CELLSRH_SET_CNF_STRU;

/*****************************************************************************
 �ṹ��    : MTA_WRR_CELLSRH_QRY_REQ_STRU
 �ṹ˵��  : MTA�·���ѯWRR cellsrh ��Ϣ

  1.��    ��   : 2012��12��29��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgName;
} MTA_WRR_CELLSRH_QRY_REQ_STRU;

/*****************************************************************************
 �ṹ��    : WRR_MTA_CELLSRH_QRY_CNF_STRU
 �ṹ˵��  : WRR��MTA�ϱ�cellsrh��ѯ���

  1.��    ��   : 2012��12��29��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgName;                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulResult;
    VOS_UINT8                           ucCellSearchType;                       /* ����ģʽ 0:����ģʽ 1:��ʹ��������Ϣģʽ */
    VOS_UINT8                           aucReserve[3];
} WRR_MTA_CELLSRH_QRY_CNF_STRU;


/*****************************************************************************
 �ṹ��    : MTA_GRR_NCELL_MONITOR_SET_REQ_STRU
 �ṹ˵��  : MTA����GRRC����ϵͳС���仯��Ϣ����������Ϣ

  1.��    ��   : 2013��5��31��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgName;                              /*_H2ASN_Skip*/
    VOS_UINT8                           ucSwitch;
    VOS_UINT8                           aucReserved[3];
} MTA_GRR_NCELL_MONITOR_SET_REQ_STRU;

/*****************************************************************************
 �ṹ��    : MTA_GRR_NCELL_MONITOR_QRY_REQ_STRU
 �ṹ˵��  : MTA����GRRC�Ĳ�ѯ��ϵͳС���仯��Ϣ����������Ϣ

  1.��    ��   : 2013��5��31��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgName;
} MTA_GRR_NCELL_MONITOR_QRY_REQ_STRU;

/*****************************************************************************
 �ṹ��    : GRR_MTA_NCELL_MONITOR_SET_CNF_STRU
 �ṹ˵��  : GRRC����MTA����ϵͳС���仯��Ϣ����������Ϣ�Ļظ�

  1.��    ��   : 2013��5��31��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgName;                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulResult;
} GRR_MTA_NCELL_MONITOR_SET_CNF_STRU;

/*****************************************************************************
 �ṹ��    : GRR_MTA_NCELL_MONITOR_QRY_CNF_STRU
 �ṹ˵��  : GRRC����MTA�Ĳ�ѯ��ϵͳС���仯��Ϣ����������Ϣ�Ļظ�

  1.��    ��   : 2013��5��31��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgName;                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulResult;
    VOS_UINT8                           ucSwitch;
    GRR_MTA_NCELL_STATE_ENUM_UINT8      enNcellState;
    VOS_UINT8                           aucReserved[2];
} GRR_MTA_NCELL_MONITOR_QRY_CNF_STRU;

/*****************************************************************************
 �ṹ��    : GRR_MTA_NCELL_MONITOR_IND_STRU
 �ṹ˵��  : GRRC����MTA����ϵͳС���仯��Ϣ�����ϱ�

  1.��    ��   : 2013��5��31��
    �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgName;                              /*_H2ASN_Skip*/
    GRR_MTA_NCELL_STATE_ENUM_UINT8      enNcellState;
    VOS_UINT8                           aucReserved[3];
} GRR_MTA_NCELL_MONITOR_IND_STRU;

/*****************************************************************************
 �ṹ��    : MTA_LRRC_CELLINFO_QRY_REQ_STRU
 �ṹ˵��  : MTA���͸�LRRC��ѯLTE��ǿ��С����Ϣ

  1.��    ��   : 2013��12��09��
    �޸�����   : ����+ECID����
*****************************************************************************/
typedef struct
{
    MSG_HEADER_STRU                     stMsgHeader;                /*_H2ASN_Skip*/
    VOS_UINT8                           aucReserved[4];             /* ����λ */
}MTA_LRRC_CELLINFO_QRY_REQ_STRU;

/*****************************************************************************
 �ṹ��    : LRRC_CELLINFO_STRU
 �ṹ˵��  : LTE��ǿ��С����Ϣ�ṹ

  1.��    ��   : 2013��12��09��
    �޸�����   : ����+ECID����
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          bitOpEarfcn   :1;
    VOS_UINT32                          bitOpRsrp     :1;
    VOS_UINT32                          bitOpRsrq     :1;
    VOS_UINT32                          bitOpTa       :1;
    VOS_UINT32                          bitSpare      :28;
    VOS_UINT32                          ulPhyCellCode;
    VOS_UINT32                          ulEarfcn;
    VOS_UINT32                          ulRsrp;
    VOS_UINT32                          ulRsrq;
    VOS_UINT32                          ulTa;                                   /* ʱ����ǰ����ȡֵ��Χ0~20512 */
}LRRC_CELLINFO_STRU;

/*****************************************************************************
 �ṹ��    : LRRC_CELLINFO_RSLT_STRU
 �ṹ˵��  : LTE��ǿ��С����Ϣ����ṹ

  1.��    ��   : 2013��12��09��
    �޸�����   : ����+ECID����
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulTac;
    VOS_UINT32                          ulCellNum;
    LRRC_CELLINFO_STRU                  astCellInfo[LRRC_MAX_NCELL_NUM];
}LRRC_CELLINFO_RSLT_STRU;

/*****************************************************************************
 �ṹ��    : LRRC_MTA_CELLINFO_QRY_CNF_STRU
 �ṹ˵��  : LRRC�ظ�MTAģ����ǿ��С����Ϣ��ѯ����ṹ

  1.��    ��   : 2013��12��09��
    �޸�����   : ����+ECID����
*****************************************************************************/
typedef struct
{
    MSG_HEADER_STRU                     stMsgHeader;                /*_H2ASN_Skip*/
    MTA_RRC_RESULT_ENUM_UINT32          enResult;
    LRRC_CELLINFO_RSLT_STRU             stCellInfoRslt;
}LRRC_MTA_CELLINFO_QRY_CNF_STRU;

/*****************************************************************************
  8 UNION����
*****************************************************************************/

/*****************************************************************************
  9 OTHERS����
*****************************************************************************/

/*****************************************************************************
  H2ASN������Ϣ�ṹ����
*****************************************************************************/
typedef struct
{
    MTA_RRC_MSG_TYPE_ENUM_UINT32        enMsgID;                                /*_H2ASN_MsgChoice_Export MTA_RRC_MSG_TYPE_ENUM_UINT32*/
    VOS_UINT8                           aucMsgBlock[4];
    /***************************************************************************
        _H2ASN_MsgChoice_When_Comment          MTA_RRC_MSG_TYPE_ENUM_UINT32
    ****************************************************************************/
}MTA_RRC_MSG_DATA;
/*_H2ASN_Length UINT32*/

typedef struct
{
    VOS_MSG_HEADER
    MTA_RRC_MSG_DATA                    stMsgData;
}MtaRrcInterface_MSG;

/*****************************************************************************
  10 ��������
*****************************************************************************/


#if ((VOS_OS_VER == VOS_WIN32) || (VOS_OS_VER == VOS_NUCLEUS))
#pragma pack()
#else
#pragma pack(0)
#endif




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of MtaRrcInterface.h */


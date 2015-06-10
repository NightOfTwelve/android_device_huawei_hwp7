/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : MtcRrcInterface.h
  �� �� ��   : ����
  ��������   : 2013��07��30��
  ����޸�   :
  ��������   : MTC��RRC֮��Ľӿ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��07��30��
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __MTCRRCINTERFACE_H__
#define __MTCRRCINTERFACE_H__

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "PsTypeDef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(4)


/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define  MTC_RRC_TDS_ARFCN_MAX_NUM                          (9)                 /* TDSƵ���б�������������GAS���������ṹ��MTC�ϱ�Ƶ����Ϣ */
#define  MTC_RRC_LTE_ARFCN_MAX_NUM                          (8)                 /* LTEƵ���б�������������GAS���������ṹ��MTC�ϱ�Ƶ����Ϣ */
#define  MTC_RRC_GSM_MAX_USING_ARFCN_NUM                    (7)                 /* GSMƵ���б������� */
#define  MTC_RRC_GSM_MA_ARFCN_MAX_NUM                       (64)                /* GSM��ƵƵ���б������� */
#define  MTC_RRC_WCDMA_MAX_USING_ARFCN_NUM                  (4)                 /* WCDMAƵ���б������� */

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
/*****************************************************************************
ö����    : MTC_RRC_MSG_ID_ENUM
�ṹ˵��  : MTC��RRC��֮�����Ϣ

  1.��    ��   : 2013��07��30��
    �޸�����  : ����MTC��RRC֮�����Ϣ
*****************************************************************************/
enum MTC_RRC_MSG_ID_ENUM
{
    /* ��Ϣ���� */                             /* ��ϢID */                     /* ��ע */
    /* MTC --> RRC */
    ID_MTC_RRC_INTRUSION_ACTION_SET_REQ     = 0x0001,                           /* _H2ASN_MsgChoice MTC_RRC_INTRUSION_ACTION_SET_REQ_STRU */
    ID_MTC_RRC_INTRUSION_BAND_SET_REQ       = 0x0003,                           /* _H2ASN_MsgChoice MTC_RRC_INTRUSION_BAND_SET_REQ_STRU */
    ID_MTC_RRC_NOTCH_CHANNEL_IND            = 0x0005,                           /* _H2ASN_MsgChoice MTC_RRC_NOTCH_CHANNEL_IND_STRU */
    ID_MTC_RRC_TDS_LTE_RF_CONTROL_IND       = 0x0009,                           /* _H2ASN_MsgChoice MTC_RRC_TDS_LTE_RF_CONTROL_IND_STRU */

    /* RRC --> MTC */
    ID_RRC_MTC_INTRUSION_ACTION_SET_CNF     = 0x0002,                           /* _H2ASN_MsgChoice RRC_MTC_RESULT_CNF_STRU */
    ID_RRC_MTC_INTRUSION_BAND_SET_CNF       = 0x0004,                           /* _H2ASN_MsgChoice RRC_MTC_RESULT_CNF_STRU */
    ID_RRC_MTC_INTRUSION_BAND_INFO_IND      = 0x0006,                           /* _H2ASN_MsgChoice RRC_MTC_INTRUSION_BAND_INFO_IND_STRU */
    ID_RRC_MTC_AREA_LOST_IND                = 0x0008,                           /* _H2ASN_MsgChoice RRC_MTC_AREA_LOST_IND_STRU */
    ID_RRC_MTC_NCELL_INFO_IND               = 0x000A,                           /* _H2ASN_MsgChoice RRC_MTC_NCELL_INFO_IND_STRU */
    ID_RRC_MTC_AREA_AVALIABLE_IND           = 0x000C,                           /* _H2ASN_MsgChoice RRC_MTC_AREA_AVALIABLE_IND_STRU */

    ID_RRC_MTC_USING_FREQ_IND               = 0x000E,                           /* _H2ASN_MsgChoice RRC_MTC_USING_FREQ_IND_STRU */

    ID_MTC_RRC_MSG_ID_ENUM_BUTT
};
typedef VOS_UINT32  MTC_RRC_MSG_ID_ENUM_UINT32;

/*****************************************************************************
 ö����    : MTC_RRC_RESULT_ENUM
 ö��˵��  : MTC��RRC��֮��Ľ����

  1.��    ��   : 2013��08��12��
    �޸�����   : V9R1 ���ſ�����Ŀ
*****************************************************************************/
enum MTC_RRC_RESULT_ENUM
{
    MTC_RRC_RESULT_NO_ERROR             = 0x0000,                               /* ��Ϣ�������� */
    MTC_RRC_RESULT_ERROR,

    MTC_RRC_RESULT_BUTT
};
typedef VOS_UINT32  MTC_RRC_RESULT_ENUM_UINT32;

/*****************************************************************************
 ö����     :RRC_MTC_GSM_BANDINDICATOR_ENUM
 Э����  :
 ASN.1����   :
 ö��˵�� : 2GС��Ƶ��ָʾ
*****************************************************************************/
enum RRC_MTC_GSM_BANDINDICATOR_ENUM
{
    RRC_MTC_GSM_BANDINDICATOR_DCS1800                          = 0,
    RRC_MTC_GSM_BANDINDICATOR_PCS1900                             ,

    RRC_MTC_GSM_BANDINDICATOR_BUTT
};
typedef VOS_UINT16    RRC_MTC_GSM_BANDINDICATOR_ENUM_UINT16;

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
 �ṹ��    : RRC_MTC_MS_BAND_INFO_STRU
 �ṹ˵��  : ͨ��Bitλ��¼Ƶ����Ϣ
             aulBandInfo[0]�����32, aulBandInfo[1]�����32��Band,
             Band��Bitλ�ɵ͵���һһ��Ӧ, ��: aulBandInfo[0]��Bit0����Band1,
             aulBandInfo[0]��Bit2����Band3, aulBandInfo[0]��Bit31����Band32
             aulBandInfo[1]��Bit0����Band33, aulBandInfo[1]��Bit6����Band39

 1.��    ��   : 2013��07��30��
   �޸�����   : ������
*******************************************************************************/
typedef struct
{
    VOS_UINT32                          aulBandInfo[2];                         /* Ƶ����Ϣ��ÿ��Bit��ʾһ��Band */
}RRC_MTC_MS_BAND_INFO_STRU;

/*******************************************************************************
 �ṹ��    : MTC_RRC_INTRUSION_ACTION_SET_REQ_STRU
 �ṹ˵��  : ID_MTC_RRC_INTRUSION_ACTION_SET_REQ����Ϣ�ṹ

 1.��    ��   : 2013��07��30��
   �޸�����   : ������
*******************************************************************************/
typedef struct
{
    MSG_HEADER_STRU                     stMsgHeader;                            /*_H2ASN_Skip*/
    PS_BOOL_ENUM_UINT8                  enActionFlag;                           /* �򿪻��߹رո��Ź��� */
    VOS_UINT8                           aucRsv[3];
}MTC_RRC_INTRUSION_ACTION_SET_REQ_STRU;

/*******************************************************************************
 �ṹ��    : MTC_RRC_INTRUSION_BAND_SET_REQ_STRU
 �ṹ˵��  : ID_MTC_RRC_INTRUSION_BAND_SET_REQ����Ϣ�ṹ

 1.��    ��   : 2013��07��30��
   �޸�����   : ������
*******************************************************************************/
typedef struct
{
    MSG_HEADER_STRU                     stMsgHeader;                            /*_H2ASN_Skip*/
    RRC_MTC_MS_BAND_INFO_STRU           stForbiddenBandInfo;                    /* ��Ҫ��ֹ��Ƶ����Ϣ, Band��Ӧ��BitλΪ0������ֹ, 1�����ֹ */
}MTC_RRC_INTRUSION_BAND_SET_REQ_STRU;

/*******************************************************************************
 �ṹ��    : RRC_MTC_RESULT_CNF_STRU
 �ṹ˵��  : RRC�ظ�MTCģ������������Ϣ�ṹ

 1.��    ��   : 2013��08��12��
   �޸�����   : V9R1 ���ſ�����Ŀ
*******************************************************************************/
typedef struct
{
    MSG_HEADER_STRU                     stMsgHeader;                            /*_H2ASN_Skip*/
    MTC_RRC_RESULT_ENUM_UINT32          enResult;
}RRC_MTC_RESULT_CNF_STRU;

/*******************************************************************************
 �ṹ��    : RRC_MTC_INTRUSION_BAND_INFO_IND_STRU
 �ṹ˵��  : ID_RRC_MTC_INTRUSION_BAND_INFO_IND����Ϣ�ṹ

 1.��    ��   : 2013��07��30��
   �޸�����   : ������

  2.��    ��   : 2014��3��24��
    �޸�����   : B39&B32 Intrusion
*******************************************************************************/
typedef struct
{
    MSG_HEADER_STRU                     stMsgHeader;                            /*_H2ASN_Skip*/
    RRC_MTC_MS_BAND_INFO_STRU           stCurBandInfo;                          /* ��ǰ��⵽��Ƶ����Ϣ(���˵���֧�ֵ�Ƶ��), Band��Ӧ��BitλΪ0��������, 1������� */
    RRC_MTC_MS_BAND_INFO_STRU           stSerCellBandInfo;                      /* ��ǰפ������Ƶ����Ϣ(���˵���֧�ֵ�Ƶ��), Band��Ӧ��BitλΪ0��������, 1������� */
}RRC_MTC_INTRUSION_BAND_INFO_IND_STRU;

/*******************************************************************************
 �ṹ��    : RRC_MTC_AREA_LOST_IND_STRU
 �ṹ˵��  : ID_RRC_MTC_AREA_LOST_IND����Ϣ�ṹ

 1.��    ��   : 2013��10��18��
   �޸�����   : ������
*******************************************************************************/
typedef struct
{
    MSG_HEADER_STRU                     stMsgHeader;                            /*_H2ASN_Skip*/
    PS_BOOL_ENUM_UINT8                  enIdleStateFlag;                        /* PS_TURE ��ʾIdle̬����*/
    VOS_UINT8                           aucRsv[3];                              /*����λ*/
}RRC_MTC_AREA_LOST_IND_STRU;

/*******************************************************************************
 �ṹ��    : MTC_RRC_NOTCH_CHANNEL_IND_STRU
 �ṹ˵��  : ID_MTC_RRC_NOTCH_CHANNEL_IND����Ϣ�ṹ

 1.��    ��   : 2013��12��18��
   �޸�����   : ������
*******************************************************************************/
typedef struct
{
    MSG_HEADER_STRU                     stMsgHeader;                            /*_H2ASN_Skip*/
    PS_BOOL_ENUM_UINT8                  enStartFlag;                            /* PS_TURE:����NOTCHͨ����PS_FALSE:������NOTCHͨ�� */
    VOS_UINT8                           aucRsv[3];                              /*����λ*/
}MTC_RRC_NOTCH_CHANNEL_IND_STRU;

/*******************************************************************************
 �ṹ��    : RRC_MTC_TDD_NCELL_INFO_STRU
 �ṹ˵��  : GSM�µ�TDSƵ���б�

 1.��    ��   : 2013��11��26��
   �޸�����   : �����ɽṹ

*******************************************************************************/
typedef struct
{
    VOS_UINT8                           ucTdsArfcnNum;                              /* TDSƵ�������0��ʾTDSƵ�㲻���� */
    VOS_UINT8                           aucRsv[1];                                  /* ����λ*/
    VOS_UINT16                          ausTdsArfcnList[MTC_RRC_TDS_ARFCN_MAX_NUM]; /* Ƶ���б� */
}RRC_MTC_TDS_NCELL_INFO_STRU;

/*******************************************************************************
 �ṹ��    : RRC_MTC_LTE_NCELL_INFO_STRU
 �ṹ˵��  : GSM�µ�LTEƵ���б�

 1.��    ��   : 2013��11��26��
   �޸�����   : �����ɽṹ

*******************************************************************************/
typedef struct
{
    VOS_UINT8                           ucLteArfcnNum;                              /* LTEƵ�������0��ʾLTEƵ�㲻���� */
    VOS_UINT8                           aucRsv[3];                                  /* ����λ*/
    VOS_UINT16                          ausLteArfcnList[MTC_RRC_LTE_ARFCN_MAX_NUM]; /* Ƶ���б� */
}RRC_MTC_LTE_NCELL_INFO_STRU;

/*******************************************************************************
 �ṹ��    : RRC_MTC_TDS_LTE_NCELL_INFO_IND_STRU
 �ṹ˵��  : ID_RRC_MTC_TDS_LTE_NCELL_INFO_IND����Ϣ��Ӧ�Ľṹ��
             ����GSM�µ�TDS/LTE�����ṹ�е�TDS/LTE��Ƶ���б�

 1.��    ��   : 2013��11��26��
   �޸�����   : �����ɽṹ

*******************************************************************************/
typedef struct
{
    MSG_HEADER_STRU                     stMsgHeader;                      /*_H2ASN_Skip*/

    RRC_MTC_TDS_NCELL_INFO_STRU         stTdsNCellInfo;                   /* TDSƵ���б���Ϣ */

    RRC_MTC_LTE_NCELL_INFO_STRU         stLteNCellInfo;                   /* LTEƵ���б���Ϣ */
}RRC_MTC_NCELL_INFO_IND_STRU;

/*******************************************************************************
 �ṹ��    : RRC_MTC_AREA_AVALIABLE_IND_STRU
 �ṹ˵��  : ID_RRC_MTC_AREA_AVALIABLE_IND��Ϣ��Ӧ�Ľṹ�������֪ͨMTC����ָ�

 1.��    ��   : 2014��01��17��
   �޸�����   : V9R1C53 C+L ������ѡ��Ŀ
*******************************************************************************/
typedef struct
{
    MSG_HEADER_STRU                     stMsgHeader;                      /*_H2ASN_Skip*/
}RRC_MTC_AREA_AVALIABLE_IND_STRU;

/*******************************************************************************
 �ṹ��    : MTC_RRC_TDS_LTE_RF_CONTROL_IND_STRU
 �ṹ˵��  : ID_MTC_RRC_TDS_LTE_RF_CONTROL_IND����Ϣ�ṹ

 1.��    ��   : 2014��03��24��
   �޸�����   : ������
*******************************************************************************/
typedef struct
{
    MSG_HEADER_STRU                     stMsgHeader;                            /*_H2ASN_Skip*/
    PS_BOOL_ENUM_UINT8                  enUplinkCloseFlag;                      /* PS_TRUE:�ر����з��䣬PS_FALSE:���ر����з��� */
    VOS_UINT8                           aucRsv[3];                              /* ����λ */
}MTC_RRC_TDS_LTE_RF_CONTROL_IND_STRU;

/*******************************************************************************
 �ṹ��    : RRC_MTC_GSM_FREQ_INFO_STRU
 �ṹ˵��  : GSMƵ����Ϣ

 1.��    ��   : 2014��2��21��
   �޸�����   : �����ɽṹ

*******************************************************************************/
typedef struct
{
    VOS_UINT16                              usGsmArfcn;     /* ����Ƶ��� */
    RRC_MTC_GSM_BANDINDICATOR_ENUM_UINT16   enBandInd;      /* Bandָʾ, ָʾ��ǰƵ���� 1800 ϵͳ �� 1900 ϵͳ */
}RRC_MTC_GSM_FREQ_INFO_STRU;

/*******************************************************************************
 �ṹ��    : RRC_MTC_GSM_FREQ_INFO_STRU
 �ṹ˵��  : GSMƵ���б�

 1.��    ��   : 2014��2��21��
   �޸�����   : �����ɽṹ

*******************************************************************************/
typedef struct
{
    VOS_UINT8                           ucGsmArfcnNum;                                      /* GSMƵ�������0��ʾGSMƵ�㲻���� */
    VOS_UINT8                           ucGsmMaArfcnNum;                                    /* ҵ��Ƶ�����, �統ǰ����G��ģ,��Ϊ0 */
    VOS_UINT8                           aucRsv[2];
    RRC_MTC_GSM_FREQ_INFO_STRU          astGsmArfcnList[MTC_RRC_GSM_MAX_USING_ARFCN_NUM];   /* ����С��������Ƶ���б�, ����С��Ƶ���ڵ�һλ */
    RRC_MTC_GSM_FREQ_INFO_STRU          astGsmMaArfcnList[MTC_RRC_GSM_MA_ARFCN_MAX_NUM];    /* ҵ��Ƶ����Ϣ������Ϣֻ��G��ģ����Ч */
}RRC_MTC_GSM_FREQ_INFO_LIST_STRU;

/*******************************************************************************
 �ṹ��    : RRC_MTC_WCDMA_FREQ_INFO_STRU
 �ṹ˵��  : WCDMAƵ���б�

 1.��    ��   : 2014��2��21��
   �޸�����   : �����ɽṹ

*******************************************************************************/
typedef struct
{
    VOS_UINT8                           ucWcdmaArfcnNum;                                        /* WƵ�������0��ʾWCDMAƵ�㲻���� */
    VOS_UINT8                           aucRsv[3];                                              /* ����λ*/
    VOS_UINT16                          ausWcdmaArfcnList[MTC_RRC_WCDMA_MAX_USING_ARFCN_NUM];   /* Ƶ���б� */
}RRC_MTC_WCDMA_FREQ_INFO_LIST_STRU;

/*******************************************************************************
 �ṹ��    : RRC_MTC_USING_FREQ_IND_STRU
 �ṹ˵��  : ID_RRC_MTC_USING_FREQ_IND����Ϣ��Ӧ�Ľṹ��
             ����GUTL��ģ��פ���Լ����ڲ����ı�ϵͳ�Լ���ϵͳƵ����Ϣ������С������Ƶ����ڵ�һ��λ�ã�
             ����Ƶ�㰴����������

 1.��    ��   : 2014��2��21��
   �޸�����   : �����ɽṹ

*******************************************************************************/
typedef struct
{
    MSG_HEADER_STRU                     stMsgHeader;                       /*_H2ASN_Skip*/

    RRC_MTC_GSM_FREQ_INFO_LIST_STRU     stGsmFreqInfo;                     /* GSMƵ���б���Ϣ */

    RRC_MTC_WCDMA_FREQ_INFO_LIST_STRU   stWcdmaFreqInfo;                   /* WCDMAƵ���б���Ϣ */

    RRC_MTC_TDS_NCELL_INFO_STRU         stTdsFreqInfo;                     /* TDSƵ���б���Ϣ */

    RRC_MTC_LTE_NCELL_INFO_STRU         stLteFreqInfo;                     /* LTEƵ���б���Ϣ */

    PS_BOOL_ENUM_UINT8                  enCsExistFlag;                     /* PS_FALSE ��ʾ������CS���� */
    PS_BOOL_ENUM_UINT8                  enPsExistFlag;                     /* PS_FALSE ��ʾ������PS���� */
    VOS_UINT8                           aucRsv[2];
}RRC_MTC_USING_FREQ_IND_STRU;



/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/

/*****************************************************************************
  10 ��������
*****************************************************************************/


/*****************************************************************************
  H2ASN������Ϣ�ṹ����
*****************************************************************************/
typedef struct
{
    MTC_RRC_MSG_ID_ENUM_UINT32          enMsgId;                                /*_H2ASN_MsgChoice_Export MTC_RRC_MSG_ID_ENUM_UINT32*/
    VOS_UINT8                           aucMsgBlock[4];
    /***************************************************************************
        _H2ASN_MsgChoice_When_Comment          MTC_RRC_MSG_ID_ENUM_UINT32
    ****************************************************************************/
}MTC_RRC_MSG_DATA;
/*_H2ASN_Length UINT32*/

typedef struct
{
    VOS_MSG_HEADER
    MTC_RRC_MSG_DATA                    stMsgData;
}MtcRrcInterface_MSG;

#if (VOS_OS_VER == VOS_WIN32)
#pragma pack()
#else
#pragma pack(0)
#endif


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif


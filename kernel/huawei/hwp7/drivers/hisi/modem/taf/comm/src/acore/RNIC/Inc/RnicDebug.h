/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : RnicDebug.h
  �� �� ��   : ����
  ��������   : 2012��1��20��
  ����޸�   :
  ��������   : RnicDebug.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��1��20��
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __RNICDEBUG_H__
#define __RNICDEBUG_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "product_config.h"
#if (FEATURE_ON == FEATURE_LTE)
#include "msp_diag.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  2 �궨��
*****************************************************************************/

#define RNIC_LATENCY_LOG(ModuleId, String)  \
            RNIC_LATENCY_OM_LOG(__FILE__, __LINE__, ModuleId, LOG_LEVEL_ERROR, String)

/* ����ͳ����Ϣ */
#define  RNIC_DBG_RECV_UL_IPV4_PKT_NUM(n, index)       (g_astRnicStats[index].ulUlRecvIpv4PktNum += (n))
#define  RNIC_DBG_RECV_UL_IPV6_PKT_NUM(n, index)       (g_astRnicStats[index].ulUlRecvIpv6PktNum += (n))
#define  RNIC_DBG_RECV_UL_IPV4V6_PKT_NUM(n, index)     (g_astRnicStats[index].ulUlRecvIpv4v6PktNum += (n))
#define  RNIC_DBG_RECV_UL_UNDIAL_PKT_NUM(n, index)     (g_astRnicStats[index].ulUlRecvUndiaPktNum += (n))
#define  RNIC_DBG_SEND_UL_PKT_NUM(n, index)            (g_astRnicStats[index].ulUlSendPktNum += (n))
#define  RNIC_DBG_SEND_UL_PKT_FAIL_NUM(n, index)       (g_astRnicStats[index].ulUlSendPktFailNum += (n))
#define  RNIC_DBG_UL_RECV_IPV4_BROADCAST_NUM(n, index) (g_astRnicStats[index].ulUlIpv4BrdcstPktNum += (n))
#define  RNIC_DBG_UL_CHANGE_IMMZC_FAIL_NUM(n, index)   (g_astRnicStats[index].ulUlImmzcFailPktNum += (n))
#define  RNIC_DBG_NETCAED_UL_DISCARD_NUM(n, index)     (g_astRnicStats[index].ulUlNetCardDiscardNum += (n))
#define  RNIC_DBG_FLOW_CTRL_UL_DISCARD_NUM(n, index)   (g_astRnicStats[index].ulUlFlowCtrlDiscardNum += (n))
#define  RNIC_DBG_RECV_UL_ERR_PKT_NUM(n, index)        (g_astRnicStats[index].ulUlRecvErrPktNum += (n))
#define  RNIC_DBG_RAB_ID_ERR_NUM(n, index)             (g_astRnicStats[index].ulUlRabIdErr += (n))
#define  RNIC_DBG_SEND_APP_DIALUP_SUCC_NUM(n, index)   (g_astRnicStats[index].ulUlSendAppDialUpSucc += (n))
#define  RNIC_DBG_SEND_APP_DIALUP_FAIL_NUM(n, index)   (g_astRnicStats[index].ulUlSendAppDialUpFail += (n))
#define  RNIC_DBG_SEND_APP_DIALDOWN_SUCC_NUM(n, index) (g_astRnicStats[index].ulUlSendAppDialDownSucc += (n))
#define  RNIC_DBG_SEND_APP_DIALDOWN_FAIL_NUM(n, index) (g_astRnicStats[index].ulUlSendAppDialDownFail += (n))
#define  RNIC_DBG_NET_ID_UL_DISCARD_NUM(n, index)      (g_astRnicStats[index].ulUlNetIdDiscardNum += (n))
#define  RNIC_DBG_MODEM_ID_UL_DISCARD_NUM(n, index)    (g_astRnicStats[index].ulUlModemIdDiscardNum += (n))

/* ����ͳ����Ϣ */
#define  RNIC_DBG_RECV_DL_IPV4_PKT_NUM(n, index)       (g_astRnicStats[index].ulDlRecvIpv4PktNum += (n))
#define  RNIC_DBG_RECV_DL_IPV6_PKT_NUM(n, index)       (g_astRnicStats[index].ulDlRecvIpv6PktNum += (n))
#define  RNIC_DBG_SEND_DL_PKT_NUM(n, index)            (g_astRnicStats[index].ulDlSendPktNum += (n))
#define  RNIC_DBG_SEND_DL_PKT_FAIL_NUM(n, index)       (g_astRnicStats[index].ulDlSendPktFailNum += (n))
#define  RNIC_DBG_RECV_DL_BIG_PKT_NUM(n, index)        (g_astRnicStats[index].ulDlRecvBigPktNum += (n))
#define  RNIC_DBG_DISCARD_DL_PKT_NUM(n, index)         (g_astRnicStats[index].ulDlDiscardPktNum += (n))
#define  RNIC_DBG_ADD_DL_MACHEAD_FAIL_NUM(n, index)    (g_astRnicStats[index].ulDlAddMacHdFailNum += (n))
#define  RNIC_DBG_NETCAED_DL_DISCARD_NUM(n, index)     (g_astRnicStats[index].ulDlNetCardDiscardNum += (n))
#define  RNIC_DBG_RECV_DL_ERR_PKT_NUM(n, index)        (g_astRnicStats[index].ulDlRecvErrPktNum += (n))
#define  RNIC_DBG_NETID_DL_DISCARD_NUM(n, index)       (g_astRnicStats[index].ulDlNetIdDiscardNum += (n))

#define RNIC_DBG_SET_SEM_INIT_FLAG(flag)               (g_astRnicStats[0].ulSemInitFlg = (flag))
#define RNIC_DBG_SAVE_BINARY_SEM_ID(sem_id)            (g_astRnicStats[0].ulBinarySemId = (sem_id))
#define RNIC_DBG_CREATE_BINARY_SEM_FAIL_NUM(n)         (g_astRnicStats[0].ulCreateBinarySemFailNum += (n))
#define RNIC_DBG_LOCK_BINARY_SEM_FAIL_NUM(n)           (g_astRnicStats[0].ulLockBinarySemFailNum += (n))
#define RNIC_DBG_SAVE_LAST_BIN_SEM_ERR_RSLT(rslt)      (g_astRnicStats[0].ulLastBinarySemErrRslt = (rslt))
#define RNIC_DBG_SAVE_CCPU_RESET_SUCCESS_NUM(n)        (g_astRnicStats[0].ulResetSucessNum += (n))

/*****************************************************************************
  3 ö�ٶ���
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
/*****************************************************************************
 �ṹ��    : RNIC_STATS_INFO_STRU
 �ṹ˵��  : RNICͳ����
  1.��    ��   : 2012��1��20��
    �޸�����   : �����ļ�
*****************************************************************************/
typedef struct
{
    /* ����ͳ����Ϣ */
    VOS_UINT32              ulUlRecvIpv4PktNum;                                 /* RNIC�յ�����IPV4���ݵĸ��� */
    VOS_UINT32              ulUlRecvIpv6PktNum;                                 /* RNIC�յ�����IPV6���ݵĸ��� */
    VOS_UINT32              ulUlRecvIpv4v6PktNum;                               /* IPV4V6���ͼ���ʱRNIC�յ��������ݵĸ���(IPV4��IPV6) */
    VOS_UINT32              ulUlRecvUndiaPktNum;                                /* RNICδ����ʱ�յ��������ݵĸ��� */
    VOS_UINT32              ulUlSendPktNum;                                     /* RNIC���з��͸�ADS���ݵĸ��� */
    VOS_UINT32              ulUlSendPktFailNum;                                 /* RNIC��ADS������������ʧ�ܵĸ��� */
    VOS_UINT32              ulUlIpv4BrdcstPktNum;                               /* RNICδ���Ϻ�ǰ�����յ�IPV4�㲥���ĸ��� */
    VOS_UINT32              ulUlImmzcFailPktNum;                                /* RNIC����ת��ΪIMM_ZCʧ�ܵĸ��� */
    VOS_UINT32              ulUlNetCardDiscardNum;                              /* RNIC����˽�����ݴ��󶪵��������ݰ��ĸ��� */
    VOS_UINT32              ulUlFlowCtrlDiscardNum;                             /* RNIC�������ض����������ݰ��ĸ��� */
    VOS_UINT32              ulUlRecvErrPktNum;                                  /* RNIC�յ��������ݰ��ĸ���(��ipv4��ipv6��) */
    VOS_UINT32              ulUlSendAppDialUpSucc;                              /* RNIC�ɹ��ϱ�APP���貦�� */
    VOS_UINT32              ulUlSendAppDialUpFail;                              /* RNIC�ϱ�APP���貦��ʧ�� */
    VOS_UINT32              ulUlSendAppDialDownSucc;                            /* RNIC�ɹ��ϱ�APP�Ͽ����� */
    VOS_UINT32              ulUlSendAppDialDownFail;                            /* RNIC�ϱ�APP�Ͽ�����ʧ�� */
    VOS_UINT32              ulUlRabIdErr;                                       /* Rab id���� */

    VOS_UINT32              ulUlNetIdDiscardNum;                                /* RNIC����ID���󶪵��������ݰ��ĸ��� */
    VOS_UINT32              ulUlModemIdDiscardNum;                              /* RNIC Modem ID���󶪵��������ݰ��ĸ��� */

    /* ����ͳ����Ϣ */
    VOS_UINT32              ulDlRecvIpv4PktNum;                                 /* RNIC�յ�����IPV4���ݵĸ��� */
    VOS_UINT32              ulDlRecvIpv6PktNum;                                 /* RNIC�յ�����IPV6���ݵĸ��� */
    VOS_UINT32              ulDlSendPktNum;                                     /* RNIC�����������ݵĸ��� */
    VOS_UINT32              ulDlSendPktFailNum;                                 /* RNIC������������ʧ�ܵĸ��� */
    VOS_UINT32              ulDlRecvBigPktNum;                                  /* RNIC�յ��������ݰ�����MTU�ĸ��� */
    VOS_UINT32              ulDlDiscardPktNum;                                  /* RNIC����δ����������ݸ��� */
    VOS_UINT32              ulDlAddMacHdFailNum;                                /* RNIC���м�MACͷʧ�ܵĸ��� */
    VOS_UINT32              ulDlNetCardDiscardNum;                              /* RNIC����˽�����ݴ��󶪵��������ݰ��ĸ��� */
    VOS_UINT32              ulDlRecvErrPktNum;                                  /* RNIC�յ��������ݰ��ĸ���(��ipv4��ipv6��) */

    VOS_UINT32              ulDlNetIdDiscardNum;                                /* RNIC����ID���󶪵��������ݰ��ĸ��� */

    /* ��λ�ź�����Ϣ */
    VOS_UINT32              ulSemInitFlg;                                       /* ��ʼ����ʶ, VOS_TRUE: �ɹ�; VOS_FALSE: ʧ�� */
    VOS_UINT32              ulBinarySemId;                                      /* �������ź���ID */
    VOS_UINT32              ulCreateBinarySemFailNum;                           /* �����������ź���ʧ�ܴ��� */
    VOS_UINT32              ulLockBinarySemFailNum;                             /* ���������ź���ʧ�ܴ��� */
    VOS_UINT32              ulLastBinarySemErrRslt;                             /* ���һ�����������ź���ʧ�ܽ�� */
    VOS_UINT32              ulResetSucessNum;                                   /* C�˸�λ�ɹ��Ĵ��� */
}RNIC_STATS_INFO_STRU;


/*****************************************************************************
  8 ȫ�ֱ�������
*****************************************************************************/
extern RNIC_STATS_INFO_STRU                    g_astRnicStats[];
extern VOS_UINT32                              g_ulRnicPrintUlDataFlg;
/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/

extern VOS_VOID RNIC_LATENCY_OM_LOG( const VOS_CHAR  *pcFileName,  VOS_UINT32  ulLineNum,
                                       VOS_UINT32      ulModuleId,   VOS_UINT32 ulLevel,
                                       const VOS_CHAR  *pcString );

extern VOS_VOID RNIC_MNTN_TraceRcvUlData(VOS_VOID);
extern VOS_VOID RNIC_MNTN_TraceRcvDlData(VOS_VOID);
extern VOS_VOID RNIC_MNTN_TraceSndDlData(VOS_VOID);

extern VOS_VOID RNIC_ShowResetStats(VOS_VOID);
extern VOS_VOID RNIC_Help(VOS_VOID);

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

#endif /* end of AdsTest.h */


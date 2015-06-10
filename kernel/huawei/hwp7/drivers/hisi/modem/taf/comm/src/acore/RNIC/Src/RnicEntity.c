/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : RnicEntity.c
  �� �� ��   : ����
  ��������   : 2011��12��06��
  ����޸�   :
  ��������   : RNICʵ�����
  �����б�   :

  �޸���ʷ   :
  1.��    ��   : 2011��12��06��
    �޸�����   : �����ļ�

******************************************************************************/
#ifdef  __cplusplus
  #if  __cplusplus
  extern "C"{
  #endif
#endif

/******************************************************************************
   1 ͷ�ļ�����
******************************************************************************/
#include "v_typdef.h"
#include "ImmInterface.h"
#include "RnicProcMsg.h"
#include "RnicLog.h"
#include "RnicEntity.h"
#include "RnicDebug.h"
#include "RnicCtx.h"

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/

#define THIS_FILE_ID PS_FILE_ID_RNIC_ENTITY_C

/******************************************************************************
   2 �ⲿ������������
******************************************************************************/

/******************************************************************************
   3 ˽�ж���
******************************************************************************/

/******************************************************************************
   4 ȫ�ֱ�������
*****************************************************************************/

/* �������Ĳ������о�̬ӳ�� */
#if (VOS_OS_VER == VOS_WIN32)
static struct net_device_ops rnic_ops;
#else
static const struct net_device_ops rnic_ops = {
       .ndo_stop                = RNIC_StopNetCard,
       .ndo_open                = RNIC_OpenNetCard,
       .ndo_start_xmit          = RNIC_StartXmit,
       .ndo_set_mac_address     = RNIC_SetMacAddress,
       .ndo_change_mtu          = RNIC_ChangeMtu,
       .ndo_tx_timeout          = RNIC_ProcTxTimeout,
       .ndo_do_ioctl            = RNIC_Ioctrl,
       .ndo_get_stats           = RNIC_GetNetCardStats,
};
#endif

#if (FEATURE_ON == FEATURE_MULTI_MODEM)
const RNIC_NETCARD_ELEMENT_TAB_STRU           g_astRnicManageTbl[RNIC_NET_ID_MAX_NUM] = {
    { RNIC_RM_NET_ID_0,                      MODEM_ID_0,                           "rmnet0",
      {{0x58, 0x02, 0x03, 0x04, 0x05, 0x06}, {0x00, 0x11, 0x09, 0x64, 0x01, 0x01}, 0x0008},
      {{0x58, 0x02, 0x03, 0x04, 0x05, 0x06}, {0x00, 0x11, 0x09, 0x64, 0x01, 0x01}, 0xDD86}},
    { RNIC_RM_NET_ID_1,                      MODEM_ID_0,                           "rmnet1",
      {{0x58, 0x02, 0x03, 0x04, 0x05, 0x07}, {0x00, 0x11, 0x09, 0x64, 0x01, 0x02}, 0x0008},
      {{0x58, 0x02, 0x03, 0x04, 0x05, 0x07}, {0x00, 0x11, 0x09, 0x64, 0x01, 0x02}, 0xDD86}},
    { RNIC_RM_NET_ID_2,                      MODEM_ID_0,                           "rmnet2",
      {{0x58, 0x02, 0x03, 0x04, 0x05, 0x08}, {0x00, 0x11, 0x09, 0x64, 0x01, 0x03}, 0x0008},
      {{0x58, 0x02, 0x03, 0x04, 0x05, 0x08}, {0x00, 0x11, 0x09, 0x64, 0x01, 0x03}, 0xDD86}},
    { RNIC_RM_NET_ID_3,                      MODEM_ID_1,                           "rmnet3",
      {{0x58, 0x02, 0x03, 0x04, 0x05, 0x09}, {0x00, 0x11, 0x09, 0x64, 0x01, 0x04}, 0x0008},
      {{0x58, 0x02, 0x03, 0x04, 0x05, 0x09}, {0x00, 0x11, 0x09, 0x64, 0x01, 0x04}, 0xDD86}},
    { RNIC_RM_NET_ID_4,                      MODEM_ID_1,                           "rmnet4",
      {{0x58, 0x02, 0x03, 0x04, 0x05, 0x0a}, {0x00, 0x11, 0x09, 0x64, 0x01, 0x05}, 0x0008},
      {{0x58, 0x02, 0x03, 0x04, 0x05, 0x0a}, {0x00, 0x11, 0x09, 0x64, 0x01, 0x05}, 0xDD86}}
};
#else
const RNIC_NETCARD_ELEMENT_TAB_STRU           g_astRnicManageTbl[RNIC_NET_ID_MAX_NUM] = {
    { RNIC_RM_NET_ID_0,                      MODEM_ID_0,                           "rmnet0",
      {{0x58, 0x02, 0x03, 0x04, 0x05, 0x06}, {0x00, 0x11, 0x09, 0x64, 0x01, 0x01}, 0x0008},
      {{0x58, 0x02, 0x03, 0x04, 0x05, 0x06}, {0x00, 0x11, 0x09, 0x64, 0x01, 0x01}, 0xDD86}},
    { RNIC_RM_NET_ID_1,                      MODEM_ID_0,                           "rmnet1",
      {{0x58, 0x02, 0x03, 0x04, 0x05, 0x07}, {0x00, 0x11, 0x09, 0x64, 0x01, 0x02}, 0x0008},
      {{0x58, 0x02, 0x03, 0x04, 0x05, 0x07}, {0x00, 0x11, 0x09, 0x64, 0x01, 0x02}, 0xDD86}},
    { RNIC_RM_NET_ID_2,                      MODEM_ID_0,                           "rmnet2",
      {{0x58, 0x02, 0x03, 0x04, 0x05, 0x08}, {0x00, 0x11, 0x09, 0x64, 0x01, 0x03}, 0x0008},
      {{0x58, 0x02, 0x03, 0x04, 0x05, 0x08}, {0x00, 0x11, 0x09, 0x64, 0x01, 0x03}, 0xDD86}}
};
#endif


/*lint -e762*/
#if (FEATURE_OFF == FEATURE_SKB_EXP)
extern int netif_rx(struct sk_buff *skb);
#else
extern int netif_rx_ni_balong(struct sk_buff *skb);
#endif

/*lint +e762*/

/******************************************************************************
   5 ����ʵ��
******************************************************************************/
/*****************************************************************************
 �� �� ��  : RNIC_ShowDataFromIpStack
 ��������  : ��ӡ�յ�IPЭ��ջ������
 �������  : pstSkb   :SKBUF�����׵�ַ
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
 1.��    ��   : 2013��04��25��
   �޸�����   : �����ɺ���
*****************************************************************************/
VOS_VOID RNIC_ShowDataFromIpStack(
    struct sk_buff                     *pstSkb
)
{
    VOS_UINT32                          i;

    /* RNIC �������ݴ�ӡ���أ���ӡ�յ�TCP/IPЭ��ջ������ */
    if (VOS_TRUE == g_ulRnicPrintUlDataFlg)
    {
        vos_printf("RNIC_ShowDataFromIpStack: data len is %d. \r\n", pstSkb->len);

        vos_printf("RNIC_ShowDataFromIpStack: data content is: \r\n");

        for (i = 0; i < pstSkb->len; i++)
        {
            if (pstSkb->data[i] > 0xf)
            {
                vos_printf("%x", pstSkb->data[i]);
            }
            else
            {
                vos_printf("0%x", pstSkb->data[i]);
            }
        }

        vos_printf("\r\n");
    }

    return;
}

/*****************************************************************************
 �� �� ��  : RNIC_ShowDataDelMacHead
 ��������  : ��ӡȥ��MACͷ������
 �������  : pstSkb   :SKBUF�����׵�ַ
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
 1.��    ��   : 2013��04��25��
   �޸�����   : �����ɺ���
*****************************************************************************/
VOS_VOID RNIC_ShowDataDelMacHead(
    struct sk_buff                     *pstSkb
)
{
    VOS_UINT32                          i;

    /* RNIC �������ݴ�ӡ���أ���ӡȥmacͷ������ */
    if (VOS_TRUE == g_ulRnicPrintUlDataFlg)
    {
        vos_printf("RNIC_ShowDataDelMacHead after pull: data len is %d. \r\n", pstSkb->len);

        vos_printf("RNIC_ShowDataDelMacHead after pull: data content is: \r\n");

        for (i = 0; i < pstSkb->len; i++)
        {
            if (pstSkb->data[i] > 0xf)
            {
                vos_printf("%x", pstSkb->data[i]);
            }
            else
            {
                vos_printf("0%x", pstSkb->data[i]);
            }
        }
        vos_printf("\r\n");
    }

    return;
}
/*****************************************************************************
 �� �� ��  : RNIC_StopNetCard
 ��������  : RNIC���ر�ʱ�Ĵ���
 �������  : pstNetDev:�����豸ָ��
 �������  : ��
 �� �� ֵ  : VOS_INT:RNIC_OK, RNIC_ERROR
 ���ú���  :
 ��������  :

 �޸���ʷ      :
 1.��    ��   : 2011��12��06��
   �޸�����   : �����ɺ���
*****************************************************************************/
VOS_INT RNIC_StopNetCard(
    struct net_device                  *pstNetDev
)
{
    RNIC_NETCARD_DEV_INFO_STRU         *pstPriv;

    /* �������Ϊ�գ��򷵻ش��� */
    if (VOS_NULL_PTR == pstNetDev)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_StopNetCard:net ptr is Null!");
        return RNIC_ERROR;
    }

    /* ��ȡ����˽������ */
    pstPriv = (RNIC_NETCARD_DEV_INFO_STRU *)netdev_priv(pstNetDev);
    if (VOS_NULL_PTR == pstPriv)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_StopNetCard:priv ptr is Null!");
        return RNIC_ERROR;
    }

    /* ʹ��netif_stop_queue()ֹͣ�����������շ� */
    netif_stop_queue(pstNetDev);

    /* ��˽�������е�����״̬��־��Ϊ�ر� */
    pstPriv->enStatus = RNIC_NETCARD_STATUS_CLOSED;

    return RNIC_OK;

}

/*****************************************************************************
 �� �� ��  : RNIC_OpenNetCard
 ��������  : RNIC����ʱ�Ĵ���
 �������  : pstNetDev:�����豸ָ��
 �������  : ��
 �� �� ֵ  : VOS_INT:RNIC_OK, RNIC_ERR, RNIC_BUSY
 ���ú���  :
 ��������  :

 �޸���ʷ      :
 1.��    ��   : 2011��12��06��
   �޸�����   : �����ɺ���
*****************************************************************************/
VOS_INT RNIC_OpenNetCard(
    struct net_device                  *pstNetDev
)
{
    RNIC_NETCARD_DEV_INFO_STRU         *pstPriv;

    /* �������Ϊ�գ��򷵻ش��� */
    if (VOS_NULL_PTR == pstNetDev)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_OpenNetCard:net ptr is Null!");
        return RNIC_ERROR;
    }

    /* ��ȡ����˽������ */
    pstPriv = (RNIC_NETCARD_DEV_INFO_STRU *)netdev_priv(pstNetDev);
    if (VOS_NULL_PTR == pstPriv)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_OpenNetCard:priv ptr is Null!");
        return RNIC_ERROR;
    }

    /* ���豸����Ѿ��� */
    if (RNIC_NETCARD_STATUS_OPENED == pstPriv->enStatus)
    {
        RNIC_WARNING_LOG(ACPU_PID_RNIC, "RNIC_OpenNetCard:NetCard is opened");
        /* ���ش�����EBUSY */
        return RNIC_BUSY;
    }

    /* ���������������� */
    netif_start_queue(pstNetDev);

    /* ��˽�������е�����״̬��־��Ϊ�� */
    pstPriv->enStatus = RNIC_NETCARD_STATUS_OPENED;

    return RNIC_OK;

}


VOS_VOID RNIC_SendULDataInPdpActive(
    IMM_ZC_STRU                        *pstImmZc,
    RNIC_NETCARD_DEV_INFO_STRU         *pstPriv,
    VOS_UINT8                           ucRabId,
    VOS_UINT8                           ucNetIndex,
    ADS_PKT_TYPE_ENUM_UINT8             enIpType
)
{
    RNIC_UL_CTX_STRU                   *pstUlCtx;
    VOS_UINT8                           ucSendAdsRabId;
    MODEM_ID_ENUM_UINT16                enModemId;

    pstUlCtx                            = RNIC_GET_UL_CTX_ADDR(ucNetIndex);
    enModemId                           = RNIC_GET_MODEM_ID_BY_NET_ID(ucNetIndex);

    /* ����Modem Id��װRabId */
    if (MODEM_ID_0 == enModemId)
    {
        ucSendAdsRabId = ucRabId;
    }
#if (FEATURE_ON == FEATURE_MULTI_MODEM)
    else if (MODEM_ID_1 == enModemId)
    {
        ucSendAdsRabId = ucRabId | RNIC_RABID_TAKE_MODEM_1_MASK;
    }
#endif
    else
    {
        /* ����Modem0��Modem1���������ݣ�ֱ�Ӷ��� */
        RNIC_DBG_MODEM_ID_UL_DISCARD_NUM(1, ucNetIndex);

        /* �ͷ��ڴ� */
        IMM_ZcFree(pstImmZc);

        /* ����ͳ�� */
        pstPriv->stStats.tx_dropped++;
        pstUlCtx->stULDataStats.ulULTotalDroppedPkts++;

        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_ProcUlDataInPdpActive:Modem Id failed");
        return;
    }

    if (VOS_OK != ADS_UL_SendPacketEx(pstImmZc, enIpType, ucSendAdsRabId))
    {
        RNIC_DBG_SEND_UL_PKT_FAIL_NUM(1, ucNetIndex);

        /* �ͷ��ڴ� */
        IMM_ZcFree(pstImmZc);

        /* ����ͳ�� */
        pstPriv->stStats.tx_dropped++;
        pstUlCtx->stULDataStats.ulULTotalDroppedPkts++;

        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_ProcUlDataInPdpActive:Send packet failed");
        return;
    }

    RNIC_DBG_SEND_UL_PKT_NUM(1, ucNetIndex);

    /* ͳ������������Ϣ */
    pstPriv->stStats.tx_packets++;
    pstPriv->stStats.tx_bytes += pstImmZc->len;

    /* ͳ�Ʒ��͵��������ݰ����������ڰ��貦�ŶϿ� */
    pstUlCtx->stULDataStats.ulULPeriodSndPkts++;

    /* ͳ�Ʒ��͵������ֽ��������������ϱ� */
    pstUlCtx->stULDataStats.ulULPeriodSndBytes += pstImmZc->len;
    pstUlCtx->stULDataStats.ulULTotalSndBytes  += pstImmZc->len;

    return;
}
VOS_UINT32 RNIC_TransSkbToImmZC(
    IMM_ZC_STRU                       **pstImmZc,
    struct sk_buff                     *pstSkb,
    RNIC_NETCARD_DEV_INFO_STRU         *pstPriv,
    VOS_UINT8                           ucNetIndex
)
{
#if(FEATURE_ON == FEATURE_SKB_EXP)
    RNIC_UL_CTX_STRU                   *pstUlCtx = VOS_NULL_PTR;
    IMM_MEM_STRU                       *pstImmMem = VOS_NULL_PTR;
    pstUlCtx                            = RNIC_GET_UL_CTX_ADDR(ucNetIndex);
#endif

#if(FEATURE_OFF == FEATURE_SKB_EXP)
    *pstImmZc = (IMM_ZC_STRU *)pstSkb;

    return VOS_OK;
#else
    /* �����Linuxϵͳ�ṩ�����ݣ���Ҫת���ɿ���ڴ� */
    if (MEM_TYPE_SYS_DEFINED == pstSkb->private_mem.enType)
    {
        *pstImmZc = IMM_ZcStaticCopy((IMM_ZC_STRU *)pstSkb);

        /* �ڴ����ʧ����Ҫ�ͷ��ڴ� */
        if (VOS_NULL_PTR == *pstImmZc)
        {
             RNIC_DBG_UL_CHANGE_IMMZC_FAIL_NUM(1, ucNetIndex);

            /* ����ͳ�� */
            pstPriv->stStats.tx_dropped++;
            pstUlCtx->stULDataStats.ulULTotalDroppedPkts++;

            /* �ͷ��ڴ�, �ýӿ��ڲ��������ڴ����Դ */
            IMM_ZcFree((IMM_ZC_STRU *)pstSkb);
            RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_SkbTransToImmZC:Malloc IMM failed");
            return VOS_ERR;
        }

        /* ת���ɹ����ں˲��ͷ�LINUXϵͳ�ṩ�������ڴ棬��RNICֱ���ͷŵ� */
        IMM_ZcFree((IMM_ZC_STRU *)pstSkb);
    }
    /* ������û�������ڴ��ṩ������ */
    else
    {
        pstImmMem = (IMM_MEM_STRU *)pstSkb->private_mem.pMem;

        /* A�˹����ڴ�ص��ڴ�,����Ҫת�� */
        if (IMM_MEM_POOL_ID_SHARE == pstImmMem->enPoolId)
        {
            *pstImmZc = (IMM_ZC_STRU *)pstSkb;
        }
        /* A�˹����ڴ��������ڴ棬��Ҫ����ת��һ�ο���ڴ� */
        else
        {
            *pstImmZc = IMM_ZcStaticCopy((IMM_ZC_STRU *)pstSkb);

            /* �ڴ����ʧ����Ҫ�ͷ��ڴ� */
            if (VOS_NULL_PTR == *pstImmZc)
            {
                 RNIC_DBG_UL_CHANGE_IMMZC_FAIL_NUM(1, ucNetIndex);

                /* ����ͳ�� */
                pstPriv->stStats.tx_dropped++;
                pstUlCtx->stULDataStats.ulULTotalDroppedPkts++;

                /* �ͷ��ڴ�, �ýӿ��ڲ��������ڴ����Դ */
                IMM_ZcFree((IMM_ZC_STRU *)pstSkb);
                RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_SkbTransToImmZC:Malloc IMM failed");
                return VOS_ERR;
            }

            /* ת���ɹ����ں˲��ͷ�LINUXϵͳ�ṩ�������ڴ棬��RNICֱ���ͷŵ� */
            IMM_ZcFree((IMM_ZC_STRU *)pstSkb);
        }
    }

    return VOS_OK;
#endif
}
VOS_VOID RNIC_SendULIpv4Data(
    struct sk_buff                     *pstSkb,
    struct net_device                  *pstNetDev,
    RNIC_NETCARD_DEV_INFO_STRU         *pstPriv,
    VOS_UINT8                           ucNetIndex
)
{
    VOS_UINT32                          ulRst;
    IMM_ZC_STRU                        *pstImmZc;
    VOS_UINT8                           ucRabId;

    pstImmZc                            = VOS_NULL_PTR;

    RNIC_DBG_RECV_UL_IPV4_PKT_NUM(1, ucNetIndex);

    ucRabId = RNIC_GET_SPEC_NET_IPV4_RABID(ucNetIndex);

    /* �Ƿ�RABID */
    if (RNIC_INVALID_RABID == ucRabId)
    {
        RNIC_DBG_RAB_ID_ERR_NUM(1, ucNetIndex);

        /* �ͷ��ڴ� */
        IMM_ZcFree((IMM_ZC_STRU *)pstSkb);

        /* �澯��Ϣ */
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_SendULIpv4Data:ipv4 is not act!");
        return;
    }

    /* ��Ӧ���·�������ת���ɿ���ڴ� */
    ulRst = RNIC_TransSkbToImmZC(&pstImmZc, pstSkb, pstPriv, ucNetIndex);
    if (VOS_OK != ulRst)
    {
        /* ���ת��ʧ�ܣ�ֱ�ӷ��أ��������������ݴ��� */
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_SendULIpv4Data: TransSkbToImmZC Fail!");
        return;
    }

    /* PDP�������������ݵĴ��� */
    RNIC_SendULDataInPdpActive(pstImmZc, pstPriv, ucRabId, ucNetIndex, ADS_PKT_TYPE_IPV4);

    return;
}


VOS_VOID RNIC_SendULIpv6Data(
    struct sk_buff                     *pstSkb,
    struct net_device                  *pstNetDev,
    RNIC_NETCARD_DEV_INFO_STRU         *pstPriv,
    VOS_UINT8                           ucNetIndex
)
{
    VOS_UINT32                          ulRst;
    IMM_ZC_STRU                        *pstImmZc;
    VOS_UINT8                           ucRabId;

    pstImmZc                            = VOS_NULL_PTR;

    RNIC_DBG_RECV_UL_IPV6_PKT_NUM(1, ucNetIndex);

    ucRabId = RNIC_GET_SPEC_NET_IPV6_RABID(ucNetIndex);

    /* �Ƿ�RABID */
    if (RNIC_INVALID_RABID == ucRabId)
    {
        RNIC_DBG_RAB_ID_ERR_NUM(1, ucNetIndex);

        /* �ͷ��ڴ� */
        IMM_ZcFree((IMM_ZC_STRU *)pstSkb);

        /* �澯��Ϣ */
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_SendULIpv6Data:ipv6 is net act!");
        return;
    }

     /* ��Ӧ���·�������ת���ɿ���ڴ� */
    ulRst = RNIC_TransSkbToImmZC(&pstImmZc, pstSkb, pstPriv, ucNetIndex);
    if (VOS_OK != ulRst)
    {
        /* ���ת��ʧ�ܣ�ֱ�ӷ��أ��������������ݴ��� */
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_SendULIpv6Data: TransSkbToImmZC Fail!");
        return;
    }

    /* PDP�������������ݵĴ��� */
    RNIC_SendULDataInPdpActive(pstImmZc, pstPriv, ucRabId, ucNetIndex, ADS_PKT_TYPE_IPV6);

    return;

}



VOS_UINT32 RNIC_ProcDemDial(
    struct sk_buff                     *pstSkb
)
{
    RNIC_DIAL_MODE_STRU                *pstDialMode;
    RNIC_TIMER_STATUS_ENUM_UINT8        enTiStatus;
    VOS_UINT32                          ulIpAddr;

    /* ��ȡIP��ַ */
    ulIpAddr = *((VOS_UINT32 *)((pstSkb->data) + RNIC_IP_HEAD_DEST_ADDR_OFFSET));

    /* ��ȡ���貦�ŵ�ģʽ�Լ�ʱ���ĵ�ַ */
    pstDialMode = RNIC_GetDialModeAddr();

    /*����ǹ㲥�����򲻷����貦�ţ�ֱ�ӹ��˵�*/
    if ( RNIC_IPV4_BROADCAST_ADDR == ulIpAddr )
    {
        RNIC_DBG_UL_RECV_IPV4_BROADCAST_NUM(1, RNIC_RM_NET_ID_0);

        /* �ͷ��ڴ� */
        IMM_ZcFree((IMM_ZC_STRU *)pstSkb);
        return VOS_ERR;
    }

    /* ��ȡ��ǰ���ű�����ʱ����״̬ */
    enTiStatus  = RNIC_GetTimerStatus(TI_RNIC_DEMAND_DIAL_PROTECT);

    /*Ϊ�˷�ֹ���貦���ϱ�̫�죬����һ�����붨ʱ����*/
    if (RNIC_TIMER_STATUS_STOP == enTiStatus)
    {
        /* ֪ͨӦ�ý��в��Ų��� */
        if (RNIC_ALLOW_EVENT_REPORT == pstDialMode->enEventReportFlag)
        {
            if (VOS_OK == RNIC_SendDialEvent(DEVICE_ID_WAN, RNIC_DAIL_EVENT_UP))
            {
                /* �������ű�����ʱ��  */
                RNIC_StartTimer(TI_RNIC_DEMAND_DIAL_PROTECT, TI_RNIC_DEMAND_DIAL_PROTECT_LEN);
                RNIC_DBG_SEND_APP_DIALUP_SUCC_NUM(1, RNIC_RM_NET_ID_0);
                RNIC_NORMAL_LOG(ACPU_PID_RNIC, "RNIC_ProcDemDial: Send dial event succ.");
            }
            else
            {
                RNIC_DBG_SEND_APP_DIALUP_FAIL_NUM(1, RNIC_RM_NET_ID_0);
                RNIC_WARNING_LOG(ACPU_PID_RNIC, "RNIC_ProcDemDial: Send dial event fail.");
            }

            RNIC_MNTN_TraceDialConnEvt();
        }
    }

    /* �ͷ��ڴ� */
    IMM_ZcFree((IMM_ZC_STRU *)pstSkb);

    return VOS_OK;
}
netdev_tx_t RNIC_StartXmit(
    struct sk_buff                     *pstSkb,
    struct net_device                  *pstNetDev
)
{
    VOS_UINT8                           ucIpType;
#if(FEATURE_ON == FEATURE_SKB_EXP)
    VOS_UINT32                          ulSndPermitFlg;
#endif
    RNIC_NETCARD_DEV_INFO_STRU         *pstPriv;
    VOS_UINT8                           ucNetIndex;

    /* ׷�����н������� */
    RNIC_MNTN_TraceRcvUlData();

    /* ���������Ч�� */
    if (VOS_NULL == pstSkb)
    {
        /* �澯��Ϣ */
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_StartXmit:skb is invalid!");

        return NETDEV_TX_OK;
    }

    /* ��������Ƿ�Ϊ�� */
    if (VOS_NULL == pstNetDev)
    {
        /* �ô���¼�����������󶪰�����,��������������ǲ�֪���ĸ����������
           Ŀǰ�����ֶ�����¼������0���� */
        RNIC_DBG_NETCAED_UL_DISCARD_NUM(1, RNIC_RM_NET_ID_0);

        /* �ͷ��ڴ� */
        IMM_ZcFree((IMM_ZC_STRU *)pstSkb);

        /* �澯��Ϣ */
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_StartXmit:net ptr is Null!");

        return NETDEV_TX_OK;
    }
    /* ���������豸��˽�����ݻ�ȡ��Ӧ����ID */
    pstPriv    = (RNIC_NETCARD_DEV_INFO_STRU *)netdev_priv(pstNetDev);

    ucNetIndex = pstPriv->enRmNetId;

    if (ucNetIndex >= RNIC_NET_ID_MAX_NUM)
    {
        /* �ô���¼�����������󶪰�����,��������������ǲ�֪���ĸ����������
           Ŀǰ�����ֶ�����¼������0���� */
        RNIC_DBG_NET_ID_UL_DISCARD_NUM(1, RNIC_RM_NET_ID_0);

        /* �ͷ��ڴ� */
        IMM_ZcFree((IMM_ZC_STRU *)pstSkb);

        /* �澯��Ϣ */
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_StartXmit:net index is Null!");

        return NETDEV_TX_OK;
    }

#if (FEATURE_ON == FEATURE_SKB_EXP)
    /* ��ȡ���������ͱ�ʶ */
    ulSndPermitFlg = ADS_UL_IsSendPermit();
#endif

    /* ��������ʱ, ֱ�Ӷ������ݰ� */
#if (FEATURE_ON == FEATURE_SKB_EXP)
    if ((RNIC_FLOW_CTRL_STATUS_START == RNIC_GET_FLOW_CTRL_STATUS(ucNetIndex))
     && (VOS_FALSE == ulSndPermitFlg))
#else
    if (RNIC_FLOW_CTRL_STATUS_START == RNIC_GET_FLOW_CTRL_STATUS(ucNetIndex))
#endif
    {
        RNIC_DBG_FLOW_CTRL_UL_DISCARD_NUM(1, ucNetIndex);

        /* �ͷ��ڴ� */
        IMM_ZcFree((IMM_ZC_STRU *)pstSkb);

        return NETDEV_TX_OK;
    }

    /* ��ӡ�յ�TCP/IPЭ��ջ������ */
    RNIC_ShowDataFromIpStack(pstSkb);

    skb_pull(pstSkb, RNIC_ETH_HDR_SIZE);

    /* ��ӡȥmacͷ������ */
    RNIC_ShowDataDelMacHead(pstSkb);

    /* ��ͨ��IP�ײ�Ϊ20�ֽ� */
    /*-------------------------------------------------------------------
      |      0        |       1        |       2        |       3        |
      |---------------|----------------|----------------|----------------|
      |0|1|2|3|4|5|6|7||0|1|2|3|4|5|6|7||0|1|2|3|4|5|6|7||0|1|2|3|4|5|6|7|
      --------------------------------------------------------------------
      | Э��  | �ײ�  |    ��������    |            �ܳ���               |
      | �汾  | ����  |     (TOS)      |           (�ֽ���)              |
      --------------------------------------------------------------------
      |            16λ��ʶ            | ��־ |         13λƬƫ��       |
      --------------------------------------------------------------------
      | ����ʱ��(TTL) |    8λЭ��     |        16λ�ײ�У���           |
      --------------------------------------------------------------------
      |                           32λԴIP��ַ                           |
      --------------------------------------------------------------------
      |                           32λĿ��IP��ַ                         |
      --------------------------------------------------------------------
    */
    /* ��ȡЭ��汾�� */
    ucIpType    = ((pstSkb->data[0] >> 4) & RNIC_IP_HEAD_VERSION_OFFSET_MASK);

    /* ֻ������0����Żᴥ�����貦�� */
    if ((RNIC_IPV4_VERSION == ucIpType)
     && (AT_RNIC_DIAL_MODE_DEMAND_DISCONNECT == RNIC_GET_DIAL_MODE())
     && (RNIC_PDP_REG_STATUS_DEACTIVE == RNIC_GET_SPEC_NET_IPV4_REG_STATE(ucNetIndex))
     && (RNIC_RM_NET_ID_0 == ucNetIndex))
    {

        /* �����貦�� */
        if (VOS_ERR == RNIC_ProcDemDial(pstSkb))
        {
            RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_StartXmit, the data is discared!");
        }

        return NETDEV_TX_OK;
    }

    /* IP���������ж� */
    switch(ucIpType)
    {
        /* IPv4 ���� */
        case RNIC_IPV4_VERSION:
            RNIC_SendULIpv4Data(pstSkb, pstNetDev, pstPriv, ucNetIndex);
            break;

        /* IPv6 ���� */
        case RNIC_IPV6_VERSION:
            RNIC_SendULIpv6Data(pstSkb, pstNetDev, pstPriv, ucNetIndex);
            break;

        default:
            RNIC_DBG_RECV_UL_ERR_PKT_NUM(1, ucNetIndex);

            /* �ͷ��ڴ� */
            IMM_ZcFree((IMM_ZC_STRU *)pstSkb);
            break;
    }

    return NETDEV_TX_OK;

}

/*****************************************************************************
 �� �� ��  : RNIC_AddMacHead
 ��������  : ��IMM_ZC_STRU �㿽���ṹ���MACͷ
 �������  : IMM_ZC_STRU *pstImmZc
             const VOS_UINT8  *pucAddData
             VOS_UINT16 usLen
 �������  : ��
 �� �� ֵ  : VOS_OK ��ӳɹ�
             VOS_ERR ���ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��2��
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 RNIC_AddMacHead (
    IMM_ZC_STRU                        *pstImmZc,
    const VOS_UINT8                    *pucAddData
)
{
    VOS_UINT8                          *pucDestAddr;


    if (VOS_NULL == pstImmZc)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_AddMacHead :pstImmZc ptr is null!");
        return VOS_ERR;
    }

    if (VOS_NULL == pucAddData)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_AddMacHead: pucData ptr is null!");
        return VOS_ERR;
    }

    if(RNIC_ETH_HDR_SIZE > (pstImmZc->data - pstImmZc->head))
    {
        RNIC_ERROR_LOG2(ACPU_PID_RNIC, "RNIC_AddMacHead : invalid data Len! data = 0x%x, head = 0x%x \n",
                    (VOS_INT32)pstImmZc->data, (VOS_INT32)pstImmZc->head);

        return VOS_ERR;
    }

    pucDestAddr = skb_push(pstImmZc, RNIC_ETH_HDR_SIZE);

    VOS_MemCpy(pucDestAddr, pucAddData, RNIC_ETH_HDR_SIZE);

    return VOS_OK;

}


VOS_UINT32 RNIC_SendDlData(
    VOS_UINT8                           ucRabid,
    IMM_ZC_STRU                        *pstImmZc,
    ADS_PKT_TYPE_ENUM_UINT8             enPktType
)
{
    RNIC_NETCARD_DEV_INFO_STRU         *pstPriv;
    RNIC_DL_CTX_STRU                   *pstDlCtx;
    VOS_UINT8                          *pucAddData = VOS_NULL_PTR;
    VOS_UINT8                           ucNetIndex;
    VOS_UINT8                           ucUseRabid;
    VOS_UINT16                          usModemId;

    /* ׷�����н������� */
    RNIC_MNTN_TraceRcvDlData();

    /* ADS����RABID������ModemId��RABID��϶��� */
    usModemId  = (ucRabid & RNIC_RABID_TAKE_MODEM_1_MASK) > 6;
    ucUseRabid = ucRabid & RNIC_RABID_UNTAKE_MODEM_1_MASK;

    /* ����RABID��ȡ��Ӧ����ID */
    ucNetIndex     = RNIC_GET_RM_NET_ID_BY_MODEM_ID(usModemId, ucUseRabid);
    if (RNIC_NET_ID_MAX_NUM == ucNetIndex)
    {
        RNIC_DBG_NETID_DL_DISCARD_NUM(1, RNIC_RM_NET_ID_0);

        /* �ͷ��ڴ� */
        IMM_ZcFree(pstImmZc);

        /* ��ӡ����LOG */
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_SendDlData:ucNetIndex is null!");

        return RNIC_INVAL;
    }

    pstDlCtx       = RNIC_GET_DL_CTX_ADDR(ucNetIndex);

    /* ��ȡ����˽�����ݵ�ַ */
    pstPriv        = RNIC_GET_SPEC_PRIV_INFO_ADDR(ucNetIndex);

    /* RNIC��˽������Ϊ�� */
    if (VOS_NULL_PTR == pstPriv)
    {
        RNIC_DBG_NETCAED_DL_DISCARD_NUM(1, ucNetIndex);

        /* �ͷ��ڴ� */
        IMM_ZcFree(pstImmZc);

        /* ��ӡ����LOG */
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_SendDlData:priv ptr is null!");

        return RNIC_INVAL;
    }

    /* ����δ�� */
    if (RNIC_NETCARD_STATUS_CLOSED == pstPriv->enStatus)
    {
        RNIC_DBG_DISCARD_DL_PKT_NUM(1, ucNetIndex);

        /* ����ͳ�ƣ��ͷ��ڴ� */
        pstPriv->stStats.rx_dropped++;
        pstDlCtx->stDLDataStats.ulDLTotalDroppedPkts++;
        IMM_ZcFree(pstImmZc);

        /* ��ӡ����LOG */
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_SendDlData:Netcard is closed!");

        return RNIC_OK;
    }

    /* skb���ݳ��ȳ�����Чֵ(������MACͷ) */
    if ((pstImmZc->len) > RNIC_MAX_PACKET)
    {
        RNIC_DBG_RECV_DL_BIG_PKT_NUM(1, ucNetIndex);

        /* ͳ�ƶ����ʹ����Ϣ */
        pstPriv->stStats.rx_errors++;
        pstPriv->stStats.rx_length_errors++;
        pstDlCtx->stDLDataStats.ulDLTotalDroppedPkts++;

        /* �ͷ��ڴ� */
        IMM_ZcFree(pstImmZc);

        /* ��ӡ����LOG */
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_SendDlData:Data is invalid!");

        return RNIC_OK;
    }

    /* ���MAC֡ͷ������ImmZc�ӿڽ�MAC֡ͷ����ImmZc�� */
    if (ADS_PKT_TYPE_IPV4 == enPktType)
    {
        RNIC_DBG_RECV_DL_IPV4_PKT_NUM(1, ucNetIndex);
        pucAddData = (VOS_UINT8*)&g_astRnicManageTbl[ucNetIndex].stIpv4Ethhead;
    }
    else if (ADS_PKT_TYPE_IPV6 == enPktType)
    {
        RNIC_DBG_RECV_DL_IPV6_PKT_NUM(1, ucNetIndex);
        pucAddData = (VOS_UINT8*)&g_astRnicManageTbl[ucNetIndex].stIpv6Ethhead;
    }
    else   /* ���ݰ����������֧�����Ͳ�һ�� */
    {
        RNIC_DBG_RECV_DL_ERR_PKT_NUM(1, ucNetIndex);
        RNIC_ERROR_LOG1(ACPU_PID_RNIC, "RNIC_SendDlData, Rab is different from PktType!", enPktType);

        /* �ͷ��ڴ� */
        IMM_ZcFree(pstImmZc);

        return RNIC_ERROR;
    }

    if (VOS_OK != RNIC_AddMacHead(pstImmZc, pucAddData))
    {
        RNIC_DBG_ADD_DL_MACHEAD_FAIL_NUM(1, ucNetIndex);
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_SendDlData, IMM_ZcAddMacHead fail!");

        /* �ͷ��ڴ� */
        IMM_ZcFree(pstImmZc);

        return RNIC_ERROR;
    }

    pstImmZc->protocol = eth_type_trans(pstImmZc, pstPriv->pstNetDev);

    /* ͳ����������������Ϣ */
    pstPriv->stStats.rx_packets++;
    pstPriv->stStats.rx_bytes += pstImmZc->len;

    /* ���� netif_rx()���������յ����ݽڵ�ݸ��ں� */
#if (FEATURE_OFF == FEATURE_SKB_EXP)
    if (NET_RX_SUCCESS != netif_rx((struct sk_buff *)pstImmZc))
#else
    if (NET_RX_SUCCESS != netif_rx_ni_balong((struct sk_buff *)pstImmZc))
#endif
    {
        RNIC_DBG_SEND_DL_PKT_FAIL_NUM(1, ucNetIndex);

        /* ����ͳ�ƣ��ͷ��ڴ� */
        pstPriv->stStats.rx_dropped++;
        pstDlCtx->stDLDataStats.ulDLTotalDroppedPkts++;
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_SendDlData:Send data failed!");
        return RNIC_ERROR;
    }

    /* �������з�������ͳ�� */
    RNIC_DBG_SEND_DL_PKT_NUM(1, ucNetIndex);

    /* ׷�����з������� */
    RNIC_MNTN_TraceSndDlData();

    /* ͳ���յ������������ֽ��������������ϱ� */
    pstDlCtx->stDLDataStats.ulDLPeriodRcvBytes += pstImmZc->len;
    pstDlCtx->stDLDataStats.ulDLTotalRcvBytes  += pstImmZc->len;

    return RNIC_OK;

}

/*****************************************************************************
 �� �� ��  : RNIC_SetMacAddress
 ��������  : RNIC����������MAC��ַ�Ĵ���
 �������  : pstNetDev:�����豸ָ��
             pMacAddr :MAC��ַ
 �������  : ��
 �� �� ֵ  : VOS_INT:RNIC_OK, RNIC_ADDR_INVALID
 ���ú���  :
 ��������  :

 �޸���ʷ     :
 1.��    ��   : 2011��12��06��
   �޸�����   : �����ɺ���
*****************************************************************************/
VOS_INT RNIC_SetMacAddress(
    struct net_device                  *pstNetDev,
    void                               *pMacAddr
)
{
    struct sockaddr                    *pstAddr;
    VOS_INT                             lisValid;

    /* ����ת�� */
    pstAddr                             = (struct sockaddr *)pMacAddr;

    /* ����ָ��Ϊ�� */
    if (VOS_NULL_PTR == pstNetDev)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_SetMacAddress:net ptr is Null!");

        return RNIC_ERROR;
    }

    /* MAC��ַΪ��Чֵ */
    lisValid = is_valid_ether_addr((VOS_UINT8 *)pstAddr->sa_data);
    if (VOS_FALSE == lisValid)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_SetMacAddress:Ether addr is invalid!");

        return RNIC_ADDR_INVALID;
    }

    /* ����ָ���dev_addrΪ�� */
    if (VOS_NULL_PTR == pstNetDev->dev_addr)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_SetMacAddress:Net dev addr is null!");

        return RNIC_ERROR;
    }

    /* ��MAC��ַ��ֵ������ */
    VOS_MemCpy(pstNetDev->dev_addr, pstAddr->sa_data, pstNetDev->addr_len);

    return RNIC_OK;

}

/*****************************************************************************
 �� �� ��  : RNIC_ChangeMtu
 ��������  : RNIC��������MTU��С
 �������  : pstNetDev:�����豸ָ��
             lNewMtu  :MTUֵ
 �������  : ��
 �� �� ֵ  : VOS_INT:RNIC_OK, RNIC_ERROR, RNIC_OUT_RANGE
 ���ú���  :
 ��������  :

 �޸���ʷ     :
 1.��    ��   : 2011��12��06��
   �޸�����   : �����ɺ���
*****************************************************************************/
VOS_INT RNIC_ChangeMtu(
    struct net_device                  *pstNetDev,
    VOS_INT                             lNewMtu
)
{
    /* ����ָ��Ϊ�� */
    if (VOS_NULL_PTR == pstNetDev)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_ChangeMtu:net ptr is Null!");

        return RNIC_ERROR;
    }

    /* new_mtu���Ȳ�����Ч��Χ�� */
    if (lNewMtu > RNIC_MAX_PACKET)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_ChangeMtu:Mtu out of range!");

        return RNIC_OUT_RANGE;
    }

    /* ����mtu��ɸ�ֵ */
    pstNetDev->mtu = (VOS_UINT)lNewMtu;

    return RNIC_OK;

}


/*****************************************************************************
 �� �� ��  : RNIC_Tx_Timeout
 ��������  : RNIC���ͳ�ʱ������,Ŀǰ��Ϊ������ַ��ͳ�ʱ���,�ݲ����κδ���
             �����Ժ���չʹ��
 �������  : pstNetDev:�����豸ָ��
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
 1.��    ��   : 2011��12��06��
   �޸�����   : �����ɺ���
*****************************************************************************/
VOS_VOID RNIC_ProcTxTimeout(
    struct net_device                  *pstNetDev
)
{
    if (VOS_NULL_PTR == pstNetDev)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_ProcTxTimeout:net ptr is Null!");

        return;
    }

    RNIC_NORMAL_LOG(ACPU_PID_RNIC, "RNIC_ProcTxTimeout:OK!");
    return;
}

/*****************************************************************************
 �� �� ��  : RNIC_Ioctrl
 ��������  : RNIC��ioctrl����,Ŀǰû�������ioctrl,�����Ժ���չ
 �������  : pstNetDev:�����豸ָ��
             pstIfr   :�û�����
             lCmd     :��������
 �������  : ��
 �� �� ֵ  : ������֧��  RNIC_NOTSUPP
 ���ú���  :
 ��������  :

 �޸���ʷ      :
 1.��    ��   : 2011��12��06��
   �޸�����   : �����ɺ���
*****************************************************************************/
VOS_INT RNIC_Ioctrl(
    struct net_device                  *pstNetDev,
    struct ifreq                       *pstIfr,
    VOS_INT                             lCmd
)
{
    RNIC_INFO_LOG(ACPU_PID_RNIC, "RNIC_Ioctrl:Ioctrl not supported!");

    return RNIC_NOTSUPP;
}


VOS_VOID RNIC_DeinitNetCard(
    VOS_VOID                           *pNetDev
)
{
    struct net_device                  *pstNetDev;
    RNIC_NETCARD_DEV_INFO_STRU         *pstPriv;

    pstNetDev                           = (struct net_device *)pNetDev;

    if (VOS_NULL_PTR == pstNetDev)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_DeinitNetCard:net ptr is Null!");

        return;
    }

    /* ��ȡ����˽������ */
    pstPriv = (RNIC_NETCARD_DEV_INFO_STRU *)netdev_priv(pstNetDev);
    if (pstPriv->enRmNetId >= RNIC_NET_ID_MAX_NUM)
    {
        return;
    }

    /* �ر��ز� */
    netif_carrier_off(pstNetDev);

    /* ֹͣ������������ */
    netif_stop_queue(pstNetDev);

    /* ȥע�������豸 */
    unregister_netdev(pstNetDev);

    /* �ͷ������豸 */
    RNIC_SetSpecNetCardPrivate(VOS_NULL_PTR, pstPriv->enRmNetId);

    free_netdev(pstNetDev);

    return;

}

/*****************************************************************************
 �� �� ��  : RNIC_GetNetCardStats
 ��������  : ��ȡRNIC����ͳ����Ϣ
 �������  : pstNetDev:�����豸ָ��
 �������  : ��
 �� �� ֵ  : net_device_stats
 ���ú���  :
 ��������  :

 �޸���ʷ     :
 1.��    ��   : 2011��12��06��
   �޸�����   : �����ɺ���
*****************************************************************************/
struct net_device_stats *RNIC_GetNetCardStats(
    struct net_device                  *pstNetDev
)

{
    RNIC_NETCARD_DEV_INFO_STRU         *pstPriv ;

    if (VOS_NULL_PTR == pstNetDev)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_GetNetCardStats:net ptr is Null!");

        return VOS_NULL_PTR;
    }

    /* ��ȡ˽������ */
    pstPriv = (RNIC_NETCARD_DEV_INFO_STRU *)netdev_priv(pstNetDev);
    if (VOS_NULL_PTR == pstPriv)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_GetNetCardStats:Priv ptr is Null!");

        return VOS_NULL_PTR;
    }

    /* ���豸��ͳ����Ϣ���� */
    return &pstPriv->stStats;
}


VOS_INT RNIC_InitNetCard(VOS_VOID)
{
    struct net_device                  *pstNetDev   = VOS_NULL_PTR;
    VOS_INT                             lRegRst;
    RNIC_NETCARD_DEV_INFO_STRU         *pstPriv ;
    RNIC_NETCARD_DEV_INFO_STRU         *pstNetCardPrivate ;
    VOS_UINT8                           ucIndex;

#if (FEATURE_ON == FEATURE_MULTI_MODEM)
    VOS_UINT8                           aucAddr[RNIC_NET_ID_MAX_NUM][RNIC_MAC_ADDR_LEN] = {
                                               {0x58,0x02,0x03,0x04,0x05,0x06},
                                               {0x58,0x02,0x03,0x04,0x05,0x07},
                                               {0x58,0x02,0x03,0x04,0x05,0x08},
                                               {0x58,0x02,0x03,0x04,0x05,0x09},
                                               {0x58,0x02,0x03,0x04,0x05,0x0a}
                                               };
#else
    VOS_UINT8                           aucAddr[RNIC_NET_ID_MAX_NUM][RNIC_MAC_ADDR_LEN] = {
                                               {0x58,0x02,0x03,0x04,0x05,0x06},
                                               {0x58,0x02,0x03,0x04,0x05,0x07},
                                               {0x58,0x02,0x03,0x04,0x05,0x08}
                                               };
#endif

    for (ucIndex = 0 ; ucIndex < RNIC_NET_ID_MAX_NUM ; ucIndex++)
    {
        /* ���������豸netdev��˽������ */
        /* alloc_etherdev����ӿڷ�����net_device�� RNIC_NETCARD_DEV_INFO_STRU�����ӿڵ��ڴ�
           RNIC_NETCARD_DEV_INFO_STRU�ṹ�ҽ���net_device�ĺ��� */
        pstNetDev = alloc_etherdev(sizeof(RNIC_NETCARD_DEV_INFO_STRU));

        /*��������ʧ�ܣ������豸ָ��ΪNULL */
        if (VOS_NULL_PTR == pstNetDev)
        {
            RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_InitNetCard:net ptr is Null!");
            return RNIC_NOMEM;
        }

        /* �����豸��ַΪ��ʱ */
        if (VOS_NULL_PTR == pstNetDev->dev_addr)
        {
            RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_InitNetCard:net addr ptr is Null!");
            free_netdev(pstNetDev);
            return RNIC_NOMEM;
        }

        /* ���豸����MAC��ַ,�Ժ���Ҫ��̶�ֵ */
        VOS_MemCpy(pstNetDev->dev_addr, aucAddr[ucIndex], RNIC_MAC_ADDR_LEN);

        /* ����Ĭ�ϵ�MTUֵ */
        pstNetDev->mtu = RNIC_DEFAULT_MTU;

        VOS_sprintf(pstNetDev->name, g_astRnicManageTbl[ucIndex].pucRnicNetCardName);

        /* �����뵽��net_device�ṹ���豸ָ��ĺ�������и�ֵ */
#if (VOS_OS_VER == VOS_WIN32)

#else
        pstNetDev->netdev_ops = &rnic_ops;
#endif

        /* ��ȡ����˽����������ַ������������������ */
        pstPriv = (RNIC_NETCARD_DEV_INFO_STRU *)netdev_priv(pstNetDev);
        pstPriv->enRmNetId = ucIndex;
        RNIC_SetSpecNetCardPrivate(pstPriv, ucIndex);

        /* ����������ַ������˽�������� */
        pstNetCardPrivate = RNIC_GetSpecNetCardPrivateAddr(ucIndex);
        pstNetCardPrivate->pstNetDev = pstNetDev;

        /* �ر��ز� */
        netif_carrier_off(pstNetDev);

        /* ֹͣ������������ */
        netif_stop_queue(pstNetDev);

        /* ����register_netdev������Netcard����ע�� */
        lRegRst = register_netdev(pstNetDev);
        if (VOS_OK != lRegRst)
        {
            /*  ע��ʧ���ͷŴ�����RNIC�豸 */
            free_netdev(pstNetDev);

            RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_InitNetCard:Register netdev failed!");

            return RNIC_STATE;
        }

        /* ���ز� */
        netif_carrier_on(pstNetDev);

        /* ���������������� */
        netif_start_queue(pstNetDev);
    }

    /* ���سɹ� */
    return RNIC_OK;
}


VOS_UINT32 RNIC_StartFlowCtrl(VOS_UINT8 ucRmNetId)
{
    RNIC_SET_FLOW_CTRL_STATUS(RNIC_FLOW_CTRL_STATUS_START, ucRmNetId);

    return VOS_OK;
}


VOS_UINT32 RNIC_StopFlowCtrl(VOS_UINT8 ucRmNetId)
{
    RNIC_SET_FLOW_CTRL_STATUS(RNIC_FLOW_CTRL_STATUS_STOP, ucRmNetId);

    return VOS_OK;
}


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

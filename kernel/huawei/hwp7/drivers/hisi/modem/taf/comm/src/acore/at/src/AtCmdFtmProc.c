/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : AtCmdFtmProc.c
  �� �� ��   : ����
  ��������   : 2013��3��13��
  ����޸�   :
  ��������   : ���̲���������ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��3��13��
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "AtCmdFtmProc.h"
#include "ATCmdProc.h"

#if (FEATURE_ON == FEATURE_LTE)
#include "msp_diag.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
/*lint -e767*/
#define    THIS_FILE_ID                 PS_FILE_ID_AT_CMD_FTM_PROC_C
/*lint +e767*/

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
/*****************************************************************************
 �� �� ��  : At_SetLogPortPara
 ��������  : ^LOGPORT�����ú���
 �������  : VOS_UINT8 ucIndex
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��6��20��
    �޸�����   : �����ɺ���
  2.��    ��   : 2013��8��9��
    �޸�����   : LOG35_TL����TL�ӿ�
*****************************************************************************/
VOS_UINT32 At_SetLogPortPara(VOS_UINT8 ucIndex)
{
    VOS_UINT32                          ulRslt;
    VOS_UINT32                          ulOmLogPort;

    /* ������� */
    if (AT_CMD_OPT_SET_PARA_CMD != g_stATParseCmd.ucCmdOptType)
    {
        return AT_ERROR;
    }

    /* �������� */
    if (gucAtParaIndex != 1)
    {
        return AT_ERROR;
    }

#if (FEATURE_ON == FEATURE_LTE)
    ulRslt = diag_LogPortSwitch(gastAtParaList[0].ulParaValue);

    if (VOS_OK != ulRslt)
    {
        AT_WARN_LOG1("At_SetLogPortPara TL Error, Para: %d \n", gastAtParaList[0].ulParaValue);
        AT_WARN_LOG1("At_SetLogPortPara TL Error, Result: 0x%x \n", ulRslt);

        return AT_ERROR;
    }
#endif

    if (AT_LOG_PORT_USB == gastAtParaList[0].ulParaValue)
    {
        ulOmLogPort = COMM_LOG_PORT_USB;
    }
    else
    {
        ulOmLogPort = COMM_LOG_PORT_VCOM;
    }

    /* ����OM�Ľӿ� */
    ulRslt = GU_OamLogPortSwitch(ulOmLogPort);

    if (VOS_OK == ulRslt)
    {
        return AT_OK;
    }
    else
    {
        return AT_ERROR;
    }
}

/*****************************************************************************
 �� �� ��  : At_QryLogPortPara
 ��������  : ^LOGPORT�Ĳ�ѯ����
 �������  : VOS_UINT8 ucIndex
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��6��20��
    �޸�����   : �����ɺ���
  2.��    ��   : 2013��8��9��
    �޸�����   : LOG35_TL����TL�ӿ�
*****************************************************************************/
VOS_UINT32 At_QryLogPortPara(VOS_UINT8 ucIndex)
{
    VOS_UINT16                          usLength;
    VOS_UINT32                          ulOmLogPort;
    VOS_UINT32                          ulAtLogPort;
    VOS_UINT32                          ulRslt;
#if (FEATURE_ON == FEATURE_LTE)
    VOS_UINT32                          ulTlLogPort;
#endif

    usLength                            = 0;
    ulOmLogPort                         = AT_LOG_PORT_USB;

    ulRslt = GU_OamQueryLogPort(&ulOmLogPort);

    if (VOS_OK != ulRslt)
    {
        return AT_ERROR;
    }

    if (COMM_LOG_PORT_USB == ulOmLogPort)
    {
        ulAtLogPort = AT_LOG_PORT_USB;
    }
    else
    {
        ulAtLogPort = AT_LOG_PORT_VCOM;
    }

#if (FEATURE_ON == FEATURE_LTE)
    ulTlLogPort = diag_getLogPort();
    if (ulAtLogPort != ulTlLogPort)
    {
        vos_printf("\n At_QryLogPortPara ulAtLogPort: %d \n", ulAtLogPort);
        vos_printf("\n At_QryLogPortPara ulTlLogPort: %d \n", ulTlLogPort);
    }
#endif

    usLength  = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                       (VOS_CHAR*)pgucAtSndCodeAddr,
                                       (VOS_CHAR*)pgucAtSndCodeAddr,
                                       "%s: ",
                                       g_stParseContext[ucIndex].pstCmdElement->pszCmdName);

    usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                       (VOS_CHAR*)pgucAtSndCodeAddr,
                                       (VOS_CHAR*)pgucAtSndCodeAddr + usLength,
                                       "%d",
                                       ulAtLogPort);

    gstAtSendData.usBufLen = usLength;

    return AT_OK;
}

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


/************************************************************************
  Copyright    : 2005-2007, Huawei Tech. Co., Ltd.
  File name    : MMA_APPRemote.c
  Version      : V200R002
  Date         : 2005-09-10
  Description  : ��C�ļ�������MMAģ���Զ��ʵ��
                        1������γ����ݴ��������mux��
                        2�������ϱ����¼������ݴ���APP��AT


  Function List:
        1) Taf_DefPhAccessMode
        2) Taf_DefPhBand
        3) Taf_DefPhClassType
        4) Taf_DefPhMode
        5) Taf_InitEventInfoOP
        6) Taf_MmaGetInfo
        7) Taf_PhoneAttach
        8) Taf_PhoneDetach
        9) Taf_PhonePinHandle
       10) Taf_PhonePlmnList
       11) Taf_PhonePlmnResel
       12) Taf_PhonePlmnUserSel
       13) Taf_PhoneStart
       14) Taf_PhoneStartInfoInd
       15) Taf_PhoneStop

  History      :
  1. Date:2005-09-10
     Modification:Create

  1.��    ��   : 2006��2��23��
    �޸�����   : ARM�ڴ��������Ҫ��4�ֽڿ�ʼ��λ�û�ȡ,Taf_PhonePlmnUserSel
                 ������������λ.���ⵥ��:A32D02119

  2.��    ��   : 2006��3��24��
    �޸�����   : ���ⵥ�ţ�A32D02738

  3. x51137 A32D06630 2006/10/25
Date         Updated By      PR Number
2006-10-26   Li Jilin        A32D06630
  5.��    ��   : 2007��02��11��
    �޸�����   : ���ⵥ��:A32D08708
  6.Date        : 2007-10-16
    Modification: ���ⵥA32D13062(�ĵ�У�Է�������)
  7.��    ��   : 2007��10��9��
    �޸�����   : �����ɺ�����A32D13994
  8.��    ��   : 2008��3��17��
    �޸�����   : ���ⵥ��:AT2D02570,NAS B005�汾�����������
  9.��    ��   : 2008��06��17��
    �޸�����   : ���ⵥ�ţ�AT2D03804,
                 ���� ^HS����
************************************************************************/


/*****************************************************************************
   1 ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "Taf_Tafm_Remote.h"
#include "ATCmdProc.h"
#include "PsCommonDef.h"
#include "NVIM_Interface.h"
#include "TafClientApi.h"


/* #include "MnComm.h" */
#include "MnCommApi.h"


#include "AtParse.h"

#ifdef  __cplusplus
 #if  __cplusplus
extern "C" {
 #endif
#endif

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/

/*lint -e767 �޸���:�޽� 107747;������:���ٻ�65952;ԭ��:Log��ӡ*/
#define    THIS_FILE_ID PS_FILE_ID_MMA_REMOTE_C
/*lint +e767 �޸���:�޽� 107747;������:sunshaohua*/

/*****************************************************************************
   2 ȫ�ֱ�������
*****************************************************************************/

/*****************************************************************************
   3 ����ʵ��
*****************************************************************************/


/**********************************************************
 Function:       Taf_DefPhServiceMode
 Description:    ҵ��ģʽ����
 Calls:          APP/AT
 Data Accessed:
 Data Updated:
 Input:      ClientId     - APP/AT��ʶ
             OpId         - ������ʶ
             ServiceMode  - ͨ�Ų���
 Output:
 Return:         TAF_SUCCESS - �ɹ�
                 TAF_FAILURE - ʧ��
 Others:
    1.Date        : 2007-10-16
      Modification: ���ⵥA32D13062(�ĵ�У�Է�������)
**********************************************************/
TAF_UINT32  Taf_DefPhClassType ( MN_CLIENT_ID_T       ClientId,
                                 MN_OPERATION_ID_T    OpId,
                                 TAF_PH_MS_CLASS_TYPE MsClass)
{
    return MN_FillAndSndAppReqMsg( ClientId,
                                   OpId,
                                   TAF_MSG_MMA_MS_CLASS,
                                   &MsClass,
                                   sizeof(TAF_PH_MS_CLASS_TYPE),
                                   I0_WUEPS_PID_MMA);
}

/*****************************************************************************
 �� �� ��  : Taf_DefPhMode
 ��������  :  �����ն˹��ܼ��𣨣�CFUN�����������󣩣�
              ͨ�����ûص���������
 �������  :  ClientId  - APP/AT��ʶ
              OpId      - ������ʶ
              PhModeSet - �ֻ�����ģʽ
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2005��8��25��
    �޸�����   : V200R001�汾�����ɺ���

*****************************************************************************/
VOS_UINT32 Taf_DefPhMode(MN_CLIENT_ID_T   ClientId,
                         MN_OPERATION_ID_T   OpId,
                         TAF_PH_OP_MODE_STRU PhModeSet)
{
    return MN_FillAndSndAppReqMsg( ClientId,
                                   OpId,
                                   TAF_MSG_MMA_FUN,
                                   &PhModeSet,
                                   sizeof(TAF_PH_OP_MODE_STRU),
                                   I0_WUEPS_PID_MMA);
}

/*****************************************************************************
 �� �� ��  : Taf_PhonePinHandle
 ��������  : +CPIN USIM�ṩ��PIN������API
             void Api_PIN_Handle(VOS_UINT8 ucClientId,VOS_UINT8 ucCmdType, VOS_UINT8
             ucPINType,\
             VOS_UINT8 *pucOldPIN, VOS_UINT8 *pucNewPIN)��
 �������  : ClientId - APP/AT��ʶ
                 OpId - ������ʶ
*pPinData - �ֻ�PIN��������ݽṹ
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2005��8��25��
    �޸�����   : V200R001�汾���ɺ���
  2.��    ��   : 2007��10��16��
    �޸�����   : ���ⵥA32D13062(�ĵ�У�Է�������)
*****************************************************************************/
TAF_UINT32 Taf_PhonePinHandle ( MN_CLIENT_ID_T          ClientId,
                                MN_OPERATION_ID_T       OpId,
                                TAF_PH_PIN_DATA_STRU   *pPinData)
{
    return MN_FillAndSndAppReqMsg( ClientId,
                                   OpId,
                                   TAF_MSG_MMA_OP_PIN_REQ,
                                   pPinData,
                                   sizeof(TAF_PH_PIN_DATA_STRU),
                                   I0_WUEPS_PID_MMA);
}


/*****************************************************************************
 �� �� ��  : Taf_MePersonalisationHandle
 ��������  : ����״̬��ѯ
 �������  : pMsg  ��Ϣ��
 �������  : ClientId - APP/AT��ʶ
             OpId - ������ʶ
 �� �� ֵ  : �ɹ���ʧ��
 �޸���ʷ      :
  1.��    ��   : 2007��9��28��
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 Taf_MePersonalisationHandle(MN_CLIENT_ID_T          ClientId,
                                       MN_OPERATION_ID_T                 OpId,
                                       TAF_ME_PERSONALISATION_DATA_STRU *pMePersonalData)
{
    return MN_FillAndSndAppReqMsg( ClientId,
                                   OpId,
                                   TAF_MSG_MMA_ME_PERSONAL_REQ,
                                   pMePersonalData,
                                   sizeof(TAF_ME_PERSONALISATION_DATA_STRU),
                                   I0_WUEPS_PID_MMA);
}

/**********************************************************/
/*                      �绰����API                       */
/**********************************************************/

/**********************************************************
 Function:       Taf_PhonePlmnResel
 Description:    PLMN��ѡ
 Calls:          APP/AT
 Data Accessed:  2005-09-16
 Data Updated:
 Input:          ClientId  - APP/AT��ʶ
                 OpId      - ������ʶ
                 ReselType - ��ѡ����
 Output:
 Return:         TAF_SUCCESS - �ɹ�
                 TAF_FAILURE - ʧ��
 Others:
    1.Date        : 2007-10-16
      Modification: ���ⵥA32D13062(�ĵ�У�Է�������)
**********************************************************/
TAF_UINT32 Taf_PhonePlmnResel ( MN_CLIENT_ID_T           ClientId,
                                MN_OPERATION_ID_T        OpId,
                                TAF_PLMN_RESEL_MODE_TYPE ReselType)
{
    return MN_FillAndSndAppReqMsg( ClientId,
                                   OpId,
                                   TAF_MSG_MMA_PLMN_RESEL,
                                   &ReselType,
                                   sizeof(TAF_PLMN_RESEL_MODE_TYPE),
                                   I0_WUEPS_PID_MMA);
}

VOS_UINT32 TAF_PhonePlmnUserSel(
    MN_CLIENT_ID_T                      ClientId,
    MN_OPERATION_ID_T                   OpId,
    VOS_UINT16                          usOprNameLen,
    VOS_UINT8                          *pucOperName,
    AT_COPS_FORMAT_TYPE                 ucFormatType,
    TAF_PH_RA_MODE                      AccessMode
)
{
    VOS_UINT32                           ulRst;
    VOS_UINT32                           ulGetOperNameRst;
    TAF_PLMN_USER_SEL_STRU               stPlmnUserSel;

    /* ����Ϊ�� */
    if (0 == usOprNameLen)
    {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    stPlmnUserSel.ucFormatType = ucFormatType;
    stPlmnUserSel.ucAccessMode = AccessMode;

    /* ��������PLMN�����������浽TAF_PLMN_USER_SEL_STRU�ṹ�У����͵�MMAģ�飬��MMA����ת��������ָ���ѡ� */
    ulGetOperNameRst = AT_GetOperatorNameFromParam(&stPlmnUserSel.usOperNameLen,
                                                    stPlmnUserSel.ucOperName,
                                                    sizeof(stPlmnUserSel.ucOperName),
                                                    stPlmnUserSel.ucFormatType);

    if (AT_OK != ulGetOperNameRst )
    {
        return ulGetOperNameRst;
    }

    /* ������Ϣ TAF_MSG_MMA_PLMN_USER_SEL �� MMA ����
       ����Ϣ��TAF_PLMN_USER_SEL_STRU���͵Ĳ����ṹ */
    ulRst = MN_FillAndSndAppReqMsg(ClientId,
                                   OpId,
                                   TAF_MSG_MMA_PLMN_USER_SEL,
                                   (VOS_UINT8*)&stPlmnUserSel, /*Ҫ���ѯPS��ע��״̬��*/
                                   sizeof(TAF_PLMN_USER_SEL_STRU),
                                   I0_WUEPS_PID_MMA);


    if (TAF_SUCCESS == ulRst )
    {
        return AT_OK;
    }

    return AT_ERROR;
}

/*****************************************************************************
 �� �� ��  : Taf_PhoneAttach
 ��������  : +CGATT ATTACH��ͨ����������ص������ϱ����ؽ��
 �������  : ClientId    - APP/AT��ʶ
             OpId        - ������ʶ
             AttachType  - ��������
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2005��8��25��
    �޸�����   :V200R001�汾 �����ɺ���
  2.��    ��   : 2007��10��16��
    �޸�����   : ���ⵥA32D13062(�ĵ�У�Է�������)

*****************************************************************************/
TAF_UINT32 Taf_PhoneAttach ( MN_CLIENT_ID_T     ClientId,
                             MN_OPERATION_ID_T  OpId,
                             TAF_PH_ATTACH_TYPE AttachType)
{
    return MN_FillAndSndAppReqMsg( ClientId,
                                   OpId,
                                   TAF_MSG_MMA_ATTACH,
                                   &AttachType,
                                   sizeof(TAF_PH_ATTACH_TYPE),
                                   I0_WUEPS_PID_MMA);
}

/**********************************************************
 Function:       Taf_PhonePlmnList
 Description:    ����PLMN����
 Calls:          APP/AT
 Data Accessed:  2005-09-16
 Data Updated:
 Input:          ClientId - APP/AT��ʶ
                 OpId     - ������ʶ
                 ListType - ��������
 Output:
 Return:         TAF_SUCCESS - �ɹ�
                 TAF_FAILURE - ʧ��
 Others:
    1.Date        : 2007-10-16
    Modification: ���ⵥA32D13062(�ĵ�У�Է�������)
**********************************************************/
TAF_UINT32 Taf_PhonePlmnList ( MN_CLIENT_ID_T           ClientId,
                               MN_OPERATION_ID_T        OpId,
                               TAF_AVAIL_PLMN_LIST_TYPE ListType)
{
    return MN_FillAndSndAppReqMsg( ClientId,
                                   OpId,
                                   TAF_MSG_MMA_PLMN_LIST,
                                   &ListType,
                                   sizeof(TAF_AVAIL_PLMN_LIST_TYPE),
                                   I0_WUEPS_PID_MMA);
}



VOS_UINT32 TAF_AbortPlmnListReq (
    MN_CLIENT_ID_T                      ClientId,
    MN_OPERATION_ID_T                   OpId
)
{
    return MN_FillAndSndAppReqMsg( ClientId,
                                   OpId,
                                   TAF_MSG_MMA_PLMN_LIST_ABORT_REQ,
                                   VOS_NULL_PTR,
                                   0,
                                   I0_WUEPS_PID_MMA );
}


/*****************************************************************************
 �� �� ��  : Taf_PhoneDetach
 ��������  : +CGATT DETACH��ͨ����������ص������ϱ����ؽ��
 �������  : ClientId   - APP/AT��ʶ
             OpId       - ������ʶ
             DetachType - �������
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2005��8��25��
    �޸�����   : V200R001�汾�����ɺ���
  2.��    ��   : 2007��10��16��
    �޸�����   : ���ⵥA32D13062(�ĵ�У�Է�������)
*****************************************************************************/
TAF_UINT32 Taf_PhoneDetach ( MN_CLIENT_ID_T     ClientId,
                             MN_OPERATION_ID_T  OpId,
                             TAF_PH_DETACH_TYPE DetachType)
{
    return MN_FillAndSndAppReqMsg( ClientId,
                                   OpId,
                                   TAF_MSG_MMA_DETACH,
                                   &DetachType,
                                   sizeof(TAF_PH_DETACH_TYPE),
                                   I0_WUEPS_PID_MMA);
}

/*****************************************************************************
 �� �� ��  : Taf_PhoneStartInfoInd
 ��������  : ��������������ϱ���Ϣ���ú��������趨�Ƿ��ϱ��ź�������С��ID����Ϣ��
             ����ϵͳ��Ϣ�����¼��ж����óɹ����
 �������  : stStartInfoInd -- �ο��ӿڶ���
 �������  : ��
 �� �� ֵ  : VOS_UINT32 -- 0:���óɹ�
                           1:����ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2005��12��13��
    �޸�����   : �����ɺ���
  2.��    ��   : 2007��10��16��
    �޸�����   : ���ⵥA32D13062(�ĵ�У�Է�������)
*****************************************************************************/
TAF_UINT32 Taf_PhoneStartInfoInd ( MN_CLIENT_ID_T          ClientId,
                                   MN_OPERATION_ID_T       OpId,
                                   TAF_START_INFO_IND_STRU StartInfoInd)
{
    return MN_FillAndSndAppReqMsg( ClientId,
                                   OpId,
                                   TAF_MSG_MMA_INFO_REPORT,
                                   &StartInfoInd,
                                   sizeof(TAF_START_INFO_IND_STRU),
                                   I0_WUEPS_PID_MMA);
}

/*****************************************************************************
 �� �� ��  : TAF_DefQuickStart
 ��������  : ���ÿ��ٿ���
 �������  : MN_CLIENT_ID_T ClientId
             MN_OPERATION_ID_T   OpId
             TAF_UINT32 ulSetValue
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2007��10��9��
    �޸�����   : �����ɺ���
*****************************************************************************/
TAF_UINT32 TAF_DefQuickStart(MN_CLIENT_ID_T ClientId, MN_OPERATION_ID_T OpId, TAF_UINT32 ulSetValue)
{
    return MN_FillAndSndAppReqMsg( ClientId,
                                   OpId,
                                   TAF_MSG_MMA_SET_QUICKSTART,
                                   &ulSetValue,
                                   sizeof(TAF_UINT32),
                                   I0_WUEPS_PID_MMA);
}


TAF_UINT32 Taf_DefPhAutoAttach(MN_CLIENT_ID_T ClientId, MN_OPERATION_ID_T OpId, TAF_UINT32 ulSetValue)
{
    return MN_FillAndSndAppReqMsg( ClientId,
                                   OpId,
                                   TAF_MSG_MMA_SET_AUTOATTACH,
                                   &ulSetValue,
                                   sizeof(TAF_UINT32),
                                   I0_WUEPS_PID_MMA);
}

/*****************************************************************************
 �� �� ��  : TAF_DefLoadDefault
 ��������  : �ָ���������
 �������  : MN_CLIENT_ID_T ClientId
             MN_OPERATION_ID_T   OpId
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2007��10��22��
    �޸�����   : creat function
*****************************************************************************/
TAF_UINT32 TAF_DefLoadDefault(MN_CLIENT_ID_T ClientId, MN_OPERATION_ID_T OpId)
{
    return MN_FillAndSndAppReqMsg( ClientId,
                                   OpId,
                                   TAF_MSG_MMA_SET_LOADDEFAULT,
                                   VOS_NULL_PTR,
                                   0,
                                   I0_WUEPS_PID_MMA);
}

/*****************************************************************************
 �� �� ��  : Taf_UsimRestrictedAccessCommand
 ��������  : ����֧�������ƵĿ���������
 �������  :
 �������  :
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2006��10��25��
    �޸�����   : �����ɺ���
*****************************************************************************/
TAF_UINT32 Taf_UsimRestrictedAccessCommand(MN_CLIENT_ID_T               ClientId,
                                           MN_OPERATION_ID_T            OpId,
                                           TAF_SIM_RESTRIC_ACCESS_STRU *pPara)
{
    return MN_FillAndSndAppReqMsg( ClientId,
                                   OpId,
                                   TAF_MSG_MMA_USIM_RESTRICTED_ACCESS,
                                   pPara,
                                   sizeof(TAF_SIM_RESTRIC_ACCESS_STRU),
                                   I0_WUEPS_PID_MMA);
}



/*****************************************************************************
 �� �� ��  : Taf_IndPhFreq
 ��������  : ָ��Ƶ������
 �������  : MN_CLIENT_ID_T ClientId
             MN_OPERATION_ID_T   OpId
             TAF_IND_FREQ_STRU Freq
 �������  : TAF_SUCCESS or TAF_FAILURE
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2006��10��24��
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 Taf_IndPhFreq(MN_CLIENT_ID_T     ClientId,
                         MN_OPERATION_ID_T        OpId,
                         TAF_IND_FREQ_STRU        Freq)
{
    VOS_UINT16 temp_Freq;
    VOS_UINT16 temp_value;

    switch (Freq.RatType)
    {
    case TAF_SYS_SUBMODE_GSM:
        temp_value   = Freq.GsmBand;
        temp_value <<= 12;
        temp_Freq   = temp_value;
        temp_value  = Freq.IndFreq;
        temp_value &= 0x0fff;
        temp_Freq |= temp_value;

        /* write temp_GSM_Freq to NVIM */
        if (NV_OK != NV_Write( en_NV_Item_Gsm_Ind_Freq, &temp_Freq, sizeof(VOS_UINT16)))
        {
            return TAF_FAILURE;
        }

        break;

    case TAF_SYS_SUBMODE_WCDMA:

        /*write Freq to NVIM */
        temp_Freq = Freq.IndFreq;
        if (NV_OK != NV_Write( en_NV_Item_Wcdma_Ind_Freq, &temp_Freq, sizeof(VOS_UINT16)))
        {
            return TAF_FAILURE;
        }

        break;

    default:
        return TAF_FAILURE;
    }

    return TAF_SUCCESS;
}

/*****************************************************************************
 �� �� ��  : Taf_FplmnHandle
 ��������  : FPLMNͨ�ò�������
 �������  :  ClientId  - APP/AT��ʶ
              OpId      - ������ʶ
              PhModeSet - �ֻ�����ģʽ
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
1.��    ��   : 2007��10��9��
  �޸�����   : �����ɺ�����A32D13994
*****************************************************************************/
TAF_UINT32 Taf_FplmnHandle(MN_CLIENT_ID_T            ClientId,
                           MN_OPERATION_ID_T         OpId,
                           TAF_PH_FPLMN_OPERATE_STRU stFplmnOperate)
{
    return MN_FillAndSndAppReqMsg( ClientId,
                                   OpId,
                                   TAF_MSG_MMA_FPLMN_HANDLE,
                                   &stFplmnOperate,
                                   sizeof(TAF_PH_FPLMN_OPERATE_STRU),
                                   I0_WUEPS_PID_MMA);
}


VOS_UINT32 Taf_SysCfgHandle(
    MN_CLIENT_ID_T                      ClientId,
    MN_OPERATION_ID_T                   OpId,
    TAF_PH_SYS_CFG_EX_STRU              stSysCfg
)
{

    return MN_FillAndSndAppReqMsg(ClientId,
                                  OpId,
                                  TAF_MSG_MMA_SYSCFG_HANDLE,
                                  &stSysCfg,
                                  sizeof(stSysCfg),
                                  I0_WUEPS_PID_MMA);
}



VOS_UINT32 Taf_NetScanHandle(
    MN_CLIENT_ID_T                      usClientId,
    MN_OPERATION_ID_T                   ucOpId,
    TAF_MMA_NET_SCAN_REQ_STRU          *pstNetScan
)
{
    return MN_FillAndSndAppReqMsg(usClientId,
                                  ucOpId,
                                  TAF_MSG_MMA_NET_SCAN_REQ,
                                  pstNetScan,
                                  sizeof(TAF_MMA_NET_SCAN_REQ_STRU),
                                  I0_WUEPS_PID_MMA);
}



VOS_UINT32 Taf_AbortNetScan (
    MN_CLIENT_ID_T                      usClientId,
    MN_OPERATION_ID_T                   ucOpId
)
{
    return MN_FillAndSndAppReqMsg( usClientId,
                                   ucOpId,
                                   TAF_MSG_MMA_ABORT_NET_SCAN_REQ,
                                   VOS_NULL_PTR,
                                   0,
                                   I0_WUEPS_PID_MMA );
}


/*****************************************************************************
 �� �� ��  : TAF_QryUsimInfo
 ��������  :
 �������  :
 �������  : Icctype
 �� �� ֵ  : TAF_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2009��9��19��
    �޸�����   : ��������,��ѯUSIM���������Ϣ

*****************************************************************************/
TAF_UINT32 TAF_QryUsimInfo(
    MN_CLIENT_ID_T                      ClientId,
    MN_OPERATION_ID_T                   OpId,
    TAF_PH_QRY_USIM_INFO_STRU           *pstInfo
)
{
    if ( (TAF_PH_ICC_UNKNOW == pstInfo->Icctype)
      || (pstInfo->Icctype > TAF_PH_ICC_USIM))
    {
        return TAF_FAILURE;
    }

    if (pstInfo->enEfId > TAF_PH_OPL_FILE)
    {
        return TAF_FAILURE;
    }
    return MN_FillAndSndAppReqMsg( ClientId,
                             OpId,
                             TAF_MSG_MMA_USIM_INFO,
                             pstInfo,
                             sizeof(TAF_PH_QRY_USIM_INFO_STRU),
                             I0_WUEPS_PID_MMA);

}

/*****************************************************************************
 �� �� ��  : TAF_QryCpnnInfo
 ��������  :
 �������  :
 �������  :
 �� �� ֵ  : TAF_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2009��9��19��
    �޸�����   : ��������,��ѯUSIM���������Ϣ

*****************************************************************************/
TAF_UINT32 TAF_QryCpnnInfo(
    MN_CLIENT_ID_T                      ClientId,
    MN_OPERATION_ID_T                   OpId,
    TAF_PH_ICC_TYPE                     IccType
)
{
    if ( (TAF_PH_ICC_UNKNOW == IccType)
      || (IccType > TAF_PH_ICC_USIM))
    {
        return TAF_FAILURE;
    }

    return MN_FillAndSndAppReqMsg( ClientId,
                             OpId,
                             TAF_MSG_MMA_CPNN_INFO,
                             &IccType,
                             sizeof(IccType),
                             I0_WUEPS_PID_MMA);
}


VOS_UINT32 Taf_SetEOPlmnHandle(
    MN_CLIENT_ID_T                      ClientId,
    MN_OPERATION_ID_T                   OpId,
    TAF_MMA_SET_EOPLMN_LIST_STRU       *pstEOPlmnCfg
)
{

    return MN_FillAndSndAppReqMsg(ClientId,
                                  OpId,
                                  TAF_MSG_MMA_EOPLMN_SET_REQ,
                                  pstEOPlmnCfg,
                                  sizeof(TAF_MMA_SET_EOPLMN_LIST_STRU),
                                  I0_WUEPS_PID_MMA);
}

#ifdef  __cplusplus
 #if  __cplusplus
}
 #endif
#endif

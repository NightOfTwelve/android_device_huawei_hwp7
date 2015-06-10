/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_dpm.h
  �� �� ��   : ����
  ��������   : 2013��2��2��
  ����޸�   :
  ��������   : drv_dpm.h ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��2��2��
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __DRV_DPM_H__
#define __DRV_DPM_H__
#include "drv_global.h"
#if (defined(FEATURE_E5) &&  defined(FEATURE_HILINK) && ((FEATURE_E5 == FEATURE_ON) || (FEATURE_HILINK == FEATURE_ON)))

/************************************************************************
 * FUNCTION
 *       pwrctrl_wifi_lowppwer_enter
 * DESCRIPTION
 *       pwrctrl_wifi_lowppwer_enter
 * INPUTS
 *       NONE
 * OUTPUTS
 *       NONE
 *************************************************************************/
void pwrctrl_wifi_lowpower_enter(void);



/************************************************************************
 * FUNCTION
 *       pwrctrl_wifi_lowppwer_exit
 * DESCRIPTION
 *       pwrctrl_wifi_lowppwer_exit
 * INPUTS
 *       NONE
 * OUTPUTS
 *       NONE
 *************************************************************************/
void pwrctrl_wifi_lowpower_exit(void);

/************************************************************************
 * FUNCTION
 *       pwrctrl_wifi_nolink_timer_set
 * DESCRIPTION
 *       pwrctrl_wifi_nolink_timer_set
 * INPUTS
 *       NONE
 * OUTPUTS
 *       NONE
 *************************************************************************/
int pwrctrl_wifi_nolink_timer_set(unsigned int time);

/************************************************************************
 * FUNCTION
 *       pwrctrl_usb_lowpower_enter
 * DESCRIPTION
 *       pwrctrl_usb_lowpower_enter
 * INPUTS
 *       NONE
 * OUTPUTS
 *       NONE
 *************************************************************************/
void pwrctrl_usb_lowpower_enter(void);


/************************************************************************
 * FUNCTION
 *       pwrctrl_usb_lowpower_exit
 * DESCRIPTION
 *       pwrctrl_usb_lowpower_exit
 * INPUTS
 *       NONE
 * OUTPUTS
 *       NONE
 *************************************************************************/
void pwrctrl_usb_lowpower_exit(void);

#endif
/*****************************************************************************
 �� �� ��  : BSP_ONOFF_DrvPowerOff
 ��������  : ����ֱ���µ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
extern void DRV_POWER_OFF(void);


/*****************************************************************************
 �� �� ��  : pwrctrl_wpa_pwr_up
 ��������  : RF�µ�
 �������  : None
 �������  : None
 �� �� ֵ  : None
*****************************************************************************/
extern void pwrctrl_wpa_pwr_up(void);
#define DRV_WPA_POWERUP()


#define PWRCTRL_GET_SUCCESS         (0x00)
#define PWRCTRL_GET_PARAINVALID     (0x01)
#define PWRCTRL_ONORLOCK            (0x10)
#define PWRCTRL_OFFORUNLOCK         (0x20)

typedef unsigned long (*pULULFUNCPTR)( unsigned long ulPara );

typedef struct
{
    pULULFUNCPTR pRestoreFunction;
    pULULFUNCPTR pEnableFunction;
    pULULFUNCPTR pDisableFunction;
    pFUNCPTR2    pDfsSetFunc;
    pULULFUNCPTR pDfsReleaseFunc;
}DPM_SLEEP_CALLBACK_STRU;

/*wangwei dfs*/
typedef  enum
{
    PWRCTRL_DFS_USB = 0,
    PWRCTRL_DFS_SD  = 1,
    PWRCTRL_DFS_PS1 = 2,
    PWRCTRL_DFS_HIFI = 3,  /*hifi*/
    PWRCTRL_DFS_BUTT
} PWC_DFS_ID_E;

typedef enum
{
    DFS_PROFILE_0 = 0,            /*Ƶ�ʼ���1*/
    DFS_PROFILE_1 = 1,            /*Ƶ�ʼ���2*/
    DFS_PROFILE_2 = 2,            /*Ƶ�ʼ���3*/
    DFS_PROFILE_3 = 3,            /*Ƶ�ʼ���4*/
    DFS_PROFILE_4 = 4,            /*Ƶ�ʼ���5*/
    DFS_PROFILE_BUTT
} PWC_DFS_PROFILE_E;


typedef enum tagPWC_COMM_MODE_E
{
    PWC_COMM_MODE_GSM=0,
    PWC_COMM_MODE_WCDMA=1,
    PWC_COMM_MODE_LTE=2,
    PWC_COMM_MODE_TDS=3,
    PWC_COMM_MODE_BUTT
}PWC_COMM_MODE_E;

typedef enum tagPWC_COMM_MODULE_E
{
    PWC_COMM_MODULE_PA,
    PWC_COMM_MODULE_RF,
    PWC_COMM_MODULE_ABB,
    PWC_COMM_MODULE_BBE16,

    PWC_COMM_MODULE_BBP_DRX,  /*W:1A,2A,2C,2D,3*/
    PWC_COMM_MODULE_BBP_DPA,  /*W:1B*/
    PWC_COMM_MODULE_BBP_MEM,  /*W:2B*/

    PWC_COMM_MODULE_BUTT
}PWC_COMM_MODULE_E;


typedef enum tagPWC_COMM_STATUS_E
{
    PWRCTRL_COMM_ON = 0x10,
    PWRCTRL_COMM_OFF = 0x20,
    PWC_COMM_STATUS_BUTT
}PWC_COMM_STATUS_E;

typedef BSP_S32 (*PWC_TCXO_FUNCPTR)(int);

/* ˯��ͶƱID */
typedef enum tagPWC_CLIENT_ID_E
{
    /*0x100*/
    PWRCTRL_SLEEP_SCI      = 0x100, /*drv*/
    PWRCTRL_SLEEP_SCI1,             /*drv*/
    PWRCTRL_SLEEP_DMA,              /*drv*/
    PWRCTRL_SLEEP_MEM,              /*drv,flash or emmc*/

    PWRCTRL_SLEEP_PS_G0,            /*oam*/
    PWRCTRL_SLEEP_PS_W0,            /*oam*/

    PWRCTRL_SLEEP_PS_G1,            /*oam*/
    PWRCTRL_SLEEP_PS_W1,            /*oam*/

    PWRCTRL_SLEEP_RNIC,             /*nas*/
    PWRCTRL_SLEEP_FTM,              /*nas*/
    PWRCTRL_SLEEP_FTM_1,            /*nas*/
    PWRCTRL_SLEEP_NAS_1,            /*nas*/
    PWRCTRL_SLEEP_NAS,              /*nas*/

    PWRCTRL_SLEEP_TLPS,             /*V7 TDS��LTE RTT����*/
    PWRCTRL_SLEEP_OAM,              /*V7 MSP*/

    PWRCTRL_SLEEP_SD,               /*v3r3 a core*/
    PWRCTRL_SLEEP_BOOT,             /*v3r3 a core*/
    PWRCTRL_SLEEP_ACPU,             /*v3r3 c core*/
    PWRCTRL_SLEEP_HSIC,             /*v3r3 a core*/

    PWRCTRL_LIGHTSLEEP_WIFI,        /* HSIC���ܵ��� */
    PWRCTRL_SLEEP_WIFI,             /*v3r3 a core*/
    PWRCTRL_SLEEP_USB,              /*v3r3 a core*/
    PWRCTRL_SLEEP_LCD,              /*v3r3 a core*/
    PWRCTRL_SLEEP_KEY,              /*v3r3 c&a core*/
    PWRCTRL_SLEEP_ACM,
    PWRCTRL_SLEEP_ICC,              /*v3r3 c&a core*/
    PWRCTRL_SLEEP_IFC,
    PWRCTRL_SLEEP_CHG,              /*v3r3 c core*/

    PWRCTRL_SLEEP_DSFLOW,           /*nas*/
    PWRCTRL_SLEEP_CDMAUART,         /*drv cdma uart ����*/

    /* Test */
    PWRCTRL_TEST_DEEPSLEEP  = 0x11F,

    PWRCTRL_CLIENT_BUTT     = 0x120
} PWC_CLIENT_ID_E;


/*
 *  DFS Device QoS
 */
typedef enum tagDFS_QOS_ID_E
{
    DFS_QOS_ID_ACPU_MINFREQ_E = 0,  /*(0)*/
    DFS_QOS_ID_ACPU_MAXFREQ_E,      /*(1)*/
    DFS_QOS_ID_CCPU_MINFREQ_E,      /*(2)*/
    DFS_QOS_ID_CCPU_MAXFREQ_E,      /*(3)*/
    DFS_QOS_ID_DDR_MINFREQ_E,       /*(4)*/
    DFS_QOS_ID_DDR_MAXFREQ_E,       /*(5)*/
    DFS_QOS_ID_GPU_MINFREQ_E,       /*(6)*/
    DFS_QOS_ID_GPU_MAXFREQ_E,       /*(7)*/
    DFS_QOS_ID_BUS_MINFREQ_E,       /*(8)*/
    DFS_QOS_ID_BUS_MAXFREQ_E        /*(9)*/
}DFS_QOS_ID_E;


typedef struct
{
    PWRCTRLFUNCPTRVOID  pFuncDsIn;
    PWRCTRLFUNCPTRVOID  pFuncDsOut;
}PWC_DS_SOCP_CB_STRU;

typedef enum tagPWC_TEMP_PROTECT_E
{
    PWC_TEMP_OLED,
    PWC_TEMP_BATTERY,
	PWC_TEMP_L_PA,
    PWC_TEMP_G_PA,
    PWC_TEMP_W_PA,
    PWC_TEMP_SIM_CARD,
    PWC_TEMP_DCXO,
    PWC_TEMP_BUTT
}PWC_TEMP_PROTECT_E;

typedef struct
{
    PWRCTRLFUNCPTR drx_slow;
} PWC_SLEEP_CALLBACK_STRU;

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_DEEPSLEEP_FOREVER
 ��������  : AARM CARM �µ�ӿ�
 �������  : None
 �������  : None
 �� �� ֵ  : None

*****************************************************************************/
extern void DRV_PWRCTRL_DEEPSLEEP_FOREVER(void );

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_SLEEP_CALLBACK_REG
 ��������  : ע��PWC����������ʹ��
 �������  : ��ע��ĺ���
 �������  : None
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
*****************************************************************************/
extern int DRV_PWRCTRL_SLEEP_CALLBACK_REG(PWC_SLEEP_CALLBACK_STRU pCallback);

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_GET_TEMP
 ��������  : ��ȡOLED����ء�GPA��WPA��SIM����DCXO���¶ȡ�
 �������  : enModule:PWC_TEMP_PROTECT_E
             hkAdcTable:��ѹ�¶Ȼ����
 �������  : pslData:�洢�ĵ�ѹת��Ϊ���¶�ֵ
 �� �� ֵ  : HKADC_OK/HKADC_ERROR ��
*****************************************************************************/
/*V3R2 CS f00164371������ӿ���ʱ�����룬��ΪSFTƽ̨�Ƿ���0��ʵ�ʻ�Ƭ��Ҫʵ�֣�
��Ҫʹ��CS_SFT��������֣����ǲ�ͬ�����CS_SFT�겻һ��ͳһ�����Բ���ֱ�����ε���
�����ڽӿڴ����ڲ����и���*/
extern BSP_S32 DRV_PWRCTRL_GET_TEMP(PWC_TEMP_PROTECT_E enModule, BSP_U16 *hkAdcTable,BSP_S32 *pslData);

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_AHB_BAK_ADDR_ALLOC
 ��������  : arm���µ繦�ܣ���Ҫ����ahb���ݣ��ýӿ��������ݱ����ַ�ͳ���
 �������  : length     ��������ռ��С��ָ��
 �������  : ��
 �� �� ֵ  : ����ռ�ĵ�ַ ��
*****************************************************************************/
/*V3R2 CS f00164371������ӿ���ʱ������*/
extern void * DRV_PWRCTRL_AHB_BAK_ADDR_ALLOC(unsigned int * length);

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_GetCcpuLoadCB
 ��������  : arm �ṩ��TTF�Ļص�����
 �������  : pFunc:TTF����ָ��
 �������  : ��
 �� �� ֵ  : ����ռ�ĵ�ַ ��
*****************************************************************************/
extern void DRV_PWRCTRL_GetCcpuLoadCB(PWRCTRLFUNCPTR pFunc );

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_GetCpuLoad
 ��������  : arm cpuռ���ʲ�ѯ����
 �������  : ��
 �������  : ulAcpuLoad:Acpuռ���ʵ�ַ.
             ulCcpuLoad:Ccpuռ���ʵ�ַ.
 �� �� ֵ  : 0/1 ��
*****************************************************************************/
extern unsigned int DRV_PWRCTRL_GetCpuLoad(unsigned int *ulAcpuLoad,unsigned int *ulCcpuLoad);

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_PWRUP
 ��������  : ͨ��ģ����Ƶ�Դ�ϵ�ӿ�
 �������  : enModeId     ͨ��ģʽ
             enModuleId   ͨ��ģ��
             enModemId    ����
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��
*****************************************************************************/
extern BSP_S32 DRV_PWRCTRL_PWRUP (PWC_COMM_MODE_E enModeId, PWC_COMM_MODULE_E enModuleId, PWC_COMM_MODEM_E enModemId);

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_PWRDOWN
 ��������  : ͨ��ģ����Ƶ�Դ�µ�ӿ�
 �������  : enModeId     ͨ��ģʽ
             enModuleId   ͨ��ģ��
             enModemId    ����
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��
*****************************************************************************/
extern BSP_S32 DRV_PWRCTRL_PWRDOWN (PWC_COMM_MODE_E enModeId, PWC_COMM_MODULE_E enModuleId, PWC_COMM_MODEM_E enModemId);

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_PWRSTATUSGET
 ��������  : ͨ��ģ����Ƶ�Դ״̬��ѯ�ӿ�
 �������  : enModeId     ͨ��ģʽ
             enModuleId   ͨ��ģ��
             enModemId    ����
 �������  : ��
 �� �� ֵ  : PWRCTRL_COMM_ON         �ϵ�
             PWRCTRL_COMM_OFF        �µ�
             RET_ERR                 ״̬��ѯʧ��
*****************************************************************************/
extern  BSP_S32 DRV_PWRCTRL_PWRSTATUSGET(PWC_COMM_MODE_E enModeId, PWC_COMM_MODULE_E enModuleId, PWC_COMM_MODEM_E enModemId);

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_PLLENABLE
 ��������  : ͨ��ģ�����ʱ��ʹ�ܽӿ�
 �������  : enModeId     ͨ��ģʽ
             enModuleId   ͨ��ģ��
             enModemId    ����
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��
*****************************************************************************/
extern BSP_S32 DRV_PWRCTRL_PLLENABLE (PWC_COMM_MODE_E enModeId, PWC_COMM_MODULE_E enModuleId, PWC_COMM_MODEM_E enModemId);

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_PLLDISABLE
 ��������  : ͨ��ģ�����ʱ�ӹرսӿ�
 �������  : enModeId     ͨ��ģʽ
             enModuleId   ͨ��ģ��
             enModemId    ����
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��
*****************************************************************************/
extern BSP_S32 DRV_PWRCTRL_PLLDISABLE (PWC_COMM_MODE_E enModeId, PWC_COMM_MODULE_E enModuleId, PWC_COMM_MODEM_E enModemId);

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_PLLSTATUSGET
 ��������  : ͨ��ģ�����ʱ��״̬��ѯ�ӿ�
 �������  : enCommMode     ͨ��ģʽ
             enCommModule   ͨ��ģ��
             enCommCard     ����
 �������  : ��
 �� �� ֵ  : PWRCTRL_COMM_ON         ʱ�Ӵ�
             PWRCTRL_COMM_OFF        ʱ�ӹر�
             RET_ERR                 ��ѯʧ��
*****************************************************************************/
extern BSP_S32 DRV_PWRCTRL_PLLSTATUSGET (PWC_COMM_MODE_E enModeId, PWC_COMM_MODULE_E enModuleId, PWC_COMM_MODEM_E enModemId);

/*****************************************************************************
* �� �� ��  : DRV_PWRCTRL_DSP_RESET
* ��������  : Lģ��DSP��λ�ӿ�
* �������  :
* �������  :
* �� �� ֵ  :
*****************************************************************************/
extern BSP_VOID DRV_PWRCTRL_DSP_RESET(BSP_VOID);


/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_SLEEPVOTE_LOCK
 ��������  : �����ֹ˯��ͶƱ�ӿڡ�
 �������  : enClientId:PWC_CLIENT_ID_E
 �������  : None
 �� �� ֵ  : PWC_PARA_INVALID/PWC_SUCCESS
*****************************************************************************/
extern BSP_U32 DRV_PWRCTRL_SLEEPVOTE_LOCK(PWC_CLIENT_ID_E enClientId);

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_SLEEPVOTE_UNLOCK
 ��������  : ��������˯��ͶƱ�ӿڡ�
 �������  : enClientId:PWC_CLIENT_ID_E
 �������  : None
 �� �� ֵ  : PWC_PARA_INVALID/PWC_SUCCESS
*****************************************************************************/
extern BSP_U32 DRV_PWRCTRL_SLEEPVOTE_UNLOCK(PWC_CLIENT_ID_E enClientId);

/*****************************************************************************
Function:   DRV_PWRCTRL_DFS_SET_PRF_CCPU
Description:Set the System Min Profile
Input:      ulClientID: The Vote Module Client
            enProfile:  The Min Profile Value
Output:     None
Return:     The Vote Result
Others:
*****************************************************************************/
extern BSP_U32  DRV_PWRCTRL_DFS_SET_PRF_CCPU(PWC_DFS_ID_E ulClientID, PWC_DFS_PROFILE_E enProfile);

/*****************************************************************************
Function:   DRV_PWRCTRL_DFS_RLS_PRF_CCPU
Description:Release the Vote Result
Input:      ulClientID: The Vote Module Client
Output:     None;
Return:     The Vote Result
Others:
*****************************************************************************/
extern BSP_U32 DRV_PWRCTRL_DFS_RLS_PRF_CCPU(PWC_DFS_ID_E ulClientID);

/*****************************************************************************
Function:   DRV_PWRCTRL_SLEEP_IN_CB
Description:
Input:
Output:     None;
Return:
Others:
*****************************************************************************/
extern int DRV_PWRCTRL_SLEEP_IN_CB(PWC_DS_SOCP_CB_STRU stFunc);

/*****************************************************************************
 �� �� ��  : BSP_PWRCTRL_UsbLowPowerEnter
 ��������  : USB����͹��Ľӿ�
 �������  : None
 �������  : None
 �� �� ֵ  : None
*****************************************************************************/
extern void DRV_PWRCTRL_USB_LOWPOWER_ENTER(void);

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_USB_LOWPOWER_EXIT
 ��������  : USB�˳��͹��Ľӿ�
 �������  : None
 �������  : None
 �� �� ֵ  : None
*****************************************************************************/
extern void DRV_PWRCTRL_USB_LOWPOWER_EXIT(void);


/*****************************************************************************
 �� �� ��  : BSP_32K_GetTick
 ��������  : 32Kʱ�Ӷ�ӦTick��ѯ�ӿ�
 �������  : None
 �������  : None
 �� �� ֵ  : 32Kʱ�Ӷ�ӦTickֵ
*****************************************************************************/
extern BSP_U32 BSP_32K_GetTick( BSP_VOID );


/*****************************************************************************
Function:   BSP_PWC_GetMeansFlag
Description:
Input:
Output:     the means flag value;
Return:
Others:
*****************************************************************************/
extern BSP_S32 BSP_PWC_GetMeansFlag(PWC_COMM_MODE_E enCommMode);
#define DRV_PWC_GET_MEANSFLAG(enCommMode) BSP_PWC_GetMeansFlag(enCommMode)

extern BSP_VOID BSP_PWC_MspVoteRegister(FUNCPTR routine);
extern BSP_VOID BSP_PWC_SocpVoteRegister(FUNCPTR routine);

/*****************************************************************************
 Function   : BSP_PWC_SetTimer4WakeSrc
 Description: ����timer4��Ϊ����Դ
 Input      :
 Return     : void
 Other      :
*****************************************************************************/
extern VOID BSP_PWC_SetTimer4WakeSrc(VOID);

/*****************************************************************************
 Function   : BSP_PWC_DelTimer4WakeSrc
 Description: ����timer4����Ϊ����Դ
 Input      :
            :
 Return     : void
 Other      :
*****************************************************************************/
extern VOID BSP_PWC_DelTimer4WakeSrc(VOID);


/*****************************************************************************
 �� �� ��  : BSP_PWRCTRL_StandbyStateCcpu/BSP_PWRCTRL_StandbyStateAcpu
 ��������  : AT^PSTANDBY
 �������  :
 �������  :
 ����ֵ��
*****************************************************************************/
extern unsigned int BSP_PWRCTRL_StandbyStateAcpu(unsigned int ulStandbyTime, unsigned int ulSwitchTime);
extern unsigned int BSP_PWRCTRL_StandbyStateCcpu(unsigned int ulStandbyTime, unsigned int ulSwitchTime);
#define DRV_PWRCTRL_STANDBYSTATEACPU(ulStandbyTime, ulSwitchTime)	BSP_PWRCTRL_StandbyStateAcpu(ulStandbyTime, ulSwitchTime)
#define DRV_PWRCTRL_STANDBYSTATECCPU(ulStandbyTime, ulSwitchTime)	BSP_PWRCTRL_StandbyStateCcpu(ulStandbyTime, ulSwitchTime)

/***************************************************************************
 �� �� ��  : BSP_TCXO_RfclkStatus
 ��������  : ��ȡRF CLK״̬
 �������  :
		enModemId  Modem ID
 �������  : ��
 �� �� ֵ  : PWRCTRL_COMM_ON         ʹ��;
             PWRCTRL_COMM_OFF        ��ֹ;
             RET_ERR                 �����쳣
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��6��24��
    �޸�����   : �����ɺ���
**************************************************************************/
extern int BSP_TCXO_RfclkStatus(PWC_COMM_MODEM_E enModemId);
#define DRV_TCXO_RFCLKSTATUS(enModemId) BSP_TCXO_RfclkStatus(enModemId)
/*****************************************************************************
 �� �� ��  : DRV_TCXO_RfclkEnable
 ��������  : ͨ��ģ�����RF 19.2Mʱ��ʹ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��6��24��
    �޸�����   : �����ɺ���

*****************************************************************************/
extern int BSP_TCXO_RfclkEnable(PWC_COMM_MODEM_E  enModemId, PWC_COMM_MODE_E enModeType);
#define DRV_TCXO_RFCLKENABLE(enModemId, enModeType) BSP_TCXO_RfclkEnable(enModemId, enModeType)
/*****************************************************************************
 �� �� ��  : BSP_TCXO_RfclkDisable
 ��������  : ͨ��ģ�����RF 19.2Mʱ��ȥʹ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��6��24��
    �޸�����   : �����ɺ���

*****************************************************************************/
extern int BSP_TCXO_RfclkDisable(PWC_COMM_MODEM_E  enModemId, PWC_COMM_MODE_E enModeType);
#define DRV_TCXO_RFCLKDISABLE(enModemId, enModeType) BSP_TCXO_RfclkDisable(enModemId, enModeType)
/***************************************************************************
 �� �� ��  : DRV_TCXO_GETSTATUS
 ��������  : ��ȡTCXO״̬
 �������  :
		enModemId  Modem ID
 �������  : ��
 �� �� ֵ  : PWRCTRL_COMM_ON         ʹ��;
             PWRCTRL_COMM_OFF        ��ֹ;
             RET_ERR                 �����쳣
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��4��19��
    �޸�����   : �����ɺ���
**************************************************************************/
extern int BSP_TCXO_GetStatus(PWC_COMM_MODEM_E enModemId);
#define DRV_TCXO_GETSTATUS(enModemId)   BSP_TCXO_GetStatus(enModemId)
/***************************************************************************
 �� �� ��  : DRV_TCXO_ENABLE
 ��������  : ʹ��TCXO
 �������  : enModemId  Modem ID
	         enModeType    Mode type
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��4��19��
    �޸�����   : �����ɺ���
**************************************************************************/
extern int BSP_TCXO_Enable(PWC_COMM_MODEM_E  enModemId, PWC_COMM_MODE_E enModeType);
#define DRV_TCXO_ENABLE(enModemId, enModeType)  BSP_TCXO_Enable(enModemId, enModeType)
/***************************************************************************
 �� �� ��  : DRV_TCXO_DISABLE
 ��������  : �ر�TCXO
 �������  : enModemId  Modem ID
	         enModeType    Mode type
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��
	         RET_ERR_NOT_FOUND û��ͶƱʹ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��4��19��
    �޸�����   : �����ɺ���
**************************************************************************/
extern int BSP_TCXO_Disable(PWC_COMM_MODEM_E  enModemId, PWC_COMM_MODE_E enModeType);
#define DRV_TCXO_DISABLE(enModemId, enModeType) BSP_TCXO_Disable(enModemId, enModeType)
/***************************************************************************
 �� �� ��  : DRV_TCXO_TIMER_START
 ��������  : TCXO ��ʱ������
 �������  : enModemId  Modem ID
    	     enModeType    Mode type
    	     routine:�ص�����  ע:�ص��������ж���������ִ��
             arg:�ص�����
             timerValue:����ֵ ��λms
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��4��19��
    �޸�����   : �����ɺ���
**************************************************************************/
extern int BSP_TCXO_Timer_Start(PWC_COMM_MODEM_E  enModemId, PWC_COMM_MODE_E enModeType, PWC_TCXO_FUNCPTR routine, int arg, unsigned int timerValue);
#define DRV_TCXO_TIMER_START(enModemId, enModeType, routine, arg, timerValue)   BSP_TCXO_Timer_Start(enModemId, enModeType, routine, arg, timerValue)

/***************************************************************************
 �� �� ��  : PWRCTRL_DfsQosRequest
 ��������  : ��ƵͶƱ����
 �������  : qos_id  ͶƱģ�� ID
    	     req_value   Ƶ��ֵ
    	     req_id: ���󵽵�ID
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��4��19��
    �޸�����   : �����ɺ���
**************************************************************************/
extern BSP_S32 PWRCTRL_DfsQosRequest(BSP_S32 qos_id, BSP_U32 req_value, BSP_S32* req_id);
/***************************************************************************
 �� �� ��  : PWRCTRL_DfsQosRelease
 ��������  : ��ƵͶƱ�ͷ�
 �������  : qos_id  ͶƱģ�� ID
    	     req_id: ���ͷŵ�ID
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��4��19��
    �޸�����   : �����ɺ���
**************************************************************************/
extern BSP_S32 PWRCTRL_DfsQosRelease(BSP_S32 qos_id, BSP_S32* req_id);
/***************************************************************************
 �� �� ��  : PWRCTRL_DfsQosUpdate
 ��������  : ��ƵͶƱֵ����
 �������  : qos_id  ͶƱģ�� ID
    	     req_id: ���ͷŵ�ID
    	     req_value:�����µ�ֵ
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��4��19��
    �޸�����   : �����ɺ���
**************************************************************************/
extern BSP_S32 PWRCTRL_DfsQosUpdate(BSP_S32 qos_id, BSP_S32 req_id, BSP_U32 req_value);

/*****************************************************************************
Function:   BSP_DSPTcmStore
Description:    BBE16 TCM���ݣ���TL�͹��ĵ���
Input:
Output:
Return:
Others:
*****************************************************************************/
extern BSP_S32 BSP_DSPTcmStore(BSP_VOID);
#define DRV_EDMA_DSP_MemStore() BSP_DSPTcmStore()

/*****************************************************************************
Function:   BSP_DSPTcmRestore
Description:    BBE16 TCM�ָ�����TL�͹��ĵ���
Input:
Output:
Return:
Others:
*****************************************************************************/
extern BSP_S32 BSP_DSPTcmRestore(BSP_VOID);
#define DRV_EDMA_DSP_MemRestore() BSP_DSPTcmRestore()

/*****************************************************************************
Function:   BSP_GetDspTaskState
Description:    BBE16 TCM���ݻָ�״̬��ѯ����TL�͹��ĵ���
Input:
Output:
Return:
Others:
*****************************************************************************/
extern BSP_S32 BSP_GetDspTaskState(BSP_VOID);
#define DRV_EDMA_DSP_StateGet() BSP_GetDspTaskState()

/*****************************************************************************
Function:   BSP_StopDsp
Description:    BBE16ֹͣ����
Input:
Output:
Return:
Others:
*****************************************************************************/
extern BSP_VOID BSP_StopDsp(BSP_VOID);
#define DRV_PWRCTRL_DSP_HOLD() BSP_StopDsp()

/*****************************************************************************
Function:   BSP_RunDsp
Description:    BBE16��ʼ����
Input:
Output:
Return:
Others:
*****************************************************************************/
extern BSP_VOID BSP_RunDsp(BSP_VOID);
#define DRV_PWRCTRL_DSP_RUN() BSP_RunDsp()

/*****************************************************************************
* �� �� ��  : BSP_ResetDsp
* ��������  : Lģ��DSP��λ�ӿ�
* �������  :
* �������  :
* �� �� ֵ  :
*****************************************************************************/
extern BSP_VOID BSP_ResetDsp(void);
#define DRV_PWRCTRL_DSP_RESET() BSP_ResetDsp()

/*****************************************************************************
* �� �� ��  : BSP_UnresetDsp
* ��������  : Lģ��DSP�⸴λ�ӿ�
* �������  :
* �������  :
* �� �� ֵ  :
*****************************************************************************/
extern BSP_VOID BSP_UnresetDsp(void);
#define DRV_PWRCTRL_DSP_UNRESET() BSP_UnresetDsp()

/*****************************************************************************
* �� �� ��  : BSP_DspClkEnable
* ��������  : Lģ��DSPʱ��ʹ�ܽӿ�
* �������  :
* �������  :
* �� �� ֵ  :
*****************************************************************************/
extern BSP_VOID BSP_DspClkEnable(void);
#define DRV_PWRCTRL_DSP_CLKENABLE() BSP_DspClkEnable()

/*****************************************************************************
* �� �� ��  : BSP_DspClkDisable
* ��������  : Lģ��DSPʱ��ȥʹ�ܽӿ�
* �������  :
* �������  :
* �� �� ֵ  :
*****************************************************************************/
extern BSP_VOID BSP_DspClkDisable(void);
#define DRV_PWRCTRL_DSP_CLKDISABLE() BSP_DspClkDisable()

/*****************************************************************************
Function:   BSP_BBPIntWakeEnable
Description:    enable bbp wakeup int
Input:  PWC_COMM_MODE_E mode
Output:
Return:
Others:
*****************************************************************************/
extern BSP_VOID BSP_BBPIntWakeEnable(PWC_COMM_MODE_E mode);
#define DRV_BBPWAKE_INT_ENABLE(mode) BSP_BBPIntWakeEnable(mode) 

/*****************************************************************************
Function:   BSP_BBPIntWakeDisable
Description:    disable bbp wakeup int
Input:  PWC_COMM_MODE_E mode
Output:
Return:
Others:
*****************************************************************************/
extern BSP_VOID BSP_BBPIntWakeDisable(PWC_COMM_MODE_E mode);
#define DRV_BBPWAKE_INT_DISABLE(mode) BSP_BBPIntWakeDisable(mode) 

/*****************************************************************************
Function:   BSP_BBPIntWakeClear
Description:    clear bbp wakeup int
Input:  PWC_COMM_MODE_E mode
Output:
Return:
Others:
*****************************************************************************/
extern BSP_VOID BSP_BBPIntWakeClear(PWC_COMM_MODE_E mode);
#define DRV_BBPWAKE_INT_CLEAR(mode) BSP_BBPIntWakeClear(mode) 

/*****************************************************************************
* �� �� ��  : PWRCTRL_GetSleepSlice
* ��������  :
* �������  :
* �������  :
* �� �� ֵ  :
*****************************************************************************/
extern BSP_U32 PWRCTRL_GetSleepSlice(void);

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_BBPCLKENABLE
 ��������  : ����ͨ��ģ��ʱ��
 �������  : enModeId     ͨ��ģʽ
             enModuleId   ͨ��ģ��
             enModemId    ����
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��
*****************************************************************************/
extern BSP_S32 DRV_PWRCTRL_BBPCLKENABLE(PWC_COMM_MODE_E enModeId, PWC_COMM_MODULE_E enModuleId, PWC_COMM_MODEM_E enModemId);

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_BBPCLKDISABLE
 ��������  : �ر�ͨ��ģ��ʱ��
 �������  : enModeId     ͨ��ģʽ
             enModuleId   ͨ��ģ��
             enModemId    ����
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��
*****************************************************************************/
extern BSP_S32 DRV_PWRCTRL_BBPCLKDISABLE(PWC_COMM_MODE_E enModeId, PWC_COMM_MODULE_E enModuleId, PWC_COMM_MODEM_E enModemId);


#endif


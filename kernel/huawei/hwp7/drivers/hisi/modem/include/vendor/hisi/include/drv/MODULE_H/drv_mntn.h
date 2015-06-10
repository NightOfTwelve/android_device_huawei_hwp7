/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_mntn.h
  �� �� ��   : ����
  ��������   : 2013��2��2��
  ����޸�   :
  ��������   : drv_mntn.h ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��2��2��
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __DRV_MNTN_H__
#define __DRV_MNTN_H__
#include "drv_global.h"



typedef enum DRV_SHUTDOWN_REASON_tag_s
{
    DRV_SHUTDOWN_LOW_BATTERY,           /* ��ص�����                 */
    DRV_SHUTDOWN_BATTERY_ERROR,         /* ����쳣                   */
    DRV_SHUTDOWN_POWER_KEY,             /* ���� Power ���ػ�          */
    DRV_SHUTDOWN_TEMPERATURE_PROTECT,   /* ���±����ػ�               */
    DRV_SHUTDOWN_LOW_TEMP_PROTECT,
    DRV_SHUTDOWN_RESET,                 /* ϵͳ��λ                 */
    DRV_SHUTDOWN_CHARGE_REMOVE,         /* �ػ����ģʽ�£��γ������ */
    DRV_SHUTDOWN_UPDATE,                /* �ػ�����������ģʽ         */
    DRV_SHUTDOWN_BUTT
}DRV_SHUTDOWN_REASON_ENUM;


#if (defined(FEATURE_E5) &&  defined(FEATURE_HILINK) && ((FEATURE_E5 == FEATURE_ON) || (FEATURE_HILINK == FEATURE_ON)))

/************************************************************************************
 *Function Name :  drvShutdown
 *Description   :  �ر�ϵͳ(������Ϣ��֪�ػ�ģ������̷��أ�ʵ�ʹػ��ɹػ�ģ�鴦��)
 *Others        :  No
************************************************************************************/
void drvShutdown( DRV_SHUTDOWN_REASON_ENUM eReason );


/************************************************************************************
 *Function Name :  drvPowerOff
 *Description   :  ֱ���µ�
 *Others        :  No
************************************************************************************/
void drvPowerOff( void );

/************************************************************************************
 *Function Name :  drvReboot(
 *Description   :  �ȸ�λ(ͨ��ϵͳ�������ĸ�λʵ�֣�������)
 *Others        :  No
************************************************************************************/
void drvReboot( void );

#endif
typedef int  (*pUartRecv)(unsigned char uPortNo, unsigned char* pData, unsigned short uslength);

#define PROCESS_ENTER(pADDRESS)  do { } while (0)
#define PROCESS_EXIT(pADDRESS)   do { } while (0)

typedef enum{
     ADDRTYPE8BIT,
     ADDRTYPE16BIT,
     ADDRTYPE32BIT
}ENADDRTYPE;

typedef enum{
     BOARD_TYPE_LTE_ONLY    = 0,
     BOARD_TYPE_GUL
}BOARD_TYPE_E;

typedef enum{
     BSP_MODEM_CORE         = 0,
     BSP_APP_CORE
}BSP_CORE_TYPE_E;

typedef enum{
     PV500_CHIP             = 0,
     V7R1_CHIP              = 1,
     PV500_PILOT_CHIP,
     V7R1_PILOT_CHIP,
     PV500_V9R1ES_CHIP
}BSP_CHIP_TYPE_E;

/******************************************************************************
* Function     :   BSP_OM_RegRead
*
* Description  :
*
* Input        :
*
* Output       :   ��
*
* return       :
******************************************************************************/
BSP_S32 BSP_OM_RegRead(BSP_U32 u32RegAddr, ENADDRTYPE enAddrType, BSP_U32 *pu32Value);


/******************************************************************************
* Function     :   BSP_OM_RegWrite
*
* Description  :
*
* Input        :
*
* Output       :   ��
*
* return       :
******************************************************************************/
BSP_S32 BSP_OM_RegWrite(BSP_U32 u32RegAddr, ENADDRTYPE enAddrType, BSP_U32 u32Value);

/******************************************************************************
* Function     :   BSP_OM_SoftReboot
*
* Description  :
*
* Input        :
*
* Output       :   ��
*
* return       :
******************************************************************************/
BSP_VOID BSP_OM_SoftReboot(void);

/*****************************************************************************
* �� �� ��  : BSP_OM_GetBoardType
*
* ��������  : ��ȡ��������
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : BSP��������ö��
*
* ����˵��  : ��
*
*****************************************************************************/
BOARD_TYPE_E BSP_OM_GetBoardType(BSP_VOID);

/*****************************************************************************
* �� �� ��  : BSP_GetHostCore
*
* ��������  : ��ѯ��ǰCPU���Ӻ�����
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : CPU���Ӻ�����
*
* ����˵��  : ��
*
*****************************************************************************/
BSP_CORE_TYPE_E BSP_GetHostCore(BSP_VOID);

/*****************************************************************************
* �� �� ��  : BSP_OM_GetChipType
*
* ��������  : ��ȡоƬ����
*
* �������  : BSP_VOID
*
* �������  : ��
*
* �� �� ֵ  : оƬ����
*             PV500_CHIP:PV500оƬ
*             V7R1_CHIP: V7R1оƬ
*
* ����˵��  : ��
*
*****************************************************************************/
BSP_CHIP_TYPE_E BSP_OM_GetChipType(BSP_VOID);

typedef struct tagBSP_OM_NET_S
{
    BSP_U32 u32NetRxStatOverFlow;       /* ����FIFO���ͳ�Ƽ��� */
    BSP_U32 u32NetRxStatPktErr;         /* �����ܴ������ */
    BSP_U32 u32NetRxStatCrcErr;         /* ����CRC������� */
    BSP_U32 u32NetRxStatLenErr;         /* ������Ч���Ȱ����� */
    BSP_U32 u32NetRxNoBufInt;           /* ����û��BUFFER�жϼ��� */
    BSP_U32 u32NetRxStopInt;            /* ����ֹͣ�жϼ��� */
    BSP_U32 u32NetRxDescErr;            /* �������������� */

    BSP_U32 u32NetTxStatUnderFlow;      /* ����FIFO����ͳ�Ƽ��� */
    BSP_U32 u32NetTxUnderFlowInt;       /* ����FIFO�����жϼ��� */
    BSP_U32 u32NetTxStopInt;            /* ����ֹͣ�жϼ��� */
    BSP_U32 u32NetTxDescErrPs;          /* ��������������(Ps) */
    BSP_U32 u32NetTxDescErrOs;          /* ��������������(Os) */
    BSP_U32 u32NetTxDescErrMsp;         /* ��������������(Msp) */

    BSP_U32 u32NetFatalBusErrInt;      /* ���ߴ���*/
}BSP_OM_NET_S;

/******************************************************************************
* Function     :   BSP_OM_NET
*
* Description  :
*
* Input        :
*
* Output       :   ��
*
* return       :
******************************************************************************/
BSP_VOID BSP_OM_NET(BSP_OM_NET_S *pstNetOm);

/*VERSIONINFO_I���ݽṹ�а汾�ַ��������Ч�ַ�����*/
#define VER_MAX_LENGTH                  30

/*memVersionCtrl�ӿڲ�������*/
#define VERIONREADMODE                  0
#define VERIONWRITEMODE                 1

#define OM_SAVE_EXCHFILE_ING            0
#define OM_SAVE_EXCHFILE_END            1


/*�������*/
typedef enum
{
    VER_BOOTLOAD = 0,
    VER_BOOTROM =1,
    VER_NV =2 ,
    VER_VXWORKS =3,
    VER_DSP =4 ,
    VER_PRODUCT_ID =5 ,
    VER_WBBP =6 ,
    VER_PS =7,
    VER_OAM =8,
    VER_GBBP =9 ,
    VER_SOC =10,
    VER_HARDWARE =11,
    VER_SOFTWARE =12,
    VER_MEDIA =13,
    VER_APP =14,
    VER_ASIC =15,
    VER_RF =16,
    VER_PMU =17,
    VER_PDM = 18,
    VER_PRODUCT_INNER_ID = 19,
    VER_INFO_NUM =20
}COMP_TYPE_I;

/* ��ɫ��״̬���*/
enum
{
    LED_LIGHT_SYSTEM_STARTUP,
    LED_LIGHT_POWER_ON,
    LED_LIGHT_G_REGISTERED,
    LED_LIGHT_W_REGISTERED,
    LED_LIGHT_G_CONNECTED,
    LED_LIGHT_W_CONNNECTED,
    LED_LIGHT_H_CONNNECTED,
    LED_LIGHT_OFFLINE,
    LED_LIGHT_FORCE_UPDATING,
    LED_LIGHT_NORMAL_UPDATING,
    LED_LIGHT_UPDATE_FAIL,
    LED_LIGHT_UPDATE_SUCCESS,
    LED_LIGHT_UPDATE_FILEFAIL,
    LED_LIGHT_UPDATE_NVFAIL,
    LED_LIGHT_SIM_ABSENT,
    LED_LIGHT_SHUTDOWN,
    LED_LIGHT_G_PLMN_SEARCH,
    LED_LIGHT_W_PLMN_SEARCH,
    LED_LIGHT_L_REGISTERED,
    LED_LIGHT_STATE_MAX = 32
};/*the state of LED */

enum SECURE_SUPPORT_STATUS_I
{
    SECURE_NOT_SUPPORT = 0,
    SECURE_SUPPORT = 1
};

enum SECURE_ENABLE_STATUS_I
{
    SECURE_DISABLE = 0,
    SECURE_ENABLE = 1
};

typedef enum                /* CACHE_TYPE */
{
    PS_OSAL_INSTRUCTION_CACHE ,
    PS_OSAL_DATA_CACHE
} PS_OSAL_CACHE_TYPE_I;

typedef int  (*OM_SAVE_FUNC)(int funcType, unsigned char *data, unsigned int* ulLength);

typedef struct
{
    unsigned char CompId;              /* ����ţ��μ�COMP_TYPE */
    unsigned char CompVer[VER_MAX_LENGTH+1];         /* ���汾���� 30 �ַ�+ \0 */
}VERSIONINFO_I;

#ifndef __SUPPORT_MAX_HSPA_CAPA_STRU
#define __SUPPORT_MAX_HSPA_CAPA_STRU
typedef struct
{
    unsigned char enHSDSCHSupport;          /*Ӳ���Ƿ�֧��HSDPA*/
    unsigned char ucHSDSCHPhyCategory;      /*Ӳ��֧�ֵ����HSDPA�����ȼ�*/
    unsigned char enEDCHSupport;            /*Ӳ���Ƿ�֧��HSUPA*/
    unsigned char ucEDCHPhyCategory;        /*Ӳ��֧�ֵ����HSUPA�����ȼ�*/
    unsigned int  reserved;                 /*Ԥ��*/
}SUPPORT_MAX_HSPA_CAPA_STRU;
#endif
/*****************************BSP_MspProcReg  begin******************************/
typedef enum tagMSP_PROC_ID_E
{
    OM_REQUEST_PROC = 0,
    OM_ADDSNTIME_PROC = 1,
    OM_PRINTF_WITH_MODULE = 2,
    OM_PRINTF = 3,

    OM_PRINTF_GET_MODULE_IDLEV = 4,
    OM_READ_NV_PROC = 5,
    OM_WRITE_NV_PROC = 6,
    OM_MNTN_ERRLOG = 7,

    MSP_PROC_REG_ID_MAX
}MSP_PROC_ID_E;

typedef void (*BSP_MspProc)(void);
/*****************************************************************************
* �� �� ��  : DRV_MSP_PROC_REG
*
* ��������  : DRV�ṩ��OM��ע�ắ��
*
* �������  : MSP_PROC_ID_E eFuncID, BSP_MspProc pFunc
* �������  : NA
*
* �� �� ֵ  : NA
*
* ����˵��  : ��ά�ɲ�ӿں���
*
*****************************************************************************/
extern void DRV_MSP_PROC_REG(MSP_PROC_ID_E eFuncID, BSP_MspProc pFunc);


/*****************************************************************************
 �� �� ��  : BSP_MNTN_GetFlashSpec
 ��������  : Get flash infomation
 �������  : �ޡ�
 �������  : �ޡ�
 �� �� ֵ  : �ޡ�
*****************************************************************************/
extern int BSP_MNTN_GetFlashSpec(unsigned char * pFlashInfo, unsigned int ulLength);
#define DRV_GET_FLASH_INFO(pFlashInfo, usLength)    BSP_MNTN_GetFlashSpec (pFlashInfo, usLength )

/*************************************************
 �� �� ��   : BSP_MNTN_ProductTypeGet
 ��������   : ���ص�ǰ��Ʒ����
 �������   : ��
 �������   : ��
 �� �� ֵ   :0:STICK
             1:MOD
             2:E5
             3:CPE
*************************************************/
extern BSP_U32 BSP_MNTN_ProductTypeGet(void);
#define DRV_PRODUCT_TYPE_GET()   BSP_MNTN_ProductTypeGet()


/*****************************************************************************
 �� �� ��  : BSP_MNTN_GetGsmPATemperature
 ��������  : ��ȡGSM PA�¶�
 �������  : Temprature
             hkAdcTalble
 �������  : �ޡ�
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
*****************************************************************************/
extern int BSP_MNTN_GetGsmPATemperature(int *temperature, unsigned short *hkAdcTable);
#define DRV_GET_PA_GTEMP(temperature, hkAdcTable)   \
                BSP_MNTN_GetGsmPATemperature(temperature, hkAdcTable)

/*****************************************************************************
 �� �� ��  : getWcdmaPATemperature
 ��������  : ��ȡWCDMA PA�¶�
 �������  : Temprature
             hkAdcTalble
 �������  : �ޡ�
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
*****************************************************************************/
extern int BSP_MNTN_GetWcdmaPATemperature (int *temperature, unsigned short *hkAdcTable);
#define  DRV_GET_PA_WTEMP(temperature, hkAdcTable)  \
              BSP_MNTN_GetWcdmaPATemperature(temperature, hkAdcTable)

/*****************************************************************************
 �� �� ��  : BSP_MNTN_GetSIMTemperature
 ��������  : ��ȡSIM���¶�
 �������  : pusHkAdcTable����ŵ�ѹ��
 �������  : pTemp��        ָ��SIM���¶ȵ�ָ�롣
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
*****************************************************************************/
extern int BSP_MNTN_GetSIMTemperature(int *plTemp, unsigned short *pusHkAdcTable);
#define DRV_GET_SIM_TEMP(plTemp, pusHkAdcTable)   \
               BSP_MNTN_GetSIMTemperature(plTemp, pusHkAdcTable)


/*****************************************************************************
 �� �� ��  : BSP_MNTN_MemVersionCtrl
 ��������  : ����汾��д�ӿڡ�
 �������  : pcData����ModeΪ����ʱ��Ϊ�����ߴ����淵�ص�����汾��Ϣ���ڴ��ַ��
                     ��ModeΪд��ʱ��Ϊ������׼��д�������汾��Ϣ�ĵ�ַ��
             ucLength����ModeΪ����ʱ��Ϊ�����ߴ����淵�ص�����汾��Ϣ���ڴ��С��
                       ��ModeΪд��ʱ��Ϊ������׼��д�������汾��Ϣ���ַ�����������'\0'����
             ucType���汾��ϢID��
                     0�� BOOTLOAD��
                     1�� BOOTROM��
                     2�� NV��
                     3�� VXWORKS��
                     4�� DSP��
                     5�� CDROMISO��
                     6�� PHY��
                     7�� PS��
                     8�� COM��
                     9�� RF��
                     10��SOC��
                     11��HARDWARE��
                     12��SOFTWARE��
                     13��MEDIA��
                     14��APP��
                     15��INFO_NUM��
             ucMode��0����ȡָ��ID������汾��Ϣ��1��д��ָ��ID������汾��Ϣ��
 �������  : �ޡ�
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
*****************************************************************************/
extern int BSP_MNTN_MemVersionCtrl(signed char *pcData, unsigned char ucLength, unsigned char ucType, unsigned char ucMode);
#define DRV_MEM_VERCTRL(pcData,ucLength,ucType,ucMode)  \
                          BSP_MNTN_MemVersionCtrl(pcData,ucLength,ucType,ucMode)

/*****************************************************************************
 �� �� ��  : BSP_MNTN_GetFullHwVersion
 ��������  : Ӳ�������汾���ӿڡ�
 �������  : pFullHwVersion��Ϊ�����ߴ����淵�ص�Ӳ�������汾��Ϣ���ڴ��׵�ַ��
             ulLength      ��Ϊ�����ߴ����淵�ص�Ӳ�������汾��Ϣ���ڴ��С��
 �������  : �ޡ�
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
*****************************************************************************/
extern int BSP_MNTN_GetFullHwVersion(char * pFullHwVersion,unsigned int ulLength);
#define  DRV_GET_FULL_HW_VER(pFullHwVersion,ulLength)  \
               BSP_MNTN_GetFullHwVersion(pFullHwVersion,ulLength)

/*****************************************************************************
 �� �� ��  : BSP_MNTN_GetProductIdInter
 ��������  : ��Ʒ���������汾���ӿڡ�
 �������  : pProductIdInter��Ϊ�����ߴ����淵�صĲ�Ʒ���������汾���ڴ��׵�ַ��
             ulLength       ��Ϊ�����ߴ����淵�صĲ�Ʒ���������汾���ڴ��С��
 �������  : �ޡ�
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
*****************************************************************************/
extern int BSP_MNTN_GetProductIdInter(char * pProductIdInter, unsigned int ulLength);
#define  DRV_GET_PRODUCTID_INTER_VER(pProductIdInter,ulLength)  \
              BSP_MNTN_GetProductIdInter(pProductIdInter,ulLength)

/*****************************************************************************
 �� �� ��  : BSP_MNTN_VersionQueryApi
 ��������  : ��ѯ��������İ汾�š�
 �������  : ppVersionInfo��������İ汾��Ϣ��ַ��
 �������  : ucLength�������ص����ݵ��ֽ�����
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
*****************************************************************************/
extern int BSP_MNTN_VersionQueryApi(void ** ppVersionInfo, unsigned int * ulLength);
#define  DRV_VER_QUERY(ppVersionInfo,ulLength)  \
              BSP_MNTN_VersionQueryApi (ppVersionInfo,ulLength)


/*****************************************************************************
 �� �� ��  : DRV_SHUT_DOWN
 ��������  : ����ػ�
 �������  : ��
 �������  : eReason��        �ػ�ԭ��
 �� �� ֵ  : ��
*****************************************************************************/
void drvShutdown( DRV_SHUTDOWN_REASON_ENUM eReason );
void DRV_SHUT_DOWN( DRV_SHUTDOWN_REASON_ENUM eReason );

typedef enum tagMNTN_FILE_TYPE_E
{
	MNTN_TYPE_ZSP = 0,
	MNTN_TYPE_HIFI,
    MNTN_TYPE_MAX
}MNTN_FILE_TYPE_E;

/*****************************************************************************
 �� �� ��  : BSP_MNTN_ABBSelfCheck
 ��������  : ����abb�Լ���
 �������  : None
 �������  : None
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
*****************************************************************************/
extern int BSP_MNTN_ABBSelfCheck(void);
#define DRV_ABB_SELFCHECK()    BSP_MNTN_ABBSelfCheck()

/*****************************************************************************
 �� �� ��  : BSP_MNTN_PmuSelfCheck
 ��������  : PMU�Լ졣
 �������  : �ޡ�
 �������  : �ޡ�
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
*****************************************************************************/
extern int BSP_MNTN_PmuSelfCheck(void);
#define DRV_PMU_SELFCHECK()    BSP_MNTN_PmuSelfCheck()

/*****************************************************************************
 �� �� ��  : BSP_MNTN_I2cSelfCheck
 ��������  : ����I2c�Լ���
 �������  : None
 �������  : None
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
 ע��������ýӿڽ���PS��PC������ʹ�ã�Ŀǰû��Ӧ�ã��ݱ�����
*****************************************************************************/
extern int BSP_MNTN_I2cSelfCheck(void);
#define DRV_I2C_SELFCHECK()    BSP_MNTN_I2cSelfCheck()

/*****************************************************************************
 �� �� ��  : BSP_MNTN_SpiSelfCheck
 ��������  : ����spi�Լ���
 �������  : None
 �������  : None
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
 ע��������ýӿڽ���PS��PC������ʹ�ã�Ŀǰû��Ӧ�ã��ݱ�����
*****************************************************************************/
extern int BSP_MNTN_SpiSelfCheck(void);
#define DRV_SPI_SELFCHECK()    BSP_MNTN_SpiSelfCheck()

/*****************************************************************************
 �� �� ��  : BSP_MNTN_GetCodecFuncStatus
 ��������  : ��ȡCODEC����״̬
 �������  : ��
 �������  : ��
 �� �� ֵ  : 1:  CODEC����
             0��CODEC ������
*****************************************************************************/
extern  int BSP_MNTN_GetCodecFuncStatus(void);
#define DRV_GET_CODEC_FUNC_STATUS()    BSP_MNTN_GetCodecFuncStatus()

/*****************************************************************************
 �� �� ��  : MNTN_RFGLockStateGet
 ��������  : ��ȡGSM RF����״̬��
 �������  : �ޡ�
 �������  : Status���������״̬�����ֵ��
                        0��TX����RX��������
                        1��TX��RX��û�б�������

 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
*****************************************************************************/
extern int BSP_MNTN_RFGLockStateGet(unsigned int *Status);
#define DRV_GET_RF_GLOCKSTATE(Status)    BSP_MNTN_RFGLockStateGet(Status)

/****************************************************************************
 �� �� ��  : BSP_MNTN_LedStateFuncReg
 ��������  : ���ӿ�Ϊ�ṩ��Э��ջ�Ĺ��Ӻ��������ڻ�ȡЭ��ջ�ĵ��״̬��
 �������  : Э��ջ��ȡ���״̬������ָ�롣
 �������  : �ޡ�
 �� �� ֵ  : �ޡ�
 ע������  : �ޡ�

******************************************************************************/
extern void BSP_MNTN_LedStateFuncReg (pFUNCPTR p);
#define DRV_LED_STATE_FUNREG(p)    BSP_MNTN_LedStateFuncReg(p)

/*****************************************************************************
 �� �� ��  : BSP_MNTN_LedFlush
 ��������  : ��ɫ�����á�
 �������  : status����ɫ�Ƶ�״̬����LED_STATUS_I
 �������  : �ޡ�
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
*****************************************************************************/
extern int BSP_MNTN_LedFlush(unsigned long state);
#define DRV_LED_FLUSH(state)    BSP_MNTN_LedFlush(state)

/*****************************************************************************
 �� �� ��  : BSP_MNTN_LedControlAddressGet
 ��������  : �õ����Ƶĵ������
 �������  : ��
 �������  : �ޡ�
 �� �� ֵ  : ��ƿ������ݵ�ȫ�ֱ�����ַ
*****************************************************************************/
extern unsigned int BSP_MNTN_LedControlAddressGet(void);
#define DRV_LED_GET_ADDRESS()   BSP_MNTN_LedControlAddressGet()

/*****************************************************************************
 �� �� ��  : BSP_MNTN_BootForceloadModeCheck
 ��������  : ǿ�Ƽ���ģʽ��ѯ��
 �������  : �ޡ�
 �������  : �ޡ�
 �� �� ֵ  : 0��ǿ�Ƽ��أ�
             1����ǿ�Ƽ��ء�
*****************************************************************************/
extern unsigned int BSP_MNTN_BootForceloadModeCheck(void);
#define DRV_BOOT_FORCELOAD_MODE_CHECK()    BSP_MNTN_BootForceloadModeCheck()

/*****************************************************************************
 �� �� ��  : BSP_MNTN_BootFlagConfig
 ��������  : ����Bootline�����е�������־Ϊ����������������ʱ7�����������ֶ�������
 �������  : ulBootFlag����δ���塣
 �������  : �ޡ�
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
*****************************************************************************/
extern  int BSP_MNTN_BootFlagConfig(unsigned int ulBootFlag);
#define DRV_BOOTFLAG_CFG(ulBootFlag)    BSP_MNTN_BootFlagConfig(ulBootFlag)

/*****************************************************************************
 �� �� ��  : BSP_MNTN_CheckArmTCM
 ��������  : TCM���
 �������  : �ޡ�
 �������  : �ޡ�
 ����ֵ��   0xffff0000�����ɹ���TCM����
            ��ַ��������ĵ�ַ��0��0x5fff��
            0xffffffff:���ʧ��

*****************************************************************************/
extern unsigned int BSP_MNTN_CheckArmTCM(void);
#define DRV_CHECK_ARM_TCM()    BSP_MNTN_CheckArmTCM()

/*****************************************************************************
 �� �� ��  : BSP_MNTN_ExchOMSaveRegister
 ��������  : ע��������trace���溯���������ڵ��帴λʱ��
             ���浱ǰ���������־��Ϣ��
 �������  : funcType: ����ID
             pFunc:ע��Ļص�����
 �������  : None
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
 ע���������ͬ��funcTypeֻ��ע��һ���ص��������ظ�ע��Ḳ��ԭ������
*****************************************************************************/
extern int BSP_MNTN_ExchOMSaveRegister(int funcType, OM_SAVE_FUNC *pFunc);
#define DRV_SAVE_REGISTER(funcType, pFunc)    BSP_MNTN_ExchOMSaveRegister(funcType, pFunc)

/*****************************************************************************
 �� �� ��  : BSP_MNTN_SystemError
 ��������  : ϵͳ��������
 �������  : �ޡ�
 �������  : �ޡ�
 �� �� ֵ  : �ޡ�
*****************************************************************************/
extern void BSP_MNTN_SystemError(int modId, int arg1, int arg2, char * arg3, int arg3Length);
#define DRV_SYSTEM_ERROR(modId, arg1, arg2, arg3, arg3Length)\
                   BSP_MNTN_SystemError(modId, arg1, arg2, arg3, arg3Length)

/*****************************************************************************
 �� �� ��  : BSP_MNTN_ExchMemMalloc
 ��������  : ��systemOsaError�ӿ�һ��������¼���ĸ�λ��Ϣ��
             Ŀǰ���֧��128k.
 �������  : ulSize - �ڴ��С
 �������  : �ޡ�
 �� �� ֵ  : �ڴ��ַ
*****************************************************************************/
extern unsigned int BSP_MNTN_ExchMemMalloc(unsigned int ulSize);
#define DRV_EXCH_MEM_MALLOC(ulSize)    BSP_MNTN_ExchMemMalloc(ulSize)

/*****************************************************************************
 �� �� ��  : BSP_MNTN_OmExchFileSave
 ��������  : ������¼ARM��DSP�������쳣�ļ�
 �������  :    address��buffer��ַ
                length���洢����
                IsFileEnd��1��ʾ�ļ������һ�飬0��ʾ�����һ��
 �������  : �ޡ�
 �� �� ֵ  : 0 ΪOK��
*****************************************************************************/
extern int BSP_MNTN_OmExchFileSave(void * address, unsigned long length,unsigned char IsFileEnd,unsigned char type);
#define DRV_EXCH_FILE_SAVE(address, length, IsFileEnd, type) BSP_MNTN_OmExchFileSave(address, length, IsFileEnd, type)

/*****************************************************************************
 �� �� ��  : BSP_MNTN_GetHwGpioInfo
 ��������  : Get flash infomation
 �������  : �ޡ�
 �������  : �ޡ�
 �� �� ֵ  : �ޡ�
*****************************************************************************/
extern int BSP_MNTN_GetHwGpioInfo(unsigned char *pGpioInfo, unsigned long usLength );
#define DRV_GET_GPIO_INFO(pGpioInfo, usLength)    BSP_MNTN_GetHwGpioInfo(pGpioInfo, usLength )

/*****************************************************************************
 �� �� ��  : BSP_MNTN_Int4ToString
 ��������  : ���Ĵ�����ֵת��ΪASCII�ַ�
 �������  : �Ĵ�����ֵ����ת���ַ��Ĵ洢�ռ�,���ռ䲻����30byte
 �������  : ת���ַ�
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
*****************************************************************************/
extern int BSP_MNTN_Int4ToString(unsigned int value, char * string);
#define DRV_MEM_VERCONVERT(value,string)    BSP_MNTN_Int4ToString(value, string)

/*****************************************************************************
 �� �� ��  : BSP_MNTN_HeapInfoGet
 ��������  : ��ȡ�������ڴ���Ϣ��
 �������  : allocSize: �Ѿ�����Ķ��ڴ��С����λbyte��
             totalSize: ���ڴ��ܳߴ磬��λbyte��
 �������  : None
 �� �� ֵ  : 0: �����ɹ���
             -1��  ����ʧ�ܡ�
*****************************************************************************/
extern int BSP_MNTN_HeapInfoGet(unsigned int *allocSize, unsigned int *totalSize);
#define DRV_GET_HEAPINFO(allocSize, totalSize)    BSP_MNTN_HeapInfoGet(allocSize, totalSize)

/*****************************************************************************
 �� �� ��  : BSP_MNTN_GetSupportBands
 ��������  : �ӵ����õ�ǰ֧�ֵ�Bands.
 �������  : pusWBands - WCDMA��Ƶ��ֵ
             pusGBands - GSM��Ƶ��ֵ

 �������  : pusWBands - WCDMA��Ƶ��ֵ
             pusGBands - GSM��Ƶ��ֵ

 �� �� ֵ  : 0����ȷ����0: ʧ��
             ͨ��Bitλ����ʾ��ЩƵ��֧�֡�
*****************************************************************************/
extern int BSP_MNTN_GetSupportBands(unsigned short *pusWBands, unsigned short *pusGBands);
#define DRV_GET_SUPPORT_BANDS(pusWBands, pusGBands)    BSP_MNTN_GetSupportBands(pusWBands, pusGBands)

/*****************************************************************************
 �� �� ��  : BSP_MNTN_GetSupportDivBands
 ��������  : �ӵ����õ�ǰ֧�ֵķּ�Ƶ��
 �������  : N/A

 �������  : pDivBands - ���ص�ǰ������֧�ֵķּ�Ƶ�Σ�Ƶ�ΰ�bit
 			      ����ʽ����(ͬW ��G��Ƶ�β�ѯ)

 �� �� ֵ  : 0����ȷ����0: ʧ��
*****************************************************************************/
extern int BSP_MNTN_GetSupportDivBands(unsigned short* pDivBands);
#define DRV_GET_SUPPORT_DIVBANDS(pDivBands)    BSP_MNTN_GetSupportDivBands(pDivBands)

/********************************************************************************************************
 �� �� ��  : BSP_MNTN_WriteSysBackupFile
 ��������  : ��FLASH����ZSP��NV����
 �������  : ��
 �������  : ��
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
********************************************************************************************************/
extern int BSP_MNTN_WriteSysBackupFile(void);
#define DRV_BACKUP_SYS_FILE()    BSP_MNTN_WriteSysBackupFile()

/*****************************************************************************
 �� �� ��  : BSP_MNTN_GetExtAntenLockState
 ��������  : ��ȡ���������ߵ�����״̬
 �������  : enModemId:0 - modem0
                       1 - modem1
 �������  : pulStatus:0 - ��������
                       1 - ��������
 �� �� ֵ  : 0 - �ɹ�
             ����Ϊʧ��

*****************************************************************************/
extern int BSP_MNTN_GetExtAntenLockState(PWC_COMM_MODEM_E enModemId, unsigned int *pulStatus);
#define DRV_GET_ANTEN_LOCKSTATE(enModemId, pulStatus)    BSP_MNTN_GetExtAntenLockState(enModemId, pulStatus)

/*****************************************************************************
 �� �� ��  : BSP_MNTN_ExtAntenIntInstall
 ��������  : �ж�ע�ắ����������õ�ǰ������״̬
 �������  : enModemId:0 - modem0
                       1 - modem1
             routine   - �жϴ�����
             para      - ��ǰ������״̬
 �������  : None
 �� �� ֵ  : void

*****************************************************************************/
extern void BSP_MNTN_ExtAntenIntInstall(PWC_COMM_MODEM_E enModemId, void* routine, int para);
#define DRV_ANTEN_INT_INSTALL(enModemId, routine, para)    BSP_MNTN_ExtAntenIntInstall(enModemId, routine, para)

/*****************************************************************************
 �� �� ��  : BSP_MNTN_ExchHookFuncAdd
 ��������  : �����л����жϹ���ע��
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
extern void BSP_MNTN_ExchHookFuncAdd(void);
#define DRV_EXCH_HOOK_FUNC_ADD()    BSP_MNTN_ExchHookFuncAdd()

/*****************************************************************************
 �� �� ��  : BSP_MNTN_ExchHookFuncDelete
 ��������  : �����л����жϹ���ע��
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
extern void BSP_MNTN_ExchHookFuncDelete(void);
#define DRV_EXCH_HOOK_FUNC_DELETE()    BSP_MNTN_ExchHookFuncDelete()

/*****************************************************************************
 �� �� ��  : BSP_MNTN_ExcStackPeakRecord
 ��������  : ��¼����ջ�����������Ϣ.
 �������  : �ޡ�
 �������  : �ޡ�
 �� �� ֵ  : �ޡ�
*****************************************************************************/
extern void BSP_MNTN_ExcStackPeakRecord(void);
#define DRV_STACK_HIGH_RECORD()    BSP_MNTN_ExcStackPeakRecord()

/*****************************************************************************
 �� �� ��  : BSP_MNTN_SubIntHook
 ��������  : �����жϷ����Ӻ���ע�ᡣ��ά�ɲ����
 �������  : p_Func�����Ӻ���ָ�롣
 �������  : �ޡ�
 �� �� ֵ  : �ޡ�
*****************************************************************************/
extern void BSP_MNTN_SubIntHook(pFUNCPTR p_Func);
#define DRV_VICINT_HOOK_ADD(p_Func)    BSP_MNTN_SubIntHook(p_Func)

/*****************************************************************************
 �� �� ��  : BSP_MNTN_SubIntHookDel
 ��������  : �����жϷ����Ӻ���ע������ά�ɲ����
 �������  : �ޡ�
 �������  : �ޡ�
 �� �� ֵ  : �ޡ�
*****************************************************************************/
extern void BSP_MNTN_SubIntHookDel(void);
#define DRV_VICINT_HOOK_DEL()    BSP_MNTN_SubIntHookDel()

/*****************************************************************************
 �� �� ��  : BSP_MNTN_IntBreakInHook
 ��������  : һ���ж���ڹ��Ӻ���ע�ᡣ��ά�ɲ����
 �������  : p_Func :���Ӻ�����
 �������  : �ޡ�
 �� �� ֵ  : �ޡ�
*****************************************************************************/
extern void BSP_MNTN_IntBreakInHook(pFUNCPTR p_Func);
#define DRV_VICINT_IN_HOOK_ADD(p_Func)   BSP_MNTN_IntBreakInHook(p_Func)

/*****************************************************************************
 �� �� ��  : BSP_MNTN_IntBreakInHookDel
 ��������  : һ���ж���ڹ��Ӻ���ע������ά�ɲ����
 �������  : �ޡ�
 �������  : �ޡ�
 �� �� ֵ  : �ޡ�
*****************************************************************************/
extern void BSP_MNTN_IntBreakInHookDel(void);
#define DRV_VICINT_IN_HOOK_DEL()    BSP_MNTN_IntBreakInHookDel()

/*****************************************************************************
 �� �� ��  : BSP_MNTN_IntBreakOutHook
 ��������  : һ���жϳ��ڹ��Ӻ���ע�ᡣ��ά�ɲ���أ�V7�ȴ�׮
 �������  : p_Func: ���Ӻ�����
 �������  : �ޡ�
 �� �� ֵ  : �ޡ�
*****************************************************************************/
extern void BSP_MNTN_IntBreakOutHook(pFUNCPTR p_Func);
#define DRV_VICINT_OUT_HOOK_ADD(p_Func)    BSP_MNTN_IntBreakOutHook(p_Func)

/*****************************************************************************
 �� �� ��  : vicIntBreakOutHookDel
 ��������  : һ���жϳ��ڹ��Ӻ���ע������ά�ɲ����
 �������  : �ޡ�
 �������  : �ޡ�
 �� �� ֵ  : �ޡ�
*****************************************************************************/
extern void BSP_MNTN_IntBreakOutHookDel(void);
#define DRV_VICINT_OUT_HOOK_DEL()    BSP_MNTN_IntBreakOutHookDel()

/*****************************************************************************
 �� �� ��  : BSP_MNTN_IntLvlChgHook
 ��������  : һ���ж�level change���Ӻ���ע�ᡣ��ά�ɲ����
 �������  : p_Func: ���Ӻ�����
 �������  : �ޡ�
 �� �� ֵ  : �ޡ�
*****************************************************************************/
extern void BSP_MNTN_IntLvlChgHook(FUNCPTR_1 p_Func);
#define DRV_VICINT_LVLCHG_HOOK_ADD(p_Func)    BSP_MNTN_IntLvlChgHook(p_Func)

/*****************************************************************************
 �� �� ��  : BSP_MNTN_IntLvlChgHookDel
 ��������  : һ���ж�level change���Ӻ���ע������ά�ɲ����
 �������  : �ޡ�
 �������  : �ޡ�
 �� �� ֵ  : �ޡ�
*****************************************************************************/
extern void BSP_MNTN_IntLvlChgHookDel(void);
#define DRV_VICINT_LVLCHG_HOOK_DEL()    BSP_MNTN_IntLvlChgHookDel()

/*****************************************************************************
 �� �� ��  : vic_int_exit_hook_register
 ��������  : ϵͳ�ж��˳����Ӻ�����ע�ṳ�Ӻ����ӿ�
 �������  : FUNCPTR_1 p_Func
 �������  : ��
 �� �� ֵ  : extern void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��9��20��
    �޸�����   : �����ɺ���

*****************************************************************************/
extern void vic_int_exit_hook_register(FUNCPTR_1 p_Func);
#define DRV_VICINT_EXIT_HOOK_ADD(p_Func)    vic_int_exit_hook_register(p_Func)

/*****************************************************************************
 �� �� ��  : vic_int_exit_hook_unregister
 ��������  : ϵͳ�ж��˳����Ӻ�����ע�����Ӻ����ӿ�
 �������  : void
 �������  : ��
 �� �� ֵ  : extern void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��9��20��
    �޸�����   : �����ɺ���

*****************************************************************************/
extern void vic_int_exit_hook_unregister(void);
#define DRV_VICINT_EXIT_HOOK_DEL()    vic_int_exit_hook_unregister()

/*****************************************************************************
 �� �� ��  : BSP_MNTN_TFUPIfNeedNvBackup
 ��������  : ʹ��TF������ǰͨ���ýӿ��ж��Ƿ���Ҫ����NV��.
 �������  : None
 �������  : None
 �� �� ֵ  : 1:��Ҫ����
             0:���豸��
*****************************************************************************/
extern int BSP_MNTN_TFUPIfNeedNvBackup(void);
#define DRV_TF_NVBACKUP_FLAG()    BSP_MNTN_TFUPIfNeedNvBackup()

/*****************************************************************************
 �� �� ��  : BSP_MNTN_GETSDSTATUS
 ��������  : ����SD����λ״̬
 �������  : �ޡ�
 �������  : �ޡ�
 ����ֵ��   1��λ
            0����λ

*****************************************************************************/
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
extern int BSP_MNTN_GETSDSTATUS(void);
#define DRV_SDMMC_GET_STATUS()    BSP_MNTN_GETSDSTATUS()
#else
extern int BSP_SDMMC_GetSDStatus(void);
#define DRV_SDMMC_GET_STATUS()    BSP_SDMMC_GetSDStatus()
#endif

/*****************************************************************************
 �� �� ��  : SDIO_read_write_blkdata
 ��������  : SD����д�ӿ�
 �������  :    nCardNo     SD����
                dwBlkNo     ����
                nBlkCount   ��д��С
                pbDataBuff  ��д������
                nFlags      ��/д
 �������  : �ޡ�
 ����ֵ��   0:  ��д�ɹ�
            ��0:������
*****************************************************************************/
extern int BSP_MNTN_SDRWBlkData(int nCardNo,unsigned int dwBlkNo,
                   unsigned int nBlkCount, unsigned char *pbDataBuff,int nFlags);
#define DRV_SDMMC_BLK_DATA_RW(nCardNo,dwBlkNo,nBlkCount,pbDataBuff,nFlags)   \
             BSP_MNTN_SDRWBlkData(nCardNo,dwBlkNo,nBlkCount,pbDataBuff, nFlags)

/*****************************************************************************
 �� �� ��  : CicomClkEnable
 ��������  : This routine enable CICOM IP clock gating.�͹�����أ�V7��׮
 �������  : None
 �������  : None
 �� �� ֵ  : None
*****************************************************************************/
/*extern void CicomClkEnable(void);*/
#define DRV_CICOM_CLK_ENABLE()

/*****************************************************************************
 �� �� ��  : CicomClkDisable
 ��������  : This routine disable CICOM IP clock gating.�͹�����أ�V7��׮
 �������  : None
 �������  : None
 �� �� ֵ  : None
*****************************************************************************/
/*extern void CicomClkDisable(void);*/
#define DRV_CICOM_CLK_DISABLE()

/*****************************************************************************
 �� �� ��  : HdlcClkEnable
 ��������  : This routine enable HDLC IP clock gating.�͹�����أ�V7��׮
 �������  : None
 �������  : None
 �� �� ֵ  : None
*****************************************************************************/
/*extern void HdlcClkEnable(void);*/
#define DRV_HDLC_CLK_ENABLE()

/*****************************************************************************
 �� �� ��  : HdlcClkDisable
 ��������  : This routine disable HDLC IP clock gating.�͹�����أ�V7��׮
 �������  : None
 �������  : None
 �� �� ֵ  : None
*****************************************************************************/
/*extern void HdlcClkDisable(void);*/
#define DRV_HDLC_CLK_DISABLE()

/*****************************************************************************
 �� �� ��  : DRV_MEM_READ
 ��������  : ��32λ��Ȳ�ѯ�ڴ棬���4�ֽ����ݡ���ά�ɲ⣬V7�ȴ�׮
 �������  : ulAddress����ѯ��ַ�ռ��ַ����ַ�����ڴ淶Χ�ڣ����򷵻�-1��
 �������  : pulData��ָ���ѯ���ݵ�ָ�롣
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
*****************************************************************************/
extern int DRV_MEM_READ(unsigned int ulAddress, unsigned int* pulData);

/*****************************************************************************
 �� �� ��  : DrvLogInstall
 ��������  : ��ӡ����ע�ᡣ��ά�ɲ⣬V7�ȴ�׮
 �������  : fptr ע��ĺ���ָ��
 �������  : None
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
*****************************************************************************/
/*extern int DrvLogInstall(PVOIDFUNC fptr, PVOIDFUNC fptr1, PVOIDFUNC fptr2, PVOIDFUNC fptr3, PVOIDFUNC fptr4);*/
#define DRV_LOG_INSTALL(fptr, fptr1, fptr2, fptr3, fptr4)     DRV_OK

/*****************************************************************************
* �� �� ��  : BSP_MNTN_GetBattState
*
* ��������  : ��ȡ�����λ״̬
*
* �������  : ��
* �������  : ��
*
* �� �� ֵ  : �ɹ���0��1��2
              ʧ�ܣ�����
*
*
*****************************************************************************/
extern BSP_S32 BSP_MNTN_GetBattState(void);
#define DRV_MNTN_GET_BATT_STATE() BSP_MNTN_GetBattState()


/*******************************************************************************
 �� �� ��: DRV_EXCH_CUST_FUNC_REG
 ��������: ��������ģ���ṩ����ӿڣ����ϲ�ע��ص������ڸ�λʱ�����ϲ�ָ����Ϣ
 �������: cb �ϲ�ָ���ص��ӿ�
 �������: ��
 �� �� ֵ: 0-��ӳɹ�������-���ʧ��
 ˵��    :  pucData :ָ����¼��Ϣ��ַ����ע�ⲻҪָ��ջ����ʱ���������ѿռ����ݣ�
                     ʹ�����Լ�ά���������ռ�ʹ���꣬�Լ��ͷš�����ȫ�ֱ�����
            aucName :����
            ulDataLen: ���ݳ���

����ֵ�б�:
    CUST_REG_OK = 0,                        ��ӳɹ�
    CUST_REG_PARM_NULL,                     ���Ϊ��
    CUST_REG_EXCH_NOT_READY,                EXCHģ����δ��ʼ��
    CUST_REG_EXCH_REBOOT_CONTEXT,           �Ѿ����븴λ����
    CUST_REG_EXCH_QUEUE_NOT_INIT,           ע�������δ��ʼ��
    CUST_REG_EXCH_QUEUE_FULL,               ��������
    CUST_REG_EXCH_ALREADY_IN_QUEUE,         �ظ�ע��
    CUST_REG_EXCH_MUTEX_REJECT,             MUTEX REJECT
    CUST_REG_EXCH_QUEUE_IN_ERR              �����ʧ��

��ͨ������ֵ��λ�������

*******************************************************************************/
#define EXCH_CB_NAME_SIZE           (32)
typedef struct
{
    char   aucName[EXCH_CB_NAME_SIZE];
    unsigned char *pucData;
    unsigned int  ulDataLen;
}cb_buf_t;

typedef int  (*exchCBReg)(cb_buf_t *);
extern int DRV_EXCH_CUST_FUNC_REG(exchCBReg cb);

/*��̬���û���Դ*/
typedef enum 
{
    DRV_WAKE_SRC_UART0 = 0,
    DRV_WAKE_SRC_SOCP,
    DRV_WAKE_SRC_DRX_TIMER,
    DRV_WAKE_SRC_BUT
}DRV_PM_WAKE_SRC_ENUM;
/*****************************************************************************
* �� �� ��  : DRV_PM_SET_WAKE_SRC
* ��������  : ��̬���û���Դ
* �������  : eWakeSrc ����Դö��ֵ
* �������  : ��
* �� �� ֵ  : ��
*****************************************************************************/
extern BSP_VOID BSP_PM_SetWakeSrc(DRV_PM_WAKE_SRC_ENUM wake_src);
#define DRV_PM_SET_WAKE_SRC(eWakeSrc)   BSP_PM_SetWakeSrc(eWakeSrc)

/*****************************************************************************
* �� �� ��  : DRV_PM_UNSET_WAKE_SRC
* ��������  : ��̬ȡ������Դ
* �������  : eWakeSrc ����Դö��ֵ
* �������  : ��
* �� �� ֵ  : ��
*****************************************************************************/
extern BSP_VOID BSP_PM_UnsetWakeSrc(DRV_PM_WAKE_SRC_ENUM wake_src);
#define DRV_PM_UNSET_WAKE_SRC(eWakeSrc)   BSP_PM_UnsetWakeSrc(eWakeSrc)
#endif


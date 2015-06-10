
#ifndef __DRV_VERSION_H__
#define __DRV_VERSION_H__
#include "drv_global.h"
/* Ӳ����Ʒ��Ϣ���� */
#define HW_VER_INVALID              (BSP_U16)0xFFFF

/* Porting���P500����� */
#define HW_VER_PRODUCT_PORTING      (BSP_U16)0xFFFE
#define HW_VER_PRODUCT_PV500        (BSP_U16)0xFFFD

/* ���԰� */
#define HW_VER_PRODUCT_UDP          (BSP_U16)0xFFFC

/* Stick */
#define HW_VER_PRODUCT_E392S_U      (BSP_U16)HW_VER_INVALID
#define HW_VER_PRODUCT_E3276S_150   (BSP_U16)0x0201

/* E5 */
#define HW_VER_PRODUCT_E5_SBM       (BSP_U16)0x0001

/*E5776*/
#define HW_VER_PRODUCT_E5776_EM     (BSP_U16)0x0000

/*Epad, S10 101u */
#define HW_VER_PRODUCT_S10_101U     (BSP_U16)0x0401

/*Epad, S10 102u */
#define HW_VER_PRODUCT_S10_102U      (BSP_U16)0x0406

/*Epad, S10 LTE */
#define HW_VER_PRODUCT_S10_101L     (BSP_U16)0x0501

/*Epad, S10 Modem */
#define HW_VER_PRODUCT_S10_MODEM     (BSP_U16)0x0000

/*Epad, S10 Temp */
#define HW_VER_PRODUCT_S10_TEMP     (BSP_U16)0x0005

/*****************************************************************************
* �� �� ��  : BSP_HwGetVerMain
* ��������  : ��ȡ��Ʒ�汾��
* �������  :
* �������  :
* �� �� ֵ  :
*****************************************************************************/
BSP_U16 BSP_HwGetVerMain(BSP_VOID);

/*****************************************************************************
* �� �� ��  : BSP_GetProductName
* ��������  : ��ȡ��Ʒ����
* �������  : char* pProductName,�ַ���ָ�룬��֤��С��32�ֽ�
*             BSP_U32 ulLength,����������
* �������  : ��
* �� �� ֵ  : 0����ȷ����0: ʧ��
*****************************************************************************/
BSP_S32 BSP_GetProductName (char * pProductName, unsigned int ulLength);

#define BUILD_DATE_LEN  12
#define BUILD_TIME_LEN  12

typedef struct  tagUE_SW_BUILD_VER_INFO_STRU
{
    BSP_U16 ulVVerNo;        /* V���� */
    BSP_U16 ulRVerNo;        /*  R����*/
    BSP_U16 ulCVerNo;        /* C����*/
    BSP_U16 ulBVerNo;         /* B����*/
    BSP_U16 ulSpcNo;          /* SPC����*/
    BSP_U16 ulCustomVer;     /* �ͻ��趨�Ʋ���, ���Ӳ���ӿڵ�8bit PCB��,��8bitHW��*/
    BSP_U32 ulProductNo;     /* such as porting,CPE, ... */
    BSP_S8 acBuildDate[BUILD_DATE_LEN];  /* build����,*/
    BSP_S8 acBuildTime[BUILD_TIME_LEN];  /* buildʱ��*/
} UE_SW_BUILD_VER_INFO_STRU; /* �ڲ��汾*/


typedef struct {
    BSP_U32   bandGU;         	/*֧�ֵ�GUƵ��*/
    BSP_U32   bamdLTELow;         /*֧��LTEƵ�Σ�B1~B32*/
    BSP_U32   bamdLTEHigh;        /*֧��LTEƵ�Σ�B33~B64*/
}BAND_INFO_T;

typedef struct {
    BSP_U16  index;         /*Ӳ���汾����ֵ(��汾��1+��汾��2)�����ֲ�ͬ��Ʒ*/
    BSP_U16   hwIdSub;         /*Ӳ���Ӱ汾�ţ����ֲ�Ʒ�Ĳ�ͬ�İ汾*/
    BSP_CHAR* name;           /*�ڲ���Ʒ��*/
    BSP_CHAR* namePlus;       /*�ڲ���Ʒ��PLUS*/
    BSP_CHAR* hwVer;          /*Ӳ���汾����*/
    BSP_CHAR* dloadId;        /*������ʹ�õ�����*/
    BSP_CHAR* productId;      /*�ⲿ��Ʒ��*/
    BAND_INFO_T  band;            /*��Ʒ֧�ֵ�Ƶ��*/
    BAND_INFO_T  bandDiv;         /*��Ʒ֧�ֵķּ�*/
    BSP_CHAR reserve[4];      /*Ԥ��*/
}PRODUCT_INFO_T;

/*****************************************************************************
* �� �� ��  : BSP_GetBuildVersion
*
* ��������  :
*
* �������  :
* �������  : ��
*
* �� �� ֵ  : ��
*
*
*****************************************************************************/
const UE_SW_BUILD_VER_INFO_STRU* BSP_GetBuildVersion(BSP_VOID);


/*****************************************************************************
* �� �� ��  : BSP_HwGetHwVersion
* ��������  : ��ȡӲ���汾����
* �������  : BSP_CHAR* pHwVersion,�ַ���ָ�룬��֤��С��32�ֽ�
* �������  : ��
* �� �� ֵ  : ��
*****************************************************************************/
BSP_S32 BSP_HwGetHwVersion (char* pFullHwVersion, BSP_U32 ulLength);


/*****************************************************************************
* �� �� ��  : BSP_GetSupportBands
*
* ��������  : ��ȡ֧�ֵ�Ƶ��
*
* �������  : BAND_INFO_T *pBandInfo,Ƶ����Ϣ�ṹ��ָ��
* �������  :
*
 �� �� ֵ  : 0����ȷ����0: ʧ��
*
* ����˵��  :
*
*****************************************************************************/
BSP_S32 BSP_GetSupportBands( BAND_INFO_T *pBandInfo);

/*****************************************************************************
* �� �� ��  : BSP_HwIsSupportWifi
* ��������  : ��ӡӲ���汾��Ϣ,��׮
* �������  : ��
* �������  : ��
* �� �� ֵ  : ��
* ����˵��  : ��
*****************************************************************************/
BSP_BOOL BSP_HwIsSupportWifi(BSP_VOID);


/********************************************************************************************************
 �� �� ��  : getHwVersionIndex
 ��������  : ��ȡ��HKADC��ȡ��Ӳ��ID����ƴװ���Ӳ������ֵ
 �������  : ��
 �������  : Ӳ��ID������ֵ

 �� �� ֵ  : ��-1:  �����ɹ���
             -1������ʧ�ܡ�
********************************************************************************************************/
extern int getHwVersionIndex(void);
#define DRV_GET_HW_VERSION_INDEX()       getHwVersionIndex()


/*****************************************************************************
 �� �� ��  : VerifySIMLock
 ��������  : �жϵ�ǰ�������Ƿ���ȷ .
 �������  : imei       - ����IMEI��
             unlockcode - ������
 �������  : �ޡ�
 �� �� ֵ  : 1��   �����ɹ�
             0:    ����ʧ��
*****************************************************************************/
extern int VerifySL(char* UnlockCode, char* Key);
#define DRV_CARDLOCK_MD5_VERIFY(unlockcode, imei)  VerifySL(unlockcode, imei)


extern int GetAuthVer(void);
#define DRV_GET_AUTH_VER()  GetAuthVer()


/* ��ǰ�汾�Ƿ�֧��ĳ��ģ�� */
typedef enum tagBSP_MODULE_SUPPORT_E
{
    BSP_MODULE_SUPPORT     = 0,
    BSP_MODULE_UNSUPPORT   = 1,
    BSP_MODULE_SUPPORT_BUTTOM
}BSP_MODULE_SUPPORT_E;

/* ��ѯ��ģ������ */
typedef enum tagBSP_MODULE_TYPE_E
{
    BSP_MODULE_TYPE_SD = 0x0,
    BSP_MODULE_TYPE_CHARGE,
    BSP_MODULE_TYPE_WIFI,
    BSP_MODULE_TYPE_OLED,
    BSP_MODULE_TYPE_HIFI,
    BSP_MODULE_TYPE_POWER_ON_OFF,
    BSP_MODULE_TYPE_HSIC,
    BSP_MODULE_TYPE_HSIC_NCM,
    BSP_MODULE_TYPE_LOCALFLASH,

    BSP_MODULE_TYPE_BUTTOM
}BSP_MODULE_TYPE_E;

/*****************************************************************************
 �� �� ��  : BSP_CheckModuleSupport
 ��������  : ��ѯģ���Ƿ�֧��
 �������  : enModuleType: ��Ҫ��ѯ��ģ������
 �������  : ��
 ����ֵ    ��BSP_MODULE_SUPPORT��BSP_MODULE_UNSUPPORT
*****************************************************************************/
BSP_MODULE_SUPPORT_E BSP_CheckModuleSupport(BSP_MODULE_TYPE_E enModuleType);

#define DRV_GET_BATTERY_SUPPORT()   BSP_CheckModuleSupport(BSP_MODULE_TYPE_CHARGE)

#define DRV_GET_WIFI_SUPPORT()      BSP_CheckModuleSupport(BSP_MODULE_TYPE_WIFI)

#define DRV_GET_SD_SUPPORT()        BSP_CheckModuleSupport(BSP_MODULE_TYPE_SD)

#define DRV_GET_OLED_SUPPORT()      BSP_CheckModuleSupport(BSP_MODULE_TYPE_OLED)

#define DRV_GET_HIFI_SUPPORT()      BSP_CheckModuleSupport(BSP_MODULE_TYPE_HIFI)

#define DRV_GET_HSIC_SUPPORT()      BSP_CheckModuleSupport(BSP_MODULE_TYPE_HSIC)


/*****************************************************************************
 �� �� ��  : BSP_InitPlatformVerInfo
 ��������  : ��ʼ��оƬ�İ汾��
 �������  : ��
 �������  : none
 �� �� ֵ  : void
*****************************************************************************/
extern BSP_VOID BSP_InitPlatformVerInfo(BSP_VOID);
#define DRV_INIT_PLATFORM_VER() BSP_InitPlatformVerInfo()


extern BSP_S32  DRV_START_MODEGET(void);

/*****************************************************************************
 �� �� ��  : BSP_GetPlatformInfo
 ��������  : ��ȡоƬ�İ汾��
 �������  : ��
 �������  : u32PlatformInfo:оƬ�İ汾��
 �� �� ֵ  : void
*****************************************************************************/
extern BSP_VOID BSP_GetPlatformInfo(unsigned long *u32PlatformInfo);
#define DRV_GET_PLATFORM_INFO(u32PlatformInfo) BSP_GetPlatformInfo(u32PlatformInfo)

#endif


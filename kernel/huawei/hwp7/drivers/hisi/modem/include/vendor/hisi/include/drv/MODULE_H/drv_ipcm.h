/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_ipcm.h
  �� �� ��   : ����
  ��������   : 2013��2��2��
  ����޸�   :
  ��������   : drv_ipcm.h ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��2��2��
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __DRV_IPCM_H__
#define __DRV_IPCM_H__
#include "drv_global.h"

#ifdef __OS_RTOSCK__

#if defined(BSP_CORE_MODEM) 
#include <sre_base.h>
#endif

#else 

#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM) || defined(__VXWORKS__)
#include <vxWorks.h>
#endif

#endif

/* ����������*/
typedef enum tagIPC_INT_CORE_E
{
    IPC_CORE_ARM11 = 0x0,   /* P500�ϵ�IPC */
    IPC_CORE_A9,
    IPC_CORE_CEVA,
    IPC_CORE_TENS0,
    IPC_CORE_TENS1,
    IPC_CORE_DSP,
    IPC_CORE_APPARM = 0x0,  /* V7R1/V3R2�ϵ�IPC */
    IPC_CORE_COMARM,
    IPC_CORE_LTEDSP,
    IPC_CORE_VDSP,
    IPC_CORE_ZSP,
    IPC_CORE_DSP_GU,
    IPC_CORE_ACPU = 0x0,/* V9R1 SFT�ϵ�IPC */
    IPC_CORE_CCPU,
    IPC_CORE_MCU,
    IPC_CORE_HIFI,
    IPC_CORE_BBE16,


    /* !!!!����Ԫ������ӵ����  */
    IPC_CORE_BUTTOM
}IPC_INT_CORE_E;

/*********************************************************
*  �����IPC��Դ��ö��������ʽ:
*  IPC_<Ŀ�괦����>_INT_SRC_<Դ������>_<����/����>
*  Ŀ�괦����:ACPU��CCPU��MCU��HIFI��BBE16
*  Դ������  :ACPU��CCPU��MCU��HIFI��BBE16
*  ����/���� :
*********************************************************/
typedef enum tagIPC_INT_LEV_E
{
   /* ����CCPU IPC�����Ϣ�ж�Դbitλ�� */
    IPC_INT_DSP_MODEM                   = 0,
    IPC_CCPU_INT_SRC_HIFI_MSG           = 1,    /* bit1, HIFI�����Ϣ֪ͨ */
    IPC_INT_DSP_MSP                     = 2,
    IPC_INT_DSP_PS 						= 3,
    IPC_CCPU_INT_SRC_MCU_MSG            = 5,    /* bit5, MCU�����Ϣ֪ͨ */
    IPC_CCPU_INT_SRC_ACPU_MSG           = 6,
    IPC_CCPU_INT_SRC_ACPU_IFC           = 7,
    IPC_INT_DSP_HALT                    = 8,    /* bit8, DSP֪ͨARM˯��*/
    IPC_INT_DSP_RESUME                  = 9,    /* bit9, DSP֪ͨARM��ɻ��Ѻ�Ļָ�����*/
    IPC_CCPU_INT_SRC_MCU_IFC            = 10,
    IPC_INT_WAKE_GU                     = 11,   /* bit11, ��ģ���Ѵ�ģ�ж�*/
    IPC_INT_SLEEP_GU                    = 12,   /* bit12, ��ģ˯���ж�*/
    IPC_INT_DICC_USRDATA_ACPU           = 13,   /* bit13, ռλ,��ֵ������IPC_INT_DICC_USRDATAһ��*/
    IPC_INT_DICC_RELDATA_ACPU           = 14,   /* bit14, ռλ,��ֵ������IPC_INT_DICC_USRDATAһ��*/
    IPC_INT_ARM_SLEEP                   = 15,   /* bit15, ��8�޸�Ϊ15*/
    /* ����TDSʹ�õ�����IPC�ж�,begin */
    IPC_INT_WAKE_GSM                    = 16,   /* bit16, BBE16֪ͨCCPU,����GSM��ģ */
    IPC_INT_WAKE_WCDMA                  = 17,   /* bit17, BBE16֪ͨCCPU,����WCDMA��ģ */
    IPC_INT_DSP_PS_PUB_MBX              = 18,   /* ����DSP->ARM,��͹���IPC�жϸ��� */
    IPC_INT_DSP_PS_MAC_MBX              = 19,   /* ����DSP->ARM,��͹���IPC�жϸ��� */
    IPC_INT_DSP_MBX_RSD                 = 20,   /* ����DSP->ARM,��͹���IPC�жϸ��� */
    /* ����TDSʹ�õ�����IPC�ж�,end */
    IPC_CCPU_INT_SRC_ACPU_ICC           = 30,
    IPC_CCPU_INT_SRC_BUTT               = 32,

   /* ����MCU IPC�����Ϣ�ж�Դbitλ�� */
    IPC_MCU_INT_SRC_ACPU_MSG            = 4,    /* bit4, ACPU�����Ϣ֪ͨ */
    IPC_MCU_INT_SRC_CCPU_MSG            = 5,    /* bit5, CCPU�����Ϣ֪ͨ */
    IPC_MCU_INT_SRC_HIFI_MSG            = 6,    /* bit6, HIFI�����Ϣ֪ͨ */
    IPC_MCU_INT_SRC_CCPU_IFC            = 7,    /* bit7, HIFI�����Ϣ֪ͨ */
    IPC_MCU_INT_SRC_CCPU_IPF            = 8,    /* bit8, IPF�͹��� */
    IPC_MCU_INT_SRC_ACPU_IFC            = 9,    /* bit9, ACPU��˺�������֪ͨ */
    IPC_MCU_INT_SRC_ACPU_HOTPLUG,
    IPC_MCU_INT_SRC_ACPU_PD             = 16,   /* bit16, acpu power down */
    IPC_MCU_INT_SRC_CCPU_PD             = 17,   /* bit17, ccpu power down */
    IPC_MCU_INT_SRC_HIFI_PD             = 18,   /* bit18, hifi power down */
    IPC_MCU_INT_SRC_MCU_AGT             = 19,   /* bit19, mcu agent */
    IPC_MCU_INT_SRC_ACPU_I2S_REMOTE_SLOW    = 21,
    IPC_MCU_INT_SRC_ACPU_I2S_REMOTE_SLEEP   = 22,
    IPC_MCU_INT_SRC_ACPU_I2S_REMOTE_INVALID = 23,
    IPC_MCU_INT_SRC_BUTT                = 32,

    /* ����ACPU IPC�����Ϣ�ж�Դbitλ�� */
    IPC_ACPU_INT_SRC_CCPU_MSG           = 1,    /* bit1, CCPU�����Ϣ֪ͨ */
    IPC_ACPU_INT_SRC_HIFI_MSG           = 2,    /* bit2, HIFI�����Ϣ֪ͨ */
    IPC_ACPU_INT_SRC_MCU_MSG            = 3,    /* bit3, ACPU�����Ϣ֪ͨ */
    IPC_ACPU_INT_SRC_CCPU_NVIM          = 4,    /* bit4,����NVIMģ��C����A��֮���ͬ��*/
    IPC_ACPU_INT_SRC_CCPU_IFC           = 5,
    IPC_ACPU_INT_SRC_MCU_IFC            = 6,
    IPC_ACPU_INT_SRC_MCU_THERMAL_HIGH   = 7,    /*MCU��طŵ���¹ػ�IPC�ж�֪ͨACPU*/
    IPC_ACPU_INT_SRC_MCU_THERMAL_LOW    = 8,    /*MCU��طŵ���¹ػ�IPC�ж�֪ͨACPU*/
    IPC_INT_DSP_APP                     = 9,
    IPC_INT_DICC_USRDATA                = 13,   /*������IPC_INT_DICC_USRDATA_ACPUͬʱ�޸�*/
    IPC_INT_DICC_RELDATA                = 14,   /*������IPC_INT_DICC_RELDATA_ACPUͬʱ�޸�*/
    IPC_ACPU_INI_SRC_MCU_EXC_REBOOT     = 27,
    IPC_ACPU_INT_SRC_CCPU_EXC_REBOOT    = 28,
    IPC_ACPU_INT_SRC_CCPU_NORMAL_REBOOT = 29,
    IPC_ACPU_INT_SRC_MCU_DDR_EXC        = 30,
    IPC_ACPU_INT_SRC_CCPU_ICC           = 31,
    IPC_ACPU_INT_SRC_BUTT               = 32,


    /* ����HIFI IPC�����Ϣ�ж�Դbitλ�� */
    IPC_HIFI_INT_SRC_ACPU_MSG           = 0,    /* bit0, ACPU�����Ϣ֪ͨ */
    IPC_HIFI_INT_SRC_CCPU_MSG           = 1,    /* bit1, CCPU�����Ϣ֪ͨ */
    IPC_HIFI_INT_SRC_BBE_MSG            = 4,    /* bit4, TDDSP�����Ϣ֪ͨ */
    IPC_HIFI_INT_SRC_MCU_MSG            = 6,
    IPC_HIFI_INT_SRC_BUTT               = 32,

    /* ����BBE16 IPC�����Ϣ�ж�Դbitλ�� */
    IPC_INT_MSP_DSP_OM_MBX              = 0,   /* ����ARM->DSP */
    IPC_INT_PS_DSP_PUB_MBX              = 1,   /* ����ARM->DSP */
    IPC_INT_PS_DSP_MAC_MBX              = 2,   /* ����ARM->DSP */
    IPC_BBE16_INT_SRC_HIFI_MSG          = 3,   /* bit3, HIFI�����Ϣ֪ͨ */
    IPC_BBE16_INT_SRC_BUTT              = 32,
    /* ������������� */
    IPC_INT_BUTTOM                      = 32,
}IPC_INT_LEV_E;

typedef enum tagIPC_SEM_ID_E
{
    IPC_SEM_ICC,
    IPC_SEM_NAND,
    IPC_SEM_MEM,
    IPC_SEM_DICC,
    IPC_SEM_RFILE_LOG,          /*����flashless��ά�ɲ�*/
    IPC_SEM_EMMC,
    IPC_SEM_NVIM,
    IPC_SEM_SMP_CPU0 = 21,
    IPC_SEM_SMP_CPU1 = 22,
    IPC_SEM_SMP_CPU2 = 23,
    IPC_SEM_SMP_CPU3 = 24,
    IPC_SEM_SYNC = 25,
    IPC_SEM_BBP = 26,           /*��ֵ��BBE16����󶨣��޸���֪ͨBBE16����Ӧ�޸�*/
    IPC_SEM_DFS_HOTPLUG = 27,   /*��Ƶ���˻�����Դ��*/
    IPC_SEM_SYSCTRL = 30,
    IPC_SEM_ZSP_HALT = 31,
    IPC_SEM_BUTTOM
}IPC_SEM_ID_E;


#define INTSRC_NUM                     32

/* ����P500��IPC�����ӿ�,��IPCרΪLģLMSP��LPS��װʹ��,��GUģ��IPC����ͻ��
*/
/*****************************************************************************
* �� �� ��  : BSP_DRV_IPCIntInit_Lt
*
* ��������  : IPCģ���ʼ��,Lģר��
*
* �������  : ��
* �������  : ��
*
* �� �� ֵ  : ��
*
*****************************************************************************/
BSP_S32 BSP_DRV_IPCIntInit_Lt(void);

/*****************************************************************************
* �� �� ��  : BSP_IPC_IntEnable_Lt
*
* ��������  : ʹ��ĳ���ж�,Lģר��
*
* �������  :   IPC_INT_CORE_E enCoreNum Ҫʹ���жϵ�core
                BSP_U32 ulLvl Ҫʹ�ܵ��жϺţ�ȡֵ��Χ0��31
* �������  : ��
*
* �� �� ֵ  : OK&ERROR
*
*****************************************************************************/
BSP_S32 BSP_IPC_IntEnable_Lt (IPC_INT_LEV_E ulLvl);

/*****************************************************************************
 * �� �� ��  : BSP_INT_Disable_Lt
 *
 * ��������  : ȥʹ��ĳ���ж�
 *
 * �������  : IPC_INT_CORE_E enCoreNum Ҫʹ���жϵ�core
                BSP_U32 ulLvl Ҫʹ�ܵ��жϺţ�ȡֵ��Χ0��31
 * �������  : ��
 *
 * �� �� ֵ  : OK&ERROR
 *
 *****************************************************************************/
BSP_S32 BSP_IPC_IntDisable_Lt (IPC_INT_LEV_E ulLvl);

/*****************************************************************************
 * �� �� ��  : BSP_IPC_IntConnect_Lt
 *
 * ��������  : ע��ĳ���ж�,Lģר��
 *
 * �������  : IPC_INT_CORE_E enCoreNum Ҫʹ���жϵ�core
               BSP_U32 ulLvl Ҫʹ�ܵ��жϺţ�ȡֵ��Χ0��31
               VOIDFUNCPTR routine �жϷ������
 *             BSP_U32 parameter      �жϷ���������
 * �������  : ��
 *
 * �� �� ֵ  : OK&ERROR
 *
 *****************************************************************************/
BSP_S32 BSP_IPC_IntConnect_Lt(IPC_INT_LEV_E ulLvl, VOIDFUNCPTR routine, BSP_U32 parameter);

/*****************************************************************************
 * �� �� ��  : BSP_IPC_IntDisonnect_Lt
 *
 * ��������  : ȡ��ע��ĳ���ж�,Lģר��
 *
 * �������  :
 *              BSP_U32 ulLvl Ҫʹ�ܵ��жϺţ�ȡֵ��Χ0��31
 *              VOIDFUNCPTR routine �жϷ������
 *             BSP_U32 parameter      �жϷ���������
 * �������  : ��
 *
 * �� �� ֵ  : OK&ERROR
 *
 *****************************************************************************/
 BSP_S32 BSP_IPC_IntDisonnect_Lt (IPC_INT_LEV_E ulLvl,VOIDFUNCPTR routine, BSP_U32 parameter);

 /*****************************************************************************
 * �� �� ��  : BSP_DRV_IpcIntHandler_Lt
 *
 * ��������  : �жϴ�����,Lģר��
 *
 * �������  : ��
 * �������  : ��
 *
 * �� �� ֵ  : ��
 *
 *****************************************************************************/
BSP_VOID BSP_DRV_IpcIntHandler_Lt(void);

/*****************************************************************************
* �� �� ��  : BSP_IPC_IntSend_Lt
*
* ��������  : �����ж�,Lģר��
*
* �������  :
                IPC_INT_CORE_E enDstore Ҫ�����жϵ�core
                BSP_U32 ulLvl Ҫ���͵��жϺţ�ȡֵ��Χ0��31
* �������  : ��
*
* �� �� ֵ  : OK&ERROR
*
*****************************************************************************/
BSP_S32 BSP_IPC_IntSend_Lt(IPC_INT_CORE_E enDstCore, IPC_INT_LEV_E ulLvl);

/* ����P500��IPC�����ӿ�,��IPCרΪLģLPHY��LPS��װʹ��,��GUģ��IPC����ͻ��
*/

/*****************************************************************************
* �� �� ��  : BSP_DRV_IPCIntInit
*
* ��������  : IPCģ���ʼ��
*
* �������  : ��
* �������  : ��
*
* �� �� ֵ  : ��
*
*****************************************************************************/
BSP_S32 BSP_DRV_IPCIntInit(void);


/*****************************************************************************
* �� �� ��  : DRV_IPC_SEMCREATE
*
* ��������  : �ź�����������
*
* �������  : ��
* �������  : ��
*
* �� �� ֵ  : ��
*
*****************************************************************************/
BSP_S32 BSP_IPC_SemCreate(BSP_U32 u32SignalNum);
extern BSP_S32 DRV_IPC_SEMCREATE(BSP_U32 u32SignalNum);

/*****************************************************************************
* �� �� ��  : DRV_IPC_SEMDELETE
*
* ��������  : ɾ���ź���
*
* �������  :   BSP_U32 u32SignalNum Ҫɾ�����ź������

* �������  : ��
*
* �� �� ֵ  : OK&ERROR
*
*****************************************************************************/
BSP_S32 BSP_IPC_SemDelete(BSP_U32 u32SignalNum);
extern BSP_S32 DRV_IPC_SEMDELETE(BSP_U32 u32SignalNum);

/*****************************************************************************
* �� �� ��  : BSP_IPC_IntEnable
*
* ��������  : ʹ��ĳ���ж�
*
* �������  :
                BSP_U32 ulLvl Ҫʹ�ܵ��жϺţ�ȡֵ��Χ0��31
* �������  : ��
*
* �� �� ֵ  : OK&ERROR
*
*****************************************************************************/
BSP_S32 BSP_IPC_IntEnable (IPC_INT_LEV_E ulLvl);
extern BSP_S32 DRV_IPC_INTENABLE(IPC_INT_LEV_E ulLvl);

/*****************************************************************************
* �� �� ��  : DRV_IPC_INTDISABLE
*
* ��������  : ȥʹ��ĳ���ж�
*
* �������  :
            BSP_U32 ulLvl Ҫʹ�ܵ��жϺţ�ȡֵ��Χ0��31
* �������  : ��
*
* �� �� ֵ  : OK&ERROR
*
*****************************************************************************/
BSP_S32 BSP_IPC_IntDisable (IPC_INT_LEV_E ulLvl);
extern BSP_S32 DRV_IPC_INTDISABLE(IPC_INT_LEV_E ulLvl);

/*****************************************************************************
* �� �� ��  : BSP_IPC_IntConnect
*
* ��������  : ע��ĳ���ж�
*
* �������  :
           BSP_U32 ulLvl Ҫʹ�ܵ��жϺţ�ȡֵ��Χ0��31
           VOIDFUNCPTR routine �жϷ������
*             BSP_U32 parameter      �жϷ���������
* �������  : ��
*
* �� �� ֵ  : OK&ERROR
*
*****************************************************************************/
BSP_S32 BSP_IPC_IntConnect  (IPC_INT_LEV_E ulLvl,VOIDFUNCPTR routine, BSP_U32 parameter);
extern BSP_S32 DRV_IPC_INTCONNECT(IPC_INT_LEV_E ulLvl,VOIDFUNCPTR routine, BSP_U32 parameter);

/*****************************************************************************
* �� �� ��  : BSP_IPC_IntDisonnect
*
* ��������  : ȡ��ע��ĳ���ж�
*
* �������  :
*              BSP_U32 ulLvl Ҫʹ�ܵ��жϺţ�ȡֵ��Χ0��31
*              VOIDFUNCPTR routine �жϷ������
*             BSP_U32 parameter      �жϷ���������
* �������  : ��
*
* �� �� ֵ  : OK&ERROR
*
*****************************************************************************/
BSP_S32 BSP_IPC_IntDisonnect  (IPC_INT_LEV_E ulLvl,VOIDFUNCPTR routine, BSP_U32 parameter);
extern BSP_S32 DRV_IPC_INTDISCONNECT  (IPC_INT_LEV_E ulLvl,VOIDFUNCPTR routine, BSP_U32 parameter);

/*****************************************************************************
* �� �� ��  : DRV_IPC_INTSEND
*
* ��������  : �����ж�
*
* �������  :
                IPC_INT_CORE_E enDstore Ҫ�����жϵ�core
                BSP_U32 ulLvl Ҫ���͵��жϺţ�ȡֵ��Χ0��31
* �������  : ��
*
* �� �� ֵ  : OK&ERROR
*
*****************************************************************************/
BSP_S32 BSP_IPC_IntSend(IPC_INT_CORE_E enDstCore, IPC_INT_LEV_E ulLvl);
extern BSP_S32 DRV_IPC_INTSEND(IPC_INT_CORE_E enDstCore, IPC_INT_LEV_E ulLvl);

/*****************************************************************************
* �� �� ��  : DRV_IPC_SEMTAKE
*
* ��������  : ��ȡ�ź���
*
* �������  : ��
* �������  : ��
*
* �� �� ֵ  : ��
*
*****************************************************************************/
BSP_S32 BSP_IPC_SemTake(BSP_U32 u32SignalNum,BSP_S32 s32timeout);
extern BSP_S32 DRV_IPC_SEMTAKE(BSP_U32 u32SignalNum, BSP_S32 s32timeout);

/*****************************************************************************
* �� �� ��  : DRV_IPC_SEMGIVE
*
* ��������  : �ͷ��ź���
*
* �������  : ��
* �������  : ��
*
* �� �� ֵ  : ��
*
*****************************************************************************/
BSP_VOID BSP_IPC_SemGive(BSP_U32 u32SignalNum);
extern BSP_VOID DRV_IPC_SEMGIVE(BSP_U32 u32SignalNum);

/*****************************************************************************
* �� �� ��  : BSP_IPC_SpinLock
*
* ��������  : ��ȡ�ź���
*
* �������  : ��
* �������  : ��
*
* �� �� ֵ  : ��
*
*****************************************************************************/
BSP_VOID BSP_IPC_SpinLock (BSP_U32 u32SignalNum);
extern BSP_VOID DRV_SPIN_LOCK (BSP_U32 u32SignalNum);

/*****************************************************************************
* �� �� ��  : DRV_SPIN_UNLOCK
*
* ��������  : �ͷ��ź���
*
* �������  : ��
* �������  : ��
*
* �� �� ֵ  : ��
*
*****************************************************************************/
BSP_VOID BSP_IPC_SpinUnLock (BSP_U32 u32SignalNum);
extern BSP_VOID DRV_SPIN_UNLOCK(BSP_U32 u32SignalNum);


BSP_S32 BSP_SGI_Connect(BSP_U32 ulLvl,VOIDFUNCPTR routine, BSP_U32 parameter);
BSP_S32 BSP_SGI_IntSend(BSP_U32 ulLvl);

#endif


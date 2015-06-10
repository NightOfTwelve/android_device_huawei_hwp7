/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_debug_led.c
  �� �� ��   : ����
  ��    ��   : 
  ��������   : 2012��3��2��
  ����޸�   :
  ��������   : ������ϲ������װ�Ľӿڲ�
  �޸���ʷ   :
  1.��    ��   : 2012��3��2��
    ��    ��   : 
    �޸�����   : �½�Drvinterface.c
    
  2.��    ��   : 2013��2��19��
    ��    ��   : 
    �޸�����   : ��Drvinterface.c�������

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "BSP.h"
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
#include <vxWorks.h>
#include <private/vmLibP.h>
#include <cacheLib.h>
#include "arm_pbxa9.h"
#include <cacheLib.h>
#include "BSP_TIMER.h"
#include "sys_config.h"
#include "soc_baseaddr_interface.h"
#include "string.h"
#include "stdio.h"
#include "BSP_LED.h"
#include "BSP_EMMC.h"
#endif

#include "soc_baseaddr_interface.h"
#include "soc_timer_interface.h"
#include "soc_sctrl_interface.h"
#include <soc_ao_sctrl_interface.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 ��������
*****************************************************************************/
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
extern BSP_S32 BSP_DBLED_ON( DBLED_ID_E ledId );
extern BSP_S32 BSP_DBLED_OFF( DBLED_ID_E ledId );
extern int BSP_SYSCTRL_REG_SET(unsigned int regAddr, unsigned int setData);
extern int BSP_SYSCTRL_REG_GET(unsigned int regAddr, unsigned int * getData);
#endif

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

/*****************************************************************************
 �� �� ��  : DRV_DBLED_ON
 ��������  : ���ԵƵƵ���������
 �������  : ledId:���ID,ȡֵ0~5
 �������  : ��
 �� �� ֵ  : int
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��25��
    ��    ��   :  
    �޸�����   : �����ɺ���

*****************************************************************************/
int DRV_DBLED_ON( unsigned int ledId )
{
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return BSP_DBLED_ON((DBLED_ID_E)ledId);
#endif

#if (defined BSP_CORE_APP)
    return 0;   /* ��׮ */
#endif
}

/*****************************************************************************
 �� �� ��  : DRV_DBLED_OFF
 ��������  : ���Ե�Ϩ������
 �������  : ledId:���ID,ȡֵ0~5
 �������  : ��
 �� �� ֵ  : int
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��25��
    ��    ��   :  
    �޸�����   : �����ɺ���

*****************************************************************************/
int DRV_DBLED_OFF( unsigned int ledId )
{
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return BSP_DBLED_OFF((DBLED_ID_E)ledId);
#endif

#if (defined BSP_CORE_APP)
    return 0;   /* ��׮ */
#endif
}

/*****************************************************************************
 �� �� ��  : DRV_SYSCTRL_REG_SET
 ��������  : DRV�ṩͳһ��sysctrlд�ӿں���
 �������  : regAddr��SC�Ĵ���ƫ�Ƶ�ַ��
             setData: SC�Ĵ���ֵ��
 �������  : ��
 �� �� ֵ  : int
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��25��
    ��    ��   :  
    �޸�����   : �����ɺ���

*****************************************************************************/
int DRV_SYSCTRL_REG_SET(unsigned int regAddr, unsigned int setData)
{
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return BSP_SYSCTRL_REG_SET(regAddr, setData);
#endif

#if (defined BSP_CORE_APP)
    return 0;   /* ��׮ */
#endif
}

/*****************************************************************************
 �� �� ��  : DRV_SYSCTRL_REG_GET
 ��������  : DRV�ṩͳһ��sysctrl���ӿں���
 �������  : regAddr�� SC�Ĵ���ƫ�Ƶ�ַ��
             getData������SC�Ĵ���ֵ��ָ�롣
 �������  : ��
 �� �� ֵ  : int
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��25��
    ��    ��   :  
    �޸�����   : �����ɺ���

*****************************************************************************/
int DRV_SYSCTRL_REG_GET(unsigned int regAddr, unsigned int * getData)
{
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return BSP_SYSCTRL_REG_GET(regAddr, getData);
#endif

#if (defined BSP_CORE_APP)
    return 0;   /* ��׮ */
#endif
}




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


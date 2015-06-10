/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_abb.c
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


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 ��������
*****************************************************************************/
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
extern BSP_S32 BSP_ABB_RegGet( BSP_U32 regAddr, BSP_U32 * getData );
extern BSP_S32 BSP_ABB_RegSet( BSP_U32 regAddr, BSP_U32 setData );
#endif

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

/*****************************************************************************
 �� �� ��  : DRV_ABB_REG_GET
 ��������  : get ABB register data
 �������  : regAddr --- �����Ĵ�����ַ,ABB��ַλ��Ϊ9bit,32bit������չ
 �������  : *getData --- ��żĴ���ֵ,ABB����λ��Ϊ8bit,32bit������չ
 �� �� ֵ  : 0    - successful
             -1 - unsuccessful

 �޸���ʷ      :
  1.��    ��   : 2012��6��19��
    ��    ��   :  
    �޸�����   : �����ɺ���

*****************************************************************************/
int DRV_ABB_REG_GET(unsigned int regAddr, unsigned int * getData)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return BSP_ABB_RegGet(regAddr, getData);
#endif

#if (defined BSP_CORE_APP)
    return 0;   /* ��׮ */
#endif
}

/*****************************************************************************
 �� �� ��  : DRV_ABB_REG_SET
 ��������  : set ABB register data
 �������  : regAddr --- �����üĴ�����ַ,ABB��ַλ��Ϊ9bit,32bit������չ
             setData --- ���õ�ֵ,ABB����λ��Ϊ8bit,32bit������չ
 �������  : ��
 �� �� ֵ  : 0    - successful
             -1   - unsuccessful

 �޸���ʷ      :
  1.��    ��   : 2012��6��19��
    ��    ��   :  
    �޸�����   : �����ɺ���

*****************************************************************************/
int DRV_ABB_REG_SET(unsigned int regAddr, unsigned int setData)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return BSP_ABB_RegSet(regAddr, setData);
#endif

#if (defined BSP_CORE_APP)
    return 0;   /* ��׮ */
#endif
}

/*****************************************************************************
 �� �� ��  : DRV_AUX_ABB_REG_SET
 ��������  : set AUX ABB register data
 �������  : regAddr --- �����üĴ�����ַ,ABB��ַλ��Ϊ9bit,32bit������չ
             setData --- ���õ�ֵ,ABB����λ��Ϊ8bit,32bit������չ
 �������  : ��
 �� �� ֵ  : 0    - successful
             -1   - unsuccessful

 �޸���ʷ      :
  1.��    ��   : 2012��6��19��
    ��    ��   :  
    �޸�����   : �����ɺ���

*****************************************************************************/
int DRV_AUX_ABB_REG_SET(unsigned int regAddr, unsigned int setData)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return BSP_OK;  /* temp tub��for RF6361 */
#endif

#if (defined BSP_CORE_APP)
    return 0;   /* ��׮ */
#endif
}

/*****************************************************************************
 �� �� ��  : DRV_AUX_ABB_REG_GET
 ��������  : get AUX ABB register data
 �������  : regAddr --- �����Ĵ�����ַ,ABB��ַλ��Ϊ9bit,32bit������չ
 �������  : *getData --- ��żĴ���ֵ,ABB����λ��Ϊ8bit,32bit������չ
 �� �� ֵ  : 0    - successful
             -1 - unsuccessful

 �޸���ʷ      :
  1.��    ��   : 2012��6��19��
    ��    ��   :  
    �޸�����   : �����ɺ���

*****************************************************************************/
int DRV_AUX_ABB_REG_GET(unsigned int regAddr, unsigned int * getData)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return BSP_OK;  /* temp tub��for RF6361 */
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


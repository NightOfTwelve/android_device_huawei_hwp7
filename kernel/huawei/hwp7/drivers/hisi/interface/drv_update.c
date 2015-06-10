/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_update.c
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
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
extern BSP_S32 BSP_DLOAD_GetDloadVer(BSP_CHAR *str, BSP_U32 len);
extern int BSP_SET_UPDATA_FLAG(int flag);
extern int BSP_DLOAD_Upgrade_Nv_Set_Result(int status);
extern int BSP_DLOAD_Get_Fs_Root_Directory(char * data, unsigned int len);
extern int BSP_DLOAD_NVBackupExtRead(unsigned char *pRamAddr, unsigned int len);
extern int BSP_DLOAD_NVBackupExtWrite(unsigned char *pRamAddr, unsigned int len);
#endif

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

/*****************************************************************************
 �� �� ��  : DRV_GET_DLOAD_VERSION
 ��������  : Get dload version
 �������  : �ޡ�
 �������  : �ޡ�
 �� �� ֵ  : �ޡ�
*****************************************************************************/
BSP_S32 DRV_GET_DLOAD_VERSION(BSP_U8 *str, int len)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return BSP_DLOAD_GetDloadVer((BSP_CHAR *)str, len);
#endif

#if (defined BSP_CORE_APP)
    return 0;   /* ��׮ */
#endif
}

/*****************************************************************************
 �� �� ��  : DRV_SET_UPDATA_FLAG
 ��������  : �����������ر�־��
 �������  : flag���������ر�־��
                   0�����������bootrom�������������ء�
                   1�������󲻽���bootrom������������
 �������  : �ޡ�
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
*****************************************************************************/
int DRV_SET_UPDATA_FLAG(int flag)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return BSP_SET_UPDATA_FLAG(flag);
#endif

#if (defined BSP_CORE_APP)
    return 0;   /* ��׮ */
#endif
}

/* Begin: DRVΪOAMʵ��NV���ݻָ����̶��ṩ�Ľӿ� */

/********************************************************************************************************
 �� �� ��  : DRV_UPGRADE_NV_SET_RESULT
 ��������  : OAMͨ���ýӿڸ��ߵ���NV�����ɹ��˻���ʧ���ˣ�
             ���NV�����ɹ���������fastboot�������óɹ���ʶ
             ���NV����ʧ�ܣ�����ʲô������
 �������  : status��0��ʾNV�����ɹ���
                     ����ֵ��ʾNV����ʧ��
 �������  : 
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
 ����ע��  : V9 BBIT ��׮
             V9 SFT  ʵ��
 
 �޸���ʷ  :
 1.��    ��   : 2012��12��6��
   ��    ��   :  
   �޸�����   : �����ɺ���               
********************************************************************************************************/
int DRV_UPGRADE_NV_SET_RESULT(int status)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return BSP_DLOAD_Upgrade_Nv_Set_Result(status);
#endif

#if (defined BSP_CORE_APP)
    return 0;   /* ��׮ */
#endif
}

/********************************************************************************************************
 �� �� ��  : DRV_NV_FILE_SEC_CHECK
 ��������  : ���ļ�����У��             
 �������  : pnvFilePath����У���ļ���·��
 �������  : 
 �� �� ֵ  : 0:  �ļ�У����ȷ��
             -1���ļ�У��ʧ�ܡ�
 ����ע��  : Ŀǰ�������߱���ֱ�Ӵ�׮���Ժ�ʵ��			 
 
 �޸���ʷ  :
 1.��    ��   : 2012��12��6��
   ��    ��   :  
   �޸�����   : �����ɺ���               
********************************************************************************************************/
int DRV_NV_FILE_SEC_CHECK(char * pnvFilePath)
{
    return 0;
}

/********************************************************************************************************
 �� �� ��  : DRV_GET_FS_ROOT_DIRECTORY
 ��������  : ��ѯ��Ŀ¼�ķ������
 �������  : data�����ڴ�Ÿ�Ŀ¼�ַ����Ļ��������׵�ַ
             len:������data�ĳ���
 �������  : 
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
 
 �޸���ʷ  :
 1.��    ��   : 2012��12��6��
   ��    ��   :  
   �޸�����   : �����ɺ���               
********************************************************************************************************/
int DRV_GET_FS_ROOT_DIRECTORY(char * data, unsigned int len)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)    
    return BSP_DLOAD_Get_Fs_Root_Directory(data, len);
#endif

#if (defined BSP_CORE_APP)
    return 0;   /* ��׮ */
#endif
}

/********************************************************************************************************
 �� �� ��  : DRV_NVBACKUP_EXT_READ
 ��������  : ��Flash�е�LastBackup����ȡ���ݣ�ΪNV�������ݶ�ʧ���ݴ�ָ������ṩ֧��
 �������  : len����LastBackup��ʼ����ʼ����Ҫ��ȡ��NV��ȣ��ֽ�������
 �������  : pRamAddr��Ŀ��RAM��ַ�����ڴ�Ŷ�����NV�����ݡ�
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
 ����ע��  : V3R3 SFT  ��׮
             V9R1 SFT  ʵ��

 �޸���ʷ  :
 1.��    ��   : 2013��10��22��
   ��    ��   :  
   �޸�����   : ��������
********************************************************************************************************/
int DRV_NVBACKUP_EXT_READ(unsigned char *pRamAddr, unsigned int len)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return BSP_DLOAD_NVBackupExtRead(pRamAddr, len);
#endif

#if (defined BSP_CORE_APP)
    return 0;   /* ��׮ */
#endif
}

/********************************************************************************************************
 �� �� ��  : DRV_NVBACKUP_EXT_WRITE
 ��������  : ���ض�����д��Flash�е�LastBackup����ΪNV�������ݶ�ʧ���ݴ�ָ������ṩ֧��
 �������  : pRamAddr��ԴRAM��ַ�����ڴ����Ҫд���NV�����ݡ�
             len����LastBackup����ʼ����ʼ����Ҫд���NV��ȣ��ֽ�������
 �������  : �ޡ�
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
 ����ע��  : V3R3 SFT  ��׮
             V9R1 SFT  ʵ��

 �޸���ʷ  :
 1.��    ��   : 2013��10��22��
   ��    ��   :  
   �޸�����   : ��������
********************************************************************************************************/
int DRV_NVBACKUP_EXT_WRITE(unsigned char *pRamAddr, unsigned int len)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return BSP_DLOAD_NVBackupExtWrite(pRamAddr, len);
#endif

#if (defined BSP_CORE_APP)
    return 0;   /* ��׮ */
#endif    
}

/* End: DRVΪOAMʵ��NV���ݻָ����̶��ṩ�Ľӿ� */

/*****************************************************************************
* �� �� ��  : BSP_DLOAD_GetBootromVer
*
* ��������  : ��ȡBOOTROM�汾��
*
* �������  : BSP_S8 *str   :�ַ���������
*             BSP_S32 len   :�ַ�������
* �������  : BSP_S8 *str   :�ַ���������
*
* �� �� ֵ  : DLOAD_ERROR   :��������Ƿ�
*             ����          :�����ַ�������
*
* ����˵��  : ��
*
*****************************************************************************/
BSP_S32 BSP_DLOAD_GetBootromVer(BSP_CHAR *str, BSP_U32 len)
{
    return 0;
}

/*****************************************************************************
* �� �� ��  : BSP_DLOAD_Reset
*
* ��������  : ��������
*
* �������  : BSP_VOID
* �������  : ��
*
* �� �� ֵ  : ��
*
* ����˵��  : ATģ�����
*             ��Ϊ˽�нӿ�
*             ����ģʽ֧��
*             ����ģʽ��֧��
*             ����ģʽ���ܵ��������Ժ󵥰彫��������
*
*****************************************************************************/
BSP_VOID BSP_DLOAD_Reset(BSP_VOID)
{
    return;
}

BSP_S32 DRV_GET_AUTHORITY_ID(unsigned char *buf, BSP_S32 len)
{
    return 0;
}


BSP_S32 DRV_GET_AUTHORITY_VERSION(BSP_U8 *str, BSP_S32 len)
{
    return 0;
}

BSP_S32 DRV_GET_DLOAD_INFO(unsigned char atCmdBuf[], unsigned int dloadType)
{
    return 0;
}




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


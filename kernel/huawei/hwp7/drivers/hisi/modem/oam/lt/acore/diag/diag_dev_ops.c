/******************************************************************************

   Copyright(C)2008,Hisilicon Co. LTD.

 ******************************************************************************
  File Name       : dms_dev_ops.c
  Description     : 
  History         :
     1.w00182550       2013-1-31   Draft Enact

******************************************************************************/


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 Include HeadFile                            
*****************************************************************************/
/*lint -save -e322 -e7 -e537*/
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "vos.h"
#include "DrvInterface.h"
#include "diag_vcom.h"
/*lint -restore*/

/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/


/*****************************************************************************
  3 Function 
*****************************************************************************/
    /*lint -save -e40 -e63*/

/*****************************************************************************
* �� �� ��  : diag_udi_open name:/dev/acm_at
*
* ��������  : ���豸(����ͨ��)
*
* �������  : pParam: �豸�Ĵ����ò���
* �������  : ��
* �� �� ֵ  : -1:ʧ�� / ����:�ɹ�
*****************************************************************************/
/*lint --e{718, 746, 730}*/
UDI_HANDLE diag_udi_open(char* name)
{
    /*lint --e{565}*/
    struct file* filp;

    filp = filp_open(name, O_RDWR, 0);
    if (IS_ERR(filp)) 
    {
        /*lint -save -e515*/
        pr_emerg("%s, open(%d) fail:%s\n", __FUNCTION__, (int)filp, name);
        /*lint -restore*/
        return UDI_INVALID_HANDLE;
    }

    return (UDI_HANDLE)filp;
}

/*****************************************************************************
* �� �� ��  : diag_udi_close
*
* ��������  : �ر��豸(����ͨ��)
*
* �������  : handle: �豸��handle
* �������  : ��
* �� �� ֵ  : �ɹ�/ʧ��
*****************************************************************************/
VOS_INT32 diag_udi_close(UDI_HANDLE handle)
{
    struct file* filp = (struct file*)handle;
    /*lint -save -e515*/
    if (unlikely((UDI_HANDLE)UDI_INVALID_HANDLE == handle || (UDI_HANDLE)0 == handle))
    {
        pr_emerg("%s, invalid param\n", __FUNCTION__);
        /*lint -restore*/
        return (UDI_HANDLE)(VOS_ERROR);
    }

    filp_close(filp, NULL);
    return VOS_OK;
}

/*****************************************************************************
* �� �� ��  : diag_udi_write
*
* ��������  : ����д
*
* �������  : handle:  �豸��handle
*             pMemObj: buffer�ڴ� �� �ڴ��������
*             u32Size: ����д�ߴ� �� �ڴ��������ɲ�����
* �������  :
*
* �� �� ֵ  : ����ֽ��� �� �ɹ�/ʧ��
*****************************************************************************/
VOS_INT32 diag_udi_write(UDI_HANDLE handle, void* pMemObj, BSP_U32 u32Size)
{
    mm_segment_t old_fs;
    struct file* filp = (struct file*)handle;
    VOS_INT32 status;
    loff_t pos = 0;

    if (unlikely((UDI_HANDLE)VOS_ERROR == handle || (UDI_HANDLE)0 == handle)) 
    {
        /*lint -save -e515*/
        pr_emerg("%s, invalid param\n", __FUNCTION__);
        /*lint -restore*/
        return (UDI_HANDLE)(VOS_ERROR);
    }

    old_fs = get_fs();
    set_fs(KERNEL_DS);
    /*lint -save -e151 -e529*/
    status = vfs_write(filp, (void __force __user *)pMemObj, u32Size, &pos);
    set_fs(old_fs);
    /*lint -restore*/
    return status;
/*lint -save -e529*/
}
/*lint -restore*/
/*****************************************************************************
* �� �� ��  : diag_udi_read
*
* ��������  : ���ݶ�
*
* �������  : handle:  �豸��handle
*             pMemObj: buffer�ڴ� �� �ڴ��������
*             u32Size: ���ݶ��ߴ� �� �ڴ��������ɲ�����
* �������  :
*
* �� �� ֵ  : ����ֽ��� �� �ɹ�/ʧ��
*****************************************************************************/
VOS_INT32 diag_udi_read(UDI_HANDLE handle, void* pMemObj, BSP_U32 u32Size)
{
    mm_segment_t old_fs;
    struct file* filp = (struct file*)handle;
    VOS_INT32 status;
    loff_t pos = 0;

    if (unlikely((UDI_HANDLE)VOS_ERROR == handle || (UDI_HANDLE)0 == handle)) 
    {
        /*lint -save -e515*/
        pr_emerg("%s, invalid param\n", __FUNCTION__);
        /*lint -restore*/
        return (UDI_HANDLE)(VOS_ERROR);
    }

    old_fs = get_fs();
    set_fs(KERNEL_DS);
/*lint -save -e151 -e529*/
    status = vfs_read(filp, (void __force __user *)pMemObj, u32Size, &pos);
    set_fs(old_fs);
    return status;
}
/*lint -restore*/
/*****************************************************************************
* �� �� ��  : diag_udi_ioctl
*
* ��������  : ����ͨ����������
*
* �������  : handle: �豸��handle
*             u32Cmd: IOCTL������
*             pParam: ��������
* �������  :
*
* �� �� ֵ  : �ɹ�/ʧ��
*****************************************************************************/
VOS_INT32 diag_udi_ioctl(UDI_HANDLE handle, BSP_U32 u32Cmd, VOID* pParam)
{
    mm_segment_t old_fs;
    struct file* filp = (struct file*)handle;
    VOS_INT32 status;

    if (unlikely((UDI_HANDLE)VOS_ERROR == handle || (UDI_HANDLE)0 == handle))
    {
        /*lint -save -e515*/
        pr_emerg("%s, invalid param\n", __FUNCTION__);
        /*lint -restore*/
        return (UDI_HANDLE)(VOS_ERROR);
    }

    old_fs = get_fs();
    set_fs(KERNEL_DS);

	printk("%s:filp=0x%x,u32Cmd=%d\n",__FUNCTION__,filp,u32Cmd);
    status = do_vfs_ioctl(filp, 0, (unsigned int)u32Cmd, (unsigned long)pParam);
    set_fs(old_fs);
    return status;
}

/*lint -restore +e40 +e63*/



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


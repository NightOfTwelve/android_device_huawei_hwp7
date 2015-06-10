/******************************************************************************

                  ��Ȩ���� (C), 2001-2013, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_nve.h
  �� �� ��   : ����
  ��������   : 2013��6��27��
  ����޸�   :
  ��������   : drv_nve.h ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��6��27��
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __DRV_NVE_H__
#define __DRV_NVE_H__
#include "drv_global.h"

#define BSP_NVE_NAME_LENGTH          8       /*NV name maximum length*/
#define BSP_NVE_NV_DATA_SIZE        104     /*NV data maximum length*/

#define BSP_NV_READ                 1       /*NV read  operation*/
#define BSP_NV_WRITE                0       /*NV write operation*/

typedef struct nve_info_stru {
	BSP_U32 nv_operation;              /*0-write,1-read*/
	BSP_U32 nv_number;                 /*NV number you want to visit*/
	BSP_S8 nv_name[BSP_NVE_NAME_LENGTH];
	BSP_U32 valid_size;
	BSP_U8 nv_data[BSP_NVE_NV_DATA_SIZE];
}NVE_INFO_S;

/*****************************************************************************
* �� �� ��  : DRV_NVE_ACCESS
*
* ��������  : ����LINUX NVE
*
* �������  : nve  ID
*
* �������  : ��
*
* �� �� ֵ  : OK& ERROR
*
*****************************************************************************/
BSP_S32 DRV_NVE_ACCESS(NVE_INFO_S *nve);


#endif


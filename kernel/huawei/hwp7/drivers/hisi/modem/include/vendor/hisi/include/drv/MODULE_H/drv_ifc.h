/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_ifc.h
  �� �� ��   : ����
  ��������   : 2013��2��2��
  ����޸�   :
  ��������   : drv_ifc.h ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��2��2��
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __DRV_IFC_H__
#define __DRV_IFC_H__
#include "drv_global.h"
/* ģ��IDö��*/
typedef enum tagIFC_MODULE_E
{
    IFC_MODULE_BSP=  0,
    IFC_MODULE_LPS=  1,
    IFC_MODULE_GUPS= 2,
    IFC_MODULE_LMSP = 3,
    IFC_MODULE_GUOM = 4,
    IFC_MODULE_BUTT
} IFC_MODULE_E;


/* IFC�ص������ṹ*/
/* pMsgBody:�ú���ID��Ӧ�����Ĳ���,ǰ���ֽ��豣֤ΪmoduleId*/
/* u32Len:pMsgBody����*/
typedef BSP_S32 (*BSP_IFC_REG_FUNC)(BSP_VOID *pMsgBody,BSP_U32 u32Len);

/*****************************************************************************
* �� �� ��  : BSP_IFC_RegFunc
*
* ��������  : ע��IFC�ص������ӿ�
*
* �������  : IFC_MODULE_E enModuleId    ģ��ID
*             BSP_IFC_REG_FUNC pFunc     �ص�����ָ��
* �������  : ��
*
* �� �� ֵ  : BSP_SUCCESS
*             BSP_ERR_MODULE_NOT_INITED
*             BSP_ERR_INVALID_PARA
*
* ����˵��  : ��
*
*****************************************************************************/
BSP_U32 BSP_IFC_RegFunc(IFC_MODULE_E enModuleId, BSP_IFC_REG_FUNC pFunc);


/*****************************************************************************
* �� �� ��  : BSP_IFC_Send
*
* ��������  : IFC������Ϣ�ӿ�
*
* �������  : BSP_VOID * pMspBody           ������Ϣ����
*             BSP_U32 u32Len               pMspBody�ĳ���
* �������  : ��
*
* �� �� ֵ  : BSP_SUCCESS
*             BSP_ERR_MODULE_NOT_INITED
*             BSP_ERR_INVALID_PARA
*             BSP_ERR_IFC_SEND_FAIL
*
* ����˵��  : ��
*
*****************************************************************************/
BSP_U32 BSP_IFC_Send(BSP_VOID * pMspBody, BSP_U32 u32Len);


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define WAITFOREVER                     (0)


/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/


/*****************************************************************************
 ʵ �� ��  : tagIFCP_FUNC_E
 ��������  : ��������˵ĺ˼亯������
             ǰ��16bit��λFUN����CPUID,����16bit���庯��FUN ID
*****************************************************************************/
typedef enum
{
    /* CCPU 0 �ϵ�FUN ID ���� */
    IFCP_FUNC_CCPU_SAMPLE_ENABLE =  0x00000,
    IFCP_FUNC_CCPU_BUTT,

    /* ACPU 1 �ϵ�FUN ID ���� */
    IFCP_FUNC_ACPU_SAMPLE_ENABLE =  0x10000,
    IFCP_FUNC_ACPU_BOARD_COMMCALL_TEST,        /* IFC �������  commcall*/
    IFCP_FUNC_ACPU_BOARD_EXTENDCALL_TEST,       /* IFC �������  extendcall*/
    IFCP_FUNC_ACPU_BUTT,

    /* GUDSP 2 �ϵ�FUN ID ���� */
    IFCP_FUNC_GUDSP_SAMPLE_ENABLE =  0x20000,
    IFCP_FUNC_GUDSP_BUTT,

    /* HIFI 3 �ϵ�FUN ID ���� */
    IFCP_FUNC_HIFI_SAMPLE_ENABLE  = 0x30000,
    IFCP_FUNC_HIFI_BUTT,

    /* MCU 4 �ϵ�FUN ID ���� */
    IFCP_FUNC_MCU_TP_ENABLE       =  0x40000,
    IFCP_FUNC_MCU_HUTAF_HLT,                   /* HUTAF HLT ���帲���� */
    IFCP_FUNC_MCU_MCA,                         /* mca interface*/
    IFCP_FUNC_MCU_BOARD_COMMCALL_TEST,        /* IFC �������  commcall*/
    IFCP_FUNC_MCU_BOARD_EXTENDCALL_TEST,       /* IFC �������  extendcall*/
    IFCP_FUNC_MCU_BUTT,

    /* BBE16 5 �ϵ�FUN ID ���� */
    IFCP_FUNC_BBE16_SAMPLE_ENABLE =  0x50000,
    IFCP_FUNC_BBE16_BUTT,

}SIFCP_FUNC_E;


/*****************************************************************************
 ʵ �� ��  : FUNCID_RELATIVE_ADDR
 ��������  : ��������˵ĺ˼亯�����ú���ID��Ե�ַ
*****************************************************************************/
typedef enum
{
    /* CCPU 0 �ϵ�FUN ID ���� */
    IFCP_FUNC_CCPU_INIT_ADDR   =  0x0,
    /* ACPU 1 �ϵ�FUN ID ���� */
    IFCP_FUNC_ACPU_INIT_ADDR   =  0x10000,
    /* GUDSP 2 �ϵ�FUN ID ���� */
    IFCP_FUNC_GUDSP_INIT_ADDR  =  0x20000,
    /* HIFI 3 �ϵ�FUN ID ���� */
    IFCP_FUNC_HIFI_INIT_ADDR   =  0x30000,
    /* MCU 4 �ϵ�FUN ID ���� */
    IFCP_FUNC_MCU_INIT_ADDR    =  0x40000,
    /* BBE16 5 �ϵ�FUN ID ���� */
    IFCP_FUNC_BBE16_INIT_ADDR  =  0x50000,
}FUNCID_RELATIVE_ADDR;


/*****************************************************************************
 ö����    : BSP_MAILBOX_IFC_CALL_FALG_ENUM
 ö��˵��  : IFC �������õĵ��÷�ʽ
*****************************************************************************/
 typedef enum
{
    BSP_MAILBOX_IFC_CALL_SYNC,        /*ͬ����0*/
    BSP_MAILBOX_IFC_CALL_ASYN,        /*�첽��1*/
    BSP_MAILBOX_IFC_CALL_RESPONSE,    /*�ظ�: 2*/
    BSP_MAILBOX_IFC_CALL_FLAG_BUTT
}BSP_MAILBOX_IFC_CALL_FALG_ENUM;

/*****************************************************************************
 ö����    : BSP_MAILBOX_IFC_REG_FUN_FLAG_ENUM
 ö��˵��  : IFC
*****************************************************************************/
typedef enum tagBSP_MAILBOX_IFC_REG_FUN_FLAG_ENUM
{
    BSP_MAILBOX_IFC_REG_FUN_FLAG_SELF            = 0,
    BSP_MAILBOX_IFC_REG_FUN_FLAG_IFC             = 1,
    BSP_MAILBOX_IFC_REG_FUN_FLAG_NONE            = 2,
    BSP_MAILBOX_IFC_REG_FUN_FLAG_BUTT
}BSP_MAILBOX_IFC_REG_FUN_FLAG_ENUM;

/*****************************************************************************
 ʵ �� ��  : BSP_MAILBOX_ERR_ENUM
 ��������  : ���������쳣ֵ
*****************************************************************************/
enum BSP_MAILBOX_ERR_ENUM
{
   BSP_MAILBOX_ERR_START                              = 0x80000000,
   BSP_MAILBOX_ERR_PARA                               = 0x80000001,
   BSP_MAILBOX_ERR_MAILBOX_ID                         = 0x80000002,
   BSP_MAILBOX_ERR_MAILBOX_WRITE                      = 0x80000003,
   BSP_MAILBOX_ERR_MAILBOX_IPC_SEND_FAIL              = 0x80000004,
   BSP_MAILBOX_ERR_MAILBOX_SEMB_CREATE                = 0x80000005,
   BSP_MAILBOX_ERR_MAILBOX_MALLOC_FAIL                = 0x80000006,
   BSP_MAILBOX_ERR_MAILBOX_TASK_CREATE                = 0x80000007,
   BSP_MAILBOX_ERR_MAILBOX_LIST_INIT                  = 0x80000008,
   BSP_MAILBOX_ERR_MAILBOX_SEND_MSG                   = 0x80000009,
   BSP_MAILBOX_ERR_MAILBOX_CALLBACK_REGISTER          = 0x8000000a,
   BSP_MAILBOX_ERR_MAILBOX_FULL                       = 0x8000000b,
   BSP_MAILBOX_ERR_MAILBOX_FID_CREATE_FAIL            = 0x8000000c,
   BSP_MAILBOX_ERR_MAILBOX_IFC_FID_CREATE_FAIL        = 0x8000000d,
   BSP_MAILBOX_ERR_MAILBOX_MSG_ALLOC_FAIL             = 0x8000000e,
   BSP_MAILBOX_ERR_MAILBOX_MSG_SEND_FAIL              = 0x8000000f,
   BSP_MAILBOX_ERR_MAILBOX_MAIL_CHECK_FAIL            = 0x80000010,
   BSP_MAILBOX_ERR_MAILBOX_INIT_NOT_OK                = 0x80000011,
   BSP_MAILBOX_ERR_MAILBOX_CALLBACK_INIT              = 0x80000012,
   BSP_MAILBOX_ERR_MAILBOX_CALLBACK_UNREGISTER        = 0x80000013,
   BSP_MAILBOX_ERR_IFC_SEMID_FULL                     = 0x80000014,
   BSP_MAILBOX_ERR_IFC_SEMID_EMPTY                    = 0x80000015,
   BSP_MAILBOX_ERR_IFC_CALLBACK_FULL                  = 0x80000016,
   BSP_MAILBOX_ERR_IFC_CALLBACK_EMPTY                 = 0x80000017,
   BSP_MAILBOX_ERR_MAILBOX_PROTECTED_WORD_FAIL        = 0x80000018,
   BSP_MAILBOX_ERR_IFC_OUT_OF_EXTENDCALL              = 0x80000019,
   BSP_MAILBOX_ERR_IFC_OUT_OF_FIVE_PARA               = 0x8000001a,
   BSP_MAILBOX_ERR_MAILBOX_OUT_OF_BUFFER              = 0x8000001b,
   BSP_MAILBOX_ERR_MAILBOX_MSG_CHECK_FAIL             = 0x8000001c,
   BSP_MAILBOX_ERR_IFC                                = 0x8000001d

};

/*****************************************************************************
 ö����    : BSP_MAILBOX_IFC_CALL_TPYE_ENUM
 ö��˵��  : IFC �������õĵ�������
*****************************************************************************/
 typedef enum
{
    BSP_MAILBOX_IFC_CALL_COMM     = 0x1,
    BSP_MAILBOX_IFC_CALL_EXTENED  = 0x2,
    BSP_MAILBOX_IFC_CALL_TYPE_BUTT,
}BSP_MAILBOX_IFC_CALL_TPYE_ENUM;

/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/

typedef void (*IFC_COMM_CALLBACK)(unsigned long fundi,long retvalue);
typedef void (*IFC_EXTEN_CALLBACK)(unsigned long fundi,unsigned char *buffer,unsigned long length);
typedef long (*IFC_REGISTER_PARA)(long Para1,long Para2,long Para3,long Para4,long Para5);
typedef long (*IFC_REGISTER_EXTEN)(unsigned char* Buffer,unsigned long Length);

/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/
extern unsigned long MB_IFC_RegFunc(unsigned long emFunid,void* pFunc, unsigned long emSelfFlag);

extern unsigned long  MB_IFC_CommCall(
             unsigned long                Funid,
             unsigned long                emSyncFlag,
             IFC_COMM_CALLBACK     CallBack,
             unsigned char*                InputPara,
             unsigned long                InputParaNumber);

extern unsigned long MB_IFC_ExtendCall(
              unsigned long                    Funid,
              unsigned long                    emSyncFlag,
              IFC_EXTEN_CALLBACK        CallBack,
              unsigned char*                    InputBuffer,
              unsigned long                    InputLen,
              unsigned char*                    OutPutBuffer,
              unsigned long*                   OutPutLen );

extern unsigned long MB_IFC_Response(
              unsigned long                SCPUID,
              unsigned long                Funid,
              unsigned long                emSyncFlag,
              void*              CallBack,
              unsigned long                CallType,
              unsigned char*                InputBuffer,
              unsigned long                InputLen,
              unsigned char*                OutputBuffer,
              unsigned long*               OutputLen );

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif


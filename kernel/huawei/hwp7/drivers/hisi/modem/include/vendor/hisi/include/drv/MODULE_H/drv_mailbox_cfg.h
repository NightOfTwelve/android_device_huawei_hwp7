/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_mailbox_cfg.h
  �� �� ��   : ����
  ��������   : 2012��6��20��
  ����޸�   :
  ��������   : ��������ڴ�/�жϵ���Դ���ýӿ�ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��6��20��
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __DRV_MAILBOX_CFG_H__
#define __DRV_MAILBOX_CFG_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "MemoryLayout.h"
#include "drv_ipcm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  2 �궨��
*****************************************************************************/

/* �ʼ����кŵĳ�ʼֵ */
#define MAILBOX_SEQNUM_START            (0)

/* ����ռ��memory����ַ */
#define MAILBOX_MEM_BASEADDR            (MEMORY_RAM_CORESHARE_MAILBOX_ADDR)

/* ����ռ��memoryԤ���ܳ���, �����������ͷ��������л��� */
#define MAILBOX_MEM_LENGTH              MEMORY_RAM_CORESHARE_MAILBOX_SIZE

/*****************************************************************************
 ʵ �� ��  : struct mb_head
 ��������  : �˼����仺��ͷ, ͨ���������ж�����ͨ����û�б���ʼ��, �˹�����Ҫ
             fastbootͬ���޸�:�ϵ��ʼ�������乲���ڴ�ͷ��������
*****************************************************************************/
typedef struct mb_head
{
    unsigned long       ulProtectWord1;     /*������ 0x55AA55AA*/
    unsigned long       ulProtectWord2;     /*������ 0x5A5A5A5A*/
    unsigned long       ulFront;            /*���д�д��Ԫ�������(����������)ͷ�ĳ��ȣ���λ32bit */
    unsigned long       ulRear;             /*���д�����Ԫ�������(����������)ͷ�ĳ��ȣ���λ32bit */
    unsigned long       ulFrontslice;       /*�������价�ζ��е�ͷָ��ϵͳʱ��*/
    unsigned long       ulRearslice;        /*�������价�ζ��е�βָ��ϵͳʱ��*/
    unsigned short      ausReserve[4];      /*����*/
    unsigned long       ulProtectWord3;     /*������ 0x55AA55AA*/
    unsigned long       ulProtectWord4;     /*������ 0x5A5A5A5A*/
} MAILBOX_HEAD_STRU;

/* ����ͷռ�ÿռ��ܳ��ȶ���*/
#define MAILBOX_HEAD_LEN                (sizeof(struct mb_head))
#define MAILBOX_MAX_CHANNEL             (30)
#define MAILBOX_MEM_HEAD_LEN            (MAILBOX_MAX_CHANNEL * MAILBOX_HEAD_LEN)

/* ���������ݶ��г��ȶ��� */
#define MAILBOX_QUEUE_SIZE(src, dst, channel) \
    MAILBOX_QUEUE_SIZE_##src##2##dst##_##channel
enum MAILBOX_QUEUE_SIZE_ENUM
{
    /* ���¸�ö�����������, ����: MAILBOX_QUEUE_SIZE_MCU2ACPU_DEFAULT */
    MAILBOX_QUEUE_SIZE(MCU,  ACPU, MSG) = 0x00002000,
    MAILBOX_QUEUE_SIZE(ACPU, MCU,  MSG) = 0x00002000,
    MAILBOX_QUEUE_SIZE(MCU, ACPU, IFC)  = 0x00002000,
    MAILBOX_QUEUE_SIZE(ACPU, MCU, IFC)  = 0x00002000,

    MAILBOX_QUEUE_SIZE(MCU,  CCPU, MSG) = 0x00002000,
    MAILBOX_QUEUE_SIZE(CCPU, MCU,  MSG) = 0x00002000,
    MAILBOX_QUEUE_SIZE(MCU, CCPU, IFC)  = 0x00002000,
    MAILBOX_QUEUE_SIZE(CCPU, MCU, IFC)  = 0x00002000,

    MAILBOX_QUEUE_SIZE(ACPU, CCPU, MSG) = 0x00008000,
    MAILBOX_QUEUE_SIZE(CCPU, ACPU, MSG) = 0x00008000,
    MAILBOX_QUEUE_SIZE(ACPU, CCPU, IFC) = 0x00008000,
    MAILBOX_QUEUE_SIZE(CCPU, ACPU, IFC) = 0x00008000,

    MAILBOX_QUEUE_SIZE(ACPU, HIFI, MSG) = 0x00001800,
    MAILBOX_QUEUE_SIZE(HIFI, ACPU, MSG) = 0x00001800,

    MAILBOX_QUEUE_SIZE(CCPU, HIFI, MSG) = 0x00001800,
    MAILBOX_QUEUE_SIZE(HIFI, CCPU, MSG) = 0x00005000,

    MAILBOX_QUEUE_SIZE(BBE16, HIFI, MSG) = 0x00001800,
    MAILBOX_QUEUE_SIZE(HIFI, BBE16, MSG) = 0x00001800
};

/* ���������ͷ��ַ���� */
#define MAILBOX_HEAD_ADDR(src, dst, channel) \
    MAILBOX_HEAD_ADDR_##src##2##dst##_##channel
enum MAILBOX_HEAD_ADDR_ENUM
{
    /* ���¸�ö�����������, ����: MAILBOX_HEAD_ADDR_MCU2ACPU_DEFAULT */
    MAILBOX_HEAD_ADDR(MCU,  ACPU, MSG)  = MAILBOX_MEM_BASEADDR,
    MAILBOX_HEAD_ADDR(ACPU, MCU,  MSG)  = MAILBOX_HEAD_ADDR(MCU,  ACPU, MSG) + MAILBOX_HEAD_LEN,
    MAILBOX_HEAD_ADDR(ACPU, HIFI, MSG)  = MAILBOX_HEAD_ADDR(ACPU, MCU,  MSG) + MAILBOX_HEAD_LEN,
    MAILBOX_HEAD_ADDR(HIFI, ACPU, MSG)  = MAILBOX_HEAD_ADDR(ACPU, HIFI, MSG) + MAILBOX_HEAD_LEN,
    MAILBOX_HEAD_ADDR(MCU,  CCPU, MSG)  = MAILBOX_HEAD_ADDR(HIFI, ACPU, MSG) + MAILBOX_HEAD_LEN,
    MAILBOX_HEAD_ADDR(CCPU, MCU,  MSG)  = MAILBOX_HEAD_ADDR(MCU,  CCPU, MSG) + MAILBOX_HEAD_LEN,
    MAILBOX_HEAD_ADDR(CCPU, HIFI, MSG)  = MAILBOX_HEAD_ADDR(CCPU, MCU,  MSG) + MAILBOX_HEAD_LEN,
    MAILBOX_HEAD_ADDR(HIFI, CCPU, MSG)  = MAILBOX_HEAD_ADDR(CCPU, HIFI, MSG) + MAILBOX_HEAD_LEN,

    MAILBOX_HEAD_ADDR(ACPU, CCPU, MSG)      = MAILBOX_HEAD_ADDR(HIFI, CCPU, MSG) + MAILBOX_HEAD_LEN,
    MAILBOX_HEAD_ADDR(CCPU, ACPU, MSG)      = MAILBOX_HEAD_ADDR(ACPU, CCPU, MSG) + MAILBOX_HEAD_LEN,
    MAILBOX_HEAD_ADDR(CCPU, ACPU, IFC)      = MAILBOX_HEAD_ADDR(CCPU, ACPU, MSG) + MAILBOX_HEAD_LEN,
    MAILBOX_HEAD_ADDR(ACPU, CCPU, IFC)      = MAILBOX_HEAD_ADDR(CCPU, ACPU, IFC) + MAILBOX_HEAD_LEN,

    MAILBOX_HEAD_ADDR(CCPU, MCU, IFC)      = MAILBOX_HEAD_ADDR(ACPU, CCPU, IFC) + MAILBOX_HEAD_LEN,
    MAILBOX_HEAD_ADDR(MCU, CCPU, IFC)      = MAILBOX_HEAD_ADDR(CCPU, MCU, IFC) + MAILBOX_HEAD_LEN,

    MAILBOX_HEAD_ADDR(ACPU, MCU, IFC)      = MAILBOX_HEAD_ADDR(MCU, CCPU, IFC) + MAILBOX_HEAD_LEN,
    MAILBOX_HEAD_ADDR(MCU, ACPU, IFC)      = MAILBOX_HEAD_ADDR(ACPU, MCU, IFC) + MAILBOX_HEAD_LEN,

    MAILBOX_HEAD_ADDR(BBE16, HIFI, MSG)  = MAILBOX_HEAD_ADDR(MCU, ACPU,  IFC) + MAILBOX_HEAD_LEN,
    MAILBOX_HEAD_ADDR(HIFI, BBE16, MSG)  = MAILBOX_HEAD_ADDR(BBE16, HIFI, MSG) + MAILBOX_HEAD_LEN,

    MAILBOX_HEAD_BOTTOM_ADDR                = MAILBOX_HEAD_ADDR(HIFI, BBE16, MSG) + MAILBOX_HEAD_LEN

};

/* ���������ݶ��е�ַ���� */
#define MAILBOX_QUEUE_ADDR(src, dst, channel) \
    MAILBOX_QUEUE_ADDR_##src##2##dst##_##channel
#define MAILBOX_QUEUE_BOTTOM_ADDR(src, dst, channel) \
    (MAILBOX_QUEUE_ADDR(src, dst, channel) + MAILBOX_QUEUE_SIZE(src, dst, channel))
enum MAILBOX_QUEUE_ADDR_ENUM
{
    /* ���¸�ö�����������, ����: MAILBOX_QUEUE_ADDR_MCU2ACPU_DEFAULT */
    MAILBOX_QUEUE_ADDR(MCU,  ACPU, MSG) = MAILBOX_MEM_BASEADDR + MAILBOX_MEM_HEAD_LEN,
    MAILBOX_QUEUE_ADDR(ACPU, MCU,  MSG) = MAILBOX_QUEUE_BOTTOM_ADDR(MCU,  ACPU, MSG),
    MAILBOX_QUEUE_ADDR(ACPU, HIFI, MSG) = MAILBOX_QUEUE_BOTTOM_ADDR(ACPU, MCU,  MSG),
    MAILBOX_QUEUE_ADDR(HIFI, ACPU, MSG) = MAILBOX_QUEUE_BOTTOM_ADDR(ACPU, HIFI, MSG),
    MAILBOX_QUEUE_ADDR(MCU,  CCPU, MSG) = MAILBOX_QUEUE_BOTTOM_ADDR(HIFI, ACPU, MSG) ,
    MAILBOX_QUEUE_ADDR(CCPU, MCU,  MSG) = MAILBOX_QUEUE_BOTTOM_ADDR(MCU,  CCPU, MSG),
    MAILBOX_QUEUE_ADDR(CCPU, HIFI, MSG) = MAILBOX_QUEUE_BOTTOM_ADDR(CCPU, MCU,  MSG),
    MAILBOX_QUEUE_ADDR(HIFI, CCPU, MSG) = MAILBOX_QUEUE_BOTTOM_ADDR(CCPU, HIFI, MSG),

    MAILBOX_QUEUE_ADDR(ACPU, CCPU, MSG)  = MAILBOX_QUEUE_BOTTOM_ADDR(HIFI, CCPU, MSG),
    MAILBOX_QUEUE_ADDR(CCPU, ACPU, MSG)  = MAILBOX_QUEUE_BOTTOM_ADDR(ACPU, CCPU, MSG),
    MAILBOX_QUEUE_ADDR(CCPU, ACPU, IFC)  = MAILBOX_QUEUE_BOTTOM_ADDR(CCPU, ACPU, MSG),
    MAILBOX_QUEUE_ADDR(ACPU, CCPU, IFC)  = MAILBOX_QUEUE_BOTTOM_ADDR(CCPU, ACPU, IFC),

    MAILBOX_QUEUE_ADDR(CCPU, MCU, IFC)   = MAILBOX_QUEUE_BOTTOM_ADDR(ACPU, CCPU, IFC),
    MAILBOX_QUEUE_ADDR(MCU, CCPU, IFC)   = MAILBOX_QUEUE_BOTTOM_ADDR(CCPU, MCU, IFC),
    MAILBOX_QUEUE_ADDR(ACPU, MCU, IFC)   = MAILBOX_QUEUE_BOTTOM_ADDR(MCU, CCPU, IFC),
    MAILBOX_QUEUE_ADDR(MCU, ACPU, IFC)   = MAILBOX_QUEUE_BOTTOM_ADDR(ACPU, MCU, IFC),

    MAILBOX_QUEUE_ADDR(BBE16, HIFI, MSG) = MAILBOX_QUEUE_BOTTOM_ADDR(MCU, ACPU, IFC),
    MAILBOX_QUEUE_ADDR(HIFI, BBE16, MSG) = MAILBOX_QUEUE_BOTTOM_ADDR(BBE16, HIFI, MSG),

    MAILBOX_MEMORY_BOTTOM_ADDR           = MAILBOX_QUEUE_BOTTOM_ADDR(HIFI, BBE16, MSG)
};

/*�����ڴ��еı����ֶ���*/
#define MAILBOX_PROTECT1                (0x55AA55AA)    /* ���䱣����1����������ͷ������, ͬʱ��ʾ����ճ�ʼ��û�н��չ�����*/
#define MAILBOX_PROTECT2                (0x5A5A5A5A)    /* ���䱣����2����������ͷ������ */
#define MAILBOX_PROTECT_LEN             (sizeof(long))  /* ���䱣���ֳ��ȣ���λbyte������ͷ������ͷ��β�������������ı����� */
#define MAILBOX_MSGHEAD_NUMBER          (0xA5A5A5A5)    /* �����Ϣ��Ϣ�ָ��� */

#define MAILBOX_DATA_BASE_PROTECT_NUM         (2)    /* ����ͨ�������ڴ����������׵�ַ��ʼ�ı����ָ���*/
#define MAILBOX_DATA_TAIL_PROTECT_NUM         (2)    /* ����ͨ�������ڴ���������β��ַ�����ı����ָ���*/

/* ����ͨ�������ڴ������������а����ı����ֵĸ����������׵�ַ��ʼ��2�������֣���ĩ��ַ������2��������*/
#define MAILBOX_DATA_LEN_PROTECT_NUM          (MAILBOX_DATA_BASE_PROTECT_NUM + MAILBOX_DATA_TAIL_PROTECT_NUM)


/* hifi�����ַ��Ϣ������ */
#define HIFI_MB_ADDR_PROTECT            (0x5a5a5a5a)

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

/*****************************************************************************
  ������������֮���IPC�жϺ�
*****************************************************************************/
#define MAILBOX_IPC_INT_NUM(src, dst, channel) \
    MAILBOX_IPC_INT_##src##2##dst##_##channel
enum IPC_MAILBOX_INT_ENUM
{
    /* ���¸�ö�����������, ����: MAILBOX_IPC_INT_CCPU2MCU_DEFAULT */
    MAILBOX_IPC_INT_NUM(CCPU, MCU, MSG)  = IPC_MCU_INT_SRC_CCPU_MSG,
    MAILBOX_IPC_INT_NUM(MCU, CCPU, MSG)  = IPC_CCPU_INT_SRC_MCU_MSG,

    MAILBOX_IPC_INT_NUM(CCPU, HIFI, MSG) = IPC_HIFI_INT_SRC_CCPU_MSG,
    MAILBOX_IPC_INT_NUM(HIFI, CCPU, MSG) = IPC_CCPU_INT_SRC_HIFI_MSG,

    MAILBOX_IPC_INT_NUM(ACPU, MCU, MSG)  = IPC_MCU_INT_SRC_ACPU_MSG,
    MAILBOX_IPC_INT_NUM(MCU, ACPU, MSG)  = IPC_ACPU_INT_SRC_MCU_MSG,

    MAILBOX_IPC_INT_NUM(ACPU, HIFI, MSG) = IPC_HIFI_INT_SRC_ACPU_MSG,
    MAILBOX_IPC_INT_NUM(HIFI, ACPU, MSG) = IPC_ACPU_INT_SRC_HIFI_MSG,

    MAILBOX_IPC_INT_NUM(HIFI, MCU, MSG)  = IPC_MCU_INT_SRC_HIFI_MSG,
    MAILBOX_IPC_INT_NUM(MCU, HIFI, MSG)  = IPC_HIFI_INT_SRC_MCU_MSG,

    MAILBOX_IPC_INT_NUM(CCPU, ACPU, MSG)    = IPC_ACPU_INT_SRC_CCPU_MSG,
    MAILBOX_IPC_INT_NUM(ACPU, CCPU, MSG)    = IPC_CCPU_INT_SRC_ACPU_MSG,

    MAILBOX_IPC_INT_NUM(CCPU, ACPU, IFC)    = IPC_ACPU_INT_SRC_CCPU_IFC,
    MAILBOX_IPC_INT_NUM(ACPU, CCPU, IFC)    = IPC_CCPU_INT_SRC_ACPU_IFC,

    MAILBOX_IPC_INT_NUM(CCPU, MCU, IFC)    = IPC_MCU_INT_SRC_CCPU_IFC,
    MAILBOX_IPC_INT_NUM(MCU, CCPU, IFC)    = IPC_CCPU_INT_SRC_MCU_IFC,

    MAILBOX_IPC_INT_NUM(ACPU, MCU, IFC)    = IPC_MCU_INT_SRC_ACPU_IFC,
    MAILBOX_IPC_INT_NUM(MCU, ACPU, IFC)    = IPC_ACPU_INT_SRC_MCU_IFC,

    MAILBOX_IPC_INT_NUM(BBE16, HIFI, MSG) = IPC_HIFI_INT_SRC_BBE_MSG,
    MAILBOX_IPC_INT_NUM(HIFI, BBE16, MSG) = IPC_BBE16_INT_SRC_HIFI_MSG
};

/* ���������ݵ����ʼ���󳤶ȶ��� */
#define MAILBOX_MAILSIZE_MAX(src, dst, channel) \
    MAILBOX_MAILSIZE_MAX_##src##2##dst##_##channel
enum MAILBOX_MAILSIZE_MAX_ENUM
{
    /* ���¸�ö�����������, ����: MAILBOX_MAILSIZE_MAX_MCU2ACPU_DEFAULT */
    MAILBOX_MAILSIZE_MAX(MCU,  ACPU ,MSG)  = MAILBOX_QUEUE_SIZE(MCU,  ACPU ,MSG)/4, //DOTO:����ֵ��Ҫ����
    MAILBOX_MAILSIZE_MAX(ACPU, MCU  ,MSG)  = MAILBOX_QUEUE_SIZE(ACPU, MCU  ,MSG)/4, //DOTO:����ֵ��Ҫ����
    MAILBOX_MAILSIZE_MAX(ACPU, HIFI ,MSG)  = MAILBOX_QUEUE_SIZE(ACPU ,HIFI ,MSG)/4, //DOTO:����ֵ��Ҫ����
    MAILBOX_MAILSIZE_MAX(HIFI, ACPU ,MSG)  = MAILBOX_QUEUE_SIZE(HIFI ,ACPU ,MSG)/4, //DOTO:����ֵ��Ҫ����
    MAILBOX_MAILSIZE_MAX(MCU , CCPU ,MSG)  = MAILBOX_QUEUE_SIZE(MCU  ,CCPU ,MSG)/4,//DOTO:����ֵ��Ҫ����
    MAILBOX_MAILSIZE_MAX(CCPU, MCU  ,MSG)  = MAILBOX_QUEUE_SIZE(CCPU ,MCU  ,MSG)/4,//DOTO:����ֵ��Ҫ����
    MAILBOX_MAILSIZE_MAX(CCPU, HIFI ,MSG)  = MAILBOX_QUEUE_SIZE(CCPU ,HIFI ,MSG)/4,//DOTO:����ֵ��Ҫ����
    MAILBOX_MAILSIZE_MAX(HIFI, CCPU ,MSG)  = MAILBOX_QUEUE_SIZE(HIFI ,CCPU ,MSG)/4,//DOTO:����ֵ��Ҫ����

    MAILBOX_MAILSIZE_MAX(CCPU, ACPU ,MSG)  = MAILBOX_QUEUE_SIZE(CCPU, ACPU ,MSG)/4,//DOTO:����ֵ��Ҫ����
    MAILBOX_MAILSIZE_MAX(ACPU, CCPU ,MSG)  = MAILBOX_QUEUE_SIZE(ACPU, CCPU ,MSG)/4,//DOTO:����ֵ��Ҫ����
    MAILBOX_MAILSIZE_MAX(CCPU, ACPU ,IFC)  = MAILBOX_QUEUE_SIZE(CCPU, ACPU ,IFC)/4,//DOTO:����ֵ��Ҫ����
    MAILBOX_MAILSIZE_MAX(ACPU, CCPU ,IFC)  = MAILBOX_QUEUE_SIZE(ACPU, CCPU ,IFC)/4,//DOTO:����ֵ��Ҫ����

    MAILBOX_MAILSIZE_MAX(CCPU, MCU ,IFC)  = MAILBOX_QUEUE_SIZE(CCPU, MCU ,IFC)/4,//DOTO:����ֵ��Ҫ����
    MAILBOX_MAILSIZE_MAX(MCU, CCPU ,IFC)  = MAILBOX_QUEUE_SIZE(MCU, CCPU ,IFC)/4,//DOTO:����ֵ��Ҫ����

    MAILBOX_MAILSIZE_MAX(ACPU, MCU ,IFC)  = MAILBOX_QUEUE_SIZE(ACPU, MCU ,IFC)/4,//DOTO:����ֵ��Ҫ����
    MAILBOX_MAILSIZE_MAX(MCU, ACPU ,IFC)  = MAILBOX_QUEUE_SIZE(MCU, ACPU ,IFC)/4,//DOTO:����ֵ��Ҫ����

    MAILBOX_MAILSIZE_MAX(BBE16, HIFI ,MSG) = MAILBOX_QUEUE_SIZE(BBE16 ,HIFI ,MSG)/4,//DOTO:����ֵ��Ҫ����
    MAILBOX_MAILSIZE_MAX(HIFI, BBE16 ,MSG) = MAILBOX_QUEUE_SIZE(HIFI ,BBE16 ,MSG)/4,//DOTO:����ֵ��Ҫ����
};

/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/

/*****************************************************************************
 ʵ �� ��  : struct mb_mail
 ��������  : �˼��ʼ�ͷ
*****************************************************************************/
typedef struct mb_mail
{
    unsigned long       ulPartition;    /*��Ϣ�ָ��� 0xA5A5A5A5*/
    unsigned long       ulWriteSlice;   /*��Ϣд��ϵͳʱ����Ϣ */
    unsigned long       ulReadSlice;    /*��Ϣ����ϵͳʱ����Ϣ */
    unsigned long       ulSeqNum;       /*��Ϣ���к� */
    unsigned long       ulPriority;     /*��Ϣ���ȼ�����������ͨ, ����*/
    unsigned long       ulMailCode;     /*�ʱ�*/
    unsigned long       ausReserve[2];  /*����*/
    unsigned long       ulMsgLength;    /*��ulMsgLength�ֶ�(��������)�������Ϣ����*/
} MAILBOX_MSG_HEADER;

/************************************************************************
 �ṹ��    : IPC_MAILBOX_QUEUE_STRU
 �ṹ˵��  : �˼����仺��
 ************************************************************************/
/* ����ṹʾ��:
typedef struct
{
    unsigned long      ulProtectWord1;     //������ 0x55aa55aa
    unsigned long      ulProtectWord2;     //������ 0a5a5a5a5a
    unsigned long      aulMsgQueue[���г���-4];//���ζ��д�����ɷ��ʼ�
    unsigned long      ulProtectWord3;     //������ 0x55aa55aa
    unsigned long      ulProtectWord4;     //������ 0x5a5a5a5a
} MAILBOX_QUEUE_STRU;
*/

/*****************************************************************************
 �ṹ��    : AARM_HIFI_DYN_ADDR_SHARE_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  :AARM��HIFI�Ĺ����ַ��OAM��Ҫ��ֵ��������Hifi���������е���Ӧ�ṹ��
*****************************************************************************/
typedef struct
{
    unsigned long uwProtectWord;                /*0x5a5a5a5a*/
    unsigned long uwHifi2AarmMailBoxLen;        /* hifi��Aarm������䳤�� */
    unsigned long uwAarm2HifiMailBoxLen;        /* Aarm��hifi������䳤�� */
    unsigned long uwHifiAarmHeadAddr;
    unsigned long uwHifiAarmBodyAddr;
    unsigned long uwAarmHifiHeadAddr;
    unsigned long uwAarmHifiBodyAddr;
    unsigned long uwReserved[2];
}AARM_HIFI_MAILBOX_STRU;

/*****************************************************************************
 �ṹ��    : CARM_HIFI_DYN_ADDR_SHARE_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  :CARM��HIFI�Ĺ����ַ��OAM��Ҫ��ֵ��������Hifi���������е���Ӧ�ṹ��
*****************************************************************************/
typedef struct
{
    unsigned long uwProtectWord;                /*0x5a5a5a5a*/
    unsigned long uwHifi2CarmMailBoxLen;        /* hifi��Carm������䳤�� */
    unsigned long uwCarm2HifiMailBoxLen;        /* Carm��hifi������䳤�� */
    unsigned long uwHifiCarmHeadAddr;
    unsigned long uwHifiCarmBodyAddr;
    unsigned long uwCarmHifiHeadAddr;
    unsigned long uwCarmHifiBodyAddr;
    unsigned long uwReserved[2];
}CARM_HIFI_MAILBOX_STRU;

/*****************************************************************************
 �ṹ��    : CARM_HIFI_DYN_ADDR_SHARE_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  :CARM��HIFI�Ĺ����ַ��OAM��Ҫ��ֵ��������Hifi���������е���Ӧ�ṹ��
*****************************************************************************/
typedef struct
{
    unsigned long uwProtectWord;            /*0x5a5a5a5a*/
    CARM_HIFI_MAILBOX_STRU stCarmHifiMB;    /*CARM��HIFI�����乲��������*/
    AARM_HIFI_MAILBOX_STRU stAarmHifiMB;    /*AARM��HIFI�����乲��������*/
    unsigned long uwNvBaseAddrPhy;          /*DDR��NV�������׵�ַ�������ַ*/
    unsigned long uwNvBaseAddrVirt;         /*DDR��NV�������׵�ַ��ARM�����ַ*/
    unsigned long uwReserved[2];
}CARM_HIFI_DYN_ADDR_SHARE_STRU;

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


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of drv_mailbox_cfg.h */


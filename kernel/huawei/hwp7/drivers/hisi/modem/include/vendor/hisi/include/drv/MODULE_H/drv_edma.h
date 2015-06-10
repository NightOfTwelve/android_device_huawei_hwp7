/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_edma.h
  �� �� ��   : ����
  ����޸�   :
  ��������   : drv_edma.h ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��2��2��
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __DRV_EDMA_H__
#define __DRV_EDMA_H__
#include "drv_global.h"

typedef void (*channel_isr)(BSP_U32 channel_arg, BSP_U32 int_status);
#if 0
/*�������������*/
typedef enum _BALONG_DMA_REQ
{
    /* BBITƽ̨ʹ��*/
    EDMA_SCI_RX = 0,
    EDMA_SCI_TX,
    EDMA_HSUART_RX,
    EDMA_HSUART_TX,
    EDMA_BBP_GRIF = 6,     /* BBP grif */

    /* ����ΪSFTƽ̨ʹ�� */
    EDMA_SPI0_RX = 0,      /* v9r1 ��������CMMB ��v3r3 ����SSP��Rx DMA����*/
    EDMA_SPI0_TX,
    EDMA_SPI1_RX,            /* v9r1����Touch Panel��Key Pad/����, v3r3 ΪHS_SPI����*/
    EDMA_SPI1_TX,
    EDMA_UART1_RX,         /* v9r1����Felica��NFC) ��v3r3 ����UART0 �� Rx DMA����*/
    EDMA_UART1_TX,
    EDMA_UART2_RX,         /* v9r1��������GPS �� v3r3 ����UART1  �� Rx DMA����*/
    EDMA_UART2_TX,
    EDMA_UART3_RX,         /* v9r1��������BT ,v3r3 ����HSUART0 �� Rx DMA����*/
    EDMA_UART3_TX,
    EDMA_UART4_RX,         /* v9r1��������CDMA ,v3r3 ����HSUART1�� Rx DMA����*/
    EDMA_UART4_TX,
    EDMA_UART5_RX,         /* ��������CDMA */
    EDMA_UART5_TX,
    EDMA_SCI0_RX = 14,     /* ˫��˫�� ,v3r3 ����sim����RX DMA����*/
    EDMA_SCI0_TX,
    EDMA_SCI1_RX,
    EDMA_SCI1_TX,
    EDMA_SIO0_RX = 18,     /* v9r1������Ƶ�ӿ� ��v3r3 ����SIO��Rx DMA����*/
    EDMA_SIO0_TX,
    EDMA_SIO1_RX,
    EDMA_SIO1_TX,
    EDMA_SIO2_RX,
    EDMA_SIO2_TX,
    EDMA_SLIMBUS4,
    EDMA_SLIMBUS5,
    EDMA_SLIMBUS6,
    EDMA_SLIMBUS7,
    EDMA_SLIMBUS8,
    EDMA_SLIMBUS9,
    EDMA_SLIMBUS10,
    EDMA_UICC,                   /* ������SIM�� */
    EDMA_MEMORY = 32,      /* 32,M2M�������������� */
    EDMA_MEMORY_DSP,      /* ΪDSP�����M2M�������������� */
    /*v3r3ʹ��begin*/
    EDMA_MMC0,
    EDMA_MMC1,
    EDMA_I2C_RX,
    EDMA_I2C_TX,
    /*v3r3ʹ��end*/
    EDMA_REQ_MAX      /* ����豸����С�ڴ�ֵ,��Ϊ�Ƿ����� */
} BALONG_DMA_REQ;
#endif
typedef enum _BALONG_DMA_REQ
{
    EDMA_SPI0_RX,
    EDMA_SPI0_TX,
    EDMA_SPI1_RX,
    EDMA_SPI1_TX,
    EDMA_UART1_RX,
    EDMA_UART1_TX,
    EDMA_UART2_RX,
    EDMA_UART2_TX,
    EDMA_UART3_RX,
    EDMA_UART3_TX,
    EDMA_UART4_RX,
    EDMA_UART4_TX,
    EDMA_UART5_RX,
    EDMA_UART5_TX,
    EDMA_SCI0_RX,
    EDMA_SCI0_TX,
    EDMA_SCI1_RX,
    EDMA_SCI1_TX,
    EDMA_SIO0_RX,
    EDMA_SIO0_TX,
    EDMA_SIO1_RX,
    EDMA_SIO1_TX,
    EDMA_SIO2_RX,
    EDMA_SIO2_TX,
    EDMA_SLIMBUS4,
    EDMA_SLIMBUS5,
    EDMA_SLIMBUS6,
    EDMA_SLIMBUS7,
    EDMA_SLIMBUS8,
    EDMA_SLIMBUS9,
    EDMA_SLIMBUS10,
    EDMA_UICC,
    EDMA_MEMORY_1,
    EDMA_MEMORY_2,
    EDMA_MEMORY_3,
    EDMA_MEMORY_4,
    EDMA_MEMORY_DSP_1,
    EDMA_MEMORY_DSP_2,
    EDMA_MEMORY_DSP_3,
    EDMA_MEMORY_DSP_4,
    EDMA_MEMORY_DSP_5,
    EDMA_MEMORY_DSP_6,
    EDMA_MEMORY_DSP_7,
    EDMA_MEMORY_DSP_8,
    EDMA_BBP_GRIF,
    EDMA_BBP_DBG,
    EDMA_MMC0,
    EDMA_MMC1,
    EDMA_I2C_RX,
    EDMA_I2C_TX,
    EDMA_LOG_CHAN_RESERVED
} BALONG_DMA_REQ;

/* ����void (*channel_isr)(BSP_U32 channel_arg, BSP_U32 int_status)�Ĳ���int_status��
    ����int balong_dma_channel_init (BALONG_DMA_REQ req,  channel_isr pFunc,
                     UINT32 channel_arg, UINT32 int_flag)�Ĳ���int_flag
    Ϊ���¼����ж����ͣ������       */
#define BALONG_DMA_INT_DONE           1          /*DMA��������ж�*/
#define BALONG_DMA_INT_LLT_DONE       2          /*��ʽDMA�ڵ㴫������ж�*/
#define BALONG_DMA_INT_CONFIG_ERR     4          /*DMA���ô����µ��ж�*/
#define BALONG_DMA_INT_TRANSFER_ERR   8          /*DMA��������µ��ж�*/
#define BALONG_DMA_INT_READ_ERR       16         /*DMA����������µ��ж�*/

/* EDMAC���䷽����*/
#define BALONG_DMA_P2M      1
#define BALONG_DMA_M2P      2
#define BALONG_DMA_M2M      3

/* EDMAC�������봫������*/
typedef enum tagEDMA_TRANS_TYPE
{
    MEM_MEM_DMA = 0x00,    /* �ڴ浽�ڴ棬DMA����*/
    MEM_PRF_DMA = 0x01,        /* �ڴ������裬DMA����*/
    MEM_PRF_PRF = 0x10        /* �ڴ������裬��������*/
} EDMA_TRANS_TYPE;

/* ͨ��״̬ */

#define   EDMA_CHN_FREE          1   /* ͨ������ */
#define   EDMA_CHN_BUSY          0   /* ͨ��æ */


/* EDMA����λ��Դ��Ŀ�ĵ�ַԼ��Ϊһ�µ�ֵ */
#define   EDMA_TRANS_WIDTH_8       0x0   /* 8bitλ��*/
#define   EDMA_TRANS_WIDTH_16      0x1   /* 16bitλ��*/
#define   EDMA_TRANS_WIDTH_32      0x2   /* 32bitλ��*/
#define   EDMA_TRANS_WIDTH_64      0x3   /* 64bitλ��*/

/*  EDMA burst length, ȡֵ��Χ0~15����ʾ��burst����Ϊ1~16*/
#define   EDMA_BUR_LEN_1    0x0    /* burst���ȣ���һ�δ���ĸ���Ϊ1��*/
#define   EDMA_BUR_LEN_2    0x1    /* burst���ȣ���һ�δ���ĸ���Ϊ2��*/
#define   EDMA_BUR_LEN_3    0x2   /* burst���ȣ���һ�δ���ĸ���Ϊ3��*/
#define   EDMA_BUR_LEN_4    0x3   /* burst���ȣ���һ�δ���ĸ���Ϊ4��*/
#define   EDMA_BUR_LEN_5    0x4   /* burst���ȣ���һ�δ���ĸ���Ϊ5��*/
#define   EDMA_BUR_LEN_6    0x5   /* burst���ȣ���һ�δ���ĸ���Ϊ6��*/
#define   EDMA_BUR_LEN_7    0x6   /* burst���ȣ���һ�δ���ĸ���Ϊ7��*/
#define   EDMA_BUR_LEN_8    0x7   /* burst���ȣ���һ�δ���ĸ���Ϊ8��*/
#define   EDMA_BUR_LEN_9    0x8   /* burst���ȣ���һ�δ���ĸ���Ϊ9��*/
#define   EDMA_BUR_LEN_10   0x9   /* burst���ȣ���һ�δ���ĸ���Ϊ10��*/
#define   EDMA_BUR_LEN_11   0xa   /* burst���ȣ���һ�δ���ĸ���Ϊ11��*/
#define   EDMA_BUR_LEN_12   0xb   /* burst���ȣ���һ�δ���ĸ���Ϊ12��*/
#define   EDMA_BUR_LEN_13   0xc   /* burst���ȣ���һ�δ���ĸ���Ϊ13��*/
#define   EDMA_BUR_LEN_14   0xd   /* burst���ȣ���һ�δ���ĸ���Ϊ14��*/
#define   EDMA_BUR_LEN_15   0xe   /* burst���ȣ���һ�δ���ĸ���Ϊ15��*/
#define   EDMA_BUR_LEN_16   0xf   /* burst���ȣ���һ�δ���ĸ���Ϊ16��*/


/* EDMA ��Ӧ�ľ���λ����EDMA  �Ĵ������ú�
       EDMAC_BASIC_CONFIG��BALONG_DMA_SET_LLI��BALONG_DMA_SET_CONFIG ʹ��*/
#define EDMAC_TRANSFER_CONFIG_SOUR_INC      (0X80000000)
#define EDMAC_TRANSFER_CONFIG_DEST_INC      (0X40000000)

#define EDMAC_TRANSFER_CONFIG_SOUR_BURST_LENGTH( _len )  ((BSP_U32)((_len)<<24))
#define EDMAC_TRANSFER_CONFIG_DEST_BURST_LENGTH( _len )  ((BSP_U32)((_len)<<20))

#define EDMAC_TRANSFER_CONFIG_SOUR_WIDTH( _len )  ((BSP_U32)((_len)<<16))
#define EDMAC_TRANSFER_CONFIG_DEST_WIDTH( _len )  ((BSP_U32)((_len)<<12))

#define EDMAC_TRANSFER_CONFIG_REQUEST( _ulReg )    ( (_ulReg ) << 4)
#define EDMAC_TRANSFER_CONFIG_FLOW_DMAC( _len )    ((BSP_U32)((_len)<<2))

#define EDMAC_TRANSFER_CONFIG_INT_TC_ENABLE            ( 0x2 )
#define EDMAC_TRANSFER_CONFIG_INT_TC_DISABLE           ( 0x0 )

#define EDMAC_TRANSFER_CONFIG_CHANNEL_ENABLE           ( 0x1 )
#define EDMAC_TRANSFER_CONFIG_CHANNEL_DISABLE          ( 0x0 )

#define EDMAC_TRANSFER_CONFIG_EXIT_ADD_MODE_A_SYNC     ( 0UL )
#define EDMAC_TRANSFER_CONFIG_EXIT_ADD_MODE_AB_SYNC    ( 0x00008000 )

#define EDMAC_NEXT_LLI_ENABLE       0x2           /* Bit 1 */


#ifdef WIN32
#ifndef __attribute__
#define __attribute__(p)
#endif
#endif

/*��ʽ����ʱ�Ľڵ���Ϣ*/
typedef struct _BALONG_DMA_CB
{
    volatile BSP_U32 lli;     /*ָ���¸�LLI*/
    volatile BSP_U32 bindx;
    volatile BSP_U32 cindx;
    volatile BSP_U32 cnt1;
    volatile BSP_U32 cnt0;   /*�鴫�����LLI�����ÿ���ڵ����ݳ��� <= 65535�ֽ�*/
    volatile BSP_U32 src_addr; /*�����ַ*/
    volatile BSP_U32 des_addr; /*�����ַ*/
    volatile BSP_U32 config;
} BALONG_DMA_CB __attribute__ ((aligned (32)));

#define P2M_CONFIG   (EDMAC_TRANSFER_CONFIG_FLOW_DMAC(MEM_PRF_DMA) | EDMAC_TRANSFER_CONFIG_DEST_INC)
#define M2P_CONFIG   (EDMAC_TRANSFER_CONFIG_FLOW_DMAC(MEM_PRF_DMA) | EDMAC_TRANSFER_CONFIG_SOUR_INC)
#define M2M_CONFIG   (EDMAC_TRANSFER_CONFIG_FLOW_DMAC(MEM_MEM_DMA) | EDMAC_TRANSFER_CONFIG_SOUR_INC | EDMAC_TRANSFER_CONFIG_DEST_INC)

#define EDMAC_BASIC_CONFIG(burst_width, burst_len) \
               ( EDMAC_TRANSFER_CONFIG_SOUR_BURST_LENGTH(burst_len) | EDMAC_TRANSFER_CONFIG_DEST_BURST_LENGTH(burst_len) \
               | EDMAC_TRANSFER_CONFIG_SOUR_WIDTH(burst_width) | EDMAC_TRANSFER_CONFIG_DEST_WIDTH(burst_width) )

#define BALONG_DMA_SET_LLI(addr, last)   ((last)?0:(EDMAC_MAKE_LLI_ADDR(addr) | EDMAC_NEXT_LLI_ENABLE))

/**************************************************************************
  �궨��
**************************************************************************/

/*�����붨��*/
#define DMAC_SUCCESS                       BSP_OK
#define DMA_FAIL                           BSP_ERROR

#define DMA_ERROR_BASE                          -100
#define DMA_CHANNEL_INVALID                     (DMA_ERROR_BASE-1)
#define DMA_TRXFERSIZE_INVALID                  (DMA_ERROR_BASE-2)
#define DMA_SOURCE_ADDRESS_INVALID              (DMA_ERROR_BASE-3)
#define DMA_DESTINATION_ADDRESS_INVALID         (DMA_ERROR_BASE-4)
#define DMA_MEMORY_ADDRESS_INVALID              (DMA_ERROR_BASE-5)
#define DMA_PERIPHERAL_ID_INVALID               (DMA_ERROR_BASE-6)
#define DMA_DIRECTION_ERROR                     (DMA_ERROR_BASE-7)
#define DMA_TRXFER_ERROR                        (DMA_ERROR_BASE-8)
#define DMA_LLIHEAD_ERROR                       (DMA_ERROR_BASE-9)
#define DMA_SWIDTH_ERROR                        (DMA_ERROR_BASE-0xa)
#define DMA_LLI_ADDRESS_INVALID                 (DMA_ERROR_BASE-0xb)
#define DMA_TRANS_CONTROL_INVALID               (DMA_ERROR_BASE-0xc)
#define DMA_MEMORY_ALLOCATE_ERROR               (DMA_ERROR_BASE-0xd)
#define DMA_NOT_FINISHED                        (DMA_ERROR_BASE-0xe)
#define DMA_CONFIG_ERROR                        (DMA_ERROR_BASE-0xf)


/*******************************************************************************
  ������:      BSP_S32 balong_dma_init(void)
  ��������:    DMA��ʼ�����򣬹ҽ��ж�
  �������:    ��
  �������:    ��
  ����ֵ:      0
*******************************************************************************/
extern BSP_S32 balong_dma_init(void);

/*******************************************************************************
  ������:       int balong_dma_current_transfer_address(UINT32 channel_id)
  ��������:     ���ĳͨ����ǰ������ڴ��ַ
  �������:     channel_id : ͨ��ID������balong_dma_channel_init�����ķ���ֵ
  �������:     ��
  ����ֵ:       �ɹ���ͨ����ǰ������ڴ��ַ
                ʧ�ܣ�-1
*******************************************************************************/
extern int balong_dma_current_transfer_address(BSP_U32 channel_id);
#define DRV_EDMA_CURR_TRANS_ADDR(channel_id)  balong_dma_current_transfer_address(channel_id)

/*******************************************************************************
  ������:       int balong_dma_channel_stop(UINT32 channel_id)
  ��������:     ָֹͣ����DMAͨ��
  �������:     channel_id : ͨ��ID������balong_dma_channel_init�����ķ���ֵ
  �������:     ��
  ����ֵ:       �ɹ���ͨ����ǰ������ڴ��ַ
                ʧ�ܣ�-1
*******************************************************************************/
extern BSP_S32 balong_dma_channel_stop(BSP_U32 channel_id);
#define DRV_EDMA_CHANNEL_STOP(channel_id)  balong_dma_channel_stop(channel_id)


/*******************************************************************************
  ������:      BALONG_DMA_CB *balong_dma_channel_get_lli_addr(UINT32 channel_id)
  ��������:    ��ȡָ��DMAͨ����������ƿ����ʼ��ַ
  �������:    channel_id��ͨ��ID,����balong_dma_channel_init�����ķ���ֵ
  �������:    ��
  ����ֵ:      �ɹ���������ƿ���ʼ��ַ
               ʧ�ܣ�0
*******************************************************************************/
extern BALONG_DMA_CB *balong_dma_channel_get_lli_addr (BSP_U32 channel_id);
#define DRV_EDMA_CHAN_GET_LLI_ADDR(channel_id)  balong_dma_channel_get_lli_addr(channel_id)


/******************************************************************************
  ������:      int balong_dma_channel_init (BALONG_DMA_REQ req,
                  channel_isr pFunc, UINT32 channel_arg, UINT32 int_flag)
  ��������:    ���������߼�ͨ����ӳ������ͨ����������ţ�ע��ͨ���жϻص�������
               ��ʼ����������ź������������д��config�Ĵ���
  �������:    req : �߼�ͨ����
               pFunc : �ϲ�ģ��ע���DMAͨ���жϴ�������NULLʱ������ע��
               channel_arg : pFunc�����1��
                             pFuncΪNULL������Ҫ�����������
                             Ϊdsp�����ͨ����Ϊ4~11,���ڸò�����
                             dsp�贫��4~11��ĳֵ����ʾ��Ҫʹ�õ�ͨ����
               int_flag : pFunc�����2, �������ж����ͣ�ȡֵ��ΧΪ
                        BALONG_DMA_INT_DONE��BALONG_DMA_INT_LLT_DONE��
                      �BALONG_DMA_INT_CONFIG_ERR��BALONG_DMA_INT_TRANSFER_ERR��
                        BALONG_DMA_INT_READ_ERR֮һ��������ϡ�
                        pFuncΪNULL������Ҫ�����������
  �������:    ��
  ����ֵ:      �ɹ���ͨ����
               ʧ�ܣ�����
*******************************************************************************/
extern BSP_S32 balong_dma_channel_init (BALONG_DMA_REQ req, channel_isr pFunc, BSP_U32 channel_arg, BSP_U32 int_flag);
#define DRV_EDMA_CHANNEL_INIT(req, pFunc,channel_arg,int_flag) balong_dma_channel_init(req, pFunc,channel_arg,int_flag)

/*******************************************************************************
  ������:      int balong_dma_channel_set_config (UINT32 channel_id,
                       UINT32 direction, UINT32 burst_width, UINT32 burst_len)
  ��������:    ����ʽDMA����ʱ�����ñ���������ͨ������
               ��ʽDMA����ʱ������Ҫʹ�ñ�������
  �������:    channel_id : ͨ��ID������balong_dma_channel_init�����ķ���ֵ
               direction : DMA���䷽��, ȡֵΪBALONG_DMA_P2M��BALONG_DMA_M2P��
                           BALONG_DMA_M2M֮һ
               burst_width��ȡֵΪ0��1��2��3����ʾ��burstλ��Ϊ8��16��32��64bit
               burst_len��ȡֵ��Χ0~15����ʾ��burst����Ϊ1~16
  �������:    ��
  ����ֵ:      �ɹ���0
               ʧ�ܣ�����
*******************************************************************************/
extern BSP_S32 balong_dma_channel_set_config (BSP_U32 channel_id, BSP_U32 direction,BSP_U32 burst_width, BSP_U32 burst_len);
#define DRV_EDMA_CHANNEL_CONFIG(channel_id, direction,burst_width,burst_len)  balong_dma_channel_set_config(channel_id, direction,burst_width,burst_len)

/*******************************************************************************
  ������:      int balong_dma_channel_dest_set_config (UINT32 channel_id,
                       UINT32 burst_width, UINT32 burst_len)
  ��������:    BBP GRIF��Ҫ��������Ŀ������λ��ͳ��ȣ����ñ��������������β�ʹ�á�
               ʹ��ʱ���ýӿ���balong_dma_channel_set_config֮����á�
  �������:    channel_id��ͨ��ID������balong_dma_channel_init�����ķ���ֵ
               burst_width��ȡֵΪ0��1��2��3����ʾ��burstλ��Ϊ8��16��32��64bit
               burst_len��ȡֵ��Χ0~15����ʾ��burst����Ϊ1~16
  �������:    ��
  ����ֵ:      �ɹ���0
               ʧ�ܣ�����
*******************************************************************************/
extern BSP_S32 balong_dma_channel_dest_set_config (BSP_U32 channel_id, BSP_U32 dest_width, BSP_U32 dest_len);
#define DRV_EDMA_CHANNEL_DEST_CONFIG(channel_id, dest_width, dest_len)  balong_dma_channel_dest_set_config(channel_id, dest_width, dest_len)

/*******************************************************************************
  ������:      int balong_dma_channel_start (UINT32 channel_id, UINT32 src_addr,
                       UINT32 des_addr, UINT32 len)
  ��������:    ����һ��ͬ��DMA����, DMA������ɺ󣬲ŷ���
               ʹ�ñ�����ʱ������Ҫע���жϴ�����
               len<65536(64K):һά����
               65536(64K)<=len<16384*65536(1G)ʱ:��ά����
               len>16384*65536:��֧��
               ��ά����lenΪ16K��������������������֧��
  �������:    channel_id��ͨ��ID,����balong_dma_channel_init�����ķ���ֵ
               src_addr�����ݴ���Դ��ַ�������������ַ
               des_addr�����ݴ���Ŀ�ĵ�ַ�������������ַ
               len�����ݴ��䳤�ȣ���λ���ֽڣ�֧����󳤶���0x40000000�ֽ�
  �������:    ��
  ����ֵ:      �ɹ���0
               ʧ�ܣ�����
*******************************************************************************/
extern BSP_S32 balong_dma_channel_start (BSP_U32 channel_id, BSP_U32 src_addr, BSP_U32 des_addr, BSP_U32 len);
#define DRV_EDMA_CHANNEL_START(channel_id,src_addr,des_addr,len)  balong_dma_channel_start(channel_id,src_addr,des_addr,len)


/*******************************************************************************
  ������:      int balong_dma_channel_async_start (UINT32 channel_id,
                unsigned int src_addr, unsigned int des_addr, unsigned int len)
  ��������:    ����һ���첽DMA���䡣����DMA����󣬾ͷ��ء����ȴ�DMA������ɡ�
               ʹ�ñ�����ʱ��ע���жϴ��������жϴ������д���DMA
��������¼�
               ���ߣ���ע���жϴ�������ʹ��balong_dma_channel_is_idle������ѯ
               DMA�����Ƿ����
               len<65536(64K):һά����
               65536(64K)<=len<16384*65536(1G)ʱ:��ά����
               len>16384*65536:��֧��
               ��ά����lenΪ16K��������������������֧��
  �������:    channel_id��ͨ��ID,����balong_dma_channel_init�����ķ���ֵ
               src_addr�����ݴ���Դ��ַ�������������ַ
               des_addr�����ݴ���Ŀ�ĵ�ַ�������������ַ
               len�����ݴ��䳤�ȣ���λ���ֽڣ�֧����󳤶���0x40000000�ֽ�
  �������:    ��
  ����ֵ:      �ɹ���0
               ʧ�ܣ�����
*******************************************************************************/
extern BSP_S32 balong_dma_channel_async_start (BSP_U32 channel_id, BSP_U32 src_addr, BSP_U32 des_addr, BSP_U32 len);
#define DRV_EDMA_CHANNEL_ASYNC_START(channel_id,src_addr,des_addr,len) balong_dma_channel_async_start(channel_id,src_addr,des_addr,len)


/*******************************************************************************
  ������:      int balong_dma_channel_lli_start (UINT32 channel_id)
  ��������:    ������ʽDMA���䡣����ʽDMA�����нڵ㴫�䶼ȫ����ɺ�ŷ��ء�
               ��ʽDMA��ÿ���ڵ����������䳤��Ϊ65535�ֽڡ�
               ע�⣺���ô˺���ǰ���������ú�������ƿ顣
  �������:    channel_id��ͨ��ID,����balong_dma_channel_init�����ķ���ֵ
  �������:    ��
  ����ֵ:      �ɹ���0
               ʧ�ܣ�����
*******************************************************************************/
extern BSP_S32 balong_dma_channel_lli_start (BSP_U32 channel_id);
#define DRV_EDMA_CHANNEL_lli_START(channel_id)   balong_dma_channel_lli_start(channel_id)

/*******************************************************************************
  ������:      int balong_dma_channel_lli_start (UINT32 channel_id)
  ��������:    ������ʽDMA���䣬Ȼ���������أ����ȴ�DMA������ɡ�
               ��ʽDMA��ÿ���ڵ����������䳤��Ϊ65535�ֽڡ�
               ע�⣺���ô˺���ǰ���������ú�������ƿ顣
  �������:    channel_id��ͨ��ID,����balong_dma_channel_init�����ķ���ֵ
  �������:    ��
  ����ֵ:      �ɹ���0
               ʧ�ܣ�����
*******************************************************************************/
extern BSP_S32 balong_dma_channel_lli_async_start (BSP_U32 channel_id);
#define DRV_EDMA_CHANNEL_lli_ASYNC_START(channel_id)   balong_dma_channel_lli_async_start(channel_id)


/******************************************************************************
*
  ������:       int balong_dma_channel_is_idle (UINT32 channel_id)
  ��������:     ��ѯDMAͨ���Ƿ����
  �������:     channel_id : ͨ��ID������balong_dma_channel_init�����ķ���ֵ
  �������:     ��
  ����ֵ:       0 : ͨ��æµ
                1 : ͨ������
                ���� : ʧ��
*******************************************************************************/
extern BSP_S32 balong_dma_channel_is_idle (BSP_U32 channel_id);
#define DRV_EDMA_CHANNEL_IS_IDLE(chanel_id) balong_dma_channel_is_idle(chanel_id)

/*****************************************************************************
 �� �� ��  : balong_dma_set_config
 ��������  : ���ڸ����û��·�EDMA������Ϣ����config�Ĵ�����Ϣ�����ظ��û�
 �������  : req         �߼������
             direction   ���䷽��
             burst_width burst����λ��
             burst_len   burst���䳤��
 �������  : ��
 �� �� ֵ  : 0-ʧ�ܣ�����-���ɵļĴ�����Ϣ
 ���ú���  : NA
 ��������  : NA

 �޸���ʷ      :
  1.��    ��   : 2013��1��15��
    �޸�����   : �����ɺ���

*****************************************************************************/
extern BSP_U32 balong_dma_set_config(BALONG_DMA_REQ req, BSP_U32 direction, BSP_U32 burst_width, BSP_U32 burst_len);
#define BALONG_DMA_SET_CONFIG(req, direction, burst_width, burst_len) \
    balong_dma_set_config(req, direction, burst_width, burst_len)

/*****************************************************************************
 �� �� ��  : DRV_EDMA_BBP_SAMPLE_REBOOT
 ��������  : BBP����ʹ�������ӿڣ�������ϵͳ��ͣ����fastboot�׶Σ��Ա㵼���ɼ����ݣ�
             SFTƽ̨A��ʹ�ã�����ֱ�ӷ���-1
 �������  : NA
 �������  : ��
 �� �� ֵ  : OK-ִ��������ERROR-��ִ������
 ���ú���  : NA
 ��������  : NA

 �޸���ʷ      :
  1.��    ��   : 2013��1��5��
    �޸�����   : �����ɺ���

*****************************************************************************/
extern BSP_S32 DRV_EDMA_BBP_SAMPLE_REBOOT(BSP_VOID);

#endif


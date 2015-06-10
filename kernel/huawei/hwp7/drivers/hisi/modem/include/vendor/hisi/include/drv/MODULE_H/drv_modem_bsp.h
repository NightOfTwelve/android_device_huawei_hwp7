
#ifndef __DRV_MODEM_BSP_H__
#define __DRV_MODEM_BSP_H__
#include "drv_global.h"
#if (defined(FEATURE_E5) &&  defined(FEATURE_HILINK) && ((FEATURE_E5 == FEATURE_ON) || (FEATURE_HILINK == FEATURE_ON)))
/*
    ����ģʽ����
*/
typedef enum DRV_START_MODE_tag
{
    DRV_START_MODE_EXCEPTION,  /* �쳣����ģʽ����ʱ�󲿷�ҵ�񶼲�����, ����ʾ��Ϣ */
    DRV_START_MODE_CHARGING,   /* ��翪��ģʽ, ps ��ҵ������  */
    DRV_START_MODE_NORMAL,     /* ��������ģʽ������ҵ������     */
    DRV_START_MODE_BUTT
}DRV_START_MODE_ENUM;

/************************************************************************************
 *Function Name :  drv_start_mode_get
 *Description   :  ��ȡ���ο�����ģʽ���μ� DRV_START_MODE_ENUM ����
 *Others        :  No
************************************************************************************/
DRV_START_MODE_ENUM drvStartModeGet( void );

/*************************************************
  Function:      drv_sleep
  Description:   ʹ��always on ��ʱ��ʵ��sleep����
  Calls:
  Called By:
  Data Accessed: NONE
  Data Updated:  NONE
  Input:    ulLength ��sleep ��λ��ĿǰΪ10ms
            ulFileID:  File name, for debug;
            usLineNo:  Line number, for debug;
  Output:
  Return:
  Others:

************************************************/
void drv_sleep( UINT32 ulLength, UINT32 ulFileID, UINT32 usLineNo);

#endif

/*****************************************************************************
* �� �� ��  : BSP_INT_Enable
*
* ��������  : ʹ��ĳ���ж�
*
* �������  : INT32 ulLvl Ҫʹ�ܵ��жϺţ�ȡֵ��Χ0��40
* �������  : ��
*
* �� �� ֵ  : OK&ERROR
*
*****************************************************************************/
BSP_S32 BSP_INT_Enable ( BSP_S32 s32Lvl);
#define DRV_VICINT_ENABLE(ulLvl)    BSP_INT_Enable(ulLvl)

/*****************************************************************************
 * �� �� ��  : BSP_INT_Disable
 *
 * ��������  : ȥʹ��ĳ���ж�
 *
 * �������  : INT32 ulLvl Ҫʹ�ܵ��жϺţ�ȡֵ��Χ0��40
 * �������  : ��
 *
 * �� �� ֵ  : OK&ERROR
 *
 *****************************************************************************/
BSP_S32 BSP_INT_Disable ( BSP_S32 s32Lvl);
#define  DRV_VICINT_DISABLE(ulLvl)    BSP_INT_Disable(ulLvl)

/*****************************************************************************
 * �� �� ��  : BSP_INT_Connect
 *
 * ��������  : ע��ĳ���ж�
 *
 * �������  : VOIDFUNCPTR * vector �ж������ţ�ȡֵ��Χ0��40
 *           VOIDFUNCPTR routine  �жϷ������
 *           INT32 parameter      �жϷ���������
 * �������  : ��
 *
 * �� �� ֵ  : OK&ERROR
 *
 *****************************************************************************/
BSP_S32 BSP_INT_Connect  (VOIDFUNCPTR * vector,VOIDFUNCPTR routine, BSP_S32 parameter);
#define DRV_VICINT_CONNECT(vector,routine,parameter)    BSP_INT_Connect(vector,routine,parameter)


/**************************************************************************
  �궨��
**************************************************************************/
/* �ڴ������, Flags����� */
typedef enum tagMEM_POOL_TYPE
{
    MEM_NORM_DDR_POOL = 0,
    MEM_ICC_DDR_POOL,
    MEM_ICC_AXI_POOL,
    MEM_POOL_MAX
}MEM_POOL_TYPE;

/**************************************************************************
  �ӿ�����
**************************************************************************/
BSP_VOID* BSP_Malloc(BSP_U32 u32Size, MEM_POOL_TYPE enFlags);
BSP_VOID* BSP_MallocDbg(BSP_U32 u32Size, MEM_POOL_TYPE enFlags, BSP_U8* pFileName, BSP_U32 u32Line);
BSP_VOID  BSP_Free(BSP_VOID* pMem);
BSP_VOID  BSP_FreeDbg(BSP_VOID* pMem, BSP_U8* pFileName, BSP_U32 u32Line);

/*****************************************************************************
* �� �� ��  : BSP_MALLOC
*
* ��������  : BSP ��̬�ڴ����
*
* �������  : sz: ����Ĵ�С(byte)
*             flags: �ڴ�����(�ݲ�ʹ��,Ԥ��)
* �������  : ��
* �� �� ֵ  : ����������ڴ�ָ��
*****************************************************************************/
#ifdef __BSP_DEBUG__
#define BSP_MALLOC(sz, flags) BSP_MallocDbg(sz, flags, __FILE__, __LINE__)
#else
#define BSP_MALLOC(sz, flags) BSP_Malloc(sz, flags)
#endif

/*****************************************************************************
* �� �� ��  : BSP_FREE
*
* ��������  : BSP ��̬�ڴ��ͷ�
*
* �������  : ptr: ��̬�ڴ�ָ��
* �������  : ��
* �� �� ֵ  : ��
*****************************************************************************/
#ifdef __BSP_DEBUG__
#define BSP_FREE(ptr) BSP_FreeDbg(ptr, __FILE__, __LINE__)
#else
#define BSP_FREE(ptr) BSP_Free(ptr)
#endif



/*****************************************************************************
* �� �� ��  : BSP_SFree
*
* ��������  : BSP ��̬�ڴ��ͷ�(��spin lock����,��˳���ʹ��)
*
* �������  : pMem: ��̬�ڴ�ָ��
* �������  : ��
* �� �� ֵ  : ��
*****************************************************************************/
BSP_VOID  BSP_SFree(BSP_VOID* pMem);
/*****************************************************************************
* �� �� ��  : BSP_SMalloc
*
* ��������  : BSP ��̬�ڴ����(��spin lock����,��˳���ʹ��)
*
* �������  : u32Size: ����Ĵ�С(byte)
*             enFlags: �ڴ�����(�ݲ�ʹ��,Ԥ��)
* �������  : ��
* �� �� ֵ  : ����������ڴ�ָ��
*****************************************************************************/
BSP_VOID* BSP_SMalloc(BSP_U32 u32Size, MEM_POOL_TYPE enFlags);


#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
extern BSP_VOID* OSAL_CacheDmaMalloc(BSP_U32  bytes);
#define DRV_CACHEDMAM_ALLOC(bytes) OSAL_CacheDmaMalloc(bytes)
#else
#define DRV_CACHEDMAM_ALLOC(bytes) kmalloc(bytes, GFP_KERNEL)
#endif

#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
extern BSP_S32 OSAL_CacheDmaFree(BSP_VOID*  pBuf);
#define DRV_CACHEDMAM_FREE(pBuf) OSAL_CacheDmaFree(pBuf)
#else
#define DRV_CACHEDMAM_FREE(pBuf) kfree(pBuf)
#endif

/************************************************************************
 * FUNCTION
 *       vmEnable
 * DESCRIPTION
 *       �ڴ��д��������ʹ�ܺ���
 * INPUTS
 *       NONE
 * OUTPUTS
 *       NONE
 *************************************************************************/
extern int vmEnable(int enable);
#define DRV_VM_ENABLE(flag)   vmEnable(flag)

/*****************************************************************************
 �� �� ��  : vmStateSet
 ��������  : ���õ�ַ�ռ�״̬
 �������  : ��

 �������  : �ޡ�
 �� �� ֵ  : ��
 ע������  ��
*****************************************************************************/
#define DRV_VM_STATESET(context, virtAdrs, len, stateMask, state) vmStateSet(context, virtAdrs, len, stateMask, state)

/*****************************************************************************
 �� �� ��  : BSP_CACHE_DATA_FLUSH
 ��������  :
 �������  :
 �������  : None
 �� �� ֵ  : void
*****************************************************************************/
extern void BSP_CACHE_DATA_FLUSH(void * addr, int size);

/************************************************************************
 * FUNCTION
 *       max_freeblock_size_get
 * DESCRIPTION
 *       get memory max free block size
 * INPUTS
 *       ��
 * OUTPUTS
 *       max free block size
 *************************************************************************/
extern int DRV_GET_FREE_BLOCK_SIZE(void);

/*****************************************************************************
 �� �� ��  : BSP_DFS_GetCurCpuLoad
 ��������  : ��ѯ��ǰCPU
 �������  : pu32AcpuLoad ACPUloadָ��
             pu32CcpuLoad CCPUloadָ��
 �������  : pu32AcpuLoad ACPUloadָ��
             pu32CcpuLoad CCPUloadָ��
 �� �� ֵ  : 0:  �����ɹ���
            -1������ʧ�ܡ�
*****************************************************************************/
extern BSP_U32 BSP_DFS_GetCurCpuLoad(BSP_U32 *pu32AcpuLoad,BSP_U32 *pu32CcpuLoad);
#define DRV_GET_CUR_CPU_LOAD(pu32AcpuLoad,pu32CcpuLoad) BSP_DFS_GetCurCpuLoad(pu32AcpuLoad,pu32CcpuLoad)


/*****************************************************************************
 �� �� ��  : BSP_GU_GetVerTime
 ��������  : ��ȡ�汾����ʱ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
extern char *BSP_GU_GetVerTime(void);
#define DRV_GET_VERSION_TIME()    BSP_GU_GetVerTime()

/***********************************�ڴ濽���Ż�*****************************************/
/*****************************************************************************
 �� �� ��  : __rt_memcpy
 ��������  : ���汾��memcpy����
 �������  : Dest :Ŀ�ĵ�ַ
             Src :Դ��ַ
             Count:�������ݵĴ�С
 �������  : �ޡ�
 �� �� ֵ  : Ŀ�ĵ�ַ��
*****************************************************************************/
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
extern void * __rt_memcpy(void * Dest, const void * Src, unsigned long Count);
#define DRV_RT_MEMCPY(Dest,Src,Count)       __rt_memcpy(Dest,Src,Count)
#else
#define DRV_RT_MEMCPY(Dest,Src,Count)       memcpy(Dest,Src,Count)
#endif

/*****************************************************************************
 �� �� ��  : bsp_memcpy_align32
 ��������  : ���汾��32λ���ݶ���memcpy����
 �������  : Dest :Ŀ�ĵ�ַ
             Src :Դ��ַ
             Count:�������ݵĴ�С
 �������  : �ޡ�
 �� �� ֵ  : Ŀ�ĵ�ַ��
*****************************************************************************/
extern void bsp_memcpy_align32(UINT32 *pDstBuf,  UINT32 *pSrcBuf, UINT32 ulSize);
#define DRV_MEMCPY_ALIGN32(Dest,Src,Count)       bsp_memcpy_align32(Dest,Src,Count)

/*****************************************************************************
 �� �� ��  : bsp_memcpy_auto_align64
 ��������  : �Զ���64λ����汾��memcpy����
 �������  : Dest :Ŀ�ĵ�ַ
             Src :Դ��ַ
             Count:�������ݵĴ�С
 �������  : �ޡ�
 �� �� ֵ  : Ŀ�ĵ�ַ��
*****************************************************************************/
extern void bsp_memcpy_auto_align(void *pDstBuf,  void *pSrcBuf, UINT32 ulLen, UINT32 ulAlignSize);
#define DRV_MEMCPY_AUTO_ALIGN64(Dest,Src,Count) bsp_memcpy_auto_align(Dest,Src,Count,8);

/*****************************************************************
Function: free_mem_size_get
Description:
    get mem size
Input:
    N/A
Output:
    N/A
Return:
    free mem size
*******************************************************************/
extern unsigned long free_mem_size_get(void);
#define FREE_MEM_SIZE_GET() free_mem_size_get()

/*****************************************************************************
 �� �� ��  : BSP_DMR_ATAnalyze
 ��������  : ��NAS��ѯ��̬�ڴ�ռ����Ϣ��ͨ��AT�����ѯ
 �������  : �ޡ�
 �������  : �ޡ�
 �� �� ֵ  : �ɹ�0��ʧ��-1
*****************************************************************************/
extern int BSP_DMR_ATANALYZE(void * pstMem,unsigned int uiSize, unsigned int * puiModIdSum);


/* ����������Ҫ��ѯ��IP���� */
typedef enum tagBSP_IP_TYPE_E
{
    BSP_IP_TYPE_SOCP = 0x0,
    BSP_IP_TYPE_CICOM0,
    BSP_IP_TYPE_CICOM1,
    BSP_IP_TYPE_HDLC,
    BSP_IP_TYPE_BBPMASTER,
    BSP_IP_TYPE_ZSP_ITCM,
    BSP_IP_TYPE_ZSP_DTCM,
    BSP_IP_TYPE_AHB,
    BSP_IP_TYPE_WBBP,
    BSP_IP_TYPE_WBBP_DRX,
    BSP_IP_TYPE_GBBP,
    BSP_IP_TYPE_GBBP_DRX,
    BSP_IP_TYPE_GBBP1,
    BSP_IP_TYPE_GBBP1_DRX,
    BSP_IP_TYPE_ZSPDMA,
    BSP_IP_TYPE_SYSCTRL,
    BSP_IP_TYPE_CTU,
    BSP_IP_TYPE_TDSSYS,
    BSP_IP_TYPE_ZSPDHI,

    BSP_IP_TYPE_BUTTOM
}BSP_IP_TYPE_E;

/*****************************************************************************
 �� �� ��  : BSP_GetIPBaseAddr
 ��������  : IP����ַ��ѯ
 �������  : enIPType: ��Ҫ��ѯ��IP����
 �������  : ��
 ����ֵ    ����ѯ����IP����ַ
*****************************************************************************/
BSP_U32 BSP_GetIPBaseAddr(BSP_IP_TYPE_E enIPType);
#define DRV_GET_IP_BASE_ADDR(enIPType)  BSP_GetIPBaseAddr(enIPType)

/* ����������Ҫ��ѯ��MEM���� */
typedef enum tagBSP_DDR_TYPE_E
{
    BSP_DDR_TYPE_DDR_GU = 0x0,          /*DDR:GUԤ��*/
    BSP_DDR_TYPE_DDR_TLPHY_IMAGE,       /*DDR:TL DSP ����*/
    /*BSP_DDR_TYPE_DDR_LPHY_SDR,*/      /*NO USED*/
    BSP_DDR_TYPE_DDR_TLPHY_LCS,         /*DDR:TL DSP LCS*/
    BSP_DDR_TYPE_DDR_TLPHY_BANDNV,      /*DDR:TL DSP BAND*/
    BSP_DDR_TYPE_DDR_TPHY_TABLE,        /*DDR:TL TDS TABLE*/
    BSP_DDR_TYPE_DDR_HIFI,              /*DDR:HIFI*/
    BSP_DDR_TYPE_SRAM_TLPHY,            /*SRAM:TL DSP*/
    BSP_DDR_TYPE_SRAM_RTT_SLEEP_FLAG,   /*SRAM:TL DSP˯�߻��ѱ��*/
    BSP_DDR_TYPE_SHM_TEMPERATURE,       /*�����ڴ�:�±�*/
    BSP_DDR_TYPE_SHM_BBE16_HIFI_HEAD,   /*�����ڴ�:BBE16->HIFI����ͷ*/
    BSP_DDR_TYPE_SHM_HIFI_BBE16_HEAD,   /*�����ڴ�:HIFI->BBE16����ͷ*/
    BSP_DDR_TYPE_SHM_BBE16_HIFI_QUEUE,  /*�����ڴ�:BBE16->HIFI�������*/
    BSP_DDR_TYPE_SHM_HIFI_BBE16_QUEUE,  /*�����ڴ�:HIFI->BBE16�������*/
    /*BSP_DDR_TYPE_SHM_TIMESTAMP,*/     /*NO USED*/

    BSP_DDR_TYPE_BUTTOM
}BSP_DDR_TYPE_E;
/*****************************************************************************
 �� �� ��  : BSP_GetMemAddrSize
 ��������  : ��ȡmem��ַ�ʹ�С��Ϣ
 �������  : enMemType: ��Ҫ��ѯ��mem����
 �������  : addr ��ַ
             size ��С
 ����ֵ    ��BSP_OK    ��ѯ�ɹ�
             BSP_ERROR ��ѯʧ��
*****************************************************************************/
BSP_U32 BSP_GetMemAddrSize(BSP_DDR_TYPE_E enDdrType, BSP_U32 *addr, BSP_U32 *size);
#define DRV_GET_MEM_ADDR_SIZE(enDdrType, pAddr, pSize)  BSP_GetMemAddrSize(enDdrType, pAddr, pSize)


/* ��Ҫ��ѯ���ж����� */
typedef enum tagBSP_INT_TYPE_E
{
    BSP_INT_TYPE_RTC = 0,
    BSP_INT_TYPE_WDT,
    BSP_INT_TYPE_USBOTG,
    BSP_INT_TYPE_UICC,
    BSP_INT_TYPE_IPF_C,
    BSP_INT_TYPE_IPF_A,
    BSP_INT_TYPE_SOCP_A,
    BSP_INT_TYPE_SOCP_C,
    BSP_INT_TYPE_CICOM_UL,
    BSP_INT_TYPE_CICOM_DL,
    BSP_INT_TYPE_CICOM1_UL,
    BSP_INT_TYPE_CICOM1_DL,
    BSP_INT_TYPE_HDLC_DEF,
    BSP_INT_TYPE_HDLC_FRM,

    BSP_INT_TYPE_GBBP,          /*G_BBP_INT_LEVEL*/
    BSP_INT_TYPE_GBBP_AWAKE,    /*G_BBP_AWAKE_INT_LEVEL*/
    BSP_INT_TYPE_GBBP_DSP,
    BSP_INT_TYPE_GBBP_AWAKE_DSP,
    BSP_INT_TYPE_GBBP1,
    BSP_INT_TYPE_GBBP1_AWAKE,
    BSP_INT_TYPE_GBBP1_DSP,
    BSP_INT_TYPE_GBBP1_AWAKE_DSP,
    BSP_INT_TYPE_WBBP_0MS,      /*card1 0ms�ж�*/
    BSP_INT_TYPE_WBBP_AWAKE,    /*˯�ߡ������ж�*/
    BSP_INT_TYPE_WBBP_SWITCH,   /*ʱ���л��ж�*/
    BSP_INT_TYPE_WBBP1_0MS,     /*Card2 0ms �ж�*/
    BSP_INT_TYPE_WBBP_SEARCH,   /*С�������ж�*/
    BSP_INT_TYPE_WBBP_RAKE,     /*Rake����ϱ��ж�*/
    BSP_INT_TYPE_WBBP_DECODE,   /*���������ϱ��ж�*/
    BSP_INT_TYPE_WBBP_TIME,     /*card1��ʱ�ж�*/
    BSP_INT_TYPE_WBBP1_TIME,    /*card2��ʱ�ж�*/
    BSP_INT_TYPE_WBBP_MULTI_SEARCH, /*�ྶ���������ж�*/
    BSP_INT_TYPE_WBBP_BBPMST,       /*����BBP Master�ϱ��ж�*/
    BSP_INT_TYPE_UPACC_DSP,
    BSP_INT_TYPE_INT_OSRTC,     /*RTC_DUAL_TIMER_INT_LEVEL*/
    BSP_INT_TYPE_INT_SMIM,      /*SOC_SMIM_INT_LEVEL*/
    BSP_INT_TYPE_INT_ZSP_DOG,   /*ZSP_WDG_INT_LEVEL*/
    BSP_INT_TYPE_INT_HIFI_DOG,  /*HIFI_WDG_INT_LEVEL*/

    BSP_INT_TYPE_INT12_G2,        /*CTU������G2��ص��ж�*/
    BSP_INT_TYPE_ODTOA_INT12,     /*Ŀ�궨λ�ж�*/
    BSP_INT_TYPE_CTU_INT_W,       /*CTU������Wģ��ص��ж�(W����Ϊ��ģ��Ҳ����Ϊ��ģ)*/
    BSP_INT_TYPE_CTU_INT_G,       /*CTU������Gģ��ص��ж�(G����Ϊ��ģ��Ҳ����Ϊ��ģ)*/
    BSP_INT_TYPE_CTU_INT_LTE,     /*CTU������LTEģ��ص��ж�(LTE����Ϊ��ģ��Ҳ����Ϊ��ģ)*/
    BSP_INT_TYPE_CTU_INT_TDS,     /*CTU������TDSģ��ص��ж�(TDS����Ϊ��ģ��Ҳ����Ϊ��ģ)*/
    BSP_INT_TYPE_UPACC_INTR,              /*UPACC�ж�*/
    BSP_INT_TYPE_GBBP_GSML_RESERVED_INTR, /*PHY_32Kʱ�����ж�(�����ǵ�����)*/

    BSP_INT_TYPE_LTE_CIPHER,          /*L Cipher�ж�*/
    BSP_INT_TYPE_LTE_APP_ARM_PUB,     /*��ƽ�жϣ�dbgҵ����أ���ARM A��*/
    BSP_INT_TYPE_LTE_ARM_POSITIONS,   /*�����жϣ���ARM M�ˣ�122.8MHz��8cycle*/
    BSP_INT_TYPE_LTE_DL_DMA,          /*�����жϣ���ARM M�ˣ�122.8MHz��8cycle*/
    BSP_INT_TYPE_LTE_ARM_POSITION,    /*��ƽ�жϣ�stu��أ���ARM M��*/
    BSP_INT_TYPE_LTE_ARM_DL_DMA,      /*��ƽ�жϣ�tdlҵ����أ���ARM M��*/
    BSP_INT_TYPE_LTE_ARM_CLK_SWITCH,  /*drxģ���жϣ���ARM M��*/
    BSP_INT_TYPE_LBBP_AWAKE,          /*drxģ���жϣ���ARM M��*/
    BSP_INT_TYPE_LTE_PUB,             /*drxģ���жϣ���ARM M��*/
    BSP_INT_TYPE_TDS_STU_ARM1,        /*tds��ʱģ����֡�жϣ���ARM��81.92Mʱ����//��ƽ�ж�*/
    BSP_INT_TYPE_TBBP_AWAKE,          /*tds drx��ARM�Ļ����ж�*/
    BSP_INT_TYPE_TDS_DRX_ARM2,        /*tds drx��ARM��clk_switch�ж�*/
    BSP_INT_TYPE_BBE16_GPIO1,         /*BBE16�ڲ���GPIO1��������ں˼�ͨ��(ltedsp_tie_expstate[1])*/

    BSP_INT_TYPE_BUTTOM
}BSP_INT_TYPE_E;


/*****************************************************************************
 �� �� ��  : BSP_GetIntNO
 ��������  : �жϺŲ�ѯ
 �������  : enIntType: ��Ҫ��ѯ���ж�����
 �������  : ��
 ����ֵ    ����ѯ�����жϺ�
*****************************************************************************/
BSP_S32 BSP_GetIntNO(BSP_INT_TYPE_E enIntType);
#define DRV_GET_INT_NO(enIntType)    BSP_GetIntNO(enIntType)


/*****************************************************************************
 �� �� ��  : BSP_GUDSP_ShareAddrGet
 ��������  : ��ȡ����㹲���ַ�ε���Ϣ��
 �������  : �ޡ�
 �������  : pulAddr���������㹲���ַ����Ϣ�Ļ��档
 �� �� ֵ  : �ޡ�
*****************************************************************************/
extern int BSP_GUDSP_ShareAddrGet(unsigned int * pulAddrInTcm, unsigned int * pulAddrInDdr, unsigned int * pulLength);
#define DRV_DSP_SHARE_ADDR_GET(pulAddrInTcm,pulAddrInDdr,pulLength)    BSP_GUDSP_ShareAddrGet(pulAddrInTcm,pulAddrInDdr,pulLength)

/*****************************************************************************
 �� �� ��  : BSP_UpateDSPShareInfo
 ��������  : ���������BSS COMMON����Ϣ��
 �������  : �ޡ�
 �������  : ��
 �� �� ֵ  : 0:�ɹ���-1:ʧ�ܡ�
*****************************************************************************/
extern int BSP_GUDSP_UpateShareInfo(unsigned long shareAddr);
#define DRV_BSP_UPDATE_DSP_SHAREINFO(shareAddr)  BSP_GUDSP_UpateShareInfo(shareAddr)
/*****************************************************************************
 �� �� ��  : BSP_GUDSP_DrxGpioValSet
 ��������  : �ṩGPIO���ýӿ�ΪDSP��DRXʹ��
 �������  : u32Data:1 GPIO����ߣ�0 GPIO����͡�
 �������  : ��
 �� �� ֵ  : 0:�ɹ���-1:ʧ�ܡ�
*****************************************************************************/
extern int BSP_GUDSP_DrxGpioValSet(BSP_U32 u32Data);
#define DRV_DSP_DRX_GPIO_VAL_SET(u32Data) BSP_GUDSP_DrxGpioValSet(u32Data)


/*****************************************************************************
 �� �� ��  : BSP_SendNMIInterrupt
 ��������  : ���� NMI ���ж�
 �������  : NMI�жϵ�bitλ
 �������  : ��
 �� �� ֵ  : �ж��Ƿ���Ҫ����
*****************************************************************************/
extern BSP_VOID BSP_SendNMIInterrupt(unsigned int SocBitNO, unsigned int ZspBitNO);
#define DRV_SEND_NMI_INT(SocBitNO,ZspBitNO) BSP_SendNMIInterrupt(SocBitNO,ZspBitNO)


/*���߰汾дSDT��Ϣ���ļ�ϵͳ�ķ�����*/
#define MANUFACTURE_ROOT_PATH "/manufacture"

/*****************************************************************************
 �� �� ��  : drvOnLineUpdateResult
 ��������  : WebUIģ�麯��ע�ᡣ
 �������  : ��
 �������  : �ޡ�
 �� �� ֵ  : ��
 ע������  ��
*****************************************************************************/
extern void drvOnLineUpdateResult(void);
#define DRV_ONLINE_UPDATE_RESULT()    drvOnLineUpdateResult()

/******************************************************************************
*
  ������:       BSP_S32 nand_get_bad_block (BSP_U32 *len, BSP_U32 **ppBadBlock)
  ��������:     ��ѯ����NAND������FLASH ����
  �������:     ��
  �������:     pNum       : ���ػ������
                ppBadBlock ������ָ�룬�������л����index����
  ����ֵ:       0    : ��ѯ�ɹ�
                ���� : ��ѯʧ��
*******************************************************************************/
extern BSP_S32 nand_get_bad_block(BSP_U32 *pNum, BSP_U32 **ppBadBlock);
#define NAND_GET_BAD_BLOCK(pNum, ppBadBlock) nand_get_bad_block(pNum, ppBadBlock)

/******************************************************************************
*
  ������:       BSP_VOID  nand_free_bad_block_mem(BSP_U32* pBadBlock)
  ��������:     ͨ��nand_get_bad_block�ӿ�����Ļ����ڴ��ɵ������룬��Э��ջ����
                �ýӿ��ͷš�
  �������:     pBadBlock
  �������:     ��
  ����ֵ:       BSP_VOID
*******************************************************************************/
extern BSP_VOID  nand_free_bad_block_mem(BSP_U32* pBadBlock);
#define NAND_FREE_BAD_BLOCK_MEM(pBadBlock)  nand_free_bad_block_mem(pBadBlock)

/******************************************************************************
*
  ������:   BSP_Sram_BackupInfoReg
  ��������: ע��SRAM�α��ݻָ���Ϣ
  �������: pucSramAddr ע��Ķ���SRAM�ļ��ص�ַ
            ulSectLen   ע��Ķε��ֽڴ�С�������ڲ�Ĭ�ϲ����κ��ֽڶ��봦��
            ulNeedMulBck�ö��Ƿ���Ҫÿ���µ綼�����ݴ���һ������֮���������
                        ���ĶΣ�����:data����Ҫÿ�ζ������������ݣ���ô�ò���
                        ��дΪ1����text������֮��һֱ���䣬�ʱ���һ�μ��ɣ�����
                        ���߷��ʴ������ò�����дΪ0
            ulNeedCheck �ָ��������Ƿ���ҪУ�飬����У�������Ӱ��ϴ󣬹ʸù���
                        ����Ŀǰ����������չ���ݲ�ʵ�֣�����ֵ��Ч
  �������: None
  ����ֵ:   BSP_OK      ����ɹ�
            BSP_ERROR   ����ʧ��
*******************************************************************************/
extern int BSP_SRAM_BackupInfoReg(unsigned char *pucSramAddr, unsigned long ulSectLen, unsigned long ulNeedMulBak, unsigned long ulNeedCheck);
#define DRV_SRAM_BACKUP_INFO_REG(pucSramAddr, ulSectLen, ulNeedMulBak, ulNeedCheck)  BSP_SRAM_BackupInfoReg(pucSramAddr, ulSectLen, ulNeedMulBak, ulNeedCheck)


#define NAND_MFU_NAME_MAX_LEN    16
#define NAND_DEV_SPEC_MAX_LEN    32

typedef struct
{
    BSP_U32           MufId;                                         /* ����ID */
    BSP_U8      aucMufName[NAND_MFU_NAME_MAX_LEN];             /* ���������ַ��� */
    BSP_U32           DevId;                                         /* �豸ID */
    BSP_U8      aucDevSpec[NAND_DEV_SPEC_MAX_LEN];             /* �豸����ַ��� */
}NAND_DEV_INFO_S;

/******************************************************************************
*
  ������:       BSP_S32 nand_get_dev_info (NAND_DEV_INFO_S *pNandDevInfo)
  ��������:     ��ѯNAND�豸�����Ϣ����������ID���������ơ��豸ID���豸���
  �������:     ��
  �������:     pNandDevInfo    �洢NAND�豸�����Ϣ�Ľṹ��
  ����ֵ:       0    : ��ѯ�ɹ�
                ���� : ��ѯʧ��
*******************************************************************************/
extern BSP_S32 nand_get_dev_info(NAND_DEV_INFO_S *pNandDevInfo);
#define NAND_GET_DEV_INFO(pNandDevInfo) nand_get_dev_info(pNandDevInfo)


/******************************************************************************
* Function     :   BSP_NANDF_Read
*
* Description  :   ��ȡNand Flash��ָ����ַ�ͳ������ݵ�ָ���ռ���
*
* Input        :   u32FlashAddr  ��ȡ���ݵ�Դ��ַ
*              :   u32NumByte    ��ȡ���ݳ��ȣ���λΪ�ֽ�
*
* Output       :   pRamAddr      ��ȡ�����ݴ�ŵĵ�ַ
*
* return       :   �������ɹ����
******************************************************************************/
BSP_S32 BSP_NANDF_Read(BSP_VOID* pRamAddr, BSP_U32 u32FlashAddr, BSP_U32 u32NumByte);

/******************************************************************************
* Function     :   BSP_NANDF_Write
*
* Description  :   ��ָ����ַ�ͳ������ݵ�д��ָ��FLASH��ַ��
*
* Input        :   pRamAddr     д����Դ��ַ
*              :   u32NumByte   ���ݳ��ȣ���λΪ�ֽ�
*
* Output       :   u32FlashAddr д����Ŀ�ĵ�ַ
*
* return       :   д�����ɹ����
******************************************************************************/
BSP_S32 BSP_NANDF_Write(BSP_U32 u32FlashAddr, BSP_VOID *pRamAddr,  BSP_U32 u32NumByte);

/******************************************************************************
* Function     :   BSP_NANDF_Erase
*
* Description  :   ����ָ��Flash��ַ���ڿ�
*
* Input        :   u32address   Ҫ�������ID
*
* Output       :   ��
*
* return       :   ���������ɹ����
******************************************************************************/
BSP_S32 BSP_NANDF_Erase(BSP_U32 u32BlockID);

BSP_U32 nand_isbad(BSP_U32 blockID, BSP_U32 *flag);

/*****************************************************************************
* �� �� ��  : BSP_IPM_FreeBspBuf
*
* ��������  : �ͷ��ڴ�ӿ�
*
* �������  : BSP_U8 *pBuf ��Ҫ�ͷŵ�ָ��
*
* �������  : ��
* �� �� ֵ  : ��
*
*****************************************************************************/
BSP_VOID BSP_IPM_FreeBspBuf(BSP_U8 *pBuf);



/*****************************************************************************
 �� �� ��  : DRV_INT_GET_CONTEXT
 ��������  : �жϵ�ǰ�����Ƿ����ж���ִ��
 �������  : ��
 �������  : ��
 ����ֵ    ��0  ������ж�
             1  �����ж�
*****************************************************************************/
unsigned int DRV_INT_GET_CONTEXT(void);

/*****************************************************************************
 �� �� ��  : DRV_INT_GET_NUM
 ��������  : ��ȡ��ǰ�жϺ�
 �������  : ��
 �������  : ��
 ����ֵ    ���жϺ�
*****************************************************************************/
signed int DRV_INT_GET_NUM(void);
#if defined(BSP_CORE_MODEM)
/*****************************************************************************
 �� �� ��  : BSP_DMB_INSTRUCTION
 ��������  : DMB�ӿڣ�DSP����
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
#define BSP_DMB_INSTRUCTION \
{\
    __asm("STMFD r13!, {r0}");\
    __asm("MOV r0, #0");\
    __asm("MCR p15, 0, r0, c7, c10, 5");\
    __asm("LDMIA r13!, {r0}");\
}
#define DRV_DMB_INSTRUCTION() BSP_DMB_INSTRUCTION
#endif

#endif


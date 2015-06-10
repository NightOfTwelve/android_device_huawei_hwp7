
#include <linux/module.h>
#include <asm/io.h>
#include <asm/string.h>
#include <linux/semaphore.h>
#include <linux/interrupt.h>
#include <linux/pm_qos_params.h>
#include <mntn/excDrv.h>

#include "BSP.h"
#include "bsp_drv_ipc.h"

#ifdef __cplusplus
extern "C" {
#endif

BSP_IPC_ENTRY	stIpcIntTable[INTSRC_NUM];
static IPC_DEV_S	g_stIpcDev = {0};
BSP_U32			g_CoreNum;

struct semaphore	g_semIpcTask[INTSRC_NUM];
IPC_DEBUG_E		g_stIpc_debug = {0};

/* base address of ipc registers */
void __iomem *ipc_base = NULL;

spinlock_t g_ipc_int_lock = SPIN_LOCK_UNLOCKED;

/*acpu idle*/
struct pm_qos_request_list ipc_sem_pm_qos_req_list[INTSRC_NUM];
static unsigned int ipc_sem_pm_qos_refcnt[INTSRC_NUM] = {0};
static spinlock_t ipc_sem_pm_qos_lock;
static int debug_on = 1;

#define pm_qos_debug_trace(msg...) do { \
	if (debug_on) { \
		printk(KERN_INFO "[%s]:", __func__); \
		printk(msg); \
		printk("\n");\
	} \
} while (0)

static irqreturn_t BSP_DRV_IpcIntHandler(int irq, void *dev_id);
static irqreturn_t BSP_IPC_SemIntHandler(int irq, void *dev_id);

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
* �޸ļ�¼  : 2009��3��5��   wangjing  creat
*****************************************************************************/
BSP_S32 BSP_DRV_IPCIntInit(void)
{
	BSP_S32 ret = 0;
	BSP_U32 i = 0;

	if (BSP_TRUE == g_stIpcDev.bInit)
	{
		return BSP_OK;
	}

	for (i = 0; i < INTSRC_NUM; i++)
	{
	    ipc_sem_pm_qos_req_list[i].pm_qos_class = 0;
	}
	spin_lock_init(&ipc_sem_pm_qos_lock);

	g_CoreNum = IPC_CORE_APPARM;

	memset((void*)stIpcIntTable, 0x0, (INTSRC_NUM * sizeof(BSP_IPC_ENTRY) ));/*lint665,modifyby z00212992,2012.4.20*/

	ipc_base = ioremap(BSP_IPC_BASE_ADDR, IPC_REG_SIZE);
	if (!ipc_base)
	{
		printk(KERN_ERR "ipc ioremap error.\n");
		return -1;
	}

    /*дȫ0�������л���Դ*/
	BSP_RegWr(BSP_IPC_CPU_INT_MASK(g_CoreNum), 0x0);
	BSP_RegWr(BSP_IPC_SEM_INT_MASK(g_CoreNum), 0x0);

    /*дȫ1��������ж�*/
    BSP_RegWr(BSP_IPC_CPU_INT_CLR_ACPU, UCOM_COMM_UINT32_MAX);

	/* 	�ҽ��ܵ��жϷ�����򣬰����жϻ������жϷ��������ź����ͷ��жϷ������*/
	ret = request_irq(INT_LEV_IPC_CPU, BSP_DRV_IpcIntHandler, 0, "ipc_irq", NULL);
	if (ret ) {
		printk(KERN_ERR "BSP_DRV_IPCIntInit: Unable to register ipc irq ret=%d.\n", ret);
		return BSP_ERROR;
	}

	ret = request_irq(INT_LEV_IPC_SEM, BSP_IPC_SemIntHandler, 0, "ipc_sem", NULL);
	if (ret ) {
		printk(KERN_ERR "BSP_DRV_IPCIntInit: Unable to register sem irq ret=%d.\n", ret);
		return BSP_ERROR;
	}

	g_stIpcDev.bInit = BSP_TRUE;

	return BSP_OK;
}

/*****************************************************************************
* �� �� ��  : BSP_IPC_SemCreate
*
* ��������  : ��ʼ���ź���
*
* �������  :   BSP_U32 u32SignalNum Ҫ��ʼ�����ź������

* �������  : ��
*
* �� �� ֵ  : ��
*
* �޸ļ�¼  : 2011��4��11�� wangjing creat
*****************************************************************************/
BSP_S32 BSP_IPC_SemCreate(BSP_U32 u32SignalNum)
{
    IPC_CHECK_PARA(u32SignalNum);
	sema_init(&g_semIpcTask[u32SignalNum], SEM_EMPTY);

    return BSP_OK;
}

/*****************************************************************************
* �� �� ��  : BSP_IPC_IntEnable
*
* ��������  : ʹ��ĳ���ж�
*
* �������  :   IPC_INT_CORE_E enCoreNum Ҫʹ���жϵ�core
                BSP_U32 ulLvl Ҫʹ�ܵ��жϺţ�ȡֵ��Χ0��31
* �������  : ��
*
* �� �� ֵ  : OK&ERROR
*
* �޸ļ�¼  : 2011��4��11�� wangjing creat
*****************************************************************************/
BSP_S32 BSP_IPC_IntEnable (IPC_INT_LEV_E ulLvl)
{
	/*�������*/
	IPC_CHECK_PARA(ulLvl);

	/*д�ж����μĴ���*/
    BSP_RegWr(BSP_IPC_CPU_INT_MASK_DIS_ACPU, 1 << ulLvl);

	return BSP_OK;
}


/*****************************************************************************
 * �� �� ��  : BSP_INT_Disable
 *
 * ��������  : ȥʹ��ĳ���ж�
 *
 * �������  : IPC_INT_CORE_E enCoreNum Ҫʹ���жϵ�core
                BSP_U32 ulLvl Ҫʹ�ܵ��жϺţ�ȡֵ��Χ0��31
 * �������  : ��
 *
 * �� �� ֵ  : OK&ERROR
 *
 * �޸ļ�¼  : 2011��4��11�� wangjing creat
 *****************************************************************************/
BSP_S32 BSP_IPC_IntDisable (IPC_INT_LEV_E ulLvl)
{
	/*�������*/
	IPC_CHECK_PARA(ulLvl);

	/*д�ж����μĴ���*/
	BSP_RegWr(BSP_IPC_CPU_INT_MASK_EN_ACPU, 1 << ulLvl);

	return BSP_OK;
}

/*****************************************************************************
 * �� �� ��  : BSP_IPC_IntConnect
 *
 * ��������  : ע��ĳ���ж�
 *
 * �������  : IPC_INT_CORE_E enCoreNum Ҫʹ���жϵ�core
               BSP_U32 ulLvl Ҫʹ�ܵ��жϺţ�ȡֵ��Χ0��31
               VOIDFUNCPTR routine �жϷ������
 *             BSP_U32 parameter      �жϷ���������
 * �������  : ��
 *
 * �� �� ֵ  : OK&ERROR
 *
 * �޸ļ�¼  : 2011��4��11�� wangjing creat
 *****************************************************************************/
BSP_S32 BSP_IPC_IntConnect  (IPC_INT_LEV_E ulLvl, VOIDFUNCPTR routine, BSP_U32 parameter)
{

	unsigned long flag = 0;/*lint530,modify by z00212992,2012.4.18*/

	/*�������*/
	IPC_CHECK_PARA(ulLvl);

	spin_lock_irqsave(&g_ipc_int_lock, flag);
	stIpcIntTable[ulLvl].routine = routine;
	stIpcIntTable[ulLvl].arg = parameter;
	spin_unlock_irqrestore(&g_ipc_int_lock, flag);

	return BSP_OK;
}

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
 * �޸ļ�¼  : 2011��4��11�� wangjing creat
 *****************************************************************************/
 BSP_S32 BSP_IPC_IntDisonnect  (IPC_INT_LEV_E ulLvl,VOIDFUNCPTR routine, BSP_U32 parameter)
 {
	unsigned long flag = 0;/*lint530,modify by z00212992,2012.4.18*/

	/*�������*/
	IPC_CHECK_PARA(ulLvl);

	spin_lock_irqsave(&g_ipc_int_lock, flag);
	stIpcIntTable[ulLvl].routine = NULL;
	stIpcIntTable[ulLvl].arg = 0;
	spin_unlock_irqrestore(&g_ipc_int_lock, flag);

	return BSP_OK;
 }

BSP_S32 ffsLsb(BSP_S32 args)
{
	BSP_S32 num = 0;
	BSP_S32 s32ImpVal = args;

	for(;;)
	{
		num++;

		if (0x1 == (s32ImpVal & 0x1))
		{
			break;
		}
		s32ImpVal = (BSP_S32)((BSP_U32)s32ImpVal >> 1);
	}

	return num;
}

 /*****************************************************************************
 * �� �� ��  : BSP_DRV_IpcIntHandler
 *
 * ��������  : �жϴ�����
 *
 * �������  : ��
 * �������  : ��
 *
 * �� �� ֵ  : ��
 *
 * �޸ļ�¼  : 2011��4��11�� wangjing creat
 *****************************************************************************/
static irqreturn_t BSP_DRV_IpcIntHandler(int irq, void *dev_id)
{
	BSP_S32 retval = IRQ_HANDLED;
	BSP_U32 newLevel = 0;
	BSP_U32 u32IntStat = 0;

	u32IntStat = BSP_RegRd(BSP_IPC_CPU_INT_STAT(g_CoreNum));

	if(0 == u32IntStat)/*lint539,modify by z00212992,2012.4.18*/
	{
		printk("BSP_DRV_IPCIntHandler: Receive ipc int, but ipc status is 0!\n");
		return (irqreturn_t)IRQ_RETVAL(retval);/*lint64,modify by z00212992,2012.4.17*/
	}
	newLevel = ffsLsb (u32IntStat);
	--newLevel;		/* ffsLsb returns numbers from 1, not 0 */

	if (check_himntn(HIMNTN_PRINTK_WHO_WAKEUP_ACPU)) {
		printk(KERN_INFO"%s:%d:Receive ipc bit:[%d]\n", __func__, __LINE__, newLevel);
	}

	/*���ж�*/
	BSP_RegWr(BSP_IPC_CPU_INT_CLR(g_CoreNum), 1 << newLevel);
	g_stIpc_debug.u32IntHandleTimes[newLevel]++;

	/*����ע����жϴ�����*/
	if (NULL != stIpcIntTable[newLevel].routine)
	{
		stIpcIntTable[newLevel].routine(stIpcIntTable[newLevel].arg);
	}

	return (irqreturn_t)IRQ_RETVAL(retval);/*lint64,modify by z00212992,2012.4.17*/
}

/*****************************************************************************
* �� �� ��  : BSP_IPC_IntSend
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
* �޸ļ�¼  : 2011��4��11�� wangjing creat
*****************************************************************************/
BSP_S32 BSP_IPC_IntSend(IPC_INT_CORE_E enDstCore, IPC_INT_LEV_E ulLvl)
{
	/*�������*/
	IPC_CHECK_PARA(ulLvl);

	/*дԭʼ�жϼĴ���,�����ж�*/
	BSP_RegWr(BSP_IPC_CPU_RAW_INT(enDstCore), 1 << ulLvl);
	g_stIpc_debug.u32RecvIntCore = enDstCore;
	g_stIpc_debug.u32IntSendTimes[ulLvl]++;

	return BSP_OK;
}

/*****************************************************************************
 * �� �� ��  : BSP_MaskInt
 *
 * ��������  : �����ź���������ж��ͷżĴ���
 *
 * �������  :
                BSP_U32 ulLvl Ҫ���͵��жϺţ�ȡֵ��Χ0��31
 * �������  : ��
 *
 * �� �� ֵ  : ��
 *
 * �޸ļ�¼  : 2011��4��11�� wangjing creat
 *****************************************************************************/

BSP_VOID BSP_MaskInt(BSP_U32 u32SignalNum)
{
	BSP_U32 u32IntMask = 0;
	/*lint82,modify by z00212992,2012.4.17*/
	if(u32SignalNum >= INTSRC_NUM)
	{
		printk("BSP_MaskInt  Parameter error, line:%d\n", __LINE__);
		return;
	}

	/*д�ж����μĴ���*/
	u32IntMask = BSP_RegRd(BSP_IPC_SEM_INT_MASK(g_CoreNum));
	u32IntMask = (BSP_U32)(u32IntMask & (~(1 << u32SignalNum))); /*lint !e502*/
	BSP_RegWr(BSP_IPC_SEM_INT_MASK(g_CoreNum), u32IntMask);
}

BSP_VOID TEST_ACPUIDLE_IPC_DEBUG_ON(int on)
{
    debug_on = on;
}

/*0:create Sem, 1:take, 2:give*/
BSP_VOID TEST_ACPUIDLE_IPC_SEM_REQ(int take_or_give)
{
    /*TODO*/
    /*
    IPC_SEM_EMMC
    BSP_IPC_SemCreate
    BSP_IPC_SemTake
    BSP_IPC_SemGive
    */

    if(0==take_or_give)
    {
        BSP_IPC_SemCreate(IPC_SEM_EMMC);
    }
    else if(1==take_or_give)
    {
        BSP_IPC_SemTake(IPC_SEM_EMMC, 2000);
    }else{
        BSP_IPC_SemGive(IPC_SEM_EMMC);
    }
}

BSP_VOID IPC_ACPUIDLE_Add_Request(BSP_U32 u32SignalNum)
{
    unsigned long flags;
    pm_qos_debug_trace("SignalNum [%d] add request PM_QOS_CPU_INT_LATENCY", u32SignalNum);

    if(0==ipc_sem_pm_qos_refcnt[u32SignalNum])
    {
        pm_qos_add_request(&ipc_sem_pm_qos_req_list[u32SignalNum], 
                                        PM_QOS_CPU_INT_LATENCY, IPC_QOS + u32SignalNum);
    }

    spin_lock_irqsave(&ipc_sem_pm_qos_lock, flags);
    ipc_sem_pm_qos_refcnt[u32SignalNum]++;
    spin_unlock_irqrestore(&ipc_sem_pm_qos_lock, flags);
}


BSP_VOID IPC_ACPUIDLE_Remove_Request(BSP_U32 u32SignalNum)
{
    unsigned long flags;


    pm_qos_debug_trace("SignalNum [%d] remove request PM_QOS_CPU_INT_LATENCY", u32SignalNum);

    if(0==ipc_sem_pm_qos_refcnt[u32SignalNum])
    {
        return;
    }

    if(1==ipc_sem_pm_qos_refcnt[u32SignalNum])
    {
        pm_qos_remove_request(&ipc_sem_pm_qos_req_list[u32SignalNum]);
    }

    spin_lock_irqsave(&ipc_sem_pm_qos_lock, flags);
    ipc_sem_pm_qos_refcnt[u32SignalNum]--;
    spin_unlock_irqrestore(&ipc_sem_pm_qos_lock, flags);
    
}

 /*****************************************************************************
 * �� �� ��  : BSP_IPC_SemTake
 *
 * ��������  : ��ȡ�ź���
 *
 * �������  : ��
 * �������  : ��
 *
 * �� �� ֵ  : ��
 *
 * �޸ļ�¼  : 2011��4��11�� wangjing creat
 *****************************************************************************/
 BSP_S32 BSP_IPC_SemTake(BSP_U32 u32SignalNum,BSP_S32 s32timeout)
 {
	BSP_U32 u32HsCtrl = 0;
	BSP_U32 u32IntMask = 0;

	/*�������*/
	IPC_CHECK_PARA(u32SignalNum);

	/*��������ź�����Ӧ���ͷ��ж�����*/
	BSP_RegWr(BSP_IPC_SEM_INT_CLR(g_CoreNum), 1 << u32SignalNum);
	while (1)
	{
		u32HsCtrl = BSP_RegRd(BSP_IPC_HS_CTRL(g_CoreNum, u32SignalNum));
		if (0 == u32HsCtrl)
		{
			/*�ź�����ռ�ɹ�,��������˵��ź����ͷ��ж�*/
			BSP_MaskInt(u32SignalNum);
			g_stIpc_debug.u32SemId = u32SignalNum;
			g_stIpc_debug.u32SemTakeTimes[u32SignalNum]++;/*lint !e661*/
			break;
		}
		else
		{
			/*ʹ���ź����ͷ��ж�*/
			u32IntMask = BSP_RegRd(BSP_IPC_SEM_INT_MASK(g_CoreNum));
			u32IntMask = (BSP_U32)(u32IntMask | ((1 << u32SignalNum)));
			BSP_RegWr(BSP_IPC_SEM_INT_MASK(g_CoreNum), u32IntMask);
			//g_SemTakeTimes++;
			
			IPC_ACPUIDLE_Add_Request(u32SignalNum);
			
			if (0 != down_timeout(&g_semIpcTask[u32SignalNum], msecs_to_jiffies(s32timeout * 10)))
			{
				/*ȥʹ���ź����ͷ��ж�*/
				BSP_MaskInt(u32SignalNum);
				printk(KERN_ERR "semTake timeout!\n");
				IPC_ACPUIDLE_Remove_Request(u32SignalNum);
				return BSP_ERROR;
			}
			
			IPC_ACPUIDLE_Remove_Request(u32SignalNum);
		}
	}

	return BSP_OK;
 }

 /*****************************************************************************
 * �� �� ��  : BSP_IPC_SemGive
 *
 * ��������  : �ͷ��ź���
 *
 * �������  : ��
 * �������  : ��
 *
 * �� �� ֵ  : ��
 *
 * �޸ļ�¼  : 2011��4��11�� wangjing creat
 *****************************************************************************/
 BSP_VOID BSP_IPC_SemGive(BSP_U32 u32SignalNum)
 {
	/*lint82,modify by z00212992,2012.4.17*/
	if(u32SignalNum >= INTSRC_NUM)
	{
		printk("BSP_IPC_SemGive  Parameter error, line:%d\n", __LINE__);
		return;
	}
	/*lint82,modify by z00212992,2012.4.17*/
	/*���ź�������Ĵ�����0*/
	BSP_RegWr(BSP_IPC_HS_CTRL(g_CoreNum, u32SignalNum), 0);
	g_stIpc_debug.u32SemGiveTimes[u32SignalNum]++;
 }
/*Modify_for_c_reset, l00212112,20130511, starts*/
/*****************************************************************************
 * �� �� ��  : BSP_IPC_SemGive_Ccore_All
 *
 * ��������  : �ͷ�C�������ź���(��C�˵�����λ��Ŀ����)
 *
 * �������  : ��
 * �������  : ��
 *
 * �� �� ֵ  : ��
 *
 * �޸ļ�¼  : 2013��5��10�� wangjing creat
 *****************************************************************************/
 BSP_VOID BSP_IPC_SemGive_Ccore_All(BSP_VOID)
 {
    BSP_U32     i = 0;
    
    for (i = 0;i < INTSRC_NUM;i++)
    {
    	/*���ź�������Ĵ�����0*/
    	BSP_RegWr(BSP_IPC_HS_CTRL(IPC_CORE_CCPU, i), 0);
    	g_stIpc_debug.u32SemGiveTimes[i]++;
        
    }
 }
/*Modify_for_c_reset, l00212112,20130511, ends*/
/*****************************************************************************
 * �� �� ��  : BSP_IPC_SemIntHandler
 *
 * ��������  : �ź����ͷ��жϴ�����
 *
 * �������  : ��
 * �������  : ��
 *
 * �� �� ֵ  : ��
 *
 * �޸ļ�¼  : 2011��4��11�� wangjing creat
 *****************************************************************************/
static irqreturn_t BSP_IPC_SemIntHandler(int irq, void *dev_id)
{
 	BSP_S32 retval = IRQ_HANDLED;
	BSP_U32 u32SNum = 0;
	BSP_U32 u32IntStat = 0;

	u32IntStat = BSP_RegRd(BSP_IPC_SEM_INT_STAT(g_CoreNum));

	/*������ź����ͷ��жϣ�������ж�*/
	u32SNum = ffsLsb (u32IntStat);
	BSP_RegWr(BSP_IPC_SEM_INT_CLR(g_CoreNum), 1 << --u32SNum);

	up(&g_semIpcTask[u32SNum]);

	return (irqreturn_t)IRQ_RETVAL(retval);/*lint64,modify by z00212992,2012.4.17*/
}

/*****************************************************************************
* �� �� ��  : BSP_IPC_SpinLock
*
* ��������  : ��ѯ�ȴ���ȡ�ź���
*
* �������  : ��
* �������  : ��
*
* �� �� ֵ  : ��
*
* �޸ļ�¼  : 2011��4��11�� wangjing creat
*****************************************************************************/
BSP_VOID BSP_IPC_SpinLock (BSP_U32 u32SignalNum)
{
	BSP_U32 u32HsCtrl;

	/*lint82,modify by z00212992,2012.4.17*/
	if(u32SignalNum >= INTSRC_NUM)
	{
		printk("BSP_IPC_SpinLock  Parameter error, line:%d\n", __LINE__);
		return;
	}
	/*lint82,modify by z00212992,2012.4.17*/
	for(;;)
	{
		u32HsCtrl = BSP_RegRd(BSP_IPC_HS_CTRL(g_CoreNum, u32SignalNum));
		if (0 == u32HsCtrl)
		{
			break;
		}
	}
}

/*****************************************************************************
* �� �� ��  : BSP_IPC_SpinUnLock
*
* ��������  : �ͷ��ź���
*
* �������  : ��
* �������  : ��
*
* �� �� ֵ  : ��
*
* �޸ļ�¼  : 2011��4��11�� wangjing creat
*****************************************************************************/
BSP_VOID BSP_IPC_SpinUnLock (BSP_U32 u32SignalNum)
{
	/*lint82,modify by z00212992,2012.4.17*/
	if(u32SignalNum >= INTSRC_NUM)
	{
		printk("BSP_IPC_SpinUnLock  Parameter error, line:%d\n", __LINE__);
		return;
	}
	/*lint82,modify by z00212992,2012.4.17*/
	/*���ź�������Ĵ�����0*/
	BSP_RegWr(BSP_IPC_HS_CTRL(g_CoreNum, u32SignalNum), 0);
}



/*****************************************************************************
* �� �� ��  : BSP_IPC_DebugShow
*
* ��������  : ��ά�ɲ�ӿ�
*
* �������  : ��
* �������  : ��
*
* �� �� ֵ  : ��
*
* �޸ļ�¼  : 2011��4��11�� wangjing creat
*****************************************************************************/
BSP_VOID BSP_IPC_DebugShow(void)
{
	BSP_U32 i;

	printk("\n��ǰռ�õ��ź���IDΪ       : \t%d\n", g_stIpc_debug.u32SemId);
	for (i = 0; i < INTSRC_NUM; i++)
	{
		printk("�ź���%d��ȡ����             : \t%d\n", i, g_stIpc_debug.u32SemTakeTimes[i]);
		printk("�ź���%d�ͷŴ���             : \t%d\n", i, g_stIpc_debug.u32SemGiveTimes[i]);
		printk("�����жϵĴ���Ϊ             : \t%d\n", g_stIpc_debug.u32IntSendTimes[i]);
		printk("�����жϴ���Ĵ���Ϊ         : \t%d\n", g_stIpc_debug.u32IntHandleTimes[i]);
	}

	printk("�����жϵ�Core IDΪ          : \t%d\n", g_stIpc_debug.u32RecvIntCore);
}

EXPORT_SYMBOL(BSP_IPC_IntEnable);
EXPORT_SYMBOL(BSP_IPC_IntDisable);
EXPORT_SYMBOL(BSP_IPC_IntConnect);
EXPORT_SYMBOL(BSP_IPC_IntSend);
EXPORT_SYMBOL(BSP_IPC_SemCreate);
EXPORT_SYMBOL(BSP_IPC_SemTake);
EXPORT_SYMBOL(BSP_IPC_SemGive);
EXPORT_SYMBOL(BSP_IPC_SpinLock);
EXPORT_SYMBOL(BSP_IPC_SpinUnLock);

#ifdef __cplusplus
}
#endif


/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : pwrctrl_sleep.c
  �� �� ��   : ����
  ��    ��   : ������ 56193
  ��������   : 2012��7��25��
  ����޸�   :
  ��������   : �͹���˯�ߵײ�ģ��
  �����б�   :
              pwrctrl_check_irq_pending_status
              pwrctrl_deep_sleep
              pwrctrl_l2c_disable
              pwrctrl_l2c_enable
              pwrctrl_restore_ip_reg
              pwrctrl_sleep_initial
              pwrctrl_sleep_tele_vote_lock
              pwrctrl_sleep_tele_vote_unlock
              pwrctrl_sleep_tele_vote_wake_status
              pwrctrl_store_ip_reg

  �޸���ʷ   :
  1.��    ��   : 2012��7��25��
    ��    ��   : ������ 56193
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#ifndef WIN32
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/pm.h>
#include <linux/suspend.h>
#include <linux/cpu.h>
#include <linux/notifier.h>
#include <linux/reboot.h>
#include <asm/memory.h>
#include <mach/system.h>
#include <mach/platform.h>
#include <linux/delay.h>
#include <linux/suspend.h>
#include <asm/mach/time.h>
#include <mach/irqs.h>
#include <mach/hipm.h>
#include <mach/early-debug.h>
#include <linux/android_pmem.h>
#include <asm/hardware/gic.h>
#include <asm/hardware/arm_timer.h>
#include <linux/slab.h>
#include <asm/io.h>
#include <mach/io.h>
#include <mach/gpio.h>
#include <linux/clk.h>
#include <linux/wakelock.h>
#endif
#include <mach/pwrctrl/pwrctrl_common.h>
#include <mach/pwrctrl/pwrctrl_perimgr.h>
#include <mach/pwrctrl/pwrctrl_sleepmgr.h>
#include <mach/pwrctrl/pwrctrl_sleep.h>

#include <asm/cacheflush.h>
#include "pwrctrl_multi_memcfg.h"
#include "drv_ipcm.h"
extern void l2x0_flush_all(void);
extern void l2x0_clean_all(void);
extern void l2x0_inv_all(void);

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
u32_t g_aAcpuStoreReg[PWC_STORE_MEM_SIZE] = {0};

u32_t g_aAcpuHwVoteBaseAddr[] =
{
    IO_ADDRESS(SOC_AO_SCTRL_SC_MCPU_VOTEEN0_ADDR(SOC_SC_ON_BASE_ADDR)),
    IO_ADDRESS(SOC_AO_SCTRL_SC_PERI_VOTEEN0_ADDR(SOC_SC_ON_BASE_ADDR)),
    IO_ADDRESS(SOC_AO_SCTRL_SC_ACPU_VOTEEN0_ADDR(SOC_SC_ON_BASE_ADDR)),
    IO_ADDRESS(SOC_AO_SCTRL_SC_MCU_VOTEEN0_ADDR(SOC_SC_ON_BASE_ADDR)),
    IO_ADDRESS(SOC_AO_SCTRL_SC_MCU_VOTE1EN0_ADDR(SOC_SC_ON_BASE_ADDR)),
    IO_ADDRESS(SOC_AO_SCTRL_SC_MCU_VOTE2EN0_ADDR(SOC_SC_ON_BASE_ADDR)),

};

ST_STORE_REG_ADDR_INFO g_aAcpuSocRegTable[] =
{
    /*SCU*/
    {(ACPU_SCU_BASE_VA),             (2)},

    /* GIC */
    {(ACPU_GIC_ICCEOIR_VA),          (1)},
    {(ACPU_GIC_ICABPR_VA),           (1)},
    {(ACPU_GIC_ICDISR_VA),           (5)},
    {(ACPU_GIC_ICDISER_VA),          (5)},
    {(ACPU_GIC_ICDISPR_VA),          (5)},
    {(ACPU_GIC_ICDIPR_VA),           (40)},
    {(ACPU_GIC_ICDSGIR_VA),          (1)},
    {(ACPU_GIC_ICDIPTR_VA),          (64*2)},
    {(ACPU_GIC_ICCICR_VA),           (3)},
    {(ACPU_GIC_ICDDCR_VA),           (1)},

    /* L2 cache */
    {(ACPU_L2CC_AUX_CTRL_VA),        (1)},
    {(ACPU_L2CC_DLTNCY_CTRL_VA),     (1)},
    {(ACPU_L2CC_PREFETCH_CTRL_VA),   (1)},
    {(ACPU_L2CC_PWR_CTRL_VA),        (1)},

};


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
/*****************************************************************************
 �� �� ��  : pwrctrl_sleep_initial
 ��������  : �͹��ĵײ��ʼ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��8��23��
    ��    ��   : ������ 56193
    �޸�����   : �����ɺ���

*****************************************************************************/
s32_t pwrctrl_sleep_initial ( void_t )
{

    pwrctrl_arm_init();

    /*�޸ĵ����������pcָ���λ��:*/
    pwrctrl_write_reg32(IO_ADDRESS(PWRCTRL_ACPU_ASM_SPACE_ADDR), PWRCTRL_JMP_INSTRUCTION);


    /*remap to sram*/
    pwrctrl_set_bits(IO_ADDRESS(SOC_AO_SCTRL_SC_SECURITY_CTRL2_ADDR(SOC_SC_ON_BASE_ADDR)), \
                     BIT(SOC_AO_SCTRL_SC_SECURITY_CTRL2_remap_sram_aarm_START));

    return RET_OK;
}

/*****************************************************************************
 �� �� ��  : pwrctrl_store_ip_reg
 ��������  : ����soc�µ���ip�ļĴ���
 �������  : ��
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��

 �޸���ʷ      :
  1.��    ��   : 2012��8��23��
    ��    ��   : ������ 56193
    �޸�����   : �����ɺ���

*****************************************************************************/
s32_t pwrctrl_store_ip_reg ( u32_t * addr )
{
    u32_t * ulbakAddr;
    u32_t i;

    ulbakAddr = addr;
    for (i = 0; i < (sizeof(g_aAcpuSocRegTable) / sizeof(ST_STORE_REG_ADDR_INFO)); i++)
    {
        pwrctrl_mem_cpy((void *)ulbakAddr, (void *)g_aAcpuSocRegTable[i].ulStartAddr, g_aAcpuSocRegTable[i].ulLength);
        ulbakAddr += g_aAcpuSocRegTable[i].ulLength;
    }

    return RET_OK;
}

/*****************************************************************************
 �� �� ��  : pwrctrl_restore_ip_reg
 ��������  : �ָ�soc�µ���ip�ļĴ���
 �������  : ��
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��

 �޸���ʷ      :
  1.��    ��   : 2012��8��23��
    ��    ��   : ������ 56193
    �޸�����   : �����ɺ���

*****************************************************************************/
s32_t pwrctrl_restore_ip_reg( u32_t * addr)
{
    u32_t * ulbakAddr;
    u32_t i;

    ulbakAddr = addr;
    for (i = 0; i < (sizeof(g_aAcpuSocRegTable) / sizeof(ST_STORE_REG_ADDR_INFO)); i++)
    {
        pwrctrl_mem_cpy((void *)g_aAcpuSocRegTable[i].ulStartAddr, (void *)ulbakAddr, g_aAcpuSocRegTable[i].ulLength);
        ulbakAddr += g_aAcpuSocRegTable[i].ulLength;
    }

    return RET_OK;
}

/*****************************************************************************
 �� �� ��  : pwrctrl_l2c_disable
 ��������  : l2cach disable
 �������  : ��
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��

 �޸���ʷ      :
  1.��    ��   : 2012��8��25��
    ��    ��   : ������ 56193
    �޸�����   : �����ɺ���(��Ҫ�޸�arm/mm�µ�cache-l2x0.c�ļ�)

*****************************************************************************/
s32_t pwrctrl_l2c_disable( void_t )
{
    /* flush cache all */
    l2x0_flush_all();

    /* disable l2x0 cache */
    pwrctrl_write_reg32(ACPU_L2CC_CTRL_VA,PWRCTRL_SWITCH_OFF);

    /* barrier */
    dmb();

    return RET_OK;
}

/*****************************************************************************
 �� �� ��  : pwrctrl_l2c_enable
 ��������  : l2cach enable
 �������  : ��
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��

 �޸���ʷ      :
  1.��    ��   : 2012��8��25��
    ��    ��   : ������ 56193
    �޸�����   : �����ɺ���(��Ҫ�޸�arm/mm�µ�cache-l2x0.c�ļ�)

*****************************************************************************/
s32_t pwrctrl_l2c_enable( void_t )
{
    /* invalidate l2x0 cache */
    l2x0_inv_all();

    /* enable l2x0 cache */
    pwrctrl_write_reg32(ACPU_L2CC_CTRL_VA,PWRCTRL_SWITCH_ON);

    mb();

    return RET_OK;
}


/*****************************************************************************
 �� �� ��  : PWRCTRL_CheckIrqPendingStatus
 ��������  : ����ж�״̬
 �������  : ��
 �������  : ��
 �� �� ֵ  : TRUE:���жϣ�FALSE:���ж�

 �޸���ʷ      :
  1.��    ��   : 2012��8��23��
    ��    ��   : ������ 56193
    �޸�����   : �����ɺ���

*****************************************************************************/
s32_t pwrctrl_check_irq_pending_status ( void_t )
{
    u32_t i, j;
    u32_t pendings32_tNum = ACPU_GIC_MAX_NUM;
    u32_t gicIcdIsprRegValue = 0;
    unsigned long key = 0;

    local_irq_save(key);
    for (i = 0; i < ACPU_GIC_ICDABR_NUM; i++)
    {
        gicIcdIsprRegValue = PWRCTRL_GET_GIC_STATUS(i);
        if (0 == gicIcdIsprRegValue)
        {
            continue;
        }
        else
        {
            for (j = 0; j < sizeof(u32_t); j++)
            {
                if ((gicIcdIsprRegValue & (1 << j)))
                {
                    pendings32_tNum = ((i * sizeof(u32_t)) + j);
                    PWC_TRACE(PWC_LOG_DEBUG, "GIC Pending %d  ... \n", pendings32_tNum);
                }
            }
        }
    }

    local_irq_restore(key);

    /*����Com ARM����DEBUGģ���ͨ�����������ж�*/
    if (((ACPU_COMMTX_CORE0 <= pendings32_tNum) && (ACPU_COMMTX_CORE3 >= pendings32_tNum)) \
     || (ACPU_GIC_MAX_NUM == pendings32_tNum))
    {
        return PWRCTRL_TRUE;
    }
    else
    {
        PWC_TRACE(PWC_LOG_DEBUG, "Can't sleep: GIC Pending %d  ... \n", pendings32_tNum);
        return PWRCTRL_FALSE;
    }
}


/*****************************************************************************
 �� �� ��  : pwrctrl_deep_sleep
 ��������  : ARM �������߻���
 �������  : ��
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��30��
    ��    ��   : ������ 56193
    �޸�����   : �����ɺ���

*****************************************************************************/
s32_t  pwrctrl_deep_sleep(suspend_state_t state)
{
    unsigned long key = 0;

    /*SOC�µ���IP�Ĵ��������ַ*/
    u32_t * pulSocRegBakAddr = (u32_t *)g_aAcpuStoreReg;

    if(state != PM_SUSPEND_MEM)
    {
        return RET_ERR;
    }

    if (RET_OK != pwrctrl_is_func_on(PWC_SWITCH_ASLEEP))
    {
        return RET_ERR;
    }

    if (has_wake_lock(WAKE_LOCK_SUSPEND))
    {
        printk("%s: has wake lock\n", __func__);
        return -EAGAIN;
    }

    local_irq_save(key);

    BSP_IPC_SpinLock(IPC_SEM_SMP_CPU0);
    g_stAcpuPwcExcLog->core0.SlpCnt ++;
    g_stAcpuPwcExcLog->core0.SlpMgrSTm = pwrctrl_get_slice_time();

    PWC_TRACE(PWC_LOG_DEBUG, "PM:Enter V9R1 arch deep sleep\n");
    /*���ж�pending�Ĵ��������жϾ��˳�˯��*/
    if (PWRCTRL_TRUE == pwrctrl_check_irq_pending_status())
    {

        flush_cache_all();

        pwrctrl_l2c_disable();

        pwrctrl_store_ip_reg(pulSocRegBakAddr);

        /*PD ENTRY*/
        pwrctrl_asm_deep_sleep_entry();
        pwrctrl_restore_ip_reg(pulSocRegBakAddr);
        pulSocRegBakAddr = NULL;

        pwrctrl_l2c_enable();

    }
    else
    {
        /*just for pclint*/
    }
    PWC_TRACE(PWC_LOG_DEBUG, "PM:Exit V9R1 arch deep sleep and wake src info\n");
    g_stAcpuPwcExcLog->core0.WkCnt ++;
    g_stAcpuPwcExcLog->core0.WkMgrSTm = pwrctrl_get_slice_time();
    BSP_IPC_SpinUnLock(IPC_SEM_SMP_CPU0);

    pwrctrl_mcu_debug_info_show();
    pwrctrl_ccpu_debug_info_show();
    pwrctrl_acpu_debug_info_show();
    /* unlock  s32_terrupt   */
    local_irq_restore(key);

    return RET_OK;
}

/*****************************************************************************
 �� �� ��  : pwrctrl_sleep_tele_vote_lock
 ��������  : Զ��ͶƱ��ֹ˯�߽ӿ�
 �������  : teleModeId         Զ��ģ��ID
             enClientId         ͶƱģ��ID
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��8��9��
    ��    ��   : ������ 56193
    �޸�����   : �����ɺ���

*****************************************************************************/
s32_t pwrctrl_sleep_tele_vote_lock(u32_t teleModeId, u32_t enClientId)
{
    ST_VOTE_HW_INFO * pStVoteInfo = NULL ;

    pStVoteInfo = (ST_VOTE_HW_INFO *)g_aAcpuHwVoteBaseAddr[teleModeId];
    pwrctrl_set_bits((int)(&(pStVoteInfo->voteEn)), BIT(enClientId));

    return RET_OK;
}

/*****************************************************************************
 �� �� ��  : pwrctrl_sleep_tele_vote_unlock
 ��������  : Զ��ͶƱ����˯�߽ӿ�
 �������  : teleModeId         Զ��ģ��ID
             enClientId         ͶƱģ��ID
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��8��9��
    ��    ��   : ������ 56193
    �޸�����   : �����ɺ���

*****************************************************************************/
s32_t pwrctrl_sleep_tele_vote_unlock( u32_t teleModeId, u32_t enClientId )
{
    ST_VOTE_HW_INFO * pStVoteInfo = NULL ;

    pStVoteInfo = (ST_VOTE_HW_INFO *)g_aAcpuHwVoteBaseAddr[teleModeId];
    pwrctrl_set_bits((int)(&(pStVoteInfo->voteDis)), BIT(enClientId));

    return RET_OK;
}

/*****************************************************************************
 �� �� ��  : pwrctrl_sleep_tele_vote_wake_status
 ��������  : Զ��ͶƱ�Ļ���ͶƱ״̬���
 �������  : teleModeId         Զ��ģ��ID
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��8��10��
    ��    ��   : ������ 56193
    �޸�����   : �����ɺ���

*****************************************************************************/
s32_t pwrctrl_sleep_tele_vote_wake_status(u32_t teleModeId)
{
    ST_VOTE_HW_INFO * pStVoteInfo = NULL ;

    pStVoteInfo = (ST_VOTE_HW_INFO *)g_aAcpuHwVoteBaseAddr[teleModeId];
    return pwrctrl_read_reg32((u32_t)(&(pStVoteInfo->voteWakeStat)));

}



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


/*
 * Copyright (c) 2011 Hisilicon Technologies Co., Ltd. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <linux/err.h>
#include <linux/io.h>
#include <linux/debugfs.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/stddef.h>
#include <linux/sysfs.h>
#include <linux/types.h>
#include <linux/mutex.h>

#include <linux/interrupt.h>
#include <asm/irq.h>
#include <linux/err.h>
#include <linux/module.h>
#include <linux/reboot.h>
#include <linux/slab.h>
#include <linux/pm_qos_params.h>
#include <linux/delay.h>
#include <linux/timer.h>
#ifdef CONFIG_DEBUG_FS
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#endif
#include <mach/util.h>				/* For mach_call_usermoduleshell*/

#include "tsensor_driver.h"
#include "drv_regulator_user.h"
#include "reg_ops.h"
#include "soc_baseaddr_interface.h"
#include "soc_sctrl_interface.h"
#include "soc_ao_sctrl_interface.h"
#include "NvIdList.h"
#include "balong_nvim.h"
#include "soc_smart_interface.h"
#include "reg_ops.h"
#include "linux/io.h"
#include "linux/spinlock.h"
#include "efuse_ctrl.h"
#include <linux/delay.h>

#define TSENSOR_READ_TEMP_MAX_COUNT     (255)

/*tsensor NV����*/
static struct tsensor_config tsensor_conf=
{
    .enable = 0x03,

    .lag_value0 = 10,   /*�ͻ�10��(10/0.7),��λ����ֵ��Ӧ���¶�ֵΪ180/255=0.7031�棬���÷�Χ0~21.9��*/
    .lag_value1 = 10,

    .thres_value0 = 95,  /*�澯���޶�95��(95+40)/0.7�����¶ȸ���140�棬����ֵΪ0xff��������ֵ֮��ļ��Ϊ180/255=0.7031��*/
    .thres_value1 = 95,

    .reset_thres_value0 = 120, /*��λ���޶�120��(120+40)/0.7�����¶ȸ���140�棬����ֵΪ0xff��������ֵ֮��ļ��Ϊ180/255=0.7031��*/
    .reset_thres_value1 = 120,

    .alarm_count1 = 5,
    .alarm_count2 = 300,
    .resume_count = 30,

    /*��λMHz*/
    .acpu_maxfreq = 800,
    .gpu_maxfreq = 120,
    .ddr_maxfreq = 360,
};

/*tsensor0 ������Ϣ*/
struct tsensor_info tsensor0_info =
{
    .name = "tsensor0",
    .wqname = "tsensor0_alarm_proc",
    .irqname = "tsensor0-irq",

    .irqnum = IRQ_TSENSOR0,

    .tsensor_alarm_flag = TSENSOR_ALARAM_OFF,
    .tsensor_alarm_count = 0,
    .tsensor_resume_count = 0,
    .hot_temp_num = 0,

    .cpu_profile_index = 0,
    .gpu_profile_index = 0,
    .ddr_profile_index = 0,

    .cpu_max_profile = 0,
    .gpu_max_profile = 0,
    .ddr_max_profile = 0,

    .last_access = TSENSOR_LOC_BUFF,

    .maxfreq_lock = false,
    .exceed_temperature_time = 0,

    .tsensor_init = tsensor0_init,
    .tsensor_suspend = tsensor0_suspend,
    .tsensor_irq = tsensor0_irq,
    .tsensor_irq_delaywork = tsensor0_irq_delaywork,
    .tsensor_temp_protect = tsensor0_temp_protect,
};

/*tsensor1 ������Ϣ*/
struct tsensor_info tsensor1_info =
{
    .name = "tsensor1",
    .wqname = "tsensor1_alarm_proc",
    .irqname = "tsensor1-irq",

    .irqnum = IRQ_TSENSOR1,

    .tsensor_alarm_flag = TSENSOR_ALARAM_OFF,
    .tsensor_alarm_count = 0,
    .tsensor_resume_count = 0,
    .hot_temp_num = 0,

    .cpu_profile_index = 0,
    .gpu_profile_index = 0,
    .ddr_profile_index = 0,

    .cpu_max_profile = 0,
    .gpu_max_profile = 0,
    .ddr_max_profile = 0,

    .last_access = TSENSOR_LOC_BUFF,

    .maxfreq_lock = false,
    .exceed_temperature_time = 0,

    .tsensor_init = tsensor1_init,
    .tsensor_suspend = tsensor1_suspend,
    .tsensor_irq = tsensor1_irq,
    .tsensor_irq_delaywork = tsensor1_irq_delaywork,
    .tsensor_temp_protect = tsensor1_temp_protect,
};

struct tsensor_info *tsensor_list[TSENSOR_TYPE_BUFF] =
{
    &tsensor0_info,
    &tsensor1_info,
};


static struct pm_qos_request_list tsensor_cpu_max_limits;
static struct pm_qos_request_list tsensor_gpu_max_limits;
static struct pm_qos_request_list tsensor_ddr_max_limits;



/*acpu��Ƶ�ȼ�*/
static int acpu_profile_lock[] =
{
    208000,
    416000,
    624000,
    798000,
    1196000,
    1596000
};

/*gpu��Ƶ�ȼ�*/
static int gpu_profile_lock[] =
{
    160000,
    266000,
    355000,
    533000
};

/*ddr��Ƶ�ȼ�*/
static int ddr_profile_lock[] =
{
    150000,
    266000,
    360000,
    533000,
    800000
};

extern struct tsensor_governor *tsensor_gov;

/* pm_qos �ӿڶ���*/
struct pm_qos_lst {
    struct pm_qos_request_list *lst;
    int qos_class;
    s32 dvalue;
};


/*pm qos����list*/
static struct pm_qos_lst pm_qos_tsensor_list[] = {
    {&tsensor_cpu_max_limits, PM_QOS_CPU_MAX_PROFILE, PM_QOS_CPU_MAXPROFILE_DEFAULT_VALUE},
    {&tsensor_gpu_max_limits, PM_QOS_GPU_MAX_PROFILE, PM_QOS_GPU_MAXPROFILE_DEFAULT_VALUE},
    {&tsensor_ddr_max_limits, PM_QOS_DDR_MAX_PROFILE, PM_QOS_DDR_MAXPROFILE_DEFAULT_VALUE}
};


/*tsensor efuse trimֵ*/
struct efuse_trim efuse_trim_data = {0, 0, 0, 0};

/******************** CPU GPU FOR TEMP LIMIT***************************/
void tsensor_pm_qos_add(void)
{

    int i = 0;
    int ilength = sizeof(pm_qos_tsensor_list)/sizeof(struct pm_qos_lst);


    for (i = 0; i < ilength; i++) {
        pm_qos_add_request(pm_qos_tsensor_list[i].lst, pm_qos_tsensor_list[i].qos_class,
            pm_qos_tsensor_list[i].dvalue);
    }

}

void tsensor_pm_qos_remove(void)
{

    int i = 0;
    int ilength = sizeof(pm_qos_tsensor_list)/sizeof(struct pm_qos_lst);

    for (i = 0; i < ilength; i++)
        pm_qos_remove_request(pm_qos_tsensor_list[i].lst);

}


/*****************************************************************************
 �� �� ��  : tsensor0_localremote_temp_get
 ��������  : tsensor0���¶Ȼ�ȡ����
 �������  :
            remotelocal: TSENSOR_TYPE_LOCAL. local; TSENSOR_TYPE_REMOTE. remote
            count:������ȡ�Ĵ���
 �������  : ��
 �� �� ֵ  : �¶�ֵ
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��6��
    ��    ��   : ���� 00217270
    �޸�����   : �����ɺ���

*****************************************************************************/
int tsensor0_localremote_temp_get(enum_tsensor_location remotelocal, int count)
{
    int regvalue = 0;
    int i = 0;

    if (tsensor_conf.enable == 0)
        return 0;

    mutex_lock(&(tsensor0_info.lock));

    if (count > TSENSOR_READ_TEMP_MAX_COUNT)
        count = TSENSOR_READ_TEMP_MAX_COUNT;

    if (tsensor0_info.last_access != remotelocal)
    {
        /*��TSensor��SC_TEMP0_EN. temp0_en_rawΪ1'b1�ڼ䣬ֻ�ܶ�ȡһ��ͨ�����¶ȣ����
            local sensorͨ����remote sensorͨ����Ҫ�л�ͨ����ȡ�¶�ֵ������Ҫ��TSensor��
            SC_TEMP0_EN. temp0_en_raw����Ϊ1'b0������������Ϊ1'b1��*/

        phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                   SOC_SCTRL_SC_TEMP0_EN_ADDR(CALC_REG_OFFSET),
                   SOC_SCTRL_SC_TEMP0_EN_temp0_en_raw_START,
                   SOC_SCTRL_SC_TEMP0_EN_temp0_en_raw_END,
                   0x00);

        phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                   SOC_SCTRL_SC_TEMP0_EN_ADDR(CALC_REG_OFFSET),
                   SOC_SCTRL_SC_TEMP0_EN_temp0_en_raw_START,
                   SOC_SCTRL_SC_TEMP0_EN_temp0_en_raw_END,
                   0x01);

        if (remotelocal == TSENSOR_LOC_REMOTE)
        {
            /*��ȡremote sensor*/
            phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                       SOC_SCTRL_SC_TEMP0_CFG_ADDR(CALC_REG_OFFSET),
                       SOC_SCTRL_SC_TEMP0_CFG_temp0_test_raw_START,
                       SOC_SCTRL_SC_TEMP0_CFG_temp0_test_raw_END,
                       0x01);
        }
        else
        {
            /*��ȡlocal sensor*/
            phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                       SOC_SCTRL_SC_TEMP0_CFG_ADDR(CALC_REG_OFFSET),
                       SOC_SCTRL_SC_TEMP0_CFG_temp0_test_raw_START,
                       SOC_SCTRL_SC_TEMP0_CFG_temp0_test_raw_END,
                       0x00);
        }
        tsensor0_info.last_access  = remotelocal;

        mdelay(2);
    }

    for (i = 0; i < count; i++)
    {
        regvalue += phy_reg_readl(SOC_SC_OFF_BASE_ADDR,
               SOC_SCTRL_SC_TEMP0_VALUE_ADDR(CALC_REG_OFFSET),
               SOC_SCTRL_SC_TEMP0_VALUE_temp0_out_s_START,
               SOC_SCTRL_SC_TEMP0_VALUE_temp0_out_s_END);

        mdelay(1);

    }
    regvalue = regvalue /count;

    printk(KERN_INFO"tsensor0_localremote_temp_get regvalue %d\n", regvalue);

    /*ת��Ϊ�¶�
    ˵����8Bits����������������¶������������¶���֮��ļ��Ϊ180/255=0.7031
    �档����ֵ��ת������Ϊ0.25��*/
    if (remotelocal == TSENSOR_LOC_LOCAL)
    {
        regvalue = ((regvalue *180) / 255) - 40 - efuse_trim_data.acpu_local/4;
    }
    else
    {
        regvalue = ((regvalue *180) / 255) - 40 - efuse_trim_data.acpu_remote/4;
    }

    mutex_unlock(&(tsensor0_info.lock));

    return regvalue;
}

/*****************************************************************************
 �� �� ��  : tsensor0_temp_get
 ��������  : ͨ��һ���㷨��local/remote temp����ó�tsensor���¶�
 �������  : ��
 �������  : ��
 �� �� ֵ  : �¶�ֵ
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��6��
    ��    ��   : ���� 00217270
    �޸�����   : �����ɺ���

*****************************************************************************/
int tsensor0_temp_get(void)
{
    int tempvalue= 0;

    /*��ʱ��ȡlocal���¶ȣ������㷨оƬ��û�и�������Ҫ��Ƭʵ�����*/
    tempvalue = tsensor0_localremote_temp_get(TSENSOR_LOC_LOCAL, 5);

    return tempvalue;
}

/*****************************************************************************
 �� �� ��  : tsensor0_alarmstate_set
 ��������  : tsensor0�澯״̬���ú���
 �������  :
            onoff: tsensor״̬ö��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��6��
    ��    ��   : ���� 00217270
    �޸�����   : �����ɺ���

*****************************************************************************/
void tsensor0_alarmstate_set(enum_tsensor_alarm onoff)
{
    printk(KERN_INFO"tsensor0_alarmstate_set onoff %d\n", onoff);

    mutex_lock(&(tsensor0_info.lock));

    tsensor0_info.tsensor_alarm_flag = onoff;

    mutex_unlock(&(tsensor0_info.lock));

}

/*****************************************************************************
 �� �� ��  : tsensor0_alarmstate_set
 ��������  : tsensor0�澯״̬��ȡ����
 �������  : ��
 �������  : ��
 �� �� ֵ  : tsensor״̬
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��6��
    ��    ��   : ���� 00217270
    �޸�����   : �����ɺ���

*****************************************************************************/
enum_tsensor_alarm tsensor0_alarmstate_get(void)
{
    enum_tsensor_alarm onoff;

    mutex_lock(&(tsensor0_info.lock));

    onoff = tsensor0_info.tsensor_alarm_flag;

    mutex_unlock(&(tsensor0_info.lock));

    return onoff;
}

/*****************************************************************************
 �� �� ��  : tsensor0_irq
 ��������  : tsensor0�жϴ�����
 �������  : ��
 �������  : ��
 �� �� ֵ  : tsensor״̬
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��6��
    ��    ��   : ���� 00217270
    �޸�����   : �����ɺ���

*****************************************************************************/
irqreturn_t tsensor0_irq(int irq, void *data)
{
    /*����ж�*/
    phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
           SOC_SCTRL_SC_TEMP0_INT_CLR_ADDR(CALC_REG_OFFSET),
           SOC_SCTRL_SC_TEMP0_INT_CLR_temp0_int_clr_START,
           SOC_SCTRL_SC_TEMP0_INT_CLR_temp0_int_clr_END,
           0x01);

    /*�����ж�*/
    phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
           SOC_SCTRL_SC_TEMP0_INT_MSK_ADDR(CALC_REG_OFFSET),
           SOC_SCTRL_SC_TEMP0_INT_MSK_temp0_int_msk_START,
           SOC_SCTRL_SC_TEMP0_INT_MSK_temp0_int_msk_END,
           0x00);

    printk(KERN_INFO"tsensor0_irq\n");
    /*����half bottom������*/
    queue_work(tsensor0_info.tsensor_alaram_wq, &tsensor0_info.tsensor_alarm_wk);


    /*ʹ���ж�*/
    phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
           SOC_SCTRL_SC_TEMP0_INT_MSK_ADDR(CALC_REG_OFFSET),
           SOC_SCTRL_SC_TEMP0_INT_MSK_temp0_int_msk_START,
           SOC_SCTRL_SC_TEMP0_INT_MSK_temp0_int_msk_END,
           0x01);

    return IRQ_HANDLED;
}

/*****************************************************************************
 �� �� ��  : tsensor0_irq_delaywork
 ��������  : tsensor0�жϵװ벿�ӳٴ�����
 �������  : ��
 �������  : ��
 �� �� ֵ  : tsensor״̬
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��6��
    ��    ��   : ���� 00217270
    �޸�����   : �����ɺ���

*****************************************************************************/
void tsensor0_irq_delaywork(struct work_struct *work)
{
    if (tsensor_gov == NULL)
        return ;

    /*tsensor״̬���ý���澯̬*/
    tsensor0_alarmstate_set(TSENSOR_ALARAM_ON);

    mutex_lock(&tsensor_gov->lock);

    printk(KERN_INFO"tsensor0_irq_delaywork\n");
    /*�����¶���ѯ*/
    cancel_delayed_work_sync(&tsensor_gov->tsensor_monitor_work);
    schedule_delayed_work(&tsensor_gov->tsensor_monitor_work, msecs_to_jiffies(0));

    mutex_unlock(&tsensor_gov->lock);
}

/*****************************************************************************
 �� �� ��  : tsensor0_suspend
 ��������  : tsensor0������
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��6��
    ��    ��   : ���� 00217270
    �޸�����   : �����ɺ���

*****************************************************************************/
void tsensor0_suspend(void)
{

    /*ȥʹ���¶ȼ��ģ��*/
    phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
               SOC_SCTRL_SC_TEMP0_EN_ADDR(CALC_REG_OFFSET),
               SOC_SCTRL_SC_TEMP0_EN_temp0_en_raw_START,
               SOC_SCTRL_SC_TEMP0_EN_temp0_en_raw_END,
               0x01);

    /*�����¶ȼ�⸴λ*/
   phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
              SOC_SCTRL_SC_TEMP0_RST_MSK_ADDR(CALC_REG_OFFSET),
              SOC_SCTRL_SC_TEMP0_RST_MSK_temp0_rst_msk_START,
              SOC_SCTRL_SC_TEMP0_RST_MSK_temp0_rst_msk_END,
              0x00);

   /*�����¶ȼ���жϣ�*/
   phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
              SOC_SCTRL_SC_TEMP0_INT_MSK_ADDR(CALC_REG_OFFSET),
              SOC_SCTRL_SC_TEMP0_INT_MSK_temp0_int_msk_START,
              SOC_SCTRL_SC_TEMP0_INT_MSK_temp0_int_msk_END,
              0x00);

    /*�ر�ʱ��*/
    phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
               SOC_SCTRL_SC_PERIPH_CLKEN2_ADDR(CALC_REG_OFFSET),
               SOC_SCTRL_SC_PERIPH_CLKEN2_periph_clken2_tsensor0_clk_START,
               SOC_SCTRL_SC_PERIPH_CLKEN2_periph_clken2_tsensor0_clk_END,
               0);
}

/*****************************************************************************
 �� �� ��  : tsensor0_init
 ��������  : tsensor0��ʼ������
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��6��
    ��    ��   : ���� 00217270
    �޸�����   : �����ɺ���

*****************************************************************************/
void tsensor0_init(void)
{
        /*���� 1 ϵͳ�ϵ�Ĭ��TSensor0�ĵ�Դ�򿪣�AVDD DVDD��������DVDD��ϵͳ������һ�𹩵磬����ϵͳ�������µ�ʱ��TSensor�޷�������*/

        /*���� 2 �������SC_PERIPH_CLKEN2.
        periph_clken2_tsensor0_clkΪ1'b1����TSensor0��ʱ�ӣ�*/
        phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                   SOC_SCTRL_SC_PERIPH_CLKEN2_ADDR(CALC_REG_OFFSET),
                   SOC_SCTRL_SC_PERIPH_CLKEN2_periph_clken2_tsensor0_clk_START,
                   SOC_SCTRL_SC_PERIPH_CLKEN2_periph_clken2_tsensor0_clk_END,
                   1);

        /*���� 3 �������SC_TEMP0_LAG.temp0_lag������TSensor0�¶ȱȽ��ͻط�Χ��*/
        phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                   SOC_SCTRL_SC_TEMP0_LAG_ADDR(CALC_REG_OFFSET),
                   SOC_SCTRL_SC_TEMP0_LAG_temp0_lag_START,
                   SOC_SCTRL_SC_TEMP0_LAG_temp0_lag_END,
                   tsensor0_info.lag_value);

        /*���� 4 �������SC_TEMP0_TH. temp0_0/1/2/3th������TSensor0�¶���ֵ��*/

        /*Ĭ������Ϊ�������*/
        phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                   SOC_SCTRL_SC_TEMP0_TH_ADDR(CALC_REG_OFFSET),
                   SOC_SCTRL_SC_TEMP0_TH_temp0_0th_START,
                   SOC_SCTRL_SC_TEMP0_TH_temp0_3th_END,
                   0xffffffff);

        /*ֻʹ��0th����*/
        phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                   SOC_SCTRL_SC_TEMP0_TH_ADDR(CALC_REG_OFFSET),
                   SOC_SCTRL_SC_TEMP0_TH_temp0_0th_START,
                   SOC_SCTRL_SC_TEMP0_TH_temp0_0th_END,
                   tsensor0_info.thres_value);


        /*���� 5 �������SC_TEMP0_RST_TH. temp0_rst_th������TSensor0��λ��ֵ��*/
        phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                   SOC_SCTRL_SC_TEMP0_RST_TH_ADDR(CALC_REG_OFFSET),
                   SOC_SCTRL_SC_TEMP0_RST_TH_temp0_rst_th_START,
                   SOC_SCTRL_SC_TEMP0_RST_TH_temp0_rst_th_END,
                   tsensor0_info.reset_thres_value);

        /*���� 6 �������SC_TEMP0_CFG. temp0_test_rawΪ3'b000����ʾ��ȡTSensor0��local sensorͨ�����¶ȣ�
        ������SC_TEMP0_CFG. temp0_test_rawΪ3'b001����ʾ��ȡTSensor0��remote  sensorͨ�����¶ȣ�
        ����ȡTSensor0��remote
        sensorͨ���¶ȣ�����������ƽ�����Լ�С�¶ȸ�����*/
        phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                   SOC_SCTRL_SC_TEMP0_CFG_ADDR(CALC_REG_OFFSET),
                   SOC_SCTRL_SC_TEMP0_CFG_temp0_test_raw_START,
                   SOC_SCTRL_SC_TEMP0_CFG_temp0_test_raw_END,
                   0x00);

        /*���� 7 �������SC_TEMP0_CFG.temp0_ct_sel_rawΪ2'b00������Tsensor0HKADCת��ʱ��Ϊ0.768ms��*/
        phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                   SOC_SCTRL_SC_TEMP0_CFG_ADDR(CALC_REG_OFFSET),
                   SOC_SCTRL_SC_TEMP0_CFG_temp0_ct_sel_raw_START,
                   SOC_SCTRL_SC_TEMP0_CFG_temp0_ct_sel_raw_END,
                   0x00);

        /*���� 8 �������SC_TEMP0_RST_MSK. temp0_rst_msk����Tsensor0�¶ȼ�⸴λ���Σ�*/
        phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                   SOC_SCTRL_SC_TEMP0_RST_MSK_ADDR(CALC_REG_OFFSET),
                   SOC_SCTRL_SC_TEMP0_RST_MSK_temp0_rst_msk_START,
                   SOC_SCTRL_SC_TEMP0_RST_MSK_temp0_rst_msk_END,
                   0x01);

        /*���� 9 �������SC_TEMP0_INT_MSK. temp0_int_msk����Tsensor0�¶ȼ���ж����Σ�*/
        phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                   SOC_SCTRL_SC_TEMP0_INT_MSK_ADDR(CALC_REG_OFFSET),
                   SOC_SCTRL_SC_TEMP0_INT_MSK_temp0_int_msk_START,
                   SOC_SCTRL_SC_TEMP0_INT_MSK_temp0_int_msk_END,
                   0x01);

        /*���� 10 �������SC_TEMP0_EN. temp0_en_rawΪ1'b1��ʹ���¶ȼ��ģ�飬��ʼ����AARM���¶ȣ�
        ��SC_TEMP0_EN. temp0_en_rawΪ1'b1�ڼ䣬����¶�����0.768msˢ��һ�Σ�*/
        phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                   SOC_SCTRL_SC_TEMP0_EN_ADDR(CALC_REG_OFFSET),
                   SOC_SCTRL_SC_TEMP0_EN_temp0_en_raw_START,
                   SOC_SCTRL_SC_TEMP0_EN_temp0_en_raw_END,
                   0x01);

            /*mdelay(2);*/

        /*���� 11 �����ȡSC_TEMP0_VALUE.temp0_out_s��ֵ������¶����ݣ�*/

        /*���� 12 �����ȡ�¶ȳ���������ж��¶���ֵ���¶ȼ��ģ����Զ������¶���ֵ�жϣ�*/

        /*���� 13
        �����ȡ�¶ȳ�������ڸ�λ�¶���ֵ���¶ȼ��ģ����Զ�������λ����λ����оƬ��*/

        /*printk(KERN_INFO"tsensor0_init exit\n");*/
}

/*****************************************************************************
 �� �� ��  : tsensor0_temp_protect
 ��������  : tsensor0�¶ȱ�������
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��6��
    ��    ��   : ���� 00217270
    �޸�����   : �����ɺ���

*****************************************************************************/
void tsensor0_temp_protect(void)
{
    int tempvalue = 0;
    unsigned int count;

    tempvalue = tsensor0_temp_get();

    printk(KERN_INFO"tsensor0_temp_protect tempvalue %d\n", tempvalue);

    if (tempvalue > (int)tsensor_conf.thres_value0)
    {
        tsensor0_info.tsensor_resume_count = 0;
        tsensor0_info.tsensor_alarm_count++;

        printk(KERN_INFO"tsensor0_temp_protect tempvalue %d, thres_value0 %d\n", \
            tempvalue, tsensor_conf.thres_value0);

        if (tsensor0_info.maxfreq_lock)
        {
            count = tsensor_conf.alarm_count2;
        }
        else
        {
            /*��һ��ʹ��alarmcount1������ */
            count = tsensor_conf.alarm_count1;
        }

        printk(KERN_INFO"tsensor0_temp_protect tsensor_alarm_count %d, count %d\n", \
            tsensor0_info.tsensor_alarm_count, count);

        if (tsensor0_info.tsensor_alarm_count >= count)
        {
            printk(KERN_ALERT"tsensor0_temp_protect pm_qos_update_request acpu profile_index %d, profile %d\n", \
                tsensor0_info.cpu_profile_index, acpu_profile_lock[tsensor0_info.cpu_profile_index]);

            printk(KERN_ALERT"tsensor0_temp_protect pm_qos_update_request ddr profile_index %d, profile %d\n", \
                tsensor0_info.ddr_profile_index, ddr_profile_lock[tsensor0_info.ddr_profile_index]);

            tsensor0_alarmstate_set(TSENSOR_ALARAM_ON);

#if 0
            printk(KERN_INFO"============cpu tsensor kernel_power_off tempvalue %d =============\n", tempvalue);

            /*�����ã�֮��ƽ̨�ȶ���ĳ���Ƶ*/
            /*kernel_power_off();*/
            mach_call_usermodeshell("/system/etc/log/thermal_reboot.sh", 0);
            mdelay(10000);

#else
            /*�����������ޣ�����maxƵ�� */
            pm_qos_update_request(&tsensor_cpu_max_limits, acpu_profile_lock[tsensor0_info.cpu_profile_index]);
            pm_qos_update_request(&tsensor_ddr_max_limits, ddr_profile_lock[tsensor0_info.ddr_profile_index]);

#endif

            tsensor0_info.maxfreq_lock = true;
            tsensor0_info.tsensor_alarm_count = 0;

            if (tsensor0_info.cpu_profile_index > 0)
                tsensor0_info.cpu_profile_index--;


            if (tsensor0_info.ddr_profile_index > 0)
                tsensor0_info.ddr_profile_index--;
        }

    }
    else if ((tsensor0_alarmstate_get() == TSENSOR_ALARAM_ON) &&
        (tempvalue < (int)(tsensor_conf.thres_value0 - tsensor_conf.lag_value0)))
    {
        tsensor0_info.tsensor_resume_count++;
        tsensor0_info.tsensor_alarm_count = 0;

        printk(KERN_INFO"tsensor0_temp_protect tsensor_resume_count %d, resume_count %d\n", \
            tsensor0_info.tsensor_resume_count, tsensor_conf.resume_count);

        if (tsensor0_info.tsensor_resume_count >= tsensor_conf.resume_count)
        {

            printk(KERN_ALERT"tsensor0_temp_protect pm_qos_update_request remove\n");

            /*�����������ޣ�ȡ������maxƵ��*/
            pm_qos_update_request(&tsensor_cpu_max_limits,  PM_QOS_CPU_MAXPROFILE_DEFAULT_VALUE);
            pm_qos_update_request(&tsensor_ddr_max_limits,  PM_QOS_DDR_MAXPROFILE_DEFAULT_VALUE);

            tsensor0_alarmstate_set(TSENSOR_ALARAM_OFF);

            tsensor0_info.cpu_profile_index = tsensor0_info.cpu_max_profile;
            tsensor0_info.ddr_profile_index = tsensor0_info.ddr_max_profile;

            tsensor0_info.tsensor_resume_count = 0;
            tsensor0_info.tsensor_alarm_count = 0;
            tsensor0_info.hot_temp_num++;

            tsensor0_info.maxfreq_lock = false;
        }
    }

}


/*****************************************************************************
 �� �� ��  : tsensor1_localremote_temp_get
 ��������  : tsensor1���¶Ȼ�ȡ����
 �������  :
            remotelocal: TSENSOR_TYPE_LOCAL. local; TSENSOR_TYPE_REMOTE. remote
            count:������ȡ�Ĵ���
 �������  : ��
 �� �� ֵ  : �¶�ֵ
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��6��
    ��    ��   : ���� 00217270
    �޸�����   : �����ɺ���

*****************************************************************************/
int tsensor1_localremote_temp_get(enum_tsensor_location remotelocal, int count)
{
    int regvalue = 0;
    int i = 0;

    if (tsensor_conf.enable == 0)
        return 0;


    mutex_lock(&(tsensor1_info.lock));

    if (count > TSENSOR_READ_TEMP_MAX_COUNT)
        count = TSENSOR_READ_TEMP_MAX_COUNT;

    if (tsensor1_info.last_access != remotelocal)
    {
        /*��TSensor��SC_TEMP0_EN. temp0_en_rawΪ1'b1�ڼ䣬ֻ�ܶ�ȡһ��ͨ�����¶ȣ����
            local sensorͨ����remote sensorͨ����Ҫ�л�ͨ����ȡ�¶�ֵ������Ҫ��TSensor��
            SC_TEMP0_EN. temp0_en_raw����Ϊ1'b0������������Ϊ1'b1��*/

        phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                   SOC_SCTRL_SC_TEMP1_EN_ADDR(CALC_REG_OFFSET),
                   SOC_SCTRL_SC_TEMP1_EN_temp1_en_raw_START,
                   SOC_SCTRL_SC_TEMP1_EN_temp1_en_raw_END,
                   0x00);

        phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                   SOC_SCTRL_SC_TEMP1_EN_ADDR(CALC_REG_OFFSET),
                   SOC_SCTRL_SC_TEMP1_EN_temp1_en_raw_START,
                   SOC_SCTRL_SC_TEMP1_EN_temp1_en_raw_END,
                   0x01);


        if (remotelocal == TSENSOR_LOC_REMOTE)
        {
            /*��ȡremote sensor */
            phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                       SOC_SCTRL_SC_TEMP1_CFG_ADDR(CALC_REG_OFFSET),
                       SOC_SCTRL_SC_TEMP1_CFG_temp1_test_raw_START,
                       SOC_SCTRL_SC_TEMP1_CFG_temp1_test_raw_END,
                       0x01);
        }
        else
        {
            /*��ȡlocal sensor*/
            phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                       SOC_SCTRL_SC_TEMP1_CFG_ADDR(CALC_REG_OFFSET),
                       SOC_SCTRL_SC_TEMP1_CFG_temp1_test_raw_START,
                       SOC_SCTRL_SC_TEMP1_CFG_temp1_test_raw_END,
                       0x00);
        }
        tsensor1_info.last_access = remotelocal;
        mdelay(2);
    }

    for (i = 0; i < count; i++)
    {
        regvalue += phy_reg_readl(SOC_SC_OFF_BASE_ADDR,
               SOC_SCTRL_SC_TEMP1_VALUE_ADDR(CALC_REG_OFFSET),
               SOC_SCTRL_SC_TEMP1_VALUE_temp1_out_s_START,
               SOC_SCTRL_SC_TEMP1_VALUE_temp1_out_s_END);

        mdelay(1);
    }

    regvalue = regvalue /count;

    printk(KERN_INFO"tsensor1_localremote_temp_get regvalue %d\n", regvalue);

    /*ת��Ϊ�¶�
    ˵����8Bits����������������¶������������¶���֮��ļ��Ϊ180/255=0.7031
    �档*/

    if (remotelocal == TSENSOR_LOC_LOCAL)
    {
        regvalue = ((regvalue *180) / 255) - 40 - efuse_trim_data.gpu_local/4;
    }
    else
    {
        regvalue = ((regvalue *180) / 255) - 40 - efuse_trim_data.gpu_remote/4;
    }


    mutex_unlock(&(tsensor1_info.lock));

    return regvalue;
}

/*****************************************************************************
 �� �� ��  : tsensor1_temp_get
 ��������  : ͨ��һ���㷨��local/remote temp����ó�tsensor���¶�
 �������  : ��
 �������  : ��
 �� �� ֵ  : �¶�ֵ
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��6��
    ��    ��   : ���� 00217270
    �޸�����   : �����ɺ���

*****************************************************************************/
int tsensor1_temp_get(void)
{
    int tempvalue;

    /*��ʱ��ȡlocal���¶ȣ������㷨оƬ��û�и�������Ҫ��Ƭʵ�����*/
    tempvalue = tsensor1_localremote_temp_get(TSENSOR_LOC_LOCAL, 5);

    return tempvalue;
}

/*****************************************************************************
 �� �� ��  : tsensor1_alarmstate_set
 ��������  : tsensor1�澯״̬���ú���
 �������  :
            onoff: tsensor״̬ö��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��6��
    ��    ��   : ���� 00217270
    �޸�����   : �����ɺ���

*****************************************************************************/
void tsensor1_alarmstate_set(enum_tsensor_alarm onoff)
{
    printk(KERN_INFO"tsensor1_alarmstate_set onoff %d\n", onoff);


    mutex_lock(&(tsensor1_info.lock));

    tsensor1_info.tsensor_alarm_flag = onoff;

    mutex_unlock(&(tsensor1_info.lock));

}

/*****************************************************************************
 �� �� ��  : tsensor1_alarmstate_set
 ��������  : tsensor1�澯״̬��ȡ����
 �������  : ��
 �������  : ��
 �� �� ֵ  : tsensor״̬
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��6��
    ��    ��   : ���� 00217270
    �޸�����   : �����ɺ���

*****************************************************************************/
enum_tsensor_alarm tsensor1_alarmstate_get(void)
{
    enum_tsensor_alarm onoff;


    mutex_lock(&(tsensor1_info.lock));

    onoff = tsensor1_info.tsensor_alarm_flag;

    mutex_unlock(&(tsensor1_info.lock));

    return onoff;
}

/*****************************************************************************
 �� �� ��  : tsensor1_irq
 ��������  : tsensor1�жϴ�����
 �������  : ��
 �������  : ��
 �� �� ֵ  : tsensor״̬
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��6��
    ��    ��   : ���� 00217270
    �޸�����   : �����ɺ���

*****************************************************************************/
irqreturn_t tsensor1_irq(int irq, void *data)
{
    /*����ж� */
    phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
           SOC_SCTRL_SC_TEMP1_INT_CLR_ADDR(CALC_REG_OFFSET),
           SOC_SCTRL_SC_TEMP1_INT_CLR_temp1_int_clr_START,
           SOC_SCTRL_SC_TEMP1_INT_CLR_temp1_int_clr_END,
           0x01);

    /*�����ж� */
    phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
           SOC_SCTRL_SC_TEMP1_INT_MSK_ADDR(CALC_REG_OFFSET),
           SOC_SCTRL_SC_TEMP1_INT_MSK_temp1_int_msk_START,
           SOC_SCTRL_SC_TEMP1_INT_MSK_temp1_int_msk_END,
           0x00);

    printk(KERN_INFO"tsensor1_irq\n");
    /*����half bottom������ */
    queue_work(tsensor1_info.tsensor_alaram_wq, &tsensor1_info.tsensor_alarm_wk);

    /*ʹ���ж� */
    phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
           SOC_SCTRL_SC_TEMP1_INT_MSK_ADDR(CALC_REG_OFFSET),
           SOC_SCTRL_SC_TEMP1_INT_MSK_temp1_int_msk_START,
           SOC_SCTRL_SC_TEMP1_INT_MSK_temp1_int_msk_END,
           0x01);


    return IRQ_HANDLED;
}

/*****************************************************************************
 �� �� ��  : tsensor1_irq_delaywork
 ��������  : tsensor1�жϵװ벿�ӳٴ�����
 �������  : ��
 �������  : ��
 �� �� ֵ  : tsensor״̬
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��6��
    ��    ��   : ���� 00217270
    �޸�����   : �����ɺ���

*****************************************************************************/
void tsensor1_irq_delaywork(struct work_struct *work)
{
    if (tsensor_gov == NULL)
        return ;

    tsensor1_alarmstate_set(TSENSOR_ALARAM_ON);

    mutex_lock(&(tsensor_gov->lock));

    printk(KERN_INFO"tsensor1_irq_delaywork\n");
    cancel_delayed_work_sync(&tsensor_gov->tsensor_monitor_work);
    schedule_delayed_work(&tsensor_gov->tsensor_monitor_work, msecs_to_jiffies(0));

    mutex_unlock(&(tsensor_gov->lock));
}

/*****************************************************************************
 �� �� ��  : tsensor1_suspend
 ��������  : tsensor1������
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��6��
    ��    ��   : ���� 00217270
    �޸�����   : �����ɺ���

*****************************************************************************/
void tsensor1_suspend(void)
{

    /*ȥʹ���¶ȼ��ģ�� */
    phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
               SOC_SCTRL_SC_TEMP1_EN_ADDR(CALC_REG_OFFSET),
               SOC_SCTRL_SC_TEMP1_EN_temp1_en_raw_START,
               SOC_SCTRL_SC_TEMP1_EN_temp1_en_raw_END,
               0x01);


    /*�����¶ȼ�⸴λ��*/
    phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
              SOC_SCTRL_SC_TEMP1_RST_MSK_ADDR(CALC_REG_OFFSET),
              SOC_SCTRL_SC_TEMP1_RST_MSK_temp1_rst_msk_START,
              SOC_SCTRL_SC_TEMP1_RST_MSK_temp1_rst_msk_END,
              0x00);

    /*�����¶ȼ���жϣ�*/
    phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
              SOC_SCTRL_SC_TEMP1_INT_MSK_ADDR(CALC_REG_OFFSET),
              SOC_SCTRL_SC_TEMP1_INT_MSK_temp1_int_msk_START,
              SOC_SCTRL_SC_TEMP1_INT_MSK_temp1_int_msk_END,
              0x00);


    /*�ر�ʱ��*/
    phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
               SOC_SCTRL_SC_PERIPH_CLKEN2_ADDR(CALC_REG_OFFSET),
               SOC_SCTRL_SC_PERIPH_CLKEN2_periph_clken2_tsensor1_clk_START,
               SOC_SCTRL_SC_PERIPH_CLKEN2_periph_clken2_tsensor1_clk_END,
               0);

}

/*****************************************************************************
 �� �� ��  : tsensor1_init
 ��������  : tsensor1��ʼ������
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��6��
    ��    ��   : ���� 00217270
    �޸�����   : �����ɺ���

*****************************************************************************/
void tsensor1_init(void)
{
        /*���� 1 ϵͳ�ϵ�Ĭ��TSensor0�ĵ�Դ�򿪣�AVDD DVDD��������DVDD��ϵͳ������һ�𹩵磬����ϵͳ�������µ�ʱ��TSensor�޷�������*/

        /*���� 2 �������SC_PERIPH_CLKEN2.periph_clken2_tsensor0_clkΪ1'b1����TSensor0��ʱ�ӣ�*/
        phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                   SOC_SCTRL_SC_PERIPH_CLKEN2_ADDR(CALC_REG_OFFSET),
                   SOC_SCTRL_SC_PERIPH_CLKEN2_periph_clken2_tsensor1_clk_START,
                   SOC_SCTRL_SC_PERIPH_CLKEN2_periph_clken2_tsensor1_clk_END,
                   1);

        /*���� 3 �������SC_TEMP0_LAG.temp0_lag������TSensor0�¶ȱȽ��ͻط�Χ��*/
        phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                  SOC_SCTRL_SC_TEMP1_LAG_ADDR(CALC_REG_OFFSET),
                  SOC_SCTRL_SC_TEMP1_LAG_temp1_lag_START,
                  SOC_SCTRL_SC_TEMP1_LAG_temp1_lag_END,
                  tsensor1_info.lag_value);


        /*���� 4 �������SC_TEMP0_TH. temp0_0/1/2/3th������TSensor0�¶���ֵ��*/

        /*Ĭ������Ϊ�������*/
        phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                   SOC_SCTRL_SC_TEMP1_TH_ADDR(CALC_REG_OFFSET),
                   SOC_SCTRL_SC_TEMP1_TH_temp1_0th_START,
                   SOC_SCTRL_SC_TEMP1_TH_temp1_3th_END,
                   0xffffffff);

        /*ֻʹ��0th����*/
        phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                   SOC_SCTRL_SC_TEMP1_TH_ADDR(CALC_REG_OFFSET),
                   SOC_SCTRL_SC_TEMP1_TH_temp1_0th_START,
                   SOC_SCTRL_SC_TEMP1_TH_temp1_0th_END,
                   tsensor1_info.thres_value);



        /*���� 5 �������SC_TEMP0_RST_TH. temp0_rst_th������TSensor0��λ��ֵ��*/
       phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                   SOC_SCTRL_SC_TEMP1_RST_TH_ADDR(CALC_REG_OFFSET),
                   SOC_SCTRL_SC_TEMP1_RST_TH_temp1_rst_th_START,
                   SOC_SCTRL_SC_TEMP1_RST_TH_temp1_rst_th_END,
                   tsensor1_info.reset_thres_value);

        /*���� 6 �������SC_TEMP0_CFG. temp0_test_rawΪ3'b000����ʾ��ȡTSensor0��local sensorͨ�����¶ȣ�
        ������SC_TEMP0_CFG. temp0_test_rawΪ3'b001����ʾ��ȡTSensor0��remote  sensorͨ�����¶ȣ�
        ����ȡTSensor0��remote sensorͨ���¶ȣ�����������ƽ�����Լ�С�¶ȸ�����*/
       phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                   SOC_SCTRL_SC_TEMP1_CFG_ADDR(CALC_REG_OFFSET),
                   SOC_SCTRL_SC_TEMP1_CFG_temp1_test_raw_START,
                   SOC_SCTRL_SC_TEMP1_CFG_temp1_test_raw_END,
                   0x00);

        /*���� 7 �������SC_TEMP0_CFG.temp0_ct_sel_rawΪ2'b00������Tsensor0HKADCת��ʱ��Ϊ0.768ms��*/
        phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                   SOC_SCTRL_SC_TEMP1_CFG_ADDR(CALC_REG_OFFSET),
                   SOC_SCTRL_SC_TEMP1_CFG_temp1_ct_sel_raw_START,
                   SOC_SCTRL_SC_TEMP1_CFG_temp1_ct_sel_raw_END,
                   0x00);


        /*/���� 8 �������SC_TEMP0_RST_MSK.temp0_rst_msk����Tsensor0�¶ȼ�⸴λ���Σ� */
        phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                   SOC_SCTRL_SC_TEMP1_RST_MSK_ADDR(CALC_REG_OFFSET),
                   SOC_SCTRL_SC_TEMP1_RST_MSK_temp1_rst_msk_START,
                   SOC_SCTRL_SC_TEMP1_RST_MSK_temp1_rst_msk_END,
                   0x01);

        /*���� 9 �������SC_TEMP0_INT_MSK. temp0_int_msk����Tsensor0�¶ȼ���ж����Σ�*/
        phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                   SOC_SCTRL_SC_TEMP1_INT_MSK_ADDR(CALC_REG_OFFSET),
                   SOC_SCTRL_SC_TEMP1_INT_MSK_temp1_int_msk_START,
                   SOC_SCTRL_SC_TEMP1_INT_MSK_temp1_int_msk_END,
                   0x01);

        /*���� 10 �������SC_TEMP0_EN. temp0_en_rawΪ1'b1��ʹ���¶ȼ��ģ�飬��ʼ����AARM���¶ȣ�
        ��SC_TEMP0_EN. temp0_en_rawΪ1'b1�ڼ䣬����¶�����0.768msˢ��һ�Σ�*/
        phy_reg_writel(SOC_SC_OFF_BASE_ADDR,
                   SOC_SCTRL_SC_TEMP1_EN_ADDR(CALC_REG_OFFSET),
                   SOC_SCTRL_SC_TEMP1_EN_temp1_en_raw_START,
                   SOC_SCTRL_SC_TEMP1_EN_temp1_en_raw_END,
                   0x01);

            /*mdelay(2);*/


        /*���� 11 �����ȡSC_TEMP0_VALUE.temp0_out_s��ֵ������¶����ݣ�*/

        /*���� 12 �����ȡ�¶ȳ���������ж��¶���ֵ���¶ȼ��ģ����Զ������¶���ֵ�жϣ�*/

        /*���� 13 �����ȡ�¶ȳ�������ڸ�λ�¶���ֵ���¶ȼ��ģ����Զ�������λ����λ����оƬ��*/

        /*printk(KERN_INFO"tsensor1_init exit\n");*/
}

/*****************************************************************************
 �� �� ��  : tsensor1_temp_protect
 ��������  : tsensor1�¶ȱ�������
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��6��
    ��    ��   : ���� 00217270
    �޸�����   : �����ɺ���

*****************************************************************************/
void tsensor1_temp_protect(void)
{
    int tempvalue = 0;
    unsigned int count = 0;

    tempvalue = tsensor1_temp_get();
    //printk(KERN_INFO"tsensor1_temp_protect tempvalue %d\n", tempvalue);

    if (tempvalue > (int)tsensor_conf.thres_value1)
    {
        printk(KERN_INFO"tsensor1_temp_protect tempvalue %d, thres_value1 %d\n", \
            tempvalue, tsensor_conf.thres_value1);

        tsensor1_info.tsensor_resume_count = 0;
        tsensor1_info.tsensor_alarm_count++;

        if (tsensor1_info.maxfreq_lock)
        {
            count = tsensor_conf.alarm_count2;
        }
        else
        {
            /*��һ��ʹ��alarmcount1������ */
            count = tsensor_conf.alarm_count1;
        }

        printk(KERN_INFO"tsensor1_temp_protect tsensor_alarm_count %d, count %d\n", \
            tsensor1_info.tsensor_alarm_count, count);

        if (tsensor1_info.tsensor_alarm_count >= count)
        {
            printk(KERN_ALERT"tsensor1_temp_protect pm_qos_update_request tsensor_gpu_max_limits gpu_profile_index %d, profile %d\n", \
                tsensor1_info.gpu_profile_index, gpu_profile_lock[tsensor1_info.gpu_profile_index]);

            tsensor1_alarmstate_set(TSENSOR_ALARAM_ON);

#if 0
            /*�����ã�֮��ƽ̨�ȶ���ĳ���Ƶ*/
            printk(KERN_INFO"============gpu tsensor kernel_power_off tempvalue %d =============\n", tempvalue);

            /*kernel_power_off();*/
			mach_call_usermodeshell("/system/etc/log/thermal_reboot.sh", 0);
            mdelay(10000);
#else
            /*�����������ޣ�����maxƵ�� */
            pm_qos_update_request(&tsensor_gpu_max_limits, gpu_profile_lock[tsensor1_info.gpu_profile_index]);
#endif
            if (tsensor1_info.gpu_profile_index > 0)
                tsensor1_info.gpu_profile_index--;

            tsensor1_info.maxfreq_lock = true;

            tsensor1_info.tsensor_alarm_count = 0;
        }
    }
    else if ((tsensor1_alarmstate_get() == TSENSOR_ALARAM_ON) &&
        (tempvalue < (int)(tsensor_conf.thres_value1 - tsensor_conf.lag_value1)))
    {
        tsensor1_info.tsensor_resume_count++;
        tsensor1_info.tsensor_alarm_count = 0;

        printk(KERN_INFO"tsensor1_temp_protect tsensor_resume_count %d, resume_count %d\n", \
            tsensor1_info.tsensor_resume_count, tsensor_conf.resume_count);

        if (tsensor1_info.tsensor_resume_count >= tsensor_conf.resume_count)
        {
            printk(KERN_ALERT"tsensor1_temp_protect pm_qos_update_request remove\n");

            /*�����������ޣ�ȡ������maxƵ��*/
            pm_qos_update_request(&tsensor_gpu_max_limits,  PM_QOS_GPU_MAXPROFILE_DEFAULT_VALUE);

            tsensor1_alarmstate_set(TSENSOR_ALARAM_OFF);

            tsensor1_info.gpu_profile_index = tsensor1_info.gpu_max_profile;
            tsensor1_info.tsensor_resume_count = 0;
            tsensor1_info.tsensor_alarm_count = 0;
            tsensor1_info.hot_temp_num++;

            tsensor1_info.maxfreq_lock = false;
        }
    }

}


/*****************************************************************************
 �� �� ��  : tsensor_config_init
 ��������  : tsensor���ó�ʼ������
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��6��
    ��    ��   : ���� 00217270
    �޸�����   : �����ɺ���

*****************************************************************************/
int tsensor_config_init(void)
{
    int ret = 0;
    int index = 0;
    int size = 0;


    /*��ȡNVֵ*/
    ret = DR_NV_Read(en_NV_Item_THERMAL_TSENSOR_CONFIG, &tsensor_conf, sizeof(tsensor_conf));
    if (ret != NV_OK)
    {
        pr_err("%s:DR_NV_Read tsensor configure\n", __func__);
        //return -1;
    }

    printk(KERN_INFO"tsensor config enable 0x%x, lag_value0 0x%x, lag_value1 0x%x, \
        thres_value0 %d, thres_value1 %d, reset_thres_value0 %d, reset_thres_value1 %d, \
        alarm_count1 %d, alarm_count2 %d, resum_count %d, acpu_maxfreq %d, gpu_maxfreq %d, ddr_maxfreq %d\n",
        tsensor_conf.enable, tsensor_conf.lag_value0, tsensor_conf.lag_value1,
        tsensor_conf.thres_value0, tsensor_conf.thres_value1,
        tsensor_conf.reset_thres_value0, tsensor_conf.reset_thres_value1,
        tsensor_conf.alarm_count1, tsensor_conf.alarm_count2,
        tsensor_conf.resume_count, tsensor_conf.acpu_maxfreq,
        tsensor_conf.gpu_maxfreq, tsensor_conf.ddr_maxfreq);

    /*�ͻ�10��(10/0.7),��λ����ֵ��Ӧ���¶�ֵΪ180/255=0.7031�棬���÷�Χ0~21.9��*/
    tsensor0_info.lag_value = (tsensor_conf.lag_value0 *255)/180;
    tsensor1_info.lag_value = (tsensor_conf.lag_value1 * 255)/180;

    printk(KERN_INFO"lag_value0 %x, lag_value1 %x\n",
        tsensor0_info.lag_value, tsensor1_info.lag_value);

    /*�澯���޶�95��(95+40)/0.7�����¶ȸ���140�棬����ֵΪ0xff��������ֵ֮��ļ��Ϊ180/255=0.7031��*/
    tsensor0_info.thres_value = ((tsensor_conf.thres_value0 + 40 + efuse_trim_data.acpu_local/4) *255)/180;
    tsensor0_info.thres_value = (tsensor0_info.thres_value>254)? 254 : tsensor0_info.thres_value;
    tsensor1_info.thres_value = ((tsensor_conf.thres_value1 + 40 + efuse_trim_data.gpu_local/4) *255)/180;
    tsensor1_info.thres_value = (tsensor1_info.thres_value>254)? 254 : tsensor1_info.thres_value;

    printk(KERN_INFO"thres_value0 %x, thres_value1 %x\n",
        tsensor0_info.thres_value, tsensor1_info.thres_value);

    /*��λ���޶�120��(120+40)/0.7�����¶ȸ���140�棬����ֵΪ0xff��������ֵ֮��ļ��Ϊ180/255=0.7031��*/
    tsensor0_info.reset_thres_value = ((tsensor_conf.reset_thres_value0 + 40 + efuse_trim_data.acpu_local/4) *255)/180;
    tsensor0_info.reset_thres_value = (tsensor0_info.reset_thres_value>254)? 254 : tsensor0_info.reset_thres_value;
    tsensor1_info.reset_thres_value = ((tsensor_conf.reset_thres_value1 + 40 + efuse_trim_data.gpu_local/4) *255)/180;
    tsensor1_info.reset_thres_value = (tsensor1_info.reset_thres_value>254)? 254 : tsensor1_info.reset_thres_value;

    printk(KERN_INFO"reset_thres_value0 %x, reset_thres_value1 %x\n",
        tsensor0_info.reset_thres_value, tsensor1_info.reset_thres_value);

    /*�����Ƶprofile�ȼ� */
    size = sizeof(acpu_profile_lock)/sizeof(int);
    for (index = (int)(size - 1); index >= 0; index--)
    {
        if ((tsensor_conf.acpu_maxfreq * 1000) >= acpu_profile_lock[index])
        {
            tsensor0_info.cpu_max_profile = index;
            break;
        }
    }
    tsensor0_info.cpu_profile_index = tsensor0_info.cpu_max_profile;

    size = sizeof(ddr_profile_lock)/sizeof(int);
    for (index = (int)(size - 1); index >= 0; index--)
    {
        if ((tsensor_conf.ddr_maxfreq * 1000) >= ddr_profile_lock[index])
        {
            tsensor0_info.ddr_max_profile = index;
            break;
        }
    }
    tsensor0_info.ddr_profile_index = tsensor0_info.ddr_max_profile;

    size = sizeof(gpu_profile_lock)/sizeof(int);
    for (index = (int)(size - 1); index >= 0; index--)
    {
        if ((tsensor_conf.gpu_maxfreq * 1000) >= gpu_profile_lock[index])
        {
            tsensor1_info.gpu_max_profile = index;
            break;
        }
    }
    tsensor1_info.gpu_profile_index = tsensor1_info.gpu_max_profile;

    printk(KERN_INFO"tsensor0_info cpu_profile_index %d, ddr_profile_index %d, \
        gpu_profile_index %d\n", \
        tsensor0_info.cpu_profile_index, tsensor0_info.ddr_profile_index, \
        tsensor0_info.gpu_profile_index);


    printk(KERN_INFO"tsensor1_info cpu_profile_index %d, ddr_profile_index %d, \
        gpu_profile_index %d\n", \
        tsensor1_info.cpu_profile_index, tsensor1_info.ddr_profile_index, \
        tsensor1_info.gpu_profile_index);

    return 0;
}

void efuse_trim_convert(unsigned int trimdata)
{
    efuse_trim_data.acpu_local = (trimdata & ACPUL_TRIM_DATA_MASK);
    if ((trimdata & ACPUL_TRIM_DATA_PLUSMINUS) == ACPUL_TRIM_DATA_PLUSMINUS)
        efuse_trim_data.acpu_local = 0 - efuse_trim_data.acpu_local;

    efuse_trim_data.acpu_remote = (trimdata & ACPUR_TRIM_DATA_MASK) >> 8;
    if ((trimdata & ACPUR_TRIM_DATA_PLUSMINUS) == ACPUR_TRIM_DATA_PLUSMINUS)
        efuse_trim_data.acpu_remote = 0 - efuse_trim_data.acpu_remote;

    efuse_trim_data.gpu_local = (trimdata & GPUL_TRIM_DATA_MASK) >> 16;
    if ((trimdata & GPUL_TRIM_DATA_PLUSMINUS) == GPUL_TRIM_DATA_PLUSMINUS)
        efuse_trim_data.gpu_local = 0 - efuse_trim_data.gpu_local;

    efuse_trim_data.gpu_remote = (trimdata & GPUR_TRIM_DATA_MASK) >> 24;
    if ((trimdata & GPUR_TRIM_DATA_PLUSMINUS) == GPUR_TRIM_DATA_PLUSMINUS)
        efuse_trim_data.gpu_remote = 0 - efuse_trim_data.gpu_remote;

    printk(KERN_INFO"efuse_trim_convert acpu_local %d, acpu_remote %d, gpu_local %d, gpu_remote %d\n",
        efuse_trim_data.acpu_local, efuse_trim_data.acpu_remote,
        efuse_trim_data.gpu_local, efuse_trim_data.gpu_remote);

}

/*****************************************************************************
 �� �� ��  : tsensor_init
 ��������  : tsensor��ʼ������
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��6��
    ��    ��   : ���� 00217270
    �޸�����   : �����ɺ���

*****************************************************************************/
int tsensor_init(void)
{
    int index = 0;
    int ret;
    unsigned int trimdata = 0;

    get_temp_adjust_value(&trimdata);

    printk(KERN_INFO"get_temp_adjust_efuse_value data %x\n", trimdata);

    efuse_trim_convert(trimdata);

    /*tsensor���ó�ʼ�� */
    tsensor_config_init();

    if (tsensor_conf.enable == 0)
	    return 0;

    tsensor_pm_qos_add();

    for (index = 0; index < TSENSOR_TYPE_BUFF; index++)
    {
        mutex_init(&(tsensor_list[index]->lock));

        if ((index == TSENSOR_TYPE_0) &&
            ((tsensor_conf.enable & ACPU_TSENSOR_ENABLE) != ACPU_TSENSOR_ENABLE))
            continue;

        if ((index == TSENSOR_TYPE_1) &&
            ((tsensor_conf.enable & GPU_TSENSOR_ENABLE) != GPU_TSENSOR_ENABLE))
            continue;

        /*tsensor��ʼ�� */
        tsensor_list[index]->tsensor_init();

        /*����tsensor�ж�workqueue*/
        tsensor_list[index]->tsensor_alaram_wq = create_singlethread_workqueue(tsensor_list[index]->wqname);

        INIT_WORK(&tsensor_list[index]->tsensor_alarm_wk, (void *)tsensor_list[index]->tsensor_irq_delaywork);

        /*ע��tsensor�ж� */
        ret = request_irq(tsensor_list[index]->irqnum, tsensor_list[index]->tsensor_irq, IRQF_DISABLED,
            tsensor_list[index]->irqname, tsensor_list[index]);

        if (ret) {
            pr_err("request_irq %d register interrupt failed!\n", index);
        }

    }

    printk(KERN_INFO"tsensor_init exit\n");

    return 0;
}

/*****************************************************************************
 �� �� ��  : tsensor_suspend
 ��������  : tsensor������
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��6��
    ��    ��   : ���� 00217270
    �޸�����   : �����ɺ���

*****************************************************************************/
int tsensor_suspend(void)
{
    int index = 0;

    /*ɾ��qm qos���� */
    /*tsensor_pm_qos_remove();*/

    if (tsensor_conf.enable == 0)
	    return 0;

    for (index = 0; index < TSENSOR_TYPE_BUFF; index++)
    {
        if ((index == TSENSOR_TYPE_0) &&
            ((tsensor_conf.enable & ACPU_TSENSOR_ENABLE) != ACPU_TSENSOR_ENABLE))
            continue;

        if ((index == TSENSOR_TYPE_1) &&
            ((tsensor_conf.enable & GPU_TSENSOR_ENABLE) != GPU_TSENSOR_ENABLE))
            continue;


        /*���ø���tsensor��suspend���� */
        tsensor_list[index]->tsensor_suspend();

        /*���irq */
        //free_irq(tsensor_list[index]->irqnum, NULL);
    }


    return 0;
}

/*****************************************************************************
 �� �� ��  : tsensor_resume
 ��������  : tsensor�ָ�����
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��6��
    ��    ��   : ���� 00217270
    �޸�����   : �����ɺ���

*****************************************************************************/
int tsensor_resume(void)
{

    int index = 0;

    if (tsensor_conf.enable == 0)
	    return 0;

    for (index = 0; index < TSENSOR_TYPE_BUFF; index++)
    {
        if ((index == TSENSOR_TYPE_0) &&
            ((tsensor_conf.enable & ACPU_TSENSOR_ENABLE) != ACPU_TSENSOR_ENABLE))
            continue;

        if ((index == TSENSOR_TYPE_1) &&
            ((tsensor_conf.enable & GPU_TSENSOR_ENABLE) != GPU_TSENSOR_ENABLE))
            continue;


        /*tsensor��ʼ������ */
        tsensor_list[index]->tsensor_init();

        tsensor_list[index]->last_access = TSENSOR_LOC_BUFF;
        tsensor_list[index]->tsensor_alarm_flag = TSENSOR_ALARAM_OFF;
        tsensor_list[index]->tsensor_alarm_count = 0;
        tsensor_list[index]->tsensor_resume_count = 0;
        tsensor_list[index]->maxfreq_lock = false;
        tsensor_list[index]->cpu_profile_index = tsensor_list[index]->cpu_max_profile;
        tsensor_list[index]->gpu_profile_index = tsensor_list[index]->gpu_max_profile;
        tsensor_list[index]->ddr_profile_index = tsensor_list[index]->ddr_max_profile;
        tsensor_list[index]->exceed_temperature_time = 0;

#if 0
        /*ע��tsensor�ж�*/
        ret = request_irq(tsensor_list[index]->irqnum, tsensor_list[index]->tsensor_irq, IRQF_DISABLED,
            tsensor_list[index]->irqname, tsensor_list[index]);

        if (ret) {
            pr_err("request_irq %d register interrupt failed!\n", index);
            //goto resume_err;
        }
#endif
    }

    return 0;
}

/*****************************************************************************
 �� �� ��  : tsensor_temp_protect
 ��������  : tsensor�¶ȼ�ر��������жϲ����󴥷�
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��6��
    ��    ��   : ���� 00217270
    �޸�����   : �����ɺ���

*****************************************************************************/
void tsensor_temp_protect(void)
{
    int index = 0;

    /*printk(KERN_INFO"enter tsensor_temp_protect\n");*/
    if (tsensor_gov == NULL)
        return ;

    if (tsensor_conf.enable == 0)
	    return ;

    for (index = 0; index < TSENSOR_TYPE_BUFF; index++)
    {
        if ((index == TSENSOR_TYPE_0) &&
            ((tsensor_conf.enable & ACPU_TSENSOR_ENABLE) != ACPU_TSENSOR_ENABLE))
            continue;

        if ((index == TSENSOR_TYPE_1) &&
            ((tsensor_conf.enable & GPU_TSENSOR_ENABLE) != GPU_TSENSOR_ENABLE))
            continue;

        /*ִ�и���tsensor���¶ȱ��� */
        tsensor_list[index]->tsensor_temp_protect();
    }

    /*����tsensor0,1�ĸ澯״̬���ж��Ƿ������ѯ����¶� */
    if ((tsensor0_alarmstate_get() == TSENSOR_ALARAM_ON) || (tsensor1_alarmstate_get() == TSENSOR_ALARAM_ON))
    {
        printk(KERN_INFO"schedule_delayed_work average_period %d\n", tsensor_gov->average_period);
        schedule_delayed_work(&tsensor_gov->tsensor_monitor_work,
        	                msecs_to_jiffies(tsensor_gov->average_period));
    }
}





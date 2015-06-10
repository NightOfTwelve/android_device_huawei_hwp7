/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : hkadc_ctrl.h
  �� �� ��   : ����
  ��    ��   : 
  ��������   : 2013��6��23��
  ����޸�   :
  ��������   : A���¶Ȼ�ȡģ��
                1.�ӹ����ڴ��ȡhkadc���ݵ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��6��23��
    ��    ��   : 
    �޸�����   : �����ļ�

******************************************************************************/
#include "drv_temp_cfg.h"


#include <mach/hardware.h>

#define HKADC_DEBUG(p) printk p

/*ACPUʵ��ַ�������ַӳ���ת���ӿ�*/
#define THERMAL_CORE_SHARE_PHY2VIRT(phy)  \
        (((unsigned int)phy) - MCU_SHARE_MEM_ADDR + MCU_SHARE_MEM_VIRT_ADDR)

/*ͨ�����ԺͶ�Ӧ�¶ȵ�ѹת����*/
struct hkadc_table_data
{
    unsigned int logic_chan;
    unsigned int phy_chan;
    unsigned int ctrlflag;
    struct hkadc_nv_config hkadc_conf;
    unsigned int table_len;
    struct hkadc_tem_value temp_table[32];
};

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

extern int thermal_data_init(void);
extern int hkadc_debug_init(int chanid);
extern int hkadc_debug_uninit(int chanid);
extern int hkadc_get_shared_value(int chanid, unsigned short* volt);

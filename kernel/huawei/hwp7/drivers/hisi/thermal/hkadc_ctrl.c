/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : hkadc_ctrl.c
  �� �� ��   : ����
  ��    ��   :
  ��������   : 2013��6��23��
  ����޸�   :
  ��������   : A��HKADC��ȡ����ģ��
                1.�ӹ����ڴ��ȡhkadc���ݵ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��6��23��
    ��    ��   :
    �޸�����   : �����ļ�

******************************************************************************/
#include "drv_temp_cfg.h"
#include "hkadc_value.h"
#include "hkadc_ctrl.h"
#ifdef _DRV_LLT_
#include "stdio.h"
#endif
#include <hsad/config_interface.h>

static unsigned int out_config_save[HKADC_CHAN_MAX];
static unsigned int have_config_save[HKADC_CHAN_MAX];
static unsigned short out_peroid_save[HKADC_CHAN_MAX];

struct ACPU_HKADC_PROPERTY
{
    struct hkadc_table_data* array;
    int size; 
};

struct ACPU_HKADC_PROPERTY g_acpu_hkadc_property = {NULL,0};

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
struct thermal_data_area *g_thermal_share_area =
        (struct thermal_data_area *)(THERMAL_CORE_SHARE_PHY2VIRT(M3_HKADC_DDR_ADDR));

/*ACPU hkadcͨ�����ԣ���ѹ�¶�ת����*/

/*
unsigned int outconfig;   bit0-bit1 0:����� 1:������� 2:ѭ�����
                          bit2 1:������� 0:�ǻ������
                          bit3: 0:ֻ�����ѹ 1:��ѹ�¶Ⱦ����
                          bit8: A�����
                          bit9: C�����
unsigned short outperiod;         ѭ�����ʱ��ѭ�����ڣ���λ:��
unsigned short convertlistlen;    �¶�ת������
*/
struct hkadc_table_data sophia_acpu_hkadc_property[] = 
{
    {
        /*���*/
        HKADC_TEMP_BATTERY,
        /*����ͨ��*/
        HKADC_CHAN_2,
        1,
        /*ͨ������*/
        {0x0102, 2, 0, {0, 0}},
        /*��ѹ�¶�ת������*/
        31,
        /*��ѹ�¶�ת����*/
        {   
            {-273, 2600}, {-40, 2380}, {-36, 2331}, {-32, 2275}, {-28, 2210},
            {-24, 2137}, {-20, 2057}, {-16, 1969}, {-12, 1875}, {-8, 1775},
            {-4, 1671}, {0, 1565}, {4, 1457}, {8, 1350}, {12, 1244},
            {16, 1142}, {20, 1044}, {24, 951}, {28, 863}, {32, 781},
            {36, 705}, {40, 636}, {44, 572}, {48, 515}, {52, 462},
            {56, 415}, {60, 372}, {64, 335}, {74, 257}, {84, 199},
            {125, 74}, {0, 0},
        },
    },

     {
        /*���ID*/
        HKADC_TEMP_BATT_ID,
        /*����ͨ��*/
        HKADC_CHAN_13,
        1,
        /*ͨ������*/
        {0x0101, 2, 0, {0, 0}},
        /*��ѹ�¶�ת������*/
        28,
        /*��ѹ�¶�ת����*/
        {
            {-20, 2353}, {-15, 2282}, {-10, 2197}, {-5, 2173}, {0, 2058},
            {5, 1927}, {10, 1782}, {15, 1623}, {20, 1463}, {25, 1300},
            {30, 1141}, {35, 991}, {40, 853}, {45, 729}, {50, 619},
            {55, 523}, {60, 441}, {65, 372}, {70, 313}, {75, 263},
            {80, 222}, {85, 187}, {90, 158}, {95, 134}, {100, 113},
            {105, 97}, {110, 83}, {115, 71}, {0, 0}, {0, 0},
            {0, 0}, {0, 0},
        },
    },
    {
        /*sCamera thermal*/
        HKADC_TEMP_SCAMERA,
        /*����ͨ��*/
        HKADC_CHAN_7,
        1,
        /*ͨ������*/
        {0x0102, 2, 0, {0, 0}},
        /*��ѹ�¶�ת������*/
        28,
        /*��ѹ�¶�ת����*/
        {
            {-300, 2388}, {-250, 2348}, {-200, 2297}, {-150, 2233}, {-100, 2154}, {-50, 2060}, {0, 1951},
            {50, 1827}, {100, 1691}, {150, 1547}, {200, 1398}, {250, 1250},
            {300, 1105}, {350, 967}, {400, 840}, {450, 725}, {500, 622},
            {550, 532}, {600, 454}, {650, 387}, {700, 329}, {750, 280},
            {800, 239}, {850, 204}, {900, 174}, {950, 149}, {1000, 128},
            {1050, 110}, {1100, 95}, {1150, 82}, {0, 0}, {0, 0},
        },
    },
    {
        /*Flash thermal*/
        HKADC_TEMP_FLASH,
        /*����ͨ��*/
        HKADC_CHAN_9,
        1,
        /*ͨ������*/
        {0x0102, 2, 0, {0, 0}},
        /*��ѹ�¶�ת������*/
        28,
        /*��ѹ�¶�ת����*/
        {
            {-300, 2388}, {-250, 2348}, {-200, 2297}, {-150, 2233}, {-100, 2154}, {-50, 2060}, {0, 1951},
            {50, 1827}, {100, 1691}, {150, 1547}, {200, 1398}, {250, 1250},
            {300, 1105}, {350, 967}, {400, 840}, {450, 725}, {500, 622},
            {550, 532}, {600, 454}, {650, 387}, {700, 329}, {750, 280},
            {800, 239}, {850, 204}, {900, 174}, {950, 149}, {1000, 128},
            {1050, 110}, {1100, 95}, {1150, 82}, {0, 0}, {0, 0},
        },
    },
    {
        /*ϵͳ����*/
        HKADC_TEMP_SYSTEMH,
        /*����ͨ��*/
        HKADC_CHAN_11,
        1,
        /*ͨ������*/
        {0x0102, 2, 0, {0, 0}},
        /*��ѹ�¶�ת������*/
        28,
        /*��ѹ�¶�ת����*/
        {
            {-300, 2388}, {-250, 2348}, {-200, 2297}, {-150, 2233}, {-100, 2154}, {-50, 2060}, {0, 1951},
            {50, 1827}, {100, 1691}, {150, 1547}, {200, 1398}, {250, 1250},
            {300, 1105}, {350, 967}, {400, 840}, {450, 725}, {500, 622},
            {550, 532}, {600, 454}, {650, 387}, {700, 329}, {750, 280},
            {800, 239}, {850, 204}, {900, 174}, {950, 149}, {1000, 128},
            {1050, 110}, {1100, 95}, {1150, 82}, {0, 0}, {0, 0},
        },
    },
    {
        /*ϵͳ����*/
        HKADC_TEMP_SYSTEML,
        HKADC_CHAN_12, 
        1,
        /*ͨ������*/
        {0x0102, 2, 0, {0, 0}},
        /*��ѹ�¶�ת������*/
        28,
        /*��ѹ�¶�ת����*/
        {
            {-300, 2388}, {-250, 2348}, {-200, 2297}, {-150, 2233}, {-100, 2154}, {-50, 2060}, {0, 1951},
            {50, 1827}, {100, 1691}, {150, 1547}, {200, 1398}, {250, 1250},
            {300, 1105}, {350, 967}, {400, 840}, {450, 725}, {500, 622},
            {550, 532}, {600, 454}, {650, 387}, {700, 329}, {750, 280},
            {800, 239}, {850, 204}, {900, 174}, {950, 149}, {1000, 128},
            {1050, 110}, {1100, 95}, {1150, 82}, {0, 0}, {0, 0},
        },
    } ,
    {
        HKADC_TEMP_PA0,
        HKADC_CHAN_5,
        0,
        {0x0102, 2, 0, {0, 0}},  
        28,
        {
            {-30, 2388}, {-25, 2348}, {-20, 2297}, {-15, 2233}, {-10, 2154}, {-5, 2060}, {0, 1951},
            {5, 1827}, {10, 1691}, {15, 1547}, {20, 1398}, {25, 1250},
            {30, 1105}, {35, 967}, {40, 840}, {45, 725}, {50, 622},
            {55, 532}, {60, 454}, {65, 387}, {70, 329}, {75, 280},
            {80, 239}, {85, 204}, {90, 174}, {95, 149}, {100, 128},
            {105, 110}, {110, 95}, {115, 82}, {0, 0}, {0, 0},
        },
    },
    {
        HKADC_TEMP_PA1,
        HKADC_CHAN_6,
        0,
        {0x0102, 2, 0, {0, 0}},  
        28,
        {
            {-30, 2388}, {-25, 2348}, {-20, 2297}, {-15, 2233}, {-10, 2154}, {-5, 2060}, {0, 1951},
            {5, 1827}, {10, 1691}, {15, 1547}, {20, 1398}, {25, 1250},
            {30, 1105}, {35, 967}, {40, 840}, {45, 725}, {50, 622},
            {55, 532}, {60, 454}, {65, 387}, {70, 329}, {75, 280},
            {80, 239}, {85, 204}, {90, 174}, {95, 149}, {100, 128},
            {105, 110}, {110, 95}, {115, 82}, {0, 0}, {0, 0},
        },
    }
};
struct hkadc_table_data default_acpu_hkadc_property[] = 
{
    {
        /*���*/
        HKADC_TEMP_BATTERY,
        /*����ͨ��*/
        HKADC_CHAN_2,
        1,
        /*ͨ������*/
        {0x0102, 2, 0, {0, 0}},
        /*��ѹ�¶�ת������*/
        31,
        /*��ѹ�¶�ת����*/
        {
            {-273, 2600}, {-40, 2380}, {-36, 2331}, {-32, 2275}, {-28, 2210},
            {-24, 2137}, {-20, 2057}, {-16, 1969}, {-12, 1875}, {-8, 1775},
            {-4, 1671}, {0, 1565}, {4, 1457}, {8, 1350}, {12, 1244},
            {16, 1142}, {20, 1044}, {24, 951}, {28, 863}, {32, 781},
            {36, 705}, {40, 636}, {44, 572}, {48, 515}, {52, 462},
            {56, 415}, {60, 372}, {64, 335}, {74, 257}, {84, 199},
            {125, 74}, {0, 0},
        },
    },

     {
        /*���ID*/
        HKADC_TEMP_BATT_ID,
        /*����ͨ��*/
        HKADC_CHAN_13,
        1,
        /*ͨ������*/
        {0x0101, 2, 0, {0, 0}},
        /*��ѹ�¶�ת������*/
        28,
        /*��ѹ�¶�ת����*/
        {
            {-20, 2353}, {-15, 2282}, {-10, 2197}, {-5, 2173}, {0, 2058},
            {5, 1927}, {10, 1782}, {15, 1623}, {20, 1463}, {25, 1300},
            {30, 1141}, {35, 991}, {40, 853}, {45, 729}, {50, 619},
            {55, 523}, {60, 441}, {65, 372}, {70, 313}, {75, 263},
            {80, 222}, {85, 187}, {90, 158}, {95, 134}, {100, 113},
            {105, 97}, {110, 83}, {115, 71}, {0, 0}, {0, 0},
            {0, 0}, {0, 0},
        },
    },
#if 0
     {
        /*camera0 ID*/
        HKADC_TEMP_CAMERA0_ID,
        /*����ͨ��*/
        HKADC_CHAN_9,
        1,
        /*ͨ������*/
        {0x0101, 2, 0, {0, 0}},
        /*��ѹ�¶�ת������*/
        28,
        /*��ѹ�¶�ת����*/
        {
            {-20, 2353}, {-15, 2282}, {-10, 2197}, {-5, 2173}, {0, 2058},
            {5, 1927}, {10, 1782}, {15, 1623}, {20, 1463}, {25, 1300},
            {30, 1141}, {35, 991}, {40, 853}, {45, 729}, {50, 619},
            {55, 523}, {60, 441}, {65, 372}, {70, 313}, {75, 263},
            {80, 222}, {85, 187}, {90, 158}, {95, 134}, {100, 113},
            {105, 97}, {110, 83}, {115, 71}, {0, 0}, {0, 0},
            {0, 0}, {0, 0},
        },
    },
#endif
    {
        /*camera1 ID*/
        HKADC_TEMP_CAMERA1_ID,
        /*����ͨ��*/
        HKADC_CHAN_7,
        1,
        /*ͨ������*/
        {0x0101, 2, 0, {0, 0}},
        /*��ѹ�¶�ת������*/
        28,
        /*��ѹ�¶�ת����*/
        {
            {-20, 2353}, {-15, 2282}, {-10, 2197}, {-5, 2173}, {0, 2058},
            {5, 1927}, {10, 1782}, {15, 1623}, {20, 1463}, {25, 1300},
            {30, 1141}, {35, 991}, {40, 853}, {45, 729}, {50, 619},
            {55, 523}, {60, 441}, {65, 372}, {70, 313}, {75, 263},
            {80, 222}, {85, 187}, {90, 158}, {95, 134}, {100, 113},
            {105, 97}, {110, 83}, {115, 71}, {0, 0}, {0, 0},
            {0, 0}, {0, 0},
        },
    },

    {
        /*ϵͳ����*/
        HKADC_TEMP_SYSTEMH,
        /*����ͨ��*/
        HKADC_CHAN_11,
        1,
        /*ͨ������*/
        {0x0102, 2, 0, {0, 0}},
        /*��ѹ�¶�ת������*/
        28,
        /*��ѹ�¶�ת����*/
        {
            {-300, 2388}, {-250, 2348}, {-200, 2297}, {-150, 2233}, {-100, 2154}, {-50, 2060}, {0, 1951},
            {50, 1827}, {100, 1691}, {150, 1547}, {200, 1398}, {250, 1250},
            {300, 1105}, {350, 967}, {400, 840}, {450, 725}, {500, 622},
            {550, 532}, {600, 454}, {650, 387}, {700, 329}, {750, 280},
            {800, 239}, {850, 204}, {900, 174}, {950, 149}, {1000, 128},
            {1050, 110}, {1100, 95}, {1150, 82}, {0, 0}, {0, 0},
        },
    },

    {
        /*ϵͳ����*/
        HKADC_TEMP_SYSTEML,
        HKADC_CHAN_12, 
        1,
        /*ͨ������*/
        {0x0102, 2, 0, {0, 0}},
        /*��ѹ�¶�ת������*/
        28,
        /*��ѹ�¶�ת����*/
        {
            {-300, 2388}, {-250, 2348}, {-200, 2297}, {-150, 2233}, {-100, 2154}, {-50, 2060}, {0, 1951},
            {50, 1827}, {100, 1691}, {150, 1547}, {200, 1398}, {250, 1250},
            {300, 1105}, {350, 967}, {400, 840}, {450, 725}, {500, 622},
            {550, 532}, {600, 454}, {650, 387}, {700, 329}, {750, 280},
            {800, 239}, {850, 204}, {900, 174}, {950, 149}, {1000, 128},
            {1050, 110}, {1100, 95}, {1150, 82}, {0, 0}, {0, 0},
        },
    } ,
    {
        HKADC_TEMP_PA0,
        HKADC_CHAN_5,
        0,
        {0x0102, 2, 0, {0, 0}},
        28,
        {
            {-30, 2388}, {-25, 2348}, {-20, 2297}, {-15, 2233}, {-10, 2154}, {-5, 2060}, {0, 1951},
            {5, 1827}, {10, 1691}, {15, 1547}, {20, 1398}, {25, 1250},
            {30, 1105}, {35, 967}, {40, 840}, {45, 725}, {50, 622},
            {55, 532}, {60, 454}, {65, 387}, {70, 329}, {75, 280},
            {80, 239}, {85, 204}, {90, 174}, {95, 149}, {100, 128},
            {105, 110}, {110, 95}, {115, 82}, {0, 0}, {0, 0},
        },
    },
    {
        HKADC_TEMP_PA1,
        HKADC_CHAN_6,
        0,
        {0x0102, 2, 0, {0, 0}},
        28,
        {
            {-30, 2388}, {-25, 2348}, {-20, 2297}, {-15, 2233}, {-10, 2154}, {-5, 2060}, {0, 1951},
            {5, 1827}, {10, 1691}, {15, 1547}, {20, 1398}, {25, 1250},
            {30, 1105}, {35, 967}, {40, 840}, {45, 725}, {50, 622},
            {55, 532}, {60, 454}, {65, 387}, {70, 329}, {75, 280},
            {80, 239}, {85, 204}, {90, 174}, {95, 149}, {100, 128},
            {105, 110}, {110, 95}, {115, 82}, {0, 0}, {0, 0},
        },
    }
};

/*v9r1 ��طŵ��¶ȱ���*/
struct sys_temp_cfg default_bat_tempproc_conf =
{
    /*enable*/      0x0000,
    /*hkadc_id*/    2,
    /*high_thres*/  55,
    /*high_count*/  10,
    /*low_thres*/   -5,
    /*low_count*/   10,
    /*reserved[2]*/{ 0, 0}
};

/*****************************************************************************
�� �� ��  : hkadc_tempvolt_trans
��������  :
�������  :
                      ucChan       :ͨ��ֵ
                      tempvalue    :�¶�ֵ
                      hkAdcTable   :��ѹ���������ݵĻ����
                      tempTableLen :hkadc��ѹ�¶�ת����ĳ���
�������  :
           N/A
�� �� ֵ  :
           �����������
���ú���  :
��������  :

�޸���ʷ      :
 1.��    ��   : 2013��5��6��
   ��    ��   : 
   �޸�����   : �����ɺ���

*****************************************************************************/

unsigned short hkadc_tempvolt_trans(unsigned char ucChan,
                short tempvalue,
                unsigned short *hkAdcTable,
                unsigned short tempTableLen)
{
    unsigned short usTemNum = 0x00;          /*��¼�¶ȸ���*/
    unsigned short RetVal = 0x00;

    struct hkadc_tem_value *pstTemVoltTAble = (struct hkadc_tem_value *)hkAdcTable;
    struct hkadc_tem_value *pstTemp = (struct hkadc_tem_value *)hkAdcTable;

    /*need convert*/
    if(!hkAdcTable)
    {
        printk(KERN_ERR "hkadc_tempvolt_trans failed ucChan %d\n", ucChan);
        /*ֱ�ӷ��ص�ѹ*/
        return HKADC_VALUE_INVALID;
    }

    for(usTemNum=0; usTemNum< tempTableLen;usTemNum++)
    {
        if((0x00 == pstTemp->volt) && (0x00 == pstTemp->temp))
        {
            break;
        }
        pstTemp++;
    }

    /*NV����Ч��������Чֵ*/
    if((0x00 == pstTemVoltTAble->temp) && (0x00 == pstTemVoltTAble->volt) )
    {
        printk(KERN_ERR "hkadc_tempvolt_trans failed\n");

        return HKADC_VALUE_INVALID;
    }
    /*NV�����ݺϷ�*/

    /*�жϼ���ֵ*/
    if(tempvalue <= pstTemVoltTAble->temp)
    {
        RetVal =  pstTemVoltTAble->volt;
        return RetVal;
    }

    pstTemp =(struct hkadc_tem_value *)((pstTemVoltTAble + usTemNum) -1);
    if(tempvalue >= pstTemp->temp)
    {
        RetVal = pstTemp->volt;
        return RetVal;
    }

    /*�����¶�ֵ�����в�ֵ����*/
    while(tempvalue > pstTemVoltTAble->temp)
    {
        pstTemVoltTAble++;
    }

    pstTemp = (struct hkadc_tem_value *)(pstTemVoltTAble -1);
    /*ָ��ǰһ��Ԫ��*/

    RetVal = pstTemp->volt - (((tempvalue - pstTemp ->temp) * (pstTemp->volt - pstTemVoltTAble->volt)) \
                /(pstTemVoltTAble->temp - pstTemp->temp));

    printk(KERN_INFO "hkadc_tempvolt_trans %d\n", RetVal);

    return RetVal;

}

/*****************************************************************************
 �� �� ��  : bat_tempprotect_param_init
 ��������  : ��طŵ��±�������ʼ��
 �������  :
 �������  : ��
 �� �� ֵ  : int
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��13��
    ��    ��   :
    �޸�����   : �����ɺ���

*****************************************************************************/
int bat_tempprotect_param_init(void)
{
    int ret = -1;
    struct hkadc_chan_config*  chan_cfg = g_thermal_share_area->chan_cfg;
    int bat_hkadc_id = 0;
    unsigned short volt_high_thres;
    unsigned short volt_low_thres;

    printk(KERN_ALERT"enter, enable %d, bat_hkadc_id %d, \
        high_thres %d, high_count %d, low_thres %d, low_count %d\n",
        g_thermal_share_area->sys_temp_cfg.enable,
        g_thermal_share_area->sys_temp_cfg.hkadc_id,
        g_thermal_share_area->sys_temp_cfg.high_thres,
        g_thermal_share_area->sys_temp_cfg.high_count,
        g_thermal_share_area->sys_temp_cfg.low_thres,
        g_thermal_share_area->sys_temp_cfg.low_count);


    /*������±����¶ȷ�ֵ���ĳɵ�ѹ*/
    bat_hkadc_id = g_thermal_share_area->sys_temp_cfg.hkadc_id;
    if (bat_hkadc_id >= HKADC_CHAN_MAX)
        return ret;

    /*���·�ֵת�� */
    volt_high_thres = hkadc_tempvolt_trans(bat_hkadc_id, g_thermal_share_area->sys_temp_cfg.high_thres,
                (unsigned short *)chan_cfg[bat_hkadc_id].temp_table, chan_cfg[bat_hkadc_id].temp_data_len);

    if (volt_high_thres == HKADC_VALUE_INVALID)
    {
        printk(KERN_ALERT"hkadc_tempvolt_trans failed\n");
        g_thermal_share_area->sys_temp_cfg.high_thres = 0;
    }
    else
    {
        g_thermal_share_area->sys_temp_cfg.high_thres = volt_high_thres;
        ret = 0;
    }

    /*���·�ֵת�� */
    volt_low_thres = hkadc_tempvolt_trans(bat_hkadc_id, g_thermal_share_area->sys_temp_cfg.low_thres,
                (unsigned short *)chan_cfg[bat_hkadc_id].temp_table, chan_cfg[bat_hkadc_id].temp_data_len);

    if (volt_low_thres == HKADC_VALUE_INVALID)
    {
        printk(KERN_ALERT"hkadc_tempvolt_trans failed\n");
        ret = -1;
    }
    /*���·�ֵ���ת��ʧ�ܣ�ֱ��ʹ��HKADC_VALUE_INVALID*/
    g_thermal_share_area->sys_temp_cfg.low_thres = volt_low_thres;

    printk(KERN_ALERT"after, enable %d, bat_hkadc_id %d, \
        high_thres %d, high_count %d, low_thres %d, low_count %d\n",
        g_thermal_share_area->sys_temp_cfg.enable,
        g_thermal_share_area->sys_temp_cfg.hkadc_id,
        g_thermal_share_area->sys_temp_cfg.high_thres,
        g_thermal_share_area->sys_temp_cfg.high_count,
        g_thermal_share_area->sys_temp_cfg.low_thres,
        g_thermal_share_area->sys_temp_cfg.low_count);


    return ret;
}

void acpu_hkadc_fill_chan_map(void)
{
    int i = 0;
    int logic_id;
    int phy_id;
    struct drv_hkadc_map * chan_map;

    for (i = 0; i < g_acpu_hkadc_property.size; i++)
    {
        logic_id = g_acpu_hkadc_property.array[i].logic_chan;
        phy_id = g_acpu_hkadc_property.array[i].phy_chan;
        chan_map = bsp_hkadc_get_chan_map(logic_id);
        if (HKADC_NULL != chan_map)
        {
            printk(KERN_ALERT"acpu_hkadc_fill_chan_map logic_id->phyID: %d -> %d \n",
                            logic_id, phy_id);
            chan_map->phy_chan = phy_id;
        }
        else
        {
            printk(KERN_ALERT"acpu_hkadc_fill_chan_map get map ERROR: logic_id %d \n",
                            logic_id);
        }   
    }

}

int thermal_data_init(void)
{
    int i = 0;
    int phy_chan;
    struct hkadc_chan_config*  chan_cfg = g_thermal_share_area->chan_cfg;

    if(product_type("SOPHIA"))
    {
        g_acpu_hkadc_property.array = sophia_acpu_hkadc_property; 
        g_acpu_hkadc_property.size = sizeof(sophia_acpu_hkadc_property)/sizeof(struct hkadc_table_data);
    }else
    {
        g_acpu_hkadc_property.array = default_acpu_hkadc_property;
        g_acpu_hkadc_property.size = sizeof(default_acpu_hkadc_property)/sizeof(struct hkadc_table_data);
    }


    /*1.���ACPUÿ��ͨ������������*/
    for (i = 0; i < g_acpu_hkadc_property.size; i++)
    {
        if(g_acpu_hkadc_property.array[i].ctrlflag == 0)
            continue;
        phy_chan = g_acpu_hkadc_property.array[i].phy_chan;
        chan_cfg[phy_chan].outcfg.out_config = g_acpu_hkadc_property.array[i].hkadc_conf.outconfig;
        chan_cfg[phy_chan].out_peroid = g_acpu_hkadc_property.array[i].hkadc_conf.outperiod;
        chan_cfg[phy_chan].temp_data_len = g_acpu_hkadc_property.array[i].table_len;
        memcpy(chan_cfg[phy_chan].temp_table, g_acpu_hkadc_property.array[i].temp_table, 
            (sizeof(struct hkadc_tem_value) * g_acpu_hkadc_property.array[i].table_len));

        chan_cfg[phy_chan].have_config = 1;

        printk(KERN_ALERT"phy [%d], out_config %x, out_period %d, temp_data_len %d\n", \
                g_acpu_hkadc_property.array[i].phy_chan, chan_cfg[g_acpu_hkadc_property.array[i].phy_chan].outcfg.out_config,  \
                chan_cfg[g_acpu_hkadc_property.array[i].phy_chan].out_peroid, chan_cfg[g_acpu_hkadc_property.array[i].phy_chan].temp_data_len);            
   
    }

    /*2.���g_hkadc_chan_map�е�����ͨ����*/
    acpu_hkadc_fill_chan_map();

    /*3.��ȡ�����ϵͳ�ŵ��±�����*/
    memcpy((void *)(&g_thermal_share_area->sys_temp_cfg), (void *)&default_bat_tempproc_conf, \
            sizeof(struct sys_temp_cfg));


    /*4.������±����¶ȷ�ֵ���ĳɵ�ѹ*/
    if (bat_tempprotect_param_init() != 0)
    {
        printk(KERN_ALERT"bat_tempprotect_param_init failed\n");
        //return -1;
    }

    /*5.������ɱ��, ��MCUͬ������±���������*/
    g_thermal_data_area->magic_end = HKADC_INIT_MAGIC_CODE;

    return 0;
}

int hkadc_debug_init(int chanid)
{
	struct hkadc_chan_config*  chan_cfg = g_thermal_share_area->chan_cfg;

	out_config_save[chanid] = chan_cfg[chanid].outcfg.out_config ;
	out_peroid_save[chanid] = chan_cfg[chanid].out_peroid;
	have_config_save[chanid] = chan_cfg[chanid].have_config;
	chan_cfg[chanid].outcfg.out_config = chan_cfg[chanid].outcfg.out_config | 0x0102;
	chan_cfg[chanid].out_peroid = 1;	

	chan_cfg[chanid].have_config = 1;
	
	return 0;
}

int hkadc_debug_uninit(int chanid)
{
	struct hkadc_chan_config*  chan_cfg = g_thermal_share_area->chan_cfg;
	 
	chan_cfg[chanid].outcfg.out_config = out_config_save[chanid];
	chan_cfg[chanid].out_peroid = out_peroid_save[chanid];	

	chan_cfg[chanid].have_config = have_config_save[chanid];

	return 0;
}

int hkadc_get_shared_value(int chanid, unsigned short* volt)
{ 
    struct hkadc_chan_out * chan_out = g_thermal_data_area->chan_out;
    struct hkadc_chan_config *chan_cfg = g_thermal_data_area->chan_cfg;
    short  volt_l;
    short  volt_r;
    unsigned int count;
    int ret = 0; 
    unsigned short voltvalue;
    OUT_PROP_CONFIG outConfig;

    /*���ͨ��û�����ã�ֱ�ӷ���HKADC_ERROR*/
    if (chan_cfg[chanid].have_config == 0)
        return -1;
        
    outConfig = chan_cfg[chanid].outcfg.bit_out_config;
    
    /*�Ƚϵ�ѹ��ԭʼֵ��ȡ��ֵ�����������򷵻�ԭʼֵ*/
    count = 5;        
    do {   
        volt_l = (short)chan_out[chanid].volt_l;
        volt_r = (short)chan_out[chanid].volt_r;
    } while ((count--) && (~volt_l != volt_r));

    if (~volt_l == volt_r) {
        voltvalue = volt_l;
    } else {
        return -1;
    }
      
    if (volt) { 
        *volt = voltvalue;
    }

    return ret;
}


/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : tsensor_driver.h
  �� �� ��   : ����
  ��    ��   : ����l00217270
  ��������   : 2013��5��14��
  ����޸�   :
  ��������   : tsensor_driver.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��5��14��
    ��    ��   : ����l00217270
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/


#ifndef __TSENSOR_DRVIER_H__
#define __TSENSOR_DRVIER_H__

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




/*****************************************************************************
  2 �궨��
*****************************************************************************/
//#define TSENSOR_DEBUG


#define TSENSOR_NAME_LEN                 (50)
#define TSENSOR_WQ_NAME_LEN         (50)
#define TSENSOR_IRQ_NAME_LEN         (50)

#define TSENSOR_NORMAL_MONITORING_RATE  (1000)

#define ACPU_TSENSOR_ENABLE             0x01
#define GPU_TSENSOR_ENABLE             0x02

/*tsensor����ֵƫ���볤��*/
#define EFUSE_TSENSOR_TRIM_OFFSET           1408 /*1280*/
#define EFUSE_TSENSOR_TRIM_LEN              32

/*tsensor����ֵת����*/
#define GPUR_TRIM_DATA_MASK                0x7F000000
#define GPUR_TRIM_DATA_PLUSMINUS           0x80000000

#define GPUL_TRIM_DATA_MASK                0x007F0000
#define GPUL_TRIM_DATA_PLUSMINUS           0x00800000

#define ACPUR_TRIM_DATA_MASK                0x00007F00
#define ACPUR_TRIM_DATA_PLUSMINUS           0x00008000

#define ACPUL_TRIM_DATA_MASK                0x0000007F
#define ACPUL_TRIM_DATA_PLUSMINUS           0x00000080


/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
typedef enum {
    TSENSOR_ALARAM_ON = 0,
    TSENSOR_ALARAM_OFF,
    TSENSOR_ALARAM_BUFF,
} enum_tsensor_alarm;


typedef enum {
    TSENSOR_LOC_LOCAL = 0,
    TSENSOR_LOC_REMOTE,
    TSENSOR_LOC_BUFF,
} enum_tsensor_location;

typedef enum {
    TSENSOR_TYPE_0 = 0,
    TSENSOR_TYPE_1,
    TSENSOR_TYPE_BUFF,
} enum_tsensor_type;

/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/
struct tsensor_config {
    unsigned int enable;               /* bit0:ACPU �±�ʹ�� bit1:GPU�±�ʹ�� 1  ʹ�� 0 �ر�*/
    unsigned int lag_value0;             /* tsensor0�ͻ�����ֵ */
    unsigned int lag_value1;             /* tsensor1�ͻ�����ֵ */
    unsigned int thres_value0;          /* tsensor0�澯����ֵ*/
    unsigned int thres_value1;          /* tsensor1�澯����ֵ*/
    unsigned int reset_thres_value0;      /* tsensor0��λ����ֵ*/   
    unsigned int reset_thres_value1;      /* tsensor1��λ����ֵ*/ 
    unsigned int alarm_count1;      /* ���ж��ϱ����ۼƼ�鳬�����޵�
                                                           �����ﵽ�����޺�����profileΪacpumaxfreq/gpumaxfreq, ddrmaxfreq 
                                                          */
    unsigned int alarm_count2;      /* �Ѿ�����profile���ۼƼ�鳬�����޵�
                                                           �������𼶽���profile 
                                                          */                                                          
    unsigned int resume_count;     /*����¶��ۼƵ����ͻ����޵Ĵ����ﵽ�����ޣ�ȡ��max���� 
                                                      */

    unsigned int acpu_maxfreq;         /* �������޺�����acpu�����Ƶ��*/
    unsigned int gpu_maxfreq;           /* �������޺�����gpu�����Ƶ��*/
    unsigned int ddr_maxfreq;            /* �������޺�����ddr�����Ƶ��*/
    unsigned int reserved[4];            /*����*/
};


struct tsensor_info {
    char name[TSENSOR_NAME_LEN];
    char wqname[TSENSOR_WQ_NAME_LEN];
    char irqname[TSENSOR_IRQ_NAME_LEN];
    struct mutex lock;
    unsigned int irqnum;
    enum_tsensor_alarm tsensor_alarm_flag;
    unsigned int tsensor_alarm_count;
    unsigned int tsensor_resume_count;
    unsigned int hot_temp_num;    

    enum_tsensor_location last_access;  /*��¼�ϴη��ʵ�sensorĿ�꣬�����ظ�����*/
    
    bool maxfreq_lock;     /*�Ƿ��Ѿ���Ƶ*/
    int exceed_temperature_time;

    /*��¼��profile*/
    int cpu_profile_index;
    int gpu_profile_index;
    int ddr_profile_index; 

    /*��¼NV����������profile*/
    int cpu_max_profile;
    int gpu_max_profile;
    int ddr_max_profile;     

    int thres_value;
    int lag_value;
    int reset_thres_value;
    
    struct work_struct tsensor_alarm_wk;
    struct workqueue_struct *tsensor_alaram_wq;
    
    void (*tsensor_init) (void);
    void (*tsensor_suspend)(void);
    irqreturn_t (*tsensor_irq)(int irq, void *data);
    void (*tsensor_irq_delaywork)(struct work_struct *work);
    void (*tsensor_temp_protect)(void);
    
};

struct tsensor_devinfo {
    struct platform_device *pdev;
    struct device *dev;
};

struct tsensor_governor { 
    struct mutex lock;
    struct delayed_work tsensor_monitor_work;
    int average_period; 
};

struct efuse_trim
{
   int gpu_remote;
   int gpu_local;
   int acpu_remote;
   int acpu_local;
};

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
    
int tsensor0_localremote_temp_get(enum_tsensor_location remotelocal, int count);
int tsensor0_temp_get(void);
void tsensor0_alarmstate_set(enum_tsensor_alarm onoff);
enum_tsensor_alarm tsensor0_alarmstate_get(void);
irqreturn_t tsensor0_irq(int irq, void *data);
void tsensor0_irq_delaywork(struct work_struct *work);
void tsensor0_suspend(void);
void tsensor0_init(void);
void tsensor0_temp_protect(void);

int tsensor1_localremote_temp_get(enum_tsensor_location remotelocal, int count);
int tsensor1_temp_get(void);
void tsensor1_alarmstate_set(enum_tsensor_alarm onoff);
enum_tsensor_alarm tsensor1_alarmstate_get(void);
irqreturn_t tsensor1_irq(int irq, void *data);
void tsensor1_irq_delaywork(struct work_struct *work);
void tsensor1_suspend(void);
void tsensor1_init(void);
void tsensor1_temp_protect(void);


int tsensor_config_init(void);


int tsensor_init(void);
int tsensor_suspend(void);
int tsensor_resume(void);
void tsensor_temp_protect(void);





#endif /* end of tsensor_driver.h */



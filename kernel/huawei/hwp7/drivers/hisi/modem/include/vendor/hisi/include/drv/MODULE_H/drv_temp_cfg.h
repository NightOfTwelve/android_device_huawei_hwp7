/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_temp_cfg.h
  �� �� ��   : ����
  ��������   : 2013��5��6��
  ����޸�   :
  ��������   : drv_temp_cfg.h ��ͷ�ļ�,����DDR�����ڴ����ݽṹ
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��5��6��
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "global_ddr_map.h"
#include "drv_hkadc.h"


#ifndef __DRV_TEMP_CFG_H__
#define __DRV_TEMP_CFG_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  2 �궨��
*****************************************************************************/
#ifndef _DRV_LLT_
/*#define THERMAL_LOG_DEBUG*/
#endif

/*��׮��*/
//#define THERMAL_STUB_ON

#define INVALID_HKADC_LOGVALUE                0xff
#define INVALID_HKADC_PHYVALUE                0xFF

#define HKADC_CONFIG_NOOUTPUT                 0x00
#define HKADC_CONFIG_SINGLEOUTPUT             0x01
#define HKADC_CONFIG_LOOPOUTPUT               0x02
#define HKADC_VALUE_INVALID                   0xffff

/*��Ч�¶�ֵ*/
#define TEMP_VALUE_INVALID                    (-255)

#define HKADC_INIT_MAGIC_CODE                 0x5A5A5A5A

#define M3_HKADC_DDR_ADDR                   MCU_HKADC_DDR_ADDR
#define M3_HKADC_DDR_SIZE                   MCU_HKADC_DDR_SIZE

#ifndef HKADC_OK
#define HKADC_OK               (0)
#endif

#ifndef HKADC_ERROR
#define HKADC_ERROR (-1)
#endif

#define HKADC_NULL      (void*)(0)
#define HKADC_FALSE            (0)
#define HKADC_TRUE             (1)

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
/*����hkadc����ͨ����*/
typedef enum hkadcChan
{
    HKADC_CHAN_INVALID = -1,
    HKADC_CHAN_0 = 0x00,    
    HKADC_CHAN_1,                    
    HKADC_CHAN_2,     
    HKADC_CHAN_3,
    HKADC_CHAN_4,
    HKADC_CHAN_5, 
    HKADC_CHAN_6,  
    HKADC_CHAN_7, 
    HKADC_CHAN_8, 
	HKADC_CHAN_9,
	HKADC_CHAN_10,
    HKADC_CHAN_11,
	HKADC_CHAN_12,
	HKADC_CHAN_13,
	
    HKADC_CHAN_MAX
}HKADCCHANNUM;


/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/
struct drv_hkadc_map
{
    unsigned int logic_chan;
    unsigned int phy_chan;
    unsigned int nv_id;
    unsigned int nv_len;
    unsigned int modem_id;
    char * name;
};

typedef struct hkadc_chan_out
{
    unsigned short volt_l;    /* ���ص�HKADC��ѹֵ����λ:���� */
    unsigned short volt_r;   /* ���ص�HKADC��ѹֵȡ�� */
    short          temp_l;     /* ���ص��¶�ֵ����λ:���϶� */
    short          temp_r;    /* ���ص��¶�ֵȡ�� */
}DRV_CHANNEL_OUTPUT;

typedef struct hkadc_tem_value
{
    signed short   temp;          /* �¶ȵ㣬��λ:���϶� */
    unsigned short volt;      /* ��ѹ�㣬��λ:���� */
}TEM_VOLT_TABLE;

typedef struct hkadc_out_config
{
   unsigned int out_prop:2; 
   unsigned int out_wake:1;
   unsigned int out_vol_temp:1;
   unsigned int out_reserv1:4;
   unsigned int out_acore:1;
   unsigned int out_ccore:1;
   unsigned int out_reserv2:22;
} OUT_PROP_CONFIG;

struct hkadc_nv_config
{
    unsigned int outconfig;         /* bit0-bit1 0:����� 1:������� 2:ѭ����� */
                                       /* bit2 1:������� 0:�ǻ������ */
                                       /* bit3: 0:ֻ�����ѹ 1:��ѹ�¶Ⱦ���� */
                                       /* bit8: A����� */
                                       /* bit9: C����� */
    unsigned short outperiod;          /* ѭ�����ʱ��ѭ�����ڣ���λ:�� */
    unsigned short convertlistlen;     /* �¶�ת������ */
    unsigned int   reserved[2];        /* �¶�ת����ʵ�ʳ��Ȳμ�usTempDataLen */
};

typedef struct hkadc_chan_config
{
    union 
    {
        unsigned int out_config;    /* bit0-bit1 0:����� 1:������� 2:ѭ����� */
                                       /* bit2 1:������� 0:�ǻ������ */
                                       /* bit3: 0:ֻ�����ѹ 1:��ѹ�¶Ⱦ���� */
                                       /* bit8: A����� */
                                       /* bit9: C����� */
        struct hkadc_out_config bit_out_config;
    } outcfg;

    unsigned int have_config;
    unsigned short out_peroid;    /* ѭ�����ʱ��ѭ�����ڣ���λ:�� */
    unsigned short temp_data_len;  /* �¶�ת������ */
    struct hkadc_tem_value temp_table[32]; /* �¶�ת����ʵ�ʳ��Ȳμ�usTempDataLen */
} DRV_CHANNEL_CONFIG;


typedef struct sys_temp_cfg
{
    unsigned short enable;       /* bit0:���±���ʹ�� bit1:���±���ʹ�� 1 ʹ�� 0 �ر�*/
    unsigned short hkadc_id;      /* ϵͳ�ŵ��¶ȱ�����hkadcͨ��ID */
    unsigned short high_thres;     /* ϵͳ�ŵ���±����ĵ�ѹ��ֵ ���¶�Խ�ߣ���ѹԽ��*/
    unsigned short high_count;    /* ϵͳ�ŵ���±����������ޣ����˺�ϵͳ�ػ� */
    unsigned short low_thres;      /* ϵͳ�ŵ���±����ĵ�ѹ��ֵ ���¶�Խ�ͣ���ѹԽ��*/
    unsigned short low_count;     /* ϵͳ�ŵ���±����������ޣ����˺�ϵͳ�ػ� */
    unsigned int   reserved[2];   /*����*/
} DRV_SYS_TEMP_PRO;

/*debug_flag*/
#define THERMAL_DEBUG_TEMP          0x01
#define THERMAL_DEBUG_TEMP_MORE     0x02
#define THERMAL_DEBUG_BAT           0x04
#define THERMAL_DEBUG_HKADC_STUB    0x08


typedef struct thermal_data_area
{
    unsigned long      magic_start;    /* ������ʼMagic Code 0x5A5A5A5A*/
    struct hkadc_chan_out chan_out[HKADC_CHAN_MAX];  /* ��ͨ���������,0x4��ʼ*/ /*monan:16 �Ƿ��ΪHKADC_CHAN_MAX ?*/
    struct hkadc_chan_config chan_cfg[HKADC_CHAN_MAX];  /* ��ͨ���������� */
    struct sys_temp_cfg   sys_temp_cfg;    /* ϵͳ�ŵ��±����� */
    unsigned long      hw_ver_id;       /* �汾ID */
    unsigned short     phy_tbl[HKADC_CHAN_MAX];
    unsigned long      debug_flag;     /* ���⿪�� */
    unsigned short     adc_value[HKADC_CHAN_MAX];
    unsigned short     acpu_temp;
    unsigned short     gpu_temp;
    unsigned long      magic_end;      /* ��������Magic Code 0x5A5A5A5A*/
} DRV_HKADC_DATA_AREA;

/* sizeof(struct thermal_data_area) = 2328 */


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

#endif /* end of drv_temp_cfg.h */


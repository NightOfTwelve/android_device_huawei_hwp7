#ifndef _HI_GLOBAL_MEM_MAP_INCLUDE_H_
#define _HI_GLOBAL_MEM_MAP_INCLUDE_H_


#include "soc_baseaddr_interface.h"

/*******************************************
��˹���ͷ�ļ�
�������������ϵͳ���ͷ�ļ�
*******************************************/

/******************************************
V900R001 Dual-Channel DDR Details:
*******************************************/

#define DDR_BASE_ADDR					(SOC_DRAM_BASE_ADDR)
#define DDR_LAST_ADDR					(0x7FFFFFFF)

/******************************************************
         �����ڴ���DDR�ǽ�֯��
********************************************************/
/******************************************************
         DDR�ǽ�֯�� �ϰ��� 128M
********************************************************/
/*����������µ��ڴ��������Ҫ�� ..\kernel\arch\arm\mach-hi6620\init_param.c
�ļ�������Ӷ�Ӧ�ļ����Ŀ-start*/
/*Modemϵͳ�ڴ� 89M*/
#define MODEM_SYS_MEM_ADDR				(DDR_BASE_ADDR)                                 /*0x00000000*/
#define MODEM_SYS_MEM_SIZE				(0x05700000)


/* when ADDR changed,please check the vendor\hisi\build\scripts\gen_img_vrl_tbl.py */

/*20130702,l00212112
��ע��:
����ڴ��Ǵ�MODEM_SYS_MEM_SIZE �����39M����������v9��c�˵�����λ�����б����ڼ���
modem bin�ļ�����ʱ�洢������������޸���οռ��С��
�����޸ĳ�С��modem bin��size��ֵ��������οռ����µ���;����ͬ���޸�c�˵�����λ!!!!*/
#define MODEM_SYS_MEM_RUN_AREA_SIZE				(0x03200000)    /*�������С�Ŀռ�֮�����C����*/
#define MODEM_RESET_LOAD_CBIN_ADDR				(MODEM_SYS_MEM_ADDR + MODEM_SYS_MEM_RUN_AREA_SIZE)                /*0x03200000*/
#define MODEM_RESET_LOAD_CBIN_SIZE				(0x02500000)    /*�ݴ�C����ռ����ֵ��37M*/
#define MODEM_RESET_LOAD_CBIN_BOUNDARY          (MODEM_RESET_LOAD_CBIN_ADDR)/*���ռ��ؾ���ռ䲻�ܳ�����ֵ*/
/*��ע��:
1.��οռ��Ǹ�modemѹ����������õģ���ѹ������İ��˵�ַ��
2.20140320������Ч���ռ䷶Χ0x4600000-0x5700000
3.�ڿ�����C�˵�����λ�������õ�.
4.��ֵ��gen_img_vrl_tbl.py��modem�ļ��ص�ַ��Ӧ������޸ģ�Ҫͬʱ�޸�*/
#define MODEM_ZIP_LOAD_ADDR                     0x45FFFF0
/*�õ�ַ�ǽ�ѹ��������ڵ�ַ�����ѹ��������ͷ����Ӧ�ø�Ϊ(MODEM_ZIP_LOAD_ADDR + ����ͷ��С)*/
#define MODEM_UNZIP_RUN_ADDR                     0x4600000

#define MODEM_ZIP_SIZE      ((MODEM_SYS_MEM_ADDR + MODEM_SYS_MEM_SIZE) - MODEM_ZIP_LOAD_ADDR)
/*��ע��:
�����ַ��modem����ļ��ص�ַ��
*/
#define MODEM_LOAD_DEST_ADDR              0x3E00

/*HIFIϵͳ�ڴ� 8M*/
#define HIFI_SYS_MEM_ADDR				(MODEM_SYS_MEM_ADDR + MODEM_SYS_MEM_SIZE)       /*0x05700000*/
#define HIFI_SYS_MEM_SIZE				(0x00800000)

/*MCUϵͳ�ڴ� 1M*/
#define MCU_SYS_MEM_ADDR			    (HIFI_SYS_MEM_ADDR + HIFI_SYS_MEM_SIZE)         /*0x05F00000*/
#define MCU_SYS_MEM_SIZE				(0x00100000)

/*C�˸�GUPSԤ�����ڴ� 19M*/
#define GU_RESERVED_MEM_ADDR		    (MCU_SYS_MEM_ADDR + MCU_SYS_MEM_SIZE)           /*0x06000000*/
#define GU_RESERVED_MEM_SIZE		    (0x01100000)

/*C�˸�LPSԤ�����ڴ�8M,����ַ�����ı�ƫ�ƣ���Ҫͬ���޸�BBE16����*/
#define LPS_RESERVED_MEM_ADDR		    (GU_RESERVED_MEM_ADDR + GU_RESERVED_MEM_SIZE)   /*0x07100000*/
#define LPS_RESERVED_MEM_SIZE		    (0x00800000)

/*�˼乲���ڴ�4M*/
#define IPC_SHARE_MEM_ADDR				(LPS_RESERVED_MEM_ADDR + LPS_RESERVED_MEM_SIZE) /*0x07900000*/
#define IPC_SHARE_MEM_SIZE				(0x00400000)

/*Modem�쳣��Ϣ��¼2M */
#define MODEM_DUMP_LOG_ADDR				(IPC_SHARE_MEM_ADDR + IPC_SHARE_MEM_SIZE)       /*0x07D00000*/
#define MODEM_DUMP_LOG_SIZE				(0x00200000)

/*MCUϵͳ�ڴ� 1M */
#define MCU_SHARE_MEM_ADDR				(MODEM_DUMP_LOG_ADDR + MODEM_DUMP_LOG_SIZE)     /*0x07F00000*/
#define MCU_SHARE_MEM_SIZE				(0x00100000)
/******************************************************
         DDR�ǽ�֯�� �°���128M
********************************************************/

/*AP�쳣��Ϣ��¼2M */
#define ANDROID_DUMP_LOG_ADDR			(MCU_SHARE_MEM_ADDR + MCU_SHARE_MEM_SIZE)       /*0x08000000*/
#define ANDROID_DUMP_LOG_SIZE			(0x00200000)

/*FASTBOOT ϵͳ�ڴ�4M*/
#define FASTBOOT_SYS_MEM_ADDR			(ANDROID_DUMP_LOG_ADDR + ANDROID_DUMP_LOG_SIZE) /*0x08200000*/
#define FASTBOOT_SYS_MEM_SIZE			(0x00400000)

/*Android ϵͳ�ڴ������ֹ�886M Ϊ��A��Kernelʹ�÷��� */
#define ANDROID_SYS_MEM_ADDR			(FASTBOOT_SYS_MEM_ADDR + FASTBOOT_SYS_MEM_SIZE) /*0x08600000*/
#define ANDROID_SYS_MEM_SIZE			(DDR_LAST_ADDR - ANDROID_SYS_MEM_ADDR + 1)

#if 0
/*FASTBOOT ϵͳ�ڴ�32M ʹ��1G/2G���32M�ռ� */
#define FASTBOOT_SYS_MEM_ADDR			(ANDROID_SYS_MEM_ADDR + ANDROID_SYS_MEM_SIZE - 0x02000000)
#define FASTBOOT_SYS_MEM_SIZE			(0x02000000)
#endif

#define SYS_DUMP_MEM_ADDR                (0x40000000)
#define SYS_DUMP_MEM_SIZE                (0x20000000)

/*����������µ��ڴ��������Ҫ�� ..\kernel\arch\arm\mach-hi6620\init_param.c
�ļ�������Ӷ�Ӧ�ļ����Ŀ end*/

/******************************************************
  ��������ҪԤ�������ַ��64MB(0xF0000000~0xF4000000)
********************************************************/
#define GLOBAL_VIRT_ADDR                (0xF0000000)
#define GLOBAL_VIRT_SIZE                (0x04000000)

/*C�˸�GUPSԤ�����ڴ����ַ*/
#define GU_RESERVED_MEM_VIRT_ADDR       (GLOBAL_VIRT_ADDR)                                  /*0xF0000000*/

/*C�˸�LPSԤ�����ڴ����ַ*/
#define LPS_RESERVED_MEM_VIRT_ADDR      (GU_RESERVED_MEM_VIRT_ADDR + GU_RESERVED_MEM_SIZE)  /*0xF1100000*/

/*�˼乲���ڴ����ַ*/
#define IPC_SHARE_MEM_VIRT_ADDR         (LPS_RESERVED_MEM_VIRT_ADDR + LPS_RESERVED_MEM_SIZE)/*0xF1700000*/

/*Modem�쳣��Ϣ��¼���ַ */
#define MODEM_DUMP_LOG_VIRT_ADDR        (IPC_SHARE_MEM_VIRT_ADDR + IPC_SHARE_MEM_SIZE)      /*0xF1B00000*/

/*�±��˼乲���ڴ����ַ*/
#define MCU_SHARE_MEM_VIRT_ADDR         (MODEM_DUMP_LOG_VIRT_ADDR + MODEM_DUMP_LOG_SIZE)    /*0xF1D00000*/





/*MCU �쳣����dump��*/
#define MCU_DUMP_LOG_ADDR				(MCU_SHARE_MEM_ADDR)
#define MCU_DUMP_LOG_SIZE               (0x000E0000)

/*mcu mntnʹ�ù�����*/
#define MCU_MNTN_DDR_ADDR               (MCU_DUMP_LOG_ADDR + MCU_DUMP_LOG_SIZE)
#define MCU_MNTN_DDR_SIZE               (0x00008000)

/*MCU ϵͳ�±������ڴ��� MCU_HKADC_DDR_ADDR*/
#define MCU_HKADC_DDR_ADDR              (MCU_MNTN_DDR_ADDR + MCU_MNTN_DDR_SIZE)
#define MCU_HKADC_DDR_SIZE              (0x0004000)

#define MCU_DDR_LOAD_DDR_ADDR              (MCU_HKADC_DDR_ADDR + MCU_HKADC_DDR_SIZE)
#define MCU_DDR_LOAD_DDR_SIZE              (0x00010000)

#define MCU_SHARE_MEM_USED_SIZE         (MCU_DUMP_LOG_SIZE+MCU_MNTN_DDR_SIZE+MCU_HKADC_DDR_SIZE+MCU_DDR_LOAD_DDR_SIZE)


#if (MCU_SHARE_MEM_USED_SIZE > MCU_SHARE_MEM_SIZE)
#error mcu_share_memory(MCU_SHARE_MEM_USED_SIZE) used beyond (MCU_SHARE_MEM_SIZE)
#endif
#endif /*_HI_GLOBAL_MEM_MAP_INCLUDE_H_*/

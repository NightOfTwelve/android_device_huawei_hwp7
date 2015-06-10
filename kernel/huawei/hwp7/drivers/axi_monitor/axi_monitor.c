/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : axi_monitor.c
  �� �� ��   : ����
  ��    ��   : �ż� 00179562
  ��������   : 2013��5��26��
  ����޸�   :
  ��������   : AXI MONITOR�ڴ���
  �����б�   :

  �޸���ʷ   :
  1.��    ��   : 2013��5��26��
    ��    ��   : �ż� 00179562
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/scatterlist.h>
#include <linux/random.h>
#include <linux/mmc/host.h>
#include <linux/mmc/mmc.h>
#include <linux/mmc/card.h>
#include <linux/ioport.h>
#include <linux/device.h>
#include <linux/switch.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/console.h>

#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/kthread.h>
#include <linux/workqueue.h>
#include <linux/freezer.h>
#include <linux/irq.h>
#include <asm/dma.h>
#include <asm/io.h>
#include <mach/irqs.h>
#include <asm/sizes.h>
#include <asm/uaccess.h>
#include <mach/hardware.h>
#include <linux/gpio.h>
#include <linux/io.h>
#include <linux/timer.h>
#include <linux/miscdevice.h>

#include <linux/semaphore.h>
#include <linux/fs.h>
#include <linux/syscalls.h>
#include <linux/vmalloc.h>


#include "drv_timer.h"
#include "axi_monitor.h"
//#include "../rfile/rfilesystem_interface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
/*�����ַ����*/
void * map_address_mon1 = NULL;
void * map_address_mon0 = NULL;
void * map_address_sc   = NULL;

/*���ݴ�ӡ�߳�ʹ��*/
struct semaphore     sem_print_axi;
struct task_struct   *g_printMonitorThread = NULL;

/*�����������*/
monitor_param mon_param;

/*����ͳ��*/
static unsigned int monitorTimes = 0;

/*������ݱ���BUFFER*/
static monitor_output_data* axi_monitor_output = NULL;
static monitor_output_data* index_axi_monitor_output = NULL;


/*timerģ����Դ����*/
extern int DRV_TIMER_STOP(unsigned int usrClkId);
extern int DRV_TIMER_START
(
    unsigned int     usrClkId,
    FUNCPTR_1        routine,
    int              arg,
    unsigned int     timerValue,
    unsigned int     mode,
    unsigned int     unitType
);
#if 0
extern FILE *BSP_fopen(const char *path, const char *mode);
extern int BSP_fseek(FILE *stream, long offset, int whence);
extern long BSP_ftell(FILE *stream);
extern int BSP_fread(void*buf,unsigned int size,unsigned int count, FILE *stream);
extern int BSP_fwrite(const void* buf,unsigned int size,unsigned int count,FILE *stream);
extern int BSP_fclose(FILE *fp);
#endif


/*monitor port,id set,enable id,monitor all addr*/
static unsigned int SOC_SELECT[SOC_SEL_MAX][AXI_MON_IDS_NUM+1] =
{
    /*id1-id8, port sel, total 9items*/
    /*id1       , id2       ,id3,       id4         id5         id6         id7       id8      prot sel*/
    /*
    amonitor 0 set description
    ID1:AMON :G3D,PORT_SEL:PORT0,enable id,monitor all addr
    ID2:AMON :G2D,PORT_SEL:PORT0,enable  id,monitor all addr
    ID3:AMON :EDC0,PORT_SEL:PORT1,enable id,monitor all addr
    ID4:AMON :EDC1,PORT_SEL:PORT1,enable id,monitor all addr
    ID5:AMON :ISP,PORT_SEL:PORT1,enable id,monitor all addr
    ID6:AMON :null  ,PORT_SEL:null
    ID7:AMON :null   ,PORT_SEL:null
    ID8:AMON :null,PORT_SEL:null
    */
    /*amonitor 0 set*/
    /*G3D           G2D         edc0        edc1        isp     no          no          no      port*/
    {0xb7ff8000, 0xB7FF9000, 0xBFCF8000,0xBFCF8040,0xBFCF8020,0xBFCF8020,0xBFCF8020,0xBFCF8020,0x1240},

    /*amonitor 1 set description
    ID1:AMON :VDEC,PORT_SEL:PORT0,enable id,monitor all addr
    ID2:AMON :VENC,PORT_SEL:PORT0,enable id,monitor all addr
    ID3:AMON :VPP,PORT_SEL:PORT0,enable id,monitor all addr
    ID4:AMON :CCPU,PORT_SEL:PORT1,enable id,monitor all addr
    ID5:AMON :MDMNOC ,PORT_SEL:PORT1,enable id,monitor all addr
    ID6:AMON :SYSNOC ,PORT_SEL:PORT1,enable id,monitor all addr
    ID7:AMON :null  ,PORT_SEL:null
    ID8:AMON :null,PORT_SEL:null
    */
    /*amonitor 1 set*/
    /*VDEC          VENC        VPP         CCPU        MDMNOC  SYSNOC      no      no          port*/
    {0xBE7F8000, 0xBE7F8100, 0xBE7F8200,0xA7FF8000,0xA7FF8100,0xA7FF8200,0xA7FF8200,0xA7FF8200,0x9200},
};

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

/*for debug,read/write register*/
void DWB_axi(void) /* drain write buffer */
{
    asm volatile (
        "mcr p15, 0, %0, c7, c10, 4\n" : : "r" (0)
        );
}
void writel_axi(unsigned val, unsigned addr)
{
    DWB_axi();
    (*(volatile unsigned *) (addr)) = (val);
    DWB_axi();
}
unsigned readl_axi(unsigned addr)
{
    return (*(volatile unsigned *) (addr));
}

/*�ݲ���Ҫ���ü�ص�ַ������id����ȫ��ַ���*/
#if 0
BSP_VOID BSP_Axi_Mon_Low_Addr_Set(BSP_U32 base_addr, BSP_U32 low_addr)
{
    BSP_U32 ids = 0;
    for(ids = 0; ids < 8; ids++)
    {
        BSP_REG_WRITE_BASE(ID_ADDR_DES_AADR(base_addr, ids), low_addr);
    }
}

BSP_VOID BSP_Axi_Mon_High_Addr_Set(BSP_U32 base_addr, BSP_U32 high_addr)
{
    BSP_U32 ids = 0;
    for(ids = 0; ids < 8; ids++)
    {
        BSP_REG_WRITE_BASE(ID_ADDR_DES_M_AADR(base_addr, ids), high_addr);
    }
}
#endif

/*****************************************************************************
 �� �� ��  : BSP_ID_Port_Select
 ��������  : ID���˿�����
 �������  : base_addr:monitor��ַ
           : monId:monitor id(0/1)
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��25��
    ��    ��   : �ż� 00179562
    �޸�����   : �����ɺ���

*****************************************************************************/
void BSP_ID_Port_Select(unsigned int base_addr, unsigned int monId)
{
    int i = 0;
    for(i = 0;i < AXI_MON_IDS_NUM; i++)
    {
        BSP_REG_WRITE_BASE(CNT_ID_ADDR(base_addr, i), SOC_SELECT[monId][i]);
    }
    BSP_REG_WRITE_BASE(CNT_PORT_SEL_ADDR(base_addr), SOC_SELECT[monId][AXI_MON_IDS_NUM]);
}

/*****************************************************************************
 �� �� ��  : BSP_Axi_Mon_Config
 ��������  : �������
 �������  : monId:monitor id(0/1)
           : ct:�������(��/д)
           : timecycles:��ʱ���(0Ϊ������ѭ��)
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��25��
    ��    ��   : �ż� 00179562
    �޸�����   : �����ɺ���

*****************************************************************************/
int BSP_Axi_Mon_Config(unsigned int monitorId, SOC_CNT_TYPE ct)
{
    unsigned int monAddr = 0;
    monAddr = BSP_Axi_Get_Module_Addr(monitorId);
    /*�������󼤻�*/
    BSP_Axi_Mon_Active();

    /*reset*/
    BSP_Axi_Mon_Reset(monitorId);

    /*port sel*/
    BSP_ID_Port_Select(monAddr,monitorId);

    /*BSP_Axi_Mon_Low_Addr_Set(monAddr, u32start);
    BSP_Axi_Mon_High_Addr_Set(monAddr, u32end);*/

    /*monitor type :read/write*/
  	axi_monitor_cnt_type_set(monAddr, ct);
    /*win counter enable*/
    BSP_REG_WRITE_BASE(CNT_CTRL_ADDR(monAddr), 0x0002);

    /*soft reset*/
    BSP_REG_WRITE_BASE(CNT_RESET_ADDR(monAddr), 0x0002);

    if(AXI_Wait_Monitor_Ready(monAddr) )
    {
        return -1;
    }

    return 0;
}

/*****************************************************************************
 �� �� ��  : BSP_Axi_Mon_Start
 ��������  : ��ؿ���
 �������  : monId:monitor id(0/1)
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��25��
    ��    ��   : �ż� 00179562
    �޸�����   : �����ɺ���

*****************************************************************************/
void BSP_Axi_Mon_Start(unsigned int id)
{
    unsigned int u32Value = 0;
    unsigned int monStartVal = 0;

    unsigned long           ulHigh32;
    unsigned long           ulLow32;
    unsigned long long      u64Data;

    if(0 == id)
    {
        monStartVal = (1<<AXI_MON0_START_BIT);
        u64Data = BSP_REG64(WIN_COUNTER_ADDR(AXI_MONITOR_0_ADDR));
    }
    else
    {
        monStartVal = (1<<AXI_MON1_START_BIT);
        u64Data = BSP_REG64(WIN_COUNTER_ADDR(AXI_MONITOR_1_ADDR));
    }

    ulHigh32 = (u64Data & 0xFFFFFFFF00000000) >> 32;
    ulLow32 = (u64Data & 0xFFFFFFFF);

    printk("BSP_Axi_Mon_Start :%d, windows count, ulHigh32 = 0x%lx, ulLow32 = 0x%lx.\r\n", id,ulHigh32, ulLow32);
    //printk("BSP_Axi_Mon_Start,timers:%d \r\n",monitorTimes);

    /*start monitor*/
    BSP_REG_READ_BASE(SC_AXI_MONITOR_ADDR, u32Value);
    u32Value = u32Value | monStartVal;
    BSP_REG_WRITE_BASE(SC_AXI_MONITOR_ADDR, u32Value);

}


/*****************************************************************************
 �� �� ��  : BSP_Axi_Mon_Stop
 ��������  : ���ֹͣ
 �������  : monId:monitor id(0/1)
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��25��
    ��    ��   : �ż� 00179562
    �޸�����   : �����ɺ���

*****************************************************************************/
void BSP_Axi_Mon_Stop(unsigned int id)
{
    unsigned long axi_1_va;
    unsigned int     u32Value;
    cnt_state_int cnt_state_int_mon_1;
    unsigned int monStartVal = 0;

    unsigned int monAddr = 0;
    monAddr = BSP_Axi_Get_Module_Addr(id);


    if(0 == id)
    {
        monStartVal = ~(1<<AXI_MON0_START_BIT);
    }
    else
    {
        monStartVal = ~(1<<AXI_MON1_START_BIT);
    }

    axi_1_va = CNT_STATE_INT_ADDR(monAddr);
    cnt_state_int_mon_1 = (cnt_state_int)BSP_REG_BASE(axi_1_va);//32

    BSP_REG_READ_BASE(SC_AXI_MONITOR_ADDR, u32Value);

    u32Value = u32Value & monStartVal;

    BSP_REG_WRITE_BASE(SC_AXI_MONITOR_ADDR, u32Value);

    while(1)
    {
         cnt_state_int_mon_1 = (cnt_state_int)BSP_REG_BASE(axi_1_va);//32
        if((cnt_state_int_mon_1.bits.system_state == 0x3) )
        {
            break;
        }
    }

    /*printk("BSP_Axi_MON_Stop id:%d.\r\n",id);*/
}

/*****************************************************************************
 �� �� ��  : BSP_AXI_Print
 ��������  : ��ӡ�������
 �������  : monId:monitor id(0/1)
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��25��
    ��    ��   : �ż� 00179562
    �޸�����   : �����ɺ���

*****************************************************************************/
void BSP_Axi_Mon_Print(unsigned int id)
{
    unsigned int             ids;
    unsigned long long      read_bytes_id_mon;//64
    unsigned long long      write_bytes_id_mon;//64
    unsigned long           ulHigh32;
    unsigned long           ulLow32;

    unsigned long long      u64Data;
    unsigned int monAddr = 0;
    monAddr = BSP_Axi_Get_Module_Addr(id);

    printf_axi_m("BSP_AXI_Print_ToDDR, start id:%d,addr:0x%x.\r\n",id,monAddr);

    u64Data = BSP_REG64(WIN_COUNTER_ADDR(monAddr));

    ulHigh32 = (u64Data & 0xFFFFFFFF00000000) >> 32;
    ulLow32 = (u64Data & 0xFFFFFFFF);

    printf_axi_m("windows count, ulHigh32 = 0x%x, ulLow32 = 0x%x.\r\n", ulHigh32, ulLow32);

    printf_axi_m(" index  READ_BYTES WRITE_BYTES INCR1_ID   INCR2_ID   INCR4_ID   INCR8_ID   INCR16_ID   WRAP_ID   BURST_ID   FINISH_ID \r\n");

    for (ids = 0; ids < 8; ids ++)
    {
        printf_axi_m("%2d  ", ids);

        read_bytes_id_mon = BSP_REG64(READ_BYTES_ID_AADR(monAddr, ids));//64
        ulHigh32 = (read_bytes_id_mon & 0xFFFFFFFF00000000) >> 32;
        ulLow32 = (read_bytes_id_mon & 0xFFFFFFFF);
        printf_axi_m("0x%x 0x%x  ", ulHigh32, ulLow32);

        write_bytes_id_mon = BSP_REG64(WRITE_BYTES_ID_AADR(monAddr, ids));//64
        ulHigh32 = (write_bytes_id_mon & 0xFFFFFFFF00000000) >> 32;
        ulLow32 = (write_bytes_id_mon & 0xFFFFFFFF);
        printf_axi_m("0x%x 0x%x  ", ulHigh32, ulLow32);

        u64Data = BSP_REG64(INCR1_ID_AADR(monAddr, ids));//64
        ulHigh32 = (u64Data & 0xFFFFFFFF00000000) >> 32;
        ulLow32 = (u64Data & 0xFFFFFFFF);
        printf_axi_m("0x%x 0x%x  ", ulHigh32, ulLow32);

        u64Data = BSP_REG64(INCR2_ID_AADR(monAddr, ids));//64
        ulHigh32 = (u64Data & 0xFFFFFFFF00000000) >> 32;
        ulLow32 = (u64Data & 0xFFFFFFFF);
        printf_axi_m("0x%x 0x%x  ", ulHigh32, ulLow32);

        u64Data = BSP_REG64(INCR4_ID_AADR(monAddr, ids));//64
        ulHigh32 = (u64Data & 0xFFFFFFFF00000000) >> 32;
        ulLow32 = (u64Data & 0xFFFFFFFF);
        printf_axi_m("0x%x 0x%x  ", ulHigh32, ulLow32);

        u64Data = BSP_REG64(INCR8_ID_AADR(monAddr, ids));//64
        ulHigh32 = (u64Data & 0xFFFFFFFF00000000) >> 32;
        ulLow32 = (u64Data & 0xFFFFFFFF);
        printf_axi_m("0x%x 0x%x  ", ulHigh32, ulLow32);

        u64Data = BSP_REG64(INCR16_ID_AADR(monAddr, ids));//64
        ulHigh32 = (u64Data & 0xFFFFFFFF00000000) >> 32;
        ulLow32 = (u64Data & 0xFFFFFFFF);
        printf_axi_m("0x%x 0x%x  ", ulHigh32, ulLow32);

        u64Data = BSP_REG64(WRAP_ID_AADR(monAddr, ids));//64
        ulHigh32 = (u64Data & 0xFFFFFFFF00000000) >> 32;
        ulLow32 = (u64Data & 0xFFFFFFFF);
        printf_axi_m("0x%x 0x%x  ", ulHigh32, ulLow32);

        u64Data = BSP_REG64(BURST_ID_AADR(monAddr, ids));//64
        ulHigh32 = (u64Data & 0xFFFFFFFF00000000) >> 32;
        ulLow32 = (u64Data & 0xFFFFFFFF);
        printf_axi_m("0x%x 0x%x  ", ulHigh32, ulLow32);

        u64Data = BSP_REG64(FINISH_ID_AADR(monAddr, ids));//64
        ulHigh32 = (u64Data & 0xFFFFFFFF00000000) >> 32;
        ulLow32 = (u64Data & 0xFFFFFFFF);
        printf_axi_m("0x%x 0x%x  ", ulHigh32, ulLow32);

        printf_axi_m("\n");
    }

    printf_axi_m("BSP_AXI_Print_ToDDR, end.\r\n");
}

/*****************************************************************************
 �� �� ��  : BSP_Axi_Mon_Reset
 ��������  : ��λmonitor
 �������  : monId:monitor id(0/1)
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��25��
    ��    ��   : �ż� 00179562
    �޸�����   : �����ɺ���

*****************************************************************************/
void BSP_Axi_Mon_Reset(unsigned int monId)
{
    unsigned int monAddr = 0;
    monAddr = BSP_Axi_Get_Module_Addr(monId);

    BSP_REG_WRITE_BASE(CNT_RESET_ADDR(monAddr), 0x0003);

    if(AXI_Wait_Monitor_Ready(monAddr) )
    {
        printk("Reset Failed!Please Try again!\n");
        return;
    }

}
/*****************************************************************************
 �� �� ��  : AXI_Wait_Monitor_Ready
 ��������  : �ȴ���λ���
 �������  : monitor_addr:monitor addr
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��25��
    ��    ��   : �ż� 00179562
    �޸�����   : �����ɺ���

*****************************************************************************/
int AXI_Wait_Monitor_Ready(unsigned int monitor_addr)
{
    unsigned int u32Value = 0x0;
    int s32Loop = MONITOR_CLK_TIMEOUT;
    BSP_REG_READ_BASE(CNT_STATE_INT_ADDR(monitor_addr), u32Value);
    if(!monitor_addr)
    {
        /*dead loop*/
        printk("AXI_Wait_Monitor_Stop: Monitor ADDR NULL!\n");
        return -1;
    }
    do
    {

        BSP_REG_READ_BASE(CNT_STATE_INT_ADDR(monitor_addr), u32Value);
		/*Release CPU*/
		s32Loop--;
		msleep(1);
    }while((0x10 != (u32Value & 0x10)));

    return 0;
}

/*****************************************************************************
 �� �� ��  : axi_monitor_cnt_type_set
 ��������  : ���ü������ ��orд
 �������  : base_addr:monitor addr
           : ct:�������
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��25��
    ��    ��   : �ż� 00179562
    �޸�����   : �����ɺ���

*****************************************************************************/
static void axi_monitor_cnt_type_set(unsigned long base_addr,  SOC_CNT_TYPE ct)
{
	int i = 0;
	unsigned long mode=0;
	if(ct <= SOC_SEL_DEFAULT || ct >= SOC_SEL_CNT_TYPE_MAX)
		ct = SOC_SEL_READ;
	mode = (ct==SOC_SEL_READ)? 0x0:0x3bf ;
	for (i = 0; i < 8 ; i++)
	{
		BSP_REG_WRITE_BASE(CNT_TYPE_ADDR(base_addr, i), mode);
	}
}
/*****************************************************************************
 �� �� ��  : BSP_Axi_Get_Module_Addr
 ��������  : ��ȡ�����ַ
 �������  : id:monitor id
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��25��
    ��    ��   : �ż� 00179562
    �޸�����   : �����ɺ���

*****************************************************************************/
unsigned int BSP_Axi_Get_Module_Addr(unsigned int id)
{
    unsigned int moduleAddr = 0;
    switch(id)
    {
        case 0:
            moduleAddr = AXI_MONITOR_0_ADDR;
            break;
        case 1:
            moduleAddr = AXI_MONITOR_1_ADDR;
            break;
        case 0xff:
            moduleAddr = SC_AXI_MONITOR_ADDR;
            break;
        default:
            moduleAddr = SC_AXI_MONITOR_ADDR;
            break;
    }

    return moduleAddr;
}



/*****************************************************************************
 �� �� ��  : printf_axi_m
 ��������  : �����ݴ�ӡ���ļ��ӿ�
 �������  : ��printf���ƣ��ļ�����·����LOG_PATH����
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��25��
    ��    ��   : �ż� 00179562
    �޸�����   : �����ɺ���

*****************************************************************************/
#if 1
int printf_axi_m(const char *fmt, ...)
{
	char *buf_t = kzalloc(SZ_4K, GFP_KERNEL);
	va_list args;
	int i;
	struct file *filp;

	va_start(args, fmt);
	i = vsnprintf(buf_t, INT_MAX, fmt, args);
	va_end(args);
	{
        loff_t pos;
		mm_segment_t sd_fs = 0;
		sd_fs = get_fs();
		set_fs(KERNEL_DS);
		filp = filp_open(LOG_PATH, O_WRONLY|O_CREAT, 0644);
		pos = filp->f_pos;
		vfs_write(filp, buf_t, i, &pos);
		filp->f_pos = pos;
		filp_close(filp,NULL);
		set_fs(sd_fs);
	}
	kfree(buf_t);
	msleep(1);
	return i;
}
#endif
//#define printf_axi_m printk
/*****************************************************************************
 �� �� ��  : BSP_AXI_Print_ToDDR_ID
 ��������  : ������ID���ݴ�ӡ��DDR
 �������  : id_data:id��Ӧ�����Ϣ
           : base_addr:monitor����ַ
           : u32ID:��Ӧid��(1-8)
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��25��
    ��    ��   : �ż� 00179562
    �޸�����   : �����ɺ���

*****************************************************************************/
void BSP_AXI_Print_ToDDR_ID(monitor_id_data* id_data,unsigned long base_addr, unsigned long u32ID)
{
    //monitor_id_data* id_data = output_data->id_data[ids -1];
    unsigned long long read_bytes,write_bytes;
    unsigned int ids = u32ID - 1;
    port_sel      port_sel_mon     = (port_sel)BSP_REG_BASE(CNT_PORT_SEL_ADDR(base_addr));//32
    unsigned long prot_sel = ((port_sel_mon.port_sel_reg >> ((u32ID - 1)*2)) & 0x3);
    read_bytes_id           read_bytes_id_mon = (read_bytes_id)BSP_REG64(READ_BYTES_ID_AADR(base_addr, ids));//64
    write_bytes_id          write_bytes_id_mon = (write_bytes_id)BSP_REG64(WRITE_BYTES_ID_AADR(base_addr, ids));//64
	//cnt_id                  cnt_id_mon = (cnt_id)BSP_REG_BASE(CNT_ID_ADDR(base_addr, ids));//32
	//id_addr_des             id_addr_des_mon = (id_addr_des)BSP_REG_BASE(ID_ADDR_DES_AADR(base_addr, ids));//32
	//id_addr_des_m           id_addr_des_m_mon = (id_addr_des_m)BSP_REG_BASE(ID_ADDR_DES_M_AADR(base_addr, ids));//32
	//wrap_id                 wrap_id_mon = (wrap_id)BSP_REG64(WRAP_ID_AADR(base_addr, ids)); //64
	//read_bytes_id           read_bytes_id_mon = (read_bytes_id)BSP_REG64(READ_BYTES_ID_AADR(base_addr, ids));//64
	//write_bytes_id          write_bytes_id_mon = (write_bytes_id)BSP_REG64(WRITE_BYTES_ID_AADR(base_addr, ids));//64
	read_bytes = read_bytes_id_mon.bits.cnt_id_rd_bytes;
	write_bytes =write_bytes_id_mon.bits.cnt_id_wr_bytes;
	id_data->cnt_type_mon = (cnt_type)BSP_REG_BASE(CNT_TYPE_ADDR(base_addr, ids));
	id_data->incr1_id_mon = (incr1_id)BSP_REG64(INCR1_ID_AADR(base_addr, ids));//64
	id_data->incr2_id_mon = (incr2_id)BSP_REG64(INCR2_ID_AADR(base_addr, ids));//64
	id_data->incr4_id_mon = (incr4_id)BSP_REG64(INCR4_ID_AADR(base_addr, ids));//64
	id_data->incr8_id_mon = (incr8_id)BSP_REG64(INCR8_ID_AADR(base_addr, ids));//64
	id_data->incr16_id_mon = (incr16_id)BSP_REG64(INCR16_ID_AADR(base_addr, ids));//64
	id_data->burst_id_mon  = (burst_id)BSP_REG64(BURST_ID_AADR(base_addr, ids));//64
	id_data->finish_id_mon = (finish_id)BSP_REG64(FINISH_ID_AADR(base_addr, ids));//64
	id_data->wrap_id_mon = (wrap_id)BSP_REG64(WRAP_ID_AADR(base_addr,ids));
    id_data->id = u32ID;
    id_data->monitor_id = (base_addr == AXI_MONITOR_0_ADDR) ? 0  : 1;
    id_data->port_sel = prot_sel;
    id_data->read_bytes = read_bytes;
    id_data->write_bytes = write_bytes;
}

/*****************************************************************************
 �� �� ��  : BSP_AXI_Print_ToDDR
 ��������  : ��monitorģ�����ݴ�ӡ��DDR
 �������  : output_data:DDR�����浽�ļ�������
           : modId:monitorģ��ID�� 0��1
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��25��
    ��    ��   : �ż� 00179562
    �޸�����   : �����ɺ���

*****************************************************************************/
void BSP_AXI_Print_ToDDR(monitor_output_data* output_data,unsigned int modId)
{

    unsigned int addr = 0;
    win_counter   win_counter_mon_1;
    win_counter   win_counter_mon_2;
    if(unlikely(!output_data))
        return;

    addr = BSP_Axi_Get_Module_Addr(modId);

    win_counter_mon_1  = (win_counter)BSP_REG64(WIN_COUNTER_ADDR(addr));//64
    win_counter_mon_2  = (win_counter)BSP_REG64(WIN_COUNTER_ADDR(addr));//64
    output_data->win_counter[0] = win_counter_mon_1.bits.win_counter;
    output_data->win_counter[1] = win_counter_mon_2.bits.win_counter;

    BSP_AXI_Print_ToDDR_ID(&output_data->id_data[0], addr, 1);
    BSP_AXI_Print_ToDDR_ID(&output_data->id_data[1], addr, 2);
    BSP_AXI_Print_ToDDR_ID(&output_data->id_data[2], addr, 3);
    BSP_AXI_Print_ToDDR_ID(&output_data->id_data[3], addr, 4);
    BSP_AXI_Print_ToDDR_ID(&output_data->id_data[4], addr, 5);
    BSP_AXI_Print_ToDDR_ID(&output_data->id_data[5], addr, 6);
    BSP_AXI_Print_ToDDR_ID(&output_data->id_data[6], addr, 7);
    BSP_AXI_Print_ToDDR_ID(&output_data->id_data[7], addr, 8);
}

/*****************************************************************************
 �� �� ��  : Axi_Mon_Timer_Isr
 ��������  : ��ʱ����жϷ������
 �������  : null
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��25��
    ��    ��   : �ż� 00179562
    �޸�����   : �����ɺ���

*****************************************************************************/
void BSP_Axi_Mon_Dump_Isr(void)
{

#if 1
    printk("Axi_Mon_Timer_Isr,timers:%d \r\n",monitorTimes);

    BSP_Axi_Mon_Stop(mon_param.monitorId);

    BSP_AXI_Print_ToDDR(index_axi_monitor_output,mon_param.monitorId);
    index_axi_monitor_output++;
    monitorTimes ++ ;

    /*���AXI_MONITOR_TEST_TIMES�κ�dump*/
    if(AXI_MONITOR_TEST_TIMES == monitorTimes)
    {
        /*���ֵ���ֹͣ��ʱ���*/
        DRV_TIMER_STOP(mon_param.axi_timerId);
        monitorTimes = 0;

        /*�ͷ��ź����������Ϣ�����߳�*/
        up(&sem_print_axi);
    }
    else
    {
        /*�����´μ��*/
        BSP_REG_WRITE_BASE(CNT_RESET_ADDR(BSP_Axi_Get_Module_Addr(mon_param.monitorId)), 0x0003);
        BSP_Axi_Mon_Start(mon_param.monitorId);
    }
#endif

}

/*****************************************************************************
 �� �� ��  : Axi_Mon_Print_Task
 ��������  : �����Ϣdump�߳�
 �������  : null
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��25��
    ��    ��   : �ż� 00179562
    �޸�����   : �����ɺ���

*****************************************************************************/
int BSP_Axi_Mon_Dump_Task(void)
{
    int i = 0,j = 0;
    monitor_output_data* tmp  = axi_monitor_output;
	char* wrap_type[4] = {"WRAP4_RD","WRAP4_WR","WRAP8_RD","WRAP8_WR"};

    for(;;)
    {
        /*�ȴ���ش��������ͷ��ź��������ڴ�dump���ݵ��ļ���*/
        down(&sem_print_axi);

#if 1
        printf_axi_m("num       wc(cycl)       s1_r       s1_w       s2_r       s2_w       s3_r       s3_w       s4_r       s4_w        s5_r       s5_w        s_r       s6_w       s7_r       s7_w       s8_r       s8_w\n");

    	tmp  = axi_monitor_output;

    	for(i =0;i < AXI_MONITOR_TEST_TIMES;i++, tmp++)
        {
           printf_axi_m("%-10d %-10lu",
                    (i+1),
                    (0xffffffff - ((unsigned long)(tmp->win_counter[mon_param.monitorId] &0xffffffff))));

           for(j = 0; j< BALONG_MONITOR_REQUEST_IDS;j++)
           {
                printf_axi_m("%10llu %10llu ", tmp->id_data[j].read_bytes, tmp->id_data[j].write_bytes);
           }
           printf_axi_m("\n");
           msleep(20);
        }
    	printf_axi_m("\n\n");


    	printf_axi_m("Detail burst Info:\n");
    	tmp  = axi_monitor_output;

    	printf_axi_m("%-12s\t", "num");
    	for(j = 0; j< BALONG_MONITOR_REQUEST_IDS;j++)
    	{
    		char *t = (0 == tmp->id_data[j].monitor_id) ? "0": "1";/*monitor 0 or 1*/
    		//printf_htimer("%-12s\t", "num");
    		printf_axi_m("%1s_%1u_FINISH_%s\t",t, tmp->id_data[j].id,(tmp->id_data[j].cnt_type_mon.bits.finish_type) ? "WR" : "RD");
    		printf_axi_m("%1s_%1u_BURST_%s\t",t, tmp->id_data[j].id,(tmp->id_data[j].cnt_type_mon.bits.burst_bype) ? "WR" : "RD");
    		printf_axi_m("%1s_%1u_%s\t",t, tmp->id_data[j].id,wrap_type[tmp->id_data[j].cnt_type_mon.bits.wrap_type]);
    		printf_axi_m("%1s_%1u_%s\t", t, tmp->id_data[j].id,
    			tmp->id_data[j].cnt_type_mon.bits.incr_type_incr1 ? "INCR1_WR" : "INCR1_RD");
    		printf_axi_m("%1s_%1u_%s\t", t, tmp->id_data[j].id,
    			tmp->id_data[j].cnt_type_mon.bits.incr_type_incr2 ? "INCR2_WR" : "INCR2_RD");
    		printf_axi_m("%1s_%1u_%s\t", t, tmp->id_data[j].id,
    			tmp->id_data[j].cnt_type_mon.bits.incr_type_incr4 ? "INCR4_WR" : "INCR4_RD");
    		printf_axi_m("%1s_%1u_%s\t", t, tmp->id_data[j].id,
    			tmp->id_data[j].cnt_type_mon.bits.incr_type_incr8 ? "INCR8_WR" : "INCR8_RD");
    		printf_axi_m("%1s_%1u_%s\t", t, tmp->id_data[j].id,
    			tmp->id_data[j].cnt_type_mon.bits.incr_type_incr16 ? "INCR16_WR" : "INCR16_RD");
    		msleep(1);
    	}
    	printf_axi_m("\n");

    	tmp  = axi_monitor_output;

    	for(i =0;i < AXI_MONITOR_TEST_TIMES;i++, tmp++)
    	{
    		printf_axi_m("%-12u\t", i);
    		for(j = 0; j< BALONG_MONITOR_REQUEST_IDS ;j++)
    		{
    			//printf_axi_m("%-12u\t", i);
    			printf_axi_m("%-12u\t",tmp->id_data[j].finish_id_mon.bits.cnt_id_finish);/*burst rd wr*/
    			printf_axi_m("%-12u\t",tmp->id_data[j].burst_id_mon.bits.cnt_id_burst);/*burst rd wr*/
    			printf_axi_m("%-12u\t", tmp->id_data[j].wrap_id_mon.bits.cnt_id_wrap4_rd );/*wrap_type ID count*/
    			printf_axi_m("%-12u\t", tmp->id_data[j].incr1_id_mon.bits.cnt_id_incr1);/*incr1 count*/
    			printf_axi_m("%-12u\t", tmp->id_data[j].incr2_id_mon.bits.cnt_id_incr2);/*incr2 count*/
    			printf_axi_m("%-12u\t", tmp->id_data[j].incr4_id_mon.bits.cnt_id_incr4);/*incr4 count*/
    			printf_axi_m("%-12u\t", tmp->id_data[j].incr8_id_mon.bits.cnt_id_incr8);/*incr8 count*/
    			printf_axi_m("%-12u\t", tmp->id_data[j].incr16_id_mon.bits.cnt_id_incr16);/*incr16 count*/
    			msleep(1);
    		}
    		printf_axi_m("\n");
    		msleep(20);
    	}
		printf_axi_m("\n\n");

    	/*ָ���0*/
        index_axi_monitor_output = axi_monitor_output;
        memset(axi_monitor_output,0,sizeof(monitor_output_data));
        /*�Ƿ�������һ��dump*/
    	if(0 != mon_param.loopMonitor)
    	{
            BSP_Axi_Mon_Reset(mon_param.monitorId);
            BSP_Axi_Mon_Start(mon_param.monitorId);
            DRV_TIMER_START(mon_param.axi_timerId, (FUNCPTR_1)BSP_Axi_Mon_Dump_Isr, 0, mon_param.timeCycle, 1, mon_param.timerMode);
    	}
	}
#endif
}
/*****************************************************************************
 �� �� ��  : Axi_Mon_Test_Entry
 ��������  : monitor��ں���
 �������  : modId:���ģ��id(0/1)
           : rwType:�������(��/д)
           : cycle:��ʱ�������ʱ�жϴ���ʱ��
           : timerId:��ʱ��id
           : mode:��ʱģʽ0:ms,1:us
           : loopMode:�Ƿ�ѭ��dump
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��25��
    ��    ��   : �ż� 00179562
    �޸�����   : �����ɺ���

*****************************************************************************/
void Axi_Mon_Entry(unsigned int modId,SOC_CNT_TYPE rwType,unsigned int cycle,unsigned int timerId,unsigned int mode,unsigned int loopMode)
{
    int ret = 0;
    mon_param.axi_mon_test_type = rwType;
    mon_param.axi_timerId = timerId;
    mon_param.monitorId = modId;
    mon_param.loopMonitor = loopMode;
    mon_param.timeCycle = cycle;
    mon_param.timerMode = mode;

    DRV_TIMER_STOP(timerId);
    BSP_Axi_Mon_Config(modId,mon_param.axi_mon_test_type);
    BSP_Axi_Mon_Start(modId);

    ret = DRV_TIMER_START(mon_param.axi_timerId, (FUNCPTR_1)BSP_Axi_Mon_Dump_Isr, 0, mon_param.timeCycle, 1, mon_param.timerMode);
    if(0 > ret)
    {
        printk("AXI_Monitor_Timer_Init failed!\r\n");
    }

    printk("Axi_Mon_Test Start...!\r\n");
}
void Axi_Mon_Entry_Stop(void)
{
    DRV_TIMER_STOP(mon_param.axi_timerId);
    BSP_Axi_Mon_Stop(mon_param.monitorId);

    //index_axi_monitor_output = axi_monitor_output;
    monitorTimes = 0;
    /*�ͷ��ź����������Ϣ�����߳�*/
    printk("Axi_Mon_Entry_Stop !\r\n");

    up(&sem_print_axi);
}


#if 1
/*just for test*/
void AXI_Monitor_Isr_test(void)
{
    printk("AXI_Monitor_isr_test \r\n");
}
void AXI_Monitor_Timer_Test(unsigned int timerId,unsigned int timeValue,unsigned int loopMode,unsigned int countMode)
{
    int ret = 0;
    ret = DRV_TIMER_START(timerId, (FUNCPTR_1)AXI_Monitor_Isr_test, 0, timeValue, loopMode, countMode);
    if(0 > ret)
    {
        printk("AXI_Monitor_Timer_Init failed!\r\n");
    }
    printk("AXI_Monitor_Timer_Test ok!\r\n");

}
#endif
/*****************************************************************************
 �� �� ��  : BSP_Axi_Mon_Active
 ��������  : �����ģ�鼤��
 �������  : ��
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��25��
    ��    ��   : �ż� 00179562
    �޸�����   : �����ɺ���

*****************************************************************************/
void BSP_Axi_Mon_Active(void)
{
    unsigned int val = 0;
    unsigned int addr = 0;

    /*�⸴λVPP,����a monitor0�Ĵ����޷�����*/
    BSP_REG_READ_BASE(AXI_MONITOR_ADDR(INTEGRATOR_SC_BASE_K,0x0800),val);
    val |= 0x200;
    BSP_REG_WRITE_BASE(AXI_MONITOR_ADDR(INTEGRATOR_SC_BASE_K,0x0800),val);/*0xf7410800*/
    BSP_REG_READ_BASE(AXI_MONITOR_ADDR(INTEGRATOR_SC_BASE_K,0x0814),val);
    val |= 0x200;
    BSP_REG_WRITE_BASE(AXI_MONITOR_ADDR(INTEGRATOR_SC_BASE_K,0x0814),val);/*0xf7410814*/
    BSP_REG_READ_BASE(AXI_MONITOR_ADDR(INTEGRATOR_SC_BASE_K,0x0824),val);
    val |= 0x200;
    BSP_REG_WRITE_BASE(AXI_MONITOR_ADDR(INTEGRATOR_SC_BASE_K,0x0824),val);/*0xf7410824*/
    BSP_REG_READ_BASE(AXI_MONITOR_ADDR(INTEGRATOR_SC_BASE_K,0x0830),val);
    val |= 0x200;
    BSP_REG_WRITE_BASE(AXI_MONITOR_ADDR(INTEGRATOR_SC_BASE_K,0x0830),val);/*0xf7410830*/
    addr = (unsigned int)(ioremap(0xf711a718,SZ_4K));
    BSP_REG_READ_BASE(addr,val);
    val |= 0x20000;
    BSP_REG_WRITE_BASE(addr,val);/*0xf711a718*/

    printk("BSP_Axi_Mon_Active OK!\n");

}
/*****************************************************************************
 �� �� ��  : BSP_Axi_Monitor_Init
 ��������  : ģ���ʼ������
 �������  : ��
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��25��
    ��    ��   : �ż� 00179562
    �޸�����   : �����ɺ���

*****************************************************************************/
int BSP_Axi_Mon_Init(void)
{
    unsigned int val = 0;
   
    /*��ַӳ��*/
    map_address_sc = (void*)(IO_ADDRESS(TMP_INTEGRATOR_SC_BASE));
    map_address_mon1= ioremap(TMP_AXI_MONITOR_1_ADDR,SZ_4K);
    map_address_mon0= ioremap(TMP_AXI_MONITOR_0_ADDR,SZ_4K);

    if((0 == map_address_mon1) || (0 == map_address_mon0))
    {
        printk(KERN_ERR"Monitor remap failed!\n");
        return -1;
    }
    sema_init(&sem_print_axi, 0);

    /*ʹ��AMONITOR0��AMONITOR1*/
    BSP_REG_READ_BASE(SC_AXI_MONITOR_ADDR, val);
    printk("BSP_Axi_Monitor_Init,old val:0x%x!\n",val);
    val |= ((1<<AXI_MON0_ENABLE_BIT)|(1<<AXI_MON1_ENABLE_BIT));
    printk("BSP_Axi_Monitor_Init,new val:0x%x!\n",val);
    BSP_REG_WRITE_BASE(SC_AXI_MONITOR_ADDR, val);
    /*�������󼤻�*/
    BSP_Axi_Mon_Active();

    /*Ϊ�����Ϣ����buffer�����ڴ�*/
    axi_monitor_output = (monitor_output_data*)vmalloc(AXI_MONITOR_TEST_TIMES*sizeof(monitor_output_data));
    memset(axi_monitor_output,0,AXI_MONITOR_TEST_TIMES*sizeof(monitor_output_data));
    if(!axi_monitor_output)
        return -1;
    index_axi_monitor_output = axi_monitor_output;


    /*�������ݴ�ӡ�߳�*/
    if(NULL == g_printMonitorThread)
    {
        g_printMonitorThread = kthread_run(BSP_Axi_Mon_Dump_Task,NULL,"monitor_dump_task");
    }


    printk("BSP_Axi_Monitor_Init OK!\n");

    return 0;
}

void BSP_Axi_Mon_Exit(void)
{
    return;
}
/*module_init(BSP_Axi_Mon_Init);*/

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


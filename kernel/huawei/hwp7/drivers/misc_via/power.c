/*
 * viatel_cbp_power.c
 *
 * VIA CBP driver for Linux
 *
 * Copyright (C) 2009 VIA TELECOM Corporation, Inc.
 * Author: VIA TELECOM Corporation, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.  */
#include <linux/ctype.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <linux/wakelock.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include "viatel.h"
#include "core.h"
#include <hsad/config_interface.h>
#include <hsad/config_mgr.h>
#include <linux/log_exception.h>
#include <linux/rtc.h>

/* ioctl for vomodem, which must be same as viatelutils.h  */
#define VMDM_IOCTL_RESET	_IO( 'v', 0x01)
#define VMDM_IOCTL_POWER	_IOW('v', 0x02, int)
#define VMDM_IOCTL_CRL	_IOW('v', 0x03, int)
#define VMDM_IOCTL_DIE		_IO( 'v', 0x04)
#define VMDM_IOCTL_WAKE		_IO( 'v', 0x05)


#define POWER_SET_DEBUGOFF     0
#define POWER_SET_DEBUGON      1
#define POWER_SET_OFF          2
#define POWER_SET_ON           3

static const char* const via_state_str[] = {
    "MODEM_STATE_OFF",
    "MODEM_STATE_POWER",
    "MODEM_STATE_READY",
    "MODEM_STATE_INVALID",
};

typedef enum ENUM_MODEM_STATE {
    MODEM_STATE_OFF=0,
    MODEM_STATE_POWER,
    MODEM_STATE_READY,
    MODEM_STATE_INVALID,
} ENUM_MODEM_STATE_T;

int cbp_rst_gpio = 0;
int cbp_pwr_en_gpio = 0;
int cbp_rst_ind_gpio = 0;
int cbp_sim_switch_gpio = 0;
int cbp_backup_gpio = 0;

static int via_curr_power_state;

static int via_modem_state = MODEM_STATE_OFF;

#define MODEM_VIACBP82D   "viacbp82d"
#define MODEM_DEVICE_BOOT(type)   type"_boot"

/* event for vmodem, which must be same as viatelutilis.h */
enum ASC_USERSPACE_NOTIFIER_CODE{
    ASC_USER_USB_WAKE =  (__SI_POLL|100),
    ASC_USER_USB_SLEEP,
    ASC_USER_UART_WAKE,
    ASC_USER_UART_SLEEP,
    ASC_USER_SDIO_WAKE,
    ASC_USER_SDIO_SLEEP,
    ASC_USER_MDM_POWER_ON = (__SI_POLL|200),
    ASC_USER_MDM_POWER_OFF,
    ASC_USER_MDM_RESET_ON,
    ASC_USER_MDM_RESET_OFF,
    ASC_USER_MDM_ERR = (__SI_POLL|300),
    ASC_USER_MDM_ERR_ENHANCE
};

#define MDM_RST_LOCK_TIME   (120) 
#define MDM_RST_HOLD_DELAY  (100) //ms
#define MDM_PWR_HOLD_DELAY  (500) //ms
#define QUEUE_NUM   8

struct viatel_modem_data {
    struct platform_device *via_pdev;
    struct fasync_struct *fasync;
    struct raw_notifier_head ntf;
    struct notifier_block rst_ntf;
    struct wake_lock wlock;
    struct work_struct work;
    struct work_struct via_uevent_work;
    char *envp[QUEUE_NUM][2];
    unsigned char head;
    unsigned char tail;
    spinlock_t lock;
    atomic_t count;
    unsigned long ntf_flags;
};

static struct viatel_modem_data *vmdata;

extern int modem_on_off_ctrl_chan(unsigned char on);
extern void gpio_irq_cbp_rst_ind(void);

static int cbp_need_apr = 0;
static time_buf[16] ={0};

static void set_time_stamp(void)
{
	struct timeval tv;
	struct rtc_time tm;
	memset(&tv, 0, sizeof(struct timeval));
	memset(&tm, 0, sizeof(struct rtc_time));
	do_gettimeofday(&tv);
	tv.tv_sec -= sys_tz.tz_minuteswest * 60;
	rtc_time_to_tm(tv.tv_sec, &tm);
	memset(time_buf, 0, sizeof(time_buf));
	pr_err("%s sizeof %d\n", __func__, sizeof(time_buf));
	snprintf(time_buf, 16,"%04d%02d%02d%02d%02d%02d",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

static void get_time_stamp(char* timestamp_buf,  unsigned int len)
{
	pr_err("%s sizeof %d\n", __func__, sizeof(timestamp_buf));
	strncpy(timestamp_buf, time_buf, len);
}

static void apr_log_entry(struct work_struct *dummy)
{
	char cmd[256];
	char time_buf[16];
	int ret = 0;
	get_time_stamp(time_buf,16);
	snprintf(cmd, 256, "%s%s%s%s%s",
			"archive -i /data/flashless",
			" -i /data/android_logs/kmsgcat-log -i /data/android_logs/kmsgcat-log.1",
			" -o ", time_buf, "_VIAMODEM -z zip");
	ret = log_to_exception("viamodem",cmd);
	if(ret < 0 ){
		pr_err("%s logexception sysfs err.\n", __func__);
	}
	pr_info("%s %d cmd %s ret %d\n", __func__, __LINE__, cmd, ret);

}
static DECLARE_DELAYED_WORK(apr_log_wk, apr_log_entry);

//schedule a work to send uevent
static void via_uevent_work_func(struct work_struct *data)
{
    struct viatel_modem_data *via_boot = vmdata;
    unsigned long flags;

    while (via_boot->head != via_boot->tail)
    {
        kobject_uevent_env(&vmdata->via_pdev->dev.kobj, KOBJ_CHANGE, &vmdata->envp[via_boot->head][0] );
        dev_info(&vmdata->via_pdev->dev, "%s. notify event[%d,%d] %s to userspace!\n",__func__,
                 via_boot->tail, via_boot->head, vmdata->envp[via_boot->head][0]);

        spin_lock_irqsave(&vmdata->lock, flags);
        vmdata->envp[via_boot->head][0] = NULL;
        via_boot->head = (via_boot->head + 1) % QUEUE_NUM;
        spin_unlock_irqrestore(&vmdata->lock, flags);
    }
}

/*
 Send the uevent notify message to userspace (rild)
*/
void via_monitor_uevent_notify(int event)
{
    struct viatel_modem_data *via_boot = vmdata;
    int temp=0;
	unsigned long flags;

    if (event >= 0) {
        printk("%s: event=%s\n",__func__,via_state_str[event]);
    }
    else {
        printk("%s: error event < 0 = %d\n",__func__, event);
    }

    spin_lock_irqsave(&vmdata->lock, flags);
    temp = (via_boot->tail + 1) % QUEUE_NUM;
    if (temp != via_boot->head)
    {
        if(event>=MODEM_STATE_OFF && event<MODEM_STATE_INVALID)
            vmdata->envp[via_boot->tail][0] = (char *)via_state_str[event];
        else
            vmdata->envp[via_boot->tail][0] = (char *)via_state_str[MODEM_STATE_INVALID];

        vmdata->envp[via_boot->tail][1] = NULL;
        via_boot->tail = temp;
    }

    schedule_work(&vmdata->via_uevent_work);
    spin_unlock_irqrestore(&vmdata->lock, flags);
}



static void modem_signal_user(int event)
{
    if(vmdata && vmdata->fasync){
        printk("%s: evnet %d.\n",__FUNCTION__, (short)event);
        kill_fasync(&vmdata->fasync, SIGIO, event);
    }
}

/* Protection for the above */
static DEFINE_RAW_SPINLOCK(rslock);

void oem_reset_modem(void)
{
   wake_lock_timeout(&vmdata->wlock, MDM_RST_LOCK_TIME * HZ);
   if(GPIO_OEM_VALID(cbp_rst_gpio)){
       oem_gpio_direction_output(GPIO_VIATEL_CRASH_CBP, 1);
       oem_gpio_direction_output(cbp_rst_gpio, 1);
       mdelay(MDM_RST_HOLD_DELAY);
       oem_gpio_direction_output(cbp_rst_gpio, 0);
       mdelay(MDM_RST_HOLD_DELAY);
   }
   printk("Warnning: reset vmodem\n");

}
EXPORT_SYMBOL(oem_reset_modem);

void oem_power_on_modem(void)
{
   if(GPIO_OEM_VALID(cbp_pwr_en_gpio)){
   	printk("%s %d\n",__func__,__LINE__);
      if(GPIO_OEM_VALID(cbp_rst_gpio)){
	  	printk("%s %d\n",__func__,__LINE__);
		  oem_gpio_direction_output(GPIO_VIATEL_CRASH_CBP, 1);
          oem_gpio_direction_output(cbp_rst_gpio, 0);
          mdelay(MDM_RST_HOLD_DELAY);
		  printk("%s %d\n",__func__,__LINE__);
      }
      oem_gpio_direction_output(cbp_pwr_en_gpio, 1);
      mdelay(MDM_PWR_HOLD_DELAY);
	  printk("%s %d\n",__func__,__LINE__);
   }

   if (cbp_need_apr){
       set_time_stamp();
       schedule_delayed_work(&apr_log_wk, msecs_to_jiffies(60*1000));/*wait for 1 minute due to ramdump may later*/
       cbp_need_apr = 0;
   }
   printk("Warnning: power on vmodem\n");

}
EXPORT_SYMBOL(oem_power_on_modem);

void oem_power_off_modem(void)
{
    if(GPIO_OEM_VALID(cbp_pwr_en_gpio)){
        oem_gpio_direction_output(cbp_pwr_en_gpio, 0);
    }

    if(GPIO_OEM_VALID(cbp_rst_gpio)){
        oem_gpio_direction_output(cbp_rst_gpio, 1);
        /*just hold the reset pin if no power enable pin*/
        if(GPIO_OEM_VALID(cbp_pwr_en_gpio)){
            mdelay(MDM_RST_HOLD_DELAY);
            oem_gpio_direction_output(cbp_rst_gpio, 0);
        }
    }
    printk("Warnning: power off vmodem\n");
}
EXPORT_SYMBOL(oem_power_off_modem);
int modem_err_indication_usr(int revocery)
{
	printk("%s %d revocery=%d\n",__func__,__LINE__,revocery);
	if(revocery){
		printk("%s %d MDM_EVT_NOTIFY_HD_ERR\n",__func__,__LINE__);
		/*1, check the rst_ind*/
		/*2, set GPIO_7_3 low*/
		pr_err("%s %d rst_ind %d\n", __func__, __LINE__, oem_gpio_get_value(cbp_rst_ind_gpio));

		oem_gpio_direction_output(cbp_backup_gpio, 1);
		mdelay(150);
		oem_gpio_direction_output(cbp_backup_gpio, 0);

		cbp_need_apr = 1;
		set_time_stamp();
		schedule_delayed_work(&apr_log_wk, 0);
		modem_notify_event(MDM_EVT_NOTIFY_HD_ERR);
	}
	else{
		printk("%s %d MDM_EVT_NOTIFY_HD_ENHANCE\n",__func__,__LINE__);
		modem_notify_event(MDM_EVT_NOTIFY_HD_ENHANCE);
	}
	return 0;
}
EXPORT_SYMBOL(modem_err_indication_usr);

void oem_let_cbp_die(void)
{
	if(GPIO_OEM_VALID(GPIO_VIATEL_CRASH_CBP)){
		oem_gpio_direction_output(GPIO_VIATEL_CRASH_CBP, 0);
		mdelay(MDM_RST_HOLD_DELAY);
		oem_gpio_direction_output(GPIO_VIATEL_CRASH_CBP, 1);
	}
	printk("let cbp die\n");
}
EXPORT_SYMBOL(oem_let_cbp_die);



static int modem_reset_notify_misc(struct notifier_block *nb, unsigned long event,
			       void *ptr)
{
	switch (event) {
        case MDM_EVT_NOTIFY_RESET_ON:
            modem_signal_user(ASC_USER_MDM_RESET_ON);
            break;
        case MDM_EVT_NOTIFY_RESET_OFF:
            modem_signal_user(ASC_USER_MDM_RESET_OFF);
            break;
        default:
            ;
    }

    return NOTIFY_OK;
}


/*
 * Notify about a modem event change.
 * 
 */
static void modem_notify_task(struct work_struct *work)
{
    int i = 0;

    for(i = 0; i < MDM_EVT_NOTIFY_NUM; i++){
        if(test_and_clear_bit(i, &vmdata->ntf_flags)){
            raw_notifier_call_chain(&vmdata->ntf, i, NULL);
        }
    }
}

void modem_notify_event(int event)
{
    if(vmdata && event < MDM_EVT_NOTIFY_NUM){
        set_bit(event, &vmdata->ntf_flags);
        schedule_work(&vmdata->work);
    }
}
EXPORT_SYMBOL(modem_notify_event);

/*
 *  register a modem events change listener
 */
int modem_register_notifier(struct notifier_block *nb)
{
	int ret = -ENODEV;
	unsigned long flags;

    if(vmdata){
	    raw_spin_lock_irqsave(&rslock, flags);
	    ret = raw_notifier_chain_register(&vmdata->ntf, nb);
	    raw_spin_unlock_irqrestore(&rslock, flags);
    }
	return ret;
}
EXPORT_SYMBOL(modem_register_notifier);

/*
 *  unregister a modem events change listener
 */
int modem_unregister_notifier(struct notifier_block *nb)
{
	int ret = -ENODEV;
	unsigned long flags;

    if(vmdata){
	    raw_spin_lock_irqsave(&rslock, flags);
	    ret = raw_notifier_chain_unregister(&vmdata->ntf, nb);
	    raw_spin_unlock_irqrestore(&rslock, flags);
    }
	return ret;
}
EXPORT_SYMBOL(modem_unregister_notifier);

static int first_irq_flag = 1;
static irqreturn_t modem_reset_indication_irq(int irq, void *data)
{
    if(GPIO_OEM_VALID(cbp_rst_ind_gpio )){
        printk("%s %d oem_gpio_get_value(GPIO_VIATEL_MDM_RST_IND)=%d \n",__func__,__LINE__,oem_gpio_get_value(cbp_rst_ind_gpio));
        if(oem_gpio_get_value(cbp_rst_ind_gpio)){
            if(first_irq_flag == 1){
                printk("%s %d first irq read rest_gpio is high,return. \n",__func__,__LINE__);
                return IRQ_HANDLED;
            }
            wake_lock_timeout(&vmdata->wlock, MDM_RST_LOCK_TIME * HZ);
            modem_notify_event(MDM_EVT_NOTIFY_RESET_ON);
            via_modem_state = MODEM_STATE_POWER;
            via_monitor_uevent_notify(MODEM_STATE_POWER);
        }else{
            if(first_irq_flag == 1){
                printk("%s %d set first_irq_flag=0. \n",__func__,__LINE__);
                first_irq_flag = 0;
            }
            modem_notify_event(MDM_EVT_NOTIFY_RESET_OFF);
        }
    }
    irq_set_irq_type(irq, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING);
    gpio_irq_cbp_rst_ind();
    oem_gpio_irq_unmask(cbp_rst_ind_gpio);
    return IRQ_HANDLED;
}



static int misc_modem_open(struct inode *inode, struct file *filp)
{
    int ret = -ENODEV;
    if(vmdata){
        filp->private_data = vmdata;
        atomic_inc(&vmdata->count);
        ret = 0;
    }

    return ret;
}

static long misc_modem_ioctl(struct file *file, unsigned int
		cmd, unsigned long arg)
{
    void __user *argp = (void __user *) arg;
    int flag,ret=-1;

    switch (cmd) {
        case VMDM_IOCTL_RESET:
            oem_reset_modem();
            break;
        case VMDM_IOCTL_POWER:
            if (copy_from_user(&flag, argp, sizeof(flag)))
                return -EFAULT;
            if (flag < 0 || flag > 1)
                return -EINVAL;
            if(flag){
                oem_power_on_modem();
            }else{
                oem_power_off_modem();
            }
            break;
	case VMDM_IOCTL_CRL:
		if (copy_from_user(&flag, argp, sizeof(flag)))
				return -EFAULT;
		if (flag < 0 || flag > 1)
			return -EINVAL;
		if(flag){
			ret=modem_on_off_ctrl_chan(1);
		}else{
			ret=modem_on_off_ctrl_chan(0);
		}
		break;
	case VMDM_IOCTL_DIE:
		oem_let_cbp_die();
		break;
	case VMDM_IOCTL_WAKE:
		if (copy_from_user(&flag, argp, sizeof(flag)))
			return -EFAULT;
		if (flag < 0 || flag > 1)
			return -EINVAL;
		if(flag){
            printk("hold on wakelock.\n");
			wake_lock(&vmdata->wlock);
		}else{
            printk("release wakelock.\n");
			wake_unlock(&vmdata->wlock);
		}
		break;
    default:
        break;
			
    }

    return 0;
}

static int misc_modem_release(struct inode *inode, struct file *filp)
{
    struct viatel_modem_data *d = (struct viatel_modem_data *)(filp->private_data);

    if(atomic_read(&vmdata->count) > 0){
        atomic_dec(&vmdata->count);
    }
    return fasync_helper(-1, filp, 0, &d->fasync);
}

static int misc_modem_fasync(int fd, struct file *filp, int on)
{
    struct viatel_modem_data *d = (struct viatel_modem_data *)(filp->private_data);

    return fasync_helper(fd, filp, on, &d->fasync);
}



static int modem_data_init(struct viatel_modem_data *d)
{
    int ret = 0;

    d->ntf_flags = 0;
    RAW_INIT_NOTIFIER_HEAD(&d->ntf);
    wake_lock_init(&d->wlock, WAKE_LOCK_SUSPEND, "cbp_rst");
    spin_lock_init( &d->lock );
    INIT_WORK(&d->work, modem_notify_task);
    INIT_WORK(&d->via_uevent_work, via_uevent_work_func);
    d->rst_ntf.notifier_call = modem_reset_notify_misc;
    atomic_set(&d->count, 0);
end:
    return ret;
}


static inline void modem_boot_change_state(int state)
{
    pr_info("modem_power: Power state change: %d -> %d\n", via_curr_power_state, state);
    via_curr_power_state = state;
}


static ssize_t modem_boot_get(struct device *dev, struct device_attribute *attr, char *buf)
{
    ssize_t ret = via_curr_power_state;
    return sprintf(buf, "%d\n", ret);
}

static ssize_t modem_boot_set(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    int state;

    dev_info(dev, "Power PHY set to %s\n", buf);

    if (sscanf(buf, "%d", &state) != 1)
        return -EINVAL;

    if (state == POWER_SET_ON) {
        modem_boot_change_state(state);
        oem_power_on_modem();
    } else if (state == POWER_SET_OFF) {
        modem_boot_change_state(state);
        via_modem_state = MODEM_STATE_OFF;
        via_monitor_uevent_notify(MODEM_STATE_OFF);
        oem_power_off_modem();
    } else if (state == POWER_SET_DEBUGON) {
        modem_boot_change_state(state);
        oem_power_on_modem();
    } else if (state == POWER_SET_DEBUGOFF) {
        modem_boot_change_state(state);
        oem_power_off_modem();
    } else {
        dev_err(dev, "Power PHY error state. %s\n", buf);
    }

    return count;
}

static ssize_t modem_state_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return ( snprintf(buf,PAGE_SIZE,"%s\n", via_state_str[via_modem_state]) );
}

static ssize_t modem_state_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    int modem_state;

    dev_info(dev, "set modem_state to  %s\n", buf);

    if (sscanf(buf, "%d", &modem_state) != 1)
        return -EINVAL;

    if (modem_state == MODEM_STATE_READY) {
        via_modem_state = MODEM_STATE_READY;
        via_monitor_uevent_notify(MODEM_STATE_READY);
    }else {
        dev_err(dev, "Power PHY error state. %s\n", buf);
    }

    return count;

}


ssize_t modem_sim_switch_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    int reset = 0;
    int ret = 0;
    if(GPIO_OEM_VALID(cbp_sim_switch_gpio))
    {
        reset = !!oem_gpio_get_value(cbp_sim_switch_gpio);
    }

    if(reset)
    {
        ret += sprintf(buf + ret, "sim_to_VIA\n");
    }
    else
    {
        ret += sprintf(buf + ret, "sim_to_Balong\n");
    }
    return ret;
}

ssize_t modem_sim_switch_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    if( !strncmp(buf, "1", strlen("1"))){
        oem_gpio_direction_output(cbp_sim_switch_gpio, 1);
        printk("Switch sim connection to VIA\n");
    }else if( !strncmp(buf, "0", strlen("0"))){
        oem_gpio_direction_output(cbp_sim_switch_gpio, 0);
        printk("Switch sim connection to Balong\n");
    }else{
        printk("Unknow command.\n");
    }
        return count;
}


static DEVICE_ATTR(state, S_IRUGO | S_IWUSR, modem_boot_get, modem_boot_set);
static DEVICE_ATTR(modem_state, S_IRUGO | S_IWUSR, modem_state_show, modem_state_store);
static DEVICE_ATTR(sim_switch, S_IRUGO | S_IWUSR, modem_sim_switch_show, modem_sim_switch_store);

static int modem_boot_probe(struct platform_device *dev)
{
    int ret = 0;

    vmdata = kzalloc(sizeof(struct viatel_modem_data), GFP_KERNEL);
    if(!vmdata)
    {
        ret = -ENOMEM;
        printk("No memory to alloc vmdata");
        goto err_create_vmdata;
    }

    vmdata->via_pdev = dev;
    vmdata->tail = 0;
    vmdata->head = 0;

    ret = modem_data_init(vmdata);
    if(ret < 0)
    {
        printk("Fail to init modem data\n");
        goto err_init_modem_data;
    }

    ret = device_create_file(&(dev->dev), &dev_attr_state);
    if(ret < 0)
    {
        dev_err(&dev->dev, "Failed to create sysfs entry state\n");
        goto err_create_sysfs_state;
    }

    ret = device_create_file(&(dev->dev), &dev_attr_modem_state);
    if(ret < 0)
    {
        dev_err(&dev->dev, "Failed to create sysfs entry modem_state\n");
        goto err_create_sysfs_modem_state;
    }

    ret = device_create_file(&(dev->dev), &dev_attr_sim_switch);
    if(ret < 0)
    {
        dev_err(&dev->dev, "Failed to create sysfs entry sim_switch\n");
        goto err_create_sysfs_sim_switch;
    }

    ret = oem_get_gpio_number(CBP_VIATEL_RST_IND_STR,&cbp_rst_ind_gpio);
    if (ret)
        goto err_get_gpio;
    pr_info("%s get CBP_VIATEL_RST_IND_STR gpio %d\n", __func__, cbp_rst_ind_gpio);

    if(GPIO_OEM_VALID(cbp_rst_ind_gpio)){
        ret = oem_gpio_request(cbp_rst_ind_gpio, "GPIO_VIATEL_MDM_RST_IND");
        oem_gpio_irq_mask(cbp_rst_ind_gpio);
        oem_gpio_direction_input_for_irq(cbp_rst_ind_gpio);
        oem_gpio_set_irq_type(cbp_rst_ind_gpio, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING);
        ret = oem_gpio_request_irq(cbp_rst_ind_gpio, modem_reset_indication_irq, \
                     IRQF_SHARED | IRQF_NO_SUSPEND | IRQF_TRIGGER_FALLING, \
                     "mdm_reset_ind", vmdata);
        oem_gpio_irq_unmask(cbp_rst_ind_gpio);

        if (ret < 0) {
            printk("fail to request mdm_rst_ind irq\n");
        }
        modem_register_notifier(&vmdata->rst_ntf);
    }

    ret = oem_get_gpio_number(CBP_VIATEL_PWR_EN_STR, &cbp_pwr_en_gpio);
    if (ret)
            goto err_get_gpio;
    pr_info(">>>> %s get gpio %d\n", __func__, cbp_pwr_en_gpio);
    if(GPIO_OEM_VALID(cbp_pwr_en_gpio)){
		ret = oem_gpio_request(cbp_pwr_en_gpio, "GPIO_VIATEL_MDM_PWR_EN");

	    if (0 > ret) {
	        printk("%s: gpio request GPIO_VIATEL_MDM_PWR_EN failed", __FUNCTION__);
	    }
		oem_gpio_direction_output(cbp_pwr_en_gpio, 0);
    }

    ret = oem_get_gpio_number(CBP_VIATEL_RST_STR, &cbp_rst_gpio);
    if (ret)
            goto err_get_gpio;
    pr_info(">>>> %s get gpio %d\n", __func__,cbp_rst_gpio);

    if(GPIO_OEM_VALID(cbp_rst_gpio)){
		ret = oem_gpio_request(cbp_rst_gpio, "GPIO_VIATEL_MDM_RST");
	    if (0 > ret) {
	        printk("%s: gpio request GPIO_VIATEL_MDM_RST failed", __FUNCTION__);
	    }
		oem_gpio_direction_output(cbp_rst_gpio, 0);
    }

    ret = oem_get_gpio_number(CBP_VIATEL_SIM_SWITCH_STR, &cbp_sim_switch_gpio);
    if(ret){
        printk("%s: Sim connection switch isn't supported on this board", __FUNCTION__);
    }else{
        printk("%s: Sim connection switch is supported on this board", __FUNCTION__);
    }
    pr_info(">>>> %s get gpio %d\n", __func__,cbp_sim_switch_gpio);
    if(GPIO_OEM_VALID(cbp_sim_switch_gpio)){
        ret = oem_gpio_request(cbp_sim_switch_gpio, "GPIO_VIATEL_MDM_SIM_SWITCH");
        if(0 > ret){
            printk("%s: gpio request GPIO_VIATEL_MDM_PWR_EN failed", __FUNCTION__);
        }
        oem_gpio_direction_output(cbp_sim_switch_gpio, 0);
    }

    /*add gpio function to make cp ramdump*/
    ret = oem_get_gpio_number(CBP_VIATEL_GPIO_BACKUP_STR, &cbp_backup_gpio);
    if (ret)
            goto err_get_gpio;

    ret = oem_gpio_request(cbp_backup_gpio, "gpio_cbp_crash");
    if(0 > ret){
        printk("%s: gpio request GPIO_6_0 failed", __FUNCTION__);
    }

    oem_gpio_direction_output(cbp_backup_gpio, 0);

    return 0;

err_get_gpio:
    device_remove_file(&(dev->dev), &dev_attr_sim_switch);
err_create_sysfs_sim_switch:
    device_remove_file(&(dev->dev), &dev_attr_modem_state);
err_create_sysfs_modem_state:
    device_remove_file(&(dev->dev), &dev_attr_state);
err_create_sysfs_state:
err_init_modem_data:
    kfree(vmdata);
    vmdata = NULL;
err_create_vmdata:

    return ret;

}

static int modem_boot_remove(struct platform_device *dev)
{

    if(GPIO_OEM_VALID(cbp_rst_ind_gpio)){
		oem_gpio_free(cbp_rst_ind_gpio);
		modem_unregister_notifier(&vmdata->rst_ntf);
    }

    if(vmdata)
        wake_lock_destroy(&vmdata->wlock);
    return 0;
}

static void modem_boot_shutdown(struct platform_device *dev)
{
    oem_power_off_modem();
}

static struct platform_driver modem_boot_driver = {
	.probe = modem_boot_probe,
	.remove = modem_boot_remove,
	.shutdown = modem_boot_shutdown,
	.driver.name = MODEM_DEVICE_BOOT(MODEM_VIACBP82D),
};

static struct platform_device platform_modem_device = {
	.name = MODEM_DEVICE_BOOT(MODEM_VIACBP82D),
};

static const struct file_operations misc_modem_fops = {
	.owner = THIS_MODULE,
	.open = misc_modem_open,
	.unlocked_ioctl = misc_modem_ioctl,
	.release = misc_modem_release,
	.fasync	= misc_modem_fasync,
};

static struct miscdevice misc_modem_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "vmodem",
	.fops = &misc_modem_fops,
};

static struct wake_lock cbp_wk_no_sleep;
static int __init modem_boot_init(void)
{
    int ret = 0;

    int support_cbp = true;
    int cannot_sleep = 0;

    ret = get_hw_config_int("modem/viacbp82d", &support_cbp, NULL);

    if ((!ret) || (0 == support_cbp)) {
        pr_info("%s has no cbp support\n", __func__);
        return -ENXIO;
    }

    ret = get_hw_config_int("modem/cbp_cannot_sleep", &cannot_sleep, NULL);
    if ((ret) && (1 == cannot_sleep)) {
	pr_info("%s cbp cannot sleep\n", __func__);
	wake_lock_init(&cbp_wk_no_sleep, WAKE_LOCK_SUSPEND, "cbp_wk_no_sleep");
	wake_lock(&cbp_wk_no_sleep);
    }


    ret = platform_device_register(&platform_modem_device);
    if (ret)
    {
        printk("platform_device_register failed\n");
        goto err_platform_device_register;	
    }

    ret = platform_driver_register(&modem_boot_driver);
    if(ret < 0)
    {
        printk("platform_driver_register failed\n");
        goto err_platform_driver_register;
    }

    ret = misc_register(&misc_modem_device);
    if(ret < 0)
    {
        printk("misc regiser via modem failed\n");
        goto err_misc_device_register;
    }

    return ret;

err_misc_device_register:
	platform_driver_unregister(&modem_boot_driver);
err_platform_driver_register:
	platform_device_unregister(&platform_modem_device);
err_platform_device_register:

    return ret;
}

static void __exit modem_boot_exit(void)
{
    platform_driver_unregister(&modem_boot_driver);
}
late_initcall_sync(modem_boot_init);
module_exit(modem_boot_exit);

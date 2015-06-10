/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : hi6620_pcm.c
  �� �� ��   : ����
  ��    ��   : ʯ���� s00212991
  ��������   : 2012��7��31��
  ����޸�   :
  ��������   : xxxx
  �����б�   :
              hi6620_exit
              hi6620_init
              hi6620_intr_handle_cp
              hi6620_intr_handle_pb
              hi6620_mailbox_send_data
              hi6620_mb_intr_handle
              hi6620_notify_isr_register
              hi6620_notify_pcm_close
              hi6620_notify_pcm_hw_free
              hi6620_notify_pcm_hw_params
              hi6620_notify_pcm_open
              hi6620_notify_pcm_prepare
              hi6620_notify_pcm_set_buf
              hi6620_notify_pcm_trigger
              hi6620_notify_recv_isr
              hi6620_pcm_close
              hi6620_pcm_free
              hi6620_pcm_hifi_close
              hi6620_pcm_hifi_hw_free
              hi6620_pcm_hifi_hw_params
              hi6620_pcm_hifi_open
              hi6620_pcm_hifi_pointer
              hi6620_pcm_hifi_prepare
              hi6620_pcm_hifi_trigger
              hi6620_pcm_hw_free
              hi6620_pcm_hw_params
              hi6620_pcm_new
              hi6620_pcm_open
              hi6620_pcm_pointer
              hi6620_pcm_prepare
              hi6620_pcm_trigger
              hi6620_platform_probe
              hi6620_platform_remove
              status_read_proc_hstatus
              status_write_proc_hstatus
  �޸���ʷ   :
  1.��    ��   : 2012��7��31��
    ��    ��   : ʯ���� s00212991
    �޸�����   : �����ļ�

******************************************************************************/

/*
the 2 MACRO should be used seperately
CONFIG_SND_TEST_AUDIO_PCM_LOOP : for ST, simu data send of mailbox
__DRV_AUDIO_MAILBOX_WORK__   : leave mailbox's work to workqueue
*/
#ifndef CONFIG_SND_TEST_AUDIO_PCM_LOOP
#define __DRV_AUDIO_MAILBOX_WORK__
#endif

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include <linux/interrupt.h>
#include <linux/time.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/kernel.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <bsp_mailbox.h>

/*
#include <hifidrvinterface.h>
*/
#include "hi6620_pcm.h"
#include "hi6620_log.h"

//systrace
#include <trace/trace_kernel.h>

/*****************************************************************************
  2 �궨��
*****************************************************************************/
/*
* PLAYBACK SUPPORT FORMATS
* BITS : 8/16/24  18/20
* LITTLE_ENDIAN / BIG_ENDIAN
* MONO / STEREO
* UNSIGNED / SIGNED
*/
#define HI6620_PB_FORMATS  (SNDRV_PCM_FMTBIT_S8 | \
                            SNDRV_PCM_FMTBIT_U8 | \
                            SNDRV_PCM_FMTBIT_S16_LE | \
                            SNDRV_PCM_FMTBIT_S16_BE | \
                            SNDRV_PCM_FMTBIT_U16_LE | \
                            SNDRV_PCM_FMTBIT_U16_BE | \
                            SNDRV_PCM_FMTBIT_S24_LE | \
                            SNDRV_PCM_FMTBIT_S24_BE | \
                            SNDRV_PCM_FMTBIT_U24_LE | \
                            SNDRV_PCM_FMTBIT_U24_BE)

/*
* PLAYBACK SUPPORT RATES
* 8/11.025/16/22.05/32/44.1/48/88.2/96kHz
*/
#define HI6620_PB_RATES    (SNDRV_PCM_RATE_8000_48000 | \
                            SNDRV_PCM_RATE_88200 | \
                            SNDRV_PCM_RATE_96000)

#define HI6620_PB_MIN_CHANNELS  ( 1 )
#define HI6620_PB_MAX_CHANNELS  ( 2 )
/* Assume the FIFO size */
#define HI6620_PB_FIFO_SIZE     ( 16 )

/* CAPTURE SUPPORT FORMATS : SIGNED 16/24bit */
#define HI6620_CP_FORMATS  ( SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S24_LE)

/* CAPTURE SUPPORT RATES : 48/96kHz */
#define HI6620_CP_RATES    ( SNDRV_PCM_RATE_48000 | SNDRV_PCM_RATE_96000 )

#define HI6620_CP_MIN_CHANNELS  ( 2 )
#define HI6620_CP_MAX_CHANNELS  ( 2 )
/* Assume the FIFO size */
#define HI6620_CP_FIFO_SIZE     ( 32 )
#define HI6620_MODEM_RATES      ( SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_16000 )
#define HI6620_BT_RATES         ( SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_16000 )
#define HI6620_FM_RATES         ( SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_16000 )

#define HI6620_MAX_BUFFER_SIZE  ( 128 * 1024 )    /* 0x20000 */
#define HI6620_BUFFER_SIZE_MM   ( 32 * 1024 )
#define HI6620_MIN_BUFFER_SIZE  ( 32 )
#define HI6620_MAX_PERIODS      ( 32 )
#define HI6620_MIN_PERIODS      ( 2 )

#ifndef OK
#define OK              0
#endif
#ifndef ERROR
#define ERROR           -1
#endif

#undef NULL
#define NULL ((void *)0)


#define HI6620_PCM_INCLUDE_SYMBOLS
#ifdef HI6620_PCM_INCLUDE_SYMBOLS
#define STATIC
#else
#define STATIC  static
#endif

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
static const unsigned int freq[] = {
    8000,   11025,  12000,  16000,
    22050,  24000,  32000,  44100,
    48000,  88200,  96000,  176400,
    192000,
};

static u64 hi6620_pcm_dmamask           = (u64)(0xffffffff);

struct proc_dir_entry *audio_pcm_dir    = NULL;

static struct snd_soc_dai_driver hi6620_dai[] =
{
    {
        .name = "hi6620-mm",
        .playback = {
            .stream_name  = "hi6620-mm Playback",
            .channels_min = HI6620_PB_MIN_CHANNELS,
            .channels_max = HI6620_PB_MAX_CHANNELS,
            .rates        = HI6620_PB_RATES,
            .formats      = HI6620_PB_FORMATS},
        .capture = {
            .stream_name  = "hi6620-mm Capture",
            .channels_min = HI6620_CP_MIN_CHANNELS,
            .channels_max = HI6620_CP_MAX_CHANNELS,
            .rates        = HI6620_CP_RATES,
            .formats      = HI6620_CP_FORMATS
        },
    },
    {
        .name = "hi6620-modem",
        .playback = {
            .stream_name  = "hi6620-modem Playback",
            .channels_min = HI6620_PB_MIN_CHANNELS,
            .channels_max = HI6620_PB_MAX_CHANNELS,
            .rates        = HI6620_MODEM_RATES,
            .formats      = HI6620_PB_FORMATS
        },
    },
    {
        .name = "hi6620-fm",
        .playback = {
            .stream_name  = "hi6620-fm Playback",
            .channels_min = HI6620_PB_MIN_CHANNELS,
            .channels_max = HI6620_PB_MAX_CHANNELS,
            .rates        = HI6620_FM_RATES,
            .formats      = HI6620_PB_FORMATS
        },
    },
    {
        .name = "hi6620-bt",
        .playback = {
            .stream_name  = "hi6620-bt Playback",
            .channels_min = HI6620_PB_MIN_CHANNELS,
            .channels_max = HI6620_PB_MAX_CHANNELS,
            .rates        = HI6620_BT_RATES,
            .formats      = HI6620_PB_FORMATS},
    },
};

/* define the capability of playback channel */
static const struct snd_pcm_hardware hi6620_hardware_playback =
{
    .info             = SNDRV_PCM_INFO_INTERLEAVED
                      | SNDRV_PCM_INFO_NONINTERLEAVED
                      | SNDRV_PCM_INFO_MMAP
                      | SNDRV_PCM_INFO_MMAP_VALID
                      | SNDRV_PCM_INFO_PAUSE,
    .formats          = SNDRV_PCM_FMTBIT_S16_LE,
    .channels_min     = HI6620_PB_MIN_CHANNELS,
    .channels_max     = HI6620_PB_MAX_CHANNELS,
    .buffer_bytes_max = HI6620_MAX_BUFFER_SIZE,
    .period_bytes_min = HI6620_MIN_BUFFER_SIZE,
    .period_bytes_max = HI6620_MAX_BUFFER_SIZE,
    .periods_min      = HI6620_MIN_PERIODS,
    .periods_max      = HI6620_MAX_PERIODS,
    .fifo_size        = HI6620_PB_FIFO_SIZE,
};

/* define the capability of capture channel */
static const struct snd_pcm_hardware hi6620_hardware_capture =
{
    .info             = SNDRV_PCM_INFO_INTERLEAVED,
    .formats          = SNDRV_PCM_FMTBIT_S16_LE,
    .rates            = SNDRV_PCM_RATE_48000,
    .channels_min     = HI6620_CP_MIN_CHANNELS,
    .channels_max     = HI6620_CP_MAX_CHANNELS,
    .buffer_bytes_max = HI6620_MAX_BUFFER_SIZE,
    .period_bytes_min = HI6620_MIN_BUFFER_SIZE,
    .period_bytes_max = HI6620_MAX_BUFFER_SIZE,
    .periods_min      = HI6620_MIN_PERIODS,
    .periods_max      = HI6620_MAX_PERIODS,
    .fifo_size        = HI6620_CP_FIFO_SIZE,
};

/* define the capability of playback channel for Modem */
static const struct snd_pcm_hardware hi6620_hardware_modem_playback =
{
    .info             = SNDRV_PCM_INFO_INTERLEAVED
                      | SNDRV_PCM_INFO_NONINTERLEAVED
                      | SNDRV_PCM_INFO_BLOCK_TRANSFER
                      | SNDRV_PCM_INFO_PAUSE,
    .formats          = SNDRV_PCM_FMTBIT_S16_LE,
    .channels_min     = HI6620_PB_MIN_CHANNELS,
    .channels_max     = HI6620_PB_MAX_CHANNELS,
    .buffer_bytes_max = HI6620_MAX_BUFFER_SIZE,
    .period_bytes_min = HI6620_MIN_BUFFER_SIZE,
    .period_bytes_max = HI6620_MAX_BUFFER_SIZE,
    .periods_min      = HI6620_MIN_PERIODS,
    .periods_max      = HI6620_MAX_PERIODS,
    .fifo_size        = HI6620_PB_FIFO_SIZE,
};

#ifdef CONFIG_SND_TEST_AUDIO_PCM_LOOP
struct hi6620_simu_pcm_data hi6620_simu_pcm;
#endif
#ifdef __DRV_AUDIO_MAILBOX_WORK__
/* workqueue for Playback and Capture */
struct hi6620_pcm_mailbox_wq hi6620_pcm_mailbox_workqueue;
#endif

static u32 pcm_cp_status_open = 0;
static u32 pcm_pb_status_open = 0;

DEFINE_SEMAPHORE(g_pcm_cp_open_sem);
DEFINE_SEMAPHORE(g_pcm_pb_open_sem);

/*****************************************************************************
  3 ��������
*****************************************************************************/
STATIC int hi6620_notify_pcm_set_buf( struct snd_pcm_substream *substream );
#ifdef CONFIG_SND_TEST_AUDIO_PCM_LOOP
STATIC irq_rt_t hi6620_notify_recv_isr( void *usr_para, void *mail_handle,  unsigned int mail_len );
#endif
#ifdef __DRV_AUDIO_MAILBOX_WORK__
STATIC irq_rt_t hi6620_mb_intr_handle(unsigned short pcm_mode,
                                        struct snd_pcm_substream *substream);
#endif

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


#ifdef CONFIG_SND_TEST_AUDIO_PCM_LOOP
void simu_pcm_work_func(struct work_struct *work)
{
    struct hi6620_simu_pcm_data *priv =
            container_of(work, struct hi6620_simu_pcm_data, simu_pcm_delay_work.work);
    struct hifi_chn_pcm_period_elapsed pk_data;

    logi("simu_pcm_work_func:\r\n");
    logi("      msg_type = %x\r\n", priv->msg_type);
    logi("      pcm_mode = %d\r\n", priv->pcm_mode);
    logi("      data_addr = 0x%x\r\n", priv->data_addr);
    logi("      data_len = %d\r\n", priv->data_len);
    logi("      substream = 0x%x\r\n", priv->substream);

    /* simu data move and make response */
    memset(&pk_data, 0, sizeof(struct hifi_chn_pcm_period_elapsed));
    pk_data.msg_type = HI_CHN_MSG_PCM_PERIOD_ELAPSED;
    pk_data.pcm_mode = priv->pcm_mode;
    pk_data.substream = priv->substream;

    hi6620_notify_recv_isr(0, &pk_data, sizeof(struct hifi_chn_pcm_period_elapsed));

}
#endif

#ifdef __DRV_AUDIO_MAILBOX_WORK__
void pcm_mailbox_work_func(struct work_struct *work)
{
    struct hi6620_pcm_mailbox_data *priv =
            container_of(work, struct hi6620_pcm_mailbox_data, pcm_mailbox_delay_work.work);
    int ret                     = 0;
    unsigned short msg_type         = 0;
    unsigned short pcm_mode         = 0;
    struct snd_pcm_substream * substream   = NULL;
    struct hi6620_runtime_data *prtd        = NULL;

    /*��ȡ��Ϣ����*/
    msg_type    = priv->msg_type;
    pcm_mode    = priv->pcm_mode;
    substream   = priv->substream;
    logd("substream : 0x%x\n", substream);
    
    if (NULL == substream)
    {
        loge("End, substream == NULL\n");
        return ;
    }
    if (NULL == substream->runtime)
    {
        loge("End, substream->runtime == NULL\n");
        return ;
    }

    prtd    = (struct hi6620_runtime_data *)substream->runtime->private_data;

    if (NULL == prtd){
        loge("prtd is null \n");
        return;
    }

    switch( msg_type )
    {
    case HI_CHN_MSG_PCM_PERIOD_ELAPSED:
        ret = hi6620_mb_intr_handle( pcm_mode, substream );
        if( ret == IRQ_NH )
        {
            loge("ret : %d\n", ret);
            return;
        }
        break;
    case HI_CHN_MSG_PCM_PERIOD_STOP:
        if (STATUS_STOPPING == prtd->status){
            prtd->status = STATUS_STOP;
            logi("stop now !\n");
        }
        break;
    default:
        ret = IRQ_NH_TYPE;
        /*������Ϣ���������Ͳ�Ӧ����*/
        loge("msg_type 0x%x\n", msg_type);
        break;
    }

    return;

}

#endif

/*****************************************************************************
 �� �� ��  : hi6620_intr_handle_pb
 ��������  : PLAYBACK�����֧, �˼�ͨ��һ�����ݴ�����ɺ�Ĵ���
 �������  : struct snd_pcm_substream *substream : �������ݴ����substream
 �������  : ��
 �� �� ֵ  : STATIC irq_rt_t
 ���ú���  : hi6620_mb_intr_handle()
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��31��
    ��    ��   : ʯ���� s00212991
    �޸�����   : �����ɺ���

*****************************************************************************/
STATIC irq_rt_t hi6620_intr_handle_pb(struct snd_pcm_substream *substream)
{
    struct hi6620_runtime_data *prtd        = NULL;
    unsigned int rt_period_size             = 0;
    unsigned int num_period                 = 0;
    snd_pcm_uframes_t avail                 = 0;
    int ret                                 = OK;

    logd("Begin\r\n");

    if (NULL == substream)
    {
        loge("End, substream == NULL\n");
        return IRQ_HDD_PTR;
    }
    if (NULL == substream->runtime)
    {
        loge("End, substream->runtime == NULL\n");
        return IRQ_HDD_PTR;
    }

    prtd    = (struct hi6620_runtime_data *)substream->runtime->private_data;
    rt_period_size  = substream->runtime->period_size;
    num_period      = substream->runtime->periods;

    if (NULL == prtd)
    {
        loge("End, prtd == NULL\n");
        return IRQ_HDD_PTR;
    }

    /* dmas��¼dma�ж�״̬����HIFIά����AP��ά��buffer״̬ */

    spin_lock(&prtd->lock);
    ++prtd->period_cur;
    prtd->period_cur = (prtd->period_cur) % num_period;
    spin_unlock(&prtd->lock);

    snd_pcm_period_elapsed(substream);

    if (STATUS_RUNNING != prtd->status)
    {
        logd("End, dma stopped\n");
        return IRQ_HDD_STATUS;
    }

    avail = (snd_pcm_uframes_t)snd_pcm_playback_hw_avail(substream->runtime);
    if(avail < rt_period_size)
    {
        logd("End, avail(%d)< rt_period_size(%d)\n", avail, rt_period_size);
        return IRQ_HDD_SIZE;
    }
    else
    {
        /*
        DMA���˽��������жϣ���������ݿɰ�ʱ��ʹ���µ�DMA����
         MailBox֪ͨHIFI������һ�ε�DMA����
        */
        ret = hi6620_notify_pcm_set_buf( substream );
        if( ret < 0 )
        {
            loge("End, hi6620_notify_pcm_set_buf(ret=%d)\n", ret);
            return IRQ_HDD_ERROR;
        }

        spin_lock(&prtd->lock);
        prtd->period_next = (prtd->period_next + 1) % num_period;
        spin_unlock(&prtd->lock);
    }

    logd("End\r\n");
    return IRQ_HDD;
}

/*****************************************************************************
 �� �� ��  : hi6620_intr_handle_cp
 ��������  : CAPTURE�����֧, �˼�ͨ��һ�����ݴ�����ɺ�Ĵ���
 �������  : struct snd_pcm_substream *substream
 �������  : ��
 �� �� ֵ  : STATIC irq_rt_t
 ���ú���  : hi6620_mb_intr_handle()
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��31��
    ��    ��   : ʯ���� s00212991
    �޸�����   : �����ɺ���

*****************************************************************************/
STATIC irq_rt_t hi6620_intr_handle_cp(struct snd_pcm_substream *substream)
{
    struct hi6620_runtime_data *prtd        = NULL;
    unsigned int rt_period_size             = 0;
    unsigned int num_period                 = 0;
    snd_pcm_uframes_t avail                 = 0;
    int ret                                 = OK;

    logd("Begin\r\n");

    if (NULL == substream)
    {
        loge("End, substream == NULL\n");
        return IRQ_HDD_PTR;
    }
    if (NULL == substream->runtime)
    {
        loge("End, substream->runtime == NULL\n");
        return IRQ_HDD_PTR;
    }

    prtd    = (struct hi6620_runtime_data *)substream->runtime->private_data;
    rt_period_size  = substream->runtime->period_size;
    num_period      = substream->runtime->periods;

    if (NULL == prtd)
    {
        loge("End, prtd == NULL\n");
        return IRQ_HDD_PTR;
    }

    spin_lock(&prtd->lock);
    ++prtd->period_cur;
    prtd->period_cur = (prtd->period_cur) % num_period;
    spin_unlock(&prtd->lock);

    if (STATUS_RUNNING != prtd->status)
    {
        logd("dma stopped\n");
        return IRQ_HDD_STATUS;
    }

    snd_pcm_period_elapsed(substream);

    avail = (snd_pcm_uframes_t)snd_pcm_capture_hw_avail(substream->runtime);
    if(avail < rt_period_size)
    {
        logd("avail(%d)< rt_period_size(%d)\n", avail, rt_period_size);
        return IRQ_HDD_SIZE;
    }
    else
    {
        /*
        DMA���˽��������жϣ���������ݿɰ�ʱ��ʹ���µ�DMA����
         MailBox֪ͨHIFI������һ�ε�DMA����
        */
        ret = hi6620_notify_pcm_set_buf( substream );
        if( ret < 0 )
        {
            loge("hi6620_notify_pcm_set_buf(%d)\n", ret);
            return IRQ_HDD_ERROR;
        }

        spin_lock(&prtd->lock);
        prtd->period_next = (prtd->period_next + 1) % num_period;
        spin_unlock(&prtd->lock);
    }

    logd("End\r\n");
    return IRQ_HDD;
}

/*****************************************************************************
 �� �� ��  : hi6620_mb_intr_handle
 ��������  : ����˼�ͨ��ͨ�ô�����
 �������  : unsigned short pcm_mode        : PLAYBACK or CAPTURE
             snd_pcm_substream *substream   : substream
 �������  : ��
 �� �� ֵ  : STATIC irq_rt_t
 ���ú���  : hi6620_notify_recv_isr()
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��31��
    ��    ��   : ʯ���� s00212991
    �޸�����   : �����ɺ���

*****************************************************************************/
STATIC irq_rt_t hi6620_mb_intr_handle(unsigned short pcm_mode,
                                        struct snd_pcm_substream *substream)
{
    irq_rt_t ret = IRQ_NH;

    switch( pcm_mode )
    {
    case SNDRV_PCM_STREAM_PLAYBACK:
        if ( NULL != substream)
        {
            /* SEM used to protect close while doing _intr_handle_pb */
            ret = down_interruptible(&g_pcm_pb_open_sem);
            if (0 == pcm_pb_status_open)
            {
                logd("pcm playback closed\n");
                up(&g_pcm_pb_open_sem);
                return IRQ_HDD;
            }

            ret = hi6620_intr_handle_pb(substream);
            up(&g_pcm_pb_open_sem);
        }
        else
        {
            loge("PB,substream is NULL\n");
            ret = IRQ_HDD_PTRS;
        }
        break;
    case SNDRV_PCM_STREAM_CAPTURE:
        if ( NULL != substream)
        {
            /* SEM used to protect close while doing _intr_handle_cp */
            ret = down_interruptible(&g_pcm_cp_open_sem);

            if (0 == pcm_cp_status_open)
            {
                logd("pcm capture closed\n");
                up(&g_pcm_cp_open_sem);
                return IRQ_HDD;
            }
            ret = hi6620_intr_handle_cp(substream);
            up(&g_pcm_cp_open_sem);
        }
        else
        {
            loge("CP,substream is NULL\n");
            ret = IRQ_HDD_PTRS;
        }
        break;
    default:
        ret = IRQ_NH_MODE;
        loge("PCM Mode error(%d)\n", pcm_mode);
        break;
    }

    return ret;
}

/*****************************************************************************
 �� �� ��  : hi6620_mailbox_send_data
 ��������  : ����˼�ͨ�����ݷ���
 �������  : void *pmsg_body        : Ҫ���͵����ݰ�
             unsigned int msg_len   : ���ݰ�����
 �������  : ��
 �� �� ֵ  : STATIC int
 ���ú���  : hi6620_notify_pcm_*()
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��31��
    ��    ��   : ʯ���� s00212991
    �޸�����   : �����ɺ���

*****************************************************************************/
STATIC int hi6620_mailbox_send_data( void *pmsg_body, unsigned int msg_len,
                                                    unsigned int msg_priority )
{
    int ret                     = 0;

#ifdef CONFIG_SND_TEST_AUDIO_PCM_LOOP
    /* simu */
    switch (((struct hifi_chn_pcm_trigger *)pmsg_body)->msg_type) {
    case HI_CHN_MSG_PCM_TRIGGER :
        if( (SNDRV_PCM_TRIGGER_START == ((struct hifi_chn_pcm_trigger *)pmsg_body)->tg_cmd) ||
                (SNDRV_PCM_TRIGGER_RESUME == ((struct hifi_chn_pcm_trigger *)pmsg_body)->tg_cmd) ||
                (SNDRV_PCM_TRIGGER_PAUSE_RELEASE == ((struct hifi_chn_pcm_trigger *)pmsg_body)->tg_cmd) )
        {
            hi6620_simu_pcm.msg_type = ((struct hifi_chn_pcm_trigger *)pmsg_body)->msg_type;
            hi6620_simu_pcm.pcm_mode = ((struct hifi_chn_pcm_trigger *)pmsg_body)->pcm_mode;
            hi6620_simu_pcm.substream = ((struct hifi_chn_pcm_trigger *)pmsg_body)->substream;
            hi6620_simu_pcm.data_addr = ((struct hifi_chn_pcm_trigger *)pmsg_body)->data_addr;
            hi6620_simu_pcm.data_len = ((struct hifi_chn_pcm_trigger *)pmsg_body)->data_len;
            queue_delayed_work(hi6620_simu_pcm.simu_pcm_delay_wq,
                    &hi6620_simu_pcm.simu_pcm_delay_work,
                    msecs_to_jiffies(20/*0*/));

        }
        break;
    case HI_CHN_MSG_PCM_SET_BUF :
        hi6620_simu_pcm.msg_type = ((struct hifi_channel_set_buffer *)pmsg_body)->msg_type;
        hi6620_simu_pcm.pcm_mode = ((struct hifi_channel_set_buffer *)pmsg_body)->pcm_mode;
        hi6620_simu_pcm.data_addr = ((struct hifi_channel_set_buffer *)pmsg_body)->data_addr;
        hi6620_simu_pcm.data_len = ((struct hifi_channel_set_buffer *)pmsg_body)->data_len;
        queue_delayed_work(hi6620_simu_pcm.simu_pcm_delay_wq,
                &hi6620_simu_pcm.simu_pcm_delay_work,
                msecs_to_jiffies(0/*20*/));
        break;
    default:
        logi("hi6620_mailbox_send_data MSG_TYPE(0x%x)\r\n", ((struct hifi_chn_pcm_trigger *)pmsg_body)->msg_type);
        break;
    }

#else

    ret = mailbox_send_msg(MAILBOX_MAILCODE_ACPU_TO_HIFI_AUDIO, pmsg_body, msg_len);
    if( 0 != ret )
    {
        loge("ret=%d\n", ret);
    }
#endif
    return ret;
}

/*****************************************************************************
 �� �� ��  : hi6620_notify_pcm_open
 ��������  : �˼�ͨ�Ÿ�֪HIFI PCM OPEN
 �������  : hifi_chn_pcm_open *psrc_data : Ҫ������������
 �������  : ��
 �� �� ֵ  : STATIC int, 0 for Success; Others for error
 ���ú���  : hi6620_pcm_hifi_open()
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��31��
    ��    ��   : ʯ���� s00212991
    �޸�����   : �����ɺ���

*****************************************************************************/
STATIC int hi6620_notify_pcm_open( unsigned short pcm_mode )
{
    struct hifi_chn_pcm_open msg_body   = { 0 };
    int ret                             = OK;

    if ( (SNDRV_PCM_STREAM_PLAYBACK != pcm_mode) &&
            (SNDRV_PCM_STREAM_CAPTURE != pcm_mode) )
    {
        loge("pcm_mode=%d\n", pcm_mode);
        return -EINVAL;
    }

    msg_body.msg_type    = (unsigned short)HI_CHN_MSG_PCM_OPEN;
    msg_body.pcm_mode    = pcm_mode;

    /* mail-box send */
    ret = hi6620_mailbox_send_data( &msg_body, sizeof(struct hifi_chn_pcm_open), 0 );
    if( OK != ret )
    {
        ret = -EBUSY;
    }
    logi("mailbox ret=%d\r\n", ret);

    return ret;
}


/*****************************************************************************
 �� �� ��  : hi6620_notify_pcm_close
 ��������  : �˼�ͨ�Ÿ�֪HIFI PCM CLOSE
 �������  : hifi_chn_pcm_close *psrc_data : Ҫ������������
 �������  : ��
 �� �� ֵ  : STATIC int, 0 for Success; Others for error
 ���ú���  : hi6620_pcm_hifi_close()
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��31��
    ��    ��   : ʯ���� s00212991
    �޸�����   : �����ɺ���

*****************************************************************************/
STATIC int hi6620_notify_pcm_close( unsigned short pcm_mode )
{
    struct hifi_chn_pcm_close msg_body  = { 0 };
    int ret                             = OK;

    if ( (SNDRV_PCM_STREAM_PLAYBACK != pcm_mode) &&
            (SNDRV_PCM_STREAM_CAPTURE != pcm_mode) )
    {
        loge("pcm_mode=%d\n", pcm_mode);
        return -EINVAL;
    }

    msg_body.msg_type    = (unsigned short)HI_CHN_MSG_PCM_CLOSE;
    msg_body.pcm_mode    = pcm_mode;

    /* mail-box send */
    ret = hi6620_mailbox_send_data( &msg_body, sizeof(struct hifi_chn_pcm_close), 0 );
    if( OK != ret )
    {
        ret = -EBUSY;
    }
    logi("mailbox ret=%d\r\n", ret);

    return ret;
}

/*****************************************************************************
 �� �� ��  : hi6620_notify_pcm_hw_params
 ��������  : �˼�ͨ�Ÿ�֪HIFI PCM HW PARAMS
 �������  : unsigned short pcm_mode : PLAYBACK or CAPTURE
             struct snd_pcm_hw_params *params : ���ò���
 �������  : ��
 �� �� ֵ  : STATIC int, 0 for Success; Negative for error
 ���ú���  : hi6620_pcm_hifi_hw_params()
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��31��
    ��    ��   : ʯ���� s00212991
    �޸�����   : �����ɺ���

*****************************************************************************/
STATIC int hi6620_notify_pcm_hw_params( unsigned short pcm_mode,
                                             struct snd_pcm_hw_params *params )
{
    struct hifi_chn_pcm_hw_params msg_body  = { 0 };
    unsigned int params_value               = 0;
    unsigned int infreq_index               = 0;
    int ret                                 = OK;

    if ( (SNDRV_PCM_STREAM_PLAYBACK != pcm_mode) &&
            (SNDRV_PCM_STREAM_CAPTURE != pcm_mode) )
    {
        loge("pcm_mode=%d\n", pcm_mode);
        return -EINVAL;
    }

    msg_body.msg_type = (unsigned short)HI_CHN_MSG_PCM_HW_PARAMS;
    msg_body.pcm_mode = pcm_mode;

    /* CHECK SUPPORT CHANNELS : mono or stereo */
    params_value = params_channels(params);
    if ( (2 == params_value) || (1 == params_value) )
    {
        msg_body.channel_num = params_value;
    }
    else
    {
        loge("DAC not support %d channels\n", params_value);
        return -EINVAL;
    }

    /* CHECK SUPPORT RATE */
    params_value = params_rate(params);
    logd("set rate = %d \n", params_value);
    for (infreq_index = 0; infreq_index < ARRAY_SIZE(freq); infreq_index++)
    {
        if(params_value == freq[infreq_index])
            break;
    }
    if ( ARRAY_SIZE(freq) <= infreq_index )
    {
        loge("set rate = %d \n", params_value);
        return -EINVAL;
    }
    msg_body.sample_rate = params_value;

    /* PLAYBACK */
    if ( SNDRV_PCM_STREAM_PLAYBACK == pcm_mode )
    {
        params_value = (unsigned int)params_format(params);
        /* check formats */
        if( (SNDRV_PCM_FORMAT_S16_BE == params_value) ||
                (SNDRV_PCM_FORMAT_S16_LE == params_value) )
        {
            msg_body.format = params_value;
        }
        else
        {
            loge("format err : %d, not support\n", params_value);
            return -EINVAL;
        }
    }
    else
    /* CAPTURE */
    {
        params_value = (unsigned int)params_format(params);
        /* check formats */
        if ( params_value > SNDRV_PCM_FORMAT_LAST )
        {
            loge("format err2 : %d, not support\n", params_value);
            return -EINVAL;
        }
        msg_body.format = params_value;
    }

    logi("%s:\r\n", __func__);
    logi("      msg_type = 0x%x\r\n", msg_body.msg_type);
    logi("      pcm_mode = %d\r\n", msg_body.pcm_mode);
    logi("      channel_num = %d\r\n", msg_body.channel_num);
    logi("      sample_rate = %d\r\n", msg_body.sample_rate);
    logi("      format = %d\r\n", msg_body.format);

    /* mail-box send */
    ret = hi6620_mailbox_send_data( &msg_body, sizeof(struct hifi_chn_pcm_hw_params), 0 );
    if( OK != ret )
    {
        ret = -EBUSY;
    }

    return ret;
}

/*****************************************************************************
 �� �� ��  : hi6620_notify_pcm_trigger
 ��������  : �˼�ͨ�Ÿ�֪HIFI PCM TRIGGER
 �������  : hifi_chn_pcm_trigger *psrc_data : Ҫ������������
 �������  : ��
 �� �� ֵ  : STATIC int, 0 for Success; Others for error
 ���ú���  : hi6620_pcm_hifi_trigger()
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��31��
    ��    ��   : ʯ���� s00212991
    �޸�����   : �����ɺ���

*****************************************************************************/
STATIC int hi6620_notify_pcm_trigger( int cmd,
                                        struct snd_pcm_substream *substream )
{
    struct hi6620_runtime_data *prtd        =
                (struct hi6620_runtime_data *)substream->runtime->private_data;
    unsigned int period_size                = prtd->period_size;
    struct hifi_chn_pcm_trigger msg_body    = { 0 };
    int ret                                 = OK;

    msg_body.msg_type   = (unsigned short)HI_CHN_MSG_PCM_TRIGGER;
    msg_body.pcm_mode   = (unsigned short)substream->stream;
    msg_body.tg_cmd     = (unsigned short)cmd;
    msg_body.enPcmObj     = (unsigned short)0/*AP*/;
    msg_body.substream = substream;

    if( (SNDRV_PCM_TRIGGER_START == cmd) ||
            (SNDRV_PCM_TRIGGER_RESUME == cmd) ||
            (SNDRV_PCM_TRIGGER_PAUSE_RELEASE == cmd) )
    {
        msg_body.data_addr = substream->runtime->dma_addr + prtd->period_next * period_size;
        msg_body.data_len = period_size;
    }

    logi("%s:\r\n", __func__);
    logi("      msg_type = 0x%x\r\n", msg_body.msg_type);
    logi("      pcm_mode = %d\r\n", msg_body.pcm_mode);
    logi("      tg_cmd = %d\r\n", msg_body.tg_cmd);
    logi("      enPcmObj = %d\r\n", msg_body.enPcmObj);
    logi("      data_addr = 0x%x\r\n", msg_body.data_addr);
    logi("      data_len = %d\r\n", msg_body.data_len);
    logi("      substream = 0x%p\r\n", msg_body.substream);

    /* mail-box send */
    ret = hi6620_mailbox_send_data( &msg_body, sizeof(struct hifi_chn_pcm_trigger), 0 );
    if( OK != ret )
    {
        ret = -EBUSY;
    }

    return ret;
}

/*****************************************************************************
 �� �� ��  : hi6620_notify_pcm_set_buf
 ��������  : �˼�ͨ�Ÿ�֪HIFI ����BUFFER�Ѿ���
 �������  : hifi_channel_set_buffer *psrc_data
 �������  : ��
 �� �� ֵ  : STATIC int, 0 for Success; Others for error
 ���ú���  : hi6620_intr_handle_pb()��hi6620_intr_handle_cp()
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��31��
    ��    ��   : ʯ���� s00212991
    �޸�����   : �����ɺ���

*****************************************************************************/
STATIC int hi6620_notify_pcm_set_buf( struct snd_pcm_substream *substream )
{
    struct hi6620_runtime_data *prtd        =
                (struct hi6620_runtime_data *)substream->runtime->private_data;
    unsigned int period_size                = prtd->period_size;
    unsigned short pcm_mode                 = (unsigned short)substream->stream;
    struct hifi_channel_set_buffer msg_body    = { 0 };
    int ret                                 = 0;

    if ( (SNDRV_PCM_STREAM_PLAYBACK != pcm_mode) &&
            (SNDRV_PCM_STREAM_CAPTURE != pcm_mode) )
    {
        loge("pcm_mode=%d\n", pcm_mode);
        return -EINVAL;
    }

    msg_body.msg_type   = (unsigned short)HI_CHN_MSG_PCM_SET_BUF;
    msg_body.pcm_mode   = pcm_mode;
    msg_body.data_addr  = substream->runtime->dma_addr + (prtd->period_next * period_size);
    msg_body.data_len   = period_size;

    logd("d_addr=0x%x(%d)\r\n", msg_body.data_addr, msg_body.data_len);

    if (STATUS_RUNNING != prtd->status){
        loge("pcm is closed \n");
        return -EINVAL;
    }

    /* mail-box send */
    trace_dot(APCM,"4",0);
    ret = hi6620_mailbox_send_data( &msg_body, sizeof(struct hifi_channel_set_buffer), 0 );
    if( OK != ret )
    {
        ret = -EBUSY;
    }
    trace_dot(APCM,"5",0);

    logd("End(%d)\r\n", ret);
    return ret;
}

/*****************************************************************************
 �� �� ��  : hi6620_notify_recv_isr
 ��������  : �����˼�ͨ����Ϣ���յ������ݰ������ݣ�������Ӧ����
                ע��Ϊ�˼�ͨ������Ľ��պ���
 �������  : void *usr_para             : ע��ʱ���ݵĲ���
             void *mail_handle          : �������ݲ���
             unsigned int mail_len      : �������ݳ���
 �������  : ��
 �� �� ֵ  : STATIC irq_rt_t
 ���ú���  : hi6620_pcm_new()
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��31��
    ��    ��   : ʯ���� s00212991
    �޸�����   : �����ɺ���

*****************************************************************************/
STATIC irq_rt_t hi6620_notify_recv_isr( void *usr_para, void *mail_handle,  unsigned int mail_len )
{
    struct snd_pcm_substream * substream    = NULL;
    struct hi6620_runtime_data *prtd        = NULL;
    struct hi6620_pcm_mailbox_data *hi6620_pcm_mailbox = NULL;
    struct hifi_chn_pcm_period_elapsed mail_buf;
    unsigned long mail_size          = mail_len;
    unsigned long ret_mail          = 0;
#ifndef __DRV_AUDIO_MAILBOX_WORK__
    irq_rt_t ret                    = IRQ_NH;
#endif

    memset(&mail_buf, 0, sizeof(struct hifi_chn_pcm_period_elapsed));
    /*��ȡ������������*/
#ifdef CONFIG_SND_TEST_AUDIO_PCM_LOOP
    memcpy(&mail_buf, mail_handle, sizeof(struct hifi_chn_pcm_period_elapsed));
#else
    ret_mail = mailbox_read_msg_data(mail_handle, (unsigned char*)&mail_buf, &mail_size);

    if ((ret_mail != MAILBOX_OK)
        || (mail_size <= 0)
        || (mail_size > sizeof(struct hifi_chn_pcm_period_elapsed)))
    {
        loge("Empty point or data length error! size: %ld\n", mail_size);
        return IRQ_NH_MB;
    }
#endif

#ifdef __DRV_AUDIO_MAILBOX_WORK__
    substream = mail_buf.substream;
    if (NULL == substream) {
        loge("substream from hifi is NULL\n");
        return IRQ_NH_OTHERS;
    }
    if (NULL == substream->runtime) {
        loge("substream->runtime is NULL\n");
        return IRQ_NH_OTHERS;
    }

    prtd = (struct hi6620_runtime_data *)substream->runtime->private_data;
    if (NULL == prtd) {
        loge("prtd is NULL\n");
        return IRQ_NH_OTHERS;
    }
    if (STATUS_STOP == prtd->status) {
        logi("process has stopped there is still info coming from hifi\n");
        return IRQ_NH_OTHERS;
    }

    logd("Begin msg_type=0x%x, substream=0x%x\n", mail_buf.msg_type, mail_buf.substream);
    hi6620_pcm_mailbox = &prtd->hi6620_pcm_mailbox;
    hi6620_pcm_mailbox->msg_type = mail_buf.msg_type;
    hi6620_pcm_mailbox->pcm_mode = mail_buf.pcm_mode;
    hi6620_pcm_mailbox->substream = mail_buf.substream;

    queue_delayed_work(hi6620_pcm_mailbox->pcm_mailbox_delay_wq,
            &hi6620_pcm_mailbox->pcm_mailbox_delay_work,
            msecs_to_jiffies(0));

    logd("End\n");
    return IRQ_HDD;
#else
    switch( mail_buf.msg_type )
    {
    case HI_CHN_MSG_PCM_PERIOD_ELAPSED:
        ret = hi6620_mb_intr_handle( mail_buf.pcm_mode, mail_buf.substream );
        if( ret == IRQ_NH )
        {
            loge("ret : %d\n", ret);
        }
        break;
    default:
        ret = IRQ_NH_TYPE;
        /*������Ϣ���������Ͳ�Ӧ����*/
        loge("msg_type 0x%x\n", mail_buf.msg_type);
        break;
    }

    return ret;
#endif
}

/*****************************************************************************
 �� �� ��  : hi6620_notify_isr_register
 ��������  : ����˼����ݽ��ջص�����ע��
 �������  : void *pisr     : �ص�����ָ��
 �������  : ��
 �� �� ֵ  : STATIC int
 ���ú���  : hi6620_pcm_new()
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��31��
    ��    ��   : ʯ���� s00212991
    �޸�����   : �����ɺ���

*****************************************************************************/
STATIC int hi6620_notify_isr_register( irq_hdl_t pisr )
{
#ifdef CONFIG_SND_TEST_AUDIO_PCM_LOOP
    return OK;
#else
    int ret                     = OK;

    if( NULL == pisr )
    {
        loge("pisr==NULL!\n");
        ret = ERROR;
    }
    else
    {
        ret = mailbox_reg_msg_cb( MAILBOX_MAILCODE_HIFI_TO_ACPU_AUDIO, (void *)pisr, NULL );
        if( OK != ret )
        {
            loge("ret : %d\n", ret);
        }
    }

    return ret;
#endif
}


/*****************************************************************************
 �� �� ��  : hi6620_pcm_hifi_hw_params
 ��������  : Ӳ��ͨ���������ã�����ʹ�ú˼�ͨ�Ÿ�֪HIFI�����������
 �������  : struct snd_pcm_substream *substream
             struct snd_pcm_hw_params *params
 �������  : ��
 �� �� ֵ  : static int
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��8��4��
    ��    ��   : ʯ���� s00212991
    �޸�����   : �����ɺ���

*****************************************************************************/
STATIC int hi6620_pcm_hifi_hw_params(struct snd_pcm_substream *substream,
        struct snd_pcm_hw_params *params)
{
    unsigned long bytes                 = params_buffer_bytes(params);
    struct hi6620_runtime_data *prtd    =
                (struct hi6620_runtime_data *)substream->runtime->private_data;
    int ret                             = 0;

    IN_FUNCTION;

    logd("entry : %s\n", substream->stream == SNDRV_PCM_STREAM_PLAYBACK
            ? "PLAYBACK" : "CAPTURE");

    ret = snd_pcm_lib_malloc_pages(substream, bytes);
    if ( ret < 0 )
    {
        loge("snd_pcm_lib_malloc_pages ret : %d\n", ret);

        OUT_FUNCTION;
        return ret;
    }
    prtd->period_size = params_period_bytes(params);
    prtd->period_next = 0;

    /* ͨ���˼�ͨ�Ÿ�֪HIFI����hw_params */
    ret = hi6620_notify_pcm_hw_params( (unsigned short)substream->stream, params );
    if ( ret < 0 )
    {
        loge("hi6620_notify_pcm_hw_params ret : %d\n", ret);
        snd_pcm_lib_free_pages(substream);

        OUT_FUNCTION;
        return ret;
    }

    OUT_FUNCTION;

    return ret;
}

STATIC int hi6620_pcm_hifi_hw_free(struct snd_pcm_substream *substream)
{
    logd("entry : %s\n", substream->stream == SNDRV_PCM_STREAM_PLAYBACK
            ? "PLAYBACK" : "CAPTURE");

    /*
    ͨ���˼�ͨ�Ÿ�֪HIFI����hw_free(Playback/Capture)
    ��ע�͵��������н���
    hi6620_notify_pcm_hw_free( pcm_mode );
    */

    return snd_pcm_lib_free_pages(substream);
}

/*****************************************************************************
 �� �� ��  : hi6620_pcm_hifi_prepare
 ��������  : ��ʼ��˽������private������ʹ�ú˼�ͨ�Ÿ�֪HIFI����pcm prepare
 �������  : struct snd_pcm_substream *substream
 �������  : ��
 �� �� ֵ  : static int
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��8��4��
    ��    ��   : ʯ���� s00212991
    �޸�����   : �����ɺ���

*****************************************************************************/
STATIC int hi6620_pcm_hifi_prepare(struct snd_pcm_substream *substream)
{
    struct hi6620_runtime_data *prtd        =
                (struct hi6620_runtime_data *)substream->runtime->private_data;
    int ret                                 = OK;

    logd("entry : %s\n", substream->stream == SNDRV_PCM_STREAM_PLAYBACK
            ? "PLAYBACK" : "CAPTURE");

    /*
    ��ע�͵��������н���
    msg_body.pcm_mode = substream->stream;
    hi6620_notify_pcm_prepare( &msg_body );
    */

    /* init prtd */
    prtd->status        = STATUS_STOP;
    prtd->period_next   = 0;
    prtd->period_cur    = 0;
    return ret;
}

/*****************************************************************************
 �� �� ��  : hi6620_pcm_hifi_trigger
 ��������  : ����������ֹͣPCM������������ʹ�ú˼�ͨ�Ÿ�֪HIFI�������������-
             DMA����
 �������  : struct snd_pcm_substream *substream
             int cmd
 �������  : ��
 �� �� ֵ  : static int
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��8��4��
    ��    ��   : ʯ���� s00212991
    �޸�����   : �����ɺ���

*****************************************************************************/
STATIC int hi6620_pcm_hifi_trigger(int cmd, struct snd_pcm_substream *substream)
{
    struct snd_pcm_runtime *runtime         = substream->runtime;
    struct hi6620_runtime_data *prtd        =
                (struct hi6620_runtime_data *)substream->runtime->private_data;
    unsigned int num_periods                = runtime->periods;
    int ret                                 = OK;

    IN_FUNCTION;

    logd("entry : %s, cmd : %d\n", substream->stream == SNDRV_PCM_STREAM_PLAYBACK
            ? "PLAYBACK" : "CAPTURE", cmd);

    switch (cmd)
    {
    case SNDRV_PCM_TRIGGER_START:
    case SNDRV_PCM_TRIGGER_RESUME:
    case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
        /* ͨ���˼�ͨ�Ÿ�֪HIFI trigger״̬ */

        ret = hi6620_notify_pcm_trigger( cmd, substream );
        if ( ret < 0 )
        {
            loge("hi6620_notify_pcm_trigger ret : %d\n", ret);
        }
        else
        {
            spin_lock(&prtd->lock);
            prtd->status = STATUS_RUNNING;
            prtd->period_next = (prtd->period_next + 1) % num_periods;
            spin_unlock(&prtd->lock);
        }
        break;

    case SNDRV_PCM_TRIGGER_STOP:
    case SNDRV_PCM_TRIGGER_SUSPEND:
    case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
        /* ͨ���˼�ͨ�Ÿ�֪HIFI trigger״̬ */

        spin_lock(&prtd->lock);
        prtd->status = STATUS_STOPPING;
        spin_unlock(&prtd->lock);

        ret = hi6620_notify_pcm_trigger( cmd, substream );
        if ( ret < 0 )
        {
            loge("hi6620_notify_pcm_trigger ret : %d\n", ret);
        }

        break;

    default:
        loge("cmd error : %d", cmd);
        ret = -EINVAL;
        break;
    }

    OUT_FUNCTION;

    return ret;
}

/*****************************************************************************
 �� �� ��  : hi6620_pcm_hifi_pointer
 ��������  : ��ѯ��ǰPCM����������֡��
 �������  : struct snd_pcm_substream *substream
 �������  : ��
 �� �� ֵ  : static snd_pcm_uframes_t
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��8��4��
    ��    ��   : ʯ���� s00212991
    �޸�����   : �����ɺ���

*****************************************************************************/
STATIC snd_pcm_uframes_t hi6620_pcm_hifi_pointer(struct snd_pcm_substream *substream)
{
    struct snd_pcm_runtime *runtime         = substream->runtime;
    struct hi6620_runtime_data *prtd        =
                (struct hi6620_runtime_data *)substream->runtime->private_data;
    long frame                     = 0L;

    frame = bytes_to_frames(runtime, prtd->period_cur * prtd->period_size);
    if(frame >= runtime->buffer_size)
        frame = 0;

    return frame;
}

/*****************************************************************************
 �� �� ��  : hi6620_pcm_hifi_open
 ��������  : PCM���������л�����ʼ��������ͬʱʹ�ú˼�ͨ�Ÿ�֪HIFI PCM����
 �������  : struct snd_pcm_substream *substream
 �������  : ��
 �� �� ֵ  : static int
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��8��4��
    ��    ��   : ʯ���� s00212991
    �޸�����   : �����ɺ���

*****************************************************************************/
STATIC int hi6620_pcm_hifi_open(struct snd_pcm_substream *substream)
{
    struct hi6620_runtime_data *prtd    = NULL;
    int ret                             = OK;

    IN_FUNCTION;

    logd("entry : %s\n", substream->stream == SNDRV_PCM_STREAM_PLAYBACK
            ? "PLAYBACK" : "CAPTURE");

    prtd = kzalloc(sizeof(struct hi6620_runtime_data), GFP_KERNEL);
    if (NULL == prtd)
    {
        loge("kzalloc faile,prtd==NULL\n");
        OUT_FUNCTION;
        return -ENOMEM;
    }

    spin_lock_init(&prtd->lock);

    substream->runtime->private_data = prtd;

    if (SNDRV_PCM_STREAM_PLAYBACK == substream->stream)
        snd_soc_set_runtime_hwparams(substream, &hi6620_hardware_playback);
    else
        snd_soc_set_runtime_hwparams(substream, &hi6620_hardware_capture);

#ifdef __DRV_AUDIO_MAILBOX_WORK__
    prtd->hi6620_pcm_mailbox.pcm_mailbox_delay_wq = hi6620_pcm_mailbox_workqueue.pcm_mailbox_delay_wq;
    INIT_DELAYED_WORK(&prtd->hi6620_pcm_mailbox.pcm_mailbox_delay_work, pcm_mailbox_work_func);
#endif

    /* ֪ͨHIFI PCM Open */
    ret = hi6620_notify_pcm_open( (unsigned short)substream->stream );
    if ( ret < 0 )
    {
        loge("hi6620_notify_pcm_open ret : %d\n", ret);
        OUT_FUNCTION;
        return ret;
    }

    OUT_FUNCTION;

    return OK;
}

STATIC int hi6620_pcm_hifi_close(struct snd_pcm_substream *substream)
{
    struct hi6620_runtime_data *prtd        =
                (struct hi6620_runtime_data *)substream->runtime->private_data;

    IN_FUNCTION;

    logd("entry : %s\n", substream->stream == SNDRV_PCM_STREAM_PLAYBACK
            ? "PLAYBACK" : "CAPTURE");

    if(NULL == prtd)
    {
        loge("prtd==NULL\n");
    }

    hi6620_notify_pcm_close( (unsigned short)substream->stream );

#ifdef CONFIG_SND_TEST_AUDIO_PCM_LOOP
    if(hi6620_simu_pcm.simu_pcm_delay_wq) {
        cancel_delayed_work(&hi6620_simu_pcm.simu_pcm_delay_work);
        flush_workqueue(hi6620_simu_pcm.simu_pcm_delay_wq);
    }
#endif

    if (prtd)
    {
        kfree(prtd);
        substream->runtime->private_data = NULL;
    }

    OUT_FUNCTION;

    return OK;
}

static int hi6620_pcm_hw_params(struct snd_pcm_substream *substream,
        struct snd_pcm_hw_params *params)
{
    int ret = 0;
    if (substream->pcm->device == 0)
        ret = hi6620_pcm_hifi_hw_params(substream, params);
    return ret;
}

static int hi6620_pcm_hw_free(struct snd_pcm_substream *substream)
{
    int ret = 0;
    struct hi6620_runtime_data *prtd        = NULL;
    int i   = 0;

    prtd    = (struct hi6620_runtime_data *)substream->runtime->private_data;

    if (substream->pcm->device == 0){
        for(i = 0; i < 30 ; i++){  /* wait for dma ok */
            if (STATUS_STOP == prtd->status){
                break;
            } else {
                msleep(10);
            }
        }
        if (30 == i){
            loge("timeout for waiting for stop info from hifi \n");
        }
#ifdef  __DRV_AUDIO_MAILBOX_WORK__
        flush_workqueue(hi6620_pcm_mailbox_workqueue.pcm_mailbox_delay_wq);
#endif
        ret = hi6620_pcm_hifi_hw_free(substream);
    }
    return ret;
}

static int hi6620_pcm_prepare(struct snd_pcm_substream *substream)
{
    int ret = 0;
    if (substream->pcm->device == 0)
        ret = hi6620_pcm_hifi_prepare(substream);
    return ret;
}

static int hi6620_pcm_trigger(struct snd_pcm_substream *substream, int cmd)
{
    int ret = 0;
    if (substream->pcm->device == 0)
        ret = hi6620_pcm_hifi_trigger(cmd, substream);
    return ret;
}

static snd_pcm_uframes_t hi6620_pcm_pointer(struct snd_pcm_substream *substream)
{
    unsigned long ret = 0L;
    if (substream->pcm->device == 0)
        ret = hi6620_pcm_hifi_pointer(substream);
    return ret;
}

static int hi6620_pcm_open(struct snd_pcm_substream *substream)
{
    int ret = 0;
    if (substream->pcm->device == 0)
    {
        if(SNDRV_PCM_STREAM_CAPTURE == substream->stream)
        {
            ret = down_interruptible(&g_pcm_cp_open_sem);
            pcm_cp_status_open = (u32)1;
            up(&g_pcm_cp_open_sem);
        }
        else if(SNDRV_PCM_STREAM_PLAYBACK == substream->stream)
        {
            ret = down_interruptible(&g_pcm_pb_open_sem);
            pcm_pb_status_open = (u32)1;
            up(&g_pcm_pb_open_sem);
        }
        else
        {
            /* do nothing */
        }
        ret = hi6620_pcm_hifi_open(substream);
    }
    else
    {
        if (SNDRV_PCM_STREAM_PLAYBACK == substream->stream)
            snd_soc_set_runtime_hwparams(substream, &hi6620_hardware_modem_playback);
    }
    return ret;
}

static int hi6620_pcm_close(struct snd_pcm_substream *substream)
{
    int ret = 0;
    if (substream->pcm->device == 0)
    {
        if(SNDRV_PCM_STREAM_CAPTURE == substream->stream)
        {
            ret = down_interruptible(&g_pcm_cp_open_sem);
            pcm_cp_status_open = (u32)0;
            ret = hi6620_pcm_hifi_close(substream);
            up(&g_pcm_cp_open_sem);
        }
        else if(SNDRV_PCM_STREAM_PLAYBACK == substream->stream)
        {
            ret = down_interruptible(&g_pcm_pb_open_sem);
            pcm_pb_status_open = (u32)0;
            ret = hi6620_pcm_hifi_close(substream);
            up(&g_pcm_pb_open_sem);
        }
        else
        {
            /* do nothing */
        }
    }
    return ret;
}

/* define all pcm ops of hi6620 pcm */
static struct snd_pcm_ops hi6620_pcm_ops = {
    .open       = hi6620_pcm_open,
    .close      = hi6620_pcm_close,
    .ioctl      = snd_pcm_lib_ioctl,
    .hw_params  = hi6620_pcm_hw_params,
    .hw_free    = hi6620_pcm_hw_free,
    .prepare    = hi6620_pcm_prepare,
    .trigger    = hi6620_pcm_trigger,
    .pointer    = hi6620_pcm_pointer,
};

STATIC int hi6620_pcm_new(struct snd_card *card,
        struct snd_soc_dai *dai, struct snd_pcm *pcm)
{
    int ret = 0;

    IN_FUNCTION;

    if (!card->dev->dma_mask)
    {
        logi("dev->dma_mask not set\n");
        card->dev->dma_mask = &hi6620_pcm_dmamask;
    }

    if (!card->dev->coherent_dma_mask)
    {
        logi("dev->coherent_dma_mask not set\n");
        card->dev->coherent_dma_mask = hi6620_pcm_dmamask;
    }
    if (pcm->device == 0)
    {
        ret = snd_pcm_lib_preallocate_pages_for_all(pcm, SNDRV_DMA_TYPE_DEV,
                pcm->card->dev, HI6620_MAX_BUFFER_SIZE, HI6620_MAX_BUFFER_SIZE);
        if (ret)
        {
            loge("snd_pcm_lib_preallocate_pages_for_all error : %d\n", ret);
            return ret;
        }
        logi("pcm->device = 0\n");

        /* ע��˼�ͨ�����ݽ��պ��� */
        ret = hi6620_notify_isr_register( (void *)hi6620_notify_recv_isr );
        if (ret)
        {
            loge("notify Isr register error : %d\n", ret);
            snd_pcm_lib_preallocate_free_for_all(pcm);
        }
    }
    else
    {
        logi("pcm-device = %d\n", pcm->device);
        ret = snd_pcm_lib_preallocate_pages_for_all(pcm, SNDRV_DMA_TYPE_DEV,
                pcm->card->dev, HI6620_BUFFER_SIZE_MM, HI6620_BUFFER_SIZE_MM);
        if (ret)
        {
            loge("snd_pcm_lib_preallocate_pages_for_all error : %d\n", ret);
            return ret;
        }
    }

    OUT_FUNCTION;

    return ret;
}

static void hi6620_pcm_free(struct snd_pcm *pcm)
{
    IN_FUNCTION;

    if (pcm->device == 0) {
        logi("pcm->device = 0\n");
    }
    snd_pcm_lib_preallocate_free_for_all(pcm);

    OUT_FUNCTION;
}

struct snd_soc_platform_driver hi6620_pcm_platform = {
    .ops      = &hi6620_pcm_ops,
    .pcm_new  = hi6620_pcm_new,
    .pcm_free = hi6620_pcm_free,
};

static int __devinit hi6620_platform_probe(struct platform_device *pdev)
{
    int ret = -ENODEV;

    IN_FUNCTION;

    /* register dai (name : hi6620-hifi) */
    ret = snd_soc_register_dais(&pdev->dev, hi6620_dai, ARRAY_SIZE(hi6620_dai));
    if (ret) {
        loge("snd_soc_register_dai return %d\n" ,ret);
        goto probe_failed;
    }

    /* register platform (name : hi6620-hifi) */
    ret = snd_soc_register_platform(&pdev->dev, &hi6620_pcm_platform);
    if (ret) {
        loge("snd_soc_register_platform return %d\n", ret);
        snd_soc_unregister_dais(&pdev->dev, ARRAY_SIZE(hi6620_dai));
        goto probe_failed;
    }

#ifdef CONFIG_SND_TEST_AUDIO_PCM_LOOP
    hi6620_simu_pcm.simu_pcm_delay_wq = create_singlethread_workqueue("simu_pcm_delay_wq");
    if (!(hi6620_simu_pcm.simu_pcm_delay_wq)) {
        pr_err("%s(%u) : workqueue create failed", __FUNCTION__,__LINE__);
        ret = -ENOMEM;
        goto simu_pcm_wq_failed;
    }
    INIT_DELAYED_WORK(&hi6620_simu_pcm.simu_pcm_delay_work, simu_pcm_work_func);
#endif
#ifdef __DRV_AUDIO_MAILBOX_WORK__
    hi6620_pcm_mailbox_workqueue.pcm_mailbox_delay_wq = create_singlethread_workqueue("pcm_mailbox_delay_wq");
    if (!(hi6620_pcm_mailbox_workqueue.pcm_mailbox_delay_wq)) {
        pr_err("%s(%u) : workqueue create failed", __FUNCTION__,__LINE__);
        ret = -ENOMEM;
        goto pcm_mailbox_wq_failed;
    }
/*  put INIT_DELAYED_WORK to open() function
    INIT_DELAYED_WORK(&hi6620_pcm_mailbox_workqueue.pcm_mailbox_delay_work, pcm_mailbox_work_func); */
#endif

    OUT_FUNCTION;

    return ret;

#ifdef CONFIG_SND_TEST_AUDIO_PCM_LOOP
simu_pcm_wq_failed:
    snd_soc_unregister_platform(&pdev->dev);
    snd_soc_unregister_dais(&pdev->dev, ARRAY_SIZE(hi6620_dai));
#endif
#ifdef __DRV_AUDIO_MAILBOX_WORK__
pcm_mailbox_wq_failed:
    snd_soc_unregister_platform(&pdev->dev);
    snd_soc_unregister_dais(&pdev->dev, ARRAY_SIZE(hi6620_dai));
#endif

probe_failed:

    OUT_FUNCTION;
    return ret;
}

static int __devexit hi6620_platform_remove(struct platform_device *pdev)
{
#ifdef CONFIG_SND_TEST_AUDIO_PCM_LOOP
    if(hi6620_simu_pcm.simu_pcm_delay_wq) {
        cancel_delayed_work(&hi6620_simu_pcm.simu_pcm_delay_work);
        flush_workqueue(hi6620_simu_pcm.simu_pcm_delay_wq);
        destroy_workqueue(hi6620_simu_pcm.simu_pcm_delay_wq);
    }
#endif
#ifdef __DRV_AUDIO_MAILBOX_WORK__
    if(hi6620_pcm_mailbox_workqueue.pcm_mailbox_delay_wq) {
        flush_workqueue(hi6620_pcm_mailbox_workqueue.pcm_mailbox_delay_wq);
        destroy_workqueue(hi6620_pcm_mailbox_workqueue.pcm_mailbox_delay_wq);
    }
#endif

    snd_soc_unregister_platform(&pdev->dev);
    snd_soc_unregister_dais(&pdev->dev, ARRAY_SIZE(hi6620_dai));

    return 0;
}

static struct platform_driver hi6620_platform_driver = {
    .driver = {
        .name = "hi6620-hifi",
        .owner = THIS_MODULE,
    },
    .probe  = hi6620_platform_probe,
    .remove = __devexit_p(hi6620_platform_remove),
};

static int status_read_proc_hstatus(char *page, char **start, off_t offset,
                    int count, int *eof, void *data)
{
    *eof = 1;
    return sprintf(page, "%d", 0);
}

static int status_write_proc_hstatus(struct file *file, const char *buffer,
                    unsigned long count, void *data)
{
    char buf[64] = {0};
    int value = 0;

    if (count < 1)
        return -EINVAL;
    if (count > 60)
        count  = 60;

    if (copy_from_user(buf, buffer, count)) {
        loge("copy_from_user Error\n");
        return -EFAULT;
    }
    if (sscanf(buf, "0x%x", &value) != 1) {
        logd("set the audiostatus error\r\n");
    }
    logd("get the audiostatus %d\r\n",value);

    return count;
}

static int __init hi6620_init(void)
{
    struct proc_dir_entry *ent;

    audio_pcm_dir = proc_mkdir("hpcm", NULL);
    if (audio_pcm_dir == NULL) {
        loge("Unable to create /proc/hpcm directory");
        return -ENOMEM;
    }

    /* Creating read/write "status" entry */
    ent = create_proc_entry("status", 0777, audio_pcm_dir);
    if (ent == NULL) {
        remove_proc_entry("hpcm", 0);
        loge("Unable to create /proc/hpcm/status entry");
        return -ENOMEM;
    }
    ent->read_proc = status_read_proc_hstatus;
    ent->write_proc = status_write_proc_hstatus;

    return platform_driver_register(&hi6620_platform_driver);
}
module_init(hi6620_init);

static void __exit hi6620_exit(void)
{
    remove_proc_entry("status", audio_pcm_dir);

    platform_driver_unregister(&hi6620_platform_driver);
}
module_exit(hi6620_exit);

MODULE_AUTHOR("S00212991");
MODULE_DESCRIPTION("Hi6620 HIFI platform driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:hifi");


/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : hifi_misc.h
  �� �� ��   : ����
  ��    ��   : 
  ��������   : 2012��8��2��
  ����޸�   :
  ��������   : hifi_misc.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��8��2��
    ��    ��   : 
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/

#ifndef __HIFI_MISC_H__
#define __HIFI_MISC_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  2 �궨��
*****************************************************************************/
/*#define HIFI_MISC_NAME "hifi"*/

/* Debug info */
#define INFO_LEVEL   0
#define DEBUG_LEVEL  0
#define ERROR_LEVEL  1
#define PAGE_MAX_SIZE        0x1000

#ifndef OK
#define OK           0
#endif
#define ERROR       (-1)
#define BUSY        (-2)
#define NOMEM       (-3)
#define INVAILD     (-4)

/* IOCTL��κͳ��ε�SIZE���� */
#define SIZE_LIMIT_PARAM        (256)

/* HIFIר���� */
#define OFFSET_HIFI_PRIV        (0x00300000)
#define SIZE_HIFI_PRIV          (0x00500000)
#define HIFI_PRIV_ADDR          (HIFI_SYS_MEM_ADDR + OFFSET_HIFI_PRIV)
#define SIZE_PARAM_PRIV         (100 * 1024)

/* ����HIFI��Ϣ��ǰ��cmd_idռ�õ��ֽ��� */

#define SIZE_CMD_ID        (8)

/* notice�����ϱ�һ�λ��� */
#define REV_MSG_NOTICE_ID_MAX       2

#define ACPU_TO_HIFI_ASYNC_CMD    0xFFFFFFFF

#define BUFFER_NUM  8

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
typedef enum{
    HIFI_CLOSE,
    HIFI_OPENED,
}HIFI_STATUS;


/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/

/*����hifi�ظ���Ϣ����¼cmd_id������*/
typedef struct
{
    unsigned char *mail_buff;       /* �������ݽ��յ�buff */
    unsigned int mail_buff_len;
    unsigned int cmd_id;            /* �����������ǰ4���ֽ���cmd_id */
    unsigned char *out_buff_ptr;    /* ָ��mail_buff cmd_id���λ�� */
    unsigned int out_buff_len;
} rev_msg_buff;

/* hifi��ά�ɲ⹤���ṹ */
typedef struct
{
    unsigned int        work_id;
    struct work_struct  work_stru;
} hifi_mntn_work_stru;


struct recv_request{
    struct list_head recv_node;
    rev_msg_buff rev_msg;
};



/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/
#ifndef LOG_TAG
#define LOG_TAG "hifi_misc "
#endif

#if INFO_LEVEL
#define logi(fmt, ...) pr_info(LOG_TAG"[I]:%s:%d: "fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define logi(fmt, ...)
#endif

#if DEBUG_LEVEL
#define logd(fmt, ...) pr_info(LOG_TAG"[D]:%s:%d: "fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define logd(fmt, ...)
#endif

#if ERROR_LEVEL
#define loge(fmt, ...) pr_info(LOG_TAG"[E]:%s:%d: "fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define loge(fmt, ...)
#endif

#define IN_FUNCTION   logd("Begin\n");
#define OUT_FUNCTION  logd("End\n");

/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/
int hifi_misc_async_write(unsigned char *arg, unsigned int len);
void __exit hifi_misc_exit( void );
void hifi_misc_handle_mail( void *usr_para, void *mail_handle, unsigned int mail_len );
int __init hifi_misc_init( void );
long hifi_misc_ioctl(struct file *fd,
                                    unsigned int cmd,
                                    unsigned long arg);
int hifi_misc_open(struct inode *finode, struct file *fd);
int hifi_misc_probe (struct platform_device *pdev);
void hifi_misc_proc_init( void );
int hifi_misc_proc_read(char *pg,
                                   char**start,
                                   off_t off,
                                   int count,
                                   int *eof,
                                   void *data);
void hifi_misc_receive_task_sync(void);
void hifi_misc_receive_task_proc(void);
int hifi_misc_release(struct inode *finode, struct file *fd);
int hifi_misc_remove(struct platform_device *pdev);
int hifi_misc_sync_write(unsigned char  *arg, unsigned int len);


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of hifi_misc.h */

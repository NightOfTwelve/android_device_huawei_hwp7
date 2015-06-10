/*
 *****************************************************************************
 * Copyright (c) 2001-2021, Huawei Tech. Co., Ltd. All rights reserved.
 *
 * File Name    : secure.c
 * Author       : chenyingguo 61362
 * Created      : 2012/9/13
 * Description  : secure boot for dx
 *
 *****************************************************************************
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *****************************************************************************
 */

/*
 *****************************************************************************
 * 1 Header File Including
 *****************************************************************************
 */
#include <linux/slab.h>
#include <mach/delay.h>
#include "soc_baseaddr_interface.h"
#include "soc_sctrl_interface.h"
#include <soc_ao_sctrl_interface.h>
#include "reset_secure.h"
#include "bootimagesverifier_api.h" 

#if 0
#include "bootimagesverifier_def.h"

#include "bootimagesverifier_parser.h"

#include "bootimagesverifier_swvalidation.h"
#include "sbrom_management_defs.h"
#endif
#include "../../mmc/host/hi6620_mmc_raw.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*
 *****************************************************************************
 * 2 Global Variable Definition
 *****************************************************************************
 */
 /* VRL Magic number */
#define DX_VRL_MAGIC_NUMBER_DEFAULT_VALUE 0xE59FF052

#define TRUE    1
#define FALSE   0

#define ERROR  -1
#define OK      0

#define EMMC_BLOCK_SIZE                 512
#define FLASH_PAGE_SIZE 2048


/* ��¼ӳ����RAM�еĵ�ַ */
int g_image_addr = 0;

/* ����VRL�Ľ����ṹ�� */
struct vrl_parse_stru g_vrl_parse_st;

/* DX��ȫУ�������bufferָ�� */
unsigned long *g_work_space = 0;

/* ��¼VRL�е���С����汾�� */
unsigned int g_sw_min_version = 0;

/* ��¼��ǰ��Ҫʹ�õĹ�Կ���� */
unsigned int g_otp_key_index = 0;

/* ��ֹ�����Ľṹ�� */
struct oem_stru g_oem_st;

/* ��ȫ����ļ��ص�ַ */
unsigned int g_image_load_addr = 0;

/*���ڱ�ʶ���ĸ�ӳ��*/
vrl_id  g_image_id = 0;
char *g_emmc_read_buf = NULL;



/*
 *****************************************************************************
 * 3 Function Definition
 *****************************************************************************
 */
char * get_emmc_buff(void)
{
    if (NULL == g_emmc_read_buf)
    {
        g_emmc_read_buf = (char*)kmalloc(FLASH_PAGE_SIZE*2, GFP_KERNEL);
        if (NULL == g_emmc_read_buf)
        {
            printk(KERN_ERR"fail to malloc for emmc buff\n");
        }
    }
    return g_emmc_read_buf;
}

/*
 *****************************************************************************
 * Prototype    : efuse_power_on
 * Description  : power on the ldo for efuse
 * Input        : None
 * Output       : None
 * Return Value : None
 * Author       : chenyingguo 61362
 *****************************************************************************
 */
void efuse_power_on(void)
{
}

/*
 *****************************************************************************
 * Prototype    : efuse_power_off
 * Description  : power off the ldo for efuse
 * Input        : None
 * Output       : None
 * Return Value : None
 * Author       : chenyingguo 61362
 *****************************************************************************
 */
void efuse_power_off(void)
{
}


/*
 *****************************************************************************
 * Prototype    : test_bit_value
 * Description  : check bits in register whether are equal to the special value
 * Input        : addr��  register address
                  bit_mask��bits to be check
                  bit_value��special value
 * Output       : None
 * Return Value : TRUE: equal
                  FALSE:not equal
 * Author       : chenyingguo 61362
 *****************************************************************************
 */
unsigned int test_bit_value(unsigned int addr, unsigned int bit_mask, unsigned int bit_value)
{
#define TIME_DELAY_MAX    1000000       /*����ӳ�ʱ��,1��*/
    unsigned int time;

    for (time = 0; time < TIME_DELAY_MAX; time++) {
        if (bit_value == (INREG32(addr) & bit_mask)) {
            return TRUE;
        }
        udelay(1);
    }

    return FALSE;
}


/*
 *****************************************************************************
 * Prototype    : read_efuse
 * Description  : read special group of efuse
 * Input        : group��special group of efuse
                  buf��  data buffer
                  len��  length of the group
 * Output       : None
 * Return Value : OK/ERROR
 * Author       : chenyingguo 61362
 *****************************************************************************
 */
int read_efuse(unsigned int group, unsigned int *buf, unsigned int len)
{
    unsigned int *ptmp = buf;
    unsigned int cnt;

    /* ����ж� */
    if (0 == buf) {
        return ERROR;
    }

    if ((group > (EFUSE_MAX_SIZE / 4) )
        || ((group + len) > (EFUSE_MAX_SIZE / 4) )) {
        return ERROR;
    }

    /*����EFUSE�ӿ�ʱ��Ϊ�ڲ�����*/
    SETBITVALUE32(EFUSEC_CFG, EFUSEC_SIG_SEL_MASK, EFUSEC_SIG_SEL_MASK);

    /* ѭ����ȡEfuseֵ */
    for (cnt = 0; cnt < len; cnt++) {
        /* ���ö�ȡ��ַ */
        OUTREG32(EFUSE_GROUP, group + cnt);

        /* ʹ�ܶ� */
        SETBITVALUE32(EFUSEC_CFG, EFUSEC_RD_EN_MASK, EFUSEC_RD_EN_MASK);

        /* �ȴ���ʹ�����óɹ�����ʹ�ܳ�ʱ���ش��� */
        if (!test_bit_value(EFUSEC_CFG, EFUSEC_RD_EN_MASK, 0)) {
            return ERROR;
        }

        /* �ȴ������ */
        if (!test_bit_value(EFUSEC_STATUS, EFUSEC_RD_STATUS, EFUSEC_RD_STATUS)) {
            return ERROR;
        }

        /* ��ȡ���� */
        *ptmp = INREG32(EFUSEC_DATA);
        ptmp++;
    }

    /* AIB select */
    SETBITVALUE32(EFUSEC_CFG, EFUSEC_SIG_SEL_INNER, 0);

    return OK;
}


/*****************************************************************************
 �� �� ��  : get_efuse_value
 ��������  : ��ȡefuse��ֵ��֧�ֿ�group��
 �������  : unsigned int start_bit
             unsigned int * buffer
             unsigned int bit_cnt
 �������  : ��
 �� �� ֵ  : int
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��3��4��
    ��    ��   : ��ӭ�� 61362
    �޸�����   : �����ɺ���

*****************************************************************************/
int get_efuse_value(unsigned int start_bit, unsigned int * buffer, unsigned int bit_cnt)
{
    unsigned int group_start;
    unsigned int read_cnt;
    unsigned int unalinged_size;

    group_start    = start_bit/32;
    unalinged_size = start_bit%32;
    read_cnt       = (unalinged_size + 31)/32;
    read_cnt      += (bit_cnt - unalinged_size + 31)/32;

    if(ERROR == read_efuse(group_start, buffer, read_cnt)) {
        return ERROR;
    }

    return OK;
}

/*
 *****************************************************************************
 * Prototype    : check_oem_hw
 * Description  : check OEM_ID and HW_ID in efuse
 * Input        : None
 * Output       : None
 * Return Value : OK
                  ERROR
 * Author       : chenyingguo 61362
 *****************************************************************************
 */
int check_oem_hw(void)
{
    unsigned char buffer[64];
    unsigned int oem_sel;
    unsigned int carrier_id;
    unsigned int hw_id;
    unsigned int oem_bit1_cnt;
    unsigned int start_bit;
    unsigned char bit_cnt;
    unsigned char * ptmp;
    unsigned int tmp;
    int i;

    /* read oem selction efuse unit */
    if (ERROR == get_efuse_value(EFUSE_OEM_SEL_BIT_START, (unsigned int *)buffer,
                                  EFUSE_OEM_SEL_BIT_SIZE)) {
        return ERROR;
    }
    oem_sel = buffer[0]&((1<<EFUSE_OEM_SEL_BIT_SIZE) - 1);
    if (0 == oem_sel) {
        g_oem_st.flag = 0;
        return OK;
    }

    /* check oem select id */
    for (i = 0; i < EFUSE_OEM_SEL_BIT_SIZE; i++) {
        if (0 == (oem_sel&(1<<i))) {
            break;
        }
    }

    if  (oem_sel != (1 << i) - 1) {
        /* should not go to here */
        printk(KERN_ERR"oem_sel is 0x%x error!\n", oem_sel);
    }


    /* read carrier ID��HW ID��OEM bit1 CNT efuse unit */
    start_bit  = EFUSE_OEM_SEL_BIT_START + EFUSE_OEM_SEL_BIT_SIZE;
    bit_cnt    = EFUSE_CARRIER_ID_BIT_SIZE + EFUSE_HW_ID_BIT_SIZE + EFUSE_CARRIER_HW_BIT1_CNT_SIZE;
    start_bit += bit_cnt * (i - 1);
    if (ERROR == get_efuse_value(start_bit , (unsigned int *)buffer, bit_cnt)) {
        return ERROR;
    }

    ptmp       = buffer + ((start_bit % 32) / 8);
    carrier_id = (unsigned int)(*ptmp + ((*(ptmp + 1)) << 8));
    hw_id      = (unsigned int)(*(ptmp + 2) + ((*(ptmp + 3)) << 8));
    bit_cnt    = *(ptmp + 4);
    tmp        = carrier_id + (hw_id << 16);

    /* check bit 1 count of carrier_id and hw_id */
    for (i = 0; i < 32; i++) {
        if (0 != (tmp & (1 << i))) {
            bit_cnt--;
        }
    }

    if (0 != bit_cnt) {
        return ERROR;
    }

    g_oem_st.flag       = 1;
    g_oem_st.carrier_id = carrier_id;
    g_oem_st.hw_id      = hw_id;

    return OK;
}

/*
 *****************************************************************************
 * Prototype    : check_secure_mode
 * Description  : check whether is in secure mode
 * Input        : None
 * Output       : None
 * Return Value : TRUE: secure mode
                  FALSE: not secure mode
 * Author       : chenyingguo 61362
 *****************************************************************************
 */
int check_secure_mode(void)
{
    SOC_AO_SCTRL_SC_SYS_STAT0_UNION *p_sc_sys_stat0;
    unsigned int temp;
    static int ret = -1;

    /* ����Ƶ���ķ���efuse */
    if (-1 != ret) {
        return ret;
    }

    /* ���ϵͳ״̬�Ĵ�������ȡ���еİ�ȫģʽbit */
    p_sc_sys_stat0 = (SOC_AO_SCTRL_SC_SYS_STAT0_UNION *)
                     SOC_AO_SCTRL_SC_SYS_STAT0_ADDR(SOC_SC_ON_BASE_ADDR);
    temp = p_sc_sys_stat0->reg.security_boot_flg;

    /* �����bitΪ0������FALSE����ʾ�Ƿǰ�ȫģʽ */
    if (temp == 0) {
        ret = FALSE;
    }
    else {
        /* �����bitΪ1����Ҫ���efuse�е�OEM_ID��HW_ID */
        if(OK == check_oem_hw()) {
            /* ��ȫģʽ*/
            ret = TRUE;
        }
        else {
            /* �ǰ�ȫģʽ*/
            ret = FALSE;
        }
    }

    return ret;
}

/*
 *****************************************************************************
 * Prototype    : sec_emmc_read
 * Description  : emmc read function for secure verify of DX
 * Input        : addr: emmc address
                  data: data buffer
                  size: length of data buffer
                  context: unused
 * Output       : None
 * Return Value : OK/ERROR
 * Author       : chenyingguo 61362
 *****************************************************************************
 */
unsigned int sec_emmc_read(unsigned int addr, unsigned char * data, unsigned int size, void *context)
{
    unsigned int ret;
    unsigned int start_block;
    unsigned int block_count;
    unsigned int unalign_size;

    context = context;
    start_block = addr/EMMC_BLOCK_SIZE;
    unalign_size = addr%EMMC_BLOCK_SIZE;
    block_count = (unalign_size + size + EMMC_BLOCK_SIZE - 1 )/EMMC_BLOCK_SIZE;

    ret = k3v2_mmc_raw_read((char*)data, start_block, block_count);
    
    if (-1 == ret) {
        return ERROR;
    }

    if (0 != unalign_size) 
    {
        memcpy(data, data + unalign_size, size);
    }

    return OK;
}

/*
 *****************************************************************************
 * Prototype    : check_vrl
 * Description  : check vrl in emmc,if vrl is bad,restore from vrl backup area
 * Input        : vrl_addr: vrl address in vrl area
                  vrl_addr_bk: vrl backup address in vrl backup area
 * Output       : None
 * Return Value : OK/ERROR
 * Author       : chenyingguo 61362
 *****************************************************************************
 */
int check_vrl(unsigned int vrl_addr, unsigned int vrl_bk_addr)
{
    VRL_Header_t *vrl_header;
    unsigned char *vrl_buf = (unsigned char *)get_emmc_buff();
    unsigned char *vrl_bk_buf = vrl_buf + VRL_SIZE;
    unsigned int ret;

    /* ��ȡVRL���� */
    ret = sec_emmc_read(vrl_addr, vrl_buf, VRL_SIZE, 0);
    ret |= sec_emmc_read(vrl_bk_addr, vrl_bk_buf, VRL_SIZE, 0);
    if (OK != ret) {
        return ERROR;
    }

    if (1 == g_oem_st.flag) {
        /* У��Carrier ID��HW ID */
        if ((*(unsigned int *)vrl_buf != g_oem_st.carrier_id ) ||
             (*(unsigned int *)(vrl_buf + 4) != g_oem_st.hw_id)) {
                printk(KERN_ERR"check Carrier ID and HW ID failed!\n");
             return ERROR;;
         }
    }

    /* ���VRL�Ƿ���ȷ */
    vrl_header = (VRL_Header_t *)(vrl_buf + ADDITIONAL_DATA_SIZE);
    if (vrl_header->magicNumber != DX_VRL_MAGIC_NUMBER_DEFAULT_VALUE) {
        vrl_header = (VRL_Header_t *)(vrl_bk_buf + ADDITIONAL_DATA_SIZE);
        if (vrl_header->magicNumber != DX_VRL_MAGIC_NUMBER_DEFAULT_VALUE) {
            return ERROR;
        }
        else {
 #if 0
            if (OK != secure_write_vrl(vrl_addr, vrl_bk_buf, VRL_SIZE)) {
                return ERROR;
            }
 #endif
        }
    }
    else {
        vrl_header = (VRL_Header_t *)(vrl_bk_buf + ADDITIONAL_DATA_SIZE);
        if (vrl_header->magicNumber != DX_VRL_MAGIC_NUMBER_DEFAULT_VALUE) {
#if 0
            if (OK != secure_write_vrl(vrl_bk_addr, vrl_buf, VRL_SIZE)) {
                (void)bprintf("secure_write_vrl write vrl bk failed!\n" );
            }
#endif
        }
    }

    return OK;
}

/*
 *****************************************************************************
 * Prototype    : check_vrl_valid
 * Description  : check whether the vrl is valid
 * Input        : buf
 * Output       : None
 * Return Value : OK/ERROR
 * Author       : chenyingguo 61362
 *****************************************************************************
 */
int check_vrl_valid(unsigned int buf)
{
    VRL_Header_t *vrl_header;
    vrl_header = (VRL_Header_t *)buf;

    if (vrl_header->magicNumber == DX_VRL_MAGIC_NUMBER_DEFAULT_VALUE) {
        return OK;
    }
    else {
        return ERROR;
    }
}


/*
 *****************************************************************************
 * Prototype    : check_image_in_emmc
 * Description  : check image in emmc,use APIs of DX
 * Input        : secure_image_id: secure image id for vrl
 * Output       : None
 * Return Value : OK/ERROR
 * Author       : chenyingguo 61362
 *****************************************************************************
 */
int check_image_in_emmc(int secure_image_id)
{
    unsigned int vrl_addr;
    unsigned int vrl_addr2;
    DX_BIM_VRL_TABLE_t vrlList;
    unsigned long index;
    unsigned long *work_space;
    DxError_t ret = OK;

    if (0 == g_work_space) 
    {
        g_work_space = (unsigned long *)kmalloc(WORKSPACE_SIZE, GFP_KERNEL);
        if (0 == g_work_space) 
        {
            return ERROR;
        }
    }

    work_space = g_work_space;


    /* ��鰲ȫӳ��ID */
    if ((secure_image_id < 0) || (secure_image_id >= (int)ID_BUTTOM)) {
        printk(KERN_ERR"secure image id 0x%x is invalid!\n", secure_image_id);
        return ERROR;
    }

    /* ���ݰ�ȫӳ��ID���ҵ�ӳ���Ӧ��VRL��ptable�еĵ�ַ */
    vrl_addr  = VRL_AREA1_OFFSET + (unsigned int)secure_image_id*VRL_SIZE;
    vrl_addr2 = VRL_AREA2_OFFSET + (unsigned int)secure_image_id*VRL_SIZE;
    if (ERROR == check_vrl(vrl_addr, vrl_addr2)) {
        printk(KERN_ERR"check_vrl failed, id is 0x%x!\n", secure_image_id);
        return ERROR;
    }

    /* ���vrlList�����ж���VRL�Ĵ洢��ַ */
    vrlList[0].VRL_Address = vrl_addr;
    vrlList[0].OTP_Key_Index = g_otp_key_index;
    vrlList[0].Magic_Number = 0;

    /* ����DX_BIV_SwImageVerification�����а�ȫУ��*/
    ret = DX_BIV_SwImageVerification((DX_SB_FlashRead)sec_emmc_read,
                                        0,
                                        SOC_SECENG_BASE_ADDR,
                                        vrlList,
                                        1,
                                        &index,
                                        work_space,
                                        WORKSPACE_SIZE);
    if(OK != ret) {
        printk(KERN_ERR"DX_BIV_SwImageVerification image id is 0x%x, return is 0x%x !\n",
                secure_image_id, ret);
    }
    
    return (int)ret;
}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


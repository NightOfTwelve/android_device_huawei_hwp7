/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
 
  This is an AUTO GENERATED file ! DON'T MODIFY MANUAL! 
  �� �� ��   : hi6620_oscar_vtl_vd_iomux_pins.h
  ��������   : 2014��5��7��
  
******************************************************************************/


#ifndef __HI6620_OSCAR_VTL_VD_IOMUX_PINS_H__
#define __HI6620_OSCAR_VTL_VD_IOMUX_PINS_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include <mach/platform.h>
#include <mach/io.h>
#include <linux/mux.h>
#include "iomux.h"

/*board id can not be used precompile*/
#define V110_VERSION

extern struct iomux_ops iomux_pin_ops;

#define IOMUX_IOMG(_iomg, _iomg_name, _iomg_reg, _func_array)\
static struct iomux_iomg _iomg = {\
    .name = _iomg_name,\
    .iomg_reg = (void __iomem    *)(IO_ADDRESS(REG_BASE_IOC_OFF) + _iomg_reg),\
    .regValue = _func_array,\
};

#define IOMUX_IOCG(_iocg, _iocg_name, _iocg_reg, _iocg_pud_mask, _iocg_drvstrength_mask)    \
static struct iomux_iocg _iocg = {\
    .name = _iocg_name,\
    .iocg_reg = (void __iomem    *)(IO_ADDRESS(REG_BASE_IOC_OFF) + _iocg_reg),\
    .iocg_pullupdown_mask = _iocg_pud_mask,\
    .iocg_drivestrength_mask = _iocg_drvstrength_mask,\
};

#define IOMUX_PIN(_iomux_pin, _iomux_pin_name, _pin_func, _pin_pull_updown, \
_pin_drive_strength, _pin_iomg, _pin_iocg)    \
static struct  iomux_pin _iomux_pin = {\
    .pin_name = _iomux_pin_name,\
    .pin_func  = _pin_func,\
    .pin_pull_updown = _pin_pull_updown,\
    .pin_drive_strength = _pin_drive_strength,\
    .ops  =  &iomux_pin_ops,\
    .pin_iomg = _pin_iomg,\
    .pin_iocg  = _pin_iocg,\
    .init = 0, \
};

#define PIN_TABLE(_pinname, _iomux_pin)    \
{\
    .pinname = _pinname,\
    .iomux_pin = _iomux_pin,\
}

/*define the iomg*/
static int iomg000_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, RESERVE, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg001_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, RESERVE, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg002_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, RESERVE, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg003_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, RESERVE, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg004_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, RESERVE, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg005_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, RESERVE, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg006_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, RESERVE, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg007_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg008_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg009_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg010_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg011_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg012_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg013_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg014_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg015_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg016_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg017_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg018_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg019_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg020_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg021_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg022_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg023_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg024_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg025_func_array_oscar_vtl_vd[] = {0, RESERVE, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg026_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg027_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg028_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg029_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg030_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg031_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg032_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg033_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg034_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg035_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg036_func_array_oscar_vtl_vd[] = {0, RESERVE, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg037_func_array_oscar_vtl_vd[] = {0, RESERVE, RESERVE, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg038_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg039_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg040_func_array_oscar_vtl_vd[] = {0, 1, 2, RESERVE, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg041_func_array_oscar_vtl_vd[] = {0, 1, 2, RESERVE, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg042_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg043_func_array_oscar_vtl_vd[] = {0, RESERVE, RESERVE, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg044_func_array_oscar_vtl_vd[] = {0, RESERVE, RESERVE, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg045_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg046_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg047_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg048_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg049_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg050_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg051_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg052_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg053_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg054_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, RESERVE, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg055_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg056_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg057_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, 5, RESERVE, RESERVE,  };
static int iomg058_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, 5, RESERVE, RESERVE,  };
static int iomg059_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg060_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg061_func_array_oscar_vtl_vd[] = {0, RESERVE, RESERVE, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg062_func_array_oscar_vtl_vd[] = {0, RESERVE, 2, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg063_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg064_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg065_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg066_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg067_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg068_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg069_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg070_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg071_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg072_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg073_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg074_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg075_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg076_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg077_func_array_oscar_vtl_vd[] = {0, RESERVE, RESERVE, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg078_func_array_oscar_vtl_vd[] = {0, RESERVE, RESERVE, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg079_func_array_oscar_vtl_vd[] = {0, RESERVE, RESERVE, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg080_func_array_oscar_vtl_vd[] = {0, RESERVE, RESERVE, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg081_func_array_oscar_vtl_vd[] = {0, RESERVE, RESERVE, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg082_func_array_oscar_vtl_vd[] = {0, RESERVE, RESERVE, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg083_func_array_oscar_vtl_vd[] = {0, RESERVE, RESERVE, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg084_func_array_oscar_vtl_vd[] = {0, RESERVE, RESERVE, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg085_func_array_oscar_vtl_vd[] = {0, RESERVE, RESERVE, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg086_func_array_oscar_vtl_vd[] = {0, RESERVE, RESERVE, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg087_func_array_oscar_vtl_vd[] = {0, RESERVE, RESERVE, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg088_func_array_oscar_vtl_vd[] = {0, RESERVE, RESERVE, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg089_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, 5, 6, RESERVE,  };
static int iomg090_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg091_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg092_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg093_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg094_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg095_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg096_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg097_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg098_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg099_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, RESERVE, RESERVE, RESERVE,  };
static int iomg100_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, 4, 5, 6, RESERVE,  };
static int iomg101_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, 4, 5, 6, RESERVE,  };
static int iomg102_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, 4, 5, 6, RESERVE,  };
static int iomg103_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, 5, 6, 7,  };
static int iomg104_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, 5, RESERVE, 7,  };
static int iomg105_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, 5, RESERVE, 7,  };
static int iomg106_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, 5, RESERVE, 7,  };
static int iomg107_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, 5, RESERVE, 7,  };
static int iomg108_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, 5, 6, 7,  };
static int iomg109_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, 5, 6, 7,  };
static int iomg110_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, 5, 6, 7,  };
static int iomg111_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, 5, 6, 7,  };
static int iomg112_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, 5, 6, 7,  };
static int iomg113_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, 5, RESERVE, 7,  };
static int iomg114_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, 5, RESERVE, 7,  };
static int iomg115_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, 5, 6, 7,  };
static int iomg116_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, 5, 6, 7,  };
static int iomg117_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, 5, 6, 7,  };
static int iomg118_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, 5, 6, 7,  };
static int iomg119_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, 5, 6, 7,  };
static int iomg120_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, 5, 6, 7,  };
static int iomg121_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, 5, 6, 7,  };
static int iomg122_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, 5, 6, 7,  };
static int iomg123_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, 5, 6, 7,  };
static int iomg124_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, 5, 6, 7,  };
static int iomg125_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, 5, 6, 7,  };
static int iomg126_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, 5, 6, 7,  };
static int iomg127_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, 5, 6, 7,  };
static int iomg128_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, 5, 6, 7,  };
static int iomg129_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, 5, 6, 7,  };
static int iomg130_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, 5, 6, 7,  };
static int iomg131_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, 5, 6, 7,  };
static int iomg132_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, 5, 6, 7,  };
static int iomg133_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, 5, 6, 7,  };
static int iomg134_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, 4, 5, 6, 7,  };
static int iomg135_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, 4, 5, 6, 7,  };
static int iomg136_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg137_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg138_func_array_oscar_vtl_vd[] = {0, RESERVE, RESERVE, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg139_func_array_oscar_vtl_vd[] = {0, RESERVE, RESERVE, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg140_func_array_oscar_vtl_vd[] = {0, RESERVE, RESERVE, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg141_func_array_oscar_vtl_vd[] = {0, RESERVE, RESERVE, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg142_func_array_oscar_vtl_vd[] = {0, RESERVE, RESERVE, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg143_func_array_oscar_vtl_vd[] = {0, RESERVE, RESERVE, 3, 4, RESERVE, RESERVE, RESERVE,  };
static int iomg144_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, 4, 5, 6, RESERVE,  };
static int iomg145_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, 4, 5, 6, RESERVE,  };
static int iomg146_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg147_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, 4, 5, 6, RESERVE,  };
static int iomg148_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg149_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, 4, 5, 6, RESERVE,  };
static int iomg150_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg151_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, 4, 5, 6, RESERVE,  };
static int iomg152_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, 4, 5, 6, RESERVE,  };
static int iomg153_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, 4, 5, 6, RESERVE,  };
static int iomg154_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg155_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg156_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, 4, 5, 6, RESERVE,  };
static int iomg157_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, 4, 5, 6, RESERVE,  };
static int iomg158_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg159_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg160_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg161_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg162_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg163_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg164_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg165_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg166_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg167_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg168_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg169_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg170_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg171_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg172_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg173_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg174_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg175_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg176_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg177_func_array_oscar_vtl_vd[] = {0, 1, RESERVE, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg178_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg179_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg180_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg181_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg182_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg183_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg184_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg185_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg186_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg187_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, 5, 6, RESERVE,  };
static int iomg188_func_array_oscar_vtl_vd[] = {0, 1, 2, 3, RESERVE, 5, 6, RESERVE,  };


IOMUX_IOMG(iomg000_oscar_vtl_vd, "iomg000", 0x000, iomg000_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg001_oscar_vtl_vd, "iomg001", 0x004, iomg001_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg002_oscar_vtl_vd, "iomg002", 0x008, iomg002_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg003_oscar_vtl_vd, "iomg003", 0x00C, iomg003_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg004_oscar_vtl_vd, "iomg004", 0x010, iomg004_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg005_oscar_vtl_vd, "iomg005", 0x014, iomg005_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg006_oscar_vtl_vd, "iomg006", 0x018, iomg006_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg007_oscar_vtl_vd, "iomg007", 0x2F0, iomg007_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg008_oscar_vtl_vd, "iomg008", 0x01C, iomg008_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg009_oscar_vtl_vd, "iomg009", 0x020, iomg009_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg010_oscar_vtl_vd, "iomg010", 0x024, iomg010_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg011_oscar_vtl_vd, "iomg011", 0x028, iomg011_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg012_oscar_vtl_vd, "iomg012", 0x02C, iomg012_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg013_oscar_vtl_vd, "iomg013", 0x030, iomg013_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg014_oscar_vtl_vd, "iomg014", 0x034, iomg014_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg015_oscar_vtl_vd, "iomg015", 0x038, iomg015_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg016_oscar_vtl_vd, "iomg016", 0x03C, iomg016_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg017_oscar_vtl_vd, "iomg017", 0x040, iomg017_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg018_oscar_vtl_vd, "iomg018", 0x044, iomg018_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg019_oscar_vtl_vd, "iomg019", 0x048, iomg019_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg020_oscar_vtl_vd, "iomg020", 0x04C, iomg020_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg021_oscar_vtl_vd, "iomg021", 0x050, iomg021_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg022_oscar_vtl_vd, "iomg022", 0x054, iomg022_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg023_oscar_vtl_vd, "iomg023", 0x058, iomg023_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg024_oscar_vtl_vd, "iomg024", 0x05C, iomg024_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg025_oscar_vtl_vd, "iomg025", 0x060, iomg025_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg026_oscar_vtl_vd, "iomg026", 0x064, iomg026_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg027_oscar_vtl_vd, "iomg027", 0x068, iomg027_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg028_oscar_vtl_vd, "iomg028", 0x06C, iomg028_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg029_oscar_vtl_vd, "iomg029", 0x070, iomg029_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg030_oscar_vtl_vd, "iomg030", 0x074, iomg030_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg031_oscar_vtl_vd, "iomg031", 0x078, iomg031_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg032_oscar_vtl_vd, "iomg032", 0x07C, iomg032_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg033_oscar_vtl_vd, "iomg033", 0x080, iomg033_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg034_oscar_vtl_vd, "iomg034", 0x084, iomg034_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg035_oscar_vtl_vd, "iomg035", 0x088, iomg035_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg036_oscar_vtl_vd, "iomg036", 0x08C, iomg036_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg037_oscar_vtl_vd, "iomg037", 0x090, iomg037_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg038_oscar_vtl_vd, "iomg038", 0x094, iomg038_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg039_oscar_vtl_vd, "iomg039", 0x098, iomg039_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg040_oscar_vtl_vd, "iomg040", 0x09C, iomg040_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg041_oscar_vtl_vd, "iomg041", 0x0A0, iomg041_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg042_oscar_vtl_vd, "iomg042", 0x0A4, iomg042_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg043_oscar_vtl_vd, "iomg043", 0x0A8, iomg043_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg044_oscar_vtl_vd, "iomg044", 0x0AC, iomg044_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg045_oscar_vtl_vd, "iomg045", 0x0B0, iomg045_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg046_oscar_vtl_vd, "iomg046", 0x0B4, iomg046_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg047_oscar_vtl_vd, "iomg047", 0x0B8, iomg047_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg048_oscar_vtl_vd, "iomg048", 0x0BC, iomg048_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg049_oscar_vtl_vd, "iomg049", 0x0C0, iomg049_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg050_oscar_vtl_vd, "iomg050", 0x0C4, iomg050_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg051_oscar_vtl_vd, "iomg051", 0x0C8, iomg051_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg052_oscar_vtl_vd, "iomg052", 0x0CC, iomg052_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg053_oscar_vtl_vd, "iomg053", 0x0D0, iomg053_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg054_oscar_vtl_vd, "iomg054", 0x0D4, iomg054_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg055_oscar_vtl_vd, "iomg055", 0x0D8, iomg055_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg056_oscar_vtl_vd, "iomg056", 0x0DC, iomg056_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg057_oscar_vtl_vd, "iomg057", 0x0E0, iomg057_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg058_oscar_vtl_vd, "iomg058", 0x0E4, iomg058_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg059_oscar_vtl_vd, "iomg059", 0x0E8, iomg059_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg060_oscar_vtl_vd, "iomg060", 0x0EC, iomg060_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg061_oscar_vtl_vd, "iomg061", 0x0F0, iomg061_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg062_oscar_vtl_vd, "iomg062", 0x0F4, iomg062_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg063_oscar_vtl_vd, "iomg063", 0x0F8, iomg063_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg064_oscar_vtl_vd, "iomg064", 0x0FC, iomg064_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg065_oscar_vtl_vd, "iomg065", 0x100, iomg065_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg066_oscar_vtl_vd, "iomg066", 0x104, iomg066_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg067_oscar_vtl_vd, "iomg067", 0x108, iomg067_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg068_oscar_vtl_vd, "iomg068", 0x10C, iomg068_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg069_oscar_vtl_vd, "iomg069", 0x110, iomg069_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg070_oscar_vtl_vd, "iomg070", 0x114, iomg070_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg071_oscar_vtl_vd, "iomg071", 0x118, iomg071_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg072_oscar_vtl_vd, "iomg072", 0x11C, iomg072_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg073_oscar_vtl_vd, "iomg073", 0x120, iomg073_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg074_oscar_vtl_vd, "iomg074", 0x124, iomg074_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg075_oscar_vtl_vd, "iomg075", 0x128, iomg075_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg076_oscar_vtl_vd, "iomg076", 0x12C, iomg076_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg077_oscar_vtl_vd, "iomg077", 0x130, iomg077_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg078_oscar_vtl_vd, "iomg078", 0x134, iomg078_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg079_oscar_vtl_vd, "iomg079", 0x138, iomg079_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg080_oscar_vtl_vd, "iomg080", 0x13C, iomg080_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg081_oscar_vtl_vd, "iomg081", 0x140, iomg081_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg082_oscar_vtl_vd, "iomg082", 0x144, iomg082_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg083_oscar_vtl_vd, "iomg083", 0x148, iomg083_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg084_oscar_vtl_vd, "iomg084", 0x14C, iomg084_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg085_oscar_vtl_vd, "iomg085", 0x150, iomg085_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg086_oscar_vtl_vd, "iomg086", 0x154, iomg086_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg087_oscar_vtl_vd, "iomg087", 0x158, iomg087_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg088_oscar_vtl_vd, "iomg088", 0x15C, iomg088_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg089_oscar_vtl_vd, "iomg089", 0x2EC, iomg089_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg090_oscar_vtl_vd, "iomg090", 0x160, iomg090_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg091_oscar_vtl_vd, "iomg091", 0x164, iomg091_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg092_oscar_vtl_vd, "iomg092", 0x168, iomg092_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg093_oscar_vtl_vd, "iomg093", 0x16C, iomg093_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg094_oscar_vtl_vd, "iomg094", 0x170, iomg094_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg095_oscar_vtl_vd, "iomg095", 0x174, iomg095_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg096_oscar_vtl_vd, "iomg096", 0x2E8, iomg096_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg097_oscar_vtl_vd, "iomg097", 0x178, iomg097_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg098_oscar_vtl_vd, "iomg098", 0x17C, iomg098_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg099_oscar_vtl_vd, "iomg099", 0x180, iomg099_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg100_oscar_vtl_vd, "iomg100", 0x184, iomg100_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg101_oscar_vtl_vd, "iomg101", 0x188, iomg101_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg102_oscar_vtl_vd, "iomg102", 0x18C, iomg102_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg103_oscar_vtl_vd, "iomg103", 0x190, iomg103_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg104_oscar_vtl_vd, "iomg104", 0x194, iomg104_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg105_oscar_vtl_vd, "iomg105", 0x198, iomg105_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg106_oscar_vtl_vd, "iomg106", 0x19C, iomg106_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg107_oscar_vtl_vd, "iomg107", 0x1A0, iomg107_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg108_oscar_vtl_vd, "iomg108", 0x1A4, iomg108_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg109_oscar_vtl_vd, "iomg109", 0x1A8, iomg109_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg110_oscar_vtl_vd, "iomg110", 0x1AC, iomg110_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg111_oscar_vtl_vd, "iomg111", 0x1B0, iomg111_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg112_oscar_vtl_vd, "iomg112", 0x1B4, iomg112_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg113_oscar_vtl_vd, "iomg113", 0x1B8, iomg113_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg114_oscar_vtl_vd, "iomg114", 0x1BC, iomg114_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg115_oscar_vtl_vd, "iomg115", 0x1C0, iomg115_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg116_oscar_vtl_vd, "iomg116", 0x1C4, iomg116_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg117_oscar_vtl_vd, "iomg117", 0x1C8, iomg117_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg118_oscar_vtl_vd, "iomg118", 0x1CC, iomg118_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg119_oscar_vtl_vd, "iomg119", 0x1D0, iomg119_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg120_oscar_vtl_vd, "iomg120", 0x1D4, iomg120_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg121_oscar_vtl_vd, "iomg121", 0x1D8, iomg121_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg122_oscar_vtl_vd, "iomg122", 0x1DC, iomg122_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg123_oscar_vtl_vd, "iomg123", 0x1E0, iomg123_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg124_oscar_vtl_vd, "iomg124", 0x1E4, iomg124_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg125_oscar_vtl_vd, "iomg125", 0x1E8, iomg125_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg126_oscar_vtl_vd, "iomg126", 0x1EC, iomg126_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg127_oscar_vtl_vd, "iomg127", 0x1F0, iomg127_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg128_oscar_vtl_vd, "iomg128", 0x1F4, iomg128_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg129_oscar_vtl_vd, "iomg129", 0x1F8, iomg129_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg130_oscar_vtl_vd, "iomg130", 0x1FC, iomg130_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg131_oscar_vtl_vd, "iomg131", 0x200, iomg131_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg132_oscar_vtl_vd, "iomg132", 0x204, iomg132_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg133_oscar_vtl_vd, "iomg133", 0x208, iomg133_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg134_oscar_vtl_vd, "iomg134", 0x20C, iomg134_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg135_oscar_vtl_vd, "iomg135", 0x210, iomg135_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg136_oscar_vtl_vd, "iomg136", 0x214, iomg136_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg137_oscar_vtl_vd, "iomg137", 0x218, iomg137_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg138_oscar_vtl_vd, "iomg138", 0x21C, iomg138_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg139_oscar_vtl_vd, "iomg139", 0x220, iomg139_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg140_oscar_vtl_vd, "iomg140", 0x224, iomg140_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg141_oscar_vtl_vd, "iomg141", 0x228, iomg141_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg142_oscar_vtl_vd, "iomg142", 0x22C, iomg142_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg143_oscar_vtl_vd, "iomg143", 0x230, iomg143_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg144_oscar_vtl_vd, "iomg144", 0x234, iomg144_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg145_oscar_vtl_vd, "iomg145", 0x238, iomg145_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg146_oscar_vtl_vd, "iomg146", 0x23C, iomg146_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg147_oscar_vtl_vd, "iomg147", 0x240, iomg147_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg148_oscar_vtl_vd, "iomg148", 0x244, iomg148_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg149_oscar_vtl_vd, "iomg149", 0x248, iomg149_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg150_oscar_vtl_vd, "iomg150", 0x24C, iomg150_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg151_oscar_vtl_vd, "iomg151", 0x250, iomg151_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg152_oscar_vtl_vd, "iomg152", 0x254, iomg152_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg153_oscar_vtl_vd, "iomg153", 0x258, iomg153_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg154_oscar_vtl_vd, "iomg154", 0x25C, iomg154_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg155_oscar_vtl_vd, "iomg155", 0x260, iomg155_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg156_oscar_vtl_vd, "iomg156", 0x264, iomg156_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg157_oscar_vtl_vd, "iomg157", 0x268, iomg157_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg158_oscar_vtl_vd, "iomg158", 0x26C, iomg158_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg159_oscar_vtl_vd, "iomg159", 0x270, iomg159_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg160_oscar_vtl_vd, "iomg160", 0x274, iomg160_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg161_oscar_vtl_vd, "iomg161", 0x278, iomg161_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg162_oscar_vtl_vd, "iomg162", 0x27C, iomg162_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg163_oscar_vtl_vd, "iomg163", 0x280, iomg163_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg164_oscar_vtl_vd, "iomg164", 0x284, iomg164_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg165_oscar_vtl_vd, "iomg165", 0x288, iomg165_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg166_oscar_vtl_vd, "iomg166", 0x28C, iomg166_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg167_oscar_vtl_vd, "iomg167", 0x290, iomg167_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg168_oscar_vtl_vd, "iomg168", 0x294, iomg168_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg169_oscar_vtl_vd, "iomg169", 0x298, iomg169_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg170_oscar_vtl_vd, "iomg170", 0x29C, iomg170_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg171_oscar_vtl_vd, "iomg171", 0x2A0, iomg171_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg172_oscar_vtl_vd, "iomg172", 0x2A4, iomg172_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg173_oscar_vtl_vd, "iomg173", 0x2A8, iomg173_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg174_oscar_vtl_vd, "iomg174", 0x2AC, iomg174_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg175_oscar_vtl_vd, "iomg175", 0x2B0, iomg175_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg176_oscar_vtl_vd, "iomg176", 0x2B4, iomg176_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg177_oscar_vtl_vd, "iomg177", 0x2B8, iomg177_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg178_oscar_vtl_vd, "iomg178", 0x2BC, iomg178_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg179_oscar_vtl_vd, "iomg179", 0x2C0, iomg179_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg180_oscar_vtl_vd, "iomg180", 0x2C4, iomg180_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg181_oscar_vtl_vd, "iomg181", 0x2C8, iomg181_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg182_oscar_vtl_vd, "iomg182", 0x2CC, iomg182_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg183_oscar_vtl_vd, "iomg183", 0x2D0, iomg183_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg184_oscar_vtl_vd, "iomg184", 0x2D4, iomg184_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg185_oscar_vtl_vd, "iomg185", 0x2D8, iomg185_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg186_oscar_vtl_vd, "iomg186", 0x2DC, iomg186_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg187_oscar_vtl_vd, "iomg187", 0x2E0, iomg187_func_array_oscar_vtl_vd)
IOMUX_IOMG(iomg188_oscar_vtl_vd, "iomg188", 0x2E4, iomg188_func_array_oscar_vtl_vd)


/*define pins*/
IOMUX_IOCG(iocg000_oscar_vtl_vd, "iocg000", 0x800, 0x0003, 0x00F0)
IOMUX_IOCG(iocg001_oscar_vtl_vd, "iocg001", 0x804, 0x0003, 0x00F0)
IOMUX_IOCG(iocg002_oscar_vtl_vd, "iocg002", 0x808, 0x0003, 0x00F0)
IOMUX_IOCG(iocg003_oscar_vtl_vd, "iocg003", 0x80C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg004_oscar_vtl_vd, "iocg004", 0x810, 0x0003, 0x00F0)
IOMUX_IOCG(iocg005_oscar_vtl_vd, "iocg005", 0x814, 0x0003, 0x00F0)
IOMUX_IOCG(iocg006_oscar_vtl_vd, "iocg006", 0x818, 0x0003, 0x00F0)
IOMUX_IOCG(iocg007_oscar_vtl_vd, "iocg007", 0x81C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg008_oscar_vtl_vd, "iocg008", 0x820, 0x0003, 0x00F0)
IOMUX_IOCG(iocg009_oscar_vtl_vd, "iocg009", 0x824, 0x0003, 0x00F0)
IOMUX_IOCG(iocg010_oscar_vtl_vd, "iocg010", 0x828, 0x0003, 0x00F0)
IOMUX_IOCG(iocg011_oscar_vtl_vd, "iocg011", 0x82C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg012_oscar_vtl_vd, "iocg012", 0x830, 0x0003, 0x00F0)
IOMUX_IOCG(iocg013_oscar_vtl_vd, "iocg013", 0x834, 0x0003, 0x00F0)
IOMUX_IOCG(iocg014_oscar_vtl_vd, "iocg014", 0x838, 0x0003, 0x00F0)
IOMUX_IOCG(iocg015_oscar_vtl_vd, "iocg015", 0x83C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg016_oscar_vtl_vd, "iocg016", 0x840, 0x0003, 0x00F0)
IOMUX_IOCG(iocg017_oscar_vtl_vd, "iocg017", 0x844, 0x0003, 0x00F0)
IOMUX_IOCG(iocg018_oscar_vtl_vd, "iocg018", 0x848, 0x0003, 0x00F0)
IOMUX_IOCG(iocg019_oscar_vtl_vd, "iocg019", 0x84C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg020_oscar_vtl_vd, "iocg020", 0x850, 0x0003, 0x00F0)
IOMUX_IOCG(iocg021_oscar_vtl_vd, "iocg021", 0x854, 0x0003, 0x00F0)
IOMUX_IOCG(iocg022_oscar_vtl_vd, "iocg022", 0x858, 0x0003, 0x00F0)
IOMUX_IOCG(iocg023_oscar_vtl_vd, "iocg023", 0x85C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg024_oscar_vtl_vd, "iocg024", 0x860, 0x0003, 0x00F0)
IOMUX_IOCG(iocg025_oscar_vtl_vd, "iocg025", 0x864, 0x0003, 0x00F0)
IOMUX_IOCG(iocg026_oscar_vtl_vd, "iocg026", 0x868, 0x0003, 0x00F0)
IOMUX_IOCG(iocg027_oscar_vtl_vd, "iocg027", 0x86C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg028_oscar_vtl_vd, "iocg028", 0x870, 0x0003, 0x00F0)
IOMUX_IOCG(iocg029_oscar_vtl_vd, "iocg029", 0x874, 0x0003, 0x00F0)
IOMUX_IOCG(iocg030_oscar_vtl_vd, "iocg030", 0x878, 0x0003, 0x00F0)
IOMUX_IOCG(iocg031_oscar_vtl_vd, "iocg031", 0x87C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg032_oscar_vtl_vd, "iocg032", 0x880, 0x0003, 0x00F0)
IOMUX_IOCG(iocg033_oscar_vtl_vd, "iocg033", 0x884, 0x0003, 0x00F0)
IOMUX_IOCG(iocg034_oscar_vtl_vd, "iocg034", 0x888, 0x0003, 0x00F0)
IOMUX_IOCG(iocg035_oscar_vtl_vd, "iocg035", 0x88C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg036_oscar_vtl_vd, "iocg036", 0x890, 0x0003, 0x00F0)
IOMUX_IOCG(iocg037_oscar_vtl_vd, "iocg037", 0x894, 0x0003, 0x00F0)
IOMUX_IOCG(iocg038_oscar_vtl_vd, "iocg038", 0x898, 0x0003, 0x00F0)
IOMUX_IOCG(iocg039_oscar_vtl_vd, "iocg039", 0x89C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg040_oscar_vtl_vd, "iocg040", 0x8A0, 0x0003, 0x00F0)
IOMUX_IOCG(iocg041_oscar_vtl_vd, "iocg041", 0x8A4, 0x0003, 0x00F0)
IOMUX_IOCG(iocg042_oscar_vtl_vd, "iocg042", 0x8A8, 0x0003, 0x00F0)
IOMUX_IOCG(iocg043_oscar_vtl_vd, "iocg043", 0x8AC, 0x0003, 0x00F0)
IOMUX_IOCG(iocg044_oscar_vtl_vd, "iocg044", 0x8B0, 0x0003, 0x00F0)
IOMUX_IOCG(iocg045_oscar_vtl_vd, "iocg045", 0x8B4, 0x0003, 0x00F0)
IOMUX_IOCG(iocg046_oscar_vtl_vd, "iocg046", 0x8B8, 0x0003, 0x00F0)
IOMUX_IOCG(iocg047_oscar_vtl_vd, "iocg047", 0x8BC, 0x0003, 0x00F0)
IOMUX_IOCG(iocg048_oscar_vtl_vd, "iocg048", 0x8C0, 0x0003, 0x00F0)
IOMUX_IOCG(iocg049_oscar_vtl_vd, "iocg049", 0x8C4, 0x0003, 0x00F0)
IOMUX_IOCG(iocg050_oscar_vtl_vd, "iocg050", 0x8C8, 0x0003, 0x00F0)
IOMUX_IOCG(iocg051_oscar_vtl_vd, "iocg051", 0x8CC, 0x0003, 0x00F0)
IOMUX_IOCG(iocg052_oscar_vtl_vd, "iocg052", 0x8D0, 0x0003, 0x00F0)
IOMUX_IOCG(iocg053_oscar_vtl_vd, "iocg053", 0x8D4, 0x0003, 0x00F0)
IOMUX_IOCG(iocg054_oscar_vtl_vd, "iocg054", 0x8D8, 0x0003, 0x00F0)
IOMUX_IOCG(iocg055_oscar_vtl_vd, "iocg055", 0x8DC, 0x0003, 0x00F0)
IOMUX_IOCG(iocg056_oscar_vtl_vd, "iocg056", 0x8E0, 0x0003, 0x00F0)
IOMUX_IOCG(iocg057_oscar_vtl_vd, "iocg057", 0x8E4, 0x0003, 0x00F0)
IOMUX_IOCG(iocg058_oscar_vtl_vd, "iocg058", 0x8E8, 0x0003, 0x00F0)
IOMUX_IOCG(iocg059_oscar_vtl_vd, "iocg059", 0x8EC, 0x0003, 0x00F0)
IOMUX_IOCG(iocg060_oscar_vtl_vd, "iocg060", 0x8F0, 0x0003, 0x00F0)
IOMUX_IOCG(iocg061_oscar_vtl_vd, "iocg061", 0x8F4, 0x0003, 0x00F0)
IOMUX_IOCG(iocg062_oscar_vtl_vd, "iocg062", 0x8F8, 0x0003, 0x00F0)
IOMUX_IOCG(iocg063_oscar_vtl_vd, "iocg063", 0x8FC, 0x0003, 0x00F0)
IOMUX_IOCG(iocg064_oscar_vtl_vd, "iocg064", 0x900, 0x0003, 0x00F0)
IOMUX_IOCG(iocg065_oscar_vtl_vd, "iocg065", 0x904, 0x0003, 0x00F0)
IOMUX_IOCG(iocg066_oscar_vtl_vd, "iocg066", 0x908, 0x0003, 0x00F0)
IOMUX_IOCG(iocg067_oscar_vtl_vd, "iocg067", 0x90C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg068_oscar_vtl_vd, "iocg068", 0x910, 0x0003, 0x00F0)
IOMUX_IOCG(iocg069_oscar_vtl_vd, "iocg069", 0x914, 0x0003, 0x00F0)
IOMUX_IOCG(iocg070_oscar_vtl_vd, "iocg070", 0x918, 0x0003, 0x00F0)
IOMUX_IOCG(iocg071_oscar_vtl_vd, "iocg071", 0x91C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg072_oscar_vtl_vd, "iocg072", 0x920, 0x0003, 0x00F0)
IOMUX_IOCG(iocg073_oscar_vtl_vd, "iocg073", 0x924, 0x0003, 0x00F0)
IOMUX_IOCG(iocg074_oscar_vtl_vd, "iocg074", 0x928, 0x0003, 0x00F0)
IOMUX_IOCG(iocg075_oscar_vtl_vd, "iocg075", 0x92C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg076_oscar_vtl_vd, "iocg076", 0x930, 0x0003, 0x00F0)
IOMUX_IOCG(iocg077_oscar_vtl_vd, "iocg077", 0x934, 0x0003, 0x00F0)
IOMUX_IOCG(iocg078_oscar_vtl_vd, "iocg078", 0x938, 0x0003, 0x00F0)
IOMUX_IOCG(iocg079_oscar_vtl_vd, "iocg079", 0x93C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg080_oscar_vtl_vd, "iocg080", 0x940, 0x0003, 0x00F0)
IOMUX_IOCG(iocg081_oscar_vtl_vd, "iocg081", 0x944, 0x0003, 0x00F0)
IOMUX_IOCG(iocg082_oscar_vtl_vd, "iocg082", 0x948, 0x0003, 0x00F0)
IOMUX_IOCG(iocg083_oscar_vtl_vd, "iocg083", 0x94C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg084_oscar_vtl_vd, "iocg084", 0x950, 0x0003, 0x00F0)
IOMUX_IOCG(iocg085_oscar_vtl_vd, "iocg085", 0x954, 0x0003, 0x00F0)
IOMUX_IOCG(iocg086_oscar_vtl_vd, "iocg086", 0x958, 0x0003, 0x00F0)
IOMUX_IOCG(iocg087_oscar_vtl_vd, "iocg087", 0x95C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg088_oscar_vtl_vd, "iocg088", 0x960, 0x0003, 0x00F0)
IOMUX_IOCG(iocg089_oscar_vtl_vd, "iocg089", 0x964, 0x0003, 0x00F0)
IOMUX_IOCG(iocg090_oscar_vtl_vd, "iocg090", 0x968, 0x0003, 0x00F0)
IOMUX_IOCG(iocg091_oscar_vtl_vd, "iocg091", 0x96C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg092_oscar_vtl_vd, "iocg092", 0x970, 0x0003, 0x00F0)
IOMUX_IOCG(iocg093_oscar_vtl_vd, "iocg093", 0x974, 0x0003, 0x00F0)
IOMUX_IOCG(iocg094_oscar_vtl_vd, "iocg094", 0x978, 0x0003, 0x00F0)
IOMUX_IOCG(iocg095_oscar_vtl_vd, "iocg095", 0x97C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg096_oscar_vtl_vd, "iocg096", 0x980, 0x0003, 0x00F0)
IOMUX_IOCG(iocg097_oscar_vtl_vd, "iocg097", 0x984, 0x0003, 0x00F0)
IOMUX_IOCG(iocg098_oscar_vtl_vd, "iocg098", 0x988, 0x0003, 0x00F0)
IOMUX_IOCG(iocg099_oscar_vtl_vd, "iocg099", 0x98C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg100_oscar_vtl_vd, "iocg100", 0x990, 0x0003, 0x00F0)
IOMUX_IOCG(iocg101_oscar_vtl_vd, "iocg101", 0x994, 0x0003, 0x00F0)
IOMUX_IOCG(iocg102_oscar_vtl_vd, "iocg102", 0x998, 0x0003, 0x00F0)
IOMUX_IOCG(iocg103_oscar_vtl_vd, "iocg103", 0x99C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg104_oscar_vtl_vd, "iocg104", 0x9A0, 0x0003, 0x00F0)
IOMUX_IOCG(iocg105_oscar_vtl_vd, "iocg105", 0x9A4, 0x0003, 0x00F0)
IOMUX_IOCG(iocg106_oscar_vtl_vd, "iocg106", 0x9A8, 0x0003, 0x00F0)
IOMUX_IOCG(iocg107_oscar_vtl_vd, "iocg107", 0x9AC, 0x0003, 0x00F0)
IOMUX_IOCG(iocg108_oscar_vtl_vd, "iocg108", 0x9B0, 0x0003, 0x00F0)
IOMUX_IOCG(iocg109_oscar_vtl_vd, "iocg109", 0x9B4, 0x0003, 0x00F0)
IOMUX_IOCG(iocg110_oscar_vtl_vd, "iocg110", 0x9B8, 0x0003, 0x00F0)
IOMUX_IOCG(iocg111_oscar_vtl_vd, "iocg111", 0x9BC, 0x0003, 0x00F0)
IOMUX_IOCG(iocg112_oscar_vtl_vd, "iocg112", 0x9C0, 0x0003, 0x00F0)
IOMUX_IOCG(iocg113_oscar_vtl_vd, "iocg113", 0x9C4, 0x0003, 0x00F0)
IOMUX_IOCG(iocg114_oscar_vtl_vd, "iocg114", 0x9C8, 0x0003, 0x00F0)
IOMUX_IOCG(iocg115_oscar_vtl_vd, "iocg115", 0x9CC, 0x0003, 0x00F0)
IOMUX_IOCG(iocg116_oscar_vtl_vd, "iocg116", 0x9D0, 0x0003, 0x00F0)
IOMUX_IOCG(iocg117_oscar_vtl_vd, "iocg117", 0x9D4, 0x0003, 0x00F0)
IOMUX_IOCG(iocg118_oscar_vtl_vd, "iocg118", 0x9D8, 0x0003, 0x00F0)
IOMUX_IOCG(iocg119_oscar_vtl_vd, "iocg119", 0x9DC, 0x0003, 0x00F0)
IOMUX_IOCG(iocg120_oscar_vtl_vd, "iocg120", 0x9E0, 0x0003, 0x00F0)
IOMUX_IOCG(iocg121_oscar_vtl_vd, "iocg121", 0x9E4, 0x0003, 0x00F0)
IOMUX_IOCG(iocg122_oscar_vtl_vd, "iocg122", 0x9E8, 0x0003, 0x00F0)
IOMUX_IOCG(iocg123_oscar_vtl_vd, "iocg123", 0x9EC, 0x0003, 0x00F0)
IOMUX_IOCG(iocg124_oscar_vtl_vd, "iocg124", 0x9F0, 0x0003, 0x00F0)
IOMUX_IOCG(iocg125_oscar_vtl_vd, "iocg125", 0x9F4, 0x0003, 0x00F0)
IOMUX_IOCG(iocg126_oscar_vtl_vd, "iocg126", 0x9F8, 0x0003, 0x00F0)
IOMUX_IOCG(iocg127_oscar_vtl_vd, "iocg127", 0x9FC, 0x0003, 0x00F0)
IOMUX_IOCG(iocg128_oscar_vtl_vd, "iocg128", 0xA00, 0x0003, 0x00F0)
IOMUX_IOCG(iocg129_oscar_vtl_vd, "iocg129", 0xA04, 0x0003, 0x00F0)
IOMUX_IOCG(iocg130_oscar_vtl_vd, "iocg130", 0xA08, 0x0003, 0x00F0)
IOMUX_IOCG(iocg131_oscar_vtl_vd, "iocg131", 0xA0C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg132_oscar_vtl_vd, "iocg132", 0xA10, 0x0003, 0x00F0)
IOMUX_IOCG(iocg133_oscar_vtl_vd, "iocg133", 0xA14, 0x0003, 0x00F0)
IOMUX_IOCG(iocg134_oscar_vtl_vd, "iocg134", 0xA18, 0x0003, 0x00F0)
IOMUX_IOCG(iocg135_oscar_vtl_vd, "iocg135", 0xA1C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg136_oscar_vtl_vd, "iocg136", 0xA20, 0x0003, 0x00F0)
IOMUX_IOCG(iocg137_oscar_vtl_vd, "iocg137", 0xA24, 0x0003, 0x00F0)
IOMUX_IOCG(iocg138_oscar_vtl_vd, "iocg138", 0xA28, 0x0003, 0x00F0)
IOMUX_IOCG(iocg139_oscar_vtl_vd, "iocg139", 0xA2C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg140_oscar_vtl_vd, "iocg140", 0xA30, 0x0003, 0x00F0)
IOMUX_IOCG(iocg141_oscar_vtl_vd, "iocg141", 0xA34, 0x0003, 0x00F0)
IOMUX_IOCG(iocg142_oscar_vtl_vd, "iocg142", 0xA38, 0x0003, 0x00F0)
IOMUX_IOCG(iocg143_oscar_vtl_vd, "iocg143", 0xA3C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg144_oscar_vtl_vd, "iocg144", 0xA40, 0x0003, 0x00F0)
IOMUX_IOCG(iocg145_oscar_vtl_vd, "iocg145", 0xA44, 0x0003, 0x00F0)
IOMUX_IOCG(iocg146_oscar_vtl_vd, "iocg146", 0xA48, 0x0003, 0x00F0)
IOMUX_IOCG(iocg147_oscar_vtl_vd, "iocg147", 0xA4C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg148_oscar_vtl_vd, "iocg148", 0xA50, 0x0003, 0x00F0)
IOMUX_IOCG(iocg149_oscar_vtl_vd, "iocg149", 0xA54, 0x0003, 0x00F0)
IOMUX_IOCG(iocg150_oscar_vtl_vd, "iocg150", 0xA58, 0x0003, 0x00F0)
IOMUX_IOCG(iocg151_oscar_vtl_vd, "iocg151", 0xA5C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg152_oscar_vtl_vd, "iocg152", 0xA60, 0x0003, 0x00F0)
IOMUX_IOCG(iocg153_oscar_vtl_vd, "iocg153", 0xA64, 0x0003, 0x00F0)
IOMUX_IOCG(iocg154_oscar_vtl_vd, "iocg154", 0xA68, 0x0003, 0x00F0)
IOMUX_IOCG(iocg155_oscar_vtl_vd, "iocg155", 0xA6C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg156_oscar_vtl_vd, "iocg156", 0xA70, 0x0003, 0x00F0)
IOMUX_IOCG(iocg157_oscar_vtl_vd, "iocg157", 0xA74, 0x0003, 0x00F0)
IOMUX_IOCG(iocg158_oscar_vtl_vd, "iocg158", 0xA78, 0x0003, 0x00F0)
IOMUX_IOCG(iocg159_oscar_vtl_vd, "iocg159", 0xA7C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg160_oscar_vtl_vd, "iocg160", 0xA80, 0x0003, 0x00F0)
IOMUX_IOCG(iocg161_oscar_vtl_vd, "iocg161", 0xA84, 0x0003, 0x00F0)
IOMUX_IOCG(iocg162_oscar_vtl_vd, "iocg162", 0xA88, 0x0003, 0x00F0)
IOMUX_IOCG(iocg163_oscar_vtl_vd, "iocg163", 0xA8C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg164_oscar_vtl_vd, "iocg164", 0xA90, 0x0003, 0x00F0)
IOMUX_IOCG(iocg165_oscar_vtl_vd, "iocg165", 0xA94, 0x0003, 0x00F0)
IOMUX_IOCG(iocg166_oscar_vtl_vd, "iocg166", 0xA98, 0x0003, 0x00F0)
IOMUX_IOCG(iocg167_oscar_vtl_vd, "iocg167", 0xA9C, 0x0003, 0x00F0)
IOMUX_IOCG(iocg168_oscar_vtl_vd, "iocg168", 0xAA0, 0x0003, 0x00F0)
IOMUX_IOCG(iocg169_oscar_vtl_vd, "iocg169", 0xAA4, 0x0003, 0x00F0)
IOMUX_IOCG(iocg170_oscar_vtl_vd, "iocg170", 0xAA8, 0x0003, 0x00F0)
IOMUX_IOCG(iocg171_oscar_vtl_vd, "iocg171", 0xAAC, 0x0003, 0x00F0)
IOMUX_IOCG(iocg172_oscar_vtl_vd, "iocg172", 0xAB0, 0x0003, 0x00F0)
IOMUX_IOCG(iocg173_oscar_vtl_vd, "iocg173", 0xAB4, 0x0003, 0x00F0)
IOMUX_IOCG(iocg174_oscar_vtl_vd, "iocg174", 0xAB8, 0x0003, 0x00F0)
IOMUX_IOCG(iocg175_oscar_vtl_vd, "iocg175", 0xABC, 0x0003, 0x00F0)
IOMUX_IOCG(iocg176_oscar_vtl_vd, "iocg176", 0xAC0, 0x0003, 0x00F0)
IOMUX_IOCG(iocg177_oscar_vtl_vd, "iocg177", 0xAC4, 0x0003, 0x00F0)
IOMUX_IOCG(iocg178_oscar_vtl_vd, "iocg178", 0xAC8, 0x0003, 0x00F0)
IOMUX_IOCG(iocg179_oscar_vtl_vd, "iocg179", 0xACC, 0x0003, 0x00F0)
IOMUX_IOCG(iocg180_oscar_vtl_vd, "iocg180", 0xAD0, 0x0003, 0x00F0)
IOMUX_IOCG(iocg181_oscar_vtl_vd, "iocg181", 0xAD4, 0x0003, 0x00F0)
IOMUX_IOCG(iocg182_oscar_vtl_vd, "iocg182", 0xAD8, 0x0003, 0x00F0)
IOMUX_IOCG(iocg183_oscar_vtl_vd, "iocg183", 0xADC, 0x0003, 0x00F0)
IOMUX_IOCG(iocg184_oscar_vtl_vd, "iocg184", 0xAE0, 0x0003, 0x00F0)
IOMUX_IOCG(iocg185_oscar_vtl_vd, "iocg185", 0xAE4, 0x0003, 0x00F0)
IOMUX_IOCG(iocg186_oscar_vtl_vd, "iocg186", 0xAE8, 0x0003, 0x00F0)
IOMUX_IOCG(iocg187_oscar_vtl_vd, "iocg187", 0xAEC, 0x0003, 0x00F0)
IOMUX_IOCG(iocg188_oscar_vtl_vd, "iocg188", 0xAF0, 0x0003, 0x00F0)
IOMUX_IOCG(iocg189_oscar_vtl_vd, "iocg189", 0xAF4, 0x0003, 0x00F0)
IOMUX_IOCG(iocg190_oscar_vtl_vd, "iocg190", 0xAF8, 0x0003, 0x00F0)
IOMUX_IOCG(iocg191_oscar_vtl_vd, "iocg191", 0xAFC, 0x0003, 0x00F0)


/*define the iocg*/
IOMUX_PIN(I2C0_SCL_OSCAR_VTL_VD, "I2C0_SCL", FUNC0, NOPULL, LEVEL0, &iomg071_oscar_vtl_vd, &iocg071_oscar_vtl_vd)
IOMUX_PIN(I2C0_SDA_OSCAR_VTL_VD, "I2C0_SDA", FUNC0, NOPULL, LEVEL0, &iomg072_oscar_vtl_vd, &iocg072_oscar_vtl_vd)
IOMUX_PIN(I2C1_SCL_OSCAR_VTL_VD, "I2C1_SCL", FUNC0, NOPULL, LEVEL0, &iomg073_oscar_vtl_vd, &iocg073_oscar_vtl_vd)
IOMUX_PIN(I2C1_SDA_OSCAR_VTL_VD, "I2C1_SDA", FUNC0, NOPULL, LEVEL0, &iomg074_oscar_vtl_vd, &iocg074_oscar_vtl_vd)
IOMUX_PIN(I2C2_SCL_OSCAR_VTL_VD, "I2C2_SCL", FUNC0, NOPULL, LEVEL0, &iomg075_oscar_vtl_vd, &iocg075_oscar_vtl_vd)
IOMUX_PIN(I2C2_SDA_OSCAR_VTL_VD, "I2C2_SDA", FUNC0, NOPULL, LEVEL0, &iomg076_oscar_vtl_vd, &iocg076_oscar_vtl_vd)
IOMUX_PIN(I2C3_SCL_OSCAR_VTL_VD, "I2C3_SCL", FUNC0, NOPULL, LEVEL0, &iomg077_oscar_vtl_vd, &iocg077_oscar_vtl_vd)
IOMUX_PIN(I2C3_SDA_OSCAR_VTL_VD, "I2C3_SDA", FUNC0, NOPULL, LEVEL0, &iomg078_oscar_vtl_vd, &iocg078_oscar_vtl_vd)
IOMUX_PIN(ISP_GPIO0_OSCAR_VTL_VD, "ISP_GPIO0", FUNC0, NOPULL, LEVEL0, &iomg008_oscar_vtl_vd, &iocg008_oscar_vtl_vd)
IOMUX_PIN(ISP_GPIO2_OSCAR_VTL_VD, "ISP_GPIO2", FUNC0, NOPULL, LEVEL0, &iomg010_oscar_vtl_vd, &iocg010_oscar_vtl_vd)
IOMUX_PIN(ISP_GPIO3_OSCAR_VTL_VD, "ISP_GPIO3", FUNC2, NOPULL, LEVEL0, &iomg011_oscar_vtl_vd, &iocg011_oscar_vtl_vd)
IOMUX_PIN(ISP_RESETB0_OSCAR_VTL_VD, "ISP_RESETB0", FUNC0, NOPULL, LEVEL0, &iomg016_oscar_vtl_vd, &iocg016_oscar_vtl_vd)
IOMUX_PIN(ISP_CCLK0_OSCAR_VTL_VD, "ISP_CCLK0", FUNC0, NOPULL, LEVEL0, &iomg014_oscar_vtl_vd, &iocg014_oscar_vtl_vd)
IOMUX_PIN(ISP_SDA0_OSCAR_VTL_VD, "ISP_SDA0", FUNC2, PULLDOWN, LEVEL0, &iomg020_oscar_vtl_vd, &iocg020_oscar_vtl_vd)
IOMUX_PIN(ISP_SCL0_OSCAR_VTL_VD, "ISP_SCL0", FUNC2, PULLDOWN, LEVEL0, &iomg021_oscar_vtl_vd, &iocg021_oscar_vtl_vd)
IOMUX_PIN(ISP_GPIO1_OSCAR_VTL_VD, "ISP_GPIO1", FUNC0, NOPULL, LEVEL0, &iomg009_oscar_vtl_vd, &iocg009_oscar_vtl_vd)
IOMUX_PIN(ISP_GPIO4_OSCAR_VTL_VD, "ISP_GPIO4", FUNC2, NOPULL, LEVEL0, &iomg012_oscar_vtl_vd, &iocg012_oscar_vtl_vd)
IOMUX_PIN(ISP_RESETB1_OSCAR_VTL_VD, "ISP_RESETB1", FUNC0, NOPULL, LEVEL0, &iomg017_oscar_vtl_vd, &iocg017_oscar_vtl_vd)
IOMUX_PIN(ISP_CCLK1_OSCAR_VTL_VD, "ISP_CCLK1", FUNC0, NOPULL, LEVEL0, &iomg015_oscar_vtl_vd, &iocg015_oscar_vtl_vd)
IOMUX_PIN(ISP_SDA1_OSCAR_VTL_VD, "ISP_SDA1", FUNC2, PULLDOWN, LEVEL0, &iomg022_oscar_vtl_vd, &iocg022_oscar_vtl_vd)
IOMUX_PIN(ISP_SCL1_OSCAR_VTL_VD, "ISP_SCL1", FUNC2, PULLDOWN, LEVEL0, &iomg023_oscar_vtl_vd, &iocg023_oscar_vtl_vd)
IOMUX_PIN(ISP_GPIO5_OSCAR_VTL_VD, "ISP_GPIO5", FUNC2, NOPULL, LEVEL0, &iomg013_oscar_vtl_vd, &iocg013_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_20_OSCAR_VTL_VD, "RF_GPIO_20", FUNC2, NOPULL, LEVEL0, &iomg178_oscar_vtl_vd, &iocg188_oscar_vtl_vd)
IOMUX_PIN(GPIO_4_2_OSCAR_VTL_VD, "GPIO_4_2", FUNC0, PULLUP, LEVEL0, &iomg130_oscar_vtl_vd, &iocg130_oscar_vtl_vd)
IOMUX_PIN(GPIO_4_3_OSCAR_VTL_VD, "GPIO_4_3", FUNC0, PULLUP, LEVEL0, &iomg131_oscar_vtl_vd, &iocg131_oscar_vtl_vd)
IOMUX_PIN(PWM_OUT1_OSCAR_VTL_VD, "PWM_OUT1", FUNC2, NOPULL, LEVEL0, &iomg025_oscar_vtl_vd, &iocg025_oscar_vtl_vd)
IOMUX_PIN(GPIO_4_6_OSCAR_VTL_VD, "GPIO_4_6", FUNC0, NOPULL, LEVEL0, &iomg134_oscar_vtl_vd, &iocg134_oscar_vtl_vd)
IOMUX_PIN(GPIO_4_7_OSCAR_VTL_VD, "GPIO_4_7", FUNC4, NOPULL, LEVEL0, &iomg135_oscar_vtl_vd, &iocg135_oscar_vtl_vd)
IOMUX_PIN(GPIO_6_6_OSCAR_VTL_VD, "GPIO_6_6", FUNC0, NOPULL, LEVEL0, &iomg110_oscar_vtl_vd, &iocg110_oscar_vtl_vd)
IOMUX_PIN(PWM_OUT0_OSCAR_VTL_VD, "PWM_OUT0", FUNC0, NOPULL, LEVEL0, &iomg024_oscar_vtl_vd, &iocg024_oscar_vtl_vd)
IOMUX_PIN(GPIO_18_7_OSCAR_VTL_VD, "GPIO_18_7", FUNC0, NOPULL, LEVEL0, &iomg007_oscar_vtl_vd, &iocg191_oscar_vtl_vd)
IOMUX_PIN(UART4_CTS_N_OSCAR_VTL_VD, "UART4_CTS_N", FUNC0, NOPULL, LEVEL0, &iomg067_oscar_vtl_vd, &iocg067_oscar_vtl_vd)
IOMUX_PIN(UART4_RTS_N_OSCAR_VTL_VD, "UART4_RTS_N", FUNC4, PULLUP, LEVEL0, &iomg068_oscar_vtl_vd, &iocg068_oscar_vtl_vd)
IOMUX_PIN(UART4_RXD_OSCAR_VTL_VD, "UART4_RXD", FUNC0, NOPULL, LEVEL0, &iomg069_oscar_vtl_vd, &iocg069_oscar_vtl_vd)
IOMUX_PIN(UART4_TXD_OSCAR_VTL_VD, "UART4_TXD", FUNC4, PULLUP, LEVEL0, &iomg070_oscar_vtl_vd, &iocg070_oscar_vtl_vd)
IOMUX_PIN(GPIO_15_7_OSCAR_VTL_VD, "GPIO_15_7", FUNC0, NOPULL, LEVEL0, &iomg127_oscar_vtl_vd, &iocg127_oscar_vtl_vd)
IOMUX_PIN(SDIO_DATA3_OSCAR_VTL_VD, "SDIO_DATA3", FUNC2, PULLUP, LEVEL0, &iomg095_oscar_vtl_vd, &iocg095_oscar_vtl_vd)
IOMUX_PIN(SDIO_DATA2_OSCAR_VTL_VD, "SDIO_DATA2", FUNC2, PULLUP, LEVEL0, &iomg094_oscar_vtl_vd, &iocg094_oscar_vtl_vd)
IOMUX_PIN(SDIO_DATA1_OSCAR_VTL_VD, "SDIO_DATA1", FUNC2, PULLUP, LEVEL0, &iomg093_oscar_vtl_vd, &iocg093_oscar_vtl_vd)
IOMUX_PIN(SDIO_DATA0_OSCAR_VTL_VD, "SDIO_DATA0", FUNC2, PULLUP, LEVEL0, &iomg092_oscar_vtl_vd, &iocg092_oscar_vtl_vd)
IOMUX_PIN(SDIO_CMD_OSCAR_VTL_VD, "SDIO_CMD", FUNC2, PULLUP, LEVEL0, &iomg091_oscar_vtl_vd, &iocg091_oscar_vtl_vd)
IOMUX_PIN(SDIO_CLK_OSCAR_VTL_VD, "SDIO_CLK", FUNC2, NOPULL, LEVEL1, &iomg090_oscar_vtl_vd, &iocg090_oscar_vtl_vd)
IOMUX_PIN(GPIO_6_1_OSCAR_VTL_VD, "GPIO_6_1", FUNC0, NOPULL, LEVEL0, &iomg105_oscar_vtl_vd, &iocg105_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_1_OSCAR_VTL_VD, "RF_GPIO_1", FUNC0, NOPULL, LEVEL0, &iomg159_oscar_vtl_vd, &iocg159_oscar_vtl_vd)
IOMUX_PIN(UART3_CTS_N_OSCAR_VTL_VD, "UART3_CTS_N", FUNC0, NOPULL, LEVEL0, &iomg063_oscar_vtl_vd, &iocg063_oscar_vtl_vd)
IOMUX_PIN(UART3_RTS_N_OSCAR_VTL_VD, "UART3_RTS_N", FUNC0, NOPULL, LEVEL0, &iomg064_oscar_vtl_vd, &iocg064_oscar_vtl_vd)
IOMUX_PIN(UART3_RXD_OSCAR_VTL_VD, "UART3_RXD", FUNC0, NOPULL, LEVEL0, &iomg065_oscar_vtl_vd, &iocg065_oscar_vtl_vd)
IOMUX_PIN(UART3_TXD_OSCAR_VTL_VD, "UART3_TXD", FUNC0, NOPULL, LEVEL0, &iomg066_oscar_vtl_vd, &iocg066_oscar_vtl_vd)
IOMUX_PIN(GPS_CLK_OSCAR_VTL_VD, "GPS_CLK", FUNC2, NOPULL, LEVEL0, &iomg049_oscar_vtl_vd, &iocg049_oscar_vtl_vd)
IOMUX_PIN(TCXO0_AFC_OSCAR_VTL_VD, "TCXO0_AFC", FUNC4, PULLDOWN, LEVEL0, &iomg144_oscar_vtl_vd, &iocg144_oscar_vtl_vd)
IOMUX_PIN(TCXO1_AFC_OSCAR_VTL_VD, "TCXO1_AFC", FUNC4, PULLDOWN, LEVEL0, &iomg145_oscar_vtl_vd, &iocg145_oscar_vtl_vd)
IOMUX_PIN(USIM0_CLK_OSCAR_VTL_VD, "USIM0_CLK", FUNC0, NOPULL, LEVEL0, &iomg043_oscar_vtl_vd, &iocg043_oscar_vtl_vd)
IOMUX_PIN(USIM0_DATA_OSCAR_VTL_VD, "USIM0_DATA", FUNC0, NOPULL, LEVEL0, &iomg044_oscar_vtl_vd, &iocg044_oscar_vtl_vd)
IOMUX_PIN(USIM0_RST_OSCAR_VTL_VD, "USIM0_RST", FUNC0, NOPULL, LEVEL0, &iomg045_oscar_vtl_vd, &iocg045_oscar_vtl_vd)
IOMUX_PIN(USIM1_CLK_OSCAR_VTL_VD, "USIM1_CLK", FUNC2, PULLDOWN, LEVEL0, &iomg046_oscar_vtl_vd, &iocg046_oscar_vtl_vd)
IOMUX_PIN(USIM1_DATA_OSCAR_VTL_VD, "USIM1_DATA", FUNC2, PULLDOWN, LEVEL0, &iomg047_oscar_vtl_vd, &iocg047_oscar_vtl_vd)
IOMUX_PIN(USIM1_RST_OSCAR_VTL_VD, "USIM1_RST", FUNC2, PULLDOWN, LEVEL0, &iomg048_oscar_vtl_vd, &iocg048_oscar_vtl_vd)
IOMUX_PIN(EMMC_CLK_OSCAR_VTL_VD, "EMMC_CLK", FUNC0, NOPULL, LEVEL1, &iomg079_oscar_vtl_vd, &iocg079_oscar_vtl_vd)
IOMUX_PIN(EMMC_CMD_OSCAR_VTL_VD, "EMMC_CMD", FUNC0, NOPULL, LEVEL1, &iomg080_oscar_vtl_vd, &iocg080_oscar_vtl_vd)
IOMUX_PIN(EMMC_DATA0_OSCAR_VTL_VD, "EMMC_DATA0", FUNC0, NOPULL, LEVEL1, &iomg081_oscar_vtl_vd, &iocg081_oscar_vtl_vd)
IOMUX_PIN(EMMC_DATA1_OSCAR_VTL_VD, "EMMC_DATA1", FUNC0, NOPULL, LEVEL1, &iomg082_oscar_vtl_vd, &iocg082_oscar_vtl_vd)
IOMUX_PIN(EMMC_DATA2_OSCAR_VTL_VD, "EMMC_DATA2", FUNC0, NOPULL, LEVEL1, &iomg083_oscar_vtl_vd, &iocg083_oscar_vtl_vd)
IOMUX_PIN(EMMC_DATA3_OSCAR_VTL_VD, "EMMC_DATA3", FUNC0, NOPULL, LEVEL1, &iomg084_oscar_vtl_vd, &iocg084_oscar_vtl_vd)
IOMUX_PIN(EMMC_DATA4_OSCAR_VTL_VD, "EMMC_DATA4", FUNC0, NOPULL, LEVEL1, &iomg085_oscar_vtl_vd, &iocg085_oscar_vtl_vd)
IOMUX_PIN(EMMC_DATA5_OSCAR_VTL_VD, "EMMC_DATA5", FUNC0, NOPULL, LEVEL1, &iomg086_oscar_vtl_vd, &iocg086_oscar_vtl_vd)
IOMUX_PIN(EMMC_DATA6_OSCAR_VTL_VD, "EMMC_DATA6", FUNC0, NOPULL, LEVEL1, &iomg087_oscar_vtl_vd, &iocg087_oscar_vtl_vd)
IOMUX_PIN(EMMC_DATA7_OSCAR_VTL_VD, "EMMC_DATA7", FUNC0, NOPULL, LEVEL1, &iomg088_oscar_vtl_vd, &iocg088_oscar_vtl_vd)
IOMUX_PIN(GPIO_2_3_OSCAR_VTL_VD, "GPIO_2_3", FUNC4, NOPULL, LEVEL0, &iomg143_oscar_vtl_vd, &iocg143_oscar_vtl_vd)
IOMUX_PIN(BOOT_SEL_OSCAR_VTL_VD, "BOOT_SEL", FUNC0, PULLUP, LEVEL0, &iomg000_oscar_vtl_vd, &iocg000_oscar_vtl_vd)
IOMUX_PIN(GPIO_5_2_OSCAR_VTL_VD, "GPIO_5_2", FUNC0, PULLDOWN, LEVEL0, &iomg098_oscar_vtl_vd, &iocg098_oscar_vtl_vd)
IOMUX_PIN(PCM1_XCLK_OSCAR_VTL_VD, "PCM1_XCLK", FUNC2, PULLDOWN, LEVEL0, &iomg034_oscar_vtl_vd, &iocg034_oscar_vtl_vd)
IOMUX_PIN(PCM1_XFS_OSCAR_VTL_VD, "PCM1_XFS", FUNC2, PULLDOWN, LEVEL0, &iomg035_oscar_vtl_vd, &iocg035_oscar_vtl_vd)
IOMUX_PIN(PCM1_DI_OSCAR_VTL_VD, "PCM1_DI", FUNC2, PULLDOWN, LEVEL0, &iomg036_oscar_vtl_vd, &iocg036_oscar_vtl_vd)
IOMUX_PIN(PCM1_DO_OSCAR_VTL_VD, "PCM1_DO", FUNC0, NOPULL, LEVEL0, &iomg037_oscar_vtl_vd, &iocg037_oscar_vtl_vd)
IOMUX_PIN(GPIO_5_1_OSCAR_VTL_VD, "GPIO_5_1", FUNC0, NOPULL, LEVEL0, &iomg097_oscar_vtl_vd, &iocg097_oscar_vtl_vd)
IOMUX_PIN(I2S_XCLK_OSCAR_VTL_VD, "I2S_XCLK", FUNC0, NOPULL, LEVEL0, &iomg026_oscar_vtl_vd, &iocg026_oscar_vtl_vd)
IOMUX_PIN(I2S_XFS_OSCAR_VTL_VD, "I2S_XFS", FUNC0, NOPULL, LEVEL0, &iomg027_oscar_vtl_vd, &iocg027_oscar_vtl_vd)
IOMUX_PIN(I2S_DI_OSCAR_VTL_VD, "I2S_DI", FUNC0, PULLDOWN, LEVEL0, &iomg028_oscar_vtl_vd, &iocg028_oscar_vtl_vd)
IOMUX_PIN(I2S_DO_OSCAR_VTL_VD, "I2S_DO", FUNC0, NOPULL, LEVEL0, &iomg029_oscar_vtl_vd, &iocg029_oscar_vtl_vd)
IOMUX_PIN(PCM0_XCLK_OSCAR_VTL_VD, "PCM0_XCLK", FUNC0, NOPULL, LEVEL0, &iomg030_oscar_vtl_vd, &iocg030_oscar_vtl_vd)
IOMUX_PIN(PCM0_XFS_OSCAR_VTL_VD, "PCM0_XFS", FUNC0, NOPULL, LEVEL0, &iomg031_oscar_vtl_vd, &iocg031_oscar_vtl_vd)
IOMUX_PIN(PCM0_DI_OSCAR_VTL_VD, "PCM0_DI", FUNC0, PULLDOWN, LEVEL0, &iomg032_oscar_vtl_vd, &iocg032_oscar_vtl_vd)
IOMUX_PIN(PCM0_DO_OSCAR_VTL_VD, "PCM0_DO", FUNC0, NOPULL, LEVEL0, &iomg033_oscar_vtl_vd, &iocg033_oscar_vtl_vd)
IOMUX_PIN(UART1_TXD_OSCAR_VTL_VD, "UART1_TXD", FUNC2, NOPULL, LEVEL0, &iomg058_oscar_vtl_vd, &iocg058_oscar_vtl_vd)
IOMUX_PIN(UART2_RXD_OSCAR_VTL_VD, "UART2_RXD", FUNC4, PULLDOWN, LEVEL0, &iomg061_oscar_vtl_vd, &iocg061_oscar_vtl_vd)
IOMUX_PIN(UART2_TXD_OSCAR_VTL_VD, "UART2_TXD", FUNC4, NOPULL, LEVEL0, &iomg062_oscar_vtl_vd, &iocg062_oscar_vtl_vd)
IOMUX_PIN(TS_ERR_OSCAR_VTL_VD, "TS_ERR", FUNC1, PULLDOWN, LEVEL0, &iomg042_oscar_vtl_vd, &iocg042_oscar_vtl_vd)
IOMUX_PIN(HKADC_SSI_OSCAR_VTL_VD, "HKADC_SSI", FUNC0, PULLDOWN, LEVEL0, &iomg050_oscar_vtl_vd, &iocg050_oscar_vtl_vd)
IOMUX_PIN(AUX_SSI0_OSCAR_VTL_VD, "AUX_SSI0", FUNC0, PULLDOWN, LEVEL0, &iomg051_oscar_vtl_vd, &iocg051_oscar_vtl_vd)
IOMUX_PIN(AUX_SSI1_OSCAR_VTL_VD, "AUX_SSI1", FUNC0, PULLDOWN, LEVEL0, &iomg052_oscar_vtl_vd, &iocg052_oscar_vtl_vd)
IOMUX_PIN(GPIO_5_0_OSCAR_VTL_VD, "GPIO_5_0", FUNC2, NOPULL, LEVEL0, &iomg096_oscar_vtl_vd, &iocg189_oscar_vtl_vd)
IOMUX_PIN(UART0_RXD_OSCAR_VTL_VD, "UART0_RXD", FUNC0, NOPULL, LEVEL0, &iomg053_oscar_vtl_vd, &iocg053_oscar_vtl_vd)
IOMUX_PIN(UART0_TXD_OSCAR_VTL_VD, "UART0_TXD", FUNC0, NOPULL, LEVEL0, &iomg054_oscar_vtl_vd, &iocg054_oscar_vtl_vd)
IOMUX_PIN(RF_RESET1_OSCAR_VTL_VD, "RF_RESET1", FUNC4, NOPULL, LEVEL0, &iomg147_oscar_vtl_vd, &iocg147_oscar_vtl_vd)
IOMUX_PIN(GPIO_4_0_OSCAR_VTL_VD, "GPIO_4_0", FUNC0, NOPULL, LEVEL0, &iomg128_oscar_vtl_vd, &iocg128_oscar_vtl_vd)
IOMUX_PIN(RF_MIPI_CLK0_OSCAR_VTL_VD, "RF_MIPI_CLK0", FUNC0, NOPULL, LEVEL0, &iomg154_oscar_vtl_vd, &iocg154_oscar_vtl_vd)
IOMUX_PIN(RF_MIPI_DATA0_OSCAR_VTL_VD, "RF_MIPI_DATA0", FUNC0, PULLDOWN, LEVEL0, &iomg155_oscar_vtl_vd, &iocg155_oscar_vtl_vd)
IOMUX_PIN(APT_PDM0_OSCAR_VTL_VD, "APT_PDM0", FUNC0, NOPULL, LEVEL0, &iomg089_oscar_vtl_vd, &iocg190_oscar_vtl_vd)
IOMUX_PIN(GPIO_7_5_OSCAR_VTL_VD, "GPIO_7_5", FUNC0, PULLDOWN, LEVEL0, &iomg117_oscar_vtl_vd, &iocg117_oscar_vtl_vd)
IOMUX_PIN(RF_TX_EN1_OSCAR_VTL_VD, "RF_TX_EN1", FUNC4, NOPULL, LEVEL0, &iomg153_oscar_vtl_vd, &iocg153_oscar_vtl_vd)
IOMUX_PIN(RF_TCVR_ON0_OSCAR_VTL_VD, "RF_TCVR_ON0", FUNC0, NOPULL, LEVEL0, &iomg150_oscar_vtl_vd, &iocg150_oscar_vtl_vd)
IOMUX_PIN(RF_TCVR_ON1_OSCAR_VTL_VD, "RF_TCVR_ON1", FUNC0, NOPULL, LEVEL0, &iomg151_oscar_vtl_vd, &iocg151_oscar_vtl_vd)
IOMUX_PIN(RF_SSI0_OSCAR_VTL_VD, "RF_SSI0", FUNC0, PULLDOWN, LEVEL0, &iomg148_oscar_vtl_vd, &iocg148_oscar_vtl_vd)
IOMUX_PIN(RF_SSI1_OSCAR_VTL_VD, "RF_SSI1", FUNC0, PULLDOWN, LEVEL0, &iomg149_oscar_vtl_vd, &iocg149_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_2_OSCAR_VTL_VD, "RF_GPIO_2", FUNC0, NOPULL, LEVEL0, &iomg160_oscar_vtl_vd, &iocg160_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_3_OSCAR_VTL_VD, "RF_GPIO_3", FUNC0, NOPULL, LEVEL0, &iomg161_oscar_vtl_vd, &iocg161_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_4_OSCAR_VTL_VD, "RF_GPIO_4", FUNC0, NOPULL, LEVEL0, &iomg162_oscar_vtl_vd, &iocg162_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_5_OSCAR_VTL_VD, "RF_GPIO_5", FUNC0, NOPULL, LEVEL0, &iomg163_oscar_vtl_vd, &iocg163_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_6_OSCAR_VTL_VD, "RF_GPIO_6", FUNC0, NOPULL, LEVEL0, &iomg164_oscar_vtl_vd, &iocg164_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_7_OSCAR_VTL_VD, "RF_GPIO_7", FUNC0, NOPULL, LEVEL0, &iomg165_oscar_vtl_vd, &iocg165_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_8_OSCAR_VTL_VD, "RF_GPIO_8", FUNC0, NOPULL, LEVEL0, &iomg166_oscar_vtl_vd, &iocg166_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_9_OSCAR_VTL_VD, "RF_GPIO_9", FUNC0, NOPULL, LEVEL0, &iomg167_oscar_vtl_vd, &iocg167_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_10_OSCAR_VTL_VD, "RF_GPIO_10", FUNC0, NOPULL, LEVEL0, &iomg168_oscar_vtl_vd, &iocg168_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_11_OSCAR_VTL_VD, "RF_GPIO_11", FUNC0, NOPULL, LEVEL0, &iomg169_oscar_vtl_vd, &iocg169_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_12_OSCAR_VTL_VD, "RF_GPIO_12", FUNC0, NOPULL, LEVEL0, &iomg170_oscar_vtl_vd, &iocg170_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_13_OSCAR_VTL_VD, "RF_GPIO_13", FUNC0, NOPULL, LEVEL0, &iomg171_oscar_vtl_vd, &iocg171_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_14_OSCAR_VTL_VD, "RF_GPIO_14", FUNC0, NOPULL, LEVEL0, &iomg172_oscar_vtl_vd, &iocg172_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_15_OSCAR_VTL_VD, "RF_GPIO_15", FUNC0, NOPULL, LEVEL0, &iomg173_oscar_vtl_vd, &iocg173_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_16_OSCAR_VTL_VD, "RF_GPIO_16", FUNC0, NOPULL, LEVEL0, &iomg174_oscar_vtl_vd, &iocg184_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_17_OSCAR_VTL_VD, "RF_GPIO_17", FUNC0, NOPULL, LEVEL0, &iomg175_oscar_vtl_vd, &iocg185_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_18_OSCAR_VTL_VD, "RF_GPIO_18", FUNC0, NOPULL, LEVEL0, &iomg176_oscar_vtl_vd, &iocg186_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_19_OSCAR_VTL_VD, "RF_GPIO_19", FUNC0, NOPULL, LEVEL0, &iomg177_oscar_vtl_vd, &iocg187_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_21_OSCAR_VTL_VD, "RF_GPIO_21", FUNC0, NOPULL, LEVEL0, &iomg179_oscar_vtl_vd, &iocg174_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_22_OSCAR_VTL_VD, "RF_GPIO_22", FUNC0, NOPULL, LEVEL0, &iomg180_oscar_vtl_vd, &iocg175_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_23_OSCAR_VTL_VD, "RF_GPIO_23", FUNC0, NOPULL, LEVEL0, &iomg181_oscar_vtl_vd, &iocg176_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_24_OSCAR_VTL_VD, "RF_GPIO_24", FUNC0, NOPULL, LEVEL0, &iomg182_oscar_vtl_vd, &iocg177_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_25_OSCAR_VTL_VD, "RF_GPIO_25", FUNC0, NOPULL, LEVEL0, &iomg183_oscar_vtl_vd, &iocg178_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_26_OSCAR_VTL_VD, "RF_GPIO_26", FUNC0, NOPULL, LEVEL0, &iomg184_oscar_vtl_vd, &iocg179_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_27_OSCAR_VTL_VD, "RF_GPIO_27", FUNC0, NOPULL, LEVEL0, &iomg185_oscar_vtl_vd, &iocg180_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_28_OSCAR_VTL_VD, "RF_GPIO_28", FUNC0, NOPULL, LEVEL0, &iomg186_oscar_vtl_vd, &iocg181_oscar_vtl_vd)
IOMUX_PIN(RF_GPIO_30_OSCAR_VTL_VD, "RF_GPIO_30", FUNC0, NOPULL, LEVEL0, &iomg188_oscar_vtl_vd, &iocg183_oscar_vtl_vd)
IOMUX_PIN(GPIO_7_4_OSCAR_VTL_VD, "GPIO_7_4", FUNC0, NOPULL, LEVEL0, &iomg116_oscar_vtl_vd, &iocg116_oscar_vtl_vd)
IOMUX_PIN(GPIO_6_5_OSCAR_VTL_VD, "GPIO_6_5", FUNC0, NOPULL, LEVEL0, &iomg109_oscar_vtl_vd, &iocg109_oscar_vtl_vd)
IOMUX_PIN(SD_CLK_OSCAR_VTL_VD, "SD_CLK", FUNC0, NOPULL, LEVEL2, &iomg001_oscar_vtl_vd, &iocg001_oscar_vtl_vd)
IOMUX_PIN(SD_CMD_OSCAR_VTL_VD, "SD_CMD", FUNC0, NOPULL, LEVEL0, &iomg002_oscar_vtl_vd, &iocg002_oscar_vtl_vd)
IOMUX_PIN(SD_DATA0_OSCAR_VTL_VD, "SD_DATA0", FUNC0, NOPULL, LEVEL0, &iomg003_oscar_vtl_vd, &iocg003_oscar_vtl_vd)
IOMUX_PIN(SD_DATA1_OSCAR_VTL_VD, "SD_DATA1", FUNC0, NOPULL, LEVEL0, &iomg004_oscar_vtl_vd, &iocg004_oscar_vtl_vd)
IOMUX_PIN(SD_DATA2_OSCAR_VTL_VD, "SD_DATA2", FUNC0, NOPULL, LEVEL0, &iomg005_oscar_vtl_vd, &iocg005_oscar_vtl_vd)
IOMUX_PIN(SD_DATA3_OSCAR_VTL_VD, "SD_DATA3", FUNC0, NOPULL, LEVEL0, &iomg006_oscar_vtl_vd, &iocg006_oscar_vtl_vd)


/*pin table*/
static struct iomux_pin_table pin_config_table_oscar_vtl_vd[] = {
    PIN_TABLE("i2c0_scl", &I2C0_SCL_OSCAR_VTL_VD),
    PIN_TABLE("i2c0_sda", &I2C0_SDA_OSCAR_VTL_VD),
    PIN_TABLE("i2c1_scl", &I2C1_SCL_OSCAR_VTL_VD),
    PIN_TABLE("i2c1_sda", &I2C1_SDA_OSCAR_VTL_VD),
    PIN_TABLE("i2c2_scl", &I2C2_SCL_OSCAR_VTL_VD),
    PIN_TABLE("i2c2_sda", &I2C2_SDA_OSCAR_VTL_VD),
    PIN_TABLE("i2c3_scl", &I2C3_SCL_OSCAR_VTL_VD),
    PIN_TABLE("i2c3_sda", &I2C3_SDA_OSCAR_VTL_VD),
    PIN_TABLE("mcamera_mcam_pwdn", &ISP_GPIO0_OSCAR_VTL_VD),
    PIN_TABLE("mcamera_mcam_vcm_pwdn", &ISP_GPIO2_OSCAR_VTL_VD),
    PIN_TABLE("mcamera_mcam_id", &ISP_GPIO3_OSCAR_VTL_VD),
    PIN_TABLE("mcamera_mcam_reset", &ISP_RESETB0_OSCAR_VTL_VD),
    PIN_TABLE("mcamera_isp_mclk", &ISP_CCLK0_OSCAR_VTL_VD),
    PIN_TABLE("mcamera_isp_sda0", &ISP_SDA0_OSCAR_VTL_VD),
    PIN_TABLE("mcamera_isp_scl0", &ISP_SCL0_OSCAR_VTL_VD),
    PIN_TABLE("scamera_scam_pwdn", &ISP_GPIO1_OSCAR_VTL_VD),
    PIN_TABLE("scamera_scam_id", &ISP_GPIO4_OSCAR_VTL_VD),
    PIN_TABLE("scamera_scam_reset", &ISP_RESETB1_OSCAR_VTL_VD),
    PIN_TABLE("scamera_isp_mclk", &ISP_CCLK1_OSCAR_VTL_VD),
    PIN_TABLE("scamera_isp_sda1", &ISP_SDA1_OSCAR_VTL_VD),
    PIN_TABLE("scamera_isp_scl1", &ISP_SCL1_OSCAR_VTL_VD),
    PIN_TABLE("flash_strobe", &ISP_GPIO5_OSCAR_VTL_VD),
    PIN_TABLE("flash_mask", &RF_GPIO_20_OSCAR_VTL_VD),
    PIN_TABLE("key_up", &GPIO_4_2_OSCAR_VTL_VD),
    PIN_TABLE("key_down", &GPIO_4_3_OSCAR_VTL_VD),
    PIN_TABLE("lcd_lcm_en", &PWM_OUT1_OSCAR_VTL_VD),
    PIN_TABLE("lcd_rst_n", &GPIO_4_6_OSCAR_VTL_VD),
    PIN_TABLE("lcd_te0", &GPIO_4_7_OSCAR_VTL_VD),
    PIN_TABLE("lcd_enp", &GPIO_6_6_OSCAR_VTL_VD),
    PIN_TABLE("lcd_pwm_en", &PWM_OUT0_OSCAR_VTL_VD),
    PIN_TABLE("tp_rst_n", &GPIO_18_7_OSCAR_VTL_VD),
    PIN_TABLE("bt_uart4_cts_n", &UART4_CTS_N_OSCAR_VTL_VD),
    PIN_TABLE("bt_uart4_rts_n", &UART4_RTS_N_OSCAR_VTL_VD),
    PIN_TABLE("bt_uart4_rxd", &UART4_RXD_OSCAR_VTL_VD),
    PIN_TABLE("bt_uart4_txd", &UART4_TXD_OSCAR_VTL_VD),
    PIN_TABLE("bt_ap_wakeup_bt", &GPIO_15_7_OSCAR_VTL_VD),
    PIN_TABLE("wifi_sdio_data3", &SDIO_DATA3_OSCAR_VTL_VD),
    PIN_TABLE("wifi_sdio_data2", &SDIO_DATA2_OSCAR_VTL_VD),
    PIN_TABLE("wifi_sdio_data1", &SDIO_DATA1_OSCAR_VTL_VD),
    PIN_TABLE("wifi_sdio_data0", &SDIO_DATA0_OSCAR_VTL_VD),
    PIN_TABLE("wifi_sdio_cmd", &SDIO_CMD_OSCAR_VTL_VD),
    PIN_TABLE("wifi_sdio_clk", &SDIO_CLK_OSCAR_VTL_VD),
    PIN_TABLE("gps_en", &GPIO_6_1_OSCAR_VTL_VD),
    PIN_TABLE("gps_blanking", &RF_GPIO_1_OSCAR_VTL_VD),
    PIN_TABLE("gps_uart3_cts_n", &UART3_CTS_N_OSCAR_VTL_VD),
    PIN_TABLE("gps_uart3_rts_n", &UART3_RTS_N_OSCAR_VTL_VD),
    PIN_TABLE("gps_uart3_rxd", &UART3_RXD_OSCAR_VTL_VD),
    PIN_TABLE("gps_uart3_txd", &UART3_TXD_OSCAR_VTL_VD),
    PIN_TABLE("gps_fref_clk", &GPS_CLK_OSCAR_VTL_VD),
    PIN_TABLE("tcxo_afc_gul1", &TCXO0_AFC_OSCAR_VTL_VD),
    PIN_TABLE("tcxo_afc_g2", &TCXO1_AFC_OSCAR_VTL_VD),
    PIN_TABLE("usima_clk_raw", &USIM0_CLK_OSCAR_VTL_VD),
    PIN_TABLE("usima_data_raw", &USIM0_DATA_OSCAR_VTL_VD),
    PIN_TABLE("usima_rst_raw", &USIM0_RST_OSCAR_VTL_VD),
    PIN_TABLE("usimb_clk_raw", &USIM1_CLK_OSCAR_VTL_VD),
    PIN_TABLE("usimb_data_raw", &USIM1_DATA_OSCAR_VTL_VD),
    PIN_TABLE("usimb_rst_raw", &USIM1_RST_OSCAR_VTL_VD),
    PIN_TABLE("emmc_clk", &EMMC_CLK_OSCAR_VTL_VD),
    PIN_TABLE("emmc_cmd", &EMMC_CMD_OSCAR_VTL_VD),
    PIN_TABLE("emmc_data0", &EMMC_DATA0_OSCAR_VTL_VD),
    PIN_TABLE("emmc_data1", &EMMC_DATA1_OSCAR_VTL_VD),
    PIN_TABLE("emmc_data2", &EMMC_DATA2_OSCAR_VTL_VD),
    PIN_TABLE("emmc_data3", &EMMC_DATA3_OSCAR_VTL_VD),
    PIN_TABLE("emmc_data4", &EMMC_DATA4_OSCAR_VTL_VD),
    PIN_TABLE("emmc_data5", &EMMC_DATA5_OSCAR_VTL_VD),
    PIN_TABLE("emmc_data6", &EMMC_DATA6_OSCAR_VTL_VD),
    PIN_TABLE("emmc_data7", &EMMC_DATA7_OSCAR_VTL_VD),
    PIN_TABLE("emmc_rst_n", &GPIO_2_3_OSCAR_VTL_VD),
    PIN_TABLE("boot_sel", &BOOT_SEL_OSCAR_VTL_VD),
    PIN_TABLE("boot_jtag_sel1", &GPIO_5_2_OSCAR_VTL_VD),
    PIN_TABLE("codec_pa_i2s_xclk", &PCM1_XCLK_OSCAR_VTL_VD),
    PIN_TABLE("codec_pa_i2s_sync", &PCM1_XFS_OSCAR_VTL_VD),
    PIN_TABLE("codec_pa_soc_i2s_do", &PCM1_DI_OSCAR_VTL_VD),
    PIN_TABLE("codec_pcm1_do", &PCM1_DO_OSCAR_VTL_VD),
    PIN_TABLE("exphp_en", &GPIO_5_1_OSCAR_VTL_VD),
    PIN_TABLE("es305_i2s_xclk", &I2S_XCLK_OSCAR_VTL_VD),
    PIN_TABLE("es305_i2s_xfs", &I2S_XFS_OSCAR_VTL_VD),
    PIN_TABLE("es305_i2s_di", &I2S_DI_OSCAR_VTL_VD),
    PIN_TABLE("es305_i2s_do", &I2S_DO_OSCAR_VTL_VD),
    PIN_TABLE("es305_pcm0_xclk", &PCM0_XCLK_OSCAR_VTL_VD),
    PIN_TABLE("es305_pcm0_xfs", &PCM0_XFS_OSCAR_VTL_VD),
    PIN_TABLE("es305_pcm0_di", &PCM0_DI_OSCAR_VTL_VD),
    PIN_TABLE("es305_pcm0_do", &PCM0_DO_OSCAR_VTL_VD),
    PIN_TABLE("es305_reset_n", &UART1_TXD_OSCAR_VTL_VD),
    PIN_TABLE("soc_audience_uart2_rxd", &UART2_RXD_OSCAR_VTL_VD),
    PIN_TABLE("soc_audience_uart2_txd", &UART2_TXD_OSCAR_VTL_VD),
    PIN_TABLE("es305_int", &TS_ERR_OSCAR_VTL_VD),
    PIN_TABLE("pmu_hkadc_ssi", &HKADC_SSI_OSCAR_VTL_VD),
    PIN_TABLE("pmu_aux_ssi0", &AUX_SSI0_OSCAR_VTL_VD),
    PIN_TABLE("pmu_aux_ssi1", &AUX_SSI1_OSCAR_VTL_VD),
    PIN_TABLE("pmu_auxdac_en", &GPIO_5_0_OSCAR_VTL_VD),
    PIN_TABLE("test_uart0_rxd", &UART0_RXD_OSCAR_VTL_VD),
    PIN_TABLE("test_uart0_txd", &UART0_TXD_OSCAR_VTL_VD),
    PIN_TABLE("odt_gpio_9_6", &RF_RESET1_OSCAR_VTL_VD),
    PIN_TABLE("charger_cd", &GPIO_4_0_OSCAR_VTL_VD),
    PIN_TABLE("rfpmu_mipi_clk0", &RF_MIPI_CLK0_OSCAR_VTL_VD),
    PIN_TABLE("rfpmu_mipi_data0", &RF_MIPI_DATA0_OSCAR_VTL_VD),
    PIN_TABLE("rf_pmu_apt_pdm0", &APT_PDM0_OSCAR_VTL_VD),
    PIN_TABLE("rfant_det0", &GPIO_7_5_OSCAR_VTL_VD),
    PIN_TABLE("rf_1p8_oe", &RF_TX_EN1_OSCAR_VTL_VD),
    PIN_TABLE("rf_tcvr_on0", &RF_TCVR_ON0_OSCAR_VTL_VD),
    PIN_TABLE("rf_tcvr_on1", &RF_TCVR_ON1_OSCAR_VTL_VD),
    PIN_TABLE("rf_ssi0", &RF_SSI0_OSCAR_VTL_VD),
    PIN_TABLE("rf_ssi1", &RF_SSI1_OSCAR_VTL_VD),
    PIN_TABLE("rf_gpio_2", &RF_GPIO_2_OSCAR_VTL_VD),
    PIN_TABLE("rf_gpio_3", &RF_GPIO_3_OSCAR_VTL_VD),
    PIN_TABLE("rf_gpio_4", &RF_GPIO_4_OSCAR_VTL_VD),
    PIN_TABLE("rf_gpio_5", &RF_GPIO_5_OSCAR_VTL_VD),
    PIN_TABLE("rf_gpio_6", &RF_GPIO_6_OSCAR_VTL_VD),
    PIN_TABLE("rf_gpio_7", &RF_GPIO_7_OSCAR_VTL_VD),
    PIN_TABLE("rf_gpio_8", &RF_GPIO_8_OSCAR_VTL_VD),
    PIN_TABLE("rf_gpio_9", &RF_GPIO_9_OSCAR_VTL_VD),
    PIN_TABLE("rf_gpio_10", &RF_GPIO_10_OSCAR_VTL_VD),
    PIN_TABLE("rf_gpio_11", &RF_GPIO_11_OSCAR_VTL_VD),
    PIN_TABLE("rf_gpio_12", &RF_GPIO_12_OSCAR_VTL_VD),
    PIN_TABLE("rf_gpio_13", &RF_GPIO_13_OSCAR_VTL_VD),
    PIN_TABLE("rf_gpio_14", &RF_GPIO_14_OSCAR_VTL_VD),
    PIN_TABLE("rf_gpio_15", &RF_GPIO_15_OSCAR_VTL_VD),
    PIN_TABLE("rf_gpio_16", &RF_GPIO_16_OSCAR_VTL_VD),
    PIN_TABLE("rf_gpio_17", &RF_GPIO_17_OSCAR_VTL_VD),
    PIN_TABLE("rf_gpio_18", &RF_GPIO_18_OSCAR_VTL_VD),
    PIN_TABLE("rf_gpio_19", &RF_GPIO_19_OSCAR_VTL_VD),
    PIN_TABLE("rf_gpio_21", &RF_GPIO_21_OSCAR_VTL_VD),
    PIN_TABLE("rf_gpio_22", &RF_GPIO_22_OSCAR_VTL_VD),
    PIN_TABLE("rf_gpio_23", &RF_GPIO_23_OSCAR_VTL_VD),
    PIN_TABLE("rf_gpio_24", &RF_GPIO_24_OSCAR_VTL_VD),
    PIN_TABLE("rf_gpio_25", &RF_GPIO_25_OSCAR_VTL_VD),
    PIN_TABLE("rf_gpio_26", &RF_GPIO_26_OSCAR_VTL_VD),
    PIN_TABLE("rf_gpio_27", &RF_GPIO_27_OSCAR_VTL_VD),
    PIN_TABLE("rf_gpio_28", &RF_GPIO_28_OSCAR_VTL_VD),
    PIN_TABLE("rf_gpio_30", &RF_GPIO_30_OSCAR_VTL_VD),
    PIN_TABLE("spk_en", &GPIO_7_4_OSCAR_VTL_VD),
    PIN_TABLE("GPIO_6_5_NFC_CLK", &GPIO_6_5_OSCAR_VTL_VD),
    PIN_TABLE("sd_clk", &SD_CLK_OSCAR_VTL_VD),
    PIN_TABLE("sd_cmd", &SD_CMD_OSCAR_VTL_VD),
    PIN_TABLE("sd_data0", &SD_DATA0_OSCAR_VTL_VD),
    PIN_TABLE("sd_data1", &SD_DATA1_OSCAR_VTL_VD),
    PIN_TABLE("sd_data2", &SD_DATA2_OSCAR_VTL_VD),
    PIN_TABLE("sd_data3", &SD_DATA3_OSCAR_VTL_VD),
    PIN_TABLE(NULL, NULL)
};


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif
/******************** (C) COPYRIGHT 2012 STMicroelectronics ********************
*
* File Name		: lsm330_gyr_sysfs.c
* Authors		: MEMS Motion Sensors Products Div- Application Team
*			: Matteo Dameno (matteo.dameno@st.com)
*			: Denis Ciocca (denis.ciocca@st.com)
*			: Both authors are willing to be considered the
*			: contact and update points for the driver.
* Version		: V 1.0.0
* Date			: 2012/07/10
* Description		: LSM330 gyroscope driver
*
********************************************************************************
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 as
* published by the Free Software Foundation.
*
* THE PRESENT SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES
* OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, FOR THE SOLE
* PURPOSE TO SUPPORT YOUR APPLICATION DEVELOPMENT.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
 ******************************************************************************
Version History.
	V 1.0.0		First Release
 ******************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/mutex.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/slab.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/stat.h>
#include <linux/board_sensors.h>
#include <linux/hrtimer.h>
#include <linux/delay.h>
#include	<linux/earlysuspend.h>
#include <linux/regulator/consumer.h>
#include "lsm330.h"

#ifdef CONFIG_HUAWEI_HW_DEV_DCT
#include <linux/hw_dev_dec.h>
#endif


#define LSM330_GYR_ENABLED		1
#define LSM330_GYR_DISABLED		0
#define SELF_TEST					1
#define SELF_TEST_OK				1
#define SELF_TEST_FAIL				0
#define MAX_VAL         13857
#define MIN_VAL         1428

/** Maximum polled-device-reported rot speed value value in dps*/
#define FS_MAX				32768

/* lsm330 gyroscope registers */
#define WHO_AM_I			(0x0F)

#define SENSITIVITY_250			8750	/*	udps/LSB */
#define SENSITIVITY_500			17500	/*	udps/LSB */
#define SENSITIVITY_2000		70000	/*	udps/LSB */

#define CTRL_REG1			(0x20)	/* CTRL REG1 */
#define CTRL_REG2			(0x21)	/* CTRL REG2 */
#define CTRL_REG3			(0x22)	/* CTRL_REG3 */
#define CTRL_REG4			(0x23)	/* CTRL_REG4 */
#define CTRL_REG5			(0x24)	/* CTRL_REG5 */
#define	REFERENCE			(0x25)	/* REFERENCE REG */
#define	FIFO_CTRL_REG			(0x2E)	/* FIFO CONTROL REGISTER */
#define FIFO_SRC_REG			(0x2F)	/* FIFO SOURCE REGISTER */
#define	OUT_X_L				(0x28)	/* 1st AXIS OUT REG of 6 */

#define AXISDATA_REG			OUT_X_L

/* CTRL_REG1 */
#define ALL_ZEROES			(0x00)
#define PM_OFF				(0x00)
#define PM_NORMAL			(0x08)
#define ENABLE_ALL_AXES			(0x07)
#define ENABLE_NO_AXES			(0x00)
#define BW00				(0x00)
#define BW01				(0x10)
#define BW10				(0x20)
#define BW11				(0x30)
#define ODR095				(0x00)	/* ODR =  95Hz */
#define ODR190				(0x40)	/* ODR = 190Hz */
#define ODR380				(0x80)	/* ODR = 380Hz */
#define ODR760				(0xC0)	/* ODR = 760Hz */

/* CTRL_REG3 bits */
#define	I2_DRDY				(0x08)
#define	I2_WTM				(0x04)
#define	I2_OVRUN			(0x02)
#define	I2_EMPTY			(0x01)
#define	I2_NONE				(0x00)
#define	I2_MASK				(0x0F)

/* CTRL_REG4 bits */
#define	FS_MASK				(0x30)
#define	BDU_ENABLE			(0x80)
#define	SELFTEST_MASK			0x06
#define     LSM330_SELFTEST_EN_POS	0x02


/* CTRL_REG5 bits */
#define	FIFO_ENABLE			(0x40)
#define HPF_ENALBE			(0x11)

/* FIFO_CTRL_REG bits */
#define	FIFO_MODE_MASK			(0xE0)
#define	FIFO_MODE_BYPASS		(0x00)
#define	FIFO_MODE_FIFO			(0x20)
#define	FIFO_MODE_STREAM		(0x40)
#define	FIFO_MODE_STR2FIFO		(0x60)
#define	FIFO_MODE_BYPASS2STR		(0x80)
#define	FIFO_WATERMARK_MASK		(0x1F)

#define FIFO_STORED_DATA_MASK		(0x1F)

#define I2C_AUTO_INCREMENT		(0x80)

/* RESUME STATE INDICES */
#define	RES_CTRL_REG1			0
#define	RES_CTRL_REG2			1
#define	RES_CTRL_REG3			2
#define	RES_CTRL_REG4			3
#define	RES_CTRL_REG5			4
#define	RES_FIFO_CTRL_REG		5
#define	RESUME_ENTRIES			6

/** Registers Contents */
#define WHOAMI_LSM330_GYR		0xD4	/* Expected content for WAI */
#define I2C_RETRY_DELAY         12               /* Waiting for signals [ms] */
#define I2C_RETRIES             6               /* Number of retries */

/* gyro kernel error code start from 0x03
 * 0x01: cannot open sensor (report from userspace directly)
 * 0x02: cannot open sysfs fail (report from userspace directly)
 * 0x03: x value
 * 0x04: y value
 * 0x05: z value
 * */
#define ERR_MSG_ERR_NONE	0x00
#define ERR_MSG_OPEN_SENSOR	0x01
#define ERR_MSG_OPEN_SYSFS	0x02
#define ERR_MSG_X_OVERFLOW	0x03
#define ERR_MSG_Y_OVERFLOW	0x04
#define ERR_MSG_Z_OVERFLOW	0x05
#define ERR_MSG_POWERON		0x06
#define ERR_MSG_GETDATA		0x07
#define ERR_MSG_SET_RESULT	0x08
#define ERR_MSG_RESUME_REG	0x09

static char *selftest_err_msg[] = {
	[ERR_MSG_ERR_NONE] = "0x00 gyro selftest pass",
	[ERR_MSG_OPEN_SENSOR] = "0x01 cannot open sensor",
	[ERR_MSG_OPEN_SYSFS] = "0x02 cannot open sysfs",
	[ERR_MSG_X_OVERFLOW] = "0x03 sleftest failed, x over threshold",
	[ERR_MSG_Y_OVERFLOW] = "0x04 sleftest failed, y over threshold",
	[ERR_MSG_Z_OVERFLOW] = "0x05 sleftest failed, z over threshold",
	[ERR_MSG_POWERON] = "0x06 sleftest failed, power on fail",
	[ERR_MSG_GETDATA] = "0x07 sleftest failed, get gyro data error",
	[ERR_MSG_SET_RESULT] = "0x08 sleftest failed, set test result failed",
	[ERR_MSG_RESUME_REG] = "0x09 sleftest failed, gyro set self test reg resume fail",
};

/* mark calibrate err*/
static atomic_t selftest_err_code = ATOMIC_INIT(0);

static int int1_gpio = LSM330_GYR_DEFAULT_INT1_GPIO;
static int int2_gpio = LSM330_GYR_DEFAULT_INT2_GPIO;
/* module_param(int1_gpio, int, S_IRUGO); */
static struct lsm330_gyr_status *stat_temp;

struct lsm330_gyr_triple {
	short	x,	/* x-axis angular rate data. */
		y,	/* y-axis angluar rate data. */
		z;	/* z-axis angular rate data. */
};

struct output_rate {
	int poll_rate_ms;
	u8 mask;
};

static const struct output_rate odr_table[] = {

	{	2,	ODR760|BW10},
	{	3,	ODR380|BW01},
	{	6,	ODR190|BW00},
	{	11,	ODR095|BW00},
};

static struct lsm330_gyr_platform_data default_lsm330_gyr_pdata = {
	.fs_range = LSM330_GYR_FS_2000DPS,
	.axis_map_x = 0,
	.axis_map_y = 1,
	.axis_map_z = 2,
	.negate_x = 0,
	.negate_y = 0,
	.negate_z = 0,

	.poll_interval = 10,
	.min_interval = LSM330_GYR_MIN_POLL_PERIOD_MS,	/* 2ms */

	.gpio_int1 = LSM330_GYR_DEFAULT_INT1_GPIO,
	.gpio_int2 = LSM330_GYR_DEFAULT_INT2_GPIO,	/* int for fifo */

};

struct lsm330_gyr_status {
	struct i2c_client *client;
	struct lsm330_gyr_platform_data *pdata;

	struct mutex lock;

	struct input_dev *input_dev;
	struct delayed_work input_work;

	int hw_initialized;
	atomic_t enabled;
	int use_smbus;

	u8 reg_addr;
	u8 resume_state[RESUME_ENTRIES];

	u32 sensitivity;

	/* interrupt related */
	int irq2;
	struct work_struct irq2_work;
	struct workqueue_struct *irq2_work_queue;
	int on_before_suspend;
	struct early_suspend early_suspend;
	/* fifo related */
	u8 watermark;
	u8 fifomode;
       struct  regulator *gyr_vdd;
       struct  regulator *gyr_io_vdd;
};


static int lsm330_gyr_i2c_read(struct lsm330_gyr_status *stat, u8 *buf,
								int len)
{

	int err;
	int tries = 0;

	struct i2c_msg	msgs[] = {
		{
			.addr = stat->client->addr,
			.flags = stat->client->flags & I2C_M_TEN,
			.len = 1,
			.buf = buf,
		},
		{
			.addr = stat->client->addr,
			.flags = (stat->client->flags & I2C_M_TEN) | I2C_M_RD,
			.len = len,
			.buf = buf,
		},
	};

	/*form power on to access device, most need 50ms*/
	do {
		err = i2c_transfer(stat->client->adapter, msgs, 2);
		if (err != 2)
			msleep_interruptible(I2C_RETRY_DELAY);
	} while ((err != 2) && (++tries < I2C_RETRIES));

	if (err != 2) {
		dev_err(&stat->client->dev, "read transfer error\n");
		err = -EIO;
	} else {
		err = 0;
	}

	return err;
}

static int lsm330_gyr_i2c_write(struct lsm330_gyr_status *stat, u8 *buf,
									int len)
{
	int ret;
	u8 reg, value;

	if (len > 1)
		buf[0] = (I2C_AUTO_INCREMENT | buf[0]);

	reg = buf[0];
	value = buf[1];

	if (stat->use_smbus) {
		if (len == 1) {
			ret = i2c_smbus_write_byte_data(stat->client,
								reg, value);
#ifdef DEBUG
			dev_warn(&stat->client->dev,
				"i2c_smbus_write_byte_data: ret=%d, len:%d, "
				"command=0x%02x, value=0x%02x\n",
				ret, len, reg , value);
#endif
			return ret;
		} else if (len > 1) {
			ret = i2c_smbus_write_i2c_block_data(stat->client,
							reg, len, buf + 1);
#ifdef DEBUG
			dev_warn(&stat->client->dev,
				"i2c_smbus_write_i2c_block_data: ret=%d, "
				"len:%d, command=0x%02x, ",
				ret, len, reg);
			unsigned int ii;
			for (ii = 0; ii < (len + 1); ii++)
				printk(KERN_DEBUG "value[%d]=0x%02x,",
								ii, buf[ii]);

			printk("\n");
#endif
			return ret;
		}
	}

	ret = i2c_master_send(stat->client, buf, len+1);
	return (ret == len+1) ? 0 : ret;
}


static int lsm330_gyr_register_write(struct lsm330_gyr_status *stat, u8 *buf,
		u8 reg_address, u8 new_value)
{
	int err;

		/* Sets configuration register at reg_address
		 *  NOTE: this is a straight overwrite  */
		buf[0] = reg_address;
		buf[1] = new_value;
		err = lsm330_gyr_i2c_write(stat, buf, 1);
		if (err < 0)
			return err;

	return err;
}

static int lsm330_gyr_register_read(struct lsm330_gyr_status *stat, u8 *buf,
		u8 reg_address)
{

	int err = -1;
	buf[0] = (reg_address);
	err = lsm330_gyr_i2c_read(stat, buf, 1);
	return err;
}

static int lsm330_gyr_register_update(struct lsm330_gyr_status *stat, u8 *buf,
		u8 reg_address, u8 mask, u8 new_bit_values)
{
	int err = -1;
	u8 init_val;
	u8 updated_val;
	err = lsm330_gyr_register_read(stat, buf, reg_address);
	if (!(err < 0)) {
		init_val = buf[0];
		updated_val = ((mask & new_bit_values) | ((~mask) & init_val));
		err = lsm330_gyr_register_write(stat, buf, reg_address,
				updated_val);
	}
	return err;
}


static int lsm330_gyr_update_watermark(struct lsm330_gyr_status *stat,
								u8 watermark)
{
	int res = 0;
	u8 buf[2];
	u8 new_value;

	mutex_lock(&stat->lock);
	new_value = (watermark % 0x20);
	res = lsm330_gyr_register_update(stat, buf, FIFO_CTRL_REG,
			 FIFO_WATERMARK_MASK, new_value);
	if (res < 0) {
		dev_err(&stat->client->dev, "failed to update watermark\n");
		return res;
	}
	dev_dbg(&stat->client->dev, "%s new_value:0x%02x,watermark:0x%02x\n",
						__func__, new_value, watermark);

	stat->resume_state[RES_FIFO_CTRL_REG] =
		((FIFO_WATERMARK_MASK & new_value) |
		(~FIFO_WATERMARK_MASK &
				stat->resume_state[RES_FIFO_CTRL_REG]));
	stat->watermark = new_value;
	mutex_unlock(&stat->lock);
	return res;
}

static int lsm330_gyr_update_fifomode(struct lsm330_gyr_status *stat,
								u8 fifomode)
{
	int res;
	u8 buf[2];
	u8 new_value;

	new_value = fifomode;
	res = lsm330_gyr_register_update(stat, buf, FIFO_CTRL_REG,
					FIFO_MODE_MASK, new_value);
	if (res < 0) {
		dev_err(&stat->client->dev, "failed to update fifoMode\n");
		return res;
	}
	/*
	dev_dbg(&stat->client->dev, "new_value:0x%02x,prev fifomode:0x%02x\n",
				__func__, new_value, stat->fifomode);
	 */
	stat->resume_state[RES_FIFO_CTRL_REG] =
		((FIFO_MODE_MASK & new_value) |
		(~FIFO_MODE_MASK &
				stat->resume_state[RES_FIFO_CTRL_REG]));
	stat->fifomode = new_value;

	return res;
}

static int lsm330_gyr_fifo_reset(struct lsm330_gyr_status *stat)
{
	u8 oldmode;
	int res;

	oldmode = stat->fifomode;
	res = lsm330_gyr_update_fifomode(stat, FIFO_MODE_BYPASS);
	if (res < 0)
		return res;
	res = lsm330_gyr_update_fifomode(stat, oldmode);
	if (res >= 0)
		dev_dbg(&stat->client->dev, "%s fifo reset to: 0x%02x\n",
							__func__, oldmode);

	return res;
}

static int lsm330_gyr_fifo_hwenable(struct lsm330_gyr_status *stat,
								u8 enable)
{
	int res;
	u8 buf[2];
	u8 set = 0x00;
	if (enable)
		set = FIFO_ENABLE;
	res = lsm330_gyr_register_update(stat, buf, CTRL_REG5,
			FIFO_ENABLE, set);
	if (res < 0) {
		dev_err(&stat->client->dev, "fifo_hw switch to:0x%02x failed\n",
									set);
		return res;
	}
	stat->resume_state[RES_CTRL_REG5] =
		((FIFO_ENABLE & set) |
		(~FIFO_ENABLE & stat->resume_state[RES_CTRL_REG5]));
	dev_dbg(&stat->client->dev, "%s set to:0x%02x\n", __func__, set);
	return res;
}

static int lsm330_gyr_manage_int2settings(struct lsm330_gyr_status *stat,
								u8 fifomode)
{
	int res;
	u8 buf[2];
	bool enable_fifo_hw;
	bool recognized_mode = false;
	u8 int2bits = I2_NONE;

	switch (fifomode) {
	case FIFO_MODE_FIFO:
		recognized_mode = true;


		int2bits = (I2_WTM | I2_OVRUN);
		enable_fifo_hw = true;

		res = lsm330_gyr_register_update(stat, buf, CTRL_REG3,
					I2_MASK, int2bits);
		if (res < 0) {
			dev_err(&stat->client->dev, "%s : failed to update "
							"CTRL_REG3:0x%02x\n",
							__func__, fifomode);
			goto err_mutex_unlock;
		}
		stat->resume_state[RES_CTRL_REG3] =
			((I2_MASK & int2bits) |
			(~(I2_MASK) & stat->resume_state[RES_CTRL_REG3]));
		/* enable_fifo_hw = true; */
		break;

	case FIFO_MODE_BYPASS:
		recognized_mode = true;


		int2bits = I2_DRDY;

		res = lsm330_gyr_register_update(stat, buf, CTRL_REG3,
					I2_MASK, int2bits);
		if (res < 0) {
			dev_err(&stat->client->dev, "%s : failed to update"
						" to CTRL_REG3:0x%02x\n",
							__func__, fifomode);
			goto err_mutex_unlock;
		}
		stat->resume_state[RES_CTRL_REG3] =
			((I2_MASK & int2bits) |
			(~I2_MASK & stat->resume_state[RES_CTRL_REG3]));
		enable_fifo_hw = false;
		break;

	default:
		recognized_mode = false;
		res = lsm330_gyr_register_update(stat, buf, CTRL_REG3,
					I2_MASK, I2_NONE);
		if (res < 0) {
			dev_err(&stat->client->dev, "%s : failed to update "
						"CTRL_REG3:0x%02x\n",
						__func__, fifomode);
			goto err_mutex_unlock;
		}
		enable_fifo_hw = false;
		stat->resume_state[RES_CTRL_REG3] =
			((I2_MASK & 0x00) |
			(~I2_MASK & stat->resume_state[RES_CTRL_REG3]));
		break;

	}
	if (recognized_mode) {
		res = lsm330_gyr_update_fifomode(stat, fifomode);
		if (res < 0) {
			dev_err(&stat->client->dev, "%s : failed to "
						"set fifoMode\n", __func__);
			goto err_mutex_unlock;
		}
	}
	res = lsm330_gyr_fifo_hwenable(stat, enable_fifo_hw);

err_mutex_unlock:

	return res;
}


static int lsm330_gyr_update_fs_range(struct lsm330_gyr_status *stat,
							u8 new_fs)
{
	int res ;
	u8 buf[2];

	u32 sensitivity;

	switch(new_fs) {
		case LSM330_GYR_FS_250DPS:
			sensitivity = SENSITIVITY_250;
			break;
		case LSM330_GYR_FS_500DPS:
			sensitivity = SENSITIVITY_500;
			break;
		case LSM330_GYR_FS_2000DPS:
			sensitivity = SENSITIVITY_2000;
			break;
		default:
			dev_err(&stat->client->dev, "invalid g range "
						"requested: %u\n", new_fs);
			return -EINVAL;
	}


	buf[0] = CTRL_REG4;

	res = lsm330_gyr_register_update(stat, buf, CTRL_REG4,
							FS_MASK, new_fs);

	if (res < 0) {
		dev_err(&stat->client->dev, "%s : failed to update fs:0x%02x\n",
							__func__, new_fs);
		return res;
	}
	stat->resume_state[RES_CTRL_REG4] =
		((FS_MASK & new_fs) |
		(~FS_MASK & stat->resume_state[RES_CTRL_REG4]));

	stat->sensitivity = sensitivity;
	return res;
}


static int lsm330_gyr_update_odr(struct lsm330_gyr_status *stat,
			unsigned int poll_interval_ms)
{
	int err = -1;
	int i;
	u8 config[2];

	for(i = ARRAY_SIZE(odr_table) - 1; i >= 0; i--) {
		if(odr_table[i].poll_rate_ms <= poll_interval_ms)
			break;
	}

	config[1] = odr_table[i].mask;
	config[1] |= (ENABLE_ALL_AXES + PM_NORMAL);

	/* If device is currently enabled, we need to write new
	 *  configuration out to it */
	if (atomic_read(&stat->enabled)) {
pr_info("prova");
		config[0] = CTRL_REG1;
		err = lsm330_gyr_i2c_write(stat, config, 1);
		if (err < 0)
			return err;
		stat->resume_state[RES_CTRL_REG1] = config[1];
	}


	return err;
}

/* gyroscope data readout */
static int lsm330_gyr_get_data(struct lsm330_gyr_status *stat,
			     struct lsm330_gyr_triple *data)
{
	int err;
	unsigned char gyro_out[6];
	/* y,p,r hardware data */
	s16 hw_d[3] = { 0 };

	gyro_out[0] = (I2C_AUTO_INCREMENT|AXISDATA_REG);

	err = lsm330_gyr_i2c_read(stat, gyro_out, 6);

	if (err < 0)
		return err;

	hw_d[0] = (s16) (((gyro_out[1]) << 8) | gyro_out[0]);
	hw_d[1] = (s16) (((gyro_out[3]) << 8) | gyro_out[2]);
	hw_d[2] = (s16) (((gyro_out[5]) << 8) | gyro_out[4]);

	//hw_d[0] = hw_d[0] * stat->sensitivity;
	//hw_d[1] = hw_d[1] * stat->sensitivity;
	//hw_d[2] = hw_d[2] * stat->sensitivity;

	data->x = ((stat->pdata->negate_x) ? (-hw_d[stat->pdata->axis_map_x])
		   : (hw_d[stat->pdata->axis_map_x]));
	data->y = ((stat->pdata->negate_y) ? (-hw_d[stat->pdata->axis_map_y])
		   : (hw_d[stat->pdata->axis_map_y]));
	data->z = ((stat->pdata->negate_z) ? (-hw_d[stat->pdata->axis_map_z])
		   : (hw_d[stat->pdata->axis_map_z]));

#ifdef DEBUG
        pr_info("%s read x=%d, y=%d, z=%d\n",
			LSM330_GYR_DEV_NAME, data->x, data->y, data->z);
	/*dev_info(&stat->client->dev, "gyro_out: x = %d, y = %d, z = %d\n",
		data->x, data->y, data->z); */
#endif

	return err;
}

static void lsm330_gyr_report_values(struct lsm330_gyr_status *stat,
						struct lsm330_gyr_triple *data)
{
	input_report_abs(stat->input_dev, ABS_X, data->x);
	input_report_abs(stat->input_dev, ABS_Y, data->y);
	input_report_abs(stat->input_dev, ABS_Z, data->z);
	input_sync(stat->input_dev);
}

static int lsm330_gyr_hw_init(struct lsm330_gyr_status *stat)
{
	int err;
	u8 buf[6];

	dev_info(&stat->client->dev, "hw init\n");

	buf[0] = (CTRL_REG1);
	buf[1] = stat->resume_state[RES_CTRL_REG1];
	buf[2] = stat->resume_state[RES_CTRL_REG2];
	buf[3] = stat->resume_state[RES_CTRL_REG3];
	buf[4] = stat->resume_state[RES_CTRL_REG4];
	buf[5] = stat->resume_state[RES_CTRL_REG5];

	err = lsm330_gyr_i2c_write(stat, buf, 5);
	if (err < 0)
		return err;

	buf[0] = (FIFO_CTRL_REG);
	buf[1] = stat->resume_state[RES_FIFO_CTRL_REG];
	err = lsm330_gyr_i2c_write(stat, buf, 1);
	if (err < 0)
			return err;

	stat->hw_initialized = 1;

	return err;
}

static void lsm330_gyr_device_power_off(struct lsm330_gyr_status *stat)
{
	int err;
	u8 buf[2];

	dev_info(&stat->client->dev, "power off\n");

	buf[0] = (CTRL_REG1);
	buf[1] = (PM_OFF);
	err = lsm330_gyr_i2c_write(stat, buf, 1);
	if (err < 0)
		dev_err(&stat->client->dev, "soft power off failed\n");

	if (stat->pdata->power_off) {
		/* disable_irq_nosync(acc->irq1); */
		disable_irq_nosync(stat->irq2);
		stat->pdata->power_off();
		stat->hw_initialized = 0;
	}

	if (stat->hw_initialized) {
		/*if (stat->pdata->gpio_int1 >= 0)*/
		/*	disable_irq_nosync(stat->irq1);*/
		if (stat->pdata->gpio_int2 >= 0) {
			disable_irq_nosync(stat->irq2);
			dev_info(&stat->client->dev,
					"power off: irq2 disabled\n");
		}
		stat->hw_initialized = 0;
	}
}

static int lsm330_gyr_device_power_on(struct lsm330_gyr_status *stat)
{
	int err;

	if (stat->pdata->power_on) {
		err = stat->pdata->power_on();
		if (err < 0)
			return err;
		if (stat->pdata->gpio_int2 >= 0)
			enable_irq(stat->irq2);
	}


	if (!stat->hw_initialized) {
		err = lsm330_gyr_hw_init(stat);
		if (err < 0) {
			lsm330_gyr_device_power_off(stat);
			return err;
		}
	}

	if (stat->hw_initialized) {
		/* if (stat->pdata->gpio_int1) {
			enable_irq(stat->irq1);
			dev_info(&stat->client->dev,
						"power on: irq1 enabled\n");
		} */
		dev_dbg(&stat->client->dev, "stat->pdata->gpio_int2 = %d\n",
						stat->pdata->gpio_int2);
		if (stat->pdata->gpio_int2 >= 0) {
			enable_irq(stat->irq2);
			dev_info(&stat->client->dev,
					"power on: irq2 enabled\n");
		}
	}

	return 0;
}

static int gyr_regulator_configure(struct lsm330_gyr_status *stat, struct i2c_client *client)
{
	int rc;
	/*if (on == false)
		goto hw_shutdown;*/

	stat->gyr_vdd = regulator_get(&client->dev, "SENSORS_ANALOG_VDD");
	if (IS_ERR(stat->gyr_vdd)) {
		rc = PTR_ERR(stat->gyr_vdd);
		dev_err(&client->dev,
			"Regulator get failed gyr_vdd rc=%d\n", rc);
		return rc;
	}

	rc = regulator_set_voltage(stat->gyr_vdd, 2850000,
							2850000);
		if (rc) {
			dev_err(&client->dev,
				"regulator set_vtg failed rc=%d\n", rc);
			return rc;
		}
	
       stat->gyr_io_vdd = regulator_get(&client->dev, "SENSOR_IO_VDD");
	if (IS_ERR(stat->gyr_io_vdd)) {
		rc = PTR_ERR(stat->gyr_io_vdd);
		dev_err(&client->dev,
			"Regulator get failed acc_io_vdd rc=%d\n", rc);
		return rc;
	}

	return 0;
}
static int  gyr_regulator_enable(struct lsm330_gyr_status *stat)
{
int rc = 1;
rc = regulator_enable(stat->gyr_vdd);
	if (rc) {
		printk(KERN_ERR "Regulator gyr_vdd enable failed rc=%d\n", rc);
		return rc;
		}


	rc = regulator_enable(stat->gyr_io_vdd);
	if (rc) {
		printk(KERN_ERR "Regulator gyr_io_vdd enable failed rc=%d\n", rc);
		return rc;
	}
	return rc ;
}
static int  gyr_regulator_disable(struct lsm330_gyr_status *stat)
{
	int rc = 1;
      rc = regulator_disable(stat->gyr_vdd);
	if (rc) {
		printk(KERN_ERR "Regulator gyr_vdd disable failed rc=%d\n", rc);
		return rc;
		}

	rc = regulator_disable(stat->gyr_io_vdd);
	if (rc) {
		printk(KERN_ERR "Regulator gyr_io_vdd disable failed rc=%d\n", rc);
		return rc;
	}
       return rc;
}


static int lsm330_gyr_enable(struct lsm330_gyr_status *stat)
{
	int err;
	unsigned int delay_ms = 0;

	if (!atomic_cmpxchg(&stat->enabled, 0, 1)) {
           // gyr_regulator_enable(stat);
		err = lsm330_gyr_device_power_on(stat);
		if (err < 0) {
			atomic_set(&stat->enabled, 0);
			return err;
		}
		/*the precision is not enough in the first 150ms after power on*/
		delay_ms = (stat->pdata->poll_interval < 150) ? 150: stat->pdata->poll_interval;
		schedule_delayed_work(&stat->input_work,
				msecs_to_jiffies(delay_ms));
	}

	return 0;
}

static int lsm330_gyr_disable(struct lsm330_gyr_status *stat)
{
	int err;
	dev_dbg(&stat->client->dev, "%s: stat->enabled = %d\n", __func__,
						atomic_read(&stat->enabled));

	if (atomic_cmpxchg(&stat->enabled, 1, 0)) {
		//cancel_delayed_work_sync(&stat->input_work);
		lsm330_gyr_device_power_off(stat);
		dev_dbg(&stat->client->dev, "%s: cancel_delayed_work_sync "
						"result: %d", __func__, err);
		//gyr_regulator_disable(stat);
	}
	return 0;
}

static ssize_t attr_polling_rate_show(struct device *dev,
				     struct device_attribute *attr,
				     char *buf)
{
	int val;
	struct lsm330_gyr_status *stat = dev_get_drvdata(dev);
	mutex_lock(&stat->lock);
	val = stat->pdata->poll_interval;
	mutex_unlock(&stat->lock);
	return snprintf(buf,PAGE_SIZE, "%d\n", val);
}

static ssize_t attr_polling_rate_store(struct device *dev,
				     struct device_attribute *attr,
				     const char *buf, size_t size)
{
	int err;
	struct lsm330_gyr_status *stat = dev_get_drvdata(dev);
	unsigned long interval_ms;

	if (strict_strtoul(buf, 10, &interval_ms))
		return -EINVAL;
	if (!interval_ms)
		return -EINVAL;

	mutex_lock(&stat->lock);
	err = lsm330_gyr_update_odr(stat, interval_ms);
	if(err >= 0)
		stat->pdata->poll_interval = interval_ms;
	mutex_unlock(&stat->lock);
	return size;
}

static ssize_t attr_range_show(struct device *dev,
			       struct device_attribute *attr, char *buf)
{
	struct lsm330_gyr_status *stat = dev_get_drvdata(dev);
	int range = 0;
	u8 val;
	mutex_lock(&stat->lock);
	val = stat->pdata->fs_range;

	switch (val) {
	case LSM330_GYR_FS_250DPS:
		range = 250;
		break;
	case LSM330_GYR_FS_500DPS:
		range = 500;
		break;
	case LSM330_GYR_FS_2000DPS:
		range = 2000;
		break;
	}
	mutex_unlock(&stat->lock);
	/* return sprintf(buf, "0x%02x\n", val); */
	return snprintf(buf, PAGE_SIZE, "%d\n", range);
}

static ssize_t attr_range_store(struct device *dev,
			      struct device_attribute *attr,
			      const char *buf, size_t size)
{
	struct lsm330_gyr_status *stat = dev_get_drvdata(dev);
	unsigned long val;
	u8 range;
	int err;
	if (strict_strtoul(buf, 10, &val))
		return -EINVAL;
	switch (val) {
	case 250:
		range = LSM330_GYR_FS_250DPS;
		break;
	case 500:
		range = LSM330_GYR_FS_500DPS;
		break;
	case 2000:
		range = LSM330_GYR_FS_2000DPS;
		break;
	default:
		dev_err(&stat->client->dev, "invalid range request: %lu,"
				" discarded\n", val);
		return -EINVAL;
	}
	mutex_lock(&stat->lock);
	err = lsm330_gyr_update_fs_range(stat, range);
	if (err >= 0)
		stat->pdata->fs_range = range;
	mutex_unlock(&stat->lock);
	dev_info(&stat->client->dev, "range set to: %lu dps\n", val);
	return size;
}

static ssize_t attr_enable_show(struct device *dev,
			       struct device_attribute *attr, char *buf)
{
	struct lsm330_gyr_status *stat = dev_get_drvdata(dev);
	int val = atomic_read(&stat->enabled);
	return snprintf(buf, PAGE_SIZE, "%d\n", val);
}

static ssize_t attr_enable_store(struct device *dev,
			       struct device_attribute *attr,
			       const char *buf, size_t size)
{
	struct lsm330_gyr_status *stat = dev_get_drvdata(dev);
	unsigned long val;

	if (strict_strtoul(buf, 10, &val))
		return -EINVAL;

	if (val)
		lsm330_gyr_enable(stat);
	else
		lsm330_gyr_disable(stat);

	return size;
}

static ssize_t attr_watermark_store(struct device *dev,
				     struct device_attribute *attr,
				     const char *buf, size_t size)
{
	struct lsm330_gyr_status *stat = dev_get_drvdata(dev);
	unsigned long watermark;
	int res;

	if (strict_strtoul(buf, 16, &watermark))
		return -EINVAL;

	res = lsm330_gyr_update_watermark(stat, watermark);
	if (res < 0)
		return res;

	return size;
}

static ssize_t attr_watermark_show(struct device *dev,
			       struct device_attribute *attr, char *buf)
{
	struct lsm330_gyr_status *stat = dev_get_drvdata(dev);
	int val = stat->watermark;
	return snprintf(buf, PAGE_SIZE, "0x%02x\n", val);
}

static ssize_t attr_fifomode_store(struct device *dev,
				     struct device_attribute *attr,
				     const char *buf, size_t size)
{
	struct lsm330_gyr_status *stat = dev_get_drvdata(dev);
	unsigned long fifomode;
	int res;

	if (strict_strtoul(buf, 16, &fifomode))
		return -EINVAL;
	/* if (!fifomode)
		return -EINVAL; */

	dev_dbg(dev, "%s, got value:0x%02x\n", __func__, (u8)fifomode);

	mutex_lock(&stat->lock);
	res = lsm330_gyr_manage_int2settings(stat, (u8) fifomode);
	mutex_unlock(&stat->lock);

	if (res < 0)
		return res;
	return size;
}

static ssize_t attr_fifomode_show(struct device *dev,
			       struct device_attribute *attr, char *buf)
{
	struct lsm330_gyr_status *stat = dev_get_drvdata(dev);
	u8 val = stat->fifomode;
	return snprintf(buf, PAGE_SIZE, "0x%02x\n", val);
}

static ssize_t attr_reg_set(struct device *dev, struct device_attribute *attr,
				const char *buf, size_t size)
{
	int rc;
	struct lsm330_gyr_status *stat = dev_get_drvdata(dev);
	u8 x[2];
	unsigned long val;

	if (strict_strtoul(buf, 16, &val))
		return -EINVAL;
	mutex_lock(&stat->lock);
	x[0] = stat->reg_addr;
	mutex_unlock(&stat->lock);
	x[1] = val;
	rc = lsm330_gyr_i2c_write(stat, x, 1);
	return size;
}

static ssize_t attr_reg_get(struct device *dev, struct device_attribute *attr,
				char *buf)
{
	ssize_t ret;
	struct lsm330_gyr_status *stat = dev_get_drvdata(dev);
	int rc;
	u8 data;

	mutex_lock(&stat->lock);
	data = stat->reg_addr;
	mutex_unlock(&stat->lock);
	rc = lsm330_gyr_i2c_read(stat, &data, 1);
	ret = snprintf(buf, PAGE_SIZE, "0x%02x\n", data);
	return ret;
}

static ssize_t attr_addr_set(struct device *dev, struct device_attribute *attr,
				const char *buf, size_t size)
{
	struct lsm330_gyr_status *stat = dev_get_drvdata(dev);
	unsigned long val;

	if (strict_strtoul(buf, 16, &val))
		return -EINVAL;

	mutex_lock(&stat->lock);

	stat->reg_addr = val;

	mutex_unlock(&stat->lock);

	return size;
}

/* the set function just for debug*/
static ssize_t attr_set_selftest_status(struct device *dev,
		struct device_attribute *attr,	const char *buf, size_t size)
{
	unsigned long val = simple_strtol(buf, NULL, 10);
	atomic_set(&selftest_err_code, val);
	return size;
}
static ssize_t attr_get_selftest_status(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%s\n", selftest_err_msg[atomic_read(&selftest_err_code)]);
}

static struct device_attribute attributes[] = {
	__ATTR(pollrate_ms, 0664, attr_polling_rate_show,
						attr_polling_rate_store),
	__ATTR(range, 0644, attr_range_show, attr_range_store),
	__ATTR(enable_device, 0664, attr_enable_show, attr_enable_store),
	__ATTR(fifo_samples, 0644, attr_watermark_show, attr_watermark_store),
	__ATTR(fifo_mode, 0644, attr_fifomode_show, attr_fifomode_store),
	__ATTR(reg_value, 0600, attr_reg_get, attr_reg_set),
	__ATTR(reg_addr, 0200, NULL, attr_addr_set),
	__ATTR(selftest_status, 0644, attr_get_selftest_status, NULL),
};

static int create_sysfs_interfaces(struct device *dev)
{
	int i;
	for (i = 0; i < ARRAY_SIZE(attributes); i++)
		if (device_create_file(dev, attributes + i))
			goto error;
	return 0;

error:
	for (; i >= 0; i--)
		device_remove_file(dev, attributes + i);
	dev_err(dev, "%s:Unable to create interface\n", __func__);
	return -1;
}

static int remove_sysfs_interfaces(struct device *dev)
{
	int i;
	for (i = 0; i < ARRAY_SIZE(attributes); i++)
		device_remove_file(dev, attributes + i);
	return 0;
}

static void lsm330_gyr_report_triple(struct lsm330_gyr_status *stat)
{
	int err;
	struct lsm330_gyr_triple data_out;

	err = lsm330_gyr_get_data(stat, &data_out);
	if (err < 0)
		dev_err(&stat->client->dev, "get_gyroscope_data failed\n");
	else
		lsm330_gyr_report_values(stat, &data_out);
}

static void lsm330_gyr_irq2_fifo(struct lsm330_gyr_status *stat)
{
	int err;
	u8 buf[2];
	u8 int_source;
	u8 samples;
	u8 workingmode;
	u8 stored_samples;

	mutex_lock(&stat->lock);

	workingmode = stat->fifomode;


	dev_dbg(&stat->client->dev, "%s : fifomode:0x%02x\n", __func__,
								workingmode);


	switch (workingmode) {
	case FIFO_MODE_BYPASS:
	{
		dev_dbg(&stat->client->dev, "%s : fifomode:0x%02x\n", __func__,
							stat->fifomode);
		lsm330_gyr_report_triple(stat);
		break;
	}
	case FIFO_MODE_FIFO:
		samples = (stat->watermark)+1;
		dev_dbg(&stat->client->dev,
			"%s : FIFO_SRC_REG init samples:%d\n",
							__func__, samples);
		err = lsm330_gyr_register_read(stat, buf, FIFO_SRC_REG);
		if (err < 0)
			dev_err(&stat->client->dev,
					"error reading fifo source reg\n");

		int_source = buf[0];
		dev_dbg(&stat->client->dev, "%s :FIFO_SRC_REG content:0x%02x\n",
							__func__, int_source);

		stored_samples = int_source & FIFO_STORED_DATA_MASK;
		dev_dbg(&stat->client->dev, "%s : fifomode:0x%02x\n", __func__,
						stat->fifomode);

		dev_dbg(&stat->client->dev, "%s : samples:%d stored:%d\n",
				__func__, samples, stored_samples);

		for (; samples > 0; samples--) {
			dev_dbg(&stat->client->dev, "%s : current sample:%d\n",
							__func__, samples);

			lsm330_gyr_report_triple(stat);

		}
		lsm330_gyr_fifo_reset(stat);
		break;
	}

	mutex_unlock(&stat->lock);
}

static irqreturn_t lsm330_gyr_isr2(int irq, void *dev)
{
	struct lsm330_gyr_status *stat = dev;

	disable_irq_nosync(irq);
	queue_work(stat->irq2_work_queue, &stat->irq2_work);
	pr_debug("%s %s: isr2 queued\n", LSM330_GYR_DEV_NAME, __func__);

	return IRQ_HANDLED;
}

static void lsm330_gyr_irq2_work_func(struct work_struct *work)
{

	struct lsm330_gyr_status *stat;
	stat = container_of(work, struct lsm330_gyr_status, irq2_work);
	/* TODO  add interrupt service procedure.
		 ie:lsm330_gyr_irq2_XXX(stat); */
	lsm330_gyr_irq2_fifo(stat);
	/*  */
	pr_debug("%s %s: IRQ2 served\n", LSM330_GYR_DEV_NAME, __func__);
/* exit: */
	enable_irq(stat->irq2);
}

static void lsm330_gyr_input_work_func(struct work_struct *work)
{
	struct lsm330_gyr_status *stat;
	struct lsm330_gyr_triple data_out;
	int err;

	stat = container_of(work, struct lsm330_gyr_status, input_work.work);
	mutex_lock(&stat->lock);
	err = lsm330_gyr_get_data(stat, &data_out);
	if (err < 0)
		dev_err(&stat->client->dev, "get_acceleration_data failed\n");
	else
		lsm330_gyr_report_values(stat, &data_out);
	if(atomic_read(&stat->enabled))
		schedule_delayed_work(&stat->input_work, msecs_to_jiffies(stat->pdata->poll_interval));  //stat->pdata->poll_interval
	mutex_unlock(&stat->lock);
}

int lsm330_gyr_input_open(struct input_dev *input)
{
	struct lsm330_gyr_status *stat = input_get_drvdata(input);
	dev_dbg(&stat->client->dev, "%s\n", __func__);
	return lsm330_gyr_enable(stat);
}

void lsm330_gyr_input_close(struct input_dev *dev)
{
	struct lsm330_gyr_status *stat = input_get_drvdata(dev);
	dev_dbg(&stat->client->dev, "%s\n", __func__);
	lsm330_gyr_disable(stat);
}

static int lsm330_gyr_validate_pdata(struct lsm330_gyr_status *stat)
{
	/* checks for correctness of minimal polling period */
	stat->pdata->min_interval =
		max((unsigned int) LSM330_GYR_MIN_POLL_PERIOD_MS,
						stat->pdata->min_interval);

	stat->pdata->poll_interval = max(stat->pdata->poll_interval,
			stat->pdata->min_interval);

	if (stat->pdata->axis_map_x > 2 ||
	    stat->pdata->axis_map_y > 2 ||
	    stat->pdata->axis_map_z > 2) {
		dev_err(&stat->client->dev,
			"invalid axis_map value x:%u y:%u z%u\n",
			stat->pdata->axis_map_x,
			stat->pdata->axis_map_y,
			stat->pdata->axis_map_z);
		return -EINVAL;
	}

	/* Only allow 0 and 1 for negation boolean flag */
	if (stat->pdata->negate_x > 1 ||
	    stat->pdata->negate_y > 1 ||
	    stat->pdata->negate_z > 1) {
		dev_err(&stat->client->dev,
			"invalid negate value x:%u y:%u z:%u\n",
			stat->pdata->negate_x,
			stat->pdata->negate_y,
			stat->pdata->negate_z);
		return -EINVAL;
	}

	/* Enforce minimum polling interval */
	if (stat->pdata->poll_interval < stat->pdata->min_interval) {
		dev_err(&stat->client->dev,
			"minimum poll interval violated\n");
		return -EINVAL;
	}
	return 0;
}

static int lsm330_gyr_input_init(struct lsm330_gyr_status *stat)
{
	int err = -1;

	dev_dbg(&stat->client->dev, "%s\n", __func__);

	INIT_DELAYED_WORK(&stat->input_work, lsm330_gyr_input_work_func);
	stat->input_dev = input_allocate_device();
	if (!stat->input_dev) {
		err = -ENOMEM;
		dev_err(&stat->client->dev,
			"input device allocation failed\n");
		goto err0;
	}
	stat->input_dev->open = lsm330_gyr_input_open;
	stat->input_dev->close = lsm330_gyr_input_close;
	stat->input_dev->name = GYRO_INPUT_DEV_NAME;
	stat->input_dev->id.bustype = BUS_I2C;
	stat->input_dev->dev.parent = &stat->client->dev;
	input_set_drvdata(stat->input_dev, stat);
	set_bit(EV_ABS, stat->input_dev->evbit);
	input_set_abs_params(stat->input_dev, ABS_X, -FS_MAX-1, FS_MAX, 0, 0);
	input_set_abs_params(stat->input_dev, ABS_Y, -FS_MAX-1, FS_MAX, 0, 0);
	input_set_abs_params(stat->input_dev, ABS_Z, -FS_MAX-1, FS_MAX, 0, 0);

	err = input_register_device(stat->input_dev);
	if (err) {
		dev_err(&stat->client->dev,
			"unable to register input polled device %s\n",
			stat->input_dev->name);
		goto err1;
	}

	return 0;
err1:
	input_free_device(stat->input_dev);
err0:
	return err;
}

static void lsm330_gyr_input_cleanup(struct lsm330_gyr_status *stat)
{
	input_unregister_device(stat->input_dev);
	input_free_device(stat->input_dev);
}

static int lsm330_gyr_selftest(struct lsm330_gyr_status *gyro)
{
	int err = -1;
	u8 buf[2] = {0x00, 0x00};
	char reg_address, mask, bit_values;

	reg_address = CTRL_REG4;
	mask = SELFTEST_MASK;
	bit_values = LSM330_SELFTEST_EN_POS;

	mutex_lock(&gyro->lock);
	err = lsm330_gyr_register_update(gyro, buf, reg_address,
		mask, bit_values);
	if (err < 0) {
		dev_err(&gyro->client->dev, "%s : failed to update:0x%02x\n",
			      __func__, bit_values);
		mutex_unlock(&gyro->lock);
		return err;
	}
	mutex_unlock(&gyro->lock);
	return err;
}

int set_selftest_lm330(int value)
{
	int err, result;
	int data;
	struct lsm330_gyr_triple data_out;
	 u8 old_ctrl_reg_state[5];
	if(stat_temp == NULL ||stat_temp->pdata == NULL) return SELF_TEST_FAIL;
	if (SELF_TEST == value) {
		old_ctrl_reg_state[0] = stat_temp->resume_state[RES_CTRL_REG1] ;
		old_ctrl_reg_state[1] = stat_temp->resume_state[RES_CTRL_REG2] ;
		old_ctrl_reg_state[2] = stat_temp->resume_state[RES_CTRL_REG3] ;
		old_ctrl_reg_state[3] = stat_temp->resume_state[RES_CTRL_REG4] ;
		old_ctrl_reg_state[4] = stat_temp->resume_state[RES_CTRL_REG5] ;

		stat_temp->resume_state[RES_CTRL_REG1] = 0x6F;
		stat_temp->resume_state[RES_CTRL_REG2] = 0x00;
		stat_temp->resume_state[RES_CTRL_REG3] = 0x00;
		stat_temp->resume_state[RES_CTRL_REG4] = 0xA0;
		stat_temp->resume_state[RES_CTRL_REG5] = 0x02;

		err = lsm330_gyr_device_power_on(stat_temp);
			  if(err){
				atomic_set(&selftest_err_code, 0x06);
				dev_err(&stat_temp->client->dev, "gyroscope power on failed err= %d\n", err);
				result = SELF_TEST_FAIL;
				goto TEST_RESULT;
			  }
		lsm330_gyr_selftest(stat_temp);
		msleep(500);

		mutex_lock(&stat_temp->lock);
		err = lsm330_gyr_get_data(stat_temp, &data_out);
		mutex_unlock(&stat_temp->lock);
		if (err < 0) {
			atomic_set(&selftest_err_code, 0x07);
			dev_err(&stat_temp->client->dev, "get_gyroscope_data failed err= %d\n",  err);
			result = SELF_TEST_FAIL;
			goto TEST_RESULT;
		}

		/*self_test value*/
		data = abs(data_out.x);
		if ((data > MAX_VAL) || (data < MIN_VAL)) {
			atomic_set(&selftest_err_code, 0x03);
			dev_err(&stat_temp->client->dev, "selftest_gyro-------MAX-MIN--X-==%d\n", data);
			result = SELF_TEST_FAIL;
			goto TEST_RESULT;
		}
		data = abs(data_out.y);
		if ((data > MAX_VAL) || (data < MIN_VAL)) {
			atomic_set(&selftest_err_code, 0x04);
			dev_err(&stat_temp->client->dev, "selftest_gyro-------MAX-MIN--Y-==%d\n", data);
			result = SELF_TEST_FAIL;
			goto TEST_RESULT;
		}
		data = abs(data_out.z);
		if ((data > MAX_VAL) || (data < MIN_VAL)) {
			atomic_set(&selftest_err_code, 0x05);
			dev_err(&stat_temp->client->dev, "selftest_gyro-------MAX-MIN--Z-==%d\n", data);
			result = SELF_TEST_FAIL;
			goto TEST_RESULT;
		}
		result = SELF_TEST_OK;
	TEST_RESULT:
		msleep(50);
		//l3g4200d_selftest(gyro_temp, 0);
		//l3g4200d_device_power_off(gyro_temp);

		stat_temp->resume_state[RES_CTRL_REG1] = old_ctrl_reg_state[0];
		stat_temp->resume_state[RES_CTRL_REG2] = old_ctrl_reg_state[1];
		stat_temp->resume_state[RES_CTRL_REG3] = old_ctrl_reg_state[2];
		stat_temp->resume_state[RES_CTRL_REG4] = old_ctrl_reg_state[3];
		stat_temp->resume_state[RES_CTRL_REG5] = old_ctrl_reg_state[4];

		err = lsm330_gyr_hw_init(stat_temp);
		if (err < 0) {
			atomic_set(&selftest_err_code, 0x09);
			dev_err(&stat_temp->client->dev, "gyro set self test reg resume fail\n");
		}
		if (0==atomic_read(&stat_temp->enabled)){
			stat_temp->hw_initialized = 0;
		}

		dev_info(&stat_temp->client->dev, "selftest_gyro-------out, result = %d\n", result);
		return result;
	}
	return SELF_TEST_FAIL;
}

#ifdef CONFIG_HAS_EARLYSUSPEND
static int lsm330_gyr_early_suspend(struct early_suspend *h)
{
	int err = 0;
	struct lsm330_gyr_status *stat = container_of(h, struct lsm330_gyr_status, early_suspend);
	struct i2c_client *client = stat->client;
	u8 buf[2];
	//printk("-----------lsm330gy early suspend  begin-----\n");
	dev_dbg(&client->dev, "%s\n", __func__);
	if (stat->on_before_suspend = atomic_read(&stat->enabled)) {

		mutex_lock(&stat->lock);
		atomic_set(&stat->enabled, 0);
#ifdef SLEEP
		err = lsm330_gyr_register_update(stat, buf, CTRL_REG1,
				0x0F, (ENABLE_NO_AXES | PM_NORMAL));
#else
		err = lsm330_gyr_register_update(stat, buf, CTRL_REG1,
				0x08, PM_OFF);
#endif 
		mutex_unlock(&stat->lock);
	}
	return err;
}

static int lsm330_gyr_later_resume(struct early_suspend *h)
{
	int err = 0;
	//printk("-----------lsm330gy later resume begin-----\n");
	struct lsm330_gyr_status *stat = container_of(h, struct lsm330_gyr_status, early_suspend);
	struct i2c_client *client = stat->client;
	u8 buf[2];

	dev_dbg(&client->dev, "%s\n", __func__);
	if (stat->on_before_suspend) {
		mutex_lock(&stat->lock);
		schedule_delayed_work(&stat->input_work,
					msecs_to_jiffies(stat->pdata->poll_interval));
#ifdef SLEEP
		err = lsm330_gyr_register_update(stat, buf, CTRL_REG1,
				0x0F, (ENABLE_ALL_AXES | PM_NORMAL));
#else
		err = lsm330_gyr_register_update(stat, buf, CTRL_REG1,
				0x08, PM_NORMAL);
#endif
		if(err)
		{
			dev_err(&client->dev,
				"failed to allocate memory for module data\n");
			mutex_unlock(&stat->lock);
			return err;
		}
		atomic_set(&stat->enabled, 1);
		mutex_unlock(&stat->lock);
	}
	return err;
}
#endif
static int lsm330_gyr_probe(struct i2c_client *client,
					const struct i2c_device_id *devid)
{
      int error;
	struct lsm330_gyr_status *stat;

	u32 smbus_func = I2C_FUNC_SMBUS_BYTE_DATA |
			I2C_FUNC_SMBUS_WORD_DATA | I2C_FUNC_SMBUS_I2C_BLOCK ;
	u8 chipid = 0;
	int err = -1;

	dev_info(&client->dev, "probe start.\n");


	stat = kzalloc(sizeof(*stat), GFP_KERNEL);
	if (stat == NULL) {
		dev_err(&client->dev,
			"failed to allocate memory for module data\n");
		err = -ENOMEM;
		goto err0;
	}
	stat_temp = stat;
      
/* set regulator*/
	error = gyr_regulator_configure(stat, client);
	if (error) {
		dev_err(&client->dev, "Failed to intialize gyr_regulator hardware\n");
		goto err1;
		
	}
      gyr_regulator_enable(stat);
	  
	/* Support for both I2C and SMBUS adapter interfaces. */
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		dev_warn(&client->dev, "client not i2c capable\n");
		if (i2c_check_functionality(client->adapter, smbus_func)) {
			stat->use_smbus = 1;
			dev_warn(&client->dev, "client using SMBUS\n");
		} else {
			err = -ENODEV;
			dev_err(&client->dev, "client nor SMBUS capable\n");
			stat->use_smbus = 0;
			goto err0;
		}
	}
	mutex_init(&stat->lock);
	mutex_lock(&stat->lock);
	stat->client = client;

	chipid = WHO_AM_I;
	err = lsm330_gyr_i2c_read(stat, &chipid, sizeof(chipid));
	if (err < 0) {
		dev_err(&client->dev, "lsm330 gyro_i2c_read err= %d\n", err);
		goto err1;
	}

	if (chipid != WHOAMI_LSM330_GYR) {
		dev_err(&client->dev,
			"device unknown. Expected: 0x%x, Replies: 0x%x\n", WHOAMI_LSM330_GYR, chipid);
		err = -ENODEV;
		goto err1;
	}

	dev_info(&client->dev, "Read lsm330 chip ok, ID is 0x%x\n", chipid);

	err = set_sensor_chip_info(GYRO, "ST LSM330");
	if (err) {
		dev_err(&client->dev, "set_sensor_chip_info error\n");
	}

	stat->pdata = kmalloc(sizeof(*stat->pdata), GFP_KERNEL);
	if (stat->pdata == NULL) {
		dev_err(&client->dev,
			"failed to allocate memory for pdata: %d\n", err);
		goto err1;
	}

	if (client->dev.platform_data == NULL) {
		default_lsm330_gyr_pdata.gpio_int1 = int1_gpio;
		default_lsm330_gyr_pdata.gpio_int2 = int2_gpio;
		memcpy(stat->pdata, &default_lsm330_gyr_pdata,
							sizeof(*stat->pdata));
		dev_info(&client->dev, "using default plaform_data\n");
	} else {
		memcpy(stat->pdata, client->dev.platform_data,
						sizeof(*stat->pdata));
	}

	err = lsm330_gyr_validate_pdata(stat);
	if (err < 0) {
		dev_err(&client->dev, "failed to validate platform data\n");
		goto err1_1;
	}

	i2c_set_clientdata(client, stat);

	if (stat->pdata->init) {
		err = stat->pdata->init();
		if (err < 0) {
			dev_err(&client->dev, "init failed: %d\n", err);
			goto err1_1;
		}
	}


	memset(stat->resume_state, 0, ARRAY_SIZE(stat->resume_state));

	stat->resume_state[RES_CTRL_REG1] = ALL_ZEROES | ENABLE_ALL_AXES
								| PM_NORMAL;
	stat->resume_state[RES_CTRL_REG2] = ALL_ZEROES;
	stat->resume_state[RES_CTRL_REG3] = ALL_ZEROES;
	stat->resume_state[RES_CTRL_REG4] = ALL_ZEROES | BDU_ENABLE;
	stat->resume_state[RES_CTRL_REG5] = ALL_ZEROES;
	stat->resume_state[RES_FIFO_CTRL_REG] = ALL_ZEROES;

	err = lsm330_gyr_device_power_on(stat);
	if (err < 0) {
		dev_err(&client->dev, "power on failed: %d\n", err);
		goto err2;
	}

	atomic_set(&stat->enabled, 1);

	err = lsm330_gyr_update_fs_range(stat, stat->pdata->fs_range);
	if (err < 0) {
		dev_err(&client->dev, "update_fs_range failed\n");
		goto err2;
	}

	err = lsm330_gyr_update_odr(stat, stat->pdata->poll_interval);
	if (err < 0) {
		dev_err(&client->dev, "update_odr failed\n");
		goto err2;
	}

	err = lsm330_gyr_input_init(stat);
	if (err < 0)
		goto err3;

	err = create_sysfs_interfaces(&client->dev);
	if (err < 0) {
		dev_err(&client->dev,
			"%s device register failed\n", LSM330_GYR_DEV_NAME);
		goto err4;
	}

	lsm330_gyr_device_power_off(stat);

	/* As default, do not report information */
	atomic_set(&stat->enabled, 0);


	if (stat->pdata->gpio_int2 >= 0) {
		stat->irq2 = gpio_to_irq(stat->pdata->gpio_int2);
		dev_info(&client->dev, "%s: %s has set irq2 to irq:"
						" %d mapped on gpio:%d\n",
			LSM330_GYR_DEV_NAME, __func__, stat->irq2,
							stat->pdata->gpio_int2);

		INIT_WORK(&stat->irq2_work, lsm330_gyr_irq2_work_func);
		stat->irq2_work_queue =
			create_singlethread_workqueue("lsm330_gyr_irq2_wq");
		if (!stat->irq2_work_queue) {
			err = -ENOMEM;
			dev_err(&client->dev, "cannot create "
						"work queue2: %d\n", err);
			goto err5;
		}

		err = request_irq(stat->irq2, lsm330_gyr_isr2,
				IRQF_TRIGGER_HIGH, "lsm330_gyr_irq2", stat);

		if (err < 0) {
			dev_err(&client->dev, "request irq2 failed: %d\n", err);
			goto err6;
		}
		disable_irq_nosync(stat->irq2);
	}
#ifdef CONFIG_HAS_EARLYSUSPEND
	stat->early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 1;
	stat->early_suspend.suspend = lsm330_gyr_early_suspend;
	stat->early_suspend.resume = lsm330_gyr_later_resume;
	register_early_suspend(&stat->early_suspend);
#endif
	mutex_unlock(&stat->lock);


	dev_info(&client->dev, "%s probed: device created successfully\n",
							LSM330_GYR_DEV_NAME);
#ifdef CONFIG_HUAWEI_HW_DEV_DCT
	/* detect current device successful, set the flag as present */
	set_hw_dev_flag(DEV_I2C_GYROSCOPE);
#endif
  //gyr_regulator_disable(stat);

	return 0;

/*err7:
	free_irq(stat->irq2, stat);
*/
err6:
	destroy_workqueue(stat->irq2_work_queue);
err5:
	lsm330_gyr_device_power_off(stat);
	remove_sysfs_interfaces(&client->dev);
err4:
	lsm330_gyr_input_cleanup(stat);
err3:
	lsm330_gyr_device_power_off(stat);
err2:
	if (stat->pdata->exit)
		stat->pdata->exit();
err1_1:
	mutex_unlock(&stat->lock);
	kfree(stat->pdata);
err1:
	kfree(stat);
	stat_temp = NULL;
err0:
		pr_err("%s: Driver Initialization failed\n",
							LSM330_GYR_DEV_NAME);
		return err;
}

static int __devexit lsm330_gyr_remove(struct i2c_client *client)
{
	struct lsm330_gyr_status *stat = i2c_get_clientdata(client);

	dev_info(&stat->client->dev, "driver removing\n");


	/*
	if (stat->pdata->gpio_int1 >= 0)
	{
		free_irq(stat->irq1, stat);
		gpio_free(stat->pdata->gpio_int1);
		destroy_workqueue(stat->irq1_work_queue);
	}
	*/
	if (stat->pdata->gpio_int2 >= 0) {
		free_irq(stat->irq2, stat);
		gpio_free(stat->pdata->gpio_int2);
		destroy_workqueue(stat->irq2_work_queue);
	}

	if (atomic_cmpxchg(&stat->enabled, 1, 0))
				cancel_delayed_work_sync(&stat->input_work);

	lsm330_gyr_disable(stat);
	lsm330_gyr_input_cleanup(stat);

	remove_sysfs_interfaces(&client->dev);

	kfree(stat->pdata);
	kfree(stat);
	return 0;
}

static const struct i2c_device_id lsm330_gyr_id[] = {
	{ LSM330_GYR_DEV_NAME , 0 },
	{},
};

MODULE_DEVICE_TABLE(i2c, lsm330_gyr_id);

static struct i2c_driver lsm330_gyr_driver = {
	.driver = {
			.owner = THIS_MODULE,
			.name = LSM330_GYR_DEV_NAME,
			//.pm = &lsm330_gyr_pm,
	},
	.probe = lsm330_gyr_probe,
	.remove = __devexit_p(lsm330_gyr_remove),
	.id_table = lsm330_gyr_id,

};

static int __init lsm330_gyr_init(void)
{

	pr_info("%s: gyroscope sysfs driver init\n", LSM330_GYR_DEV_NAME);

	return i2c_add_driver(&lsm330_gyr_driver);
}

static void __exit lsm330_gyr_exit(void)
{

	pr_info("%s exit\n", LSM330_GYR_DEV_NAME);
	i2c_del_driver(&lsm330_gyr_driver);
	return;
}

module_init(lsm330_gyr_init);
module_exit(lsm330_gyr_exit);

MODULE_DESCRIPTION("lsm330 gyroscope driver");
MODULE_AUTHOR("Matteo Dameno, Denis Ciocca, STMicroelectronics");
MODULE_LICENSE("GPL");

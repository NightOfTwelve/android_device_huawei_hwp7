
/* DO NOT EDIT - Generated automatically by battery_para_gen.pl */




static struct single_row_lut guangyu_4000_fcc_temp = {
	.x		= {-20, -10, 0, 25, 40, 60},
	.y		= {3898, 3901, 3893, 3883, 3894, 3860},
	.cols	= 6
};


static struct single_row_lut guangyu_4000_fcc_sf = {
	.x		= {0, 100, 200, 300, 400, 500},
	.y		= {100, 96, 94, 92, 90, 88},
	.cols	= 6
};


static struct sf_lut guangyu_4000_pc_sf = {
	.rows = 1,
	.cols = 1,
	.row_entries = {0},
	.percent = {100},
	.sf = {
		{100}
	}
};


static struct sf_lut guangyu_4000_rbatt_sf = {
	.rows = 28,
	.cols = 6,
	.row_entries = {-20, -10, 0, 25, 40, 60},
	.percent = {100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
	.sf = {
		{902, 391, 212, 100, 84, 82},
		{897, 399, 217, 101, 85, 82},
		{892, 408, 221, 103, 86, 83},
		{885, 415, 227, 104, 86, 83},
		{893, 430, 239, 108, 88, 84},
		{834, 438, 257, 113, 90, 85},
		{830, 401, 243, 121, 93, 87},
		{828, 395, 229, 131, 100, 90},
		{834, 390, 216, 122, 106, 95},
		{860, 393, 216, 104, 93, 90},
		{890, 401, 217, 103, 86, 83},
		{927, 422, 217, 105, 88, 84},
		{969, 449, 224, 106, 89, 85},
		{1020, 483, 237, 108, 91, 87},
		{1078, 523, 258, 109, 90, 86},
		{1149, 569, 280, 108, 89, 84},
		{1245, 633, 301, 109, 88, 85},
		{1386, 744, 349, 109, 88, 84},
		{2274, 1071, 448, 113, 90, 86},
		{2844, 1211, 449, 112, 90, 87},
		{3068, 1327, 489, 113, 89, 88},
		{3283, 1428, 528, 114, 90, 88},
		{3572, 1545, 559, 116, 91, 88},
		{3884, 1698, 590, 118, 90, 87},
		{4204, 1877, 646, 119, 91, 88},
		{4686, 2081, 704, 125, 94, 104},
		{5275, 2398, 806, 192, 131, 104},
		{13185, 2559, 814, 208, 131, 104 },
	}
};


static struct pc_temp_ocv_lut guangyu_4000_pc_temp_ocv = {
	.rows = 29,
	.cols = 6,
	.temp = {-20, -10, 0, 25, 40, 60},
	.percent = {100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0},
	.ocv = {
		{4345, 4341, 4334, 4333, 4326, 4316},
		{4244, 4256, 4262, 4270, 4266, 4256},
		{4172, 4194, 4203, 4213, 4210, 4201},
		{4109, 4137, 4147, 4159, 4156, 4148},
		{4059, 4086, 4096, 4107, 4103, 4096},
		{3977, 4033, 4048, 4059, 4055, 4048},
		{3927, 3963, 3986, 4011, 4009, 4003},
		{3886, 3916, 3936, 3968, 3969, 3963},
		{3850, 3876, 3890, 3918, 3925, 3921},
		{3829, 3844, 3858, 3872, 3876, 3875},
		{3813, 3817, 3830, 3843, 3842, 3840},
		{3801, 3801, 3806, 3819, 3819, 3816},
		{3787, 3789, 3790, 3800, 3800, 3798},
		{3771, 3776, 3778, 3784, 3784, 3782},
		{3753, 3759, 3766, 3769, 3767, 3760},
		{3732, 3737, 3746, 3752, 3750, 3737},
		{3709, 3713, 3716, 3727, 3728, 3715},
		{3678, 3692, 3694, 3695, 3694, 3681},
		{3612, 3657, 3678, 3682, 3674, 3663},
		{3592, 3646, 3671, 3680, 3673, 3661},
		{3572, 3633, 3664, 3677, 3671, 3659},
		{3548, 3614, 3655, 3673, 3668, 3656},
		{3526, 3591, 3641, 3664, 3662, 3648},
		{3504, 3565, 3617, 3642, 3644, 3626},
		{3472, 3534, 3584, 3601, 3604, 3586},
		{3439, 3497, 3541, 3543, 3550, 3534},
		{3395, 3453, 3484, 3471, 3482, 3464},
		{3318, 3363, 3383, 3364, 3377, 3364},
		{3200, 3200, 3200, 3200, 3200, 3200}
	}
};


struct hisi_smartstar_coul_battery_data guangyu_4000_battery_data = {
	.id_voltage_min = 500,
	.id_voltage_max = 900,
	.fcc = 3920,
	.fcc_temp_lut  = &guangyu_4000_fcc_temp,
	.fcc_sf_lut = &guangyu_4000_fcc_sf,
	.pc_temp_ocv_lut = &guangyu_4000_pc_temp_ocv,
	.pc_sf_lut = &guangyu_4000_pc_sf,
	.rbatt_sf_lut = &guangyu_4000_rbatt_sf,
	.default_rbatt_mohm = 156,
	.max_currentmA = 1900,
    .max_voltagemV = 4352,
    .max_cin_currentmA = 2000,
    .terminal_currentmA = 150,
    .charge_in_temp_5 = 1,
    .charge_in_temp_10 = 1,
    .batt_brand = "GuangYu",
};
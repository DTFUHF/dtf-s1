######################################
# Makefile by CubeMX2Makefile.py
######################################

######################################
# target
######################################
TARGET = dtf-s1

######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -O0

.PRECIOUS: %.s

#######################################
# pathes
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
C_SOURCES = \
  Src/main.c \
  Src/frsky2way_common.c \
  Src/dma.c \
  Src/stm32f0xx_hal_msp.c \
  Src/config.c \
  Src/crc.c \
  Src/drv_spi.c \
  Src/cc2500.c \
  Src/usbd_conf.c \
  Src/usb_device.c \
  Src/rc_output.c \
  Src/cc2500_tx_carrier_test.c \
  Src/tim.c \
  Src/drv_ppm.c \
  Src/usbd_desc.c \
  Src/stm32f0xx_it.c \
  Src/spi.c \
  Src/drv_sbus.c \
  Src/frsky2way_rx.c \
  Src/gpio.c \
  Src/frsky2way_rx_normal.c \
  Src/frsky2way_rx_binding.c \
  Src/drv_sumd.c \
  Src/usart.c \
#  Middlewares/Third_Party/FreeRTOS/Source/list.c \
#  Middlewares/Third_Party/FreeRTOS/Source/timers.c \
#  Middlewares/Third_Party/FreeRTOS/Source/tasks.c \
#  Middlewares/Third_Party/FreeRTOS/Source/croutine.c \
#  Middlewares/Third_Party/FreeRTOS/Source/event_groups.c \
#  Middlewares/Third_Party/FreeRTOS/Source/queue.c \
#  Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c \
#  Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c \
#  Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_5.c \
#  Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_2.c \
#  Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_1.c \
#  Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_3.c \
#  Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM0/port.c \
  Middlewares/Third_Party/FatFs/src/ff_gen_drv.c \
  Middlewares/Third_Party/FatFs/src/ff.c \
  Middlewares/Third_Party/FatFs/src/diskio.c \
  Middlewares/Third_Party/FatFs/src/option/cc932.c \
  Middlewares/Third_Party/FatFs/src/option/cc949.c \
  Middlewares/Third_Party/FatFs/src/option/cc936.c \
  Middlewares/Third_Party/FatFs/src/option/unicode.c \
  Middlewares/Third_Party/FatFs/src/option/syscall.c \
  Middlewares/Third_Party/FatFs/src/option/ccsbcs.c \
  Middlewares/Third_Party/FatFs/src/option/cc950.c \
  Middlewares/ST/STM32_USB_Device_Library/Class/HID/Src/usbd_hid.c \
  Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c \
  Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c \
  Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c \
  Drivers/CMSIS/DSP_Lib/Source/StatisticsFunctions/arm_min_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/StatisticsFunctions/arm_mean_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/StatisticsFunctions/arm_rms_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/StatisticsFunctions/arm_mean_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/StatisticsFunctions/arm_rms_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/StatisticsFunctions/arm_power_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/StatisticsFunctions/arm_rms_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/StatisticsFunctions/arm_var_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/StatisticsFunctions/arm_power_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/StatisticsFunctions/arm_max_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/StatisticsFunctions/arm_max_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/StatisticsFunctions/arm_mean_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/StatisticsFunctions/arm_var_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/StatisticsFunctions/arm_mean_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/StatisticsFunctions/arm_min_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/StatisticsFunctions/arm_std_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/StatisticsFunctions/arm_power_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/StatisticsFunctions/arm_max_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/StatisticsFunctions/arm_min_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/StatisticsFunctions/arm_min_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/StatisticsFunctions/arm_max_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/StatisticsFunctions/arm_std_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/StatisticsFunctions/arm_std_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/StatisticsFunctions/arm_var_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/StatisticsFunctions/arm_power_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/SupportFunctions/arm_q31_to_float.c \
  Drivers/CMSIS/DSP_Lib/Source/SupportFunctions/arm_fill_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/SupportFunctions/arm_q15_to_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/SupportFunctions/arm_fill_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/SupportFunctions/arm_fill_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/SupportFunctions/arm_q15_to_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/SupportFunctions/arm_q7_to_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/SupportFunctions/arm_copy_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/SupportFunctions/arm_copy_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/SupportFunctions/arm_fill_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/SupportFunctions/arm_q7_to_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/SupportFunctions/arm_q7_to_float.c \
  Drivers/CMSIS/DSP_Lib/Source/SupportFunctions/arm_q31_to_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/SupportFunctions/arm_q15_to_float.c \
  Drivers/CMSIS/DSP_Lib/Source/SupportFunctions/arm_copy_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/SupportFunctions/arm_float_to_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/SupportFunctions/arm_copy_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/SupportFunctions/arm_q31_to_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/SupportFunctions/arm_float_to_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/SupportFunctions/arm_float_to_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_biquad_cascade_df1_init_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_conv_fast_opt_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_init_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_biquad_cascade_stereo_df2T_init_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_interpolate_init_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_biquad_cascade_df1_init_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_correlate_opt_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_interpolate_init_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_correlate_fast_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_conv_partial_fast_opt_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_init_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_conv_opt_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_biquad_cascade_df1_init_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_sparse_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_interpolate_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_conv_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_decimate_init_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_conv_partial_fast_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_biquad_cascade_df2T_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_iir_lattice_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_conv_fast_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_lattice_init_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_conv_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_biquad_cascade_df1_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_lms_init_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_init_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_biquad_cascade_df1_fast_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_conv_partial_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_lattice_init_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_correlate_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_sparse_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_lms_norm_init_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_interpolate_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_lattice_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_lms_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_conv_partial_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_lms_norm_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_conv_partial_opt_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_biquad_cascade_df1_fast_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_fast_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_conv_partial_fast_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_correlate_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_interpolate_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_lms_init_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_biquad_cascade_df1_32x64_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_sparse_init_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_sparse_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_biquad_cascade_df2T_init_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_iir_lattice_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_decimate_fast_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_lms_norm_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_sparse_init_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_decimate_init_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_conv_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_lattice_init_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_biquad_cascade_df1_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_iir_lattice_init_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_conv_opt_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_decimate_fast_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_lattice_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_fast_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_lms_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_correlate_opt_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_iir_lattice_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_sparse_init_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_decimate_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_lms_norm_init_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_iir_lattice_init_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_biquad_cascade_df2T_f64.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_lms_norm_init_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_correlate_fast_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_init_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_correlate_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_iir_lattice_init_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_biquad_cascade_df1_32x64_init_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_conv_partial_opt_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_lms_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_decimate_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_sparse_init_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_correlate_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_biquad_cascade_stereo_df2T_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_biquad_cascade_df2T_init_f64.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_conv_partial_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_lattice_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_lms_norm_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_conv_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_sparse_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_conv_partial_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_correlate_fast_opt_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_biquad_cascade_df1_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_lms_init_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_interpolate_init_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_decimate_init_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_fir_decimate_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/arm_conv_fast_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/MatrixFunctions/arm_mat_scale_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/MatrixFunctions/arm_mat_sub_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/MatrixFunctions/arm_mat_add_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/MatrixFunctions/arm_mat_add_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/MatrixFunctions/arm_mat_init_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/MatrixFunctions/arm_mat_trans_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/MatrixFunctions/arm_mat_mult_fast_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/MatrixFunctions/arm_mat_sub_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/MatrixFunctions/arm_mat_mult_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/MatrixFunctions/arm_mat_inverse_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/MatrixFunctions/arm_mat_scale_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/MatrixFunctions/arm_mat_cmplx_mult_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/MatrixFunctions/arm_mat_trans_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/MatrixFunctions/arm_mat_init_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/MatrixFunctions/arm_mat_cmplx_mult_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/MatrixFunctions/arm_mat_mult_fast_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/MatrixFunctions/arm_mat_inverse_f64.c \
  Drivers/CMSIS/DSP_Lib/Source/MatrixFunctions/arm_mat_scale_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/MatrixFunctions/arm_mat_cmplx_mult_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/MatrixFunctions/arm_mat_mult_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/MatrixFunctions/arm_mat_add_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/MatrixFunctions/arm_mat_mult_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/MatrixFunctions/arm_mat_init_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/MatrixFunctions/arm_mat_trans_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/MatrixFunctions/arm_mat_sub_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FastMathFunctions/arm_sin_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FastMathFunctions/arm_sin_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FastMathFunctions/arm_cos_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FastMathFunctions/arm_cos_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FastMathFunctions/arm_sin_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/FastMathFunctions/arm_sqrt_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/FastMathFunctions/arm_cos_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/FastMathFunctions/arm_sqrt_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/ComplexMathFunctions/arm_cmplx_conj_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/ComplexMathFunctions/arm_cmplx_mult_cmplx_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/ComplexMathFunctions/arm_cmplx_mult_real_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/ComplexMathFunctions/arm_cmplx_dot_prod_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/ComplexMathFunctions/arm_cmplx_mag_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/ComplexMathFunctions/arm_cmplx_mult_real_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/ComplexMathFunctions/arm_cmplx_mag_squared_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/ComplexMathFunctions/arm_cmplx_mag_squared_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/ComplexMathFunctions/arm_cmplx_mag_squared_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/ComplexMathFunctions/arm_cmplx_mult_real_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/ComplexMathFunctions/arm_cmplx_conj_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/ComplexMathFunctions/arm_cmplx_mag_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/ComplexMathFunctions/arm_cmplx_dot_prod_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/ComplexMathFunctions/arm_cmplx_mag_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/ComplexMathFunctions/arm_cmplx_mult_cmplx_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/ComplexMathFunctions/arm_cmplx_dot_prod_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/ComplexMathFunctions/arm_cmplx_conj_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/ComplexMathFunctions/arm_cmplx_mult_cmplx_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_shift_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_shift_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_shift_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_scale_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_abs_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_negate_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_add_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_sub_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_mult_q7.c \
  Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/arm_offset_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix8_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_bitreversal.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_init_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_fast_init_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_init_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_init_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_fast_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix4_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_cfft_radix2_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_rfft_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/arm_dct4_init_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/CommonTables/arm_common_tables.c \
  Drivers/CMSIS/DSP_Lib/Source/CommonTables/arm_const_structs.c \
  Drivers/CMSIS/DSP_Lib/Source/ControllerFunctions/arm_sin_cos_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_reset_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_reset_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_init_q15.c \
  Drivers/CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_init_q31.c \
  Drivers/CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_init_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/ControllerFunctions/arm_sin_cos_f32.c \
  Drivers/CMSIS/DSP_Lib/Source/ControllerFunctions/arm_pid_reset_q15.c \
  Drivers/CMSIS/Device/ST/STM32F0xx/Source/Templates/system_stm32f0xx.c \
  Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc.c \
  Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pcd.c \
  Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim_ex.c \
  Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr.c \
  Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_uart.c \
  Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash_ex.c \
  Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc_ex.c \
  Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_spi_ex.c \
  Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_cortex.c \
  Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash.c \
  Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c.c \
  Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c_ex.c \
  Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_dma.c \
  Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_spi.c \
  Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pcd_ex.c \
  Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_crc_ex.c \
  Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_gpio.c \
  Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_uart_ex.c \
  Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal.c \
  Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim.c \
  Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr_ex.c \
  Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_crc.c  
ASM_SOURCES = Drivers/CMSIS/Device/ST/STM32F0xx/Source/Templates/gcc/startup_stm32f072xb.s
#  MDK-ARM/startup_stm32f072xb.s \

#######################################
# binaries
#######################################
CC = arm-none-eabi-gcc
AS = arm-none-eabi-gcc -x assembler-with-cpp
CP = arm-none-eabi-objcopy
AR = arm-none-eabi-ar
SZ = arm-none-eabi-size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# macros for gcc
AS_DEFS =
C_DEFS = -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F072xB -std=c99
# includes for gcc
AS_INCLUDES =
#C_INCLUDES = -IMDK-ARM/RTE
C_INCLUDES += -IInc
#C_INCLUDES += -IMiddlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS
#C_INCLUDES += -IMiddlewares/Third_Party/FreeRTOS/Source/include
#C_INCLUDES += -IMiddlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM0
C_INCLUDES += -IMiddlewares/Third_Party/FatFs/src
C_INCLUDES += -IMiddlewares/ST/STM32_USB_Device_Library/Class/HID/Inc
C_INCLUDES += -IMiddlewares/ST/STM32_USB_Device_Library/Core/Inc
#C_INCLUDES += -IDrivers/CMSIS/RTOS/Template
C_INCLUDES += -IDrivers/CMSIS/Device/ST/STM32F0xx/Include
C_INCLUDES += -IDrivers/CMSIS/Include
C_INCLUDES += -IDrivers/STM32F0xx_HAL_Driver/Inc
C_INCLUDES += -IDrivers/STM32F0xx_HAL_Driver/Inc/Legacy
# compile gcc flags
ASFLAGS = -mthumb -mcpu=cortex-m0 $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
CFLAGS = -mthumb -mcpu=cortex-m0 $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif
# Generate dependency information
CFLAGS += -MD -MP -MF .dep/$(@F).d

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F072CBTx_FLASH.ld
# libraries
LIBS = -lc -lm -lnosys
LIBDIR =
LDFLAGS = -mthumb -mcpu=cortex-m0 -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir -p $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR .dep $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

.PHONY: clean all

# *** EOF ***

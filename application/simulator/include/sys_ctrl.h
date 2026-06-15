#ifndef __SYS_CTRL_H__
#define __SYS_CTRL_H__

#include <stdint.h>
#include "app_data.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SYS_POWER_ON_RESET     0x00
#define SYS_NON_POWER_ON_RESET 0x01

void sys_ctrl_reset(void);
void sys_ctrl_independent_watchdog_init(void);
void sys_ctrl_independent_watchdog_reset(void);
void sys_ctrl_soft_watchdog_init(uint32_t timeout);
void sys_ctrl_soft_watchdog_reset(void);
void sys_ctrl_soft_watchdog_enable(void);
void sys_ctrl_soft_watchdog_disable(void);
void sys_ctrl_soft_watchdog_increase_counter(void);
void sys_ctrl_delay(volatile uint32_t count);
void sys_ctrl_delay_ms(volatile uint32_t count);
void sys_ctrl_delay_us(volatile uint32_t count);
uint32_t sys_ctrl_micros(void);
uint32_t sys_ctrl_millis(void);
uint8_t sys_ctrl_shell_get_char(void);
void sys_ctrl_shell_put_char(uint8_t c);
void sys_ctrl_shell_put_char_block(uint8_t c);
void sys_ctrl_shell_sw_to_block(void);
void sys_ctrl_shell_sw_to_nonblock(void);
void sys_ctrl_get_firmware_info(firmware_header_t* header);
void sys_ctr_sleep_wait_for_irq(void);
uint32_t sys_ctr_get_exception_number(void);
void sys_ctr_restart_app(void);
uint32_t sys_ctr_get_vbat_voltage(void);
uint32_t sys_ctr_get_mcu_temperature(void);
void sys_ctr_stop_mcu(void);
int asm_test_add(int a, int b);
uint32_t sys_stack_count_init(void);
uint32_t sys_stack_usage(void);
uint32_t sys_stack_get_size(void);

#ifdef __cplusplus
}
#endif

#endif

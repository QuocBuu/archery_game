#include "sim_internal.h"

#include "app_data.h"
#include "button.h"
#include "app_bsp.h"
#include "buzzer.h"
#include "io_cfg.h"
#include "sys_boot.h"
#include "sys_ctrl.h"
#include "sys_dbg.h"
#include "timer.h"

#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" void sys_irq_timer_10ms(void);

static uint8_t eeprom_data[EEPROM_MAX_SIZE];
static uint8_t flash_data[64 * 1024];
static sys_boot_t boot_data;
static bool flash_selected;

extern "C" {

uint32_t _start_boot_share_data_flash = 0;
uint32_t __heap_end__ = 0xFFFFFFFFu;

void sim_platform_init(void) {
	memset(eeprom_data, 0xFF, sizeof(eeprom_data));
	memset(flash_data, 0xFF, sizeof(flash_data));
	memset(&boot_data, 0, sizeof(boot_data));
}

void sim_platform_step(uint32_t elapsed_ms) {
	static uint32_t button_tick_accumulator = 0;

	button_tick_accumulator += elapsed_ms;
	while (button_tick_accumulator >= 10) {
		sys_irq_timer_10ms();
		button_tick_accumulator -= 10;
	}

	timer_tick(elapsed_ms);
	task_run_step();
}

void io_button_mode_init(void) {}
void io_button_up_init(void) {}
void io_button_down_init(void) {}

uint8_t io_button_mode_read(void) { return (uint8_t)sim_input_read_button(BUTTON_MODE_ID); }
uint8_t io_button_up_read(void) { return (uint8_t)sim_input_read_button(BUTTON_UP_ID); }
uint8_t io_button_down_read(void) { return (uint8_t)sim_input_read_button(BUTTON_DOWN_ID); }

void led_life_init(void) {}
void led_life_on(void) {}
void led_life_off(void) {}

void flash_io_ctrl_init(void) { flash_selected = false; }
void flash_cs_low(void) { flash_selected = true; }
void flash_cs_high(void) { flash_selected = false; }
uint8_t flash_transfer(uint8_t byte) {
	(void)flash_selected;
	(void)byte;
	return 0xFF;
}

void io_cfg_adc1(void) {}
void adc_bat_io_cfg(void) {}
uint16_t adc_bat_io_read(uint8_t channel) {
	(void)channel;
	return 0;
}

void oled_clk_input_mode(void) {}
void oled_clk_output_mode(void) {}
void oled_clk_digital_write_low(void) { sim_gpio_set_pin(SIM_PIN_OLED_CLK, 0); }
void oled_clk_digital_write_high(void) { sim_gpio_set_pin(SIM_PIN_OLED_CLK, 1); }
int oled_clk_digital_read(void) { return sim_gpio_get_pin(SIM_PIN_OLED_CLK); }

void oled_data_input_mode(void) {}
void oled_data_output_mode(void) {}
void oled_data_digital_write_low(void) { sim_gpio_set_pin(SIM_PIN_OLED_DATA, 0); }
void oled_data_digital_write_high(void) { sim_gpio_set_pin(SIM_PIN_OLED_DATA, 1); }
int oled_data_digital_read(void) { return sim_gpio_get_pin(SIM_PIN_OLED_DATA); }

void oled_res_input_mode(void) {}
void oled_res_output_mode(void) {}
void oled_res_digital_write_low(void) { sim_gpio_set_pin(SIM_PIN_OLED_RES, 0); }
void oled_res_digital_write_high(void) { sim_gpio_set_pin(SIM_PIN_OLED_RES, 1); }
int oled_res_digital_read(void) { return sim_gpio_get_pin(SIM_PIN_OLED_RES); }

uint8_t io_eeprom_read(uint32_t address, uint8_t* data, uint32_t len) {
	if (address + len > sizeof(eeprom_data)) {
		return 1;
	}
	memcpy(data, &eeprom_data[address], len);
	return 0;
}

uint8_t io_eeprom_write(uint32_t address, uint8_t* data, uint32_t len) {
	if (address + len > sizeof(eeprom_data)) {
		return 1;
	}
	memcpy(&eeprom_data[address], data, len);
	return 0;
}

uint8_t io_eeprom_erase(uint32_t address, uint32_t len) {
	if (address + len > sizeof(eeprom_data)) {
		return 1;
	}
	memset(&eeprom_data[address], 0xFF, len);
	return 0;
}

void internal_flash_unlock(void) {}
void internal_flash_lock(void) {}
void internal_flash_erase_pages_cal(uint32_t address, uint32_t len) {
	if (address < sizeof(flash_data)) {
		uint32_t available = sizeof(flash_data) - address;
		memset(&flash_data[address], 0xFF, len < available ? len : available);
	}
}
uint8_t internal_flash_write_cal(uint32_t address, uint8_t* data, uint32_t len) {
	if (address + len > sizeof(flash_data)) {
		return 1;
	}
	memcpy(&flash_data[address], data, len);
	return 0;
}

uint8_t flash_is_connected(void) { return 0; }
uint8_t flash_read(uint32_t address, uint8_t* pbuf, uint32_t len) {
	if (address + len > sizeof(flash_data)) {
		memset(pbuf, 0, len);
		return 1;
	}
	memcpy(pbuf, &flash_data[address], len);
	return 0;
}
uint8_t flash_write(uint32_t address, uint8_t* pbuf, uint32_t len) {
	if (address + len > sizeof(flash_data)) {
		return 1;
	}
	memcpy(&flash_data[address], pbuf, len);
	return 0;
}
uint8_t flash_erase_sector(uint32_t address) {
	if (address >= sizeof(flash_data)) {
		return 1;
	}
	uint32_t len = 0x1000u;
	if (address + len > sizeof(flash_data)) {
		len = sizeof(flash_data) - address;
	}
	memset(&flash_data[address], 0xFF, len);
	return 0;
}
uint8_t flash_erase_block_32k(uint32_t address) { return flash_erase_sector(address); }
uint8_t flash_erase_block_64k(uint32_t address) { return flash_erase_sector(address); }
uint8_t flash_erase_full(void) {
	memset(flash_data, 0xFF, sizeof(flash_data));
	return 0;
}
uint8_t flash_erase_synchronous(void) { return 0; }

void BUZZER_Init(void) {}
void BUZZER_Enable(uint16_t freq, uint32_t duration) { (void)freq; (void)duration; }
void BUZZER_Disable(void) {}
void BUZZER_PlaySound(buzzer_sound_t sound) { (void)sound; }
void BUZZER_Sleep(bool sleep) { (void)sleep; }

void sys_boot_init(void) {}
void sys_boot_get(sys_boot_t* obj) { *obj = boot_data; }
uint8_t sys_boot_set(sys_boot_t* obj) {
	boot_data = *obj;
	return SYS_BOOT_OK;
}

void sys_ctrl_reset(void) { sim_input_request_quit(); }
void sys_ctrl_independent_watchdog_init(void) {}
void sys_ctrl_independent_watchdog_reset(void) {}
void sys_ctrl_soft_watchdog_init(uint32_t timeout) { (void)timeout; }
void sys_ctrl_soft_watchdog_reset(void) {}
void sys_ctrl_soft_watchdog_enable(void) {}
void sys_ctrl_soft_watchdog_disable(void) {}
void sys_ctrl_soft_watchdog_increase_counter(void) {}
void sys_ctrl_delay(volatile uint32_t count) { while (count--) {} }
void sys_ctrl_delay_ms(volatile uint32_t count) { SDL_Delay(count); }
void sys_ctrl_delay_us(volatile uint32_t count) { SDL_Delay((count + 999u) / 1000u); }
uint32_t sys_ctrl_micros(void) { return SDL_GetTicks() * 1000u; }
uint32_t sys_ctrl_millis(void) { return SDL_GetTicks(); }
uint8_t sys_ctrl_shell_get_char(void) { return 0; }
void sys_ctrl_shell_put_char(uint8_t c) { fputc(c, stdout); }
void sys_ctrl_shell_put_char_block(uint8_t c) { fputc(c, stdout); }
void sys_ctrl_shell_sw_to_block(void) {}
void sys_ctrl_shell_sw_to_nonblock(void) {}
void sys_ctrl_get_firmware_info(firmware_header_t* header) { memset(header, 0, sizeof(*header)); }
void sys_ctr_sleep_wait_for_irq(void) {}
uint32_t sys_ctr_get_exception_number(void) { return 0; }
void sys_ctr_restart_app(void) { sim_input_request_quit(); }
uint32_t sys_ctr_get_vbat_voltage(void) { return 3300; }
uint32_t sys_ctr_get_mcu_temperature(void) { return 25; }
void sys_ctr_stop_mcu(void) { sim_input_request_quit(); }
int asm_test_add(int a, int b) { return a + b; }
uint32_t sys_stack_count_init(void) { return 0; }
uint32_t sys_stack_usage(void) { return 0; }
uint32_t sys_stack_get_size(void) { return 0; }

void sys_dbg_fatal(const int8_t* s, uint8_t c) {
	fprintf(stderr, "FATAL %s 0x%02X\n", s ? (const char*)s : "SIM", c);
	sim_input_request_quit();
}
void sys_dbg_func_stack_dump(uint32_t* stack) { (void)stack; }
void sys_dbg_cpu_dump(void) {}
void sys_dbg_stack_space_dump(void) {}

}

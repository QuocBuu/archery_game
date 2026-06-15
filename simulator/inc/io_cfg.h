#ifndef __IO_CFG_H__
#define __IO_CFG_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OLED_CLK_PIN  SIM_PIN_OLED_CLK
#define OLED_DATA_PIN SIM_PIN_OLED_DATA
#define OLED_RES_PIN  SIM_PIN_OLED_RES

#include "sim_internal.h"

void io_button_mode_init(void);
void io_button_up_init(void);
void io_button_down_init(void);

uint8_t io_button_mode_read(void);
uint8_t io_button_up_read(void);
uint8_t io_button_down_read(void);

void led_life_init(void);
void led_life_on(void);
void led_life_off(void);

void flash_io_ctrl_init(void);
void flash_cs_low(void);
void flash_cs_high(void);
uint8_t flash_transfer(uint8_t byte);

void io_cfg_adc1(void);
void adc_bat_io_cfg(void);
uint16_t adc_bat_io_read(uint8_t channel);

void oled_clk_input_mode(void);
void oled_clk_output_mode(void);
void oled_clk_digital_write_low(void);
void oled_clk_digital_write_high(void);
int oled_clk_digital_read(void);

void oled_data_input_mode(void);
void oled_data_output_mode(void);
void oled_data_digital_write_low(void);
void oled_data_digital_write_high(void);
int oled_data_digital_read(void);

void oled_res_input_mode(void);
void oled_res_output_mode(void);
void oled_res_digital_write_low(void);
void oled_res_digital_write_high(void);
int oled_res_digital_read(void);

#define EEPROM_BASE_ADDRESS 0x08080000u
#define EEPROM_MAX_SIZE     0x1000u

uint8_t io_eeprom_read(uint32_t address, uint8_t* data, uint32_t len);
uint8_t io_eeprom_write(uint32_t address, uint8_t* data, uint32_t len);
uint8_t io_eeprom_erase(uint32_t address, uint32_t len);

void internal_flash_unlock(void);
void internal_flash_lock(void);
void internal_flash_erase_pages_cal(uint32_t address, uint32_t len);
uint8_t internal_flash_write_cal(uint32_t address, uint8_t* data, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif

#ifndef SIM_INTERNAL_H
#define SIM_INTERNAL_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SIM_OLED_WIDTH 128
#define SIM_OLED_HEIGHT 64
#define SIM_OLED_SCALE 4

#define SIM_PIN_OLED_CLK  0x03
#define SIM_PIN_OLED_DATA 0x04
#define SIM_PIN_OLED_RES  0x08

void sim_oled_init(void);
void sim_oled_write_i2c_byte(uint8_t byte);
void sim_oled_i2c_start(void);
void sim_oled_i2c_stop(void);
void sim_oled_render(void);
void sim_oled_cleanup(void);

void sim_input_init(void);
void sim_input_poll(void);
bool sim_input_get_running(void);
void sim_input_request_quit(void);
int sim_input_read_button(uint8_t button_id);

void sim_gpio_set_pin(uint8_t pin, uint8_t value);
uint8_t sim_gpio_get_pin(uint8_t pin);

void sim_platform_init(void);
void sim_platform_step(uint32_t elapsed_ms);

#ifdef __cplusplus
}
#endif

#endif

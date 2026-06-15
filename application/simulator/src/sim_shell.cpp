#include "task_shell.h"

struct ak_msg_t;

volatile struct shell_t shell;
uint8_t buffer_console_rev[BUFFER_CONSOLE_REV_SIZE];
ring_buffer_char_t ring_buffer_console_rev;

void sys_irq_shell(void) {}

void task_shell(ak_msg_t* msg) {
	(void)msg;
}

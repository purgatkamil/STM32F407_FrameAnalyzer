#ifndef INC_HCI_H_
#define INC_HCI_H_

#include <lcd_menu.h>
#include "lcd.h"
#include "lcd_buffer.h"

#include "tim.h"
#include "stdio.h"
#include "stdbool.h"

extern volatile int scroll_result;

typedef enum
{
	DISPLAY_MODE_DATA_ONLY,
	DISPLAY_MODE_WHOLE_FRAME,
}display_mode;

typedef enum
{
	HCI_OFF_MODE,
	HCI_I2C_MODE,
	HCI_SPI_MODE,
	HCI_UART_MODE,
}hci_analyse_mode;

int hci_scroll();

void hci_display_menu();
void hci_encoder_init();
void hci_hide_menu();
void hci_menu();
void hci_i2c_data_only_mode();
void hci_i2c_whole_frame_mode();

int hci_return_delay_timer_state();
void hci_start_delay_timer();
void hci_reset_delay_timer();

#endif /* INC_HCI_H_ */

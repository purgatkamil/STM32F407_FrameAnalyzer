#ifndef INC_HCI_H_
#define INC_HCI_H_

#include <lcd_menu.h>
#include "lcd.h"

#include "tim.h"
#include "stdio.h"
#include "stdbool.h"

typedef enum{
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

typedef enum
{
	HCI_GOTO_MENU,
	HCI_GOTO_I2C,
	HCI_GOTO_SPI,
	HCI_GOTO_UART,
	HCI_EXIT,
}hci_menu_where_go_next;

void hci_display_menu();
void hci_scroll();
void hci_encoder_init();
void hci_hide_menu();
void hci_menu();
void hci_i2c_data_only_mode();
void hci_i2c_whole_frame_mode();

#endif /* INC_HCI_H_ */

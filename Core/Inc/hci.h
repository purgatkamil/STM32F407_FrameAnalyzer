/*
 * hci.h
 *
 *  Created on: Aug 26, 2024
 *      Author: KP
 */

#ifndef INC_HCI_H_
#define INC_HCI_H_

#include "lcd.h"

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

#endif /* INC_HCI_H_ */

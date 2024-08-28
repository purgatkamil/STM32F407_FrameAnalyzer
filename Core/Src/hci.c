#include "hci.h"
#include "tim.h"
#include "stdio.h"

#define MENU_START_X 20
#define MENU_START_Y 5

#define CURSOR_START_X 5
#define CURSOR_START_Y 5

int8_t encoder_flag = 0;
uint32_t old_encoder_value = 0;

extern bool menu_flag;

void hci_encoder_init()
{
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	old_encoder_value = 0;
}


volatile hci_analyse_mode analyse_mode = HCI_OFF_MODE;
volatile hci_menu_where_go_next menu_where_to_go = HCI_GOTO_MENU;

void hci_display_menu()
{
	char menu1[] = "I2C analyze";
	char menu2[] = "SPI analyze";
	char menu3[] = "UART analyze";
	char cursor[] = ">";

	analyse_mode = HCI_I2C_MODE;


	if(menu_where_to_go == HCI_GOTO_MENU)
	{
		LCD_DisplayString(CURSOR_START_X, CURSOR_START_Y, cursor, WHITE, LCD_FONT16);
		menu_where_to_go = HCI_GOTO_I2C;
		LCD_DisplayString(MENU_START_X, MENU_START_Y, menu1, WHITE, LCD_FONT16);
		LCD_DisplayString(MENU_START_X, MENU_START_Y + 18, menu2, WHITE, LCD_FONT16);
		LCD_DisplayString(MENU_START_X, MENU_START_Y + 36, menu3, WHITE, LCD_FONT16);
	}


	lcd_copy();
}

void hci_hide_menu()
{
	fill_with(BLACK);
	lcd_copy();
}

void hci_scroll()
{
	  int32_t value = __HAL_TIM_GET_COUNTER(&htim2);
	  char cursor = '>';

	  if (old_encoder_value != value)
	  {
		HAL_TIM_Base_Stop_IT(&htim14);
		__HAL_TIM_SET_COUNTER(&htim14, 0);

		int switch_value = (value / 2) % 3;

		LCD_DisplayChar(CURSOR_START_X, CURSOR_START_Y, cursor, BLACK, LCD_FONT16);
		LCD_DisplayChar(CURSOR_START_X, CURSOR_START_Y + 18, cursor, BLACK, LCD_FONT16);
		LCD_DisplayChar(CURSOR_START_X, CURSOR_START_Y + 36, cursor, BLACK, LCD_FONT16);
		LCD_DisplayChar(CURSOR_START_X, CURSOR_START_Y + 54, cursor, BLACK, LCD_FONT16);

		switch(switch_value)
		{
			case 0:
				LCD_DisplayChar(CURSOR_START_X, CURSOR_START_Y, cursor, WHITE, LCD_FONT16);
				menu_where_to_go = HCI_GOTO_I2C;
				analyse_mode = HCI_I2C_MODE;
				break;

			case 1:
				LCD_DisplayChar(CURSOR_START_X, CURSOR_START_Y + 18, cursor, WHITE, LCD_FONT16);
				menu_where_to_go = HCI_GOTO_SPI;
				analyse_mode = HCI_SPI_MODE;
				break;

			case 2:
				LCD_DisplayChar(CURSOR_START_X, CURSOR_START_Y + 36, cursor, WHITE, LCD_FONT16);
				menu_where_to_go = HCI_GOTO_UART;
				analyse_mode = HCI_UART_MODE;
				break;

			default:
				break;
		}

		old_encoder_value = value;

		HAL_TIM_Base_Start_IT(&htim14);
	  }

	  if(encoder_flag == 1)
	  {
		  HAL_TIM_Base_Stop_IT(&htim14);
		  lcd_copy();
		  encoder_flag = 0;
	  }
}

extern bool menu_flag;

void hci_menu()
{
	switch(menu_where_to_go)
	{
		case HCI_GOTO_MENU:
			menu_flag = true;
			hci_display_menu();
			break;

		case HCI_EXIT:
			menu_flag = false;
			menu_where_to_go = HCI_GOTO_MENU;
			hci_hide_menu();
			break;

		case HCI_GOTO_I2C:
			menu_where_to_go = HCI_EXIT;
			break;

		case HCI_GOTO_SPI:
			menu_where_to_go = HCI_EXIT;
			break;

		case HCI_GOTO_UART:
			menu_where_to_go = HCI_EXIT;
			break;


		default:
			break;

	}
}

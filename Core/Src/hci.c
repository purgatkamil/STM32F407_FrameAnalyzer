#include "hci.h"

#define MENU_START_X 20
#define MENU_START_Y 5

#define CURSOR_START_X 5
#define CURSOR_START_Y 5

int8_t encoder_flag = 0;
uint32_t old_encoder_value = 0;

void hci_encoder_init()
{
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	old_encoder_value = 0;
}

volatile int menu_opened = 0;

void hci_display_menu()
{
	menu_reset();
	menu_opened = 1;
	menu_refresh();
}

void hci_hide_menu()
{
	menu_opened = 0;
	fill_with(BLACK);
	lcd_copy();
}

volatile bool hci_i2c_flag = false;
display_mode mode = DISPLAY_MODE_WHOLE_FRAME;

void hci_i2c_whole_frame_mode()
{
	hci_hide_menu();
	hci_i2c_flag = true;
	mode = DISPLAY_MODE_WHOLE_FRAME;
}

void hci_i2c_data_only_mode()
{
	hci_hide_menu();
	hci_i2c_flag = true;
	mode = DISPLAY_MODE_DATA_ONLY;
}

void hci_scroll()
{
	  int32_t value = ( __HAL_TIM_GET_COUNTER(&htim2) / 2);

	  if (old_encoder_value != value)
	  {
		HAL_TIM_Base_Stop_IT(&htim14);
		__HAL_TIM_SET_COUNTER(&htim14, 0);

		if(old_encoder_value < value)
		{
			menu_next();
		}
		else if((old_encoder_value > value) && (value == 0))
		{
			menu_next();
		}
		else
		{
			menu_prev();
		}

		old_encoder_value = value;

		HAL_TIM_Base_Start_IT(&htim14);

	  }

	  if(encoder_flag == 1)
	  {
		  HAL_TIM_Base_Stop_IT(&htim14);
		  menu_refresh();
		  encoder_flag = 0;
	  }
}


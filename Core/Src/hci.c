#include "hci.h"

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
	fill_with(BLACK);
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

volatile int scroll_result = 0;

void hci_reset_delay_timer()
{
	HAL_TIM_Base_Stop_IT(&htim14);
	__HAL_TIM_SET_COUNTER(&htim14, 0);
}

void hci_start_delay_timer()
{
	HAL_TIM_Base_Start_IT(&htim14);
}

int hci_return_delay_timer_state()
{
	int result = 0;
	if(encoder_flag == 1)
	{
		HAL_TIM_Base_Stop_IT(&htim14);
		encoder_flag = 0;
		result = 1;
	}

	return result;
}

int hci_scroll()
{
	  int32_t value = ( __HAL_TIM_GET_COUNTER(&htim2) / 2);

	  if (old_encoder_value != value)
	  {
		hci_reset_delay_timer();

		if(old_encoder_value < value)
		{
			scroll_result = 1;
		}
		else if((old_encoder_value > value) && (value == 0))
		{
			scroll_result = 1;
		}
		else
		{
			scroll_result = -1;
		}

		old_encoder_value = value;

		hci_start_delay_timer();

	  }
	  else
	  {
		  scroll_result = 0;
	  }

	/*  if(encoder_flag == 1)
	  {
		  HAL_TIM_Base_Stop_IT(&htim14);
		  menu_refresh();
		  encoder_flag = 0;
	  }*/
	  return scroll_result;
}


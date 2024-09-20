#include "whole_frame_display.h"

extern i2c_bit_buffer_s i2c_bits;
extern i2c_byte_buffer_s i2c_bytes;

extern lcd_buffer_s lcd_buffer;

int last_displayed_bit_cnt = BITS_ON_SCREEN - ALIGN_TO_TABLE;
int displayed_bit_counter = 0;

void start_display_whole_frame()
{
	fill_with(BLACK);

	const int one_bit_lcd_size = LCD_WIDTH / BITS_ON_SCREEN;

	int last_state = 0;

	for(int i = 0; i < BITS_ON_SCREEN; i++)
	{

		int state = lcd_buffer_read_from_tail(&lcd_buffer, i);

		if(state == 1)
		{
			lcd_draw_horizontal_line(10, (i * one_bit_lcd_size) + 1, ((i+1) * one_bit_lcd_size), WHITE);
		}
		else if(state == 0)
		{
			lcd_draw_horizontal_line(30, (i * one_bit_lcd_size) + 1, ((i+1) * one_bit_lcd_size), WHITE);
		}

		if(last_state != state)
		{
			lcd_draw_vertical_line(i * one_bit_lcd_size, 10, 31, WHITE);
		}
		if( ( ( ( i + displayed_bit_counter) * 5) % 40) == 0)
		{
			int count_x = i * 5;
			lcd_draw_vertical_line(count_x, 5, 38, RED);
		}

		last_state = state;
	}

	lcd_copy();
	HAL_Delay(10);
}

extern int whole_frame_flag;

void whole_frame_scrolling()
{
	fill_with(BLACK);

  	if(scroll_result == 1)
  	{
  		int size = i2c_get_bits_buffor_size();
  		if(last_displayed_bit_cnt < size)
  		{
  			last_displayed_bit_cnt++;
  			displayed_bit_counter++;
  			lcd_buffer_add_next(&lcd_buffer, i2c_bits.data[last_displayed_bit_cnt]);
  		}
  	}
  	else if(scroll_result == -1)
  	{
  		if((last_displayed_bit_cnt - BITS_ON_SCREEN) >= 0)
  		{
  	  		lcd_buffer_add_previous(&lcd_buffer, i2c_bits.data[last_displayed_bit_cnt - BITS_ON_SCREEN]);
  	  		last_displayed_bit_cnt--;
  	  		displayed_bit_counter--;
  		}
  	}

    if( (hci_return_delay_timer_state() == 1) && (whole_frame_flag == 1) )
    {
  	  start_display_whole_frame();
    }
}

#include "lcd_buffer.h"

void lcd_buffer_init(lcd_buffer_s *buffer)
{
	buffer->head = BITS_ON_SCREEN - 1;
  	for (int i = 0; i < BITS_ON_SCREEN; i++){
  		buffer->data[i] = 0;
  	}
}

void lcd_buffer_add_next(lcd_buffer_s *buffer, uint16_t value)
{
    buffer->data[buffer->head] = value;
    buffer->head = (buffer->head + 1) % BITS_ON_SCREEN;
}

void lcd_buffer_add_previous(lcd_buffer_s *buffer, uint16_t value)
{
	buffer->data[buffer->head] = value;
    buffer->head = (buffer->head - 1 + BITS_ON_SCREEN) % BITS_ON_SCREEN;
}

int lcd_buffer_read_from_head(lcd_buffer_s *buffer, int index)
{
	int cnt = ( (buffer->head) + index) % BITS_ON_SCREEN;
    int result = buffer->data[cnt];

    return result;
}

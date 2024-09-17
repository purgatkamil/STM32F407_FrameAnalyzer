/*
 * lcd_buffer.h
 *
 *  Created on: Sep 17, 2024
 *      Author: KP
 */

#ifndef INC_LCD_BUFFER_H_
#define INC_LCD_BUFFER_H_

#include <stdint.h>

#define BITS_ON_SCREEN 32
#define ALIGN_TO_TABLE 1

typedef struct
{
	int data[BITS_ON_SCREEN];
	int head;
}lcd_buffer_s;

void lcd_buffer_init(lcd_buffer_s *buffer);
void lcd_buffer_add_next(lcd_buffer_s *buffer, uint16_t value);
void lcd_buffer_add_previous(lcd_buffer_s *buffer, uint16_t value);
int lcd_buffer_read_from_head(lcd_buffer_s *buffer, int index);

#endif /* INC_LCD_BUFFER_H_ */

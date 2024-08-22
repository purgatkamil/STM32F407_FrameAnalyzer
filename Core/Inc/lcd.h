#ifndef INC_LCD_H_
#define INC_LCD_H_

#include <stdint.h>
#include <stdbool.h>

#define LCD_WIDTH	160
#define LCD_HEIGHT	128

#define BLACK			0x0000
#define RED				0x00f8
#define GREEN			0xe007
#define BLUE			0x1f00
#define YELLOW			0xe0ff
#define MAGENTA			0x1ff8
#define CYAN			0xff07
#define WHITE			0xffff

void lcd_init(void);
void lcd_put_pixel(int x, int y, uint16_t color);
void lcd_copy(void); // Przesłanie zawartości bufora

void lcd_transfer_done(void);
bool lcd_is_busy(void);

void LCD_DrawLine(int Xstart, int Ystart, int Xend, int Yend, uint16_t color);
void fill_with(uint16_t color);
void LCD_DisplayChar(uint16_t Xpoint, uint16_t Ypoint, char Acsii_Char, uint16_t Color);
void LCD_DisplayString(uint16_t Xstart, uint16_t Ystart, char* pString, uint16_t Color);
void lcd_draw_horizontal_line(int x, uint16_t color);
void lcd_draw_circle(int x, int y, int r);

void LCD_DrawCircle ( 	int X_Center, int Y_Center, int Radius, uint16_t color);

#endif /* INC_LCD_H_ */

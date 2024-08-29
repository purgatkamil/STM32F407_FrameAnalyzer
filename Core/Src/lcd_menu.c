#include <lcd_menu.h>

menu_t *currentPointer = &menu1;

uint8_t menu_index;
uint8_t lcd_row_pos;

// definition of menu's components: (*name, *next, *prev, *child, *parent, (*menu_function))
menu_t menu1 = { "I2C Analyse", &menu2, &menu3, &sub_menu1_1, NULL, NULL };
	menu_t sub_menu1_1 = { "WHOLE FRAME", &sub_menu1_2, &sub_menu1_2, NULL, &menu1, hci_i2c_whole_frame_mode};
	menu_t sub_menu1_2 = { "DATA ONLY", NULL, &sub_menu1_1, NULL, &menu1, hci_i2c_data_only_mode};
	//menu_t sub_menu1_3 = { "BACK", NULL, &sub_menu1_1, NULL, &menu1, menu_back};
menu_t menu2 = { "SPI Analyse", &menu3, &menu1, &sub_menu2_1, NULL, NULL };
	menu_t sub_menu2_1 = { "ELEMENT 2_1", &sub_menu2_2, &sub_menu2_4, NULL, &menu2, NULL };
	menu_t sub_menu2_2 = { "ELEMENT 2_2", &sub_menu2_3, &sub_menu2_1, &sub_menu2_2_1, &menu2, NULL };
		menu_t sub_menu2_2_1 = { "ELEMENT 2_2_1", &sub_menu2_2_2, &sub_menu2_2_5, NULL, &sub_menu2_2, NULL };
		menu_t sub_menu2_2_2 = { "ELEMENT 2_2_2", &sub_menu2_2_3, &sub_menu2_2_1, NULL, &sub_menu2_2, NULL };
		menu_t sub_menu2_2_3 = { "ELEMENT 2_2_3", &sub_menu2_2_4, &sub_menu2_2_2, NULL, &sub_menu2_2, NULL };
		menu_t sub_menu2_2_4 = { "ELEMENT 2_2_4", &sub_menu2_2_5, &sub_menu2_2_3, NULL, &sub_menu2_2, NULL };
		menu_t sub_menu2_2_5 = { "ELEMENT 2_2_5", NULL, &sub_menu2_2_4, NULL, &sub_menu2_2, NULL };
	menu_t sub_menu2_3 = { "ELEMENT 2_3", &sub_menu2_4, &sub_menu2_2, NULL, &menu2, NULL };
	menu_t sub_menu2_4 = { "ELEMENT 2_4", NULL, &sub_menu2_3, NULL, &menu2, NULL };
menu_t menu3 = { "UART Analyse", NULL, &menu2, NULL, NULL, NULL };


uint8_t lcd_row_pos_level_1, lcd_row_pos_level_2;

uint8_t menu_get_index(menu_t *q)
{

	menu_t *temp;
	uint8_t i = 0;

	if (q->parent)
	{
		temp = (q->parent)->child;
	}
	else
	{
		temp = &menu1;
	}

	while (temp != q)
	{
		temp = temp->next;
		i++;
	}

	return i;
}

uint8_t menu_get_level(menu_t *q)
{

	menu_t *temp = q;
	uint8_t i = 0;

	if (!q->parent)
	{
		return 0;
	}

	while (temp->parent != NULL)
	{
		temp = temp->parent;
		i++;
	}

	return i;
}

void menu_refresh(void)
{

	menu_t *temp;
	uint8_t i;

	if(currentPointer->parent)
	{
		temp = (currentPointer->parent)->child;
	}
	else
	{
		temp = &menu1;
	}

	for(i = 0; i != menu_index - lcd_row_pos; i++)
	{
		temp = temp->next;
	}

	fill_with(BLACK);

	for(i = 0; i < LCD_ROWS; i++)
	{
		if(temp == currentPointer)
		{
			LCD_DisplayChar(5, 5 + (20 * i), '>', WHITE, LCD_FONT16);
		}
		else
		{
			LCD_DisplayChar(5, 5 + (20 * i), '>', BLACK, LCD_FONT16);
		}

		LCD_DisplayString(20, 5 + (20 * i), temp->name, WHITE, LCD_FONT16);
		temp = temp->next;

		if(!temp)
		{
			break;
		}
	}

	lcd_copy();
	HAL_TIM_Base_Start_IT(&htim14);
}

void menu_next(void)
{
	if(currentPointer->next)
	{
		currentPointer = currentPointer->next;
		menu_index++;

		if(++lcd_row_pos > LCD_ROWS - 1)
		{
			lcd_row_pos = LCD_ROWS - 1;
		}
	}
	else
	{
		menu_index = 0;
		lcd_row_pos = 0;

		if(currentPointer->parent)
		{
			currentPointer = (currentPointer->parent)->child;
		}
		else
		{
			currentPointer = &menu1;
		}
	}

	//menu_refresh();
}

void menu_prev(void)
{

	currentPointer = currentPointer->prev;

	if (menu_index)
	{
		menu_index--;

		if(lcd_row_pos > 0)
		{
			lcd_row_pos--;
		}
	}
	else
	{
		menu_index = menu_get_index(currentPointer);

		if(menu_index >= LCD_ROWS - 1)
		{
			lcd_row_pos = LCD_ROWS - 1;
		}
		else
		{
			lcd_row_pos = menu_index;
		}
	}

	//menu_refresh();
}

void menu_enter(void)
{

	if (currentPointer->menu_function)
	{
		currentPointer->menu_function();
	}

	if (currentPointer->child)
	{

		switch (menu_get_level(currentPointer))
		{
			case 0:
				lcd_row_pos_level_1 = lcd_row_pos;
				break;

			case 1:
				lcd_row_pos_level_2 = lcd_row_pos;
				break;
		}

		menu_index = 0;
		lcd_row_pos = 0;

		currentPointer = currentPointer->child;

	}
}

void menu_back(void)
{

	if (currentPointer->parent)
	{
		switch (menu_get_level(currentPointer))
		{
			case 1:
				lcd_row_pos = lcd_row_pos_level_1;
				break;

			case 2:
				lcd_row_pos = lcd_row_pos_level_2;
				break;
		}

		currentPointer = currentPointer->parent;
		menu_index = menu_get_index(currentPointer);
	}
}


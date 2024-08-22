#include <i2c_analyze.h>

volatile static int i2c_bits[64];
volatile static int i2c_bits_counter = 0;

volatile int i2c_bytes[64];
volatile int i2c_bytes_counter = 0;
volatile int i2c_next_byte_counter = 0;

static i2c_conversion_state conversion = CONVERSION_NOT_READY;

volatile int i2c_last_scl_bit_value = 1;

static void i2c_convert_i2c_byte()
{
	volatile int i2c_ready_byte = 0;

	for(int i = 0; i < 8; i++)
	{
		i2c_ready_byte += i2c_bits[7 - i] << i;
	}

	i2c_bytes[i2c_bytes_counter] = i2c_ready_byte;
	i2c_bytes_counter++;
	conversion = CONVERSION_READY;
}

i2c_analyze_state i2c_read_data()
{
	i2c_last_scl_bit_value = 1;
	i2c_bits[i2c_bits_counter] = HAL_GPIO_ReadPin(ANALYZER_SDA_IT_RISING_GPIO_Port, ANALYZER_SDA_IT_RISING_Pin);


	if ( ( ( i2c_bits_counter + OFFSET ) % ACK_BIT ) != 0 )
	{
		i2c_bits_counter++;
		conversion = CONVERSION_NOT_READY;
		return I2C_BIT_SAVED;
	}
	else
	{
		i2c_convert_i2c_byte();
		i2c_bits_counter = 0;
		return I2C_BYTE_SAVED;
	}


}

i2c_conversion_state i2c_is_data_ready()
{
	if(i2c_bytes_counter > i2c_next_byte_counter)
	{
		return CONVERSION_READY;
	}
	else
	{
		return CONVERSION_NOT_READY;
	}
}

int i2c_get_ready_i2c_byte()
{
	int temp = i2c_next_byte_counter;
	i2c_next_byte_counter++;
	i2c_conversion_state state = conversion;
	if(state == CONVERSION_READY)
	{
		conversion = CONVERSION_NOT_READY;
	}

	return i2c_bytes[temp];
}

i2c_tx_state i2c_check_for_start()
{
	if(HAL_GPIO_ReadPin(ANALYZER_SCL_IT_RISING_GPIO_Port, ANALYZER_SCL_IT_RISING_Pin) == GPIO_PIN_SET)
	{
		if(i2c_last_scl_bit_value == 1)
		{
			return I2C_START;
		}
	}
	else
	{
		return I2C_NO_TX;
	}
}

i2c_tx_state i2c_check_for_stop()
{
	if(HAL_GPIO_ReadPin(ANALYZER_SCL_IT_RISING_GPIO_Port, ANALYZER_SCL_IT_RISING_Pin) == GPIO_PIN_SET)
	{
		if(i2c_last_scl_bit_value == 1)
		{
			return I2C_STOP;
		}
	}
	else
	{
		return I2C_DATA;
	}
}

void i2c_scl_falling()
{
	i2c_last_scl_bit_value = 1;
}

void i2c_reset_all()
{
	for(int i = 0; i < 64; i++)
	{
		i2c_bits[i] = 0;
		i2c_bytes[i] = 0;
	}

	i2c_bits_counter = 0;

	i2c_bytes_counter = 0;
	i2c_next_byte_counter = 0;

	conversion = CONVERSION_NOT_READY;
}





#include <i2c_analyze.h>

static i2c_bit_buffer_s i2c_bits;
static int i2c_bits_counter = 0;

i2c_byte_buffer_s i2c_bytes;
int i2c_bytes_counter = 0;
int i2c_next_byte_counter = 0;

volatile int i2c_bytes_count_after_stop = 0;

static i2c_conversion_state_e conversion = CONVERSION_NOT_READY;

volatile int i2c_last_scl_bit_value = 1;

static void i2c_bit_buffer_init(i2c_bit_buffer_s *buffer)
{
	buffer->head = 0;
	uint16_t i;
	for (i=0; i < BIT_BUFFER_SIZE; i++){
		buffer->data[i] = 0;
	}
}

static void i2c_bit_buffer_add(i2c_bit_buffer_s *buffer, uint16_t value)
{
    buffer->data[buffer->head] = value;
    buffer->head = (buffer->head + 1) % BIT_BUFFER_SIZE;
}

static void i2c_byte_buffer_init(i2c_byte_buffer_s *buffer)
{
	buffer->head = 0;
	uint16_t i;
	for (i=0; i < BYTE_BUFFER_SIZE; i++){
		buffer->data[i] = 0;
	}
}

static void i2c_byte_buffer_add(i2c_byte_buffer_s *buffer, uint16_t value)
{
    buffer->data[buffer->head] = value;
    buffer->head = (buffer->head + 1) % BYTE_BUFFER_SIZE;
}

void i2c_convert_i2c_bytes()
{
	volatile int i2c_ready_byte = 0;
	int counter = 0;

	for(int i = 0; i < i2c_bits.head; i++)
	{
		if ( ( ( i + ACK_OFFSET ) % ACK_BIT ) != 0 )
		{
			i2c_ready_byte += i2c_bits.data[i] << (7 - counter);
			counter++;
		}
		else
		{
			i2c_byte_buffer_add(&i2c_bytes, i2c_ready_byte);
			i2c_ready_byte = 0;
			counter = 0;
		}
	}
	conversion = CONVERSION_READY;
}

void i2c_read_data()
{
	i2c_last_scl_bit_value = 1;

	i2c_bit_buffer_add(&i2c_bits, HAL_GPIO_ReadPin(ANALYZER_SDA_IT_RISING_GPIO_Port, ANALYZER_SDA_IT_RISING_Pin));
}

i2c_conversion_state_e i2c_is_data_ready()
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
	i2c_conversion_state_e state = conversion;
	if(state == CONVERSION_READY)
	{
		conversion = CONVERSION_NOT_READY;
	}

	return i2c_bytes.data[temp];
}

i2c_tx_state_e i2c_check_for_start()
{
	i2c_tx_state_e state;

	if(HAL_GPIO_ReadPin(ANALYZER_SCL_IT_RISING_GPIO_Port, ANALYZER_SCL_IT_RISING_Pin) == GPIO_PIN_SET)
	{
		if(i2c_last_scl_bit_value == 1)
		{
			state = I2C_START;
		}
	}
	else
	{
		state = I2C_NO_TX;
	}

	return state;
}

i2c_tx_state_e i2c_check_for_stop()
{
	i2c_tx_state_e state;

	if(HAL_GPIO_ReadPin(ANALYZER_SCL_IT_RISING_GPIO_Port, ANALYZER_SCL_IT_RISING_Pin) == GPIO_PIN_SET)
	{
		if(i2c_last_scl_bit_value == 1)
		{
			i2c_bytes_count_after_stop = i2c_bytes_counter;
			state = I2C_STOP;
		}
	}
	else
	{
		state = I2C_DATA;
	}

	return state;
}

void i2c_scl_falling()
{
	i2c_last_scl_bit_value = 1;
}

void i2c_reset_all()
{
	i2c_bit_buffer_init(&i2c_bits);
	i2c_byte_buffer_init(&i2c_bytes);

	i2c_bits_counter = 0;

	i2c_bytes_counter = 0;
	i2c_next_byte_counter = 0;

	conversion = CONVERSION_NOT_READY;
}

int i2c_get_bytes_buffor_size()
{
	return i2c_bytes.head;
}







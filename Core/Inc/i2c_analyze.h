#ifndef INC_I2C_ANALYZE_H_
#define INC_I2C_ANALYZE_H_

#include "main.h"

#define ACK_BIT 9
#define ACK_OFFSET 1			/* to show that ACK bit is 9bit
								(counting from zero would result in the number 8 and could
								be confusing when analyzing the code) */

#define BIT_BUFFER_SIZE 1000	// (( 8 bits + ACK bit ) * 100 bytes available to store ) + 100 just in case
#define BYTE_BUFFER_SIZE 100

typedef struct
{
    volatile int32_t data[BIT_BUFFER_SIZE];
    volatile int head;
}i2c_bit_buffer_s;

typedef struct
{
    volatile int32_t data[BYTE_BUFFER_SIZE];
    volatile int head;
}i2c_byte_buffer_s;

typedef enum
{
	CONVERSION_NOT_READY,
	CONVERSION_READY,
}i2c_conversion_state_e;

typedef enum
{
	I2C_STOP,
	I2C_START,
	I2C_DATA,
	I2C_NO_TX,
}i2c_tx_state_e;

void i2c_scl_falling();
void i2c_reset_all();

i2c_conversion_state_e i2c_is_data_ready();
void i2c_read_data();
void i2c_convert_i2c_bytes();

i2c_tx_state_e i2c_check_for_start();
i2c_tx_state_e i2c_check_for_stop();


int i2c_get_ready_bits();
int i2c_get_bits_buffor_size();

int i2c_get_ready_i2c_byte();
int i2c_get_bytes_buffor_size();


#endif /* INC_I2C_ANALYZE_H_ */

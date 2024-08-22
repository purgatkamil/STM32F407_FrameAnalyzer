#ifndef INC_I2C_ANALYZE_H_
#define INC_I2C_ANALYZE_H_

#include "main.h"

#define ACK_BIT 9
#define OFFSET 1

uint8_t i2c_read_data();

typedef enum{
	I2C_BIT_SAVED,
	I2C_BYTE_SAVED,
}i2c_analyze_state;

typedef enum{
	CONVERSION_NOT_READY,
	CONVERSION_READY,
}i2c_conversion_state;

typedef enum{
	I2C_STOP,
	I2C_START,
	I2C_DATA,
	I2C_NO_TX,
}i2c_tx_state;

void i2c_scl_falling();
void i2c_reset_all();

i2c_conversion_state i2c_is_data_ready();
i2c_analyze_state i2c_read_data();

i2c_tx_state i2c_check_for_start();
i2c_tx_state i2c_check_for_stop();

int i2c_get_ready_i2c_byte();
int i2c_get_bytes_buffor_size();

#endif /* INC_I2C_ANALYZE_H_ */

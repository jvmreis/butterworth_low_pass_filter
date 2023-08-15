/* INCLUDES */
#include "adc.h"

/* DEFINES */
#define VOL_MIN	0x68
#define VOL_ZERO 0x00
#define VOL_MAX	0x28

#define FS_48K

/* VARIABLES */
uint8_t data[2]; // [Register, Value]

/* FUNCTIONS */

/* Initiate the ADC */
void adc_init(void){
//	HAL_Delay(4);
//	HAL_GPIO_WritePin(AUDIO_RESET_GPIO_Port, AUDIO_RESET_Pin, GPIO_PIN_SET);
	HAL_Delay(5);
}

void adc_mic_config3(void) {
	data[0] = 0x02; // SLEEP_CFG
	data[1] = 0b10000001;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	HAL_Delay(2);

	data[0] = 0x05; // SHDN_CFG
	data[1] = 0b00010101;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	HAL_Delay(2);

	data[0] = 0x07; // ASI_CFG0
	data[1] = 0b01110000;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	HAL_Delay(2);

	data[0] = 0x0C; // ASI_CH2
	data[1] = 0b00000000;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	HAL_Delay(2);

	data[0] = 0x3B; // BIAS_CFG
	data[1] = 0b01100000;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	HAL_Delay(2);

	data[0] = 0x41; // CH2_CFG0
	data[1] = 0b00100101; // DRE disabled: 0x20
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	HAL_Delay(2);

	data[0] = 0x42; // CH2_CFG1
	data[1] = 0b00000000; // 0dB: 0x00 ; 9dB: 0x24 ;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	HAL_Delay(2);

	data[0] = 0x43; // CH2_CFG2
	data[1] = 0b11001001; // 0dB: 0xC9 ; -9dB: 0xB7
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	HAL_Delay(2);

	data[0] = 0x6B; // DSP_CFG0*****
	data[1] = 0b00000001;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	HAL_Delay(2);

	data[0] = 0x73; // IN_CH_EN
	data[1] = 0b01000000;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	HAL_Delay(2);

	data[0] = 0x74; // ASI_OUT_CH_EN
	data[1] = 0b01000000;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	HAL_Delay(2);

	data[0] = 0x75; // PWR_CFG
	data[1] = 0b11100000;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	HAL_Delay(2);
}

/* Set the ADC sample rate */
void adc_set_fs(void){
#if defined(FS_16K)
	// Set P = 1 R = 4
	data[0] = 0x05;
	data[1] = 0x94;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	// Set J = 40
	data[0] = 0x06;
	data[1] = 0x28;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	// Set D = 0
	data[0] = 0x07;
	data[1] = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	data[0] = 0x08;
	data[1] = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	// Set NADC = 2
	data[0] = 0x12;
	data[1] = 0x82;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	// Set MADC = 20
	data[0] = 0x13;
	data[1] = 0x94;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	// Set AOSR = 128
	data[0] = 0x14;
	data[1] = 0x80;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

#elif defined(FS_48K)
	// Set P = 1 R = 4
	data[0] = 0x05;
	data[1] = 0x94;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	// Set J = 16
	data[0] = 0x06;
	data[1] = 0x10;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	// Set D = 0
	data[0] = 0x07;
	data[1] = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	data[0] = 0x08;
	data[1] = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	// Set NADC = 2
	data[0] = 0x12;
	data[1] = 0x82;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	// Set MADC = 8
	data[0] = 0x13;
	data[1] = 0x88;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	// Set AOSR = 128
	data[0] = 0x14;
	data[1] = 0x80;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

#elif defined(FS_96K)
	// Set P = 1 R = 4
	data[0] = 0x05;
	data[1] = 0x94;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	// Set J = 8
	data[0] = 0x06;
	data[1] = 0x08;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	// Set D = 0
	data[0] = 0x07;
	data[1] = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	data[0] = 0x08;
	data[1] = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	// Set NADC = 2
	data[0] = 0x12;
	data[1] = 0x82;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	// Set MADC = 4
	data[0] = 0x13;
	data[1] = 0x84;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);

	// Set AOSR = 128
	data[0] = 0x14;
	data[1] = 0x80;
	HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_ADDRESS, data, 2, 100);
#endif
}

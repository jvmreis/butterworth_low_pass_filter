#ifndef _ADC_H_
#define _ADC_H_

/* INCLUDES */
#include "main.h"

/* DEFINES */
//#define ADC_I2C_ADDRESS 0x4C << 1
#define ADC_I2C_ADDRESS 0x4E << 1

extern I2C_HandleTypeDef hi2c1;
extern SAI_HandleTypeDef hsai_BlockA1;

/* FUNCTIONS PROTOTYPE */
void adc_init(void);
void adc_mic_config3(void);
void adc_set_fs(void);																	// Set the ADC sample rate

#endif

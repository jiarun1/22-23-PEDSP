/*
 * Doppler_Radar.h
 *
 *  Created on: Oct 17, 2022
 *      Author: ljr05
 */

#ifndef DOPPLER_RADAR_H_
#define DOPPLER_RADAR_H_

#include "stm32l4xx_hal.h"
#include "arm_math.h"

#define SAMPLE_POINT	1024

typedef struct {

	struct{
		ADC_HandleTypeDef* ADC;
		DMA_HandleTypeDef* ADC_DMA;
		uint16_t Period;
	}Input;

	float32_t Average;
	uint32_t ADC_Measure[SAMPLE_POINT];
	float32_t fft_result[SAMPLE_POINT];
	float32_t fft_mag[SAMPLE_POINT];
	uint32_t max_frequency;
}DopplerADC_t;

extern void Doppler_Radar_Init(DopplerADC_t* Dev, ADC_HandleTypeDef* adc, DMA_HandleTypeDef* adc_dma, uint16_t period);

extern void Doppler_Radar_IQRHandler(DopplerADC_t* Dev);




#endif /* DOPPLER_RADAR_H_ */

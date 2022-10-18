/*
 * Doppler_Radar.c
 *
 *  Created on: Oct 17, 2022
 *      Author: ljr05
 */
#include "Doppler_Radar.h"
#include "stdlib.h"

void Doppler_Radar_Init(DopplerADC_t* Dev, ADC_HandleTypeDef* adc, DMA_HandleTypeDef* adc_dma, uint16_t period)
{
	Dev->Input.ADC = adc;
	Dev->Input.ADC_DMA = adc_dma;
	Dev->Input.Period = period;

	HAL_ADC_Start_DMA(Dev->Input.ADC, Dev->ADC_Measure, SAMPLE_POINT);
}



void Doppler_Radar_IQRHandler(DopplerADC_t* Dev)
{
	if(__HAL_DMA_GET_FLAG(Dev->Input.ADC_DMA, DMA_FLAG_TC1) == RESET)
	{

		return;
	}

	HAL_ADC_Stop_DMA(Dev->Input.ADC);

	__HAL_DMA_CLEAR_FLAG(Dev->Input.ADC_DMA, DMA_FLAG_TC1);

	float32_t Processing_Sig[SAMPLE_POINT];

	// pre processing of the ADC measurement

	Dev->Average = 0;

	for(int i = 0; i < SAMPLE_POINT; i++)
	{
		Processing_Sig[i] = Dev->ADC_Measure[i];
		Dev->Average += (float32_t)Processing_Sig[i] / SAMPLE_POINT;
	}

	for(int i = 0; i < SAMPLE_POINT; i++)
	{
		Processing_Sig[i] -= Dev->Average;
	}



	arm_rfft_fast_instance_f32 S;
	arm_rfft_fast_init_f32(&S, SAMPLE_POINT);
	arm_rfft_fast_f32(&S, Processing_Sig, Dev->fft_result, 0);

	arm_cmplx_mag_f32(Dev->fft_result, Dev->fft_mag, SAMPLE_POINT);

	float32_t max_fft = 0;
	for(uint16_t i = 0; i < SAMPLE_POINT/2;i++)
	{
		if(Dev->fft_mag[i] > max_fft)
		{
			max_fft = Dev->fft_mag[i];
			Dev->max_frequency = i * Dev->Input.Period;
		}
	}

	HAL_ADC_Start_DMA(Dev->Input.ADC, Dev->ADC_Measure, SAMPLE_POINT);
}




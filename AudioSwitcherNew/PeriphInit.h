#pragma once

#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_adc.h>
#include <stm32f10x_dma.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_tim.h>
#include "Channel.h"
#include "cmath"

using namespace Models;


#ifdef __cplusplus
extern "C" {
#endif
	
	namespace Init
	{
		class PeriphInit
		{
		public:
			static void InitGPIO(Channel* channels[], uint8_t channelsCount);
			static void InitHSI();
			static void InitPLL();
			static void ADC_DMAInit(Channel* channels[], const uint32_t usingDMAPeriph, DMA_Channel_TypeDef* usingDMA, const uint32_t UsingADCPeriph, ADC_TypeDef* usingADC, uint32_t buffer, uint8_t channelsCount);
			static void InitTimers(uint32_t measurementFrequency, uint32_t measurementsDuration);
		};
	}
	
#ifdef __cplusplus
}
#endif


#pragma once
#include "IIndicators.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include <functional>

namespace Periph
{
	class IndicatorsProvider : public IIndicators
	{
	public:
		IndicatorsProvider(uint32_t ledAPB2Periph, GPIO_TypeDef* ledGPIOGpoup, int16_t ledGPIOPin, uint32_t ampAPB2Periph, GPIO_TypeDef* ampGPIOGpoup, int16_t ampGPIOPin);
		std::function<void(bool state)> LEDWriteState;
		std::function<void(bool state)> AMPWriteState;
		
	private:
		const uint32_t mLedAPB2Periph;
		GPIO_TypeDef* mLedGPIOGroup;
		const int16_t mLedGPIOPin;
		const uint32_t mAmpAPB2Periph;
		GPIO_TypeDef* mAmpGPIOGroup;
		const int16_t mAmpGPIOPin;
		
	};
}

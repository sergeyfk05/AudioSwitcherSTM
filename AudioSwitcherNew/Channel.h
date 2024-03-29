#pragma once
#include "Settings.cpp"
#include <stm32f10x.h>
#include <stdlib.h>
#include "ChannelSwitcher.h"


#ifdef __cplusplus
extern "C" {
#endif
	
	namespace Models
	{
	
class Channel
		{
		
		public:
			Channel(uint8_t ADCCh, GPIO_TypeDef* gPIOGroup, uint8_t pin, uint8_t ADCST, uint16_t MBL, uint32_t MF, uint8_t SC, GPIO_TypeDef* switcherGPIOGroup, uint8_t switcherPin);
		
			~Channel();
			const uint8_t ADCChannel;
			const uint8_t ADCSampleTime;
			const uint16_t MeasurementsBufferLength;
			uint8_t* MeasurementsBuffer;
			const uint16_t Pin;
			const uint32_t MeasurementFrequency;
			const uint8_t SkipCoef;
			void WriteMeasurement(uint8_t value);
			uint32_t GetCurrentPos();
			GPIO_TypeDef* GetGPIOGroup();
			ChannelSwitcher* GetSwitcher();
		
		private:
			ChannelSwitcher* Switcher;
			GPIO_TypeDef* GPIOGroup;
			uint8_t skipped = 1;
			uint32_t currentPos = 0;
			
		};
	
#ifdef __cplusplus
	}
#endif
}

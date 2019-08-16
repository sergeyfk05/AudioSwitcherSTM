#pragma once
#include "Settings.cpp"
#include <stm32f10x.h>
#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif
	
	namespace Models
	{
	
		struct Rellay
		{
		public:
			Rellay(GPIO_TypeDef* gPIOGproup, uint16_t pin)
				: Pin(pin)
			{
				GPIOGroup = gPIOGproup;
			}
			const uint16_t Pin;
			GPIO_TypeDef* GetGPIOGroup()  const
			{
				return GPIOGroup;
			}
		private:
			GPIO_TypeDef* GPIOGroup;
		};
	
		class Channel
		{
		
		public:
			Channel(uint8_t ADCCh, uint8_t pin, GPIO_TypeDef* gPIOGroup, uint8_t ADCST, uint16_t MBL, uint32_t MF, uint8_t SC, uint8_t relayPin, GPIO_TypeDef* relayGPIOGroup);
		
			~Channel();
			const uint8_t ADCChannel;
			const uint8_t ADCSampleTime;
			const uint16_t MeasurementsBufferLength;
			uint8_t* MeasurementsBuffer;
			const uint16_t Pin;
			const Rellay rellay;
			const uint32_t MeasurementFrequency;
			const uint8_t SkipCoef;
			void WriteMeasurement(uint8_t value);
			uint32_t GetCurrentPos();
			GPIO_TypeDef* GetGPIOGroup();
		
		private:
			GPIO_TypeDef* GPIOGroup;
			uint8_t skipped = 1;
			uint32_t currentPos = 0;
		};
	
#ifdef __cplusplus
	}
#endif
}

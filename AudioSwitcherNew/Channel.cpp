#include "Channel.h"
#include <stm32f10x.h>

namespace Models
{

	Channel::Channel(uint8_t ADCCh, uint8_t pin, GPIO_TypeDef* gPIOGroup, uint8_t ADCST, uint16_t MBL, uint32_t MF, uint8_t SC, uint8_t relayPin, GPIO_TypeDef* relayGPIOGroup)
		: ADCChannel(ADCCh)
		, Pin(pin)
		, GPIOGroup(gPIOGroup)
		, ADCSampleTime(ADCST)
		, MeasurementsBufferLength(MBL)
		, MeasurementFrequency(MF)
		, MeasurementsBuffer((uint8_t*)malloc(sizeof(uint8_t) * MBL))
		, SkipCoef(SC)
		, rellay(relayGPIOGroup, relayPin)
	{
	}
	Channel::~Channel()
	{
		delete MeasurementsBuffer;
	}

	void Channel::WriteMeasurement(uint8_t value)
	{
		if (skipped < SkipCoef)
		{
			skipped++;
			return;
		}
	
		skipped = 1;
		MeasurementsBuffer[currentPos++] = value;
		currentPos %= MeasurementsBufferLength;
	}
	uint32_t Channel::GetCurrentPos()
	{
		return currentPos;
	}

	GPIO_TypeDef* Channel::GetGPIOGroup()
	{
		return GPIOGroup;
	}
}
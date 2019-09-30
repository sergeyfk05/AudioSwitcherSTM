#include "IndicatorsProvider.h"

namespace Periph
{	
	IndicatorsProvider::IndicatorsProvider(uint32_t ledAPB2Periph, GPIO_TypeDef* ledGPIOGpoup, int16_t ledGPIOPin, uint32_t ampAPB2Periph, GPIO_TypeDef* ampGPIOGpoup, int16_t ampGPIOPin)
		: mLedAPB2Periph(ledAPB2Periph),
		mLedGPIOPin(ledGPIOPin),
		mAmpAPB2Periph(ampAPB2Periph),
		mAmpGPIOPin(ampGPIOPin)
	{
		mLedGPIOGroup = ledGPIOGpoup;
		mAmpGPIOGroup = ampGPIOGpoup;
		
		//init periph
		GPIO_InitTypeDef GPIO_InitStructure;
	
		if (mLedGPIOPin != -1)
		{
					
			RCC_APB2PeriphClockCmd(ledAPB2Periph, ENABLE);
			GPIO_InitStructure.GPIO_Pin = mLedGPIOPin;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_Init(mLedGPIOGroup, &GPIO_InitStructure);
			GPIO_WriteBit(mLedGPIOGroup, mLedGPIOPin, Bit_RESET);
			
			this->LEDWriteState = [=](bool state) 
			{
				GPIO_WriteBit(mLedGPIOGroup, mLedGPIOPin, (BitAction)state);
			};
		}
		
		
		if (mAmpGPIOPin != -1)
		{
			RCC_APB2PeriphClockCmd(mAmpAPB2Periph, ENABLE);	
			GPIO_InitStructure.GPIO_Pin = mAmpGPIOPin;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_Init(mAmpGPIOGroup, &GPIO_InitStructure);
			GPIO_WriteBit(mAmpGPIOGroup, mAmpGPIOPin, Bit_RESET);
			this->AMPWriteState = [=](bool state) 
			{
				GPIO_WriteBit(mAmpGPIOGroup, mAmpGPIOPin, (BitAction)state);
			};
		}

	}
}
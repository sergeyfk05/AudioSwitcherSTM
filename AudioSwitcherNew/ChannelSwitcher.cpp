#include "ChannelSwitcher.h"

namespace Models
{
	ChannelSwitcher::ChannelSwitcher(GPIO_TypeDef* gPIOGroup, uint8_t pin)
		: GPIOGroup(gPIOGroup)
		, Pin(pin)
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		
		GPIO_InitStructure.GPIO_Pin = pin;
		GPIO_Init(gPIOGroup, &GPIO_InitStructure);
		GPIO_WriteBit(gPIOGroup, pin, Bit_RESET);
	
	}
	
	bool ChannelSwitcher::SetStatus(ChannelStatus chSt)
	{
		if (chSt == Open)
		{
			GPIO_WriteBit(GPIOGroup, Pin, Bit_SET);
		}
		else
		{
			GPIO_WriteBit(GPIOGroup, Pin, Bit_RESET);
		}		
		
		return true;
	}
	
	ChannelStatus ChannelSwitcher::GetStatus()
	{
		return GPIO_ReadOutputDataBit(GPIOGroup, Pin) == Bit_SET ? ChannelStatus::Open : ChannelStatus::Close;
	}
}
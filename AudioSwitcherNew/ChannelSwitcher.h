#pragma once
#include "Settings.cpp"
#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif
	
	namespace Models
	{
		
		enum ChannelStatus
		{
			Close,
			Open
		};

		class ChannelSwitcher
		{
		public:
			ChannelSwitcher(GPIO_TypeDef* gPIOGroup, uint8_t pin);
			bool SetStatus(ChannelStatus chSt);
			ChannelStatus GetStatus();
		private:
			GPIO_TypeDef* GPIOGroup;
			const uint8_t Pin;
		};
	}
	
#ifdef __cplusplus
}		  
#endif 

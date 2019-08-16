#pragma once
#include "Settings.cpp"

#include <stm32f10x.h>
#include "Channel.h"
#include <stm32f10x_gpio.h>


//defines
#if DEBUG1 == 1

#include <string.h>
#include <stdlib.h>
#include <cstdio>
#include "USART.h"
#endif



#ifdef __cplusplus
extern "C" {
#endif
	namespace Models
	{
		class MusicDetector
		{
			const uint8_t countOfIterationsForSwitch;
			const uint8_t channelsCount;
			bool** history;
			bool* statuses;
			Channel** channels;
			
			static bool isMusic(Channel* ch, bool isActive);
		public:
			void CheckAndSwitch();
			MusicDetector(Channel* _channels[], uint8_t _countOfIterationsForSwitch, uint8_t _channelsCount);
			~MusicDetector();
		};

		
	}
	 
#ifdef __cplusplus
}
#endif

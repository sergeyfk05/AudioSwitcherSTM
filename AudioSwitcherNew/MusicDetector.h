#pragma once
#include "Settings.cpp"

#include <stm32f10x.h>
#include "Channel.h"
#include "IIndicators.h"
#include <stm32f10x_gpio.h>

using namespace Periph;

#define MaxEqualToZeroValue 3 //the value which equal or less is equated to zero
#define MaxAvarageForNoise (float)0.4//this is max avarage of measurement values so that the sound is considered noise for NOT active channel
#define MaxAvarageForActiveNoise (float)0.06//this is max avarage of measurement values so that the sound is considered noise for active channel
#define CountOfConsecutiveZeroValueForNoise 250//if count of consecutive zero values bugger it that sound is equated to noise
#define MinCountOfZeroValue 550//it's minimum count of zero values to equate to music(not consecutive)


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
			
			IIndicators* indicators;
		public:
			void CheckAndSwitch();
			MusicDetector(Channel* _channels[], uint8_t _countOfIterationsForSwitch, uint8_t _channelsCount, IIndicators* _indicators);
			~MusicDetector();
		};

		
	}
	 
#ifdef __cplusplus
}
#endif

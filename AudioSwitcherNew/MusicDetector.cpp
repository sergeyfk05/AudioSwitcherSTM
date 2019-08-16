#include <stm32f10x.h>
#include "MusicDetector.h"
#include "Settings.cpp"


namespace Models
{
	MusicDetector::MusicDetector(Channel* _channels[], uint8_t _countOfIterationsForSwitch, uint8_t _channelsCount)
		: countOfIterationsForSwitch(_countOfIterationsForSwitch)
		, channelsCount(_channelsCount)
	{
		channels = _channels;
				statuses = (bool*)calloc(sizeof(bool), channelsCount);
				history = (bool**)calloc(sizeof(bool*), countOfIterationsForSwitch);
				for (uint8_t i = 0; i < countOfIterationsForSwitch; i++)
					history[i] = (bool*)calloc(sizeof(bool), channelsCount);
	}
	MusicDetector::~MusicDetector()
	{
		delete statuses;
		for (uint8_t i = 0; i < countOfIterationsForSwitch; i++)
			delete history[i];
		
		delete history;
	}
	
	bool MusicDetector::isMusic(Channel* ch, bool isActive)
	{
		uint16_t countOfZero = 0;
		bool isLastZero = false;
		float avarage = 0;
	
		for (uint32_t i = ch->GetCurrentPos(), processed = 0; processed < ch->MeasurementsBufferLength; ++i %= ch->MeasurementsBufferLength, processed++)
		{		
			avarage += ch->MeasurementsBuffer[i];
			if (ch->MeasurementsBuffer[i] < MaxEqualToZeroValue)
			{
				countOfZero++;
			
			}
		}
		
		avarage /= ch->MeasurementsBufferLength;		

		return countOfZero > MinCountOfZeroValue &&  (isActive ? avarage > MaxAvarageForActiveNoise : avarage > MaxAvarageForNoise);
	}
	
	void MusicDetector::CheckAndSwitch()
	{
		
		//ведем историю результатов проверки на музыку
		static uint8_t currentPos = 0;
		for (uint8_t i = 0; i < channelsCount; i++, ++currentPos %= countOfIterationsForSwitch)
		{
			
			history[currentPos][i] = isMusic(channels[i], statuses[i]);
#if DEBUG1 == 1
			
			char b[10];
			char buf[10];
			buf[0] = 0;
			sprintf(b, "%d", i);	
			strcat(buf, b);
			strcat(buf, ":  ");
			if (history[currentPos][i])
				strcat(buf, "true");
			else
				strcat(buf, "false");

			strcat(buf, ";\n");
			USART::PutString(USART1, buf);
#endif

		}
		
		//смоттрим достаточно ли одинаковых результатов дл€ изменени€ состо€ни€ и измен€ем
		for(uint8_t i = 0 ; i < channelsCount ; i++)
		{
			uint8_t isChangable = 0;
			for (uint8_t y = 0; y < countOfIterationsForSwitch; y++)
			{
				if (!statuses[i] == history[y][i])
					isChangable++;
			}
			
			if (isChangable == countOfIterationsForSwitch)
				statuses[i] = !statuses[i];
		}
		
		
		//вкл/выкл реле
		bool isSelected = false;
		for (uint8_t i = 0; i < channelsCount; i++)
		{
			if ((!isSelected) && (statuses[i]))
			{
				isSelected = true;
				GPIO_WriteBit(channels[i]->rellay.GetGPIOGroup(), channels[i]->rellay.Pin, Bit_SET);
			}
			else
			{
				GPIO_WriteBit(channels[i]->rellay.GetGPIOGroup(), channels[i]->rellay.Pin, Bit_RESET);
			}
		}
		
#if USE_LED == 1 || USE_AMP == 1
		if (isSelected)
		{
#if USE_LED
			GPIO_WriteBit(LED_GPIO_GROUP, LED_GPIO_PIN, Bit_SET);
#endif
#if USE_AMP
			GPIO_WriteBit(AMP_GPIO_GROUP, AMP_GPIO_PIN, Bit_SET);
#endif
		}
		else
		{
#if USE_LED
			GPIO_WriteBit(LED_GPIO_GROUP, LED_GPIO_PIN, Bit_RESET);
#endif
#if USE_AMP
			GPIO_WriteBit(AMP_GPIO_GROUP, AMP_GPIO_PIN, Bit_RESET);
#endif
		}
#endif

	}
}
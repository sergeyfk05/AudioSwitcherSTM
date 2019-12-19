#include <stm32f10x.h>
#include "MusicDetector.h"
#include "Settings.cpp"


namespace Models
{
	MusicDetector::MusicDetector(Channel* _channels[], uint8_t _countOfIterationsForSwitch, uint8_t _channelsCount, IIndicators* _indicators)
		: countOfIterationsForSwitch(_countOfIterationsForSwitch)
		, channelsCount(_channelsCount)
	{
		indicators = _indicators;
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
	
	
	//method which checks whether this is music
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
		
		//calculate avarage value
		avarage /= ch->MeasurementsBufferLength;		

		
		//return the result
		return countOfZero > MinCountOfZeroValue &&  (isActive ? avarage > MaxAvarageForActiveNoise : avarage > MaxAvarageForNoise);
	}
	
	void MusicDetector::CheckAndSwitch()
	{
		
		static uint8_t currentPos = 0;
		for (uint8_t i = 0; i < channelsCount; i++, ++currentPos %= countOfIterationsForSwitch)
		{
			
			//the history of isMusic() results
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
		
		for (uint8_t i = 0; i < channelsCount; i++)
		{
			//if count of consecutive same results of isMusic() are enought switch channel state
			uint8_t isChangable = 0;
			for (uint8_t y = 0; y < countOfIterationsForSwitch; y++)
			{
				if (!statuses[i] == history[y][i])
					isChangable++;
			}
			
			if (isChangable == countOfIterationsForSwitch)
				statuses[i] = !statuses[i];
		}
		
		
		//turn on/off the audio-rellay
		bool isSelected = false;
		for (uint8_t i = 0; i < channelsCount; i++)
		{
			if ((!isSelected) && (statuses[i]))
			{
				isSelected = true;
				channels[i]->GetSwitcher()->SetStatus(ChannelStatus::Open);
			}
			else
			{
				channels[i]->GetSwitcher()->SetStatus(ChannelStatus::Close);
			}
		}
		
		//turn on/off the LED oe/and Amplifier if enabled
		if(isSelected)
		{
			indicators->LEDWriteState(true);
			indicators->AMPWriteState(true);
		}
		else
		{
			indicators->LEDWriteState(false);
			indicators->AMPWriteState(false);
		}
	}
}
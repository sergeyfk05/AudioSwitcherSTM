#include <AudioAnalyzer/Analyzer.h>
#include <math.h>

AudioAnalyzer::Analyzer::Analyzer(volatile uint16_t* adc_left_source_param, volatile uint16_t* adc_right_source_param)
{
	ch_info[0].adc_source = adc_left_source_param;
	ch_info[1].adc_source = adc_right_source_param;

	ch_info[0].Reset();
	ch_info[1].Reset();
}


void AudioAnalyzer::Analyzer::ChannelData::Reset()
{
	adc_meas_count = 0;
	adc_meas_sum = 0;
	min = UINT16_MAX;
	max = 0;
	adc_buffer_index = 0;
}


uint16_t AudioAnalyzer::Analyzer::ChannelData::GetDelta()
{
	return abs(max - min);
}

uint16_t AudioAnalyzer::Analyzer::ChannelData::GetMean()
{
	if(adc_meas_count == 0)
	{
		return 0;
	}

	return adc_meas_sum / adc_meas_count;
}


void AudioAnalyzer::Analyzer::ChannelData::Update()
{
	adc_meas_count++;
	adc_meas_sum += *adc_source;

	if(((uint32_t)adc_source != 0x200000b0) && ((uint32_t)adc_source != 0x200000b2))
	{
		return;
	}

	if(adc_buffer_index < SEGMENT_ADC_MEAS_COUNT - 1)
	{
		adc_buffer[adc_buffer_index++] = *adc_source;
	}

	if(*adc_source < min)
	{
		min = *adc_source;
	}

	if(*adc_source > max)
	{
		max = *adc_source;
	}
}

uint16_t AudioAnalyzer::Analyzer::ChannelData::GetZeroValueDelta(uint16_t accuracity)
{
	uint16_t i = 0;
	uint16_t last_zero;
	uint16_t min = UINT16_MAX;
	uint16_t max = 0;

	//find first zero
	for(; i < SEGMENT_ADC_MEAS_COUNT; i++)
	{
		if(adc_buffer[i] <= accuracity)
		{
			last_zero = i;
			break;
		}
	}


	for(; i < SEGMENT_ADC_MEAS_COUNT; i++)
	{
		if(adc_buffer[i] <= accuracity)
		{
			if((i - last_zero) < min)
			{
				min = i - last_zero;
			}


			if((i - last_zero) > max)
			{
				max = i - last_zero;
			}

			last_zero = i;
		}
	}

	return max - min;

}

uint16_t AudioAnalyzer::Analyzer::ChannelData::GetMaxValueDelta(float accuracity)
{
	uint16_t i = 0;
	uint16_t last_max;
	uint16_t min_l = UINT16_MAX;
	uint16_t max_l = 0;

	//find first max
	for(; i <= adc_buffer_index; i++)
	{
		if(adc_buffer[i] >= max * accuracity)
		{
			last_max = i;
			break;
		}
	}


	for(; i <= adc_buffer_index; i++)
	{
		if(adc_buffer[i] >= max * accuracity)
		{
			if((i - last_max) < min_l)
			{
				min_l = i - last_max;
			}


			if((i - last_max) > max_l)
			{
				max_l = i - last_max;
			}

			last_max  = i;
		}
	}

	return max_l - min_l;
}

void AudioAnalyzer::Analyzer::UpdateData()
{
	ch_info[0].Update();
	ch_info[1].Update();

	bool buf[2] = {false, false};

	if((ch_info[0].adc_meas_count >= SEGMENT_ADC_MEAS_COUNT) || (ch_info[1].adc_meas_count >= SEGMENT_ADC_MEAS_COUNT))
	{
		//check music
		for(uint8_t i = 0; i < 2; i++)
		{
			if((ch_info[i].GetMean() >= 0) && (ch_info[i].GetMean() <= 65))
			{
				if(ch_info[i].GetDelta() >= 30)
				{
					buf[i] = true;
				}
			}
			else
			{
				if(ch_info[i].min < 15)
				{
					buf[i] = true;
				}
			}
		}

		if((ch_info[0].GetDelta() <= 15) || (ch_info[1].GetDelta() <= 15))
		{
			buf[0] = false;
			buf[1] = false;
		}

		/*if(abs(ch_info[0].GetMean() - ch_info[1].GetMean()) >= ((ch_info[0].GetDelta() + ch_info[1].GetDelta())/2))
		{
			buf[0] = false;
			buf[1] = false;
		}*/

		if((ch_info[0].GetDelta() <= 125) || (ch_info[1].GetDelta() <= 125))
				{
		last_freq[0] = ch_info[0].GetMaxValueDelta();
		last_freq[1] = ch_info[1].GetMaxValueDelta();
		if(last_freq[0] < 20)
		{
			if(last_freq[1] < 20)
			{
				//if(abs(last_freq[0] - last_freq[1]) < 10)
				{
					buf[0] = false;
					buf[1] = false;
				}
			}
		}
				}

		ch_info[0].Reset();
		ch_info[1].Reset();

		if(buf[0] && buf[1])
		{
			if(good_segments_count < MIN_SEGMENTS_FOR_AUDIO*5)
			{
				good_segments_count++;
				if(good_segments_count == MIN_SEGMENTS_FOR_AUDIO)
					good_segments_count = MIN_SEGMENTS_FOR_AUDIO * 2;
			}
		}
		else
		{
			if(good_segments_count > 0)
			{
				good_segments_count--;
			}
		}
	}
}

bool AudioAnalyzer::Analyzer::IsAudio()
{
	return good_segments_count >= MIN_SEGMENTS_FOR_AUDIO;
}

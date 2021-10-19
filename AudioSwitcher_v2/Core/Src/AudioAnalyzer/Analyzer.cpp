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

	if(*adc_source < min)
	{
		min = *adc_source;
	}

	if(*adc_source > max)
	{
		max = *adc_source;
	}
}


void AudioAnalyzer::Analyzer::UpdateData()
{
	ch_info[0].Update();
	ch_info[1].Update();

	bool buf = false;

	if((ch_info[0].adc_meas_count >= SEGMENT_ADC_MEAS_COUNT) || (ch_info[1].adc_meas_count >= SEGMENT_ADC_MEAS_COUNT))
	{
		//check music
		for(uint8_t i = 0; i < 2; i++)
		{
			if(ch_info[i].min > 35)
			{
				continue;
			}

			if(ch_info[i].GetMean() > 35)
			{
				if(ch_info[i].min <= 2)
				{
					buf = true;
					break;
				}
			}
			else
			{
				if(ch_info[i].GetDelta() > 20)
				{
					buf = true;
					break;
				}
			}
		}

		ch_info[0].Reset();
		ch_info[1].Reset();

		if(buf)
		{
			if(good_segments_count < UINT16_MAX)
			{
				good_segments_count++;
			}
		}
		else
		{
			good_segments_count = 0;
		}
	}
}

bool AudioAnalyzer::Analyzer::IsAudio()
{
	return good_segments_count >= MIN_SEGMENTS_FOR_AUDIO;
}

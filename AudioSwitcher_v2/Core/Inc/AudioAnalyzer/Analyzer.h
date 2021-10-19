#pragma once

#include <stm32g0xx_hal.h>
#define SEGMENT_ADC_MEAS_COUNT		(400)
#define MIN_SEGMENTS_FOR_AUDIO		(25)


namespace AudioAnalyzer
{
	class Analyzer
	{
	private:
		struct ChannelData
		{
			volatile uint16_t* adc_source;
			uint32_t adc_meas_sum;
			uint16_t adc_meas_count;
			uint16_t min;
			uint16_t max;

			void Reset();
			uint16_t GetMean();
			uint16_t GetDelta();
			void Update();
		};

		ChannelData ch_info[2];
		uint16_t good_segments_count;
	protected:
	public:
		Analyzer(volatile uint16_t* adc_left_source_param, volatile uint16_t* adc_right_source_param);
		void UpdateData();
		bool IsAudio();
	};
}

#pragma once

#include <stm32g0xx_hal.h>
#define SEGMENT_ADC_MEAS_COUNT		(335)
#define MIN_SEGMENTS_FOR_AUDIO		(15)


namespace AudioAnalyzer
{
	class Analyzer
	{
	private:
		struct ChannelData
		{
			const volatile uint16_t* adc_source;
			uint32_t adc_meas_sum;
			uint16_t adc_meas_count;
			uint16_t min;
			uint16_t max;

			uint16_t adc_buffer[SEGMENT_ADC_MEAS_COUNT];
			uint16_t adc_buffer_index;

			void Reset();
			uint16_t GetMean();
			uint16_t GetDelta();
			void Update();
			uint16_t GetZeroValueDelta(uint16_t accuracity = 5);
			uint16_t GetMaxValueDelta(float accuracity = 0.8);
		};

		ChannelData ch_info[2];
		uint16_t good_segments_count;
	protected:
	public:
		Analyzer(volatile uint16_t* adc_left_source_param, volatile uint16_t* adc_right_source_param);
		void UpdateData();
		bool IsAudio();
		uint16_t last_freq[2];
	};
}

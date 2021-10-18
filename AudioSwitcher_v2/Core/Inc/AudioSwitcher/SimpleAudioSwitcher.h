#pragma once

#include <AudioSwitcher/AudioSwitcherInterface.h>
#include <stm32g0xx_hal.h>

namespace AudioSwitcher
{
	class SimpleAudioSwitcher : AudioSwitcherInterface
	{
	private:
		const GPIO_TypeDef* group;
		const uint32_t pin;
	public:
		SimpleAudioSwitcher(GPIO_TypeDef* group_param, uint32_t pin_param);
		///true = commutated; false = nc
		void SetCommutationState(bool state = false);
		///true = commutated; false = nc
		bool GetCommutationState();
	};
}

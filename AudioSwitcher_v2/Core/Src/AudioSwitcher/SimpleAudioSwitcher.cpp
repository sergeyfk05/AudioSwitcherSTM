#include <AudioSwitcher/SimpleAudioSwitcher.h>


AudioSwitcher::SimpleAudioSwitcher::SimpleAudioSwitcher(GPIO_TypeDef* group_param, uint32_t pin_param)
	: group(group_param), pin(pin_param)
{
	SetCommutationState(false);
}

void AudioSwitcher::SimpleAudioSwitcher::SetCommutationState(bool state)
{
	HAL_GPIO_WritePin((GPIO_TypeDef*)group, pin, state ? GPIO_PIN_RESET : GPIO_PIN_SET);
}


bool AudioSwitcher::SimpleAudioSwitcher::GetCommutationState()
{
	return HAL_GPIO_ReadPin((GPIO_TypeDef*)group, pin) == GPIO_PIN_RESET;
}

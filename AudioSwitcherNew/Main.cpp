#include "Settings.cpp"
#include <stm32f10x.h>
#include "MyMath.h"
#include "MusicDetector.h"
#include "Channel.h"
#include "PeriphInit.h"
#include "USART.h"
#include <functional>


using namespace Models;
using namespace Init;



void Reader(const volatile uint16_t* buffer, Channel** channels, uint8_t channelsCount);
std::function<void()> ReaderHandler;
std::function<void()> SwitcherHandler;

int main()
{
	PeriphInit::InitPLL();
	
	//periph select
	const uint32_t usingDMAPeriph = RCC_AHBPeriph_DMA1;
	DMA_Channel_TypeDef* usingDMA = DMA1_Channel1;
	const uint32_t usingADCPeriph = RCC_APB2Periph_ADC1;
	ADC_TypeDef* usingADC = ADC1;
	
	const uint8_t channelsCount = 2; //channels count
	Channel* channels[channelsCount];
	volatile uint16_t ADCBuffer[channelsCount];
	const uint8_t countOfIterationsForSwitch = 5; //count of detection needed for switch
	const uint8_t ADCChannels[] = { ADC_Channel_3, ADC_Channel_4, ADC_Channel_5, ADC_Channel_6 }; //input channels
	const uint16_t rellayPins[] = { GPIO_Pin_7, GPIO_Pin_6, ADC_Channel_5, GPIO_Pin_15 };//pins which commutation audio channel to output
	const uint8_t ADCSampleTime = ADC_SampleTime_239Cycles5;//ADC sample time, bigger - better
	//static uint16_t* MeasurementsBuffer[ChannelsCount];
	const uint16_t measurementsDuration = 2000;  // how often channels are checked for sound(in ms)
	const uint32_t measurementFrequencies[] = { 1000, 1000, 1000, 1000 }; //count of measurement in second of each channel
	
	
#if DEBUG0 == 1 || DEBUG1 == 1 || DEBUG2 == 1 
	USART::InitUSART();
#endif
	
	uint32_t lcm = 1;
	for (int i = 0; i < channelsCount; i++)
		lcm = Math::LCM(lcm, measurementFrequencies[i]);
	
	for (int i = 0; i < channelsCount; i++)
	{
		uint32_t bufferLength = measurementFrequencies[i] * measurementsDuration / 1000;
	
		channels[i] = new Channel(ADCChannels[i],
			0x0001 << ADCChannels[i], 
			GPIOA,
			ADCSampleTime,
			bufferLength,
			measurementFrequencies[i], 
			lcm / measurementFrequencies[i],
			rellayPins[i],
			GPIOB);
	}
	
	PeriphInit::InitGPIO(channels, channelsCount);
	PeriphInit::ADC_DMAInit(channels, usingDMAPeriph, usingDMA, usingADCPeriph, usingADC, (uint32_t)ADCBuffer, channelsCount);
	PeriphInit::InitTimers(lcm, measurementsDuration);
	
	ReaderHandler = [=, &channels, &ADCBuffer]() {Reader(ADCBuffer, channels, channelsCount); };	
	MusicDetector* detector = new MusicDetector(channels, countOfIterationsForSwitch, channelsCount);
	SwitcherHandler = [=]() {detector->CheckAndSwitch(); };
	
	
	while (1)
	{
		
	}
}


void Reader(const volatile uint16_t* buffer, Channel** channels, uint8_t channelsCount)
{
	static uint16_t currentPos = 0;

	uint16_t a = ADC1->DR;
#if DEBUG0 == 1
	char b[20];
	char buf[20];
	buf[0] = 0;
	strcat(buf, "$");
		
	for (int i = 0; i < channelsCount; i++)
	{
		sprintf(b, "%d", buffer[i]);	
		strcat(buf, b);
		if (i != channelsCount - 1)
			strcat(buf, " ");
	}

	strcat(buf, ";");
	USART::PutString(USART1, buf);
#endif
	
	for (int i = 0; i < channelsCount; i++)
	{
		channels[i]->WriteMeasurement(buffer[i]);
	}
}
extern "C" void TIM2_IRQHandler()
{
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{		
		if (ReaderHandler != NULL)
			ReaderHandler();
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

extern "C" void TIM3_IRQHandler()
{ 
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) 
	{
		NVIC_DisableIRQ(TIM2_IRQn);
		
		if(SwitcherHandler != NULL)
			SwitcherHandler();
		
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		NVIC_EnableIRQ(TIM2_IRQn);		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
#include "PeriphInit.h"
#include <stm32f10x.h>


namespace Init
{	
	void PeriphInit::InitGPIO(Channel* channels[], uint8_t channelsCount)
	{
		//������������ ������ ����� ���
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
		//������������ ������ ��������� ����
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		for (uint8_t i = 0; i < channelsCount; i++)
		{
			GPIO_InitStructure.GPIO_Pin = channels[i]->rellay.Pin;
			GPIO_Init(channels[i]->rellay.GetGPIOGroup(), &GPIO_InitStructure);
			GPIO_WriteBit(channels[i]->rellay.GetGPIOGroup(), channels[i]->rellay.Pin, Bit_RESET);
		}	
	}
	void PeriphInit::InitHSI()
	{
		// ��������� ����������� �� �� �� HSI ()
		//���� �� ��� �������� � HSI ����� ����� ������ ����������
		// ���� ������ ������ HSE � PLL
		if((RCC->CFGR&RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)// �� ����������� �� �� HSI
		{
			// ������ !!! ���������� 05.05.2015 ������� (Fao xis)
			// RCC->CIR |= RCC_CIR_HSIRDYIE; // ����� ����� RCC_CR_HSIRDY (��� ��� ���������� ����������)
			RCC->CIR |= RCC_CIR_HSIRDYC;    // ����� ����� RCC_CR_HSIRDY
 
			RCC->CR |= RCC_CR_HSION;    // ������ HSI
			while((RCC->CR & RCC_CR_HSIRDY) != RCC_CR_HSIRDY);   // �������� ���������� HSI.
 
			//(���������� �������� ��� SYSCLK)
			RCC->CFGR &= ~(RCC_CFGR_SW_0 | RCC_CFGR_SW_1);    // ����������� �� HSI
			while((RCC->CFGR&RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);    // �������� ������������ �� HSI.
		}
	}
	//64MHz
	void PeriphInit::InitPLL()
	{
		uint32_t buf;
 
		if ((RCC->CFGR&RCC_CFGR_SWS) == RCC_CFGR_SWS_PLL)// �� ����������� �� PLL
		// ����� �� �������� ������������� �� HSI
		InitHSI();
 
		RCC->CR &= ~RCC_CR_PLLON;    // ��������� PLL. ������ ������� ����
 
		RCC->CFGR &= ~RCC_CFGR_PLLSRC;
 
		RCC->CFGR &= ~RCC_CFGR_PLLMULL;    //������� ����
		RCC->CFGR |= 0x00380000;     // �������� ������� �� PLLMUL (��� ���� ���������� � 18 ���)
 
		// ������ !!! ���������� 05.05.2015 ������� (Fao xis)
		// RCC->CIR |= RCC_CIR_PLLRDYF; // ����� ����� RCC_CR_PLLRDY (��� ��� ���������� ���� ����������)
		RCC->CIR |= RCC_CIR_PLLRDYC;    // ����� ����� RCC_CR_PLLRDY
 
		RCC->CR |= RCC_CR_PLLON;    // ��������� PLL.
		while((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);   // �������� ���������� PLL.
 
		buf = RCC->CFGR;
		buf &= ~RCC_CFGR_SW;    // �������� ���� SW0 � SW1
		buf |= RCC_CFGR_SW_PLL;
		//(���������� �������� ��� SYSCLK)
		RCC->CFGR = buf;    // ������������ � ������ PLL.
		while((RCC->CFGR&RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);    // �������� ������������ �� PLL.
	}
	
	void PeriphInit::ADC_DMAInit(Channel* channels[], const uint32_t usingDMAPeriph, DMA_Channel_TypeDef* usingDMA, const uint32_t UsingADCPeriph, ADC_TypeDef* usingADC, uint32_t buffer, uint8_t channelsCount)
	{
	
		ADC_InitTypeDef ADC_InitStructure;
	
		//72 / 6 = 12MHz
		RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	
		DMA_InitTypeDef DMA_InitStruct;
		RCC_AHBPeriphClockCmd(usingDMAPeriph, ENABLE);
		DMA_StructInit(&DMA_InitStruct);
		DMA_InitStruct.DMA_BufferSize = channelsCount;
		DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)buffer;
		DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
		DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
		DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&usingADC->DR;
		DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
		DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
		DMA_Init(usingDMA, &DMA_InitStruct);
		DMA_Cmd(usingDMA, ENABLE);
 
	
	
		GPIO_InitTypeDef GPIO_InitStructure;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		for (int i = 0; i < channelsCount; i++)
		{
			GPIO_InitStructure.GPIO_Pin = channels[i]->Pin;
			GPIO_Init(channels[i]->GetGPIOGroup(), &GPIO_InitStructure);
		}
	
		RCC_APB2PeriphClockCmd(UsingADCPeriph | RCC_APB2Periph_AFIO, ENABLE);
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		ADC_InitStructure.ADC_NbrOfChannel = channelsCount;
		ADC_InitStructure.ADC_ScanConvMode = ENABLE;
		ADC_Init(usingADC, &ADC_InitStructure);
	
		for (int i = 0; i < channelsCount; i++)
			ADC_RegularChannelConfig(usingADC, channels[i]->ADCChannel, i + 1, channels[i]->ADCSampleTime);
	
		ADC_Cmd(usingADC, ENABLE);
		ADC_DMACmd(usingADC, ENABLE);
		ADC_ResetCalibration(usingADC);
 
		while (ADC_GetResetCalibrationStatus(usingADC)) ;
		ADC_StartCalibration(usingADC);
 
		while (ADC_GetCalibrationStatus(usingADC)) ;
		ADC_SoftwareStartConvCmd(usingADC, ENABLE);
	}

	void PeriphInit::InitTimers(uint32_t measurementFrequency, uint32_t measurementsDuration) 
	{
		SystemCoreClockUpdate();	
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
		TIM_TimeBaseInitTypeDef TimBaseInit;
		TimBaseInit.TIM_CounterMode = TIM_CounterMode_Up;
		TimBaseInit.TIM_Prescaler = sqrt(SystemCoreClock / measurementFrequency) - 1;
		TimBaseInit.TIM_Period = sqrt(SystemCoreClock / measurementFrequency);
		TimBaseInit.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseInit(TIM2, &TimBaseInit);
	
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		TimBaseInit.TIM_Prescaler = 1 - 1;
		TimBaseInit.TIM_Period = measurementsDuration * measurementFrequency / 1000;
		TIM_TimeBaseInit(TIM3, &TimBaseInit);
	
	
		TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
		TIM_SelectInputTrigger(TIM3, TIM_TS_ITR1);
		TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_External1);
	
		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
		NVIC_SetPriority(TIM2_IRQn, 0);
		NVIC_SetPriority(TIM3_IRQn, 1);
	
		TIM_Cmd(TIM2, ENABLE); 
		TIM_Cmd(TIM3, ENABLE);
	
		NVIC_EnableIRQ(TIM2_IRQn);
		NVIC_EnableIRQ(TIM3_IRQn);
	}
}
#pragma once
#include "Settings.cpp"

#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_gpio.h>
#if DEBUG0 == 1 || DEBUG1 == 1 || DEBUG2 == 1

#include <string.h>
#include <stdlib.h>
#include <cstdio>
#endif


#ifdef __cplusplus
extern "C" {
#endif
	namespace Models
	{	
#if DEBUG0 == 1 || DEBUG1 == 1 || DEBUG2 == 1

		class USART
		{
		public:
			static void InitUSART();			
			static void PutChar(USART_TypeDef* USARTx, char ch);
			static void PutString(USART_TypeDef* USARTx, char * str);
		};
#endif

	}
	 
#ifdef __cplusplus
}
#endif

#include "USART.h"
namespace Models
{
#if DEBUG0 == 1 || DEBUG1 == 1 || DEBUG2 == 1

	void USART::InitUSART()
	{
		// Настройка ножек ==============
		GPIO_InitTypeDef MyPin;              // создаём переменную (структуру) для ножек
		GPIO_StructInit(&MyPin);                 // настройка структуры по умолчанию
 
		// Настройка тактировани
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
 
		// PA10 == RX - Настроить на вход
		MyPin.GPIO_Pin   = GPIO_Pin_10;                        // PA10 == RX.
		MyPin.GPIO_Mode  = GPIO_Mode_IN_FLOATING;              // определяем режим работы ножки
		GPIO_Init(GPIOA, &MyPin);                            // Запись регистров
 
		// PA9 == TX - Настроить на выход (альтернативная ф-ия)
		MyPin.GPIO_Pin   = GPIO_Pin_9;                         // PA9 == TX
		MyPin.GPIO_Mode  = GPIO_Mode_AF_PP;                    // определяем режим работы ножки
		MyPin.GPIO_Speed = GPIO_Speed_50MHz;                   // так как режим на выход то определяем скорость работы ножки
		GPIO_Init(GPIOA, &MyPin);                            // Запись регистров
		// ==============
 
 
		// Настройка USART1 ==============
		USART_InitTypeDef MyUSART;
		USART_StructInit(&MyUSART);
 
		// Настройка тактировани
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
 
		MyUSART.USART_BaudRate            = 115200;
		MyUSART.USART_WordLength          = USART_WordLength_8b;
		MyUSART.USART_StopBits            = USART_StopBits_1;
		MyUSART.USART_Parity              = USART_Parity_No;
		MyUSART.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
		MyUSART.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
 
		USART_Init(USART1, &MyUSART);               // Заносим чначения в регистры
		USART_Cmd(USART1, ENABLE);                  // Запускаем USART1
		// ==============
	}
	void USART::PutChar(USART_TypeDef* USARTx, char ch)
	{
		while (!(USARTx->SR & USART_SR_TC)) ;
		USARTx->DR = ch;  
	}
	void USART::PutString(USART_TypeDef* USARTx, char * str)
	{
		while (*str != 0)
		{
			PutChar(USARTx, *str);
			str++;
		}
	}
#endif
}
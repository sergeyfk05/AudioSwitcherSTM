
#define DEBUG0 0//init USART and send all measurement values
#define DEBUG1 1//init USART and send information about recognition music or not
#define DEBUG2 0//just init USART
#define LED_GPIO_PERIPH RCC_APB2Periph_GPIOC
#define LED_GPIO_GROUP GPIOC
#define LED_GPIO_PIN GPIO_Pin_13 //set (-1) to disable led indicator;
#define AMP_GPIO_PERIPH RCC_APB2Periph_GPIOB
#define AMP_GPIO_GROUP GPIOB
#define AMP_GPIO_PIN GPIO_Pin_12 // set (-1) to disable turning amplifier;
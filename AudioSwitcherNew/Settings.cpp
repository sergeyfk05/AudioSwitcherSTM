
#define DEBUG0 0//init USART and send all measurement values
#define DEBUG1 1//init USART and send information about recognition music or not
#define DEBUG2 0//just init USART
#define MaxEqualToZeroValue 3 //the value which equal or less is equated to zero
#define MaxAvarageForNoise (float)0.4//this is max avarage of measurement values so that the sound is considered noise for NOT active channel
#define MaxAvarageForActiveNoise (float)0.06//this is max avarage of measurement values so that the sound is considered noise for active channel
#define CountOfConsecutiveZeroValueForNoise 250//if count of consecutive zero values bugger it that sound is equated to noise
#define MinCountOfZeroValue 550//it's minimum count of zero values to equate to music(not consecutive)
#define USE_LED 1
#define LED_GPIO_PERIPH RCC_APB2Periph_GPIOC
#define LED_GPIO_GROUP GPIOC
#define LED_GPIO_PIN GPIO_Pin_13
#define USE_AMP 1
#define AMP_GPIO_PERIPH RCC_APB2Periph_GPIOB
#define AMP_GPIO_GROUP GPIOB
#define AMP_GPIO_PIN GPIO_Pin_12
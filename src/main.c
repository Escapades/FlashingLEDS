#include "stm32f4xx_conf.h"
#include "utils.h"

// Private variables
volatile uint32_t time_var1, time_var2;

// Private function prototypes
void Delay(volatile uint32_t nCount);
void init();
void blinky_led();
void SetupTimerDelay(uint16_t time);
void Timed_Wait(uint16_t time);

int main(void) {

	init();
	SetupTimerDelay(1000);
	blinky_led();
	//Updated with a comment for github
	for(;;) {

	}

	return 0;
}

void blinky_led() {
	
	while (1)
		{
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
		//Delay(5000);
		Timed_Wait(1000);
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
		//Delay(5000);		
		Timed_Wait(1000);
		GPIO_SetBits(GPIOD, GPIO_Pin_14);
		//Delay(5000);
		Timed_Wait(1000);
		GPIO_SetBits(GPIOD, GPIO_Pin_15);
		//Delay(5000);
		Timed_Wait(1000);	
		GPIO_ResetBits(GPIOD, GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
		//Delay(5000);
		Timed_Wait(1000);
		}
}

void init() {
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	// ---------- SysTick timer -------- //
	if (SysTick_Config(SystemCoreClock / 1000)) {
		// Capture error
		while (1){};
	}

	// GPIOD Periph clock enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	// Configure PD12, PD13, PD14 and PD15 in output pushpull mode
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/* Use TIM6 to provide one millisecond for de-bounce */
void SetupTimerDelay(uint16_t time)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	/* TIM6 Clock Enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
	/* Time Base Configuration */
	TIM_TimeBaseStructure.TIM_Period = time;
	TIM_TimeBaseStructure.TIM_Prescaler = 8400;
	/* 1/10 ms */
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

	/* Prescaler Configuration */
	TIM_PrescalerConfig(TIM6, 8400, TIM_PSCReloadMode_Immediate);
	
	TIM_Cmd(TIM6, ENABLE);
}

void Timed_Wait(uint16_t time)
{
	/* set period in ARR */
	TIM_SetAutoreload(TIM6, time);
	TIM_SetCounter(TIM6, 0);
	while (TIM_GetFlagStatus(TIM6, TIM_FLAG_Update) != 1)
	{}
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
}

/* Called from systick handler */
void timing_handler() {
	if (time_var1) {
		time_var1--;
	}

	time_var2++;
}

/* Delay a number of systick cycles (1ms) */
void Delay(volatile uint32_t nCount) {
	time_var1 = nCount;
	while(time_var1){};
}

/* Dummy function to avoid compiler error */
void _init() {
}


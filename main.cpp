#include "hal.h"
#include <iostream>
#include <bitset>

using namespace std;

bitset<4> sw;

unsigned int leds_num[8] = {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5,
                            GPIO_PIN_6, GPIO_PIN_8, GPIO_PIN_9,
                            GPIO_PIN_11, GPIO_PIN_12};
unsigned int sw_num[4] = {GPIO_PIN_12, GPIO_PIN_10, GPIO_PIN_8, GPIO_PIN_4};

int step = 0;

int arr[12][3] = {{5, 7, 6},
                    {7, 6, 5},
                    {6, 5, 4},
                    {5, 4, 3},
                    {4, 3, 2},
                    {3, 2, 1},
                    {2, 1, 0},
                    {0, 1, 2},
                    {1, 2, 3},
                    {2, 3, 4},
                    {3, 4, 5},
                    {4, 5, 6}
};

void read_and_set(){
    for(int i = 0; i<4; i++) {
        GPIO_PinState state = HAL_GPIO_ReadPin(GPIOE, sw_num[i]);
        sw[i] = state == GPIO_PIN_SET;
    }
}

void animate_step(int s1, int s2, int s3){
    HAL_GPIO_WritePin(GPIOD, leds_num[s1], GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, leds_num[s2], GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, leds_num[s3], GPIO_PIN_SET);
}

void TIM6_IRQ_Handler(){
    animate_step(arr[step][0], arr[step][1], arr[step][2]);
    step++;
    if (step == 12) step = 0;
    read_and_set();
    WRITE_REG(TIM6_ARR, 500 + sw.to_ulong()*100);
}


int umain()
{
    registerTIM6_IRQHandler(TIM6_IRQ_Handler);
    WRITE_REG(TIM6_ARR, 500);
    WRITE_REG(TIM6_DIER, TIM_DIER_UIE);
    WRITE_REG(TIM6_PSC, 0);
    WRITE_REG(TIM6_CR1, TIM_CR1_CEN);

    __enable_irq();
    return 0;
}

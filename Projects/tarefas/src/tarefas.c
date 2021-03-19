#include "system_tm4c1294.h" // CMSIS-Core
#include "driverleds.h" // device drivers
#include "cmsis_os2.h" // CMSIS-RTOS

#define HALF_LED1_PERIOD 100
#define HALF_LED2_PERIOD 150
#define HALF_LED3_PERIOD 250
#define HALF_LED4_PERIOD 350

osThreadId_t thread1_id, thread2_id, thread3_id, thread4_id;

typedef struct Led_Parameters
{
  uint8_t       led;
  uint32_t      delay;
}ledSelect;

void ledSelectInit(ledSelect* parameters, uint8_t led, uint32_t delay)
{
  parameters->led   = led;
  parameters->delay = delay;
}

void threadled(void *arg){
  uint8_t state = 0;
  uint32_t tick;
  
  ledSelect *parameters = arg;
  
  while(1){
    tick = osKernelGetTickCount();
    
    state ^= parameters->led;
    LEDWrite(parameters->led, state);
    
    osDelayUntil(tick + parameters->delay);
  } // while
} // thread2

void main(void){
  LEDInit( LED4 | LED3 | LED2 | LED1 );

  osKernelInitialize();
  
  ledSelect param1, param2, param3, param4;
  
  ledSelectInit(&param1, LED1, HALF_LED1_PERIOD);
  ledSelectInit(&param2, LED2, HALF_LED2_PERIOD);
  ledSelectInit(&param3, LED3, HALF_LED3_PERIOD);
  ledSelectInit(&param4, LED4, HALF_LED4_PERIOD);

  thread1_id = osThreadNew(threadled, &param1, NULL);
  thread2_id = osThreadNew(threadled, &param2, NULL);
  thread3_id = osThreadNew(threadled, &param3, NULL);
  thread4_id = osThreadNew(threadled, &param4, NULL);

  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1);
} // main

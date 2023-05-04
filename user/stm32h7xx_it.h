#ifndef POWER_MANAGER_SYSTEM_USER_STM32H7xx_IT_H_
#define POWER_MANAGER_SYSTEM_USER_STM32H7xx_IT_H_

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void DMA1_Stream0_IRQHandler(void);
void DMA1_Stream1_IRQHandler(void);
void DMA1_Stream2_IRQHandler(void);
void SPI3_IRQHandler(void);
void TIM6_DAC_IRQHandler(void);
void DMA2_Stream0_IRQHandler(void);

#endif /* POWER_MANAGER_SYSTEM_USER_STM32H7xx_IT_H_ */

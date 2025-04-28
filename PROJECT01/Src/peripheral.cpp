#include "peripheral.hpp"
extern "C"{
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  // Kiểm tra ngắt từ USART6
  if (huart->Instance == USART6)
  {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    traceISR_ENTER();
    xTaskNotifyFromISR(TaskReceiveUART6, (uint32_t)uart6_read_isr, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    traceISR_EXIT();
  }
  if (huart->Instance == USART1)
  {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    traceISR_ENTER();
    xTaskNotifyFromISR(TaskReceiveUART1, (uint32_t)uart1_read_isr, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    traceISR_EXIT();
  }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
    xSemaphoreGiveFromISR(xSemaphoreUART1, pdFALSE);
  if (huart->Instance == USART6)
    xSemaphoreGiveFromISR(xSemaphoreUART6, pdFALSE);
}
void vApplicationIdleHook( void )
{
  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}
}
#ifndef TASK_HANDLER_H
#define TASK_HANDLER_H
#include "MessageParser.hpp"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "timers.h"
#include "LoRa_AS32_lib.h"
#include "Config.h"
#include "Calculate_Function.hpp"
// typedef:
//variable:
// Task Create:
extern TaskHandle_t TaskReceiveUART1;
extern TaskHandle_t TaskReceiveUART6;
extern TaskHandle_t TaskSendUART6;
extern TaskHandle_t TaskSendUART1;
extern TaskHandle_t TaskErrorHandle;
extern TaskHandle_t TaskTestSend;
extern TaskHandle_t TaskTestReceive;
extern TaskHandle_t TaskSetupCommunication;
extern TaskHandle_t TaskSend_or_ReceiveData;
extern TaskHandle_t TaskGetData;
extern TaskHandle_t TaskDisplayData;
//Message Task Create:
extern TaskHandle_t FreeSlotMessageTask;
extern TaskHandle_t RequestSlotMessageTask;
extern TaskHandle_t AckRequestSlotSuccessMessageTask;
extern TaskHandle_t AckRequestSlotFailMessageTask;
extern TaskHandle_t ForwardPacketMessageTask;
extern TaskHandle_t AckPacketToGWMessageTask;
extern TaskHandle_t AckPacketFailToGWMessageTask;
// End.
// Semaphore:
extern SemaphoreHandle_t xSemaphoreUART6;
extern SemaphoreHandle_t xSemaphoreUART1;
// Queue:
extern QueueHandle_t xQueueUART1Send;
extern QueueHandle_t xQueueUART6Send;
// extern QueueHandle_t xQueueMessage;
// Timer:
extern TimerHandle_t xSlotTimer;
extern TimerHandle_t xRequestSlotTimer;
extern TimerHandle_t xBeginTimer;
extern TimerHandle_t xDataSendTimeOut;
extern TimerHandle_t xGetDataTimer;
extern TimerHandle_t xStopSendDataTimer;
extern TimerHandle_t xDisplayDataTimer;
//variable:
extern volatile uint8_t uart6_read_isr;
extern volatile uint8_t uart1_read_isr;
extern volatile uint8_t currentLocalNodeNumber;
extern volatile bool is1stTimeInit;
extern volatile bool MarkZero;
extern volatile uint8_t SetCom_NotifyNum;
//Node needed information:
extern SelfInformation selfInf;
extern neighbourNode neighbourInf[MAX_NEIGHBOUR];
extern SlotInformation slotInf[MAX_SLOT];
extern uint16_t data_adc[5];
extern DataPacket dataBuffer[DATA_BUFFER_SIZE];
extern DataPacket DisplayBuffer[DISPLAY_BUFFER_SIZE];
extern volatile bool isMultiSending;
struct UART_Inf
{
    uint8_t *messagePtr;
    uint16_t length;
};
// task_hanlder:
void firstInitializtion(void);
void Task_Send_UART6_Handler(void *pvParameters);
void Task_Send_UART1_Hanlder(void *pvParameters);
void Task_Receive_UART6_Hanlder(void *pvParameters);
void Task_Receive_UART1_Hanlder(void *pvParameters);
void Task_Error_Handler(void *pvParameters);
//void Task_Message_Check_Handler(void *pvParameters);
void Task_Test_Send_Handler(void *pvParameters);
void Task_Test_Receive_Handler(void *pvParameters);
void Task_Setup_Communication_Handler(void *pvParameters);
void Task_Send_or_ReceiveData_Handler(void *pvParameters);
void Task_Get_Data_Handler(void *pvParameters);
void Task_Display_Data_Handler(void *pvParameters);
//Message Task Handler:
void FreeSlot_Message_Handler(void *pvParameters);
void RequestSlot_Message_Handler(void *pvParameters);
void AckRequestSlotSuccess_Message_Handler(void *pvParameters);
void AckRequestSlotFail_Message_Handler(void *pvParameters);
void ForwardPacket_Message_Handler(void *pvParameters);
void AckPacketToGW_Message_Handler(void *pvParameters);
void AckPacketFailToGW_Message_Handler(void *pvParameters);
// Timer Callback:
void vSlotTimerCallback(TimerHandle_t xTimer);
void vRequestSlotTimerCallback(TimerHandle_t xTimer);
void vBeginTimerCallback(TimerHandle_t xTimer);
void vDataSendTimeOutCallback(TimerHandle_t xTimer);
void vGetDataTimerCallback(TimerHandle_t xTimer);
void vStopSendDataTimerCallback(TimerHandle_t xTimer);
void vDisplayDataTimerCallback(TimerHandle_t xTimer);
#endif //TASK_HANDLER_C_H

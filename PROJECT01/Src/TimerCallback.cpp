#include "task_handler.hpp"

/* Slot Timer Callback function
* The timer ID is used to identify the slot number in period.
*/
void vSlotTimerCallback(TimerHandle_t xTimer)
{
    const uint32_t ulMaxExpiryCountBeforeStopping = MAX_SLOT;
    selfInf.slot = (uint32_t) pvTimerGetTimerID( xTimer );
    selfInf.slot++;
    if( selfInf.slot == ulMaxExpiryCountBeforeStopping) selfInf.slot = 0;
    if(is1stTimeInit == true && MarkZero == false){
        if(selfInf.slot== 0){ 
            is1stTimeInit = false;
            selfInf.NumberOfCycle++;
        }
    }
    else if(MarkZero == true && selfInf.slot== 0){
        MarkZero = false;
        is1stTimeInit = false;
        selfInf.NumberOfCycle++;
    }
    else{
        if(selfInf.slot == 0){
            selfInf.sendLocation = NeighbourLocation(neighbourInf, chooseSlotToForwardPacket(neighbourInf, currentLocalNodeNumber), 
            currentLocalNodeNumber);
            selfInf.PRR = calculatePRR(selfInf.totalSend, selfInf.totalSuccessSend, neighbourInf[selfInf.sendLocation]);
            selfInf.NumberOfCycle++;
        }
        if(slotInf[selfInf.slot].isSlotAvailable == true && slotInf[selfInf.slot].isSlotSetup == false && currentLocalNodeNumber <= MAX_NEIGHBOUR){
            if(DEVICE_TYPE == GATEWAY && selfInf.slot < MAX_NEIGHBOUR){
                xTaskNotify(TaskSetupCommunication, 0, eNoAction);
                SetCom_NotifyNum = SEND_FREE_SLOT_MESSAGE;
            }
        }
        else if(slotInf[selfInf.slot].isSlotAvailable == false && slotInf[selfInf.slot].isSlotSetup == true){
            if(DEVICE_TYPE == NODE && selfInf.NumberOfCycle > 1){
                xTaskNotify(TaskSend_or_ReceiveData, SEND_DATA_MESSAGE, eSetValueWithOverwrite);
                isMultiSending = true;
            }
        else if(slotInf[selfInf.slot].isSlotAvailable == false && slotInf[selfInf.slot].isSlotSetup == false){}
        }
    }
    vTimerSetTimerID( xTimer, ( void * ) selfInf.slot );
}
/* Request Slot Timer Callback function
* This function is called when the request slot timer end.
* It will notify the TaskSetupCommunication to resend Request Slot Messaage.
*/
void vRequestSlotTimerCallback(TimerHandle_t xTimer)
{
    uint32_t ulCount;
    xTaskNotify(TaskSetupCommunication, 0, eNoAction);
    SetCom_NotifyNum = SEND_REQUEST_MESSAGE;
    ulCount = (uint32_t)pvTimerGetTimerID( xTimer );
    ulCount++;
    if( ulCount == MAX_REQUEST_SPAM )
        xTimerStop( xTimer, 0 );
    else
        vTimerSetTimerID( xTimer, ( void * ) ulCount );
}
/* Begin Timer Callback function
* This function is called when the begin timer end. 
* It will notify the TaskSetupCommunication to start setup the communication.
*/
void vBeginTimerCallback(TimerHandle_t xTimer)
{
    if(DEVICE_TYPE == GATEWAY){
    xTimerStart(xSlotTimer, 0);
    xTimerStart(xDisplayDataTimer, 0);
    vTimerSetTimerID(xSlotTimer, (void *)0);
    xTaskNotify(TaskSetupCommunication, 0, eNoAction);
    }
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
    if (DEVICE_TYPE == NODE) xTimerStart(xGetDataTimer, 0);
    HAL_UART_Receive_IT(&huart6, (uint8_t *)&uart6_read_isr, 1);
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&uart1_read_isr, 1);
}

void vDataSendTimeOutCallback(TimerHandle_t xTimer)
{
    xTaskNotify(TaskSend_or_ReceiveData, ACK_TIMEOUT, eSetValueWithOverwrite);
}
void vStopSendDataTimerCallback(TimerHandle_t xTimer)
{
    isMultiSending = false;
}
void vGetDataTimerCallback(TimerHandle_t xTimer)
{
    if(DEVICE_TYPE == NODE)
        xTaskNotify(TaskGetData, 0, eNoAction); 
}

void vDisplayDataTimerCallback(TimerHandle_t xTimer)
{
    xTaskNotify(TaskDisplayData, 0, eNoAction);
}
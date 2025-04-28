#include "task_handler.hpp"
// Free Slot Message Handler:
void FreeSlot_Message_Handler(void *pvParameters)
{
    bool isMessageValid = false;
    // uint8_t DebugData[50];
    UART_Inf messageptr;
    TickType_t xNow = 0;
    TickType_t xExpiry = 0;
    TickType_t xRemaining = 0; //remaining time until slot timer expires
    int slot_check = 0;
    while (1)
    {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
       sprintf((char*)DebugData, "Task Receive Free Slot Message\n");
        if (freeSlotMessage.checkSum == CheckSumCalculate(ReadData, FREE_SLOT_SIZE)){
            HAL_UART_Transmit(&huart1, DebugData, strlen((char*)DebugData), 100);
            if(DEVICE_TYPE == TEST_NODE) xTaskNotify(TaskTestReceive, (uint32_t)FREE_SLOT, eSetValueWithOverwrite);
            else {
                if(selfInf.slot == 0 && is1stTimeInit == true && xTimerIsTimerActive(xSlotTimer) == pdFALSE) slot_check = 0;
                else{
                    if (xTimerIsTimerActive(xSlotTimer) == pdTRUE ){
                        xExpiry = xTimerGetExpiryTime(xSlotTimer);
                        xNow = xTaskGetTickCount();
                        if (xExpiry >= xNow){
                            xRemaining = xExpiry - xNow;
                        }
                        else{
                            xRemaining = (portMAX_DELAY - xNow) + xExpiry;
                        }
                    }
                    if ((int)xRemaining < (int)pdMS_TO_TICKS(500)){
                        if (selfInf.slot + 1 < MAX_SLOT) slot_check = selfInf.slot + 1;
                        else slot_check = 0;
                    }
                    else slot_check = selfInf.slot;
                }
                isMessageValid = true;
                if(currentLocalNodeNumber == MAX_NEIGHBOUR) isMessageValid = false;
                else if(slotInf[slot_check].isSlotAvailable == false) isMessageValid = false;
                else {
                    for(int i = 0; i < MAX_NEIGHBOUR; i++){
                        if(AddressCompare(&neighbourInf[i].Address, &freeSlotMessage.sourceAdress)){
                            isMessageValid = false;
                            break;
                        }
                    }
                }
                if(isMessageValid){
                    if(freeSlotMessage.slot == 0) MarkZero = true;
                    else MarkZero = false;
                    xTaskNotify(TaskSetupCommunication, 0, eNoAction);
                    SetCom_NotifyNum = RECEIVE_FREE_SLOT_MESSAGE;
                }
                else{
                    if(is1stTimeInit){
                        MACAddress GWAddress;
                        GWAddress.OUIBits[0] = 0x00;
                        GWAddress.OUIBits[1] = 0x0A;
                        GWAddress.OUIBits[2] = 0xAA;
                        GWAddress.NICSpecificBits[0] = 0x00;
                        GWAddress.NICSpecificBits[1] = 0x00;
                        GWAddress.NICSpecificBits[2] = 0x01;
                        if(DEVICE_TYPE == GATEWAY || AddressCompare(&freeSlotMessage.sourceAdress, &GWAddress)){
                        slotInf[slot_check].isSlotAvailable = false;
                        slotInf[slot_check].isSlotSetup = false;
                        }
                    }
                }
            }
        }
        else xTaskNotify(TaskErrorHandle, INVALID_MESSAGE, eSetValueWithOverwrite);
    }
}
// Request Slot Message Handler:
void RequestSlot_Message_Handler(void *pvParameters)
{
    // uint8_t DebugData[50];
    UART_Inf messageptr;
    while (1)
    {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
       // sprintf((char*)DebugData, "Task Receive Request Slot Message\n");
        if (requestSlotMessage.checkSum == CheckSumCalculate(ReadData, REQUEST_SLOT_SIZE)){
            // HAL_UART_Transmit(&huart1, DebugData, strlen((char*)DebugData), 100);
            if(DEVICE_TYPE == TEST_NODE) xTaskNotify(TaskTestReceive, (uint32_t)REQUEST_SLOT, eSetValueWithOverwrite);
            else {
                if(AddressCompare(&selfInf.SelfAdress, &requestSlotMessage.destinationAdress)
                && requestSlotMessage.slot == selfInf.slot){
                    xTaskNotify(TaskSetupCommunication, 0, eNoAction);
                    SetCom_NotifyNum = RECEIVE_REQUEST_MESSAGE;
                }
            }
        }
        else xTaskNotify(TaskErrorHandle, INVALID_MESSAGE, eSetValueWithOverwrite);
    }
}
// Ack Request Slot Success Message Handler:
void AckRequestSlotSuccess_Message_Handler(void *pvParameters)
{
    // uint8_t DebugData[50];
    UART_Inf messageptr;
    while (1)
    {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
       // sprintf((char*)DebugData, "Task Receive Ack Request Slot Success Message\n");
        if (ackRequestSlotSuccessMessage.checkSum == CheckSumCalculate(ReadData, ACK_REQUEST_SLOT_SUCCESS_SIZE)){
            // HAL_UART_Transmit(&huart1, DebugData, strlen((char*)DebugData), 100);
            if(DEVICE_TYPE == TEST_NODE) xTaskNotify(TaskTestReceive, (uint32_t)ACK_REQUEST_SLOT_SUCCESS, eSetValueWithOverwrite);
            else { 
                if( AddressCompare(&selfInf.SelfAdress, &ackRequestSlotSuccessMessage.destinationAdress)
                    && AddressCompare(&freeSlotMessage.sourceAdress, &ackRequestSlotSuccessMessage.sourceAdress)){
                    xTaskNotify(TaskSetupCommunication, 0, eNoAction);
                    SetCom_NotifyNum = RECEIVE_ACK_SUCCESS_MESSAGE;
                }
                else if(!AddressCompare(&selfInf.SelfAdress, &ackRequestSlotSuccessMessage.destinationAdress) && is1stTimeInit == false){
                    slotInf[selfInf.slot].isSlotAvailable = false;
                    slotInf[selfInf.slot].isSlotSetup = false;
                }
            }
        }
        else xTaskNotify(TaskErrorHandle, INVALID_MESSAGE, eSetValueWithOverwrite);
    }
}
// Ack Request Slot Fail Message Handler:
void AckRequestSlotFail_Message_Handler(void *pvParameters)
{
    // uint8_t DebugData[50];
    UART_Inf messageptr;
    while (1)
    {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
       // sprintf((char*)DebugData, "Task Receive Ack Request Slot Fail Message\n");
        if (ackRequestSlotFailMessage.checkSum == CheckSumCalculate(ReadData, ACK_REQUEST_SLOT_FAIL_SIZE)){
            // HAL_UART_Transmit(&huart1, DebugData, strlen((char*)DebugData), 100);
            if(DEVICE_TYPE == TEST_NODE) xTaskNotify(TaskTestReceive, (uint32_t)ACK_REQUEST_SLOT_FAIL, eSetValueWithOverwrite);
            else {
                if( AddressCompare(&selfInf.SelfAdress, &ackRequestSlotFailMessage.destinationAdress)
                    && AddressCompare(&freeSlotMessage.sourceAdress, &ackRequestSlotFailMessage.sourceAdress)){
                    xTaskNotify(TaskSetupCommunication, 0, eNoAction);
                    SetCom_NotifyNum = RECEIVE_ACK_FAIL_MESSAGE;
                }
            }
        }
        else xTaskNotify(TaskErrorHandle, INVALID_MESSAGE, eSetValueWithOverwrite);
    }
}

// Forward Packet Message Handler:
void ForwardPacket_Message_Handler(void *pvParameters)
{
    // uint8_t DebugData[50];
    UART_Inf messageptr;
    while (1)
    {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
       // sprintf((char*)DebugData, "Task Receive Forward Packet Message\n");
        if (forwardPacketMessage.checkSum == CheckSumCalculate(ReadData, forwardPacketMessage.messageSize)){
            // HAL_UART_Transmit(&huart1, DebugData, strlen((char*)DebugData), 100);
            if(DEVICE_TYPE == TEST_NODE) xTaskNotify(TaskTestReceive, (uint32_t)FORWARD_PACKET, eSetValueWithOverwrite);
            if(AddressCompare(&selfInf.SelfAdress, &forwardPacketMessage.destinationAdress)){
                xTaskNotify(TaskSend_or_ReceiveData, RECEIVE_DATA_MESSAGE, eSetValueWithOverwrite);
            }
        }
        else
        {
            xTaskNotify(TaskErrorHandle, INVALID_MESSAGE, eSetValueWithOverwrite);
        }
    }
}
// Ack Packet To GW Message Handler:
void AckPacketToGW_Message_Handler(void *pvParameters)
{
    // uint8_t DebugData[50];
    UART_Inf messageptr;
    while (1)
    {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
       // sprintf((char*)DebugData, "Task Receive Ack Packet To GW Message\n");
        if (ackPacketToGWMessage.checkSum == CheckSumCalculate(ReadData, ACK_PACKET_TO_GW_SIZE)){
            // HAL_UART_Transmit(&huart1, DebugData, strlen((char*)DebugData), 100);
            if(DEVICE_TYPE == TEST_NODE) xTaskNotify(TaskTestReceive, (uint32_t)ACK_PACKET_TO_GW, eSetValueWithOverwrite);
            if(AddressCompare(&selfInf.SelfAdress, &ackPacketToGWMessage.destinationAdress)){
                xTaskNotify(TaskSend_or_ReceiveData, RECEIVE_DACK_SUCCESS_MESSAGE, eSetValueWithOverwrite);
            }
        }
        else
        {
            xTaskNotify(TaskErrorHandle, INVALID_MESSAGE, eSetValueWithOverwrite);
        }
    }
}
// Ack Packet Fail To GW Message Handler:
void AckPacketFailToGW_Message_Handler(void *pvParameters)
{
    // uint8_t DebugData[50];
    UART_Inf messageptr;
    while (1)
    {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
       // sprintf((char*)DebugData, "Task Receive Ack Packet Fail To GW Message\n");
        if (ackPacketFailToGWMessage.checkSum == CheckSumCalculate(ReadData, ACK_PACKET_FAIL_TO_GW_SIZE)){
            // HAL_UART_Transmit(&huart1, DebugData, strlen((char*)DebugData), 100); 
            if(DEVICE_TYPE == TEST_NODE) xTaskNotify(TaskTestReceive, (uint32_t)ACK_PACKET_FAIL_TO_GW, eSetValueWithOverwrite);
            if(AddressCompare(&selfInf.SelfAdress, &ackPacketFailToGWMessage.destinationAdress)){
                xTaskNotify(TaskSend_or_ReceiveData, RECEIVE_DACK_FAIL_MESSAGE, eSetValueWithOverwrite);
            }
        }
        else
        {
            xTaskNotify(TaskErrorHandle, INVALID_MESSAGE, eSetValueWithOverwrite);
        }
    }
}

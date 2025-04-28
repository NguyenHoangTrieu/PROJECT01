#include "task_handler.hpp"
// Task Create
TaskHandle_t TaskReceiveUART1;
TaskHandle_t TaskReceiveUART6;
TaskHandle_t TaskSendUART1;
TaskHandle_t TaskSendUART6;
TaskHandle_t TaskErrorHandle;
//TaskHandle_t MessageCheckTask;
TaskHandle_t TaskTestSend;
TaskHandle_t TaskTestReceive;
TaskHandle_t TaskSetupCommunication;
TaskHandle_t TaskSend_or_ReceiveData;
TaskHandle_t TaskGetData;
TaskHandle_t TaskDisplayData;
//Message Task Create:
TaskHandle_t FreeSlotMessageTask;
TaskHandle_t RequestSlotMessageTask;
TaskHandle_t AckRequestSlotSuccessMessageTask;
TaskHandle_t AckRequestSlotFailMessageTask;
TaskHandle_t ForwardPacketMessageTask;
TaskHandle_t AckPacketToGWMessageTask;
TaskHandle_t AckPacketFailToGWMessageTask;
// End.
// Semaphore:
SemaphoreHandle_t xSemaphoreUART6;
SemaphoreHandle_t xSemaphoreUART1;
// Queue:
QueueHandle_t xQueueUART1Send;
QueueHandle_t xQueueUART6Send;
//QueueHandle_t xQueueMessage;
// Timer:
TimerHandle_t xSlotTimer;
TimerHandle_t xRequestSlotTimer;
TimerHandle_t xBeginTimer;
TimerHandle_t xDataSendTimeOut; // ACK Data send timeout
TimerHandle_t xGetDataTimer;
TimerHandle_t xStopSendDataTimer;
TimerHandle_t xDisplayDataTimer;
//variable:
volatile uint8_t uart1_read_isr;
volatile uint8_t uart6_read_isr;
volatile uint8_t currentLocalNodeNumber;
volatile bool is1stTimeInit;
volatile bool MarkZero;
volatile uint8_t SetCom_NotifyNum;
//Node || Gateway needed information:
SelfInformation selfInf;
neighbourNode neighbourInf[MAX_NEIGHBOUR];
SlotInformation slotInf[MAX_SLOT];
uint16_t data_adc[5];
DataPacket dataBuffer[DATA_BUFFER_SIZE];
DataPacket DisplayBuffer[DISPLAY_BUFFER_SIZE];
volatile bool isMultiSending;
//first intalization
void firstInitializtion(void)
{
    // Create Semaphore, Queue, etc:
    xSemaphoreUART1 = xSemaphoreCreateMutex();
    configASSERT( xSemaphoreUART1 != NULL );
    xSemaphoreUART6 = xSemaphoreCreateMutex();
    configASSERT( xSemaphoreUART6 != NULL );
    xQueueUART1Send = xQueueCreate(20, sizeof(UART_Inf));
    configASSERT( xQueueUART1Send != NULL );
    xQueueUART6Send = xQueueCreate(20, sizeof(UART_Inf));
    configASSERT( xQueueUART6Send != NULL );
    xSlotTimer = xTimerCreate("SlotTimer", pdMS_TO_TICKS(SLOT_TIME), pdTRUE, (void*)0, vSlotTimerCallback);
    configASSERT( xSlotTimer != NULL );
    xRequestSlotTimer = xTimerCreate("RequestSlotTimer", pdMS_TO_TICKS(REQUEST_SPAM_TIME), pdTRUE, (void*)0, vRequestSlotTimerCallback);
    configASSERT( xRequestSlotTimer != NULL );
    xBeginTimer = xTimerCreate("BeginTimer", pdMS_TO_TICKS(BEGIN_TIME), pdFALSE, (void*)0, vBeginTimerCallback);
    configASSERT( xBeginTimer != NULL );
    xDataSendTimeOut = xTimerCreate("DataSendTimeOut", pdMS_TO_TICKS(DATA_SEND_TIMEOUT), pdFALSE, (void*)0, vDataSendTimeOutCallback);
    configASSERT( xDataSendTimeOut != NULL );
    xStopSendDataTimer = xTimerCreate("StopSendDataTimer", pdMS_TO_TICKS(SEND_TIME),  pdFALSE, (void*)0, vStopSendDataTimerCallback);
    configASSERT( xStopSendDataTimer != NULL);
    // Create Task:
    BaseType_t status;  
    configASSERT( xSemaphoreUART1 != NULL );
    status = xTaskCreate(Task_Receive_UART6_Hanlder, "TaskReceiveUART6", 150, NULL, 4, &TaskReceiveUART6);
    configASSERT( status == pdPASS );
    status = xTaskCreate(Task_Receive_UART1_Hanlder, "TaskReceiveUART1", 150, NULL, 3, &TaskReceiveUART1);
    configASSERT( status == pdPASS );
    status = xTaskCreate(Task_Send_UART6_Handler, "TaskSendUART6", 150, NULL, 3, &TaskSendUART6);
    configASSERT( status == pdPASS );
    status = xTaskCreate(Task_Send_UART1_Hanlder, "TaskSendUART1", 150, NULL, 3, &TaskSendUART1);
    configASSERT( status == pdPASS );
    status = xTaskCreate(Task_Error_Handler, "TaskErrorHandle", 150, NULL, 3, &TaskErrorHandle);
    configASSERT( status == pdPASS );
    status = xTaskCreate(Task_Setup_Communication_Handler, "TaskSetupCommunication", 250, NULL, 3, &TaskSetupCommunication);
    configASSERT( status == pdPASS );
    status = xTaskCreate(Task_Send_or_ReceiveData_Handler, "TaskSendOrReceiveData", 250, NULL, 3, &TaskSend_or_ReceiveData);
    configASSERT( status == pdPASS );
    // Message Task Create:
    status = xTaskCreate(FreeSlot_Message_Handler, "FreeSlotMessageTask", 150, NULL, 3, &FreeSlotMessageTask);
    configASSERT( status == pdPASS );
    status = xTaskCreate(RequestSlot_Message_Handler, "RequestSlotMessageTask", 150, NULL, 3, &RequestSlotMessageTask);
    configASSERT( status == pdPASS );
    status = xTaskCreate(AckRequestSlotSuccess_Message_Handler, "AckRequestSlotSuccessMessageTask", 150, NULL, 3, &AckRequestSlotSuccessMessageTask);
    configASSERT( status == pdPASS );
    status = xTaskCreate(AckRequestSlotFail_Message_Handler, "AckRequestSlotFailMessageTask", 150, NULL, 3, &AckRequestSlotFailMessageTask);
    configASSERT( status == pdPASS );
    status = xTaskCreate(ForwardPacket_Message_Handler, "ForwardPacketMessageTask", 150, NULL, 3, &ForwardPacketMessageTask);
    configASSERT( status == pdPASS );
    status = xTaskCreate(AckPacketToGW_Message_Handler, "AckPacketToGWMessageTask", 150, NULL, 3, &AckPacketToGWMessageTask);
    configASSERT( status == pdPASS );
    status = xTaskCreate(AckPacketFailToGW_Message_Handler, "AckPacketFailToGWMessageTask", 150, NULL, 3, &AckPacketFailToGWMessageTask);
    configASSERT( status == pdPASS );
    // Other setup:
    is1stTimeInit = true;
    for(int i = 0; i < MAX_SLOT; i++)
    {
        slotInf[i].isSlotSetup = false;
        slotInf[i].isSlotAvailable = true;
    }
    xSemaphoreGive(xSemaphoreUART1);
    xSemaphoreGive(xSemaphoreUART6);
    Operation_MODE_Init(GPIOC, GPIOC, GPIO_PIN_8, GPIO_PIN_9, GENERAL_MODE);
    // self information setup:
    selfInf.SelfAdress.OUIBits[0] = 0x00;
    selfInf.SelfAdress.OUIBits[1] = 0x0A;
    selfInf.SelfAdress.OUIBits[2] = 0xAA;
    selfInf.SelfAdress.NICSpecificBits[0] = NIC_BIT_1;
    selfInf.SelfAdress.NICSpecificBits[1] = NIC_BIT_2;
    selfInf.SelfAdress.NICSpecificBits[2] = NIC_BIT_3;
    xTimerStart(xBeginTimer, 0);
    if(DEVICE_TYPE == GATEWAY){
        status = xTaskCreate(Task_Display_Data_Handler, "TaskDisplayData", 150, NULL, 2, &TaskDisplayData);
        configASSERT( status == pdPASS );
        xDisplayDataTimer = xTimerCreate("DisplayDataTimer", pdMS_TO_TICKS(3000), pdTRUE, (void*)0, vDisplayDataTimerCallback);
        configASSERT( xDisplayDataTimer != NULL );
        selfInf.PRR = 100;
        is1stTimeInit = false;
        selfInf.slot = 0;
        selfInf.totalSend = 0;
        selfInf.totalSuccessSend = 0;
        SetCom_NotifyNum = SEND_FREE_SLOT_MESSAGE;
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
    }
    else if(DEVICE_TYPE == TEST_NODE){
        status = xTaskCreate(Task_Test_Send_Handler, "TaskTestSend", 100, NULL, 3, &TaskTestSend);
        configASSERT( status == pdPASS );
        status = xTaskCreate(Task_Test_Receive_Handler, "TaskTestReceive", 400, NULL, 3, &TaskTestReceive);
        configASSERT( status == pdPASS );
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
    }
    else{
    status = xTaskCreate(Task_Get_Data_Handler, "TaskGetData", 150, NULL, 3, &TaskGetData);
    configASSERT( status == pdPASS );
    xGetDataTimer = xTimerCreate("GetDataTimer", pdMS_TO_TICKS(DATA_GET), pdTRUE, (void*)0, vGetDataTimerCallback);
    configASSERT( xGetDataTimer != NULL );
    selfInf.PRR = 0;
    selfInf.slot = 0;
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
    }
    HAL_ADC_Start_DMA(&hadc1,(uint32_t *)data_adc,5);
    vTaskStartScheduler();
}
// Task Receive UART6 Handler
void Task_Receive_UART6_Hanlder(void* pvParameters)
{
    uint8_t data = 0;
    uint16_t len;
    while (1)
    {
        xTaskNotifyWait(0, 0, (uint32_t*)&data, portMAX_DELAY);
        xSemaphoreTake(xSemaphoreUART6, portMAX_DELAY);
        ReadData[0] = data;
        len = UART_ReceiveMessage(ReadData, MAX_MESSAGE_SIZE, huart6);
        if (len > MAX_MESSAGE_SIZE + 1) 
            xTaskNotify(TaskErrorHandle, BUFFER_OVERFLOW, eSetValueWithOverwrite);
        else
        {
            Message msg(ReadData, len);
            msg.execute();
        }
        HAL_UART_Receive_IT(&huart6, (uint8_t *)&uart6_read_isr, 1);
        xSemaphoreGive(xSemaphoreUART6);
    }
}
// Task Receive UART1 Handler
void Task_Receive_UART1_Hanlder(void* pvParameters)
{
    uint8_t data = 0;
    uint16_t len;
    uint8_t data_usart[10];
    while (1)
    {
        xTaskNotifyWait(0, 0, (uint32_t*)&data, portMAX_DELAY);
        xSemaphoreTake(xSemaphoreUART1, portMAX_DELAY);
        data_usart[0] = data;
        len = UART_ReceiveMessage(data_usart, 10, huart1);
        xTaskNotify(TaskTestSend, (uint32_t)data_usart, eSetValueWithOverwrite);
        HAL_UART_Receive_IT(&huart1, (uint8_t *)&uart1_read_isr, 1);
        xSemaphoreGive(xSemaphoreUART1);
    }
}
// Task Error Handler
void Task_Error_Handler(void* pvParameters)
{
    uint8_t ErrorMesssage[50];
    UART_Inf Emessage;
    ErrorType type;
    while (1)
    {
        xTaskNotifyWait(0, 0, (uint32_t*)&type, portMAX_DELAY);
        xSemaphoreTake(xSemaphoreUART1, portMAX_DELAY);
        switch (type)
        {
            case BUFFER_OVERFLOW:
                sprintf((char*)ErrorMesssage, "Buffer overflow");
                HAL_UART_Transmit(&huart1, ErrorMesssage, strlen((char*)ErrorMesssage), 100);
                break;
            case INVALID_MESSAGE:
                sprintf((char*)ErrorMesssage, "Invalid message");
                HAL_UART_Transmit(&huart1, ErrorMesssage, strlen((char*)ErrorMesssage), 100);
                break;
            default:
                break;
        }
    }
}
// Task Send UART1 Handler
void Task_Send_UART1_Hanlder(void* pvParameters)
{
    UART_Inf data;
    while (1)
    {
        if(huart1.gState == HAL_UART_STATE_READY){
            xSemaphoreGive(xSemaphoreUART1);
            xQueueReceive(xQueueUART1Send, &data, portMAX_DELAY);
        }
        xSemaphoreTake(xSemaphoreUART1, portMAX_DELAY);
        HAL_UART_Transmit(&huart1, data.messagePtr, data.length, 100);
    }
}
// Task Send UART6 Handler
void Task_Send_UART6_Handler(void *pvParameters)
{
    UART_Inf data;
    while (1)
    {
        xQueueReceive(xQueueUART6Send, &data, portMAX_DELAY);
        xSemaphoreTake(xSemaphoreUART6, portMAX_DELAY);
        HAL_UART_Transmit(&huart6, data.messagePtr, data.length, 100);
        xSemaphoreGive(xSemaphoreUART6);
    }
}

/* Setup Communication and Update Table Task Handler, 
this task will handle the communication setup and update the neighbour table */
void Task_Setup_Communication_Handler(void *pvParameters)
{
    uint8_t RequestSpamNum = 0;
    uint8_t DebugData[50];
    UART_Inf messageptr;
    bool isTimerSet = false;
    while (1)
    {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
        //sprintf((char*)DebugData, "Task Setup Communication: %d, %d\n", SetCom_NotifyNum, selfInf.slot);
        //HAL_UART_Transmit(&huart1, DebugData, strlen((char*)DebugData), 100);
        //vTaskDelay(pdMS_TO_TICKS(10));
        switch (SetCom_NotifyNum)
        {
            case RECEIVE_FREE_SLOT_MESSAGE:
                if(!isTimerSet){
                    xTimerReset(xSlotTimer, 0);
                    xTimerStart(xSlotTimer, 0);
                    vTimerSetTimerID(xSlotTimer, (void*)0);
                    isTimerSet = true;
                    selfInf.slot = 0;
                    currentLocalNodeNumber = 0;
                    for (int i = 0; i < MAX_SLOT; i++){
                        slotInf[i].isSlotSetup = false;
                        slotInf[i].isSlotAvailable = true;
                    }
                }
                xTimerReset(xRequestSlotTimer, 0);
                xTimerStart(xRequestSlotTimer, 0);
                vTimerSetTimerID(xRequestSlotTimer, (void*)0);
                // Create Request Slot Message and send it:
                RequestSlotMessage message1;
                message1.sourceAdress = selfInf.SelfAdress;
                message1.destinationAdress = freeSlotMessage.sourceAdress;
                message1.messageSize = REQUEST_SLOT_SIZE;
                message1.NumberOfHops = 0;
                message1.messageType = REQUEST_SLOT;
                message1.slot = freeSlotMessage.slot;
                WriteRequestSlotMessage(WriteData, &message1);
                messageptr.length = message1.messageSize;
                messageptr.messagePtr = WriteData;
                RequestSpamNum = 0;
                xQueueSend(xQueueUART6Send, &messageptr, portMAX_DELAY);
                break;
            case SEND_REQUEST_MESSAGE:
                RequestSpamNum++;
                if(RequestSpamNum < MAX_REQUEST_SPAM)
                    xQueueSend(xQueueUART6Send, &messageptr, portMAX_DELAY);
                else if(RequestSpamNum == MAX_REQUEST_SPAM){
                    RequestSpamNum = 0;
                    slotInf[selfInf.slot].isSlotAvailable = false;
                    slotInf[selfInf.slot].isSlotSetup = false;
                    if (slotInf[0].isSlotAvailable == false && slotInf[0].isSlotSetup == false){ 
                        isTimerSet = false;
                        xTimerStop(xSlotTimer, 0);
                        slotInf[0].isSlotAvailable = true;
                        slotInf[0].isSlotSetup = false;
                    }
                }
                break;
            case RECEIVE_ACK_SUCCESS_MESSAGE:
                xTimerStop(xRequestSlotTimer, 0);
                slotInf[selfInf.slot].isSlotSetup = true;
                slotInf[selfInf.slot].isSlotAvailable = false;
                if(currentLocalNodeNumber < MAX_NEIGHBOUR){
                    // Store neighbour node information:
                    neighbourInf[currentLocalNodeNumber].Address = ackRequestSlotSuccessMessage.sourceAdress;
                    neighbourInf[currentLocalNodeNumber].percentToGW = freeSlotMessage.percentToGW;
                    neighbourInf[currentLocalNodeNumber].timeToGW = freeSlotMessage.timeToGW;
                    neighbourInf[currentLocalNodeNumber].slot = selfInf.slot;
                    neighbourInf[currentLocalNodeNumber].updateFirst = false;
                    neighbourInf[currentLocalNodeNumber].r_Packet = 0;
                    neighbourInf[currentLocalNodeNumber].ackPacket = 0;
                    neighbourInf[currentLocalNodeNumber].sentPacket = 0;
                    neighbourInf[currentLocalNodeNumber].seqNum = 0;
                    currentLocalNodeNumber++;
                }
                // Update self information:
                selfInf.sendLocation = NeighbourLocation(neighbourInf, chooseSlotToForwardPacket(neighbourInf, currentLocalNodeNumber), 
                currentLocalNodeNumber);
                selfInf.PRR = calculatePRR(selfInf.totalSend, selfInf.totalSuccessSend, neighbourInf[selfInf.sendLocation]);
                break;
            case RECEIVE_ACK_FAIL_MESSAGE:
                xTimerStop(xRequestSlotTimer, 0);
                slotInf[selfInf.slot].isSlotAvailable = false;
                slotInf[selfInf.slot].isSlotSetup = false;
                if (slotInf[0].isSlotAvailable == false && slotInf[0].isSlotSetup == false){ 
                    isTimerSet = false;
                    xTimerStop(xSlotTimer, 0);
                    slotInf[0].isSlotAvailable = true;
                    slotInf[0].isSlotSetup = false;
                }
                break;
            case SEND_FREE_SLOT_MESSAGE:
                FreeSlotMessage message2;
                message2.sourceAdress = selfInf.SelfAdress;
                message2.destinationAdress.OUIBits[0] = 0x00;
                message2.destinationAdress.OUIBits[1] = 0x0A;
                message2.destinationAdress.OUIBits[2] = 0xAA;
                message2.destinationAdress.NICSpecificBits[0] = 0xFF;
                message2.destinationAdress.NICSpecificBits[1] = 0xFF;
                message2.destinationAdress.NICSpecificBits[2] = 0xFF;
                message2.messageSize = (uint16_t)FREE_SLOT_SIZE;
                message2.NumberOfHops = 0;
                message2.percentToGW = selfInf.PRR;
                message2.timeToGW.simtime = calculateTimeToGW(selfInf.slot, neighbourInf[selfInf.sendLocation]);
                message2.messageType = FREE_SLOT;
                message2.slot = selfInf.slot;
                WriteFreeSlotMessage(WriteData, &message2);
                messageptr.messagePtr = WriteData;
                messageptr.length = message2.messageSize;
                xQueueSend(xQueueUART6Send, &messageptr, portMAX_DELAY);
                break;
            case RECEIVE_REQUEST_MESSAGE:
                if(slotInf[requestSlotMessage.slot].isSlotAvailable == true && slotInf[requestSlotMessage.slot].isSlotSetup == false){
                    neighbourInf[currentLocalNodeNumber].Address = requestSlotMessage.sourceAdress;
                    neighbourInf[currentLocalNodeNumber].percentToGW = 0;
                    neighbourInf[currentLocalNodeNumber].timeToGW.simtime = 1000000;
                    neighbourInf[currentLocalNodeNumber].slot = selfInf.slot;
                    neighbourInf[currentLocalNodeNumber].updateFirst = true;
                    neighbourInf[currentLocalNodeNumber].r_Packet = 0;
                    neighbourInf[currentLocalNodeNumber].ackPacket = 0;
                    neighbourInf[currentLocalNodeNumber].sentPacket = 0;
                    neighbourInf[currentLocalNodeNumber].seqNum = 0;
                    currentLocalNodeNumber++;
                    slotInf[requestSlotMessage.slot].isSlotAvailable = false;
                    slotInf[requestSlotMessage.slot].isSlotSetup = true;
                    xTaskNotify(TaskSetupCommunication, 0, eNoAction);
                    SetCom_NotifyNum = SEND_ACK_SUCCESS_MESSAGE;
                }
                else{
                    xTaskNotify(TaskSetupCommunication, 0, eNoAction);
                    SetCom_NotifyNum = SEND_ACK_FAIL_MESSAGE;
                }
                break;
            case SEND_ACK_SUCCESS_MESSAGE:
                AckRequestSlotSuccessMessage message3;
                message3.sourceAdress = selfInf.SelfAdress;
                message3.destinationAdress = requestSlotMessage.sourceAdress;
                message3.messageSize = (uint16_t)ACK_REQUEST_SLOT_SUCCESS_SIZE;
                message3.NumberOfHops = 0;
                message3.messageType = ACK_REQUEST_SLOT_SUCCESS;
                message3.startTimeSlot = 0;
                message3.slot = requestSlotMessage.slot;
                WriteAckRequestSlotSuccessMessage(WriteData, &message3);
                messageptr.messagePtr = WriteData;
                messageptr.length = message3.messageSize;
                xQueueSend(xQueueUART6Send, &messageptr, portMAX_DELAY);
                break;
            case SEND_ACK_FAIL_MESSAGE:
                AckRequestSlotFailMessage message4;
                message4.sourceAdress = selfInf.SelfAdress;
                message4.destinationAdress = requestSlotMessage.sourceAdress;
                message4.messageSize = (uint16_t)ACK_REQUEST_SLOT_FAIL_SIZE;
                message4.NumberOfHops = 0;
                message4.messageType = ACK_REQUEST_SLOT_FAIL;
                message4.slot = requestSlotMessage.slot;
                WriteAckRequestSlotFailMessage(WriteData, &message4);
                messageptr.messagePtr = WriteData;
                messageptr.length = message4.messageSize;
                xQueueSend(xQueueUART6Send, &messageptr, portMAX_DELAY);
                break;
            default:
                break;
        }
    }
}

// Task Send or Receive Data Handler
// This task will handle the data sending and receiving process
void Task_Send_or_ReceiveData_Handler(void *pvParameters){
    //uint8_t DebugData[50];
    UART_Inf messageptr;
    uint32_t SendData_NotifyNum = 0;
    ForwardPacketMessage message;
    AckPacketToGWMessage message1;
    AckPacketFailToGWMessage message2;
    bool isEnqueueSuccess = false;
    int location = 0;
    while (1)
    {
        xTaskNotifyWait(0, 0, &SendData_NotifyNum, portMAX_DELAY);
        //sprintf((char*)DebugData, "Task Send or Receive Data: %d, Packets: %d\n", SendData_NotifyNum, selfInf.HoldingPackets);
        switch (SendData_NotifyNum){
            case SEND_DATA_MESSAGE:
                if (selfInf.HoldingPackets > 0){
                    message.sourceAdress = selfInf.SelfAdress;
                    message.destinationAdress = neighbourInf[selfInf.sendLocation].Address;
                    message.messageSize = (uint16_t)FORWARD_PACKET_SIZE;
                    message.NumberOfHops = 0;
                    message.messageType = FORWARD_PACKET;
                    message.slot = selfInf.slot;
                    message.startNodeAddress = dataBuffer[selfInf.HoldingPackets - 1].startNodeAdress;
                    for(uint32_t i = 0; i < DATA_SIZE; i++)
                        message.data[i] = dataBuffer[selfInf.HoldingPackets - 1].data[i];
                    message.seqNum = neighbourInf[selfInf.sendLocation].seqNum;
                    WriteForwardPacketMessage(WriteData, &message);
                    messageptr.messagePtr = WriteData;
                    messageptr.length = message.messageSize;
                    xQueueSend(xQueueUART6Send, &messageptr, portMAX_DELAY);
                    xTimerReset(xDataSendTimeOut, 0);
                    xTimerStart(xDataSendTimeOut, 0);
                    neighbourInf[selfInf.sendLocation].sentPacket++;
                    selfInf.totalSend++;
                }
                break;
            case RECEIVE_DATA_MESSAGE:
                location = NeighbourLocation(neighbourInf, selfInf.slot, currentLocalNodeNumber);
                isEnqueueSuccess = enqueueDataBuffer(forwardPacketMessage, selfInf.HoldingPackets, dataBuffer);
                neighbourInf[location].seqNum = forwardPacketMessage.seqNum;
                if(isEnqueueSuccess == true || DEVICE_TYPE == GATEWAY){
                    xTaskNotify(TaskSend_or_ReceiveData, SEND_DACK_SUCCESS_MESSAGE, eSetValueWithOverwrite);
                    if(DEVICE_TYPE == GATEWAY) updateDisplayBuffer(forwardPacketMessage, selfInf.NumberOfNodeToDisplay, DisplayBuffer);
                }
                else{
                    xTaskNotify(TaskSend_or_ReceiveData, SEND_DACK_FAIL_MESSAGE, eSetValueWithOverwrite);
                }
                neighbourInf[location].r_Packet++;
                break;
            case SEND_DACK_SUCCESS_MESSAGE:
                message1.sourceAdress = selfInf.SelfAdress;
                message1.destinationAdress = forwardPacketMessage.sourceAdress;
                message1.messageSize = (uint16_t)ACK_PACKET_TO_GW_SIZE;
                message1.NumberOfHops = 0;
                message1.messageType = ACK_PACKET_TO_GW;
                message1.slot = selfInf.slot;
                message1.seqNum = neighbourInf[location].seqNum;
                WriteAckPacketToGWMessage(WriteData, &message1);
                messageptr.messagePtr = WriteData;
                messageptr.length = message1.messageSize;
                xQueueSend(xQueueUART6Send, &messageptr, portMAX_DELAY);
                break;
            case SEND_DACK_FAIL_MESSAGE:
                message2.sourceAdress = selfInf.SelfAdress;
                message2.destinationAdress = forwardPacketMessage.sourceAdress;
                message2.messageSize = (uint16_t)ACK_PACKET_FAIL_TO_GW_SIZE;
                message2.NumberOfHops = 0;
                message2.messageType = ACK_PACKET_FAIL_TO_GW;
                message2.percentToGW = selfInf.PRR;
                message2.timeToGW.simtime = calculateTimeToGW(selfInf.slot, neighbourInf[selfInf.sendLocation]);
                message2.seqNum = neighbourInf[location].seqNum;
                WriteAckPacketFailToGWMessage(WriteData, &message2);
                messageptr.messagePtr = WriteData;
                messageptr.length = message2.messageSize;
                xQueueSend(xQueueUART6Send, &messageptr, portMAX_DELAY);
                break;
            case RECEIVE_DACK_SUCCESS_MESSAGE:
                neighbourInf[selfInf.sendLocation].ackPacket++;
                neighbourInf[selfInf.sendLocation].seqNum++;
                dequeueDataBuffer(selfInf.HoldingPackets, dataBuffer);
                selfInf.totalSuccessSend++;
                if (isMultiSending == true && selfInf.HoldingPackets > 0){
                    xTaskNotify(TaskSend_or_ReceiveData, SEND_DATA_MESSAGE, eSetValueWithOverwrite);
                }
                xTimerStop(xDataSendTimeOut, 0);
                break;
            case RECEIVE_DACK_FAIL_MESSAGE:
                selfInf.PRR = calculatePRR(selfInf.totalSend, selfInf.totalSuccessSend, neighbourInf[selfInf.sendLocation]);
                isMultiSending = false;
                neighbourInf[selfInf.sendLocation].ackPacket++;
                xTimerStop(xDataSendTimeOut, 0);
                xTimerStop(xStopSendDataTimer, 0);
                break;
            case ACK_TIMEOUT:
                selfInf.PRR = calculatePRR(selfInf.totalSend, selfInf.totalSuccessSend, neighbourInf[selfInf.sendLocation]);
                if (isMultiSending == true && selfInf.HoldingPackets > 0){
                    xTaskNotify(TaskSend_or_ReceiveData, SEND_DATA_MESSAGE, eSetValueWithOverwrite);
                }
                break;
            default:
                break;
        }
    }
}

void Task_Get_Data_Handler(void *pvParameters){
    uint8_t DebugData[50];
    UART_Inf messageptr;
    DataPacket dataGet;
    while (1)
    {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
        //sprintf((char*)DebugData, "Task Get Data\n");
        //HAL_UART_Transmit(&huart1, DebugData, strlen((char*)DebugData), 100);
        HAL_ADC_Stop_DMA(&hadc1);
        dataGet.startNodeAdress = selfInf.SelfAdress;
        for (uint8_t i = 0; i < 5; i++){
            memcpy(&dataGet.data[i*2], (uint8_t*)&data_adc[i], 2);
        }
        sprintf((char*)DebugData, "1: 0x%04X, 2: 0x%04X, 3: 0x%04X, 4: 0x%04X, 5: 0x%04X\n",
        data_adc[0], data_adc[1], data_adc[2], data_adc[3], data_adc[4]);
        HAL_UART_Transmit(&huart1, DebugData, strlen((char*)DebugData), 100);
        enqueueDataBuffer(dataGet, selfInf.HoldingPackets, dataBuffer);
        HAL_ADC_Start_DMA(&hadc1,(uint32_t *)data_adc,5);
        
    }
}

// Task Display Data Handler:
void Task_Display_Data_Handler(void *pvParameters){
    uint8_t DebugData[50];
    UART_Inf messageptr;
    while (1)
    {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
        for (uint8_t i = 0; i < selfInf.NumberOfNodeToDisplay; i++){
            sprintf((char*)DebugData, "Node %d: \n", DisplayBuffer[i].startNodeAdress.NICSpecificBits[2]);
            HAL_UART_Transmit(&huart1, DebugData, strlen((char*)DebugData), 100);
            for (uint8_t j = 0; j < 5; j++){
           sprintf((char*)DebugData, "Data From SenSor Device %d: 0x%04X\n", j + 1, (DisplayBuffer[i].data[j*2 + 1] << 8) | DisplayBuffer[i].data[j*2]);
            HAL_UART_Transmit(&huart1, DebugData, strlen((char*)DebugData), 100);
            }
        }
    }
}
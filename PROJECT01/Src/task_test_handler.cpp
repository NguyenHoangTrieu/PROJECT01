#include "task_handler.hpp"
// Task Test Send Handler
void Task_Test_Send_Handler(void* pvParameters)
{
    uint8_t *data;
    UART_Inf messageptr;
    static uint8_t Message[MAX_MESSAGE_SIZE];
    bool check_msg = false;
    while (1)
    {
        xTaskNotifyWait(0, 0, (uint32_t*)&data, portMAX_DELAY);
        if(strcmp((char*)data, "01") == 0){
            FreeSlotMessage message;
            message.sourceAdress.OUIBits[0] = 0x00;
            message.sourceAdress.OUIBits[1] = 0x0A;
            message.sourceAdress.OUIBits[2] = 0xAA;
            message.sourceAdress.NICSpecificBits[0] = 0x00;
            message.sourceAdress.NICSpecificBits[1] = 0x00;
            message.sourceAdress.NICSpecificBits[2] = 0x01;
            message.destinationAdress.OUIBits[0] = 0x00;
            message.destinationAdress.OUIBits[1] = 0x0A;
            message.destinationAdress.OUIBits[2] = 0xAA;
            message.destinationAdress.NICSpecificBits[0] = 0x00;
            message.destinationAdress.NICSpecificBits[1] = 0x00;
            message.destinationAdress.NICSpecificBits[2] = 0x02;
            message.messageSize = (uint16_t)FREE_SLOT_SIZE;
            message.NumberOfHops = 1;
            message.checkSum = 0b11000011;
            message.percentToGW = 0.8;
            message.timeToGW.simtime = 5;
            message.messageType = FREE_SLOT;
            message.slot = 1;
            messageptr.length = message.messageSize;
            WriteFreeSlotMessage(Message, &message);
            check_msg = true;
        }
        else if(strcmp((char*)data, "02") == 0){
            RequestSlotMessage message;
            message.sourceAdress.OUIBits[0] = 0x00;
            message.sourceAdress.OUIBits[1] = 0x0A;
            message.sourceAdress.OUIBits[2] = 0xAA;
            message.sourceAdress.NICSpecificBits[0] = 0x00;
            message.sourceAdress.NICSpecificBits[1] = 0x00;
            message.sourceAdress.NICSpecificBits[2] = 0x01;
            message.destinationAdress.OUIBits[0] = 0x00;
            message.destinationAdress.OUIBits[1] = 0x0A;
            message.destinationAdress.OUIBits[2] = 0xAA;
            message.destinationAdress.NICSpecificBits[0] = 0x00;
            message.destinationAdress.NICSpecificBits[1] = 0x00;
            message.destinationAdress.NICSpecificBits[2] = 0x02;
            message.messageSize = (uint16_t)REQUEST_SLOT_SIZE;
            message.NumberOfHops = 0;
            message.checkSum = 0b11000011;
            message.messageType = REQUEST_SLOT;
            message.slot = 1;
            messageptr.length = message.messageSize;
            WriteRequestSlotMessage(Message, &message);
            check_msg = true;
        }
        else if(strcmp((char*)data, "03") == 0){
            AckRequestSlotSuccessMessage message;
            message.sourceAdress.OUIBits[0] = 0x00;
            message.sourceAdress.OUIBits[1] = 0x0A;
            message.sourceAdress.OUIBits[2] = 0xAA;
            message.sourceAdress.NICSpecificBits[0] = 0x00;
            message.sourceAdress.NICSpecificBits[1] = 0x00;
            message.sourceAdress.NICSpecificBits[2] = 0x01;
            message.destinationAdress.OUIBits[0] = 0x00;
            message.destinationAdress.OUIBits[1] = 0x0A;
            message.destinationAdress.OUIBits[2] = 0xAA;
            message.destinationAdress.NICSpecificBits[0] = 0x00;
            message.destinationAdress.NICSpecificBits[1] = 0x00;
            message.destinationAdress.NICSpecificBits[2] = 0x02;
            message.messageSize = (uint16_t)ACK_REQUEST_SLOT_SUCCESS_SIZE;  
            message.NumberOfHops = 0;
            message.checkSum = 0b11000011;
            message.messageType = ACK_REQUEST_SLOT_SUCCESS;
            message.startTimeSlot = 0.0001;
            message.slot = 1;
            messageptr.length = message.messageSize;
            WriteAckRequestSlotSuccessMessage(Message, &message);
            check_msg = true;
        }
        else if(strcmp((char*)data, "04") == 0){
            AckRequestSlotFailMessage message;
            message.sourceAdress.OUIBits[0] = 0x00;
            message.sourceAdress.OUIBits[1] = 0x0A;
            message.sourceAdress.OUIBits[2] = 0xAA;
            message.sourceAdress.NICSpecificBits[0] = 0x00;
            message.sourceAdress.NICSpecificBits[1] = 0x00;
            message.sourceAdress.NICSpecificBits[2] = 0x01;
            message.destinationAdress.OUIBits[0] = 0x00;
            message.destinationAdress.OUIBits[1] = 0x0A;
            message.destinationAdress.OUIBits[2] = 0xAA;
            message.destinationAdress.NICSpecificBits[0] = 0x00;
            message.destinationAdress.NICSpecificBits[1] = 0x00;
            message.destinationAdress.NICSpecificBits[2] = 0x02;
            message.messageSize = (uint16_t)ACK_REQUEST_SLOT_FAIL_SIZE;
            message.NumberOfHops = 0;
            message.checkSum = 0b11000011;
            message.messageType = ACK_REQUEST_SLOT_FAIL;
            message.slot = 1;
            messageptr.length = message.messageSize;
            WriteAckRequestSlotFailMessage(Message, &message);
            check_msg = true;
        }
        else if(strcmp((char*)data, "06") == 0){
            ForwardPacketMessage message;
            message.sourceAdress.OUIBits[0] = 0x00;
            message.sourceAdress.OUIBits[1] = 0x0A;
            message.sourceAdress.OUIBits[2] = 0xAA;
            message.sourceAdress.NICSpecificBits[0] = 0x00;
            message.sourceAdress.NICSpecificBits[1] = 0x00;
            message.sourceAdress.NICSpecificBits[2] = 0x01;
            message.destinationAdress.OUIBits[0] = 0x00;
            message.destinationAdress.OUIBits[1] = 0x0A;
            message.destinationAdress.OUIBits[2] = 0xAA;
            message.destinationAdress.NICSpecificBits[0] = 0x00;
            message.destinationAdress.NICSpecificBits[1] = 0x00;
            message.destinationAdress.NICSpecificBits[2] = 0x02;
            message.messageSize = FORWARD_PACKET_SIZE;
            message.NumberOfHops = 0;
            message.checkSum = 0b11000011;
            message.messageType = FORWARD_PACKET;
            message.slot = 1;
            for (int i = 0; i < DATA_SIZE; i++) message.data[i] = 0x01 + i;
            message.seqNum = 25;
            messageptr.length = message.messageSize;
            WriteForwardPacketMessage(Message, &message);
            check_msg = true;
        }
        else if(strcmp((char*)data, "07") == 0){
            AckPacketToGWMessage message;
            message.sourceAdress.OUIBits[0] = 0x00;
            message.sourceAdress.OUIBits[1] = 0x0A;
            message.sourceAdress.OUIBits[2] = 0xAA;
            message.sourceAdress.NICSpecificBits[0] = 0x00;
            message.sourceAdress.NICSpecificBits[1] = 0x00;
            message.sourceAdress.NICSpecificBits[2] = 0x01;
            message.destinationAdress.OUIBits[0] = 0x00;
            message.destinationAdress.OUIBits[1] = 0x0A;
            message.destinationAdress.OUIBits[2] = 0xAA;
            message.destinationAdress.NICSpecificBits[0] = 0x00;
            message.destinationAdress.NICSpecificBits[1] = 0x00;
            message.destinationAdress.NICSpecificBits[2] = 0x02;
            message.messageSize = (uint16_t)ACK_PACKET_TO_GW_SIZE;
            message.NumberOfHops = 0;
            message.checkSum = 0b11000011;
            message.messageType = ACK_PACKET_TO_GW;
            message.slot = 1;
            message.seqNum = 1;
            messageptr.length = message.messageSize;
            WriteAckPacketToGWMessage(Message, &message);
            check_msg = true;
        }
        else if(strcmp((char*)data, "08") == 0){
            AckPacketFailToGWMessage message;
            message.sourceAdress.OUIBits[0] = 0x00;
            message.sourceAdress.OUIBits[1] = 0x0A;
            message.sourceAdress.OUIBits[2] = 0xAA;
            message.sourceAdress.NICSpecificBits[0] = 0x00;
            message.sourceAdress.NICSpecificBits[1] = 0x00;
            message.sourceAdress.NICSpecificBits[2] = 0x01;
            message.destinationAdress.OUIBits[0] = 0x00;
            message.destinationAdress.OUIBits[1] = 0x0A;
            message.destinationAdress.OUIBits[2] = 0xAA;
            message.destinationAdress.NICSpecificBits[0] = 0x00;
            message.destinationAdress.NICSpecificBits[1] = 0x00;
            message.destinationAdress.NICSpecificBits[2] = 0x02;
            message.messageSize = (uint16_t)ACK_PACKET_FAIL_TO_GW_SIZE;
            message.NumberOfHops = 0;
            message.checkSum = 0b11000011;
            message.messageType = ACK_PACKET_FAIL_TO_GW;
            message.percentToGW = 0.97;
            message.timeToGW.simtime = 0.0001;
            message.seqNum = 1;
            messageptr.length = message.messageSize;
            WriteAckPacketFailToGWMessage(Message, &message);
            check_msg = true;
        }
        if(check_msg){
            Operation_MODE_Init(GPIOC, GPIOC, GPIO_PIN_8, GPIO_PIN_9, GENERAL_MODE);
            messageptr.messagePtr = Message;
            xQueueSend(xQueueUART6Send, &messageptr, portMAX_DELAY);
            check_msg = false;
        }
    }
}
// Task Test Receive Handler
void Task_Test_Receive_Handler(void* pvParameters)
{
    MessageType type;
    char *buffer = " CHECK ";
    char data[200];
    UART_Inf msg;
    while (1)
    {
        xTaskNotifyWait(0, 0, (uint32_t*)&type, portMAX_DELAY);
        switch (type)
        {
            case FREE_SLOT:
                sprintf(data, 
                    "FreeSlotMessage:\n"
                    "  Source MAC: %02X:%02X:%02X:%02X:%02X:%02X\n"
                    "  Destination MAC: %02X:%02X:%02X:%02X:%02X:%02X\n"
                    "  Percent to GW: %2d\n"
                    "  Time to GW: %2d\n"
                    "  Slot: %2d\n",
                    freeSlotMessage.sourceAdress.OUIBits[0], freeSlotMessage.sourceAdress.OUIBits[1], freeSlotMessage.sourceAdress.OUIBits[2],
                    freeSlotMessage.sourceAdress.NICSpecificBits[0], freeSlotMessage.sourceAdress.NICSpecificBits[1], freeSlotMessage.sourceAdress.NICSpecificBits[2],
                    freeSlotMessage.destinationAdress.OUIBits[0], freeSlotMessage.destinationAdress.OUIBits[1], freeSlotMessage.destinationAdress.OUIBits[2],
                    freeSlotMessage.destinationAdress.NICSpecificBits[0], freeSlotMessage.destinationAdress.NICSpecificBits[1], freeSlotMessage.destinationAdress.NICSpecificBits[2],
                    (int)freeSlotMessage.percentToGW,
                    (int)freeSlotMessage.timeToGW.simtime,
                    freeSlotMessage.slot
                );
                break;
            case REQUEST_SLOT:
                sprintf(data, 
                    "RequestSlotMessage:\n"
                    "  Source MAC: %02X:%02X:%02X:%02X:%02X:%02X\n"
                    "  Destination MAC: %02X:%02X:%02X:%02X:%02X:%02X\n"
                    "  Slot: %2d\n",
                    requestSlotMessage.sourceAdress.OUIBits[0], requestSlotMessage.sourceAdress.OUIBits[1], requestSlotMessage.sourceAdress.OUIBits[2],
                    requestSlotMessage.sourceAdress.NICSpecificBits[0], requestSlotMessage.sourceAdress.NICSpecificBits[1], requestSlotMessage.sourceAdress.NICSpecificBits[2],
                    requestSlotMessage.destinationAdress.OUIBits[0], requestSlotMessage.destinationAdress.OUIBits[1], requestSlotMessage.destinationAdress.OUIBits[2],
                    requestSlotMessage.destinationAdress.NICSpecificBits[0], requestSlotMessage.destinationAdress.NICSpecificBits[1], requestSlotMessage.destinationAdress.NICSpecificBits[2],
                    requestSlotMessage.slot
                );
                break;
            case ACK_REQUEST_SLOT_SUCCESS:
                sprintf(data, 
                    "AckRequestSlotSuccessMessage:\n"
                    "  Source MAC: %02X:%02X:%02X:%02X:%02X:%02X\n"
                    "  Destination MAC: %02X:%02X:%02X:%02X:%02X:%02X\n"
                    "  Slot: %2d\n",
                    ackRequestSlotSuccessMessage.sourceAdress.OUIBits[0], ackRequestSlotSuccessMessage.sourceAdress.OUIBits[1], ackRequestSlotSuccessMessage.sourceAdress.OUIBits[2],
                    ackRequestSlotSuccessMessage.sourceAdress.NICSpecificBits[0], ackRequestSlotSuccessMessage.sourceAdress.NICSpecificBits[1], ackRequestSlotSuccessMessage.sourceAdress.NICSpecificBits[2],
                    ackRequestSlotSuccessMessage.destinationAdress.OUIBits[0], ackRequestSlotSuccessMessage.destinationAdress.OUIBits[1], ackRequestSlotSuccessMessage.destinationAdress.OUIBits[2],
                    ackRequestSlotSuccessMessage.destinationAdress.NICSpecificBits[0], ackRequestSlotSuccessMessage.destinationAdress.NICSpecificBits[1], ackRequestSlotSuccessMessage.destinationAdress.NICSpecificBits[2],
                    ackRequestSlotSuccessMessage.slot
                );
                break;
            case ACK_REQUEST_SLOT_FAIL:
                sprintf(data, 
                    "AckRequestSlotFailMessage:\n"
                    "  Source MAC: %02X:%02X:%02X:%02X:%02X:%02X\n"
                    "  Destination MAC: %02X:%02X:%02X:%02X:%02X:%02X\n"
                    "  Slot: %2d\n",
                    ackRequestSlotFailMessage.sourceAdress.OUIBits[0], ackRequestSlotFailMessage.sourceAdress.OUIBits[1], ackRequestSlotFailMessage.sourceAdress.OUIBits[2],
                    ackRequestSlotFailMessage.sourceAdress.NICSpecificBits[0], ackRequestSlotFailMessage.sourceAdress.NICSpecificBits[1], ackRequestSlotFailMessage.sourceAdress.NICSpecificBits[2],
                    ackRequestSlotFailMessage.destinationAdress.OUIBits[0], ackRequestSlotFailMessage.destinationAdress.OUIBits[1], ackRequestSlotFailMessage.destinationAdress.OUIBits[2],
                    ackRequestSlotFailMessage.destinationAdress.NICSpecificBits[0], ackRequestSlotFailMessage.destinationAdress.NICSpecificBits[1], ackRequestSlotFailMessage.destinationAdress.NICSpecificBits[2],
                    ackRequestSlotFailMessage.slot
                );
                break;
            case FORWARD_PACKET:
                sprintf(data, 
                    "ForwardPacketMessage:\n"
                    "  Source MAC: %02X:%02X:%02X:%02X:%02X:%02X\n"
                    "  Destination MAC: %02X:%02X:%02X:%02X:%02X:%02X\n"
                    "  Slot: %2d\n"
                    " seqNum: %d\n",
                    forwardPacketMessage.sourceAdress.OUIBits[0], forwardPacketMessage.sourceAdress.OUIBits[1], forwardPacketMessage.sourceAdress.OUIBits[2],
                    forwardPacketMessage.sourceAdress.NICSpecificBits[0], forwardPacketMessage.sourceAdress.NICSpecificBits[1], forwardPacketMessage.sourceAdress.NICSpecificBits[2],
                    forwardPacketMessage.destinationAdress.OUIBits[0], forwardPacketMessage.destinationAdress.OUIBits[1], forwardPacketMessage.destinationAdress.OUIBits[2],
                    forwardPacketMessage.destinationAdress.NICSpecificBits[0], forwardPacketMessage.destinationAdress.NICSpecificBits[1], forwardPacketMessage.destinationAdress.NICSpecificBits[2],
                    forwardPacketMessage.slot,
                    forwardPacketMessage.seqNum
                );
                break;
            case ACK_PACKET_TO_GW:
                sprintf(data, 
                    "AckPacketToGWMessage:\n"
                    "  Source MAC: %02X:%02X:%02X:%02X:%02X:%02X\n"
                    "  Destination MAC: %02X:%02X:%02X:%02X:%02X:%02X\n"
                    "  Slot: %2d\n"
                    "  SeqNum: %d\n",
                    ackPacketToGWMessage.sourceAdress.OUIBits[0], ackPacketToGWMessage.sourceAdress.OUIBits[1], ackPacketToGWMessage.sourceAdress.OUIBits[2],
                    ackPacketToGWMessage.sourceAdress.NICSpecificBits[0], ackPacketToGWMessage.sourceAdress.NICSpecificBits[1], ackPacketToGWMessage.sourceAdress.NICSpecificBits[2],
                    ackPacketToGWMessage.destinationAdress.OUIBits[0], ackPacketToGWMessage.destinationAdress.OUIBits[1], ackPacketToGWMessage.destinationAdress.OUIBits[2],
                    ackPacketToGWMessage.destinationAdress.NICSpecificBits[0], ackPacketToGWMessage.destinationAdress.NICSpecificBits[1], ackPacketToGWMessage.destinationAdress.NICSpecificBits[2],
                    ackPacketToGWMessage.slot,
                    ackPacketToGWMessage.seqNum
                );
                break;
            case ACK_PACKET_FAIL_TO_GW:
                sprintf(data, 
                    "AckPacketFailToGWMessage:\n"
                    "  Source MAC: %02X:%02X:%02X:%02X:%02X:%02X\n"
                    "  Destination MAC: %02X:%02X:%02X:%02X:%02X:%02X\n"
                    "  Percent to GW: %2d\n"
                    "  Time to GW: %2d\n"
                    "  SeqNum: %d\n",
                    ackPacketFailToGWMessage.sourceAdress.OUIBits[0], ackPacketFailToGWMessage.sourceAdress.OUIBits[1], ackPacketFailToGWMessage.sourceAdress.OUIBits[2],
                    ackPacketFailToGWMessage.sourceAdress.NICSpecificBits[0], ackPacketFailToGWMessage.sourceAdress.NICSpecificBits[1], ackPacketFailToGWMessage.sourceAdress.NICSpecificBits[2],
                    ackPacketFailToGWMessage.destinationAdress.OUIBits[0], ackPacketFailToGWMessage.destinationAdress.OUIBits[1], ackPacketFailToGWMessage.destinationAdress.OUIBits[2],
                    ackPacketFailToGWMessage.destinationAdress.NICSpecificBits[0], ackPacketFailToGWMessage.destinationAdress.NICSpecificBits[1], ackPacketFailToGWMessage.destinationAdress.NICSpecificBits[2],
                    (int)ackPacketFailToGWMessage.percentToGW,
                    (int)ackPacketFailToGWMessage.timeToGW.simtime,
                    ackPacketFailToGWMessage.seqNum
                );
                break;
            default:
                break;
        }
        msg.messagePtr = (uint8_t*) data;
        msg.length = strlen(data);
        xQueueSend(xQueueUART1Send, &msg, 100);
    }
}

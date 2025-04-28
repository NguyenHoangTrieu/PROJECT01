#include "MessageReadWrite.hpp"
/* Read Header of Message from data buffer */
void ReadHeader(uint8_t *buffer, FullMessage *msg){
    memcpy(&msg->sourceAdress.OUIBits[0], &buffer[0], 3);
    memcpy(&msg->sourceAdress.NICSpecificBits[0], &buffer[3], 3);
    memcpy(&msg->destinationAdress.OUIBits[0], &buffer[6], 3);
    memcpy(&msg->destinationAdress.NICSpecificBits[0], &buffer[9], 3);
    memcpy(&msg->messageSize, &buffer[12], 2);
    memcpy(&msg->NumberOfHops, &buffer[14], 2);
    msg->checkSum = buffer[16];
    msg->messageType = (MessageType)buffer[17];
}

/* Read Free Slot Message Payload from data buffer */
void ReadFreeSlotMessage(uint8_t *buffer, FreeSlotMessage *message){
ReadHeader(buffer, message);
memcpy(&message->percentToGW, &buffer[18], 8);
memcpy(&message->timeToGW.simtime, &buffer[26], 8);
memcpy(&message->slot, &buffer[34], 4);
}

/* Read Request Slot Message Payload from data buffer */
void ReadRequestSlotMessage(uint8_t *buffer, RequestSlotMessage *message){
ReadHeader(buffer, message);
memcpy(&message->slot, &buffer[18], 4);
}

/* Read Ack Request Slot Success Message Payload from data buffer */
void ReadAckRequestSlotSuccessMessage(uint8_t *buffer, AckRequestSlotSuccessMessage *message){
ReadHeader(buffer, message);
memcpy(&message->startTimeSlot, &buffer[18], 8);
memcpy(&message->slot, &buffer[26], 4);
}

/* Read Ack Request Slot Fail Message Payload from data buffer */
void ReadAckRequestSlotFailMessage(uint8_t *buffer, AckRequestSlotFailMessage *message){
ReadHeader(buffer, message);
memcpy(&message->slot, &buffer[18], 4);
}

/* Read Update Heat Value Message Payload from data buffer */
void ReadUpdateHeatValueMessage(uint8_t *buffer, UpdateHeatValueMessage *message){
ReadHeader(buffer, message);
memcpy(&message->startSendTime, &buffer[18], 8);
memcpy(&message->percentToGW, &buffer[26], 8);
memcpy(&message->timeToGW.simtime, &buffer[34], 8);
memcpy(&message->slot, &buffer[42], 4);
}

/* Read Forward Packet Message Payload from data buffer */
void ReadForwardPacketMessage(uint8_t *buffer, ForwardPacketMessage *message){
ReadHeader(buffer, message);
memcpy(&message->slot, &buffer[18], 4);
memcpy(&message->startNodeAddress.OUIBits[0], &buffer[22], 3);
memcpy(&message->startNodeAddress.NICSpecificBits[0], &buffer[25], 3);
for(uint32_t i = 0; i < DATA_SIZE; i++)
    message->data[i] = buffer[28 + i];
memcpy(&message->seqNum, &buffer[28 + DATA_SIZE], 4);
}

/* Read Ack Packet To GW Message Payload from data buffer */
void ReadAckPacketToGWMessage(uint8_t *buffer, AckPacketToGWMessage *message){
ReadHeader(buffer, message);
memcpy(&message->slot, &buffer[18], 4);
memcpy(&message->seqNum, &buffer[22], 4);
}

/* Read Ack Packet Fail To GW Message Payload from data buffer */
void ReadAckPacketFailToGWMessage(uint8_t *buffer, AckPacketFailToGWMessage *message){
ReadHeader(buffer, message);
memcpy(&message->percentToGW, &buffer[18], 8);
memcpy(&message->timeToGW.simtime, &buffer[26], 8);
memcpy(&message->seqNum, &buffer[34], 4);
}

/* Read Ack Update Heat Value Message Payload from data buffer */
void ReadAckUpdateHeatValueMessage(uint8_t *buffer, AckUpdateHeatValueMessage *message){
ReadHeader(buffer, message);
memcpy(&message->slot, &buffer[18], 4);
memcpy(&message->percentToGW, &buffer[22], 8);
memcpy(&message->timeToGW.simtime, &buffer[30], 8);
}

/* Write Header of Message to data buffer */
void WriteHeader(uint8_t *buffer, FullMessage *msg){
memcpy(&buffer[0], &msg->sourceAdress.OUIBits[0], 3);
memcpy(&buffer[3], &msg->sourceAdress.NICSpecificBits[0], 3);
memcpy(&buffer[6], &msg->destinationAdress.OUIBits[0], 3);
memcpy(&buffer[9], &msg->destinationAdress.NICSpecificBits[0], 3);
memcpy(&buffer[12], &msg->messageSize, 2);
memcpy(&buffer[14], &msg->NumberOfHops, 2);
buffer[17] = msg->messageType;
}

/* Write Free Slot Message Payload to data buffer */
void WriteFreeSlotMessage(uint8_t *buffer, FreeSlotMessage *message){
WriteHeader(buffer, message);
memcpy(&buffer[18], &message->percentToGW, 8);
memcpy(&buffer[26], &message->timeToGW.simtime, 8);
memcpy(&buffer[34], &message->slot, 4);
message->checkSum = CheckSumCalculate(buffer, FREE_SLOT_SIZE);
buffer[16] = message->checkSum;
}

/* Write Request Slot Message Payload to data buffer */
void WriteRequestSlotMessage(uint8_t *buffer, RequestSlotMessage *message){
WriteHeader(buffer, message);
memcpy(&buffer[18], &message->slot, 4);
message->checkSum = CheckSumCalculate(buffer, REQUEST_SLOT_SIZE);
buffer[16] = message->checkSum;
}

/* Write Ack Request Slot Success Message Payload to data buffer */
void WriteAckRequestSlotSuccessMessage(uint8_t *buffer, AckRequestSlotSuccessMessage *message){
WriteHeader(buffer, message);
memcpy(&buffer[18], &message->startTimeSlot, 8);
memcpy(&buffer[26], &message->slot, 4);
message->checkSum = CheckSumCalculate(buffer, ACK_REQUEST_SLOT_SUCCESS_SIZE);
buffer[16] = message->checkSum;
}

/* Write Ack Request Slot Fail Message Payload to data buffer */
void WriteAckRequestSlotFailMessage(uint8_t *buffer, AckRequestSlotFailMessage *message){
WriteHeader(buffer, message);
memcpy(&buffer[18], &message->slot, 4);
message->checkSum = CheckSumCalculate(buffer, ACK_REQUEST_SLOT_FAIL_SIZE);
buffer[16] = message->checkSum;
}

/* Write Update Heat Value Message Payload to data buffer */
void WriteUpdateHeatValueMessage(uint8_t *buffer, UpdateHeatValueMessage *message){
WriteHeader(buffer, message);
memcpy(&buffer[18], &message->startSendTime, 8);
memcpy(&buffer[26], &message->percentToGW, 8);
memcpy(&buffer[34], &message->timeToGW.simtime, 8);
memcpy(&buffer[42], &message->slot, 4);
message->checkSum = CheckSumCalculate(buffer, UPDATE_HEAT_VALUE_SIZE);
buffer[16] = message->checkSum;
}

/* Write Forward Packet Message Payload to data buffer */
void WriteForwardPacketMessage(uint8_t *buffer, ForwardPacketMessage *message){
WriteHeader(buffer, message);
memcpy(&buffer[18], &message->slot, 4);
memcpy(&buffer[22], &message->startNodeAddress.OUIBits[0], 3);
memcpy(&buffer[25], &message->startNodeAddress.NICSpecificBits[0], 3);
for(uint32_t i = 0; i < DATA_SIZE; i++)
    buffer[28 + i] = message->data[i];
memcpy(&buffer[28 + DATA_SIZE], &message->seqNum, 4);
message->checkSum = CheckSumCalculate(buffer, FORWARD_PACKET_SIZE);
buffer[16] = message->checkSum;
}

/* Write Ack Packet To GW Message Payload to data buffer */
void WriteAckPacketToGWMessage(uint8_t *buffer, AckPacketToGWMessage *message){
WriteHeader(buffer, message);
memcpy(&buffer[18], &message->slot, 4);
memcpy(&buffer[22], &message->seqNum, 4);
message->checkSum = CheckSumCalculate(buffer, ACK_PACKET_TO_GW_SIZE);
buffer[16] = message->checkSum;
}

/* Write Ack Packet Fail To GW Message Payload to data buffer */
void WriteAckPacketFailToGWMessage(uint8_t *buffer, AckPacketFailToGWMessage *message){
WriteHeader(buffer, message);
memcpy(&buffer[18], &message->percentToGW, 8);
memcpy(&buffer[26], &message->timeToGW.simtime, 8);
memcpy(&buffer[34], &message->seqNum, 4);
message->checkSum = CheckSumCalculate(buffer, ACK_PACKET_FAIL_TO_GW_SIZE);
buffer[16] = message->checkSum;
}
/* Write Ack Update Heat Value Message Payload to data buffer */
void WriteAckUpdateHeatValueMessage(uint8_t *buffer, AckUpdateHeatValueMessage *message){
WriteHeader(buffer, message);
memcpy(&buffer[18], &message->slot, 4);
memcpy(&buffer[22], &message->percentToGW, 8);
memcpy(&buffer[30], &message->timeToGW.simtime, 8);
message->checkSum = CheckSumCalculate(buffer, ACK_UPDATE_HEAT_VALUE_SIZE);
buffer[16] = message->checkSum;
}
/* Calculate CheckSum */
uint8_t CheckSumCalculate(uint8_t *buffer, uint16_t len){
    uint16_t checkSum = 0;
    for (int i = 0; i < len; i++){
        if(i != 16) checkSum += buffer[i];
    }
    return checkSum & 0xFF;
}
#ifndef MESSAGEREADWRITE_H
#define MESSAGEREADWRITE_H
#include "protocolNaming.hpp"
uint8_t CheckSumCalculate(uint8_t *buffer, uint16_t len);
/*Read function:*/
void ReadHeader(uint8_t *buffer, FullMessage *msg);
void ReadFreeSlotMessage(uint8_t *buffer, FreeSlotMessage *message);
void ReadRequestSlotMessage(uint8_t *buffer, RequestSlotMessage *message);
void ReadAckRequestSlotSuccessMessage(uint8_t *buffer, AckRequestSlotSuccessMessage *message);
void ReadAckRequestSlotFailMessage(uint8_t *buffer, AckRequestSlotFailMessage *message);
void ReadUpdateHeatValueMessage(uint8_t *buffer, UpdateHeatValueMessage *message);
void ReadForwardPacketMessage(uint8_t *buffer, ForwardPacketMessage *message);
void ReadAckPacketToGWMessage(uint8_t *buffer, AckPacketToGWMessage *message);
void ReadAckPacketFailToGWMessage(uint8_t *buffer, AckPacketFailToGWMessage *message);
void ReadAckUpdateHeatValueMessage(uint8_t *buffer, AckUpdateHeatValueMessage *message);
/*Write function:*/
void WriteHeader(uint8_t *buffer, FullMessage *msg);
void WriteFreeSlotMessage(uint8_t *buffer, FreeSlotMessage *message);
void WriteRequestSlotMessage(uint8_t *buffer, RequestSlotMessage *message);
void WriteAckRequestSlotSuccessMessage(uint8_t *buffer, AckRequestSlotSuccessMessage *message);
void WriteAckRequestSlotFailMessage(uint8_t *buffer, AckRequestSlotFailMessage *message);
void WriteUpdateHeatValueMessage(uint8_t *buffer, UpdateHeatValueMessage *message);
void WriteForwardPacketMessage(uint8_t *buffer, ForwardPacketMessage *message);
void WriteAckPacketToGWMessage(uint8_t *buffer, AckPacketToGWMessage *message);
void WriteAckPacketFailToGWMessage(uint8_t *buffer, AckPacketFailToGWMessage *message);
void WriteAckUpdateHeatValueMessage(uint8_t *buffer, AckUpdateHeatValueMessage *message);
#endif
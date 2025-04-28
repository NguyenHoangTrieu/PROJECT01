#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H
#include "main.h"
#include "protocolNaming.hpp"
#include "MessageReadWrite.hpp"
extern FreeSlotMessage freeSlotMessage;
extern RequestSlotMessage requestSlotMessage;  
extern AckRequestSlotSuccessMessage ackRequestSlotSuccessMessage;
extern AckRequestSlotFailMessage ackRequestSlotFailMessage;
extern UpdateHeatValueMessage updateHeatValueMessage;
extern ForwardPacketMessage forwardPacketMessage;
extern AckPacketToGWMessage ackPacketToGWMessage;
extern AckPacketFailToGWMessage ackPacketFailToGWMessage;
extern AckUpdateHeatValueMessage ackUpdateHeatValueMessage;
extern uint8_t ReadData[MAX_MESSAGE_SIZE];
extern uint8_t WriteData[MAX_MESSAGE_SIZE];
class Message {
public:
	Message(uint8_t *ReadData, uint16_t len);
	void execute();
private:
	uint16_t len;
	MessageType type;
	void(*callback)(void* param1, void* param2, uint16_t param3);
};
uint16_t UART_ReceiveMessage(uint8_t *buffer, uint16_t bufferSize, UART_HandleTypeDef huartx);
void ClearDataBuffer(uint8_t *buffer, uint16_t len);
bool AddressCompare(MACAddress *address1, MACAddress *address2);
#endif
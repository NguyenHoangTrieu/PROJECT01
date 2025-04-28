#include "MessageParser.hpp"
#include "task_handler.hpp"
/*Declare Message class*/
FreeSlotMessage freeSlotMessage;
RequestSlotMessage requestSlotMessage;  
AckRequestSlotSuccessMessage ackRequestSlotSuccessMessage;
AckRequestSlotFailMessage ackRequestSlotFailMessage;
UpdateHeatValueMessage updateHeatValueMessage;
ForwardPacketMessage forwardPacketMessage;
AckPacketToGWMessage ackPacketToGWMessage;
AckPacketFailToGWMessage ackPacketFailToGWMessage;
AckUpdateHeatValueMessage ackUpdateHeatValueMessage;
uint8_t ReadData[MAX_MESSAGE_SIZE];
uint8_t WriteData[MAX_MESSAGE_SIZE];
/*Constructor of Message class*/
Message::Message(uint8_t *ReadData, uint16_t len){
    this->callback = NULL;
    this->len = len;
    this->type = (MessageType)ReadData[17];
    switch(this->type){
        case FREE_SLOT:
            this->callback = [](void* _self, void* _buffer, uint16_t _len){
                if ( _len == FREE_SLOT_SIZE){
                    auto* data = static_cast<uint8_t*>(_buffer);
                    ReadFreeSlotMessage(data, &freeSlotMessage);
                    xTaskNotify(FreeSlotMessageTask, 0, eNoAction);
                    //xQueueSend(xQueueMessage, &messageCheckPtr1, 10);
                }
            };
        break;
        case REQUEST_SLOT:
            this->callback = [](void* _self, void* _buffer, uint16_t _len){
                if ( _len == REQUEST_SLOT_SIZE){
                    auto *data = static_cast<uint8_t*>(_buffer);
                    ReadRequestSlotMessage(data, &requestSlotMessage);
                    xTaskNotify(RequestSlotMessageTask, 0, eNoAction);
                    //xQueueSend(xQueueMessage, &messageCheckPtr2, 10);
                }
            };
        break;
        case ACK_REQUEST_SLOT_SUCCESS:
            this->callback = [](void* _self, void* _buffer, uint16_t _len){
                if ( _len == ACK_REQUEST_SLOT_SUCCESS_SIZE){
                    auto *data = static_cast<uint8_t*>(_buffer);
                    ReadAckRequestSlotSuccessMessage(data, &ackRequestSlotSuccessMessage);
                    xTaskNotify(AckRequestSlotSuccessMessageTask, 0, eNoAction);
                    //xQueueSend(xQueueMessage, &messageCheckPtr3, 10);
                }
            };
        break;
        case ACK_REQUEST_SLOT_FAIL:
            this->callback = [](void* _self, void* _buffer, uint16_t _len){
                if ( _len == ACK_REQUEST_SLOT_FAIL_SIZE){
                    auto *data = static_cast<uint8_t*>(_buffer);
                    ReadAckRequestSlotFailMessage(data, &ackRequestSlotFailMessage);
                    xTaskNotify(AckRequestSlotFailMessageTask, 0, eNoAction);
                    //xQueueSend(xQueueMessage, &messageCheckPtr4, 10);
                }
            };
        break;
        case UPDATE_HEAT_VALUE:
            this->callback = [](void* _self, void* _buffer, uint16_t _len){
                if (_len == UPDATE_HEAT_VALUE_SIZE){
                    auto *data = static_cast<uint8_t*>(_buffer);
                    ReadUpdateHeatValueMessage(data, &updateHeatValueMessage);
                    xTaskNotify(UpdateHeatValueMessageTask, 0, eNoAction);
                    //xQueueSend(xQueueMessage, &messageCheckPtr5, 10);
                }
            }; 
        break;
        case FORWARD_PACKET:
            this->callback = [](void* _self, void* _buffer, uint16_t _len){
                if ( _len <= FORWARD_PACKET_SIZE){
                    auto* data = static_cast<uint8_t*>(_buffer);
                    ReadForwardPacketMessage(data, &forwardPacketMessage);
                    xTaskNotify(ForwardPacketMessageTask, 0, eNoAction);
                    //xQueueSend(xQueueMessage, &messageCheckPtr6, 10);
                }
            };
        break;
        case ACK_PACKET_TO_GW:
            this->callback = [](void* _self, void* _buffer, uint16_t _len){
                if ( _len == ACK_PACKET_TO_GW_SIZE){
                    auto *data = static_cast<uint8_t*>(_buffer);
                    ReadAckPacketToGWMessage(data, &ackPacketToGWMessage);
                    xTaskNotify(AckPacketToGWMessageTask, 0, eNoAction);
                    //xQueueSend(xQueueMessage, &messageCheckPtr7, 10);
                    
                }
            };
        break;
        case ACK_PACKET_FAIL_TO_GW:
            this->callback = [](void* _self, void* _buffer, uint16_t _len){
                if ( _len == ACK_PACKET_FAIL_TO_GW_SIZE){
                    auto *data = static_cast<uint8_t*>(_buffer);
                    ReadAckPacketFailToGWMessage(data, &ackPacketFailToGWMessage);
                    xTaskNotify(AckPacketFailToGWMessageTask, 0, eNoAction);
                    //xQueueSend(xQueueMessage, &messageCheckPtr8, 10);
                }
            };
        case ACK_UPDATE_HEAT_VALUE:
            this->callback = [](void* _self, void* _buffer, uint16_t _len){
                if ( _len == ACK_UPDATE_HEAT_VALUE_SIZE){
                    auto *data = static_cast<uint8_t*>(_buffer);
                    ReadAckUpdateHeatValueMessage(data, &ackUpdateHeatValueMessage);
                    xTaskNotify(AckUpdateHeatValueMessageTask, 0, eNoAction);
                    //xQueueSend(xQueueMessage, &messageCheckPtr9, 10);
                }
            };
        break;
        default:
        break;
    }
}
void Message::execute() {
	if (this->callback != NULL) 
		this->callback((void*)this, (void*)ReadData, this->len);
}

/*Read UART buffer*/
uint16_t UART_ReceiveMessage(uint8_t *buffer, uint16_t bufferSize, UART_HandleTypeDef huartx){
    uint16_t count_byte = 1;
    while (HAL_UART_Receive(&huartx, &buffer[count_byte], 1, 1) == HAL_OK && count_byte < bufferSize - 1)
        count_byte++;
    buffer[count_byte] = '\0';
    return count_byte;
}

/* Clear Data Buffer */
void ClearDataBuffer(uint8_t *buffer, uint16_t len){
    for(uint16_t i = 0; i < len; i++)
        buffer[i] = 0;
}

/* Compare Address */
bool AddressCompare(MACAddress *address1, MACAddress *address2){
    if(address1->OUIBits[0] == address2->OUIBits[0] && address1->OUIBits[1] == address2->OUIBits[1] && 
        address1->OUIBits[2] == address2->OUIBits[2] && address1->NICSpecificBits[0] == address2->NICSpecificBits[0] && 
        address1->NICSpecificBits[1] == address2->NICSpecificBits[1] && address1->NICSpecificBits[2] == address2->NICSpecificBits[2])
        return true;
    return false;
}
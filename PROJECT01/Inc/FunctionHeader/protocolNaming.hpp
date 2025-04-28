#ifndef __PROTOCOLNAMING_H
#define __PROTOCOLNAMING_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define ULONG_MAX 0xffff
// define the protocol
#define DATA_SIZE                       10 //byte
#define HEADER_SIZE                     18
#define FREE_SLOT_SIZE                  HEADER_SIZE + 8 + 8 + 4
#define REQUEST_SLOT_SIZE               HEADER_SIZE + 4
#define ACK_REQUEST_SLOT_SUCCESS_SIZE   HEADER_SIZE + 8 + 4
#define ACK_REQUEST_SLOT_FAIL_SIZE      HEADER_SIZE + 4
#define UPDATE_HEAT_VALUE_SIZE          HEADER_SIZE + (8 + 8 + 8 + 4)
#define FORWARD_PACKET_SIZE             HEADER_SIZE + 4 + 6 + DATA_SIZE + 4
#define ACK_PACKET_TO_GW_SIZE           HEADER_SIZE + 4 + 4
#define ACK_PACKET_FAIL_TO_GW_SIZE      HEADER_SIZE + 8 + 8 + 4
#define ACK_UPDATE_HEAT_VALUE_SIZE      HEADER_SIZE + 4 + 8 + 8
#define MAX_MESSAGE_SIZE                UPDATE_HEAT_VALUE_SIZE + 10
#define MAX_NEIGHBOUR                   6
#define MAX_SLOT                        MAX_NEIGHBOUR//begin from slots 0 and multiply by 2 for reserve.
#define DATA_BUFFER_SIZE                20 // packets
#define DISPLAY_BUFFER_SIZE             10 // Nodes  
#define SLOT_TIME                       1000 // ms
#define REQUEST_SPAM_TIME               160 // ms
#define MAX_REQUEST_SPAM                3
#define UPDATE_TIME_OUT                 200 // ms
#define RESEND_TIME_OUT                 500 // ms
#define DATA_GET                        5000 // ms
#define SEND_TIME                       500 // ms
#define DATA_SEND_TIMEOUT               200
typedef uint8_t MessageType;
#define FREE_SLOT                      (MessageType)0x01
#define REQUEST_SLOT                   (MessageType)0x02
#define ACK_REQUEST_SLOT_SUCCESS       (MessageType)0x03
#define ACK_REQUEST_SLOT_FAIL          (MessageType)0x04
#define UPDATE_HEAT_VALUE              (MessageType)0x05
#define FORWARD_PACKET                 (MessageType)0x06
#define ACK_PACKET_TO_GW               (MessageType)0x07
#define ACK_PACKET_FAIL_TO_GW          (MessageType)0x08
#define ACK_UPDATE_HEAT_VALUE          (MessageType)0x09
// define the setup communication action
#define SEND_REQUEST_MESSAGE                0x01
#define SEND_ACK_SUCCESS_MESSAGE            0x02
#define SEND_ACK_FAIL_MESSAGE               0x03
#define SEND_FREE_SLOT_MESSAGE              0x04
#define RECEIVE_REQUEST_MESSAGE             0x05
#define RECEIVE_ACK_SUCCESS_MESSAGE         0x06
#define RECEIVE_ACK_FAIL_MESSAGE            0x07
#define RECEIVE_FREE_SLOT_MESSAGE           0x08
//define Update Heat Value action
#define SEND_UPDATE_HEAT_VALUE_MESSAGE      0x01
#define TIMEOUT_MESSAGE                     0x02
#define RECEIVE_ACK_UPDATE                  0x03
#define RECEIVE_UPDATE_HEAT_VALUE           0x04
//Define Send or Receive Data action
#define SEND_DATA_MESSAGE             0x01
#define RECEIVE_DATA_MESSAGE          0x02
#define SEND_DACK_SUCCESS_MESSAGE     0x03
#define SEND_DACK_FAIL_MESSAGE        0x04
#define RECEIVE_DACK_SUCCESS_MESSAGE  0x05
#define RECEIVE_DACK_FAIL_MESSAGE     0x06
#define ACK_TIMEOUT                   0x07
//enum and struct:
enum ErrorType{
    BUFFER_OVERFLOW = 0x00,
    INVALID_MESSAGE = 0x01
};
struct MACAddress
{
    uint8_t OUIBits[3];
    uint8_t NICSpecificBits[3];
};
struct simtime_t
{
    double simtime;
};
struct SlotInformation{
    bool isSlotSetup; // is slot setup for communication
    bool isSlotAvailable; // is slot available for setup communication
};
struct SelfInformation{
    MACAddress SelfAdress;
    double PRR;
    uint8_t slot;
    unsigned int NumberOfCycle;
    int sendLocation;
    unsigned int totalSend;
    int HoldingPackets; // total send data packet in queue
    unsigned int totalSuccessSend; // total send and success send data packet
    int NumberOfNodeToDisplay; // total node in network to display data
};
struct neighbourNode{
    MACAddress Address;
    MACAddress forwardAddress;
    double percentToGW; 
    simtime_t timeToGW;
    int slot;
    int r_Packet;
    bool updateFirst;
    int ackPacket;
    int sentPacket;
    uint32_t seqNum;
};
struct FullMessage{
    // Header
    struct MACAddress sourceAdress;
    struct MACAddress destinationAdress;
    uint16_t messageSize;
    uint16_t NumberOfHops;
    uint8_t checkSum;
    //Payload
    MessageType messageType;
};
struct MessageCheckPtr{
    FullMessage *message;
    uint16_t len;
    MessageType type;
};
/*FREE_SLOT (1): A broadcast packet that advertises available time slots,
informing neighboring nodes about unused time slots that the sending
node can allocate for communication.*/
struct FreeSlotMessage : FullMessage{
    double percentToGW;
    struct simtime_t timeToGW;
    int slot;
};
/*REQUEST_SLOT (2): A packet used to request a dedicated communication time slot after receiving a FREE_SLOT broadcast.*/
struct RequestSlotMessage : FullMessage{
    int slot;
};
/*ACK_REQUEST_SLOT_SUCCESS (3): A confirmation packet sent to acknowledge
the successful registration of a communication time slot with another node.*/
struct AckRequestSlotSuccessMessage : FullMessage{
    double startTimeSlot;
    int slot;
};
/*ACK_REQUEST_SLOT_FAIL (4): A rejection packet sent when a
previously advertised time slot has already been assigned, preventing registration by another node.*/
struct AckRequestSlotFailMessage : FullMessage{
    int slot;
};
/*UPDATE_HEAT_VALUE (5): A packet used to update the heat value of a node for all neighbour node.*/
struct UpdateHeatValueMessage : FullMessage{
    double startSendTime;
    double percentToGW;
    struct simtime_t timeToGW;
    int slot;
};
/*FORWARD_PACKET (6): A packet carrying data to the gateway, either
generated by the node itself or forwarded on behalf of another node.*/
struct ForwardPacketMessage : FullMessage{
    int slot;
    MACAddress startNodeAddress;
    uint8_t data[DATA_SIZE];
    uint32_t seqNum;

};
/*ACK_PACKET_TO_GW (7): A packet sent by the relay node to the sending node
to confirm receipt of a message being forwarded to the gateway.
Once the sending node receives this acknowledgment, it is no longer responsible for the message.*/
struct AckPacketToGWMessage : FullMessage{
    int slot;
    uint32_t seqNum;
};
/*ACK_PACKET_FAIL_TO_GW (7): A packet sent by the relay node to the sending node
to indicate that the message being forwarded to the gateway has failed to reach the gateway.*/
struct AckPacketFailToGWMessage : FullMessage{
    double percentToGW;
    struct simtime_t timeToGW;
    uint32_t seqNum;
};
/*ACK_UPDATE_HEAT_VALUE (9): A packet sent by the relay node to the sending node
to confirm receipt of the UPDATE_HEAT_VALUE message.*/
struct AckUpdateHeatValueMessage : FullMessage{
    int slot;
    double percentToGW;
    struct simtime_t timeToGW;
};

struct DataPacket{
    MACAddress startNodeAdress;
    uint8_t data[DATA_SIZE];
};
#endif

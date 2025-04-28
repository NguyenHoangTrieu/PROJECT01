#include <gtest/gtest.h>
#include <cstring>
#include <cstdint> 
#include "MessageReadWrite.hpp"
#include "Calculate_Function.hpp"
int currentLocalNodeNumber = 6;
neighbourNode neighbourInf[6];
// Test case for ReadHeader function
TEST(ReadTest, ReadHeaderTest){
    uint8_t buffer[18] = {
        0x00, 0x0A, 0xAA,  // source OUI
        0x04, 0x05, 0x06,  // source NIC
        0x00, 0x0A, 0xAA,  // dest OUI
        0x0A, 0x0B, 0x0C,  // dest NIC
        0x11, 0x00,        // messageSize
        0x00, 0x00,        // NumberOfHops
        0x50,              // checkSum
        0x01               // messageType FReeSLot
    };
    FullMessage msg;
    ReadHeader(buffer, &msg);
    EXPECT_EQ(msg.sourceAdress.OUIBits[0], 0x00);
    EXPECT_EQ(msg.sourceAdress.OUIBits[1], 0x0A);
    EXPECT_EQ(msg.sourceAdress.OUIBits[2], 0xAA);
    EXPECT_EQ(msg.sourceAdress.NICSpecificBits[0], 0x04);
    EXPECT_EQ(msg.sourceAdress.NICSpecificBits[1], 0x05);
    EXPECT_EQ(msg.sourceAdress.NICSpecificBits[2], 0x06);
    EXPECT_EQ(msg.destinationAdress.OUIBits[0], 0x00);
    EXPECT_EQ(msg.destinationAdress.OUIBits[1], 0x0A);
    EXPECT_EQ(msg.destinationAdress.OUIBits[2], 0xAA);
    EXPECT_EQ(msg.destinationAdress.NICSpecificBits[0], 0x0A);
    EXPECT_EQ(msg.destinationAdress.NICSpecificBits[1], 0x0B);
    EXPECT_EQ(msg.destinationAdress.NICSpecificBits[2], 0x0C);
    EXPECT_EQ(msg.messageSize, (uint16_t)17);
    EXPECT_EQ(msg.NumberOfHops, 0);
    EXPECT_EQ(msg.checkSum, 0x50);
    EXPECT_EQ(msg.messageType, FREE_SLOT);
}
// Test case for ReadFreeSlotMessage function
TEST(ReadTest, ReadFreeSlotMessageTest){
    uint8_t buffer[FREE_SLOT_SIZE] = {
        0x00, 0x0A, 0xAA,  // source OUI
        0x04, 0x05, 0x06,  // source NIC
        0x00, 0x0A, 0xAA,  // dest OUI
        0x0A, 0x0B, 0x0C,  // dest NIC
        0x26, 0x00,        // messageSize (38)
        0x00, 0x00,        // NumberOfHops (0)
        0x50,              // checkSum
        0x01,              // messageType FREE_SLOT
        0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x58, 0x40, // percentToGW
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x40, // timeToGW.simtime
        0x01, 0x00, 0x00, 0x00 // slot
    };
    FreeSlotMessage msg;
    ReadFreeSlotMessage(buffer, &msg);
    EXPECT_EQ(msg.messageSize, (uint16_t)FREE_SLOT_SIZE);
    EXPECT_EQ(msg.messageType, FREE_SLOT);
    EXPECT_EQ(msg.percentToGW, (double)97.5);
    EXPECT_EQ(msg.timeToGW.simtime, (double)12.5);
    EXPECT_EQ(msg.slot, FREE_SLOT);
}
// Test case for ReadRequestSlotMessage function
TEST(ReadTest, ReadRequestSlotMessageTest){
    uint8_t buffer[REQUEST_SLOT_SIZE] = {
        0x00, 0x0A, 0xAA,  // source OUI
        0x04, 0x05, 0x06,  // source NIC
        0x00, 0x0A, 0xAA,  // dest OUI
        0x0A, 0x0B, 0x0C,  // dest NIC
        0x16, 0x00,        // messageSize (26)
        0x00, 0x00,        // NumberOfHops (0)
        0x50,              // checkSum
        0x02,              // messageType REQUEST_SLOT
        0x01, 0x00, 0x00, 0x00 // slot
    };
    RequestSlotMessage msg;
    ReadRequestSlotMessage(buffer, &msg);
    EXPECT_EQ(msg.messageSize, (uint16_t)REQUEST_SLOT_SIZE);
    EXPECT_EQ(msg.messageType, REQUEST_SLOT);
    EXPECT_EQ(msg.slot, 1);
}
// Test case for ReadAckRequestSlotSuccessMessage function
TEST(ReadTest, ReadAckRequestSlotSuccessMessageTest){
    uint8_t buffer[ACK_REQUEST_SLOT_SUCCESS_SIZE] = {
        0x00, 0x0A, 0xAA,  // source OUI
        0x04, 0x05, 0x06,  // source NIC
        0x00, 0x0A, 0xAA,  // dest OUI
        0x0A, 0x0B, 0x0C,  // dest NIC
        0x1E, 0x00,        // messageSize (30)
        0x00, 0x00,        // NumberOfHops (0)
        0x50,              // checkSum
        0x03,              // messageType ACK_REQUEST_SLOT_SUCCESS
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x3F, // startTimeSlot
        0x01, 0x00, 0x00, 0x00 // slot
    };
    AckRequestSlotSuccessMessage msg;
    ReadAckRequestSlotSuccessMessage(buffer, &msg);
    EXPECT_EQ(msg.messageSize, (uint16_t)ACK_REQUEST_SLOT_SUCCESS_SIZE);
    EXPECT_EQ(msg.messageType, ACK_REQUEST_SLOT_SUCCESS);
    EXPECT_EQ(msg.startTimeSlot, (double)1.5);
    EXPECT_EQ(msg.slot, 1);
}

// Test case for ReadAckRequestSlotFailMessage function
TEST(ReadTest, ReadAckRequestSlotFailMessageTest){
    uint8_t buffer[ACK_REQUEST_SLOT_FAIL_SIZE] = {
        0x00, 0x0A, 0xAA,  // source OUI
        0x04, 0x05, 0x06,  // source NIC
        0x00, 0x0A, 0xAA,  // dest OUI
        0x0A, 0x0B, 0x0C,  // dest NIC
        0x16, 0x00,        // messageSize (22)
        0x00, 0x00,        // NumberOfHops (0)
        0x50,              // checkSum
        0x04,               // messageType ACK_REQUEST_SLOT_FAIL
        0x01, 0x00, 0x00, 0x00 // slot

    };
    AckRequestSlotFailMessage msg;
    ReadAckRequestSlotFailMessage(buffer, &msg);
    EXPECT_EQ(msg.messageSize, (uint16_t)ACK_REQUEST_SLOT_FAIL_SIZE);
    EXPECT_EQ(msg.messageType, ACK_REQUEST_SLOT_FAIL);
    EXPECT_EQ(msg.slot, 1);
}
// Test case for ReadUpdateHeatValueMessage function
TEST(ReadTest, ReadUpdateHeatValueMessage){
    uint8_t buffer[UPDATE_HEAT_VALUE_SIZE] = {
        0x00, 0x0A, 0xAA,  // source OUI
        0x04, 0x05, 0x06,  // source NIC
        0x00, 0x0A, 0xAA,  // dest OUI
        0x0A, 0x0B, 0x0C,  // dest NIC
        0x2E, 0x00,        // messageSize (46)
        0x00, 0x00,        // NumberOfHops (0)
        0x50,              // checkSum
        0x05,              // messageType UPDATE_HEAT_VALUE
        0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x31, 0x40,// StartSendTime
        0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x58, 0x40, // percentToGW
        0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x31, 0x40, // timeToGW.simtime
        0x01, 0x00, 0x00, 0x00 // slot
    };
    UpdateHeatValueMessage msg;
    ReadUpdateHeatValueMessage(buffer, &msg);
    EXPECT_EQ(msg.messageSize, (uint16_t)UPDATE_HEAT_VALUE_SIZE);
    EXPECT_EQ(msg.messageType, UPDATE_HEAT_VALUE);
    EXPECT_EQ(msg.startSendTime, (double)17.5);
    EXPECT_EQ(msg.percentToGW, (double)97.5);
    EXPECT_EQ(msg.timeToGW.simtime, (double)17.5);
    EXPECT_EQ(msg.slot, 1);
}
// Test case for ReadForwardPacketMessage function
TEST(ReadTest, ReadForwardPacketMessage){
    uint8_t buffer[FORWARD_PACKET_SIZE] = {
        0x00, 0x0A, 0xAA,  // source OUI
        0x04, 0x05, 0x06,  // source NIC
        0x00, 0x0A, 0xAA,  // dest OUI
        0x0A, 0x0B, 0x0C,   // dest NIC
        0x2A, 0x00,        // messageSize 42
        0x00, 0x00,        // NumberOfHops (0)
        0x50,              // checkSum
        0x06,              // messageType FORWARD_PACKET
        0x01, 0x00, 0x00, 0x00, // slot
        0x00, 0x0A, 0xAA,  // startNodeAddress OUI
        0x04, 0x05, 0x05,  // startNodeAddress NIC
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, // data
        0x01, 0x00, 0x00, 0x00 // seqNum
    };
    ForwardPacketMessage msg;
    ReadForwardPacketMessage(buffer, &msg);
    EXPECT_EQ(msg.messageSize, (uint16_t)FORWARD_PACKET_SIZE);
    EXPECT_EQ(msg.messageType, FORWARD_PACKET);
    EXPECT_EQ(msg.slot, 1);
    EXPECT_EQ(msg.startNodeAddress.OUIBits[0], 0x00);
    EXPECT_EQ(msg.startNodeAddress.OUIBits[1], 0x0A);
    EXPECT_EQ(msg.startNodeAddress.OUIBits[2], 0xAA);
    EXPECT_EQ(msg.startNodeAddress.NICSpecificBits[0], 0x04);
    EXPECT_EQ(msg.startNodeAddress.NICSpecificBits[1], 0x05);
    EXPECT_EQ(msg.startNodeAddress.NICSpecificBits[2], 0x05);
    uint8_t check = strncmp((char*)msg.data, (char*)buffer + 28, DATA_SIZE);
    EXPECT_EQ(check, 0);
    EXPECT_EQ(msg.seqNum, 1);
}
// Test case for ReadAckPacketToGWMessage function
TEST(ReadTest, ReadAckPacketToGWMessage){
    uint8_t buffer[ACK_PACKET_TO_GW_SIZE] = {
        0x00, 0x0A, 0xAA,  // source OUI
        0x04, 0x05, 0x06,  // source NIC
        0x00, 0x0A, 0xAA,  // dest OUI
        0x0A, 0x0B, 0x0C,  // dest NIC
        0x1A, 0x00,        // messageSize (26)
        0x00, 0x00,        // NumberOfHops (0)
        0x50,              // checkSum
        0x07,              // messageType ACK_PACKET_TO_GW
        0x01, 0x00, 0x00, 0x00, // slot
        0x01, 0x00, 0x00, 0x00 // seqNum
    };
    AckPacketToGWMessage msg;
    ReadAckPacketToGWMessage(buffer, &msg);
    EXPECT_EQ(msg.messageSize, (uint16_t)ACK_PACKET_TO_GW_SIZE);
    EXPECT_EQ(msg.messageType, ACK_PACKET_TO_GW);
    EXPECT_EQ(msg.slot, 1);
    EXPECT_EQ(msg.seqNum, 1);
}
// Test case for ReadAckPacketFailToGWMessage function
TEST(ReadTest, ReadAckPacketFailToGWMessage){
    uint8_t buffer[ACK_PACKET_FAIL_TO_GW_SIZE] = {
        0x00, 0x0A, 0xAA,  // source OUI
        0x04, 0x05, 0x06,  // source NIC
        0x00, 0x0A, 0xAA,  // dest OUI
        0x0A, 0x0B, 0x0C,  // dest NIC
        0x26, 0x00,        // messageSize (38)
        0x00, 0x00,        // NumberOfHops (0)
        0x50,              // checkSum
        0x08,              // messageType ACK_PACKET_FAIL_TO_GW
        0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x58, 0x40, // percentToGW
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x40, // timeToGW.simtime
        0x01, 0x00, 0x00, 0x00 // seqNum
    };
    AckPacketFailToGWMessage msg;
    ReadAckPacketFailToGWMessage(buffer, &msg);
    EXPECT_EQ(msg.messageSize, (uint16_t)ACK_PACKET_FAIL_TO_GW_SIZE);
    EXPECT_EQ(msg.messageType, ACK_PACKET_FAIL_TO_GW);
    EXPECT_EQ(msg.percentToGW, (double)97.5);
    EXPECT_EQ(msg.timeToGW.simtime, (double)12.5);
    EXPECT_EQ(msg.seqNum, 1);
}

TEST(ReadTest, ReadAckUpdateHeatValueMessage){
    uint8_t buffer[ACK_UPDATE_HEAT_VALUE_SIZE] = {
        0x00, 0x0A, 0xAA,  // source OUI
        0x04, 0x05, 0x06,  // source NIC
        0x00, 0x0A, 0xAA,  // dest OUI
        0x0A, 0x0B, 0x0C,  // dest NIC
        0x26, 0x00,        // messageSize (38)
        0x00, 0x00,        // NumberOfHops (0)
        0x50,              // checkSum
        0x09,              // messageType ACK_UPDATE_HEAT_VALUE
        0x01, 0x00, 0x00, 0x00, // slot
        0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x58, 0x40, // percentToGW
        0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x31, 0x40 // timeToGW.simtime
    };
    AckUpdateHeatValueMessage msg;
    ReadAckUpdateHeatValueMessage(buffer, &msg);
    EXPECT_EQ(msg.messageSize, (uint16_t)ACK_UPDATE_HEAT_VALUE_SIZE);
    EXPECT_EQ(msg.messageType, ACK_UPDATE_HEAT_VALUE);
    EXPECT_EQ(msg.slot, (int32_t)1);
    EXPECT_EQ(msg.percentToGW, (double)97.5);
    EXPECT_EQ(msg.timeToGW.simtime, (double)17.5);
}

// Test case for WriteHeader function
TEST(WriteTest, WriteHeaderTest){
    uint8_t buffer[18] = {0};
    FullMessage msg;
    msg.sourceAdress.OUIBits[0] = 0x00;
    msg.sourceAdress.OUIBits[1] = 0x0A;
    msg.sourceAdress.OUIBits[2] = 0xAA;
    msg.sourceAdress.NICSpecificBits[0] = 0x04;
    msg.sourceAdress.NICSpecificBits[1] = 0x05;
    msg.sourceAdress.NICSpecificBits[2] = 0x06;
    msg.destinationAdress.OUIBits[0] = 0x00;
    msg.destinationAdress.OUIBits[1] = 0x0A;
    msg.destinationAdress.OUIBits[2] = 0xAA;
    msg.destinationAdress.NICSpecificBits[0] = 0x0A;
    msg.destinationAdress.NICSpecificBits[1] = 0x0B;
    msg.destinationAdress.NICSpecificBits[2] = 0x0C;
    msg.messageSize = 18;
    msg.NumberOfHops = 0;
    msg.messageType = FREE_SLOT;
    WriteHeader(buffer, &msg);
    msg.checkSum = CheckSumCalculate(buffer, 18);
    EXPECT_EQ(buffer[0], 0x00);
    EXPECT_EQ(buffer[1], 0x0A);
    EXPECT_EQ(buffer[2], 0xAA);
    EXPECT_EQ(buffer[3], 0x04);
    EXPECT_EQ(buffer[4], 0x05);
    EXPECT_EQ(buffer[5], 0x06);
    EXPECT_EQ(buffer[6], 0x00);
    EXPECT_EQ(buffer[7], 0x0A);
    EXPECT_EQ(buffer[8], 0xAA);
    EXPECT_EQ(buffer[9], 0x0A);
    EXPECT_EQ(buffer[10], 0x0B);
    EXPECT_EQ(buffer[11], 0x0C);
    EXPECT_EQ(buffer[12], 0x12);
    EXPECT_EQ(buffer[13], 0x00);
    EXPECT_EQ(buffer[14], 0x00);
    EXPECT_EQ(buffer[15], 0x00);
    EXPECT_EQ(buffer[17], 0x01);
}
// Test case for WriteFreeSlotMessage function
TEST(WriteTest, WriteFreeSlotMessageTest){
    uint8_t buffer[FREE_SLOT_SIZE] = {0};
    FreeSlotMessage msg;
    msg.messageSize = FREE_SLOT_SIZE;
    msg.messageType = FREE_SLOT;
    msg.percentToGW = 97.5;
    msg.timeToGW.simtime = 12.5;
    msg.slot = FREE_SLOT;
    WriteFreeSlotMessage(buffer, &msg);
    msg.checkSum = CheckSumCalculate(buffer, FREE_SLOT_SIZE);
    EXPECT_EQ(buffer[12], FREE_SLOT_SIZE);
    EXPECT_EQ(buffer[13], 0x00);
    EXPECT_EQ(buffer[16], msg.checkSum);
    EXPECT_EQ(buffer[17], 0x01);
    uint8_t check = strncmp((char*)buffer + 18, (char*)&msg.percentToGW, 8);
    EXPECT_EQ(check, 0);
    check = strncmp((char*)buffer + 26, (char*)&msg.timeToGW.simtime, 8);
    EXPECT_EQ(check, 0);
    check = strncmp((char*)buffer + 34, (char*)&msg.slot, 4);
    EXPECT_EQ(check, 0);
}
// Test case for WriteRequestSlotMessage function
TEST(WriteTest, WriteRequestSlotMessageTest){
    uint8_t buffer[REQUEST_SLOT_SIZE] = {0};
    RequestSlotMessage msg;
    msg.messageSize = REQUEST_SLOT_SIZE;
    msg.messageType = REQUEST_SLOT;
    msg.slot = 1;
    WriteRequestSlotMessage(buffer, &msg);
    msg.checkSum = CheckSumCalculate(buffer, REQUEST_SLOT_SIZE);
    EXPECT_EQ(buffer[12], REQUEST_SLOT_SIZE);
    EXPECT_EQ(buffer[13], 0x00);
    EXPECT_EQ(buffer[16], msg.checkSum);
    EXPECT_EQ(buffer[17], 0x02);
    uint8_t check = strncmp((char*)buffer + 18, (char*)&msg.slot, 4);
    EXPECT_EQ(check, 0);
}
// Test case for WriteAckRequestSlotSuccessMessage function
TEST(WriteTest, WriteAckRequestSlotSuccessMessageTest){
    uint8_t buffer[ACK_REQUEST_SLOT_SUCCESS_SIZE] = {0};
    AckRequestSlotSuccessMessage msg;
    msg.messageSize = ACK_REQUEST_SLOT_SUCCESS_SIZE;
    msg.messageType = ACK_REQUEST_SLOT_SUCCESS;
    msg.startTimeSlot = 1.5;
    msg.slot = 1;
    WriteAckRequestSlotSuccessMessage(buffer, &msg);
    msg.checkSum = CheckSumCalculate(buffer, ACK_REQUEST_SLOT_SUCCESS_SIZE);
    EXPECT_EQ(buffer[12], ACK_REQUEST_SLOT_SUCCESS_SIZE);
    EXPECT_EQ(buffer[13], 0x00);
    EXPECT_EQ(buffer[16], msg.checkSum);
    EXPECT_EQ(buffer[17], 0x03);
    uint8_t check = strncmp((char*)buffer + 18, (char*)&msg.startTimeSlot, 8);
    EXPECT_EQ(check, 0);
    check = strncmp((char*)buffer + 26, (char*)&msg.slot, 4);
    EXPECT_EQ(check, 0);
}
// Test case for WriteAckRequestSlotFailMessage function
TEST(WriteTest, WriteAckRequestSlotFailMessageTest){
    uint8_t buffer[ACK_REQUEST_SLOT_FAIL_SIZE] = {0};
    AckRequestSlotFailMessage msg;
    msg.messageSize = ACK_REQUEST_SLOT_FAIL_SIZE;
    msg.messageType = ACK_REQUEST_SLOT_FAIL;
    msg.slot = 1;
    WriteAckRequestSlotFailMessage(buffer, &msg);
    msg.checkSum = CheckSumCalculate(buffer, ACK_REQUEST_SLOT_FAIL_SIZE);
    EXPECT_EQ(buffer[12], ACK_REQUEST_SLOT_FAIL_SIZE);
    EXPECT_EQ(buffer[13], 0x00);
    EXPECT_EQ(buffer[16], msg.checkSum);
    EXPECT_EQ(buffer[17], 0x04);
    uint8_t check = strncmp((char*)buffer + 18, (char*)&msg.slot, 4);
    EXPECT_EQ(check, 0);
}
// Test case for WriteUpdateHeatValueMessage function
TEST(WriteTest, WriteUpdateHeatValueMessage){
    uint8_t buffer[UPDATE_HEAT_VALUE_SIZE] = {0};
    UpdateHeatValueMessage msg;
    msg.messageSize = UPDATE_HEAT_VALUE_SIZE;
    msg.messageType = UPDATE_HEAT_VALUE;
    msg.startSendTime = 17.5;
    msg.percentToGW = 97.5;
    msg.timeToGW.simtime = 17.5;
    msg.slot = 1;
    WriteUpdateHeatValueMessage(buffer, &msg);
    msg.checkSum = CheckSumCalculate(buffer, UPDATE_HEAT_VALUE_SIZE);
    EXPECT_EQ(buffer[12], UPDATE_HEAT_VALUE_SIZE);
    EXPECT_EQ(buffer[13], 0x00);
    EXPECT_EQ(buffer[16], msg.checkSum);
    EXPECT_EQ(buffer[17], 0x05);
    uint8_t check = strncmp((char*)buffer + 18, (char*)&msg.startSendTime, 8);
    EXPECT_EQ(check, 0);
    check = strncmp((char*)buffer + 26, (char*)&msg.percentToGW, 8);
    EXPECT_EQ(check, 0);
    check = strncmp((char*)buffer + 34, (char*)&msg.timeToGW.simtime, 8);
    EXPECT_EQ(check, 0);
    check = strncmp((char*)buffer + 42, (char*)&msg.slot, 4);
    EXPECT_EQ(check, 0);
}
// Test case for WriteForwardPacketMessage function
TEST(WriteTest, WriteForwardPacketMessage){
    uint8_t buffer[FORWARD_PACKET_SIZE] = {0};
    ForwardPacketMessage msg;
    msg.messageSize = FORWARD_PACKET_SIZE;
    msg.messageType = FORWARD_PACKET;
    msg.slot = 1;
    msg.seqNum = 1;
    msg.startNodeAddress.OUIBits[0] = 0x00;
    msg.startNodeAddress.OUIBits[1] = 0x0A;
    msg.startNodeAddress.OUIBits[2] = 0xAA;
    msg.startNodeAddress.NICSpecificBits[0] = 0x04;
    msg.startNodeAddress.NICSpecificBits[1] = 0x05;
    msg.startNodeAddress.NICSpecificBits[2] = 0x05;
    WriteForwardPacketMessage(buffer, &msg);
    msg.checkSum = CheckSumCalculate(buffer, FORWARD_PACKET_SIZE);
    EXPECT_EQ(buffer[12], FORWARD_PACKET_SIZE);
    EXPECT_EQ(buffer[13], 0x00);
    EXPECT_EQ(buffer[16], msg.checkSum);
    EXPECT_EQ(buffer[17], 0x06);
    uint8_t check = strncmp((char*)buffer + 18, (char*)&msg.slot, 4);
    EXPECT_EQ(check, 0);
    check = strncmp((char*)buffer + 22, (char*)&msg.startNodeAddress.OUIBits[0], 3);
    EXPECT_EQ(check, 0);
    check = strncmp((char*)buffer + 25, (char*)&msg.startNodeAddress.NICSpecificBits[0], 3);
    EXPECT_EQ(check, 0);
    check = strncmp((char*)buffer + 28, (char*)msg.data, DATA_SIZE);
    EXPECT_EQ(check, 0);
    check = strncmp((char*)buffer + 28 + DATA_SIZE, (char*)&msg.seqNum, 4);
    EXPECT_EQ(check, 0);
}
// Test case for WriteAckPacketToGWMessage function
TEST(WriteTest, WriteAckPacketToGWMessage){
    uint8_t buffer[ACK_PACKET_TO_GW_SIZE] = {0};
    AckPacketToGWMessage msg;
    msg.messageSize = ACK_PACKET_TO_GW_SIZE;
    msg.messageType = ACK_PACKET_TO_GW;
    msg.slot = 1;
    msg.seqNum = 1;
    WriteAckPacketToGWMessage(buffer, &msg);
    msg.checkSum = CheckSumCalculate(buffer, ACK_PACKET_TO_GW_SIZE);
    EXPECT_EQ(buffer[12], ACK_PACKET_TO_GW_SIZE);
    EXPECT_EQ(buffer[13], 0x00);
    EXPECT_EQ(buffer[16], msg.checkSum);
    EXPECT_EQ(buffer[17], 0x07);
    uint8_t check = strncmp((char*)buffer + 18, (char*)&msg.slot, 4);
    EXPECT_EQ(check, 0);
    check = strncmp((char*)buffer + 22, (char*)&msg.seqNum, 4);
    EXPECT_EQ(check, 0);
}
// Test case for WriteAckPacketFailToGWMessage function
TEST(WriteTest, WriteAckPacketFailToGWMessage){
    uint8_t buffer[ACK_PACKET_FAIL_TO_GW_SIZE] = {0};
    AckPacketFailToGWMessage msg;
    msg.messageSize = ACK_PACKET_FAIL_TO_GW_SIZE;
    msg.messageType = ACK_PACKET_FAIL_TO_GW;
    msg.percentToGW = 97.5;
    msg.timeToGW.simtime = 12.5;
    msg.seqNum = 1;
    WriteAckPacketFailToGWMessage(buffer, &msg);
    msg.checkSum = CheckSumCalculate(buffer, ACK_PACKET_FAIL_TO_GW_SIZE);
    EXPECT_EQ(buffer[12], ACK_PACKET_FAIL_TO_GW_SIZE);
    EXPECT_EQ(buffer[13], 0x00);
    EXPECT_EQ(buffer[16], msg.checkSum);
    EXPECT_EQ(buffer[17], 0x08);
    uint8_t check = strncmp((char*)buffer + 18, (char*)&msg.percentToGW, 8);
    EXPECT_EQ(check, 0);
    check = strncmp((char*)buffer + 26, (char*)&msg.timeToGW.simtime, 8);
    EXPECT_EQ(check, 0);
    check = strncmp((char*)buffer + 34, (char*)&msg.seqNum, 4);
    EXPECT_EQ(check, 0);
}

TEST(WriteTest, WriteAckUpdateHeatValueMessage){
    uint8_t buffer[ACK_UPDATE_HEAT_VALUE_SIZE] = {0};
    AckUpdateHeatValueMessage msg;
    msg.messageSize = ACK_UPDATE_HEAT_VALUE_SIZE;
    msg.messageType = ACK_UPDATE_HEAT_VALUE;
    msg.slot = 1;
    msg.percentToGW = 97.5;
    msg.timeToGW.simtime = 17.5;
    WriteAckUpdateHeatValueMessage(buffer, &msg);
    msg.checkSum = CheckSumCalculate(buffer, ACK_UPDATE_HEAT_VALUE_SIZE);
    EXPECT_EQ(buffer[12], ACK_UPDATE_HEAT_VALUE_SIZE);
    EXPECT_EQ(buffer[13], 0x00);
    EXPECT_EQ(buffer[16], msg.checkSum);
    EXPECT_EQ(buffer[17], 0x09);
    uint8_t check;
    check = strncmp((char*)buffer + 18, (char*)&msg.slot, 4);
    EXPECT_EQ(check, 0);
    check = strncmp((char*)buffer + 22, (char*)&msg.percentToGW, 8);
    EXPECT_EQ(check, 0);
    check = strncmp((char*)buffer + 30, (char*)&msg.timeToGW.simtime, 8);
    EXPECT_EQ(check, 0);
}

TEST(WriteTest, CheckSumCalculateTest){
    uint8_t buffer[18] = {
        0x00, 0x0A, 0xAA,
        0x04, 0x05, 0x06,
        0x00, 0x0A, 0xAA,
        0x0A, 0x0B, 0x0C,
        0x11, 0x00,
        0x00, 0x00,
        0xAA,
        0x01
    };
    uint8_t checkSum = CheckSumCalculate(buffer, 18);
    EXPECT_EQ(checkSum, 0xAA);
}

TEST(CalculateTest, CalculateTimeToGWTest){
    neighbourInf[0].timeToGW.simtime = 10.5;
    neighbourInf[0].percentToGW = 97.0;
    neighbourInf[0].slot = 0;
    neighbourInf[1].timeToGW.simtime = 20.0;
    neighbourInf[1].percentToGW = 95.0;
    neighbourInf[1].slot = 1;
    neighbourInf[2].timeToGW.simtime = 30.0;
    neighbourInf[2].percentToGW = 90.0;
    neighbourInf[2].slot = 2;
    neighbourInf[3].timeToGW.simtime = 40.0;
    neighbourInf[3].percentToGW = 95.0;
    neighbourInf[3].slot = 3;
    neighbourInf[4].timeToGW.simtime = 0.0;
    neighbourInf[4].percentToGW = 100.0;
    neighbourInf[4].slot = 4;
    neighbourInf[5].timeToGW.simtime = 10.0;
    neighbourInf[5].percentToGW = 100.0;
    neighbourInf[5].slot = 5;
    double timeToGW = calculateTimeToGW(2, neighbourInf[0]);
    EXPECT_EQ(timeToGW, 10.5 + static_cast<double>(0 + MAX_SLOT - 2) *SLOT_TIME/1000);
    timeToGW = calculateTimeToGW(3, neighbourInf[5]);
    EXPECT_EQ(timeToGW, 10.0 + static_cast<double>(5 - 3) *SLOT_TIME/1000);
    timeToGW = calculateTimeToGW(1, neighbourInf[4]);
    EXPECT_EQ(timeToGW, 3.0);
}

TEST(CalculateTest, CalculatePercentToGWTest){
    double percentToGW = calculatePRR(1300, 1023, neighbourInf[4]);
    EXPECT_EQ(percentToGW, 78.6923076923077);
}

TEST(CalculateTest, CalculateSlotTest){
    neighbourInf[4].timeToGW.simtime = 10.0;
    neighbourInf[5].percentToGW = 97.5;
    int slot = chooseSlotToForwardPacket(neighbourInf, currentLocalNodeNumber);
    EXPECT_EQ(slot, 4);
}

DataPacket dataBufferTest[6];
int NumberOfPackets = 6;
bool isFull = false;
TEST(CalculateTest, queueDequeueTest){
    for (int i = 0; i < 6; i++){
        for (int j = 0; j < DATA_SIZE; j++){
            dataBufferTest[i].data[j] = j + i;
        }
    }
    ForwardPacketMessage forwardPacketMessage;
    forwardPacketMessage.startNodeAddress.OUIBits[0] = 0x00;
    forwardPacketMessage.startNodeAddress.OUIBits[1] = 0x0A;
    forwardPacketMessage.startNodeAddress.OUIBits[2] = 0xAA;
    forwardPacketMessage.startNodeAddress.NICSpecificBits[0] = 0x04;
    forwardPacketMessage.startNodeAddress.NICSpecificBits[1] = 0x05;
    forwardPacketMessage.startNodeAddress.NICSpecificBits[2] = 0x06;
    for (int i = 0; i < DATA_SIZE; i++){
        forwardPacketMessage.data[i] = i + 5;
    }
    dequeueDataBuffer(NumberOfPackets, dataBufferTest);
    EXPECT_EQ(NumberOfPackets, 5);
    for (int i = 0; i < DATA_SIZE; i++){
        EXPECT_EQ(dataBufferTest[5].data[i], 0);
        EXPECT_EQ(dataBufferTest[4].data[i], i + 5);
        EXPECT_EQ(dataBufferTest[3].data[i], i + 4);
        EXPECT_EQ(dataBufferTest[2].data[i], i + 3);
        EXPECT_EQ(dataBufferTest[1].data[i], i + 2);
        EXPECT_EQ(dataBufferTest[0].data[i], i + 1);
    }
    isFull = enqueueDataBuffer(forwardPacketMessage, NumberOfPackets, dataBufferTest);
    EXPECT_EQ(dataBufferTest[5].startNodeAdress.NICSpecificBits[0], 0x04);
    EXPECT_EQ(dataBufferTest[5].startNodeAdress.NICSpecificBits[1], 0x05);
    EXPECT_EQ(dataBufferTest[5].startNodeAdress.NICSpecificBits[2], 0x06);
    EXPECT_EQ(dataBufferTest[5].data[0], 0 + 5);
    EXPECT_EQ(dataBufferTest[5].data[9], 9 + 5);
    EXPECT_EQ(NumberOfPackets, 6);
    EXPECT_EQ(isFull, false);
    NumberOfPackets = 20;
    isFull = enqueueDataBuffer(forwardPacketMessage, NumberOfPackets, dataBufferTest);
    EXPECT_EQ(isFull, true);
}

TEST(CalculateTest, CheckDisplayTest){
    DataPacket displayBuffer[5];
    int NodeNumber = 0;
    ForwardPacketMessage forwardPacketMessage;
    forwardPacketMessage.startNodeAddress.OUIBits[0] = 0x00;
    forwardPacketMessage.startNodeAddress.OUIBits[1] = 0x0A;
    forwardPacketMessage.startNodeAddress.OUIBits[2] = 0xAA;
    forwardPacketMessage.startNodeAddress.NICSpecificBits[0] = 0x04;
    forwardPacketMessage.startNodeAddress.NICSpecificBits[1] = 0x05;
    forwardPacketMessage.startNodeAddress.NICSpecificBits[2] = 0x06;
    for (int i = 0; i < DATA_SIZE; i++){
        forwardPacketMessage.data[i] = i + 5;
    }
    updateDisplayBuffer(forwardPacketMessage, NodeNumber, displayBuffer);
    for (int i = 0; i < DATA_SIZE; i++){
        EXPECT_EQ(displayBuffer[NodeNumber-1].data[i], i + 5);
    }
    EXPECT_EQ(NodeNumber, 1);
    for (int i = 0; i < DATA_SIZE; i++){
        forwardPacketMessage.data[i] = i + 7;
    }
    updateDisplayBuffer(forwardPacketMessage, NodeNumber, displayBuffer);
    for (int i = 0; i < DATA_SIZE; i++){
        EXPECT_EQ(displayBuffer[NodeNumber-1].data[i], i + 7);
    }
    EXPECT_EQ(NodeNumber, 1);
}
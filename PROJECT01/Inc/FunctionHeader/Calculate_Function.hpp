#include "protocolNaming.hpp"
#include "Config.h"
double calculatePRR(unsigned int totalSend, unsigned int totalSuccessSend, neighbourNode neighbourToSend);
double calculateTimeToGW(int slot, neighbourNode neighbourInf);
int chooseSlotToForwardPacket(neighbourNode *neighbourInf, int currentLocalNodeNumber);
int NeighbourLocation(neighbourNode *neighbourInf, int slot, int currentLocalNodeNumber);
void dequeueDataBuffer(int &NumberOfPackets, DataPacket *dataBuffer);
void updateDisplayBuffer(ForwardPacketMessage forwardPacketMessage, int &NumberOfNodeToDisplay, DataPacket *DisplayBuffer);
bool enqueueDataBuffer(ForwardPacketMessage forwardPacketMessage, int &NumberOfPackets, DataPacket *dataBuffer);
bool enqueueDataBuffer(DataPacket dataGet, int &NumberOfPackets, DataPacket *dataBuffer);
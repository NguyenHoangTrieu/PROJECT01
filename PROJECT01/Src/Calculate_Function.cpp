#include "Calculate_Function.hpp"
/* * @brief calculate the PRR of the node
 * @param lastSend: the array of the last send time of the node.
 * @return the PRR of the node in percentage
 */
double calculatePRR(unsigned int totalSend, unsigned int totalSuccessSend, neighbourNode neighbourToSend){
    if (DEVICE_TYPE == GATEWAY) return 100.0;
    else if(totalSend <= 10) return 100.0;
    else{
        double PRR = static_cast<double>(totalSuccessSend) / totalSend * 100.0;
        PRR = static_cast<double>(PRR * neighbourToSend.percentToGW) / 100.0;
        return PRR;
    }
}
/* * @brief calculate the time to gateway
 * @param slot: the current slot of the node.
 * @param neighbourInf: the information of the neighbour node/gateway that node will sent to.
 * @return the time to gateway in seconds
 */
double calculateTimeToGW(int slot, neighbourNode neighbourInf){
    if (DEVICE_TYPE == GATEWAY) return 0.0;
    double timeToGW = 0.0;
    if (neighbourInf.slot - slot > 0) 
        timeToGW = (neighbourInf.slot - slot)*SLOT_TIME/1000 + neighbourInf.timeToGW.simtime;
    else
        timeToGW = static_cast<double>(neighbourInf.slot + MAX_SLOT - slot)*SLOT_TIME/1000 + neighbourInf.timeToGW.simtime;
    return timeToGW;
}

int chooseSlotToForwardPacket(neighbourNode *neighbourInf, int currentLocalNodeNumber){
    if (DEVICE_TYPE == GATEWAY) return -1;
    bool continueCheck = false;
    // Sort neighbourInf array by timeToGW from min to max
    for (int i = 0; i < currentLocalNodeNumber - 1; i++) {
        for (int j = i + 1; j < currentLocalNodeNumber; j++) {
            if (neighbourInf[i].timeToGW.simtime > neighbourInf[j].timeToGW.simtime) {
                neighbourNode temp = neighbourInf[i];
                neighbourInf[i] = neighbourInf[j];
                neighbourInf[j] = temp;
            }
        }
    }
    double minTimeToGW = neighbourInf[0].timeToGW.simtime;
    int minTimeToGWIndex = 0;
    // Check if how many node has a same timeToGW as the first node
    for (int i = 1; i < currentLocalNodeNumber; i++){
        if (neighbourInf[i].timeToGW.simtime == minTimeToGW){
            minTimeToGWIndex++;
            continueCheck = true;
        }
        else break;
    }
    if (continueCheck == false) return neighbourInf[0].slot;
    else{
        double maxPRR = neighbourInf[0].percentToGW;
        int slotSendData = neighbourInf[0].slot;
        for (int i = 1; i < minTimeToGWIndex; i++){
            if (neighbourInf[i].percentToGW > maxPRR){
                maxPRR = neighbourInf[i].percentToGW;
                slotSendData = neighbourInf[i].slot;
            }
        }
        return slotSendData;
    }
}
/* * @brief find the location of the neighbour node in the neighbourInf array
 * @param neighbourInf: the information of the neighbour node/gateway that node will sent to.
 * @param slot: the slot to communicate with the neighbour node
 * @param currentLocalNodeNumber: the number of local nodes in the network
 * @return the location of the neighbour node in the neighbourInf array
 */
int NeighbourLocation(neighbourNode *neighbourInf, int slot, int currentLocalNodeNumber){
    for (int i = 0; i < currentLocalNodeNumber; i++){
        if (neighbourInf[i].slot == slot) return i;
    }
    return currentLocalNodeNumber;
}

/* * @brief dequeue the data buffer to the data buffer
 * @param NumberOfPackets: the number of packets in the data buffer
 * @param dataBuffer: the data buffer that will be dequeued from
 */
void dequeueDataBuffer(int &NumberOfPackets, DataPacket *dataBuffer){
    if (DEVICE_TYPE == GATEWAY) return;
    for (int i = 0; i < NumberOfPackets; i++){
        dataBuffer[i].startNodeAdress.OUIBits[0] = dataBuffer[i + 1].startNodeAdress.OUIBits[0];
        dataBuffer[i].startNodeAdress.OUIBits[1] = dataBuffer[i + 1].startNodeAdress.OUIBits[1];
        dataBuffer[i].startNodeAdress.OUIBits[2] = dataBuffer[i + 1].startNodeAdress.OUIBits[2];
        dataBuffer[i].startNodeAdress.NICSpecificBits[0] = dataBuffer[i + 1].startNodeAdress.NICSpecificBits[0];
        dataBuffer[i].startNodeAdress.NICSpecificBits[1] = dataBuffer[i + 1].startNodeAdress.NICSpecificBits[1];
        dataBuffer[i].startNodeAdress.NICSpecificBits[2] = dataBuffer[i + 1].startNodeAdress.NICSpecificBits[2];
        for (int j = 0; j < DATA_SIZE; j++){
            dataBuffer[i].data[j] = dataBuffer[i + 1].data[j];
        }
    }
    dataBuffer[NumberOfPackets - 1].startNodeAdress.OUIBits[0] = 0;
    dataBuffer[NumberOfPackets - 1].startNodeAdress.OUIBits[1] = 0;
    dataBuffer[NumberOfPackets - 1].startNodeAdress.OUIBits[2] = 0;
    dataBuffer[NumberOfPackets - 1].startNodeAdress.NICSpecificBits[0] = 0;
    dataBuffer[NumberOfPackets - 1].startNodeAdress.NICSpecificBits[1] = 0;
    dataBuffer[NumberOfPackets - 1].startNodeAdress.NICSpecificBits[2] = 0;
    for (int j = 0; j < DATA_SIZE; j++){
        dataBuffer[NumberOfPackets - 1].data[j] = 0;
    }
    NumberOfPackets--;
}
/* * @brief enqueue the data buffer to the data buffer
 * @param forwardPacketMessage: the message that will be enqueued to the data buffer
 * @param NumberOfPackets: the number of packets in the data buffer
 * @param dataBuffer: the data buffer that will be enqueued to
 * @return true if the data buffer is full, false if the data buffer is not full
 */

bool enqueueDataBuffer(ForwardPacketMessage forwardPacketMessage, int &NumberOfPackets, DataPacket *dataBuffer){
    if (DEVICE_TYPE == GATEWAY) return false;
    if (NumberOfPackets < DATA_BUFFER_SIZE){
    dataBuffer[NumberOfPackets].startNodeAdress = forwardPacketMessage.startNodeAddress;
    for (int j = 0; j < DATA_SIZE; j++){
        dataBuffer[NumberOfPackets].data[j] = forwardPacketMessage.data[j];
    }
        NumberOfPackets++;
        return false;
    }
    else return true;
}

/* * @brief enqueue self data buffer to the data buffer
 * @param forwardPacketMessage: the message that will be enqueued to the data buffer
 * @param NumberOfPackets: the number of packets in the data buffer
 * @param dataBuffer: the data buffer that will be enqueued to
 * @return true if the data buffer is full, false if the data buffer is not full
 */
bool enqueueDataBuffer(DataPacket dataGet, int &NumberOfPackets, DataPacket *dataBuffer){
    if (DEVICE_TYPE == GATEWAY) return false;
    if (NumberOfPackets < DATA_BUFFER_SIZE){
        dataBuffer[NumberOfPackets].startNodeAdress = dataGet.startNodeAdress;
        for (int j = 0; j < DATA_SIZE; j++){
            dataBuffer[NumberOfPackets].data[j] = dataGet.data[j];
        }
        NumberOfPackets++;
        return false;
    }
    else return true;
}
/* * @brief update the display buffer to the data buffer
 * @param forwardPacketMessage: the message that will be enqueued to the data buffer
 * @param NumberOfPackets: the number of packets in the data buffer
 * @param dataBuffer: the data buffer that will be enqueued to
 */
void updateDisplayBuffer(ForwardPacketMessage forwardPacketMessage, int &NumberOfNodeToDisplay, DataPacket *DisplayBuffer){
    for (int i = 0; i < NumberOfNodeToDisplay; i++){
        if (DisplayBuffer[i].startNodeAdress.OUIBits[0] == forwardPacketMessage.startNodeAddress.OUIBits[0] &&
            DisplayBuffer[i].startNodeAdress.OUIBits[1] == forwardPacketMessage.startNodeAddress.OUIBits[1] &&
            DisplayBuffer[i].startNodeAdress.OUIBits[2] == forwardPacketMessage.startNodeAddress.OUIBits[2] &&
            DisplayBuffer[i].startNodeAdress.NICSpecificBits[0] == forwardPacketMessage.startNodeAddress.NICSpecificBits[0] &&
            DisplayBuffer[i].startNodeAdress.NICSpecificBits[1] == forwardPacketMessage.startNodeAddress.NICSpecificBits[1] &&
            DisplayBuffer[i].startNodeAdress.NICSpecificBits[2] == forwardPacketMessage.startNodeAddress.NICSpecificBits[2]){
                for (int j = 0; j < DATA_SIZE; j++){
                    DisplayBuffer[i].data[j] = forwardPacketMessage.data[j];
                }
                return;
        }
    }
    if (NumberOfNodeToDisplay < DISPLAY_BUFFER_SIZE){
        DisplayBuffer[NumberOfNodeToDisplay].startNodeAdress = forwardPacketMessage.startNodeAddress;
        for (int j = 0; j < DATA_SIZE; j++){
            DisplayBuffer[NumberOfNodeToDisplay].data[j] = forwardPacketMessage.data[j];
        }
        NumberOfNodeToDisplay++;
    }
}
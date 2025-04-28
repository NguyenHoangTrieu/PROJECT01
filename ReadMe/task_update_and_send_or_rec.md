# **Sequence Diagram for Update Heat Value and Send/Receive Data Packet:**

###  1. Sequence Diagram for Update Heat Value (At the begin of slot that has been set):
####    *Assume that Node A send Update and Node B receive.*
```mermaid
sequenceDiagram
    participant Slot as Slot Service Task
    participant Update as Update Heat Value Task
    participant Send as Send UART6 Task (Node A)
    participant Receive as Node B

    activate Slot
    Slot ->> Update: xTaskNotify()
    deactivate Slot
    activate Update
    Update ->> Update: Create_Update_Heat_Message()
    Update ->> Send: xQueueSend()
    deactivate Update
    activate Send
    Send ->> Receive: SendUpdateHeatValue_Message()
    deactivate Send
```

### 2. Sequence Diagram for Receive Update/ACKUpdate Heat Value:

```mermaid
sequenceDiagram
    participant ISR as USART6 ISR
    participant Receive as Receive UART6 Task
    participant Update as Update/ACKUpdate Handler Task
    participant Setup as Update Heat Value Task

    activate ISR
    ISR ->>Receive: xTaskNotifyFromISR()
    deactivate ISR
    activate Receive
    
    alt MessageType = Update/ACKUpdate
        Receive ->> Receive: ReadMessage()
        Receive ->> Receive: ParseMessage()
        Receive ->> Update: xTaskNotify()
        deactivate Receive
        
        activate Update
        Update ->> Update: CheckMessageValid()
        alt Message valid
            Update ->> Setup: xTaskNotify()
            deactivate Update
            activate Setup
            deactivate Setup
        end
    end
```

### 3. Sequence Diagram for Handle Update Heat Value Message:

```mermaid
sequenceDiagram
    participant Setup as Update Heat Value Task
    Participant Data as Send/Receive Data Task
    participant Send as SendUART6 Task (Node B)
    participant Receive as Node A
    alt Receive Update Message
    activate Setup
    Setup ->> Setup: PrepareACKUpdateMessage()
    Setup ->> Setup: Mark_Receive_Update()
    Setup ->> Send: xQueueSend()
    deactivate Setup
    activate Send
    Send ->> Receive: SendACKUpdateMessage()
    deactivate Send
    else Timeout (Not Receive Update)
    activate Setup
    Setup ->> Data: xTaskNotify()
    deactivate Setup
    activate Data
    deactivate Data
    end
```
### 4. Sequence Diagram for Handle ACK Update Message:

```mermaid
sequenceDiagram
    participant Setup as Update Heat Value Task
    Participant Data as Send/Receive Data Task
    alt Receive Update Message
        activate Setup
        Setup ->> Setup: Mark_Receive_ACKUpdate()
        Setup ->> Data: xTaskNotify()
        deactivate Setup
        activate Data
        deactivate Data
    else Timeout (Not Receive Update)
        activate Setup
        Setup ->> Data: xTaskNotify()
        deactivate Setup
        activate Data
        deactivate Data
    end
```

### 5. Seuquence Diagram for Receive Data (Data from Node B):

```mermaid
sequenceDiagram
    participant ISR as USART6 ISR
    participant Receive as Receive UART6 Task
    participant Forward as Forward Packet Handler Task
    participant Data as Send/Receive Data Task
    participant Send as Send UART6 Task
    activate ISR
    ISR ->>Receive: xTaskNotifyFromISR()
    deactivate ISR
    activate Receive
    alt MessageType = Update/ACKUpdate
        Receive ->> Receive: ReadMessage()
        Receive ->> Receive: ParseMessage()
        Receive ->> Forward: xTaskNotify()
        deactivate Receive
        activate Forward
        Forward ->> Data: CheckMessageValid()
        alt Message valid
            Forward ->> Data: xTaskNotify()
            deactivate Forward
            alt Buffer Not Full
                activate Data
                Data ->> Data: SaveDataToBuffer()
                Data ->> Data: PrepareACKPacketSuccessMessage()
                Data ->> Send: xQueueSend()
                deactivate Data
                activate Send
                Send ->> Node B: ACKPacketSuccessMessage()
                deactivate Send
            else Buffer Full
                activate Data
                Data ->> Data: PrepareACKPacketFailMessage()
                Data ->> Send: xQueueSend()
                deactivate Data
                activate Send
                Send ->> Node B: ACKPacketFailMessage()
                deactivate Send
            end
        end

    end
```

### 6. Seuquence Diagram for Receive ACKPacket (after ACK (Fail/Success) Message Handler Task receive xTaskNotify() From ACK Receive UART6 Task or not receive any ACKPacket):

```mermaid
sequenceDiagram
    participant ACK as ACK Message Handler Task
    participant Data as Send/Receive Data Task

    alt Receive ACK Packet Success
        activate ACK
        ACK ->> Data: xTaskNotify()
        deactivate ACK
        activate Data
        Data ->> Data: Mark_Send_Success()
        Data ->> Data: Remove_Data_From_Queue()
        alt QueueNotEmpty
            Data ->> Data: ContinueSend()
        end
        deactivate Data
    else Receive ACK Packet Fail
        activate ACK
        ACK ->> Data: xTaskNotify()
        deactivate ACK
        activate Data
        Data ->> Data: Mark_Send_Fail()
        Data ->> Data: Recalculate_Heat()
        Data ->> Data: StopSend()
        deactivate Data
        else Time Out
        activate Data
        Data ->> Data: Mark_Send_Fail()
        Data ->> Data: Recalculate_Heat()
        Data ->> Data: ReSend()
        deactivate Data
    end
```
### 7. Send Data Process After Update Success (in 1 slot):
```mermaid
sequenceDiagram
    participant NodeB as Node B (Send)
    participant NodeA as Node A (Receive)

    activate NodeB
    NodeB ->> NodeA: Forward_Packet _1
    activate NodeA
    NodeA ->> NodeA: Check_Queue_Not_Full
    NodeA ->> NodeB: ACK_Success_1
    deactivate NodeA
    deactivate NodeB
    activate NodeB
    NodeB ->> NodeA: Forward_Packet _2
    activate NodeA
    NodeA ->> NodeA: Check_Queue_Not_Full
    NodeA ->> NodeB: ACK_Success_2
    deactivate NodeA
    deactivate NodeB
    activate NodeB
    NodeB ->> NodeA: Forward_Packet _3
    activate NodeA
    NodeA ->> NodeA: Check_Queue_Full
    NodeA ->> NodeB: ACK_Fail_3
    deactivate NodeA
    NodeB ->> NodeB: Stop_Sending
    deactivate NodeB
```

### 7. Send Data Process After Update Fail:
```mermaid
sequenceDiagram
    participant NodeB as Node B (Send)
    participant NodeA as Node A (Receive)

    activate NodeB
    NodeB ->> NodeA: Forward_Packet _1
    activate NodeA
    NodeA ->> NodeA: Check_Queue_Not_Full
    NodeA ->> NodeB: ACK_Success_1
    deactivate NodeA
    NodeB ->> NodeB: Wait_for_Update_Again
    deactivate NodeB
```
### 8. other Information:
- Who send update first is the one receive Data.
- Heat Value (Percentage to Gateway) calculate by using number of success sent data / number of data have been sent.
- Forwarding Time = the time from the slot receiving data from the source to the slot forwarding this data to another node or gateway.

For example:
<table border="1" cellspacing="0" cellpadding="5" style="text-align:center;">
  <tr>
    <th>Slot 5</th>
    <th>Slot 0</th>
    <th>Slot 1</th>
    <th>Slot 2</th>
    <th>Slot 3</th>
    <th>Slot 4</th>
  </tr>
  <tr>
    <th>Receive Here</th>
    <th>Check</th>
    <th>Check</th>
    <th>Check</th>
    <th>Forward Here</th>
    <th>Check</th>
  </tr>
<table>
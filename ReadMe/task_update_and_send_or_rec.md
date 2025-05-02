### 1. Seuquence Diagram for Receive Data in Gateway (Data from Node B):

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
            activate Data
            Data ->> Data: Update_Data()
            Data ->> Data: PrepareACKPacketSuccessMessage()
            Data ->> Send: xQueueSend()
            deactivate Data
            activate Send
            Send ->> Node B: ACKPacketSuccessMessage()
            deactivate Send
        end
    end
```

### 2. Seuquence Diagram for Receive ACK Success Message Handler Task receive xTaskNotify() From ACK Receive UART6 Task or not receive any ACK Success:

```mermaid
sequenceDiagram
    participant ACK as ACK Message Handler Task
    participant Data as Send/Receive Data Task
    loop Until Send Data Timer End
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
        else Time Out
            activate Data
            Data ->> Data: Mark_Send_Fail()
            Data ->> Data: ReSend()
            deactivate Data
        end
    end
```
### 3. Send Data Process After Update Success (in 1 slot):
```mermaid
sequenceDiagram
    participant NodeB as Node B (Send)
    participant Gateway as Gateway (Receive)
    activate NodeB
    NodeB ->> Gateway: Forward_Packet _1
    activate Gateway
    Gateway ->> Gateway: Check_Message_And_Update
    Gateway ->> NodeB: ACK_Success_1
    deactivate Gateway
    deactivate NodeB
    activate NodeB
    NodeB ->> Gateway: Forward_Packet _2
    activate Gateway
    Gateway ->> Gateway: Check_Message_And_Update
    Gateway ->> NodeB: ACK_Success_2
    deactivate Gateway
    deactivate NodeB
    activate NodeB
    NodeB -x Gateway: Forward_Packet _3
    NodeB ->> NodeB: Time_out
    NodeB ->> NodeB: Resend_Packet_3
    NodeB ->> Gateway: Forward_Packet_3
    activate Gateway
    Gateway ->> Gateway: Check_Message_And_Update
    Gateway ->> NodeB: ACK_Success_3
    deactivate Gateway
    deactivate NodeB

```
### 4. Flowchart: Send And Receive Data Between Node and Gateway
```mermaid
flowchart TD
Start([Start]) --> DataTimer[Start Send Data Timeout Timer] --> Send[Send/Resend Forward Packet Message and Start Receive ACK Timeout Timer]
Send --> Wait[Wait ACK Packet to Gateway Success Message]
Wait -- Receive ACK Timeout --> Check{"Data Timer<br/>Timeout?"} -- Yes --> End([Done])
Wait -- Receive ACK Success --> Check -- No & ACK Success --> Check2{"Is Data Buffer<br/>Empty?"}
Check -- No & ACK Timeout --> Send
Check2 -- No -->Send
Check2 -- Yes --> End
```
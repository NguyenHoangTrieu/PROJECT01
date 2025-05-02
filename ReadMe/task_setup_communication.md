# **Sequence Diagram for Setup communication between Node with Gateway:**

###  1. Sequence Diagram for Receive Free Slot Message:

```mermaid
sequenceDiagram
    participant ISR as USART6 ISR
    participant Receive as Receive UART6 Task
    participant FreeSlot as FreeSlot Handler Task
    participant Setup as SetupCommunication Task

    activate ISR
    ISR ->>Receive: xTaskNotifyFromISR()
    deactivate ISR
    activate Receive
    
    alt MessageType = FreeSlot
        Receive ->> Receive: ReadMessage()
        Receive ->> Receive: ParseMessage()
        Receive ->> FreeSlot: xTaskNotify()
        deactivate Receive
        
        activate FreeSlot
        FreeSlot ->> FreeSlot: CheckMessageValid()
        alt Message valid
            FreeSlot ->> Setup: xTaskNotify()
            deactivate FreeSlot
            activate Setup
            deactivate Setup
        end
    end
```
#### This Sequence Diagram is same when receive Request Slot Message, ACK Request Success Message, ACK Reuqest Fail Message, Update HEAT Value Message.

### 2. Sequence Diagram for Send Request Slot Message:

```mermaid
sequenceDiagram
    participant Setup as SetupCommunication Task
    participant Send as SendUART6 Task

    activate Setup
    Setup ->> Setup: PrepareRequestSlotMessage()
    deactivate Setup
    alt SlotIsNotSetAndAvaiable
    loop Until Receive ACK Request Slot Message or request over n time
        Setup ->> Send: xQueueSend()
        activate Send
        Send ->> Send: SendRequestSlotMessage()
        deactivate Send
    end
    end
```

### 3. Sequence Diagram for Handle Request Slot Message in Gateway and Receive ACK in Node B:
```mermaid
sequenceDiagram
    participant Setup as SetupCommunication Task in Gateway
    participant Send as SendUART6 Task
    participant NodeB as Node B
    activate Setup
    Setup ->> Setup: CheckRequestSlotMessage()
    alt Slot Is Not Set
        Setup ->> Send: xQueueSend()
        deactivate Setup
        activate Send
        Send ->> NodeB: SendACKRequestSlotSuccessMessage()
        deactivate Send
        activate NodeB
        NodeB ->> NodeB: MarkSlotIsSet_And_SetGatewayInf()
        deactivate NodeB
    else Slot Is Set
        activate Setup
        Setup ->> Send: xQueueSend()
        deactivate Setup
        activate Send
        Send ->> NodeB: SendACKRequestSlotFailMessage()
        deactivate Send
        activate NodeB
        NodeB ->> NodeB: MarkSlotIsUnavailable()
        deactivate NodeB
    end
```
### Flowchart: Setup Communication Between Node and Gateway

```mermaid
flowchart TD
  Start([Start]) --> Wait[Wait Free Slot Message]
  Wait --> Check{Received?}
  Check -- Yes --> Create[Create Request Slot Message and Create Resend Timer]
  Create --> Send[Send Request Slot Message]
  Check -- No --> Wait
  Send --> Wait_ACK[Wait for ACK Request Slot Success Message]
  Wait_ACK -- Receive ACK Request Slot Fail --> MarkNot[Mark this slot is unavailable for communication] --> Wait
  Wait_ACK -- Receive ACK Request Slot Success --> MarkSet[Mark Slot Is Setup and Update Gateway Information] --> End([Done])
  Wait_ACK -- Resend Timer Timeout --> CheckResend{"Is request<br/>over n time?"}
  CheckResend -- No --> Send
  CheckResend -- Yes --> Wait
```
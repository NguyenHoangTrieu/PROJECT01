#include "LoRa_AS32_lib.h"
void Operation_MODE_Init(GPIO_TypeDef *GPIOmd0, GPIO_TypeDef *GPIOmd1,
    uint16_t GPIO_Pin_MD0, uint16_t GPIO_Pin_MD1,
    OPERATION_MODE Mode){
    switch (Mode)
    {
    case GENERAL_MODE:
        HAL_GPIO_WritePin(GPIOmd1, GPIO_Pin_MD1, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOmd0, GPIO_Pin_MD0, GPIO_PIN_RESET);
        break;
    case POWE_SAVING_MODE:
        HAL_GPIO_WritePin(GPIOmd1, GPIO_Pin_MD1, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOmd0, GPIO_Pin_MD0, GPIO_PIN_SET);
        break;
    case WAKE_UP_MODE:
        HAL_GPIO_WritePin(GPIOmd1, GPIO_Pin_MD1, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOmd0, GPIO_Pin_MD0, GPIO_PIN_RESET);
        break;
    case SLEEP_MODE:
        HAL_GPIO_WritePin(GPIOmd1, GPIO_Pin_MD1, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOmd0, GPIO_Pin_MD0, GPIO_PIN_SET);
        break;
    default:
        break;
    }
}
void WriteNormalCommand(UART_HandleTypeDef huartx, NORMAL_COMMAND CommandType){
    HAL_UART_DeInit(&huartx);
    huartx.Init.BaudRate = 9600;
    HAL_UART_Init(&huartx);
    switch (CommandType)
    {
    case READ_CONFIG_PARAMETERS:
        HAL_UART_Transmit(&huartx, (uint8_t *)"\xC1\xC1\xC1", 3, 100);
        break;
    case READ_MODULE_VERSION:
        HAL_UART_Transmit(&huartx, (uint8_t *)"\xC3\xC3\xC3", 3, 100);
        break;
    case RESET_MODULE:
        HAL_UART_Transmit(&huartx, (uint8_t *)"\xC4\xC4\xC4", 3, 100);
        break;
    case READ_VOLTAGE:
        HAL_UART_Transmit(&huartx, (uint8_t *)"\xC5\xC5\xC5", 3, 100); // not working
        break;
    case RESTORE_DEFAULT_PARAMETERS:
        HAL_UART_Transmit(&huartx, (uint8_t *)"\xC9\xC9\xC9", 3, 100);
        break;
    case HANDSHAKE_CMD:
        HAL_UART_Transmit(&huartx, (uint8_t *)"\xE1\xE1\xE1", 3, 100);
        break;
    case READ_SOFTWARE_VERSION:
        HAL_UART_Transmit(&huartx, (uint8_t *)"\xF3\xF3\xF3", 3, 100);
        break;
    case READ_RSSI_DATA_SIGNAL:
        HAL_UART_Transmit(&huartx, (uint8_t *)"\xAF\xAF\x73\x00\xAF\xF3", 6, 100); // not working
        break;
    case READ_RSSI_ENVIRONMENTAL_SIGNAL:
        HAL_UART_Transmit(&huartx, (uint8_t *)"\xAF\xAF\x74\x00\xAF\xF4", 6, 100); //not working
        break;
    default:
        break;
   }
}

void WriteConfigParameters(UART_HandleTypeDef huartx, CONFIG_COMMAND ConfigType,
    uint8_t ADDH, uint8_t ADDL, AS32_UART_PARITY UART_Parity,
    UART_BPS_TYPE UART_BPS, AIR_DATA_RATE AirDataRate,
    uint8_t CHAN, FIXED_TRANSMISSION FixedTransmission,
    IO_DRIVE_MODE IODriveMode,
    WIRELESS_WAKE_UP_TIME WirelessWakeUpTime,
    TRANSMISSION_POWER TransmissionPower){
    HAL_UART_DeInit(&huartx);
    huartx.Init.BaudRate = 9600;
    HAL_UART_Init(&huartx);
    uint8_t ConfigData[6];
    ConfigData[1] = ADDH;
    ConfigData[2] = ADDL;
    ConfigData[3] = (UART_Parity << 6) | (UART_BPS << 3) | AirDataRate;
    ConfigData[4] = CHAN;
    ConfigData[5] = (FixedTransmission << 7) | (IODriveMode << 6) | (WirelessWakeUpTime << 3) | TransmissionPower;
    switch (ConfigType)
    {
    huartx.Init.BaudRate = 9600;
    case WRITE_SAVED_CONFIG_PARAMETERS:
        ConfigData[0] = 0xC0;
        HAL_UART_Transmit(&huartx, ConfigData, 6, 100);
        break;
    case WRITE_CONFIG_PARAMETERS:
        ConfigData[0] = 0xC2;
        HAL_UART_Transmit(&huartx, ConfigData, 6, 100);
        break;
    default:
        break;
    }
}
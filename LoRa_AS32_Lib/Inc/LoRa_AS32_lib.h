#ifndef __LoRa_AS32_LIB_H
#define __LoRa_AS32_LIB_H
#include "stm32f4xx_hal.h"
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum
{
    UART_BPS_1200 = 0b000,
    UART_BPS_2400 = 0b001,
    UART_BPS_4800 = 0b010,
    UART_BPS_9600 = 0b011,
    UART_BPS_19200 = 0b100,
    UART_BPS_38400 = 0b101,
    UART_BPS_57600 = 0b110,
    UART_BPS_115200 = 0b111
} UART_BPS_TYPE;
typedef enum
{
    MODE_00_8N1 = 0b00,
    MODE_01_8O1 = 0b01,
    MODE_10_8E1 = 0b10,
    MODE_11_8N1 = 0b11
} AS32_UART_PARITY;
typedef enum
{
    AIR_DATA_RATE_000_03 = 0b000,
    AIR_DATA_RATE_001_12 = 0b001,
    AIR_DATA_RATE_010_24 = 0b010,
    AIR_DATA_RATE_011_48 = 0b011,
    AIR_DATA_RATE_100_96 = 0b100,
    AIR_DATA_RATE_101_192 = 0b101
} AIR_DATA_RATE;
typedef enum
{
    POWER_20 = 0b00,
    POWER_17 = 0b01,
    POWER_14 = 0b10,
    POWER_10 = 0b11
} TRANSMISSION_POWER;
typedef enum
{
    WAKE_UP_250 = 0b000,
    WAKE_UP_500 = 0b001,
    WAKE_UP_750 = 0b010,
    WAKE_UP_1000 = 0b011,
    WAKE_UP_1250 = 0b100,
    WAKE_UP_1500 = 0b101,
    WAKE_UP_1750 = 0b110,
    WAKE_UP_2000 = 0b111
} WIRELESS_WAKE_UP_TIME;
typedef enum
{
    TRANSPARENT_TRANSMISSION = 0,
    P_TO_P_TRANSMISSION = 1
} FIXED_TRANSMISSION;
typedef enum
{
    IO_D_MODE_OPEN_COLLECTOR = 0b0,
    IO_D_MODE_PUSH_PULLS_PULL_UPS = 0b1
} IO_DRIVE_MODE;
typedef enum
{
    READ_CONFIG_PARAMETERS = 0,
    READ_MODULE_VERSION = 1,
    RESET_MODULE = 2,
    READ_VOLTAGE = 3,
    RESTORE_DEFAULT_PARAMETERS = 4,
    HANDSHAKE_CMD = 5,
    READ_SOFTWARE_VERSION = 6,
    READ_RSSI_DATA_SIGNAL = 7,
    READ_RSSI_ENVIRONMENTAL_SIGNAL = 8
} NORMAL_COMMAND;
typedef enum
{
    WRITE_SAVED_CONFIG_PARAMETERS = 0,
    WRITE_CONFIG_PARAMETERS = 1
} CONFIG_COMMAND;
typedef enum{
    GENERAL_MODE = 0,
    POWE_SAVING_MODE = 1,
    WAKE_UP_MODE = 2,
    SLEEP_MODE = 3
} OPERATION_MODE;
void Operation_MODE_Init(GPIO_TypeDef *GPIOmd0, GPIO_TypeDef *GPIOmd1,
                        uint16_t GPIO_Pin_MD0, uint16_t GPIO_Pin_MD1,
                        OPERATION_MODE Mode);

void WriteConfigParameters(UART_HandleTypeDef huartx, CONFIG_COMMAND ConfigType,
                            uint8_t ADDH, uint8_t ADDL, AS32_UART_PARITY UART_Parity,
                            UART_BPS_TYPE UART_BPS, AIR_DATA_RATE AirDataRate,
                            uint8_t CHAN, FIXED_TRANSMISSION FixedTransmission,
                            IO_DRIVE_MODE IODriveMode,
                            WIRELESS_WAKE_UP_TIME WirelessWakeUpTime,
                            TRANSMISSION_POWER TransmissionPower);
void EncryptKey(UART_HandleTypeDef huartx, uint16_t *Key);
void WriteNormalCommand(UART_HandleTypeDef huartx, NORMAL_COMMAND CommandType);
#ifdef __cplusplus
}
#endif
#endif
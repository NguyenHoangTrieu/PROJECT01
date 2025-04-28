#ifndef HEATCONFIG_H
#define HEATCONFIG_H
// define the device type
#define GATEWAY         0
#define NODE            1
#define TEST_NODE       2
#define DEVICE_TYPE     NODE
#if DEVICE_TYPE == GATEWAY
#define NIC_BIT_1       0x00
#define NIC_BIT_2       0x00
#define NIC_BIT_3       0x01
#elif DEVICE_TYPE == NODE
#define NIC_BIT_1       0x00
#define NIC_BIT_2       0x00
#define NIC_BIT_3       0x02
#else
#define NIC_BIT_1       0x00
#define NIC_BIT_2       0x00
#define NIC_BIT_3       0x04
#endif
#define BEGIN_TIME      250000 // sooner 2 seconds than Node 2 (flash time)
/*----------------------*/
#endif
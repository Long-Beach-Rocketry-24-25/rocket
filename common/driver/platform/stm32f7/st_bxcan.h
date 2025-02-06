/**
 * 
 * 
 */

#pragma once

#include "stm32f7xx.h"

#include "can.h"
#include "gpio.h"
#include "timeout.h"

#define CAN_NUM_TX_MAILBOXES 3
#define CAN_NUM_RX_MAILBOXES 2
#define CAN_MAX_DATA_BYTES 8

typedef struct
{
    uint8_t sjw;
    uint8_t ts1;
    uint8_t ts2;
    uint16_t brp;
} CanBitTiming;

typedef struct
{
    CAN_TypeDef* instance;
    CanId id;
    uint8_t tx_box;
    uint8_t rx_box;
    Timeout* timer;
    CanBitTiming bit_timing;
    Gpio rx;
    Gpio tx;
    bool remote;
} StPrivBxCan;

typedef struct
{
    StPrivBxCan priv;
    uint32_t base_addr;
    uint8_t tx_box;
    uint8_t rx_box;
    CanBitTiming timing;
    CanId id;
} StBxCanParams;

bool StBxCanInit(CanBus* can, StBxCanParams* params, Timeout* timer);
void StBxCanConfig(CanBus* can);
bool StBxCanSend(CanBus* can, uint8_t* data, size_t size);
bool StBxCanRecv(CanBus* can, uint8_t* data, size_t size);
void StBxCanSetId(CanBus* can, CanId new_id);

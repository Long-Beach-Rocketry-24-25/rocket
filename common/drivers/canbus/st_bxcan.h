/**
 * 
 * 
 */

#pragma once

#include "gpio.h"
#include "can.h"
#include "st_bx_can_def.h"
#include "timeout.h"

#define CAN_NUM_TX_MAILBOXES 3
#define CAN_NUM_RX_MAILBOXES 2
#define CAN_MAX_DATA_BYTES 8

/**
 * @brief Controller Area Network TxMailBox
 */
typedef struct
{
    volatile uint32_t TIR;  /*!< CAN TX mailbox identifier register */
    volatile uint32_t TDTR; /*!< CAN mailbox data length control and time stamp register */
    volatile uint32_t TDLR; /*!< CAN mailbox data low register */
    volatile uint32_t TDHR; /*!< CAN mailbox data high register */
} ST_CAN_TxMailBox_TypeDef;

/**
 * @brief Controller Area Network FIFOMailBox
 */
typedef struct
{
    volatile uint32_t RIR;  /*!< CAN receive FIFO mailbox identifier register */
    volatile uint32_t RDTR; /*!< CAN receive FIFO mailbox data length control and time stamp register */
    volatile uint32_t RDLR; /*!< CAN receive FIFO mailbox data low register */
    volatile uint32_t RDHR; /*!< CAN receive FIFO mailbox data high register */
} ST_CAN_FIFOMailBox_TypeDef;

/**
 * @brief Controller Area Network FilterRegister
 */
typedef struct
{
    volatile uint32_t FR1; /*!< CAN Filter bank register 1 */
    volatile uint32_t FR2; /*!< CAN Filter bank register 1 */
} ST_CAN_FilterRegister_TypeDef;

/**
 * @brief Controller Area Network
 */
typedef struct
{
    volatile uint32_t              MCR;                 /*!< CAN master control register,         Address offset: 0x00          */
    volatile uint32_t              MSR;                 /*!< CAN master status register,          Address offset: 0x04          */
    volatile uint32_t              TSR;                 /*!< CAN transmit status register,        Address offset: 0x08          */
    volatile uint32_t              RF0R;                /*!< CAN receive FIFO 0 register,         Address offset: 0x0C          */
    volatile uint32_t              RF1R;                /*!< CAN receive FIFO 1 register,         Address offset: 0x10          */
    volatile uint32_t              IER;                 /*!< CAN interrupt enable register,       Address offset: 0x14          */
    volatile uint32_t              ESR;                 /*!< CAN error status register,           Address offset: 0x18          */
    volatile uint32_t              BTR;                 /*!< CAN bit timing register,             Address offset: 0x1C          */
    uint32_t                   RESERVED0[88];       /*!< Reserved, 0x020 - 0x17F                                            */
    ST_CAN_TxMailBox_TypeDef      sTxMailBox[3];       /*!< CAN Tx MailBox,                      Address offset: 0x180 - 0x1AC */
    ST_CAN_FIFOMailBox_TypeDef    sFIFOMailBox[2];     /*!< CAN FIFO MailBox,                    Address offset: 0x1B0 - 0x1CC */
    uint32_t                   RESERVED1[12];       /*!< Reserved, 0x1D0 - 0x1FF                                            */
    volatile uint32_t              FMR;                 /*!< CAN filter master register,          Address offset: 0x200         */
    volatile uint32_t              FM1R;                /*!< CAN filter mode register,            Address offset: 0x204         */
    uint32_t                   RESERVED2;           /*!< Reserved, 0x208                                                    */
    volatile uint32_t              FS1R;                /*!< CAN filter scale register,           Address offset: 0x20C         */
    uint32_t                   RESERVED3;           /*!< Reserved, 0x210                                                    */
    volatile uint32_t              FFA1R;               /*!< CAN filter FIFO assignment register, Address offset: 0x214         */
    uint32_t                   RESERVED4;           /*!< Reserved, 0x218                                                    */
    volatile uint32_t              FA1R;                /*!< CAN filter activation register,      Address offset: 0x21C         */
    uint32_t                   RESERVED5[8];        /*!< Reserved, 0x220-0x23F                                              */
    ST_CAN_FilterRegister_TypeDef sFilterRegister[28]; /*!< CAN Filter Register,                 Address offset: 0x240-0x31C   */
} ST_CAN_TypeDef;

typedef struct
{
    uint8_t sjw;
    uint8_t ts1;
    uint8_t ts2;
    uint16_t brp;
} CanBitTiming;

typedef struct
{
    ST_CAN_TypeDef* instance;
    CanId id;
    uint8_t tx_box;
    uint8_t rx_box;
    Timeout* timer;
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

bool St_BxCan_Init(CanBus *can, StBxCanParams *params, Timeout *timer);

void St_BxCan_Config(CanBus *can);

bool St_BxCan_Send(CanBus *can, uint8_t *data, size_t size);

bool St_BxCan_Recv(CanBus *can, uint8_t *data, size_t size);

void St_BxCan_Set_Id(CanBus *can, CanId new_id);

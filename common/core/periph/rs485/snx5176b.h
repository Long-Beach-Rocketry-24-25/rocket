/**
 * SNX5176B RS485 Transceiver Driver. Uses a UART driver
 * instance and two GPIO driver instances, and implements
 * a UART driver interface itself.
 */

#pragma once

#include "gpio.h"
#include "ring_buffer.h"
#include "usart.h"

/**
 * SNx5176B Transceiver Instance.
 */
typedef struct
{
    /**
     * A UART interface to send data over.
     */
    Usart usart;

    /**
     * A GPIO for enabling TX mode on the transceiver.
     */
    Gpio txe;

    /**
     * A GPIO for enabling RX mode on the transceiver.
     */
    Gpio rxe;

} Snx5176b;

/**
 * Initializes a UART interface with a SNx5176B instance.
 * 
 * @param rs485 the UART interface to initialize.
 * @param chip the driver instance to use.
 */
void Snx5176bInit(Usart* rs485, Snx5176b* chip);

/**
 * Configures GPIO for a SNx5176B instance. Starts in RX mode.
 * 
 * @param dev the chip instance to configure for.
 */
void Snx5176bConfig(Snx5176b* dev);

/**
 * Sends bytes of data through the RS485 transceiver.
 * 
 * @param rs485 the UART interface for the bus line.
 * @param data bytes of data to send.
 * @param size number of bytes to send.
 * 
 * @return True on success, false otherwise.
 */
bool Snx5176bSend(Usart* rs485, const uint8_t* data, size_t size);

/**
 * Receives bytes of data through the RS485 transceiver.
 * 
 * @param rs485 the UART interface for the bus line.
 * @param data the buffer to receive data into.
 * @param size the number of bytes to receive.
 * 
 * @return True on success, false otherwise.
 */
bool Snx5176bRecv(Usart* rs485, uint8_t* data, size_t size);

/**
 * Sets the mode of the transceiver.
 * 
 * @param enable True for TX mode, false for RX mode.
 */
void Snx5176bTxSetEn(Snx5176b* chip, bool enable);

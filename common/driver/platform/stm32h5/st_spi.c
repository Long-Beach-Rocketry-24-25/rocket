
#include "st_spi.h"

typedef volatile uint8_t * DataReg;

void StSpiInit(Spi *spi, StPrivSpi *st_spi, uint32_t base_addr, Timeout *timer)
{
    st_spi->instance = (SPI_TypeDef *) base_addr;
    st_spi->timer = timer;
    spi->priv = (void *) st_spi;
    spi->send = StSpiSend;
    spi->read = StSpiRead;
    spi->transact = StSpiTransact;
}

void StSpiConfig(Spi * spi)
{
    StPrivSpi * dev = (StPrivSpi *) spi->priv;

    dev->mosi.config(&dev->mosi);
    dev->miso.config(&dev->miso);
    dev->scl.config(&dev->scl);

    dev->instance->CR1 &= ~SPI_CR1_SPE;
    // SSI bit usage off
    dev->instance->CR1 = SPI_CR1_SSI;

    /**
     * Disable SPI and turn off: CRC, DMA, FTHLVL = 1-data
     * 8 bit frames
	 * Set bitrate: freq pclk / 2 ^ (BR + 1)
	 */
    dev->instance->CFG1 = (0x7 << SPI_CFG1_DSIZE_Pos) | (0x7 << SPI_CFG1_CRCSIZE_Pos) | SPI_CFG1_MBR;

    // CPOL/CPHA = 0, full-duplex,
    dev->instance->CFG2 = SPI_CFG2_SSM | SPI_CFG2_MASTER;

    dev->instance->I2SCFGR &= ~SPI_I2SCFGR_I2SMOD;

    dev->instance->CFG2 |= SPI_CFG2_AFCNTR;
}

bool StSpiSend(Spi * spi, uint8_t *data, size_t size)
{
    StPrivSpi * dev = (StPrivSpi *) spi->priv;

    dev->instance->CR1 &= ~SPI_CR1_SPE;
    dev->instance->CR2 |= (size << SPI_CR2_TSIZE_Pos) & SPI_CR2_TSIZE;
    dev->instance->CR1 |= SPI_CR1_SPE;
    dev->instance->CR1 |= SPI_CR1_CSTART;

    for (size_t i = 0; i < size;)
    {
        // Wait for space in TX FIFO
		WAIT(dev->timer, dev->instance->SR & SPI_SR_TXP, false);

		/**
         * Send: Fill TXFIFO (by writing to SPI_DR)
         * When transmission is enabled, seq begins and continues
         * while data present in FIFO. CLK ends when FIFO becomes empty
         */
        *(DataReg)&dev->instance->TXDR = data[i++];

        // Wait for RX FIFO to fill
		WAIT(dev->timer, dev->instance->SR & SPI_SR_RXP, false);

		/**
         * Read access to SPI_DR returns oldest value stored in RXFIFO not yet read
         * Write access to SPI_DR stores data in TXFIFO at end of queue
         * Must be aligned with RXFIFO threshold conf by FRXTH, FTLVL and FRLVL indicate FIFO occupancy
         */
		uint8_t dummy = *(DataReg)&dev->instance->RXDR;
	}

    WAIT(dev->timer, !(dev->instance->SR & SPI_SR_EOT), false);

    dev->instance->CR1 &= ~SPI_CR1_SPE;

    return true;
}

bool StSpiRead(Spi * spi, uint8_t *data, size_t size)
{
    StPrivSpi * dev = (StPrivSpi *) spi->priv;

    dev->instance->CR1 &= ~SPI_CR1_SPE;
    dev->instance->CR2 |= (size << SPI_CR2_TSIZE_Pos) & SPI_CR2_TSIZE;
    dev->instance->CR1 |= SPI_CR1_SPE;
    dev->instance->CR1 |= SPI_CR1_CSTART;

    for (size_t i = 0; i < size;)
    {
        // Wait for space in TX FIFO
		WAIT(dev->timer, dev->instance->SR & SPI_SR_TXP, false);

		/**
         * Send: Fill TXFIFO (by writing to SPI_DR)
         * When transmission is enabled, seq begins and continues
         * while data present in FIFO. CLK ends when FIFO becomes empty
         */
        *(DataReg)&dev->instance->TXDR = 0;

        // Wait for RX FIFO to fill
		WAIT(dev->timer, dev->instance->SR & SPI_SR_RXP, false);

		/**
         * Read access to SPI_DR returns oldest value stored in RXFIFO not yet read
         * Write access to SPI_DR stores data in TXFIFO at end of queue
         * Must be aligned with RXFIFO threshold conf by FRXTH, FTLVL and FRLVL indicate FIFO occupancy
         */
		data[i++] = *(DataReg)&dev->instance->RXDR;
	}

    WAIT(dev->timer, !(dev->instance->SR & SPI_SR_EOT), false);

    dev->instance->CR1 &= ~SPI_CR1_SPE;

    return true;
}

bool StSpiTransact(Spi * spi, uint8_t *txdata, uint8_t *rxdata, size_t size)
{
    StPrivSpi * dev = (StPrivSpi *) spi->priv;

    dev->instance->CR1 &= ~SPI_CR1_SPE;
    dev->instance->CR2 |= (size << SPI_CR2_TSIZE_Pos) & SPI_CR2_TSIZE;
    dev->instance->CR1 |= SPI_CR1_SPE;
    dev->instance->CR1 |= SPI_CR1_CSTART;

    for (size_t i = 0; i < size;)
    {
        // Wait for space in TX FIFO
		WAIT(dev->timer, dev->instance->SR & SPI_SR_TXP, false);

		/**
         * Send: Fill TXFIFO (by writing to SPI_DR)
         * When transmission is enabled, seq begins and continues
         * while data present in FIFO. CLK ends when FIFO becomes empty
         */
        *(DataReg)&dev->instance->TXDR = txdata[i];

        // Wait for RX FIFO to fill
		WAIT(dev->timer, dev->instance->SR & SPI_SR_RXP, false);

		/**
         * Read access to SPI_DR returns oldest value stored in RXFIFO not yet read
         * Write access to SPI_DR stores data in TXFIFO at end of queue
         * Must be aligned with RXFIFO threshold conf by FRXTH, FTLVL and FRLVL indicate FIFO occupancy
         */
		rxdata[i++] = *(DataReg)&dev->instance->RXDR;
	}

    WAIT(dev->timer, dev->instance->SR & SPI_SR_EOT, false);

    dev->instance->CR1 &= ~SPI_CR1_SPE;

    return true;
}

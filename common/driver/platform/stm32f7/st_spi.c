
#include "st_spi.h"

typedef volatile uint8_t * DataReg;

void StSpiInit(Spi *spi, StPrivSpi *st_spi, uint32_t base_addr, Timeout *timer)
{
    st_spi->instance = (SPI_TypeDef *) base_addr;
    st_spi->timer = timer;
    spi->priv = (void *) st_spi;
    spi->transact = StSpiTransact;
}

void StSpiConfig(Spi * spi)
{
    StPrivSpi * dev = (StPrivSpi *) spi->priv;

    dev->mosi.config(&dev->mosi);
    dev->miso.config(&dev->miso);
    dev->scl.config(&dev->scl);

    // Disable SPI and turn off: Bidirectional mode, CRC, RX only
    // set CPOL/CPHA = 0, 8-bit
    dev->instance->CR1 &= 0;

    // SSI bit usage off
    dev->instance->CR1 |= (SPI_CR1_SSM | SPI_CR1_SSI); 

    // 0 = MSB first 1 = LSB first
    dev->instance->CR1 &= ~SPI_CR1_LSBFIRST;

    /** 
     * Set bitrate: freq pclk / 2 ^ (BR + 1)
     * 72/32 = 2.25MHz, 84/32 = 2.625MHz
     */
    dev->instance->CR1 |= SPI_CR1_BR;

    dev->instance->CR1 |= SPI_CR1_MSTR; // Master mode

    // Reset reg to set motorola mode, disable SS
    dev->instance->CR2 &= 0;
    dev->instance->CR2 |= SPI_CR2_FRXTH; // 0 = 16 bit 1 = 8bit
    dev->instance->CR2 |= (SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2); // 8bit, 0111

    // Enable SPI
    dev->instance->CR1 |= SPI_CR1_SPE;

    dev->cs.deselect(&dev->cs);
}

static inline bool transfer(StPrivSpi *dev, uint8_t *txdata, uint8_t *rxdata, size_t size)
{
    for (size_t i = 0; i < size;)
    {
        // Wait for space in TX FIFO
		WAIT(dev->timer, dev->instance->SR & SPI_SR_TXE, false);

		/**
         * Send: Fill TXFIFO (by writing to SPI_DR)
         * When transmission is enabled, seq begins and continues
         * while data present in FIFO. CLK ends when FIFO becomes empty
         */
        *(DataReg)&dev->instance->DR = txdata[i];

        // Wait for RX FIFO to fill
		WAIT(dev->timer, dev->instance->SR & SPI_SR_RXNE, false);

		/** 
         * Read access to SPI_DR returns oldest value stored in RXFIFO not yet read
         * Write access to SPI_DR stores data in TXFIFO at end of queue
         * Must be aligned with RXFIFO threshold conf by FRXTH, FTLVL and FRLVL indicate FIFO occupancy
         */
		rxdata[i++] = *(DataReg)&dev->instance->DR;
	}

    return true;
}

bool StSpiTransact(Spi * spi, uint8_t *txdata, uint8_t *rxdata, size_t size)
{
    StPrivSpi * dev = (StPrivSpi *) spi->priv;

	// Check if line is busy, or if CS fails
	if (dev->instance->SR & SPI_SR_BSY || !dev->cs.select(&dev->cs))
    {
		return false;
	}

    bool success = transfer(dev, txdata, rxdata, size);

    WAIT(dev->timer, !(dev->instance->SR & SPI_SR_BSY),
         dev->cs.deselect(&dev->cs) && success);
}

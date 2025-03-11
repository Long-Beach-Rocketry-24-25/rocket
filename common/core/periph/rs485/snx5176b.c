
#include "snx5176b.h"

void Snx5176bInit(Usart* rs485, Snx5176b* chip)
{
    rs485->priv = (void*)chip;
    rs485->send = Snx5176bSend;
    rs485->recv = Snx5176bRecv;
}

void Snx5176bConfig(Snx5176b* dev)
{
    dev->txe.config(&dev->txe);
    dev->rxe.config(&dev->rxe);
    Snx5176bTxSetEn(dev, false);
}

bool Snx5176bSend(Usart* rs485, const uint8_t* data, size_t size)
{
    Snx5176b* dev = (Snx5176b*)rs485->priv;
    Snx5176bTxSetEn(dev, true);
    bool success = dev->usart.send(&dev->usart, data, size);
    Snx5176bTxSetEn(dev, false);
    return success;
}

bool Snx5176bRecv(Usart* rs485, uint8_t* data, size_t size)
{
    Snx5176b* dev = (Snx5176b*)rs485->priv;
    return dev->usart.recv(&dev->usart, data, size);
}

void Snx5176bTxSetEn(Snx5176b* chip, bool enable)
{
    chip->rxe.set(&chip->rxe, enable);
    chip->txe.set(&chip->txe, enable);
}

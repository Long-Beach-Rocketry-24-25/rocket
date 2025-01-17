
#include "arm32_usart.h"


bool Arm32UsartInit(Usart *usart, Arm32PrivUsart *arm32_usart, const char* path)
{
    int fd = open(path, O_RDWR | O_NDELAY | O_NOCTTY);
    arm32_usart->file_handle = fd;
    if (fd < 0)
    {
        return false;
    }

    struct termios options;
    tcgetattr(fd, &options);
    options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &options);
    usart->priv = (void *) arm32_usart;
    usart->send = Arm32UsartSend;
    usart->recv = Arm32UsartRecv;

    return true;
}

void Arm32UsartClose(Usart *usart)
{
    Arm32PrivUsart * dev = (Arm32PrivUsart *) usart->priv;
    close(dev->file_handle);
}

bool Arm32UsartSend(Usart *usart, uint8_t *data, size_t size)
{
    Arm32PrivUsart * dev = (Arm32PrivUsart *) usart->priv;
    if (dev->file_handle < 0)
    {
        return false;
    }

    return write(dev->file_handle, data, size) >= 0;
}

bool Arm32UsartRecv(Usart *usart, uint8_t *data, size_t size)
{
    Arm32PrivUsart * dev = (Arm32PrivUsart *) usart->priv;
    if (dev->file_handle < 0)
    {
        return false;
    }

    return read(dev->file_handle, data, size) >= 0;
}


#include <pthread.h>
#include <stdio.h>

#include "arm32_usart.h"

Usart usart;
Arm32PrivUsart arm32_usart;
pthread_mutex_t lock;

#define RECV_BUF_SIZE 0x1000

void* receive(void* args)
{
    while (1)
    {
        uint8_t data = 0;
        pthread_mutex_lock(&lock);
        if (usart.recv(&usart, &data, 1))
        {
            printf("%c", data);
        }
        pthread_mutex_unlock(&lock);
    }
}

void* send(void* args)
{
    while (1)
    {
        char msg[RECV_BUF_SIZE];
        scanf("%s", &msg);
        size_t size = 0;
        while (msg[size++] != '\0' && size < (RECV_BUF_SIZE - 1))
            ;
        msg[size - 1] = '\n';
        pthread_mutex_lock(&lock);
        usart.send(&usart, msg, size);
        pthread_mutex_unlock(&lock);
    }
}

int main(int argc, char* argv[])
{
    bool success = false;
    if (argv[1] != NULL)
    {
        printf("Using port: %s\n", argv[1]);
        success = Arm32UsartInit(&usart, &arm32_usart, argv[1]);
    }
    else
    {
        printf("Using port: /dev/serial0\n");
        success = Arm32UsartInit(&usart, &arm32_usart, "/dev/serial0");
    }

    success = success && (pthread_mutex_init(&lock, NULL) == 0);

    if (success)
    {
        printf("Opening FD: %d\n", arm32_usart.file_handle);
        pthread_t sendThread;
        pthread_t recvThread;
        pthread_create(&sendThread, NULL, send, NULL);
        pthread_create(&recvThread, NULL, receive, NULL);
        pthread_join(sendThread, NULL);
        pthread_join(recvThread, NULL);
    }
    else
    {
        printf("Failed to open port!\n");
    }
}
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct net_if net_if;
struct net_if
{
    bool (*send)(uint8_t* buff, size_t size);
    bool (*receive)(uint8_t* buff, size_t size);
};
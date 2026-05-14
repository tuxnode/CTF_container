typedef struct {
    unsigned int size;
    char         data[128];
} Packet;

volatile Packet packet = { .size = 0 };
volatile int    packet_ready = 0;

typedef struct {
    char         buf[64];
    unsigned int auth;
} SafeZone;

/*
 * 这里隐藏了打印flag的函数
 * */

extern void *hack_func(void *arg);

int main(){
    pthread_t hack;
    pthread_create(&hack, NULL, hack_func, NULL);

    SafeZone safe_zone;

    for(;;){
        while(!packet_ready);
        packet_ready = 0;

        unsigned int sz = packet.size; // 指检查了size哦~
        if(sz > 64){
            printf("first_check: 传入了%u太大了,再试试\n", sz);
            continue;
        }
        usleep(1);  // 关键再这里哦~

        memset(&safe_zone, 0, sizeof(safe_zone));
        memcpy(safe_zone.buf, (const void*)packet.data, packet.size); // !!

        if(safe_zone.auth != 0xdeadbeef){
            printf("second_check: 再试试\n");
            continue;
        }
        print_flag();
        exit(0);
    }
    pthread_join(hack, NULL);
    return 0;
}

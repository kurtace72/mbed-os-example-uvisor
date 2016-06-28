#include "fun_bag.h"
#include "uvisor-lib/uvisor-lib.h"
#include "mbed.h"
#include "rtos.h"
#include "main-hw.h"

struct box_context {
    Thread * thread;
    Thread * thread2;
    Thread * thread3;
    uint32_t heartbeat;
    int toggle;
};

static const UvisorBoxAclItem acl[] = {
};

static void led1_main(const void *);

UVISOR_BOX_NAMESPACE(NULL);
UVISOR_BOX_HEAPSIZE(8192);
UVISOR_BOX_MAIN(led1_main, osPriorityNormal, UVISOR_BOX_STACK_SIZE);
UVISOR_BOX_CONFIG(box_led1, acl, UVISOR_BOX_STACK_SIZE, box_context);

static void run_1(const void *)
{
    while (1) {
        const int toggle = uvisor_ctx->toggle;
        uint16_t size = toggle ? 150 : 99;

        uvisor_ctx->toggle = !uvisor_ctx->toggle;
        alloc_wait_free(size, 0);
    }
}

static void led1_main(const void *)
{
    DigitalOut led1(LED1);
    led1 = LED_OFF;
    uvisor_ctx->thread2 = new Thread(run_1);
    uvisor_ctx->thread3 = new Thread(run_1);

    while (1) {
        static const size_t size = 20;

        led1 = !led1;
        ++uvisor_ctx->heartbeat;
        alloc_wait_free(size, 200);
    }
}

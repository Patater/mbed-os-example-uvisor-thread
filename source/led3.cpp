#include "fun_bag.h"
#include "uvisor-lib/uvisor-lib.h"
#include "mbed.h"
#include "rtos.h"
#include "main-hw.h"

struct box_context {
    uint32_t heartbeat;
    int toggle;
};

static const UvisorBoxAclItem acl[] = {
};

static void led3_main(const void *);

UVISOR_BOX_NAMESPACE(NULL);
UVISOR_BOX_HEAPSIZE(3500);
UVISOR_BOX_MAIN(led3_main, osPriorityNormal, 512);
UVISOR_BOX_CONFIG(box_led3, acl, UVISOR_BOX_STACK_SIZE, box_context);

static void run_3(void)
{
    while (1) {
        const int toggle = uvisor_ctx->toggle;
        uint16_t size = toggle ? 150 : 99;
        uint16_t seed = (size << 8) | (uvisor_ctx->heartbeat & 0xFF);

        uvisor_ctx->toggle = !uvisor_ctx->toggle;
        alloc_fill_wait_verify_free(size, seed, 0);
    }
}

static void led3_main(const void *)
{
    osStatus status;
    DigitalOut led3(LED3);
    led3 = LED_OFF;

    static const uint32_t STACK_SIZE = 768;

    unsigned char * stack1 = (unsigned char *) malloc(STACK_SIZE);
    if (stack1 == NULL) {
        puts("stack1 malloc failed\r\n");
        fflush(stdout);
        uvisor_error(USER_NOT_ALLOWED);
    }
    Thread * thread1 = new Thread(osPriorityNormal, STACK_SIZE, stack1);
    status = thread1->start(run_3);
    if (status != osOK) {
        printf("Could not start box_led3 thread1.\r\n");
        uvisor_error(USER_NOT_ALLOWED);
    }

    unsigned char * stack2 = (unsigned char *) malloc(STACK_SIZE);
    if (stack2 == NULL) {
        puts("stack2 malloc failed\r\n");
        fflush(stdout);
        uvisor_error(USER_NOT_ALLOWED);
    }
    Thread * thread2 = new Thread(osPriorityNormal, STACK_SIZE, stack2);
    status = thread2->start(run_3);
    if (status != osOK) {
        printf("Could not start box_led3 thread2.\r\n");
        uvisor_error(USER_NOT_ALLOWED);
    }

    /* Create page-backed allocator. */
    const uint32_t kB = 1024;
    SecureAllocator alloc = secure_allocator_create_with_pages(4*kB, 1*kB);
    /* Prepare the thread definition structure. */
    osThreadDef_t thread_def;
    thread_def.stacksize = 4*kB;
    /* Allocate the stack inside the page allocator! */
    thread_def.stack_pointer = (uint32_t *) secure_malloc(alloc, STACK_SIZE);
    thread_def.tpriority = osPriorityNormal;
    thread_def.pthread = (void (*)(const void *)) &run_3;
    /* Create a thread with the page allocator as heap. */
    osThreadContextCreate(&thread_def, NULL, alloc);


    while (1) {
        static const size_t size = 20;
        uint16_t seed = (size << 8) | (uvisor_ctx->heartbeat & 0xFF);

        led3 = !led3;
        ++uvisor_ctx->heartbeat;
        alloc_fill_wait_verify_free(size, seed, 211);
    }
}

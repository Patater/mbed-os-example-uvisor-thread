/*
 * Copyright (c) 2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "green.h"
#include "blue.h"
#include "runner.h"
#include "uvisor-lib/uvisor-lib.h"
#include "mbed.h"
#include "rtos.h"
#include "main-hw.h"
#include <stdint.h>

/* Create ACLs for main box. */
MAIN_ACL(g_main_acl);


/* Register privleged system hooks. */
UVISOR_EXTERN void SVC_Handler(void);
UVISOR_EXTERN void PendSV_Handler(void);
UVISOR_EXTERN void SysTick_Handler(void);

UVISOR_SET_PRIV_SYS_HOOKS(SVC_Handler, PendSV_Handler, SysTick_Handler, __uvisor_semaphore_post);

/* Enable uVisor. */
UVISOR_SET_MODE_ACL(UVISOR_ENABLED, g_main_acl);

int main(void)
{
    //printf("\r\n***** threaded blinky uvisor-rtos example *****\r\n");
    putc('*', stdout);
    fflush(stdout);

    size_t count = 0;

    /* Startup a few RPC runners. */
    static const uint32_t stack_size = 512;
    Thread * sync_1 = new(std::nothrow) Thread(sync_runner, &main_sync_1, osPriorityNormal, stack_size);
    Thread * sync_2 = new(std::nothrow) Thread(sync_runner, &main_sync_2, osPriorityNormal, stack_size);
    Thread * sync_3 = new(std::nothrow) Thread(sync_runner, &main_sync_3, osPriorityNormal, stack_size);
    Thread * async_1 = new(std::nothrow) Thread(async_runner, &main_async_1, osPriorityNormal, stack_size);
    Thread * async_2 = new(std::nothrow) Thread(async_runner, &main_async_2, osPriorityNormal, stack_size);
    Thread * async_3 = new(std::nothrow) Thread(async_runner, &main_async_3, osPriorityNormal, stack_size);
    if (sync_1 == NULL || sync_2 == NULL || sync_3 == NULL || async_1 == NULL || async_2 == NULL || async_3 == NULL) {
        uvisor_error(USER_NOT_ALLOWED);
    }

    while (1)
    {
#define STATS_ENABLED 0
#if STATS_ENABLED
        uint32_t ticks_can_suspend = os_suspend();
        puts("\r\n>---------------------------------------\r\n");
        printf("ticks_can_suspend: %lu\r\n", ticks_can_suspend);
        printf("green_handler_a_1: %u\r\n", green_handler_a_1.num_handled);
        printf("green_handler_a_2: %u\r\n", green_handler_a_2.num_handled);
        printf("green_handler_ab_1: %u\r\n", green_handler_ab_1.num_handled);
        printf("green_handler_c_1: %u\r\n", green_handler_c_1.num_handled);
        puts("<---------------------------------------\r\n");
        os_resume(0); /* Pretend no ticks went by while suspended. */
#endif

        /* Spin forever. */
        Thread::wait(10000);
    }

    return 0;
}

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
#include "runner_context.h"
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

/* Setup runner contexts. */
/* The delays are all relatively prime numbers to encourage a wide range of
 * interesting, but still deterministic, behaviors. */
struct runner_context main_sync_1 = {'w', 20};
struct runner_context main_sync_2 = {'W', 30};
struct runner_context main_sync_3 = {'\\', 50};
struct runner_context main_async_1 = {'m', 70};
struct runner_context main_async_2 = {'M', 110};
struct runner_context main_async_3 = {'/', 130};
struct runner_context secondary_sync_1 = {'s', 170};
struct runner_context secondary_sync_2 = {'S', 190};
struct runner_context secondary_sync_3 = {'$', 230};
struct runner_context secondary_async_1 = {'z', 310};
struct runner_context secondary_async_2 = {'Z', 370};
struct runner_context secondary_async_3 = {'&', 410};
struct runner_context green_async_1 = {'G', 430};
struct runner_context green_async_2 = {'g', 470};

int main(void)
{
    //printf("\r\n***** threaded blinky uvisor-rtos example *****\r\n");
    putc('*', stdout);
    fflush(stdout);

    size_t count = 0;

    /* Startup a few RPC runners. */
    Thread * sync_1 = new(std::nothrow) Thread(green_sync_runner, &main_sync_1);
    Thread * sync_2 = new(std::nothrow) Thread(blue_sync_runner, &main_sync_2);
    Thread * async_1 = new(std::nothrow) Thread(green_async_runner, &main_async_1);
    Thread * async_2 = new(std::nothrow) Thread(blue_async_runner, &main_async_2);

    while (1)
    {
        /* Spin forever. */
        ++count;
    }

    return 0;
}

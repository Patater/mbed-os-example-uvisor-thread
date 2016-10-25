/*
 * Copyright (c) 2013-2016, ARM Limited, All Rights Reserved
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
#include "fun_bag.h"
#include "uvisor-lib/uvisor-lib.h"
#include "mbed.h"
#include "rtos.h"
#include "main-hw.h"

static const uint32_t kB = 1024;

/* Create ACLs for main box. */
MAIN_ACL(g_main_acl);
/* Enable uVisor. */
UVISOR_SET_MODE_ACL(UVISOR_ENABLED, g_main_acl);
UVISOR_SET_PAGE_HEAP(8 * kB, 9);

static void main_alloc(void)
{
    uint16_t seed = 0x10;
    SecureAllocator alloc = secure_allocator_create_with_pages(4 * kB, 1 * kB);

    while (1) {
        alloc_fill_wait_verify_free(500, seed, 577);
        specific_alloc_fill_wait_verify_free(alloc, 1 * kB, seed, 97);
        seed++;
    }
}

int main(void)
{
    osStatus status;
    static const uint32_t STACK_SIZE = 512;
    unsigned char * stack = (unsigned char *) malloc(STACK_SIZE);
    if (stack == NULL) {
        puts("stack malloc failed\r\n");
        fflush(stdout);
        uvisor_error(USER_NOT_ALLOWED);
    }
    Thread thread(osPriorityNormal, STACK_SIZE, stack);
    status = thread.start(main_alloc);
    if (status != osOK) {
        printf("Could not start main thread.\r\n");
        uvisor_error(USER_NOT_ALLOWED);
    }

    printf("\r\n***** threaded blinky uvisor-rtos example *****\r\n");

    size_t count = 0;

    while (1)
    {
        printf("Main loop count: %d\r\n", count++);
    }

    return 0;
}

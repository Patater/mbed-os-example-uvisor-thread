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
#include "uvisor-lib/uvisor-lib.h"
#include "mbed.h"
#include "rtos.h"
#include "main-hw.h"

#define ARRAY_COUNT(x) (sizeof(x) / sizeof(*x))

struct box_context {
    uint32_t blinks;
};

static const UvisorBoxAclItem acl[] = {
};

static void blue_main(const void *);
static int blue_7_ms(void);
static int blue_11_ms(void);
static int blue_13_ms(void);

/* Box configuration */
UVISOR_BOX_NAMESPACE(NULL);
UVISOR_BOX_HEAPSIZE(8192);
UVISOR_BOX_MAIN(blue_main, osPriorityNormal, UVISOR_BOX_STACK_SIZE);
UVISOR_BOX_CONFIG(box_blue, acl, UVISOR_BOX_STACK_SIZE, box_context);

/* Gateways */
UVISOR_BOX_RPC_GATEWAY_SYNC(box_blue, blue_7_ms_sync, blue_7_ms, int, void);
UVISOR_BOX_RPC_GATEWAY_ASYNC(box_blue, blue_7_ms_async, blue_7_ms, int, void);
UVISOR_BOX_RPC_GATEWAY_SYNC(box_blue, blue_11_ms_sync, blue_11_ms, int, void);
UVISOR_BOX_RPC_GATEWAY_ASYNC(box_blue, blue_11_ms_async, blue_11_ms, int, void);
UVISOR_BOX_RPC_GATEWAY_SYNC(box_blue, blue_13_ms_sync, blue_13_ms, int, void);
UVISOR_BOX_RPC_GATEWAY_ASYNC(box_blue, blue_13_ms_async, blue_13_ms, int, void);

/* Green */
DigitalOut led3(LED3);

static int blue_for_duration(uint32_t duration_ms)
{
    //putc('0' + uvisor_box_id_caller(), stdout);
    //fflush(stdout);

    led3 = LED_OFF;
    Thread::wait(duration_ms);
    led3 = LED_ON;

    ++uvisor_ctx->blinks;

    return uvisor_ctx->blinks;
}

static int blue_7_ms(void)
{
    return blue_for_duration(7);
}

static int blue_11_ms(void)
{
    return blue_for_duration(11);
}

static int blue_13_ms(void)
{
    return blue_for_duration(13);
}

static void blue_main(const void *)
{
    led3 = LED_OFF;

    /* The blue box doesn't call into itself. */

    /* The list of functions we are interested in handling RPC requests for */
    const TFN_Ptr my_fn_array[] = {
        (TFN_Ptr) blue_7_ms,
    };

    while (1) {
        int status;

        status = rpc_fncall_waitfor(my_fn_array, ARRAY_COUNT(my_fn_array), NULL, UVISOR_WAIT_FOREVER);

        if (status) {
            printf("Failure is not an option.\r\n");
            uvisor_error(USER_NOT_ALLOWED);
        }
    }
}

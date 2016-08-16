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
#include "runner_context.h"
#include "mbed.h"
#include "rtos.h"
#include "main-hw.h"

#define ARRAY_COUNT(x) (sizeof(x) / sizeof(*x))

struct box_context {
    uint32_t blinks;
};

static const UvisorBoxAclItem acl[] = {
};

static void green_main(const void *);
static int green_2_ms(void);
static int green_3_ms(void);
static int green_5_ms(void);

/* Box configuration */
UVISOR_BOX_NAMESPACE(NULL);
UVISOR_BOX_HEAPSIZE(9000);
UVISOR_BOX_MAIN(green_main, osPriorityNormal, UVISOR_BOX_STACK_SIZE);
UVISOR_BOX_RPC_MAX_INCOMING(8);
UVISOR_BOX_CONFIG(box_green, acl, UVISOR_BOX_STACK_SIZE, box_context);

/* Gateways */
UVISOR_BOX_RPC_GATEWAY_SYNC(box_green, green_2_ms_sync, green_2_ms, int, void);
UVISOR_BOX_RPC_GATEWAY_ASYNC(box_green, green_2_ms_async, green_2_ms, int, void);
UVISOR_BOX_RPC_GATEWAY_SYNC(box_green, green_3_ms_sync, green_3_ms, int, void);
UVISOR_BOX_RPC_GATEWAY_ASYNC(box_green, green_3_ms_async, green_3_ms, int, void);
UVISOR_BOX_RPC_GATEWAY_SYNC(box_green, green_5_ms_sync, green_5_ms, int, void);
UVISOR_BOX_RPC_GATEWAY_ASYNC(box_green, green_5_ms_async, green_5_ms, int, void);

/* Green */
DigitalOut led2(LED2);

static int green_for_duration(uint32_t duration_ms)
{
    putc('0' + uvisor_box_id_caller(), stdout);
    fflush(stdout);

    led2 = LED_OFF;
    Thread::wait(duration_ms);
    led2 = LED_ON;

    ++uvisor_ctx->blinks;

    return uvisor_ctx->blinks;
}

static int green_2_ms(void)
{
    return green_for_duration(2);
}

static int green_3_ms(void)
{
    return green_for_duration(3);
}

static int green_5_ms(void)
{
    return green_for_duration(5);
}

void green_async_runner(const void * ctx)
{
    struct runner_context *rc = (struct runner_context *) ctx;


    while (1) {
        uvisor_rpc_result_t result;
        result = green_2_ms_async();

        /* Wait for a non-error result synchronously. */
        while (1) {
            int status;
            uint32_t ret;
            status = rpc_fncall_wait(result, UVISOR_WAIT_FOREVER, &ret);
            if (!status) {
                break;
            }
        }

        putc(rc->id, stdout);
        fflush(stdout);

        Thread::wait(rc->delay_ms);
    }
}

void green_sync_runner(const void * ctx)
{
    struct runner_context *rc = (struct runner_context *) ctx;

    while (1) {
        green_2_ms_sync(); /* This waits forever for a result. */

        putc(rc->id, stdout);
        fflush(stdout);

        Thread::wait(rc->delay_ms);
    }
}

static void green_handler_a(void)
{
    /* The list of functions we are interested in handling RPC requests for */
    const TFN_Ptr my_fn_array[] = {
        (TFN_Ptr) green_2_ms,
    };

    while (1) {
        int status;

        status = rpc_fncall_waitfor(my_fn_array, ARRAY_COUNT(my_fn_array), UVISOR_WAIT_FOREVER);

        /* Failure is an option, because we have two handlers that can handle
         * calls to green_2_ms. */
    }
}

static void green_handler_ab(void)
{
    /* The list of functions we are interested in handling RPC requests for */
    const TFN_Ptr my_fn_array[] = {
        (TFN_Ptr) green_2_ms,
        (TFN_Ptr) green_3_ms,
    };

    while (1) {
        int status;

        status = rpc_fncall_waitfor(my_fn_array, ARRAY_COUNT(my_fn_array), UVISOR_WAIT_FOREVER);

        /* Failure is an option, because we have two handlers that can handle
         * calls to green_2_ms. */
    }
}

static void green_handler_c(void)
{
    /* The list of functions we are interested in handling RPC requests for */
    const TFN_Ptr my_fn_array[] = {
        (TFN_Ptr) green_5_ms,
    };

    while (1) {
        int status;

        status = rpc_fncall_waitfor(my_fn_array, ARRAY_COUNT(my_fn_array), UVISOR_WAIT_FOREVER);

        if (status) {
            printf("Failure is not an option.\r\n");
            uvisor_error(USER_NOT_ALLOWED);
        }
    }
}

static void green_main(const void *)
{
    led2 = LED_OFF;

    /* The green box calls into itself. */
    //Thread async_1(green_async_runner, &green_async_1);
    //Thread async_2(green_async_runner, &green_async_2);

    /* Spawn some handlers */
    Thread * handler_a_1 = new(std::nothrow) Thread(green_handler_a);
    Thread * handler_a_2 = new(std::nothrow) Thread(green_handler_a);
    Thread * handler_ab = new(std::nothrow) Thread(green_handler_ab);
    Thread * handler_c = new(std::nothrow) Thread(green_handler_c);
    if (handler_a_1 == NULL || handler_a_2 == NULL || handler_ab == NULL || handler_c == NULL) {
        uvisor_error(USER_NOT_ALLOWED);
    }

    /* Prevent thread from exiting, because if it exits, mbed_die is called. */
    while (1);
}

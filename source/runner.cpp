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
#include "runner.h"
#include "green.h"
#include "blue.h"
#include "mbed.h"
#include "rtos.h"
#include <stdio.h>

/* Setup runner contexts. */
/* The delays are all relatively prime numbers to encourage a wide range of
 * interesting, but still deterministic, behaviors. */
sync_runner_ctx  main_sync_1        = {green_2_ms_sync, 'w', 20};
sync_runner_ctx  main_sync_2        = {blue_7_ms_sync, 'W', 30};
sync_runner_ctx  main_sync_3        = {green_5_ms_sync, '\\', 50};
async_runner_ctx main_async_1       = {green_2_ms_async, 'm', 70};
async_runner_ctx main_async_2       = {green_3_ms_async, 'M', 110};
async_runner_ctx main_async_3       = {green_5_ms_async, '/', 130};
sync_runner_ctx  secondary_sync_1   = {0, 's', 170};
sync_runner_ctx  secondary_sync_2   = {0, 'S', 190};
sync_runner_ctx  secondary_sync_3   = {0, '$', 230};
async_runner_ctx secondary_async_1  = {0, 'z', 310};
async_runner_ctx secondary_async_2  = {0, 'Z', 370};
async_runner_ctx secondary_async_3  = {0, '&', 410};
async_runner_ctx green_async_1      = {green_2_ms_async, 'G', 430};
async_runner_ctx green_async_2      = {green_5_ms_async, 'g', 470};

handler_ctx      green_handler_a_1  = {530};
handler_ctx      green_handler_a_2  = {590};
handler_ctx      green_handler_ab_1 = {610};
handler_ctx      green_handler_c_1  = {670};


void sync_runner(const void * ctx)
{
    struct sync_runner_ctx *rc = (struct sync_runner_ctx *) ctx;

    while (1) {
        rc->run(); /* This waits forever for a result. */

        putc(rc->id, stdout);
        fflush(stdout);

        Thread::wait(rc->delay_ms);
    }
}

void async_runner(const void * ctx)
{
    struct async_runner_ctx *rc = (struct async_runner_ctx *) ctx;

    while (1) {
        uvisor_rpc_result_t result;
        result = rc->run();

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

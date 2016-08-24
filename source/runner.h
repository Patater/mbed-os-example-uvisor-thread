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
#ifndef RUNNER_H
#define RUNNER_H

#include "uvisor-lib/uvisor-lib.h"
#include <stdint.h>

struct sync_runner_ctx {
    int (*run)(void);
    char id;
    uint32_t delay_ms;
};

struct async_runner_ctx {
    uvisor_rpc_result_t (*run)(void);
    char id;
    uint32_t delay_ms;
};

struct handler_ctx {
    uint32_t delay_ms;
    size_t num_handled;
};

extern sync_runner_ctx main_sync_1;
extern sync_runner_ctx main_sync_2;
extern sync_runner_ctx main_sync_3;
extern async_runner_ctx main_async_1;
extern async_runner_ctx main_async_2;
extern async_runner_ctx main_async_3;
extern sync_runner_ctx secondary_sync_1;
extern sync_runner_ctx secondary_sync_2;
extern sync_runner_ctx secondary_sync_3;
extern async_runner_ctx secondary_async_1;
extern async_runner_ctx secondary_async_2;
extern async_runner_ctx secondary_async_3;
extern async_runner_ctx green_async_1;
extern async_runner_ctx green_async_2;

extern handler_ctx green_handler_a_1;
extern handler_ctx green_handler_a_2;
extern handler_ctx green_handler_ab_1;
extern handler_ctx green_handler_c_1;

void sync_runner(const void * ctx);
void async_runner(const void * ctx);

#endif

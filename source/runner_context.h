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
#ifndef RUNNER_CONTEXT_H
#define RUNNER_CONTEXT_H

#include <stdint.h>

struct runner_context {
    char id;
    uint32_t delay_ms;
};

extern struct runner_context main_sync_1;
extern struct runner_context main_sync_2;
extern struct runner_context main_sync_3;
extern struct runner_context main_async_1;
extern struct runner_context main_async_2;
extern struct runner_context main_async_3;
extern struct runner_context secondary_sync_1;
extern struct runner_context secondary_sync_2;
extern struct runner_context secondary_sync_3;
extern struct runner_context secondary_async_1;
extern struct runner_context secondary_async_2;
extern struct runner_context secondary_async_3;
extern struct runner_context green_async_1;
extern struct runner_context green_async_2;

#endif

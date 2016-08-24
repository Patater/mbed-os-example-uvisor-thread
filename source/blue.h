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
#ifndef BLUE_H
#define BLUE_H

#include "uvisor-lib/uvisor-lib.h"

extern int (*blue_7_ms_sync)(void);
extern uvisor_rpc_result_t (*blue_7_ms_async)(void);

extern int (*blue_11_ms_sync)(void);
extern uvisor_rpc_result_t (*blue_11_ms_async)(void);

extern int (*blue_13_ms_sync)(void);
extern uvisor_rpc_result_t (*blue_13_ms_async)(void);

#endif

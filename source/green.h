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
#ifndef GREEN_H
#define GREEN_H

#include "uvisor-lib/uvisor-lib.h"

extern int (*green_2_ms_sync)(void);
extern uvisor_rpc_result_t (*green_2_ms_async)(void);

extern int (*green_3_ms_sync)(void);
extern uvisor_rpc_result_t (*green_3_ms_async)(void);

extern int (*green_5_ms_sync)(void);
extern uvisor_rpc_result_t (*green_5_ms_async)(void);

#endif

/*
 * Copyright (c) [2020] Huawei Technologies Co.,Ltd.All rights reserved.
 *
 * OpenArkCompiler is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *
 *     http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR
 * FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#ifndef MAPLEBE_INCLUDE_CG_AARCH64_AARCH64_IMMEDIATE_H
#define MAPLEBE_INCLUDE_CG_AARCH64_AARCH64_IMMEDIATE_H

#include "types_def.h"  /* maple_ir/include/typedef.h */
#include <array>

namespace maplebe {
bool IsBitSizeImmediate(maple::uint64 val, maple::uint32 bitLen, maple::uint32 nLowerZeroBits);
bool IsBitmaskImmediate(maple::uint64 val, maple::uint32 bitLen);
bool IsMoveWidableImmediate(maple::uint64 val, maple::uint32 bitLen);
bool BetterUseMOVZ(maple::uint64 val);
}  /* namespace maplebe */

#endif  /* MAPLEBE_INCLUDE_CG_AARCH64_AARCH64_IMMEDIATE_H */

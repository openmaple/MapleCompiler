/*
 * Copyright (c) [2019] Huawei Technologies Co.,Ltd.All rights reserved.
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
#ifndef MAPLE_ME_INCLUDE_FUNC_EMIT_H
#define MAPLE_ME_INCLUDE_FUNC_EMIT_H
#include "bb.h"

// Provide emit service for both MeFunction and WpoFunction.
namespace maple {
class FuncEmit {
 public:
  void EmitBeforeHSSA(MIRFunction &func, const MapleVector<BB*> &bbList) const;
  virtual ~FuncEmit() = default;

 private:
  void EmitLabelForBB(MIRFunction &func, BB &bb) const;
};
}  // namespace maple
#endif  // MAPLE_ME_INCLUDE_FUNC_EMIT_H

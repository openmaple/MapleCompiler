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
#include "compiler_selector.h"
#include <algorithm>
#include "mpl_logging.h"

namespace maple {
Compiler *CompilerSelectorImpl::FindCompiler(const SupportedCompilers &compilers, const std::string &name) const {
  auto compiler = compilers.find(name);
  if (compiler != compilers.end()) {
    return compiler->second;
  }
  return nullptr;
}

ErrorCode CompilerSelectorImpl::InsertCompilerIfNeeded(std::vector<Compiler*> &selected,
                                                       const SupportedCompilers &compilers,
                                                       const std::string &name) const {
  Compiler *compiler = FindCompiler(compilers, name);
  if (compiler != nullptr) {
    if (std::find(selected.cbegin(), selected.cend(), compiler) == selected.cend()) {
      selected.push_back(compiler);
    }
    return kErrorNoError;
  }

  LogInfo::MapleLogger(kLlErr) << name << " not found!!!" << '\n';
  return kErrorToolNotFound;
}

ErrorCode CompilerSelectorImpl::Select(const SupportedCompilers &supportedCompilers, const MplOptions &mplOptions,
                                       std::vector<Compiler*> &selected) const {
  bool combPhases = false;
  if (!mplOptions.GetSelectedExes().empty()) {
    for (const std::string &runningExe : mplOptions.GetSelectedExes()) {
      if (runningExe == kBinNameMe) {
        combPhases = true;
      } else if (runningExe == kBinNameMpl2mpl && combPhases) {
        continue;
      }
      ErrorCode ret = InsertCompilerIfNeeded(selected, supportedCompilers, runningExe);
      if (ret != kErrorNoError) {
        return kErrorToolNotFound;
      }
    }
  }
  return selected.empty() ? kErrorToolNotFound : kErrorNoError;
}
}  // namespace maple

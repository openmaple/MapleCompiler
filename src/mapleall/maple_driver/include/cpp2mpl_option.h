/*
 * Copyright (c) [2021] Huawei Technologies Co.,Ltd.All rights reserved.
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
#ifndef MAPLE_CPP2MPL_OPTION_H
#define MAPLE_CPP2MPL_OPTION_H
#include "option_descriptor.h"

namespace maple {
enum CppOptionIndex {
  kInAst,
  kCpp2mplHelp,
};

const mapleOption::Descriptor cppUsage[] = {
  { kUnknown, 0, "", "", mapleOption::kBuildTypeAll, mapleOption::kArgCheckPolicyUnknown,
    "========================================\n"
    " Usage: mplfe [ options ]\n"
    " options:\n",
    "mplfe",
    {} },

  { kInAst, 0, "", "inast", mapleOption::kBuildTypeAll, mapleOption::kArgCheckPolicyRequired,
    "  -inast file1.ast,file2.ast\n"
    "                         : input ast files",
    "c2mpl",
    {} },

  { kCpp2mplHelp, 0, "h", "help", mapleOption::kBuildTypeExperimental, mapleOption::kArgCheckPolicyNone,
    "   -h, --help          : print usage and exit.\n",
    "mplfe",
    {} },

  { kUnknown, 0, "", "", mapleOption::kBuildTypeAll, mapleOption::kArgCheckPolicyNone,
    "",
    "mplfe",
    {} }
};
} // namespace maple

#endif //MAPLE_CPP2MPL_OPTION_H


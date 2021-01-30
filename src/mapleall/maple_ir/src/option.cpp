/*
 * Copyright (c) [2019-2021] Huawei Technologies Co.,Ltd.All rights reserved.
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
#include "option.h"
#include <iostream>
#include <cstring>
#include <cctype>
#include "mpl_logging.h"
#include "option_parser.h"

namespace maple {
using namespace mapleOption;

bool Options::dumpBefore = false;
bool Options::dumpAfter = false;
std::string Options::dumpPhase = "*";
std::string Options::dumpFunc = "*";
std::string Options::skipPhase;
std::string Options::skipFrom;
std::string Options::skipAfter;
bool Options::quiet = false;
bool Options::regNativeFunc = false;
bool Options::nativeWrapper = true;         // Enabled by default
bool Options::inlineWithProfile = false;
bool Options::useInline = true;             // Enabled by default
bool Options::useCrossModuleInline = true;  // Enabled by default
std::string Options::noInlineFuncList = "";
uint32 Options::inlineSmallFunctionThreshold = 15;
uint32 Options::inlineHotFunctionThreshold = 30;
uint32 Options::inlineRecursiveFunctionThreshold = 15;
uint32 Options::inlineModuleGrowth = 10;
uint32 Options::inlineColdFunctionThreshold = 3;
uint32 Options::profileHotCount = 1000;
uint32 Options::profileColdCount = 10;
bool Options::profileHotCountSeted = false;
bool Options::profileColdCountSeted = false;
uint32 Options::profileHotRate = 500000;
uint32 Options::profileColdRate = 900000;
bool Options::regNativeDynamicOnly = false;
std::string Options::staticBindingList;
bool Options::usePreg = false;
bool Options::mapleLinker = false;
bool Options::dumpMuidFile = false;
bool Options::emitVtableImpl = false;
#if MIR_JAVA
bool Options::skipVirtualMethod = false;
#endif
bool Options::nativeOpt = true;
bool Options::O2 = false;
bool Options::noDot = false;
bool Options::genIRProfile = false;
bool Options::profileTest = false;
std::string Options::criticalNativeFile = "maple/mrt/codetricks/profile.pv/criticalNative.list";
std::string Options::fastNativeFile = "maple/mrt/codetricks/profile.pv/fastNative.list";
bool Options::barrier = false;
std::string Options::nativeFuncPropertyFile = "maple/mrt/codetricks/native_binding/native_func_property.list";
bool Options::mapleLinkerTransformLocal = true;
bool Options::decoupleStatic = false;
bool Options::partialAot = false;
uint32 Options::decoupleInit = 0;
uint32 Options::buildApp = kNoDecouple;
std::string Options::sourceMuid = "";
bool Options::decoupleSuper = false;
bool Options::deferredVisit = false;
bool Options::deferredVisit2 = false;
bool Options::genVtabAndItabForDecouple = false;
bool Options::profileFunc = false;
uint32 Options::parserOpt = 0;
std::string Options::dumpDevirtualList = "";
std::string Options::readDevirtualList = "";
bool Options::usePreloadedClass = false;
std::string Options::profile = "";
std::string Options::appPackageName = "";
bool Options::profileStaticFields = false;
std::string Options::proFileData = "";
std::string Options::proFileFuncData = "";
std::string Options::proFileClassData = "";
bool Options::checkArrayStore = false;
bool Options::noComment = false;

enum OptionIndex {
  kMpl2MplDumpPhase = kCommonOptionEnd + 1,
  kMpl2MplSkipPhase,
  kMpl2MplSkipFrom,
  kMpl2MplSkipAfter,
  kMpl2MplDumpFunc,
  kMpl2MplQuiet,
  kMpl2MplStubJniFunc,
  kMpl2MplSkipVirtual,
  kRegNativeDynamicOnly,
  kRegNativeStaticBindingList,
  kNativeWrapper,
  kMpl2MplDumpBefore,
  kMpl2MplDumpAfter,
  kMpl2MplMapleLinker,
  kMplnkDumpMuid,
  kEmitVtableImpl,
  kInlineWithProfile,
  kInlineWithoutProfile,
  kMpl2MplUseInline,
  kMpl2MplNoInlineFuncList,
  kMpl2MplUseCrossModuleInline,
  kInlineSmallFunctionThreshold,
  kInlineHotFunctionThreshold,
  kInlineRecursiveFunctionThreshold,
  kInlineModuleGrowth,
  kInlineColdFunctionThreshold,
  kProfileHotCount,
  kProfileColdCount,
  kProfileHotRate,
  kProfileColdRate,
  kMpl2MplNativeOpt,
  kMpl2MplOptL0,
  kMpl2MplOptL2,
  kMpl2MplNoDot,
  kGenIRProfile,
  kProfileTest,
  kNoComment
};

const Descriptor kUsage[] = {
  { kMpl2MplDumpPhase,
    0,
    "",
    "dump-phase",
    kBuildTypeDebug,
    kArgCheckPolicyRequired,
    "  --dump-phase                \tEnable debug trace for specified phase (can only specify once)\n"
    "                              \t--dump-phase=PHASENAME\n",
    "mpl2mpl",
    {} },
  { kMpl2MplSkipPhase,
    0,
    "",
    "skip-phase",
    kBuildTypeProduct,
    kArgCheckPolicyRequired,
    "  --skip-phase                \tSkip the phase when adding it to phase manager\n"
    "                              \t--skip-phase=PHASENAME\n",
    "mpl2mpl",
    {} },
  { kMpl2MplSkipFrom,
    0,
    "",
    "skip-from",
    kBuildTypeExperimental,
    kArgCheckPolicyRequired,
    "  --skip-from                 \tSkip all remaining phases including PHASENAME\n"
    "                              \t--skip-from=PHASENAME\n",
    "mpl2mpl",
    {} },
  { kMpl2MplSkipAfter,
    0,
    "",
    "skip-after",
    kBuildTypeExperimental,
    kArgCheckPolicyRequired,
    "  --skip-after                \tSkip all remaining phases after PHASENAME\n"
    "                              \t--skip-after=PHASENAME\n",
    "mpl2mpl",
    {} },
  { kMpl2MplDumpFunc,
    0,
    "",
    "dump-func",
    kBuildTypeDebug,
    kArgCheckPolicyRequired,
    "  --dump-func                 \tDump/trace only for functions whose names contain FUNCNAME as substring\n"
    "                              \t(can only specify once)\n"
    "                              \t--dump-func=FUNCNAME\n",
    "mpl2mpl",
    {} },
  { kMpl2MplQuiet,
    kEnable,
    "",
    "quiet",
    kBuildTypeProduct,
    kArgCheckPolicyBool,
    "  --quiet                     \tDisable brief trace messages with phase/function names\n"
    "  --no-quiet                  \tEnable brief trace messages with phase/function names\n",
    "mpl2mpl",
    {} },
  { kMpl2MplMapleLinker,
    kEnable,
    "",
    "maplelinker",
    kBuildTypeProduct,
    kArgCheckPolicyBool,
    "  --maplelinker               \tGenerate MUID symbol tables and references\n"
    "  --no-maplelinker            \tDon't Generate MUID symbol tables and references\n",
    "mpl2mpl",
    {} },
  { kMpl2MplStubJniFunc,
    kEnable,
    "",
    "regnativefunc",
    kBuildTypeProduct,
    kArgCheckPolicyBool,
    "  --regnativefunc             \tGenerate native stub function to support JNI registration and calling\n"
    "  --no-regnativefunc          \tDisable regnativefunc\n",
    "mpl2mpl",
    {} },
  { kInlineWithProfile,
    kEnable,
    "",
    "inline-with-profile",
    kBuildTypeExperimental,
    kArgCheckPolicyBool,
    "  --inline-with-profile       \tEnable profile-based inlining\n"
    "  --no-inline-with-profile    \tDisable profile-based inlining\n",
    "mpl2mpl",
    {} },
  { kMpl2MplUseInline,
    kEnable,
    "",
    "inline",
    kBuildTypeExperimental,
    kArgCheckPolicyBool,
    "  --inline                    \tEnable function inlining\n"
    "  --no-inline                 \tDisable function inlining\n",
    "mpl2mpl",
    {} },
  { kMpl2MplNoInlineFuncList,
    0,
    "",
    "no-inlinefunclist",
    kBuildTypeExperimental,
    kArgCheckPolicyRequired,
    "  --no-inlinefunclist=list    \tDo not inline function in this list\n",
    "mpl2mpl",
    {} },
  { kMpl2MplUseCrossModuleInline,
    kEnable,
    "",
    "cross-module-inline",
    kBuildTypeExperimental,
    kArgCheckPolicyBool,
    "  --cross-module-inline       \tEnable cross-module inlining\n"
    "  --no-cross-module-inline    \tDisable cross-module inlining\n",
    "mpl2mpl",
    {} },
  { kInlineSmallFunctionThreshold,
    0,
    "",
    "inline-small-function-threshold",
    kBuildTypeExperimental,
    kArgCheckPolicyRequired,
    "  --inline-small-function-threshold=15            \tThreshold for inlining small function\n",
    "mpl2mpl",
    {} },
  { kInlineHotFunctionThreshold,
    0,
    "",
    "inline-hot-function-threshold",
    kBuildTypeExperimental,
    kArgCheckPolicyRequired,
    "  --inline-hot-function-threshold=30              \tThreshold for inlining hot function\n",
    "mpl2mpl",
    {} },
  { kInlineRecursiveFunctionThreshold,
    0,
    "",
    "inline-recursive-function-threshold",
    kBuildTypeExperimental,
    kArgCheckPolicyRequired,
    "  --inline-recursive-function-threshold=15              \tThreshold for inlining recursive function\n",
    "mpl2mpl",
    {} },
  { kInlineModuleGrowth,
    0,
    "",
    "inline-module-growth",
    kBuildTypeExperimental,
    kArgCheckPolicyRequired,
    "  --inline-module-growth=100000                   \tThreshold for maxmium code size growth rate (10%)\n",
    "mpl2mpl",
    {} },
  { kInlineColdFunctionThreshold,
    0,
    "",
    "inline-cold-function-threshold",
    kBuildTypeExperimental,
    kArgCheckPolicyRequired,
    "  --inline-cold-function-threshold=3              \tThreshold for inlining hot function\n",
    "mpl2mpl",
    {} },
  { kProfileHotCount,
    0,
    "",
    "profile-hot-count",
    kBuildTypeExperimental,
    kArgCheckPolicyRequired,
    "  --profile-hot-count=1000    \tA count is regarded as hot if it exceeds this number\n",
    "mpl2mpl",
    {} },
  { kProfileColdCount,
    0,
    "",
    "profile-cold-count",
    kBuildTypeExperimental,
    kArgCheckPolicyRequired,
    "  --profile-cold-count=10     \tA count is regarded as cold if it is below this number\n",
    "mpl2mpl",
    {} },
  { kProfileHotRate,
    0,
    "",
    "profile-hot-rate",
    kBuildTypeExperimental,
    kArgCheckPolicyRequired,
    "  --profile-hot-rate=500000   \tA count is regarded as hot if it is in the largest 50%\n",
    "mpl2mpl",
    {} },
  { kProfileColdRate,
    0,
    "",
    "profile-cold-rate",
    kBuildTypeExperimental,
    kArgCheckPolicyRequired,
    "  --profile-cold-rate=900000  \tA count is regarded as cold if it is in the smallest 10%\n",
    "mpl2mpl",
    {} },
  { kNativeWrapper,
    kEnable,
    "",
    "nativewrapper",
    kBuildTypeExperimental,
    kArgCheckPolicyBool,
    "  --nativewrapper             \tGenerate native wrappers [default]\n",
    "mpl2mpl",
    {} },
  { kRegNativeDynamicOnly,
    kEnable,
    "",
    "regnative-dynamic-only",
    kBuildTypeProduct,
    kArgCheckPolicyBool,
    "  --regnative-dynamic-only    \tOnly Generate dynamic register code, Report Fatal Msg if no implemented\n"
    "  --no-regnative-dynamic-only \tDisable regnative-dynamic-only\n",
    "mpl2mpl",
    {} },
  { kRegNativeStaticBindingList,
    0,
    "",
    "static-binding-list",
    kBuildTypeExperimental,
    kArgCheckPolicyRequired,
    "  --static-bindig-list        \tOnly Generate static binding function in file configure list\n"
    "                              \t--static-bindig-list=file\n",
    "mpl2mpl",
    {} },
  { kMpl2MplDumpBefore,
    kEnable,
    "",
    "dump-before",
    kBuildTypeDebug,
    kArgCheckPolicyBool,
    "  --dump-before               \tDo extra IR dump before the specified phase\n"
    "  --no-dump-before            \tDon't extra IR dump before the specified phase\n",
    "mpl2mpl",
    {} },
  { kMpl2MplDumpAfter,
    kEnable,
    "",
    "dump-after",
    kBuildTypeDebug,
    kArgCheckPolicyBool,
    "  --dump-after                \tDo extra IR dump after the specified phase\n"
    "  --no-dump-after             \tDon't extra IR dump after the specified phase\n",
    "mpl2mpl",
    {} },
  { kMplnkDumpMuid,
    kEnable,
    "",
    "dump-muid",
    kBuildTypeDebug,
    kArgCheckPolicyBool,
    "  --dump-muid                 \tDump MUID def information into a .muid file\n"
    "  --no-dump-muid              \tDon't dump MUID def information into a .muid file\n",
    "mpl2mpl",
    {} },
  { kEmitVtableImpl,
    kEnable,
    "",
    "emitVtableImpl",
    kBuildTypeProduct,
    kArgCheckPolicyBool,
    "  --emitVtableImpl            \tgenerate VtableImpl file\n"
    "  --no-emitVtableImpl         \tDon't generate VtableImpl file\n",
    "mpl2mpl",
    {} },
#if MIR_JAVA
  { kMpl2MplSkipVirtual,
    kEnable,
    "",
    "skipvirtual",
    kBuildTypeExperimental,
    kArgCheckPolicyBool,
    "  --skipvirtual\n"
    "  --no-skipvirtual\n",
    "mpl2mpl",
    {} },
#endif
  { kMpl2MplNativeOpt,
    kEnable,
    "",
    "nativeopt",
    kBuildTypeProduct,
    kArgCheckPolicyBool,
    "  --nativeopt                 \tEnable native opt\n"
    "  --no-nativeopt              \tDisable native opt\n",
    "mpl2mpl",
    {} },
  { kMpl2MplOptL0,
    0,
    "",
    "O0",
    kBuildTypeProduct,
    kArgCheckPolicyOptional,
    "  -O0                         \tDo some optimization.\n",
    "mpl2mpl",
    {} },
  { kMpl2MplOptL2,
    0,
    "",
    "O2",
    kBuildTypeProduct,
    kArgCheckPolicyOptional,
    "  -O2                         \tDo some optimization.\n",
    "mpl2mpl",
    {} },
  { kMpl2MplNoDot,
    kEnable,
    "",
    "nodot",
    kBuildTypeExperimental,
    kArgCheckPolicyBool,
    "  --nodot                     \tDisable dot file generation from cfg\n"
    "  --no-nodot                  \tEnable dot file generation from cfg\n",
    "mpl2mpl",
    {} },
  { kGenIRProfile,
    kEnable,
    "",
    "ir-profile-gen",
    mapleOption::BuildType::kBuildTypeExperimental,
    mapleOption::ArgCheckPolicy::kArgCheckPolicyBool,
    "  --ir-profile-gen              \tGen IR level Profile\n"
    "  --no-ir-profile-gen           \tDisable Gen IR level Profile\n",
    "mpl2mpl",
    {} },
  { kProfileTest,
    kEnable,
    "",
    "profile-test",
    mapleOption::BuildType::kBuildTypeExperimental,
    mapleOption::ArgCheckPolicy::kArgCheckPolicyBool,
    "  --profile-test              \tprofile test\n"
    "  --no-profile-test           \tDisable profile test\n",
    "mpl2mpl",
    {} },
  { kNoComment,
    0,
    "",
    "no-comment",
    kBuildTypeProduct,
    kArgCheckPolicyBool,
    "  --no-comment             \tbuild inlineCache 0:off, 1:open\n",
    "mpl2mpl",
    {} },
  { kUnknown,
    0,
    "",
    "",
    kBuildTypeAll,
    kArgCheckPolicyNone,
    "",
    "mpl2mpl",
    {} }
};

Options &Options::GetInstance() {
  static Options instance;
  return instance;
}

Options::Options() {
  CreateUsages(kUsage);
}

void Options::DecideMpl2MplRealLevel(const std::vector<mapleOption::Option> &inputOptions) const {
  int realLevel = -1;
  for (const mapleOption::Option &opt : inputOptions) {
    switch (opt.Index()) {
      case kMpl2MplOptL0:
        realLevel = kLevelZero;
        break;
      case kMpl2MplOptL2:
        realLevel = kLevelTwo;
        break;
      default:
        break;
    }
  }
  if (realLevel == kLevelTwo) {
    O2 = true;
    usePreg = true;
  }
}

bool Options::SolveOptions(const std::vector<Option> &opts, bool isDebug) const {
  DecideMpl2MplRealLevel(opts);
  bool result = true;
  for (const mapleOption::Option &opt : opts) {
    if (isDebug) {
      LogInfo::MapleLogger() << "mpl2mpl options: " << opt.Index() << " " << opt.OptionKey() << " " <<
                                opt.Args() << '\n';
    }
    switch (opt.Index()) {
      case kMpl2MplDumpBefore:
        dumpBefore = (opt.Type() == kEnable);
        break;
      case kMpl2MplDumpAfter:
        dumpAfter = (opt.Type() == kEnable);
        break;
      case kMpl2MplDumpFunc:
        dumpFunc = opt.Args();
        break;
      case kMpl2MplQuiet:
        quiet = (opt.Type() == kEnable);
        break;
      case kVerbose:
        quiet = (opt.Type() == kEnable) ? false : true;
        break;
      case kMpl2MplDumpPhase:
        dumpPhase = opt.Args();
        break;
      case kMpl2MplSkipPhase:
        skipPhase = opt.Args();
        break;
      case kMpl2MplSkipFrom:
        skipFrom = opt.Args();
        break;
      case kMpl2MplSkipAfter:
        skipAfter = opt.Args();
        break;
      case kRegNativeDynamicOnly:
        regNativeDynamicOnly = (opt.Type() == kEnable);
        break;
      case kRegNativeStaticBindingList:
        staticBindingList = opt.Args();
        break;
      case kMpl2MplStubJniFunc:
        regNativeFunc = (opt.Type() == kEnable);
        break;
      case kNativeWrapper:
        nativeWrapper = (opt.Type() == kEnable);
        break;
      case kInlineWithProfile:
        inlineWithProfile = (opt.Type() == kEnable);
        break;
      case kMpl2MplUseInline:
        useInline = (opt.Type() == kEnable);
        break;
      case kMpl2MplNoInlineFuncList:
        noInlineFuncList = opt.Args();
        break;
      case kMpl2MplUseCrossModuleInline:
        useCrossModuleInline = (opt.Type() == kEnable);
        break;
      case kInlineSmallFunctionThreshold:
        if (opt.Args().empty()) {
          LogInfo::MapleLogger(kLlErr) << "expecting not empty for --inline-small-function-threshold\n";
          result = false;
        } else {
          inlineSmallFunctionThreshold = std::stoul(opt.Args());
        }
        break;
      case kInlineHotFunctionThreshold:
        if (opt.Args().empty()) {
          LogInfo::MapleLogger(kLlErr) << "expecting not empty for --inline-hot-function-threshold\n";
          result = false;
        } else {
          inlineHotFunctionThreshold = std::stoul(opt.Args());
        }
        break;
      case kInlineRecursiveFunctionThreshold:
        if (opt.Args().empty()) {
          LogInfo::MapleLogger(kLlErr) << "expecting not empty for --inline-recursive-function-threshold\n";
          result = false;
        } else {
          inlineRecursiveFunctionThreshold = std::stoul(opt.Args());
        }
        break;
      case kInlineModuleGrowth:
        if (opt.Args().empty()) {
          LogInfo::MapleLogger(kLlErr) << "expecting not empty for --inline-module-growth\n";
          result = false;
        } else {
          inlineModuleGrowth = std::stoul(opt.Args());
        }
        break;
      case kInlineColdFunctionThreshold:
        if (opt.Args().empty()) {
          LogInfo::MapleLogger(kLlErr) << "expecting not empty for --inline-cold-function-threshold\n";
          result = false;
        } else {
          inlineColdFunctionThreshold = std::stoul(opt.Args());
        }
        break;
      case kProfileHotCount:
        if (opt.Args().empty()) {
          LogInfo::MapleLogger(kLlErr) << "expecting not empty for --profile-hot-count\n";
          result = false;
        } else {
          profileHotCount = std::stoul(opt.Args());
        }
        break;
      case kProfileColdCount:
        if (opt.Args().empty()) {
          LogInfo::MapleLogger(kLlErr) << "expecting not empty for --profile-cold-count\n";
          result = false;
        } else {
          profileColdCount = std::stoul(opt.Args());
        }
        break;
      case kProfileHotRate:
        if (opt.Args().empty()) {
          LogInfo::MapleLogger(kLlErr) << "expecting not empty for --profile-hot-rate\n";
          result = false;
        } else {
          profileHotRate = std::stoul(opt.Args());
        }
        break;
      case kProfileColdRate:
        if (opt.Args().empty()) {
          LogInfo::MapleLogger(kLlErr) << "expecting not empty for --profile-cold-rate\n";
          result = false;
        } else {
          profileColdRate = std::stoul(opt.Args());
        }
        break;
      case kMpl2MplMapleLinker:
        mapleLinker = (opt.Type() == kEnable);
        dumpMuidFile = (opt.Type() == kEnable);
        if (isDebug) {
          LogInfo::MapleLogger() << "--sub options: dumpMuidFile " << dumpMuidFile << '\n';
        }
        break;
      case kMplnkDumpMuid:
        dumpMuidFile = (opt.Type() == kEnable);
        break;
      case kEmitVtableImpl:
        emitVtableImpl = (opt.Type() == kEnable);
        break;
#if MIR_JAVA
      case kMpl2MplSkipVirtual:
        skipVirtualMethod = (opt.Type() == kEnable);
        break;
#endif
      case kMpl2MplNativeOpt:
        nativeOpt = (opt.Type() == kEnable);
        break;
      case kMpl2MplOptL2:
        // Already handled above in DecideMpl2MplRealLevel
        break;
      case kMpl2MplNoDot:
        noDot = (opt.Type() == kEnable);
        break;
      case kGenIRProfile:
        genIRProfile = (opt.Type() == kEnable);
        break;
      case kProfileTest:
        profileTest = (opt.Type() == kEnable);
        break;
      case kNoComment:
        noComment = true;
        break;
      default:
        WARN(kLncWarn, "input invalid key for mpl2mpl " + opt.OptionKey());
        break;
    }
  }
  return result;
}

bool Options::ParseOptions(int argc, char **argv, std::string &fileName) const {
  OptionParser optionParser;
  optionParser.RegisteUsages(DriverOptionCommon::GetInstance());
  optionParser.RegisteUsages(Options::GetInstance());
  int ret = optionParser.Parse(argc, argv, "mpl2mpl");
  CHECK_FATAL(ret == kErrorNoError, "option parser error");
  bool result = SolveOptions(optionParser.GetOptions(), false);
  if (!result) {
    return result;
  }
  if (optionParser.GetNonOptionsCount() != 1) {
    LogInfo::MapleLogger(kLlErr) << "expecting one .mpl file as last argument, found: ";
    for (const auto &optionArg : optionParser.GetNonOptions()) {
      LogInfo::MapleLogger(kLlErr) << optionArg << " ";
    }
    LogInfo::MapleLogger(kLlErr) << "\n";
    result = false;
  }

  if (result) {
    fileName = optionParser.GetNonOptions().front();
  }
  return result;
}

void Options::DumpOptions() const {
  LogInfo::MapleLogger() << "phase sequence : \t";
  if (phaseSeq.empty()) {
    LogInfo::MapleLogger() << "default phase sequence\n";
  } else {
    for (size_t i = 0; i < phaseSeq.size(); ++i) {
      LogInfo::MapleLogger() << " " << phaseSeq[i];
    }
  }
  LogInfo::MapleLogger() << "\n";
}
};  // namespace maple

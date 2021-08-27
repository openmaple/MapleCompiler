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
#include "me_phase_manager.h"
#include "ipa_phase_manager.h"
#include <iostream>
#include <vector>
#include <string>

#define JAVALANG (mirModule.IsJavaModule())
#define CLANG (mirModule.IsCModule())

namespace maple {
bool IpaSccPM::PhaseRun(MIRModule &m) {
  bool oldPropInIrmapBuild = MeOption::propDuringBuild;
  MeOption::propDuringBuild = false;
  SetQuiet(false);
  DoPhasesPopulate(m);
  bool changed = false;
  auto admMempool = AllocateMemPoolInPhaseManager("Ipa Phase Manager's Analysis Data Manager mempool");
  auto *serialADM = GetManagerMemPool()->New<AnalysisDataManager>(*(admMempool.get()));
  CallGraph *cg = GET_ANALYSIS(M2MCallGraph, m);
  // Need reverse sccV
  const MapleVector<SCCNode*> &topVec = cg->GetSCCTopVec();
  for (MapleVector<SCCNode*>::const_reverse_iterator it = topVec.rbegin(); it != topVec.rend(); ++it) {
    if (!IsQuiet()) {
      LogInfo::MapleLogger() << ">>>>>>>>>>> Optimizing SCC  < " << " >---\n";
      (*it)->Dump();
    }

    auto meFuncMP = std::make_unique<ThreadLocalMemPool>(memPoolCtrler, "maple_ipa per-scc mempool");
    auto meFuncStackMP = std::make_unique<StackMemPool>(memPoolCtrler, "");
    for (auto *cgNode : (*it)->GetCGNodes()) {
      MIRFunction *func = cgNode->GetMIRFunction();
      if (func->IsEmpty()) {
        continue;
      }
      m.SetCurFunction(func);
      MemPool *versMP = new ThreadLocalMemPool(memPoolCtrler, "first verst mempool");
      MeFunction &meFunc = *(meFuncMP->New<MeFunction>(&m, func, meFuncMP.get(), *meFuncStackMP, versMP, "unknown"));
      func->SetMeFunc(&meFunc);
      meFunc.PartialInit();
      if (!IsQuiet()) {
        LogInfo::MapleLogger() << "---Preparing Function  < " << func->GetName() << " > ---\n";
      }
      serialADM->Dump();
      if (func->GetName() == "push_secondary_reload") {
        std::cout << std::endl;
      }
      meFunc.Prepare();
      MaplePhaseID ids[] = {&MEMeCfg::id, &MECFGOPT::id, &MESSATab::id, &MEAliasClass::id, &MESSA::id, &MESideEffect::id};
      for (MaplePhaseID id : ids) {
        const MaplePhaseInfo *phase = MaplePhaseRegister::GetMaplePhaseRegister()->GetPhaseByID(id);
        if (!IsQuiet()) {
          LogInfo::MapleLogger() << "---Run " << (phase->IsAnalysis() ? "analysis" : "transform")
                                 << " Phase [ " << phase->PhaseName() << " ]---\n";
        }
        if (phase->IsAnalysis()) {
          RunAnalysisPhase<meFuncOptTy, MeFunction>(*phase, *serialADM, meFunc, 1);
        } else {
          RunTransformPhase<meFuncOptTy, MeFunction>(*phase, *serialADM, meFunc, 1);
        }
        CHECK_FATAL(meFunc.GetCfg() != nullptr, "error");
//        meFunc.Dump(false);
      }
    }
    for (size_t i = 0; i < phasesSequence.size(); ++i) {
      const MaplePhaseInfo *curPhase = MaplePhaseRegister::GetMaplePhaseRegister()->GetPhaseByID(phasesSequence[i]);
      changed |= RunTransformPhase<MapleSccPhase<SCCNode>, SCCNode>(*curPhase, *serialADM, **it);
    }
    for (auto *cgNode : (*it)->GetCGNodes()) {
      MIRFunction *func = cgNode->GetMIRFunction();
      if (func->IsEmpty()) {
        continue;
      }
      m.SetCurFunction(func);
      const MaplePhaseInfo *phase = MaplePhaseRegister::GetMaplePhaseRegister()->GetPhaseByID(&MEEmit::id);
      if (!IsQuiet()) {
        LogInfo::MapleLogger() << "---Run " << (phase->IsAnalysis() ? "analysis" : "transform")
                               << " Phase [ " << phase->PhaseName() << " ]---\n";
      }

      RunTransformPhase<meFuncOptTy, MeFunction>(*phase, *serialADM, *func->GetMeFunc());
    }
    serialADM->EraseAllAnalysisPhase();
  }
  MeOption::propDuringBuild = oldPropInIrmapBuild;
  return changed;
}

void IpaSccPM::DoPhasesPopulate(const MIRModule &mirModule) {
  (void)mirModule;
  AddPhase("foo", true);
}

void IpaSccPM::GetAnalysisDependence(maple::AnalysisDep &aDep) const {
  aDep.AddRequired<M2MCallGraph>();
  aDep.AddRequired<M2MKlassHierarchy>();
  aDep.AddPreserved<M2MCallGraph>();
  aDep.AddPreserved<M2MKlassHierarchy>();
}

MAPLE_ANALYSIS_PHASE_REGISTER(SCCFoo, foo)
}  // namespace maple

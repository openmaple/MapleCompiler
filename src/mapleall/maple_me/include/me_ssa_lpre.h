/*
 * Copyright (c) [2020-2021] Huawei Technologies Co.,Ltd.All rights reserved.
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
#ifndef MAPLE_ME_INCLUDE_ME_SSA_LPRE_H
#define MAPLE_ME_INCLUDE_ME_SSA_LPRE_H
#include "ssa_pre.h"
#include "me_irmap.h"
#include "me_loop_analysis.h"

namespace maple {
constexpr size_t kDoLpreBBsLimit = 0x7fffff;
class MeSSALPre : public SSAPre {
 public:
  MeSSALPre(MeFunction &f, MeIRMap &hMap, Dominance &dom, MemPool &memPool, MemPool &mp2, PreKind kind, uint32 limit)
      : SSAPre(hMap, dom, memPool, mp2, kind, limit),
        irMap(&hMap),
        func(&f),
        assignedFormals(ssaPreAllocator.Adapter()),
        loopHeadBBs(ssaPreAllocator.Adapter()),
        candsForSSAUpdate(ssaPreAllocator.Adapter()) {}

  virtual ~MeSSALPre() = default;
  void FindLoopHeadBBs(const IdentifyLoops &identLoops);

  MapleMap<OStIdx, MapleSet<BBId>*> &GetCandsForSSAUpdate() {
    return candsForSSAUpdate;
  }

  void EnterCandsForSSAUpdate(OStIdx ostIdx, const BB &bb) override {
    if (candsForSSAUpdate.find(ostIdx) == candsForSSAUpdate.end()) {
      MapleSet<BBId> *bbSet = ssaPreMemPool->New<MapleSet<BBId>>(std::less<BBId>(), ssaPreAllocator.Adapter());
      (void)bbSet->insert(bb.GetBBId());
      candsForSSAUpdate[ostIdx] = bbSet;
    } else {
      (void)candsForSSAUpdate[ostIdx]->insert(bb.GetBBId());
    }
  }
 private:
  void GenerateSaveRealOcc(MeRealOcc&) override;
  MeExpr *GetTruncExpr(const VarMeExpr &theLHS, MeExpr &savedRHS);
  void GenerateReloadRealOcc(MeRealOcc&) override;
  MeExpr *PhiOpndFromRes(MeRealOcc&, size_t) const override;
  void ComputeVarAndDfPhis() override;
  bool ScreenPhiBB(BBId) const override {
    return true;
  }

  void CollectVarForMeExpr(MeExpr &meExpr, std::vector<MeExpr*> &varVec) const override {
    if (meExpr.GetMeOp() == kMeOpAddrof ||
        meExpr.GetMeOp() == kMeOpAddroffunc ||
        meExpr.GetMeOp() == kMeOpConst) {
      return;
    }
    varVec.push_back(&meExpr);
  }

  void CollectVarForCand(MeRealOcc &realOcc, std::vector<MeExpr*> &varVec) const override {
    if (realOcc.GetMeExpr()->GetMeOp() == kMeOpAddrof ||
        realOcc.GetMeExpr()->GetMeOp() == kMeOpAddroffunc ||
        realOcc.GetMeExpr()->GetMeOp() == kMeOpConst) {
      return;
    }
    varVec.push_back(realOcc.GetMeExpr());
  }

  void BuildEntryLHSOcc4Formals() const override;
  void BuildWorkListLHSOcc(MeStmt &meStmt, int32 seqStmt) override;
  void CreateMembarOccAtCatch(BB &bb) override;
  void BuildWorkListExpr(MeStmt &meStmt, int32 seqStmt, MeExpr &meExpr, bool isRebuild, MeExpr *tmpVar,
                         bool isRootExpr, bool insertSorted) override;
  void BuildWorkList() override;
  BB *GetBB(BBId id) const override {
    return func->GetCfg()->GetBBFromID(id);
  }

  PUIdx GetPUIdx() const override {
    return func->GetMirFunc()->GetPuidx();
  }

  bool IsLoopHeadBB(BBId bbId) const override {
    return loopHeadBBs.find(bbId) != loopHeadBBs.end();
  }

  MeIRMap *irMap;
  MeFunction *func;
  MapleSet<OStIdx> assignedFormals;  // set of formals that are assigned
  MapleSet<BBId> loopHeadBBs;
  MapleMap<OStIdx, MapleSet<BBId>*> candsForSSAUpdate;
};

MAPLE_FUNC_PHASE_DECLARE(MESSALPre, MeFunction)
}  // namespace maple
#endif  // MAPLE_ME_INCLUDE_ME_SSA_LPRE_H

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

#ifndef MAPLE_ME_INCLUDE_DEMAND_DRIVEN_ALIAS_ANALYSIS_H
#define MAPLE_ME_INCLUDE_DEMAND_DRIVEN_ALIAS_ANALYSIS_H
#include <map>
#include <vector>
#include <bitset>
#include <array>
#include "me_function.h"
#include "fstream"

namespace maple {
enum AliasAttribute {
  kAliasAttrNotAllDefsSeen = 0, // var pointed to by ptr with unknown value
  kAliasAttrGlobal = 1, // global var
  kAliasAttrNextLevNotAllDefsSeen = 2, // ptr with unknown value
  kAliasAttrFormal = 3, // ptr from formal
  kAliasAttrEscaped = 4, // may val alias with unknown var
  kEndOfAliasAttr
};

class PEGNode; // circular dependency exists, no other choice
class PtrValueNode {
 public:
  PtrValueNode(PEGNode *node, OffsetType offset) : pegNode(node), offset(offset) {}
  PtrValueNode() = default;
  ~PtrValueNode() = default;

  bool operator==(const PtrValueNode &other) const {
    return pegNode == other.pegNode && offset == other.offset;
  }

  PEGNode *pegNode = nullptr;
  OffsetType offset = OffsetType(0);
};

// node of program expresion graph
using AliasAttr = std::bitset<kEndOfAliasAttr>;
class PEGNode {
 public:
  explicit PEGNode(OriginalSt *ost) : ost(ost) {
    if (ost->GetPrevLevelOst() != nullptr) {
      multiDefed = true;
    }
  }
  ~PEGNode() = default;

  void SetPrevLevelNode(PEGNode *node) {
    prevLevNode = node;
  }

  void AddNextLevelNode(PEGNode *node) {
    const auto &it = std::find(nextLevNodes.begin(), nextLevNodes.end(), node);
    if (it == nextLevNodes.end()) {
      nextLevNodes.emplace_back(node);
    }
  }

  void AddAssignFromNode(PEGNode *node, OffsetType offset) {
    PtrValueNode assignFromNode(node, offset);
    const auto &it = std::find(assignFrom.begin(), assignFrom.end(), assignFromNode);
    if (it == assignFrom.end()) {
      assignFrom.emplace_back(assignFromNode);
    }
  }

  void AddAssignToNode(PEGNode *node, OffsetType offset) {
    PtrValueNode assignToNode(node, offset);
    const auto &it = std::find(assignTo.begin(), assignTo.end(), assignToNode);
    if (it == assignTo.end()) {
      assignTo.emplace_back(assignToNode);
    }
  }

  void CopyAttrFromValueAliasedNode(const PEGNode *other) {
    attr[kAliasAttrNextLevNotAllDefsSeen] =
        attr[kAliasAttrNextLevNotAllDefsSeen] | other->attr[kAliasAttrNextLevNotAllDefsSeen];
    attr[kAliasAttrEscaped] = attr[kAliasAttrEscaped] | other->attr[kAliasAttrEscaped];
  }

  void SetMultiDefined() {
    multiDefed = true;
  }

  void Dump();

  OriginalSt *ost = nullptr;
  AliasAttr attr = {false};
  std::vector<PtrValueNode> assignFrom;
  std::vector<PtrValueNode> assignTo;
  PEGNode *prevLevNode = nullptr;
  std::vector<PEGNode*> nextLevNodes;
  bool multiDefed = false;
  bool processed = false;
};

class ProgramExprGraph {
 public:
  ProgramExprGraph(MemPool *memPool) : memPool(memPool) {}
  ~ProgramExprGraph() {
    allNodes.clear();
  }

  void AddAssignEdge(PEGNode *lhs, PEGNode *rhs, OffsetType offset) {
    ASSERT(idOfVal2IdOfNode.find(lhs->ost->GetIndex()) != idOfVal2IdOfNode.end(), "");
    ASSERT(idOfVal2IdOfNode.find(rhs->ost->GetIndex()) != idOfVal2IdOfNode.end(), "");
    lhs->AddAssignFromNode(rhs, offset);
    rhs->AddAssignToNode(lhs, offset);
  }

  std::vector<PEGNode*> &GetAllNodes() {
    return allNodes;
  }

  PEGNode *GetOrCreateNodeOf(OriginalSt *ost);
  PEGNode *GetNodeOf(OriginalSt *ost) const;
  void Dump() const;

 private:
  MemPool *memPool;
  std::vector<PEGNode*> allNodes; // index is id of PEGNode
  std::map<OStIdx, uint32> idOfVal2IdOfNode;
};

class PEGBuilder {
 public:
  PEGBuilder(MeFunction *func, ProgramExprGraph *programExprGraph, SSATab *ssaTab)
      : func(func), ssaTab(ssaTab), peg(programExprGraph) {}

  ~PEGBuilder() {}

  class PtrValueRecorder {
   public:
    PtrValueRecorder(PEGNode *node, FieldID id, OffsetType offset = OffsetType(0))
        : pegNode(node), fieldId(id), offset(offset) {}
    ~PtrValueRecorder() = default;

    PEGNode *pegNode;
    FieldID fieldId;
    OffsetType offset;
  };

  PtrValueRecorder BuildPEGNodeOfExpr(const BaseNode *expr);
  void AddAssignEdge(const StmtNode *stmt, PEGNode *lhsNode, PEGNode *rhsNode, OffsetType offset);
  void BuildPEGNodeInStmt(const StmtNode *stmt);
  void BuildPEG();

 private:
  void UpdateAttributes();
  PtrValueRecorder BuildPEGNodeOfDread(const AddrofSSANode *dread);
  PtrValueRecorder BuildPEGNodeOfAddrof(const AddrofSSANode *dread);
  PtrValueRecorder BuildPEGNodeOfRegread(const RegreadSSANode *regread);
  PtrValueRecorder BuildPEGNodeOfIread(const IreadSSANode *iread);
  PtrValueRecorder BuildPEGNodeOfIaddrof(const IreadNode *iaddrof);
  PtrValueRecorder BuildPEGNodeOfAdd(const BinaryNode *binaryNode);
  PtrValueRecorder BuildPEGNodeOfArray(const ArrayNode *arrayNode);
  PtrValueRecorder BuildPEGNodeOfSelect(const  TernaryNode* selectNode);
  PtrValueRecorder BuildPEGNodeOfIntrisic(const IntrinsicopNode* intrinsicNode);

  void BuildPEGNodeInAssign(const StmtNode *stmt);
  void BuildPEGNodeInIassign(const IassignNode *iassign);
  void BuildPEGNodeInDirectCall(const CallNode *call);
  void BuildPEGNodeInIcall(const IcallNode *icall);
  void BuildPEGNodeInVirtualcall(const NaryStmtNode *virtualCall);
  void BuildPEGNodeInIntrinsicCall(const IntrinsiccallNode *intrinsiccall);

  MeFunction *func;
  SSATab *ssaTab = nullptr;
  ProgramExprGraph *peg;
};

class DemandDrivenAliasAnalysis {
 public:
  DemandDrivenAliasAnalysis(MeFunction *fn, SSATab *ssaTab, MemPool *mp, bool debugDDAA)
      : func(fn),
        ssaTab(ssaTab),
        tmpMP(mp),
        tmpAlloc(tmpMP),
        reachNodes(tmpAlloc.Adapter()),
        aliasSets(tmpAlloc.Adapter()),
        peg(tmpMP),
        enableDebug(debugDDAA) {
    BuildPEG();
  }

  ~DemandDrivenAliasAnalysis() = default;

  enum ReachState {
    S1 = 0,
    S2 = 1,
    S3 = 2,
    S4 = 3,
  };

  class ReachItem {
   public:
    ReachItem(PEGNode *src, ReachState state, OffsetType offset = OffsetType(0))
        : src(src), state(state), offset(offset) {}

    bool operator<(const ReachItem &other) const {
      return (src < other.src) && (state < other.state) && (offset < other.offset);
    }

    bool operator==(const ReachItem &other) const {
      return (src == other.src) && (state == other.state) && (offset == other.offset);
    }

    PEGNode *src;
    ReachState state;
    OffsetType offset;
  };

  struct ReachItemComparator {
    bool operator()(const ReachItem *itemA, const ReachItem *itemB) const {
      if (itemA->src->ost->GetIndex() < itemB->src->ost->GetIndex()) {
        return true;
      } else if (itemA->src->ost->GetIndex() > itemB->src->ost->GetIndex()) {
        return false;
      }
      return (itemA->state < itemB->state);
    }
  };

  class WorkListItem {
   public:
    WorkListItem(PEGNode *to, PEGNode *src, ReachState state, OffsetType offset = OffsetType(0))
        : to(to), srcItem(src, state, offset) {}

    PEGNode *to;
    ReachItem srcItem;
  };

  using WorkListType = std::list<WorkListItem>;
  bool MayAlias(PEGNode *to, PEGNode *src);
  bool MayAlias(OriginalSt *ostA, OriginalSt *ostB);

  void BuildPEG() {
    PEGBuilder builder(func, &peg, ssaTab);
    builder.BuildPEG();
    if (enableDebug) {
      peg.Dump();
    }
  }

 private:
  MapleSet<ReachItem*, ReachItemComparator> *ReachSetOf(PEGNode *node) {
    auto *reachSet = reachNodes[node];
    if (reachSet == nullptr) {
      reachSet = tmpMP->New<MapleSet<ReachItem*, ReachItemComparator>>(tmpAlloc.Adapter());
      reachNodes[node] = reachSet;
      return reachSet;
    }
    return reachSet;
  }

  bool AddReachNode(PEGNode *to, PEGNode *src, ReachState state, OffsetType offset) {
    auto *reachSet = ReachSetOf(to);
    auto reachItem = tmpMP->New<ReachItem>(src, state, offset);
    const auto &insertedReachItemPair = reachSet->insert(reachItem);
    if (!insertedReachItemPair.second && (*(insertedReachItemPair.first))->offset != offset) {
      (*insertedReachItemPair.first)->offset.Set(kOffsetUnknown);
      to->SetMultiDefined();
    }
    return insertedReachItemPair.second;
  }

  void Propagate(WorkListType &workList, PEGNode *to, PEGNode *src, ReachState state,
                 OffsetType offset = OffsetType(0));
  void UpdateAliasInfoOfPegNode(PEGNode *pegNode);

  MapleSet<OriginalSt*> *AliasSetOf(OriginalSt *ost) {
    auto it = aliasSets.find(ost);
    if (it->second == nullptr || it == aliasSets.end()) {
      auto *aliasSet = tmpMP->New<MapleSet<OriginalSt*>>(tmpAlloc.Adapter());
      aliasSets[ost] = aliasSet;
      return aliasSet;
    }
    return it->second;
  }

  void AddAlias(OriginalSt *ostA, OriginalSt *ostB) {
    AliasSetOf(ostA)->insert(ostB);
    AliasSetOf(ostB)->insert(ostA);
  }

  bool AliasBasedOnAliasAttr(PEGNode *to, PEGNode *src) const;

  bool ReachFromSrc(PEGNode *to, PEGNode *src) {
    auto *reachSet = reachNodes[to];
    if (reachSet == nullptr) {
      return false;
    }
    for (const auto *item : *reachSet) {
      if (item->src == src) {
        return true;
      }
    }
    return false;
  }

  PEGNode *GetPrevLevPEGNode(PEGNode *pegNode) const {
    CHECK_FATAL(pegNode != nullptr, "null ptr check");
    return pegNode->prevLevNode;
  }

  MeFunction *func;
  SSATab *ssaTab;
  MemPool *tmpMP;
  MapleAllocator tmpAlloc;
  MapleMap<PEGNode *, MapleSet<ReachItem *, ReachItemComparator> *> reachNodes; //
  MapleMap<OriginalSt *, MapleSet<OriginalSt *> *> aliasSets; // ost in MapleSet memory-alias with the key ost
  ProgramExprGraph peg;
  bool enableDebug;
};
} // namespace maple
#endif //MAPLE_ME_INCLUDE_DEMAND_DRIVEN_ALIAS_ANALYSIS_H

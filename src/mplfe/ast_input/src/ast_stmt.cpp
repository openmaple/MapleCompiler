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
#include "ast_stmt.h"
#include "ast_decl.h"
#include "ast_util.h"
#include "mpl_logging.h"
#include "feir_stmt.h"
#include "feir_builder.h"
#include "fe_utils_ast.h"
#include "fe_manager.h"
#include "ast_util.h"

namespace maple {
// ---------- ASTStmt ----------
void ASTStmt::SetASTExpr(ASTExpr *astExpr) {
  exprs.emplace_back(astExpr);
}

void ASTCompoundStmt::SetASTStmt(ASTStmt *astStmt) {
  astStmts.emplace_back(astStmt);
}

const std::list<ASTStmt*> &ASTCompoundStmt::GetASTStmtList() const {
  return astStmts;
}

std::list<UniqueFEIRStmt> ASTCompoundStmt::Emit2FEStmtImpl() const {
  std::list<UniqueFEIRStmt> stmts;
  for (auto it : astStmts) {
    stmts.splice(stmts.end(), it->Emit2FEStmt());
  }
  return stmts;
}

std::list<UniqueFEIRStmt> ASTReturnStmt::Emit2FEStmtImpl() const {
  std::list<UniqueFEIRStmt> stmts;
  auto astExpr = exprs.front();
  UniqueFEIRExpr feExpr = (astExpr != nullptr) ? astExpr->Emit2FEExpr(stmts) : nullptr;
  UniqueFEIRStmt stmt = std::make_unique<FEIRStmtReturn>(std::move(feExpr));
  stmts.emplace_back(std::move(stmt));
  return stmts;
}

std::list<UniqueFEIRStmt> ASTIfStmt::Emit2FEStmtImpl() const {
  std::list<UniqueFEIRStmt> stmts;
  std::list<UniqueFEIRStmt> thenStmts;
  std::list<UniqueFEIRStmt> elseStmts;
  if (thenStmt != nullptr) {
    thenStmts = thenStmt->Emit2FEStmt();
  }
  if (elseStmt != nullptr) {
    elseStmts = elseStmt->Emit2FEStmt();
  }
  UniqueFEIRExpr condFEExpr = condExpr->Emit2FEExpr(stmts);
  UniqueFEIRStmt ifStmt;
  ifStmt = FEIRBuilder::CreateStmtIf(std::move(condFEExpr), thenStmts, elseStmts);
  stmts.emplace_back(std::move(ifStmt));
  return stmts;
}

std::list<UniqueFEIRStmt> ASTForStmt::Emit2FEStmtImpl() const {
  std::list<UniqueFEIRStmt> stmts;
  std::string loopBodyEndLabelName = FEUtils::GetSequentialName("dowhile_body_end_");
  std::string loopEndLabelName = FEUtils::GetSequentialName("dowhile_end_");
  AstLoopUtil::Instance().PushLoop(std::make_pair(loopBodyEndLabelName, loopEndLabelName));
  auto labelBodyEndStmt = std::make_unique<FEIRStmtLabel>(loopBodyEndLabelName);
  auto labelLoopEndStmt = std::make_unique<FEIRStmtLabel>(loopEndLabelName);
  if (initStmt != nullptr) {
    std::list<UniqueFEIRStmt> feStmts = initStmt->Emit2FEStmt();
    stmts.splice(stmts.cend(), feStmts);
  }
  std::list<UniqueFEIRStmt> bodyFEStmts = bodyStmt->Emit2FEStmt();
  bodyFEStmts.emplace_back(std::move(labelBodyEndStmt));
  UniqueFEIRExpr condFEExpr;
  if (condExpr != nullptr) {
    condFEExpr = condExpr->Emit2FEExpr(stmts);
    std::list<UniqueFEIRStmt> condStmts;
    condFEExpr = condExpr->Emit2FEExpr(condStmts);
    bodyFEStmts.splice(bodyFEStmts.cend(), condStmts);
  } else {
    condFEExpr = std::make_unique<FEIRExprConst>(static_cast<int64>(1), PTY_i32);
  }
  if (incExpr != nullptr) {
    std::list<UniqueFEIRExpr> exprs;
    std::list<UniqueFEIRStmt> incStmts;
    UniqueFEIRExpr incFEExpr = incExpr->Emit2FEExpr(incStmts);
    exprs.emplace_back(std::move(incFEExpr));
    auto incStmt = std::make_unique<FEIRStmtNary>(OP_eval, std::move(exprs));
    incStmts.emplace_back(std::move(incStmt));
    bodyFEStmts.splice(bodyFEStmts.cend(), incStmts);
  }
  UniqueFEIRStmt whileStmt = std::make_unique<FEIRStmtDoWhile>(OP_while, std::move(condFEExpr), std::move(bodyFEStmts));
  stmts.emplace_back(std::move(whileStmt));
  stmts.emplace_back(std::move(labelLoopEndStmt));
  AstLoopUtil::Instance().PopCurrentLoop();
  return stmts;
}

std::list<UniqueFEIRStmt> ASTWhileStmt::Emit2FEStmtImpl() const {
  std::list<UniqueFEIRStmt> stmts;
  std::string loopBodyEndLabelName = FEUtils::GetSequentialName("dowhile_body_end_");
  std::string loopEndLabelName = FEUtils::GetSequentialName("dowhile_end_");
  AstLoopUtil::Instance().PushLoop(std::make_pair(loopBodyEndLabelName, loopEndLabelName));
  auto labelBodyEndStmt = std::make_unique<FEIRStmtLabel>(loopBodyEndLabelName);
  auto labelLoopEndStmt = std::make_unique<FEIRStmtLabel>(loopEndLabelName);
  std::list<UniqueFEIRStmt> bodyFEStmts = bodyStmt->Emit2FEStmt();
  std::list<UniqueFEIRStmt> condStmts;
  std::list<UniqueFEIRStmt> condPreStmts;
  UniqueFEIRExpr condFEExpr = condExpr->Emit2FEExpr(condStmts);
  (void)condExpr->Emit2FEExpr(condPreStmts);
  bodyFEStmts.emplace_back(std::move(labelBodyEndStmt));
  bodyFEStmts.splice(bodyFEStmts.end(), condPreStmts);
  auto whileStmt = std::make_unique<FEIRStmtDoWhile>(OP_while, std::move(condFEExpr), std::move(bodyFEStmts));
  stmts.splice(stmts.end(), condStmts);
  stmts.emplace_back(std::move(whileStmt));
  stmts.emplace_back(std::move(labelLoopEndStmt));
  AstLoopUtil::Instance().PopCurrentLoop();
  return stmts;
}

std::list<UniqueFEIRStmt> ASTDoStmt::Emit2FEStmtImpl() const {
  std::list<UniqueFEIRStmt> stmts;
  std::string loopBodyEndLabelName = FEUtils::GetSequentialName("dowhile_body_end_");
  std::string loopEndLabelName = FEUtils::GetSequentialName("dowhile_end_");
  AstLoopUtil::Instance().PushLoop(std::make_pair(loopBodyEndLabelName, loopEndLabelName));
  auto labelBodyEndStmt = std::make_unique<FEIRStmtLabel>(loopBodyEndLabelName);
  auto labelLoopEndStmt = std::make_unique<FEIRStmtLabel>(loopEndLabelName);
  std::list<UniqueFEIRStmt> bodyFEStmts;
  if (bodyStmt != nullptr) {
    bodyFEStmts = bodyStmt->Emit2FEStmt();
  }
  bodyFEStmts.emplace_back(std::move(labelBodyEndStmt));
  std::list<UniqueFEIRStmt> condStmts;
  UniqueFEIRExpr condFEExpr = condExpr->Emit2FEExpr(condStmts);
  bodyFEStmts.splice(bodyFEStmts.end(), condStmts);
  UniqueFEIRStmt whileStmt = std::make_unique<FEIRStmtDoWhile>(OP_dowhile, std::move(condFEExpr),
                                                               std::move(bodyFEStmts));
  stmts.emplace_back(std::move(whileStmt));
  stmts.emplace_back(std::move(labelLoopEndStmt));
  AstLoopUtil::Instance().PopCurrentLoop();
  return stmts;
}

std::list<UniqueFEIRStmt> ASTBreakStmt::Emit2FEStmtImpl() const {
  std::list<UniqueFEIRStmt> stmts;
  auto stmt = std::make_unique<FEIRStmtBreak>();
  if (!AstLoopUtil::Instance().IsLoopLabelsEmpty()) {
    stmt->SetLoopLabelName(AstLoopUtil::Instance().GetCurrentLoop().second);
  }
  stmts.emplace_back(std::move(stmt));
  return stmts;
}

std::list<UniqueFEIRStmt> ASTLabelStmt::Emit2FEStmtImpl() const {
  std::list<UniqueFEIRStmt> stmts;
  auto feStmt = std::make_unique<FEIRStmtLabel>(labelName);
  stmts.emplace_back(std::move(feStmt));
  stmts.splice(stmts.end(), subStmt->Emit2FEStmt());
  return stmts;
}

std::list<UniqueFEIRStmt> ASTContinueStmt::Emit2FEStmtImpl() const {
  std::list<UniqueFEIRStmt> stmts;
  auto stmt = std::make_unique<FEIRStmtContinue>();
  stmt->SetLabelName(AstLoopUtil::Instance().GetCurrentLoop().first);
  stmts.emplace_back(std::move(stmt));
  return stmts;
}

// ---------- ASTUnaryOperatorStmt ----------
std::list<UniqueFEIRStmt> ASTUnaryOperatorStmt::Emit2FEStmtImpl() const {
  std::list<UniqueFEIRStmt> stmts;
  auto astExpr = exprs.front();
  astExpr->Emit2FEExpr(stmts);
  return stmts;
}

// ---------- ASTGotoStmt ----------
std::list<UniqueFEIRStmt> ASTGotoStmt::Emit2FEStmtImpl() const {
  std::list<UniqueFEIRStmt> stmts;
  UniqueFEIRStmt stmt = FEIRBuilder::CreateStmtGoto(labelName);
  stmts.emplace_back(std::move(stmt));
  return stmts;
}

// ---------- ASTSwitchStmt ----------
std::list<UniqueFEIRStmt> ASTSwitchStmt::Emit2FEStmtImpl() const {
  std::list<UniqueFEIRStmt> stmts;
  UniqueFEIRExpr expr = condExpr->Emit2FEExpr(stmts);
  auto switchStmt = std::make_unique<FEIRStmtSwitchForC>(std::move(expr), hasDefualt);
  for (auto &s : bodyStmt->Emit2FEStmt()) {
    switchStmt.get()->AddFeirStmt(std::move(s));
  }
  stmts.emplace_back(std::move(switchStmt));
  return stmts;
}

// ---------- ASTCaseStmt ----------
std::list<UniqueFEIRStmt> ASTCaseStmt::Emit2FEStmtImpl() const {
  std::list<UniqueFEIRStmt> stmts;
  auto caseStmt = std::make_unique<FEIRStmtCaseForC>(lCaseTag);
  caseStmt.get()->AddCaseTag2CaseVec(lCaseTag, rCaseTag);
  for (auto &s : subStmt->Emit2FEStmt()) {
    caseStmt.get()->AddFeirStmt(std::move(s));
  }
  stmts.emplace_back(std::move(caseStmt));
  return stmts;
}

// ---------- ASTDefaultStmt ----------
std::list<UniqueFEIRStmt> ASTDefaultStmt::Emit2FEStmtImpl() const {
  std::list<UniqueFEIRStmt> stmts;
  auto defalutStmt = std::make_unique<FEIRStmtDefaultForC>();
  for (auto &s : child->Emit2FEStmt()) {
    defalutStmt.get()->AddFeirStmt(std::move(s));
  }
  stmts.emplace_back(std::move(defalutStmt));
  return stmts;
}

// ---------- ASTNullStmt ----------
std::list<UniqueFEIRStmt> ASTNullStmt::Emit2FEStmtImpl() const {
  // there is no need to process this stmt
  std::list<UniqueFEIRStmt> stmts;
  return stmts;
}

// ---------- ASTDeclStmt ----------
std::list<UniqueFEIRStmt> ASTDeclStmt::Emit2FEStmtImpl() const {
  std::list<UniqueFEIRStmt> stmts;
  for (auto decl : subDecls) {
    decl->GenerateInitStmt(stmts);
  }
  return stmts;
}

// ---------- ASTCallExprStmt ----------
std::map<std::string, ASTCallExprStmt::FuncPtrBuiltinFunc> ASTCallExprStmt::funcPtrMap =
    ASTCallExprStmt::InitFuncPtrMap();

std::map<std::string, ASTCallExprStmt::FuncPtrBuiltinFunc> ASTCallExprStmt::InitFuncPtrMap() {
  std::map<std::string, FuncPtrBuiltinFunc> ans;
  ans["__builtin_va_start"] = &ASTCallExprStmt::ProcessBuiltinVaStart;
  ans["__builtin_va_end"] = &ASTCallExprStmt::ProcessBuiltinVaEnd;
  return ans;
}

std::list<UniqueFEIRStmt> ASTCallExprStmt::Emit2FEStmtCall() const {
  std::list<UniqueFEIRStmt> stmts;
  ASTCallExpr *callExpr = static_cast<ASTCallExpr*>(exprs.front());
  // callassigned &funcName
  std::string funcName = callExpr->GetFuncName();
  StructElemNameIdx *nameIdx = FEManager::GetManager().GetStructElemMempool()->New<StructElemNameIdx>(funcName);
  FEStructMethodInfo *info = static_cast<FEStructMethodInfo*>(
      FEManager::GetTypeManager().RegisterStructMethodInfo(*nameIdx, kSrcLangC, false));
  std::unique_ptr<FEIRStmtCallAssign> callStmt = std::make_unique<FEIRStmtCallAssign>(
      *info, OP_callassigned, nullptr, false);
  // args
  std::vector<ASTExpr*> argsExprs = callExpr->GetArgsExpr();
  for (int32 i = argsExprs.size() - 1; i >= 0; --i) {
    UniqueFEIRExpr expr = argsExprs[i]->Emit2FEExpr(stmts);
    callStmt->AddExprArgReverse(std::move(expr));
  }
  // return
  MIRType *retType = callExpr->GetRetType();
  FEIRTypeNative *retTypeInfo = FEManager::GetManager().GetModule().GetMemPool()->New<FEIRTypeNative>(*retType);
  info->SetReturnType(retTypeInfo);
  if (retType->GetPrimType() != PTY_void) {
    const std::string &varName = FEUtils::GetSequentialName("retVar_");
    UniqueFEIRVar var = FEIRBuilder::CreateVarNameForC(varName, *retType, false, false);
    callStmt->SetVar(std::move(var));
  }
  stmts.emplace_back(std::move(callStmt));
  return stmts;
}

std::list<UniqueFEIRStmt> ASTCallExprStmt::Emit2FEStmtICall() const {
  std::list<UniqueFEIRStmt> stmts;
  std::unique_ptr<FEIRStmtICallAssign> icallStmt = std::make_unique<FEIRStmtICallAssign>();
  ASTCallExpr *callExpr = static_cast<ASTCallExpr*>(exprs.front());
  ASTExpr *calleeExpr = callExpr->GetCalleeExpr();
  CHECK_NULL_FATAL(calleeExpr);
  // args
  UniqueFEIRExpr expr = calleeExpr->Emit2FEExpr(stmts);
  icallStmt->AddExprArgReverse(std::move(expr));
  // return
  MIRType *retType = callExpr->GetRetType();
  if (retType->GetPrimType() != PTY_void) {
    const std::string &varName = FEUtils::GetSequentialName("retVar_");
    UniqueFEIRVar var = FEIRBuilder::CreateVarNameForC(varName, *retType, false, false);
    icallStmt->SetVar(std::move(var));
  }
  stmts.emplace_back(std::move(icallStmt));
  return stmts;
}

std::list<UniqueFEIRStmt> ASTCallExprStmt::Emit2FEStmtImpl() const {
  ASTCallExpr *callExpr = static_cast<ASTCallExpr*>(exprs.front());
  if (callExpr->IsIcall()) {
    return Emit2FEStmtICall();
  } else {
    if (callExpr->GetCalleeExpr() != nullptr && callExpr->GetCalleeExpr()->GetASTOp() == kASTOpCast &&
        static_cast<ASTImplicitCastExpr*>(callExpr->GetCalleeExpr())->IsBuilinFunc()) {
      auto ptrFunc = funcPtrMap.find(callExpr->GetFuncName());
      if (ptrFunc != funcPtrMap.end()) {
        return (this->*(ptrFunc->second))();
      }
    }
    return Emit2FEStmtCall();
  }
}

std::list<UniqueFEIRStmt> ASTCallExprStmt::ProcessBuiltinVaStart() const {
  std::list<UniqueFEIRStmt> stmts;
  ASTCallExpr *callExpr = static_cast<ASTCallExpr*>(exprs.front());
  // args
  std::vector<ASTExpr*> argsExprs = callExpr->GetArgsExpr();
  auto exprArgList = std::make_unique<std::list<UniqueFEIRExpr>>();
  for (int32 i = argsExprs.size() - 1; i >= 0; --i) {
    UniqueFEIRExpr expr = argsExprs[i]->Emit2FEExpr(stmts);
    // addrof va_list instead of dread va_list
    if (i == 0 && expr->GetKind() == kExprDRead) {
      UniqueFEIRVar var = static_cast<FEIRExprDRead*>(expr.get())->GetVar()->Clone();
      expr = FEIRBuilder::CreateExprAddrofVar(std::move(var));
    }
    exprArgList->push_front(std::move(expr));
  }
#ifndef USE_OPS
    CHECK_FATAL(false, "implemention in ops branch");
#else
  std::unique_ptr<FEIRStmtIntrinsicCallAssign> stmt = std::make_unique<FEIRStmtIntrinsicCallAssign>(
      INTRN_C_va_start, nullptr /* type */, nullptr /* retVar */, std::move(exprArgList));
  stmts.emplace_back(std::move(stmt));
#endif
  return stmts;
}

std::list<UniqueFEIRStmt> ASTCallExprStmt::ProcessBuiltinVaEnd() const {
  std::list<UniqueFEIRStmt> stmts;
  ASTCallExpr *callExpr = static_cast<ASTCallExpr*>(exprs.front());
  // args
  std::vector<ASTExpr*> argsExprs = callExpr->GetArgsExpr();
  std::list<UniqueFEIRExpr> exprArgList;
  for (int32 i = argsExprs.size() - 1; i >= 0; --i) {
    UniqueFEIRExpr expr = argsExprs[i]->Emit2FEExpr(stmts);
    // addrof va_list instead of dread va_list
    if (i == 0 && expr->GetKind() == kExprDRead) {
      UniqueFEIRVar var = static_cast<FEIRExprDRead*>(expr.get())->GetVar()->Clone();
      expr = FEIRBuilder::CreateExprAddrofVar(std::move(var));
    }
    exprArgList.push_front(std::move(expr));
  }
  auto stmt = std::make_unique<FEIRStmtNary>(OP_eval, std::move(exprArgList));
  stmts.emplace_back(std::move(stmt));
  return stmts;
}

// ---------- ASTImplicitCastExprStmt ----------
std::list<UniqueFEIRStmt> ASTImplicitCastExprStmt::Emit2FEStmtImpl() const {
  CHECK_FATAL(exprs.size() == 1, "Only one sub expr supported!");
  std::list<UniqueFEIRStmt> stmts;
  UniqueFEIRExpr feirExpr = exprs.front()->Emit2FEExpr(stmts);
  std::list<UniqueFEIRExpr> feirExprs;
  feirExprs.emplace_back(std::move(feirExpr));
  auto stmt = std::make_unique<FEIRStmtNary>(OP_eval, std::move(feirExprs));
  stmts.emplace_back(std::move(stmt));
  return stmts;
}

// ---------- ASTParenExprStmt ----------
std::list<UniqueFEIRStmt> ASTParenExprStmt::Emit2FEStmtImpl() const {
  std::list<UniqueFEIRStmt> stmts;
  std::list<UniqueFEIRExpr> feExprs;
  auto feExpr = exprs.front()->Emit2FEExpr(stmts);
  if (feExpr != nullptr) {
    feExprs.emplace_back(std::move(feExpr));
    auto stmt = std::make_unique<FEIRStmtNary>(OP_eval, std::move(feExprs));
    stmts.emplace_back(std::move(stmt));
  }
  return stmts;
}

// ---------- ASTIntegerLiteralStmt ----------
std::list<UniqueFEIRStmt> ASTIntegerLiteralStmt::Emit2FEStmtImpl() const {
  CHECK_FATAL(false, "NYI");
  std::list<UniqueFEIRStmt> stmts;
  return stmts;
}

// ---------- ASTVAArgExprStmt ----------
std::list<UniqueFEIRStmt> ASTVAArgExprStmt::Emit2FEStmtImpl() const {
  std::list<UniqueFEIRStmt> stmts;
  return stmts;
}

// ---------- ASTConditionalOperatorStmt ----------
std::list<UniqueFEIRStmt> ASTConditionalOperatorStmt::Emit2FEStmtImpl() const {
  std::list<UniqueFEIRStmt> stmts;
  auto astExpr = exprs.front();
  astExpr->Emit2FEExpr(stmts);
  return stmts;
}

// ---------- ASTCharacterLiteralStmt ----------
std::list<UniqueFEIRStmt> ASTCharacterLiteralStmt::Emit2FEStmtImpl() const {
  CHECK_FATAL(false, "NYI");
  std::list<UniqueFEIRStmt> stmts;
  return stmts;
}

// ---------- ASTStmtExprStmt ----------
std::list<UniqueFEIRStmt> ASTStmtExprStmt::Emit2FEStmtImpl() const {
  return cpdStmt->Emit2FEStmt();
}

// ---------- ASTCStyleCastExprStmt ----------
std::list<UniqueFEIRStmt> ASTCStyleCastExprStmt::Emit2FEStmtImpl() const {
  CHECK_FATAL(exprs.front() != nullptr, "child expr must not be nullptr!");
  std::list<UniqueFEIRStmt> stmts;
  exprs.front()->Emit2FEExpr(stmts);
  return stmts;
}

// ---------- ASTCompoundAssignOperatorStmt ----------
std::list<UniqueFEIRStmt> ASTCompoundAssignOperatorStmt::Emit2FEStmtImpl() const {
  CHECK_FATAL(exprs.size() == 1, "ASTCompoundAssignOperatorStmt must contain only one bo expr!");
  std::list<UniqueFEIRStmt> stmts;
  CHECK_FATAL(static_cast<ASTAssignExpr*>(exprs.front()) != nullptr, "Child expr must be ASTCompoundAssignOperator!");
  exprs.front()->Emit2FEExpr(stmts);
  return stmts;
}

std::list<UniqueFEIRStmt> ASTBinaryOperatorStmt::Emit2FEStmtImpl() const {
  CHECK_FATAL(exprs.size() == 1, "ASTBinaryOperatorStmt must contain only one bo expr!");
  std::list<UniqueFEIRStmt> stmts;
  auto boExpr = static_cast<ASTBinaryOperatorExpr*>(exprs.front());
  if (boExpr->GetASTOp() == kASTOpBO) {
    UniqueFEIRExpr boFEExpr = boExpr->Emit2FEExpr(stmts);
    std::list<UniqueFEIRExpr> exprs;
    exprs.emplace_back(std::move(boFEExpr));
    auto stmt = std::make_unique<FEIRStmtNary>(OP_eval, std::move(exprs));
    stmts.emplace_back(std::move(stmt));
  } else {
    // has been processed by child expr emit, skip here
    UniqueFEIRExpr boFEExpr = boExpr->Emit2FEExpr(stmts);
    return stmts;
  }
  return stmts;
}

// ---------- ASTAtomicExprStmt ----------
std::list<UniqueFEIRStmt> ASTAtomicExprStmt::Emit2FEStmtImpl() const {
  std::list<UniqueFEIRStmt> stmts;
  auto astExpr = exprs.front();
  UniqueFEIRExpr feExpr = astExpr->Emit2FEExpr(stmts);
  auto stmt = std::make_unique<FEIRStmtAtomic>(std::move(feExpr));
  stmts.emplace_back(std::move(stmt));
  return stmts;
}

// ---------- ASTGCCAsmStmt ----------
std::list<UniqueFEIRStmt> ASTGCCAsmStmt::Emit2FEStmtImpl() const {
  std::list<UniqueFEIRStmt> stmts;
  std::vector<UniqueFEIRExpr> outputsExprs;
  std::vector<UniqueFEIRExpr> inputsExprs;
  for (uint32 i = 0; i < numOfOutputs; ++i) {
    outputsExprs.emplace_back(exprs[i]->Emit2FEExpr(stmts));
  }
  for (uint32 i = 0; i < numOfInputs; ++i) {
    inputsExprs.emplace_back(exprs[i + numOfOutputs]->Emit2FEExpr(stmts));
  }
  // Translate asm info to FEIR and MIR.
  return stmts;
}
} // namespace maple

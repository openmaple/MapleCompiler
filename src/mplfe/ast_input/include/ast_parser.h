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
#ifndef MPLFE_AST_INPUT_INCLUDE_AST_PARSER_H
#define MPLFE_AST_INPUT_INCLUDE_AST_PARSER_H
#include <string>
#include <functional>
#include "mempool_allocator.h"
#include "ast_decl.h"
#include "ast_interface.h"

namespace maple {
class ASTInput;
class ASTParser {
 public:
  ASTParser(MapleAllocator &allocatorIn, uint32 fileIdxIn, const std::string &fileNameIn)
      : fileIdx(fileIdxIn), fileName(fileNameIn), globalVarDecles(allocatorIn.Adapter()),
        funcDecles(allocatorIn.Adapter()), recordDecles(allocatorIn.Adapter()),
        globalEnumDecles(allocatorIn.Adapter()) {}
  virtual ~ASTParser() = default;
  bool OpenFile();
  bool Verify();
  bool PreProcessAST();

  bool RetrieveStructs(MapleAllocator &allocator, MapleList<ASTStruct*> &structs);
  bool RetrieveFuncs(MapleAllocator &allocator, MapleList<ASTFunc*> &funcs);
  bool RetrieveGlobalVars(MapleAllocator &allocator, MapleList<ASTVar*> &vars);

  bool ProcessGlobalEnums(MapleAllocator &allocator, MapleList<ASTVar*> &vars);

  const std::string &GetSourceFileName() const;
  const uint32 GetFileIdx() const;
  void SetAstIn(ASTInput *in) {
    astIn = in;
  }
  // ProcessStmt
  ASTStmt *ProcessStmt(MapleAllocator &allocator, const clang::Stmt &stmt);
  ASTStmt *ProcessFunctionBody(MapleAllocator &allocator, const clang::CompoundStmt &cpdStmt);
#define PROCESS_STMT(CLASS) ProcessStmt##CLASS(MapleAllocator&, const clang::CLASS&)
  ASTStmt *PROCESS_STMT(UnaryOperator);
  ASTStmt *PROCESS_STMT(BinaryOperator);
  ASTStmt *PROCESS_STMT(CompoundAssignOperator);
  ASTStmt *PROCESS_STMT(ImplicitCastExpr);
  ASTStmt *PROCESS_STMT(ParenExpr);
  ASTStmt *PROCESS_STMT(IntegerLiteral);
  ASTStmt *PROCESS_STMT(VAArgExpr);
  ASTStmt *PROCESS_STMT(ConditionalOperator);
  ASTStmt *PROCESS_STMT(CharacterLiteral);
  ASTStmt *PROCESS_STMT(StmtExpr);
  ASTStmt *PROCESS_STMT(CallExpr);
  ASTStmt *PROCESS_STMT(ReturnStmt);
  ASTStmt *PROCESS_STMT(IfStmt);
  ASTStmt *PROCESS_STMT(ForStmt);
  ASTStmt *PROCESS_STMT(WhileStmt);
  ASTStmt *PROCESS_STMT(DoStmt);
  ASTStmt *PROCESS_STMT(BreakStmt);
  ASTStmt *PROCESS_STMT(ContinueStmt);
  ASTStmt *PROCESS_STMT(CompoundStmt);
  ASTStmt *PROCESS_STMT(GotoStmt);
  ASTStmt *PROCESS_STMT(SwitchStmt);
  ASTStmt *PROCESS_STMT(CaseStmt);
  ASTStmt *PROCESS_STMT(DefaultStmt);
  ASTStmt *PROCESS_STMT(NullStmt);
  ASTStmt *PROCESS_STMT(CStyleCastExpr);
  ASTStmt *PROCESS_STMT(DeclStmt);
  bool HasDefault(const clang::Stmt &stmt);

  // ProcessExpr
  const clang::Expr *PeelParen(const clang::Expr &expr);
  ASTUnaryOperatorExpr *AllocUnaryOperatorExpr(MapleAllocator &allocator, const clang::UnaryOperator &expr);
  ASTExpr *ProcessExpr(MapleAllocator &allocator, const clang::Expr *expr);
  ASTBinaryOperatorExpr *AllocBinaryOperatorExpr(MapleAllocator &allocator, const clang::BinaryOperator &bo);
#define PROCESS_EXPR(CLASS) ProcessExpr##CLASS(MapleAllocator&, const clang::CLASS&)
  ASTExpr *PROCESS_EXPR(UnaryOperator);
  ASTExpr *PROCESS_EXPR(NoInitExpr);
  ASTExpr *PROCESS_EXPR(PredefinedExpr);
  ASTExpr *PROCESS_EXPR(OpaqueValueExpr);
  ASTExpr *PROCESS_EXPR(BinaryConditionalOperator);
  ASTExpr *PROCESS_EXPR(CompoundLiteralExpr);
  ASTExpr *PROCESS_EXPR(OffsetOfExpr);
  ASTExpr *PROCESS_EXPR(InitListExpr);
  ASTExpr *PROCESS_EXPR(BinaryOperator);
  ASTExpr *PROCESS_EXPR(ImplicitValueInitExpr);
  ASTExpr *PROCESS_EXPR(StringLiteral);
  ASTExpr *PROCESS_EXPR(ArraySubscriptExpr);
  ASTExpr *PROCESS_EXPR(UnaryExprOrTypeTraitExpr);
  ASTExpr *PROCESS_EXPR(MemberExpr);
  ASTExpr *PROCESS_EXPR(DesignatedInitUpdateExpr);
  ASTExpr *PROCESS_EXPR(ImplicitCastExpr);
  ASTExpr *PROCESS_EXPR(DeclRefExpr);
  ASTExpr *PROCESS_EXPR(ParenExpr);
  ASTExpr *PROCESS_EXPR(IntegerLiteral);
  ASTExpr *PROCESS_EXPR(FloatingLiteral);
  ASTExpr *PROCESS_EXPR(CharacterLiteral);
  ASTExpr *PROCESS_EXPR(ConditionalOperator);
  ASTExpr *PROCESS_EXPR(VAArgExpr);
  ASTExpr *PROCESS_EXPR(GNUNullExpr);
  ASTExpr *PROCESS_EXPR(SizeOfPackExpr);
  ASTExpr *PROCESS_EXPR(UserDefinedLiteral);
  ASTExpr *PROCESS_EXPR(ShuffleVectorExpr);
  ASTExpr *PROCESS_EXPR(TypeTraitExpr);
  ASTExpr *PROCESS_EXPR(ConstantExpr);
  ASTExpr *PROCESS_EXPR(ImaginaryLiteral);
  ASTExpr *PROCESS_EXPR(CallExpr);
  ASTExpr *PROCESS_EXPR(CompoundAssignOperator);
  ASTExpr *PROCESS_EXPR(StmtExpr);
  ASTExpr *PROCESS_EXPR(CStyleCastExpr);
  ASTExpr *PROCESS_EXPR(ArrayInitLoopExpr);
  ASTExpr *PROCESS_EXPR(ArrayInitIndexExpr);
  ASTExpr *PROCESS_EXPR(ExprWithCleanups);
  ASTExpr *PROCESS_EXPR(MaterializeTemporaryExpr);
  ASTExpr *PROCESS_EXPR(SubstNonTypeTemplateParmExpr);
  ASTExpr *PROCESS_EXPR(DependentScopeDeclRefExpr);
  ASTExpr *PROCESS_EXPR(AtomicExpr);

ASTDecl *ProcessDecl(MapleAllocator &allocator, const clang::Decl &decl);
#define PROCESS_DECL(CLASS) ProcessDecl##CLASS##Decl(MapleAllocator &allocator, const clang::CLASS##Decl&)
  ASTDecl *PROCESS_DECL(Field);
  ASTDecl *PROCESS_DECL(Function);
  ASTDecl *PROCESS_DECL(Record);
  ASTDecl *PROCESS_DECL(Var);
  ASTDecl *PROCESS_DECL(Enum);

 private:
  void TraverseDecl(const clang::Decl *decl, std::function<void (clang::Decl*)> const &functor);
  ASTDecl *GetAstDeclOfDeclRefExpr(MapleAllocator &allocator, const clang::Expr &expr);
  void SetSourceFileInfo(clang::Decl *decl);
  uint32 fileIdx;
  const std::string fileName;
  std::unique_ptr<LibAstFile> astFile;
  AstUnitDecl *astUnitDecl = nullptr;
  MapleList<clang::Decl*> globalVarDecles;
  MapleList<clang::Decl*> funcDecles;
  MapleList<clang::Decl*> recordDecles;
  MapleList<clang::Decl*> globalEnumDecles;
  ASTInput *astIn = nullptr;

  uint32 srcFileNum = 2; // src files start from 2, 1 is mpl file
};
}  // namespace maple
#endif // MPLFE_AST_INPUT_INCLUDE_AST_PARSER_H

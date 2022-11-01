/*
 * @Author: Outsider
 * @Date: 2022-10-31 21:28:22
 * @LastEditors: Outsider
 * @LastEditTime: 2022-11-01 16:39:21
 * @Description: In User Settings Edit
 * @FilePath: /compiler/src/codegen.cc
 */
#include "codegen.hh"

llvm::LLVMContext TheContext;
static std::unique_ptr<llvm::Module> TheModule;
// static std::unique_ptr<llvm::IRBuilder<> > Builder;
// static std::map<std::string, llvm::Value *> NamedValues;
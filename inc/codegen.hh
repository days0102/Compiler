/*
 * @Author: Outsider
 * @Date: 2022-10-31 20:32:31
 * @LastEditors: Outsider
 * @LastEditTime: 2022-11-21 20:46:19
 * @Description: In User Settings Edit
 * @FilePath: /compiler/inc/codegen.hh
 */
#ifndef CODEGEN_H
#define CODEGEN_H

#include<map>
#include "llvm/IR/Constants.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Type.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/Support/DataTypes.h"


#include <iostream>
using std::cout;
using std::endl;

llvm::Value *LogErrorV(const char *Str);

// static std::unique_ptr<llvm::LLVMContext> TheContext;
static llvm::LLVMContext TheContext;
static std::unique_ptr<llvm::Module> TheModule;
static llvm::IRBuilder<> Builder(TheContext);
static std::map<std::string, llvm::Value *> NamedValues;

#endif
/*
 * @Author: Outsider
 * @Date: 2022-10-31 20:32:31
 * @LastEditors: Outsider
 * @LastEditTime: 2022-11-30 16:35:14
 * @Description: In User Settings Edit
 * @FilePath: /compiler/inc/codegen.hh
 */
#ifndef CODEGEN_H
#define CODEGEN_H

#include <map>
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

#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"

#include <iostream>
using std::cout;
using std::endl;

llvm::Value *LogErrorV(const char *Str);

// static std::unique_ptr<llvm::LLVMContext> TheContext;
static llvm::LLVMContext TheContext; // LLVM 上下文，主要用于生成各种对象
// TheModule是一个包含函数和全局变量的LLVM构造器。
static llvm::Module *TheModule = new llvm::Module("module", TheContext);
/*
Builder对象是一个帮助器对象，可以方便地生成LLVM指令。
IRBuilder类模板的实例跟踪插入指令的当前位置，并具有创建新指令的方法。
*/
static llvm::IRBuilder<> Builder(TheContext);
// static std::map<std::string, llvm::Value *> NamedValues;

namespace codeGen
{
    class SymbolTable
    {
        // private:
    public:
        std::map<std::string, llvm::Value *> table;
        llvm::BasicBlock *block;
        SymbolTable *parent;

    public:
        SymbolTable();
        SymbolTable(llvm::BasicBlock *block);
        llvm::BasicBlock *curBlock();
        llvm::Value *find(std::string);
        void enter(llvm::BasicBlock *block);
        void exit();
    };
}

void IRCode();
void ObjectCode(int);
#endif
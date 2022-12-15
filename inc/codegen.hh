/*
 * @Author: Outsider
 * @Date: 2022-10-31 20:32:31
 * @LastEditors: Outsider
 * @LastEditTime: 2022-12-15 12:06:28
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

llvm::Value *LogErrorV(const char *Str); // 用于报告 LLVM 代码生成过程中发现的错误

// static std::unique_ptr<llvm::LLVMContext> TheContext;
static llvm::LLVMContext TheContext; // LLVM 上下文，主要用于生成各种对象
// TheModule是一个包含函数和全局变量的LLVM构造器。
static llvm::Module *TheModule = new llvm::Module("module", TheContext);
/*
Builder对象是一个帮助器对象，可以方便地生成LLVM指令。当然不使用也可以
IRBuilder类模板的实例跟踪插入指令的当前位置，并具有创建新指令的方法。
*/
static llvm::IRBuilder<> Builder(TheContext);
// static std::map<std::string, llvm::Value *> NamedValues;

// 代码生成时所使用的符号表，与语义分析时使用的符号表不同
// 这个更多保存的时LLVM的对象，用以LLVM生成代码
namespace codeGen
{
    class SymbolTable
    {
        // private:
    public:
        std::map<std::string, llvm::Value *> table; // 存储 Value
        llvm::BasicBlock *block;                    // LLVM 基本块
        SymbolTable *parent;                        // 指向父作用域的符合表

    public:
        // 以下函数定义在Codegen.cc
        SymbolTable();
        SymbolTable(llvm::BasicBlock *block);
        llvm::BasicBlock *curBlock();        // 当前所处作用域的基本块
        llvm::Value *find(std::string);      // 在当前作用域查找LLVM value
        void enter(llvm::BasicBlock *block); // 进入作用域，更新基本块
        void exit();                         // 退出作用域
    };
}

void IRCode();     // 输出IR代码到文件
void GenCode(int); // 输出汇编或目标代码到文件
#endif
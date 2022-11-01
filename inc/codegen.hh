/*
 * @Author: Outsider
 * @Date: 2022-10-31 20:32:31
 * @LastEditors: Outsider
 * @LastEditTime: 2022-11-01 16:05:34
 * @Description: In User Settings Edit
 * @FilePath: /compiler/inc/codegen.hh
 */
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
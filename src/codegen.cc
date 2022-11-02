/*
 * @Author: Outsider
 * @Date: 2022-10-31 21:28:22
 * @LastEditors: Outsider
 * @LastEditTime: 2022-11-02 09:54:31
 * @Description: In User Settings Edit
 * @FilePath: /compiler/src/codegen.cc
 */
#include "codegen.hh"

llvm::Value *LogErrorV(const char *Str) {
  cout<<Str<<endl;;
  return nullptr;
}
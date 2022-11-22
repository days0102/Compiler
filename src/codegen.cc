/*
 * @Author: Outsider
 * @Date: 2022-10-31 21:28:22
 * @LastEditors: Outsider
 * @LastEditTime: 2022-11-22 16:00:14
 * @Description: In User Settings Edit
 * @FilePath: /compiler/src/codegen.cc
 */
#include "codegen.hh"
#include "tree.hh"

llvm::Value *LogErrorV(const char *Str)
{
    cout << Str << endl;
    return nullptr;
}

llvm::Value *Prohead::CodeGen()
{
    return nullptr;
}

llvm::Value *Expressions::CodeGen()
{
    return nullptr;
}
llvm::Value *Classbody::CodeGen()
{
    return nullptr;
}

llvm::Value *Class::CodeGen()
{
    return nullptr;
}

llvm::Value *Proclass::CodeGen()
{
    return nullptr;
}

llvm::Value *Evaluate::CodeGen()
{
    return nullptr;
}

llvm::Value *Number::CodeGen()
{
    return llvm::ConstantFP::get(TheContext, llvm::APFloat(this->val));
}

llvm::Value *Object::CodeGen()
{
    return nullptr;
}

llvm::Value *Use::CodeGen()
{
    return nullptr;
}

llvm::Value *Operation::CodeGen()
{
    llvm::Value *lv = this->left->CodeGen();
    llvm::Value *rv = this->right->CodeGen();

    switch (this->op)
    {
    case '+':
        return Builder.CreateFAdd(lv, rv, "addtmp");
    case '-':
        return Builder.CreateFSub(lv, rv, "subtmp");
    case '*':
        return Builder.CreateFMul(lv, rv, "multmp");
    case '/':
        return Builder.CreateFDiv(lv, rv, "divtmp");
    default:
        return LogErrorV("invalid binary operator");
    }
}

llvm::Value *Parameter::CodeGen()
{
    return nullptr;
}

llvm::Value *Parameters::CodeGen()
{
    return nullptr;
}

llvm::Value *Function::CodeGen()
{
    return nullptr;
}
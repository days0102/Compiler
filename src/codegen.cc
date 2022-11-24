/*
 * @Author: Outsider
 * @Date: 2022-10-31 21:28:22
 * @LastEditors: Outsider
 * @LastEditTime: 2022-11-23 19:55:44
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
codeGen::SymbolTable *cgStb;

codeGen::SymbolTable::SymbolTable() : block(nullptr), parent(nullptr) {}
codeGen::SymbolTable::SymbolTable(llvm::BasicBlock *block) : block(block),
                                                             parent(nullptr) {}
void codeGen::SymbolTable::enter(llvm::BasicBlock *block)
{
    auto stb = new codeGen::SymbolTable(block);
    stb->parent = cgStb;
    cgStb = stb;
}
void codeGen::SymbolTable::exit()
{
    cgStb = cgStb->parent;
}
llvm::Value *Program::CodeGen()
{
    // 把整个程序作为一个函数
    llvm::Type *voidType = llvm::Type::getVoidTy(TheContext);
    llvm::FunctionType *functionType = llvm::FunctionType::get(voidType, false);
    llvm::Function *function = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "program", TheModule);

    llvm::BasicBlock *block = llvm::BasicBlock::Create(TheContext, "global", function);
    llvm::BasicBlock *b = llvm::BasicBlock::Create(TheContext, "g", nullptr, block);
    cgStb = new codeGen::SymbolTable(block);

    Builder.SetInsertPoint(block);
    llvm::BinaryOperator::Create(llvm::Instruction::Add, llvm::ConstantFP::get(TheContext, llvm::APFloat(2.0)),
                                 llvm::ConstantFP::get(TheContext, llvm::APFloat(3.3)), "", block);
    llvm::BinaryOperator::Create(llvm::Instruction::Add, llvm::ConstantFP::get(TheContext, llvm::APFloat(2.0)),
                                 llvm::ConstantFP::get(TheContext, llvm::APFloat(3.3)), "", b);
    this->prohead->CodeGen();
    this->proclass->CodeGen();
    llvm::ReturnInst::Create(TheContext, block);
    return function;
}

llvm::Value *Prohead::CodeGen()
{
    return nullptr;
}

llvm::Value *Expressions::CodeGen()
{
    auto it = this->explist.begin();
    for (; it != this->explist.end(); ++it)
    {
        (*it)->CodeGen();
    }
    return nullptr;
}
llvm::Value *Classbody::CodeGen()
{
    this->explist->CodeGen();
    return nullptr;
}

llvm::Value *Class::CodeGen()
{
    llvm::BasicBlock *block = llvm::BasicBlock::Create(TheContext, this->token->name);
    cgStb->enter(block);
    this->classbody->CodeGen();
    cgStb->exit();
    return nullptr;
}

llvm::Value *Proclass::CodeGen()
{
    auto it = this->classes.begin();
    for (; it != this->classes.end(); ++it)
    {
        (*it)->CodeGen();
    }
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
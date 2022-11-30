/*
 * @Author: Outsider
 * @Date: 2022-10-31 21:28:22
 * @LastEditors: Outsider
 * @LastEditTime: 2022-11-30 15:25:10
 * @Description: In User Settings Edit
 * @FilePath: /compiler/src/codegen.cc
 */
#include "codegen.hh"
#include "tree.hh"

// 用于报告 LLVM 代码生成过程中发现的错误
llvm::Value *LogErrorV(const char *Str)
{
    cout << Str << endl;
    return nullptr;
}

// 输出LLVM IR
void IRCode()
{
    std::error_code EC;
    llvm::raw_fd_ostream dest("ir", EC, llvm::sys::fs::OF_None);
    TheModule->print(dest,nullptr);
    dest.flush();
}

void ObjectCode()
{
    auto TargetTriple = llvm::sys::getDefaultTargetTriple();
    cout << TargetTriple << endl;
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    llvm::ExecutionEngine *ee = llvm::EngineBuilder(std::unique_ptr<llvm::Module>(TheModule)).setEngineKind(llvm::EngineKind::JIT).create();

    auto func = TheModule->getFunction(llvm::StringRef("printf"));
    ee->addGlobalMapping(func, (void *)printf);

    std::string Error;
    auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);

    // Print an error and exit if we couldn't find the requested target.
    // This generally occurs if we've forgotten to initialise the
    // TargetRegistry or we have a bogus target triple.
    if (!Target)
    {
        llvm::errs() << Error;
        return;
    }

    auto CPU = "generic";
    auto Features = "";

    llvm::TargetOptions opt;
    auto RM = llvm::Optional<llvm::Reloc::Model>();
    auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);
    TheModule->setDataLayout(TargetMachine->createDataLayout());
    TheModule->setTargetTriple(TargetTriple);

    auto Filename = "output.o";
    std::error_code EC;
    llvm::raw_fd_ostream dest(Filename, EC, llvm::sys::fs::OF_None);

    if (EC)
    {
        llvm::errs() << "Could not open file: " << EC.message();
        return;
    }
    llvm::legacy::PassManager pass;
    // auto FileType = llvm::CGFT_AssemblyFile;
    auto FileType = llvm::CGFT_ObjectFile;

    if (TargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType))
    {
        llvm::errs() << "TargetMachine can't emit a file of this type";
        return;
    }

    pass.run(*TheModule);
    dest.flush();
}

codeGen::SymbolTable *cgStb;

codeGen::SymbolTable::SymbolTable() : block(nullptr), parent(nullptr) {}
codeGen::SymbolTable::SymbolTable(llvm::BasicBlock *block) : block(block),
                                                             parent(nullptr) {}

llvm::BasicBlock *codeGen::SymbolTable::curBlock()
{
    return this->block;
}

llvm::Value *codeGen::SymbolTable::find(std::string name)
{
    auto res = this->table.find(name);
    if (res != this->table.end())
        return res->second;
    else
        return this->parent->find(name);
    return nullptr;
}

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
llvm::Module *Program::CodeGen()
{
    // 把整个程序作为一个函数
    llvm::Type *voidType = llvm::Type::getVoidTy(TheContext);
    llvm::FunctionType *functionType = llvm::FunctionType::get(voidType, false);
    llvm::Function *function = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, "program", TheModule);

    llvm::BasicBlock *block = llvm::BasicBlock::Create(TheContext, "global", function);
    // llvm::BasicBlock *b = llvm::BasicBlock::Create(TheContext, "g", nullptr, block);
    cgStb = new codeGen::SymbolTable(block);

    // llvm::BinaryOperator::Create(llvm::Instruction::Add, llvm::ConstantFP::get(TheContext, llvm::APFloat(2.0)),
    //                              llvm::ConstantFP::get(TheContext, llvm::APFloat(3.3)), "", block);
    // llvm::BinaryOperator::Create(llvm::Instruction::Add, llvm::ConstantFP::get(TheContext, llvm::APFloat(2.0)),
    //  llvm::ConstantFP::get(TheContext, llvm::APFloat(3.3)), "", b);
    this->prohead->CodeGen();
    this->proclass->CodeGen();
    llvm::ReturnInst::Create(TheContext, block);
    Builder.SetInsertPoint(block);
    // Builder.CreateAnd(llvm::ConstantFP::get(TheContext, llvm::APFloat(3.3)), llvm::ConstantFP::get(TheContext, llvm::APFloat(3.3)));

    return TheModule;
}

llvm::Value *Prohead::CodeGen()
{
    if (this->token->name == "sys")
    {
        // 声明输出函数
        TheModule->getOrInsertFunction(
            "printf",
            llvm::FunctionType::get(
                llvm::IntegerType::getInt32Ty(TheContext),
                llvm::Type::getInt8Ty(TheContext)->getPointerTo(),
                true /* this is variadic func */
                ));
        // 声明输入函数
        TheModule->getOrInsertFunction(
            "scanf",
            llvm::FunctionType::get(
                llvm::IntegerType::getInt32Ty(TheContext),
                llvm::Type::getInt8Ty(TheContext)->getPointerTo(),
                true /* this is variadic func */
                ));

        TheModule->getOrInsertFunction(
            "out",
            llvm::FunctionType::get(
                llvm::IntegerType::getInt32Ty(TheContext),
                llvm::IntegerType::getDoubleTy(TheContext),
                false));
    }
    auto function = TheModule->getFunction(llvm::StringRef("out"));

    auto block = llvm::BasicBlock::Create(TheContext, "print", function);
    Builder.SetInsertPoint(block);

    auto print = TheModule->getFunction(llvm::StringRef("printf"));
    std::vector<llvm::Value *> printfArgs;
    llvm::Value *formatStrVal = Builder.CreateGlobalStringPtr("%f\n");
    printfArgs.push_back(formatStrVal);
    // std::cout<<function->arg_size()<<std::endl;
    printfArgs.push_back(function->getArg(0));
    Builder.CreateRet(Builder.CreateCall(print, printfArgs));
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
    llvm::Type *voidType = llvm::Type::getVoidTy(TheContext);
    llvm::FunctionType *functionType = llvm::FunctionType::get(voidType, false);
    llvm::Function *function = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, this->token->name, TheModule);

    llvm::BasicBlock *block = llvm::BasicBlock::Create(TheContext, "class", function);

    cgStb->enter(block);
    this->classbody->CodeGen();
    llvm::ReturnInst::Create(TheContext, cgStb->curBlock());
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
    llvm::Value *alloc = cgStb->find(this->left->name);
    Builder.CreateStore(this->right->CodeGen(), alloc);
    return nullptr;
}

llvm::Value *Number::CodeGen()
{
    return llvm::ConstantFP::get(TheContext, llvm::APFloat(this->val));
}

llvm::Value *Object::CodeGen()
{
    llvm::Value *alloc = cgStb->find(this->token->name);
    return Builder.CreateLoad(alloc);
}

llvm::Value *Use::CodeGen()
{
    llvm::AllocaInst *alloc = new llvm::AllocaInst(llvm::Type::getDoubleTy(TheContext), 0, this->exp->left->name, cgStb->curBlock());
    cgStb->table[this->exp->left->name] = alloc;
    Builder.SetInsertPoint(cgStb->curBlock());
    Builder.CreateStore(this->exp->right->CodeGen(), alloc);
    return nullptr;
}

llvm::Value *Operation::CodeGen()
{
    llvm::Value *lv = this->left->CodeGen();
    llvm::Value *rv = this->right->CodeGen();

    switch (this->op)
    {
    case '+':
        return llvm::BinaryOperator::Create(llvm::Instruction::FAdd, lv,
                                            rv, "", cgStb->curBlock());
        // return Builder.CreateFAdd(lv, rv, "addtmp");
    case '-':
        return llvm::BinaryOperator::Create(llvm::Instruction::FSub, lv,
                                            rv, "", cgStb->curBlock());
        // return Builder.CreateFSub(lv, rv, "subtmp");
    case '*':
        return llvm::BinaryOperator::Create(llvm::Instruction::FMul, lv,
                                            rv, "", cgStb->curBlock());
        // return Builder.CreateFMul(lv, rv, "multmp");
    case '/':
        return llvm::BinaryOperator::Create(llvm::Instruction::FDiv, lv,
                                            rv, "", cgStb->curBlock());
        // return Builder.CreateFDiv(lv, rv, "divtmp");
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
llvm::Value *Call::CodeGen()
{
    auto func = TheModule->getFunction(llvm::StringRef(this->token->name));
    // std::vector<llvm::Value *> args;
    // for (auto arg : this->args)
    // {
    //     args.push_back(arg->CodeGen());
    // }
    auto it = this->args.begin();
    Builder.CreateCall(func, (*it)->CodeGen());
    return nullptr;
}
/*
 * @Author: Outsider
 * @Date: 2022-10-31 21:28:22
 * @LastEditors: Outsider
 * @LastEditTime: 2022-12-15 15:12:58
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
    std::error_code EC; // 错误代码
    // 创建LLVM的文件流
    llvm::raw_fd_ostream dest("output.ir", EC, llvm::sys::fs::OF_None);
    if (EC)
    {
        llvm::errs() << "Could not open file: " << EC.message();
        return;
    }
    // 把IR代码写入文件流中
    TheModule->print(dest, nullptr);
    dest.flush();
}

/**
  LLVM 生成代码输出到文件
  type=0 汇编代码
  type=1 二进制代码
 */
void GenCode(int type)
{
    // 以下大部分代码来源于 LLVM Tutorial

    auto TargetTriple = llvm::sys::getDefaultTargetTriple();
    // cout << TargetTriple << endl;
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    // 将 C 库函数printf映射到自己声明的printf函数中，用以实现在自己语言中实现输出
    llvm::ExecutionEngine *ee = llvm::EngineBuilder(
                                    std::unique_ptr<llvm::Module>(TheModule))
                                    .setEngineKind(llvm::EngineKind::JIT)
                                    .create();
    // 获取IR中声明的printf函数
    auto print = TheModule->getFunction(llvm::StringRef("printf"));
    ee->addGlobalMapping(print, (void *)printf); // 生成映射
    auto scan = TheModule->getFunction(llvm::StringRef("scanf"));
    ee->addGlobalMapping(scan, (void *)scanf);

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
    auto TargetMachine = Target->createTargetMachine(TargetTriple,
                                                     CPU, Features, opt, RM);
    TheModule->setDataLayout(TargetMachine->createDataLayout());
    TheModule->setTargetTriple(TargetTriple);

    std::string Filename = "output.o";
    llvm::CodeGenFileType FileType;
    if (type == 0)
    {
        FileType = llvm::CGFT_AssemblyFile;
        Filename = "output.S";
    }
    else if (type == 1)
        FileType = llvm::CGFT_ObjectFile;

    std::error_code EC;
    llvm::raw_fd_ostream dest(Filename, EC, llvm::sys::fs::OF_None);

    if (EC)
    {
        llvm::errs() << "Could not open file: " << EC.message();
        return;
    }
    llvm::legacy::PassManager pass;

    if (TargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType))
    {
        llvm::errs() << "TargetMachine can't emit a file of this type";
        return;
    }

    pass.run(*TheModule);
    dest.flush();
}

// 代码生成时所使用的符号表
codeGen::SymbolTable *cgStb;
// 构造函数
codeGen::SymbolTable::SymbolTable() : block(nullptr), parent(nullptr) {}
codeGen::SymbolTable::SymbolTable(llvm::BasicBlock *block) : block(block),
                                                             parent(nullptr) {}
// 获取当前作用域的基本块
llvm::BasicBlock *codeGen::SymbolTable::curBlock()
{
    return this->block;
}
// 在当前作用域的符号表中查找值
// 找到返回相应值，没有返回null
llvm::Value *codeGen::SymbolTable::find(std::string name)
{
    auto res = this->table.find(name);
    if (res != this->table.end())
        return res->second;
    if (this->parent != nullptr) // 往父级作用域查找
        return this->parent->find(name);
    return nullptr;
}
// 进入一个新的作用域，并将当前基本块改为 block
void codeGen::SymbolTable::enter(llvm::BasicBlock *block)
{
    auto stb = new codeGen::SymbolTable(block);
    stb->parent = cgStb; // 指向其父作用域
    cgStb = stb;
    Builder.SetInsertPoint(block);
}
// 退出当前作用域
void codeGen::SymbolTable::exit()
{
    cgStb = cgStb->parent;
    Builder.SetInsertPoint(cgStb->curBlock());
}

// 整个程序的代码生成入口
// 返回LLVM Moudle
llvm::Module *Program::CodeGen()
{
    // 把整个程序作为一个函数

    /*  创建一个void类型，getVoidTy接收一个llvm context做参数
        可以通过 Type::getXXXTy获取其它的值（定义在头文件源码Type.h中）
    */
    llvm::Type *voidType = llvm::Type::getVoidTy(TheContext);
    /*  获取一个void返回值类型的函数
        FunctionType::get 有两个重载，一个接收3个参数，一个接收2个
        2个参数第一个为返回值类型，第二个为是否是可变参数函数
        3个参数第一个为返回值类型，第二为参数列表，第三为是否是可变参数函数
    */
    llvm::FunctionType *functionType = llvm::FunctionType::get(voidType, false);
    /*  创建一个外部链接的函数，有两重载一个有默认值一个没有
        参数1函数类型，参数2为函数链接类型，参数3函数名，参数4指定生成到哪个模块
    */
    llvm::Function *function = llvm::Function::Create(
        functionType, llvm::GlobalValue::ExternalLinkage, "main", TheModule);
    /*  为函数创建一个基本块
        参数2块名，参数3块所属的函数(如果参数4为nullptr)，参数4块所属的块
    */
    llvm::BasicBlock *block = llvm::BasicBlock::Create(TheContext, "global", function);
    // llvm::BasicBlock *b = llvm::BasicBlock::Create(TheContext, "g", nullptr, block);
    cgStb = new codeGen::SymbolTable(block); // 创建作用域

    // llvm::BinaryOperator::Create(llvm::Instruction::Add, llvm::ConstantFP::get(TheContext, llvm::APFloat(2.0)),
    //                              llvm::ConstantFP::get(TheContext, llvm::APFloat(3.3)), "", block);
    // llvm::BinaryOperator::Create(llvm::Instruction::Add, llvm::ConstantFP::get(TheContext, llvm::APFloat(2.0)),
    //  llvm::ConstantFP::get(TheContext, llvm::APFloat(3.3)), "", b);
    if (this->prohead != nullptr)
        this->prohead->CodeGen();
    if (this->proclass != nullptr)
        this->proclass->CodeGen();
    llvm::ReturnInst::Create(TheContext, block); // 在block中添加函数返回值
    Builder.SetInsertPoint(block);               // 设置builder的插入点
    // Builder.CreateAnd(llvm::ConstantFP::get(TheContext, llvm::APFloat(3.3)), llvm::ConstantFP::get(TheContext, llvm::APFloat(3.3)));

    return TheModule;
}

// 程序头using语句的代码生成
llvm::Value *Prohead::CodeGen()
{
    // 声明语言的运行时系统，用于输入和输出
    if (this->token->name == "sys")
    {
        /* 在本语言中输出函数输出实现思路
           在自己定义的语言中，
        */

        // 声明printf输出函数,将c库printf函数映射到这声明的函数中
        TheModule->getOrInsertFunction(
            "printf",
            llvm::FunctionType::get(
                llvm::IntegerType::getInt32Ty(TheContext),         /* 返回值 */
                llvm::Type::getInt8Ty(TheContext)->getPointerTo(), /* 参数 */
                true                                               /* 可变参数 */
                ));
        // 声明输入函数,将c库scanf映射到这声明的函数中
        TheModule->getOrInsertFunction(
            "scanf",
            llvm::FunctionType::get(
                llvm::IntegerType::getInt32Ty(TheContext),
                llvm::Type::getInt8Ty(TheContext)->getPointerTo(),
                true));

        /* sys中的输出函数 out()
           out目前只接收一个参数，参数可以是变量或表达式
           后面会通过IR实现该函数(调用声明printf函数)
        */
        TheModule->getOrInsertFunction(
            "out",
            llvm::FunctionType::get(
                llvm::IntegerType::getInt32Ty(TheContext),
                llvm::IntegerType::getDoubleTy(TheContext),
                false));
        /* sys中的输出函数 in()
            in目前不接受参数，返回从控制台输入的值
            后面会通过IR实现该函数(调用scanf函数)
        */
        TheModule->getOrInsertFunction(
            "in",
            llvm::FunctionType::get(
                llvm::IntegerType::getDoubleTy(TheContext),
                llvm::NoneType::None,
                false));
    }
    // 实现out函数
    auto function = TheModule->getFunction(llvm::StringRef("out"));
    auto block = llvm::BasicBlock::Create(TheContext, "print", function);
    Builder.SetInsertPoint(block);
    // 调用printf函数
    auto print = TheModule->getFunction(llvm::StringRef("printf"));
    std::vector<llvm::Value *> printfArgs; // 参数列表
    llvm::Value *formatStrVal = Builder.CreateGlobalStringPtr("%f\n");
    printfArgs.push_back(formatStrVal);
    // std::cout<<function->arg_size()<<std::endl;
    printfArgs.push_back(function->getArg(0));                // out的第一个参数作为printfmap的第二个参数
    Builder.CreateRet(Builder.CreateCall(print, printfArgs)); // 调用printfmap

    // 实现in函数
    auto func = TheModule->getFunction(llvm::StringRef("in"));
    auto inblock = llvm::BasicBlock::Create(TheContext, "scan", func);
    Builder.SetInsertPoint(inblock);
    auto scan = TheModule->getFunction(llvm::StringRef("scanf"));
    std::vector<llvm::Value *> scanfArgs;
    llvm::Value *fStrVal = Builder.CreateGlobalStringPtr("%d");
    scanfArgs.push_back(fStrVal);
    // 用一个临时变量保存输入的值
    llvm::AllocaInst *alloc = new llvm::AllocaInst(llvm::Type::getDoubleTy(TheContext), 0, "temp", inblock);
    scanfArgs.push_back(alloc);
    Builder.CreateCall(scan, scanfArgs);
    // 返回输入的值
    Builder.CreateRet(Builder.CreateLoad(alloc));
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
    // 把一个class当成是一个函数
    llvm::Type *voidType = llvm::Type::getVoidTy(TheContext);
    llvm::FunctionType *functionType = llvm::FunctionType::get(voidType, false);
    llvm::Function *function = llvm::Function::Create(
        functionType,
        llvm::GlobalValue::ExternalLinkage, this->token->name, TheModule);
    llvm::BasicBlock *block = llvm::BasicBlock::Create(TheContext, "class", function);
    // 类内是一个作用域
    cgStb->enter(block); // 进入作用域
    this->classbody->CodeGen();
    llvm::ReturnInst::Create(TheContext, cgStb->curBlock()); // 函数返回
    cgStb->exit();                                           // 退出作用域
    // 调用函数
    std::vector<llvm::Value *> args;
    Builder.CreateCall(function, args);
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

// 赋值表达式
llvm::Value *Evaluate::CodeGen()
{
    // 获取变量实例 (经语义分析semantic()后alloc非nullptr)
    llvm::Value *alloc = cgStb->find(this->left->name);
    // 赋值，将右边的值赋给左边变量
    Builder.CreateStore(this->right->CodeGen(), alloc);
    return nullptr;
}

llvm::Value *Number::CodeGen()
{
    // 获取一个常量实例(也可以使用Builder获取)
    return llvm::ConstantFP::get(TheContext, llvm::APFloat(this->val));
    // Builder.getInt32(36);
}

llvm::Value *Object::CodeGen()
{
    // 从符号表中获取变量
    llvm::Value *alloc = cgStb->find(this->token->name);
    return Builder.CreateLoad(alloc); // 返回变量的值
}

llvm::Value *Use::CodeGen()
{
    // 声明变量
    llvm::AllocaInst *alloc = new llvm::AllocaInst(llvm::Type::getDoubleTy(TheContext), 0, this->exp->left->name, cgStb->curBlock());
    // 添加至符号表
    cgStb->table[this->exp->left->name] = alloc;
    Builder.SetInsertPoint(cgStb->curBlock());
    // 赋值
    Builder.CreateStore(this->exp->right->CodeGen(), alloc);
    return nullptr;
}

// + - * / 表达式
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
// todo
llvm::Value *Parameter::CodeGen()
{
    return nullptr;
}
// todo
llvm::Value *Parameters::CodeGen()
{
    return nullptr;
}
// todo
llvm::Value *Function::CodeGen()
{
    return nullptr;
}
llvm::Value *Call::CodeGen()
{
    // 获取函数
    auto func = TheModule->getFunction(llvm::StringRef(this->token->name));
    if (func == nullptr)
    {
        cout << "No this function " << this->token->name << endl;
        return nullptr;
    }
    std::vector<llvm::Value *> args; // 函数参数列表
    for (auto arg : this->args)
    {
        args.push_back(arg->CodeGen());
    }
    return Builder.CreateCall(func, args);
    // auto it = this->args.begin();
    // if (it != this->args.end())
    //     return Builder.CreateCall(func, (*it)->CodeGen());
    // else
    //     return Builder.CreateCall(func, llvm::NoneType::None);
}
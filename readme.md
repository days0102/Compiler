<!--
 * @Author: Outsider
 * @Date: 2022-11-30 20:28:10
 * @LastEditors: Outsider
 * @LastEditTime: 2022-12-16 16:36:25
 * @Description: In User Settings Edit
 * @FilePath: /compiler/readme.md
-->
# 测试环境
## flex
### flex 版本 flex 2.5.4  (apt install flex-old)
## llvm
### (apt install llvm)
## clang
### (apt install clang)
## bison
### bison 版本 bison 3.5.1
----
# 项目结构
|文件或目录|说明|
|:--:|:--:|
|bison.y|bison 文件|
|compile_commands.json|vscode-c++配 置文件
|.vscode|vscode配置文件|
|compiler|shell脚本,接收参数执行parser   
|flex.l|flex文件|
|inc|包含的头文件|
|lib|打印语法树库|
|llvm|llvm头文件源码，在这里无实质作用，仅为了生成vscode配置文件，llvm代码阅读时方便代码跳转|
|llvm-c|llvm头文件源码，在这里无实质作用，仅为了生成vscode配置文件，llvm代码阅读时方便代码跳转|
|Makefile|Makefile文件，里面包含的大量的执行命令和说明
|other|空|
|readme|说明书|
|src|项目主要代码|
|text.dd|需要编译的代码|
---
# 项目主要文件说明
|文件|说明|
|:--:|:--:|
|inc/codegen.hh|代码生成头文件，里面包含llvm的头文件，同时包含llvm代码生成时使用的符号表|
|src/codegen.cc|包含每个类继承基类纯虚函数CodeGen()的实现|
|inc/print.hh|打印语法树结构的头文件，包含了lib中的头文件|
|src/print.hh|包含每个类继承基类纯虚函数getNode()的实现|
|inc/semantic.hh|简单的语义分析，只分析了部分的节点|
|src/semantic.cc|语义分析的实现代码|
|inc/symbols.hh|包含了语义分析使用的符号表|
|src/symbols.cc|符号表代码的实现，添加符号，进入作用域等|
|inc/tokens.hh|token类的实现|
|src/tokens.cc|token类函数的实现代码|
|inc/tree.hh|语法树结构，其中包含了大量的继承与treenode和Expression的类|
|src/tree.cc|语法树类函数的实现|
|src/main.cc|整个程序的主函数|
---

# 符号表说明
在实现整个程序中设计使用了两种符号表
1. 语义分析时使用的符号表（代码在symbols.cc和symbols.hh中）
``` c++
class SymbolTable
{
private:
    std::map<std::string, Symbol *> table;
    /*
    一个作用域，可能有多个子作用域
    一个作用域，只能有一个直接父作用域
    */
    SymbolTable *parent;                          // 外部作用域
    std::map<std::string, SymbolTable *> childen; // 内部作用域

public:
    SymbolTable();
    SymbolTable(SymbolTable *);
    Symbol *globalFind(std::string);
    Symbol *find(std::string); // 在当前作用域查找
    Symbol *add(std::string, Symbol *);
    SymbolTable *enter(std::string); // 进入一个作用域
    SymbolTable *exit();             // 退出作用域

    void show(int level); // 输出符号表
};
```
2. 代码生成时所使用的符号表
```c++
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
```

# Makefile说明
|参数|作用|
|:--:|:--:|
|all|编译产生整个程序 parser|
|run|运行parser编译文件text.dd,并输出所有文件和内容|
|ir|输出ir代码到output.ir文件|
|tree|输出语法树结构到tree中|
|assembly|输出汇编代码到output.S文件|
|assembly-clean|输出简洁汇编代码到output.s文件|
|object|输出目标代码到output.o。这是未链接的文件，无法执行|
|out|通过输出的汇编代码，使用clang链接成可执行文件a.out。通过llvm得到的这个可执行文件不能使用gdb调试而应使用lldb调试|

# 定义的语言说明
|关键字|作用|
|:--:|:--:|
|class|暂时只是作为一个作用域|
|using|暂时只能using sys,包含内置的输出函数out()|
|use|定义变量|
|ftn|函数定义未实现|

|内置函数|说明|例子|
|:--:|:--:|:--:|
|out()|控制台输出函数。接受一个参数，参数可以是表达式，数字或已定义变量。使用前要using sys.|out(1+4+3)|
in()|从控制台接受输入，返回输入数值。这个函数实现暂时还有bug|a=in()|
---
> linux下使用控制台输出需要调用write系统调用，单纯的实现输出函数比较麻烦
这里使用在编译时将c库函数printf映射到要预先定义的函数中，
（codegen.cc文件GenCode函数中映射），再在实现的输出函数中调用预定义函数实现输出
> 在自己定义的语言中，定义一个占位函数printf,该函数在这里不实现（相当于定义）
然后定义一个系统输出函数out,在这里实现该函数，函数主要调用之前定义的printf
最后将c库函数printf全局映射到这里定义的printf的函数中
定义的系统输出函数out则在实现时调用先前定义的printf函数以实现输出
in的实现同理，相关代码在codegen.cc文件中
---
```
@@ 这是一条单行注释
using sys @@头文件，要么包含要么不包含。内置out函数

@@ 整个程序会从前往后执行class
class first{     @@ 目前一个类仅作为一个作用域
    5-3-1+1+3
    use a = 1   @@ 定义一个变量a，暂不支持连续赋值
    use b=a+4   @@ 不允许使用逗号表达式use a=1,b=2
    out(b)      @@ 输出b的值
    out(3+5)
}
class abc{      @@ 一个程序中可以含有多个class
    use a=3+5
    use name=a+a
    a=name+a
    out(a)
}
```

**程序实现的编译器不专注于性能问题，申请的内存没有特别回收**

**使用llvm生成代码时并不进行代码优化**

**更多详细内容在代码注释中**

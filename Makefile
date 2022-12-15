-include */*.d

BFLAGS = -d

# CXX=g++ #使用LLVM时会出错
CXX=clang++
CXXFLAGES=-g -MD
#使用clang++时vscode调试无法显示std::string的值
#设置-fno-limit-debug-info以显示string值
#(vscode将gdb改为lldb是一种方案,linux下安装libstdc++也可能可行,-D_GLIBCXX_DEBUG也可能可行)
CXXFLAGES+=-fno-limit-debug-info
CXXFLAGES+=-Wno-write-strings
CXXFLAGES+=-Wno-deprecated-register#禁止c++17 register 警告
# (未知 bug)当使用LLVM时-std=c++17无法生效
CXXFLAGES+=-std=c++17# 兼容树结构打印库
CXXFLAGES+=-I. -Iinc -Ilib
CXXFLAGES+=-Illvm
CXXFLAGES+=-Illvm-c
# CXXFLAGES+= -I/usr/include/llvm-10
# CXXFLAGES+= -I/usr/include/llvm-c-10
# LLVM 参数
CXXFLAGES+=`llvm-config --cxxflags --ldflags --system-libs --libs core`#--link-static
CXXFLAGES+=-DLLVM_DISABLE_ABI_BREAKING_CHECKS_ENFORCING

SRCS =  src/main.cc \
		src/tokens.cc \
		src/tree.cc \
		src/symbols.cc \
		src/codegen.cc \
		src/print.cc \
		src/semantic.cc \
		bison.tab.cc \

LIBS = lib/array.cpp \
	   lib/string_utils.cpp \
	   lib/tree_printer.cpp
	
OBJS = $(SRCS:.cc=.o)
OBJS += $(SRCS:.c=.o)

TARGET= parser

all: $(SRCS)
	${CXX} ${CXXFLAGES} ${SRCS} ${LIBS} -lfl -o ${TARGET}

lex.yy.c: flex.l
	flex flex.l

bison.tab.cc: lex.yy.c bison.y
	bison -d bison.y
	mv bison.tab.c bison.tab.cc
	mv bison.tab.h bison.tab.hh

.PRECIOUS: %.o # 保留中间过程的 .o 文件
# %.o : %.cc
# 	${CXX} ${CXXFLAGES} -c -o $@ $<
# %.o : %.c
# 	${CXX} ${CXXFLAGES} -c -o $@ $<

# bison.tab.c: bison.y
# 	bison ${BFLAGS} bison.y

# flex: flex.l
# 	flex flex.l

# bison: bison.y
# 	bison ${BFLAGS} bison.y

clean:
	rm *.tab.* lex.yy.c parser ir ir.bc ir.s output.o output.S output.s tree  a.out 2> /dev/null || :

run:${TARGET} ${SRCS}
	@./parser text.dd -A -a

# 输出llvm ir代码
ir:${TARGET} ${SRCS}
	@./parser text.dd -I
	@echo "LLVM IR file ir has been generated!"

# 输出汇编代码
assembly:${TARGET} ${SRCS}
	@./parser text.dd -S
	@echo "Assembly file output.S has been generated!"

# 由llvm目标代码反汇编等到简洁的代码
assembly-clean:${TARGET} ${SRCS}
	@./parser text.dd -o
	@llvm-objdump -d output.o > output.s

# 输出目标代码
object:${TARGET} ${SRCS}
	@./parser text.dd -o
	@echo "Object file output.o has been generated!"

# 通过clang链接输出可执行文件
out:${TARGET} ${SRCS}
	@./parser text.dd -S
	@clang output.S -o a.out
	@echo "Executable file a.out has been generated!"

# 输出语法树
tree:${TARGET} ${SRCS}
	@./parser text.dd -t > tree
	@echo "Tree file a.out has been generated!"
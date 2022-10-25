-include */*.d

BFLAGS = -d

CXX=g++
CXXFLAGES=-g -Wno-write-strings -MD -Iinc -std=c++11

SRCS =  src/main.cc \
		src/tokens.cc \
		src/tree.cc \
		src/symbols.cc \
		bison.tab.cc \
	
OBJS = $(SRCS:.cc=.o)
OBJS += $(SRCS:.c=.o)

TARGET= parser

all: $(SRCS)
	${CXX} ${CXXFLAGES} ${SRCS} -lfl -o ${TARGET}

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
	rm *.tab.* lex.yy.c parser 2> /dev/null || :

run:${TARGET} ${SRCS}
	@./parser text.dd
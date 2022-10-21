BFLAGS = -d

CXX=g++
CXXFLAGES=-g -Wno-write-strings -MD

all: bison.tab.cc main.cc
	${CXX} ${CXXFLAGES} main.cc bison.tab.cc -o parser -lfl

lex.yy.c: flex.l
	flex flex.l

bison.tab.cc: lex.yy.c bison.y
	bison -d bison.y
	mv bison.tab.c bison.tab.cc

bison.tab.c: bison.y
	bison ${BFLAGS} bison.y

flex: flex.l
	flex flex.l

bison: bison.y
	bison ${BFLAGS} bison.y

clean:
	rm *.tab.* lex.yy.c parser 

run:parser
	@./parser text.dd

SRC=array.cpp \
	string_utils.cpp \
	tree_printer.cpp \
	test.cc	\

# TARGET=$(SRC:.cc=.o)

target: ${SRC}
	clang++ -std=c++14 -g ${SRC} -o target

# ${TARGET}:${SRC}
# 	clang++ -std=c++17 -g $@ -o $<

clean:
	rm target